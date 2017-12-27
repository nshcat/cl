#pragma once

#include <diagnostics/output.hxx>
#include <diagnostics/message.hxx>
#include <diagnostics/simple_message.hxx>

namespace cl
{
	namespace diagnostics
	{
		class renderer
		{
			public:
				virtual auto render(const simple_message&, output&) -> void = 0;
		};
	}
}
