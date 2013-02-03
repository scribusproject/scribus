/***************************************************************************
 *   Copyright (C) 2009 by Pierre Marchand                                 *
 *   pierre@oep-h.com                                                      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#ifndef PAGEITEMSETTERSMANAGER_H
#define PAGEITEMSETTERSMANAGER_H

#include <QObject>
#include <QList>

class PageItemSetterBase;
class PageItem;
class Selection;
class ScribusDoc;

/**
  PageItemSettersManager acts as a registry for page item setters.
  Notifying setters of document changes (selection, units, etc.).
  */

class PageItemSettersManager : public QObject
{
	Q_OBJECT

	static PageItemSettersManager * instance;
	static PageItemSettersManager * that();

	PageItemSettersManager(QObject * parent = 0);

	QList<PageItemSetterBase*> setters;
	Selection * selection;
	ScribusDoc * doc;

public:
	static void registerSetter(PageItemSetterBase* base);
	static void setSelection(Selection* selection);
	static PageItemSetterBase* getClone(const QString& type);

private slots:
	void UnRegisterSetter();
	void rotationModeChanged(int);
	void updateSelection();

};

#endif // PAGEITEMSETTERSMANAGER_H
