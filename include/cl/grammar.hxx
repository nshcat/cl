#pragma once

#include <pegtl.hh>

#include "handler.hxx"
#include "parser_state.hxx"

namespace cl
{
	namespace internal
	{
		using namespace pegtl;


		struct R_Meta_IsSwitch
		{
			template<	pegtl::apply_mode A,
						template< typename... > class Action,
						template< typename... > class Control,
						typename... States
					>
			static bool match( pegtl::input&, handler& p_hndlr, parser_state& p_state, States&... )
			{
				// This assumes that a short name was already matched.
				// Check if argument with given short name is a switch type argument.
				return p_hndlr.argument_short(p_state.short_name())->is_switch();
			}
		};



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
						not_at< one<'"'> >,
						ascii::any
					>
				>
		{};


		// R_PositionalArgList
		struct R_PositionalArgList
			:	star<
					seq<
						one<'"'>,
						not_at<R_ArgBegin>,					
						R_Value,
						one<'"'>,
						space
					>
				>
		{};

		// R_NonAssignment
		// Action: Call read("", values). If argument is positional, clear value list.
		struct R_NonAssignment
			:	seq<
					one<'"'>,
					R_ArgBegin,
					one<'"'>,
					space,
					R_PositionalArgList
				>
		{};


		// R_Assignment_Eq: Assignment using "=" sign
		struct R_Assignment_Eq
			:	seq<
					one<'"'>,
					R_ArgBegin,
					one<'='>,
					R_Value,
					one<'"'>,
					space
				>
		{};

		// R_Assignment_Alt: Assignment without "=" sign, forced quoted
		struct R_Assignment_Alt
			:	seq<
					one<'"'>,
					R_Short,
					R_ShortName,
					R_Value,
					one<'"'>,
					space
				>
		{};


		// R_Multi_Switch: Multiple short switches in form "-abc". Only matches if first char is actually
		// argument of switch type.
		struct R_Multi_Switch
			:	seq<
					one<'"'>,
					R_Short,
					R_ShortName,
					R_Meta_IsSwitch,
					plus< R_ShortName >,
					one<'"'>,
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