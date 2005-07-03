/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ALIGNSELECT_H
#define ALIGNSELECT_H

#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

class AlignSelect : public QButtonGroup
{

Q_OBJECT

public:
	AlignSelect(QWidget* parent);
	~AlignSelect() {};
	void setStyle(int s);
	int getStyle();

    QButtonGroup* GroupAlign;
    QToolButton* TextL;
    QToolButton* TextR;
    QToolButton* TextC;
    QToolButton* TextB;
    QToolButton* TextF;

		
private slots:
	void setTypeStyle(int a);

signals:
	void State(int);

protected:
    QGridLayout* GroupAlignLayout;

};

#endif

