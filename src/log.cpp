#include "config.hpp"

#include "log.hpp"

#include <cstdio>


void warn
(
 const std::string_view & msg
)
noexcept
{
	std::fputs("warning: ", stderr);
	std::fwrite(msg.data(), 1, msg.size(), stderr);
	std::fputc('\n', stderr);
}

void err
(
 const std::string_view & msg
)
noexcept
{
	std::fputs("error: ", stderr);
	std::fwrite(msg.data(), 1, msg.size(), stderr);
	std::fputc('\n', stderr);
}
