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
#include "util.h"
#include "commonstrings.h"
#include "prefsstructs.h"
#include <QDomElement>
#include <QTextStream>
#include <QByteArray>
//Added by qt3to4:

ColorSetManager::ColorSetManager()
{
}

ColorSetManager::~ColorSetManager()
{
}

void ColorSetManager::initialiseDefaultPrefs(struct ApplicationPrefs& appPrefs)
{
	QString pfadC = ScPaths::instance().shareDir()+"swatches/";
	QString pfadC2 = pfadC + "Scribus_Basic.xml";
	QFile fiC(pfadC2);
	if (!fiC.exists())
	{
		appPrefs.DColors.insert("White", ScColor(0, 0, 0, 0));
		appPrefs.DColors.insert("Black", ScColor(0, 0, 0, 255));
		ScColor cc = ScColor(255, 255, 255, 255);
		cc.setRegistrationColor(true);
		appPrefs.DColors.insert("Registration", cc);
		appPrefs.DColors.insert("Blue", ScColor(255, 255, 0, 0));
		appPrefs.DColors.insert("Cyan", ScColor(255, 0, 0, 0));
		appPrefs.DColors.insert("Green", ScColor(255, 0, 255, 0));
		appPrefs.DColors.insert("Red", ScColor(0, 255, 255, 0));
		appPrefs.DColors.insert("Yellow", ScColor(0, 0, 255, 0));
		appPrefs.DColors.insert("Magenta", ScColor(0, 255, 0, 0));
		appPrefs.DColorSet = "Scribus_Small";
	}
	else
	{
		if (fiC.open(QIODevice::ReadOnly))
		{
			QString ColorEn, Cname;
			int Rval, Gval, Bval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			if (ColorEn.startsWith("<?xml version="))
			{
				QByteArray docBytes("");
				loadRawText(pfadC2, docBytes);
				QString docText("");
				docText = QString::fromUtf8(docBytes);
				QDomDocument docu("scridoc");
				docu.setContent(docText);
				ScColor lf = ScColor();
				QDomElement elem = docu.documentElement();
				QDomNode PAGE = elem.firstChild();
				while(!PAGE.isNull())
				{
					QDomElement pg = PAGE.toElement();
					if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
					{
						if (pg.hasAttribute("CMYK"))
							lf.setNamedColor(pg.attribute("CMYK"));
						else
							lf.fromQColor(QColor(pg.attribute("RGB")));
						if (pg.hasAttribute("Spot"))
							lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
						else
							lf.setSpotColor(false);
						if (pg.hasAttribute("Register"))
							lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
						else
							lf.setRegistrationColor(false);
						appPrefs.DColors.insert(pg.attribute("NAME"), lf);
					}
					PAGE=PAGE.nextSibling();
				}
			}
			else
			{
				while (!tsC.atEnd())
				{
					ColorEn = tsC.readLine();
					QTextStream CoE(&ColorEn, QIODevice::ReadOnly);
					CoE >> Rval;
					CoE >> Gval;
					CoE >> Bval;
					CoE >> Cname;
					ScColor tmp;
					tmp.setColorRGB(Rval, Gval, Bval);
					appPrefs.DColors.insert(Cname, tmp);
				}
			}
			fiC.close();
		}
		appPrefs.DColorSet = "Scribus Basic";
	}
}

void ColorSetManager::findPaletteLocations()
{
	paletteLocations.clear();
	QStringList locations=ScPaths::instance().getSystemCreateSwatchesDirs();
	locations << ScPaths::instance().shareDir()+"swatches/";
	locations << ScPaths::getApplicationDataDir() + "swatches/locked/";
	locations << ScPaths::getApplicationDataDir() + "swatches/";
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
		QDir dir(path , "*.xml *.gpl *.eps", QDir::Name, QDir::Files | QDir::NoSymLinks);
		if (dir.exists() && (dir.count() != 0))
			for (uint i = 0; i < dir.count(); ++i) 
			{
				QFileInfo file(path + dir[i]);
				QString setName=file.baseName();
				setName.replace("_", " ");
				palettes.insert(setName, file.absoluteFilePath());
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

QString ColorSetManager::paletteFileFromName(const QString& paletteName)
{
	if (palettes.contains(paletteName))
		return palettes[paletteName];
	return QString();
}
