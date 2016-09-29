#pragma once

#include <type_traits>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <ut/throwf.hxx>
#include "tags.hxx"
#include "readable_base.hxx"


namespace cl
{
	class boolean_argument /* Flag element. Can be used without argument (flag) or with argument (bool value) */
		: public internal::readable_base<bool>
	{
		using Tbase = internal::readable_base<bool>;

		public:
			template< typename... Ttags >
			boolean_argument(const Ttags&... p_tags)
				: Tbase{}, m_IsInverse(false)
			{
				this->dispatch_all<boolean_argument>(p_tags...);
			}

		public:
			virtual void read(::std::list<::std::string>& p_val, bool p_isAssignment) override
			{
				if(p_isAssignment) /* User provided actual value. Behave like value_base<bool>. */
				{
					Tbase::read(p_val, p_isAssignment);
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

		public:
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
