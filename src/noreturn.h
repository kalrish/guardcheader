#ifdef __STDC_VERSION__
	#if __STDC_VERSION__ >= 201112L
		#include <stdnoreturn.h>
	#else
		#define WORKAROUND
	#endif
#else
	#define WORKAROUND
#endif

#ifdef WORKAROUND
	#ifdef __GNUC__
		#define noreturn __attribute__((noreturn))
	#endif
	
	#ifdef _MSC_VER
		#define noreturn __declspec(noreturn)
	#endif
	
	#ifndef noreturn
		#define noreturn
	#endif
	
	#undef WORKAROUND
#endif
