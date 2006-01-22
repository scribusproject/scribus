/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SHADEBUTTON_H
#define SHADEBUTTON_H

#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include "scribusapi.h"

class SCRIBUS_API ShadeButton : public QToolButton
{

Q_OBJECT

public:
	ShadeButton(QWidget* parent);
	~ShadeButton() {};
	QPopupMenu *FillSh;
	int getValue();
	void setValue(int val);

private slots:
	virtual void setShade(int id);
};
#endif

