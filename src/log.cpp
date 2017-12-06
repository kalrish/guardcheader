#include "config.hpp"

#include "log.hpp"

#include <cstdio>


void warn
(
 const char * const s
)
noexcept
{
	std::fputs("warning: ", stderr);
	std::fputs(s, stderr);
	std::fputc('\n', stderr);
}

void err
(
 const char * const s
)
noexcept
{
	std::fputs("error: ", stderr);
	std::fputs(s, stderr);
	std::fputc('\n', stderr);
}
