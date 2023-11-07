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


#include "canvasmode_eyedropper.h"

#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QEvent>
#include <QInputDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <QDebug>

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "sccolorengine.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/propertiespalette.h"
#include "util.h"
#include "util_math.h"


CanvasMode_EyeDropper::CanvasMode_EyeDropper(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
}

void CanvasMode_EyeDropper::grabMouse()
{
	if (!m_mouseGrabbed)
	{
		m_view->widget()->grabMouse(modeCursor());
		m_mouseGrabbed = true;
	}
}

void CanvasMode_EyeDropper::releaseMouse()
{
	if (m_mouseGrabbed)
	{
		m_view->widget()->releaseMouse();
		m_mouseGrabbed = false;
	}
}

void CanvasMode_EyeDropper::drawControls(QPainter* p)
{
	commonDrawControls(p, false);
}

void CanvasMode_EyeDropper::enterEvent(QEvent *e)
{
	setModeCursor();
}

void CanvasMode_EyeDropper::leaveEvent(QEvent *e)
{
}


void CanvasMode_EyeDropper::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_EyeDropper::activate" << fromGesture;
	CanvasMode::activate(fromGesture);

	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
	grabMouse();
}

void CanvasMode_EyeDropper::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_EyeDropper::deactivate" << forGesture;
	m_view->setRedrawMarkerShown(false);
	releaseMouse();

	CanvasMode::deactivate(forGesture);
}

void CanvasMode_EyeDropper::keyPressEvent(QKeyEvent *e)
{
	commonkeyPressEvent_Default(e);
}

void CanvasMode_EyeDropper::keyReleaseEvent(QKeyEvent *e)
{
	commonkeyReleaseEvent(e);
}

void CanvasMode_EyeDropper::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
}

void CanvasMode_EyeDropper::mouseMoveEvent(QMouseEvent *m)
{
	/*m->accept();
	if (commonMouseMove(m))
		return;*/
	m->accept();
}

void CanvasMode_EyeDropper::mousePressEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
}

void CanvasMode_EyeDropper::mouseReleaseEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
//	m_view->stopDragTimer();

	releaseMouse();

	m_view->setCursor(QCursor(Qt::ArrowCursor));
	QPixmap pm;
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
		pm = screen->grabWindow( QApplication::desktop()->winId(), m->globalPos().x(), m->globalPos().y(), 1, 1);
	QImage img = pm.toImage();
	QColor selectedColor = img.pixel(0, 0);

	QString colorName;
	for (auto it = m_doc->PageColors.begin(); it != m_doc->PageColors.end(); ++it)
	{
		if (selectedColor == ScColorEngine::getRGBColor(it.value(), m_doc))
		{
			colorName = it.key();
			break;
		}
	}

	if (colorName.isEmpty())
	{
		bool ok;
		bool nameFound = false;
		QString questionString = "<qt>" + tr("The selected color does not exist in the document's color set.") + "<br/>" + tr("Please enter a name for this new color.") + "</qt>";
		do
		{
			colorName = QInputDialog::getText(m_ScMW, tr("Color Not Found"), questionString, QLineEdit::Normal, tr("RGB %1").arg(selectedColor.name()), &ok);
			if (ok)
			{
				if (m_doc->PageColors.contains(colorName))
					questionString = "<qt>" + tr("The name you have selected already exists.") + "<br/>" + tr("Please enter a different name for this new color.") + "</qt>";
				else
					nameFound = true;
			}
		} while (!nameFound && ok);
		if (!ok)
			colorName.clear();
		if (!colorName.isEmpty())
		{
			ScColor newColor(selectedColor.red(), selectedColor.green(), selectedColor.blue());
			m_doc->PageColors[colorName] = newColor;
			m_doc->changed();
			m_ScMW->updateColorLists();
		}
	}

	int docSelectionCount = m_doc->m_Selection->count();
	if (!colorName.isEmpty() && docSelectionCount > 0)
	{
		for (int i = 0; i < docSelectionCount; ++i)
		{
			PageItem *currItem = m_doc->m_Selection->itemAt(i);
			if (!currItem)
				continue;
			if ((m->modifiers() & Qt::ControlModifier) && (currItem->isTextFrame() || currItem->asPathText()))
				m_doc->itemSelection_SetFillColor(colorName); //Text colour
			else if (m->modifiers() & Qt::AltModifier) //Line colour
				m_doc->itemSelection_SetItemPen(colorName);
			else
				m_doc->itemSelection_SetItemBrush(colorName); //Fill colour
		}
	}
	m_view->requestMode(modeNormal);
}

