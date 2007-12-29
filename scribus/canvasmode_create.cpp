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


#include "canvasmode_create.h"

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



CreateMode::CreateMode(ScribusView* view) : CanvasMode(view) 
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
	resizeGesture = NULL;
}


void CreateMode::drawControls(QPainter* p) 
{
	//
}

inline bool CreateMode::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}




// the following code was moved from scribusview.cpp:


void CreateMode::enterEvent(QEvent *)
{
	if (!m_MouseButtonPressed)
	{
		setModeCursor();
	}
}


void CreateMode::leaveEvent(QEvent *e)
{
	if (!m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CreateMode::activate(bool fromGesture)
{
	PageItem* currItem;
	qDebug() << "CreateMode::activate" << fromGesture;
	if (fromGesture && GetItem(&currItem))
	{
		double itemX = fabs(currItem->width());
		double itemY = fabs(currItem->height());
		if ((!m_view->moveTimerElapsed()) || ((itemX < 2.0) && (itemY < 2.0)))
		{
			PrefsContext* sizes = PrefsManager::instance()->prefsFile->getContext("ObjectSize");
			double xSize, ySize;
			int originPoint;
			xSize = sizes->getDouble("defWidth", 100.0);
			ySize = sizes->getDouble("defHeight", 100.0);
			originPoint = sizes->getInt("Origin", 0);
			bool doRemember = sizes->getBool("Remember", true);
			bool doCreate = false;
//			if (m->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
//				doCreate = true;
//			else
			{
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				OneClick *dia = new OneClick(m_view, ScribusView::tr("Enter Object Size"), m_doc->unitIndex(), xSize, ySize, doRemember, originPoint, 0);
				if (dia->exec())
				{
					doRemember = dia->checkRemember->isChecked();
					xSize = dia->spinWidth->value() / unitGetRatioFromIndex(m_doc->unitIndex());
					ySize = dia->spinHeight->value() / unitGetRatioFromIndex(m_doc->unitIndex());
					originPoint = dia->RotationGroup->checkedId();
					if (doRemember)
					{
						sizes->set("defWidth", xSize);
						sizes->set("defHeight", ySize);
						sizes->set("Origin", originPoint);
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
				currItem->Sizing = false;
				m_doc->SizeItem(xSize, ySize, currItem->ItemNr, false, true, false);
				m_doc->AdjustItemSize(currItem);
				currItem->ContourLine = currItem->PoLine.copy();
				switch (originPoint)
				{
					case 0:
						break;
					case 1:
						m_doc->MoveItem(-currItem->width(), 0.0, currItem, false);
						break;
					case 2:
						m_doc->MoveItem(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem, false);
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
				currItem->update();
			}
			inItemCreation = false;
		}
		else
		{
			m_doc->AdjustItemSize(currItem);
//			currItem->OldB2 = currItem->width();
//			currItem->OldH2 = currItem->height();
//			currItem->updateClip();
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			currItem->Sizing = false;
			currItem->update();
			inItemCreation = false;
		}
		m_doc->changed();
		if (!PrefsManager::instance()->appPrefs.stickyTools)
		{
			m_view->requestMode(modeNormal);
		}
	}
	else
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
	setModeCursor();
}

void CreateMode::deactivate(bool flag)
{
	qDebug() << "CreateMode::deactivate" << flag;
	m_view->redrawMarker->hide();
}

void CreateMode::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
}


void CreateMode::mouseMoveEvent(QMouseEvent *m)
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
/*	if (m_doc->guidesSettings.guidesShown)
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
*/
	
	if (m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
	{
		m_view->requestMode(modePanning);
	}

		
	if ((GetItem(&currItem)) && (!shiftSelItems))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		
		if (m_doc->DragP)
			return;
		
		/*
		if (m_MouseButtonPressed && (m_doc->appMode == modeDrawRegularPolygon))
		{
			if (m->modifiers() & Qt::ShiftModifier)
			{
//				mop = QPoint(m->x(), static_cast<int>((currItem->yPos() + (newX - currItem->xPos())) * sc));
				newY = static_cast<int>(currItem->yPos() + (newX - currItem->xPos()));
				QCursor::setPos(m_canvas->canvasToGlobal(FPoint(newX, newY)));
//				newY = static_cast<int>(mop.y()/sc);
			}
			if ((m_doc->useRaster) && (m_doc->OnPage(currItem) != -1))
			{
				newX = qRound(newX / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
				newY = qRound(newY / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
			}
/ *			QMatrix mp = currItem->getTransform();
//			mp.translate(currItem->xPos(),// - m_doc->minCanvasCoordinate.x(), 
//						 currItem->yPos()); // - m_doc->minCanvasCoordinate.y());
//			mp.rotate(currItem->rotation());
			np2 = QPoint(static_cast<int>(newX), static_cast<int>(newY)) * mp.inverted();
			QMatrix pm;
			m_canvas->Transform(currItem, pm);
			m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), np2)));
* /
			m_view->updateCanvas();
//			emit ItemGeom(newX - currItem->xPos(), newY - currItem->yPos());
			Mxp = newX;
			Myp = newY;
		}*/
	}
	else
	{
		if ((m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton) && (GyM == -1) && (GxM == -1))
		{
			newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
			if (m_doc->appMode == modeMagnifier)
				newY = qRound(Myp + ((SeRx - Mxp) * m_view->visibleHeight()) / m_view->visibleWidth());
			else
				newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
			if (m_doc->appMode == modeDrawTable)
			{
				FPoint np2 = m_doc->ApplyGridF(FPoint(newX, newY));
				double nx = np2.x();
				double ny = np2.y();
				m_doc->ApplyGuides(&nx, &ny);
				newX = qRound(nx);
				newY = qRound(ny);
				GyM = -1;
				GxM = -1;
//			}
			SeRx = newX;
			SeRy = newY;
			QPoint startP = m_canvas->canvasToGlobal(m_doc->appMode == modeDrawTable? QPointF(Dxp, Dyp) : QPointF(Mxp, Myp));
			m_view->redrawMarker->setGeometry(QRect(startP, m->globalPos()).normalized());
			if (!m_view->redrawMarker->isVisible())
				m_view->redrawMarker->show();
			m_view->HaveSelRect = true;
			}
		}
	}
}

void CreateMode::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	int z;
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
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
	switch (m_doc->appMode)
	{
		case modeDrawShapes:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			switch (m_doc->SubMode)
			{
			case 0:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 0;
					SetupDrawNoResize(z);
				}
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 0;
					SetupDraw(z);
				}
				break;
			case 1:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 1;
					SetupDrawNoResize(z);
				}
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 1;
					SetupDraw(z);
				}
				break;
			default:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
					m_doc->Items->at(z)->SetFrameShape(m_doc->ValCount, m_doc->ShapeValues);
					m_doc->setRedrawBounding(m_doc->Items->at(z));
					m_doc->Items->at(z)->FrameType = m_doc->SubMode+2;
					SetupDrawNoResize(z);
				}
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
					m_doc->Items->at(z)->SetFrameShape(m_doc->ValCount, m_doc->ShapeValues);
					m_doc->setRedrawBounding(m_doc->Items->at(z));
					m_doc->Items->at(z)->FrameType = m_doc->SubMode+2;
					SetupDraw(z);
				}
				break;
			}
			break;
		case modeDrawLatex:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawImage:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawText:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}	
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawRegularPolygon:
			{
				if (m->button() != Qt::LeftButton)
					break;
				selectPage(m);
				if (m->modifiers() == Qt::ShiftModifier)
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_MouseButtonPressed);
				}
				currItem = m_doc->Items->at(z);
				FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), m_doc->toolSettings.polyC, m_doc->toolSettings.polyS, m_doc->toolSettings.polyF, m_doc->toolSettings.polyR);
				FPoint np(cli.point(0));
				currItem->PoLine.resize(2);
				currItem->PoLine.setPoint(0, np);
				currItem->PoLine.setPoint(1, np);
				for (uint ax = 1; ax < cli.size(); ++ax)
				{
					np = FPoint(cli.point(ax));
					currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
				}
				np = FPoint(cli.point(0));
				currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
				currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
				if (m->modifiers() == Qt::ShiftModifier)
				{
					SetupDrawNoResize(z);
				}	
				else
				{
					SetupDraw(z);
				}
				/*
				qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
				m_doc->m_Selection->clear();
				m_doc->m_Selection->addItem(currItem);
				currItem->update();
				inItemCreation = true;
//FIXME:				m_canvas->m_viewMode.operItemResizing = true;
				if (m->modifiers() == Qt::ShiftModifier)
				{
					m_view->requestMode(modeNormal);
// itemAdd calls PageItem::update					emit DocChanged();
					inItemCreation = false;
//					m_view->updateContents();
				}
				else
				{
//FIXME:					m_canvas->m_viewMode.operItemMoving = true;
					m_canvas->setRenderModeFillBuffer();
				}*/
				break;
			}
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			m_doc->ApplyGuides(&Rxp, &Ryp);
			z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_MouseButtonPressed);
			currItem = m_doc->Items->at(z);
			currItem->setIsAnnotation(true);
			currItem->AutoName = false;
			switch (m_doc->appMode)
			{
			case modeInsertPDFButton:
				currItem->annotation().setType(2);
				currItem->annotation().setFlag(65536);
				break;
			case modeInsertPDFTextfield:
				currItem->annotation().setType(3);
				break;
			case modeInsertPDFCheckbox:
				currItem->annotation().setType(4);
				break;
			case modeInsertPDFCombobox:
				currItem->annotation().setType(5);
				currItem->annotation().setFlag(131072);
				break;
			case modeInsertPDFListbox:
				currItem->annotation().setType(6);
				break;
			case modeInsertPDFTextAnnotation:
				currItem->annotation().setType(10);
				break;
			case modeInsertPDFLinkAnnotation:
				currItem->annotation().setType(11);
				currItem->annotation().setZiel(m_doc->currentPage()->pageNr());
				currItem->annotation().setAction("0 0");
				currItem->setTextFlowMode(PageItem::TextFlowDisabled);
				break;
			}
			SetupDraw(z);
			break;
		case modeDrawTable:
			if (m->button() != Qt::LeftButton)
				break;
			m_view->Deselect(false);
//			Mxp = qRound(m->x()/m_canvas->scale() + m_doc->minCanvasCoordinate.x());
//			Myp = qRound(m->y()/m_canvas->scale() + m_doc->minCanvasCoordinate.y());
			Rxp = mousePointDoc.x(); //m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x();
			Ryp = mousePointDoc.y(); //m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y();
			npf = m_doc->ApplyGridF(FPoint(Rxp, Ryp));
			Rxp = npf.x();
			Ryp = npf.y();
			m_doc->ApplyGuides(&Rxp, &Ryp);
			GyM = -1;
			GxM = -1;
			Mxp = qRound(Rxp);
			Myp = qRound(Ryp);
			SeRx = Mxp;
			SeRy = Myp;
			Dxp = m->globalPos().x();
			Dyp = m->globalPos().y();
			m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			m_view->redrawMarker->show();
			break;
	}
}



void CreateMode::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	PageItem *currItem;
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
	
	if (GetItem(&currItem))
	{	
		if (m_doc->appMode == modeDrawTable)
		{
			if ((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt))
			{
				QRect AreaR = QRect(static_cast<int>(Mxp), static_cast<int>(Myp), static_cast<int>(SeRx-Mxp), static_cast<int>(SeRy-Myp)).normalized();
				m_view->HaveSelRect = false;
				double Tx, Ty, Tw, Th;
				FPoint np2 = m_doc->ApplyGridF(FPoint(Mxp, Myp));
				Tx = np2.x();
				Ty = np2.y();
				m_doc->ApplyGuides(&Tx, &Ty);
				Mxp = qRound(Tx);
				Myp = qRound(Ty);
				np2 = m_doc->ApplyGridF(FPoint(SeRx, SeRy));
				Tw = np2.x();
				Th = np2.y();
				m_doc->ApplyGuides(&Tw, &Th);
				SeRx = qRound(Tw);
				SeRy = qRound(Th);
				Tw = Tw - Tx;
				Th = Th - Ty;
				int z;
				int Cols, Rows;
				double deltaX, deltaY, offX, offY;
				if ((Th < 6) || (Tw < 6))
				{
					m_view->redrawMarker->hide();
					m_view->requestMode(submodePaintingDone);
					return;
				}
				InsertTable *dia = new InsertTable(m_view, static_cast<int>(Th / 6), static_cast<int>(Tw / 6));
				if (!dia->exec())
				{
					m_view->redrawMarker->hide();
					m_view->requestMode(submodePaintingDone);
					delete dia;
					dia=NULL;
					return;
				}
				m_view->redrawMarker->hide();
				Cols = dia->Cols->value();
				Rows = dia->Rows->value();
				delete dia;
				dia=NULL;
				deltaX = Tw / Cols;
				deltaY = Th / Rows;
				offX = 0.0;
				offY = 0.0;
				m_doc->m_Selection->clear();
				if (UndoManager::undoEnabled())
					m_view->undoManager->beginTransaction(m_doc->currentPage()->getUName(),
														  Um::ITable, Um::CreateTable,
														  QString(Um::RowsCols).arg(Rows).arg(Cols),
														  Um::ICreate);
				for (int rc = 0; rc < Rows; ++rc)
				{
					for (int cc = 0; cc < Cols; ++cc)
					{
						//z = PaintText(Tx + offX, Ty + offY, deltaX, deltaY, m_doc->toolSettings.dWidth, m_doc->toolSettings.dPenText);
						z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Tx + offX, Ty + offY, deltaX, deltaY, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_MouseButtonPressed);
						currItem = m_doc->Items->at(z);
						currItem->isTableItem = true;
						//currItem->setTextFlowsAroundFrame(true);
						//currItem->setTextFlowUsesBoundingBox(true);
						currItem->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
						m_doc->m_Selection->addItem(currItem);
						offX += deltaX;
					}
					offY += deltaY;
					offX = 0.0;
				}
				for (int rc = 0; rc < Rows; ++rc)
				{
				for (int cc = 0; cc < Cols; ++cc)
				{
					currItem = m_doc->m_Selection->itemAt((rc * Cols) + cc);
					if (rc == 0)
						currItem->TopLink = 0;
					else
						currItem->TopLink = m_doc->m_Selection->itemAt(((rc-1)*Cols)+cc);
					if (rc == Rows-1)
						currItem->BottomLink = 0;
					else
						currItem->BottomLink = m_doc->m_Selection->itemAt(((rc+1)*Cols)+cc);
					if (cc == 0)
						currItem->LeftLink = 0;
					else
						currItem->LeftLink = m_doc->m_Selection->itemAt((rc*Cols)+cc-1);
					if (cc == Cols-1)
						currItem->RightLink = 0;
					else
						currItem->RightLink = m_doc->m_Selection->itemAt((rc*Cols)+cc+1);
				}
				}
				m_doc->itemSelection_GroupObjects(false, false);
				if (UndoManager::undoEnabled())
					m_view->undoManager->commit();
			}
		} // if (modeDrawTable)
		
		if ((m_doc->appMode == modeDrawRegularPolygon) && (inItemCreation))
		{
			double itemX = fabs(currItem->width());
			double itemY = fabs(currItem->height());
			if ((!m_view->moveTimerElapsed()) || ((itemX < 2.0) && (itemY < 2.0)))
			{
				PrefsContext* sizes = PrefsManager::instance()->prefsFile->getContext("ObjectSize");
				double xSize, ySize;
				int originPoint;
				xSize = sizes->getDouble("defWidth", 100.0);
				ySize = sizes->getDouble("defHeight", 100.0);
				originPoint = sizes->getInt("Origin", 0);
				bool doRemember = sizes->getBool("Remember", true);
				bool doCreate = false;
				if (m->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
					doCreate = true;
				else
				{
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					OneClick *dia = new OneClick(m_view, ScribusView::tr("Enter Object Size"), m_doc->unitIndex(), xSize, ySize, doRemember, originPoint, 0);
					if (dia->exec())
					{
						doRemember = dia->checkRemember->isChecked();
						xSize = dia->spinWidth->value() / unitGetRatioFromIndex(m_doc->unitIndex());
						ySize = dia->spinHeight->value() / unitGetRatioFromIndex(m_doc->unitIndex());
						originPoint = dia->RotationGroup->checkedId();
						if (doRemember)
						{
							sizes->set("defWidth", xSize);
							sizes->set("defHeight", ySize);
							sizes->set("Origin", originPoint);
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
					currItem->setWidthHeight(xSize, ySize);
					FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), m_doc->toolSettings.polyC, m_doc->toolSettings.polyS, m_doc->toolSettings.polyF, m_doc->toolSettings.polyR);
					FPoint np(cli.point(0));
					currItem->PoLine.resize(2);
					currItem->PoLine.setPoint(0, np);
					currItem->PoLine.setPoint(1, np);
					for (uint ax = 1; ax < cli.size(); ++ax)
					{
						np = FPoint(cli.point(ax));
						currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
					}
					np = FPoint(cli.point(0));
					currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
					FPoint tp2(getMinClipF(&currItem->PoLine));
					if ((tp2.x() > -1) || (tp2.y() > -1))
						m_doc->SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem->ItemNr, false, false, false);
					FPoint tp(getMaxClipF(&currItem->PoLine));
					m_doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
					//						currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
					m_doc->AdjustItemSize(currItem);
					currItem->ContourLine = currItem->PoLine.copy();
					switch (originPoint)
					{
						case 0:
							break;
						case 1:
							m_doc->MoveItem(-currItem->width(), 0.0, currItem, false);
							break;
						case 2:
							m_doc->MoveItem(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem, false);
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
					currItem->update();
				}
			}
			else // not OneClick
			{
				FPoint np1(mousePointDoc); //m_view->translateToDoc(m->x(), m->y()));
				np1 = m_doc->ApplyGridF(np1);
				double w = np1.x() - currItem->xPos();
				double h = np1.y()- currItem->yPos();
				currItem->setWidthHeight(fabs(w), fabs(h));
				if (w < 0.0)
				{
					currItem->setXPos(currItem->xPos() - fabs(w), true);
				}
				if (h < 0.0)
				{
					currItem->setYPos(currItem->yPos() - fabs(h), true);
				}
				FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), m_doc->toolSettings.polyC, m_doc->toolSettings.polyS, m_doc->toolSettings.polyF, m_doc->toolSettings.polyR);
				FPoint np(cli.point(0));
				currItem->PoLine.resize(2);
				currItem->PoLine.setPoint(0, np);
				currItem->PoLine.setPoint(1, np);
				for (uint ax = 1; ax < cli.size(); ++ax)
				{
					np = FPoint(cli.point(ax));
					currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
				}
				np = FPoint(cli.point(0));
				currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
				FPoint tp2(getMinClipF(&currItem->PoLine));
				if ((tp2.x() > -1) || (tp2.y() > -1))
				{
					m_doc->SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem->ItemNr, false, false, false);
				}
				FPoint tp(getMaxClipF(&currItem->PoLine));
				m_doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
				//			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
				m_doc->AdjustItemSize(currItem);
				currItem->ContourLine = currItem->PoLine.copy();
				m_doc->setRedrawBounding(currItem);
				currItem->OwnPage = m_doc->OnPage(currItem);
				currItem->OldB2 = currItem->width();
				currItem->OldH2 = currItem->height();
				//FIXME			m_canvas->m_viewMode.operItemResizing = false;
				m_canvas->setRenderModeUseBuffer(false);
				m_view->updateCanvas();
			}
		} // if (modeDrawRegPoly)
	} // ! GetItem()
	if (!PrefsManager::instance()->appPrefs.stickyTools)
	{
		m_view->requestMode(modeNormal);
	}
	else
	{
		int appMode = m_doc->appMode;
		m_view->requestMode(appMode);
	}
	inItemCreation = false;
}



void CreateMode::selectPage(QMouseEvent *m)
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


void CreateMode::SetupDraw(int nr)
{
	PageItem* currItem = m_doc->Items->at(nr);
	//	currItem->setFont(Doc->toolSettings.defFont);
	//	currItem->setFontSize(Doc->toolSettings.defSize);
//FIXME	m_canvas->m_viewMode.operItemResizing = true;
	frameResizeHandle = 1;
	qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
	m_doc->m_Selection->setIsGUISelection(true);
	m_doc->m_Selection->clear();
	m_doc->m_Selection->addItem(currItem);
	m_doc->m_Selection->connectItemToGUI();
	currItem->update();
	if (!resizeGesture)
		resizeGesture = new ResizeGesture(this);
	m_view->startGesture(resizeGesture);
//	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	inItemCreation = true;
	m_canvas->setRenderModeFillBuffer();
	m_view->resetMoveTimer();
}

void CreateMode::SetupDrawNoResize(int nr)
{
	PageItem* currItem = m_doc->Items->at(nr);
	//	currItem->setFont(Doc->toolSettings.defFont);
	//	currItem->setFontSize(Doc->toolSettings.defSize);
	m_doc->m_Selection->setIsGUISelection(true);
	m_doc->m_Selection->clear();
	m_doc->m_Selection->addItem(currItem);
	m_doc->m_Selection->connectItemToGUI();
//	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	m_view->resetMoveTimer();
}

void CreateMode::setResizeCursor(int how)
{
	switch (how)
	{
		case 1:
		case 2:
			qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			break;
		case 3:
		case 4:
			qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
			break;
		case 5:
		case 8:
			qApp->changeOverrideCursor(QCursor(Qt::SizeVerCursor));
			break;
		case 6:
		case 7:
			qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			break;
		default:
			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
			break;
	}
}

