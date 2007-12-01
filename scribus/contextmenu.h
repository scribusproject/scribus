/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

//CB Added 22102007 for the canvas context menu

#include <QMenu>
#include <QStringList>

class QWidget;


#include "selection.h"

class ScribusDoc;
class ScribusMainWindow;

class ContextMenu : public QMenu
{
	Q_OBJECT
public:
	ContextMenu(Selection &sel, ScribusMainWindow *actionsParent, ScribusDoc* doc, QWidget* parent=0);
	ContextMenu(ScribusMainWindow *actionsParent, ScribusDoc* doc, double mx, double my, QWidget* parent=0);
	
protected:
	void processSelection();
	void createMenuItems_Selection();
	void createMenuItems_NoSelection(double mx, double my);
	
	Selection m_Sel;
	QStringList m_actionList;
	ScribusMainWindow *m_AP;
	ScribusDoc *m_doc;
};
