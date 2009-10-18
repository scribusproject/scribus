/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolormngtenginefactory.h"
#include "sclcmscolormngtengineimpl.h"

ScColorMngtEngineFactory colorMngtEngineFactory;

ScColorMngtEngine ScColorMngtEngineFactory::createEngine(int engineID)
{
	// for now just return default engine
	return createDefaultEngine();
}

ScColorMngtEngine ScColorMngtEngineFactory::createDefaultEngine()
{
	ScColorMngtEngine lcmsEngine(new ScLcmsColorMngtEngineImpl());
	return lcmsEngine;
}