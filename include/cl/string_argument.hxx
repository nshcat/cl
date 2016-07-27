#pragma once

#include <string>

#include "value_base.hxx"
#include "utility.hxx"

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
				// Dispatch all tags
				std::initializer_list<int> tmp = { (dispatch(p_tags), 0)... };

				// Silence "not used" warning
				(void)tmp;

				// Check if long name was set.
				if (this->long_name().empty())
					throw std::runtime_error("Long name was not set!");
			}

		protected:
			using Tbase::dispatch;

		public:
			virtual void read(std::list<std::string>& p_vals, bool) override
			{
				// Check if value list is empty
				if (p_vals.empty())
				{
					THROW_FMT(std::runtime_error, "No value supplied for given argument \"--%s\"!", this->m_LongName.c_str());
				}
				
				// Set value and consume it from list
				this->m_Value = p_vals.front();
				p_vals.pop_front();

				this->m_Supplied = true;
			}
	};
}