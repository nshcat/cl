#pragma once

#include "output.hxx"

namespace cl
{
	namespace diagnostics
	{
		class default_output
			: public output
		{
			public:
				virtual auto error_stream() -> ::std::ostream& override;
				virtual auto output_stream() -> ::std::ostream& override;
		};
	}
}
