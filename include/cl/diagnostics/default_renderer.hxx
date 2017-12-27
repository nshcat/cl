#pragma once

#include <diagnostics/renderer.hxx>

namespace cl
{
	namespace diagnostics
	{
		class default_renderer
			: public renderer
		{
			virtual auto render(const simple_message&, output&)
				-> void override;
		};
	}
}
