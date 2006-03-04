/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribusview.cpp  -  description
                             -------------------
    begin                : Fre Apr  6 21:47:55 CEST 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "scribusview.h"
#include "scribusview.moc"

#include "scconfig.h"

#include <qcolor.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qstringlist.h>
#include <qdragobject.h>
#include <qimage.h>
#include <qcstring.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <qcursor.h>
#include <qurl.h>
#include <qdir.h>
#include <qevent.h>
#include <qsizegrip.h>
#if QT_VERSION  > 0x030102
	#define SPLITVC SplitHCursor
	#define SPLITHC SplitVCursor
#else
	#define SPLITVC SplitVCursor
	#define SPLITHC SplitHCursor
#endif
#include "scribus.h"
#include "tree.h"
#include "mpalette.h"
#include "scribusXml.h"
#include "selection.h"
#include "serializer.h"
#include "insertTable.h"
#include "hruler.h"
#include "vruler.h"
#include "filewatcher.h"
#include "undomanager.h"
#include "units.h"
#include "extimageprops.h"
#include "page.h"
#include "pageitemattributes.h"
#include "pageitem_imageframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#include "pageselector.h"
#include "scpaths.h"
#include "actionmanager.h"
#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif
#ifdef HAVE_CMS
	#include CMS_INC
#endif

#include "scfontmetrics.h"
#include "scmessagebox.h"
#include "util.h"
#include "story.h"
#include "prefsmanager.h"
#include "rulermover.h"
#include "hyphenator.h"
#include "commonstrings.h"

using namespace std;



ScribusView::ScribusView(QWidget *parent, ScribusDoc *doc) :
	QScrollView(parent, "s", WRepaintNoErase | WNorthWestGravity),
	Doc(doc),
	Prefs(&(PrefsManager::instance()->appPrefs)),
	undoManager(UndoManager::instance()),
	OldScale(0),
	GroupX(0), GroupY(0), GroupW(0), GroupH(0),
	oldW(-1), oldCp(-1),
	Mxp(-1), Myp(-1), Dxp(-1), Dyp(-1),
	frameResizeHandle(-1),
	SeRx(-1), SeRy(-1), GyM(-1), GxM(-1),
	ClRe(-1), ClRe2(-1),
	SegP1(-1), SegP2(-1),
	RotMode(0),
	DrHY(-1), DrVX(-1),
	EdPoints(true),
	m_MouseButtonPressed(false),	
	operItemMoving(false),
	MoveGY(false), MoveGX(false),
	HaveSelRect(false),
	operItemResizing(false),
	EditContour(false),
	//GroupSel(false),
	DraggedGroup(false),
	DraggedGroupFirst(false),
	operItemResizeInEditMode(false),
	MidButt(false),
	updateOn(true),
	FirstPoly(true),
	Magnify(false),
	MoveSym(false),
	previewMode(false),
	RCenter(-1,-1),
	RecordP(),
	Ready(false),
	oldX(0), oldY(0),
	_groupTransactionStarted(false),
	_isGlobalMode(true),
	evSpon(false),
	forceRedraw(false),
	Scale(Prefs->DisScale),
	m_vhRulerHW(17),
	m_cursorVisible(false)
{
	setHScrollBarMode(QScrollView::AlwaysOn);
	setVScrollBarMode(QScrollView::AlwaysOn);
	setMargins(m_vhRulerHW, m_vhRulerHW, 0, 0);
	setResizePolicy(Manual);
	viewport()->setBackgroundMode(PaletteBackground);
	QFont fo = QFont(font());
	int posi = fo.pointSize()-2;
	fo.setPointSize(posi);
	unitSwitcher = new QComboBox( false, this, "unitSwitcher" );
	unitSwitcher->setFocusPolicy(QWidget::NoFocus);
	unitSwitcher->setFont(fo);
	for (int i=0;i<=unitGetMaxIndex();++i)
		unitSwitcher->insertItem(unitGetStrFromIndex(i));
	zoomSpinBox = new MSpinBox( 10, 3200, this, 2 );
	zoomSpinBox->setFont(fo);
	zoomSpinBox->setValue( 100 );
	zoomSpinBox->setFocusPolicy(QWidget::ClickFocus);
	zoomSpinBox->setSuffix( tr( " %" ) );
#if OPTION_USE_QTOOLBUTTON
	zoomOutToolbarButton = new QToolButton(this);
	zoomDefaultToolbarButton = new QToolButton(this);
	zoomInToolbarButton = new QToolButton(this);
	zoomDefaultToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	zoomOutToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	zoomInToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
#else
	zoomDefaultToolbarButton = new QPushButton(this);
	zoomDefaultToolbarButton->setFocusPolicy(QWidget::NoFocus);
	zoomDefaultToolbarButton->setDefault( false );
	zoomDefaultToolbarButton->setAutoDefault( false );
	zoomDefaultToolbarButton->setFlat(OPTION_FLAT_BUTTON);
	zoomOutToolbarButton = new QPushButton(this);
	zoomOutToolbarButton->setFocusPolicy(QWidget::NoFocus);
	zoomOutToolbarButton->setDefault( false );
	zoomOutToolbarButton->setAutoDefault( false );
	zoomOutToolbarButton->setFlat(OPTION_FLAT_BUTTON);
	zoomInToolbarButton = new QPushButton(this);
	zoomInToolbarButton->setFocusPolicy(QWidget::NoFocus);
	zoomInToolbarButton->setDefault( false );
	zoomInToolbarButton->setAutoDefault( false );
	zoomInToolbarButton->setFlat(OPTION_FLAT_BUTTON);
#endif
	zoomDefaultToolbarButton->setText("1:1");
//	zoomDefaultToolbarButton->setPixmap(loadIcon("viewmag1.png"));
	zoomOutToolbarButton->setPixmap(loadIcon("viewmagout.png"));
	zoomInToolbarButton->setPixmap(loadIcon("viewmagin.png"));
	pageSelector = new PageSelector(this, 1);
	pageSelector->setFont(fo);
	pageSelector->setFocusPolicy(QWidget::ClickFocus);
	layerMenu = new QComboBox( true, this, "LY" );
	layerMenu->setEditable(false);
	layerMenu->setFont(fo);
	layerMenu->setFocusPolicy(QWidget::NoFocus);
	horizRuler = new Hruler(this, Doc);
	vertRuler = new Vruler(this, Doc);
	rulerMover = new RulerMover(this);
	rulerMover->setFocusPolicy(QWidget::NoFocus);
	Ready = true;
	viewport()->setMouseTracking(true);
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setDragAutoScroll(false);
	//languageChange();
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Doc->SubMode = -1;
	connect(zoomOutToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
	connect(zoomInToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
	connect(zoomDefaultToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoom100()));
	connect(zoomSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setZoom()));
	connect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	connect(unitSwitcher, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
	connect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
	connect(this, SIGNAL(HaveSel(int)), this, SLOT(selectionChanged()));
}

void ScribusView::languageChange()
{
	disconnect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	disconnect(unitSwitcher, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
	zoomSpinBox->setSuffix( tr( " %" ) );
	layerMenu->setCurrentText( tr("Layer")+" 0");
	//CB TODO Convert to actions later
	unitSwitcher->clear();
	for (int i=0;i<=unitGetMaxIndex();++i)
		unitSwitcher->insertItem(unitGetStrFromIndex(i));
	unitSwitcher->setCurrentText(unitGetStrFromIndex(Doc->unitIndex()));
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	connect(unitSwitcher, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
}

void ScribusView::viewportPaintEvent ( QPaintEvent * p )
{
	#ifndef _WIN32
	if (p->spontaneous())
		evSpon = true;
	#endif
	QScrollView::viewportPaintEvent(p);
}

void ScribusView::drawContents(QPainter *, int clipx, int clipy, int clipw, int cliph)
{
	if (Doc->isLoading())
		return;
	if (!updateOn)
		return;
//	QTime tim;
//	tim.start();
	if ((clipw > 0) && (cliph > 0))
	{
		QPoint vr = contentsToViewport(QPoint(clipx, clipy));
		ScPainter *painter = new ScPainter(viewport(), clipw, cliph, vr.x(), vr.y());
		painter->clear(paletteBackgroundColor());
		painter->translate(-Doc->minCanvasCoordinate.x()*Scale, -Doc->minCanvasCoordinate.y()*Scale);
		painter->translate(-clipx, -clipy);
		painter->setLineWidth(1);
		painter->setFillMode(ScPainter::Solid);
//		painter->translate(0.5, 0.5);
		painter->setZoomFactor(1.0);
/* Draw Page Outlines */
		if (!Doc->masterPageMode())
		{
			uint docPagesCount=Doc->Pages->count();
			for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
			{
				int x = qRound(Doc->Pages->at(a)->xOffset() * Scale);
				int y = qRound(Doc->Pages->at(a)->yOffset() * Scale);
				int w = qRound(Doc->Pages->at(a)->width() * Scale);
				int h = qRound(Doc->Pages->at(a)->height() * Scale);
				QRect drawRect = QRect(x, y, w+5, h+5);
				drawRect.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
				if (drawRect.intersects(QRect(clipx, clipy, clipw, cliph)))
				{
					painter->setFillMode(ScPainter::Solid);
					painter->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
					painter->setBrush(QColor(128,128,128));
					painter->drawRect(x+5, y+5, w, h);
					if (a == Doc->currentPageNumber())
						painter->setPen(red, 2, SolidLine, FlatCap, MiterJoin);
					else
						painter->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
					painter->setBrush(Doc->papColor);
					painter->drawRect(x, y, w, h);
					if (Doc->guidesSettings.before)
						DrawPageMarks(painter, Doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
				}
				DrawMasterItems(painter, Doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
			}
			DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
			if (!Doc->guidesSettings.before)
			{
				for (uint a = 0; a < docPagesCount; ++a)
				{
					int x = static_cast<int>(Doc->Pages->at(a)->xOffset() * Scale);
					int y = static_cast<int>(Doc->Pages->at(a)->yOffset() * Scale);
					int w = static_cast<int>(Doc->Pages->at(a)->width() * Scale);
					int h = static_cast<int>(Doc->Pages->at(a)->height() * Scale);
					QRect drawRect = QRect(x, y, w+5, h+5);
					drawRect.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
					if (drawRect.intersects(QRect(clipx, clipy, clipw, cliph)))
						DrawPageMarks(painter, Doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
				}
			}
		}
		else
		{
			int x = static_cast<int>(Doc->ScratchLeft * Scale);
			int y = static_cast<int>(Doc->ScratchTop * Scale);
			int w = static_cast<int>(Doc->currentPage->width() * Scale);
			int h = static_cast<int>(Doc->currentPage->height() * Scale);
			QRect drawRect = QRect(x, y, w+5, h+5);
			drawRect.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
			if (drawRect.intersects(QRect(clipx, clipy, clipw, cliph)))
			{
				painter->setFillMode(ScPainter::Solid);
				painter->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
				painter->setBrush(QColor(128,128,128));
				painter->drawRect(x+5, y+5, w, h);
				painter->setBrush(Doc->papColor);
				painter->drawRect(x, y, w, h);
				if (Doc->guidesSettings.before)
					DrawPageMarks(painter, Doc->currentPage, QRect(clipx, clipy, clipw, cliph));
			}
			DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
			if ((!Doc->guidesSettings.before) && (drawRect.intersects(QRect(clipx, clipy, clipw, cliph))))
				DrawPageMarks(painter, Doc->currentPage, QRect(clipx, clipy, clipw, cliph));
		}
		if ((Doc->selection->count() != 0) || (linkedFramesToShow.count() != 0))
		{
			double z = painter->zoomFactor();
			painter->setZoomFactor(Scale);
			painter->save();
			PageItem *currItem;
			if ((Doc->guidesSettings.linkShown) && (linkedFramesToShow.count() != 0))
				currItem = linkedFramesToShow.at(0);
			else
			{
				if (linkedFramesToShow.count() != 0)
					currItem = linkedFramesToShow.at(0);
				else
					currItem = Doc->selection->itemAt(0);
			}
			//Draw the frame links
			if ((((Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
				   && (currItem->itemType() == PageItem::TextFrame)) || (Doc->guidesSettings.linkShown))
			{
				PageItem *nextItem = currItem;
				if (Doc->guidesSettings.linkShown)
				{
					for (uint lks = 0; lks < linkedFramesToShow.count(); ++lks)
					{
						nextItem = linkedFramesToShow.at(lks);
						while (nextItem != 0)
						{
							//Calculate the link points of the frames
							double x11 = nextItem->xPos();
							double y11 = nextItem->yPos();
							double x12 = x11+nextItem->width();
							double y12 = y11+nextItem->height();
							double x1mid = x11+(x12-x11)/2;
							double y1mid = y11+(y12-y11)/2;

							if (nextItem->rotation()!=0.000)
							{
								FPoint tempPoint(0,0, x11, y11, nextItem->rotation(), 1, 1);
								x11=tempPoint.x();
								y11=tempPoint.y();
								FPoint tempPoint2(0,0, x12, y12, nextItem->rotation(), 1, 1);
								x12=tempPoint2.x();
								y12=tempPoint2.y();
								FPoint tempPoint3(0,0, x1mid, y1mid, nextItem->rotation(), 1, 1);
								x1mid=tempPoint3.x();
								y1mid=tempPoint3.y();
							}


							double a1, b1, a2, b2;
							a1 = a2 = b1 = b2 = 0;
							if (nextItem->NextBox!=NULL)
							{
								double x21 = nextItem->NextBox->xPos();
								double y21 = nextItem->NextBox->yPos();
								double x22 = x21+nextItem->NextBox->width();
								double y22 = y21+nextItem->NextBox->height();
								double x2mid = x21 + nextItem->NextBox->width()/2;
								double y2mid = y21 + nextItem->NextBox->height()/2;
								//x2mid = x21+(x22-x21)/2;
								//y2mid = y21+(y22-y21)/2;

								if (nextItem->NextBox->rotation()!=0.000)
								{
									FPoint tempPoint(0,0, x21, y21, nextItem->NextBox->rotation(), 1, 1);
									x21=tempPoint.x();
									y21=tempPoint.y();
									FPoint tempPoint2(0,0, x22, y22, nextItem->NextBox->rotation(), 1, 1);
									x22=tempPoint2.x();
									y22=tempPoint2.y();
									FPoint tempPoint3(0,0, x2mid, y2mid, nextItem->NextBox->rotation(), 1, 1);
									x2mid=tempPoint3.x();
									y2mid=tempPoint3.y();
								}

								if (x22<x11) { a1 = x11; a2 = x22; }
								if (x21>x12) { a1 = x12; a2 = x21; }
								if (y22<y11) { b1 = y11; b2 = y22; }
								if (y21>y12) { b1 = y12; b2 = y21; }

								if (x21<x12 && x21>x11) { a1 = x1mid; a2 = x2mid; }
								if (x21<x11 && x22>x11) { a1 = x1mid; a2 = x2mid; }

								if (y21<y12 && y21>y11) { b1 = y1mid; b2 = y2mid; }
								if (y21<y11 && y22>y11) { b1 = y1mid; b2 = y2mid; }

							}
							//Draw the link frame lines
							FPoint Start(a1-nextItem->xPos(), b1-nextItem->yPos(), nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
							//FPoint Start = transformPoint(FPoint(nextItem->Width/2, nextItem->Height), nextItem->xPos(), nextItem->yPos(), nextItem->Rot, 1, 1);
							nextItem = nextItem->NextBox;
							if (nextItem != NULL)
							{
								FPoint End(a2-nextItem->xPos(), b2-nextItem->yPos(), nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
								drawLinkFrameLine(painter, Start, End);
							}
						}
					}
				}
				else
				{
					while (nextItem != 0)
					{
						if (nextItem->BackBox != 0)
							nextItem = nextItem->BackBox;
						else
							break;
					}
					while (nextItem != 0)
					{
						FPoint Start(nextItem->width()/2, nextItem->height(), nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
						nextItem = nextItem->NextBox;
						//Draw the link frame indicator for a new link
						//CB unsure if we need to do this
						if (nextItem != 0)
						{
							FPoint End(nextItem->width()/2, 0, nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
							drawLinkFrameLine(painter, Start, End);
						}
					}
				}
			}
			painter->setLineWidth(1);
			painter->setPenOpacity(1.0);
			painter->restore();
			painter->setZoomFactor(z);
		}
		painter->end();
		delete painter;
		painter=NULL;
	}
	if (Doc->selection->count() != 0)
	{
		PageItem *currItem = Doc->selection->itemAt(0);
		currItem->paintObj();
		if ((Doc->EditClip) && (currItem->isSelected()))
		{
		if (EditContour)
			MarkClip(currItem, currItem->ContourLine, true);
		else
			MarkClip(currItem, currItem->PoLine, true);
		}
		if (Doc->selection->isMultipleSelection())
		{
			setGroupRect();
			paintGroupRect();
		}
	}
	if (Doc->appMode == modeEdit)
		slotDoCurs(true);
	if (Doc->appMode == modeEditGradientVectors)
	{
		PageItem *currItem = Doc->selection->itemAt(0);
		QPainter p;
		p.begin(viewport());
		ToView(&p);
		Transform(currItem, &p);
		p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
		p.setBrush(NoBrush);
		p.drawLine(QPoint(qRound(currItem->GrStartX), qRound(currItem->GrStartY)), QPoint(qRound(currItem->GrEndX), qRound(currItem->GrEndY)));
		p.setPen(QPen(magenta, 8, SolidLine, RoundCap, MiterJoin));
		p.drawLine(QPoint(qRound(currItem->GrStartX), qRound(currItem->GrStartY)), QPoint(qRound(currItem->GrStartX), qRound(currItem->GrStartY)));
		p.drawLine(QPoint(qRound(currItem->GrEndX), qRound(currItem->GrEndY)), QPoint(qRound(currItem->GrEndX), qRound(currItem->GrEndY)));
		p.end();
	}
	evSpon = false;
	forceRedraw = false;
//	qDebug( "Time elapsed: %d ms", tim.elapsed() );
}

void ScribusView::DrawMasterItems(ScPainter *painter, Page *page, QRect clip)
{
	double z = painter->zoomFactor();
	if (!page->MPageNam.isEmpty())
	{
		Page* Mp = Doc->MasterPages.at(Doc->MasterNames[page->MPageNam]);
		if (page->FromMaster.count() != 0)
		{
			//QPainter p;
			int Lnr;
			struct Layer ll;
			PageItem *currItem;
			ll.isViewable = false;
			ll.LNr = 0;
			Lnr = 0;
			uint layerCount=Doc->layerCount();
			for (uint la = 0; la < layerCount; ++la)
			{
				Level2Layer(Doc, &ll, Lnr);
				bool pr = true;
				if ((previewMode) && (!ll.isPrintable))
					pr = false;
				if ((ll.isViewable) && (pr))
				{
					uint pageFromMasterCount=page->FromMaster.count();
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						if ((previewMode) && (!currItem->printEnabled()))
							continue;
						currItem->savedOwnPage = currItem->OwnPage;
//						int savedOwnPage = currItem->OwnPage;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						currItem->OwnPage = page->pageNr();
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->moveBy(-Mp->xOffset() + page->xOffset(), -Mp->yOffset() + page->yOffset(), true);
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						if (evSpon)
							currItem->Dirty = true;
						QRect oldR(currItem->getRedrawBounding(Scale));
						if (clip.intersects(oldR))
							currItem->DrawObj(painter, clip);
						currItem->OwnPage = currItem->savedOwnPage;
//						currItem->OwnPage = savedOwnPage;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX, OldY, true);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
					}
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if (!currItem->isTableItem)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX - Mp->xOffset() + page->xOffset(), OldY - Mp->yOffset() + page->yOffset(), true);
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						QRect oldR(currItem->getRedrawBounding(Scale));
						if (clip.intersects(oldR))
						{
							painter->setZoomFactor(Scale);
							painter->save();
							painter->translate(currItem->xPos()*Scale, currItem->yPos()*Scale);
							painter->rotate(currItem->rotation());
							if (currItem->lineColor() != CommonStrings::None)
							{
								QColor tmp;
								currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
								if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
								{
									painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
									if (currItem->TopLine)
										painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
									if (currItem->RightLine)
										painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
									if (currItem->BottomLine)
										painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
									if (currItem->LeftLine)
										painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
								}
							}
							painter->restore();
						}
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX, OldY, true);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
					}
				}
				Lnr++;
			}
		}
	}
	painter->setZoomFactor(z);
}

void ScribusView::DrawPageItems(ScPainter *painter, QRect clip)
{
	linkedFramesToShow.clear();
	double z = painter->zoomFactor();
	if (Doc->Items->count() != 0)
	{
		int Lnr=0;
		struct Layer ll;
		PageItem *currItem;
		ll.isViewable = false;
		ll.LNr = 0;
		uint layerCount=Doc->layerCount();
		int docCurrPageNo=static_cast<int>(Doc->currentPageNumber());
		for (uint la2 = 0; la2 < layerCount; ++la2)
		{
			Level2Layer(Doc, &ll, Lnr);
			bool pr = true;
			if ((previewMode) && (!ll.isPrintable))
				pr = false;
			if ((ll.isViewable) && (pr))
			{
				QPtrListIterator<PageItem> docItem(*Doc->Items);
				while ( (currItem = docItem.current()) != 0)
				{
					++docItem;
					if (currItem->LayerNr != ll.LNr)
						continue;
					if ((previewMode) && (!currItem->printEnabled()))
						continue;
					if ((Doc->masterPageMode()) && ((currItem->OwnPage != -1) && (currItem->OwnPage != docCurrPageNo)))
						continue;
					if (!Doc->masterPageMode() && !currItem->OnMasterPage.isEmpty())
					{
						if (currItem->OnMasterPage != Doc->currentPage->PageNam)
							continue;
					}
					QRect oldR(currItem->getRedrawBounding(Scale));
					if (clip.intersects(oldR))
					{
						if (evSpon)
							currItem->Dirty = true;
						if (forceRedraw)
							currItem->Dirty = false;
//						if ((!m_MouseButtonPressed) || (Doc->EditClip))
							currItem->DrawObj(painter, clip);
						currItem->Redrawn = true;
						if ((currItem->asTextFrame()) && ((currItem->NextBox != 0) || (currItem->BackBox != 0)))
						{
							PageItem *nextItem = currItem;
							while (nextItem != 0)
							{
								if (nextItem->BackBox != 0)
									nextItem = nextItem->BackBox;
								else
									break;
							}
							if (linkedFramesToShow.find(nextItem) == -1)
								linkedFramesToShow.append(nextItem);
						}
						if ((Doc->appMode == modeEdit) && (currItem->isSelected()) && (currItem->itemType() == PageItem::TextFrame))
						{
							//CB 230305 Stop redrawing the horizontal ruler if it hasnt changed when typing text!!!
							if ((qRound(horizRuler->ItemPos*10000) != qRound((currItem->xPos())*10000)) || (qRound(horizRuler->ItemEndPos*10000) != qRound(((currItem->xPos()+currItem->width()) )*10000)))
							{
								horizRuler->setItemPosition(currItem->xPos(), currItem->width());
								if (currItem->lineColor() != CommonStrings::None)
									horizRuler->lineCorr = currItem->lineWidth() / 2.0;
								else
									horizRuler->lineCorr = 0;
								horizRuler->ColGap = currItem->ColGap;
								horizRuler->Cols = currItem->Cols;
								horizRuler->Extra = currItem->textToFrameDistLeft();
								horizRuler->RExtra = currItem->textToFrameDistRight();
								horizRuler->First = Doc->docParagraphStyles[Doc->currentParaStyle].First;
								horizRuler->Indent = Doc->docParagraphStyles[Doc->currentParaStyle].Indent;
								if (currItem->imageFlippedH() || (currItem->reversed()))
									horizRuler->Revers = true;
								else
									horizRuler->Revers = false;
								horizRuler->ItemPosValid = true;
								if (Doc->currentParaStyle < 5)
									horizRuler->TabValues = currItem->TabValues;
								else
									horizRuler->TabValues = Doc->docParagraphStyles[Doc->currentParaStyle].TabValues;
								horizRuler->repaint();
							}
						}
					}
				}
				QPtrListIterator<PageItem> docItem2(*Doc->Items);
				while ( (currItem = docItem2.current()) != 0 )
				{
					++docItem2;
					if (currItem->LayerNr != ll.LNr)
						continue;
					if (!currItem->isTableItem)
						continue;
					QRect oldR(currItem->getRedrawBounding(Scale));
					if (clip.intersects(oldR))
					{
						painter->setZoomFactor(Scale);
						painter->save();
						painter->translate(currItem->xPos()*Scale, currItem->yPos()*Scale);
						painter->rotate(currItem->rotation());
						if (currItem->lineColor() != CommonStrings::None)
						{
							QColor tmp;
							currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
							if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
							{
								painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
								if (currItem->TopLine)
									painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
								if (currItem->RightLine)
									painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
								if (currItem->BottomLine)
									painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
								if (currItem->LeftLine)
									painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
							}
						}
						painter->restore();
					}
				}
			}
			Lnr++;
		}
	}
	painter->setZoomFactor(z);
}

void ScribusView::DrawPageMarks(ScPainter *p, Page *page, QRect)
{
	double z = p->zoomFactor();
	p->save();
	p->setZoomFactor(Scale);
	p->translate(page->xOffset() * Scale, page->yOffset() * Scale);
	double lineWidth = 0.5 / Scale;
	p->setLineWidth(lineWidth);
	double pageHeight=page->height();
	double pageWidth=page->width();
	//Draw the margins
	if (Doc->guidesSettings.marginsShown)
	{
		p->setPen(Doc->guidesSettings.margColor);
		if (Doc->marginColored)
		{
			p->setBrush(Doc->guidesSettings.margColor);
			p->drawRect(0, 0, pageWidth, page->Margins.Top);
			p->drawRect(0, page->Margins.Top, page->Margins.Left, pageHeight - page->Margins.Top);
			p->drawRect(page->Margins.Left, pageHeight - page->Margins.Bottom, pageWidth - page->Margins.Right - page->Margins.Left, page->Margins.Bottom);
			p->drawRect(pageWidth - page->Margins.Right, page->Margins.Top, page->Margins.Right, pageHeight-page->Margins.Top);
		}
		p->setPen(Doc->guidesSettings.margColor);
		p->drawLine(FPoint(0, page->Margins.Top), FPoint(pageWidth, page->Margins.Top));
		p->drawLine(FPoint(0, pageHeight - page->Margins.Bottom), FPoint(pageWidth, pageHeight - page->Margins.Bottom));
		p->drawLine(FPoint(page->Margins.Left, 0), FPoint(page->Margins.Left, pageHeight));
		p->drawLine(FPoint(pageWidth - page->Margins.Right, 0), FPoint(pageWidth - page->Margins.Right, pageHeight));
	}
	//Draw the baseline grid
	if (Doc->guidesSettings.baseShown)
	{
		p->setPen(Doc->guidesSettings.baseColor, lineWidth, SolidLine, FlatCap, MiterJoin);
		for (double yg = Doc->typographicSettings.offsetBaseGrid; yg < pageHeight; yg += Doc->typographicSettings.valueBaseGrid)
			p->drawLine(FPoint(0, yg), FPoint(pageWidth, yg));
	}
	//Draw the grid lines
	if (Doc->guidesSettings.gridShown)
	{
		double stx = 0;
		double endx = pageWidth;
		double sty = 0;
		double endy = pageHeight;
/*		double stx = QMAX((clip.x() - page->Xoffset) / Scale, 0);
		double endx = QMIN(stx + clip.width() / Scale, page->width());
		double sty = QMAX((clip.y() - page->Yoffset) / Scale, 0);
		double endy = QMIN(sty + clip.height() / Scale, page->height()); */
		if (Scale > 0.49)
		{
			double i,start;
			i = Doc->guidesSettings.majorGrid;
			p->setPen(Doc->guidesSettings.majorColor, lineWidth, SolidLine, FlatCap, MiterJoin);
			start=floor(sty/i);
			start*=i;
			for (double b = start; b < endy; b+=i)
				p->drawLine(FPoint(0, b), FPoint(pageWidth, b));
			start=floor(stx/i);
			start*=i;
			for (double b = start; b <= endx; b+=i)
				p->drawLine(FPoint(b, 0), FPoint(b, pageHeight));
			i = Doc->guidesSettings.minorGrid;
			p->setPen(Doc->guidesSettings.minorColor, lineWidth, DotLine, FlatCap, MiterJoin);
			start=floor(sty/i);
			start*=i;
			for (double b = start; b < endy; b+=i)
				p->drawLine(FPoint(0, b), FPoint(pageWidth, b));
			start=floor(stx/i);
			start*=i;
			for (double b = start; b <= endx; b+=i)
				p->drawLine(FPoint(b, 0), FPoint(b, pageHeight));
		}
	}
	//Draw the guides
	if (Doc->guidesSettings.guidesShown)
	{
		p->setPen(Doc->guidesSettings.guideColor, lineWidth, DotLine, FlatCap, MiterJoin);
		if (page->XGuides.count() != 0)
		{
			for (uint xg = 0; xg < page->XGuides.count(); ++xg)
			{
				if ((page->XGuides[xg] >= 0) && (page->XGuides[xg] <= page->width()))
					p->drawLine(FPoint(page->XGuides[xg], 0), FPoint(page->XGuides[xg], pageHeight));
			}
		}
		if (page->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < page->YGuides.count(); ++yg)
			{
				if ((page->YGuides[yg] >= 0) && (page->YGuides[yg] <= pageHeight))
					p->drawLine(FPoint(0, page->YGuides[yg]), FPoint(page->width(), page->YGuides[yg]));
			}
		}
	}
	p->restore();
	p->setZoomFactor(z);
}

void ScribusView::leaveEvent(QEvent *)
{
/*	if (BlockLeave)
		return; */
	if (!m_MouseButtonPressed)
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
/*	else
	{
		if ((SelItem.count() != 0) && (m_MouseButtonPressed) && (!doku->DragP) && (doku->appMode == 1))
		{
			PageItem *currItem = SelItem.at(0);
			if ((b->Locked) || (b->Sizing))
				return;
			doku->DragP = true;
			doku->leaveDrag = true;
			doku->DraggedElem = b;
			doku->DragElements.clear();
			for (uint dre=0; dre<SelItem.count(); ++dre)
				doku->DragElements.append(SelItem.at(dre)->ItemNr);
			ScriXmlDoc *ss = new ScriXmlDoc();
			QDragObject *dr = new QTextDrag(ss->WriteElem(&SelItem, doku), this);
			dr->setPixmap(loadIcon("DragPix.xpm"));
			dr->drag();
			delete ss;
			ss=NULL;
			doku->DragP = false;
			doku->leaveDrag = false;
			m_MouseButtonPressed = false;
			doku->DraggedElem = 0;
			doku->DragElements.clear();
		}
	} */
}

void ScribusView::contentsDragEnterEvent(QDragEnterEvent *e)
{
	QString text;
	e->accept(QTextDrag::canDecode(e));
	if (QTextDrag::decode(e, text))
	{
		double gx, gy, gw, gh;
		setActiveWindow();
		raise();
		ScMW->newActWin(Doc->WinHan);
		updateContents();
//		SeleItemPos(e->pos());
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (fi.exists())
			text = ur.path();
		if(ss->ReadElemHeader(text,fi.exists(), &gx, &gy, &gw, &gh))
		{
			GroupX = e->pos().x() / Scale;
			GroupY = e->pos().y() / Scale;
			GroupW = gw;
			GroupH = gh;
			DraggedGroup = true;
			DraggedGroupFirst = true;
			//GroupSel = false;
			QPainter p;
			p.begin(viewport());
			PaintSizeRect(&p, QRect());
			emit ItemGeom(gw, gh);
//			QPoint pv = QPoint(qRound(gx), qRound(gy));
//			PaintSizeRect(&p, QRect(pv, QPoint(pv.x()+qRound(gw), pv.y()+qRound(gh))));
			p.end();
		}
		delete ss;
		ss=NULL;
	}
}

void ScribusView::contentsDragMoveEvent(QDragMoveEvent *e)
{
	QString text;
//	PageItem *currItem;
//	bool img;
	e->accept(QTextDrag::canDecode(e));
	if (QTextDrag::decode(e, text))
	{
		if (DraggedGroup)
		{
			double gx, gy, gw, gh;
			GroupX = e->pos().x() / Scale;
			GroupY = e->pos().y() / Scale;
			getGroupRectScreen(&gx, &gy, &gw, &gh);
			QPainter p;
			p.begin(viewport());
			gx += Doc->minCanvasCoordinate.x();
			gy += Doc->minCanvasCoordinate.y();
			QPoint pv = QPoint(qRound(gx), qRound(gy));
			if (!DraggedGroupFirst)
				PaintSizeRect(&p, QRect(pv, QPoint(pv.x()+qRound(gw), pv.y()+qRound(gh))));
			DraggedGroupFirst = false;
			p.end();
			emit MousePos(GroupX+Doc->minCanvasCoordinate.x(), GroupY+Doc->minCanvasCoordinate.y());
			horizRuler->Draw(e->pos().x());
			vertRuler->Draw(e->pos().y());
			return;
		}
/*		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).upper();
		QStrList imfo = QImageIO::inputFormats();
		if (ext == "JPG")
			ext = "JPEG";
		img = ((imfo.contains(ext))||(ext=="PS")||(ext=="EPS")||(ext=="TIF"));
		if (!SeleItemPos(e->pos()))
		{
			if (SelItem.count() != 0)
				Deselect(true);
		}
		else
		{
			b = SelItem.at(0);
			if (img)
			{
				if (b->PType != 2)
					Deselect(true);
			}
			else
			{
				if (b->PType != 4)
					Deselect(true);
			}
		} */
	}
}

void ScribusView::contentsDragLeaveEvent(QDragLeaveEvent *)
{
	if (DraggedGroup)
	{
		updateContents();
		DraggedGroup = false;
		DraggedGroupFirst = false;
	}
}

void ScribusView::contentsDropEvent(QDropEvent *e)
{
	QString text;
	PageItem *currItem;
	bool img = false;
//	struct ScText *hg;
//	uint a;
	int re = 0;
	e->accept(QTextDrag::canDecode(e));
	DraggedGroupFirst = false;
	int ex = qRound(e->pos().x()/Scale + Doc->minCanvasCoordinate.x());
	int ey = qRound(e->pos().y()/Scale + Doc->minCanvasCoordinate.y());
	if (QTextDrag::decode(e, text))
	{
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).upper();
		QStrList imfo = QImageIO::inputFormats();
		if (ext == "JPG")
			ext = "JPEG";
		//CB Need to handle this ugly file extension list elsewhere... some capabilities class perhaps
		img = ((imfo.contains(ext))||(ext=="PS")||(ext=="EPS")||(ext=="PDF")||(ext=="TIF")||(ext=="PSD"));
		bool selectedItemByDrag=false;
		int pscx=qRound(e->pos().x()/Scale), pscy=qRound(e->pos().y()/Scale);
		//Loop through all items and see which one(s) were under the drop point on the current layer
		//Should make a nice function for this.
		for (uint i=0; i<Doc->Items->count(); ++i)
		{
			if (Doc->Items->at(i)->LayerNr==Doc->activeLayer())
			{
				if (Doc->Items->at(i)->pointWithinItem(pscx, pscy))
				{
					Deselect(false);
					SelectItem(Doc->Items->at(i));
					selectedItemByDrag=true;
					break;
				}
			}
		}
		//CB When we drag an image to a page from outside
		//SeleItemPos is from 1.2.x. Needs reenabling for dragging *TO* a frame
		if ((fi.exists()) && (img) && !selectedItemByDrag)// && (!SeleItemPos(e->pos())))
		{
			//int z = PaintPict(qRound(e->pos().x()/doku->Scale), qRound(e->pos().y()/doku->Scale), 1, 1);
			int z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, pscx, pscy, 1, 1, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, true);
			PageItem *b = Doc->Items->at(z);
			Doc->LoadPict(ur.path(), b->ItemNr);
			b->setWidth(static_cast<double>(b->pixm.width()));
			b->setHeight(static_cast<double>(b->pixm.height()));
			b->OldB2 = b->width();
			b->OldH2 = b->height();
			b->updateClip();
			emit DocChanged();
			update();
			return;
		}
		//if ((SeleItemPos(e->pos())) && (!text.startsWith("<SCRIBUSELEM")))
		if (Doc->selection->count()>0 && Doc->selection->itemAt(0)->pointWithinItem(pscx, pscy) && (!text.startsWith("<SCRIBUSELEM")))
		{
			PageItem *b = Doc->selection->itemAt(0);
			if (b->itemType() == PageItem::ImageFrame)
			{
				if ((fi.exists()) && (img))
				{
					Doc->LoadPict(ur.path(), b->ItemNr);
					update();
				}
			}
			/* CB leaving this out for now...
			if (b->PType == 4)
			{
				if ((b->BackBox != 0) && (b->itemText.count() == 0))
					return;
				if ((fi.exists()) && (!img) && (fi.size() < 500000))
				{
					Serializer *ss = new Serializer(ur.path());
					if (ss->Read())
					{
						int st = doku->currentParaStyle;
						ss->GetText(b, st, doku->docParagraphStyles[st].Font, doku->docParagraphStyles[st].FontSize, true);
						emit DocChanged();
					}
					delete ss;
					ss=NULL;
					update();
				}
				else
				{
					slotDoCurs(false);
					slotSetCurs(e->pos().x(), e->pos().y());
					if (text.startsWith("<SCRIBUSELEM"))
						return;
					for (a=0; a<text.length(); ++a)
					{
						hg = new ScText;
						hg->ch = text.at(a);
						if (hg->ch == QChar(10))
							hg->ch = QChar(13);
						if (hg->ch == QChar(4))
							hg->ch = QChar(9);
						if (hg->ch == QChar(5))
							hg->ch = QChar(13);
						hg->cfont = b->IFont;
						hg->csize = b->ISize;
						hg->ccolor = b->TxtFill;
						hg->cshade = b->ShTxtFill;
						hg->cstroke = b->TxtStroke;
						hg->cshade2 = b->ShTxtStroke;
						hg->cselect = false;
						hg->cscale = b->TxtScale;
						hg->cextra = 0;
						hg->cstyle = 0;
						hg->cab = 0;
						hg->xp = 0;
						hg->yp = 0;
						hg->PRot = 0;
						hg->PtransX = 0;
						hg->PtransY = 0;
						b->itemText.insert(b->CPos, hg);
						b->CPos += 1;
					}
					emit DocChanged();
					update();
				}
			}*/
		}
		else
		{
			for (uint as = 0; as < Doc->Items->count(); ++as)
			{
				Doc->Items->at(as)->setSelected(false);
			}
			uint oldDocItemCount = Doc->Items->count();
			if ((!img) && (Doc->DraggedElem == 0))
			{
				if ((fi.exists()) && (!img))
					emit LoadElem(ur.path(), ex, ey, true, false, Doc, this);
				else
					emit LoadElem(QString(text), ex, ey, false, false, Doc, this);
				Doc->selection->clear();
				for (uint as = oldDocItemCount; as < Doc->Items->count(); ++as)
				{
					currItem = Doc->Items->at(as);
					Doc->selection->addItem(currItem);
					if (currItem->isBookmark)
						emit AddBM(currItem);
				}
			}
			else
			{
				if (Doc->DraggedElem != 0)
				{
					if (!Doc->leaveDrag)
					{
						QPopupMenu *pmen = new QPopupMenu();
						qApp->setOverrideCursor(QCursor(ArrowCursor), true);
						pmen->insertItem( tr("Copy Here"));
						pmen->insertItem( tr("Move Here"));
						pmen->insertItem( tr("Cancel"));
						re = pmen->indexOf(pmen->exec(QCursor::pos()));
						delete pmen;
						pmen=NULL;
					}
					else
						re = 1;
					if ((re == 2) || (re == -1))
					{
						updateContents();
						return;
					}
					if ((re == 1) || (Doc->leaveDrag))
					{
						QPtrList<PageItem> pasted;
						emit LoadElem(QString(text), ex, ey, false, false, Doc, this);
						for (uint as = oldDocItemCount; as < Doc->Items->count(); ++as)
						{
							pasted.append(Doc->Items->at(as));
						}
						Doc->selection->clear();
						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							Doc->selection->addItem(Doc->Items->at(Doc->DragElements[dre]));
						}
						PageItem* bb;
						int fin;
						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							bb = pasted.at(dre);
							currItem = Doc->selection->itemAt(dre);
							if ((currItem->asTextFrame()) && ((currItem->NextBox != 0) || (currItem->BackBox != 0)))
							{
								if (currItem->BackBox != 0)
								{
									bb->BackBox = currItem->BackBox;
									fin = Doc->selection->findItem(currItem->BackBox);
									if (fin != -1)
										bb->BackBox = pasted.at(fin);
									bb->BackBox->NextBox = bb;
								}
								if (currItem->NextBox != 0)
								{
									bb->NextBox = currItem->NextBox;
									fin = Doc->selection->findItem(currItem->NextBox);
									if (fin != -1)
										bb->NextBox = pasted.at(fin);
									bb->NextBox->BackBox = bb;
								}
							}
						}
						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							currItem = Doc->selection->itemAt(dre);
							currItem->NextBox = 0;
							currItem->BackBox = 0;
						}
						pasted.clear();
						Doc->itemSelection_DeleteItem();
					}
				}
				if ((!img) && ((re == 0)))
					emit LoadElem(QString(text), ex, ey, false, false, Doc, this);
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
				Doc->selection->clear();
				for (uint as = oldDocItemCount; as < Doc->Items->count(); ++as)
				{
					currItem = Doc->Items->at(as);
					Doc->selection->addItem(currItem);
					if (currItem->isBookmark)
						emit AddBM(currItem);
				}
			}
			if (Doc->selection->count() > 1)
			{
				setGroupRect();
				paintGroupRect();
				double x, y, w, h;
				getGroupRect(&x, &y, &w, &h);
				emit ItemPos(x, y);
				emit ItemGeom(w, h);
			}
			updateContents();
		}
	}
}

void ScribusView::contentsMouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_MouseButtonPressed = false;
	PageItem *currItem = 0;
	if (Doc->EditClip)
	{
		emit EndNodeEdit();
		return;
	}
	if ((Doc->selection->isMultipleSelection()) || (Doc->appMode != modeNormal))
	{
		if ((Doc->selection->isMultipleSelection()) && (Doc->appMode == modeNormal))
		{
			if (GetItem(&currItem))
			{
				if (currItem->isTableItem)
				{
					Deselect(false);
					Doc->selection->addItem(currItem);
					currItem->isSingleSel = true;
					//CB FIXME dont call this if the added item is item 0
					if (!Doc->selection->primarySelectionIs(currItem))
						currItem->emitAllToGUI();
					currItem->paintObj();
				}
			}
			return;
		}
		else
		{
			if (!(GetItem(&currItem) && (Doc->appMode == modeEdit) && currItem->asTextFrame()))
			{
				contentsMousePressEvent(m);
				return;
			}
		}
	}
	if (GetItem(&currItem))
	{
		if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
		{
			if ((currItem->locked()) || (!currItem->ScaleType))
			{
				contentsMousePressEvent(m);
				return;
			}
			//If we double click on an image frame and theres no image assigned, open the
			//load picture dialog, else put it into edit mode if the frame is set to show the image
			if (currItem->itemType() == PageItem::ImageFrame)
			{
				if (currItem->Pfile.isEmpty())
					emit LoadPic();
				else if (currItem->imageShown())
					emit Amode(modeEdit);
 			}
		}
		else
			if (currItem->itemType() == PageItem::TextFrame)
			{
				//CB old code
				//emit currItem->isAnnotation() ? AnnotProps() : Amode(modeEdit);
				//contentsMousePressEvent(m);
				//CB if annotation, open the annotation dialog
				if (currItem->isAnnotation())
				{
					emit AnnotProps();
					contentsMousePressEvent(m);
				}
				//else if not in mode edit, set mode edit
				else if (Doc->appMode != modeEdit)
				{
					Amode(modeEdit);
					//CB ignore the double click and go with a single one
					//if we werent in mode edit before.
					//unsure if this is correct, but its ok given we had no
					//double click select until now.
					contentsMousePressEvent(m);
				}
				//otherwise, select between the whitespace
				else
				{	//Double click in a frame to select a word
					PageItem_TextFrame *cItem=currItem->asTextFrame();
					bool inText = slotSetCurs(m->x(), m->y());
					if (!inText)
					{
						Deselect(true);
						slotDoCurs(true);
						emit Amode(modeNormal);
						return;
					}
					int a=cItem->CPos;
					while(a>0)
					{
						if (cItem->itemText.at(a-1)->ch.at(0).isLetterOrNumber())
							--a;
						else
							break;
					}
					uint b=cItem->CPos;
					while(b<cItem->itemText.count())
					{
						if (cItem->itemText.at(b)->ch.at(0).isLetterOrNumber())
							++b;
						else
							break;
					}
					oldCp=a;
					cItem->CPos=b;
					cItem->ExpandSel(1, oldCp);
					slotDoCurs(true);
				}
			}
	}
}

void ScribusView::contentsMouseReleaseEvent(QMouseEvent *m)
{
	PageItem *currItem;
	m_MouseButtonPressed = false;
	if (Doc->guidesSettings.guidesShown)
	{
		bool foundGuide = false;
		double nx = m->x()/Scale + Doc->minCanvasCoordinate.x();
		double ny = m->y()/Scale + Doc->minCanvasCoordinate.y();
		if (Doc->currentPage->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < Doc->currentPage->YGuides.count(); ++yg)
			{
				if ((Doc->currentPage->YGuides[yg]+Doc->currentPage->yOffset()< (ny+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->YGuides[yg]+Doc->currentPage->yOffset()> (ny-Doc->guidesSettings.grabRad)))
				{
					foundGuide = true;
					break;
				}
			}
		}
		if (Doc->currentPage->XGuides.count() != 0)
		{
			for (uint xg = 0; xg < Doc->currentPage->XGuides.count(); ++xg)
			{
				if ((Doc->currentPage->XGuides[xg]+Doc->currentPage->xOffset()< (nx+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->XGuides[xg]+Doc->currentPage->xOffset()> (nx-Doc->guidesSettings.grabRad)))
				{
					foundGuide = true;
					break;
				}
			}
		}
		if ((foundGuide) && (m->button() == RightButton) && (!GetItem(&currItem)))
		{
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			MoveGY = false;
			MoveGX = false;
			emit EditGuides();
			return;
		}
		if (MoveGY)
		{
			SetYGuide(m, GyM);
			MoveGY = false;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			updateContents();
			GyM = -1;
			return;
		}
		if (MoveGX)
		{
			SetXGuide(m, GxM);
			MoveGX = false;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			updateContents();
			GxM = -1;
			return;
		}
	}
	if (Doc->appMode == modeEditGradientVectors)
		return;
	if (Doc->appMode == modeCopyProperties)
		return;
	if (Doc->appMode == modeMeasurementTool)
	{
		QPainter p;
		p.begin(viewport());
		QPoint out = contentsToViewport(QPoint(0, 0));
		p.translate(out.x(), out.y());
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.drawLine(Dxp, Dyp, Mxp, Myp);
		p.end();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		//emit PaintingDone();
		return;
	}
	if (Doc->appMode == modePanning)
	{
		if ((m->state() & Qt::RightButton) && (m->state() & Qt::ControlButton))
		{
			ScMW->setAppMode(modeNormal);
		}
		return;
	}
	if (Doc->appMode == modeDrawTable)
	{
		if ((Doc->selection->count() == 0) && (HaveSelRect) && (!MidButt))
		{
			QRect AreaR = QRect(static_cast<int>(Mxp), static_cast<int>(Myp), static_cast<int>(SeRx-Mxp), static_cast<int>(SeRy-Myp));
			QPainter p;
			p.begin(viewport());
			ToView(&p);
			p.scale(Scale, Scale);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if(Mxp > SeRx)
			{
				double tmp=SeRx;
				SeRx=static_cast<int>(Mxp);
				Mxp=static_cast<int>(tmp);
			}
			if(Myp > SeRy)
			{
				double tmp=SeRy;
				SeRy=static_cast<int>(Myp);
				Myp=static_cast<int>(tmp);
			}
			HaveSelRect = false;
			double Tx, Ty, Tw, Th;
			Tx = Mxp;
			Ty = Myp;
			Tw = SeRx-Mxp;
			Th = SeRy-Myp;
			int z;
			int Cols, Rows;
			double deltaX, deltaY, offX, offY;
			if ((Th < 6) || (Tw < 6))
			{
				p.drawRect(AreaR);
				p.end();
				Doc->appMode = modeNormal;
				emit PaintingDone();
				return;
			}
			InsertTable *dia = new InsertTable(this, static_cast<int>(Th / 6), static_cast<int>(Tw / 6));
			if (!dia->exec())
			{
				p.drawRect(AreaR);
				p.end();
				Doc->appMode = modeNormal;
				emit PaintingDone();
				delete dia;
				dia=NULL;
				return;
			}
			p.end();
			Cols = dia->Cols->value();
			Rows = dia->Rows->value();
			delete dia;
			dia=NULL;
			deltaX = Tw / Cols;
			deltaY = Th / Rows;
			offX = 0.0;
			offY = 0.0;
			Doc->selection->clear();
			if (UndoManager::undoEnabled())
				undoManager->beginTransaction(Doc->currentPage->getUName(), Um::ITable, Um::CreateTable,
											  QString(Um::RowsCols).arg(Rows).arg(Cols), Um::ICreate);
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					//z = PaintText(Tx + offX, Ty + offY, deltaX, deltaY, Doc->toolSettings.dWidth, Doc->toolSettings.dPenText);
					z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Tx + offX, Ty + offY, deltaX, deltaY, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_MouseButtonPressed);
					currItem = Doc->Items->at(z);
					currItem->isTableItem = true;
					currItem->setTextFlowsAroundFrame(true);
					currItem->setTextFlowUsesBoundingBox(true);
					Doc->selection->addItem(currItem);
					offX += deltaX;
				}
				offY += deltaY;
				offX = 0.0;
			}
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					currItem = Doc->selection->itemAt((rc * Cols) + cc);
					if (rc == 0)
						currItem->TopLink = 0;
					else
						currItem->TopLink = Doc->selection->itemAt(((rc-1)*Cols)+cc);
					if (rc == Rows-1)
						currItem->BottomLink = 0;
					else
						currItem->BottomLink = Doc->selection->itemAt(((rc+1)*Cols)+cc);
					if (cc == 0)
						currItem->LeftLink = 0;
					else
						currItem->LeftLink = Doc->selection->itemAt((rc*Cols)+cc-1);
					if (cc == Cols-1)
						currItem->RightLink = 0;
					else
						currItem->RightLink = Doc->selection->itemAt((rc*Cols)+cc+1);
				}
			}
			emit DoGroup();
			if (UndoManager::undoEnabled())
				undoManager->commit();
		}
		Doc->appMode = modeNormal;
		emit PaintingDone();
		emit DocChanged();
		updateContents();
		return;
	}
	if (Doc->appMode == modeDrawFreehandLine)
	{
		if (RecordP.size() > 1)
		{
			uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, 0, 0, 1, 1, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenLine, !m_MouseButtonPressed);
			currItem = Doc->Items->at(z);
			currItem->PoLine.resize(0);
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
			AdjustItemSize(currItem);
			Doc->selection->clear();
			Doc->selection->addItem(currItem);
			currItem->ClipEdited = true;
			//currItem->Select = true;
			currItem->FrameType = 3;
			currItem->OwnPage = Doc->OnPage(currItem);
			//qDebug("emit ItemPos(currItem->xPos(), currItem->yPos());");
			/*CB Done with addItem
			emit SetSizeValue(currItem->Pwidth);
			emit SetLineArt(currItem->PLineArt, currItem->PLineEnd, currItem->PLineJoin);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
			emit ItemGradient(currItem->GrType);
			emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
			*/
			//emit HaveSel(PageItem::PolyLine);
		}
		Doc->appMode = modeNormal;
		updateContents();
		emit PaintingDone();
		emit DocChanged();
		return;
	}
	if ((Doc->EditClip) && (ClRe == -1) && (HaveSelRect))
	{
		double sc = Scale;
		QPainter p;
		p.begin(viewport());
		ToView(&p);
		p.scale(Scale, Scale);
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.drawRect(Mxp, Myp, SeRx-Mxp, SeRy-Myp);
		p.end();
		if(Mxp > SeRx)
		{
			double tmp=SeRx;
			SeRx=static_cast<int>(Mxp);
			Mxp=static_cast<int>(tmp);
		}
		if(Myp > SeRy)
		{
			double tmp=SeRy;
			SeRy=static_cast<int>(Myp);
			Myp=static_cast<int>(tmp);
		}
		currItem = Doc->selection->itemAt(0);
		SelNode.clear();
		QRect Sele = QRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>((SeRx-Mxp)*sc), static_cast<int>((SeRy-Myp)*sc));
		FPointArray Clip;
		if (EditContour)
			Clip = currItem->ContourLine;
		else
			Clip = currItem->PoLine;
		for (uint a = 0; a < Clip.size(); ++a)
		{
			if (Clip.point(a).x() > 900000)
				continue;
			p.begin(viewport());
			Transform(currItem, &p);
			QPoint npf = p.xForm(Clip.pointQ(a));
			p.end();
			if ((Sele.contains(npf)) && ((a == 0) || (((a-2) % 4) == 0)))
			{
				ClRe = a;
				SelNode.append(a);
				emit ClipPo(Clip.point(a).x(), Clip.point(a).y());
			}
		}
		HaveSelRect = false;
		if (EditContour)
			MarkClip(currItem, currItem->ContourLine, true);
		else
			MarkClip(currItem, currItem->PoLine, true);
		return;
	}
	if ((moveTimer.elapsed() > 500) && (Doc->EditClip) && (SegP1 == -1) && (SegP2 == -1))
	{
		currItem = Doc->selection->itemAt(0);
		if (operItemMoving)
		{
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			double nx = m->x()/Scale + Doc->minCanvasCoordinate.x();
			double ny = m->y()/Scale + Doc->minCanvasCoordinate.y();
			if (!ApplyGuides(&nx, &ny))
			{
				FPoint npg(ApplyGridF(FPoint(nx, ny)));
				nx = npg.x();
				ny = npg.y();
			}
			FPoint np(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
			MoveClipPoint(currItem, np);
		}
		AdjustItemSize(currItem);
		emit DocChanged();
		updateContents();
		operItemMoving = false;
		return;
	}
	if ((moveTimer.elapsed() > 500) && (Doc->EditClip) && (SegP1 != -1) && (SegP2 != -1))
	{
		SegP1 = -1;
		SegP2 = -1;
		currItem = Doc->selection->itemAt(0);
		operItemMoving = false;
		AdjustItemSize(currItem);
		emit DocChanged();
		updateContents();
		return;
	}
	if ((!GetItem(&currItem)) && (m->button() == RightButton) && (!Doc->DragP) && (Doc->appMode == modeNormal))
	{
		QPopupMenu *pmen = new QPopupMenu();
		if (ScMW->Buffer2.startsWith("<SCRIBUSELEM"))
		{
			Mxp = m->x();
			Myp = m->y();
			pmen->insertItem( tr("&Paste") , this, SLOT(PasteToPage()));
			pmen->insertSeparator();
		}
		setObjectUndoMode();
		ScMW->scrActions["editUndoAction"]->addTo(pmen);
		ScMW->scrActions["editRedoAction"]->addTo(pmen);
		pmen->insertSeparator();
		ScMW->scrActions["viewShowMargins"]->addTo(pmen);
		ScMW->scrActions["viewShowFrames"]->addTo(pmen);
		ScMW->scrActions["viewShowImages"]->addTo(pmen);
		ScMW->scrActions["viewShowGrid"]->addTo(pmen);
		ScMW->scrActions["viewShowGuides"]->addTo(pmen);
		ScMW->scrActions["viewShowBaseline"]->addTo(pmen);
		ScMW->scrActions["viewShowTextChain"]->addTo(pmen);
		ScMW->scrActions["viewRulerMode"]->addTo(pmen);
		pmen->insertSeparator();
//		pmen->insertItem( tr("Unit"), unitSwitcher);
//		pmen->insertSeparator();
		ScMW->scrActions["viewSnapToGrid"]->addTo(pmen);
		ScMW->scrActions["viewSnapToGuides"]->addTo(pmen);
		pmen->insertSeparator();
		ScMW->scrActions["pageApplyMasterPage"]->addTo(pmen);
		ScMW->scrActions["pageManageGuides"]->addTo(pmen);
		ScMW->scrActions["pageManageMargins"]->addTo(pmen);
		pmen->exec(QCursor::pos());
		setGlobalUndoMode();
		delete pmen;
		pmen=NULL;
		return;
	}
	if ((Doc->appMode != modeMagnifier) && (!Doc->EditClip) && (Doc->appMode != modeDrawBezierLine))
	{
		if ((GetItem(&currItem)) && (m->button() == RightButton) && (!Doc->DragP))
		{
			QPopupMenu *pmen = new QPopupMenu();
			QPopupMenu *pmen2 = new QPopupMenu();
			pmen3 = new QPopupMenu();
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			QPopupMenu *pmen4 = new QPopupMenu();
			QPopupMenu *pmenLevel = new QPopupMenu();
			QPopupMenu *pmenPDF = new QPopupMenu();
			pmenResolution = new QPopupMenu();
			setObjectUndoMode();
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
			{
				QButtonGroup *InfoGroup = new QButtonGroup( this, "InfoGroup" );
				InfoGroup->setFrameShape( QButtonGroup::NoFrame );
				InfoGroup->setFrameShadow( QButtonGroup::Plain );
				InfoGroup->setTitle("");
				InfoGroup->setExclusive( true );
				InfoGroup->setColumnLayout(0, Qt::Vertical );
				InfoGroup->layout()->setSpacing( 0 );
				InfoGroup->layout()->setMargin( 0 );
				QGridLayout *InfoGroupLayout = new QGridLayout( InfoGroup->layout() );
				InfoGroupLayout->setAlignment( Qt::AlignTop );
				InfoGroupLayout->setSpacing( 2 );
				InfoGroupLayout->setMargin( 0 );
				QString txtC, txtC2;
				QLabel *InfoT = new QLabel(InfoGroup, "ct");
				QLabel *ParCT = new QLabel(InfoGroup, "pt");
				QLabel *ParC = new QLabel(InfoGroup, "pc");
				QLabel *WordCT = new QLabel(InfoGroup, "wt");
				QLabel *WordC = new QLabel(InfoGroup, "wc");
				QLabel *CharCT = new QLabel(InfoGroup, "ct");
				QLabel *CharC = new QLabel(InfoGroup, "cc");
				QLabel *ColCT = new QLabel(InfoGroup, "ct");
				QLabel *ColC = new QLabel(InfoGroup, "cc");
				QLabel *PrintCT = new QLabel(InfoGroup, "nt"); // <a.l.e>
				QLabel *PrintC = new QLabel(InfoGroup, "nc"); // </a.l.e>
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					QFileInfo fi = QFileInfo(currItem->Pfile);
					InfoT->setText( tr("Picture"));
					InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignCenter );
					ParCT->setText( tr("File: "));
					InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
					ParC->setText(fi.fileName());
					InfoGroupLayout->addWidget( ParC, 1, 1 );
					WordCT->setText( tr("Original PPI: "));
					InfoGroupLayout->addWidget( WordCT, 2, 0, Qt::AlignRight );
					WordC->setText(txtC.setNum(qRound(currItem->pixm.imgInfo.xres))+" x "+txtC2.setNum(qRound(currItem->pixm.imgInfo.yres)));
					InfoGroupLayout->addWidget( WordC, 2, 1 );
					CharCT->setText( tr("Actual PPI: "));
					InfoGroupLayout->addWidget( CharCT, 3, 0, Qt::AlignRight );
					CharC->setText(txtC.setNum(qRound(72.0 / currItem->imageXScale()))+" x "+
					               txtC2.setNum(qRound(72.0 / currItem->imageYScale())));
					InfoGroupLayout->addWidget( CharC, 3, 1 );
					ColCT->setText( tr("Colorspace: "));
					InfoGroupLayout->addWidget( ColCT, 4, 0, Qt::AlignRight );
					QString cSpace;
					QString ext = fi.extension(false).lower();
					if ((ext == "pdf") || (ext == "eps") || (ext == "ps"))
						cSpace = tr("Unknown");
					else
					{
						switch (currItem->pixm.imgInfo.colorspace)
						{
							case 0:
								cSpace = tr("RGB");
								break;
							case 1:
								cSpace = tr("CMYK");
								break;
							case 2:
								cSpace = tr("Grayscale");
								break;
						}
					}
					ColC->setText(cSpace);
					InfoGroupLayout->addWidget( ColC, 4, 1 );
				}
				if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				{
					int Parag = 0;
					int Words = 0;
					int Chara = 0;
					int ParagN = 0;
					int WordsN = 0;
					int CharaN = 0;
					if (currItem->itemType() == PageItem::TextFrame)
					{
						if ((currItem->NextBox != 0) || (currItem->BackBox != 0))
							InfoT->setText( tr("Linked Text"));
						else
							InfoT->setText( tr("Text Frame"));
					}
					else
						InfoT->setText( tr("Text on a Path"));
					InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignCenter );
					WordAndPara(currItem, &Words, &Parag, &Chara, &WordsN, &ParagN, &CharaN);
					ParCT->setText( tr("Paragraphs: "));
					InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
					if (ParagN != 0)
						ParC->setText(txtC.setNum(Parag+ParagN)+" ("+txtC2.setNum(ParagN)+")");
					else
						ParC->setText(txtC.setNum(Parag));
					InfoGroupLayout->addWidget( ParC, 1, 1 );
					WordCT->setText( tr("Words: "));
					InfoGroupLayout->addWidget( WordCT, 2, 0, Qt::AlignRight );
					if (WordsN != 0)
						WordC->setText(txtC.setNum(Words+WordsN)+" ("+txtC2.setNum(WordsN)+")");
					else
						WordC->setText(txtC.setNum(Words));
					InfoGroupLayout->addWidget( WordC, 2, 1 );
					CharCT->setText( tr("Chars: "));
					InfoGroupLayout->addWidget( CharCT, 3, 0, Qt::AlignRight );
					if (CharaN != 0)
						CharC->setText(txtC.setNum(Chara+CharaN)+" ("+txtC2.setNum(CharaN)+")");
					else
						CharC->setText(txtC.setNum(Chara));
					InfoGroupLayout->addWidget( CharC, 3, 1 );
				}

				int row = InfoGroupLayout->numRows(); // <a.l.e>

				PrintCT->setText( tr("Print: "));
				InfoGroupLayout->addWidget( PrintCT, row, 0, Qt::AlignRight );
				if (currItem->printEnabled())
					PrintC->setText( tr("Enabled"));
				else
					PrintC->setText( tr("Disabled"));
				InfoGroupLayout->addWidget( PrintC, row, 1 ); // </a.l.e>

				pmen4->insertItem(InfoGroup);
				if ((currItem->itemType() == PageItem::ImageFrame) && (currItem->pixm.imgInfo.exifDataValid))
					ScMW->scrActions["itemImageInfo"]->addTo(pmen4);
				pmen->insertItem( tr("In&fo"), pmen4);
			}
			pmen->insertSeparator();
			ScMW->scrActions["editUndoAction"]->addTo(pmen);
			ScMW->scrActions["editRedoAction"]->addTo(pmen);
			if (currItem->itemType() == PageItem::ImageFrame ||
				currItem->itemType() == PageItem::TextFrame ||
				currItem->itemType() == PageItem::PathText)
			{
				pmen->insertSeparator();
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					ScMW->scrActions["fileImportImage"]->addTo(pmen);
					ScMW->scrActions["itemImageIsVisible"]->addTo(pmen);
					pmen->insertItem( tr("Preview Settings"), pmenResolution);
					ScMW->scrActions["itemPreviewLow"]->addTo(pmenResolution);
					ScMW->scrActions["itemPreviewNormal"]->addTo(pmenResolution);
					ScMW->scrActions["itemPreviewFull"]->addTo(pmenResolution);
					if ((currItem->PicAvail) && (currItem->pixm.imgInfo.valid))
						ScMW->scrActions["itemExtendedImageProperties"]->addTo(pmen);
					if (currItem->PicAvail && currItem->isRaster)
					{
						ScMW->scrActions["styleImageEffects"]->addTo(pmen);
						ScMW->scrActions["editEditWithImageEditor"]->addTo(pmen);
					}
					if (currItem->PicAvail)
						ScMW->scrActions["itemUpdateImage"]->addTo(pmen);
					if ((currItem->PicAvail) && (!currItem->isTableItem))
						ScMW->scrActions["itemAdjustFrameToImage"]->addTo(pmen);
				}
				if (currItem->itemType() == PageItem::TextFrame)
				{
					ScMW->scrActions["fileImportText"]->addTo(pmen);
					ScMW->scrActions["fileImportAppendText"]->addTo(pmen);
					ScMW->scrActions["toolsEditWithStoryEditor"]->addTo(pmen);
					ScMW->scrActions["insertSampleText"]->addTo(pmen);
				}
				if (currItem->itemType() == PageItem::PathText)
					ScMW->scrActions["toolsEditWithStoryEditor"]->addTo(pmen);
			}
			pmen->insertSeparator();
			ScMW->scrActions["itemAttributes"]->addTo(pmen);
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if (Doc->currentPage->PageNam.isEmpty())
				{
					ScMW->scrActions["itemPDFIsAnnotation"]->addTo(pmenPDF);
					ScMW->scrActions["itemPDFIsBookmark"]->addTo(pmenPDF);
					if (currItem->isAnnotation())
					{
						if ((currItem->annotation().Type() == 0) || (currItem->annotation().Type() == 1) || (currItem->annotation().Type() > 9))
							ScMW->scrActions["itemPDFAnnotationProps"]->addTo(pmenPDF);
						else
							ScMW->scrActions["itemPDFFieldProps"]->addTo(pmenPDF);
					}
				}
				pmen->insertItem( tr("&PDF Options"), pmenPDF);
			}
			pmen->insertSeparator();
			ScMW->scrActions["itemLock"]->addTo(pmen);
			ScMW->scrActions["itemLockSize"]->addTo(pmen);
			if (!currItem->isSingleSel)
			{
				ScMW->scrActions["itemSendToScrapbook"]->addTo(pmen);
				if (Doc->layerCount() > 1)
				{
					QMap<int,int> layerMap;
					for (QValueList<Layer>::iterator it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it)
						layerMap.insert((*it).Level, (*it).LNr);
					int i=layerMap.count()-1;
					while (i>=0)
						ScMW->scrLayersActions[QString::number(layerMap[i--])]->addTo(pmen3);

					pmen->insertItem( tr("Send to La&yer"), pmen3);
				}
			}
			if (!currItem->locked())
			{
				if (Doc->selection->count() > 1)
				{
					bool isGroup = true;
					int firstElem = -1;
					if (currItem->Groups.count() != 0)
						firstElem = currItem->Groups.top();
					for (uint bx = 0; bx < Doc->selection->count(); ++bx)
					{
						if (Doc->selection->itemAt(bx)->Groups.count() != 0)
						{
							if (Doc->selection->itemAt(bx)->Groups.top() != firstElem)
								isGroup = false;
						}
						else
							isGroup = false;
					}
					if (!isGroup)
						ScMW->scrActions["itemGroup"]->addTo(pmen);
				}
				if (currItem->Groups.count() != 0)
					ScMW->scrActions["itemUngroup"]->addTo(pmen);
				if ((!currItem->isTableItem) && (!currItem->isSingleSel))
				{
					pmen->insertItem( tr("Le&vel"), pmenLevel);
					ScMW->scrActions["itemRaiseToTop"]->addTo(pmenLevel);
					ScMW->scrActions["itemRaise"]->addTo(pmenLevel);
					ScMW->scrActions["itemLower"]->addTo(pmenLevel);
					ScMW->scrActions["itemLowerToBottom"]->addTo(pmenLevel);
				}
			}
			if (Doc->appMode != modeEdit) //Create convertTo Menu
			{
				bool insertConvertToMenu=false;
				if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				{
					insertConvertToMenu=true;
					if (currItem->itemType() == PageItem::PathText)
						ScMW->scrActions["itemConvertToOutlines"]->addTo(pmen2);
					else
					{
						if (currItem->isTableItem)
							ScMW->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
						if ((!currItem->isTableItem) && (currItem->BackBox == 0) && (currItem->NextBox == 0))
						{
							ScMW->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
							ScMW->scrActions["itemConvertToOutlines"]->addTo(pmen2);
							ScMW->scrActions["itemConvertToPolygon"]->addTo(pmen2);
						}
					}
				}
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					insertConvertToMenu=true;
					ScMW->scrActions["itemConvertToTextFrame"]->addTo(pmen2);
					if (!currItem->isTableItem)
						ScMW->scrActions["itemConvertToPolygon"]->addTo(pmen2);
				}
				if (currItem->itemType() == PageItem::Polygon)
				{
					insertConvertToMenu=true;
					ScMW->scrActions["itemConvertToBezierCurve"]->addTo(pmen2);
					ScMW->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
					ScMW->scrActions["itemConvertToTextFrame"]->addTo(pmen2);
				}
				if (insertConvertToMenu)
					pmen->insertItem( tr("Conve&rt to"), pmen2);
			}
			pmen->insertSeparator();
			if (!currItem->locked() && !(currItem->isTableItem && currItem->isSingleSel))
				ScMW->scrActions["editCut"]->addTo(pmen);
			if (!(currItem->isTableItem && currItem->isSingleSel))
				ScMW->scrActions["editCopy"]->addTo(pmen);
			if ((Doc->appMode == modeEdit) && (ScMW->Buffer2.startsWith("<SCRIBUSTEXT")) && (currItem->itemType() == PageItem::TextFrame))
				ScMW->scrActions["editPaste"]->addTo(pmen);
			if (!currItem->locked() && (Doc->appMode != modeEdit) && (!(currItem->isTableItem && currItem->isSingleSel)))
				pmen->insertItem( tr("&Delete"), Doc, SLOT(itemSelection_DeleteItem()));
			if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
				ScMW->scrActions["editClearContents"]->addTo(pmen);
			pmen->insertSeparator();
			ScMW->scrActions["toolsProperties"]->addTo(pmen);

			pmen->exec(QCursor::pos());
			setGlobalUndoMode();
			delete pmen;
			delete pmen2;
			delete pmen3;
			delete pmen4;
			delete pmenLevel;
			delete pmenPDF;
			delete pmenResolution;
			pmen=NULL;
			pmen2=NULL;
			pmen3=NULL;
			pmen4=NULL;
			pmenLevel=NULL;
			pmenPDF=NULL;
			pmenResolution=NULL;
		}
		if ((Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
		{
			updateContents();
			if (Doc->ElemToLink != 0)
				return;
			else
			{
				Doc->appMode = modeNormal;
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				emit PaintingDone();
				return;
			}
		}
		if (Doc->appMode == modeDrawRegularPolygon)
		{
			currItem = Doc->selection->itemAt(0);
			FPoint np1(m->x() / Scale + Doc->minCanvasCoordinate.x(), m->y() / Scale + Doc->minCanvasCoordinate.y());
			np1 = ApplyGridF(np1);
			currItem->setWidthHeight(np1.x() - currItem->xPos(), np1.y()- currItem->yPos());
			FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
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
				SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem->ItemNr, false, false, false);
			}
			FPoint tp(getMaxClipF(&currItem->PoLine));
			SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
			AdjustItemSize(currItem);
			currItem->ContourLine = currItem->PoLine.copy();
			Doc->setRedrawBounding(currItem);
			currItem->OwnPage = Doc->OnPage(currItem);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			updateContents();
		}
		if (Doc->appMode == modeDrawLine)
		{
			currItem = Doc->selection->itemAt(0);
			QPainter p;
			p.begin(viewport());
			Transform(currItem, &p);
			QPoint np = p.xFormDev(m->pos());
			p.end();
			np += QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
			np = ApplyGrid(np);
			double newRot=xy2Deg(np.x(), np.y());
			//Constrain rotation angle, when the mouse is released from drawing a line
			if (m->state() & ControlButton)
				newRot=constrainAngle(newRot);
			currItem->setRotation(newRot);
			currItem->setWidthHeight(sqrt(pow(np.x(),2.0)+pow(np.y(),2.0)), 1.0);
			currItem->Sizing = false;
			currItem->updateClip();
			Doc->setRedrawBounding(currItem);
			currItem->OwnPage = Doc->OnPage(currItem);
			updateContents();
		}
		if ((moveTimer.elapsed() > 500) && (GetItem(&currItem)))
		{
			if (Doc->selection->isMultipleSelection())
			{
				if (operItemResizing)
				{
					double gx, gy, gh, gw, nx, ny, scx, scy;
					getGroupRect(&gx, &gy, &gw, &gh);
					double sc = Scale;
					scx = sc;
					scy = sc;
					QPoint np2;
					double newXF = m->x()/sc + Doc->minCanvasCoordinate.x();
					double newYF = m->y()/sc + Doc->minCanvasCoordinate.y();
					if (m->state() & ControlButton)
						np2 = QPoint(qRound(newXF), qRound(gy+(gh * ((newXF-gx) / gw))));
					else
						np2 = QPoint(qRound(newXF), qRound(newYF));
					nx = np2.x();
					ny = np2.y();
					if (!ApplyGuides(&nx, &ny))
					{
						np2 = ApplyGrid(np2);
						nx = np2.x();
						ny = np2.y();
					}
					switch (frameResizeHandle)
					{
						case 1:
							scy = fabs(ny-gy) / gh;
							scx = fabs(nx-gx) / gw;
							break;
						case 2:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
						case 3:
							scx = fabs(nx-gx) / gw;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
						case 4:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = fabs(ny-gy) / gh;
							break;
						case 5:
							scx = 1.0;
							scy = fabs(ny-gy) / gh;
							break;
						case 6:
							scx = fabs(nx-gx) / gw;
							scy = 1.0;
							break;
						case 7:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = 1.0;
							break;
						case 8:
							scx = 1.0;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
					}
					RotMode = Doc->RotMode;
					Doc->RotMode = 0;
					//CB #3012 only scale text in a group if alt is pressed
					if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & AltButton))
						scaleGroup(scx, scy, true);
					else
						scaleGroup(scx, scy, false);
					if ((frameResizeHandle == 3) || (frameResizeHandle == 8))
						moveGroup(0, ny-gy);
					if (frameResizeHandle == 2)
						moveGroup(nx-gx, ny-gy);
					if ((frameResizeHandle == 7) || (frameResizeHandle == 4))
						moveGroup(nx-gx, 0);
					Doc->RotMode = RotMode;
					evSpon = false;
					updateContents();
					emit DocChanged();
				}
			}
			else
			{
				//Where a single frame resize stops when the mouse buttno is released
				//FIXME: reduce code!
				if (currItem->Sizing)
				{
					FPoint npx;
					double nx = m->pos().x()/Scale + Doc->minCanvasCoordinate.x();
					double ny = m->pos().y()/Scale + Doc->minCanvasCoordinate.y();
					if (Doc->SnapGuides)
					{
						ApplyGuides(&nx, &ny);
						npx = FPoint(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
					}
					else
						npx = ApplyGridF(FPoint(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true));
					if ((frameResizeHandle == 1) && !(currItem->asLine()) && (Doc->SnapGuides))
						SizeItem(npx.x(), npx.y(), currItem->ItemNr);
					bool sav = Doc->SnapGuides;
					Doc->SnapGuides = false;
					switch (frameResizeHandle)
					{
					case 1:
						if (!currItem->asLine())
						{
							if (currItem->isTableItem)
							{
								double dist;
								if (currItem->LeftLink != 0)
									dist = npx.y() - currItem->LeftLink->height();
								else if (currItem->RightLink != 0)
									dist = npx.y() - currItem->RightLink->height();
								else
									dist = npx.y() - currItem->height();
								PageItem* bb2;
								PageItem* bb = currItem;
								while (bb->LeftLink != 0)
								{
									bb = bb->LeftLink;
								}
								while (bb->RightLink != 0)
								{
									bb2 = bb;
									while (bb2->BottomLink != 0)
									{
										MoveRotated(bb2->BottomLink, FPoint(0, dist));
										bb2 = bb2->BottomLink;
									}
									if (bb != currItem)
										MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
									bb = bb->RightLink;
								}
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								if (bb != currItem)
									MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = currItem;
								if (currItem->TopLink != 0)
									dist = npx.x() - currItem->TopLink->width();
								else if (currItem->BottomLink != 0)
									dist = npx.x() - currItem->BottomLink->width();
								else
									dist = npx.x() - currItem->width();
								while (bb->TopLink != 0)
								{
									bb = bb->TopLink;
								}
								while (bb->BottomLink != 0)
								{
									bb2 = bb;
									while (bb2->RightLink != 0)
									{
										MoveRotated(bb2->RightLink, FPoint(dist, 0));
										bb2 = bb2->RightLink;
									}
									if (bb != currItem)
										MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
									bb = bb->BottomLink;
								}
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								if (bb != currItem)
									MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
							}
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
								double nx = m->pos().x()/Scale + Doc->minCanvasCoordinate.x();
								double ny = m->pos().y()/Scale + Doc->minCanvasCoordinate.y();
								if (Doc->useRaster)
								{
									FPoint ra(ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								Doc->SnapGuides = sav;
								ApplyGuides(&nx, &ny);
								Doc->SnapGuides = false;
								double r = atan2(ny - currItem->yPos(), nx - currItem->xPos())*(180.0/M_PI);
								RotateItem(r, currItem->ItemNr);
								double w = sqrt(pow(nx - currItem->xPos(), 2) + pow(ny - currItem->yPos(),2));
								SizeItem(w, currItem->height(), currItem->ItemNr, true);
							}
						}
						currItem->Sizing = false;
						break;
					case 2:
						if (!currItem->asLine())
						{
							if (currItem->isTableItem)
							{
								PageItem* bb2;
								PageItem* bb = currItem;
								while (bb->TopLink != 0)
								{
									bb = bb->TopLink;
								}
								while (bb->BottomLink != 0)
								{
									bb2 = bb;
									while (bb2->LeftLink != 0)
									{
										MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
										bb2 = bb2->LeftLink;
									}
									MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
									bb = bb->BottomLink;
								}
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = currItem;
								while (bb->LeftLink != 0)
								{
									bb = bb->LeftLink;
								}
								while (bb->RightLink != 0)
								{
									bb2 = bb;
									while (bb2->TopLink != 0)
									{
										MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
										bb2 = bb2->TopLink;
									}
									MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
									bb = bb->RightLink;
								}
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
							}
							else
								MoveSizeItem(npx, npx, currItem->ItemNr);
								
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
								updateContents(currItem->getRedrawBounding(Scale));
							}
							if (!currItem->imageFlippedV())
							{
								currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
								updateContents(currItem->getRedrawBounding(Scale));
							}
						}
						else
						{
							if (sav)
							{
								double nx = m->pos().x()/Scale + Doc->minCanvasCoordinate.x();
								double ny = m->pos().y()/Scale + Doc->minCanvasCoordinate.y();
								if (Doc->useRaster)
								{
									FPoint ra(ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								Doc->SnapGuides = sav;
								ApplyGuides(&nx, &ny);
								Doc->SnapGuides = false;
								QWMatrix ma;
								ma.translate(currItem->xPos(), currItem->yPos());
								ma.rotate(currItem->rotation());
								double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
								double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
								double r = atan2(my-ny,mx-nx)*(180.0/M_PI);
								double w = sqrt(pow(mx-nx,2)+pow(my-ny,2));
								MoveItem(nx - currItem->xPos(), ny - currItem->yPos(), currItem, true);
								SizeItem(w, currItem->height(), currItem->ItemNr, true);
								RotateItem(r, currItem->ItemNr);
								currItem->Sizing = false;
							}
						}
						break;
					case 3:
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->width();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->RightLink != 0)
							{
								MoveRotated(bb2->RightLink, FPoint(dist, 0));
								bb2 = bb2->RightLink;
							}
							MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
							bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->TopLink != 0)
							{
								MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
								bb2 = bb2->TopLink;
							}
							MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
						}
						else
							MoveSizeItem(FPoint(0, npx.y()), FPoint(currItem->width() - npx.x(), npx.y()), currItem->ItemNr);
							
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
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->height();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->BottomLink != 0)
							{
								MoveRotated(bb2->BottomLink, FPoint(0, dist));
								bb2 = bb2->BottomLink;
							}
							MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
							bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->LeftLink != 0)
							{
								MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
								bb2 = bb2->LeftLink;
							}
							MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
						}
						else
							MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), currItem->height() - npx.y()), currItem->ItemNr);
						
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
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->height();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->BottomLink != 0)
							{
								MoveRotated(bb2->BottomLink, FPoint(0, dist));
								bb2 = bb2->BottomLink;
							}
							MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
						}
						else
							MoveSizeItem(FPoint(0, 0), FPoint(0, currItem->height() - npx.y()), currItem->ItemNr);
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
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->width();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->RightLink != 0)
							{
								MoveRotated(bb2->RightLink, FPoint(dist, 0));
								bb2 = bb2->RightLink;
							}
							MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
						}
						else
							MoveSizeItem(FPoint(0, 0), FPoint(currItem->width() - npx.x(), 0), currItem->ItemNr);
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
						if (currItem->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->LeftLink != 0)
							{
								MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
								bb2 = bb2->LeftLink;
							}
							MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
						}
						else
							MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), currItem->ItemNr);
						
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
						if (currItem->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->TopLink != 0)
							{
								MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
								bb2 = bb2->TopLink;
							}
							MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
						}
						else
							MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), currItem->ItemNr);
							
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
					
							
					//TextFrame resize - Resize text with resize of frame
					//alt resize, free resize with text scaling
					//shift alt, square resize with text scaling
					//control alt, proportional resize with text scaling
					//if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & ShiftButton) && (m->state() & ControlButton))
					//NOTE: this does not cover groups, strangely enough. Thats done in scaleGroup()
					if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & AltButton))
					{
						double scx = currItem->width() / currItem->OldB2;
						double scy = currItem->height() / currItem->OldH2;
						scx = scx != scy ? scx / scy : 1.0;
						if (currItem->itemText.count() != 0)
						{
							currItem->setFontSize(QMAX(qRound(currItem->fontSize() * scy), 1));
							currItem->setLineSpacing(((currItem->fontSize() / 10.0)* static_cast<double>(Doc->typographicSettings.autoLineSpacing) / 100) + (currItem->fontSize() / 10.0));
							currItem->TxtScale = QMIN(QMAX(qRound(currItem->TxtScale * scx), 25), 400);
							Doc->CurrTextScale = currItem->TxtScale;
							Doc->CurrFontSize = currItem->fontSize();
							Doc->CurrTextBase = currItem->TxtBase;
							Doc->CurrTextShadowX = currItem->TxtShadowX;
							Doc->CurrTextShadowY = currItem->TxtShadowY;
							Doc->CurrTextOutline = currItem->TxtOutline;
							Doc->CurrTextUnderPos = currItem->TxtUnderPos;
							Doc->CurrTextUnderWidth = currItem->TxtUnderWidth;
							Doc->CurrTextStrikePos = currItem->TxtStrikePos;
							Doc->CurrTextStrikeWidth = currItem->TxtStrikeWidth;
							emit ItemTextStrike(currItem->TxtStrikePos, currItem->TxtStrikeWidth);
							emit ItemTextUnderline(currItem->TxtUnderPos, currItem->TxtUnderWidth);
							emit ItemTextOutline(currItem->TxtOutline);
							emit ItemTextShadow(currItem->TxtShadowX, currItem->TxtShadowY);
							//emit ItemTextAttr(currItem->LineSp);
							emit ItemTextCols(currItem->Cols, currItem->ColGap);
							emit ItemTextSize(currItem->fontSize());
							emit ItemTextSca(currItem->TxtScale);
							emit ItemTextScaV(currItem->TxtScaleV);
							emit ItemTextBase(currItem->TxtBase);
							for (uint aa = 0; aa < currItem->itemText.count(); ++aa)
							{
								currItem->itemText.at(aa)->csize = QMAX(qRound(currItem->itemText.at(aa)->csize*scy), 1);
								currItem->itemText.at(aa)->cscale = QMAX(QMIN(qRound(currItem->itemText.at(aa)->cscale*scx), 4000), 100);
							}
						}
					}
					if (currItem->itemType() == PageItem::ImageFrame)
					{
						currItem->AdjustPictScale();
						/*Dont need this now adjustpictscale will emit for 1st selected item
						emit SetLocalValues(currItem->LocalScX, currItem->LocalScY, currItem->LocalX, currItem->LocalY );
						*/
					}
					currItem->updateClip();
					emit ItemTextCols(currItem->Cols, currItem->ColGap);
					Doc->SnapGuides = sav;
					evSpon = false;
					updateContents();
					emit DocChanged();
				}
				Doc->setRedrawBounding(currItem);
				currItem->OwnPage = Doc->OnPage(currItem);
			}
			if (operItemMoving)
			{
				evSpon = false;
				if (Doc->selection->isMultipleSelection())
				{
					setGroupRect();
					double gx, gy, gh, gw;
					getGroupRect(&gx, &gy, &gw, &gh);
					double nx = gx;
					double ny = gy;
					if (!ApplyGuides(&nx, &ny))
					{
						FPoint npx;
						npx = ApplyGridF(FPoint(nx, ny));
						nx = npx.x();
						ny = npx.y();
					}
					moveGroup(nx-gx, ny-gy, false);
					setGroupRect();
					getGroupRect(&gx, &gy, &gw, &gh);
					nx = gx+gw;
					ny = gy+gh;
					ApplyGuides(&nx, &ny);
					moveGroup(nx-(gx+gw), ny-(gy+gh), false);
					setGroupRect();
				}
				else
				{
					currItem = Doc->selection->itemAt(0);
					if (!currItem->asLine())
					{
						if (fabs(currItem->width()) < 5)
							currItem->setWidth(5.0);
						if (fabs(currItem->height()) < 5)
							currItem->setHeight(5.0);
					}
					if (Doc->useRaster)
					{
						double nx = currItem->xPos();
						double ny = currItem->yPos();
						if (!ApplyGuides(&nx, &ny))
						{
							FPoint npx;
							npx = ApplyGridF(FPoint(nx, ny));
							nx = npx.x();
							ny = npx.y();
						}
						MoveItem(nx-currItem->xPos(), ny-currItem->yPos(), currItem);
					}
					else
						MoveItem(0, 0, currItem, false);
				}
				operItemMoving = false;
				if (Doc->selection->isMultipleSelection())
				{
					double gx, gy, gh, gw;
					getGroupRect(&gx, &gy, &gw, &gh);
					FPoint maxSize(gx+gw+Doc->ScratchRight, gy+gh+Doc->ScratchBottom);
					FPoint minSize(gx-Doc->ScratchLeft, gy-Doc->ScratchTop);
					Doc->adjustCanvas(minSize, maxSize);
				}
				Doc->setRedrawBounding(currItem);
				currItem->OwnPage = Doc->OnPage(currItem);
				if (currItem->OwnPage != -1)
				{
					Doc->currentPage = Doc->Pages->at(currItem->OwnPage);
					setMenTxt(currItem->OwnPage);
				}
				//CB done with emitAllToGUI
				//emit HaveSel(currItem->itemType());
				//EmitValues(currItem);
				//CB need this for? a moved item will send its new data with the new xpos/ypos emits
				//CB TODO And what if we have dragged to a new page. Items X&Y are not updated anyway now
				//currItem->emitAllToGUI();
				updateContents();
				emit DocChanged();
			}
		}
		//CB Drag selection performed here
		if ((Doc->selection->count() == 0) && (HaveSelRect) && (!MidButt))
		{
			double sc = Scale;
			QPainter p;
			p.begin(viewport());
			ToView(&p);
			p.scale(Scale, Scale);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			p.drawRect(Mxp, Myp, SeRx-Mxp, SeRy-Myp);
			p.end();
			if(Mxp > SeRx)
			{
				double tmp=SeRx;
				SeRx=static_cast<int>(Mxp);
				Mxp=static_cast<int>(tmp);
			}
			if(Myp > SeRy)
			{
				double tmp=SeRy;
				SeRy=static_cast<int>(Myp);
				Myp=static_cast<int>(tmp);
			}
			QRect Sele = QRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>((SeRx-Mxp)*sc), static_cast<int>((SeRy-Myp)*sc));
			uint docItemCount=Doc->Items->count();
			if (docItemCount != 0)
			{
				for (uint a = 0; a < docItemCount; ++a)
				{
					PageItem* docItem = Doc->Items->at(a);
					p.begin(viewport());
					Transform(docItem, &p);
					QRegion apr = QRegion(p.xForm(docItem->Clip));
					QRect apr2(docItem->getRedrawBounding(Scale));
					p.end();
					if ((Doc->masterPageMode()) && (docItem->OnMasterPage != Doc->currentPage->PageNam))
						continue;
					//CB Finally Items are selected here
					if (((Sele.contains(apr.boundingRect())) || (Sele.contains(apr2))) && (docItem->LayerNr == Doc->activeLayer()))
					{
					//CB set draw to true to (dis)enable some actions via emit to HaveNewSel in scapp.
					//CB FIXME emit from selection when multiple selected instead
						bool redrawSelection=(a==docItemCount-1);
						SelectItemNr(a, redrawSelection);
					}
				}
			}
			HaveSelRect = false;
		}
		if (Doc->appMode != modeEdit)
		{
			if (Doc->appMode == modeRotation)
				Doc->RotMode = RotMode;
			Doc->appMode = modeNormal;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			emit PaintingDone();
		}
		if (GetItem(&currItem))
		{
			if (Doc->selection->count() > 1)
			{
				setGroupRect();
				paintGroupRect();
				//CB shouldnt need this i think
				//emit HaveSel(currItem->itemType());
				double x, y, w, h;
				getGroupRect(&x, &y, &w, &h);
				emit ItemPos(x, y);
				emit ItemGeom(w, h);
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
	if ((Doc->appMode == modeEdit) && !operItemResizeInEditMode)
	{
		currItem = Doc->selection->itemAt(0);
		uint a;
		if (currItem->asTextFrame())
		{
			if (oldCp == currItem->CPos)
			{
				for (a = 0; a < currItem->itemText.count(); ++a)
					currItem->itemText.at(a)->cselect = false;
				currItem->HasSel = false;
				emit HasNoTextSel();
				RefreshItem(currItem);
			}
		}
		else
			emit DocChanged();
	}
	if (Doc->appMode == modeMagnifier)
	{
		double sc = Scale;
		if (HaveSelRect)
		{
			if((Mxp*sc) > SeRx)
			{
				double tmp=SeRx;
				SeRx=static_cast<int>(Mxp*sc);
				Mxp=static_cast<int>(tmp/sc);
			}
			if((Myp*sc) > SeRy)
			{
				double tmp=SeRy;
				SeRy=static_cast<int>(Myp*sc);
				Myp=static_cast<int>(tmp/sc);
			}
			double yf = height() / (SeRy/sc-Myp);
			double xf = width() / (SeRx/sc-Mxp);
			setScale(QMIN(yf, xf));
			slotDoZoom();
			if (sc == Scale)
			{
				SetCPo(Mxp, Myp);
				HaveSelRect = false;
				Doc->appMode = modeNormal;
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				emit PaintingDone();
			}
		}
		else
		{
			int mx = qRound(m->x() / Scale + Doc->minCanvasCoordinate.x());
			int my = qRound(m->y() / Scale + Doc->minCanvasCoordinate.y());
			Magnify ? slotZoomIn(mx,my) : slotZoomOut(mx,my);
			if (sc == Scale)
			{
				HaveSelRect = false;
				Doc->appMode = modeNormal;
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				emit PaintingDone();
			}
			else
				qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
		}
	}
	if ((Doc->appMode == modeDrawBezierLine) && (m->button() == LeftButton))
	{
		currItem = Doc->selection->itemAt(0);
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		QPainter p;
		QPointArray Bez(4);
		p.begin(viewport());
		Transform(currItem, &p);
		FPoint npf(p.xFormDev(m->pos()));
		npf += FPoint(Doc->minCanvasCoordinate.x(), Doc->minCanvasCoordinate.y());
		npf = ApplyGridF(npf);
		currItem->PoLine.addPoint(npf);
		bool ssiz = currItem->Sizing;
		currItem->Sizing = true;
		if ((currItem->PoLine.size() % 4 == 0) && (currItem->PoLine.size() > 3))
		{
			FPoint lxy(currItem->PoLine.point(currItem->PoLine.size()-2));
			FPoint lk(currItem->PoLine.point(currItem->PoLine.size()-1));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			currItem->PoLine.addPoint(lxy);
			currItem->PoLine.addPoint(lk);
		}
		FPoint np2(getMinClipF(&currItem->PoLine));
		if (np2.x() < 0)
		{
			currItem->PoLine.translate(-np2.x(), 0);
			MoveItem(np2.x(), 0, currItem);
		}
		if (np2.y() < 0)
		{
			currItem->PoLine.translate(0, -np2.y());
			MoveItem(0, np2.y(), currItem);
		}
		if (FirstPoly)
		{
			FirstPoly = false;
			currItem->Sizing = ssiz;
		}
		else
		{
		SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false, false);
		currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2, 1)));
		AdjustItemSize(currItem);
		currItem->Sizing = ssiz;
		currItem->ContourLine = currItem->PoLine.copy();
		RefreshItem(currItem);
		}
		p.end();
	}
	if ((Doc->appMode == modeDrawBezierLine) && (m->button() == RightButton))
	{
		currItem = Doc->selection->itemAt(0);
		if (currItem!=0)
		{
			currItem->PoLine.resize(currItem->PoLine.size()-2);
			if (currItem->PoLine.size() < 4)
			{
	//			emit DelObj(Doc->currentPage->pageNr(), currItem->ItemNr);
				Doc->Items->remove(currItem->ItemNr);
				Doc->selection->removeFirst();
				emit HaveSel(-1);
			}
			else
			{
				SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false);
				currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2.0, 1)));
				AdjustItemSize(currItem);
				currItem->ContourLine = currItem->PoLine.copy();
			}
		}
		Doc->appMode = modeNormal;
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		emit PaintingDone();
		emit DocChanged();
		FirstPoly = true;
		updateContents();
	}
	Doc->DragP = false;
	Doc->leaveDrag = false;
	operItemMoving = false;
	operItemResizing = false;
	MidButt = false;
	Doc->SubMode = -1;
	if (_groupTransactionStarted)
	{
		for (uint i = 0; i < Doc->selection->count(); ++i)
			Doc->selection->itemAt(i)->checkChanges(true);
		undoManager->commit();
		_groupTransactionStarted = false;
	}

	for (uint i = 0; i < Doc->selection->count(); ++i)
		Doc->selection->itemAt(i)->checkChanges(true);
	//Commit drag created items to undo manager. View needs to emit this AddObj() for the outline palette (for now)
	if (Doc->selection->itemAt(0)!=NULL)
	{
		bool emitToOutline = Doc->itemAddCommit(Doc->selection->itemAt(0)->ItemNr);
		if (emitToOutline)
			emit AddObj(Doc->selection->itemAt(0));
	}
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	zoomSpinBox->clearFocus();
	pageSelector->clearFocus();
}

void ScribusView::contentsMouseMoveEvent(QMouseEvent *m)
{
	int newX, newY, c;
	double nx, ny, dx, dy;
	uint a;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QPainter p;
	QRect tx;
	QPointArray Bez(4);
	bool erf = false;
	double sc = Scale;
	horizRuler->Draw(m->x());
	vertRuler->Draw(m->y());
	emit MousePos(m->x()/Scale + Doc->minCanvasCoordinate.x(), m->y()/Scale + Doc->minCanvasCoordinate.y());
	if (Doc->guidesSettings.guidesShown)
	{
		if (MoveGY)
		{
			FromHRuler(m);
			if (((m->y()/sc) < Doc->currentPage->yOffset()- Doc->minCanvasCoordinate.y()) || ((m->y()/sc) > Doc->currentPage->height()+Doc->currentPage->yOffset()- Doc->minCanvasCoordinate.y()))
				qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png")), true);
			else
				qApp->setOverrideCursor(QCursor(SPLITHC), true);
			return;
		}
		if (MoveGX)
		{
			FromVRuler(m);
			if (((m->x()/sc) < Doc->currentPage->xOffset()- Doc->minCanvasCoordinate.x()) || ((m->x()/sc) > Doc->currentPage->width()+Doc->currentPage->xOffset()- Doc->minCanvasCoordinate.x()))
				qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png")), true);
			else
				qApp->setOverrideCursor(QCursor(SPLITVC), true);
			return;
		}
	}
/*	if ((!Doc->DragP) && (m_MouseButtonPressed) && (SelItem.count() != 0))
	{
		int cx = Anz->contentsX() - Anz->childX(doku->currentPage->parentWidget());
		int cy = Anz->contentsY() - Anz->childY(doku->currentPage->parentWidget());
		int cw = Anz->visibleWidth();
		int ch = Anz->visibleHeight();
		if ((m->y() < cy+10) || ((ch - (m->y() - cy)) - 10 < 0) || (m->x() < cx + 10) || ((cw - (m->x() - cx)) - 10 < 0))
		{
			if (m->y() < cy+10)
				Anz->scrollBy(0, m->y() - cy - 10);
			else if ((ch - (m->y() - cy)) - 10 < 0)
				Anz->scrollBy(0, m->y() - cy - ch +10);
			else if (m->x() < cx + 10)
				Anz->scrollBy(m->x() - cx - 10, 0);
			else if ((cw - (m->x() - cx)) - 10 < 0)
				Anz->scrollBy(m->x() - cx - cw + 10, 0);
			BlockLeave = true;
			update();
		}
	else
		BlockLeave = false;
	} */

	if (m_MouseButtonPressed && (Doc->appMode == modeEditGradientVectors))
	{
		PageItem *currItem = Doc->selection->itemAt(0);
		newX = m->x();
		newY = m->y();
		if (m->state() == LeftButton)
		{
			currItem->GrStartX -= (Mxp - newX) / Scale;
			currItem->GrStartX = QMIN(QMAX(0.0, currItem->GrStartX), currItem->width());
			currItem->GrStartY -= (Myp - newY) / Scale;
			currItem->GrStartY = QMIN(QMAX(0.0, currItem->GrStartY), currItem->height());
		}
		if (m->state() == RightButton)
		{
			currItem->GrEndX -= (Mxp - newX) / Scale;
			currItem->GrEndX = QMIN(QMAX(0.0, currItem->GrEndX), currItem->width());
			currItem->GrEndY -= (Myp - newY) / Scale;
			currItem->GrEndY = QMIN(QMAX(0.0, currItem->GrEndY), currItem->height());
		}
		Mxp = newX;
		Myp = newY;
		RefreshItem(currItem);
		ScMW->propertiesPalette->updateColorSpecialGradient();
		return;
	}
	if (m_MouseButtonPressed && (Doc->appMode == modeMeasurementTool))
	{
		newX = m->x();
		newY = m->y();
		p.begin(viewport());
		QPoint out = contentsToViewport(QPoint(0, 0));
		p.translate(out.x(), out.y());
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.drawLine(Dxp, Dyp, Mxp, Myp);
		p.drawLine(Dxp, Dyp, newX, newY);
		p.end();
		Mxp = newX;
		Myp = newY;
		double dxp = Dxp / sc - Doc->currentPage->xOffset()+ Doc->minCanvasCoordinate.x();
		double dyp = Dyp / sc - Doc->currentPage->yOffset()+ Doc->minCanvasCoordinate.y();
		double nxp = newX / sc - Doc->currentPage->xOffset()+ Doc->minCanvasCoordinate.x();
		double nyp = newY / sc - Doc->currentPage->yOffset()+ Doc->minCanvasCoordinate.y();
		emit MVals(dxp, dyp, nxp, nyp, -xy2Deg(newX/sc - Dxp*sc, newY/sc - Dyp/sc), sqrt(pow(newX/sc - Dxp/sc,2)+pow(newY/sc - Dyp/sc,2)), Doc->unitIndex());
		return;
	}
	if (m_MouseButtonPressed && (m->state() & Qt::RightButton) && (m->state() & Qt::ControlButton))
	{
		ScMW->setAppMode(modePanning);
	}
	if (m_MouseButtonPressed && (Doc->appMode == modePanning))
	{
		int scroX = m->x() - qRound((Mxp * sc));
		int scroY = m->y() - qRound((Myp * sc));
		scrollBy(-scroX, -scroY);
		Mxp = static_cast<int>((m->x()-scroX)/sc);
		Myp = static_cast<int>((m->y()-scroY)/sc);
		return;
	}
	if (m_MouseButtonPressed && (Doc->appMode == modeDrawFreehandLine))
	{
		newX = m->x();
		newY = m->y();
		double newXF = qRound(m->x()/sc + Doc->minCanvasCoordinate.x());
		double newYF = qRound(m->y()/sc + Doc->minCanvasCoordinate.y());
		if (RecordP.size() > 0)
		{
			if (FPoint(newXF, newYF) != RecordP.point(RecordP.size()-1))
				RecordP.addPoint(FPoint(newXF, newYF));
		}
		else
			RecordP.addPoint(FPoint(newXF, newYF));
		p.begin(viewport());
		ToView(&p);
		p.translate(qRound(Doc->minCanvasCoordinate.x()*Scale), qRound(Doc->minCanvasCoordinate.y()*Scale));
		if (RecordP.size() > 1)
		{
			FPoint xp(RecordP.point(RecordP.size()-2));
			p.drawLine(qRound((xp.x() - Doc->minCanvasCoordinate.x())*sc), qRound((xp.y() - Doc->minCanvasCoordinate.y())*sc), newX, newY);
		}
		else
			p.drawPoint(m->x(), m->y());
		p.end();
		return;
	}
	if (GetItem(&currItem))
	{
		newX = qRound(m->x()/sc + Doc->minCanvasCoordinate.x());
		newY = qRound(m->y()/sc + Doc->minCanvasCoordinate.y());
		if ((moveTimer.elapsed() > 500) && (m_MouseButtonPressed) && (m->state() == RightButton) && (!Doc->DragP) && (Doc->appMode == modeNormal) && (!currItem->locked()) && (!(currItem->isTableItem && currItem->isSingleSel)))
		{
			if ((abs(Dxp - newX) > 10) || (abs(Dyp - newY) > 10))
			{
				Doc->DragP = true;
				Doc->leaveDrag = false;
				Doc->DraggedElem = currItem;
				Doc->DragElements.clear();
				for (uint dre=0; dre<Doc->selection->count(); ++dre)
					Doc->DragElements.append(Doc->selection->itemAt(dre)->ItemNr);
				ScriXmlDoc *ss = new ScriXmlDoc();
				QDragObject *dr = new QTextDrag(ss->WriteElem(Doc, this, Doc->selection), this);
				dr->setPixmap(loadIcon("DragPix.xpm"));
				dr->drag();
//				if (!dr->drag())
//					qDebug("ScribusView::contentsMouseMoveEvent: couldn't start drag operation!");
/* commented out the code above as the debug message is incorrect,
   see the Qt Reference: "The function returns TRUE if the caller should delete the original copy
    of the dragged data (but see target()); otherwise returns FALSE." */
				delete ss;
				ss=NULL;
				Doc->DragP = false;
				Doc->leaveDrag = false;
				m_MouseButtonPressed = false;
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
			}
			return;
		}
		if (Doc->DragP)
			return;
		if ((moveTimer.elapsed() > 500) && m_MouseButtonPressed && (Doc->appMode == modeRotation))
		{
			double newW = xy2Deg(m->x()/sc - RCenter.x(), m->y()/sc - RCenter.y());
			if (m->state() & ControlButton)
			{
				newW=constrainAngle(newW);
				oldW=constrainAngle(oldW);
				//RotateGroup uses MoveBy so its pretty hard to constrain the result
				if (Doc->selection->isMultipleSelection())
					RotateGroup(newW-oldW);
				else
					RotateItem(newW, currItem->ItemNr);
			}
			else
			{
				if (Doc->selection->isMultipleSelection())
					RotateGroup(newW - oldW);
				else
					RotateItem(currItem->rotation() - (oldW - newW), currItem->ItemNr);
			}
			oldW = newW;
			emit DocChanged();
		}
		if (Doc->appMode == modeDrawBezierLine)
		{
			p.begin(viewport());
			ToView(&p);
			p.scale(Scale, Scale);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if ((Doc->useRaster) && (Doc->OnPage(currItem) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			if (!m_MouseButtonPressed)
			{
				QPoint a1 = currItem->PoLine.pointQ(currItem->PoLine.size()-2);
				QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
				a1 += QPoint(qRound(currItem->xPos()), qRound(currItem->yPos()));
				a2 += QPoint(qRound(currItem->xPos()), qRound(currItem->yPos()));
				BezierPoints(&Bez, a1, a2, QPoint(Mxp, Myp), QPoint(Mxp, Myp));
				p.drawCubicBezier(Bez);
				Bez.setPoint(2, QPoint(newX, newY));
				Bez.setPoint(3, QPoint(newX, newY));
				p.drawCubicBezier(Bez);
			}
			else
			{
				QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
				a2 += QPoint(qRound(currItem->xPos()), qRound(currItem->yPos()));
				if (currItem->PoLine.size() > 2)
				{
					QPoint a1 = currItem->PoLine.pointQ(currItem->PoLine.size()-2);
					QPoint a3 = currItem->PoLine.pointQ(currItem->PoLine.size()-3);
					a1 += QPoint(qRound(currItem->xPos()), qRound(currItem->yPos()));
					a3 += QPoint(qRound(currItem->xPos()), qRound(currItem->yPos()));
					BezierPoints(&Bez, a3, a1, QPoint(Mxp, Myp), a2);
					p.drawCubicBezier(Bez);
					Bez.setPoint(2, QPoint(newX, newY));
					Bez.setPoint(3, a2);
					p.drawCubicBezier(Bez);
				}
				p.drawLine(a2, QPoint(Mxp, Myp));
				p.drawLine(a2, QPoint(newX, newY));
			}
			p.end();
			Mxp = newX;
			Myp = newY;
		}
		if (m_MouseButtonPressed && (Doc->appMode == modeDrawRegularPolygon))
		{
			p.begin(viewport());
			ToView(&p);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (m->state() & ShiftButton)
			{
				mop = QPoint(m->x(), static_cast<int>((currItem->yPos() + (newX - currItem->xPos())) * sc));
				QCursor::setPos(mapToGlobal(mop));
				newY = static_cast<int>(mop.y()/sc);
			}
			if ((Doc->useRaster) && (Doc->OnPage(currItem) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			p.drawRect(static_cast<int>(currItem->xPos()*sc), static_cast<int>(currItem->yPos()*sc),
			           static_cast<int>(Mxp*sc-currItem->xPos()*sc), static_cast<int>(Myp*sc-currItem->yPos()*sc));
			p.drawRect(static_cast<int>(currItem->xPos()*sc), static_cast<int>(currItem->yPos()*sc),
			           static_cast<int>(newX*sc-currItem->xPos()*sc), static_cast<int>(newY*sc-currItem->yPos()*sc));
			p.end();
			emit ItemGeom(newX - currItem->xPos(), newY - currItem->yPos());
			Mxp = newX;
			Myp = newY;
		}
		if (m_MouseButtonPressed && (Doc->appMode == modeDrawLine))
		{
			p.begin(viewport());
			ToView(&p);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (Doc->useRaster)
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			double newRot=xy2Deg(newX - currItem->xPos(), newY - currItem->yPos());
			//Constrain rotation angle, when the mouse is being dragged around for a new line
			if (m->state() & ControlButton)
			{
				//Flip our angles around here
				if (newRot<0.0)
					newRot=-newRot;
				else
					newRot=360-newRot;
				newRot=constrainAngle(newRot);
				double hlen=sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2));
				newX = qRound(currItem->xPos()+(hlen * cos(newRot/(180.0/M_PI))));
				newY = qRound(currItem->yPos()-(hlen * sin(newRot/(180.0/M_PI))));
			}
			
			p.drawLine(static_cast<int>(currItem->xPos()*sc), static_cast<int>(currItem->yPos()*sc), static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc));
			p.drawLine(static_cast<int>(currItem->xPos()*sc), static_cast<int>(currItem->yPos()*sc), static_cast<int>(newX*sc), static_cast<int>(newY*sc));
			p.end();
			emit SetAngle(newRot);
			emit ItemGeom(sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2)), 0);
			Mxp = newX;
			Myp = newY;
		}
		if (m_MouseButtonPressed && (Doc->appMode == modeEdit) && (!operItemResizeInEditMode))
		{
			if (currItem->asImageFrame())
			{
				QWMatrix ro;
				ro.rotate(-currItem->rotation());
				QPoint rota = ro.map(QPoint(newX-Mxp,newY-Myp));
				currItem->moveImageInFrame(rota.x()/currItem->imageXScale(), rota.y()/currItem->imageYScale());
				updateContents(currItem->getRedrawBounding(Scale));
				Mxp = newX;
				Myp = newY;
			}
			if (currItem->asTextFrame())
			{
				for (a = 0; a < currItem->itemText.count(); ++a)
					currItem->itemText.at(a)->cselect = false;
				currItem->HasSel = false;
				slotSetCurs(m->x(), m->y());
				if (currItem->itemText.count() > 0)
				{
					if (currItem->CPos < oldCp)
					{
						for (c = currItem->CPos; c < oldCp; ++c)
						{
							currItem->itemText.at(c)->cselect = true;
							currItem->HasSel = true;
						}
					}
					if (currItem->CPos > oldCp)
					{
						for (c = oldCp; c < currItem->CPos; ++c)
						{
							currItem->itemText.at(c)->cselect = true;
							currItem->HasSel = true;
						}
					}
				}
				RefreshItem(currItem);
				emit currItem->HasSel ? HasTextSel() : HasNoTextSel();
			}
		}
		//Operations run here:
		//Item resize, esp after creating a new one
		if ((moveTimer.elapsed() > 500) && m_MouseButtonPressed && (m->state() & LeftButton) && ((Doc->appMode == modeNormal) || ((Doc->appMode == modeEdit) && operItemResizeInEditMode)) && (!currItem->locked()))
		{
			if (Doc->EditClip)
			{
				if ((m_MouseButtonPressed) && (ClRe == -1) && (SegP1 == -1) && (SegP2 == -1))
				{
					newX = qRound(m->x()/sc + Doc->minCanvasCoordinate.x());
					newY = qRound(m->y()/sc + Doc->minCanvasCoordinate.y());
					p.begin(viewport());
					ToView(&p);
					p.scale(Scale, Scale);
					p.setRasterOp(XorROP);
					p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
					p.drawRect(Mxp, Myp, SeRx-Mxp, SeRy-Myp);
					p.drawRect(Mxp, Myp, newX-Mxp, newY-Myp);
					p.end();
					SeRx = newX;
					SeRy = newY;
					HaveSelRect = true;
					return;
				}
				newX = m->x();
				newY = m->y();
				operItemMoving = true;
				currItem = Doc->selection->itemAt(0);
				currItem->OldB2 = currItem->width();
				currItem->OldH2 = currItem->height();
				FPointArray Clip;
				if (EditContour)
					Clip = currItem->ContourLine;
				else
					Clip = currItem->PoLine;
				npf.setX(Clip.point(ClRe).x() + (newX-Mxp) / Scale);
				npf.setY(Clip.point(ClRe).y() + (newY-Myp) / Scale);
				if ((SegP1 != -1) && (SegP2 != -1))
				{
					npf.setX(Clip.point(SegP2).x() + (newX-Mxp) / Scale);
					npf.setY(Clip.point(SegP2).y() + (newY-Myp) / Scale);
					ClRe = SegP2;
					MoveClipPoint(currItem, npf);
					currItem->OldB2 = currItem->width();
					currItem->OldH2 = currItem->height();
					ClRe = SegP1;
					npf2.setX(Clip.point(SegP1).x() + (newX-Mxp) / Scale);
					npf2.setY(Clip.point(SegP1).y() + (newY-Myp) / Scale);
					MoveClipPoint(currItem, npf2);
					Mxp = newX;
					Myp = newY;
				}
				else
				{
					if ((SelNode.count() != 0) && (EdPoints))
					{
						int storedClRe = ClRe;
						for (uint itm = 0; itm < SelNode.count(); ++itm)
						{
							npf.setX(Clip.point(*SelNode.at(itm)).x() + (newX-Mxp) / Scale);
							npf.setY(Clip.point(*SelNode.at(itm)).y() + (newY-Myp) / Scale);
							ClRe = *SelNode.at(itm);
							currItem->OldB2 = currItem->width();
							currItem->OldH2 = currItem->height();
							if (((ClRe != 0) && (SelNode.count() > 1)) || (SelNode.count() == 1))
								MoveClipPoint(currItem, npf);
						}
						currItem->OldB2 = currItem->width();
						currItem->OldH2 = currItem->height();
						ClRe = storedClRe;
					}
					else
						MoveClipPoint(currItem, npf);
					Mxp = newX;
					Myp = newY;
				}
				return;
			}
			if (operItemResizing)
			{
				newX = static_cast<int>(m->x()/sc);
				newY = static_cast<int>(m->y()/sc);
				operItemMoving = false;
				if (Doc->selection->isMultipleSelection())
				{
					newX = qRound(m->x()/sc + Doc->minCanvasCoordinate.x());
					newY = qRound(m->y()/sc + Doc->minCanvasCoordinate.y());
					double gx, gy, gh, gw;
					getGroupRect(&gx, &gy, &gw, &gh);
					int ox1 = qRound(gx*sc);
					int oy1 = qRound(gy*sc);
					int ox2 = qRound((gx+gw)*sc);
					int oy2 = qRound((gy+gh)*sc);
					np2 = QPoint(newX, newY);
					np2 = ApplyGrid(np2);
					double nx = np2.x();
					double ny = np2.y();
					ApplyGuides(&nx, &ny);
					np2 = QPoint(qRound(nx*sc), qRound(ny*sc));
					p.begin(viewport());
					ToView(&p);
					switch (frameResizeHandle)
					{
					case 1:
						if (m->state() & ControlButton)
							np2 = QPoint(qRound(newX), qRound(gy+(gh * ((newX-gx) / gw))));
						else
							np2 = QPoint(qRound(newX), qRound(newY));
						np2 = ApplyGrid(np2);
						nx = np2.x();
						ny = np2.y();
						ApplyGuides(&nx, &ny);
						np2 = QPoint(qRound(nx*sc), qRound(ny*sc));
						PaintSizeRect(&p, QRect(QPoint(qRound(gx*sc), qRound(gy*sc)), np2));
						break;
					case 2:
						PaintSizeRect(&p, QRect(np2, QPoint(ox2,oy2)));
						break;
					case 3:
						PaintSizeRect(&p, QRect(np2, QPoint(ox1, oy2)));
						break;
					case 4:
						PaintSizeRect(&p, QRect(np2, QPoint(ox2, oy1)));
						break;
					case 5:
						PaintSizeRect(&p, QRect(QPoint(ox1, oy1), QPoint(ox2, np2.y())));
						break;
					case 6:
						PaintSizeRect(&p, QRect(QPoint(np2.x(), oy2), QPoint(ox1,oy1)));
						break;
					case 7:
						PaintSizeRect(&p, QRect(QPoint(np2.x(), oy1), QPoint(ox2, oy2)));
						break;
					case 8:
						PaintSizeRect(&p, QRect(QPoint(ox1, qRound(np2.y())), QPoint(ox2, oy2)));
						break;
					}
					Mxp = qRound(np2.x());
					Myp = qRound(np2.y());
					p.end();
				}
				else
				{
					//qDebug(QString("frameResizeHandle %1").arg(frameResizeHandle));
					for (a = 0; a < Doc->selection->count(); ++a)
					{
						currItem = Doc->selection->itemAt(0);
						double nh = currItem->height();
						if ((frameResizeHandle == 1) || (frameResizeHandle == 2))
						{
							switch (frameResizeHandle)
							{
							case 1:
								p.begin(viewport());
								Transform(currItem, &p);
								p.translate(qRound(-Doc->minCanvasCoordinate.x()), qRound(-Doc->minCanvasCoordinate.y()));
								//Shift proportional square resize
								if ((m->state() & ShiftButton) && (!(m->state() & ControlButton)))
								{
									QWMatrix ma;
									ma.translate(currItem->xPos(), currItem->yPos());
									ma.rotate(currItem->rotation());
									ma = ma.invert();
									nh = ma.m11() * newX + ma.m21() * newY + ma.dx();
									mop = QPoint(m->x(), m->y());
								}
								else
								{
									//Control proportional resize
									if ((m->state() & ControlButton) && (!(m->state() & ShiftButton)))
									{
										QWMatrix ma;
										ma.translate(currItem->xPos(), currItem->yPos());
										ma.rotate(currItem->rotation());
										ma = ma.invert();
										double nX = ma.m11() * newX + ma.m21() * newY + ma.dx();
										nh = nX / currItem->OldB2 * currItem->OldH2;
										mop = QPoint(m->x(), m->y());
									}
									else
										mop = QPoint(m->x(), m->y());
								}
								np = p.xFormDev(mop);
								nx = np.x();
								ny = np.y();
								p.end();
								if (!currItem->asLine())
								{
									if ((Doc->useRaster) && (Doc->OnPage(currItem) != -1))
									{
										dx = currItem->xPos() - int (currItem->xPos() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
										dy = currItem->yPos() - int (currItem->yPos() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
										nx = (qRound(np.x() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid - dx);
										ny = (qRound(np.y() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid - dy);
									}
									if (Doc->SnapGuides)
									{
										nx += currItem->xPos();
										ny += currItem->yPos();
										ApplyGuides(&nx, &ny);
										nx -= currItem->xPos();
										ny -= currItem->yPos();
									}
									if ((m->state() & ControlButton) || ((m->state() & ShiftButton)))
										erf = SizeItem(nx, nh, currItem->ItemNr);
									else
									erf = SizeItem(nx, ny, currItem->ItemNr);
								}
								else
								{
									p.begin(viewport());
									double rba = currItem->rotation();
									currItem->setRotation(0.0);
									Transform(currItem, &p);
									p.translate(qRound(-Doc->minCanvasCoordinate.x()), qRound(-Doc->minCanvasCoordinate.y()));
									np = p.xFormDev(QPoint(m->x(), m->y()));
									p.end();
									double sizeItemX=np.x(), sizeItemY=np.y();
									//Constrain rotation angle, when the mouse is moving the non-origin point of a line
									if (m->state() & ControlButton)
									{
										double newRot=xy2Deg(np.x(), np.y());
										rba=constrainAngle(newRot);
										double hlen=sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2));
										sizeItemX = hlen * cos(rba/(180.0/M_PI));
										sizeItemY = hlen * sin(rba/(180.0/M_PI));
									}
									currItem->setRotation(rba);
									np = ApplyGrid(np);
									erf = SizeItem(sizeItemX, sizeItemY, currItem->ItemNr);
									if (Doc->SnapGuides)
									{
										p.begin(viewport());
										currItem->Sizing = true;
										p.setRasterOp(XorROP);
										p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
										p.drawLine(static_cast<int>(currItem->xPos()*sc), static_cast<int>(currItem->yPos()*sc),
												static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc));
										p.drawLine(static_cast<int>(currItem->xPos()*sc), static_cast<int>(currItem->yPos()*sc),
												static_cast<int>(newX*sc), static_cast<int>(newY*sc));
										p.end();
									}
								}
								break;
							case 2:
								if (currItem->asLine())
								{
									double sav = Doc->SnapGuides;
									npf2 = FPoint(newX-Mxp, newY-Myp);
									//Constrain rotation on left point move, disabled for now in movesizeitem
									erf = MoveSizeItem(npf2, FPoint(0, 0), currItem->ItemNr, false, (m->state() & ControlButton));
									Doc->SnapGuides = sav;
									if (sav)
										currItem->Sizing = true;
								}
								else
								{
									newX = qRound(m->x()/sc + Doc->minCanvasCoordinate.x());
									newY = qRound(m->y()/sc + Doc->minCanvasCoordinate.y());
									p.begin(viewport());
									np2 = QPoint(newX, newY);
									np2 = ApplyGrid(np2);
									double nx = np2.x();
									double ny = np2.y();
									ApplyGuides(&nx, &ny);
									p.translate(static_cast<int>(currItem->xPos()), static_cast<int>(currItem->yPos()));
									p.rotate(currItem->rotation());
									np2 = p.xFormDev(QPoint(qRound(nx), qRound(ny)));
									p.end();
									p.begin(viewport());
									ToView(&p);
									Transform(currItem, &p);
									PaintSizeRect(&p, QRect(np2, QPoint(qRound(currItem->width()), qRound(currItem->height()))));
									p.end();
								}
								break;
							}
						}
						else
						{
							p.begin(viewport());
							np2 = QPoint(newX, newY);
							np2 = ApplyGrid(np2);
							double nx = np2.x();
							double ny = np2.y();
							ApplyGuides(&nx, &ny);
							p.translate(static_cast<int>(currItem->xPos() - Doc->minCanvasCoordinate.x()), static_cast<int>(currItem->yPos() - Doc->minCanvasCoordinate.y()));
							p.rotate(currItem->rotation());
							np2 = p.xFormDev(QPoint(qRound(nx), qRound(ny)));
							p.end();
							p.begin(viewport());
							ToView(&p);
							Transform(currItem, &p);
							switch (frameResizeHandle)
							{
							case 0:
								break;
							case 3:
								PaintSizeRect(&p, QRect(np2, QPoint(0, qRound(currItem->height()))));
								break;
							case 4:
								PaintSizeRect(&p, QRect(np2, QPoint(qRound(currItem->width()), 0)));
								break;
							case 5:
								PaintSizeRect(&p, QRect(QPoint(0, 0), QPoint(qRound(currItem->width()), np2.y())));
								break;
							case 6:
								PaintSizeRect(&p, QRect(QPoint(0, 0), QPoint(np2.x(), qRound(currItem->height()))));
								break;
							case 7:
								PaintSizeRect(&p, QRect(QPoint(np2.x(), 0), QPoint(qRound(currItem->width()), qRound(currItem->height()))));
								break;
							case 8:
								PaintSizeRect(&p, QRect(QPoint(0, np2.y()), QPoint(qRound(currItem->width()), qRound(currItem->height()))));
								break;
							}
							p.end();
						}
					}
				}
				if ((erf) || (frameResizeHandle > 1))
				{
					Mxp = newX;
					Myp = newY;
				}
			}
			else
			{
				//Dragging an item (plus more?)
				newX = static_cast<int>(m->x()/sc);
				newY = static_cast<int>(m->y()/sc);
				operItemMoving = true;
				erf = false;
				int dX=newX-Mxp, dY=newY-Myp;
				if (!Doc->selection->isMultipleSelection())
				{
					erf=true;
					currItem = Doc->selection->itemAt(0);
					if ((currItem->asImageFrame()) && (m->state() & ControlButton) && (m->state() & AltButton))
					{
						currItem->moveImageInFrame(dX/currItem->imageXScale(),dY/currItem->imageYScale());
						updateContents(currItem->getRedrawBounding(Scale));
						
					}
					else
					{
						if (m->state() & ControlButton)
						{
							if (abs(dX)>abs(dY))
								dY=0;
							else
							if (abs(dY)>abs(dX))
								dX=0;
							erf=false;
							dX+=qRound(dragConstrainInitPtX-currItem->xPos());
							dY+=qRound(dragConstrainInitPtY-currItem->yPos());
						}
						if (!(currItem->isTableItem && currItem->isSingleSel))
						{
							moveGroup(dX, dY, false);
							if (Doc->SnapGuides)
							{
								double nx = currItem->xPos();
								double ny = currItem->yPos();
								ApplyGuides(&nx, &ny);
								moveGroup(nx-currItem->xPos(), ny-currItem->yPos(), false);
								nx = currItem->xPos()+currItem->width();
								ny = currItem->yPos()+currItem->height();
								ApplyGuides(&nx, &ny);
								moveGroup(nx-(currItem->xPos()+currItem->width()), ny-(currItem->yPos()+currItem->height()), false);
							}
						}
					}
					//erf = true;
				}
				else
				{
					setGroupRect();
					double gx, gy, gh, gw;
					getGroupRect(&gx, &gy, &gw, &gh);
					double gxs, gys, ghs, gws;
					getGroupRectScreen(&gxs, &gys, &gws, &ghs);
					int dX=newX-Mxp, dY=newY-Myp;
					erf = true;
					if (m->state() & ControlButton)
					{
						if (abs(dX)>abs(dY))
							dY=0;
						else
						if (abs(dY)>abs(dX))
							dX=0;
						erf=false;
						dX+=dragConstrainInitPtX-qRound(gx);
						dY+=dragConstrainInitPtY-qRound(gy);
					}
					
					moveGroup(dX, dY, false);
					if (Doc->SnapGuides)
					{
						double nx = gx;
						double ny = gy;
						ApplyGuides(&nx, &ny);
						moveGroup(nx-gxs, ny-gys, false);
						setGroupRect();
						getGroupRect(&gx, &gy, &gw, &gh);
						nx = gx+gw;
						ny = gy+gh;
						ApplyGuides(&nx, &ny);
						moveGroup(nx-(gx+gw), ny-(gy+gh), false);
					}
					setGroupRect();
					getGroupRect(&gx, &gy, &gw, &gh);
					//erf = true;
				}
				if (erf)
				{
					Mxp = newX;
					Myp = newY;
				}
			}
		}
		if ((!m_MouseButtonPressed) && (Doc->appMode != modeDrawBezierLine))
		{
			if (Doc->appMode == modeMagnifier)
			{
				qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
				return;
			}
			if (Doc->selection->isMultipleSelection())
			{
				QRect mpo = QRect(qRound(m->x()/Scale)-Doc->guidesSettings.grabRad, qRound(m->y()/Scale)-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
				mpo.moveBy(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
				double gx, gy, gh, gw;
				getGroupRect(&gx, &gy, &gw, &gh);
				if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					&& ((Doc->appMode == modeNormal) || (Doc->appMode == modeRotation)))
				{
					int how = 0;
					gx -= Doc->minCanvasCoordinate.x();
					gy -= Doc->minCanvasCoordinate.y();
					QMap<double,int> distance;
					double d1 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
					if (d1 < Doc->guidesSettings.grabRad)
						distance.insert(d1, 1);
					double d2 = sqrt(pow((gx * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
					if (d2 < Doc->guidesSettings.grabRad)
						distance.insert(d2, 2);
					double d3 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
					if (d3 < Doc->guidesSettings.grabRad)
						distance.insert(d3, 3);
					double d4 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
					if (d4 < Doc->guidesSettings.grabRad)
						distance.insert(d4, 4);
					double d5 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
					if (d5 < Doc->guidesSettings.grabRad)
						distance.insert(d5, 5);
					double d6 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
					if (d6 < Doc->guidesSettings.grabRad)
						distance.insert(d6, 6);
					double d7 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
					if (d7 < Doc->guidesSettings.grabRad)
						distance.insert(d7, 7);
					double d8 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
					if (d8 < Doc->guidesSettings.grabRad)
						distance.insert(d8, 8);
					QValueList<int> result = distance.values();
					if (result.count() != 0)
					{
						how = result[0];
						switch (how)
						{
							case 1:
							case 2:
								qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
								break;
							case 3:
							case 4:
								qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
								break;
							case 5:
							case 8:
								qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
								break;
							case 6:
							case 7:
								qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
								break;
							default:
								qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
								break;
						}
					}
					else
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
					if (Doc->appMode == modeRotation)
						qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
				}
				else
				{
					switch (Doc->appMode)
					{
						case modeDrawShapes:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")), true);
							break;
						case modeDrawPicture:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")), true);
							break;
						case modeDrawText:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")), true);
							break;
						case modeDrawTable:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTable.xpm")), true);
							break;
						case modeDrawRegularPolygon:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")), true);
							break;
						default:
							qApp->setOverrideCursor(QCursor(ArrowCursor), true);
						break;
					}
				}
				return;
			}
			for (a = 0; a < Doc->selection->count(); ++a)
			{
				currItem = Doc->selection->itemAt(a);
				if (currItem->locked())
					break;
				p.begin(viewport());
				Transform(currItem, &p);
				QRect mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
				mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
				if (Doc->EditClip)
				{
					FPointArray Clip;
					QPointArray cli;
					ClRe2 = -1;
					SegP1 = -1;
					SegP2 = -1;
					if (EditContour)
						Clip = currItem->ContourLine;
					else
						Clip = currItem->PoLine;
					if ((Doc->EditClipMode == 2) || (Doc->EditClipMode == 0) || (Doc->EditClipMode == 3))
					{
						for (a=0; a<Clip.size(); ++a)
						{
							if (((EdPoints) && (a % 2 != 0)) || ((!EdPoints) && (a % 2 == 0)))
								continue;
							np = p.xForm(Clip.pointQ(a));
							tx = QRect(np.x()-3, np.y()-3, 6, 6);
							if (tx.intersects(mpo))
							{
								if (Doc->EditClipMode == 0)
									qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
								if (Doc->EditClipMode == 2)
									qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
								if (Doc->EditClipMode == 3)
									qApp->setOverrideCursor(QCursor(loadIcon("Split.png"), 4, 3), true);
								p.end();
								return;
							}
						}
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
					}
					if ((Doc->EditClipMode == 1) || (Doc->EditClipMode == 0) && (EdPoints))
					{
						for (uint poi=0; poi<Clip.size()-3; poi += 4)
						{
							BezierPoints(&Bez, Clip.pointQ(poi), Clip.pointQ(poi+1), Clip.pointQ(poi+3), Clip.pointQ(poi+2));
							cli = Bez.cubicBezier();
							for (uint clp = 0; clp < cli.size()-1; ++clp)
							{
								if (PointOnLine(cli.point(clp), cli.point(clp+1), p.xFormDev(mpo)))
								{
									if (Doc->EditClipMode == 0)
										qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
									if (Doc->EditClipMode == 1)
										qApp->setOverrideCursor(QCursor(loadIcon("AddPoint.png"), 4, 3), true);
									ClRe2 = poi;
									p.end();
									return;
								}
							}
							qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
						}
					}
				}
				if ((QRegion(p.xForm(QPointArray(QRect(-3, -3, static_cast<int>(currItem->width()+6), static_cast<int>(currItem->height()+6))))).contains(mpo))
					&& ((Doc->appMode == modeNormal) || (Doc->appMode == modeRotation) || (Doc->appMode == modeEdit)))
				{
					tx = p.xForm(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						if (Doc->appMode == modeRotation)
							qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
						else
						if (Doc->appMode == modeEdit)
						{
							if (currItem->asTextFrame())
								qApp->setOverrideCursor(QCursor(ibeamCursor), true);
							if (currItem->asImageFrame())
								qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
						}
						else
							qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
						if (!currItem->sizeLocked())
							HandleCurs(&p, currItem, mpo);
					}
				}
				else
				{
					switch (Doc->appMode)
					{
						case modeDrawShapes:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")), true);
							break;
						case modeDrawPicture:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")), true);
							break;
						case modeDrawText:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")), true);
							break;
						case modeDrawTable:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTable.xpm")), true);
							break;
						case modeDrawRegularPolygon:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")), true);
							break;
						default:
							qApp->setOverrideCursor(QCursor(ArrowCursor), true);
						break;
					}
				}
				p.end();
			}
		}
	}
	else
	{
		if ((m_MouseButtonPressed) && (m->state() == LeftButton) && (GyM == -1) && (GxM == -1))
		{
			newX = qRound(m->x()/sc + Doc->minCanvasCoordinate.x());
			newY = qRound(m->y()/sc + Doc->minCanvasCoordinate.y());
			p.begin(viewport());
			ToView(&p);
			p.scale(Scale, Scale);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			p.drawRect(Mxp, Myp, SeRx-Mxp, SeRy-Myp);
			p.drawRect(Mxp, Myp, newX-Mxp, newY-Myp);
			p.end();
			SeRx = newX;
			SeRy = newY;
			HaveSelRect = true;
			return;
		}
		if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == modeNormal) && (!Doc->GuideLock) && (Doc->OnPage(m->x()/sc, m->y()/sc) != -1) && (!GetItem(&currItem)))
		{
			if (Doc->currentPage->YGuides.count() != 0)
			{
				for (uint yg = 0; yg < Doc->currentPage->YGuides.count(); ++yg)
				{
					if ((Doc->currentPage->YGuides[yg]+Doc->currentPage->yOffset()- Doc->minCanvasCoordinate.y() < (static_cast<int>(m->y()/sc)+Doc->guidesSettings.guideRad)) &&
							(Doc->currentPage->YGuides[yg]+Doc->currentPage->yOffset()- Doc->minCanvasCoordinate.y() > (static_cast<int>(m->y()/sc)-Doc->guidesSettings.guideRad)))
					{
						if ((m_MouseButtonPressed) && (GyM != -1))
							MoveGY = true;
						if (((m->x()/sc) < Doc->currentPage->xOffset()- Doc->minCanvasCoordinate.x()) || ((m->x()/sc) >= Doc->currentPage->width()-1+Doc->currentPage->xOffset()- Doc->minCanvasCoordinate.x()))
							qApp->setOverrideCursor(QCursor(ArrowCursor), true);
						else
							qApp->setOverrideCursor(QCursor(SPLITHC), true);
						return;
					}
				}
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			}
			if (Doc->currentPage->XGuides.count() != 0)
			{
				for (uint xg = 0; xg < Doc->currentPage->XGuides.count(); ++xg)
				{
					if ((Doc->currentPage->XGuides[xg]+Doc->currentPage->xOffset()- Doc->minCanvasCoordinate.x() < (static_cast<int>(m->x()/sc)+Doc->guidesSettings.guideRad)) &&
							(Doc->currentPage->XGuides[xg]+Doc->currentPage->xOffset()- Doc->minCanvasCoordinate.x() > (static_cast<int>(m->x()/sc)-Doc->guidesSettings.guideRad)))
					{
						if ((m_MouseButtonPressed) && (GxM != -1))
							MoveGX = true;
						if (((m->y()/sc) < Doc->currentPage->yOffset()- Doc->minCanvasCoordinate.x()) || ((m->y()/sc) >= Doc->currentPage->height()-1+Doc->currentPage->yOffset()- Doc->minCanvasCoordinate.y()))
							qApp->setOverrideCursor(QCursor(ArrowCursor), true);
						else
							qApp->setOverrideCursor(QCursor(SPLITVC), true);
						return;
					}
				}
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			}
		}
	}
}

void ScribusView::contentsMousePressEvent(QMouseEvent *m)
{
	bool inText;
	uint a;
	int z;
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
	PageItem *bb;
	QPainter p;
	p.begin(viewport());
	PaintSizeRect(&p, QRect());
	p.end();
	FPoint npf, npf2;
	QPointArray Bez(4);
	QRect tx;
	m_MouseButtonPressed = true;
	operItemMoving = false;
	HaveSelRect = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	moveTimer.start();
	Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
	Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	if (Doc->appMode != modeEdit)
	{
		Rxp = ApplyGrid(QPoint(Mxp, Myp)).x();
		Rxpd = Mxp - Rxp;
		Mxp = qRound(Rxp);
		Ryp = ApplyGrid(QPoint(Mxp, Myp)).y();
		Rypd = Myp - Ryp;
		Myp = qRound(Ryp);
	}
	else
	{
		Ryp = Myp;
		Rxp = Mxp;
	}
	switch (Doc->appMode)
	{
		case modeNormal:
			Mxp = qRound(m->x()/Scale);
			Myp = qRound(m->y()/Scale);
			SeRx = Mxp;
			SeRy = Myp;
			if ((Doc->EditClip) && (Doc->selection->count() != 0))
			{
				currItem = Doc->selection->itemAt(0);
				FPointArray Clip;
				bool edited = false;
				bool pfound = false;
				if (EditContour)
					Clip = currItem->ContourLine;
				else
					Clip = currItem->PoLine;
				p.begin(viewport());
				Transform(currItem, &p);
				npf2 = FPoint(p.xFormDev(m->pos()));
				ClRe = -1;
				for (a=0; a<Clip.size(); ++a)
				{
					if (((EdPoints) && (a % 2 != 0)) || ((!EdPoints) && (a % 2 == 0)))
						continue;
					npf = FPoint(p.xForm(Clip.pointQ(a)));
					tx = QRect(static_cast<int>(npf.x()-3), static_cast<int>(npf.y()-3), 6, 6);
					if (tx.intersects(mpo))
					{
						ClRe = a;
						if ((EdPoints) && (SelNode.contains(a) == 0))
						{
							if (m->state() == ShiftButton)
								SelNode.append(a);
							else
							{
								SelNode.clear();
								SelNode.append(a);
							}
						}
						emit ClipPo(Clip.point(a).x(), Clip.point(a).y());
						pfound = true;
						break;
					}
				}
				if ((!pfound) || (!EdPoints))
					SelNode.clear();
				p.end();
				if ((Doc->EditClipMode == 0) && (ClRe2 != -1) && (ClRe == -1))
				{
					SegP1 = ClRe2;
					SegP2 = ClRe2+2;
				}
				FPointArray cli;
				uint EndInd = Clip.size();
				uint StartInd = 0;
				for (uint n = ClRe; n < Clip.size(); ++n)
				{
					if (Clip.point(n).x() > 900000)
					{
						EndInd = n;
						break;
					}
				}
				if (ClRe > 0)
				{
					for (uint n2 = ClRe; n2 > 0; n2--)
					{
						if (n2 == 0)
							break;
						if (Clip.point(n2).x() > 900000)
						{
							StartInd = n2 + 1;
							break;
						}
					}
				}
				if (Doc->EditClipMode == 3)
				{
					if (!EdPoints)
						return;
					if (ClRe != -1)
					{
						if (currItem->asPolygon())
						{
							if ((ClRe != 0) && (ClRe != static_cast<int>(EndInd-2)))
							{
								if (currItem->Segments.count() == 0)
								{
									cli.putPoints(0, EndInd-(ClRe+2), Clip, ClRe+2);
									cli.putPoints(cli.size(), ClRe+2, Clip);
								}
								else
								{
									cli.putPoints(0, EndInd-StartInd, Clip, StartInd);
									//z = PaintPoly(currItem->xPos(), currItem->yPos(), currItem->Width, currItem->Height, currItem->Pwidth, currItem->fillColor(), currItem->lineColor());
									z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
									bb = Doc->Items->at(z);
									if (EditContour)
										bb->ContourLine.resize(0);
									else
										bb->PoLine.resize(0);
									if (StartInd != 0)
									{
										if (EditContour)
										{
											bb->ContourLine.putPoints(0, StartInd - 4, Clip);
											bb->ContourLine.putPoints(bb->ContourLine.size(), Clip.size()-EndInd, Clip, EndInd);
										}
										else
										{
											bb->PoLine.putPoints(0, StartInd - 4, Clip);
											bb->PoLine.putPoints(bb->PoLine.size(), Clip.size()-EndInd, Clip, EndInd);
										}
									}
									else
									{
										if (EditContour)
											bb->ContourLine.putPoints(0, Clip.size()-EndInd-4, Clip, EndInd+4);
										else
											bb->PoLine.putPoints(0, Clip.size()-EndInd-4, Clip, EndInd+4);
									}
									bb->setRotation(currItem->rotation());
									AdjustItemSize(bb);
									bb->ClipEdited = true;
									PageItem *bx = Doc->Items->take(bb->ItemNr);
									Doc->Items->insert(bb->ItemNr-1, bx);
								}
								currItem->PoLine = cli.copy();
							}
							ClRe = -1;
							currItem->ClipEdited = true;
							edited = true;
							Doc->EditClipMode = 0;
							//currItem->convertTo(PageItem::PolyLine);
							//currItem->SetPolyClip(qRound(QMAX(currItem->Pwidth / 2, 1)));
							PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PolyLine);
							//newItem->SetPolyClip(qRound(QMAX(newItem->Pwidth / 2, 1)));
							currItem=newItem;
							emit PolyOpen();
						}
						else
						{
							if ((currItem->asPolyLine()) || (currItem->asPathText()))
							{
								if ((ClRe > 1) && (ClRe < static_cast<int>(Clip.size()-2)))
								{
									//z = PaintPolyLine(currItem->xPos(), currItem->yPos(), currItem->Width, currItem->Height, currItem->Pwidth, currItem->fillColor(), currItem->lineColor());
									z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
									bb = Doc->Items->at(z);
									if (EditContour)
										bb->ContourLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									else
										bb->PoLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									bb->setRotation(currItem->rotation());
									AdjustItemSize(bb);
									bb->ClipEdited = true;
									cli.resize(0);
									cli.putPoints(0, ClRe+2, Clip);
									currItem->PoLine = cli.copy();
								}
								ClRe = -1;
								currItem->ClipEdited = true;
								edited = true;
								Doc->EditClipMode = 0;
								currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2.0, 1)));
								emit PolyOpen();
							}
						}
					}
				}
				if ((Doc->EditClipMode == 2) && (ClRe != -1))
				{
					if (!EdPoints)
						return;
					if ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame()))
					{
						if ((currItem->Segments.count() == 0) && (Clip.size() < 13))
							return;
					}
					else
					{
						if (Clip.size() < 5)
							return;
					}
					if ((currItem->Segments.count() != 0) && ((EndInd - StartInd) < 13))
					{
						if (StartInd != 0)
							cli.putPoints(0, StartInd-4, Clip);
						cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
					}
					else
					{
						if (ClRe == static_cast<int>(StartInd))
						{
							if ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame()))
							{
								FPoint kp(Clip.point(EndInd-3));
								cli.putPoints(0, StartInd, Clip);
								cli.putPoints(cli.size(), EndInd - StartInd - 4, Clip, StartInd);
								cli.setPoint(StartInd, cli.point(cli.size()-2));
								cli.setPoint(StartInd + 1, kp);
								cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
							}
							else
							{
								cli.putPoints(0, StartInd, Clip);
								cli.putPoints(cli.size(), EndInd - StartInd - 4, Clip, StartInd+4);
								cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
							}
						}
						else
						{
							if (ClRe != 0)
								cli.putPoints(0, ClRe, Clip);
							cli.putPoints(cli.size(), Clip.size()-(ClRe + 4), Clip, ClRe+4);
						}
					}
					if (EditContour)
						currItem->ContourLine = cli.copy();
					else
						currItem->PoLine = cli.copy();
					ClRe = -1;
					currItem->ClipEdited = true;
					edited = true;
				}
				if ((Doc->EditClipMode == 1) && (ClRe2 != -1))
				{
					cli.putPoints(0, ClRe2+2, Clip);
					cli.resize(cli.size()+4);
					cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
					cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
					if (EditContour)
						currItem->ContourLine = cli.copy();
					else
						currItem->PoLine = cli.copy();
					ClRe2 = -1;
					currItem->ClipEdited = true;
					edited = true;
				}
				if (edited)
				{
					currItem->FrameType = 3;
					if (!currItem->asPolyLine())
						currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
					AdjustItemSize(currItem);
					updateContents();
					if (EditContour)
						MarkClip(currItem, currItem->ContourLine, true);
					else
						MarkClip(currItem, currItem->PoLine, true);
					emit DocChanged();
				}
				if ((SelNode.count() != 0) || ((SegP1 != -1) && (SegP2 != -1)))
				{
					Mxp = m->x();
					Myp = m->y();
				}
				return;
			}
			if (GetItem(&currItem))
			{
				if (Doc->selection->isMultipleSelection())
				{
					p.begin(viewport());
					QRect ne = QRect();
					PaintSizeRect(&p, ne);
					p.end();
					double gx, gy, gh, gw;;
					getGroupRect(&gx, &gy, &gw, &gh);
					dragConstrainInitPtX = qRound(gx);
					dragConstrainInitPtY = qRound(gy);
					mpo = QRect(qRound(m->x() / Scale) - Doc->guidesSettings.grabRad, qRound(m->y() / Scale) - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
					mpo.moveBy(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
					if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					      && (m->state() != (ControlButton | AltButton)) && (m->state() != ShiftButton))
					{
						frameResizeHandle = 0;
						QMap<double,int> distance;
						gx -= Doc->minCanvasCoordinate.x();
						gy -= Doc->minCanvasCoordinate.y();
						double d1 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
						if (d1 < Doc->guidesSettings.grabRad)
							distance.insert(d1, 1);
						double d2 = sqrt(pow((gx * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
						if (d2 < Doc->guidesSettings.grabRad)
							distance.insert(d2, 2);
						double d3 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
						if (d3 < Doc->guidesSettings.grabRad)
							distance.insert(d3, 3);
						double d4 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
						if (d4 < Doc->guidesSettings.grabRad)
							distance.insert(d4, 4);
						double d5 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
						if (d5 < Doc->guidesSettings.grabRad)
							distance.insert(d5, 5);
						double d6 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
						if (d6 < Doc->guidesSettings.grabRad)
							distance.insert(d6, 6);
						double d7 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
						if (d7 < Doc->guidesSettings.grabRad)
							distance.insert(d7, 7);
						double d8 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
						if (d8 < Doc->guidesSettings.grabRad)
							distance.insert(d8, 8);
						QValueList<int> result = distance.values();
						if (result.count() != 0)
						{
							frameResizeHandle = result[0];
							switch (frameResizeHandle)
							{
								case 1:
								case 2:
									qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
									break;
								case 3:
								case 4:
									qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
									break;
								case 5:
								case 8:
									qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
									break;
								case 6:
								case 7:
									qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
									break;
							}
						}
						if (currItem->sizeLocked())
						{
							qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
							frameResizeHandle = 0;
						}
						if (frameResizeHandle != 0)
							operItemResizing = true;
					}
					else
						SeleItem(m);
					if (Doc->selection->count() == 0)
					{
						Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
						Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
						SeRx = Mxp;
						SeRy = Myp;
					}
				}
				else
				{
					dragConstrainInitPtX = qRound(currItem->xPos());
					dragConstrainInitPtY = qRound(currItem->yPos());
					SeleItem(m); //Where we send the mouse press event to select an item
					if (Doc->selection->count() != 0)
					{
						currItem = Doc->selection->itemAt(0);
						p.begin(viewport());
						Transform(currItem, &p);
						if (!currItem->locked())
						{
							HandleSizer(&p, currItem, mpo, m);
							if (frameResizeHandle != 0)
							{
								if (!currItem->asLine())
									currItem->Sizing = true;
								operItemResizing = true;
							}
						}
						p.end();
					}
					else
					{
						Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
						Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
						SeRx = Mxp;
						SeRy = Myp;
					}
				}
			}
			else
			{
				SeleItem(m);
				if (Doc->selection->count() == 0)
				{
					Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
					Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
					SeRx = Mxp;
					SeRy = Myp;
				}
			}
			if (m->button() == MidButton)
			{
				MidButt = true;
				if (Doc->selection->count() != 0)
					Deselect(true);
				DrawNew();
			}
			if ((Doc->selection->count() != 0) && (m->button() == RightButton))
			{
				m_MouseButtonPressed = true;
				Dxp = Mxp;
				Dyp = Myp;
			}
			break;
		case modeDrawShapes:
			selectPage(m);
			switch (Doc->SubMode)
			{
			case 0:
				z = Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
				SetupDraw(z);
				break;
			case 1:
				z = Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
				SetupDraw(z);
				break;
			default:
				z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
				Doc->Items->at(z)->SetFrameShape(Doc->ValCount, Doc->ShapeValues);
				Doc->setRedrawBounding(Doc->Items->at(z));
				Doc->Items->at(z)->FrameType = Doc->SubMode+2;
				SetupDraw(z);
				break;
			}
			break;
		case modeDrawPicture:
			selectPage(m);
			z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
			SetupDraw(z);
			break;
		case modeDrawText:
			selectPage(m);
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_MouseButtonPressed);
			SetupDraw(z);
			break;
		case modeMagnifier:
			m_MouseButtonPressed = true;
			if ((m->state() == ShiftButton) || (m->button() == RightButton))
				Magnify = false;
			else
				Magnify = true;
			break;
		case modeEdit:
			{
				frameResizeHandle = 0;
				operItemResizeInEditMode = false;
				int oldP=0;
				if (GetItem(&currItem))
				{
					slotDoCurs(false);
					if (!currItem->locked())
					{
						p.begin(viewport());
						Transform(currItem, &p);
						HandleSizer(&p, currItem, mpo, m);
						tx = p.xForm(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
						p.end();
						if (frameResizeHandle != 0)
						{
							operItemResizeInEditMode = true;
							slotDoCurs(true);
							return;
						}
					}
					oldP = currItem->CPos;
				}
				//CB Where we set the cursor for a click in text frame
				inText = slotSetCurs(m->x(), m->y());
				if (!inText)
				{
					Deselect(true);
					slotDoCurs(true);
					emit Amode(modeNormal);
					return;
				}
				//<<CB Add in shift select to text frames
				if (m->state() & Qt::ShiftButton)
				{
					int dir=1;
					if (oldCp>currItem->CPos)
						dir=-1;
					if (currItem->asTextFrame())
						currItem->asTextFrame()->ExpandSel(dir, oldP);
					oldCp = oldP;
				}
				else //>>CB
					oldCp = currItem->CPos;
				currItem = Doc->selection->itemAt(0);
				slotDoCurs(true);
				if ((!inText) && ((currItem->asTextFrame()) || (currItem->asImageFrame())))
				{
					Deselect(true);
					if (SeleItem(m))
					{
						currItem = Doc->selection->itemAt(0);
						if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
							emit Amode(modeEdit);
						else
						{
							emit PaintingDone();
							qApp->setOverrideCursor(QCursor(ArrowCursor), true);
						}
					}
					else
					{
						emit PaintingDone();
						qApp->setOverrideCursor(QCursor(ArrowCursor), true);
					}
				}
				else
				{
					if ((m->button() == MidButton) && (currItem->asTextFrame()))
					{
						m_MouseButtonPressed = false;
						MidButt = false;
						QString cc;
						cc = QApplication::clipboard()->text(QClipboard::Selection);
						if (cc.isNull())
							cc = QApplication::clipboard()->text(QClipboard::Clipboard);
						if (!cc.isNull())
						{
							Serializer *ss = new Serializer("");
							ss->Objekt = cc;
							int st = Doc->currentParaStyle;
							if (st > 5)
								ss->GetText(currItem, st, Doc->docParagraphStyles[st].Font, Doc->docParagraphStyles[st].FontSize, true);
							else
								ss->GetText(currItem, st, currItem->font(), currItem->fontSize(), true);
							delete ss;
							ss=NULL;
							if (Doc->docHyphenator->AutoCheck)
								Doc->docHyphenator->slotHyphenate(currItem);
							emit ChBMText(currItem);
							ScMW->outlinePalette->BuildTree();
						}
						else
						{
							if (ScMW->Buffer2.startsWith("<SCRIBUSTEXT"))
								ScMW->slotEditPaste();
						}
						RefreshItem(currItem);
					}
					if (currItem->asImageFrame() && !tx.contains(m->x(), m->y()))
					{
						Deselect(true);
						emit Amode(modeNormal);
					}
				}
			}
			break;
		case modeDrawLine:
			selectPage(m);
			z = Doc->itemAdd(PageItem::Line, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, Rypd, Doc->toolSettings.dWidthLine, CommonStrings::None, Doc->toolSettings.dPenLine, !m_MouseButtonPressed);
			currItem = Doc->Items->at(z);
			qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
			Doc->selection->clear();
			Doc->selection->addItem(currItem);
			currItem->paintObj();
			operItemMoving = true;
			break;
		case modeRotation:
			if (GetItem(&currItem))
			{
				RotMode = Doc->RotMode;
				if (Doc->selection->isMultipleSelection())
				{
					double gx, gy, gh, gw;
					double gxR, gyR, ghR, gwR;
					getGroupRectScreen(&gx, &gy, &gw, &gh);
					getGroupRect(&gxR, &gyR, &gwR, &ghR);
					if (QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					{
						Doc->RotMode = 2;
						RCenter = FPoint(gxR+gwR/2.0, gyR+ghR/2.0);
						if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
						{
							RCenter = FPoint(gxR, gyR);
							Doc->RotMode = 0;
						}
					}
					oldW = xy2Deg(m->x()/Scale - RCenter.x(), m->y()/Scale - RCenter.y());
				}
				else
				{
					p.begin(viewport());
					Transform(currItem, &p);
					Doc->RotMode = 2;
					RCenter = FPoint(currItem->xPos()+currItem->width()/2, currItem->yPos()+currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, true);
					if (QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo))
					{
						if (p.xForm(QRect(static_cast<int>(currItem->width())-6, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
						{
							RCenter = FPoint(currItem->xPos(), currItem->yPos());
							Doc->RotMode = 0;
						}
						if (p.xForm(QRect(0, 0, 6, 6)).intersects(mpo))
						{
							RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
							Doc->RotMode = 4;
						}
						if (p.xForm(QRect(0, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
						{
							RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos(), 0, 0, currItem->rotation(), 1, 1, true);
							Doc->RotMode = 1;
						}
						if (p.xForm(QRect(static_cast<int>(currItem->width())-6, 0, 6, 6)).intersects(mpo))
						{
							RCenter = FPoint(currItem->xPos(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
							Doc->RotMode = 3;
						}
						oldW = xy2Deg(m->x()/Scale - RCenter.x(), m->y()/Scale - RCenter.y());
					}
					p.end();
				}
			}
			break;
		case modeLinkFrames:
			currItem = Doc->ElemToLink;
			if (currItem==NULL)
				break;
			SeleItem(m);
			if (GetItem(&bb))
			{
				if ((bb->itemText.count() == 0) && (bb->NextBox == 0) && (bb->BackBox == 0) && (currItem != bb))
				{
					currItem->NextBox = bb;
					bb->BackBox = currItem;
					if (bb->ItemNr < currItem->ItemNr)
					{
						Doc->Items->insert(currItem->ItemNr+1, bb);
						bb = Doc->Items->take(bb->ItemNr);
					}
					updateContents();
					emit DocChanged();
					Doc->ElemToLink = bb;
				}
				else
					QMessageBox::warning(this, tr("Linking Text Frames"),
											 "<qt>" + tr("You are trying to link to a filled frame, or a frame to itself.") + "</qt>");
			}
			else
				Doc->ElemToLink = NULL;
			break;
		case modeUnlinkFrames:
			SeleItem(m);
			if (GetItem(&currItem))
			{
				if (currItem->BackBox != 0)
				{
					if (currItem->NextBox != 0)
					{
						PageItem *nextItem = currItem->NextBox;
						while (nextItem != 0)
						{
							uint a = nextItem->itemText.count();
							for (uint s=0; s<a; ++s)
								currItem->itemText.append(nextItem->itemText.take(0));
							nextItem = nextItem->NextBox;
						}
					}
					uint a2 = currItem->itemText.count();
					for (uint s = 0; s < a2; ++s)
						currItem->BackBox->itemText.append(currItem->itemText.take(0));
					currItem->BackBox->NextBox = 0;
					currItem->BackBox = 0;
				}
				emit DocChanged();
				updateContents();
			}
			break;
		case modeDrawRegularPolygon:
			{
				selectPage(m);
				z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
				currItem = Doc->Items->at(z);
				FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
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
				qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
				Doc->selection->clear();
				Doc->selection->addItem(currItem);
				currItem->paintObj();
				operItemMoving = true;
				break;
			}
		case modeDrawBezierLine:
			if (m->button() == RightButton)
				break;
			if (FirstPoly)
			{
				selectPage(m);
				z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenLine, !m_MouseButtonPressed);
				currItem = Doc->Items->at(z);
				Doc->selection->clear();
				Doc->selection->addItem(currItem);
				qApp->setOverrideCursor(QCursor(crossCursor), true);
			}
			currItem = Doc->selection->itemAt(0);
			p.begin(viewport());
			Transform(currItem, &p);
			npf = FPoint(p.xFormDev(m->pos()));
			p.end();
			npf += FPoint(Doc->minCanvasCoordinate.x(), Doc->minCanvasCoordinate.y());
			npf = ApplyGridF(npf);
			currItem->PoLine.addPoint(npf);
			npf2 = getMinClipF(&currItem->PoLine);
			if (npf2.x() < 0)
			{
				currItem->PoLine.translate(-npf2.x(), 0);
				MoveItem(npf2.x(), 0, currItem);
			}
			if (npf2.y() < 0)
			{
				currItem->PoLine.translate(0, -npf2.y());
				MoveItem(0, npf2.y(), currItem);
			}
			SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false, false);
			currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2, 1)));
			currItem->paintObj();
			break;
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
			selectPage(m);
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_MouseButtonPressed);
			currItem = Doc->Items->at(z);
			currItem->setIsAnnotation(true);
			switch (Doc->appMode)
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
				currItem->annotation().setZiel(Doc->currentPage->pageNr());
				currItem->annotation().setAction("0 0");
				currItem->setTextFlowsAroundFrame(false);
				break;
			}
			SetupDraw(z);
			break;
		case modeDrawFreehandLine:
			RecordP.resize(0);
			Deselect(false);
			break;
		case modeDrawTable:
			Deselect(false);
			break;
		case modePanning:
			break;
		case modeMeasurementTool:
		case modeEditGradientVectors:
			m_MouseButtonPressed = true;
			qApp->setOverrideCursor(QCursor(CrossCursor), true);
			Dxp = m->x();
			Dyp = m->y();
			Mxp = m->x();
			Myp = m->y();
			break;
		case modeCopyProperties:
			SeleItem(m);
			if (GetItem(&currItem))
			{
				currItem->ColGap = Doc->ElemToLink->ColGap;
				currItem->Cols = Doc->ElemToLink->Cols;
				currItem->BottomLine = Doc->ElemToLink->BottomLine;
				currItem->TopLine = Doc->ElemToLink->TopLine;
				currItem->LeftLine = Doc->ElemToLink->LeftLine;
				currItem->RightLine = Doc->ElemToLink->RightLine;
				currItem->setTextToFrameDist(Doc->ElemToLink->textToFrameDistLeft(), Doc->ElemToLink->textToFrameDistRight(), Doc->ElemToLink->textToFrameDistTop(), Doc->ElemToLink->textToFrameDistBottom());
				currItem->setLineStyle(Doc->ElemToLink->lineStyle());
				currItem->setLineWidth(Doc->ElemToLink->lineWidth());
				currItem->setLineTransparency(Doc->ElemToLink->lineTransparency());
				currItem->setLineShade(Doc->ElemToLink->lineShade());
				currItem->setLineColor(Doc->ElemToLink->lineColor());
				currItem->setLineEnd(Doc->ElemToLink->lineEnd());
				currItem->setLineJoin(Doc->ElemToLink->lineJoin());
				currItem->setCustomLineStyle(Doc->ElemToLink->customLineStyle());
				currItem->setEndArrowIndex(Doc->ElemToLink->endArrowIndex());
				currItem->setStartArrowIndex(Doc->ElemToLink->startArrowIndex());
				currItem->setFillColor(Doc->ElemToLink->fillColor());
				currItem->setFillShade(Doc->ElemToLink->fillShade());
				currItem->setFillTransparency(Doc->ElemToLink->fillTransparency());
				Doc->ElemToLink = currItem;
				emit DocChanged();
				updateContents();
			}
			else
			{
				Doc->ElemToLink = NULL;
				Doc->appMode = modeNormal;
				emit PaintingDone();
			}
			break;
	}
}

//CB-->Doc?
bool ScribusView::ApplyGuides(double *x, double *y)
{
	bool ret = false;
	int pg = Doc->OnPage(*x, *y);
	if (pg == -1)
		return ret;
	Page* page = Doc->Pages->at(pg);
	if (Doc->SnapGuides)
	{
		if (page->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < page->YGuides.count(); ++yg)
			{
				if ((page->YGuides[yg]+page->yOffset() < (*y+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->yOffset() > (*y-Doc->guidesSettings.guideRad)))
				{
					*y= page->YGuides[yg]+page->yOffset();
					ret = true;
					break;
				}
			}
		}
		if (page->XGuides.count() != 0)
		{
			for (uint xg = 0; xg < page->XGuides.count(); ++xg)
			{
				if ((page->XGuides[xg]+page->xOffset() < (*x+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->xOffset() > (*x-Doc->guidesSettings.guideRad)))
				{
					*x = page->XGuides[xg]+page->xOffset();
					ret = true;
					break;
				}
			}
		}
		if ((page->Margins.Left+page->xOffset() < (*x+Doc->guidesSettings.guideRad)) && (page->Margins.Left+page->xOffset() > (*x-Doc->guidesSettings.guideRad)))
		{
			*x = page->Margins.Left+page->xOffset();
			ret = true;
		}
		if (((page->width() - page->Margins.Right)+page->xOffset() < (*x+Doc->guidesSettings.guideRad)) && ((page->width() - page->Margins.Right)+page->xOffset() > (*x-Doc->guidesSettings.guideRad)))
		{
			*x = page->width() - page->Margins.Right+page->xOffset();
			ret = true;
		}
		if ((page->Margins.Top+page->yOffset() < (*y+Doc->guidesSettings.guideRad)) && (page->Margins.Top+page->yOffset() > (*y-Doc->guidesSettings.guideRad)))
		{
			*y = page->Margins.Top+page->yOffset();
			ret = true;
		}
		if (((page->height() - page->Margins.Bottom)+page->yOffset() < (*y+Doc->guidesSettings.guideRad)) && ((page->height() - page->Margins.Bottom)+page->yOffset() > (*y-Doc->guidesSettings.guideRad)))
		{
			*y = page->height() - page->Margins.Bottom+page->yOffset();
			ret = true;
		}
	}
	return ret;
}

//CB-->Doc
void ScribusView::SnapToGuides(PageItem *currItem)
{
	int pg = Doc->OnPage(currItem);
	if (pg == -1)
		return;
	Page* page = Doc->Pages->at(pg);
	if (page->YGuides.count() != 0)
	{
		for (uint yg = 0; yg < page->YGuides.count(); ++yg)
		{
			if ((page->YGuides[yg]+page->yOffset() < (currItem->yPos()+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->yOffset() > (currItem->yPos()-Doc->guidesSettings.guideRad)))
			{
				currItem->setYPos(page->YGuides[yg]+page->yOffset());
				break;
			}
			if (currItem->asLine())
			{
				QWMatrix ma;
				ma.translate(currItem->xPos(), currItem->yPos());
				ma.rotate(currItem->rotation());
				double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
				if ((page->YGuides[yg]+page->yOffset() < (my+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->yOffset() > (my-Doc->guidesSettings.guideRad)))
				{
					currItem->moveBy(0.0, page->YGuides[yg] - my + page->yOffset());
					break;
				}
			}
			else
			{
				if ((page->YGuides[yg]+page->yOffset() < (currItem->yPos()+currItem->height()+Doc->guidesSettings.guideRad)) &&
				     (page->YGuides[yg]+page->yOffset() > ((currItem->yPos()+currItem->height())-Doc->guidesSettings.guideRad)))
				{
					currItem->setYPos(page->YGuides[yg]-currItem->height()+page->yOffset());
					break;
				}
			}
		}
	}
	if (page->XGuides.count() != 0)
	{
		for (uint xg = 0; xg < page->XGuides.count(); ++xg)
		{
			if ((page->XGuides[xg]+page->xOffset() < (currItem->xPos()+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->xOffset() > (currItem->xPos()-Doc->guidesSettings.guideRad)))
			{
				currItem->setXPos(page->XGuides[xg]+page->xOffset());
				break;
			}
			if (currItem->asLine())
			{
				QWMatrix ma;
				ma.translate(currItem->xPos(), currItem->yPos());
				ma.rotate(currItem->rotation());
				double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
				if ((page->XGuides[xg]+page->xOffset() < (mx+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->xOffset() > (mx-Doc->guidesSettings.guideRad)))
				{
					currItem->moveBy(page->XGuides[xg] - mx + page->xOffset(), 0.0);
					break;
				}
			}
			else
			{
				if ((page->XGuides[xg]+page->xOffset() < (currItem->xPos()+currItem->width()+Doc->guidesSettings.guideRad)) &&
				     (page->XGuides[xg]+page->xOffset() > ((currItem->xPos()+currItem->width())-Doc->guidesSettings.guideRad)))
				{
					currItem->setXPos(page->XGuides[xg]-currItem->width()+page->xOffset());
					break;
				}
			}
		}
	}
}

//CB-->Doc
QPoint ScribusView::ApplyGrid(const QPoint& in)
{
	QPoint np;
	int onp = Doc->OnPage(in.x(), in.y());
	if ((Doc->useRaster) && (onp != -1))
	{
		np.setX(static_cast<int>(qRound((in.x() - Doc->Pages->at(onp)->xOffset()) / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid + Doc->Pages->at(onp)->xOffset()));
		np.setY(static_cast<int>(qRound((in.y() - Doc->Pages->at(onp)->yOffset()) / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid + Doc->Pages->at(onp)->yOffset()));
	}
	else
		np = in;
	return np;
}

//CB-->Doc
FPoint ScribusView::ApplyGridF(const FPoint& in)
{
	FPoint np;
	int onp = Doc->OnPage(in.x(), in.y());
	if ((Doc->useRaster) && (onp != -1))
	{
		np.setX(qRound((in.x() - Doc->Pages->at(onp)->xOffset()) / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid + Doc->Pages->at(onp)->xOffset());
		np.setY(qRound((in.y() - Doc->Pages->at(onp)->yOffset()) / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid + Doc->Pages->at(onp)->yOffset());
	}
	else
		np = in;
	return np;
}

//CB-->Doc/selection
void ScribusView::setGroupRect()
{
	PageItem *currItem;
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	uint selectedItemCount=Doc->selection->count();
	for (uint gc = 0; gc < selectedItemCount; ++gc)
	{
		currItem = Doc->selection->itemAt(gc);
		if (currItem->rotation() != 0)
		{
			FPointArray pb(4);
			pb.setPoint(0, FPoint(currItem->xPos(), currItem->yPos()));
			pb.setPoint(1, FPoint(currItem->width(), 0.0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0));
			pb.setPoint(2, FPoint(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0));
			pb.setPoint(3, FPoint(0.0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0));
			for (uint pc = 0; pc < 4; ++pc)
			{
				minx = QMIN(minx, pb.point(pc).x());
				miny = QMIN(miny, pb.point(pc).y());
				maxx = QMAX(maxx, pb.point(pc).x());
				maxy = QMAX(maxy, pb.point(pc).y());
			}
		}
		else
		{
			minx = QMIN(minx, currItem->xPos());
			miny = QMIN(miny, currItem->yPos());
			maxx = QMAX(maxx, currItem->xPos() + currItem->width());
			maxy = QMAX(maxy, currItem->yPos() + currItem->height());
		}
	}
	GroupX = minx;
	GroupY = miny;
	GroupW = maxx - minx;
	GroupH = maxy - miny;
	//GroupSel = true;
}

void ScribusView::getGroupRectScreen(double *x, double *y, double *w, double *h)
{
	QPoint in  = QPoint(qRound(GroupX*Scale), qRound(GroupY*Scale));
	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = GroupW*Scale;
	*h = GroupH*Scale;
}

//CB-->selection?
void ScribusView::getGroupRect(double *x, double *y, double *w, double *h)
{
	*x = GroupX;
	*y = GroupY;
	*w = GroupW;
	*h = GroupH;
}

void ScribusView::paintGroupRect(bool norm)
{
	double x, y, w, h;
	getGroupRectScreen(&x, &y, &w, &h);
	QPainter pgc;
	pgc.begin(viewport());
	pgc.setPen(QPen((norm ? red : black), 1, DotLine, FlatCap, MiterJoin));
	pgc.setBrush(NoBrush);
	pgc.drawRect(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));
	pgc.setBrush(red);
	if (norm)
	{
		pgc.setPen(QPen(red, 1, SolidLine, FlatCap, MiterJoin));
		pgc.drawRect(qRound(x+w-6), qRound(y+h-6), 6, 6);
		pgc.drawRect(qRound(x+w/2 - 3), qRound(y+h-6), 6, 6);
		pgc.drawRect(qRound(x+w/2 - 3), qRound(y), 6, 6);
		pgc.drawRect(qRound(x+w-6), qRound(y+h/2 - 3), 6, 6);
		pgc.drawRect(qRound(x+w-6), qRound(y), 6, 6);
		pgc.drawRect(qRound(x), qRound(y), 6, 6);
		pgc.drawRect(qRound(x), qRound(y+h/2 - 3), 6, 6);
		pgc.drawRect(qRound(x), qRound(y+h-6), 6, 6);
	}
	pgc.end();
}

void ScribusView::PaintSizeRect(QPainter *p, QRect newRect)
{
	static QRect oldRect;
	if (!newRect.isNull())
	{
		QWMatrix ma(p->worldMatrix());
		ma.setTransformationMode ( QWMatrix::Areas );
		p->setWorldMatrix(ma);
		p->setRasterOp(XorROP);
		p->setBrush(NoBrush);
		p->setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		if (!oldRect.isNull())
			p->drawRect(oldRect);
		p->drawRect(newRect);
	}
	oldRect = newRect;
}

void ScribusView::ToView(QPainter *p)
{
	QPoint out(contentsToViewport(QPoint(0, 0)));
	p->translate(qRound(-Doc->minCanvasCoordinate.x()*Scale), qRound(-Doc->minCanvasCoordinate.y()*Scale));
	p->translate(out.x(), out.y());
}

void ScribusView::RefreshItem(PageItem *currItem)
{
	updateContents(currItem->getRedrawBounding(Scale));
}

//CB-->Doc
bool ScribusView::MoveItem(double newX, double newY, PageItem* currItem, bool fromMP)
{
	if (currItem->locked())
		return false;
	bool retw = false;
	double oldx = currItem->xPos();
	double oldy = currItem->yPos();
	currItem->moveBy(newX, newY);
	if ((Doc->useRaster) && (!operItemMoving) && (!fromMP) && (static_cast<int>(Doc->currentPage->pageNr()) == currItem->OwnPage))
	{
		currItem->setXYPos(qRound(currItem->xPos() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid, qRound(currItem->yPos() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
	}
	if ((Doc->SnapGuides) && (!operItemMoving) && (Doc->appMode == modeNormal) && (!Doc->EditClip) && (!fromMP))
		SnapToGuides(currItem);
	if ((currItem->xPos() != oldx) || (currItem->yPos() != oldy))
		retw = true;
	if (!fromMP)
	{
/*		if (GroupSel)
		{
			double gx, gy, gh, gw;
			setGroupRect();
			getGroupRect(&gx, &gy, &gw, &gh);
			emit ItemPos(gx, gy);
		}
		else */
		//CB if (!GroupSel)
		//CB	emit ItemPos(currItem->xPos(), currItem->yPos());
		//CB qDebug("if (!GroupSel) 			emit ItemPos(currItem->xPos(), currItem->yPos());");
	}
/*	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->pageNr(), b->ItemNr); */
	QRect oldR(currItem->getRedrawBounding(Scale));
	Doc->setRedrawBounding(currItem);
	QRect newR(currItem->getRedrawBounding(Scale));
	if ((!operItemMoving) && (!currItem->Sizing))
		updateContents(newR.unite(oldR));
	currItem->OwnPage = Doc->OnPage(currItem);
	return retw;
}

void ScribusView::MarkClip(PageItem *currItem, FPointArray cli, bool once)
{
	double x, y;
	QPainter p;
	QPointArray Bez(4);
	p.begin(viewport());
	ToView(&p);
	Transform(currItem, &p);
	p.scale(0.1, 0.1);
	if (once)
		p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
	else
	{
		p.setRasterOp(XorROP);
		p.setPen(QPen(yellow, 1, SolidLine, FlatCap, MiterJoin));
	}
	p.setBrush(NoBrush);
	if ((EditContour) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
	if (cli.size() > 3)
	{
		for (uint poi=0; poi<cli.size()-3; poi += 4)
		{
			if (cli.point(poi).x() > 900000)
				continue;
			if (once)
				p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
			else
				p.setPen(QPen(yellow, 1, SolidLine, FlatCap, MiterJoin));
			BezierPoints(&Bez,	QPoint(qRound(cli.point(poi).x()*10), qRound(cli.point(poi).y()*10)),
												QPoint(qRound(cli.point(poi+1).x()*10), qRound(cli.point(poi+1).y()*10)),
												QPoint(qRound(cli.point(poi+3).x()*10), qRound(cli.point(poi+3).y()*10)),
												QPoint(qRound(cli.point(poi+2).x()*10), qRound(cli.point(poi+2).y()*10)));
			p.drawCubicBezier(Bez);
			if (once)
				p.setPen(QPen(blue, 1, DotLine, FlatCap, MiterJoin));
			else
				p.setPen(QPen(yellow, 1, DotLine, FlatCap, MiterJoin));
			p.drawLine(Bez.point(0), Bez.point(1));
			p.drawLine(Bez.point(2), Bez.point(3));
		}
	}
	for (uint a=0; a<cli.size()-1; a += 2)
	{
		if (cli.point(a).x() > 900000)
			continue;
		if (EdPoints)
		{
			if (ClRe == static_cast<int>(a+1))
			{
				if (once)
					p.setPen(QPen(red, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(cyan, 8, SolidLine, RoundCap, MiterJoin));
			}
			else
			{
				if (once)
					p.setPen(QPen(magenta, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(green, 8, SolidLine, RoundCap, MiterJoin));
			}
			cli.point(a+1, &x, &y);
			p.drawLine(qRound(x*10), qRound(y*10), qRound(x*10), qRound(y*10));
			if (ClRe == static_cast<int>(a))
			{
				if (once)
					p.setPen(QPen(red, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(cyan, 8, SolidLine, RoundCap, MiterJoin));
			}
			else
			{
				if (once)
					p.setPen(QPen(blue, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(yellow, 8, SolidLine, RoundCap, MiterJoin));
			}
			cli.point(a, &x, &y);
			p.drawLine(qRound(x*10), qRound(y*10), qRound(x*10), qRound(y*10));
		}
		else
		{
			if (ClRe == static_cast<int>(a))
			{
				if (once)
					p.setPen(QPen(red, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(cyan, 8, SolidLine, RoundCap, MiterJoin));
			}
			else
			{
				if (once)
					p.setPen(QPen(blue, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(yellow, 8, SolidLine, RoundCap, MiterJoin));
			}
			cli.point(a, &x, &y);
			p.drawLine(qRound(x*10), qRound(y*10), qRound(x*10), qRound(y*10));
			if (ClRe == static_cast<int>(a+1))
			{
				if (once)
					p.setPen(QPen(red, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(cyan, 8, SolidLine, RoundCap, MiterJoin));
			}
			else
			{
				if (once)
					p.setPen(QPen(magenta, 8, SolidLine, RoundCap, MiterJoin));
				else
					p.setPen(QPen(green, 8, SolidLine, RoundCap, MiterJoin));
			}
			cli.point(a+1, &x, &y);
			p.drawLine(qRound(x*10), qRound(y*10), qRound(x*10), qRound(y*10));
		}
	}
	if (ClRe != -1)
	{
		if (once)
		{
			p.setPen(QPen(red, 8, SolidLine, RoundCap, MiterJoin));
//		else
//			p.setPen(QPen(cyan, 8, SolidLine, RoundCap, MiterJoin));
			cli.point(ClRe, &x, &y);
			p.drawLine(qRound(x*10), qRound(y*10), qRound(x*10), qRound(y*10));
			QValueList<int>::Iterator itm;
			for (itm = SelNode.begin(); itm != SelNode.end(); ++itm)
			{
				cli.point((*itm), &x, &y);
				p.drawLine(qRound(x*10), qRound(y*10), qRound(x*10), qRound(y*10));
			}
		}
		emit HavePoint(true, MoveSym);
	}
	else
		emit HavePoint(false, MoveSym);
	p.end();
}

//CB-->elsewhere, util?
bool ScribusView::PointOnLine(QPoint Start, QPoint Ende, QRect MArea)
{
	QPoint an, en;
	if (Start.x() == Ende.x())
	{
		an = Start.y() > Ende.y() ? Ende : Start;
		en = an == Ende ? Start : Ende;
		for (int a=an.y(); a<en.y(); ++a)
		{
			if (MArea.contains(QPoint(an.x(), a)))
				return true;
		}
	}
	if (Start.y() == Ende.y())
	{
		an = Start.x() > Ende.x() ? Ende : Start;
		en = an == Ende ? Start : Ende;
		for (int a=an.x(); a<en.x(); ++a)
		{
			if (MArea.contains(QPoint(a, an.y())))
				return true;
		}
	}
	an = Start.x() > Ende.x() ? Ende : Start;
	en = an == Ende ? Start : Ende;
	double stg = (en.y() - an.y()) / static_cast<double>((en.x() - an.x()));
	for (int a = an.x(); a < en.x(); ++a)
	{
		if (MArea.contains(QPoint(a, an.y()+qRound((a-an.x())*stg))))
			return true;
	}
	return false;
}

//CB-->Doc??
void ScribusView::TransformPoly(int mode, int rot, double scaling)
{
	PageItem *currItem = Doc->selection->itemAt(0);
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (EditContour)
	{
		FPoint tp2(getMinClipF(&currItem->ContourLine));
		FPoint tp(getMaxClipF(&currItem->ContourLine));
		currItem->ContourLine.translate(-qRound((tp.x() + tp2.x()) / 2.0), -qRound((tp.y() + tp2.y()) / 2.0));
		switch (mode)
		{
		case 0:
			ma.rotate(-rot);
			break;
		case 1:
			ma.rotate(rot);
			break;
		case 2:
			ma.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
			break;
		case 3:
			ma.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
			break;
		case 4:
			ma.shear(0.017455, 0);
			break;
		case 5:
			ma.shear(-0.017455, 0);
			break;
		case 6:
			ma.shear(0, -0.017455);
			break;
		case 7:
			ma.shear(0, 0.017455);
			break;
		case 8:
			ma.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
			break;
		case 9:
			ma.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
			break;
		}
		currItem->ContourLine.map(ma);
		currItem->ContourLine.translate(qRound((tp.x() + tp2.x()) / 2.0), qRound((tp.y() + tp2.y()) / 2.0));
		updateContents();
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		MarkClip(currItem, currItem->ContourLine, true);
		if (UndoManager::undoEnabled())
		{
			undoManager->setUndoEnabled(false);
			currItem->checkChanges(true);
			undoManager->setUndoEnabled(true);
			SimpleState *ss = new SimpleState(Um::EditContourLine, "", Um::IBorder);
			ss->set("EDIT_CONTOUR", "edit_contour");
			ss->set("MODE", mode);
			ss->set("ROT", rot);
			ss->set("SCALING", scaling);
			undoManager->action(currItem, ss);
		}
		emit DocChanged();
		return;
	}
	FPoint oldPos(currItem->xyPos());
	double offsX = currItem->width() / 2.0;
	double offsY = currItem->height() / 2.0;
	ma.translate(-offsX, -offsY);
	switch (mode)
	{
	case 0:
		ma.rotate(-rot);
		break;
	case 1:
		ma.rotate(rot);
		break;
	case 2:
		ma.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
		break;
	case 3:
		ma.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
		break;
	case 4:
		ma.shear(0.017455, 0);
		break;
	case 5:
		ma.shear(-0.017455, 0);
		break;
	case 6:
		ma.shear(0, -0.017455);
		break;
	case 7:
		ma.shear(0, 0.017455);
		break;
	case 8:
		ma.scale(1.0 - (scaling / currItem->width()),1.0 - (scaling / currItem->height()));
		break;
	case 9:
		ma.scale(1.0 + (scaling / currItem->width()),1.0 + (scaling / currItem->height()));
		break;
	}
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(offsX, offsY);
	AdjustItemSize(currItem);
	QWMatrix ma2;
	ma2.translate(oldPos.x(), oldPos.y());
	ma2.scale(1, 1);
	ma2.translate(offsX, offsY);
	FPoint n(-offsX, -offsY);
	switch (mode)
	{
	case 0:
		ma2.rotate(-rot);
		break;
	case 1:
		ma2.rotate(rot);
		break;
	case 2:
		ma2.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
		break;
	case 3:
		ma2.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
		break;
	case 4:
		ma2.shear(0.017455, 0);
		break;
	case 5:
		ma2.shear(-0.017455, 0);
		break;
	case 6:
		ma2.shear(0, -0.017455);
		break;
	case 7:
		ma2.shear(0, 0.017455);
		break;
	case 8:
		ma2.scale(1.0 - (scaling / currItem->width()),1.0 - (scaling / currItem->height()));
		break;
	case 9:
		ma2.scale(1.0 + (scaling / currItem->width()),1.0 + (scaling / currItem->height()));
		break;
	}
	double x = ma2.m11() * n.x() + ma2.m21() * n.y() + ma2.dx();
	double y = ma2.m22() * n.y() + ma2.m12() * n.x() + ma2.dy();
	MoveItem(x-oldPos.x(), y-oldPos.y(), currItem);
	if (currItem->asPathText())
		currItem->updatePolyClip();
	Doc->setRedrawBounding(currItem);
	RefreshItem(currItem);
	MarkClip(currItem, currItem->PoLine, true);
	currItem->FrameType = 3;
	if (UndoManager::undoEnabled())
	{
		undoManager->setUndoEnabled(false);
		currItem->checkChanges(true);
		undoManager->setUndoEnabled(true);
		SimpleState *ss = new SimpleState(Um::EditShape, "", Um::IBorder);
		ss->set("EDIT_SHAPE", "edit_shape");
		ss->set("MODE", mode);
		ss->set("ROT", rot);
		ss->set("SCALING", scaling);
		undoManager->action(currItem, ss);
	}
	emit DocChanged();
}

void ScribusView::Reset1Control()
{
	PageItem *currItem = Doc->selection->itemAt(0);
	currItem->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = currItem->ContourLine.point(ClRe-1);
	else
		np = currItem->PoLine.point(ClRe-1);
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if (EditContour)
	{
		currItem->ContourLine.setPoint(ClRe, np);
		updateContents();
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
	}
	else
	{
		currItem->PoLine.setPoint(ClRe, np);
		AdjustItemSize(currItem);
	}
	FPointArray cli;
	if ((EditContour) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
	MarkClip(currItem, cli, true);
	emit DocChanged();
}

void ScribusView::ResetControl()
{
	PageItem *currItem = Doc->selection->itemAt(0);
	currItem->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = currItem->ContourLine.point(ClRe);
	else
		np = currItem->PoLine.point(ClRe);
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if ((ClRe == 0) || (ClRe == static_cast<int>(currItem->PoLine.size()-2)))
	{
		if (EditContour)
			currItem->ContourLine.setPoint(ClRe+1, np);
		else
			currItem->PoLine.setPoint(ClRe+1, np);
	}
	else
	{
		if (EditContour)
		{
			currItem->ContourLine.setPoint(ClRe+1, np);
			currItem->ContourLine.setPoint((ClRe % 4 != 0 ? ClRe + 3 : ClRe - 1), np);
		}
		else
		{
			currItem->PoLine.setPoint(ClRe+1, np);
			currItem->PoLine.setPoint((ClRe % 4 != 0 ? ClRe + 3 : ClRe - 1), np);
		}
	}
	if (!EditContour)
		AdjustItemSize(currItem);
	else
	{
		updateContents();
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
	}
	FPointArray cli;
	if ((EditContour) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
	MarkClip(currItem, cli, true);
	emit DocChanged();
}

//CB-->Doc
void ScribusView::MoveClipPoint(PageItem *currItem, FPoint ip)
{
	if (((EdPoints) && (ClRe % 2 != 0)) || ((!EdPoints) && (ClRe % 2 == 0)))
		return;
	currItem->ClipEdited = true;
	FPointArray Clip;
	if (EditContour)
		Clip = currItem->ContourLine.copy();
	else
		Clip = currItem->PoLine.copy();
	currItem->FrameType = 3;
	uint EndInd = Clip.size();
	uint StartInd = 0;
	for (uint n = ClRe; n < Clip.size(); ++n)
	{
		if (Clip.point(n).x() > 900000)
		{
			EndInd = n;
			break;
		}
	}
	if (ClRe > 0)
	{
		for (uint n2 = ClRe; n2 > 0; n2--)
		{
			if (n2 == 0)
				break;
			if (Clip.point(n2).x() > 900000)
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}
	FPoint np(ip);
	if (ClRe != -1)
	{
		if ((np.x() < 0) && (!EditContour))
		{
			SizeItem(currItem->width() - np.x(), currItem->height(), currItem->ItemNr, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(np.x(), 0);
				MoveRotated(currItem, npv);
			}
			else
				MoveItem(np.x(), 0, currItem);
			Clip.translate(-np.x(), 0);
			if (!currItem->imageFlippedH())
				currItem->moveImageInFrame(-np.x()/currItem->imageXScale(), 0);
			np.setX(0);
		}
		if ((np.y() < 0) && (!EditContour))
		{
			SizeItem(currItem->width(), currItem->height() - np.y(), currItem->ItemNr, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(0, np.y());
				MoveRotated(currItem, npv);
			}
			else
				MoveItem(0, np.y(), currItem);
			Clip.translate(0, -np.y());
			if (!currItem->imageFlippedV())
				currItem->moveImageInFrame(0, -np.y()/currItem->imageYScale());
			np.setY(0);
		}
		emit ClipPo(np.x(), np.y());
		if ((ClRe+1 < static_cast<int>(EndInd)) && (ClRe % 2 == 0))
		{
			FPoint ap(Clip.point(ClRe));
			FPoint ap2(Clip.point(ClRe+1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+1, ap2);
		}
		Clip.setPoint(ClRe, np);
		if (((ClRe % 4 != 0) && (ClRe % 2 == 0)) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(ClRe+2));
			FPoint ap2(Clip.point(ClRe+3));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+3, ap2);
			Clip.setPoint(ClRe+2, np);
		}
		if ((ClRe % 4 == 0) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(ClRe-2));
			FPoint ap2(Clip.point(ClRe-1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe-1, ap2);
			Clip.setPoint(ClRe-2, np);
		}
		/*
		{
			qDebug(QString("is a text %1").arg((currItem->asTextFrame()!=0)));
			qDebug(QString("is a image %1").arg((currItem->asImageFrame()!=0)));
			qDebug(QString("is a line %1").arg((currItem->asLine()!=0)));
			qDebug(QString("is a pathtext %1").arg((currItem->asPathText()!=0)));
			qDebug(QString("is a polygon %1").arg((currItem->asPolygon()!=0)));
			qDebug(QString("is a polyline %1").arg((currItem->asPolyLine()!=0)));

			qDebug(QString("item type is %1").arg(currItem->itemType()));
		}
		*/
		if (((ClRe == static_cast<int>(StartInd)) || (ClRe == static_cast<int>(EndInd-2))) &&
//		        ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame())))
	((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)))
		{
			if (ClRe == static_cast<int>(StartInd))
			{
				FPoint ap(Clip.point(EndInd-2));
				FPoint ap2(Clip.point(EndInd-1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(EndInd-2, Clip.point(StartInd));
				Clip.setPoint(EndInd-1, ap2);
			}
			else
			{
				FPoint ap(Clip.point(StartInd));
				FPoint ap2(Clip.point(StartInd + 1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(StartInd, Clip.point(EndInd-2));
				Clip.setPoint(StartInd + 1, ap2);
			}
		}
		if (((ClRe == static_cast<int>(StartInd+1)) || (ClRe == static_cast<int>(EndInd-1))) &&
//		        ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame())) && (MoveSym))
((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)) && (MoveSym))
		{
			uint kon = 0;
			if (ClRe == static_cast<int>(StartInd+1))
				kon = EndInd-1;
			else
				kon = StartInd + 1;
			FPoint lxy(Clip.point(ClRe-1));
			FPoint lk(Clip.point(ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if ((ClRe % 2 != 0) && (MoveSym) && (ClRe != static_cast<int>(StartInd + 1)) && (ClRe != static_cast<int>(EndInd-1)))
		{
			uint kon = 0;
			if ((ClRe+1) % 4 == 0)
				kon = ClRe + 2;
			else
				kon = ClRe - 2;
			FPoint lxy(Clip.point(ClRe-1));
			FPoint lk(Clip.point(ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if (EditContour)
		{
			MarkClip(currItem, currItem->ContourLine);
			currItem->ContourLine = Clip.copy();
		}
		else
		{
			MarkClip(currItem, currItem->PoLine);
			currItem->PoLine = Clip.copy();
		}
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
		MarkClip(currItem, Clip);
	}
}

//CB-->Doc
bool ScribusView::SizeItem(double newX, double newY, int ite, bool fromMP, bool DoUpdateClip, bool redraw)
{
	return SizeItem(newX, newY, Doc->Items->at(ite), fromMP, DoUpdateClip, redraw);
}

//CB-->Doc
bool ScribusView::SizeItem(double newX, double newY, PageItem *pi, bool fromMP, bool DoUpdateClip, bool redraw)
{
	PageItem *currItem = pi;
	if (currItem->locked())
		return false;
	QPainter p;
	QRect oldR(currItem->getRedrawBounding(Scale));
	//Uncomment for stopping resize in any direction
	//and remove the height/width <0 tests in item sizing switch
	/*
	if (!currItem->asLine())
	{
		newX = QMAX(newX, 1);
		newY = QMAX(newY, 1);
	}
	*/
	int ph = static_cast<int>(QMAX(1.0, currItem->lineWidth() / 2.0));
	QWMatrix ma;
	ma.rotate(currItem->rotation());
	double dX = ma.m11() * (currItem->width() - newX) + ma.m21() * (currItem->height() - newY) + ma.dx();
	double dY = ma.m22() * (currItem->height() - newY) + ma.m12() * (currItem->width() - newX) + ma.dy();
	currItem->setWidthHeight(newX, newY);
	if ((Doc->RotMode != 0) && (fromMP) && (!Doc->isLoading()) && (Doc->appMode == modeNormal))
	{
		double moveX=dX, moveY=dY;
		if (Doc->RotMode==2)
		{
			moveX/=2.0;
			moveY/=2.0;
		}
		else if (Doc->RotMode==3)
			moveX=0.0;
		else if (Doc->RotMode==1)
			moveY=0.0;
		MoveItem(moveX, moveY, currItem);
	}
	currItem->setCornerRadius(QMIN(currItem->cornerRadius(), QMIN(currItem->width(),currItem->height())/2));
	if ((currItem->asImageFrame()) && (!currItem->Sizing) && (!Doc->EditClip))
	{
		currItem->AdjustPictScale();
	}
	if (currItem->asLine())
	{
		if (!fromMP)
		{
			currItem->setRotation(atan2(currItem->height(),currItem->width())*(180.0/M_PI));
			currItem->setWidthHeight(sqrt(pow(currItem->width(),2)+pow(currItem->height(),2)), 1.0);
		}
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
	}
	Doc->setRedrawBounding(currItem);
	currItem->OwnPage = Doc->OnPage(currItem);
	if (currItem->Sizing)
	{
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		if ((frameResizeHandle == 1) && !(currItem->asLine()))
			currItem->paintObj();
		if ((currItem->FrameType == 0) || (currItem->asLine()) || (frameResizeHandle != 1))
			return true;
		QPainter p;
		p.begin(viewport());
		QPoint in  = QPoint(qRound((currItem->xPos()-Doc->minCanvasCoordinate.x())*Scale), qRound((currItem->yPos()-Doc->minCanvasCoordinate.y())*Scale));
		QPoint out = contentsToViewport(in);
		p.translate(out.x(), out.y());
		p.scale(Scale, Scale);
		p.rotate(currItem->rotation());
		p.setRasterOp(XorROP);
		p.setBrush(NoBrush);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		currItem->DrawPolyL(&p, currItem->Clip);
		currItem->updateClip();
		currItem->updateGradientVectors();
		currItem->DrawPolyL(&p, currItem->Clip);
		p.end();
		return true;
	}
	if (DoUpdateClip)
	{
		double nX=0.0, nY=0.0;
		if (fromMP)
		{
			if (currItem->imageFlippedH())
				nX=-(currItem->width() - currItem->OldB2)/currItem->imageXScale();
			if (currItem->imageFlippedV())
				nY=-(currItem->height() - currItem->OldH2)/currItem->imageYScale();
		}
		else
		{
			if (!currItem->imageFlippedH())
				nX=(currItem->width() - currItem->OldB2)/currItem->imageXScale();
			if (!currItem->imageFlippedV())
				nY=(currItem->height() - currItem->OldH2)/currItem->imageYScale();
		}
		if (nX!=0.0 || nY!=0.0)
			currItem->moveImageInFrame(dX,dY);
		currItem->updateClip();
	}
	currItem->updateGradientVectors();
	if (redraw)
	{
		QRect newR(currItem->getRedrawBounding(Scale));
		updateContents(newR.unite(oldR));
	}
	if (!fromMP)
	{
		if (Doc->selection->isMultipleSelection())
		{
			double gx, gy, gh, gw;
			setGroupRect();
			getGroupRect(&gx, &gy, &gw, &gh);
			emit ItemGeom(gw, gh);
		}
	}
	return true;
}

//CB-->Doc
void ScribusView::MoveRotated(PageItem *currItem, FPoint npv, bool fromMP)
{
	QWMatrix ma;
	ma.translate(currItem->xPos(), currItem->yPos());
	ma.rotate(currItem->rotation());
	double mxc = currItem->xPos() - (ma.m11() * npv.x() + ma.m21() * npv.y() + ma.dx());
	double myc = currItem->yPos() - (ma.m22() * npv.y() + ma.m12() * npv.x() + ma.dy());
	MoveItem(-mxc, -myc, currItem, fromMP);
}

//CB-->Doc
bool ScribusView::MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP, bool constrainRotation)
{
	PageItem *currItem = Doc->Items->at(ite);
	QRect oldR(currItem->getRedrawBounding(Scale));
	if (currItem->asLine())
	{
		QWMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.dx();
		double my = ma.m12() * currItem->width() + ma.dy();
		MoveItem(newX.x(), newX.y(), currItem, fromMP);
		double newRot=xy2Deg(mx - currItem->xPos(), my - currItem->yPos());
		//CB Hmm should work, doesnt. (constraining on the first point of a line)
		//if (constrainRotation)
		//	qDebug(QString("%1").arg(constrainAngle(newRot)));
		currItem->setRotation(newRot);
		currItem->setWidthHeight(sqrt(pow(mx - currItem->xPos(),2)+pow(my - currItem->yPos(),2)), 1.0);
		currItem->updateClip();
		Doc->setRedrawBounding(currItem);
		QRect newR(currItem->getRedrawBounding(Scale));
		updateContents(newR.unite(oldR));
	}
	else
	{
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		if (currItem->rotation() != 0)
		{
			FPoint npv(newX.x(), newX.y());
			QWMatrix ma3;
			ma3.translate(currItem->xPos(), currItem->yPos());
			ma3.rotate(currItem->rotation());
			double mxc3 = currItem->xPos() - (ma3.m11() * npv.x() + ma3.m21() * npv.y() + ma3.dx());
			double myc3 = currItem->yPos() - (ma3.m22() * npv.y() + ma3.m12() * npv.x() + ma3.dy());
			SizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), ite, fromMP, true, false);
			MoveItem(-mxc3, -myc3, currItem, fromMP);
		}
		else
		{
			SizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), ite, fromMP, true, false);
			MoveItem(newX.x(), newX.y(), currItem, fromMP);
		}
	}
	return true;
}

//CB-->Doc
void ScribusView::moveGroup(double x, double y, bool fromMP)
{
	uint docSelectionCount=Doc->selection->count();
	if (!_groupTransactionStarted && docSelectionCount > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < docSelectionCount; ++i)
			tooltip += "\t" + Doc->selection->itemAt(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Move, tooltip, Um::IMove);
		_groupTransactionStarted = true;
	}
	PageItem* currItem;
	QPainter p;
	double gx, gy, gw, gh;
	double sc = Scale;
	for (uint a = 0; a < docSelectionCount; ++a)
	{
		currItem = Doc->selection->itemAt(a);
		if ((!fromMP) && (docSelectionCount < moveWithBoxesOnlyThreshold))
		{
			p.begin(viewport());
			ToView(&p);
			p.translate(qRound(currItem->xPos()*Scale), qRound(currItem->yPos()*sc));
			p.scale(sc, sc);
			p.rotate(currItem->rotation());
			p.setRasterOp(XorROP);
			p.setBrush(NoBrush);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (docSelectionCount < moveWithFullOutlinesThreshold)
			{
				if (!(currItem->asLine()) && (currItem->FrameType != 0) || (currItem->asPolyLine()))
					currItem->DrawPolyL(&p, currItem->Clip);
			}
			else
				p.drawRect(0, 0, static_cast<int>(currItem->width())+1, static_cast<int>(currItem->height())+1);
			p.end();
		}
		MoveItem(x, y, currItem, fromMP);
		if ((!fromMP) && (Doc->selection->count() < moveWithBoxesOnlyThreshold))
		{
			p.begin(viewport());
			ToView(&p);
			p.translate(qRound(currItem->xPos()*sc), qRound(currItem->yPos()*sc));
			p.scale(sc, sc);
			p.rotate(currItem->rotation());
			p.setRasterOp(XorROP);
			p.setBrush(NoBrush);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (Doc->selection->count() < moveWithFullOutlinesThreshold)
			{
				if (!(currItem->asLine()) && (currItem->FrameType != 0) || (currItem->asPolyLine()))
					currItem->DrawPolyL(&p, currItem->Clip);
			}
			else
				p.drawRect(0, 0, static_cast<int>(currItem->width())+1, static_cast<int>(currItem->height())+1);
			p.end();
		}
	}
	if (Doc->selection->isMultipleSelection())
	{
			setGroupRect();
			getGroupRect(&gx, &gy, &gw, &gh);
			emit ItemPos(gx, gy);
		currItem = Doc->selection->itemAt(0);
		Doc->GroupOnPage(currItem);
		if (fromMP)
		{
			gx -= Doc->minCanvasCoordinate.x();
			gy -= Doc->minCanvasCoordinate.y();
			updateContents(QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10)));
		}

		getGroupRectScreen(&gx, &gy, &gw, &gh);
		p.begin(viewport());
		PaintSizeRect(&p, QRect(qRound(gx), qRound(gy), qRound(gw), qRound(gh)));
		p.end();
	}
	else
	{
		//Paint the drag moved item, ie the black dashed line representing the frame
		currItem = Doc->selection->itemAt(0);
		QRect oldR = QRect(qRound(currItem->BoundingX * Scale), qRound(currItem->BoundingY * Scale), qRound(currItem->BoundingW * Scale), qRound(currItem->BoundingH * Scale));
		//CB this breaks dragging an item when the canvas has been push resized
		//oldR.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
		p.begin(viewport());
		ToView(&p);
		PaintSizeRect(&p, oldR);
		p.end();
	}
}

//CB-->Doc
void ScribusView::RotateGroup(double win)
{
	if (!_groupTransactionStarted && Doc->selection->count() > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < Doc->selection->count(); ++i)
			tooltip += "\t" + Doc->selection->itemAt(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Rotate, tooltip, Um::IRotate);
		_groupTransactionStarted = true;
	}
	double gxS, gyS, ghS, gwS;
	double sc = Scale;
	PageItem* currItem;
	getGroupRect(&gxS, &gyS, &gwS, &ghS);
	QWMatrix ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(win);
	gxS -= Doc->minCanvasCoordinate.x();
	gyS -= Doc->minCanvasCoordinate.y();
	QRect oldR = QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10));
	FPoint n;
	for (uint a = 0; a < Doc->selection->count(); ++a)
	{
		currItem = Doc->selection->itemAt(a);
		n = FPoint(currItem->xPos() - RCenter.x(), currItem->yPos() - RCenter.y());
		currItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
		currItem->rotateBy(win);
		Doc->setRedrawBounding(currItem);
	}
	currItem = Doc->selection->itemAt(0);
	Doc->GroupOnPage(currItem);
	setGroupRect();
	getGroupRect(&gxS, &gyS, &gwS, &ghS);
	gxS -= Doc->minCanvasCoordinate.x();
	gyS -= Doc->minCanvasCoordinate.y();
	updateContents(QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10)).unite(oldR));
}

//CB-->Doc
void ScribusView::scaleGroup(double scx, double scy, bool scaleText)
{
	uint docSelectionCount=Doc->selection->count();
	if (!_groupTransactionStarted && docSelectionCount > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < docSelectionCount; ++i)
			tooltip += "\t" + Doc->selection->itemAt(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Resize, tooltip, Um::IResize);
		_groupTransactionStarted = true;
	}
	PageItem *bb;
	double gx, gy, gh, gw, x, y;
	uint aa;
	double sc = Scale;

	getGroupRect(&gx, &gy, &gw, &gh);
	gx -= Doc->minCanvasCoordinate.x();
	gy -= Doc->minCanvasCoordinate.y();
	QRect oldR = QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10));
	getGroupRect(&gx, &gy, &gw, &gh);
	double origGW = gw;
	double origGH = gh;
	setUpdatesEnabled(false);
	uint selectedItemCount=Doc->selection->count();
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		bb = Doc->selection->itemAt(a);
		if ((bb->locked()) || (bb->sizeLocked()))
			continue;
		bb->OldB = bb->width();
		bb->OldH = bb->height();
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		bb->Sizing = false;
		double oldRot, oldLocalX, oldLocalY;
		oldRot = bb->rotation();
		oldLocalX = bb->imageXOffset();
		oldLocalY = bb->imageYOffset();
		FPointArray gr;
		gr.addPoint(bb->GrStartX, bb->GrStartY);
		gr.addPoint(bb->GrEndX, bb->GrEndY);
		FPoint g(gx, gy);
		FPoint b(0, 0, bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		b -= g;
		FPoint b1(b.x(), b.y(), 0, 0, 0, scx, scy);
		FPoint t(bb->width(), 0, bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		t -= g;
		FPoint t1(t.x(), t.y(), 0, 0, 0, scx, scy);
		FPoint h(0, bb->height(), bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		h -= g;
		FPoint h1(h.x(), h.y(), 0, 0, 0, scx, scy);
		bb->setLineWidth(QMAX(bb->lineWidth()*((scx+scy)/2), 0.01));
		if (bb->itemType() == PageItem::Line)
		{
			bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
			bb->setWidth(sqrt(pow(t1.x()-b1.x(),2)+pow(t1.y()-b1.y(),2)));
			bb->setXYPos(b1.x()+gx, b1.y()+gy);
		}
		else
		{
			FPoint oldPos(bb->xPos(), bb->yPos());
			QWMatrix ma;
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QWMatrix ma2;
			ma2.translate(gx-bb->xPos(), gy-bb->yPos());
			ma2.scale(scx, scy);
			bb->PoLine.map(ma2);
			bb->setRotation(0.0);
			bb->ClipEdited = true;
			AdjustItemSize(bb);
			QWMatrix ma3;
			ma3.translate(gx, gy);
			ma3.scale(scx, scy);
			FPoint n(gx-oldPos.x(), gy-oldPos.y());
			x = ma3.m11() * n.x() + ma3.m21() * n.y() + ma3.dx();
			y = ma3.m22() * n.y() + ma3.m12() * n.x() + ma3.dy();
			MoveItem(gx-x, gy-y, bb, true);
			if (oldRot != 0)
			{
				bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
				QWMatrix ma;
				ma.rotate(-bb->rotation());
				bb->PoLine.map(ma);
				AdjustItemSize(bb);
			}
		}
		if (scaleText)
		{
			bb->setFontSize(QMAX(qRound(bb->fontSize()*((scx+scy)/2)), 1));
			if ((bb->itemText.count() != 0) && (!bb->isTableItem))
			{
				bb->setLineSpacing(((bb->fontSize() / 10.0) * static_cast<double>(Doc->typographicSettings.autoLineSpacing) / 100) + (bb->fontSize() / 10.0));
				for (aa = 0; aa < bb->itemText.count(); ++aa)
					bb->itemText.at(aa)->csize = QMAX(qRound(bb->itemText.at(aa)->csize*((scx+scy)/2)), 1);
				if (bb->asPathText())
					bb->updatePolyClip();
			}
		}
		bb->setImageXYOffset(oldLocalX, oldLocalY);
		double dX = bb->width() - bb->OldB2;
		double dY = bb->height() - bb->OldH2;
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		QWMatrix ma4;
		ma4.rotate(oldRot);
		ma4.scale(scx, scy);
		gr.map(ma4);
		bb->ContourLine.map(ma4);
		bb->GrStartX = gr.point(0).x();
		bb->GrStartY = gr.point(0).y();
		bb->GrEndX = gr.point(1).x();
		bb->GrEndY = gr.point(1).y();
		bb->updateGradientVectors();
		if ((Doc->RotMode != 0) && (!Doc->isLoading()))
		{
			switch (Doc->RotMode)
			{
			case 2:
				MoveItem(dX / 2.0, dY / 2.0, bb);
				break;
			case 4:
				MoveItem(dX, dY, bb);
				break;
			case 3:
				MoveItem(0.0, dY, bb);
				break;
			case 1:
				MoveItem(dX, 0.0, bb);
				break;
			}
		}
	}
	bb = Doc->selection->itemAt(0);
	Doc->GroupOnPage(bb);
	setGroupRect();
	getGroupRect(&gx, &gy, &gw, &gh);
	if ((Doc->RotMode != 0) && (!Doc->isLoading()))
	{
		switch (Doc->RotMode)
		{
		case 2:
			moveGroup((origGW-gw) / 2.0, (origGH-gh) / 2.0, true);
			break;
		case 4:
			moveGroup(origGW-gw, origGH-gh, true);
			break;
		case 3:
			moveGroup(0.0, origGH-gh, true);
			break;
		case 1:
			moveGroup(origGW-gw, 0.0, true);
			break;
		}
	}
	gx -= Doc->minCanvasCoordinate.x();
	gy -= Doc->minCanvasCoordinate.y();
	setUpdatesEnabled(true);
	updateContents(QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10)).unite(oldR));
	emit DocChanged();
}

//CB-->Doc
void ScribusView::RotateItem(double win, int ite)
{
	RotateItem(win, Doc->Items->at(ite));
}

//CB-->Doc
void ScribusView::RotateItem(double win, PageItem *currItem)
{
	if (currItem->locked())
		return;
	QRect oldR(currItem->getRedrawBounding(Scale));
	if ((Doc->RotMode != 0) && !(currItem->asLine()))
	{
		QWMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.scale(1, 1);
		ma.rotate(currItem->rotation());
		double ro = win-currItem->rotation();
		currItem->setRotation(win);
		FPoint n;
		switch (Doc->RotMode)
		{
		case 2:
			ma.translate(currItem->width()/2, currItem->height()/2);
			n = FPoint(-currItem->width()/2, -currItem->height()/2);
			break;
		case 4:
			ma.translate(currItem->width(), currItem->height());
			n = FPoint(-currItem->width(), -currItem->height());
			break;
		case 3:
			ma.translate(0, currItem->height());
			n = FPoint(0, -currItem->height());
			break;
		case 1:
			ma.translate(currItem->width(), 0);
			n = FPoint(-currItem->width(), 0);
			break;
		}
		ma.rotate(ro);
		double x = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		double y = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		bool oldS = currItem->Sizing;
		currItem->Sizing = true;
		MoveItem(x-currItem->xPos(), y-currItem->yPos(), currItem);
		currItem->Sizing = oldS;
	}
	else
	{
		currItem->setRotation(win);
		Doc->setRedrawBounding(currItem);
	}
	QRect newR(currItem->getRedrawBounding(Scale));
	updateContents(newR.unite(oldR));
	//emit SetAngle(currItem->rotation());
}

//CB-->Doc
void ScribusView::AdjustItemSize(PageItem *currItem)
{
	bool siz = currItem->Sizing;
	currItem->Sizing = false;
	FPointArray Clip;
	Clip = currItem->PoLine;
	FPoint tp2(getMinClipF(&Clip));
	SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem, true, false, false);
	Clip.translate(-tp2.x(), -tp2.y());
	if (currItem->rotation() != 0)
	{
		FPoint npv(tp2.x(), tp2.y());
		MoveRotated(currItem, npv, true);
	}
	else
		MoveItem(tp2.x(), tp2.y(), currItem, true);
	if (!currItem->imageFlippedH())
		currItem->moveImageInFrame(-tp2.x()/currItem->imageXScale(), 0);
	if (!currItem->imageFlippedV())
		currItem->moveImageInFrame(0, -tp2.y()/currItem->imageYScale());
	FPoint tp(getMaxClipF(&Clip));
	if (currItem->imageFlippedH())
		currItem->moveImageInFrame((currItem->width() - tp.x())/currItem->imageXScale(), 0);
	if (currItem->imageFlippedV())
		currItem->moveImageInFrame(0, (currItem->height() - tp.y())/currItem->imageYScale());
	SizeItem(tp.x(), tp.y(), currItem, true, false);
	currItem->ClipEdited = true;
	currItem->PoLine = Clip.copy();
	if (currItem->asPolyLine())
		currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2, 1)));
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->Sizing = siz;
}

/*/CB Unused
void ScribusView::AdvanceSel(PageItem *currItem, int oldPos, int len, int dir, int expandSel, int state)
{
	int i;
	bool isSpace;
	if ( dir > 0 )
	{
		isSpace = currItem->itemText.at(oldPos)->ch.at(0).isSpace();
		currItem->CPos = oldPos +1;
		for (i=oldPos+1; i < len; i++)
		{
			if ( currItem->itemText.at(i)->ch.at(0).isSpace() == isSpace )
			{
				if ( expandSel )
				currItem->itemText.at(i)->cselect = state;
			}
			else
				break;
			currItem->CPos++;
		}
	}
	else if ( oldPos > 0 )
	{
		isSpace = currItem->itemText.at(oldPos-1)->ch.at(0).isSpace();
		for (i=oldPos-2; i > 0; i--)
		{
			if (  currItem->itemText.at(i)->ch.at(0).isSpace() == isSpace )
			{
				if ( expandSel )
				currItem->itemText.at(i)->cselect = state;
			}
			else
				break;
			currItem->CPos--;
		}
	}
}
*/

bool ScribusView::slotSetCurs(int x, int y)
{
	PageItem *currItemGeneric;
	if (GetItem(&currItemGeneric))
	{
		if (!((currItemGeneric->asTextFrame()) || (currItemGeneric->asImageFrame())))
			return false;
		int xP = qRound(x/Scale + Doc->minCanvasCoordinate.x());
		int yP = qRound(y/Scale + Doc->minCanvasCoordinate.y());
		QPainter p;
		p.begin(this);
		Transform(currItemGeneric, &p);
		p.translate(qRound(-Doc->minCanvasCoordinate.x()), qRound(-Doc->minCanvasCoordinate.y()));
		if (currItemGeneric->asImageFrame())
			return true;
		PageItem_TextFrame *currItem=currItemGeneric->asTextFrame();
		if (currItem==0)
			return false;
		QRect mpo(x - Doc->guidesSettings.grabRad, y - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
		if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo)) ||
		        (QRegion(p.xForm(currItem->Clip)).contains(mpo)))
		{
			m_cursorVisible = true;
			//Work out which column we are in
			double colWidth=currItem->columnWidth();
			double colGap=currItem->ColGap;
			int currCol=0;
			int cp=static_cast<int>(currItem->xPos()+currItem->textToFrameDistLeft()+qRound(colGap)+qRound(colWidth));
			while (xP>cp)
			{
				++currCol;
				cp+=qRound(colGap);
				cp+=qRound(colWidth);
			};
			uint a;
			int xp, yp, h;
			bool breakAndReturn=false;
			uint currItemTextCount=currItem->itemText.count();
			for (a=0; a<currItemTextCount; ++a)
			{
				xp = static_cast<int>(currItem->itemText.at(a)->xp);
				//If x pos of curr char is less than left position of current column, continue
				if (xp+currItem->xPos()<cp-colWidth-colGap)
					continue;
				yp = static_cast<int>(currItem->itemText.at(a)->yp);
				h = static_cast<int>(Doc->docParagraphStyles[currItem->itemText.at(a)->cab].LineSpa);

				if (a<currItemTextCount-1)
				{
					if ((xp+currItem->xPos()+1>=xP) && (yp+currItem->yPos()>=yP))
					{
						//beginning of new paragraph or not
						if (((currItem->itemText.at(a)->ch.at(0).latin1() == 13) || (currItem->itemText.at(a)->ch.at(0).latin1() == 28)))
							currItem->CPos = a+1;
						else
							currItem->CPos = a;
						p.end();
						breakAndReturn=true;
						break;
					}
					if (yp-h+currItem->yPos()<=yP && yp+currItem->yPos()>=yP)
					{
						//click where next char is on next line in same or next column
						if (static_cast<int>(currItem->itemText.at(a+1)->yp)!=yp)
						{
							if (((currItem->itemText.at(a+1)->ch.at(0).latin1() == 13) || (currItem->itemText.at(a+1)->ch.at(0).latin1() == 28)))
								currItem->CPos = a+1;
							else
								currItem->CPos = a;
							p.end();
							breakAndReturn=true;
							break;
						}
					}
				}

				/* CB old, rather complicated code...
				chx = currItem->itemText.at(a)->ch;
				if (chx == QChar(30))
					chx = currItem->ExpandToken(a);
				if (chx == QChar(29))
					chx = " ";
				chs = currItem->itemText.at(a)->csize;
				currItem->SetZeichAttr(currItem->itemText.at(a), &chs, &chx);
				if ((chx == QChar(13)) || (chx == QChar(9)) || (chx == QChar(27)))
					w = 1;
				else
					w = qRound(Cwidth(Doc, currItem->itemText.at(a)->cfont, chx, chs)*(currItem->itemText.at(a)->cscale / 1000.0));
				h = static_cast<int>(Doc->docParagraphStyles[currItem->itemText.at(a)->cab].LineSpa);
				//If we click within text
				if (QRegion(p.xForm(QRect(xp-1, yp-h, w+1, h))).contains(QPoint(x, y)))
				{
					currItem->CPos = a;
					p.end();
					breakAndReturn=true;
					break;
				}
				//If we are not at the end of the text and the next characters y position > current one
				//IE, we are at the end of the line
				else if ((a<currItemTextCount-1) && (static_cast<int>(currItem->itemText.at(a+1)->yp)>yp))
				{
					//If one column only and we are within the width of the frame
					//Yes, some people want selection outside the frame to work too, this works ok for now
					if ((currItem->Cols==1) && (QRegion(p.xForm(QRect(xp-1, yp-h, static_cast<int>(currItem->width()-xp-1), h))).contains(QPoint(x, y))))
					{
						currItem->CPos = a;
						p.end();
						breakAndReturn=true;
						break;
					}
					//If columns>1, then work out what column we are in and place our cursor
					//TODO: Move this column placement code into PageItem_TextFrame.
					else if (currItem->Cols>1)
					{
						double colWidth=currItem->columnWidth();
						int currCol=0;
						int cp=static_cast<int>(currItem->xPos()+currItem->textToFrameDistLeft());
						do
						{
							++currCol;
							cp+=qRound(currItem->ColGap);
							if (currCol>1)
								cp+=qRound(colWidth);
						} while (xp>cp);
						if (QRegion(p.xForm(QRect(xp-1, yp-h, static_cast<int>(cp-xp-1), h))).contains(QPoint(x, y)))
						{
							currItem->CPos = a;
							p.end();
							breakAndReturn=true;
							break;
						}
					}
				}*/
			}
			if (breakAndReturn)
			{
				Doc->CurrFont = currItem->itemText.at(a)->cfont->scName();
				Doc->CurrFontSize = currItem->itemText.at(a)->csize;
				Doc->CurrTextFill = currItem->itemText.at(a)->ccolor;
				Doc->CurrTextFillSh = currItem->itemText.at(a)->cshade;
				Doc->CurrTextStroke = currItem->itemText.at(a)->cstroke;
				Doc->CurrTextStrokeSh = currItem->itemText.at(a)->cshade2;
				Doc->CurrTextScale = currItem->itemText.at(a)->cscale;
				Doc->CurrTextScaleV = currItem->itemText.at(a)->cscalev;
				Doc->CurrTextBase = currItem->itemText.at(a)->cbase;
				Doc->CurrTextShadowX = currItem->itemText.at(a)->cshadowx;
				Doc->CurrTextShadowY = currItem->itemText.at(a)->cshadowy;
				Doc->CurrTextOutline = currItem->itemText.at(a)->coutline;
				Doc->CurrTextUnderPos = currItem->itemText.at(a)->cunderpos;
				Doc->CurrTextUnderWidth = currItem->itemText.at(a)->cunderwidth;
				Doc->CurrTextStrikePos = currItem->itemText.at(a)->cstrikepos;
				Doc->CurrTextStrikeWidth = currItem->itemText.at(a)->cstrikewidth;
				emit ItemTextStrike(currItem->itemText.at(a)->cstrikepos, currItem->itemText.at(a)->cstrikewidth);
				emit ItemTextUnderline(currItem->itemText.at(a)->cunderpos, currItem->itemText.at(a)->cunderwidth);
				emit ItemTextOutline(currItem->itemText.at(a)->coutline);
				emit ItemTextShadow(currItem->itemText.at(a)->cshadowx, currItem->itemText.at(a)->cshadowy);
				emit ItemTextBase(currItem->itemText.at(a)->cbase);
				emit ItemTextSca(currItem->itemText.at(a)->cscale);
				emit ItemTextScaV(currItem->itemText.at(a)->cscalev);
				emit ItemTextFont(currItem->itemText.at(a)->cfont->scName());
				emit ItemTextSize(currItem->itemText.at(a)->csize);
				emit ItemTextUSval(currItem->itemText.at(a)->cextra);
				emit ItemTextStil(currItem->itemText.at(a)->cstyle);
				emit ItemTextAbs(currItem->itemText.at(a)->cab);
				emit ItemTextFarben(currItem->itemText.at(a)->cstroke, currItem->itemText.at(a)->ccolor, currItem->itemText.at(a)->cshade2, currItem->itemText.at(a)->cshade);
				return true;
			}

			/*currItemTextCount=currItem->itemText.count();
			qDebug(QString("%1").arg(2));
			for (a=0; a<currItemTextCount; ++a)
			{
				xp = static_cast<int>(currItem->itemText.at(a)->xp);
				yp = static_cast<int>(currItem->itemText.at(a)->yp);
				QPoint np = p.xForm(QPoint(xp, yp - static_cast<int>(Doc->docParagraphStyles[currItem->itemText.at(a)->cab].LineSpa)));
				if (yP < np.y())
				{
					currItem->CPos = a;
					p.end();
					uint i=a;
					if (a > 0)
						i--;
					Doc->CurrFont = currItem->itemText.at(i)->cfont->scName();
					Doc->CurrFontSize = currItem->itemText.at(i)->csize;
					Doc->CurrTextFill = currItem->itemText.at(i)->ccolor;
					Doc->CurrTextFillSh = currItem->itemText.at(i)->cshade;
					Doc->CurrTextStroke = currItem->itemText.at(i)->cstroke;
					Doc->CurrTextStrokeSh = currItem->itemText.at(i)->cshade2;
					Doc->CurrTextScale = currItem->itemText.at(i)->cscale;
					Doc->CurrTextScaleV = currItem->itemText.at(i)->cscalev;
					Doc->CurrTextBase = currItem->itemText.at(i)->cbase;
					Doc->CurrTextShadowX = currItem->itemText.at(i)->cshadowx;
					Doc->CurrTextShadowY = currItem->itemText.at(i)->cshadowy;
					Doc->CurrTextOutline = currItem->itemText.at(i)->coutline;
					Doc->CurrTextUnderPos = currItem->itemText.at(i)->cunderpos;
					Doc->CurrTextUnderWidth = currItem->itemText.at(i)->cunderwidth;
					Doc->CurrTextStrikePos = currItem->itemText.at(i)->cstrikepos;
					Doc->CurrTextStrikeWidth = currItem->itemText.at(i)->cstrikewidth;
					emit ItemTextStrike(currItem->itemText.at(i)->cstrikepos, currItem->itemText.at(i)->cstrikewidth);
					emit ItemTextUnderline(currItem->itemText.at(i)->cunderpos, currItem->itemText.at(i)->cunderwidth);
					emit ItemTextOutline(currItem->itemText.at(i)->coutline);
					emit ItemTextShadow(currItem->itemText.at(i)->cshadowx, currItem->itemText.at(i)->cshadowy);
					emit ItemTextSca(currItem->itemText.at(i)->cscale);
					emit ItemTextScaV(currItem->itemText.at(i)->cscalev);
					emit ItemTextFarben(currItem->itemText.at(i)->cstroke, currItem->itemText.at(i)->ccolor, currItem->itemText.at(i)->cshade2, currItem->itemText.at(i)->cshade);
					emit ItemTextFont(currItem->itemText.at(i)->cfont->scName());
					emit ItemTextSize(currItem->itemText.at(i)->csize);
					emit ItemTextUSval(currItem->itemText.at(i)->cextra);
					emit ItemTextStil(currItem->itemText.at(i)->cstyle);
					emit ItemTextAbs(currItem->itemText.at(i)->cab);
					emit ItemTextBase(currItem->itemText.at(i)->cbase);
					return true;
				}
			}
			*/
			//End of text in frame
			if (currItem->itemText.count() != 0)
			{
				a=currItem->itemText.count()-1;
				int w = qRound(Cwidth(Doc, currItem->itemText.at(a)->cfont, currItem->itemText.at(a)->ch, currItem->itemText.at(a)->csize)*(currItem->itemText.at(a)->cscale / 1000.0));
				if (xp+currItem->xPos()+w<xP || yp+currItem->yPos()<yP)
					currItem->CPos = a+1;
				else
					currItem->CPos = a;
			}
			else
				currItem->CPos = 0;
			p.end();

			if (currItem->itemText.count() > 0)
			{
				int b=currItem->CPos-1;
				if (b<0)
					b=0;
				Doc->CurrFont = currItem->itemText.at(b)->cfont->scName();
				Doc->CurrFontSize = currItem->itemText.at(b)->csize;
				Doc->CurrTextFill = currItem->itemText.at(b)->ccolor;
				Doc->CurrTextFillSh = currItem->itemText.at(b)->cshade;
				Doc->CurrTextStroke = currItem->itemText.at(b)->cstroke;
				Doc->CurrTextStrokeSh = currItem->itemText.at(b)->cshade2;
				Doc->CurrTextScale = currItem->itemText.at(b)->cscale;
				Doc->CurrTextScaleV = currItem->itemText.at(b)->cscalev;
				Doc->CurrTextBase = currItem->itemText.at(b)->cbase;
				Doc->CurrTextShadowX = currItem->itemText.at(b)->cshadowx;
				Doc->CurrTextShadowY = currItem->itemText.at(b)->cshadowy;
				Doc->CurrTextOutline = currItem->itemText.at(b)->coutline;
				Doc->CurrTextUnderPos = currItem->itemText.at(b)->cunderpos;
				Doc->CurrTextUnderWidth = currItem->itemText.at(b)->cunderwidth;
				Doc->CurrTextStrikePos = currItem->itemText.at(b)->cstrikepos;
				Doc->CurrTextStrikeWidth = currItem->itemText.at(b)->cstrikewidth;
				emit ItemTextStrike(currItem->itemText.at(b)->cstrikepos, currItem->itemText.at(b)->cstrikewidth);
				emit ItemTextUnderline(currItem->itemText.at(b)->cunderpos, currItem->itemText.at(b)->cunderwidth);
				emit ItemTextOutline(currItem->itemText.at(b)->coutline);
				emit ItemTextShadow(currItem->itemText.at(b)->cshadowx, currItem->itemText.at(b)->cshadowy);
				emit ItemTextSca(currItem->itemText.at(b)->cscale);
				emit ItemTextScaV(currItem->itemText.at(b)->cscalev);
				emit ItemTextFarben(currItem->itemText.at(b)->cstroke, currItem->itemText.at(b)->ccolor, currItem->itemText.at(b)->cshade2, currItem->itemText.at(b)->cshade);
				emit ItemTextFont(currItem->itemText.at(b)->cfont->scName());
				emit ItemTextSize(currItem->itemText.at(b)->csize);
				emit ItemTextUSval(currItem->itemText.at(b)->cextra);
				emit ItemTextStil(currItem->itemText.at(b)->cstyle);
				emit ItemTextAbs(currItem->itemText.at(b)->cab);
				emit ItemTextBase(currItem->itemText.at(b)->cbase);
				return true;
			}
			else
			{
				Doc->CurrFont = currItem->font();
				Doc->CurrFontSize = currItem->fontSize();
				Doc->CurrTextFill = currItem->TxtFill;
				Doc->CurrTextFillSh = currItem->ShTxtFill;
				Doc->CurrTextStroke = currItem->TxtStroke;
				Doc->CurrTextStrokeSh = currItem->ShTxtStroke;
				Doc->CurrTextScale = currItem->TxtScale;
				Doc->CurrTextScaleV = currItem->TxtScaleV;
				Doc->CurrTextBase = currItem->TxtBase;
				Doc->CurrTextShadowX = currItem->TxtShadowX;
				Doc->CurrTextShadowY = currItem->TxtShadowY;
				Doc->CurrTextOutline = currItem->TxtOutline;
				Doc->CurrTextUnderPos = currItem->TxtUnderPos;
				Doc->CurrTextUnderWidth = currItem->TxtUnderWidth;
				Doc->CurrTextStrikePos = currItem->TxtStrikePos;
				Doc->CurrTextStrikeWidth = currItem->TxtStrikeWidth;
				emit ItemTextStrike(currItem->TxtStrikePos, currItem->TxtStrikeWidth);
				emit ItemTextUnderline(currItem->TxtUnderPos, currItem->TxtUnderWidth);
				emit ItemTextOutline(currItem->TxtOutline);
				emit ItemTextShadow(currItem->TxtShadowX, currItem->TxtShadowY);
				emit ItemTextSca(currItem->TxtScale);
				emit ItemTextScaV(currItem->TxtScaleV);
				emit ItemTextFarben(currItem->TxtStroke, currItem->TxtFill, currItem->ShTxtStroke, currItem->ShTxtFill);
				emit ItemTextFont(currItem->font());
				emit ItemTextSize(currItem->fontSize());
				emit ItemTextUSval(currItem->ExtraV);
				emit ItemTextStil(currItem->TxTStyle);
				emit ItemTextAbs(currItem->textAlignment);
				emit ItemTextBase(currItem->TxtBase);
				return true;
			}
		}
	}
	return false;
}

void ScribusView::slotDoCurs(bool draw)
{
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		if (!currItem->asTextFrame())
			return;
		QPainter p;
		int xp, yp, yp1, desc, asce;
		p.begin(viewport());
		ToView(&p);
		Transform(currItem, &p);
		TransformM(currItem, &p);
		if (currItem->CPos > 0)
		{
			int offs = QMIN(currItem->CPos-1, static_cast<int>(currItem->itemText.count()-1));
			if (currItem->CPos < static_cast<int>(currItem->itemText.count()-1))
			{
				if (currItem->itemText.at(offs+1)->cstyle & 4096)
				{
					if (currItem->CPos < static_cast<int>(currItem->itemText.count()-1))
					currItem->CPos++;
					return;
				}
			}
			if (currItem->itemText.at(offs)->yp == 0)
				return;
			if (Doc->CurTimer != 0)
				Doc->CurTimer->stop();
			QString chx(currItem->itemText.at(offs)->ch);
			if (chx == QChar(30))
				chx = currItem->ExpandToken(offs);
			if (chx == QChar(29))
				chx = " ";
			int chs = currItem->itemText.at(offs)->csize;
			currItem->SetZeichAttr(currItem->itemText.at(offs), &chs, &chx);
			if (currItem->CPos != static_cast<int>(currItem->itemText.count()))
			{
				if (currItem->itemText.at(currItem->CPos)->ch == QChar(9))
				{
					xp = static_cast<int>(currItem->itemText.at(currItem->CPos-1)->xp);
					chs = currItem->itemText.at(currItem->CPos-1)->csize;
					chx = currItem->itemText.at(currItem->CPos-1)->ch;
					xp += qRound(Cwidth(Doc, currItem->itemText.at(currItem->CPos-1)->cfont, chx, chs)*(currItem->itemText.at(currItem->CPos-1)->cscale / 1000.0));
				}
				else
					xp = static_cast<int>(currItem->itemText.at(offs+1)->xp);
			}
			else
			{
				xp = static_cast<int>(currItem->itemText.at(offs)->xp);
				if (currItem->itemText.at(offs)->ch != QChar(9))
				{
					chs = currItem->itemText.at(offs)->csize;
					chx = currItem->itemText.at(offs)->ch;
					xp += qRound(Cwidth(Doc, currItem->itemText.at(offs)->cfont, chx, chs)*(currItem->itemText.at(offs)->cscale / 1000.0));
				}
			}
			if (currItem->CPos != static_cast<int>(currItem->itemText.count()))
			{
				if (currItem->itemText.at(offs)->yp != currItem->itemText.at(offs+1)->yp)
				{
					offs++;
					if ((currItem->itemText.at(offs)->ch == QChar(13)) || (currItem->itemText.at(offs)->ch == QChar(28)))
					{
						offs--;
						xp = static_cast<int>(currItem->itemText.at(offs)->xp);
						chs = currItem->itemText.at(offs)->csize;
						chx = currItem->itemText.at(offs)->ch;
						xp += qRound(Cwidth(Doc, currItem->itemText.at(offs)->cfont, chx, chs)*(currItem->itemText.at(offs)->cscale / 1000.0));
					}
					else
						xp = static_cast<int>(currItem->itemText.at(offs)->xp);
				}
			}
			yp = static_cast<int>(currItem->itemText.at(offs)->yp);
			desc = static_cast<int>(currItem->itemText.at(offs)->cfont->numDescender * (-currItem->itemText.at(offs)->csize / 10.0));
			asce = static_cast<int>(currItem->itemText.at(offs)->cfont->numAscent * (currItem->itemText.at(offs)->csize / 10.0));
		}
		else
		{
			if (currItem->itemText.count() == 0)
			{
				double lineCorr;
				if (currItem->lineColor() != CommonStrings::None)
					lineCorr = currItem->lineWidth() / 2.0;
				else
					lineCorr = 0;
				xp = static_cast<int>(currItem->textToFrameDistLeft() + lineCorr);
				yp = static_cast<int>(currItem->textToFrameDistTop() + lineCorr + currItem->lineSpacing());
				desc = static_cast<int>((*Doc->AllFonts)[currItem->font()]->numDescender * (-currItem->fontSize() / 10.0));
				asce = static_cast<int>((*Doc->AllFonts)[currItem->font()]->numAscent * (currItem->fontSize() / 10.0));
			}
			else
			{
				if ((currItem->itemText.at(currItem->CPos)->ch == QChar(9)) || (currItem->itemText.at(currItem->CPos)->ch == QChar(13)) || (currItem->itemText.at(currItem->CPos)->ch == QChar(28)))
				{
					double lineCorr;
					if (currItem->lineColor() != CommonStrings::None)
						lineCorr = currItem->lineWidth() / 2.0;
					else
						lineCorr = 0;
					xp = static_cast<int>(currItem->textToFrameDistLeft() + lineCorr);
					yp = static_cast<int>(currItem->textToFrameDistTop() + lineCorr + currItem->lineSpacing());
					desc = static_cast<int>((*Doc->AllFonts)[currItem->font()]->numDescender * (-currItem->fontSize() / 10.0));
					asce = static_cast<int>((*Doc->AllFonts)[currItem->font()]->numAscent * (currItem->fontSize() / 10.0));
				}
				else
				{
					xp = static_cast<int>(currItem->itemText.at(currItem->CPos)->xp);
					yp = static_cast<int>(currItem->itemText.at(currItem->CPos)->yp);
					desc = static_cast<int>(currItem->itemText.at(currItem->CPos)->cfont->numDescender * (-currItem->itemText.at(currItem->CPos)->csize / 10.0));
					asce = static_cast<int>(currItem->itemText.at(currItem->CPos)->cfont->numAscent * (currItem->itemText.at(currItem->CPos)->csize / 10.0));
				}
			}
		}
		yp1 = yp - asce;
		yp += desc;
		p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
		p.setRasterOp(XorROP);
		if (draw)
		{
			p.drawLine(xp, QMIN(QMAX(yp1,0),static_cast<int>(currItem->height())), xp, QMIN(QMAX(yp,0),static_cast<int>(currItem->height())));
			m_cursorVisible = !m_cursorVisible;
			if (Doc->CurTimer != 0)
			{
				if (!Doc->CurTimer->isActive())
					Doc->CurTimer->start(500);
			}
		}
		else
		{
			if (Doc->CurTimer != 0)
				Doc->CurTimer->stop();
			if (m_cursorVisible)
			{
				p.drawLine(xp, QMIN(QMAX(yp1,0),static_cast<int>(currItem->height())), xp, QMIN(QMAX(yp,0),static_cast<int>(currItem->height())));
				m_cursorVisible = false;
			}
		}
		p.end();
	}
}

void ScribusView::blinkCursor()
{
	slotDoCurs(true);
}

void ScribusView::HandleCurs(QPainter *p, PageItem *currItem, QRect mpo)
{
	QPoint tx, tx2;
	QWMatrix ma = p->worldMatrix();
	ma.setTransformationMode ( QWMatrix::Areas );
	p->setWorldMatrix(ma);
	tx = p->xForm(QPoint(static_cast<int>(currItem->width()), 0));
	tx2 = p->xForm(QPoint(0, static_cast<int>(currItem->height())));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == modeRotation)
			qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
		else
		{
			double rr = fabs(currItem->rotation());
			if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >=315.0) && (rr <= 360.0)))
				qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
			if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
				qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
		}
	}
	tx = p->xForm(QPoint(static_cast<int>(currItem->width()), static_cast<int>(currItem->height())/2));
	tx2 = p->xForm(QPoint(0, static_cast<int>(currItem->height())/2));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->rotation());
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
	}
	tx = p->xForm(QPoint(static_cast<int>(currItem->width())/2, 0));
	tx2 = p->xForm(QPoint(static_cast<int>(currItem->width())/2, static_cast<int>(currItem->height())));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->rotation());
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
	}
	tx = p->xForm(QPoint(static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
	tx2 = p->xForm(QPoint(0, 0));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == modeRotation)
			qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
		else
		{
			double rr = fabs(currItem->rotation());
			if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) ||
			        ((rr >= 315.0) && (rr <= 360.0)))
				qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
			if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
				qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
		}
	}
	if (Doc->EditClip)
		qApp->setOverrideCursor(QCursor(crossCursor), true);
}

void ScribusView::SelectItemNr(int nr, bool draw, bool single)
{
	SelectItem(Doc->Items->at(nr), draw, single);
}

//CB-->Doc/Fix
void ScribusView::SelectItem(PageItem *currItem, bool draw, bool single)
{
	if (!currItem->isSelected())
	{
		if (single)
		{
			Doc->selection->addItem(currItem);
			currItem->isSingleSel = true;
			currItem->FrameOnly = true;
			currItem->paintObj();
		}
		else
		{
			if (currItem->Groups.count() != 0)
			{
				if (Doc->selection->count() != 0)
				{
					if (Doc->selection->findItem(currItem) == -1)
						Doc->selection->addItem(currItem);
				}
				else
					Doc->selection->addItem(currItem);
				
				for (uint ga=0; ga<Doc->Items->count(); ++ga)
				{
					if (Doc->Items->at(ga)->Groups.count() != 0)
					{
						if (Doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
						{
							if (Doc->Items->at(ga)->ItemNr != currItem->ItemNr)
							{
								if (Doc->selection->findItem(Doc->Items->at(ga)) == -1)
									Doc->selection->addItem(Doc->Items->at(ga));
							}
							Doc->Items->at(ga)->FrameOnly = true;
							Doc->Items->at(ga)->paintObj();
						}
					}
				}
			}
			else
			{
				Doc->selection->addItem(currItem);
				currItem->FrameOnly = true;
				currItem->paintObj();
			}
			//CB FIXME/TODO We are surely prepending here and we have turned off 
			//emitting in prepend below so do it here.
			Doc->selection->itemAt(0)->emitAllToGUI();
		}
	}
	else
	{
	//CB Prepend used to occur to enable level changes to work properly, however with 
	//current selection code we dont seem to need that anymore
	/*
		if (Doc->selection->count() > 1)
		{
			PageItem *bb = Doc->selection->itemAt(0);
			Doc->selection->removeItem(currItem);
			Doc->selection->prependItem(currItem, false);
			currItem->FrameOnly = true;
			currItem->paintObj();
			bb->FrameOnly = true;
			bb->paintObj();
		}*/
	}
	if (draw)
	{
		if (Doc->selection->count() > 1)
		{
			setGroupRect();
			paintGroupRect();
			double x, y, w, h;
			getGroupRect(&x, &y, &w, &h);
			emit ItemPos(x, y);
			emit ItemGeom(w, h);
			//CB move in here as the emitAllToGUI will do it otherwise
			emit HaveSel(currItem->itemType());
		}
		//CB done by addItem for single selection or the frame data is already there
		//else
			//EmitValues(currItem);
			//currItem->emitAllToGUI();
	}
}

void ScribusView::selectionChanged()
{
	if (ScMW->isObjectSpecificUndo())
	{
		uint docSelectionCount=Doc->selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(Doc->selection->itemAt(0)->getUId());
		else if (docSelectionCount == 0)
			undoManager->showObject(Doc->currentPage->getUId());
		else
			undoManager->showObject(Um::NO_UNDO_STACK);
	}
}

void ScribusView::selectPage(QMouseEvent *m)
{
	m_MouseButtonPressed = true;
	Mxp = static_cast<int>(m->x()/Scale);
	Myp = static_cast<int>(m->y()/Scale);
	QRect mpo(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	ClRe = -1;
	Deselect(false);
	if (!Doc->masterPageMode())
	{
		uint docPagesCount=Doc->Pages->count();
		uint docCurrPageNo=Doc->currentPageNumber();
		for (uint i = 0; i < docPagesCount; ++i)
		{
			int x = static_cast<int>(Doc->Pages->at(i)->xOffset() * Scale);
			int y = static_cast<int>(Doc->Pages->at(i)->yOffset() * Scale);
			int w = static_cast<int>(Doc->Pages->at(i)->width() * Scale);
			int h = static_cast<int>(Doc->Pages->at(i)->height() * Scale);
			if (QRect(x, y, w, h).intersects(mpo))
			{
				if (docCurrPageNo != i)
				{
					Doc->currentPage = Doc->Pages->at(i);
					setMenTxt(i);
					DrawNew();
				}
				break;
			}
		}
		setRulerPos(contentsX(), contentsY());
	}
}

//CB-->Doc/Fix
bool ScribusView::SeleItem(QMouseEvent *m)
{
	QPainter p;
	QRect tx, mpo;
	PageItem *currItem;
	m_MouseButtonPressed = true;
	Mxp = static_cast<int>(m->x()/Scale);
	Myp = static_cast<int>(m->y()/Scale);
	int MxpS = static_cast<int>(m->x()/Scale + Doc->minCanvasCoordinate.x());
	int MypS = static_cast<int>(m->y()/Scale + Doc->minCanvasCoordinate.y());
	mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	ClRe = -1;
	if ((Doc->selection->count() != 0) && (m->state() == ControlButton))
		currItem = Doc->selection->itemAt(0);
	else
		currItem = Doc->Items->last();
	uint a;
	if (!Doc->masterPageMode())
	{
		for (a = 0; a < Doc->Pages->count(); ++a)
		{
			int x = static_cast<int>(Doc->Pages->at(a)->xOffset() * Scale);
			int y = static_cast<int>(Doc->Pages->at(a)->yOffset() * Scale);
			int w = static_cast<int>(Doc->Pages->at(a)->width() * Scale);
			int h = static_cast<int>(Doc->Pages->at(a)->height() * Scale);
			if (QRect(x, y, w, h).intersects(mpo))
			{
				if (Doc->currentPage->pageNr() != a)
				{
					Doc->currentPage = Doc->Pages->at(a);
					setMenTxt(a);
					DrawNew();
				}
				break;
			}
		}
		setRulerPos(contentsX(), contentsY());
	}
	if (m->state() == (ControlButton | AltButton))
		Deselect(false);

	if ((m->state() == (ShiftButton | AltButton)) && (!Doc->masterPageMode()) && (Doc->currentPage->FromMaster.count() != 0))
	{
		Page* Mp = Doc->MasterPages.at(Doc->MasterNames[Doc->currentPage->MPageNam]);
		currItem = Doc->currentPage->FromMaster.last();
		for (a = 0; a < Doc->currentPage->FromMaster.count(); ++a)
		{
			if (currItem->LayerNr == Doc->activeLayer())
			{
				p.begin(this);
				double OldX = currItem->xPos();
				double OldY = currItem->yPos();
				if (!currItem->ChangedMasterItem)
				{
					currItem->moveBy(-Mp->xOffset() + Doc->currentPage->xOffset(), -Mp->yOffset() + Doc->currentPage->yOffset());
				}
				Transform(currItem, &p);
				if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo)) ||
						(QRegion(p.xForm(currItem->Clip)).contains(mpo)))
				{
					if (!currItem->isSelected())
					{
						if ((m->state() != ShiftButton) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
							Deselect(false);
						if (currItem->Groups.count() != 0)
						{
							if (Doc->selection->count() != 0)
							{
								if (Doc->selection->findItem(currItem) == -1)
									Doc->selection->addItem(currItem);
							}
							else
								Doc->selection->addItem(currItem);
							if (m->state() != (ControlButton | AltButton))
							{
								for (uint ga=0; ga<Doc->Items->count(); ++ga)
								{
									if (Doc->Items->at(ga)->Groups.count() != 0)
									{
										if (Doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
										{
											if (Doc->Items->at(ga)->ItemNr != currItem->ItemNr)
											{
												if (Doc->selection->findItem(Doc->Items->at(ga)) == -1)
												{
													Doc->selection->addItem(Doc->Items->at(ga));
												}

											}
											Doc->Items->at(ga)->isSingleSel = false;
											Doc->Items->at(ga)->FrameOnly = true;
										}
									}
								}
							}
							else
							{
								currItem->isSingleSel = true;
								currItem->FrameOnly = true;
								currItem->paintObj();
							}
						}
						else
						{
							Doc->selection->addItem(currItem);
							currItem->FrameOnly = true;
							currItem->paintObj();
						}
					}
					else
					{
						if (Doc->selection->count() > 1)
						{
							PageItem *bb = Doc->selection->itemAt(0);
							Doc->selection->removeItem(currItem);
							Doc->selection->prependItem(currItem);
							currItem->FrameOnly = true;
							currItem->paintObj();
							bb->FrameOnly = true;
							bb->paintObj();
						}
					}
					if (Doc->selection->count() > 1)
					{
						for (uint aa = 0; aa < Doc->selection->count(); ++aa)
						{
							PageItem *bb = Doc->selection->itemAt(aa);
							bb->paintObj();
						}
						setGroupRect();
						paintGroupRect();
						double x, y, w, h;
						getGroupRect(&x, &y, &w, &h);
						emit ItemPos(x, y);
						emit ItemGeom(w, h);
						emit HaveSel(currItem->itemType());
					}
					else
					{
						currItem->emitAllToGUI();
					}
					p.end();
					if (!currItem->ChangedMasterItem)
					{
						currItem->setXYPos(OldX, OldY);
					}
					return true;
				}
				p.end();
				if (!currItem->ChangedMasterItem)
				{
					currItem->setXYPos(OldX, OldY);
				}
			}
			currItem = Doc->currentPage->FromMaster.prev();
		}
	}
	if ((m->state() == (ControlButton | ShiftButton)) && (Doc->selection->count() != 0))
	{
		for (a = 0; a < Doc->Items->count(); ++a)
		{
			if (currItem->isSelected())
			{
				if (currItem->ItemNr == 0)
				{
					currItem = Doc->Items->last();
					break;
				}
				currItem = Doc->Items->prev();
				break;
			}
			currItem = Doc->Items->prev();
		}
	}
	//Where all basic selection occurs having found the click location and the current page
	for (a = 0; a < Doc->Items->count(); ++a)
	{
		if (currItem == NULL)
		{
			Deselect(true);
			//Doc->selection->clear();
			return false;
		}
		if ((Doc->masterPageMode())  && (!((currItem->OwnPage == -1) || (currItem->OwnPage == static_cast<int>(Doc->currentPage->pageNr())))))
		{
			currItem = Doc->Items->prev();
			continue;
		}
		if (currItem->LayerNr == Doc->activeLayer())
		{
			p.begin(this);
			Transform(currItem, &p);
			if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo)) ||
			        (QRegion(p.xForm(currItem->Clip)).contains(mpo)))
			{
				//If the clicked on item is not tagged as selected
				if (!currItem->isSelected())
				{
					if ((m->state() != ShiftButton) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
						Deselect(false);
					//If we are selecting an item that is part of a group...
					if (currItem->Groups.count() != 0)
					{
						if (Doc->selection->count() != 0)
						{
							if (Doc->selection->findItem(currItem) == -1)
								Doc->selection->addItem(currItem);
						}
						else
							Doc->selection->addItem(currItem);
						//CB This is where we add the items of an unselected group
						if (m->state() != (ControlButton | AltButton))
						{
							for (uint ga=0; ga<Doc->Items->count(); ++ga)
							{
								if (Doc->Items->at(ga)->Groups.count() != 0)
								{
									if (Doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
									{
										if (Doc->Items->at(ga)->ItemNr != currItem->ItemNr)
										{
											if (Doc->selection->findItem(Doc->Items->at(ga)) == -1)
												Doc->selection->addItem(Doc->Items->at(ga));
										}
										Doc->Items->at(ga)->isSingleSel = false;
										Doc->Items->at(ga)->FrameOnly = true;
									}
								}
							}
						}
						else
						{
							currItem->isSingleSel = true;
							currItem->FrameOnly = true;
							currItem->paintObj();
						}
					}
					else
					//If we are just selecting one item
					{
						Doc->selection->addItem(currItem);
						currItem->FrameOnly = true;
						currItem->paintObj();
					}
				}
				else
				{
					if (Doc->selection->count() > 1)
					{
						PageItem *bb = Doc->selection->itemAt(0);
						Doc->selection->removeItem(currItem);
						Doc->selection->prependItem(currItem);
						currItem->FrameOnly = true;
						currItem->paintObj();
						bb->FrameOnly = true;
						//CB dont think we need to paint here when we paint below
						bb->paintObj();
					}
				}
				if (Doc->selection->count() > 1)
				{
					for (uint aa = 0; aa < Doc->selection->count(); ++aa)
					{
						PageItem *bb = Doc->selection->itemAt(aa);
						bb->paintObj();
					}
					setGroupRect();
					paintGroupRect();
					double x, y, w, h;
					getGroupRect(&x, &y, &w, &h);
					emit ItemPos(x, y);
					emit ItemGeom(w, h);
					emit HaveSel(currItem->itemType());
				}
				else
				{
					//CB Dont need this as creating the 0th selection does this
					currItem->paintObj();
				}
				if (Doc->selection->count() == 1)
				{
					HandleSizer(&p, currItem, mpo, m);
					if ((frameResizeHandle == 0) && (!currItem->locked()))
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
				}
				else
				{
					qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
					operItemResizing = false;
				}
				p.end();
				return true;
			}
			p.end();
		}
		currItem = Doc->Items->prev();
	}
	if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == modeNormal) && (!Doc->GuideLock) && (Doc->OnPage(MxpS, MypS) != -1))
	{
		GxM = -1;
		GyM = -1;
		if (Doc->currentPage->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < Doc->currentPage->YGuides.count(); ++yg)
			{
				if ((Doc->currentPage->YGuides[yg]+Doc->currentPage->yOffset()< (MypS+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->YGuides[yg]+Doc->currentPage->yOffset()> (MypS-Doc->guidesSettings.grabRad)))
				{
					GyM = yg;
					QPoint py = viewport()->mapFromGlobal(m->globalPos());
					DrHY = py.y();
					break;
				}
			}
		}
		if (Doc->currentPage->XGuides.count() != 0)
		{
			for (uint xg = 0; xg < Doc->currentPage->XGuides.count(); ++xg)
			{
				if ((Doc->currentPage->XGuides[xg]+Doc->currentPage->xOffset()< (MxpS+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->XGuides[xg]+Doc->currentPage->xOffset()> (MxpS-Doc->guidesSettings.grabRad)))
				{
					GxM = xg;
					QPoint py = viewport()->mapFromGlobal(m->globalPos());
					DrVX = py.x();
					break;
				}
			}
		}
		if (GxM!=-1 || GyM!=-1)
		{
			if (GxM==-1)
				// Horizontal Guide
				emit signalGuideInformation(0, qRound(Doc->currentPage->YGuides[GyM] * 10000.0) / 10000.0);
			else
				// Vertical Guide
				emit signalGuideInformation(1, qRound(Doc->currentPage->XGuides[GxM] * 10000.0) / 10000.0);
		}
		//else
		//	emit signalGuideInformation(-1, 0.0);
	}
	Deselect(true);
	return false;
}

//CB Fix item->old* stuff
void ScribusView::HandleSizer(QPainter *p, PageItem *currItem, QRect mpo, QMouseEvent *m)
{
	currItem->OldB = currItem->width();
	currItem->OldH = currItem->height();
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	frameResizeHandle = 0;
	if (currItem->sizeLocked())
		return;
	QRect ne = QRect();
	PaintSizeRect(p, ne);
	double d1;
	QMap<double,int> distance;
	FPoint n1(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
	n1 -= QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 1);
	n1 = FPoint(0, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
	n1 -= QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 2);
	if (!currItem->asLine())
	{
		QPoint docMinCanvasCoordinate(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
		n1 = FPoint(currItem->width(), 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 3);
		n1 = FPoint(0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 4);
		n1 = FPoint(currItem->width()/2, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 5);
		n1 = FPoint(currItem->width(), currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 6);
		n1 = FPoint(0, currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 7);
		n1 = FPoint(currItem->width()/2, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 8);
	}
	QValueList<int> result = distance.values();
	if (result.count() != 0)
		frameResizeHandle = result[0];
	mpo.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	HandleCurs(p, currItem, mpo);
	if (frameResizeHandle != 0)
	{
		if (!currItem->asLine())
			currItem->Sizing = true;
		operItemResizing = true;
	}
}

bool ScribusView::GetItem(PageItem **currItem, int nr)
{
	int n=nr;
	if (n == -1)
		n=0;
	*(currItem) = Doc->selection->itemAt(n);
	return (*(currItem)!=NULL);
}

//CB Remove bookmark interaction here, item/doc should do it
void ScribusView::Deselect(bool prop)
{
	if (!Doc->selection->isEmpty())
	{
		PageItem* currItem=NULL;
		for (uint a = 0; a < Doc->selection->count(); ++a)
		{
			currItem = Doc->selection->itemAt(a);
			if ((currItem->asTextFrame()) && (currItem->isBookmark))
				emit ChBMText(currItem);
		}
		if (Doc->selection->isMultipleSelection())
		{
			double x, y, w, h;
			getGroupRect(&x, &y, &w, &h);
			Doc->selection->clear();
			x -= Doc->minCanvasCoordinate.x();
			y -= Doc->minCanvasCoordinate.y();
			updateContents(static_cast<int>(x*Scale-5), static_cast<int>(y*Scale-5), static_cast<int>(w*Scale+10), static_cast<int>(h*Scale+10));
		}
		else
		{
			currItem=Doc->selection->itemAt(0);
			Doc->selection->clear();
			if (currItem!=NULL)
				updateContents(currItem->getRedrawBounding(Scale));
		}
	}
	if (prop)
		emit HaveSel(-1);
}

void ScribusView::SetupDraw(int nr)
{
	PageItem* currItem = Doc->Items->at(nr);
	currItem->setFont(Doc->toolSettings.defFont);
	currItem->setFontSize(Doc->toolSettings.defSize);
	operItemResizing = true;
	frameResizeHandle = 1;
	qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
	Doc->selection->clear();
	Doc->selection->addItem(currItem);
	currItem->paintObj();
	operItemMoving = true;
	Doc->appMode = modeNormal;
	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	moveTimer = moveTimer.addMSecs(500);
}

//CB-->Doc/Fix
void ScribusView::ToggleBookmark()
{
	uint docSelectionCount=Doc->selection->count();
	if (docSelectionCount != 0)
	{
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			PageItem* currItem = Doc->selection->itemAt(a);
			if (currItem->asTextFrame())
			{
				if (currItem->OwnPage != -1)
				{
					bool old = currItem->isBookmark;
					currItem->isBookmark = !currItem->isBookmark;
					if (currItem->isBookmark)
					{
						currItem->setIsAnnotation(false);
						emit AddBM(currItem);
					}
					else
					{
						if (old)
							emit DelBM(currItem);
					}
				}
			}
		}
		ScMW->actionManager->setPDFActions(this);
		emit DocChanged();
	}
}

//CB-->Doc/Fix
void ScribusView::ToggleAnnotation()
{
	if (Doc->selection->count() != 0)
	{
		for (uint a = 0; a < Doc->selection->count(); ++a)
		{
			PageItem* currItem = Doc->selection->itemAt(a);
			if (currItem->asTextFrame())
			{
				bool old = currItem->isBookmark;
				currItem->setIsAnnotation(!currItem->isAnnotation());
				if (currItem->isAnnotation())
				{
					if (old)
						emit DelBM(currItem);
					currItem->isBookmark = false;
				}
			}
		}
		ScMW->actionManager->setPDFActions(this);
		emit DocChanged();
	}
}

void ScribusView::sentToScrap()
{
	ScriXmlDoc *ss = new ScriXmlDoc();
	emit ToScrap(ss->WriteElem(Doc, this, Doc->selection));
	delete ss;
	ss=NULL;
}

//CB-->Doc/Fix
void ScribusView::ToBack()
{
	if (Doc->sendItemSelectionToBack())
	{
		ScMW->outlinePalette->BuildTree();
		emit LevelChanged(0);
		emit DocChanged();
		updateContents();
	}
}

//CB-->Doc/Fix
void ScribusView::ToFront()
{
	if (Doc->bringItemSelectionToFront())
	{
		ScMW->outlinePalette->BuildTree();
		emit LevelChanged(Doc->selection->itemAt(0)->ItemNr);
		emit DocChanged();
		updateContents();
	}
}

//CB-->Doc/Fix
void ScribusView::LowerItem()
{
	uint low = Doc->Items->count();
	uint high = 0;
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	PageItem *b2;
	uint docSelectionCount=Doc->selection->count();
	if ((Doc->Items->count() > 1) && (docSelectionCount != 0))
	{
		bool wasGUISelection=Doc->selection->isGUISelection();
		if (wasGUISelection)
		{
			Doc->selection->setIsGUISelection(false);
			Doc->selection->disconnectAllItemsFromGUI();
		}
		Selection tempSelection(*Doc->selection);
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = Doc->selection->itemAt(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			low = QMIN(currItem->ItemNr, low);
			high = QMAX(currItem->ItemNr, high);
		}
		if (low == 0)
			return;
		b2 = Doc->Items->at(high);
		Doc->selection->clear();
		SelectItemNr(low-1, false);
		for (uint c = 0; c < Doc->selection->count(); ++c)
		{
			currItem = Doc->selection->itemAt(c);
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items->findRef(currItem);
			Doc->Items->take(d);
		}
		d = Doc->Items->findRef(b2);
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			Doc->Items->insert(d+1, Doc->selection->itemAt(Oindex[c]));
		}
		Doc->selection->clear();
		for (uint a = 0; a < Doc->Items->count(); ++a)
		{
			Doc->Items->at(a)->ItemNr = a;
		}
		ScMW->outlinePalette->BuildTree();
		if (wasGUISelection)
			tempSelection.setIsGUISelection(true);
		*Doc->selection=tempSelection;
		emit LevelChanged(Doc->selection->itemAt(0)->ItemNr);
		emit DocChanged();
		updateContents();
	}
}

//CB-->Doc/Fix
void ScribusView::RaiseItem()
{
	uint low = Doc->Items->count();
	uint high = 0;
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	PageItem *b2;
	uint docSelectionCount=Doc->selection->count();
	if ((Doc->Items->count() > 1) && (docSelectionCount != 0))
	{
		bool wasGUISelection=Doc->selection->isGUISelection();
		if (wasGUISelection)
		{
			Doc->selection->setIsGUISelection(false);
			Doc->selection->disconnectAllItemsFromGUI();
		}
		Selection tempSelection(*Doc->selection);
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = Doc->selection->itemAt(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			low = QMIN(currItem->ItemNr, low);
			high = QMAX(currItem->ItemNr, high);
		}
		if (high == Doc->Items->count()-1)
			return;
		b2 = Doc->Items->at(low);
		Doc->selection->clear();
		SelectItemNr(high+1, false);
		for (uint c = 0; c < Doc->selection->count(); ++c)
		{
			currItem = Doc->selection->itemAt(c);
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items->findRef(currItem);
			Doc->Items->take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = 0; c <static_cast<int>(Oindex.count()); ++c)
		{
			d = Doc->Items->findRef(b2);
			if (d==-1)
				d=0;
			Doc->Items->insert(d, Doc->selection->itemAt(Oindex[c]));
		}
		Doc->selection->clear();
		for (uint a = 0; a < Doc->Items->count(); ++a)
		{
			Doc->Items->at(a)->ItemNr = a;
		}
		ScMW->outlinePalette->BuildTree();
		if (wasGUISelection)
			tempSelection.setIsGUISelection(true);
		*Doc->selection=tempSelection;
		emit LevelChanged(Doc->selection->itemAt(0)->ItemNr);
		emit DocChanged();
		updateContents();
	}
}

//CB Remove emit/start pasting objects
void ScribusView::PasteToPage()
{
	uint ac = Doc->Items->count();
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(Doc->currentPage->getUName(), 0, Um::Paste, "", Um::IPaste);
	emit LoadElem(ScMW->Buffer2, Mxp / Scale, Myp / Scale, false, false, Doc, this);
	Doc->DraggedElem = 0;
	Doc->DragElements.clear();
	updateContents();
	for (uint as = ac; as < Doc->Items->count(); ++as)
	{
		PageItem* currItem = Doc->Items->at(as);
		if (currItem->isBookmark)
			emit AddBM(currItem);
	}
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

/** Waagrechter Scrollbalken */
void ScribusView::setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (Ready)
		horizRuler->setGeometry(m_vhRulerHW, 1, w-m_vhRulerHW-1, m_vhRulerHW);
}

/** Senkrechter Scrollbalken */
void ScribusView::setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (Ready)
	{
		vertRuler->setGeometry(1, m_vhRulerHW, m_vhRulerHW, h-m_vhRulerHW-1);
		rulerMover->setGeometry(1, 1, m_vhRulerHW, m_vhRulerHW);
	}
}

bool ScribusView::mousePressed()
{
	return m_MouseButtonPressed;
}

bool ScribusView::groupTransactionStarted()
{
	return _groupTransactionStarted;
}

void ScribusView::setGroupTransactionStarted(bool isOn)
{
	_groupTransactionStarted = isOn;
}

// jjsa 27-03-2004 add for better setting while zooming
//CB find a new name
void ScribusView::rememberPreviousSettings(int mx, int my)
{
	oldX = mx;
	oldY = my;
}

void ScribusView::setRulerPos(int x, int y)
{
	if (ScMW->ScriptRunning)
		return;
	if (Doc->guidesSettings.rulerMode)
	{
		horizRuler->offs = x / Scale - Doc->currentPage->xOffset();
		vertRuler->offs = y / Scale - Doc->currentPage->yOffset();
	}
	else
	{
		horizRuler->offs = x / Scale;
		vertRuler->offs = y / Scale;
	}
//	horizRuler->offs += qRound(Doc->minCanvasCoordinate.x() - 1 - Doc->rulerXoffset);
//	vertRuler->offs += qRound(Doc->minCanvasCoordinate.y() - 1 - Doc->rulerYoffset);
	horizRuler->offs += Doc->minCanvasCoordinate.x()  - Doc->rulerXoffset;
	vertRuler->offs += Doc->minCanvasCoordinate.y()  - Doc->rulerYoffset;
	horizRuler->repaint();
	vertRuler->repaint();
	evSpon = true;
	QString newStatusBarText;
	if ((verticalScrollBar()->draggingSlider()) || (horizontalScrollBar()->draggingSlider()))
	{
		QValueList<int> pag;
		pag.clear();
		uint docPageCount=Doc->Pages->count();
		for (uint a = 0; a < docPageCount; ++a)
		{
			int xs = static_cast<int>(Doc->Pages->at(a)->xOffset() * Scale);
			int ys = static_cast<int>(Doc->Pages->at(a)->yOffset() * Scale);
			int ws = static_cast<int>(Doc->Pages->at(a)->width() * Scale);
			int hs = static_cast<int>(Doc->Pages->at(a)->height() * Scale);
			QRect drawRect = QRect(x, y, visibleWidth(), visibleHeight());
			drawRect.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
			if (drawRect.intersects(QRect(xs, ys, ws, hs)))
				pag.append(a+1);
		}
		if (!pag.isEmpty())
			newStatusBarText=( tr("Page %1 to %2").arg(pag.first()).arg(pag.last()));
	}
	ScMW->mainWindowStatusLabel->setText(newStatusBarText);
}

void ScribusView::setZoom()
{
	int x = qRound(QMAX(contentsX() / Scale, 0));
	int y = qRound(QMAX(contentsY() / Scale, 0));
	int w = qRound(QMIN(visibleWidth() / Scale, Doc->pageWidth));
	int h = qRound(QMIN(visibleHeight() / Scale, Doc->pageHeight));
	rememberPreviousSettings(w / 2 + x,h / 2 + y);
	setScale(zoomSpinBox->value() / 100.0 * Prefs->DisScale);
	slotDoZoom();
	ScMW->setFocus();
}

//CB This MUST now be called AFTER a call to doc->addPage or doc->addMasterPage as it
//does NOT create a page anymore.
Page* ScribusView::addPage(int nr, bool mov)
{
	Page* fe=Doc->Pages->at(nr);
	Q_ASSERT(fe!=0);
	if (fe==0)
		return 0;
	//Note this picks up the new page or master page depending on the mode.
	reformPages(mov);
	setMenTxt(nr);
	m_MouseButtonPressed = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	operItemMoving = false;
	MidButt = false;
	HaveSelRect = false;
	Magnify = false;
	FirstPoly = true;
	EdPoints = true;
	//GroupSel = false;
	DraggedGroup = false;
	MoveGY = false;
	MoveGX = false;
	EditContour = false;
	return fe;
}

void ScribusView::reformPages(bool moveObjects)
{
	Doc->reformPages(moveObjects);
	if (!ScMW->ScriptRunning)
		setContentsPos(qRound((Doc->currentPage->xOffset()-10 - Doc->minCanvasCoordinate.x()) * Scale), qRound((Doc->currentPage->yOffset()-10 - Doc->minCanvasCoordinate.y()) * Scale));
	if (!Doc->isLoading())
	{
		setRulerPos(contentsX(), contentsY());
		setMenTxt(Doc->currentPage->pageNr());
	}
}

void ScribusView::adjustCanvas(double width, double height, double dX, double dY)
{
	if (!operItemMoving)
	{
		updateOn = false;
		resizeContents(qRound((width) * Scale), qRound((height) * Scale));
		scrollBy(qRound((dX) * Scale), qRound((dY) * Scale));
		setRulerPos(contentsX(), contentsY());
		updateOn = true;
	}
	evSpon = false;
}

void ScribusView::setMenTxt(int Seite)
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	pageSelector->setMaxValue(Doc->masterPageMode() ? 1 : Doc->Pages->count());
	if ((!Doc->isLoading()) && (!Doc->masterPageMode()))
		pageSelector->GotoPg(Seite);
	connect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
}

/** Fuehrt die Vergroesserung/Verkleinerung aus */
void ScribusView::slotDoZoom()
{
	undoManager->setUndoEnabled(false);
	if (Scale > 32*Prefs->DisScale)
	{
		setScale(32*Prefs->DisScale);
		return;
	}
	updateOn = false;
	resizeContents(qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * Scale), qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * Scale));
	if (Doc->selection->count() != 0)
	{
		PageItem *currItem = Doc->selection->itemAt(0);
		SetCCPo(static_cast<int>(currItem->xPos() + currItem->width()/2), static_cast<int>(currItem->yPos() + currItem->height()/2));
	}
	else
		SetCCPo(oldX, oldY);
	updateOn = true;
	DrawNew();
	undoManager->setUndoEnabled(true);
}

void ScribusView::slotZoom100()
{
	int x = qRound(QMAX(contentsX() / Scale, Doc->minCanvasCoordinate.x()));
	int y = qRound(QMAX(contentsY() / Scale, Doc->minCanvasCoordinate.y()));
	int w = qRound(QMIN(visibleWidth() / Scale, Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
	int h = qRound(QMIN(visibleHeight() / Scale, Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
	rememberPreviousSettings(w / 2 + x,h / 2 + y);
	setScale(Prefs->DisScale);
	reformPages(false);
	slotDoZoom();
}

void ScribusView::slotZoomIn(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(QMAX(contentsX() / Scale, Doc->minCanvasCoordinate.x()));
		int y = qRound(QMAX(contentsY() / Scale, Doc->minCanvasCoordinate.y()));
		int w = qRound(QMIN(visibleWidth() / Scale, Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
		int h = qRound(QMIN(visibleHeight() / Scale, Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
		rememberPreviousSettings(w/2+x, h/2+y);
	}
	else
		rememberPreviousSettings(mx,my);
	setScale(Scale * static_cast<double>(Doc->toolSettings.magStep)/100.0);
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(QMAX(contentsX() / Scale, Doc->minCanvasCoordinate.x()));
		int y = qRound(QMAX(contentsY() / Scale, Doc->minCanvasCoordinate.y()));
		int w = qRound(QMIN(visibleWidth() / Scale, Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
		int h = qRound(QMIN(visibleHeight() / Scale, Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
		rememberPreviousSettings(w/2+x, h/2+y);
	}
	else
		rememberPreviousSettings(mx,my);
	setScale(Scale / (static_cast<double>(Doc->toolSettings.magStep)/100.0));
	slotDoZoom();
}

void ScribusView::DrawNew()
{
	if (ScMW->ScriptRunning)
		return;
	evSpon = false;
	forceRedraw = true;
	updateContents();
	setRulerPos(contentsX(), contentsY());
	setMenTxt(Doc->currentPage->pageNr());
	disconnect(zoomSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setZoom()));
	zoomSpinBox->setValue(Scale/Prefs->DisScale*100);
	connect(zoomSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setZoom()));
}

void ScribusView::SetCCPo(int x, int y)
{
	if (ScMW->ScriptRunning)
		return;
	center(static_cast<int>((x - Doc->minCanvasCoordinate.x()) * Scale), static_cast<int>((y - Doc->minCanvasCoordinate.y())* Scale));
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::SetCPo(int x, int y)
{
	if (ScMW->ScriptRunning)
		return;
	setContentsPos(static_cast<int>((x - Doc->minCanvasCoordinate.x()) * Scale), static_cast<int>((y - Doc->minCanvasCoordinate.y())* Scale));
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::updateLayerMenu()
{
	disconnect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	layerMenu->clear();
	QStringList newNames;
	Doc->orderedLayerList(&newNames);
	for (QStringList::Iterator it=newNames.begin(); it!=newNames.end(); ++it)
        layerMenu->insertItem(*it);
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
}

void ScribusView::setLayerMenuText(const QString &layerName)
{
	disconnect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	if (layerMenu->count() != 0)
		layerMenu->setCurrentText(layerName);
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
}

void ScribusView::GotoLa(int l)
{
	int level = Doc->layerCount()-l-1;
	int layerNumber=Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	Doc->setActiveLayer(layerNumber);
	//CB TODO fix this to use view calls after 1.3.2 release
	ScMW->changeLayer(Doc->activeLayer());
	emit changeLA(layerNumber);
}

void ScribusView::ChgUnit(int art)
{
	emit changeUN(art);
	unitChange();
	vertRuler->repaint();
	horizRuler->repaint();
}

void ScribusView::GotoPa(int Seite)
{
	GotoPage(Seite-1);
}

void ScribusView::GotoPage(int Seite)
{
	Doc->currentPage = Doc->Pages->at(Seite);
	if (ScMW->ScriptRunning)
		return;
	setMenTxt(Seite);
	SetCPo(qRound(Doc->currentPage->xOffset()-10),qRound(Doc->currentPage->yOffset()-10));
}

void ScribusView::showMasterPage(int nr)
{
	Deselect(false);
	OldScale = Scale;
	Doc->setMasterPageMode(true);
	Doc->currentPage = Doc->Pages->at(nr);
	pageSelector->setEnabled(false);
	updateOn = false;
//	reformPages();
	slotDoZoom();
	oldX = qRound(Doc->currentPage->xOffset()- 10);
	oldY = qRound(Doc->currentPage->yOffset()- 10);
	SetCPo(oldX, oldY);
	updateOn = true;
	DrawNew();
}

void ScribusView::hideMasterPage()
{
	Deselect(true);
	Doc->setMasterPageMode(false);
//	Doc->currentPage = Doc->Pages->at(0);
	pageSelector->setEnabled(true);
	resizeContents(qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * Scale), qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * Scale));
//	setScale(OldScale);
//	updateOn = false;
//	GotoPage(0);
//	slotDoZoom();
//	oldX = qRound(Doc->currentPage->xOffset()- 10);
//	oldY = qRound(Doc->currentPage->yOffset()- 10);
//	SetCPo(oldX, oldY);
//	updateOn = true;
//	DrawNew();
}

QImage ScribusView::MPageToPixmap(QString name, int maxGr)
{
	QImage pm;
	QImage im;
	int Nr = Doc->MasterNames[name];
	int clipx = static_cast<int>(Doc->ScratchLeft);
	int clipy = static_cast<int>(Doc->ScratchTop);
	int clipw = qRound(Doc->MasterPages.at(Nr)->width());
	int cliph = qRound(Doc->MasterPages.at(Nr)->height());
	if ((clipw > 0) && (cliph > 0))
	{
		double sca = Scale;
		bool frs = Doc->guidesSettings.framesShown;
		Page* act = Doc->currentPage;
		Doc->currentPage = Doc->MasterPages.at(Nr);
		Doc->guidesSettings.framesShown = false;
		setScale(1.0);
		previewMode = true;
		pm = QImage(clipw, cliph, 32, QImage::BigEndian);
		ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height());
		painter->clear(white);
		painter->translate(-clipx, -clipy);
		painter->setLineWidth(1);
		painter->setBrush(Doc->papColor);
		painter->drawRect(clipx, clipy, clipw, cliph);
		//Hmm do we need master page mode before this? Seiten.cpp uses this function.
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		Doc->guidesSettings.framesShown = frs;
		setScale(sca);
		Doc->currentPage = act;
		painter->end();
		double sx = im.width() / static_cast<double>(maxGr);
		double sy = im.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im = pm.smoothScale(static_cast<int>(pm.width() / sx), static_cast<int>(pm.height() / sx));
		else
			im = pm.smoothScale(static_cast<int>(pm.width() / sy), static_cast<int>(pm.height() / sy));
		delete painter;
		painter=NULL;
		previewMode = false;
	}
	return im;
}

QImage ScribusView::PageToPixmap(int Nr, int maxGr)
{
//	QImage pm;
	QImage im;
	double sx = maxGr / Doc->Pages->at(Nr)->width();
	double sy = maxGr / Doc->Pages->at(Nr)->height();
	double sc = QMIN(sx, sy);
	int clipx = static_cast<int>(Doc->Pages->at(Nr)->xOffset() * sc);
	int clipy = static_cast<int>(Doc->Pages->at(Nr)->yOffset() * sc);
	int clipw = qRound(Doc->Pages->at(Nr)->width() * sc);
	int cliph = qRound(Doc->Pages->at(Nr)->height() * sc);
	if ((clipw > 0) && (cliph > 0))
	{
		double sca = Scale;
		bool frs = Doc->guidesSettings.framesShown;
		Doc->guidesSettings.framesShown = false;
		setScale(sc);
		previewMode = true;
		Page* act = Doc->currentPage;
		Doc->currentPage = Doc->Pages->at(Nr);
		im = QImage(clipw, cliph, 32, QImage::BigEndian);
		ScPainter *painter = new ScPainter(&im, im.width(), im.height());
		painter->clear(Doc->papColor);
		painter->translate(-clipx, -clipy);
		painter->setFillMode(ScPainter::Solid);
		painter->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
		painter->setBrush(Doc->papColor);
		painter->drawRect(clipx, clipy, clipw, cliph);
		DrawMasterItems(painter, Doc->Pages->at(Nr), QRect(clipx, clipy, clipw, cliph));
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->end();
		Doc->guidesSettings.framesShown = frs;
		setScale(sca);
		Doc->currentPage = act;
/*		double sx = pm.width() / static_cast<double>(maxGr);
		double sy = pm.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im = pm.smoothScale(static_cast<int>(pm.width() / sx), static_cast<int>(pm.height() / sx));
		else
			im = pm.smoothScale(static_cast<int>(pm.width() / sy), static_cast<int>(pm.height() / sy)); */
		delete painter;
		painter=NULL;
		previewMode = false;
	}
	return im;
}

void ScribusView::rulerMove(QMouseEvent *m)
{
	QPoint py(viewport()->mapFromGlobal(m->globalPos()));
	int newY = py.y();
	int newX = py.x();
	QPoint out = viewportToContents(py);
	emit MousePos((py.x() + contentsX())/Scale, (py.y() + contentsY())/Scale);
	horizRuler->Draw(out.x());
	vertRuler->Draw(out.y());
	QPainter p;
	p.begin(viewport());
	p.setRasterOp(XorROP);
	p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
	p.drawLine(0, DrHY, viewport()->width(), DrHY);
	p.drawLine(0, newY, viewport()->width(), newY);
	p.drawLine(DrVX, 0, DrVX, viewport()->height());
	p.drawLine(newX, 0, newX, viewport()->height());
	p.end();
	DrHY = newY;
	DrVX = newX;
}

void ScribusView::setNewRulerOrigin(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	Doc->rulerXoffset = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	Doc->rulerYoffset = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
	if (Doc->guidesSettings.rulerMode)
	{
		Doc->rulerXoffset -= Doc->currentPage->xOffset();
		Doc->rulerYoffset -= Doc->currentPage->yOffset();
	}
	setRulerPos(contentsX(), contentsY());
	updateContents();
	int docSelectionCount=Doc->selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
		{
			double x, y, w, h;
			getGroupRect(&x, &y, &w, &h);
			emit ItemPos(x, y);
			emit ItemGeom(w, h);
		}
		else
			Doc->selection->itemAt(0)->emitAllToGUI();
	}
}

void ScribusView::FromHRuler(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	int newY = py.y();
	QPoint out = viewportToContents(py);
	emit MousePos((py.x() + contentsX())/Scale, (py.y() + contentsY())/Scale);
	horizRuler->Draw(out.x());
	vertRuler->Draw(out.y());
	QPainter p;
	p.begin(viewport());
	p.setRasterOp(XorROP);
	p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
	p.drawLine(0, DrHY, viewport()->width(), DrHY);
	p.drawLine(0, newY, viewport()->width(), newY);
	p.end();
	DrHY = newY;
}

void ScribusView::FromVRuler(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	int newY = py.x();
	QPoint out = viewportToContents(py);
	emit MousePos((py.x() + contentsX())/Scale, (py.y() + contentsY())/Scale);
	horizRuler->Draw(out.x());
	vertRuler->Draw(out.y());
	QPainter p;
	p.begin(viewport());
	p.setRasterOp(XorROP);
	p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
	p.drawLine(DrVX, 0, DrVX, viewport()->height());
	p.drawLine(newY, 0, newY, viewport()->height());
	p.end();
	DrVX = newY;
}

void ScribusView::SetYGuide(QMouseEvent *m, int oldIndex)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	double newX = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	double newY = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
	int pg = Doc->OnPage(newX, newY);
	if (pg != -1)
	{
		if (oldIndex < 0)
			Doc->Pages->at(pg)->addYGuide(newY-Doc->Pages->at(pg)->yOffset());
		else
			Doc->Pages->at(pg)->moveYGuide(oldIndex, newY-Doc->Pages->at(pg)->yOffset());
		emit DocChanged();
		emit signalGuideInformation(0, qRound((newY-Doc->Pages->at(pg)->yOffset()) * 10000.0) / 10000.0);
	}
	else if (oldIndex >= 0)
	{
		if ((newY > Doc->currentPage->yOffset()) && (newY < Doc->currentPage->yOffset() + Doc->currentPage->height()))
		{
			Doc->currentPage->moveYGuide(oldIndex, newY-Doc->currentPage->yOffset());
			emit signalGuideInformation(0, qRound((newY-Doc->currentPage->yOffset()) * 10000.0) / 10000.0);
		}
		else
		{
			Doc->currentPage->removeYGuide(oldIndex);
			emit signalGuideInformation(-1, 0.0);
		}
		emit DocChanged();
	}
}

void ScribusView::SetXGuide(QMouseEvent *m, int oldIndex)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	double newX = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	double newY = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
	int pg = Doc->OnPage(newX, newY);
	if (pg != -1)
	{
		if (oldIndex < 0)
			Doc->Pages->at(pg)->addXGuide(newX-Doc->Pages->at(pg)->xOffset());
		else
			Doc->Pages->at(pg)->moveXGuide(oldIndex, newX-Doc->Pages->at(pg)->xOffset());
		emit DocChanged();
		emit signalGuideInformation(1, qRound((newX-Doc->Pages->at(pg)->xOffset()) * 10000.0) / 10000.0);
	}
	else if (oldIndex >= 0)
	{
		if ((newX > Doc->currentPage->xOffset()) && (newX < Doc->currentPage->xOffset() + Doc->currentPage->width()))
		{
			Doc->currentPage->moveXGuide(oldIndex, newX-Doc->currentPage->xOffset());
			emit signalGuideInformation(0, qRound((newX-Doc->currentPage->xOffset()) * 10000.0) / 10000.0);
		}
		else
		{
			Doc->currentPage->removeXGuide(oldIndex);
			emit signalGuideInformation(-1, 0.0);
		}
		emit DocChanged();
	}
}

void ScribusView::Transform(PageItem *currItem, QPainter *p)
{
	p->translate(static_cast<int>(currItem->xPos()*Scale), static_cast<int>(currItem->yPos()*Scale));
	p->scale(Scale, Scale);
	p->rotate(currItem->rotation());
}

void ScribusView::TransformM(PageItem *currItem, QPainter *p)
{
	if (currItem->imageFlippedH())
	{
		p->translate(currItem->width(), 0);
		p->scale(-1, 1);
	}
	if (currItem->imageFlippedV())
	{
		p->translate(0, currItem->height());
		p->scale(1, -1);
	}
}

//CB-->Doc
void ScribusView::SetFrameRect()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		currItem->SetRectFrame();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(Scale));
	}
}

//CB-->Doc
void ScribusView::SetFrameRounded()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		if (currItem->cornerRadius() == 0)
		{
			SetFrameRect();
			return;
		}
		currItem->SetFrameRound();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(Scale));
	}
}

//CB-->Doc
void ScribusView::SetFrameOval()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		currItem->SetOvalFrame();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(Scale));
	}
}

void ScribusView::editExtendedImageProperties()
{
	if (Doc->selection->count() != 0)
	{
		PageItem *currItem = Doc->selection->itemAt(0);
		if (currItem->pixm.imgInfo.valid)
		{
			ExtImageProps* dia = new ExtImageProps(this, &currItem->pixm.imgInfo, currItem, this);
			dia->exec();
			delete dia;
			dia=NULL;
		}
	}
}

//CB-->Doc
//Fix size/move item calls
void ScribusView::adjustFrametoImageSize()
{
	uint docSelectionCount=Doc->selection->count();
	if (docSelectionCount > 0)
	{
		bool toUpdate=false;
		for (uint i = 0; i < docSelectionCount; ++i)
		{
			PageItem *currItem = Doc->selection->itemAt(i);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame() && currItem->PicAvail && !currItem->isTableItem)
				{
					double w, h, x, y;
					w = currItem->OrigW * currItem->imageXScale();
					h = currItem->OrigH * currItem->imageYScale();
					x = currItem->imageXOffset() * currItem->imageXScale();
					y = currItem->imageYOffset() * currItem->imageYScale();
					SizeItem(w, h, currItem->ItemNr);
					MoveItem(x, y, currItem);
					currItem->setImageXYOffset(0.0, 0.0);
					toUpdate=true;
				}
			}
		}
		if (toUpdate)
		{
			updateContents();
			emit DocChanged();
		}
	}
}

//CB Stop using this for putting items on pages apart from pasting
//IE write a more generic function in the doc
void ScribusView::PasteItem(struct CopyPasteBuffer *Buffer, bool loading, bool drag)
{
	QColor tmp;
	if (!loading)
		Deselect(true);
	double x = Buffer->Xpos;
	double y = Buffer->Ypos;
	double w = Buffer->Width;
	double h = Buffer->Height;
	double pw = Buffer->Pwidth;
	int z = 0;
	ScText *hg;
	switch (Buffer->PType)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	//
	case PageItem::ImageFrame:
		z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x, y, w, h, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
		Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
		Doc->Items->at(z)->setImageXYOffset(Buffer->LocalX, Buffer->LocalY);
		Doc->Items->at(z)->Pfile = Buffer->Pfile;
		Doc->Items->at(z)->IProfile = Buffer->IProfile;
		Doc->Items->at(z)->EmProfile = Buffer->EmProfile;
		Doc->Items->at(z)->IRender = Buffer->IRender;
		Doc->Items->at(z)->UseEmbedded = Buffer->UseEmbedded;
		if (!Doc->Items->at(z)->Pfile.isEmpty())
			Doc->LoadPict(Doc->Items->at(z)->Pfile, z);
		Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
		Doc->Items->at(z)->setImageShown(Buffer->PicArt);
/*		Doc->Items->at(z)->BBoxX = Buffer->BBoxX;
		Doc->Items->at(z)->BBoxH = Buffer->BBoxH; */
		Doc->Items->at(z)->ScaleType = Buffer->ScaleType;
		Doc->Items->at(z)->AspectRatio = Buffer->AspectRatio;
		Doc->Items->at(z)->setLineWidth(Buffer->Pwidth);
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType3:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	//
	case PageItem::PathText:
	case PageItem::TextFrame:
		if (Buffer->PType == PageItem::PathText)
			z = Doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, !m_MouseButtonPressed);
		else
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, !m_MouseButtonPressed);
		if ((Buffer->m_isAnnotation) && (Buffer->m_annotation.UseIcons()))
		{
			Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
			Doc->Items->at(z)->setImageXYOffset(Buffer->LocalX, Buffer->LocalY);
			Doc->Items->at(z)->Pfile = Buffer->Pfile;
			Doc->Items->at(z)->Pfile2 = Buffer->Pfile2;
			Doc->Items->at(z)->Pfile3 = Buffer->Pfile3;
			Doc->Items->at(z)->IProfile = Buffer->IProfile;
			Doc->Items->at(z)->EmProfile = Buffer->EmProfile;
			Doc->Items->at(z)->IRender = Buffer->IRender;
			Doc->Items->at(z)->UseEmbedded = Buffer->UseEmbedded;
			Doc->LoadPict(Doc->Items->at(z)->Pfile, z);
			Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
			Doc->Items->at(z)->setImageShown(Buffer->PicArt);
		}
		if (!Buffer->itemText.isEmpty())
		{
			QTextStream t(&Buffer->itemText, IO_ReadOnly);
			QString cc;
			while (!t.atEnd())
			{
				cc = t.readLine();
				if (cc.isEmpty())
					continue;
				QStringList wt;
				QStringList::Iterator it;
				wt = QStringList::split("\t", cc);
				it = wt.begin();
				hg = new ScText;
				hg->ch = (*it);
				if (hg->ch == QChar(5))
					hg->ch = QChar(13);
				if (hg->ch == QChar(4))
					hg->ch = QChar(9);
				it++;
				hg->cfont = (*Doc->AllFonts)[*it];
				it++;
				hg->csize = qRound((*it).toDouble() * 10);
				it++;
				hg->ccolor = *it;
				it++;
				hg->cextra = (*it).toInt();
				it++;
				hg->cshade = (*it).toInt();
				hg->cselect = false;
				it++;
				hg->cstyle = it == NULL ? 0 : (*it).toInt();
				it++;
				hg->cab = it == NULL ? 0 : (*it).toInt();
				it++;
				hg->cstroke = it == NULL ? CommonStrings::None : *it;
				it++;
				hg->cshade2 = it == NULL ? 100 : (*it).toInt();
				it++;
				if (it == NULL)
					hg->cscale = 1000;
				else
					hg->cscale = QMIN(QMAX((*it).toInt(), 100), 4000);
				it++;
				if (it == NULL)
					hg->cscalev = 1000;
				else
					hg->cscalev = QMIN(QMAX((*it).toInt(), 100), 4000);
				it++;
				hg->cbase = it == NULL ? 0 : (*it).toInt();
				it++;
				hg->cshadowx = it == NULL ? 50 : (*it).toInt();
				it++;
				hg->cshadowy = it == NULL ? -50 : (*it).toInt();
				it++;
				hg->coutline = it == NULL ? 10 : (*it).toInt();
				it++;
				hg->cunderpos = it == NULL ? -1 : (*it).toInt();
				it++;
				hg->cunderwidth = it == NULL ? -1 : (*it).toInt();
				it++;
				hg->cstrikepos = it == NULL ? -1 : (*it).toInt();
				it++;
				hg->cstrikewidth = it == NULL ? -1 : (*it).toInt();
				hg->xp = 0;
				hg->yp = 0;
				hg->PRot = 0;
				hg->PtransX = 0;
				hg->PtransY = 0;
				Doc->Items->at(z)->itemText.append(hg);
			}
		}
		Doc->Items->at(z)->setLineSpacing(Buffer->LineSp);
//		Doc->Items->at(z)->convertTo(Buffer->PType);
		break;
	case PageItem::Line:
		z = Doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w ,0, pw, CommonStrings::None, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	case PageItem::Polygon:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	case PageItem::PolyLine:
		z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	}
	PageItem *currItem = Doc->Items->at(z);
	currItem->setLineSpacingMode(Buffer->LineSpMode);
	if (currItem->lineSpacingMode() == 3)
	{
		Doc->docParagraphStyles[0].BaseAdj = true;
		currItem->setLineSpacing(Doc->typographicSettings.valueBaseGrid-1);
	}
	currItem->setImageFlippedH(Buffer->flippedH);
	currItem->setImageFlippedV(Buffer->flippedV);
	currItem->setCornerRadius(Buffer->RadRect);
	currItem->FrameType = Buffer->FrameType;
	currItem->ClipEdited = Buffer->ClipEdited;
	currItem->setFillColor(Buffer->Pcolor);
	currItem->setLineColor(Buffer->Pcolor2);
	currItem->setFillShade(Buffer->Shade);
	currItem->setLineShade(Buffer->Shade2);
	currItem->TxtStroke = Buffer->TxtStroke;
	currItem->TxtFill = Buffer->TxtFill;
	currItem->ShTxtStroke = Buffer->ShTxtStroke;
	currItem->ShTxtFill = Buffer->ShTxtFill;
	currItem->TxtScale = Buffer->TxtScale;
	currItem->TxtScaleV = Buffer->TxtScaleV;
	currItem->TxTStyle = Buffer->TxTStyle;
	currItem->TxtShadowX = Buffer->TxtShadowX;
	currItem->TxtShadowY = Buffer->TxtShadowY;
	currItem->TxtOutline = Buffer->TxtOutline;
	currItem->TxtUnderPos = Buffer->TxtUnderPos;
	currItem->TxtUnderWidth = Buffer->TxtUnderWidth;
	currItem->TxtStrikePos = Buffer->TxtStrikePos;
	currItem->TxtStrikeWidth = Buffer->TxtStrikeWidth;
	currItem->setRotation(Buffer->Rot);
	currItem->setTextToFrameDist(Buffer->Extra, Buffer->RExtra, Buffer->TExtra, Buffer->BExtra);
	currItem->PLineArt = PenStyle(Buffer->PLineArt);
	currItem->PLineEnd = PenCapStyle(Buffer->PLineEnd);
	currItem->PLineJoin = PenJoinStyle(Buffer->PLineJoin);
	currItem->setPrintEnabled(Buffer->isPrintable);
	currItem->isBookmark = Buffer->isBookmark;
//	currItem->BMnr = Buffer->BMnr;
	currItem->setIsAnnotation(Buffer->m_isAnnotation);
	currItem->setAnnotation(Buffer->m_annotation);
	if (!Buffer->AnName.isEmpty())
	{
		if (!drag)
		{
			if (currItem->itemName() == Buffer->AnName)
				currItem->AutoName = true;
			else
			{
				currItem->setItemName(Buffer->AnName);
				currItem->AutoName = false;
			}
		}
		else
		{
			currItem->setItemName(currItem->generateUniqueCopyName(Buffer->AnName));
			currItem->AutoName = false;
		}
	}

	currItem->Clip = Buffer->Clip.copy();
	currItem->PoShow = Buffer->PoShow;
	currItem->BaseOffs = Buffer->BaseOffs;
	currItem->setTextFlowsAroundFrame(Buffer->Textflow);
	currItem->setTextFlowUsesBoundingBox(Buffer->Textflow2);
	currItem->textAlignment = Buffer->textAlignment;
	currItem->setFont(Buffer->IFont);
	currItem->setFontSize(Buffer->ISize);
	currItem->ExtraV = Buffer->ExtraV;
	currItem->Groups = Buffer->Groups;
	currItem->TabValues = Buffer->TabValues;
	currItem->DashValues = Buffer->DashValues;
	currItem->DashOffset = Buffer->DashOffset;
	currItem->setLocked(Buffer->Locked);
	currItem->setSizeLocked(Buffer->LockRes);
	currItem->setFillTransparency(Buffer->Transparency);
	currItem->setLineTransparency(Buffer->TranspStroke);
	currItem->setReversed(Buffer->Reverse);
	currItem->NamedLStyle = Buffer->NamedLStyle;
	currItem->Language = ScMW->GetLang(Buffer->Language);
	currItem->Cols = Buffer->Cols;
	currItem->ColGap = Buffer->ColGap;
	if (Buffer->LayerNr != -1)
		currItem->LayerNr = Buffer->LayerNr;
	currItem->PoLine = Buffer->PoLine.copy();
	currItem->setTextFlowUsesContourLine(Buffer->UseContour);
	if (Buffer->ContourLine.size() == 0)
		currItem->ContourLine = currItem->PoLine.copy();
	else
		currItem->ContourLine = Buffer->ContourLine.copy();
	if (!currItem->asLine())
	{
		// OBSOLETE CR 2005-02-06
		if ((currItem->PoLine.size() == 0) && (currItem->itemType() != PageItem::ItemType1))
			currItem->convertClip();
		else
			//
			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	}
	else
	{
		int ph = static_cast<int>(QMAX(1.0, currItem->lineWidth() / 2.0));
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
		currItem->setHeight(1.0);
	}
	// OBSOLETE CR 2005-02-06
	if (currItem->itemType() == PageItem::ItemType1)
	{
		currItem->SetOvalFrame();
		Doc->setRedrawBounding(currItem);
	}
	// OBSOLETE CR 2005-02-06
	if (currItem->itemType() == PageItem::ItemType3)
	{
		if (currItem->cornerRadius() != 0.0)
		{
			ClRe = -1;
			currItem->SetFrameRound();
			Doc->setRedrawBounding(currItem);
		}
		else
		{
			currItem->SetRectFrame();
			Doc->setRedrawBounding(currItem);
		}
		currItem->ClipEdited = true;
	}
	if (currItem->asImageFrame())
		currItem->AdjustPictScale();
	if (!(currItem->asTextFrame()) && !(currItem->asPathText()))
		currItem->setFont(Doc->toolSettings.defFont);
	if (currItem->asPathText())
	{
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		currItem->updatePolyClip();
		currItem->Frame = true;
	}
	if (Buffer->GrType != 0)
	{
		if ((!Buffer->GrColor.isEmpty()) && (!Buffer->GrColor2.isEmpty()))
		{
			currItem->fill_gradient.clearStops();
			if (Buffer->GrType == 5)
			{
				if ((Buffer->GrColor != CommonStrings::None) && (!Buffer->GrColor.isEmpty()))
					currItem->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
				currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
				if ((Buffer->GrColor2 != CommonStrings::None) && (!Buffer->GrColor2.isEmpty()))
					currItem->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
				currItem->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
			}
			else
			{
				if ((Buffer->GrColor2 != CommonStrings::None) && (!Buffer->GrColor2.isEmpty()))
					currItem->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
				currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
				if ((Buffer->GrColor != CommonStrings::None) && (!Buffer->GrColor.isEmpty()))
					currItem->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
				currItem->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
			}
		}
		else
			currItem->fill_gradient = Buffer->fill_gradient;
		currItem->GrType = Buffer->GrType;
		currItem->GrStartX = Buffer->GrStartX;
		currItem->GrStartY = Buffer->GrStartY;
		currItem->GrEndX = Buffer->GrEndX;
		currItem->GrEndY = Buffer->GrEndY;
		currItem->updateGradientVectors();
	}
	currItem->setObjectAttributes(&(Buffer->pageItemAttributes));
	Doc->setRedrawBounding(currItem);
	currItem->OwnPage = Doc->OnPage(currItem);
	if (!loading)
	{
		Doc->selection->addItem(currItem);
		emit HaveSel(currItem->itemType());
		currItem->emitAllToGUI();
		emit DocChanged();
		updateContents();
	}
}

//CB get this out of here, remove the need completely if possible. Used for cpal on mpalette
void ScribusView::QueryFarben()
{
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
		emit ItemGradient(currItem->GrType);
		emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
	}
}

void ScribusView::ToPicFrame()
{
	PageItem *currItem = Doc->selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::ImageFrame);
	RefreshItem(newItem);
	SelectItem(newItem);
	if (!Doc->isLoading())
		emit UpdtObj(Doc->currentPage->pageNr(), newItem->ItemNr);
	emit DocChanged();
}

void ScribusView::ToPolyFrame()
{
	PageItem *currItem = new PageItem_Polygon(*Doc->selection->itemAt(0));
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::Polygon);
	RefreshItem(newItem);
	SelectItem(newItem);
	if (!Doc->isLoading())
		emit UpdtObj(Doc->currentPage->pageNr(), newItem->ItemNr);
	emit DocChanged();
}

void ScribusView::ToTextFrame()
{
	PageItem *currItem = Doc->selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::TextFrame);
	RefreshItem(newItem);
	SelectItem(newItem);
	if (!Doc->isLoading())
		emit UpdtObj(Doc->currentPage->pageNr(), newItem->ItemNr);
	emit DocChanged();
}

void ScribusView::ToBezierFrame()
{
	PageItem *currItem = Doc->selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PolyLine);
	RefreshItem(newItem);
	SelectItem(newItem);
	if (!Doc->isLoading())
		emit UpdtObj(Doc->currentPage->pageNr(), newItem->ItemNr);
	emit DocChanged();
}

void ScribusView::Bezier2Poly()
{
	PageItem *currItem = Doc->selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::Polygon);
	RefreshItem(newItem);
	SelectItem(newItem);
	if (!Doc->isLoading())
		emit UpdtObj(Doc->currentPage->pageNr(), newItem->ItemNr);
	emit DocChanged();
}

void ScribusView::ToPathText()
{
	if (Doc->selection->count() > 1)
	{
		PageItem* currItem=Doc->selection->itemAt(0);
		PageItem *polyLineItem;
		if (currItem->asTextFrame())
			polyLineItem=Doc->selection->itemAt(1);
		else
		{
			polyLineItem=Doc->selection->itemAt(0);
			currItem=Doc->selection->itemAt(1);
		}
		if (!polyLineItem->asPolyLine())
			return;

		Deselect(true);
		PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PathText, polyLineItem);
		RefreshItem(newItem);
		SelectItem(newItem);
		if (!Doc->isLoading())
			emit UpdtObj(Doc->currentPageNumber(), newItem->ItemNr);
		emit DocChanged();
	}
}

void ScribusView::FromPathText()
{
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		Deselect(true);
		PageItem* newItem=Doc->convertItemTo(currItem, PageItem::TextFrame);
		SelectItem(newItem);
		ToFront();
		update();
	}
}

void ScribusView::TextToPath()
{
	ScMW->NoFrameEdit();
	uint selectedItemCount=Doc->selection->count();
	if (selectedItemCount != 0)
	{
		QPtrList<PageItem> delItems,newGroupedItems;
		uint offset=0;
		for(uint i=0; i<selectedItemCount; ++i)
		{
			PageItem *currItem = Doc->selection->itemAt(offset);
			bool cont=false;
			if ((!((currItem->asTextFrame()) || (currItem->asPathText()))) || (currItem->isTableItem && currItem->isSingleSel) || (currItem->locked()) || currItem->itemText.count() == 0)
				cont=true;
			if (currItem==ScMW->storyEditor->currentItem() && Doc==ScMW->storyEditor->currentDocument())
			{
				QMessageBox::information(ScMW, tr("Cannot Convert In-Use Item"), "<qt>" + tr("The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped").arg(currItem->itemName()) + "</qt>", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
				cont=true;
			}
			//Deselect();
			if (cont)
			{
				++offset;
				continue;
			}
			newGroupedItems.clear();
			FPointArray pts;
			double x, y, wide;
			QString chx, ccounter;
			PageItem* bb;
			ScText *hl;
			for (uint a = 0; a < currItem->MaxChars; ++a)
			{
				pts.resize(0);
				x = 0.0;
				y = 0.0;
				hl = currItem->itemText.at(a);
				chx = hl->ch;
				if ((chx == QChar(13)) || (chx == QChar(32)) || (chx == QChar(29)))
					continue;
				if (chx == QChar(30))
				{
					chx = currItem->ExpandToken(a);
					if (chx == QChar(32))
						continue;
				}
				int chs = hl->csize;
				if (hl->cstyle & 64)
				{
					if (chx.upper() != chx)
					{
						chs = QMAX(static_cast<int>(hl->csize * Doc->typographicSettings.valueSmallCaps / 100), 1);
						chx = chx.upper();
					}
				}
				double csi = static_cast<double>(chs) / 100.0;
				uint chr = chx[0].unicode();
				QWMatrix chma, chma2, chma3, chma4, chma6;
				if (currItem->asPathText())
				{
					QWMatrix trafo = QWMatrix( 1, 0, 0, -1, -hl->PRot, 0 );
					trafo *= QWMatrix( hl->PtransX, hl->PtransY, hl->PtransY, -hl->PtransX, hl->xp, hl->yp);
					if (currItem->rotation() != 0)
					{
						QWMatrix sca;
						sca.translate(-currItem->xPos(), -currItem->yPos());
						sca.rotate(currItem->rotation());
						trafo *= sca;
					}
					chma.scale(csi, csi);
					chma2.scale(hl->cscale / 1000.0, hl->cscalev / 1000.0);
					if (currItem->reversed())
					{
						if (a < currItem->itemText.count()-1)
							wide = Cwidth(Doc, hl->cfont, chx, hl->csize, currItem->itemText.at(a+1)->ch);
						else
							wide = Cwidth(Doc, hl->cfont, chx, hl->csize);
						chma3.scale(-1, 1);
						chma3.translate(-wide, 0);
						chma4.translate(0, currItem->BaseOffs-((hl->csize / 10.0) * (hl->cscalev / 1000.0)));
					}
					else
						chma4.translate(0, currItem->BaseOffs-((hl->csize / 10.0) * (hl->cscalev / 1000.0)));
					if (hl->cbase != 0)
						chma6.translate(0, -(hl->csize / 10.0) * (hl->cbase / 1000.0));
					pts = currItem->itemText.at(a)->cfont->GlyphArray[chr].Outlines.copy();
					if (pts.size() < 4)
						continue;
					pts.map(chma * chma2 * chma3 * chma4 * chma6);
					pts.map(trafo);
					if (currItem->rotation() != 0)
					{
						QWMatrix sca;
						sca.translate(currItem->xPos(), currItem->yPos());
						pts.map(sca);
					}
				}
				else
				{
					chma.scale(csi, csi);
					pts = hl->cfont->GlyphArray[chr].Outlines.copy();
					if (pts.size() < 4)
						continue;
					x = hl->cfont->GlyphArray[chr].x * csi;
					y = hl->cfont->GlyphArray[chr].y * csi;
					pts.map(chma);
					chma = QWMatrix();
					chma.scale(hl->cscale / 1000.0, hl->cscalev / 1000.0);
					pts.map(chma);
					chma = QWMatrix();
					if (currItem->imageFlippedH() && (!currItem->reversed()))
						chma.scale(-1, 1);
					if (currItem->imageFlippedV())
						chma.scale(1, -1);
					pts.map(chma);
				}
				uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_MouseButtonPressed);
				bb = Doc->Items->at(z);
				bb->setTextFlowsAroundFrame(currItem->textFlowsAroundFrame());
				bb->setTextFlowUsesBoundingBox(currItem->textFlowUsesBoundingBox());
				bb->setSizeLocked(currItem->sizeLocked());
				bb->setLocked(currItem->locked());
				bb->NamedLStyle = currItem->NamedLStyle;
				bb->setItemName(currItem->itemName()+"+"+ccounter.setNum(a));
				bb->AutoName = false;
				bb->PoLine = pts.copy();
				if (!currItem->asPathText())
					bb->setRotation(currItem->rotation());
				bb->setFillColor(hl->ccolor);
				bb->setFillShade(hl->cshade);
				if (currItem->itemText.at(a)->cstyle & 4)
				{
					bb->setLineColor(hl->cstroke);
					bb->setLineShade(hl->cshade2);
				}
				else
				{
					bb->setLineColor(CommonStrings::None);
					bb->setLineShade(100);
				}
				bb->setLineWidth(chs * hl->coutline / 10000.0);
				if (currItem->asPathText())
					AdjustItemSize(bb);
				else
				{
					FPoint tp2(getMinClipF(&bb->PoLine));
					bb->PoLine.translate(-tp2.x(), -tp2.y());
					FPoint tp(getMaxClipF(&bb->PoLine));
					bb->setWidthHeight(tp.x(), tp.y());
					bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
					double textX = hl->xp;
					double textY = hl->yp;
					if (a < currItem->itemText.count()-1)
						wide = Cwidth(Doc, hl->cfont, chx, hl->csize, currItem->itemText.at(a+1)->ch);
					else
						wide = Cwidth(Doc, hl->cfont, chx, hl->csize);
					if (currItem->imageFlippedH())
						textX = currItem->width() - textX - wide;
					if (currItem->imageFlippedV())
						textY = currItem->height() - textY+ y - (bb->height() - y);
					FPoint npo(textX+x, textY-y, 0.0, 0.0, currItem->rotation(), 1.0, 1.0);
					bb->moveBy(npo.x(),npo.y());
				}
				bb->ContourLine = bb->PoLine.copy();
				bb->ClipEdited = true;
				Doc->setRedrawBounding(bb);
				newGroupedItems.append(bb);
			}
			if ((currItem->asPathText()) && (currItem->PoShow))
			{
				uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), CommonStrings::None, currItem->lineColor(), !m_MouseButtonPressed);
				PageItem *bb = Doc->Items->at(z);
				bb->PoLine = currItem->PoLine.copy();
				bb->ClipEdited = true;
				bb->FrameType = 3;
				bb->setRotation(currItem->rotation());
				bb->setPolyClip(qRound(QMAX(bb->lineWidth() / 2, 1)));
				AdjustItemSize(bb);
				newGroupedItems.append(bb);
			}
			if (newGroupedItems.count() > 1)
			{
				for (uint ag = 0; ag < newGroupedItems.count(); ++ag)
				{
					bb = newGroupedItems.at(ag);
					bb->Groups.push(Doc->GroupCounter);
				}
			}
			Doc->GroupCounter++;
			delItems.append(Doc->selection->takeItem(offset));
		}

		uint toDeleteItemCount=delItems.count();
		if (toDeleteItemCount != 0)
		{
			Doc->selection->clear();
			for(uint i=0; i<toDeleteItemCount; ++i)
				Doc->selection->addItem(delItems.take(0)); //yes, 0, remove the first
			Doc->itemSelection_DeleteItem();
		}
	}
}

void ScribusView::UniteObj()
{
	PageItem *currItem;
	PageItem *bb;
	QValueList<int> toDel;
	toDel.clear();
	uint docSelectionCount=Doc->selection->count();
	if (docSelectionCount > 1)
	{
		currItem = Doc->selection->itemAt(0);
		currItem->Frame = false;
		currItem->ClipEdited = true;
		currItem->Groups.clear();
		for (uint a = 1; a < docSelectionCount; ++a)
		{
			bb = Doc->selection->itemAt(a);
			bb->Groups.clear();
			toDel.append(bb->ItemNr);
			QWMatrix ma;
			ma.translate(bb->xPos(), bb->yPos());
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QWMatrix ma2;
			ma2.translate(currItem->xPos(), currItem->yPos());
			ma2.rotate(currItem->rotation());
			ma2 = ma2.invert();
			bb->PoLine.map(ma2);
			currItem->PoLine.setMarker();
			currItem->PoLine.putPoints(currItem->PoLine.size(), bb->PoLine.size(), bb->PoLine);
		}
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
		AdjustItemSize(currItem);
		currItem->ContourLine = currItem->PoLine.copy();
		Deselect(true);
		for (uint c = 0; c < toDel.count(); ++c)
			SelectItemNr(*toDel.at(c));
		Doc->itemSelection_DeleteItem();
		updateContents();
	}
}

void ScribusView::SplitObj()
{
	PageItem *bb;
	uint StartInd = 0;
	uint z;
	PageItem *currItem = Doc->selection->itemAt(0);
	uint EndInd = currItem->PoLine.size();
	for (uint a = EndInd-1; a > 0; --a)
	{
		if (currItem->PoLine.point(a).x() > 900000)
		{
			StartInd = a + 1;
			z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), !m_MouseButtonPressed);
			bb = Doc->Items->at(z);
			bb->PoLine.resize(0);
			bb->PoLine.putPoints(0, EndInd - StartInd, currItem->PoLine, StartInd);
			bb->setRotation(currItem->rotation());
			AdjustItemSize(bb);
			bb->ContourLine = bb->PoLine.copy();
			bb->ClipEdited = true;
			a -= 3;
			EndInd = StartInd - 4;
		}
	}
	currItem->PoLine.resize(StartInd-4);
	AdjustItemSize(currItem);
	currItem->ContourLine = currItem->PoLine.copy();
	currItem->ClipEdited = true;
	Deselect(true);
	updateContents();
}

void ScribusView::contentsWheelEvent(QWheelEvent *w)
{
	QScrollView::contentsWheelEvent(w);
	evSpon = true;
	if ((m_MouseButtonPressed) && (MidButt) || ( w->state() & ControlButton ))
	{
		w->delta() > 0 ? slotZoomIn() : slotZoomOut();
	}
	else
	{
		int dX=0,dY=0;
		int moveBy=(w->delta() < 0) ? Prefs->Wheelval : -Prefs->Wheelval;
		if ((w->orientation() != Qt::Vertical) || ( w->state() & ShiftButton ))
			dX = moveBy;
		else
			dY = moveBy;
		scrollBy(dX, dY);
	}
	w->accept();
}

void ScribusView::setObjectUndoMode()
{
	_isGlobalMode = undoManager->isGlobalMode();
	if (ScMW->HaveDoc)
	{
		ScMW->scrActions["editActionMode"]->setOn(true);
		uint docSelectionCount=Doc->selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(Doc->selection->itemAt(0)->getUId());
		else if (docSelectionCount > 1)
			undoManager->showObject(Um::NO_UNDO_STACK);
		else if (docSelectionCount == 0)
			undoManager->showObject(Doc->currentPage->getUId());
	}
}

void ScribusView::setGlobalUndoMode()
{
	if (ScMW->HaveDoc)
	{
		ScMW->scrActions["editActionMode"]->setOn(!_isGlobalMode);
		if (_isGlobalMode)
			undoManager->showObject(Um::GLOBAL_UNDO_MODE);
		else
		{
			uint docSelectionCount=Doc->selection->count();
			if (docSelectionCount == 1)
				undoManager->showObject(Doc->selection->itemAt(0)->getUId());
			else if (docSelectionCount > 1)
				undoManager->showObject(Um::NO_UNDO_STACK);
			else if (docSelectionCount == 0)
				undoManager->showObject(Doc->currentPage->getUId());
		}
	}
}

void ScribusView::unitChange()
{
	vertRuler->unitChange();
	horizRuler->unitChange();
}

void ScribusView::setRulersShown(bool isShown)
{
	vertRuler->setShown(isShown);
	horizRuler->setShown(isShown);
	rulerMover->setShown(isShown);
	int newTopLeftMargin = isShown ? m_vhRulerHW : 0;
	setMargins(newTopLeftMargin, newTopLeftMargin, 0, 0);
}

void ScribusView::slotUpdateContents()
{
	updateContents();
}

void ScribusView::slotUpdateContents(const QRect &r)
{
	updateContents(r);
}

void ScribusView::setScale(const double newScale)
{
	Scale=newScale;
	if (Scale < Doc->toolSettings.magMin*Prefs->DisScale/100.0)
		Scale=Doc->toolSettings.magMin*Prefs->DisScale/100.0;

	if (Scale > Doc->toolSettings.magMax*Prefs->DisScale/100.0)
		Scale=Doc->toolSettings.magMax*Prefs->DisScale/100.0;

	if (Scale > 32*Prefs->DisScale)
		Scale=32*Prefs->DisScale;

	unitChange();
}

void ScribusView::drawLinkFrameLine(ScPainter* painter, FPoint &start, FPoint &end)
{
//CB FIXME Add some checking that the painter is setup?
	Q_ASSERT(painter!=NULL);
	painter->setPen(black, 1.0 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	painter->setPenOpacity(1.0);
	painter->drawLine(start, end);
	QWMatrix arrowTrans;
	arrowTrans.translate(end.x(), end.y());
	double r = atan2(end.y()-start.y(), end.x()-start.x())*(180.0/M_PI);
	arrowTrans.rotate(r);
	double sc = 0.8 / Scale;
	arrowTrans.scale(sc, sc);
	FPointArray arrow;
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.addQuadPoint(-15, -5, -15, -5, -15, -5, -15, -5);
	arrow.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
	arrow.addQuadPoint(-15, 5, -15, 5, -15, 5, -15, 5);
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.map(arrowTrans);
	painter->setBrush(painter->pen());
	painter->setBrushOpacity(1.0);
	painter->setLineWidth(0);
	painter->setFillMode(ScPainter::Solid);
	painter->setupPolygon(&arrow);
	painter->fillPath();
}
