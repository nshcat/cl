#pragma once

#include <string>
#include <functional>

#include <ut/bitmask.hxx>

namespace cl
{
	namespace internal
	{
		// Enumeration describing action taken on error
		enum class error_mode_
		{
			throw_exception,	/* throw exception on failure */
			terminate,			/* show help according to help_mode and terminate */
			return_value,		/* indicate failure using boolean return value */
			custom_handler		/* call custom handler on failure. Will throw if no handler was set. */			
		};
		
		// Enumeration describing how help should be shown to the user
		// TODO maybe remove prefix here
		enum class help_mode_
		{
			show_none 		= 0x0,	/* Don't do anything. This is the default value. */
			show_usage 		= 0x1,	/* Show a general usage string that can be set by cl::usage. */
			show_summary 	= 0x2,	/* Show a summary of all registered options. */
			on_error		= 0x4,	/* Show help on error. */
		};
		
		// The help_mode_ enumeration should be usable as a bitmask type.
		LIBUT_MAKE_BITMASK(help_mode_)
		
		// Summary display style
		enum class summary_style_
		{
			spaces,
			dots			
		};
	}
}
