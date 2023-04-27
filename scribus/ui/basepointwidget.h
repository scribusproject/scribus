/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef BASEPOINTWIDGET_H
#define BASEPOINTWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API BasePointWidget : public QWidget
{
	Q_OBJECT

public:
	enum Mode { Full = 0, Frame = 1, Line = 2 };
	Q_ENUM(Mode)

	BasePointWidget(Mode mode, QWidget *parent = nullptr);
	BasePointWidget(QWidget *parent = nullptr);
	~BasePointWidget() {}


	AnchorPoint selectedAnchor();
	int angle();
	BasePointWidget::Mode mode();

	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *m);
	void leaveEvent(QEvent*);

public slots:
	void setAngle(int angle);
	void setSelectedAnchor(AnchorPoint id);
	void setMode(BasePointWidget::Mode mode);

private:
	Mode m_mode;
	AnchorPoint m_selectedAnchor;
	AnchorPoint m_hoveredAnchor;
	bool m_pressed;
	int m_angle;

	QMap<AnchorPoint, QRect>anchors;
	QTransform m_transform;
	QRectF m_baseRect;

	QPoint mapToTransformation(QPoint position);
	void setup();

signals:
	void buttonClicked(AnchorPoint);
};

#endif
