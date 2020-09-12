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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#ifndef PAGEITEMROTATIONSETTER_H
#define PAGEITEMROTATIONSETTER_H

#include "ui/pageitemsetterbase.h"
#include "ui_rotationsetter.h"

class PageItem;

class PageItemRotationSetter : public PageItemSetterBase, private Ui::RotationWidget
{
	Q_OBJECT

	Selection * m_select;

public:
	PageItemRotationSetter(QWidget* parent = nullptr);

	void changeItem(Selection* sel);
	PageItemSetterBase * clone();

private:
	void refineSetup();

	void createConnections();
	void removeConnections();
	PageItem * connectedItem;

	inline double returnedVal(const double& val) const;
	void rotate(double angle, PageItem* item);

private slots:
	void updateRotation(double val);
	void modRot(double val);
	void rotate90();

};

#endif // PAGEITEMROTATIONSETTER_H
