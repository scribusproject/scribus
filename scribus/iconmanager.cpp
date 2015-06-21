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
#include "scpixmapcache.h"

IconManager* IconManager::_instance = 0;

IconManager::IconManager(QObject *parent)
 : QObject(parent)
{
}

IconManager::~IconManager()
{
}

IconManager* IconManager::instance()
{
	if (_instance == 0)
		_instance = new IconManager();

	return _instance;
}

void IconManager::deleteInstance()
{
	if (_instance)
		delete _instance;
	_instance = 0;
}

void IconManager::setup()
{
	if (!initIcons())
		qDebug()<<"No icons found :(";
}


bool IconManager::initIcons()
{
	readIconConfigFiles();
	if (m_iconSets.isEmpty())
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
			qDebug()<<file.absoluteFilePath();
			QFile dataFile(file.absoluteFilePath());
			if (!dataFile.exists())
				continue;
			dataFile.open(QIODevice::ReadOnly);
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
				if (data.toLower().contains("404 not found"))
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
					if (e.tagName()=="path")
					{
						isd.path=e.text();
						isd.baseName=e.text();
						qDebug()<<e.tagName()<<e.text();
					}
					else
					if (e.tagName()=="author")
					{
						isd.author=e.text();
						qDebug()<<e.tagName()<<e.text();
					}
					else
					if (e.tagName()=="license")
					{
						isd.license=e.text();
						qDebug()<<e.tagName()<<e.text();
					}
					else
					if (e.tagName()=="nametext")
					{
						if (e.hasAttribute("lang"))
							isd.nameTranslations.insert(e.attribute("lang"),e.text());
						qDebug()<<e.tagName()<<e.text();
					}
				}
				n = n.nextSibling();
			}
			m_iconSets.insert(isd.path, isd);
		}
	}
}

QCursor IconManager::loadCursor(const QString nam, bool forceUseColor)
{
	return QCursor(loadPixmap(nam, forceUseColor));
}

QIcon IconManager::loadIcon(const QString nam, bool forceUseColor)
{
	return QIcon(loadPixmap(nam, forceUseColor));
}

QPixmap IconManager::loadPixmap(const QString nam, bool forceUseColor)
{
	static ScPixmapCache<QString> pxCache;
	if (pxCache.contains(nam))
		return *pxCache[nam];

	QString iconFilePath(pathForIcon(nam));
	QPixmap *pm = new QPixmap();
	pm->load(iconFilePath);
	if (pm->isNull())
		qWarning("Unable to load icon %s: Got null pixmap", iconFilePath.toLatin1().constData());
//	else
//		qDebug()<<"Successful icon load from"<<iconFilePath;
	if (PrefsManager::instance()->appPrefs.uiPrefs.grayscaleIcons && !forceUseColor)
		iconToGrayscale(pm);
	pxCache.insert(nam, pm);
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




QString IconManager::pathForIcon(const QString nam)
{
	QString iconset(PrefsManager::instance()->appPrefs.uiPrefs.iconSet);
	QString iconSubdir("1_5_0/");
	QString primaryIconSubdir("1_5_0/");
	//temporary until second icon dir is there
	if (iconset=="Dezso" || iconset=="1_5_1")
		iconSubdir="1_5_1/";


	QString iconFilePath(QString("%1%2%3").arg(ScPaths::instance().iconDir()).arg(iconSubdir).arg(nam));
	if (QFile::exists(iconFilePath))
		return iconFilePath;

	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
	iconFilePath=QString("%1%2%3").arg(ScPaths::instance().iconDir()).arg(primaryIconSubdir).arg(nam);

	if (QFile::exists(iconFilePath))
		return iconFilePath;

	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
	return "";
}

