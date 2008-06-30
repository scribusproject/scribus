/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MARGINDIALOG_H
#define MARGINDIALOG_H

#include "scribusapi.h"
#include <QDialog>

class QGroupBox;
class QLabel;
class QPushbutton;
class QComboBox;
class QCheckBox;
class QLabel;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class ScrSpinBox;
class MarginWidget;
class ScribusDoc;

/*! \brief A dialog to setup the existing document margins.
In modal mode called from void ScribusMainWindow::changePageMargins() */
class SCRIBUS_API MarginDialog : public QDialog
{
	Q_OBJECT

public:
	/*! \brief GUI setup
	\param parent Scribus main window in this case
	\param doc current document */
	MarginDialog( QWidget* parent,  ScribusDoc* doc);
	~MarginDialog() {};

	/*! \brief Returns recomputed "Links" index/value for facing pages.
	\retval int 0 for max item, 1 for min. item++ for middle. Why? */
	int pageOrder();
	/*! \brief Current value of orientationQComboBox.
	\retval int index of combobox */
	int getPageOrientation();
	/*! \brief Selected page width
	\retval double X */
	double getPageWidth();
	/*! \brief Selected page height
	\retval double Y */
	double getPageHeight();
	/*! \brief Bool value of moveObjects "property"
	\retval bool true for moving */
	bool getMoveObjects();
	/*! \brief Returns prefsPageSizeName value
	\retval QString prefsPageSizeName property */
	QString getpPrefsPageSizeName();
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
	/*! \brief Master Page
	\retval QString Master Page Name */
	QString masterPage();
	int getMarginPreset();

public slots:
	//! \brief as setOrien for orientationQComboBox current item
	virtual void setPageSize();
	/*! \brief Sets page size and enables/disables size widgets.
	\param gr A QString with name of the page size or "Custom" */
	virtual void setSize(const QString & gr);
	/*! \brief Sets page orientation and sizes regarding the given value
	\param  ori integer from orientationQComboBox item index */
	virtual void setOrien(int ori);
	/*! \brief Recompute the page width for current unit.
	It's called from width spinbox.
	\param v new width */
	virtual void setPageWidth(double v);
	/*! \brief Recompute the page height for current unit.
	It's called from width spinbox.
	\param v new height */
	virtual void setPageHeight(double v);

private:
	MarginWidget* GroupRand;
	QGroupBox* dsGroupBox7;
	QGroupBox* groupMaster;
	QLabel* masterPageLabel;
	QComboBox* masterPageComboBox;
	ScrSpinBox* widthSpinBox;
	ScrSpinBox* heightSpinBox;
	QLabel* widthQLabel;
	QLabel* heightQLabel;
	QLabel* TextLabel1;
	QLabel* TextLabel2;
	QComboBox* sizeQComboBox;
	QComboBox* orientationQComboBox;
	QComboBox* Links;
	QLabel* TextLabel3;
	QCheckBox* moveObjects;
	QPushButton* cancelButton;
	QPushButton* okButton;
	double unitRatio;
	double pageWidth;
	double pageHeight;
	//! \brief Old orientation. Before it's changed via combobox.
	int oldOri;
	QString prefsPageSizeName;
	QGridLayout* dsGroupBox7Layout;
	QVBoxLayout* dialogLayout;
	QHBoxLayout* okCancelLayout;
	QHBoxLayout* masterLayout;


};

#endif
