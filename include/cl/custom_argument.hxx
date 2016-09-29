#pragma once
#include <limits>
#include <vector>
#include <cinttypes>
#include <type_traits>
#include <ut/throwf.hxx>

#include "readable_base.hxx"


namespace cl
{
	template< typename T, typename TReader = default_reader<T> >
	class custom_argument
		: public internal::readable_base<T, TReader>
	{
		using Tthis = custom_argument<T, TReader>;
		using Tbase = internal::readable_base<T, TReader>;

		public:
			template< typename... Ttags >
			custom_argument(const Ttags&... p_tags)
				: Tbase{}
			{
				this->template dispatch_all<Tthis>(p_tags...);	
			}

		public:
			using Tbase::dispatch;
	};
}
