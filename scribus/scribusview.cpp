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
#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif
#ifdef HAVE_CMS
	#include CMS_INC
#endif

using namespace std;

extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern double Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2 = " ");
extern ScribusApp* ScApp;
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern QPixmap loadIcon(QString nam);
extern double xy2Deg(double x, double y);
extern void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
extern FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
extern FPoint getMaxClipF(FPointArray* Clip);
extern FPoint getMinClipF(FPointArray* Clip);
extern void WordAndPara(PageItem* b, int *w, int *p, int *c, int *wN, int *pN, int *cN);
#ifdef HAVE_CMS
QImage ProofPict(QImage *Im, QString Prof, int Rend, cmsHPROFILE emPr=0);
#else
QImage ProofPict(QImage *Im, QString Prof, int Rend);
#endif
extern int callGS(const QStringList & args);
extern QImage LoadPicture(QString fn, QString Prof, int rend, bool useEmbedded, bool useProf, int requestType, int gsRes, bool *realCMYK = 0, ImageInfoRecord *info = 0);
extern double UmReFaktor;
extern ProfilesL InputProfiles;

ScribusView::ScribusView(QWidget *parent, ScribusDoc *doc, ApplicationPrefs *prefs) : QScrollView(parent, "s", WRepaintNoErase | WNorthWestGravity)
{
	Ready = false;
	updateOn = true;
	Doc = doc;
	Doc->PageC = 0;
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
	SB1 = new QPushButton(this);
	SB1->setPixmap(loadIcon("Klein.xpm"));
	SB1->setFocusPolicy(QWidget::NoFocus);
	SB2 = new QPushButton(this);
	SB2->setFocusPolicy(QWidget::NoFocus);
	SB2->setPixmap(loadIcon("Gross.xpm"));
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
	for (uint i=0;i<=unitGetMaxIndex();++i)
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
	connect(SB1, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
	connect(SB2, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
	connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
	connect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	connect(Laymen, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	connect(Unitmen, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
	connect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
	connect(this, SIGNAL(HaveSel(int)), this, SLOT(selectionChanged()));
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
		QPixmap pm = QPixmap(clipw, cliph);
		ScPainter *painter = new ScPainter(&pm, clipw, cliph);
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
			PageItem *b;
			if ((Doc->guidesSettings.linkShown) && (linkedFramesToShow.count() != 0))
				b = linkedFramesToShow.at(0);
			else
			{
				if (linkedFramesToShow.count() != 0)
					b = linkedFramesToShow.at(0);
				else
					b = SelItem.at(0);
			}
			if ((((Doc->appMode == LinkFrames) || (Doc->appMode == UnlinkFrames))
				   && (b->itemType() == PageItem::TextFrame)) || (Doc->guidesSettings.linkShown))
			{
				PageItem *nb = b;
				if (Doc->guidesSettings.linkShown)
				{
					for (uint lks = 0; lks < linkedFramesToShow.count(); ++lks)
					{
						nb = linkedFramesToShow.at(lks);
						while (nb != 0)
						{
							double x11, y11, x12, y12, x1mid, y1mid;
							double x21, y21, x22, y22, x2mid, y2mid;
							x11 = nb->Xpos;
							y11 = nb->Ypos;
							x12 = nb->Xpos+nb->Width;
							y12 = nb->Ypos+nb->Height;
							x1mid = x11+(x12-x11)/2;
							y1mid = y11+(y12-y11)/2;

							if (nb->Rot!=0.000)
							{
								FPoint tempPoint = transformPoint(FPoint(0,0), x11, y11, nb->Rot, 1, 1);
								x11=tempPoint.x();
								y11=tempPoint.y();
								tempPoint = transformPoint(FPoint(0,0), x12, y12, nb->Rot, 1, 1);
								x12=tempPoint.x();
								y12=tempPoint.y();
								tempPoint = transformPoint(FPoint(0,0), x1mid, y1mid, nb->Rot, 1, 1);
								x1mid=tempPoint.x();
								y1mid=tempPoint.y();
							}


							double a1, b1, a2, b2;
							a1 = a2 = b1 = b2 = 0;
							if (nb->NextBox!=NULL)
							{
								x21 = nb->NextBox->Xpos;
								y21 = nb->NextBox->Ypos;
								x22 = nb->NextBox->Xpos+nb->NextBox->Width;
								y22 = nb->NextBox->Ypos+nb->NextBox->Height;
								x2mid = nb->NextBox->Xpos + nb->NextBox->Width/2;
								y2mid = nb->NextBox->Ypos + nb->NextBox->Height/2;
								x2mid = x21+(x22-x21)/2;
								y2mid = y21+(y22-y21)/2;

								if (nb->NextBox->Rot!=0.000)
								{
									FPoint tempPoint = transformPoint(FPoint(0,0), x21, y21, nb->NextBox->Rot, 1, 1);
									x21=tempPoint.x();
									y21=tempPoint.y();
									tempPoint = transformPoint(FPoint(0,0), x22, y22, nb->NextBox->Rot, 1, 1);
									x22=tempPoint.x();
									y22=tempPoint.y();
									tempPoint = transformPoint(FPoint(0,0), x2mid, y2mid, nb->NextBox->Rot, 1, 1);
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

							FPoint Start = transformPoint(FPoint(a1-nb->Xpos, b1-nb->Ypos), nb->Xpos, nb->Ypos, nb->Rot, 1, 1);
							//FPoint Start = transformPoint(FPoint(nb->Width/2, nb->Height), nb->Xpos, nb->Ypos, nb->Rot, 1, 1);
							nb = nb->NextBox;
							if (nb != NULL)
							{
								FPoint End = transformPoint(FPoint(a2-nb->Xpos, b2-nb->Ypos), nb->Xpos, nb->Ypos, nb->Rot, 1, 1);
								//FPoint End = transformPoint(FPoint(nb->Width/2, 0), nb->Xpos, nb->Ypos, nb->Rot, 1, 1);
								painter->setPen(black, 5.0 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
								painter->setPenOpacity(0.3);
								painter->drawLine(Start, End);
								double r = atan2(End.y()-Start.y(),End.x()-Start.x())*(180.0/3.1415927);
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
					while (nb != 0)
					{
						if (nb->BackBox != 0)
							nb = nb->BackBox;
						else
							break;
					}
					while (nb != 0)
					{
						FPoint Start = transformPoint(FPoint(nb->Width/2, nb->Height), nb->Xpos, nb->Ypos, nb->Rot, 1, 1);
						nb = nb->NextBox;
						if (nb != 0)
						{
							FPoint End = transformPoint(FPoint(nb->Width/2, 0), nb->Xpos, nb->Ypos, nb->Rot, 1, 1);
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
		vr = contentsToViewport(QPoint(clipx, clipy));
		bitBlt( viewport(), vr.x(), vr.y(), &pm, 0, 0, clipw, cliph );
		delete painter;
	}
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		b->paintObj();
		if ((Doc->EditClip) && (b->Select))
			MarkClip(b);
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
		PageItem *b = SelItem.at(0);
		QPainter p;
		p.begin(viewport());
		ToView(&p);
		Transform(b, &p);
		p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
		p.setBrush(NoBrush);
		p.drawLine(QPoint(qRound(b->GrStartX), qRound(b->GrStartY)), QPoint(qRound(b->GrEndX), qRound(b->GrEndY)));
		p.setPen(QPen(magenta, 8, SolidLine, RoundCap, MiterJoin));
		p.drawLine(QPoint(qRound(b->GrStartX), qRound(b->GrStartY)), QPoint(qRound(b->GrStartX), qRound(b->GrStartY)));
		p.drawLine(QPoint(qRound(b->GrEndX), qRound(b->GrEndY)), QPoint(qRound(b->GrEndX), qRound(b->GrEndY)));
		p.end();
	}
//	qDebug( "Time elapsed: %d ms", tim.elapsed() );
}

void ScribusView::DrawMasterItems(ScPainter *painter, Page *page, QRect clip)
{
	QPainter p;
	int Lnr;
	struct Layer ll;
	PageItem* b;
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
				if (ll.isViewable)
				{
					for (uint a = 0; a < page->FromMaster.count(); ++a)
					{
						b = page->FromMaster.at(a);
						if (b->LayerNr != ll.LNr)
							continue;
						if ((b->OwnPage != -1) && (b->OwnPage != static_cast<int>(Mp->PageNr)))
							continue;
						uint OldOwn = b->OwnPage;
						double OldX = b->Xpos;
						double OldY = b->Ypos;
						double OldBX = b->BoundingX;
						double OldBY = b->BoundingY;
						b->OwnPage = page->PageNr;
						if (!b->ChangedMasterItem)
						{
							b->Xpos = OldX - Mp->Xoffset + page->Xoffset;
							b->Ypos = OldY - Mp->Yoffset + page->Yoffset;
							b->BoundingX = OldBX - Mp->Xoffset + page->Xoffset;
							b->BoundingY = OldBY - Mp->Yoffset + page->Yoffset;
						}
						QRect oldR = getRedrawBounding(b);
						if (clip.intersects(oldR))
							b->DrawObj(painter, clip);
						b->OwnPage = OldOwn;
						if (!b->ChangedMasterItem)
						{
							b->Xpos = OldX;
							b->Ypos = OldY;
							b->BoundingX = OldBX;
							b->BoundingY = OldBY;
						}
					}
					for (uint a = 0; a < page->FromMaster.count(); ++a)
					{
						b = page->FromMaster.at(a);
						if (b->LayerNr != ll.LNr)
							continue;
						if (!b->isTableItem)
							continue;
						if ((b->OwnPage != -1) && (b->OwnPage != static_cast<int>(Mp->PageNr)))
							continue;
						double OldX = b->Xpos;
						double OldY = b->Ypos;
						double OldBX = b->BoundingX;
						double OldBY = b->BoundingY;
						if (!b->ChangedMasterItem)
						{
							b->Xpos = OldX - Mp->Xoffset + page->Xoffset;
							b->Ypos = OldY - Mp->Yoffset + page->Yoffset;
							b->BoundingX = OldBX - Mp->Xoffset + page->Xoffset;
							b->BoundingY = OldBY - Mp->Yoffset + page->Yoffset;
						}
						QRect oldR = getRedrawBounding(b);
						if (clip.intersects(oldR))
						{
							painter->setZoomFactor(Scale);
							painter->save();
							painter->translate(b->Xpos*Scale, b->Ypos*Scale);
							painter->rotate(b->Rot);
							if (b->lineColor() != "None")
							{
								QColor tmp;
								b->SetFarbe(&tmp, b->lineColor(), b->lineShade());
								if ((b->TopLine) || (b->RightLine) || (b->BottomLine) || (b->LeftLine))
								{
									painter->setPen(tmp, b->Pwidth, b->PLineArt, Qt::SquareCap, b->PLineJoin);
									if (b->TopLine)
										painter->drawLine(FPoint(0.0, 0.0), FPoint(b->Width, 0.0));
									if (b->RightLine)
										painter->drawLine(FPoint(b->Width, 0.0), FPoint(b->Width, b->Height));
									if (b->BottomLine)
										painter->drawLine(FPoint(b->Width, b->Height), FPoint(0.0, b->Height));
									if (b->LeftLine)
										painter->drawLine(FPoint(0.0, b->Height), FPoint(0.0, 0.0));
								}
							}
							painter->restore();
						}
						if (!b->ChangedMasterItem)
						{
							b->Xpos = OldX;
							b->Ypos = OldY;
							b->BoundingX = OldBX;
							b->BoundingY = OldBY;
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
	PageItem* b;
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
			if (ll.isViewable)
			{
				QPtrListIterator<PageItem> docItem(Doc->Items);
				 while ( (b = docItem.current()) != 0 )
				 {
        			++docItem;
					if (b->LayerNr != ll.LNr)
						continue;
					if ((Doc->MasterP) && ((b->OwnPage != -1) && (b->OwnPage != static_cast<int>(Doc->currentPage->PageNr))))
						continue;
					QRect oldR = getRedrawBounding(b);
					if (clip.intersects(oldR))
					{
						if (!((Doc->EditClip) && (Mpressed)))
							b->DrawObj(painter, clip);
						b->Redrawn = true;
						if ((b->itemType() == PageItem::TextFrame) && ((b->NextBox != 0) || (b->BackBox != 0)))
						{
							PageItem *nb = b;
							while (nb != 0)
							{
								if (nb->BackBox != 0)
									nb = nb->BackBox;
								else
									break;
							}
							if (linkedFramesToShow.find(nb) == -1)
								linkedFramesToShow.append(nb);
						}
						if ((Doc->appMode == EditMode) && (b->Select) && (b->itemType() == PageItem::TextFrame))
						{
							horizRuler->ItemPos = b->Xpos - Doc->ScratchLeft;
							horizRuler->ItemEndPos = (b->Xpos+b->Width) - Doc->ScratchLeft;
							if (b->lineColor() != "None")
								horizRuler->lineCorr = b->Pwidth / 2.0;
							else
								horizRuler->lineCorr = 0;
							horizRuler->ColGap = b->ColGap;
							horizRuler->Cols = b->Cols;
							horizRuler->Extra = b->Extra;
							horizRuler->RExtra = b->RExtra;
							horizRuler->First = Doc->docParagraphStyles[Doc->currentParaStyle].First;
							horizRuler->Indent = Doc->docParagraphStyles[Doc->currentParaStyle].Indent;
							if (b->imageFlippedH() || (b->Reverse))
								horizRuler->Revers = true;
							else
								horizRuler->Revers = false;
							horizRuler->ItemPosValid = true;
							horizRuler->repX = false;
							if (Doc->currentParaStyle < 5)
								horizRuler->TabValues = b->TabValues;
							else
								horizRuler->TabValues = Doc->docParagraphStyles[Doc->currentParaStyle].TabValues;
							horizRuler->repaint();
						}
					}
				}
				QPtrListIterator<PageItem> docItem2(Doc->Items);
				 while ( (b = docItem2.current()) != 0 )
				 {
        			++docItem2;
					if (b->LayerNr != ll.LNr)
						continue;
					if (!b->isTableItem)
						continue;
					QRect oldR = getRedrawBounding(b);
					if (clip.intersects(oldR))
					{
						painter->setZoomFactor(Scale);
						painter->save();
						painter->translate(b->Xpos*Scale, b->Ypos*Scale);
						painter->rotate(b->Rot);
						if (b->lineColor() != "None")
						{
							QColor tmp;
							b->SetFarbe(&tmp, b->lineColor(), b->lineShade());
							if ((b->TopLine) || (b->RightLine) || (b->BottomLine) || (b->LeftLine))
							{
								painter->setPen(tmp, b->Pwidth, b->PLineArt, Qt::SquareCap, b->PLineJoin);
								if (b->TopLine)
									painter->drawLine(FPoint(0.0, 0.0), FPoint(b->Width, 0.0));
								if (b->RightLine)
									painter->drawLine(FPoint(b->Width, 0.0), FPoint(b->Width, b->Height));
								if (b->BottomLine)
									painter->drawLine(FPoint(b->Width, b->Height), FPoint(0.0, b->Height));
								if (b->LeftLine)
									painter->drawLine(FPoint(0.0, b->Height), FPoint(0.0, 0.0));
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
/*		double stx = clip.x() / Scale;
		double endx = QMIN(stx + clip.width() / Scale, page->Width);
		double sty = clip.y() / Scale;
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
			PageItem *b = SelItem.at(0);
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
//	PageItem *b;
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
	PageItem *b;
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
							b = SelItem.at(dre);
							if ((b->itemType() == PageItem::TextFrame) && ((b->NextBox != 0) || (b->BackBox != 0)))
							{
								if (b->BackBox != 0)
								{
									bb->BackBox = b->BackBox;
									fin = SelItem.find(b->BackBox);
									if (fin != -1)
										bb->BackBox = pasted.at(fin);
									bb->BackBox->NextBox = bb;
								}
								if (b->NextBox != 0)
								{
									bb->NextBox = b->NextBox;
									fin = SelItem.find(b->NextBox);
									if (fin != -1)
										bb->NextBox = pasted.at(fin);
									bb->NextBox->BackBox = bb;
								}
							}
						}
						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							b = SelItem.at(dre);
							b->NextBox = 0;
							b->BackBox = 0;
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
	PageItem *b = 0;
	if (Doc->EditClip)
	{
		emit EndNodeEdit();
		return;
	}
	if ((GroupSel) || (Doc->appMode != NormalMode))
	{
		if ((GroupSel) && (Doc->appMode == NormalMode))
		{
			if (GetItem(&b))
			{
				if (b->isTableItem)
				{
					Deselect(false);
					SelItem.append(b);
					b->isSingleSel = true;
					b->Select = true;
					emit HaveSel(b->itemType());
					EmitValues(b);
					b->paintObj();
				}
			}
		}
		else
			contentsMousePressEvent(m);
		return;
	}
	if (GetItem(&b))
	{
		if ((b->itemType() == PageItem::Polygon) || (b->itemType() == PageItem::PolyLine) || (b->itemType() == PageItem::ImageFrame) || (b->itemType() == PageItem::PathText))
		{
			if ((b->locked()) || (!b->ScaleType))
			{
				contentsMousePressEvent(m);
				return;
			}
			emit Amode(EditMode);
		}
		else
			if (b->itemType() == PageItem::TextFrame)
			{
				emit b->isAnnotation ? AnnotProps() : Amode(EditMode);
				contentsMousePressEvent(m);
			}
	}
}

void ScribusView::contentsMouseReleaseEvent(QMouseEvent *m)
{
	PageItem *b;
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
		if ((fg) && (m->button() == RightButton) && (!GetItem(&b)))
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
					b = Doc->Items.at(z);
					b->isTableItem = true;
					SelItem.append(b);
					offX += deltaX;
				}
				offY += deltaY;
				offX = 0.0;
			}
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					b = SelItem.at((rc * Cols) + cc);
					if (rc == 0)
						b->TopLink = 0;
					else
						b->TopLink = SelItem.at(((rc-1)*Cols)+cc);
					if (rc == Rows-1)
						b->BottomLink = 0;
					else
						b->BottomLink = SelItem.at(((rc+1)*Cols)+cc);
					if (cc == 0)
						b->LeftLink = 0;
					else
						b->LeftLink = SelItem.at((rc*Cols)+cc-1);
					if (cc == Cols-1)
						b->RightLink = 0;
					else
						b->RightLink = SelItem.at((rc*Cols)+cc+1);
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
			b = Doc->Items.at(z);
			b->PoLine.resize(0);
			b->PoLine.addPoint(RecordP.point(0));
			b->PoLine.addPoint(RecordP.point(0));
			for (uint px = 1; px < RecordP.size()-1; ++px)
			{
				b->PoLine.addPoint(RecordP.point(px));
				b->PoLine.addPoint(RecordP.point(px));
				b->PoLine.addPoint(RecordP.point(px));
				b->PoLine.addPoint(RecordP.point(px));
			}
			b->PoLine.addPoint(RecordP.point(RecordP.size()-1));
			b->PoLine.addPoint(RecordP.point(RecordP.size()-1));
			AdjustItemSize(b);
			SelItem.clear();
			SelItem.append(b);
			b->ClipEdited = true;
			b->Select = true;
			b->FrameType = 3;
			b->OwnPage = OnPage(b);
			emit ItemPos(b->Xpos, b->Ypos);
			emit SetSizeValue(b->Pwidth);
			emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
			emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
			emit ItemGradient(b->GrType);
			emit ItemTrans(b->fillTransparency(), b->lineTransparency());
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
		b = SelItem.at(0);
		SelNode.clear();
		QRect Sele = QRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>((SeRx-Mxp)*sc), static_cast<int>((SeRy-Myp)*sc));
		FPointArray Clip;
		if (EditContour)
			Clip = b->ContourLine;
		else
			Clip = b->PoLine;
		for (uint a = 0; a < Clip.count(); ++a)
		{
			if (Clip.point(a).x() > 900000)
				continue;
			p.begin(viewport());
			Transform(b, &p);
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
		MarkClip(b);
		return;
	}
	if ((Doc->EditClip) && (SegP1 == -1) && (SegP2 == -1))
	{
		if (Imoved)
		{
			b = SelItem.at(0);
			b->OldB2 = b->Width;
			b->OldH2 = b->Height;
			double nx = m->x()/Scale;
			double ny = m->y()/Scale;
			if (!ApplyGuides(&nx, &ny))
			{
				FPoint npg = ApplyGridF(FPoint(nx, ny));
				nx = npg.x();
				ny = npg.y();
			}
			FPoint np = transformPointI(FPoint(nx, ny), b->Xpos, b->Ypos, b->Rot, 1, 1);
			MoveClipPoint(b, np);
		}
		updateContents();
		Imoved = false;
		return;
	}
	if ((Doc->EditClip) && (SegP1 != -1) && (SegP2 != -1))
	{
		SegP1 = -1;
		SegP2 = -1;
		b = SelItem.at(0);
		Imoved = false;
		updateContents();
		return;
	}
	if ((!GetItem(&b)) && (m->button() == RightButton) && (!Doc->DragP) && (Doc->appMode == NormalMode))
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
		if ((GetItem(&b)) && (m->button() == RightButton) && (!Doc->DragP))
		{
			QPopupMenu *pmen = new QPopupMenu();
			QPopupMenu *pmen2 = new QPopupMenu();
			pmen3 = new QPopupMenu();
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			QPopupMenu *pmen4 = new QPopupMenu();
			QPopupMenu *pmenLevel = new QPopupMenu();
			QPopupMenu *pmenPDF = new QPopupMenu();
			setObjectUndoMode();
			if ((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame) || (b->itemType() == PageItem::PathText))
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
				if (b->itemType() == PageItem::ImageFrame)
				{
					QFileInfo fi = QFileInfo(b->Pfile);
					InfoT->setText( tr("Picture"));
					InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignCenter );
					ParCT->setText( tr("File: "));
					InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
					ParC->setText(fi.fileName());
					InfoGroupLayout->addWidget( ParC, 1, 1 );
					WordCT->setText( tr("Original PPI: "));
					InfoGroupLayout->addWidget( WordCT, 2, 0, Qt::AlignRight );
					WordC->setText(txtC.setNum(qRound(b->dpiX))+" x "+txtC2.setNum(qRound(b->dpiY)));
					InfoGroupLayout->addWidget( WordC, 2, 1 );
					CharCT->setText( tr("Actual PPI: "));
					InfoGroupLayout->addWidget( CharCT, 3, 0, Qt::AlignRight );
					CharC->setText(txtC.setNum(qRound(72.0 / b->LocalScX))+" x "+
					               txtC2.setNum(qRound(72.0 / b->LocalScY)));
					InfoGroupLayout->addWidget( CharC, 3, 1 );
				}
				if ((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::PathText))
				{
					int Parag = 0;
					int Words = 0;
					int Chara = 0;
					int ParagN = 0;
					int WordsN = 0;
					int CharaN = 0;
					if (b->itemType() == PageItem::TextFrame)
					{
						if ((b->NextBox != 0) || (b->BackBox != 0))
							InfoT->setText( tr("Linked Text"));
						else
							InfoT->setText( tr("Text Frame"));
					}
					else
						InfoT->setText( tr("Text on a Path"));
					InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignCenter );
					WordAndPara(b, &Words, &Parag, &Chara, &WordsN, &ParagN, &CharaN);
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
				if (b->isPrintable == true)
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
			if (b->itemType() == PageItem::ImageFrame)
			{
				ScApp->scrActions["fileImportImage"]->addTo(pmen);
				//pmen->insertItem( tr("&Get Picture..."), this, SIGNAL(LoadPic()));
				int px = pmen->insertItem( tr("I&mage Visible"), this, SLOT(TogglePic()));
				pmen->setItemChecked(px, b->PicArt);
				if (b->PicAvail)
					pmen->insertItem( tr("&Update Picture"), this, SLOT(UpdatePic()));
				if (b->PicAvail && b->isRaster)
					pmen->insertItem( tr("&Edit Picture"), this, SIGNAL(callGimp()));
				if ((b->PicAvail) && (!b->isTableItem))
					pmen->insertItem( tr("&Adjust Frame to Picture"), this, SLOT(FrameToPic()));
			}
			if (b->itemType() == PageItem::TextFrame)
			{
				ScApp->scrActions["fileImportText"]->addTo(pmen);
				ScApp->scrActions["fileImportAppendText"]->addTo(pmen);
				//pmen->insertItem( tr("&Get Text..."), this, SIGNAL(LoadPic()));
				//pmen->insertItem( tr("&Append Text..."), this, SIGNAL(AppendText()));
				pmen->insertItem( tr("&Edit Text..."), this, SIGNAL(EditText()));
				if (Doc->currentPage->PageNam == "")
				{
					int pxb = pmenPDF->insertItem( tr("Is PDF &Bookmark"), this, SLOT(ToggleBookmark()));
					pmenPDF->setItemChecked(pxb, b->isBookmark);
					pxb = pmenPDF->insertItem( tr("Is PDF A&nnotation"), this, SLOT(ToggleAnnotation()));
					pmenPDF->setItemChecked(pxb, b->isAnnotation);
					if (b->isAnnotation)
					{
						if ((b->AnType == 0) || (b->AnType == 1) || (b->AnType > 9))
							pmenPDF->insertItem( tr("Annotation P&roperties"), this, SIGNAL(AnnotProps()));
						else
							pmenPDF->insertItem( tr("Field P&roperties"), this, SIGNAL(AnnotProps()));
					}
				}
				pmen->insertItem( tr("&PDF Options"), pmenPDF);
			}
			if (b->itemType() == PageItem::PathText)
				pmen->insertItem( tr("Edit Text..."), this, SIGNAL(EditText()));
			if (!b->locked())
				pmen->insertItem( tr("&Lock"), this, SLOT(ToggleLock()));
			else
				pmen->insertItem( tr("Un&lock"), this, SLOT(ToggleLock()));
			if (!b->sizeLocked())
				pmen->insertItem( tr("Lock Object &Size"), this, SLOT(ToggleResize()));
			else
				pmen->insertItem( tr("Unlock Object &Size"), this, SLOT(ToggleResize()));
			if (!b->isSingleSel)
			{
				pmen->insertItem( tr("Send to S&crapbook"), this, SLOT(sentToScrap()));
				if (Doc->Layers.count() > 1)
				{
					for (uint lam=0; lam < Doc->Layers.count(); ++lam)
					{
						int lai = pmen3->insertItem(Doc->Layers[lam].Name);
						if (static_cast<int>(lam) == Doc->ActiveLayer)
							pmen3->setItemEnabled(lai, 0);
					}
					pmen->insertItem( tr("Send to La&yer"), pmen3);
				}
				connect(pmen3, SIGNAL(activated(int)), this, SLOT(sentToLayer(int)));
			}
			if (b->itemType() == PageItem::TextFrame)
				pmen->insertItem( tr("&Insert Sample Text"), this, SLOT(LoremIpsum()));
			if (!b->locked())
			{
				if (SelItem.count() > 1)
				{
					bool isGroup = true;
					int firstElem = -1;
					if (b->Groups.count() != 0)
						firstElem = b->Groups.top();
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
						pmen->insertItem( tr("&Group"), this, SIGNAL(DoGroup()));
				}
				if (b->Groups.count() != 0)
					pmen->insertItem( tr("Un&group"), this, SIGNAL(DoUnGroup()));
				if ((!b->isTableItem) && (!b->isSingleSel))
				{
					pmen->insertItem( tr("Le&vel"), pmenLevel);
					pmenLevel->insertItem( tr("Send to &Back"), this, SLOT(ToBack()));
					pmenLevel->insertItem( tr("Bring to &Front"), this, SLOT(ToFront()));
					pmenLevel->insertItem( tr("&Lower"), this, SLOT(LowerItem()));
					pmenLevel->insertItem( tr("&Raise"), this, SLOT(RaiseItem()));
				}
			}
			if (((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame) || (b->itemType() == PageItem::Polygon)) && (Doc->appMode != EditMode))
			{
				if (b->itemType() == PageItem::TextFrame)
				{
					pmen2->insertItem( tr("&Picture Frame"), this, SLOT(ToPicFrame()));
					if (!b->isTableItem)
					{
						pmen2->insertItem( tr("Pol&ygon"), this, SLOT(ToPolyFrame()));
						pmen2->insertItem( tr("&Outlines"), this, SLOT(TextToPath()));
					}
				}
				if (b->itemType() == PageItem::ImageFrame)
				{
					pmen2->insertItem( tr("&Text Frame"), this, SLOT(ToTextFrame()));
					if (!b->isTableItem)
						pmen2->insertItem( tr("Pol&ygon"), this, SLOT(ToPolyFrame()));
				}
				if (b->itemType() == PageItem::Polygon)
				{
					pmen2->insertItem( tr("&Text Frame"), this, SLOT(ToTextFrame()));
					pmen2->insertItem( tr("&Picture Frame"), this, SLOT(ToPicFrame()));
					pmen2->insertItem( tr("&Bezier Curve"), this, SLOT(ToBezierFrame()));
				}
				pmen->insertItem( tr("Conve&rt to"), pmen2);
			}
			pmen->insertSeparator();
			if (!b->locked() && !(b->isTableItem && b->isSingleSel))
				ScApp->scrActions["editCut"]->addTo(pmen);
			if (!(b->isTableItem && b->isSingleSel))
				ScApp->scrActions["editCopy"]->addTo(pmen);
			if ((Doc->appMode == EditMode) && (ScApp->Buffer2.startsWith("<SCRIBUSTEXT")) && (b->itemType() == PageItem::TextFrame))
				ScApp->scrActions["editPaste"]->addTo(pmen);
			if (!b->locked() && (Doc->appMode != 7) && (!(b->isTableItem && b->isSingleSel)))
				pmen->insertItem( tr("&Delete"), this, SLOT(DeleteItem()));
			if ((b->itemType() == PageItem::ImageFrame) || (b->itemType() == PageItem::TextFrame))
				pmen->insertItem( tr("C&lear Contents"), this, SLOT(ClearItem()));
			pmen->insertSeparator();
			ScApp->scrActions["toolsProperties"]->addTo(pmen);
			/*
			if (!ScApp->propertiesPalette->isVisible())
				pmen->insertItem( tr("Show P&roperties..."), ScApp, SLOT(togglePropertiesPalette()));
			else
				pmen->insertItem( tr("Hide P&roperties..."), ScApp, SLOT(togglePropertiesPalette()));
			*/
			pmen->exec(QCursor::pos());
			setGlobalUndoMode();
			delete pmen;
			delete pmen2;
			disconnect(pmen3, SIGNAL(activated(int)), this, SLOT(sentToLayer(int)));
			delete pmen3;
			delete pmen4;
			delete pmenLevel;
			delete pmenPDF;
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
			b = SelItem.at(0);
			FPoint np1 = FPoint(m->x() / Scale, m->y() / Scale);
			np1 = ApplyGridF(np1);
			b->Width = np1.x() - b->Xpos;
			b->Height = np1.y()- b->Ypos;
			FPointArray cli = RegularPolygonF(b->Width, b->Height, Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
			FPoint np = FPoint(cli.point(0));
			b->PoLine.resize(2);
			b->PoLine.setPoint(0, np);
			b->PoLine.setPoint(1, np);
			for (uint ax = 1; ax < cli.size(); ++ax)
			{
				np = FPoint(cli.point(ax));
				b->PoLine.putPoints(b->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
			}
			np = FPoint(cli.point(0));
			b->PoLine.putPoints(b->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
			FPoint tp2 = getMinClipF(&b->PoLine);
			if ((tp2.x() > -1) || (tp2.y() > -1))
			{
				SizeItem(b->Width - tp2.x(), b->Height - tp2.y(), b->ItemNr, false, false, false);
			}
			FPoint tp = getMaxClipF(&b->PoLine);
			SizeItem(tp.x(), tp.y(), b->ItemNr, false, false, false);
			b->Clip = FlattenPath(b->PoLine, b->Segments);
			AdjustItemSize(b);
			b->ContourLine = b->PoLine.copy();
			setRedrawBounding(b);
			b->OwnPage = OnPage(b);
			updateContents();
		}
		if (Doc->appMode == DrawLine)
		{
			b = SelItem.at(0);
			QPainter p;
			p.begin(viewport());
			Transform(b, &p);
			QPoint np = p.xFormDev(m->pos());
			p.end();
			np = ApplyGrid(np);
			b->Rot = xy2Deg(np.x(), np.y());
			b->Width = sqrt(pow(np.x(),2.0)+pow(np.y(),2.0));
			b->Height = 1;
			b->Sizing = false;
			UpdateClip(b);
			setRedrawBounding(b);
			b->OwnPage = OnPage(b);
			updateContents();
		}
		if (GetItem(&b))
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
				if (b->Sizing)
				{
					FPoint npx;
					double nx = m->pos().x()/Scale;
					double ny = m->pos().y()/Scale;
					if (Doc->SnapGuides)
					{
						ApplyGuides(&nx, &ny);
						npx = transformPointI(FPoint(nx, ny), b->Xpos, b->Ypos, b->Rot, 1, 1);
					}
					else
						npx = ApplyGridF(transformPointI(FPoint(nx, ny), b->Xpos, b->Ypos, b->Rot, 1, 1));
					if ((HowTo == 1) && (b->itemType() != PageItem::Line) && (Doc->SnapGuides))
						SizeItem(npx.x(), npx.y(), b->ItemNr);
					bool sav = Doc->SnapGuides;
					Doc->SnapGuides = false;
					switch (HowTo)
					{
					case 1:
						if (b->itemType() != PageItem::Line)
						{
							if (b->isTableItem)
							{
								double dist;
								if (b->LeftLink != 0)
									dist = npx.y() - b->LeftLink->Height;
								else if (b->RightLink != 0)
									dist = npx.y() - b->RightLink->Height;
								else
									dist = npx.y() - b->Height;
								PageItem* bb2;
								PageItem* bb = b;
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
									if (bb != b)
										MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
									bb = bb->RightLink;
								}
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								if (bb != b)
									MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = b;
								if (b->TopLink != 0)
									dist = npx.x() - b->TopLink->Width;
								else if (b->BottomLink != 0)
									dist = npx.x() - b->BottomLink->Width;
								else
									dist = npx.x() - b->Width;
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
									if (bb != b)
										MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
									bb = bb->BottomLink;
								}
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								if (bb != b)
									MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
							}
							if (b->imageFlippedH())
								MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
							if (b->imageFlippedV())
								MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
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
								double r = atan2(ny - b->Ypos, nx - b->Xpos)*(180.0/3.1415927);
								RotateItem(r, b->ItemNr);
								double w = sqrt(pow(nx - b->Xpos, 2) + pow(ny - b->Ypos,2));
								SizeItem(w, b->Height, b->ItemNr, true);
							}
						}
						b->Sizing = false;
						break;
					case 2:
						if (b->itemType() != PageItem::Line)
						{
							if (b->isTableItem)
							{
								PageItem* bb2;
								PageItem* bb = b;
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
								bb = b;
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
								MoveSizeItem(npx, npx, b->ItemNr);
							b->Sizing = false;
							if (!b->imageFlippedH())
								MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
							if (!b->imageFlippedV())
								MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
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
								ma.translate(b->Xpos, b->Ypos);
								ma.rotate(b->Rot);
								double mx = ma.m11() * b->Width + ma.m21() * b->Height + ma.dx();
								double my = ma.m22() * b->Height + ma.m12() * b->Width + ma.dy();
								double r = atan2(my-ny,mx-nx)*(180.0/3.1415927);
								double w = sqrt(pow(mx-nx,2)+pow(my-ny,2));
								MoveItem(nx - b->Xpos, ny - b->Ypos, b, true);
								SizeItem(w, b->Height, b->ItemNr, true);
								RotateItem(r, b->ItemNr);
								b->Sizing = false;
							}
						}
						break;
					case 3:
						if (b->isTableItem)
						{
							double dist = npx.x() - b->Width;
							PageItem* bb2;
							PageItem* bb = b;
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
							bb = b;
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
							MoveSizeItem(FPoint(0, npx.y()), FPoint(b->Width - npx.x(), npx.y()), b->ItemNr);
						b->Sizing = false;
						if (b->imageFlippedH())
							MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
						if (!b->imageFlippedV())
							MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
						break;
					case 4:
						if (b->isTableItem)
						{
							double dist = npx.y() - b->Height;
							PageItem* bb2;
							PageItem* bb = b;
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
							bb = b;
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
							MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), b->Height - npx.y()), b->ItemNr);
						b->Sizing = false;
						if (!b->imageFlippedH())
							MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
						if (b->imageFlippedV())
							MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
						break;
					case 5:
						if (b->isTableItem)
						{
							double dist = npx.y() - b->Height;
							PageItem* bb2;
							PageItem* bb = b;
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
							MoveSizeItem(FPoint(0, 0), FPoint(0, b->Height - npx.y()), b->ItemNr);
						if (b->imageFlippedV())
							MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
						b->Sizing = false;
						break;
					case 6:
						if (b->isTableItem)
						{
							double dist = npx.x() - b->Width;
							PageItem* bb2;
							PageItem* bb = b;
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
							MoveSizeItem(FPoint(0, 0), FPoint(b->Width - npx.x(), 0), b->ItemNr);
						if (b->imageFlippedH())
							MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
						b->Sizing = false;
						break;
					case 7:
						if (b->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = b;
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
							MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), b->ItemNr);
						b->Sizing = false;
						if (!b->imageFlippedH())
							MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
						if (b->imageFlippedV())
							MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
						break;
					case 8:
						if (b->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = b;
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
							MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), b->ItemNr);
						b->Sizing = false;
						if (b->imageFlippedH())
							MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
						if (!b->imageFlippedV())
							MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
						break;
					}
					if ((b->itemType() == PageItem::TextFrame) && (m->state() & ShiftButton) && (m->state() & ControlButton))
					{
						double scx = b->Width / b->OldB2;
						double scy = b->Height / b->OldH2;
						scx = scx != scy ? scx / scy : 1.0;
						if (b->itemText.count() != 0)
						{
							b->ISize = QMAX(qRound(b->ISize * scy), 1);
							b->LineSp = ((b->ISize / 10.0)* static_cast<double>(Doc->typographicSetttings.autoLineSpacing) / 100) + (b->ISize / 10.0);
							b->TxtScale = QMIN(QMAX(qRound(b->TxtScale * scx), 25), 400);
							Doc->CurrTextScale = b->TxtScale;
							Doc->CurrFontSize = b->ISize;
							emit ItemTextAttr(b->LineSp);
							emit ItemTextCols(b->Cols, b->ColGap);
							emit ItemTextSize(b->ISize);
							emit ItemTextSca(b->TxtScale);
							for (uint aa = 0; aa < b->itemText.count(); ++aa)
							{
								b->itemText.at(aa)->csize = QMAX(qRound(b->itemText.at(aa)->csize*scy), 1);
								b->itemText.at(aa)->cscale = QMAX(QMIN(qRound(b->itemText.at(aa)->cscale*scx), 400), 25);
							}
						}
					}
					if (b->itemType() == PageItem::ImageFrame)
					{
						AdjustPictScale(b);
						AdjustPreview(b, false);
					}
					UpdateClip(b);
					emit ItemTextCols(b->Cols, b->ColGap);
					Doc->SnapGuides = sav;
					updateContents();
					emit DocChanged();
				}
				setRedrawBounding(b);
				b->OwnPage = OnPage(b);
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
					b = SelItem.at(0);
					if (b->itemType() != PageItem::Line)
					{
						if (fabs(b->Width) < 5)
							b->Width = 5;
						if (fabs(b->Height) < 5)
							b->Height = 5;
					}
					if (Doc->useRaster)
					{
						double nx = b->Xpos;
						double ny = b->Ypos;
						if (!ApplyGuides(&nx, &ny))
						{
							FPoint npx;
							npx = ApplyGridF(FPoint(nx, ny));
							nx = npx.x();
							ny = npx.y();
						}
						MoveItem(nx-b->Xpos, ny-b->Ypos, b);
					}
					else
						MoveItem(0, 0, b, false);
				}
				Imoved = false;
				updateContents();
				emit DocChanged();
				setRedrawBounding(b);
				b->OwnPage = OnPage(b);
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
					p.begin(viewport());
					Transform(Doc->Items.at(a), &p);
					QRegion apr = QRegion(p.xForm(Doc->Items.at(a)->Clip));
					QRect apr2 = getRedrawBounding(Doc->Items.at(a));
					p.end();
					if (((Sele.contains(apr.boundingRect())) || (Sele.contains(apr2))) && (Doc->Items.at(a)->LayerNr == Doc->ActiveLayer))
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
		if (GetItem(&b))
		{
			if (SelItem.count() > 1)
			{
				setGroupRect();
				paintGroupRect();
				emit HaveSel(b->itemType());
				double x, y, w, h;
				getGroupRect(&x, &y, &w, &h);
				emit ItemPos(x, y);
				emit ItemGeom(w, h);
			}
			else
			{
				emit HaveSel(b->itemType());
				EmitValues(b);
			}
		}
	}
	if ((Doc->appMode == EditMode) && !HanMove)
	{
		b = SelItem.at(0);
		uint a;
		if (b->itemType() == PageItem::TextFrame)
		{
			if (oldCp == b->CPos)
			{
				for (a = 0; a < b->itemText.count(); ++a)
					b->itemText.at(a)->cselect = false;
				b->HasSel = false;
				emit HasNoTextSel();
				RefreshItem(b);
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
		b = SelItem.at(0);
		b->ClipEdited = true;
		b->FrameType = 3;
		QPainter p;
		QPointArray Bez(4);
		p.begin(viewport());
		Transform(b, &p);
		FPoint npf = FPoint(p.xFormDev(m->pos()));
		npf = ApplyGridF(npf);
		b->PoLine.addPoint(npf);
		bool ssiz = b->Sizing;
		b->Sizing = true;
		if ((b->PoLine.size() % 4 == 0) && (b->PoLine.size() > 3))
		{
			FPoint lxy = b->PoLine.point(b->PoLine.size()-2);
			FPoint lk = b->PoLine.point(b->PoLine.size()-1);
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			b->PoLine.addPoint(lxy);
			b->PoLine.addPoint(lk);
		}
		FPoint np2 = getMinClipF(&b->PoLine);
		if (np2.x() < 0)
		{
			b->PoLine.translate(-np2.x(), 0);
			MoveItem(np2.x(), 0, b);
		}
		if (np2.y() < 0)
		{
			b->PoLine.translate(0, -np2.y());
			MoveItem(0, np2.y(), b);
		}
		if (FirstPoly)
		{
			FirstPoly = false;
			b->Sizing = ssiz;
		}
		else
		{
		SizeItem(b->PoLine.WidthHeight().x(), b->PoLine.WidthHeight().y(), b->ItemNr, false, false, false);
		SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
		AdjustItemSize(b);
		b->Sizing = ssiz;
		b->ContourLine = b->PoLine.copy();
		RefreshItem(b);
		}
		p.end();
	}
	if ((Doc->appMode == DrawBezierLine) && (m->button() == RightButton))
	{
		b = SelItem.at(0);
		b->PoLine.resize(b->PoLine.size()-2);
		if (b->PoLine.size() < 4)
		{
//			emit DelObj(Doc->currentPage->PageNr, b->ItemNr);
			Doc->Items.remove(b->ItemNr);
			SelItem.removeFirst();
			emit HaveSel(-1);
		}
		else
		{
			SizeItem(b->PoLine.WidthHeight().x(), b->PoLine.WidthHeight().y(), b->ItemNr, false, false);
			SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
			AdjustItemSize(b);
			b->ContourLine = b->PoLine.copy();
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

void ScribusView::contentsMouseMoveEvent(QMouseEvent *m)
{
	int newX, newY, c;
	double nx, ny, dx, dy;
	uint a;
	PageItem *b;
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
		PageItem *b = SelItem.at(0);
		newX = m->x();
		newY = m->y();
		if (m->state() == LeftButton)
		{
			b->GrStartX -= (Mxp - newX) / Scale;
			b->GrStartX = QMIN(QMAX(0.0, b->GrStartX), b->Width);
			b->GrStartY -= (Myp - newY) / Scale;
			b->GrStartY = QMIN(QMAX(0.0, b->GrStartY), b->Height);
		}
		if (m->state() == RightButton)
		{
			b->GrEndX -= (Mxp - newX) / Scale;
			b->GrEndX = QMIN(QMAX(0.0, b->GrEndX), b->Width);
			b->GrEndY -= (Myp - newY) / Scale;
			b->GrEndY = QMIN(QMAX(0.0, b->GrEndY), b->Height);
		}
		Mxp = newX;
		Myp = newY;
		RefreshItem(b);
		ScApp->propertiesPalette->Cpal->setSpecialGradient(b->GrStartX * UmReFaktor, b->GrStartY * UmReFaktor,
																 					 b->GrEndX * UmReFaktor, b->GrEndY * UmReFaktor,
																 					 b->Width * UmReFaktor, b->Height * UmReFaktor);
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
	if (GetItem(&b))
	{
		newX = static_cast<int>(m->x()/sc);
		newY = static_cast<int>(m->y()/sc);
		if ((Mpressed) && (m->state() == RightButton) && (!Doc->DragP) && (Doc->appMode == NormalMode) && (!b->locked()) && (!(b->isTableItem && b->isSingleSel)))
		{
			if ((abs(Dxp - newX) > 10) || (abs(Dyp - newY) > 10))
			{
				Doc->DragP = true;
				Doc->leaveDrag = false;
				Doc->DraggedElem = b;
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
				RotateItem(b->Rot - (oldW - newW), b->ItemNr);
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
			if ((Doc->useRaster) && (OnPage(b) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			if (!Mpressed)
			{
				QPoint a1 = b->PoLine.pointQ(b->PoLine.size()-2);
				QPoint a2 = b->PoLine.pointQ(b->PoLine.size()-1);
				a1 += QPoint(qRound(b->Xpos), qRound(b->Ypos));
				a2 += QPoint(qRound(b->Xpos), qRound(b->Ypos));
				BezierPoints(&Bez, a1, a2, QPoint(Mxp, Myp), QPoint(Mxp, Myp));
				p.drawCubicBezier(Bez);
				Bez.setPoint(2, QPoint(newX, newY));
				Bez.setPoint(3, QPoint(newX, newY));
				p.drawCubicBezier(Bez);
			}
			else
			{
				QPoint a2 = b->PoLine.pointQ(b->PoLine.size()-1);
				a2 += QPoint(qRound(b->Xpos), qRound(b->Ypos));
				if (b->PoLine.size() > 2)
				{
					QPoint a1 = b->PoLine.pointQ(b->PoLine.size()-2);
					QPoint a3 = b->PoLine.pointQ(b->PoLine.size()-3);
					a1 += QPoint(qRound(b->Xpos), qRound(b->Ypos));
					a3 += QPoint(qRound(b->Xpos), qRound(b->Ypos));
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
				mop = QPoint(m->x(), static_cast<int>((b->Ypos + (newX - b->Xpos)) * sc));
				QCursor::setPos(mapToGlobal(mop));
				newY = static_cast<int>(mop.y()/sc);
			}
			if ((Doc->useRaster) && (OnPage(b) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			p.drawRect(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc),
			           static_cast<int>(Mxp*sc-b->Xpos*sc), static_cast<int>(Myp*sc-b->Ypos*sc));
			p.drawRect(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc),
			           static_cast<int>(newX*sc-b->Xpos*sc), static_cast<int>(newY*sc-b->Ypos*sc));
			p.end();
			emit ItemGeom(newX - b->Xpos, newY - b->Ypos);
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
			p.drawLine(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc), static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc));
			p.drawLine(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc), static_cast<int>(newX*sc), static_cast<int>(newY*sc));
			p.end();
			emit SetAngle(xy2Deg(newX - b->Xpos, newY - b->Ypos));
			emit ItemGeom(sqrt(pow(newX - b->Xpos,2)+pow(newY - b->Ypos,2)), 0);
			Mxp = newX;
			Myp = newY;
		}
		if (Mpressed && (Doc->appMode == EditMode) && (!HanMove))
		{
			if (b->itemType() == PageItem::ImageFrame)
			{
				MoveItemI((newX-Mxp)/b->LocalScX, (newY-Myp)/b->LocalScY, b->ItemNr);
				Mxp = newX;
				Myp = newY;
			}
			if (b->itemType() == PageItem::TextFrame)
			{
				for (a = 0; a < b->itemText.count(); ++a)
					b->itemText.at(a)->cselect = false;
				b->HasSel = false;
				slotSetCurs(m->x(), m->y());
				if (b->itemText.count() > 0)
				{
					if (b->CPos < oldCp)
					{
						for (c = b->CPos; c < oldCp; ++c)
						{
							b->itemText.at(c)->cselect = true;
							b->HasSel = true;
						}
					}
					if (b->CPos > oldCp)
					{
						for (c = oldCp; c < b->CPos; ++c)
						{
							b->itemText.at(c)->cselect = true;
							b->HasSel = true;
						}
					}
				}
				RefreshItem(b);
				emit b->HasSel ? HasTextSel() : HasNoTextSel();
			}
		}
		if (Mpressed && (m->state() & LeftButton) && ((Doc->appMode == NormalMode) || ((Doc->appMode == EditMode) && HanMove)) && (!b->locked()))
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
				b = SelItem.at(0);
				b->OldB2 = b->Width;
				b->OldH2 = b->Height;
				p.begin(viewport());
				Transform(b, &p);
				npf = FPoint(p.xFormDev(m->pos()));
				p.end();
				if ((SegP1 != -1) && (SegP2 != -1))
				{
					FPointArray Clip;
					if (EditContour)
						Clip = b->ContourLine;
					else
						Clip = b->PoLine;
					p.begin(viewport());
					p.translate(static_cast<int>(b->Xpos*Scale), static_cast<int>(b->Ypos*Scale));
					p.rotate(b->Rot);
					FPoint npfN = FPoint(p.xFormDev(QPoint(newX, newY)));
					FPoint npfM = FPoint(p.xFormDev(QPoint(Mxp, Myp)));
					npf.setX(Clip.point(SegP2).x() + (npfN.x()-npfM.x()));
					npf.setY(Clip.point(SegP2).y() + (npfN.y()-npfM.y()));
					ClRe = SegP2;
					MoveClipPoint(b, npf);
					b->OldB2 = b->Width;
					b->OldH2 = b->Height;
					if (EditContour)
						Clip = b->ContourLine;
					else
						Clip = b->PoLine;
					ClRe = SegP1;
					npf2.setX(Clip.point(SegP1).x() + (npfN.x()-npfM.x()));
					npf2.setY(Clip.point(SegP1).y() + (npfN.y()-npfM.y()));
					MoveClipPoint(b, npf2);
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
							Clip = b->ContourLine;
						else
							Clip = b->PoLine;
						for (uint itm = 0; itm < SelNode.count(); ++itm)
						{
							p.begin(viewport());
							p.translate(static_cast<int>(b->Xpos*Scale), static_cast<int>(b->Ypos*Scale));
							p.rotate(b->Rot);
							FPoint npfN = FPoint(p.xFormDev(QPoint(newX, newY)));
							FPoint npfM = FPoint(p.xFormDev(QPoint(Mxp, Myp)));
							p.end();
							npf.setX(Clip.point(*SelNode.at(itm)).x() + (npfN.x()-npfM.x()));
							npf.setY(Clip.point(*SelNode.at(itm)).y() + (npfN.y()-npfM.y()));
							ClRe = *SelNode.at(itm);
							b->OldB2 = b->Width;
							b->OldH2 = b->Height;
							MoveClipPoint(b, npf);
						}
						b->OldB2 = b->Width;
						b->OldH2 = b->Height;
						ClRe = storedClRe;
					}
					else
						MoveClipPoint(b, npf);
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
						b = SelItem.at(0);
						if ((HowTo == 1) || (HowTo == 2))
						{
							switch (HowTo)
							{
							case 1:
								p.begin(viewport());
								Transform(b, &p);
								if ((m->state() & ShiftButton) && (!(m->state() & ControlButton)))
									mop = QPoint(m->x(), static_cast<int>((b->Ypos + (newX - b->Xpos)) * sc));
								else
								{
									if ((m->state() & ControlButton) && (!(m->state() & ShiftButton)))
										mop = QPoint(m->x(), static_cast<int>((b->Ypos + ((newX - b->Xpos) / b->OldB2 * b->OldH2)) * sc));
									else
										mop = QPoint(m->x(), m->y());
								}
								np = p.xFormDev(mop);
								nx = np.x();
								ny = np.y();
								p.end();
								if (b->itemType() != PageItem::Line)
								{
									if ((Doc->useRaster) && (OnPage(b) != -1))
									{
										dx = b->Xpos - int (b->Xpos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
										dy = b->Ypos - int (b->Ypos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
										nx = (qRound(np.x() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid - dx);
										ny = (qRound(np.y() / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid - dy);
									}
									if (Doc->SnapGuides)
									{
										nx += b->Xpos;
										ny += b->Ypos;
										ApplyGuides(&nx, &ny);
										nx -= b->Xpos;
										ny -= b->Ypos;
									}
									erf = SizeItem(nx, ny, b->ItemNr);
								}
								else
								{
									p.begin(viewport());
									double rba = b->Rot;
									b->Rot = 0;
									Transform(b, &p);
									np = p.xFormDev(QPoint(m->x(), m->y()));
									p.end();
									b->Rot = rba;
									np = ApplyGrid(np);
									erf = SizeItem(np.x(), np.y(), b->ItemNr);
									if (Doc->SnapGuides)
									{
										p.begin(viewport());
										b->Sizing = true;
										p.setRasterOp(XorROP);
										p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
										p.drawLine(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc),
												static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc));
										p.drawLine(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc),
												static_cast<int>(newX*sc), static_cast<int>(newY*sc));
										p.end();
									}
								}
								break;
							case 2:
								if (b->itemType() == PageItem::Line)
								{
									p.begin(viewport());
									Transform(b, &p);
									mop = QPoint(m->x(), m->y());
									npf = p.xFormDev(mop);
									nx = np.x();
									ny = np.y();
									p.end();
									double sav = Doc->SnapGuides;
									npf2 = FPoint(nx-Mxp, ny-Myp);
									erf = MoveSizeItem(npf, npf, b->ItemNr);
									Doc->SnapGuides = sav;
									if (sav)
										b->Sizing = true;
								}
								else
								{
									p.begin(viewport());
									np2 = QPoint(newX, newY);
									np2 = ApplyGrid(np2);
									double nx = np2.x();
									double ny = np2.y();
									ApplyGuides(&nx, &ny);
									p.translate(static_cast<int>(b->Xpos), static_cast<int>(b->Ypos));
									p.rotate(b->Rot);
									np2 = p.xFormDev(QPoint(qRound(nx), qRound(ny)));
									p.end();
									p.begin(viewport());
									ToView(&p);
									Transform(b, &p);
									PaintSizeRect(&p, QRect(np2, QPoint(qRound(b->Width), qRound(b->Height))));
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
							p.translate(static_cast<int>(b->Xpos), static_cast<int>(b->Ypos));
							p.rotate(b->Rot);
							np2 = p.xFormDev(QPoint(qRound(nx), qRound(ny)));
							p.end();
							p.begin(viewport());
							ToView(&p);
							Transform(b, &p);
							switch (HowTo)
							{
							case 0:
								break;
							case 3:
								PaintSizeRect(&p, QRect(np2, QPoint(0, qRound(b->Height))));
								break;
							case 4:
								PaintSizeRect(&p, QRect(np2, QPoint(qRound(b->Width), 0)));
								break;
							case 5:
								PaintSizeRect(&p, QRect(QPoint(0, 0), QPoint(qRound(b->Width), np2.y())));
								break;
							case 6:
								PaintSizeRect(&p, QRect(QPoint(0, 0), QPoint(np2.x(), qRound(b->Height))));
								break;
							case 7:
								PaintSizeRect(&p, QRect(QPoint(np2.x(), 0), QPoint(qRound(b->Width), qRound(b->Height))));
								break;
							case 8:
								PaintSizeRect(&p, QRect(QPoint(0, np2.y()), QPoint(qRound(b->Width), qRound(b->Height))));
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
					b = SelItem.at(0);
					if (!(b->isTableItem && b->isSingleSel))
					{
						moveGroup(newX-Mxp, newY-Myp, false);
						if (Doc->SnapGuides)
						{
							double nx = b->Xpos;
							double ny = b->Ypos;
							ApplyGuides(&nx, &ny);
							moveGroup(nx-b->Xpos, ny-b->Ypos, false);
							nx = b->Xpos+b->Width;
							ny = b->Ypos+b->Height;
							ApplyGuides(&nx, &ny);
							moveGroup(nx-(b->Xpos+b->Width), ny-(b->Ypos+b->Height), false);
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
				if (QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
				{
					qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
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
				b = SelItem.at(a);
				if (b->locked())
					break;
				p.begin(viewport());
				Transform(b, &p);
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				QRect mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
				if (Doc->EditClip)
				{
					FPointArray Clip;
					QPointArray cli;
					ClRe2 = -1;
					SegP1 = -1;
					SegP2 = -1;
					if (EditContour)
						Clip = b->ContourLine;
					else
						Clip = b->PoLine;
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
						}
					}
				}
				if (QRegion(p.xForm(QPointArray(QRect(-3, -3, static_cast<int>(b->Width+6), static_cast<int>(b->Height+6))))).contains(mpo))
				{
					tx = p.xForm(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height)));
					if ((tx.intersects(mpo)) && (!b->locked()))
					{
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
						if (Doc->appMode == Rotation)
							qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
						if (Doc->appMode == EditMode)
						{
							if (b->itemType() == PageItem::TextFrame)
								qApp->setOverrideCursor(QCursor(ibeamCursor), true);
							if (b->itemType() == PageItem::ImageFrame)
								qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
						}
						if (!b->sizeLocked())
							HandleCurs(&p, b, mpo);
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
		if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == NormalMode) && (!Doc->GuideLock) && (OnPage(m->x()/sc, m->y()/sc) != -1))
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
	PageItem *b;
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
	SeRy = qRound(m->x()/Scale);
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
				b = SelItem.at(0);
				FPointArray Clip;
				bool edited = false;
				bool pfound = false;
				if (EditContour)
					Clip = b->ContourLine;
				else
					Clip = b->PoLine;
				p.begin(viewport());
				Transform(b, &p);
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
						if (b->itemType() == PageItem::Polygon)
						{
							if ((ClRe != 0) && (ClRe != static_cast<int>(EndInd-2)))
							{
								if (b->Segments.count() == 0)
								{
									cli.putPoints(0, EndInd-(ClRe+2), Clip, ClRe+2);
									cli.putPoints(cli.size(), ClRe+2, Clip);
								}
								else
								{
									cli.putPoints(0, EndInd-StartInd, Clip, StartInd);
									z = PaintPoly(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, b->fillColor(), b->lineColor());
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
									bb->Rot = b->Rot;
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
								b->PoLine = cli.copy();
							}
							ClRe = -1;
							b->ClipEdited = true;
							edited = true;
							Doc->EditClipMode = 0;
							b->convertTo(PageItem::PolyLine);
							SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
							emit PolyOpen();
						}
						else
						{
							if ((b->itemType() == PageItem::PolyLine) || (b->itemType() == PageItem::PathText))
							{
								if ((ClRe > 1) && (ClRe < static_cast<int>(Clip.size()-2)))
								{
									z = PaintPolyLine(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, b->fillColor(), b->lineColor());
									bb = Doc->Items.at(z);
									if (EditContour)
										bb->ContourLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									else
										bb->PoLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									bb->Rot = b->Rot;
									AdjustItemSize(bb);
									bb->ClipEdited = true;
									cli.resize(0);
									cli.putPoints(0, ClRe+2, Clip);
									b->PoLine = cli.copy();
								}
								ClRe = -1;
								b->ClipEdited = true;
								edited = true;
								Doc->EditClipMode = 0;
								SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
								emit PolyOpen();
							}
						}
					}
				}
				if ((Doc->EditClipMode == 2) && (ClRe != -1))
				{
					if (!EdPoints)
						return;
					if ((b->itemType() == PageItem::Polygon) || (b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame))
					{
						if ((b->Segments.count() == 0) && (Clip.size() < 13))
							return;
					}
					else
					{
						if (Clip.size() < 5)
							return;
					}
					if ((b->Segments.count() != 0) && ((EndInd - StartInd) < 13))
					{
						if (StartInd != 0)
							cli.putPoints(0, StartInd-4, Clip);
						cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
					}
					else
					{
						if (ClRe == static_cast<int>(StartInd))
						{
							if ((b->itemType() == PageItem::Polygon) || (b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame))
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
						b->ContourLine = cli.copy();
					else
						b->PoLine = cli.copy();
					ClRe = -1;
					b->ClipEdited = true;
					edited = true;
				}
				if ((Doc->EditClipMode == 1) && (ClRe2 != -1))
				{
					cli.putPoints(0, ClRe2+2, Clip);
					cli.resize(cli.size()+4);
					cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
					cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
					if (EditContour)
						b->ContourLine = cli.copy();
					else
						b->PoLine = cli.copy();
					ClRe2 = -1;
					b->ClipEdited = true;
					edited = true;
				}
				if (edited)
				{
					if (b->itemType() != PageItem::PolyLine)
						b->Clip = FlattenPath(b->PoLine, b->Segments);
					AdjustItemSize(b);
					updateContents();
					emit PStatus(b->itemType(), b->PoLine.size());
					emit DocChanged();
					qApp->setOverrideCursor(QCursor(pointingHandCursor), true);
				}
				MarkClip(b);
				return;
			}
			if (GetItem(&b))
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
						if (b->sizeLocked())
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
						b = SelItem.at(0);
						p.begin(viewport());
						Transform(b, &p);
						if (!b->locked())
						{
							HandleSizer(&p, b, mpo, m);
							if (HowTo != 0)
							{
								if (b->itemType() != PageItem::Line)
									b->Sizing = true;
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
			slotDoCurs(false);
			if (GetItem(&b))
			{
				if (!b->locked())
				{
					p.begin(viewport());
					Transform(b, &p);
					HandleSizer(&p, b, mpo, m);
					p.end();
					if (HowTo != 0)
					{
						HanMove = true;
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
			b = SelItem.at(0);
			oldCp = b->CPos;
			slotDoCurs(true);
			if ((!inText) && ((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame)))
			{
				Deselect(true);
				if (SeleItem(m))
				{
					b = SelItem.at(0);
					if ((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame))
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
				if ((m->button() == MidButton) && (b->itemType() == PageItem::TextFrame))
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
							ss->GetText(b, st, Doc->docParagraphStyles[st].Font, Doc->docParagraphStyles[st].FontSize, true);
						else
							ss->GetText(b, st, b->IFont, b->ISize, true);
						delete ss;
						if (Doc->docHyphenator->AutoCheck)
							Doc->docHyphenator->slotHyphenate(b);
					}
					else
					{
						if (ScApp->Buffer2.startsWith("<SCRIBUSTEXT"))
							ScApp->slotEditPaste();
					}
					RefreshItem(b);
				}
			}
			break;
		case DrawLine:
			selectPage(m);
			z = PaintLine(Rxp, Ryp, 1+Rxpd, Rypd, Doc->toolSettings.dWidthLine, Doc->toolSettings.dPenLine);
			b = Doc->Items.at(z);
			b->Select = true;
			qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
			SelItem.clear();
			SelItem.append(b);
			b->paintObj();
			Imoved = true;
			emit ItemPos(b->Xpos, b->Ypos);
			emit SetSizeValue(b->Pwidth);
			emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
			emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
			emit ItemGradient(b->GrType);
			emit ItemTrans(b->fillTransparency(), b->lineTransparency());
			emit HaveSel(PageItem::Line);
			break;
		case Rotation:
			if (GetItem(&b))
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
					Transform(b, &p);
					Doc->RotMode = 2;
					RCenter = transformPointI(FPoint(b->Xpos+b->Width/2, b->Ypos+b->Height/2), 0, 0, b->Rot, 1, 1);
					if (QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))))).contains(mpo))
					{
						if (p.xForm(QRect(static_cast<int>(b->Width)-6, static_cast<int>(b->Height)-6, 6, 6)).intersects(mpo))
						{
							RCenter = FPoint(b->Xpos, b->Ypos);
							Doc->RotMode = 0;
						}
						if (p.xForm(QRect(0, 0, 6, 6)).intersects(mpo))
						{
							RCenter = transformPointI(FPoint(b->Xpos+b->Width, b->Ypos+b->Height), 0, 0, b->Rot, 1, 1);
							Doc->RotMode = 4;
						}
						if (p.xForm(QRect(0, static_cast<int>(b->Height)-6, 6, 6)).intersects(mpo))
						{
							RCenter = transformPointI(FPoint(b->Xpos+b->Width, b->Ypos), 0, 0, b->Rot, 1, 1);
							Doc->RotMode = 1;
						}
						if (p.xForm(QRect(static_cast<int>(b->Width)-6, 0, 6, 6)).intersects(mpo))
						{
							RCenter = transformPointI(FPoint(b->Xpos, b->Ypos+b->Height), 0, 0, b->Rot, 1, 1);
							Doc->RotMode = 3;
						}
						oldW = xy2Deg(m->x()/Scale - RCenter.x(), m->y()/Scale - RCenter.y());
						p.end();
					}
				}
			}
			break;
		case LinkFrames:
			b = Doc->ElemToLink;
			SeleItem(m);
			if (GetItem(&bb))
			{
				if ((bb->itemText.count() == 0) && (bb->NextBox == 0) && (bb->BackBox == 0) && (b != bb))
				{
					b->NextBox = bb;
					bb->BackBox = b;
					if (bb->ItemNr < b->ItemNr)
					{
						Doc->Items.insert(b->ItemNr+1, bb);
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
				Doc->ElemToLink = 0;
			break;
		case UnlinkFrames:
			SeleItem(m);
			if (GetItem(&b))
			{
				if (b->BackBox != 0)
				{
					if (b->NextBox != 0)
					{
						PageItem* nb = b->NextBox;
						while (nb != 0)
						{
							uint a = nb->itemText.count();
							for (uint s=0; s<a; ++s)
								b->itemText.append(nb->itemText.take(0));
							nb = nb->NextBox;
						}
					}
					uint a2 = b->itemText.count();
					for (uint s = 0; s < a2; ++s)
						b->BackBox->itemText.append(b->itemText.take(0));
					b->BackBox->NextBox = 0;
					b->BackBox = 0;
				}
				emit DocChanged();
				updateContents();
			}
			break;
		case DrawRegularPolygon:
			{
				selectPage(m);
				z = PaintPoly(Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen);
				b = Doc->Items.at(z);
				FPointArray cli = RegularPolygonF(b->Width, b->Height, Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
				FPoint np = FPoint(cli.point(0));
				b->PoLine.resize(2);
				b->PoLine.setPoint(0, np);
				b->PoLine.setPoint(1, np);
				for (uint ax = 1; ax < cli.size(); ++ax)
				{
					np = FPoint(cli.point(ax));
					b->PoLine.putPoints(b->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
				}
				np = FPoint(cli.point(0));
				b->PoLine.putPoints(b->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
				b->Clip = FlattenPath(b->PoLine, b->Segments);
				b->Select = true;
				qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
				SelItem.clear();
				SelItem.append(b);
				b->paintObj();
				Imoved = true;
				emit ItemPos(b->Xpos, b->Ypos);
				emit SetSizeValue(b->Pwidth);
				emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
				emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
				emit ItemGradient(b->GrType);
				emit ItemTrans(b->fillTransparency(), b->lineTransparency());
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
				b = Doc->Items.at(z);
				SelItem.clear();
				SelItem.append(b);
				b->Select = true;
				qApp->setOverrideCursor(QCursor(crossCursor), true);
			}
			b = SelItem.at(0);
			p.begin(viewport());
			Transform(b, &p);
			npf = FPoint(p.xFormDev(m->pos()));
			p.end();
			npf = ApplyGridF(npf);
			b->PoLine.addPoint(npf);
			npf2 = getMinClipF(&b->PoLine);
			if (npf2.x() < 0)
			{
				b->PoLine.translate(-npf2.x(), 0);
				MoveItem(npf2.x(), 0, b);
			}
			if (npf2.y() < 0)
			{
				b->PoLine.translate(0, -npf2.y());
				MoveItem(0, npf2.y(), b);
			}
			SizeItem(b->PoLine.WidthHeight().x(), b->PoLine.WidthHeight().y(), b->ItemNr, false, false, false);
			SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
			b->paintObj();
			emit ItemPos(b->Xpos, b->Ypos);
			emit SetSizeValue(b->Pwidth);
			emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
			emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
			emit ItemGradient(b->GrType);
			emit ItemTrans(b->fillTransparency(), b->lineTransparency());
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
			b = Doc->Items.at(z);
			b->isAnnotation = true;
			switch (Doc->appMode)
			{
			case InsertPDFButton:
				b->AnType = 2;
				b->AnFlag = 65536;
				break;
			case InsertPDFTextfield:
				b->AnType = 3;
				break;
			case InsertPDFCheckbox:
				b->AnType = 4;
				break;
			case InsertPDFCombobox:
				b->AnType = 5;
				b->AnFlag = 131072;
				break;
			case InsertPDFListbox:
				b->AnType = 6;
				break;
			case InsertPDFTextAnnotation:
				b->AnType = 10;
				break;
			case InsertPDFLinkAnnotation:
				b->AnType = 11;
				b->AnZiel = Doc->currentPage->PageNr;
				b->AnAction = "0 0";
				b->Textflow = false;
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

void ScribusView::SnapToGuides(PageItem* b)
{
	int pg = OnPage(b);
	if (pg == -1)
		return;
	Page* page = Doc->Pages.at(pg);
	if (page->YGuides.count() != 0)
	{
		for (uint yg = 0; yg < page->YGuides.count(); ++yg)
		{
			if ((page->YGuides[yg]+page->Yoffset < (b->Ypos+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->Yoffset > (b->Ypos-Doc->guidesSettings.guideRad)))
			{
				b->Ypos = page->YGuides[yg]+page->Yoffset;
				break;
			}
			if (b->itemType() == PageItem::Line)
			{
				QWMatrix ma;
				ma.translate(b->Xpos, b->Ypos);
				ma.rotate(b->Rot);
				double my = ma.m22() * b->Height + ma.m12() * b->Width + ma.dy();
				if ((page->YGuides[yg]+page->Yoffset < (my+Doc->guidesSettings.guideRad)) && (page->YGuides[yg]+page->Yoffset > (my-Doc->guidesSettings.guideRad)))
				{
					b->Ypos = b->Ypos + page->YGuides[yg] - my + page->Yoffset;
					break;
				}
			}
			else
			{
				if ((page->YGuides[yg]+page->Yoffset < (b->Ypos+b->Height+Doc->guidesSettings.guideRad)) &&
				     (page->YGuides[yg]+page->Yoffset > ((b->Ypos+b->Height)-Doc->guidesSettings.guideRad)))
				{
					b->Ypos = page->YGuides[yg]-b->Height+page->Yoffset;
					break;
				}
			}
		}
	}
	if (page->XGuides.count() != 0)
	{
		for (uint xg = 0; xg < page->XGuides.count(); ++xg)
		{
			if ((page->XGuides[xg]+page->Xoffset < (b->Xpos+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->Xoffset > (b->Xpos-Doc->guidesSettings.guideRad)))
			{
				b->Xpos = page->XGuides[xg]+page->Xoffset;
				break;
			}
			if (b->itemType() == PageItem::Line)
			{
				QWMatrix ma;
				ma.translate(b->Xpos, b->Ypos);
				ma.rotate(b->Rot);
				double mx = ma.m11() * b->Width + ma.m21() * b->Height + ma.dx();
				if ((page->XGuides[xg]+page->Xoffset < (mx+Doc->guidesSettings.guideRad)) && (page->XGuides[xg]+page->Xoffset > (mx-Doc->guidesSettings.guideRad)))
				{
					b->Xpos = b->Xpos + page->XGuides[xg] - mx + page->Xoffset;
					break;
				}
			}
			else
			{
				if ((page->XGuides[xg]+page->Xoffset < (b->Xpos+b->Width+Doc->guidesSettings.guideRad)) &&
				     (page->XGuides[xg]+page->Xoffset > ((b->Xpos+b->Width)-Doc->guidesSettings.guideRad)))
				{
					b->Xpos = page->XGuides[xg]-b->Width+page->Xoffset;
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

void ScribusView::getBoundingRect(PageItem *b, double *x1, double *y1, double *x2, double *y2)
{
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	if (b->Rot != 0)
	{
		FPointArray pb;
		FPoint p1;
		pb.resize(0);
		pb.addPoint(FPoint(b->Xpos, b->Ypos));
		p1 = transformPoint(FPoint(b->Width, 0.0), b->Xpos, b->Ypos, b->Rot, 1.0, 1.0);
		pb.addPoint(p1);
		p1 = transformPoint(FPoint(b->Width, b->Height), b->Xpos, b->Ypos, b->Rot, 1.0, 1.0);
		pb.addPoint(p1);
		p1 = transformPoint(FPoint(0.0, b->Height), b->Xpos, b->Ypos, b->Rot, 1.0, 1.0);
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
		*x1 = b->Xpos;
		*y1 = b->Ypos;
		*x2 = b->Xpos + b->Width;
		*y2 = b->Ypos + b->Height;
	}
}

QRect ScribusView::getRedrawBounding(PageItem* b)
{
	int x = qRound(floor(b->BoundingX - b->OldPwidth / 2.0 - 5) * Scale);
	int y = qRound(floor(b->BoundingY - b->OldPwidth / 2.0 - 5) * Scale);
	int w = qRound(ceil(b->BoundingW + b->OldPwidth + 10) * Scale);
	int h = qRound(ceil(b->BoundingH + b->OldPwidth + 10) * Scale);
	return QRect(x, y, w, h);
}

void ScribusView::setRedrawBounding(PageItem* b)
{
	double bw, bh;
	getBoundingRect(b, &b->BoundingX, &b->BoundingY, &bw, &bh);
	b->BoundingW = bw - b->BoundingX;
	b->BoundingH = bh - b->BoundingY;
	if (b->itemType() == PageItem::Line)
		b->BoundingH = QMAX(b->BoundingH, 1);
}

void ScribusView::setGroupRect()
{
	PageItem* b;
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	for (uint gc = 0; gc < SelItem.count(); ++gc)
	{
		b = SelItem.at(gc);
		if (b->Rot != 0)
		{
			FPointArray pb;
			FPoint p1;
			pb.resize(0);
			pb.addPoint(FPoint(b->Xpos, b->Ypos));
			p1 = transformPoint(FPoint(b->Width, 0.0), b->Xpos, b->Ypos, b->Rot, 1.0, 1.0);
			pb.addPoint(p1);
			p1 = transformPoint(FPoint(b->Width, b->Height), b->Xpos, b->Ypos, b->Rot, 1.0, 1.0);
			pb.addPoint(p1);
			p1 = transformPoint(FPoint(0.0, b->Height), b->Xpos, b->Ypos, b->Rot, 1.0, 1.0);
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
			minx = QMIN(minx, b->Xpos);
			miny = QMIN(miny, b->Ypos);
			maxx = QMAX(maxx, b->Xpos + b->Width);
			maxy = QMAX(maxy, b->Ypos + b->Height);
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

void ScribusView::RefreshItem(PageItem *b)
{
	updateContents(getRedrawBounding(b));
}

bool ScribusView::MoveItem(double newX, double newY, PageItem* b, bool fromMP)
{
	if (b->locked())
		return false;
	bool retw = false;
	double oldx = b->Xpos;
	double oldy = b->Ypos;
	b->Xpos += newX;
	b->Ypos += newY;
	if ((Doc->useRaster) && (!Imoved) && (!fromMP) && (static_cast<int>(Doc->currentPage->PageNr) == b->OwnPage))
	{
		b->Xpos = qRound(b->Xpos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
		b->Ypos = qRound(b->Ypos / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid;
	}
	if ((Doc->SnapGuides) && (!Imoved) && (Doc->appMode == NormalMode) && (!Doc->EditClip) && (!fromMP))
		SnapToGuides(b);
	if ((b->Xpos != oldx) || (b->Ypos != oldy))
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
			emit ItemPos(b->Xpos, b->Ypos);
	}
/*	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr); */
	QRect oldR = getRedrawBounding(b);
	setRedrawBounding(b);
	QRect newR = getRedrawBounding(b);
	if ((!Imoved) && (!b->Sizing))
		updateContents(newR.unite(oldR));
	b->OwnPage = OnPage(b);
	return retw;
}

void ScribusView::MoveItemI(double newX, double newY, int ite, bool redraw)
{
	PageItem *b = Doc->Items.at(ite);
	if ((b->locked()) || (!b->ScaleType))
		return;
	if (b->imageFlippedH())
		b->LocalX -= newX;
	else
		b->LocalX += newX;
	if (b->imageFlippedV())
		b->LocalY -= newY;
	else
		b->LocalY += newY;
	if (redraw)
		updateContents(getRedrawBounding(b));
	emit SetLocalValues(b->LocalScX, b->LocalScY, b->LocalX, b->LocalY);
}

void ScribusView::ConvertClip(PageItem *b)
{
	if (b->Clip.count() != 0)
	{
		FPoint np = FPoint(b->Clip.point(0));
		b->PoLine.resize(2);
		b->PoLine.setPoint(0, np);
		b->PoLine.setPoint(1, np);
		for (uint a = 1; a < b->Clip.size(); ++a)
		{
			np = FPoint(b->Clip.point(a));
			b->PoLine.putPoints(b->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(),
			                    np.x(), np.y());
		}
		np = FPoint(b->Clip.point(0));
		b->PoLine.putPoints(b->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	}
	else
		SetRectFrame(b);
}

void ScribusView::UpdateClip(PageItem* b)
{
	if (Doc->appMode == DrawBezierLine)
		return;
	int ph = static_cast<int>(QMAX(1.0, b->Pwidth / 2.0));
	switch (b->itemType())
	{
	case PageItem::Line:
		b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
		                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
		                  -ph,static_cast<int>(b->Height+ph));
		break;
	default:
		if (((!b->ClipEdited) || (b->FrameType < 3)) && (b->itemType() != PageItem::PathText))
		{
			switch (b->FrameType)
			{
			case 0:
				SetRectFrame(b);
				break;
			case 1:
				SetOvalFrame(b);
				break;
			case 2:
				SetFrameRound(b);
				break;
			default:
				break;
			}
			if ((b->OldB2 != 0) && (b->OldH2 != 0))
			{
				double scx = b->Width / b->OldB2;
				double scy = b->Height / b->OldH2;
				QWMatrix ma;
				ma.scale(scx, scy);
				FPointArray gr;
				gr.addPoint(b->GrStartX, b->GrStartY);
				gr.addPoint(b->GrEndX, b->GrEndY);
				gr.map(ma);
				b->GrStartX = gr.point(0).x();
				b->GrStartY = gr.point(0).y();
				b->GrEndX = gr.point(1).x();
				b->GrEndY = gr.point(1).y();
				if (b->FrameType > 2)
				{
					b->PoLine.map(ma);
					b->ContourLine.map(ma);
					if (b->itemType() == PageItem::PathText)
						UpdatePolyClip(b);
					else
						b->Clip = FlattenPath(b->PoLine, b->Segments);
				}
			}
			b->OldB2 = b->Width;
			b->OldH2 = b->Height;
			if (b->FrameType < 3)
				b->ContourLine = b->PoLine.copy();
		}
		else
		{
			if (Doc->SubMode != -1)
			{
				switch (Doc->SubMode)
				{
				case 0:
					SetRectFrame(b);
					break;
				case 1:
					SetOvalFrame(b);
					break;
				default:
					SetFrameShape(b, Doc->ValCount, Doc->ShapeValues);
					break;
				}
				b->OldB2 = b->Width;
				b->OldH2 = b->Height;
				b->ContourLine = b->PoLine.copy();
			}
			if ((b->OldB2 == 0) || (b->OldH2 == 0))
				return;
			double scx = b->Width / b->OldB2;
			double scy = b->Height / b->OldH2;
			QWMatrix ma;
			ma.scale(scx, scy);
			FPointArray gr;
			gr.addPoint(b->GrStartX, b->GrStartY);
			gr.addPoint(b->GrEndX, b->GrEndY);
			gr.map(ma);
			b->GrStartX = gr.point(0).x();
			b->GrStartY = gr.point(0).y();
			b->GrEndX = gr.point(1).x();
			b->GrEndY = gr.point(1).y();
			b->PoLine.map(ma);
			b->ContourLine.map(ma);
			if (b->itemType() == PageItem::PathText)
				UpdatePolyClip(b);
			else
				b->Clip = FlattenPath(b->PoLine, b->Segments);
			b->OldB2 = b->Width;
			b->OldH2 = b->Height;
		}
		break;
	}
	updateGradientVectors(b);
}

void ScribusView::SetPolyClip(PageItem *b, int up)
{
	QPoint np, np2;
	QPointArray cl, cl1, cl2;
	double rot;
	b->Clip.resize(0);
	if (b->PoLine.size() < 4)
		return;
	cl = FlattenPath(b->PoLine, b->Segments);
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
	b->Clip.putPoints(b->Clip.size(), cl1.size(), cl1);
	for (int a2 = cl2.size()-1; a2 > -1; a2--)
	{
		b->Clip.resize(b->Clip.size()+1);
		b->Clip.setPoint(b->Clip.size()-1, cl2.point(a2));
	}
}

void ScribusView::UpdatePolyClip(PageItem *b)
{
	struct ScText *hl;
	int asce = 1;
	int desc = 1;
	for (uint a = 0; a < b->itemText.count(); ++a)
	{
		hl = b->itemText.at(a);
		int des = static_cast<int>((*Doc->AllFonts)[hl->cfont]->numDescender * (-hl->csize / 10.0));
		int asc = static_cast<int>((*Doc->AllFonts)[hl->cfont]->numAscent * (hl->csize / 10.0));
		if (asc > asce)
			asce = asc;
		if (des > desc)
			desc = des;
	}
	SetPolyClip(b, static_cast<int>(asce+b->BaseOffs));
}

void ScribusView::MarkClip(PageItem *b)
{
	double x, y;
	QPainter p;
	FPointArray cli;
	QPointArray Bez(4);
	p.begin(viewport());
	ToView(&p);
	Transform(b, &p);
	p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
	p.setBrush(NoBrush);
	if ((EditContour) && (b->ContourLine.size() != 0))
		cli = b->ContourLine;
	else
		cli = b->PoLine;
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
	PageItem *b = SelItem.at(0);
	b->ClipEdited = true;
	QWMatrix ma;
	if (EditContour)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
			ss->set("MIRROR_PATH_H", "mirror_path_h");
			ss->set("IS_CONTOUR", true);
			undoManager->action(b, ss, Um::IBorder);
		}
		FPoint tp, tp2;
		tp2 = getMinClipF(&b->ContourLine);
		tp = getMaxClipF(&b->ContourLine);
		ma.translate(qRound(tp.x()), 0);
		ma.scale(-1, 1);
		b->ContourLine.map(ma);
		RefreshItem(b);
		b->FrameOnly = true;
		b->Tinput = true;
		b->paintObj();
		b->FrameOnly = false;
		MarkClip(b);
		return;
	}
	ma.scale(-1, 1);
	b->PoLine.map(ma);
	b->PoLine.translate(b->Width, 0);
	if (b->itemType() == PageItem::PathText)
		UpdatePolyClip(b);
	else
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	setRedrawBounding(b);
	RefreshItem(b);
	MarkClip(b);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
		ss->set("MIRROR_PATH_H", "mirror_path_h");
		ss->set("IS_CONTOUR", false);
		undoManager->action(b, ss, Um::IBorder);
	}
}

void ScribusView::MirrorPolyV()
{
	PageItem *b = SelItem.at(0);
	b->ClipEdited = true;
	QWMatrix ma;
	if (EditContour)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
			ss->set("MIRROR_PATH_V", "mirror_path_v");
			ss->set("IS_CONTOUR", true);
			undoManager->action(b, ss, Um::IBorder);
		}
		FPoint tp, tp2;
		tp2 = getMinClipF(&b->ContourLine);
		tp = getMaxClipF(&b->ContourLine);
		ma.translate(0, qRound(tp.y()));
		ma.scale(1, -1);
		b->ContourLine.map(ma);
		RefreshItem(b);
		b->FrameOnly = true;
		b->Tinput = true;
		b->paintObj();
		b->FrameOnly = false;
		MarkClip(b);
		return;
	}
	ma.scale(1, -1);
	b->PoLine.map(ma);
	b->PoLine.translate(0, b->Height);
	if (b->itemType() == PageItem::PathText)
		UpdatePolyClip(b);
	else
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	setRedrawBounding(b);
	RefreshItem(b);
	MarkClip(b);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
		ss->set("MIRROR_PATH_V", "mirror_path_v");
		ss->set("IS_CONTOUR", false);
		undoManager->action(b, ss, Um::IBorder);
	}
}

void ScribusView::TransformPoly(int mode, int rot, double scaling)
{
	PageItem *b = SelItem.at(0);
	b->ClipEdited = true;
	QWMatrix ma;
	if (EditContour)
	{
		FPoint tp, tp2;
		tp2 = getMinClipF(&b->ContourLine);
		tp = getMaxClipF(&b->ContourLine);
		b->ContourLine.translate(-qRound((tp.x() + tp2.x()) / 2.0), -qRound((tp.y() + tp2.y()) / 2.0));
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
		b->ContourLine.map(ma);
		b->ContourLine.translate(qRound((tp.x() + tp2.x()) / 2.0), qRound((tp.y() + tp2.y()) / 2.0));
		RefreshItem(b);
		b->FrameOnly = true;
		b->Tinput = true;
		b->paintObj();
		b->FrameOnly = false;
		MarkClip(b);
		if (UndoManager::undoEnabled())
		{
			undoManager->setUndoEnabled(false);
			b->checkChanges(true);
			undoManager->setUndoEnabled(true);
			SimpleState *ss = new SimpleState(Um::EditContourLine, "", Um::IBorder);
			ss->set("EDIT_CONTOUR", "edit_contour");
			ss->set("MODE", mode);
			ss->set("ROT", rot);
			ss->set("SCALING", scaling);
			undoManager->action(b, ss);
		}
		return;
	}
	FPoint oldPos = FPoint(b->Xpos, b->Ypos);
	double offsX = b->Width / 2.0;
	double offsY = b->Height / 2.0;
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
	b->PoLine.map(ma);
	b->PoLine.translate(offsX, offsY);
	AdjustItemSize(b);
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
	MoveItem(x-oldPos.x(), y-oldPos.y(), b);
	if (b->itemType() == PageItem::PathText)
		UpdatePolyClip(b);
	setRedrawBounding(b);
	RefreshItem(b);
	MarkClip(b);
	b->FrameType = 3;
	if (UndoManager::undoEnabled())
	{
		undoManager->setUndoEnabled(false);
		b->checkChanges(true);
		undoManager->setUndoEnabled(true);
		SimpleState *ss = new SimpleState(Um::EditShape, "", Um::IBorder);
		ss->set("EDIT_SHAPE", "edit_shape");
		ss->set("MODE", mode);
		ss->set("ROT", rot);
		ss->set("SCALING", scaling);
		undoManager->action(b, ss);
	}
}

void ScribusView::Reset1Control()
{
	PageItem *b = SelItem.at(0);
	b->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = b->ContourLine.point(ClRe-1);
	else
		np = b->PoLine.point(ClRe-1);
	b->OldB2 = b->Width;
	b->OldH2 = b->Height;
	if (EditContour)
	{
		b->ContourLine.setPoint(ClRe, np);
		RefreshItem(b);
		b->FrameOnly = true;
		b->Tinput = true;
		b->paintObj();
		b->FrameOnly = false;
	}
	else
	{
		b->PoLine.setPoint(ClRe, np);
		AdjustItemSize(b);
	}
	MarkClip(b);
}

void ScribusView::ResetControl()
{
	PageItem *b = SelItem.at(0);
	b->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = b->ContourLine.point(ClRe);
	else
		np = b->PoLine.point(ClRe);
	b->OldB2 = b->Width;
	b->OldH2 = b->Height;
	if ((ClRe == 0) || (ClRe == static_cast<int>(b->PoLine.size()-2)))
	{
		if (EditContour)
			b->ContourLine.setPoint(ClRe+1, np);
		else
			b->PoLine.setPoint(ClRe+1, np);
	}
	else
	{
		if (EditContour)
		{
			b->ContourLine.setPoint(ClRe+1, np);
			b->ContourLine.setPoint((ClRe % 4 != 0 ? ClRe + 3 : ClRe - 1), np);
		}
		else
		{
			b->PoLine.setPoint(ClRe+1, np);
			b->PoLine.setPoint((ClRe % 4 != 0 ? ClRe + 3 : ClRe - 1), np);
		}
	}
	if (!EditContour)
		AdjustItemSize(b);
	else
	{
		RefreshItem(b);
		b->FrameOnly = true;
		b->Tinput = true;
		b->paintObj();
		b->FrameOnly = false;
	}
	MarkClip(b);
}

void ScribusView::MoveClipPoint(PageItem *b, FPoint ip)
{
	if (((EdPoints) && (ClRe % 2 != 0)) || ((!EdPoints) && (ClRe % 2 == 0)))
		return;
	b->ClipEdited = true;
	FPointArray Clip;
	if (EditContour)
		Clip = b->ContourLine;
	else
		Clip = b->PoLine;
	b->FrameType = 3;
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
			SizeItem(b->Width - np.x(), b->Height, b->ItemNr, false, false);
			if (b->Rot != 0)
			{
				FPoint npv = FPoint(np.x(), 0);
				MoveRotated(b, npv);
			}
			else
				MoveItem(np.x(), 0, b);
			Clip.translate(-np.x(), 0);
			if (!b->imageFlippedH())
				MoveItemI(-np.x()/b->LocalScX, 0, b->ItemNr, false);
			np.setX(0);
		}
		if ((np.y() < 0) && (!EditContour))
		{
			SizeItem(b->Width, b->Height - np.y(), b->ItemNr, false, false);
			if (b->Rot != 0)
			{
				FPoint npv = FPoint(0, np.y());
				MoveRotated(b, npv);
			}
			else
				MoveItem(0, np.y(), b);
			Clip.translate(0, -np.y());
			if (!b->imageFlippedV())
				MoveItemI(0, -np.y()/b->LocalScY, b->ItemNr, false);
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
		        ((b->itemType() == PageItem::Polygon) || (b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame)))
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
		        ((b->itemType() == PageItem::Polygon) || (b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame)) && (MoveSym))
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
			b->ContourLine = Clip.copy();
		else
			b->PoLine = Clip.copy();
		b->Clip = FlattenPath(b->PoLine, b->Segments);
		if (!EditContour)
			AdjustItemSize(b);
		else
		{
			RefreshItem(b);
			b->FrameOnly = true;
			b->Tinput = true;
			b->paintObj();
			b->FrameOnly = false;
			MarkClip(b);
		}
	}
}

bool ScribusView::SizeItem(double newX, double newY, int ite, bool fromMP, bool DoUpdateClip, bool redraw)
{
	return SizeItem(newX, newY, Doc->Items.at(ite), fromMP, DoUpdateClip, redraw);
}

bool ScribusView::SizeItem(double newX, double newY, PageItem *pi, bool fromMP, bool DoUpdateClip, bool redraw)
{
	PageItem *b = pi;
	if (b->locked())
		return false;
	QPainter p;
	QRect oldR = getRedrawBounding(b);
	if (b->itemType() != PageItem::Line)
	{
		newX = QMAX(newX, 1);
		newY = QMAX(newY, 1);
	}
	int ph = static_cast<int>(QMAX(1.0, b->Pwidth / 2.0));
	QWMatrix ma;
	ma.rotate(b->Rot);
	double dX = ma.m11() * (b->Width - newX) + ma.m21() * (b->Height - newY) + ma.dx();
	double dY = ma.m22() * (b->Height - newY) + ma.m12() * (b->Width - newX) + ma.dy();
	b->Width = newX;
	b->Height = newY;
	if ((Doc->RotMode != 0) && (fromMP))
	{
		switch (Doc->RotMode)
		{
		case 2:
			MoveItem(dX / 2.0, dY / 2.0, b);
			break;
		case 4:
			MoveItem(dX, dY, b);
			break;
		case 3:
			MoveItem(0.0, dY, b);
			break;
		case 1:
			MoveItem(dX, 0.0, b);
			break;
		}
	}
	b->RadRect = QMIN(b->RadRect, QMIN(b->Width,b->Height)/2);
	if ((b->itemType() == PageItem::ImageFrame) && (!b->Sizing) && (!Doc->EditClip))
	{
		AdjustPictScale(b);
		AdjustPreview(b, false);
	}
	if (b->itemType() == PageItem::Line)
	{
		if (!fromMP)
		{
			b->Rot = atan2(b->Height,b->Width)*(180.0/3.1415927);
			b->Width = sqrt(pow(b->Width,2)+pow(b->Height,2));
			b->Height = 1;
			emit SetAngle(b->Rot);
		}
		b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
		                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
		                  -ph,static_cast<int>(b->Height+ph));
	}
	setRedrawBounding(b);
	b->OwnPage = OnPage(b);
	if (b->Sizing)
	{
		emit ItemGeom(b->Width, b->Height);
		emit ItemRadius(b->RadRect);
		b->FrameOnly = true;
		b->Tinput = true;
		if ((HowTo == 1) && (b->itemType() != PageItem::Line))
			b->paintObj();
		if ((b->FrameType == 0) || (b->itemType() == PageItem::Line) || (HowTo != 1))
			return true;
		QPainter p;
		p.begin(viewport());
		QPoint in  = QPoint(qRound(b->Xpos*Scale), qRound(b->Ypos*Scale));
		QPoint out = contentsToViewport(in);
		p.translate(out.x(), out.y());
		p.scale(Scale, Scale);
		p.rotate(b->Rot);
		p.setRasterOp(XorROP);
		p.setBrush(NoBrush);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		b->DrawPolyL(&p, b->Clip);
		UpdateClip(b);
		updateGradientVectors(b);
		b->DrawPolyL(&p, b->Clip);
		p.end();
		return true;
	}
	if (DoUpdateClip)
	{
		if (fromMP)
		{
			if (b->imageFlippedH())
				MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
			if (b->imageFlippedV())
				MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
		}
		else
		{
			if (!b->imageFlippedH())
				MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr, false);
			if (!b->imageFlippedV())
				MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr, false);
		}
		UpdateClip(b);
	}
	updateGradientVectors(b);
	if (redraw)
	{
		QRect newR = getRedrawBounding(b);
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
			emit ItemGeom(b->Width, b->Height);
	}
	emit ItemRadius(b->RadRect);
	return true;
}

void ScribusView::MoveRotated(PageItem *b, FPoint npv, bool fromMP)
{
	QWMatrix ma;
	ma.translate(b->Xpos, b->Ypos);
	ma.rotate(b->Rot);
	double mxc = b->Xpos - (ma.m11() * npv.x() + ma.m21() * npv.y() + ma.dx());
	double myc = b->Ypos - (ma.m22() * npv.y() + ma.m12() * npv.x() + ma.dy());
	MoveItem(-mxc, -myc, b, fromMP);
}

bool ScribusView::MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP)
{
	PageItem *b = Doc->Items.at(ite);
	QRect oldR = getRedrawBounding(b);
	if (b->itemType() == PageItem::Line)
	{
		QWMatrix ma;
		ma.translate(b->Xpos, b->Ypos);
		ma.rotate(b->Rot);
		double mx = ma.m11() * b->Width + ma.m21() * b->Height + ma.dx();
		double my = ma.m22() * b->Height + ma.m12() * b->Width + ma.dy();
		MoveItem(newX.x(), newX.y(), b, fromMP);
		b->Rot = xy2Deg(mx - b->Xpos, my - b->Ypos);
		b->Width = sqrt(pow(mx - b->Xpos,2)+pow(my - b->Ypos,2));
		b->Height = 1;
		UpdateClip(b);
		setRedrawBounding(b);
		QRect newR = getRedrawBounding(b);
		updateContents(newR.unite(oldR));
		emit ItemPos(b->Xpos, b->Ypos);
		emit ItemGeom(b->Width, b->Height);
		emit SetAngle(b->Rot);
	}
	else
	{
		b->OldB2 = b->Width;
		b->OldH2 = b->Height;
		if (b->Rot != 0)
		{
			FPoint npv = FPoint(newX.x(), newX.y());
			QWMatrix ma3;
			ma3.translate(b->Xpos, b->Ypos);
			ma3.rotate(b->Rot);
			double mxc3 = b->Xpos - (ma3.m11() * npv.x() + ma3.m21() * npv.y() + ma3.dx());
			double myc3 = b->Ypos - (ma3.m22() * npv.y() + ma3.m12() * npv.x() + ma3.dy());
			SizeItem(b->Width - newY.x(), b->Height - newY.y(), ite, fromMP, true, false);
			MoveItem(-mxc3, -myc3, b, fromMP);
		}
		else
		{
			SizeItem(b->Width - newY.x(), b->Height - newY.y(), ite, fromMP, true, false);
			MoveItem(newX.x(), newX.y(), b, fromMP);
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
	PageItem* b;
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
		b = SelItem.at(a);
		if (!fromMP)
		{
			p.begin(viewport());
			ToView(&p);
			p.translate(qRound(b->Xpos*Scale), qRound(b->Ypos*sc));
			p.scale(sc, sc);
			p.rotate(b->Rot);
			p.setRasterOp(XorROP);
			p.setBrush(NoBrush);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if ((b->itemType() != PageItem::Line) && (b->FrameType != 0) || (b->itemType() == PageItem::PolyLine))
				b->DrawPolyL(&p, b->Clip);
			else
				p.drawRect(0, 0, static_cast<int>(b->Width)+1, static_cast<int>(b->Height)+1);
			p.end();
		}
		MoveItem(x, y, b, fromMP);
		if (!fromMP)
		{
			p.begin(viewport());
			ToView(&p);
			p.translate(qRound(b->Xpos*sc), qRound(b->Ypos*sc));
			p.scale(sc, sc);
			p.rotate(b->Rot);
			p.setRasterOp(XorROP);
			p.setBrush(NoBrush);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if ((b->itemType() != PageItem::Line) && (b->FrameType != 0) || (b->itemType() == PageItem::PolyLine))
				b->DrawPolyL(&p, b->Clip);
			else
				p.drawRect(0, 0, static_cast<int>(b->Width)+1, static_cast<int>(b->Height)+1);
			p.end();
		}
	}
	if (GroupSel)
	{
		b = SelItem.at(0);
		GroupOnPage(b);
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
	PageItem* b;
	FPoint n;
	getGroupRect(&gxS, &gyS, &gwS, &ghS);
	QWMatrix ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(win);
	QRect oldR = QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10));
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		b = SelItem.at(a);
		n = FPoint(b->Xpos - RCenter.x(), b->Ypos - RCenter.y());
		b->Xpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		b->Ypos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		b->Rot += win;
		setRedrawBounding(b);
	}
	b = SelItem.at(0);
	GroupOnPage(b);
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
	PageItem *b;
	b = pi;
	if (b->locked())
		return;
	FPoint n;
	QRect oldR = getRedrawBounding(b);
	if ((Doc->RotMode != 0) && (b->itemType() != PageItem::Line))
	{
		QWMatrix ma;
		ma.translate(b->Xpos, b->Ypos);
		ma.scale(1, 1);
		ma.rotate(b->Rot);
		double ro = win-b->Rot;
		b->Rot = win;
		switch (Doc->RotMode)
		{
		case 2:
			ma.translate(b->Width/2, b->Height/2);
			n = FPoint(-b->Width/2, -b->Height/2);
			break;
		case 4:
			ma.translate(b->Width, b->Height);
			n = FPoint(-b->Width, -b->Height);
			break;
		case 3:
			ma.translate(0, b->Height);
			n = FPoint(0, -b->Height);
			break;
		case 1:
			ma.translate(b->Width, 0);
			n = FPoint(-b->Width, 0);
			break;
		}
		ma.rotate(ro);
		double x = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		double y = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		bool oldS = b->Sizing;
		b->Sizing = true;
		MoveItem(x-b->Xpos, y-b->Ypos, b);
		b->Sizing = oldS;
	}
	else
	{
		b->Rot = win;
		setRedrawBounding(b);
	}
	QRect newR = getRedrawBounding(b);
	updateContents(newR.unite(oldR));
	emit SetAngle(b->Rot);
}

void ScribusView::AdjustItemSize(PageItem *b)
{
	bool siz = b->Sizing;
	b->Sizing = false;
	FPointArray Clip;
	Clip = b->PoLine;
	FPoint tp2 = getMinClipF(&Clip);
	SizeItem(b->Width - tp2.x(), b->Height - tp2.y(), b->ItemNr, true, false, false);
	Clip.translate(-tp2.x(), -tp2.y());
	if (b->Rot != 0)
	{
		FPoint npv = FPoint(tp2.x(), tp2.y());
		MoveRotated(b, npv, true);
	}
	else
		MoveItem(tp2.x(), tp2.y(), b, true);
	if (!b->imageFlippedH())
		MoveItemI(-tp2.x()/b->LocalScX, 0, b->ItemNr, false);
	if (!b->imageFlippedV())
		MoveItemI(0, -tp2.y()/b->LocalScY, b->ItemNr, false);
	FPoint tp = getMaxClipF(&Clip);
	if (b->imageFlippedH())
		MoveItemI((b->Width - tp.x())/b->LocalScX, 0, b->ItemNr, false);
	if (b->imageFlippedV())
		MoveItemI(0, (b->Height - tp.y())/b->LocalScY, b->ItemNr, false);
	SizeItem(tp.x(), tp.y(), b->ItemNr, true, false);
	b->ClipEdited = true;
	b->PoLine = Clip.copy();
	if (b->itemType() == PageItem::PolyLine)
		SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
	else
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	b->Sizing = siz;
}

void ScribusView::AdvanceSel(PageItem *b, int oldPos, int len, int dir, int expandSel, int state)
{
	int i;
	int isSpace;
	if ( dir > 0 )
	{
		isSpace = b->itemText.at(oldPos)->ch.at(0).isSpace();
		b->CPos = oldPos +1;
		for (i=oldPos+1; i < len; i++)
		{
			if ( b->itemText.at(i)->ch.at(0).isSpace() == isSpace )
			{
				if ( expandSel )
				b->itemText.at(i)->cselect = state;
			}
			else
				break;
			b->CPos++;
		}
	}
	else if ( oldPos > 0 )
	{
		isSpace = b->itemText.at(oldPos-1)->ch.at(0).isSpace();
		for (i=oldPos-2; i > 0; i--)
		{
			if (  b->itemText.at(i)->ch.at(0).isSpace() == isSpace )
			{
				if ( expandSel )
				b->itemText.at(i)->cselect = state;
			}
			else
				break;
			b->CPos--;
		}
	}
}

// jjsa added on 15-mar-2004
// calculate the end position while ctrl + arrow pressed

void ScribusView::setNewPos(PageItem *b, int oldPos, int len, int dir)
{
	int i;
	int isSpace;
	if ( dir > 0 && oldPos < len )
	{
		isSpace = b->itemText.at(oldPos)->ch.at(0).isSpace();
		b->CPos = oldPos +1;
		for (i=oldPos+1; i < len; i++)
		{
			if ( b->itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			b->CPos++;
		}
	}
	else if ( dir < 0 && oldPos > 0 )
	{
		oldPos--;
		isSpace = b->itemText.at(oldPos)->ch.at(0).isSpace();
		for (i=oldPos; i >= 0; i--)
		{
			if (  b->itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			b->CPos--;
		}
	}
}

// jjsa added on 15-mar-2004 expand / decrease selection

// jjsa added on 14-mar-2004 text selection with pressed
// shift button and <-, -> cursor keys
// Parameters
//   PageItem *b text item to be processed
//   inc < 0 for left key > 0 for right key
//  if value is +/-1 work on slection
//  if value is +/-2 refresh if text under cursor is selected

void ScribusView::ExpandSel(PageItem *b, int dir, int oldPos)
{
	int len = b->itemText.count();
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
	if ( b->HasSel ) /* selection allready present */
	{
		if (dir > 0 && oldPos < len) // -> key
		{
			if ( oldPos == 0 )
				leftSel = false;
			else
				leftSel = b->itemText.at(oldPos-1)->cselect;
			actSel =   b->itemText.at(oldPos)->cselect;
			rightSel = false; // not relevant
		}
		else if ( dir > 0 && oldPos == len) // -> key
		{
			return;
		}
		else if ( dir < 0 && oldPos > 0 ) // <- key
		{
			actSel  =   b->itemText.at(oldPos-1)->cselect;
			leftSel = false; // not relevant
			if ( oldPos < len  )
				rightSel = b->itemText.at(oldPos)->cselect;
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
				b->itemText.at(i)->cselect = false;
			}
			b->HasSel = false;
			emit HasNoTextSel();
		}
		else if ( !selMode )
		{
			if (leftSel||actSel||rightSel)
				RefreshItem(b);
		}
	}
	if ( !selMode )
		return;
   // no selection
	if ( !b->HasSel )
	{
		b->HasSel = true;
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
		end   = b->CPos;
		sel = leftSel == true;
	}
	else
	{
		start = b->CPos;
		end   = oldPos;
		sel = rightSel == true;
	}
	for ( i = start; i < end; i++)
		b->itemText.at(i)->cselect = sel;
	if ( ! sel )
		emit  HasNoTextSel();
	RefreshItem(b);
}

void ScribusView::deselectAll(PageItem *b)
{
	PageItem *i = b;
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
	PageItem *b;
	if (GetItem(&b))
	{
		QRect mpo;
		int xP, yP;
		xP = qRound(x / Scale);
		yP = qRound(y / Scale);
		if (!((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::ImageFrame)))
			return false;
		QPainter p;
		QString chx;
		p.begin(viewport());
//		ToView(&p);
		Transform(b, &p);
		mpo = QRect(xP - Doc->guidesSettings.grabRad, yP - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
		if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))))).contains(mpo)) ||
		        (QRegion(p.xForm(b->Clip)).contains(mpo)))
		{
			if (b->itemType() == PageItem::ImageFrame)
				return true;
			TransformM(b, &p);
			uint a, i;
			int xp, yp, w, h, chs;
			for (a=0; a<b->itemText.count(); ++a)
			{
				xp = static_cast<int>(b->itemText.at(a)->xp);
				yp = static_cast<int>(b->itemText.at(a)->yp);
				chx = b->itemText.at(a)->ch;
				if (chx == QChar(30))
					chx = b->ExpandToken(a);
				if (chx == QChar(29))
					chx = " ";
				chs = b->itemText.at(a)->csize;
				b->SetZeichAttr(b->itemText.at(a), &chs, &chx);
				if ((chx == QChar(13)) || (chx == QChar(9)))
					w = 1;
				else
					w = qRound(Cwidth(Doc, b->itemText.at(a)->cfont, chx, chs)*(b->itemText.at(a)->cscale / 100.0));
				h = static_cast<int>(Doc->docParagraphStyles[b->itemText.at(a)->cab].LineSpa);
				if (QRegion(p.xForm(QRect(xp-1, yp-h, w+1, h))).contains(QPoint(xP, yP)))
				{
					b->CPos = a;
					p.end();
					Doc->CurrFont = b->itemText.at(a)->cfont;
					Doc->CurrFontSize = b->itemText.at(a)->csize;
					Doc->CurrTextFill = b->itemText.at(a)->ccolor;
					Doc->CurrTextFillSh = b->itemText.at(a)->cshade;
					Doc->CurrTextStroke = b->itemText.at(a)->cstroke;
					Doc->CurrTextStrokeSh = b->itemText.at(a)->cshade2;
					Doc->CurrTextScale = b->itemText.at(a)->cscale;
					emit ItemTextSca(b->itemText.at(a)->cscale);
					emit ItemTextFont(b->itemText.at(a)->cfont);
					emit ItemTextSize(b->itemText.at(a)->csize);
					emit ItemTextUSval(b->itemText.at(a)->cextra);
					emit ItemTextStil(b->itemText.at(a)->cstyle);
					emit ItemTextAbs(b->itemText.at(a)->cab);
					emit ItemTextFarben(b->itemText.at(a)->cstroke, b->itemText.at(a)->ccolor, b->itemText.at(a)->cshade2, b->itemText.at(a)->cshade);
					return true;
				}
			}
			QPoint np;
			for (a=0; a<b->itemText.count(); ++a)
			{
				xp = static_cast<int>(b->itemText.at(a)->xp);
				yp = static_cast<int>(b->itemText.at(a)->yp);
				np = p.xForm(QPoint(xp, yp - static_cast<int>(Doc->docParagraphStyles[b->itemText.at(a)->cab].LineSpa)));
				if (yP < np.y())
				{
					b->CPos = a;
					p.end();
					if (a > 0)
						i = a - 1;
					else
						i = a;
					Doc->CurrFont = b->itemText.at(i)->cfont;
					Doc->CurrFontSize = b->itemText.at(i)->csize;
					Doc->CurrTextFill = b->itemText.at(i)->ccolor;
					Doc->CurrTextFillSh = b->itemText.at(i)->cshade;
					Doc->CurrTextStroke = b->itemText.at(i)->cstroke;
					Doc->CurrTextStrokeSh = b->itemText.at(i)->cshade2;
					Doc->CurrTextScale = b->itemText.at(i)->cscale;
					emit ItemTextSca(b->itemText.at(i)->cscale);
					emit ItemTextFarben(b->itemText.at(i)->cstroke, b->itemText.at(i)->ccolor, b->itemText.at(i)->cshade2, b->itemText.at(i)->cshade);
					emit ItemTextFont(b->itemText.at(i)->cfont);
					emit ItemTextSize(b->itemText.at(i)->csize);
					emit ItemTextUSval(b->itemText.at(i)->cextra);
					emit ItemTextStil(b->itemText.at(i)->cstyle);
					emit ItemTextAbs(b->itemText.at(i)->cab);
					return true;
				}
			}
			b->CPos = b->itemText.count();
			if (b->itemText.count() != 0)
			{
				Doc->CurrFont = b->itemText.at(b->CPos-1)->cfont;
				Doc->CurrFontSize = b->itemText.at(b->CPos-1)->csize;
				Doc->CurrTextFill = b->itemText.at(b->CPos-1)->ccolor;
				Doc->CurrTextFillSh = b->itemText.at(b->CPos-1)->cshade;
				Doc->CurrTextStroke = b->itemText.at(b->CPos-1)->cstroke;
				Doc->CurrTextStrokeSh = b->itemText.at(b->CPos-1)->cshade2;
				Doc->CurrTextScale = b->itemText.at(b->CPos-1)->cscale;
				emit ItemTextSca(b->itemText.at(b->CPos-1)->cscale);
				emit ItemTextFarben(b->itemText.at(b->CPos-1)->cstroke, b->itemText.at(b->CPos-1)->ccolor, b->itemText.at(b->CPos-1)->cshade2, b->itemText.at(b->CPos-1)->cshade);
				emit ItemTextFont(b->itemText.at(b->CPos-1)->cfont);
				emit ItemTextSize(b->itemText.at(b->CPos-1)->csize);
				emit ItemTextUSval(b->itemText.at(b->CPos-1)->cextra);
				emit ItemTextStil(b->itemText.at(b->CPos-1)->cstyle);
				emit ItemTextAbs(b->itemText.at(b->CPos-1)->cab);
				p.end();
				return true;
			}
			else
			{
				Doc->CurrFont = b->IFont;
				Doc->CurrFontSize = b->ISize;
				Doc->CurrTextFill = b->TxtFill;
				Doc->CurrTextFillSh = b->ShTxtFill;
				Doc->CurrTextStroke = b->TxtStroke;
				Doc->CurrTextStrokeSh = b->ShTxtStroke;
				Doc->CurrTextScale = b->TxtScale;
				emit ItemTextSca(b->TxtScale);
				emit ItemTextFarben(b->TxtStroke, b->TxtFill, b->ShTxtStroke, b->ShTxtFill);
				emit ItemTextFont(b->IFont);
				emit ItemTextSize(b->ISize);
				emit ItemTextUSval(b->ExtraV);
				emit ItemTextStil(b->TxTStyle);
				emit ItemTextAbs(b->textAlignment);
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
	PageItem *b;
	if (GetItem(&b))
	{
		if (b->itemType() != PageItem::TextFrame)
			return;
		QPainter p;
		QString chx;
		int xp, yp, yp1, desc, asce;
		p.begin(viewport());
		ToView(&p);
		Transform(b, &p);
		TransformM(b, &p);
		if (b->CPos > 0)
		{
			offs = QMIN(b->CPos-1, static_cast<int>(b->itemText.count()-1));
			if (b->CPos < static_cast<int>(b->itemText.count()-1))
			{
				if (b->itemText.at(offs+1)->cstyle & 256)
				{
					if (b->CPos < static_cast<int>(b->itemText.count()-1))
					b->CPos++;
					return;
				}
			}
			if (b->itemText.at(offs)->yp == 0)
				return;
			chx = b->itemText.at(offs)->ch;
			if (chx == QChar(30))
				chx = b->ExpandToken(offs);
			if (chx == QChar(29))
				chx = " ";
			chs = b->itemText.at(offs)->csize;
			b->SetZeichAttr(b->itemText.at(offs), &chs, &chx);
			if (b->CPos != static_cast<int>(b->itemText.count()))
			{
				if (b->itemText.at(b->CPos)->ch == QChar(9))
				{
					xp = static_cast<int>(b->itemText.at(b->CPos-1)->xp);
					chs = b->itemText.at(b->CPos-1)->csize;
					chx = b->itemText.at(b->CPos-1)->ch;
					xp += qRound(Cwidth(Doc, b->itemText.at(b->CPos-1)->cfont, chx, chs)*(b->itemText.at(b->CPos-1)->cscale / 100.0));
				}
				else
					xp = static_cast<int>(b->itemText.at(offs+1)->xp);
			}
			else
			{
				xp = static_cast<int>(b->itemText.at(offs)->xp);
				if (b->itemText.at(offs)->ch != QChar(9))
				{
					chs = b->itemText.at(offs)->csize;
					chx = b->itemText.at(offs)->ch;
					xp += qRound(Cwidth(Doc, b->itemText.at(offs)->cfont, chx, chs)*(b->itemText.at(offs)->cscale / 100.0));
				}
			}
			if (b->CPos != static_cast<int>(b->itemText.count()))
			{
				if (b->itemText.at(offs)->yp != b->itemText.at(offs+1)->yp)
				{
					offs++;
					if ((b->itemText.at(offs)->ch == QChar(13)) || (b->itemText.at(offs)->ch == QChar(28)))
					{
						offs--;
						xp = static_cast<int>(b->itemText.at(offs)->xp);
						chs = b->itemText.at(offs)->csize;
						chx = b->itemText.at(offs)->ch;
						xp += qRound(Cwidth(Doc, b->itemText.at(offs)->cfont, chx, chs)*(b->itemText.at(offs)->cscale / 100.0));
					}
					else
						xp = static_cast<int>(b->itemText.at(offs)->xp);
				}
			}
			yp = static_cast<int>(b->itemText.at(offs)->yp);
			desc = static_cast<int>((*Doc->AllFonts)[b->itemText.at(offs)->cfont]->numDescender * (-b->itemText.at(offs)->csize / 10.0));
			asce = static_cast<int>((*Doc->AllFonts)[b->itemText.at(offs)->cfont]->numAscent * (b->itemText.at(offs)->csize / 10.0));
		}
		else
		{
			if (b->itemText.count() == 0)
			{
				double lineCorr;
				if (b->lineColor() != "None")
					lineCorr = b->Pwidth / 2.0;
				else
					lineCorr = 0;
				xp = static_cast<int>(b->Extra + lineCorr);
				yp = static_cast<int>(b->TExtra + lineCorr + b->LineSp);
				desc = static_cast<int>((*Doc->AllFonts)[b->IFont]->numDescender * (-b->ISize / 10.0));
				asce = static_cast<int>((*Doc->AllFonts)[b->IFont]->numAscent * (b->ISize / 10.0));
			}
			else
			{
				if (b->itemText.at(b->CPos)->ch == QChar(9))
				{
					double lineCorr;
					if (b->lineColor() != "None")
						lineCorr = b->Pwidth / 2.0;
					else
						lineCorr = 0;
					xp = static_cast<int>(b->Extra + lineCorr);
				}
				else
					xp = static_cast<int>(b->itemText.at(b->CPos)->xp);
				yp = static_cast<int>(b->itemText.at(b->CPos)->yp);
				desc = static_cast<int>((*Doc->AllFonts)[b->itemText.at(b->CPos)->cfont]->numDescender * (-b->itemText.at(b->CPos)->csize / 10.0));
				asce = static_cast<int>((*Doc->AllFonts)[b->itemText.at(b->CPos)->cfont]->numAscent * (b->itemText.at(b->CPos)->csize / 10.0));
			}
		}
		yp1 = yp - asce;
		yp += desc;
		p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
		p.setRasterOp(XorROP);
		if (draw)
		{
			p.drawLine(xp, QMIN(QMAX(yp1,0),static_cast<int>(b->Height)), xp, QMIN(QMAX(yp,0),static_cast<int>(b->Height)));
			CursVis = !CursVis;
			if (Doc->CurTimer != 0)
			{
				if (!Doc->CurTimer->isActive())
					Doc->CurTimer->start(500);
			}
		}
		else
		{
			if (CursVis)
				p.drawLine(xp, QMIN(QMAX(yp1,0),static_cast<int>(b->Height)), xp, QMIN(QMAX(yp,0),static_cast<int>(b->Height)));
			CursVis = false;
			if (Doc->CurTimer != 0)
				Doc->CurTimer->stop();
		}
		p.end();
	}
}

void ScribusView::BlinkCurs()
{
	slotDoCurs(true);
}

void ScribusView::HandleCurs(QPainter *p, PageItem *b, QRect mpo)
{
	QPoint tx, tx2;
	QWMatrix ma = p->worldMatrix();
	ma.setTransformationMode ( QWMatrix::Areas );
	p->setWorldMatrix(ma);
	tx = p->xForm(QPoint(static_cast<int>(b->Width), 0));
	tx2 = p->xForm(QPoint(0, static_cast<int>(b->Height)));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == Rotation)
			qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
		else
		{
			double rr = fabs(b->Rot);
			if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >=315.0) && (rr <= 360.0)))
				qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
			if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
				qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
		}
	}
	tx = p->xForm(QPoint(static_cast<int>(b->Width), static_cast<int>(b->Height)/2));
	tx2 = p->xForm(QPoint(0, static_cast<int>(b->Height)/2));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(b->Rot);
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
	}
	tx = p->xForm(QPoint(static_cast<int>(b->Width)/2, 0));
	tx2 = p->xForm(QPoint(static_cast<int>(b->Width)/2, static_cast<int>(b->Height)));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(b->Rot);
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
	}
	tx = p->xForm(QPoint(static_cast<int>(b->Width), static_cast<int>(b->Height)));
	tx2 = p->xForm(QPoint(0, 0));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == Rotation)
			qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
		else
		{
			double rr = fabs(b->Rot);
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

int ScribusView::OnPage(PageItem *b)
{
	int retw = -1;
	if (Doc->MasterP)
	{
		int x = static_cast<int>(Doc->currentPage->Xoffset);
		int y = static_cast<int>(Doc->currentPage->Yoffset);
		int w = static_cast<int>(Doc->currentPage->Width);
		int h = static_cast<int>(Doc->currentPage->Height);
		int x2 = static_cast<int>(b->Xpos);
		int y2 = static_cast<int>(b->Ypos);
		int w2 = static_cast<int>(b->Width);
		int h2 = static_cast<int>(b->Height);
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
			int x2 = static_cast<int>(b->Xpos);
			int y2 = static_cast<int>(b->Ypos);
			int w2 = static_cast<int>(b->Width);
			int h2 = static_cast<int>(b->Height);
			if (QRect(x, y, w, h).intersects(QRect(x2, y2, w2, h2)))
			{
				retw = a;
				break;
			}
		}
	}
	return retw;
}

void ScribusView::GroupOnPage(PageItem* b)
{
	QPtrList<PageItem> Objects;
	PageItem* item;
	if (b->Groups.count() == 0)
		return;
	int ObjGroup = b->Groups.top();
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
	PageItem *b = pi;
	if (!b->Select)
	{
		if (single)
		{
			SelItem.append(b);
			b->Select = true;
			b->isSingleSel = true;
			b->FrameOnly = true;
			b->paintObj();
		}
		else
		{
			if (b->Groups.count() != 0)
			{
				if (SelItem.count() != 0)
				{
					if (SelItem.find(b) == -1)
						SelItem.append(b);
				}
				else
					SelItem.append(b);
				for (uint ga=0; ga<Doc->Items.count(); ++ga)
				{
					if (Doc->Items.at(ga)->Groups.count() != 0)
					{
						if (Doc->Items.at(ga)->Groups.top() == b->Groups.top())
						{
							if (Doc->Items.at(ga)->ItemNr != b->ItemNr)
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
				SelItem.append(b);
				b->Select = true;
				b->FrameOnly = true;
				b->paintObj();
			}
		}
	}
	else
	{
		if (SelItem.count() > 1)
		{
			PageItem *bb = SelItem.at(0);
			SelItem.remove(b);
			SelItem.prepend(b);
			b->FrameOnly = true;
			b->paintObj();
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
			EmitValues(b);
		emit HaveSel(b->itemType());
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
	PageItem* b;
	Mpressed = true;
	Mxp = static_cast<int>(m->x()/Scale);
	Myp = static_cast<int>(m->y()/Scale);
	mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	ClRe = -1;
	if ((SelItem.count() != 0) && (m->state() == ControlButton))
		b = SelItem.at(0);
	else
		b = Doc->Items.last();
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
		b = Doc->currentPage->FromMaster.last();
		for (a = 0; a < Doc->currentPage->FromMaster.count(); ++a)
		{
			if (b->LayerNr == Doc->ActiveLayer)
			{
				p.begin(this);
				double OldX = b->Xpos;
				double OldY = b->Ypos;
				if (!b->ChangedMasterItem)
				{
					b->Xpos = OldX - Mp->Xoffset + Doc->currentPage->Xoffset;
					b->Ypos = OldY - Mp->Yoffset + Doc->currentPage->Yoffset;
				}
				Transform(b, &p);
				if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))))).contains(mpo)) ||
						(QRegion(p.xForm(b->Clip)).contains(mpo)))
				{
					if (!b->Select)
					{
						if ((m->state() != ShiftButton) || (Doc->appMode == LinkFrames) || (Doc->appMode == UnlinkFrames))
							Deselect(false);
						if (b->Groups.count() != 0)
						{
							if (SelItem.count() != 0)
							{
								if (SelItem.find(b) == -1)
									SelItem.append(b);
							}
							else
								SelItem.append(b);
							if (m->state() != (ControlButton | AltButton))
							{
								for (uint ga=0; ga<Doc->Items.count(); ++ga)
								{
									if (Doc->Items.at(ga)->Groups.count() != 0)
									{
										if (Doc->Items.at(ga)->Groups.top() == b->Groups.top())
										{
											if (Doc->Items.at(ga)->ItemNr != b->ItemNr)
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
								b->isSingleSel = true;
								b->Select = true;
								b->FrameOnly = true;
								b->paintObj();
							}
						}
						else
						{
							SelItem.append(b);
							b->Select = true;
							b->FrameOnly = true;
							b->paintObj();
						}
					}
					else
					{
						if (SelItem.count() > 1)
						{
							PageItem *bb = SelItem.at(0);
							SelItem.remove(b);
							SelItem.prepend(b);
							b->FrameOnly = true;
							b->paintObj();
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
						emit HaveSel(b->itemType());
					}
					else
					{
						EmitValues(b);
						if (b->itemType() == PageItem::Line)
							emit ItemGeom(b->Width, b->Height);
						emit HaveSel(b->itemType());
					}
					p.end();
					if (!b->ChangedMasterItem)
					{
						b->Xpos = OldX;
						b->Ypos = OldY;
					}
					return true;
				}
				p.end();
				if (!b->ChangedMasterItem)
				{
					b->Xpos = OldX;
					b->Ypos = OldY;
				}
			}
			b = Doc->currentPage->FromMaster.prev();
		}
	}
	if ((m->state() == (ControlButton | ShiftButton)) && (SelItem.count() != 0))
	{
		for (a = 0; a < Doc->Items.count(); ++a)
		{
			if (b->Select)
			{
				if (b->ItemNr == 0)
				{
					b = Doc->Items.last();
					break;
				}
				b = Doc->Items.prev();
				break;
			}
			b = Doc->Items.prev();
		}
	}
	for (a = 0; a < Doc->Items.count(); ++a)
	{
		if (b == NULL)
		{
			Deselect(true);
			SelItem.clear();
			return false;
		}
		if ((Doc->MasterP)  && (!((b->OwnPage == -1) || (b->OwnPage == static_cast<int>(Doc->currentPage->PageNr)))))
			continue;
		if (b->LayerNr == Doc->ActiveLayer)
		{
			p.begin(this);
			Transform(b, &p);
			if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))))).contains(mpo)) ||
			        (QRegion(p.xForm(b->Clip)).contains(mpo)))
			{
				if (!b->Select)
				{
					if ((m->state() != ShiftButton) || (Doc->appMode == LinkFrames) || (Doc->appMode == UnlinkFrames))
						Deselect(false);
					if (b->Groups.count() != 0)
					{
						if (SelItem.count() != 0)
						{
							if (SelItem.find(b) == -1)
								SelItem.append(b);
						}
						else
							SelItem.append(b);
						if (m->state() != (ControlButton | AltButton))
						{
							for (uint ga=0; ga<Doc->Items.count(); ++ga)
							{
								if (Doc->Items.at(ga)->Groups.count() != 0)
								{
									if (Doc->Items.at(ga)->Groups.top() == b->Groups.top())
									{
										if (Doc->Items.at(ga)->ItemNr != b->ItemNr)
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
							b->isSingleSel = true;
							b->Select = true;
							b->FrameOnly = true;
							b->paintObj();
						}
					}
					else
					{
						SelItem.append(b);
						b->Select = true;
						b->FrameOnly = true;
						b->paintObj();
					}
				}
				else
				{
					if (SelItem.count() > 1)
					{
						PageItem *bb = SelItem.at(0);
						SelItem.remove(b);
						SelItem.prepend(b);
						b->FrameOnly = true;
						b->paintObj();
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
					emit HaveSel(b->itemType());
				}
				else
				{
					EmitValues(b);
					b->paintObj();
					if (b->itemType() == PageItem::Line)
						emit ItemGeom(b->Width, b->Height);
					emit HaveSel(b->itemType());
				}
				if (SelItem.count() == 1)
				{
					HandleSizer(&p, b, mpo, m);
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
		b = Doc->Items.prev();
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

void ScribusView::HandleSizer(QPainter *p, PageItem *b, QRect mpo, QMouseEvent *m)
{
	b->OldB = b->Width;
	b->OldH = b->Height;
	b->OldB2 = b->Width;
	b->OldH2 = b->Height;
	HowTo = 0;
	if (b->sizeLocked())
		return;
	QRect ne = QRect();
	PaintSizeRect(p, ne);
	FPoint n1;
	double d1;
	QMap<double,int> distance;
	n1 = transformPoint( FPoint(b->Width, b->Height), b->Xpos, b->Ypos, b->Rot, 1, 1);
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 1);
	n1 = transformPoint( FPoint(0, 0), b->Xpos, b->Ypos, b->Rot, 1, 1);
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 2);
	if (b->itemType() != PageItem::Line)
	{
		n1 = transformPoint( FPoint(b->Width, 0), b->Xpos, b->Ypos, b->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 3);
		n1 = transformPoint( FPoint(0, b->Height), b->Xpos, b->Ypos, b->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 4);
		n1 = transformPoint( FPoint(b->Width/2, b->Height), b->Xpos, b->Ypos, b->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 5);
		n1 = transformPoint( FPoint(b->Width, b->Height/2), b->Xpos, b->Ypos, b->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 6);
		n1 = transformPoint( FPoint(0, b->Height/2), b->Xpos, b->Ypos, b->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 7);
		n1 = transformPoint( FPoint(b->Width/2, 0), b->Xpos, b->Ypos, b->Rot, 1, 1);
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 8);
	}
	QValueList<int> result = distance.values();
	if (result.count() != 0)
		HowTo = result[0];
	HandleCurs(p, b, mpo);
	if (HowTo != 0)
	{
		if (b->itemType() != PageItem::Line)
			b->Sizing = true;
		mCG = true;
	}
}

bool ScribusView::GetItem(PageItem **b, int nr)
{
	if (SelItem.count() != 0)
	{
		if (nr == -1)
			*(b) = SelItem.at(0);
		else
			*(b) = SelItem.at(nr);
		return true;
	}
	else
	{
		*(b) = 0;
		return false;
	}
}

void ScribusView::Deselect(bool prop)
{
	double x, y, w, h;
	PageItem* b;
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if ((b->itemType() == PageItem::TextFrame) && (b->isBookmark))
				emit ChBMText(b);
			if (b->itemType() == PageItem::ImageFrame)
				AdjustPreview(b, !Doc->DragP);
			b->Select = false;
			b->isSingleSel = false;
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
			updateContents(getRedrawBounding(b));
		}
		GroupSel = false;
	}
	if (prop)
		emit HaveSel(-1);
	ScApp->propertiesPalette->Cpal->gradEditButton->setOn(false);
}

void ScribusView::updateGradientVectors(PageItem *b)
{
	switch (b->GrType)
	{
		case 0:
		case 1:
			b->GrStartX = 0;
			b->GrStartY = b->Height / 2.0;
			b->GrEndX = b->Width;
			b->GrEndY = b->Height / 2.0;
			break;
		case 2:
			b->GrStartX = b->Width / 2.0;
			b->GrStartY = 0;
			b->GrEndX = b->Width / 2.0;
			b->GrEndY = b->Height;
			break;
		case 3:
			b->GrStartX = 0;
			b->GrStartY = 0;
			b->GrEndX = b->Width;
			b->GrEndY = b->Height;
			break;
		case 4:
			b->GrStartX = 0;
			b->GrStartY = b->Height;
			b->GrEndX = b->Width;
			b->GrEndY = 0;
			break;
		case 5:
			b->GrStartX = b->Width / 2.0;
			b->GrStartY = b->Height / 2.0;
			if (b->Width >= b->Height)
			{
				b->GrEndX = b->Width;
				b->GrEndY = b->Height / 2.0;
			}
			else
			{
				b->GrEndX = b->Width / 2.0;
				b->GrEndY = b->Height;
			}
			break;
		default:
			break;
	}
	ScApp->propertiesPalette->Cpal->setSpecialGradient(b->GrStartX * UmReFaktor, b->GrStartY * UmReFaktor,
																 b->GrEndX * UmReFaktor, b->GrEndY * UmReFaktor,
																 b->Width * UmReFaktor, b->Height * UmReFaktor);
}

void ScribusView::SetupDraw(int nr)
{
	PageItem* b = Doc->Items.at(nr);
	b->Select = true;
	b->IFont = Doc->toolSettings.defFont;
	b->ISize = Doc->toolSettings.defSize;
	mCG = true;
	HowTo = 1;
	qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
	SelItem.clear();
	SelItem.append(b);
	b->paintObj();
	Imoved = true;
	Doc->appMode = NormalMode;
	emit DocChanged();
	b->Sizing = b->itemType() == PageItem::Line ? false : true;
	EmitValues(b);
}

void ScribusView::EmitValues(PageItem *b)
{
	emit ItemPos(b->Xpos, b->Ypos);
	emit ItemGeom(b->Width, b->Height);
	emit SetAngle(b->Rot);
	emit SetSizeValue(b->Pwidth);
	emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
	emit SetLocalValues(b->LocalScX, b->LocalScY, b->LocalX, b->LocalY );
	emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
	emit ItemGradient(b->GrType);
	emit ItemTrans(b->fillTransparency(), b->lineTransparency());
	emit ItemTextAttr(b->LineSp);
	emit ItemTextUSval(b->ExtraV);
	emit SetDistValues(b->Extra, b->TExtra, b->BExtra, b->RExtra);
	emit ItemTextCols(b->Cols, b->ColGap);
	if (Doc->appMode != EditMode)
	{
		emit ItemTextAbs(b->textAlignment);
		emit ItemTextFont(b->IFont);
		emit ItemTextSize(b->ISize);
	}
	ScApp->propertiesPalette->Cpal->setSpecialGradient(b->GrStartX * UmReFaktor, b->GrStartY * UmReFaktor,
																 b->GrEndX * UmReFaktor, b->GrEndY * UmReFaktor,
																 b->Width * UmReFaktor, b->Height * UmReFaktor);
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
	for (uint a = 0; a < Doc->Items.count(); ++a)
	{
		PageItem *b = Doc->Items.at(a);
		if ((b->PicAvail) && (b->Pfile == name))
		{
			bool fho = b->imageFlippedH();
			bool fvo = b->imageFlippedV();
			b->pixmOrg = QImage();
			LoadPict(b->Pfile, b->ItemNr, true);
			b->setImageFlippedH(fho);
			b->setImageFlippedV(fvo);
			AdjustPictScale(b);
			AdjustPreview(b, false);
		}
	}
	for (uint a = 0; a < Doc->MasterItems.count(); ++a)
	{
		PageItem *b = Doc->MasterItems.at(a);
		if ((b->PicAvail) && (b->Pfile == name))
		{
			bool fho = b->imageFlippedH();
			bool fvo = b->imageFlippedV();
			b->pixmOrg = QImage();
			LoadPict(b->Pfile, b->ItemNr, true);
			b->setImageFlippedH(fho);
			b->setImageFlippedV(fvo);
			AdjustPictScale(b);
			AdjustPreview(b, false);
		}
	}
	updateContents();
	emit DocChanged();
}

void ScribusView::removePict(QString name)
{
	for (uint a = 0; a < Doc->DocItems.count(); ++a)
	{
		PageItem *b = Doc->DocItems.at(a);
		if ((b->PicAvail) && (b->Pfile == name))
		{
			b->PicAvail = false;
			b->pixm = QImage();
			b->pixmOrg = QImage();
/*			if (b->itemType() == PageItem::ImageFrame)
				emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr); */
		}
	}
	for (uint a = 0; a < Doc->MasterItems.count(); ++a)
	{
		PageItem *b = Doc->MasterItems.at(a);
		if ((b->PicAvail) && (b->Pfile == name))
		{
			b->PicAvail = false;
			b->pixm = QImage();
			b->pixmOrg = QImage();
		}
	}
	updateContents();
	emit DocChanged();
}

void ScribusView::UpdatePic()
{
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		if (b->PicAvail)
		{
			int fho = b->imageFlippedH();
			int fvo = b->imageFlippedV();
			b->pixmOrg = QImage();
			LoadPict(b->Pfile, b->ItemNr, true);
			b->setImageFlippedH(fho);
			b->setImageFlippedV(fvo);
			AdjustPictScale(b);
			AdjustPreview(b, false);
			updateContents();
		}
	}
}

void ScribusView::FrameToPic()
{
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		if (b->PicAvail)
		{
			AdjustPreview(b, false);
			double w = static_cast<double>(b->pixm.width());
			double h = static_cast<double>(b->pixm.height());
			if (b->LocalViewX > 1.00)
				w *= b->LocalViewX;
			if (b->LocalViewY > 1.00)
				h *= b->LocalViewY;
			double x = b->LocalX * b->LocalScX;
			double y = b->LocalY * b->LocalScY;
			if (!b->isTableItem)
			{
				SizeItem(w, h, b->ItemNr);
				MoveItem(x, y, b);
				b->LocalX = 0;
				b->LocalY = 0;
			}
			updateContents();
			emit DocChanged();
		}
	}
}

void ScribusView::ToggleBookmark()
{
	uint a;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			bool old = SelItem.at(a)->isBookmark;
			SelItem.at(a)->isBookmark = !SelItem.at(a)->isBookmark;
			if (SelItem.at(a)->isBookmark)
			{
				SelItem.at(a)->isAnnotation = false;
				emit AddBM(SelItem.at(a));
			}
			else
			{
				if (old)
					emit DelBM(SelItem.at(a));
			}
		}
		emit DocChanged();
	}
}

void ScribusView::ToggleAnnotation()
{
	uint a;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			bool old = SelItem.at(a)->isBookmark;
			SelItem.at(a)->isAnnotation = !SelItem.at(a)->isAnnotation;
			if (SelItem.at(a)->isAnnotation)
			{
				if (old)
					emit DelBM(SelItem.at(a));
				SelItem.at(a)->isBookmark = false;
			}
		}
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

void ScribusView::ToggleResize()
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
	int d = pmen3->indexOf(id);
	int dd = Doc->Layers[d].LNr;
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			PageItem *b = SelItem.at(a);
			b->LayerNr = dd;
		}
	}
	Deselect(true);
	updateContents();
	emit DocChanged();
}

void ScribusView::LoremIpsum()
{
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		if (b->itemText.count() != 0)
		{
			int t = QMessageBox::warning(this, tr("Warning"),
	                             tr("Do you really want to clear all your Text?"),
	                             QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
			if (t == QMessageBox::No)
				return;
		}
		QString pfad = SAMPLESDIR;
		QString pfad2;
		pfad2 = pfad + "LoremIpsum.txt";
		Serializer *ss = new Serializer(pfad2);
		if (ss->Read(""))
		{
			int st = Doc->currentParaStyle;
			if (st > 5)
				ss->GetText(b, st, Doc->docParagraphStyles[st].Font, Doc->docParagraphStyles[st].FontSize, true);
			else
				ss->GetText(b, st, b->IFont, b->ISize, true);
		}
		delete ss;
		if (Doc->docHyphenator->AutoCheck)
			Doc->docHyphenator->slotHyphenate(b);
		updateContents();
		emit DocChanged();
	}
}

void ScribusView::ToBack()
{
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *b;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			b = SelItem.at(c);
			if (b->isTableItem && b->isSingleSel)
				return;
			ObjOrder.insert(b->ItemNr, c);
			d = Doc->Items.findRef(b);
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
		if (Doc->TemplateMode)
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
	PageItem *b;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			b = SelItem.at(c);
			if (b->isTableItem && b->isSingleSel)
				return;
			ObjOrder.insert(b->ItemNr, c);
			d = Doc->Items.findRef(b);
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
		if (Doc->TemplateMode)
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
	PageItem *b;
	PageItem *b2;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			b = SelItem.at(c);
			if (b->isTableItem && b->isSingleSel)
				return;
			low = QMIN(b->ItemNr, low);
			high = QMAX(b->ItemNr, high);
		}
		if (low == 0)
			return;
		b2 = Doc->Items.at(high);
		SelItem.clear();
		SelectItemNr(low-1, false);
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			b = SelItem.at(c);
			b->Select = false;
			ObjOrder.insert(b->ItemNr, c);
			d = Doc->Items.findRef(b);
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
		if (Doc->TemplateMode)
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
	PageItem *b;
	PageItem *b2;
	if ((Doc->Items.count() > 1) && (SelItem.count() != 0))
	{
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			b = SelItem.at(c);
			if (b->isTableItem && b->isSingleSel)
				return;
			low = QMIN(b->ItemNr, low);
			high = QMAX(b->ItemNr, high);
		}
		if (high == Doc->Items.count()-1)
			return;
		b2 = Doc->Items.at(low);
		SelItem.clear();
		SelectItemNr(high+1, false);
		for (uint c = 0; c < SelItem.count(); ++c)
		{
			b = SelItem.at(c);
			b->Select = false;
			ObjOrder.insert(b->ItemNr, c);
			d = Doc->Items.findRef(b);
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
		if (Doc->TemplateMode)
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
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->convertTo(PageItem::ImageFrame);
	b->Frame = true;
	RefreshItem(b);
	emit HaveSel(b->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr);
	EmitValues(b);
	emit DocChanged();
}

void ScribusView::ToPolyFrame()
{
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->convertTo(PageItem::Polygon);
	b->Frame = false;
	b->ClipEdited = true;
	b->FrameType = 3;
	b->Clip = FlattenPath(b->PoLine, b->Segments);
	b->ContourLine = b->PoLine.copy();
	RefreshItem(b);
	emit HaveSel(b->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr);
	EmitValues(b);
	emit DocChanged();
}

void ScribusView::ToTextFrame()
{
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->convertTo(PageItem::TextFrame);
	b->Frame = true;
	RefreshItem(b);
	emit HaveSel(b->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr);
	EmitValues(b);
	emit DocChanged();
}

void ScribusView::ToBezierFrame()
{
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->convertTo(PageItem::PolyLine);
	b->ClipEdited = true;
	SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
	AdjustItemSize(b);
	RefreshItem(b);
	emit HaveSel(b->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr);
	EmitValues(b);
	emit DocChanged();
}

void ScribusView::Bezier2Poly()
{
	PageItem *b = SelItem.at(0);
	b->convertTo(PageItem::Polygon);
	b->Frame = false;
	b->ClipEdited = true;
	b->FrameType = 3;
	b->PoLine.addPoint(b->PoLine.point(b->PoLine.size()-2));
	b->PoLine.addPoint(b->PoLine.point(b->PoLine.size()-3));
	b->PoLine.addPoint(b->PoLine.point(0));
	b->PoLine.addPoint(b->PoLine.point(0));
	b->Clip = FlattenPath(b->PoLine, b->Segments);
	b->ContourLine = b->PoLine.copy();
	RefreshItem(b);
	emit HaveSel(b->itemType());
	if (!Doc->loading)
		emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr);
	EmitValues(b);
	emit DocChanged();
}

void ScribusView::ClearItem()
{
	PageItem *b;
	if (SelItem.count() != 0)
	{
		b = SelItem.at(0);
		if ((b->itemType() == PageItem::ImageFrame) || (b->itemType() == PageItem::TextFrame))
		{
			if (b->itemType() == PageItem::TextFrame)
			{
				if ((b->itemText.count() != 0) && ((b->NextBox == 0) || (b->BackBox == 0)))
				{
					int t = QMessageBox::warning(this, tr("Warning"),
										tr("Do you really want to clear all your Text?"),
										QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
					if (t == QMessageBox::No)
						return;
				}
				PageItem *nb = b;
				while (nb != 0)
				{
					if (nb->BackBox != 0)
						nb = nb->BackBox;
					else
						break;
				}
				while (nb != 0)
				{
					nb->itemText.clear();
					nb->CPos = 0;
					nb = nb->NextBox;
				}
			}
			if ((b->itemType() == PageItem::ImageFrame) && ((ScApp->fileWatcher->files().contains(b->Pfile) != 0) && (b->PicAvail)))
				ScApp->fileWatcher->removeFile(b->Pfile);
			b->Pfile = "";
			b->PicAvail = false;
			b->pixm = QImage();
			b->pixmOrg = QImage();
			if (b->itemType() == PageItem::ImageFrame)
			{
				b->LocalScX = 1;
				b->LocalScY = 1;
				b->LocalViewX = 1;
				b->LocalViewY = 1;
				b->OrigW = 0;
				b->OrigH = 0;
				b->dpiX = 72.0;
				b->dpiY = 72.0;
				b->LocalX = 0;
				b->LocalY = 0;
				b->setImageFlippedH(false);
				b->setImageFlippedV(false);
				b->textAlignment = 0;
				b->EmProfile = "";
				b->ScaleType = true;
				b->AspectRatio = true;
				b->setFillTransparency(0.0);
				b->setLineTransparency(0.0);
				b->InvPict = false;
/*				emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr); */
			}
			updateContents();
			emit DocChanged();
		}
	}
}

void ScribusView::DeleteItem()
{
	uint a, c, itnr, anz;
	QPtrList<PageItem> delItems;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		anz = SelItem.count();
		uint offs = 0;
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint de = 0; de < anz; ++de)
		{
			b = SelItem.at(offs);
			if ((b->isTableItem && b->isSingleSel) || (b->locked()))
			{
				offs++;
				continue;
			}
			tooltip += "\t" + b->getUName() + "\n";
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
			b = delItems.at(0);
			if ((b->itemType() == PageItem::ImageFrame) && ((ScApp->fileWatcher->files().contains(b->Pfile) != 0) && (b->PicAvail)))
				ScApp->fileWatcher->removeFile(b->Pfile);
			if (b->itemType() == PageItem::TextFrame)
			{
				if ((b->NextBox != 0) || (b->BackBox != 0))
				{
					if (b->BackBox == 0)
					{
						b->NextBox->BackBox = b->BackBox;
						c = b->itemText.count();
						for (a = 0; a < c; ++a)
							b->NextBox->itemText.prepend(b->itemText.take(b->itemText.count()-1));
						if ((b->isAutoText) && (b->NextBox == 0))
							Doc->LastAuto = 0;
					}
					else
					{
						b->BackBox->NextBox = b->NextBox;
						if (b->NextBox != 0)
							b->NextBox->BackBox = b->BackBox;
						else
						{
							if (b->isAutoText)
								Doc->LastAuto = b->BackBox;
						}
						c = b->itemText.count();
						for (a = 0; a < c; ++a)
							b->BackBox->itemText.append(b->itemText.take(0));
					}
				}
				else
				{
					if (b->isAutoText)
					{
						Doc->LastAuto = 0;
						Doc->FirstAuto = 0;
					}
				}
			}
			if (b->isBookmark)
				emit DelBM(SelItem.at(0));
//			if (anz != 1)
//			{
				Doc->Items.remove(b->ItemNr);
				b->Select = false;
// 				delete b;
//			}
//			emit DelObj(PageNr, SelItem.at(0)->ItemNr);
			delItems.removeFirst();
			for (a = 0; a < Doc->Items.count(); ++a)
			{
				itnr = Doc->Items.at(a)->ItemNr;
				Doc->Items.at(a)->ItemNr = a;
				if (Doc->Items.at(a)->isBookmark)
					emit NewBMNr(Doc->Items.at(a)->BMnr, a);
				for (uint dxx=0; dxx<delItems.count(); ++dxx)
				{
					if (delItems.at(dxx)->ItemNr == itnr)
						delItems.at(dxx)->ItemNr = a;
				}
			}
			// send the undo action to the UndoManager
			if (UndoManager::undoEnabled())
			{
				ItemState<PageItem*> *is =
						new ItemState<PageItem*>(Um::Delete + " " + b->getUName(), "", Um::IDelete);
				is->setItem(b);
				is->set("DELETE_ITEM", "delete_item");
				UndoObject *target;
				if (b->OwnPage > -1)
					target = Doc->Pages.at(b->OwnPage);
				else
					target = Doc->Pages.at(0);
				undoManager->action(target, is, b->getUPixmap());
			}
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
		if (Doc->TemplateMode)
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
		SB1->setGeometry(x+60+sadj, y, 15, h);
		SB2->setGeometry(x+75+sadj, y, 15, h);
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
}

void ScribusView::Zval()
{
	int x = qRound(QMAX(contentsX() / Scale, 0));
	int y = qRound(QMAX(contentsY() / Scale, 0));
	int w = qRound(QMIN(visibleWidth() / Scale, Doc->PageB));
	int h = qRound(QMIN(visibleHeight() / Scale, Doc->PageH));
	rememberPreviousSettings(w / 2 + x,h / 2 + y);
	Scale = LE->value() / 100.0 * Prefs->DisScale;
	slotDoZoom();
	ScApp->setFocus();
}

/** Adds a Page */
Page* ScribusView::addPage(int nr)
{
	Page* fe = new Page(Doc->ScratchLeft, Doc->PageC*(Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop, Doc->PageB, Doc->PageH);
	fe->Margins.Top = Doc->PageM.Top;
	fe->Margins.Bottom = Doc->PageM.Bottom;
	fe->setPageNr(nr);
	Doc->Pages.insert(nr, fe);
	Doc->currentPage = fe;
	Doc->PageC++;
	PGS->setMaxValue(Doc->PageC);
	reformPages();
	if ((Doc->PageAT) && (!Doc->loading))
	{
		int z = PaintText(fe->Margins.Left+fe->Xoffset,
		                  fe->Margins.Top+fe->Yoffset,
		                  Doc->PageB-fe->Margins.Right-fe->Margins.Left,
		                  Doc->PageH-fe->Margins.Bottom-fe->Margins.Top,
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
	if (Doc->PageC == 1)
		return;
	if (Doc->PageC < Nr-1)
		return;
	Page* Seite;
	Seite = Doc->Pages.at(Nr);
	PageItem* b;
	for (b = Seite->FromMaster.first(); b; b = Seite->FromMaster.next())
	{
		if (b->ChangedMasterItem)
		{
			Seite->FromMaster.remove(b);
			delete b;
		}
	}
	Seite->FromMaster.clear();
	Doc->Pages.remove(Nr);
	delete Seite;
	Doc->PageC -= 1;
	Doc->currentPage = Doc->Pages.at(0);
	if (Doc->TemplateMode)
		Doc->MasterPages = Doc->Pages;
	else
		Doc->DocPages = Doc->Pages;
	PGS->setMaxValue(Doc->PageC);
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
	if (Doc->TemplateMode)
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
					Seite->Margins.Left = Doc->PageM.Right;
					Seite->Margins.Right = Doc->PageM.Left;
				}
				else
				{
					Seite->Margins.Right = Doc->PageM.Right;
					Seite->Margins.Left = Doc->PageM.Left;
				}
			}
			else
			{
				if (a % 2 == 0)
				{
					if (Doc->FirstPageLeft)
					{
						Seite->Margins.Left = Doc->PageM.Right;
						Seite->Margins.Right = Doc->PageM.Left;
					}
					else
					{
						Seite->Margins.Right = Doc->PageM.Right;
						Seite->Margins.Left = Doc->PageM.Left;
					}
				}
				else
				{
					if (Doc->FirstPageLeft)
					{
						Seite->Margins.Right = Doc->PageM.Right;
						Seite->Margins.Left = Doc->PageM.Left;
					}
					else
					{
						Seite->Margins.Left = Doc->PageM.Right;
						Seite->Margins.Right = Doc->PageM.Left;
					}
				}
			}
		}
		else
		{
			Seite->Margins.Right = Doc->PageM.Right;
			Seite->Margins.Left = Doc->PageM.Left;
		}
		Seite->Margins.Top = Doc->PageM.Top;
		Seite->Margins.Bottom = Doc->PageM.Bottom;
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
						Seite->Yoffset = a/2 * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
					else
					{
						Seite->Xoffset = Doc->ScratchLeft + Doc->PageB;
						Seite->Yoffset = (a+1)/2 * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
				}
				else
				{
					if (Doc->FirstPageLeft)
					{
						Seite->Xoffset = Doc->ScratchLeft + Doc->PageB;
						Seite->Yoffset = a/2 * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
					else
					{
						Seite->Xoffset = Doc->ScratchLeft;
						Seite->Yoffset = (a+1)/2 * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
					}
				}
			}
			else
			{
				Seite->Xoffset = Doc->ScratchLeft;
				Seite->Yoffset = a * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop)+Doc->ScratchTop;
			}
		}
	}
	for (uint ite = 0; ite < Doc->Items.count(); ++ite)
	{
		PageItem *item = Doc->Items.at(ite);
		oldPg = pageTable[item->OwnPage];
		item->Xpos = item->Xpos - oldPg.oldXO + Doc->Pages.at(oldPg.newPg)->Xoffset;
		item->Ypos = item->Ypos - oldPg.oldYO + Doc->Pages.at(oldPg.newPg)->Yoffset;
		item->OwnPage = static_cast<int>(oldPg.newPg);
		setRedrawBounding(item);
	}
	if (Doc->PageFP)
	{
		if (Doc->FirstPageLeft)
			resizeContents(qRound((Doc->PageB*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(((Doc->PageC-1)/2 + 1) * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
		else
			resizeContents(qRound((Doc->PageB*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound((Doc->PageC/2 + 1) * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop * Scale)));
	}
	else
		resizeContents(qRound((Doc->PageB+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(Doc->PageC * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
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
	PGS->setMaxValue(Doc->PageC);
	if ((!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(Seite);
	connect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
}

void ScribusView::setLayMenTxt(int l)
{
	QValueList<Layer>::iterator it;
	QString lName;
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
			resizeContents(qRound((Doc->PageB*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(((Doc->PageC-1)/2 + 1) * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
		else
			resizeContents(qRound((Doc->PageB*2+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound((Doc->PageC/2 + 1) * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
	}
	else
		resizeContents(qRound((Doc->PageB+Doc->ScratchLeft+Doc->ScratchRight) * Scale), qRound(Doc->PageC * (Doc->PageH+Doc->ScratchBottom+Doc->ScratchTop) * Scale));
	setRulerPos(contentsX(), contentsY());
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		SetCCPo(static_cast<int>(b->Xpos + b->Width/2), static_cast<int>(b->Ypos + b->Height/2));
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
		int w = qRound(QMIN(visibleWidth() / Scale, Doc->PageB));
		int h = qRound(QMIN(visibleHeight() / Scale, Doc->PageH));
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
		int w = qRound(QMIN(visibleWidth() / Scale, Doc->PageB));
		int h = qRound(QMIN(visibleHeight() / Scale, Doc->PageH));
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
 	Laymen->clear();
 	if (Doc->Layers.count() != 0)
 	{
		for (a=0; a < Doc->Layers.count(); a++)
 		{
			int curr=0;
			int n=0;
			for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it,n++)
			{
				if (Doc->Layers.count()-(*it).Level-1 == a)
				{
					Laymen->insertItem((*it).Name);
					if (Doc->ActiveLayer == n)
						curr=Doc->Layers.count()-(*it).Level-1;
				}
			}
			Laymen->setItemChecked(Laymen->idAt(curr), true);
 		}
 	}
}

void ScribusView::GotoLa(int l)
{
	int a=0;
	int d = Doc->Layers.count()-Laymen->indexOf(l)-1;
	QValueList<Layer>::iterator it;
	for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it,a++)
	{
		if ( d == (*it).Level )
		{
			Doc->ActiveLayer = a;
			LY->setText(Doc->Layers[a].Name);
			emit changeLA(a);
			break;
		}
	}
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
	PGS->setMaxValue(Doc->PageC);
	if ((!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(Seite);
}

void ScribusView::ShowTemplate(int nr)
{
	OldScale = Scale;
	if (!Doc->MasterP)
	{
		Doc->DocPages = Doc->Pages;
		Doc->Pages = Doc->MasterPages;
		Doc->DocItems = Doc->Items;
		Doc->Items = Doc->MasterItems;
	}
	Doc->PageC = 1;
	Doc->MasterP = true;
	Doc->currentPage = Doc->Pages.at(nr);
	PGS->setEnabled(false);
	updateOn = false;
	slotDoZoom();
	oldX = qRound(Doc->currentPage->Xoffset - 10);
	oldY = qRound(Doc->currentPage->Yoffset - 10);
	SetCPo(oldX, oldY);
	updateOn = true;
	DrawNew();
}

void ScribusView::HideTemplate()
{
	Deselect(true);
	Doc->MasterItems = Doc->Items;
	Doc->Items = Doc->DocItems;
	Doc->MasterPages = Doc->Pages;
	Doc->PageC = Doc->DocPages.count();
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
					LoadPict(it->Pfile, i);
				else
				{
					it->IProfile = Doc->CMSSettings.DefaultInputProfile;
					LoadPict(it->Pfile, i);
				}
				it->Sizing = true;
				AdjustPreview(it);
				it->Sizing = false;
			}
			counter++;
			if (dia != NULL)
				dia->setProgress(counter);
		}
	}
}

QPixmap ScribusView::MPageToPixmap(QString name, int maxGr)
{
	QPixmap pm = QPixmap();
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
		Doc->MasterP = true;
		Page* act = Doc->currentPage;
		Doc->currentPage = Doc->MasterPages.at(Nr);
		if (!mas)
		{
			Doc->DocItems = Doc->Items;
			Doc->Items = Doc->MasterItems;
		}
		Doc->guidesSettings.framesShown = false;
		Scale = 1;
		pm = QPixmap(clipw, cliph);
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
		QImage im2;
		QImage im = pm.convertToImage();
		double sx = im.width() / static_cast<double>(maxGr);
		double sy = im.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im2 = im.smoothScale(static_cast<int>(im.width() / sx), static_cast<int>(im.height() / sx));
		else
			im2 = im.smoothScale(static_cast<int>(im.width() / sy), static_cast<int>(im.height() / sy));
		pm.convertFromImage(im2);
		im2.detach();
		delete painter;
	}
	return pm;
}

QPixmap ScribusView::PageToPixmap(int Nr, int maxGr)
{
	QPixmap pm = QPixmap();
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
		Page* act = Doc->currentPage;
		Doc->currentPage = Doc->Pages.at(Nr);
		pm = QPixmap(clipw, cliph);
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
		QImage im2;
		QImage im = pm.convertToImage();
		double sx = im.width() / static_cast<double>(maxGr);
		double sy = im.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im2 = im.smoothScale(static_cast<int>(im.width() / sx), static_cast<int>(im.height() / sx));
		else
			im2 = im.smoothScale(static_cast<int>(im.width() / sy), static_cast<int>(im.height() / sy));
		pm.convertFromImage(im2);
		im2.detach();
		delete painter;
	}
	return pm;
}

void ScribusView::FromHRuler(QMouseEvent *m)
{
	if (Doc->guidesSettings.guidesShown)
	{
		QPoint py = viewport()->mapFromGlobal(m->globalPos());
		int newY = py.y();
		emit MousePos(m->x()/Scale-Doc->currentPage->Xoffset, m->y()/Scale-Doc->currentPage->Yoffset);
		horizRuler->Draw(m->x());
		vertRuler->Draw(m->y());
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
		emit MousePos(m->x()/Scale-Doc->currentPage->Xoffset, m->y()/Scale-Doc->currentPage->Yoffset);
		horizRuler->Draw(m->x());
		vertRuler->Draw(m->y());
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
	QPoint out = viewportToContents(py);
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
	QPoint out = viewportToContents(py);
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

void ScribusView::Transform(PageItem *b, QPainter *p)
{
	p->translate(static_cast<int>(b->Xpos*Scale), static_cast<int>(b->Ypos*Scale));
	p->scale(Scale, Scale);
	p->rotate(b->Rot);
}

void ScribusView::TransformM(PageItem *b, QPainter *p)
{
	if (b->imageFlippedH())
	{
		p->translate(b->Width, 0);
		p->scale(-1, 1);
	}
	if (b->imageFlippedV())
	{
		p->translate(0, b->Height);
		p->scale(1, -1);
	}
}

void ScribusView::SetFrameShape(PageItem *b, int count, double *vals)
{
	b->PoLine.resize(0);
	for (int a = 0; a < count-3; a += 4)
	{
		if (vals[a] < 0)
		{
			b->PoLine.setMarker();
			continue;
		}
		double x1 = b->Width * vals[a] / 100.0;
		double y1 = b->Height * vals[a+1] / 100.0;
		double x2 = b->Width * vals[a+2] / 100.0;
		double y2 = b->Height * vals[a+3] / 100.0;
		b->PoLine.addPoint(x1, y1);
		b->PoLine.addPoint(x2, y2);
	}
	b->Clip = FlattenPath(b->PoLine, b->Segments);
	b->ClipEdited = true;
	setRedrawBounding(b);
}

void ScribusView::SetRectFrame(PageItem *b)
{
	double rect[] = {   0.0,   0.0,   0.0,   0.0,
	                    100.0,   0.0, 100.0,   0.0,
	                    100.0,   0.0, 100.0,   0.0,
	                    100.0, 100.0, 100.0, 100.0,
	                    100.0, 100.0, 100.0, 100.0,
	                    0.0, 100.0,   0.0, 100.0,
	                    0.0, 100.0,   0.0, 100.0,
	                    0.0,   0.0,   0.0,   0.0};
	SetFrameShape(b, 32, rect);
	b->ClipEdited = false;
	b->FrameType = 0;
}

void ScribusView::SetOvalFrame(PageItem *b)
{
	double rect[] = { 100.0,  50.0, 100.0,       77.615235,
	                  50.0, 100.0,  77.615235, 100.0,
	                  50.0, 100.0,  22.385765, 100.0,
	                  0.0,  50.0,   0.0,       77.615235,
	                  0.0,  50.0,   0.0,       22.385765,
	                  50.0,   0.0,  22.385765,   0.0,
	                  50.0,   0.0,  77.615235,   0.0,
	                  100.0,  50.0, 100.0,       22.385765};
	SetFrameShape(b, 32, rect);
	b->FrameType = 1;
	b->ClipEdited = false;
}

void ScribusView::SetFrameRound(PageItem* b)
{
	ClRe = -1;
	b->RadRect = QMIN(b->RadRect, QMIN(b->Width,b->Height)/2);
	b->PoLine.resize(0);
	double rr = fabs(b->RadRect);
	if (b->RadRect > 0)
	{
		b->PoLine.addQuadPoint(rr, 0, rr, 0, b->Width-rr, 0, b->Width-rr, 0);
		b->PoLine.addQuadPoint(b->Width-rr, 0, b->Width-rr+rr*0.552284749, 0, b->Width, rr, b->Width, rr*0.552284749);
		b->PoLine.addQuadPoint(b->Width, rr, b->Width, rr, b->Width, b->Height-rr, b->Width, b->Height-rr);
		b->PoLine.addQuadPoint(b->Width, b->Height-rr, b->Width, b->Height-rr+rr*0.552284749, b->Width-rr, b->Height, b->Width-rr+rr*0.552284749, b->Height);
		b->PoLine.addQuadPoint(b->Width-rr, b->Height, b->Width-rr, b->Height, rr, b->Height, rr, b->Height);
		b->PoLine.addQuadPoint(rr, b->Height, rr*0.552284749, b->Height, 0, b->Height-rr, 0, b->Height-rr+rr*0.552284749);
		b->PoLine.addQuadPoint(0, b->Height-rr, 0, b->Height-rr, 0, rr, 0, rr);
		b->PoLine.addQuadPoint(0, rr, 0, rr*0.552284749, rr, 0, rr*0.552284749, 0);
	}
	else
	{
		b->PoLine.addQuadPoint(rr, 0, rr, 0, b->Width-rr, 0, b->Width-rr, 0);
		b->PoLine.addQuadPoint(b->Width-rr, 0, b->Width-rr, rr*0.552284749, b->Width, rr, b->Width-rr*0.552284749, rr);
		b->PoLine.addQuadPoint(b->Width, rr, b->Width, rr, b->Width, b->Height-rr, b->Width, b->Height-rr);
		b->PoLine.addQuadPoint(b->Width, b->Height-rr, b->Width-rr*0.552284749, b->Height-rr, b->Width-rr, b->Height, b->Width-rr, b->Height-rr*0.552284749);
		b->PoLine.addQuadPoint(b->Width-rr, b->Height, b->Width-rr, b->Height, rr, b->Height, rr, b->Height);
		b->PoLine.addQuadPoint(rr, b->Height, rr, b->Height-rr*0.552284749, 0, b->Height-rr, rr*0.552284749, b->Height-rr);
		b->PoLine.addQuadPoint(0, b->Height-rr, 0, b->Height-rr, 0, rr, 0, rr);
		b->PoLine.addQuadPoint(0, rr, rr*0.552284749, rr, rr, 0, rr, rr*0.552284749);
	}
	b->Clip = FlattenPath(b->PoLine, b->Segments);
	b->ClipEdited = false;
	b->FrameType = 2;
	setRedrawBounding(b);
	emit ItemRadius(b->RadRect);
}

void ScribusView::SetFrameRect()
{
	ClRe = -1;
	PageItem *b;
	if (GetItem(&b))
	{
		SetRectFrame(b);
		updateContents(getRedrawBounding(b));
	}
}

void ScribusView::SetFrameRounded()
{
	ClRe = -1;
	PageItem *b;
	if (GetItem(&b))
	{
		if (b->RadRect == 0)
		{
			SetFrameRect();
			return;
		}
		SetFrameRound(b);
		updateContents(getRedrawBounding(b));
	}
}

void ScribusView::SetFrameOval()
{
	ClRe = -1;
	PageItem *b;
	if (GetItem(&b))
	{
		SetOvalFrame(b);
		updateContents(getRedrawBounding(b));
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
			PageItem *b = SelItem.at(a);
			b->OldPwidth = b->Pwidth;
			b->setLineWidth(w);
			if (b->itemType() == PageItem::PolyLine)
				SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
			if (b->itemType() == PageItem::Line)
			{
				int ph = static_cast<int>(QMAX(1.0, w / 2.0));
				b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
				                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
				                  -ph,static_cast<int>(b->Height+ph));
			}
			RefreshItem(b);
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
		PageItem *b;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			b->LocalX = x;
			b->LocalY = y;
			RefreshItem(b);
		}
	}
}

void ScribusView::ChLocalSc(double x, double y)
{
	if (SelItem.count() != 0)
	{
		double oldx, oldy;
		PageItem *b;

		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			oldx = b->LocalViewX / b->LocalScX;
			oldy = b->LocalViewY / b->LocalScY;
			b->LocalScX = x;
			b->LocalScY = y;
			b->LocalViewX = oldy * b->LocalScX;
			b->LocalViewY = oldy * b->LocalScY;
			RefreshItem(b);
		}
	}
}

void ScribusView::ItemFont(QString fon)
{
	if (SelItem.count() != 0)
	{
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFont,
										 fon, Um::IFont);
		for (uint aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *b = SelItem.at(aa);
			if (Doc->appMode == NormalMode)
			{
				b->setFont(fon);
				if (b->itemText.count() != 0)
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
						b->itemText.at(a)->cfont = fon;
					if (b->itemType() == PageItem::PathText)
					{
						UpdatePolyClip(b);
						AdjustItemSize(b);
					}
/*					if (!Doc->loading)
						emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr); */
					RefreshItem(b);
				}
			}
			if ((b->HasSel) && (Doc->appMode == EditMode))
			{
				if (b->itemText.count() != 0)
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
					{
						if (b->itemText.at(a)->cselect)
							b->itemText.at(a)->cfont = fon;
					}
					RefreshItem(b);
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
		PageItem *b;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if ((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::PathText))
			{
				if (Doc->appMode != EditMode)
					b->setFontFillColor(farbe);
				for (uint i=0; i<b->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (b->itemText.at(i)->cselect)
							b->itemText.at(i)->ccolor = farbe;
					}
					else
						b->itemText.at(i)->ccolor = farbe;
				}
			}
			RefreshItem(b);
			emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemTextBrushS(int sha)
{
	if (SelItem.count() != 0)
	{
		PageItem *b;
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontFillShade,
									  QString("%1").arg(sha), Um::IFont);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if (b->itemType() == PageItem::TextFrame)
			{
				if (Doc->appMode != EditMode)
					b->setFontFillShade(sha);
				for (uint i=0; i<b->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (b->itemText.at(i)->cselect)
							b->itemText.at(i)->cshade = sha;
					}
					else
						b->itemText.at(i)->cshade = sha;
				}
			}
			RefreshItem(b);
			emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
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
		PageItem *b;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if ((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::PathText))
			{
				if (Doc->appMode != EditMode)
					b->setFontStrokeColor(farbe);
				for (uint i=0; i<b->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (b->itemText.at(i)->cselect)
							b->itemText.at(i)->cstroke = farbe;
					}
					else
						b->itemText.at(i)->cstroke = farbe;
				}
			}
			RefreshItem(b);
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
		PageItem *b;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if (b->itemType() == PageItem::TextFrame)
			{
				if (Doc->appMode != EditMode)
					b->setFontStrokeShade(sha);
				for (uint i=0; i<b->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (b->itemText.at(i)->cselect)
							b->itemText.at(i)->cshade2 = sha;
					}
					else
						b->itemText.at(i)->cshade2 = sha;
				}
			}
			RefreshItem(b);
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
		PageItem *b;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if ((b->itemType() == PageItem::TextFrame) || (b->itemType() == PageItem::PathText))
			{
				if (Doc->appMode != EditMode)
					b->setFontWidth(sha);
				for (uint i=0; i<b->itemText.count(); ++i)
				{
					if (Doc->appMode == EditMode)
					{
						if (b->itemText.at(i)->cselect)
							b->itemText.at(i)->cscale = sha;
					}
					else
						b->itemText.at(i)->cscale = sha;
				}
			}
			RefreshItem(b);
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
		PageItem *b;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			b->setFillColor(farbe);
			RefreshItem(b);
			emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
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
		PageItem *b;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			b->setFillShade(sha);
			emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
			RefreshItem(b);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemPenShade(int sha)
{
	if (SelItem.count() != 0)
	{
		PageItem *i;
		if (SelItem.count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
							Um::IGroup, Um::SetLineShade, QString("%1").arg(sha), Um::IShade);
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			i = SelItem.at(a);
			i->setLineShade(sha);
			RefreshItem(i);
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::ItemGradFill(int typ)
{
	if (SelItem.count() != 0)
	{
		PageItem *i;
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			i = SelItem.at(a);
			i->GrType = typ;
			updateGradientVectors(i);
			RefreshItem(i);
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
			PageItem *b = SelItem.at(aa);
			if (Doc->appMode != EditMode)
				b->setFontEffects(s);
			if (b->itemText.count() != 0)
			{
				if (Doc->appMode == EditMode)
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
					{
						if (b->itemText.at(a)->cselect)
						{
							b->itemText.at(a)->cstyle &= ~127;
							b->itemText.at(a)->cstyle |= s;
						}
					}
				}
				else
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
					{
						b->itemText.at(a)->cstyle &= ~127;
						b->itemText.at(a)->cstyle |= s;
					}
				}
				RefreshItem(b);
			}
		}
		if (SelItem.count() > 1)
			undoManager->commit();
	}
}

void ScribusView::SetAbStyle(int s)
{
	PageItem *b;
	if (GetItem(&b))
		chAbStyle(b, s);
}

void ScribusView::chAbStyle(PageItem *b, int s)
{
	int a, ax;
	PageItem *nb;
	bool cr = true;
	if (Doc->appMode == EditMode)
	{
		nb = b;
		a = b->CPos;
		if (a == static_cast<int>(nb->itemText.count()))
			a -= 1;
		while ((cr) && (nb != 0))
		{
			if (nb->itemText.count() == 0)
			{
				nb = nb->BackBox;
				if (nb == 0)
				{
					cr = false;
					break;
				}
				a = static_cast<int>(nb->itemText.count()-1);
			}
			ax = a;
			for (int xx=0; xx<ax+1; ++xx)
			{
				if (nb->itemText.at(a)->ch == QChar(13))
				{
					cr = false;
					break;
				}
				if (s > 4)
				{
					if (Doc->docParagraphStyles[s].Font != "")
					{
						nb->itemText.at(a)->cfont = Doc->docParagraphStyles[s].Font;
						nb->itemText.at(a)->csize = Doc->docParagraphStyles[s].FontSize;
						nb->itemText.at(a)->cstyle &= ~127;
						nb->itemText.at(a)->cstyle |= Doc->docParagraphStyles[s].FontEffect;
						nb->itemText.at(a)->ccolor = Doc->docParagraphStyles[s].FColor;
						nb->itemText.at(a)->cshade = Doc->docParagraphStyles[s].FShade;
						nb->itemText.at(a)->cstroke = Doc->docParagraphStyles[s].SColor;
						nb->itemText.at(a)->cshade2 = Doc->docParagraphStyles[s].SShade;
					}
				}
				if ((s < 5) && (nb->itemText.at(a)->cab > 4))
				{
					nb->itemText.at(a)->ccolor = nb->TxtFill;
					nb->itemText.at(a)->cshade = nb->ShTxtFill;
					nb->itemText.at(a)->cstroke = nb->TxtStroke;
					nb->itemText.at(a)->cshade2 = nb->ShTxtStroke;
					nb->itemText.at(a)->csize = nb->ISize;
					nb->itemText.at(a)->cfont = nb->IFont;
					nb->itemText.at(a)->cstyle &= ~127;
					nb->itemText.at(a)->cstyle |= nb->TxTStyle;
				}
				nb->itemText.at(a)->cab = s;
				a--;
			}
			if (cr)
			{
				nb = nb->BackBox;
				if (nb != 0)
					a = static_cast<int>(nb->itemText.count()-1);
			}
		}
		a = b->CPos;
		cr = true;
		nb = b;
		if (a == static_cast<int>(nb->itemText.count()))
			cr = false;
		while ((cr) && (nb != 0))
		{
			while (a < static_cast<int>(nb->itemText.count()))
			{
				if (s > 4)
				{
					if (Doc->docParagraphStyles[s].Font != "")
					{
						nb->itemText.at(a)->cfont = Doc->docParagraphStyles[s].Font;
						nb->itemText.at(a)->csize = Doc->docParagraphStyles[s].FontSize;
						nb->itemText.at(a)->cstyle &= ~127;
						nb->itemText.at(a)->cstyle |= Doc->docParagraphStyles[s].FontEffect;
						nb->itemText.at(a)->ccolor = Doc->docParagraphStyles[s].FColor;
						nb->itemText.at(a)->cshade = Doc->docParagraphStyles[s].FShade;
						nb->itemText.at(a)->cstroke = Doc->docParagraphStyles[s].SColor;
						nb->itemText.at(a)->cshade2 = Doc->docParagraphStyles[s].SShade;
					}
				}
				if ((s < 5) && (nb->itemText.at(a)->cab > 4))
				{
					nb->itemText.at(a)->ccolor = nb->TxtFill;
					nb->itemText.at(a)->cshade = nb->ShTxtFill;
					nb->itemText.at(a)->cstroke = nb->TxtStroke;
					nb->itemText.at(a)->cshade2 = nb->ShTxtStroke;
					nb->itemText.at(a)->csize = nb->ISize;
					nb->itemText.at(a)->cfont = nb->IFont;
					nb->itemText.at(a)->cstyle &= ~127;
					nb->itemText.at(a)->cstyle |= nb->TxTStyle;
				}
				nb->itemText.at(a)->cab = s;
				if ((nb->itemText.at(a)->ch == QChar(13)) && (!nb->itemText.at(a)->cselect))
				{
					cr = false;
					break;
				}
				a++;
			}
			if (cr)
			{
				nb = nb->NextBox;
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
			ss->set("OLD_STYLE", b->textAlignment);
			ss->set("NEW_STYLE", s);
			undoManager->action(b, ss);
		}
		b->textAlignment = s;
		if (b->itemText.count() != 0)
		{
			for (a = 0; a < static_cast<int>(b->itemText.count()); ++a)
			{
				if (s > 4)
				{
					if (Doc->docParagraphStyles[s].Font != "")
					{
						b->itemText.at(a)->cfont = Doc->docParagraphStyles[s].Font;
						b->itemText.at(a)->csize = Doc->docParagraphStyles[s].FontSize;
						b->itemText.at(a)->cstyle &= ~127;
						b->itemText.at(a)->cstyle |= Doc->docParagraphStyles[s].FontEffect;
						b->itemText.at(a)->ccolor = Doc->docParagraphStyles[s].FColor;
						b->itemText.at(a)->cshade = Doc->docParagraphStyles[s].FShade;
						b->itemText.at(a)->cstroke = Doc->docParagraphStyles[s].SColor;
						b->itemText.at(a)->cshade2 = Doc->docParagraphStyles[s].SShade;
					}
				}
				if ((s < 5) && (b->itemText.at(a)->cab > 4))
				{
					b->itemText.at(a)->ccolor = b->TxtFill;
					b->itemText.at(a)->cshade = b->ShTxtFill;
					b->itemText.at(a)->cstroke = b->TxtStroke;
					b->itemText.at(a)->cshade2 = b->ShTxtStroke;
					b->itemText.at(a)->cfont = b->IFont;
					b->itemText.at(a)->csize = b->ISize;
					b->itemText.at(a)->cstyle &= ~127;
					b->itemText.at(a)->cstyle |= b->TxTStyle;
				}
				b->itemText.at(a)->cab = s;
			}
		}
	}
	if (!b->Tinput)
		RefreshItem(b);
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
			PageItem *b = SelItem.at(aa);
			if ((b->HasSel) && (Doc->appMode == EditMode))
			{
				if (b->itemText.count() != 0)
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
					{
						if (b->itemText.at(a)->cselect)
							b->itemText.at(a)->cextra = us;
					}
					RefreshItem(b);
				}
			}
			else
			{
				if (Doc->appMode != EditMode)
					b->setKerning(us);
				if (b->itemText.count() != 0)
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
					{
						b->itemText.at(a)->cextra = us;
					}
					RefreshItem(b);
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
			PageItem *b = SelItem.at(0);
			Doc->CurrFontSize = size;
			if (Doc->appMode != EditMode)
			{
				b->LineSp = ((size / 10.0) * static_cast<double>(Doc->typographicSetttings.autoLineSpacing) / 100) + (size / 10.0);
				Doc->docParagraphStyles[0].LineSpa = b->LineSp;
				emit ItemTextAttr(b->LineSp);
				b->setFontSize(size);
				emit ItemTextSize(b->ISize);
				emit ItemTextCols(b->Cols, b->ColGap);
			}
			if (b->itemText.count() != 0)
			{
				if (Doc->appMode == EditMode)
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
					{
						if (b->itemText.at(a)->cselect)
							b->itemText.at(a)->csize = size;
					}
				}
				else
				{
					for (uint a = 0; a < b->itemText.count(); ++a)
					{
						if (b->itemText.at(a)->cab < 5)
							b->itemText.at(a)->csize = size;
					}
				}
				if (b->itemType() == PageItem::PathText)
				{
					UpdatePolyClip(b);
					AdjustItemSize(b);
				}
				RefreshItem(b);
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

void ScribusView::LoadPict(QString fn, int ItNr, bool reload)
{
	ImageInfoRecord imgInfo;
	imgInfo.clipPath.resize(0);
	bool dummy;
	QFileInfo fi = QFileInfo(fn);
	PageItem *Item = Doc->Items.at(ItNr);
	if (!reload)
	{
		if ((ScApp->fileWatcher->files().contains(Item->Pfile) != 0) && (Item->PicAvail))
			ScApp->fileWatcher->removeFile(Item->Pfile);
	}
	QImage img = LoadPicture(fn, Item->IProfile, Item->IRender, Item->UseEmbedded, true, 2, 72, &dummy, &imgInfo);
	if (img.isNull())
	{
		Item->Pfile = fi.absFilePath();
		Item->PicAvail = false;
		Item->PicArt = false;
	}
	else
	{
		double xres = qRound(img.dotsPerMeterX() * 0.0254);
		double yres = qRound(img.dotsPerMeterY() * 0.0254);
		Item->pixm = img.copy();
		Item->pixmOrg = img.copy();
		Item->PicAvail = true;
		Item->PicArt = true;
		Item->BBoxX = 0;
		if (Item->Pfile != fn)
		{
			Item->LocalScX = 72.0 / xres;
			Item->LocalScY = 72.0 / yres;
			Item->LocalViewX = 72.0 / xres;
			Item->LocalViewY = 72.0 / yres;
		}
		else
		{
			Item->LocalViewX = Item->LocalScX;
			Item->LocalViewY = Item->LocalScY;
		}
		Item->Pfile = fi.absFilePath();
		if (!reload)
			ScApp->fileWatcher->addFile(Item->Pfile);
		Item->BBoxH = Item->pixm.height();
		Item->OrigW = Item->pixm.width();
		Item->OrigH = Item->pixm.height();
		Item->isRaster = true;
		Item->dpiX = xres;
		Item->dpiY = yres;
		if (imgInfo.clipPath.size() != 0)
		{
			Item->PoLine = imgInfo.clipPath.copy();
			QWMatrix cl;
			cl.scale(72.0 / xres, 72.0 / yres);
			Item->PoLine.map(cl);
			Item->FrameType = 3;
			Item->Clip = FlattenPath(Item->PoLine, Item->Segments);
			Item->ClipEdited = true;
			AdjustItemSize(Item);
			setRedrawBounding(Item);
		}
	}
	if (!Doc->loading)
	{
		emit RasterPic(Item->isRaster);
//		emit UpdtObj(PageNr, ItNr);
	}
	emit DocChanged();
}

void ScribusView::AdjustPreview(PageItem *b, bool reload)
{
	int neww, newh;
	bool savF, fho, fvo;
	if ((b->LocalViewX < 1.0) || (b->LocalViewY < 1.0))
	{
		if ((b->Pfile != "") && (b->PicAvail))
		{
			if (!b->Sizing)
			{
				fho = b->imageFlippedH();
				fvo = b->imageFlippedV();
				savF = b->PicArt;
				if ( !b->pixmOrg.isNull() )
				{
					b->pixm = b->pixmOrg.copy();
					b->OrigW = b->pixm.width();
					b->OrigH  = b->pixm.height();
					b->LocalViewX = b->LocalScX;
					b->LocalViewY = b->LocalScY;
				}
				else if (reload == false ) // jjsa begin
				{
					b->OrigW = b->pixm.width();
					b->OrigH  = b->pixm.height();
					b->LocalViewX = b->LocalScX;
					b->LocalViewY = b->LocalScY; // jjsa end
				}
				else
					LoadPict(b->Pfile, b->ItemNr, true);
				b->setImageFlippedH(fho);
				b->setImageFlippedV(fvo);
				b->PicArt = savF;
			}
			neww = qRound(b->pixm.width() * b->LocalViewX);
			newh = qRound(b->pixm.height() * b->LocalViewY);
			b->pixm = b->pixm.smoothScale(neww, newh);
			b->LocalViewX = 1;
			b->LocalViewY = 1;
		}
	}
}

void ScribusView::AdjustPictScale(PageItem *b, bool reload)
{
	if (b->ScaleType)
		return;
	if ((b->OrigW == 0) || (b->OrigH == 0))
		return;
	bool savF;
	b->LocalX = 0;
	b->LocalY = 0;
	double xs = b->Width / static_cast<double>(b->OrigW);
	double ys = b->Height / static_cast<double>(b->OrigH);
	if (!b->Sizing)
	{
		bool fho = b->imageFlippedH();
		bool fvo = b->imageFlippedV();
		savF = b->PicArt;
		if (!b->pixmOrg.isNull())
		{
			b->pixm = b->pixmOrg.copy();
			b->OrigW = b->pixm.width();
			b->OrigH  = b->pixm.height();
			b->LocalViewX = b->LocalScX;
			b->LocalViewY = b->LocalScY;
		}
		else if ( reload == false ) // jjsa begin
			{
			b->OrigW = b->pixm.width();
			b->OrigH  = b->pixm.height();
			b->LocalViewX = b->LocalScX;
			b->LocalViewY = b->LocalScY;
			}
		else // jjsa end
			LoadPict(b->Pfile, b->ItemNr, true);
		b->setImageFlippedH(fho);
		b->setImageFlippedV(fvo);
		b->PicArt = savF;
	}
	if (b->AspectRatio)
	{
		b->LocalScX = QMIN(xs, ys);
		b->LocalScY = QMIN(xs, ys);
		b->LocalViewX = QMIN(xs, ys);
		b->LocalViewY = QMIN(xs, ys);
	}
	else
	{
		b->LocalScX = xs;
		b->LocalScY = ys;
		b->LocalViewX = xs;
		b->LocalViewY = ys;
	}
	emit SetLocalValues(b->LocalScX, b->LocalScY, b->LocalX, b->LocalY );
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
				hg->cfont = *it;
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
		z = PaintLine(x, y, w, h, pw, Buffer->Pcolor2);
		break;
	case PageItem::Polygon:
		z = PaintPoly(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	case PageItem::PolyLine:
		z = PaintPolyLine(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	}
	PageItem* b = Doc->Items.at(z);
	b->setImageFlippedH(Buffer->flippedH);
	b->setImageFlippedV(Buffer->flippedV);
	b->RadRect = Buffer->RadRect;
	b->FrameType = Buffer->FrameType;
	b->ClipEdited = Buffer->ClipEdited;
	b->setFillColor(Buffer->Pcolor);
	b->setLineColor(Buffer->Pcolor2);
	b->setFillShade(Buffer->Shade);
	b->setLineShade(Buffer->Shade2);
	b->TxtStroke = Buffer->TxtStroke;
	b->TxtFill = Buffer->TxtFill;
	b->ShTxtStroke = Buffer->ShTxtStroke;
	b->ShTxtFill = Buffer->ShTxtFill;
	b->TxtScale = Buffer->TxtScale;
	b->TxTStyle = Buffer->TxTStyle;
	b->Rot = Buffer->Rot;
	b->Extra = Buffer->Extra;
	b->TExtra = Buffer->TExtra;
	b->BExtra = Buffer->BExtra;
	b->RExtra = Buffer->RExtra;
	b->PLineArt = PenStyle(Buffer->PLineArt);
	b->PLineEnd = PenCapStyle(Buffer->PLineEnd);
	b->PLineJoin = PenJoinStyle(Buffer->PLineJoin);
	b->isPrintable = Buffer->isPrintable;
	b->isBookmark = Buffer->isBookmark;
	b->BMnr = Buffer->BMnr;
	b->isAnnotation = Buffer->isAnnotation;
	b->AnType = Buffer->AnType;
	b->AnAction = Buffer->AnAction;
	b->An_E_act = Buffer->An_E_act;
	b->An_X_act = Buffer->An_X_act;
	b->An_D_act = Buffer->An_D_act;
	b->An_Fo_act = Buffer->An_Fo_act;
	b->An_Bl_act = Buffer->An_Bl_act;
	b->An_K_act = Buffer->An_K_act;
	b->An_F_act = Buffer->An_F_act;
	b->An_V_act = Buffer->An_V_act;
	b->An_C_act = Buffer->An_C_act;
	b->An_Extern = Buffer->An_Extern;
	b->AnZiel = Buffer->AnZiel;
	b->AnActType = Buffer->AnActType;
	b->TopLine = Buffer->TopLine;
	b->RightLine = Buffer->RightLine;
	b->LeftLine = Buffer->LeftLine;
	b->BottomLine = Buffer->BottomLine;
	b->isTableItem = Buffer->isTableItem;
	b->TopLinkID = Buffer->TopLinkID;
	b->LeftLinkID = Buffer->LeftLinkID;
	b->RightLinkID = Buffer->RightLinkID;
	b->BottomLinkID = Buffer->BottomLinkID;
	b->startArrowIndex = Buffer->startArrowIndex;
	b->endArrowIndex = Buffer->endArrowIndex;
	if (Buffer->AnName != "")
	{
		if (!drag)
		{
			if (b->itemName() == Buffer->AnName)
				b->AutoName = true;
			else
			{
				b->setItemName(Buffer->AnName);
				b->AutoName = false;
			}
		}
		else
		{
			bool found = false;
			for (uint c = 0; c < Doc->Items.count(); ++c)
			{
				if (Buffer->AnName == Doc->Items.at(c)->itemName())
				{
					found = true;
					break;
				}
			}
			if (found)
				b->setItemName(tr("Copy of")+" "+Buffer->AnName);
			else
				b->setItemName(Buffer->AnName);
			b->AutoName = false;
		}
	}
	b->AnToolTip = Buffer->AnToolTip;
	b->AnBwid = Buffer->AnBwid;
	b->AnBsty = Buffer->AnBsty;
	b->AnFeed = Buffer->AnFeed;
	b->AnVis = Buffer->AnVis;
	b->AnFlag = Buffer->AnFlag;
	b->AnFont = Buffer->AnFont;
	b->AnFormat = Buffer->AnFormat;
	b->AnMaxChar = Buffer->AnMaxChar;
	b->AnRollOver = Buffer->AnRollOver;
	b->AnDown = Buffer->AnDown;
	b->AnIsChk = Buffer->AnIsChk;
	b->AnAAact = Buffer->AnAAact;
	b->AnChkStil = Buffer->AnChkStil;
	b->AnHTML = Buffer->AnHTML;
	b->AnUseIcons = Buffer->AnUseIcons;
	b->AnIPlace = Buffer->AnIPlace;
	b->AnScaleW = Buffer->AnScaleW;
	b->AnBColor = Buffer->AnBColor;
	b->Clip = Buffer->Clip.copy();
	b->PoShow = Buffer->PoShow;
	b->BaseOffs = Buffer->BaseOffs;
	b->Textflow = Buffer->Textflow;
	b->Textflow2 = Buffer->Textflow2;
	b->textAlignment = Buffer->textAlignment;
	b->IFont = Buffer->IFont;
	b->ISize = Buffer->ISize;
	b->ExtraV = Buffer->ExtraV;
	b->Groups = Buffer->Groups;
	b->TabValues = Buffer->TabValues;
	b->DashValues = Buffer->DashValues;
	b->DashOffset = Buffer->DashOffset;
	b->setLocked(Buffer->Locked);
	b->setSizeLocked(Buffer->LockRes);
	b->setFillTransparency(Buffer->Transparency);
	b->setLineTransparency(Buffer->TranspStroke);
	b->Reverse = Buffer->Reverse;
	b->InvPict = Buffer->InvPict;
	b->NamedLStyle = Buffer->NamedLStyle;
	b->Language = ScApp->GetLang(Buffer->Language);
	b->Cols = Buffer->Cols;
	b->ColGap = Buffer->ColGap;
	if (Buffer->LayerNr != -1)
		b->LayerNr = Buffer->LayerNr;
	b->PoLine = Buffer->PoLine.copy();
	b->UseContour = Buffer->UseContour;
	if (Buffer->ContourLine.size() == 0)
		b->ContourLine = b->PoLine.copy();
	else
		b->ContourLine = Buffer->ContourLine.copy();
	if (b->itemType() != PageItem::Line)
	{
		// OBSOLETE CR 2005-02-06
		if ((b->PoLine.size() == 0) && (b->itemType() != PageItem::FrameType1))
			ConvertClip(b);
		else
			//
			b->Clip = FlattenPath(b->PoLine, b->Segments);
	}
	else
	{
		int ph = static_cast<int>(QMAX(1.0, b->Pwidth / 2.0));
		b->Segments.clear();
		b->PoLine.resize(0);
		b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
		                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
		                  -ph,static_cast<int>(b->Height+ph));
		b->Height = 1;
	}
	// OBSOLETE CR 2005-02-06
	if (b->itemType() == PageItem::FrameType1)
		SetOvalFrame(b);
	// OBSOLETE CR 2005-02-06
	if (b->itemType() == PageItem::FrameType3)
	{
		if (b->RadRect != 0)
			SetFrameRound(b);
		else
			SetRectFrame(b);
		b->ClipEdited = true;
	}
	//
	if (b->itemType() == PageItem::ImageFrame)
	{
		AdjustPictScale(b);
		AdjustPreview(b, false);
	}
	if ((b->itemType() != PageItem::TextFrame) && (b->itemType() != PageItem::PathText))
		b->IFont = Doc->toolSettings.defFont;
	if (Buffer->GrType != 0)
	{
		if ((Buffer->GrColor != "") && (Buffer->GrColor2 != ""))
		{
			b->fill_gradient.clearStops();
			if (Buffer->GrType == 5)
			{
				if ((Buffer->GrColor != "None") && (Buffer->GrColor != ""))
					b->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
				b->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
				if ((Buffer->GrColor2 != "None") && (Buffer->GrColor2 != ""))
					b->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
				b->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
			}
			else
			{
				if ((Buffer->GrColor2 != "None") && (Buffer->GrColor2 != ""))
					b->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
				b->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
				if ((Buffer->GrColor != "None") && (Buffer->GrColor != ""))
					b->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
				b->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
			}
		}
		else
			b->fill_gradient = Buffer->fill_gradient;
		b->GrType = Buffer->GrType;
		b->GrStartX = Buffer->GrStartX;
		b->GrStartY = Buffer->GrStartY;
		b->GrEndX = Buffer->GrEndX;
		b->GrEndY = Buffer->GrEndY;
		updateGradientVectors(b);
	}
	setRedrawBounding(b);
	b->OwnPage = OnPage(b);
	if (!loading)
	{
		b->Select = true;
		SelItem.append(b);
		emit HaveSel(b->itemType());
		EmitValues(b);
		emit DocChanged();
		updateContents();
	}
/*	if ((b->isBookmark) && (!loading))
		emit NewBMNr(b->BMnr, z); */
}

void ScribusView::BuildAObj()
{
	PageItem *b;
	int ObjGroup;
	struct AlignObjs Object;
	AObjects.clear();
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		b = SelItem.at(a);
		Object.Objects.clear();
		getBoundingRect(b, &Object.x1, &Object.y1, &Object.x2, &Object.y2);
		if (b->Groups.count() > 0)
		{
			ObjGroup = b->Groups.top();
			bool found = false;
			for (uint a2 = 0; a2 < AObjects.count(); ++a2)
			{
				if (AObjects[a2].Group == ObjGroup)
				{
					AObjects[a2].x1 = QMIN(AObjects[a2].x1, Object.x1);
					AObjects[a2].y1 = QMIN(AObjects[a2].y1, Object.y1);
					AObjects[a2].x2 = QMAX(AObjects[a2].x2, Object.x2);
					AObjects[a2].y2 = QMAX(AObjects[a2].y2, Object.y2);
					AObjects[a2].Objects.append(b);
					found = true;
					break;
				}
			}
			if (!found)
			{
				Object.Group = ObjGroup;
				Object.ObjNr = 0;
				Object.Objects.append(b);
				AObjects.append(Object);
			}
		}
		else
		{
			Object.Group = 0;
			Object.ObjNr = b->ItemNr;
			Object.Objects.append(b);
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
	PageItem *b;
	if (GetItem(&b))
	{
		emit ItemFarben(b->lineColor(), b->fillColor(), b->lineShade(), b->fillShade());
		emit ItemGradient(b->GrType);
		emit ItemTrans(b->fillTransparency(), b->lineTransparency());
	}
}

void ScribusView::ToPathText()
{
	if (SelItem.count() > 1)
	{
		PageItem *b = SelItem.at(0);
		PageItem *bb;
		if (b->itemType() == PageItem::TextFrame)
			bb = SelItem.at(1);
		else
		{
			bb = SelItem.at(0);
			b = SelItem.at(1);
		}
		if (bb->itemType() != PageItem::PolyLine)
			return;
		b->Frame = false;
		b->ClipEdited = true;
		b->convertTo(PageItem::PathText);
		b->PoLine = bb->PoLine.copy();
		b->Pwidth = bb->Pwidth;
		b->setLineColor(bb->lineColor());
		b->PLineArt = bb->PLineArt;
		b->PLineEnd = bb->PLineEnd;
		b->PLineJoin = bb->PLineJoin;
/*		if (!Doc->loading)
			emit UpdtObj(Doc->currentPage->PageNr, b->ItemNr); */
		UpdatePolyClip(b);
		AdjustItemSize(b);
		double dx = bb->Xpos - b->Xpos;
		double dy = bb->Ypos - b->Ypos;
		MoveItem(dx, dy, b);
		b->Rot = bb->Rot;
		b->FrameType = 3;
		Deselect(true);
		SelectItemNr(bb->ItemNr);
		DeleteItem();
		updateContents();
	}
}

void ScribusView::FromPathText()
{
	PageItem *b;
	if (GetItem(&b))
	{
		uint z = PaintPolyLine(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, "None", b->lineColor());
		PageItem *bb = Doc->Items.at(z);
		bb->PoLine = b->PoLine.copy();
		bb->ClipEdited = true;
		bb->FrameType = 3;
		bb->Rot = b->Rot;
		SetPolyClip(bb, qRound(QMAX(bb->Pwidth / 2, 1)));
		AdjustItemSize(bb);
		b->convertTo(PageItem::TextFrame);
		b->setLineColor("None");
		b->Frame = true;
		SetRectFrame(b);
		SelectItemNr(b->ItemNr);
		ToFront();
		Deselect(true);
		update();
	}
}

void ScribusView::TextToPath()
{
	if (SelItem.count() > 0)
	{
		PageItem *bb;
		FPointArray pts;
		double x, y;
		QString chx, ccounter;
		PageItem *b = SelItem.at(0);
		Deselect();
		if (b->itemText.count() == 0)
			return;
		for (uint a = 0; a < b->MaxChars; ++a)
		{
			pts.resize(0);
			x = 0.0;
			y = 0.0;
			chx = b->itemText.at(a)->ch;
			if ((chx == QChar(13)) || (chx == QChar(32)) || (chx == QChar(29)))
				continue;
			if (chx == QChar(30))
			{
				chx = b->ExpandToken(a);
				if (chx == QChar(32))
					continue;
			}
			int chs = b->itemText.at(a)->csize;
			if (b->itemText.at(a)->cstyle & 64)
			{
				if (chx.upper() != chx)
				{
					chs = QMAX(static_cast<int>(b->itemText.at(a)->csize * Doc->typographicSetttings.valueSmallCaps / 100), 1);
					chx = chx.upper();
				}
			}
			double csi = static_cast<double>(chs) / 100.0;
			uint chr = chx[0].unicode();
			QWMatrix chma;
			chma.scale(csi, csi);
			pts = (*Doc->AllFonts)[b->itemText.at(a)->cfont]->GlyphArray[chr].Outlines.copy();
			if (pts.size() < 4)
				continue;
			x = (*Doc->AllFonts)[b->itemText.at(a)->cfont]->GlyphArray[chr].x * csi;
			y = (*Doc->AllFonts)[b->itemText.at(a)->cfont]->GlyphArray[chr].y * csi;
			pts.map(chma);
			chma = QWMatrix();
			chma.scale(b->itemText.at(a)->cscale / 100.0, 1);
			pts.map(chma);
			chma = QWMatrix();
			if (b->imageFlippedH() && (!b->Reverse))
				chma.scale(-1, 1);
			if (b->imageFlippedV())
				chma.scale(1, -1);
			pts.map(chma);
			uint z = PaintPoly(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, b->lineColor(), b->fillColor());
			bb = Doc->Items.at(z);
			bb->Textflow = b->Textflow;
			bb->Textflow2 = b->Textflow2;
			bb->setSizeLocked(b->sizeLocked());
			bb->setLocked(b->locked());
			bb->NamedLStyle = b->NamedLStyle;
			bb->setItemName(b->itemName()+"+"+ccounter.setNum(a));
			bb->AutoName = false;
			bb->PoLine = pts.copy();
			bb->Rot = b->Rot;
			bb->setFillColor(b->itemText.at(a)->ccolor);
			bb->setFillShade(b->itemText.at(a)->cshade);
			if (b->itemText.at(a)->cstyle & 4)
			{
				bb->lineColor() = b->itemText.at(a)->cstroke;
				bb->setLineShade(b->itemText.at(a)->cshade2);
			}
			else
			{
				bb->lineColor() = "None";
				bb->setLineShade(100);
			}
			bb->Pwidth = QMAX((*Doc->AllFonts)[b->itemText.at(a)->cfont]->strokeWidth * chs / 2.0, 1);
			FPoint tp2 = getMinClipF(&bb->PoLine);
			bb->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp = getMaxClipF(&bb->PoLine);
			bb->Width = tp.x();
			bb->Height = tp.y();
			bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
			FPoint npo;
			double textX = b->itemText.at(a)->xp;
			double textY = b->itemText.at(a)->yp;
			double wide;
			if (a < b->itemText.count()-1)
				wide = Cwidth(Doc, b->itemText.at(a)->cfont, chx, b->itemText.at(a)->csize, b->itemText.at(a+1)->ch);
			else
				wide = Cwidth(Doc, b->itemText.at(a)->cfont, chx, b->itemText.at(a)->csize);
			if (b->imageFlippedH())
				textX = b->Width - textX - wide;
			if (b->imageFlippedV())
				textY = b->Height - textY+ y - (bb->Height - y);
			npo = transformPoint(FPoint(textX+x, textY-y), 0.0, 0.0, b->Rot, 1.0, 1.0);
			bb->Xpos = b->Xpos+npo.x();
			bb->Ypos = b->Ypos+npo.y();
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
		SelItem.append(b);
		DeleteItem();
	}
}

void ScribusView::UniteObj()
{
	PageItem *b;
	PageItem *bb;
	QValueList<int> toDel;
	toDel.clear();
	if (SelItem.count() > 1)
	{
		b = SelItem.at(0);
		b->Frame = false;
		b->ClipEdited = true;
		b->Groups.clear();
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
			ma2.translate(b->Xpos, b->Ypos);
			ma2.rotate(b->Rot);
			ma2 = ma2.invert();
			bb->PoLine.map(ma2);
			b->PoLine.setMarker();
			b->PoLine.putPoints(b->PoLine.size(), bb->PoLine.size(), bb->PoLine);
		}
		b->Clip = FlattenPath(b->PoLine, b->Segments);
		AdjustItemSize(b);
		b->ContourLine = b->PoLine.copy();
		Deselect(true);
		for (uint c = 0; c < toDel.count(); ++c)
			SelectItemNr(*toDel.at(c));
		DeleteItem();
		updateContents();
	}
}

void ScribusView::SplitObj()
{
	PageItem *b;
	PageItem *bb;
	uint StartInd = 0;
	uint z;
	b = SelItem.at(0);
	uint EndInd = b->PoLine.size();
	for (uint a = b->PoLine.size()-1; a > 0; a--)
	{
		if (b->PoLine.point(a).x() > 900000)
		{
			StartInd = a + 1;
			z = PaintPoly(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, b->fillColor(), b->lineColor());
			bb = Doc->Items.at(z);
			bb->PoLine.resize(0);
			bb->PoLine.putPoints(0, EndInd - StartInd, b->PoLine, StartInd);
			bb->Rot = b->Rot;
			AdjustItemSize(bb);
			bb->ContourLine = bb->PoLine.copy();
			bb->ClipEdited = true;
			a -= 3;
			EndInd = StartInd - 4;
		}
	}
	b->PoLine.resize(StartInd-4);
	AdjustItemSize(b);
	b->ContourLine = b->PoLine.copy();
	b->ClipEdited = true;
	Deselect(true);
	updateContents();
}

void ScribusView::contentsWheelEvent(QWheelEvent *w)
{
	QScrollView::contentsWheelEvent(w);
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
