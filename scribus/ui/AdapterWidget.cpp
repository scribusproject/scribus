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
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

#include "scconfig.h"

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <iostream>
#include <iomanip>
#include <fstream>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <cstdlib>
#include <cassert>
#include <string>
#include "util_icon.h"
#include "AdapterWidget.h"

AdapterWidget::AdapterWidget ( QWidget * parent, const char * name, const QGLWidget * shareWidget, Qt::WFlags f ) : QGLWidget ( parent, shareWidget, f )
{
	_gw = new osgViewer::GraphicsWindowEmbedded ( 0,0,width(),height() );
	setFocusPolicy ( Qt::ClickFocus );
	button = 0;
}

void AdapterWidget::resizeGL ( int width, int height )
{
	_gw->getEventQueue()->windowResize ( 0, 0, width, height );
	_gw->resized ( 0,0,width,height );
}

void AdapterWidget::keyPressEvent ( QKeyEvent* event )
{
	_gw->getEventQueue()->keyPress ( ( osgGA::GUIEventAdapter::KeySymbol ) * ( event->text().toAscii().data() ) );
}

void AdapterWidget::keyReleaseEvent ( QKeyEvent* event )
{
	_gw->getEventQueue()->keyRelease ( ( osgGA::GUIEventAdapter::KeySymbol ) * ( event->text().toAscii().data() ) );
}

void AdapterWidget::mousePressEvent ( QMouseEvent* event )
{
	switch ( event->button() )
	{
		case Qt::LeftButton:
			qApp->changeOverrideCursor(QCursor(Qt::OpenHandCursor));
			button = 1;
			break;
		case Qt::MidButton:
			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
			button = 2;
			break;
		case Qt::RightButton:
			qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			button = 3;
			break;
		case Qt::NoButton:
			button = 0;
			break;
		default:
			button = 0;
			break;
	}
	_gw->getEventQueue()->mouseButtonPress ( event->x(), event->y(), button );
}

void AdapterWidget::mouseReleaseEvent ( QMouseEvent* event )
{
	switch ( event->button() )
	{
		case ( Qt::LeftButton ) : button = 1; break;
		case ( Qt::MidButton ) : button = 2; break;
		case ( Qt::RightButton ) : button = 3; break;
		case ( Qt::NoButton ) : button = 0; break;
		default: button = 0; break;
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	_gw->getEventQueue()->mouseButtonRelease ( event->x(), event->y(), button );
}

void AdapterWidget::mouseMoveEvent ( QMouseEvent* event )
{
	_gw->getEventQueue()->mouseMotion ( event->x(), event->y() );
	emit mouseMoved();
}
