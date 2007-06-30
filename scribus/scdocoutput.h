/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCDOCOUTPUT_H
#define SCDOCOUTPUT_H

#include <vector>

#include "scribusapi.h"
#include "scribusdoc.h"
#include "page.h"
#include "scpainterexbase.h"
#include "scpageoutput.h"

class SCRIBUS_API ScDocOutput
{
protected:
	ScDocOutput() {}

	virtual void begin(void) {}
	virtual void end(void) {}

	virtual ScPageOutput* createPageOutputComponent(int pageIndex) { return NULL; }

public:
	virtual ~ScDocOutput() {}

	virtual bool makeOutput(ScribusDoc* doc, std::vector<int>& pageNumbers);
};

#endif
