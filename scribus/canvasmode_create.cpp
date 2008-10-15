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
	canvasPressCoord.setXY(-1.0, -1.0);
	mouseGlobalCoord.setXY(-1.0, -1.0);
	inItemCreation = false;
	createObjectMode     = 0;
	createObjectSubMode  = 0;
	modifiers            = 0;
	m_MouseButtonPressed = false;
	m_createTransaction  = NULL;
}


void CreateMode::drawControls(QPainter* p) 
{
	if (!inItemCreation) return;

	QPointF topLeft(createObjectPos.x(), createObjectPos.y());
	QPointF btRight(canvasCurrCoord.x(), canvasCurrCoord.y());
	QColor  drawColor = qApp->palette().color(QPalette::Active, QPalette::Highlight);

	if (createObjectMode != modeDrawLine)
	{
		QRectF  bounds = QRectF(topLeft, btRight).normalized();
		QRect   localRect = m_canvas->canvasToLocal(bounds);

		p->setRenderHint(QPainter::Antialiasing);

		p->save();
		p->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		drawColor.setAlpha(64);
		p->setBrush(drawColor);
		p->drawRect(localRect);

		drawColor.setAlpha(255);
		p->setBrush(Qt::NoBrush);
		p->setPen(QPen(drawColor, 1, Qt::DashLine, Qt::FlatCap, Qt::MiterJoin));

		int frameType = 0, itemType = 0;
		getFrameItemTypes(itemType, frameType);
		if (frameType == PageItem::Ellipse)
		{
			p->drawEllipse(localRect);
		}
		else if (createObjectMode == modeDrawRegularPolygon)
		{
			FPointArray cli = RegularPolygonF(localRect.width(), localRect.height(), m_doc->toolSettings.polyC, m_doc->toolSettings.polyS, m_doc->toolSettings.polyF, m_doc->toolSettings.polyR);
			FPoint np(cli.point(0));
			QPainterPath path;
			path.moveTo(np.x(), np.y());
			for (uint ax = 1; ax < cli.size(); ++ax)
			{
				np = FPoint(cli.point(ax));
				path.lineTo(np.x(), np.y());
			}
			np = FPoint(cli.point(0));
			path.lineTo(np.x(), np.y());
			p->translate(localRect.left(), localRect.top());
			p->drawPath(path);
		}
		else if ((createObjectMode == modeDrawShapes) && (createObjectSubMode > 1))
		{
			FPointArray poly;
			int valCount = m_doc->ValCount;
			double *vals = m_doc->ShapeValues;
			for (int a = 0; a < valCount-3; a += 4)
			{
				if (vals[a] < 0)
				{
					poly.setMarker();
					continue;
				}
				double x1 = localRect.width()  * vals[a] / 100.0;
				double y1 = localRect.height() * vals[a+1] / 100.0;
				double x2 = localRect.width()  * vals[a+2] / 100.0;
				double y2 = localRect.height() * vals[a+3] / 100.0;
				poly.addPoint(x1, y1);
				poly.addPoint(x2, y2);
			}
			QPainterPath path = poly.toQPainterPath(false);
			p->translate(localRect.left(), localRect.top());
			p->drawPath(path);
		}
		p->restore();
	}
	else
	{
		QPoint p1 = m_canvas->canvasToLocal(topLeft);
		QPoint p2 = m_canvas->canvasToLocal(btRight);
		
		p->save();
		p->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		p->setBrush(drawColor);
		p->drawLine(p1, p2);
		p->restore();
	}
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
//	qDebug() << "CreateMode::activate" << fromGesture;
	if (!fromGesture || !GetItem(&currItem) || !m_createTransaction)
	{
		if (m_createTransaction)
		{
//			qDebug() << "canceling left over create Transaction";
			m_createTransaction->cancel();
			delete m_createTransaction;
			m_createTransaction = NULL;
		}
		canvasPressCoord.setXY(-1.0, -1.0);
		mouseGlobalCoord.setXY(-1.0, -1.0);
		inItemCreation = false;
	}		
	setModeCursor();
}

void CreateMode::deactivate(bool forGesture)
{
//	qDebug() << "CreateMode::deactivate" << forGesture;
	if (!forGesture)
	{		
		if (m_createTransaction)
		{
//			qDebug() << "CreateMode::deactivate: canceling left over create Transaction";
			m_createTransaction->cancel();
			delete m_createTransaction;
			m_createTransaction = NULL;
		}
	}
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
	modifiers = m->modifiers();
	
	double newX, newY;
	PageItem *currItem;
	QPoint np, np2, mop;
	QPainter p;
	QRect tx;
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());
	
	if (m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
	{
		m_view->requestMode(modePanning);
	}
		
	if (GetItem(&currItem))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		
		if (m_doc->DragP)
			return;
	}
	else
	{
		if ((m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
		{
			newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
			newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
			if (createObjectMode == modeDrawLine)
			{
				if (m_doc->useRaster)
				{
					newX = qRound(newX / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
					newY = qRound(newY / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
				}
				if (m->modifiers() & Qt::ControlModifier)
				{
					QRectF bounds(QPoint(createObjectPos.x(), createObjectPos.y()), QPoint(newX, newY));
					double newRot = xy2Deg(bounds.width(), bounds.height());
					if (newRot < 0.0)
						newRot += 360;
					newRot = constrainAngle(newRot, m_doc->toolSettings.constrain);
					double len = qMax(0.01, distance(bounds.width(), bounds.height()));
					bounds.setSize(len * QSizeF(cosd(newRot), sind(newRot)));
					newX = bounds.right();
					newY = bounds.bottom();
				}
			}
			else
			{
				FPoint np2 = m_doc->ApplyGridF(FPoint(newX, newY));
				double nx = np2.x();
				double ny = np2.y();
				m_doc->ApplyGuides(&nx, &ny);
				newX = qRound(nx);
				newY = qRound(ny);
			}
			canvasCurrCoord.setXY(newX, newY);
			m_view->HaveSelRect = true;

			double wSize = canvasCurrCoord.x() - createObjectPos.x();
			double hSize = canvasCurrCoord.y() - createObjectPos.y();
			QRectF createObjectRect(createObjectPos.x(), createObjectPos.y(), wSize, hSize);
			createObjectRect = createObjectRect.normalized();
			if (createObjectMode != modeDrawLine)
				m_canvas->displaySizeHUD(m->globalPos(), createObjectRect.width(), createObjectRect.height(), false);
			else
			{
				double angle = -xy2Deg(wSize, hSize);
				if (angle < 0.0)
					angle = angle + 360;
				double trueLength = sqrt(pow(createObjectRect.width(), 2) + pow(createObjectRect.height(), 2));
				m_canvas->displaySizeHUD(m->globalPos(), trueLength, angle, true);
			}

			// Necessary for drawControls to be called
			m_canvas->repaint();
		}
		else
			m_canvas->displayCorrectedXYHUD(m->globalPos(), mousePointDoc.x(), mousePointDoc.y());
	}
}

void CreateMode::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	modifiers = m->modifiers();
	
	double Rxp = 0, Ryp = 0;
	double Rxpd = 0,Rypd = 0;
	m_MouseButtonPressed = true;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	inItemCreation = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	canvasPressCoord = mousePointDoc;
	createObjectMode = m_doc->appMode;
	createObjectSubMode = m_doc->SubMode;
	createObjectPos  = m_doc->ApplyGridF(canvasPressCoord);
	
	Rxp  = m_doc->ApplyGridF(canvasPressCoord).x();
	Rxpd = canvasPressCoord.x() - Rxp;
	canvasPressCoord.setX(qRound(Rxp));
	Ryp  = m_doc->ApplyGridF(canvasPressCoord).y();
	Rypd = canvasPressCoord.y() - Ryp;

	canvasPressCoord.setXY(qRound(Rxp), qRound(Ryp));
	canvasCurrCoord = canvasPressCoord;
	createObjectPos.setXY(Rxp, Ryp);
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		m_view->DrawNew();
		return;
	}
	
	if (m->button() == Qt::RightButton)
	{
		m_view->requestMode(modeNormal);
		return;
	}
	if (m->button() != Qt::LeftButton)
		return;
	selectPage(m);

	switch (m_doc->appMode)
	{
	case modeDrawTable:
		m_view->Deselect(false);
		break;
	}

	inItemCreation = true;
}



void CreateMode::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	modifiers = m->modifiers();

	PageItem *currItem;
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();

	m_createTransaction = new UndoTransaction(Um::instance()->beginTransaction("creating"));
	currItem = doCreateNewObject();
	if (m_createTransaction && currItem)
	{
		m_view->resetMousePressed();
		currItem->checkChanges(true);
		QString targetName = Um::ScratchSpace;
		if (currItem->OwnPage > -1)
			targetName = m_doc->Pages->at(currItem->OwnPage)->getUName();
		m_createTransaction->commit(targetName, currItem->getUPixmap(),
									Um::Create + " " + currItem->getUName(),  "", Um::ICreate);
		m_doc->changed();
		delete m_createTransaction;
		m_createTransaction = NULL;	
		/*currItem->update();
		currItem->emitAllToGUI();*/
	}
	else if (m_createTransaction)
	{
		m_createTransaction->cancel();
		delete m_createTransaction;
		m_createTransaction = NULL;
	}
	if (!PrefsManager::instance()->appPrefs.stickyTools)
	{
		m_view->requestMode(modeNormal);
	}
	else
	{
		m_view->updateCanvas();
		int appMode = m_doc->appMode;
		m_view->requestMode(appMode);
	}
	inItemCreation = false;
}



void CreateMode::selectPage(QMouseEvent *m)
{
	m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	canvasPressCoord     = mousePointDoc;
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	m_doc->nodeEdit.deselect();
	m_view->Deselect(false);
	if (!m_doc->masterPageMode())
	{
		int i = m_doc->OnPage(canvasPressCoord.x(), canvasPressCoord.y());
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

void CreateMode::SetupDrawNoResize(int nr)
{
	PageItem* currItem = m_doc->Items->at(nr);
	//	currItem->setFont(Doc->toolSettings.defFont);
	//	currItem->setFontSize(Doc->toolSettings.defSize);
	m_doc->m_Selection->delaySignalsOn();
	m_doc->m_Selection->clear();
	m_doc->m_Selection->addItem(currItem);
	m_doc->m_Selection->delaySignalsOff();
//	emit DocChanged();
	currItem->Sizing =  false /*currItem->asLine() ? false : true*/;
	//#6456 m_view->resetMoveTimer();
}

void CreateMode::getFrameItemTypes(int& itemType, int& frameType)
{
	itemType  = (int) PageItem::Polygon;
	frameType = (int) PageItem::Rectangle;
	switch (createObjectMode)
	{
	case modeDrawShapes:
		switch (createObjectSubMode)
		{
		case 0:
			itemType  = (int) PageItem::Polygon;
			frameType = (int) PageItem::Rectangle;
			break;
		case 1:
			itemType  = (int) PageItem::Polygon;
			frameType = (int) PageItem::Ellipse;
			break;
		default:
			itemType  = (int) PageItem::Polygon;
			frameType = (int) PageItem::Unspecified;
			break;
		}
		break;
	case modeDrawLine:
		itemType  = (int) PageItem::Line;
		frameType = (int) PageItem::Unspecified;
		break;
	case modeDrawLatex:
		itemType  = (int) PageItem::LatexFrame;
		frameType = (int) PageItem::Unspecified;
		break;
	case modeDrawImage:
		itemType  = (int) PageItem::ImageFrame;
		frameType = (int) PageItem::Unspecified;
		break;
	case modeDrawText:
		itemType  = (int) PageItem::TextFrame;
		frameType = (int) PageItem::Unspecified;
		break;
	case modeDrawRegularPolygon:
		itemType  = (int) PageItem::Polygon;
		frameType = (int) PageItem::Unspecified;
		break;
	case modeInsertPDFButton:
	case modeInsertPDFTextfield:
	case modeInsertPDFCheckbox:
	case modeInsertPDFCombobox:
	case modeInsertPDFListbox:
	case modeInsertPDFTextAnnotation:
	case modeInsertPDFLinkAnnotation:
		itemType  = (int) PageItem::TextFrame;
		frameType = (int) PageItem::Unspecified;
		break;
	case modeDrawTable:
		itemType  = (int) PageItem::TextFrame;
		frameType = (int) PageItem::Unspecified;
		break;
	}
}

PageItem* CreateMode::doCreateNewObject(void)
{
	int z = -1;
	double rot, len;

	double wSize = canvasCurrCoord.x() - createObjectPos.x();
	double hSize = canvasCurrCoord.y() - createObjectPos.y();
	bool   skipOneClick = ((modifiers == Qt::ShiftModifier);
	if ((createObjectMode == modeDrawLine) || (createObjectMode == modeDrawTable) ||
		(createObjectMode == modeInsertPDFButton) || (createObjectMode == modeInsertPDFTextfield) ||
		(createObjectMode == modeInsertPDFTextfield) || (createObjectMode == modeInsertPDFCheckbox) ||
		(createObjectMode == modeInsertPDFCombobox) || (createObjectMode == modeInsertPDFListbox) ||
		(createObjectMode == modeInsertPDFTextAnnotation) || (createObjectMode == modeInsertPDFLinkAnnotation))
	{
		skipOneClick = false;
	}
	if (!skipOneClick)
	{
		if ((!m_view->moveTimerElapsed()) || ((fabs(wSize) < 2.0) && (fabs(hSize) < 2.0)))
		{
			if (!doOneClick(createObjectPos, canvasCurrCoord))
			{
				return NULL;
			}
		}
	}

	wSize = canvasCurrCoord.x() - createObjectPos.x();
	hSize = canvasCurrCoord.y() - createObjectPos.y();
	PageItem *newObject = NULL, *currItem = NULL;
	// FIXME for modeDrawLine
	QRectF createObjectRect(createObjectPos.x(), createObjectPos.y(), wSize, hSize);
	if (createObjectMode != modeDrawLine)
		createObjectRect = createObjectRect.normalized();
	double Rxp  = createObjectRect.x();
	double Ryp  = createObjectRect.y();
	double Rxpd = createObjectRect.width();
	double Rypd = createObjectRect.height();

	switch (createObjectMode)
	{
	case modeDrawShapes:
		switch (createObjectSubMode)
		{
		case 0:
			if (modifiers == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
				m_doc->Items->at(z)->FrameType = 0;
			}
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
				m_doc->Items->at(z)->FrameType = 0;
			}
			break;
		case 1:
			if (modifiers == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
				m_doc->Items->at(z)->FrameType = 1;
			}
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
				m_doc->Items->at(z)->FrameType = 1;
			}
			break;
		default:
			if (modifiers == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
				m_doc->Items->at(z)->SetFrameShape(m_doc->ValCount, m_doc->ShapeValues);
				m_doc->setRedrawBounding(m_doc->Items->at(z));
				m_doc->Items->at(z)->FrameType = createObjectSubMode + 2;
			}
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
				m_doc->Items->at(z)->SetFrameShape(m_doc->ValCount, m_doc->ShapeValues);
				m_doc->setRedrawBounding(m_doc->Items->at(z));
				m_doc->Items->at(z)->FrameType = createObjectSubMode + 2;
			}
			break;
		}
		break;
	case modeDrawLine:
		Rxp = createObjectPos.x();
		Ryp = createObjectPos.y();
		m_doc->ApplyGuides(&Rxp, &Ryp);
		rot = xy2Deg(Rxpd, Rypd);
		if (rot < 0.0) 
			rot += 360;
		len = qMax(0.01, distance(Rxpd, Rypd));
		z = m_doc->itemAdd(PageItem::Line, PageItem::Unspecified, Rxp, Ryp, len, 1, m_doc->toolSettings.dWidthLine, CommonStrings::None, m_doc->toolSettings.dPenLine, true);
		m_doc->Items->at(z)->setRotation(rot);
		break;
	case modeDrawLatex:
		if (modifiers == Qt::ShiftModifier)
		{
			z = m_doc->itemAddArea(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, true);
		}
		else
		{
			m_doc->ApplyGuides(&Rxp, &Ryp);
			z = m_doc->itemAdd(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrushPict, CommonStrings::None, true);
		}
		break;
	case modeDrawImage:
		if (modifiers == Qt::ShiftModifier)
		{
			z = m_doc->itemAddArea(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, true);
		}
		else
		{
			m_doc->ApplyGuides(&Rxp, &Ryp);
			z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrushPict, CommonStrings::None, true);
		}
		break;
	case modeDrawText:
		if (modifiers == Qt::ShiftModifier)
		{
			z = m_doc->itemAddArea(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, true);
		}	
		else
		{
			m_doc->ApplyGuides(&Rxp, &Ryp);
			z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, true);
		}
		break;
	case modeDrawRegularPolygon:
		{
			if (modifiers == Qt::ShiftModifier)
				z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, true);
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
		m_doc->ApplyGuides(&Rxp, &Ryp);
		z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, Rxpd, Rypd, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, true);
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
		break;
	case modeDrawTable:
		/*m_view->Deselect(false);
		Rxp = mousePointDoc.x();
		Ryp = mousePointDoc.y();
		npf = m_doc->ApplyGridF(FPoint(Rxp, Ryp));
		Rxp = npf.x();
		Ryp = npf.y();
		m_doc->ApplyGuides(&Rxp, &Ryp);
		canvasPressCoord.setXY(qRound(Rxp), qRound(Ryp));
		canvasCurrCoord = canvasPressCoord;
		mouseGlobalCoord.setXY(m->globalPos().x(), m->globalPos().y());
		m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
		m_view->redrawMarker->show();*/

		if ((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt))
		{
			UndoTransaction * activeTransaction = NULL;
			m_view->HaveSelRect = false;
			double Tx, Ty, Tw, Th;
			FPoint np2 = m_doc->ApplyGridF(canvasPressCoord);
			Tx = np2.x();
			Ty = np2.y();
			m_doc->ApplyGuides(&Tx, &Ty);
			canvasPressCoord.setXY(qRound(Tx), qRound(Ty));
			np2 = m_doc->ApplyGridF(canvasCurrCoord);
			Tw = np2.x();
			Th = np2.y();
			m_doc->ApplyGuides(&Tw, &Th);
			canvasCurrCoord.setXY(qRound(Tw), qRound(Th));
			Tw = Tw - Tx;
			Th = Th - Ty;
			int z;
			int Cols, Rows;
			double deltaX, deltaY, offX, offY;
			if ((Th < 6) || (Tw < 6))
			{
				m_view->requestMode(submodePaintingDone);
				break;
			}
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			InsertTable *dia = new InsertTable(m_view, static_cast<int>(Th / 6), static_cast<int>(Tw / 6));
			if (!dia->exec())
			{
				m_view->requestMode(submodePaintingDone);
				delete dia;
				dia = NULL;
				break;
			}
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
				activeTransaction = new UndoTransaction(m_view->undoManager->beginTransaction(m_doc->currentPage()->getUName(),
																						  Um::ITable, Um::CreateTable,
																						  QString(Um::RowsCols).arg(Rows).arg(Cols),
																						  Um::ICreate));
			m_doc->m_Selection->delaySignalsOn();
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Tx + offX, Ty + offY, deltaX, deltaY, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, true);
					currItem = m_doc->Items->at(z);
					currItem->isTableItem = true;
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
			if (activeTransaction)
			{
				activeTransaction->commit();
				delete activeTransaction;
				activeTransaction = NULL;
			}
			m_doc->m_Selection->delaySignalsOff();
		}
		break;
	}
	if (z >= 0)
	{
		SetupDrawNoResize(z);
		newObject = m_doc->Items->at(z);
		newObject->ContourLine = newObject->PoLine.copy();
	}
	return newObject;
}

bool CreateMode::doOneClick(FPoint& startPoint, FPoint& endPoint)
{
	bool doCreate = false;
	double xSize, ySize;
	int  originPoint = 0;
	
	if (QApplication::keyboardModifiers() & Qt::ControlModifier)
		return true;

	PrefsContext* sizes = PrefsManager::instance()->prefsFile->getContext("ObjectSize");
	bool doRemember     = sizes->getBool("Remember", true);

	int lmode = (createObjectMode == modeDrawLine) ? 1 : 0;
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
			endPoint.setXY(startPoint.x() + xSize, startPoint.y() + ySize);
			switch (originPoint)
			{
				case 0:
					break;
				case 1:
					startPoint.setX(startPoint.x() - xSize);
					endPoint.setX(endPoint.x() - xSize);
					break;
				case 2:
					startPoint.setXY(startPoint.x() - xSize / 2.0, startPoint.y() - ySize / 2.0);
					endPoint.setXY(endPoint.x() - xSize / 2.0, endPoint.y() - ySize / 2.0);
					break;
				case 3:
					startPoint.setY(startPoint.y() - ySize);
					endPoint.setY(endPoint.y() - ySize);
					break;
				case 4:
					startPoint.setXY(startPoint.x() - xSize, startPoint.y() - ySize);
					endPoint.setXY(endPoint.x() - xSize, endPoint.y() - ySize);
					break;
			}
		}
		else
		{
			FPoint oldStart = startPoint;
			xSize = dia->spinWidth->value() / unitGetRatioFromIndex(m_doc->unitIndex());
			ySize = dia->spinHeight->value();
			originPoint = dia->RotationGroup->checkedId();
			if (doRemember)
			{
				sizes->set("defLength", xSize);
				sizes->set("defAngle", ySize);
				sizes->set("OriginL", originPoint);
			}
			double angle = -ySize * M_PI / 180.0;
			switch (originPoint)
			{
				case 0:
					endPoint = FPoint(startPoint.x() + xSize * cos(angle), startPoint.y() + xSize * sin(angle));
					break;
				case 1:
					startPoint = FPoint(oldStart.x() - xSize * cos(angle), oldStart.y() - xSize * sin(angle));
					endPoint   = oldStart;
					break;
				case 2:
					startPoint = FPoint(oldStart.x() - xSize / 2.0 * cos(angle), oldStart.y() - xSize / 2.0 * sin(angle));
					endPoint   = FPoint(oldStart.x() + xSize / 2.0 * cos(angle), oldStart.y() + xSize / 2.0 * sin(angle));
					break;
			}
		}
		sizes->set("Remember", doRemember);
		doCreate = true;
	}
	delete dia;
	return doCreate;
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

