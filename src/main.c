#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#include "guardcheader.h"


static void
print_version
( )
{
	puts("guardcheader C 0.1");
}

int main
(
 int argc,
 char * * argv
)
{
	switch ( argc )
	{
		case 2:
			if ( !strcmp(argv[1], "--version") )
			{
				print_version();
				return EXIT_SUCCESS;
			}
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
