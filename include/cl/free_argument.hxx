#pragma once

#include <type_traits>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "tags.hxx"
#include "value_base.hxx"
#include "default_reader.hxx"

namespace cl
{
	// Will not throw if conversion is not possible.
	template<	typename T = ::std::string,
				typename TReader = default_reader<T>,
				typename TContainer = ::std::vector<T>
	>
	class free_argument
		: public internal::value_base<TContainer>
	{
		using Tthis = free_argument<T, TReader, TContainer>;
		using Tbase = internal::value_base<TContainer>;

		public:
			template< typename... Ttags >
			free_argument(Ttags&&... p_tags)
				: Tbase{}
			{
				// argument_base::dispatch_all can't be used here since
				// free_argument does not need a long_name to be set.
				
				// Dispatch all tags
				std::initializer_list<int> tmp = { (dispatch(::std::forward<Ttags>(p_tags)), 0)... };

				// Silence "not used" warning
				(void)tmp;
			}

		public:
			using Tbase::dispatch;

			// Block long_name from being set // TODO better error message
			void dispatch(const internal::long_name_t&) = delete;

			// Block short_name from being set
			void dispatch(const internal::short_name_t&) = delete;

		public:
			virtual void read(::std::list<::std::string>& p_args, bool) override
			{
				// Consume all supplied strings and try to convert them to T
				for(const auto& p_str: p_args)
				{
					this->m_Value.push_back(m_Reader.read(p_str));
				}

				// Clear input list since we consumed everything
				p_args.clear();

				this->m_Supplied = true;
			}
			
		protected:
			TReader m_Reader;
	};
}
