#pragma once

#include <type_traits>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <limits>
#include <ut/always_false.hxx>
#include <ut/small_vector.hxx>
#include <ut/array_view.hxx>
#include <ut/throwf.hxx>
#include "tags.hxx"
#include "multi_value_base.hxx"

namespace cl
{
	namespace internal
	{	
		enum class multi_mode
		{
			multi,
			positional
		};
		
		
		template< 	typename T,
					typename TReader,
					multi_mode Mode
		>
		class multi_base /* Base class for arguments that handles multiple values. This implements the reading logic. */
			: public multi_value_base<T>
		{		
			public:
				multi_base()
				{
				}
				
			public:
				virtual void read_end() override
				{
					// Refresh references only if argument was given
					if(this->supplied())
						this->refresh_references();
						
					this->argument_base::read_end();
				}
				
				virtual void read(::std::list<::std::string>& p_args, bool) override
				{
					using TIsPositional =
						::std::integral_constant<bool, Mode == multi_mode::positional>;
						
					read_impl(TIsPositional{}, p_args);
				}
				
			public:
				using multi_value_base<T>::dispatch;
				
			protected:			
				void read_impl(::std::false_type, ::std::list<::std::string>& p_vals)
				{
					// Only try to read one value
					if(p_vals.empty())
					{
						ut::throwf<std::runtime_error>(
							"No value supplied for given argument \"--%s\"!", this->m_LongName
						);
					}
					else
					{
						// Parse value
						this->m_Values.push_back(m_Reader.read(p_vals.front()));
						this->do_callbacks();
			
						// Consume value from list
						p_vals.pop_front();
						this->m_Supplied = true;
					}
				}
				
				void read_impl(::std::true_type, ::std::list<::std::string>& p_vals)
				{
					// Match as many values as possible
					if(p_vals.empty())
					{
						ut::throwf<std::runtime_error>(
							"No value supplied for given argument \"--%s\"!", this->m_LongName
						);
					}
					else
					{
						// Parse values
						for(const auto& t_str: p_vals)
						{
							this->m_Values.push_back(m_Reader.read(t_str));
							this->do_callbacks();
						}
			
						// Consume list
						p_vals.clear();
						this->m_Supplied = true;
					}
				}
				
			protected:
				TReader m_Reader;
		};
	}
}
