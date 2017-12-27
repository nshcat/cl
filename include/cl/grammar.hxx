#pragma once

#include <algorithm>

#include <pegtl.hh>

#include "command.hxx"
#include "parser_state.hxx"
#include "parser_utility.hxx"

namespace cl
{
	namespace internal
	{
		using namespace pegtl;


		struct R_Meta_IsSwitch
		{
			template<	pegtl::apply_mode A,
						template< typename... > class Action,
						template< typename... > class Control
					>
			static bool match(pegtl::input&, command_base& p_hndlr, parser_state& p_state, diagnostics_state& p_diagState)
			{				
				// This assumes that a short name was already matched.
				// Check if argument with given short name is a switch type argument.
				if(!p_hndlr.has(p_state.short_name()))
				{
					// At this point, the argument_range is not yet set in the diagnostics state
					// (because the parent grammar rule has not been matched yet),
					// so we have to generate our own here. This is not a problem though, since
					// this rule only appears in one position: directly after a short argument
					// (of the form `-X`, where X is an arbitrary printable character)
					// One problem is that other short arguments might follow, in the form
					// `-XYZ...`, but those aren't matched yet so we do not have any location data
					// about them. We thus have to find the first white space to determine
					// how much we need to display.
					auto t_state{p_diagState};
					
					const auto t_src = t_state.source();
					const auto t_name = t_state.name_range();
					
					const auto t_it = ::std::find(::std::next(t_src.begin(), t_name.start()), t_src.end(), ' ');			
					
					t_state.argument_range() = diagnostics::source_range{
						diagnostics::from_length,
						t_state.name_range().start() - 1U,
						static_cast<diagnostics::source_range::size_type>(
							1 + ::std::distance(::std::next(t_src.begin(), t_name.start()), t_it)
						)
					};
				
					report_invalid(p_state.name(), true, t_state);
				}

				return p_hndlr.get(p_state.short_name())->is_switch();
			}
		};


		// R_Delim: Non-printable delimiter used to mark argument values that
		// might contain spaces. We don't use \" here because that is difficult
		// to remove. (For now)
		struct R_Delim
			:	one<'\x11'>
		{};

		// R_Long: Start of a long-name argument ("--")
		// Action: Set flag to indicate long name
		struct R_Long 
			:	string<'-', '-'>
		{};

		// R_Short: Start of a short-name argument ("-")
		// Action: Set flag to indicate short name
		struct R_Short
			:	seq<
					one<'-'>,
					not_at< one<'-'> >
				>
		{};



		// Action: Set name.
		struct R_ShortName
			:	ranges< 'a', 'z', 'A', 'Z' >
		{};

		// Action: Set name.
		struct R_LongName
			:	plus<
					sor<
						ranges< 'a', 'z', 'A', 'Z', '0', '9' >,
						one<'_'>,
						one<'-'>
					>
				>
		{};


		// R_ArgBegin: Start of an argument. Either long or short.
		struct R_ArgBegin
			:	sor<
					seq<
						R_Long,
						R_LongName
					>,
					seq<
						R_Short,
						R_ShortName
					>
				>
		{};


		// R_Value_Impl
		// Action: Insert value into state.
		struct R_Value   // Interpret as value
			:	plus<
					seq<
						not_at< R_Delim >,	// TODO maybe we have to accept \" here aswell
						ascii::any
					>
				>
		{};


		// R_PositionalArgList
		struct R_PositionalArgList
			:	star<
					seq<
						R_Delim,
						not_at<R_ArgBegin>,					
						R_Value,
						R_Delim,
						space
					>
				>
		{};

		// R_NonAssignment
		// Action: Call read("", values). If argument is positional, clear value list.
		struct R_NonAssignment
			:	seq<
					R_Delim,
					R_ArgBegin,
					R_Delim,
					space,
					R_PositionalArgList
				>
		{};


		// R_Assignment_Eq: Assignment using "=" sign
		struct R_Assignment_Eq
			:	seq<
					R_Delim,
					R_ArgBegin,
					one<'='>,
					R_Value,
					R_Delim,
					space
				>
		{};

		// R_Assignment_Alt: Assignment without "=" sign, forced quoted
		struct R_Assignment_Alt
			:	seq<
					R_Delim,
					R_Short,
					R_ShortName,
					R_Value,
					R_Delim,
					space
				>
		{};


		// R_Multi_Switch: Multiple short switches in form "-abc". Only matches if first char is actually
		// argument of switch type.
		struct R_Multi_Switch
			:	seq<
					R_Delim,
					R_Short,
					R_ShortName,
					R_Meta_IsSwitch,
					plus< R_ShortName >,
					R_Delim,
					space
				>
		{};

		// R_Assignment
		// Action: Call read(value, "") on matching argument. Clear value list.
		struct R_Assignment
			:	sor<
					R_Assignment_Eq,
					R_Assignment_Alt			
				>
		{};

		// R_Argument
		// Action: If values is nonempty, add them to free_arguments. Clear value list.
		struct R_Argument
			:	sor<
					R_Multi_Switch,
					R_Assignment,
					R_NonAssignment,
					R_PositionalArgList
				>
		{};

		// R_Commandline
		struct R_Commandline
			:	seq<
					plus< R_Argument >,
					must< eof >
				>
		{};
	}
}
