#ifndef MARGINWIDGET_H
#define MARGINWIDGET_H

#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qcombobox.h>

#include "scribusapi.h"
#include "scribusstructs.h"

class MSpinBox;


/*! This is inherited QComboBox widget used in MarginWidget as "Preset List".
It contains functionality for margins setting in various ways.
\author Petr Vanek, <petr@yarpen.cz>
*/
class SCRIBUS_API PresetLayout: public QComboBox
{
	Q_OBJECT

public:
	/*! QComboBox like constructor. Values/names are set here. Tooltip etc. too. */
	PresetLayout(QWidget *parent = 0, const char * name = 0);
	~PresetLayout(){};

	/*! Compute the margins here.
	\param index selected item
	\param pageWidth width of the page. Taken from NewDoc dialog.
	\param pageHeight height of the page. Taken from NewDoc dialog.
	\param leftMargin leadin margin value. The others margins are set in various ratios related to this one.
	*/
	MarginStruct getMargins(int index, double pageWidth, double pageHeight, double leftMargin);

	/*! Integerized indexes for tr() strings*/
	enum presetID
	{
		none = 0,
		book = 1,
		magazine = 2,
		fibonacci = 3,
		goldencut = 4,
		nineparts = 5
	};

	/*! returns updateMargins value */
	bool needUpdate();

private:
	/*! Flag if is needed to recompute values and disable widgets */
	bool updateMargins;
};


/*! Widget for Margins setting.
Used e.g. in "New Doc Dialog" or "Preferences".
*/
class SCRIBUS_API MarginWidget : public QGroupBox
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
	/*! Setup the spinboxes properties (min/max value etc.) by width and height */
	void setPageWidthHeight(double width, double height);
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
	PresetLayout* presetCombo;
	/*! Labels */
	QLabel* lText;
	QLabel* rText;
	QLabel* tText;
	QLabel* bText;
	QLabel* presetLabel;
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
	/*! Recompute margins in PresetLayout combobox and disable/enable widgets. */
	void setPreset();

protected:
	QGridLayout* GroupLayout;
};

#endif
