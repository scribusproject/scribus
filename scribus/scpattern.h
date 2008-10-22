/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scpattern.h  -  description
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

#ifndef SCPATTERN_H
#define SCPATTERN_H

#include <QImage>
#include <QList>
#include <QString>

#include "scribusapi.h"

class PageItem;
class ScribusDoc;

class SCRIBUS_API ScPattern
{
public:
	ScPattern();
	~ScPattern() {};
	QImage* getPattern();
	void setDoc(ScribusDoc *doc);
	void setPattern(QString file);
	double scaleX;
	double scaleY;
	double height;
	double width;
	double xoffset;
	double yoffset;
	QList<PageItem*> items;
	ScribusDoc *m_doc;
	QImage pattern;
};

#endif
