/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ACTIONSEARCH_H
#define ACTIONSEARCH_H

class QAction;
class QMenu;
class QMenuBar;

#include <QList>
#include <QHash>

#include "scribusapi.h"

class SCRIBUS_API ActionSearch
{
public:
	ActionSearch(QMenuBar *menuBar);
	~ActionSearch() = default;
	
	QList<QString> getActionNames() const { return m_actions.keys(); }

	void update();
	void execute(const QString& actionName);

private:
	QMenuBar* menuBar;

	void readMenuActions(QMenu* menu);

	QHash<QString, QAction*> m_actions; // std::unordered_map
};

#endif
