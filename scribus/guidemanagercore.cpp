/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QPen>
#include <QTabWidget>
#include "guidemanagercore.h"
#include "scpainter.h"
#include "page.h"
#include "fpoint.h"
#include "undomanager.h"
#include "undostate.h"
#include "ui/guidemanager.h"
#include "scclocale.h"
#include "scribuscore.h"
#include "pagestructs.h"
#include "selection.h"


GuideManagerCore::GuideManagerCore():
	undoManager(UndoManager::instance()),
	m_page(0),
	m_horizontalAutoGap(0.0),
	m_verticalAutoGap(0.0),
	m_horizontalAutoCount(0),
	m_verticalAutoCount(0),
	m_horizontalAutoRefer(0),
	m_verticalAutoRefer(0)
{
	verticalStdG.clear();
	verticalAutoG.clear();
	horizontalStdG.clear();
	horizontalAutoG.clear();
}

GuideManagerCore::GuideManagerCore(Page *parentPage):
	undoManager(UndoManager::instance()),
	m_page(parentPage),
	m_horizontalAutoGap(0.0),
	m_verticalAutoGap(0.0),
	m_horizontalAutoCount(0),
	m_verticalAutoCount(0),
	m_horizontalAutoRefer(0),
	m_verticalAutoRefer(0)
{
	verticalStdG.clear();
	verticalAutoG.clear();
	horizontalStdG.clear();
	horizontalAutoG.clear();
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
			if (!horizontalStdG.contains(value))
			{
				horizontalStdG.append(value);
				if (UndoManager::undoEnabled())
				{
					SimpleState* ss = new SimpleState(Um::AddVGuide, 0, Um::IGuides);
					ss->set("ADD_H", value);
					undoManager->action(m_page, ss);
				}
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::addHorizontals(Guides values, GuideType type)
{
	Guides::iterator it;
	switch (type)
	{
		case Standard:
			for (it = values.begin(); it != values.end(); ++it)
			{
				if (!horizontalStdG.contains((*it)))
					horizontalStdG.append((*it));
			}
			break;
		case Auto:
			horizontalAutoG.clear();
			for (it = values.begin(); it != values.end(); ++it)
				horizontalAutoG.append((*it));
			break;
	}
}

void GuideManagerCore::addVertical(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			if (!verticalStdG.contains(value))
			{
				verticalStdG.append(value);
				if (UndoManager::undoEnabled())
				{
					SimpleState* ss = new SimpleState(Um::AddVGuide, 0, Um::IGuides);
					ss->set("ADD_V", value);
					undoManager->action(m_page, ss);
				}
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::addVerticals(Guides values, GuideType type)
{
	Guides::iterator it;
	switch (type)
	{
		case Standard:
			for (it = values.begin(); it != values.end(); ++it)
			{
				if (!verticalStdG.contains((*it)))
					verticalStdG.append((*it));
			}
			break;
		case Auto:
			verticalAutoG.clear();
			for (it = values.begin(); it != values.end(); ++it)
				verticalAutoG.append((*it));
			break;
	}
}

void GuideManagerCore::deleteHorizontal(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			horizontalStdG.removeAt(horizontalStdG.indexOf(value));
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
			verticalStdG.removeAt(verticalStdG.indexOf(value));
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
			return horizontalAutoG;
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
			if (undoManager->undoEnabled())
			{
				for (int i = 0; i < horizontalStdG.count(); ++i)
				{
					SimpleState* ss = new SimpleState(Um::DelVGuide, 0, Um::IGuides);
					ss->set("REMOVE_H", horizontalStdG[i]);
					undoManager->action(m_page, ss);
				}
			}
			horizontalStdG.clear();
			break;
		case Auto:
			if (undoManager->undoEnabled())
			{
				SimpleState * ss = new SimpleState(Um::DelHAGuide, 0, Um::IGuides);
				ss->set("REMOVE_HA_GAP", m_horizontalAutoGap);
				ss->set("REMOVE_HA_COUNT", m_horizontalAutoCount);
				ss->set("REMOVE_HA_REFER", m_horizontalAutoRefer);
				undoManager->action(m_page, ss);
			}

			m_horizontalAutoGap = 0.0;
			m_horizontalAutoCount= 0;
			m_horizontalAutoRefer = 0;
			horizontalAutoG.clear();
			break;
	}
}

void GuideManagerCore::clearVerticals(GuideType type)
{
	switch (type)
	{
		case Standard:
			if (undoManager->undoEnabled())
			{
				for (int i = 0; i < verticalStdG.count(); ++i)
				{
					SimpleState* ss = new SimpleState(Um::DelVGuide, 0, Um::IGuides);
					ss->set("REMOVE_V", verticalStdG[i]);
					undoManager->action(m_page, ss);
				}
			}
			verticalStdG.clear();
			break;
		case Auto:
			if (undoManager->undoEnabled())
			{
				SimpleState * ss = new SimpleState(Um::DelVAGuide, 0, Um::IGuides);
				ss->set("REMOVE_VA_GAP", m_verticalAutoGap);
				ss->set("REMOVE_VA_COUNT", m_verticalAutoCount);
				ss->set("REMOVE_VA_REFER", m_verticalAutoRefer);
				undoManager->action(m_page, ss);
			}

			m_verticalAutoGap = 0.0;
			m_verticalAutoCount = 0;
			m_verticalAutoRefer = 0;
			verticalAutoG.clear();
			break;
	}
}
void GuideManagerCore::moveHorizontal(double from, double to, GuideType type)
{
	switch (type)
	{
		case Standard:
			horizontalStdG.removeAt(horizontalStdG.indexOf(from));
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
			verticalStdG.removeAt(verticalStdG.indexOf(from));
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
	copy(target, Standard);
	copy(target, Auto);
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
			target->setHorizontalAutoRefer(m_horizontalAutoRefer);
			target->setVerticalAutoRefer(m_verticalAutoRefer);
			target->addHorizontals(horizontalAutoG, Auto);
			target->addVerticals(verticalAutoG, Auto);
			target->gx = gx;
			target->gy = gy;
			target->gw = gw;
			target->gh = gh;
			break;
	}
}

void GuideManagerCore::drawPage(ScPainter *p, ScribusDoc *doc, double lineWidth)
{
	Guides::iterator it;
	QColor color(doc->guidesSettings.guideColor);

	if (!m_page || ScCore->primaryMainWindow()->guidePalette->pageNr() < 0)
		return;

	// real painting margins including bleeds
	double verticalFrom = 0.0 - doc->bleeds.Top;
	double verticalTo = m_page->height() + doc->bleeds.Bottom;
	double horizontalFrom = 0.0 - doc->bleeds.Left;
	double horizontalTo = m_page->width() + doc->bleeds.Right;

	// all standard
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
	for (it = verticalStdG.begin(); it != verticalStdG.end(); ++it)
// 		if ((*it) >= 0 && (*it) <= m_page->width())
// 			p->drawLine(FPoint((*it), 0), FPoint((*it), m_page->height()));
		p->drawLine(FPoint((*it), verticalFrom), FPoint((*it), verticalTo));
	for (it = horizontalStdG.begin(); it != horizontalStdG.end(); ++it)
// 		if ((*it) >= 0 && (*it) <= m_page->height())
// 			p->drawLine(FPoint(0, (*it)), FPoint(m_page->width(), (*it)));
		p->drawLine(FPoint(horizontalFrom, (*it)), FPoint(horizontalTo, (*it)));
	// highlight selected standards
	if (ScCore->primaryMainWindow()->guidePalette->currentIndex() == 0
		   && m_page->pageNr() == ScCore->primaryMainWindow()->guidePalette->pageNr())
	{
		p->setPen(Qt::red, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
		Guides highlight = ScCore->primaryMainWindow()->guidePalette->selectedVerticals();
		for (it = highlight.begin(); it != highlight.end(); ++it)
// 			if ((*it) >= 0 && (*it) <= m_page->width())
// 				p->drawLine(FPoint((*it), 0), FPoint((*it), m_page->height()));
			p->drawLine(FPoint((*it), verticalFrom), FPoint((*it), verticalTo));
		highlight = ScCore->primaryMainWindow()->guidePalette->selectedHorizontals();
		for (it = highlight.begin(); it != highlight.end(); ++it)
// 			if ((*it) >= 0 && (*it) <= m_page->height())
// 				p->drawLine(FPoint(0, (*it)), FPoint(m_page->width(), (*it)));
			p->drawLine(FPoint(horizontalFrom, (*it)), FPoint(horizontalTo, (*it)));
	}
	// all auto
	if (ScCore->primaryMainWindow()->guidePalette->currentIndex() == 1)
		color = Qt::red;
	else
		color = doc->guidesSettings.guideColor;
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);

	for (it = verticalAutoG.begin(); it != verticalAutoG.end(); ++it)
// 		if ((*it) >= 0 && (*it) <= m_page->width())
// 			p->drawLine(FPoint((*it), 0), FPoint((*it), m_page->height()));
		p->drawLine(FPoint((*it), verticalFrom), FPoint((*it), verticalTo));
	for (it = horizontalAutoG.begin(); it != horizontalAutoG.end(); ++it)
// 		if ((*it) >= 0 && (*it) <= m_page->height())
// 			p->drawLine(FPoint(0, (*it)), FPoint(m_page->width(), (*it)));
		p->drawLine(FPoint(horizontalFrom, (*it)), FPoint(horizontalTo, (*it)));
}

int GuideManagerCore::isMouseOnHorizontal(double low, double high, GuideType type)
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
		double guideOffset = (*it) + m_page->yOffset();
		if (guideOffset < low && guideOffset > high)
			return it - tmp.begin();
	}
	return -1;
}

int GuideManagerCore::isMouseOnVertical(double low, double high, GuideType type)
{
	Guides tmp;
	Guides::iterator it;

	switch (type)
	{
		case Standard:
			tmp = verticalStdG;
			break;
		case Auto:
			tmp = horizontalAutoG;
			break;
	}
	for (it = tmp.begin(); it != tmp.end(); ++it)
	{
		double guideOffset = (*it) + m_page->xOffset();
		if (guideOffset < low && guideOffset > high)
			return it - tmp.begin();
	}
	return -1;
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
	for (int i = 0; i < horizontalStdG.size(); ++i)
	{
		if (horizontalStdG[i] < y && horizontalStdG[i] > closest)
			closest = horizontalStdG[i];
	}

	for (int i = 0; i < horizontalAutoG.size(); ++i)
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
	for (int i = 0; i < horizontalStdG.size(); ++i)
	{
		if (horizontalStdG[i] > y && horizontalStdG[i] < closest)
			closest = horizontalStdG[i];
	}

	for (int i = 0; i < horizontalAutoG.size(); ++i)
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
	for (int i = 0; i < verticalStdG.size(); ++i)
	{
		if (verticalStdG[i] < x && verticalStdG[i] > closest)
			closest = verticalStdG[i];
	}

	for (int i = 0; i < verticalAutoG.size(); ++i)
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
	for (int i = 0; i < verticalStdG.size(); ++i)
	{
		if (verticalStdG[i] > x && verticalStdG[i] < closest)
			closest = verticalStdG[i];
	}

	for (int i = 0; i < verticalAutoG.size(); ++i)
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


void GuideManagerIO::readVerticalGuides(const QString guideString, Page *page, GuideManagerCore::GuideType type, bool useOldGuides)
{
	QStringList gVal(guideString.split(' ', QString::SkipEmptyParts));
	for (QStringList::Iterator it = gVal.begin(); it != gVal.end(); ++it )
		useOldGuides ?
			page->guides.addHorizontal(ScCLocale::toDoubleC((*it)), type) :
			page->guides.addVertical(ScCLocale::toDoubleC((*it)), type);
}

void GuideManagerIO::readHorizontalGuides(const QString guideString, Page *page, GuideManagerCore::GuideType type, bool useOldGuides)
{
	QStringList gVal(guideString.split(' ', QString::SkipEmptyParts));
	for (QStringList::Iterator it = gVal.begin(); it != gVal.end(); ++it )
		useOldGuides ?
			page->guides.addVertical(ScCLocale::toDoubleC((*it)), type):
			page->guides.addHorizontal(ScCLocale::toDoubleC((*it)), type);
}

QString GuideManagerIO::writeHorizontalGuides(Page *page, GuideManagerCore::GuideType type)
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

QString GuideManagerIO::writeVerticalGuides(Page *page, GuideManagerCore::GuideType type)
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

QString GuideManagerIO::writeSelection(Page *page)
{
	return QString("%1 %2 %3 %4").arg(page->guides.gx).arg(page->guides.gy).arg(page->guides.gw).arg(page->guides.gh);
}

void GuideManagerIO::readSelection(const QString guideString, Page *page)
{
	// TODO: examine this check in 134vs.134qt4 - PV
	if (guideString.isNull() || guideString.isEmpty())
		return;
	QStringList gVal(guideString.split(' ', QString::SkipEmptyParts));
	page->guides.gx = ScCLocale::toDoubleC(gVal[0]);
	page->guides.gy = ScCLocale::toDoubleC(gVal[1]);
	page->guides.gw = ScCLocale::toDoubleC(gVal[2]);
	page->guides.gh = ScCLocale::toDoubleC(gVal[3]);
}
