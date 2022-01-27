/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pageitem.h"
#include "propertywidgetbase.h"
#include "scribusdoc.h"
#include "selection.h"

void PropertyWidgetBase::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

PageItem* PropertyWidgetBase::currentItemFromSelection()
{
	if (!m_doc)
		return nullptr;

	PageItem *currentItem = nullptr;
	if (m_doc->m_Selection->count() > 0)
		currentItem = m_doc->m_Selection->itemAt(0);
	return currentItem;
}
