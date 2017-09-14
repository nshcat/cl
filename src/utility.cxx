#include <sstream>

#include <utility.hxx>

namespace cl
{
	namespace internal
	{
		auto insert_line_breaks(const ::std::string& p_in, ::std::string::size_type p_lineLength, ::std::string::size_type p_offset)
			-> ::std::string
		{
			::std::istringstream t_words{p_in};
			::std::ostringstream t_wrapped{ };
			::std::string t_word{ };
			
			if(t_words >> t_word)
			{
				t_wrapped << t_word;
				::std::size_t t_spaceLeft = p_lineLength - t_word.length();
				while(t_words >> t_word)
				{
					if(t_spaceLeft < t_word.length() + 1)
					{
						t_wrapped << '\n' << ::std::string(p_offset, ' ') << t_word;
						t_spaceLeft = p_lineLength - t_word.length();
					}
					else
					{
						t_wrapped << ' ' << t_word;
						t_spaceLeft -= t_word.length() + 1;
					}
				}
			}
			
			return t_wrapped.str();
		}
	}
}
