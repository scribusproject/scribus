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


#include "canvasmode.h"

#include "canvas.h"
#include "canvasmode_create.h"
#include "canvasmode_drawbezier.h"
#include "canvasmode_legacy.h"
#include "canvasmode_nodeedit.h"
#include "canvasmode_drawfreehand.h"
#include "selection.h"
#include "scribusview.h"
#include "util_icon.h"

#include <QPainter>


CanvasMode::CanvasMode (ScribusView* view) :
	m_view(view),
	m_canvas(view->m_canvas),
	m_doc(view->Doc)
{
}



CanvasMode* CanvasMode::createForAppMode(ScribusView* view, int appMode)
{
	CanvasMode* result;
	
	switch (appMode)
	{
		case modeEditClip:
			result = new CanvasMode_NodeEdit(view);
			break;
		case modeDrawBezierLine:
			result = new BezierMode(view);
			break;
		case modeDrawFreehandLine:
			result = new FreehandMode(view);
			break;
		case modeDrawLine:
			result = new LegacyMode(view);
			break;
		case modeDrawShapes:
		case modeDrawPicture:
		case modeDrawLatex:
		case modeDrawText:
		case modeDrawTable:
		case modeDrawRegularPolygon:
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
			result = new CreateMode(view);
			break;
			
			// more modes as they are defined...
			
		default:
			result = new LegacyMode(view);
			break;
	}
	return result;
}


void CanvasMode::updateViewMode(CanvasViewMode* viewmode) 
{
	viewmode->drawSelectedItemsWithControls = false;
	viewmode->drawFramelinksWithContents = false;	
}


void CanvasMode::drawSelection(QPainter* psx)
{
	psx->scale(m_canvas->scale(), m_canvas->scale());
	if (m_doc->m_Selection->count() != 0)
	{
		uint docSelectionCount = m_doc->m_Selection->count();
		PageItem *currItem;
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = m_doc->m_Selection->itemAt(a);
			psx->save();
			psx->translate(static_cast<int>(currItem->xPos()), static_cast<int>(currItem->yPos()));
			psx->rotate(currItem->rotation());			
			currItem->paintObj(psx);
			psx->restore();
		}
	}
	if (m_doc->m_Selection->isMultipleSelection())
	{
		double x, y, w, h;
		m_doc->m_Selection->setGroupRect();
		m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
//		x *= m_canvas->scale();
//		y *= m_canvas->scale();
//		w *= m_canvas->scale();
//		h *= m_canvas->scale();
		psx->setPen(QPen(Qt::red, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
		psx->setBrush(Qt::NoBrush);
		psx->drawRect(QRectF(x, y, w, h));
		psx->setBrush(Qt::red);
		psx->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		psx->drawRect(QRectF(x+w-6, y+h-6, 6, 6));
		psx->drawRect(QRectF(x+w/2 - 3, y+h-6, 6, 6));
		psx->drawRect(QRectF(x+w/2 - 3, y, 6, 6));
		psx->drawRect(QRectF(x+w-6, y+h/2 - 3, 6, 6));
		psx->drawRect(QRectF(x+w-6, y, 6, 6));
		psx->drawRect(QRectF(x, y, 6, 6));
		psx->drawRect(QRectF(x, y+h/2 - 3, 6, 6));
		psx->drawRect(QRectF(x, y+h-6, 6, 6));
	}
}

void CanvasMode::setModeCursor()
{
	//NOTE: Merge with similar code in ScribusMainWindow::setAppMode()
	switch (m_doc->appMode)
	{
		case modeDrawShapes:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")));
			break;
		case modeDrawPicture:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")));
			break;
		case modeDrawLatex:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawLatexFrame.xpm")));
			break;
		case modeDrawText:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")));
			break;
		case modeDrawTable:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawTable.xpm")));
			break;
		case modeDrawRegularPolygon:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")));
			break;
		case modeDrawLine:
		case modeDrawBezierLine:
			qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
			break;
		case modeDrawFreehandLine:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawFreeLine.png"), 0, 32));
			break;
		case modeMagnifier:
			if (m_view->Magnify)
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			else
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
			break;
		case modePanning:
			qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
			break;
		case modeEyeDropper:
			qApp->changeOverrideCursor(QCursor(loadIcon("colorpickercursor.png"), 0, 32));
			break;
		case modeMeasurementTool:
		case modeEditGradientVectors:
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
			qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
			break;
		default:
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			break;
	}
}


