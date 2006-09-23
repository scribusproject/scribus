/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MULTIPLEDUPLICATE
#define MULTIPLEDUPLICATE

#include "multipleduplicatebase.h"
#include "usertaskstructs.h"

class MultipleDuplicate : public MultipleDuplicateBase
{
	Q_OBJECT
	public:
		MultipleDuplicate(int unitIndex, QWidget* parent, const char* name = "" , WFlags fl=0);
		~MultipleDuplicate();
		void getMultiplyData(itemMultipleDuplicateData&);
	protected:
		int m_unitIndex;
		double m_unitRatio;
	protected slots:
		void setCopiesShiftGap(int);
};

#endif
