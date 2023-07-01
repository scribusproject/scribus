//
// C++ Interface: iview
//
// Description:
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef IVIEW_H
#define IVIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QRect>
#include <QPixmap>

class IView: public QGraphicsView
{
	Q_OBJECT

	public:
		IView ( QWidget* parent );
		~IView();

		void setImage ( const QPixmap& pixmap );
		QPixmap getPixmap();

		void fitImage();
		void setKeepFitted ( bool theValue ) { m_keepFitted = theValue; }

		void setZoom(double xz, double yz = 0);
		double getZoom() const;

	protected:
		void mouseMoveEvent(QMouseEvent* e) override;
		void mousePressEvent(QMouseEvent* e) override;
		void mouseReleaseEvent(QMouseEvent* e) override;
		void resizeEvent(QResizeEvent* event) override;

	private:
		QGraphicsPixmapItem* curImage { nullptr };

		QPointF mouseStartPoint;
		QRect theRect;
		bool isPanning { false };

		bool m_keepFitted { false };
};

#endif
