/* This file is part of the KDE project
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
#ifndef KO_TOOLDOCKBASE_H
#define KO_TOOLDOCKBASE_H

#include <qwidget.h>
#include <qintdict.h>
#include <qframe.h>
#include <qtimer.h>
class QGridLayout;
class QTabWidget;

class KoToolDockMoveManager;

enum KoToolDockPosition {
    KoToolDockLeft,
    KoToolDockRight,
    KoToolDockTop,
    KoToolDockBottom,
    KoToolDockCenter
};

class KoToolDockBase;
class KoToolDockManager;

/**
 * Class that manages the borders of the tooldock palette window.
 */
class KoToolDockBaseBorder
			: public QWidget
{

	Q_OBJECT

public:
	enum Position { Left, Right, Top, Bottom, TopLeft, TopRight, BottomLeft, BottomRight };

	KoToolDockBaseBorder( Position, KoToolDockBase* parent, const char* name = 0 );
	~KoToolDockBaseBorder();

	Position position() const { return pos; }

signals:
	void resizeStart();
	void resizeStop();

protected:
	void paintEvent( QPaintEvent* );
	void mousePressEvent( QMouseEvent* );
	void mouseReleaseEvent( QMouseEvent* );

private:
	Position pos;
};

/******************************************************************************/

/**
 * A toggle button that goes into the titlebar of the tooldock palette window.
 */
class KoToolDockButton : public QFrame
{
	Q_OBJECT

public:
	KoToolDockButton( QWidget* parent, const char* name = 0 );
	~KoToolDockButton();

	void setPixmap( const QPixmap& );
	void setToggled( bool );
	void setDown( bool );

signals:
	void clicked();
	void toggled(bool);

protected:
	void paintEvent( QPaintEvent* );
	void mousePressEvent( QMouseEvent* );
	void mouseReleaseEvent( QMouseEvent* );
	void enterEvent( QEvent* );
	void leaveEvent( QEvent* );

private:
	QPixmap* pixmap;
	bool tg;
	bool down;
	bool isin;
	bool mdown;
};


/******************************************************************************/

/**
 * The window title for the tooldock palette window.
 */
class KoToolDockBaseCaption : public QWidget
{

	Q_OBJECT

public:

	KoToolDockBaseCaption( KoToolDockPosition, KoToolDockBase* parent, const char* name = 0 );
	~KoToolDockBaseCaption();

signals:
	void moveStart();
	void moveStop();

	void doClose();
	void doStick( bool );
	void doLock( bool );

public slots:

	void stick( bool );
	void lock( bool );

protected slots:

	void slotClose();
	void slotStick( bool );
	void slotLock( bool );

protected:

	void paintEvent( QPaintEvent* );
	void mousePressEvent( QMouseEvent* );
	void mouseMoveEvent( QMouseEvent* );
	void mouseReleaseEvent( QMouseEvent* );

private:

	bool m_bPressed;
	bool m_bMove;
	QPoint m_ppoint;
	KoToolDockPosition position;
	KoToolDockButton* closeButton;
	KoToolDockButton* stickButton;
	KoToolDockButton* lockButton;
};

/******************************************************************************/

/**
 * XXX
 */
class KoToolDockBaseCaptionManager : public QObject
{

	Q_OBJECT

public:

	KoToolDockBaseCaptionManager( KoToolDockBase* parent, const char* name = 0 );
	~KoToolDockBaseCaptionManager();

	void setView( KoToolDockPosition );
	KoToolDockBaseCaption* captionWidget( KoToolDockPosition );
	int captionHeight() const;

signals:
	void doClose();
	void doStick(bool);
	void doLock( bool );

protected slots:

	void slotClose();
	void slotStick(bool);
	void slotLock( bool );

private:

	KoToolDockBaseCaption* m_pLeft;
	KoToolDockBaseCaption* m_pRight;
	KoToolDockBaseCaption* m_pTop;
	KoToolDockBaseCaption* m_pBottom;
};

/******************************************************************************/

/**
 * XXX
 */
class KoToolDockSnap
{
public:
	void clear();
	void set(KoToolDockPosition,QWidget*);
	QWidget* get(KoToolDockPosition);

private:
	QIntDict<QWidget> snaps;
};


/******************************************************************************/


/**
 * Base class for individual 'floating palette' type
 * dialogs. These dialogs do not take focus from the
 * main view and can automatically hide into the borders
 * of the application.
 *
 * TODO:
 *   * Make titlebar the KDE standard titlebar color (or are the grips
 *     really better?).
 *   * Add button to enable/disable border hiding.
 *   * Make it possible to drag dockers outside the parent view.
 */
class KoToolDockBase : public QWidget
{

	Q_OBJECT
	friend class KoToolDockManager;

public:

	virtual ~KoToolDockBase();

	void activate();
	void restore();
protected:

	KoToolDockBase( QWidget* parent, const char* name = 0 );


public slots:

	void show();
	void hide();
	void makeVisible(bool);

signals:

	void visibleChange(bool);

protected slots:

	void setView( QWidget* );
	void beginResize();
	void stopResize();

	void beginMove();
	void stopMove();

	void positionChanged();
	void sizeChanged();

	void fixPosition(int& x, int& y, int& w, int& h);
	void fixSize(int& x, int& y, int& w, int& h);

	void slotStick(bool);
	bool isStick() const { return stick; }

	void slotLock( bool );
	bool isLocked() const { return lock; }

protected:

void enterEvent( QEvent* );
	void mousePressEvent( QMouseEvent* );
	void resizeEvent( QResizeEvent* );
	void paintEvent( QPaintEvent* );

	void updateCaption();
	KoToolDockPosition getCaptionPos( bool* = 0L );
	void mouseStatus(bool);

protected slots:

	void slotHideTimeOut();
	void slotHideProcessTimeOut();
	void hideProcessStop();
	void showProcessStop();

private:
	bool stick;
	bool lock;
QWidget* m_pView;
QGridLayout* m_pBaseLayout;

KoToolDockBaseBorder* m_pBorderLeft;
KoToolDockBaseBorder* m_pBorderRight;
KoToolDockBaseBorder* m_pBorderTop;
KoToolDockBaseBorder* m_pBorderBottom;

KoToolDockBaseBorder* m_pBorderTopLeft;
KoToolDockBaseBorder* m_pBorderTopRight;
KoToolDockBaseBorder* m_pBorderBottomRight;
KoToolDockBaseBorder* m_pBorderBottomLeft;

KoToolDockBaseCaptionManager* m_pCaptionManager;

KoToolDockMoveManager* mrManager;
KoToolDockSnap snaps;

QGridLayout* m_pLayout;

int hdx;
QPixmap* hpixmap;
QSize hminsize;
QSize hmaxsize;
QSize hsize;
QTimer hideTimer;
QTimer hideProcessTimer;
KoToolDockPosition hideDirection;
enum hstatus { hnone, hstarthide, hprocesshide, hdonehide, hprocessshow };
hstatus hStatus;
};

/*******************************************************************************/

class KoTabbedToolDock : public KoToolDockBase
{

Q_OBJECT

public:

KoTabbedToolDock ( QWidget* parent = 0, const char* name = 0 );
	~KoTabbedToolDock();

	void plug(QWidget *w);
	void unplug(QWidget *w);
	void showPage(QWidget *w);

private:

	QTabWidget *m_tabwidget;

};

#endif
