/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMALIGNVERTICALSELECT_H
#define SMALIGNVERTICALSELECT_H

#include "alignverticalselect.h"

class SMAlignVerticalSelect : public AlignVerticalSelect
{
	Q_OBJECT
public:
	SMAlignVerticalSelect(QWidget *parent);
	~SMAlignVerticalSelect() {}

	void setStyle(int s);
	void setStyle(int s, bool isParentValue);

	void setParentItem(int s);
	bool useParentValue();

	QToolButton* parentButton { nullptr };

private:
	bool   m_hasParent = false;
	bool   m_useParentStyle = false;
	int    m_pStyle = 0;

private slots:
	void styleChanged();
	void pbPressed();
	void iconSetChange();
};

#endif
