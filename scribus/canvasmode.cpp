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
#include "canvasmode_measurements.h"
#include "canvasmode_nodeedit.h"
#include "canvasmode_drawfreehand.h"
#include "selection.h"
#include "scribusview.h"
#include "util_icon.h"

#include <QPainter>


CanvasMode::CanvasMode (ScribusView* view) :
	QObject(),
	m_view(view),
	m_canvas(view->m_canvas),
	m_doc(view->Doc)
{
	m_pen["outline"]	= QPen(Qt::gray, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["outline"].setCosmetic(true);
	m_pen["selection"]	= QPen(Qt::red, 1.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection"].setCosmetic(true);
	m_pen["selection-group"] = QPen(Qt::red, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["selection-group"].setCosmetic(true);
	m_pen["handle"]		= QPen(Qt::red, 1.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	m_pen["handle"].setCosmetic(true);
	
	m_brush["outline"]	= Qt::NoBrush;
	m_brush["selection"]	= Qt::NoBrush;
	m_brush["selection-group"] = QColor(255,0,0,22);
	m_brush["handle"]	= Qt::red;
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
		case modeMeasurementTool:
			result = new MeasurementsMode(view);
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
	QTime t;
	t.start();
	QTime tt;
	int tg(0);
	QStringList tu;
	QString ds;
	psx->scale(m_canvas->scale(), m_canvas->scale());
	psx->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	
	psx->setClipping(true);
	psx->setClipRegion(QRegion ( m_canvas->exposedRect() ) );
	if (m_doc->m_Selection->isMultipleSelection())
	{
		bool drawHandles(true);
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

		tt.start();
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
		tg=tt.elapsed();
		psx->restore();
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
			if (currItem->rotation() != 0)
			{
				psx->setRenderHint(QPainter::Antialiasing);
				psx->translate(currItem->xPos(), currItem->yPos());
				psx->rotate(currItem->rotation());
				x = currItem->lineWidth()/-2.0;
				y = currItem->lineWidth()/-2.0;
			}
			else
			{
				psx->translate(currItem->visualXPos(), currItem->visualYPos());
				x = -lineAdjust;
				y = -lineAdjust;
			}
			w = currItem->visualWidth() ;
			h = currItem->visualHeight() ;
			
			tt.start();
			psx->drawRect(QRectF(x, y, w, h));
			tu << QString::number(tt.elapsed());
			if(!currItem->locked())
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

			QImage *pixItem(0);
			if( m_pixmapCache.contains(currItem) )
				pixItem = m_pixmapCache.value(currItem);
			else
			{
				pixItem = new QImage( currItem->DrawObj_toImage() );
				QImage aMask(pixItem->size(), QImage::Format_Indexed8);
				aMask.fill(128);
				pixItem->setAlphaChannel(aMask);
				
				m_pixmapCache[currItem] = pixItem;
			}
			QRectF br(currItem->getVisualBoundingRect());
			{
				p->save();
				p->translate(br.x(),br.y());
				p->translate(deltax, deltay);
				p->drawImage( pixItem->rect(), *pixItem, pixItem->rect() );
				p->restore();
			}
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
						QImage *pixItem(0);
						if( m_pixmapCache.contains(currItem) )
							pixItem = m_pixmapCache.value(currItem);
						else
						{
							pixItem = new QImage( currItem->DrawObj_toImage() );
							QImage aMask(pixItem->size(), QImage::Format_Indexed8);
							aMask.fill(128);
							pixItem->setAlphaChannel(aMask);
							m_pixmapCache[currItem] = pixItem;
						}
						QRectF br(currItem->getVisualBoundingRect());
						{
							p->save();
							p->translate(br.x() /*- x*/, br.y() /*- y*/);
							p->drawImage( pixItem->rect(), *pixItem, pixItem->rect() );
							p->restore();
						}
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

void CanvasMode::clearPixmapCache()
{
	if(m_pixmapCache.count())
	{
		foreach(QImage* ip, m_pixmapCache)
		{
			if(ip)
				delete ip;
		}
		m_pixmapCache.clear();
	}
}


