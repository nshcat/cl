#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iterator>

#include <ut/cast.hxx>

#include <diagnostics.hxx>

namespace cl
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
		
		auto post_diagnostic(diagnostics_level p_level, ut::string_view p_sender, source_location p_loc, ut::string_view p_message)
			-> void
		{
			const auto t_color = get_type_clr(p_level);
			const auto t_type_str = get_type_str(p_level);
			
			// Sender and line, column
			{
				::std::cout << p_sender << ":";
			
				if(p_loc.line() != source_location::npos)
					::std::cout << p_loc.line() << ":";
				
				if(p_loc.column() != source_location::npos)
					::std::cout << p_loc.column() << ":";
				
				::std::cout << ' ';
			}
			
			// Diag level
			{
				::std::cout << ut::foreground(t_color)
							<< t_type_str
							<< ut::reset_color
							<< ": "
							<< p_message
							<< ::std::endl;
			}
		}
		
		
		auto post_source_view(ut::string_view p_src, source_range p_range, source_range p_underline, ::std::size_t p_caretOffset)
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
			::std::cout << '\t' << t_srcPiece << ::std::endl;
			
			if(!p_underline.empty())
			{
				// Check if it is inside the displayed range
				if((p_range.start() > p_underline.start()) || (p_range.end() < p_underline.end()))
					throw ::std::runtime_error("post_source_view: Underline source range not inside of display range");
				
				// Check if caret is inside of underline range
				if(p_caretOffset >= p_underline.length())
					throw ::std::runtime_error("post_source_view: Caret offset out of range");
				
				// Convert to relative (based on start of displayed source range)
				source_range t_relative{
					p_underline.start() - p_range.start(),
					p_underline.length() // This is intended, the length becomes the end marker
										 // because it is relative now.
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
				t_underlineStr.at(p_caretOffset) = '^';
							
				// Display
				::std::cout << '\t'
							<< ut::foreground(ut::console_color::bright_green)
							<< t_underlineStr
							<< ut::reset_color
							<< ::std::endl;
			}	
		}
			
		auto post_fixit(ut::string_view p_src, source_range p_range, source_range p_underline, ut::string_view p_fix, ::std::size_t p_caretOffset)
			-> void
		{
			// Underline is not optional here
			if(p_underline.empty())
				throw ::std::runtime_error("post_fixit: Underline range ist empty");
				
			// Fix string must not be empty
			if(p_fix.empty())
				throw ::std::runtime_error("post_fixit: Fixit hint string is empty");
		
			// Write out source view and squiggly lines
			post_source_view(p_src, p_range, p_underline, p_caretOffset = 0U);
							
			// Convert to relative (based on start of displayed source range)
			source_range t_relative{
				p_underline.start() - p_range.start(),
				p_underline.length() // This is intended, the length becomes the end marker
									 // because it is relative now.
			};
			
			::std::cout << '\t';
			
			// Write spaces to the output stream to correctly align the fix hint.
			// This dependends on the start of the underline (given as t_relative)
			// and the caret offset
			::std::fill_n(::std::ostream_iterator<char>(::std::cout), t_relative.start() + p_caretOffset, ' ');
			
			// Write hint
			::std::cout << ut::foreground(ut::console_color::bright_green)
						<< p_fix
						<< ut::reset_color
						<< ::std::endl;
		}
	}
}
