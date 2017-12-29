#pragma once

#include <string>
#include <vector>
#include <ios>

#include <ut/string_view.hxx>

namespace cl
{
	namespace diagnostics
	{
		// Modifier codes used in diagnostics messages to indicated desired
		// colorization
		enum class color_modifier
			: char
		{
			escape	= '^',
			reset 	= '0',
				
			red 	= '1',
			green 	= '2',
			yellow 	= '3',
			blue 	= '4',
			magenta = '5',
			cyan 	= '6',
			white 	= '7',
			black 	= '8',
					
			vivid_red 		= 'a',
			vivid_green 	= 'b',
			vivid_yellow 	= 'c',
			vivid_blue 		= 'd',
			vivid_magenta 	= 'e',
			vivid_cyan 		= 'f',
			vivid_white 	= 'g',
			vivid_black 	= 'h',
			
			dull_begin 	= red,
			dull_end 	= black,
			vivid_begin = vivid_red,
			vivid_end 	= vivid_black
		};
		
	
		enum class diagnostics_level
		{
			error,
			warning,
			note,
			remark,
			debug
		};
	
		struct from_length_t
		{
		};
		
		constexpr static const from_length_t from_length{ };
	
		// A single location in the source code
		class source_location
		{
			public:
				using size_type = ::std::size_t;
			
			public:
				static const size_type npos = size_type(-1);
			
			public:
				source_location() = default;
			
				source_location(size_type p_line, size_type p_col);
			
			public:
				auto column() const
					-> size_type;
				
				auto line() const
					-> size_type;
			
			private:
				size_type m_Col{ npos };
				size_type m_Line{ npos };
		};
	
		// A range in one line of source code.
		class source_range
		{
			public:
				using size_type = ::std::size_t;
		
			public:
				static const size_type npos = size_type(-1);
		
			public:
				source_range() = default;
			
				source_range(size_type p_start, size_type p_end, size_type p_line = { });
				
				source_range(from_length_t, size_type p_start, size_type p_length, size_type p_line = { });
				
			public:
				auto reset()
					-> void;
					
			public:
				auto location() const
					-> source_location;
			
				auto start() const
					-> size_type;
				
				auto end() const
					-> size_type;
					
				auto length() const
					-> size_type;
				
				auto line() const
					-> size_type;
				
				auto empty() const
					-> bool;
		
			private:
				size_type m_Start{ npos };
				size_type m_End{ npos };
				size_type m_Line{ npos };
		};
	}
}

namespace std
{
	template< typename T, typename TTraits >
	auto operator<<(::std::basic_ostream<T, TTraits>& p_str, const ::cl::diagnostics::source_range& p_range)
		-> ::std::basic_ostream<T, TTraits>&
	{
		p_str << "[" << p_range.start() << ", " << p_range.end() << "] (Length: " << p_range.length() << ")";
		
		return p_str;
	}
}
