/***************************************************************************
                          page.cpp  -  description
                             -------------------
    begin                : Sat Apr 7 2001
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

#include "page.h"
#include "page.moc"
#include "pageitem.h"
#include "serializer.h"
#include "scribusXml.h"
#include "scribus.h"
#include "insertTable.h"

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include <qapplication.h>
#include <qcolor.h>
#include <qrect.h>
#include <qpointarray.h>
#include <qregion.h>
#include <qcursor.h>
#include <qimage.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qcstring.h>
#include <qdragobject.h>
#include <qurl.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <qbitmap.h>
#include <qwmatrix.h>
#include <qpopupmenu.h>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qevent.h>
#include <qeventloop.h>
#include <qprocess.h>
#include <qscrollbar.h>
#include <unistd.h>

#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif
#ifdef HAVE_CMS
	#include CMS_INC
#endif
extern int PolyC;
extern int PolyFd;
extern double PolyF;
extern bool PolyS;
extern double PolyR;
extern ProfilesL InputProfiles;
extern QPixmap loadIcon(QString nam);
extern double Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2 = " ");
#ifdef HAVE_CMS
QImage ProofPict(QImage *Im, QString Prof, int Rend, cmsHPROFILE emPr=0);
#else
QImage ProofPict(QImage *Im, QString Prof, int Rend);
#endif
extern FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern double xy2Deg(double x, double y);
extern void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern FPoint GetMaxClipF(FPointArray Clip);
extern void WordAndPara(PageItem* b, int *w, int *p, int *c, int *wN, int *pN, int *cN);
extern ScribusApp* ScApp;
extern int callGS(const QStringList & args);

Page::Page(QWidget *pa, int x, int y, int b, int h, ScribusDoc *doc, QScrollView *view)
		: QWidget(pa, "ps", WRepaintNoErase)
{
	resize(b, h);
	move(x, y);
	setEraseColor(doc->papColor);
	doku = doc;
	Anz = view;
	Items.clear();
	Items.setAutoDelete(true);
	Margins.Top = 9;
	Margins.Left = 9;
	Margins.Right = 9;
	Margins.Bottom = 40;
	SelItem.clear();
	ClRe = -1;
	ClRe2 = -1;
	setAcceptDrops(true);
	emit DocChanged();
	doku->DragP = false;
	doku->leaveDrag = false;
	Imoved = false;
	Mpressed = false;
	MidButt = false;
	HaveSelRect = false;
	PageNam = "";
	LeftPg = false;
	Magnify = false;
	FirstPoly = true;
	EdPoints = true;
	MoveSym = false;
	GroupSel = false;
	DraggedGroup = false;
	GroupX = 0;
	GroupY = 0;
	GroupW = 0;
	GroupH = 0;
	DrHY = -1;
	DrVX = -1;
	XGuides.clear();
	YGuides.clear();
	GyM = 0;
	MoveGY = false;
	GxM = 0;
	MoveGX = false;
}

void Page::dragLeaveEvent(QDragLeaveEvent *e)
{
	if (DraggedGroup)
	{
		double gx, gy, gw, gh;
		getGroupRectScreen(&gx, &gy, &gw, &gh);
		repaint(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh));
		DraggedGroup = false;
	}
}

void Page::dragEnterEvent(QDragEnterEvent *e)
{
	QString text;
	e->accept(QTextDrag::canDecode(e));
	if (QTextDrag::decode(e, text))
	{
		double gx, gy, gw, gh;
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (fi.exists())
			text = ur.path();
		if(ss->ReadElemHeader(text,fi.exists(), &gx, &gy, &gw, &gh))
		{
			GroupX = e->pos().x() / doku->Scale;
			GroupY = e->pos().y() / doku->Scale;
			GroupW = gw;
			GroupH = gh;
			DraggedGroup = true;
			GroupSel = false;
		}
		delete ss;
		setActiveWindow();
		raise();
		ScApp->newActWin(doku->WinHan);
		SeleItemPos(e->pos());
	}
}

void Page::dragMoveEvent(QDragMoveEvent *e)
{
	QString text;
	PageItem *b;
	bool img;
	e->accept(QTextDrag::canDecode(e));
	if (QTextDrag::decode(e, text))
	{
		if (DraggedGroup)
		{
			double gx, gy, gw, gh;
			getGroupRectScreen(&gx, &gy, &gw, &gh);
			repaint(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh));
			GroupX = e->pos().x() / doku->Scale;
			GroupY = e->pos().y() / doku->Scale;
			paintGroupRect(false);
			emit MousePos(GroupX, GroupY);
			emit Hrule(e->pos().x()+Anz->childX(parentWidget()));
			emit Vrule(e->pos().y()+Anz->childY(parentWidget()));
			return;
		}
		QUrl ur(text);
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
		}
	}
}

void Page::dropEvent(QDropEvent *e)
{
	QString text;
	PageItem *b;
	bool img = false;
	struct Pti *hg;
	uint a;
	int re;
	if (QTextDrag::decode(e, text))
	{
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).upper();
		QStrList imfo = QImageIO::inputFormats();
		if (ext == "JPG")
			ext = "JPEG";
		img = ((imfo.contains(ext))||(ext=="PS")||(ext=="EPS")||(ext=="PDF")||(ext=="TIF"));
		if ((fi.exists()) && (img) && (!SeleItemPos(e->pos())))
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
		}
		if ((SeleItemPos(e->pos())) && (!text.startsWith("<SCRIBUSELEM")))
		{
			b = SelItem.at(0);
			if (b->PType == 2)
			{
				if ((fi.exists()) && (img))
				{
					LoadPict(ur.path(), b->ItemNr);
					update();
				}
			}
			if (b->PType == 4)
			{
				if ((b->BackBox != 0) && (b->Ptext.count() == 0))
					return;
				if ((fi.exists()) && (!img) && (fi.size() < 500000))
				{
					Serializer *ss = new Serializer(ur.path());
					if (ss->Read())
					{
						int st = doku->CurrentABStil;
						ss->GetText(b, st, doku->Vorlagen[st].Font, doku->Vorlagen[st].FontSize);
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
						hg = new Pti;
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
						b->Ptext.insert(b->CPos, hg);
						b->CPos += 1;
					}
					b->Dirty = true;
					emit DocChanged();
					update();
				}
			}
		}
		else
		{
			if ((!img) && (doku->DraggedElem == 0))
				emit LoadElem(QString(text), qRound(e->pos().x()/doku->Scale), qRound(e->pos().y()/doku->Scale), false, false, doku);
			else
			{
				if (doku->DraggedElem != 0)
				{
					if (!doku->leaveDrag)
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
					if (re == 2)
					{
						update();
						return;
					}
					if ((re == 1) || (doku->leaveDrag))
					{
						uint ac = Items.count();
						QPtrList<PageItem> pasted;
						emit LoadElem(QString(text), qRound(e->pos().x()/doku->Scale), qRound(e->pos().y()/doku->Scale), false, false, doku);
						for (uint as = ac; as < doku->ActPage->Items.count(); ++as)
						{
							pasted.append(Items.at(as));
						}
						doku->DraggedElem->OwnPage->SelItem.clear();
						for (uint dre=0; dre<doku->DragElements.count(); ++dre)
						{
							doku->DraggedElem->OwnPage->SelItem.append(doku->DraggedElem->OwnPage->Items.at(doku->DragElements[dre]));
						}
						PageItem* bb;
						int fin;
						for (uint dre=0; dre<doku->DragElements.count(); ++dre)
						{
							bb = pasted.at(dre);
							b = doku->DraggedElem->OwnPage->SelItem.at(dre);
							if ((b->PType == 4) && ((b->NextBox != 0) || (b->BackBox != 0)))
							{
								if (b->BackBox != 0)
								{
									bb->BackBox = b->BackBox;
									fin = doku->DraggedElem->OwnPage->SelItem.find(b->BackBox);
									if (fin != -1)
										bb->BackBox = pasted.at(fin);
									bb->BackBox->NextBox = bb;
								}
								if (b->NextBox != 0)
								{
									bb->NextBox = b->NextBox;
									fin = doku->DraggedElem->OwnPage->SelItem.find(b->NextBox);
									if (fin != -1)
										bb->NextBox = pasted.at(fin);
									bb->NextBox->BackBox = bb;
								}
							}
						}
						for (uint dre=0; dre<doku->DragElements.count(); ++dre)
						{
							b = doku->DraggedElem->OwnPage->SelItem.at(dre);
							b->NextBox = 0;
							b->BackBox = 0;
						}
						pasted.clear();
						doku->DraggedElem->OwnPage->DeleteItem();
					}
				}
				if ((!img) && ((re == 0)))
					emit LoadElem(QString(text), qRound(e->pos().x()/doku->Scale), qRound(e->pos().y()/doku->Scale), false, false, doku);
				doku->DraggedElem = 0;
				doku->DragElements.clear();
				update();
			}
		}
	}
}
void Page::leaveEvent(QEvent *)
{

	if (BlockLeave)
		return;
	if (!Mpressed)
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	else
	{
		if ((SelItem.count() != 0) && (Mpressed) && (!doku->DragP) && (doku->AppMode == 1))
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
	}
}

void Page::paintEvent(QPaintEvent *e)
{
	QRect vr = ViewReg().boundingRect().intersect(e->rect());
	if ((vr.width() < 1) || (vr.height() < 1))
		return;
	QPixmap pgPix(vr.width(), vr.height());
	ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
	painter->clear(doku->papColor);
	painter->translate(0.5, 0.5);
	if (doku->Before)
		DrawPageMarks(painter, vr);
	//	QTime tim;
	//	tim.start();
	DrawPageItems(painter, vr);
	//	qDebug( "Time elapsed: %d ms", tim.elapsed() );
	if (!doku->Before)
		DrawPageMarks(painter, vr);
	painter->end();
	bitBlt( this, vr.x(), vr.y(), &pgPix, 0, 0, pgPix.width(), pgPix.height() );
	QPainter p;
	p.begin(this);
	p.setPen(QColor(0, 0, 0));
	p.setBrush(NoBrush);
	p.drawRect(0, 0, width(), height());
	p.end();
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		b->paintObj(e->rect());
		if ((doku->EditClip) && (b->Select))
			MarkClip(b);
		if (GroupSel)
		{
			setGroupRect();
			paintGroupRect();
		}
	}
	delete painter;
}

void Page::DrawPageMarks(ScPainter *p, QRect rd)
{
	double b;
	if ((rd.width() == 0) || (rd.height() == 0))
		return;
	double lw = 1.0 / doku->Scale;
	p->setZoomFactor(doku->Scale);
	QWMatrix ma = p->worldMatrix();
	QWMatrix ma2 = p->worldMatrix();
	ma.translate(-rd.x(), -rd.y());
	p->setWorldMatrix(ma);
	p->setLineWidth(lw);
	if (ScApp->Prefs.MarginsShown)
	{
		p->setPen(doku->margColor);
		if (doku->RandFarbig)
		{
			p->setBrush(doku->margColor);
			p->drawRect(0, 0, width()/doku->Scale, Margins.Top);
			p->drawRect(0, Margins.Top, Margins.Left, height()/doku->Scale-Margins.Top);
			p->drawRect(Margins.Left, height()/doku->Scale-Margins.Bottom,
			            width()/doku->Scale-Margins.Right-Margins.Left, Margins.Bottom);
			p->drawRect(width()/doku->Scale-Margins.Right, Margins.Top, Margins.Right, height()/doku->Scale);
		}
		p->setPen(doku->margColor);
		p->drawLine(FPoint(0, Margins.Top), FPoint(width()/doku->Scale, Margins.Top));
		p->drawLine(FPoint(0, height()/doku->Scale-Margins.Bottom), FPoint(width()/doku->Scale,
		            height()/doku->Scale-Margins.Bottom));
		p->drawLine(FPoint(Margins.Left, 0), FPoint(Margins.Left, height()/doku->Scale));
		p->drawLine(FPoint(width()/doku->Scale-Margins.Right, 0), FPoint(width()/doku->Scale-Margins.Right,
		            height()/doku->Scale));
	}
	if (ScApp->Prefs.BaseShown)
	{
		p->setPen(doku->guideColor, lw, SolidLine, FlatCap, MiterJoin);
		for (double yg = doku->BaseOffs; yg < doku->PageH; yg += doku->BaseGrid)
			p->drawLine(FPoint(0, yg), FPoint(doku->PageB, yg));
	}
	if (ScApp->Prefs.GridShown)
	{
		double stx = rd.x()/doku->Scale;
		double endx = rd.x()/doku->Scale+rd.width()/doku->Scale;
		double sty = rd.y()/doku->Scale;
		double endy = rd.y()/doku->Scale+rd.height()/doku->Scale;
		if (doku->Scale > 0.49)
		{
			double i,start;
			i=doku->majorGrid;
			p->setPen(doku->majorColor, lw, SolidLine, FlatCap, MiterJoin);
			start=floor(sty/i);
			start*=i;
			for (b = start; b < endy; b+=i)
				p->drawLine(FPoint(0, b), FPoint(width()/doku->Scale, b));
			start=floor(stx/i);
			start*=i;
			for (b = start; b <= endx; b+=i)
				p->drawLine(FPoint(b, 0), FPoint(b, height()/doku->Scale));
			i=doku->minorGrid;
			p->setPen(doku->minorColor, lw, DotLine, FlatCap, MiterJoin);
			start=floor(sty/i);
			start*=i;
			for (b = start; b < endy; b+=i)
				p->drawLine(FPoint(0, b), FPoint(width()/doku->Scale, b));
			start=floor(stx/i);
			start*=i;
			for (b = start; b <= endx; b+=i)
				p->drawLine(FPoint(b, 0), FPoint(b, height()/doku->Scale));
		}
	}
	if (ScApp->Prefs.GuidesShown)
	{
		if (XGuides.count() != 0)
		{
			p->setPen(doku->guideColor, lw, SolidLine, FlatCap, MiterJoin);
			for (uint xg = 0; xg < XGuides.count(); ++xg)
				p->drawLine(FPoint(XGuides[xg], 0), FPoint(XGuides[xg], doku->PageH));
		}
		if (YGuides.count() != 0)
		{
			p->setPen(doku->guideColor, lw, SolidLine, FlatCap, MiterJoin);
			for (uint yg = 0; yg < YGuides.count(); ++yg)
				p->drawLine(FPoint(0, YGuides[yg]), FPoint(doku->PageB, YGuides[yg]));
		}
	}
	p->setWorldMatrix(ma2);
}

void Page::DrawPageItems(ScPainter *painter, QRect rd)
{
	QPainter p;
	uint a;
	int Lnr;
	struct Layer ll;
	PageItem* b;
	ll.Sichtbar = false;
	ll.LNr = 0;
	ScribusView* vi = (ScribusView*)Anz;
	if ((rd.width() == 0) || (rd.height() == 0))
		return;
	if (MPageNam != "")
	{
		Page* Mp = vi->MasterPages.at(vi->MasterNames[MPageNam]);
		QWidget* Opa;
		Page* Opa2;
		if (Mp->Items.count() != 0)
		{
			Lnr = 0;
			for (uint la = 0; la < doku->Layers.count(); ++la)
			{
				Level2Layer(doku, &ll, Lnr);
				if (ll.Sichtbar)
				{
					for (a = 0; a < Mp->Items.count(); ++a)
					{
						b = Mp->Items.at(a);
						if (b->LayerNr != ll.LNr)
							continue;
						Opa = b->Parent;
						Opa2 = b->OwnPage;
						b->Parent = this;
						b->OwnPage = this;
						QPainter p;
						p.begin(this);
						Transform(b, &p);
						QRegion apr = QRegion(p.xForm(b->Clip));
						QRegion apr2 = QRegion(p.xForm(QRect(-1, -1, static_cast<int>(b->Width), static_cast<int>(b->Height))));
						p.end();
						if ((rd.intersects(apr.boundingRect())) || (rd.intersects(apr2.boundingRect())))
							b->DrawObj(painter, rd);
						b->Parent = Opa;
						b->OwnPage = Opa2;
					}
					for (a = 0; a < Mp->Items.count(); ++a)
					{
						b = Mp->Items.at(a);
						if (b->LayerNr != ll.LNr)
							continue;
						if (!b->isTableItem)
							continue;
						Opa = b->Parent;
						Opa2 = b->OwnPage;
						b->Parent = this;
						b->OwnPage = this;
						QPainter p;
						p.begin(this);
						Transform(b, &p);
						QRegion apr = QRegion(p.xForm(b->Clip));
						QRegion apr2 = QRegion(p.xForm(QRect(-1, -1, static_cast<int>(b->Width), static_cast<int>(b->Height))));
						p.end();
						if ((rd.intersects(apr.boundingRect())) || (rd.intersects(apr2.boundingRect())))
						{
							painter->setZoomFactor(doku->Scale);
							painter->save();
							painter->translate(-rd.x(), -rd.y());
							painter->translate(b->Xpos*doku->Scale, b->Ypos*doku->Scale);
							painter->rotate(b->Rot);
							if (b->Pcolor2 != "None")
							{
								QColor tmp;
								b->SetFarbe(&tmp, b->Pcolor2, b->Shade2);
								if ((b->TopLine) || (b->RightLine) || (b->BottomLine) || (b->LeftLine))
								{
									painter->setPen(tmp, b->Pwidth, b->PLineArt, Qt::SquareCap, b->PLineJoin);
									painter->newPath();
									if (b->TopLine)
									{
										painter->moveTo(0.0, 0.0);
										painter->lineTo(b->Width, 0.0);
									}
									if (b->RightLine)
									{
										painter->moveTo(b->Width, 0.0);
										painter->lineTo(b->Width, b->Height);
									}
									if (b->BottomLine)
									{
										painter->moveTo(0.0, b->Height);
										painter->lineTo(b->Width, b->Height);
									}
									if (b->LeftLine)
									{
										painter->moveTo(0.0, 0.0);
										painter->lineTo(0.0, b->Height);
									}
									painter->drawPolyLine();
								}
							}
							painter->restore();
						}
						b->Parent = Opa;
						b->OwnPage = Opa2;
					}
				}
				Lnr++;
			}
		}
	}
	if (Items.count() != 0)
	{
		Lnr = 0;
		for (uint la2 = 0; la2 < doku->Layers.count(); ++la2)
		{
			Level2Layer(doku, &ll, Lnr);
			if (ll.Sichtbar)
			{
				for (a = 0; a < Items.count(); ++a)
				{
					b = Items.at(a);
					if (b->LayerNr != ll.LNr)
						continue;
					QPainter p;
					p.begin(this);
					Transform(b, &p);
					QRegion apr = QRegion(p.xForm(b->Clip));
					QRegion apr2 = QRegion(p.xForm(QRect(-1, -1, static_cast<int>(b->Width),
					                                     static_cast<int>(b->Height))));
					p.end();
					if ((rd.intersects(apr.boundingRect())) || (rd.intersects(apr2.boundingRect())))
					{
		if (!((doku->EditClip) && (Mpressed)))
						b->DrawObj(painter, rd);
						b->Redrawn = true;
						if ((doku->AppMode == 7) && (b->Select))
							slotDoCurs(true);
					}
				}
				for (a = 0; a < Items.count(); ++a)
				{
					b = Items.at(a);
					if (b->LayerNr != ll.LNr)
						continue;
					if (!b->isTableItem)
						continue;
					QPainter p;
					p.begin(this);
					Transform(b, &p);
					QRegion apr = QRegion(p.xForm(b->Clip));
					QRegion apr2 = QRegion(p.xForm(QRect(-1, -1, static_cast<int>(b->Width),
					                                     static_cast<int>(b->Height))));
					p.end();
					if ((rd.intersects(apr.boundingRect())) || (rd.intersects(apr2.boundingRect())))
					{
						painter->setZoomFactor(doku->Scale);
						painter->save();
						painter->translate(-rd.x(), -rd.y());
						painter->translate(b->Xpos*doku->Scale, b->Ypos*doku->Scale);
						painter->rotate(b->Rot);
						if (b->Pcolor2 != "None")
						{
							QColor tmp;
							b->SetFarbe(&tmp, b->Pcolor2, b->Shade2);
							if ((b->TopLine) || (b->RightLine) || (b->BottomLine) || (b->LeftLine))
							{
								painter->setPen(tmp, b->Pwidth, b->PLineArt, Qt::SquareCap, b->PLineJoin);
								painter->newPath();
								if (b->TopLine)
								{
									painter->moveTo(0.0, 0.0);
									painter->lineTo(b->Width, 0.0);
								}
								if (b->RightLine)
								{
									painter->moveTo(b->Width, 0.0);
									painter->lineTo(b->Width, b->Height);
								}
								if (b->BottomLine)
								{
									painter->moveTo(0.0, b->Height);
									painter->lineTo(b->Width, b->Height);
								}
								if (b->LeftLine)
								{
									painter->moveTo(0.0, 0.0);
									painter->lineTo(0.0, b->Height);
								}
								painter->drawPolyLine();
							}
						}
						painter->restore();
					}
				}
			}
			Lnr++;
		}
	}
}

void Page::setGroupRect()
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

void Page::moveGroup(double x, double y, bool fromMP)
{
	PageItem* b;
	double gx, gy, gw, gh;
	if (GroupSel)
		getGroupRectScreen(&gx, &gy, &gw, &gh);
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		b = SelItem.at(a);
		MoveItem(x, y, b, fromMP);
	}
	if (GroupSel)
		repaint(QRect(static_cast<int>(gx-5), static_cast<int>(gy-5), static_cast<int>(gw+10), static_cast<int>(gh+10)));
}

void Page::getGroupRect(double *x, double *y, double *w, double *h)
{
	*x = GroupX;
	*y = GroupY;
	*w = GroupW;
	*h = GroupH;
}

void Page::getGroupRectScreen(double *x, double *y, double *w, double *h)
{
	double sc = doku->Scale;
	*x = GroupX*sc;
	*y = GroupY*sc;
	*w = GroupW*sc;
	*h = GroupH*sc;
}

void Page::paintGroupRect(bool norm)
{
	double x, y, w, h;
	getGroupRectScreen(&x, &y, &w, &h);
	QPainter pgc;
	pgc.begin(this);
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

void Page::storeUndoInf(PageItem* b)
{
	if ((doku->UnData.UnCode == 0) && (doku->UnDoValid))
		delete doku->UnData.Item;
	doku->UnDoValid = false;
	doku->UnData.Xpos = b->Xpos;
	doku->UnData.Ypos = b->Ypos;
	doku->UnData.Width = b->Width;
	doku->UnData.Height = b->Height;
	doku->UnData.Rot = b->Rot;
	doku->UnData.Item = b;
	doku->UnData.PageNr = PageNr;
	doku->UnData.ItemNr = b->ItemNr;
}

QRegion Page::ViewReg()
{
	int cx = Anz->contentsX() - Anz->childX(parentWidget());
	int cy = Anz->contentsY() - Anz->childY(parentWidget());
	int cw = Anz->visibleWidth();
	int ch = Anz->visibleHeight();
	return QRegion(QRect(cx, cy, cw, ch));
}

void Page::Transform(PageItem *b, QPainter *p)
{
	p->translate(static_cast<int>(b->Xpos*doku->Scale), static_cast<int>(b->Ypos*doku->Scale));
	p->scale(doku->Scale, doku->Scale);
	p->rotate(b->Rot);
}

bool Page::GetItem(PageItem **b, int nr)
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

void Page::TransformM(PageItem *b, QPainter *p)
{
	if (b->flippedH % 2 != 0)
	{
		p->translate(b->Width, 0);
		p->scale(-1, 1);
	}
	if (b->flippedV % 2 != 0)
	{
		p->translate(0, b->Height);
		p->scale(1, -1);
	}
}

void Page::PaintSizeRect(QPainter *p, QRect neu)
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

QPoint Page::ApplyGrid(QPoint in)
{
	QPoint np;
	if (doku->useRaster)
	{
		np.setX(static_cast<int>(qRound(in.x() / doku->minorGrid) * doku->minorGrid));
		np.setY(static_cast<int>(qRound(in.y() / doku->minorGrid) * doku->minorGrid));
	}
	else
		np = in;
	return np;
}

void Page::ApplyGuides(double *x, double *y)
{
	if (doku->SnapGuides)
	{
		if (YGuides.count() != 0)
		{
			for (uint yg = 0; yg < YGuides.count(); ++yg)
			{
				if ((YGuides[yg] < (*y+doku->GuideRad)) && (YGuides[yg] > (*y-doku->GuideRad)))
				{
					*y= YGuides[yg];
					break;
				}
			}
		}
		if (XGuides.count() != 0)
		{
			for (uint xg = 0; xg < XGuides.count(); ++xg)
			{
				if ((XGuides[xg] < (*x+doku->GuideRad)) && (XGuides[xg] > (*x-doku->GuideRad)))
				{
					*x = XGuides[xg];
					break;
				}
			}
		}
	}
}

FPoint Page::ApplyGridF(FPoint in)
{
	FPoint np;
	if (doku->useRaster)
	{
		np.setX(qRound(in.x() / doku->minorGrid) * doku->minorGrid);
		np.setY(qRound(in.y() / doku->minorGrid) * doku->minorGrid);
	}
	else
		np = in;
	return np;
}

void Page::RefreshItem(PageItem *b, bool single)
{
	QPainter p;
	p.begin(this);
	Transform(b, &p);
	QRect rd = QRect(-36, -36, qRound(b->Width) + 74, qRound(b->Height) + 74);
	if (single)
		RepaintTextRegion(b, QRegion(p.xForm(rd)), true);
	else
		update(QRegion(p.xForm(rd)).intersect(ViewReg()).boundingRect());
	p.end();
}

void Page::RepaintTextRegion(PageItem *b, QRegion alt, bool single)
{
	if (!isUpdatesEnabled())
		return;
	QPainter p;
	QRegion neu;
	p.begin(this);
	Transform(b, &p);
	neu = QRegion(p.xForm(QRect(0, 0, static_cast<int>(b->Width+1), static_cast<int>(b->Height+1)))).unite(alt);
	p.end();
	b->Dirty = true;
	QRect g = neu.boundingRect();
	g.setX(g.x()-static_cast<int>(QMAX(10.0, b->Pwidth / 1.4)));
	g.setY(g.y()-static_cast<int>(QMAX(10.0, b->Pwidth / 1.4)));
	g.setWidth(g.width()+static_cast<int>(QMAX(10.0, b->Pwidth / 1.4)));
	g.setHeight(g.height()+static_cast<int>(QMAX(10.0, b->Pwidth / 1.4)));
	if (single)
	{
		QRect rd = ViewReg().boundingRect().intersect(g);
		if ((rd.width() < 1) || (rd.height() < 1))
			return;
		QPixmap pgPix(rd.width(), rd.height());
		ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
		painter->translate(0.5, 0.5);
		if (doku->Before)
			DrawPageMarks(painter, rd);
		if (!((doku->EditClip) && (Mpressed)))
			b->DrawObj(painter, rd);
		if (!doku->Before)
			DrawPageMarks(painter, rd);
		painter->end();
		bitBlt( this, rd.x(), rd.y(), &pgPix, 0, 0, pgPix.width(), pgPix.height() );
		QPainter px;
		px.begin(this);
		px.setPen(QColor(0, 0, 0));
		px.setBrush(NoBrush);
		px.drawRect(0, 0, width(), height());
		px.end();
		b->paintObj(rd);
		if ((doku->EditClip) && (b->Select))
			MarkClip(b);
		if (GroupSel)
		{
			setGroupRect();
			paintGroupRect();
		}
		delete painter;
	}
	else
	{
		neu = QRegion(g);
		update(neu.intersect(ViewReg()).boundingRect());
	}
}

void Page::AdjustPreview(PageItem *b, bool reload)
{
	int neww, newh, fho, fvo;
	bool savF;
	if ((b->LocalViewX < 1.0) || (b->LocalViewY < 1.0))
	{
		if ((b->Pfile != "") && (b->PicAvail))
		{
			if (!b->Sizing)
			{
				fho = b->flippedH;
				fvo = b->flippedV;
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
					LoadPict(b->Pfile, b->ItemNr);
				b->flippedH = fho;
				b->flippedV = fvo;
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
void Page::AdjustPictScale(PageItem *b, bool reload)
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
		int fho = b->flippedH, fvo = b->flippedV;
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
			LoadPict(b->Pfile, b->ItemNr);
		b->flippedH = fho;
		b->flippedV = fvo;
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

bool Page::MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP)
{
	QRegion alt;
	QPainter p;
	PageItem *b = Items.at(ite);
	p.begin(this);
	Transform(b, &p);
	alt = QRegion(p.xForm(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))));
	p.end();
	if (b->PType == 5)
	{
		QWMatrix ma;
		ma.translate(b->Xpos, b->Ypos);
		ma.rotate(b->Rot);
		double mx = ma.m11() * b->Width + ma.m21() * b->Height + ma.dx();
		double my = ma.m22() * b->Height + ma.m12() * b->Width + ma.dy();
		MoveItem(newX.x(), newX.y(), b, fromMP);
		b->Rot = xy2Deg(mx - b->Xpos, my - b->Ypos);
		b->Width = sqrt(pow(mx - b->Xpos,2)+pow(my - b->Ypos,2));
		b->Height = 0;
		UpdateClip(b);
		RepaintTextRegion(b, alt);
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
			SizeItem(b->Width - newY.x(), b->Height - newY.y(), ite, fromMP);
			MoveItem(-mxc3, -myc3, b, fromMP);
		}
		else
		{
			SizeItem(b->Width - newY.x(), b->Height - newY.y(), ite, fromMP);
			MoveItem(newX.x(), newX.y(), b, fromMP);
		}
	}
	return true;
}

void Page::UpdateClip(PageItem* b)
{
	if (doku->AppMode == 13)
		return;
	int ph = static_cast<int>(QMAX(1.0, b->Pwidth / 2.0));
	switch (b->PType)
	{
	case 5:
		b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
		                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
		                  -ph,static_cast<int>(b->Height+ph));
		break;
	default:
		if (!b->ClipEdited)
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
			}
			b->OldB2 = b->Width;
			b->OldH2 = b->Height;
		}
		else
		{
			if (doku->SubMode != -1)
			{
				switch (doku->SubMode)
				{
				case 0:
					SetRectFrame(b);
					break;
				case 1:
					SetOvalFrame(b);
					break;
				default:
					SetFrameShape(b, doku->ValCount, doku->ShapeValues);
					break;
				}
				b->OldB2 = b->Width;
				b->OldH2 = b->Height;
			}
			if ((b->OldB2 == 0) || (b->OldH2 == 0))
				return;
			double scx = b->Width / b->OldB2;
			double scy = b->Height / b->OldH2;
			QWMatrix ma;
			ma.scale(scx, scy);
			b->PoLine.map(ma);
			if (b->PType == 8)
				UpdatePolyClip(b);
			else
				b->Clip = FlattenPath(b->PoLine, b->Segments);
			b->OldB2 = b->Width;
			b->OldH2 = b->Height;
		}
		break;
	}
}

bool Page::SizeItem(double newX, double newY, int ite, bool fromMP, bool DoUpdateClip)
{
	PageItem *b = Items.at(ite);
	if (b->Locked)
		return false;
	QRegion alt, neu;
	QPainter p;
	if (b->PType != 5)
	{
		newX = QMAX(newX, 1);
		newY = QMAX(newY, 1);
	}
	int ph = static_cast<int>(QMAX(1.0, b->Pwidth / 2.0));
	p.begin(this);
	Transform(b, &p);
	alt = QRegion(p.xForm(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))));
	p.end();
	QWMatrix ma;
	ma.rotate(b->Rot);
	double dX = ma.m11() * (b->Width - newX) + ma.m21() * (b->Height - newY) + ma.dx();
	double dY = ma.m22() * (b->Height - newY) + ma.m12() * (b->Width - newX) + ma.dy();
	b->Width = newX;
	b->Height = newY;
	if ((doku->RotMode != 0) && (fromMP))
	{
		switch (doku->RotMode)
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
	if ((b->PType == 2) && (!b->Sizing) && (!doku->EditClip))
	{
		AdjustPictScale(b);
		AdjustPreview(b, false);
	}
	if (b->PType == 5)
	{
		if (!fromMP)
		{
			b->Rot = atan2(b->Height,b->Width)*(180.0/3.1415927);
			b->Width = sqrt(pow(b->Width,2)+pow(b->Height,2));
			b->Height = 0;
			emit SetAngle(b->Rot);
		}
		b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
		                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
		                  -ph,static_cast<int>(b->Height+ph));
	}
	if (b->Sizing)
	{
		emit ItemGeom(b->Width, b->Height);
		emit ItemRadius(b->RadRect);
		b->FrameOnly = true;
		b->Tinput = true;
		if ((HowTo == 1) && (b->PType != 5))
			b->paintObj();
		if ((b->FrameType == 0) || (b->PType == 5) || (HowTo != 1))
			return true;
		QPainter p;
		p.begin(this);
		Transform(b, &p);
		p.setRasterOp(XorROP);
		p.setBrush(NoBrush);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		b->DrawPolyL(&p, b->Clip);
		UpdateClip(b);
		b->DrawPolyL(&p, b->Clip);
		p.end();
		return true;
	}
	if (DoUpdateClip)
	{
		if (fromMP)
		{
			if (b->flippedH % 2 != 0)
				MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
			if (b->flippedV % 2 != 0)
				MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
		}
		else
		{
			if (b->flippedH % 2 == 0)
				MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
			if (b->flippedV % 2 == 0)
				MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
		}
		UpdateClip(b);
	}
	RepaintTextRegion(b, alt);
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

void Page::MoveItemI(double newX, double newY, int ite)
{
	PageItem *b = Items.at(ite);
	if ((b->Locked) || (!b->ScaleType))
		return;
	QPainter p;
	p.begin(this);
	Transform(b, &p);
	TransformM(b, &p);
	QRegion alt = QRegion(p.xForm(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))));
	p.end();
	if (b->flippedH % 2 != 0)
		b->LocalX -= newX;
	else
		b->LocalX += newX;
	if (b->flippedV % 2 != 0)
		b->LocalY -= newY;
	else
		b->LocalY += newY;
	repaint(alt);
	emit SetLocalValues(b->LocalScX, b->LocalScY, b->LocalX, b->LocalY);
}

bool Page::MoveItem(double newX, double newY, PageItem* b, bool fromMP)
{
	if (b->Locked)
		return false;
	QRegion alt;
	bool retw = false;
	double oldx = b->Xpos;
	double oldy = b->Ypos;
	if (!Imoved)
	{
		QPainter p;
		p.begin(this);
		Transform(b, &p);
		alt = QRegion(p.xForm(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))));
		p.end();
	}
	b->Xpos += newX;
	b->Ypos += newY;
	if ((doku->useRaster) && (!Imoved))
	{
		b->Xpos = qRound(b->Xpos / doku->minorGrid) * doku->minorGrid;
		b->Ypos = qRound(b->Ypos / doku->minorGrid) * doku->minorGrid;
	}
	if ((doku->SnapGuides) && (!Imoved) && (doku->AppMode == 1) && (!doku->EditClip))
		SnapToGuides(b);
	if ((b->Xpos != oldx) || (b->Ypos != oldy))
		retw = true;
	if ((!Imoved) && (!b->Sizing))
		RepaintTextRegion(b, alt);
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
	if (!doku->loading)
		emit UpdtObj(PageNr, b->ItemNr);
	return retw;
}

void Page::RotateGroup(double win)
{
	double gxS, gyS, ghS, gwS;
	PageItem* b;
	FPoint n;
	getGroupRectScreen(&gxS, &gyS, &gwS, &ghS);
	QWMatrix ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(win);
	QRect alt = QRect(static_cast<int>(gxS-5), static_cast<int>(gyS-5), static_cast<int>(gwS+10),
	                  static_cast<int>(ghS+10));
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		b = SelItem.at(a);
		n = FPoint(b->Xpos - RCenter.x(), b->Ypos - RCenter.y());
		b->Xpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		b->Ypos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		b->Rot += win;
	}
	setGroupRect();
	getGroupRectScreen(&gxS, &gyS, &gwS, &ghS);
	repaint(QRect(static_cast<int>(gxS-5), static_cast<int>(gyS-5), static_cast<int>(gwS+10),
	              static_cast<int>(ghS+10)).unite(alt));
}

void Page::RotateItem(double win, int ite)
{
	PageItem *b;
	b = Items.at(ite);
	if (b->Locked)
		return;
	QRegion alt, neu;
	QPainter p;
	FPoint n;
	p.begin(this);
	Transform(b, &p);
	alt = QRegion(p.xForm(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))));
	p.end();
	if ((doku->RotMode != 0) && (b->PType != 5))
	{
		QWMatrix ma;
		ma.translate(b->Xpos, b->Ypos);
		ma.scale(1, 1);
		ma.rotate(b->Rot);
		double ro = win-b->Rot;
		b->Rot = win;
		switch (doku->RotMode)
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
		MoveItem(x-b->Xpos, y-b->Ypos, b);
	}
	else
		b->Rot = win;
	RepaintTextRegion(b, alt);
	emit SetAngle(b->Rot);
}

void Page::SnapToGuides(PageItem* b)
{
	if (YGuides.count() != 0)
	{
		for (uint yg = 0; yg < YGuides.count(); ++yg)
		{
			if ((YGuides[yg] < (b->Ypos+doku->GuideRad)) && (YGuides[yg] > (b->Ypos-doku->GuideRad)))
			{
				b->Ypos = YGuides[yg];
				break;
			}
			if (b->PType == 5)
			{
				QWMatrix ma;
				ma.translate(b->Xpos, b->Ypos);
				ma.rotate(b->Rot);
				double my = ma.m22() * b->Height + ma.m12() * b->Width + ma.dy();
				if ((YGuides[yg] < (my+doku->GuideRad)) && (YGuides[yg] > (my-doku->GuideRad)))
				{
					b->Ypos = b->Ypos + YGuides[yg] - my;
					break;
				}
			}
			else
			{
				if ((YGuides[yg] < (b->Ypos+b->Height+doku->GuideRad)) &&
				        (YGuides[yg] > ((b->Ypos+b->Height)-doku->GuideRad)))
				{
					b->Ypos = YGuides[yg]-b->Height;
					break;
				}
			}
		}
	}
	if (XGuides.count() != 0)
	{
		for (uint xg = 0; xg < XGuides.count(); ++xg)
		{
			if ((XGuides[xg] < (b->Xpos+doku->GuideRad)) && (XGuides[xg] > (b->Xpos-doku->GuideRad)))
			{
				b->Xpos = XGuides[xg];
				break;
			}
			if (b->PType == 5)
			{
				QWMatrix ma;
				ma.translate(b->Xpos, b->Ypos);
				ma.rotate(b->Rot);
				double mx = ma.m11() * b->Width + ma.m21() * b->Height + ma.dx();
				if ((XGuides[xg] < (mx+doku->GuideRad)) && (XGuides[xg] > (mx-doku->GuideRad)))
				{
					b->Xpos = b->Xpos + XGuides[xg] - mx;
					break;
				}
			}
			else
			{
				if ((XGuides[xg] < (b->Xpos+b->Width+doku->GuideRad)) &&
				        (XGuides[xg] > ((b->Xpos+b->Width)-doku->GuideRad)))
				{
					b->Xpos = XGuides[xg]-b->Width;
					break;
				}
			}
		}
	}
}

void Page::sentToScrap()
{
	ScriXmlDoc *ss = new ScriXmlDoc();
	emit ToScrap(ss->WriteElem(&SelItem, doku));
	delete ss;
}

void Page::sentToLayer(int id)
{
	int d = pmen3->indexOf(id);
	int dd = doku->Layers[d].LNr;
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			PageItem *b = SelItem.at(a);
			b->LayerNr = dd;
		}
	}
	Deselect(true);
	update();
}

bool Page::PointOnLine(QPoint Start, QPoint Ende, QRect MArea)
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

void Page::MoveRotated(PageItem *b, FPoint npv, bool fromMP)
{
	QWMatrix ma;
	ma.translate(b->Xpos, b->Ypos);
	ma.rotate(b->Rot);
	double mxc = b->Xpos - (ma.m11() * npv.x() + ma.m21() * npv.y() + ma.dx());
	double myc = b->Ypos - (ma.m22() * npv.y() + ma.m12() * npv.x() + ma.dy());
	MoveItem(-mxc, -myc, b, fromMP);
}

void Page::AdjustItemSize(PageItem *b)
{
	bool siz = b->Sizing;
	b->Sizing = false;
	FPointArray Clip;
	Clip = b->PoLine;
	FPoint tp2 = GetMinClipF(Clip);
	SizeItem(b->Width - tp2.x(), b->Height - tp2.y(), b->ItemNr, true, false);
	Clip.translate(-tp2.x(), -tp2.y());
	if (b->Rot != 0)
	{
		FPoint npv = FPoint(tp2.x(), tp2.y());
		MoveRotated(b, npv, true);
	}
	else
		MoveItem(tp2.x(), tp2.y(), b, true);
	if (b->flippedH % 2 == 0)
		MoveItemI(-tp2.x()/b->LocalScX, 0, b->ItemNr);
	if (b->flippedV % 2 == 0)
		MoveItemI(0, -tp2.y()/b->LocalScY, b->ItemNr);
	FPoint tp = GetMaxClipF(Clip);
	if (b->flippedH % 2 != 0)
		MoveItemI((b->Width - tp.x())/b->LocalScX, 0, b->ItemNr);
	if (b->flippedV % 2 != 0)
		MoveItemI(0, (b->Height - tp.y())/b->LocalScY, b->ItemNr);
	SizeItem(tp.x(), tp.y(), b->ItemNr, true, false);
	b->ClipEdited = true;
	b->PoLine = Clip.copy();
	if (b->PType == 7)
		SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
	else
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	b->Sizing = siz;
}

void Page::MirrorPolyH()
{
	PageItem *b = SelItem.at(0);
	QWMatrix ma;
	ma.scale(-1, 1);
	b->PoLine.map(ma);
	b->PoLine.translate(b->Width, 0);
	if (b->PType == 8)
		UpdatePolyClip(b);
	else
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	RefreshItem(b);
	MarkClip(b);
}

void Page::MirrorPolyV()
{
	PageItem *b = SelItem.at(0);
	QWMatrix ma;
	ma.scale(1, -1);
	b->PoLine.map(ma);
	b->PoLine.translate(0, b->Height);
	if (b->PType == 8)
		UpdatePolyClip(b);
	else
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	RefreshItem(b);
	MarkClip(b);
}

void Page::TransformPoly(int mode)
{
	PageItem *b = SelItem.at(0);
	QWMatrix ma;
	FPoint oldPos = FPoint(b->Xpos, b->Ypos);
	double offsX = b->Width / 2.0;
	double offsY = b->Height / 2.0;
	ma.translate(-offsX, -offsY);
	switch (mode)
	{
	case 0:
		ma.rotate(-1.0);
		break;
	case 1:
		ma.rotate(1.0);
		break;
	case 2:
		ma.scale(0.9, 0.9);
		break;
	case 3:
		ma.scale(1.1, 1.1);
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
		ma2.rotate(-1.0);
		break;
	case 1:
		ma2.rotate(1.0);
		break;
	case 2:
		ma2.scale(0.9, 0.9);
		break;
	case 3:
		ma2.scale(1.1, 1.1);
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
	if (b->PType == 8)
		UpdatePolyClip(b);
	RefreshItem(b);
	MarkClip(b);
	b->FrameType = 3;
}

void Page::Reset1Control()
{
	PageItem *b = SelItem.at(0);
	FPoint np = b->PoLine.point(ClRe-1);
	b->OldB2 = b->Width;
	b->OldH2 = b->Height;
	b->PoLine.setPoint(ClRe, np);
	AdjustItemSize(b);
	MarkClip(b);
}

void Page::ResetControl()
{
	PageItem *b = SelItem.at(0);
	FPoint np = b->PoLine.point(ClRe);
	b->OldB2 = b->Width;
	b->OldH2 = b->Height;
	if ((ClRe == 0) || (ClRe == static_cast<int>(b->PoLine.size()-2)))
		b->PoLine.setPoint(ClRe+1, np);
	else
	{
		b->PoLine.setPoint(ClRe+1, np);
		b->PoLine.setPoint((ClRe % 4 != 0 ? ClRe + 3 : ClRe - 1), np);
	}
	AdjustItemSize(b);
	MarkClip(b);
}

void Page::MoveClipPoint(PageItem *b, FPoint ip)
{
	if (((EdPoints) && (ClRe % 2 != 0)) || ((!EdPoints) && (ClRe % 2 == 0)))
		return;
	FPointArray Clip;
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
	//	FPoint np = ApplyGridF(ip);
	FPoint np = ip;
	if (ClRe != -1)
	{
		if (np.x() < 0)
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
			if (b->flippedH % 2 == 0)
				MoveItemI(-np.x()/b->LocalScX, 0, b->ItemNr);
			np.setX(0);
		}
		if (np.y() < 0)
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
			if (b->flippedV % 2 == 0)
				MoveItemI(0, -np.y()/b->LocalScY, b->ItemNr);
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
		if (((ClRe % 4 != 0) && (ClRe % 2 == 0)) && (ClRe+3 < static_cast<int>(EndInd)) &&
		        (ClRe != static_cast<int>(StartInd)))
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
		        ((b->PType == 6) || (b->PType == 4) || (b->PType == 2)))
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
		        ((b->PType == 6) || (b->PType == 4) || (b->PType == 2)) && (MoveSym))
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
		if ((ClRe % 2 != 0) && (MoveSym) && (ClRe != static_cast<int>(StartInd + 1)) &&
		        (ClRe != static_cast<int>(EndInd-1)))
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
		b->PoLine = Clip.copy();
		b->Clip = FlattenPath(b->PoLine, b->Segments);
		AdjustItemSize(b);
		//					emit DocChanged();
	}
}
void Page::ConvertClip(PageItem *b)
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

void Page::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	Mpressed = false;
	PageItem *b = 0;
	if ((GroupSel) || (doku->AppMode != 1) || (doku->EditClip))
	{
		if ((GroupSel) && (doku->AppMode == 1))
		{
			if (GetItem(&b))
			{
				if (b->isTableItem)
				{
					Deselect(false);
					SelItem.append(b);
					b->isSingleSel = true;
					b->Select = true;
					emit HaveSel(b->PType);
					EmitValues(b);
					b->paintObj();
				}
			}
		}
		else
			mousePressEvent(m);
		return;
	}
	if (GetItem(&b))
	{
		if ((b->PType == 6) || (b->PType == 7) || (b->PType == 2) || (b->PType == 8))
		{
			if ((b->Locked) || (!b->ScaleType))
			{
				mousePressEvent(m);
				return;
			}
			emit Amode(7);
		}
		else
			if (b->PType == 4)
				emit b->isAnnotation ? AnnotProps() : Amode(7);
	}
}

FPoint Page::transformPointI(FPoint in, double dx, double dy, double rot, double sx, double sy)
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

FPoint Page::transformPoint(FPoint in, double dx, double dy, double rot, double sx, double sy)
{
	QWMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	double x = ma.m11() * in.x() + ma.m21() * in.y() + ma.dx();
	double y = ma.m22() * in.y() + ma.m12() * in.x() + ma.dy();
	return FPoint(x, y);
}

void Page::scaleGroup(double scx, double scy)
{
	PageItem *bb;
	double gx, gy, gh, gw;
	uint aa;
	FPoint n;
	getGroupRect(&gx, &gy, &gw, &gh);
	setUpdatesEnabled(false);
	for (uint a = 0; a < SelItem.count(); ++a)
	{
		bb = SelItem.at(a);
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
		if (bb->PType == 5)
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
		double x = ma3.m11() * n.x() + ma3.m21() * n.y() + ma3.dx();
		double y = ma3.m22() * n.y() + ma3.m12() * n.x() + ma3.dy();
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
		if ((bb->Ptext.count() != 0) && (!bb->isTableItem))
		{
			bb->LineSp = ((bb->ISize / 10.0) * static_cast<double>(doku->AutoLine) / 100) + (bb->ISize / 10.0);
			for (aa = 0; aa < bb->Ptext.count(); ++aa)
				bb->Ptext.at(aa)->csize = QMAX(qRound(bb->Ptext.at(aa)->csize*((scx+scy)/2)), 1);
			if (bb->PType == 8)
				UpdatePolyClip(bb);
		}
		bb->LocalX = oldLocalX;
		bb->LocalY = oldLocalY;
		bb->OldB2 = bb->Width;
		bb->OldH2 = bb->Height;
	}
	setGroupRect();
	setUpdatesEnabled(true);
	update();
	emit DocChanged();
}

void Page::PasteToPage()
{
	emit LoadElem(ScApp->Buffer2, qRound(Mxp/doku->Scale), qRound(Myp/doku->Scale), false, false, doku);
	doku->DraggedElem = 0;
	doku->DragElements.clear();
	update();
}

void Page::mouseReleaseEvent(QMouseEvent *m)
{
	PageItem *b;
	Mpressed = false;
	if (ScApp->Prefs.GuidesShown)
	{
		bool fg = false;
		double nx = m->x()/doku->Scale;
		double ny = m->y()/doku->Scale;
		if (YGuides.count() != 0)
		{
			for (uint yg = 0; yg < YGuides.count(); ++yg)
			{
				if ((YGuides[yg] < (ny+doku->GrabRad)) && (YGuides[yg] > (ny-doku->GrabRad)))
				{
					fg = true;
					break;
				}
			}
		}
		if (XGuides.count() != 0)
		{
			for (uint xg = 0; xg < XGuides.count(); ++xg)
			{
				if ((XGuides[xg] < (nx+doku->GrabRad)) && (XGuides[xg] > (nx-doku->GrabRad)))
				{
					fg = true;
					break;
				}
			}
		}
		if ((fg) && (m->button() == RightButton))
		{
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			MoveGY = false;
			MoveGX = false;
			emit EditGuides();
			return;
		}
		if (MoveGY)
		{
			YGuides.remove(YGuides[GyM]);
			SetYGuide(m);
			MoveGY = false;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			update();
			return;
		}
		if (MoveGX)
		{
			XGuides.remove(XGuides[GxM]);
			SetXGuide(m);
			MoveGX = false;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			update();
			return;
		}
	}
	if (doku->AppMode == 22)
	{
		if ((SelItem.count() == 0) && (HaveSelRect) && (!MidButt))
		{
			double sc = doku->Scale;
			QRect AreaR = QRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>(SeRx-(Mxp*sc)), static_cast<int>(SeRy-(Myp*sc)));
			QPainter p;
			p.begin(this);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
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
			HaveSelRect = false;
			double Tx, Ty, Tw, Th;
			Tx = Mxp;
			Ty = Myp;
			Tw = SeRx/sc-Mxp;
			Th = SeRy/sc-Myp;
			int z;
			int Cols, Rows;
			double deltaX, deltaY, offX, offY;
			if ((Th < 6) || (Tw < 6))
			{
				p.drawRect(AreaR);
				p.end();
				doku->AppMode = 1;
				emit PaintingDone();
				return;
			}
			InsertTable *dia = new InsertTable(this, static_cast<int>(Th / 6), static_cast<int>(Tw / 6));
			if (!dia->exec())
			{
				p.drawRect(AreaR);
				p.end();
				doku->AppMode = 1;
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
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					z = PaintText(Tx + offX, Ty + offY, deltaX, deltaY, doku->Dwidth, doku->DpenText);
					b = Items.at(z);
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
		}
		doku->AppMode = 1;
		emit PaintingDone();
		emit DocChanged();
		update();
		return;
	}
	if (doku->AppMode == 21)
	{
		doku->AppMode = 1;
		if (RecordP.size() > 1)
		{
			uint z = PaintPolyLine(0, 0, 1, 1, doku->Dwidth, "None", doku->Dpen);
			b = Items.at(z);
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
			b->Select = true;
			emit ItemPos(b->Xpos, b->Ypos);
			emit SetSizeValue(b->Pwidth);
			emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
			emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
			emit ItemGradient(b->GrColor2, b->GrColor, b->GrShade2, b->GrShade, b->GrType);
			emit ItemTrans(b->Transparency, b->TranspStroke);
			emit HaveSel(7);
		}
		update();
		emit PaintingDone();
		emit DocChanged();
		return;
	}
	if ((doku->EditClip) && (ClRe == -1) && (HaveSelRect))
	{
		double sc = doku->Scale;
		QPainter p;
		p.begin(this);
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.drawRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>(SeRx-(Mxp*sc)),
		           static_cast<int>(SeRy-(Myp*sc)));
		p.end();
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
		b = SelItem.at(0);
		SelNode.clear();
		QRect Sele = QRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc),
		                   static_cast<int>(SeRx-(Mxp*sc)), static_cast<int>(SeRy-(Myp*sc)));
		for (uint a = 0; a < b->PoLine.count(); ++a)
		{
			if (b->PoLine.point(a).x() > 900000)
				continue;
			p.begin(this);
			Transform(b, &p);
			QPoint npf = p.xForm(b->PoLine.pointQ(a));
			p.end();
			if ((Sele.contains(npf)) && ((a == 0) || (((a-2) % 4) == 0)))
			{
				ClRe = a;
				SelNode.append(a);
				emit ClipPo(b->PoLine.point(a).x(), b->PoLine.point(a).y());
			}
		}
		HaveSelRect = false;
		MarkClip(b);
		return;
	}
	if ((doku->EditClip) && (SegP1 == -1) && (SegP2 == -1))
	{
		if (Imoved)
		{
			b = SelItem.at(0);
			b->OldB2 = b->Width;
			b->OldH2 = b->Height;
			double nx = m->x()/doku->Scale;
			double ny = m->y()/doku->Scale;
			FPoint npg = ApplyGridF(FPoint(nx, ny));
			nx = npg.x();
			ny = npg.y();
			ApplyGuides(&nx, &ny);
			FPoint np = transformPointI(FPoint(nx, ny), b->Xpos, b->Ypos, b->Rot, 1, 1);
			MoveClipPoint(b, np);
		}
		Imoved = false;
		return;
	}
	if ((doku->EditClip) && (SegP1 != -1) && (SegP2 != -1))
	{
		SegP1 = -1;
		SegP2 = -1;
		b = SelItem.at(0);
		Imoved = false;
		b->paintObj();
		MarkClip(b);
		return;
	}
	if ((!GetItem(&b)) && (m->button() == RightButton) && (!doku->DragP) && (doku->AppMode == 1))
	{
		QPopupMenu *pmen = new QPopupMenu();
		if (ScApp->Buffer2.startsWith("<SCRIBUSELEM"))
		{
			Mxp = m->x();
			Myp = m->y();
			pmen->insertItem( tr("Paste") , this, SLOT(PasteToPage()));
			pmen->insertSeparator();
		}
		pmen->insertItem((ScApp->Prefs.MarginsShown ? tr("Hide Margins") : tr("Show Margins")), ScApp, SLOT(ToggleMarks()));
		pmen->insertItem((ScApp->Prefs.FramesShown ? tr("Hide Frames") : tr("Show Frames")), ScApp, SLOT(ToggleFrames()));
		pmen->insertItem((doku->ShowPic ? tr("Hide Images") : tr("Show Images")), ScApp, SLOT(TogglePics()));
		pmen->insertItem((ScApp->Prefs.GridShown ? tr("Hide Grid") : tr("Show Grid")), ScApp, SLOT(ToggleRaster()));
		pmen->insertItem((ScApp->Prefs.GuidesShown ? tr("Hide Guides") : tr("Show Guides")), ScApp, SLOT(ToggleGuides()));
		pmen->insertItem((ScApp->Prefs.BaseShown ? tr("Hide Baseline Grid") : tr("Show Baseline Grid")), ScApp, SLOT(ToggleBase()));
		int uRas = pmen->insertItem( tr("Snap to Grid"), ScApp, SLOT(ToggleURaster()));
		pmen->setItemChecked(uRas, doku->useRaster);
		int uGuide = pmen->insertItem( tr("Snap to Guides"), ScApp, SLOT(ToggleUGuides()));
		pmen->setItemChecked(uGuide, doku->SnapGuides);
		pmen->exec(QCursor::pos());
		delete pmen;
		return;
	}
	if ((doku->AppMode != 6) && (!doku->EditClip) && (doku->AppMode != 13))
	{
		if ((GetItem(&b)) && (m->button() == RightButton) && (!doku->DragP))
		{
			QPopupMenu *pmen = new QPopupMenu();
			QPopupMenu *pmen2 = new QPopupMenu();
			pmen3 = new QPopupMenu();
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			QPopupMenu *pmen4 = new QPopupMenu();

			if ((b->PType == 4) || (b->PType == 2) || (b->PType == 8))
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
				if (b->PType == 2)
				{
					QFileInfo fi = QFileInfo(b->Pfile);
					InfoT->setText( tr("Picture"));
					InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignCenter );
					ParCT->setText( tr("File: "));
					InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
					ParC->setText(fi.fileName());
					InfoGroupLayout->addWidget( ParC, 1, 1 );
					WordCT->setText( tr("Original ppi: "));
					InfoGroupLayout->addWidget( WordCT, 2, 0, Qt::AlignRight );
					WordC->setText(txtC.setNum(qRound(b->dpiX))+" x "+txtC2.setNum(qRound(b->dpiY)));
					InfoGroupLayout->addWidget( WordC, 2, 1 );
					CharCT->setText( tr("Actual ppi: "));
					InfoGroupLayout->addWidget( CharCT, 3, 0, Qt::AlignRight );
					CharC->setText(txtC.setNum(qRound(72.0 / b->LocalScX))+" x "+
					               txtC2.setNum(qRound(72.0 / b->LocalScY)));
					InfoGroupLayout->addWidget( CharC, 3, 1 );
				}
				if ((b->PType == 4) || (b->PType == 8))
				{
					int Parag = 0;
					int Words = 0;
					int Chara = 0;
					int ParagN = 0;
					int WordsN = 0;
					int CharaN = 0;
					if (b->PType == 4)
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

				pmen->insertItem( tr("Info"), pmen4);
			}
			if (b->PType == 2)
			{
				pmen->insertItem( tr("Get Picture..."), this, SIGNAL(LoadPic()));
				int px = pmen->insertItem( tr("Image Visible"), this, SLOT(TogglePic()));
				pmen->setItemChecked(px, b->PicArt);
				if (b->PicAvail)
					pmen->insertItem( tr("Update Picture"), this, SLOT(UpdatePic()));
				if (b->PicAvail && b->isRaster)
					pmen->insertItem( tr("Edit Picture"), this, SLOT(CallGimp()));
			}
			if (b->PType == 4)
			{
				pmen->insertItem( tr("Get Text..."), this, SIGNAL(LoadPic()));
				pmen->insertItem( tr("Append Text..."), this, SIGNAL(AppendText()));
				pmen->insertItem( tr("Edit Text..."), this, SIGNAL(EditText()));
				if (PageNam == "")
				{
					int pxb = pmen->insertItem( tr("Is PDF-Bookmark"), this, SLOT(ToggleBookmark()));
					pmen->setItemChecked(pxb, b->isBookmark);
					pxb = pmen->insertItem( tr("Is PDF-Annotation"), this, SLOT(ToggleAnnotation()));
					pmen->setItemChecked(pxb, b->isAnnotation);
					if (b->isAnnotation)
					{
						if ((b->AnType == 0) || (b->AnType == 1) || (b->AnType > 9))
							pmen->insertItem( tr("Annotation Properties"), this, SIGNAL(AnnotProps()));
						else
							pmen->insertItem( tr("Field Properties"), this, SIGNAL(AnnotProps()));
					}
				}
			}
			if (b->PType == 8)
				pmen->insertItem( tr("Edit Text..."), this, SIGNAL(EditText()));
			if (!b->Locked)
			{
				if (SelItem.count() > 1)
					pmen->insertItem( tr("Group"), this, SIGNAL(DoGroup()));
				if (b->Groups.count() != 0)
					pmen->insertItem( tr("Un-group"), this, SIGNAL(DoUnGroup()));
				pmen->insertItem( tr("Lock"), this, SLOT(ToggleLock()));
				if ((!b->isTableItem) && (!b->isSingleSel))
				{
					pmen->insertItem( tr("Send to Back"), this, SLOT(ToBack()));
					pmen->insertItem( tr("Bring to Front"), this, SLOT(ToFront()));
					pmen->insertItem( tr("Lower"), this, SLOT(LowerItem()));
					pmen->insertItem( tr("Raise"), this, SLOT(RaiseItem()));
				}
			}
			else
				pmen->insertItem( tr("Unlock"), this, SLOT(ToggleLock()));
			if ((!b->isTableItem) && (!b->isSingleSel))
			{
				pmen->insertItem( tr("Send to Scrapbook"), this, SLOT(sentToScrap()));
				if (doku->Layers.count() > 1)
				{
					for (uint lam=0; lam < doku->Layers.count(); ++lam)
					{
						int lai = pmen3->insertItem(doku->Layers[lam].Name);
						if (static_cast<int>(lam) == doku->ActiveLayer)
							pmen3->setItemEnabled(lai, 0);
					}
					pmen->insertItem( tr("Send to Layer"), pmen3);
				}
				connect(pmen3, SIGNAL(activated(int)), this, SLOT(sentToLayer(int)));
			}
			if (((b->PType == 4) || (b->PType == 2) || (b->PType == 6)) && (doku->AppMode != 7))
			{
				if (b->PType == 4)
				{
					pmen2->insertItem( tr("Picture Frame"), this, SLOT(ToPicFrame()));
					if (!b->isTableItem)
					{
						pmen2->insertItem( tr("Polygon"), this, SLOT(ToPolyFrame()));
						pmen2->insertItem( tr("Outlines"), this, SLOT(TextToPath()));
					}
				}
				if (b->PType == 2)
				{
					pmen2->insertItem( tr("Text Frame"), this, SLOT(ToTextFrame()));
					if (!b->isTableItem)
						pmen2->insertItem( tr("Polygon"), this, SLOT(ToPolyFrame()));
				}
				if (b->PType == 6)
				{
					pmen2->insertItem( tr("Text Frame"), this, SLOT(ToTextFrame()));
					pmen2->insertItem( tr("Picture Frame"), this, SLOT(ToPicFrame()));
					pmen2->insertItem( tr("Bezier Curve"), this, SLOT(ToBezierFrame()));
				}
				pmen->insertItem( tr("Convert to"), pmen2);
			}
			if (!ScApp->Mpal->isVisible())
				pmen->insertItem( tr("Show Properties..."), ScApp, SLOT(ToggleMpal()));
			else
				pmen->insertItem( tr("Hide Properties..."), ScApp, SLOT(ToggleMpal()));
			pmen->insertSeparator();
			if ((!b->Locked) && (!((b->isTableItem) && (b->isSingleSel))))
				pmen->insertItem( tr("Cut"), this, SIGNAL(CutItem()));
			if (!((b->isTableItem) && (b->isSingleSel)))
				pmen->insertItem( tr("Copy"), this, SIGNAL(CopyItem()));
			if ((doku->AppMode == 7) && (ScApp->Buffer2.startsWith("<SCRIBUSTEXT")) && (b->PType == 4))
				pmen->insertItem( tr("Paste"), ScApp, SLOT(slotEditPaste()));
			if ((b->PType == 2) || ((b->PType == 4) && (b->NextBox == 0) && (b->BackBox == 0)))
				pmen->insertItem( tr("Clear Contents"), this, SLOT(ClearItem()));
			if ((!b->Locked) && (doku->AppMode != 7) && (!((b->isTableItem) && (b->isSingleSel))))
				pmen->insertItem( tr("Delete"), this, SLOT(DeleteItem()));
			pmen->exec(QCursor::pos());
			delete pmen;
			delete pmen2;
			disconnect(pmen3, SIGNAL(activated(int)), this, SLOT(sentToLayer(int)));
			delete pmen3;
			delete pmen4;
		}
		if (doku->AppMode == 10)
			update();
		if (doku->AppMode == 12)
		{
			b = SelItem.at(0);
			QPainter p;
			FPointArray cli;
			p.begin(this);
			Transform(b, &p);
			QPoint np1 = p.xFormDev(m->pos());
			p.end();
			np1 = ApplyGrid(np1);
			b->Width = np1.x();
			b->Height = np1.y();
			cli = RegularPolygonF(b->Width, b->Height, PolyC, PolyS, PolyF, PolyR);
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
			FPoint tp2 = GetMinClipF(b->PoLine);
			if ((tp2.x() > -1) || (tp2.y() > -1))
			{
				SizeItem(b->Width - tp2.x(), b->Height - tp2.y(), b->ItemNr, false, false);
			}
			FPoint tp = GetMaxClipF(b->PoLine);
			SizeItem(tp.x(), tp.y(), b->ItemNr, false, false);
			b->Clip = FlattenPath(b->PoLine, b->Segments);
			AdjustItemSize(b);
			update();
		}
		if (doku->AppMode == 8)
		{
			b = SelItem.at(0);
			QPainter p;
			p.begin(this);
			Transform(b, &p);
			QPoint np = p.xFormDev(m->pos());
			p.end();
			np = ApplyGrid(np);
			b->Rot = xy2Deg(np.x(), np.y());
			b->Width = sqrt(pow(np.x(),2)+pow(np.y(),2));
			b->Height = 0;
			b->Sizing = false;
			UpdateClip(b);
			update();
		}
		if (GetItem(&b))
		{
			if (GroupSel)
			{
				if (mCG)
				{
					double gx, gy, gh, gw, mx, my, scx, scy;
					mx = Mxp;
					my = Myp;
					getGroupRect(&gx, &gy, &gw, &gh);
					switch (HowTo)
					{
						case 1:
							scx = fabs(mx-gx) / gw;
							scy = fabs(my-gy) / gh;
							break;
						case 2:
							scx = fabs(mx-(gx+gw)) / gw;
							scy = fabs(my-(gy+gh)) / gh;
							break;
						case 3:
							scx = fabs(mx-gx) / gw;
							scy = fabs(my-(gy+gh)) / gh;
							break;
						case 4:
							scx = fabs(mx-(gx+gw)) / gw;
							scy = fabs(my-gy) / gh;
							break;
						case 5:
							scx = 1.0;
							scy = fabs(my-gy) / gh;
							break;
						case 6:
							scx = fabs(mx-gx) / gw;
							scy = 1.0;
							break;
						case 7:
							scx = fabs(mx-(gx+gw)) / gw;
							scy = 1.0;
							break;
						case 8:
							scx = 1.0;
							scy = fabs(my-(gy+gh)) / gh;
							break;
					}
					RotMode = doku->RotMode;
					doku->RotMode = 0;
					scaleGroup(scx, scy);
					if ((HowTo == 3) || (HowTo == 8))
						moveGroup(0, my-gy);
					if (HowTo == 2)
						moveGroup(mx-gx, my-gy);
					if ((HowTo == 7) || (HowTo == 4))
						moveGroup(mx-gx, 0);
					doku->RotMode = RotMode;
				}
			}
			else
			{
				if (b->Sizing)
				{
					FPoint npx;
					double nx = m->pos().x()/doku->Scale;
					double ny = m->pos().y()/doku->Scale;
					if (doku->SnapGuides)
					{
						ApplyGuides(&nx, &ny);
						npx = transformPointI(FPoint(nx, ny), b->Xpos, b->Ypos, b->Rot, 1, 1);
					}
					else
						npx = ApplyGridF(transformPointI(FPoint(nx, ny), b->Xpos, b->Ypos, b->Rot, 1, 1));
					if ((HowTo == 1) && (b->PType != 5) && (doku->SnapGuides))
						SizeItem(npx.x(), npx.y(), b->ItemNr);
					bool sav = doku->SnapGuides;
					doku->SnapGuides = false;
					switch (HowTo)
					{
					case 1:
						if (b->PType != 5)
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
							if (b->flippedH % 2 != 0)
								MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
							if (b->flippedV % 2 != 0)
								MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
						}
						else
						{
							if (sav)
							{
								double nx = m->pos().x()/doku->Scale;
								double ny = m->pos().y()/doku->Scale;
								if (doku->useRaster)
								{
									FPoint ra = ApplyGridF(FPoint(nx, ny));
									nx = ra.x();
									ny = ra.y();
								}
								doku->SnapGuides = sav;
								ApplyGuides(&nx, &ny);
								doku->SnapGuides = false;
								double r = atan2(ny - b->Ypos, nx - b->Xpos)*(180.0/3.1415927);
								RotateItem(r, b->ItemNr);
								double w = sqrt(pow(nx - b->Xpos, 2) + pow(ny - b->Ypos,2));
								SizeItem(w, b->Height, b->ItemNr, true);
							}
						}
						b->Sizing = false;
						break;
					case 2:
						if (b->PType != 5)
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
							if (b->flippedH % 2 == 0)
								MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
							if (b->flippedV % 2 == 0)
								MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
						}
						else
						{
							if (sav)
							{
								double nx = m->pos().x()/doku->Scale;
								double ny = m->pos().y()/doku->Scale;
								if (doku->useRaster)
								{
									FPoint ra = ApplyGridF(FPoint(nx, ny));
									nx = ra.x();
									ny = ra.y();
								}
								doku->SnapGuides = sav;
								ApplyGuides(&nx, &ny);
								doku->SnapGuides = false;
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
						if (b->flippedH % 2 != 0)
							MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
						if (b->flippedV % 2 == 0)
							MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
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
						if (b->flippedH % 2 == 0)
							MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
						if (b->flippedV % 2 != 0)
							MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
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
						if (b->flippedV % 2 != 0)
							MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
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
						if (b->flippedH % 2 != 0)
							MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
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
						if (b->flippedH % 2 == 0)
							MoveItemI((b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
						if (b->flippedV % 2 != 0)
							MoveItemI(0, -(b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
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
						if (b->flippedH % 2 != 0)
							MoveItemI(-(b->Width - b->OldB2)/b->LocalScX, 0, b->ItemNr);
						if (b->flippedV % 2 == 0)
							MoveItemI(0, (b->Height - b->OldH2)/b->LocalScY, b->ItemNr);
						break;
					}
					if ((b->PType == 4) && (m->state() & ShiftButton) && (m->state() & ControlButton))
					{
						double scx = b->Width / b->OldB2;
						double scy = b->Height / b->OldH2;
						scx = scx != scy ? scx / scy : 1.0;
						if (b->Ptext.count() != 0)
						{
							b->ISize = QMAX(qRound(b->ISize * scy), 1);
							b->LineSp = ((b->ISize / 10.0)* static_cast<double>(doku->AutoLine) / 100) + (b->ISize / 10.0);
							b->TxtScale = QMIN(QMAX(qRound(b->TxtScale * scx), 25), 400);
							doku->CurrTextScale = b->TxtScale;
							doku->CurrFontSize = b->ISize;
							emit ItemTextAttr(b->LineSp);
							emit ItemTextCols(b->Cols, b->ColGap);
							emit ItemTextSize(b->ISize);
							emit ItemTextSca(b->TxtScale);
							for (uint aa = 0; aa < b->Ptext.count(); ++aa)
							{
								b->Ptext.at(aa)->csize = QMAX(qRound(b->Ptext.at(aa)->csize*scy), 1);
								b->Ptext.at(aa)->cscale = QMAX(QMIN(qRound(b->Ptext.at(aa)->cscale*scx), 400), 25);
							}
						}
					}
					if (b->PType == 2)
					{
						AdjustPictScale(b);
						AdjustPreview(b, false);
					}
					UpdateClip(b);
					emit ItemTextCols(b->Cols, b->ColGap);
					doku->SnapGuides = sav;
					update();
					emit DocChanged();
				}
			}
			if (Imoved)
			{
				b = SelItem.at(0);
				if (b->PType != 5)
				{
					if (fabs(b->Width) < 5)
						b->Width = 5;
					if (fabs(b->Height) < 5)
						b->Height = 5;
				}
				Imoved = false;
				MoveItem(0, 0, b, false);
				update();
				emit DocChanged();
			}
		}
		if ((SelItem.count() == 0) && (HaveSelRect) && (!MidButt))
		{
			double sc = doku->Scale;
			QPainter p;
			p.begin(this);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			p.drawRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>(SeRx-(Mxp*sc)),
			           static_cast<int>(SeRy-(Myp*sc)));
			p.end();
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
			QRect Sele = QRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc),
			                   static_cast<int>(SeRx-(Mxp*sc)), static_cast<int>(SeRy-(Myp*sc)));
			if (Items.count() != 0)
			{
				for (uint a = 0; a < Items.count(); ++a)
				{
					p.begin(this);
					Transform(Items.at(a), &p);
					QRegion apr = QRegion(p.xForm(Items.at(a)->Clip));
					p.end();
					if ((Sele.contains(apr.boundingRect())) && (Items.at(a)->LayerNr == doku->ActiveLayer))
						SelectItemNr(a);
				}
			}
			HaveSelRect = false;
		}
		if (doku->AppMode != 7)
		{
			if (doku->AppMode == 9)
				doku->RotMode = RotMode;
			doku->AppMode = 1;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			emit PaintingDone();
		}
		if (GetItem(&b))
		{
			if (doku->ActPage->SelItem.count() > 1)
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
		}
	}
	if (doku->AppMode == 6)
	{
		if (HaveSelRect)
		{
			double sc = doku->Scale;
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
			doku->Scale = QMIN(yf, xf);
			emit ZoomAbs();
			emit AbsPosi(Mxp, Myp);
			HaveSelRect = false;
			doku->AppMode = 1;
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			emit PaintingDone();
		}
		else
		{
			int mx=m->x();
			int my=m->y();
			emit Magnify ? ZoomIn(mx,my) : ZoomOut(mx,my);
			HaveSelRect = false;
		}
	}
	if ((doku->AppMode == 7) && !HanMove)
	{
		b = SelItem.at(0);
		uint a;
		if (b->PType == 4)
		{
			if (oldCp == b->CPos)
			{
				for (a = 0; a < b->Ptext.count(); ++a)
					b->Ptext.at(a)->cselect = false;
				b->HasSel = false;
				emit HasNoTextSel();
				b->Dirty = true;
				RefreshItem(b);
			}
		}
	}
	if ((doku->AppMode == 13) && (m->button() == LeftButton))
	{
		b = SelItem.at(0);
		QPainter p;
		QPointArray Bez(4);
		p.begin(this);
		Transform(b, &p);
		FPoint npf = FPoint(p.xFormDev(m->pos()));
		npf = ApplyGridF(npf);
		b->PoLine.addPoint(npf);
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
		FPoint np2 = GetMinClipF(b->PoLine);
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
		SizeItem(b->PoLine.WidthHeight().x(), b->PoLine.WidthHeight().y(), b->ItemNr, false, false);
		SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
		AdjustItemSize(b);
		RefreshItem(b);
		p.end();
	}
	if ((doku->AppMode == 13) && (m->button() == RightButton))
	{
		b = SelItem.at(0);
		b->PoLine.resize(b->PoLine.size()-2);
		SizeItem(b->PoLine.WidthHeight().x(), b->PoLine.WidthHeight().y(), b->ItemNr, false, false);
		SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
		AdjustItemSize(b);
		doku->AppMode = 1;
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		emit PaintingDone();
		emit DocChanged();
		FirstPoly = true;
		update();
	}
	doku->DragP = false;
	doku->leaveDrag = false;
	Imoved = false;
	mCG = false;
	MidButt = false;
	doku->SubMode = -1;
}

void Page::SetYGuide(QMouseEvent *m)
{
	QPoint py = mapFromGlobal(m->globalPos());
	double newY = py.y() / doku->Scale;
	if ((newY > 0) && (newY < doku->PageH) && (ScApp->Prefs.GuidesShown))
	{
		YGuides.append(newY);
		qHeapSort(YGuides);
		update();
		emit DocChanged();
	}
}

void Page::SetXGuide(QMouseEvent *m)
{
	QPoint py = mapFromGlobal(m->globalPos());
	double newY = py.x() / doku->Scale;
	if ((newY > 0) && (newY < doku->PageB) && (ScApp->Prefs.GuidesShown))
	{
		XGuides.append(newY);
		qHeapSort(XGuides);
		update();
		emit DocChanged();
	}
}

void Page::FromHRuler(QMouseEvent *m)
{
	double sc = doku->Scale;
	int newY;
	if (ScApp->Prefs.GuidesShown)
	{
		QPoint py = mapFromGlobal(m->globalPos());
		newY = py.y();
		emit MousePos(py.x()/sc, py.y()/sc);
		emit Hrule(py.x()+Anz->childX(parentWidget()));
		emit Vrule(py.y()+Anz->childY(parentWidget()));
		QPainter p;
		p.begin(this);
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
		p.drawLine(0, DrHY, static_cast<int>(doku->PageB * sc), DrHY);
		p.drawLine(0, newY, static_cast<int>(doku->PageB * sc), newY);
		p.end();
		DrHY = newY;
	}
}

void Page::FromVRuler(QMouseEvent *m)
{
	double sc = doku->Scale;
	int newY;
	if (ScApp->Prefs.GuidesShown)
	{
		QPoint py = mapFromGlobal(m->globalPos());
		newY = py.x();
		emit MousePos(py.x()/sc, py.y()/sc);
		emit Hrule(py.x()+Anz->childX(parentWidget()));
		emit Vrule(py.y()+Anz->childY(parentWidget()));
		QPainter p;
		p.begin(this);
		p.setRasterOp(XorROP);
		p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
		p.drawLine(DrVX, 0, DrVX, static_cast<int>(doku->PageH * sc));
		p.drawLine(newY, 0, newY, static_cast<int>(doku->PageH * sc));
		p.end();
		DrVX = newY;
	}
}

void Page::mouseMoveEvent(QMouseEvent *m)
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
	double sc = doku->Scale;
	emit MousePos(m->x()/sc, m->y()/sc);
	emit Hrule(m->x()+Anz->childX(parentWidget()));
	emit Vrule(m->y()+Anz->childY(parentWidget()));
	if (ScApp->Prefs.GuidesShown)
	{
		if (MoveGY)
		{
			FromHRuler(m);
			if (((m->y()/sc) < 0) || ((m->y()/sc) > doku->PageH))
				qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png")), true);
			else
				qApp->setOverrideCursor(QCursor(SplitHCursor), true);
			return;
		}
		if (MoveGX)
		{
			FromVRuler(m);
			if (((m->x()/sc) < 0) || ((m->x()/sc) > doku->PageB))
				qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png")), true);
			else
				qApp->setOverrideCursor(QCursor(SplitVCursor), true);
			return;
		}
	}
	if ((!doku->DragP) && (Mpressed) && (SelItem.count() != 0))
	{
		int cx = Anz->contentsX() - Anz->childX(doku->ActPage->parentWidget());
		int cy = Anz->contentsY() - Anz->childY(doku->ActPage->parentWidget());
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
	}
	if (Mpressed && (doku->AppMode == 21))
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
		p.begin(this);
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
		if ((Mpressed) && (m->state() == RightButton) && (!doku->DragP) && (doku->AppMode == 1) && (!b->Locked) && (!((b->isTableItem) && (b->isSingleSel))))
		{
			if ((abs(Dxp - newX) > 5) || (abs(Dyp - newY) > 5))
			{
				doku->DragP = true;
				doku->leaveDrag = false;
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
			return;
		}
		if (doku->DragP)
			return;
		if (Mpressed && (doku->AppMode == 9))
		{
			double newW = xy2Deg(m->x()/sc - RCenter.x(), m->y()/sc - RCenter.y());
			if (GroupSel)
				RotateGroup(newW - oldW);
			else
				RotateItem(b->Rot - (oldW - newW), b->ItemNr);
			oldW = newW;
			emit DocChanged();
		}
		if (doku->AppMode == 13)
		{
			p.begin(this);
			Transform(b, &p);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (doku->useRaster)
			{
				newX = static_cast<int>(qRound(newX / doku->minorGrid) * doku->minorGrid);
				newY = static_cast<int>(qRound(newY / doku->minorGrid) * doku->minorGrid);
			}
			if (!Mpressed)
			{
				BezierPoints(&Bez, b->PoLine.pointQ(b->PoLine.size()-2), b->PoLine.pointQ(b->PoLine.size()-1),
				             p.xFormDev(QPoint(static_cast<int>(Mxp*sc),
				                               static_cast<int>(Myp*sc))),
				             p.xFormDev(QPoint(static_cast<int>(Mxp*sc),
				                               static_cast<int>(Myp*sc))));
				p.drawCubicBezier(Bez);
				Bez.setPoint(2, p.xFormDev(QPoint(static_cast<int>(newX*sc), static_cast<int>(newY*sc))));
				Bez.setPoint(3, p.xFormDev(QPoint(static_cast<int>(newX*sc), static_cast<int>(newY*sc))));
				p.drawCubicBezier(Bez);
			}
			else
			{
				if (b->PoLine.size() > 2)
				{
					BezierPoints(&Bez, b->PoLine.pointQ(b->PoLine.size()-3), b->PoLine.pointQ(b->PoLine.size()-2),
					             p.xFormDev(QPoint(static_cast<int>(Mxp*sc),
					                               static_cast<int>(Myp*sc))),
					             b->PoLine.pointQ(b->PoLine.size()-1));
					p.drawCubicBezier(Bez);
					Bez.setPoint(2, p.xFormDev(QPoint(static_cast<int>(newX*sc), static_cast<int>(newY*sc))));
					Bez.setPoint(3, b->PoLine.pointQ(b->PoLine.size()-1));
					p.drawCubicBezier(Bez);
				}
				p.drawLine(b->PoLine.pointQ(b->PoLine.size()-1), p.xFormDev(QPoint(static_cast<int>(Mxp*sc),
				           static_cast<int>(Myp*sc))));
				p.drawLine(b->PoLine.pointQ(b->PoLine.size()-1), p.xFormDev(QPoint(static_cast<int>(newX*sc),
				           static_cast<int>(newY*sc))));
			}
			p.end();
			Mxp = newX;
			Myp = newY;
		}
		if (Mpressed && (doku->AppMode == 12))
		{
			p.begin(this);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (m->state() & ShiftButton)
			{
				mop = QPoint(m->x(), static_cast<int>((b->Ypos + (newX - b->Xpos)) * sc));
				QCursor::setPos(mapToGlobal(mop));
				newY = static_cast<int>(mop.y()/sc);
			}
			if (doku->useRaster)
			{
				newX = static_cast<int>(qRound(newX / doku->minorGrid) * doku->minorGrid);
				newY = static_cast<int>(qRound(newY / doku->minorGrid) * doku->minorGrid);
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
		if (Mpressed && (doku->AppMode == 8))
		{
			p.begin(this);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			if (doku->useRaster)
			{
				newX = static_cast<int>(qRound(newX / doku->minorGrid) * doku->minorGrid);
				newY = static_cast<int>(qRound(newY / doku->minorGrid) * doku->minorGrid);
			}
			p.drawLine(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc), static_cast<int>(Mxp*sc),
			           static_cast<int>(Myp*sc));
			p.drawLine(static_cast<int>(b->Xpos*sc), static_cast<int>(b->Ypos*sc), static_cast<int>(newX*sc),
			           static_cast<int>(newY*sc));
			p.end();
			emit SetAngle(xy2Deg(newX - b->Xpos, newY - b->Ypos));
			emit ItemGeom(sqrt(pow(newX - b->Xpos,2)+pow(newY - b->Ypos,2)), 0);
			Mxp = newX;
			Myp = newY;
		}
		if (Mpressed && (doku->AppMode == 7) && (!HanMove))
		{
			if (b->PType == 2)
			{
				MoveItemI((newX-Mxp)/b->LocalScX, (newY-Myp)/b->LocalScY, b->ItemNr);
				Mxp = newX;
				Myp = newY;
			}
			if (b->PType == 4)
			{
				for (a = 0; a < b->Ptext.count(); ++a)
					b->Ptext.at(a)->cselect = false;
				b->HasSel = false;
				slotSetCurs(m->x(), m->y());
				if (b->CPos < oldCp)
				{
					for (c = b->CPos; c < oldCp; ++c)
					{
						b->Ptext.at(c)->cselect = true;
						b->HasSel = true;
					}
				}
				if (b->CPos > oldCp)
				{
					for (c = oldCp; c < b->CPos; ++c)
					{
						b->Ptext.at(c)->cselect = true;
						b->HasSel = true;
					}
				}
				RefreshItem(b);
				emit b->HasSel ? HasTextSel() : HasNoTextSel();
			}
		}
		if (Mpressed && ((doku->AppMode == 1) || ((doku->AppMode == 7) && HanMove)) && (!b->Locked))
		{
			if (doku->EditClip)
			{
				if ((Mpressed) && (ClRe == -1) && (SegP1 == -1) && (SegP2 == -1))
				{
					newX = m->x();
					newY = m->y();
					p.begin(this);
					p.setRasterOp(XorROP);
					p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
					p.drawRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc),
					           static_cast<int>(SeRx-(Mxp*sc)), static_cast<int>(SeRy-(Myp*sc)));
					p.drawRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc),
					           static_cast<int>(newX-(Mxp*sc)), static_cast<int>(newY-(Myp*sc)));
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
				p.begin(this);
				Transform(b, &p);
				npf = FPoint(p.xFormDev(m->pos()));
				p.end();
				if ((SegP1 != -1) && (SegP2 != -1))
				{
					FPointArray Clip;
					Clip = b->PoLine;
					p.begin(this);
					p.translate(static_cast<int>(b->Xpos*doku->Scale), static_cast<int>(b->Ypos*doku->Scale));
					p.rotate(b->Rot);
					FPoint npfN = FPoint(p.xFormDev(QPoint(newX, newY)));
					FPoint npfM = FPoint(p.xFormDev(QPoint(Mxp, Myp)));
					npf.setX(Clip.point(SegP2).x() + (npfN.x()-npfM.x()));
					npf.setY(Clip.point(SegP2).y() + (npfN.y()-npfM.y()));
					ClRe = SegP2;
					MoveClipPoint(b, npf);
					b->OldB2 = b->Width;
					b->OldH2 = b->Height;
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
						for (uint itm = 0; itm < SelNode.count(); ++itm)
						{
							p.begin(this);
							p.translate(static_cast<int>(b->Xpos*doku->Scale),
							            static_cast<int>(b->Ypos*doku->Scale));
							p.rotate(b->Rot);
							FPoint npfN = FPoint(p.xFormDev(QPoint(newX, newY)));
							FPoint npfM = FPoint(p.xFormDev(QPoint(Mxp, Myp)));
							p.end();
							npf.setX(b->PoLine.point(*SelNode.at(itm)).x() + (npfN.x()-npfM.x()));
							npf.setY(b->PoLine.point(*SelNode.at(itm)).y() + (npfN.y()-npfM.y()));
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
					p.begin(this);
					switch (HowTo)
					{
					case 1:
						if (m->state() & ControlButton)
						{
							np2 = QPoint(m->x(), static_cast<int>((gy+(gh * ((newX-gx) / gw)))*sc));
							QCursor::setPos(mapToGlobal(np2));
						}
						else
							np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(QPoint(qRound(gx*sc), qRound(gy*sc)), np2));
						break;
					case 2:
						np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(np2, QPoint(qRound((gx+gw)*sc), qRound((gy+gh)*sc))));
						break;
					case 3:
						np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(np2, QPoint(qRound(gx*sc), qRound((gy+gh)*sc))));
						break;
					case 4:
						np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(np2, QPoint(qRound((gx+gw)*sc), qRound(gy*sc))));
						break;
					case 5:
						np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(qRound(gx*sc), qRound(gy*sc), qRound(gw*sc),  np2.y()-qRound(gy*sc)));
						break;
					case 6:
						np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(qRound(gx*sc), qRound(gy*sc),  np2.x()-qRound(gx*sc), qRound(gh*sc)));
						break;
					case 7:
						np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(QPoint(np2.x(), qRound(gy*sc)), QPoint(qRound((gx+gw)*sc), qRound((gy+gh)*sc))));
						break;
					case 8:
						np2 = QPoint(m->x(), m->y());
						PaintSizeRect(&p, QRect(QPoint(qRound(gx*sc), np2.y()), QPoint(qRound((gx+gw)*sc), qRound((gy+gh)*sc))));
						break;
					}
					Mxp = qRound(np2.x()/sc);
					Myp = qRound(np2.y()/sc);
					p.end();
				}
				else
				{
					for (a = 0; a < SelItem.count(); ++a)
					{
						QWMatrix ma;
						b = SelItem.at(0);
						switch (HowTo)
						{
						case 0:
							break;
						case 1:
							p.begin(this);
							Transform(b, &p);
							if ((m->state() & ShiftButton) && (!(m->state() & ControlButton)))
							{
								mop = QPoint(m->x(), static_cast<int>((b->Ypos + (newX - b->Xpos)) * sc));
								QCursor::setPos(mapToGlobal(mop));
							}
							else
							{
								if ((m->state() & ControlButton) && (!(m->state() & ShiftButton)))
								{
									mop = QPoint(m->x(), static_cast<int>((b->Ypos + ((newX - b->Xpos) / b->OldB2 * b->OldH2)) * sc));
									QCursor::setPos(mapToGlobal(mop));
								}
								else
									mop = QPoint(m->x(), m->y());
							}
							np = p.xFormDev(mop);
							nx = np.x();
							ny = np.y();
							p.end();
							if (b->PType != 5)
							{
								if (doku->useRaster)
								{
									dx = b->Xpos - int (b->Xpos / doku->minorGrid) * doku->minorGrid;
									dy = b->Ypos - int (b->Ypos / doku->minorGrid) * doku->minorGrid;
									nx = (qRound(np.x() / doku->minorGrid) * doku->minorGrid - dx);
									ny = (qRound(np.y() / doku->minorGrid) * doku->minorGrid - dy);
								}
								erf = SizeItem(nx, ny, b->ItemNr);
							}
							else
							{
								p.begin(this);
								double rba = b->Rot;
								b->Rot = 0;
								Transform(b, &p);
								np = p.xFormDev(QPoint(m->x(), m->y()));
								p.end();
								b->Rot = rba;
								np = ApplyGrid(np);
								erf = SizeItem(np.x(), np.y(), b->ItemNr);
								if (doku->SnapGuides)
								{
									p.begin(this);
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
							if (b->PType == 5)
							{
								p.begin(this);
								Transform(b, &p);
								mop = QPoint(m->x(), m->y());
								npf = p.xFormDev(mop);
								nx = np.x();
								ny = np.y();
								p.end();
								double sav = doku->SnapGuides;
								npf2 = FPoint(nx-Mxp, ny-Myp);
								erf = MoveSizeItem(npf, npf, b->ItemNr);
								doku->SnapGuides = sav;
								if (sav)
									b->Sizing = true;
							}
							else
							{
								p.begin(this);
								Transform(b, &p);
								ma = p.worldMatrix();
								ma.setTransformationMode ( QWMatrix::Areas );
								p.setWorldMatrix(ma);
								np2 = ApplyGrid(p.xFormDev(QPoint(m->x(), m->y())));
								PaintSizeRect(&p, QRect(np2, QPoint(qRound(b->Width), qRound(b->Height))));
								p.end();
							}
							break;
						case 3:
							p.begin(this);
							Transform(b, &p);
							ma = p.worldMatrix();
							ma.setTransformationMode ( QWMatrix::Areas );
							p.setWorldMatrix(ma);
							np2 = ApplyGrid(p.xFormDev(QPoint(m->x(), m->y())));
							PaintSizeRect(&p, QRect(np2, QPoint(0, qRound(b->Height))));
							p.end();
							break;
						case 4:
							p.begin(this);
							Transform(b, &p);
							ma = p.worldMatrix();
							ma.setTransformationMode ( QWMatrix::Areas );
							p.setWorldMatrix(ma);
							np2 = ApplyGrid(p.xFormDev(QPoint(m->x(), m->y())));
							PaintSizeRect(&p, QRect(np2, QPoint(qRound(b->Width), 0)));
							p.end();
							break;
						case 5:
							{
							p.begin(this);
							Transform(b, &p);
							ma = p.worldMatrix();
							ma.setTransformationMode ( QWMatrix::Areas );
							p.setWorldMatrix(ma);
							np2 = ApplyGrid(p.xFormDev(QPoint(m->x(), m->y())));
							PaintSizeRect(&p, QRect(0, 0, qRound(b->Width), np2.y()));
							p.end();
							break;
							}
						case 6:
							p.begin(this);
							Transform(b, &p);
							ma = p.worldMatrix();
							ma.setTransformationMode ( QWMatrix::Areas );
							p.setWorldMatrix(ma);
							np2 = ApplyGrid(p.xFormDev(QPoint(m->x(), m->y())));
							PaintSizeRect(&p, QRect(QPoint(np2.x(), qRound(b->Height)), QPoint(0, 0)));
							p.end();
							break;
						case 7:
							p.begin(this);
							Transform(b, &p);
							ma = p.worldMatrix();
							ma.setTransformationMode ( QWMatrix::Areas );
							p.setWorldMatrix(ma);
							np2 = ApplyGrid(p.xFormDev(QPoint(m->x(), m->y())));
							PaintSizeRect(&p, QRect(QPoint(np2.x(), qRound(b->Height)), QPoint(qRound(b->Width), 0)));
							p.end();
							break;
						case 8:
							p.begin(this);
							Transform(b, &p);
							ma = p.worldMatrix();
							ma.setTransformationMode ( QWMatrix::Areas );
							p.setWorldMatrix(ma);
							np2 = ApplyGrid(p.xFormDev(QPoint(m->x(), m->y())));
							PaintSizeRect(&p, QRect(QPoint(qRound(b->Width),np2.y()), QPoint(0, qRound(b->Height))));
							p.end();
							break;
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
				if (!GroupSel)
				{
					doku->UnData.UnCode = 1;
					doku->UnDoValid = true;
					emit UndoAvail();
				}
				else
				{
					doku->UnDoValid = false;
					emit UndoAvail();
				}
				Imoved = true;
				for (a = 0; a < SelItem.count(); ++a)
				{
					b = SelItem.at(a);
					if ((b->isTableItem) && (b->isSingleSel))
						continue;
					p.begin(this);
					Transform(b, &p);
					p.setRasterOp(XorROP);
					p.setBrush(NoBrush);
					p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
					if ((b->PType != 5) && (b->FrameType != 0) || (b->PType == 7))
						b->DrawPolyL(&p, b->Clip);
					else
						p.drawRect(0, 0, static_cast<int>(b->Width)+1, static_cast<int>(b->Height)+1);
					p.end();
					erf = MoveItem(newX-Mxp, newY-Myp, b);
					p.begin(this);
					Transform(b, &p);
					p.setRasterOp(XorROP);
					p.setBrush(NoBrush);
					p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
					if ((b->PType != 5) && (b->FrameType != 0) || (b->PType == 7))
						b->DrawPolyL(&p, b->Clip);
					else
						p.drawRect(0, 0, static_cast<int>(b->Width)+1, static_cast<int>(b->Height)+1);
					p.end();
				}
				if (erf)
				{
					Mxp = newX;
					Myp = newY;
				}
			}
		}
		if ((!Mpressed) && (doku->AppMode != 13))
		{
			if (GroupSel)
			{
				QRect mpo = QRect(m->x()-doku->GrabRad, m->y()-doku->GrabRad, doku->GrabRad*2, doku->GrabRad*2);
				double gx, gy, gh, gw;
				getGroupRectScreen(&gx, &gy, &gw, &gh);
				if (QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
				{
					qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
					if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
					if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
					if (QRect(static_cast<int>(gx+gw/2)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
					if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh/2)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
					if (QRect(static_cast<int>(gx+gw/2)-6, static_cast<int>(gy)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
					if (QRect(static_cast<int>(gx)-6, static_cast<int>(gy)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
					if (QRect(static_cast<int>(gx)-6, static_cast<int>(gy+gh/2)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
					if (QRect(static_cast<int>(gx)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
						qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
					if (doku->AppMode == 9)
						qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
				}
				else
					qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				return;
			}
			for (a = 0; a < doku->ActPage->SelItem.count(); ++a)
			{
				b = doku->ActPage->SelItem.at(a);
				if (b->Locked)
					break;
				p.begin(this);
				Transform(b, &p);
				QRegion ganz = ViewReg();
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				if (ganz.intersect(QRegion(p.xForm(QPointArray(QRect(-3, -3, static_cast<int>(b->Width+6),
				                                   static_cast<int>(b->Height+6)))))).contains(m->pos()))
				{
					if (doku->EditClip)
					{
						QRect mpo = p.xFormDev(QRect(m->x()-doku->GrabRad, m->y()-doku->GrabRad,
						                             doku->GrabRad*2, doku->GrabRad*2));
						FPointArray Clip;
						QPointArray cli;
						ClRe2 = -1;
						SegP1 = -1;
						SegP2 = -1;
						Clip = b->PoLine;
						if ((doku->EditClipMode == 2) || (doku->EditClipMode == 0) || (doku->EditClipMode == 3))
						{
							np2 = p.xFormDev(m->pos());
							for (a=0; a<Clip.size(); ++a)
							{
								if (((EdPoints) && (a % 2 != 0)) || ((!EdPoints) && (a % 2 == 0)))
									continue;
								np = p.xForm(Clip.pointQ(a));
								tx = QRect(np.x()-3, np.y()-3, 6, 6);
								if (tx.contains(m->pos()))
								{
									if (doku->EditClipMode == 0)
										qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
									if (doku->EditClipMode == 2)
										qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
									if (doku->EditClipMode == 3)
										qApp->setOverrideCursor(QCursor(loadIcon("Split.png"), 4, 3), true);
									p.end();
									return;
								}
							}
						}
						if ((doku->EditClipMode == 1) || (doku->EditClipMode == 0) && (EdPoints))
						{
							for (uint poi=0; poi<Clip.size()-3; poi += 4)
							{
								BezierPoints(&Bez, Clip.pointQ(poi), Clip.pointQ(poi+1), Clip.pointQ(poi+3),
								             Clip.pointQ(poi+2));
								cli = Bez.cubicBezier();
								for (uint clp = 0; clp < cli.size()-1; ++clp)
								{
									if (PointOnLine(cli.point(clp), cli.point(clp+1), mpo))
									{
										if (doku->EditClipMode == 0)
											qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
										if (doku->EditClipMode == 1)
											qApp->setOverrideCursor(QCursor(loadIcon("AddPoint.png"), 4, 3), true);
										ClRe2 = poi;
										p.end();
										return;
									}
								}
							}
						}
					}
					QWMatrix ma;
					ma.scale(doku->Scale, doku->Scale);
					ma.translate(b->Xpos, b->Ypos);
					ma.rotate(b->Rot);
					ma.setTransformationMode ( QWMatrix::Areas );
					p.setWorldMatrix(ma);
					tx = p.xForm(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height)));
					if (tx.contains(m->pos()))
					{
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
						if (doku->AppMode == 9)
							qApp->setOverrideCursor(QCursor(loadIcon("Rotieren2.xpm")), true);
						if (doku->AppMode == 7)
						{
							if (b->PType == 4)
								qApp->setOverrideCursor(QCursor(ibeamCursor), true);
							if (b->PType == 2)
								qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
						}
						QRect mpo = QRect(m->x()-doku->GrabRad, m->y()-doku->GrabRad, doku->GrabRad*2, doku->GrabRad*2);
						HandleCurs(&p, b, mpo);
					}
					else
						qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				}
				p.end();
			}
		}
	}
	else
	{
		if (Mpressed)
		{
			newX = m->x();
			newY = m->y();
			p.begin(this);
			p.setRasterOp(XorROP);
			p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
			p.drawRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>(SeRx-(Mxp*sc)),
			           static_cast<int>(SeRy-(Myp*sc)));
			p.drawRect(static_cast<int>(Mxp*sc), static_cast<int>(Myp*sc), static_cast<int>(newX-(Mxp*sc)),
			           static_cast<int>(newY-(Myp*sc)));
			p.end();
			SeRx = newX;
			SeRy = newY;
			HaveSelRect = true;
		}
		else
		{
			if ((ScApp->Prefs.GuidesShown) && (doku->AppMode == 1))
			{
				if (YGuides.count() != 0)
				{
					for (uint yg = 0; yg < YGuides.count(); ++yg)
					{
						if ((YGuides[yg] < (static_cast<int>(m->y()/sc)+doku->GuideRad)) &&
						        (YGuides[yg] > (static_cast<int>(m->y()/sc)-doku->GuideRad)))
						{
							if (((m->x()/sc) < 0) || ((m->x()/sc) >= doku->PageB-1))
								qApp->setOverrideCursor(QCursor(ArrowCursor), true);
							else
								qApp->setOverrideCursor(QCursor(SplitHCursor), true);
							return;
						}
					}
					qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				}
				if (XGuides.count() != 0)
				{
					for (uint xg = 0; xg < XGuides.count(); ++xg)
					{
						if ((XGuides[xg] < (static_cast<int>(m->x()/sc)+doku->GuideRad)) &&
						        (XGuides[xg] > (static_cast<int>(m->x()/sc)-doku->GuideRad)))
						{
							if (((m->y()/sc) < 0) || ((m->y()/sc) >= doku->PageH-1))
								qApp->setOverrideCursor(QCursor(ArrowCursor), true);
							else
								qApp->setOverrideCursor(QCursor(SplitVCursor), true);
							return;
						}
					}
					qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				}
			}
		}
	}
}

void Page::mousePressEvent(QMouseEvent *m)
{
	double sc;
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
	FPoint npf, npf2;
	QPointArray Bez(4);
	QRect tx, mpo;
	sc = doku->Scale;
	Mpressed = true;
	Imoved = false;
	SeRx = m->x();
	SeRy = m->y();
	HaveSelRect = false;
	doku->DragP = false;
	doku->leaveDrag = false;
	Mxp = qRound(m->x()/sc);
	Myp = qRound(m->y()/sc);
	mpo = QRect(m->x()-doku->GrabRad, m->y()-doku->GrabRad, doku->GrabRad*2, doku->GrabRad*2);
	if (doku->AppMode != 7)
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
	switch (doku->AppMode)
	{
	case 1:
		if ((doku->EditClip) && (SelItem.count() != 0))
		{
			b = SelItem.at(0);
			FPointArray Clip;
			bool edited = false;
			bool pfound = false;
			Clip = b->PoLine;
			p.begin(this);
			Transform(b, &p);
			npf2 = FPoint(p.xFormDev(m->pos()));
			ClRe = -1;
			for (a=0; a<Clip.size(); ++a)
			{
				if (((EdPoints) && (a % 2 != 0)) || ((!EdPoints) && (a % 2 == 0)))
					continue;
				npf = FPoint(p.xForm(Clip.pointQ(a)));
				tx = QRect(static_cast<int>(npf.x()-3), static_cast<int>(npf.y()-3), 6, 6);
				if (tx.contains(m->pos()))
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
			if ((doku->EditClipMode == 0) && (ClRe2 != -1) && (ClRe == -1))
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
			if (doku->EditClipMode == 3)
			{
				if (!EdPoints)
					return;
				if (ClRe != -1)
				{
					if (b->PType == 6)
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
								z = PaintPoly(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth,
								              b->Pcolor, b->Pcolor2);
								bb = Items.at(z);
								bb->PoLine.resize(0);
								if (StartInd != 0)
								{
									bb->PoLine.putPoints(0, StartInd - 4, Clip);
									bb->PoLine.putPoints(bb->PoLine.size(), Clip.size()-EndInd, Clip, EndInd);
								}
								else
									bb->PoLine.putPoints(0, Clip.size()-EndInd-4, Clip, EndInd+4);
								bb->Rot = b->Rot;
								AdjustItemSize(bb);
								bb->ClipEdited = true;
								b->Dirty = true;
								PageItem *bx = Items.take(bb->ItemNr);
								Items.insert(bb->ItemNr-1, bx);
								for (uint al = 0; al < Items.count(); ++al)
								{
									Items.at(al)->ItemNr = al;
									if (Items.at(al)->isBookmark)
										emit NewBMNr(Items.at(al)->BMnr, al);
								}
							}
							b->PoLine = cli.copy();
						}
						ClRe = -1;
						b->ClipEdited = true;
						edited = true;
						doku->EditClipMode = 0;
						b->PType = 7;
						SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
						emit PolyOpen();
					}
					else
					{
						if ((b->PType == 7) || (b->PType == 8))
						{
							if ((ClRe > 1) && (ClRe < static_cast<int>(Clip.size()-2)))
							{
								z = PaintPolyLine(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth,
								                  b->Pcolor, b->Pcolor2);
								bb = Items.at(z);
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
							doku->EditClipMode = 0;
							SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
							emit PolyOpen();
						}
					}
				}
			}
			if ((doku->EditClipMode == 2) && (ClRe != -1))
			{
				if (!EdPoints)
					return;
				if ((b->PType == 6) || (b->PType == 4) || (b->PType == 2))
				{
					if ((b->Segments.count() == 0) && (Clip.size() < 13))
						return;
				}
				else
				{
					if (Clip.size() < 9)
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
						if ((b->PType == 6) || (b->PType == 4) || (b->PType == 2))
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
				b->PoLine = cli.copy();
				ClRe = -1;
				b->ClipEdited = true;
				edited = true;
			}
			if ((doku->EditClipMode == 1) && (ClRe2 != -1))
			{
				cli.putPoints(0, ClRe2+2, Clip);
				cli.resize(cli.size()+4);
				cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(),
				              npf2.y(), npf2.x(), npf2.y());
				cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
				b->PoLine = cli.copy();
				ClRe2 = -1;
				b->ClipEdited = true;
				edited = true;
			}
			if (edited)
			{
				if (b->PType != 7)
					b->Clip = FlattenPath(b->PoLine, b->Segments);
				AdjustItemSize(b);
				RefreshItem(b);
				update();
				emit PStatus(b->PType, b->PoLine.size());
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
				p.begin(this);
				QRect ne = QRect();
				PaintSizeRect(&p, ne);
				p.end();
				double gx, gy, gh, gw;
				getGroupRectScreen(&gx, &gy, &gw, &gh);
				if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw),
				           static_cast<int>(gh)).intersects(mpo)) && (m->state() != (ControlButton | AltButton)) && (m->state() != ShiftButton))
				{
					HowTo = 0;
					if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
					{
						HowTo = 1;
						qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
					}
					if (QRect(static_cast<int>(gx)-6, static_cast<int>(gy)-6, 6, 6).intersects(mpo))
					{
						HowTo = 2;
						qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
					}
					if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy)-6, 6, 6).intersects(mpo))
					{
						HowTo = 3;
						qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
					}
					if (QRect(static_cast<int>(gx)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
					{
						HowTo = 4;
						qApp->setOverrideCursor(QCursor(SizeBDiagCursor), true);
					}
					if (QRect(static_cast<int>(gx+gw/2)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
					{
						HowTo = 5;
						qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
					}
					if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh/2)-6, 6, 6).intersects(mpo))
					{
						HowTo = 6;
						qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
					}
					if (QRect(static_cast<int>(gx)-6, static_cast<int>(gy+gh/2)-6, 6, 6).intersects(mpo))
					{
						HowTo = 7;
						qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
					}
					if (QRect(static_cast<int>(gx+gw/2)-6, static_cast<int>(gy)-6, 6, 6).intersects(mpo))
					{
						HowTo = 8;
						qApp->setOverrideCursor(QCursor(SizeVerCursor), true);
					}
					if (b->LockRes)
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
				p.begin(this);
				Transform(b, &p);
				QWMatrix ma;
				ma.scale(doku->Scale, doku->Scale);
				ma.translate(b->Xpos, b->Ypos);
				ma.rotate(b->Rot);
				ma.setTransformationMode ( QWMatrix::Areas );
				p.setWorldMatrix(ma);
				if (!QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))))).contains(mpo))
				{
					SeleItem(m);
					if (SelItem.count() != 0)
						storeUndoInf(SelItem.at(0));
				}
				else
				{
					SeleItem(m);
					if (!b->Locked)
					{
						HandleSizer(&p, b, mpo);
						if (HowTo != 0)
						{
							doku->UnData.UnCode = 2;
							if (b->PType != 5)
								b->Sizing = true;
							mCG = true;
						}
						else
							doku->UnData.UnCode = 1;
						doku->UnDoValid = true;
						emit UndoAvail();
					}
				}
				p.end();
			}
		}
		else
		{
			SeleItem(m);
			if (SelItem.count() != 0)
				storeUndoInf(SelItem.at(0));
		}
		if (m->button() == MidButton)
		{
			MidButt = true;
			if (SelItem.count() != 0)
				Deselect(true);
			repaint();
		}
		if ((SelItem.count() != 0) && (m->button() == RightButton))
		{
			Mpressed = true;
			Dxp = Mxp;
			Dyp = Mxp;
		}
		break;
	case 2:
		SeleItem(m);
		Deselect(false);
		switch (doku->SubMode)
		{
		case 0:
			z = PaintRect(Rxp, Ryp, 1+Rxpd, 1+Rypd, doku->Dwidth, doku->Dbrush, doku->Dpen);
			SetupDraw(z);
			break;
		case 1:
			z = PaintEllipse(Rxp, Ryp, 1+Rxpd, 1+Rypd, doku->Dwidth, doku->Dbrush, doku->Dpen);
			SetupDraw(z);
			break;
		default:
			z = PaintPoly(Rxp, Ryp, 1+Rxpd, 1+Rypd, doku->Dwidth, doku->Dbrush, doku->Dpen);
			SetFrameShape(Items.at(z), doku->ValCount, doku->ShapeValues);
			Items.at(z)->FrameType = doku->SubMode+2;
			SetupDraw(z);
			break;
		}
		emit HaveSel(6);
		break;
	case 4:
		SeleItem(m);
		Deselect(false);
		z = PaintPict(Rxp, Ryp, 1+Rxpd, 1+Rypd);
		SetupDraw(z);
		emit HaveSel(2);
		break;
	case 5:
		SeleItem(m);
		Deselect(false);
		z = PaintText(Rxp, Ryp, 1+Rxpd, 1+Rypd, doku->Dwidth, doku->DpenText);
		SetupDraw(z);
		emit HaveSel(4);
		break;
	case 6:
		Mpressed = true;
		if (m->state() == ShiftButton)
			Magnify = false;
		else
			Magnify = true;
		break;
	case 7:
		HowTo = 0;
		HanMove = false;
		slotDoCurs(false);
		if (GetItem(&b))
		{
			if (!b->Locked)
			{
				p.begin(this);
				Transform(b, &p);
				HandleSizer(&p, b, mpo);
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
			if (!SeleItem(m))
			{
				slotDoCurs(true);
				emit Amode(1);
				return;
			}
		}
		b = doku->ActPage->SelItem.at(0);
		oldCp = b->CPos;
		slotDoCurs(true);
		if ((!inText) && ((b->PType == 4) || (b->PType == 2)))
		{
			Deselect(true);
			if (SeleItem(m))
			{
				b = doku->ActPage->SelItem.at(0);
				if ((b->PType == 4) || (b->PType == 2))
					emit Amode(7);
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
		break;
	case 8:
		SeleItem(m);
		Deselect(false);
		z = PaintLine(Rxp, Ryp, 1+Rxpd, Rypd, doku->DwidthLine, doku->DpenLine);
		b = Items.at(z);
		b->Select = true;
		qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
		SelItem.clear();
		SelItem.append(b);
		b->paintObj();
		Imoved = true;
		emit ItemPos(b->Xpos, b->Ypos);
		emit SetSizeValue(b->Pwidth);
		emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
		emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
		emit ItemGradient(b->GrColor2, b->GrColor, b->GrShade2, b->GrShade, b->GrType);
		emit ItemTrans(b->Transparency, b->TranspStroke);
		emit HaveSel(5);
		break;
	case 9:
		if (GetItem(&b))
		{
			RotMode = doku->RotMode;
			if (GroupSel)
			{
				double gx, gy, gh, gw;
				double gxR, gyR, ghR, gwR;
				getGroupRectScreen(&gx, &gy, &gw, &gh);
				getGroupRect(&gxR, &gyR, &gwR, &ghR);
				if (QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw),
				          static_cast<int>(gh)).intersects(mpo))
				{
					doku->RotMode = 2;
					RCenter = FPoint(gxR+gwR/2.0, gyR+ghR/2.0);
					if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
					{
						RCenter = FPoint(gxR, gyR);
						doku->RotMode = 0;
					}
				}
				oldW = xy2Deg(m->x()/sc - RCenter.x(), m->y()/sc - RCenter.y());
				doku->UnDoValid = false;
				emit UndoAvail();
			}
			else
			{
				p.begin(this);
				Transform(b, &p);
				doku->RotMode = 2;
				RCenter = transformPointI(FPoint(b->Xpos+b->Width/2, b->Ypos+b->Height/2), 0, 0, b->Rot, 1, 1);
				if (QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width),
				                                      static_cast<int>(b->Height))))).contains(mpo))
				{
					if (p.xForm(QRect(static_cast<int>(b->Width)-6, static_cast<int>(b->Height)-6,
					                  6, 6)).intersects(mpo))
					{
						RCenter = FPoint(b->Xpos, b->Ypos);
						doku->RotMode = 0;
					}
					if (p.xForm(QRect(0, 0, 6, 6)).intersects(mpo))
					{
						RCenter = transformPointI(FPoint(b->Xpos+b->Width, b->Ypos+b->Height), 0, 0,
						                          b->Rot, 1, 1);
						doku->RotMode = 4;
					}
					if (p.xForm(QRect(0, static_cast<int>(b->Height)-6, 6, 6)).intersects(mpo))
					{
						RCenter = transformPointI(FPoint(b->Xpos+b->Width, b->Ypos), 0, 0, b->Rot, 1, 1);
						doku->RotMode = 1;
					}
					if (p.xForm(QRect(static_cast<int>(b->Width)-6, 0, 6, 6)).intersects(mpo))
					{
						RCenter = transformPointI(FPoint(b->Xpos, b->Ypos+b->Height), 0, 0, b->Rot, 1, 1);
						doku->RotMode = 3;
					}
					oldW = xy2Deg(m->x()/sc - RCenter.x(), m->y()/sc - RCenter.y());
					doku->UnData.UnCode = 3;
					storeUndoInf(b);
					doku->UnDoValid = true;
					emit UndoAvail();
					p.end();
				}
			}
		}
		break;
	case 10:
		b = doku->ElemToLink;
		SeleItem(m);
		if (GetItem(&bb))
		{
			if ((bb->Ptext.count() == 0) && (bb->NextBox == 0) && (bb->BackBox == 0) && (b != bb))
			{
				b->NextBox = bb;
				bb->BackBox = b;
				if ((bb->ItemNr < b->ItemNr) && (bb->OwnPage == b->OwnPage))
				{
					Items.insert(b->ItemNr+1, bb);
					bb = Items.take(bb->ItemNr);
					for (uint a = 0; a < Items.count(); ++a)
					{
						Items.at(a)->ItemNr = a;
						if (Items.at(a)->isBookmark)
							emit NewBMNr(Items.at(a)->BMnr, a);
					}
				}
				b->OwnPage->repaint();
			}
			emit DocChanged();
		}
		doku->ElemToLink = 0;
		break;
	case 11:
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
						uint a = nb->Ptext.count();
						for (uint s=0; s<a; ++s)
							b->Ptext.append(nb->Ptext.take(0));
						nb = nb->NextBox;
					}
				}
				uint a2 = b->Ptext.count();
				for (uint s = 0; s < a2; ++s)
					b->BackBox->Ptext.append(b->Ptext.take(0));
				b->BackBox->NextBox = 0;
				b->BackBox = 0;
			}
			emit DocChanged();
			b->OwnPage->repaint();
		}
		break;
	case 12:
		{
			SeleItem(m);
			Deselect(false);
			z = PaintPoly(Rxp, Ryp, 1+Rxpd, 1+Rypd, doku->Dwidth, doku->Dbrush, doku->Dpen);
			b = Items.at(z);
			FPointArray cli = RegularPolygonF(b->Width, b->Height, PolyC, PolyS, PolyF, PolyR);
			FPoint np = FPoint(cli.point(0));
			b->PoLine.resize(2);
			b->PoLine.setPoint(0, np);
			b->PoLine.setPoint(1, np);
			for (uint ax = 1; ax < cli.size(); ++ax)
			{
				np = FPoint(cli.point(ax));
				b->PoLine.putPoints(b->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(),
				                    np.x(), np.y());
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
			emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
			emit ItemGradient(b->GrColor2, b->GrColor, b->GrShade2, b->GrShade, b->GrType);
			emit ItemTrans(b->Transparency, b->TranspStroke);
			emit HaveSel(6);
			break;
		}
	case 13:
		if (m->button() == RightButton)
			break;
		if (FirstPoly)
		{
			SeleItem(m);
			Deselect(false);
			z = PaintPolyLine(Rxp, Ryp, 1+Rxpd, 1+Rypd, doku->Dwidth, "None", doku->Dpen);
			b = Items.at(z);
			SelItem.clear();
			SelItem.append(b);
			b->Select = true;
			qApp->setOverrideCursor(QCursor(crossCursor), true);
			FirstPoly = false;
		}
		b = SelItem.at(0);
		p.begin(this);
		Transform(b, &p);
		npf = FPoint(p.xFormDev(m->pos()));
		p.end();
		npf = ApplyGridF(npf);
		b->PoLine.addPoint(npf);
		npf2 = GetMinClipF(b->PoLine);
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
		SizeItem(b->PoLine.WidthHeight().x(), b->PoLine.WidthHeight().y(), b->ItemNr, false, false);
		SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
		b->paintObj();
		if (b->PoLine.size() > 2)
		{
			p.begin(this);
			Transform(b, &p);
			p.setPen(QPen(black, 1, DotLine, FlatCap, MiterJoin));
			BezierPoints(&Bez, b->PoLine.pointQ(b->PoLine.size()-3), b->PoLine.pointQ(b->PoLine.size()-2),
			             p.xFormDev(m->pos()), b->PoLine.pointQ(b->PoLine.size()-1));
			p.drawCubicBezier(Bez);
			p.end();
		}
		emit ItemPos(b->Xpos, b->Ypos);
		emit SetSizeValue(b->Pwidth);
		emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
		emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
		emit ItemGradient(b->GrColor2, b->GrColor, b->GrShade2, b->GrShade, b->GrType);
		emit ItemTrans(b->Transparency, b->TranspStroke);
		emit HaveSel(7);
		break;
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		SeleItem(m);
		Deselect(false);
		z = PaintText(Rxp, Ryp, 1+Rxpd, 1+Rypd, doku->Dwidth, doku->DpenText);
		b = Items.at(z);
		b->isAnnotation = true;
		switch (doku->AppMode)
		{
		case 14:
			b->AnType = 2;
			b->AnFlag = 65536;
			break;
		case 15:
			b->AnType = 3;
			break;
		case 16:
			b->AnType = 4;
			break;
		case 17:
			b->AnType = 5;
			b->AnFlag = 131072;
			break;
		case 18:
			b->AnType = 6;
			break;
		case 19:
			b->AnType = 10;
			break;
		case 20:
			b->AnType = 11;
			b->AnZiel = PageNr;
			b->AnAction = "0 0";
			b->Textflow = false;
			break;
		}
		SetupDraw(z);
		emit HaveSel(4);
		break;
	case 21:
		RecordP.resize(0);
		Deselect(false);
		break;
	case 22:
		Deselect(false);
		break;
	}
}

void Page::HandleCurs(QPainter *p, PageItem *b, QRect mpo)
{
	QWMatrix ma;
	ma.scale(doku->Scale, doku->Scale);
	ma.translate(b->Xpos, b->Ypos);
	ma.rotate(b->Rot);
	ma.setTransformationMode ( QWMatrix::Areas );
	p->setWorldMatrix(ma);
	QPoint tx, tx2;
	tx = p->xForm(QPoint(static_cast<int>(b->Width), 0));
	tx2 = p->xForm(QPoint(0, static_cast<int>(b->Height)));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (doku->AppMode == 9)
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
		if (doku->AppMode == 9)
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
	if (doku->EditClip)
		qApp->setOverrideCursor(QCursor(crossCursor), true);
}

void Page::HandleSizer(QPainter *p, PageItem *b, QRect mpo)
{
	b->OldB = b->Width;
	b->OldH = b->Height;
	b->OldB2 = b->Width;
	b->OldH2 = b->Height;
	HowTo = 0;
	if (b->LockRes)
		return;
	if (mpo.contains(p->xForm(QPoint(static_cast<int>(b->Width), static_cast<int>(b->Height)))))
		HowTo = 1;
	QWMatrix ma;
	ma.scale(doku->Scale, doku->Scale);
	ma.translate(b->Xpos, b->Ypos);
	ma.rotate(b->Rot);
	ma.setTransformationMode ( QWMatrix::Areas );
	p->setWorldMatrix(ma);
	QRect ne = QRect();
	PaintSizeRect(p, ne);
	if (b->PType != 5)
	{
		if (mpo.contains(p->xForm(QPoint(0, 0))))
			HowTo = 2;
		if (mpo.contains(p->xForm(QPoint(static_cast<int>(b->Width/2), 0))))
			HowTo = 8;
		if (mpo.contains(p->xForm(QPoint(0, static_cast<int>(b->Height)/2))))
			HowTo = 7;
		if (mpo.contains(p->xForm(QPoint(static_cast<int>(b->Width), static_cast<int>(b->Height)/2))))
			HowTo = 6;
		if (mpo.contains(p->xForm(QPoint(static_cast<int>(b->Width)/2, static_cast<int>(b->Height)))))
			HowTo = 5;
		if (mpo.contains(p->xForm(QPoint(0, static_cast<int>(b->Height)))))
			HowTo = 4;
		if (mpo.contains(p->xForm(QPoint(static_cast<int>(b->Width), 0))))
			HowTo = 3;
	}
	HandleCurs(p, b, mpo);
	storeUndoInf(b);
	if (HowTo != 0)
	{
		doku->UnData.UnCode = 2;
		if (b->PType != 5)
			b->Sizing = true;
		mCG = true;
		doku->UnDoValid = true;
		emit UndoAvail();
	}
}

FPoint Page::GetMinClipF(FPointArray Clip)
{
	FPoint np, rp;
	double mx = 99999;
	double my = 99999;
	for (uint c = 0; c < Clip.size(); ++c)
	{
		np = Clip.point(c);
		if (np.x() > 900000)
			continue;
		if (np.x() < mx)
			mx = np.x();
		if (np.y() < my)
			my = np.y();
	}
	rp = FPoint(mx, my);
	return rp;
}

QPoint Page::GetMinClip(QPointArray Clip)
{
	QPoint np, rp;
	int mx = 99999;
	int my = 99999;
	for (uint c = 0; c < Clip.size(); ++c)
	{
		np = Clip.point(c);
		if (np.x() < mx)
			mx = np.x();
		if (np.y() < my)
			my = np.y();
	}
	rp = QPoint(mx, my);
	return rp;
}

QPoint Page::GetMaxClip(QPointArray Clip)
{
	QPoint np, rp;
	int mx = 0;
	int my = 0;
	for (uint c = 0; c < Clip.size(); ++c)
	{
		np = Clip.point(c);
		if (np.x() > mx)
			mx = np.x();
		if (np.y() > my)
			my = np.y();
	}
	rp = QPoint(mx, my);
	return rp;
}

bool Page::SeleItemPos(QPoint m)
{
	uint a;
	double sc = doku->Scale;
	QPainter p;
	PageItem* b = Items.last();
	Mxp = static_cast<int>(m.x()/sc);
	Myp = static_cast<int>(m.y()/sc);
	ClRe = -1;
	if (doku->ActPage != this)
	{
		Deselect(true);
		doku->ActPage = this;
		emit PgCh(PageNr);
	}
	if (doku->DragP)
		return false;
	for (a = 0; a < Items.count(); ++a)
	{
		if (b->LayerNr == doku->ActiveLayer)
		{
			p.begin(this);
			Transform(b, &p);
			if (QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width),
			                                      static_cast<int>(b->Height))))).contains(m))
			{
				if ((b->PType == 4) || (b->PType == 2))
				{
					if (!b->Select)
					{
						b->Select = true;
						b->FrameOnly = true;
						b->paintObj();
						SelItem.append(b);
						emit HaveSel(b->PType);
						EmitValues(b);
					}
					p.end();
					if (SelItem.count() > 1)
					{
						setGroupRect();
						paintGroupRect();
					}
					return true;
				}
			}
			p.end();
		}
		b = Items.prev();
	}
	return false;
}

bool Page::SeleItem(QMouseEvent *m)
{
	uint a;
	double sc = doku->Scale;
	QPainter p;
	QRect tx, mpo;
	PageItem* b;
	Mpressed = true;
	Mxp = static_cast<int>(m->x()/sc);
	Myp = static_cast<int>(m->y()/sc);
	mpo = QRect(m->x()-doku->GrabRad, m->y()-doku->GrabRad, doku->GrabRad*2, doku->GrabRad*2);
	ClRe = -1;
	if ((SelItem.count() != 0) && (m->state() == ControlButton))
		b = SelItem.at(0);
	else
		b = Items.last();
	if (doku->ActPage != this)
	{
		Deselect(false);
		doku->ActPage = this;
		emit PgCh(PageNr);
	}
	if (m->state() == (ControlButton | AltButton))
		Deselect(false);
	if ((m->state() == (ControlButton | ShiftButton)) && (SelItem.count() != 0))
	{
		for (a = 0; a < Items.count(); ++a)
		{
			if (b->Select)
			{
				if (b->ItemNr == 0)
				{
					b = Items.last();
					break;
				}
				b = Items.prev();
				break;
			}
			b = Items.prev();
		}
	}
	for (a = 0; a < Items.count(); ++a)
	{
		if (b == NULL)
		{
			Deselect(true);
			SelItem.clear();
			return false;
		}
		if (b->LayerNr == doku->ActiveLayer)
		{
			p.begin(this);
			Transform(b, &p);
			QWMatrix ma;
			ma.scale(doku->Scale, doku->Scale);
			ma.translate(b->Xpos, b->Ypos);
			ma.rotate(b->Rot);
			ma.setTransformationMode ( QWMatrix::Areas );
			if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width), static_cast<int>(b->Height))))).contains(mpo)) ||
			        (QRegion(p.xForm(b->Clip)).contains(mpo)))
			{
				if (!b->Select)
				{
					if ((m->state() != ShiftButton) || (doku->AppMode == 10) || (doku->AppMode == 11))
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
							for (uint ga=0; ga<Items.count(); ++ga)
							{
								if (Items.at(ga)->Groups.count() != 0)
								{
									if (Items.at(ga)->Groups.top() == b->Groups.top())
									{
										if (Items.at(ga)->ItemNr != b->ItemNr)
										{
											if (SelItem.find(Items.at(ga)) == -1)
												SelItem.append(Items.at(ga));
										}
										Items.at(ga)->isSingleSel = false;
										Items.at(ga)->Select = true;
										Items.at(ga)->FrameOnly = true;
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
					emit HaveSel(b->PType);
				}
				else
				{
					EmitValues(b);
					emit HaveSel(b->PType);
					if (b->PType == 5)
						emit ItemGeom(b->Width, b->Height);
				}
				tx = p.xForm(QRect(static_cast<int>(b->Width-6), static_cast<int>(b->Height-6), 6, 6));
				if (tx.contains(mpo))
				{
					mCG = true;
					HowTo = 1;
					qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
				}
				else
				{
					tx = p.xForm(QRect(0, 0, 6, 6));
					if (tx.contains(mpo))
					{
						mCG = true;
						HowTo = 2;
						qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
					}
					else
					{
						mCG = false;
						qApp->setOverrideCursor(QCursor(SizeAllCursor), true);
					}
				}
				p.end();
				return true;
			}
			p.end();
		}
		b = Items.prev();
	}
	if ((ScApp->Prefs.GuidesShown) && (doku->AppMode == 1) && (!doku->GuideLock))
	{
		if (YGuides.count() != 0)
		{
			for (uint yg = 0; yg < YGuides.count(); ++yg)
			{
				if ((YGuides[yg] < (Myp+doku->GrabRad)) && (YGuides[yg] > (Myp-doku->GrabRad)))
				{
					GyM = yg;
					MoveGY = true;
					DrHY = static_cast<int>(YGuides[yg] * sc);
					break;
				}
			}
		}
		if (XGuides.count() != 0)
		{
			for (uint xg = 0; xg < XGuides.count(); ++xg)
			{
				if ((XGuides[xg] < (Mxp+doku->GrabRad)) && (XGuides[xg] > (Mxp-doku->GrabRad)))
				{
					GxM = xg;
					MoveGX = true;
					DrVX = static_cast<int>(XGuides[xg] * sc);
					break;
				}
			}
		}
	}
	Deselect(true);
	SelItem.clear();
	return false;
}

void Page::SelectItemNr(int nr)
{
	PageItem *b = doku->ActPage->Items.at(nr);
	if (!b->Select)
	{
		if (b->Groups.count() != 0)
		{
			if (doku->ActPage->SelItem.count() != 0)
			{
				if (doku->ActPage->SelItem.find(b) == -1)
					doku->ActPage->SelItem.append(b);
			}
			else
				doku->ActPage->SelItem.append(b);
			for (uint ga=0; ga<doku->ActPage->Items.count(); ++ga)
			{
				if (doku->ActPage->Items.at(ga)->Groups.count() != 0)
				{
					if (doku->ActPage->Items.at(ga)->Groups.top() == b->Groups.top())
					{
						if (doku->ActPage->Items.at(ga)->ItemNr != b->ItemNr)
						{
							if (doku->ActPage->SelItem.find(doku->ActPage->Items.at(ga)) == -1)
								doku->ActPage->SelItem.append(Items.at(ga));
						}
						doku->ActPage->Items.at(ga)->Select = true;
						doku->ActPage->Items.at(ga)->FrameOnly = true;
						doku->ActPage->Items.at(ga)->paintObj();
					}
				}
			}
		}
		else
		{
			doku->ActPage->SelItem.append(b);
			b->Select = true;
			b->FrameOnly = true;
			b->paintObj();
		}
	}
	else
	{
		if (doku->ActPage->SelItem.count() > 1)
		{
			PageItem *bb = doku->ActPage->SelItem.at(0);
			doku->ActPage->SelItem.remove(b);
			doku->ActPage->SelItem.prepend(b);
			b->FrameOnly = true;
			b->paintObj();
			bb->FrameOnly = true;
			bb->paintObj();
		}
	}
	if (doku->ActPage->SelItem.count() > 1)
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
	emit HaveSel(b->PType);
}

void Page::AdvanceSel(PageItem *b, int oldPos, int len, int dir, int expandSel, int state)
{
	int i;
	int isSpace;
	if ( dir > 0 )
	{
		isSpace = b->Ptext.at(oldPos)->ch.at(0).isSpace();
		b->CPos = oldPos +1;
		for (i=oldPos+1; i < len; i++)
		{
			if ( b->Ptext.at(i)->ch.at(0).isSpace() == isSpace )
			{
				if ( expandSel )
				b->Ptext.at(i)->cselect = state;
			}
			else
				break;
			b->CPos++;
		}
	}
	else if ( oldPos > 0 )
	{
		isSpace = b->Ptext.at(oldPos-1)->ch.at(0).isSpace();
		for (i=oldPos-2; i > 0; i--)
		{
			if (  b->Ptext.at(i)->ch.at(0).isSpace() == isSpace )
			{
				if ( expandSel )
				b->Ptext.at(i)->cselect = state;
			}
			else
				break;
			b->CPos--;
		}
	}
}

// jjsa added on 15-mar-2004 
// calculate the end position while ctrl + arrow pressed

void Page::setNewPos(PageItem *b, int oldPos, int len, int dir)
{
	int i;
	int isSpace;
	if ( dir > 0 && oldPos < len )
	{
		isSpace = b->Ptext.at(oldPos)->ch.at(0).isSpace();
		b->CPos = oldPos +1;
		for (i=oldPos+1; i < len; i++)
		{
			if ( b->Ptext.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			b->CPos++;
		}
	}
	else if ( dir < 0 && oldPos > 0 )
	{
		oldPos--;
		isSpace = b->Ptext.at(oldPos)->ch.at(0).isSpace();
		for (i=oldPos; i >= 0; i--)
		{
			if (  b->Ptext.at(i)->ch.at(0).isSpace() != isSpace )
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

void Page::ExpandSel(PageItem *b, int dir, int oldPos)
{
	int len = b->Ptext.count();
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
				leftSel = b->Ptext.at(oldPos-1)->cselect;
			actSel =   b->Ptext.at(oldPos)->cselect;
			rightSel = false; // not relevant
		}
		else if ( dir > 0 && oldPos == len) // -> key
		{
			return;
		}
		else if ( dir < 0 && oldPos > 0 ) // <- key
		{
			actSel  =   b->Ptext.at(oldPos-1)->cselect;
			leftSel = false; // not relevant
			if ( oldPos < len  )
				rightSel = b->Ptext.at(oldPos)->cselect;
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
				b->Ptext.at(i)->cselect = false;
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
		b->Ptext.at(i)->cselect = sel;
	if ( ! sel )
		emit  HasNoTextSel();
	RefreshItem(b);
}

void Page::deselectAll(PageItem *b)
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
			l = i->Ptext.count();
			for (n=0; n < l; n++ )
				i->Ptext.at(n)->cselect = false;
			i->OwnPage->RefreshItem(i);
			i->HasSel = false;
		}
		i = i->NextBox;
	}
	emit  HasNoTextSel();
}

bool Page::slotSetCurs(int x, int y)
{
	if (doku->ActPage != this) {return false;}
	PageItem *b;
	if (GetItem(&b))
	{
		QRect mpo;
		if (!((b->PType == 4) || (b->PType == 2)))
			return false;
		QPainter p;
		QString chx;
		p.begin(this);
		Transform(b, &p);
		mpo = QRect(x-doku->GrabRad, y-doku->GrabRad, doku->GrabRad*2, doku->GrabRad*2);
		if ((QRegion(p.xForm(QPointArray(QRect(0, 0, static_cast<int>(b->Width),
		                                       static_cast<int>(b->Height))))).contains(mpo)) ||
		        (QRegion(p.xForm(b->Clip)).contains(mpo)))
		{
			if (b->PType == 2)
				return true;
			TransformM(b, &p);
			uint a, i;
			int xp, yp, w, h, chs;
			for (a=0; a<b->Ptext.count(); ++a)
			{
				xp = static_cast<int>(b->Ptext.at(a)->xp);
				yp = static_cast<int>(b->Ptext.at(a)->yp);
				chx = b->Ptext.at(a)->ch;
				if (chx == QChar(30))
					chx = b->ExpandToken(a);
				if (chx == QChar(29))
					chx = " ";
				chs = b->Ptext.at(a)->csize;
				b->SetZeichAttr(b->Ptext.at(a), &chs, &chx);
				if ((chx == QChar(13)) || (chx == QChar(9)))
					w = 1;
				else
					w = qRound(Cwidth(doku, b->Ptext.at(a)->cfont, chx, chs)*(b->Ptext.at(a)->cscale / 100.0));
				h = static_cast<int>(doku->Vorlagen[b->Ptext.at(a)->cab].LineSpa);
				if (QRegion(p.xForm(QRect(xp-1, yp-h, w+1, h))).contains(QPoint(x, y)))
				{
					b->CPos = a;
					p.end();
					doku->CurrFont = b->Ptext.at(a)->cfont;
					doku->CurrFontSize = b->Ptext.at(a)->csize;
					doku->CurrTextFill = b->Ptext.at(a)->ccolor;
					doku->CurrTextFillSh = b->Ptext.at(a)->cshade;
					doku->CurrTextStroke = b->Ptext.at(a)->cstroke;
					doku->CurrTextStrokeSh = b->Ptext.at(a)->cshade2;
					doku->CurrTextScale = b->Ptext.at(a)->cscale;
					emit ItemTextSca(b->Ptext.at(a)->cscale);
					emit ItemTextFont(b->Ptext.at(a)->cfont);
					emit ItemTextSize(b->Ptext.at(a)->csize);
					emit ItemTextUSval(b->Ptext.at(a)->cextra);
					emit ItemTextStil(b->Ptext.at(a)->cstyle);
					emit ItemTextAbs(b->Ptext.at(a)->cab);
					emit ItemTextFarben(b->Ptext.at(a)->cstroke, b->Ptext.at(a)->ccolor, b->Ptext.at(a)->cshade2, b->Ptext.at(a)->cshade);
					return true;
				}
			}
			QPoint np;
			for (a=0; a<b->Ptext.count(); ++a)
			{
				xp = static_cast<int>(b->Ptext.at(a)->xp);
				yp = static_cast<int>(b->Ptext.at(a)->yp);
				np = p.xForm(QPoint(xp, yp - static_cast<int>(doku->Vorlagen[b->Ptext.at(a)->cab].LineSpa)));
				if (y < np.y())
				{
					b->CPos = a;
					p.end();
					if (a > 0)
						i = a - 1;
					else
						i = a;
					doku->CurrFont = b->Ptext.at(i)->cfont;
					doku->CurrFontSize = b->Ptext.at(i)->csize;
					doku->CurrTextFill = b->Ptext.at(i)->ccolor;
					doku->CurrTextFillSh = b->Ptext.at(i)->cshade;
					doku->CurrTextStroke = b->Ptext.at(i)->cstroke;
					doku->CurrTextStrokeSh = b->Ptext.at(i)->cshade2;
					doku->CurrTextScale = b->Ptext.at(i)->cscale;
					emit ItemTextSca(b->Ptext.at(i)->cscale);
					emit ItemTextFarben(b->Ptext.at(i)->cstroke, b->Ptext.at(i)->ccolor, b->Ptext.at(i)->cshade2, b->Ptext.at(i)->cshade);
					emit ItemTextFont(b->Ptext.at(i)->cfont);
					emit ItemTextSize(b->Ptext.at(i)->csize);
					emit ItemTextUSval(b->Ptext.at(i)->cextra);
					emit ItemTextStil(b->Ptext.at(i)->cstyle);
					emit ItemTextAbs(b->Ptext.at(i)->cab);
					return true;
				}
			}
			b->CPos = b->Ptext.count();
			if (b->Ptext.count() != 0)
			{
				doku->CurrFont = b->Ptext.at(b->CPos-1)->cfont;
				doku->CurrFontSize = b->Ptext.at(b->CPos-1)->csize;
				doku->CurrTextFill = b->Ptext.at(b->CPos-1)->ccolor;
				doku->CurrTextFillSh = b->Ptext.at(b->CPos-1)->cshade;
				doku->CurrTextStroke = b->Ptext.at(b->CPos-1)->cstroke;
				doku->CurrTextStrokeSh = b->Ptext.at(b->CPos-1)->cshade2;
				doku->CurrTextScale = b->Ptext.at(b->CPos-1)->cscale;
				emit ItemTextSca(b->Ptext.at(b->CPos-1)->cscale);
				emit ItemTextFarben(b->Ptext.at(b->CPos-1)->cstroke, b->Ptext.at(b->CPos-1)->ccolor, b->Ptext.at(b->CPos-1)->cshade2, b->Ptext.at(b->CPos-1)->cshade);
				emit ItemTextFont(b->Ptext.at(b->CPos-1)->cfont);
				emit ItemTextSize(b->Ptext.at(b->CPos-1)->csize);
				emit ItemTextUSval(b->Ptext.at(b->CPos-1)->cextra);
				emit ItemTextStil(b->Ptext.at(b->CPos-1)->cstyle);
				emit ItemTextAbs(b->Ptext.at(b->CPos-1)->cab);
				p.end();
				return true;
			}
			p.end();
		}
	}
	return false;
}

void Page::slotDoCurs(bool draw)
{
	QColor tmp;
	int chs, offs;
	PageItem *b;
	if (GetItem(&b))
	{
		if ((b->PType != 4) || (b->Ptext.count() == 0)) { return; }
		QPainter p;
		QString chx;
		int xp, yp, yp1, desc, asce;
		p.begin(this);
		p.setClipRegion(ViewReg());
		Transform(b, &p);
		TransformM(b, &p);
		if (b->CPos > 0)
		{
			offs = QMIN(b->CPos-1, static_cast<int>(b->Ptext.count()-1));
			if (b->Ptext.at(offs)->yp == 0)
				return;
			chx = b->Ptext.at(offs)->ch;
			if (chx == QChar(30))
				chx = b->ExpandToken(offs);
			if (chx == QChar(29))
				chx = " ";
			chs = b->Ptext.at(offs)->csize;
			b->SetZeichAttr(b->Ptext.at(offs), &chs, &chx);
			xp = static_cast<int>(b->Ptext.at(offs)->xp);
			if ((b->Ptext.at(offs)->ch == QChar(13)) && (b->CPos != static_cast<int>(b->Ptext.count())))
				xp = static_cast<int>(b->Ptext.at(offs+1)->xp);
			if ((b->Ptext.at(offs)->ch != QChar(13)) && (b->Ptext.at(offs)->ch != QChar(9)))
				xp += qRound(Cwidth(doku, b->Ptext.at(offs)->cfont, chx, chs)*(b->Ptext.at(offs)->cscale / 100.0));
			if (b->CPos != static_cast<int>(b->Ptext.count()))
			{
				if (b->Ptext.at(offs)->yp != b->Ptext.at(offs+1)->yp)
				{
					offs++;
					if (b->Ptext.at(offs)->ch == QChar(13))
					{
						offs--;
						xp = static_cast<int>(b->Ptext.at(offs)->xp);
						chs = b->Ptext.at(offs)->csize;
						chx = b->Ptext.at(offs)->ch;
						xp += qRound(Cwidth(doku, b->Ptext.at(offs)->cfont, chx, chs)*(b->Ptext.at(offs)->cscale / 100.0));
					}
					else
						xp = static_cast<int>(b->Ptext.at(offs)->xp);
				}
			}
			yp = static_cast<int>(b->Ptext.at(offs)->yp);
			desc = static_cast<int>((*doku->AllFonts)[b->Ptext.at(offs)->cfont]->numDescender * (-b->Ptext.at(offs)->csize / 10.0));
			asce = static_cast<int>((*doku->AllFonts)[b->Ptext.at(offs)->cfont]->numAscent * (b->Ptext.at(offs)->csize / 10.0));
		}
		else
		{
			xp = static_cast<int>(b->Ptext.at(b->CPos)->xp);
			yp = static_cast<int>(b->Ptext.at(b->CPos)->yp);
			desc = static_cast<int>((*doku->AllFonts)[b->Ptext.at(b->CPos)->cfont]->numDescender * (-b->Ptext.at(b->CPos)->csize / 10.0));
			asce = static_cast<int>((*doku->AllFonts)[b->Ptext.at(b->CPos)->cfont]->numAscent * (b->Ptext.at(b->CPos)->csize / 10.0));
		}
		yp1 = yp - asce;
		yp += desc;
		if (draw)
		{
			p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
			CursVis = false;
		}
		else
		{
			if (b->Pcolor != "None")
			{
				b->SetFarbe(&tmp, b->Pcolor, b->Shade);
				p.setPen(tmp);
			}
			else
				p.setPen(QPen(white, 1, SolidLine, FlatCap, MiterJoin));
			CursVis = true;
		}
		p.drawLine(xp, QMIN(QMAX(yp1,0),static_cast<int>(b->Height)), xp, QMIN(QMAX(yp,0),static_cast<int>(b->Height)));
		p.end();
	}
}

void Page::BlinkCurs()
{
	slotDoCurs(CursVis);
}

void Page::MarkClip(PageItem *b)
{
	double x, y;
	QPainter p;
	FPointArray cli;
	QPointArray Bez(4);
	p.begin(this);
	p.setClipRegion(ViewReg());
	p.setClipping(true);
	Transform(b, &p);
	p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
	p.setBrush(NoBrush);
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

void Page::ToTextFrame()
{
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->PType = 4;
	b->Frame = true;
	RefreshItem(b);
	emit HaveSel(b->PType);
	if (!doku->loading)
		emit UpdtObj(PageNr, b->ItemNr);
	EmitValues(b);
}

void Page::ToPicFrame()
{
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->PType = 2;
	b->Frame = true;
	RefreshItem(b);
	emit HaveSel(b->PType);
	if (!doku->loading)
		emit UpdtObj(PageNr, b->ItemNr);
	EmitValues(b);
}

void Page::ToPolyFrame()
{
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->PType = 6;
	b->Frame = false;
	b->ClipEdited = true;
	b->FrameType = 3;
	b->Clip = FlattenPath(b->PoLine, b->Segments);
	RefreshItem(b);
	emit HaveSel(b->PType);
	if (!doku->loading)
		emit UpdtObj(PageNr, b->ItemNr);
	EmitValues(b);
}

void Page::ToBezierFrame()
{
	emit Amode(1);
	PageItem *b = SelItem.at(0);
	b->PType = 7;
	b->ClipEdited = true;
	SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
	AdjustItemSize(b);
	RefreshItem(b);
	emit HaveSel(b->PType);
	if (!doku->loading)
		emit UpdtObj(PageNr, b->ItemNr);
	EmitValues(b);
}

void Page::Bezier2Poly()
{
	PageItem *b = SelItem.at(0);
	b->PType = 6;
	b->Frame = false;
	b->ClipEdited = true;
	b->FrameType = 3;
	b->PoLine.addPoint(b->PoLine.point(b->PoLine.size()-2));
	b->PoLine.addPoint(b->PoLine.point(b->PoLine.size()-3));
	b->PoLine.addPoint(b->PoLine.point(0));
	b->PoLine.addPoint(b->PoLine.point(0));
	b->Clip = FlattenPath(b->PoLine, b->Segments);
	RefreshItem(b);
	emit HaveSel(b->PType);
	if (!doku->loading)
		emit UpdtObj(PageNr, b->ItemNr);
	EmitValues(b);
}

void Page::SetPolyClip(PageItem *b, int up, int down)
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

void Page::UpdatePolyClip(PageItem *b)
{
	struct Pti *hl;
	int asce = 1;
	int desc = 1;
	for (uint a = 0; a < b->Ptext.count(); ++a)
	{
		hl = b->Ptext.at(a);
		int des = static_cast<int>((*doku->AllFonts)[hl->cfont]->numDescender * (-hl->csize / 10.0));
		int asc = static_cast<int>((*doku->AllFonts)[hl->cfont]->numAscent * (hl->csize / 10.0));
		if (asc > asce)
			asce = asc;
		if (des > desc)
			desc = des;
	}
	SetPolyClip(b, static_cast<int>(asce+b->BaseOffs), static_cast<int>(desc+b->BaseOffs));
}

void Page::SplitObj()
{
	PageItem *b;
	PageItem *bb;
	uint StartInd, z;
	b = SelItem.at(0);
	uint EndInd = b->PoLine.size();
	for (uint a = b->PoLine.size()-1; a > 0; a--)
	{
		if (b->PoLine.point(a).x() > 900000)
		{
			StartInd = a + 1;
			z = PaintPoly(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, b->Pcolor, b->Pcolor2);
			bb = Items.at(z);
			bb->PoLine.resize(0);
			bb->PoLine.putPoints(0, EndInd - StartInd, b->PoLine, StartInd);
			bb->Rot = b->Rot;
			AdjustItemSize(bb);
			bb->ClipEdited = true;
			a -= 3;
			EndInd = StartInd - 4;
		}
	}
	b->PoLine.resize(StartInd-4);
	AdjustItemSize(b);
	b->ClipEdited = true;
	Deselect(true);
	update();
}

void Page::UniteObj()
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
		Deselect(true);
		for (uint c = 0; c < toDel.count(); ++c)
			SelectItemNr(*toDel.at(c));
		DeleteItem();
		update();
	}
}

void Page::TextToPath()
{
	PageItem *b;
	PageItem *bb;
	FPointArray pts;
	double x, y;
	QString chx,ccounter;
	if (SelItem.count() > 0)
	{
		b = SelItem.at(0);
		Deselect();
		if (b->Ptext.count() == 0)
			return;
		for (uint a = 0; a < b->MaxChars; ++a)
		{
			pts.resize(0);
			x = 0.0;
			y = 0.0;
			chx = b->Ptext.at(a)->ch;
			if ((chx == QChar(13)) || (chx == QChar(32)) || (chx == QChar(29)))
				continue;
			if (chx == QChar(30))
			{
				chx = b->ExpandToken(a);
				if (chx == QChar(32))
					continue;
			}
			int chs = b->Ptext.at(a)->csize;
			if (b->Ptext.at(a)->cstyle & 64)
			{
				if (chx.upper() != chx)
				{
					chs = QMAX(static_cast<int>(b->Ptext.at(a)->csize * b->OwnPage->doku->VKapit / 100), 1);
					chx = chx.upper();
				}
			}
			double csi = static_cast<double>(chs) / 100.0;
			uint chr = chx[0].unicode();
			QWMatrix chma;
			chma.scale(csi, csi);
			pts = (*doku->AllFonts)[b->Ptext.at(a)->cfont]->GlyphArray[chr].Outlines.copy();
			if (pts.size() < 4)
				continue;
			x = (*doku->AllFonts)[b->Ptext.at(a)->cfont]->GlyphArray[chr].x * csi;
			y = (*doku->AllFonts)[b->Ptext.at(a)->cfont]->GlyphArray[chr].y * csi;
			pts.map(chma);
			chma = QWMatrix();
			chma.scale(b->Ptext.at(a)->cscale / 100.0, 1);
			pts.map(chma);
			uint z = PaintPoly(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, b->Pcolor2, b->Pcolor);
			bb = Items.at(z);
			emit AddObj(PageNr, z);
			bb->Textflow = b->Textflow;
			bb->Textflow2 = b->Textflow2;
			bb->LockRes = b->LockRes;
			bb->Locked = b->Locked;
			bb->NamedLStyle = b->NamedLStyle;
			bb->AnName = b->AnName+"+"+ccounter.setNum(a);
			bb->AutoName = false;
			bb->PoLine = pts.copy();
			bb->Rot = b->Rot;
			bb->Pcolor = b->Ptext.at(a)->ccolor;
			bb->Shade = b->Ptext.at(a)->cshade;
			if (b->Ptext.at(a)->cstyle & 4)
			{
				bb->Pcolor2 = b->Ptext.at(a)->cstroke;
				bb->Shade2 = b->Ptext.at(a)->cshade2;
			}
			else
			{
				bb->Pcolor2 = "None";
				bb->Shade2 = 100;
			}
			bb->Pwidth = (*doku->AllFonts)[b->Ptext.at(a)->cfont]->strokeWidth * chs / 2.0;
			FPoint tp2 = GetMinClipF(bb->PoLine);
			bb->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp = GetMaxClipF(bb->PoLine);
			bb->Width = tp.x();
			bb->Height = tp.y();
			bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
			FPoint npo;
			if (b->Reverse)
			{
				double wide;
				if (a < b->Ptext.count()-1)
					wide = Cwidth(doku, b->Ptext.at(a)->cfont, chx, b->Ptext.at(a)->csize, b->Ptext.at(a+1)->ch);
				else
					wide = Cwidth(doku, b->Ptext.at(a)->cfont, chx, b->Ptext.at(a)->csize);
				npo = transformPoint(FPoint(b->Width-b->Ptext.at(a)->xp-wide+x,b->Ptext.at(a)->yp-y), b->Xpos, b->Ypos, b->Rot, 1.0, 1.0);
			}
			else
				npo = transformPoint(FPoint(b->Ptext.at(a)->xp+x,b->Ptext.at(a)->yp-y), 0.0, 0.0, b->Rot, 1.0, 1.0);
			bb->Xpos = b->Xpos+npo.x();
			bb->Ypos = b->Ypos+npo.y();
			bb->ClipEdited = true;
			SelItem.append(bb);
		}
		if (SelItem.count() > 1)
		{
			for (uint ag = 0; ag < SelItem.count(); ++ag)
			{
				bb = SelItem.at(ag);
				bb->Groups.push(doku->GroupCounter);
			}
		}
		doku->GroupCounter++;
		SelItem.clear();
		SelItem.append(b);
		DeleteItem();
	}
}

void Page::FromPathText()
{
	PageItem *b;
	PageItem *bb;
	uint z;
	if (GetItem(&b))
	{
		z = PaintPolyLine(b->Xpos, b->Ypos, b->Width, b->Height, b->Pwidth, "None", b->Pcolor2);
		bb = Items.at(z);
		bb->PoLine = b->PoLine.copy();
		bb->ClipEdited = true;
		SetPolyClip(bb, qRound(QMAX(bb->Pwidth / 2, 1)), qRound(QMAX(bb->Pwidth / 2, 1)));
		AdjustItemSize(bb);
		b->PType = 4;
		b->Pcolor2 = "None";
		b->Frame = true;
		SetRectFrame(b);
		SelectItemNr(b->ItemNr);
		ToFront();
		Deselect(true);
		update();
	}
}

void Page::ToPathText()
{
	PageItem *b;
	PageItem *bb;
	if (SelItem.count() > 1)
	{
		b = SelItem.at(0);
		if (b->PType == 4)
			bb = SelItem.at(1);
		else
		{
			bb = SelItem.at(0);
			b = SelItem.at(1);
		}
		if (bb->PType != 7)
			return;
		b->Frame = false;
		b->ClipEdited = true;
		b->PType = 8;
		b->PoLine = bb->PoLine.copy();
		b->Pwidth = bb->Pwidth;
		b->Pcolor2 = bb->Pcolor2;
		b->PLineArt = bb->PLineArt;
		b->PLineEnd = bb->PLineEnd;
		b->PLineJoin = bb->PLineJoin;
		if (!doku->loading)
			emit UpdtObj(PageNr, b->ItemNr);
		UpdatePolyClip(b);
		AdjustItemSize(b);
		double dx = bb->Xpos - b->Xpos;
		double dy = bb->Ypos - b->Ypos;
		MoveItem(dx, dy, b);
		Deselect(true);
		SelectItemNr(bb->ItemNr);
		DeleteItem();
		update();
	}
}

void Page::SetFrameShape(PageItem *b, int count, double *vals)
{
	ClRe = -1;
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
}

void Page::SetFrameRect()
{
	PageItem *b;
	if (GetItem(&b))
	{
		SetRectFrame(b);
		RefreshItem(b);
	}
}

void Page::SetRectFrame(PageItem *b)
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

void Page::SetFrameRounded()
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
		update();
	}
}

void Page::SetFrameRound(PageItem* b)
{
	b->RadRect = QMIN(b->RadRect, QMIN(b->Width,b->Height)/2);
	b->PoLine.resize(0);
	double rr = fabs(b->RadRect);
	if (b->RadRect > 0)
	{
		b->PoLine.addPoint(rr, 0);
		b->PoLine.addPoint(rr, 0);
		b->PoLine.addPoint(b->Width-rr, 0);
		b->PoLine.addPoint(b->Width-rr, 0);
		b->PoLine.addPoint(b->Width-rr, 0);
		b->PoLine.addPoint(b->Width-rr+rr*0.552284749, 0);
		b->PoLine.addPoint(b->Width, rr);
		b->PoLine.addPoint(b->Width, rr*0.552284749);
		b->PoLine.addPoint(b->Width, rr);
		b->PoLine.addPoint(b->Width, rr);
		b->PoLine.addPoint(b->Width, b->Height-rr);
		b->PoLine.addPoint(b->Width, b->Height-rr);
		b->PoLine.addPoint(b->Width, b->Height-rr);
		b->PoLine.addPoint(b->Width, b->Height-rr+rr*0.552284749);
		b->PoLine.addPoint(b->Width-rr, b->Height);
		b->PoLine.addPoint(b->Width-rr+rr*0.552284749, b->Height);
		b->PoLine.addPoint(b->Width-rr, b->Height);
		b->PoLine.addPoint(b->Width-rr, b->Height);
		b->PoLine.addPoint(rr, b->Height);
		b->PoLine.addPoint(rr, b->Height);
		b->PoLine.addPoint(rr, b->Height);
		b->PoLine.addPoint(rr*0.552284749, b->Height);
		b->PoLine.addPoint(0, b->Height-rr);
		b->PoLine.addPoint(0, b->Height-rr+rr*0.552284749);
		b->PoLine.addPoint(0, b->Height-rr);
		b->PoLine.addPoint(0, b->Height-rr);
		b->PoLine.addPoint(0, rr);
		b->PoLine.addPoint(0, rr);
		b->PoLine.addPoint(0, rr);
		b->PoLine.addPoint(0, rr*0.552284749);
		b->PoLine.addPoint(rr, 0);
		b->PoLine.addPoint(rr*0.552284749, 0);
	}
	else
	{
		b->PoLine.addPoint(rr, 0);
		b->PoLine.addPoint(rr, 0);
		b->PoLine.addPoint(b->Width-rr, 0);
		b->PoLine.addPoint(b->Width-rr, 0);
		b->PoLine.addPoint(b->Width-rr, 0);
		b->PoLine.addPoint(b->Width-rr, rr*0.552284749);
		b->PoLine.addPoint(b->Width, rr);
		b->PoLine.addPoint(b->Width-rr*0.552284749, rr);
		b->PoLine.addPoint(b->Width, rr);
		b->PoLine.addPoint(b->Width, rr);
		b->PoLine.addPoint(b->Width, b->Height-rr);
		b->PoLine.addPoint(b->Width, b->Height-rr);
		b->PoLine.addPoint(b->Width, b->Height-rr);
		b->PoLine.addPoint(b->Width-rr*0.552284749, b->Height-rr);
		b->PoLine.addPoint(b->Width-rr, b->Height);
		b->PoLine.addPoint(b->Width-rr, b->Height-rr*0.552284749);
		b->PoLine.addPoint(b->Width-rr, b->Height);
		b->PoLine.addPoint(b->Width-rr, b->Height);
		b->PoLine.addPoint(rr, b->Height);
		b->PoLine.addPoint(rr, b->Height);
		b->PoLine.addPoint(rr, b->Height);
		b->PoLine.addPoint(rr, b->Height-rr*0.552284749);
		b->PoLine.addPoint(0, b->Height-rr);
		b->PoLine.addPoint(rr*0.552284749, b->Height-rr);
		b->PoLine.addPoint(0, b->Height-rr);
		b->PoLine.addPoint(0, b->Height-rr);
		b->PoLine.addPoint(0, rr);
		b->PoLine.addPoint(0, rr);
		b->PoLine.addPoint(0, rr);
		b->PoLine.addPoint(rr*0.552284749, rr);
		b->PoLine.addPoint(rr, 0);
		b->PoLine.addPoint(rr, rr*0.552284749);
	}
	b->Clip = FlattenPath(b->PoLine, b->Segments);
	b->ClipEdited = false;
	b->FrameType = 2;
	emit ItemRadius(b->RadRect);
}

void Page::SetFrameOval()
{
	PageItem *b;
	if (GetItem(&b))
	{
		SetOvalFrame(b);
		RefreshItem(b);
	}
}

void Page::SetOvalFrame(PageItem *b)
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

void Page::ChLineWidth(double w)
{
	uint a;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			PageItem *b = SelItem.at(a);
			b->Pwidth = w;
			if (b->PType == 7)
				SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)), qRound(QMAX(b->Pwidth / 2, 1)));
			if (b->PType == 5)
			{
				int ph = static_cast<int>(QMAX(1.0, w / 2.0));
				b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
				                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
				                  -ph,static_cast<int>(b->Height+ph));
			}
			RefreshItem(b);
		}
	}
}

void Page::ChLineArt(PenStyle w)
{
	uint a;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->PLineArt = w;
			RefreshItem(SelItem.at(a));
		}
	}
}

void Page::ChLineJoin(PenJoinStyle w)
{
	uint a;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->PLineJoin = w;
			RefreshItem(SelItem.at(a));
		}
	}
}

void Page::ChLineEnd(PenCapStyle w)
{
	uint a;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->PLineEnd = w;
			RefreshItem(SelItem.at(a));
		}
	}
}

void Page::ChLineSpa(double w)
{
	uint a;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->LineSp = w;
			RefreshItem(SelItem.at(a));
		}
		doku->Vorlagen[0].LineSpa = w;
	}
}

void Page::ChLocalXY(double x, double y)
{
	uint a;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			b->LocalX = x;
			b->LocalY = y;
			RefreshItem(b);
		}
	}
}

void Page::ChLocalSc(double x, double y)
{
	uint a;
	double oldx, oldy;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
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

void Page::ItemFont(QString fon)
{
	uint a;
	uint aa;
	if (SelItem.count() != 0)
	{
		for (aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *b = SelItem.at(aa);
			if (doku->AppMode == 1)
			{
				b->IFont = fon;
				if (b->Ptext.count() != 0)
				{
					for (a = 0; a < b->Ptext.count(); ++a)
						b->Ptext.at(a)->cfont = fon;
					if (b->PType == 8)
					{
						UpdatePolyClip(b);
						AdjustItemSize(b);
					}
					b->Dirty = true;
					if (!doku->loading)
						emit UpdtObj(PageNr, b->ItemNr);
					RefreshItem(b);
				}
			}
			if ((b->HasSel) && (doku->AppMode == 7))
			{
				if (b->Ptext.count() != 0)
				{
					for (a = 0; a < b->Ptext.count(); ++a)
					{
						if (b->Ptext.at(a)->cselect)
							b->Ptext.at(a)->cfont = fon;
					}
					b->Dirty = true;
					RefreshItem(b);
				}
			}
		}
	}
}

void Page::ItemPen(QString farbe)
{
	uint a;
	PageItem *i;
	if (farbe == tr("None"))
		farbe = "None";
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			i = SelItem.at(a);
			if ((i->PType == 5) && (farbe == "None"))
				continue;
			i->Pcolor2 = farbe;
			RefreshItem(i);
			emit ItemFarben(i->Pcolor2, i->Pcolor, i->Shade2, i->Shade);
		}
	}
}

void Page::ItemTextBrush(QString farbe)
{
	uint a, i;
	PageItem *b;
	if (farbe == tr("None"))
		farbe = "None";
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if ((b->PType == 4) || (b->PType == 8))
			{
				for (i=0; i<b->Ptext.count(); ++i)
				{
					if (doku->AppMode == 7)
					{
						if (b->Ptext.at(i)->cselect)
							b->Ptext.at(i)->ccolor = farbe;
					}
					else
					{
						b->Ptext.at(i)->ccolor = farbe;
						b->TxtFill = farbe;
					}
				}
			}
			RefreshItem(b);
			emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
		}
	}
}

void Page::ItemTextBrushS(int sha)
{
	uint a, i;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if (b->PType == 4)
			{
				for (i=0; i<b->Ptext.count(); ++i)
				{
					if (doku->AppMode == 7)
					{
						if (b->Ptext.at(i)->cselect)
							b->Ptext.at(i)->cshade = sha;
					}
					else
					{
						b->Ptext.at(i)->cshade = sha;
						b->ShTxtFill = sha;
					}
				}
			}
			RefreshItem(b);
			emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
		}
	}
}

void Page::ItemTextPen(QString farbe)
{
	uint a, i;
	PageItem *b;
	if (farbe == tr("None"))
		farbe = "None";
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if ((b->PType == 4) || (b->PType == 8))
			{
				for (i=0; i<b->Ptext.count(); ++i)
				{
					if (doku->AppMode == 7)
					{
						if (b->Ptext.at(i)->cselect)
							b->Ptext.at(i)->cstroke = farbe;
					}
					else
					{
						b->Ptext.at(i)->cstroke = farbe;
						b->TxtStroke = farbe;
					}
				}
			}
			RefreshItem(b);
		}
	}
}

void Page::ItemTextPenS(int sha)
{
	uint a, i;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if (b->PType == 4)
			{
				for (i=0; i<b->Ptext.count(); ++i)
				{
					if (doku->AppMode == 7)
					{
						if (b->Ptext.at(i)->cselect)
							b->Ptext.at(i)->cshade2 = sha;
					}
					else
					{
						b->Ptext.at(i)->cshade2 = sha;
						b->ShTxtStroke = sha;
					}
				}
			}
			RefreshItem(b);
		}
	}
}

void Page::ItemTextScale(int sha)
{
	uint a, i;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			if (b->PType == 4)
			{
				for (i=0; i<b->Ptext.count(); ++i)
				{
					if (doku->AppMode == 7)
					{
						if (b->Ptext.at(i)->cselect)
							b->Ptext.at(i)->cscale = sha;
					}
					else
					{
						b->Ptext.at(i)->cscale = sha;
						b->TxtScale = sha;
					}
				}
			}
			RefreshItem(b);
		}
	}
}

void Page::ItemBrush(QString farbe)
{
	uint a;
	PageItem *b;
	if (farbe == tr("None"))
		farbe = "None";
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			b->Pcolor = farbe;
			RefreshItem(b);
			emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
		}
	}
}

void Page::ItemBrushShade(int sha)
{
	uint a;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			b = SelItem.at(a);
			b->Shade = sha;
			emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
			RefreshItem(b);
		}
	}
}

void Page::ItemPenShade(int sha)
{
	PageItem *i;
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			i = SelItem.at(a);
			i->Shade2 = sha;
			RefreshItem(i);
		}
	}
}

void Page::AdjItemGradient(PageItem *b, int typ, QString col1, int sh1, QString col2, int sh2)
{
	QColor tmp;
	b->fill_gradient.clearStops();
	QString c1 = typ == 5 ? col2 : col1, c2 = typ == 5 ? col1 : col2;
	int s1 = typ == 5 ? sh2 : sh1, s2 = typ == 5 ? sh1 : sh2;
	b->SetFarbe(&tmp, c1, s1);
	b->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0);
	b->SetFarbe(&tmp, c2, s2);
	b->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0);
	b->GrColor2 = col1;
	b->GrShade2 = sh1;
	b->GrColor = col2;
	b->GrShade = sh2;
	b->GrType = typ;
}

void Page::ItemGradFill(int typ, QString col1, int sh1, QString col2, int sh2)
{
	uint a;
	PageItem *i;
	QString col1c, col2c;
	if ((col1 == "None") || (col1 == ""))
		col1c = doku->PageColors.begin().key();
	else
		col1c = col1;
	if ((col2 == "None") || (col2 == ""))
		col2c = doku->PageColors.begin().key();
	else
		col2c = col2;
	if (SelItem.count() != 0)
	{
		for (a = 0; a < SelItem.count(); ++a)
		{
			i = SelItem.at(a);
			AdjItemGradient(i, typ, col1c, sh1, col2c, sh2);
			RefreshItem(i);
		}
	}
}

void Page::chTyStyle(int s)
{
	if (SelItem.count() != 0)
	{
		for (uint aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *b = SelItem.at(aa);
			if (b->Ptext.count() != 0)
			{
				if (doku->AppMode == 7)
				{
					for (uint a = 0; a < b->Ptext.count(); ++a)
					{
						if (b->Ptext.at(a)->cselect)
						{
							b->Ptext.at(a)->cstyle &= ~127;
							b->Ptext.at(a)->cstyle |= s;
						}
					}
				}
				else
				{
					for (uint a = 0; a < b->Ptext.count(); ++a)
					{
						b->Ptext.at(a)->cstyle &= ~127;
						b->Ptext.at(a)->cstyle |= s;
					}
					b->TxTStyle &= ~127;
					b->TxTStyle |= s;
				}
				b->Dirty = true;
				RefreshItem(b);
			}
		}
	}
}

void Page::SetAbStyle(int s)
{
	PageItem *b;
	if (GetItem(&b))
		chAbStyle(b, s);
}

void Page::chAbStyle(PageItem *b, int s)
{
	int a, ax;
	PageItem *nb;
	bool cr = true;
	if (doku->AppMode == 7)
	{
		nb = b;
		a = b->CPos;
		if (a == static_cast<int>(nb->Ptext.count()))
			a -= 1;
		while ((cr) && (nb != 0))
		{
			if (nb->Ptext.count() == 0)
			{
				nb = nb->BackBox;
				if (nb == 0)
				{
					cr = false;
					break;
				}
				a = static_cast<int>(nb->Ptext.count()-1);
			}
			ax = a;
			for (int xx=0; xx<ax+1; ++xx)
			{
				if (nb->Ptext.at(a)->ch == QChar(13))
				{
					cr = false;
					break;
				}
				nb->Ptext.at(a)->cab = s;
				if (s > 4)
				{
					if (doku->Vorlagen[s].Font != "")
					{
						nb->Ptext.at(a)->cfont = doku->Vorlagen[s].Font;
						nb->Ptext.at(a)->csize = doku->Vorlagen[s].FontSize;
						nb->Ptext.at(a)->cstyle &= ~127;
						nb->Ptext.at(a)->cstyle |= doku->Vorlagen[s].FontEffect;
						nb->Ptext.at(a)->ccolor = doku->Vorlagen[s].FColor;
						nb->Ptext.at(a)->cshade = doku->Vorlagen[s].FShade;
						nb->Ptext.at(a)->cstroke = doku->Vorlagen[s].SColor;
						nb->Ptext.at(a)->cshade2 = doku->Vorlagen[s].SShade;
					}
				}
				if (s == 0)
				{
					nb->Ptext.at(a)->ccolor = nb->TxtFill;
					nb->Ptext.at(a)->cshade = nb->ShTxtFill;
					nb->Ptext.at(a)->cstroke = nb->TxtStroke;
					nb->Ptext.at(a)->cshade2 = nb->ShTxtStroke;
					nb->Ptext.at(a)->csize = nb->ISize;
					nb->Ptext.at(a)->cfont = nb->IFont;
					nb->Ptext.at(a)->cstyle &= ~127;
					nb->Ptext.at(a)->cstyle |= nb->TxTStyle;
				}
				a--;
			}
			if (cr)
			{
				nb = nb->BackBox;
				if (nb != 0)
					a = static_cast<int>(nb->Ptext.count()-1);
			}
		}
		a = b->CPos;
		cr = true;
		nb = b;
		if (a == static_cast<int>(nb->Ptext.count()))
			cr = false;
		while ((cr) && (nb != 0))
		{
			while (a < static_cast<int>(nb->Ptext.count()))
			{
				nb->Ptext.at(a)->cab = s;
				if (s > 4)
				{
					if (doku->Vorlagen[s].Font != "")
					{
						nb->Ptext.at(a)->cfont = doku->Vorlagen[s].Font;
						nb->Ptext.at(a)->csize = doku->Vorlagen[s].FontSize;
						nb->Ptext.at(a)->cstyle &= ~127;
						nb->Ptext.at(a)->cstyle |= doku->Vorlagen[s].FontEffect;
						nb->Ptext.at(a)->ccolor = doku->Vorlagen[s].FColor;
						nb->Ptext.at(a)->cshade = doku->Vorlagen[s].FShade;
						nb->Ptext.at(a)->cstroke = doku->Vorlagen[s].SColor;
						nb->Ptext.at(a)->cshade2 = doku->Vorlagen[s].SShade;
					}
				}
				if (s == 0)
				{
					nb->Ptext.at(a)->ccolor = nb->TxtFill;
					nb->Ptext.at(a)->cshade = nb->ShTxtFill;
					nb->Ptext.at(a)->cstroke = nb->TxtStroke;
					nb->Ptext.at(a)->cshade2 = nb->ShTxtStroke;
					nb->Ptext.at(a)->csize = nb->ISize;
					nb->Ptext.at(a)->cfont = nb->IFont;
					nb->Ptext.at(a)->cstyle &= ~127;
					nb->Ptext.at(a)->cstyle |= nb->TxTStyle;
				}
				if (nb->Ptext.at(a)->ch == QChar(13))
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
		b->Ausrich = s;
		if (b->Ptext.count() != 0)
		{
			for (a = 0; a < static_cast<int>(b->Ptext.count()); ++a)
			{
				b->Ptext.at(a)->cab = s;
				if (s > 4)
				{
					if (doku->Vorlagen[s].Font != "")
					{
						b->Ptext.at(a)->cfont = doku->Vorlagen[s].Font;
						b->Ptext.at(a)->csize = doku->Vorlagen[s].FontSize;
						b->Ptext.at(a)->cstyle &= ~127;
						b->Ptext.at(a)->cstyle |= doku->Vorlagen[s].FontEffect;
						b->Ptext.at(a)->ccolor = doku->Vorlagen[s].FColor;
						b->Ptext.at(a)->cshade = doku->Vorlagen[s].FShade;
						b->Ptext.at(a)->cstroke = doku->Vorlagen[s].SColor;
						b->Ptext.at(a)->cshade2 = doku->Vorlagen[s].SShade;
					}
				}
				if (s == 0)
				{
					b->Ptext.at(a)->ccolor = b->TxtFill;
					b->Ptext.at(a)->cshade = b->ShTxtFill;
					b->Ptext.at(a)->cstroke = b->TxtStroke;
					b->Ptext.at(a)->cshade2 = b->ShTxtStroke;
					b->Ptext.at(a)->cfont = b->IFont;
					b->Ptext.at(a)->csize = b->ISize;
					b->Ptext.at(a)->cstyle &= ~127;
					b->Ptext.at(a)->cstyle |= b->TxTStyle;
				}
			}
		}
	}
	if (!b->Tinput)
	{
		b->Dirty = true;
		RefreshItem(b);
	}
}

void Page::chKerning(double us)
{
	uint a;
	uint aa;
	if (SelItem.count() != 0)
	{
		for (aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *b = SelItem.at(aa);
			if ((b->HasSel) && (doku->AppMode == 7))
			{
				if (b->Ptext.count() != 0)
				{
					for (a = 0; a < b->Ptext.count(); ++a)
					{
						if (b->Ptext.at(a)->cselect)
							b->Ptext.at(a)->cextra = us;
					}
					b->Dirty = true;
					RefreshItem(b);
				}
			}
			else
			{
				if (b->Ptext.count() != 0)
				{
					for (a = 0; a < b->Ptext.count(); ++a)
					{
						b->Ptext.at(a)->cextra = us;
					}
					b->Dirty = true;
					RefreshItem(b);
					b->ExtraV = us;
				}
			}
		}
	}
}

void Page::chFSize(int size)
{
	uint a;
	uint aa;
	if (SelItem.count() != 0)
	{
		for (aa = 0; aa < SelItem.count(); ++aa)
		{
			PageItem *b = SelItem.at(0);
			doku->CurrFontSize = size;
			if (doku->AppMode != 7)
			{
				b->LineSp = ((size / 10.0) * static_cast<double>(doku->AutoLine) / 100) + (size / 10.0);
				doku->Vorlagen[0].LineSpa = b->LineSp;
				emit ItemTextAttr(b->LineSp);
				b->ISize = size;
				emit ItemTextSize(b->ISize);
				emit ItemTextCols(b->Cols, b->ColGap);
			}
			if (b->Ptext.count() != 0)
			{
				if (doku->AppMode == 7)
				{
					for (a = 0; a < b->Ptext.count(); ++a)
					{
						if (b->Ptext.at(a)->cselect)
							b->Ptext.at(a)->csize = size;
					}
				}
				else
				{
					for (a = 0; a < b->Ptext.count(); ++a)
					{
						if (b->Ptext.at(a)->cab < 5)
							b->Ptext.at(a)->csize = size;
					}
				}
				if (b->PType == 8)
				{
					UpdatePolyClip(b);
					AdjustItemSize(b);
				}
				b->Dirty = true;
				RefreshItem(b);
			}
		}
	}
}

void Page::QueryFarben()
{
	PageItem *b;
	if (GetItem(&b))
	{
		emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
		emit ItemGradient(b->GrColor2, b->GrColor, b->GrShade2, b->GrShade, b->GrType);
		emit ItemTrans(b->Transparency, b->TranspStroke);
	}
}

void Page::ToggleBookmark()
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

void Page::ToggleAnnotation()
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

void Page::ToggleLock()
{
	if (SelItem.count() != 0)
	{
		for ( uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->Locked = !SelItem.at(a)->Locked;
			RefreshItem(SelItem.at(a));
			emit HaveSel(SelItem.at(a)->PType);
		}
		emit DocChanged();
	}
}

void Page::TogglePic()
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

void Page::UpdatePic()
{
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		if (b->PicAvail)
		{
			int fho = b->flippedH;
			int fvo = b->flippedV;
			b->pixmOrg = QImage();
			LoadPict(b->Pfile, b->ItemNr);
			b->flippedH = fho;
			b->flippedV = fvo;
			AdjustPictScale(b);
			AdjustPreview(b, false);
			update();
		}
	}
}

// jjsa, added the following object for waiting on external process
// and refreshing the main application

class AppUserFilter : public QObject
{
public:
	AppUserFilter(QObject *o){};
protected:
	bool eventFilter(QObject *o, QEvent *e );
};

static QProcess *proc = 0;
static AppUserFilter *filter = 0;

bool AppUserFilter::eventFilter(QObject *o, QEvent *e )
{
	switch(e->type())
	{
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseButtonDblClick:
	case QEvent::MouseMove:
	case QEvent::TabletPress:
	case QEvent::TabletRelease:
	case QEvent::TabletMove:
	case QEvent::Enter:
	case QEvent::Leave:
	case QEvent::Close:
	case QEvent::Quit:
		return TRUE;
		break;
	default:
		return false;
		break;
	}
	return FALSE;
}

/* on exit of external programm, remove our event filter and */
/* do the rest of the job */
void Page::GimpExited()
{
	int ex;
	if ( proc != 0 )
		ex = proc->exitStatus();
	if ( filter != 0 )
	{
		qApp->removeEventFilter(filter);
		delete filter;
		filter = 0;
	}
	if ( proc != 0 )
	{
		ex = proc->exitStatus();
		delete proc;
		proc = 0;
	}
	PageItem *b = SelItem.at(0);
	if (b->PicAvail)
	{
		if ( ex == 0 )
		{
			int fho = b->flippedH;
			int fvo = b->flippedV;
			LoadPict(b->Pfile, b->ItemNr);
			b->flippedH = fho;
			b->flippedV = fvo;
			AdjustPictScale(b);
			AdjustPreview(b, false);
			update();
		}
	}
	qApp->mainWidget()->setEnabled(true);
}

/* call gimp and wait uppon completion */
void Page::CallGimp()
{
	QStringList cmd;
	if (SelItem.count() != 0)
	{
		PageItem *b = SelItem.at(0);
		if (b->PicAvail)
		{
			b->pixmOrg = QImage();
			proc = new QProcess(NULL);
            cmd = QStringList::split(" ", ScApp->Prefs.gimp_exe);
			cmd.append(b->Pfile);
			proc->setArguments(cmd);
			if ( !proc->start() )
			{
				delete proc;
				proc = 0;
				QString mess = tr("The Program")+" "+ScApp->Prefs.gimp_exe;
				mess += "\n" + tr("is missing!");
				QMessageBox::critical(this, tr("Warning"), mess, 1, 0, 0);
				return;
			}
			qApp->mainWidget()->setEnabled(false);
			connect(proc, SIGNAL(processExited()), this, SLOT(GimpExited()));
			filter = new AppUserFilter(this);
			qApp->installEventFilter(filter);
		}
	}
}

void Page::FlipImageH()
{
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->flippedH += 1;
			RefreshItem(SelItem.at(a));
		}
		emit DocChanged();
	}
}

void Page::FlipImageV()
{
	if (SelItem.count() != 0)
	{
		for (uint a = 0; a < SelItem.count(); ++a)
		{
			SelItem.at(a)->flippedV += 1;
			RefreshItem(SelItem.at(a));
		}
		emit DocChanged();
	}
}

void Page::ToBack()
{
	uint a, old;
	if ((Items.count() > 1) && (SelItem.count() != 0))
	{
		PageItem *b = SelItem.at(0);
		if ((b->isTableItem) && (b->isSingleSel))
			return;
		b = Items.take(SelItem.at(0)->ItemNr);
		old = SelItem.at(0)->ItemNr;
		storeUndoInf(b);
		doku->UnData.UnCode = 4;
		doku->UnDoValid = true;
		emit UndoAvail();
		b->Dirty = true;
		Items.prepend(b);
		for (a = 0; a < Items.count(); ++a)
		{
			Items.at(a)->ItemNr = a;
			if (Items.at(a)->isBookmark)
				emit NewBMNr(Items.at(a)->BMnr, a);
		}
		SelItem.clear();
		SelItem.append(b);
		emit MoveObj(PageNr, old, 0);
		emit DocChanged();
		update();
	}
}

void Page::ToFront()
{
	uint a, old;
	if ((Items.count() > 1) && (SelItem.count() != 0))
	{
		PageItem *b = SelItem.at(0);
		if ((b->isTableItem) && (b->isSingleSel))
			return;
		b = Items.take(SelItem.at(0)->ItemNr);
		old = SelItem.at(0)->ItemNr;
		storeUndoInf(b);
		doku->UnData.UnCode = 4;
		doku->UnDoValid = true;
		emit UndoAvail();
		b->Dirty = true;
		Items.append(b);
		for (a = 0; a < Items.count(); ++a)
		{
			Items.at(a)->ItemNr = a;
			if (Items.at(a)->isBookmark)
				emit NewBMNr(Items.at(a)->BMnr, a);
		}
		SelItem.clear();
		SelItem.append(b);
		emit MoveObj(PageNr, old, b->ItemNr);
		emit DocChanged();
		update();
	}
}

void Page::LowerItem()
{
	uint a, old;
	if ((Items.count() > 1) && (SelItem.count() != 0) && (SelItem.at(0)->ItemNr>0))
	{
		PageItem *b = SelItem.at(0);
		if ((b->isTableItem) && (b->isSingleSel))
			return;
		b = Items.take(SelItem.at(0)->ItemNr);
		old = SelItem.at(0)->ItemNr;
		storeUndoInf(b);
		doku->UnData.UnCode = 4;
		doku->UnDoValid = true;
		emit UndoAvail();
		b->Dirty = true;
		Items.insert(SelItem.at(0)->ItemNr-1, b);
		for (a = 0; a < Items.count(); ++a)
		{
			Items.at(a)->ItemNr = a;
			if (Items.at(a)->isBookmark)
				emit NewBMNr(Items.at(a)->BMnr, a);
		}
		SelItem.clear();
		SelItem.append(b);
		emit MoveObj(PageNr, old, old-1);
		emit DocChanged();
		update();
	}
}

void Page::RaiseItem()
{
	uint a, old;
	if ((Items.count() > 1) && (SelItem.count() != 0) && (SelItem.at(0)->ItemNr<Items.count()-2))
	{
		PageItem *b = SelItem.at(0);
		if ((b->isTableItem) && (b->isSingleSel))
			return;
		b = Items.take(SelItem.at(0)->ItemNr);
		old = SelItem.at(0)->ItemNr;
		storeUndoInf(b);
		doku->UnData.UnCode = 4;
		doku->UnDoValid = true;
		emit UndoAvail();
		b->Dirty = true;
		Items.insert(SelItem.at(0)->ItemNr+1, b);
		for (a = 0; a < Items.count(); ++a)
		{
			Items.at(a)->ItemNr = a;
			if (Items.at(a)->isBookmark)
				emit NewBMNr(Items.at(a)->BMnr, a);
		}
		SelItem.clear();
		SelItem.append(b);
		emit DocChanged();
		emit MoveObj(PageNr, old, old+1);
		update();
	}
}

void Page::ClearItem()
{
	PageItem *b;
	if (SelItem.count() != 0)
	{
		b = SelItem.at(0);
		if ((b->PType == 2) || (b->PType == 4))
		{
			if ((b->PType == 4) && (b->NextBox == 0) && (b->BackBox == 0))
			{
				b->Ptext.clear();
				b->CPos = 0;
			}
			b->Pfile = "";
			b->PicAvail = false;
			b->pixm = QImage();
			b->pixmOrg = QImage();
			if (b->PType == 2)
				emit UpdtObj(PageNr, b->ItemNr);
			RefreshItem(b);
			emit DocChanged();
		}
	}
}

void Page::DeleteItem()
{
	uint a, c, itnr, anz;
	PageItem *b;
	if (SelItem.count() != 0)
	{
		anz = SelItem.count();
		if (anz == 1)
		{
			if ((doku->UnData.UnCode == 0) && (doku->UnDoValid))
				delete doku->UnData.Item;
			doku->UnData.UnCode = 0;
			doku->UnData.PageNr = PageNr;
			doku->UnDoValid = true;
			emit UndoAvail();
		}
		for (uint de=0; de<anz; ++de)
		{
			b = SelItem.at(0);
			if ((b->isTableItem) && (b->isSingleSel))
				continue;
			if (b->PType == 4)
			{
				if ((b->NextBox != 0) || (b->BackBox != 0))
				{
					if (b->BackBox == 0)
					{
						b->NextBox->BackBox = b->BackBox;
						c = b->Ptext.count();
						for (a = 0; a < c; ++a)
							b->NextBox->Ptext.prepend(b->Ptext.take(b->Ptext.count()-1));
					}
					else
					{
						b->BackBox->NextBox = b->NextBox;
						if (b->NextBox != 0)
						{
							b->NextBox->BackBox = b->BackBox;
						}
						else
						{
							if (b->isAutoText)
								doku->LastAuto = b->BackBox;
						}
						c = b->Ptext.count();
						for (a = 0; a < c; ++a)
							b->BackBox->Ptext.append(b->Ptext.take(0));
					}
				}
			}
			if (SelItem.at(0)->isBookmark)
				emit DelBM(SelItem.at(0));
			doku->UnData.Item = Items.take(SelItem.at(0)->ItemNr);
			emit DelObj(PageNr, SelItem.at(0)->ItemNr);
			SelItem.removeFirst();
			for (a = 0; a < Items.count(); ++a)
			{
				itnr = Items.at(a)->ItemNr;
				Items.at(a)->ItemNr = a;
				if (Items.at(a)->isBookmark)
					emit NewBMNr(Items.at(a)->BMnr, a);
				for (uint dxx=0; dxx<SelItem.count(); ++dxx)
				{
					if (SelItem.at(dxx)->ItemNr == itnr)
						SelItem.at(dxx)->ItemNr = a;
				}
			}
		}
		if (GroupSel)
		{
			double x, y, w, h;
			getGroupRectScreen(&x, &y, &w, &h);
			SelItem.clear();
			GroupSel = false;
			repaint(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10),
			              static_cast<int>(h+10)));
		}
		else
		{
			SelItem.clear();
			update();
		}
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		emit HaveSel(-1);
		emit DocChanged();
	}
}

void Page::Deselect(bool prop)
{
	double x, y, w, h;
	PageItem* b;
	if (doku->ActPage->SelItem.count() != 0)
	{
		for (uint a = 0; a < doku->ActPage->Items.count(); ++a)
		{
			b = doku->ActPage->Items.at(a);
			if ((b->PType == 4) && (b->isBookmark))
				emit ChBMText(b);
			if (b->PType == 2)
				AdjustPreview(b, !doku->DragP);
			if (b->Select)
				b->Select = false;
		}
		if (GroupSel)
		{
			getGroupRectScreen(&x, &y, &w, &h);
			doku->ActPage->SelItem.clear();
			repaint(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10),
			              static_cast<int>(h+10)));
		}
		else
			doku->ActPage->SelItem.clear();
		doku->ActPage->update();
		GroupSel = false;
	}
	if (prop)
		emit HaveSel(-1);
}

void Page::PasteItem(struct CLBuf *Buffer, bool loading, bool drag)
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
	struct Pti *hg;
	switch (Buffer->PType)
	{
	case 1:
		z = PaintEllipse(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	case 2:
		z = PaintPict(x, y, w, h);
		Items.at(z)->LocalScX = Buffer->LocalScX;
		Items.at(z)->LocalScY = Buffer->LocalScY;
		Items.at(z)->LocalX = Buffer->LocalX;
		Items.at(z)->LocalY = Buffer->LocalY;
		Items.at(z)->Pfile = Buffer->Pfile;
		Items.at(z)->IProfile = Buffer->IProfile;
		Items.at(z)->EmProfile = Buffer->EmProfile;
		Items.at(z)->IRender = Buffer->IRender;
		Items.at(z)->UseEmbedded = Buffer->UseEmbedded;
		if (Items.at(z)->Pfile != "")
			LoadPict(Items.at(z)->Pfile, z);
		Items.at(z)->LocalScX = Buffer->LocalScX;
		Items.at(z)->LocalScY = Buffer->LocalScY;
		Items.at(z)->PicArt = Buffer->PicArt;
		Items.at(z)->BBoxX = Buffer->BBoxX;
		Items.at(z)->BBoxH = Buffer->BBoxH;
		Items.at(z)->ScaleType = Buffer->ScaleType;
		Items.at(z)->AspectRatio = Buffer->AspectRatio;
		Items.at(z)->Pwidth = Buffer->Pwidth;
		break;
	case 3:
		z = PaintRect(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	case 8:
	case 4:
		z = PaintText(x, y, w, h, pw, Buffer->Pcolor);
		if ((Buffer->isAnnotation) && (Buffer->AnUseIcons))
		{
			Items.at(z)->LocalScX = Buffer->LocalScX;
			Items.at(z)->LocalScY = Buffer->LocalScY;
			Items.at(z)->LocalX = Buffer->LocalX;
			Items.at(z)->LocalY = Buffer->LocalY;
			Items.at(z)->Pfile = Buffer->Pfile;
			Items.at(z)->Pfile2 = Buffer->Pfile2;
			Items.at(z)->Pfile3 = Buffer->Pfile3;
			Items.at(z)->IProfile = Buffer->IProfile;
			Items.at(z)->EmProfile = Buffer->EmProfile;
			Items.at(z)->IRender = Buffer->IRender;
			Items.at(z)->UseEmbedded = Buffer->UseEmbedded;
			LoadPict(Items.at(z)->Pfile, z);
			Items.at(z)->LocalScX = Buffer->LocalScX;
			Items.at(z)->LocalScY = Buffer->LocalScY;
			Items.at(z)->PicArt = Buffer->PicArt;
			Items.at(z)->BBoxX = Buffer->BBoxX;
			Items.at(z)->BBoxH = Buffer->BBoxH;
		}
		if (Buffer->Ptext != "")
		{
			QTextStream t(&Buffer->Ptext, IO_ReadOnly);
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
				hg = new Pti;
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
				hg->cstroke = it == NULL ? "None" : *it;
				it++;
				hg->cshade2 = it == NULL ? 100 : (*it).toInt();
				it++;
				hg->cscale = it == NULL ? 100 : (*it).toInt();
				hg->xp = 0;
				hg->yp = 0;
				hg->PRot = 0;
				hg->PtransX = 0;
				hg->PtransY = 0;
				Items.at(z)->Ptext.append(hg);
			}
		}
		Items.at(z)->LineSp = Buffer->LineSp;
		Items.at(z)->PType = Buffer->PType;
		break;
	case 5:
		z = PaintLine(x, y, w, h, pw, Buffer->Pcolor2);
		break;
	case 6:
		z = PaintPoly(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	case 7:
		z = PaintPolyLine(x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2);
		break;
	}
	PageItem* b = Items.at(z);
	b->flippedH = Buffer->flippedH;
	b->flippedV = Buffer->flippedV;
	b->RadRect = Buffer->RadRect;
	b->FrameType = Buffer->FrameType;
	b->ClipEdited = Buffer->ClipEdited;
	b->Pcolor = Buffer->Pcolor;
	b->Pcolor2 = Buffer->Pcolor2;
	b->Shade = Buffer->Shade;
	b->Shade2 = Buffer->Shade2;
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
	if (Buffer->AnName != "")
	{
		if (!drag)
		{
			if (b->AnName == Buffer->AnName)
				b->AutoName = true;
			else
			{
				b->AnName = Buffer->AnName;
				b->AutoName = false;
			}
		}
		else
		{
			ScribusView* view = (ScribusView*)Anz;
			bool found = false;
			for (uint a = 0; a < view->Pages.count(); ++a)
			{
				for (uint c = 0; c < view->Pages.at(a)->Items.count(); ++c)
				{
					if (Buffer->AnName == view->Pages.at(a)->Items.at(c)->AnName)
					{
						found = true;
						break;
					}
				}
				if (found)
					break;
			}
			if (found)
				b->AnName = tr("Copy of")+" "+Buffer->AnName;
			else
				b->AnName = Buffer->AnName;
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
	b->Ausrich = Buffer->Ausrich;
	b->IFont = Buffer->IFont;
	b->ISize = Buffer->ISize;
	b->ExtraV = Buffer->ExtraV;
	b->Groups = Buffer->Groups;
	b->TabValues = Buffer->TabValues;
	b->DashValues = Buffer->DashValues;
	b->DashOffset = Buffer->DashOffset;
	b->Locked = Buffer->Locked;
	b->LockRes = Buffer->LockRes;
	b->Transparency = Buffer->Transparency;
	b->TranspStroke = Buffer->TranspStroke;
	b->Reverse = Buffer->Reverse;
	b->InvPict = Buffer->InvPict;
	b->NamedLStyle = Buffer->NamedLStyle;
	b->Language = Buffer->Language;
	b->Cols = Buffer->Cols;
	b->ColGap = Buffer->ColGap;
	if (Buffer->LayerNr != -1)
		b->LayerNr = Buffer->LayerNr;
	b->PoLine = Buffer->PoLine.copy();
	if (b->PType != 5)
	{
		if ((b->PoLine.size() == 0) && (b->PType != 1))
			ConvertClip(b);
		else
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
	}
	if (b->PType == 1)
		SetOvalFrame(b);
	if (b->PType == 8)
	{
		b->Frame = false;
		UpdatePolyClip(b);
	}
	if (b->PType == 3)
	{
		if (b->RadRect != 0)
			SetFrameRound(b);
		else
			SetRectFrame(b);
	}
	if (b->PType == 2)
	{
		AdjustPictScale(b);
		AdjustPreview(b, false);
	}
	if ((b->PType != 4) && (b->PType != 8))
		b->IFont = doku->Dfont;
	if (Buffer->GrType != 0)
	{
		b->fill_gradient.clearStops();
		if (Buffer->GrType == 5)
		{
			if ((Buffer->GrColor != "None") && (Buffer->GrColor != ""))
				b->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
			b->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0);
			if ((Buffer->GrColor2 != "None") && (Buffer->GrColor2 != ""))
				b->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
			b->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0);
		}
		else
		{
			if ((Buffer->GrColor2 != "None") && (Buffer->GrColor2 != ""))
				b->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
			b->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0);
			if ((Buffer->GrColor != "None") && (Buffer->GrColor != ""))
				b->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
			b->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0);
		}
		b->GrColor2 = Buffer->GrColor2;
		b->GrShade2 = Buffer->GrShade2;
		b->GrColor = Buffer->GrColor;
		b->GrShade = Buffer->GrShade;
		b->GrType = Buffer->GrType;
	}
	if (!loading)
	{
		b->Select = true;
		b->Dirty = true;
		SelItem.append(b);
		emit HaveSel(b->PType);
		EmitValues(b);
		emit DocChanged();
		update();
	}
	if ((b->PType == 4) && (loading))
		b->Dirty = true;
	if ((b->isBookmark) && (!loading))
		emit NewBMNr(b->BMnr, z);
}

void Page::SetupDraw(int nr)
{
	PageItem* b = Items.at(nr);
	b->Select = true;
	mCG = true;
	HowTo = 1;
	qApp->setOverrideCursor(QCursor(SizeFDiagCursor), true);
	SelItem.clear();
	SelItem.append(b);
	b->paintObj();
	Imoved = true;
	doku->AppMode = 1;
	emit DocChanged();
	b->Sizing = b->PType == 5 ? false : true;
	EmitValues(b);
}

void Page::EmitValues(PageItem *b)
{
	emit ItemPos(b->Xpos, b->Ypos);
	emit ItemGeom(b->Width, b->Height);
	emit SetAngle(b->Rot);
	emit SetSizeValue(b->Pwidth);
	emit SetLineArt(b->PLineArt, b->PLineEnd, b->PLineJoin);
	emit SetLocalValues(b->LocalScX, b->LocalScY, b->LocalX, b->LocalY );
	emit ItemFarben(b->Pcolor2, b->Pcolor, b->Shade2, b->Shade);
	emit ItemGradient(b->GrColor2, b->GrColor, b->GrShade2, b->GrShade, b->GrType);
	emit ItemTrans(b->Transparency, b->TranspStroke);
	emit ItemTextAttr(b->LineSp);
	emit ItemTextUSval(b->ExtraV);
	emit ItemTextCols(b->Cols, b->ColGap);
	if (doku->AppMode != 7)
	{
		emit ItemTextAbs(b->Ausrich);
		emit ItemTextFont(b->IFont);
		emit ItemTextSize(b->ISize);
	}
}

void Page::AlignObj(bool xa, bool ya, bool Vth, bool Vtv, double xdisp, double ydisp, int xart, int yart)
{
	uint a;
	double xp = 999999;
	double yp = 999999;
	double xm = 0;
	double xd, xo;
	QPtrList<double> ml;
	QPainter p;
	PageItem *b;
	ml.setAutoDelete(true);
	ml.clear();
	if (xa)
	{
		if (Vth)
		{
			for (a=0; a<SelItem.count(); ++a)
			{
				p.begin(this);
				b = SelItem.at(a);
				Transform(b, &p);
				switch (xart)
				{
				case 0:
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().x()));
					break;
				case 1:
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().x()+
					                     QRegion(p.xForm(b->Clip)).boundingRect().width() / 2));
					break;
				case 2:
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().right()));
					break;
				}
				p.end();
			}
			xp = 99999;
			xm = 0;
			for (a=0; a<SelItem.count(); ++a)
			{
				xp = QMIN(*ml.at(a), xp);
				xm = QMAX(*ml.at(a), xm);
			}
			xd = (xm - xp) / (SelItem.count()-1);
			for	(a=0; a<SelItem.count(); ++a)
			{
				xo = *ml.at(a);
				MoveItem((xp + xd * a) - xo, 0, SelItem.at(a));
			}
			ml.clear();
		}
		else
		{
			switch (xart)
			{
			case 0:
				for (a=0; a<SelItem.count(); ++a)
				{
					b = SelItem.at(a);
					p.begin(this);
					Transform(b, &p);
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().x()));
					p.end();
				}
				xp = 99999;
				for (a=0; a<SelItem.count(); ++a)
					xp = QMIN(*ml.at(a), xp);
				for	(a=0; a<SelItem.count(); ++a)
					MoveItem(xp - *ml.at(a) + a*xdisp, 0, SelItem.at(a));
				ml.clear();
				break;
			case 1:
				for (a=0; a<SelItem.count(); ++a)
				{
					b = SelItem.at(a);
					p.begin(this);
					Transform(b, &p);
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().x()+
					                     QRegion(p.xForm(b->Clip)).boundingRect().width() / 2));
					p.end();
				}
				xp = *ml.at(0);
				for	(a=0; a<SelItem.count(); ++a)
					MoveItem(xp - *ml.at(a) + a*xdisp, 0, SelItem.at(a));
				ml.clear();
				break;
			case 2:
				xp = 0;
				for (a=0; a<SelItem.count(); ++a)
				{
					b = SelItem.at(a);
					p.begin(this);
					Transform(b, &p);
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().right()));
					p.end();
				}
				for (a=0; a<SelItem.count(); ++a)
					xp = QMAX(*ml.at(a), xp);
				for	(a=0; a<SelItem.count(); ++a)
					MoveItem(xp - *ml.at(a) + a*xdisp, 0, SelItem.at(a));
				ml.clear();
				break;
			}
		}
	}
	if (ya)
	{
		if (Vtv)
		{
			for (a=0; a<SelItem.count(); ++a)
			{
				p.begin(this);
				b = SelItem.at(a);
				Transform(b, &p);
				switch (yart)
				{
				case 0:
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().y()));
					break;
				case 1:
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().y()+
					                     QRegion(p.xForm(b->Clip)).boundingRect().height() / 2));
					break;
				case 2:
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().bottom()));
					break;
				}
				p.end();
			}
			xp = 99999;
			xm = 0;
			for (a=0; a<SelItem.count(); ++a)
			{
				xp = QMIN(*ml.at(a), xp);
				xm = QMAX(*ml.at(a), xm);
			}
			xd = (xm - xp) / (SelItem.count()-1);
			for	(a=0; a<SelItem.count(); ++a)
			{
				xo = *ml.at(a);
				MoveItem(0, (xp + xd * a) - xo, SelItem.at(a));
			}
			ml.clear();
		}
		else
		{
			switch (yart)
			{
			case 0:
				for (a=0; a<SelItem.count(); ++a)
				{
					b = SelItem.at(a);
					p.begin(this);
					Transform(b, &p);
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().y()));
					p.end();
				}
				yp = 99999;
				for (a=0; a<SelItem.count(); ++a)
					yp = QMIN(*ml.at(a), yp);
				for	(a=0; a<SelItem.count(); ++a)
					MoveItem(0, yp - *ml.at(a) + a*ydisp, SelItem.at(a));
				ml.clear();
				break;
			case 1:
				for (a=0; a<SelItem.count(); ++a)
				{
					b = SelItem.at(a);
					p.begin(this);
					Transform(b, &p);
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().y()+
					                     QRegion(p.xForm(b->Clip)).boundingRect().height() / 2));
					p.end();
				}
				xp = *ml.at(0);
				for	(a=0; a<SelItem.count(); ++a)
					MoveItem(0, xp - *ml.at(a) + a*ydisp, SelItem.at(a));
				ml.clear();
				break;
			case 2:
				yp = 0;
				for (a=0; a<SelItem.count(); ++a)
				{
					b = SelItem.at(a);
					p.begin(this);
					Transform(b, &p);
					ml.append(new double(QRegion(p.xForm(b->Clip)).boundingRect().bottom()));
					p.end();
				}
				for (a=0; a<SelItem.count(); ++a)
					yp = QMAX(*ml.at(a), yp);
				for	(a=0; a<SelItem.count(); ++a)
					MoveItem(0, yp - *ml.at(a) + a*ydisp, SelItem.at(a));
				ml.clear();
				break;
			}
		}
	}
	update();
}

/** Zeichnet eine Ellipse */
int Page::PaintEllipse(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	PageItem* ite = new PageItem(this, 6, x, y, b, h, w, fill, outline, doku);
	Items.append(ite);
	ite->PLineArt = doku->DLineArt;
	ite->Shade = doku->Dshade;
	ite->Shade2 = doku->Dshade2;
	ite->ItemNr = Items.count()-1;
	SetOvalFrame(ite);
	if (!doku->loading)
	{
		ite->paintObj();
		emit AddObj(PageNr, ite->ItemNr);
	}
	return ite->ItemNr;
}

/** Zeichnet einen Bildrahmen */
int Page::PaintPict(double x, double y, double b, double h)
{
	PageItem* ite = new PageItem(this, 2, x, y, b, h, 1, doku->DbrushPict, "None", doku);
	Items.append(ite);
	ite->Shade = doku->ShadePict;
	ite->LocalScX = doku->ScaleX;
	ite->LocalScY = doku->ScaleY;
	ite->ScaleType = doku->ScaleType;
	ite->AspectRatio = doku->AspectRatio;
	ite->IProfile = doku->CMSSettings.DefaultInputProfile;
	ite->IRender = doku->CMSSettings.DefaultIntentMonitor2;
	ite->ItemNr = Items.count()-1;
	SetRectFrame(ite);
	if (!doku->loading)
	{
		ite->paintObj();
		emit AddObj(PageNr, ite->ItemNr);
	}
	return ite->ItemNr;
}

/** Zeichnet ein Rechteck */
int Page::PaintRect(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	PageItem* ite = new PageItem(this, 6, x, y, b, h, w, fill, outline, doku);
	Items.append(ite);
	ite->PLineArt = doku->DLineArt;
	ite->Shade = doku->Dshade;
	ite->Shade2 = doku->Dshade2;
	ite->ItemNr = Items.count()-1;
	SetRectFrame(ite);
	if (!doku->loading)
	{
		ite->paintObj();
		emit AddObj(PageNr, ite->ItemNr);
	}
	return ite->ItemNr;
}

/** Zeichnet ein Polygon */
int Page::PaintPoly(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	PageItem* ite = new PageItem(this, 6, x, y, b, h, w, fill, outline, doku);
	Items.append(ite);
	ite->PLineArt = doku->DLineArt;
	ite->Shade = doku->Dshade;
	ite->Shade2 = doku->Dshade2;
	ite->ItemNr = Items.count()-1;
	ite->ClipEdited = true;
	ite->FrameType = 3;
	if (!doku->loading)
	{
		ite->paintObj();
		emit AddObj(PageNr, ite->ItemNr);
	}
	return ite->ItemNr;
}

/** Zeichnet eine Polyline */
int Page::PaintPolyLine(double x, double y, double b, double h, double w, QString fill, QString outline)
{
	PageItem* ite = new PageItem(this, 7, x, y, b, h, w, fill, outline, doku);
	Items.append(ite);
	ite->PLineArt = doku->DLineArt;
	ite->Shade = doku->Dshade;
	ite->Shade2 = doku->Dshade2;
	ite->ItemNr = Items.count()-1;
	ite->ClipEdited = true;
	if (!doku->loading)
	{
		ite->paintObj();
		emit AddObj(PageNr, ite->ItemNr);
	}
	return ite->ItemNr;
}

/** Zeichnet einen Textrahmen */
int Page::PaintText(double x, double y, double b, double h, double w, QString outline)
{
	if (w == 0)
		w = 1;
	PageItem* ite = new PageItem(this, 4, x, y, b, h, w, "None", outline, doku);
	Items.append(ite);
	ite->ItemNr = Items.count()-1;
	SetRectFrame(ite);
	if (!doku->loading)
	{
		ite->paintObj();
		emit AddObj(PageNr, ite->ItemNr);
	}
	return ite->ItemNr;
}

/** Zeichnet eine Linie */
int Page::PaintLine(double x, double y, double b, double h, double w, QString outline)
{
	if (w == 0)
		w = 1;
//	PageItem* ite = new PageItem(this, 5, x, y, b, h, w, "Black", outline, doku);
	PageItem* ite = new PageItem(this, 5, x, y, b, h, w, "None", outline, doku);
	Items.append(ite);
	ite->PLineArt = doku->DLstyleLine;
	ite->Shade2 = doku->DshadeLine;
	ite->ItemNr = Items.count()-1;
	if (!doku->loading)
	{
		ite->paintObj();
		emit AddObj(PageNr, ite->ItemNr);
	}
	return ite->ItemNr;
}

void Page::insertColor(QString nam, double c, double m, double y, double k)
{
	if (!doku->PageColors.contains(nam))
	{
		CMYKColor tmp = CMYKColor(static_cast<int>(255 * c), static_cast<int>(255 * m),
		                          static_cast<int>(255 * y), static_cast<int>(255 * k));
		doku->PageColors.insert(nam, tmp);
	}
}

void Page::LoadPict(QString fn, int ItNr)
{
	QString tmp, tmp2, dummy, cmd1, cmd2, BBox, FarNam;
	QChar tc;
	QString oldPr = doku->CMSSettings.DefaultInputProfile;
	double x, y, b, h, c, m, k;
	bool found = false;
	int ret = -1;
	QString tmpFile = QString(getenv("HOME"))+"/.scribus/sc.png";
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
	{
		Items.at(ItNr)->Pfile = fi.absFilePath();
		Items.at(ItNr)->PicAvail = false;
		Items.at(ItNr)->PicArt = false;
		return;
	}
	QString ext = fi.extension(false).lower();
	if (ext == "pdf")
	{
		QStringList args;
		args.append("-r72");
		args.append("-sOutputFile="+tmpFile);
		args.append("-dFirstPage=1");
		args.append("-dLastPage=1");
		args.append(fn);
		ret = callGS(args);
		if (ret == 0)
		{
			QImage im4;
			QImage image;
			image.load(tmpFile);
			image = image.convertDepth(32);
			im4 = ProofPict(&image, Items.at(ItNr)->IProfile, Items.at(ItNr)->IRender);
			Items.at(ItNr)->pixm = im4;
			Items.at(ItNr)->pixmOrg = im4.copy();
			Items.at(ItNr)->Pfile = fi.absFilePath();
			Items.at(ItNr)->PicAvail = true;
			Items.at(ItNr)->PicArt = true;
			Items.at(ItNr)->isRaster = false;
			Items.at(ItNr)->BBoxX = 0;
			Items.at(ItNr)->BBoxH = Items.at(ItNr)->pixm.height();
			Items.at(ItNr)->OrigW = Items.at(ItNr)->pixm.width();
			Items.at(ItNr)->OrigH = Items.at(ItNr)->pixm.height();
			Items.at(ItNr)->LocalViewX = Items.at(ItNr)->LocalScX;
			Items.at(ItNr)->LocalViewY = Items.at(ItNr)->LocalScY;
			Items.at(ItNr)->dpiX = 72.0;
			Items.at(ItNr)->dpiY = 72.0;
			unlink(tmpFile);
		}
		else
		{
			Items.at(ItNr)->Pfile = fi.absFilePath();
			Items.at(ItNr)->PicAvail = false;
			Items.at(ItNr)->PicArt = false;
		}
		if (!doku->loading)
			emit RasterPic(Items.at(ItNr)->isRaster);
		return;
	}
	if ((ext == "eps") || (ext == "ps"))
	{
		QFile f(fn);
		if (f.open(IO_ReadOnly))
		{
			QTextStream ts(&f);
			while (!ts.atEnd())
			{
				tc = ' ';
				tmp = "";
				while ((tc != '\n') && (tc != '\r'))
				{
					ts >> tc;
					if ((tc != '\n') && (tc != '\r'))
						tmp += tc;
				}
				if (tmp.startsWith("%%BoundingBox"))
				{
					found = true;
					BBox = tmp;
				}
				if (tmp.startsWith("%%CMYKCustomColor"))
				{
					QTextStream ts2(&tmp, IO_ReadOnly);
					ts2 >> dummy >> c >> m >> y >> k;
					FarNam = ts2.read();
					FarNam = FarNam.stripWhiteSpace();
					FarNam = FarNam.remove(0,1);
					FarNam = FarNam.remove(FarNam.length()-1,1);
					insertColor(FarNam, c, m, y, k);
					while (!ts.atEnd())
					{
						tc = ' ';
						tmp = "";
						while ((tc != '\n') && (tc != '\r'))
						{
							ts >> tc;
							if ((tc != '\n') && (tc != '\r'))
								tmp += tc;
						}
						if (!tmp.startsWith("%%+"))
							break;
						QTextStream ts2(&tmp, IO_ReadOnly);
						ts2 >> dummy >> c >> m >> y >> k;
						FarNam = ts2.read();
						FarNam = FarNam.stripWhiteSpace();
						FarNam = FarNam.remove(0,1);
						FarNam = FarNam.remove(FarNam.length()-1,1);
						insertColor(FarNam, c, m, y, k);
					}
				}
				if (tmp.startsWith("%%EndComments"))
					break;
			}
			f.close();
			if (found)
			{
				QTextStream ts2(&BBox, IO_ReadOnly);
				ts2 >> dummy >> x >> y >> b >> h;
				QStringList args;
				args.append("-r72");
				args.append("-g"+tmp.setNum(qRound(b))+"x"+tmp2.setNum(qRound(h)));
				args.append("-sOutputFile="+tmpFile);
				args.append(fn);
				ret = callGS(args);
				if (ret == 0)
				{
					QImage im4;
					QImage image;
					image.load(tmpFile);
					image = image.convertDepth(32);
					image.setAlphaBuffer(true);
					if (ScApp->HavePngAlpha != 0)
					{
						int wi = image.width();
						int hi = image.height();
						for( int yi=0; yi < hi; ++yi )
						{
							QRgb *s = (QRgb*)(image.scanLine( yi ));
							for(int xi=0; xi < wi; ++xi )
							{
								if((*s) == 0xffffffff)
									(*s) &= 0x00ffffff;
								s++;
							}
						}
					}
					im4.setAlphaBuffer(true);
					im4 = image.copy(static_cast<int>(x), 0, static_cast<int>(b-x), static_cast<int>(h-y));
					image = ProofPict(&im4, Items.at(ItNr)->IProfile, Items.at(ItNr)->IRender);
					Items.at(ItNr)->pixm = image;
					Items.at(ItNr)->pixmOrg = image.copy();
					Items.at(ItNr)->PicAvail = true;
					Items.at(ItNr)->PicArt = true;
					Items.at(ItNr)->isRaster = false;
					Items.at(ItNr)->BBoxX = x;
					Items.at(ItNr)->BBoxH = h;
					Items.at(ItNr)->OrigW = Items.at(ItNr)->pixm.width();
					Items.at(ItNr)->OrigH = Items.at(ItNr)->pixm.height();
					Items.at(ItNr)->LocalViewX = Items.at(ItNr)->LocalScX;
					Items.at(ItNr)->LocalViewY = Items.at(ItNr)->LocalScY;
					Items.at(ItNr)->dpiX = 72.0;
					Items.at(ItNr)->dpiY = 72.0;
					Items.at(ItNr)->Pfile = fi.absFilePath();
					unlink(tmpFile);
				}
				else
				{
					Items.at(ItNr)->Pfile = fi.absFilePath();
					Items.at(ItNr)->PicAvail = false;
					Items.at(ItNr)->PicArt = false;
				}
			}
		}
		else
		{
			Items.at(ItNr)->Pfile = fi.absFilePath();
			Items.at(ItNr)->PicAvail = false;
			Items.at(ItNr)->PicArt = false;
		}
		if (!doku->loading)
			emit RasterPic(Items.at(ItNr)->isRaster);
		return;
	}
#ifdef HAVE_TIFF
	if (ext == "tif")
	{
		QImage img;
		QImage inI2;
#ifdef HAVE_CMS
		DWORD EmbedLen = 0;
		LPBYTE EmbedBuffer;
		const char *Descriptor;
		cmsHPROFILE hIn;
#endif
		TIFF* tif = TIFFOpen(fn, "r");
		if(tif)
		{
			unsigned width, height,size;
			float xresIn, yresIn;
			double xres, yres;
			TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
			TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
			TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xresIn);
			TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yresIn);
			xres = static_cast<double>(xresIn);
			yres = static_cast<double>(yresIn);
			size=width*height;
			uint32 *bits=(uint32*) _TIFFmalloc(size * sizeof (uint32));
			if(bits)
			{
				if (TIFFReadRGBAImage(tif, width, height, bits, 0))
				{
					img.create(width,height,32);
					img.setAlphaBuffer(true);
					if(TIFFGetR(0x1234567)==qRed  (0x1234567) &&
					        TIFFGetG(0x1234567)==qGreen(0x1234567) &&
					        TIFFGetB(0x1234567)==qBlue (0x1234567))
					{
						for (unsigned y=0; y<height; ++y)
							memcpy(img.scanLine(height-1-y),bits+y*width,width*4);
					}
					else
					{
						uint32 *inp=bits;
						for (unsigned y=0; y<height; ++y)
						{
							QRgb *row=(QRgb*) (img.scanLine(height-1-y));
							for(unsigned x=0; x<width; ++x)
							{
								const uint32 col=*(inp++);
								row[x]=qRgba(TIFFGetR(col), TIFFGetG(col), TIFFGetB(col), TIFFGetA(col));
							}
						}
					}
#ifdef HAVE_CMS
					if((TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer)) &&
					        (Items.at(ItNr)->UseEmbedded))
					{
						hIn = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
						Descriptor = cmsTakeProductDesc(hIn);
						if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
						{
							Items.at(ItNr)->IProfile = "Embedded " + QString(Descriptor);
							Items.at(ItNr)->EmProfile = "Embedded " + QString(Descriptor);
							inI2 = ProofPict(&img, Items.at(ItNr)->IProfile, Items.at(ItNr)->IRender, hIn);
						}
						else
						{
							if (!InputProfiles.contains(Items.at(ItNr)->IProfile))
								Items.at(ItNr)->IProfile = oldPr;
							Items.at(ItNr)->EmProfile = "";
							Items.at(ItNr)->UseEmbedded = false;
							inI2 = ProofPict(&img, Items.at(ItNr)->IProfile, Items.at(ItNr)->IRender);
						}
						cmsCloseProfile(hIn);
					}
					else
					{
						if (!InputProfiles.contains(Items.at(ItNr)->IProfile))
							Items.at(ItNr)->IProfile = oldPr;
						inI2 = ProofPict(&img, Items.at(ItNr)->IProfile, Items.at(ItNr)->IRender);
					}
#else
					inI2 = ProofPict(&img, Items.at(ItNr)->IProfile, Items.at(ItNr)->IRender);
#endif
					Items.at(ItNr)->pixm = inI2;
					Items.at(ItNr)->pixmOrg = QImage();
					Items.at(ItNr)->PicAvail = true;
					Items.at(ItNr)->PicArt = true;
					Items.at(ItNr)->BBoxX = 0;
					if (Items.at(ItNr)->Pfile != fn)
					{
						Items.at(ItNr)->LocalScX = 72.0 / xres;
						Items.at(ItNr)->LocalScY = 72.0 / yres;
						Items.at(ItNr)->LocalViewX = 72.0 / xres;
						Items.at(ItNr)->LocalViewY = 72.0 / yres;
					}
					else
					{
						Items.at(ItNr)->LocalViewX = Items.at(ItNr)->LocalScX;
						Items.at(ItNr)->LocalViewY = Items.at(ItNr)->LocalScY;
					}
					Items.at(ItNr)->Pfile = fi.absFilePath();
					Items.at(ItNr)->BBoxH = Items.at(ItNr)->pixm.height();
					Items.at(ItNr)->OrigW = Items.at(ItNr)->pixm.width();
					Items.at(ItNr)->OrigH = Items.at(ItNr)->pixm.height();
					Items.at(ItNr)->isRaster = true;
					Items.at(ItNr)->dpiX = xres;
					Items.at(ItNr)->dpiY = yres;
				}
				_TIFFfree(bits);
			}
			TIFFClose(tif);
		}
		else
		{
			Items.at(ItNr)->Pfile = fi.absFilePath();
			Items.at(ItNr)->PicAvail = false;
			Items.at(ItNr)->PicArt = false;
		}
	}
#endif
	else
	{
		QImage inI;
		QImage inI2;
		if (inI.load(fn))
		{
			double dpiX = QMAX(72.0, qRound(inI.dotsPerMeterX() / 100.0 * 2.54));
			double dpiY = QMAX(72.0, qRound(inI.dotsPerMeterY() / 100.0 * 2.54));
			inI = inI.convertDepth(32);
			inI2 = ProofPict(&inI, Items.at(ItNr)->IProfile, Items.at(ItNr)->IRender);
			Items.at(ItNr)->pixm = inI2.copy();
			Items.at(ItNr)->pixmOrg = QImage();
			if (Items.at(ItNr)->Pfile != fn)
			{
				Items.at(ItNr)->LocalScX = 72.0 / dpiX;
				Items.at(ItNr)->LocalScY = 72.0 / dpiY;
				Items.at(ItNr)->LocalViewX = 72.0 / dpiX;
				Items.at(ItNr)->LocalViewY = 72.0 / dpiY;
			}
			else
			{
				Items.at(ItNr)->LocalViewX = Items.at(ItNr)->LocalScX;
				Items.at(ItNr)->LocalViewY = Items.at(ItNr)->LocalScY;
			}
			Items.at(ItNr)->Pfile = fi.absFilePath();
			Items.at(ItNr)->PicAvail = true;
			Items.at(ItNr)->PicArt = true;
			Items.at(ItNr)->BBoxX = 0;
			Items.at(ItNr)->BBoxH = Items.at(ItNr)->pixm.height();
			Items.at(ItNr)->isRaster = true;
			Items.at(ItNr)->OrigW = Items.at(ItNr)->pixm.width();
			Items.at(ItNr)->OrigH = Items.at(ItNr)->pixm.height();
			Items.at(ItNr)->dpiX = dpiX;
			Items.at(ItNr)->dpiY = dpiY;
		}
		else
		{
			Items.at(ItNr)->Pfile = fi.absFilePath();
			Items.at(ItNr)->PicAvail = false;
			Items.at(ItNr)->PicArt = false;
		}
	}
	Items.at(ItNr)->flippedH = 0;
	Items.at(ItNr)->flippedV = 0;
	if (!doku->loading)
	{
		emit RasterPic(Items.at(ItNr)->isRaster);
		emit UpdtObj(PageNr, ItNr);
	}
	emit DocChanged();
}

