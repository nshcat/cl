#pragma once

#include <string>
#include <functional>

namespace cl
{
	namespace internal
	{
		// Enumeration describing action taken on error
		enum class error_mode_
		{
			throw_exception,	/* throw exception on failure */
			terminate,			/* terminate on failure */
			return_value,		/* indicate failure using boolean return value */
			custom_handler		/* call custom handler on failure. Will throw if no handler was set. */			
		};
	}
}