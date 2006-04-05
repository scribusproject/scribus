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
#include "scribus.h"


GuideManagerCore::GuideManagerCore(Page *parentPage):
	undoManager(UndoManager::instance())
{
	page = parentPage;
}

GuideManagerCore::~GuideManagerCore()
{
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
				undoManager->action(page, ss);
			}
			// will be sorted in GUI? qHeapSort(horizontalStdG);
			break;
		case Auto:
			horizontalAutoG.append(value);
			//qHeapSort(horizontalAutoG);
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
			horizontalAutoG += values;
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
				undoManager->action(page, ss);
			}
			break;
		case Auto:
			verticalAutoG.append(value);
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
			verticalAutoG += values;
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
				undoManager->action(page, ss);
			}
			break;
		case Auto:
			horizontalAutoG.remove(horizontalAutoG.find(value));
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
				undoManager->action(page, ss);
			}
			break;
		case Auto:
			verticalAutoG.remove(verticalAutoG.find(value));
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
			return verticalAutoG;
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
			return verticalAutoG;
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
			return horizontalAutoG[ix];
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
			return verticalAutoG[ix];
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
			horizontalAutoG.clear();
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
			verticalAutoG.clear();
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
				undoManager->action(page, ss);
			}
			break;
		case Auto:
			horizontalAutoG.remove(horizontalAutoG.find(from));
			horizontalAutoG.append(to);
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
				undoManager->action(page, ss);
			}
			break;
		case Auto:
			verticalAutoG.remove(verticalAutoG.find(from));
			verticalAutoG.append(to);
			break;
	}
}

void GuideManagerCore::copy(GuideManagerCore *target)
{
	target->addHorizontals(horizontalStdG, Standard);
	target->addHorizontals(horizontalAutoG, Auto);
	target->addVerticals(verticalStdG, Standard);
	target->addVerticals(verticalAutoG, Auto);
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
			target->addHorizontals(horizontalAutoG, Auto);
			target->addVerticals(verticalAutoG, Auto);
			break;
	}
}

void GuideManagerCore::drawPage(ScPainter *p, ScribusDoc *doc, double lineWidth)
{
	// from void ScribusView::DrawPageMarks(ScPainter *p, Page *page, QRect)
	Guides::iterator it;
	QColor color(doc->guidesSettings.guideColor);

	// all standard
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
	for (it = verticalStdG.begin(); it != verticalStdG.end(); ++it)
		if ((*it) >= 0 && (*it) <= page->width())
			p->drawLine(FPoint((*it), 0), FPoint((*it), page->height()));
	for (it = horizontalStdG.begin(); it != horizontalStdG.end(); ++it)
		if ((*it) >= 0 && (*it) <= page->height())
			p->drawLine(FPoint(0, (*it)), FPoint(page->width(), (*it)));
	// highlight selected standards
	if (ScMW->guidePalette->tabWidget->currentPageIndex() == 0
		   && page->pageNr() == ScMW->guidePalette->pageNr())
	{
		p->setPen(Qt::red, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
		Guides highlight = ScMW->guidePalette->selectedVerticals();
		for (it = highlight.begin(); it != highlight.end(); ++it)
			if ((*it) >= 0 && (*it) <= page->width())
				p->drawLine(FPoint((*it), 0), FPoint((*it), page->height()));
		highlight = ScMW->guidePalette->selectedHorizontals();
		for (it = highlight.begin(); it != highlight.end(); ++it)
			if ((*it) >= 0 && (*it) <= page->height())
				p->drawLine(FPoint(0, (*it)), FPoint(page->width(), (*it)));
	}
	// all auto
	if (ScMW->guidePalette->tabWidget->currentPageIndex() == 1)
		color = Qt::red;
	else
		color = doc->guidesSettings.guideColor;
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
	for (it = verticalAutoG.begin(); it != verticalAutoG.end(); ++it)
		if ((*it) >= 0 && (*it) <= page->width())
			p->drawLine(FPoint((*it), 0), FPoint((*it), page->height()));
	for (it = horizontalAutoG.begin(); it != horizontalAutoG.end(); ++it)
		if ((*it) >= 0 && (*it) <= page->height())
			p->drawLine(FPoint(0, (*it)), FPoint(page->width(), (*it)));
	ScMW->guidePalette->setupWidgets();
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
			tmp = horizontalAutoG;
			break;
	}
	for (it = tmp.begin(); it != tmp.end(); ++it)
	{
		double guideOffset = (*it) + page->yOffset();
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
			tmp = verticalAutoG;
			break;
	}
	for (it = tmp.begin(); it != tmp.end(); ++it)
	{
		double guideOffset = (*it) + page->xOffset();
		if (guideOffset < low && guideOffset > high)
			return true;
	}
	return false;
}
