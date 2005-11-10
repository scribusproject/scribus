#ifndef AUTOFORMBUTTONGROUP_H
#define AUTOFORMBUTTONGROUP_H

#include <qwidget.h>
#include <q3buttongroup.h>
#include <qtoolbutton.h>
//Added by qt3to4:
#include <QPixmap>
#include <QGridLayout>
#include <Q3PopupMenu>
#include "scribusapi.h"
class QGridLayout;
class Q3PopupMenu;
class QPixmap;


class SCRIBUS_API AutoformButtonGroup : public Q3ButtonGroup
{
    Q_OBJECT

public:
	AutoformButtonGroup( QWidget* parent );
	~AutoformButtonGroup() {};
	double* getShapeData(int a, int *n);
	QPixmap getIconPixmap(int nr);

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
	
public slots:
	void selForm(int a);

signals:
	void FormSel(int, int, double *);

protected:
	QGridLayout* buttonGroup1Layout;
};

#endif


