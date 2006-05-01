/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2006 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#ifndef TABKEYBOARDSHORTCUTSWIDGET_H
#define TABKEYBOARDSHORTCUTSWIDGET_H

#include "tabkeyboardshortcutswidgetbase.h"
#include "scribusapi.h"
#include "scribusstructs.h"

#include <qmap.h>
#include <qpair.h>
#include <qptrlist.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qvaluevector.h>

class QListViewItem;

class TabKeyboardShortcutsWidget: public TabKeyboardShortcutsWidgetBase 
{
	Q_OBJECT
	public:
		TabKeyboardShortcutsWidget(QMap<QString, Keys> oldKeyMap, QWidget *parent = 0, const char *name = 0);
		const QMap<QString, Keys> getNewKeyMap();
		static QString getKeyText(int KeyC);
		bool event( QEvent* ev );
		void keyPressEvent(QKeyEvent *k);
		void keyReleaseEvent(QKeyEvent *k);
		
	protected:
		QMap<QString,Keys> keyMap;
		QMap<QString,Keys>::Iterator currentKeyMapRow;
		QMap<QString, QString> keySetList;
		QMap<QListViewItem*, QString> lviToActionMap;
		QPtrList<QListViewItem> lviToMenuMap;
		QValueVector< QPair<QString, QStringList> >* defMenus;
		QListViewItem* selectedLVI;
		int keyCode;
		QString Part0;
		QString Part1;
		QString Part2;
		QString Part3;
		QString Part4;
	
		void insertActions();
		void importKeySet(QString);
		bool exportKeySet(QString);
		QStringList scanForSets();
		bool checkKey(int Code);
	
	protected slots:
		void setKeyText();
		void dispKey(QListViewItem*);
		void setNoKey();
		void loadKeySetFile();
		void importKeySetFile();
		void exportKeySetFile();
		void resetKeySet();
		void clearSearchString();
		void applySearch( const QString & newss );
};

#endif
