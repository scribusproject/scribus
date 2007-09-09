/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	copyright            : (C) 2006 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

#ifndef COLORSETMANAGER_H
#define COLORSETMANAGER_H

#include "scribusapi.h"
#include <QMap>
#include <QString>
#include <QStringList>

struct ApplicationPrefs;

class SCRIBUS_API ColorSetManager
{
	public:
		ColorSetManager();
		~ColorSetManager();
		
		void initialiseDefaultPrefs(struct ApplicationPrefs& appPrefs);
		void findPaletteLocations();
		void findPalettes();
		QStringList paletteNames();
		QString paletteFileFromName(const QString& paletteName);
		
	protected:
		QStringList paletteLocations;
		QMap<QString, QString> palettes;
};
#endif
