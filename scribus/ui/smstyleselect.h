/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSTYLESELECT_H
#define SMSTYLESELECT_H

#include "styleselect.h"

class SMStyleSelect  : public StyleSelect
{
	Q_OBJECT
public:
	SMStyleSelect(QWidget *parent);
	~SMStyleSelect() {}

	void setStyle(int i);
	void setStyle(int i, bool isParentValue);

	void setParentItem(int i);
	bool useParentValue();

private:
	bool   m_hasParent = false;
	bool   m_useParentStyle = false;
	int    m_pStyle = 0;

	QToolButton *parentButton = nullptr;

	void setFont(bool wantBold);

private slots:
	void styleChanged();
	void pbPressed();
};

#endif
