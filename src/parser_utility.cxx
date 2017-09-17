#include <cstdlib>

#include <parser_utility.hxx>
#include <diagnostics.hxx>

namespace cl
{
	namespace internal
	{
		auto report_invalid(ut::string_view p_name, bool p_isShort, const diagnostics_state& p_state)
			-> void
		{
			post_diagnostic(
				diagnostics_level::error,
				"commandline",
				p_state.name_range().location(),
				"Unknown argument: \"%s%s\"",
				(p_isShort ? "-" : "--"),
				p_name
			);
			
			post_source_view(
				p_state.source(),
				p_state.argument_range(),
				p_state.name_range(),
				leftmost
			);
			
			::std::exit(EXIT_FAILURE);
		}
	}
}
