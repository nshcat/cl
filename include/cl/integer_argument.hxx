#pragma once
#include <limits>
#include <vector>
#include <cinttypes>
#include <type_traits>
#include <ut/throwf.hxx>

#include "value_base.hxx"


namespace cl
{
	template< typename T >
	class integer_argument /* integral value argument */
		: public internal::value_base<T>
	{
		static_assert(std::is_integral<T>::value,
			"Type T is not an integral value type!");

		using Tbase = internal::value_base<T>;

		public:
			template< typename... Ttags >
			integer_argument(const Ttags&... p_tags)
				: Tbase{}, m_HasMax(false), m_HasMin(false), m_Clamp(false)
			{
				// Check for at least one long_name_t
				static_assert(ut::contains<internal::long_name_t, std::decay_t<Ttags>...>::value,
					"Long name tag is required for all argument types");

				// Dispatch all tags
				std::initializer_list<int> tmp = { (dispatch(p_tags), 0)... };

				// Silence "not used" warning
				(void)tmp;

				// Check if long name was set.
				if (this->long_name().empty())
					throw std::runtime_error("Long name was not set!");
			}

		protected:
			using Tbase::dispatch;

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
					else ut::throwf<std::runtime_error>(out_of_range_fmt(),
						this->m_LongName.c_str(), integer_promote(this->m_Value), "Minimum", integer_promote(m_Min));
				}
				else if (m_HasMax && this->m_Value > m_Max)
				{
					if (m_Clamp)
						this->m_Value = m_Max;
					else ut::throwf<std::runtime_error>(out_of_range_fmt(),
						this->m_LongName.c_str(), integer_promote(this->m_Value), "Maximum", integer_promote(m_Max));
				}
			}

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

				// Check range for clamping
				check_range();
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

		private:
			// Format string for out of range exception message
			constexpr static const char* out_of_range_fmt()
			{
				return out_of_range_fmt_(std::is_integral<T>{});
			}

			constexpr static const char* out_of_range_fmt_(std::true_type)
			{
				return "Supplied value for \"--%s\" out of range: \"%" PRId64 "\" [%s: %" PRId64 "]";
			}

			constexpr static const char* out_of_range_fmt_(std::false_type)
			{
				return "Supplied value for \"--%s\" out of range: \"%" PRIu64 "\" [%s: %" PRIu64 "]";
			}

			// Promote integer to biggest possible type
			auto integer_promote(T p_value)
			{
				return integer_promote_(std::is_integral<T>{}, p_value);
			}

			long long integer_promote_(std::true_type, T p_value)
			{
				return static_cast<long long>(p_value);
			}

			unsigned long long integer_promote_(std::false_type, T p_value)
			{
				return static_cast<unsigned long long>(p_value);
			}

		protected:
			bool	m_HasMax;
			T		m_Max;
			bool	m_HasMin;
			T		m_Min;
			bool	m_Clamp;
	};
}
