/***************************************************************************
 *   Copyright (C) 2009 by Pierre Marchand                                 *
 *   pierre@oep-h.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PAGEITEMPOSITIONSETTER_H
#define PAGEITEMPOSITIONSETTER_H

#include "pageitemsetterbase.h"
#include "ui_positionsetter.h"

#include <QRectF>

class PageItem;

class PageItemPositionSetter : public PageItemSetterBase, private Ui::PositionSetterWidget
{
	Q_OBJECT

	Selection* m_select;

public:
	PageItemPositionSetter(QWidget * parent = 0);

	void changeItem(Selection* sel);
	PageItemSetterBase* clone();

private:
	void refineSetup();

	void createConnections();
	void removeConnections();
	PageItem * connectedItem;
	bool hasConnections;

	void updateCanvas(QRectF rect);

	void adjustBase(double &baseAdjustX , double &baseAdjustY, QRectF bb);
	int rotmode;

private slots:
	void updateValues();
	void captureChanges(double, double);
	void modXpos(double val);
	void modYpos(double val);
	void modWidth(double val);
	void modHeight(double val);

};

#endif // PAGEITEMPOSITIONSETTER_H
