/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MARGINWIDGET_H
#define MARGINWIDGET_H

#include "scribusapi.h"
#include "scribusstructs.h"

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

class QCheckBox;

class MSpinBox;


/*! \brief This is inherited QComboBox widget used in MarginWidget as "Preset List".
It contains functionality for margins setting in various ways.
\author Petr Vanek, <petr@yarpen.cz>
*/
class SCRIBUS_API PresetLayout: public QComboBox
{
	Q_OBJECT

public:
	/*! \brief QComboBox like constructor. Values/names are set here. Tooltip etc. too. */
	PresetLayout(QWidget *parent = 0, const char * name = 0);
	~PresetLayout(){};

	/*! \brief Compute the margins here.
	\param index selected item
	\param pageWidth width of the page. Taken from NewDoc dialog.
	\param pageHeight height of the page. Taken from NewDoc dialog.
	\param leftMargin leadin margin value. The others margins are set in various ratios related to this one.
	*/
	MarginStruct getMargins(int index, double pageWidth, double pageHeight, double leftMargin);

	/*! \brief Integerized indexes for tr() strings*/
	enum presetID
	{
		none = 0,
		gutenberg = 1,
		magazine = 2,
		fibonacci = 3,
		goldencut = 4,
		nineparts = 5
	};

	/*! \brief returns updateMargins value */
	bool needUpdate();

private:
	/*! \brief Flag if is needed to recompute values and disable widgets */
	bool updateMargins;
};


/*! \brief Widget for Margins setting.
Used e.g. in "New Doc Dialog" or "Preferences".
*/
class SCRIBUS_API MarginWidget : public QGroupBox
{
	Q_OBJECT

public:
	/*! \brief It creates an user interface for base margin settings for the document.
	\param parent Parent widget (used as standard in Qt)
	\param title Title of the Group box
	\param MarginStruct* margs: A reference to the margins in the preferences.
	\param decimals How much decimals in spinboxes: e.g. 0.00 for 2 decimals.
	\param unit mainly doc->unitRatio
	\param einh unit text to display in the spin boxes (mm, pt etc)
	*/
	MarginWidget( QWidget* parent, QString title, MarginStruct* margs, int unitIndex, bool showChangeAll=false);
	~MarginWidget() {};
	/*! \brief Setup the labels by facing pages option */
	void setFacingPages(bool facing);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by width and height */
	void setPageWidthHeight(double width, double height);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by width */
	void setPageWidth(double width);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by height */
	void setPageHeight(double heigth);
	/*! \brief Setup the widgets by new options */
	void unitChange(double newUnit, int newDecimals, QString newSuffix);
	/*! \brief Set the page size for margin getting from cups */
	void setPageSize(const QString& pageSize);
	
	/*! \brief Top Margin
	\retval double margin size */
	double top();
	/*! \brief Bottom Margin
	\retval double margin size */
	double bottom();
	/*! \brief Left Margin
	\retval double margin size */
	double left();
	/*! \brief Right Margin
	\retval double margin size */
	double right();

	/*! \brief set new margin values for the dialog
	It calls overridden setTop(), setBottom() etc. slots itself
	via signals emitted from spinboxes.
	\param t new top value
	\param b new bottom value
	\param l new left value
	\param r new right value
	*/
	void setNewMargins(double t, double b, double l, double r);
	
	/*! \brief Return marginsForAllPages property
	\retval bool true to apply for all pages */
	bool getMarginsForAllPages();

private:
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
	QCheckBox* marginsForAllPages;
	QPushButton* usePrinterMarginsButton;
	/*! \brief Top margin value converted by unitRatio */
	double RandT;
	/*! \brief Bottom margin value converted by unitRatio */
	double RandB;
	/*! \brief Left margin value converted by unitRatio */
	double RandL;
	/*! \brief Right margin value converted by unitRatio */
	double RandR;
	/*! \brief Internally used page width */
	double pageWidth;
	/*! \brief Internally used page height */
	double pageHeight;

public slots:
	/*! \brief Recompute the values after spinbox change */
	void setTop();
	void setBottom();
	void setLeft();
	void setRight();
	/*! \brief Recompute margins in PresetLayout combobox and disable/enable widgets. */
	void setPreset();
	
protected slots:	
	virtual void setMarginsToPrinterMargins();

protected:
	QGridLayout* GroupLayout;
	QString m_pageSize;
	QString m_suffix;
	double m_unitRatio;
	int m_docUnitIndex;
	//! \brief if the outer facing palette points to the facing pages item
	bool facingPages;
};

#endif
