#ifndef WINCONFIG_H
#define WINCONFIG_H

/* Are we building win32 native version. */
#define BUILD_WINDOWS_VERSION 1

/* Are we using GDI for ScPainter::end() */
#define SC_USE_GDI 1

/* Are we using GDI+ for ScGDIPainter */
#ifndef SC_SHOULD_NOT_USE_GDIPLUS
#define SC_USE_GDIPLUS 1
#endif

/* Are we using native api for dll management */
#define DLL_USE_NATIVE_API 1

/* Are we building an executable to be included in MacOS/X .app bundle? */
#undef BUILD_MAC_BUNDLE

/* ColorManagementInclude */
#define CMS_INC "lcms.h"

/* ColorManagement (LittleCms) */
#define HAVE_CMS 1

/* ColorManagement (Windows) */
#define HAVE_ICM 1

/* Can we use unicode functions (Windows) */
#define HAVE_UNICODE 1

/* Define if you have the CoreAudio API */
#undef HAVE_COREAUDIO

/* Define to 1 if you have the <crt_externs.h> header file. */
#undef HAVE_CRT_EXTERNS_H

/* Defines if your system has the crypt function */
#undef HAVE_CRYPT

/* Cups */
#undef HAVE_CUPS

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Defines if your system has the libfontconfig library */
#undef HAVE_FONTCONFIG

/* Defines if your system has the libfreetype library */
#define HAVE_FREETYPE 1

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Defines if your system has the cairo library */
#define HAVE_CAIRO 1

/* Define if you have libjpeg */
#define HAVE_LIBJPEG 1

/* Define if you have libpng */
#define HAVE_LIBPNG 1

/* Define if you have a working libpthread (will enable threaded code) */
#undef HAVE_LIBPTHREAD

/* Define if you have libz */
#define HAVE_LIBZ 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if your system needs _NSGetEnviron to set up the environment */
#undef HAVE_NSGETENVIRON

/* Define if you have the development files for python */
#define HAVE_PYTHON 1

/* Define if you have the development files for podofo */
#define HAVE_PODOFO 1

/* Python */
#undef HAVE_PYTHON_23

/* Define if you have the res_init function */
#undef HAVE_RES_INIT

/* Define to 1 if you have the `snprintf' function. */
#undef HAVE_SNPRINTF
#ifdef _MSC_VER
#define HAVE_SNPRINTF 1
#define snprintf(a, b, c, d) _snprintf(a, b, c, d)
#endif

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
// #define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if you have strlcat */
#undef HAVE_STRLCAT

/* Define if you have the strlcat prototype */
#undef HAVE_STRLCAT_PROTO

/* Define if you have strlcpy */
#undef HAVE_STRLCPY

/* Define if you have the strlcpy prototype */
#undef HAVE_STRLCPY_PROTO

/* Define to 1 if you have the <sys/bitypes.h> header file. */
#undef HAVE_SYS_BITYPES_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Tifflib */
#define HAVE_TIFF 1

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Defines if your system has the libxml2 library */
#define HAVE_XML 1

/* Defines if your system has the libxml2 library6 */
#define HAVE_XML26 1

/* Suffix for lib directories */
#undef KDELIBSUFF

/* Name of package */
#undef PACKAGE

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* The size of a `char *', as computed by sizeof. */
#define SIZEOF_CHAR_P 4

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of a `short', as computed by sizeof. */
#define SIZEOF_SHORT 2 

/* The size of a `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 4

/* The size of a `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG 4

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.3.5svn"

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#undef WORDS_BIGENDIAN

#define HAVE_CONFIG_H 1
#undef  LIBART_COMPILATION
#define WIN32_LEAN_AND_MEAN

typedef unsigned char boolean;
#define HAVE_BOOLEAN

#endif
