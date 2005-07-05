#ifndef _SCCONFIG_H
#define _SCCONFIG_H

/*
 * IMPORTANT
 *
 * This file is included by files compiled with a pure C compiler.
 * Protect C++ code with appropriate preprocessor directives, or you
 * might break the build. That means only C-style comments, too.
 *
 */

/* Include win32 specific config instead of portable one if appropriate */
#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#endif
