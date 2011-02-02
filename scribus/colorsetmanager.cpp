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
#include "util_icon.h"
#include "util_color.h"
#include "util_formats.h"
#include "commonstrings.h"
#include "prefsstructs.h"
#include "fileloader.h"
#include "loadsaveplugin.h"
#include "plugins/formatidlist.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include <QDomElement>
#include <QTextStream>
#include <QByteArray>
#include <QDebug>

ColorSetManager::ColorSetManager()
{
}

ColorSetManager::~ColorSetManager()
{
}

void ColorSetManager::initialiseDefaultPrefs(struct ApplicationPrefs& appPrefs)
{
	QString defaultSwatch = ScPaths::instance().shareDir() + "swatches/" + "Scribus_Basic.xml";
	QFile fiC(defaultSwatch);
	if (!fiC.exists())
	{
		appPrefs.colorPrefs.DColors.insert("White", ScColor(0, 0, 0, 0));
		appPrefs.colorPrefs.DColors.insert("Black", ScColor(0, 0, 0, 255));
		ScColor cc = ScColor(255, 255, 255, 255);
		cc.setRegistrationColor(true);
		appPrefs.colorPrefs.DColors.insert("Registration", cc);
		appPrefs.colorPrefs.DColors.insert("Blue", ScColor(255, 255, 0, 0));
		appPrefs.colorPrefs.DColors.insert("Cyan", ScColor(255, 0, 0, 0));
		appPrefs.colorPrefs.DColors.insert("Green", ScColor(255, 0, 255, 0));
		appPrefs.colorPrefs.DColors.insert("Red", ScColor(0, 255, 255, 0));
		appPrefs.colorPrefs.DColors.insert("Yellow", ScColor(0, 0, 255, 0));
		appPrefs.colorPrefs.DColors.insert("Magenta", ScColor(0, 255, 0, 0));
		appPrefs.colorPrefs.DColorSet = "Scribus_Small";
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
				loadRawText(defaultSwatch, docBytes);
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
						appPrefs.colorPrefs.DColors.insert(pg.attribute("NAME"), lf);
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
					appPrefs.colorPrefs.DColors.insert(Cname, tmp);
				}
			}
			fiC.close();
		}
		appPrefs.colorPrefs.DColorSet = ScPaths::instance().shareDir() + "swatches/" + "Scribus Basic";
	}
}

void ColorSetManager::findPaletteLocations()
{
	paletteLocations.clear();
	QStringList locations=ScPaths::instance().getSystemCreateSwatchesDirs();
	locations << ScPaths::instance().shareDir()+"swatches/";
	locations << ScPaths::instance().getDirsFromEnvVar("XDG_DATA_HOME", "scribus/swatches/");
	for ( QStringList::Iterator it = locations.begin(); it != locations.end(); ++it )
	{
		QFileInfo paletteDir(*it);
		if (paletteDir.exists())
		{
			paletteLocations << (*it);
			paletteLocationLocks.insert((*it), !paletteDir.isWritable());
		}
	}
	QStringList xdgSysLocations=ScPaths::instance().getDirsFromEnvVar("XDG_DATA_DIRS", "scribus/swatches/");
	for ( QStringList::Iterator it = xdgSysLocations.begin(); it != xdgSysLocations.end(); ++it )
	{
		QFile paletteDir(*it);
		if (paletteDir.exists())
		{
			paletteLocations << (*it);
			paletteLocationLocks.insert((*it), true);
		}
	}
}

void ColorSetManager::searchDir(QString path, QMap<QString, QString> &pList, QTreeWidgetItem* parent)
{
	QStringList exts;
	exts << "acb" << "aco" << "ai" << "eps" << "gpl" << "skp" << "sla" << "soc" << "xar" << "xml";
	QDir dirs(path, "*", QDir::Name,  QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks);
	dirs.setSorting(QDir::Name | QDir::DirsFirst);
	if ((dirs.exists()) && (dirs.count() != 0))
	{
		for (uint dc = 0; dc < dirs.count(); ++dc)
		{
			QFileInfo fi(path + dirs[dc]);
			if (fi.isDir())
			{
				QDir sd(path + dirs[dc], "*", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks);
				if (sd.count() > 0)
				{
					QString setName = fi.baseName();
					setName.replace("_", " ");
					if (parent != NULL)
					{
						QTreeWidgetItem* item;
						if (path + dirs[dc] == ScPaths::getApplicationDataDir()+"swatches/locked")
							item = parent;
						else
						{
							item = new QTreeWidgetItem(parent);
							item->setFlags(Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
							item->setIcon(0, QIcon(loadIcon("16/folder.png")));
							item->setText(0, setName);
						}
						searchDir(path + dirs[dc] + "/", pList, item);
					}
					else
						searchDir(path + dirs[dc] + "/", pList, parent);
				}
			}
			else
			{
				if (exts.contains(fi.suffix().toLower()))
				{
					QString setName = fi.baseName();
					setName.replace("_", " ");
					pList.insert(fi.absolutePath() + "/" + setName, fi.absoluteFilePath());
					if (parent != 0)
					{
						QTreeWidgetItem* item = new QTreeWidgetItem(parent);
						item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						item->setText(0, setName);
						item->setData(0, Qt::UserRole, fi.absolutePath());
						if ((!fi.isWritable()) || (fi.absolutePath().contains(ScPaths::getApplicationDataDir()+"swatches/locked")))
							item->setIcon(0, QIcon(loadIcon("16/lock.png")));
					}
				}
			}
		}
	}
}

void ColorSetManager::findPalettes(QTreeWidgetItem* parent)
{
	palettes.clear();
	QString path;
	for ( QStringList::Iterator it = paletteLocations.begin(); it != paletteLocations.end(); ++it )
	{
		path = (*it);
		searchDir(path, palettes, parent);
	}
}

void ColorSetManager::findUserPalettes(QTreeWidgetItem* parent)
{
	userPalettes.clear();
	searchDir(ScPaths::getApplicationDataDir() + "swatches/", userPalettes, parent);
}

QStringList ColorSetManager::paletteNames()
{
	QStringList nameList;
	for ( QMap<QString, QString>::Iterator it = palettes.begin(); it != palettes.end(); ++it )
		nameList << it.key();
	return nameList;
}

QStringList ColorSetManager::userPaletteNames()
{
	QStringList nameList;
	for ( QMap<QString, QString>::Iterator it = userPalettes.begin(); it != userPalettes.end(); ++it )
		nameList << it.key();
	return nameList;
}

QString ColorSetManager::paletteFileFromName(const QString& paletteName)
{
	if (palettes.contains(paletteName))
		return palettes[paletteName];
	return QString();
}

QString ColorSetManager::userPaletteFileFromName(const QString& paletteName)
{
	if (userPalettes.contains(paletteName))
		return userPalettes[paletteName];
	return QString();
}

bool ColorSetManager::paletteLocationLocked(const QString& palettePath)
{
	return (paletteLocationLocks.contains(palettePath) && paletteLocationLocks.value(palettePath)==true);
}

bool ColorSetManager::checkPaletteFormat(const QString& paletteFileName)
{
	QFile f(paletteFileName);
	if(!f.open(QIODevice::ReadOnly))
		return false;
	QDomDocument docu("scridoc");
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	QString errorMsg;
	int errorLine = 0, errorColumn = 0;
	if( !docu.setContent(ts.readAll(), &errorMsg, &errorLine, &errorColumn) )
	{
		f.close();
		return false;
	}
	f.close();
	QDomElement elem = docu.documentElement();
	if (elem.tagName() != "SCRIBUSCOLORS")
		return false;
	return true;
}

bool ColorSetManager::loadPalette(const QString& paletteFileName, ScribusDoc *doc, ColorList &colors, QMap<QString,VGradient> &gradients, QMap<QString, ScPattern> &patterns, bool merge)
{
	if (checkPaletteFormat(paletteFileName))
	{
		ColorList colorListBack = doc->PageColors;
		QMap<QString, VGradient> dialogGradientsBack = doc->docGradients;
		QMap<QString, ScPattern> dialogPatternsBack = doc->docPatterns;
		if (merge)
		{
			doc->PageColors = colors;
			doc->docGradients = gradients;
			doc->docPatterns = patterns;
		}
		else
		{
			doc->PageColors.clear();
			doc->docGradients.clear();
			doc->docPatterns.clear();
		}
		const FileFormat *fmt = LoadSavePlugin::getFormatById(FORMATID_SLA150IMPORT);
		if (fmt)
		{
			fmt->setupTargets(doc, 0, doc->scMW(), 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
			fmt->loadPalette(paletteFileName);
		}
		else
			return false;
		colors = doc->PageColors;
		colors.setDocument(doc);
		gradients = doc->docGradients;
		patterns = doc->docPatterns;
		doc->PageColors = colorListBack;
		doc->docGradients = dialogGradientsBack;
		doc->docPatterns = dialogPatternsBack;
	}
	else
	{
		if (importColorsFromFile(paletteFileName, colors, &gradients, merge))
			colors.ensureDefaultColors();
		else
			return false;
	}
	return true;
}
