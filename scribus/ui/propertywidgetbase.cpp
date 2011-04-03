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
	PageItem *currentItem = NULL;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 1)
		{
			/*uint lowestItem = 999999;
			for (int a=0; a<m_doc->m_Selection->count(); ++a)
			{
				currentItem = m_doc->m_Selection->itemAt(a);
				lowestItem = qMin(lowestItem, currentItem->ItemNr);
			}
			currentItem = m_doc->Items->at(lowestItem);*/
			currentItem = m_doc->m_Selection->itemAt(0);
		}
		else if (m_doc->m_Selection->count() == 1)
		{
			currentItem = m_doc->m_Selection->itemAt(0);
		}
	}

	return currentItem;
}