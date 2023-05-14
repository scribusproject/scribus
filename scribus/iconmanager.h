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

	void setIconsForDarkMode(bool forDarkMode);
	QColor baseColor();

	void clearCache();
	void rebuildCache();
	bool createCache();

	bool setup(bool forDarkMode = false);

	QCursor loadCursor(const QString& name, int hotX = -1, int hotY = -1, bool forceUseColor=false);
	QIcon loadIcon(const QString& name, bool forceUseColor=false);
	QPixmap loadPixmap(const QString& name, bool forceUseColor=false, bool rtlFlip=false);

	void addIcon(const QString & name, QPainterPath path);

	bool setActiveFromPrefs(const QString& prefsSet);
	QString activeSetBasename() { return m_activeSetBasename; }
	QString pathForIcon(const QString& name);
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

	IconManager(QObject *parent = nullptr);
	~IconManager() = default;

	static IconManager* m_instance;

	QMap<QString, ScIconSetData> m_iconSets;
	QMap<QString, QPainterPath*> m_iconPaths;
	ScPixmapCache<QString> m_pxCache;
	QString m_activeSetBasename;
	QString m_activeSetVersion;
	QString m_backupSetBasename;
	QString m_backupSetVersion;
	bool m_forDarkMode;
	QRect m_splashScreenRect;
	QPixmap m_splashScreen;

	bool initIconSets();
	void readIconConfigFiles();

	QPixmap *initPixmap(const QString filePath, QColor color);
	QPixmap *renderPath(QPainterPath path);
	void renderIcons();

	bool readXMLFile(QString filePath, QDomDocument &document, QString fileExtenstion);
//    void tintPixmap(QPixmap &pixmap, QColor color);

	void applyColors(QDomDocument &doc, QString fileName, QColor color);
	void applyInlineStyleToElement(const QDomElement &elem, QMap<QString, QString> *styles);
	void parseStyleSheet(QString styleString, QMap<QString, QString> *styles);

	QColor parseColor(const QString str);

};


//class SCRIBUS_API IconManager : public QObject
//{

//	Q_OBJECT

//	public:
//		IconManager(IconManager const&) = delete;
//		void operator=(IconManager const&) = delete;
//		/**
//		* @brief Returns a pointer to the IconManager instance
//		* @return A pointer to the IconManager instance
//		*/
//		static IconManager& instance();
		
//		void clearCache();
//		bool setup();

//		QCursor loadCursor(const QString& name, int hotX = -1, int hotY = -1, bool forceUseColor=false);
//		QIcon loadIcon(const QString& name, bool forceUseColor=false);
//		QPixmap loadPixmap(const QString& name, bool forceUseColor=false, bool rtlFlip=false);
//		QString pathForIcon(const QString& name);
//		QStringList pathList() const;
//		QStringList nameList(const QString& language) const;
//		void iconToGrayscale(QPixmap *pm);
//		bool setActiveFromPrefs(const QString& prefsSet);
//		QString activeSetBasename() { return m_activeSetBasename; }
//		QString baseNameForTranslation(const QString& transName) const;

//	public slots:
//		void languageChange();

//	private:
//		IconManager(QObject *parent = nullptr);
//		~IconManager() = default;
//		/**
//		* @brief The only instance of IconManager available.
//		*
//		* IconManager is singleton and the instance can be queried with the method
//		* instance().
//		*/
//		static IconManager* m_instance;
//		QMap<QString, ScIconSetData> m_iconSets;
//		ScPixmapCache<QString> m_pxCache;
//		QString m_activeSetBasename;
//		QString m_activeSetVersion;
//		QString m_backupSetBasename;
//		QString m_backupSetVersion;

//		bool initIcons();
//		void readIconConfigFiles();


//	signals:
//		void prefsChanged();
//};

#endif
