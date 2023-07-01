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

public:
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
	int checkedId() const;
	void setCheckedId(int id);

protected:
	void paintEvent(QPaintEvent *e) override;
	void mousePressEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void leaveEvent(QEvent*) override;

private:
	int m_mode { 0 };
	int m_selectedBase { 0 };
	int m_hooveredBase { -1 };
	bool m_mousePressed { false };

signals:
	void buttonClicked(int);
};

#endif
