/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SHADEBUTTON_H
#define SHADEBUTTON_H

#include <qtoolbutton.h>
#include <qpopupmenu.h>

class ShadeButton : public QToolButton
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

