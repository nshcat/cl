#pragma once

#include <pegtl.hh>

#include "command_base.hxx"
#include "grammar.hxx"
#include "positional_argument.hxx"
#include "parser_state.hxx"
#include "diagnostics_state.hxx"

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
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				p_state.names().push_back(p_in.string());
				
				p_diagState.name_range() = source_range{ from_length, p_in.column(), p_in.size(), p_in.line() };
			}
		};

		template< >
		struct parser_action <R_LongName>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				p_state.names().push_back(p_in.string());
				
				p_diagState.name_range() = source_range{ from_length, p_in.column(), p_in.size(), p_in.line() };
			}
		};

		template< >
		struct parser_action <R_Value>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				p_state.values().push_back(p_in.string());
				
				p_diagState.value_ranges().push_back({ from_length, p_in.column(), p_in.size(), p_in.line() });
			}
		};

		template< >
		struct parser_action <R_Long>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				p_state.is_short() = false;
			}
		};

		template< >
		struct parser_action <R_Short>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				p_state.is_short() = true;
			}
		};

		template< >
		struct parser_action <R_Assignment>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				p_diagState.argument_range() = source_range{ from_length, p_in.column(), p_in.size(), p_in.line() };
				
				if(p_state.is_short())
				{
					auto t_argument = p_hndlr.get(p_state.name().at(0));
					t_argument->set_diag_state(p_diagState);
					t_argument->read(p_state.values(), true);
				}
				else
				{
					auto t_argument = p_hndlr.get(p_state.name());
					t_argument->set_diag_state(p_diagState);
					t_argument->read(p_state.values(), true);			
				}			
			}
		};

		template< >
		struct parser_action <R_Multi_Switch>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				p_diagState.argument_range() = source_range{ from_length, p_in.column(), p_in.size(), p_in.line() };
			
				// Diagnostics: Since more than one ShortName is matched, the name_range field in the
				// diagnostics state points to the last short name.
				// Here, for correct diagnostics, the position has to be corrected after each
				// iteration.				
				auto t_nameRange = p_diagState.name_range();
				for(::std::size_t t_ix = 0; t_ix < p_state.names().size(); ++t_ix)
				{
					// Adjust name range
					p_diagState.name_range() = source_range{
						from_length,
						t_nameRange.start() - (p_state.names().size() - 1 - t_ix),
						1U,
						p_in.line()
					};
					
					auto t_argument = p_hndlr.get(p_state.names()[t_ix].at(0));
					t_argument->set_diag_state(p_diagState);
					t_argument->read(p_state.values(), false);				
				}				
			}
		};

		template< >
		struct parser_action <R_NonAssignment>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				// We will not include the whole list of positional arguments here, so the range
				// of the whole argument is the range of the current name, altered based on whether
				// it is a short or long argument.
				const auto t_adjust = (p_state.is_short() ? 1U : 2U);
				
				p_diagState.argument_range() = source_range{
					from_length,
					p_in.column(),
					p_diagState.name_range().length() + t_adjust,
					p_in.line()
				};
			
				if(p_state.is_short())
				{
					auto t_argument = p_hndlr.get(p_state.name().at(0));
					t_argument->set_diag_state(p_diagState);
					t_argument->read(p_state.values(), false);			
				}
				else
				{
					auto t_argument = p_hndlr.get(p_state.name());
					t_argument->set_diag_state(p_diagState);
					t_argument->read(p_state.values(), false);			
				}		
			}
		};

		template< >
		struct parser_action <R_Argument>
		{
			static void apply(const pegtl::input& p_in, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{
				if (!p_state.values().empty())
				{
					// The use might not have supplied a handler for free arguments
					if(p_hndlr.has(""))
					{
						// Based on how many values got consumed, we have to remove
						// the value source ranges starting with the first one.				
						const auto t_consumedValues = p_diagState.value_ranges().size() - p_state.values().size();
						
						// Remove elements
						if(t_consumedValues > 0U)
						{
							auto& t_container = p_diagState.value_ranges();
							t_container.erase(t_container.begin(), ::std::next(t_container.begin(), t_consumedValues));
						}							
					
						auto t_argument = p_hndlr.get("");
						t_argument->set_diag_state(p_diagState);
						t_argument->read(p_state.values(), false);
					}
				}

				// Clean up state for next match
				p_state.cleanup();
				
				// Clean up diagnostics state for next match
				p_diagState.cleanup();
			}
		};
	}
}
