#pragma once

#include "output.hxx"
#include "message.hxx"
#include "simple_message.hxx"

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
