/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPRINTENGINE_PS_H
#define SCPRINTENGINE_PS_H

#include "scprintengine.h"
#include "scribusapi.h"

class SCRIBUS_API ScPrintEngine_PS : public ScPrintEngine
{
public:
	ScPrintEngine_PS(ScribusDoc& doc);
	~ScPrintEngine_PS() {}

	bool print(PrintOptions& options) override;
};

#endif
