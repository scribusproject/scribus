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

#include "pageitempositionsetter.h"
#include "pageitemsettersmanager.h"

#include "pageitem.h"
#include "scribusdoc.h"
#include "units.h"
#include "selection.h"

#include <QDebug>

PageItemPositionSetter::PageItemPositionSetter(QWidget * parent)
		:PageItemSetterBase(parent), m_select(0), hasConnections(false)
{
	setupUi(this);
	setObjectName("XYZ.geometry.position");
	refineSetup();
	activate(false);
	PageItemSettersManager::registerSetter(this);
}

void PageItemPositionSetter::refineSetup()
{
	keepFrameWHRatioButton->setCheckable( true );
	keepFrameWHRatioButton->setAutoRaise( true );
	keepFrameWHRatioButton->setMaximumSize( QSize( 15, 32767 ) );
	keepFrameWHRatioButton->setChecked(true);
}

PageItemSetterBase * PageItemPositionSetter::clone()
{
	return new PageItemPositionSetter(0);
}

void PageItemPositionSetter::changeItem(Selection * sel)
{
	removeConnections();

	m_select = sel;
	if(!m_select || (m_select->count() == 0))
	{
		activate(false);
		xposSpinBox->setValue(0);
		yposSpinBox->setValue(0);
		widthSpinBox->setValue(0);
		heightSpinBox->setValue(0);
		return;
	}
	activate(true);
	PageItem * refItem = m_select->itemAt(0);
	double unitRatio = refItem->m_Doc->unitRatio();
	double maxXYWHVal = 0xFFFFFF * unitRatio;
	double minXYVal = -0xFFFFFF * unitRatio;
	int unitIndex = refItem->m_Doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(unitIndex);
	xposSpinBox->setValues( minXYVal, maxXYWHVal, precision, minXYVal);
	yposSpinBox->setValues( minXYVal, maxXYWHVal, precision, minXYVal);
	widthSpinBox->setValues(unitRatio, maxXYWHVal, precision, unitRatio);
	heightSpinBox->setValues(unitRatio, maxXYWHVal, precision, unitRatio);
	xposSpinBox->setNewUnit(unitIndex);
	yposSpinBox->setNewUnit(unitIndex);
	widthSpinBox->setNewUnit(unitIndex);
	heightSpinBox->setNewUnit(unitIndex);
	rotmode = refItem->m_Doc->RotMode();
	updateValues();
}

void PageItemPositionSetter::createConnections()
{
	if(hasConnections)
		return;
	connectedItem = m_select->itemAt(0);
	connect(connectedItem, SIGNAL(position(double,double)), this, SLOT(captureChanges(double,double)));
	connect(connectedItem, SIGNAL(widthAndHeight(double,double)), this, SLOT(captureChanges(double,double)));
	connect(xposSpinBox, SIGNAL(valueChanged(double)), this, SLOT(modXpos(double)));
	connect(yposSpinBox, SIGNAL(valueChanged(double)), this, SLOT(modYpos(double)));
	connect(heightSpinBox,SIGNAL(valueChanged(double)), this, SLOT(modHeight(double)));
	connect(widthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(modWidth(double)));
	hasConnections = true;
	activate(true);
}

void PageItemPositionSetter::removeConnections()
{
	if(!hasConnections)
		return;
	disconnect(connectedItem, SIGNAL(position(double,double)), this, SLOT(captureChanges(double,double)));
	disconnect(connectedItem, SIGNAL(widthAndHeight(double,double)), this, SLOT(captureChanges(double,double)));
	disconnect(xposSpinBox, SIGNAL(valueChanged(double)), this, SLOT(modXpos(double)));
	disconnect(yposSpinBox, SIGNAL(valueChanged(double)), this, SLOT(modYpos(double)));
	disconnect(heightSpinBox,SIGNAL(valueChanged(double)), this, SLOT(modHeight(double)));
	disconnect(widthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(modWidth(double)));
	hasConnections = false;
	connectedItem = 0;
	activate(false);
}

void PageItemPositionSetter::updateValues()
{
	if(!m_select || (m_select->count() == 0))
		return;
	removeConnections();

	PageItem * Item =  m_select->itemAt(0);
	QRectF r(Item->xPos(), Item->yPos(), Item->width(), Item->height());
	if(m_select->isMultipleSelection())
			r = m_select->getGroupRect();
	double baseAdjustX = 0;
	double baseAdjustY = 0;
	adjustBase(baseAdjustX, baseAdjustY, r);

	qDebug()<<"U"<<r<<baseAdjustX<<baseAdjustY;

	ScribusDoc * doc = m_select->itemAt(0)->m_Doc;
	double displayX = (r.x() - doc->currentPage()->xOffset() + baseAdjustX) * doc->unitRatio();
	double displayY	= (r.y() - doc->currentPage()->yOffset() + baseAdjustY) * doc->unitRatio();
	double displayW = r.width() *  doc->unitRatio();
	double displayH = r.height() * doc->unitRatio();

	xposSpinBox->setValue(displayX);
	yposSpinBox->setValue(displayY);
	widthSpinBox->setValue(displayW);
	heightSpinBox->setValue(displayH);

	createConnections();
}

void PageItemPositionSetter::captureChanges(double, double)
{
	updateValues();
}

void PageItemPositionSetter::modXpos(double val)
{
	if(!m_select || (m_select->count() == 0))
		return;
	// first check if we can move at all, though it should be prevented elsewhere
	foreach(PageItem* pit, m_select->selectionList())
	{
		if(pit->locked())
			return;
	}
	removeConnections();
	ScribusDoc * doc = m_select->itemAt(0)->m_Doc;
	QRectF oldVisualRect = m_select->getVisualGroupRect();
	QRectF oldRect = m_select->getGroupRect();
	double baseAdjustX = 0;
	double baseAdjustY = 0;
	adjustBase(baseAdjustX, baseAdjustY, m_select->getGroupRect());
	double xoffset = doc->currentPage()->xOffset();
	double newX = (xposSpinBox->value() /  doc->unitRatio())  - baseAdjustX;
	double shiftH = newX - (oldRect.x() - xoffset) ;
	foreach(PageItem* pit, m_select->selectionList())
	{
		pit->moveBy(shiftH,0);
	}
	updateCanvas(oldVisualRect.unite(m_select->getVisualGroupRect()));
	createConnections();
}

void PageItemPositionSetter::modYpos(double val)
{
	if(!m_select || (m_select->count() == 0))
		return;
	// first check if we can move at all, though it should be prevented elsewhere
	foreach(PageItem* pit, m_select->selectionList())
	{
		if(pit->locked())
			return;
	}
	removeConnections();
	ScribusDoc * doc = m_select->itemAt(0)->m_Doc;
	QRectF oldVisualRect = m_select->getVisualGroupRect();
	QRectF oldRect = m_select->getGroupRect();
	double baseAdjustX = 0;
	double baseAdjustY = 0;
	adjustBase(baseAdjustX, baseAdjustY, m_select->getGroupRect());
	double yoffset = doc->currentPage()->yOffset();
	double newY = (yposSpinBox->value() /  doc->unitRatio()) - baseAdjustY;
	double shiftV = newY - (oldRect.y() - yoffset);
	foreach(PageItem* pit, m_select->selectionList())
	{
		pit->moveBy(0, shiftV);
	}
	updateCanvas(oldVisualRect.unite(m_select->getVisualGroupRect()));
	createConnections();
}

void PageItemPositionSetter::modWidth(double val)
{
	if(!m_select || (m_select->count() == 0))
		return;
	foreach(PageItem* pit, m_select->selectionList())
	{
		if(pit->locked() || pit->sizeLocked())
			return;
	}
	removeConnections();
	double unitRatio = m_select->itemAt(0)->m_Doc->unitRatio();
	QRectF oldRect = m_select->getVisualGroupRect();
	QRectF r = m_select->getGroupRect();
	double scaleFactor = (widthSpinBox->value() / unitRatio) / r.width() ;
	foreach(PageItem* pit, m_select->selectionList())
	{
		double newW = pit->width() * scaleFactor;
		pit->setWidth(newW);
		if(keepFrameWHRatioButton->isChecked())
		{
			double newH = pit->height() * scaleFactor;
			pit->setHeight(newH);
			heightSpinBox->setValue(newH * pit->m_Doc->unitRatio());
		}
	}
	updateCanvas(oldRect.unite(m_select->getVisualGroupRect()));
	createConnections();
}

void PageItemPositionSetter::modHeight(double val)
{
	if(!m_select || (m_select->count() == 0))
		return;
	foreach(PageItem* pit, m_select->selectionList())
	{
		if(pit->locked() || pit->sizeLocked())
			return;
	}
	removeConnections();
	double unitRatio = m_select->itemAt(0)->m_Doc->unitRatio();
	QRectF oldRect = m_select->getVisualGroupRect();
	QRectF r = m_select->getGroupRect();
	double scaleFactor =  (heightSpinBox->value() / unitRatio) / r.height() ;
	foreach(PageItem* pit, m_select->selectionList())
	{
		double newH = pit->height() * scaleFactor;
		pit->setHeight(newH);
		if(keepFrameWHRatioButton->isChecked())
		{
			double newW = pit->width() * scaleFactor;
			pit->setWidth(newW);
			widthSpinBox->setValue(newW * pit->m_Doc->unitRatio());
		}
	}
	updateCanvas(oldRect.unite(m_select->getVisualGroupRect()));
	createConnections();
}

void PageItemPositionSetter::updateCanvas(QRectF rect)
{
	//	qDebug()<<"updateCanvas"<<rect;
	//	m_Item->setRedrawBounding();

	m_select->itemAt(0)->m_Doc->regionsChanged()->update(rect);
}

void PageItemPositionSetter::adjustBase(double &baseAdjustX , double &baseAdjustY, QRectF bb)
{
	if(rotmode == 1)
		baseAdjustX = bb.width();
	else if(rotmode == 2)
	{
		baseAdjustX =  bb.width() / 2.0;
		baseAdjustY =  bb.height() / 2.0;
	}
	else if(rotmode == 3)
		baseAdjustY = bb.height();
	else if(rotmode == 4)
	{
		baseAdjustX =  bb.width();
		baseAdjustY =  bb.height();
	}

}
