#pragma once

#include <string>
#include <stdexcept>
#include <cctype>
#include <functional>
#include <ut/type_traits.hxx>
#include <ut/array_view.hxx>


namespace cl
{
	namespace internal
	{
		template< typename >
		struct nullary_tag_t
		{
			
		};
		
		template< typename T, typename >
		class unary_tag_t
		{
			public:
				unary_tag_t(const T& p_val)
					: m_Value(p_val)
				{

				}

			public:
				const T& value() const
				{
					return m_Value;
				}

			private:
				T m_Value;
		};
		
		template< typename T, typename U, typename >
		class binary_tag_t
		{
			public:
				binary_tag_t(const T& p_first, const U& p_second)
					: m_First(p_first), m_Second(p_second)
				{

				}

			public:
				const T& first() const
				{
					return m_First;
				}
				
				const U& second() const
				{
					return m_Second;
				}

			private:
				T m_First;
				U m_Second;
		};
	}
	
	
	namespace internal
	{
		// Ignore case. Used by enum_argument<E>.
		using ignore_case_t = nullary_tag_t<struct _ignore_case>;

		// Flags argument as "required". If it is not supplied, an exception is thrown.
		using required_t = nullary_tag_t<struct _required>;

		// Flags argument as "optional". This has no effect since all arguments are optional
		// by default. This is used to make intented meaning clear.
		using optional_t = nullary_tag_t<struct _optional>;

		// Flags argument as "inverse". This is used in flags that are "false" when they are present,
		// for example --NoCopy and --Copy sharing a reference.
		using inverse_t = nullary_tag_t<struct _inverse>;

		// Instructs the argument to clamp to range instead of throwing an exception.
		// This is intended to work with min<T>/max<T>.
		using clamp_t  = nullary_tag_t<struct _clamp>;

		// Sets the id of an argument. The id is used to access the stored value later on.
		using id_t = unary_tag_t<::std::size_t, struct _id>;

		// Sets the "long name" of given argument.
		using long_name_t = unary_tag_t<::std::string, struct _long_name>;

		// Sets the "short name" of given argument. (optional)
		using short_name_t = unary_tag_t<char, struct _short_name>;
		
		// Sets the description of given argument.
		using description_t = unary_tag_t<::std::string, struct _description>;
		
		// Sets the category of given argument.
		using category_t = unary_tag_t<::std::string, struct _category>;

		// DEPRECATED
		using exclusive_t = unary_tag_t<::std::string, struct _exclusive>;

		// Sets the default value of given argument.
		template< typename T >
		using default_value_t = unary_tag_t<T, struct _default_value>;
		
		// A callback for a supplied value.
		template< typename T >
		using callback_t = unary_tag_t<::std::function<void(const T&)>, struct _callback>;

		// Sets the maximum value of given argument.
		template< typename T >
		using max_t = unary_tag_t<T, struct _max>;

		// Sets the minimum value of given argument.
		template< typename T >
		using min_t = unary_tag_t<T, struct _min>;
				
		template< typename T >
		using value_t = unary_tag_t<T, struct _value>;
		
		template< typename T >
		using switch_value_t = unary_tag_t<T, struct _swvalue>;
		
		template< typename T >
		using reference_t = unary_tag_t<T*, struct _reference>;

		template< typename T >
		using view_t = unary_tag_t<::ut::array_view<T>, struct _view>;
		
		template< typename T, typename U >
		using key_value_t = binary_tag_t<T, U, struct _key_value>;

		template< typename T >
		using range_t = binary_tag_t<T, T, struct _range>;
	}


	template< typename T >
	static internal::min_t<T> min(T p_val)
	{
		return internal::min_t<T>(p_val);
	}

	template< typename T >
	static internal::max_t<T> max(T p_val)
	{
		return internal::max_t<T>(p_val);
	}
	
	template< typename T >
	static internal::range_t<T> range(T p_min, T p_max)
	{
		return internal::range_t<T>(p_min, p_max);
	}

	template< typename T >
	static internal::reference_t<T> reference(T& p_val)
	{
		return internal::reference_t<T>(::std::addressof(p_val));
	}
	
	template< typename T > /* TODO maybe rename to "reference_view" or "view" if this doesnt work */
	static internal::view_t<T> reference(::ut::array_view<T> p_val)
	{
		return internal::view_t<T>(p_val);
	}

	template< typename T >
	static internal::default_value_t<T> default_value(const T& p_val)
	{
		return internal::default_value_t<T>(p_val);
	}

	static internal::long_name_t long_name(const std::string& p_str)
	{
		return internal::long_name_t(p_str);
	}

	static internal::short_name_t short_name(char p_chr)
	{
		return internal::short_name_t(p_chr);
	}
	
	static internal::description_t description(const std::string& p_str)
	{
		return internal::description_t(p_str);
	}
	
	static internal::category_t category(const std::string& p_str)
	{
		return internal::category_t(p_str);
	}

	static internal::exclusive_t exclusive(const std::string& p_str)
	{
		if (p_str.empty())
			throw std::runtime_error("Invalid exclusive group name! (empty string not allowed)");
		else return internal::exclusive_t(p_str);
	}

	template< typename E >
	static internal::key_value_t<::std::string, E> enum_value(const std::string& p_key, E p_val)
	{
		return internal::key_value_t<::std::string, E>(p_key, p_val);
	}
	
	template< typename T, typename U >
	static internal::key_value_t<T, U> key_value(const T& p_key, const U& p_val)
	{
		return internal::key_value_t<T, U>(p_key, p_val);
	}
	
	template< typename T >
	static internal::value_t<T> value(const T& p_val)
	{
		return internal::value_t<T>(p_val);
	}
	
	template< typename T >
	static internal::switch_value_t<T> switch_value(const T& p_val)
	{
		return internal::switch_value_t<T>(p_val);
	}
	
	template< typename E >
	static internal::key_value_t<::std::string, E> enum_key_value(const std::string& p_key, E p_val)
	{
		return internal::key_value_t<::std::string, E>(p_key, p_val);
	}
	
	template< typename E >
	static internal::value_t<E> enum_value(E p_val)
	{
		return internal::value_t<E>(p_val);
	}

	template< typename T >
	static internal::id_t id(T t)
	{
		static_assert(ut::is_static_castable<T, std::size_t>::value,
			"ID type has to be castable to size_t!");

		return internal::id_t(static_cast<std::size_t>(t));
	}
	
	template< typename T >
	static internal::callback_t<T> callback(::std::function<void(const T&)> p_func)
	{
		return internal::callback_t<T>(p_func);
	}

	// Constant instances for simple tags
	static constexpr internal::inverse_t inverse{};
	static constexpr internal::optional_t optional{};
	static constexpr internal::required_t required{};
	static constexpr internal::clamp_t clamp{};
	static constexpr internal::ignore_case_t ignore_case{};
}