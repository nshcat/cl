#pragma once

#include <string>
#include <vector>

#include <ut/string_view.hxx>

namespace cl
{
	namespace internal
	{
		struct from_length_t
		{
		};
		
		static const from_length_t from_length{ };
	
		// A single location in the source code
		class source_location
		{
			using size_type = ::std::size_t;
			
			public:
				static const size_type npos = size_type(-1);
			
			public:
				source_location() = default;
			
				source_location(size_type p_line, size_type p_col)
					: m_Line{p_line}, m_Col{p_col}
				{
				}
			
			public:
				auto column() const
					-> size_type
				{
					return m_Col;
				}
				
				auto line() const
					-> size_type
				{
					return m_Line;
				}
			
			private:
				size_type m_Col{ npos };
				size_type m_Line{ npos };
		};
	
		// A range in one line of source code.
		class source_range
		{
			using size_type = ::std::size_t;
		
			public:
				static const size_type npos = size_type(-1);
		
			public:
				source_range() = default;
			
				source_range(size_type p_start, size_type p_end, size_type p_line = { })
					: m_Start{p_start}, m_End{p_end}, m_Line{p_line}
				{
				}
				
				source_range(from_length_t, size_type p_start, size_type p_length, size_type p_line = { })
					: m_Start{p_start}, m_End{p_start + p_length}, m_Line{p_line}
				{
				}
				
			public:
				auto reset()
					-> void
				{
					m_Start = m_End = npos;
				}
		
			public:
				auto location() const
					-> source_location
				{
					return { line(), start() };
				}
			
				auto start() const
					-> size_type
				{
					return m_Start;
				}
				
				auto end() const
					-> size_type
				{
					return m_End;
				}
				
				auto length() const
					-> size_type
				{
					if(m_Start == npos || m_End == npos)
						return 0;
					else return m_End - m_Start;
				}
				
				auto line() const
					-> size_type
				{
					return m_Line;
				}
				
				auto empty() const
					-> bool
				{
					return (m_Start == npos || m_End == npos);
				}
		
			private:
				size_type m_Start{ npos };
				size_type m_End{ npos };
				size_type m_Line{ npos };
		};
	
		class diagnostics_state final
		{
			using range_container_type = ::std::vector<source_range>;
		
			public:
				diagnostics_state(const ::std::string& p_src)
					: m_Source{p_src}
				{
				}
				
				diagnostics_state()
					: m_Source{}
				{
				}
				
			public:
				diagnostics_state(const diagnostics_state&) = default;
				diagnostics_state(diagnostics_state&&) = default;
				
				diagnostics_state& operator=(const diagnostics_state&) = default;
				diagnostics_state& operator=(diagnostics_state&&) = default;
				
			public:
				auto cleanup()
					-> void
				{
					m_ArgumentRange.reset();
					m_NameRange.reset();
					m_ValueRanges.clear();
				}
				
			public:
				auto source() const
					-> ut::string_view
				{
					return { m_Source };
				}
				
				auto argument_range() const
					-> const source_range&
				{
					return m_ArgumentRange;
				}
				
				auto name_range() const
					-> const source_range&
				{
					return m_NameRange;
				}
				
				auto value_ranges() const
					-> const range_container_type&
				{
					return m_ValueRanges;
				}
				
				auto argument_range()
					-> source_range&
				{
					return m_ArgumentRange;
				}
				
				auto name_range()
					-> source_range&
				{
					return m_NameRange;
				}
				
				auto value_ranges()
					-> range_container_type&
				{
					return m_ValueRanges;
				}
				
			private:
				::std::string m_Source;				//< Copy of the input string
				source_range m_ArgumentRange;		//< Source range of whole argument (assignment or not)
				source_range m_NameRange;			//< Source range of argument name
				range_container_type m_ValueRanges;	//< Source ranges of all values following the assignment
													//  (includes the value bound by assignment) (might be empty)
		};
	}
}
