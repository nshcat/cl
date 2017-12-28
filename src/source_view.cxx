#include <diagnostics/renderer.hxx>
#include <diagnostics/source_view.hxx>

namespace cl
{
	namespace diagnostics
	{
		source_view::source_view(copy_source_t,
							ut::string_view p_src,
							const source_range& p_range,
							const source_range& p_underline,
							offset_type p_caretOffset)
			: m_Source{ p_src.to_string() }, m_Offset{ p_caretOffset }, m_SrcRange{ p_range }, m_ULRange{ p_underline }
		{
		}
		
		source_view::source_view(ref_source_t,
							ut::string_view p_src,
							const source_range& p_range,
							const source_range& p_underline,
							offset_type p_caretOffset)
			: m_SourceRef{ p_src }, m_Offset{ p_caretOffset }, m_SrcRange{ p_range }, m_ULRange{ p_underline }
		{
		}
			
		auto source_view::source() const
			-> ut::string_view
		{
			if(m_Source)
				return { *m_Source };
			else return m_SourceRef;
		}
		
		auto source_view::offset() const
			-> offset_type
		{
			return m_Offset;
		}
		
		auto source_view::range() const
			-> const source_range&
		{
			return m_SrcRange;
		}
		
		auto source_view::underline_range() const
			-> const source_range&
		{
			return m_ULRange;
		}
		
		auto source_view::apply_renderer(renderer& p_rndr, output& p_out) const
			-> void
		{
			p_rndr.render(*this, p_out);
		}
	}
}
