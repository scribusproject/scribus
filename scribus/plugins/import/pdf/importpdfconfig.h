/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTPDFCONFIG_H
#define IMPORTPDFCONFIG_H

#include <poppler/cpp/poppler-version.h>

#define POPPLER_VERSION_ENCODE(major, minor, micro) (	\
	  ((major) * 10000)				\
	+ ((minor) *   100)				\
	+ ((micro) *     1))
#define POPPLER_ENCODED_VERSION POPPLER_VERSION_ENCODE(POPPLER_VERSION_MAJOR, POPPLER_VERSION_MINOR, POPPLER_VERSION_MICRO)

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 64, 0)
#define POPPLER_CONST const
#else
#define POPPLER_CONST
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 70, 0)
#define POPPLER_CONST_070 const
#else
#define POPPLER_CONST_070
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 71, 0)
#define GBool  bool
#define gTrue  true
#define gFalse false
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 72, 0)
#define getCString  c_str
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 73, 0)
#define Guchar  unsigned char
#define Gushort unsigned short
#define Guint   unsigned int
#define Gulong  unsigned long
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 75, 0)
#define POPPLER_CONST_075 const
#define POPPLER_REF &
#else
#define POPPLER_CONST_075
#define POPPLER_REF
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 82, 0)
#define POPPLER_CONST_082 const
#else
#define POPPLER_CONST_082
#endif

#endif
