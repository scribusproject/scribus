#ifndef AUTOFORM_H
#define AUTOFORM_H

#include <qwidget.h>
#include <qtoolbutton.h>
class QGridLayout;
class QPopupMenu;
class QPixmap;
class QBbuttonGroup;


class Autoforms : public QToolButton
{
    Q_OBJECT

public:
	Autoforms( QWidget* parent );
	~Autoforms() {};
	double* getShapeData(int a, int *n);
	QPixmap getIconPixmap(int nr);

	QButtonGroup* buttonGroup1;
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
	QPopupMenu* Men;

public slots:
	void selForm(int a);

signals:
	void FormSel(int, int, double *);

protected:
	QGridLayout* buttonGroup1Layout;
};

#endif

