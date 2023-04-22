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

class SCRIBUS_API BasePointWidget : public QWidget
{
	Q_OBJECT

/*public:
	BasePointWidget(QWidget *pa, int mode = 0);
	~BasePointWidget() {}
	/**
	  BasePoint values:
	  0 => top left
	  1 => top right
	  2 => center
	  3 => bottom left
	  4 => bottom right
	  */
/*	int checkedId() const;
	void setCheckedId(int id);
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *m);
	void leaveEvent(QEvent*);
private:
	int m_mode { 0 };
	int m_selectedBase { 0 };
	int m_hooveredBase { -1 };
	bool m_mousePressed { false };
signals:
	void buttonClicked(int);
};
*/

public:
    enum Mode { Full = 0, Frame = 1, Line = 2 };
    Q_ENUM(Mode)

    enum AnchorPoint { None = 0, TopLeft = 1, Top = 2, TopRight = 3, Left = 4, Center = 5, Right = 6, BottomLeft = 7, Bottom = 9, BottomRight = 10 };
    Q_ENUM(AnchorPoint)

    BasePointWidget(Mode mode, QWidget *parent = nullptr);
    BasePointWidget(QWidget *parent = nullptr);
    ~BasePointWidget() {}


    BasePointWidget::AnchorPoint selectedAnchor();
    int angle();
    BasePointWidget::Mode mode();

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *m);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *m);
    void leaveEvent(QEvent*);

public slots:
    void setAngle(int angle);
    void setSelectedAnchor(BasePointWidget::AnchorPoint id);
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
    void buttonClicked(BasePointWidget::AnchorPoint);
};

#endif
