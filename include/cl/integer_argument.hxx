#pragma once
#include <limits>
#include <vector>
#include <cinttypes>
#include <type_traits>
#include <ut/throwf.hxx>

#include "arithmetic_base.hxx"


namespace cl
{
	template< typename T >
	class integer_argument /* integral value argument */
		: public internal::arithmetic_base<T>
	{
		static_assert(std::is_integral<T>::value,
			"Type T is not an integral value type!");

		using Tbase = internal::arithmetic_base<T>;

		public:
			template< typename... Ttags >
			integer_argument(const Ttags&... p_tags)
				: Tbase{}, m_HasMax(false), m_HasMin(false), m_Clamp(false)
			{
				this->template dispatch_all<integer_argument<T>>(p_tags...);	
			}

		public:
			using Tbase::dispatch;

		public:
			virtual void read(std::list<std::string>& p_vals, bool) override
			{
				// Check if value list is empty
				if (p_vals.empty())
				{
					ut::throwf<std::runtime_error>("No value supplied for given argument \"--%s\"!", this->m_LongName.c_str());
				}

				// Try to convert value string to integral value. This will throw if invalid or out of range.
				try
				{
					this->m_Value = this->parse_value(std::is_signed<T>{}, p_vals.front());
				}
				catch (...)
				{
					ut::throwf<std::runtime_error>("Supplied value for \"--%s\" invalid or out of range: \"%s\"", this->m_LongName.c_str(), p_vals.front().c_str());
				}

				// Consume value from list
				p_vals.pop_front();
				this->m_Supplied = true;
			}

		protected:
			T parse_value(std::false_type, const std::string& p_str)			
			{
				auto val = std::stoull(p_str);

				if (val <= std::numeric_limits<T>::max())
				{
					return static_cast<T>(val);
				}
				else throw std::runtime_error("Out of range!");
			}


			T parse_value(std::true_type, const std::string& p_str)
			{
				auto val = std::stoll(p_str);

				if (val >= std::numeric_limits<T>::min() &&
					val <= std::numeric_limits<T>::max())
				{
					return static_cast<T>(val);
				}
				else throw std::runtime_error("Out of range!");	
			}

		protected:
			bool	m_HasMax;
			T		m_Max;
			bool	m_HasMin;
			T		m_Min;
			bool	m_Clamp;
	};
}
