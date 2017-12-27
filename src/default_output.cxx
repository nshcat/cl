#include <diagnostics/default_output.hxx>

namespace cl
{
	namespace diagnostics
	{
		auto default_output::error_stream()
			-> ::std::ostream&
		{
			return ::std::cerr;
		}
		
		auto default_output::output_stream()
			-> ::std::ostream&
		{
			return ::std::cout;
		}
	}
}
