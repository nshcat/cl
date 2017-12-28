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
				
			virtual auto render(const source_view&, output&)
				-> void override;
				
			virtual auto render(const fixit_hint&, output&)
				-> void override;
		};
	}
}
