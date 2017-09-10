#pragma once

#include <string>
#include <functional>

#include "enumerations.hxx"

namespace cl
{
	namespace internal
	{		
		// Struct holding all command data that can be accessed
		// by local option tags
		struct command_data
		{
			::std::string m_Usage;
		};
	}
}
