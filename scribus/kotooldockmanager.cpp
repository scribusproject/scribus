/*
 * This file is part of the KDE project
 * Copyright (C) 2000-2001 theKompany.com & Dave Marotti
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "kotooldockmanager.h"
#include "kotooldockbase.h"

#include <qapplication.h>
#include <qcursor.h>

KoToolDockManager::KoToolDockManager( QWidget* parent, const char* name )
		: QObject(parent,name)
{
	m_pView = parent;
	m_pView -> installEventFilter(this);
	connect(&timer,SIGNAL(timeout()),SLOT(slotTimeOut()));
	timer.start(1000);
}

KoToolDockManager::~KoToolDockManager()
{}

void KoToolDockManager::slotTimeOut()
{
	QPoint p = m_pView -> mapFromGlobal(QCursor::pos());
	for (KoToolDockBase* b = tools.first(); b; b = tools.next() )
	{
		b -> mouseStatus(b -> geometry().contains(p));
	}
}

void KoToolDockManager::addToolWindow( KoToolDockBase* t )
{
	tools.append(t);
	connect(t,SIGNAL(destroyed()),SLOT(removeToolWindow()));
}

void KoToolDockManager::removeToolWindow( KoToolDockBase* t )
{
	tools.removeRef(t);
}

void KoToolDockManager::removeToolWindow()
{
	removeToolWindow((KoToolDockBase*)sender());
}

bool KoToolDockManager::eventFilter( QObject* obj, QEvent* ev )
{
	if (obj == m_pView && ev -> type() == QEvent::Resize )
	{
		for (KoToolDockBase* b = tools.first(); b; b = tools.next() )
		{
			QRect g = b -> geometry();
			if (b -> snaps.get(KoToolDockRight) == m_pView)
			{
				if (b -> snaps.get(KoToolDockLeft) == m_pView)
				{
					g.setWidth(m_pView -> width());
				}
				else
				{
					g.moveTopLeft(QPoint(m_pView -> width()-b -> width(), g.y()));
				}
			}
			if (b -> snaps.get(KoToolDockBottom) == m_pView)
			{
				if (b -> snaps.get(KoToolDockTop) == m_pView)
				{
					g.setHeight(m_pView -> height());
				}
				else
				{
					g.moveTopLeft(QPoint(g.x(), m_pView -> height() - b -> height()));
				}
			}
			if (b -> geometry()!=g)
				b -> setGeometry(g);
		}
		return false;
	}

	return false;
}

KoToolDockBase* KoToolDockManager::createSimpleToolDock( QWidget* view, const char* name )
{
	KoToolDockBase* b = new KoToolDockBase(m_pView, name);
	b -> setView(view);
	addToolWindow(b);
	return b;
}

KoTabbedToolDock* KoToolDockManager::createTabbedToolDock( const char* name )
{
	KoTabbedToolDock* b = new KoTabbedToolDock(m_pView, name);
	addToolWindow(b);
	return b;
}



#include "kotooldockmanager.moc"
