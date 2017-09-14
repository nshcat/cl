#pragma once

#include <pegtl.hh>

#include "command_base.hxx"
#include "grammar.hxx"
#include "positional_argument.hxx"
#include "parser_state.hxx"

namespace cl
{
	namespace internal
	{
		template< typename T >
		struct parser_action
			:	pegtl::nothing<T>
		{};

		template< >
		struct parser_action <R_ShortName>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				p_state.names().push_back(p_in.string());
			}
		};

		template< >
		struct parser_action <R_LongName>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				p_state.names().push_back(p_in.string());
			}
		};

		template< >
		struct parser_action <R_Value>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				p_state.values().push_back(p_in.string());
			}
		};

		template< >
		struct parser_action <R_Long>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				p_state.is_short() = false;
			}
		};

		template< >
		struct parser_action <R_Short>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				p_state.is_short() = true;
			}
		};

		template< >
		struct parser_action <R_Assignment>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				if(p_state.is_short())
					p_hndlr.get(p_state.name().at(0))->read(p_state.values(), true);
				else p_hndlr.get(p_state.name())->read(p_state.values(), true);
			}
		};

		template< >
		struct parser_action <R_Multi_Switch>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{	
				for (auto& t_name : p_state.names())
					p_hndlr.get(t_name.at(0))->read(p_state.values(), false);			
			}
		};

		template< >
		struct parser_action <R_NonAssignment>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				if(p_state.is_short())
					p_hndlr.get(p_state.name().at(0))->read(p_state.values(), false);
				else p_hndlr.get(p_state.name())->read(p_state.values(), false);
			}
		};

		template< >
		struct parser_action <R_Argument>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state)
			{
				if (!p_state.values().empty())
				{
					/*// Try to add values to free_arguments. TODO solve without try catch
					try
					{
						p_hndlr.get("")->read(p_state.values(), false);
					}
					catch (...)
					{
						// If no free_arguments was added, just ignore the values
					}*/
					// The use might not have supplied a handler for free arguments
					if(p_hndlr.has(""))
						p_hndlr.get("")->read(p_state.values(), false);
				}

				// Clean up state for next match
				p_state.cleanup();
			}
		};
	}
}
