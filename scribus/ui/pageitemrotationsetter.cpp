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
#include "pageitemrotationsetter.h"
#include "pageitemsettersmanager.h"

#include <cmath>
#include "pageitem.h"
#include "scribusdoc.h"
#include "units.h"
#include "selection.h"

PageItemRotationSetter::PageItemRotationSetter(QWidget * parent )
		:PageItemSetterBase(parent), m_select(0), connectedItem(0)
{
	setupUi(this);
	setObjectName("XYZ.geometry.rotation");
	refineSetup();
	activate(false);
	PageItemSettersManager::registerSetter(this);
}

PageItemSetterBase * PageItemRotationSetter::clone()
{
	return new PageItemRotationSetter(0);
}

void PageItemRotationSetter::refineSetup()
{
	rotationSpinBox->setNewUnit(SC_DEGREES);
	rotationSpinBox->setValues( 0, 359.99, 1, 0);
	rotationSpinBox->setWrapping(true);
}

void PageItemRotationSetter::changeItem(Selection * sel)
{
	removeConnections();
	m_select = sel;
	// We do not allow to set rotation for multis.
	if(!m_select || (m_select->count() != 1))
	{
		activate(false);
		return;
	}
	activate(true);
	updateRotation(m_select->itemAt(0)->rotation());
}

void PageItemRotationSetter::createConnections()
{

	if(connectedItem != 0)
		return;
	connectedItem = m_select->itemAt(0);
	connect(connectedItem, SIGNAL(rotation(double)), this, SLOT(updateRotation(double)));
	connect(rotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(modRot(double)));
	connect(rot90Button, SIGNAL(clicked()), this, SLOT(rotate90()));
	activate(true);
}

void PageItemRotationSetter::removeConnections()
{
	if(0 == connectedItem)
		return;
	disconnect(connectedItem, SIGNAL(rotation(double)), this, SLOT(updateRotation(double)));
	disconnect(rotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(modRot(double)));
	disconnect(rot90Button, SIGNAL(clicked()), this, SLOT(rotate90()));
	connectedItem = 0;
	activate(false);
}


void PageItemRotationSetter::updateRotation(double val)
{
	if(!m_select || (m_select->count() != 1))
		return;
	removeConnections();
	rotationSpinBox->setValue(returnedVal(val));
	createConnections();
}

void PageItemRotationSetter::modRot(double val)
{
	if(!m_select || (m_select->count() != 1))
		return;
	removeConnections();
	PageItem * Item = m_select->itemAt(0);
	QRectF oldR = Item->getVisualBoundingRect();
//	Item->setRotation(returnedVal(val));
	rotate(returnedVal(val), Item);
	Item->m_Doc->regionsChanged()->update(oldR.unite(Item->getBoundingRect()));
	createConnections();

}

void PageItemRotationSetter::rotate90()
{
	if(!m_select || (m_select->count() != 1))
		return;
	removeConnections();
	PageItem * Item = m_select->itemAt(0);
	QRectF oldR = Item->getVisualBoundingRect();
	double newRot = (rotationSpinBox->value() + 90.0) ;
	if(newRot > 360.0)
		newRot -= 360.0;
//	Item->setRotation(returnedVal(newRot));
	rotate(returnedVal(newRot), Item);
	rotationSpinBox->setValue(newRot);
	Item->m_Doc->regionsChanged()->update(oldR.unite(Item->getBoundingRect()));
	createConnections();
}

double PageItemRotationSetter::returnedVal(const double& val) const
{
	double rr = val;
	if (rr > 0)
		rr = 360.0 - rr;
	return fabs(rr);
}

void PageItemRotationSetter::rotate(double angle, PageItem * item)
{
	int rotMode = item->m_Doc->RotMode();
	if (rotMode != 0)
	{
		QTransform ma;
		ma.translate(item->xPos(), item->yPos());
		ma.scale(1, 1);
		ma.rotate(item->rotation());
		double ro = angle - item->rotation();
		FPoint n(0,0);
		switch (rotMode)
		{
		case 2:
			ma.translate(item->width()/2.0, item->height()/2.0);
			n = FPoint(-item->width()/2.0, -item->height()/2.0);
			break;
		case 4:
			ma.translate(item->width(), item->height());
			n = FPoint(-item->width(), -item->height());
			break;
		case 3:
			ma.translate(0, item->height());
			n = FPoint(0, -item->height());
			break;
		case 1:
			ma.translate(item->width(), 0);
			n = FPoint(-item->width(), 0);
			break;
		}
		ma.rotate(ro);
		double x = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		double y = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();

		item->setRotation(angle);
		item->moveBy(x - item->xPos(), y - item->yPos());
	}
	else
		item->setRotation(angle);
}
