#pragma once

#include <string>
#include <vector>
#include <cstdio>

namespace cl
{
	namespace internal
	{
		template< typename... Targs >
		std::string sprintf(const std::string& fmt, Targs... args)
		{
			//static_assert(internal::all_of<std::is_scalar, Targs...>::value, "Only scalar types are allowed as arguments!");

			// Initialize temporary buffer with reasonable starting capacity.
			std::vector<char> buf{};
			buf.resize(1024);

			// Try until buffer is large enough
			while (true)
			{
				// Call C snprintf and receive total number of chars that would be written
				// if buffer was large enough
				int n = snprintf(&buf[0], buf.size(), fmt.c_str(), args...);

				if (n < 0)
				{
					// We got an error here. Double buffer size.
					buf.resize(buf.size() * 2);
				}
				else if (static_cast<size_t>(n) > buf.size())
				{
					// There was not enough room for all characters.
					// Set buffer to correct size and try again.
					buf.resize(n + 1);
				}
				else
				{
					// Success. Return string.
					return std::string(&buf[0], n);
				}
			}
		}


		#define THROW_FMT( _Tex, _fmt, ... ) \
			do { \
				std::string _str = ::cl::internal::sprintf(_fmt, __VA_ARGS__); \
				throw _Tex(_str.c_str()); \
			} while (0)
	}
}