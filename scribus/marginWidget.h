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

#include <QComboBox>
#include <QTabWidget>
#include "linkbutton.h"
class QCheckBox;
class QPushButton;
class QLabel;
class QGridLayout;
class QHBoxLayout;
class ScrSpinBox;


/*! \brief This is inherited QComboBox widget used in MarginWidget as "Preset List".
It contains functionality for margins setting in various ways.
\author Petr Vanek, <petr@yarpen.cz>
*/
class SCRIBUS_API PresetLayout: public QComboBox
{
	Q_OBJECT

public:
	/*! \brief QComboBox like constructor. Values/names are set here. Tooltip etc. too. */
	PresetLayout(QWidget *parent = 0);
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
class SCRIBUS_API MarginWidget : public QTabWidget
{
	Q_OBJECT

public:
	/*! \brief It creates an user interface for base margin settings for the document.
	\param parent Parent widget (used as standard in Qt)
	\param title Title of the Group box
	\param margs A reference to the margins in the preferences.
	\param unitIndex and index of the unit combobox
	\param showChangeAll show "apply to all pages" related items
	*/
	MarginWidget( QWidget* parent, QString title, const MarginStruct* margs, int unitIndex, bool showChangeAll=false, bool showBleeds=true);
	~MarginWidget() {};
	/*! \brief Setup the labels by facing pages option */
	void setFacingPages(bool facing, int pagetype = 0);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by width and height */
	void setPageWidthHeight(double width, double height);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by width */
	void setPageWidth(double width);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by height */
	void setPageHeight(double heigth);
	/*! \brief Setup the widgets by new options */
	void setNewUnit(int newUnitIndex);
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
	\param m MarginStruct 
	*/
	void setNewMargins(MarginStruct& m);
	
	/*! \brief Top Bleed
	\retval double bleed size */
	double topBleed();
	/*! \brief Bottom Bleed
	\retval double bleed size */
	double bottomBleed();
	/*! \brief Left Bleed
	\retval double bleed size */
	double leftBleed();
	/*! \brief Right Bleed
	\retval double bleed size */
	double rightBleed();

	/*! \brief set new bleed values for the dialog
	\param b MarginStruct
	*/
	void setNewBleeds(MarginStruct& b);
	
	/*! \brief Return marginsForAllPages property
	\retval bool true to apply for all pages */
	bool getMarginsForAllPages();
	bool getMarginsForAllMasterPages();

private:
	/*! Tabs */
	QWidget *marginPage;
	QWidget* bleedPage;
	/*! Spinboxes */
	ScrSpinBox* topR;
	ScrSpinBox* bottomR;
	ScrSpinBox* rightR;
	ScrSpinBox* leftR;
	ScrSpinBox* BleedBottom;
	ScrSpinBox* BleedLeft;
	ScrSpinBox* BleedRight;
	ScrSpinBox* BleedTop;
	PresetLayout* presetCombo;
	/*! Labels */
	QLabel* lText;
	QLabel* rText;
	QLabel* tText;
	QLabel* bText;
	QLabel* presetLabel;
	QLabel* marginsForPages;
	QLabel* BleedTxt1;
	QLabel* BleedTxt2;
	QLabel* BleedTxt3;
	QLabel* BleedTxt4;
	QCheckBox* marginsForAllPages;
	QCheckBox* marginsForAllMasterPages;
	QPushButton* usePrinterMarginsButton;
	LinkButton* linkBleeds;
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
	void ToggleKette();
	void changeBleeds();
	
protected slots:	
	virtual void setMarginsToPrinterMargins();

protected:
	QGridLayout* GroupLayout;
	QHBoxLayout* marginsForPagesLayout;
	QGridLayout* BleedGroupLayout;
	QString m_pageSize;
	int m_unitIndex;
	double m_unitRatio;
	//! \brief if the outer facing palette points to the facing pages item
	bool facingPages;
	bool useBleeds;
	int pageType;
};

#endif
