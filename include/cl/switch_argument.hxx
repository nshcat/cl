#pragma once
#include <limits>
#include <vector>
#include <cinttypes>
#include <type_traits>
#include <ut/throwf.hxx>

#include "value_base.hxx"


namespace cl
{
	template< typename T >
	class switch_argument
		: public internal::value_base<T>
	{
		using Tthis = switch_argument<T>;
		using Tbase = internal::value_base<T>;

		public:
			template< typename... Ttags >
			switch_argument(const Ttags&... p_tags)
				: Tbase{}
			{
				this->template dispatch_all<Tthis>(p_tags...);	
			}
			
		public:
			virtual void read(::std::list<::std::string>& p_vals, bool p_IsAssignment) override
			{
				// If there was an assignment, throw
				if(p_IsAssignment)
				{
					ut::throwf<::std::runtime_error>(
						"Syntax error: Switch argument \"--%s\" does not accept any parameter!", this->m_LongName
					);
				}
				else
				{
					this->m_Value = m_SwValue;
					this->m_Supplied = true;
				}
			}

		public:
			using Tbase::dispatch;
			
			void dispatch(const internal::switch_value_t<T>& p_tag)
			{
				m_SwValue = p_tag.value();
			}
			
		private:
			T m_SwValue{};
	};
}
