/*
 * This file is part of the KDE project
  Copyright (C) 2000-2001 theKompany.com & Dave Marotti
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
#include "kotooldockmovemanager.h"

#include <qcursor.h>
#include <qapplication.h>
#include <qwindowdefs.h>
#include <qtimer.h>
#include <qrect.h>
#include <qframe.h>
#include <qpainter.h>

KoToolDockMoveManager::KoToolDockMoveManager()
		: QObject()
{
#if defined Q_WS_X11 && !defined K_WS_QTONLY
	XGCValues gv;
#endif

	working=false;
	noLast=true;

#if defined Q_WS_X11 && !defined K_WS_QTONLY
	scr = qt_xscreen();
	root = qt_xrootwin();

	gv.function = GXxor;
	gv.line_width = 2;
	gv.foreground = WhitePixel(qt_xdisplay(), scr)^BlackPixel(qt_xdisplay(), scr);
	gv.subwindow_mode = IncludeInferiors;
	long mask = GCForeground | GCFunction | GCLineWidth | GCSubwindowMode;
	rootgc = XCreateGC(qt_xdisplay(), qt_xrootwin(), mask, &gv);
#endif

	timer = new QTimer(this);
}

KoToolDockMoveManager::~KoToolDockMoveManager()
{
	stop();
	delete timer;
}

void KoToolDockMoveManager::doMove( QWidget* _w )
{
	if (working)
		return;

	working=true;
	isDoMove = true;
	mirrorX=false;
	mirrorY=false;

	setWidget(_w);

	offX = rx - p.x();
	offY = ry - p.y();

	orig_x = p.x();
	orig_y = p.y();
	orig_w = w;
	orig_h = h;

	QApplication::setOverrideCursor(QCursor(sizeAllCursor));

	movePause(false,false);

	drawRectangle(xp, yp, w, h);

	timer->disconnect();
	connect(timer,SIGNAL(timeout()),SLOT(doMoveInternal()));
	timer->start(0);
}

void KoToolDockMoveManager::doMoveInternal()
{
	if ( !working )
		return;

	if (!pauseMoveX)
		rx = QCursor::pos().x();

	if (!pauseMoveY)
		ry = QCursor::pos().y();

	xp = rx - offX;
	yp = ry - offY;

	emit positionChanged();

	if (check(xp, yp, w, h))
	{
		paintProcess(false,xp, yp, w, h);

#if defined Q_WS_X11 && !defined K_WS_QTONLY
		XFlush(qt_xdisplay());
		XSync(qt_xdisplay(),false);
#endif

	}
}

void KoToolDockMoveManager::stop()
{
	if (!working)
		return;

	timer->stop();
	QApplication::restoreOverrideCursor();

	paintProcess();
#if defined Q_WS_X11 && !defined K_WS_QTONLY
	XFlush(qt_xdisplay());
#endif

	working = false;
}

void KoToolDockMoveManager::setGeometry( const QRect& r )
{
	setGeometry(r.x(),r.y(),r.width(),r.height());
}

void KoToolDockMoveManager::setGeometry(int _x, int _y, int _w, int _h)
{
	xp=_x;
	yp=_y;
	w=_w;
	h=_h;

	check(_x, _y, _w, _h, true);
	paintProcess(false,_x, _y, _w, _h);

#if defined Q_WS_X11 && !defined K_WS_QTONLY
	XFlush(qt_xdisplay());
	XSync(qt_xdisplay(),false);
#endif
}

void KoToolDockMoveManager::drawRectangle( int _x, int _y, int _w, int _h)
{
	if (!noLast)
		return;

	ox = _x;
	oy = _y;
	ow = _w;
	oh = _h;

#if defined Q_WS_X11 && !defined K_WS_QTONLY
	XDrawRectangle(qt_xdisplay(), root, rootgc, _x, _y, _w, _h);
#endif
	noLast = false;
}

void KoToolDockMoveManager::paintProcess( bool onlyDelete, int _x, int _y, int _w, int _h )
{
	if (noLast && onlyDelete)
		return;

	if ( ox == _x && oy == _y && ow ==_w && oh == _h )
		return;

#if defined Q_WS_X11 && !defined K_WS_QTONLY
	XDrawRectangle(qt_xdisplay(), root, rootgc, ox, oy, ow, oh);
#endif
	noLast = true;

	drawRectangle(_x,_y,_w,_h);
}

void KoToolDockMoveManager::movePause( bool horizontal, bool vertical )
{
	pauseMoveX = horizontal;
	pauseMoveY = vertical;
}

void KoToolDockMoveManager::moveContinue()
{
	pauseMoveX = false;
	pauseMoveY = false;
}

void KoToolDockMoveManager::doXResize( QWidget* w, bool mirror )
{
	if (working)
		return;

	mirrorX = mirror;
	mirrorY = false;

	yOnly = false;
	xOnly = true;

	doResize(w);
}

void KoToolDockMoveManager::doYResize( QWidget* w, bool mirror )
{
	if (working)
		return;

	mirrorX = false;
	mirrorY = mirror;

	yOnly = true;
	xOnly = false;

	doResize(w);
}

void KoToolDockMoveManager::doXYResize( QWidget* w, bool _mirrorX, bool _mirrorY )
{
	if (working)
		return;

	mirrorX = _mirrorX;
	mirrorY = _mirrorY;

	yOnly = false;
	xOnly = false;

	doResize(w);
}

void KoToolDockMoveManager::doResizeInternal()
{
	if (!yOnly)
		rx = QCursor::pos().x();

	if (!xOnly)
		ry = QCursor::pos().y();

	int dx = rx - sx;
	int dy = ry - sy;

	if ( mirrorX )
	{
		w = rr.width() - dx;
		xp = rr.x() + dx;
	}
	else
	{
		w = rr.width() + dx;
	}

	if ( mirrorY )
	{
		h = rr.height() - dy;
		yp = rr.y() + dy;
	}
	else
	{
		h = rr.height() + dy;
	}

	emit sizeChanged();

	if (check(xp, yp, w, h))
	{
		paintProcess(false,xp, yp, w, h);

#if defined Q_WS_X11 && !defined K_WS_QTONLY
		XFlush(qt_xdisplay());
		XSync(qt_xdisplay(),false);
#endif

	}
}

void KoToolDockMoveManager::setWidget( QWidget* _w )
{
	widget = _w;
	minSize = widget->minimumSize();
	maxSize = widget->maximumSize();

	rr = QRect(widget->mapToGlobal(QPoint(0,0)),widget->size());
	p =  rr.topLeft();

	xp = rr.x();
	yp = rr.y();
	w  = rr.width();
	h  = rr.height();

	rx = QCursor::pos().x();
	ry = QCursor::pos().y();
}

void KoToolDockMoveManager::doResize( QWidget* _w )
{
	setWidget(_w);

	working=true;
	isDoMove = false;

	QPoint curPos = QCursor::pos();
	sx = curPos.x();
	sy = curPos.y();

	offX = sx - p.x();
	offY = sy - p.y();

	drawRectangle(xp, yp, w, h);

	timer->disconnect();
	connect(timer,SIGNAL(timeout()),SLOT(doResizeInternal()));
	timer->start(0);
}

bool KoToolDockMoveManager::check(int& x, int& y, int& w, int& h, bool change)
{

	int w1 = QMIN(QMAX(minSize.width(), w), maxSize.width());
	int h1 = QMIN(QMAX(minSize.height(), h), maxSize.height());

	bool f1 = (w1-w)+(h1-h) == 0;

	if (change)
	{
		if (mirrorX)
			x += w - w1;
		w = w1;
		if (mirrorY)
			y += h - h1;
		h = h1;
	}

	int x0 = x;
	int y0 = y;
	int w0 = w;
	int h0 = h;

	if (isDoMove)
		emit fixPosition(x0,y0,w0,h0);
	else
		emit fixSize(x0,y0,w0,h0);

	bool f2 = (x0==x)&&(y0==y)&&(w0==w)&&(h0==h);

	if (change)
	{
		x = x0;
		y = y0;
		w = w0;
		h = h0;
	}

	return f1&&f2;
}

QRect KoToolDockMoveManager::geometry()
{
	int x0 = xp;
	int y0 = yp;
	int w0 = w;
	int h0 = h;
	check(x0,y0,w0,h0,true);

	return QRect(x0,y0,w0,h0);
}
#include "kotooldockmovemanager.moc"
