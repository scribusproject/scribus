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

#include <qbitmap.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qcursor.h>
#include <qdrawutil.h>
#include <qapplication.h>
#include <qobjectlist.h>
#include <qtabwidget.h>

#include "kotooldockbase.h"
#include "kotooldockmanager.h"
#include "kotooldockmovemanager.h"

/******************************************************************************/

KoToolDockBaseBorder::KoToolDockBaseBorder( Position position, KoToolDockBase* parent, const char* name )
		: QWidget(parent,name)
{
	connect(this,SIGNAL(resizeStart()),parent,SLOT(beginResize()));
	connect(this,SIGNAL(resizeStop()),parent,SLOT(stopResize()));

	pos = position;
	switch (pos)
	{
	case Left:
	case Right:
		setCursor(SizeHorCursor);
		setFixedWidth(3);
		break;
	case Top:
	case Bottom:
		setCursor(SizeVerCursor);
		setFixedHeight(3);
		break;

	case TopLeft:
	case BottomRight:
		setCursor(SizeFDiagCursor);
		setFixedSize(3,3);
		break;
	case TopRight:
	case BottomLeft:
		setCursor(SizeBDiagCursor);
		setFixedSize(3,3);
		break;
	}
}

void KoToolDockBaseBorder::paintEvent( QPaintEvent* /*ev*/ )
{
	if (!parent() -> parent())
		return;

	QPainter p(this,this);
	int w = width()-1;
	int h = height()-1;
	switch (pos)
	{
	case Left:
		p.setPen(white);
		p.drawLine(0,0,0,h);
		break;
	case Right:
		p.setPen("gray30");
		p.drawLine(w,0,w,h);
		break;
	case Top:
		p.setPen(white);
		p.drawLine(0,0,w,0);
		break;
	case Bottom:
		p.setPen("gray30");
		p.drawLine(0,h,w,h);
		break;

	case TopLeft:
		p.setPen(white);
		p.drawLine(0,2,2,0);
		break;
	case BottomRight:
		p.setPen("gray30");
		p.drawLine(w,h-2,w-2,h);
		break;
	case TopRight:
		p.setPen("gray30");
		p.drawLine(w-2,0,w,2);
		break;
	case BottomLeft:
		p.setPen("gray30");
		p.drawLine(0,h-2,2,h);
		break;
	}
	p.end();
}

void KoToolDockBaseBorder::mousePressEvent( QMouseEvent* /*ev*/ )
{
	((KoToolDockBase*)parentWidget()) -> activate();
	emit resizeStart();
}

void KoToolDockBaseBorder::mouseReleaseEvent( QMouseEvent* /*ev*/ )
{
	emit resizeStop();
}

KoToolDockBaseBorder::~KoToolDockBaseBorder()
{}

/******************************************************************************/

KoToolDockBaseCaptionManager::KoToolDockBaseCaptionManager( KoToolDockBase* parent, const char* name )
		: QObject(parent,name)
{
	m_pLeft = new KoToolDockBaseCaption(KoToolDockLeft,parent);
	m_pRight = new KoToolDockBaseCaption(KoToolDockRight,parent);
	m_pTop = new KoToolDockBaseCaption(KoToolDockTop,parent);
	m_pBottom = new KoToolDockBaseCaption(KoToolDockBottom,parent);

	connect(m_pLeft,SIGNAL(doClose()),SLOT(slotClose()));
	connect(m_pRight,SIGNAL(doClose()),SLOT(slotClose()));
	connect(m_pTop,SIGNAL(doClose()),SLOT(slotClose()));
	connect(m_pBottom,SIGNAL(doClose()),SLOT(slotClose()));

	connect(m_pLeft,SIGNAL(doStick(bool)),SLOT(slotStick(bool)));
	connect(m_pRight,SIGNAL(doStick(bool)),SLOT(slotStick(bool)));
	connect(m_pTop,SIGNAL(doStick(bool)),SLOT(slotStick(bool)));
	connect(m_pBottom,SIGNAL(doStick(bool)),SLOT(slotStick(bool)));

	connect(m_pLeft,SIGNAL(doLock(bool)),SLOT(slotLock(bool)));
	connect(m_pRight,SIGNAL(doLock(bool)),SLOT(slotLock(bool)));
	connect(m_pTop,SIGNAL(doLock(bool)),SLOT(slotLock(bool)));
	connect(m_pBottom,SIGNAL(doLock(bool)),SLOT(slotLock(bool)));


	m_pLeft -> hide();
	m_pRight -> hide();
	m_pTop -> hide();
	m_pBottom -> hide();
}

int KoToolDockBaseCaptionManager::captionHeight() const
{
	return m_pTop -> height();
}

KoToolDockBaseCaptionManager::~KoToolDockBaseCaptionManager()
{}

void KoToolDockBaseCaptionManager::slotClose()
{
	emit doClose();
}

void KoToolDockBaseCaptionManager::slotStick(bool f)
{
	emit doStick(f);
	m_pLeft -> stick(f);
	m_pRight -> stick(f);
	m_pTop -> stick(f);
	m_pBottom -> stick(f);
}

void KoToolDockBaseCaptionManager::slotLock( bool f )
{
	emit doLock( f );
	m_pLeft -> lock(f);
	m_pRight -> lock(f);
	m_pTop -> lock(f);
	m_pBottom -> lock(f);
}

KoToolDockBaseCaption* KoToolDockBaseCaptionManager::captionWidget( KoToolDockPosition pos )
{
	switch (pos)
	{
	case KoToolDockLeft:
		return m_pLeft;
		break;
	case KoToolDockRight:
		return m_pRight;
		break;
	case KoToolDockTop:
		return m_pTop;
		break;
	case KoToolDockBottom:
		return m_pBottom;
		break;
	default:
		break;
	}
	return 0L;
}

void KoToolDockBaseCaptionManager::setView( KoToolDockPosition pos )
{
	m_pLeft -> hide();
	m_pRight -> hide();
	m_pTop -> hide();
	m_pBottom -> hide();

	switch (pos)
	{
	case KoToolDockLeft:
		//        if(!kapp -> reverseLayout())
		//		 {
		m_pLeft -> show();
		//        } else {
		//           m_pRight -> show();
		//      }
		break;
	case KoToolDockRight:
		//        if(!kapp -> reverseLayout())
		//		 {
		m_pRight -> show();
		//      } else {
		//       m_pLeft -> show();
		//    }
		break;
	case KoToolDockTop:
		m_pTop -> show();
		break;
	case KoToolDockBottom:
		m_pBottom -> show();
		break;
	default:
		break;
	}
}

/******************************************************************************/

KoToolDockBaseCaption::KoToolDockBaseCaption( KoToolDockPosition pos, KoToolDockBase* parent, const char* name )
		: QWidget(parent, name)
{
	setCursor(ArrowCursor);

	QFont f(font());
	f.setPointSize(8);
	f.setBold(true);
	setFont(f);

	m_bPressed = false;
	m_bMove = false;
	position = pos;

	connect(this,SIGNAL(moveStart()),parent,SLOT(beginMove()));
	connect(this,SIGNAL(moveStop()),parent,SLOT(stopMove()));

	stickButton = new KoToolDockButton( this );
	closeButton = new KoToolDockButton( this );
	//     lockButton = new KoToolDockButton( this );

	connect( closeButton,SIGNAL(clicked()),SLOT(slotClose()));
	connect( stickButton,SIGNAL(toggled(bool)),SLOT(slotStick(bool)));
	//     connect( lockButton,  SIGNAL( toggled( bool ) ),  SLOT( slotLock( bool ) ) );

	stickButton -> setToggled(true);
	//     lockButton -> setToggled( true );

	const char* close_xpm[] =
	    {
	        "5 5 2 1",
	        "# c black",
	        ". c None",
	        "#...#",
	        ".#.#.",
	        "..#..",
	        ".#.#.",
	        "#...#"
	    };

	const char* stick_xpm[] =
	    {
	        "8 7 2 1",
	        "# c black",
	        ". c None",
	        "...#....",
	        "...##.##",
	        "...#.#.#",
	        "####...#",
	        "...#.#.#",
	        "...##.##",
	        "...#...."
	    };

	const char* lock_xmp[] =
	    {
	        "7 8 2 1",
	        "# c black",
	        ". c None",
	        "..###..",
	        "..#.#..",
	        ".#####.",
	        ".##.##.",
	        ".#...#.",
	        ".##.##.",
	        ".#####.",
	        "..###.."
	    };

	stickButton -> setPixmap( stick_xpm );
	closeButton -> setPixmap( close_xpm );
	//     lockButton -> setPixmap( lock_xmp );

	if ( pos == KoToolDockTop || pos == KoToolDockBottom )
	{
		setFixedHeight(14);
		setMinimumWidth(100);
		QHBoxLayout* layout = new QHBoxLayout(this,1,2);
		layout -> addStretch(1);
		//layout -> addWidget( lockButton );
		layout -> addWidget( stickButton );
		layout -> addWidget( closeButton );

	}
	else
	{
		setFixedWidth(14);
		setMinimumHeight(100);
		QVBoxLayout* layout = new QVBoxLayout(this,1,2);
		if ( pos == KoToolDockLeft )
		{
			//layout -> addWidget( lockButton );
			layout -> addWidget(closeButton);
			layout -> addWidget(stickButton);
			layout -> addStretch(1);
		}
		else
		{
			layout -> addStretch(1);
			//layout -> addWidget( lockButton );
			layout -> addWidget(stickButton);
			layout -> addWidget(closeButton);
		}
	}

}

KoToolDockBaseCaption::~KoToolDockBaseCaption()
{}

void KoToolDockBaseCaption::slotClose()
{
	emit doClose();
}

void KoToolDockBaseCaption::slotStick(bool f)
{
	emit doStick(f);
}

void KoToolDockBaseCaption::slotLock( bool f )
{
	emit doLock( f );
}

void KoToolDockBaseCaption::stick(bool f)
{
	stickButton -> setDown(f);
}

void KoToolDockBaseCaption::lock( bool f )
{
	lockButton -> setDown( f );
}

void KoToolDockBaseCaption::paintEvent( QPaintEvent* )
{
	QPainter p(this,this);

	int w;
	int h;
	bool f = false;
	int dl = 1;

	if ( position == KoToolDockTop || position == KoToolDockBottom )
	{
		w = width();
		h = height();
	}
	else
	{
		w = height();
		h = width();
		if ( position == KoToolDockLeft )
		{
			p.rotate(-90);
			p.translate(-w,0);
		}
		else
		{
			p.rotate(90);
			p.translate(0,-h);
			f = true;
			dl++;
		}
	}

	QString t(parentWidget() -> caption());
	int tw = w-37-3;
	int textw = p.fontMetrics().width(t);
	int lw = 10 + QMAX(0,tw-textw);

	p.drawText(3, 0, w-40, h, AlignLeft | AlignVCenter, t);

	QPoint p1(w-lw-25, (h-5)/2+dl);
	QPoint p2(p1);
	p1 += QPoint(lw-5,0);

	qDrawShadeLine( &p, p1, p2, colorGroup(), f, 1, 0 );
	p1 += QPoint(0,3);
	p2 += QPoint(0,3);
	qDrawShadeLine( &p, p1, p2, colorGroup(), f, 1, 0 );

	p.end();
}

void KoToolDockBaseCaption::mouseMoveEvent( QMouseEvent* ev )
{
	if (m_bPressed)
	{
		QPoint p = m_ppoint;
		p -= ev -> pos();
		if (p.manhattanLength() > 2 && !m_bMove )
		{
			m_bMove = true;
			emit moveStart();
		}
	}
}

void KoToolDockBaseCaption::mousePressEvent( QMouseEvent* ev )
{
	((KoToolDockBase*)parentWidget()) -> activate();
	m_bPressed = true;
	m_ppoint = ev -> pos();
}

void KoToolDockBaseCaption::mouseReleaseEvent( QMouseEvent* )
{
	m_bPressed = false;
	if (m_bMove)
	{
		m_bMove = false;
		emit moveStop();
	}
}



/******************************************************************************/

void KoToolDockSnap::clear()
{
	snaps.clear();
}

void KoToolDockSnap::set( KoToolDockPosition pos, QWidget* widget )
{
	snaps.replace(pos,widget);
}

QWidget* KoToolDockSnap::get( KoToolDockPosition pos )
{
	return snaps[pos];
}


/******************************************************************************/

KoToolDockButton::KoToolDockButton( QWidget* parent, const char* name )
		: QFrame(parent,name)
{
	setFixedSize(12,12);
	tg = false;
	down = false;
	isin = false;
	mdown = false;
	pixmap = 0L;
}

KoToolDockButton::~KoToolDockButton()
{
	if (pixmap)
		delete pixmap;
}

void KoToolDockButton::paintEvent( QPaintEvent* ev )
{
	QFrame::paintEvent(ev);
	if (!pixmap)
		return;

	QPainter p(this,this);
	p.drawPixmap((width()-pixmap -> width())/2,(height()-pixmap -> height())/2,*pixmap);
	p.end();
}

void KoToolDockButton::mousePressEvent( QMouseEvent* )
{
	mdown = true;
	setFrameStyle(Panel|Sunken);
}

void KoToolDockButton::mouseReleaseEvent( QMouseEvent* )
{
	mdown = false;
	if (tg)
	{
		if (isin)
		{
			setDown(!down);
			emit toggled(down);
		}
		else
		{
			setFrameStyle( NoFrame );
		}
	}
	else
	{
		setFrameStyle( isin ? Panel|Raised : NoFrame );
	}

	if (isin)
		emit clicked();
}

void KoToolDockButton::enterEvent( QEvent* )
{
	isin = true;
	if (!down && !mdown)
		setFrameStyle(Panel|Raised);
}

void KoToolDockButton::leaveEvent( QEvent* )
{
	isin = false;
	if (!down && !mdown)
		setFrameStyle(NoFrame);
}

void KoToolDockButton::setToggled( bool f )
{
	tg = f;
}

void KoToolDockButton::setDown( bool f )
{
	down = f;
	setFrameStyle(down?Panel|Sunken:NoFrame);
}

void KoToolDockButton::setPixmap( const QPixmap& p )
{
	pixmap = p.isNull() ? 0L : new QPixmap(p);
	repaint();
}

/***************************************************************************/

KoToolDockBase::KoToolDockBase( QWidget* parent, const char* name )
		: QWidget(parent,name,  WStyle_Tool | WStyle_StaysOnTop | WStyle_Customize | WResizeNoErase | WRepaintNoErase)
{
//	QFont f;
//	f.setPointSize(8);
//	setFont(f);

	stick = false;
	lock = true;
	hStatus = hnone;

	connect(&hideTimer,SIGNAL(timeout()),SLOT(slotHideTimeOut()));
	connect(&hideProcessTimer,SIGNAL(timeout()),SLOT(slotHideProcessTimeOut()));

	mrManager = new KoToolDockMoveManager;
	m_pView = parent;

	connect(mrManager, SIGNAL(sizeChanged()),SLOT(sizeChanged()));
	connect(mrManager, SIGNAL(positionChanged()),SLOT(positionChanged()));
	connect(mrManager, SIGNAL(fixPosition(int&, int&, int&, int&)), SLOT(fixPosition(int&, int&, int&, int&)));

	connect(mrManager, SIGNAL(fixSize(int&, int&, int&, int&)), SLOT(fixSize(int&, int&, int&, int&)));

	m_pLayout = new QGridLayout(this,5,5,0,0);
	m_pLayout -> setResizeMode(QLayout::Minimum);

	m_pBaseLayout = new QGridLayout(3,3);
	m_pBaseLayout -> setResizeMode(QLayout::Minimum);

	m_pBaseLayout -> setRowStretch(1,10);
	m_pBaseLayout -> setColStretch(1,10);

	m_pBorderLeft = new KoToolDockBaseBorder(KoToolDockBaseBorder::Left,this);
	m_pBorderRight = new KoToolDockBaseBorder(KoToolDockBaseBorder::Right,this);
	m_pBorderTop = new KoToolDockBaseBorder(KoToolDockBaseBorder::Top,this);
	m_pBorderBottom = new KoToolDockBaseBorder(KoToolDockBaseBorder::Bottom,this);

	m_pBorderTopLeft = new KoToolDockBaseBorder(KoToolDockBaseBorder::TopLeft,this);
	m_pBorderTopRight = new KoToolDockBaseBorder(KoToolDockBaseBorder::TopRight,this);
	m_pBorderBottomRight = new KoToolDockBaseBorder(KoToolDockBaseBorder::BottomRight,this);
	m_pBorderBottomLeft = new KoToolDockBaseBorder(KoToolDockBaseBorder::BottomLeft,this);

	m_pCaptionManager = new KoToolDockBaseCaptionManager(this);
	connect(m_pCaptionManager,SIGNAL(doClose()),SLOT(hide()));
	connect(m_pCaptionManager,SIGNAL(doStick(bool)),SLOT(slotStick(bool)));
	connect( m_pCaptionManager, SIGNAL( doLock( bool ) ), SLOT( slotLock ( bool ) ) );

	m_pLayout -> addWidget(m_pBorderTopLeft,0,0);
	m_pLayout -> addMultiCellWidget(m_pBorderTop,0,0,1,3);
	m_pLayout -> addWidget(m_pBorderTopRight,0,4);

	m_pLayout -> addMultiCellWidget(m_pCaptionManager -> captionWidget(KoToolDockTop),1,1,1,3);

	m_pLayout -> addMultiCellWidget(m_pBorderLeft,1,3,0,0);
	m_pLayout -> addMultiCellWidget(m_pCaptionManager -> captionWidget(KoToolDockLeft),1,3,1,1);
	m_pLayout -> addLayout(m_pBaseLayout,2,2);
	m_pLayout -> addMultiCellWidget(m_pCaptionManager -> captionWidget(KoToolDockRight),1,3,3,3);
	m_pLayout -> addMultiCellWidget(m_pBorderRight,1,3,4,4);

	m_pLayout -> addMultiCellWidget(m_pCaptionManager -> captionWidget(KoToolDockBottom),3,3,1,3);

	m_pLayout -> addWidget(m_pBorderBottomLeft,4,0);
	m_pLayout -> addMultiCellWidget(m_pBorderBottom,4,4,1,3);
	m_pLayout -> addWidget(m_pBorderBottomRight,4,4);

	m_pCaptionManager -> setView(KoToolDockTop);

}

void KoToolDockBase::restore()
{

	/*    KApplication *app = KApplication::kApplication();
	    Q_ASSERT(app);
	    KConfig * cfg = app -> config() ;
	    Q_ASSERT( cfg );
	 
	    cfg -> setGroup( QString( "ToolDock-" ) + name() );
	 
	    int x, y, w, h;
	    x = cfg -> readNumEntry( "x", 0 );
	    y = cfg -> readNumEntry( "y", 0 );
	    w = cfg -> readNumEntry( "w", 10 );
	    h = cfg -> readNumEntry( "h",  10 );
	 
	*/
	//     kdDebug() << "Before fix dims for docker " << name() << " to x,y "
	// 	      << x << ", " << y
	// 	      << " and w,h " << w << ", " << h
	// 	      << "\n";


	//     fixPosition( x, y, w, h );
	//    fixSize( x, y, w, h );

	//    QPoint p = QPoint(x, y);
	//    QSize s = QSize(w ,h);

	//     kdDebug() << " After fix dims for docker " << name() << " to x,y "
	// 	      << p.x() << ", " << p.y()
	// 	      << " and w,h " << s.width() << ", " << s.height()
	// 	      << " inside parent " << m_pView -> name()
	// 	      << " with dimensions " << m_pView -> x() << ","
	// 	      << m_pView -> y() << " : "
	// 	      << m_pView -> width() << ", " << m_pView -> height()
	// 	      << "\n";

	/*    move( p );
	    resize( s );
	 
	    makeVisible( cfg -> readBoolEntry( "visible", false ) );
	 
	    slotStick( cfg -> readBoolEntry( "stick", false ) );
	    slotLock( cfg -> readBoolEntry( "lock", true ) ); */
}


KoToolDockBase::~KoToolDockBase()
{
	/*    KApplication *app = KApplication::kApplication();
	    Q_ASSERT(app);
	    KConfig * cfg = app -> config() ;
	    Q_ASSERT( cfg );
	    cfg -> setGroup( QString( "ToolDock-" ) + name() );
	    cfg -> writeEntry( "x",  x() );
	    cfg -> writeEntry( "y",  y() );
	    cfg -> writeEntry( "w",  width() );
	    cfg -> writeEntry( "h",  width() );
	    cfg -> writeEntry( "visible",  isVisible() );
	    cfg -> writeEntry( "lock",  isLocked() );
	    cfg -> writeEntry( "stick", isStick() );
	*/
	// XXX: write current snap? Or does the docker
	// resnap based on position?


}

void KoToolDockBase::updateCaption()
{
	m_pCaptionManager -> setView(getCaptionPos());
}

KoToolDockPosition KoToolDockBase::getCaptionPos(bool* f)
{
	bool l = snaps.get(KoToolDockLeft);
	bool r = snaps.get(KoToolDockRight);
	bool t = snaps.get(KoToolDockTop);
	bool b = snaps.get(KoToolDockBottom);
	if (f)
		*f = true;

	if (l && r && t && b)
		return KoToolDockTop;

	if (l && r && t)
		return KoToolDockBottom;

	if (l && r && b)
		return KoToolDockTop;

	if (l)
		return KoToolDockRight;

	if (r)
		return KoToolDockLeft;

	if (t)
		return KoToolDockBottom;

	if (b)
		return KoToolDockTop;

	if (f)
		*f = false;

	return KoToolDockTop;
}


void KoToolDockBase::slotStick(bool f)
{
	stick = f;
}

void KoToolDockBase::slotLock( bool f )
{
	lock = f;
}


void KoToolDockBase::show()
{
	snaps.clear();

	QPoint p(x(),y());

	if ( QABS(m_pView -> height() - (p.y() + height())) < 17 )
	{
		p.setY(m_pView -> height() - height());
		snaps.set(KoToolDockBottom, m_pView);
	}
	if ( QABS(p.y()) < 17 )
	{
		p.setY(0);
		snaps.set(KoToolDockTop, m_pView);
	}
	if ( QABS(p.x()) < 17 )
	{
		p.setX(0);
		snaps.set(KoToolDockLeft, m_pView);
	}
	if ( QABS(m_pView -> width() - (p.x() + width())) < 17 )
	{
		p.setX(m_pView -> width() - width());
		snaps.set(KoToolDockRight, m_pView);
	}
	updateCaption();

	QWidget::show();


	emit visibleChange(true);
}

void KoToolDockBase::hide()
{
	QWidget::hide();
	emit visibleChange(false);
}

void KoToolDockBase::makeVisible(bool v)
{
	if (v)
		show();
	else
		hide();
}

void KoToolDockBase::setView( QWidget* w )
{
	if (w -> parentWidget() != this)
		w -> reparent(this,QPoint(0, 0), true);
	m_pBaseLayout -> addWidget(w, 1, 1);
}

void KoToolDockBase::resizeEvent( QResizeEvent* ev )
{
	QWidget::resizeEvent(ev);

	int w = width();
	int h = height();

	QPoint p(x(),y());
	if ((m_pView -> height() - (p.y()+h) < 17) || (m_pView -> height() < (p.y()+h)))
	{
		p.setY(m_pView -> height() - h);
	}
	if (p.y() < 17 || p.y() < 0)
	{
		p.setY(0);
	}
	if (p.x() < 17 || p.x() < 0)
	{
		p.setX(0);
	}
	if ((m_pView -> width() - (p.x()+w) < 17) || (m_pView -> width() < (p.x()+w)))
	{
		p.setX(m_pView -> width() - w);
	}
	move(p);

	QRegion r1(2,0,w-4,h);
	QRegion r2(0,2,w,h-4);
	QRegion r3(1,1,w-2,h-2);
	setMask(r1.unite(r2).unite(r3));
}

void KoToolDockBase::beginResize()
{
	KoToolDockBaseBorder* b = (KoToolDockBaseBorder*)sender();
	switch (b -> position())
	{
	case KoToolDockBaseBorder::Left:
		mrManager -> doXResize(this,true);
		break;
	case KoToolDockBaseBorder::Right:
		mrManager -> doXResize(this,false);
		break;
	case KoToolDockBaseBorder::Top:
		mrManager -> doYResize(this,true);
		break;
	case KoToolDockBaseBorder::Bottom:
		mrManager -> doYResize(this,false);
		break;
	case KoToolDockBaseBorder::TopLeft:
		mrManager -> doXYResize(this,true,true);
		break;
	case KoToolDockBaseBorder::BottomRight:
		mrManager -> doXYResize(this,false,false);
		break;
	case KoToolDockBaseBorder::TopRight:
		mrManager -> doXYResize(this,false,true);
		break;
	case KoToolDockBaseBorder::BottomLeft:
		mrManager -> doXYResize(this,true,false);
		break;
	}
}

void KoToolDockBase::stopResize()
{
	mrManager -> stop();
	resize(mrManager -> geometry().size());
	move(parentWidget() -> mapFromGlobal(mrManager -> geometry().topLeft()));
	updateCaption();
}

void KoToolDockBase::beginMove()
{
	mrManager -> doMove(this);
}

void KoToolDockBase::stopMove()
{
	mrManager -> stop();
	move(parentWidget() -> mapFromGlobal(mrManager -> geometry().topLeft()));
	updateCaption();
}

void KoToolDockBase::positionChanged()
{
	snaps.clear();

	QPoint p(mrManager -> x(),mrManager -> y());
	p = m_pView -> mapFromGlobal(p);

	if ( QABS(m_pView -> height() - (p.y()+height())) < 17 )
	{
		p.setY(m_pView -> height() - height());
		snaps.set(KoToolDockBottom,m_pView);
	}
	if ( QABS(p.y()) < 17 )
	{
		p.setY(0);
		snaps.set(KoToolDockTop,m_pView);
	}
	if ( QABS(p.x()) < 17 )
	{
		p.setX(0);
		snaps.set(KoToolDockLeft,m_pView);
	}
	if ( QABS(m_pView -> width() - (p.x()+width())) < 17 )
	{
		p.setX(m_pView -> width() - width());
		snaps.set(KoToolDockRight,m_pView);
	}
	mrManager -> move(m_pView -> mapToGlobal(p));
}

void KoToolDockBase::sizeChanged()
{
	snaps.clear();

	QPoint p(mrManager -> x(),mrManager -> y());
	p = m_pView -> mapFromGlobal(p);
	QRect r(p,QSize(mrManager -> width(),mrManager -> height()));

	if ( QABS(m_pView -> height() - r.bottom()) < 17 )
	{
		r.setHeight(m_pView -> height() - r.y());
		snaps.set(KoToolDockBottom,m_pView);
	}
	if ( QABS(r.y()) < 17 )
	{
		r.setY(0);
		snaps.set(KoToolDockTop,m_pView);
	}
	if ( QABS(r.x()) < 17 )
	{
		r.setX(0);
		snaps.set(KoToolDockLeft,m_pView);
	}
	if ( QABS(m_pView -> width() - r.right()) < 17 )
	{
		r.setWidth(m_pView -> width() - r.x());
		snaps.set(KoToolDockRight,m_pView);
	}

	r.moveTopLeft(m_pView -> mapToGlobal(r.topLeft()));
	mrManager -> setGeometry(r);
}

void KoToolDockBase::fixPosition(int& x, int& y, int& w, int& h)
{
	QRect r(m_pView -> mapToGlobal(QPoint(0,0)),m_pView -> size());
	x = QMIN(QMAX(r.x(),x),r.right()-w+1);
	y = QMIN(QMAX(r.y(),y),r.bottom()-h+1);
}

void KoToolDockBase::fixSize(int& x, int& y, int& w, int& h)
{
	QRect r(m_pView -> mapToGlobal(QPoint(0,0)),m_pView -> size());
	if (r.x()>x)
	{
		w -= r.x()-x;
		x = r.x();
	}
	if (r.y()>y)
	{
		h -= r.y()-y;
		y = r.y();
	}
	if (x+w>r.right())
	{
		w -= x + w -r.right()-1;
	}
	if (y+h>r.bottom())
	{
		h -= y + h -r.bottom()-1;
	}
}

void KoToolDockBase::mousePressEvent( QMouseEvent* )
{
	activate();
}

void KoToolDockBase::activate()
{
	raise();
}


void KoToolDockBase::mouseStatus(bool f)
{
	if (!isVisible())
		return;

	if (f || mrManager -> isWorking())
	{
		hideTimer.stop();
		if (hStatus == hstarthide)
			hStatus = hnone;
	}
	else
	{
		if (hStatus!=hnone || stick)
			return;

		bool f;
		hideDirection = getCaptionPos(&f);
		switch (hideDirection)
		{
		case KoToolDockLeft:
			hideDirection = KoToolDockRight;
			break;
		case KoToolDockRight:
			hideDirection = KoToolDockLeft;
			break;
		case KoToolDockTop:
			hideDirection = KoToolDockBottom;
			break;
		case KoToolDockBottom:
			hideDirection = KoToolDockTop;
			break;
		default:
			break;
		}

		if (f)
		{
			hStatus = hstarthide;
			hideTimer.start(1000,true);
		}
	}
}

void KoToolDockBase::slotHideTimeOut()
{
	m_pBaseLayout -> setEnabled(false);
	m_pLayout -> setEnabled(false);

	hpixmap = new QPixmap(QPixmap::grabWidget(this));
	hminsize = minimumSize();
	hmaxsize = maximumSize();
	hsize = size();

	int m = m_pCaptionManager -> captionHeight()+3;

	int wb = 0;
	switch (hideDirection)
	{
	case KoToolDockLeft:
	case KoToolDockRight:
		wb = width();
		break;
	case KoToolDockTop:
	case KoToolDockBottom:
		wb = height();
		break;
	default:
		break;
	}
	hdx = QMAX((wb-m)/50,1);
	hStatus = hprocesshide;
	setFixedSize(size());

	QObjectList* ch = (QObjectList*)children();
	for (QObject* o = ch -> first();o;o=ch -> next())
		if (o -> isWidgetType())
			((QWidget*)o) -> hide();

	hideProcessTimer.start(1);
}

void KoToolDockBase::slotHideProcessTimeOut()
{
	QPoint p(pos());
	QSize s(size());
	int z;
	int m = m_pCaptionManager -> captionHeight()+3;

	switch (hideDirection)
	{
	case KoToolDockLeft:
		s.setWidth(QMIN(QMAX(s.width()-hdx,m),hsize.width()));
		break;
	case KoToolDockRight:
		z = s.width();
		s.setWidth(QMIN(QMAX(s.width()-hdx,m),hsize.width()));
		p.setX(p.x()+(z-s.width()));
		break;
	case KoToolDockTop:
		s.setHeight(QMIN(QMAX(s.height()-hdx,m),hsize.height()));
		break;
	case KoToolDockBottom:
		z = s.height();
		s.setHeight(QMIN(QMAX(s.height()-hdx,m),hsize.height()));
		p.setY(p.y()+(z-s.height()));
		break;
	default:
		break;
	}
	setFixedSize(s);
	move(p);

	if (s.height()==m || s.width()==m)
		hideProcessStop();

	if (s.height()==hsize.height() && s.width()==hsize.width())
		showProcessStop();
}

void KoToolDockBase::hideProcessStop()
{
	hideProcessTimer.stop();
	hStatus = hdonehide;
}

void KoToolDockBase::paintEvent( QPaintEvent* )
{
	if (!(hStatus==hprocesshide||hStatus==hdonehide||hStatus==hprocessshow))
		return;

	QPainter p(this,this);
	switch (hideDirection)
	{
	case KoToolDockLeft:
		p.drawPixmap(width() - hpixmap -> width(),0,*hpixmap);
		break;
	case KoToolDockRight:
	case KoToolDockBottom:
		p.drawPixmap(0,0,*hpixmap);
		break;
	case KoToolDockTop:
		p.drawPixmap(0,height() - hpixmap -> height(),*hpixmap);
		break;
	default:
		break;
	}
	p.end();
}

void KoToolDockBase::enterEvent( QEvent* )
{
	if (hStatus == hdonehide)
	{
		hStatus = hprocessshow;
		hdx = -2*hdx;
		hideProcessTimer.start(1);
	}
}

void KoToolDockBase::showProcessStop()
{
	hideProcessTimer.stop();
	hStatus = hnone;

	QObjectList* ch = (QObjectList*)children();
	for (QObject* o = ch -> first(); o; o = ch -> next())
		if (o -> isWidgetType() && !o -> isA("KoToolDockBaseCaption"))
			((QWidget*)o) -> show();

	updateCaption();
	setMinimumSize(hminsize);
	setMaximumSize(hmaxsize);
	delete hpixmap;

	m_pBaseLayout -> setEnabled(true);
	m_pLayout -> setEnabled(true);

	hsize.setWidth(hsize.width() + 1);
	resize(hsize);

	hsize.setWidth(hsize.width() - 1);
	resize(hsize);
}

/*********************************************************************************************/

KoTabbedToolDock::KoTabbedToolDock( QWidget* parent, const char* name )
		: KoToolDockBase ( parent, name )
{
	QFont f;
	f.setPointSize(8);
	setFont(f);

	setView( m_tabwidget = new QTabWidget( this ) );
	m_tabwidget -> setBaseSize( 175, 125 );

}

KoTabbedToolDock::~KoTabbedToolDock()
{
	delete m_tabwidget;
}

void KoTabbedToolDock::plug (QWidget* w)
{
	m_tabwidget-> addTab( w , w -> caption());
}

void KoTabbedToolDock::unplug(QWidget *w)
{
	m_tabwidget -> removePage(w);
}

void KoTabbedToolDock::showPage(QWidget *w)
{
	m_tabwidget-> showPage(w);
}

#include "kotooldockbase.moc"
