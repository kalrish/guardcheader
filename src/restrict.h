#ifdef __STDC_VERSION__
	#if __STDC_VERSION__ < 199901L
		#define WORKAROUND
	#endif
#else
	#define WORKAROUND
#endif

#ifdef WORKAROUND
	#ifdef _MSC_VER
		#define restrict __restrict
	#endif
	
	#ifndef restrict
		#define restrict
	#endif
	
	#undef WORKAROUND
#endif
