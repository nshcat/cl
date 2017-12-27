#pragma once

#include <string>
#include <type_traits>
#include <ut/format.hxx>

#include <diagnostics/message.hxx>
#include <diagnostics/types.hxx>

namespace cl
{
	namespace diagnostics
	{
		/* A simple message only containg a message level and string, and the source label */
		class simple_message
			: public message
		{
			public:
				simple_message(diagnostics_level, ::std::string, ::std::string);
				
				
				template<	typename... Ts,
							typename = ::std::enable_if_t<sizeof...(Ts) >= 1>
				>
				simple_message(diagnostics_level p_lvl, ::std::string p_sender, ut::string_view p_fmt, Ts&&... p_args)
					: simple_message(p_lvl, p_sender, ut::sprintf(p_fmt, ::std::forward<Ts>(p_args)...))
				{
					
				}
				
			public:
				virtual auto apply_renderer(renderer&, output&)
					-> void override;
			
			protected:
				diagnostics_level m_Level{ };	//< Severity level of the message
				::std::string m_Sender{ };		//< Label of the sender entity
				::std::string m_Message{ };		//< Actual diagnostic message		
		};
	}
}
