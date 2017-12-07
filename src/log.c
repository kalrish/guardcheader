#include "config.h"

#include "log.h"

#include <stdio.h>
#include "strlit.h"


void warn_impl
(
 const char * const restrict s,
 const size_t n
)
{
	static strlit(s_warning, "warning: ");
	fwrite(s_warning, 1, sizeof(s_warning), stderr);
	fwrite(s, 1, n, stderr);
	fputc('\n', stderr);
}

void err_impl
(
 const char * const restrict s,
 const size_t n
)
{
	static strlit(s_error, "error: ");
	fwrite(s_error, 1, sizeof(s_error), stderr);
	fwrite(s, 1, n, stderr);
	fputc('\n', stderr);
}
