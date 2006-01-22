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
#include <qfileinfo.h>
#include <qdir.h>
#include "scribuswin.h"
#include "pageselector.h"
#include "scmessagebox.h"
#include "scribuswin.moc"
#include "scribusXml.h"
#include "scribus.h"
#include "story.h"
#include "util.h"
#include "commonstrings.h"



ScribusWin::ScribusWin(QWidget* parent, ScribusDoc* doc) : QMainWindow(parent, "", WDestructiveClose)
{
	setIcon(loadIcon("AppIcon2.png"));
	m_Doc = doc;
	m_masterPagesPalette = NULL;
}

void ScribusWin::setView(ScribusView* newView)
{
	m_View = newView;
	++m_Doc->viewCount;
	winIndex = ++m_Doc->viewID;
	QPoint point(0,0);
	statusFrame = new QFrame(this, "newDocFrame");
	statusFrameLayout = new QHBoxLayout( statusFrame, 0, 0, "statusFrame");
	m_View->unitSwitcher->reparent(statusFrame, point);
	m_View->layerMenu->reparent(statusFrame, point);
	m_View->zoomOutToolbarButton->reparent(statusFrame, point);
	m_View->zoomDefaultToolbarButton->reparent(statusFrame, point);
	m_View->zoomInToolbarButton->reparent(statusFrame, point);
	m_View->pageSelector->reparent(statusFrame, point);
	m_View->zoomSpinBox->reparent(statusFrame, point);
	statusFrameLayout->addWidget(m_View->unitSwitcher);
	statusFrameLayout->addWidget(m_View->zoomSpinBox);
	statusFrameLayout->addWidget(m_View->zoomOutToolbarButton);
	statusFrameLayout->addWidget(m_View->zoomDefaultToolbarButton);
	statusFrameLayout->addWidget(m_View->zoomInToolbarButton);
	statusFrameLayout->addWidget(m_View->pageSelector);
	statusFrameLayout->addWidget(m_View->layerMenu);
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	statusFrameLayout->addItem( spacer );
	statusBar()->addWidget(statusFrame, 4, true);
}

void ScribusWin::slotAutoSave()
{
	if ((m_Doc->hasName) && (m_Doc->isModified()))
	{
		moveFile(m_Doc->DocName, m_Doc->DocName+".bak");
		QFileInfo fi(m_Doc->DocName);
		QDir::setCurrent(fi.dirPath(true));
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (ss->WriteDoc(m_Doc->DocName, m_Doc, 0))
		{
			m_Doc->setModified(false);
			setCaption(m_Doc->DocName);
			qApp->processEvents();
			emit AutoSaved();
		}
		delete ss;
	}
}

void ScribusWin::closeEvent(QCloseEvent *ce)
{
	if (m_Doc->isModified() && (m_Doc->viewCount == 1))
	{
		int exit=ScMessageBox::information(ScMW, CommonStrings::trWarning,
		                                  tr("Document:")+" "+m_Doc->DocName+"\n"+ tr("has been changed since the last save."),
		                                  CommonStrings::tr_Save, tr("&Discard"), CommonStrings::tr_Cancel, 2, 2);
		if (exit==2)
			return;
		if (exit==0)
		{
			if (ScMW->slotFileSave())
			{
				if (m_Doc==ScMW->storyEditor->currentDocument())
					ScMW->storyEditor->close();
			}
			else
				return;
		}
	}
	ScMW->DoFileClose();
	ce->accept();
}

void ScribusWin::setMasterPagesPaletteShown(bool isShown) const
{
	if (m_masterPagesPalette==NULL)
		return;
	if (isShown)
		m_masterPagesPalette->show();
	else
		m_masterPagesPalette->hide();
}
