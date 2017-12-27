#pragma once

#include <iostream>

namespace cl
{
	namespace diagnostics
	{
		class output
		{
			public:
				virtual auto error_stream() -> ::std::ostream& = 0;
				virtual auto output_stream() -> ::std::ostream& = 0;
		};
	}
}
