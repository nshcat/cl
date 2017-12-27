#include <diagnostics/simple_message.hxx>
#include <diagnostics/renderer.hxx>

namespace cl
{
	namespace diagnostics
	{
		simple_message::simple_message(diagnostics_level p_lvl, ::std::string p_sndr, ::std::string p_msg)
			: m_Level{p_lvl}, m_Sender{::std::move(p_sndr)}, m_Message{::std::move(p_msg)}
		{
		}
		
		
		auto simple_message::apply_renderer(renderer& p_r, output& p_out)
			-> void
		{
			p_r.render(*this, p_out);
		}
	}
}
