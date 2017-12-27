#include <diagnostics/types.hxx>


namespace cl
{
	namespace diagnostics
	{
		// --- source_location implementation
		auto source_location::column() const
			-> size_type
		{
			return m_Col;
		}
		
		auto source_location::line() const
			-> size_type
		{
			return m_Line;
		}
		
		source_location::source_location(size_type p_line, size_type p_col)
			: m_Line{p_line}, m_Col{p_col}
		{
		}
		// ---
		
		
		// --- source_range implementation
		source_range::source_range(size_type p_start, size_type p_end, size_type p_line)
			: m_Start{p_start}, m_End{p_end}, m_Line{p_line}
		{
		}
		
		source_range::source_range(from_length_t, size_type p_start, size_type p_length, size_type p_line)
			: m_Start{p_start}, m_End{p_start + p_length}, m_Line{p_line}
		{
		}
		
		auto source_range::reset()
			-> void
		{
			m_Start = m_End = npos;
		}
		
		auto source_range::location() const
			-> source_location
		{
			return { line(), start() };
		}
		
		auto source_range::start() const
			-> size_type
		{
			return m_Start;
		}
		
		auto source_range::end() const
			-> size_type
		{
			return m_End;
		}
				
		auto source_range::length() const
			-> size_type
		{
			if(m_Start == npos || m_End == npos)
				return 0;
			else return m_End - m_Start;
		}
		
		auto source_range::line() const
			-> size_type
		{
			return m_Line;
		}
		
		auto source_range::empty() const
			-> bool
		{
			return (m_Start == npos || m_End == npos) || (length() == 0U);
		}
		// ---
	}
}
