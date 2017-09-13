#include <utility.hxx>

namespace cl
{
	namespace internal
	{
		auto insert_line_breaks(const ::std::string& p_in, ::std::string::size_type p_lineLength, ::std::string::size_type p_offset)
			-> ::std::string
		{
			::std::string t_result{ };
			t_result.reserve(p_in.size() + (p_in.size() / p_lineLength) * (p_offset+1));
			
			for(::std::string::size_type t_ix = 0; t_ix < p_in.size(); ++t_ix)
			{
				if(!(t_ix % p_lineLength) && t_ix && (t_ix != p_in.size()-1))
				{
					t_result.push_back('\n');
					t_result.append(p_offset, ' ');
				}
				t_result.push_back(p_in[t_ix]);
			}
			
			return t_result;
		}
	}
}
