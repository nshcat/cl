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
#include "local_option.hxx"
#include "global_option.hxx"
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
			
			command(this_type&&) = default;
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
			
			auto has_id() const
				-> bool
			{
				return m_HasId;
			}

		private:
			template< typename T >
			auto dispatch(T&& p_arg)
				-> void
			{
				dispatch(internal::tag_category_of_t<::std::decay_t<T>>{}, ::std::forward<T>(p_arg));
			}
			
			// Argument tag: Add to container.
			template< typename T >
			auto dispatch(internal::argument_tag_t, T&& p_arg)
				-> void
			{
				this->add(::std::make_unique<std::decay_t<T>>(::std::forward<T>(p_arg)));
			}
				
			// Global option tag: ignore.
			template< typename T >
			auto dispatch(internal::global_option_tag_t, T&& p_arg)
				-> void
			{
				// At this point, all option tags are in a moved-from state and must not be accessed.
				// We sink them here. This weird design is needed to greatly simplify the design of the non-multi handler:
				// It does not have to actually remove the already dispatched option tags from the command constructor call.
			}
					
			// Local option tag: apply
			template< typename T >
			auto dispatch(internal::local_option_tag_t, T&& p_arg)
				-> void
			{
				p_arg.apply(local_data());
			}
			
			// Matching key-value tag: Set name and id.
			auto dispatch(internal::invalid_tag_t, const internal::key_value_t<::std::string, enum_type>& p_kvp)
				-> void
			{
				m_Name = p_kvp.first();
				m_Id = p_kvp.second();
				m_HasId = true;
			}
			
			// Invalid tag supplied.
			template< typename T >
			auto dispatch(internal::invalid_tag_t, const T&)
				-> void
			{
				static_assert(ut::always_false_v<T>,
					"Invalid object supplied in handler constructor!");
			}
	
			
		protected:		
			enum_type m_Id;
			::std::string m_Name;

			bool m_HasId{false};
	};
	
	template< typename TEnum >
	using command_view = ut::observer_ptr<command<TEnum>>;
}
