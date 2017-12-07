#include <stddef.h>
#include "restrict.h"
#include "strlit.h"


#define log_shortcut(str, func) \
	{ \
		strlit(s, str); \
		func(s, sizeof(s)); \
	}

#define warn(str) log_shortcut(str, warn_impl)
void warn_impl
(
 const char * const restrict,
 const size_t
);

#define err(str) log_shortcut(str, err_impl)
void err_impl
(
 const char * const restrict,
 const size_t
);
