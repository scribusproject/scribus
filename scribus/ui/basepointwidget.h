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

protected:
	void paintEvent(QPaintEvent *e) override;
	void mousePressEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void leaveEvent(QEvent*) override;

public slots:
	void setAngle(int angle);
	void setSelectedAnchor(AnchorPoint id);
	void setMode(BasePointWidget::Mode mode);

private:
	Mode m_mode { BasePointWidget::Full };
	AnchorPoint m_selectedAnchor { AnchorPoint::TopLeft };
	AnchorPoint m_hoveredAnchor { AnchorPoint::None };
	bool m_pressed { false };
	int m_angle { 0 };

	QMap<AnchorPoint, QRect> anchors;
	QTransform m_transform;
	QRectF m_baseRect;

	QPoint mapToTransformation(QPoint position);
	void setup();

signals:
	void buttonClicked(AnchorPoint);
};

#endif
