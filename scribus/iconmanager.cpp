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

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QIcon>
#include <QTextStream>
#include <QtSvg/QSvgRenderer>
#include <QRegularExpression>
#include <QStyleHints>

#include "api/api_application.h"
#include "iconmanager.h"
#include "scpaths.h"

IconManager* IconManager::m_instance = nullptr;

IconManager::IconManager(QObject *parent)
	: QObject(parent)
{
	m_splashScreenRect = QRect();
}

IconManager& IconManager::instance()
{
	static IconManager m_instance;
	return m_instance;
}

bool IconManager::setup()
{

	m_devicePixelRatio = qApp->devicePixelRatio();

	if (!initIconSets())
	{
		qWarning()<<"No icons found :(";
		return false;
	}

	if (!createLookupTable())
	{
		qWarning()<<"Can't load icons from iconset.";
		return false;
	}

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	connect(QApplication::styleHints(), &QStyleHints::colorSchemeChanged, this, &IconManager::changeColorScheme);
#endif

	return true;
}

QCursor IconManager::loadCursor(const QString &name, int hotX, int hotY, int width)
{
	return QCursor(loadPixmap(name, width), hotX, hotY);
}

QIcon IconManager::loadIcon(const QString& name, int width)
{
	return QIcon(loadPixmap(name, width));
}

QIcon IconManager::loadIcon(const QString &name, QSize size)
{
	return QIcon(loadPixmap(name, size));
}

QPixmap IconManager::loadPixmap(const QString &name, int width)
{
	return loadPixmap(name, QSize(width, width));
}

QPixmap IconManager::loadPixmap(const QString &name, QSize size)
{
	QString cName = buildName(name, "icon_", QString::number(size.width()) + "_" + QString::number(size.height()));

	// Use icon from icon cache
	if (m_pxCache.contains(cName))
		return *m_pxCache[cName];

	// Check in item cache
	if (m_lookupTable.contains(name))
	{
		Item item = m_lookupTable[name];
		m_pxCache.insert(cName, pixmapFromFile(item.filePath, item.color, size));
		return *m_pxCache[cName];
	}

	// Show error only if icon file doesn't exist!
	qWarning() << QString("IconManager: No icon with name '%1' found in %2").arg(name, m_activeSetBasename) ;

	return QPixmap();
}

void IconManager::addIconFromPainterPath(const QString &name, QPainterPath path)
{
	QString bn = buildName(name, "path_");

	if (!m_pxCache.contains(bn))
	{
		m_iconPaths.insert(bn, &path);
		m_pxCache.insert(bn, pixmapFromPainterPath(path));
	}
	else
	{
		qWarning() << "IconManager: Icon with name '" + name + "' already exists in cache.";
	}
}

QColor IconManager::baseColor() const
{
	return QApplication::palette().windowText().color();
}

bool IconManager::iconsForDarkMode() const
{	
 #if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	const auto* pStyleHints = QApplication::styleHints();
 	if (pStyleHints->colorScheme() == Qt::ColorScheme::Light)
 		return false;
 	if (pStyleHints->colorScheme() == Qt::ColorScheme::Dark)
 		return true;
 #endif
	return (baseColor().lightness() >= 128) ? true : false;
}

void IconManager::rebuildCache()
{
	m_pxCache.clear();

	// add icons from icon set
	if (!createLookupTable())
	{
		qWarning()<<"IconManager: Can't load icons from iconset.";
	}

	// add icons from path
	insertPathIconsToCache();
}

bool IconManager::createLookupTable()
{
		QString iconSubdir(m_iconSets[m_activeSetBasename].path);
		QString iconSetPath(QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, ".xml"));
		QDomDocument document;

		if (!readXMLFile(iconSetPath, document, "xml"))
			return false;

		m_lookupTable.clear();

		QDomElement documentElement = document.documentElement();
		QDomNodeList elements = documentElement.elementsByTagName( tagIcon );

		for (int i = 0; i < elements.length(); i++)
		{
			QDomElement icon = elements.at(i).toElement();

			QString iconPath = QString("%1%2%3").arg(ScPaths::instance().iconDir(), iconSubdir, "/" + icon.attribute("file"));
			QString iconName = icon.attribute("id");
			QColor iconColor = baseColor();

			if (m_lookupTable.contains(iconName))
				continue;

			// if defined, override icon base color with color from iconset
			if (iconsForDarkMode() && icon.hasAttribute(colorOnDark))
			{
				iconColor = parseColor(icon.attribute(colorOnDark));
			}
			else if (!iconsForDarkMode() && icon.hasAttribute(colorOnLight))
			{
				iconColor = parseColor(icon.attribute(colorOnLight));
			}

			Item item;
			item.name = iconName;
			item.filePath = iconPath;
			item.color = iconColor;

			m_lookupTable.insert(iconName, item);
		}

		return true;
}

void IconManager::insertPathIconsToCache()
{
	QMapIterator<QString, QPainterPath*> i(m_iconPaths);
	while (i.hasNext())
	{
		i.next();

		QString bn = buildName(i.key(), "path_");

		if (!m_pxCache.contains(bn))
			m_pxCache.insert(bn, pixmapFromPainterPath(*i.value()));
	}
}

void IconManager::applyColors(QDomDocument &doc, QString fileName, QColor color)
{
	QDomElement rootElement = doc.documentElement();

	// Add "dark" selector to root node
	if (iconsForDarkMode())
		rootElement.setAttribute("class", classDark);

	// Set icon base color if not already set
	if (!rootElement.hasAttribute("fill"))
	{
		QColor iconBaseColor = (color.isValid()) ? color : baseColor();
		QString baseColor = "rgb(" + QString::number(iconBaseColor.red()) + "," + QString::number(iconBaseColor.green()) + "," + QString::number(iconBaseColor.blue()) + ")";
		rootElement.setAttribute("fill", baseColor);
	}

	QMap<QString, QString> *styles = new QMap<QString, QString>();

	QDomElement nodeStyle = rootElement.firstChildElement("style");
	if (!nodeStyle.isNull())
	{
		QString styleImport = nodeStyle.childNodes().at(0).toText().data();

		static QRegularExpression rx_css("(.*?@import.*?')(.*?)('.*)");
		QRegularExpressionMatch mImport = rx_css.match(styleImport);

		// Check if stylesheet import is available
		if (mImport.hasMatch())
		{
			QString cssFile = mImport.captured(2).trimmed();
			QString absFile = QDir::cleanPath(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + cssFile);

			// Check if css file exists and add content to node
			QFile fileCSS(absFile);
			if (fileCSS.exists())
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
	if (elem.hasAttribute("class"))
	{
		QString darClass = (iconsForDarkMode()) ? "." + classDark : "";
		QString style = styles->value(darClass + "." + elem.attribute("class"));

		if (!style.isEmpty())
			elem.toElement().setAttribute("style", style);
	}

	QDomElement child = elem.firstChildElement();
	while (!child.isNull())
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
	static QRegularExpression rx_comments("(\\/\\*[\\s\\S]*?(.*?)\\*\\/)|(\\/\\/.*)");
	styleString.remove(rx_comments);

	// Group 1: tags, like ".onDark .success"
	// Group 2: strings within {}
	static QRegularExpression rx_Settings("(.*?)\\s*{([^\\}]+)}");
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
		static QRegularExpression rx_rgba("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
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
		static QRegularExpression rx_rgb("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
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
		static QRegularExpression rx_hsva("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
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
		static QRegularExpression rx_hsv("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
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
		static QRegularExpression rx_hsla("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
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
		static QRegularExpression rx_hsl("\\(\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)");
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
		static QRegularExpression rx("#?[0-9A-Fa-f]+");
		QRegularExpressionMatch mHex = rx.match(str);

		if (mHex.hasMatch())
			return QColor(mHex.captured(0));
	}
	return QColor();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
void IconManager::changeColorScheme(Qt::ColorScheme colorScheme)
{
	Q_UNUSED(colorScheme);
	rebuildCache();
}
#endif

QPixmap *IconManager::pixmapFromFile(const QString filePath, QColor color, QSize size)
{
	QDomDocument document;
	// Load SVG file
	if (readXMLFile(filePath, document, "svg"))
	{
		// process style from css file
		applyColors(document, filePath, color);

		QSvgRenderer svgRenderer(document.toByteArray());

		double svgWidth = svgRenderer.defaultSize().width();
		double svgHeight = svgRenderer.defaultSize().height();
		double width = (size.width() > 0) ?  size.width() : svgWidth;
		double height = (size.height() > 0) ?  size.height() : svgHeight;

		QSize newSize(width, height);

		// invalid width
		if (size.width() < 1)
		{
			double scaleFactor = (svgHeight > height) ? svgHeight / height : height / svgHeight;
			newSize.setWidth(svgWidth * scaleFactor);
		}

		// invalid height
		if (size.height() < 1)
		{
			double scaleFactor = (svgWidth > width) ? svgWidth / width : width / svgWidth;
			newSize.setHeight(svgHeight * scaleFactor);
		}

		// something goes wrong and we fallback to original SVG size
		if (!newSize.isValid())
			newSize = svgRenderer.defaultSize();

		QPixmap *iconPixmap = new QPixmap(newSize * m_devicePixelRatio);
		iconPixmap->setDevicePixelRatio(m_devicePixelRatio);
		iconPixmap->fill(Qt::transparent);

		QPainter painter(iconPixmap);
		svgRenderer.render(&painter, QRect(QPoint(0, 0), newSize));
		painter.end();

		return iconPixmap;
	}

	// Load any other file format
	return new QPixmap(filePath);

}

QPixmap *IconManager::pixmapFromPainterPath(QPainterPath path)
{
	QSize size(path.boundingRect().size().toSize());
	QPixmap pixmap(size * m_devicePixelRatio);
	pixmap.setDevicePixelRatio(m_devicePixelRatio);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QBrush(baseColor()));
	painter.setPen(Qt::NoPen);
	painter.drawPath(path);
	painter.end();

	return new QPixmap(pixmap);
}

QString IconManager::buildName(const QString &name, const QString &prefix, const QString &suffix) const
{
	return prefix + name + suffix;
}



bool IconManager::readXMLFile(QString filePath, QDomDocument &document, QString fileExtension)
{
	QFileInfo fileInfo(filePath);
	if (fileInfo.completeSuffix() != fileExtension)
	{
		return false;
	}

	QFile inFile( filePath );
	if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qWarning() << "IconManager: Failed to open file '" + filePath + "' for reading.";
		return false;
	}

	if (!document.setContent(&inFile))
	{
		qWarning() << "IconManager: Failed to parse the file '" + filePath + "'into a DOM tree.";
		inFile.close();
		return false;
	}

	inFile.close();
	return true;
}


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

			if (!readXMLFile(file.absoluteFilePath(), xmlData, "xml"))
				continue;

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
					isd.splashScreenDarkPath = (e.hasAttribute("imageDark")) ? e.attribute("imageDark") : "";
					isd.splashScreenLightPath = (e.hasAttribute("imageLight")) ? e.attribute("imageLight") : "";

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

//				if (!isd.isDefault)
//					continue;

				m_backupSetBasename = m_activeSetBasename;
				m_activeSetBasename = isd.baseName;
				m_activeSetVersion = isd.activeversion;
				m_splashScreenRect = isd.splashMessgeRect;
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

	rebuildCache();
	return true;
}

QString IconManager::pathForIcon(const QString &name) const
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
			if (nameValue.value() == transName)
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
	if (iconsForDarkMode())
		return QPixmap(pathForIcon(m_iconSets[m_activeSetBasename].splashScreenDarkPath));
	else
		return QPixmap(pathForIcon(m_iconSets[m_activeSetBasename].splashScreenLightPath));
}
