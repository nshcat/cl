#include <diagnostics/renderer.hxx>

namespace cl
{
	namespace diagnostics
	{
		auto renderer::render(const combined_message& p_msg, output& p_out)
			-> void
		{
			for(const auto& t_msg: p_msg)
				t_msg->apply_renderer(*this, p_out);
		}
	}
}
