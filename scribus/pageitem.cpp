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
#include "pageitem.moc"
#include <qpainter.h>
#include <qpen.h>
#include <qfont.h>
#include <qregion.h>
#include <qpoint.h>
#include <qfileinfo.h>
#include <qdrawutil.h>
#include <qbitmap.h>
#include <cmath>
#include "page.h"
#include "scribus.h"
#include "scribusdoc.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include <ft2build.h>
#include FT_GLYPH_H

using namespace std;

extern double Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2 = " ");
extern double RealCWidth(ScribusDoc *doc, QString name, QString ch, int Siz);
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern double xy2Deg(double x, double y);
extern void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
extern ScribusApp* ScApp;

PageItem::PageItem(ScribusDoc *pa, int art, double x, double y, double w, double h, double w2, QString fill, QString outline) : QObject(pa)
{
	QString tmp;
	BackBox = 0;
	NextBox = 0;
	Locked = false;
	LockRes = false;
	Xpos = x;
	Ypos = y;
	Width = w;
	Height = h;
	BoundingX = x;
	BoundingY = y;
	BoundingW = w;
	BoundingH = h;
	OldB = Width;
	OldH = Height;
	OldB2 = Width;
	OldH2 = Height;
	PType = art;
	Rot = 0;
	Doc = pa;
	Pcolor = fill;
	Pcolor2 = PType == 4 ? fill : outline;
	TxtFill = Doc->DpenText;
	TxtStroke = Doc->DstrokeText;
	ShTxtStroke = 100;
	ShTxtFill = 100;
	TxtScale = 100;
	TxTStyle = 0;
	Shade = 100;
	Shade2 = 100;
	GrType = 0;
	GrStartX = 0;
	GrStartY = 0;
	GrEndX = w;
	GrEndY = 0;
	Pwidth = w2;
	OldPwidth = w2;
	PLineArt = Doc->DLineArt;
	PLineEnd = FlatCap;
	PLineJoin = MiterJoin;
	Select = false;
	FrameOnly = false;
	ClipEdited = false;
	FrameType = 0;
	IFont = Doc->Dfont;
	ISize = Doc->Dsize;
	LineSp = ((Doc->Dsize / 10.0) * static_cast<double>(Doc->typographicSetttings.autoLineSpacing) / 100) + (Doc->Dsize / 10.0);
	Doc->Vorlagen[0].LineSpa = LineSp;
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
	MaxChars = 0;
	Pfile = "";
	pixm = QImage();
	pixmOrg = QImage();
	Pfile2 = "";
	Pfile3 = "";
	LocalScX = 1;
	LocalScY = 1;
	LocalViewX = 1;
	LocalViewY = 1;
	OrigW = 0;
	OrigH = 0;
	dpiX = 72.0;
	dpiY = 72.0;
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
			Clip.setPoints(4, static_cast<int>(w/2), 0, static_cast<int>(w), static_cast<int>(h/2),
								static_cast<int>(w/2), static_cast<int>(h), 0,static_cast<int>(h/2));
			break;
		default:
			Clip.setPoints(4, 0,0, static_cast<int>(w),0, static_cast<int>(w), static_cast<int>(h), 0,static_cast<int>(h));
			break;
	}
	PoLine.resize(0);
	ContourLine.resize(0);
	UseContour = false;
	Segments.clear();
	PoShow = false;
	BaseOffs = 0;
	OwnPage = Doc->ActPage->PageNr;
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
	switch (PType)
	{
	case 2:
		AnName = tr("Image");
		break;
	case 4:
		AnName = tr("Text");
		break;
	case 5:
		AnName = tr("Line");
		break;
	case 6:
		AnName = tr("Polygon");
		break;
	case 7:
		AnName = tr("Polyline");
		break;
	case 8:
		AnName = tr("PathText");
		break;
	default:
		AnName = "Item";
		break;
	}
	AnName += tmp.setNum(Doc->TotalItems); // +" "+QDateTime::currentDateTime().toString();
	AutoName = true;
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
	Textflow = false;
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
	TranspStroke = 0.0;
	Reverse = false;
	InvPict = false;
	NamedLStyle = "";
	DashValues.clear();
	TabValues.clear();
	DashOffset = 0;
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();
	if (Pcolor == "None")
		fill_gradient.addStop(Doc->PageColors[Doc->Dbrush].getRGBColor(), 0.0, 0.5, 1.0, Doc->Dbrush, 100);
	else
		fill_gradient.addStop(Doc->PageColors[Pcolor].getRGBColor(), 0.0, 0.5, 1.0, Pcolor, 100);
	if (Pcolor2 == "None")
		fill_gradient.addStop(Doc->PageColors[Doc->Dpen].getRGBColor(), 1.0, 0.5, 1.0, Doc->Dpen, 100);
	else
	fill_gradient.addStop(Doc->PageColors[Pcolor2].getRGBColor(), 1.0, 0.5, 1.0, Pcolor2, 100);
	Language = Doc->Language;
	Cols = Doc->DCols;
	ColGap = Doc->DGap;
	LeftLink = 0;
	RightLink = 0;
	TopLink = 0;
	BottomLink = 0;
	LeftLinkID = 0;
	RightLinkID = 0;
	TopLinkID = 0;
	BottomLinkID = 0;
	LeftLine = 0;
	RightLine = false;
	TopLine = false;
	BottomLine = false;
	isTableItem = false;
	isSingleSel = false;
	Dirty = false;
	ChangedMasterItem = false;
	OnMasterPage = Doc->ActPage->PageNam;
	startArrowIndex = Doc->DstartArrow;
	endArrowIndex = Doc->DendArrow;
}

/** Zeichnet das Item */
void PageItem::DrawObj(ScPainter *p, QRect e)
{
	QColor tmp;
	FPointArray CL;
	QPointArray cl;
	QPainter pf, pp, pf2;
	PageItem *nb;
	QPoint pt1, pt2;
	FPoint gv, ColBound;
	QRegion cm;
	uint a, nrc, nrc2, zae, startLin;
	int desc, asce, absa, aSpa, chs, chsd, CurrCol;
	uint BuPos, LastSP, MaxText;
	double oldCurY, LastXp, EndX, OFs, OFs2, wide, rota, wid, lineCorr, ColWidth, kernVal, RTabX;
	double sc = ScApp->view->Scale;
	QString chx, chx2, chx3;
	struct Pti *hl;
	struct ZZ *Zli;
	struct ZZ *Zli2;
	QPtrList<ZZ> LiList;
	bool outs = false;
	bool fBorder = false;
	bool RTab = false;
	bool goNoRoom = false;
	uint StartRT, StartRT2;
	int TabCode = 0;
	int HyphenCount = 0;
	QValueList<double> tTabValues;
	bool DropCmode = false;
	bool AbsHasDrop = false;
	double maxDY, firstDes, desc2, maxDX;
	int DropLines;
	bool StartOfCol = true;
	tTabValues.clear();
	if (!Doc->DoDrawing)
	{
		Redrawn = true;
		Tinput = false;
		FrameOnly = false;
		return;
	}
	LiList.setAutoDelete(true);
	for (int xxx=0; xxx<5; ++xxx)
	{
		Doc->Vorlagen[xxx].LineSpa = LineSp;
		Doc->Vorlagen[xxx].FontSize = ISize;
		Doc->Vorlagen[xxx].Indent = 0;
		Doc->Vorlagen[xxx].First = 0;
		Doc->Vorlagen[xxx].Avor = 0;
		Doc->Vorlagen[xxx].Anach = 0;
		Doc->Vorlagen[xxx].Ausri = xxx;
	}
	pf.begin(ScApp->view->viewport());
	QPoint trans = ScApp->view->contentsToViewport(QPoint(qRound(Xpos*sc), qRound(Ypos*sc)));
	pf.translate(trans.x(), trans.y());
	pf.rotate(Rot);
	pf.scale(sc, sc);
	if (!Doc->RePos)
		pf.setClipRect(!e.isEmpty() ? e : QRect(0, 0, ScApp->view->viewport()->width(), ScApp->view->viewport()->height()));
	bool doStroke = true;
	QRect e2 = QRect(qRound(e.x() / sc), qRound(e.y() / sc), qRound(e.width() / sc), qRound(e.height() / sc));
	p->setZoomFactor(sc);
	p->save();
	p->translate(Xpos*sc, Ypos*sc);
	p->rotate(Rot);
	p->setLineWidth(Pwidth);
	if (GrType != 0)
	{
		p->setFillMode(ScPainter::Gradient);
		p->fill_gradient = fill_gradient;
		QWMatrix grm;
		grm.rotate(Rot);
		FPointArray gra;
		switch (GrType)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 6:
				gra.setPoints(2, GrStartX, GrStartY, GrEndX, GrEndY);
				gra.map(grm);
				p->setGradient(VGradient::linear, gra.point(0), gra.point(1));
				break;
			case 5:
			case 7:
				gra.setPoints(2, GrStartX, GrStartY, GrEndX, GrEndY);
				p->setGradient(VGradient::radial, gra.point(0), gra.point(1), gra.point(0));
				break;
		}
	}
	else
	{
		p->fill_gradient = VGradient(VGradient::linear);
		if (Pcolor != "None")
		{
			SetFarbe(&tmp, Pcolor, Shade);
			p->setBrush(tmp);
			p->setFillMode(ScPainter::Solid);
		}
		else
			p->setFillMode(ScPainter::None);
	}
	if (Pcolor2 != "None")
	{
		SetFarbe(&tmp, Pcolor2, Shade2);
		if ((Pwidth == 0) && (PType != 5))
			p->setLineWidth(0);
		else
		{
			p->setPen(tmp, Pwidth, PLineArt, PLineEnd, PLineJoin);
			if (DashValues.count() != 0)
				p->setDash(DashValues, DashOffset);
		}
	}
	else
		p->setLineWidth(0);
	p->setBrushOpacity(1.0 - Transparency);
	p->setPenOpacity(1.0 - TranspStroke);
	switch (PType)
	{
		case 2:
			if (Doc->RePos)
				break;
			if ((Pcolor != "None") || (GrType != 0))
			{
				p->setupPolygon(&PoLine);
				p->fillPath();
			}
			if (Pfile == "")
			{
				if ((Frame) && (Doc->guidesSettings.framesShown))
				{
					p->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
					p->drawLine(FPoint(0, 0), FPoint(Width, Height));
					p->drawLine(FPoint(0, Height), FPoint(Width, 0));
				}
			}
			else
			{
				if ((!PicArt) || (!PicAvail))
				{
					if ((Frame) && (Doc->guidesSettings.framesShown))
					{
						p->setPen(red, 1, SolidLine, FlatCap, MiterJoin);
						p->drawLine(FPoint(0, 0), FPoint(Width, Height));
						p->drawLine(FPoint(0, Height), FPoint(Width, 0));
					}
				}
				else
				{
					p->setupPolygon(&PoLine);
					p->setClipPath();
					p->save();
					if (flippedH % 2 != 0)
					{
						p->translate(Width * sc, 0);
						p->scale(-1, 1);
					}
					if (flippedV % 2 != 0)
					{
						p->translate(0, Height * sc);
						p->scale(1, -1);
					}
					if ((LocalViewX != 1) || (LocalViewY != 1))
						p->scale(LocalViewX, LocalViewY);
					p->translate(LocalX*LocalScX*sc, LocalY*LocalScY*sc);
					if (InvPict)
					{
						QImage ip = pixm.copy();
						p->drawImage(&ip);
					}
					else
						p->drawImage(&pixm);
					p->restore();
				}
			}
			break;
		case 5:
			if (Doc->RePos)
				break;
			doStroke = false;
			if (NamedLStyle == "")
				p->drawLine(FPoint(0, 0), FPoint(Width, 0));
			else
			{
				multiLine ml = Doc->MLineStyles[NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(tmp, ml[it].Width,
									 static_cast<PenStyle>(ml[it].Dash),
									 static_cast<PenCapStyle>(ml[it].LineEnd),
									 static_cast<PenJoinStyle>(ml[it].LineJoin));
					p->drawLine(FPoint(0, 0), FPoint(Width, 0));
				}
			}
			if (startArrowIndex != 0)
			{
				QWMatrix arrowTrans;
				FPointArray arrow = (*Doc->arrowStyles.at(startArrowIndex-1)).points.copy();
				arrowTrans.translate(0, 0);
				arrowTrans.scale(Pwidth, Pwidth);
				arrowTrans.scale(-1,1);
				arrow.map(arrowTrans);
				p->setBrush(p->pen());
				p->setBrushOpacity(1.0 - TranspStroke);
				p->setLineWidth(0);
				p->setFillMode(ScPainter::Solid);
				p->setupPolygon(&arrow);
				p->fillPath();
			}
			if (endArrowIndex != 0)
			{
				QWMatrix arrowTrans;
				FPointArray arrow = (*Doc->arrowStyles.at(endArrowIndex-1)).points.copy();
				arrowTrans.translate(Width, 0);
				arrowTrans.scale(Pwidth, Pwidth);
				arrow.map(arrowTrans);
				p->setBrush(p->pen());
				p->setBrushOpacity(1.0 - TranspStroke);
				p->setLineWidth(0);
				p->setFillMode(ScPainter::Solid);
				p->setupPolygon(&arrow);
				p->fillPath();
			}
			break;
		case 1:
		case 3:
		case 6:
			if (Doc->RePos)
				break;
			p->setupPolygon(&PoLine);
			p->fillPath();
			break;
		case 7:
			doStroke = false;
			if (Doc->RePos)
				break;
			if (PoLine.size() < 4)
				break;
			if ((Pcolor != "None") || (GrType != 0))
			{
				FPointArray cli;
				FPoint Start;
				bool firstp = true;
				for (uint n = 0; n < PoLine.size()-3; n += 4)
				{
					if (firstp)
					{
						Start = PoLine.point(n);
						firstp = false;
					}
					if (PoLine.point(n).x() > 900000)
					{
						cli.addPoint(PoLine.point(n-2));
						cli.addPoint(PoLine.point(n-2));
						cli.addPoint(Start);
						cli.addPoint(Start);
						cli.setMarker();
						firstp = true;
						continue;
					}
					cli.addPoint(PoLine.point(n));
					cli.addPoint(PoLine.point(n+1));
					cli.addPoint(PoLine.point(n+2));
					cli.addPoint(PoLine.point(n+3));
				}
				if (cli.size() > 2)
				{
					FPoint l1 = cli.point(cli.size()-2);
					cli.addPoint(l1);
					cli.addPoint(l1);
					cli.addPoint(Start);
					cli.addPoint(Start);
				}
				p->setupPolygon(&cli);
				p->fillPath();
			}
			p->setupPolygon(&PoLine, false);
			if (NamedLStyle == "")
				p->strokePath();
			else
			{
				multiLine ml = Doc->MLineStyles[NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(tmp, ml[it].Width,
									 static_cast<PenStyle>(ml[it].Dash),
									 static_cast<PenCapStyle>(ml[it].LineEnd),
									 static_cast<PenJoinStyle>(ml[it].LineJoin));
					p->strokePath();
				}
			}
			if (startArrowIndex != 0)
			{
				FPoint Start = PoLine.point(0);
				for (uint xx = 1; xx < PoLine.size(); xx += 2)
				{
					FPoint Vector = PoLine.point(xx);
					if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
					{
						double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/3.1415927);
						QWMatrix arrowTrans;
						FPointArray arrow = (*Doc->arrowStyles.at(startArrowIndex-1)).points.copy();
						arrowTrans.translate(Start.x(), Start.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(Pwidth, Pwidth);
						arrow.map(arrowTrans);
						p->setBrush(p->pen());
						p->setBrushOpacity(1.0 - TranspStroke);
						p->setLineWidth(0);
						p->setFillMode(ScPainter::Solid);
						p->setupPolygon(&arrow);
						p->fillPath();
						break;
					}
				}
			}
			if (endArrowIndex != 0)
			{
				FPoint End = PoLine.point(PoLine.size()-2);
				for (uint xx = PoLine.size()-1; xx > 0; xx -= 2)
				{
					FPoint Vector = PoLine.point(xx);
					if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
					{
						double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/3.1415927);
						QWMatrix arrowTrans;
						FPointArray arrow = (*Doc->arrowStyles.at(endArrowIndex-1)).points.copy();
						arrowTrans.translate(End.x(), End.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(Pwidth, Pwidth);
						arrow.map(arrowTrans);
						p->setBrush(p->pen());
						p->setBrushOpacity(1.0 - TranspStroke);
						p->setLineWidth(0);
						p->setFillMode(ScPainter::Solid);
						p->setupPolygon(&arrow);
						p->fillPath();
						break;
					}
				}
			}
			break;
		case 4:
			p->save();
			pf2.begin(ScApp->view->viewport());
			pf2.translate(Xpos, Ypos);
			pf2.rotate(Rot);
			if ((Pcolor != "None") || (GrType != 0))
			{
				p->setupPolygon(&PoLine);
				p->fillPath();
			}
			if (Pcolor2 != "None")
				lineCorr = Pwidth / 2.0;
			else
				lineCorr = 0;
			if ((isAnnotation) && (AnType == 2) && (Pfile != "") && (PicAvail) && (PicArt) && (AnUseIcons))
			{
				p->setupPolygon(&PoLine);
				p->setClipPath();
				p->save();
				p->scale(LocalScX, LocalScY);
				p->translate(static_cast<int>(LocalX*LocalScX), static_cast<int>(LocalY*LocalScY));
				if (!pixm.isNull())
					p->drawImage(&pixm);
				p->restore();
			}
			if ((Ptext.count() != 0) && (Dirty))
			{
				if (flippedH % 2 != 0)
				{
					p->translate(Width * sc, 0);
					p->scale(-1, 1);
				}
				if (flippedV % 2 != 0)
				{
					p->translate(0, Height * sc);
					p->scale(1, -1);
				}
				struct ZZ Zli3;
				for (a = 0; a < Ptext.count(); ++a)
				{
					if (a > MaxChars)
						break;
					hl = Ptext.at(a);
					chx = hl->ch;
					if (hl->ch == QChar(30))
						chx = ExpandToken(a);
					if (hl->ccolor != "None")
					{
						SetFarbe(&tmp, hl->ccolor, hl->cshade);
						p->setBrush(tmp);
					}
					if (hl->cstroke != "None")
					{
						SetFarbe(&tmp, hl->cstroke, hl->cshade2);
						p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
					}
					chs = hl->csize;
					oldCurY = SetZeichAttr(hl, &chs, &chx);
					Zli3.Zeich = chx;
					Zli3.Farb = hl->ccolor;
					Zli3.Farb2 = hl->cstroke;
					Zli3.shade = hl->cshade;
					Zli3.shade2 = hl->cshade2;
					Zli3.xco = hl->xp;
					Zli3.yco = hl->yp;
					Zli3.Sele = hl->cselect;
					Zli3.Siz = chs;
					Zli3.Style = hl->cstyle;
					Zli3.ZFo = hl->cfont;
					Zli3.wide = Cwidth(Doc, hl->cfont, chx, hl->csize);
					Zli3.kern = hl->cextra;
					Zli3.scale = hl->cscale;
					if (!Doc->RePos)
					{
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-LineSp), qRound(Zli3.wide+1), qRound(LineSp*1.5)))))
							DrawZeichenS(p, &Zli3);
						if (hl->cstyle & 512)
						{
							Zli3.Zeich = "-";
							Zli3.xco = Zli3.xco + Cwidth(Doc, hl->cfont, chx, hl->csize);
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-LineSp), qRound(Zli3.wide+1), qRound(LineSp*1.5)))))
								DrawZeichenS(p, &Zli3);
						}
					}
				}
				if (Ptext.count() > MaxChars)
				{
					if (!Doc->RePos)
					{
						double scp1 = 1 / QMAX(ScApp->view->Scale, 1);
						double scp16 = 16 * scp1;
						double scp14 = 14 * scp1;
						double scp3 = 3 * scp1;
						double scpwidth16 = Width - scp16;
						double scpheight16 = Height - scp16;
						double scpwidth3 = Width - scp3;
						double scpheight3 = Height - scp3;
						p->setBrush(white);
						p->setPen(black, scp1, SolidLine, FlatCap, MiterJoin);
						p->drawRect(scpwidth16, scpheight16, scp14, scp14);
						p->drawLine(FPoint(scpwidth16, scpheight16), FPoint(scpwidth3, scpheight3));
						p->drawLine(FPoint(scpwidth16, scpheight3), FPoint(scpwidth3, scpheight16));
					}
				}
				Dirty = false;
				Redrawn = true;
				pf2.end();
				p->restore();
				break;
			}
			if ((Ptext.count() != 0) || (NextBox != 0))
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
						nb->MaxChars = 0;
						nb = nb->NextBox;
					}
					nb = NextBox;
				}
				Doc->Vorlagen[0].LineSpa = LineSp;
				QRegion cl = QRegion(pf2.xForm(Clip));
				for (a = 0; a < Doc->Items.count(); ++a)
				{
					if (((Doc->Items.at(a)->ItemNr > ItemNr) && (Doc->Items.at(a)->LayerNr == LayerNr))
   							|| (Doc->Layers[Doc->Items.at(a)->LayerNr].Level > Doc->Layers[LayerNr].Level))
					{
						if (Doc->Items.at(a)->Textflow)
						{
							pp.begin(ScApp->view->viewport());
							pp.translate(Doc->Items.at(a)->Xpos, Doc->Items.at(a)->Ypos);
							pp.rotate(Doc->Items.at(a)->Rot);
							if (Doc->Items.at(a)->Textflow2)
							{
								QPointArray tcli;
								tcli.resize(4);
								tcli.setPoint(0, QPoint(0,0));
								tcli.setPoint(1, QPoint(qRound(Doc->Items.at(a)->Width), 0));
								tcli.setPoint(2, QPoint(qRound(Doc->Items.at(a)->Width), qRound(Doc->Items.at(a)->Height)));
								tcli.setPoint(3, QPoint(0, qRound(Doc->Items.at(a)->Height)));
								cm = QRegion(pp.xForm(tcli));
							}
							else
							{
								if ((Doc->Items.at(a)->UseContour) && (Doc->Items.at(a)->ContourLine.size() != 0))
								{
									QValueList<uint> Segs;
									QPointArray Clip2 = FlattenPath(Doc->Items.at(a)->ContourLine, Segs);
									cm = QRegion(pp.xForm(Clip2));
								}
								else
									cm = QRegion(pp.xForm(Doc->Items.at(a)->Clip));
							}
							pp.end();
							cl = cl.subtract(cm);
						}
					}
				}
				if (flippedH % 2 != 0)
				{
					p->translate(Width * sc, 0);
					p->scale(-1, 1);
					pf2.translate(Width, 0);
					pf2.scale(-1, 1);
				}
				if (flippedV % 2 != 0)
				{
					p->translate(0, Height * sc);
					p->scale(1, -1);
					pf2.translate(0, Height);
					pf2.scale(1, -1);
				}
				CurrCol = 0;
				ColWidth = (Width - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
				ColBound = FPoint((ColWidth + ColGap) * CurrCol+Extra + lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
				ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
				CurX = ColBound.x();
				if (Ptext.count() > 0)
				{
					hl = Ptext.at(0);
					if (Doc->Vorlagen[hl->cab].Drop)
					{
						if (Doc->Vorlagen[hl->cab].BaseAdj)
							chs = qRound(Doc->typographicSetttings.valueBaseGrid  * Doc->Vorlagen[hl->cab].DropLin * 10);
						else
							chs = qRound(Doc->Vorlagen[hl->cab].LineSpa * Doc->Vorlagen[hl->cab].DropLin * 10);
					}
					else
						chs = hl->csize;
					desc2 = -(*Doc->AllFonts)[hl->cfont]->numDescender * (chs / 10.0);
					CurY = TExtra+lineCorr;
				}
				else
				{
					desc2 = -(*Doc->AllFonts)[IFont]->numDescender * (ISize / 10.0);
					CurY = Doc->Vorlagen[0].LineSpa+TExtra+lineCorr-desc2;
				}
				firstDes = desc2;
				LiList.clear();
				BuPos = 0;
				LastSP = 0;
				LastXp = 0;
				outs = false;
				OFs = 0;
				OFs2 = 0;
				aSpa = 0;
				absa = 0;
				MaxChars = 0;
				MaxText = Ptext.count();
				StartOfCol = true;
				for (a = 0; a < MaxText; ++a)
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
										if (chx != QChar(13))
											DropCmode = Doc->Vorlagen[absa].Drop;
										else
											DropCmode = false;
										if (DropCmode)
											DropLines = Doc->Vorlagen[absa].DropLin;
										break;
									}
									else
									{
										CurX += Doc->Vorlagen[absa].Indent;
										break;
									}
								}
								nb = nb->BackBox;
							}
						}
						else
						{
							if (chx != QChar(13))
								DropCmode = Doc->Vorlagen[absa].Drop;
							else
								DropCmode = false;
							CurX += Doc->Vorlagen[absa].First;
							CurX += Doc->Vorlagen[absa].Indent;
							CurY += Doc->Vorlagen[absa].Avor;
							if (DropCmode)
								DropLines = Doc->Vorlagen[absa].DropLin;
						}
					}
					if (((Doc->Vorlagen[absa].Ausri == 3) || (Doc->Vorlagen[absa].Ausri == 4)) && (LiList.count() == 0) && (hl->ch == " "))
					{
						hl->cstyle |= 256;
						continue;
					}
					else
						hl->cstyle &= 255;
					if (LiList.count() == 0)
					{
						if ((a > 0) && (Ptext.at(a-1)->ch == QChar(13)))
						{
							CurX += Doc->Vorlagen[absa].First;
							CurX += Doc->Vorlagen[absa].Indent;
							CurY += Doc->Vorlagen[absa].Avor;
							if (chx != QChar(13))
								DropCmode = Doc->Vorlagen[absa].Drop;
							else
								DropCmode = false;
							if (DropCmode)
							{
								DropLines = Doc->Vorlagen[absa].DropLin;
								if (Doc->Vorlagen[hl->cab].BaseAdj)
									CurY += Doc->typographicSetttings.valueBaseGrid * (DropLines-1);
								else
									CurY += Doc->Vorlagen[absa].LineSpa * (DropLines-1);
							}
						}
					}
					if (DropCmode)
					{
						if (Doc->Vorlagen[hl->cab].BaseAdj)
						{
							chsd = qRound(10 * ((Doc->typographicSetttings.valueBaseGrid * DropLines) / ((*Doc->AllFonts)[hl->cfont]->numAscent+(*Doc->AllFonts)[hl->cfont]->numDescender)));
							chs = qRound(10 * ((Doc->typographicSetttings.valueBaseGrid * DropLines) / (*Doc->AllFonts)[hl->cfont]->numAscent));
						}
						else
						{
							chsd = qRound(10 * ((Doc->Vorlagen[absa].LineSpa * DropLines) / ((*Doc->AllFonts)[hl->cfont]->numAscent+(*Doc->AllFonts)[hl->cfont]->numDescender)));
							chs = qRound(10 * ((Doc->Vorlagen[absa].LineSpa * DropLines) / (*Doc->AllFonts)[hl->cfont]->numAscent));
						}
						hl->csize = chsd;
					}
					else
						chs = hl->csize;
					oldCurY = SetZeichAttr(hl, &chs, &chx);
					if (chx == QChar(29))
						chx2 = " ";
					else
						chx2 = chx;
					if (a < MaxText-1)
					{
						if (Ptext.at(a+1)->ch == QChar(29))
							chx3 = " ";
						else
							chx3 = Ptext.at(a+1)->ch;
						wide = Cwidth(Doc, hl->cfont, chx2, chs, chx3);
					}
					else
						wide = Cwidth(Doc, hl->cfont, chx2, chs);
					if (DropCmode)
					{
						wide = RealCWidth(Doc, hl->cfont, chx2, chsd);
						desc2 = 0;
						desc = 0;
						asce = qRound((*Doc->AllFonts)[hl->cfont]->numAscent * (chs / 10.0));
					}
					else
					{
						desc2 = -(*Doc->AllFonts)[hl->cfont]->numDescender * (chs / 10.0);
						desc = qRound(-(*Doc->AllFonts)[hl->cfont]->numDescender * (chs / 10.0));
						asce = qRound((*Doc->AllFonts)[hl->cfont]->numAscent * (chs / 10.0));
					}
					wide = wide * (hl->cscale / 100.0);
					fBorder = false;
					if (LiList.isEmpty())
					{
						startLin = a;
						double TopOffset = asce;
						double BotOffset = desc2;
						goNoRoom = false;
						if (StartOfCol)
						{
							CurY = asce+TExtra+lineCorr+1;
							if (((a > 0) && (Ptext.at(a-1)->ch == QChar(13))) || (a == 0))
								CurY += Doc->Vorlagen[hl->cab].Avor;
						}
						if (Doc->Vorlagen[hl->cab].BaseAdj)
						{
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - Doc->Pages.at(OwnPage)->Yoffset;
							int ol1 = qRound((by + CurY - Doc->typographicSetttings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / Doc->typographicSetttings.valueBaseGrid);
							CurY = ceil(  ol2 / 10000.0 ) * Doc->typographicSetttings.valueBaseGrid + Doc->typographicSetttings.offsetBaseGrid - by;
						}
						if (CurY-TopOffset < 0.0)
							CurY = TopOffset+1;
						pt1 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(CurY+BotOffset));
						pt2 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(ceil(CurY-TopOffset)));
						while ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))))
						{
							fBorder = true;
							CurX++;
							if (CurX+RExtra+lineCorr > ColBound.y())
							{
								fBorder = false;
								if (StartOfCol)
									CurY++;
								else
									CurY += Doc->Vorlagen[hl->cab].LineSpa;
								if (Doc->Vorlagen[hl->cab].BaseAdj)
								{
									double by = Ypos;
									if (OwnPage != -1)
										by = Ypos - Doc->Pages.at(OwnPage)->Yoffset;
									int ol1 = qRound((by + CurY - Doc->typographicSetttings.offsetBaseGrid) * 10000.0);
									int ol2 = static_cast<int>(ol1 / Doc->typographicSetttings.valueBaseGrid);
									CurY = ceil(  ol2 / 10000.0 ) * Doc->typographicSetttings.valueBaseGrid + Doc->typographicSetttings.offsetBaseGrid - by;
								}
								CurX = ColBound.x();
								if (CurY+BExtra+lineCorr > Height)
								{
									StartOfCol = true;
									fBorder = false;
									CurrCol++;
									if (CurrCol < Cols)
									{
										ColWidth = (Width - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
										ColBound = FPoint((ColWidth + ColGap) * CurrCol + Extra+lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
										CurX = ColBound.x();
										ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
										CurY = asce+TExtra+lineCorr+1;
										if ((a > 0) && (Ptext.at(a-1)->ch == QChar(13)))
										{
											if (chx != QChar(13))
												DropCmode = Doc->Vorlagen[hl->cab].Drop;
											else
												DropCmode = false;
											CurX += Doc->Vorlagen[hl->cab].First;
											CurX += Doc->Vorlagen[hl->cab].Indent;
											if (DropCmode)
											{
												if (Doc->Vorlagen[hl->cab].BaseAdj)
													desc2 = -(*Doc->AllFonts)[hl->cfont]->numDescender * Doc->typographicSetttings.valueBaseGrid * Doc->Vorlagen[hl->cab].DropLin;
												else
													desc2 = -(*Doc->AllFonts)[hl->cfont]->numDescender * Doc->Vorlagen[hl->cab].LineSpa * Doc->Vorlagen[hl->cab].DropLin;
											}
											CurY += Doc->Vorlagen[hl->cab].Avor;
											if (DropCmode)
												DropLines = Doc->Vorlagen[hl->cab].DropLin;
										}
										else
											CurX += Doc->Vorlagen[hl->cab].Indent;
										if (Doc->Vorlagen[hl->cab].BaseAdj)
										{
											double by = Ypos;
											if (OwnPage != -1)
												by = Ypos - Doc->Pages.at(OwnPage)->Yoffset;
											int ol1 = qRound((by + CurY - Doc->typographicSetttings.offsetBaseGrid) * 10000.0);
											int ol2 = static_cast<int>(ol1 / Doc->typographicSetttings.valueBaseGrid);
											CurY = ceil(  ol2 / 10000.0 ) * Doc->typographicSetttings.valueBaseGrid + Doc->typographicSetttings.offsetBaseGrid - by;
										}
									}
									else
									{
										nrc = a;
										goto NoRoom;
									}
								}
							}
							pt1 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(CurY+BotOffset));
							pt2 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(ceil(CurY-TopOffset)));
						}
						if ((fBorder) && (!AbsHasDrop))
							CurX += Extra;
						fBorder = false;
					}
					StartOfCol = false;
					if (RTab)
					{
						if (((hl->ch == ".") && (TabCode == 2)) ||
							((hl->ch == ",") && (TabCode == 3)) ||
							(hl->ch == QChar(9)))
						{
							RTab = false;
							TabCode = 0;
						}
					}
					if (hl->ch == QChar(9))
					{
						wide = 1;
						if (RTab)
							RTab = false;
						else
						{
							RTabX = CurX+wide;
							if (hl->cab < 5)
								tTabValues = TabValues;
							else
								tTabValues = Doc->Vorlagen[hl->cab].TabValues;
							if (tTabValues.isEmpty())
							{
								if ((CurX - ColBound.x()) != 0)
									CurX = ColBound.x() + ceil((CurX-ColBound.x()) / 36.0) * 36.0;
								else
									CurX = ColBound.x() + 36.0;
								TabCode = 0;
								RTab = false;
							}
							else
							{
								double tCurX = CurX - ColBound.x();
								double oCurX = CurX - ColBound.x();
								for (int yg = static_cast<int>(tTabValues.count()-1); yg > 0; yg -= 2)
								{
									if (oCurX <= tTabValues[yg])
									{
										tCurX = tTabValues[yg];
										TabCode = static_cast<int>(tTabValues[yg-1]);
									}
								}
								if (TabCode == 0)
									RTab = false;
								else
									RTab = true;
								if (tCurX == oCurX)
									CurX = ColBound.x() + ceil((CurX-ColBound.x()) / 36.0) * 36.0;
								else
									CurX = ColBound.x() + tCurX;
							}
							CurX -= 1;
							StartRT = LiList.count();
							StartRT2 = a;
						}
					}
					hl->yp = CurY + oldCurY;
					if (LiList.count() == 0)
						kernVal = 0;
					else
						kernVal = hl->cextra;
					if (!RTab)
					{
						hl->xp = CurX+kernVal;
						CurX += wide+kernVal;
					}
					else
						hl->xp = CurX;
					if ((TabCode == 4) && (RTab))
						CurX += (wide+kernVal) / 2;
					pt1 = QPoint(static_cast<int>(ceil(CurX+RExtra)), static_cast<int>(CurY+desc));
					pt2 = QPoint(static_cast<int>(ceil(CurX+RExtra)), static_cast<int>(ceil(CurY-asce)));
					if ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))) || (CurX+RExtra+lineCorr > ColBound.y()))
						outs = true;
					if (CurY > (Height - BExtra - lineCorr))
						outs = true;
					Zli = new ZZ;
					Zli->Zeich = chx;
					Zli->Farb = hl->ccolor;
					Zli->shade = hl->cshade;
					Zli->Farb2 = hl->cstroke;
					Zli->shade2 = hl->cshade2;
					Zli->xco = hl->xp;
					Zli->yco = hl->yp;
					Zli->Sele = hl->cselect;
					if (DropCmode)
						Zli->Siz = chsd;
					else
						Zli->Siz = chs;
					Zli->Style = hl->cstyle;
					Zli->ZFo = hl->cfont;
					Zli->wide = wide;
					Zli->kern = kernVal;
					Zli->scale = hl->cscale;
					if (((hl->ch == " ") || (hl->ch == QChar(9))) && (!outs))
					{
						if (a > 0)
						{
							if (Ptext.at(a-1)->ch !=  " ")   // || ((Doc->AppMode == 7) && (Ptext.at(QMAX(CPos-1,0))->yp == hl->yp) && (Select)))
							{
							LastXp = hl->xp;
							LastSP = BuPos;
							}
						}
						else
						{
							LastXp = hl->xp;
							LastSP = BuPos;
						}
					}
					if (((hl->cstyle & 128) || (hl->ch == "-")) && (!outs))
					{
						if ((HyphenCount < Doc->HyCount) || (Doc->HyCount == 0))
						{
							if (hl->ch == "-")
								LastXp = CurX;
							else
								LastXp = CurX + Cwidth(Doc, hl->cfont, "-", hl->csize);
							LastSP = BuPos;
						}
					}
					LiList.append(Zli);
					if (RTab)
					{
						uint rtx2 = 0;
						double cen = 1;
						if (TabCode == 4)
							cen = 2;
						for (uint rtx = StartRT; rtx < LiList.count(); ++rtx)
						{
								LiList.at(rtx)->xco = QMAX(LiList.at(rtx)->xco-(wide+kernVal) / cen, 0.0);
								Ptext.at(StartRT2+rtx2)->xp = QMAX(Ptext.at(StartRT2+rtx2)->xp-(wide+kernVal) / cen, 0.0);
								if (Ptext.at(StartRT2+rtx2)->xp < RTabX)
								{
									RTab = false;
									TabCode = 0;
								}
								rtx2++;
						}
					}
					BuPos++;
					if (DropCmode)
					{
						DropCmode = false;
						AbsHasDrop = true;
						maxDY = CurY;
						maxDX = CurX;
						QPointArray tcli;
						tcli.resize(4);
						if (Doc->Vorlagen[hl->cab].BaseAdj)
						{
							CurY -= Doc->typographicSetttings.valueBaseGrid * (DropLines-1);
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - Doc->Pages.at(OwnPage)->Yoffset;
							int ol1 = qRound((by + CurY - Doc->typographicSetttings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / Doc->typographicSetttings.valueBaseGrid);
							CurY = ceil(  ol2 / 10000.0 ) * Doc->typographicSetttings.valueBaseGrid + Doc->typographicSetttings.offsetBaseGrid - by;
							tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*Doc->typographicSetttings.valueBaseGrid)));
							tcli.setPoint(1, QPoint(qRound(hl->xp+wide), qRound(maxDY-DropLines*Doc->typographicSetttings.valueBaseGrid)));
						}
						else
						{
							CurY -= Doc->Vorlagen[absa].LineSpa * (DropLines-1);
							tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*Doc->Vorlagen[absa].LineSpa)));
							tcli.setPoint(1, QPoint(qRound(hl->xp+wide), qRound(maxDY-DropLines*Doc->Vorlagen[absa].LineSpa)));
						}
						tcli.setPoint(2, QPoint(qRound(hl->xp+wide), qRound(maxDY)));
						tcli.setPoint(3, QPoint(qRound(hl->xp), qRound(maxDY)));
						cm = QRegion(pf2.xForm(tcli));
						cl = cl.subtract(cm);
					}
					if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (outs))
					{
						RTab = false;
						TabCode = 0;
						if (outs)
						{
							if (LastSP != 0)            // Hier koenen auch andere Trennungen eingebaut werden
							{
								a -= BuPos - LastSP;
								a++;
								if (Ptext.at(a)->cstyle & 128)
								{
									HyphenCount++;
									Ptext.at(a)->cstyle |= 512;
									Zli = new ZZ;
									Zli->Zeich = "-";
									Zli->Farb = Ptext.at(a)->ccolor;
									Zli->Farb2 = Ptext.at(a)->cstroke;
									Zli->shade = Ptext.at(a)->cshade;
									Zli->shade2 = Ptext.at(a)->cshade2;
									Zli->xco = LastXp - Cwidth(Doc, Ptext.at(a)->cfont, "-", Ptext.at(a)->csize);
									Zli->yco = Ptext.at(a)->yp;
									Zli->Sele = Ptext.at(a)->cselect;
									Zli->Siz = Ptext.at(a)->csize;
									Zli->Style = Ptext.at(a)->cstyle;
									Zli->ZFo = Ptext.at(a)->cfont;
									Zli->wide = Cwidth(Doc, Ptext.at(a)->cfont, "-", Ptext.at(a)->csize);
									Zli->kern = Ptext.at(a)->cextra;
									Zli->scale = Ptext.at(a)->cscale;
									LiList.insert(LastSP+1, Zli);
									LastSP += 1;
								}
								else
								{
									HyphenCount = 0;
									hl->cstyle &= 511;
								}
								BuPos = LastSP+1;
								if (Doc->Vorlagen[absa].Ausri != 0)
								{
									EndX = LastXp;
									do
									{
										pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
										pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
										EndX++;
									}
									while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
									if (Doc->Vorlagen[absa].Ausri == 2)
										OFs = EndX - LastXp;
									if (Doc->Vorlagen[absa].Ausri == 1)
										OFs = (EndX - LastXp) / 2;
									if ((Doc->Vorlagen[absa].Ausri == 3) || (Doc->Vorlagen[absa].Ausri == 4))
									{
										aSpa = 0;
										for (uint sof = 0; sof<BuPos-1; ++sof)
										{
											if ((LiList.at(sof)->Zeich == QChar(32)) || (LiList.at(sof)->Zeich == QChar(29)))
												aSpa++;
										}
										if (aSpa > 1)
											OFs2 = (EndX - LastXp) / aSpa;
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
											if ((LiList.at(yof)->Zeich == QChar(32)) || (LiList.at(yof)->Zeich == QChar(29)))
												OFs += OFs2;
										}
									}
									else
									{
										for (uint xof = 0; xof<LiList.count(); ++xof)
										{
											LiList.at(xof)->xco += OFs;
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
									pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
									pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
									EndX++;
								}
								while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
								if (Doc->Vorlagen[absa].Ausri == 2)
									OFs = EndX - CurX;
								if (Doc->Vorlagen[absa].Ausri == 1)
									OFs = (EndX - CurX) / 2;
								if (Doc->Vorlagen[absa].Ausri == 3)
									OFs = 0;
								if (Doc->Vorlagen[absa].Ausri == 4)
								{
									aSpa = 0;
									for (uint sof = 0; sof<LiList.count(); ++sof)
									{
										if ((LiList.at(sof)->Zeich == QChar(32)) || (LiList.at(sof)->Zeich == QChar(29)))
											aSpa++;
									}
									if (aSpa != 0)
										OFs2 = (EndX - CurX) / aSpa;
									else
										OFs2 = 0;
									OFs = 0;
									for (uint yof = 0; yof < LiList.count(); ++yof)
									{
										LiList.at(yof)->xco += OFs;
										if ((LiList.at(yof)->Zeich == QChar(32)) || (LiList.at(yof)->Zeich == QChar(29)))
											OFs += OFs2;
									}
								}
								else
								{
									for (uint xof = 0; xof<LiList.count(); ++xof)
									{
										LiList.at(xof)->xco += OFs;
									}
								}
							}
						}
						uint BuPos3 = BuPos;
						if ((outs) || (hl->ch == QChar(13)) || (hl->ch == QChar(28)))
						{
							if ((outs) && (CurX+RExtra+lineCorr < ColBound.y()))
							{
								if (((hl->ch == QChar(13)) || (hl->ch == QChar(28))) && (AbsHasDrop))
								{
									AbsHasDrop = false;
									if (CurY < maxDY)
										CurY = maxDY;
								}
								bool fromOut = true;
								double BotOffset = desc+BExtra+lineCorr;
								pt1 = QPoint(qRound(CurX+RExtra), static_cast<int>(CurY+BotOffset));
								pt2 = QPoint(qRound(CurX+RExtra), static_cast<int>(ceil(CurY-asce)));
								while ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))))
								{
									CurX++;
									if (CurX+RExtra+lineCorr > ColBound.y())
									{
										fromOut = false;
										CurY += Doc->Vorlagen[hl->cab].LineSpa;
										if ((CurY+desc+BExtra+lineCorr > Height) && (CurrCol+1 == Cols))
										{
											goNoRoom = true;
											break;
										}
										if (AbsHasDrop)
										{
											if (CurY > maxDY)
											{
												AbsHasDrop = false;
												CurX = ColBound.x();
											}
											else
												CurX = maxDX;
										}
										else
											CurX = ColBound.x();
										if (hl->ch != QChar(13))
											CurX += Doc->Vorlagen[hl->cab].Indent;
										else
										{
											CurY += Doc->Vorlagen[hl->cab].Anach;
											if (BuPos3 > 0)
												BuPos3 -= 1;
										}
										break;
									}
									pt1 = QPoint(qRound(CurX+RExtra), static_cast<int>(CurY+BotOffset));
									pt2 = QPoint(qRound(CurX+RExtra), static_cast<int>(ceil(CurY-asce)));
								}
								if (fromOut)
									CurX--;
							}
							else
							{
								if (((hl->ch == QChar(13)) || (hl->ch == QChar(28))) && (AbsHasDrop))
								{
									AbsHasDrop = false;
									if (CurY < maxDY)
										CurY = maxDY;
								}
								CurY += Doc->Vorlagen[hl->cab].LineSpa;
								if (AbsHasDrop)
								{
									if (CurY > maxDY)
									{
										AbsHasDrop = false;
										CurX = ColBound.x();
									}
									else
										CurX = maxDX;
								}
								else
									CurX = ColBound.x();
								if (hl->ch != QChar(13))
									CurX += Doc->Vorlagen[hl->cab].Indent;
								else
								{
									CurY += Doc->Vorlagen[hl->cab].Anach;
									if (BuPos3 > 0)
										BuPos3 -= 1;
								}
							}
						}
						hl->xp = CurX;
						hl->yp = CurY;
						LiList.at(LiList.count()-1)->xco = hl->xp;
						LiList.at(LiList.count()-1)->yco = hl->yp;
						for (uint zc = 0; zc<BuPos3; ++zc)
						{
							double wide2 = 0;
							Zli2 = LiList.at(zc);
							double xcoZli = Zli2->xco;
							Ptext.at(startLin+zc)->xp = Zli2->xco;
							Ptext.at(startLin+zc)->yp = Zli2->yco;
							if (Zli2->Farb != "None")
							{
								SetFarbe(&tmp, Zli2->Farb, Zli2->shade);
								p->setBrush(tmp);
							}
							if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (Doc->AppMode == 7))
							{
								wide = Zli2->wide;
								desc = static_cast<int>((*Doc->AllFonts)[Zli2->ZFo]->numDescender * (-Zli2->Siz / 10.0));
								asce = static_cast<int>((*Doc->AllFonts)[Zli2->ZFo]->numAscent * (Zli2->Siz / 10.0));
								p->setFillMode(1);
								p->setBrush(darkBlue);
								p->setLineWidth(0);
								if ((zc > 0) && (Zli2->Zeich == QChar(9)))
								{
									wide2 = LiList.at(zc-1)->wide;
									xcoZli = LiList.at(zc-1)->xco+wide2;
									wide = Zli2->xco - xcoZli + Zli2->wide;
								}
								if (!Doc->RePos)
									p->drawRect(xcoZli, Zli2->yco-asce, wide+1, asce+desc);
								p->setBrush(white);
							}
							if (Zli2->Farb2 != "None")
							{
								SetFarbe(&tmp, Zli2->Farb2, Zli2->shade2);
								p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
							}
							if (!Doc->RePos)
							{
								if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-LineSp), qRound(Zli2->wide+1), qRound(LineSp*1.5)))))
									DrawZeichenS(p, Zli2);
							}
						}
						LiList.clear();
						BuPos = 0;
						LastSP = 0;
						LastXp = 0;
						outs = false;
						if (goNoRoom)
						{
							goNoRoom = false;
							nrc = a+1;
							goto NoRoom;
						}
					}
				}
				if (Doc->Vorlagen[absa].Ausri != 0)
				{
					EndX = CurX;
					do
					{
						pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
						pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
						EndX++;
					}
					while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
					if (Doc->Vorlagen[absa].Ausri == 2)
						OFs = EndX - CurX;
					if (Doc->Vorlagen[absa].Ausri == 1)
						OFs = (EndX - CurX) / 2;
					if (Doc->Vorlagen[absa].Ausri == 3)
						OFs = 0;
					if (Doc->Vorlagen[absa].Ausri == 4)
					{
						aSpa = 0;
						for (uint sof = 0; sof<LiList.count(); ++sof)
						{
							if ((LiList.at(sof)->Zeich == QChar(32)) || (LiList.at(sof)->Zeich == QChar(29)))
								aSpa++;
						}
						if (aSpa != 0)
							OFs2 = (EndX - CurX) / aSpa;
						else
							OFs2 = 0;
						OFs = 0;
						for (uint yof = 0; yof < LiList.count(); ++yof)
						{
							LiList.at(yof)->xco += OFs;
							if ((LiList.at(yof)->Zeich == QChar(32)) || (LiList.at(yof)->Zeich == QChar(29)))
								OFs += OFs2;
						}
					}
					else
					{
						for (uint xof = 0; xof<LiList.count(); ++xof)
						{
							LiList.at(xof)->xco += OFs;
						}
					}
				}
				for (uint zc = 0; zc<LiList.count(); ++zc)
				{
					double wide2 = 0;
					Zli2 = LiList.at(zc);
					double xcoZli = Zli2->xco;
					Ptext.at(startLin+zc)->xp = Zli2->xco;
					Ptext.at(startLin+zc)->yp = Zli2->yco;
					if (Zli2->Farb != "None")
					{
						SetFarbe(&tmp, Zli2->Farb, Zli2->shade);
						p->setBrush(tmp);
					}
					if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (Doc->AppMode == 7))
					{
						wide = Zli2->wide;
						desc = qRound((*Doc->AllFonts)[Zli2->ZFo]->numDescender * (-Zli2->Siz / 10.0));
						asce = qRound((*Doc->AllFonts)[Zli2->ZFo]->numAscent * (Zli2->Siz / 10.0));
						p->setFillMode(1);
						p->setBrush(darkBlue);
						p->setLineWidth(0);
						if ((zc > 0) && (Zli2->Zeich == QChar(9)))
						{
							wide2 = LiList.at(zc-1)->wide;
							xcoZli = LiList.at(zc-1)->xco+wide2;
							wide = Zli2->xco - xcoZli + Zli2->wide;
						}
						if (!Doc->RePos)
							p->drawRect(xcoZli, Zli2->yco-asce, wide+1, asce+desc);
						p->setBrush(white);
					}
					if (Zli2->Farb2 != "None")
					{
						SetFarbe(&tmp, Zli2->Farb2, Zli2->shade2);
						p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
					}
					if (!Doc->RePos)
					{
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-LineSp), qRound(Zli2->wide+1), qRound(LineSp*1.5)))))
							DrawZeichenS(p, Zli2);
					}
				}
				LiList.clear();
				BuPos = 0;
				LastSP = 0;
				outs = false;
			}
			MaxChars = Ptext.count();
			Redrawn = true;
			pf2.end();
			p->restore();
			break;
NoRoom: pf2.end();
				if (NextBox != 0)
				{
					nrc2 = Ptext.count();
					for (uint ss=nrc; ss<nrc2; ++ss)
					{
						NextBox->Ptext.append(Ptext.take(nrc));
					}
					if (uint(CPos) > nrc)
					{
						int nCP = CPos - nrc;
						CPos = nrc;
						if ((Doc->AppMode == 7) && (Tinput))
						{
//							OwnPage->Deselect(true);
							NextBox->CPos = QMAX(nCP, 1);
//							Doc->ActPage = NextBox->OwnPage;
//							NextBox->OwnPage->SelectItemNr(NextBox->ItemNr);
							break;
						}
					}
					p->save();
					bool rep = Doc->RePos;
					Doc->RePos = true;
					NextBox->Dirty = false;
					QPixmap pgPix(1, 1);
					ScPainter *painter = new ScPainter(&pgPix, 1, 1);
					painter->translate(0.5, 0.5);
					NextBox->DrawObj(painter, QRect(0, 0, 1, 1));
					NextBox->Dirty = true;
					painter->end();
					delete painter;
					p->restore();
					Doc->RePos = rep;
				}
				else
				{
					if (!Doc->RePos)
					{
						double scp1 = 1.0/QMAX(ScApp->view->Scale, 1);
						double scp16 = 16.0*scp1;
						double scp14 = 14.0*scp1;
						double scp3 = 3.0*scp1;
						double scpwidth16 = Width - scp16;
						double scpheight16 = Height - scp16;
						double scpwidth3 = Width - scp3;
						double scpheight3 = Height - scp3;
						p->setPen(black, scp1, SolidLine, FlatCap, MiterJoin);
						p->setBrush(white);
						p->drawRect(scpwidth16, scpheight16, scp14, scp14);
						p->drawLine(FPoint(scpwidth16, scpheight16), FPoint(scpwidth3, scpheight3));
						p->drawLine(FPoint(scpwidth16, scpheight3), FPoint(scpwidth3, scpheight16));
					}
				}
				MaxChars = nrc;
				Redrawn = true;
				p->restore();
				break;
		case 8:
			if (!PoShow)
				doStroke = false;
			cl = FlattenPath(PoLine, Segments);
			CurX = Extra;
			if (Ptext.count() != 0)
				CurX += Ptext.at(0)->cextra;
			zae = 0;
			wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2.0)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2.0));
			while (wid < 1)
			{
				zae++;
				if (zae > cl.size()-2)
				{
					MaxChars = Ptext.count();
					break;
				}
				wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2.0) + pow(cl.point(zae+1).y()-cl.point(zae).y(),2.0));
			}
			rota = xy2Deg(cl.point(zae+1).x()-cl.point(zae).x(),cl.point(zae+1).y()-cl.point(zae).y());
			for (a = 0; a < Ptext.count(); ++a)
			{
				CurY = 0;
				hl = Ptext.at(a);
				chx = hl->ch;
				if ((chx == QChar(30)) || (chx == QChar(13)) || (chx == QChar(9)) || (chx == QChar(28)))
					continue;
				chs = hl->csize;
				SetZeichAttr(hl, &chs, &chx);
				if (chx == QChar(29))
					chx2 = " ";
				else
					chx2 = chx;
				if (a < Ptext.count()-1)
				{
					if (Ptext.at(a+1)->ch == QChar(29))
						chx3 = " ";
					else
						chx3 = Ptext.at(a+1)->ch;
					wide = Cwidth(Doc, hl->cfont, chx2, chs, chx3);
				}
				else
					wide = Cwidth(Doc, hl->cfont, chx2, chs);
				wide = wide * (hl->cscale / 100.0);
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
								if (zae > cl.size()-2)
								{
									MaxChars = a+1;
									break;
								}
								wid = sqrt(pow(cl.point(zae+1).x()-cl.point(zae).x(),2.0)+pow(cl.point(zae+1).y()-cl.point(zae).y(),2.0));
								rota = xy2Deg(cl.point(zae+1).x()-cl.point(zae).x(),cl.point(zae+1).y()-cl.point(zae).y());
							}
							while (wid == 0);
							EndX -= wid;
							if (zae > cl.size()-2)
							{
								MaxChars = a+1;
								break;
							}
						}
						while (wid < EndX);
						CurX = EndX + wid;
					}
					else
					{
						MaxChars = a+1;
						break;
					}
				}
				if (zae > cl.size()-2)
					break;
				p->save();
				p->translate(cl.point(zae).x()*sc, cl.point(zae).y()*sc);
				p->rotate(rota);
				hl->xp = CurX+hl->cextra;
				hl->yp = CurY+BaseOffs;
				hl->PtransX = cl.point(zae).x();
				hl->PtransY = cl.point(zae).y();
				hl->PRot = rota;
				Zli = new ZZ;
				Zli->Zeich = chx;
				if (hl->ccolor != "None")
				{
					SetFarbe(&tmp, hl->ccolor, hl->cshade);
					p->setBrush(tmp);
				}
				if (hl->cstroke != "None")
				{
					SetFarbe(&tmp, hl->cstroke, hl->cshade2);
					p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
				}
				Zli->Farb = hl->ccolor;
				Zli->Farb2 = hl->cstroke;
				Zli->shade = hl->cshade;
				Zli->shade2 = hl->cshade2;
				Zli->xco = hl->xp;
				Zli->yco = hl->yp;
				Zli->Sele = hl->cselect;
				Zli->Siz = chs;
				Zli->Style = hl->cstyle;
				Zli->ZFo = hl->cfont;
				Zli->wide = wide;
				Zli->kern = hl->cextra;
				Zli->scale = hl->cscale;
				if (!Doc->RePos)
					DrawZeichenS(p, Zli);
				delete Zli;
				p->restore();
				p->setZoomFactor(sc);
				MaxChars = a+1;
				CurX += wide+hl->cextra;
			}
		default:
			break;
	}
	if ((doStroke) && (!Doc->RePos))
	{
		if (Pcolor2 != "None")
		{
			SetFarbe(&tmp, Pcolor2, Shade2);
			p->setPen(tmp, Pwidth, PLineArt, PLineEnd, PLineJoin);
			if (DashValues.count() != 0)
				p->setDash(DashValues, DashOffset);
		}
		else
			p->setLineWidth(0);
		if (!isTableItem)
		{
			p->setupPolygon(&PoLine);
			if (NamedLStyle == "")
				p->strokePath();
			else
			{
				multiLine ml = Doc->MLineStyles[NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(tmp, ml[it].Width,
									 static_cast<PenStyle>(ml[it].Dash),
									 static_cast<PenCapStyle>(ml[it].LineEnd),
									 static_cast<PenJoinStyle>(ml[it].LineJoin));
					p->strokePath();
				}
			}
		}
	}
	if ((!Tinput) && (!Doc->RePos))
	{
		double scp = QMAX(ScApp->view->Scale, 1);
		if ((Frame) && (Doc->guidesSettings.framesShown) && ((PType == 2) || (PType == 4)))
		{
			p->setPen(black, 1 / scp, DotLine, FlatCap, MiterJoin);
			if ((isBookmark) || (isAnnotation))
				p->setPen(blue, 1 / scp, DotLine, FlatCap, MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(red, 1 / scp, SolidLine, FlatCap, MiterJoin);
			if (Locked)
				p->setPen(darkRed, 1 / scp, SolidLine, FlatCap, MiterJoin);
			p->setFillMode(0);
			p->setupPolygon(&PoLine);
			p->strokePath();
		}
		if ((Doc->guidesSettings.framesShown) && (UseContour) && (ContourLine.size() != 0))
		{
			p->setPen(lightGray, 1 / scp, DotLine, FlatCap, MiterJoin);
			p->setupPolygon(&ContourLine);
			p->strokePath();
		}
	}
	Tinput = false;
	FrameOnly = false;
	p->restore();
	pf.end();
}

void PageItem::paintObj(QRect e, QPixmap *ppX)
{
	QPainter p;
	double sc = ScApp->view->Scale;
	if (!Doc->DoDrawing)
	{
		Redrawn = true;
		Tinput = false;
		FrameOnly = false;
		return;
	}
	if (toPixmap)
		p.begin(ppX);
	else
		p.begin(ScApp->view->viewport());
	if ((!toPixmap) && (!Doc->RePos))
	{
		if (!e.isEmpty())
			p.setClipRect(e);
		else
		{
			int x = ScApp->view->contentsX();
			int y = ScApp->view->contentsY();
			QPoint out = ScApp->view->contentsToViewport(QPoint(x,y));
			p.setClipRect(QRect(out.x(), out.y(), ScApp->view->visibleWidth(), ScApp->view->visibleWidth()));
		}
	}
	QPoint in  = QPoint(qRound(Xpos*sc), qRound(Ypos*sc));
	QPoint out = ScApp->view->contentsToViewport(in);
	p.translate(out.x(), out.y());
	p.scale(sc, sc);
	p.rotate(Rot);
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
		if (Select) // && (!Doc->EditClip))
		{
			if (!ScApp->view->SelItem.isEmpty())
			{
				if (Groups.count() == 0)
				{
					QPainter pr;
					pr.begin(ScApp->view->viewport());
					pr.translate(out.x(), out.y());
					pr.rotate(Rot);
					if (Locked)
						pr.setPen(QPen(darkRed, 1, SolidLine, FlatCap, MiterJoin));
					else
						pr.setPen(QPen(red, 1, DotLine, FlatCap, MiterJoin));
					pr.setBrush(NoBrush);
					pr.drawRect(-1, -1, static_cast<int>(Width*sc)+2, static_cast<int>(Height*sc)+2);
					pr.setPen(QPen(red, 1, SolidLine, FlatCap, MiterJoin));
					pr.setBrush(red);
					if ((!Locked) && (!LockRes))
					{
						if (PType != 5)
						{
							pr.drawRect(-1, -1, 6, 6);
							pr.drawRect(static_cast<int>(Width*sc), static_cast<int>(Height*sc), -6, -6);
							pr.drawRect(static_cast<int>(Width*sc), -1, -6, 6);
							pr.drawRect(-1, static_cast<int>(Height*sc), 6, -6);
							if (Width > 6)
							{
								pr.drawRect(static_cast<int>(Width/2*sc - 3), static_cast<int>(Height*sc), 6, -6);
								pr.drawRect(static_cast<int>(Width/2*sc - 3), -1, 6, 6);
							}
							if (Height > 6)
							{
								pr.drawRect(static_cast<int>(Width*sc), static_cast<int>(Height/2*sc - 3), -6, 6);
								pr.drawRect(-1, static_cast<int>(Height/2*sc - 3), 6, 6);
							}
						}
						else
						{
							pr.drawRect(-3, -3, 6, 6);
							pr.drawRect(static_cast<int>(Width*sc)+3, -3, -6, 6);
						}
					}
					pr.end();
				}
				else
				{
					p.setPen(QPen(darkCyan, 1, DotLine, FlatCap, MiterJoin));
					p.setBrush(NoBrush);
					p.drawRect(-1, -1, static_cast<int>(Width+2), static_cast<int>(Height+2));
					if (ScApp->view->SelItem.count() == 1)
					{
						QPainter pr;
						pr.begin(ScApp->view->viewport());
						pr.translate(out.x(), out.y());
						pr.rotate(Rot);
						pr.setPen(QPen(darkCyan, 1, SolidLine, FlatCap, MiterJoin));
						pr.setBrush(darkCyan);
						pr.drawRect(-1, -1, 6, 6);
						pr.drawRect(static_cast<int>(Width*sc), static_cast<int>(Height*sc), -6, -6);
						pr.drawRect(static_cast<int>(Width*sc), -1, -6, 6);
						pr.drawRect(-1, static_cast<int>(Height*sc), 6, -6);
						if (Width > 6)
						{
							pr.drawRect(static_cast<int>(Width/2*sc - 3), static_cast<int>(Height*sc), 6, -6);
							pr.drawRect(static_cast<int>(Width/2*sc - 3), -1, 6, 6);
						}
						if (Height > 6)
						{
							pr.drawRect(static_cast<int>(Width*sc), static_cast<int>(Height/2*sc - 3), -6, 6);
							pr.drawRect(-1, static_cast<int>(Height/2*sc - 3), 6, 6);
						}
						pr.end();
					}
				}
			}
		}
	}
	Tinput = false;
	FrameOnly = false;
	p.end();
}

QString PageItem::ExpandToken(uint base)
{
	uint zae = 0;
	uint za2 = base;
	QString chx = "#";
	if ((!Doc->MasterP) && (OwnPage != -1))
	{
		do
		{
			if (za2 == 0)
				break;
			za2--;
		}
		while (Ptext.at(za2)->ch == QChar(30));
		if (Ptext.at(za2)->ch != QChar(30))
			za2++;
		while (Ptext.at(za2+zae)->ch == QChar(30))
		{
			zae++;
			if (za2+zae == Ptext.count())
				break;
		}
		QString out="%1";
		QString out2;
		out2 = out.arg(OwnPage+Doc->FirstPnum, -zae);
		chx = out2.mid(base-za2, 1);
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

double PageItem::SetZeichAttr(struct Pti *hl, int *chs, QString *chx)
{
	double retval = 0.0;
	double asce = (*Doc->AllFonts)[hl->cfont]->numAscent * (hl->csize / 10.0);
	int chst = hl->cstyle & 127;
	if (chst != 0)
	{
		if (chst & 1)
		{
			retval -= asce * Doc->typographicSetttings.valueSuperScript / 100;
			*chs = QMAX(static_cast<int>(hl->csize * Doc->typographicSetttings.scalingSuperScript / 100), 1);
		}
		if (chst & 2)
		{
			retval += asce * Doc->typographicSetttings.valueSubScript / 100;
			*chs = QMAX(static_cast<int>(hl->csize * Doc->typographicSetttings.scalingSubScript / 100), 1);
		}
		if (chst & 64)
		{
			if (chx->upper() != *chx)
			{
				*chs = QMAX(static_cast<int>(hl->csize * Doc->typographicSetttings.valueSmallCaps / 100), 1);
				*chx = chx->upper();
			}
		}
	}
	return retval;
}

void PageItem::DrawZeichenS(ScPainter *p, struct ZZ *hl)
{
	QString ccx = hl->Zeich;
	if ((ccx == QChar(13)) || (ccx == QChar(9)) || (ccx == QChar(28)))
		return;
	if (ccx == QChar(29))
		ccx = " ";
	double csi = static_cast<double>(hl->Siz) / 100.0;
	uint chr = ccx[0].unicode();
	if ((*Doc->AllFonts)[hl->ZFo]->CharWidth.contains(chr))
	{
		QWMatrix chma, chma2, chma3, chma4, chma5;
		chma.scale(csi, csi);
		chma5.scale(p->zoomFactor(), p->zoomFactor());
		FPointArray gly = (*Doc->AllFonts)[hl->ZFo]->GlyphArray[chr].Outlines.copy();
		if (gly.size() > 3)
		{
			chma2.scale(hl->scale / 100.0, 1);
			if (Reverse)
			{
				chma3.scale(-1, 1);
				chma3.translate(-hl->wide, 0);
				chma4.translate(hl->xco, hl->yco-(hl->Siz / 10.0));
			}
			else
				chma4.translate(hl->xco, hl->yco-(hl->Siz / 10.0));
			gly.map(chma * chma2 * chma3 * chma4 * chma5);
			p->setFillMode(1);
			p->setupTextPolygon(&gly);
			if ((*Doc->AllFonts)[hl->ZFo]->isStroked)
			{
				QColor tmp = p->brush();
				p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
				p->setLineWidth(QMAX((*Doc->AllFonts)[hl->ZFo]->strokeWidth * (hl->Siz / 10.0) / 2, 1));
				p->strokePath();
			}
			else
			{
				if (hl->Farb != "None")
					p->fillPath();
				if ((hl->Style & 4) && (hl->Farb2 != "None"))
				{
					p->setLineWidth(QMAX((*Doc->AllFonts)[hl->ZFo]->strokeWidth * (hl->Siz / 10.0) / 2, 1));
					p->strokePath();
				}
			}
		}
		if (hl->Style & 16)
		{
			p->setPen(p->brush());
			double st = (*Doc->AllFonts)[hl->ZFo]->strikeout_pos * (hl->Siz / 10.0);
			p->setLineWidth(QMAX((*Doc->AllFonts)[hl->ZFo]->strokeWidth * (hl->Siz / 10.0), 1));
			p->drawLine(FPoint(hl->xco-hl->kern, hl->yco-st), FPoint(hl->xco+hl->wide, hl->yco-st));
		}
		if (hl->Style & 8)
		{
			double st = (*Doc->AllFonts)[hl->ZFo]->underline_pos * (hl->Siz / 10.0);
			p->setPen(p->brush());
			p->setLineWidth(QMAX((*Doc->AllFonts)[hl->ZFo]->strokeWidth * (hl->Siz / 10.0), 1));
			p->drawLine(FPoint(hl->xco-hl->kern, hl->yco-st), FPoint(hl->xco+hl->wide, hl->yco-st));
		}
	}
	else
	{
		p->setLineWidth(1);
		p->setPen(black);
		p->setFillMode(0);
		p->drawRect(hl->xco, hl->yco-(hl->Siz / 10.0), (hl->Siz / 10.0)*(hl->scale / 100.0), (hl->Siz / 10.0));
	}
}

void PageItem::DrawPolyL(QPainter *p, QPointArray pts)
{
	QColor tmp;
	if (Segments.count() != 0)
	{
		QValueList<uint>::Iterator it2;
		uint FirstVal = 0;
		for (it2 = Segments.begin(); it2 != Segments.end(); ++it2)
		{
			if (NamedLStyle == "")
				p->drawPolyline(pts, FirstVal, (*it2)-FirstVal);
			else
			{
				multiLine ml = Doc->MLineStyles[NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(QPen(tmp,
									 QMAX(static_cast<int>(ml[it].Width*ScApp->view->Scale), 1),
									 static_cast<PenStyle>(ml[it].Dash),
									 static_cast<PenCapStyle>(ml[it].LineEnd),
									 static_cast<PenJoinStyle>(ml[it].LineJoin)));
					p->drawPolyline(pts, FirstVal, (*it2)-FirstVal);
				}
			}
			FirstVal = (*it2);
		}
		if (NamedLStyle == "")
			p->drawPolyline(pts, FirstVal);
		else
		{
			multiLine ml = Doc->MLineStyles[NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 QMAX(static_cast<int>(ml[it].Width*ScApp->view->Scale), 1),
								 static_cast<PenStyle>(ml[it].Dash),
								 static_cast<PenCapStyle>(ml[it].LineEnd),
								 static_cast<PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolyline(pts, FirstVal);
			}
		}
	}
	else
	{
		if (NamedLStyle == "")
			p->drawPolyline(pts);
		else
		{
			multiLine ml = Doc->MLineStyles[NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 QMAX(static_cast<int>(ml[it].Width*ScApp->view->Scale), 1),
								 static_cast<PenStyle>(ml[it].Dash),
								 static_cast<PenCapStyle>(ml[it].LineEnd),
								 static_cast<PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolyline(pts);
			}
		}
	}
}
