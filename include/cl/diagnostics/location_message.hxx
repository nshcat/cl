#pragma once

#include "simple_message.hxx"
#include "types.hxx"

namespace cl
{
	namespace diagnostics
	{
		// TODO: Better name
		/* A diagnostics message containing a source position that indicates the
	       origin of the message. */
		class location_message
			: public simple_message
		{
			public:
				location_message(diagnostics_level, ::std::string, const source_location&, ::std::string);
				
				
				template<	typename... Ts,
							typename = ::std::enable_if_t<sizeof...(Ts) >= 1>
				>
				location_message(diagnostics_level p_lvl, ::std::string p_sender, const source_location& p_loc, ut::string_view p_fmt, Ts&&... p_args)
					: location_message(p_lvl, p_sender, p_loc, ut::sprintf(p_fmt, ::std::forward<Ts>(p_args)...))
				{
					
				}
			
			public:
				virtual auto apply_renderer(renderer&, output&) const
					-> void override;
			
			public:
				auto location() const
					-> const source_location&;
								
			protected:
				source_location m_Loc{ };
		};
	}
}
