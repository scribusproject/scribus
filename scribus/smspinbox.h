/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSPINBOX_H
#define SMSPINBOX_H

#include <QSpinBox>

class SMSpinBox : public QSpinBox
{
	Q_OBJECT
public:
	SMSpinBox(QWidget *parent);
	~SMSpinBox() {};

	void setValue(int val);
	void setValue(int val, bool isParentVal);

	void clear();

	void setParentValue(int val);

	bool useParentValue();

protected:
	void interpretText();

private:
	bool   hasParent_;
	bool   useParentValue_;
	int    pValue_;
	void setFont(bool wantBold);

private slots:
	void slotValueChanged();
};

#endif
