#pragma once

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
				void apply(handler_data& p_data) const
				{
					p_data.*Ptr = m_Val;
				}
			
			private:
				V m_Val;
		};
	}
}
