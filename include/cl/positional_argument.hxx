#pragma once

#include <type_traits>
#include <stdexcept>
#include <iterator>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "tags.hxx"
#include "value_base.hxx"
#include "default_reader.hxx"

namespace cl
{
	template<	typename T,
				typename TReader = default_reader<T>,
				typename TContainer = ::std::vector<T>
	>
	class positional_argument
		: public internal::value_base<TContainer>
	{
		using Tbase = internal::value_base<TContainer>;
		using Tthis = positional_argument<T, TReader, TContainer>;
		
		// TODO static check if TContainer has push_back
		// TODO maybe use something other than push_back? Maybe insert(end)?
		
		public:
			template< typename... Ttags >
			positional_argument(const Ttags&... p_tags)
				: Tbase{}
			{
				this->template dispatch_all<Tthis>(p_tags...);
			}

		public:
			void read(std::list<std::string>& p_args, bool) override
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

		public:
			using Tbase::dispatch;
			
		protected:
			TReader m_Reader;
	};
}
