#pragma once

#include <diagnostics/output.hxx>

namespace cl
{
	namespace diagnostics
	{
		class renderer;
	
		class message
		{
			public:
				virtual auto apply_renderer(renderer&, output&) -> void = 0;			
		};
	}
}
