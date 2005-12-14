#ifndef SCRIBUS_SCRIPTER2BASE_H
#define SCRIBUS_SCRIPTER2BASE_H

#include "cmdvar.h"

/**
 * @brief Set up scripter2 module
 *
 * This function sets up the Boost::Python-based scripter2 module, if available.
 * If we've been built without Boost::Python it's a no-op.
 */
void scribus2_init();

#endif
