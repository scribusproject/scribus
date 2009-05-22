/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMCOLORCOMBO_H
#define SMCOLORCOMBO_H

#include "colorcombo.h"


class SMColorCombo  : public ColorCombo
{
	Q_OBJECT
public:
	SMColorCombo(QWidget *parent);
	SMColorCombo(bool rw, QWidget* parent);
	~SMColorCombo() {};

	void setCurrentItem(int i);
	void setCurrentItem(int i, bool isParentValue);

	void setCurrentText(const QString &s);
	void setCurrentText(const QString &s, bool isParentValue);

	void setParentItem(int i);
	void setParentText(const QString &s);

	bool useParentValue();

private:
	bool   hasParent_;
	bool   useParentValue_;
	int    pItem_;
	QString pText_;
	void setFont(bool wantBold);

private slots:
	void currentChanged();
};

#endif
