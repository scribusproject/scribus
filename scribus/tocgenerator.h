/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jun 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TOCGENERATOR_H
#define TOCGENERATOR_H

#include <QObject>

#include "scribusapi.h"

class ScribusDoc;
class PageItem;

/**
@author Craig Bradney
*/
class SCRIBUS_API TOCGenerator : public QObject
{
	Q_OBJECT
	public:
		TOCGenerator(QObject *parent = NULL, ScribusDoc *doc = NULL);
		~TOCGenerator();
		
		void setDoc(ScribusDoc *doc = NULL);
		
	public slots:
		void generateDefault();

	private:
		PageItem* findTargetFrame(const QString &targetFrameName);
		ScribusDoc *currDoc;
};

#endif
