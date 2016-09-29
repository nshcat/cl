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
#include "enum_argument.hxx"
#include "handler_data.hxx"
#include "options.hxx"

namespace cl
{
	namespace internal
	{
		struct option_tag_t{};
		struct argument_tag_t{};
		struct invalid_tag_t{};
		
		template< typename T >
		using is_option_tag = ::std::is_base_of<internal::option_base, T>;
		
		template< typename T >
		using is_argument_tag = ::std::is_base_of<internal::argument_base, T>;
		
		template< typename T >
		using tag_category_of_t =
			ut::category_of_t<
				T,
				ut::category<option_tag_t, is_option_tag>,
				ut::category<argument_tag_t, is_argument_tag>,
				ut::category<invalid_tag_t>
			>;		
	}
	
	class handler
	{
		public:
			template< typename... Targs,
					  typename = ::std::enable_if_t< ! ::std::is_same<::std::decay_t<ut::first_t<Targs...>>, handler>::value, void>	
			>
			handler(Targs&&... p_args)
			{
				auto x = { (dispatch(::std::forward<Targs>(p_args)), 0)... };
				(void)x;
			}

			handler()
			{

			}

		public:
			bool read(int argc, const char* argv[]);

		public:
			// Official interface

			auto print_help() const -> void;
			
			// Get argument by id
			auto argument(std::size_t) const -> internal::argument_base*;

			// Get argument by id convertible to size_t
			template< typename T >
			auto argument(T p_id) const -> internal::argument_base*
			{
				static_assert(ut::is_static_castable<T, std::size_t>::value,
					"ID type needs to be static_castable to size_t!");

				return argument(static_cast<std::size_t>(p_id));
			}

			// Get value by id convertible to size_t
			template< typename T, typename E >
			auto value(E p_id) const -> const T&
			{
				return argument(p_id)->template as<internal::value_base<T>>()->value();
			}

			template< typename T >
			bool is_supplied(T p_id)
			{
				return argument(p_id)->supplied();
			}

			template< typename T >
			auto operator[](T p_id) -> internal::argument_base*
			{
				return argument(p_id);
			}

			// Deprecated interface, only used internally TODO move it to parser_state
			auto argument_short(char p_name) const -> internal::argument_base*;
			auto argument_long(const std::string&) const -> internal::argument_base*;
			auto argument_str(const std::string& p_name, bool p_isShort) const -> internal::argument_base*;

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
				add_internal(std::make_unique<std::decay_t<T>>(std::forward<T>(p_arg)));
			}
				
			// Option tag: Call apply.
			template< typename T >
			void dispatch(internal::option_tag_t, T&& p_arg)
			{
				p_arg.apply(m_Data);
			}
			
			// Invalid tag supplied.
			template< typename T >
			void dispatch(internal::invalid_tag_t, T&&)
			{
				static_assert(ut::always_false_v<T>,
					"Invalid object supplied in handler constructor!");
			}
			
		private:
			void add_internal(std::unique_ptr<internal::argument_base> p_ptr);

			void check_required();
			void check_exclusive();
			
			bool handle_error(const ::std::exception& p_ex);
			[[noreturn]] void rethrow_error();
	
		private:
			internal::handler_data m_Data;
			std::map<std::string, std::unique_ptr<internal::argument_base>> m_Arguments;
			std::map<size_t, ut::observer_ptr<internal::argument_base>> m_ArgMap;
	};
}
