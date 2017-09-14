#pragma once

#include "switch_argument.hxx"

namespace cl
{
	class flag_argument /* Argument type that sets boolean flag, but is not assignable */
		: public switch_argument<bool>
	{
		using Tthis = flag_argument;
		using Tbase = switch_argument<bool>;

		public:
			template< typename... Ttags >
			flag_argument(Ttags&&... p_tags)
				: Tbase{ ::std::forward<Ttags>(p_tags)... }
			{
				this->m_SwValue = true;
			}
	};
}
