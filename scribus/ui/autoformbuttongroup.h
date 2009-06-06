/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef AUTOFORMBUTTONGROUP_H
#define AUTOFORMBUTTONGROUP_H

#include <QWidget>
#include <QMenu>
#include <QSignalMapper>
//#include <QToolButton>
//#include <QFrame>
//#include <QGridLayout>
#include <QPixmap>
//#include <QButtonGroup>
//#include <QCheckBox>
#include <QEvent>
#include "scribusapi.h"

class SCRIBUS_API AutoformButtonGroup : public QMenu
{
    Q_OBJECT

public:
	AutoformButtonGroup( QWidget* parent );
	~AutoformButtonGroup() {};
	void addShape(QMenu* menu, int shapenum);
	qreal* getShapeData(int a, int *n);
	QPixmap getIconPixmap(int nr, int pixmapSize=22);
	virtual void changeEvent(QEvent *e);
/*	QButtonGroup* buttonGroup;
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
	QCheckBox* checkDoubleArrows;
	QToolButton* toolButton21;
	QToolButton* toolButton22;
	QToolButton* toolButton23;
	QCheckBox* checkFlowChart;
	QToolButton* toolButton25;
	QToolButton* toolButton26;
	QToolButton* toolButton27;
	QToolButton* toolButton28;
	QToolButton* toolButton29;
	QToolButton* toolButton30;
	QToolButton* toolButton31;
	QToolButton* toolButton32;
	QCheckBox* checkJigSaw;
	QToolButton* toolButton33;
	QToolButton* toolButton34;
	QToolButton* toolButton35;
	QToolButton* toolButton36;
	QToolButton* toolButton37;
	QToolButton* toolButton38;
	QToolButton* toolButton39;
	QToolButton* toolButton40;
	QToolButton* toolButton41;
	QToolButton* toolButton42;
	QToolButton* toolButton43;
	QToolButton* toolButton44;
	QCheckBox* checkSpecials;
	QToolButton* toolButton24;
	QToolButton* toolButton45;
*/
public slots:
	void selForm(int a);
	void languageChange();
/*
private slots:
	void toggleDoubleArrows(bool visible);
	void toggleSpecials(bool visible);
	void toggleFlowChart(bool visible);
	void toggleJigSaw(bool visible);
*/
signals:
	void FormSel(int, int, qreal *);
	void buttonClicked(int);

protected:
	QSignalMapper *signalMapper;
	QMenu *menu1;
	QMenu *menu2;
	QMenu *menu3;
	QMenu *menu4;
	QMenu *menu5;
/*	QGridLayout* buttonGroup1Layout;
*/
};

#endif


