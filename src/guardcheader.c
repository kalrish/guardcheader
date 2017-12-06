#include "config.h"

#include "guardcheader.h"

#include "config/buffer_size.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <setjmp.h>
#include <stdio.h>
#include "restrict.h"
#include "noreturn.h"
#include "log.h"


static jmp_buf jump_buffer;

noreturn
static void throw
(
 int status
)
{
	longjmp(jump_buffer, status);
}

static void write
(
 const void * const restrict data,
 const size_t size,
 FILE * const restrict output
)
{
	if ( fwrite(data, 1, size, output) != size )
	{
		err("couldn't write to the output file");
		
		throw(2);
	}
}

static void write_c
(
 const char c,
 FILE * const restrict output
)
{
	if ( fputc(c, output) == EOF )
	{
		err("couldn't write to the output file");
		
		throw(2);
	}
}

static void write_sz
(
 const char * const restrict data,
 FILE * const restrict output
)
{
	if ( fputs(data, output) == EOF )
	{
		err("couldn't write to the output file");
		
		throw(2);
	}
}

#define write_s(str, output) \
	{ \
		const char s[] = str; \
		write(s, sizeof(s)-1, output); \
	}

static void write_guard
(
 const char * const restrict guard_base,
 const char * restrict p,
 FILE * const restrict output
)
{
	write_sz(guard_base, output);
	
	write_c('_', output);
	
	char c = *p;
	
	do
	{
		switch ( c )
		{
			case '.':
			case '-':
			case '/':
			case '\\':
				write_c('_', output);
				break;
			default:
				write_c(c, output);
		}
		
		++p;
		c = *p;
	}
	while ( c != 0 );
}

static void write_filename_escaped
(
 const char * restrict p,
 FILE * const restrict output
)
{
	const char backslash_escape[] = { '\\', '\\' };
	
	char c = *p;
	
	do
	{
		if ( c == '\\' )
			write(backslash_escape, sizeof(backslash_escape), output);
		else
			write_c(c, output);
		
		++p;
		c = *p;
	}
	while ( c != 0 );
}

typedef uintmax_t hash_type;

static void write_hash
(
 const hash_type hash,
 FILE * const restrict output
)
{
	fprintf(output, "%" PRIuMAX "UL", hash);
}

static hash_type copy_contents
(
 FILE * const restrict input,
 FILE * const restrict output
)
{
	unsigned char buffer[BUFFER_SIZE];
	size_t read;
	hash_type hash = 0;
	
again:
	read = fread(buffer, 1, sizeof(buffer), input);
	
	if ( ferror(input) == 0 )
	{
		if ( read > 0 )
		{
			write(buffer, read, output);
			
			const unsigned char * restrict cur = buffer;
			const unsigned char * const restrict end = buffer+read;
			
			for ( ; cur != end ; ++cur )
				hash += *cur;
			
			goto again;
		}
		else if ( feof(input) == 0 )
		{
			/* Nothing was read but end of stream hasn't been
			 * reached; try again. Warning: this could initiate
			 * an infinite loop.
			*/
			goto again;
		}
	}
	else
	{
		throw(1);
	}
	
	return hash;
}

static void guardcheader_internal
(
 const char * const restrict guard_base,
 FILE * const restrict input,
 FILE * const restrict output,
 const char * const restrict input_name,
 const char * const restrict output_name
)
{
	write_s("#ifndef ", output);
	write_guard(guard_base, output_name, output);
	write_s("\n\n\n#line 1 \"", output);
	write_filename_escaped(input_name, output);
	write_s("\"\n", output);
	const hash_type hash = copy_contents(input, output);
	write_s("\n\n#define ", output);
	write_guard(guard_base, output_name, output);
	write_c(' ', output);
	write_hash(hash, output);
	write_s("\n#elif ", output);
	write_guard(guard_base, output_name, output);
	write_s(" != ", output);
	write_hash(hash, output);
	write_s("\n#error Inclusion guard collision: a different file is using the same inclusion guard (", output);
	write_guard(guard_base, output_name, output);
	write_s(")\n#endif\n", output);
}

int guardcheader
(
 const char * const restrict guard_base,
 const char * const restrict input_name,
 const char * const restrict output_name
)
{
#if FOPEN_MAX < 2
	#error Less than two files may be open simultaneously
#endif
	
	/* These variables don't have to be volatile because
	 * their values aren't changed between the call to
	 * setjmp and the (possible) call to longjmp.
	 */
	
	FILE * const restrict input = fopen(input_name, "rb");
	
	if ( input )
	{
		int rv;
		FILE * const restrict output = fopen(output_name, "wb");
		
		if ( output )
		{
			switch ( setjmp(jump_buffer) )
			{
				case 0:
					guardcheader_internal(guard_base, input, output, input_name, output_name);
					
					rv = EXIT_SUCCESS;
					break;
				default:
					rv = EXIT_FAILURE;
			}
			
			if ( fclose(output) == EOF )
				warn("couldn't close output file");
		}
		else
		{
			err("couldn't open output file");
			
			rv = EXIT_FAILURE;
		}
		
		if ( fclose(input) == EOF )
			warn("couldn't close input file");
		
		return rv;
	}
	else
	{
		err("couldn't open input file");
		
		return EXIT_FAILURE;
	}
}
