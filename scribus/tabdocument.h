/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABDOCUMENT_H
#define TABDOCUMENT_H

#include <qwidget.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

#include "scribusapi.h"

class QComboBox;
class QSpinBox;
class MSpinBox;
class PageLayouts;
class MarginWidget;
class Q3HBoxLayout;
class QLabel;
class Q3ButtonGroup;
class Q3GroupBox;
class Q3VBoxLayout;
class QCheckBox;
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
	MSpinBox* pageWidth;
	MSpinBox* pageHeight;
	PageLayouts* docLayout;
	MarginWidget* marginGroup;
	Q3ButtonGroup* GroupSize;
	QComboBox* pageSizeComboBox;
	QComboBox* pageOrientationComboBox;
	int choosenLayout;
	double pageW;
	double pageH;
	QString prefsPageSizeName;
	QComboBox* unitCombo;
	Q3GroupBox* GroupAS;
	QSpinBox* ASTime;
	Q3GroupBox* urGroup;
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
	void setPageWidth(int);
	/*!
	\author Franz Schmid
	\brief Preferences (Document / Page Size), sets Page height values
	\param v Height value
	 */
	void setPageHeight(int);
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
	Q3HBoxLayout* sizePagesLayout;
	Q3HBoxLayout* tabLayout_7;
	Q3VBoxLayout* Layout21;
	Q3HBoxLayout* dsLayout4p;
	Q3VBoxLayout* dsLayout4pv;
	Q3VBoxLayout* GroupSizeLayout;
	Q3GridLayout* Layout6;
	Q3HBoxLayout* Layout5_2;
	Q3HBoxLayout* GroupASLayout;
};

#endif
