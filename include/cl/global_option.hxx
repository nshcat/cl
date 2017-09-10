#pragma once

#include <string>

#include "handler_data.hxx"
#include "value_option.hxx"


namespace cl
{
	namespace internal
	{
		struct global_option_base
		{
		};
		
		template< typename TValue, TValue handler_data::* Ptr >
		using global_value_option = value_option<
			global_option_base,
			handler_data,
			TValue,
			Ptr
		>;
	}

	constexpr static auto throw_exception = internal::error_mode_::throw_exception;
	constexpr static auto return_value = internal::error_mode_::return_value;
	constexpr static auto custom_handler = internal::error_mode_::custom_handler;
	constexpr static auto terminate = internal::error_mode_::terminate;
	
	// TODO put in source file
	static auto error_mode(internal::error_mode_ p_val)
		-> internal::global_value_option<internal::error_mode_, &internal::handler_data::m_ErrorMode>
	{
		return { p_val };
	}
	
	static auto error_handler(internal::error_handler_ p_val)
		-> internal::global_value_option<internal::error_handler_, &internal::handler_data::m_ErrorHandler>
	{
		return { p_val };
	}
	
	// TODO maybe remove prefix here? Seems kinda redundand.
	static auto application_name(const ::std::string& p_val)
		-> internal::global_value_option<::std::string, &internal::handler_data::m_ApplicationName>
	{
		return { p_val };
	}
	
	static auto application_version(const ::std::string& p_val)
		-> internal::global_value_option<::std::string, &internal::handler_data::m_ApplicationVersion>
	{
		return { p_val };
	}
	
	static auto application_usage(const ::std::string& p_val)
		-> internal::global_value_option<::std::string, &internal::handler_data::m_ApplicationUsage>
	{
		return { p_val };
	}	
}
