/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <qpen.h>
#include <qtabwidget.h>
#include "guidemanagercore.h"
#include "scpainter.h"
#include "page.h"
#include "fpoint.h"
#include "undomanager.h"
#include "undostate.h"
#include "guidemanager.h"
#include "scribuscore.h"
#include "pagestructs.h"
#include "selection.h"


GuideManagerCore::GuideManagerCore():
	undoManager(UndoManager::instance())
{
	m_horizontalAutoGap = 0.0;
	m_verticalAutoGap = 0.0;
	m_autoRefer = 0;
	m_horizontalAutoCount = 0;
	m_verticalAutoCount = 0;
}

GuideManagerCore::GuideManagerCore(Page *parentPage):
	undoManager(UndoManager::instance())
{
	m_page = parentPage;
	m_horizontalAutoGap = 0.0;
	m_verticalAutoGap = 0.0;
	m_autoRefer = 0;
	m_horizontalAutoCount = 0;
	m_verticalAutoCount = 0;
}

GuideManagerCore::~GuideManagerCore()
{
}

void GuideManagerCore::setPage(Page *p)
{
	m_page = p;
}

void GuideManagerCore::addHorizontal(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			horizontalStdG.append(value);
			if (UndoManager::undoEnabled())
			{
				SimpleState* ss = new SimpleState(Um::AddVGuide, 0, Um::IGuides);
				ss->set("ADD_H", value);
				undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::addHorizontals(Guides values, GuideType type)
{
	switch (type)
	{
		case Standard:
			horizontalStdG += values;
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::addVertical(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			verticalStdG.append(value);
			if (UndoManager::undoEnabled())
			{
				SimpleState* ss = new SimpleState(Um::AddVGuide, 0, Um::IGuides);
				ss->set("ADD_V", value);
				undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::addVerticals(Guides values, GuideType type)
{
	switch (type)
	{
		case Standard:
			verticalStdG += values;
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::deleteHorizontal(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			horizontalStdG.remove(horizontalStdG.find(value));
			if (UndoManager::undoEnabled())
			{
				SimpleState* ss = new SimpleState(Um::DelVGuide, 0, Um::IGuides);
				ss->set("REMOVE_H", value);
				undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::deleteVertical(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			verticalStdG.remove(verticalStdG.find(value));
			if (UndoManager::undoEnabled())
			{
				SimpleState* ss = new SimpleState(Um::DelVGuide, 0, Um::IGuides);
				ss->set("REMOVE_V", value);
				undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

Guides GuideManagerCore::horizontals(GuideType type)
{
	switch (type)
	{
		case Standard:
			return horizontalStdG;
			break;
		case Auto:
			break;
	}
	// just to prevent the compiler warnings
	return horizontalStdG;
}

Guides GuideManagerCore::verticals(GuideType type)
{
	switch (type)
	{
		case Standard:
			return verticalStdG;
			break;
		case Auto:
			break;
	}
	return verticalStdG;
}

double GuideManagerCore::horizontal(uint ix, GuideType type)
{
	switch (type)
	{
		case Standard:
			return horizontalStdG[ix];
			break;
		case Auto:
			break;
	}
	return -1.0; // just for compiler warning
}

double GuideManagerCore::vertical(uint ix, GuideType type)
{
	switch (type)
	{
		case Standard:
			return verticalStdG[ix];
			break;
		case Auto:
			break;
	}
	return -1.0; // just for compiler warning
}

void GuideManagerCore::clearHorizontals(GuideType type)
{
	switch (type)
	{
		case Standard:
			horizontalStdG.clear();
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::clearVerticals(GuideType type)
{
	switch (type)
	{
		case Standard:
			verticalStdG.clear();
			break;
		case Auto:
			break;
	}
}
void GuideManagerCore::moveHorizontal(double from, double to, GuideType type)
{
	switch (type)
	{
		case Standard:
			horizontalStdG.remove(horizontalStdG.find(from));
			horizontalStdG.append(to);
			if (UndoManager::undoEnabled())
			{
				SimpleState* ss = new SimpleState(Um::MoveVGuide, 0, Um::IGuides);
				ss->set("MOVE_H_FROM", from);
				ss->set("MOVE_H_TO", to);
				undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::moveVertical(double from, double to, GuideType type)
{
	switch (type)
	{
		case Standard:
			verticalStdG.remove(verticalStdG.find(from));
			verticalStdG.append(to);
			if (UndoManager::undoEnabled())
			{
				SimpleState* ss = new SimpleState(Um::MoveVGuide, 0, Um::IGuides);
				ss->set("MOVE_V_FROM", from);
				ss->set("MOVE_V_TO", to);
				undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::copy(GuideManagerCore *target)
{
	target->addHorizontals(horizontalStdG, Standard);
	target->addVerticals(verticalStdG, Standard);
	target->setHorizontalAutoCount(m_horizontalAutoCount);
	target->setVerticalAutoCount(m_verticalAutoCount);
	target->setHorizontalAutoGap(m_horizontalAutoGap);
	target->setVerticalAutoGap(m_verticalAutoGap);
	target->setAutoRefer(m_autoRefer);
}

void GuideManagerCore::copy(GuideManagerCore *target, GuideType type)
{
	switch (type)
	{
		case Standard:
			target->addHorizontals(horizontalStdG, Standard);
			target->addVerticals(verticalStdG, Standard);
			break;
		case Auto:
			target->setHorizontalAutoCount(m_horizontalAutoCount);
			target->setVerticalAutoCount(m_verticalAutoCount);
			target->setHorizontalAutoGap(m_horizontalAutoGap);
			target->setVerticalAutoGap(m_verticalAutoGap);
			target->setAutoRefer(m_autoRefer);
			break;
	}
}

void GuideManagerCore::drawPage(ScPainter *p, ScribusDoc *doc, double lineWidth)
{
	Guides::iterator it;
	QColor color(doc->guidesSettings.guideColor);

	// all standard
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
	for (it = verticalStdG.begin(); it != verticalStdG.end(); ++it)
		if ((*it) >= 0 && (*it) <= m_page->width())
			p->drawLine(FPoint((*it), 0), FPoint((*it), m_page->height()));
	for (it = horizontalStdG.begin(); it != horizontalStdG.end(); ++it)
		if ((*it) >= 0 && (*it) <= m_page->height())
			p->drawLine(FPoint(0, (*it)), FPoint(m_page->width(), (*it)));
	// highlight selected standards
	if (ScCore->primaryMainWindow()->guidePalette->tabWidget->currentPageIndex() == 0
		   && m_page->pageNr() == ScCore->primaryMainWindow()->guidePalette->pageNr())
	{
		p->setPen(Qt::red, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
		Guides highlight = ScCore->primaryMainWindow()->guidePalette->selectedVerticals();
		for (it = highlight.begin(); it != highlight.end(); ++it)
			if ((*it) >= 0 && (*it) <= m_page->width())
				p->drawLine(FPoint((*it), 0), FPoint((*it), m_page->height()));
		highlight = ScCore->primaryMainWindow()->guidePalette->selectedHorizontals();
		for (it = highlight.begin(); it != highlight.end(); ++it)
			if ((*it) >= 0 && (*it) <= m_page->height())
				p->drawLine(FPoint(0, (*it)), FPoint(m_page->width(), (*it)));
	}
	// all auto
	if (ScCore->primaryMainWindow()->guidePalette->tabWidget->currentPageIndex() == 1)
		color = Qt::red;
	else
		color = doc->guidesSettings.guideColor;
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
	Guides verticalAutoG = getAutoVerticals();
	for (it = verticalAutoG.begin(); it != verticalAutoG.end(); ++it)
		if ((*it) >= 0 && (*it) <= m_page->width())
			p->drawLine(FPoint((*it), 0), FPoint((*it), m_page->height()));
	Guides horizontalAutoG = getAutoHorizontals();
	for (it = horizontalAutoG.begin(); it != horizontalAutoG.end(); ++it)
		if ((*it) >= 0 && (*it) <= m_page->height())
			p->drawLine(FPoint(0, (*it)), FPoint(m_page->width(), (*it)));
}

bool GuideManagerCore::isMouseOnHorizontal(double low, double high, GuideType type)
{
	Guides tmp;
	Guides::iterator it;

	switch (type)
	{
		case Standard:
			tmp = horizontalStdG;
			break;
		case Auto:
			tmp = getAutoHorizontals();
			break;
	}
	for (it = tmp.begin(); it != tmp.end(); ++it)
	{
		double guideOffset = (*it) + m_page->yOffset();
		if (guideOffset < low && guideOffset > high)
			return true;
	}
	return false;
}

bool GuideManagerCore::isMouseOnVertical(double low, double high, GuideType type)
{
	Guides tmp;
	Guides::iterator it;

	switch (type)
	{
		case Standard:
			tmp = verticalStdG;
			break;
		case Auto:
			tmp = getAutoVerticals();
			break;
	}
	for (it = tmp.begin(); it != tmp.end(); ++it)
	{
		double guideOffset = (*it) + m_page->xOffset();
		if (guideOffset < low && guideOffset > high)
			return true;
	}
	return false;
}

QPair<double, double> GuideManagerCore::topLeft(double x, double y)// const
{
	return QPair<double, double>(closestVertLeft(x), closestHorAbove(y));
}

QPair<double, double> GuideManagerCore::topRight(double x, double y)// const
{
	return QPair<double, double>(closestVertRight(x), closestHorAbove(y));
}

QPair<double, double> GuideManagerCore::bottomLeft(double x, double y)// const
{
	return QPair<double, double>(closestVertLeft(x), closestHorBelow(y));
}

QPair<double, double> GuideManagerCore::bottomRight(double x, double y)// const
{
	return QPair<double, double>(closestVertRight(x), closestHorBelow(y));
}

double GuideManagerCore::closestHorAbove(double y)// const
{
	double closest = 0.0;
	for (uint i = 0; i < horizontalStdG.size(); ++i)
	{
		if (horizontalStdG[i] < y && horizontalStdG[i] > closest)
			closest = horizontalStdG[i];
	}

	Guides horizontalAutoG = getAutoHorizontals();
	for (uint i = 0; i < horizontalAutoG.size(); ++i)
	{
		if (horizontalAutoG[i] < y && horizontalAutoG[i] > closest)
			closest = horizontalAutoG[i];
	}

	if (m_page->Margins.Top < y && m_page->Margins.Top > closest)
		closest = m_page->Margins.Top;
	if (m_page->height() - m_page->Margins.Bottom < y && m_page->height() - m_page->Margins.Bottom > closest)
		closest = m_page->height() - m_page->Margins.Bottom;

	return closest;
}

double GuideManagerCore::closestHorBelow(double y)// const
{
	double closest = m_page->height();
	for (uint i = 0; i < horizontalStdG.size(); ++i)
	{
		if (horizontalStdG[i] > y && horizontalStdG[i] < closest)
			closest = horizontalStdG[i];
	}

	Guides horizontalAutoG = getAutoHorizontals();
	for (uint i = 0; i < horizontalAutoG.size(); ++i)
	{
		if (horizontalAutoG[i] > y && horizontalAutoG[i] < closest)
			closest = horizontalAutoG[i];
	}

	if (m_page->Margins.Top > y && m_page->Margins.Top < closest)
		closest = m_page->Margins.Top;
	if (m_page->height() - m_page->Margins.Bottom > y && m_page->height() - m_page->Margins.Bottom < closest)
		closest = m_page->height() - m_page->Margins.Bottom;

	return closest;
}

double GuideManagerCore::closestVertLeft(double x)// const
{
	double closest = 0.0;
	for (uint i = 0; i < verticalStdG.size(); ++i)
	{
		if (verticalStdG[i] < x && verticalStdG[i] > closest)
			closest = verticalStdG[i];
	}

	Guides verticalAutoG = getAutoVerticals();
	for (uint i = 0; i < verticalAutoG.size(); ++i)
	{
		if (verticalAutoG[i] < x && verticalAutoG[i] > closest)
			closest = verticalAutoG[i];
	}

	if (m_page->Margins.Left < x && m_page->Margins.Left > closest)
		closest = m_page->Margins.Left;
	if (m_page->width() - m_page->Margins.Right < x && m_page->width() - m_page->Margins.Right > closest)
		closest = m_page->width() - m_page->Margins.Right;

	return closest;
}

double GuideManagerCore::closestVertRight(double x)// const
{
	double closest = m_page->width();
	for (uint i = 0; i < verticalStdG.size(); ++i)
	{
		if (verticalStdG[i] > x && verticalStdG[i] < closest)
			closest = verticalStdG[i];
	}

	Guides verticalAutoG = getAutoVerticals();
	for (uint i = 0; i < verticalAutoG.size(); ++i)
	{
		if (verticalAutoG[i] > x && verticalAutoG[i] < closest)
			closest = verticalAutoG[i];
	}

	if (m_page->Margins.Left > x  && m_page->Margins.Left < closest)
		closest = m_page->Margins.Left;
	if (m_page->width() - m_page->Margins.Right > x && m_page->width() - m_page->Margins.Right < closest)
		closest = m_page->width() - m_page->Margins.Right;

	return closest;
}

void GuideManagerCore::readVerticalGuides(const QString guideString, Page *page, GuideType type, bool useOldGuides)
{
	QStringList gVal(QStringList::split(' ', guideString));
	page->guides.clearVerticals(type);
	for (QStringList::Iterator it = gVal.begin(); it != gVal.end(); ++it )
		useOldGuides ?
			page->guides.addHorizontal((*it).toDouble(), type) :
			page->guides.addVertical((*it).toDouble(), type);
}

void GuideManagerCore::readHorizontalGuides(const QString guideString, Page *page, GuideType type, bool useOldGuides)
{
	QStringList gVal(QStringList::split(' ', guideString));
	page->guides.clearHorizontals(type);
	for (QStringList::Iterator it = gVal.begin(); it != gVal.end(); ++it )
		useOldGuides ?
			page->guides.addVertical((*it).toDouble(), type):
			page->guides.addHorizontal((*it).toDouble(), type);
}

QString GuideManagerCore::writeHorizontalGuides(Page *page, GuideType type)
{
	Guides::iterator it;
	QString retval;
	QString tmp;
	Guides tmpGuides = page->guides.horizontals(type);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it)
	{
		tmp = tmp.setNum((*it));
		retval += tmp + " ";
	}
	return retval;
}

QString GuideManagerCore::writeVerticalGuides(Page *page, GuideType type)
{
	Guides::iterator it;
	QString retval;
	QString tmp;
	Guides tmpGuides = page->guides.verticals(type);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it)
	{
		tmp = tmp.setNum((*it));
		retval += tmp + " ";
	}
	return retval;
}

Guides GuideManagerCore::getAutoVerticals()
{
	resetMarginsForPage();

	Guides retval;
	double columnSize;
	int value = m_verticalAutoCount;
	double offset = 0;
	double newPageWidth = locPageWidth;

	if (value == 0)
		return retval;
	++value;

	if (m_autoRefer == 1)
	{
		newPageWidth = locPageWidth - locLeft - locRight;
		offset = locLeft;
	}
	else if (m_autoRefer == 2)
	{
		offset = gx;
		newPageWidth = gw;
	}

	if (m_verticalAutoGap > 0.0)
		columnSize = (newPageWidth - (value - 1) * m_verticalAutoGap) / value;
	else
		columnSize = newPageWidth / value;

	for (int i = 1, gapCount = 0; i < value; ++i)
	{
		if (m_verticalAutoGap > 0.0)
		{
			retval.append(offset + i * columnSize + gapCount * m_verticalAutoGap);
			++gapCount;
			retval.append(offset + i * columnSize + gapCount * m_verticalAutoGap);
		}
		else
			retval.append(offset + columnSize * i);
	}
	return retval;
}

Guides GuideManagerCore::getAutoHorizontals()
{
	resetMarginsForPage();

	Guides retval;
	double rowSize;
	int value = m_horizontalAutoCount;
	double offset = 0;
	double newPageHeight = locPageHeight;

	if (value == 0)
		return retval;
	++value;

	if (m_autoRefer == 1)
	{
		newPageHeight = locPageHeight - locTop - locBottom;
		offset = locTop;
	}
	else if (m_autoRefer == 2)
	{
		offset = gy;
		newPageHeight = gh;
	}

	if (m_horizontalAutoGap > 0.0)
		rowSize = (newPageHeight - (value - 1) * m_horizontalAutoGap) / value;
	else
		rowSize = newPageHeight / value;

	for (int i = 1, gapCount = 0; i < value; ++i)
	{
		if (m_horizontalAutoGap > 0.0)
		{
			retval.append(offset + i * rowSize + gapCount * m_horizontalAutoGap);
			++gapCount;
			retval.append(offset + i * rowSize + gapCount * m_horizontalAutoGap);
		}
		else
			retval.append(offset + rowSize * i);
	}
	return retval;
}

void GuideManagerCore::resetMarginsForPage()
{
	locPageWidth = m_page->width();
	locPageHeight = m_page->height();
	// whatif selection settings
	FPoint selectionTopLeft = FPoint(0, 0);
	FPoint selectionBottomRight = FPoint(0, 0);

	int docSelectionCount = m_page->doc()->m_Selection->count();

	// multiselection
	if (docSelectionCount > 1)
	{
		selectionTopLeft.setXY(m_page->doc()->view()->GroupX - m_page->xOffset(),
							   m_page->doc()->view()->GroupY - m_page->yOffset());
		selectionBottomRight.setXY(m_page->doc()->view()->GroupW,
								   m_page->doc()->view()->GroupH);
	}
	// only one item selected
	else if (docSelectionCount == 1)
	{
		PageItem *currItem = m_page->doc()->m_Selection->itemAt(0);
		selectionTopLeft.setXY(currItem->xPos() - m_page->xOffset(),
							   currItem->yPos() - m_page->yOffset());
		selectionBottomRight.setXY(currItem->width(), currItem->height());
	}

	if (selectionBottomRight != FPoint(0, 0))
	{
		gy = selectionTopLeft.y();
		gx = selectionTopLeft.x();
		gw = selectionBottomRight.x();
		gh = selectionBottomRight.y();
	}

	locTop = m_page->doc()->pageMargins.Top;
	locBottom = m_page->doc()->pageMargins.Bottom;

	PageLocation pageLocation = m_page->doc()->locationOfPage(m_page->doc()->currentPageNumber());
	switch (pageLocation)
	{
		case MiddlePage :
			locLeft = m_page->doc()->pageMargins.Left;
			locRight = m_page->doc()->pageMargins.Left;
			break;
		case LeftPage:
			locLeft = m_page->doc()->pageMargins.Right;
			locRight = m_page->doc()->pageMargins.Left;
			break;
		case RightPage:
			locRight = m_page->doc()->pageMargins.Right;
			locLeft = m_page->doc()->pageMargins.Left;
			break;
		default:
			locRight = 0;
			locLeft = 0;
	}
}
