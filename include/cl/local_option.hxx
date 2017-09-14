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
		
		template< typename TValue, TValue command_data::* Ptr >
		using local_value_option = value_option<
			local_option_base,
			command_data,
			TValue,
			Ptr
		>;
	}
	
	static auto usage(const ::std::string& p_val)
		-> internal::local_value_option<::std::string, &internal::command_data::m_Usage>
	{
		return { p_val };
	}
}
