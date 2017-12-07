#include "config.hpp"

#include "log.hpp"

#include <cstdio>


namespace
{
	void log_basic
	(
	 const std::string_view & lvl,
	 const std::string_view & msg
	)
	noexcept
	{
		std::fwrite(lvl.data(), 1, lvl.size(), stderr);
		std::fwrite(msg.data(), 1, msg.size(), stderr);
		std::fputc('\n', stderr);
	}
}

using namespace std::literals::string_view_literals;

void warn
(
 const std::string_view & msg
)
noexcept
{
	log_basic("warning: "sv, msg);
}

void err
(
 const std::string_view & msg
)
noexcept
{
	log_basic("error: "sv, msg);
}
