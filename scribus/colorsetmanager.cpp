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

#include "colorsetmanager.h"
#include "scpaths.h"

ColorSetManager::ColorSetManager()
{
}

ColorSetManager::~ColorSetManager()
{
}

void ColorSetManager::initialiseDefaultPrefs(struct ApplicationPrefs& appPrefs)
{
	QString pfadC = ScPaths::instance().libDir()+"swatches/";
	QString pfadC2 = pfadC + "Scribus_X11.txt";
	QFile fiC(pfadC2);
	if (!fiC.exists())
	{
		appPrefs.DColors.insert("White", ScColor(0, 0, 0, 0));
		appPrefs.DColors.insert("Black", ScColor(0, 0, 0, 255));
		appPrefs.DColors.insert("Blue", ScColor(255, 255, 0, 0));
		appPrefs.DColors.insert("Cyan", ScColor(255, 0, 0, 0));
		appPrefs.DColors.insert("Green", ScColor(255, 0, 255, 0));
		appPrefs.DColors.insert("Red", ScColor(0, 255, 255, 0));
		appPrefs.DColors.insert("Yellow", ScColor(0, 0, 255, 0));
		appPrefs.DColors.insert("Magenta", ScColor(0, 255, 0, 0));
		appPrefs.DColorSet = "Scribus-Small";
	}
	else
	{
		if (fiC.open(IO_ReadOnly))
		{
			QString ColorEn, Cname;
			int Rval, Gval, Bval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			while (!tsC.atEnd())
			{
				ColorEn = tsC.readLine();
				QTextStream CoE(&ColorEn, IO_ReadOnly);
				CoE >> Rval;
				CoE >> Gval;
				CoE >> Bval;
				CoE >> Cname;
				ScColor tmp;
				tmp.setColorRGB(Rval, Gval, Bval);
				appPrefs.DColors.insert(Cname, tmp);
			}
			fiC.close();
		}
		appPrefs.DColorSet = "X11 RGB-Set";
	}
}

void ColorSetManager::findPaletteLocations()
{
	paletteLocations.clear();
	QStringList locations=ScPaths::instance().getSystemCreateDirs();
	locations << ScPaths::instance().libDir()+"swatches/";
	for ( QStringList::Iterator it = locations.begin(); it != locations.end(); ++it )
	{
		QFile createDir(*it);
		if (createDir.exists())
			paletteLocations << (*it);
    }
}

void ColorSetManager::findPalettes()
{
	palettes.clear();
	QString path;
	for ( QStringList::Iterator it = paletteLocations.begin(); it != paletteLocations.end(); ++it )
	{
		path=(*it);
		QDir dir(path , "*", QDir::Name, QDir::Files | QDir::NoSymLinks);
		if (dir.exists() && (dir.count() != 0))
			for (uint i = 0; i < dir.count(); ++i) 
			{
				QFileInfo file(path + dir[i]);
				QString setName=file.baseName();
				setName.replace("_", " ");
				palettes.insert(setName, file.absFilePath());
			}
	}
}

QStringList ColorSetManager::paletteNames( )
{
	QStringList nameList;
	for ( QMap<QString, QString>::Iterator it = palettes.begin(); it != palettes.end(); ++it )
		nameList << it.key();
	return nameList;
}

const QString& ColorSetManager::paletteFileFromName(const QString& paletteName)
{
	if (palettes.contains(paletteName))
		return palettes[paletteName];
	return QString::null;
}
