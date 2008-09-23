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

#include "guidemanager.h"
#include "guidemanagercore.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "util_icon.h"

void RulerGesture::drawControls(QPainter* p)
{
	Page* page = m_doc->currentPage();
	QColor color(m_doc->guidesSettings.guideColor);
	p->save();
	QPoint pageOrigin = m_canvas->canvasToLocal(QPointF(page->xOffset(), page->yOffset()));
	QSize pageSize = (QSizeF(page->width(), page->height()) * m_canvas->scale()).toSize();
	switch (m_mode)
	{
		case HORIZONTAL:
			p->setPen(QPen(color, 1.0 / m_canvas->scale(), Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(QPoint(pageOrigin.x(), m_xy.y()), QPoint(pageOrigin.x() + pageSize.width(), m_xy.y()));
			break;
		case VERTICAL:
			p->setPen(QPen(color, 1.0 / m_canvas->scale(), Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(QPoint(m_xy.x(), pageOrigin.y()), QPoint(m_xy.x(), pageOrigin.y() + pageSize.height()));
			break;
		case ORIGIN:
			p->setPen(QPen(color, 1.0 / m_canvas->scale(), Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
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
	m_haveCursor = (qApp->overrideCursor() != NULL);
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

void RulerGesture::deactivate(bool)
{
	if (m_haveCursor)
		qApp->changeOverrideCursor(m_cursor);
	m_haveGuide = false;
}


bool RulerGesture::mouseHitsGuide(FPoint mousePointDoc)
{
	const int page = m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y());
	if ((m_doc->guidesSettings.guidesShown) && (!m_doc->GuideLock) && page >= 0)
	{
		double grabRadScale = m_doc->guidesSettings.grabRad / m_canvas->scale();
		int index = m_doc->Pages->at(page)->guides.isMouseOnVertical(mousePointDoc.x() + grabRadScale, mousePointDoc.x() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			m_mode = VERTICAL;
			m_haveGuide = true;
			m_guide = m_doc->Pages->at(page)->guides.vertical(index, GuideManagerCore::Standard);
			m_page = page;
// 			emit guideInfo(m_mode, m_guide);
			return true;
		}
		index = m_doc->Pages->at(page)->guides.isMouseOnHorizontal(mousePointDoc.y() + grabRadScale, mousePointDoc.y() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			m_mode = HORIZONTAL;
			m_haveGuide = true;
			m_guide = m_doc->Pages->at(page)->guides.horizontal(index, GuideManagerCore::Standard);
			m_page = page;
// 			emit guideInfo(m_mode, m_guide);
			return true;
		}
	}
	m_haveGuide = false;
	return false;
}


void RulerGesture::movePoint(QMouseEvent* m)
{
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	const int page = m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y());
	QRect viewport(m_view->viewport()->mapToGlobal(QPoint(0,0)), QSize(m_view->visibleWidth(), m_view->visibleHeight()));
	QPoint newMousePoint = m->globalPos() - m_canvas->mapToGlobal(QPoint(0,0));
	switch (m_mode)
	{
		case ORIGIN:
			m_canvas->repaint();
			break;
		case HORIZONTAL:
			m_canvas->update(0, m_xy.y()-2, m_canvas->width(), 4);
			m_canvas->update(0, newMousePoint.y()-2, m_canvas->width(), 4);
			if ((page >= 0) && (viewport.contains(m->globalPos())))
			{
				if (!m_haveGuide)
				{
					qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
					m_doc->Pages->at(page)->guides.addHorizontal(mousePointDoc.y() - m_doc->Pages->at(page)->yOffset(), GuideManagerCore::Standard);
					m_page = page;
					m_haveGuide = true;
				}
				else
				{
					if (page == m_page)
						m_doc->Pages->at(page)->guides.moveHorizontal( m_guide, mousePointDoc.y() - m_doc->Pages->at(page)->yOffset(), GuideManagerCore::Standard);
					else
					{
						m_doc->Pages->at(m_page)->guides.deleteHorizontal( m_guide, GuideManagerCore::Standard);
						m_doc->Pages->at(page)->guides.addHorizontal(mousePointDoc.y() - m_doc->Pages->at(page)->yOffset(), GuideManagerCore::Standard);
						m_page = page;
					}
				}
				m_guide = mousePointDoc.y() - m_doc->Pages->at(page)->yOffset();
			}
			else
			{ 
				if (m_haveGuide)
				{
					qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png")));
					m_doc->Pages->at(m_page)->guides.deleteHorizontal( m_guide, GuideManagerCore::Standard);
					m_haveGuide = false;
				}
			}
			break;
		case VERTICAL:
			m_canvas->update(m_xy.x()-2, 0, 4, m_canvas->height());
			m_canvas->update(newMousePoint.x()-2, 0, 4, m_canvas->height());
			if ((page >= 0) && viewport.contains(m->globalPos()))
			{
				if (!m_haveGuide)
				{
					qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
					m_doc->Pages->at(page)->guides.addVertical(mousePointDoc.x() - m_doc->Pages->at(page)->xOffset(), GuideManagerCore::Standard);
					m_page = page;
					m_haveGuide = true;
				}
				else
				{
					if (page == m_page)
						m_doc->Pages->at(page)->guides.moveVertical( m_guide, mousePointDoc.x() - m_doc->Pages->at(page)->xOffset(), GuideManagerCore::Standard);
					else
					{
						m_doc->Pages->at(m_page)->guides.deleteVertical( m_guide, GuideManagerCore::Standard);
						m_doc->Pages->at(page)->guides.addVertical(mousePointDoc.x() - m_doc->Pages->at(page)->xOffset(), GuideManagerCore::Standard);
						m_page = page;
					}
				}
				m_guide = mousePointDoc.x() - m_doc->Pages->at(page)->xOffset();
			}
			else
			{ 
				if (m_haveGuide)
				{
					qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png")));
					m_doc->Pages->at(m_page)->guides.deleteVertical( m_guide, GuideManagerCore::Standard);
					m_haveGuide = false;
				}
			}
			break;
	}
	m_xy = newMousePoint;
}


void RulerGesture::mouseMoveEvent(QMouseEvent* m)
{
	movePoint(m);
	m->accept();
	emit guideInfo(m_mode, m_guide);
}


void RulerGesture::mouseReleaseEvent(QMouseEvent* m)
{
	movePoint(m);
	if (m_mode == ORIGIN)
		m_view->setNewRulerOrigin(m);
	else
		m_ScMW->guidePalette->setupPage();
	m_haveGuide = false;
	m->accept();
	m_canvas->repaint();
	m_view->stopGesture();
	emit guideInfo(m_mode, m_guide);
}


void RulerGesture::mousePressEvent(QMouseEvent* m)
{
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	if (mouseHitsGuide(mousePointDoc))
	{
		m_xy = m->globalPos() - m_canvas->mapToGlobal(QPoint(0,0));
		m->accept();
	}
	emit guideInfo(m_mode, m_guide);
}
