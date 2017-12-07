#include "config.h"

#include "log.h"

#include <stdio.h>
#include "strlit.h"


static void log_basic
(
 const char * const restrict lvl,
 const size_t lvl_s,
 const char * const restrict msg,
 const size_t msg_s
)
{
	fwrite(lvl, 1, lvl_s, stderr);
	fwrite(msg, 1, msg_s, stderr);
	fputc('\n', stderr);
}

void warn_impl
(
 const char * const restrict s,
 const size_t n
)
{
	static strlit(s_warning, "warning: ");
	log_basic(s_warning, sizeof(s_warning), s, n);
}

void err_impl
(
 const char * const restrict s,
 const size_t n
)
{
	static strlit(s_error, "error: ");
	log_basic(s_error, sizeof(s_error), s, n);
}
