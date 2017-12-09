# guardcheader

This program adds inclusion guards to C and C++ headers so that their contents do not get processed more than once even if they are included multiple times. The **#line** directive is used so that warnings and errors refer to the original file and the proper lines.


##  Background

Even in small-sized C and C++ projects, the same header files might be included multiple times in the same translation unit. Except in simple cases, this causes problems like multiple definitions. Thus, the headers' contents should be processed only once, and the standard way to achieve that are the so-called "inclusion guards", a preprocessor construct. Inclusion guards employ a macro that is usually derived from the file name and, thus, its addition is best automatized, which is what this program is for.


##  Build

The Tup build system manages the process, which comprises two steps: configuration and build.

###  Configuration
Parameters are specified in the usual way, in a `tup.config` file. You might be interested in using variants.

The steps are as follows:

 1.  Choose whether to use the C or the C++ version of the program (`CONFIG_LANGUAGE`).
 2.  Choose an appropriate toolchain (`CONFIG_TOOLCHAIN`) which supports the chosen language. Toolchains are defined in [src/toolchains.tup](src/toolchains.tup).
 3.  Fulfill the config items supported or required by the chosen toolchain (e.g. `CONFIG_CFLAGS` for the GNU toolchain when building the C version).
 4.  Choose appropriate values for toolchain-independent variables (like `CONFIG_BUFFER_SIZE`).

Example configurations which may or may not be suitable for your build environment are provided in [configs.tup](configs.tup). You could try to proceed to the build step by just copying any of them to the root as `tup.config`.

###  Build
The build step follows the standard procedure:

    $  tup

####  Requirements

 -  the [Tup build system](http://gittup.org/tup/)
 -  a C or C++ toolchain


##  Usage

This is a command-line program. The accepted command-line depends on the version.

All versions admit the GNU standard to find out their version:

    $  guardcheader --version

This prints `guardcheader `_`LANGUAGE`_` `_`VERSION`_, where _LANGUAGE_ is the programming language in which the used version was written (C or C++) and _VERSION_, the version string.

###  Current version

The program shall be invoked as follows:

    $  guardcheader BASE INPUT OUTPUT

_INPUT_ and _OUTPUT_ are the names of the files that shall be read and written, respectively. They should be different files.

_BASE_ is the start of the inclusion guard (e.g.: `MYLIB`). It is used to identify libraries and should not be empty.

Assuming the hand-written header, which has no inclusion guards, is called `foo.h.in` and belongs to a library called "mylib", just run:

    $  guardcheader MYLIB foo.h.in foo.h
