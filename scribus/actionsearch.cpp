/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "actionsearch.h"

#include <QAction>
#include <QDebug>
#include <QList>
#include <QMenuBar>
#include <QMenu>
#include <QStringList>

ActionSearch::ActionSearch(QMenuBar *menuBar)
	: menuBar{menuBar}
{

}

void ActionSearch::update()
{
	m_actions.clear();
	
	for (auto menuAction: menuBar->actions())
		readMenuActions(menuAction->menu());
}

void ActionSearch::execute(const QString& actionName)
{
	if (!m_actions.contains(actionName))
		return;

	QAction* action = m_actions[actionName];
	if (!action->isEnabled())
		return;
	action->trigger();
}

void ActionSearch::readMenuActions(QMenu* menu)
{
	// TODO: check why menu can be null
	if (menu == nullptr)
		return;
	
	QStringList menus;
	QMenu* currentMenu = menu;
	while (currentMenu != nullptr)
	{
		QString title = currentMenu->title().replace("&", "");
		menus.prepend(title);
		currentMenu = dynamic_cast<QMenu*>(currentMenu->parentWidget());
	}
	QString menuName(menus.join(" > "));

	for (auto action: menu->actions())
	{
		if (action->menu() != nullptr)
		{
			readMenuActions(action->menu());
			continue;
		}

		QString actionName = action->text().replace("&", "");
		if (actionName.isEmpty() || !action->isEnabled())
			continue;

		// TODO: we might want to have a multilevel menuName
		if (!menuName.isEmpty())
			actionName += " (" + menuName +")";
		m_actions.insert(actionName, action);
	}
}
