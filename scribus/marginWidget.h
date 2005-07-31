#ifndef MARGINWIDGET_H
#define MARGINWIDGET_H

#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include "scribusstructs.h"
class MSpinBox;

/*! Widget for Margins setting.
Used e.g. in "New Doc Dialog" or "Preferences".
*/
class MarginWidget : public QGroupBox
{
	Q_OBJECT

public:
	/*! It creates an user interface for base margin settings for the document.
	\param parent Parent widget (used as standard in Qt)
	\param title Title of the Group box
	\param MarginStruct* margs: A reference to the margins in the preferences.
	\param decimals How much decimals in spinboxes: e.g. 0.00 for 2 decimals.
	\param unit mainly doc->unitRatio
	\param einh unit text to display in the spin boxes (mm, pt etc)
	*/
	MarginWidget( QWidget* parent, QString title, MarginStruct* margs, int decimals, double unitRatio, QString einh);
	~MarginWidget() {};
	/*! Setup the labels by facing pages option */
	void setFacingPages(bool facing);
	/*! Setup the spinboxes properties (min/max value etc.) by width */
	void setPageWidth(double width);
	/*! Setup the spinboxes properties (min/max value etc.) by height */
	void setPageHeight(double heigth);
	/*! Setup the widgets by new options */
	void unitChange(double newUnit, int newDecimals, QString newSuffix);
	/*! Spinboxes */
	MSpinBox* topR;
	MSpinBox* bottomR;
	MSpinBox* rightR;
	MSpinBox* leftR;
	/*! Labels */
	QLabel* GRText1;
	QLabel* GRText2;
	QLabel* GRText3;
	QLabel* GRText4;
	/*! Top margin value converted by unitRatio */
	double RandT;
	/*! Bottom margin value converted by unitRatio */
	double RandB;
	/*! Left margin value converted by unitRatio */
	double RandL;
	/*! Right margin value converted by unitRatio */
	double RandR;
	/*! Self contained ratio taken in constructor */
	double unitRatio;
	/*! Internally used page width */
	double pageWidth;
	/*! Internally used page height */
	double pageHeight;
	/*! unused */
	int docUnitIndex;

public slots:
	/*! Recompute the values after spinbox change */
	void setTop();
	void setBottom();
	void setLeft();
	void setRight();

protected:
	QGridLayout* GroupLayout;
};

#endif
