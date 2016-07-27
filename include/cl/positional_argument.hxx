#pragma once

#include <type_traits>
#include <stdexcept>
#include <iterator>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "tags.hxx"
#include "value_base.hxx"

namespace cl
{
	class positional_argument /* Flag element. Can be used without argument (flag) or with argument (bool value) */
		: public internal::value_base<std::vector<std::string>>
	{
		using Tbase = internal::value_base<std::vector<std::string>>;

		public:
			template< typename... Ttags >
			positional_argument(const Ttags&... p_tags)
				: Tbase{}
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
			void read(std::list<std::string>& p_args, bool) override
			{
				// Copy even if p_args is empty.
				// this->m_Value = p_args;
				std::copy(p_args.begin(), p_args.end(), std::back_inserter(this->m_Value));

				// Consume all values
				p_args.clear();

				this->m_Supplied = true;
			}


		protected:
			using Tbase::dispatch;
	};
}
