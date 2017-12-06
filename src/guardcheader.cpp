#include "config.hpp"
#include "config/buffer_size.hpp"

#include "guardcheader.hpp"

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <cstdio>

#include <array>
#include <string_view>

#include "log.hpp"


namespace
{
	void write
	(
	 const char * const data,
	 const std::size_t size,
	 std::FILE * const output
	)
	{
		if ( std::fwrite(data, 1, size, output) != size )
		{
			err("couldn't write to the output file");
			
			throw 0;
		}
	}
	
	void write
	(
	 const char c,
	 std::FILE * const output
	)
	{
		if ( std::fputc(c, output) == EOF )
		{
			err("couldn't write to the output file");
			
			throw 0;
		}
	}
	
	void write
	(
	 const char * const data,
	 std::FILE * const output
	)
	{
		if ( std::fputs(data, output) == EOF )
		{
			err("couldn't write to the output file");
			
			throw 0;
		}
	}
	
	void write
	(
	 const std::string_view & s,
	 std::FILE * const output
	)
	{
		write(s.data(), s.size(), output);
	}
	
	void write_guard
	(
	 const char * const guard_base,
	 const char * p,
	 std::FILE * const output
	)
	{
		write(guard_base, output);
		
		write('_', output);
		
		char c = *p;
		
		do
		{
			switch ( c )
			{
				case '.':
				case '-':
				case '/':
				case '\\':
					write('_', output);
					break;
				default:
					write(c, output);
			}
			
			++p;
			c = *p;
		}
		while ( c != 0 );
	}
	
	void write_filename_escaped
	(
	 const char * p,
	 std::FILE * const output
	)
	{
		using namespace std::literals::string_view_literals;
		
		char c = *p;
		
		do
		{
			if ( c == '\\' )
				write("\\\\"sv, output);
			else
				write(c, output);
			
			++p;
			c = *p;
		}
		while ( c != 0 );
	}
	
	using hash_type = std::uintmax_t;
	
	void write_hash
	(
	 const hash_type hash,
	 std::FILE * const output
	)
	{
		std::fprintf(output, "%" PRIuMAX "UL", hash);
	}
	
	hash_type copy_contents
	(
	 std::FILE * const input,
	 std::FILE * const output
	)
	{
		std::array<char, buffer_size> buffer;
		hash_type hash = 0;
		
again:
		auto read = std::fread(buffer.data(), 1, buffer.size(), input);
		
		if ( std::ferror(input) == 0 )
		{
			if ( read > 0 )
			{
				write(buffer.data(), read, output);
				
				for ( const auto c : buffer )
					hash += c;
				
				goto again;
			}
			else if ( std::feof(input) == 0 )
			{
				// didn't read anything but end of stream hasn't been reached - error
			}
		}
		else
		{
			throw 0;
		}
		
		return hash;
	}
	
	void guardcheader_internal
	(
	 const char * const guard_base,
	 std::FILE * const input,
	 std::FILE * const output,
	 const char * const input_name,
	 const char * const output_name
	)
	{
		using namespace std::literals::string_view_literals;
		
		write("#ifndef "sv, output);
		write_guard(guard_base, output_name, output);
		write("\n\n\n#line 1 \""sv, output);
		write_filename_escaped(input_name, output);
		write("\"\n"sv, output);
		const auto hash = copy_contents(input, output);
		write("\n\n#define "sv, output);
		write_guard(guard_base, output_name, output);
		write(' ', output);
		write_hash(hash, output);
		write("\n#elif "sv, output);
		write_guard(guard_base, output_name, output);
		write(" != "sv, output);
		write_hash(hash, output);
		write("\n#error Inclusion guard collision: a different file is using the same inclusion guard ("sv, output);
		write_guard(guard_base, output_name, output);
		write(")\n#endif\n"sv, output);
	}
}

int guardcheader
(
 const char * const guard_base,
 const char * const input_name,
 const char * const output_name
)
noexcept
{
#if FOPEN_MAX < 2
	#error Less than two files may be open simultaneously
#endif
	
	std::FILE * const input = std::fopen(input_name, "rb");
	
	if ( input )
	{
		int rv;
		std::FILE * const output = std::fopen(output_name, "wb");
		
		if ( output )
		{
			try
			{
				guardcheader_internal(guard_base, input, output, input_name, output_name);
				
				rv = EXIT_SUCCESS;
			}
			catch ( int )
			{
				rv = EXIT_FAILURE;
			}
			
			if ( std::fclose(output) == EOF )
				warn("couldn't close output file");
		}
		else
		{
			err("couldn't open output file");
			
			rv = EXIT_FAILURE;
		}
		
		if ( std::fclose(input) == EOF )
			warn("couldn't close input file");
		
		return rv;
	}
	else
	{
		err("couldn't open input file");
		
		return EXIT_FAILURE;
	}
}
