#pragma once

#include <ut/string_view.hxx>
#include "diagnostics_state.hxx"

namespace cl
{
	namespace internal
	{
		auto report_invalid(ut::string_view p_name, bool p_isShort, const diagnostics_state& p_state)
			-> void;
	}
}
