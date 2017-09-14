#pragma once

#include <string>

namespace cl
{
	namespace internal
	{
		// Modifies string to word-wrap based on a maximum line length.
		// Optionally, an offset can be set to append spaces after each line break to
		// align all lines at a specific column.
		auto insert_line_breaks(const ::std::string& p_in, ::std::string::size_type p_lineLength, ::std::string::size_type p_offset = 0U)
			-> ::std::string;
	}
}
