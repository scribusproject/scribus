/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_item.h"
#include "utils.h"
#include "undomanager.h"
#include "selection.h"

ScribusItem::ScribusItem(PageItem* it) : QObject(COLLECTOR)
{
	qDebug() << "ItemAPI loaded";
	setObjectName("item");
	item = it;
}

QString ScribusItem::itemName()
{
	return item->itemName();
}

void ScribusItem::setItemName(QString name)
{
	item->setItemName(name);
}


QString ScribusItem::fillColor()
{
	return item->fillColor();
}

void ScribusItem::setFillColor(QString color)
{
	item->setFillColor(color);
}

QString ScribusItem::lineColor()
{
	return item->lineColor();
}

void ScribusItem::setLineColor(QString color)
{
	item->setLineColor(color);
}


double ScribusItem::fillShade()
{
	return item->fillShade();
}

void ScribusItem::setFillShade(double value)
{
	item->setFillShade(value);
}

double ScribusItem::lineShade()
{
	return item->lineShade();
}

void ScribusItem::setLineShade(double value)
{
	item->setLineShade(value);
}

double ScribusItem::fillTransparency()
{
	return item->fillTransparency();
}

void ScribusItem::setFillTransparency(double value)
{
	item->setFillTransparency(value);
}


double ScribusItem::lineTransparency()
{
	return item->lineTransparency();
}

void ScribusItem::setLineTransparency(double value)
{
	item->setLineTransparency(value);
}

bool ScribusItem::locked()
{
	return item->locked();
}

void ScribusItem::setLocked(bool value)
{
	item->setLocked(value);
}

bool ScribusItem::sizeLocked()
{
	return item->sizeLocked();
}

void ScribusItem::setSizeLocked(bool value)
{
	item->setSizeLocked(value);
}

bool ScribusItem::imageFlippedV()
{
	return item->imageFlippedV();
}

void ScribusItem::setImageFlippedV(bool value)
{
	item->setImageFlippedV(value);
}

bool ScribusItem::imageFlippedH()
{
	return item->imageFlippedH();
}

void ScribusItem::setImageFlippedH(bool value)
{
	item->setImageFlippedH(value);
}

double ScribusItem::lineWidth()
{
	return item->lineWidth();
}

void ScribusItem::setLineWidth(double value)
{
	item->setLineWidth(value);
}

QString ScribusItem::customLineStyle()
{
	return item->customLineStyle();
}

void ScribusItem::setCustomLineStyle(QString name)
{
	item->setCustomLineStyle(name);
}

int ScribusItem::startArrowIndex()
{
	return item->startArrowIndex();
}

void ScribusItem::setStartArrowIndex(int value)
{
	item->setStartArrowIndex(value);
}

int ScribusItem::endArrowIndex()
{
	return item->endArrowIndex();
}

void ScribusItem::setEndArrowIndex(int value)
{
	item->setEndArrowIndex(value);
}

bool ScribusItem::printEnabled()
{
	return item->printEnabled();
}

void ScribusItem::setPrintEnabled(bool value)
{
	item->setPrintEnabled(value);
}

double ScribusItem::xPos()
{
	return item->xPos();
}

void ScribusItem::setXPos(double value)
{
	item->setXPos(value);
}

double ScribusItem::yPos()
{
	return item->yPos();
}

void ScribusItem::setYPos(double value)
{
	item->setYPos(value);
}

double ScribusItem::width()
{
	return item->width();
}

void ScribusItem::setWidth(double value)
{
	item->setWidth(value);
}

double ScribusItem::height()
{
	return item->height();
}

void ScribusItem::setHeight(double value)
{
	item->setHeight(value);
}

double ScribusItem::rotation()
{
	return item->rotation();
}

void ScribusItem::setRotation(double rotation)
{
	item->setRotation(rotation);
}

bool ScribusItem::reversed()
{
	return item->reversed();
}

void ScribusItem::setReversed(bool value)
{
	item->setReversed(value);
}

double ScribusItem::cornerRadius()
{
	return item->cornerRadius();
}

void ScribusItem::setCornerRadius(double value)
{
	item->setCornerRadius(value);
}

double ScribusItem::columnGap()
{
	return item->columnGap();
}

void ScribusItem::setColumnGap(double value)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (value < 0.0)
	{
		RAISE("Column gap out of bounds, must be positive.");
	}
	item->ColGap = ValueToPoint(value);
}

int ScribusItem::columns()
{
	return item->columns();
}

void ScribusItem::setColumns(int value)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (value < 1)
	{
		RAISE("Column count out of bounds, must be > 1.");
	}
	item->Cols = value;
}


/**
 * Methods
 */

void ScribusItem::move(double dx, double dy)
{
	if (!checkHaveDocument())
		return;
	if (item==NULL)
		return;
	// Grab the old selection - but use it only where is there any
	Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	// Clear the selection
	ScCore->primaryMainWindow()->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	ScCore->primaryMainWindow()->view->SelectItem(item);
	// Move the item, or items
	if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
	{
		ScCore->primaryMainWindow()->view->startGroupTransaction(Um::Move, "", Um::IMove);
		ScCore->primaryMainWindow()->doc->moveGroup(ValueToPoint(dx), ValueToPoint(dy));
		ScCore->primaryMainWindow()->view->endGroupTransaction();
	}
	else
	{
		ScCore->primaryMainWindow()->doc->MoveItem(ValueToPoint(dx), ValueToPoint(dy), item);
	}
	// Now restore the selection.
	ScCore->primaryMainWindow()->view->Deselect();
	if (hadOrigSelection)
		*ScCore->primaryMainWindow()->doc->m_Selection=tempSelection;

}

void ScribusItem::moveAbs(double x, double y)
{
	if (!checkHaveDocument())
		return;
	if (item == NULL)
		return;
	// Grab the old selection - but use it only where is there any
	Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	// Clear the selection
	ScCore->primaryMainWindow()->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	ScCore->primaryMainWindow()->view->SelectItem(item);
	// Move the item, or items
	if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
	{
		ScCore->primaryMainWindow()->view->startGroupTransaction(Um::Move, "", Um::IMove);
		double x2, y2, w, h;
		ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
		ScCore->primaryMainWindow()->doc->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
		ScCore->primaryMainWindow()->view->endGroupTransaction();
	}
	else
		ScCore->primaryMainWindow()->doc->MoveItem(pageUnitXToDocX(x) - item->xPos(), pageUnitYToDocY(y) - item->yPos(), item);
	// Now restore the selection.
	ScCore->primaryMainWindow()->view->Deselect();
	if (hadOrigSelection)
		*ScCore->primaryMainWindow()->doc->m_Selection=tempSelection;
}

void ScribusItem::resize(double width, double height)
{
	if (!checkHaveDocument())
		return;
	if (item == NULL)
		return;
	ScCore->primaryMainWindow()->doc->SizeItem(ValueToPoint(width), ValueToPoint(height), item);
}

void ScribusItem::rotate(double rot)
{
	if (!checkHaveDocument())
		return;
	if (item == NULL)
		return;
	ScCore->primaryMainWindow()->doc->RotateItem(item->rotation() - rot, item);

}
void ScribusItem::rotateAbs(double rot)
{
	if (!checkHaveDocument())
		return;
	if (item == NULL)
		return;
	ScCore->primaryMainWindow()->doc->RotateItem(rot * -1.0, item);
}


ScribusItem::~ScribusItem()
{
	qDebug() << "ItemAPI deleted";
}


