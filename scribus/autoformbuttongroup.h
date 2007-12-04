/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef AUTOFORMBUTTONGROUP_H
#define AUTOFORMBUTTONGROUP_H

#include <QWidget>
#include <QToolButton>
#include <QFrame>
#include <QGridLayout>
#include <QPixmap>
#include <QButtonGroup>
#include "scribusapi.h"


class SCRIBUS_API AutoformButtonGroup : public QFrame
{
    Q_OBJECT

public:
	AutoformButtonGroup( QWidget* parent );
	~AutoformButtonGroup() {};
	double* getShapeData(int a, int *n);
	QPixmap getIconPixmap(int nr, int pixmapSize=22);
	QButtonGroup* buttonGroup;
	QToolButton* toolButton1;
	QToolButton* toolButton2;
	QToolButton* toolButton3;
	QToolButton* toolButton4;
	QToolButton* toolButton5;
	QToolButton* toolButton6;
	QToolButton* toolButton7;
	QToolButton* toolButton8;
	QToolButton* toolButton9;
	QToolButton* toolButton10;
	QToolButton* toolButton11;
	QToolButton* toolButton12;
	QToolButton* toolButton13;
	QToolButton* toolButton14;
	QToolButton* toolButton15;
	QToolButton* toolButton16;
	QToolButton* toolButton17;
	QToolButton* toolButton18;
	QToolButton* toolButton19;
	QToolButton* toolButton20;
	QToolButton* toolButton21;
	QToolButton* toolButton22;
	QToolButton* toolButton23;
	QToolButton* toolButton24;
	
public slots:
	void selForm(int a);

signals:
	void FormSel(int, int, double *);
	void buttonClicked(int);

protected:
	QGridLayout* buttonGroup1Layout;
};

#endif


