#pragma once

#include <memory>
#include <stdexcept>
#include <exception>
#include <string>
#include <type_traits>
#include <map>
#include <ut/type_traits.hxx>
#include <ut/observer_ptr.hxx>
#include <ut/throwf.hxx>
#include <ut/meta.hxx>

#include "tags.hxx"
#include "boolean_argument.hxx"
#include "integer_argument.hxx"
#include "positional_argument.hxx"
#include "argument_base.hxx"
#include "free_argument.hxx"
#include "string_argument.hxx"
#include "command_base.hxx"
#include "enum_argument.hxx"
#include "handler_data.hxx"
#include "options.hxx"
#include "meta.hxx"

namespace cl
{
	template< typename TEnum >
	class command
		: public internal::command_base
	{
		using this_type = command<TEnum>;
		using enum_type = TEnum;
		using base_type = command_base;
	
		public:
			template< typename... Targs,
					  typename = ::std::enable_if_t<
						not ::std::is_same<
							::std::decay_t<ut::first_t<Targs...>>,
							this_type
						>::value,
						void
					  >	
			>
			command(Targs&&... p_args)
			{
				auto x = { (dispatch(::std::forward<Targs>(p_args)), 0)... };
				(void)x;
			}
			
			command(const this_type&) = delete;
			this_type& operator=(const this_type&) = delete;
			
			command(this_type&&) = delete;
			this_type& operator=(this_type&&) = default;
			
		public:
			auto id() const
				-> enum_type
			{
				return m_Id;
			}
			
			auto name() const
				-> const ::std::string&
			{
				return m_Name;
			}

		private:
			template< typename T >
			void dispatch(T&& p_arg)
			{
				dispatch(internal::tag_category_of_t<::std::decay_t<T>>{}, ::std::forward<T>(p_arg));
			}
			
			// Argument tag: Add to container.
			template< typename T >
			void dispatch(internal::argument_tag_t, T&& p_arg)
			{
				this->add(::std::make_unique<std::decay_t<T>>(::std::forward<T>(p_arg)));
			}
				
			// Option tag: ignore.
			template< typename T >
			void dispatch(internal::option_tag_t, T&& p_arg)
			{
			}
			
			// Matching key-value tag: Set name and id.
			void dispatch(internal::invalid_tag_t, const internal::key_value_t<::std::string, enum_type>& p_kvp)
			{
				m_Name = p_kvp.first();
				m_Id = p_kvp.second();
			}
			
			// Invalid tag supplied.
			template< typename T >
			void dispatch(internal::invalid_tag_t, T&&)
			{
				static_assert(ut::always_false_v<T>,
					"Invalid object supplied in handler constructor!");
			}
					
			/*bool handle_error(const ::std::exception& p_ex);
			[[noreturn]] void rethrow_error();
			*/
			
		protected:
			enum_type m_Id;
			::std::string m_Name;		
	};
}
