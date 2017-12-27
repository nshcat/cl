#pragma once
#include <type_traits>
#include <algorithm>
#include <string>
#include <map>
#include <ut/throwf.hxx>

#include "value_base.hxx"
#include "diagnostics.hxx"

namespace cl
{
	template< typename E >
	class enum_argument
		: public internal::value_base<E>
	{
		static_assert(::std::is_enum<E>::value,
			"Type E is not an enumeration type!");

		using Tbase = internal::value_base<E>;

		public:
			template< typename... Ttags >
			enum_argument(Ttags&&... p_tags)
				: Tbase{}, m_IgnoreCase{ false }
			{
				this->template dispatch_all<enum_argument>(::std::forward<Ttags>(p_tags)...);
			}

		public:
			using Tbase::dispatch;

			void dispatch(const internal::key_value_t<::std::string, E>& p_tag)
			{
				// Allow overwriting by not catching "already exists"
				m_Map[p_tag.first()] = p_tag.second();
			}
			
			template< typename T, typename U >
			void dispatch(const internal::key_value_t<T, U>&)
			{
				static_assert(ut::always_false_v<T>,
					"enum_argument: Key value pair type mismatch!");
			}
			
			template< typename T >
			void dispatch(const internal::value_t<T>&)
			{
				static_assert(ut::always_false_v<T>,
					"enum_argument: Expected enum key value pair!");
			}

			void dispatch(const internal::ignore_case_t&)
			{
				m_IgnoreCase = true;
			}

		public:
			virtual void read(::std::list<std::string>& p_vals, bool p_isAssignment) override
			{
				// Check if value list is empty
				if (p_vals.empty())
				{
					ut::throwf<std::runtime_error>("No value supplied for given argument \"--%s\"!", this->m_LongName);
				}

				// Check if supplied enumeration value is known.
				auto it = m_Map.end();

				// If case is to be ignored, we sadly have to use dirty O(n) search.
				if (m_IgnoreCase)
				{
					::std::string t_str = to_lower(p_vals.front());

					it = ::std::find_if(m_Map.begin(), m_Map.end(),
						[&t_str, this](auto& pair) -> bool
						{
							return this->to_lower(pair.first) == t_str;
						});
				}
				else it = m_Map.find(p_vals.front());


				if (it != m_Map.end())
				{
					this->m_Value = it->second;
					p_vals.pop_front();
					this->m_Supplied = true;
				}
				/*else ut::throwf<::std::runtime_error>("Invalid enumeration value for argument \"--%s\": \"%s\"",
					this->m_LongName, p_vals.front());*/
				else
				{
					diagnostics::post_diagnostic(
						::std::cout,
						diagnostics::diagnostics_level::error,
						"commandline",
						(p_isAssignment ? this->diag_state().argument_range().location() : this->diag_state().value_ranges()[0].location()),
						"Invalid enumeration value for argument \"--%s\": \"%s\"",
						this->m_LongName,
						p_vals.front()
					);
						
					diagnostics::post_source_view(
						this->diag_state().source(),
						diagnostics::source_range{
							this->diag_state().argument_range().start(),
							this->diag_state().value_ranges()[0].end()
						},				
						this->diag_state().value_ranges()[0],
						diagnostics::rightmost
					);
					
					
					
					::std::exit(EXIT_FAILURE);
				}
			}

		private:
			::std::string to_lower(::std::string p_str)
			{
				::std::transform(p_str.begin(), p_str.end(), p_str.begin(), [](char c) -> char { return ::std::tolower(c); });
				return p_str;
			}

		private:
			::std::map<::std::string, E>	m_Map;
			bool							m_IgnoreCase;
	};
}
