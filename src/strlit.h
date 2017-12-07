/* Defines a char array from a string literal
 * omitting the terminating 0 to save space. */
#define strlit(id, s) \
	const char id[sizeof(s)-1] = s
