#pragma once

#include "output.hxx"

namespace cl
{
	namespace diagnostics
	{
		class renderer;
	
		class message
		{
			public:
				virtual auto apply_renderer(renderer&, output&) const -> void = 0;			
		};
	}
}
