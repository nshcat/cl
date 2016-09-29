#pragma once

#include <type_traits>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <limits>
#include "tags.hxx"
#include "readable_base.hxx"

namespace cl
{
	namespace internal
	{
		template< typename T >
		class arithmetic_base /* Base for arguments that supply an arithmetic value of type T */
			: public readable_base<T>
		{
			using TBase = readable_base<T>;
			
			static_assert(std::is_arithmetic<T>::value,
				"T needs to be an arithmetic type!");
			
			public:
				arithmetic_base()
					: TBase{}, m_Clamp{false}, m_HasMax{false}, m_HasMin{false}
				{
				}
				
			public:
				virtual void read_end() override
				{
					// Check for out of range
					check_range();
					
					// Call base class read_end().
					// This will update all references.
					this->value_base<T>::read_end();
				}

			public:
				using TBase::dispatch;

				void dispatch(const internal::min_t<T>& p_tag)
				{
					m_HasMin = true;
					m_Min = p_tag.value();
				}

				void dispatch(const internal::max_t<T>& p_tag)
				{
					m_HasMax = true;
					m_Max = p_tag.value();
				}
								
				void dispatch(const internal::range_t<T>& p_tag)
				{
					m_HasMin = m_HasMax = true;
					m_Min = p_tag.first();
					m_Max = p_tag.second();
				}

				void dispatch(internal::clamp_t)
				{
					m_Clamp = true;
				}

			protected:			
				void check_range()
				{
					if (m_HasMin && this->m_Value < m_Min)
					{
						if (m_Clamp)
							this->m_Value = m_Min;
						else range_error(true);
					}
					else if (m_HasMax && this->m_Value > m_Max)
					{
						if (m_Clamp)
							this->m_Value = m_Max;
						else range_error(false);
					}
				}
				
				// We can't use throwf here since we can't easily deduce matching printf specifiers for T.
				// Maybe later this can be done somehow, since the set of arithmetical types is relatively small.
				// But for now, stringstream it is.		
				void range_error(bool p_min)
				{
					std::ostringstream t_str;
					
					t_str	<< "Supplied value for \""
							<< this->m_LongName
							<< "\" out of range: \""
							<< this->m_Value
							<< "\" ["
							<< (p_min ? "Minium: " : "Maximum: ")
							<< (p_min ? m_Min : m_Max)
							<< "]";
							
					throw std::runtime_error(t_str.str());
				}			

			protected:
				bool	m_HasMax;
				T		m_Max;
				bool	m_HasMin;
				T		m_Min;
				bool	m_Clamp;
		};
	}
}