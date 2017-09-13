#pragma once

#include <string>

namespace cl
{
	namespace internal
	{
		auto insert_line_breaks(const ::std::string& p_in, ::std::string::size_type p_lineLength, ::std::string::size_type p_offset)
			-> ::std::string;
	}
}
