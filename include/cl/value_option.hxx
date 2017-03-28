#pragma once

#include <utility>
#include <type_traits>
#include <ut/type_traits.hxx>
#include "handler_data.hxx"
#include "option_base.hxx"

namespace cl
{
	namespace internal
	{
		template< typename V, V handler_data::* Ptr >
		class value_option
			: public option_base
		{
			public:
				value_option(const V& p_val)
					: m_Val(p_val)
				{
						
				}
				
			public:
				auto apply(handler_data& p_data) const &
					-> void
				{
					p_data.*Ptr = m_Val;
				}

				auto apply(handler_data& p_data) &&
					-> void
				{
					// this instance is a rvalue, take advantage by moving value
					p_data.*Ptr = ::std::move(m_Val);
				}
			
			private:
				V m_Val;
		};
	}
}
