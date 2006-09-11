/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scpattern.cpp  -  description
                             -------------------
    begin                : Sat Sep 9 2006
    copyright            : (C) 2006 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qstring.h>

#include "scpattern.h"
#include "scimage.h"
#include "cmsettings.h"
#include "pageitem.h"

ScPattern::ScPattern()
{
	file = "";
	items.setAutoDelete(true);
	items.clear();
	pattern = QImage();
};

QImage* ScPattern::getPattern()
{
	return &pattern;
}

void ScPattern::setPattern(QString name)
{
	typ = 0;
	ScImage im;
	CMSettings cms(0, "", 0);
	bool mode;
	if (im.LoadPicture(name, cms, false, false, ScImage::RGBData, 72, &mode))
	{
		pattern = im.qImage().copy();
		file = name;
	}
	else
		pattern = QImage();
}

void ScPattern::setPattern(PageItem *item)
{
	typ = 1;
}
