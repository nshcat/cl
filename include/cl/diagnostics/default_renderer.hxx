#pragma once

#include "renderer.hxx"

namespace cl
{
	namespace diagnostics
	{
		class default_renderer
			: public renderer
		{
			virtual auto render(const simple_message&, output&)
				-> void override;
				
			virtual auto render(const location_message&, output&)
				-> void override;
		};
	}
}
