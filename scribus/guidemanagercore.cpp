/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QPen>
#include <QTabWidget>

#include "guidemanagercore.h"

#include "fpoint.h"
#include "scclocale.h"
#include "scpage.h"
#include "scpainter.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "pagestructs.h"
#include "selection.h"
#include "undomanager.h"
#include "undostate.h"
#include "ui/guidemanager.h"

GuideManagerCore::GuideManagerCore():
	m_undoManager(UndoManager::instance())
{
}

GuideManagerCore::GuideManagerCore(ScPage *parentPage):
	m_undoManager(UndoManager::instance()),
	m_page(parentPage)
{
}

void GuideManagerCore::setPage(ScPage *p)
{
	m_page = p;
}

void GuideManagerCore::addHorizontal(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			if (!m_horizontalStdG.contains(value))
			{
				m_horizontalStdG.append(value);
				if (UndoManager::undoEnabled())
				{
					auto* ss = new SimpleState(Um::AddHGuide, nullptr, Um::IGuides);
					ss->set("ADD_H", value);
					m_undoManager->action(m_page, ss);
				}
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
			for (auto guidePos : values)
			{
				if (!m_horizontalStdG.contains(guidePos))
					m_horizontalStdG.append(guidePos);
			}
			break;
		case Auto:
			m_horizontalAutoG.clear();
			for (auto guidePos : values)
				m_horizontalAutoG.append(guidePos);
			break;
	}
}

void GuideManagerCore::addVertical(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			if (!m_verticalStdG.contains(value))
			{
				m_verticalStdG.append(value);
				if (UndoManager::undoEnabled())
				{
					auto* ss = new SimpleState(Um::AddVGuide, nullptr, Um::IGuides);
					ss->set("ADD_V", value);
					m_undoManager->action(m_page, ss);
				}
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
			for (auto guidePos : values)
			{
				if (!m_verticalStdG.contains(guidePos))
					m_verticalStdG.append(guidePos);
			}
			break;
		case Auto:
			m_verticalAutoG.clear();
			for (auto guidePos : values)
				m_verticalAutoG.append(guidePos);
			break;
	}
}

void GuideManagerCore::deleteHorizontal(double value, GuideType type)
{
	switch (type)
	{
		case Standard:
			m_horizontalStdG.removeAt(m_horizontalStdG.indexOf(value));
			if (UndoManager::undoEnabled())
			{
				auto* ss = new SimpleState(Um::DelHGuide, nullptr, Um::IGuides);
				ss->set("REMOVE_H", value);
				m_undoManager->action(m_page, ss);
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
			m_verticalStdG.removeAt(m_verticalStdG.indexOf(value));
			if (UndoManager::undoEnabled())
			{
				auto* ss = new SimpleState(Um::DelVGuide, nullptr, Um::IGuides);
				ss->set("REMOVE_V", value);
				m_undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

Guides GuideManagerCore::horizontals(GuideType type) const
{
	switch (type)
	{
		case Standard:
			return m_horizontalStdG;
		case Auto:
			return m_horizontalAutoG;
	}
	// just to prevent the compiler warnings
	return m_horizontalStdG;
}

Guides GuideManagerCore::verticals(GuideType type) const
{
	switch (type)
	{
		case Standard:
			return m_verticalStdG;
		case Auto:
			return m_verticalAutoG;
	}
	return m_verticalStdG;
}

double GuideManagerCore::horizontal(uint ix, GuideType type) const
{
	switch (type)
	{
		case Standard:
			return m_horizontalStdG[ix];
		case Auto:
			break;
	}
	return -1.0; // just for compiler warning
}

double GuideManagerCore::vertical(uint ix, GuideType type) const
{
	switch (type)
	{
		case Standard:
			return m_verticalStdG[ix];
		case Auto:
			break;
	}
	return -1.0; // just for compiler warning
}

Guides GuideManagerCore::getAutoHorizontals(const ScPage* page) const
{
	Guides guides;
	int value = m_horizontalAutoCount;
	
	if (page == nullptr)
		page = m_page;
	if (page == nullptr)
		return guides;
	if (m_horizontalAutoCount == 0)
		return guides;
	++value;
	double newPageHeight = page->height();
	double offset = 0.0;
	if (m_horizontalAutoRefer == 1)
	{
		newPageHeight = newPageHeight - page->Margins.top() - page->Margins.bottom();
		offset = page->Margins.top();
	}
	else if (m_horizontalAutoRefer == 2)
	{
		if (qRound(page->guides.gy) != 0.0)
		{
			offset = page->guides.gy;
			newPageHeight = page->guides.gh;
		}
	}
	double rowSize;
	if (page->guides.horizontalAutoGap() > 0.0)
		rowSize = (newPageHeight - (value - 1) * page->guides.horizontalAutoGap()) / value;
	else
		rowSize = newPageHeight / value;

	for (int i = 1, gapCount = 0; i < value; ++i)
	{
		if (page->guides.horizontalAutoGap() > 0.0)
		{
			guides.append(offset + i * rowSize + gapCount * page->guides.horizontalAutoGap());
			++gapCount;
			guides.append(offset + i * rowSize + gapCount * page->guides.horizontalAutoGap());
		}
		else
			guides.append(offset + rowSize * i);
	}
	return guides;
}

Guides GuideManagerCore::getAutoVerticals(const ScPage* page) const
{
	Guides guides;
	int value = m_verticalAutoCount;
	
	if (page == nullptr)
		page = m_page;
	if (page == nullptr)
		return guides;
	if (m_verticalAutoCount == 0)
		return guides;
	++value;
	double newPageWidth = page->width();
	double offset = 0.0;
	if (m_verticalAutoRefer == 1)
	{
		newPageWidth = newPageWidth - page->Margins.left() - page->Margins.right();
		offset = page->Margins.left();
	}
	else if (m_verticalAutoRefer == 2)
	{
		if (qRound(page->guides.gx) != 0)
		{
			offset = page->guides.gx;
			newPageWidth = page->guides.gw;
		}
	}
	double columnSize;
	if (page->guides.verticalAutoGap() > 0.0)
		columnSize = (newPageWidth - (value - 1) * page->guides.verticalAutoGap()) / value;
	else
		columnSize = newPageWidth / value;

	for (int i = 1, gapCount = 0; i < value; ++i)
	{
		if (page->guides.verticalAutoGap() > 0.0)
		{
			guides.append(offset + i * columnSize + gapCount * page->guides.verticalAutoGap());
			++gapCount;
			guides.append(offset + i * columnSize + gapCount * page->guides.verticalAutoGap());
		}
		else
			guides.append(offset + columnSize * i);
	}
	return guides;
}

void GuideManagerCore::clearHorizontals(GuideType type)
{
	switch (type)
	{
		case Standard:
			if (UndoManager::undoEnabled())
			{
				for (int i = 0; i < m_horizontalStdG.count(); ++i)
				{
					auto* ss = new SimpleState(Um::DelHGuide, nullptr, Um::IGuides);
					ss->set("REMOVE_H", m_horizontalStdG[i]);
					m_undoManager->action(m_page, ss);
				}
			}
			m_horizontalStdG.clear();
			break;
		case Auto:
			if (UndoManager::undoEnabled())
			{
				auto* ss = new SimpleState(Um::DelHAGuide, nullptr, Um::IGuides);
				ss->set("REMOVE_HA_GAP", m_horizontalAutoGap);
				ss->set("REMOVE_HA_COUNT", m_horizontalAutoCount);
				ss->set("REMOVE_HA_REFER", m_horizontalAutoRefer);
				m_undoManager->action(m_page, ss);
			}

			m_horizontalAutoGap = 0.0;
			m_horizontalAutoCount= 0;
			m_horizontalAutoRefer = 0;
			m_horizontalAutoG.clear();
			break;
	}
}

void GuideManagerCore::clearVerticals(GuideType type)
{
	switch (type)
	{
		case Standard:
			if (UndoManager::undoEnabled())
			{
				for (int i = 0; i < m_verticalStdG.count(); ++i)
				{
					auto* ss = new SimpleState(Um::DelVGuide, nullptr, Um::IGuides);
					ss->set("REMOVE_V", m_verticalStdG[i]);
					m_undoManager->action(m_page, ss);
				}
			}
			m_verticalStdG.clear();
			break;
		case Auto:
			if (UndoManager::undoEnabled())
			{
				auto* ss = new SimpleState(Um::DelVAGuide, nullptr, Um::IGuides);
				ss->set("REMOVE_VA_GAP", m_verticalAutoGap);
				ss->set("REMOVE_VA_COUNT", m_verticalAutoCount);
				ss->set("REMOVE_VA_REFER", m_verticalAutoRefer);
				m_undoManager->action(m_page, ss);
			}

			m_verticalAutoGap = 0.0;
			m_verticalAutoCount = 0;
			m_verticalAutoRefer = 0;
			m_verticalAutoG.clear();
			break;
	}
}
void GuideManagerCore::moveHorizontal(double from, double to, GuideType type)
{
	switch (type)
	{
		case Standard:
			m_horizontalStdG.removeAt(m_horizontalStdG.indexOf(from));
			m_horizontalStdG.append(to);
			if (UndoManager::undoEnabled())
			{
				auto* ss = new SimpleState(Um::MoveHGuide, nullptr, Um::IGuides);
				ss->set("MOVE_H_FROM", from);
				ss->set("MOVE_H_TO", to);
				m_undoManager->action(m_page, ss);
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
			m_verticalStdG.removeAt(m_verticalStdG.indexOf(from));
			m_verticalStdG.append(to);
			if (UndoManager::undoEnabled())
			{
				auto* ss = new SimpleState(Um::MoveVGuide, nullptr, Um::IGuides);
				ss->set("MOVE_V_FROM", from);
				ss->set("MOVE_V_TO", to);
				m_undoManager->action(m_page, ss);
			}
			break;
		case Auto:
			break;
	}
}

void GuideManagerCore::copy(GuideManagerCore *target) const
{
	copy(target, Standard);
	copy(target, Auto);
}

void GuideManagerCore::copy(GuideManagerCore *target, GuideType type) const
{
	switch (type)
	{
		case Standard:
			target->addHorizontals(m_horizontalStdG, Standard);
			target->addVerticals(m_verticalStdG, Standard);
			break;
		case Auto:
			target->setHorizontalAutoCount(m_horizontalAutoCount);
			target->setVerticalAutoCount(m_verticalAutoCount);
			target->setHorizontalAutoGap(m_horizontalAutoGap);
			target->setVerticalAutoGap(m_verticalAutoGap);
			target->setHorizontalAutoRefer(m_horizontalAutoRefer);
			target->setVerticalAutoRefer(m_verticalAutoRefer);
			target->addHorizontals(m_horizontalAutoG, Auto);
			target->addVerticals(m_verticalAutoG, Auto);
			target->gx = gx;
			target->gy = gy;
			target->gw = gw;
			target->gh = gh;
			break;
	}
}

void GuideManagerCore::drawPage(ScPainter *p, ScribusDoc *doc, double lineWidth) const
{
	const GuideManager* guideManager = ScCore->primaryMainWindow()->guidePalette;
	QColor color(doc->guidesPrefs().guideColor);

	if (!m_page || guideManager->pageNr() < 0)
		return;

	// real painting margins including bleeds
	double verticalFrom = 0.0 - doc->bleeds()->top();
	double verticalTo = m_page->height() + doc->bleeds()->bottom();
	double horizontalFrom = 0.0 - doc->bleeds()->left();
	double horizontalTo = m_page->width() + doc->bleeds()->right();

	// all standard
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
	for (auto guide : m_verticalStdG)
		p->drawLine(FPoint(guide, verticalFrom), FPoint(guide, verticalTo));
	for (auto guide : m_horizontalStdG)
		p->drawLine(FPoint(horizontalFrom, guide), FPoint(horizontalTo, guide));

	// highlight selected standards
	if (guideManager->currentIndex() == 0
		   && m_page->pageNr() == guideManager->pageNr())
	{
		p->setPen(Qt::red, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);
		Guides highlight = guideManager->selectedVerticals();
		for (auto guide : highlight)
			p->drawLine(FPoint(guide, verticalFrom), FPoint(guide, verticalTo));
		highlight = guideManager->selectedHorizontals();
		for (auto guide : highlight)
			p->drawLine(FPoint(horizontalFrom, guide), FPoint(horizontalTo, guide));
	}

	// all auto
	if (guideManager->currentIndex() == 1 && guideManager->isVisible())
		color = Qt::red;
	else
		color = doc->guidesPrefs().guideColor;
	p->setPen(color, lineWidth, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin);

	for (auto guide : m_verticalAutoG)
		p->drawLine(FPoint(guide, verticalFrom), FPoint(guide, verticalTo));
	for (auto guide : m_horizontalAutoG)
		p->drawLine(FPoint(horizontalFrom, guide), FPoint(horizontalTo, guide));
}

int GuideManagerCore::isMouseOnHorizontal(double low, double high, GuideType type) const
{
	Guides tmp;

	switch (type)
	{
		case Standard:
			tmp = m_horizontalStdG;
			break;
		case Auto:
			tmp = m_horizontalAutoG;
			break;
	}
	for (auto it = tmp.cbegin(); it != tmp.cend(); ++it)
	{
		double guideOffset = (*it) + m_page->yOffset();
		if (guideOffset < low && guideOffset > high)
			return it - tmp.cbegin();
	}
	return -1;
}

int GuideManagerCore::isMouseOnVertical(double low, double high, GuideType type) const
{
	Guides tmp;

	switch (type)
	{
		case Standard:
			tmp = m_verticalStdG;
			break;
		case Auto:
			tmp = m_horizontalAutoG;
			break;
	}
	for (auto it = tmp.cbegin(); it != tmp.cend(); ++it)
	{
		double guideOffset = (*it) + m_page->xOffset();
		if (guideOffset < low && guideOffset > high)
			return it - tmp.cbegin();
	}
	return -1;
}

QPair<double, double> GuideManagerCore::topLeft(double x, double y) const
{
	return QPair<double, double>(closestVertLeft(x), closestHorAbove(y));
}

QPair<double, double> GuideManagerCore::topRight(double x, double y) const
{
	return QPair<double, double>(closestVertRight(x), closestHorAbove(y));
}

QPair<double, double> GuideManagerCore::bottomLeft(double x, double y) const
{
	return QPair<double, double>(closestVertLeft(x), closestHorBelow(y));
}

QPair<double, double> GuideManagerCore::bottomRight(double x, double y) const
{
	return QPair<double, double>(closestVertRight(x), closestHorBelow(y));
}

double GuideManagerCore::closestHorAbove(double y) const
{
	double closest = 0.0;
	for (auto guidePos : m_horizontalStdG)
	{
		if (guidePos < y && guidePos > closest)
			closest = guidePos;
	}

	for (auto guidePos : m_horizontalAutoG)
	{
		if (guidePos < y && guidePos > closest)
			closest = guidePos;
	}

	if (m_page->Margins.top() < y && m_page->Margins.top() > closest)
		closest = m_page->Margins.top();
	if (m_page->height() - m_page->Margins.bottom() < y && m_page->height() - m_page->Margins.bottom() > closest)
		closest = m_page->height() - m_page->Margins.bottom();

	return closest;
}

double GuideManagerCore::closestHorBelow(double y) const
{
	double closest = m_page->height();
	for (auto guidePos : m_horizontalStdG)
	{
		if (guidePos > y && guidePos < closest)
			closest = guidePos;
	}

	for (auto guidePos : m_horizontalAutoG)
	{
		if (guidePos > y && guidePos < closest)
			closest = guidePos;
	}

	if (m_page->Margins.top() > y && m_page->Margins.top() < closest)
		closest = m_page->Margins.top();
	if (m_page->height() - m_page->Margins.bottom() > y && m_page->height() - m_page->Margins.bottom() < closest)
		closest = m_page->height() - m_page->Margins.bottom();

	return closest;
}

double GuideManagerCore::closestVertLeft(double x) const
{
	double closest = 0.0;
	for (auto guidePos : m_verticalStdG)
	{
		if (guidePos < x && guidePos > closest)
			closest = guidePos;
	}

	for (auto guidePos : m_verticalAutoG)
	{
		if (guidePos < x && guidePos > closest)
			closest = guidePos;
	}

	if (m_page->Margins.left() < x && m_page->Margins.left() > closest)
		closest = m_page->Margins.left();
	if (m_page->width() - m_page->Margins.right() < x && m_page->width() - m_page->Margins.right() > closest)
		closest = m_page->width() - m_page->Margins.right();

	return closest;
}

double GuideManagerCore::closestVertRight(double x) const
{
	double closest = m_page->width();
	for (auto guidePos : m_verticalStdG)
	{
		if (guidePos > x && guidePos < closest)
			closest = guidePos;
	}

	for (auto guidePos : m_verticalAutoG)
	{
		if (guidePos > x && guidePos < closest)
			closest = guidePos;
	}

	if (m_page->Margins.left() > x && m_page->Margins.left() < closest)
		closest = m_page->Margins.left();
	if (m_page->width() - m_page->Margins.right() > x && m_page->width() - m_page->Margins.right() < closest)
		closest = m_page->width() - m_page->Margins.right();

	return closest;
}

void GuideManagerCore::resetSelectionForPage(ScPage* page)
{
	auto doc = page->doc();
	int docSelectionCount = doc->m_Selection->count();

	page->guides.gx = page->guides.gy = page->guides.gw = page->guides.gh = 0.0;

	// multiselection
	if (docSelectionCount > 1)
	{
		double x, y;
		doc->m_Selection->getGroupRect(&x, &y, &page->guides.gw, &page->guides.gh);
		page->guides.gx = x - page->xOffset();
		page->guides.gy = y - page->yOffset();
	}
	// only one item selected
	else if (docSelectionCount == 1)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
		page->guides.gx = currItem->xPos() - page->xOffset();
		page->guides.gy = currItem->yPos() - page->yOffset();
		page->guides.gw = currItem->width();
		page->guides.gh = currItem->height();
	}
}

void GuideManagerIO::readVerticalGuides(const QString& guideString, ScPage *page, GuideManagerCore::GuideType type, bool useOldGuides)
{
	QStringList gVal(guideString.split(' ', Qt::SkipEmptyParts));
	for (const QString& guideStr : gVal)
		useOldGuides ?
			page->guides.addHorizontal(ScCLocale::toDoubleC(guideStr), type) :
			page->guides.addVertical(ScCLocale::toDoubleC(guideStr), type);
}

void GuideManagerIO::readHorizontalGuides(const QString& guideString, ScPage *page, GuideManagerCore::GuideType type, bool useOldGuides)
{
	QStringList gVal(guideString.split(' ', Qt::SkipEmptyParts));
	for (const QString& guideStr : gVal)
		useOldGuides ?
			page->guides.addVertical(ScCLocale::toDoubleC(guideStr), type):
			page->guides.addHorizontal(ScCLocale::toDoubleC(guideStr), type);
}

QString GuideManagerIO::writeHorizontalGuides(const ScPage *page, GuideManagerCore::GuideType type)
{
	QString retval;
	QString tmp;
	Guides tmpGuides = page->guides.horizontals(type);
	for (auto guidePos : tmpGuides)
	{
		tmp.setNum(guidePos);
		retval += tmp + " ";
	}
	return retval;
}

QString GuideManagerIO::writeVerticalGuides(const ScPage *page, GuideManagerCore::GuideType type)
{
	QString retval;
	QString tmp;
	Guides tmpGuides = page->guides.verticals(type);
	for (auto guidePos : tmpGuides)
	{
		tmp.setNum(guidePos);
		retval += tmp + " ";
	}
	return retval;
}

QString GuideManagerIO::writeSelection(const ScPage *page)
{
	return QString("%1 %2 %3 %4").arg(page->guides.gx).arg(page->guides.gy).arg(page->guides.gw).arg(page->guides.gh);
}

void GuideManagerIO::readSelection(const QString& guideString, ScPage *page)
{
	if (guideString.isEmpty())
		return;
	QStringList gVal(guideString.split(' ', Qt::SkipEmptyParts));
	page->guides.gx = ScCLocale::toDoubleC(gVal[0]);
	page->guides.gy = ScCLocale::toDoubleC(gVal[1]);
	page->guides.gw = ScCLocale::toDoubleC(gVal[2]);
	page->guides.gh = ScCLocale::toDoubleC(gVal[3]);
}
