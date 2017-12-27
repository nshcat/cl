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
			diagnostics::post_diagnostic(
				::std::cout,
				diagnostics::diagnostics_level::error,
				"commandline",
				p_state.name_range().location(),
				"Unknown argument: \"%s%s\"",
				(p_isShort ? "-" : "--"),
				p_name
			);
			
			diagnostics::post_source_view(
				::std::cout,
				p_state.source(),
				p_state.argument_range(),
				p_state.name_range(),
				diagnostics::leftmost
			);
			
			::std::exit(EXIT_FAILURE);
		}
	}
}
