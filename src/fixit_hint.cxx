#include <diagnostics/renderer.hxx>
#include <diagnostics/fixit_hint.hxx>

namespace cl
{
	namespace diagnostics
	{
		fixit_hint::fixit_hint(copy_source_t,
				   			   ut::string_view p_src,
				   			   const source_range& p_range,
			       			   const source_range& p_underline,
			       			   ut::string_view p_hint,
				   			   offset_type p_caretOffset)
			: source_view(copy_source_t{}, p_src, p_range, p_underline, p_caretOffset), m_Hint{ p_hint.to_string() }
		{
		}
		
		fixit_hint::fixit_hint(ref_source_t,
				   			   ut::string_view p_src,
				   			   const source_range& p_range,
			       			   const source_range& p_underline,
			       			   ut::string_view p_hint,
				   			   offset_type p_caretOffset)
			: source_view(ref_source_t{}, p_src, p_range, p_underline, p_caretOffset), m_Hint{ p_hint.to_string() }
		{
		}
		
		auto fixit_hint::hint() const
			-> ut::string_view
		{
			return { m_Hint };
		}
		
		auto fixit_hint::apply_renderer(renderer& p_rndr, output& p_out) const
			-> void
		{
			p_rndr.render(*this, p_out);
		}
	}
}

