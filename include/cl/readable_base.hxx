#pragma once

#include <type_traits>
#include <vector>
#include <stdexcept>
#include <limits>
#include <ut/throwf.hxx>
#include "tags.hxx"
#include "value_base.hxx"
#include "default_reader.hxx"

namespace cl
{
	namespace internal
	{
		template< typename T, typename TReader = default_reader<T> >
		class readable_base /* Base for arguments that use a reader class to parse input */
			: public value_base<T>
		{
			using TBase = value_base<T>;
			
			public:
				readable_base()
					: TBase{}
				{
					
				}
			
			public:
				virtual void read(::std::list<::std::string>& p_vals, bool p_IsAssignment) override
				{
					if(p_vals.empty())
					{
						ut::throwf<std::runtime_error>(
							"No value supplied for given argument \"--%s\"!", this->m_LongName
						);
					}
					else
					{
						// Parse value
						this->m_Value = m_Reader.read(p_vals.front());
			
						// Consume value from list
						p_vals.pop_front();
						this->m_Supplied = true;
					}
				}
			
			public:
				using TBase::dispatch;
				
			protected:
				TReader m_Reader;
		};
	}
}