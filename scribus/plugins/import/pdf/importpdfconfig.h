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

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 82, 0)
#define POPPLER_CONST_082 const
#else
#define POPPLER_CONST_082
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 83, 0)
#define POPPLER_CONST_083 const
#else
#define POPPLER_CONST_083
#endif

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(25, 2, 0)
#define POPPLER_CONST_25_02 const
#else
#define POPPLER_CONST_25_02
#endif

#endif
