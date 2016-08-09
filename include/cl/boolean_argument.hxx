#pragma once

#include <type_traits>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <ut/throwf.hxx>
#include "tags.hxx"
#include "value_base.hxx"


namespace cl
{
	class boolean_argument /* Flag element. Can be used without argument (flag) or with argument (bool value) */
		: public internal::value_base<bool>
	{
		using Tbase = internal::value_base<bool>;

		public:
			template< typename... Ttags >
			boolean_argument(const Ttags&... p_tags)
				: Tbase{}, m_IsInverse(false)
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

		public:
			virtual void read(std::list<std::string>& p_val, bool p_IsAssignment) override
			{
				if(p_IsAssignment) /* User provided actual value. Behave like value_base<bool>. */
				{
					// We are garuantueed to have a value here.
					std::string t_str{ p_val.front() };

					// Consume value
					p_val.pop_front();

					// Try to parse as boolalpha.
					// std::for_each(t_str.begin(), t_str.end(), [](char& c) { c = tolower(c); });
					std::transform(t_str.begin(), t_str.end(), t_str.begin(),
						[](char c) -> char
						{
							return tolower(c);
						}
					);

					if (t_str.compare("true") == 0)
						this->m_Value = !m_IsInverse;
					else if (t_str.compare("false") == 0)
						this->m_Value = m_IsInverse;
					else ut::throwf<std::runtime_error>("Supplied value for \"--%s\" invalid or out of range: \"%s\"", this->m_LongName.c_str(), t_str.c_str());
				}
				else /* Used as a flag. Behave like a switch. */
				{
					this->m_Value = !m_IsInverse;
				}		

				this->m_Supplied = true;
			}

			virtual bool is_switch() override
			{
				return true;
			}

		protected:
			using Tbase::dispatch;

			// Inverse
			void dispatch(internal::inverse_t)
			{
				m_IsInverse = true;
			}


		private:
			bool	m_IsInverse;
	};
}
