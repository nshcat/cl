#pragma once
// TODO to_string (virtual): 
// TODO case_sensitive(true-/false)
// Groups! (group_trigger, group_optional, group_required)
// TODO better Exception macro that uses a stringstream to assemble a message. Wrap in do{}while(false).
// TODO better, cleaner code *puke*. (Would be good to do a PEGTL Parser for arguments. Its easy to do (greedy matching von positionals)
// -> maybe no AST, just directly invoke the read methods in the actions! (argument object list in the parser state)
// Follow the normal inheritance approach. (Does dispatch work then?! Maybe if virtual dispatch methods!)
// -> only read is virtual. value() is different vor each. handler<T> tries to get argument as T. value() is used there.
// Maybe replace Exceptions? They cant really give good hints on what is missing.
// Help printer. Uses category and description fields.
// Put as much as possible in CXX files. No header-only library! (hides dependency on PEGTL)

/* TODO
namespace cl
{
	template< typename T >
	auto reference(T& ref)    // <- automatically deduces template argument. Do this with every tag.
	{
		return cl::internal::reference_t<T>(ref);
	}
}


*/

#include <memory>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <regex>
#include <map>
#include <ut/type_traits.hxx>
#include <ut/observer_ptr.hxx>

#include "tags.hxx"
#include "boolean_argument.hxx"
#include "integer_argument.hxx"
#include "positional_argument.hxx"
#include "argument_base.hxx"
#include "free_argument.hxx"
#include "string_argument.hxx"
#include "enum_argument.hxx"
#include "utility.hxx"

namespace cl
{
	class handler
	{
		public:
			template< typename... Targs,
					  typename = std::enable_if_t< ! std::is_same<std::decay_t<ut::first_t<Targs...>>, handler>::value, void>	
			>
			handler(Targs&&... p_args)
			{
				auto x = { (add(std::forward<Targs>(p_args)), 0)... };
				(void)x;
			}

			handler()
			{

			}

		public:
			void read(int argc, const char* argv[]);

		public:
			// Official interface

			// Get argument by id
			auto argument(size_t) const -> internal::argument_base*;


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
			void add(T&& p_arg)
			{
				static_assert(std::is_base_of<internal::argument_base, std::decay_t<T>>::value,
					"Non-argument supplied in handler constructor!");

				add_internal(std::make_unique<std::decay_t<T>>(std::forward<T>(p_arg)));
			}

		private:
			void add_internal(std::unique_ptr<internal::argument_base> p_ptr);

			void check_required();
			void check_exclusive();
	
		private:
			std::map<std::string, std::unique_ptr<internal::argument_base>> m_Arguments;
			std::map<size_t, ut::observer_ptr<internal::argument_base>> m_ArgMap;
	};
}
