/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2010 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
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
#ifndef SWATCHCOMBO_H
#define SWATCHCOMBO_H

#include <QTreeWidget>
#include <QToolButton>
#include <QMenu>
#include <QWidgetAction>

#include "scribusapi.h"

class SCRIBUS_API SwatchCombo : public QToolButton
{
	Q_OBJECT

public:
	SwatchCombo( QWidget* parent = NULL );
	QTreeWidgetItem* addTopLevelItem(QString name);
	QTreeWidgetItem* addSubItem(QString name, QTreeWidgetItem* parent, bool selectable = true);
	void setCurrentComboItem(QString text);
	QTreeWidgetItem* currentItem();
signals:
	void activated(const QString &);
	void activated(QTreeWidgetItem*);
private slots:
	void itemActivated(QTreeWidgetItem* item);
private:
	QTreeWidget* dataTree;
	QMenu* popUp;
	QWidgetAction* popUpAct;
};
#endif
