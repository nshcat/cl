#pragma once

#include <string>
#include <ut/optional.hxx>
#include <ut/string_view.hxx>

#include "message.hxx"
#include "types.hxx"
#include "low_level.hxx"

namespace cl
{
	namespace diagnostics
	{
		struct copy_source_t
		{
		};
		
		struct ref_source_t
		{
		};
		
		constexpr static const copy_source_t copy_source = { };
		constexpr static const ref_source_t ref_source = { };
	
		/* A diagnostics message displaying a section of the source code,
		   with a part of it highlighted by a squiggly line.
		   Can either store the source code as hard copy or as a reference. */
		class source_view
			: public message
		{
			protected:
				using offset_type = ::std::size_t;	//< Can't be negative
			
			public:
				source_view(copy_source_t,
							ut::string_view p_src,
							const source_range& p_range,
							const source_range& p_underline,
							offset_type p_caretOffset = leftmost);
		
				source_view(ref_source_t,
							ut::string_view p_src,
							const source_range& p_range,
							const source_range& p_underline,
							offset_type p_caretOffset = leftmost);
			
			public:
				auto source() const
					-> ut::string_view;
					
				auto offset() const
					-> offset_type;
					
				auto range() const
					-> const source_range&;
					
				auto underline_range() const
					-> const source_range&;
					
			public:
				virtual auto apply_renderer(renderer&, output&) const
					-> void override;
		
			protected:
				offset_type m_Offset{ };
				source_range m_SrcRange{ };
				source_range m_ULRange{ };
				ut::optional<::std::string> m_Source{ };	//< TODO: `variant` would be way better here.
				ut::string_view m_SourceRef{ }; 			//< Invariant: m_SourceRef is always a valid view on the source string,
															//  no matter if the particular instance is constructed by reference or copy.
		};
	}
}
