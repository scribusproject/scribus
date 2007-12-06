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
#include "scribusview.h"

void RulerGesture::drawControls(QPainter* p)
{
	p->setPen(Qt::red);
	switch (m_mode)
	{
		case HORIZONTAL:
			break;
		case VERTICAL:
			break;
		case ORIGIN:
			break;
	}
}

void RulerGesture::activate(bool fromGesture)
{
	if (!fromGesture)
		m_cursor = qApp->overrideCursor();
	switch (m_mode)
	{
		case HORIZONTAL:
			qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
			break;
		case VERTICAL:
			qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
			break;
		case ORIGIN:
			m_index = -1;
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			break;
	}
}

void RulerGesture::deactivate(bool)
{
	if (m_cursor)
		qApp->changeOverrideCursor(*m_cursor);
}


bool RulerGesture::mouseHitsGuide(FPoint mousePointDoc)
{
//	const int page = m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y());
	if ((m_doc->guidesSettings.guidesShown) && (!m_doc->GuideLock)) // page >= 0
	{
		double grabRadScale = m_doc->guidesSettings.grabRad / m_canvas->scale();
		int index = m_doc->currentPage()->guides.isMouseOnHorizontal(mousePointDoc.y() + grabRadScale, mousePointDoc.y() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			m_mode = HORIZONTAL;
			m_index = index;
			m_page = m_doc->currentPage()->pageNr();
			return true;
		}
		index = m_doc->currentPage()->guides.isMouseOnVertical(mousePointDoc.x() + grabRadScale, mousePointDoc.x() - grabRadScale, GuideManagerCore::Standard);
		if (index >= 0)
		{
			m_mode = VERTICAL;
			m_index = index;
			m_page = m_doc->currentPage()->pageNr();
			return true;
		}
	}
	return false;
}


void RulerGesture::movePoint(QMouseEvent* m)
{
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	const int page = m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y());
	switch (m_mode)
	{
		case ORIGIN:
			break;
		case HORIZONTAL:
			break;
		case VERTICAL:
			if ((page != -1) && (QRect(0, 0, m_view->visibleWidth(), m_view->visibleHeight()).contains(m->pos())))
			{
				if (m_index < 0)
				{
					m_doc->Pages->at(page)->guides.addVertical(mousePointDoc.x() - m_doc->Pages->at(page)->xOffset(), GuideManagerCore::Standard);
//					emit signalGuideInformation(0, qRound((newX-m_doc->Pages->at(page)->xOffset()) * 10000.0) / 10000.0);
				}
				else
				{
					if (page == m_doc->currentPageNumber())
					{
						m_doc->currentPage()->guides.moveVertical( m_doc->currentPage()->guides.vertical(m_index, GuideManagerCore::Standard),
																   mousePointDoc.x() - m_doc->currentPage()->xOffset(), GuideManagerCore::Standard);
//						emit signalGuideInformation(0, qRound((newX-m_doc->Pages->at(page)->xOffset()) * 10000.0) / 10000.0);
					}
					else
					{
						m_doc->currentPage()->guides.deleteVertical( m_doc->currentPage()->guides.vertical(m_index, GuideManagerCore::Standard), GuideManagerCore::Standard);
//						emit signalGuideInformation(-1, 0.0);
						m_doc->Pages->at(page)->guides.addVertical(mousePointDoc.x() - m_doc->Pages->at(page)->xOffset(), GuideManagerCore::Standard);
					}
				}
//				m_view->m_ScMW->guidePalette->clearRestoreVerticalList();
			}
			else
			{ 
				if (m_index >= 0)
				{
					m_doc->currentPage()->guides.deleteVertical( m_doc->currentPage()->guides.vertical(m_index, GuideManagerCore::Standard), GuideManagerCore::Standard);
//					emit signalGuideInformation(-1, 0.0);
//					m_view->m_ScMW->guidePalette->clearRestoreVerticalList();
				}
			}
//			emit DocChanged();
			break;
	}
	m_xy = m->pos();
}


void RulerGesture::mouseMoveEvent(QMouseEvent* m)
{
	movePoint(m);
}


void RulerGesture::mouseReleaseEvent(QMouseEvent* m)
{
	movePoint(m);
	m_view->stopGesture();
}


void RulerGesture::mousePressEvent(QMouseEvent* m)
{
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	if (mouseHitsGuide(mousePointDoc))
		m_xy = m->pos();
}