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

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

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
#include <unistd.h>
#include <qcursor.h>
#include <qurl.h>
#include <qdir.h>
#include <qevent.h>
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
#include "serializer.h"
#include "insertTable.h"
#include "hruler.h"
#include "vruler.h"
#include "filewatcher.h"
#include "undomanager.h"
#include "units.h"
#include "extimageprops.h"
#include "pageitemattributes.h"
#include "scpaths.h"
#include "actionmanager.h"
#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif
#ifdef HAVE_CMS
	#include CMS_INC
#endif

using namespace std;

extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern double Cwidth(ScribusDoc *doc, Foi* name, QString ch, int Siz, QString ch2 = " ");
extern ScribusApp* ScApp;
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern QPixmap loadIcon(QString nam);
extern double xy2Deg(double x, double y);
extern void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
extern FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
extern FPoint getMaxClipF(FPointArray* Clip);
extern FPoint getMinClipF(FPointArray* Clip);
extern void WordAndPara(PageItem *currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN);
extern int callGS(const QStringList & args);
extern ProfilesL InputProfiles;

ScribusView::ScribusView(QWidget *parent, ScribusDoc *doc, ApplicationPrefs *prefs) : QScrollView(parent, "s", WRepaintNoErase | WNorthWestGravity)
{
	Ready = false;
	updateOn = true;
	Doc = doc;
	Doc->pageCount = 0;
	Prefs = prefs;
	setHScrollBarMode(QScrollView::AlwaysOn);
	setVScrollBarMode(QScrollView::AlwaysOn);
	setMargins(25, 25, 0, 0);
	setResizePolicy(Manual);
	viewport()->setBackgroundMode(PaletteBackground);
	QFont fo = QFont(font());
	fo.setPointSize(10);
	LE = new MSpinBox( 10, 3200, this, 2 );
	LE->setFont(fo);
	LE->setSuffix( tr( " %" ) );
	LE->setValue( 100 );
	LE->setFocusPolicy(QWidget::ClickFocus);
	zoomOutToolbarButton = new QPushButton(this);
	zoomOutToolbarButton->setPixmap(loadIcon("Klein.xpm"));
	zoomOutToolbarButton->setFocusPolicy(QWidget::NoFocus);
	zoomInToolbarButton = new QPushButton(this);
	zoomInToolbarButton->setFocusPolicy(QWidget::NoFocus);
	zoomInToolbarButton->setPixmap(loadIcon("Gross.xpm"));
	PGS = new PageSelector(this, 1);
	PGS->setFont(fo);
	PGS->setFocusPolicy(QWidget::ClickFocus);
	LY = new QPushButton(this);
	LY->setFont(fo);
	Laymen = new QPopupMenu(this);
	Laymen->setFont(fo);
	LY->setText( tr("Layer")+" 0");
	LY->setPopup(Laymen);
	LY->setFocusPolicy(QWidget::NoFocus);
	horizRuler = new Hruler(this, Doc);
	vertRuler = new Vruler(this, Doc);
	UN = new QToolButton(this);
	Unitmen = new QPopupMenu(this);
	//CB TODO Convert to actions later
	for (int i=0;i<=unitGetMaxIndex();++i)
		Unitmen->insertItem(unitGetStrFromIndex(i));
	UN->setPopup(Unitmen);
	UN->setFocusPolicy(QWidget::NoFocus);
	UN->setPopupDelay(10);
	UN->setText(unitGetStrFromIndex(doc->docUnitIndex));
	Ready = true;
	viewport()->setMouseTracking(true);
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setDragAutoScroll(false);
	SelItem.clear();
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Imoved = false;
	Mpressed = false;
	MidButt = false;
	HaveSelRect = false;
	Magnify = false;
	FirstPoly = true;
	EdPoints = true;
	GroupSel = false;
	DraggedGroup = false;
	MoveGY = false;
	MoveGX = false;
	EditContour = false;
	CursVis = false;
	mCG = false;
	MidButt = false;
	previewMode = false;
	Doc->SubMode = -1;
	GroupX = 0;
	GroupY = 0;
	GroupW = 0;
	GroupH = 0;
	DrHY = -1;
	DrVX = -1;
	GyM = 0;
	GxM = 0;
	ClRe = -1;
	ClRe2 = -1;
	_groupTransactionStarted = false;
	_itemCreationTransactionStarted = false;
	_isGlobalMode = true;
	undoManager = UndoManager::instance();
	connect(zoomOutToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
	connect(zoomInToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
	connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
	connect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	connect(Laymen, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	connect(Unitmen, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
	connect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
	connect(this, SIGNAL(HaveSel(int)), this, SLOT(selectionChanged()));
	evSpon = false;
}

void ScribusView::viewportPaintEvent ( QPaintEvent * p )
{
	if (p->spontaneous())
		evSpon = true;
	QScrollView::viewportPaintEvent(p);
}

void ScribusView::drawContents(QPainter *, int clipx, int clipy, int clipw, int cliph)
{
	QPoint vr;
	if (Doc->loading)
		return;
	if (!updateOn)
		return;
//	QTime tim;
//	tim.start();
	if ((clipw > 0) && (cliph > 0))
	{
		vr = contentsToViewport(QPoint(clipx, clipy));
		ScPainter *painter = new ScPainter(viewport(), clipw, cliph, vr.x(), vr.y());
		painter->clear(paletteBackgroundColor());
		painter->translate(-clipx, -clipy);
		painter->setLineWidth(1);
		painter->setFillMode(ScPainter::Solid);
		painter->translate(0.5, 0.5);
		painter->setZoomFactor(1.0);
/* Draw Page Outlines */
		if (!Doc->MasterP)
		{
			for (uint a = 0; a < Doc->Pages.count(); ++a)
			{
				int x = static_cast<int>(Doc->Pages.at(a)->Xoffset * Scale);
				int y = static_cast<int>(Doc->Pages.at(a)->Yoffset * Scale);
				int w = static_cast<int>(Doc->Pages.at(a)->Width * Scale);
				int h = static_cast<int>(Doc->Pages.at(a)->Height * Scale);
				if (QRect(x, y, w+5, h+5).intersects(QRect(clipx, clipy, clipw, cliph)))
				{
					painter->setFillMode(ScPainter::Solid);
					painter->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
					painter->setBrush(QColor(128,128,128));
					painter->drawRect(x+5, y+5, w, h);
					if (a == Doc->currentPage->PageNr)
						painter->setPen(red, 2, SolidLine, FlatCap, MiterJoin);
					else
						painter->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
					painter->setBrush(Doc->papColor);
					painter->drawRect(x, y, w, h);
					if (Doc->guidesSettings.before)
						DrawPageMarks(painter, Doc->Pages.at(a), QRect(clipx, clipy, clipw, cliph));
				}
				DrawMasterItems(painter, Doc->Pages.at(a), QRect(clipx, clipy, clipw, cliph));
			}
			DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
			if (!Doc->guidesSettings.before)
			{
				for (uint a = 0; a < Doc->Pages.count(); ++a)
				{
					int x = static_cast<int>(Doc->Pages.at(a)->Xoffset * Scale);
					int y = static_cast<int>(Doc->Pages.at(a)->Yoffset * Scale);
					int w = static_cast<int>(Doc->Pages.at(a)->Width * Scale);
					int h = static_cast<int>(Doc->Pages.at(a)->Height * Scale);
					if (QRect(x, y, w+5, h+5).intersects(QRect(clipx, clipy, clipw, cliph)))
						DrawPageMarks(painter, Doc->Pages.at(a), QRect(clipx, clipy, clipw, cliph));
				}
			}
		}
		else
		{
			int x = static_cast<int>(Doc->ScratchLeft * Scale);
			int y = static_cast<int>(Doc->ScratchTop * Scale);
			int w = static_cast<int>(Doc->currentPage->Width * Scale);
			int h = static_cast<int>(Doc->currentPage->Height * Scale);
			if (QRect(x, y, w+5, h+5).intersects(QRect(clipx, clipy, clipw, cliph)))
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
			if ((!Doc->guidesSettings.before) && (QRect(x, y, w+5, h+5).intersects(QRect(clipx, clipy, clipw, cliph))))
				DrawPageMarks(painter, Doc->currentPage, QRect(clipx, clipy, clipw, cliph));
		}
		if ((SelItem.count() != 0) || (linkedFramesToShow.count() != 0))
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
					currItem = SelItem.at(0);
			}
			if ((((Doc->appMode == LinkFrames) || (Doc->appMode == UnlinkFrames))
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
							double x11, y11, x12, y12, x1mid, y1mid;
							double x21, y21, x22, y22, x2mid, y2mid;
							x11 = nextItem->Xpos;
							y11 = nextItem->Ypos;
							x12 = nextItem->Xpos+nextItem->Width;
							y12 = nextItem->Ypos+nextItem->Height;
							x1mid = x11+(x12-x11)/2;
							y1mid = y11+(y12-y11)/2;

							if (nextItem->Rot!=0.000)
							{
								FPoint tempPoint = transformPoint(FPoint(0,0), x11, y11, nextItem->Rot, 1, 1);
								x11=tempPoint.x();
								y11=tempPoint.y();
								tempPoint = transformPoint(FPoint(0,0), x12, y12, nextItem->Rot, 1, 1);
								x12=tempPoint.x();
								y12=tempPoint.y();
								tempPoint = transformPoint(FPoint(0,0), x1mid, y1mid, nextItem->Rot, 1, 1);
								x1mid=tempPoint.x();
								y1mid=tempPoint.y();
							}


							double a1, b1, a2, b2;
							a1 = a2 = b1 = b2 = 0;
							if (nextItem->NextBox!=NULL)
							{
								x21 = nextItem->NextBox->Xpos;
								y21 = nextItem->NextBox->Ypos;
								x22 = nextItem->NextBox->Xpos+nextItem->NextBox->Width;
								y22 = nextItem->NextBox->Ypos+nextItem->NextBox->Height;
								x2mid = nextItem->NextBox->Xpos + nextItem->NextBox->Width/2;
								y2mid = nextItem->NextBox->Ypos + nextItem->NextBox->Height/2;
								x2mid = x21+(x22-x21)/2;
								y2mid = y21+(y22-y21)/2;

								if (nextItem->NextBox->Rot!=0.000)
								{
									FPoint tempPoint = transformPoint(FPoint(0,0), x21, y21, nextItem->NextBox->Rot, 1, 1);
									x21=tempPoint.x();
									y21=tempPoint.y();
									tempPoint = transformPoint(FPoint(0,0), x22, y22, nextItem->NextBox->Rot, 1, 1);
									x22=tempPoint.x();
									y22=tempPoint.y();
									tempPoint = transformPoint(FPoint(0,0), x2mid, y2mid, nextItem->NextBox->Rot, 1, 1);
									x2mid=tempPoint.x();
									y2mid=tempPoint.y();
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

							FPoint Start = transformPoint(FPoint(a1-nextItem->Xpos, b1-nextItem->Ypos), nextItem->Xpos, nextItem->Ypos, nextItem->Rot, 1, 1);
							//FPoint Start = transformPoint(FPoint(nextItem->Width/2, nextItem->Height), nextItem->Xpos, nextItem->Ypos, nextItem->Rot, 1, 1);
							nextItem = nextItem->NextBox;
							if (nextItem != NULL)
							{
								FPoint End = transformPoint(FPoint(a2-nextItem->Xpos, b2-nextItem->Ypos), nextItem->Xpos, nextItem->Ypos, nextItem->Rot, 1, 1);
								//FPoint End = transformPoint(FPoint(nextItem->Width/2, 0), nextItem->Xpos, nextItem->Ypos, nextItem->Rot, 1, 1);
								painter->setPen(black, 5.0 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
								painter->setPenOpacity(0.3);
								painter->drawLine(Start, End);
								double r = atan2(End.y()-Start.y(),End.x()-Start.x())*(180.0/M_PI);
								QWMatrix arrowTrans;
								FPointArray arrow;
								arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
								arrow.addQuadPoint(-15, -5, -15, -5, -15, -5, -15, -5);
								arrow.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
								arrow.addQuadPoint(-15, 5, -15, 5, -15, 5, -15, 5);
								arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
								arrowTrans.translate(End.x(), End.y());
								arrowTrans.rotate(r);
								arrowTrans.scale(2.5 / Scale, 2.5 / Scale);
								arrow.map(arrowTrans);
								painter->setBrush(painter->pen());
								painter->setBrushOpacity(0.3);
								painter->setLineWidth(0);
								painter->setFillMode(ScPainter::Solid);
								painter->setupPolygon(&arrow);
								painter->fillPath();
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
						FPoint Start = transformPoint(FPoint(nextItem->Width/2, nextItem->Height), nextItem->Xpos, nextItem->Ypos, nextItem->Rot, 1, 1);
						nextItem = nextItem->NextBox;
						if (nextItem != 0)
						{
							FPoint End = transformPoint(FPoint(nextItem->Width/2, 0), nextItem->Xpos, nextItem->Ypos, nextItem->Rot, 1, 1);
							painter->setPen(black, 5.0 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
							painter->setPenOpacity(0.3);
							painter->drawLine(Start, End);
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
	}
	if (SelItem.count() != 0)
	{
		PageItem *currItem = SelItem.at(0);
		currItem->paintObj();
		if ((Doc->EditClip) && (currItem->Select))
			MarkClip(currItem);
		if (GroupSel)
		{
			setGroupRect();
			paintGroupRect();
		}
	}
	if (Doc->appMode == EditMode)
		slotDoCurs(true);
	if (Doc->appMode == EditGradientVectors)
	{
		PageItem *currItem = SelItem.at(0);
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
//	qDebug( "Time elapsed: %d ms", tim.elapsed() );
}

void ScribusView::DrawMasterItems(ScPainter *painter, Page *page, QRect clip)
{
	QPainter p;
	int Lnr;
	struct Layer ll;
	PageItem *currItem;
	ll.isViewable = false;
	ll.LNr = 0;
	double z = painter->zoomFactor();
	if (page->MPageNam != "")
	{
		Page* Mp = Doc->MasterPages.at(Doc->MasterNames[page->MPageNam]);
		if (page->FromMaster.count() != 0)
		{
			Lnr = 0;
			for (uint la = 0; la < Doc->Layers.count(); ++la)
			{
				Level2Layer(Doc, &ll, Lnr);
				bool pr = true;
				if ((previewMode) && (!ll.isPrintable))
					pr = false;
				if ((ll.isViewable) && (pr))
				{
					for (uint a = 0; a < page->FromMaster.count(); ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->PageNr)))
							continue;
						if ((previewMode) && (!currItem->isPrintable))
							continue;
						currItem->savedOwnPage = currItem->OwnPage;
						double OldX = currItem->Xpos;
						double OldY = currItem->Ypos;
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						currItem->OwnPage = page->PageNr;
						if (!currItem->ChangedMasterItem)
						{
							currItem->Xpos = OldX - Mp->Xoffset + page->Xoffset;
							currItem->Ypos = OldY - Mp->Yoffset + page->Yoffset;
							currItem->BoundingX = OldBX - Mp->Xoffset + page->Xoffset;
							currItem->BoundingY = OldBY - Mp->Yoffset + page->Yoffset;
						}
						if (evSpon)
							currItem->Dirty = true;
						QRect oldR = getRedrawBounding(currItem);
						if (clip.intersects(oldR))
							currItem->DrawObj(painter, clip);
						currItem->OwnPage = currItem->savedOwnPage;
						if (!currItem->ChangedMasterItem)
						{
							currItem->Xpos = OldX;
							currItem->Ypos = OldY;
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
					}
					for (uint a = 0; a < page->FromMaster.count(); ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if (!currItem->isTableItem)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->PageNr)))
							continue;
						double OldX = currItem->Xpos;
						double OldY = currItem->Ypos;
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						if (!currItem->ChangedMasterItem)
						{
							currItem->Xpos = OldX - Mp->Xoffset + page->Xoffset;
							currItem->Ypos = OldY - Mp->Yoffset + page->Yoffset;
							currItem->BoundingX = OldBX - Mp->Xoffset + page->Xoffset;
							currItem->BoundingY = OldBY - Mp->Yoffset + page->Yoffset;
						}
						QRect oldR = getRedrawBounding(currItem);
						if (clip.intersects(oldR))
						{
							painter->setZoomFactor(Scale);
							painter->save();
							painter->translate(currItem->Xpos*Scale, currItem->Ypos*Scale);
							painter->rotate(currItem->Rot);
							if (currItem->lineColor() != "None")
							{
								QColor tmp;
								currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
								if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
								{
									painter->setPen(tmp, currItem->Pwidth, currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
									if (currItem->TopLine)
										painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->Width, 0.0));
									if (currItem->RightLine)
										painter->drawLine(FPoint(currItem->Width, 0.0), FPoint(currItem->Width, currItem->Height));
									if (currItem->BottomLine)
										painter->drawLine(FPoint(currItem->Width, currItem->Height), FPoint(0.0, currItem->Height));
									if (currItem->LeftLine)
										painter->drawLine(FPoint(0.0, currItem->Height), FPoint(0.0, 0.0));
								}
							}
							painter->restore();
						}
						if (!currItem->ChangedMasterItem)
						{
							currItem->Xpos = OldX;
							currItem->Ypos = OldY;
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
	QPainter p;
	int Lnr;
	struct Layer ll;
	PageItem *currItem;
	ll.isViewable = false;
	ll.LNr = 0;
	linkedFramesToShow.clear();
	double z = painter->zoomFactor();
	if (Doc->Items.count() != 0)
	{
		Lnr = 0;
		for (uint la2 = 0; la2 < Doc->Layers.count(); ++la2)
		{
			Level2Layer(Doc, &ll, Lnr);
			bool pr = true;
			if ((previewMode) && (!ll.isPrintable))
				pr = false;
			if ((ll.isViewable) && (pr))
			{
				QPtrListIterator<PageItem> docItem(Doc->Items);
				 while ( (currItem = docItem.current()) != 0 )
				 {
        			++docItem;
					if (currItem->LayerNr != ll.LNr)
						continue;
					if ((previewMode) && (!currItem->isPrintable))
						continue;
					if ((Doc->MasterP) && ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Doc->currentPage->PageNr))))
						continue;
					if ((!Doc->MasterP) && (currItem->OnMasterPage != ""))
					{
						if (currItem->OnMasterPage != Doc->currentPage->PageNam)
							continue;
					}
					QRect oldR = getRedrawBounding(currItem);
					if (clip.intersects(oldR))
					{
						if (evSpon)
							currItem->Dirty = true;
						if (!((Doc->EditClip) && (Mpressed)))
							currItem->DrawObj(painter, clip);
						currItem->Redrawn = true;
						if ((currItem->itemType() == PageItem::TextFrame) && ((currItem->NextBox != 0) || (currItem->BackBox != 0)))
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
						if ((Doc->appMode == EditMode) && (currItem->Select) && (currItem->itemType() == PageItem::TextFrame))
						{
							//CB 230305 Stop redrawing the horizontal ruler if it hasnt changed when typing text!!!
							if ((qRound(horizRuler->ItemPos*10000) != qRound((currItem->Xpos - Doc->ScratchLeft)*10000)) || (qRound(horizRuler->ItemEndPos*10000) != qRound(((currItem->Xpos+currItem->Width) - Doc->ScratchLeft)*10000)))
							{
								horizRuler->ItemPos = currItem->Xpos - Doc->ScratchLeft;
								horizRuler->ItemEndPos = (currItem->Xpos+currItem->Width) - Doc->ScratchLeft;
								if (currItem->lineColor() != "None")
									horizRuler->lineCorr = currItem->Pwidth / 2.0;
								else
									horizRuler->lineCorr = 0;
								horizRuler->ColGap = currItem->ColGap;
								horizRuler->Cols = currItem->Cols;
								horizRuler->Extra = currItem->Extra;
								horizRuler->RExtra = currItem->RExtra;
								horizRuler->First = Doc->docParagraphStyles[Doc->currentParaStyle].First;
								horizRuler->Indent = Doc->docParagraphStyles[Doc->currentParaStyle].Indent;
								if (currItem->imageFlippedH() || (currItem->Reverse))
									horizRuler->Revers = true;
								else
									horizRuler->Revers = false;
								horizRuler->ItemPosValid = true;
								horizRuler->repX = false;
								if (Doc->currentParaStyle < 5)
									horizRuler->TabValues = currItem->TabValues;
								else
									horizRuler->TabValues = Doc->docParagraphStyles[Doc->currentParaStyle].TabValues;
								horizRuler->repaint();
							}
						}
					}
				}
				QPtrListIterator<PageItem> docItem2(Doc->Items);
				 while ( (currItem = docItem2.current()) != 0 )
				 {
        			++docItem2;
					if (currItem->LayerNr != ll.LNr)
						continue;
					if (!currItem->isTableItem)
						continue;
					QRect oldR = getRedrawBounding(currItem);
					if (clip.intersects(oldR))
					{
						painter->setZoomFactor(Scale);
						painter->save();
						painter->translate(currItem->Xpos*Scale, currItem->Ypos*Scale);
						painter->rotate(currItem->Rot);
						if (currItem->lineColor() != "None")
						{
							QColor tmp;
							currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
							if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
							{
								painter->setPen(tmp, currItem->Pwidth, currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
								if (currItem->TopLine)
									painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->Width, 0.0));
								if (currItem->RightLine)
									painter->drawLine(FPoint(currItem->Width, 0.0), FPoint(currItem->Width, currItem->Height));
								if (currItem->BottomLine)
									painter->drawLine(FPoint(currItem->Width, currItem->Height), FPoint(0.0, currItem->Height));
								if (currItem->LeftLine)
									painter->drawLine(FPoint(0.0, currItem->Height), FPoint(0.0, 0.0));
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
	double lw = 1.0 / Scale;
	double z = p->zoomFactor();
	p->save();
	p->setZoomFactor(Scale);
	p->translate(page->Xoffset * Scale, page->Yoffset * Scale);
	p->setLineWidth(lw);
	if (Doc->guidesSettings.marginsShown)
	{
		p->setPen(Doc->guidesSettings.margColor);
		if (Doc->marginColored)
		{
			p->setBrush(Doc->guidesSettings.margColor);
			p->drawRect(0, 0, page->Width, page->Margins.Top);
			p->drawRect(0, page->Margins.Top, page->Margins.Left, page->Height - page->Margins.Top);
			p->drawRect(page->Margins.Left, page->Height - page->Margins.Bottom, page->Width - page->Margins.Right - page->Margins.Left, page->Margins.Bottom);
			p->drawRect(page->Width - page->Margins.Right, page->Margins.Top, page->Margins.Right, page->Height-page->Margins.Top);
		}
		p->setPen(Doc->guidesSettings.margColor);
		p->drawLine(FPoint(0, page->Margins.Top), FPoint(page->Width, page->Margins.Top));
		p->drawLine(FPoint(0, page->Height - page->Margins.Bottom), FPoint(page->Width, page->Height - page->Margins.Bottom));
		p->drawLine(FPoint(page->Margins.Left, 0), FPoint(page->Margins.Left, page->Height));
		p->drawLine(FPoint(page->Width - page->Margins.Right, 0), FPoint(page->Width - page->Margins.Right, page->Height));
	}
	if (Doc->guidesSettings.baseShown)
	{
		p->setPen(Doc->guidesSettings.baseColor, lw, SolidLine, FlatCap, MiterJoin);
		for (double yg = Doc->typographicSetttings.offsetBaseGrid; yg < page->Height; yg += Doc->typographicSetttings.valueBaseGrid)
			p->drawLine(FPoint(0, yg), FPoint(page->Width, yg));
	}
	if (Doc->guidesSettings.gridShown)
	{
		double stx = 0;
		double endx = page->Width;
		double sty = 0;
		double endy = page->Height;
/*		double stx = QMAX((clip.x() - page->Xoffset) / Scale, 0);
		double endx = QMIN(stx + clip.width() / Scale, page->Width);
		double sty = QMAX((clip.y() - page->Yoffset) / Scale, 0);
		double endy = QMIN(sty + clip.height() / Scale, page->Height); */
		if (Scale > 0.49)
		{
			double i,start;
			i = Doc->guidesSettings.majorGrid;
			p->setPen(Doc->guidesSettings.majorColor, lw, SolidLine, FlatCap, MiterJoin);
			start=floor(sty/i);
			start*=i;
			for (double b = start; b < endy; b+=i)
			{
				p->drawLine(FPoint(0, b), FPoint(page->Width, b));
			}
			start=floor(stx/i);
			start*=i;
			for (double b = start; b <= endx; b+=i)
			{
				p->drawLine(FPoint(b, 0), FPoint(b, page->Height));
			}
			i = Doc->guidesSettings.minorGrid;
			p->setPen(Doc->guidesSettings.minorColor, lw, DotLine, FlatCap, MiterJoin);
			start=floor(sty/i);
			start*=i;
			for (double b = start; b < endy; b+=i)
			{
				p->drawLine(FPoint(0, b), FPoint(page->Width, b));
			}
			start=floor(stx/i);
			start*=i;
			for (double b = start; b <= endx; b+=i)
			{
				p->drawLine(FPoint(b, 0), FPoint(b, page->Height));
			}
		}
	}
	if (Doc->guidesSettings.guidesShown)
	{
		p->setPen(Doc->guidesSettings.guideColor, lw, DotLine, FlatCap, MiterJoin);
		if (page->XGuides.count() != 0)
		{
			for (uint xg = 0; xg < page->XGuides.count(); ++xg)
				p->drawLine(FPoint(page->XGuides[xg], 0), FPoint(page->XGuides[xg], page->Height));
		}
		if (page->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < page->YGuides.count(); ++yg)
				p->drawLine(FPoint(0, page->YGuides[yg]), FPoint(page->Width, page->YGuides[yg]));
		}
	}
	p->restore();
	p->setZoomFactor(z);
}

void ScribusView::leaveEvent(QEvent *)
{
/*	if (BlockLeave)
		return; */
	if (!Mpressed)
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
/*	else
	{
		if ((SelItem.count() != 0) && (Mpressed) && (!doku->DragP) && (doku->appMode == 1))
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
			doku->DragP = false;
			doku->leaveDrag = false;
			Mpressed = false;
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
		ScApp->newActWin(Doc->WinHan);
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
			GroupSel = false;
			QPainter p;
			p.begin(viewport());
			PaintSizeRect(&p, QRect());
//			QPoint pv = QPoint(qRound(gx), qRound(gy));
//			PaintSizeRect(&p, QRect(pv, QPoint(pv.x()+qRound(gw), pv.y()+qRound(gh))));
			p.end();
		}
		delete ss;
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
			QPoint pv = QPoint(qRound(gx), qRound(gy));
			if (!DraggedGroupFirst)
				PaintSizeRect(&p, QRect(pv, QPoint(pv.x()+qRound(gw), pv.y()+qRound(gh))));
			DraggedGroupFirst = false;
			p.end();
			emit MousePos(GroupX-Doc->currentPage->Xoffset, GroupY-Doc->currentPage->Yoffset);
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
	if (QTextDrag::decode(e, text))
	{
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).upper();
		QStrList imfo = QImageIO::inputFormats();
		if (ext == "JPG")
			ext = "JPEG";
		img = ((imfo.contains(ext))||(ext=="PS")||(ext=="EPS")||(ext=="PDF")||(ext=="TIF"));
/*		if ((fi.exists()) && (img) && (!SeleItemPos(e->pos())))
		{
			int z = PaintPict(qRound(e->pos().x()/doku->Scale), qRound(e->pos().y()/doku->Scale), 1, 1);
			b = Items.at(z);
			LoadPict(ur.path(), b->ItemNr);
			b->Width = static_cast<double>(b->pixm.width());
			b->Height = static_cast<double>(b->pixm.height());
			b->OldB2 = b->Width;
			b->OldH2 = b->Height;
			UpdateClip(b);
			emit DocChanged();
			update();
			return;
		} */
/*		if ((SeleItemPos(e->pos())) && (!text.startsWith("<SCRIBUSELEM")))
		{
			b = SelItem.at(0);
			if (b->itemType() == PageItem::ImageFrame)
			{
				if ((fi.exists()) && (img))
				{
					LoadPict(ur.path(), b->ItemNr);
					update();
				}
			}
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
			}
		}
		else
		{ */
			for (uint as = 0; as < Doc->Items.count(); ++as)
			{
				Doc->Items.at(as)->Select = false;
			}
			uint ac = Doc->Items.count();
			if ((!img) && (Doc->DraggedElem == 0))
			{
				if ((fi.exists()) && (!img))
					emit LoadElem(ur.path(), qRound(e->pos().x()/Scale), qRound(e->pos().y()/Scale), true, false, Doc, this);
				else
					emit LoadElem(QString(text), qRound(e->pos().x()/Scale), qRound(e->pos().y()/Scale), false, false, Doc, this);
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
						emit LoadElem(QString(text), qRound(e->pos().x()/Scale), qRound(e->pos().y()/Scale), false, false, Doc, this);
						for (uint as = ac; as < Doc->Items.count(); ++as)
						{
							pasted.append(Doc->Items.at(as));
						}
						SelItem.clear();
						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							SelItem.append(Doc->Items.at(Doc->DragElements[dre]));
						}
						PageItem* bb;
						int fin;
						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							bb = pasted.at(dre);
							currItem = SelItem.at(dre);
							if ((currItem->itemType() == PageItem::TextFrame) && ((currItem->NextBox != 0) || (currItem->BackBox != 0)))
							{
								if (currItem->BackBox != 0)
								{
									bb->BackBox = currItem->BackBox;
									fin = SelItem.find(currItem->BackBox);
									if (fin != -1)
										bb->BackBox = pasted.at(fin);
									bb->BackBox->NextBox = bb;
								}
								if (currItem->NextBox != 0)
								{
									bb->NextBox = currItem->NextBox;
									fin = SelItem.find(currItem->NextBox);
									if (fin != -1)
										bb->NextBox = pasted.at(fin);
									bb->NextBox->BackBox = bb;
								}
							}
						}
						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							currItem = SelItem.at(dre);
							currItem->NextBox = 0;
							currItem->BackBox = 0;
						}
						pasted.clear();
						DeleteItem();
					}
				}
				if ((!img) && ((re == 0)))
					emit LoadElem(QString(text), qRound(e->pos().x()/Scale), qRound(e->pos().y()/Scale), false, false, Doc, this);
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
				SelItem.clear();
				for (uint as = ac; as < Doc->Items.count(); ++as)
				{
					SelectItemNr(as);
				}
				updateContents();
			}
//		}
	}
}

void ScribusView::contentsMouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	Mpressed = false;
	PageItem *currItem = 0;
	if (Doc->EditClip)
	{
		emit EndNodeEdit();
		return;
	}
	if ((GroupSel) || (Doc->appMode != NormalMode))
	{
		if ((GroupSel) && (Doc->appMode == NormalMode))
		{
			if (GetItem(&currItem))
			{
				if (currItem->isTableItem)
				{
					Deselect(false);
					SelItem.append(currItem);
					currItem->isSingleSel = true;
					currItem->Select = true;
					emit HaveSel(currItem->itemType());
					EmitValues(currItem);
					currItem->paintObj();
				}
			}
		}
		else
			contentsMousePressEvent(m);
		return;
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
			emit Amode(EditMode);
		}
		else
			if (currItem->itemType() == PageItem::TextFrame)
			{
				emit currItem->isAnnotation ? AnnotProps() : Amode(EditMode);
				contentsMousePressEvent(m);
			}
	}
}

void ScribusView::contentsMouseReleaseEvent(QMouseEvent *m)
{
	PageItem *currItem;
	Mpressed = false;
	if (Doc->guidesSettings.guidesShown)
	{
		bool fg = false;
		double nx = m->x()/Scale;
		double ny = m->y()/Scale;
		if (Doc->currentPage->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < Doc->currentPage->YGuides.count(); ++yg)
			{
				if ((Doc->currentPage->YGuides[yg]+Doc->currentPage->Yoffset < (ny+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->YGuides[yg]+Doc->currentPage->Yoffset > (ny-Doc->guidesSettings.grabRad)))
				{
					fg = true;
					break;
				}
			}
		}
		if (Doc->currentPage->XGuides.count() != 0)
		{
			for (uint xg = 0; xg < Doc->currentPage->XGuides.count(); ++xg)
			{
				if ((Doc->currentPage->XGuides[xg]+Doc->currentPage->Xoffset < (nx+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->XGuides[xg]+Doc->currentPage->Xoffset > (nx-Doc->guidesSettings.grabRad)))
				{
					fg = true;
					break;
				}
			}
		}
		if ((fg) && (m->button() == RightButton) && (!GetItem(&currItem)))
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
			return;
		}
		if (MoveGX)
		{
			SetXGuide(m, GxM);
			MoveGX = false;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			updateContents();
			return;
		}
	}
	if (Doc->appMode == EditGradientVectors)
		return;
	if (Doc->appMode == CopyProperties)
		return;
	if (Doc->appMode == MeasurementTool)
	{
		QPainter p;
		p.begin(viewport());
		ToView(&p);
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.drawLine(Dxp, Dyp, Mxp, Myp);
		p.end();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		//emit PaintingDone();
		return;
	}
	if (Doc->appMode == PanningMode)
		return;
	if (Doc->appMode == DrawTable)
	{
		if ((SelItem.count() == 0) && (HaveSelRect) && (!MidButt))
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
				Doc->appMode = NormalMode;
				emit PaintingDone();
				return;
			}
			InsertTable *dia = new InsertTable(this, static_cast<int>(Th / 6), static_cast<int>(Tw / 6));
			if (!dia->exec())
			{
				p.drawRect(AreaR);
				p.end();
				Doc->appMode = NormalMode;
				emit PaintingDone();
				delete dia;
				return;
			}
			p.end();
			Cols = dia->Cols->value();
			Rows = dia->Rows->value();
			delete dia;
			deltaX = Tw / Cols;
			deltaY = Th / Rows;
			offX = 0.0;
			offY = 0.0;
			SelItem.clear();
			if (UndoManager::undoEnabled())
				undoManager->beginTransaction(Doc->currentPage->getUName(), Um::ITable, Um::CreateTable,
											  QString(Um::RowsCols).arg(Rows).arg(Cols), Um::ICreate);
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					z = PaintText(Tx + offX, Ty + offY, deltaX, deltaY, Doc->toolSettings.dWidth, Doc->toolSettings.dPenText);
					currItem = Doc->Items.at(z);
					currItem->isTableItem = true;
					SelItem.append(currItem);
					offX += deltaX;
				}
				offY += deltaY;
				offX = 0.0;
			}
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					currItem = SelItem.at((rc * Cols) + cc);
					if (rc == 0)
						currItem->TopLink = 0;
					else
						currItem->TopLink = SelItem.at(((rc-1)*Cols)+cc);
					if (rc == Rows-1)
						currItem->BottomLink = 0;
					else
						currItem->BottomLink = SelItem.at(((rc+1)*Cols)+cc);
					if (cc == 0)
						currItem->LeftLink = 0;
					else
						currItem->LeftLink = SelItem.at((rc*Cols)+cc-1);
					if (cc == Cols-1)
						currItem->RightLink = 0;
					else
						currItem->RightLink = SelItem.at((rc*Cols)+cc+1);
				}
			}
			emit DoGroup();
			if (UndoManager::undoEnabled())
				undoManager->commit();
		}
		Doc->appMode = NormalMode;
		emit PaintingDone();
		emit DocChanged();
		updateContents();
		return;
	}
	if (Doc->appMode == DrawFreehandLine)
	{
		Doc->appMode = NormalMode;
		if (RecordP.size() > 1)
		{
			uint z = PaintPolyLine(0, 0, 1, 1, Doc->toolSettings.dWidth, "None", Doc->toolSettings.dPenLine);
			currItem = Doc->Items.at(z);
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
			SelItem.clear();
			SelItem.append(currItem);
			currItem->ClipEdited = true;
			currItem->Select = true;
			currItem->FrameType = 3;
			currItem->OwnPage = OnPage(currItem);
			emit ItemPos(currItem->Xpos, currItem->Ypos);
			emit SetSizeValue(currItem->Pwidth);
			emit SetLineArt(currItem->PLineArt, currItem->PLineEnd, currItem->PLineJoin);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
			emit ItemGradient(currItem->GrType);
			emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
			emit HaveSel(PageItem::PolyLine);
		}
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
		currItem = SelItem.at(0);
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
		MarkClip(currItem);
		return;
	}
	if ((Doc->EditClip) && (SegP1 == -1) && (SegP2 == -1))
	{
		if (Imoved)
		{
			currItem = SelItem.at(0);
			currItem->OldB2 = currItem->Width;
			currItem->OldH2 = currItem->Height;
			double nx = m->x()/Scale;
			double ny = m->y()/Scale;
			if (!ApplyGuides(&nx, &ny))
			{
				FPoint npg = ApplyGridF(FPoint(nx, ny));
				nx = npg.x();
				ny = npg.y();
			}
			FPoint np = transformPointI(FPoint(nx, ny), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
			MoveClipPoint(currItem, np);
		}
		updateContents();
		Imoved = false;
		return;
	}
	if ((Doc->EditClip) && (SegP1 != -1) && (SegP2 != -1))
	{
		SegP1 = -1;
		SegP2 = -1;
		currItem = SelItem.at(0);
		Imoved = false;
		updateContents();
		return;
	}
	if ((!GetItem(&currItem)) && (m->button() == RightButton) && (!Doc->DragP) && (Doc->appMode == NormalMode))
	{
		QPopupMenu *pmen = new QPopupMenu();
		if (ScApp->Buffer2.startsWith("<SCRIBUSELEM"))
		{
			Mxp = m->x();
			Myp = m->y();
			pmen->insertItem( tr("&Paste") , this, SLOT(PasteToPage()));
			pmen->insertSeparator();
		}
		setObjectUndoMode();
		pmen->insertSeparator();
		ScApp->scrActions["editUndoAction"]->addTo(pmen);
		ScApp->scrActions["editRedoAction"]->addTo(pmen);
		pmen->insertSeparator();
		ScApp->scrActions["viewShowMargins"]->addTo(pmen);
		ScApp->scrActions["viewShowFrames"]->addTo(pmen);
		ScApp->scrActions["viewShowImages"]->addTo(pmen);
		ScApp->scrActions["viewShowGrid"]->addTo(pmen);
		ScApp->scrActions["viewShowGuides"]->addTo(pmen);
		ScApp->scrActions["viewShowBaseline"]->addTo(pmen);
		ScApp->scrActions["viewShowTextChain"]->addTo(pmen);
		pmen->insertSeparator();
		ScApp->scrActions["viewSnapToGrid"]->addTo(pmen);
		ScApp->scrActions["viewSnapToGuides"]->addTo(pmen);
		pmen->exec(QCursor::pos());
		setGlobalUndoMode();
		delete pmen;
		return;
	}
	if ((Doc->appMode != Magnifier) && (!Doc->EditClip) && (Doc->appMode != DrawBezierLine))
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
					CharC->setText(txtC.setNum(qRound(72.0 / currItem->LocalScX))+" x "+
					               txtC2.setNum(qRound(72.0 / currItem->LocalScY)));
					InfoGroupLayout->addWidget( CharC, 3, 1 );
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
				if (currItem->isPrintable == true)
					PrintC->setText( tr("Enabled"));
				else
					PrintC->setText( tr("Disabled"));
				InfoGroupLayout->addWidget( PrintC, row, 1 ); // </a.l.e>

				pmen4->insertItem(InfoGroup);
				pmen->insertItem( tr("In&fo"), pmen4);
			}
			pmen->insertSeparator();
			ScApp->scrActions["editUndoAction"]->addTo(pmen);
			ScApp->scrActions["editRedoAction"]->addTo(pmen);
			pmen->insertSeparator();
			ScApp->scrActions["itemAttributes"]->addTo(pmen);
			if (currItem->itemType() == PageItem::ImageFrame)
			{
				ScApp->scrActions["fileImportImage"]->addTo(pmen);
				ScApp->scrActions["itemImageIsVisible"]->addTo(pmen);
				pmenResolution->insertItem( tr("Low Resolution"));
				pmenResolution->insertItem( tr("Normal Resolution"));
				pmenResolution->insertItem( tr("Full Resolution"));
				pmen->insertItem( tr("Preview Settings"), pmenResolution);
				pmenResolution->setItemChecked(pmenResolution->idAt(2 - currItem->pixm.imgInfo.lowResType), true);
				connect(pmenResolution, SIGNAL(activated(int)), this, SLOT(changePreview(int)));
				if ((currItem->PicAvail) && (currItem->pixm.imgInfo.valid))
					pmen->insertItem( tr("Extended Image Properties"), this, SLOT(useEmbeddedPath()));
				if (currItem->PicAvail)
					pmen->insertItem( tr("&Update Picture"), this, SLOT(UpdatePic()));
				if (currItem->PicAvail && currItem->isRaster)
					pmen->insertItem( tr("&Edit Picture"), this, SIGNAL(callGimp()));
				if ((currItem->PicAvail) && (!currItem->isTableItem))
					pmen->insertItem( tr("&Adjust Frame to Picture"), this, SLOT(FrameToPic()));
			}
			if (currItem->itemType() == PageItem::TextFrame)
			{
				ScApp->scrActions["fileImportText"]->addTo(pmen);
				ScApp->scrActions["fileImportAppendText"]->addTo(pmen);
				ScApp->scrActions["toolsEditWithStoryEditor"]->addTo(pmen);
				if (Doc->currentPage->PageNam == "")
				{
					ScApp->scrActions["itemPDFIsAnnotation"]->addTo(pmenPDF);
					ScApp->scrActions["itemPDFIsBookmark"]->addTo(pmenPDF);
					if (currItem->isAnnotation)
					{
						if ((currItem->AnType == 0) || (currItem->AnType == 1) || (currItem->AnType > 9))
							ScApp->scrActions["itemPDFAnnotationProps"]->addTo(pmenPDF);
						else
							ScApp->scrActions["itemPDFFieldProps"]->addTo(pmenPDF);
					}
				}
				pmen->insertItem( tr("&PDF Options"), pmenPDF);
			}
			if (currItem->itemType() == PageItem::PathText)
				ScApp->scrActions["toolsEditWithStoryEditor"]->addTo(pmen);
			ScApp->scrActions["itemLock"]->addTo(pmen);
			ScApp->scrActions["itemLockSize"]->addTo(pmen);
			if (!currItem->isSingleSel)
			{
				ScApp->scrActions["itemSendToScrapbook"]->addTo(pmen);
				if (Doc->Layers.count() > 1)
				{
					for (uint lam=0; lam < Doc->Layers.count(); ++lam)
					{
						int lai = pmen3->insertItem(Doc->Layers[lam].Name);
						if (Doc->Layers[lam].LNr == Doc->ActiveLayer)
							pmen3->setItemEnabled(lai, 0);
					}
					pmen->insertItem( tr("Send to La&yer"), pmen3);
				}
				connect(pmen3, SIGNAL(activated(int)), this, SLOT(sentToLayer(int)));
			}
			if (!currItem->locked())
			{
				if (SelItem.count() > 1)
				{
					bool isGroup = true;
					int firstElem = -1;
					if (currItem->Groups.count() != 0)
						firstElem = currItem->Groups.top();
					for (uint bx = 0; bx < SelItem.count(); ++bx)
					{
						if (SelItem.at(bx)->Groups.count() != 0)
						{
							if (SelItem.at(bx)->Groups.top() != firstElem)
								isGroup = false;
						}
						else
							isGroup = false;
					}
					if (!isGroup)
						ScApp->scrActions["itemGroup"]->addTo(pmen);
				}
				if (currItem->Groups.count() != 0)
					ScApp->scrActions["itemUngroup"]->addTo(pmen);
				if ((!currItem->isTableItem) && (!currItem->isSingleSel))
				{
					pmen->insertItem( tr("Le&vel"), pmenLevel);
					ScApp->scrActions["itemRaise"]->addTo(pmenLevel);
					ScApp->scrActions["itemLower"]->addTo(pmenLevel);
					ScApp->scrActions["itemRaiseToTop"]->addTo(pmenLevel);
					ScApp->scrActions["itemLowerToBottom"]->addTo(pmenLevel);
				}
			}
			if (Doc->appMode != EditMode) //Create convertTo Menu
			{
				bool insertConvertToMenu=false;
				if (currItem->itemType() == PageItem::TextFrame)
				{
					insertConvertToMenu=true;
					ScApp->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
					if (!currItem->isTableItem)
					{
						ScApp->scrActions["itemConvertToOutlines"]->addTo(pmen2);
						ScApp->scrActions["itemConvertToPolygon"]->addTo(pmen2);
					}
				}
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					insertConvertToMenu=true;
					ScApp->scrActions["itemConvertToTextFrame"]->addTo(pmen2);
					if (!currItem->isTableItem)
						ScApp->scrActions["itemConvertToPolygon"]->addTo(pmen2);
				}
				if (currItem->itemType() == PageItem::Polygon)
				{
					insertConvertToMenu=true;
					ScApp->scrActions["itemConvertToBezierCurve"]->addTo(pmen2);
					ScApp->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
					ScApp->scrActions["itemConvertToTextFrame"]->addTo(pmen2);
				}
				if (insertConvertToMenu)
					pmen->insertItem( tr("Conve&rt to"), pmen2);
			}
			pmen->insertSeparator();
			if (!currItem->locked() && !(currItem->isTableItem && currItem->isSingleSel))
				ScApp->scrActions["editCut"]->addTo(pmen);
			if (!(currItem->isTableItem && currItem->isSingleSel))
				ScApp->scrActions["editCopy"]->addTo(pmen);
			if ((Doc->appMode == EditMode) && (ScApp->Buffer2.startsWith("<SCRIBUSTEXT")) && (currItem->itemType() == PageItem::TextFrame))
				ScApp->scrActions["editPaste"]->addTo(pmen);
			if (!currItem->locked() && (Doc->appMode != 7) && (!(currItem->isTableItem && currItem->isSingleSel)))
				pmen->insertItem( tr("&Delete"), this, SLOT(DeleteItem()));
			if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
				ScApp->scrActions["editClearContents"]->addTo(pmen);
			pmen->insertSeparator();
			ScApp->scrActions["toolsProperties"]->addTo(pmen);

			pmen->exec(QCursor::pos());
			setGlobalUndoMode();
			delete pmen;
			delete pmen2;
			disconnect(pmen3, SIGNAL(activated(int)), this, SLOT(sentToLayer(int)));
			disconnect(pmenResolution, SIGNAL(activated(int)), this, SLOT(changePreview(int)));
			delete pmen3;
			delete pmen4;
			delete pmenLevel;
			delete pmenPDF;
			delete pmenResolution;
		}
		if (Doc->appMode == LinkFrames)
		{
			updateContents();
			if (Doc->ElemToLink != 0)
				return;
			else
			{
				Doc->appMode = NormalMode;
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				emit PaintingDone();
				return;
			}
		}
		if (Doc->appMode == DrawRegularPolygon)
		{
			currItem = SelItem.at(0);
			FPoint np1 = FPoint(m->x() / Scale, m->y() / Scale);
			np1 = ApplyGridF(np1);
			currItem->Width = np1.x() - currItem->Xpos;
			currItem->Height = np1.y()- currItem->Ypos;
			FPointArray cli = RegularPolygonF(currItem->Width, currItem->Height, Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
			FPoint np = FPoint(cli.point(0));
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
			FPoint tp2 = getMinClipF(&currItem->PoLine);
			if ((tp2.x() > -1) || (tp2.y() > -1))
			{
				SizeItem(currItem->Width - tp2.x(), currItem->Height - tp2.y(), currItem->ItemNr, false, false, false);
			}
			FPoint tp = getMaxClipF(&currItem->PoLine);
			SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
			AdjustItemSize(currItem);
			currItem->ContourLine = currItem->PoLine.copy();
			setRedrawBounding(currItem);
			currItem->OwnPage = OnPage(currItem);
			updateContents();
		}
		if (Doc->appMode == DrawLine)
		{
			currItem = SelItem.at(0);
			QPainter p;
			p.begin(viewport());
			Transform(currItem, &p);
			QPoint np = p.xFormDev(m->pos());
			p.end();
			np = ApplyGrid(np);
			currItem->Rot = xy2Deg(np.x(), np.y());
			currItem->Width = sqrt(pow(np.x(),2.0)+pow(np.y(),2.0));
			currItem->Height = 1;
			currItem->Sizing = false;
			UpdateClip(currItem);
			setRedrawBounding(currItem);
			currItem->OwnPage = OnPage(currItem);
			updateContents();
		}
		if (GetItem(&currItem))
		{
			if (GroupSel)
			{
				if (mCG)
				{
					double gx, gy, gh, gw, nx, ny, scx, scy;
					getGroupRect(&gx, &gy, &gw, &gh);
					double sc = Scale;
					scx = sc;
					scy = sc;
					QPoint np2;
					if (m->state() & ControlButton)
						np2 = QPoint(qRound(m->x()/sc), qRound(((gy+(gh * ((m->x()/sc-gx) / gw)))*sc)/sc));
					else
						np2 = QPoint(qRound(m->x()/sc), qRound(m->y()/sc));
					nx = np2.x();
					ny = np2.y();
					if (!ApplyGuides(&nx, &ny))
					{
						np2 = ApplyGrid(np2);
						nx = np2.x();
						ny = np2.y();
					}
					switch (HowTo)
					{
						case 1:
							scx = fabs(nx-gx) / gw;
							scy = fabs(ny-gy) / gh;
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
					scaleGroup(scx, scy);
					if ((HowTo == 3) || (HowTo == 8))
						moveGroup(0, ny-gy);
					if (HowTo == 2)
						moveGroup(nx-gx, ny-gy);
					if ((HowTo == 7) || (HowTo == 4))
						moveGroup(nx-gx, 0);
					Doc->RotMode = RotMode;
					updateContents();
					emit DocChanged();
				}
			}
			else
			{
				if (currItem->Sizing)
				{
					FPoint npx;
					double nx = m->pos().x()/Scale;
					double ny = m->pos().y()/Scale;
					if (Doc->SnapGuides)
					{
						ApplyGuides(&nx, &ny);
						npx = transformPointI(FPoint(nx, ny), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
					}
					else
						npx = ApplyGridF(transformPointI(FPoint(nx, ny), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1));
					if ((HowTo == 1) && (currItem->itemType() != PageItem::Line) && (Doc->SnapGuides))
						SizeItem(npx.x(), npx.y(), currItem->ItemNr);
					bool sav = Doc->SnapGuides;
					Doc->SnapGuides = false;
					switch (HowTo)
					{
					case 1:
						if (currItem->itemType() != PageItem::Line)
						{
							if (currItem->isTableItem)
							{
								double dist;
								if (currItem->LeftLink != 0)
									dist = npx.y() - currItem->LeftLink->Height;
								else if (currItem->RightLink != 0)
									dist = npx.y() - currItem->RightLink->Height;
								else
									dist = npx.y() - currItem->Height;
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
									dist = npx.x() - currItem->TopLink->Width;
								else if (currItem->BottomLink != 0)
									dist = npx.x() - currItem->BottomLink->Width;
								else
									dist = npx.x() - currItem->Width;
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
							if (currItem->imageFlippedH())
								MoveItemI(-(currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
							if (currItem->imageFlippedV())
								MoveItemI(0, -(currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
						}
						else
						{
							if (sav)
							{
								double nx = m->pos().x()/Scale;
								double ny = m->pos().y()/Scale;
								if (Doc->useRaster)
								{
									FPoint ra = ApplyGridF(FPoint(nx, ny));
									nx = ra.x();
									ny = ra.y();
								}
								Doc->SnapGuides = sav;
								ApplyGuides(&nx, &ny);
								Doc->SnapGuides = false;
								double r = atan2(ny - currItem->Ypos, nx - currItem->Xpos)*(180.0/M_PI);
								RotateItem(r, currItem->ItemNr);
								double w = sqrt(pow(nx - currItem->Xpos, 2) + pow(ny - currItem->Ypos,2));
								SizeItem(w, currItem->Height, currItem->ItemNr, true);
							}
						}
						currItem->Sizing = false;
						break;
					case 2:
						if (currItem->itemType() != PageItem::Line)
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
							currItem->Sizing = false;
							if (!currItem->imageFlippedH())
								MoveItemI((currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr);
							if (!currItem->imageFlippedV())
								MoveItemI(0, (currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr);
						}
						else
						{
							if (sav)
							{
								double nx = m->pos().x()/Scale;
								double ny = m->pos().y()/Scale;
								if (Doc->useRaster)
								{
									FPoint ra = ApplyGridF(FPoint(nx, ny));
									nx = ra.x();
									ny = ra.y();
								}
								Doc->SnapGuides = sav;
								ApplyGuides(&nx, &ny);
								Doc->SnapGuides = false;
								QWMatrix ma;
								ma.translate(currItem->Xpos, currItem->Ypos);
								ma.rotate(currItem->Rot);
								double mx = ma.m11() * currItem->Width + ma.m21() * currItem->Height + ma.dx();
								double my = ma.m22() * currItem->Height + ma.m12() * currItem->Width + ma.dy();
								double r = atan2(my-ny,mx-nx)*(180.0/M_PI);
								double w = sqrt(pow(mx-nx,2)+pow(my-ny,2));
								MoveItem(nx - currItem->Xpos, ny - currItem->Ypos, currItem, true);
								SizeItem(w, currItem->Height, currItem->ItemNr, true);
								RotateItem(r, currItem->ItemNr);
								currItem->Sizing = false;
							}
						}
						break;
					case 3:
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->Width;
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
							MoveSizeItem(FPoint(0, npx.y()), FPoint(currItem->Width - npx.x(), npx.y()), currItem->ItemNr);
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							MoveItemI(-(currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
						if (!currItem->imageFlippedV())
							MoveItemI(0, (currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
						break;
					case 4:
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->Height;
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
							MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), currItem->Height - npx.y()), currItem->ItemNr);
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							MoveItemI((currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
						if (currItem->imageFlippedV())
							MoveItemI(0, -(currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
						break;
					case 5:
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->Height;
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
							MoveSizeItem(FPoint(0, 0), FPoint(0, currItem->Height - npx.y()), currItem->ItemNr);
						if (currItem->imageFlippedV())
							MoveItemI(0, -(currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
						currItem->Sizing = false;
						break;
					case 6:
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->Width;
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
							MoveSizeItem(FPoint(0, 0), FPoint(currItem->Width - npx.x(), 0), currItem->ItemNr);
						if (currItem->imageFlippedH())
							MoveItemI(-(currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
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
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							MoveItemI((currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
						if (currItem->imageFlippedV())
							MoveItemI(0, -(currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
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
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							MoveItemI(-(currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
						if (!currItem->imageFlippedV())
							MoveItemI(0, (currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
						break;
					}
					if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & ShiftButton) && (m->state() & ControlButton))
					{
						double scx = currItem->Width / currItem->OldB2;
						double scy = currItem->Height / currItem->OldH2;
						scx = scx != scy ? scx / scy : 1.0;
						if (currItem->itemText.count() != 0)
						{
							currItem->ISize = QMAX(qRound(currItem->ISize * scy), 1);
							currItem->LineSp = ((currItem->ISize / 10.0)* static_cast<double>(Doc->typographicSetttings.autoLineSpacing) / 100) + (currItem->ISize / 10.0);
							currItem->TxtScale = QMIN(QMAX(qRound(currItem->TxtScale * scx), 25), 400);
							Doc->CurrTextScale = currItem->TxtScale;
							Doc->CurrFontSize = currItem->ISize;
							emit ItemTextAttr(currItem->LineSp);
							emit ItemTextCols(currItem->Cols, currItem->ColGap);
							emit ItemTextSize(currItem->ISize);
							emit ItemTextSca(currItem->TxtScale);
							for (uint aa = 0; aa < currItem->itemText.count(); ++aa)
							{
								currItem->itemText.at(aa)->csize = QMAX(qRound(currItem->itemText.at(aa)->csize*scy), 1);
								currItem->itemText.at(aa)->cscale = QMAX(QMIN(qRound(currItem->itemText.at(aa)->cscale*scx), 400), 25);
							}
						}
					}
					if (currItem->itemType() == PageItem::ImageFrame)
						AdjustPictScale(currItem);
					UpdateClip(currItem);
					emit ItemTextCols(currItem->Cols, currItem->ColGap);
					Doc->SnapGuides = sav;
					updateContents();
					emit DocChanged();
				}
				setRedrawBounding(currItem);
				currItem->OwnPage = OnPage(currItem);
			}
			if (Imoved)
			{
				if (GroupSel)
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
					currItem = SelItem.at(0);
					if (currItem->itemType() != PageItem::Line)
					{
						if (fabs(currItem->Width) < 5)
							currItem->Width = 5;
						if (fabs(currItem->Height) < 5)
							currItem->Height = 5;
					}
					if (Doc->useRaster)
					{
						double nx = currItem->Xpos;
						double ny = currItem->Ypos;
						if (!ApplyGuides(&nx, &ny))
						{
							FPoint npx;
							npx = ApplyGridF(FPoint(nx, ny));
							nx = npx.x();
							ny = npx.y();
						}
						MoveItem(nx-currItem->Xpos, ny-currItem->Ypos, currItem);
					}
					else
						MoveItem(0, 0, currItem, false);
				}
				Imoved = false;
				setRedrawBounding(currItem);
				currItem->OwnPage = OnPage(currItem);
				if (currItem->OwnPage != -1)
				{
					Doc->currentPage = Doc->Pages.at(currItem->OwnPage);
					setMenTxt(currItem->OwnPage);
				}
				emit HaveSel(currItem->itemType());
				EmitValues(currItem);
				updateContents();
				emit DocChanged();
			}
		}
		if ((SelItem.count() == 0) && (HaveSelRect) && (!MidButt))
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
			if (Doc->Items.count() != 0)
			{
				for (uint a = 0; a < Doc->Items.count(); ++a)
				{
					PageItem* docItem = Doc->Items.at(a);
					p.begin(viewport());
					Transform(docItem, &p);
					QRegion apr = QRegion(p.xForm(docItem->Clip));
					QRect apr2 = getRedrawBounding(docItem);
					p.end();
					if ((Doc->MasterP) && (docItem->OnMasterPage != Doc->currentPage->PageNam))
						continue;
					if (((Sele.contains(apr.boundingRect())) || (Sele.contains(apr2))) && (docItem->LayerNr == Doc->ActiveLayer))
						SelectItemNr(a, false);
				}
			}
			HaveSelRect = false;
		}
		if (Doc->appMode != EditMode)
		{
			if (Doc->appMode == Rotation)
				Doc->RotMode = RotMode;
			Doc->appMode = NormalMode;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			emit PaintingDone();
		}
		if (GetItem(&currItem))
		{
			if (SelItem.count() > 1)
			{
				setGroupRect();
				paintGroupRect();
				emit HaveSel(currItem->itemType());
				double x, y, w, h;
				getGroupRect(&x, &y, &w, &h);
				emit ItemPos(x, y);
				emit ItemGeom(w, h);
			}
			else
			{
				emit HaveSel(currItem->itemType());
				EmitValues(currItem);
			}
		}
	}
	if ((Doc->appMode == EditMode) && !HanMove)
	{
		currItem = SelItem.at(0);
		uint a;
		if (currItem->itemType() == PageItem::TextFrame)
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
	if (Doc->appMode == Magnifier)
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
			Scale = QMIN(yf, xf);
			slotDoZoom();
			if (sc == Scale)
			{
				SetCPo(Mxp, Myp);
				HaveSelRect = false;
				Doc->appMode = NormalMode;
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				emit PaintingDone();
			}
		}
		else
		{
			int mx = qRound(m->x() / Scale);
			int my = qRound(m->y() / Scale);
			Magnify ? slotZoomIn2(mx,my) : slotZoomOut2(mx,my);
			if (sc == Scale)
			{
				HaveSelRect = false;
				Doc->appMode = NormalMode;
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				emit PaintingDone();
			}
			else
				qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
		}
	}
	if ((Doc->appMode == DrawBezierLine) && (m->button() == LeftButton))
	{
		currItem = SelItem.at(0);
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		QPainter p;
		QPointArray Bez(4);
		p.begin(viewport());
		Transform(currItem, &p);
		FPoint npf = FPoint(p.xFormDev(m->pos()));
		npf = ApplyGridF(npf);
		currItem->PoLine.addPoint(npf);
		bool ssiz = currItem->Sizing;
		currItem->Sizing = true;
		if ((currItem->PoLine.size() % 4 == 0) && (currItem->PoLine.size() > 3))
		{
			FPoint lxy = currItem->PoLine.point(currItem->PoLine.size()-2);
			FPoint lk = currItem->PoLine.point(currItem->PoLine.size()-1);
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			currItem->PoLine.addPoint(lxy);
			currItem->PoLine.addPoint(lk);
		}
		FPoint np2 = getMinClipF(&currItem->PoLine);
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
		SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
		AdjustItemSize(currItem);
		currItem->Sizing = ssiz;
		currItem->ContourLine = currItem->PoLine.copy();
		RefreshItem(currItem);
		}
		p.end();
	}
	if ((Doc->appMode == DrawBezierLine) && (m->button() == RightButton))
	{
		currItem = SelItem.at(0);
		currItem->PoLine.resize(currItem->PoLine.size()-2);
		if (currItem->PoLine.size() < 4)
		{
//			emit DelObj(Doc->currentPage->PageNr, currItem->ItemNr);
			Doc->Items.remove(currItem->ItemNr);
			SelItem.removeFirst();
			emit HaveSel(-1);
		}
		else
		{
			SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false);
			SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
			AdjustItemSize(currItem);
			currItem->ContourLine = currItem->PoLine.copy();
		}
		Doc->appMode = NormalMode;
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		emit PaintingDone();
		emit DocChanged();
		FirstPoly = true;
		updateContents();
	}
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Imoved = false;
	mCG = false;
	MidButt = false;
	Doc->SubMode = -1;
	if (_groupTransactionStarted)
	{
		for (uint i = 0; i < SelItem.count(); ++i)
			SelItem.at(i)->checkChanges(true);
		undoManager->commit();
		_groupTransactionStarted = false;
	}

	for (uint i = 0; i < SelItem.count(); ++i)
		SelItem.at(i)->checkChanges();
	if (_itemCreationTransactionStarted && Doc->appMode !=  DrawBezierLine)
	{
		PageItem *ite = SelItem.at(0);
		if (ite!=NULL)
		{
			ite->checkChanges(true);
			QString targetName = Um::ScratchSpace;
			if (ite->OwnPage > -1)
				targetName = Doc->Pages.at(ite->OwnPage)->getUName();
			undoManager->commit(targetName, ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
			if (!Doc->loading)
				emit AddObj(ite);
		}
	}
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
	emit MousePos(m->x()/Scale-Doc->currentPage->Xoffset, m->y()/Scale-Doc->currentPage->Yoffset);
	if (Doc->guidesSettings.guidesShown)
	{
		if (MoveGY)
		{
			FromHRuler(m);
			if (((m->y()/sc) < Doc->currentPage->Yoffset) || ((m->y()/sc) > Doc->currentPage->Height+Doc->currentPage->Yoffset))
				qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png")), true);
			else
				qApp->setOverrideCursor(QCursor(SPLITHC), true);
			return;
		}
		if (MoveGX)
		{
			FromVRuler(m);
			if (((m->x()/sc) < Doc->currentPage->Xoffset) || ((m->x()/sc) > Doc->currentPage->Width+Doc->currentPage->Xoffset))
				qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png")), true);
			else
				qApp->setOverrideCursor(QCursor(SPLITVC), true);
			return;
		}
	}
/*	if ((!Doc->DragP) && (Mpressed) && (SelItem.count() != 0))
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
	if (Mpressed && (Doc->appMode == EditGradientVectors))
	{
		PageItem *currItem = SelItem.at(0);
		newX = m->x();
		newY = m->y();
		if (m->state() == LeftButton)
		{
			currItem->GrStartX -= (Mxp - newX) / Scale;
			currItem->GrStartX = QMIN(QMAX(0.0, currItem->GrStartX), currItem->Width);
			currItem->GrStartY -= (Myp - newY) / Scale;
			currItem->GrStartY = QMIN(QMAX(0.0, currItem->GrStartY), currItem->Height);
		}
		if (m->state() == RightButton)
		{
			currItem->GrEndX -= (Mxp - newX) / Scale;
			currItem->GrEndX = QMIN(QMAX(0.0, currItem->GrEndX), currItem->Width);
			currItem->GrEndY -= (Myp - newY) / Scale;
			currItem->GrEndY = QMIN(QMAX(0.0, currItem->GrEndY), currItem->Height);
		}
		Mxp = newX;
		Myp = newY;
		RefreshItem(currItem);
		ScApp->propertiesPalette->Cpal->setSpecialGradient(currItem->GrStartX * Doc->unitRatio, currItem->GrStartY * Doc->unitRatio,
															currItem->GrEndX * Doc->unitRatio, currItem->GrEndY * Doc->unitRatio,
															currItem->Width * Doc->unitRatio, currItem->Height * Doc->unitRatio);
		return;
	}
	if (Mpressed && (Doc->appMode == MeasurementTool))
	{
		newX = m->x();
		newY = m->y();
		p.begin(viewport());
		ToView(&p);
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.drawLine(Dxp, Dyp, Mxp, Myp);
		p.drawLine(Dxp, Dyp, newX, newY);
		p.end();
		Mxp = newX;
		Myp = newY;
		double dxp = Dxp / sc - Doc->currentPage->Xoffset;
		double dyp = Dyp / sc - Doc->currentPage->Yoffset;
		double nxp = newX / sc - Doc->currentPage->Xoffset;
		double nyp = newY / sc - Doc->currentPage->Yoffset;
		emit MVals(dxp, dyp, nxp, nyp, -xy2Deg(newX/sc - Dxp*sc, newY/sc - Dyp/sc), sqrt(pow(newX/sc - Dxp/sc,2)+pow(newY/sc - Dyp/sc,2)), Doc->docUnitIndex);
		return;
	}
	if (Mpressed && (Doc->appMode == PanningMode))
	{
		int scroX = m->x() - qRound((Mxp * sc));
		int scroY = m->y() - qRound((Myp * sc));
		scrollBy(-scroX, -scroY);
		Mxp = static_cast<int>((m->x()-scroX)/sc);
		Myp = static_cast<int>((m->y()-scroY)/sc);
		return;
	}
	if (Mpressed && (Doc->appMode == DrawFreehandLine))
	{
		newX = m->x();
		newY = m->y();
		double newXF = m->x()/sc;
		double newYF = m->y()/sc;
		if (RecordP.size() > 0)
		{
			if (FPoint(newXF, newYF) != RecordP.point(RecordP.size()-1))
				RecordP.addPoint(FPoint(newXF, newYF));
		}
		else
			RecordP.addPoint(FPoint(newXF, newYF));
		p.begin(viewport());
		ToView(&p);
		if (RecordP.size() > 1)
		{
			FPoint xp = RecordP.point(RecordP.size()-2);
			p.drawLine(qRound(xp.x()*sc), qRound(xp.y()*sc), newX, newY);
		}
		else
			p.drawPoint(m->x(), m->y());
		p.end();
		return;
	}
	if (GetItem(&currItem))
	{
		newX = static_cast<int>(m->x()/sc);
		newY = static_cast<int>(m->y()/sc);
		if ((Mpressed) && (m->state() == RightButton) && (!Doc->DragP) && (Doc->appMode == NormalMode) && (!currItem->locked()) && (!(currItem->isTableItem && currItem->isSingleSel)))
		{
			if ((abs(Dxp - newX) > 10) || (abs(Dyp - newY) > 10))
			{
				Doc->DragP = true;
				Doc->leaveDrag = false;
				Doc->DraggedElem = currItem;
				Doc->DragElements.clear();
				for (uint dre=0; dre<SelItem.count(); ++dre)
					Doc->DragElements.append(SelItem.at(dre)->ItemNr);
				ScriXmlDoc *ss = new ScriXmlDoc();
				QDragObject *dr = new QTextDrag(ss->WriteElem(&SelItem, Doc, this), this);
				dr->setPixmap(loadIcon("DragPix.xpm"));
				dr->drag();
				delete ss;
				Doc->DragP = false;
				Doc->leaveDrag = false;
				Mpressed = false;
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
			}
			return;
		}
		if (Doc->DragP)
			return;
		if (Mpressed && (Doc->appMode == Rotation))
		{
			double newW = xy2Deg(m->x()/sc - RCenter.x(), m->y()/sc - RCenter.y());
			if (GroupSel)
				RotateGroup(newW - oldW);
			else
				RotateItem(currItem->Rot - (oldW - newW), currItem->ItemNr);
			oldW = newW;
			emit DocChanged();
		}
		if (Doc->appMode == DrawBezierLine)
		{
			p.begin(viewport());
			ToView(&p);
			p.scale(Scale, Scale);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if ((Doc->useRaster) && (OnPage(currItem) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			if (!Mpressed)
			{
				QPoint a1 = currItem->PoLine.pointQ(currItem->PoLine.size()-2);
				QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
				a1 += QPoint(qRound(currItem->Xpos), qRound(currItem->Ypos));
				a2 += QPoint(qRound(currItem->Xpos), qRound(currItem->Ypos));
				BezierPoints(&Bez, a1, a2, QPoint(Mxp, Myp), QPoint(Mxp, Myp));
				p.drawCubicBezier(Bez);
				Bez.setPoint(2, QPoint(newX, newY));
				Bez.setPoint(3, QPoint(newX, newY));
				p.drawCubicBezier(Bez);
			}
			else
			{
				QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
				a2 += QPoint(qRound(currItem->Xpos), qRound(currItem->Ypos));
				if (currItem->PoLine.size() > 2)
				{
					QPoint a1 = currItem->PoLine.pointQ(currItem->PoLine.size()-2);
					QPoint a3 = currItem->PoLine.pointQ(currItem->PoLine.size()-3);
					a1 += QPoint(qRound(currItem->Xpos), qRound(currItem->Ypos));
					a3 += QPoint(qRound(currItem->Xpos), qRound(currItem->Ypos));
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
		if (Mpressed && (Doc->appMode == DrawRegularPolygon))
		{
			p.begin(viewport());
			ToView(&p);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (m->state() & ShiftButton)
			{
				mop = QPoint(m->x(), static_cast<int>((currItem->Ypos + (newX - currItem->Xpos)) * sc));
				QCursor::setPos(mapToGlobal(mop));
				newY = static_cast<int>(mop.y()/sc);
			}
			if ((Doc->useRaster) && (OnPage(currItem) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			p.drawRect(static_cast<int>(currItem->Xpos*sc), static_cast<int>(currItem->Ypos*sc),
			           static_cast<int>(Mxp*sc-currItem->Xpos*sc), static_cast<int>(Myp*sc-currItem->Ypos*sc));
			p.drawRect(static_cast<int>(currItem->Xpos*sc), static_cast<int>(currItem->Ypos*sc),
			           static_cast<int>(newX*sc-currItem->Xpos*sc), static_cast<int>(newY*sc-currItem->Ypos*sc));
			p.end();
			emit ItemGeom(newX - currItem->Xpos, newY - currItem->Ypos);
			Mxp = newX;
			Myp = newY;
		}
		if (Mpressed && (Doc->appMode == DrawLine))
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
			p.drawLine(static_cast<int>(currItem->Xpos*sc), static_cast<int>(currItem->Ypos*sc), static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc));
			p.drawLine(static_cast<int>(currItem->Xpos*sc), static_cast<int>(currItem->Ypos*sc), static_cast<int>(newX*sc), static_cast<int>(newY*sc));
			p.end();
			emit SetAngle(xy2Deg(newX - currItem->Xpos, newY - currItem->Ypos));
			emit ItemGeom(sqrt(pow(newX - currItem->Xpos,2)+pow(newY - currItem->Ypos,2)), 0);
			Mxp = newX;
			Myp = newY;
		}
		if (Mpressed && (Doc->appMode == EditMode) && (!HanMove))
		{
			if (currItem->itemType() == PageItem::ImageFrame)
			{
				MoveItemI((newX-Mxp)/currItem->LocalScX, (newY-Myp)/currItem->LocalScY, currItem->ItemNr);
				Mxp = newX;
				Myp = newY;
			}
			if (currItem->itemType() == PageItem::TextFrame)
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
		if (Mpressed && (m->state() & LeftButton) && ((Doc->appMode == NormalMode) || ((Doc->appMode == EditMode) && HanMove)) && (!currItem->locked()))
		{
			if (Doc->EditClip)
			{
				if ((Mpressed) && (ClRe == -1) && (SegP1 == -1) && (SegP2 == -1))
				{
					newX = qRound(m->x()/sc);
					newY = qRound(m->y()/sc);
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
				Imoved = true;
				currItem = SelItem.at(0);
				currItem->OldB2 = currItem->Width;
				currItem->OldH2 = currItem->Height;
				p.begin(viewport());
				Transform(currItem, &p);
				npf = FPoint(p.xFormDev(m->pos()));
				p.end();
				if ((SegP1 != -1) && (SegP2 != -1))
				{
					FPointArray Clip;
					if (EditContour)
						Clip = currItem->ContourLine;
					else
						Clip = currItem->PoLine;
					p.begin(viewport());
					p.translate(static_cast<int>(currItem->Xpos*Scale), static_cast<int>(currItem->Ypos*Scale));
					p.rotate(currItem->Rot);
					FPoint npfN = FPoint(p.xFormDev(QPoint(newX, newY)));
					FPoint npfM = FPoint(p.xFormDev(QPoint(Mxp, Myp)));
					npf.setX(Clip.point(SegP2).x() + (npfN.x()-npfM.x()));
					npf.setY(Clip.point(SegP2).y() + (npfN.y()-npfM.y()));
					ClRe = SegP2;
					MoveClipPoint(currItem, npf);
					currItem->OldB2 = currItem->Width;
					currItem->OldH2 = currItem->Height;
					if (EditContour)
						Clip = currItem->ContourLine;
					else
						Clip = currItem->PoLine;
					ClRe = SegP1;
					npf2.setX(Clip.point(SegP1).x() + (npfN.x()-npfM.x()));
					npf2.setY(Clip.point(SegP1).y() + (npfN.y()-npfM.y()));
					MoveClipPoint(currItem, npf2);
					p.end();
					Mxp = newX;
					Myp = newY;
				}
				else
				{
					if ((SelNode.count() != 0) && (EdPoints))
					{
						int storedClRe = ClRe;
						FPointArray Clip;
						if (EditContour)
							Clip = currItem->ContourLine;
						else
							Clip = currItem->PoLine;
						for (uint itm = 0; itm < SelNode.count(); ++itm)
						{
							p.begin(viewport());
							p.translate(static_cast<int>(currItem->Xpos*Scale), static_cast<int>(currItem->Ypos*Scale));
							p.rotate(currItem->Rot);
							FPoint npfN = FPoint(p.xFormDev(QPoint(newX, newY)));
							FPoint npfM = FPoint(p.xFormDev(QPoint(Mxp, Myp)));
							p.end();
							npf.setX(Clip.point(*SelNode.at(itm)).x() + (npfN.x()-npfM.x()));
							npf.setY(Clip.point(*SelNode.at(itm)).y() + (npfN.y()-npfM.y()));
							ClRe = *SelNode.at(itm);
							currItem->OldB2 = currItem->Width;
							currItem->OldH2 = currItem->Height;
							MoveClipPoint(currItem, npf);
						}
						currItem->OldB2 = currItem->Width;
						currItem->OldH2 = currItem->Height;
						ClRe = storedClRe;
					}
					else
						MoveClipPoint(currItem, npf);
					Mxp = newX;
					Myp = newY;
				}
				return;
			}
			if (mCG)
			{
				Imoved = false;
				if (GroupSel)
				{
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
					switch (HowTo)
					{
					case 1:
						if (m->state() & ControlButton)
						{
							np2 = QPoint(m->x(), qRound((gy+(gh * ((newX-gx) / gw)))*sc));
							np2 = QPoint(qRound(np2.x()/sc), qRound(np2.y()/sc));
						}
						else
							np2 = QPoint(qRound(m->x()/sc), qRound(m->y()/sc));
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
					for (a = 0; a < SelItem.count(); ++a)
					{
						currItem = SelItem.at(0);
						if ((HowTo == 1) || (HowTo == 2))
						{
							switch (HowTo)
							{
							case 1:
								p.begin(viewport());
								Transform(currItem, &p);
								if ((m->state() & ShiftButton) && (!(m->state() & ControlButton)))
									mop = QPoint(m->x(), static_cast<int>((currItem->Ypos + (newX - currItem->Xpos)) * sc));
								else
								{
									if ((m->state() & ControlButton) && (!(m->state() & ShiftButton)))
										mop = QPoint(m->x(), static_cast<int>((currItem->Ypos + ((newX - currItem->Xpos) / currItem->OldB2 * currItem->OldH2)) * sc));
									else
										mop = QPoint(m->x(), m->y());
								}
								np = p.xFormDev(mop);
								nx = np.x();
								ny = np.y();
								p.end();
								if (currItem->itemType() != PageItem::Line)
								{
									if ((Doc->useRaster) && (OnPage(currItem) != -1))
									{
										dx = currItem->Xpos - int (currItem->Xpos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
										dy = currItem->Ypos - int (currItem->Ypos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
										nx = (qRound(np.x() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid - dx);
										ny = (qRound(np.y() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid - dy);
									}
									if (Doc->SnapGuides)
									{
										nx += currItem->Xpos;
										ny += currItem->Ypos;
										ApplyGuides(&nx, &ny);
										nx -= currItem->Xpos;
										ny -= currItem->Ypos;
									}
									erf = SizeItem(nx, ny, currItem->ItemNr);
								}
								else
								{
									p.begin(viewport());
									double rba = currItem->Rot;
									currItem->Rot = 0;
									Transform(currItem, &p);
									np = p.xFormDev(QPoint(m->x(), m->y()));
									p.end();
									currItem->Rot = rba;
									np = ApplyGrid(np);
									erf = SizeItem(np.x(), np.y(), currItem->ItemNr);
									if (Doc->SnapGuides)
									{
										p.begin(viewport());
										currItem->Sizing = true;
										p.setRasterOp(XorROP);
										p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
										p.drawLine(static_cast<int>(currItem->Xpos*sc), static_cast<int>(currItem->Ypos*sc),
												static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc));
										p.drawLine(static_cast<int>(currItem->Xpos*sc), static_cast<int>(currItem->Ypos*sc),
												static_cast<int>(newX*sc), static_cast<int>(newY*sc));
										p.end();
									}
								}
								break;
							case 2:
								if (currItem->itemType() == PageItem::Line)
								{
									double sav = Doc->SnapGuides;
									npf2 = FPoint(newX-Mxp, newY-Myp);
									erf = MoveSizeItem(npf2, FPoint(0, 0), currItem->ItemNr);
									Doc->SnapGuides = sav;
									if (sav)
										currItem->Sizing = true;
								}
								else
								{
									p.begin(viewport());
									np2 = QPoint(newX, newY);
									np2 = ApplyGrid(np2);
									double nx = np2.x();
									double ny = np2.y();
									ApplyGuides(&nx, &ny);
									p.translate(static_cast<int>(currItem->Xpos), static_cast<int>(currItem->Ypos));
									p.rotate(currItem->Rot);
									np2 = p.xFormDev(QPoint(qRound(nx), qRound(ny)));
									p.end();
									p.begin(viewport());
									ToView(&p);
									Transform(currItem, &p);
									PaintSizeRect(&p, QRect(np2, QPoint(qRound(currItem->Width), qRound(currItem->Height))));
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
							p.translate(static_cast<int>(currItem->Xpos), static_cast<int>(currItem->Ypos));
							p.rotate(currItem->Rot);
							np2 = p.xFormDev(QPoint(qRound(nx), qRound(ny)));
							p.end();
							p.begin(viewport());
							ToView(&p);
							Transform(currItem, &p);
							switch (HowTo)
							{
							case 0:
								break;
							case 3:
								PaintSizeRect(&p, QRect(np2, QPoint(0, qRound(currItem->Height))));
								break;
							case 4:
								PaintSizeRect(&p, QRect(np2, QPoint(qRound(currItem->Width), 0)));
								break;
							case 5:
								PaintSizeRect(&p, QRect(QPoint(0, 0), QPoint(qRound(currItem->Width), np2.y())));
								break;
							case 6:
								PaintSizeRect(&p, QRect(QPoint(0, 0), QPoint(np2.x(), qRound(currItem->Height))));
								break;
							case 7:
								PaintSizeRect(&p, QRect(QPoint(np2.x(), 0), QPoint(qRound(currItem->Width), qRound(currItem->Height))));
								break;
							case 8:
								PaintSizeRect(&p, QRect(QPoint(0, np2.y()), QPoint(qRound(currItem->Width), qRound(currItem->Height))));
								break;
							}
							p.end();
						}
					}
				}
				if ((erf) || (HowTo > 1))
				{
					Mxp = newX;
					Myp = newY;
				}
			}
			else
			{
				Imoved = true;
				erf = false;
				if (!GroupSel)
				{
					currItem = SelItem.at(0);
					if (!(currItem->isTableItem && currItem->isSingleSel))
					{
						moveGroup(newX-Mxp, newY-Myp, false);
						if (Doc->SnapGuides)
						{
							double nx = currItem->Xpos;
							double ny = currItem->Ypos;
							ApplyGuides(&nx, &ny);
							moveGroup(nx-currItem->Xpos, ny-currItem->Ypos, false);
							nx = currItem->Xpos+currItem->Width;
							ny = currItem->Ypos+currItem->Height;
							ApplyGuides(&nx, &ny);
							moveGroup(nx-(currItem->Xpos+currItem->Width), ny-(currItem->Ypos+currItem->Height), false);
						}
						erf = true;
					}
				}
				else
				{
					setGroupRect();
					double gx, gy, gh, gw;
					getGroupRectScreen(&gx, &gy, &gw, &gh);
					moveGroup(newX-Mxp, newY-Myp, false);
					if (Doc->SnapGuides)
					{
						double nx = gx;
						double ny = gy;
						ApplyGuides(&nx, &ny);
						moveGroup(nx-gx, ny-gy, false);
						setGroupRect();
						getGroupRect(&gx, &gy, &gw, &gh);
						nx = gx+gw;
						ny = gy+gh;
						ApplyGuides(&nx, &ny);
						moveGroup(nx-(gx+gw), ny-(gy+gh), false);
					}
					setGroupRect();
					getGroupRect(&gx, &gy, &gw, &gh);
					erf = true;
				}
				if (erf)
				{
					Mxp = newX;
					Myp = newY;
				}
			}
		}
		if ((!Mpressed) && (Doc->appMode != DrawBezierLine))
		{
			if (Doc->appMode == Magnifier)
			{
				qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
				return;
			}
			if (GroupSel)
			{
				QRect mpo = QRect(qRound(m->x()/Scale)-Doc->guidesSettings.grabRad, qRound(m->y()/Scale)-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
				double gx, gy, gh, gw;
				getGroupRect(&gx, &gy, &gw, &gh);
				if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					&& ((Doc->appMode == NormalMode) || (Doc->appMode == Rotation)))
				{
					int how = 0;
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
					if (Doc->appMode == Rotation)
						qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
				}
				else
				{
					switch (Doc->appMode)
					{
						case DrawShapes:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")), true);
							break;
						case DrawPicture:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")), true);
							break;
						case DrawText:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")), true);
							break;
						case DrawTable:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTable.xpm")), true);
							break;
						case DrawRegularPolygon:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")), true);
							break;
						default:
							qApp->setOverrideCursor(QCursor(ArrowCursor), true);
						break;
					}
				}
				return;
			}
			for (a = 0; a < SelItem.count(); ++a)
			{
				currItem = SelItem.at(a);
				if (currItem->locked())
					break;
				p.begin(viewport());
				Transform(currItem, &p);
				QRect mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
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
				if ((QRegion(p.xForm(QPointArray(QRect(-3, -3, static_cast<int>(currItem->Width+6), static_cast<int>(currItem->Height+6))))).contains(mpo))
					&& ((Doc->appMode == NormalMode) || (Doc->appMode == Rotation) || (Doc->appMode == EditMode)))
				{
					tx = p.xForm(QRect(0, 0, static_cast<int>(currItem->Width), static_cast<int>(currItem->Height)));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
						if (Doc->appMode == Rotation)
							qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
						if (Doc->appMode == EditMode)
						{
							if (currItem->itemType() == PageItem::TextFrame)
								qApp->setOverrideCursor(QCursor(ibeamCursor), true);
							if (currItem->itemType() == PageItem::ImageFrame)
								qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
						}
						if (!currItem->sizeLocked())
							HandleCurs(&p, currItem, mpo);
					}
				}
				else
				{
					switch (Doc->appMode)
					{
						case DrawShapes:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")), true);
							break;
						case DrawPicture:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")), true);
							break;
						case DrawText:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")), true);
							break;
						case DrawTable:
							qApp->setOverrideCursor(QCursor(loadIcon("DrawTable.xpm")), true);
							break;
						case DrawRegularPolygon:
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
		if ((Mpressed) && (m->state() == LeftButton))
		{
			newX = qRound(m->x()/sc);
			newY = qRound(m->y()/sc);
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
		}
		if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == NormalMode) && (!Doc->GuideLock) && (OnPage(m->x()/sc, m->y()/sc) != -1) && (!GetItem(&currItem)))
		{
			if (Doc->currentPage->YGuides.count() != 0)
			{
				for (uint yg = 0; yg < Doc->currentPage->YGuides.count(); ++yg)
				{
					if ((Doc->currentPage->YGuides[yg]+Doc->currentPage->Yoffset < (static_cast<int>(m->y()/sc)+Doc->guidesSettings.guideRad)) &&
							(Doc->currentPage->YGuides[yg]+Doc->currentPage->Yoffset > (static_cast<int>(m->y()/sc)-Doc->guidesSettings.guideRad)))
					{
						if ((Mpressed) && (GyM != -1))
							MoveGY = true;
						if (((m->x()/sc) < Doc->currentPage->Xoffset) || ((m->x()/sc) >= Doc->currentPage->Width-1+Doc->currentPage->Xoffset))
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
					if ((Doc->currentPage->XGuides[xg]+Doc->currentPage->Xoffset < (static_cast<int>(m->x()/sc)+Doc->guidesSettings.guideRad)) &&
							(Doc->currentPage->XGuides[xg]+Doc->currentPage->Xoffset > (static_cast<int>(m->x()/sc)-Doc->guidesSettings.guideRad)))
					{
						if ((Mpressed) && (GxM != -1))
							MoveGX = true;
						if (((m->y()/sc) < Doc->currentPage->Yoffset) || ((m->y()/sc) >= Doc->currentPage->Height-1+Doc->currentPage->Yoffset))
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
	QRect tx, mpo;
	Mpressed = true;
	Imoved = false;
	SeRx = qRound(m->x()/Scale);
	SeRy = qRound(m->y()/Scale);
	HaveSelRect = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Mxp = qRound(m->x()/Scale);
	Myp = qRound(m->y()/Scale);
	mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	if (Doc->appMode != EditMode)
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
		case NormalMode:
			if ((Doc->EditClip) && (SelItem.count() != 0))
			{
				currItem = SelItem.at(0);
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
						if (currItem->itemType() == PageItem::Polygon)
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
									z = PaintPoly(currItem->Xpos, currItem->Ypos, currItem->Width, currItem->Height, currItem->Pwidth, currItem->fillColor(), currItem->lineColor());
									bb = Doc->Items.at(z);
									if (EditContour)
										bb->ContourLine.resize(0);
									else
										bb->PoLine.resize(0);
									if (StartInd != 0)
									{
										if (EditContour)
										{
											bb->ContourLine.putPoints(0, StartInd - 4, Clip);
											bb->ContourLine.putPoints(bb->PoLine.size(), Clip.size()-EndInd, Clip, EndInd);
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
									bb->Rot = currItem->Rot;
									AdjustItemSize(bb);
									bb->ClipEdited = true;
									PageItem *bx = Doc-> Items.take(bb->ItemNr);
									Doc->Items.insert(bb->ItemNr-1, bx);
									for (uint al = 0; al < Doc->Items.count(); ++al)
									{
										Doc->Items.at(al)->ItemNr = al;
										if (Doc->Items.at(al)->isBookmark)
											emit NewBMNr(Doc->Items.at(al)->BMnr, al);
									}
								}
								currItem->PoLine = cli.copy();
							}
							ClRe = -1;
							currItem->ClipEdited = true;
							edited = true;
							Doc->EditClipMode = 0;
							currItem->convertTo(PageItem::PolyLine);
							SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
							emit PolyOpen();
						}
						else
						{
							if ((currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::PathText))
							{
								if ((ClRe > 1) && (ClRe < static_cast<int>(Clip.size()-2)))
								{
									z = PaintPolyLine(currItem->Xpos, currItem->Ypos, currItem->Width, currItem->Height, currItem->Pwidth, currItem->fillColor(), currItem->lineColor());
									bb = Doc->Items.at(z);
									if (EditContour)
										bb->ContourLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									else
										bb->PoLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									bb->Rot = currItem->Rot;
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
								SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
								emit PolyOpen();
							}
						}
					}
				}
				if ((Doc->EditClipMode == 2) && (ClRe != -1))
				{
					if (!EdPoints)
						return;
					if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame))
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
							if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame))
							{
								FPoint kp = Clip.point(EndInd-3);
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
					if (currItem->itemType() != PageItem::PolyLine)
						currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
					AdjustItemSize(currItem);
					updateContents();
					emit PStatus(currItem->itemType(), currItem->PoLine.size());
					emit DocChanged();
					qApp->setOverrideCursor(QCursor(pointingHandCursor), true);
				}
				MarkClip(currItem);
				return;
			}
			if (GetItem(&currItem))
			{
				if (GroupSel)
				{
					p.begin(viewport());
					QRect ne = QRect();
					PaintSizeRect(&p, ne);
					p.end();
					double gx, gy, gh, gw;
					getGroupRect(&gx, &gy, &gw, &gh);
					mpo = QRect(qRound(m->x() / Scale) - Doc->guidesSettings.grabRad, qRound(m->y() / Scale) - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
					if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					      && (m->state() != (ControlButton | AltButton)) && (m->state() != ShiftButton))
					{
						HowTo = 0;
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
							HowTo = result[0];
							switch (HowTo)
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
							HowTo = 0;
						}
						if (HowTo != 0)
							mCG = true;
					}
					else
						SeleItem(m);
				}
				else
				{
					SeleItem(m);
					if (SelItem.count() != 0)
					{
						currItem = SelItem.at(0);
						p.begin(viewport());
						Transform(currItem, &p);
						if (!currItem->locked())
						{
							HandleSizer(&p, currItem, mpo, m);
							if (HowTo != 0)
							{
								if (currItem->itemType() != PageItem::Line)
									currItem->Sizing = true;
								mCG = true;
							}
						}
						p.end();
					}
				}
			}
			else
			{
				SeleItem(m);
				if (SelItem.count() == 0)
				{
					Mxp = qRound(m->x()/Scale);
					Myp = qRound(m->y()/Scale);
					SeRx = Mxp;
					SeRy = Myp;
				}
			}
			if (m->button() == MidButton)
			{
				MidButt = true;
				if (SelItem.count() != 0)
					Deselect(true);
				DrawNew();
			}
			if ((SelItem.count() != 0) && (m->button() == RightButton))
			{
				Mpressed = true;
				Dxp = Mxp;
				Dyp = Myp;
			}
			break;
		case DrawShapes:
			selectPage(m);
			switch (Doc->SubMode)
			{
			case 0:
				z = PaintRect(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen);
				SetupDraw(z);
				break;
			case 1:
				z = PaintEllipse(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen);
				SetupDraw(z);
				break;
			default:
				z = PaintPoly(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen);
				SetFrameShape(Doc->Items.at(z), Doc->ValCount, Doc->ShapeValues);
				Doc->Items.at(z)->FrameType = Doc->SubMode+2;
				SetupDraw(z);
				break;
			}
			emit HaveSel(PageItem::Polygon);
			break;
		case DrawPicture:
			selectPage(m);
			z = PaintPict(Rxp, Ryp, 1+Rxpd, 1+Rypd);
			SetupDraw(z);
			emit HaveSel(PageItem::ImageFrame);
			break;
		case DrawText:
			selectPage(m);
			z = PaintText(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dPenText);
			SetupDraw(z);
			emit HaveSel(PageItem::TextFrame);
			break;
		case Magnifier:
			Mpressed = true;
			if ((m->state() == ShiftButton) || (m->button() == RightButton))
				Magnify = false;
			else
				Magnify = true;
			break;
		case EditMode:
			HowTo = 0;
			HanMove = false;
//			slotDoCurs(false);
			if (GetItem(&currItem))
			{
				slotDoCurs(false);
				if (!currItem->locked())
				{
					p.begin(viewport());
					Transform(currItem, &p);
					HandleSizer(&p, currItem, mpo, m);
					p.end();
					if (HowTo != 0)
					{
						HanMove = true;
						slotDoCurs(true);
						return;
					}
				}
			}
			inText = slotSetCurs(m->x(), m->y());
			if (!inText)
			{
				Deselect(true);
				slotDoCurs(true);
				emit Amode(NormalMode);
				return;
			}
			currItem = SelItem.at(0);
			oldCp = currItem->CPos;
			slotDoCurs(true);
			if ((!inText) && ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)))
			{
				Deselect(true);
				if (SeleItem(m))
				{
					currItem = SelItem.at(0);
					if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame))
						emit Amode(EditMode);
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
				if ((m->button() == MidButton) && (currItem->itemType() == PageItem::TextFrame))
				{
					Mpressed = false;
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
							ss->GetText(currItem, st, currItem->IFont, currItem->ISize, true);
						delete ss;
						if (Doc->docHyphenator->AutoCheck)
							Doc->docHyphenator->slotHyphenate(currItem);
					}
					else
					{
						if (ScApp->Buffer2.startsWith("<SCRIBUSTEXT"))
							ScApp->slotEditPaste();
					}
					RefreshItem(currItem);
				}
			}
			break;
		case DrawLine:
			selectPage(m);
			z = PaintLine(Rxp, Ryp, 1+Rxpd, Rypd, Doc->toolSettings.dWidthLine, Doc->toolSettings.dPenLine);
			currItem = Doc->Items.at(z);
			currItem->Select = true;
			qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
			SelItem.clear();
			SelItem.append(currItem);
			currItem->paintObj();
			Imoved = true;
			emit ItemPos(currItem->Xpos, currItem->Ypos);
			emit SetSizeValue(currItem->Pwidth);
			emit SetLineArt(currItem->PLineArt, currItem->PLineEnd, currItem->PLineJoin);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
			emit ItemGradient(currItem->GrType);
			emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
			emit HaveSel(PageItem::Line);
			break;
		case Rotation:
			if (GetItem(&currItem))
			{
				RotMode = Doc->RotMode;
				if (GroupSel)
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
					RCenter = transformPointI(FPoint(currItem->Xpos+currItem->Width/2, currItem->Ypos+currItem->Height/2), 0, 0, currItem->Rot, 1, 1);
					if (QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->Width), static_cast<int>(currItem->Height))))).contains(mpo))
					{
						if (p.xForm(QRect(static_cast<int>(currItem->Width)-6, static_cast<int>(currItem->Height)-6, 6, 6)).intersects(mpo))
						{
							RCenter = FPoint(currItem->Xpos, currItem->Ypos);
							Doc->RotMode = 0;
						}
						if (p.xForm(QRect(0, 0, 6, 6)).intersects(mpo))
						{
							RCenter = transformPointI(FPoint(currItem->Xpos+currItem->Width, currItem->Ypos+currItem->Height), 0, 0, currItem->Rot, 1, 1);
							Doc->RotMode = 4;
						}
						if (p.xForm(QRect(0, static_cast<int>(currItem->Height)-6, 6, 6)).intersects(mpo))
						{
							RCenter = transformPointI(FPoint(currItem->Xpos+currItem->Width, currItem->Ypos), 0, 0, currItem->Rot, 1, 1);
							Doc->RotMode = 1;
						}
						if (p.xForm(QRect(static_cast<int>(currItem->Width)-6, 0, 6, 6)).intersects(mpo))
						{
							RCenter = transformPointI(FPoint(currItem->Xpos, currItem->Ypos+currItem->Height), 0, 0, currItem->Rot, 1, 1);
							Doc->RotMode = 3;
						}
						oldW = xy2Deg(m->x()/Scale - RCenter.x(), m->y()/Scale - RCenter.y());
						p.end();
					}
				}
			}
			break;
		case LinkFrames:
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
						Doc->Items.insert(currItem->ItemNr+1, bb);
						bb = Doc->Items.take(bb->ItemNr);
						for (uint a = 0; a < Doc->Items.count(); ++a)
						{
							Doc->Items.at(a)->ItemNr = a;
							if (Doc->Items.at(a)->isBookmark)
								emit NewBMNr(Doc->Items.at(a)->BMnr, a);
						}
					}
				}
				updateContents();
				emit DocChanged();
				Doc->ElemToLink = bb;
			}
			else
				Doc->ElemToLink = NULL;
			if (Doc->masterPageMode)
				Doc->MasterItems = Doc->Items;
			else
				Doc->DocItems = Doc->Items;
			break;
		case UnlinkFrames:
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
			if (Doc->masterPageMode)
				Doc->MasterItems = Doc->Items;
			else
				Doc->DocItems = Doc->Items;
			break;
		case DrawRegularPolygon:
			{
				selectPage(m);
				z = PaintPoly(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen);
				currItem = Doc->Items.at(z);
				FPointArray cli = RegularPolygonF(currItem->Width, currItem->Height, Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
				FPoint np = FPoint(cli.point(0));
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
				currItem->Select = true;
				qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
				SelItem.clear();
				SelItem.append(currItem);
				currItem->paintObj();
				Imoved = true;
				emit ItemPos(currItem->Xpos, currItem->Ypos);
				emit SetSizeValue(currItem->Pwidth);
				emit SetLineArt(currItem->PLineArt, currItem->PLineEnd, currItem->PLineJoin);
				emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
				emit ItemGradient(currItem->GrType);
				emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
				emit HaveSel(PageItem::Polygon);
				break;
			}
		case DrawBezierLine:
			if (m->button() == RightButton)
				break;
			if (FirstPoly)
			{
				selectPage(m);
				z = PaintPolyLine(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, "None", Doc->toolSettings.dPenLine);
				currItem = Doc->Items.at(z);
				SelItem.clear();
				SelItem.append(currItem);
				currItem->Select = true;
				qApp->setOverrideCursor(QCursor(crossCursor), true);
			}
			currItem = SelItem.at(0);
			p.begin(viewport());
			Transform(currItem, &p);
			npf = FPoint(p.xFormDev(m->pos()));
			p.end();
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
			SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
			currItem->paintObj();
			emit ItemPos(currItem->Xpos, currItem->Ypos);
			emit SetSizeValue(currItem->Pwidth);
			emit SetLineArt(currItem->PLineArt, currItem->PLineEnd, currItem->PLineJoin);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
			emit ItemGradient(currItem->GrType);
			emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
			emit HaveSel(PageItem::PolyLine);
			break;
		case InsertPDFButton:
		case InsertPDFTextfield:
		case InsertPDFCheckbox:
		case InsertPDFCombobox:
		case InsertPDFListbox:
		case InsertPDFTextAnnotation:
		case InsertPDFLinkAnnotation:
			selectPage(m);
			z = PaintText(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dPenText);
			currItem = Doc->Items.at(z);
			currItem->isAnnotation = true;
			switch (Doc->appMode)
			{
			case InsertPDFButton:
				currItem->AnType = 2;
				currItem->AnFlag = 65536;
				break;
			case InsertPDFTextfield:
				currItem->AnType = 3;
				break;
			case InsertPDFCheckbox:
				currItem->AnType = 4;
				break;
			case InsertPDFCombobox:
				currItem->AnType = 5;
				currItem->AnFlag = 131072;
				break;
			case InsertPDFListbox:
				currItem->AnType = 6;
				break;
			case InsertPDFTextAnnotation:
				currItem->AnType = 10;
				break;
			case InsertPDFLinkAnnotation:
				currItem->AnType = 11;
				currItem->AnZiel = Doc->currentPage->PageNr;
				currItem->AnAction = "0 0";
				currItem->setTextFlowsAroundFrame(false);
				break;
			}
			SetupDraw(z);
			emit HaveSel(PageItem::TextFrame);
			break;
		case DrawFreehandLine:
			RecordP.resize(0);
			Deselect(false);
			break;
		case DrawTable:
			Deselect(false);
			break;
		case PanningMode:
			break;
		case MeasurementTool:
		case EditGradientVectors:
			Mpressed = true;
			qApp->setOverrideCursor(QCursor(CrossCursor), true);
			Dxp = m->x();
			Dyp = m->y();
			Mxp = m->x();
			Myp = m->y();
			break;
		case CopyProperties:
			SeleItem(m);
			if (GetItem(&currItem))
			{
				currItem->ColGap = Doc->ElemToLink->ColGap;
				currItem->Cols = Doc->ElemToLink->Cols;
				currItem->BottomLine = Doc->ElemToLink->BottomLine;
				currItem->TopLine = Doc->ElemToLink->TopLine;
				currItem->LeftLine = Doc->ElemToLink->LeftLine;
				currItem->RightLine = Doc->ElemToLink->RightLine;
				currItem->BExtra = Doc->ElemToLink->BExtra;
				currItem->RExtra = Doc->ElemToLink->RExtra;
				currItem->Extra = Doc->ElemToLink->Extra;
				currItem->TExtra = Doc->ElemToLink->TExtra;
				currItem->setLineStyle(Doc->ElemToLink->lineStyle());
				currItem->setLineWidth(Doc->ElemToLink->lineWidth());
				currItem->setLineTransparency(Doc->ElemToLink->lineTransparency());
				currItem->setLineShade(Doc->ElemToLink->lineShade());
				currItem->setLineColor(Doc->ElemToLink->lineColor());
				currItem->setLineEnd(Doc->ElemToLink->lineEnd());
				currItem->setLineJoin(Doc->ElemToLink->lineJoin());
				currItem->setCustomLineStyle(Doc->ElemToLink->customLineStyle());
				currItem->setEndArrowIndex(Doc->ElemToLink->getEndArrowIndex());
				currItem->setStartArrowIndex(Doc->ElemToLink->getStartArrowIndex());
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
				Doc->appMode = NormalMode;
				emit PaintingDone();
			}
			break;
	}
}

bool ScribusView::ApplyGuides(double *x, double *y)
{
	bool ret = false;
	int pg = OnPage(*x, *y);
	if (pg == -1)
		return ret;
	Page* page = Doc->Pages.at(pg);
	if (Doc->SnapGuides)
	{
		if (page->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < page->YGuides.count(); ++yg)
			{
				if ((page->YGuides[yg]+page->Yoffset < (*y+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->Yoffset > (*y-Doc->guidesSettings.guideRad)))
				{
					*y= page->YGuides[yg]+page->Yoffset;
					ret = true;
					break;
				}
			}
		}
		if (page->XGuides.count() != 0)
		{
			for (uint xg = 0; xg < page->XGuides.count(); ++xg)
			{
				if ((page->XGuides[xg]+page->Xoffset < (*x+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->Xoffset > (*x-Doc->guidesSettings.guideRad)))
				{
					*x = page->XGuides[xg]+page->Xoffset;
					ret = true;
					break;
				}
			}
		}
		if ((page->Margins.Left+page->Xoffset < (*x+Doc->guidesSettings.guideRad)) && (page->Margins.Left+page->Xoffset > (*x-Doc->guidesSettings.guideRad)))
		{
			*x = page->Margins.Left+page->Xoffset;
			ret = true;
		}
		if (((page->Width - page->Margins.Right)+page->Xoffset < (*x+Doc->guidesSettings.guideRad)) && ((page->Width - page->Margins.Right)+page->Xoffset > (*x-Doc->guidesSettings.guideRad)))
		{
			*x = page->Width - page->Margins.Right+page->Xoffset;
			ret = true;
		}
		if ((page->Margins.Top+page->Yoffset < (*y+Doc->guidesSettings.guideRad)) && (page->Margins.Top+page->Yoffset > (*y-Doc->guidesSettings.guideRad)))
		{
			*y = page->Margins.Top+page->Yoffset;
			ret = true;
		}
		if (((page->Height - page->Margins.Bottom)+page->Yoffset < (*y+Doc->guidesSettings.guideRad)) && ((page->Height - page->Margins.Bottom)+page->Yoffset > (*y-Doc->guidesSettings.guideRad)))
		{
			*y = page->Height - page->Margins.Bottom+page->Yoffset;
			ret = true;
		}
	}
	return ret;
}

void ScribusView::SnapToGuides(PageItem *currItem)
{
	int pg = OnPage(currItem);
	if (pg == -1)
		return;
	Page* page = Doc->Pages.at(pg);
	if (page->YGuides.count() != 0)
	{
		for (uint yg = 0; yg < page->YGuides.count(); ++yg)
		{
			if ((page->YGuides[yg]+page->Yoffset < (currItem->Ypos+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->Yoffset > (currItem->Ypos-Doc->guidesSettings.guideRad)))
			{
				currItem->Ypos = page->YGuides[yg]+page->Yoffset;
				break;
			}
			if (currItem->itemType() == PageItem::Line)
			{
				QWMatrix ma;
				ma.translate(currItem->Xpos, currItem->Ypos);
				ma.rotate(currItem->Rot);
				double my = ma.m22() * currItem->Height + ma.m12() * currItem->Width + ma.dy();
				if ((page->YGuides[yg]+page->Yoffset < (my+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->Yoffset > (my-Doc->guidesSettings.guideRad)))
				{
					currItem->Ypos = currItem->Ypos + page->YGuides[yg] - my + page->Yoffset;
					break;
				}
			}
			else
			{
				if ((page->YGuides[yg]+page->Yoffset < (currItem->Ypos+currItem->Height+Doc->guidesSettings.guideRad)) &&
				     (page->YGuides[yg]+page->Yoffset > ((currItem->Ypos+currItem->Height)-Doc->guidesSettings.guideRad)))
				{
					currItem->Ypos = page->YGuides[yg]-currItem->Height+page->Yoffset;
					break;
				}
			}
		}
	}
	if (page->XGuides.count() != 0)
	{
		for (uint xg = 0; xg < page->XGuides.count(); ++xg)
		{
			if ((page->XGuides[xg]+page->Xoffset < (currItem->Xpos+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->Xoffset > (currItem->Xpos-Doc->guidesSettings.guideRad)))
			{
				currItem->Xpos = page->XGuides[xg]+page->Xoffset;
				break;
			}
			if (currItem->itemType() == PageItem::Line)
			{
				QWMatrix ma;
				ma.translate(currItem->Xpos, currItem->Ypos);
				ma.rotate(currItem->Rot);
				double mx = ma.m11() * currItem->Width + ma.m21() * currItem->Height + ma.dx();
				if ((page->XGuides[xg]+page->Xoffset < (mx+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->Xoffset > (mx-Doc->guidesSettings.guideRad)))
				{
					currItem->Xpos = currItem->Xpos + page->XGuides[xg] - mx + page->Xoffset;
					break;
				}
			}
			else
			{
				if ((page->XGuides[xg]+page->Xoffset < (currItem->Xpos+currItem->Width+Doc->guidesSettings.guideRad)) &&
				     (page->XGuides[xg]+page->Xoffset > ((currItem->Xpos+currItem->Width)-Doc->guidesSettings.guideRad)))
				{
					currItem->Xpos = page->XGuides[xg]-currItem->Width+page->Xoffset;
					break;
				}
			}
		}
	}
}

QPoint ScribusView::ApplyGrid(QPoint in)
{
	QPoint np;
	if ((Doc->useRaster) && (OnPage(in.x(), in.y()) != -1))
	{
		np.setX(static_cast<int>(qRound(in.x() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid));
		np.setY(static_cast<int>(qRound(in.y() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid));
	}
	else
		np = in;
	return np;
}

FPoint ScribusView::ApplyGridF(FPoint in)
{
	FPoint np;
	if ((Doc->useRaster) && (OnPage(in.x(), in.y()) != -1))
	{
		np.setX(qRound(in.x() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
		np.setY(qRound(in.y() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
	}
	else
		np = in;
	return np;
}

FPoint ScribusView::transformPointI(FPoint in, double dx, double dy, double rot, double sx, double sy)
{
	QWMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	ma = ma.invert();
	double x = ma.m11() * in.x() + ma.m21() * in.y() + ma.dx();
	double y = ma.m22() * in.y() + ma.m12() * in.x() + ma.dy();
	return FPoint(x, y);
}

FPoint ScribusView::transformPoint(FPoint in, double dx, double dy, double rot, double sx, double sy)
{
	QWMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	double x = ma.m11() * in.x() + ma.m21() * in.y() + ma.dx();
	double y = ma.m22() * in.y() + ma.m12() * in.x() + ma.dy();
	return FPoint(x, y);
}

void ScribusView::getBoundingRect(PageItem *currItem, double *x1, double *y1, double *x2, double *y2)
{
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	if (currItem->Rot != 0)
	{
		FPointArray pb;
		FPoint p1;
		pb.resize(0);
		pb.addPoint(FPoint(currItem->Xpos, currItem->Ypos));
		p1 = transformPoint(FPoint(currItem->Width, 0.0), currItem->Xpos, currItem->Ypos, currItem->Rot, 1.0, 1.0);
		pb.addPoint(p1);
		p1 = transformPoint(FPoint(currItem->Width, currItem->Height), currItem->Xpos, currItem->Ypos, currItem->Rot, 1.0, 1.0);
		pb.addPoint(p1);
		p1 = transformPoint(FPoint(0.0, currItem->Height), currItem->Xpos, currItem->Ypos, currItem->Rot, 1.0, 1.0);
		pb.addPoint(p1);
		for (uint pc = 0; pc < 4; ++pc)
		{
			minx = QMIN(minx, pb.point(pc).x());
			miny = QMIN(miny, pb.point(pc).y());
			maxx = QMAX(maxx, pb.point(pc).x());
			maxy = QMAX(maxy, pb.point(pc).y());
		}
		*x1 = minx;
		*y1 = miny;
		*x2 = maxx;
		*y2 = maxy;
	}
	else
	{
		*x1 = currItem->Xpos;
		*y1 = currItem->Ypos;
		*x2 = currItem->Xpos + currItem->Width;
		*y2 = currItem->Ypos + currItem->Height;
	}
}

QRect ScribusView::getRedrawBounding(PageItem *currItem)
{
	int x = qRound(floor(currItem->BoundingX - currItem->OldPwidth / 2.0 - 5) * Scale);
	int y = qRound(floor(currItem->BoundingY - currItem->OldPwidth / 2.0 - 5) * Scale);
	int w = qRound(ceil(currItem->BoundingW + currItem->OldPwidth + 10) * Scale);
	int h = qRound(ceil(currItem->BoundingH + currItem->OldPwidth + 10) * Scale);
	return QRect(x, y, w, h);
}

void ScribusView::setRedrawBounding(PageItem *currItem)
{
	double bw, bh;
	getBoundingRect(currItem, &currItem->BoundingX, &currItem->BoundingY, &bw, &bh);
	currItem->BoundingW = bw - currItem->BoundingX;
	currItem->BoundingH = bh - currItem->BoundingY;
	if (currItem->itemType() == PageItem::Line)
		currItem->BoundingH = QMAX(currItem->BoundingH, 1);
}

void ScribusView::setGroupRect()
{
	PageItem *currItem;
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	for (uint gc = 0; gc < SelItem.count(); ++gc)
	{
		currItem = SelItem.at(gc);
		if (currItem->Rot != 0)
		{
			FPointArray pb;
			FPoint p1;
			pb.resize(0);
			pb.addPoint(FPoint(currItem->Xpos, currItem->Ypos));
			p1 = transformPoint(FPoint(currItem->Width, 0.0), currItem->Xpos, currItem->Ypos, currItem->Rot, 1.0, 1.0);
			pb.addPoint(p1);
			p1 = transformPoint(FPoint(currItem->Width, currItem->Height), currItem->Xpos, currItem->Ypos, currItem->Rot, 1.0, 1.0);
			pb.addPoint(p1);
			p1 = transformPoint(FPoint(0.0, currItem->Height), currItem->Xpos, currItem->Ypos, currItem->Rot, 1.0, 1.0);
			pb.addPoint(p1);
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
			minx = QMIN(minx, currItem->Xpos);
			miny = QMIN(miny, currItem->Ypos);
			maxx = QMAX(maxx, currItem->Xpos + currItem->Width);
			maxy = QMAX(maxy, currItem->Ypos + currItem->Height);
		}
	}
	GroupX = minx;
	GroupY = miny;
	GroupW = maxx - minx;
	GroupH = maxy - miny;
	GroupSel = true;
}

void ScribusView::getGroupRectScreen(double *x, double *y, double *w, double *h)
{
	QPoint in  = QPoint(qRound(GroupX*Scale), qRound(GroupY*Scale));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = GroupW*Scale;
	*h = GroupH*Scale;
}

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

void ScribusView::PaintSizeRect(QPainter *p, QRect neu)
{
	static QRect old;
	if (!neu.isNull())
	{
		QWMatrix ma = p->worldMatrix();
		ma.setTransformationMode ( QWMatrix::Areas );
		p->setWorldMatrix(ma);
		p->setRasterOp(XorROP);
		p->setBrush(NoBrush);
		p->setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		if (!old.isNull())
			p->drawRect(old);
		p->drawRect(neu);
	}
	old = neu;
}

void ScribusView::ToView(QPainter *p)
{
	QPoint out = contentsToViewport(QPoint(0, 0));
	p->translate(out.x(), out.y());
}

void ScribusView::RefreshItem(PageItem *currItem)
{
	updateContents(getRedrawBounding(currItem));
}

bool ScribusView::MoveItem(double newX, double newY, PageItem* currItem, bool fromMP)
{
	if (currItem->locked())
		return false;
	bool retw = false;
	double oldx = currItem->Xpos;
	double oldy = currItem->Ypos;
	currItem->Xpos += newX;
	currItem->Ypos += newY;
	if ((Doc->useRaster) && (!Imoved) && (!fromMP) && (static_cast<int>(Doc->currentPage->PageNr) == currItem->OwnPage))
	{
		currItem->Xpos = qRound(currItem->Xpos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
		currItem->Ypos = qRound(currItem->Ypos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
	}
	if ((Doc->SnapGuides) && (!Imoved) && (Doc->appMode == NormalMode) && (!Doc->EditClip) && (!fromMP))
		SnapToGuides(currItem);
	if ((currItem->Xpos != oldx) || (currItem->Ypos != oldy))
		retw = true;
	if (!fromMP)
	{
		if (GroupSel)
		{
			double gx, gy, gh, gw;
			setGroupRect();
			getGroupRect(&gx, &gy, &gw, &gh);
			emit ItemPos(gx, gy);
		}
		else
			emit ItemPos(currItem->Xpos, currItem->Ypos);
	}
/*	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr); */
	QRect oldR = getRedrawBounding(currItem);
	setRedrawBounding(currItem);
	QRect newR = getRedrawBounding(currItem);
	if ((!Imoved) && (!currItem->Sizing))
		updateContents(newR.unite(oldR));
	currItem->OwnPage = OnPage(currItem);
	return retw;
}

void ScribusView::MoveItemI(double newX, double newY, int ite, bool redraw)
{
	PageItem *currItem = Doc->Items.at(ite);
	if ((currItem->locked()) || (!currItem->ScaleType))
		return;
	if (currItem->imageFlippedH())
		currItem->LocalX -= newX;
	else
		currItem->LocalX += newX;
	if (currItem->imageFlippedV())
		currItem->LocalY -= newY;
	else
		currItem->LocalY += newY;
	if (currItem->imageClip.size() != 0)
	{
		currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
		QWMatrix cl;
		cl.translate(currItem->LocalX*currItem->LocalScX, currItem->LocalY*currItem->LocalScY);
		cl.scale(currItem->LocalScX, currItem->LocalScY);
		currItem->imageClip.map(cl);
	}
	if (redraw)
		updateContents(getRedrawBounding(currItem));
	emit SetLocalValues(currItem->LocalScX, currItem->LocalScY, currItem->LocalX, currItem->LocalY);
}

void ScribusView::ConvertClip(PageItem *currItem)
{
	if (currItem->Clip.count() != 0)
	{
		FPoint np = FPoint(currItem->Clip.point(0));
		currItem->PoLine.resize(2);
		currItem->PoLine.setPoint(0, np);
		currItem->PoLine.setPoint(1, np);
		for (uint a = 1; a < currItem->Clip.size(); ++a)
		{
			np = FPoint(currItem->Clip.point(a));
			currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
		}
		np = FPoint(currItem->Clip.point(0));
		currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	}
	else
		SetRectFrame(currItem);
}

void ScribusView::UpdateClip(PageItem* currItem)
{
	if (Doc->appMode == DrawBezierLine)
		return;
	int ph = static_cast<int>(QMAX(1.0, currItem->Pwidth / 2.0));
	switch (currItem->itemType())
	{
	case PageItem::Line:
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->Width+ph),-ph,
		                  static_cast<int>(currItem->Width+ph),static_cast<int>(currItem->Height+ph),
		                  -ph,static_cast<int>(currItem->Height+ph));
		break;
	default:
		if (((!currItem->ClipEdited) || (currItem->FrameType < 3)) && (currItem->itemType() != PageItem::PathText))
		{
			switch (currItem->FrameType)
			{
			case 0:
				SetRectFrame(currItem);
				break;
			case 1:
				SetOvalFrame(currItem);
				break;
			case 2:
				SetFrameRound(currItem);
				break;
			default:
				break;
			}
			if ((currItem->OldB2 != 0) && (currItem->OldH2 != 0))
			{
				double scx = currItem->Width / currItem->OldB2;
				double scy = currItem->Height / currItem->OldH2;
				QWMatrix ma;
				ma.scale(scx, scy);
				FPointArray gr;
				gr.addPoint(currItem->GrStartX, currItem->GrStartY);
				gr.addPoint(currItem->GrEndX, currItem->GrEndY);
				gr.map(ma);
				currItem->GrStartX = gr.point(0).x();
				currItem->GrStartY = gr.point(0).y();
				currItem->GrEndX = gr.point(1).x();
				currItem->GrEndY = gr.point(1).y();
				if (currItem->FrameType > 2)
				{
					currItem->PoLine.map(ma);
					currItem->ContourLine.map(ma);
					if (currItem->itemType() == PageItem::PathText)
						UpdatePolyClip(currItem);
					else
						currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
				}
			}
			currItem->OldB2 = currItem->Width;
			currItem->OldH2 = currItem->Height;
			if (currItem->FrameType < 3)
				currItem->ContourLine = currItem->PoLine.copy();
		}
		else
		{
			if (Doc->SubMode != -1)
			{
				switch (Doc->SubMode)
				{
				case 0:
					SetRectFrame(currItem);
					break;
				case 1:
					SetOvalFrame(currItem);
					break;
				default:
					SetFrameShape(currItem, Doc->ValCount, Doc->ShapeValues);
					break;
				}
				currItem->OldB2 = currItem->Width;
				currItem->OldH2 = currItem->Height;
				currItem->ContourLine = currItem->PoLine.copy();
			}
			if ((currItem->OldB2 == 0) || (currItem->OldH2 == 0))
				return;
			double scx = currItem->Width / currItem->OldB2;
			double scy = currItem->Height / currItem->OldH2;
			QWMatrix ma;
			ma.scale(scx, scy);
			FPointArray gr;
			gr.addPoint(currItem->GrStartX, currItem->GrStartY);
			gr.addPoint(currItem->GrEndX, currItem->GrEndY);
			gr.map(ma);
			currItem->GrStartX = gr.point(0).x();
			currItem->GrStartY = gr.point(0).y();
			currItem->GrEndX = gr.point(1).x();
			currItem->GrEndY = gr.point(1).y();
			currItem->PoLine.map(ma);
			currItem->ContourLine.map(ma);
			if (currItem->itemType() == PageItem::PathText)
				UpdatePolyClip(currItem);
			else
				currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
			currItem->OldB2 = currItem->Width;
			currItem->OldH2 = currItem->Height;
		}
		break;
	}
	updateGradientVectors(currItem);
}

void ScribusView::SetPolyClip(PageItem *currItem, int up)
{
	QPoint np, np2;
	QPointArray cl, cl1, cl2;
	double rot;
	currItem->Clip.resize(0);
	if (currItem->PoLine.size() < 4)
		return;
	cl = FlattenPath(currItem->PoLine, currItem->Segments);
	for (uint a = 0; a < cl.size()-1; ++a)
	{
		rot = xy2Deg(cl.point(a+1).x()-cl.point(a).x(),cl.point(a+1).y()-cl.point(a).y());
		QWMatrix ma;
		ma.rotate(rot);
		np = ma*QPoint(0, -up);
		cl1.resize(cl1.size()+1);
		cl1.setPoint(cl1.size()-1, np+cl.point(a));
		cl1.resize(cl1.size()+1);
		cl1.setPoint(cl1.size()-1, np+cl.point(a+1));
		cl2.resize(cl2.size()+1);
		cl2.setPoint(cl2.size()-1, np2+cl.point(a));
		cl2.resize(cl2.size()+1);
		cl2.setPoint(cl2.size()-1, np2+cl.point(a+1));
	}
	cl1.resize(cl1.size()+1);
	cl1.setPoint(cl1.size()-1, np+cl.point(cl.size()-1));
	cl2.resize(cl2.size()+1);
	cl2.setPoint(cl2.size()-1, np2+cl.point(cl.size()-1));
	currItem->Clip.putPoints(currItem->Clip.size(), cl1.size(), cl1);
	for (int a2 = cl2.size()-1; a2 > -1; a2--)
	{
		currItem->Clip.resize(currItem->Clip.size()+1);
		currItem->Clip.setPoint(currItem->Clip.size()-1, cl2.point(a2));
	}
}

void ScribusView::UpdatePolyClip(PageItem *currItem)
{
	struct ScText *hl;
	int asce = 1;
	int desc = 1;
	for (uint a = 0; a < currItem->itemText.count(); ++a)
	{
		hl = currItem->itemText.at(a);
		int des = static_cast<int>(hl->cfont->numDescender * (-hl->csize / 10.0));
		int asc = static_cast<int>(hl->cfont->numAscent * (hl->csize / 10.0));
		if (asc > asce)
			asce = asc;
		if (des > desc)
			desc = des;
	}
	SetPolyClip(currItem, static_cast<int>(asce+currItem->BaseOffs));
}

void ScribusView::MarkClip(PageItem *currItem)
{
	double x, y;
	QPainter p;
	FPointArray cli;
	QPointArray Bez(4);
	p.begin(viewport());
	ToView(&p);
	Transform(currItem, &p);
	p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
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
			p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
			BezierPoints(&Bez, cli.pointQ(poi), cli.pointQ(poi+1), cli.pointQ(poi+3), cli.pointQ(poi+2));
			p.drawCubicBezier(Bez);
			p.setPen(QPen(blue, 1, DotLine, FlatCap, MiterJoin));
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
			p.setPen(QPen(magenta, 8, SolidLine, RoundCap, MiterJoin));
			cli.point(a+1, &x, &y);
			p.drawLine(qRound(x), qRound(y), qRound(x), qRound(y));
			p.setPen(QPen(blue, 8, SolidLine, RoundCap, MiterJoin));
			cli.point(a, &x, &y);
			p.drawLine(qRound(x), qRound(y), qRound(x), qRound(y));
		}
		else
		{
			p.setPen(QPen(blue, 8, SolidLine, RoundCap, MiterJoin));
			cli.point(a, &x, &y);
			p.drawLine(qRound(x), qRound(y), qRound(x), qRound(y));
			p.setPen(QPen(magenta, 8, SolidLine, RoundCap, MiterJoin));
			cli.point(a+1, &x, &y);
			p.drawLine(qRound(x), qRound(y), qRound(x), qRound(y));
		}
	}
	if (ClRe != -1)
	{
		p.setPen(QPen(red, 8, SolidLine, RoundCap, MiterJoin));
		cli.point(ClRe, &x, &y);
		p.drawLine(qRound(x), qRound(y), qRound(x), qRound(y));
		QValueList<int>::Iterator itm;
		for (itm = SelNode.begin(); itm != SelNode.end(); ++itm)
		{
			cli.point((*itm), &x, &y);
			p.drawLine(qRound(x), qRound(y), qRound(x), qRound(y));
		}
		emit HavePoint(true, MoveSym);
	}
	else
		emit HavePoint(false, MoveSym);
	p.end();
}

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

void ScribusView::MirrorPolyH()
{
	PageItem *currItem = SelItem.at(0);
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (EditContour)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
			ss->set("MIRROR_PATH_H", "mirror_path_h");
			ss->set("IS_CONTOUR", true);
			undoManager->action(currItem, ss, Um::IBorder);
		}
		FPoint tp, tp2;
		tp2 = getMinClipF(&currItem->ContourLine);
		tp = getMaxClipF(&currItem->ContourLine);
		ma.translate(qRound(tp.x()), 0);
		ma.scale(-1, 1);
		currItem->ContourLine.map(ma);
		RefreshItem(currItem);
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		MarkClip(currItem);
		return;
	}
	ma.scale(-1, 1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(currItem->Width, 0);
	if (currItem->itemType() == PageItem::PathText)
		UpdatePolyClip(currItem);
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	setRedrawBounding(currItem);
	RefreshItem(currItem);
	MarkClip(currItem);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
		ss->set("MIRROR_PATH_H", "mirror_path_h");
		ss->set("IS_CONTOUR", false);
		undoManager->action(currItem, ss, Um::IBorder);
	}
}

void ScribusView::MirrorPolyV()
{
	PageItem *currItem = SelItem.at(0);
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (EditContour)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
			ss->set("MIRROR_PATH_V", "mirror_path_v");
			ss->set("IS_CONTOUR", true);
			undoManager->action(currItem, ss, Um::IBorder);
		}
		FPoint tp, tp2;
		tp2 = getMinClipF(&currItem->ContourLine);
		tp = getMaxClipF(&currItem->ContourLine);
		ma.translate(0, qRound(tp.y()));
		ma.scale(1, -1);
		currItem->ContourLine.map(ma);
		RefreshItem(currItem);
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		MarkClip(currItem);
		return;
	}
	ma.scale(1, -1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(0, currItem->Height);
	if (currItem->itemType() == PageItem::PathText)
		UpdatePolyClip(currItem);
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	setRedrawBounding(currItem);
	RefreshItem(currItem);
	MarkClip(currItem);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
		ss->set("MIRROR_PATH_V", "mirror_path_v");
		ss->set("IS_CONTOUR", false);
		undoManager->action(currItem, ss, Um::IBorder);
	}
}

void ScribusView::TransformPoly(int mode, int rot, double scaling)
{
	PageItem *currItem = SelItem.at(0);
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (EditContour)
	{
		FPoint tp, tp2;
		tp2 = getMinClipF(&currItem->ContourLine);
		tp = getMaxClipF(&currItem->ContourLine);
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
		}
		currItem->ContourLine.map(ma);
		currItem->ContourLine.translate(qRound((tp.x() + tp2.x()) / 2.0), qRound((tp.y() + tp2.y()) / 2.0));
		RefreshItem(currItem);
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		MarkClip(currItem);
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
		return;
	}
	FPoint oldPos = FPoint(currItem->Xpos, currItem->Ypos);
	double offsX = currItem->Width / 2.0;
	double offsY = currItem->Height / 2.0;
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
	}
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(offsX, offsY);
	AdjustItemSize(currItem);
	QWMatrix ma2;
	ma2.translate(oldPos.x(), oldPos.y());
	ma2.scale(1, 1);
	ma2.translate(offsX, offsY);
	FPoint n = FPoint(-offsX, -offsY);
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
	}
	double x = ma2.m11() * n.x() + ma2.m21() * n.y() + ma2.dx();
	double y = ma2.m22() * n.y() + ma2.m12() * n.x() + ma2.dy();
	MoveItem(x-oldPos.x(), y-oldPos.y(), currItem);
	if (currItem->itemType() == PageItem::PathText)
		UpdatePolyClip(currItem);
	setRedrawBounding(currItem);
	RefreshItem(currItem);
	MarkClip(currItem);
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
}

void ScribusView::Reset1Control()
{
	PageItem *currItem = SelItem.at(0);
	currItem->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = currItem->ContourLine.point(ClRe-1);
	else
		np = currItem->PoLine.point(ClRe-1);
	currItem->OldB2 = currItem->Width;
	currItem->OldH2 = currItem->Height;
	if (EditContour)
	{
		currItem->ContourLine.setPoint(ClRe, np);
		RefreshItem(currItem);
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
	MarkClip(currItem);
}

void ScribusView::ResetControl()
{
	PageItem *currItem = SelItem.at(0);
	currItem->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = currItem->ContourLine.point(ClRe);
	else
		np = currItem->PoLine.point(ClRe);
	currItem->OldB2 = currItem->Width;
	currItem->OldH2 = currItem->Height;
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
		RefreshItem(currItem);
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
	}
	MarkClip(currItem);
}

void ScribusView::MoveClipPoint(PageItem *currItem, FPoint ip)
{
	if (((EdPoints) && (ClRe % 2 != 0)) || ((!EdPoints) && (ClRe % 2 == 0)))
		return;
	currItem->ClipEdited = true;
	FPointArray Clip;
	if (EditContour)
		Clip = currItem->ContourLine;
	else
		Clip = currItem->PoLine;
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
	FPoint np = ip;
	if (ClRe != -1)
	{
		if ((np.x() < 0) && (!EditContour))
		{
			SizeItem(currItem->Width - np.x(), currItem->Height, currItem->ItemNr, false, false);
			if (currItem->Rot != 0)
			{
				FPoint npv = FPoint(np.x(), 0);
				MoveRotated(currItem, npv);
			}
			else
				MoveItem(np.x(), 0, currItem);
			Clip.translate(-np.x(), 0);
			if (!currItem->imageFlippedH())
				MoveItemI(-np.x()/currItem->LocalScX, 0, currItem->ItemNr, false);
			np.setX(0);
		}
		if ((np.y() < 0) && (!EditContour))
		{
			SizeItem(currItem->Width, currItem->Height - np.y(), currItem->ItemNr, false, false);
			if (currItem->Rot != 0)
			{
				FPoint npv = FPoint(0, np.y());
				MoveRotated(currItem, npv);
			}
			else
				MoveItem(0, np.y(), currItem);
			Clip.translate(0, -np.y());
			if (!currItem->imageFlippedV())
				MoveItemI(0, -np.y()/currItem->LocalScY, currItem->ItemNr, false);
			np.setY(0);
		}
		emit ClipPo(np.x(), np.y());
		if ((ClRe+1 < static_cast<int>(EndInd)) && (ClRe % 2 == 0))
		{
			FPoint ap = Clip.point(ClRe);
			FPoint ap2 = Clip.point(ClRe+1);
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+1, ap2);
		}
		Clip.setPoint(ClRe, np);
		if (((ClRe % 4 != 0) && (ClRe % 2 == 0)) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap = Clip.point(ClRe+2);
			FPoint ap2 = Clip.point(ClRe+3);
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+3, ap2);
			Clip.setPoint(ClRe+2, np);
		}
		if ((ClRe % 4 == 0) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap = Clip.point(ClRe-2);
			FPoint ap2 = Clip.point(ClRe-1);
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe-1, ap2);
			Clip.setPoint(ClRe-2, np);
		}
		if (((ClRe == static_cast<int>(StartInd)) || (ClRe == static_cast<int>(EndInd-2))) &&
		        ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)))
		{
			if (ClRe == static_cast<int>(StartInd))
			{
				FPoint ap = Clip.point(EndInd-2);
				FPoint ap2 = Clip.point(EndInd-1);
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(EndInd-2, Clip.point(StartInd));
				Clip.setPoint(EndInd-1, ap2);
			}
			else
			{
				FPoint ap = Clip.point(StartInd);
				FPoint ap2 = Clip.point(StartInd + 1);
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(StartInd, Clip.point(EndInd-2));
				Clip.setPoint(StartInd + 1, ap2);
			}
		}
		if (((ClRe == static_cast<int>(StartInd+1)) || (ClRe == static_cast<int>(EndInd-1))) &&
		        ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)) && (MoveSym))
		{
			uint kon = 0;
			if (ClRe == static_cast<int>(StartInd+1))
				kon = EndInd-1;
			else
				kon = StartInd + 1;
			FPoint lxy = Clip.point(ClRe-1);
			FPoint lk = Clip.point(ClRe);
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
			FPoint lxy = Clip.point(ClRe-1);
			FPoint lk = Clip.point(ClRe);
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if (EditContour)
			currItem->ContourLine = Clip.copy();
		else
			currItem->PoLine = Clip.copy();
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
		if (!EditContour)
			AdjustItemSize(currItem);
		else
		{
			RefreshItem(currItem);
			currItem->FrameOnly = true;
			currItem->Tinput = true;
			currItem->paintObj();
			currItem->FrameOnly = false;
			MarkClip(currItem);
		}
	}
}

bool ScribusView::SizeItem(double newX, double newY, int ite, bool fromMP, bool DoUpdateClip, bool redraw)
{
	return SizeItem(newX, newY, Doc->Items.at(ite), fromMP, DoUpdateClip, redraw);
}

bool ScribusView::SizeItem(double newX, double newY, PageItem *pi, bool fromMP, bool DoUpdateClip, bool redraw)
{
	PageItem *currItem = pi;
	if (currItem->locked())
		return false;
	QPainter p;
	QRect oldR = getRedrawBounding(currItem);
	if (currItem->itemType() != PageItem::Line)
	{
		newX = QMAX(newX, 1);
		newY = QMAX(newY, 1);
	}
	int ph = static_cast<int>(QMAX(1.0, currItem->Pwidth / 2.0));
	QWMatrix ma;
	ma.rotate(currItem->Rot);
	double dX = ma.m11() * (currItem->Width - newX) + ma.m21() * (currItem->Height - newY) + ma.dx();
	double dY = ma.m22() * (currItem->Height - newY) + ma.m12() * (currItem->Width - newX) + ma.dy();
	currItem->Width = newX;
	currItem->Height = newY;
	if ((Doc->RotMode != 0) && (fromMP))
	{
		switch (Doc->RotMode)
		{
		case 2:
			MoveItem(dX / 2.0, dY / 2.0, currItem);
			break;
		case 4:
			MoveItem(dX, dY, currItem);
			break;
		case 3:
			MoveItem(0.0, dY, currItem);
			break;
		case 1:
			MoveItem(dX, 0.0, currItem);
			break;
		}
	}
	currItem->RadRect = QMIN(currItem->RadRect, QMIN(currItem->Width,currItem->Height)/2);
	if ((currItem->itemType() == PageItem::ImageFrame) && (!currItem->Sizing) && (!Doc->EditClip))
		AdjustPictScale(currItem);
	if (currItem->itemType() == PageItem::Line)
	{
		if (!fromMP)
		{
			currItem->Rot = atan2(currItem->Height,currItem->Width)*(180.0/M_PI);
			currItem->Width = sqrt(pow(currItem->Width,2)+pow(currItem->Height,2));
			currItem->Height = 1;
			emit SetAngle(currItem->Rot);
		}
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->Width+ph),-ph,
		                  static_cast<int>(currItem->Width+ph),static_cast<int>(currItem->Height+ph),
		                  -ph,static_cast<int>(currItem->Height+ph));
	}
	setRedrawBounding(currItem);
	currItem->OwnPage = OnPage(currItem);
	if (currItem->Sizing)
	{
		emit ItemGeom(currItem->Width, currItem->Height);
		emit ItemRadius(currItem->RadRect);
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		if ((HowTo == 1) && (currItem->itemType() != PageItem::Line))
			currItem->paintObj();
		if ((currItem->FrameType == 0) || (currItem->itemType() == PageItem::Line) || (HowTo != 1))
			return true;
		QPainter p;
		p.begin(viewport());
		QPoint in  = QPoint(qRound(currItem->Xpos*Scale), qRound(currItem->Ypos*Scale));
		QPoint out = contentsToViewport(in);
		p.translate(out.x(), out.y());
		p.scale(Scale, Scale);
		p.rotate(currItem->Rot);
		p.setRasterOp(XorROP);
		p.setBrush(NoBrush);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		currItem->DrawPolyL(&p, currItem->Clip);
		UpdateClip(currItem);
		updateGradientVectors(currItem);
		currItem->DrawPolyL(&p, currItem->Clip);
		p.end();
		return true;
	}
	if (DoUpdateClip)
	{
		if (fromMP)
		{
			if (currItem->imageFlippedH())
				MoveItemI(-(currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
			if (currItem->imageFlippedV())
				MoveItemI(0, -(currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
		}
		else
		{
			if (!currItem->imageFlippedH())
				MoveItemI((currItem->Width - currItem->OldB2)/currItem->LocalScX, 0, currItem->ItemNr, false);
			if (!currItem->imageFlippedV())
				MoveItemI(0, (currItem->Height - currItem->OldH2)/currItem->LocalScY, currItem->ItemNr, false);
		}
		UpdateClip(currItem);
	}
	updateGradientVectors(currItem);
	if (redraw)
	{
		QRect newR = getRedrawBounding(currItem);
		updateContents(newR.unite(oldR));
	}
	if (!fromMP)
	{
		if (GroupSel)
		{
			double gx, gy, gh, gw;
			setGroupRect();
			getGroupRect(&gx, &gy, &gw, &gh);
			emit ItemGeom(gw, gh);
		}
		else
			emit ItemGeom(currItem->Width, currItem->Height);
	}
	emit ItemRadius(currItem->RadRect);
	return true;
}

void ScribusView::MoveRotated(PageItem *currItem, FPoint npv, bool fromMP)
{
	QWMatrix ma;
	ma.translate(currItem->Xpos, currItem->Ypos);
	ma.rotate(currItem->Rot);
	double mxc = currItem->Xpos - (ma.m11() * npv.x() + ma.m21() * npv.y() + ma.dx());
	double myc = currItem->Ypos - (ma.m22() * npv.y() + ma.m12() * npv.x() + ma.dy());
	MoveItem(-mxc, -myc, currItem, fromMP);
}

bool ScribusView::MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP)
{
	PageItem *currItem = Doc->Items.at(ite);
	QRect oldR = getRedrawBounding(currItem);
	if (currItem->itemType() == PageItem::Line)
	{
		QWMatrix ma;
		ma.translate(currItem->Xpos, currItem->Ypos);
		ma.rotate(currItem->Rot);
		double mx = ma.m11() * currItem->Width  + ma.dx();
		double my = ma.m12() * currItem->Width + ma.dy();
		MoveItem(newX.x(), newX.y(), currItem, fromMP);
		currItem->Rot = xy2Deg(mx - currItem->Xpos, my - currItem->Ypos);
		currItem->Width = sqrt(pow(mx - currItem->Xpos,2)+pow(my - currItem->Ypos,2));
		currItem->Height = 1;
		UpdateClip(currItem);
		setRedrawBounding(currItem);
		QRect newR = getRedrawBounding(currItem);
		updateContents(newR.unite(oldR));
		emit ItemPos(currItem->Xpos, currItem->Ypos);
		emit ItemGeom(currItem->Width, currItem->Height);
		emit SetAngle(currItem->Rot);
	}
	else
	{
		currItem->OldB2 = currItem->Width;
		currItem->OldH2 = currItem->Height;
		if (currItem->Rot != 0)
		{
			FPoint npv = FPoint(newX.x(), newX.y());
			QWMatrix ma3;
			ma3.translate(currItem->Xpos, currItem->Ypos);
			ma3.rotate(currItem->Rot);
			double mxc3 = currItem->Xpos - (ma3.m11() * npv.x() + ma3.m21() * npv.y() + ma3.dx());
			double myc3 = currItem->Ypos - (ma3.m22() * npv.y() + ma3.m12() * npv.x() + ma3.dy());
			SizeItem(currItem->Width - newY.x(), currItem->Height - newY.y(), ite, fromMP, true, false);
			MoveItem(-mxc3, -myc3, currItem, fromMP);
		}
		else
		{
			SizeItem(currItem->Width - newY.x(), currItem->Height - newY.y(), ite, fromMP, true, false);
			MoveItem(newX.x(), newX.y(), currItem, fromMP);
		}
	}
	return true;
}

void ScribusView::moveGroup(double x, double y, bool fromMP)
{
	if (!_groupTransactionStarted && SelItem.count() > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < SelItem.count(); ++i)
			tooltip += "\t" + SelItem.at(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Move, tooltip, Um::IMove);
		_groupTransactionStarted = true;
	}
	PageItem* currItem;
	QPainter p;
	double gx, gy, gw, gh;
	double sc = Scale;
	if (GroupSel)
	{
		p.begin(viewport());
		getGroupRectScreen(&gx, &gy, &gw, &gh);
		PaintSizeRect(&p, QRect(qRound(gx), qRound(gy), qRound(gw), qRound(gh)));
		p.end();
	}
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		currItem = SelItem.at(a);
		if (!fromMP)
		{
			p.begin(viewport());
			ToView(&p);
			p.translate(qRound(currItem->Xpos*Scale), qRound(currItem->Ypos*sc));
			p.scale(sc, sc);
			p.rotate(currItem->Rot);
			p.setRasterOp(XorROP);
			p.setBrush(NoBrush);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if ((currItem->itemType() != PageItem::Line) && (currItem->FrameType != 0) || (currItem->itemType() == PageItem::PolyLine))
				currItem->DrawPolyL(&p, currItem->Clip);
			else
				p.drawRect(0, 0, static_cast<int>(currItem->Width)+1, static_cast<int>(currItem->Height)+1);
			p.end();
		}
		MoveItem(x, y, currItem, fromMP);
		if (!fromMP)
		{
			p.begin(viewport());
			ToView(&p);
			p.translate(qRound(currItem->Xpos*sc), qRound(currItem->Ypos*sc));
			p.scale(sc, sc);
			p.rotate(currItem->Rot);
			p.setRasterOp(XorROP);
			p.setBrush(NoBrush);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if ((currItem->itemType() != PageItem::Line) && (currItem->FrameType != 0) || (currItem->itemType() == PageItem::PolyLine))
				currItem->DrawPolyL(&p, currItem->Clip);
			else
				p.drawRect(0, 0, static_cast<int>(currItem->Width)+1, static_cast<int>(currItem->Height)+1);
			p.end();
		}
	}
	if (GroupSel)
	{
		currItem = SelItem.at(0);
		GroupOnPage(currItem);
		if (fromMP)
			updateContents(QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10)));
	}
}

void ScribusView::RotateGroup(double win)
{
	if (!_groupTransactionStarted && SelItem.count() > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < SelItem.count(); ++i)
			tooltip += "\t" + SelItem.at(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Rotate, tooltip, Um::IRotate);
		_groupTransactionStarted = true;
	}
	double gxS, gyS, ghS, gwS;
	double sc = Scale;
	PageItem* currItem;
	FPoint n;
	getGroupRect(&gxS, &gyS, &gwS, &ghS);
	QWMatrix ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(win);
	QRect oldR = QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10));
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		currItem = SelItem.at(a);
		n = FPoint(currItem->Xpos - RCenter.x(), currItem->Ypos - RCenter.y());
		currItem->Xpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		currItem->Ypos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		currItem->Rot += win;
		setRedrawBounding(currItem);
	}
	currItem = SelItem.at(0);
	GroupOnPage(currItem);
	setGroupRect();
	getGroupRect(&gxS, &gyS, &gwS, &ghS);
	updateContents(QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10)).unite(oldR));
}

void ScribusView::scaleGroup(double scx, double scy)
{
	if (!_groupTransactionStarted && SelItem.count() > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < SelItem.count(); ++i)
			tooltip += "\t" + SelItem.at(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Resize, tooltip, Um::IResize);
		_groupTransactionStarted = true;
	}
	PageItem *bb;
	double gx, gy, gh, gw, x, y;
	uint aa;
	double sc = Scale;
	FPoint n;
	getGroupRect(&gx, &gy, &gw, &gh);
	QRect oldR = QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10));
	setUpdatesEnabled(false);
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		bb = SelItem.at(a);
		if ((bb->locked()) || (bb->sizeLocked()))
			continue;
		bb->OldB = bb->Width;
		bb->OldH = bb->Height;
		bb->OldB2 = bb->Width;
		bb->OldH2 = bb->Height;
		bb->Sizing = false;
		FPoint b, b1, t, t1, h, h1, g, tes, tes2;
		double oldRot, oldLocalX, oldLocalY;
		oldRot = bb->Rot;
		oldLocalX = bb->LocalX;
		oldLocalY = bb->LocalY;
		FPointArray gr;
		gr.addPoint(bb->GrStartX, bb->GrStartY);
		gr.addPoint(bb->GrEndX, bb->GrEndY);
		g = FPoint(gx, gy);
		b = transformPoint(FPoint(0, 0), bb->Xpos, bb->Ypos, bb->Rot, 1, 1);
		b -= g;
		b1 = transformPoint(b, 0, 0, 0, scx, scy);
		t = transformPoint(FPoint(bb->Width, 0), bb->Xpos, bb->Ypos, bb->Rot, 1, 1);
		t -= g;
		t1 = transformPoint(t, 0, 0, 0, scx, scy);
		h = transformPoint(FPoint(0, bb->Height), bb->Xpos, bb->Ypos, bb->Rot, 1, 1);
		h -= g;
		h1 = transformPoint(h, 0, 0, 0, scx, scy);
		bb->Pwidth = QMAX(bb->Pwidth*((scx+scy)/2), 0.01);
		if (bb->itemType() == PageItem::Line)
		{
			bb->Rot = atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI);
			bb->Width = sqrt(pow(t1.x()-b1.x(),2)+pow(t1.y()-b1.y(),2));
			bb->Xpos = b1.x()+gx;
			bb->Ypos = b1.y()+gy;
		}
		else
		{
			FPoint oldPos = FPoint(bb->Xpos, bb->Ypos);
			QWMatrix ma;
			ma.rotate(bb->Rot);
			bb->PoLine.map(ma);
			QWMatrix ma2;
			ma2.translate(gx-bb->Xpos, gy-bb->Ypos);
			ma2.scale(scx, scy);
			bb->PoLine.map(ma2);
			bb->Rot = 0;
			bb->ClipEdited = true;
			AdjustItemSize(bb);
			QWMatrix ma3;
			ma3.translate(gx, gy);
			ma3.scale(scx, scy);
			n = FPoint(gx-oldPos.x(), gy-oldPos.y());
			x = ma3.m11() * n.x() + ma3.m21() * n.y() + ma3.dx();
			y = ma3.m22() * n.y() + ma3.m12() * n.x() + ma3.dy();
			MoveItem(gx-x, gy-y, bb, true);
			if (oldRot != 0)
			{
				bb->Rot = atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI);
				QWMatrix ma;
				ma.rotate(-bb->Rot);
				bb->PoLine.map(ma);
				AdjustItemSize(bb);
			}
		}
		bb->ISize = QMAX(qRound(bb->ISize*((scx+scy)/2)), 1);
		if ((bb->itemText.count() != 0) && (!bb->isTableItem))
		{
			bb->LineSp = ((bb->ISize / 10.0) * static_cast<double>(Doc->typographicSetttings.autoLineSpacing) / 100) + (bb->ISize / 10.0);
			for (aa = 0; aa < bb->itemText.count(); ++aa)
				bb->itemText.at(aa)->csize = QMAX(qRound(bb->itemText.at(aa)->csize*((scx+scy)/2)), 1);
			if (bb->itemType() == PageItem::PathText)
				UpdatePolyClip(bb);
		}
		bb->LocalX = oldLocalX;
		bb->LocalY = oldLocalY;
		bb->OldB2 = bb->Width;
		bb->OldH2 = bb->Height;
		QWMatrix ma4;
		ma4.rotate(oldRot);
		ma4.scale(scx, scy);
		gr.map(ma4);
		bb->ContourLine.map(ma4);
		bb->GrStartX = gr.point(0).x();
		bb->GrStartY = gr.point(0).y();
		bb->GrEndX = gr.point(1).x();
		bb->GrEndY = gr.point(1).y();
		updateGradientVectors(bb);
	}
	bb = SelItem.at(0);
	GroupOnPage(bb);
	setGroupRect();
	setUpdatesEnabled(true);
	getGroupRect(&gx, &gy, &gw, &gh);
	updateContents(QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10)).unite(oldR));
	emit DocChanged();
}

void ScribusView::RotateItem(double win, int ite)
{
	RotateItem(win, Doc->Items.at(ite));
}

void ScribusView::RotateItem(double win, PageItem *pi)
{
	PageItem *currItem;
	currItem = pi;
	if (currItem->locked())
		return;
	FPoint n;
	QRect oldR = getRedrawBounding(currItem);
	if ((Doc->RotMode != 0) && (currItem->itemType() != PageItem::Line))
	{
		QWMatrix ma;
		ma.translate(currItem->Xpos, currItem->Ypos);
		ma.scale(1, 1);
		ma.rotate(currItem->Rot);
		double ro = win-currItem->Rot;
		currItem->Rot = win;
		switch (Doc->RotMode)
		{
		case 2:
			ma.translate(currItem->Width/2, currItem->Height/2);
			n = FPoint(-currItem->Width/2, -currItem->Height/2);
			break;
		case 4:
			ma.translate(currItem->Width, currItem->Height);
			n = FPoint(-currItem->Width, -currItem->Height);
			break;
		case 3:
			ma.translate(0, currItem->Height);
			n = FPoint(0, -currItem->Height);
			break;
		case 1:
			ma.translate(currItem->Width, 0);
			n = FPoint(-currItem->Width, 0);
			break;
		}
		ma.rotate(ro);
		double x = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		double y = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		bool oldS = currItem->Sizing;
		currItem->Sizing = true;
		MoveItem(x-currItem->Xpos, y-currItem->Ypos, currItem);
		currItem->Sizing = oldS;
	}
	else
	{
		currItem->Rot = win;
		setRedrawBounding(currItem);
	}
	QRect newR = getRedrawBounding(currItem);
	updateContents(newR.unite(oldR));
	emit SetAngle(currItem->Rot);
}

void ScribusView::AdjustItemSize(PageItem *currItem)
{
	bool siz = currItem->Sizing;
	currItem->Sizing = false;
	FPointArray Clip;
	Clip = currItem->PoLine;
	FPoint tp2 = getMinClipF(&Clip);
	SizeItem(currItem->Width - tp2.x(), currItem->Height - tp2.y(), currItem->ItemNr, true, false, false);
	Clip.translate(-tp2.x(), -tp2.y());
	if (currItem->Rot != 0)
	{
		FPoint npv = FPoint(tp2.x(), tp2.y());
		MoveRotated(currItem, npv, true);
	}
	else
		MoveItem(tp2.x(), tp2.y(), currItem, true);
	if (!currItem->imageFlippedH())
		MoveItemI(-tp2.x()/currItem->LocalScX, 0, currItem->ItemNr, false);
	if (!currItem->imageFlippedV())
		MoveItemI(0, -tp2.y()/currItem->LocalScY, currItem->ItemNr, false);
	FPoint tp = getMaxClipF(&Clip);
	if (currItem->imageFlippedH())
		MoveItemI((currItem->Width - tp.x())/currItem->LocalScX, 0, currItem->ItemNr, false);
	if (currItem->imageFlippedV())
		MoveItemI(0, (currItem->Height - tp.y())/currItem->LocalScY, currItem->ItemNr, false);
	SizeItem(tp.x(), tp.y(), currItem->ItemNr, true, false);
	currItem->ClipEdited = true;
	currItem->PoLine = Clip.copy();
	if (currItem->itemType() == PageItem::PolyLine)
		SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->Sizing = siz;
}

void ScribusView::AdvanceSel(PageItem *currItem, int oldPos, int len, int dir, int expandSel, int state)
{
	int i;
	int isSpace;
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

// jjsa added on 15-mar-2004
// calculate the end position while ctrl + arrow pressed

void ScribusView::setNewPos(PageItem *currItem, int oldPos, int len, int dir)
{
	int i;
	int isSpace;
	if ( dir > 0 && oldPos < len )
	{
		isSpace = currItem->itemText.at(oldPos)->ch.at(0).isSpace();
		currItem->CPos = oldPos +1;
		for (i=oldPos+1; i < len; i++)
		{
			if ( currItem->itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			currItem->CPos++;
		}
	}
	else if ( dir < 0 && oldPos > 0 )
	{
		oldPos--;
		isSpace = currItem->itemText.at(oldPos)->ch.at(0).isSpace();
		for (i=oldPos; i >= 0; i--)
		{
			if (  currItem->itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			currItem->CPos--;
		}
	}
}

// jjsa added on 15-mar-2004 expand / decrease selection

// jjsa added on 14-mar-2004 text selection with pressed
// shift button and <-, -> cursor keys
// Parameters
//   PageItem *currItem text item to be processed
//   inc < 0 for left key > 0 for right key
//  if value is +/-1 work on slection
//  if value is +/-2 refresh if text under cursor is selected

void ScribusView::ExpandSel(PageItem *currItem, int dir, int oldPos)
{
	int len = currItem->itemText.count();
	int rightSel = false; // assume left right and actual char not selected
	int leftSel  = false;
	int actSel   = false;
	int selMode  = false;

	if ( dir == -1 || dir == 1 )
		selMode = true;
	else
	{
		if ( dir > 0 )
			dir = 1;
		else
			dir = -1;
	}
   // show for selection of previous, actual and next character
	if ( currItem->HasSel ) /* selection allready present */
	{
		if (dir > 0 && oldPos < len) // -> key
		{
			if ( oldPos == 0 )
				leftSel = false;
			else
				leftSel = currItem->itemText.at(oldPos-1)->cselect;
			actSel =   currItem->itemText.at(oldPos)->cselect;
			rightSel = false; // not relevant
		}
		else if ( dir > 0 && oldPos == len) // -> key
		{
			return;
		}
		else if ( dir < 0 && oldPos > 0 ) // <- key
		{
			actSel  =   currItem->itemText.at(oldPos-1)->cselect;
			leftSel = false; // not relevant
			if ( oldPos < len  )
				rightSel = currItem->itemText.at(oldPos)->cselect;
			else
				rightSel = false;
		}
		else if ( dir < 0 && oldPos == 0 ) // <- key
		{
         return;
		}
		if ( selMode && ! (leftSel||actSel||rightSel) )
		{
         // selected outside from concerned range
         // deselect all
			int i;
			for (i=0; i < len; i++ )
			{
				currItem->itemText.at(i)->cselect = false;
			}
			currItem->HasSel = false;
			emit HasNoTextSel();
		}
		else if ( !selMode )
		{
			if (leftSel||actSel||rightSel)
				RefreshItem(currItem);
		}
	}
	if ( !selMode )
		return;
   // no selection
	if ( !currItem->HasSel )
	{
		currItem->HasSel = true;
		emit HasTextSel();
		leftSel = true;
		rightSel = true;
	}
	int i;
	int start;
	int end;
	int sel;
	if (dir == 1) // ->  key
	{
		start = oldPos;
		end   = currItem->CPos;
		sel = leftSel == true;
	}
	else
	{
		start = currItem->CPos;
		end   = oldPos;
		sel = rightSel == true;
	}
	for ( i = start; i < end; i++)
		currItem->itemText.at(i)->cselect = sel;
	if ( ! sel )
		emit  HasNoTextSel();
	RefreshItem(currItem);
}

void ScribusView::deselectAll(PageItem *currItem)
{
	PageItem *i = currItem;
	int       n;
	int       l;
	while( i->BackBox )
		i=i->BackBox;
	while ( i )
	{
		if ( i->HasSel )
		{
			l = i->itemText.count();
			for (n=0; n < l; n++ )
				i->itemText.at(n)->cselect = false;
			RefreshItem(i);
			i->HasSel = false;
		}
		i = i->NextBox;
	}
	emit  HasNoTextSel();
}

bool ScribusView::slotSetCurs(int x, int y)
{
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		if (!((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)))
			return false;
		QRect mpo;
		int xP, yP;
		xP = qRound(x / Scale);
		yP = qRound(y / Scale);
		QPainter p;
		QString chx;
		p.begin(this);
		Transform(currItem, &p);
		mpo = QRect(x - Doc->guidesSettings.grabRad, y - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
		if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->Width), static_cast<int>(currItem->Height))))).contains(mpo)) ||
		        (QRegion(p.xForm(currItem->Clip)).contains(mpo)))
		{
			if (currItem->itemType() == PageItem::ImageFrame)
				return true;
			uint a, i;
			int xp, yp, w, h, chs;
			CursVis = true;
			for (a=0; a<currItem->itemText.count(); ++a)
			{
				xp = static_cast<int>(currItem->itemText.at(a)->xp);
				yp = static_cast<int>(currItem->itemText.at(a)->yp);
				chx = currItem->itemText.at(a)->ch;
				if (chx == QChar(30))
					chx = currItem->ExpandToken(a);
				if (chx == QChar(29))
					chx = " ";
				chs = currItem->itemText.at(a)->csize;
				currItem->SetZeichAttr(currItem->itemText.at(a), &chs, &chx);
				if ((chx == QChar(13)) || (chx == QChar(9)))
					w = 1;
				else
					w = qRound(Cwidth(Doc, currItem->itemText.at(a)->cfont, chx, chs)*(currItem->itemText.at(a)->cscale / 100.0));
				h = static_cast<int>(Doc->docParagraphStyles[currItem->itemText.at(a)->cab].LineSpa);
				if (QRegion(p.xForm(QRect(xp-1, yp-h, w+1, h))).contains(QPoint(x, y)))
				{
					currItem->CPos = a;
					p.end();
					Doc->CurrFont = currItem->itemText.at(a)->cfont->SCName;
					Doc->CurrFontSize = currItem->itemText.at(a)->csize;
					Doc->CurrTextFill = currItem->itemText.at(a)->ccolor;
					Doc->CurrTextFillSh = currItem->itemText.at(a)->cshade;
					Doc->CurrTextStroke = currItem->itemText.at(a)->cstroke;
					Doc->CurrTextStrokeSh = currItem->itemText.at(a)->cshade2;
					Doc->CurrTextScale = currItem->itemText.at(a)->cscale;
					emit ItemTextSca(currItem->itemText.at(a)->cscale);
					emit ItemTextFont(currItem->itemText.at(a)->cfont->SCName);
					emit ItemTextSize(currItem->itemText.at(a)->csize);
					emit ItemTextUSval(currItem->itemText.at(a)->cextra);
					emit ItemTextStil(currItem->itemText.at(a)->cstyle);
					emit ItemTextAbs(currItem->itemText.at(a)->cab);
					emit ItemTextFarben(currItem->itemText.at(a)->cstroke, currItem->itemText.at(a)->ccolor, currItem->itemText.at(a)->cshade2, currItem->itemText.at(a)->cshade);
					return true;
				}
			}
			QPoint np;
			for (a=0; a<currItem->itemText.count(); ++a)
			{
				xp = static_cast<int>(currItem->itemText.at(a)->xp);
				yp = static_cast<int>(currItem->itemText.at(a)->yp);
				np = p.xForm(QPoint(xp, yp - static_cast<int>(Doc->docParagraphStyles[currItem->itemText.at(a)->cab].LineSpa)));
				if (yP < np.y())
				{
					currItem->CPos = a;
					p.end();
					if (a > 0)
						i = a - 1;
					else
						i = a;
					Doc->CurrFont = currItem->itemText.at(i)->cfont->SCName;
					Doc->CurrFontSize = currItem->itemText.at(i)->csize;
					Doc->CurrTextFill = currItem->itemText.at(i)->ccolor;
					Doc->CurrTextFillSh = currItem->itemText.at(i)->cshade;
					Doc->CurrTextStroke = currItem->itemText.at(i)->cstroke;
					Doc->CurrTextStrokeSh = currItem->itemText.at(i)->cshade2;
					Doc->CurrTextScale = currItem->itemText.at(i)->cscale;
					emit ItemTextSca(currItem->itemText.at(i)->cscale);
					emit ItemTextFarben(currItem->itemText.at(i)->cstroke, currItem->itemText.at(i)->ccolor, currItem->itemText.at(i)->cshade2, currItem->itemText.at(i)->cshade);
					emit ItemTextFont(currItem->itemText.at(i)->cfont->SCName);
					emit ItemTextSize(currItem->itemText.at(i)->csize);
					emit ItemTextUSval(currItem->itemText.at(i)->cextra);
					emit ItemTextStil(currItem->itemText.at(i)->cstyle);
					emit ItemTextAbs(currItem->itemText.at(i)->cab);
					return true;
				}
			}
			currItem->CPos = currItem->itemText.count();
			if (currItem->itemText.count() != 0)
			{
				Doc->CurrFont = currItem->itemText.at(currItem->CPos-1)->cfont->SCName;
				Doc->CurrFontSize = currItem->itemText.at(currItem->CPos-1)->csize;
				Doc->CurrTextFill = currItem->itemText.at(currItem->CPos-1)->ccolor;
				Doc->CurrTextFillSh = currItem->itemText.at(currItem->CPos-1)->cshade;
				Doc->CurrTextStroke = currItem->itemText.at(currItem->CPos-1)->cstroke;
				Doc->CurrTextStrokeSh = currItem->itemText.at(currItem->CPos-1)->cshade2;
				Doc->CurrTextScale = currItem->itemText.at(currItem->CPos-1)->cscale;
				emit ItemTextSca(currItem->itemText.at(currItem->CPos-1)->cscale);
				emit ItemTextFarben(currItem->itemText.at(currItem->CPos-1)->cstroke, currItem->itemText.at(currItem->CPos-1)->ccolor, currItem->itemText.at(currItem->CPos-1)->cshade2, currItem->itemText.at(currItem->CPos-1)->cshade);
				emit ItemTextFont(currItem->itemText.at(currItem->CPos-1)->cfont->SCName);
				emit ItemTextSize(currItem->itemText.at(currItem->CPos-1)->csize);
				emit ItemTextUSval(currItem->itemText.at(currItem->CPos-1)->cextra);
				emit ItemTextStil(currItem->itemText.at(currItem->CPos-1)->cstyle);
				emit ItemTextAbs(currItem->itemText.at(currItem->CPos-1)->cab);
				p.end();
				return true;
			}
			else
			{
				Doc->CurrFont = currItem->IFont;
				Doc->CurrFontSize = currItem->ISize;
				Doc->CurrTextFill = currItem->TxtFill;
				Doc->CurrTextFillSh = currItem->ShTxtFill;
				Doc->CurrTextStroke = currItem->TxtStroke;
				Doc->CurrTextStrokeSh = currItem->ShTxtStroke;
				Doc->CurrTextScale = currItem->TxtScale;
				emit ItemTextSca(currItem->TxtScale);
				emit ItemTextFarben(currItem->TxtStroke, currItem->TxtFill, currItem->ShTxtStroke, currItem->ShTxtFill);
				emit ItemTextFont(currItem->IFont);
				emit ItemTextSize(currItem->ISize);
				emit ItemTextUSval(currItem->ExtraV);
				emit ItemTextStil(currItem->TxTStyle);
				emit ItemTextAbs(currItem->textAlignment);
				p.end();
				return true;
			}
			p.end();
		}
	}
	return false;
}

void ScribusView::slotDoCurs(bool draw)
{
	int chs, offs;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		if (currItem->itemType() != PageItem::TextFrame)
			return;
		QPainter p;
		QString chx;
		int xp, yp, yp1, desc, asce;
		p.begin(viewport());
		ToView(&p);
		Transform(currItem, &p);
		TransformM(currItem, &p);
		if (currItem->CPos > 0)
		{
			offs = QMIN(currItem->CPos-1, static_cast<int>(currItem->itemText.count()-1));
			if (currItem->CPos < static_cast<int>(currItem->itemText.count()-1))
			{
				if (currItem->itemText.at(offs+1)->cstyle & 256)
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
			chx = currItem->itemText.at(offs)->ch;
			if (chx == QChar(30))
				chx = currItem->ExpandToken(offs);
			if (chx == QChar(29))
				chx = " ";
			chs = currItem->itemText.at(offs)->csize;
			currItem->SetZeichAttr(currItem->itemText.at(offs), &chs, &chx);
			if (currItem->CPos != static_cast<int>(currItem->itemText.count()))
			{
				if (currItem->itemText.at(currItem->CPos)->ch == QChar(9))
				{
					xp = static_cast<int>(currItem->itemText.at(currItem->CPos-1)->xp);
					chs = currItem->itemText.at(currItem->CPos-1)->csize;
					chx = currItem->itemText.at(currItem->CPos-1)->ch;
					xp += qRound(Cwidth(Doc, currItem->itemText.at(currItem->CPos-1)->cfont, chx, chs)*(currItem->itemText.at(currItem->CPos-1)->cscale / 100.0));
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
					xp += qRound(Cwidth(Doc, currItem->itemText.at(offs)->cfont, chx, chs)*(currItem->itemText.at(offs)->cscale / 100.0));
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
						xp += qRound(Cwidth(Doc, currItem->itemText.at(offs)->cfont, chx, chs)*(currItem->itemText.at(offs)->cscale / 100.0));
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
				if (currItem->lineColor() != "None")
					lineCorr = currItem->Pwidth / 2.0;
				else
					lineCorr = 0;
				xp = static_cast<int>(currItem->Extra + lineCorr);
				yp = static_cast<int>(currItem->TExtra + lineCorr + currItem->LineSp);
				desc = static_cast<int>((*Doc->AllFonts)[currItem->IFont]->numDescender * (-currItem->ISize / 10.0));
				asce = static_cast<int>((*Doc->AllFonts)[currItem->IFont]->numAscent * (currItem->ISize / 10.0));
			}
			else
			{
				if ((currItem->itemText.at(currItem->CPos)->ch == QChar(9)) || (currItem->itemText.at(currItem->CPos)->ch == QChar(13)) || (currItem->itemText.at(currItem->CPos)->ch == QChar(28)))
				{
					double lineCorr;
					if (currItem->lineColor() != "None")
						lineCorr = currItem->Pwidth / 2.0;
					else
						lineCorr = 0;
					xp = static_cast<int>(currItem->Extra + lineCorr);
					yp = static_cast<int>(currItem->TExtra + lineCorr + currItem->LineSp);
					desc = static_cast<int>((*Doc->AllFonts)[currItem->IFont]->numDescender * (-currItem->ISize / 10.0));
					asce = static_cast<int>((*Doc->AllFonts)[currItem->IFont]->numAscent * (currItem->ISize / 10.0));
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
			p.drawLine(xp, QMIN(QMAX(yp1,0),static_cast<int>(currItem->Height)), xp, QMIN(QMAX(yp,0),static_cast<int>(currItem->Height)));
			CursVis = !CursVis;
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
			if (CursVis)
				p.drawLine(xp, QMIN(QMAX(yp1,0),static_cast<int>(currItem->Height)), xp, QMIN(QMAX(yp,0),static_cast<int>(currItem->Height)));
			CursVis = false;
		}
		p.end();
	}
}

void ScribusView::BlinkCurs()
{
	slotDoCurs(true);
}

void ScribusView::HandleCurs(QPainter *p, PageItem *currItem, QRect mpo)
{
	QPoint tx, tx2;
	QWMatrix ma = p->worldMatrix();
	ma.setTransformationMode ( QWMatrix::Areas );
	p->setWorldMatrix(ma);
	tx = p->xForm(QPoint(static_cast<int>(currItem->Width), 0));
	tx2 = p->xForm(QPoint(0, static_cast<int>(currItem->Height)));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == Rotation)
			qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
		else
		{
			double rr = fabs(currItem->Rot);
			if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >=315.0) && (rr <= 360.0)))
				qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
			if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
				qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
		}
	}
	tx = p->xForm(QPoint(static_cast<int>(currItem->Width), static_cast<int>(currItem->Height)/2));
	tx2 = p->xForm(QPoint(0, static_cast<int>(currItem->Height)/2));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->Rot);
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
	}
	tx = p->xForm(QPoint(static_cast<int>(currItem->Width)/2, 0));
	tx2 = p->xForm(QPoint(static_cast<int>(currItem->Width)/2, static_cast<int>(currItem->Height)));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->Rot);
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
	}
	tx = p->xForm(QPoint(static_cast<int>(currItem->Width), static_cast<int>(currItem->Height)));
	tx2 = p->xForm(QPoint(0, 0));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == Rotation)
			qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
		else
		{
			double rr = fabs(currItem->Rot);
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

int ScribusView::OnPage(double x2, double  y2)
{
	int retw = -1;
	if (Doc->MasterP)
	{
		int x = static_cast<int>(Doc->currentPage->Xoffset);
		int y = static_cast<int>(Doc->currentPage->Yoffset);
		int w = static_cast<int>(Doc->currentPage->Width);
		int h = static_cast<int>(Doc->currentPage->Height);
		if (QRect(x, y, w, h).contains(QPoint(qRound(x2), qRound(y2))))
			retw = Doc->currentPage->PageNr;
	}
	else
	{
		for (int a = 0; a < static_cast<int>(Doc->Pages.count()); ++a)
		{
			int x = static_cast<int>(Doc->Pages.at(a)->Xoffset);
			int y = static_cast<int>(Doc->Pages.at(a)->Yoffset);
			int w = static_cast<int>(Doc->Pages.at(a)->Width);
			int h = static_cast<int>(Doc->Pages.at(a)->Height);
			if (QRect(x, y, w, h).contains(QPoint(qRound(x2), qRound(y2))))
			{
				retw = a;
				break;
			}
		}
	}
	return retw;
}

int ScribusView::OnPage(PageItem *currItem)
{
	int retw = -1;
	if (Doc->MasterP)
	{
		int x = static_cast<int>(Doc->currentPage->Xoffset);
		int y = static_cast<int>(Doc->currentPage->Yoffset);
		int w = static_cast<int>(Doc->currentPage->Width);
		int h = static_cast<int>(Doc->currentPage->Height);
		int x2 = static_cast<int>(currItem->Xpos);
		int y2 = static_cast<int>(currItem->Ypos);
		int w2 = static_cast<int>(currItem->Width);
		int h2 = static_cast<int>(currItem->Height);
		if (QRect(x, y, w, h).intersects(QRect(x2, y2, w2, h2)))
			retw = Doc->currentPage->PageNr;
	}
	else
	{
		for (int a = 0; a < static_cast<int>(Doc->Pages.count()); ++a)
		{
			int x = static_cast<int>(Doc->Pages.at(a)->Xoffset);
			int y = static_cast<int>(Doc->Pages.at(a)->Yoffset);
			int w = static_cast<int>(Doc->Pages.at(a)->Width);
			int h = static_cast<int>(Doc->Pages.at(a)->Height);
			int x2 = static_cast<int>(currItem->Xpos);
			int y2 = static_cast<int>(currItem->Ypos);
			int w2 = static_cast<int>(currItem->Width);
			int h2 = static_cast<int>(currItem->Height);
			if (QRect(x, y, w, h).intersects(QRect(x2, y2, w2, h2)))
			{
				retw = a;
				break;
			}
		}
	}
	if ((retw == -1) && (currItem->isBookmark))
	{
		emit DelBM(currItem);
		currItem->isBookmark = false;
	}
	return retw;
}

void ScribusView::GroupOnPage(PageItem* currItem)
{
	QPtrList<PageItem> Objects;
	PageItem* item;
	if (currItem->Groups.count() == 0)
		return;
	int ObjGroup = currItem->Groups.top();
	for (uint a = 0; a < Doc->Items.count(); ++a)
	{
		item = Doc->Items.at(a);
		if (item->Groups.top() == ObjGroup)
			Objects.append(item);
	}
	int Off_Page = -1;
	int On_Page = 0;
	for (uint a = 0; a < Objects.count(); ++a)
	{
		int res = OnPage(Objects.at(a));
		Off_Page = QMAX(Off_Page, res);
		if (res != -1)
			On_Page = QMIN(On_Page, res);
	}
	int final = -1;
	if (Off_Page != -1)
		final = On_Page;
	for (uint a = 0; a < Objects.count(); ++a)
	{
		Objects.at(a)->OwnPage = final;
	}
}

void ScribusView::SelectItemNr(int nr, bool draw, bool single)
{
	SelectItem(Doc->Items.at(nr), draw, single);
}

void ScribusView::SelectItem(PageItem *pi, bool draw, bool single)
{
	PageItem *currItem = pi;
	if (!currItem->Select)
	{
		if (single)
		{
			SelItem.append(currItem);
			currItem->Select = true;
			currItem->isSingleSel = true;
			currItem->FrameOnly = true;
			currItem->paintObj();
		}
		else
		{
			if (currItem->Groups.count() != 0)
			{
				if (SelItem.count() != 0)
				{
					if (SelItem.find(currItem) == -1)
						SelItem.append(currItem);
				}
				else
					SelItem.append(currItem);
				for (uint ga=0; ga<Doc->Items.count(); ++ga)
				{
					if (Doc->Items.at(ga)->Groups.count() != 0)
					{
						if (Doc->Items.at(ga)->Groups.top() == currItem->Groups.top())
						{
							if (Doc->Items.at(ga)->ItemNr != currItem->ItemNr)
							{
								if (SelItem.find(Doc->Items.at(ga)) == -1)
									SelItem.append(Doc->Items.at(ga));
							}
							Doc->Items.at(ga)->Select = true;
							Doc->Items.at(ga)->FrameOnly = true;
							Doc->Items.at(ga)->paintObj();
						}
					}
				}
			}
			else
			{
				SelItem.append(currItem);
				currItem->Select = true;
				currItem->FrameOnly = true;
				currItem->paintObj();
			}
		}
	}
	else
	{
		if (SelItem.count() > 1)
		{
			PageItem *bb = SelItem.at(0);
			SelItem.remove(currItem);
			SelItem.prepend(currItem);
			currItem->FrameOnly = true;
			currItem->paintObj();
			bb->FrameOnly = true;
			bb->paintObj();
		}
	}
	if (draw)
	{
		if (SelItem.count() > 1)
		{
			setGroupRect();
			paintGroupRect();
			double x, y, w, h;
			getGroupRect(&x, &y, &w, &h);
			emit ItemPos(x, y);
			emit ItemGeom(w, h);
		}
		else
			EmitValues(currItem);
		emit HaveSel(currItem->itemType());
	}
}

void ScribusView::selectionChanged()
{
	if (ScApp->isObjectSpecificUndo())
	{
		if (SelItem.count() == 1)
			undoManager->showObject(SelItem.at(0)->getUId());
		else if (SelItem.count() == 0)
			undoManager->showObject(Doc->currentPage->getUId());
		else
			undoManager->showObject(Um::NO_UNDO_STACK);
	}
}

void ScribusView::selectPage(QMouseEvent *m)
{
	QRect mpo;
	Mpressed = true;
	Mxp = static_cast<int>(m->x()/Scale);
	Myp = static_cast<int>(m->y()/Scale);
	mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	ClRe = -1;
	Deselect(false);
	if (!Doc->MasterP)
	{
		for (uint a = 0; a < Doc->Pages.count(); ++a)
		{
			int x = static_cast<int>(Doc->Pages.at(a)->Xoffset * Scale);
			int y = static_cast<int>(Doc->Pages.at(a)->Yoffset * Scale);
			int w = static_cast<int>(Doc->Pages.at(a)->Width * Scale);
			int h = static_cast<int>(Doc->Pages.at(a)->Height * Scale);
			if (QRect(x, y, w, h).intersects(mpo))
			{
				if (Doc->currentPage->PageNr != a)
				{
					Doc->currentPage = Doc->Pages.at(a);
					setMenTxt(a);
					DrawNew();
				}
				break;
			}
		}
	}
}

bool ScribusView::SeleItem(QMouseEvent *m)
{
	uint a;
	QPainter p;
	QRect tx, mpo;
	PageItem *currItem;
	Mpressed = true;
	Mxp = static_cast<int>(m->x()/Scale);
	Myp = static_cast<int>(m->y()/Scale);
	mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	ClRe = -1;
	if ((SelItem.count() != 0) && (m->state() == ControlButton))
		currItem = SelItem.at(0);
	else
		currItem = Doc->Items.last();
	if (!Doc->MasterP)
	{
		for (a = 0; a < Doc->Pages.count(); ++a)
		{
			int x = static_cast<int>(Doc->Pages.at(a)->Xoffset * Scale);
			int y = static_cast<int>(Doc->Pages.at(a)->Yoffset * Scale);
			int w = static_cast<int>(Doc->Pages.at(a)->Width * Scale);
			int h = static_cast<int>(Doc->Pages.at(a)->Height * Scale);
			if (QRect(x, y, w, h).intersects(mpo))
			{
				if (Doc->currentPage->PageNr != a)
				{
					Doc->currentPage = Doc->Pages.at(a);
					setMenTxt(a);
					DrawNew();
				}
				break;
			}
		}
	}
	if (m->state() == (ControlButton | AltButton))
		Deselect(false);

	if ((m->state() == (ShiftButton | AltButton)) && (!Doc->MasterP) && (Doc->currentPage->FromMaster.count() != 0))
	{
		Page* Mp = Doc->MasterPages.at(Doc->MasterNames[Doc->currentPage->MPageNam]);
		currItem = Doc->currentPage->FromMaster.last();
		for (a = 0; a < Doc->currentPage->FromMaster.count(); ++a)
		{
			if (currItem->LayerNr == Doc->ActiveLayer)
			{
				p.begin(this);
				double OldX = currItem->Xpos;
				double OldY = currItem->Ypos;
				if (!currItem->ChangedMasterItem)
				{
					currItem->Xpos = OldX - Mp->Xoffset + Doc->currentPage->Xoffset;
					currItem->Ypos = OldY - Mp->Yoffset + Doc->currentPage->Yoffset;
				}
				Transform(currItem, &p);
				if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->Width), static_cast<int>(currItem->Height))))).contains(mpo)) ||
						(QRegion(p.xForm(currItem->Clip)).contains(mpo)))
				{
					if (!currItem->Select)
					{
						if ((m->state() != ShiftButton) || (Doc->appMode == LinkFrames) || (Doc->appMode == UnlinkFrames))
							Deselect(false);
						if (currItem->Groups.count() != 0)
						{
							if (SelItem.count() != 0)
							{
								if (SelItem.find(currItem) == -1)
									SelItem.append(currItem);
							}
							else
								SelItem.append(currItem);
							if (m->state() != (ControlButton | AltButton))
							{
								for (uint ga=0; ga<Doc->Items.count(); ++ga)
								{
									if (Doc->Items.at(ga)->Groups.count() != 0)
									{
										if (Doc->Items.at(ga)->Groups.top() == currItem->Groups.top())
										{
											if (Doc->Items.at(ga)->ItemNr != currItem->ItemNr)
											{
												if (SelItem.find(Doc->Items.at(ga)) == -1)
													SelItem.append(Doc->Items.at(ga));
											}
											Doc->Items.at(ga)->isSingleSel = false;
											Doc->Items.at(ga)->Select = true;
											Doc->Items.at(ga)->FrameOnly = true;
										}
									}
								}
							}
							else
							{
								currItem->isSingleSel = true;
								currItem->Select = true;
								currItem->FrameOnly = true;
								currItem->paintObj();
							}
						}
						else
						{
							SelItem.append(currItem);
							currItem->Select = true;
							currItem->FrameOnly = true;
							currItem->paintObj();
						}
					}
					else
					{
						if (SelItem.count() > 1)
						{
							PageItem *bb = SelItem.at(0);
							SelItem.remove(currItem);
							SelItem.prepend(currItem);
							currItem->FrameOnly = true;
							currItem->paintObj();
							bb->FrameOnly = true;
							bb->paintObj();
						}
					}
					if (SelItem.count() > 1)
					{
						for (uint aa = 0; aa < SelItem.count(); ++aa)
						{
							PageItem *bb = SelItem.at(aa);
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
						EmitValues(currItem);
						if (currItem->itemType() == PageItem::Line)
							emit ItemGeom(currItem->Width, currItem->Height);
						emit HaveSel(currItem->itemType());
					}
					p.end();
					if (!currItem->ChangedMasterItem)
					{
						currItem->Xpos = OldX;
						currItem->Ypos = OldY;
					}
					return true;
				}
				p.end();
				if (!currItem->ChangedMasterItem)
				{
					currItem->Xpos = OldX;
					currItem->Ypos = OldY;
				}
			}
			currItem = Doc->currentPage->FromMaster.prev();
		}
	}
	if ((m->state() == (ControlButton | ShiftButton)) && (SelItem.count() != 0))
	{
		for (a = 0; a < Doc->Items.count(); ++a)
		{
			if (currItem->Select)
			{
				if (currItem->ItemNr == 0)
				{
					currItem = Doc->Items.last();
					break;
				}
				currItem = Doc->Items.prev();
				break;
			}
			currItem = Doc->Items.prev();
		}
	}
	for (a = 0; a < Doc->Items.count(); ++a)
	{
		if (currItem == NULL)
		{
			Deselect(true);
			SelItem.clear();
			return false;
		}
		if ((Doc->MasterP)  && (!((currItem->OwnPage == -1) || (currItem->OwnPage == static_cast<int>(Doc->currentPage->PageNr)))))
		{
			currItem = Doc->Items.prev();
			continue;
		}
		if (currItem->LayerNr == Doc->ActiveLayer)
		{
			p.begin(this);
			Transform(currItem, &p);
			if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(currItem->Width), static_cast<int>(currItem->Height))))).contains(mpo)) ||
			        (QRegion(p.xForm(currItem->Clip)).contains(mpo)))
			{
				if (!currItem->Select)
				{
					if ((m->state() != ShiftButton) || (Doc->appMode == LinkFrames) || (Doc->appMode == UnlinkFrames))
						Deselect(false);
					if (currItem->Groups.count() != 0)
					{
						if (SelItem.count() != 0)
						{
							if (SelItem.find(currItem) == -1)
								SelItem.append(currItem);
						}
						else
							SelItem.append(currItem);
						if (m->state() != (ControlButton | AltButton))
						{
							for (uint ga=0; ga<Doc->Items.count(); ++ga)
							{
								if (Doc->Items.at(ga)->Groups.count() != 0)
								{
									if (Doc->Items.at(ga)->Groups.top() == currItem->Groups.top())
									{
										if (Doc->Items.at(ga)->ItemNr != currItem->ItemNr)
										{
											if (SelItem.find(Doc->Items.at(ga)) == -1)
												SelItem.append(Doc->Items.at(ga));
										}
										Doc->Items.at(ga)->isSingleSel = false;
										Doc->Items.at(ga)->Select = true;
										Doc->Items.at(ga)->FrameOnly = true;
									}
								}
							}
						}
						else
						{
							currItem->isSingleSel = true;
							currItem->Select = true;
							currItem->FrameOnly = true;
							currItem->paintObj();
						}
					}
					else
					{
						SelItem.append(currItem);
						currItem->Select = true;
						currItem->FrameOnly = true;
						currItem->paintObj();
					}
				}
				else
				{
					if (SelItem.count() > 1)
					{
						PageItem *bb = SelItem.at(0);
						SelItem.remove(currItem);
						SelItem.prepend(currItem);
						currItem->FrameOnly = true;
						currItem->paintObj();
						bb->FrameOnly = true;
						bb->paintObj();
					}
				}
				if (SelItem.count() > 1)
				{
					for (uint aa = 0; aa < SelItem.count(); ++aa)
					{
						PageItem *bb = SelItem.at(aa);
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
					EmitValues(currItem);
					currItem->paintObj();
					if (currItem->itemType() == PageItem::Line)
						emit ItemGeom(currItem->Width, currItem->Height);
					emit HaveSel(currItem->itemType());
				}
				if (SelItem.count() == 1)
				{
					HandleSizer(&p, currItem, mpo, m);
					if (HowTo == 0)
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
				}
				else
				{
					qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
					mCG = false;
				}
				p.end();
				return true;
			}
			p.end();
		}
		currItem = Doc->Items.prev();
	}
	if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == NormalMode) && (!Doc->GuideLock) && (OnPage(Mxp, Myp) != -1))
	{
		GxM = -1;
		GyM = -1;
		if (Doc->currentPage->YGuides.count() != 0)
		{
			for (uint yg = 0; yg < Doc->currentPage->YGuides.count(); ++yg)
			{
				if ((Doc->currentPage->YGuides[yg]+Doc->currentPage->Yoffset < (Myp+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->YGuides[yg]+Doc->currentPage->Yoffset > (Myp-Doc->guidesSettings.grabRad)))
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
				if ((Doc->currentPage->XGuides[xg]+Doc->currentPage->Xoffset < (Mxp+Doc->guidesSettings.grabRad)) &&
					 (Doc->currentPage->XGuides[xg]+Doc->currentPage->Xoffset > (Mxp-Doc->guidesSettings.grabRad)))
				{
					GxM = xg;
					QPoint py = viewport()->mapFromGlobal(m->globalPos());
					DrVX = py.x();
					break;
				}
			}
		}
	}
	Deselect(true);
	SelItem.clear();
	return false;
}

void ScribusView::HandleSizer(QPainter *p, PageItem *currItem, QRect mpo, QMouseEvent *m)
{
	currItem->OldB = currItem->Width;
	currItem->OldH = currItem->Height;
	currItem->OldB2 = currItem->Width;
	currItem->OldH2 = currItem->Height;
	HowTo = 0;
	if (currItem->sizeLocked())
		return;
	QRect ne = QRect();
	PaintSizeRect(p, ne);
	FPoint n1;
	double d1;
	QMap<double,int> distance;
	n1 = transformPoint( FPoint(currItem->Width, currItem->Height), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 1);
	n1 = transformPoint( FPoint(0, 0), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 2);
	if (currItem->itemType() != PageItem::Line)
	{
		n1 = transformPoint( FPoint(currItem->Width, 0), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 3);
		n1 = transformPoint( FPoint(0, currItem->Height), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 4);
		n1 = transformPoint( FPoint(currItem->Width/2, currItem->Height), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 5);
		n1 = transformPoint( FPoint(currItem->Width, currItem->Height/2), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 6);
		n1 = transformPoint( FPoint(0, currItem->Height/2), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 7);
		n1 = transformPoint( FPoint(currItem->Width/2, 0), currItem->Xpos, currItem->Ypos, currItem->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 8);
	}
	QValueList<int> result = distance.values();
	if (result.count() != 0)
		HowTo = result[0];
	HandleCurs(p, currItem, mpo);
	if (HowTo != 0)
	{
		if (currItem->itemType() != PageItem::Line)
			currItem->Sizing = true;
		mCG = true;
	}
}

bool ScribusView::GetItem(PageItem **currItem, int nr)
{
	if (SelItem.count() != 0)
	{
		if (nr == -1)
			*(currItem) = SelItem.at(0);
		else
			*(currItem) = SelItem.at(nr);
		return true;
	}
	else
	{
		*(currItem) = 0;
		return false;
	}
}

void ScribusView::Deselect(bool prop)
{
	double x, y, w, h;
	PageItem* currItem;
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			if ((currItem->itemType() == PageItem::TextFrame) && (currItem->isBookmark))
				emit ChBMText(currItem);
			currItem->Select = false;
			currItem->isSingleSel = false;
		}
		if (GroupSel)
		{
			getGroupRect(&x, &y, &w, &h);
			SelItem.clear();
			updateContents(static_cast<int>(x*Scale-5), static_cast<int>(y*Scale-5), static_cast<int>(w*Scale+10), static_cast<int>(h*Scale+10));
		}
		else
		{
			SelItem.clear();
			updateContents(getRedrawBounding(currItem));
		}
		GroupSel = false;
	}
	if (prop)
		emit HaveSel(-1);
	ScApp->propertiesPalette->Cpal->gradEditButton->setOn(false);
}

void ScribusView::updateGradientVectors(PageItem *currItem)
{
	switch (currItem->GrType)
	{
		case 0:
		case 1:
			currItem->GrStartX = 0;
			currItem->GrStartY = currItem->Height / 2.0;
			currItem->GrEndX = currItem->Width;
			currItem->GrEndY = currItem->Height / 2.0;
			break;
		case 2:
			currItem->GrStartX = currItem->Width / 2.0;
			currItem->GrStartY = 0;
			currItem->GrEndX = currItem->Width / 2.0;
			currItem->GrEndY = currItem->Height;
			break;
		case 3:
			currItem->GrStartX = 0;
			currItem->GrStartY = 0;
			currItem->GrEndX = currItem->Width;
			currItem->GrEndY = currItem->Height;
			break;
		case 4:
			currItem->GrStartX = 0;
			currItem->GrStartY = currItem->Height;
			currItem->GrEndX = currItem->Width;
			currItem->GrEndY = 0;
			break;
		case 5:
			currItem->GrStartX = currItem->Width / 2.0;
			currItem->GrStartY = currItem->Height / 2.0;
			if (currItem->Width >= currItem->Height)
			{
				currItem->GrEndX = currItem->Width;
				currItem->GrEndY = currItem->Height / 2.0;
			}
			else
			{
				currItem->GrEndX = currItem->Width / 2.0;
				currItem->GrEndY = currItem->Height;
			}
			break;
		default:
			break;
	}
	currItem->GrEndX = QMIN(QMAX(currItem->GrEndX, 0), currItem->Width);
	currItem->GrEndY = QMIN(QMAX(currItem->GrEndY, 0), currItem->Height);
	currItem->GrStartX = QMIN(QMAX(currItem->GrStartX, 0), currItem->Width);
	currItem->GrStartY = QMIN(QMAX(currItem->GrStartY, 0), currItem->Height);
	ScApp->propertiesPalette->Cpal->setSpecialGradient(currItem->GrStartX * Doc->unitRatio, currItem->GrStartY * Doc->unitRatio,
														 currItem->GrEndX * Doc->unitRatio, currItem->GrEndY * Doc->unitRatio,
														 currItem->Width * Doc->unitRatio, currItem->Height * Doc->unitRatio);
}

void ScribusView::SetupDraw(int nr)
{
	PageItem* currItem = Doc->Items.at(nr);
	currItem->Select = true;
	currItem->IFont = Doc->toolSettings.defFont;
	currItem->ISize = Doc->toolSettings.defSize;
	mCG = true;
	HowTo = 1;
	qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
	SelItem.clear();
	SelItem.append(currItem);
	currItem->paintObj();
	Imoved = true;
	Doc->appMode = NormalMode;
	emit DocChanged();
	currItem->Sizing = currItem->itemType() == PageItem::Line ? false : true;
	EmitValues(currItem);
}

void ScribusView::EmitValues(PageItem *currItem)
{
	emit ItemPos(currItem->Xpos, currItem->Ypos);
	emit ItemGeom(currItem->Width, currItem->Height);
	emit SetAngle(currItem->Rot);
	emit SetSizeValue(currItem->Pwidth);
	emit SetLineArt(currItem->PLineArt, currItem->PLineEnd, currItem->PLineJoin);
	emit SetLocalValues(currItem->LocalScX, currItem->LocalScY, currItem->LocalX, currItem->LocalY );
	emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
	emit ItemGradient(currItem->GrType);
	emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
	emit ItemTextAttr(currItem->LineSp);
	emit ItemTextUSval(currItem->ExtraV);
	emit SetDistValues(currItem->Extra, currItem->TExtra, currItem->BExtra, currItem->RExtra);
	emit ItemTextCols(currItem->Cols, currItem->ColGap);
	if (Doc->appMode != EditMode)
	{
		emit ItemTextAbs(currItem->textAlignment);
		emit ItemTextFont(currItem->IFont);
		emit ItemTextSize(currItem->ISize);
	}
	ScApp->propertiesPalette->Cpal->setSpecialGradient(currItem->GrStartX * Doc->unitRatio, currItem->GrStartY * Doc->unitRatio,
																 currItem->GrEndX * Doc->unitRatio, currItem->GrEndY * Doc->unitRatio,
																 currItem->Width * Doc->unitRatio, currItem->Height * Doc->unitRatio);
}

void ScribusView::ToggleBookmark()
{
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			PageItem* currItem = SelItem.at(a);
			if (currItem->itemType()==PageItem::TextFrame)
			{
				if (currItem->OwnPage != -1)
				{
					bool old = currItem->isBookmark;
					currItem->isBookmark = !currItem->isBookmark;
					if (currItem->isBookmark)
					{
						currItem->isAnnotation = false;
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
		
		ScApp->actionManager->setPDFActions(this);
		
		emit DocChanged();
	}
}

void ScribusView::ToggleAnnotation()
{
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			PageItem* currItem = SelItem.at(a);
			if (currItem->itemType()==PageItem::TextFrame)
			{
				bool old = currItem->isBookmark;
				currItem->isAnnotation = !currItem->isAnnotation;
				if (currItem->isAnnotation)
				{
					if (old)
						emit DelBM(currItem);
					currItem->isBookmark = false;
				}
			}
		}
		
		ScApp->actionManager->setPDFActions(this);
		
		emit DocChanged();
	}
}

void ScribusView::ToggleLock()
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
		{
			if (SelItem.at(0)->locked())
				undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::UnLock, 0, Um::IUnLock);
			else
				undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::Lock, 0, Um::ILock);
		}
		for ( uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->toggleLock();
			RefreshItem(SelItem.at(a));
			emit HaveSel(SelItem.at(a)->itemType());
		}
		emit DocChanged();
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ToggleSizeLock()
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
		{
			if (SelItem.at(0)->sizeLocked())
				undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::SizeUnLock, 0, Um::IUnLock);
			else
				undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::SizeLock, 0, Um::ILock);
		}
		for ( uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->toggleSizeLock();
			RefreshItem(SelItem.at(a));
			emit HaveSel(SelItem.at(a)->itemType());
		}
		emit DocChanged();
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::sentToScrap()
{
	ScriXmlDoc *ss = new ScriXmlDoc();
	emit ToScrap(ss->WriteElem(&SelItem, Doc, this));
	delete ss;
}

void ScribusView::sentToLayer(int id)
{
	QString laName = pmen3->text(id);
	int dd = 0;
	for (uint lam=0; lam < Doc->Layers.count(); ++lam)
	{
		if (Doc->Layers[lam].Name == laName)
		{
			dd = Doc->Layers[lam].LNr;
			break;
		}
	}
	if (SelItem.count() != 0)
	{
		if (UndoManager::undoEnabled() && SelItem.count() > 1)
			undoManager->beginTransaction();
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			PageItem *currItem = SelItem.at(a);
			currItem->setLayer(dd);
			tooltip += "\t" + currItem->getUName() + "\n";
		}
		if (UndoManager::undoEnabled() && SelItem.count() > 1)
			undoManager->commit(Um::Selection,
								Um::IGroup,
								Um::SendToLayer,
								tooltip,
								Um::ILayerAction);
	}

	Deselect(true);
	updateContents();
	emit DocChanged();
}

void ScribusView::ToBack()
{
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			currItem = SelItem.at(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items.findRef(currItem);
			Doc->Items.take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			Doc->Items.prepend(SelItem.at(Oindex[c]));
		}
		for (uint a = 0; a < Doc->Items.count(); ++a)
		{
			Doc->Items.at(a)->ItemNr = a;
			if (Doc->Items.at(a)->isBookmark)
				emit NewBMNr(Doc->Items.at(a)->BMnr, a);
		}
		if (Doc->masterPageMode)
			Doc->MasterItems = Doc->Items;
		else
			Doc->DocItems = Doc->Items;
		ScApp->outlinePalette->BuildTree(Doc);
		emit LevelChanged(0);
		emit DocChanged();
		updateContents();
	}
}

void ScribusView::ToFront()
{
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			currItem = SelItem.at(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items.findRef(currItem);
			Doc->Items.take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = 0; c <static_cast<int>(Oindex.count()); ++c)
		{
			Doc->Items.append(SelItem.at(Oindex[c]));
		}
		for (uint a = 0; a < Doc->Items.count(); ++a)
		{
			Doc->Items.at(a)->ItemNr = a;
			if (Doc->Items.at(a)->isBookmark)
				emit NewBMNr(Doc->Items.at(a)->BMnr, a);
		}
		if (Doc->masterPageMode)
			Doc->MasterItems = Doc->Items;
		else
			Doc->DocItems = Doc->Items;
		ScApp->outlinePalette->BuildTree(Doc);
		emit LevelChanged(SelItem.at(0)->ItemNr);
		emit DocChanged();
		updateContents();
	}
}

void ScribusView::LowerItem()
{
	uint low = Doc->Items.count();
	uint high = 0;
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	PageItem *b2;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			currItem = SelItem.at(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			low = QMIN(currItem->ItemNr, low);
			high = QMAX(currItem->ItemNr, high);
		}
		if (low == 0)
			return;
		b2 = Doc->Items.at(high);
		SelItem.clear();
		SelectItemNr(low-1, false);
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			currItem = SelItem.at(c);
			currItem->Select = false;
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items.findRef(currItem);
			Doc->Items.take(d);
		}
		d = Doc->Items.findRef(b2);
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			Doc->Items.insert(d+1, SelItem.at(Oindex[c]));
		}
		SelItem.clear();
		for (uint a = 0; a < Doc->Items.count(); ++a)
		{
			Doc->Items.at(a)->ItemNr = a;
			if (Doc->Items.at(a)->isBookmark)
				emit NewBMNr(Doc->Items.at(a)->BMnr, a);
			if (Doc->Items.at(a)->Select)
				SelItem.append(Doc->Items.at(a));
		}
		if (Doc->masterPageMode)
			Doc->MasterItems = Doc->Items;
		else
			Doc->DocItems = Doc->Items;
		ScApp->outlinePalette->BuildTree(Doc);
		emit LevelChanged(SelItem.at(0)->ItemNr);
		emit DocChanged();
		updateContents();
	}
}

void ScribusView::RaiseItem()
{
	uint low = Doc->Items.count();
	uint high = 0;
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	PageItem *b2;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			currItem = SelItem.at(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			low = QMIN(currItem->ItemNr, low);
			high = QMAX(currItem->ItemNr, high);
		}
		if (high == Doc->Items.count()-1)
			return;
		b2 = Doc->Items.at(low);
		SelItem.clear();
		SelectItemNr(high+1, false);
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			currItem = SelItem.at(c);
			currItem->Select = false;
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items.findRef(currItem);
			Doc->Items.take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = 0; c <static_cast<int>(Oindex.count()); ++c)
		{
			d = Doc->Items.findRef(b2);
			Doc->Items.insert(d, SelItem.at(Oindex[c]));
		}
		SelItem.clear();
		for (uint a = 0; a < Doc->Items.count(); ++a)
		{
			Doc->Items.at(a)->ItemNr = a;
			if (Doc->Items.at(a)->isBookmark)
				emit NewBMNr(Doc->Items.at(a)->BMnr, a);
			if (Doc->Items.at(a)->Select)
				SelItem.append(Doc->Items.at(a));
		}
		if (Doc->masterPageMode)
			Doc->MasterItems = Doc->Items;
		else
			Doc->DocItems = Doc->Items;
		ScApp->outlinePalette->BuildTree(Doc);
		emit LevelChanged(SelItem.at(0)->ItemNr);
		emit DocChanged();
		updateContents();
	}
}

void ScribusView::ToPicFrame()
{
	//emit Amode(1);
	PageItem *currItem = SelItem.at(0);
	currItem->convertTo(PageItem::ImageFrame);
	currItem->Frame = true;
	RefreshItem(currItem);
	emit HaveSel(currItem->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr);
	EmitValues(currItem);
	emit DocChanged();
}

void ScribusView::ToPolyFrame()
{
	//emit Amode(1);
	PageItem *currItem = SelItem.at(0);
	currItem->convertTo(PageItem::Polygon);
	currItem->Frame = false;
	currItem->ClipEdited = true;
	currItem->FrameType = 3;
	currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->ContourLine = currItem->PoLine.copy();
	RefreshItem(currItem);
	emit HaveSel(currItem->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr);
	EmitValues(currItem);
	emit DocChanged();
}

void ScribusView::ToTextFrame()
{
	//emit Amode(1);
	PageItem *currItem = SelItem.at(0);
	currItem->convertTo(PageItem::TextFrame);
	currItem->Frame = true;
	RefreshItem(currItem);
	emit HaveSel(currItem->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr);
	EmitValues(currItem);
	emit DocChanged();
}

void ScribusView::ToBezierFrame()
{
	//emit Amode(1);
	PageItem *currItem = SelItem.at(0);
	currItem->convertTo(PageItem::PolyLine);
	currItem->ClipEdited = true;
	SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
	AdjustItemSize(currItem);
	RefreshItem(currItem);
	emit HaveSel(currItem->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr);
	EmitValues(currItem);
	emit DocChanged();
}

void ScribusView::Bezier2Poly()
{
	PageItem *currItem = SelItem.at(0);
	currItem->convertTo(PageItem::Polygon);
	currItem->Frame = false;
	currItem->ClipEdited = true;
	currItem->FrameType = 3;
	currItem->PoLine.addPoint(currItem->PoLine.point(currItem->PoLine.size()-2));
	currItem->PoLine.addPoint(currItem->PoLine.point(currItem->PoLine.size()-3));
	currItem->PoLine.addPoint(currItem->PoLine.point(0));
	currItem->PoLine.addPoint(currItem->PoLine.point(0));
	currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->ContourLine = currItem->PoLine.copy();
	RefreshItem(currItem);
	emit HaveSel(currItem->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr);
	EmitValues(currItem);
	emit DocChanged();
}

void ScribusView::ClearItem()
{
	PageItem *currItem;
	if (SelItem.count() != 0)
	{
		currItem = SelItem.at(0);
		if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
		{
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if ((currItem->itemText.count() != 0) && ((currItem->NextBox == 0) || (currItem->BackBox == 0)))
				{
					int t = QMessageBox::warning(this, tr("Warning"),
										tr("Do you really want to clear all your Text?"),
										QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
					if (t == QMessageBox::No)
						return;
				}
				PageItem *nextItem = currItem;
				while (nextItem != 0)
				{
					if (nextItem->BackBox != 0)
						nextItem = nextItem->BackBox;
					else
						break;
				}
				while (nextItem != 0)
				{
					nextItem->itemText.clear();
					nextItem->CPos = 0;
					nextItem = nextItem->NextBox;
				}
			}
			if ((currItem->itemType() == PageItem::ImageFrame) && ((ScApp->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->PicAvail)))
				ScApp->fileWatcher->removeFile(currItem->Pfile);
			currItem->Pfile = "";
			currItem->PicAvail = false;
			currItem->pixm = ScImage();
			if (currItem->itemType() == PageItem::ImageFrame)
			{
				currItem->LocalScX = 1;
				currItem->LocalScY = 1;
				currItem->OrigW = 0;
				currItem->OrigH = 0;
				currItem->LocalX = 0;
				currItem->LocalY = 0;
				currItem->setImageFlippedH(false);
				currItem->setImageFlippedV(false);
				currItem->textAlignment = 0;
				currItem->EmProfile = "";
				currItem->ScaleType = true;
				currItem->AspectRatio = true;
				currItem->setFillTransparency(0.0);
				currItem->setLineTransparency(0.0);
				currItem->imageClip.resize(0);
/*				emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr); */
			}
			updateContents();
			emit DocChanged();
		}
	}
}

void ScribusView::DeleteItem()
{
	uint a, c, anz;
	QPtrList<PageItem> delItems;
	PageItem *currItem;
	if (SelItem.count() != 0)
	{
		anz = SelItem.count();
		uint offs = 0;
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint de = 0; de < anz; ++de)
		{
			currItem = SelItem.at(offs);
			if ((currItem->isTableItem && currItem->isSingleSel) || (currItem->locked()))
			{
				offs++;
				continue;
			}
			tooltip += "\t" + currItem->getUName() + "\n";
			delItems.append(SelItem.take(offs));
		}
		if (delItems.count() == 0)
			return;
		anz = delItems.count();

		if (anz > 1)
			undoManager->beginTransaction(Um::Group + "/" + Um::Selection, Um::IGroup,
										  Um::Delete, tooltip, Um::IDelete);

		for (uint de = 0; de < anz; ++de)
		{
			currItem = delItems.at(0);
			if ((currItem->itemType() == PageItem::ImageFrame) && ((ScApp->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->PicAvail)))
				ScApp->fileWatcher->removeFile(currItem->Pfile);
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if ((currItem->NextBox != 0) || (currItem->BackBox != 0))
				{
					if (currItem->BackBox == 0)
					{
						currItem->NextBox->BackBox = currItem->BackBox;
						c = currItem->itemText.count();
						for (a = 0; a < c; ++a)
							currItem->NextBox->itemText.prepend(currItem->itemText.take(currItem->itemText.count()-1));
						if ((currItem->isAutoText) && (currItem->NextBox == 0))
							Doc->LastAuto = 0;
					}
					else
					{
						currItem->BackBox->NextBox = currItem->NextBox;
						if (currItem->NextBox != 0)
							currItem->NextBox->BackBox = currItem->BackBox;
						else
						{
							if (currItem->isAutoText)
								Doc->LastAuto = currItem->BackBox;
						}
						c = currItem->itemText.count();
						for (a = 0; a < c; ++a)
							currItem->BackBox->itemText.append(currItem->itemText.take(0));
					}
				}
				else
				{
					if (currItem->isAutoText)
					{
						Doc->LastAuto = 0;
						Doc->FirstAuto = 0;
					}
				}
			}
			if (currItem->isBookmark)
				emit DelBM(currItem);
			Doc->Items.remove(currItem);
			currItem->Select = false;
			delItems.removeFirst();
			// send the undo action to the UndoManager
			if (UndoManager::undoEnabled())
			{
				ItemState<PageItem*> *is = new ItemState<PageItem*>(Um::Delete + " " + currItem->getUName(), "", Um::IDelete);
				is->setItem(currItem);
				is->set("DELETE_ITEM", "delete_item");
				UndoObject *target;
				if (currItem->OwnPage > -1)
					target = Doc->Pages.at(currItem->OwnPage);
				else
					target = Doc->Pages.at(0);
				undoManager->action(target, is, currItem->getUPixmap());
			}
		}
		for (a = 0; a < Doc->Items.count(); ++a)
		{
			Doc->Items.at(a)->ItemNr = a;
			if (Doc->Items.at(a)->isBookmark)
				emit NewBMNr(Doc->Items.at(a)->BMnr, a);
		}

		if (anz > 1)
			undoManager->commit();

		if (GroupSel)
		{
			double x, y, w, h;
			getGroupRect(&x, &y, &w, &h);
			GroupSel = false;
			updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
		}
		else
			updateContents();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		if (Doc->masterPageMode)
			Doc->MasterItems = Doc->Items;
		else
			Doc->DocItems = Doc->Items;
		ScApp->outlinePalette->BuildTree(Doc);
		if (SelItem.count() == 0)
			emit HaveSel(-1);
		else
			emit HaveSel(SelItem.at(0)->itemType());
		emit DocChanged();
	}
}

void ScribusView::PasteToPage()
{
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(Doc->currentPage->getUName(), 0, Um::Paste, "", Um::IPaste);
	emit LoadElem(ScApp->Buffer2, qRound(Mxp/Scale), qRound(Myp/Scale), false, false, Doc, this);
	Doc->DraggedElem = 0;
	Doc->DragElements.clear();
	updateContents();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

/** Waagrechter Scrollbalken */
void ScribusView::setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x+270, y, w-270, h);
	if (Ready)
	{
		QFontMetrics fom(LE->font());
		LE->setGeometry(x, y, 60, h);
		if (fom.height() > LE->ed->height())
		{
			QFont ff = LE->font();
			do
			{
				int si = LE->font().pointSize();
				ff.setPointSize(si-1);
				LE->setFont(ff);
				fom = QFontMetrics(LE->font());
			}
			while (fom.height() > LE->ed->height());
			PGS->setFont(ff);
			LY->setFont(ff);
			horizRuler->setFont(ff);
			vertRuler->setFont(ff);
		}
		QRect forec = fom.boundingRect("3200.00 %");
		int sadj = forec.width() - LE->ed->width();
		LE->setGeometry(x, y, 60+sadj, h);
		zoomOutToolbarButton->setGeometry(x+60+sadj, y, 15, h);
		zoomInToolbarButton->setGeometry(x+75+sadj, y, 15, h);
		PGS->setGeometry(x+90+sadj, y, 215, h);
		LY->setGeometry(x+305+sadj, y, 110, h);
		horizRuler->setGeometry(25, 1, w-24, 25);
		bar.setGeometry(x+415+sadj, y, w-(415+sadj), h);
	}
}

/** Senkrechter Scrollbalken */
void ScribusView::setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (Ready)
	{
		vertRuler->setGeometry(1, 25, 25, h-24);
		UN->setGeometry(1, 1, 25, 25);
	}
}

bool ScribusView::mousePressed()
{
	return Mpressed;
}

bool ScribusView::groupTransactionStarted()
{
	return _groupTransactionStarted;
}

void ScribusView::setGroupTransactionStarted(bool isOn)
{
	_groupTransactionStarted = isOn;
}

// jjsa 27-03-2004 add for better settinf while zooming
void ScribusView::rememberPreviousSettings(int mx, int my)
{
	oldX = mx;
	oldY = my;
}

void ScribusView::setRulerPos(int x, int y)
{
	if (ScApp->ScriptRunning)
		return;
	horizRuler->offs = x-static_cast<int>(Doc->ScratchLeft*Scale)-1;
	horizRuler->repX = false;
	horizRuler->repaint();
	vertRuler->offs = y-static_cast<int>(Doc->ScratchTop*Scale)-1;
	vertRuler->repaint();
	evSpon = true;
}

void ScribusView::Zval()
{
	int x = qRound(QMAX(contentsX() / Scale, 0));
	int y = qRound(QMAX(contentsY() / Scale, 0));
	int w = qRound(QMIN(visibleWidth() / Scale, Doc->pageWidth));
	int h = qRound(QMIN(visibleHeight() / Scale, Doc->pageHeight));
	rememberPreviousSettings(w / 2 + x,h / 2 + y);
	Scale = LE->value() / 100.0 * Prefs->DisScale;
	slotDoZoom();
	ScApp->setFocus();
}

/** Adds a Page */
Page* ScribusView::addPage(int nr)
{
	Page* fe = new Page(Doc->ScratchLeft, Doc->pageCount*(Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop, Doc->pageWidth, Doc->pageHeight);
	fe->Margins.Top = Doc->pageMargins.Top;
	fe->Margins.Bottom = Doc->pageMargins.Bottom;
	fe->setPageNr(nr);
	Doc->Pages.insert(nr, fe);
	Doc->currentPage = fe;
	Doc->pageCount++;
	PGS->setMaxValue(Doc->pageCount);
	reformPages();
	if ((Doc->PageAT) && (!Doc->loading))
	{
		int z = PaintText(fe->Margins.Left+fe->Xoffset,
		                  fe->Margins.Top+fe->Yoffset,
		                  Doc->pageWidth-fe->Margins.Right-fe->Margins.Left,
		                  Doc->pageHeight-fe->Margins.Bottom-fe->Margins.Top,
		                  1, Doc->toolSettings.dPen);
		Doc->Items.at(z)->isAutoText = true;
		Doc->Items.at(z)->BackBox = Doc->LastAuto;
		Doc->Items.at(z)->Cols = qRound(Doc->PageSp);
		Doc->Items.at(z)->ColGap = Doc->PageSpa;
		if (Doc->LastAuto != 0)
			Doc->LastAuto->NextBox = Doc->Items.at(z);
		else
			Doc->FirstAuto = Doc->Items.at(z);
		Doc->LastAuto = Doc->Items.at(z);
		SetRectFrame(Doc->Items.at(z));
		if (nr > 0)
		{
			bool savre = Doc->RePos;
			Doc->RePos = true;
			QPixmap pgPix(1, 1);
			ScPainter *painter = new ScPainter(&pgPix, 1, 1);
			painter->translate(0.5, 0.5);
			if (Doc->Items.at(z)->BackBox != 0)
				Doc->Items.at(z)->BackBox->DrawObj(painter, QRect(0, 0, 1, 1));
			painter->end();
			delete painter;
			Doc->RePos = savre;
		}
	}
	if ((!ScApp->ScriptRunning) && (!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(nr);
	Mpressed = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Imoved = false;
	MidButt = false;
	HaveSelRect = false;
	Magnify = false;
	FirstPoly = true;
	EdPoints = true;
	GroupSel = false;
	DraggedGroup = false;
	MoveGY = false;
	MoveGX = false;
	EditContour = false;
	return fe;
}

/** Lscht eine Seite */
void ScribusView::delPage(int Nr)
{
	if (Doc->pageCount == 1)
		return;
	if (Doc->pageCount < Nr-1)
		return;
	Page* Seite;
	Seite = Doc->Pages.at(Nr);
	PageItem *currItem;
	for (currItem = Seite->FromMaster.first(); currItem; currItem = Seite->FromMaster.next())
	{
		if (currItem->ChangedMasterItem)
		{
			Seite->FromMaster.remove(currItem);
			delete currItem;
		}
	}
	Seite->FromMaster.clear();
	Doc->Pages.remove(Nr);
	delete Seite;
	Doc->pageCount -= 1;
	Doc->currentPage = Doc->Pages.at(0);
	if (Doc->masterPageMode)
		Doc->MasterPages = Doc->Pages;
	else
		Doc->DocPages = Doc->Pages;
	PGS->setMaxValue(Doc->pageCount);
	PGS->GotoPg(0);
}

void ScribusView::movePage(int from, int to, int ziel, int art)
{
	QPtrList<Page> Buf;
	int a;
	uint b;
	int zz = ziel;
	Buf.clear();
	for (a = from; a < to; a++)
	{
		Buf.append(Doc->Pages.at(from));
		Doc->Pages.remove(from);
		if (a <= zz)
			zz--;
	}
	switch (art)
	{
	case 0:
		for (b = 0; b < Buf.count(); b++)
		{
			Doc->Pages.insert(zz, Buf.at(b));
			zz++;
		}
		break;
	case 1:
		for (b = 0; b < Buf.count(); b++)
		{
			zz++;
			Doc->Pages.insert(zz, Buf.at(b));
		}
		break;
	case 2:
		for (b = 0; b < Buf.count(); b++)
		{
			Doc->Pages.append(Buf.at(b));
		}
		break;
	}
	if (Doc->masterPageMode)
		Doc->MasterPages = Doc->Pages;
	else
		Doc->DocPages = Doc->Pages;
	reformPages();
}

void ScribusView::reformPages()
{
	Page* Seite;
	QMap<uint, oldPageVar> pageTable;
	struct oldPageVar oldPg;
	for (uint a = 0; a < Doc->Pages.count(); ++a)
	{
		Seite = Doc->Pages.at(a);
		oldPg.oldXO = Seite->Xoffset;
		oldPg.oldYO = Seite->Yoffset;
		oldPg.newPg = a;
		pageTable.insert(Seite->PageNr, oldPg);
		Seite->setPageNr(a);
		if (Doc->PageFP)
		{
			if (Doc->MasterP)
			{
				if (Seite->LeftPg)
				{
					Seite->Margins.Left = Doc->pageMargins.Right;
					Seite->Margins.Right = Doc->pageMargins.Left;
				}
				else
				{
					Seite->Margins.Right = Doc->pageMargins.Right;
					Seite->Margins.Left = Doc->pageMargins.Left;
				}
			}
			else
			{
				if (a % 2 == 0)
				{
					if (Doc->FirstPageLeft)
					{
						Seite->Margins.Left = Doc->pageMargins.Right;
						Seite->Margins.Right = Doc->pageMargins.Left;
					}
					else
					{
						Seite->Margins.Right = Doc->pageMargins.Right;
						Seite->Margins.Left = Doc->pageMargins.Left;
					}
				}
				else
				{
					if (Doc->FirstPageLeft)
					{
						Seite->Margins.Right = Doc->pageMargins.Right;
						Seite->Margins.Left = Doc->pageMargins.Left;
					}
					else
					{
						Seite->Margins.Left = Doc->pageMargins.Right;
						Seite->Margins.Right = Doc->pageMargins.Left;
					}
				}
			}
		}
		else
		{
			Seite->Margins.Right = Doc->pageMargins.Right;
			Seite->Margins.Left = Doc->pageMargins.Left;
		}
		Seite->Margins.Top = Doc->pageMargins.Top;
		Seite->Margins.Bottom = Doc->pageMargins.Bottom;
		if (Doc->MasterP)
		{
			Seite->Xoffset = Doc->ScratchLeft;
			Seite->Yoffset = Doc->ScratchTop;
		}
		else
		{
			if (Doc->PageFP)
			{
				if (a % 2 == 0)
				{
					if (Doc->FirstPageLeft)
					{
						Seite->Xoffset = Doc->ScratchLeft;
						Seite->Yoffset = a/2 * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
					else
					{
						Seite->Xoffset = Doc->ScratchLeft + Doc->pageWidth;
						Seite->Yoffset = (a+1)/2 * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
				}
				else
				{
					if (Doc->FirstPageLeft)
					{
						Seite->Xoffset = Doc->ScratchLeft + Doc->pageWidth;
						Seite->Yoffset = a/2 * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
					else
					{
						Seite->Xoffset = Doc->ScratchLeft;
						Seite->Yoffset = (a+1)/2 * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
				}
			}
			else
			{
				Seite->Xoffset = Doc->ScratchLeft;
				Seite->Yoffset = a * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
			}
		}
	}
	for (uint ite = 0; ite < Doc->Items.count(); ++ite)
	{
		PageItem *item = Doc->Items.at(ite);
		if (item->OwnPage < 0)
		{
			item->OwnPage = OnPage(item);
			setRedrawBounding(item);
		}
		else
		{
			oldPg = pageTable[item->OwnPage];
			item->Xpos = item->Xpos - oldPg.oldXO + Doc->Pages.at(oldPg.newPg)->Xoffset;
			item->Ypos = item->Ypos - oldPg.oldYO + Doc->Pages.at(oldPg.newPg)->Yoffset;
			item->OwnPage = static_cast<int>(oldPg.newPg);
			setRedrawBounding(item);
		}
	}
	if (Doc->PageFP)
	{
		if (Doc->FirstPageLeft)
			resizeContents(qRound((Doc->pageWidth*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(((Doc->pageCount-1)/2 + 1) * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
		else
			resizeContents(qRound((Doc->pageWidth*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound((Doc->pageCount/2 + 1) * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
	}
	else
		resizeContents(qRound((Doc->pageWidth+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(Doc->pageCount * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
	if (!ScApp->ScriptRunning)
		setContentsPos(qRound((Doc->currentPage->Xoffset-10) * Scale), qRound((Doc->currentPage->Yoffset-10) * Scale));
	setRulerPos(contentsX(), contentsY());
	setMenTxt(Doc->currentPage->PageNr);
}

void ScribusView::setMenTxt(int Seite)
{
	if (ScApp->ScriptRunning)
		return;
	disconnect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	PGS->setMaxValue(Doc->pageCount);
	if ((!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(Seite);
	connect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
}

/** Fuehrt die Vergroesserung/Verkleinerung aus */
void ScribusView::slotDoZoom()
{
	undoManager->setUndoEnabled(false);
	if (Scale > 32*Prefs->DisScale)
	{
		Scale = 32*Prefs->DisScale;
		return;
	}
	updateOn = false;
	if (Doc->PageFP)
	{
		if (Doc->FirstPageLeft)
			resizeContents(qRound((Doc->pageWidth*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(((Doc->pageCount-1)/2 + 1) * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
		else
			resizeContents(qRound((Doc->pageWidth*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound((Doc->pageCount/2 + 1) * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
	}
	else
		resizeContents(qRound((Doc->pageWidth+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(Doc->pageCount * (Doc->pageHeight+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
	setRulerPos(contentsX(), contentsY());
	if (SelItem.count() != 0)
	{
		PageItem *currItem = SelItem.at(0);
		SetCCPo(static_cast<int>(currItem->Xpos + currItem->Width/2), static_cast<int>(currItem->Ypos + currItem->Height/2));
	}
	else
		SetCCPo(oldX, oldY);
	updateOn = true;
	DrawNew();
	undoManager->setUndoEnabled(true);
}

void ScribusView::slotZoomIn(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(QMAX(contentsX() / Scale, 0));
		int y = qRound(QMAX(contentsY() / Scale, 0));
		int w = qRound(QMIN(visibleWidth() / Scale, Doc->pageWidth));
		int h = qRound(QMIN(visibleHeight() / Scale, Doc->pageHeight));
		rememberPreviousSettings(w / 2 + x,h / 2 + y);
	}
	else
		rememberPreviousSettings(mx,my);
	Scale *= 2;
	if (Scale > 32*Prefs->DisScale)
	{
		Scale = 32*Prefs->DisScale;
		return;
	}
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(QMAX(contentsX() / Scale, 0));
		int y = qRound(QMAX(contentsY() / Scale, 0));
		int w = qRound(QMIN(visibleWidth() / Scale, Doc->pageWidth));
		int h = qRound(QMIN(visibleHeight() / Scale, Doc->pageHeight));
		rememberPreviousSettings(w / 2 + x,h / 2 + y);
	}
	else
		rememberPreviousSettings(mx,my);
	Scale /= 2;
	slotDoZoom();
}


void ScribusView::slotZoomIn2(int mx,int my)
{
	rememberPreviousSettings(mx,my);
	Scale += static_cast<double>(Doc->toolSettings.magStep*Prefs->DisScale)/100.0;
	if (Scale > static_cast<double>(Doc->toolSettings.magMax*Prefs->DisScale)/100.0)
	{
		Scale = static_cast<double>(Doc->toolSettings.magMax*Prefs->DisScale)/100.0;
		return;
	}
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut2(int mx,int my)
{
	rememberPreviousSettings(mx,my);
	Scale -= static_cast<double>(Doc->toolSettings.magStep*Prefs->DisScale)/100.0;
	if (Scale < static_cast<double>(Doc->toolSettings.magMin*Prefs->DisScale)/100.0)
		Scale = static_cast<double>(Doc->toolSettings.magMin*Prefs->DisScale)/100.0;
	slotDoZoom();
}

void ScribusView::DrawNew()
{
	if (ScApp->ScriptRunning)
		return;
	updateContents();
	horizRuler->repX = false;
	horizRuler->repaint();
	vertRuler->repaint();
	setMenTxt(Doc->currentPage->PageNr);
	disconnect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
	LE->setValue(Scale/Prefs->DisScale*100);
	connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
}

void ScribusView::SetCCPo(int x, int y)
{
	if (ScApp->ScriptRunning)
		return;
	center(static_cast<int>(x * Scale), static_cast<int>(y * Scale));
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::SetCPo(int x, int y)
{
	if (ScApp->ScriptRunning)
		return;
	setContentsPos(static_cast<int>(x * Scale), static_cast<int>(y * Scale));
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::LaMenu()
{
	uint a;
	QValueList<Layer>::iterator it;
	disconnect(Laymen, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
 	Laymen->clear();
 	if (Doc->Layers.count() != 0)
 	{
		for (a=0; a < Doc->Layers.count(); a++)
 		{
			for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it)
			{
				if (Doc->Layers.count()-(*it).Level-1 == a)
					Laymen->insertItem((*it).Name);
			}
 		}
 	}
	for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it)
	{
		if ((*it).LNr == Doc->ActiveLayer)
			break;
	}
	Laymen->setItemChecked(Laymen->idAt(Laymen->count()-1-(*it).Level), true);
	connect(Laymen, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
}

void ScribusView::setLayMenTxt(int l)
{
	QValueList<Layer>::iterator it;
	QString lName;
	disconnect(Laymen, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it)
	{
		if ((*it).LNr == l)
			break;
	}
	lName=(*it).Name;
	int len = lName.length();
	while (LY->fontMetrics().width(lName) > LY->width()-25)
	{
		len--;
		if (len == 0)
			break;
		lName.truncate(len);
		lName += ".";
	}
	LY->setText(lName);
	connect(Laymen, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
}

void ScribusView::GotoLa(int l)
{
	int d = Doc->Layers.count()-Laymen->indexOf(l)-1;
	QValueList<Layer>::iterator it;
	for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it)
	{
		if ( d == (*it).Level )
		{
			Doc->ActiveLayer = (*it).LNr;
			setLayMenTxt((*it).LNr);
			emit changeLA((*it).LNr);
			break;
		}
	}
	for (uint al = 0; al < Laymen->count(); ++al)
	{
		Laymen->setItemChecked(Laymen->idAt(al), false);
	}
	Laymen->setItemChecked(l, true);
}

void ScribusView::GotoPa(int Seite)
{
	GotoPage(Seite-1);
	ScApp->setFocus();
}

void ScribusView::ChgUnit(int art)
{
	int d = Unitmen->indexOf(art);
	emit changeUN(d);
}

void ScribusView::GotoPage(int Seite)
{
	Doc->currentPage = Doc->Pages.at(Seite);
	if (ScApp->ScriptRunning)
		return;
	SetCPo(qRound(Doc->currentPage->Xoffset-10), qRound(Doc->currentPage->Yoffset-10));
	PGS->setMaxValue(Doc->pageCount);
	if ((!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(Seite);
}

void ScribusView::showMasterPage(int nr)
{
	OldScale = Scale;
	if (!Doc->MasterP)
	{
		Doc->DocPages = Doc->Pages;
		Doc->Pages = Doc->MasterPages;
		Doc->DocItems = Doc->Items;
		Doc->Items = Doc->MasterItems;
	}
	Doc->pageCount = 1;
	Doc->MasterP = true;
	Doc->currentPage = Doc->Pages.at(nr);
	PGS->setEnabled(false);
	updateOn = false;
//	reformPages();
	slotDoZoom();
	oldX = qRound(Doc->currentPage->Xoffset - 10);
	oldY = qRound(Doc->currentPage->Yoffset - 10);
	SetCPo(oldX, oldY);
	updateOn = true;
	DrawNew();
}

void ScribusView::hideMasterPage()
{
	Deselect(true);
	Doc->MasterItems = Doc->Items;
	Doc->Items = Doc->DocItems;
	Doc->MasterPages = Doc->Pages;
	Doc->pageCount = Doc->DocPages.count();
	Doc->Pages = Doc->DocPages;
	Doc->MasterP = false;
	Doc->currentPage = Doc->Pages.at(0);
	PGS->setEnabled(true);
	Scale = OldScale;
	updateOn = false;
	GotoPage(0);
	slotDoZoom();
	oldX = qRound(Doc->currentPage->Xoffset - 10);
	oldY = qRound(Doc->currentPage->Yoffset - 10);
	SetCPo(oldX, oldY);
	updateOn = true;
	DrawNew();
}

int ScribusView::CountElements()
{
	return static_cast<int>(Doc->Items.count());
}

void ScribusView::RecalcPictures(ProfilesL *Pr, QProgressBar *dia)
{
	PageItem* it;
	int counter;
	if (dia != NULL)
		counter = dia->progress();
	else
		counter = 0;
	if (Doc->Items.count() != 0)
	{
		for (uint i=0; i < Doc->Items.count(); i++)
		{
			it = Doc->Items.at(i);
			if ((it->itemType() == PageItem::ImageFrame) && (it->PicAvail))
			{
				if (Pr->contains(it->IProfile))
					LoadPict(it->Pfile, i, true);
				else
				{
					it->IProfile = Doc->CMSSettings.DefaultInputProfile;
					LoadPict(it->Pfile, i, true);
				}
			}
			counter++;
			if (dia != NULL)
				dia->setProgress(counter);
		}
	}
}

QImage ScribusView::MPageToPixmap(QString name, int maxGr)
{
	QImage pm;
	QImage im;
	int Nr = Doc->MasterNames[name];
	int clipx = static_cast<int>(Doc->ScratchLeft);
	int clipy = static_cast<int>(Doc->ScratchTop);
	int clipw = qRound(Doc->MasterPages.at(Nr)->Width);
	int cliph = qRound(Doc->MasterPages.at(Nr)->Height);
	if ((clipw > 0) && (cliph > 0))
	{
		double sca = Scale;
		bool frs = Doc->guidesSettings.framesShown;
		bool mas = Doc->MasterP;
		Page* act = Doc->currentPage;
		Doc->currentPage = Doc->MasterPages.at(Nr);
		if (!mas)
		{
			Doc->DocItems = Doc->Items;
			Doc->Items = Doc->MasterItems;
		}
		Doc->guidesSettings.framesShown = false;
		Scale = 1;
		previewMode = true;
		pm = QImage(clipw, cliph, 32, QImage::BigEndian);
		ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height());
		painter->clear(white);
		painter->translate(-clipx, -clipy);
		painter->setLineWidth(1);
		painter->setBrush(Doc->papColor);
		painter->drawRect(clipx, clipy, clipw, cliph);
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		Doc->guidesSettings.framesShown = frs;
		Scale = sca;
		Doc->currentPage = act;
		if (!mas)
		{
			Doc->MasterItems = Doc->Items;
			Doc->Items = Doc->DocItems;
		}
		Doc->MasterP = mas;
		painter->end();
		double sx = im.width() / static_cast<double>(maxGr);
		double sy = im.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im = pm.smoothScale(static_cast<int>(pm.width() / sx), static_cast<int>(pm.height() / sx));
		else
			im = pm.smoothScale(static_cast<int>(pm.width() / sy), static_cast<int>(pm.height() / sy));
		delete painter;
		previewMode = false;
	}
	return im;
}

QImage ScribusView::PageToPixmap(int Nr, int maxGr)
{
	QImage pm;
	QImage im;
	int clipx = static_cast<int>(Doc->Pages.at(Nr)->Xoffset);
	int clipy = static_cast<int>(Doc->Pages.at(Nr)->Yoffset);
	int clipw = qRound(Doc->Pages.at(Nr)->Width);
	int cliph = qRound(Doc->Pages.at(Nr)->Height);
	if ((clipw > 0) && (cliph > 0))
	{
		double sca = Scale;
		bool frs = Doc->guidesSettings.framesShown;
		Doc->guidesSettings.framesShown = false;
		Scale = 1;
		previewMode = true;
		Page* act = Doc->currentPage;
		Doc->currentPage = Doc->Pages.at(Nr);
		pm = QImage(clipw, cliph, 32, QImage::BigEndian);
		ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height());
		painter->clear(Doc->papColor);
		painter->translate(-clipx, -clipy);
		painter->setFillMode(ScPainter::Solid);
		painter->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
		painter->setBrush(Doc->papColor);
		painter->drawRect(clipx, clipy, clipw, cliph);
		painter->translate(0.5, 0.5);
		DrawMasterItems(painter, Doc->Pages.at(Nr), QRect(clipx, clipy, clipw, cliph));
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->end();
		Doc->guidesSettings.framesShown = frs;
		Scale = sca;
		Doc->currentPage = act;
		double sx = pm.width() / static_cast<double>(maxGr);
		double sy = pm.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im = pm.smoothScale(static_cast<int>(pm.width() / sx), static_cast<int>(pm.height() / sx));
		else
			im = pm.smoothScale(static_cast<int>(pm.width() / sy), static_cast<int>(pm.height() / sy));
		delete painter;
		previewMode = false;
	}
	return im;
}

void ScribusView::FromHRuler(QMouseEvent *m)
{
	if (Doc->guidesSettings.guidesShown)
	{
		QPoint py = viewport()->mapFromGlobal(m->globalPos());
		int newY = py.y();
		QPoint out = viewportToContents(py);
		emit MousePos((py.x() + contentsX())/Scale-Doc->currentPage->Xoffset, (py.y() + contentsY())/Scale-Doc->currentPage->Yoffset);
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
}

void ScribusView::FromVRuler(QMouseEvent *m)
{
	if (Doc->guidesSettings.guidesShown)
	{
		QPoint py = viewport()->mapFromGlobal(m->globalPos());
		int newY = py.x();
		QPoint out = viewportToContents(py);
		emit MousePos((py.x() + contentsX())/Scale-Doc->currentPage->Xoffset, (py.y() + contentsY())/Scale-Doc->currentPage->Yoffset);
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
}

void ScribusView::SetYGuide(QMouseEvent *m, int oldIndex)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	double newX = (py.x() + contentsX()) / Scale;
	double newY = (py.y() + contentsY()) / Scale;
	int pg = OnPage(newX, newY);
	if (pg != -1)
	{
		if (oldIndex < 0)
			Doc->Pages.at(pg)->addYGuide(newY-Doc->Pages.at(pg)->Yoffset);
		else
			Doc->Pages.at(pg)->moveYGuide(oldIndex, newY-Doc->Pages.at(pg)->Yoffset);
		emit DocChanged();
	}
	else if (oldIndex >= 0)
	{
		Doc->currentPage->removeYGuide(oldIndex);
		emit DocChanged();
	}
	updateContents();
}

void ScribusView::SetXGuide(QMouseEvent *m, int oldIndex)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	double newX = (py.x() + contentsX()) / Scale;
	double newY = (py.y() + contentsY()) / Scale;
	int pg = OnPage(newX, newY);
	if (pg != -1)
	{
		if (oldIndex < 0)
			Doc->Pages.at(pg)->addXGuide(newX-Doc->Pages.at(pg)->Xoffset);
		else
			Doc->Pages.at(pg)->moveXGuide(oldIndex, newX-Doc->Pages.at(pg)->Xoffset);
		emit DocChanged();
	}
	else if (oldIndex >= 0)
	{
		Doc->currentPage->removeXGuide(oldIndex);
		emit DocChanged();
	}
	updateContents();
}

void ScribusView::Transform(PageItem *currItem, QPainter *p)
{
	p->translate(static_cast<int>(currItem->Xpos*Scale), static_cast<int>(currItem->Ypos*Scale));
	p->scale(Scale, Scale);
	p->rotate(currItem->Rot);
}

void ScribusView::TransformM(PageItem *currItem, QPainter *p)
{
	if (currItem->imageFlippedH())
	{
		p->translate(currItem->Width, 0);
		p->scale(-1, 1);
	}
	if (currItem->imageFlippedV())
	{
		p->translate(0, currItem->Height);
		p->scale(1, -1);
	}
}

void ScribusView::SetFrameShape(PageItem *currItem, int count, double *vals)
{
	currItem->PoLine.resize(0);
	for (int a = 0; a < count-3; a += 4)
	{
		if (vals[a] < 0)
		{
			currItem->PoLine.setMarker();
			continue;
		}
		double x1 = currItem->Width * vals[a] / 100.0;
		double y1 = currItem->Height * vals[a+1] / 100.0;
		double x2 = currItem->Width * vals[a+2] / 100.0;
		double y2 = currItem->Height * vals[a+3] / 100.0;
		currItem->PoLine.addPoint(x1, y1);
		currItem->PoLine.addPoint(x2, y2);
	}
	currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->ClipEdited = true;
	setRedrawBounding(currItem);
}

void ScribusView::SetRectFrame(PageItem *currItem)
{
	double rect[] = {   0.0,   0.0,   0.0,   0.0,
	                    100.0,   0.0, 100.0,   0.0,
	                    100.0,   0.0, 100.0,   0.0,
	                    100.0, 100.0, 100.0, 100.0,
	                    100.0, 100.0, 100.0, 100.0,
	                    0.0, 100.0,   0.0, 100.0,
	                    0.0, 100.0,   0.0, 100.0,
	                    0.0,   0.0,   0.0,   0.0};
	SetFrameShape(currItem, 32, rect);
	currItem->ClipEdited = false;
	currItem->FrameType = 0;
}

void ScribusView::SetOvalFrame(PageItem *currItem)
{
	double rect[] = { 100.0,  50.0, 100.0,       77.615235,
	                  50.0, 100.0,  77.615235, 100.0,
	                  50.0, 100.0,  22.385765, 100.0,
	                  0.0,  50.0,   0.0,       77.615235,
	                  0.0,  50.0,   0.0,       22.385765,
	                  50.0,   0.0,  22.385765,   0.0,
	                  50.0,   0.0,  77.615235,   0.0,
	                  100.0,  50.0, 100.0,       22.385765};
	SetFrameShape(currItem, 32, rect);
	currItem->FrameType = 1;
	currItem->ClipEdited = false;
}

void ScribusView::SetFrameRound(PageItem* currItem)
{
	ClRe = -1;
	currItem->RadRect = QMIN(currItem->RadRect, QMIN(currItem->Width,currItem->Height)/2);
	currItem->PoLine.resize(0);
	double rr = fabs(currItem->RadRect);
	if (currItem->RadRect > 0)
	{
		currItem->PoLine.addQuadPoint(rr, 0, rr, 0, currItem->Width-rr, 0, currItem->Width-rr, 0);
		currItem->PoLine.addQuadPoint(currItem->Width-rr, 0, currItem->Width-rr+rr*0.552284749, 0, currItem->Width, rr, currItem->Width, rr*0.552284749);
		currItem->PoLine.addQuadPoint(currItem->Width, rr, currItem->Width, rr, currItem->Width, currItem->Height-rr, currItem->Width, currItem->Height-rr);
		currItem->PoLine.addQuadPoint(currItem->Width, currItem->Height-rr, currItem->Width, currItem->Height-rr+rr*0.552284749, currItem->Width-rr, currItem->Height, currItem->Width-rr+rr*0.552284749, currItem->Height);
		currItem->PoLine.addQuadPoint(currItem->Width-rr, currItem->Height, currItem->Width-rr, currItem->Height, rr, currItem->Height, rr, currItem->Height);
		currItem->PoLine.addQuadPoint(rr, currItem->Height, rr*0.552284749, currItem->Height, 0, currItem->Height-rr, 0, currItem->Height-rr+rr*0.552284749);
		currItem->PoLine.addQuadPoint(0, currItem->Height-rr, 0, currItem->Height-rr, 0, rr, 0, rr);
		currItem->PoLine.addQuadPoint(0, rr, 0, rr*0.552284749, rr, 0, rr*0.552284749, 0);
	}
	else
	{
		currItem->PoLine.addQuadPoint(rr, 0, rr, 0, currItem->Width-rr, 0, currItem->Width-rr, 0);
		currItem->PoLine.addQuadPoint(currItem->Width-rr, 0, currItem->Width-rr, rr*0.552284749, currItem->Width, rr, currItem->Width-rr*0.552284749, rr);
		currItem->PoLine.addQuadPoint(currItem->Width, rr, currItem->Width, rr, currItem->Width, currItem->Height-rr, currItem->Width, currItem->Height-rr);
		currItem->PoLine.addQuadPoint(currItem->Width, currItem->Height-rr, currItem->Width-rr*0.552284749, currItem->Height-rr, currItem->Width-rr, currItem->Height, currItem->Width-rr, currItem->Height-rr*0.552284749);
		currItem->PoLine.addQuadPoint(currItem->Width-rr, currItem->Height, currItem->Width-rr, currItem->Height, rr, currItem->Height, rr, currItem->Height);
		currItem->PoLine.addQuadPoint(rr, currItem->Height, rr, currItem->Height-rr*0.552284749, 0, currItem->Height-rr, rr*0.552284749, currItem->Height-rr);
		currItem->PoLine.addQuadPoint(0, currItem->Height-rr, 0, currItem->Height-rr, 0, rr, 0, rr);
		currItem->PoLine.addQuadPoint(0, rr, rr*0.552284749, rr, rr, 0, rr, rr*0.552284749);
	}
	currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->ClipEdited = false;
	currItem->FrameType = 2;
	setRedrawBounding(currItem);
	emit ItemRadius(currItem->RadRect);
}

void ScribusView::SetFrameRect()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		SetRectFrame(currItem);
		updateContents(getRedrawBounding(currItem));
	}
}

void ScribusView::SetFrameRounded()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		if (currItem->RadRect == 0)
		{
			SetFrameRect();
			return;
		}
		SetFrameRound(currItem);
		updateContents(getRedrawBounding(currItem));
	}
}

void ScribusView::SetFrameOval()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		SetOvalFrame(currItem);
		updateContents(getRedrawBounding(currItem));
	}
}

/** Zeichnet eine Ellipse */
int ScribusView::PaintEllipse(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
	}
	PageItem* ite = new PageItem(Doc, PageItem::Polygon, x, y, b, h, w, fill, outline);
	Doc->Items.append(ite);
	if (Doc->MasterP)
		Doc->MasterItems = Doc->Items;
	else
		Doc->DocItems = Doc->Items;
	ite->PLineArt = PenStyle(Doc->toolSettings.dLineArt);
	ite->setFillShade(Doc->toolSettings.dShade);
	ite->setLineShade(Doc->toolSettings.dShade2);
	ite->ItemNr = Doc->Items.count()-1;
	SetOvalFrame(ite);
	ite->ContourLine = ite->PoLine.copy();
	if (!Doc->loading)
	{
		ite->paintObj();
//		emit AddObj(ite);
	}
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(ite);
		UndoObject *target;
		if (ite->OwnPage > -1)
			target = Doc->Pages.at(ite->OwnPage);
		else
			target = Doc->Pages.at(0);
		undoManager->action(target, is);
		if (!Mpressed) // commit the creation transaction if the item
		{              // is not created by dragging with a mouse
			ite->checkChanges(true);
			undoManager->commit(Doc->Pages.at(ite->OwnPage)->getUName(), ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return ite->ItemNr;
}

/** Zeichnet einen Bildrahmen */
int ScribusView::PaintPict(double x, double y, double b, double h)
{
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
	}
	PageItem* ite = new PageItem(Doc, PageItem::ImageFrame, x, y, b, h, 1, Doc->toolSettings.dBrushPict, "None");
	Doc->Items.append(ite);
	if (Doc->MasterP)
		Doc->MasterItems = Doc->Items;
	else
		Doc->DocItems = Doc->Items;
	ite->setFillShade(Doc->toolSettings.shadePict);
	ite->LocalScX = Doc->toolSettings.scaleX;
	ite->LocalScY = Doc->toolSettings.scaleY;
	ite->ScaleType = Doc->toolSettings.scaleType;
	ite->AspectRatio = Doc->toolSettings.aspectRatio;
	ite->IProfile = Doc->CMSSettings.DefaultInputProfile;
	ite->IRender = Doc->CMSSettings.DefaultIntentMonitor2;
	ite->ItemNr = Doc->Items.count()-1;
	SetRectFrame(ite);
	ite->ContourLine = ite->PoLine.copy();
	if (!Doc->loading)
	{
		ite->paintObj();
//		emit AddObj(ite);
	}
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(ite);
		UndoObject *target = Doc->Pages.at(0);
		if (ite->OwnPage > -1)
			target = target = Doc->Pages.at(ite->OwnPage);
		undoManager->action(target, is);
		if (!Mpressed)
		{
			ite->checkChanges(true);
			undoManager->commit(Doc->Pages.at(ite->OwnPage)->getUName(), ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return ite->ItemNr;
}

/** Zeichnet ein Rechteck */
int ScribusView::PaintRect(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
	}
	PageItem* ite = new PageItem(Doc, PageItem::Polygon, x, y, b, h, w, fill, outline);
	Doc->Items.append(ite);
	if (Doc->MasterP)
		Doc->MasterItems = Doc->Items;
	else
		Doc->DocItems = Doc->Items;
	ite->PLineArt = PenStyle(Doc->toolSettings.dLineArt);
	ite->setFillShade(Doc->toolSettings.dShade);
	ite->setLineShade(Doc->toolSettings.dShade2);
	ite->ItemNr = Doc->Items.count()-1;
	SetRectFrame(ite);
	ite->ContourLine = ite->PoLine.copy();
	if (!Doc->loading)
	{
		ite->paintObj();
//		emit AddObj(ite);
	}
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(ite);
		UndoObject *target = Doc->Pages.at(0);
		if (ite->OwnPage > -1)
			target = target = Doc->Pages.at(ite->OwnPage);
		undoManager->action(target, is);
		if (!Mpressed)
		{
			ite->checkChanges(true);
			undoManager->commit(Doc->Pages.at(ite->OwnPage)->getUName(), ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return ite->ItemNr;
}

/** Zeichnet ein Polygon */
int ScribusView::PaintPoly(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
	}
	PageItem* ite = new PageItem(Doc, PageItem::Polygon, x, y, b, h, w, fill, outline);
	Doc->Items.append(ite);
	if (Doc->MasterP)
		Doc->MasterItems = Doc->Items;
	else
		Doc->DocItems = Doc->Items;
	ite->PLineArt = PenStyle(Doc->toolSettings.dLineArt);
	ite->setFillShade(Doc->toolSettings.dShade);
	ite->setLineShade(Doc->toolSettings.dShade2);
	ite->ItemNr =Doc-> Items.count()-1;
	ite->ClipEdited = true;
	ite->FrameType = 3;
	if (!Doc->loading)
	{
		ite->paintObj();
//		emit AddObj(ite);
	}
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(ite);
		UndoObject *target = Doc->Pages.at(0);
		if (ite->OwnPage > -1)
			target = target = Doc->Pages.at(ite->OwnPage);
		undoManager->action(target, is);
		if (!Mpressed)
		{
			ite->checkChanges(true);
			undoManager->commit(Doc->Pages.at(ite->OwnPage)->getUName(), ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return ite->ItemNr;
}

/** Zeichnet eine Polyline */
int ScribusView::PaintPolyLine(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
	}
	PageItem* ite = new PageItem(Doc, PageItem::PolyLine, x, y, b, h, w, fill, outline);
	Doc->Items.append(ite);
	if (Doc->MasterP)
		Doc->MasterItems = Doc->Items;
	else
		Doc->DocItems = Doc->Items;
	ite->PLineArt = PenStyle(Doc->toolSettings.dLineArt);
	ite->setFillShade(Doc->toolSettings.dShade);
	ite->setLineShade(Doc->toolSettings.dShade2);
	ite->ItemNr = Doc->Items.count()-1;
	ite->ClipEdited = true;
	if (!Doc->loading)
	{
		ite->paintObj();
//		emit AddObj(ite);
	}
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(ite);
		UndoObject *target = Doc->Pages.at(0);
		if (ite->OwnPage > -1)
			target = target = Doc->Pages.at(ite->OwnPage);
		undoManager->action(target, is);
		if (!Mpressed)
		{
			ite->checkChanges(true);
			undoManager->commit(Doc->Pages.at(ite->OwnPage)->getUName(), ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return ite->ItemNr;
}

/** Zeichnet einen Textrahmen */
int ScribusView::PaintText(double x, double y, double b, double h, double w, QString outline)
{
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
	}
	PageItem* ite = new PageItem(Doc, PageItem::TextFrame, x, y, b, h, w, "None", outline);
	Doc->Items.append(ite);
	if (Doc->MasterP)
		Doc->MasterItems = Doc->Items;
	else
		Doc->DocItems = Doc->Items;
	ite->ItemNr = Doc->Items.count()-1;
	SetRectFrame(ite);
	ite->ContourLine = ite->PoLine.copy();
	if (!Doc->loading)
	{
		ite->paintObj();
//		emit AddObj(ite);
	}
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(ite);
		UndoObject *target = Doc->Pages.at(0);
		if (ite->OwnPage > -1)
			target = target = Doc->Pages.at(ite->OwnPage);
		undoManager->action(target, is);
		if (!Mpressed)
		{
			ite->checkChanges(true);
			undoManager->commit(Doc->Pages.at(ite->OwnPage)->getUName(), ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return ite->ItemNr;
}

/** Zeichnet eine Linie */
int ScribusView::PaintLine(double x, double y, double b, double h, double w, QString outline)
{
	if (w == 0)
		w = 1;
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
	}
	PageItem* ite = new PageItem(Doc, PageItem::Line, x, y, b, h, w, "None", outline);
	Doc->Items.append(ite);
	if (Doc->MasterP)
		Doc->MasterItems = Doc->Items;
	else
		Doc->DocItems = Doc->Items;
	ite->PLineArt = PenStyle(Doc->toolSettings.dLstyleLine);
	ite->setLineShade(Doc->toolSettings.dShadeLine);
	ite->ItemNr = Doc->Items.count()-1;
	if (!Doc->loading)
	{
		ite->paintObj();
//		emit AddObj(ite);
	}
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(ite);
		UndoObject *target = Doc->Pages.at(0);
		if (ite->OwnPage > -1)
			target = target = Doc->Pages.at(ite->OwnPage);
		undoManager->action(target, is);
		if (!Mpressed)
		{
			ite->checkChanges(true);
			undoManager->commit(Doc->Pages.at(ite->OwnPage)->getUName(), ite->getUPixmap(),
								Um::Create + " " + ite->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return ite->ItemNr;
}

void ScribusView::insertColor(QString nam, double c, double m, double y, double k)
{
	if (!Doc->PageColors.contains(nam))
	{
		CMYKColor tmp = CMYKColor(static_cast<int>(255 * c), static_cast<int>(255 * m),
		                          static_cast<int>(255 * y), static_cast<int>(255 * k));
		Doc->PageColors.insert(nam, tmp);
	}
}

void ScribusView::ChLineWidth(double w)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineWidth, "", Um::ILineStyle);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			PageItem *currItem = SelItem.at(a);
			currItem->OldPwidth = currItem->Pwidth;
			currItem->setLineWidth(w);
			if (currItem->itemType() == PageItem::PolyLine)
				SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
			if (currItem->itemType() == PageItem::Line)
			{
				int ph = static_cast<int>(QMAX(1.0, w / 2.0));
				currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->Width+ph),-ph,
				                  static_cast<int>(currItem->Width+ph),static_cast<int>(currItem->Height+ph),
				                  -ph,static_cast<int>(currItem->Height+ph));
			}
			RefreshItem(currItem);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ChLineArt(PenStyle w)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
						  Um::IGroup, Um::LineStyle, "", Um::ILineStyle);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->setLineStyle(w);
			RefreshItem(SelItem.at(a));
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ChLineJoin(PenJoinStyle w)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineJoin, "", Um::ILineStyle);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->setLineJoin(w);
			RefreshItem(SelItem.at(a));
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ChLineEnd(PenCapStyle w)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineEnd, "", Um::ILineStyle);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->setLineEnd(w);
			RefreshItem(SelItem.at(a));
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ChLineSpa(double w)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetLineSpacing, QString("%1").arg(w), Um::IFont);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->setLineSpacing(w);
			RefreshItem(SelItem.at(a));
		}
		Doc->docParagraphStyles[0].LineSpa = w;
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ChLocalXY(double x, double y)
{
	if (SelItem.count() != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			currItem->LocalX = x;
			currItem->LocalY = y;
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QWMatrix cl;
				cl.translate(currItem->LocalX*currItem->LocalScX, currItem->LocalY*currItem->LocalScY);
				cl.scale(currItem->LocalScX, currItem->LocalScY);
				currItem->imageClip.map(cl);
			}
			RefreshItem(currItem);
		}
	}
}

void ScribusView::ChLocalSc(double x, double y)
{
	if (SelItem.count() != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			currItem->LocalScX = x;
			currItem->LocalScY = y;
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QWMatrix cl;
				cl.translate(currItem->LocalX*currItem->LocalScX, currItem->LocalY*currItem->LocalScY);
				cl.scale(currItem->LocalScX, currItem->LocalScY);
				currItem->imageClip.map(cl);
			}
			RefreshItem(currItem);
		}
	}
}

void ScribusView::ItemFont(QString fon)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFont, fon, Um::IFont);
		for (uint aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *currItem = SelItem.at(aa);
			if (Doc->appMode == NormalMode)
			{
				currItem->setFont(fon);
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
						currItem->itemText.at(a)->cfont = (*Doc->AllFonts)[fon];
					if (currItem->itemType() == PageItem::PathText)
					{
						UpdatePolyClip(currItem);
						AdjustItemSize(currItem);
					}
/*					if (!Doc->loading)
						emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr); */
					RefreshItem(currItem);
				}
			}
			if ((currItem->HasSel) && (Doc->appMode == EditMode))
			{
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cselect)
							currItem->itemText.at(a)->cfont = (*Doc->AllFonts)[fon];
					}
					RefreshItem(currItem);
				}
			}
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemPen(QString farbe)
{
	if (farbe == tr("None"))
		farbe = "None";
	if (SelItem.count() != 0)
	{
		PageItem *i;
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetLineColor, farbe, Um::IFill);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			i = SelItem.at(a);
			if ((i->itemType() == PageItem::Line) && (farbe == "None"))
				continue;
			i->setLineColor(farbe);
			RefreshItem(i);
			emit ItemFarben(i->lineColor(), i->fillColor(), i->lineShade(), i->fillShade());
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemTextBrush(QString farbe)
{
	if (farbe == tr("None"))
		farbe = "None";
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontFill,
										  farbe, Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			{
				if (Doc->appMode != EditMode)
					currItem->setFontFillColor(farbe);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->ccolor = farbe;
					}
					else
						currItem->itemText.at(i)->ccolor = farbe;
				}
			}
			RefreshItem(currItem);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemTextBrushS(int sha)
{
	if (SelItem.count() != 0)
	{
		PageItem *currItem;
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontFillShade,
									  QString("%1").arg(sha), Um::IFont);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if (Doc->appMode != EditMode)
					currItem->setFontFillShade(sha);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cshade = sha;
					}
					else
						currItem->itemText.at(i)->cshade = sha;
				}
			}
			RefreshItem(currItem);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemTextPen(QString farbe)
{
	if (farbe == tr("None"))
		farbe = "None";
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontStroke,
										  farbe, Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			{
				if (Doc->appMode != EditMode)
					currItem->setFontStrokeColor(farbe);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cstroke = farbe;
					}
					else
						currItem->itemText.at(i)->cstroke = farbe;
				}
			}
			RefreshItem(currItem);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemTextPenS(int sha)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontStrokeShade,
									  QString("%1").arg(sha), Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if (Doc->appMode != EditMode)
					currItem->setFontStrokeShade(sha);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cshade2 = sha;
					}
					else
						currItem->itemText.at(i)->cshade2 = sha;
				}
			}
			RefreshItem(currItem);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemTextScale(int sha)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontWidth,
										  QString("%1").arg(sha), Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			{
				if (Doc->appMode != EditMode)
					currItem->setFontWidth(sha);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cscale = sha;
					}
					else
						currItem->itemText.at(i)->cscale = sha;
				}
			}
			RefreshItem(currItem);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemBrush(QString farbe)
{
	if (farbe == tr("None"))
		farbe = "None";

	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetFill, farbe, Um::IFill);
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			currItem->setFillColor(farbe);
			RefreshItem(currItem);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemBrushShade(int sha)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetShade, QString("%1").arg(sha),
										  Um::IShade);
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			currItem->setFillShade(sha);
			emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
			RefreshItem(currItem);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemPenShade(int sha)
{
	if (SelItem.count() != 0)
	{
		PageItem *currItem;
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
							Um::IGroup, Um::SetLineShade, QString("%1").arg(sha), Um::IShade);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			currItem->setLineShade(sha);
			RefreshItem(currItem);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemGradFill(int typ)
{
	if (SelItem.count() != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			currItem = SelItem.at(a);
			currItem->GrType = typ;
			updateGradientVectors(currItem);
			RefreshItem(currItem);
		}
	}
}

void ScribusView::chTyStyle(int s)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontEffect, "", Um::IFont);
		for (uint aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *currItem = SelItem.at(aa);
			if (Doc->appMode != EditMode)
				currItem->setFontEffects(s);
			if (currItem->itemText.count() != 0)
			{
				if (Doc->appMode == EditMode)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cselect)
						{
							currItem->itemText.at(a)->cstyle &= ~127;
							currItem->itemText.at(a)->cstyle |= s;
						}
					}
				}
				else
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						currItem->itemText.at(a)->cstyle &= ~127;
						currItem->itemText.at(a)->cstyle |= s;
					}
				}
				RefreshItem(currItem);
			}
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::SetAbStyle(int s)
{
	PageItem *currItem;
	if (GetItem(&currItem))
		chAbStyle(currItem, s);
}

void ScribusView::chAbStyle(PageItem *currItem, int s)
{
	int a, ax;
	PageItem *nextItem;
	bool cr = true;
	if (Doc->appMode == EditMode)
	{
		nextItem = currItem;
		a = currItem->CPos;
		if (a == static_cast<int>(nextItem->itemText.count()))
			a -= 1;
		while ((cr) && (nextItem != 0))
		{
			if (nextItem->itemText.count() == 0)
			{
				nextItem = nextItem->BackBox;
				if (nextItem == 0)
				{
					cr = false;
					break;
				}
				a = static_cast<int>(nextItem->itemText.count()-1);
			}
			ax = a;
			for (int xx=0; xx<ax+1; ++xx)
			{
				if (nextItem->itemText.at(a)->ch == QChar(13))
				{
					cr = false;
					break;
				}
				if (s > 4)
				{
					if (Doc->docParagraphStyles[s].Font != "")
					{
						nextItem->itemText.at(a)->cfont = (*Doc->AllFonts)[Doc->docParagraphStyles[s].Font];
						nextItem->itemText.at(a)->csize = Doc->docParagraphStyles[s].FontSize;
						nextItem->itemText.at(a)->cstyle &= ~127;
						nextItem->itemText.at(a)->cstyle |= Doc->docParagraphStyles[s].FontEffect;
						nextItem->itemText.at(a)->ccolor = Doc->docParagraphStyles[s].FColor;
						nextItem->itemText.at(a)->cshade = Doc->docParagraphStyles[s].FShade;
						nextItem->itemText.at(a)->cstroke = Doc->docParagraphStyles[s].SColor;
						nextItem->itemText.at(a)->cshade2 = Doc->docParagraphStyles[s].SShade;
					}
				}
				if ((s < 5) && (nextItem->itemText.at(a)->cab > 4))
				{
					nextItem->itemText.at(a)->ccolor = nextItem->TxtFill;
					nextItem->itemText.at(a)->cshade = nextItem->ShTxtFill;
					nextItem->itemText.at(a)->cstroke = nextItem->TxtStroke;
					nextItem->itemText.at(a)->cshade2 = nextItem->ShTxtStroke;
					nextItem->itemText.at(a)->csize = nextItem->ISize;
					nextItem->itemText.at(a)->cfont = (*Doc->AllFonts)[nextItem->IFont];
					nextItem->itemText.at(a)->cstyle &= ~127;
					nextItem->itemText.at(a)->cstyle |= nextItem->TxTStyle;
				}
				nextItem->itemText.at(a)->cab = s;
				a--;
			}
			if (cr)
			{
				nextItem = nextItem->BackBox;
				if (nextItem != 0)
					a = static_cast<int>(nextItem->itemText.count()-1);
			}
		}
		a = currItem->CPos;
		cr = true;
		nextItem = currItem;
		if (a == static_cast<int>(nextItem->itemText.count()))
			cr = false;
		while ((cr) && (nextItem != 0))
		{
			while (a < static_cast<int>(nextItem->itemText.count()))
			{
				if (s > 4)
				{
					if (Doc->docParagraphStyles[s].Font != "")
					{
						nextItem->itemText.at(a)->cfont = (*Doc->AllFonts)[Doc->docParagraphStyles[s].Font];
						nextItem->itemText.at(a)->csize = Doc->docParagraphStyles[s].FontSize;
						nextItem->itemText.at(a)->cstyle &= ~127;
						nextItem->itemText.at(a)->cstyle |= Doc->docParagraphStyles[s].FontEffect;
						nextItem->itemText.at(a)->ccolor = Doc->docParagraphStyles[s].FColor;
						nextItem->itemText.at(a)->cshade = Doc->docParagraphStyles[s].FShade;
						nextItem->itemText.at(a)->cstroke = Doc->docParagraphStyles[s].SColor;
						nextItem->itemText.at(a)->cshade2 = Doc->docParagraphStyles[s].SShade;
					}
				}
				if ((s < 5) && (nextItem->itemText.at(a)->cab > 4))
				{
					nextItem->itemText.at(a)->ccolor = nextItem->TxtFill;
					nextItem->itemText.at(a)->cshade = nextItem->ShTxtFill;
					nextItem->itemText.at(a)->cstroke = nextItem->TxtStroke;
					nextItem->itemText.at(a)->cshade2 = nextItem->ShTxtStroke;
					nextItem->itemText.at(a)->csize = nextItem->ISize;
					nextItem->itemText.at(a)->cfont = (*Doc->AllFonts)[nextItem->IFont];
					nextItem->itemText.at(a)->cstyle &= ~127;
					nextItem->itemText.at(a)->cstyle |= nextItem->TxTStyle;
				}
				nextItem->itemText.at(a)->cab = s;
				if ((nextItem->itemText.at(a)->ch == QChar(13)) && (!nextItem->itemText.at(a)->cselect))
				{
					cr = false;
					break;
				}
				a++;
			}
			if (cr)
			{
				nextItem = nextItem->NextBox;
				a = 0;
			}
		}
	}
	else
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(
					s > 4 ? Um::SetStyle : Um::AlignText, "", Um::IFont);
			ss->set("PSTYLE", "pstyle");
			ss->set("OLD_STYLE", currItem->textAlignment);
			ss->set("NEW_STYLE", s);
			undoManager->action(currItem, ss);
		}
		currItem->textAlignment = s;
		if (currItem->itemText.count() != 0)
		{
			for (a = 0; a < static_cast<int>(currItem->itemText.count()); ++a)
			{
				if (s > 4)
				{
					if (Doc->docParagraphStyles[s].Font != "")
					{
						currItem->itemText.at(a)->cfont = (*Doc->AllFonts)[Doc->docParagraphStyles[s].Font];
						currItem->itemText.at(a)->csize = Doc->docParagraphStyles[s].FontSize;
						currItem->itemText.at(a)->cstyle &= ~127;
						currItem->itemText.at(a)->cstyle |= Doc->docParagraphStyles[s].FontEffect;
						currItem->itemText.at(a)->ccolor = Doc->docParagraphStyles[s].FColor;
						currItem->itemText.at(a)->cshade = Doc->docParagraphStyles[s].FShade;
						currItem->itemText.at(a)->cstroke = Doc->docParagraphStyles[s].SColor;
						currItem->itemText.at(a)->cshade2 = Doc->docParagraphStyles[s].SShade;
					}
				}
				if ((s < 5) && (currItem->itemText.at(a)->cab > 4))
				{
					currItem->itemText.at(a)->ccolor = currItem->TxtFill;
					currItem->itemText.at(a)->cshade = currItem->ShTxtFill;
					currItem->itemText.at(a)->cstroke = currItem->TxtStroke;
					currItem->itemText.at(a)->cshade2 = currItem->ShTxtStroke;
					currItem->itemText.at(a)->cfont = (*Doc->AllFonts)[currItem->IFont];
					currItem->itemText.at(a)->csize = currItem->ISize;
					currItem->itemText.at(a)->cstyle &= ~127;
					currItem->itemText.at(a)->cstyle |= currItem->TxTStyle;
				}
				currItem->itemText.at(a)->cab = s;
			}
		}
	}
	if (!currItem->Tinput)
		RefreshItem(currItem);
}

void ScribusView::chKerning(double us)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetKerning,
										  QString("%1").arg(us), Um::IFont);
		for (uint aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *currItem = SelItem.at(aa);
			if ((currItem->HasSel) && (Doc->appMode == EditMode))
			{
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cselect)
							currItem->itemText.at(a)->cextra = us;
					}
					RefreshItem(currItem);
				}
			}
			else
			{
				if (Doc->appMode != EditMode)
					currItem->setKerning(us);
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						currItem->itemText.at(a)->cextra = us;
					}
					RefreshItem(currItem);
				}
			}
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::chFSize(int size)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontSize,
										  QString("%1").arg(size/10.0), Um::IFont);
		for (uint aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *currItem = SelItem.at(0);
			Doc->CurrFontSize = size;
			if (Doc->appMode != EditMode)
			{
				currItem->LineSp = ((size / 10.0) * static_cast<double>(Doc->typographicSetttings.autoLineSpacing) / 100) + (size / 10.0);
				Doc->docParagraphStyles[0].LineSpa = currItem->LineSp;
				emit ItemTextAttr(currItem->LineSp);
				currItem->setFontSize(size);
				emit ItemTextSize(currItem->ISize);
				emit ItemTextCols(currItem->Cols, currItem->ColGap);
			}
			if (currItem->itemText.count() != 0)
			{
				if (Doc->appMode == EditMode)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cselect)
							currItem->itemText.at(a)->csize = size;
					}
				}
				else
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cab < 5)
							currItem->itemText.at(a)->csize = size;
					}
				}
				if (currItem->itemType() == PageItem::PathText)
				{
					UpdatePolyClip(currItem);
					AdjustItemSize(currItem);
				}
				RefreshItem(currItem);
			}
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::FlipImageH()
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
										  Um::FlipH, 0, Um::IFlipH);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->flipImageH();
			RefreshItem(SelItem.at(a));
		}
		emit DocChanged();
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::FlipImageV()
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
										  Um::FlipV, 0, Um::IFlipV);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->flipImageV();
			RefreshItem(SelItem.at(a));
		}
		emit DocChanged();
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::useEmbeddedPath()
{
	if (SelItem.count() != 0)
	{
		PageItem *Item = SelItem.at(0);
		if (Item->pixm.imgInfo.valid)
		{
			ExtImageProps* dia = new ExtImageProps(this, &Item->pixm.imgInfo, Item, this);
			dia->exec();
			delete dia;
		}
	}
}

void ScribusView::LoadPict(QString fn, int ItNr, bool reload)
{
	loadPict(fn, Doc->Items.at(ItNr), reload);
}

void ScribusView::loadPict(QString fn, PageItem *pageItem, bool reload)
{
	bool dummy;
	QFileInfo fi = QFileInfo(fn);
	PageItem *Item = pageItem;
	QString clPath = Item->pixm.imgInfo.usedPath;
	Item->pixm.imgInfo.valid = false;
	Item->pixm.imgInfo.clipPath = "";
	Item->pixm.imgInfo.PDSpathData.clear();
	Item->pixm.imgInfo.layerInfo.clear();
	Item->pixm.imgInfo.usedPath = "";
	Item->imageClip.resize(0);
	if (!reload)
	{
		if ((ScApp->fileWatcher->files().contains(Item->Pfile) != 0) && (Item->PicAvail))
			ScApp->fileWatcher->removeFile(Item->Pfile);
	}
	if (!Item->pixm.LoadPicture(fn, Item->IProfile, Item->IRender, Item->UseEmbedded, true, 2, 72, &dummy))
	{
		Item->Pfile = fi.absFilePath();
		Item->PicAvail = false;
		Item->PicArt = false;
	}
	else
	{
		if (UndoManager::undoEnabled() && !reload)
		{
			SimpleState *ss = new SimpleState(Um::GetImage, fn, Um::IGetImage);
			ss->set("GET_IMAGE", "get_image");
			ss->set("OLD_IMAGE_PATH", Item->Pfile);
			ss->set("NEW_IMAGE_PATH", fn);
			undoManager->action(Item, ss);
		}
		double xres = Item->pixm.imgInfo.xres;
		double yres = Item->pixm.imgInfo.yres;
		Item->PicAvail = true;
		Item->PicArt = true;
		Item->BBoxX = 0;
		if (Item->Pfile != fn)
		{
			Item->LocalScX = 72.0 / xres;
			Item->LocalScY = 72.0 / yres;
			Item->LocalX = 0;
			Item->LocalY = 0;
			if ((Doc->toolSettings.useEmbeddedPath) && (Item->pixm.imgInfo.clipPath != ""))
			{
				Item->pixm.imgInfo.usedPath = Item->pixm.imgInfo.clipPath;
				clPath = Item->pixm.imgInfo.clipPath;
				if (Item->pixm.imgInfo.PDSpathData.contains(clPath))
				{
					Item->imageClip = Item->pixm.imgInfo.PDSpathData[clPath].copy();
					Item->pixm.imgInfo.usedPath = clPath;
					QWMatrix cl;
					cl.translate(Item->LocalX*Item->LocalScX, Item->LocalY*Item->LocalScY);
					cl.scale(Item->LocalScX, Item->LocalScY);
					Item->imageClip.map(cl);
				}
			}
		}
		Item->Pfile = fi.absFilePath();
		if (!reload)
			ScApp->fileWatcher->addFile(Item->Pfile);
		else
		{
			if (Item->pixm.imgInfo.PDSpathData.contains(clPath))
			{
				Item->imageClip = Item->pixm.imgInfo.PDSpathData[clPath].copy();
				Item->pixm.imgInfo.usedPath = clPath;
				QWMatrix cl;
				cl.translate(Item->LocalX*Item->LocalScX, Item->LocalY*Item->LocalScY);
				cl.scale(Item->LocalScX, Item->LocalScY);
				Item->imageClip.map(cl);
			}
		}
		Item->BBoxH = Item->pixm.height();
		Item->OrigW = Item->pixm.width();
		Item->OrigH = Item->pixm.height();
		QString ext = fi.extension(false).lower();
		if ((ext == "pdf") || (ext == "ps") || (ext == "eps"))
			Item->isRaster = false;
		else
			Item->isRaster = true;
		if (Item->pixm.imgInfo.profileName != "")
		{
			Item->IProfile = "Embedded " + Item->pixm.imgInfo.profileName;
			Item->EmProfile = "Embedded " + Item->pixm.imgInfo.profileName;
		}
	}
	if (Item->PicAvail)
	{
		if (Item->pixm.imgInfo.lowResType != 0)
		{
			double scaling = 1.0;
			if (Item->pixm.imgInfo.lowResType == 1)
				scaling = Item->pixm.imgInfo.xres / 72.0;
			else
				scaling = Item->pixm.imgInfo.xres / 36.0;
			Item->pixm.createLowRes(scaling);
			Item->pixm.imgInfo.lowResScale = scaling;
		}
		Item->pixm.applyEffect(Item->effectsInUse, Doc->PageColors, false);
	}
	if (!Doc->loading)
	{
		emit RasterPic(Item->isRaster);
//		emit UpdtObj(PageNr, ItNr);
		emit DocChanged();
	}
}

void ScribusView::changePreview(int id)
{
	int d = pmenResolution->indexOf(id);
	if (SelItem.count() != 0)
	{
		PageItem *Item = SelItem.at(0);
		Item->pixm.imgInfo.lowResType = 2 - d;
		UpdatePic();
	}
}

void ScribusView::AdjustPictScale(PageItem *currItem, bool )
{
	if (currItem->ScaleType)
		return;
	if ((currItem->OrigW == 0) || (currItem->OrigH == 0))
		return;
	currItem->LocalX = 0;
	currItem->LocalY = 0;
	double xs = currItem->Width / static_cast<double>(currItem->OrigW);
	double ys = currItem->Height / static_cast<double>(currItem->OrigH);
	if (currItem->AspectRatio)
	{
		currItem->LocalScX = QMIN(xs, ys);
		currItem->LocalScY = QMIN(xs, ys);
	}
	else
	{
		currItem->LocalScX = xs;
		currItem->LocalScY = ys;
	}
	if (currItem->imageClip.size() != 0)
	{
		currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
		QWMatrix cl;
		cl.translate(currItem->LocalX*currItem->LocalScX, currItem->LocalY*currItem->LocalScY);
		cl.scale(currItem->LocalScX, currItem->LocalScY);
		currItem->imageClip.map(cl);
	}
	emit SetLocalValues(currItem->LocalScX, currItem->LocalScY, currItem->LocalX, currItem->LocalY );
}

void ScribusView::TogglePic()
{
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->PicArt = !SelItem.at(a)->PicArt;
			RefreshItem(SelItem.at(a));
		}
		emit DocChanged();
	}
}

void ScribusView::updatePict(QString name)
{
	for (uint a = 0; a < Doc->DocItems.count(); ++a)
	{
		PageItem *currItem = Doc->DocItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			AdjustPictScale(currItem);
		}
	}
	for (uint a = 0; a < Doc->MasterItems.count(); ++a)
	{
		PageItem *currItem = Doc->MasterItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			AdjustPictScale(currItem);
		}
	}
	updateContents();
	emit DocChanged();
}

void ScribusView::RecalcPicturesRes()
{
	for (uint a = 0; a < Doc->DocItems.count(); ++a)
	{
		PageItem *currItem = Doc->DocItems.at(a);
		if (currItem->PicAvail)
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			AdjustPictScale(currItem);
		}
	}
	for (uint a = 0; a < Doc->MasterItems.count(); ++a)
	{
		PageItem *currItem = Doc->MasterItems.at(a);
		if (currItem->PicAvail)
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			AdjustPictScale(currItem);
		}
	}
	updateContents();
	emit DocChanged();
}

void ScribusView::removePict(QString name)
{
	for (uint a = 0; a < Doc->DocItems.count(); ++a)
	{
		PageItem *currItem = Doc->DocItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = QImage();
/*			if (currItem->itemType() == PageItem::ImageFrame)
				emit UpdtObj(Doc->currentPage->PageNr, currItem->ItemNr); */
		}
	}
	for (uint a = 0; a < Doc->MasterItems.count(); ++a)
	{
		PageItem *currItem = Doc->MasterItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = QImage();
		}
	}
	updateContents();
	emit DocChanged();
}

void ScribusView::UpdatePic()
{
	if (SelItem.count() != 0)
	{
		PageItem *currItem = SelItem.at(0);
		if (currItem->PicAvail)
		{
			int fho = currItem->imageFlippedH();
			int fvo = currItem->imageFlippedV();
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			AdjustPictScale(currItem);
			updateContents();
		}
	}
}

void ScribusView::FrameToPic()
{
	if (SelItem.count() != 0)
	{
		PageItem *currItem = SelItem.at(0);
		if (currItem->PicAvail)
		{
			double w = static_cast<double>(currItem->pixm.width())*currItem->pixm.imgInfo.lowResScale * currItem->LocalScX;
			double h = static_cast<double>(currItem->pixm.height())*currItem->pixm.imgInfo.lowResScale * currItem->LocalScY;
			double x = currItem->LocalX * currItem->LocalScX;
			double y = currItem->LocalY * currItem->LocalScY;
			if (!currItem->isTableItem)
			{
				SizeItem(w, h, currItem->ItemNr);
				MoveItem(x, y, currItem);
				currItem->LocalX = 0;
				currItem->LocalY = 0;
			}
			updateContents();
			emit DocChanged();
		}
	}
}

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
	struct ScText *hg;
	switch (Buffer->PType)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::FrameType1:
		z = PaintEllipse(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	//
	case PageItem::ImageFrame:
		z = PaintPict(x, y, w, h);
		Doc->Items.at(z)->LocalScX = Buffer->LocalScX;
		Doc->Items.at(z)->LocalScY = Buffer->LocalScY;
		Doc->Items.at(z)->LocalX = Buffer->LocalX;
		Doc->Items.at(z)->LocalY = Buffer->LocalY;
		Doc->Items.at(z)->Pfile = Buffer->Pfile;
		Doc->Items.at(z)->IProfile = Buffer->IProfile;
		Doc->Items.at(z)->EmProfile = Buffer->EmProfile;
		Doc->Items.at(z)->IRender = Buffer->IRender;
		Doc->Items.at(z)->UseEmbedded = Buffer->UseEmbedded;
		if (Doc->Items.at(z)->Pfile != "")
			LoadPict(Doc->Items.at(z)->Pfile, z);
		Doc->Items.at(z)->LocalScX = Buffer->LocalScX;
		Doc->Items.at(z)->LocalScY = Buffer->LocalScY;
		Doc->Items.at(z)->PicArt = Buffer->PicArt;
		Doc->Items.at(z)->BBoxX = Buffer->BBoxX;
		Doc->Items.at(z)->BBoxH = Buffer->BBoxH;
		Doc->Items.at(z)->ScaleType = Buffer->ScaleType;
		Doc->Items.at(z)->AspectRatio = Buffer->AspectRatio;
		Doc->Items.at(z)->Pwidth = Buffer->Pwidth;
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::FrameType3:
		z = PaintRect(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	//
	case PageItem::PathText:
	case PageItem::TextFrame:
		z = PaintText(x, y, w, h, pw, Buffer->Pcolor);
		if ((Buffer->isAnnotation) && (Buffer->AnUseIcons))
		{
			Doc->Items.at(z)->LocalScX = Buffer->LocalScX;
			Doc->Items.at(z)->LocalScY = Buffer->LocalScY;
			Doc->Items.at(z)->LocalX = Buffer->LocalX;
			Doc->Items.at(z)->LocalY = Buffer->LocalY;
			Doc->Items.at(z)->Pfile = Buffer->Pfile;
			Doc->Items.at(z)->Pfile2 = Buffer->Pfile2;
			Doc->Items.at(z)->Pfile3 = Buffer->Pfile3;
			Doc->Items.at(z)->IProfile = Buffer->IProfile;
			Doc->Items.at(z)->EmProfile = Buffer->EmProfile;
			Doc->Items.at(z)->IRender = Buffer->IRender;
			Doc->Items.at(z)->UseEmbedded = Buffer->UseEmbedded;
			LoadPict(Doc->Items.at(z)->Pfile, z);
			Doc->Items.at(z)->LocalScX = Buffer->LocalScX;
			Doc->Items.at(z)->LocalScY = Buffer->LocalScY;
			Doc->Items.at(z)->PicArt = Buffer->PicArt;
			Doc->Items.at(z)->BBoxX = Buffer->BBoxX;
			Doc->Items.at(z)->BBoxH = Buffer->BBoxH;
		}
		if (Buffer->itemText != "")
		{
			QTextStream t(&Buffer->itemText, IO_ReadOnly);
			QString cc;
			while (!t.atEnd())
			{
				cc = t.readLine();
				if (cc == "")
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
				hg->cextra = (*it).toDouble();
				it++;
				hg->cshade = (*it).toInt();
				hg->cselect = false;
				it++;
				hg->cstyle = it == NULL ? 0 : (*it).toInt();
				it++;
				hg->cab = it == NULL ? 0 : (*it).toInt();
				it++;
				hg->cstroke = it == NULL ? QString("None") : *it;
				it++;
				hg->cshade2 = it == NULL ? 100 : (*it).toInt();
				it++;
				if (it == NULL)
					hg->cscale = 100;
				else
					hg->cscale = QMIN(QMAX((*it).toInt(), 25), 400);
				hg->xp = 0;
				hg->yp = 0;
				hg->PRot = 0;
				hg->PtransX = 0;
				hg->PtransY = 0;
				Doc->Items.at(z)->itemText.append(hg);
			}
		}
		Doc->Items.at(z)->LineSp = Buffer->LineSp;
		Doc->Items.at(z)->convertTo(Buffer->PType);
		break;
	case PageItem::Line:
		z = PaintLine(x, y, w, 0, pw, Buffer->Pcolor2);
		break;
	case PageItem::Polygon:
		z = PaintPoly(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	case PageItem::PolyLine:
		z = PaintPolyLine(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	}
	PageItem *currItem = Doc->Items.at(z);
	currItem->setImageFlippedH(Buffer->flippedH);
	currItem->setImageFlippedV(Buffer->flippedV);
	currItem->RadRect = Buffer->RadRect;
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
	currItem->TxTStyle = Buffer->TxTStyle;
	currItem->Rot = Buffer->Rot;
	currItem->Extra = Buffer->Extra;
	currItem->TExtra = Buffer->TExtra;
	currItem->BExtra = Buffer->BExtra;
	currItem->RExtra = Buffer->RExtra;
	currItem->PLineArt = PenStyle(Buffer->PLineArt);
	currItem->PLineEnd = PenCapStyle(Buffer->PLineEnd);
	currItem->PLineJoin = PenJoinStyle(Buffer->PLineJoin);
	currItem->isPrintable = Buffer->isPrintable;
	currItem->isBookmark = Buffer->isBookmark;
	currItem->BMnr = Buffer->BMnr;
	currItem->isAnnotation = Buffer->isAnnotation;
	currItem->AnType = Buffer->AnType;
	currItem->AnAction = Buffer->AnAction;
	currItem->An_E_act = Buffer->An_E_act;
	currItem->An_X_act = Buffer->An_X_act;
	currItem->An_D_act = Buffer->An_D_act;
	currItem->An_Fo_act = Buffer->An_Fo_act;
	currItem->An_Bl_act = Buffer->An_Bl_act;
	currItem->An_K_act = Buffer->An_K_act;
	currItem->An_F_act = Buffer->An_F_act;
	currItem->An_V_act = Buffer->An_V_act;
	currItem->An_C_act = Buffer->An_C_act;
	currItem->An_Extern = Buffer->An_Extern;
	currItem->AnZiel = Buffer->AnZiel;
	currItem->AnActType = Buffer->AnActType;
	currItem->TopLine = Buffer->TopLine;
	currItem->RightLine = Buffer->RightLine;
	currItem->LeftLine = Buffer->LeftLine;
	currItem->BottomLine = Buffer->BottomLine;
	currItem->isTableItem = Buffer->isTableItem;
	currItem->TopLinkID = Buffer->TopLinkID;
	currItem->LeftLinkID = Buffer->LeftLinkID;
	currItem->RightLinkID = Buffer->RightLinkID;
	currItem->BottomLinkID = Buffer->BottomLinkID;
	currItem->startArrowIndex = Buffer->startArrowIndex;
	currItem->endArrowIndex = Buffer->endArrowIndex;
	if (Buffer->AnName != "")
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
	currItem->AnToolTip = Buffer->AnToolTip;
	currItem->AnBwid = Buffer->AnBwid;
	currItem->AnBsty = Buffer->AnBsty;
	currItem->AnFeed = Buffer->AnFeed;
	currItem->AnVis = Buffer->AnVis;
	currItem->AnFlag = Buffer->AnFlag;
	currItem->AnFont = Buffer->AnFont;
	currItem->AnFormat = Buffer->AnFormat;
	currItem->AnMaxChar = Buffer->AnMaxChar;
	currItem->AnRollOver = Buffer->AnRollOver;
	currItem->AnDown = Buffer->AnDown;
	currItem->AnIsChk = Buffer->AnIsChk;
	currItem->AnAAact = Buffer->AnAAact;
	currItem->AnChkStil = Buffer->AnChkStil;
	currItem->AnHTML = Buffer->AnHTML;
	currItem->AnUseIcons = Buffer->AnUseIcons;
	currItem->AnIPlace = Buffer->AnIPlace;
	currItem->AnScaleW = Buffer->AnScaleW;
	currItem->AnBColor = Buffer->AnBColor;
	currItem->Clip = Buffer->Clip.copy();
	currItem->PoShow = Buffer->PoShow;
	currItem->BaseOffs = Buffer->BaseOffs;
	currItem->setTextFlowsAroundFrame(Buffer->Textflow);
	currItem->setTextFlowUsesBoundingBox(Buffer->Textflow2);
	currItem->textAlignment = Buffer->textAlignment;
	currItem->IFont = Buffer->IFont;
	currItem->ISize = Buffer->ISize;
	currItem->ExtraV = Buffer->ExtraV;
	currItem->Groups = Buffer->Groups;
	currItem->TabValues = Buffer->TabValues;
	currItem->DashValues = Buffer->DashValues;
	currItem->DashOffset = Buffer->DashOffset;
	currItem->setLocked(Buffer->Locked);
	currItem->setSizeLocked(Buffer->LockRes);
	currItem->setFillTransparency(Buffer->Transparency);
	currItem->setLineTransparency(Buffer->TranspStroke);
	currItem->Reverse = Buffer->Reverse;
	currItem->NamedLStyle = Buffer->NamedLStyle;
	currItem->Language = ScApp->GetLang(Buffer->Language);
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
	if (currItem->itemType() != PageItem::Line)
	{
		// OBSOLETE CR 2005-02-06
		if ((currItem->PoLine.size() == 0) && (currItem->itemType() != PageItem::FrameType1))
			ConvertClip(currItem);
		else
			//
			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	}
	else
	{
		int ph = static_cast<int>(QMAX(1.0, currItem->Pwidth / 2.0));
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->Width+ph),-ph,
		                  static_cast<int>(currItem->Width+ph),static_cast<int>(currItem->Height+ph),
		                  -ph,static_cast<int>(currItem->Height+ph));
		currItem->Height = 1;
	}
	// OBSOLETE CR 2005-02-06
	if (currItem->itemType() == PageItem::FrameType1)
		SetOvalFrame(currItem);
	// OBSOLETE CR 2005-02-06
	if (currItem->itemType() == PageItem::FrameType3)
	{
		if (currItem->RadRect != 0)
			SetFrameRound(currItem);
		else
			SetRectFrame(currItem);
		currItem->ClipEdited = true;
	}
	if (currItem->itemType() == PageItem::ImageFrame)
		AdjustPictScale(currItem);
	if ((currItem->itemType() != PageItem::TextFrame) && (currItem->itemType() != PageItem::PathText))
		currItem->IFont = Doc->toolSettings.defFont;
	if (Buffer->GrType != 0)
	{
		if ((Buffer->GrColor != "") && (Buffer->GrColor2 != ""))
		{
			currItem->fill_gradient.clearStops();
			if (Buffer->GrType == 5)
			{
				if ((Buffer->GrColor != "None") && (Buffer->GrColor != ""))
					currItem->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
				currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
				if ((Buffer->GrColor2 != "None") && (Buffer->GrColor2 != ""))
					currItem->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
				currItem->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
			}
			else
			{
				if ((Buffer->GrColor2 != "None") && (Buffer->GrColor2 != ""))
					currItem->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
				currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
				if ((Buffer->GrColor != "None") && (Buffer->GrColor != ""))
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
		updateGradientVectors(currItem);
	}
	currItem->setObjectAttributes(&(Buffer->pageItemAttributes));
	setRedrawBounding(currItem);
	currItem->OwnPage = OnPage(currItem);
	if (!loading)
	{
		currItem->Select = true;
		SelItem.append(currItem);
		emit HaveSel(currItem->itemType());
		EmitValues(currItem);
		emit DocChanged();
		updateContents();
	}
/*	if ((currItem->isBookmark) && (!loading))
		emit NewBMNr(currItem->BMnr, z); */
}

void ScribusView::BuildAObj()
{
	PageItem *currItem;
	int ObjGroup;
	struct AlignObjs Object;
	AObjects.clear();
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		currItem = SelItem.at(a);
		Object.Objects.clear();
		getBoundingRect(currItem, &Object.x1, &Object.y1, &Object.x2, &Object.y2);
		if (currItem->Groups.count() > 0)
		{
			ObjGroup = currItem->Groups.top();
			bool found = false;
			for (uint a2 = 0; a2 < AObjects.count(); ++a2)
			{
				if (AObjects[a2].Group == ObjGroup)
				{
					AObjects[a2].x1 = QMIN(AObjects[a2].x1, Object.x1);
					AObjects[a2].y1 = QMIN(AObjects[a2].y1, Object.y1);
					AObjects[a2].x2 = QMAX(AObjects[a2].x2, Object.x2);
					AObjects[a2].y2 = QMAX(AObjects[a2].y2, Object.y2);
					AObjects[a2].Objects.append(currItem);
					found = true;
					break;
				}
			}
			if (!found)
			{
				Object.Group = ObjGroup;
				Object.ObjNr = 0;
				Object.Objects.append(currItem);
				AObjects.append(Object);
			}
		}
		else
		{
			Object.Group = 0;
			Object.ObjNr = currItem->ItemNr;
			Object.Objects.append(currItem);
			AObjects.append(Object);
		}
	}
}

void ScribusView::doAlign(QValueList<uint> &Object, int moveCode, double xp, double xdisp, double ydisp, double minx)
{
	double xd = 0;
	uint start, end;
	if ((moveCode == 3) || (moveCode == 13))
	{
		start = 1;
		end = Object.count()-1;
		if (Object.count() < 3)
			return;
	}
	else
	{
		start = 0;
		end = Object.count();
	}
	for (uint a2 = start; a2 < end; ++a2)
	{
		switch (moveCode)
		{
		case 0:
			xd = (xp - AObjects[Object[a2]].x1) + a2*xdisp;
			break;
		case 1:
			xd = (xp - (AObjects[Object[a2]].x1 + (AObjects[Object[a2]].x2 - AObjects[Object[a2]].x1) / 2.0)) + a2*xdisp;
			break;
		case 2:
			xd = (xp - AObjects[Object[a2]].x2) + a2*xdisp;
			break;
		case 3:
			xd = ((a2 * xp + minx) - ((AObjects[Object[a2]].x2 - AObjects[Object[a2]].x1) / 2.0)) - AObjects[Object[a2]].x1;
			break;
		case 10:
			xd = (xp - AObjects[Object[a2]].y1) + a2*ydisp;
			break;
		case 11:
			xd = (xp - (AObjects[Object[a2]].y1 + (AObjects[Object[a2]].y2 - AObjects[Object[a2]].y1) / 2.0)) + a2*ydisp;
			break;
		case 12:
			xd = (xp - AObjects[Object[a2]].y2) + a2*ydisp;
			break;
		case 13:
			xd = ((a2 * xp + minx) - ((AObjects[Object[a2]].y2 - AObjects[Object[a2]].y1) / 2.0)) - AObjects[Object[a2]].y1;
			break;
		}
		for (uint a3 = 0; a3 < AObjects[Object[a2]].Objects.count(); ++a3)
		{
			if ((moveCode == 0) || (moveCode == 1) || (moveCode == 2) || (moveCode == 3))
				AObjects[Object[a2]].Objects.at(a3)->Xpos += xd;
			else
				AObjects[Object[a2]].Objects.at(a3)->Ypos += xd;
		}
	}
}

void ScribusView::AlignObj(bool xa, bool ya, bool Vth, bool Vtv, double xdisp, double ydisp, int xart, int yart)
{
	double xp = 99999.9;
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	int moveCode = 0;
	QMap<double,uint> Xsorted;
	QMap<double,uint> Ysorted;
	BuildAObj();
	for (uint a = 0; a < AObjects.count(); ++a)
	{
		Ysorted.insert(qRound(AObjects[a].y1)*1000000+qRound(AObjects[a].x1*100)+a, a, false);
		Xsorted.insert(qRound(AObjects[a].x1)*1000000+qRound(AObjects[a].y1*100)+a, a, false);
	}
	QValueList<uint> Yindex = Ysorted.values();
	QValueList<uint> Xindex = Xsorted.values();
	if (xa)
	{
		if (Vth)
		{
			if (AObjects.count() < 3)
				return;
			for (uint a = 0; a < AObjects.count(); ++a)
			{
				minx = QMIN(AObjects[a].x1, minx);
				maxx = QMAX(AObjects[a].x2, maxx);
			}
			xp = (maxx - minx) / (AObjects.count() - 1);
			if (Xindex.count() == 1)
				doAlign(Yindex, 3, xp, xdisp, ydisp, minx);
			else
				doAlign(Xindex, 3, xp, xdisp, ydisp, minx);
		}
		else
		{
			switch (xart)
			{
				case 0:
					for (uint a = 0; a < AObjects.count(); ++a)
					{
						xp = QMIN(AObjects[a].x1, xp);
					}
					moveCode = 0;
					break;
				case 1:
					for (uint a = 0; a < AObjects.count(); ++a)
					{
						minx = QMIN(AObjects[a].x1, minx);
						maxx = QMAX(AObjects[a].x2, maxx);
					}
					xp = minx + (maxx - minx) / 2.0;
					moveCode = 1;
					break;
				case 2:
					xp = 0;
					for (uint a = 0; a < AObjects.count(); ++a)
					{
						xp = QMAX(AObjects[a].x2, xp);
					}
					moveCode = 2;
					break;
			}
			if (Xindex.count() == 1)
				doAlign(Yindex, moveCode, xp, xdisp, ydisp);
			else
				doAlign(Xindex, moveCode, xp, xdisp, ydisp);
		}
	}
	xp = 99999.9;
	minx = 99999.9;
	miny = 99999.9;
	maxx = -99999.9;
	maxy = -99999.9;
	if (ya)
	{
		if (Vtv)
		{
			if (AObjects.count() < 3)
				return;
			for (uint a = 0; a < AObjects.count(); ++a)
			{
				minx = QMIN(AObjects[a].y1, minx);
				maxx = QMAX(AObjects[a].y2, maxx);
			}
			xp = (maxx - minx) / (AObjects.count() - 1);
			if (Yindex.count() == 1)
				doAlign(Xindex, 13, xp, xdisp, ydisp, minx);
			else
				doAlign(Yindex, 13, xp, xdisp, ydisp, minx);
		}
		else
		{
			switch (yart)
			{
				case 0:
					for (uint a = 0; a < AObjects.count(); ++a)
					{
						xp = QMIN(AObjects[a].y1, xp);
					}
					moveCode = 10;
					break;
				case 1:
					for (uint a = 0; a < AObjects.count(); ++a)
					{
						miny = QMIN(AObjects[a].y1, miny);
						maxy = QMAX(AObjects[a].y2, maxy);
					}
					xp = miny + (maxy - miny) / 2.0;
					moveCode = 11;
					break;
				case 2:
					xp = 0;
					for (uint a = 0; a < AObjects.count(); ++a)
					{
						xp = QMAX(AObjects[a].y2, xp);
					}
					moveCode = 12;
					break;
			}
			if (Yindex.count() == 1)
				doAlign(Xindex, moveCode, xp, xdisp, ydisp);
			else
				doAlign(Yindex, moveCode, xp, xdisp, ydisp);
		}
	}
	updateContents();
}

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

void ScribusView::ToPathText()
{
	if (SelItem.count() > 1)
	{
		PageItem *currItem = SelItem.at(0);
		PageItem *bb;
		if (currItem->itemType() == PageItem::TextFrame)
			bb = SelItem.at(1);
		else
		{
			bb = SelItem.at(0);
			currItem = SelItem.at(1);
		}
		if (bb->itemType() != PageItem::PolyLine)
			return;
		currItem->Frame = false;
		currItem->ClipEdited = true;
		currItem->convertTo(PageItem::PathText);
		currItem->PoLine = bb->PoLine.copy();
		currItem->Pwidth = bb->Pwidth;
		currItem->setLineColor(bb->lineColor());
		currItem->PLineArt = bb->PLineArt;
		currItem->PLineEnd = bb->PLineEnd;
		currItem->PLineJoin = bb->PLineJoin;
/*		if (!Doc->loading)
			emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr); */
		UpdatePolyClip(currItem);
		AdjustItemSize(currItem);
		double dx = bb->Xpos - currItem->Xpos;
		double dy = bb->Ypos - currItem->Ypos;
		MoveItem(dx, dy, currItem);
		currItem->Rot = bb->Rot;
		currItem->FrameType = 3;
		Deselect(true);
		SelectItemNr(bb->ItemNr);
		DeleteItem();
		updateContents();
	}
}

void ScribusView::FromPathText()
{
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		uint z = PaintPolyLine(currItem->Xpos, currItem->Ypos, currItem->Width, currItem->Height, currItem->Pwidth, "None", currItem->lineColor());
		PageItem *bb = Doc->Items.at(z);
		bb->PoLine = currItem->PoLine.copy();
		bb->ClipEdited = true;
		bb->FrameType = 3;
		bb->Rot = currItem->Rot;
		SetPolyClip(bb, qRound(QMAX(bb->Pwidth / 2, 1)));
		AdjustItemSize(bb);
		currItem->convertTo(PageItem::TextFrame);
		currItem->setLineColor("None");
		currItem->Frame = true;
		SetRectFrame(currItem);
		SelectItemNr(currItem->ItemNr);
		ToFront();
		Deselect(true);
		update();
	}
}

void ScribusView::TextToPath()
{
	ScApp->NoFrameEdit();
	if (SelItem.count() > 0)
	{
		PageItem *bb;
		FPointArray pts;
		double x, y;
		QString chx, ccounter;
		PageItem *currItem = SelItem.at(0);
		Deselect();
		if (currItem->itemText.count() == 0)
			return;
		for (uint a = 0; a < currItem->MaxChars; ++a)
		{
			pts.resize(0);
			x = 0.0;
			y = 0.0;
			chx = currItem->itemText.at(a)->ch;
			if ((chx == QChar(13)) || (chx == QChar(32)) || (chx == QChar(29)))
				continue;
			if (chx == QChar(30))
			{
				chx = currItem->ExpandToken(a);
				if (chx == QChar(32))
					continue;
			}
			int chs = currItem->itemText.at(a)->csize;
			if (currItem->itemText.at(a)->cstyle & 64)
			{
				if (chx.upper() != chx)
				{
					chs = QMAX(static_cast<int>(currItem->itemText.at(a)->csize * Doc->typographicSetttings.valueSmallCaps / 100), 1);
					chx = chx.upper();
				}
			}
			double csi = static_cast<double>(chs) / 100.0;
			uint chr = chx[0].unicode();
			QWMatrix chma;
			chma.scale(csi, csi);
			pts = currItem->itemText.at(a)->cfont->GlyphArray[chr].Outlines.copy();
			if (pts.size() < 4)
				continue;
			x = currItem->itemText.at(a)->cfont->GlyphArray[chr].x * csi;
			y = currItem->itemText.at(a)->cfont->GlyphArray[chr].y * csi;
			pts.map(chma);
			chma = QWMatrix();
			chma.scale(currItem->itemText.at(a)->cscale / 100.0, 1);
			pts.map(chma);
			chma = QWMatrix();
			if (currItem->imageFlippedH() && (!currItem->Reverse))
				chma.scale(-1, 1);
			if (currItem->imageFlippedV())
				chma.scale(1, -1);
			pts.map(chma);
			uint z = PaintPoly(currItem->Xpos, currItem->Ypos, currItem->Width, currItem->Height, currItem->Pwidth, currItem->lineColor(), currItem->fillColor());
			bb = Doc->Items.at(z);
			bb->setTextFlowsAroundFrame(currItem->textFlowsAroundFrame());
			bb->setTextFlowUsesBoundingBox(currItem->textFlowUsesBoundingBox());
			bb->setSizeLocked(currItem->sizeLocked());
			bb->setLocked(currItem->locked());
			bb->NamedLStyle = currItem->NamedLStyle;
			bb->setItemName(currItem->itemName()+"+"+ccounter.setNum(a));
			bb->AutoName = false;
			bb->PoLine = pts.copy();
			bb->Rot = currItem->Rot;
			bb->setFillColor(currItem->itemText.at(a)->ccolor);
			bb->setFillShade(currItem->itemText.at(a)->cshade);
			if (currItem->itemText.at(a)->cstyle & 4)
			{
				bb->lineColor() = currItem->itemText.at(a)->cstroke;
				bb->setLineShade(currItem->itemText.at(a)->cshade2);
			}
			else
			{
				bb->lineColor() = "None";
				bb->setLineShade(100);
			}
			bb->Pwidth = QMAX(currItem->itemText.at(a)->cfont->strokeWidth * chs / 2.0, 1);
			FPoint tp2 = getMinClipF(&bb->PoLine);
			bb->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp = getMaxClipF(&bb->PoLine);
			bb->Width = tp.x();
			bb->Height = tp.y();
			bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
			FPoint npo;
			double textX = currItem->itemText.at(a)->xp;
			double textY = currItem->itemText.at(a)->yp;
			double wide;
			if (a < currItem->itemText.count()-1)
				wide = Cwidth(Doc, currItem->itemText.at(a)->cfont, chx, currItem->itemText.at(a)->csize, currItem->itemText.at(a+1)->ch);
			else
				wide = Cwidth(Doc, currItem->itemText.at(a)->cfont, chx, currItem->itemText.at(a)->csize);
			if (currItem->imageFlippedH())
				textX = currItem->Width - textX - wide;
			if (currItem->imageFlippedV())
				textY = currItem->Height - textY+ y - (bb->Height - y);
			npo = transformPoint(FPoint(textX+x, textY-y), 0.0, 0.0, currItem->Rot, 1.0, 1.0);
			bb->Xpos = currItem->Xpos+npo.x();
			bb->Ypos = currItem->Ypos+npo.y();
			bb->ContourLine = bb->PoLine.copy();
			bb->ClipEdited = true;
			setRedrawBounding(bb);
			SelItem.append(bb);
		}
		if (SelItem.count() > 1)
		{
			for (uint ag = 0; ag < SelItem.count(); ++ag)
			{
				bb = SelItem.at(ag);
				bb->Groups.push(Doc->GroupCounter);
			}
		}
		Doc->GroupCounter++;
		SelItem.clear();
		SelItem.append(currItem);
		DeleteItem();
	}
}

void ScribusView::UniteObj()
{
	PageItem *currItem;
	PageItem *bb;
	QValueList<int> toDel;
	toDel.clear();
	if (SelItem.count() > 1)
	{
		currItem = SelItem.at(0);
		currItem->Frame = false;
		currItem->ClipEdited = true;
		currItem->Groups.clear();
		for (uint a = 1; a < SelItem.count(); ++a)
		{
			bb = SelItem.at(a);
			bb->Groups.clear();
			toDel.append(bb->ItemNr);
			QWMatrix ma;
			ma.translate(bb->Xpos, bb->Ypos);
			ma.rotate(bb->Rot);
			bb->PoLine.map(ma);
			QWMatrix ma2;
			ma2.translate(currItem->Xpos, currItem->Ypos);
			ma2.rotate(currItem->Rot);
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
		DeleteItem();
		updateContents();
	}
}

void ScribusView::SplitObj()
{
	PageItem *bb;
	uint StartInd = 0;
	uint z;
	PageItem *currItem = SelItem.at(0);
	uint EndInd = currItem->PoLine.size();
	for (uint a = currItem->PoLine.size()-1; a > 0; a--)
	{
		if (currItem->PoLine.point(a).x() > 900000)
		{
			StartInd = a + 1;
			z = PaintPoly(currItem->Xpos, currItem->Ypos, currItem->Width, currItem->Height, currItem->Pwidth, currItem->fillColor(), currItem->lineColor());
			bb = Doc->Items.at(z);
			bb->PoLine.resize(0);
			bb->PoLine.putPoints(0, EndInd - StartInd, currItem->PoLine, StartInd);
			bb->Rot = currItem->Rot;
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
	if ((Mpressed) && (MidButt))
	{
		w->delta() > 0 ? slotZoomIn2() : slotZoomOut2();
	}
	else
	{
		if ((w->orientation() != Qt::Vertical) || ( w->state() & ShiftButton ))
		{
			if (w->delta() < 0)
				scrollBy(Prefs->Wheelval, 0);
			else
				scrollBy(-Prefs->Wheelval, 0);
		}
		else
		{
			if (w->delta() < 0)
				scrollBy(0, Prefs->Wheelval);
			else
				scrollBy(0, -Prefs->Wheelval);
		}
	}
	w->accept();
}

void ScribusView::setObjectUndoMode()
{
	_isGlobalMode = undoManager->isGlobalMode();
	if (ScApp->HaveDoc)
	{
		ScApp->scrActions["editActionMode"]->setOn(true);
		if (SelItem.count() == 1)
			undoManager->showObject(SelItem.at(0)->getUId());
		else if (SelItem.count() > 1)
			undoManager->showObject(Um::NO_UNDO_STACK);
		else if (SelItem.count() == 0)
			undoManager->showObject(Doc->currentPage->getUId());
	}
}

void ScribusView::setGlobalUndoMode()
{
	if (ScApp->HaveDoc)
	{
		ScApp->scrActions["editActionMode"]->setOn(!_isGlobalMode);
		if (_isGlobalMode)
			undoManager->showObject(Um::GLOBAL_UNDO_MODE);
		else
		{
			if (SelItem.count() == 1)
				undoManager->showObject(SelItem.at(0)->getUId());
			else if (SelItem.count() > 1)
				undoManager->showObject(Um::NO_UNDO_STACK);
			else if (SelItem.count() == 0)
				undoManager->showObject(Doc->currentPage->getUId());
		}
	}
}

