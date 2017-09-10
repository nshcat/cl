#pragma once

#include <string>
#include <functional>

#include "enumerations.hxx"

namespace cl
{
	namespace internal
	{		
		using error_handler_ = ::std::function<void(const ::std::string&)>;
			
		// Struct holding all handler data that can be accessed
		// by global option tags
		struct handler_data
		{
			::std::string m_ApplicationName;
			::std::string m_ApplicationVersion;
			error_mode_ m_ErrorMode;
			error_handler_ m_ErrorHandler;
			help_mode_ m_HelpMode;
		};
	}
}
