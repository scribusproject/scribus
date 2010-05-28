/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scconfig.h"
#include "sccolormgmtenginefactory.h"

#ifdef HAVE_LCMS2
#include "sclcms2colormgmtengineimpl.h"
#else
#include "sclcmscolormgmtengineimpl.h"
#endif

ScColorMgmtEngineFactory colorMgmtEngineFactory;

ScColorMgmtEngine ScColorMgmtEngineFactory::createEngine(int engineID)
{
	// for now just return default engine
	return createDefaultEngine();
}

ScColorMgmtEngine ScColorMgmtEngineFactory::createDefaultEngine()
{
#ifdef HAVE_LCMS2
	ScColorMgmtEngine lcms2Engine(new ScLcms2ColorMgmtEngineImpl());
	return lcms2Engine;
#else
	ScColorMgmtEngine lcmsEngine(new ScLcmsColorMgmtEngineImpl());
	return lcmsEngine;
#endif
}