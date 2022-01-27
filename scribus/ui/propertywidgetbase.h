/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGETBASE_H
#define PROPERTYWIDGETBASE_H

#include "scribusapi.h"
#include "scguardedptr.h"

class PageItem;
class ScribusDoc;

class PropertyWidgetBase
{
public:
	void setDoc(ScribusDoc *doc);

protected:
	ScGuardedPtr<ScribusDoc> m_doc;
	
	PageItem* currentItemFromSelection();
};

#endif
