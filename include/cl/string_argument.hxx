#pragma once

#include <string>
#include "readable_base.hxx"

namespace cl
{
	class string_argument
		: public internal::readable_base<::std::string>
	{
		using Tbase = internal::readable_base<::std::string>;

		public:
			template< typename... Ttags >
			string_argument(Ttags&&... p_tags)
				: Tbase{}
			{
				this->dispatch_all<string_argument>(::std::forward<Ttags>(p_tags)...);
			}

		public:
			using Tbase::dispatch;
	};
}