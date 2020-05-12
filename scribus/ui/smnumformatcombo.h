/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMNUMFORMATCOMBO_H
#define SMNUMFORMATCOMBO_H

#include "numformatcombo.h"

class SMNumFormatCombo  : public NumFormatCombo
{
	Q_OBJECT

public:
	SMNumFormatCombo(QWidget *parent);
	~SMNumFormatCombo() {};

	void setCurrentFormat(NumFormat format);
	void setCurrentFormat(NumFormat format, bool isParentValue);
	
	void setParentFormat(NumFormat format);
	bool useParentFormat();

private:
	bool   m_hasParent = false;
	bool   m_useParentValue = false;
	NumFormat m_parentFormat = Type_1_2_3;

	void setFont(bool wantBold);

private slots:
	void currentChanged();
};

#endif
