#pragma once

#include <string>

#include "types.hxx"
#include "source_view.hxx"

namespace cl
{
	namespace diagnostics
	{
		class fixit_hint
			: public source_view
		{
			public:
				fixit_hint(copy_source_t,
						   ut::string_view p_src,
						   const source_range& p_range,
					       const source_range& p_underline,
					       ut::string_view p_hint,
						   offset_type p_caretOffset = leftmost);
		
				fixit_hint(ref_source_t,
						   ut::string_view p_src,
						   const source_range& p_range,
						   const source_range& p_underline,
						   ut::string_view p_hint,
						   offset_type p_caretOffset = leftmost);
			
			public:
				auto hint() const
					-> ut::string_view;
					
			public:
				virtual auto apply_renderer(renderer&, output&) const
					-> void override;
		
			protected:
				::std::string m_Hint{ };
		};
	}
}
