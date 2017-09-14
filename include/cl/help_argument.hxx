#pragma once

#include "flag_argument.hxx"

namespace cl
{
	class help_argument /* Special argument that is used to trigger help display */
		: public flag_argument
	{
		using Tthis = help_argument;
		using Tbase = flag_argument;
		
		public:
			template< typename... Ttags >
			help_argument(Ttags&&... p_tags)
				: Tbase{ ::std::forward<Ttags>(p_tags)... }
			{
			}
	};
}
