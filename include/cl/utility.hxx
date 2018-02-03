#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

namespace cl
{
	namespace internal
	{
		// Modifies string to word-wrap based on a maximum line length.
		// Optionally, an offset can be set to append spaces after each line break to
		// align all lines at a specific column.
		auto insert_line_breaks(const ::std::string& p_in, ::std::string::size_type p_lineLength, ::std::string::size_type p_offset = 0U)
			-> ::std::string;
			
			
			
		template< typename Tit1, typename Tit2 >
		auto levenshtein_distance(Tit1 p_begin1, Tit1 p_end1, Tit2 p_begin2, Tit2 p_end2)
			-> ::std::size_t
		{
			const ::std::size_t t_len1 = ::std::distance(p_begin1, p_end1);
			const ::std::size_t t_len2 = ::std::distance(p_begin2, p_end2);
			
			::std::vector<::std::size_t> t_col(t_len2+1U), t_prevCol(t_len2+1U);
			
			for(::std::size_t i = 0; i < t_prevCol.size(); i++)
				t_prevCol[i] = i;
			for(::std::size_t i = 0; i < t_len1; i++) {
				t_col[0] = i+1;
				for (::std::size_t j = 0; j < t_len2; j++)
					t_col[j+1] = std::min({ t_prevCol[1 + j] + 1, t_col[j] + 1, t_prevCol[j] + (p_begin1[i]==p_begin2[j] ? 0 : 1) });
				t_col.swap(t_prevCol);
			}
			return t_prevCol[t_len2];
		}
	}
}
