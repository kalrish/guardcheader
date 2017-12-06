#include "config.hpp"

#include <cstdlib>
#include <cstdio>

#include <string_view>

#include "log.hpp"
#include "guardcheader.hpp"


namespace
{
	void print_version
	( )
	noexcept
	{
		std::puts("guardcheader C++ 0.1");
	}
}

int main
(
 int argc,
 char * * argv
)
{
	using namespace std::literals::string_view_literals;
	
	switch ( argc )
	{
		case 2:
			if ( "--version"sv.compare(argv[1]) == 0 )
			{
				print_version();
				return EXIT_SUCCESS;
			}
		[[fallthrough]];
		case 1:
		case 3:
			err("too few arguments");
			break;
		case 4:
			return guardcheader(argv[1], argv[2], argv[3]);
		default:
			err("too many arguments");
	}
	return EXIT_FAILURE;
}
