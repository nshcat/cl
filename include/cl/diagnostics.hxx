#pragma once

#include <ut/string_view.hxx>
#include <ut/console_color.hxx>
#include <ut/format.hxx>

#include "diagnostics_state.hxx"

namespace cl
{
	namespace internal
	{
		enum class diagnostics_level
		{
			error,
			warning,
			note,
			remark,
			debug
		};
		
		auto get_type_str(diagnostics_level)
			-> ut::string_view;
			
		auto get_type_clr(diagnostics_level)
			-> ut::console_color;
		
		
		
		// test:3:5: error: bla
		// Writes a diagnostics to screen.
		// Any member with the value npos in the source location will cause the corresponding
		// number to not be displayed. Thus, a fully default construced instance of source_location
		// will cause this method to not diplay any source location at all.
		auto post_diagnostic(diagnostics_level p_level, ut::string_view p_sender, source_location p_loc, ut::string_view p_message)
			-> void;
		
		template< typename... Ts >
		auto post_diagnostic(diagnostics_level p_level, ut::string_view p_sender, source_location p_loc, ut::string_view p_message, Ts&&... p_fmtargs)
			-> void
		{
			const auto t_msg = ut::sprintf(p_message, ::std::forward<Ts>(p_fmtargs)...);
			post_diagnostic(p_level, p_sender, p_loc, {t_msg});
		}
		
		
		//     blablablabla
		//	   ^~~~~
		// Writes a part of the given source view to screen, indented by a tab.
		// Supports adding squiggly-lines. The caret can optionally be offset from
		// the start of the underline (relative)
		auto post_source_view(ut::string_view p_src, source_range p_range, source_range p_underline, ::std::size_t p_caretOffset = 0U)
			-> void;
			
			
		//     blablablabla
		//	   ^~~~~
		//	   bla
		// Writes a part of the given source view to screen, indented by a tab.
		// Supports adding squiggly-lines. The caret can optionally be offset from
		// the start of the underline (relative).
		// This will add a string as a hint to the user. It will be positioned
		// at the specified caret position.
		auto post_fixit(ut::string_view p_src, source_range p_range, source_range p_underline, ut::string_view p_fix, ::std::size_t p_caretOffset = 0U)
			-> void;
	}
}
