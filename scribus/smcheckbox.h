/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMCHECKBOX_H
#define SMCHECKBOX_H

#include <QCheckBox>

class SMCheckBox : public QCheckBox
{
	Q_OBJECT
public:
	SMCheckBox(QWidget *parent);
	~SMCheckBox() {};

	void setChecked(bool);
	void setChecked(bool val, bool isParentVal);

	void setParentValue(bool);

	bool useParentValue();

private:
	bool   hasParent_;
	bool   useParentValue_;
	bool   pValue_;
	void setFont(bool wantBold);

private slots:
	void slotStateChanged(int);
};

#endif
