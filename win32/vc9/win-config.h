#ifndef WINCONFIG_H
#define WINCONFIG_H

/* Version number of package */
#define VERSION "1.4.5"

/* Are we building win32 native version. */
#define BUILD_WINDOWS_VERSION 1

/* Are we using native api for dll management */
#define DLL_USE_NATIVE_API 1

/* Are we building an executable to be included in MacOS/X .app bundle? */
#undef BUILD_MAC_BUNDLE

/* ColorManagementInclude */
#define CMS_INC "lcms2.h"

/* ColorManagement (LittleCms) */
#define HAVE_CMS 1

/* ColorManagement (LittleCms 2.1) */
#define HAVE_LCMS21 1

/* ColorManagement (Windows) */
#define HAVE_ICM 1

/* Can we use unicode functions (Windows) */
#define HAVE_UNICODE 1

/* Cups */
#undef HAVE_CUPS

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Defines if your system has the libfontconfig library */
#undef HAVE_FONTCONFIG

/* Defines if your system has the libfreetype library */
#define HAVE_FREETYPE 1

/* Defines if your system has the cairo library */
/*#define HAVE_CAIRO 1*/

/* Define if you have libjpeg */
#define HAVE_LIBJPEG 1

/* Define if you have libpng */
#define HAVE_LIBPNG 1

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

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#undef WORDS_BIGENDIAN

#define HAVE_CONFIG_H 1
#define WIN32_LEAN_AND_MEAN

typedef unsigned char boolean;
#define HAVE_BOOLEAN

#endif
