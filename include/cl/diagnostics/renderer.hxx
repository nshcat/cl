#pragma once

#include "output.hxx"
#include "message.hxx"
#include "simple_message.hxx"
#include "location_message.hxx"
#include "combined_message.hxx"
#include "source_view.hxx"
#include "fixit_hint.hxx"

namespace cl
{
	namespace diagnostics
	{
		class renderer
		{
			// TODO: Provide default implementation for combined_message, so that
			//		 specific renderers do not need to replicate that code.
			public:
				virtual auto render(const simple_message&, output&) -> void = 0;
				virtual auto render(const location_message&, output&) -> void = 0;
				virtual auto render(const source_view&, output&) -> void = 0;
				virtual auto render(const fixit_hint&, output&) -> void = 0;
				virtual auto render(const combined_message&, output&) -> void; //< We supply a default implementation here
		};
	}
}
