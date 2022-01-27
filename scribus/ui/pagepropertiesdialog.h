/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGEPROPERTIESDIALOG_H
#define PAGEPROPERTIESDIALOG_H

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
In modal mode called from void ScribusMainWindow::changePageProperties() */
class SCRIBUS_API PagePropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/*! \brief GUI setup
	\param parent Scribus main window in this case
	\param doc current document */
	PagePropertiesDialog( QWidget* parent,  ScribusDoc* doc);
	~PagePropertiesDialog() {}

	/*! \brief Returns recomputed "Links" index/value for facing pages.
	\retval int 0 for max item, 1 for min. item++ for middle. Why? */
	int pageOrder() const;
	/*! \brief Current value of orientationQComboBox.
	\retval int index of combobox */
	int getPageOrientation() const;
	/*! \brief Selected page width
	\retval double X */
	double getPageWidth() const;
	/*! \brief Selected page height
	\retval double Y */
	double getPageHeight() const;
	/*! \brief Bool value of moveObjects "property"
	\retval bool true for moving */
	bool getMoveObjects() const;
	/*! \brief Returns prefsPageSizeName value
	\retval QString prefsPageSizeName property */
	QString getPrefsPageSizeName() const;
	/*! \brief Top Margin
	\retval double margin size */
	double top() const;
	/*! \brief Bottom Margin
	\retval double margin size */
	double bottom() const;
	/*! \brief Left Margin
	\retval double margin size */
	double left() const;
	/*! \brief Right Margin
	\retval double margin size */
	double right() const;
	/*! \brief Master Page
	\retval QString Master Page Name */
	QString masterPage() const;
	int getMarginPreset() const;

public slots:
	//! \brief as setOrientation for orientationQComboBox current item
	void setPageSize();
	/*! \brief Sets page size and enables/disables size widgets.
	\param gr A QString with name of the page size or "Custom" */
	void setSize(const QString & gr);
	/*! \brief Sets page orientation and sizes regarding the given value
	\param  ori integer from orientationQComboBox item index */
	void setOrientation(int ori);
	/*! \brief Recompute the page width for current unit.
	It's called from width spinbox.
	\param v new width */
	void setPageWidth(double v);
	/*! \brief Recompute the page height for current unit.
	It's called from width spinbox.
	\param v new height */
	void setPageHeight(double v);

private:
	MarginWidget* marginWidget { nullptr };
	QGroupBox* dsGroupBox7 { nullptr };
	QGroupBox* groupMaster { nullptr };
	QLabel* masterPageLabel { nullptr };
	QComboBox* masterPageComboBox { nullptr };
	ScrSpinBox* widthSpinBox { nullptr };
	ScrSpinBox* heightSpinBox { nullptr };
	QLabel* widthQLabel { nullptr };
	QLabel* heightQLabel { nullptr };
	QLabel* TextLabel1 { nullptr };
	QLabel* TextLabel2 { nullptr };
	QComboBox* sizeQComboBox { nullptr };
	QComboBox* orientationQComboBox { nullptr };
	QComboBox* Links { nullptr };
	QLabel* TextLabel3 { nullptr };
	QCheckBox* moveObjects { nullptr };
	QPushButton* pageFillColorButton { nullptr };
	QPushButton* cancelButton { nullptr };
	QPushButton* okButton { nullptr };
	QGridLayout* dsGroupBox7Layout { nullptr };
	QVBoxLayout* dialogLayout { nullptr };
	QHBoxLayout* okCancelLayout { nullptr };
	QHBoxLayout* masterLayout { nullptr };

	double m_unitRatio { 1.0 };
	double m_pageWidth { 1.0 };
	double m_pageHeight { 1.0 };
	//! \brief Old orientation. Before it's changed via combobox.
	int    oldOri { 0 };
	QString prefsPageSizeName;


};

#endif
