/***************************************************************************
                          pageitem.cpp  -  description
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

#include "pageitem.h"
#include <qpainter.h>
#include <qpen.h>
#include <qfontmetrics.h>
#include <qregion.h>
#include <qpoint.h>
#include <qfileinfo.h>
#include <qpaintdevicemetrics.h>
#include <qdrawutil.h>
#include <qbitmap.h>
#include <cmath>
#include "page.h"
#include "config.h"
#ifdef HAVE_FREETYPE
	#include <ft2build.h>
	#include FT_GLYPH_H
#endif
extern float Cwidth(ScribusDoc *doc, QPainter *p, QString name, QString ch, int Siz, QString ch2 = " ");
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern float xy2Deg(float x, float y);
extern void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);

PageItem::PageItem(Page *pa, int art, float x, float y, float w, float h, float w2, QString fill, QString outline, ScribusDoc *doc) : QObject(pa)
{
	QString tmp;
	BackBox = 0;
	NextBox = 0;
	Locked = false;
	Xpos = x;
	Ypos = y;
	Width = w;
	Height = h;
	OldB = Width;
	OldH = Height;
	OldB2 = Width;
	OldH2 = Height;
	PType = art;
	Rot = 0;
	Parent = pa;
	Doc = doc;
	Pcolor = fill;
	Pcolor2 = outline;
	Shade = 100;
	Shade2 = 100;
	GrColor = "";
	GrColor2 = "";
	GrShade = 100;
	GrShade2 = 100;
	GrType = 0;
	Pwidth = w2;
	PLineArt = Doc->DLineArt;
	PLineEnd = FlatCap;
	PLineJoin = MiterJoin;
	Select = false;
	FrameOnly = false;
	ClipEdited = false;
	FrameType = 0;
	QFont ffo;
	QPainter p;
	p.begin(Parent);
	QPaintDeviceMetrics pm(Parent);
	DevRes = 75.0 / static_cast<float>(pm.logicalDpiX());
	ffo = Doc->UsedFonts[Doc->Dfont];
	ffo.setPointSize(static_cast<int>(Doc->Dsize * DevRes));
	DevResX = pm.logicalDpiX();
	DevResY = pm.logicalDpiY();
	p.setFont(ffo);
	IFont = Doc->Dfont;
	ISize = Doc->Dsize;
	LineSp = (Doc->Dsize * static_cast<float>(Doc->AutoLine) / 100) + Doc->Dsize;
	Doc->Vorlagen[0].LineSpa = LineSp;
	p.end();
	CurX = 0;
	CurY = 0;
	CPos = 0;
	Extra = 1;
	TExtra = 1;
	BExtra = 1;
	RExtra = 1;
	ExtraV = 0;
	Ptext.clear();
	Ptext.setAutoDelete(true);
	Pfile = "";
	pixm = QImage();
	Pfile2 = "";
	Pfile3 = "";
	LocalScX = 1;
	LocalScY = 1;
	LocalViewX = 1;
	LocalViewY = 1;
	OrigW = 0;
	OrigH = 0;
	LocalX = 0;
	LocalY = 0;
	flippedH = 0;
	flippedV = 0;
	BBoxX = 0;
	BBoxH = 0;
	RadRect = 0;
	if ((art == 4) || (art == 2))
		Frame = true;
	else
		Frame = false;
	switch (art)
		{
		case 6:
			Clip.setPoints(4, static_cast<int>(w/2),0, static_cast<int>(w),static_cast<int>(h/2), static_cast<int>(w/2),static_cast<int>(h), 0,static_cast<int>(h/2));
			break;
		default:
			Clip.setPoints(4, 0,0, static_cast<int>(w),0, static_cast<int>(w),static_cast<int>(h), 0,static_cast<int>(h));
			break;
		}
	PoLine.resize(0);
	Segments.clear();
	PoShow = false;
	BaseOffs = 0;
	OwnPage = pa;
	PicArt = true;
	PicAvail = false;
	isPrintable = true;
	isBookmark = false;
  BMnr = 0;
	isAnnotation = false;
	AnType = 0;
	AnActType = 0;
	AnBwid = 1;
	AnAction = "";
	An_E_act = "";
	An_X_act = "";
	An_D_act = "";
	An_Fo_act = "";
	An_Bl_act = "";
	An_K_act = "";
	An_F_act = "";
	An_V_act = "";
	An_C_act = "";
	An_Extern = "";
	AnName = "Item"+tmp.setNum(Doc->TotalItems);
	Doc->TotalItems++;
	AnToolTip = "";
	AnRollOver = "";
	AnDown = "";
	AnBsty = 0;
	AnFeed = 1;
	AnFlag = 0;
	AnZiel = 0;
	AnVis = 0;
	AnChkStil = 0;
	AnFormat = 0;
	AnFont = 4;
	AnIsChk = false;
	AnAAact = false;
	AnHTML = false;
	AnUseIcons = false;
	AnIPlace = 1;
	AnScaleW = 0;
	AnMaxChar = -1;
	AnBColor = outline;
	HasSel = false;
	Textflow = true;
	Textflow2 = false;
	Tinput = false;
	isAutoText = false;
	Ausrich = 0;
	Redrawn = false;
	isRaster = false;
	Sizing = false;
	toPixmap = false;
	UseEmbedded = true;
	EmProfile = "";
	Groups.clear();
	LayerNr = Doc->ActiveLayer;
	ScaleType = true;
	AspectRatio = true;
	Transparency = 0.0;
	Reverse = false;
	InvPict = false;
}

/** Zeichnet das Item */
void PageItem::paintObj(QRect e, QPixmap *ppX)
{
	QPainter p, pp;
	QPoint pt1, pt2;
	QRegion cm;
	QFont ffo = Doc->UsedFonts[IFont];
	QPointArray Bez(4);
	QPointArray cl;
	uint a, nrc, nrc2, zae;
	int desc, asce, xp, yp, leng, absa, aSpa, chs;
	uint BuPos, LastSP, BuPos2;
	float oldCurY, LastXp, EndX, OFs, OFs2, wide, rota, wid;
	bool outs = false;
	PageItem *nb;
	QColor tmp;
	QString chx;
	struct Pti *hl;
	struct ZZ *Zli;
	struct ZZ *Zli2;
	QPtrList<ZZ> LiList;
#if QT_VERSION  >= 0x030100
	QBitmap bmd;
	QPixmap pmd;
	QPainter pb, pd;
#endif
	LiList.setAutoDelete(true);
	for (int xxx=0; xxx<5; ++xxx)
		{
		Doc->Vorlagen[xxx].LineSpa = LineSp;
		Doc->Vorlagen[xxx].Indent = 0;
		Doc->Vorlagen[xxx].First = 0;
		Doc->Vorlagen[xxx].Avor = 0;
		Doc->Vorlagen[xxx].Anach = 0;
		}
	Doc->Vorlagen[0].Ausri = Ausrich;
	if (toPixmap)
		p.begin(ppX);
	else
		p.begin(Parent);
	QPaintDeviceMetrics pm(Parent);
	DevRes = 75.0 / float(pm.logicalDpiX());
	if ((!toPixmap) && (!Doc->RePos))
		{
		if (!e.isEmpty())
			p.setClipRect(e);
		else
			p.setClipRect(OwnPage->ViewReg().boundingRect());
		}
	QRegion gesClip = p.clipRegion();
	p.translate(static_cast<int>(Xpos*Doc->Scale), static_cast<int>(Ypos*Doc->Scale));
	p.scale(static_cast<double>(Doc->Scale), static_cast<double>(Doc->Scale));
	p.rotate(static_cast<double>(Rot));
	if (Pcolor != "None")
		{
		SetFarbe(&tmp, Pcolor, Shade);
		p.setBrush(tmp);
		p.setBackgroundColor(tmp);
		}
	else
		p.setBrush(NoBrush);
	if (Pcolor2 != "None")
		{
		SetFarbe(&tmp, Pcolor2, Shade2);
		if ((Pwidth == 0) && (PType != 5))
			p.setPen(NoPen);
		else
			p.setPen(QPen(tmp, QMAX(static_cast<int>(Pwidth*Doc->Scale), 1), PLineArt, PLineEnd, PLineJoin));
		}
	else
		p.setPen(NoPen);
	if (!FrameOnly)
	{
	if (!((Doc->ActPage->Imoved) && (Select)))
		{
		switch (PType)
			{
			case 2:
#if QT_VERSION  >= 0x030100
				if (toPixmap)
					p.setClipRegion(QRegion(p.xForm(QRect(0, 0, static_cast<int>(Width), static_cast<int>(Height)))));
				else
					p.setClipRegion(QRegion(p.xForm(QRect(0, 0, static_cast<int>(Width), static_cast<int>(Height)))).intersect(gesClip));
				bmd = QBitmap(static_cast<int>(Width), static_cast<int>(Height));
				bmd.fill(Qt::color0);
				pb.begin(&bmd);
				pb.setBrush(Qt::color1);
				pb.setPen(QPen(Qt::color1, 1, DotLine, FlatCap, MiterJoin));
				DrawPoly(&pb, Clip, pb.brush().color(), true);
				pb.end();
				pmd = QPixmap(static_cast<int>(Width), static_cast<int>(Height));
				pmd.fill();
				pd.begin(&pmd);
				if (Pcolor != "None")
					{
					pd.setPen(NoPen);
					SetFarbe(&tmp, Pcolor, Shade);
					pd.setBrush(tmp);
					if (!Doc->RePos)
						DrawPoly(&pd, Clip, pd.brush().color());
//						pd.drawPolygon(Clip);
					}
				if (Pfile == "")
					{
					pd.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
					if (!Doc->RePos)
						{
						pd.drawLine(0, 0, static_cast<int>(Width), static_cast<int>(Height));
						pd.drawLine(0, static_cast<int>(Height), static_cast<int>(Width), 0);
						}
					pd.end();						
					}
				else
					{
					if (!Doc->RePos)
						{
						if ((!PicArt) || (!PicAvail))
							{
							pd.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
							pd.drawLine(0, 0, static_cast<int>(Width), static_cast<int>(Height));
							pd.drawLine(0, static_cast<int>(Height), static_cast<int>(Width), 0);
							if (PicAvail)
								pd.setPen(blue);
							else
								pd.setPen(red);
							pd.save();
							pd.setBackgroundMode(OpaqueMode);
							pd.setFont(QFont("Helvetica", 18));
							QFileInfo fi = QFileInfo(Pfile);
							leng = p.fontMetrics().width(fi.fileName());
							xp = static_cast<int>(Width / 2 - leng / 2);
							yp = static_cast<int>(Height / 2 + p.fontMetrics().height() / 2);
							pd.drawText(xp, yp, fi.fileName());
							pd.restore();
							pd.end();
							}
						else
							{
							pd.save();
							if (flippedH % 2 != 0)
								{
								pd.translate(Width, 0);
								pd.scale(-1, 1);
								}
							if (flippedV % 2 != 0)
								{
								pd.translate(0, static_cast<int>(Height));
								pd.scale(1, -1);
								}

							if ((LocalViewX != 1) || (LocalViewY != 1))
								pd.scale(LocalViewX, LocalViewY);
							if (InvPict)
								{
								QImage ip = pixm.copy();
								ip.invertPixels();
								pd.drawImage(static_cast<int>(LocalX*LocalScX), static_cast<int>(LocalY*LocalScY), ip);
								}
							else
								pd.drawImage(static_cast<int>(LocalX*LocalScX), static_cast<int>(LocalY*LocalScY), pixm);
							pd.restore();
							pd.end();
							}
						}
					}
				pmd.setMask(bmd);
				p.drawPixmap(0, 0, pmd);
#else
				if (toPixmap)
					p.setClipRegion(QRegion(p.xForm(Clip)));
				else
					p.setClipRegion(QRegion(p.xForm(Clip)).intersect(gesClip));
				if (Pcolor != "None")
					{
					p.setPen(NoPen);
					if (!Doc->RePos)
						DrawPoly(&p, Clip, p.brush().color());
					}
				if (Pfile == "")
					{
					p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
					if (!Doc->RePos)
						{
						p.drawLine(0, 0, static_cast<int>(Width), static_cast<int>(Height));
						p.drawLine(0, static_cast<int>(Height), static_cast<int>(Width), 0);
						}
					}
				else
					{
					if (!Doc->RePos)
						{
						if ((!PicArt) || (!PicAvail))
							{
							p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
							p.drawLine(0, 0, static_cast<int>(Width), static_cast<int>(Height));
							p.drawLine(0, static_cast<int>(Height), static_cast<int>(Width), 0);
							if (PicAvail)
								p.setPen(blue);
							else
								p.setPen(red);
							p.save();
							p.setBackgroundMode(OpaqueMode);
							p.setFont(QFont("Helvetica", 18));
							QFileInfo fi = QFileInfo(Pfile);
							leng = p.fontMetrics().width(fi.fileName());
							xp = static_cast<int>(Width / 2 - leng / 2);
							yp = static_cast<int>(Height / 2 + p.fontMetrics().height() / 2);
							p.drawText(xp, yp, fi.fileName());
							p.restore();
							}
						else
							{
							p.save();
							if (flippedH % 2 != 0)
								{
								p.translate(Width, 0);
								p.scale(-1, 1);
								}
							if (flippedV % 2 != 0)
								{
								p.translate(0, static_cast<int>(Height));
								p.scale(1, -1);
								}
							if ((LocalViewX != 1) || (LocalViewY != 1))
								p.scale(LocalViewX, LocalViewY);
							if (InvPict)
								{
								QImage ip = pixm.copy();
								ip.invertPixels();
								p.drawImage(static_cast<int>(LocalX*LocalScX), static_cast<int>(LocalY*LocalScY), ip);
								}
							else
							p.drawImage(static_cast<int>(LocalX*LocalScX), static_cast<int>(LocalY*LocalScY), pixm);
							p.restore();
							}
						}
					}
#endif
				if (Pcolor2 != "None")
					{
					p.setBrush(NoBrush);
					DrawPolyL(&p, Clip);
					}
				break;
			case 4:
				p.save();
#if QT_VERSION  >= 0x030100
				if (toPixmap)
					p.setClipRegion(QRegion(p.xForm(QRect(0, 0, static_cast<int>(Width), static_cast<int>(Height)))));
				else
					p.setClipRegion(QRegion(p.xForm(QRect(0, 0, static_cast<int>(Width), static_cast<int>(Height)))).intersect(gesClip));
				if (PoLine.size() > 16)
					{
					bmd = QBitmap(static_cast<int>(Width), static_cast<int>(Height));
					bmd.fill(Qt::color0);
					pb.begin(&bmd);
					pb.setBrush(Qt::color1);
					pb.setPen(QPen(Qt::color1, 1, DotLine, FlatCap, MiterJoin));
					DrawPoly(&pb, Clip, pb.brush().color(), true);
					pb.end();
					pmd = QPixmap(static_cast<int>(Width), static_cast<int>(Height));
					pmd.fill();
					pd.begin(&pmd);
					if (!Doc->RePos)
						{
						if (GrType == 0)
							{
							if (Pcolor != "None")
								{
								pd.setPen(NoPen);
								SetFarbe(&tmp, Pcolor, Shade);
								pd.setBrush(tmp);
								DrawPoly(&pd, Clip, pd.brush().color());
								}
							}
						else
							{
							if (!pixm.isNull())
								pd.drawImage(0, 0, pixm);
							}
						}
					pd.end();
					pmd.setMask(bmd);
					p.drawPixmap(0, 0, pmd);
					}
				else
					{
					if ((toPixmap) || (Doc->RePos))
						p.setClipRegion(QRegion(p.xForm(QRect(0, 0, static_cast<int>(Width), static_cast<int>(Height)))));
//						p.setClipRegion(QRegion(p.xForm(Clip)));
					else
						p.setClipRegion(QRegion(p.xForm(Clip)).intersect(gesClip));
					if (!Doc->RePos)
						{
						if (GrType == 0)
							{
							if (Pcolor != "None")
								{
								p.setPen(NoPen);
								DrawPoly(&p, Clip, p.brush().color());
								}
							}
						else
							{
							if (!pixm.isNull())
								p.drawImage(0, 0, pixm);
							}
						}
					}
#else
				if ((toPixmap) || (Doc->RePos))
					p.setClipRegion(QRegion(p.xForm(Clip)));
				else
					p.setClipRegion(QRegion(p.xForm(Clip)).intersect(gesClip));
				if (!Doc->RePos)
					{
					if (GrType == 0)
						{
						if (Pcolor != "None")
							{
							p.setPen(NoPen);
							DrawPoly(&p, Clip, p.brush().color());
							}
						}
					else
						{
						if (!pixm.isNull())
							p.drawImage(0, 0, pixm);
						}
					}
#endif
				if ((isAnnotation) && (AnType == 2))
					{
					p.save();
    			QColorGroup cg;
    			cg.setColor( QColorGroup::Foreground, black );
    			cg.setColor( QColorGroup::Button, QColor( 211, 211, 211) );
    			cg.setColor( QColorGroup::Light, white );
    			cg.setColor( QColorGroup::Midlight, QColor( 233, 233, 233) );
    			cg.setColor( QColorGroup::Dark, QColor( 105, 105, 105) );
    			cg.setColor( QColorGroup::Mid, QColor( 140, 140, 140) );
    			cg.setColor( QColorGroup::Text, black );
    			cg.setColor( QColorGroup::BrightText, QColor( 236, 236, 236) );
    			cg.setColor( QColorGroup::ButtonText, black );
    			cg.setColor( QColorGroup::Base, white );
    			cg.setColor( QColorGroup::Background, white );
    			cg.setColor( QColorGroup::Shadow, black );
    			cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    			cg.setColor( QColorGroup::HighlightedText, white );
					bool bsun = false;
					if (AnBsty == 4)
						bsun = true;
					QBrush qb = p.brush();
					if (!Doc->RePos)
						qDrawWinButton(&p, 1, 1, static_cast<int>(Width), static_cast<int>(Height), cg, bsun, &qb);
					p.restore();
					}
				if ((isAnnotation) && (AnType == 2) && (Pfile != "") && (PicAvail) && (PicArt) && (AnUseIcons))
					{
					p.save();
					p.scale(LocalScX, LocalScY);
					if (!Doc->RePos)
						p.drawImage(static_cast<int>(LocalX), static_cast<int>(LocalY), pixm);
					p.restore();
					}
				if ((Ptext.count() != 0) || (Dirty) || (NextBox != 0))
					{
					if (NextBox != 0)
						{
						nb = NextBox;
						while (nb != 0)
							{
							a = nb->Ptext.count();
							for (uint s=0; s<a; ++s)
								{
								Ptext.append(nb->Ptext.take(0));
								}
							nb->CPos = 0;
							nb->Dirty = true;
							nb = nb->NextBox;
							}
						nb = NextBox;
						}
					Doc->Vorlagen[0].LineSpa = LineSp;
					QRegion cl = QRegion(p.xForm(Clip));
//					if ((OwnPage->Items.count()-1) > ItemNr)
//						{
						for (a=0; a<OwnPage->Items.count(); ++a)
							{
 							if (((OwnPage->Items.at(a)->ItemNr > ItemNr)
     								&& (OwnPage->Items.at(a)->LayerNr == LayerNr))
      							|| (Doc->Layers[OwnPage->Items.at(a)->LayerNr].Level > Doc->Layers[LayerNr].Level))
								{
								if (OwnPage->Items.at(a)->Textflow)
									{
									pp.begin(Parent);
									pp.translate(OwnPage->Items.at(a)->Xpos*Doc->Scale, OwnPage->Items.at(a)->Ypos*Doc->Scale);
									pp.scale(Doc->Scale, Doc->Scale);
									pp.rotate(OwnPage->Items.at(a)->Rot);
									if (OwnPage->Items.at(a)->Textflow2)
										{
										QPointArray tcli;
										tcli.resize(4);
										tcli.setPoint(0, QPoint(0,0));
										tcli.setPoint(1, QPoint(qRound(OwnPage->Items.at(a)->Width), 0));
										tcli.setPoint(2, QPoint(qRound(OwnPage->Items.at(a)->Width), qRound(OwnPage->Items.at(a)->Height)));
										tcli.setPoint(3, QPoint(0, qRound(OwnPage->Items.at(a)->Height)));
										cm = QRegion(pp.xForm(tcli));
										}
									else
										cm = QRegion(pp.xForm(OwnPage->Items.at(a)->Clip));
									pp.end();
									cl = cl.subtract(cm);
									}
								}
							}
					if (flippedH % 2 != 0)
						{
						p.translate(static_cast<int>(Width), 0);
						p.scale(-1, 1);
						}
					if (flippedV % 2 != 0)
						{
						p.translate(0, static_cast<int>(Height));
						p.scale(1, -1);
						}
					if ((Doc->AppMode == 7) && (Dirty))
						{
						if ((CPos < static_cast<int>(Ptext.count())) && (CPos > 2) && (Tinput))
							{
							hl = Ptext.at(CPos-2);
							if (!Doc->RePos)
								p.eraseRect(0, static_cast<int>(hl->yp-LineSp), static_cast<int>(Width), static_cast<int>(Height-hl->yp));
							}
						else
							{
							if (!Doc->RePos)
								p.eraseRect(0, 0, static_cast<int>(Width), static_cast<int>(Height));
							}
						Dirty = false;
						}
					CurX = Extra;
					CurY = Doc->Vorlagen[0].LineSpa+TExtra;
					LiList.clear();
					BuPos = 0;
					BuPos2 = 0;
					LastSP = 0;
					LastXp = 0;
					outs = false;
					OFs = 0;
					OFs2 = 0;
					aSpa = 0;
					absa = 0;
					for (a = 0; a < Ptext.count(); ++a)
						{
						hl = Ptext.at(a);
						chx = hl->ch;
						if (hl->ch == QChar(30))
							chx = ExpandToken(a);
						absa = hl->cab;
						if (a == 0)
							{
							if (BackBox != 0)
								{
								nb = BackBox;
								while (nb != 0)
									{
									if (nb->Ptext.count() != 0)
										{
										if (nb->Ptext.at(nb->Ptext.count()-1)->ch == QChar(13))
											{
											CurX += Doc->Vorlagen[absa].First;
											CurX += Doc->Vorlagen[absa].Indent;
											CurY += Doc->Vorlagen[absa].Avor;
											break;
											}
										else
											{
											CurX += Doc->Vorlagen[absa].Indent;
											break;
											}
										nb = nb->BackBox;
										}
									}
								}
							else
								{
								CurX += Doc->Vorlagen[absa].First;
								CurX += Doc->Vorlagen[absa].Indent;
								CurY += Doc->Vorlagen[absa].Avor;
								}
							}
						if ((Doc->Vorlagen[0].LineSpa != Doc->Vorlagen[absa].LineSpa) && (a == 0))
							{
//							CurY += Doc->Vorlagen[absa].LineSpa - Doc->Vorlagen[0].LineSpa;
							CurY += Doc->Vorlagen[absa].Avor;
							}
						if (hl->ccolor != "None")
							{
							SetFarbe(&tmp, hl->ccolor, hl->cshade);
							p.setPen(tmp);
							}
						else
							p.setPen(NoPen);
						if (LiList.count() == 0)
							{
							CurY += Doc->Vorlagen[absa].LineSpa - Doc->Vorlagen[0].LineSpa;
							if ((a > 0) && (Ptext.at(a-1)->ch == QChar(13)))
								{
								CurX += Doc->Vorlagen[hl->cab].First;
								CurX += Doc->Vorlagen[hl->cab].Indent;
								CurY += Doc->Vorlagen[hl->cab].Avor;
								}
							}
						oldCurY = CurY;
						chs = hl->csize;
						SetZeichAttr(&p, &ffo, hl, &chs, &chx);
						if (a < Ptext.count()-1)
							wide = Cwidth(Doc, &p, hl->cfont, chx, chs, Ptext.at(a+1)->ch);
						else
							wide = Cwidth(Doc, &p, hl->cfont, chx, chs);
						desc = p.fontMetrics().descent();
						asce = p.fontMetrics().ascent();
						if (LiList.isEmpty())
							{
							pt1 = QPoint(static_cast<int>(CurX), static_cast<int>(CurY+desc+BExtra));
							pt2 = QPoint(static_cast<int>(CurX), static_cast<int>(CurY-asce));
							while ((!cl.contains(p.xForm(pt1))) || (!cl.contains(p.xForm(pt2))))
								{
								CurX++;
								pt1 = QPoint(static_cast<int>(CurX), static_cast<int>(CurY+desc));
								pt2 = QPoint(static_cast<int>(CurX), static_cast<int>(CurY-asce));
								if (CurX+RExtra > Width)
									{
									CurY += Doc->Vorlagen[hl->cab].LineSpa;
									CurY += Doc->Vorlagen[hl->cab].Anach;
									CurX = Extra;
									if (CurY+BExtra > Height)
										{
										nrc = a;
										goto NoRoom;
										}
									}
								}
								CurX += Extra;
							}
						hl->xp = CurX+hl->cextra;
						hl->yp = CurY;
						CurY = oldCurY;
						CurX += wide+hl->cextra;
						pt1 = QPoint(static_cast<int>(CurX+RExtra), static_cast<int>(CurY+desc+BExtra));
						pt2 = QPoint(static_cast<int>(CurX+RExtra), static_cast<int>(CurY-asce));
						if ((!cl.contains(p.xForm(pt1))) || (!cl.contains(p.xForm(pt2))))
							outs = true;
						Zli = new ZZ;
						Zli->Zeich = chx;
						Zli->Farb = tmp;
						Zli->Zsatz = p.font();
						Zli->xco = hl->xp;
						Zli->yco = hl->yp;
						Zli->Sele = hl->cselect;
						Zli->Siz = chs;
//						Zli->Siz = hl->csize;
						Zli->ZFo = hl->cfont;
						Zli->wide = wide;
						if ((hl->ch == " ") && (!outs))
							{
							LastXp = hl->xp;
							LastSP = BuPos;
							}
						if ((hl->ch == "-") && (!outs))
							{
							LastXp = CurX;
							LastSP = BuPos;
							}
						if ((hl->cstyle & 128) && (!outs))
							{
							LastXp = CurX + Cwidth(Doc, &p, hl->cfont, "-", hl->csize);
							LastSP = BuPos;
							}
						LiList.append(Zli);
						BuPos++;
						if ((hl->ch == QChar(13)) || (outs))
							{
							if (outs)
								{
								if (LastSP != 0)            // Hier können auch andere Trennungen eingebaut werden
									{
									a -= BuPos - LastSP;
									a++;
									if (Ptext.at(a)->cstyle & 128)
										{
										Zli = new ZZ;
										Zli->Zeich = "-";
										Zli->Farb = tmp;
										Zli->Zsatz = p.font();
										Zli->xco = LastXp - Cwidth(Doc, &p, Ptext.at(a)->cfont, "-", Ptext.at(a)->csize);
										Zli->yco = Ptext.at(a)->yp;
										Zli->Sele = Ptext.at(a)->cselect;
										Zli->Siz = Ptext.at(a)->csize;
										Zli->ZFo = Ptext.at(a)->cfont;
										Zli->wide = Cwidth(Doc, &p, Ptext.at(a)->cfont, "-", Ptext.at(a)->csize);
										LiList.insert(LastSP+1, Zli);
										LastSP += 1;
										}
									BuPos = LastSP+1;
									if (Tinput)
										{
										if (!Doc->RePos)
											{
											if (Doc->Vorlagen[absa].Ausri == 0)
												p.eraseRect(static_cast<int>(LastXp), static_cast<int>(CurY-asce), static_cast<int>(Width-LastXp), asce+desc+1);
											else
												p.eraseRect(0, static_cast<int>(CurY-asce), static_cast<int>(Width), asce+desc+1);
											}
										}
									if (Doc->Vorlagen[absa].Ausri != 0)
										{
										EndX = LastXp;
										do
										{
											pt1 = QPoint(static_cast<int>(EndX+RExtra), static_cast<int>(CurY+desc));
											pt2 = QPoint(static_cast<int>(EndX+RExtra), static_cast<int>(CurY-asce));
											EndX++;
										}
										while ((cl.contains(p.xForm(pt1))) && (cl.contains(p.xForm(pt2))));
										if (Doc->Vorlagen[absa].Ausri == 2)
											OFs = EndX - LastXp;
										if (Doc->Vorlagen[absa].Ausri == 1)
											OFs = (EndX - LastXp) / 2;
										if ((Doc->Vorlagen[absa].Ausri == 3) || (Doc->Vorlagen[absa].Ausri == 4))
											{
											aSpa = 0;
											for (uint sof = 0; sof<BuPos-1; ++sof)
												{
												if (LiList.at(sof)->Zeich == QChar(32))
													aSpa++;
												}
											if (aSpa > 1)
												{
												OFs2 = (EndX - LastXp) / aSpa;
												}
											else
												{
												if (aSpa == 0)
													OFs2 = 0;
												else
													OFs2 = (EndX - LastXp);
												}
											OFs = 0;
											for (uint yof = 0; yof < LiList.count(); ++yof)
												{
												LiList.at(yof)->xco += OFs;
												Ptext.at(BuPos2)->xp += OFs;
												BuPos2++;
												if (LiList.at(yof)->Zeich == QChar(32))
													OFs += OFs2;										
												}
											}
										else
											{
											for (uint xof = 0; xof<LiList.count(); ++xof)
												{
												LiList.at(xof)->xco += OFs;
												Ptext.at(BuPos2)->xp += OFs;
												BuPos2++;
												}
											}
										}
									}
								else
									{
									a--;
									BuPos--;
									}
								}
							else
								{
								if (Doc->Vorlagen[absa].Ausri != 0)
									{
									EndX = CurX;
									do
									{
										pt1 = QPoint(static_cast<int>(EndX+RExtra), static_cast<int>(CurY+desc));
										pt2 = QPoint(static_cast<int>(EndX+RExtra), static_cast<int>(CurY-asce));
										EndX++;
									}
									while ((cl.contains(p.xForm(pt1))) && (cl.contains(p.xForm(pt2))));
									if (Doc->Vorlagen[absa].Ausri == 2)
										OFs = EndX - CurX - Extra;
									if (Doc->Vorlagen[absa].Ausri == 1)
										OFs = (EndX - CurX - Extra) / 2;
									if (Doc->Vorlagen[absa].Ausri == 3)
										OFs = 0;
									if (Doc->Vorlagen[absa].Ausri == 4)
										{
										aSpa = 0;
										for (uint sof = 0; sof<LiList.count(); ++sof)
											{
											if (LiList.at(sof)->Zeich == QChar(32))
												aSpa++;
											}
										if (aSpa != 0)
											{
											OFs2 = (EndX - CurX - Extra) / aSpa;
											}
										else
											OFs2 = 0;
										OFs = 0;
										for (uint yof = 0; yof < LiList.count(); ++yof)
											{
											LiList.at(yof)->xco += OFs;
											Ptext.at(BuPos2)->xp += OFs;
											BuPos2++;
											if (LiList.at(yof)->Zeich == QChar(32))
												OFs += OFs2;										
											}
										}
									else
										{
										for (uint xof = 0; xof<LiList.count(); ++xof)
											{
											LiList.at(xof)->xco += OFs;
											Ptext.at(BuPos2)->xp += OFs;
											BuPos2++;
											}
										}
									}							
								}
							BuPos2 = a;
							BuPos2++;
							uint BuPos3 = BuPos;
							CurY += Doc->Vorlagen[0].LineSpa;
							CurX = Extra;
							if (hl->ch != QChar(13))
								CurX += Doc->Vorlagen[hl->cab].Indent;
							else
								{
								CurY += Doc->Vorlagen[hl->cab].Anach;
								if (BuPos3 > 0)
									BuPos3 -= 1;
								}
							hl->xp = CurX;
							hl->yp = CurY;
							LiList.at(LiList.count()-1)->xco = hl->xp;
							LiList.at(LiList.count()-1)->yco = hl->yp;
							for (uint zc = 0; zc<BuPos3; ++zc)
								{
								Zli2 = LiList.at(zc);
								p.setFont(Zli2->Zsatz);
								p.setPen(Zli2->Farb);
								if ((Zli2->Sele) && (Doc->AppMode == 7) && (Select))
									{
									p.save();
									wide = Zli2->wide;
									desc = p.fontMetrics().descent();
									asce = p.fontMetrics().ascent();
									p.setPen(NoPen);
            			p.setBrush(darkBlue);
									if (!Doc->RePos)
            				p.drawRect(static_cast<int>(Zli2->xco), static_cast<int>(Zli2->yco-asce), static_cast<int>(wide+1), asce+desc);
									p.restore();
									p.setPen(white);
									}
								else
									{
									if (Doc->AppMode == 7)
										{
										p.save();
										wide = Zli2->wide;
										desc = p.fontMetrics().descent();
										asce = p.fontMetrics().ascent();
										if (!Doc->RePos)
            					p.eraseRect(static_cast<int>(Zli2->xco), static_cast<int>(Zli2->yco-asce), static_cast<int>(wide+1), asce+desc);
										p.restore();
										}
									}
								if (!Doc->RePos)
									DrawZeichen(&p, Zli2);
								}
							LiList.clear();
							BuPos = 0;
							LastSP = 0;
							LastXp = 0;
							outs = false;
							}
						}
						if (Doc->Vorlagen[absa].Ausri != 0)
							{
							EndX = CurX;
							desc = p.fontMetrics().descent();
							asce = p.fontMetrics().ascent();
							if (Tinput)
								p.eraseRect(0, static_cast<int>(CurY-asce), static_cast<int>(Width), asce+desc+1);
							do
							{
								pt1 = QPoint(static_cast<int>(EndX+RExtra), static_cast<int>(CurY+desc));
								pt2 = QPoint(static_cast<int>(EndX+RExtra), static_cast<int>(CurY-asce));
								EndX++;
							}
							while ((cl.contains(p.xForm(pt1))) && (cl.contains(p.xForm(pt2))));
							if (Doc->Vorlagen[absa].Ausri == 2)
								OFs = EndX - CurX - Extra;
							if (Doc->Vorlagen[absa].Ausri == 1)
								OFs = (EndX - CurX - Extra) / 2;
							if (Doc->Vorlagen[absa].Ausri == 3)
								OFs = 0;
							if (Doc->Vorlagen[absa].Ausri == 4)
								{
								aSpa = 0;
								for (uint sof = 0; sof<LiList.count(); ++sof)
									{
									if (LiList.at(sof)->Zeich == QChar(32))
										aSpa++;
									}
								if (aSpa != 0)
									{
									OFs2 = (EndX - CurX - Extra) / aSpa;
									}
								else
									OFs2 = 0;
								OFs = 0;
								for (uint yof = 0; yof < LiList.count(); ++yof)
									{
									LiList.at(yof)->xco += OFs;
									Ptext.at(BuPos2)->xp += OFs;
									BuPos2++;
									if (LiList.at(yof)->Zeich == QChar(32))
										OFs += OFs2;										
									}
								}
							else
								{
								for (uint xof = 0; xof<LiList.count(); ++xof)
									{
										LiList.at(xof)->xco += OFs;
										Ptext.at(BuPos2)->xp += OFs;
										BuPos2++;
									}
								}
							}
						for (uint zc = 0; zc<LiList.count(); ++zc)
							{
							Zli2 = LiList.at(zc);
							p.setFont(Zli2->Zsatz);
							p.setPen(Zli2->Farb);
							if ((Zli2->Sele) && (Doc->AppMode == 7) && (Select))
								{
								p.save();
								wide = Zli2->wide;
								desc = p.fontMetrics().descent();
								asce = p.fontMetrics().ascent();
								p.setPen(NoPen);
            		p.setBrush(darkBlue);
								if (!Doc->RePos)
            			p.drawRect(static_cast<int>(Zli2->xco), static_cast<int>(Zli2->yco-asce), static_cast<int>(wide+1), asce+desc);
								p.restore();
								p.setPen(white);
								}
							else
								{
								if (Doc->AppMode == 7)
									{
									p.save();
									wide = Zli2->wide;
									desc = p.fontMetrics().descent();
									asce = p.fontMetrics().ascent();
									if (!Doc->RePos)
           					p.eraseRect(static_cast<int>(Zli2->xco), static_cast<int>(Zli2->yco-asce), static_cast<int>(wide+1), asce+desc);
									p.restore();
									}
								}
							if (!Doc->RePos)
								DrawZeichen(&p, Zli2);
							}
						LiList.clear();
						BuPos = 0;
						LastSP = 0;
						outs = false;
					} 
					MaxChars = Ptext.count();
					Redrawn = true;
					p.restore();
					break;
NoRoom:	 if (NextBox != 0)
						{
						nrc2 = Ptext.count();
						for (uint ss=nrc; ss<nrc2; ++ss)
							{
							NextBox->Ptext.append(Ptext.take(nrc));
							}
						NextBox->Dirty = true;
						if (uint(CPos) > nrc)
							{
							CPos = nrc;
							if ((Doc->AppMode == 7) && (Tinput))
								{
								OwnPage->Deselect(true);
								NextBox->CPos = 1;
								Doc->ActPage = NextBox->OwnPage;
								NextBox->OwnPage->SelectItemNr(NextBox->ItemNr);
								break;
								}
							}
						NextBox->paintObj();
						}
					else
						{
//						if (uint(CPos) > nrc)
//							CPos = nrc;
						if (!Doc->RePos)
							{
							p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
							p.setBrush(white);
							p.drawRect(static_cast<int>(Width-16), static_cast<int>(Height-16), 14, 14);
							p.drawLine(static_cast<int>(Width-16), static_cast<int>(Height-16), static_cast<int>(Width-3), static_cast<int>(Height-3));
							p.drawLine(static_cast<int>(Width-16), static_cast<int>(Height-3), static_cast<int>(Width-3), static_cast<int>(Height-16));
							}
						}
					MaxChars = nrc;
					Redrawn = true;
					p.restore();
					break;
			case 5:
				if (!Doc->RePos)
					p.drawLine(0, 0, qRound(Width), 0);
				break;
			case 1:
			case 3:
			case 6:
				if (GrType == 0)
					{
					if (!Doc->RePos)
						DrawPoly(&p, Clip, p.brush().color());
					}
				else
					{
					if (!Doc->RePos)
						{
#if QT_VERSION  >= 0x030100
						if (toPixmap)
							p.setClipRegion(QRegion(p.xForm(QRect(0, 0, static_cast<int>(Width), static_cast<int>(Height)))));
						else
							p.setClipRegion(QRegion(p.xForm(QRect(0, 0, static_cast<int>(Width), static_cast<int>(Height)))).intersect(gesClip));
						bmd = QBitmap(static_cast<int>(Width), static_cast<int>(Height));
						bmd.fill(Qt::color0);
						pb.begin(&bmd);
						pb.setBrush(Qt::color1);
						pb.setPen(QPen(Qt::color1, 1, DotLine, FlatCap, MiterJoin));
						DrawPoly(&pb, Clip, pb.brush().color(), true);
						pb.end();
						pmd = QPixmap(static_cast<int>(Width), static_cast<int>(Height));
						pmd.fill();
						pd.begin(&pmd);
						if ((!Doc->RePos) && (!pixm.isNull()))
							pd.drawImage(0, 0, pixm);
						pd.end();
						pmd.setMask(bmd);
						p.drawPixmap(0, 0, pmd);
#else
						if (toPixmap)
							p.setClipRegion(QRegion(p.xForm(Clip)));
						else
							p.setClipRegion(QRegion(p.xForm(Clip)).intersect(gesClip));
						if (!pixm.isNull())
							p.drawImage(0, 0, pixm);
#endif
						p.setBrush(NoBrush);
						DrawPolyL(&p, Clip);
						}
					}
				break;
			case 7:
				if (PoLine.size() > 3)
					{
					for (uint poi=0; poi<PoLine.size()-3; poi += 4)
						{
						if (PoLine.point(poi).x() > 900000)
							continue;
						BezierPoints(&Bez, PoLine.pointQ(poi), PoLine.pointQ(poi+1), PoLine.pointQ(poi+3), PoLine.pointQ(poi+2));
						if (!Doc->RePos)
							p.drawCubicBezier(Bez);
						}
					}
				break;
			case 8:
				if (PoLine.size() > 3)
					{
					if (PoShow)
						{
						for (uint poi=0; poi<PoLine.size()-3; poi += 4)
							{
							if (PoLine.point(poi).x() > 900000)
								continue;
							BezierPoints(&Bez, PoLine.pointQ(poi), PoLine.pointQ(poi+1), PoLine.pointQ(poi+3), PoLine.pointQ(poi+2));
							if (!Doc->RePos)
								p.drawCubicBezier(Bez);
							}
						}
					}
				cl = FlattenPath(PoLine, Segments);
				CurX = Extra;
				if (Ptext.count() != 0)
					CurX += Ptext.at(0)->cextra;
				zae = 0;
				wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2));
				while (wid < 1)
					{
					zae++;
					if (zae == cl.size()-1)
						goto PfadEnd;
					wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2));
					}
				rota = xy2Deg(cl.point(zae+1).x()-cl.point(zae).x(),cl.point(zae+1).y()-cl.point(zae).y());
				for (a = 0; a < Ptext.count(); ++a)
					{
					CurY = 0;
					hl = Ptext.at(a);
					chx = hl->ch;
					if ((chx == QChar(30)) || (chx == QChar(13)))
						continue;
					if (hl->ccolor != "None")
						{
						SetFarbe(&tmp, hl->ccolor, hl->cshade);
						p.setPen(tmp);
						}
					else
						p.setPen(NoPen);
					chs = hl->csize;
					SetZeichAttr(&p, &ffo, hl, &chs, &chx);
					if (a < Ptext.count()-1)
						wide = Cwidth(Doc, &p, hl->cfont, chx, chs, Ptext.at(a+1)->ch);
					else
						wide = Cwidth(Doc, &p, hl->cfont, chx, chs);
					if ((CurX+(wide+hl->cextra)/2) >= wid)
						{
						if (zae < cl.size()-1)
							{
							CurX = CurX - wid;
							wid = 0;
							EndX = CurX;
							do
								{
								do
									{
									zae++;
									if (zae == cl.size()-1)
										goto PfadEnd;
									wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2));
									rota = xy2Deg(cl.point(zae+1).x()-cl.point(zae).x(),cl.point(zae+1).y()-cl.point(zae).y());
									}
								while (wid == 0);
								EndX -= wid;
								}
							while (wid < EndX);
							CurX = EndX + wid;
							}
						else
							goto PfadEnd;
						}
					p.save();
					p.translate(cl.point(zae).x(), cl.point(zae).y());
					p.rotate(rota);
					hl->xp = CurX+hl->cextra;
					hl->yp = CurY+BaseOffs;
					hl->PtransX = cl.point(zae).x();
					hl->PtransY = cl.point(zae).y();
					hl->PRot = rota;
					if (!Doc->RePos)
						DrawZeichen(&p, hl);
					p.restore();
					CurX += wide+hl->cextra;
					}
PfadEnd:	MaxChars = Ptext.count();
				break;
			}
		}
	}
	p.setClipping(false);
	p.setClipRegion(gesClip);
	if (Sizing)
		{
		p.setRasterOp(XorROP);
		p.setBrush(NoBrush);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.drawRect(0, 0, static_cast<int>(OldB), static_cast<int>(OldH));
		p.drawRect(0, 0, static_cast<int>(Width), static_cast<int>(Height));
		OldB = Width;
		OldH = Height;
		}
	if ((!Tinput) && (!Doc->RePos))
		{
		if ((Frame) && (Doc->ShFrames))
			{
			p.setPen(QPen(black, 1, DotLine, FlatCap, MiterJoin));
			if ((isBookmark) || (isAnnotation))
				p.setPen(QPen(blue, 1, DotLine, FlatCap, MiterJoin));
			if ((BackBox != 0) || (NextBox != 0))
				p.setPen(QPen(red, 1, SolidLine, FlatCap, MiterJoin));
			if (Locked)
				p.setPen(QPen(darkRed, 1, SolidLine, FlatCap, MiterJoin));
			p.setBrush(NoBrush);
			DrawPolyL(&p, Clip);
			}
		if (Select) // && (!Doc->EditClip))
			{
			if (!OwnPage->SelItem.isEmpty())
				{
				if (Groups.count() == 0)
					{
					QPainter pr;
					pr.begin(Parent);
					pr.translate(static_cast<int>(Xpos*Doc->Scale), static_cast<int>(Ypos*Doc->Scale));
					pr.rotate(static_cast<double>(Rot));
					if (Locked)
						pr.setPen(QPen(darkRed, 1, SolidLine, FlatCap, MiterJoin));
					else
						pr.setPen(QPen(red, 1, DotLine, FlatCap, MiterJoin));
					pr.setBrush(NoBrush);
					pr.drawRect(-1, -1, static_cast<int>(Width*Doc->Scale)+2, static_cast<int>(Height*Doc->Scale)+2);
					if (Locked)
						{
						pr.setPen(QPen(darkRed, 1, SolidLine, FlatCap, MiterJoin));
						pr.setBrush(darkRed);
						}
					else
						{
						pr.setPen(QPen(red, 1, SolidLine, FlatCap, MiterJoin));
						pr.setBrush(red);
						}
					if (PType != 5)
						{				
						pr.drawRect(-1, -1, 6, 6);
						pr.drawRect(static_cast<int>(Width*Doc->Scale), static_cast<int>(Height*Doc->Scale), -6, -6);
						pr.drawRect(static_cast<int>(Width*Doc->Scale), -1, -6, 6);
						pr.drawRect(-1, static_cast<int>(Height*Doc->Scale), 6, -6);
						if (Width > 6)
							{
							pr.drawRect(static_cast<int>(Width/2*Doc->Scale - 3), static_cast<int>(Height*Doc->Scale), 6, -6);
							pr.drawRect(static_cast<int>(Width/2*Doc->Scale - 3), -1, 6, 6);
							}
						if (Height > 6)
							{
							pr.drawRect(static_cast<int>(Width*Doc->Scale), static_cast<int>(Height/2*Doc->Scale - 3), -6, 6);
							pr.drawRect(-1, static_cast<int>(Height/2*Doc->Scale - 3), 6, 6);
							}
						}
					else
						{
						pr.drawRect(-3, -3, 6, 6);
						pr.drawRect(static_cast<int>(Width*Doc->Scale)+3, -3, -6, 6);
						}
					pr.end();
					}
				else
					{
					p.setPen(QPen(darkCyan, 1, DotLine, FlatCap, MiterJoin));
					p.setBrush(NoBrush);
					p.drawRect(-1, -1, static_cast<int>(Width+2), static_cast<int>(Height+2));
					}
				}
			}
		}
	Tinput = false;
	FrameOnly = false;
	Dirty = false;
	p.end();
}

QString PageItem::ExpandToken(uint base)
{
	uint zae = 0;
	QString chx = "#";
	if (!Doc->MasterP)
		{
		while (Ptext.at(base+zae)->ch == QChar(30))
			{
			zae++;
			if (base+zae == Ptext.count())
				break;
			}
		QString out="%1";
		chx = out.arg(OwnPage->PageNr+Doc->FirstPnum, zae).right(zae).left(1);
		}
	return chx;
}

void PageItem::SetFarbe(QColor *tmp, QString farbe, int shad)
{
	int h, s, v, sneu;
	Doc->PageColors[farbe].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
		{
		Doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shad / 100);
		tmp->setHsv(h, s, sneu);			
		}
	else
		{
		Doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shad / 100;
		tmp->setHsv(h, sneu, v);
		}
}

void PageItem::SetZeichAttr(QPainter *p, QFont *ffo, struct Pti *hl, int *chs, QString *chx)
{
	QFont ff(Doc->UsedFonts[hl->cfont]);
	ff.setPointSize(QMAX(static_cast<int>(hl->csize*DevRes), 1));
	p->setFont(ff);
	int asce = p->fontMetrics().ascent();
	int chst = hl->cstyle & 127;
	if (chst != 0)
		{
		if (chst & 16)
			ff.setStrikeOut(true);
		if (chst & 8)
			ff.setUnderline(true);
		if (chst & 1)
			{
			CurY -= asce * Doc->VHoch / 100;
			ff.setPointSize(QMAX(static_cast<int>(hl->csize * DevRes * Doc->VHochSc / 100), 1));
			*chs = QMAX(static_cast<int>(hl->csize * Doc->VHochSc / 100), 1);
			}
		if (chst & 2)
			{
			CurY += asce * Doc->VTief / 100;
			ff.setPointSize(QMAX(static_cast<int>(hl->csize * DevRes * Doc->VTiefSc / 100) ,1));
			*chs = QMAX(hl->csize * Doc->VTiefSc / 100, 1);
			}
		if (chst & 64)
			{
			if (chx->upper() != *chx)
				{
				ff.setPointSize(QMAX(static_cast<int>(hl->csize * DevRes * Doc->VKapit / 100), 1));
				*chs = QMAX(static_cast<int>(hl->csize * Doc->VKapit / 100), 1);
				*chx = chx->upper();
				}
			}
		}
	p->setFont(ff);
}

void PageItem::DrawZeichen(QPainter *p, struct Pti *hl)
{
	struct ZZ *Zli;
	Zli = new ZZ;
	Zli->Zeich = hl->ch;
	Zli->xco = hl->xp;
	Zli->yco = hl->yp;
	Zli->Sele = hl->cselect;
	Zli->Siz = hl->csize;
	Zli->ZFo = hl->cfont;
	Zli->wide = Cwidth(Doc, p, hl->cfont, hl->ch, hl->csize);
	DrawZeichen(p, Zli);
	delete Zli;
}

void PageItem::DrawZeichen(QPainter *p, struct ZZ *hl)
{
	QRegion cr = p->clipRegion();
	QRect rr = QRect(static_cast<int>(hl->xco), static_cast<int>(hl->yco - LineSp), static_cast<int>(hl->wide), static_cast<int>(LineSp));
	if (!cr.contains(p->xForm(rr)))
		return;
#ifdef HAVE_FREETYPE
	FT_Face face;
	uint chr = hl->Zeich[0].unicode();
	if ((chr > 255) || (Doc->Scale > 1))
		{
		if ((*Doc->AllFonts)[hl->ZFo]->CharWidth.contains(chr))
			{
			face = Doc->FFonts[hl->ZFo];
			FT_Set_Char_Size(Doc->FFonts[hl->ZFo], 0, QMAX(hl->Siz*64, 1), qRound(QMAX(1, DevResX * Doc->Scale)), qRound(QMAX(1, DevResY * Doc->Scale)));
			FT_Load_Char(face, chr, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_MONOCHROME);
			QByteArray bd(face->glyph->bitmap.rows * face->glyph->bitmap.pitch);
			uint yy = 0;
			uint adv;
			if ((face->glyph->bitmap.width % 8) == 0)
				adv = face->glyph->bitmap.width / 8;
			else
				adv = face->glyph->bitmap.width / 8 + 1;
			for (int y = 0; y < face->glyph->bitmap.rows; ++y)
				{
				memcpy(bd.data()+yy, face->glyph->bitmap.buffer+(y * face->glyph->bitmap.pitch), adv);
				yy += adv;
				}
			QBitmap bb(face->glyph->bitmap.width, face->glyph->bitmap.rows, (uchar*)bd.data(), false);
			QPixmap pixm(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			if (!pixm.isNull())
				{
				pixm.fill(p->pen().color());
				pixm.setMask(bb);
				p->save();
				if (Reverse)
					{
					p->translate(static_cast<int>(hl->xco), static_cast<int>(hl->yco));
					p->scale(-1, 1);
					p->scale(static_cast<double>(1.0 / Doc->Scale), static_cast<double>(1.0 / Doc->Scale));
					p->translate(-pixm.width(), 0);
					p->drawPixmap(-face->glyph->bitmap_left, -face->glyph->bitmap_top, pixm);
					}
				else
					{
					p->translate(static_cast<int>(hl->xco), static_cast<int>(hl->yco));
					p->scale(static_cast<double>(1.0 / Doc->Scale), static_cast<double>(1.0 / Doc->Scale));
					p->drawPixmap(face->glyph->bitmap_left, -face->glyph->bitmap_top, pixm);
					}
				p->restore();
				}
			if (p->font().strikeOut())
				{
				int st = -p->fontMetrics().strikeOutPos();
				QPen px = p->pen();
				px.setWidth(QMAX(p->fontMetrics().lineWidth(), 1));
				p->setPen(px);
				p->drawLine(static_cast<int>(hl->xco), static_cast<int>(hl->yco)+st, static_cast<int>(hl->xco)+static_cast<int>(hl->wide), static_cast<int>(hl->yco)+st);
				}
			if (p->font().underline())
				{
				int st = p->fontMetrics().underlinePos();
				QPen px = p->pen();
				px.setWidth(QMAX(p->fontMetrics().lineWidth(), 1));
				p->setPen(px);
				p->drawLine(static_cast<int>(hl->xco), static_cast<int>(hl->yco)+st, static_cast<int>(hl->xco)+static_cast<int>(hl->wide), static_cast<int>(hl->yco)+st);
				}
			}
		else
			{
			QPen px = p->pen();
			px.setWidth(1);
			p->setPen(px);
			p->setBrush(NoBrush);
			p->drawRect(QRect(static_cast<int>(hl->xco), static_cast<int>(hl->yco)-hl->Siz, hl->Siz, hl->Siz));
			}
		}
	else
		{
		if (Reverse)
			{
			p->save();
			p->translate(static_cast<int>(hl->xco), static_cast<int>(hl->yco));
			p->scale(-1, 1);
			p->translate(-hl->wide, 0);
			p->drawText(0, 0, hl->Zeich);
			p->restore();
			}
		else
			p->drawText(static_cast<int>(hl->xco), static_cast<int>(hl->yco), hl->Zeich);
		}
#else
	if (Reverse)
		{
		p->save();
		p->translate(static_cast<int>(hl->xco), static_cast<int>(hl->yco));
		p->scale(-1, 1);
		p->translate(-hl->wide, 0);
		p->drawText(0, 0, hl->Zeich);
		p->restore();
		}
	else
		p->drawText(static_cast<int>(hl->xco), static_cast<int>(hl->yco), hl->Zeich);
#endif
}

void PageItem::DrawPoly(QPainter *p, QPointArray pts, QColor BackF, bool bitm)
{
	if ((Pcolor != "None") || (GrType != 0) || (PType == 2))
		{
		QBitmap bm(static_cast<int>(Width), static_cast<int>(Height));
		bm.fill(Qt::color0);
		QPainter pbm;
		pbm.begin(&bm);
		pbm.setBrush(Qt::color1);
		pbm.setPen(NoPen);
		pbm.setRasterOp(XorROP);
		QPointArray dr;
		QValueList<uint>::Iterator it3;
		uint FirstVal = 0;
		for (it3 = Segments.begin(); it3 != Segments.end(); ++it3)
			{
			dr.resize(0);
			dr.putPoints(0, (*it3)-FirstVal-1, pts, FirstVal);
			pbm.drawPolygon(dr);
			FirstVal = (*it3);
			}
		dr.resize(0);
		dr.putPoints(0, pts.size()-FirstVal-1, pts, FirstVal);
		pbm.drawPolygon(dr);
		pbm.end();
		if (bitm)
			p->drawPixmap(0, 0, bm);
		else
			{
			QPixmap ppm(static_cast<int>(Width), static_cast<int>(Height));
			ppm.fill(BackF);
			ppm.setMask(bm);
			p->drawPixmap(0, 0, ppm);
			}
		}
	p->setBrush(Qt::NoBrush);
	if (Segments.count() != 0)
		{
		QValueList<uint>::Iterator it2;
		uint FirstVal = 0;
		for (it2 = Segments.begin(); it2 != Segments.end(); ++it2)
			{
			p->drawPolyline(pts, FirstVal, (*it2)-FirstVal);
			FirstVal = (*it2);
			}
		p->drawPolyline(pts, FirstVal);
		}
	else
		p->drawPolyline(pts);
}

void PageItem::DrawPolyL(QPainter *p, QPointArray pts)
{
	if (Segments.count() != 0)
		{
		QValueList<uint>::Iterator it2;
		uint FirstVal = 0;
		for (it2 = Segments.begin(); it2 != Segments.end(); ++it2)
			{
			p->drawPolyline(pts, FirstVal, (*it2)-FirstVal);
			FirstVal = (*it2);
			}
		p->drawPolyline(pts, FirstVal);
		}
	else
		p->drawPolyline(pts);
}

void PageItem::CopyIt(struct CLBuf *Buffer)
{
	uint a;
	Buffer->PType = PType;
	Buffer->Xpos = Xpos;
	Buffer->Ypos = Ypos;
	Buffer->Width = Width;
	Buffer->Height = Height;
	Buffer->RadRect = RadRect;
	Buffer->FrameType = FrameType;
	Buffer->ClipEdited = ClipEdited;
	Buffer->Pwidth = Pwidth;
	Buffer->Pcolor = Pcolor;
	Buffer->Pcolor2 = Pcolor2;
	Buffer->Shade = Shade;
	Buffer->Shade2 = Shade2;
	Buffer->GrColor = GrColor;
	Buffer->GrColor2 = GrColor2;
	Buffer->GrShade = GrShade;
	Buffer->GrShade2 = GrShade2;
	Buffer->GrType = GrType;
	Buffer->Rot = Rot;
	Buffer->PLineArt = PLineArt;
	Buffer->PLineEnd = PLineEnd;
	Buffer->PLineJoin = PLineJoin;
	Buffer->LineSp = LineSp;
	Buffer->LocalScX = LocalScX;
	Buffer->LocalScY = LocalScY;
	Buffer->LocalX = LocalX;
	Buffer->LocalY = LocalY;
	Buffer->PicArt = PicArt;
	Buffer->flippedH = flippedH;
	Buffer->flippedV = flippedV;
	Buffer->BBoxX = BBoxX;
	Buffer->BBoxH = BBoxH;
	Buffer->isPrintable = isPrintable;
	Buffer->isBookmark = isBookmark;
	Buffer->BMnr = BMnr;
	Buffer->isAnnotation = isAnnotation;
	Buffer->AnType = AnType;
	Buffer->AnAction = AnAction;
	Buffer->An_E_act = An_E_act;
	Buffer->An_X_act = An_X_act;
	Buffer->An_D_act = An_D_act;
	Buffer->An_Fo_act = An_Fo_act;
	Buffer->An_Bl_act = An_Bl_act;
	Buffer->An_K_act = An_K_act;
	Buffer->An_F_act = An_F_act;
	Buffer->An_V_act = An_V_act;
	Buffer->An_C_act = An_C_act;
	Buffer->An_Extern = An_Extern;
	Buffer->AnZiel = AnZiel;
	Buffer->AnName = AnName;
	Buffer->AnActType = AnActType;
	Buffer->AnToolTip = AnToolTip;
	Buffer->AnBwid = AnBwid;
	Buffer->AnBsty = AnBsty;
	Buffer->AnFeed = AnFeed;
	Buffer->AnFlag = AnFlag;
	Buffer->AnFont = AnFont;
	Buffer->AnRollOver = AnRollOver;
	Buffer->AnDown = AnDown;
	Buffer->AnFormat = AnFormat;
	Buffer->AnVis = AnVis;
	Buffer->AnMaxChar = AnMaxChar;
	Buffer->AnChkStil = AnChkStil;
	Buffer->AnIsChk = AnIsChk;
	Buffer->AnAAact = AnAAact;
	Buffer->AnBColor = AnBColor;
	Buffer->AnHTML = AnHTML;
	Buffer->AnUseIcons = AnUseIcons;
	Buffer->AnIPlace = AnIPlace;
	Buffer->AnScaleW = AnScaleW;
	Buffer->Extra = Extra;
	Buffer->TExtra = TExtra;
	Buffer->BExtra = BExtra;
	Buffer->RExtra = RExtra;
	Buffer->Pfile = Pfile;
	Buffer->Pfile2 = Pfile2;
	Buffer->Pfile3 = Pfile3;
	QString Text = "";
	if (Ptext.count() != 0)
		{
		for (a=0; a<Ptext.count(); ++a)
			{
			if( (Ptext.at(a)->ch == "\n") || (Ptext.at(a)->ch == "\r"))
				Text += QString(QChar(5))+"\t";
			else
				Text += Ptext.at(a)->ch+"\t";
			Text += Ptext.at(a)->cfont+"\t";
			Text += QString::number(Ptext.at(a)->csize)+"\t";
			Text += Ptext.at(a)->ccolor+"\t";
			Text += QString::number(Ptext.at(a)->cextra)+"\t";
			Text += QString::number(Ptext.at(a)->cshade)+'\t';
			Text += QString::number(Ptext.at(a)->cstyle)+'\t';
			Text += QString::number(Ptext.at(a)->cab)+'\n';
			}
		}
	Buffer->Ptext = Text;
	Buffer->Clip = Clip.copy();
	Buffer->PoLine = PoLine.copy();
	Buffer->PoShow = PoShow;
	Buffer->BaseOffs = BaseOffs;
	Buffer->Textflow = Textflow;
	Buffer->Textflow2 = Textflow2;
	Buffer->Ausrich = Ausrich;
	Buffer->IFont = IFont;
	Buffer->ISize = ISize;
	Buffer->ExtraV = ExtraV;
	Buffer->Groups = Groups;
	Buffer->IProfile = IProfile;
	Buffer->IRender = IRender;
	Buffer->UseEmbedded = UseEmbedded;
	Buffer->EmProfile = EmProfile;
	Buffer->LayerNr = LayerNr;
	Buffer->ScaleType = ScaleType;
	Buffer->AspectRatio = AspectRatio;
	Buffer->Locked = Locked;
	Buffer->Transparency = Transparency;
	Buffer->Reverse = Reverse;
	Buffer->InvPict = InvPict;
}
