//  command_base: This class contains the actual parsing code for a command line, aswell as
//  related bookkeeping and support routines.
//  It is not a template to hide the PEGTL dependency. This class should never be used directly.
//  The user-visible interface is called command<T>.

#pragma once

#include <algorithm>
#include <map>
#include <string>

#include <ut/observer_ptr.hxx>
#include <ut/array_view.hxx>
#include <ut/string_view.hxx>

#include "argument_base.hxx"
#include "handler_data.hxx"
#include "command_data.hxx"

namespace cl
{
	namespace internal
	{	
		class command_base
		{
			using argument_ptr = ::std::unique_ptr<internal::argument_base>;
			using global_data_ptr = ut::observer_ptr<const handler_data>;
			using argument_view = ut::observer_ptr<internal::argument_base>;
			using const_argument_view = ut::observer_ptr<const internal::argument_base>;
			using input_type = ut::array_view<const char*>;
			using argument_map_type = ::std::map<std::string, argument_ptr>;
			using id_map_type = ::std::map<size_t, argument_view>;
			using this_type = command_base;
			

			public:
				// Get by long name
				auto get(const ::std::string&)
					-> argument_view;
					
				// Get by short name
				auto get(char)
					-> argument_view;
					
				// Get by id
				auto get(::std::size_t)
					-> argument_view;
					
				// Get by long name
				auto get(const ::std::string&) const
					-> const_argument_view;
					
				// Get by short name
				auto get(char) const
					-> const_argument_view;
					
				// Get by id
				auto get(::std::size_t) const
					-> const_argument_view;
					
				// Check by long name
				auto has(const ::std::string&) const
					-> bool;
					
				// Check by short name
				auto has(char) const
					-> bool;
					
				// Check by id
				auto has(::std::size_t) const
					-> bool;
					
				auto supplied(const ::std::string&) const
					-> bool;
				
				auto supplied(char) const
					-> bool;
					
				auto supplied(::std::size_t) const
					-> bool;
						
				// Preprocess and parse given command line
				auto read(input_type)
					-> void;	
				
				// Add new argument
				auto add(argument_ptr)
					-> void;
				
			public:
				// Get by id convertible to size_t (e.g. scoped enumerations)
				template<	typename T,
							typename = ::std::enable_if_t<
								not ::std::is_same<::std::decay_t<T>, ::std::size_t>::value
								&& ut::is_static_castable_v<T, ::std::size_t>
							>
				>
				auto get(T p_id)
					-> argument_view
				{
					return get(static_cast<::std::size_t>(p_id));
				}
				
				
				template<	typename T,
							typename = ::std::enable_if_t<
								not ::std::is_same<::std::decay_t<T>, ::std::size_t>::value
								&& ut::is_static_castable_v<T, ::std::size_t>
							>
				>
				auto get(T p_id) const
					-> const_argument_view
				{
					return get(static_cast<::std::size_t>(p_id));
				}	
				
				
				// Check by id convertible to size_t (e.g. scoped enumerations)
				template<	typename T,
							typename = ::std::enable_if_t<
								not ::std::is_same<::std::decay_t<T>, ::std::size_t>::value
								&& ut::is_static_castable_v<T, ::std::size_t>
							>
				>
				auto has(T p_id) const
					-> bool
				{
					return has(static_cast<::std::size_t>(p_id));
				}
				
				
				template<	typename T,
							typename = ::std::enable_if_t<
								not ::std::is_same<::std::decay_t<T>, ::std::size_t>::value
								&& ut::is_static_castable_v<T, ::std::size_t>
							>
				>
				auto supplied(T p_id) const
					-> bool
				{
					return supplied(static_cast<::std::size_t>(p_id));
				}
				
				
				template<	typename T,
							typename = ::std::enable_if_t<
								ut::is_static_castable_v<T, ::std::size_t>
							>
				>
				auto operator[](T p_id) const
					-> const_argument_view
				{
					return get(static_cast<::std::size_t>(p_id));
				}
				
				
				template<	typename T,
							typename = ::std::enable_if_t<
								ut::is_static_castable_v<T, ::std::size_t>
							>
				>
				auto operator[](T p_id)
					-> argument_view
				{
					return get(static_cast<::std::size_t>(p_id));
				}
				
			public:
				// Prints help according to help_mode
				auto print_help() const
					-> void;
					
				// Prints a summary of all registered arguments
				auto print_summary() const
					-> void;
					
				// Prints a usage string, if specified
				auto print_usage() const
					-> void;
					
			public:
				auto set_global_data(global_data_ptr p_ptr)
					-> void;
				
			public:
				auto local_data() const
					-> const command_data&;
					
				auto global_data() const
					-> const handler_data&;
					
			protected:
				auto local_data()
					-> command_data&;
						
			protected:
				auto check_required() const
					-> void;
			
			protected:
				global_data_ptr m_GlobalDataPtr;
				argument_map_type m_ArgMap;
				id_map_type m_IdMap;
				command_data m_LocalData;
		};
	}
}
