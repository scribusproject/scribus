/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QEvent>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QMouseEvent>
#include <QLabel>
#include <QKeyEvent>
#include <QPaintEvent>
#include "fpointarray.h"
#include "scribusapi.h"

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QPushButton;
class QToolButton;
class QLabel;

class SCRIBUS_API KCurve : public QWidget
{
	Q_OBJECT

public:
	KCurve(QWidget *parent);
	virtual ~KCurve();
	void setPixmap(QPixmap pix);

protected:
	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent (QMouseEvent * e);
	void mouseReleaseEvent ( QMouseEvent * e );
	void mouseMoveEvent ( QMouseEvent * e );
	void leaveEvent ( QEvent * );

public:
	double getCurveValue(double x);
	FPointArray getCurve();
	void setCurve(FPointArray inlist);
	void resetCurve();
	void setLinear(bool setter);
	bool isLinear();

signals:
	void modified();

private:
	double m_leftmost;
	double m_rightmost;
	FPoint m_grab_point;
	int m_pos;
	bool m_dragging;
	bool m_linear;
	double m_grabOffsetX;
	double m_grabOffsetY;
	FPointArray m_points;
	FPointArray m_points_back;
	QPixmap *m_pix;
};

class SCRIBUS_API CurveWidget : public QWidget
{
	Q_OBJECT

public:
	CurveWidget(QWidget* parent);
	~CurveWidget() {};
	void setLinear(bool setter);
	QPushButton* invertButton;
	QPushButton* resetButton;
	QToolButton *linearButton;
	QPushButton* loadButton;
	QPushButton* saveButton;
	KCurve* cDisplay;

private slots:
	void doInvert();
	void doReset();
	void doLinear();
	void doLoad();
	void doSave();

protected:
	Q3HBoxLayout* CurveWidgetLayout;
	Q3VBoxLayout* layout1;
	QSpacerItem* spacer1;

protected slots:
	virtual void languageChange();
};

#endif // CURVEWIDGET_H
