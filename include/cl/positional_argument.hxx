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
				this->dispatch_all<positional_argument>(p_tags...);
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

		public:
			using Tbase::dispatch;
	};
}
