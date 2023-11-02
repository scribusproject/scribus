/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "canvasgesture_rulermove.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "appmodes.h"
#include "guidemanagercore.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "ui/guidemanager.h"
#include "iconmanager.h"


RulerGesture::RulerGesture(ScribusView *view, RulerGesture::Mode mode) :
	CanvasGesture(view),
	m_mode(mode),
	m_ScMW(m_view->m_ScMW)
{
}


void RulerGesture::drawControls(QPainter* p)
{
	int page = -1;
	//This is !null where we've entered the RulerGesture::mouseMoveEvent
	if (!m_mousePoint.isNull())
		page = m_doc->OnPage(m_mousePoint.x(), m_mousePoint.y());
	if (page == -1)
		return;
	ScPage* dragToPage = m_doc->Pages->at(page);
	if (!dragToPage)
		return;
	if (m_haveGuide)
		dragToPage = m_doc->Pages->at(m_page);
	QColor color(m_doc->guidesPrefs().guideColor);
	p->save();
	QPoint pageOrigin = m_canvas->canvasToLocal(QPointF(dragToPage->xOffset(), dragToPage->yOffset()));
	QSize pageSize = (QSizeF(dragToPage->width(), dragToPage->height()) * m_canvas->scale()).toSize();
	switch (m_mode)
	{
		case HORIZONTAL:
			p->setPen(QPen(color, 1.0, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(QPoint(pageOrigin.x(), m_xy.y()), QPoint(pageOrigin.x() + pageSize.width(), m_xy.y()));
			break;
		case VERTICAL:
			p->setPen(QPen(color, 1.0, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(QPoint(m_xy.x(), pageOrigin.y()), QPoint(m_xy.x(), pageOrigin.y() + pageSize.height()));
			break;
		case ORIGIN:
			p->setPen(QPen(color, 1.0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(QPoint(m_xy.x(), 0), QPoint(m_xy.x(), m_canvas->height()));
			p->drawLine(QPoint(0, m_xy.y()), QPoint(m_canvas->width(), m_xy.y()));
			break;
	}
	p->restore();
}

void RulerGesture::clear()
{
	m_haveGuide = false;
}

void RulerGesture::prepare(Mode mode)
{
	m_haveGuide = false;
	m_mode = mode;
}

void RulerGesture::activate(bool fromGesture)
{
	CanvasGesture::activate(fromGesture);

	m_haveCursor = (qApp->overrideCursor() != nullptr);
	if ( (!fromGesture) && qApp->overrideCursor())
	{
		m_haveCursor = true;
		m_cursor = *(qApp->overrideCursor());
	}
	else
	{
		m_haveCursor = false;
	}
	switch (m_mode)
	{
		case HORIZONTAL:
			qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
			break;
		case VERTICAL:
			qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
			break;
		case ORIGIN:
			qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
			break;
	}
	emit guideInfo(m_mode, m_guide);
}

void RulerGesture::deactivate(bool fromGesture)
{
	if (m_haveCursor)
		qApp->changeOverrideCursor(m_cursor);
	m_haveGuide = false;
	m_mousePoint = FPoint();

	CanvasGesture::deactivate(fromGesture);
}


bool RulerGesture::mouseHitsGuide(const FPoint& mousePointDoc)
{
	const int page = m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y());
	if ((m_doc->guidesPrefs().guidesShown) && (!m_doc->GuideLock) && page >= 0)
	{
		double grabRadScale = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
		int index = m_doc->Pages->at(page)->guides.isMouseOnVertical(mousePointDoc.x() + grabRadScale, mousePointDoc.x() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			m_mode = VERTICAL;
			m_haveGuide = true;
			m_guide = m_doc->Pages->at(page)->guides.vertical(index, GuideManagerCore::Standard);
			m_currentGuide = m_guide;
			m_page = page;
			return true;
		}
		index = m_doc->Pages->at(page)->guides.isMouseOnHorizontal(mousePointDoc.y() + grabRadScale, mousePointDoc.y() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			m_mode = HORIZONTAL;
			m_haveGuide = true;
			m_guide = m_doc->Pages->at(page)->guides.horizontal(index, GuideManagerCore::Standard);
			m_currentGuide = m_guide;
			m_page = page;
			return true;
		}
	}
	m_haveGuide = false;
	return false;
}

void RulerGesture::mouseSelectGuide(QMouseEvent *m)
{
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	const int page = m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y());
	if ((m_doc->guidesPrefs().guidesShown) && page >= 0)
	{
		double grabRadScale = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
		int index = m_doc->Pages->at(page)->guides.isMouseOnVertical(mousePointDoc.x() + grabRadScale, mousePointDoc.x() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			emit guideInfo(VERTICAL, m_doc->Pages->at(page)->guides.vertical(index, GuideManagerCore::Standard));
			return;
		}
		index = m_doc->Pages->at(page)->guides.isMouseOnHorizontal(mousePointDoc.y() + grabRadScale, mousePointDoc.y() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			emit guideInfo(HORIZONTAL, m_doc->Pages->at(page)->guides.horizontal(index, GuideManagerCore::Standard));
			return;
		}
	}
}

void RulerGesture::movePoint(QMouseEvent* m, bool mouseRelease)
{
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	const int page = m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y());
	QRect  viewport(m_view->viewport()->mapToGlobal(QPoint(0,0)), QSize(m_view->visibleWidth(), m_view->visibleHeight()));
	QPoint newMousePoint = m->globalPos() - (m_canvas->mapToParent(QPoint(0, 0)) + m_canvas->parentWidget()->mapToGlobal(QPoint(0, 0)));
	double x = mousePointDoc.x();
	double y = mousePointDoc.y();
	switch (m_mode)
	{
		case ORIGIN:
			m_canvas->repaint();
			m_canvas->displayCorrectedXYHUD(m->globalPos(), x, y);
			break;
		case HORIZONTAL:
			if (!m_ScMW->doc->guidesPrefs().guidesShown)
				break;
			m_canvas->update(0, m_xy.y() - 2, m_canvas->width(), 4);
			
			if ((page >= 0) && (viewport.contains(m->globalPos())))
			{
				ScPage* currentPage = m_doc->Pages->at(page);
				if (m_doc->SnapElement)
				{
					double xout = 0;
					double yout = 0;
					m_doc->getClosestElementBorder(x, y, &xout, &yout, ScribusDoc::IncludeSelection);
					if (yout != y)
					{
						y = yout;
						newMousePoint = m_canvas->canvasToLocal(QPointF(x, y));
					}
				}
				if (!m_haveGuide)
				{
					qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
					if (mouseRelease)
					{
						currentPage->guides.addHorizontal(y - currentPage->yOffset(), GuideManagerCore::Standard);
						m_guide = y - currentPage->yOffset();
						m_page  = page;
						m_haveGuide = true;
						m_doc->changed();
					}
				}
				else if (mouseRelease)
				{
					if (page == m_page)
						currentPage->guides.moveHorizontal(m_guide, y - currentPage->yOffset(), GuideManagerCore::Standard);
					else
					{
						m_doc->Pages->at(m_page)->guides.deleteHorizontal(m_guide, GuideManagerCore::Standard);
						currentPage->guides.addHorizontal(y - currentPage->yOffset(), GuideManagerCore::Standard);
						m_page = page;
					}
					if (m_doc->currentPage() != m_doc->Pages->at(m_page))
						m_doc->setCurrentPage( m_doc->Pages->at(m_page) );
					m_doc->changed();
				}
				else
				{
					QCursor* cursor = qApp->overrideCursor();
					if (cursor && (cursor->shape() != Qt::SplitVCursor))
						qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
				}
				m_currentGuide = y - currentPage->yOffset();
			}
			else
			{ 
				if (m_haveGuide)
				{
					qApp->changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png"));
					if (mouseRelease)
					{
						m_doc->Pages->at(m_page)->guides.deleteHorizontal( m_guide, GuideManagerCore::Standard);
						m_haveGuide = false;
						m_doc->changed();
					}
				}
			}
			m_canvas->update(0, newMousePoint.y() - 2, m_canvas->width(), 4);
			m_canvas->displayCorrectedSingleHUD(m->globalPos(), y, false);
			break;
		case VERTICAL:
			if (!m_ScMW->doc->guidesPrefs().guidesShown)
				break;
			m_canvas->update(m_xy.x() - 2, 0, 4, m_canvas->height());
			if ((page >= 0) && viewport.contains(m->globalPos()))
			{
				ScPage* currentPage = m_doc->Pages->at(page);
				if (m_doc->SnapElement)
				{
					double xout = 0;
					double yout = 0;
					m_doc->getClosestElementBorder(x, y, &xout, &yout, ScribusDoc::IncludeSelection);
					if (xout != x)
					{
						x = xout;
						newMousePoint = m_canvas->canvasToLocal(QPointF(x, y));
					}
				}
				if (!m_haveGuide)
				{
					qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
					if (mouseRelease)
					{
						currentPage->guides.addVertical(x - currentPage->xOffset(), GuideManagerCore::Standard);
						m_guide = x - currentPage->xOffset();
						m_page  = page;
						m_haveGuide = true;
						m_doc->changed();
					}
				}
				else if (mouseRelease)
				{
					if (page == m_page)
						currentPage->guides.moveVertical(m_guide, x - currentPage->xOffset(), GuideManagerCore::Standard);
					else
					{
						m_doc->Pages->at(m_page)->guides.deleteVertical(m_guide, GuideManagerCore::Standard);
						currentPage->guides.addVertical(x - currentPage->xOffset(), GuideManagerCore::Standard);
						m_page = page;
					}
					if (m_doc->currentPage() != m_doc->Pages->at(m_page))
						m_doc->setCurrentPage( m_doc->Pages->at(m_page) );
					m_doc->changed();
				}
				else
				{
					QCursor* cursor = qApp->overrideCursor();
					if (cursor && (cursor->shape() != Qt::SplitHCursor))
						qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
				}
				m_currentGuide = x - currentPage->xOffset();
			}
			else
			{ 
				if (m_haveGuide)
				{
					qApp->changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png"));
					if (mouseRelease)
					{
						m_doc->Pages->at(m_page)->guides.deleteVertical( m_guide, GuideManagerCore::Standard);
						m_haveGuide = false;
						m_doc->changed();
					}
				}
			}
			m_canvas->update(newMousePoint.x() - 2, 0, 4, m_canvas->height());
			m_canvas->displayCorrectedSingleHUD(m->globalPos(), x, true);
			break;
	}
	m_xy = newMousePoint;
}


void RulerGesture::keyPressEvent(QKeyEvent* e)
{
	e->accept();

	if (e->key() == Qt::Key_Escape)
	{
		// Go back to normal mode.
		m_view->stopGesture();
		return;
	}
}

void RulerGesture::mouseMoveEvent(QMouseEvent* m)
{
	m_mousePoint = m_canvas->globalToCanvas(m->globalPos());
	m->accept();
	if (m_view->moveTimerElapsed())
	{
		movePoint(m, false);
		if (m_ScMW->doc->guidesPrefs().guidesShown)
			emit guideInfo(m_mode, m_currentGuide);
	}
}


void RulerGesture::mouseReleaseEvent(QMouseEvent* m)
{
	m->accept();
	if (m_view->moveTimerElapsed())
	{
		movePoint(m, true);
		if (m_mode == ORIGIN)
			m_view->setNewRulerOrigin(m);
		else
		{
			if (m_ScMW->doc->guidesPrefs().guidesShown)
				m_ScMW->guidePalette->setupPage();
		}
	}
	m_haveGuide = false;
	//#9391: Force redraw to get the guide drawn if we draw a guide in an edit mode
	if (m_ScMW->doc->appMode != modeNormal)
		m_canvas->setForcedRedraw(true);
	m_canvas->repaint();
	m_view->stopGesture();
	m_mousePoint = QPoint(0, 0);
	if (m_ScMW->doc->guidesPrefs().guidesShown)
		emit guideInfo(m_mode, m_currentGuide);
}


void RulerGesture::mousePressEvent(QMouseEvent* m)
{
	m->accept();
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_view->registerMousePress(m->globalPos());
	if (mouseHitsGuide(mousePointDoc))
	{
		m_xy = m->globalPos() - (m_canvas->mapToParent(QPoint(0, 0)) + m_canvas->parentWidget()->mapToGlobal(QPoint(0, 0)));
	}
	if (m_ScMW->doc->guidesPrefs().guidesShown)
		emit guideInfo(m_mode, m_currentGuide);
}

