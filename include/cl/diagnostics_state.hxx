#pragma once

#include <string>
#include <vector>
#include <ios>
#include <ut/string_view.hxx>
#include "diagnostics.hxx"

namespace cl
{
	namespace internal
	{
		class diagnostics_state final
		{
			using range_container_type = ::std::vector<diagnostics::source_range>;
		
			public:
				diagnostics_state(const ::std::string& p_src)
					: m_Source{p_src}
				{
					// Filter out markers
					::std::transform(m_Source.begin(), m_Source.end(), m_Source.begin(),
						[](char c) -> char
						{
							if(c == '\x11')
								return ' ';
							else return c;
						}
					);
					//m_Source.erase(::std::remove(m_Source.begin(), m_Source.end(), '\x11'), m_Source.end());
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
					-> const diagnostics::source_range&
				{
					return m_ArgumentRange;
				}
				
				auto name_range() const
					-> const diagnostics::source_range&
				{
					return m_NameRange;
				}
				
				auto value_ranges() const
					-> const range_container_type&
				{
					return m_ValueRanges;
				}
				
				auto argument_range()
					-> diagnostics::source_range&
				{
					return m_ArgumentRange;
				}
				
				auto name_range()
					-> diagnostics::source_range&
				{
					return m_NameRange;
				}
				
				auto value_ranges()
					-> range_container_type&
				{
					return m_ValueRanges;
				}
				
			private:
				::std::string m_Source;							//< Copy of the input string
				diagnostics::source_range m_ArgumentRange;		//< Source range of whole argument (assignment or not)
				diagnostics::source_range m_NameRange;			//< Source range of argument name
				range_container_type m_ValueRanges;				//< Source ranges of all values following the assignment
																//  (includes the value bound by assignment) (might be empty)
		};
	}
}
