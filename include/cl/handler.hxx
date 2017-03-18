#pragma once

#include <type_traits>

#include <ut/type_traits.hxx>
#include <ut/array_view.hxx>
#include <ut/observer_ptr.hxx>

#include "handler_base.hxx"
#include "command.hxx"
#include "meta.hxx"

namespace cl
{
	namespace internal
	{
		enum class placeholder
		{
			first
		};
	}
	
	
	class handler
		: 	public internal::handler_base,
			public command<internal::placeholder>
	{
		using input_type = ut::array_view<const char*>;
		using base_type = internal::handler_base;
		using command_type = command<internal::placeholder>;
		
		public:
			template< 	typename... Ts,
						typename = ::std::enable_if_t<
							not ::std::is_same<
								handler,
								::std::decay_t<ut::first_t<Ts...>>
							>::value
						>
			>
			handler(Ts&&... p_args)
				: command_type(::std::forward<Ts>(p_args)...)
			{
				// It is ok to access option tags here despite being forwarded beforehand,
				// because cl::command will ignore all option tags completely.
				const auto x = { (this->base_type::dispatch(::std::forward<Ts>(p_args)), 0)... };
				(void)x;
			}
			
			handler() = delete;
			handler(const handler&) = delete;
			handler(handler&&) = default;
			
			handler& operator=(const handler&) = delete;
			handler& operator=(handler&&) = default;
			
		public:
			auto read(input_type)
				-> bool;
				
			auto read(int p_argc, const char** p_argv)
				-> bool;
			
		private:
			//command_type m_Cmd;
	};
}
