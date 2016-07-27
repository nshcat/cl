#pragma once

#include <type_traits>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "tags.hxx"
#include "value_base.hxx"

namespace cl
{
	class free_argument /* Flag element. Can be used without argument (flag) or with argument (bool value) */
		: public internal::value_base<std::vector<std::string>>
	{
		using Tbase = internal::value_base<std::vector<std::string>>;

		public:
			template< typename... Ttags >
			free_argument(const Ttags&... p_tags)
				: Tbase{}
			{
				// Dispatch all tags
				std::initializer_list<int> tmp = { (dispatch(p_tags), 0)... };

				// Silence "not used" warning
				(void)tmp;
			}

		protected:
			using Tbase::dispatch;

			// Block long_name from being set // TODO better error message
			void dispatch(const internal::long_name_t&) = delete;

			// Block short_name from being set
			void dispatch(const internal::short_name_t&) = delete;

		public:
			virtual void read(std::list<std::string>& p_vals, bool) override
			{
				// Write all arguments to internal vector
				//this->m_Value = p_args;
				std::copy(p_vals.begin(), p_vals.end(), std::back_inserter(this->m_Value));

				// Consume all values
				p_vals.clear();

				this->m_Supplied = true;
			}

			virtual bool is_switch() override
			{
				return false;
			}

	};
}
