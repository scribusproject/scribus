/***************************************************************************
    begin                : Jan 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRACTION_H
#define SCRACTION_H

#include <qaction.h>


/**
@author Craig Bradney
*/
class ScrAction : public QAction
{
	Q_OBJECT;
public:
	enum MenuType {Normal, RecentFile, DLL};
	
	ScrAction ( QObject *parent, const char *name = 0 ) ;
	ScrAction( const QString &menuText, QKeySequence accel, QObject *parent, const char *name = 0 );
	ScrAction( MenuType mType, const QString &menuText, QKeySequence accel, QObject *parent, const char *name = 0 );
	ScrAction( const QIconSet & icon, const QString & menuText, QKeySequence accel, QObject *parent, const char * name = 0 );
	ScrAction( const int dllID, const QString &menuText, QKeySequence accel, QObject *parent, const char *name = 0);
	ScrAction( const int dllID, const QIconSet &icon, const QString &menuText, QKeySequence accel, QObject *parent, const char *name = 0 );
    ~ScrAction();
	
		
	const int getMenuIndex();
	bool addTo(QWidget *w);
	const QWidget *getWidgetAddedTo();
signals:
	void activatedDLL(int);
	void activatedRecentFile(QString);
protected:
	int menuIndex;
	int pluginID;
	MenuType menuType;
	QWidget *widgetAddedTo;
	QWidget *containerWidgetAddedTo;
	QPopupMenu *popupMenuAddedTo;
	void addedTo( int index, QPopupMenu * menu );
	void addedTo( QWidget * actionWidget, QWidget * container );
	void setIconSizes();
private slots:
	void activatedToActivatedDLL();
	void activatedToActivatedRecentFile();
};

#endif
