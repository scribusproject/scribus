/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCOLORMNGTENGINEFACTORY_H
#define SCCOLORMNGTENGINEFACTORY_H

#include "sccolormngtengine.h"

class ScColorMngtEngineFactory
{
public:
	ScColorMngtEngine createEngine(int engineID);
	ScColorMngtEngine createDefaultEngine();
};

extern ScColorMngtEngineFactory colorMngtEngineFactory;

#endif
