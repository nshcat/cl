#include <diagnostics/default_renderer.hxx>
#include <diagnostics/low_level.hxx>

namespace cl
{
	namespace diagnostics
	{
		auto default_renderer::render(const simple_message& p_msg, output& p_out)
			-> void
		{
			post_diagnostic(p_out.output_stream(),
							p_msg.level(),
							p_msg.sender(),
							{},
							p_msg.message()
			);
		}
		
		auto default_renderer::render(const location_message& p_msg, output& p_out)
			-> void
		{
			post_diagnostic(p_out.output_stream(),
							p_msg.level(),
							p_msg.sender(),
							p_msg.location(),
							p_msg.message()
			);
		}
		
		auto default_renderer::render(const source_view& p_msg, output& p_out)
			-> void
		{
			post_source_view(p_out.output_stream(),
							 p_msg.source(),
							 p_msg.range(),
							 p_msg.underline_range(),
							 p_msg.offset()						
			);
		}
		
		auto default_renderer::render(const fixit_hint& p_msg, output& p_out)
			-> void
		{
			post_fixit(p_out.output_stream(),
					   p_msg.source(),
					   p_msg.range(),
					   p_msg.underline_range(),
					   p_msg.hint(),
					   p_msg.offset()						
			);
		}
	}
}
