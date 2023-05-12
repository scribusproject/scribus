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
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QTextCodec>
#include <QTextStream>
#include <QDomDocument>
#include <QApplication>
#include <QtSvg/QSvgRenderer>
#include <QRegularExpression>

#include "api/api_application.h"
#include "iconmanager.h"
#include "prefsmanager.h"
#include "scpaths.h"

IconManager* IconManager::m_instance = nullptr;

IconManager::IconManager(QObject *parent)
	: QObject(parent)
{
	m_forDarkMode = false;
	m_splashScreenRect = QRect();
	m_splashScreen = QPixmap();

}


IconManager& IconManager::instance()
{
	static IconManager m_instance;
	return m_instance;
}


bool IconManager::setup(bool forDarkMode)
{

	m_forDarkMode = forDarkMode;

	if (!initIconSets())
	{
		qWarning()<<"No icons found :(";
		return false;
	}

	if (!createCache())
	{
		qWarning()<<"Can't load icons from iconset.";
		return false;
	}

	return true;
}

QCursor IconManager::loadCursor(const QString &name, int hotX, int hotY, bool forceUseColor)
{
	return QCursor(loadPixmap(name), hotX, hotY);
}

QIcon IconManager::loadIcon(const QString& name, bool forceUseColor)
{
	return QIcon(loadPixmap(name));
}

QPixmap IconManager::loadPixmap(const QString &name, bool forceUseColor, bool rtlFlip)
{
	if (m_pxCache.contains(name))
	{
		return *m_pxCache[name];
	}
	else
	{
		qWarning() << "IconManager: No icon with name '" + name + "' in icon cache.";
		return QPixmap();
	}

}

void IconManager::addIcon(const QString &name, QPainterPath path)
{
	if (!m_pxCache.contains(name))
	{
		m_iconPaths.insert(name, &path);
		m_pxCache.insert(name, renderPath(path));
	}
	else
	{
		qWarning() << "IconManager: Icon with name '" + name + "' already exists in cache.";
	}
}

void IconManager::setIconsForDarkMode(bool forDarkMode)
{
	m_forDarkMode = forDarkMode;

}

QColor IconManager::baseColor()
{
	return qApp->palette().windowText().color();
}

void IconManager::clearCache()
{
	m_pxCache.clear();
}

void IconManager::rebuildCache()
{
	clearCache();

	// add icons from icon set
	if (!createCache())
	{
		qWarning()<<"IconManager: Can't load icons from iconset.";
	}

	// add icons from path
	renderIcons();
}

bool IconManager::createCache()
{

	QString iconSubdir(m_iconSets[m_activeSetBasename].path);
	QString iconSetPath(QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, ".xml"));
	QDomDocument document;

	if(!readXMLFile(iconSetPath, document, "xml"))
		return false;

	QDomElement documentElement = document.documentElement();
	QDomNodeList elements = documentElement.elementsByTagName( tagIcon );

	for (int i=0; i<elements.length(); i++)
	{

		QDomElement icon = elements.at(i).toElement();

		QString iconPath = QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, "/" + icon.attribute("file"));
		QString iconName = icon.attribute("id");
		QColor iconColor = baseColor();

		if (m_pxCache.contains(iconName))
			continue;

		// if defined, override icon base color with color from iconset
		if(m_forDarkMode && icon.hasAttribute(colorOnDark))
		{
			iconColor = parseColor(icon.attribute(colorOnDark));
		}
		else if(!m_forDarkMode && icon.hasAttribute(colorOnLight))
		{
			iconColor = parseColor(icon.attribute(colorOnLight));
		}

		m_pxCache.insert(iconName, initPixmap(iconPath, iconColor));

	}

	return true;

}

void IconManager::renderIcons()
{

	QMapIterator<QString, QPainterPath*> i(m_iconPaths);
	while (i.hasNext())
	{
		i.next();
		if (!m_pxCache.contains(i.key()))
			m_pxCache.insert(i.key(), renderPath(*i.value()));
	}


}

void IconManager::applyColors(QDomDocument &doc, QString fileName, QColor color)
{

	QDomElement rootElement = doc.documentElement();

	// Add "dark" selector to root node
	if(m_forDarkMode) rootElement.setAttribute("class", classDark);

	// Set icon base color if not already set
	if(!rootElement.hasAttribute("fill"))
	{
		QColor iconBaseColor = (color.isValid()) ? color : baseColor();
		QString baseColor = "rgb(" + QString::number(iconBaseColor.red()) + "," + QString::number(iconBaseColor.green()) + "," + QString::number(iconBaseColor.blue()) + ")";
		rootElement.setAttribute("fill", baseColor);
	}


	QMap<QString, QString> *styles = new QMap<QString, QString>();

	QDomElement nodeStyle = rootElement.firstChildElement("style");


	if(!nodeStyle.isNull())
	{
		QString styleImport = nodeStyle.childNodes().at(0).toText().data();

		QRegularExpression rx_css("(.*?@import.*?')(.*?)('.*)");
		QRegularExpressionMatch mImport = rx_css.match(styleImport);

		// Check if stylesheet import is available
		if (mImport.hasMatch())
		{

			QString cssFile = mImport.captured(2).trimmed();
			QString absFile = QDir::cleanPath(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + cssFile);

			// Check if css file exists and add content to node
			QFile fileCSS(absFile);
			if(fileCSS.exists())
			{

				fileCSS.open(QIODevice::ReadOnly);
				QByteArray baData = fileCSS.readAll();

				QString styleSheet = QString(baData);

				// Parse Style Sheet
				parseStyleSheet(styleSheet, styles);
			}
			else
			{
				qWarning() << "IconManager: Couldn't load CSS file for icon style! [" << fileName << "]";
			}

		}
	}

	applyInlineStyleToElement(doc.documentElement(), styles);
	delete styles;
}

void IconManager::applyInlineStyleToElement(const QDomElement &elem, QMap<QString, QString> *styles)
{
	if( elem.hasAttribute("class") )
	{
		QString darClass = (m_forDarkMode) ? "." + classDark : "";
		QString style = styles->value(darClass + "." + elem.attribute("class"));

		if(!style.isEmpty()){
			elem.toElement().setAttribute("style", style );
		}
	}

	QDomElement child = elem.firstChildElement();
	while( !child.isNull() )
	{
		applyInlineStyleToElement(child, styles);
		child = child.nextSiblingElement();
	}
}


void IconManager::parseStyleSheet(QString styleString, QMap<QString, QString> *styles)
{
	if (styleString.isEmpty())
	{
		qDebug("IconManager::parseStyleSheet: Style string is empty!");
		return;
	}

	// Remove C-style comments /* */ or // from stylesheet
	QRegularExpression rx_comments("(\\/\\*[\\s\\S]*?(.*?)\\*\\/)|(\\/\\/.*)");
	styleString.remove(rx_comments);

	// Group 1: tags, like ".onDark .success"
	// Group 2: strings within {}
	QRegularExpression rx_Settings("(.*?)\\s*{([^\\}]+)}");
	QRegularExpressionMatchIterator i = rx_Settings.globalMatch(styleString);


	while (i.hasNext())
	{
		QRegularExpressionMatch mStyle = i.next();
		styles->insert(mStyle.captured(1).trimmed().replace(" ", ""), mStyle.captured(2).trimmed());
	}
}

QColor IconManager::parseColor(const QString str)
{
	if (str.startsWith("rgba"))
	{
		QRegularExpression rx_rgba("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
		QRegularExpressionMatch mRGBA = rx_rgba.match(str);

		if (mRGBA.hasMatch())
		{
			int r = qBound(0, mRGBA.captured(1).toInt(), 255);
			int g = qBound(0, mRGBA.captured(2).toInt(), 255);
			int b = qBound(0, mRGBA.captured(3).toInt(), 255);
			qreal a = qBound(0., mRGBA.captured(4).toDouble() / 100 *255, 255.);
			return QColor(r, g, b, a);
		}

	}
	else if (str.startsWith("rgb"))
	{
		QRegularExpression rx_rgb("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
		QRegularExpressionMatch mRGB = rx_rgb.match(str);

		if (mRGB.hasMatch())
		{
			int r = qBound(0, mRGB.captured(1).toInt(), 255);
			int g = qBound(0, mRGB.captured(2).toInt(), 255);
			int b = qBound(0, mRGB.captured(3).toInt(), 255);
			return QColor(r, g, b);
		}

	}
	else if (str.startsWith("hsva"))
	{

		QRegularExpression rx_hsva("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
		QRegularExpressionMatch mHSVA = rx_hsva.match(str);

		if (mHSVA.hasMatch())
		{
			qreal h = qBound(0., mHSVA.captured(1).toDouble() / 360, 1.);
			qreal s = qBound(0., mHSVA.captured(2).toDouble() / 100, 1.);
			qreal v = qBound(0., mHSVA.captured(3).toDouble() / 100, 1.);
			qreal a = qBound(0., mHSVA.captured(4).toDouble() / 100, 1.);
			return QColor::fromHsvF(h, s, v, a);
		}

	}
	else if (str.startsWith("hsv"))
	{

		QRegularExpression rx_hsv("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
		QRegularExpressionMatch mHSV = rx_hsv.match(str);

		if (mHSV.hasMatch())
		{
			qreal h = qBound(0., mHSV.captured(1).toDouble() / 360, 1.);
			qreal s = qBound(0., mHSV.captured(2).toDouble() / 100, 1.);
			qreal v = qBound(0., mHSV.captured(3).toDouble() / 100, 1.);
			return QColor::fromHsvF(h, s, v);
		}

	}
	else if (str.startsWith("hsla"))
	{
		QRegularExpression rx_hsla("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
		QRegularExpressionMatch mHSLA = rx_hsla.match(str);

		if (mHSLA.hasMatch())
		{
			qreal h = qBound(0., mHSLA.captured(1).toDouble() / 360, 1.);
			qreal s = qBound(0., mHSLA.captured(2).toDouble() / 100, 1.);
			qreal l = qBound(0., mHSLA.captured(3).toDouble() / 100, 1.);
			qreal a = qBound(0., mHSLA.captured(4).toDouble() / 100, 1.);
			return QColor::fromHslF(h, s, l, a);
		}

	}
	else if (str.startsWith("hsl"))
	{

		QRegularExpression rx_hsl("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
		QRegularExpressionMatch mHSL = rx_hsl.match(str);

		if (mHSL.hasMatch())
		{
			qreal h = qBound(0., mHSL.captured(1).toDouble() / 360, 1.);
			qreal s = qBound(0., mHSL.captured(2).toDouble() / 100, 1.);
			qreal l = qBound(0., mHSL.captured(3).toDouble() / 100, 1.);
			return QColor::fromHslF(h, s, l);
		}

	}
	else
	{
		QRegularExpression rx("#?[0-9A-Fa-f]+");
		QRegularExpressionMatch mHex = rx.match(str);

		if (mHex.hasMatch())
			return QColor(mHex.captured(0));
	}
	return QColor();
}

QPixmap *IconManager::initPixmap(const QString filePath, QColor color)
{

	QDomDocument document;

	// Load SVG file
	if(readXMLFile(filePath, document, "svg")){

		// process style from css file
		applyColors(document, filePath, color);

		QSvgRenderer svgRenderer(document.toByteArray());

		QPixmap *iconPixmap = new QPixmap(svgRenderer.defaultSize());
		iconPixmap->fill(Qt::transparent);

		QPainter painter(iconPixmap);
		svgRenderer.render(&painter);
		painter.end();

		return iconPixmap;
	}
	// Load any other file format
	else
	{
		return new QPixmap(filePath);
	}
}

QPixmap *IconManager::renderPath(QPainterPath path)
{
	QSize size(path.boundingRect().width(), path.boundingRect().height());
	QPixmap pixmap(size);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QBrush(baseColor()));
	painter.setPen(Qt::NoPen);
	painter.drawPath(path);
	painter.end();

	return new QPixmap(pixmap);

}

bool IconManager::readXMLFile(QString filePath, QDomDocument &document, QString fileExtenstion)
{

//    QFileInfo file(filePath);
//    QFile dataFile(file.absoluteFilePath());
//    if (!dataFile.exists())
//        return false;
//    if (!dataFile.open(QIODevice::ReadOnly))
//        return false;
//    QTextStream ts(&dataFile);
//    ts.setEncoding(QStringConverter::Utf8); // --> Qt6
//    //ts.setCodec(QTextCodec::codecForName("UTF-8")); // --> Qt5
//    QString errorMsg;
//    int eline;
//    int ecol;
//    document = QDomDocument( QString(file.baseName()));
//    QString data(ts.readAll());
//    dataFile.close();

//    if (!document.setContent( data, &errorMsg, &eline, &ecol))
//    {
//        qDebug()<<data<<errorMsg<<eline<<ecol;
//        if (data.contains("404 not found", Qt::CaseInsensitive))
//            qDebug()<<"File not found on server";
//        else
//            qDebug()<<"Could not open file"<<dataFile.fileName();
//        return false;
//    }

//    return true;

		QFileInfo fileInfo(filePath);
		if(fileInfo.completeSuffix() != fileExtenstion) return false;


		QFile inFile( filePath );
		if( !inFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
		{
			qWarning() << "IconManager: Failed to open file '" + filePath + "' for reading.";
			return false;
		}

		if( !document.setContent( &inFile ) )
		{
			qWarning() << "IconManager: Failed to parse the file '" + filePath + "'into a DOM tree.";
			inFile.close();
			return false;
		}

		inFile.close();
		return true;
}

//void IconManager::tintPixmap(QPixmap &pixmap, QColor color)
//{
//    //QPixmap pm = pixmap;
//    QPainter painter(&pixmap);
//    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
//    painter.fillRect(pixmap.rect(), color);
//    painter.end();
//}



bool IconManager::initIconSets()
{
	readIconConfigFiles();
	return !(m_iconSets.isEmpty() || m_activeSetBasename.isEmpty());
}

void IconManager::readIconConfigFiles()
{
	QString baseIconDir(ScPaths::instance().iconDir());
	QStringList locations;
	locations << baseIconDir;

	for (QStringList::Iterator it = locations.begin(); it != locations.end(); ++it)
	{
		QFileInfo iconDir(*it);
		if (!iconDir.exists())
			continue;

		QDir id(*it, "*.xml", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks);
		if (!id.exists() || (id.count() == 0))
			continue;

		// Read all iconset files
		for (uint i = 0; i < id.count(); ++i)
		{

			QFileInfo file(*it + id[i]);
			//qDebug()<<file.absoluteFilePath();
			QDomDocument xmlData;

			if(!readXMLFile(file.absoluteFilePath(), xmlData, "xml")) continue;

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

					if (e.hasAttribute("default"))
					{
						isd.isDefault = (e.attribute("default") == "yes") ?  true : false;
					} else isd.isDefault = false;
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
				else if (e.tagName() == "splashscreen")
				{
					int l,t,w,h;

					l = (e.hasAttribute("left")) ? e.attribute("left").toInt() : 0;
					t = (e.hasAttribute("top")) ? e.attribute("top").toInt() : 0;
					w = (e.hasAttribute("width")) ? e.attribute("width").toInt() : 0;
					h = (e.hasAttribute("height")) ? e.attribute("height").toInt() : 0;
					isd.splashMessgeRect = QRect(l,t,w,h);
					isd.splashScreenPath = (e.hasAttribute("image")) ? e.attribute("image") : "";
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

			// Add iconset to list of available iconsets
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
				if (av_fullver == curr_fullver)
					continue;

//				if(!isd.isDefault)
//					continue;

				m_backupSetBasename = m_activeSetBasename;
				m_activeSetBasename = isd.baseName;
				m_activeSetVersion = isd.activeversion;
				m_splashScreenRect = isd.splashMessgeRect;
				m_splashScreen = QPixmap(pathForIcon(isd.splashScreenPath));
			}
		}
	}
}

bool IconManager::setActiveFromPrefs(const QString& prefsSet)
{
	//qDebug()<<"setting active from prefs to"<<prefsSet;
	if (!m_iconSets.contains(prefsSet))
		return false;
	m_activeSetBasename = m_iconSets[prefsSet].baseName;
	m_activeSetVersion = m_iconSets[prefsSet].activeversion;
	m_splashScreenRect = m_iconSets[prefsSet].splashMessgeRect;
	m_splashScreen = QPixmap(pathForIcon(m_iconSets[prefsSet].splashScreenPath));

	rebuildCache();
	return true;
}

QString IconManager::pathForIcon(const QString& name)
{
	//QString iconset(PrefsManager::instance().appPrefs.uiPrefs.iconSet);
	QString iconSubdir(m_iconSets[m_activeSetBasename].path + "/");
	QString primaryIconSubdir(m_iconSets[m_backupSetBasename].path + "/");

	QString iconFilePath(QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, name));
	if (QFile::exists(iconFilePath))
		return iconFilePath;

	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
	iconFilePath = QString("%1%2%3").arg(ScPaths::instance().iconDir(), primaryIconSubdir, name);

	if (QFile::exists(iconFilePath))
	{
		//qDebug()<<iconFilePath;
		return iconFilePath;
	}
#ifdef WANT_DEBUG
	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
#endif
	return QString();
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
			if (nameValue.value()==transName)
				return value.nameTranslations["en_US"];
		}
	}
	return name;
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

QRect IconManager::splashScreenRect() const
{
	return m_splashScreenRect;
}

QPixmap IconManager::splashScreen() const
{
	return m_splashScreen;
}


//IconManager* IconManager::m_instance = nullptr;

//IconManager::IconManager(QObject *parent)
// : QObject(parent)
//{
//}

//IconManager& IconManager::instance()
//{
//	static IconManager m_instance;
//	return m_instance;
//}

//void IconManager::clearCache()
//{
//	m_pxCache.clear();
//}

//bool IconManager::setup()
//{
//	if (!initIcons())
//	{
//		qDebug()<<"No icons found :(";
//		return false;
//	}
//	return true;
//}


//bool IconManager::initIcons()
//{
//	readIconConfigFiles();
//	return !(m_iconSets.isEmpty() || m_activeSetBasename.isEmpty());
//}

//void IconManager::readIconConfigFiles()
//{
//	QString baseIconDir(ScPaths::instance().iconDir());
//	QStringList locations;
//	locations << baseIconDir;
//	for (QStringList::Iterator it = locations.begin(); it != locations.end(); ++it)
//	{
//		QFileInfo iconDir(*it);
//		if (!iconDir.exists())
//			continue;
//		QDir id(*it, "*.xml", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks);
//		if (!id.exists() || (id.count() == 0))
//			continue;
//		for (uint i = 0; i < id.count(); ++i)
//		{
//			QFileInfo file(*it + id[i]);
//			//qDebug()<<file.absoluteFilePath();
//			QFile dataFile(file.absoluteFilePath());
//			if (!dataFile.exists())
//				continue;
//			if (!dataFile.open(QIODevice::ReadOnly))
//				continue;
//			QTextStream ts(&dataFile);
//			ts.setEncoding(QStringConverter::Utf8);
//			QString errorMsg;
//			int eline;
//			int ecol;
//			QDomDocument xmlData( QString(file.baseName()));
//			QString data(ts.readAll());
//			dataFile.close();
//			if (!xmlData.setContent( data, &errorMsg, &eline, &ecol))
//			{
//				qDebug()<<data<<errorMsg<<eline<<ecol;
//				if (data.contains("404 not found", Qt::CaseInsensitive))
//					qDebug()<<"File not found on server";
//				else
//					qDebug()<<"Could not open file"<<dataFile.fileName();
//				continue;
//			}
//			QDomElement docElem = xmlData.documentElement();
//			ScIconSetData isd;
//			for (QDomNode n = docElem.firstChild(); !n.isNull(); n = n.nextSibling())
//			{
//				QDomElement e = n.toElement();
//				if (e.isNull())
//					continue;
//				//qDebug()<<e.tagName()<<e.text();
//				if (e.tagName() == "path")
//				{
//					isd.path = e.text();
//				}
//				else if (e.tagName() == "author")
//				{
//					isd.author = e.text();
//				}
//				else if (e.tagName() == "license")
//				{
//					isd.license = e.text();
//				}
//				else if (e.tagName() == "activeversion")
//				{
//					isd.activeversion = e.text();
//				}
//				else if (e.tagName() == "variant")
//				{
//					isd.variant = e.text();
//				}
//				else if (e.tagName() == "nametext")
//				{
//					if (e.hasAttribute("lang"))
//					{
//						isd.nameTranslations.insert(e.attribute("lang"),e.text());
//						if (e.attribute("lang") == "en_US")
//							isd.baseName = e.text();
//					}
//				}
//			}

//			//just in case there's no en_US basename
//			if (isd.baseName.isEmpty())
//				continue;

//			m_iconSets.insert(isd.baseName, isd);
//			if (isd.activeversion.isEmpty())
//				continue;

//			int av_major = isd.activeversion.section(".", 0, 0).toInt();
//			int av_minor = isd.activeversion.section(".", 1, 1).toInt();
//			int av_patch = isd.activeversion.section(".", 2, 2).toInt();
//			int av_fullver = av_major * 10000 + av_minor * 100 + av_patch;
//			int curr_major = m_activeSetVersion.section(".", 0, 0).toInt();
//			int curr_minor = m_activeSetVersion.section(".", 1, 1).toInt();
//			int curr_patch = m_activeSetVersion.section(".", 2, 2).toInt();
//			int curr_fullver = curr_major * 10000 + curr_minor * 100 + curr_patch;
//			int ver_major = ScribusAPI::getVersionMajor();
//			int ver_minor = ScribusAPI::getVersionMinor();
//			int ver_patch = ScribusAPI::getVersionPatch();
//			int ver_fullver = ver_major * 10000 + ver_minor * 100 + ver_patch;
//			//If iconset version <= app version, and iconset version >= current active iconset version
//			if ((av_fullver <= ver_fullver) && (av_fullver >= curr_fullver))
//			{
//				// Do not use variant of an iconset such as "dark" ones by default
//				if ((av_fullver == curr_fullver) && !isd.variant.isEmpty())
//					continue;
//				m_backupSetBasename = m_activeSetBasename;
////				m_backupSetVersion = m_backupSetVersion;
//				m_activeSetBasename = isd.baseName;
//				m_activeSetVersion = isd.activeversion;
//				//qDebug()<<"backupSetBasename"<<m_backupSetBasename<<"activeSetBasename"<<m_activeSetBasename;
//			}
//		}
//	}
//}

//QCursor IconManager::loadCursor(const QString& name, int hotX, int hotY, bool forceUseColor)
//{
//	return QCursor(loadPixmap(name, forceUseColor), hotX, hotY);
//}

//QIcon IconManager::loadIcon(const QString& name, bool forceUseColor)
//{
//	return QIcon(loadPixmap(name, forceUseColor));
//}

//QPixmap IconManager::loadPixmap(const QString& name, bool forceUseColor, bool rtlFlip)
//{
//	if (m_pxCache.contains(name))
//		return *m_pxCache[name];

//	QString iconFilePath(pathForIcon(name));
//	QPixmap *pm = new QPixmap();
//	pm->load(iconFilePath);
//	if (pm->isNull())
//		qWarning("Unable to load icon %s: Got null pixmap", iconFilePath.toLatin1().constData());
////	else
////		qDebug()<<"Successful icon load from"<<iconFilePath;
//	if (PrefsManager::instance().appPrefs.uiPrefs.grayscaleIcons && !forceUseColor)
//		iconToGrayscale(pm);
//	if (rtlFlip)
//	{
//		QTransform t;
//		t.rotate(180);
//		*pm = pm->transformed(t);
//	}
//	m_pxCache.insert(name, pm);
//	return *pm;
//}

//void IconManager::iconToGrayscale(QPixmap* pm)
//{
//	QImage qi(pm->toImage());
//	int h = qi.height();
//	int w = qi.width();
//	QRgb c_rgb;
//	for (int i = 0; i < w; ++i)
//	{
//		for (int j = 0; j < h; ++j)
//		{
//			c_rgb = qi.pixel(i, j);
//			int k = qMin(qRound(0.3 * qRed(c_rgb) + 0.59 * qGreen(c_rgb) + 0.11 * qBlue(c_rgb)), 255);
//			qi.setPixel(i, j, qRgba(k, k, k, qAlpha(c_rgb)));
//		}
//	}
//	*pm = QPixmap::fromImage(qi);
//}

//bool IconManager::setActiveFromPrefs(const QString& prefsSet)
//{
//	//qDebug()<<"setting active from prefs to"<<prefsSet;
//	if (!m_iconSets.contains(prefsSet))
//		return false;
//	m_activeSetBasename = m_iconSets[prefsSet].baseName;
//	m_activeSetVersion = m_iconSets[prefsSet].activeversion;
//	return true;
//}

//QString IconManager::baseNameForTranslation(const QString& transName) const
//{
//	QString name;
//	for (const ScIconSetData& value : m_iconSets)
//	{
//		QMapIterator<QString, QString> nameValue(value.nameTranslations);
//		while (nameValue.hasNext())
//		{
//			nameValue.next();
//			if (nameValue.value()==transName)
//				return value.nameTranslations["en_US"];
//		}
//	}
//	return name;
//}

//QString IconManager::pathForIcon(const QString& name)
//{
//	//QString iconset(PrefsManager::instance().appPrefs.uiPrefs.iconSet);
//	QString iconSubdir(m_iconSets[m_activeSetBasename].path + "/");
//	QString primaryIconSubdir(m_iconSets[m_backupSetBasename].path + "/");

//	QString iconFilePath(QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, name));
//	if (QFile::exists(iconFilePath))
//		return iconFilePath;

//	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
//	iconFilePath = QString("%1%2%3").arg(ScPaths::instance().iconDir(), primaryIconSubdir, name);

//	if (QFile::exists(iconFilePath))
//	{
//		//qDebug()<<iconFilePath;
//		return iconFilePath;
//	}
//#ifdef WANT_DEBUG
//	qWarning("pathForIcon: Unable to load icon %s: File not found", iconFilePath.toLatin1().constData());
//#endif
//	return QString();
//}

//QStringList IconManager::pathList() const
//{
//	QStringList paths;
//	for (const ScIconSetData& value :  m_iconSets)
//		paths << value.path;
//	return paths;
//}

//QStringList IconManager::nameList(const QString& language) const
//{
//	QStringList names;
//	for (const ScIconSetData& value : m_iconSets)
//	{
//		if (value.nameTranslations.contains(language))
//			names << value.nameTranslations.value(language);
//		else if (value.nameTranslations.contains("en_US"))
//			names << value.nameTranslations.value("en_US");
//		else if (value.nameTranslations.contains("en"))
//			names << value.nameTranslations.value("en");
//		else
//			names << value.path;
//	}
//	return names;
//}

//void IconManager::languageChange()
//{

//}

