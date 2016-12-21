#pragma once

#include <type_traits>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "tags.hxx"
#include "multi_base.hxx"
#include "default_reader.hxx"

namespace cl
{
	// Will not throw if conversion is not possible.
	template<	typename T = ::std::string,
				typename TReader = default_reader<T>
	>
	class free_argument /* Argument type that consumes all values that are not consumed by anything else */
		: public internal::multi_base<T, TReader, internal::multi_mode::positional>
	{
		using Tthis = free_argument<T, TReader>;
		using Tbase = internal::multi_base<T, TReader, internal::multi_mode::positional>;

		public:
			template< typename... Ttags >
			free_argument(Ttags&&... p_tags)
				: Tbase{}
			{
				// argument_base::dispatch_all can't be used here since
				// free_argument does not need a long_name to be set.
				
				// Dispatch all tags
				std::initializer_list<int> tmp = { (dispatch(::std::forward<Ttags>(p_tags)), 0)... };

				// Silence "not used" warning
				(void)tmp;
			}

		public:
			using Tbase::dispatch;

			// Block long_name from being set // TODO better error message
			void dispatch(const internal::long_name_t&) = delete;

			// Block short_name from being set
			void dispatch(const internal::short_name_t&) = delete;
	};
}
