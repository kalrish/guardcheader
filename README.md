# guardcheader
Program to guard C and C++ headers

This program adds inclusion guards to C and C++ headers so that their contents do not get processed more than once even if they are included multiple times.


##  Background

Even in small-sized C and C++ projects, the same header files might be included multiple times in the same translation unit, which causes problems except in simple cases. To avoid problems, the headers' contents should be processed only once, and the standard way to achieve that are the so-called "inclusion guards". Inclusion guards employ a macro that is usually derived from the file name. Thus, it is best to automatize its addition.


##  Build

The Tup build system manages the process, which comprises two steps: configuration and build.

###  Configuration
Parameters are specified in the usual way, in a `tup.config` file. You might be interested in using variants.

The steps are as follows:

 1.  Choose whether to use the C or the C++ version of the program (`CONFIG_LANGUAGE`).
 2.  Choose an appropriate toolchain (`CONFIG_TOOLCHAIN`) which supports the chosen language. Toolchains are defined in [src/toolchains.tup](src/toolchains.tup).
 3.  Fulfill the config items supported or required by the chosen toolchain (e.g. `CONFIG_CFLAGS` for the GNU toolchain when building the C version).
 4.  Choose appropriate values for toolchain-independent variables (like `CONFIG_BUFFER_SIZE`).

Example configurations which may or may not be suitable for your build environment are provided in the `configs.tup` directory. You could try to proceed to the build step by just copying any of them to the root as `tup.config`.

###  Build
The build step follows the standard procedure:

    $  tup

####  Requirements

 -  the [Tup build system](http://gittup.org/tup/)
 -  a C or C++ toolchain


##  Example

Assuming the hand-written header, which has no inclusion guards, is called `foo.h.in` and belongs to a library called "bar", just run:

    $  guardcheader BAR foo.h.in foo.h
