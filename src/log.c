#include "config.h"

#include "log.h"

#include <stdio.h>
#include "strlit.h"


void warn
(
 const char * const restrict s
)
{
	static strlit(s_warning, "warning: ");
	fwrite(s_warning, 1, sizeof(s_warning), stderr);
	fputs(s, stderr);
	fputc('\n', stderr);
}

void err
(
 const char * const restrict s
)
{
	static strlit(s_error, "error: ");
	fwrite(s_error, 1, sizeof(s_error), stderr);
	fputs(s, stderr);
	fputc('\n', stderr);
}
