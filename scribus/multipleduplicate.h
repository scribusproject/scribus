/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MULTIPLEDUPLICATE
#define MULTIPLEDUPLICATE

#include "ui_multipleduplicate.h"
struct ItemMultipleDuplicateData;

class MultipleDuplicate : public QDialog, Ui::MultipleDuplicate
{
	Q_OBJECT
	public:
		MultipleDuplicate(int unitIndex, QWidget* parent, Qt::WFlags fl=0);
		~MultipleDuplicate();
		void getMultiplyData(ItemMultipleDuplicateData&);
	protected:
		int m_unitIndex;
		double m_unitRatio;
	protected slots:
		void setCopiesShift();
		void setCopiesGap();
};

#endif
