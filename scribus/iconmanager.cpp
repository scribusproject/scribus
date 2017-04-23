/***************************************************************************
	copyright            : (C) 2015 by Craig Bradney
	email                : mrb@scribus.info
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QCursor>
#include <QDebug>
#include <QFile>
#include <QIcon>
#include <QTextCodec>
#include <QTextStream>
#include <QDomDocument>

#include "iconmanager.h"
#include "prefsmanager.h"
#include "scpaths.h"

IconManager* IconManager::m_instance = 0;

IconManager::IconManager(QObject *parent)
 : QObject(parent)
{
}

IconManager::~IconManager()
{
}

IconManager* IconManager::instance()
{
	if (m_instance == 0)
		m_instance = new IconManager();

	return m_instance;
}

void IconManager::deleteInstance()
{
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}

bool IconManager::setup()
{
	if (!initIcons())
	{
		qDebug()<<"No icons found :(";
		return false;
	}
	return true;
}


bool IconManager::initIcons()
{
	readIconConfigFiles();
	if (m_iconSets.isEmpty() || m_activeSetBasename.isEmpty())
		return false;
	return true;
}

void IconManager::readIconConfigFiles()
{
	QString baseIconDir(ScPaths::instance().iconDir());
	QStringList locations;
	locations<<baseIconDir;
	QStringList configNames;
	for ( QStringList::Iterator it = locations.begin(); it != locations.end(); ++it )
	{
		QFileInfo iconDir(*it);
		if (!iconDir.exists())
			continue;
		QDir id(*it, "*.xml", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks);
		if (!id.exists() || (id.count() == 0))
			continue;
		for (uint i = 0; i < id.count(); ++i)
		{
			QFileInfo file(*it + id[i]);
			//qDebug()<<file.absoluteFilePath();
			QFile dataFile(file.absoluteFilePath());
			if (!dataFile.exists())
				continue;
			if (!dataFile.open(QIODevice::ReadOnly))
				continue;
			QTextStream ts(&dataFile);
			ts.setCodec(QTextCodec::codecForName("UTF-8"));
			QString errorMsg;
			int eline;
			int ecol;
			QDomDocument xmlData( QString(file.baseName()));
			QString data(ts.readAll());
			dataFile.close();
			if ( !xmlData.setContent( data, &errorMsg, &eline, &ecol ))
			{
				qDebug()<<data<<errorMsg<<eline<<ecol;
				if (data.contains("404 not found", Qt::CaseInsensitive))
					qDebug()<<"File not found on server";
				else
					qDebug()<<"Could not open file"<<dataFile.fileName();
				continue;
			}
			QDomElement docElem = xmlData.documentElement();
			QDomNode n = docElem.firstChild();
			ScIconSetData isd;
			while( !n.isNull() )
			{
				QDomElement e = n.toElement();
				if( !e.isNull() )
				{
					//qDebug()<<e.tagName()<<e.text();
					if (e.tagName()=="path")
					{
						isd.path=e.text();
					}
					else
					if (e.tagName()=="author")
					{
						isd.author=e.text();
					}
					else
					if (e.tagName()=="license")
					{
						isd.license=e.text();
					}
					else
					if (e.tagName()=="activeversion")
					{
						isd.activeversion=e.text();
					}
					else
					if (e.tagName()=="nametext")
					{
						if (e.hasAttribute("lang"))
						{
							isd.nameTranslations.insert(e.attribute("lang"),e.text());
							if (e.attribute("lang")=="en_US")
								isd.baseName=e.text();
						}
					}
				}
				n = n.nextSibling();
			}
			//just in case there's no en_US basename
			if (!isd.baseName.isEmpty())
			{

				m_iconSets.insert(isd.baseName, isd);
				if(!isd.activeversion.isEmpty())
				{
					int av_major, av_minor, av_patch, curr_major, curr_minor, curr_patch, ver_major, ver_minor, ver_patch;
					av_major=isd.activeversion.section(".",0,0).toInt();
					av_minor=isd.activeversion.section(".",1,1).toInt();
					av_patch=isd.activeversion.section(".",2,2).toInt();
					curr_major=m_activeSetVersion.section(".",0,0).toInt();
					curr_minor=m_activeSetVersion.section(".",1,1).toInt();
					curr_patch=m_activeSetVersion.section(".",2,2).toInt();
					ver_major=QString(VERSION).section(".",0,0).toInt();
					ver_minor=QString(VERSION).section(".",1,1).toInt();
					ver_patch=QString(VERSION).section(".",2,2).toInt();
					//If iconset version <= app version, and iconset version >= current active iconset version
					if (av_major<=ver_major &&
						av_minor<=ver_minor &&
						av_patch<=ver_patch &&
						(
						av_major>=curr_major ||
						(av_major==curr_major && av_minor>=curr_minor) ||
						(av_major==curr_major && av_minor==curr_minor && av_patch>=curr_patch)
						)
						)
					{
						m_backupSetBasename=m_activeSetBasename;
						m_backupSetVersion=m_backupSetVersion;
						m_activeSetBasename=isd.baseName;
						m_activeSetVersion=isd.activeversion;
						//qDebug()<<"backupSetBasename"<<m_backupSetBasename<<"activeSetBasename"<<m_activeSetBasename;
					}
				}
			}
		}
	}
}

QCursor IconManager::loadCursor(const QString nam, int hotX, int hotY, bool forceUseColor)
{
	return QCursor(loadPixmap(nam, forceUseColor), hotX, hotY);
}

QIcon IconManager::loadIcon(const QString nam, bool forceUseColor)
{
	return QIcon(loadPixmap(nam, forceUseColor));
}

QPixmap IconManager::loadPixmap(const QString nam, bool forceUseColor, bool rtlFlip)
{
	if (m_pxCache.contains(nam))
		return *m_pxCache[nam];

	QString iconFilePath(pathForIcon(nam));
	QPixmap *pm = new QPixmap();
	pm->load(iconFilePath);
	if (pm->isNull())
		qWarning("Unable to load icon %s: Got null pixmap", iconFilePath.toLatin1().constData());
//	else
//		qDebug()<<"Successful icon load from"<<iconFilePath;
	if (PrefsManager::instance()->appPrefs.uiPrefs.grayscaleIcons && !forceUseColor)
		iconToGrayscale(pm);
	if (rtlFlip)
	{
		QTransform t;
		t.rotate(180);
		*pm = pm->transformed(t);
	}
	m_pxCache.insert(nam, pm);
	return *pm;
}

void IconManager::iconToGrayscale(QPixmap* pm)
{
	QImage qi(pm->toImage());
	int h=qi.height();
	int w=qi.width();
	QRgb c_rgb;
	for (int i=0;i<w;++i)
	{
		for (int j=0;j<h;++j)
		{
			c_rgb=qi.pixel(i,j);
			int k = qMin(qRound(0.3 * qRed(c_rgb) + 0.59 * qGreen(c_rgb) + 0.11 * qBlue(c_rgb)), 255);
			qi.setPixel(i, j, qRgba(k, k, k, qAlpha(c_rgb)));
		}
	}
	*pm=QPixmap::fromImage(qi);
}

bool IconManager::setActiveFromPrefs(QString prefsSet)
{
	//qDebug()<<"setting active from prefs to"<<prefsSet;
	if (m_iconSets.contains(prefsSet))
	{
		m_activeSetBasename=m_iconSets[prefsSet].baseName;
		m_activeSetVersion=m_iconSets[prefsSet].activeversion;
		return true;
	}
	return false;
}

QString IconManager::baseNameForTranslation(QString transName)
{
	QString name;
	foreach (ScIconSetData value, m_iconSets)
	{
		QMapIterator<QString, QString> nameValue(value.nameTranslations);
		while (nameValue.hasNext())
		{
			nameValue.next();
			if (nameValue.value()==transName)
				return value.nameTranslations["en_US"];
		}
	}
	return name;
}

QString IconManager::pathForIcon(const QString nam)
{
	//QString iconset(PrefsManager::instance()->appPrefs.uiPrefs.iconSet);
	QString iconSubdir(m_iconSets[m_activeSetBasename].path+"/");
	QString primaryIconSubdir(m_iconSets[m_backupSetBasename].path+"/");

	QString iconFilePath(QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, nam));
	if (QFile::exists(iconFilePath))
		return iconFilePath;

	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
	iconFilePath=QString("%1%2%3").arg(ScPaths::instance().iconDir(), primaryIconSubdir, nam);

	if (QFile::exists(iconFilePath))
	{
		//qDebug()<<iconFilePath;
		return iconFilePath;
	}
#ifdef WANT_DEBUG
	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
#endif
	return "";
}

QStringList IconManager::pathList()
{
	QStringList paths;
	foreach (ScIconSetData value, m_iconSets)
		paths << value.path;
	return paths;
}

QStringList IconManager::nameList(QString language)
{
	QStringList names;
	foreach (ScIconSetData value, m_iconSets)
	{
		if (value.nameTranslations.contains(language))
			names << value.nameTranslations.value(language);
		else if (value.nameTranslations.contains("en_US"))
			names << value.nameTranslations.value("en_US");
		else if (value.nameTranslations.contains("en"))
			names << value.nameTranslations.value("en");
		else
			names << value.path;
	}
	return names;
}

void IconManager::languageChange()
{

}

