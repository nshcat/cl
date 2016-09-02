#pragma once

#include <string>
#include <ut/throwf.hxx>

#include "value_base.hxx"


namespace cl
{
	class string_argument
		: public internal::value_base<std::string>
	{
		using Tbase = internal::value_base<std::string>;

		public:
			template< typename... Ttags >
			string_argument(const Ttags&... p_tags)
				: Tbase{}
			{
				this->dispatch_all<string_argument>(p_tags...);
			}

		public:
			using Tbase::dispatch;

		public:
			virtual void read(std::list<std::string>& p_vals, bool) override
			{
				// Check if value list is empty
				if (p_vals.empty())
				{
					ut::throwf<std::runtime_error>("No value supplied for given argument \"--%s\"!", this->m_LongName.c_str());
				}
				
				// Set value and consume it from list
				this->m_Value = p_vals.front();
				p_vals.pop_front();

				this->m_Supplied = true;
			}
	};
}