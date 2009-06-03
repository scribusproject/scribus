/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSCRSPINBOX_H
#define SMSCRSPINBOX_H

#include "scrspinbox.h"


class SMScrSpinBox : public ScrSpinBox
{
	Q_OBJECT
public:
	SMScrSpinBox(QWidget *pa, int s);
	SMScrSpinBox(double minValue, double maxValue, QWidget *pa, int s);
	SMScrSpinBox(QWidget *parent, const char * name = 0);
	~SMScrSpinBox() {};

	void setValue(double val);
	void setValue(double val, bool isParentVal);

	void clear();

	void setParentValue(double val);

	bool useParentValue();

protected:
	void interpretText();

private:
	bool   hasParent_;
	bool   useParentValue_;
	double pValue_;
	void setFont(bool wantBold);

private slots:
	void slotValueChanged();
};

#endif
