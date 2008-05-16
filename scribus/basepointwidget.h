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
	~BasePointWidget() {};
	int checkedId();
	void setCheckedId(int id);
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *m);
	void leaveEvent(QEvent*);
private:
	int m_mode;
	int selectedBase;
	int hooveredBase;
	bool Mpressed;
signals:
	void buttonClicked(int);
};

#endif
