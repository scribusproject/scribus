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



#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QDebug>

#include "canvas.h"
#include "canvasmode.h"
#include "canvasmode_imageimport.h"
#include "iconmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"

CanvasMode_ImageImport::CanvasMode_ImageImport(ScribusView *view) : CanvasMode(view), m_ScMW(view->m_ScMW)
{
	const QPoint cursorPos = QCursor::pos();
	m_Mx = cursorPos.x();
	m_My = cursorPos.y();
}

void CanvasMode_ImageImport::setImageList(QStringList l)
{
	m_imageList = l;
	if (l.size()==1)
	{
		while (!(m_doc->m_Selection->itemAt(0)->isImageFrame()))
			m_doc->m_Selection->removeFirst();
		if (m_doc->m_Selection->isNotEmpty())
		{
			setImage(m_doc->m_Selection->itemAt(0));
			m_view->requestMode(submodePaintingDone);
		}
		else
			updatePreview(l.first());
	}
	else
		updatePreview(l.first());
}

void CanvasMode_ImageImport::displayPreview(QPointF globalPos)
{
	if (!m_previewPixmap.isNull())
		m_canvas->displayHUD(globalPos, m_previewPixmap, false);
	else
		m_canvas->displayHUD(globalPos, m_previewText, false);
}

void CanvasMode_ImageImport::updatePreview(const QString& name)
{
	const qreal dpr = m_view->devicePixelRatioF();
	QPixmap preview(QSize(80, 80) * dpr);
	preview.setDevicePixelRatio(dpr);
	QBrush background(QColor(205, 205, 205), IconManager::instance().loadPixmap("testfill"));
	QImage thumb;
	const bool hasThumbnail = thumb.load(name);
	{
		QPainter painter(&preview);
		painter.fillRect(QRectF(0, 0, 80, 80), background);
		if (hasThumbnail)
		{
			thumb = thumb.scaled(QSize(80, 80) * dpr, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			thumb.setDevicePixelRatio(dpr);
			const QSizeF thumbSize = thumb.deviceIndependentSize();
			painter.drawImage(QPointF((80 - thumbSize.width()) / 2, (80 - thumbSize.height()) / 2), thumb);
		}
	}

	if (hasThumbnail)
	{
		m_previewPixmap = preview;
		m_previewText.clear();
	}
	else
	{
		m_previewPixmap = QPixmap();
		m_previewText = QFileInfo(name).fileName();
	}

	displayPreview(QPointF(m_Mx, m_My));
}

void CanvasMode_ImageImport::drawControls(QPainter* p)
{
	commonDrawControls(p, false);
}

void CanvasMode_ImageImport::enterEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		setModeCursor();
}

void CanvasMode_ImageImport::keyPressEvent(QKeyEvent *e)
{
	int kk = e->key();
	if (m_keyRepeat)
		return;
	m_keyRepeat = true;
	e->accept();
	switch (kk)
	{
		case Qt::Key_Tab:
			m_imageList.prepend(m_imageList.takeLast());
			break;
		case Qt::Key_Backtab:
			m_imageList.append(m_imageList.takeFirst());
			break;
		case Qt::Key_Backspace:
			m_imageList.removeFirst();
			break;
		case Qt::Key_Escape:
			m_imageList.clear();
			break;
	}
	updateList();
	m_keyRepeat = false;
}


void CanvasMode_ImageImport::leaveEvent(QEvent *e)
{
	m_canvas->hideHUD();
}

void CanvasMode_ImageImport::activate(bool fromGesture)
{
	CanvasMode::activate(fromGesture);

	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_keyRepeat = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	setModeCursor();
	if (fromGesture)
		m_view->update();
}

void CanvasMode_ImageImport::deactivate(bool forGesture)
{
	m_canvas->hideHUD();
	m_view->setRedrawMarkerShown(false);
	CanvasMode::deactivate(forGesture);
}

void CanvasMode_ImageImport::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
}

void CanvasMode_ImageImport::mouseMoveEvent(QMouseEvent *m)
{
	m_Mx = m->globalPosition().x();
	m_My = m->globalPosition().y();
	displayPreview(m->globalPosition());
	m->accept();
	PageItem *item;
	if ((item = m_canvas->itemUnderCursor(m->globalPosition())) != nullptr)
	{
		PageItem_ImageFrame *currItem;
		if ((currItem = item->asImageFrame()) != nullptr)
			m_view->setCursor(IconManager::instance().loadCursor("cursor-image"));
		else
			m_view->setCursor(QCursor(Qt::ArrowCursor));
	}
	else
		m_view->setCursor(IconManager::instance().loadCursor("cursor-image"));
	if (commonMouseMove(m))
		return;
}

void CanvasMode_ImageImport::mousePressEvent(QMouseEvent *m)
{
	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPosition());
	PageItem *item;
	if (!m_imageList.empty())
	{
		if ((item = m_canvas->itemUnderCursor(m->globalPosition())) != nullptr)
		{
			PageItem_ImageFrame *currItem;
			if ((currItem = item->asImageFrame()) != nullptr)
				setImage(currItem);
		}
		else
			view()->ImageAfterDraw = true;
	}
	if (m->button() == Qt::MiddleButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
}

void CanvasMode_ImageImport::mouseReleaseEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->updateContents();
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	updateList();
}

void CanvasMode_ImageImport::setImage(PageItem *currItem)
{
	QString fileName = m_imageList.takeFirst();
	currItem->pixm.imgInfo.isRequest = false;
	currItem->UseEmbedded = true;
	currItem->EmbeddedProfile.clear();
	currItem->ImageProfile = m_doc->cmsSettings().DefaultImageRGBProfile;
	currItem->ImageIntent = m_doc->cmsSettings().DefaultIntentImages;
	qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	m_doc->loadPict(fileName, currItem, false, true);
	// Call to showScaleAndOffset() is now very likely unnecessary
	// due to mechanisms used to update properties in PP in 1.5.x+
	// m_ScMW->contentPalette->update(currItem->isImageFrame());
	m_ScMW->repaint();
	qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	m_view->DrawNew();
	currItem->emitAllToGUI();
}

void CanvasMode_ImageImport::updateList()
{
	if (m_imageList.isEmpty())
	{
		m_view->requestMode(submodePaintingDone);
		m_canvas->hideHUD();
	}
	else
		updatePreview(m_imageList.constFirst());
}
