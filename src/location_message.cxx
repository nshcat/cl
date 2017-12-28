#include <diagnostics/location_message.hxx>
#include <diagnostics/renderer.hxx>

namespace cl
{
	namespace diagnostics
	{
		location_message::location_message(diagnostics_level p_lvl, ::std::string p_src, const source_location& p_loc, ::std::string p_msg)
			: simple_message(p_lvl, p_src, p_msg), m_Loc{p_loc}
		{
		}
		
		auto location_message::apply_renderer(renderer& p_rndr, output& p_out) const
			-> void
		{
			p_rndr.render(*this, p_out);
		}
		
		auto location_message::location() const
			-> const source_location&
		{
			return m_Loc;
		}
	}
}
