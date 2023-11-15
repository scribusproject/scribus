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

#include "api/api_application.h"
#include "iconmanager.h"
#include "prefsmanager.h"
#include "scpaths.h"

IconManager* IconManager::m_instance = nullptr;

IconManager::IconManager(QObject *parent)
 : QObject(parent)
{
}

IconManager& IconManager::instance()
{
	static IconManager m_instance;
	return m_instance;
}

void IconManager::clearCache()
{
	m_pxCache.clear();
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
	return !(m_iconSets.isEmpty() || m_activeSetBasename.isEmpty());
}

void IconManager::readIconConfigFiles()
{
	QString baseIconDir(ScPaths::instance().iconDir());
	QStringList locations;
	locations << baseIconDir;
	QStringList configNames;
	for (QStringList::Iterator it = locations.begin(); it != locations.end(); ++it)
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
			if (!xmlData.setContent( data, &errorMsg, &eline, &ecol))
			{
				qDebug()<<data<<errorMsg<<eline<<ecol;
				if (data.contains("404 not found", Qt::CaseInsensitive))
					qDebug()<<"File not found on server";
				else
					qDebug()<<"Could not open file"<<dataFile.fileName();
				continue;
			}
			QDomElement docElem = xmlData.documentElement();
			ScIconSetData isd;
			for (QDomNode n = docElem.firstChild(); !n.isNull(); n = n.nextSibling())
			{
				QDomElement e = n.toElement();
				if (e.isNull())
					continue;
				//qDebug()<<e.tagName()<<e.text();
				if (e.tagName() == "path")
				{
					isd.path = e.text();
				}
				else if (e.tagName() == "author")
				{
					isd.author = e.text();
				}
				else if (e.tagName() == "license")
				{
					isd.license = e.text();
				}
				else if (e.tagName() == "activeversion")
				{
					isd.activeversion = e.text();
				}
				else if (e.tagName() == "variant")
				{
					isd.variant = e.text();
				}
				else if (e.tagName() == "nametext")
				{
					if (e.hasAttribute("lang"))
					{
						isd.nameTranslations.insert(e.attribute("lang"),e.text());
						if (e.attribute("lang") == "en_US")
							isd.baseName = e.text();
					}
				}
			}

			//just in case there's no en_US basename
			if (isd.baseName.isEmpty())
				continue;

			m_iconSets.insert(isd.baseName, isd);
			if (isd.activeversion.isEmpty())
				continue;

			int av_major = isd.activeversion.section(".", 0, 0).toInt();
			int av_minor = isd.activeversion.section(".", 1, 1).toInt();
			int av_patch = isd.activeversion.section(".", 2, 2).toInt();
			int av_fullver = av_major * 10000 + av_minor * 100 + av_patch;
			int curr_major = m_activeSetVersion.section(".", 0, 0).toInt();
			int curr_minor = m_activeSetVersion.section(".", 1, 1).toInt();
			int curr_patch = m_activeSetVersion.section(".", 2, 2).toInt();
			int curr_fullver = curr_major * 10000 + curr_minor * 100 + curr_patch;
			int ver_major = ScribusAPI::getVersionMajor();
			int ver_minor = ScribusAPI::getVersionMinor();
			int ver_patch = ScribusAPI::getVersionPatch();
			int ver_fullver = ver_major * 10000 + ver_minor * 100 + ver_patch;
			//If iconset version <= app version, and iconset version >= current active iconset version
			if ((av_fullver <= ver_fullver) && (av_fullver >= curr_fullver))
			{
				// Do not use variant of an iconset such as "dark" ones by default
				if ((av_fullver == curr_fullver) && !isd.variant.isEmpty())
					continue;
				m_backupSetBasename = m_activeSetBasename;
//				m_backupSetVersion = m_backupSetVersion;
				m_activeSetBasename = isd.baseName;
				m_activeSetVersion = isd.activeversion;
				//qDebug()<<"backupSetBasename"<<m_backupSetBasename<<"activeSetBasename"<<m_activeSetBasename;
			}
		}
	}
}

QCursor IconManager::loadCursor(const QString& name, int hotX, int hotY, bool forceUseColor)
{
	return QCursor(loadPixmap(name, forceUseColor), hotX, hotY);
}

QIcon IconManager::loadIcon(const QString& name, bool forceUseColor)
{
	return QIcon(loadPixmap(name, forceUseColor));
}

QPixmap IconManager::loadPixmap(const QString& name, bool forceUseColor, bool rtlFlip)
{
	if (m_pxCache.contains(name))
		return *m_pxCache[name];

	QString iconFilePath(pathForIcon(name));
	QPixmap *pm = new QPixmap();
	pm->load(iconFilePath);
	if (pm->isNull())
		qWarning("Unable to load icon %s: Got null pixmap", iconFilePath.toLatin1().constData());
//	else
//		qDebug()<<"Successful icon load from"<<iconFilePath;
	if (PrefsManager::instance().appPrefs.uiPrefs.grayscaleIcons && !forceUseColor)
		iconToGrayscale(pm);
	if (rtlFlip)
	{
		QTransform t;
		t.rotate(180);
		*pm = pm->transformed(t);
	}
	m_pxCache.insert(name, pm);
	return *pm;
}

void IconManager::iconToGrayscale(QPixmap* pm)
{
	QImage qi(pm->toImage());
	int h = qi.height();
	int w = qi.width();
	QRgb c_rgb;
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			c_rgb = qi.pixel(i, j);
			int k = qMin(qRound(0.3 * qRed(c_rgb) + 0.59 * qGreen(c_rgb) + 0.11 * qBlue(c_rgb)), 255);
			qi.setPixel(i, j, qRgba(k, k, k, qAlpha(c_rgb)));
		}
	}
	*pm = QPixmap::fromImage(qi);
}

bool IconManager::setActiveFromPrefs(const QString& prefsSet)
{
	//qDebug()<<"setting active from prefs to"<<prefsSet;
	if (!m_iconSets.contains(prefsSet))
		return false;
	m_activeSetBasename = m_iconSets[prefsSet].baseName;
	m_activeSetVersion = m_iconSets[prefsSet].activeversion;
	return true;
}

QString IconManager::baseNameForTranslation(const QString& transName) const
{
	QString name;
	for (const ScIconSetData& value : m_iconSets)
	{
		QMapIterator<QString, QString> nameValue(value.nameTranslations);
		while (nameValue.hasNext())
		{
			nameValue.next();
			if (nameValue.value() == transName)
				return value.nameTranslations["en_US"];
		}
	}
	return name;
}

QString IconManager::pathForIcon(const QString& name)
{
	//QString iconset(PrefsManager::instance().appPrefs.uiPrefs.iconSet);
	QString iconSubdir(m_iconSets[m_activeSetBasename].path + "/");
	QString primaryIconSubdir(m_iconSets[m_backupSetBasename].path + "/");

	QString iconFilePath(QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, name));
	if (QFile::exists(iconFilePath))
		return iconFilePath;

	QString iconFilePath2(QString("%1%2%3").arg(ScPaths::instance().iconDir(), primaryIconSubdir, name));
	if (QFile::exists(iconFilePath2))
		return iconFilePath2;

	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath2.toLatin1().constData());

	return QString();
}

QStringList IconManager::pathList() const
{
	QStringList paths;
	for (const ScIconSetData& value :  m_iconSets)
		paths << value.path;
	return paths;
}

QStringList IconManager::nameList(const QString& language) const
{
	QStringList names;
	for (const ScIconSetData& value : m_iconSets)
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

