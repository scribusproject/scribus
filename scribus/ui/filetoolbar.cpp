/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          texttoolb.cpp  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QString>
#include <QToolButton>

#include "filetoolbar.h"
#include "ui/scmwmenumanager.h"
#include "scraction.h"
#include "scribus.h"

FileToolBar::FileToolBar(ScribusMainWindow* p) : ScToolBar( tr("File"), "File", p)
{
	parent=p;
	addAction(p->scrActions["fileNew"]);
	addAction(p->scrActions["fileOpen"]);
	addAction(p->scrActions["fileSave"]);
	addAction(p->scrActions["fileClose"]);
	addAction(p->scrActions["filePrint"]);
	addAction(p->scrActions["toolsPreflightVerifier"]);
	addAction(p->scrActions["fileExportAsPDF"]);
}

void FileToolBar::rebuildRecentFileMenu()
{
	QToolButton* tb = dynamic_cast<QToolButton*>(widgetForAction(parent->scrActions["fileOpen"]));
	QMenu *m=new QMenu(this);
	ScrAction *recentFileAction;
	foreach (recentFileAction, parent->scrRecentFileActions)
		m->addAction(recentFileAction);
	tb->setMenu(m);
//	tb->setPopupMode(QToolButton::MenuButtonPopup);
	tb->setPopupMode(QToolButton::DelayedPopup);
}
