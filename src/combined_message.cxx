#include <utility>
#include <diagnostics/combined_message.hxx>
#include <diagnostics/renderer.hxx>

namespace cl
{
	namespace diagnostics
	{
		auto combined_message::apply_renderer(renderer& p_rndr, output& p_out) const
			-> void
		{
			p_rndr.render(*this, p_out);
		}
		
		auto combined_message::add(message_ptr&& p_ptr)
			-> void
		{
			m_Cont.push_back(::std::move(p_ptr));
		}
		
		auto combined_message::begin()
			-> iterator
		{
			return m_Cont.begin();
		}
			
		auto combined_message::begin() const
			-> const_iterator
		{
			return m_Cont.cbegin();
		}
			
		auto combined_message::cbegin() const
			-> const_iterator
		{
			return m_Cont.cbegin();
		}
			
		auto combined_message::end()
			-> iterator
		{
			return m_Cont.end();
		}
			
		auto combined_message::end() const
			-> const_iterator
		{
			return m_Cont.cend();
		}
			
		auto combined_message::cend() const
			-> const_iterator
		{
			return m_Cont.cend();
		}
	}
}
