/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSHADEBUTTON_H
#define SMSHADEBUTTON_H

#include "shadebutton.h"


class SMShadeButton  : public ShadeButton
{
	Q_OBJECT
public:
	SMShadeButton(QWidget *parent);
	~SMShadeButton() {};

	void setValue(int i);
	void setValue(int i, bool isParentValue);

	void setParentValue(int i);
	bool useParentValue();

private:
	bool   m_hasParent = false;
	bool   m_useParentValue = false;
	int    m_pValue = 0;

	QString useParentValueText();
	void setFont(bool wantBold);

private slots:
	void currentChanged();
	void slotUseParent();
};

#endif
