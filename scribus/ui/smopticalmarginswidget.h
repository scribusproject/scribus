/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMOPTICALMARGINSWIDGET_H
#define SMOPTICALMARGINSWIDGET_H

#include "opticalmarginswidget.h"

class SMOpticalMarginsWidget : public OpticalMarginsWidget
{
	Q_OBJECT
public:
	SMOpticalMarginsWidget(QWidget *parent);
	~SMOpticalMarginsWidget() {}

	void setOpticalMargins(int o);
	void setOpticalMargins(int o, int ps, bool isParentValue);

	bool useParentValue();

private:
	bool m_hasParent = false;
	bool m_useParentStyle = false;
	int m_pStyle = -1;

private slots:
	void styleChanged();
	void pbPressed();
	void iconSetChange();
};

#endif // SMOPTICALMARGINSWIDGET_H
