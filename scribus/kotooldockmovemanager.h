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
#ifndef KO_TOOLDOCK_MOVEMANAGER_H
#define KO_TOOLDOCK_MOVEMANAGER_H

#include <qobject.h>

#if defined Q_WS_X11 && !defined K_WS_QTONLY
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif
class QTimer;
class QWidget;


/**
 * The move manager handles moving tooldocks or other widgets that it
 * can manage, such as the Kivio stencil bar.
 */
class KoToolDockMoveManager: public QObject
{
	Q_OBJECT

public:

	KoToolDockMoveManager();
	~KoToolDockMoveManager();

	bool isWorking() const { return working; }

	void doXResize( QWidget*, bool mirror );
	void doYResize( QWidget*, bool mirror );
	void doXYResize( QWidget*, bool _mirrorX, bool _mirrorY );

	void doMove( QWidget* );
	void movePause( bool horizontal = true, bool vertical = true );
	void moveContinue();

	void setGeometry( const QRect& r );
	void setGeometry(int x, int y, int w, int h);

	QRect geometry();

	void resize( const QSize& s ) { setGeometry(xp, yp, s.width(), s.height()); }
	void resize(int rw, int rh) { setGeometry(xp, yp, rw, rh); }
	void move( int rx, int ry) { setGeometry(rx, ry, w, h); }
	void move( const QPoint& p ) {setGeometry(p.x(), p.y(), w, h); }

	int x()const { return xp; }
	int y() const { return yp; }
	int width()const { return w; }
	int height()const { return h; }

	bool isXMirror() const { return mirrorX; }
	bool isYMirror() const{ return mirrorY; }

	void stop ();
	QWidget* getWidget()const{ return widget; }

signals:

	void positionChanged();
	void sizeChanged();
	void fixPosition(int& x, int& y, int& w, int& h);
	void fixSize(int& x, int& y, int& w, int& h);

protected:

	void doResize( QWidget* );
	void setWidget( QWidget* );
	void drawRectangle (int x, int y, int w, int h);
	void paintProcess( bool onlyDeelete = true, int _x = 0, int _y = 0, int _w = 0, int _h = 0 );
	bool check(int& x, int& y, int& w, int& h, bool change = false);

private slots:

	void doMoveInternal();
	void doResizeInternal();

private:

	bool pauseMoveX;
	bool pauseMoveY;
	int xp, yp, w, h;
	int ox, oy, ow, oh;
	int orig_x, orig_y, orig_w, orig_h;
	bool noLast;
	bool working;

	QSize maxSize;
	QSize minSize;
	QWidget* widget;
	QTimer* timer;

	int rx, ry, sx, sy;
	int offX, offY;

	/* X-stuff */
#if defined Q_WS_X11 && !defined K_WS_QTONLY
	Window root;
	GC rootgc;
	int scr;
	XEvent ev;
#endif

	bool mirrorX;
	bool mirrorY;
	bool xOnly;
	bool yOnly;

	bool isDoMove;
	QRect rr;
	QPoint p;
};

#endif // KO_TOOLDOCK_MOVEMANAGER_H

