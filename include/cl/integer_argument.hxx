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
	class integer_argument /* integral value argument */
		: public internal::arithmetic_base<T>
	{
		static_assert(::std::is_integral<T>::value,
			"Type T is not an integral value type!");

		using Tbase = internal::arithmetic_base<T>;

		public:
			template< typename... Ttags >
			integer_argument(Ttags&&... p_tags)
				: Tbase{}
			{
				this->template dispatch_all<integer_argument<T>>(::std::forward<Ttags>(p_tags)...);	
			}

		public:
			using Tbase::dispatch;
	};
}
