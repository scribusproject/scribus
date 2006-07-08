/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABDOCUMENT_H
#define TABDOCUMENT_H

#include <qwidget.h>

#include "scribusapi.h"

class MSpinBox;
class PageLayouts;
class MarginWidget;
class QHBoxLayout;
class QLabel;
class QButtonGroup;
class QVBoxLayout;

/*! \brief Document panel for preferences dialog.
This class is inherited from UI base class.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API TabDocument : public QWidget
{
	Q_OBJECT

	public:
		TabDocument(QWidget* parent = 0, const char* name = 0);
		~TabDocument(){};
		void restoreDefaults(struct ApplicationPrefs *prefsData);
		void unitChange(int docUnitIndex);

		MSpinBox* pageWidth;
		MSpinBox* pageHeight;
		PageLayouts* docLayout;
		MarginWidget* marginGroup;
		QButtonGroup* GroupSize;
		QComboBox* pageSizeComboBox;
		QComboBox* pageOrientationComboBox;
		int choosenLayout;
		double Pagebr;
		double Pageho;
		QString prefsPageSizeName;
		QComboBox* unitCombo;
		QGroupBox* GroupAS;
		QSpinBox* ASTime;
		QGroupBox* urGroup;
		QSpinBox* urSpinBox;

	private:
		double unitRatio;
		//Replace with commonstrings when translators have that one done.
		QString customText, customTextTR;

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

	protected:
		QLabel* GZText1;
		QLabel* GZText2;
		QLabel* GZText3;
		QLabel* GZText4;
		QLabel* unitComboText;
		QLabel* ASText;
		QLabel* urLabel;
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
