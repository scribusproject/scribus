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
	void setup();
	QCursor loadCursor(const QString nam, bool forceUseColor=false);
	QIcon loadIcon(const QString nam, bool forceUseColor=false);
	QPixmap loadPixmap(const QString nam, bool forceUseColor=false);
	QString pathForIcon(const QString nam);
	void iconToGrayscale(QPixmap *pm);

public slots:
	void languageChange();

private:
	/**
	* @brief The only instance of IconManager available.
	*
	* IconManager is singleton and the instance can be queried with the method
	* instance().
	*/
	static IconManager* _instance;
	QMap<QString, ScIconSetData> m_iconSets;

	bool initIcons();
	void readIconConfigFiles();


signals:
	void prefsChanged();
};

#endif
