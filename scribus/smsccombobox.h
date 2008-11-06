/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSCCOMBOBOX_H
#define SMSCCOMBOBOX_H

#include "sccombobox.h"


class SMScComboBox  : public ScComboBox
{
	Q_OBJECT
public:
	SMScComboBox(QWidget *parent);
	~SMScComboBox() {};

	void setCurrentItem(int i);
	void setCurrentItem(int i, bool isParentValue);
	
	// Set the current index of a combobox according to the value
	// stored into the data associated to items. Thus, allowing 
	// separation between presentation and data.Isn’t that cool? ;-)
	void setCurrentItemByData(int i);
	void setCurrentItemByData(int i, bool isParentValue);
	void setCurrentItemByData(double d);
	void setCurrentItemByData(double d, bool isParentValue);
	
	int getItemIndexForData(int i);
	int getItemIndexForData(double d);
	
	void setParentItem(int i);

	bool useParentValue();

private:
	bool   hasParent_;
	bool   useParentValue_;
	int    pItem_;
	void setFont(bool wantBold);

private slots:
	void currentChanged();
};

#endif
