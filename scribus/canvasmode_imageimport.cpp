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

#include "canvasmode_imageimport.h"

#include <QApplication>
#include <QBuffer>
#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QToolTip>
#include <QDebug>

#include "canvas.h"
#include "canvasmode.h"
#include "prefsmanager.h"
#include "ui/propertiespalette.h"
#include "ui/propertiespalette_image.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "util_icon.h"

CanvasMode_ImageImport::CanvasMode_ImageImport(ScribusView *view) : CanvasMode(view), m_ScMW(view->m_ScMW)
{
	Mx=0;
	My=0;
}

void CanvasMode_ImageImport::setImageList(QStringList l)
{
	imageList = l;
	if(l.size()==1)
	{
		while(!(m_doc->m_Selection->itemAt(0)->isImageFrame()))
			m_doc->m_Selection->removeFirst();
		if(!m_doc->m_Selection->isEmpty())
		{
			setImage(m_doc->m_Selection->itemAt(0));
			m_view->requestMode(submodePaintingDone);
		}
		else
			newToolTip(l.first());
	}
	else
		newToolTip(l.first());
}

void CanvasMode_ImageImport::newToolTip(QString name)
{
	QPainter p;
	QImage pm = QImage(80, 80, QImage::Format_ARGB32_Premultiplied);
	QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
	p.begin(&pm);
	p.fillRect(QRectF(0, 0, 80, 80), b);
	QImage thumb;
	if(thumb.load(name))
	{
		thumb = thumb.scaled(80,80,Qt::KeepAspectRatio);
		p.drawImage((80 - thumb.width()) / 2, (80 - thumb.height()) / 2, thumb);
		QBuffer buffer;
		buffer.open(QIODevice::WriteOnly);
		pm.save(&buffer, "PNG");
		QByteArray ba = buffer.buffer().toBase64();
		buffer.close();
		tipText = "<p align=\"center\"><img src=\"data:image/png;base64," + QString(ba) + "\"></p>";
	}
	else
		tipText = "<p align=\"center\">" + name.right(name.lastIndexOf("/")) + "</p>";
	QToolTip::showText(QPoint(Mx,My), tipText, qApp->activeWindow());
	p.end();
}

void CanvasMode_ImageImport::drawControls(QPainter* p)
{
	commonDrawControls(p, false);
}

void CanvasMode_ImageImport::enterEvent(QEvent *)
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
			imageList.prepend(imageList.takeLast());
			break;
		case Qt::Key_Backtab:
			imageList.append(imageList.takeFirst());
			break;
		case Qt::Key_Backspace:
			imageList.removeFirst();
			break;
		case Qt::Key_Escape:
			imageList.clear();
			break;
	}
	updateList();
	m_keyRepeat = false;
}


void CanvasMode_ImageImport::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void CanvasMode_ImageImport::activate(bool fromGesture)
{
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
	m_view->redrawMarker->hide();
}

void CanvasMode_ImageImport::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
}

void CanvasMode_ImageImport::mouseMoveEvent(QMouseEvent *m)
{
	Mx = m->globalPos().x();
	My = m->globalPos().y();
	QToolTip::showText(m->globalPos(), tipText, qApp->activeWindow());
	QToolTip::showText(m->globalPos(), tipText + "<b></b>", qApp->activeWindow());
	m->accept();
	PageItem *item;
	if((item = m_canvas->itemUnderCursor(m->globalPos())) != NULL)
	{
		PageItem_ImageFrame *currItem;
		if((currItem = item->asImageFrame()) != NULL)
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")));
		else
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	else
		qApp->changeOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")));
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
	m_view->registerMousePress(m->globalPos());
	PageItem *item;
	if(imageList.size()>0)
	{
		if((item = m_canvas->itemUnderCursor(m->globalPos())) != NULL)
		{
			PageItem_ImageFrame *currItem;
			if((currItem = item->asImageFrame()) != NULL)
				setImage(currItem);
		}
		else
			view()->ImageAfterDraw = true;
	}
	if (m->button() == Qt::MidButton)
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
	QString fileName = imageList.takeFirst();
	currItem->EmProfile = "";
	currItem->pixm.imgInfo.isRequest = false;
	currItem->UseEmbedded = true;
	currItem->IProfile = m_doc->cmsSettings().DefaultImageRGBProfile;
	currItem->IRender = m_doc->cmsSettings().DefaultIntentImages;
	qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	m_doc->loadPict(fileName, currItem, false, true);
	m_ScMW->propertiesPalette->imagePal->displayScaleAndOffset(currItem->imageXScale(), currItem->imageYScale(), currItem->imageXOffset(), currItem->imageYOffset());
	m_ScMW->repaint();
	qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	m_view->DrawNew();
	currItem->emitAllToGUI();
}

void CanvasMode_ImageImport::updateList()
{
	if(imageList.isEmpty())
	{
		m_view->requestMode(submodePaintingDone);
		QToolTip::hideText();
	}
	else
		newToolTip(imageList.first());
}
