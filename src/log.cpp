#include "config.hpp"

#include "log.hpp"

#include <cstdio>


namespace
{
	void log_basic
	(
	 const char * const lvl,
	 const std::string_view & msg
	)
	noexcept
	{
		std::fputs(lvl, stderr);
		std::fwrite(msg.data(), 1, msg.size(), stderr);
		std::fputc('\n', stderr);
	}
}

void warn
(
 const std::string_view & msg
)
noexcept
{
	log_basic("warning: ", msg);
}

void err
(
 const std::string_view & msg
)
noexcept
{
	log_basic("error: ", msg);
}
