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
			
		using error_handler_ = ::std::function<void(const ::std::string&)>;
			
		// Struct holding all handler data that can be accessed
		// by global option tags
		struct handler_data
		{
			::std::string m_ApplicationName;
			::std::string m_ApplicationVersion;
			error_mode_ m_ErrorMode;
			error_handler_ m_ErrorHandler;
			bool m_PrintHelp{true};
		};
	}
}