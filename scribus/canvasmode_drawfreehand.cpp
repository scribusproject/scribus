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


#include "canvasmode_drawfreehand.h"

#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QCursor>
#include <QEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QRect>
#include <QWidgetAction>


#include "canvas.h"
#include "canvasgesture_resize.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hruler.h"
#include "vruler.h"
#include "hyphenator.h"
#include "insertTable.h"
#include "KarbonCurveFit.h"
#include "oneclick.h"
#include "pageitem_textframe.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "scraction.h"
#include "scrapbookpalette.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribusXml.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"



FreehandMode::FreehandMode(ScribusView* view) : CanvasMode(view) 
{
	GxM = GyM = -1;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	MoveGX = MoveGY = false;
	oldCp = Cp = -1;
	frameResizeHandle = -1;
	RotMode = 0;
	inItemCreation = false;
	shiftSelItems = false;
	FirstPoly = true;
}


void FreehandMode::drawControls(QPainter* p) 
{
	//
}

inline bool FreehandMode::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}




// the following code was moved from scribusview.cpp:


void FreehandMode::enterEvent(QEvent *)
{
	if (!m_MouseButtonPressed)
	{
		setModeCursor();
	}
}


void FreehandMode::leaveEvent(QEvent *e)
{
	if (!m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void FreehandMode::activate(bool flag)
{
	qDebug() << "FreehandMode::activate" << flag;
	GxM = GyM = -1;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	MoveGX = MoveGY = false;
	oldCp = Cp = -1;
	frameResizeHandle = -1;
	RotMode = 0;
	inItemCreation = false;
	shiftSelItems = false;
	FirstPoly = true;
	setModeCursor();
}

void FreehandMode::deactivate(bool flag)
{
	qDebug() << "FreehandMode::deactivate" << flag;
	m_view->redrawMarker->hide();
}

void FreehandMode::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	mousePressEvent(m);
}


void FreehandMode::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	double newX, newY;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QPainter p;
	QRect tx;
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());
	if (m_doc->guidesSettings.guidesShown)
	{
		if (MoveGY)
		{
			m_view->FromHRuler(m);
			return;
		}
		if (MoveGX)
		{
			m_view->FromVRuler(m);
			return;
		}
	}

	
	if (m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
	{
		m_view->requestMode(modePanning);
	}

	
	if (m_MouseButtonPressed && (m_doc->appMode == modeDrawFreehandLine))
	{
		//newX = m->x();
		//newY = m->y();
		double newXF = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
		double newYF = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
		if (RecordP.size() > 0)
		{
			if (FPoint(newXF, newYF) != RecordP.point(RecordP.size()-1))
				RecordP.addPoint(FPoint(newXF, newYF));
		}
		else
			RecordP.addPoint(FPoint(newXF, newYF));
		QPolygon& redrawPolygon(m_canvas->newRedrawPolygon());
		for (uint pp = 0; pp < RecordP.size(); pp++)
		{
			redrawPolygon << RecordP.pointQ(pp);
		}
//FIXME		
		m_canvas->m_viewMode.operItemResizing = true;
		QRect bRect = m_canvas->redrawPolygon().boundingRect();
//		QPoint in(qRound(bRect.x()*m_canvas->scale()), qRound(bRect.y()*m_canvas->scale()));
//		in -= QPoint(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
//		QPoint out = contentsToViewport(in);
//		m_view->updateContents(QRect(out.x()+0*contentsX(), out.y()+0*contentsY(), qRound(bRect.width()*m_canvas->scale()), qRound(bRect.height()*m_canvas->scale())).adjusted(-10, -10, 20, 20));
	//	m_canvas->update(bRect);
		m_view->updateCanvas(bRect);
		return;
	}
	
	if ((GetItem(&currItem)) && (!shiftSelItems))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		
		if (m_doc->DragP)
			return;
				
		if ((!m_MouseButtonPressed) && (m_doc->appMode != modeDrawBezierLine))
		{
			if (m_doc->m_Selection->isMultipleSelection())
			{
				setModeCursor();
				return;
			}
			for (int a = 0; a < m_doc->m_Selection->count(); ++a)
			{
				currItem = m_doc->m_Selection->itemAt(a);
				if (currItem->locked())
					break;
				setModeCursor();
			}
		}
	}
	else
	{
		if ((m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton) && (GyM == -1) && (GxM == -1))
		{
			newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
			newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
			SeRx = newX;
			SeRy = newY;
			/*
			if (m_doc->appMode == modeDrawTable)
				m_view->redrawMarker->setGeometry(QRect(Dxp, Dyp, m->globalPos().x() - Dxp, m->globalPos().y() - Dyp).normalized());
			else
				m_view->redrawMarker->setGeometry(QRect(Mxp, Myp, m->globalPos().x() - Mxp, m->globalPos().y() - Myp).normalized());
			*/
			QPoint startP = m_canvas->canvasToGlobal(m_doc->appMode == modeDrawTable? QPointF(Dxp, Dyp) : QPointF(Mxp, Myp));
			m_view->redrawMarker->setGeometry(QRect(startP, m->globalPos()).normalized());
			if (!m_view->redrawMarker->isVisible())
				m_view->redrawMarker->show();
			m_view->HaveSelRect = true;
			return;
		}
	}
}

void FreehandMode::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	QPainter p;
//	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QRect tx;
	QMatrix pm;
	m_MouseButtonPressed = true;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	MoveGX = MoveGY = false;
	inItemCreation = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Rxpd = Mxp - Rxp;
	Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
	Rypd = Myp - Ryp;
	Myp = qRound(Ryp);
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		m_view->DrawNew();
		return;
	}
	if (m->button() != Qt::LeftButton)
	{
		m_view->stopGesture();
		return;
	}
	RecordP.resize(0);
	m_view->Deselect(false);
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	SeRx = Mxp;
	SeRy = Myp;
	m_canvas->setRenderModeFillBuffer();
}



void FreehandMode::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	

	PageItem *currItem;
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
	
	if (m_doc->appMode == modeDrawFreehandLine)
	{
		if (RecordP.size() > 1)
		{
			uint z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Mxp, Myp, 1, 1, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenLine, !m_MouseButtonPressed);
			currItem = m_doc->Items->at(z);
			currItem->PoLine.resize(0);
			if (m->modifiers() & Qt::ControlModifier)
			{
				QList<QPointF> clip;
				for (uint px = 0; px < RecordP.size()-1; ++px)
				{
					FPoint clp = RecordP.point(px);
					clip.append(QPointF(clp.x(), clp.y()));
				}
				QPainterPath pp = bezierFit(clip, 5.0);
				currItem->PoLine.fromQPainterPath(pp);
			}
			else
			{
				currItem->PoLine.addPoint(RecordP.point(0));
				currItem->PoLine.addPoint(RecordP.point(0));
				for (uint px = 1; px < RecordP.size()-1; ++px)
				{
					currItem->PoLine.addPoint(RecordP.point(px));
					currItem->PoLine.addPoint(RecordP.point(px));
					currItem->PoLine.addPoint(RecordP.point(px));
					currItem->PoLine.addPoint(RecordP.point(px));
				}
				currItem->PoLine.addPoint(RecordP.point(RecordP.size()-1));
				currItem->PoLine.addPoint(RecordP.point(RecordP.size()-1));
			}
			FPoint tp2(getMinClipF(&currItem->PoLine));
			currItem->setXYPos(tp2.x(), tp2.y(), true);
			currItem->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp(getMaxClipF(&currItem->PoLine));
			m_doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
			m_doc->AdjustItemSize(currItem);
			m_doc->m_Selection->clear();
			m_doc->m_Selection->addItem(currItem);
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currItem->OwnPage = m_doc->OnPage(currItem);
//FIXME	
			m_canvas->m_viewMode.operItemResizing = false;
			inItemCreation = false;
			m_doc->changed();
//			m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()).adjusted(-10, -10, 20, 20));
		}
		if (!PrefsManager::instance()->appPrefs.stickyTools)
		{
			m_view->requestMode(modeNormal);
//			m_view->requestMode(submodePaintingDone);
		}
		else
			m_view->requestMode(m_doc->appMode);
// itemAdd calls PageItem::update		emit DocChanged();
		return;
	}
	
	if (m_doc->appMode != modeDrawBezierLine)
	{		
		if (inItemCreation)
		{
			currItem = m_doc->m_Selection->itemAt(0);
			double itemX = 0.0;
			double itemY = 0.0;
			if (m_doc->appMode == modeDrawLine)
			{
				QMatrix p = currItem->getTransform();
				FPoint np = m_doc->ApplyGridF(mousePointDoc).transformPoint(p, true);
				itemX = sqrt(pow(np.x(),2.0)+pow(np.y(),2.0));
				itemY = 1.0;
			}
			else if (m_doc->appMode == modeDrawRegularPolygon)
			{
				FPoint np1(mousePointDoc); //m_view->translateToDoc(m->x(), m->y()));
				np1 = m_doc->ApplyGridF(np1);
				itemX = fabs(np1.x() - currItem->xPos());
				itemY = fabs(np1.y() - currItem->yPos());
			}
			else
			{
				itemX = fabs(currItem->width());
				itemY = fabs(currItem->height());
			}
			if ((!m_view->moveTimerElapsed()) || ((itemX < 2.0) && (itemY < 2.0)) || ((m_doc->appMode == modeDrawLine) && (itemX < 2.0)))
			{
				int lmode = 0;
				if (m_doc->appMode == modeDrawLine)
					lmode = 1;
				PrefsContext* sizes = PrefsManager::instance()->prefsFile->getContext("ObjectSize");
				double xSize, ySize;
				int originPoint;
				if (lmode == 0)
				{
					xSize = sizes->getDouble("defWidth", 100.0);
					ySize = sizes->getDouble("defHeight", 100.0);
					originPoint = sizes->getInt("Origin", 0);
				}
				else
				{
					xSize = sizes->getDouble("defLength", 100.0);
					ySize = sizes->getDouble("defAngle", 0.0);
					originPoint = sizes->getInt("OriginL", 0);
				}
				bool doRemember = sizes->getBool("Remember", true);
				bool doCreate = false;
				if (m->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
					doCreate = true;
				else
				{
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					OneClick *dia = new OneClick(m_view, ScribusView::tr("Enter Object Size"), m_doc->unitIndex(), xSize, ySize, doRemember, originPoint, lmode);
					if (dia->exec())
					{
						doRemember = dia->checkRemember->isChecked();
						if (lmode == 0)
						{
							xSize = dia->spinWidth->value() / unitGetRatioFromIndex(m_doc->unitIndex());
							ySize = dia->spinHeight->value() / unitGetRatioFromIndex(m_doc->unitIndex());
							originPoint = dia->RotationGroup->checkedId();
							if (doRemember)
							{
								sizes->set("defWidth", xSize);
								sizes->set("defHeight", ySize);
								sizes->set("Origin", originPoint);
							}
						}
						else
						{
							xSize = dia->spinWidth->value() / unitGetRatioFromIndex(m_doc->unitIndex());
							ySize = dia->spinHeight->value();
							originPoint = dia->RotationGroup->checkedId();
							if (doRemember)
							{
								sizes->set("defLength", xSize);
								sizes->set("defAngle", ySize);
								sizes->set("OriginL", originPoint);
							}
						}
						sizes->set("Remember", doRemember);
						doCreate = true;
					}
					else
					{
						m_view->Deselect(false);
						m_doc->Items->removeAt(currItem->ItemNr);
					}
					delete dia;
				}
				if (doCreate)
				{
					bool oldSnap = m_doc->SnapGuides;
					m_doc->SnapGuides = false;
					m_doc->SizeItem(xSize, ySize, currItem->ItemNr, false, false, false);
					currItem->updateClip();
					currItem->ContourLine = currItem->PoLine.copy();
					switch (originPoint)
					{
						case 0:
							break;
						case 1:
							if (lmode == 0)
								m_doc->MoveItem(-currItem->width(), 0.0, currItem, false);
							else
								m_doc->MoveRotated(currItem, FPoint(-currItem->width() / 2.0, 0.0), false);
							break;
						case 2:
							if (lmode == 0)
								m_doc->MoveItem(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem, false);
							else
								m_doc->MoveRotated(currItem, FPoint(-currItem->width(), 0.0), false);
							break;
						case 3:
							m_doc->MoveItem(0.0, -currItem->height(), currItem, false);
							break;
						case 4:
							m_doc->MoveItem(-currItem->width(), -currItem->height(), currItem, false);
							break;
					}
					m_doc->setRedrawBounding(currItem);
					currItem->OwnPage = m_doc->OnPage(currItem);
					currItem->OldB2 = currItem->width();
					currItem->OldH2 = currItem->height();
					currItem->Sizing = false;
					m_doc->SnapGuides = oldSnap;
					if (!PrefsManager::instance()->appPrefs.stickyTools)
					{
						m_view->requestMode(modeNormal);
//						m_view->requestMode(submodePaintingDone);
					}
					else
					{
						int appMode = m_doc->appMode;
						if ((inItemCreation) && (appMode == modeNormal))
						{
							currItem = m_doc->m_Selection->itemAt(0);
							if (currItem->asTextFrame())
								appMode = modeDrawText;
							else if (currItem->asImageFrame())
								appMode = modeDrawImage;
							else if (m_doc->SubMode != -1)
								appMode = modeDrawShapes;
						}
						m_view->requestMode(appMode);
					}
					currItem->update();
					m_doc->changed();
//					emit DocChanged();
				}
				else
				{
					m_view->requestMode(submodePaintingDone);
//					emit HaveSel(-1);
				}
				inItemCreation = false;
				m_doc->DragP = false;
				m_doc->leaveDrag = false;
//FIXME				m_canvas->m_viewMode.operItemResizing = false;
				m_canvas->setRenderModeUseBuffer(false);
				m_view->MidButt = false;
				shiftSelItems = false;
				m_view->updateCanvas();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			}
		}
		
		if (m_view->moveTimerElapsed() && (GetItem(&currItem)))
		{
			m_canvas->setRenderModeUseBuffer(false);
			if (m_doc->m_Selection->isMultipleSelection())
			{
				
			}
			else
			{
				//Where a single frame resize stops when the mouse buttno is released
				//FIXME: reduce code!
				if (currItem->Sizing)
				{
					FPoint npx;
					double nx = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
					double ny = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
					if (m_doc->SnapGuides)
					{
						m_doc->ApplyGuides(&nx, &ny);
						npx = FPoint(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
					}
					else
					{
						npx = m_doc->ApplyGridF(FPoint(nx, ny));
						npx = FPoint(npx.x(), npx.y(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
					}
					if ((frameResizeHandle == 1) && !(currItem->asLine()) && (m_doc->SnapGuides))
						m_doc->SizeItem(npx.x(), npx.y(), currItem->ItemNr);
					bool sav = m_doc->SnapGuides;
					m_doc->SnapGuides = false;
					if (UndoManager::undoEnabled())
					{
						m_view->undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(),
							Um::Resize, QString(Um::ResizeFromTo).arg(currItem->width()).arg(currItem->height()).arg(currItem->width() - npx.x()).arg(currItem->height() - npx.y()), Um::IResize);
					}
//					m_canvas->m_viewMode.operItemResizing = false;
					switch (frameResizeHandle)
					{
					case 1:
						if (!currItem->asLine())
						{
							//<<Swap location if width/height is <0
							if (currItem->width()<0)
							{
								currItem->setWidth(-currItem->width());
								currItem->setXPos(currItem->xPos()-currItem->width());
							}
							if (currItem->height()<0)
							{
								currItem->setHeight(-currItem->height());
								currItem->setYPos(currItem->yPos()-currItem->height());
							}
							//>>
							if (currItem->imageFlippedH())
								currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
							if (currItem->imageFlippedV())
								currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						}
						else
						{
							if (sav)
							{
								double nx = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
								double ny = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
								if (m_doc->useRaster)
								{
									FPoint ra(m_doc->ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								m_doc->SnapGuides = sav;
								m_doc->ApplyGuides(&nx, &ny);
								m_doc->SnapGuides = false;
								double r = atan2(ny - currItem->yPos(), nx - currItem->xPos())*(180.0/M_PI);
								m_doc->RotateItem(r, currItem->ItemNr);
								double w = sqrt(pow(nx - currItem->xPos(), 2) + pow(ny - currItem->yPos(),2));
								m_doc->SizeItem(w, currItem->height(), currItem->ItemNr, true);
							}
						}
						currItem->Sizing = false;
						break;
					case 2:
						if (!currItem->asLine())
						{
							m_doc->MoveSizeItem(npx, npx, currItem->ItemNr);
								
							//<<Swap location if width/height is <0
							if (currItem->width()<0)
							{
								currItem->setWidth(-currItem->width());
								currItem->setXPos(currItem->xPos()-currItem->width());
							}
							if (currItem->height()<0)
							{
								currItem->setHeight(-currItem->height());
								currItem->setYPos(currItem->yPos()-currItem->height());
							}
							//>>
								
							currItem->Sizing = false;
							if (!currItem->imageFlippedH())
							{
								currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
								currItem->update();
							}
							if (!currItem->imageFlippedV())
							{
								currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
								currItem->update();
							}
						}
						else
						{
							if (sav)
							{
								double nx = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
								double ny = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
								if (m_doc->useRaster)
								{
									FPoint ra(m_doc->ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								m_doc->SnapGuides = sav;
								m_doc->ApplyGuides(&nx, &ny);
								m_doc->SnapGuides = false;
								QMatrix ma;
								ma.translate(currItem->xPos(), currItem->yPos());
								ma.rotate(currItem->rotation());
								double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
								double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
								double r = atan2(my-ny,mx-nx)*(180.0/M_PI);
								double w = sqrt(pow(mx-nx,2)+pow(my-ny,2));
								m_doc->MoveItem(nx - currItem->xPos(), ny - currItem->yPos(), currItem, true);
								m_doc->SizeItem(w, currItem->height(), currItem->ItemNr, true);
								m_doc->RotateItem(r, currItem->ItemNr);
								currItem->Sizing = false;
							}
						}
						break;
					case 3:
						m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(currItem->width() - npx.x(), npx.y()), currItem->ItemNr);
							
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (!currItem->imageFlippedV())
							currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 4:
					
						m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), currItem->height() - npx.y()), currItem->ItemNr);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 5:
						m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, currItem->height() - npx.y()), currItem->ItemNr);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						break;
					case 6:
						m_doc->MoveSizeItem(FPoint(0, 0), FPoint(currItem->width() - npx.x(), 0), currItem->ItemNr);
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
						
						currItem->Sizing = false;
						break;
					case 7:
						m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), currItem->ItemNr);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 8:
						m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), currItem->ItemNr);
							
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (!currItem->imageFlippedV())
							currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					}

					currItem->updateClip();
//					emit ItemTextCols(currItem->Cols, currItem->ColGap);
					m_doc->SnapGuides = sav;
//					evSpon = false;
					currItem->update(); // MoveSizeItem *should*  call PageItem::update
					m_view->updateCanvas();
//					emit DocChanged();
					currItem->checkChanges();
					if (UndoManager::undoEnabled())
						m_view->undoManager->commit();
				}
				m_doc->setRedrawBounding(currItem);
				currItem->OwnPage = m_doc->OnPage(currItem);
//				m_canvas->m_viewMode.operItemResizing = false;
				if (currItem->asLine())
					m_view->updateCanvas();
			}
		}
		//CB Drag selection performed here
		if (((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt)) || ((shiftSelItems) && (m_view->HaveSelRect) && (!m_view->MidButt)))
		{
			QRectF Sele = QRectF(Dxp, Dyp, SeRx-Dxp, SeRy-Dyp).normalized();
			if (!m_doc->masterPageMode())
			{
				uint docPagesCount=m_doc->Pages->count();
				uint docCurrPageNo=m_doc->currentPageNumber();
				for (uint i = 0; i < docPagesCount; ++i)
				{
					if (QRectF(m_doc->Pages->at(i)->xOffset(), m_doc->Pages->at(i)->yOffset(), m_doc->Pages->at(i)->width(), m_doc->Pages->at(i)->height()).intersects(Sele))
					{
						if (docCurrPageNo != i)
						{
							m_doc->setCurrentPage(m_doc->Pages->at(i));
							m_view->setMenTxt(i);
						}
						break;
					}
				}
//FIXME				m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
			}
/* FIXME
				int docItemCount=m_doc->Items->count();
			if (docItemCount != 0)
			{
				m_doc->m_Selection->setIsGUISelection(false);
				for (int a = 0; a < docItemCount; ++a)
				{
					PageItem* docItem = m_doc->Items->at(a);
					QMatrix p;
					m_canvas->Transform(docItem, p);
					QRegion apr = QRegion(docItem->Clip * p);
					QRect apr2(docItem->getRedrawBounding(1.0));
					if ((m_doc->masterPageMode()) && (docItem->OnMasterPage != m_doc->currentPage()->pageName()))
						continue;
					//CB Finally Items are selected here
					if (((Sele.contains(apr.boundingRect())) || (Sele.contains(apr2))) && (docItem->LayerNr == m_doc->activeLayer()) && (!m_doc->layerLocked(docItem->LayerNr)))
					{
					//CB set draw to true to (dis)enable some actions via emit to HaveNewSel in scapp.
					//CB FIXME emit from selection when multiple selected instead
						//CB #3523 bool redrawSelection=(a==docItemCount-1);
						bool redrawSelection=false;
						m_view->SelectItemNr(a, redrawSelection);
					}
				}
				m_doc->m_Selection->setIsGUISelection(true);
				m_doc->m_Selection->connectItemToGUI();
				if (m_doc->m_Selection->count() > 1)
				{
					m_doc->m_Selection->setGroupRect();
					double x, y, w, h;
					m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
//					emit ItemPos(x, y);
//					emit ItemGeom(w, h);
					m_view->getGroupRectScreen(&x, &y, &w, &h);
				}
//				if (m_doc->m_Selection->count() > 0)
//					emit HaveSel(m_doc->m_Selection->itemAt(0)->itemType());
			}*/
			
			m_view->HaveSelRect = false;
			shiftSelItems = false;
			m_view->redrawMarker->hide();
			m_view->updateCanvas();
		}
		if (m_doc->appMode != modeEdit)
		{
			if (!PrefsManager::instance()->appPrefs.stickyTools)
			{
//				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				m_view->requestMode(modeNormal);
//				m_view->requestMode(submodePaintingDone);
			}
			else
			{
				int appMode = m_doc->appMode;
				if ((inItemCreation) && (appMode == modeNormal))
				{
					currItem = m_doc->m_Selection->itemAt(0);
					if (currItem->asTextFrame())
						appMode = modeDrawText;
					else if (currItem->asImageFrame())
						appMode = modeDrawImage;
					else if (m_doc->SubMode != -1)
						appMode = modeDrawShapes;
				}
				m_view->requestMode(appMode);
			}
		}
		if (GetItem(&currItem))
		{
			if (m_doc->m_Selection->count() > 1)
			{
				m_doc->m_Selection->setGroupRect();
				//CB shouldnt need this i think
				//emit HaveSel(currItem->itemType());
				double x, y, w, h;
				m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
//				emit ItemPos(x, y);
//				emit ItemGeom(w, h);
//				m_view->getGroupRectScreen(&x, &y, &w, &h);
				
//				m_canvas->m_viewMode.operItemResizing = false;
				m_canvas->update(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
			}
			else
				currItem->emitAllToGUI();
				
			/*else
			{
				//CB Dont think we need this here with the new selection code
				//For a select, deselect operation, this will cause 2x emit
				//emit HaveSel(currItem->itemType());
				//EmitValues(currItem);
				//currItem->emitAllToGUI();
			}*/
		}
	}
	
	
	m_canvas->setRenderModeUseBuffer(false);
	
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
//	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	shiftSelItems = false;
	inItemCreation = false;
//	m_doc->SubMode = -1;
	if (m_view->groupTransactionStarted())
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->undoManager->commit();
		m_view->setGroupTransactionStarted(false);
	}

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);

	//Commit drag created items to undo manager.
	if (m_doc->m_Selection->itemAt(0)!=NULL)
	{
		m_doc->itemAddCommit(m_doc->m_Selection->itemAt(0)->ItemNr);
	}
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	m_view->zoomSpinBox->clearFocus();
	m_view->pageSelector->clearFocus();
	if (m_doc->m_Selection->itemAt(0) != 0) // is there the old clip stored for the undo action
	{
		currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.finishTransaction(currItem);
	}
	else
	{
		//delete oldClip;
		//oldClip = 0;
	}
}



void FreehandMode::selectPage(QMouseEvent *m)
{
	m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	Mxp = mousePointDoc.x(); //static_cast<int>(m->x()/m_canvas->scale());
	Myp = mousePointDoc.y(); //static_cast<int>(m->y()/m_canvas->scale());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	m_doc->nodeEdit.deselect();
	m_view->Deselect(false);
	if (!m_doc->masterPageMode())
	{
		int i = m_doc->OnPage(Mxp, Myp);
		if (i!=-1)
		{
			uint docCurrPageNo=m_doc->currentPageNumber();
			uint j=static_cast<uint>(i);
			if (docCurrPageNo != j)
			{
				m_doc->setCurrentPage(m_doc->Pages->at(j));
				m_view->setMenTxt(j);
				m_view->DrawNew();
			}
		}
/*		uint docPagesCount=m_doc->Pages->count();
		uint docCurrPageNo=m_doc->currentPageNumber();
		for (uint i = 0; i < docPagesCount; ++i)
		{
			int x = static_cast<int>(m_doc->Pages->at(i)->xOffset() * m_canvas->scale());
			int y = static_cast<int>(m_doc->Pages->at(i)->yOffset() * m_canvas->scale());
			int w = static_cast<int>(m_doc->Pages->at(i)->width() * m_canvas->scale());
			int h = static_cast<int>(m_doc->Pages->at(i)->height() * m_canvas->scale());
			if (QRect(x, y, w, h).intersects(mpo))
			{
				if (docCurrPageNo != i)
				{
					m_doc->setCurrentPage(m_doc->Pages->at(i));
					setMenTxt(i);
					DrawNew();
				}
				break;
			}
		} */

		//FIXME m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
	}
}



