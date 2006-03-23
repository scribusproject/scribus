/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMSERRORHANDLING_H
#define CMSERRORHANDLING_H

#include <setjmp.h>

/*!
\brief Jump buffer used by cmsErrorHandler, must be set with the setjmp() function
*      before cmsErrorHandler may be called
*/
extern jmp_buf cmsJumpBuffer;

/*!
\brief Simple error handler for use in conjunction with littlecms
\param ErrorCode error code issued by little cms
\param ErrorText error message corresponding to the error code
*/
int    cmsErrorHandler(int ErrorCode, const char *ErrorText);

#endif

