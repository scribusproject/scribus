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
	curImage = 0;
	setScene ( new QGraphicsScene );
	setInteractive ( true );
	isPanning = false;
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
	if ( isPanning )
	{
		QPointF pos ( e->pos() );
		int vDelta ( qRound ( mouseStartPoint.y() - pos.y() ) );
		int hDelta ( qRound ( mouseStartPoint.x() - pos.x() ) );
		verticalScrollBar()->setValue ( verticalScrollBar()->value() + vDelta );
		horizontalScrollBar()->setValue ( horizontalScrollBar()->value() + hDelta );
		mouseStartPoint = pos;
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
		mouseStartPoint =  e->pos() ;
		isPanning = true;
		QApplication::setOverrideCursor ( QCursor ( Qt::ClosedHandCursor ) );
	}
}

void IView::mouseReleaseEvent ( QMouseEvent * e )
{
	//qDebug()<<"IView::mouseReleaseEvent";
	if ( isPanning )
	{
		isPanning = false;
		QApplication::restoreOverrideCursor();
		return;
	}
}


void IView::setImage(const QPixmap & pixmap)
{
	//qDebug()<<"IView::setImage";
	if ( curImage )
	{
		delete curImage;
		curImage = 0;
	}
	
	curImage = scene()->addPixmap ( pixmap );
}

QPixmap IView::getPixmap()
{
	//qDebug()<<"IView::getPixmap";
	if ( curImage )
	{
		return  curImage->pixmap();
	}
	return QPixmap();
}



void IView::fitImage()
{
	//qDebug()<<"IView::fitImage";
	if(!curImage)
		return;
	
	double wR = width() /curImage->boundingRect().width()  ;
	double hR =  height()/ curImage->boundingRect().height();
	
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

