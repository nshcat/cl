#pragma once

#include <memory>
#include <type_traits>
#include <vector>
#include <ut/type_traits.hxx>
#include "message.hxx"

namespace cl
{
	namespace diagnostics
	{
		class combined_message
			: public message
		{
			using message_ptr = ::std::unique_ptr<message>;
			using container_type = ::std::vector<message_ptr>;
			using iterator = container_type::iterator;
			using const_iterator = container_type::const_iterator;
			using value_type = container_type::value_type;
			using reference = container_type::reference;
			
			public:
				combined_message() = default;
				
				
				template<	typename... Ts,
							typename = ::std::enable_if_t<
								sizeof...(Ts) >= 1 &&
								ut::conjunction<::std::is_base_of<message, ::std::decay_t<Ts>>...>::value
							>
				>
				combined_message(Ts&&... p_args)
				{
					m_Cont.reserve(sizeof...(Ts));
					auto x = { (add(::std::forward<Ts>(p_args)), 0)... };
					(void)x;
				}
				
				
			public:
				template<	typename T,
							typename = ::std::enable_if_t<::std::is_base_of<message, ::std::decay_t<T>>::value>
				>
				auto add(T&& p_val)
					-> void
				{
					add(::std::make_unique<::std::decay_t<T>>(::std::forward<T>(p_val)));
				}

				virtual auto apply_renderer(renderer&, output&) const
					-> void override;
					
			public:
				auto begin()
					-> iterator;
					
				auto begin() const
					-> const_iterator;
					
				auto cbegin() const
					-> const_iterator;
					
				auto end()
					-> iterator;
					
				auto end() const
					-> const_iterator;
					
				auto cend() const
					-> const_iterator;
				
			private:
				auto add(message_ptr&&)
					-> void;
				
			protected:
				container_type m_Cont{ };	
		};
	}
}
