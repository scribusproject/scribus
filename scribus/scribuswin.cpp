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
#include <QCloseEvent>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "commonstrings.h"
#include "fileloader.h"
#include "pageselector.h"
#include "scribus.h"
#include "scribuswin.h"
#include "story.h"
#include "util.h"

ScribusWin::ScribusWin(QWidget* parent, ScribusDoc* doc) : QMainWindow(parent, "", Qt::WDestructiveClose)
{
	setIcon(loadIcon("AppIcon2.png"));
	m_Doc = doc;
	m_masterPagesPalette = NULL;
	currentDir = QDir::currentDirPath();
}

void ScribusWin::setMainWindow(ScribusMainWindow *mw)
{
	m_MainWindow=mw;
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
	m_View->cmsToolbarButton->reparent(statusFrame, point);
	m_View->previewToolbarButton->reparent(statusFrame, point);
	m_View->visualMenu->reparent(statusFrame, point);
	statusFrameLayout->addWidget(m_View->unitSwitcher);
	statusFrameLayout->addWidget(m_View->zoomSpinBox);
	statusFrameLayout->addWidget(m_View->zoomOutToolbarButton);
	statusFrameLayout->addWidget(m_View->zoomDefaultToolbarButton);
	statusFrameLayout->addWidget(m_View->zoomInToolbarButton);
	statusFrameLayout->addWidget(m_View->pageSelector);
	statusFrameLayout->addWidget(m_View->layerMenu);
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	statusFrameLayout->addItem( spacer );
	statusFrameLayout->addWidget(m_View->cmsToolbarButton);
	statusFrameLayout->addWidget(m_View->previewToolbarButton);
	statusFrameLayout->addWidget(m_View->visualMenu);
	statusBar()->addWidget(statusFrame, 4, true);
	currentDir = QDir::currentDirPath();
}

void ScribusWin::slotAutoSave()
{
	if ((m_Doc->hasName) && (m_Doc->isModified()))
	{
		moveFile(m_Doc->DocName, m_Doc->DocName+".bak");
		QFileInfo fi(m_Doc->DocName);
		QDir::setCurrent(fi.dirPath(true));
		FileLoader fl(m_Doc->DocName);
		if (fl.SaveFile(m_Doc->DocName, m_Doc, 0))
		{
			m_Doc->setModified(false);
			setCaption(QDir::convertSeparators(m_Doc->DocName));
			qApp->processEvents();
			emit AutoSaved();
		}
	}
}

void ScribusWin::closeEvent(QCloseEvent *ce)
{
	if (m_Doc->isModified() && (m_Doc->viewCount == 1))
	{
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		int exit = QMessageBox::information(m_MainWindow, CommonStrings::trWarning, tr("Document:")+" "+
											QDir::convertSeparators(m_Doc->DocName)+"\n"+
											tr("has been changed since the last save."),
											QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
											QMessageBox::Cancel);
		if (exit == QMessageBox::Cancel)
		{
			ce->ignore();
			return;
		}
		if (exit == QMessageBox::Save)
		{
			if (m_MainWindow->slotFileSave())
			{
				if (m_Doc==m_MainWindow->storyEditor->currentDocument())
					m_MainWindow->storyEditor->close();
			}
			else
			{
				ce->ignore();
				return;
			}
		}
	}
	m_MainWindow->DoFileClose();
	ce->accept();
}

void ScribusWin::resizeEvent(QResizeEvent *re)
{
	statusBar()->setSizeGripEnabled(!isMaximized());
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

void ScribusWin::windowActivationChange ( bool oldActive )
{
	if( isActiveWindow() )
		QDir::setCurrent( currentDir );
	else
		currentDir = QDir::currentDirPath();
	QMainWindow::windowActivationChange( oldActive );
}
