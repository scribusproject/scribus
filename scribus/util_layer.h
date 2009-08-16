/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_LAYER_H
#define _UTIL_LAYER_H

#include "scribusapi.h"

class ScribusDoc;

/*! \brief Recreate layers so that pre-1.5.0svn doc keep their visual aspect when loaded in 1.5.0svn+
 * \param doc the scribus document
 */
void SCRIBUS_API handleOldLayerBehavior(ScribusDoc* doc);

#endif
