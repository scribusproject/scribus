#ifndef MARGINWIDGET_H
#define MARGINWIDGET_H

#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include "scribusstructs.h"
class MSpinBox;

class MarginWidget : public QGroupBox
{
	Q_OBJECT

public:
	MarginWidget( QWidget* parent, QString title, MarginStruct* margs, int decimals, double unitRatio, QString einh);
	~MarginWidget() {};
	void setFacingPages(bool facing);
	void setPageWidth(double width);
	void setPageHeight(double heigth);
	void unitChange(double newUnit, int newDecimals, QString newSuffix);
	MSpinBox* topR;
	MSpinBox* bottomR;
	MSpinBox* rightR;
	MSpinBox* leftR;
	QLabel* GRText1;
	QLabel* GRText2;
	QLabel* GRText3;
	QLabel* GRText4;
	double RandT;
	double RandB;
	double RandL;
	double RandR;
	double unitRatio;
	double pageWidth;
	double pageHeight;
	int docUnitIndex;

public slots:
	void setTop();
	void setBottom();
	void setLeft();
	void setRight();

protected:
	QGridLayout* GroupLayout;
};

#endif
