/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribuswin.cpp  -  description
                             -------------------
    begin                : Mit Nov 6 2002
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

#include "scribuswin.h"

#include <QApplication>
#include <QDir>
#include <QMessageBox>

#include "commonstrings.h"
#include "iconmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"


ScribusWin::ScribusWin(QWidget* parent, ScribusDoc* doc) :
	QMainWindow(parent),
	m_subWindow(0),
	m_ScMW(0),
	m_View(0),
	m_winIndex(0)
{
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon2.png"));
	setAttribute(Qt::WA_DeleteOnClose);
	m_Doc = doc;
}

void ScribusWin::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW=mw;
}

void ScribusWin::setView(ScribusView* newView)
{
	m_View = newView;
	++m_Doc->viewCount;
	m_winIndex = ++m_Doc->viewID;
	setCentralWidget(newView);
	setStatusBar(0);
}

void ScribusWin::closeEvent(QCloseEvent *ce)
{
	activateWindow();
	m_ScMW->newActWin(getSubWin());
	if (m_Doc->isModified() && (m_Doc->viewCount == 1))
	{
		int exit = ScMessageBox::information(m_ScMW, CommonStrings::trWarning, tr("Document:")+" "+
											QDir::toNativeSeparators(m_Doc->DocName)+"\n"+
											tr("has been changed since the last save."),
											QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
											QMessageBox::Cancel,	// GUI default
											QMessageBox::Discard);	// batch default
		if (exit == QMessageBox::Cancel)
		{
			ce->ignore();
			return;
		}
		if (exit == QMessageBox::Save)
		{
			if (!m_ScMW->slotFileSave())
			{
				ce->ignore();
				return;
			}
		}
	}
	m_ScMW->DoFileClose();
	ce->accept();
}
