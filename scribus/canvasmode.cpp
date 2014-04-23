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
#include "canvasmode_edit.h"
#include "canvasmode_editgradient.h"
#include "canvasmode_eyedropper.h"
#include "canvasmode_framelinks.h"
#include "canvasmode_legacy.h"
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
#include "prefsmanager.h"
#include "selection.h"
#include "scpainter.h"
#include "scresizecursor.h"
#include "scribus.h"
#include "scribusview.h"
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
	m_pen["selection"]	= QPen(PrefsManager::instance()->appPrefs.DFrameColor, 1.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection"].setCosmetic(true);
	m_pen["selection-group"] = QPen(Qt::red, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection-group"].setCosmetic(true);
	m_pen["selection-group-inside"] = QPen(PrefsManager::instance()->appPrefs.DFrameGroupColor, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection-group-inside"].setCosmetic(true);
	m_pen["handle"]		= QPen(PrefsManager::instance()->appPrefs.DFrameColor, 1.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["handle"].setCosmetic(true);
	
	m_brush["outline"]	= Qt::NoBrush;
	m_brush["selection"]	= Qt::NoBrush;
	m_brush["selection-group"] = QColor(255,0,0,10);
	m_brush["selection-group-inside"] = Qt::NoBrush;
	m_brush["handle"]	= PrefsManager::instance()->appPrefs.DFrameColor;
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
		case modeDrawBezierLine:
			result = new BezierMode(view);
			break;
		case modeDrawFreehandLine:
			result = new FreehandMode(view);
			break;
		case modeDrawLine:
		case modeDrawShapes:
		case modeDrawImage:
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
				if( currItem->Groups.count() > 0)
				{
					if(! currItem->controlsGroup() )
						continue;
					else
					{
						if(currItem->Groups.count() > 1)
							continue;
					}
				}
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
				
				//Try to detect if the pageitem is a top level item.
				if( currItem->Groups.count() > 0)
				{
					if(! currItem->controlsGroup() ) 
						continue;
					else
					{
						if(currItem->Groups.count() > 1)
							continue;
					}
				}
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

QCursor CanvasMode::modeCursor()
{
	QCursor cursor;
	switch (m_doc->appMode)
	{
		case modeDrawShapes:
			cursor = QCursor(loadIcon("DrawFrame.xpm"));
			break;
		case modeDrawImage:
			cursor =  QCursor(loadIcon("DrawImageFrame.xpm"));
			break;
		case modeDrawLatex:
			cursor = QCursor(loadIcon("DrawLatexFrame.xpm"));
			break;
		case modeDrawText:
			cursor = QCursor(loadIcon("DrawTextFrame.xpm"));
			break;
		case modeDrawTable:
			cursor = QCursor(loadIcon("DrawTable.xpm"));
			break;
		case modeDrawRegularPolygon:
			cursor = QCursor(loadIcon("DrawPolylineFrame.xpm"));
			break;
		case modeDrawLine:
		case modeDrawBezierLine:
			cursor = QCursor(Qt::CrossCursor);
			break;
		case modeDrawFreehandLine:
			cursor = QCursor(loadIcon("DrawFreeLine.png"), 0, 32);
			break;
		case modeImportObject:
			cursor = QCursor(loadIcon("DragPix.xpm"));
			break;
		case modeMagnifier:
			if (m_view->Magnify)
				cursor = QCursor(loadIcon("LupeZ.xpm"));
			else
				cursor = QCursor(loadIcon("LupeZm.xpm"));
			break;
		case modePanning:
			cursor = QCursor(loadIcon("HandC.xpm"));
			break;
		case modeEyeDropper:
			cursor = QCursor(loadIcon("colorpickercursor.png"), 0, 32);
			break;
		case modeLinkFrames:
			cursor = QCursor(loadIcon("LinkTextFrame.png"), 0, 31);
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
			cursor = QCursor(Qt::CrossCursor);
			break;
		default:
			cursor = QCursor(Qt::ArrowCursor);
			break;
	}
	return cursor;
}

void CanvasMode::setModeCursor()
{
	QCursor cursor = modeCursor();
	m_view->setCursor(cursor);
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
			m_view->setCursor(ScResizeCursor(135 + rot));// Qt::SizeFDiagCursor
			break;
		case 3:
		case 4:
			m_view->setCursor(ScResizeCursor(45 + rot));// Qt::SizeBDiagCursor
			break;
		case 5:
		case 8:
			m_view->setCursor(ScResizeCursor(0 + rot));// Qt::SizeVerCursor
			break;
		case 6:
		case 7:
			m_view->setCursor(ScResizeCursor(90 + rot));// Qt::SizeHorCursor
			break;
		default:
			m_view->setCursor(QCursor(Qt::SizeAllCursor));
			break;
	}
}

bool CanvasMode::commonMouseMove(QMouseEvent *m)
{
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
