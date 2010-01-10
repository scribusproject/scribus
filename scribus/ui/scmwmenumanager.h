/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Jan 2010
	copyright            : (C) 2010 by Craig Bradney
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

#ifndef SCMWMENUMANAGER_H
#define SCMWMENUMANAGER_H

#include <QMenu>
#include <QObject>

#include "menumanager.h"
#include "scribusapi.h"

class ScMWMenuManager : public MenuManager
{
	Q_OBJECT
	public:
		 ScMWMenuManager(QMenuBar* mb, QObject *parent=0);

	public slots:
		void languageChange();
};

#endif // SCMWMENUMANAGER_H
