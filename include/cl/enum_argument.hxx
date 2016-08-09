#pragma once
#include <type_traits>
#include <algorithm>
#include <map>
#include <ut/throwf.hxx>

#include "value_base.hxx"

namespace cl
{
	template< typename E >
	class enum_argument
		: public internal::value_base<E>
	{
		static_assert(std::is_enum<E>::value,
			"Type E is not an enumeration type!");

		using Tbase = internal::value_base<E>;

		public:
			template< typename... Ttags >
			enum_argument(const Ttags&... p_tags)
				: Tbase{}, m_IgnoreCase{ false }
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

			void dispatch(const internal::enum_value_t<E>& p_tag)
			{
				// Allow overwriting by not catching "already exists"
				m_Map[p_tag.key()] = p_tag.value();
			}

			void dispatch(const internal::ignore_case_t&)
			{
				m_IgnoreCase = true;
			}

		public:
			virtual void read(std::list<std::string>& p_vals, bool) override
			{
				// Check if value list is empty
				if (p_vals.empty())
				{
					ut::throwf<std::runtime_error>("No value supplied for given argument \"--%s\"!", this->m_LongName.c_str());
				}

				// Check if supplied enumeration value is known.
				auto it = m_Map.end();

				// If case is to be ignored, we sadly have to use dirty O(n) search.
				if (m_IgnoreCase)
				{
					std::string t_str = to_lower(p_vals.front());

					it = std::find_if(m_Map.begin(), m_Map.end(),
						[&t_str, this](auto& pair) -> bool
						{
							return to_lower(pair.first) == t_str;
						});
				}
				else it = m_Map.find(p_vals.front());


				if (it != m_Map.end())
				{
					this->m_Value = it->second;
					p_vals.pop_front();
					this->m_Supplied = true;
				}
				else ut::throwf<std::runtime_error>("Invalid enumeration value for argument \"--%s\": \"%s\"",
					this->m_LongName.c_str(), p_vals.front().c_str());
			}

		private:
			std::string to_lower(std::string p_str)
			{
				std::transform(p_str.begin(), p_str.end(), p_str.begin(), [](char c) -> char { return std::tolower(c); });
				return p_str;
			}

		private:
			std::map<std::string, E>	m_Map;
			bool						m_IgnoreCase;
	};
}