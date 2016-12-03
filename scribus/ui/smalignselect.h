/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMALIGNSELECT_H
#define SMALIGNSELECT_H

#include "alignselect.h"


class SMAlignSelect : public AlignSelect
{
	Q_OBJECT
public:
	SMAlignSelect(QWidget *parent);
	~SMAlignSelect() {}

	void setStyle(int a, int d);
	void setStyle(int a, int d, bool isParentValue);

	void setParentItem(int a, int d);

	bool useParentValue();

	QToolButton *parentButton;

private:
	bool   m_hasParent;
	bool   m_useParentStyle;
	int    m_pStyle;
	int    m_pDirection;
	void setFont(bool wantBold);

private slots:
	void styleChanged();
	void pbPressed();
};

#endif
