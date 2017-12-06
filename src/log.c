#include "config.h"

#include "log.h"

#include <stdio.h>


void warn
(
 const char * const restrict s
)
{
	static const char s_warning[9] = "warning: ";
	fwrite(s_warning, 1, sizeof(s_warning), stderr);
	fputs(s, stderr);
	fputc('\n', stderr);
}

void err
(
 const char * const restrict s
)
{
	static const char s_error[7] = "error: ";
	fwrite(s_error, 1, sizeof(s_error), stderr);
	fputs(s, stderr);
	fputc('\n', stderr);
}
