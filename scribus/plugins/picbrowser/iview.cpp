//
// C++ Implementation: iview
//
// Description:
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "iview.h"

#include <QApplication>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPolygonF>
#include <QDebug>

IView::IView ( QWidget * parent )
		:QGraphicsView ( parent )
{
	m_curImage = 0;
	setScene ( new QGraphicsScene );
	setInteractive ( true );
	m_isPanning = false;
	m_keepFitted = false;
	setAlignment ( Qt::AlignTop | Qt::AlignLeft );
	setTransformationAnchor ( QGraphicsView::NoAnchor );
	setRenderHint(QPainter::SmoothPixmapTransform);
}

IView::~ IView()
{
	QGraphicsScene * s(scene());
	if(s)
		delete s;
}

void IView::mouseMoveEvent ( QMouseEvent * e )
{
	////qDebug()<<"IView::mouseMoveEvent";
	if ( m_isPanning )
	{
		QPointF pos ( e->pos() );
		int vDelta ( qRound ( m_mouseStartPoint.y() - pos.y() ) );
		int hDelta ( qRound ( m_mouseStartPoint.x() - pos.x() ) );
		verticalScrollBar()->setValue ( verticalScrollBar()->value() + vDelta );
		horizontalScrollBar()->setValue ( horizontalScrollBar()->value() + hDelta );
		m_mouseStartPoint = pos;
		return;
	}
}

void IView::mousePressEvent ( QMouseEvent * e )
{
	//qDebug()<<"IView::mousePressEvent";
	if ( !scene() )
		return;

// 	if ( e->button() == Qt::MidButton )
	{
		m_mouseStartPoint =  e->pos() ;
		m_isPanning = true;
		QApplication::setOverrideCursor ( QCursor ( Qt::ClosedHandCursor ) );
	}
}

void IView::mouseReleaseEvent ( QMouseEvent * e )
{
	//qDebug()<<"IView::mouseReleaseEvent";
	if ( m_isPanning )
	{
		m_isPanning = false;
		QApplication::restoreOverrideCursor();
		return;
	}
}


void IView::setImage(const QPixmap & pixmap)
{
	//qDebug()<<"IView::setImage";
	if ( m_curImage )
	{
		delete m_curImage;
		m_curImage = 0;
	}
	
	m_curImage = scene()->addPixmap ( pixmap );
}

QPixmap IView::getPixmap()
{
	//qDebug()<<"IView::getPixmap";
	if ( m_curImage )
	{
		return  m_curImage->pixmap();
	}
	return QPixmap();
}



void IView::fitImage()
{
	//qDebug()<<"IView::fitImage";
	if(!m_curImage)
		return;
	
	double wR = width() /m_curImage->boundingRect().width()  ;
	double hR =  height()/ m_curImage->boundingRect().height();
	
	double R = (wR > hR) ? hR : wR;
	QTransform T;
	T.scale(R,R);
	setTransform( T  , false);
	
}

void IView::resizeEvent(QResizeEvent * event)
{
	//qDebug()<<"View::resizeEvent";
	if(m_keepFitted)
		fitImage();
}

void IView::setZoom(double xz, double yz)
{
	QTransform T;
	T.scale(xz, (yz > 0) ? yz : xz);
	setTransform( T  , false);
}

double IView::getZoom() const
{
	return transform().m11();
}

