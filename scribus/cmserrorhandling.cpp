/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cmserrorhandling.h"
#include <iostream>

jmp_buf cmsJumpBuffer;

int cmsErrorHandler(int /*ErrorCode*/, const char *ErrorText)
{
	std::cerr << "Littlecms : " << ErrorText << std::endl;
	longjmp(cmsJumpBuffer, 1);
	return 1;
}

