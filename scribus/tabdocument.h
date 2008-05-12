/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABDOCUMENT_H
#define TABDOCUMENT_H

#include <QWidget>
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QLabel;

#include "scribusapi.h"

class ScrSpinBox;
class PageLayouts;
class MarginWidget;
class ScribusDoc;

/*! \brief Document panel for preferences dialog.
This class is inherited from UI base class.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API TabDocument : public QWidget
{
	Q_OBJECT

public:
	TabDocument(QWidget* parent = 0, const char* name = 0, const bool reform = false);
	~TabDocument(){};
	void restoreDefaults(struct ApplicationPrefs *prefsData);
	void restoreDefaults(ScribusDoc *prefsData);
	void unitChange();
	//! \brief Hide non-needed GUI for ReformDoc
	void hideReform();
	ScrSpinBox* pageWidth;
	ScrSpinBox* pageHeight;
	PageLayouts* docLayout;
	MarginWidget* marginGroup;
	QGroupBox* GroupSize;
	QComboBox* pageSizeComboBox;
	QComboBox* pageOrientationComboBox;
	int choosenLayout;
	double pageW;
	double pageH;
	QString prefsPageSizeName;
	QComboBox* unitCombo;
	QGroupBox* GroupAS;
	QSpinBox* ASTime;
	QGroupBox* urGroup;
	QSpinBox* urSpinBox;
	// reform widgets
	QCheckBox* sizeAllPages;
	QCheckBox* sizeAllMasterPages;
private:
	double unitRatio;
private slots:
	/*!
	\author Franz Schmid
	\brief Preferences (Document / Page Size), sets Page width values
	\param v Width value
	 */
	void setPageWidth(double v);
	/*!
	\author Franz Schmid
	\brief Preferences (Document / Page Size), sets Page height values
	\param v Height value
	 */
	void setPageHeight(double v);
	/*!
	\author Franz Schmid
	\brief Preferences (Document / Page Size), sets Page orientation value and page dimensions
	\param ori Orientation value
	 */
	void setOrien(int ori);
	/*!
	\author Franz Schmid
	\brief Preferences (Document / Page Size), sets Page size values. Connects signals for setting page dimensions.
	\param gr Standard page size value (eg A4)
	 */
	void setSize(const QString & gr);
	void setPageSize();
	void slotUndo(bool isEnabled);

protected:
	QLabel* GZText1;
	QLabel* GZText2;
	QLabel* GZText3;
	QLabel* GZText4;
	QLabel* unitComboText;
	QLabel* ASText;
	QLabel* urLabel;
	QLabel* sizePages;
	QHBoxLayout* sizePagesLayout;
	QHBoxLayout* tabLayout_7;
	QVBoxLayout* Layout21;
	QHBoxLayout* dsLayout4p;
	QVBoxLayout* dsLayout4pv;
	QVBoxLayout* GroupSizeLayout;
	QGridLayout* Layout6;
	QHBoxLayout* Layout5_2;
	QHBoxLayout* GroupASLayout;
};

#endif
