#pragma once
#include <limits>
#include <vector>
#include <cinttypes>
#include <type_traits>
#include <ut/throwf.hxx>

#include "arithmetic_base.hxx"


namespace cl
{
	template< typename T >
	class floating_argument /* integral value argument */
		: public internal::arithmetic_base<T>
	{
		static_assert(::std::is_floating_point<T>::value,
			"Type T is not an floating point value type!");

		using Tbase = internal::arithmetic_base<T>;

		public:
			template< typename... Ttags >
			floating_argument(const Ttags&... p_tags)
				: Tbase{}
			{
				this->template dispatch_all<floating_argument<T>>(p_tags...);	
			}

		public:
			using Tbase::dispatch;
	};
}
