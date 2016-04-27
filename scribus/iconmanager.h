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

#include "prefsstructs.h"
#include "scribusapi.h"
#include "scpixmapcache.h"

class PrefsFile;
class ScribusMainWindow;


/**
  * @author Craig Bradney
  * @brief Manage Scribus icons here, and here alone
  */
class SCRIBUS_API IconManager : public QObject
{

	Q_OBJECT

	public:
		IconManager(QObject *parent = 0);
		~IconManager();

		/**
		* @brief Returns a pointer to the IconManager instance
		* @return A pointer to the IconManager instance
		*/
		static IconManager* instance();
		/**
		* @brief Deletes the IconManager Instance
		* Must be called when IconManager is no longer needed.
		*/
		static void deleteInstance();
		bool setup();
		QCursor loadCursor(const QString nam, int hotX = -1, int hotY = -1, bool forceUseColor=false);
		QIcon loadIcon(const QString nam, bool forceUseColor=false);
		QPixmap loadPixmap(const QString nam, bool forceUseColor=false, bool rtlFlip=false);
		QString pathForIcon(const QString nam);
		QStringList pathList();
		QStringList nameList(QString language);
		void iconToGrayscale(QPixmap *pm);
		bool setActiveFromPrefs(QString prefsSet);
		QString activeSetBasename() { return m_activeSetBasename; }
		QString baseNameForTranslation(QString transName);

	public slots:
		void languageChange();

	private:
		/**
		* @brief The only instance of IconManager available.
		*
		* IconManager is singleton and the instance can be queried with the method
		* instance().
		*/
		static IconManager* m_instance;
		QMap<QString, ScIconSetData> m_iconSets;
		ScPixmapCache<QString> m_pxCache;
		QString m_activeSetBasename;
		QString m_activeSetVersion;
		QString m_backupSetBasename;
		QString m_backupSetVersion;

		bool initIcons();
		void readIconConfigFiles();


	signals:
		void prefsChanged();
};

#endif
