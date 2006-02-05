/***************************************************************************
 *   Copyright (C) 2006 by Riku Leino                                      *
 *   riku@scribus.info                                                     *
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

#include "sctoolbar.h"
#include "sctoolbar.moc"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include <qevent.h>
#include <qdockwindow.h>
#include <qmainwindow.h>
#include <qstring.h>

ScToolBar::ScToolBar(const QString& name, QMainWindow *parent, QDockWindow::Orientation o)
: QToolBar(tr(name), parent),
  m_name(QString("ToolBar-%1").arg(name)),
  m_prefs(PrefsManager::instance()->prefsFile->getContext(m_name))
{
	setCloseMode(QDockWindow::Never);
	if (m_prefs->contains("IsDocked"))
	{
		if (m_prefs->getBool("IsDocked", true)) {
			dock();
		} else {
			undock();
			setOrientation(o);
			move(QPoint(m_prefs->getInt("x", 50), m_prefs->getInt("y", 50)));
		}
	}

	if (m_prefs->contains("IsVisible")) 
	{
		if (m_prefs->getBool("IsVisible", true))
			show();
		else
			hide();
	}

	connect(this, SIGNAL(placeChanged(QDockWindow::Place)),
	        this, SLOT(slotPlaceChanged(QDockWindow::Place)));
	connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(slotVisibilityChanged(bool)));
}

void ScToolBar::slotPlaceChanged(QDockWindow::Place p)
{
	m_prefs->set("IsDocked", p == QDockWindow::InDock);
	m_prefs->set("x", x());
	m_prefs->set("y", y());
}

void ScToolBar::slotVisibilityChanged(bool visible)
{
	m_prefs->set("IsVisible", visible);
}

ScToolBar::~ScToolBar()
{
	
}
