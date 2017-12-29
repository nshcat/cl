#pragma once

#include <iostream>

#include <ut/string_view.hxx>
#include <ut/console_color.hxx>
#include <ut/format.hxx>
#include "types.hxx"


namespace cl
{
	namespace diagnostics
	{
		namespace internal
		{
			auto get_type_str(diagnostics_level)
				-> ut::string_view;
			
			auto get_type_clr(diagnostics_level)
				-> ut::console_color;
				
			auto get_manip(char p_modifier)
				-> ut::console_color;
				
			auto is_manip(char p_modifier)
				-> bool;
		}
		
		// Constants used to set caret offset.
		constexpr static const ::std::size_t leftmost = 0U;
		constexpr static const ::std::size_t rightmost = ::std::size_t(-1);
		
		
		// Outputs given string reference to given output stream, applying color iff stream supports it
		// and color modifiers are encountered in the string.
		auto output_colored(::std::ostream& p_out, ut::string_view p_str)
			-> void;
		
		
		// Strips given string of all color modifiers.
		auto strip_modifiers(ut::string_view p_str)
			-> ::std::string;
		
		
		// test:3:5: error: bla
		// Writes a diagnostics message to screen.
		// Any member with the value npos in the source location will cause the corresponding
		// number to not be displayed. Thus, a fully default construced instance of source_location
		// will cause this method to not diplay any source location at all.
		auto post_diagnostic(::std::ostream& p_out, diagnostics_level p_level, ut::string_view p_sender, source_location p_loc, ut::string_view p_message)
			-> void;
		
		template< typename... Ts >	
		auto post_diagnostic(::std::ostream& p_out, diagnostics_level p_level, ut::string_view p_sender, source_location p_loc, ut::string_view p_message, Ts&&... p_fmtargs)
			-> void
		{
			const auto t_msg = ut::sprintf(p_message, ::std::forward<Ts>(p_fmtargs)...);
			post_diagnostic(p_out, p_level, p_sender, p_loc, {t_msg});
		}
				
		
		//     blablablabla
		//	   ^~~~~
		// Writes a part of the given source view to screen, indented by a tab.
		// Supports adding squiggly-lines. The caret can optionally be offset from
		// the start of the underline (relative)
		auto post_source_view(::std::ostream& p_out, ut::string_view p_src, source_range p_range, source_range p_underline, ::std::size_t p_caretOffset = leftmost)
			-> void;
			
			
		//     blablablabla
		//	   ^~~~~
		//	   bla
		// Writes a part of the given source view to screen, indented by a tab.
		// Supports adding squiggly-lines. The caret can optionally be offset from
		// the start of the underline (relative).
		// This will add a string as a hint to the user. It will be positioned
		// at the specified caret position.
		auto post_fixit(::std::ostream& p_out, ut::string_view p_src, source_range p_range, source_range p_underline, ut::string_view p_fix, ::std::size_t p_caretOffset = leftmost)
			-> void;
	}
}
