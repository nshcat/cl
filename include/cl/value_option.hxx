#pragma once

#include <utility>
#include <type_traits>
#include <ut/type_traits.hxx>

namespace cl
{
	namespace internal
	{
		template< typename TBase, typename TTarget, typename TValue, TValue TTarget::* Ptr >
		class value_option
			: public TBase
		{
			public:
				value_option(const TValue& p_val)
					: m_Val(p_val)
				{
						
				}
				
			public:
				auto apply(TTarget& p_data) const &
					-> void
				{
					p_data.*Ptr = m_Val;
				}

				auto apply(TTarget& p_data) &&
					-> void
				{
					// this instance is a rvalue, take advantage by moving value
					p_data.*Ptr = ::std::move(m_Val);
				}
			
			private:
				TValue m_Val;
		};
	}
}
