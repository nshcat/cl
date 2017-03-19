#pragma once

#include <type_traits>
#include <map>

#include <ut/type_traits.hxx>
#include <ut/array_view.hxx>
#include <ut/throwf.hxx>
#include <ut/observer_ptr.hxx>
#include <ut/hash.hxx>
#include <ut/cast.hxx>
#include <ut/always_false.hxx>

#include "handler_base.hxx"
#include "command.hxx"
#include "meta.hxx"

namespace cl
{	
	template< typename TEnum >
	class multi_handler
		: 	public internal::handler_base
	{
		using this_type = multi_handler<TEnum>;
		using input_type = ut::array_view<const char*>;
		using base_type = internal::handler_base;
		using enum_type = TEnum;
		using command_type = command<enum_type>;
		using command_ptr = ::std::unique_ptr<command_type>;
		using command_view = ut::observer_ptr<command_type>;
		using id_map_type = ::std::map<enum_type, command_view>;
		using command_map_type = ::std::map<::std::string, command_ptr>;
		
		private:
			template< typename T >
			using is_command_tag = ::std::is_same<T, command_type>;
			
			struct command_tag_t
			{};
			
			template< typename T >
			using command_category_of_t =
				ut::category_of_t<
					T,
					ut::category<internal::option_tag_t, internal::is_option_tag>,
					ut::category<command_tag_t, is_command_tag>,
					ut::category<internal::invalid_tag_t>
			>;
		
		
		public:
			template< 	typename... Ts,
						typename = ::std::enable_if_t<
							not ::std::is_same<
								handler,
								::std::decay_t<ut::first_t<Ts...>>
							>::value
						>
			>
			multi_handler(Ts&&... p_args)
			{
				// It is ok to access option tags here despite being forwarded beforehand,
				// because cl::command will ignore all option tags completely.
				const auto x = { (dispatch(::std::forward<Ts>(p_args)), 0)... };
				(void)x;
			}
			
			multi_handler() = delete;
			multi_handler(const this_type&) = delete;
			multi_handler(this_type&&) = default;
			
			this_type& operator=(const this_type&) = delete;
			this_type& operator=(this_type&&) = default;
			
		public:
			auto read(input_type p_in)
				-> bool
			{
				try
				{
					// Determine command that is meant to handle the supplied command line.
					if(p_in.size() <= 0)
						throw ::std::runtime_error("No arguments supplied");
					
					const auto t_name = p_in[0];
					
					if(has_command(t_name))
					{
						command(t_name)->read(p_in);
					}
					else
					{
						ut::throwf<::std::runtime_error>(
							"Unknown subcommand \"%s\"!",
							t_name
						);
					}			

					return true;
				}
				catch(const ::std::exception& p_ex)
				{
					return this->handle_error(p_ex);
				}	
			}
				
			auto read(int p_argc, const char** p_argv)
				-> bool
			{
				return read(ut::make_view<const char*>(p_argv+1, p_argc-1));
			}
				
		public:
			auto command(enum_type p_id)
				-> command_view
			{
				const auto it = m_IdMap.find(p_id);
				
				if(it != m_IdMap.end())
				{
					return it->second;
				}
				else throw ::std::runtime_error("multi_handler: Tried to retrieve nonexistant command!");
			}
			
			auto command(const ::std::string& p_name)
				-> command_view
			{
				const auto it = m_CmdMap.find(p_name);
				
				if(it != m_CmdMap.end())
				{
					return ut::make_observer<command_type>(it->second.get());
				}
				else throw ::std::runtime_error("multi_handler: Tried to retrieve nonexistant command!");
			}
			
			auto has_command(const ::std::string& p_name)
				-> bool
			{
				return m_CmdMap.count(p_name) >= 1;
			}
			
			auto supplied_id() const
				-> enum_type
			{
				return m_GivenCommand;
			}
			
			// TODO const correctness
			auto command()
				-> command_view
			{
				return m_IdMap[supplied_id()];
			}
				
		private:
			template< typename T >
			auto dispatch(T&& p_arg)
				-> void
			{
				dispatch(command_category_of_t<::std::decay_t<T>>{}, ::std::forward<T>(p_arg));
			}
			
			// Global option: forward to base
			template< typename T >
			auto dispatch(internal::option_tag_t, T&& p_arg)
				-> void
			{
				this->base_type::dispatch(::std::forward<T>(p_arg));
			}
			
			// Command: add to maps
			template< typename T >
			auto dispatch(command_tag_t, T&& p_arg)
				-> void
			{
				add(::std::make_unique<std::decay_t<T>>(::std::forward<T>(p_arg)));
			}
			
			// Invalid tag
			template< typename T >
			auto dispatch(internal::invalid_tag_t, T&&)
				-> void			
			{
				static_assert(ut::always_false_v<T>,
				"multi_handler: Invalid tag in constructor!");
			}
			
		private:
			auto add(command_ptr p_cmd)
				-> void
			{
				if(!p_cmd->has_id())
					throw ::std::runtime_error("multi_handler: Supplied command without id and name!");
				
				// Check for duplicates
				if(m_CmdMap.count(p_cmd->name()) != 0)
					throw ::std::runtime_error("multi_handler: Supplied multiple commands with identical name!");
				
				// Insert element into both maps
				m_IdMap[p_cmd->id()] = ut::make_observer<command_type>(p_cmd.get());
				m_CmdMap[p_cmd->name()] = ::std::move(p_cmd);
			}
			
		private:
			id_map_type m_IdMap;
			command_map_type m_CmdMap;
			enum_type m_GivenCommand;
	};
}
