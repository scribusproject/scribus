/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTPREVIEWCREATORFACTORY_H
#define PRINTPREVIEWCREATORFACTORY_H

#include "printpreviewcreator.h"
#include "scribusstructs.h"

class ScribusDoc;

class PrintPreviewCreatorFactory
{
public:
	static PrintPreviewCreator* create(ScribusDoc* doc, PrintLanguage prnLanguage);
};

#endif