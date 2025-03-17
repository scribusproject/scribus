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
#ifndef ICONMANAGER_H
#define ICONMANAGER_H


#include <QMap>
#include <QObject>
#include <QString>
#include <QStringView>

#include "prefsstructs.h"
#include "scribusapi.h"
#include "scpixmapcache.h"

class PrefsFile;
class QDomDocument;
class QDomElement;
class ScribusMainWindow;


/**
  * @author Craig Bradney
  * @brief Manage Scribus icons here, and here alone
  */
class SCRIBUS_API IconManager: public QObject
{
	Q_OBJECT

public:
	IconManager(IconManager const&) = delete;
	void operator=(IconManager const&) = delete;
	static IconManager& instance();

	QColor baseColor() const;
	bool iconsForDarkMode() const;
	bool setup();

	void rebuildCache();

	QCursor loadCursor(const QString& name, int hotX = -1, int hotY = -1, int width = -1);
	QIcon loadIcon(const QString& name, int width = -1);
	QIcon loadIcon(const QString& name, QSize size);
	QPixmap loadPixmap(const QString& name, int width = -1);
	QPixmap loadPixmap(const QString& name, QSize size);

	void addIconFromPainterPath(const QString & name, QPainterPath path);

	bool setActiveFromPrefs(const QString& prefsSet);
	QString activeSetBasename() { return m_activeSetBasename; }
	QString pathForIcon(const QString& name) const;
	QString baseNameForTranslation(const QString& transName) const;
	QStringList nameList(const QString& language) const;

	QRect splashScreenRect() const;
	QPixmap splashScreen() const;

protected:
	const QString classDark = "onDark";
	const QString tagIcon = "icon";
	const QString colorOnDark = "onDark";
	const QString colorOnLight = "onLight";

private:

	struct Item {
		QString name;
		QColor color;
		QString filePath;
	};

	IconManager(QObject *parent = nullptr);
	~IconManager() = default;

	static IconManager* m_instance;

	QMap<QString, ScIconSetData> m_iconSets;
	QMap<QString, QPainterPath*> m_iconPaths;
	QMap<QString, Item> m_lookupTable;
	ScPixmapCache<QString> m_pxCache;
	QString m_activeSetBasename;
	QString m_activeSetVersion;
	QString m_backupSetBasename;
	QString m_backupSetVersion;
	QRect m_splashScreenRect;
	qreal m_devicePixelRatio { 1.0 };

	bool initIconSets();
	bool createLookupTable();
	bool readXMLFile(QString filePath, QDomDocument &document, QString fileExtension);

	QPixmap *pixmapFromFile(const QString filePath, QColor color, QSize size);
	QPixmap *pixmapFromPainterPath(QPainterPath path);

	QString buildName(const QString &name, const QString &prefix = "", const QString &suffix = "") const;

	void insertPathIconsToCache();
	void readIconConfigFiles();
	void applyColors(QDomDocument &doc, QString fileName, QColor color);
	void applyInlineStyleToElement(const QDomElement &elem, QMap<QString, QString> *styles);
	void parseStyleSheet(QString styleString, QMap<QString, QString> *styles);

	QColor parseColor(const QString str);

private slots:
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	void changeColorScheme(Qt::ColorScheme colorScheme);
#endif
};

#endif
