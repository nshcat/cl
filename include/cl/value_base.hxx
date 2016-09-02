#pragma once

#include <type_traits>
#include <vector>
#include <stdexcept>
#include <limits>
#include "tags.hxx"
#include "ut/always_false.hxx"
#include "argument_base.hxx"

namespace cl
{
	namespace internal
	{
		template< typename T >
		class value_base /* Base for arguments that supply a value of type T */
			: public argument_base
		{
			public:
				value_base()
					: argument_base{}, m_HasReference(false), m_Value{}
				{
				}

			public:
				const T& value() const
				{
					return m_Value;
				}

			public:
				virtual void read_end() override
				{
					// Refresh references, if any
					refresh_references();
				}

			public:
				using argument_base::dispatch;

				// Set default value
				void dispatch(const internal::default_value_t<T>& p_tag) /* Could be complex object, so reference it here */
				{
					m_Value = p_tag.value();
				}

				// Set reference
				void dispatch(const internal::reference_t<T>& p_tag)
				{
					m_HasReference = true;
					m_References.push_back(p_tag.ptr());
				}
				
				// Provide better error message if reference is mismatching
				template< typename U >
				void dispatch(reference_t<U>)
				{
					static_assert( ut::always_false<U>::value,
						"Reference tag with mismatching type supplied!" );
				}

			protected:
				void refresh_references()
				{
					if (m_HasReference)
						for (auto ptr : m_References)
							*ptr = m_Value;
				}

			protected:
				T					m_Value;
				bool				m_HasReference;
				std::vector<T*>		m_References;
		};
	}
}