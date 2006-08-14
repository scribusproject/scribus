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
#include "fpointarray.h"
#include "scribusapi.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
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

signals:
	void modified();

private:
	double m_leftmost;
	double m_rightmost;
	FPoint m_grab_point;
	int m_pos;
	bool m_dragging;
	double m_grabOffsetX;
	double m_grabOffsetY;
	FPointArray m_points;
	QPixmap *m_pix;
};

class SCRIBUS_API CurveWidget : public QWidget
{
	Q_OBJECT

public:
	CurveWidget(QWidget* parent);
	~CurveWidget() {};

	QPushButton* invertButton;
	QPushButton* resetButton;
	QPushButton* loadButton;
	QPushButton* saveButton;
	KCurve* cDisplay;

private slots:
	void doInvert();
	void doReset();

protected:
	QHBoxLayout* CurveWidgetLayout;
	QVBoxLayout* layout1;
	QSpacerItem* spacer1;

protected slots:
	virtual void languageChange();
};

#endif // CURVEWIDGET_H
