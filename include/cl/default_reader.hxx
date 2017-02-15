#pragma once

#include <sstream>
#include <type_traits>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <ut/type_traits.hxx>
#include <ut/throwf.hxx>
#include <ut/final_act.hxx>
#include <ut/meta.hxx>

namespace cl
{
	namespace internal
	{
		struct integral_tag_t{};
		struct signed_tag_t{};
		struct floating_tag_t{};
		struct boolean_tag_t{};
		struct string_tag_t{};
		struct no_tag_t{};
		
		template< typename T >
		using is_boolean = ::std::is_same<T, bool>;
		
		template< typename T >
		using is_string = ::std::is_same<T, ::std::string>;
		
		template< typename T >
		using value_category_t =
			ut::category_of_t<
				T,
				ut::category<boolean_tag_t, is_boolean>,
				ut::category<integral_tag_t, ::std::is_integral>,
				ut::category<floating_tag_t, ::std::is_floating_point>,
				ut::category<string_tag_t, is_string>,
				ut::category<no_tag_t>
			>;		
	};
	
	
	namespace internal
	{
		// Detect if T can be retrieved from an input stream by the default reader.
		template< typename T >
		using is_default_readable_check = decltype(
			::std::declval<::std::istream&>() >> ::std::declval<T&>()
		);
		
		template< typename T >
		using is_default_readable = ut::is_detected<is_default_readable_check, T>;
		
		template< typename T >
		constexpr bool is_default_readable_v = is_default_readable<T>::value;
		//
			
	
		// Catch-all specialization: Tries to input using stringstream.
		template< typename T, typename >
		class default_reader_impl
		{
			static_assert(is_default_readable_v<T>,
				"T needs to provide overload for stream extraction in order to use default_reader!");
		
			static_assert(::std::is_default_constructible<T>::value,
				"T needs to provide a default constructor in order to use default_reader!");
		
			public:
				default_reader_impl() = default;
				default_reader_impl(const default_reader_impl&) = delete;
				default_reader_impl(default_reader_impl&&) = default;
				default_reader_impl& operator=(const default_reader_impl&) = delete;
				default_reader_impl& operator=(default_reader_impl&&) = default;
				
			public:
				auto read(const ::std::string& p_str)
					-> T
				{
					// Guarantuee that the string stream will be cleared after use.
					auto t_guard = ut::finally(
						[this]()
						{
							m_Stream.clear();
							m_Stream.str(::std::string{});
						}
					);
					
					T t_tmp{ };
								
					// Extract from string using string_stream.
					m_Stream << p_str;								
					m_Stream >> t_tmp;
					
					// Throw if failure, otherwise return value.
					if(m_Stream.fail())
						ut::throwf<::std::runtime_error>("Failed to parse \"%s\": Invalid value!", p_str);
					else return t_tmp;
				}
		
			private:
				::std::stringstream m_Stream;
		};
		
		
		// For strings there is nothing to be done.
		template< typename T >
		class default_reader_impl <T, string_tag_t>
		{
			public:
				auto read(const ::std::string& p_str)
					-> T
				{
					return { p_str };
				}
		};
		//
		
		
		// Boolean values
		template< typename T >
		class default_reader_impl <T, boolean_tag_t>
		{
			public:
				auto read(const ::std::string& p_str)
					-> T
				{			
					T t_tmp{ };
					
					// Create lower-case copy of source string
					::std::string t_str{p_str};
					::std::transform(t_str.begin(), t_str.end(), t_str.begin(), ::tolower);
					
					// Compare case-insensitive					
					if(t_str == "true" || t_str == "1")
						t_tmp = true;
					else if(t_str == "false" || t_str == "0")
						t_tmp = false;
					else ut::throwf<::std::runtime_error>("Failed to parse \"%s\" as boolean value: Invalid input!", p_str);
										
					return t_tmp;
				}			
		};
		//
		
		// Floating point values
		// We are just using the default implementation here since writing correct and fast
		// float parsing code is hard.
		
		
		// Integral values
		template< typename T >
		class default_reader_impl <T, integral_tag_t>
		{
			public:
				auto read(const ::std::string& p_str)
					-> T
				{					
					try
					{
						// Try to convert to biggest integral type
						auto t_val = read_impl(::std::is_signed<T>{}, p_str);
						
						// Check range
						if (t_val >= ::std::numeric_limits<T>::min() &&
							t_val <= ::std::numeric_limits<T>::max())
						{
							return static_cast<T>(t_val);
						}
						else
						{
							ut::throwf<::std::out_of_range>(
								"Failed to parse \"%s\" as integral value: Value out of range!",
								p_str
							);
						}
					}
					catch(const ::std::invalid_argument&)
					{
						ut::throwf<::std::runtime_error>(
							"Failed to parse \"%s\" as integral value: Invalid format!",
							p_str
						);
					}
					catch(const ::std::out_of_range&)
					{
						ut::throwf<::std::runtime_error>(
							"Failed to parse \"%s\" as integral value: Value out of range!",
							p_str
						);
					}
				}
				
			private:
				// Unsigned
				auto read_impl(::std::false_type, const ::std::string& p_str)
					-> ::std::uintmax_t
				{
					return ::std::stoull(p_str, nullptr, 0);
				}
				
				// Signed
				auto read_impl(::std::true_type, const ::std::string& p_str)
					-> ::std::intmax_t
				{
					return ::std::stoll(p_str, nullptr, 0);
				}
				
		};
	}
	
	
	template< typename T >
	using default_reader = internal::default_reader_impl<T, internal::value_category_t<T>>;
}