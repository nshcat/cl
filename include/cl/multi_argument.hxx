#pragma once

#include <type_traits>
#include <stdexcept>
#include <iterator>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "tags.hxx"
#include "multi_base.hxx"
#include "default_reader.hxx"

namespace cl
{
	template<	typename T,
				typename TReader = default_reader<T>
	>
	class multi_argument /* Argument type that only matches one value per occurence. */
		: public internal::multi_base<T, TReader, internal::multi_mode::multi>
	{
		using base_type = internal::multi_base<T, TReader, internal::multi_mode::multi>;
		using this_type = multi_argument<T, TReader>;
		
		public:
			template< typename... Ttags >
			multi_argument(Ttags&&... p_tags)
				: base_type{}
			{
				this->template dispatch_all<this_type>(::std::forward<Ttags>(p_tags)...);
			}

		public:
			using base_type::dispatch;	
	};
}
