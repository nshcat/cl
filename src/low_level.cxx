#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <ut/cast.hxx>
#include <ut/utility.hxx>
#include <ut/throwf.hxx>
#include <diagnostics/low_level.hxx>

namespace cl
{
	namespace diagnostics
	{
		namespace internal
		{
			auto get_type_str(diagnostics_level p_level)
				-> ut::string_view
			{
				const static ::std::array<const char*, 5> t_table = {{
					"error",
					"warning",
					"note",
					"remark",
					"debug"
				}};
				
				return { t_table.at(ut::enum_cast(p_level)) };
			}
			
			auto get_type_clr(diagnostics_level p_level)
				-> ut::console_color
			{
				const static ::std::array<ut::console_color, 5> t_table = {{
					ut::console_color::bright_red,
					ut::console_color::bright_yellow,
					ut::console_color::bright_white,
					ut::console_color::bright_white,
					ut::console_color::bright_magenta
				}};
				
				return t_table.at(ut::enum_cast(p_level));
			}
					
			auto get_manip(char p_modifier)
				-> ut::console_color
			{
				if(p_modifier == ut::enum_cast(color_modifier::reset))
					return ut::console_color::reset;
			
				constexpr auto t_beginDull = ut::enum_cast(color_modifier::dull_begin);
				constexpr auto t_endDull = ut::enum_cast(color_modifier::dull_end);
				constexpr auto t_beginVivid = ut::enum_cast(color_modifier::vivid_begin);
				constexpr auto t_endVivid = ut::enum_cast(color_modifier::vivid_end);
						
				if(ut::in_range(p_modifier, t_beginDull, t_endDull))
				{
					// Dull color
					return ut::enum_cast<ut::console_color>((p_modifier - t_beginDull) * 2);
				}
				else if(ut::in_range(p_modifier, t_beginVivid, t_endVivid))
				{
					// Vivid color
					return ut::enum_cast<ut::console_color>(((p_modifier - t_beginVivid) * 2) + 1);
				}
				else ut::throwf<::std::runtime_error>("Invalid color modifier: ^%c", p_modifier);		
			}
			
			auto is_manip(char p_modifier)
				-> bool
			{
				if(p_modifier == ut::enum_cast(color_modifier::reset))
					return true;
			
				constexpr auto t_beginDull = ut::enum_cast(color_modifier::dull_begin);
				constexpr auto t_endDull = ut::enum_cast(color_modifier::dull_end);
				constexpr auto t_beginVivid = ut::enum_cast(color_modifier::vivid_begin);
				constexpr auto t_endVivid = ut::enum_cast(color_modifier::vivid_end);
				
				return ut::in_range(p_modifier, t_beginDull, t_endDull) ||
				       ut::in_range(p_modifier, t_beginVivid, t_endVivid);
			}
		}
		
		
		auto output_colored(::std::ostream& p_out, ut::string_view p_str)
			-> void
		{
			constexpr auto t_escape = ut::enum_cast(color_modifier::escape);
		
			while(p_str.length() > 0U)
			{
				// Find next escape character
				const auto t_pos = p_str.find(t_escape);
				
				if(t_pos == ut::string_view::npos || t_pos == (p_str.length() - 1U))
				{
					// No escape character found or is last character in string. Output remaining string and return.
					p_out << p_str;
					return;
				}
				else
				{
					// The remaining string contains a escape character that is not the last character in the
					// string.
					
					// Retrieve string preceding the modifier, excluding the escape character and output it.
					p_out << p_str.substr(0, t_pos);
					
					// Save (potential) modifier for later use
					// This is safe to do since we know that the escape character was not the last character
					// in the string
					const auto t_mod = p_str[t_pos+1];
					
					// Remove preceding string and escaped modifier from string
					p_str.remove_prefix(t_pos+2);
					
					if(internal::is_manip(t_mod))
						p_out << ut::foreground(internal::get_manip(t_mod));
					else
						p_out << t_escape << t_mod;
				}
			}
		}
		
		
				
		auto post_diagnostic(::std::ostream& p_out, diagnostics_level p_level, ut::string_view p_sender, source_location p_loc, ut::string_view p_message)
			-> void
		{
			const auto t_color = internal::get_type_clr(p_level);
			const auto t_type_str = internal::get_type_str(p_level);
			
			// Sender and line, column
			{
				p_out << p_sender << ":";
			
				if(p_loc.line() != source_location::npos)
					p_out << p_loc.line() << ":";
				
				if(p_loc.column() != source_location::npos)
					p_out << p_loc.column() << ":";
				
				p_out << ' ';
			}
			
			// Diag level
			{
				p_out 	<< ut::foreground(t_color)
						<< t_type_str
						<< ut::reset_color
						<< ": ";
						
				output_colored(p_out, p_message);
					
				p_out	<< ::std::endl;
			}
		}
		
		
		auto post_source_view(::std::ostream& p_out, ut::string_view p_src, source_range p_range, source_range p_underline, ::std::size_t p_caretOffset)
			-> void
		{
			// TODO most of the following implementation will only work when the source consists
			// of only one line. This should be fixed if this code is to be reused.
		
			// The source view must not be empty
			if(p_src.empty())
				throw ::std::runtime_error("post_source_view: Source view is empty");
		
			// If the source range is not well defined, throw exception since we cant do anything
			if(p_range.empty())
				throw ::std::runtime_error("post_source_view: Invalid source range");
					
			// Retrieve source piece to display
			auto t_srcPiece = p_src.substr(p_range.start(), p_range.length());
			
			// Write it out
			p_out << '\t' << t_srcPiece << ::std::endl;
			
			if(!p_underline.empty())
			{
				// Check if it is inside the displayed range
				if((p_range.start() > p_underline.start()) || (p_range.end() < p_underline.end()))
					throw ::std::runtime_error("post_source_view: Underline source range not inside of display range");
				
				// Check if caret is inside of underline range
				if((p_caretOffset >= p_underline.length()) && (p_caretOffset != rightmost))
					throw ::std::runtime_error("post_source_view: Caret offset out of range");
				
				// Convert to relative (based on start of displayed source range)
				source_range t_relative{
					from_length,
					p_underline.start() - p_range.start(),
					p_underline.length()
				};
						
				// Build underline string
				::std::ostringstream t_underlineStrStream{ };
				
				// First part: spaces before the squiggly line
				::std::fill_n(::std::ostream_iterator<char>(t_underlineStrStream), t_relative.start(), ' ');
				
				// Second part: squiggly line
				::std::fill_n(::std::ostream_iterator<char>(t_underlineStrStream), t_relative.length(), '~');
				
				// Retrieve string
				::std::string t_underlineStr{ t_underlineStrStream.str() };
				
				// Third part: caret
				const auto t_caretIndex = (p_caretOffset == rightmost) ? (t_relative.end() - 1) : t_relative.start() + p_caretOffset;
				t_underlineStr.at(t_caretIndex) = '^';
							
				// Display
				p_out	<< '\t'
						<< ut::foreground(ut::console_color::bright_green)
						<< t_underlineStr
						<< ut::reset_color
						<< ::std::endl;
			}	
		}
			
		auto post_fixit(::std::ostream& p_out, ut::string_view p_src, source_range p_range, source_range p_underline, ut::string_view p_fix, ::std::size_t p_caretOffset)
			-> void
		{
			// Underline is not optional here
			if(p_underline.empty())
				throw ::std::runtime_error("post_fixit: Underline range ist empty");
				
			// Fix string must not be empty
			if(p_fix.empty())
				throw ::std::runtime_error("post_fixit: Fixit hint string is empty");
		
			// Write out source view and squiggly lines
			post_source_view(p_out, p_src, p_range, p_underline, p_caretOffset = 0U);
							
			// Convert to relative (based on start of displayed source range)
			source_range t_relative{
				p_underline.start() - p_range.start(),
				p_underline.length() // This is intended, the length becomes the end marker
									 // because it is relative now.
			};
			
			p_out << '\t';
			
			// Write spaces to the output stream to correctly align the fix hint.
			// This dependends on the start of the underline (given as t_relative)
			// and the caret offset
			::std::fill_n(::std::ostream_iterator<char>(p_out), t_relative.start() + p_caretOffset, ' ');
			
			// Write hint
			p_out	<< ut::foreground(ut::console_color::bright_green)
					<< p_fix
					<< ut::reset_color
					<< ::std::endl;
		}
	}
}
