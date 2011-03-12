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
#include "canvasgesture_pan.h"
#include "canvasmode_copyproperties.h"
#include "canvasmode_create.h"
#include "canvasmode_drawbezier.h"
#include "canvasmode_drawfreehand.h"
#include "canvasmode_drawcalligraphic.h"
#include "canvasmode_edit.h"
#include "canvasmode_editarc.h"
#include "canvasmode_editgradient.h"
#include "canvasmode_editmeshgradient.h"
#include "canvasmode_editpolygon.h"
#include "canvasmode_editspiral.h"
#include "canvasmode_eyedropper.h"
#include "canvasmode_framelinks.h"
#include "canvasmode_magnifier.h"
#include "canvasmode_measurements.h"
#include "canvasmode_nodeedit.h"
#include "canvasmode_normal.h"
#include "canvasmode_objimport.h"
#include "canvasmode_panning.h"
#include "canvasmode_rotate.h"
#ifdef GESTURE_FRAME_PREVIEW
#include "pageitempreview.h"
#endif
#include "pageitem_group.h"
#include "prefsmanager.h"
#include "selection.h"
#include "scpainter.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "ui/checkDocument.h"
#include "ui/contextmenu.h"
#include "ui/outlinepalette.h"
#include "ui/pageselector.h"
#include "ui/scrapbookpalette.h"
#include "ui/scrspinbox.h"
#include "ui/scresizecursor.h"
#include "undomanager.h"
#include "units.h"
#include "util_icon.h"

#include <QPainter>


CanvasMode::CanvasMode (ScribusView* view) :
	QObject(),
	m_view(view),
	m_canvas(view->m_canvas),
	m_doc(view->Doc),
	m_panGesture(NULL)
{
	m_pen["outline"]	= QPen(Qt::gray, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["outline"].setCosmetic(true);
	m_pen["selection"]	= QPen(Qt::red, 1.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection"].setCosmetic(true);
	m_pen["selection-group"] = QPen(Qt::red, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection-group"].setCosmetic(true);
	m_pen["selection-group-inside"] = QPen(Qt::red, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection-group-inside"].setCosmetic(true);
	m_pen["handle"]		= QPen(Qt::red, 1.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["handle"].setCosmetic(true);
	
	m_brush["outline"]	= Qt::NoBrush;
	m_brush["selection"]	= Qt::NoBrush;
	m_brush["selection-group"] = QColor(255,0,0,10);
	m_brush["selection-group-inside"] = Qt::NoBrush;
	m_brush["handle"]	= Qt::red;

	m_keyRepeat = false;
	m_arrowKeyDown = false;
}

CanvasMode::~CanvasMode()
{
	if (m_panGesture)
		delete m_panGesture;
}

CanvasMode* CanvasMode::createForAppMode(ScribusView* view, int appMode)
{
	CanvasMode* result;
	
	switch (appMode)
	{
		case modeNormal:
		case modeStoryEditor:
			result = new CanvasMode_Normal(view);
			break;
		case modeCopyProperties:
			result = new CanvasMode_CopyProperties(view);
			break;
		case modeEdit:
			result = new CanvasMode_Edit(view);
			break;
		case modeEditClip:
			result = new CanvasMode_NodeEdit(view);
			break;
		case modeEditGradientVectors:
			result = new CanvasMode_EditGradient(view);
			break;
		case modeEditMeshGradient:
			result = new CanvasMode_EditMeshGradient(view);
			break;
		case modeDrawBezierLine:
			result = new BezierMode(view);
			break;
		case modeDrawFreehandLine:
			result = new FreehandMode(view);
			break;
		case modeDrawCalligraphicLine:
			result = new CalligraphicMode(view);
			break;
		case modeDrawLine:
		case modeDrawShapes:
		case modeDrawImage:
		case modeDrawLatex:
		case modeDrawText:
		case modeDrawTable:
		case modeDrawRegularPolygon:
		case modeDrawArc:
		case modeDrawSpiral:
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
		case modeInsertPDF3DAnnotation:
			result = new CreateMode(view);
			break;
		case modeEyeDropper:
			result = new CanvasMode_EyeDropper(view);
			break;
		case modeImportObject:
			result = new CanvasMode_ObjImport(view);
			break;
		case modeLinkFrames:
		case modeUnlinkFrames:
			result = new CanvasMode_FrameLinks(view);
			break;
		case modeMagnifier:
			result = new CanvasMode_Magnifier(view);
			break;
		case modeMeasurementTool:
			result = new MeasurementsMode(view);
			break;
		case modePanning:
			result = new CanvasMode_Panning(view);
			break;
		case modeRotation:
			result = new CanvasMode_Rotate(view);
			break;
		case modeEditArc:
			result = new CanvasMode_EditArc(view);
			break;
		case modeEditPolygon:
			result = new CanvasMode_EditPolygon(view);
			break;
		case modeEditSpiral:
			result = new CanvasMode_EditSpiral(view);
			break;
			// more modes as they are defined...
			
		default:
			result = new CanvasMode_Normal(view);
			break;
	}
	return result;
}


void CanvasMode::updateViewMode(CanvasViewMode* viewmode) 
{
	viewmode->drawSelectedItemsWithControls = false;
	viewmode->drawFramelinksWithContents = false;	
}


void CanvasMode::drawSelection(QPainter* psx, bool drawHandles)
{
//	QTime t;
//	t.start();
//	QTime tt;
//	int tg(0);
//	QStringList tu;
	QString ds;
	psx->scale(m_canvas->scale(), m_canvas->scale());
	psx->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	
	psx->setClipping(true);
	psx->setClipRegion(QRegion ( m_canvas->exposedRect() ) );
	if (m_doc->m_Selection->isMultipleSelection())
	{
		PageItem *curItem(0);
		for(int a=0; a<m_doc->m_Selection->count(); ++a)
		{
			curItem = m_doc->m_Selection->itemAt(a);
			
			if(drawHandles)
				drawHandles = !curItem->locked();
		}
		
		psx->save();
		psx->setPen(m_pen["selection-group"]);
		psx->setBrush(m_brush["selection-group"]);
		double lineAdjust(psx->pen().width()/m_canvas->scale());
		double x, y, w, h;
		m_doc->m_Selection->setGroupRect();
		m_doc->m_Selection->getVisualGroupRect(&x, &y, &w, &h);
		const double markWidth = 4 / m_canvas->scale();
		const double halfMarkWidth = 2 / m_canvas->scale();
		
		psx->translate(x,y);
		x = -lineAdjust;
		y = -lineAdjust;

//		tt.start();
		psx->drawRect(QRectF(x, y, w, h));
		if(drawHandles)
		{
			psx->setBrush(m_brush["handle"]);
			psx->setPen(m_pen["handle"]);
			psx->drawRect(QRectF(x+w-markWidth, y+h-markWidth, markWidth, markWidth));
			psx->drawRect(QRectF(x+w/2 - halfMarkWidth, y+h-markWidth, markWidth, markWidth));
			psx->drawRect(QRectF(x+w/2 - halfMarkWidth, y, markWidth, markWidth));
			psx->drawRect(QRectF(x+w-markWidth, y+h/2 - halfMarkWidth, markWidth, markWidth));
			psx->drawRect(QRectF(x+w-markWidth, y, markWidth, markWidth));
			psx->drawRect(QRectF(x, y, markWidth, markWidth));
			psx->drawRect(QRectF(x, y+h/2 - halfMarkWidth, markWidth, markWidth));
			psx->drawRect(QRectF(x, y+h-markWidth, markWidth, markWidth));
		}
//		tg=tt.elapsed();
		psx->restore();

		// items inside a a multi
		if (m_doc->m_Selection->count() > 1)
		{
			uint docSelectionCount = m_doc->m_Selection->count();
			PageItem *currItem;
			for (uint a=0; a<docSelectionCount; ++a)
			{
				currItem = m_doc->m_Selection->itemAt(a);
				if (!m_doc->Items->contains(currItem))
					continue;
				psx->save();
				psx->setPen(m_pen["selection-group-inside"]);
				psx->setBrush(m_brush["selection-group-inside"]);
				double lineAdjust(psx->pen().width()/m_canvas->scale());
				double x, y, w, h;
				w = currItem->visualWidth() ;
				h = currItem->visualHeight() ;
				if (currItem->rotation() != 0)
				{
					psx->setRenderHint(QPainter::Antialiasing);
					psx->translate(currItem->xPos(), currItem->yPos());
					psx->rotate(currItem->rotation());
					x = currItem->asLine() ? 0 : (currItem->visualXPos() - currItem->xPos() - lineAdjust);
					y = currItem->asLine() ? (h / -2.0) : (currItem->visualYPos() - currItem->yPos() - lineAdjust);
				}
				else
				{
					psx->translate(currItem->visualXPos(), currItem->visualYPos());
					x = currItem->asLine() ? 0 : -lineAdjust;
					y = currItem->asLine() ? 0 : -lineAdjust;
				}

				psx->drawRect(QRectF(x, y, w, h));

				psx->restore();
			}
		}
	}
	else if (m_doc->m_Selection->count() != 0)
	{
// 		ds = "S" + QString::number(m_doc->m_Selection->count())+" ";
		const double markWidth = 4 / m_canvas->scale();
		const double halfMarkWidth = 2 / m_canvas->scale();

		uint docSelectionCount = m_doc->m_Selection->count();
		PageItem *currItem;
		
		// FIXME when more than 1 item is selected, first Rect is drew normally (<10ms here)
		// but followings are damn long - pm 
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = m_doc->m_Selection->itemAt(a);
// 			qDebug()<<"It"<<currItem->xPos()<< currItem->yPos();
			
			psx->save();
			psx->setPen(m_pen["selection"]);
			psx->setBrush(m_brush["selection"]);
			double lineAdjust(psx->pen().width()/m_canvas->scale());
			double x, y, w, h;
			w = currItem->visualWidth();
			h = currItem->visualHeight();
			if (currItem->rotation() != 0)
			{
				psx->setRenderHint(QPainter::Antialiasing);
				psx->translate(currItem->xPos(), currItem->yPos());
				psx->rotate(currItem->rotation());
				x = currItem->asLine() ? 0 : (currItem->visualXPos() - currItem->xPos() - lineAdjust);
				y = currItem->asLine() ? (h / -2.0) : (currItem->visualYPos() - currItem->yPos() - lineAdjust);
			}
			else
			{
				psx->translate(currItem->visualXPos(), currItem->visualYPos());
				x = currItem->asLine() ? 0 : -lineAdjust;
				y = currItem->asLine() ? 0 : -lineAdjust;
			}
			
//			tt.start();
			psx->drawRect(QRectF(x, y, w, h));
//			tu << QString::number(tt.elapsed());
			if(drawHandles && !currItem->locked())
			{
				psx->setBrush(m_brush["handle"]);
				psx->setPen(m_pen["handle"]);
				if(currItem->asLine())
				{
					psx->setRenderHint(QPainter::Antialiasing);
					psx->drawEllipse(QRectF(x+w-markWidth, y+h/2.0-markWidth, 2* markWidth,2* markWidth));
// 					psx->setBrush(Qt::blue); // sometimes we forget which is what :)
					psx->drawEllipse(QRectF(x-markWidth, y+h/2.0-markWidth, 2* markWidth, 2* markWidth));
				}
				else
				{
					psx->drawRect(QRectF(x+w-markWidth, y+h-markWidth, markWidth, markWidth));
					psx->drawRect(QRectF(x+w/2 - halfMarkWidth, y+h-markWidth, markWidth, markWidth));
					psx->drawRect(QRectF(x+w/2 - halfMarkWidth, y, markWidth, markWidth));
					psx->drawRect(QRectF(x+w-markWidth, y+h/2 - halfMarkWidth, markWidth, markWidth));
					psx->drawRect(QRectF(x+w-markWidth, y, markWidth, markWidth));
					psx->drawRect(QRectF(x, y, markWidth, markWidth));
					psx->drawRect(QRectF(x, y+h/2 - halfMarkWidth, markWidth, markWidth));
					psx->drawRect(QRectF(x, y+h-markWidth, markWidth, markWidth));
				}
			}
			
			psx->restore();
		}
		
	}
	

// 	qDebug()<<ds<< t.elapsed() <<"U"<<tu.join(",")<<"G"<<tg;
}


void CanvasMode::drawOutline(QPainter* p, double scalex, double scaley, double deltax, double deltay)
{
	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	
			
	if (m_doc->m_Selection->count() == 1)
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
				
		if((scalex != 1.0) || (scaley != 1.0)) // changing size of page item
		{
			p->setBrush(m_brush["outline"]);
			p->setPen(m_pen["outline"]);
			p->translate(currItem->xPos(), currItem->yPos());
			p->translate(deltax, deltay);
			if (currItem->rotation() != 0)
			{
				p->setRenderHint(QPainter::Antialiasing);
				p->rotate(currItem->rotation());
			}
			p->scale(scalex, scaley);
			currItem->DrawPolyL(p, currItem->Clip);
		}
		else // moving page item
		{
			QRectF br(currItem->getVisualBoundingRect());
#ifdef GESTURE_FRAME_PREVIEW
			QImage *pixItem(0);
			if( m_pixmapCache.contains(currItem) )
			{
				if( m_pixmapCache.value(currItem)->isReady() )
					pixItem = m_pixmapCache.value(currItem)->getImage();
			}
			else
			{
				m_pixmapCache[currItem] = new PageItemPreview(currItem);
			}

			if(pixItem)
			{
				p->save();
				p->translate(br.x(),br.y());
				p->translate(deltax, deltay);
				p->drawImage( br.toRect(), *pixItem, pixItem->rect() );
				p->restore();
			}
#endif // GESTURE_FRAME_PREVIEW
			{
//				QRect vr(m_canvas->exposedRect());
//				QImage img(vr.width(), vr.height(), QImage::Format_ARGB32);
//				ScPainter scp(&img,vr. width(), vr.height());
//				scp.translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
////				scp.translate(currItem->xPos(), currItem->yPos());
////				scp.translate(deltax, deltay);
////				scp.scale(scalex, scaley);
////				scp.scale(m_canvas->scale(), m_canvas->scale());
//				currItem->invalid = false;
//				currItem->DrawObj(&scp, vr);
//				p->drawImage(vr, img, img.rect() );
				
				if (currItem->isGroup())
				{
					p->save();
					p->setBrush(m_brush["outline"]);
					p->setPen(m_pen["outline"]);
					p->translate(currItem->xPos(), currItem->yPos());
					p->translate(deltax, deltay);
					if (currItem->rotation() != 0)
					{
						p->setRenderHint(QPainter::Antialiasing);
						p->rotate(currItem->rotation());
					}
					p->scale(scalex, scaley);
					PageItem_Group* gItem = currItem->asGroupFrame();
					uint itemCountG = gItem->groupItemList.count();
					if (itemCountG < m_canvas->moveWithFullOutlinesThreshold)
					{
						p->scale(gItem->width() / gItem->groupWidth, gItem->height() / gItem->groupHeight);
						for (uint cg = 0; cg < itemCountG; cg++)
						{
							p->save();
							currItem = gItem->groupItemList.at(cg);
							p->translate(currItem->gXpos, currItem->gYpos);
							currItem->DrawPolyL(p, currItem->Clip);
							p->restore();
						}
					}
					else
					{
						currItem->DrawPolyL(p, currItem->Clip);
					}
					p->restore();
				}
				else
				{
					p->save();
					p->setBrush(m_brush["outline"]);
					p->setPen(m_pen["outline"]);
					p->translate(currItem->xPos(), currItem->yPos());
					p->translate(deltax, deltay);
					if (currItem->rotation() != 0)
					{
						p->setRenderHint(QPainter::Antialiasing);
						p->rotate(currItem->rotation());
					}
					p->scale(scalex, scaley);
					currItem->DrawPolyL(p, currItem->Clip);
					p->restore();
				}
			}
		}	
	}
	else if (m_doc->m_Selection->count() > 1)
	{
		double x, y, w, h;
		m_doc->m_Selection->setGroupRect();
		m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
		uint docSelectionCount = m_doc->m_Selection->count();
		if (docSelectionCount < m_canvas->moveWithBoxesOnlyThreshold)
		{
			PageItem *currItem;
// 			p->translate(x, y);
			for (uint a=0; a<docSelectionCount; ++a)
			{
				currItem = m_doc->m_Selection->itemAt(a);
				if (!m_doc->Items->contains(currItem))
					continue;
				p->save();
				if (docSelectionCount < m_canvas->moveWithFullOutlinesThreshold /*&& currItem->rotation() == 0*/)
				{
					if((scalex != 1.0) || (scaley != 1.0))
					{
						p->setBrush(m_brush["outline"]);
						p->setPen(m_pen["outline"]);
						p->translate(deltax, deltay);
						p->translate(currItem->xPos() /*- x*/, currItem->yPos() /*- y*/);
						p->scale(scalex, scaley);
						if (currItem->rotation() != 0)
						{
							p->setRenderHint(QPainter::Antialiasing);
							p->rotate(currItem->rotation());
						}
						currItem->DrawPolyL(p, currItem->Clip);
					}
					else
					{
						QRectF br(currItem->getVisualBoundingRect());
#ifdef GESTURE_FRAME_PREVIEW
						QImage *pixItem(0);
						if( m_pixmapCache.contains(currItem) )
						{
							if( m_pixmapCache.value(currItem)->isReady() )
								pixItem = m_pixmapCache.value(currItem)->getImage();
						}
						else
						{
							m_pixmapCache[currItem] = new PageItemPreview(currItem);
						}
			

						if(pixItem)
						{
							p->save();
//							p->translate(br.x() /*- x*/, br.y() /*- y*/);
							p->drawImage( br.toRect(), *pixItem, pixItem->rect() );
							p->restore();
						}
#endif  // GESTURE_FRAME_PREVIEW
						{
							p->save();
							p->setBrush(m_brush["outline"]);
							p->setPen(m_pen["outline"]);
							p->translate(currItem->xPos(), currItem->yPos());
							p->translate(deltax, deltay);
							if (currItem->rotation() != 0)
							{
								p->setRenderHint(QPainter::Antialiasing);
								p->rotate(currItem->rotation());
							}
							p->scale(scalex, scaley);
							currItem->DrawPolyL(p, currItem->Clip);
							p->restore();
						}
					}
				}
				else
				{
					p->setBrush(m_brush["outline"]);
					p->setPen(m_pen["outline"]);
					p->translate(deltax, deltay);
					p->translate(x, y);
					if (currItem->rotation() != 0)
					{
						p->setRenderHint(QPainter::Antialiasing);
						p->rotate(currItem->rotation());
					}
					p->drawRect(QRectF(0.0, 0.0, currItem->width()+1.0, currItem->height()+1.0));
				}
				p->restore();
			}
		}
		else
		{
			p->setBrush(m_brush["outline"]);
			p->setPen(m_pen["outline"]);
			p->translate(deltax, deltay);
			p->translate(x, y);
			p->scale(scalex, scaley);
			p->drawRect(QRectF(0, 0, w, h));
		}
	}
	p->restore();
}


void CanvasMode::setModeCursor()
{
	//NOTE: Merge with similar code in ScribusMainWindow::setAppMode()
	switch (m_doc->appMode)
	{
		case modeDrawShapes:
		case modeDrawArc:
		case modeDrawSpiral:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")));
			break;
		case modeDrawImage:
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
		case modeDrawCalligraphicLine:
			qApp->changeOverrideCursor(QCursor(loadIcon("DrawFreeLine.png"), 0, 32));
			break;
		case modeImportObject:
			qApp->changeOverrideCursor(QCursor(loadIcon("DragPix.xpm")));
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
		case modeEditMeshGradient:
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
		case modeInsertPDF3DAnnotation:
		case modeEditArc:
		case modeEditPolygon:
		case modeEditSpiral:
			qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
			break;
		default:
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			break;
	}
}

#ifdef GESTURE_FRAME_PREVIEW
void CanvasMode::clearPixmapCache()
{
	if(m_pixmapCache.count())
	{
		foreach(PageItemPreview* ip, m_pixmapCache)
		{
			if(ip)
				delete ip;
		}
		m_pixmapCache.clear();
	}
}
#endif // GESTURE_FRAME_PREVIEW



void CanvasMode::setResizeCursor(int how, double rot)
{
	switch (how)
	{
		case 1:
		case 2:
			qApp->changeOverrideCursor(ScResizeCursor(135 + rot));// Qt::SizeFDiagCursor
			break;
		case 3:
		case 4:
			qApp->changeOverrideCursor(ScResizeCursor(45 + rot));// Qt::SizeBDiagCursor
			break;
		case 5:
		case 8:
			qApp->changeOverrideCursor(ScResizeCursor(0 + rot));// Qt::SizeVerCursor
			break;
		case 6:
		case 7:
			qApp->changeOverrideCursor(ScResizeCursor(90 + rot));// Qt::SizeHorCursor
			break;
		default:
			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
			break;
	}
}

bool CanvasMode::commonMouseMove(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	if ((m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier)) || ((!(m->modifiers() & Qt::ControlModifier)) && (m->buttons() & Qt::MidButton)))
	{
		if (!m_panGesture)
		{
			m_panGesture = new PanGesture(this);
		}
		m_view->startGesture(m_panGesture);
		m_panGesture->mousePressEvent(m); // Not an error, this is used to register current canvas point
		return true;
	}
	return false;
}

void CanvasMode::commonDrawControls(QPainter* p, bool drawHandles)
{
	if (m_canvas->m_viewMode.operItemMoving)
		drawOutline(p);
	else
		drawSelection(p, drawHandles);
}

void CanvasMode::commonkeyPressEvent_NormalNodeEdit(QKeyEvent *e)
{
	int kk = e->key();
	QString uc = e->text();
	ScribusMainWindow* mainWindow = m_view->m_ScMW;
	QList<QMdiSubWindow *> windows;
	
	QMdiArea* mdiArea = mainWindow->mdiArea;
	QMdiSubWindow* w  = NULL;
	PrefsManager*   prefsManager      = PrefsManager::instance();
	OutlinePalette* outlinePalette    = mainWindow->outlinePalette;
	CheckDocument*  docCheckerPalette = mainWindow->docCheckerPalette;
	QMap<QString, QPointer<ScrAction> >& scrActions(mainWindow->scrActions);

	if (m_keyRepeat)
		return;
	m_keyRepeat = true;

	int keyMod=0;
	if (e->modifiers() & Qt::ShiftModifier)
		keyMod |= Qt::SHIFT;
	if (e->modifiers() & Qt::ControlModifier)
		keyMod |= Qt::CTRL;
	if (e->modifiers() & Qt::AltModifier)
		keyMod |= Qt::ALT;
	//User presses escape and we have a doc open, and we have an item selected
	if (kk == Qt::Key_Escape)
	{
		m_keyRepeat = false;
		PageItem *currItem;
		if (m_doc->m_Selection->count() != 0)
		{
			currItem = m_doc->m_Selection->itemAt(0);
			currItem->Sizing = false;
			if (m_doc->SubMode != -1)
			{
				m_view->Deselect(false);
				m_doc->Items->removeAt(currItem->ItemNr);
			}
			else
				m_view->Deselect(false);
			m_view->cancelGroupTransaction();
		}
		m_doc->DragP = false;
		m_doc->leaveDrag = false;
		m_view->stopAllDrags();
		m_doc->SubMode = -1;
		m_doc->ElemToLink = NULL;
		mainWindow->slotSelect();
		if (m_doc->m_Selection->count() == 0)
			mainWindow->HaveNewSel(-1);
		prefsManager->appPrefs.uiPrefs.stickyTools = false;
		scrActions["stickyTools"]->setChecked(false);
		return;
	}
	Qt::KeyboardModifiers buttonModifiers = e->modifiers();
	/**If we have a doc and we are not changing the page or zoom level in the status bar */
	if ((!m_view->zoomSpinBox->hasFocus()) && (!m_view->pageSelector->hasFocus()))
	{
		//Show our context menu
		QKeySequence currKeySeq = QKeySequence(kk | keyMod);
		if (currKeySeq.matches(scrActions["viewShowContextMenu"]->shortcut()) == QKeySequence::ExactMatch)
		{
			ContextMenu* cmen=NULL;
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			if (m_doc->m_Selection->count() == 0)
			{
				//CB We should be able to get this calculated by the canvas.... it is already in m_canvas->globalToCanvas(m->globalPos());
				FPoint fp = m_canvas->globalToCanvas(QCursor::pos());
				cmen = new ContextMenu(mainWindow, m_doc, fp.x(), fp.y());
			}
			else
				cmen = new ContextMenu(*(m_doc->m_Selection), mainWindow, m_doc);
			if (cmen)
			{
				mainWindow->setUndoMode(true);
				cmen->exec(QCursor::pos());
				mainWindow->setUndoMode(false);
			}
			delete cmen;
		}


		/**
		 * With no item selected we can:
		 * - With space, get into panning mode (modePanning)
		 * - With PageUp, scroll up
		 * - With PageDown, scroll down
		 * - With Tab, change active document windowActivated
		 */

		if (m_doc->m_Selection->count() == 0)
		{
			int pg;
			int wheelVal = prefsManager->mouseWheelJump();
			if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
				wheelVal = qMax(qRound(wheelVal / 10.0), 1);
			switch (kk)
			{
			case Qt::Key_Space:
				m_keyRepeat = false;
				if (m_doc->appMode == modePanning)
					m_view->requestMode(modeNormal);
				else
					m_view->requestMode(modePanning);
				return;
				break;
			case Qt::Key_PageUp:
				if (m_doc->masterPageMode() || m_doc->symbolEditMode())
					m_view->scrollBy(0, -prefsManager->mouseWheelJump());
				else
				{
					pg = m_doc->currentPageNumber();
					if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
						pg--;
					else
						pg -= m_doc->pageSets()[m_doc->pagePositioning()].Columns;
					if (pg > -1)
						m_view->GotoPage(pg);
				}
				m_keyRepeat = false;
				return;
				break;
			case Qt::Key_PageDown:
				if (m_doc->masterPageMode() || m_doc->symbolEditMode())
					m_view->scrollBy(0, prefsManager->mouseWheelJump());
				else
				{
					pg = m_doc->currentPageNumber();
					if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
						pg++;
					else
						pg += m_doc->pageSets()[m_doc->pagePositioning()].Columns;
					if (pg < static_cast<int>(m_doc->Pages->count()))
						m_view->GotoPage(pg);
				}
				m_keyRepeat = false;
				return;
				break;
			case Qt::Key_Left:
				m_view->scrollBy(-wheelVal, 0);
				m_keyRepeat = false;
				return;
				break;
			case Qt::Key_Right:
				m_view->scrollBy(wheelVal, 0);
				m_keyRepeat = false;
				return;
				break;
			case Qt::Key_Up:
				m_view->scrollBy(0, -wheelVal);
				m_keyRepeat = false;
				return;
				break;
			case Qt::Key_Down:
				m_view->scrollBy(0, wheelVal);
				m_keyRepeat = false;
				return;
				break;
			case Qt::Key_Tab:
				if (buttonModifiers == Qt::ControlModifier)
				{
					m_keyRepeat = false;
					windows = mdiArea->subWindowList();
					if (windows.count() > 1)
					{
						for (int i = 0; i < static_cast<int>(windows.count()); ++i)
						{
							if (mdiArea->activeSubWindow() == windows.at(i))
							{
								if (i == static_cast<int>(windows.count()-1))
									w = windows.at(0);
								else
									w = windows.at(i+1);
								break;
							}
						}
						outlinePalette->buildReopenVals();
						docCheckerPalette->clearErrorList();
						if ( w )
							w->showNormal();
						mainWindow->newActWin(w);
					}
					return;
				}
				break;
			}
		}
		/** Now if we have an item selected
		 * - In normal mode we can:
		 * -- Use backspace or delete to delete the item
		 * -- Use itemRaise action shortcut to raise an item (actionmanager.cpp connect, no code here)
		 * -- Use itemLower action shortcut to lower an item (actionmanager.cpp connect, no code here)
		 * -- Use the arrow keys to move an item or group around for !inches:
		 		With no meta, by 1.0 unit
		 		Ctrl, by 10.0 units
		 		Shift by 0.1 units
		 		Ctrl Shift 0.01 units
		 	- For inches:
		 		With no meta, by 1.0 pt
		 		Ctrl, by 1.0 unit
		 		Shift by 0.1 units
		 		Ctrl Shift 0.01 units
		 * -- Use the arrow keys to resize an item:
		 		Alt right arrow, move right side outwards (expand)
		 		Alt left arrow, move left side outwards (expand)
		 		Alt Shift right arrow, move left side inwards (shrink)
		 		Alt Shift left arrow, move right side inwards (shrink)
		 * -- In edit mode of an image frame, use the arrow keys to resize the image:
		 		(flows to pageitem_imageframe for control)
		 		Alt right arrow, move right side of image outwards (expand)
		 		Alt left arrow, move right side inwards (shrink)
		 		Alt down arrow, move bottom side downwards (expand)
		 		Alt up arrow, move top side inwards (shrink)
		 */
		if (m_doc->m_Selection->count() != 0)
		{
			double moveBy=1.0;
			if (m_doc->unitIndex()!=SC_INCHES)
			{
				if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.1;
				else if (!(buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=10.0;
				else if ((buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.01;

				moveBy/=m_doc->unitRatio();//Lets allow movement by the current doc ratio, not only points
			}
			else
			{
				if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.1/m_doc->unitRatio();
				else if (!(buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=1.0/m_doc->unitRatio();
				else if ((buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.01/m_doc->unitRatio();
			}
			bool resizing=((buttonModifiers & Qt::AltModifier) && !(buttonModifiers & Qt::ControlModifier));
			bool resizingsmaller=(resizing && (buttonModifiers & Qt::ShiftModifier));
			double resizeBy=1.0;
			//CB with control locked out due to the requirement of moveby of 0.01, we cannot support
			//resizeby 10 units unless we move to supporting modifier keys that most people dont have.
			//if (buttonModifiers & Qt::ControlModifier)
			//	resizeBy*=10.0;
			resizeBy/=m_doc->unitRatio();
			if (resizingsmaller)
				resizeBy*=-1.0;

			PageItem *currItem = m_doc->m_Selection->itemAt(0);

			switch (kk)
			{
			case Qt::Key_Backspace:
			case Qt::Key_Delete:
				m_doc->itemSelection_DeleteItem();
				break;
				/* CB: Stop using inflexible hardcoded keys here, actions for lower/raise work without this
					per note above with shortcuts.
			case Qt::Key_PageUp:
				if (!currItem->locked())
				{
					m_view->RaiseItem();
				}
				break;
			case Qt::Key_PageDown:
				if (!currItem->locked())
				{
					m_view->LowerItem();
				}
				break;
				*/
			case Qt::Key_Left:
				if (!currItem->locked())
				{
					if (!resizing)
					{
						if ((m_doc->appMode == modeEditClip) && (m_doc->nodeEdit.hasNodeSelected()))
						{
							int storedClRe = m_doc->nodeEdit.ClRe;
							if ((m_doc->nodeEdit.SelNode.count() != 0) && (m_doc->nodeEdit.EdPoints))
							{
								for (int itm = 0; itm < m_doc->nodeEdit.SelNode.count(); ++itm)
								{
									FPoint np;
									int clRe = m_doc->nodeEdit.SelNode.at(itm);
									if (m_doc->nodeEdit.isContourLine)
										np = currItem->ContourLine.point(clRe);
									else
										np = currItem->PoLine.point(clRe);
									m_doc->nodeEdit.ClRe = clRe;
									np = np - FPoint(moveBy, 0);
									m_doc->nodeEdit.moveClipPoint(currItem, np);
								}
							}
							m_doc->nodeEdit.ClRe = storedClRe;
						}
						else
						{
						/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
						/* as the user might be trying to fine tune a position */
							bool sav1 = m_doc->SnapGuides;
							bool sav2 = m_doc->useRaster;
							m_doc->SnapGuides = false;
							m_doc->useRaster = false;
							if (m_doc->m_Selection->count() > 1)
								m_view->startGroupTransaction(Um::Move, "", Um::IMove);
							m_doc->moveGroup(-moveBy, 0);
							if (m_doc->m_Selection->count() > 1)
								m_view->endGroupTransaction();
							m_doc->SnapGuides = sav1;
							m_doc->useRaster = sav2;
						}
					}
					else
					{
						//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
						if(m_doc->nodeEdit.isContourLine)
							m_view->TransformPoly(10, 0, resizeBy/unitGetRatioFromIndex(m_doc->unitIndex()));
						else
						{
							if (resizingsmaller)
							{
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr);
							}
							else
							{
								m_doc->MoveItem(-resizeBy, 0, currItem, false);
								currItem->moveImageXYOffsetBy(resizeBy/currItem->imageXScale(), 0);
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr);
							}
						}
					}
					currItem->update();
					mainWindow->slotDocCh();
				}
				break;
			case Qt::Key_Right:
				if (!currItem->locked())
				{
					if (!resizing)
					{
						if ((m_doc->appMode == modeEditClip) && (m_doc->nodeEdit.hasNodeSelected()))
						{
							int storedClRe = m_doc->nodeEdit.ClRe;
							if ((m_doc->nodeEdit.SelNode.count() != 0) && (m_doc->nodeEdit.EdPoints))
							{
								for (int itm = 0; itm < m_doc->nodeEdit.SelNode.count(); ++itm)
								{
									FPoint np;
									int clRe = m_doc->nodeEdit.SelNode.at(itm);
									if (m_doc->nodeEdit.isContourLine)
										np = currItem->ContourLine.point(clRe);
									else
										np = currItem->PoLine.point(clRe);
									m_doc->nodeEdit.ClRe = clRe;
									np = np + FPoint(moveBy, 0);
									m_doc->nodeEdit.moveClipPoint(currItem, np);
								}
							}
							m_doc->nodeEdit.ClRe = storedClRe;
						}
						else
						{
						/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
						/* as the user might be trying to fine tune a position */
							bool sav1 = m_doc->SnapGuides;
							bool sav2 = m_doc->useRaster;
							m_doc->SnapGuides = false;
							m_doc->useRaster = false;
							if (m_doc->m_Selection->count() > 1)
								m_view->startGroupTransaction(Um::Move, "", Um::IMove);
							m_doc->moveGroup(moveBy, 0);
							if (m_doc->m_Selection->count() > 1)
								m_view->endGroupTransaction();
							m_doc->SnapGuides = sav1;
							m_doc->useRaster = sav2;
						}
					}
					else
					{
						//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
						if(m_doc->nodeEdit.isContourLine)
							m_view->TransformPoly(11, 0, resizeBy/unitGetRatioFromIndex(m_doc->unitIndex()));
						else
						{
							if (resizingsmaller)
							{
								m_doc->MoveItem(-resizeBy, 0, currItem, false);
								currItem->moveImageXYOffsetBy(resizeBy/currItem->imageXScale(), 0);
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr);
							}
							else
							{
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr);
							}
						}
					}
					currItem->update();
					mainWindow->slotDocCh();
				}
				break;
			case Qt::Key_Up:
				if (!currItem->locked())
				{
					if (!resizing)
					{
						if ((m_doc->appMode == modeEditClip) && (m_doc->nodeEdit.hasNodeSelected()))
						{
							int storedClRe = m_doc->nodeEdit.ClRe;
							if ((m_doc->nodeEdit.SelNode.count() != 0) && (m_doc->nodeEdit.EdPoints))
							{
								for (int itm = 0; itm < m_doc->nodeEdit.SelNode.count(); ++itm)
								{
									FPoint np;
									int clRe = m_doc->nodeEdit.SelNode.at(itm);
									if (m_doc->nodeEdit.isContourLine)
										np = currItem->ContourLine.point(clRe);
									else
										np = currItem->PoLine.point(clRe);
									m_doc->nodeEdit.ClRe = clRe;
									np = np - FPoint(0, moveBy);
									m_doc->nodeEdit.moveClipPoint(currItem, np);
								}
							}
							m_doc->nodeEdit.ClRe = storedClRe;
						}
						else
						{
						/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
						/* as the user might be trying to fine tune a position */
							bool sav1 = m_doc->SnapGuides;
							bool sav2 = m_doc->useRaster;
							m_doc->SnapGuides = false;
							m_doc->useRaster = false;
							if (m_doc->m_Selection->count() > 1)
								m_view->startGroupTransaction(Um::Move, "", Um::IMove);
							m_doc->moveGroup(0, -moveBy);
							if (m_doc->m_Selection->count() > 1)
								m_view->endGroupTransaction();
							m_doc->SnapGuides = sav1;
							m_doc->useRaster = sav2;
						}
					}
					else
					{
						//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
						if(m_doc->nodeEdit.isContourLine)
							m_view->TransformPoly(12, 0, resizeBy/unitGetRatioFromIndex(m_doc->unitIndex()));
						else
						{
							if (resizingsmaller)
							{
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr);
							}
							else
							{
								m_doc->MoveItem(0, -resizeBy, currItem, false);
								currItem->moveImageXYOffsetBy(0, resizeBy/currItem->imageYScale());
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr);
							}
						}
					}
					currItem->update();
					mainWindow->slotDocCh();
				}
				break;
			case Qt::Key_Down:
				if (!currItem->locked())
				{
					if (!resizing)
					{
						if ((m_doc->appMode == modeEditClip) && (m_doc->nodeEdit.hasNodeSelected()))
						{
							int storedClRe = m_doc->nodeEdit.ClRe;
							if ((m_doc->nodeEdit.SelNode.count() != 0) && (m_doc->nodeEdit.EdPoints))
							{
								for (int itm = 0; itm < m_doc->nodeEdit.SelNode.count(); ++itm)
								{
									FPoint np;
									int clRe = m_doc->nodeEdit.SelNode.at(itm);
									if (m_doc->nodeEdit.isContourLine)
										np = currItem->ContourLine.point(clRe);
									else
										np = currItem->PoLine.point(clRe);
									m_doc->nodeEdit.ClRe = clRe;
									np = np - FPoint(0, -moveBy);
									m_doc->nodeEdit.moveClipPoint(currItem, np);
								}
							}
							m_doc->nodeEdit.ClRe = storedClRe;
						}
						else
						{
						/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
						/* as the user might be trying to fine tune a position */
							bool sav1 = m_doc->SnapGuides;
							bool sav2 = m_doc->useRaster;
							m_doc->SnapGuides = false;
							m_doc->useRaster = false;
							if (m_doc->m_Selection->count() > 1)
								m_view->startGroupTransaction(Um::Move, "", Um::IMove);
							m_doc->moveGroup(0, moveBy);
							if (m_doc->m_Selection->count() > 1)
								m_view->endGroupTransaction();
							m_doc->SnapGuides = sav1;
							m_doc->useRaster = sav2;
						}
					}
					else
					{
						//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
						if(m_doc->nodeEdit.isContourLine)
							m_view->TransformPoly(13, 0, resizeBy/unitGetRatioFromIndex(m_doc->unitIndex()));
						else
						{
							if (resizingsmaller)
							{
								m_doc->MoveItem(0, -resizeBy, currItem, false);
								currItem->moveImageXYOffsetBy(0, resizeBy/currItem->imageYScale());
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr);
							}
							else
							{
								currItem->Sizing = false;
								m_doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr);
							}
						}
					}
					currItem->update();
					mainWindow->slotDocCh();
				}
				break;
			default:
				break;
			}
		}
	}
	switch(kk)
	{
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
			m_arrowKeyDown = true;
	}
	m_keyRepeat = false;
}

void CanvasMode::commonkeyReleaseEvent(QKeyEvent *e)
{
	//Exit out of panning mode if Control is release while the right mouse button is pressed
	if ((m_doc->appMode == modePanning) && (e->key() == Qt::Key_Control) && (QApplication::mouseButtons() & Qt::RightButton))
		m_view->requestMode(modeNormal);
	if (m_doc->appMode == modeMagnifier)
		qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
	if (e->isAutoRepeat() || !m_arrowKeyDown)
		return;
	switch(e->key())
	{
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
			m_arrowKeyDown = false;
			if ((!m_view->zoomSpinBox->hasFocus()) && (!m_view->pageSelector->hasFocus()))
			{
				int docSelectionCount=m_doc->m_Selection->count();
				if ((docSelectionCount != 0) && (m_doc->appMode == modeEditClip) && (m_doc->nodeEdit.hasNodeSelected()))
				{
					PageItem *currItem = m_doc->m_Selection->itemAt(0);
					double xposOrig = currItem->xPos();
					double yposOrig = currItem->yPos();
					m_doc->AdjustItemSize(currItem);
					if (!m_doc->nodeEdit.isContourLine)
						currItem->ContourLine.translate(xposOrig - currItem->xPos(),yposOrig - currItem->yPos());
					currItem->update();
				}
				for (int i = 0; i < docSelectionCount; ++i)
					m_doc->m_Selection->itemAt(i)->checkChanges(true);
				if (docSelectionCount > 1 && m_view->groupTransactionStarted())
					m_view->endGroupTransaction();
			}
			break;
	}
}
