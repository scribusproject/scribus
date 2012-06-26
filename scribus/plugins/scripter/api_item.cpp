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

ItemAPI::ItemAPI(PageItem* it) : QObject(COLLECTOR)
{
	qDebug() << "ItemAPI loaded";
    setObjectName("ItemAPI");
	item = it;
}

QString ItemAPI::name()
{
	return item->itemName();
}

void ItemAPI::setName(QString name)
{
	item->setItemName(name);
}


QString ItemAPI::fillColor()
{
	return item->fillColor();
}

void ItemAPI::setFillColor(QString color)
{
	item->setFillColor(color);
}

QString ItemAPI::lineColor()
{
	return item->lineColor();
}

void ItemAPI::setLineColor(QString color)
{
	item->setLineColor(color);
}


double ItemAPI::fillShade()
{
	return item->fillShade();
}

void ItemAPI::setFillShade(double value)
{
	item->setFillShade(value);
}

double ItemAPI::lineShade()
{
	return item->lineShade();
}

void ItemAPI::setLineShade(double value)
{
	item->setLineShade(value);
}

double ItemAPI::fillTransparency()
{
	return item->fillTransparency();
}

void ItemAPI::setFillTransparency(double value)
{
	item->setFillTransparency(value);
}


double ItemAPI::lineTransparency()
{
	return item->lineTransparency();
}

void ItemAPI::setLineTransparency(double value)
{
	item->setLineTransparency(value);
}

bool ItemAPI::locked()
{
	return item->locked();
}

void ItemAPI::setLocked(bool value)
{
	item->setLocked(value);
}

bool ItemAPI::sizeLocked()
{
	return item->sizeLocked();
}

void ItemAPI::setSizeLocked(bool value)
{
	item->setSizeLocked(value);
}

bool ItemAPI::imageFlippedV()
{
	return item->imageFlippedV();
}

void ItemAPI::setImageFlippedV(bool value)
{
	item->setImageFlippedV(value);
}

bool ItemAPI::imageFlippedH()
{
	return item->imageFlippedH();
}

void ItemAPI::setImageFlippedH(bool value)
{
	item->setImageFlippedH(value);
}

double ItemAPI::lineWidth()
{
	return item->lineWidth();
}

void ItemAPI::setLineWidth(double value)
{
	item->setLineWidth(value);
}

QString ItemAPI::customLineStyle()
{
	return item->customLineStyle();
}

void ItemAPI::setCustomLineStyle(QString name)
{
	item->setCustomLineStyle(name);
}

int ItemAPI::startArrowIndex()
{
	return item->startArrowIndex();
}

void ItemAPI::setStartArrowIndex(int value)
{
	item->setStartArrowIndex(value);
}

int ItemAPI::endArrowIndex()
{
	return item->endArrowIndex();
}

void ItemAPI::setEndArrowIndex(int value)
{
	item->setEndArrowIndex(value);
}

bool ItemAPI::printEnabled()
{
	return item->printEnabled();
}

void ItemAPI::setPrintEnabled(bool value)
{
	item->setPrintEnabled(value);
}

double ItemAPI::xPos()
{
	return item->xPos();
}

void ItemAPI::setXPos(double value)
{
	item->setXPos(value);
}

double ItemAPI::yPos()
{
	return item->yPos();
}

void ItemAPI::setYPos(double value)
{
	item->setYPos(value);
}

double ItemAPI::width()
{
	return item->width();
}

void ItemAPI::setWidth(double value)
{
	item->setWidth(value);
}

double ItemAPI::height()
{
	return item->height();
}

void ItemAPI::setHeight(double value)
{
	item->setHeight(value);
}

double ItemAPI::rotation()
{
	return item->rotation();
}

void ItemAPI::setRotation(double rotation)
{
	item->setRotation(rotation);
}

bool ItemAPI::reversed()
{
	return item->reversed();
}

void ItemAPI::setReversed(bool value)
{
	item->setReversed(value);
}

double ItemAPI::cornerRadius()
{
	return item->cornerRadius();
}

void ItemAPI::setCornerRadius(double value)
{
	item->setCornerRadius(value);
}

double ItemAPI::columnGap()
{
	return item->columnGap();
}

void ItemAPI::setColumnGap(double value)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (value < 0.0)
	{
		RAISE("Column gap out of bounds, must be positive.");
	}
	item->ColGap = ValueToPoint(value);
}

int ItemAPI::columns()
{
	return item->columns();
}

void ItemAPI::setColumns(int value)
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

void ItemAPI::move(double dx, double dy)
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

void ItemAPI::moveAbs(double x, double y)
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

void ItemAPI::resize(double width, double height)
{
	if (!checkHaveDocument())
		return;
	if (item == NULL)
		return;
	ScCore->primaryMainWindow()->doc->SizeItem(ValueToPoint(width), ValueToPoint(height), item);
}

void ItemAPI::rotate(double rot)
{
	if (!checkHaveDocument())
		return;
	if (item == NULL)
		return;
	ScCore->primaryMainWindow()->doc->RotateItem(item->rotation() - rot, item);

}
void ItemAPI::rotateAbs(double rot)
{
	if (!checkHaveDocument())
		return;
	if (item == NULL)
		return;
	ScCore->primaryMainWindow()->doc->RotateItem(rot * -1.0, item);
}


ItemAPI::~ItemAPI()
{
	qDebug() << "ItemAPI deleted";
}


