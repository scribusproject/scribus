/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//#include <QDebug>

#include "scribusdoc.h"
#include "util_text.h"

int findParagraphStyle(ScribusDoc* doc, const ParagraphStyle& parStyle)
{
	bool named = !parStyle.name().isEmpty();
	//qDebug() << QString("looking up %1/ %2").arg(parStyle.name()).arg(parStyle.alignment()); 
	if (named) {
		for (int i=0; i < doc->paragraphStyles().count(); ++i)
		{
			//qDebug() << QString("%1 %2").arg(i).arg(doc->paragraphStyles()[i].name());
			if (parStyle.name() == doc->paragraphStyles()[i].name()) {
				return i;
			}
		}
		assert(false);
		return -1;
	}
	else {
		return -1;
	}
}

int findParagraphStyle(ScribusDoc* doc, const QString &name)
{
	for (int i=0; i < doc->paragraphStyles().count(); ++i)
	{
		if (name == doc->paragraphStyles()[i].name()) {
			return i;
		}
	}
	assert(false);
	return -1;
}
