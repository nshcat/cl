#pragma once

#include <string>

#include "command_data.hxx"
#include "value_option.hxx"


namespace cl
{
	namespace internal
	{
		struct local_option_base
		{
		};
		
		template< typename TValue, TValue handler_data::* Ptr >
		using local_value_option = value_option<
			local_option_base,
			command_data,
			TValue,
			Ptr
		>;
	}
	
	constexpr static auto show_none = internal::help_mode_::show_none;
	constexpr static auto show_usage = internal::help_mode_::show_usage;
	constexpr static auto show_summary = internal::help_mode_::show_summary;
	constexpr static auto show_all = show_usage | show_summary;
	
	
	static auto help_mode(internal::help_mode_ p_val)
		-> internal::local_value_option<internal::help_mode_, &internal::command_data::m_HelpMode>
	{
		return { p_val };
	}
}
