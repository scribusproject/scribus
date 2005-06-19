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
#include <qmessagebox.h>
#include <cmath>
#include <cassert>
#include "scpaths.h"
#include "page.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undostate.h"
#include "mpalette.h"
#include "serializer.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include <ft2build.h>
#include FT_GLYPH_H

#include "util.h"

using namespace std;

extern ScribusApp* ScApp;

PageItem::PageItem(ScribusDoc *pa, ItemType newType, double x, double y, double w, double h, double w2, QString fill, QString outline) 
	// Initialize superclasses
	: QObject(pa),
	// Initialize member variables - 2005-03-10 CR. Initializer lists can be faster and safer.
	lineShadeVal(100),
	fillShadeVal(100),
	fillTransparencyVal(0.0),
	lineTransparencyVal(0.0),
	imageIsFlippedH(0),
	imageIsFlippedV(0),
	Locked(false),
	LockRes(false),
	textFlowsAroundFrameVal(false),
	textFlowUsesBoundingBoxVal(false),
	textFlowUsesContourLineVal(false)
{
	QString tmp;
	BackBox = 0;
	NextBox = 0;
	Xpos = x;
	oldXpos = x;
	Ypos = y;
	oldYpos = y;
	Width = w;
	oldWidth = w;
	Height = h;
	oldHeight = h;
	BoundingX = x;
	BoundingY = y;
	BoundingW = w;
	BoundingH = h;
	OldB = Width;
	OldH = Height;
	OldB2 = Width;
	OldH2 = Height;
	itemTypeVal = newType;
	Rot = 0;
	oldRot = 0;
	Doc = pa;
	fillColorVal = fill;
	lineColorVal = itemTypeVal == PageItem::TextFrame ? fill : outline;
	TxtFill = Doc->toolSettings.dPenText;
	TxtStroke = Doc->toolSettings.dStrokeText;
	ShTxtStroke = 100;
	ShTxtFill = 100;
	TxtScale = 1000;
	TxtScaleV = 1000;
	TxtShadowX = 50;
	TxtShadowY = -50;
	TxtOutline = 10;
	TxTStyle = 0;
	TxtBase = 0;
	TxtUnderWidth = Doc->typographicSetttings.valueUnderlineWidth;
	TxtUnderPos = Doc->typographicSetttings.valueUnderlinePos;
	TxtStrikePos = Doc->typographicSetttings.valueStrikeThruPos;
	TxtStrikeWidth = Doc->typographicSetttings.valueStrikeThruWidth;
	GrType = 0;
	GrStartX = 0;
	GrStartY = 0;
	GrEndX = w;
	GrEndY = 0;
	Pwidth = w2;
	OldPwidth = w2;
	PLineArt = PenStyle(Doc->toolSettings.dLineArt);
	PLineEnd = FlatCap;
	PLineJoin = MiterJoin;
	Select = false;
	FrameOnly = false;
	ClipEdited = false;
	FrameType = 0;
	IFont = Doc->toolSettings.defFont;
	ISize = Doc->toolSettings.defSize;
	LineSpMode = 0;
	LineSp = ((Doc->toolSettings.defSize / 10.0) * static_cast<double>(Doc->typographicSetttings.autoLineSpacing) / 100) + (Doc->toolSettings.defSize / 10.0);
	Doc->docParagraphStyles[0].LineSpa = LineSp;
	CurX = 0;
	CurY = 0;
	CPos = 0;
	Extra = 1;
	TExtra = 1;
	BExtra = 1;
	RExtra = 1;
	ExtraV = 0;
	itemText.clear();
	itemText.setAutoDelete(true);
	MaxChars = 0;
	Pfile = "";
	pixm = ScImage();
	pixm.imgInfo.lowResType = Doc->toolSettings.lowResType;
	Pfile2 = "";
	Pfile3 = "";
	LocalScX = 1;
	LocalScY = 1;
	OrigW = 0;
	OrigH = 0;
	LocalX = 0;
	LocalY = 0;
	BBoxX = 0;
	BBoxH = 0;
	RadRect = 0;
	if ((itemTypeVal == TextFrame) || (itemTypeVal == ImageFrame))
		// TODO: Frame should become a read-only calculated property
		Frame = true;
	else
		Frame = false;
	switch (itemTypeVal)
	{
		case Polygon:
			Clip.setPoints(4, static_cast<int>(w/2), 0, static_cast<int>(w), static_cast<int>(h/2),
								static_cast<int>(w/2), static_cast<int>(h), 0,static_cast<int>(h/2));
			break;
		default:
			Clip.setPoints(4, 0,0, static_cast<int>(w),0, static_cast<int>(w), static_cast<int>(h), 0,static_cast<int>(h));
			break;
	}
	PoLine.resize(0);
	ContourLine.resize(0);
	imageClip.resize(0);
	Segments.clear();
	PoShow = false;
	BaseOffs = 0;
	OwnPage = Doc->currentPage->PageNr;
	oldOwnPage = OwnPage;
	savedOwnPage = OwnPage;
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
	switch (itemTypeVal)
	{
	case ImageFrame:
		AnName = tr("Image");
		setUPixmap(Um::IImageFrame);
		break;
	case TextFrame:
		AnName = tr("Text");
		setUPixmap(Um::ITextFrame);
		break;
	case Line:
		AnName = tr("Line");
		setUPixmap(Um::ILine);
		break;
	case Polygon:
		AnName = tr("Polygon");
		setUPixmap(Um::IPolygon);
		break;
	case PolyLine:
		AnName = tr("Polyline");
		setUPixmap(Um::IPolyline);
		break;
	case PathText:
		AnName = tr("PathText");
		setUPixmap(Um::IPathText);
		break;
	default:
		AnName = "Item";
		break;
	}
	AnName += tmp.setNum(Doc->TotalItems); // +" "+QDateTime::currentDateTime().toString();
	AutoName = true;
	setUName(AnName);
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
	Tinput = false;
	isAutoText = false;
	textAlignment = 0;
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
	Reverse = false;
	NamedLStyle = "";
	DashValues.clear();
	TabValues.clear();
	DashOffset = 0;
	fillRule = true;
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();
	if (fillColor() == "None")
		fill_gradient.addStop(Doc->PageColors[Doc->toolSettings.dBrush].getRGBColor(), 0.0, 0.5, 1.0, Doc->toolSettings.dBrush, 100);
	else
		fill_gradient.addStop(Doc->PageColors[fillColor()].getRGBColor(), 0.0, 0.5, 1.0, fillColor(), 100);
	if (lineColor() == "None")
		fill_gradient.addStop(Doc->PageColors[Doc->toolSettings.dPen].getRGBColor(), 1.0, 0.5, 1.0, Doc->toolSettings.dPen, 100);
	else
		fill_gradient.addStop(Doc->PageColors[lineColor()].getRGBColor(), 1.0, 0.5, 1.0, lineColor(), 100);
	Language = Doc->Language;
	Cols = Doc->toolSettings.dCols;
	ColGap = Doc->toolSettings.dGap;
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
	isEmbedded = false;
	OnMasterPage = Doc->currentPage->PageNam;
	startArrowIndex = Doc->toolSettings.dStartArrow;
	endArrowIndex = Doc->toolSettings.dEndArrow;
	undoManager = UndoManager::instance();
	effectsInUse.clear();
	//Page Item Attributes
	pageItemAttributes.clear();
	for(ObjAttrVector::Iterator objAttrIt = Doc->docItemAttributes.begin() ; objAttrIt != Doc->docItemAttributes.end(); ++objAttrIt )
	{
		if (((*objAttrIt).autoaddto=="textframes" && itemTypeVal==TextFrame) ||
			((*objAttrIt).autoaddto=="imageframes" && itemTypeVal==ImageFrame)
			)
			pageItemAttributes.append(*objAttrIt);
	}
}

/** Zeichnet das Item */
void PageItem::DrawObj(ScPainter *p, QRect e)
{
	double sc;
	if (!Doc->DoDrawing)
	{
		Redrawn = true;
		Tinput = false;
		FrameOnly = false;
		return;
	}
	DrawObj_Pre(p, sc);
	switch(itemType())
	{
		case ImageFrame:
			DrawObj_ImageFrame(p, sc);
			break;
		case TextFrame:
			DrawObj_TextFrame(p, e, sc);
			break;
		case Line:
			DrawObj_Line(p);
			break;
		case Polygon:
			DrawObj_Polygon(p);
			break;
		case PolyLine:
			DrawObj_PolyLine(p);
			break;
		case PathText:
			DrawObj_PathText(p, sc);
			break;
		default:
			break;
	}
	DrawObj_Post(p);
}

void PageItem::DrawObj_Pre(ScPainter *p, double &sc)
{
	sc = ScApp->view->getScale();
	p->save();
	if (!isEmbedded)
	{
		p->setZoomFactor(sc);
		p->translate(Xpos*sc, Ypos*sc);
		p->rotate(Rot);
	}
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
		if (fillColor() != "None")
		{
			QColor tmp;
			SetFarbe(&tmp, fillColor(), fillShade());
			p->setBrush(tmp);
			p->setFillMode(ScPainter::Solid);
		}
		else
			p->setFillMode(ScPainter::None);
	}
	if (lineColor() != "None")
	{
		QColor tmp;
		SetFarbe(&tmp, lineColor(), lineShade());
		if ((Pwidth == 0) && (itemType() != Line))
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
	p->setBrushOpacity(1.0 - fillTransparency());
	p->setPenOpacity(1.0 - lineTransparency());
}

void PageItem::DrawObj_Post(ScPainter *p)
{
	bool doStroke=true;
	if ((itemType()==PathText && !PoShow) || itemType()==PolyLine || itemType()==Line)
		doStroke=false;
	if ((doStroke) && (!Doc->RePos))
	{
		if (lineColor() != "None")
		{
			QColor tmp;
			SetFarbe(&tmp, lineColor(), lineShade());
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
				QColor tmp;
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
	if ((!isEmbedded) && (!Doc->RePos))
	{
		double scpInv = 1.0 / (QMAX(ScApp->view->getScale(), 1));
		if ((Frame) && (Doc->guidesSettings.framesShown) && ((itemType() == ImageFrame) || (itemType() == TextFrame)))
		{
			p->setPen(black, scpInv, DotLine, FlatCap, MiterJoin);
			if ((isBookmark) || (isAnnotation))
				p->setPen(blue, scpInv, DotLine, FlatCap, MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(red, scpInv, SolidLine, FlatCap, MiterJoin);
			if (Locked)
				p->setPen(darkRed, scpInv, SolidLine, FlatCap, MiterJoin);
				
			p->setFillMode(0);
			p->setupPolygon(&PoLine);
			p->strokePath();
		}
		if ((Doc->guidesSettings.framesShown) && textFlowUsesContourLine() && (ContourLine.size() != 0))
		{
			p->setPen(lightGray, scpInv, DotLine, FlatCap, MiterJoin);
			p->setupPolygon(&ContourLine);
			p->strokePath();
		}
	}
	Tinput = false;
	FrameOnly = false;
	p->restore();
}

void PageItem::DrawObj_Embedded(ScPainter *p, QRect e, struct ZZ *hl)
{
	if (hl->embedded != 0)
	{
		if (!Doc->DoDrawing)
		{
			hl->embedded->Redrawn = true;
			hl->embedded->Tinput = false;
			hl->embedded->FrameOnly = false;
			return;
		}
		struct ParagraphStyle vg;
		QValueList<ParagraphStyle> savedParagraphStyles;
		for (int xxx=0; xxx<5; ++xxx)
		{
			vg.LineSpaMode = Doc->docParagraphStyles[xxx].LineSpaMode;
			vg.BaseAdj = Doc->docParagraphStyles[xxx].BaseAdj;
			vg.LineSpa = Doc->docParagraphStyles[xxx].LineSpa;
			vg.FontSize = Doc->docParagraphStyles[xxx].FontSize;
			vg.Indent = Doc->docParagraphStyles[xxx].Indent;
			vg.First = Doc->docParagraphStyles[xxx].First;
			vg.gapBefore = Doc->docParagraphStyles[xxx].gapBefore;
			vg.gapAfter = Doc->docParagraphStyles[xxx].gapAfter;
			savedParagraphStyles.append(vg);
		}
		p->save();
		hl->embedded->Xpos = Xpos + hl->xco;
		hl->embedded->Ypos = Ypos + (hl->yco - (hl->embedded->Height * (hl->scalev / 1000.0)));
		p->translate(hl->xco * p->zoomFactor(), (hl->yco - (hl->embedded->Height * (hl->scalev / 1000.0))) * p->zoomFactor());
		if (hl->base != 0)
		{
			p->translate(0, -hl->embedded->Height * (hl->base / 1000.0) * p->zoomFactor());
			hl->embedded->Ypos -= hl->embedded->Height * (hl->base / 1000.0);
		}
		p->scale(hl->scale / 1000.0, hl->scalev / 1000.0);
		hl->embedded->Dirty = Dirty;
		double sc;
		double pws = hl->embedded->Pwidth;
		hl->embedded->DrawObj_Pre(p, sc);
		switch(hl->embedded->itemType())
		{
			case ImageFrame:
				hl->embedded->DrawObj_ImageFrame(p, sc);
				break;
			case TextFrame:
				hl->embedded->DrawObj_TextFrame(p, e, sc);
				break;
			case Line:
				hl->embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
				hl->embedded->DrawObj_Line(p);
				break;
			case Polygon:
				hl->embedded->DrawObj_Polygon(p);
				break;
			case PolyLine:
				hl->embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
				hl->embedded->DrawObj_PolyLine(p);
				break;
			case PathText:
				hl->embedded->DrawObj_PathText(p, sc);
				break;
			default:
				break;
		}
		hl->embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
		hl->embedded->DrawObj_Post(p);
		p->restore();
		hl->embedded->Pwidth = pws;
		for (int xxx=0; xxx<5; ++xxx)
		{
			Doc->docParagraphStyles[xxx].LineSpaMode = savedParagraphStyles[xxx].LineSpaMode;
			Doc->docParagraphStyles[xxx].BaseAdj = savedParagraphStyles[xxx].BaseAdj;
			Doc->docParagraphStyles[xxx].LineSpa = savedParagraphStyles[xxx].LineSpa;
			Doc->docParagraphStyles[xxx].FontSize = savedParagraphStyles[xxx].FontSize;
			Doc->docParagraphStyles[xxx].Indent = savedParagraphStyles[xxx].Indent;
			Doc->docParagraphStyles[xxx].First = savedParagraphStyles[xxx].First;
			Doc->docParagraphStyles[xxx].gapBefore = savedParagraphStyles[xxx].gapBefore;
			Doc->docParagraphStyles[xxx].gapAfter = savedParagraphStyles[xxx].gapAfter;
		}
		savedParagraphStyles.clear();
	}
}

/** Zeichnet das Item */
void PageItem::DrawObj_ImageFrame(ScPainter *p, double sc)
{
	if(!Doc->RePos)
	{
		if ((fillColor() != "None") || (GrType != 0))
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
				if (imageClip.size() != 0)
					p->setupPolygon(&imageClip);
				else
					p->setupPolygon(&PoLine);
				p->setClipPath();
				p->save();
				if (imageFlippedH())
				{
					p->translate(Width * sc, 0);
					p->scale(-1, 1);
				}
				if (imageFlippedV())
				{
					p->translate(0, Height * sc);
					p->scale(1, -1);
				}
				p->translate(LocalX*LocalScX*sc, LocalY*LocalScY*sc);
				p->scale(LocalScX, LocalScY);
				if (pixm.imgInfo.lowResType != 0)
					p->scale(pixm.imgInfo.lowResScale, pixm.imgInfo.lowResScale);
				p->drawImage(&pixm);
				p->restore();
			}
		}
	}
}

/** Zeichnet das Item */
void PageItem::DrawObj_TextFrame(ScPainter *p, QRect e, double sc)
{
	switch (itemType())
	{
		case TextFrame:
		{
			QPainter pp, pf2;
			PageItem *nextItem;
			QPoint pt1, pt2;
			FPoint ColBound;
			QRegion cm;
			uint a, nrc, nrc2, startLin;
			int absa, aSpa, chs, chsd, CurrCol;
			uint BuPos, LastSP, MaxText;
			double oldCurY, LastXp, EndX, OFs, OFs2, wide, lineCorr, ColWidth, kernVal, RTabX;
			QString chx, chx2, chx3;
			struct ScText *hl;
			struct ZZ *Zli;
			struct ZZ *Zli2;
	
			bool outs = false;
			bool fBorder = false;
			bool RTab = false;
			bool goNoRoom = false;
			bool goNextColumn = false;
			uint StartRT, StartRT2;
			int TabCode = 0;
			int HyphenCount = 0;
			QValueList<TabRecord> tTabValues;
			bool DropCmode = false;
			bool AbsHasDrop = false;
			double desc, asce, maxDY, firstDes, desc2, maxDX, tabDist;
			int DropLines;
			bool StartOfCol = true;
			tTabValues.clear();
	
			for (int xxx=0; xxx<5; ++xxx)
			{
				Doc->docParagraphStyles[xxx].LineSpaMode = LineSpMode;
				if (LineSpMode == 2)
					Doc->docParagraphStyles[xxx].BaseAdj = true;
				else
					Doc->docParagraphStyles[xxx].BaseAdj = false;
				Doc->docParagraphStyles[xxx].LineSpa = LineSp;
				Doc->docParagraphStyles[xxx].FontSize = ISize;
				Doc->docParagraphStyles[xxx].Indent = 0;
				Doc->docParagraphStyles[xxx].First = 0;
				Doc->docParagraphStyles[xxx].gapBefore = 0;
				Doc->docParagraphStyles[xxx].gapAfter = 0;
				Doc->docParagraphStyles[xxx].textAlignment = xxx;
			}
			
			QPtrList<ZZ> LiList;
			LiList.setAutoDelete(true);
			QRect e2 = QRect(qRound(e.x() / sc), qRound(e.y() / sc), qRound(e.width() / sc), qRound(e.height() / sc));
			p->save();
			pf2.begin(ScApp->view->viewport());
			pf2.translate(Xpos, Ypos);
			pf2.rotate(Rot);
			if ((fillColor() != "None") || (GrType != 0))
			{
				p->setupPolygon(&PoLine);
				p->fillPath();
			}
			if (lineColor() != "None")
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
			if ((itemText.count() != 0) && (Dirty))
			{
				if (imageFlippedH())
				{
					p->translate(Width * sc, 0);
					p->scale(-1, 1);
				}
				if (imageFlippedV())
				{
					p->translate(0, Height * sc);
					p->scale(1, -1);
				}
				struct ZZ Zli3;
				CurrCol = 0;
				ColWidth = (Width - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
				ColBound = FPoint((ColWidth + ColGap) * CurrCol+Extra + lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
				ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
				tabDist = ColBound.x();
				uint tabCc = 0;
				for (a = 0; a < itemText.count(); ++a)
				{
					if (a >= MaxChars)
						break;
					hl = itemText.at(a);
					if (hl->cab < 5)
						tTabValues = TabValues;
					else
						tTabValues = Doc->docParagraphStyles[hl->cab].TabValues;
					if (hl->cstyle & 16384)
						tabCc = 0;
					chx = hl->ch;
					if (hl->yp == 0)
						continue;
					if (hl->ch == QChar(30))
						chx = ExpandToken(a);
					if (hl->ccolor != "None")
					{
						QColor tmp;
						SetFarbe(&tmp, hl->ccolor, hl->cshade);
						p->setBrush(tmp);
					}
					if (hl->cstroke != "None")
					{
						QColor tmp;
						SetFarbe(&tmp, hl->cstroke, hl->cshade2);
						p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
					}
					chs = hl->csize;
					if (hl->cstyle & 2048)
					{
						if (Doc->docParagraphStyles[hl->cab].BaseAdj)
							chs = qRound(10 * ((Doc->typographicSetttings.valueBaseGrid * (Doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(Doc, hl->cfont, chx, 10))));
						else
						{
							if (Doc->docParagraphStyles[hl->cab].LineSpaMode == 0)
								chs = qRound(10 * ((Doc->docParagraphStyles[hl->cab].LineSpa * (Doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(Doc, hl->cfont, chx, 10))));
							else
							{
								double currasce = RealFHeight(Doc, hl->cfont, Doc->docParagraphStyles[hl->cab].FontSize);
								chs = qRound(10 * ((currasce * (Doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCHeight(Doc, hl->cfont, chx, 10)));
							}
						}
					}
					oldCurY = SetZeichAttr(hl, &chs, &chx);
					if ((chx == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
					{
						double wt = Cwidth(Doc, hl->cfont, QString(tTabValues[tabCc].tabFillChar), chs);
						int coun = static_cast<int>((hl->xp - tabDist) / wt);
						double sPos = hl->xp - (hl->xp - tabDist) + 1;
						desc = hl->cfont->numDescender * (-chs / 10.0);
						asce = hl->cfont->numAscent * (chs / 10.0);
						Zli3.Zeich = QString(tTabValues[tabCc].tabFillChar);
						Zli3.Farb = hl->ccolor;
						Zli3.Farb2 = hl->cstroke;
						Zli3.shade = hl->cshade;
						Zli3.shade2 = hl->cshade2;
						Zli3.yco = hl->yp;
						Zli3.Sele = hl->cselect;
						Zli3.Siz = chs;
						Zli3.realSiz = hl->csize;
						Zli3.Style = hl->cstyle;
						Zli3.ZFo = hl->cfont;
						Zli3.wide = wt;
						Zli3.kern = 0;
						Zli3.scale = 1000;
						Zli3.scalev = 1000;
						Zli3.shadowX = hl->cshadowx;
						Zli3.shadowY = hl->cshadowy;
						Zli3.outline = hl->coutline;
						Zli3.base = hl->cbase;
						Zli3.underpos = hl->cunderpos;
						Zli3.underwidth = hl->cunderwidth;
						Zli3.strikepos = hl->cstrikepos;
						Zli3.strikewidth = hl->cstrikewidth;
						for (int cx = 0; cx < coun; ++cx)
						{
							Zli3.xco =  sPos + wt * cx;
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
								DrawZeichenS(p, &Zli3);
						}
					}
					if (chx == QChar(9))
						tabCc++;
					Zli3.Zeich = chx;
					Zli3.Farb = hl->ccolor;
					Zli3.Farb2 = hl->cstroke;
					Zli3.shade = hl->cshade;
					Zli3.shade2 = hl->cshade2;
					Zli3.xco = hl->xp;
					Zli3.yco = hl->yp;
					Zli3.Sele = hl->cselect;
					Zli3.Siz = chs;
					Zli3.realSiz = hl->csize;
					Zli3.Style = hl->cstyle;
					Zli3.ZFo = hl->cfont;
					if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						Zli3.wide = (hl->cembedded->Width + hl->cembedded->Pwidth) * (hl->cscale / 1000.0);
					else
						Zli3.wide = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 1000.0);
					if (hl->cstyle & 16384)
						Zli3.kern = 0;
					else
						Zli3.kern = chs * hl->cextra / 10000.0;
					Zli3.scale = hl->cscale;
					Zli3.scalev = hl->cscalev;
					Zli3.base = hl->cbase;
					Zli3.shadowX = hl->cshadowx;
					Zli3.shadowY = hl->cshadowy;
					Zli3.outline = hl->coutline;
					Zli3.underpos = hl->cunderpos;
					Zli3.underwidth = hl->cunderwidth;
					Zli3.strikepos = hl->cstrikepos;
					Zli3.strikewidth = hl->cstrikewidth;
					Zli3.embedded = hl->cembedded;
					if (!Doc->RePos)
					{
						desc = Zli3.ZFo->numDescender * (-Zli3.Siz / 10.0);
						asce = Zli3.ZFo->numAscent * (Zli3.Siz / 10.0);
						if (((chx == QChar(13)) || (chx == QChar(28))) && (Doc->guidesSettings.showControls))
						{
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco+Zli3.wide),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
							{
								FPointArray points;
								QWMatrix chma, chma2, chma4, chma5;
								double ytrans, xtrans;
								if (chx == QChar(13))
								{
									points = Doc->symReturn.copy();
									if (a > 0)
										ytrans = itemText.at(a-1)->yp-((Zli3.Siz / 10.0) * 0.8);
									else
										ytrans = Zli3.yco-Doc->docParagraphStyles[hl->cab].LineSpa-((Zli3.Siz / 10.0) * 0.8);
								}
								else
								{
									points = Doc->symNewLine.copy();
									if (a > 0)
										ytrans = itemText.at(a-1)->yp-((Zli3.Siz / 10.0) * 0.4);
									else
										ytrans = Zli3.yco-Doc->docParagraphStyles[hl->cab].LineSpa-((Zli3.Siz / 10.0) * 0.4);
								}
								if (hl->cstyle & 16384)
									xtrans = Zli3.xco;
								else
								{
									if (a > 0)
										xtrans = itemText.at(a-1)->xp+ Cwidth(Doc, itemText.at(a-1)->cfont, itemText.at(a-1)->ch, itemText.at(a-1)->csize);
									else
										xtrans = Zli3.xco;
								}
								chma4.translate(xtrans, ytrans);
								chma.scale(Zli3.Siz / 100.0, Zli3.Siz / 100.0);
								chma2.scale(Zli3.scale / 1000.0, Zli3.scalev / 1000.0);
								chma5.scale(p->zoomFactor(), p->zoomFactor());
								points.map(chma * chma2 * chma4 * chma5);
								p->setupTextPolygon(&points);
								p->setFillMode(1);
								p->fillPath();
							}
						}
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
						{
							if (Zli3.Zeich == QChar(25))
								DrawObj_Embedded(p, e, &Zli3);
							else
								DrawZeichenS(p, &Zli3);
						}
						if (hl->cstyle & 8192)
						{
							Zli3.Zeich = "-";
							Zli3.xco = Zli3.xco + Zli3.wide;
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
								DrawZeichenS(p, &Zli3);
						}
					}
					tabDist = Zli3.xco+Zli3.wide;
				}
				if (itemText.count() > MaxChars)
				{
					if (!Doc->RePos)
					{
						double scp1 = 1 / QMAX(ScApp->view->getScale(), 1);
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
			if ((itemText.count() != 0) || (NextBox != 0))
			{
				if (NextBox != 0)
				{
					nextItem = NextBox;
					while (nextItem != 0)
					{
						a = nextItem->itemText.count();
						for (uint s=0; s<a; ++s)
						{
							itemText.append(nextItem->itemText.take(0));
						}
						nextItem->MaxChars = 0;
						nextItem = nextItem->NextBox;
					}
					nextItem = NextBox;
				}
				Doc->docParagraphStyles[0].LineSpa = LineSp;
				QRegion cl = QRegion(pf2.xForm(Clip));
				int LayerLev = Layer2Level(Doc, LayerNr);
				if (!isEmbedded)
				{
					if (OnMasterPage != "")
					{
						Page* Mp = Doc->MasterPages.at(Doc->MasterNames[OnMasterPage]);
						Page* Dp = Doc->Pages.at(savedOwnPage);
						for (a = 0; a < Doc->MasterItems.count(); ++a)
						{
							PageItem* docItem = Doc->MasterItems.at(a);
							int LayerLevItem = Layer2Level(Doc, docItem->LayerNr);
							if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev))
							{
								if (docItem->textFlowsAroundFrame())
								{
									pp.begin(ScApp->view->viewport());
									pp.translate(docItem->Xpos - Mp->Xoffset + Dp->Xoffset, docItem->Ypos - Mp->Yoffset + Dp->Yoffset);
									pp.rotate(docItem->Rot);
									if (docItem->textFlowUsesBoundingBox())
									{
										QPointArray tcli;
										tcli.resize(4);
										tcli.setPoint(0, QPoint(0,0));
										tcli.setPoint(1, QPoint(qRound(docItem->Width), 0));
										tcli.setPoint(2, QPoint(qRound(docItem->Width), qRound(docItem->Height)));
										tcli.setPoint(3, QPoint(0, qRound(docItem->Height)));
										cm = QRegion(pp.xForm(tcli));
									}
									else
									{
										if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
										{
											QValueList<uint> Segs;
											QPointArray Clip2 = FlattenPath(docItem->ContourLine, Segs);
											cm = QRegion(pp.xForm(Clip2));
										}
										else
											cm = QRegion(pp.xForm(docItem->Clip));
									}
									pp.end();
									cl = cl.subtract(cm);
								}
							}
						}
						for (a = 0; a < Doc->Items.count(); ++a)
						{
							PageItem* docItem = Doc->Items.at(a);
							if (docItem->textFlowsAroundFrame())
							{
								pp.begin(ScApp->view->viewport());
								pp.translate(docItem->Xpos, docItem->Ypos);
								pp.rotate(docItem->Rot);
								if (docItem->textFlowUsesBoundingBox())
								{
									QPointArray tcli;
									tcli.resize(4);
									tcli.setPoint(0, QPoint(0,0));
									tcli.setPoint(1, QPoint(qRound(docItem->Width), 0));
									tcli.setPoint(2, QPoint(qRound(docItem->Width), qRound(docItem->Height)));
									tcli.setPoint(3, QPoint(0, qRound(docItem->Height)));
									cm = QRegion(pp.xForm(tcli));
								}
								else
								{
									if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
									{
										QValueList<uint> Segs;
										QPointArray Clip2 = FlattenPath(docItem->ContourLine, Segs);
										cm = QRegion(pp.xForm(Clip2));
									}
									else
										cm = QRegion(pp.xForm(docItem->Clip));
								}
								pp.end();
								cl = cl.subtract(cm);
							}
						}
					}
					for (a = 0; a < Doc->Items.count(); ++a)
					{
						PageItem* docItem = Doc->Items.at(a);
						int LayerLevItem = Layer2Level(Doc, docItem->LayerNr);
						if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev))
						{
							if (docItem->textFlowsAroundFrame())
							{
								pp.begin(ScApp->view->viewport());
								pp.translate(docItem->Xpos, docItem->Ypos);
								pp.rotate(docItem->Rot);
								if (docItem->textFlowUsesBoundingBox())
								{
									QPointArray tcli;
									tcli.resize(4);
									tcli.setPoint(0, QPoint(0,0));
									tcli.setPoint(1, QPoint(qRound(docItem->Width), 0));
									tcli.setPoint(2, QPoint(qRound(docItem->Width), qRound(docItem->Height)));
									tcli.setPoint(3, QPoint(0, qRound(docItem->Height)));
									cm = QRegion(pp.xForm(tcli));
								}
								else
								{
									if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
									{
										QValueList<uint> Segs;
										QPointArray Clip2 = FlattenPath(docItem->ContourLine, Segs);
										cm = QRegion(pp.xForm(Clip2));
									}
									else
										cm = QRegion(pp.xForm(docItem->Clip));
								}
								pp.end();
								cl = cl.subtract(cm);
							}
						}
					}
				}
				if (imageFlippedH())
				{
					p->translate(Width * sc, 0);
					p->scale(-1, 1);
					pf2.translate(Width, 0);
					pf2.scale(-1, 1);
				}
				if (imageFlippedV())
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
				if (itemText.count() > 0)
				{
					hl = itemText.at(0);
					if (Doc->docParagraphStyles[hl->cab].Drop)
					{
						if (Doc->docParagraphStyles[hl->cab].BaseAdj)
							chs = qRound(Doc->typographicSetttings.valueBaseGrid  * Doc->docParagraphStyles[hl->cab].DropLin * 10);
						else
							chs = qRound(Doc->docParagraphStyles[hl->cab].LineSpa * Doc->docParagraphStyles[hl->cab].DropLin * 10);
					}
					else
						chs = hl->csize;
					desc2 = -hl->cfont->numDescender * (chs / 10.0);
					CurY = TExtra+lineCorr;
				}
				else
				{
					desc2 = -(*Doc->AllFonts)[IFont]->numDescender * (ISize / 10.0);
					CurY = Doc->docParagraphStyles[0].LineSpa+TExtra+lineCorr-desc2;
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
				tabDist = 0;
				MaxText = itemText.count();
				StartOfCol = true;
				for (a = 0; a < MaxText; ++a)
				{
					hl = itemText.at(a);
					chx = hl->ch;
					if (hl->ch == QChar(30))
						chx = ExpandToken(a);
					absa = hl->cab;
					if (Doc->docParagraphStyles[absa].LineSpaMode == 1)
						Doc->docParagraphStyles[absa].LineSpa = RealFHeight(Doc, hl->cfont, hl->csize);
					if (a == 0)
					{
						if (BackBox != 0)
						{
							nextItem = BackBox;
							while (nextItem != 0)
							{
								if (nextItem->itemText.count() != 0)
								{
									if (nextItem->itemText.at(nextItem->itemText.count()-1)->ch == QChar(13))
									{
										CurY += Doc->docParagraphStyles[absa].gapBefore;
										if (chx != QChar(13))
											DropCmode = Doc->docParagraphStyles[absa].Drop;
										else
											DropCmode = false;
										if (DropCmode)
											DropLines = Doc->docParagraphStyles[absa].DropLin;
										break;
									}
									else
										break;
								}
								nextItem = nextItem->BackBox;
							}
						}
						else
						{
							if (chx != QChar(13))
								DropCmode = Doc->docParagraphStyles[absa].Drop;
							else
								DropCmode = false;
							CurY += Doc->docParagraphStyles[absa].gapBefore;
							if (DropCmode)
								DropLines = Doc->docParagraphStyles[absa].DropLin;
						}
					}
					hl->cstyle &= 0xF7FF; // 2047;
					hl->cstyle &= 8191;
					if (((Doc->docParagraphStyles[absa].textAlignment == 3) || (Doc->docParagraphStyles[absa].textAlignment == 4)) && (LiList.count() == 0) && (hl->ch == " "))
					{
						hl->cstyle |= 4096;
						continue;
					}
					else
						hl->cstyle &= 0xEFFF; // 4095;
					if (LiList.count() == 0)
					{
						if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
						{
							CurY += Doc->docParagraphStyles[absa].gapBefore;
							if (chx != QChar(13))
								DropCmode = Doc->docParagraphStyles[absa].Drop;
							else
								DropCmode = false;
							if (DropCmode)
							{
								DropLines = Doc->docParagraphStyles[absa].DropLin;
								if (Doc->docParagraphStyles[hl->cab].BaseAdj)
									CurY += Doc->typographicSetttings.valueBaseGrid * (DropLines-1);
								else
								{
									if (Doc->docParagraphStyles[absa].LineSpaMode == 0)
										CurY += Doc->docParagraphStyles[absa].LineSpa * (DropLines-1);
									else
										CurY += RealFHeight(Doc, hl->cfont, Doc->docParagraphStyles[absa].FontSize) * (DropLines-1);
								}
							}
						}
					}
					if (DropCmode)
					{
						if (Doc->docParagraphStyles[hl->cab].BaseAdj)
						{
							chsd = qRound(10 * ((Doc->typographicSetttings.valueBaseGrid * (DropLines-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(Doc, hl->cfont, chx, 10))));
							chs = qRound(10 * ((Doc->typographicSetttings.valueBaseGrid * (DropLines-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(Doc, hl->cfont, chx, 10)));
						}
						else
						{
							if (Doc->docParagraphStyles[absa].LineSpaMode == 0)
							{
								chsd = qRound(10 * ((Doc->docParagraphStyles[absa].LineSpa * (DropLines-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(Doc, hl->cfont, chx, 10))));
								chs = qRound(10 * ((Doc->docParagraphStyles[absa].LineSpa * (DropLines-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(Doc, hl->cfont, chx, 10)));
							}
							else
							{
								double currasce = RealFHeight(Doc, hl->cfont, Doc->docParagraphStyles[hl->cab].FontSize);
								chsd = qRound(10 * ((currasce * (DropLines-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(Doc, hl->cfont, chx, 10))));
								chs = qRound(10 * ((currasce * (DropLines-1)+(hl->cfont->numAscent * (Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(Doc, hl->cfont, chx, 10)));
							}
						}
						hl->cstyle |= 2048;
					}
					else
					{
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
							chs = qRound((hl->cembedded->Height + hl->cembedded->Pwidth) * 10);
						else
							chs = hl->csize;
					}
					oldCurY = SetZeichAttr(hl, &chs, &chx);
					if (chx == QChar(29))
						chx2 = " ";
					else if (chx == QChar(24))
						chx2 = "-";
					else
						chx2 = chx;
					if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						wide = hl->cembedded->Width + hl->cembedded->Pwidth;
					else
					{
						if (a < MaxText-1)
						{
							if (itemText.at(a+1)->ch == QChar(29))
								chx3 = " ";
							else if (itemText.at(a+1)->ch == QChar(24))
								chx3 = "-";
							else
								chx3 = itemText.at(a+1)->ch;
							wide = Cwidth(Doc, hl->cfont, chx2, chs, chx3);
						}
						else
							wide = Cwidth(Doc, hl->cfont, chx2, chs);
					}
					if (DropCmode)
					{
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						{
							wide = hl->cembedded->Width + hl->cembedded->Pwidth;
							if (Doc->docParagraphStyles[hl->cab].BaseAdj)
								asce = Doc->typographicSetttings.valueBaseGrid * DropLines;
							else
							{
								if (Doc->docParagraphStyles[absa].LineSpaMode == 0)
									asce = Doc->docParagraphStyles[absa].LineSpa * DropLines;
								else
									asce = RealFHeight(Doc, hl->cfont, Doc->docParagraphStyles[absa].FontSize) * DropLines;
							}
							hl->cscalev = qRound(asce / (hl->cembedded->Height + hl->cembedded->Pwidth) * 1000.0);
							hl->cscale = hl->cscalev;
						}
						else
						{
							wide = RealCWidth(Doc, hl->cfont, chx2, chsd);
							asce = RealCHeight(Doc, hl->cfont, chx2, chsd);
						}
						desc2 = 0;
						desc = 0;
					}
					else
					{
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						{
							asce = hl->cfont->numAscent * (hl->csize / 10.0);
							desc2 = 0;
							desc = 0;
						}
						else
						{
							desc2 = -hl->cfont->numDescender * (hl->csize / 10.0);
							desc = -hl->cfont->numDescender * (hl->csize / 10.0);
							asce = hl->cfont->numAscent * (hl->csize / 10.0);
						}
					}
					wide = wide * (hl->cscale / 1000.0);
					fBorder = false;
					if (CurY+BExtra+lineCorr > Height)
					{
						StartOfCol = true;
						CurrCol++;
						if (CurrCol < Cols)
						{
							ColWidth = (Width - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
							ColBound = FPoint((ColWidth + ColGap) * CurrCol + Extra+lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
							CurX = ColBound.x();
							ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
							CurY = asce+TExtra+lineCorr+1;
							if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
							{
								if (chx != QChar(13))
									DropCmode = Doc->docParagraphStyles[hl->cab].Drop;
								else
									DropCmode = false;
								if (DropCmode)
								{
									if (Doc->docParagraphStyles[hl->cab].BaseAdj)
										desc2 = -hl->cfont->numDescender * Doc->typographicSetttings.valueBaseGrid * Doc->docParagraphStyles[hl->cab].DropLin;
									else
										desc2 = -hl->cfont->numDescender * Doc->docParagraphStyles[hl->cab].LineSpa * Doc->docParagraphStyles[hl->cab].DropLin;
								}
								if (DropCmode)
									DropLines = Doc->docParagraphStyles[hl->cab].DropLin;
							}
							if (Doc->docParagraphStyles[hl->cab].BaseAdj)
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
					if (LiList.isEmpty())
					{
						startLin = a;
						double TopOffset = asce;
						double BotOffset = desc2;
						goNoRoom = false;
						if (StartOfCol)
						{
							CurY = asce+TExtra+lineCorr+1;
							if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
								CurY += Doc->docParagraphStyles[hl->cab].gapBefore;
						}
						if (Doc->docParagraphStyles[hl->cab].BaseAdj)
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
								{
									CurX = ColBound.x();
									CurY++;
								}
								else
								{
									CurY += Doc->docParagraphStyles[hl->cab].LineSpa;
									CurX = ColBound.x();
								}
								if (Doc->docParagraphStyles[hl->cab].BaseAdj)
								{
									double by = Ypos;
									if (OwnPage != -1)
										by = Ypos - Doc->Pages.at(OwnPage)->Yoffset;
									int ol1 = qRound((by + CurY - Doc->typographicSetttings.offsetBaseGrid) * 10000.0);
									int ol2 = static_cast<int>(ol1 / Doc->typographicSetttings.valueBaseGrid);
									CurY = ceil(  ol2 / 10000.0 ) * Doc->typographicSetttings.valueBaseGrid + Doc->typographicSetttings.offsetBaseGrid - by;
								}
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
										if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
										{
											if (chx != QChar(13))
												DropCmode = Doc->docParagraphStyles[hl->cab].Drop;
											else
												DropCmode = false;
											if (DropCmode)
											{
												if (Doc->docParagraphStyles[hl->cab].BaseAdj)
													desc2 = -hl->cfont->numDescender * Doc->typographicSetttings.valueBaseGrid * Doc->docParagraphStyles[hl->cab].DropLin;
												else
													desc2 = -hl->cfont->numDescender * Doc->docParagraphStyles[hl->cab].LineSpa * Doc->docParagraphStyles[hl->cab].DropLin;
											}
											if (DropCmode)
												DropLines = Doc->docParagraphStyles[hl->cab].DropLin;
										}
										if (Doc->docParagraphStyles[hl->cab].BaseAdj)
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
						if (a > 0)
						{
							if (itemText.at(a-1)->ch == QChar(13))
								CurX += Doc->docParagraphStyles[hl->cab].First;
						}
						else
						{
							if (BackBox == 0)
								CurX += Doc->docParagraphStyles[hl->cab].First;
							else
							{
								if (BackBox->itemText.at(BackBox->itemText.count()-1)->ch == QChar(13))
									CurX += Doc->docParagraphStyles[hl->cab].First;
							}
						}
						if (!AbsHasDrop)
							CurX += Doc->docParagraphStyles[hl->cab].Indent;
						fBorder = false;
					}
					if (RTab)
					{
						if (((hl->ch == ".") && (TabCode == 2)) || ((hl->ch == ",") && (TabCode == 3)) || (hl->ch == QChar(9)))
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
								tTabValues = Doc->docParagraphStyles[hl->cab].TabValues;
							if (tTabValues.isEmpty())
							{
								if ((CurX - ColBound.x()) != 0)
								{
									if (CurX == ColBound.x() + ceil((CurX-ColBound.x()) / Doc->toolSettings.dTabWidth) * Doc->toolSettings.dTabWidth)
										CurX += Doc->toolSettings.dTabWidth;
									else
										CurX = ColBound.x() + ceil((CurX-ColBound.x()) / Doc->toolSettings.dTabWidth) * Doc->toolSettings.dTabWidth;
								}
								else
									CurX = ColBound.x() + Doc->toolSettings.dTabWidth;
								TabCode = 0;
								RTab = false;
							}
							else
							{
								double tCurX = CurX - ColBound.x();
								double oCurX = tCurX + wide;
								for (int yg = static_cast<int>(tTabValues.count()-1); yg > -1; yg--)
								{
									if (oCurX < tTabValues[yg].tabPosition)
									{
										tCurX = tTabValues[yg].tabPosition;
										TabCode = static_cast<int>(tTabValues[yg].tabType);
									}
								}
								if (TabCode == 0)
									RTab = false;
								else
									RTab = true;
								if (tCurX == oCurX-wide)
									CurX = ColBound.x() + ceil((CurX-ColBound.x()) / Doc->toolSettings.dTabWidth) * Doc->toolSettings.dTabWidth;
								else
									CurX = ColBound.x() + tCurX;
							}
							CurX -= 1;
							StartRT = LiList.count();
							StartRT2 = a;
						}
					}
					hl->yp = CurY + oldCurY;
					if (DropCmode)
						hl->yp -= RealCHeight(Doc, hl->cfont, chx2, chsd) - RealCAscent(Doc, hl->cfont, chx2, chsd);
					if (LiList.count() == 0)
					{
						itemText.at(a)->cstyle |= 16384;
						kernVal = 0;
					}
					else
					{
						kernVal = chs * hl->cextra / 10000.0;
						itemText.at(a)->cstyle &= 16383;
					}
					if (!RTab)
					{
						hl->xp = QMAX(CurX+kernVal, ColBound.x());
						CurX += wide+kernVal;
						CurX = QMAX(CurX, ColBound.x());
					}
					else
					{
						CurX = QMAX(CurX, ColBound.x());
						hl->xp = CurX;
					}
					if ((TabCode == 4) && (RTab))
					{
						CurX += (wide+kernVal) / 2;
						CurX = QMAX(CurX, ColBound.x());
					}
					if (((hl->cstyle & 128) || (hl->ch == "-")) && ((HyphenCount < Doc->HyCount) || (Doc->HyCount == 0)))
					{
						if (hl->cstyle & 128)
						{
							pt1 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(Doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0))), qRound(CurY+desc));
							pt2 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(Doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0))), qRound(ceil(CurY-asce)));
						}
						else
						{
							pt1 = QPoint(qRound(ceil(CurX+RExtra)), qRound(CurY+desc));
							pt2 = QPoint(qRound(ceil(CurX+RExtra)), qRound(ceil(CurY-asce)));
						}
					}
					else
					{
						pt1 = QPoint(qRound(ceil(CurX+RExtra)), qRound(CurY+desc));
						pt2 = QPoint(qRound(ceil(CurX+RExtra)), qRound(ceil(CurY-asce)));
					}
					if ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))) || (CurX+RExtra+lineCorr > ColBound.y()))
						outs = true;
					if (CurY > (Height - BExtra - lineCorr))
						outs = true;
					if ((hl->ch == QChar(27)) && (a < itemText.count()-1))
						goNoRoom = true;
					if ((hl->ch == QChar(26)) && (Cols > 1))
						goNextColumn = true;
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
					{
						Zli->Siz = chsd;
						Zli->realSiz = Doc->docParagraphStyles[hl->cab].FontSize;
					}
					else
					{
						Zli->Siz = chs;
						Zli->realSiz = hl->csize;
					}
					Zli->Style = hl->cstyle;
					Zli->ZFo = hl->cfont;
					Zli->wide = wide;
					Zli->kern = kernVal;
					Zli->scale = hl->cscale;
					Zli->scalev = hl->cscalev;
					Zli->base = hl->cbase;
					Zli->shadowX = hl->cshadowx;
					Zli->shadowY = hl->cshadowy;
					Zli->outline = hl->coutline;
					Zli->underpos = hl->cunderpos;
					Zli->underwidth = hl->cunderwidth;
					Zli->strikepos = hl->cstrikepos;
					Zli->strikewidth = hl->cstrikewidth;
					Zli->embedded = hl->cembedded;
					if (((hl->ch == " ") || (hl->ch == QChar(9))) && (!outs))
					{
						if (a > 0)
						{
							if (itemText.at(a-1)->ch !=  " ")
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
								LastXp = CurX + Cwidth(Doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0);
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
							itemText.at(StartRT2+rtx2)->xp = QMAX(itemText.at(StartRT2+rtx2)->xp-(wide+kernVal) / cen, 0.0);
							if (itemText.at(StartRT2+rtx2)->xp < RTabX)
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
						CurX += Doc->docParagraphStyles[hl->cab].DropDist;
						CurX = QMAX(CurX, ColBound.x());
						maxDX = CurX;
						QPointArray tcli;
						tcli.resize(4);
						if (Doc->docParagraphStyles[hl->cab].BaseAdj)
						{
							CurY -= Doc->typographicSetttings.valueBaseGrid * (DropLines-1);
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - Doc->Pages.at(OwnPage)->Yoffset;
							int ol1 = qRound((by + CurY - Doc->typographicSetttings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / Doc->typographicSetttings.valueBaseGrid);
							CurY = ceil(  ol2 / 10000.0 ) * Doc->typographicSetttings.valueBaseGrid + Doc->typographicSetttings.offsetBaseGrid - by;
							tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*Doc->typographicSetttings.valueBaseGrid)));
							tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*Doc->typographicSetttings.valueBaseGrid)));
						}
						else
						{
							if (Doc->docParagraphStyles[absa].LineSpaMode == 0)
							{
								CurY -= Doc->docParagraphStyles[absa].LineSpa * (DropLines-1);
								tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*Doc->docParagraphStyles[absa].LineSpa)));
								tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*Doc->docParagraphStyles[absa].LineSpa)));
							}
							else
							{
								double currasce = RealFHeight(Doc, hl->cfont, Doc->docParagraphStyles[hl->cab].FontSize);
								CurY -= currasce * (DropLines-1);
								tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*currasce)));
								tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*currasce)));
							}
						}
						tcli.setPoint(2, QPoint(qRound(maxDX), qRound(maxDY)));
						tcli.setPoint(3, QPoint(qRound(hl->xp), qRound(maxDY)));
						cm = QRegion(pf2.xForm(tcli));
						cl = cl.subtract(cm);
					}
					if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27))  || ((hl->ch == QChar(26)) && (Cols > 1)) || (outs))
					{
						RTab = false;
						TabCode = 0;
						if (outs)
						{
							if (LastSP != 0)            // Hier koenen auch andere Trennungen eingebaut werden
							{
								a -= BuPos - LastSP;
								a++;
								if (itemText.at(a)->cstyle & 128)
								{
									HyphenCount++;
									itemText.at(a)->cstyle |= 8192;
									Zli = new ZZ;
									Zli->Zeich = "-";
									Zli->Farb = itemText.at(a)->ccolor;
									Zli->Farb2 = itemText.at(a)->cstroke;
									Zli->shade = itemText.at(a)->cshade;
									Zli->shade2 = itemText.at(a)->cshade2;
									Zli->xco = LastXp - Cwidth(Doc, itemText.at(a)->cfont, "-", itemText.at(a)->csize) * (itemText.at(a)->cscale / 1000.0);
									Zli->yco = itemText.at(a)->yp;
									Zli->Sele = itemText.at(a)->cselect;
									Zli->Siz = itemText.at(a)->csize;
									Zli->realSiz = itemText.at(a)->csize;
									Zli->Style = itemText.at(a)->cstyle;
									Zli->ZFo = itemText.at(a)->cfont;
									Zli->wide = Cwidth(Doc, itemText.at(a)->cfont, "-", itemText.at(a)->csize) * (itemText.at(a)->cscale / 1000.0);
									Zli->kern = itemText.at(a)->csize * itemText.at(a)->cextra / 10000.0;
									Zli->scale = itemText.at(a)->cscale;
									Zli->scalev = itemText.at(a)->cscalev;
									Zli->base = itemText.at(a)->cbase;
									Zli->shadowX = itemText.at(a)->cshadowx;
									Zli->shadowY = itemText.at(a)->cshadowy;
									Zli->outline = itemText.at(a)->coutline;
									Zli->underpos = itemText.at(a)->cunderpos;
									Zli->underwidth = itemText.at(a)->cunderwidth;
									Zli->strikepos = itemText.at(a)->cstrikepos;
									Zli->strikewidth = itemText.at(a)->cstrikewidth;
									Zli->embedded = 0;
									LiList.insert(LastSP+1, Zli);
									LastSP += 1;
								}
								else
								{
									HyphenCount = 0;
									hl->cstyle &= 8191;
								}
								BuPos = LastSP+1;
								if (Doc->docParagraphStyles[absa].textAlignment != 0)
								{
									EndX = LastXp;
									do
									{
										pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
										pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
										EndX++;
									}
									while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
									if (Doc->docParagraphStyles[absa].textAlignment == 2)
										OFs = EndX - LastXp;
									if (Doc->docParagraphStyles[absa].textAlignment == 1)
										OFs = (EndX - LastXp) / 2;
									if ((Doc->docParagraphStyles[absa].textAlignment == 3) || (Doc->docParagraphStyles[absa].textAlignment == 4))
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
									CurX = EndX;
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
							if (Doc->docParagraphStyles[absa].textAlignment != 0)
							{
								EndX = CurX;
								do
								{
									pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
									pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
									EndX++;
								}
								while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
								if (Doc->docParagraphStyles[absa].textAlignment == 2)
									OFs = EndX - CurX;
								if (Doc->docParagraphStyles[absa].textAlignment == 1)
									OFs = (EndX - CurX) / 2;
								if (Doc->docParagraphStyles[absa].textAlignment == 3)
									OFs = 0;
								if (Doc->docParagraphStyles[absa].textAlignment == 4)
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
								CurX = EndX;
							}
						}
						uint BuPos3 = BuPos;
						if ((outs) || (hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || ((hl->ch == QChar(26)) && (Cols > 1)))
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
										if (Doc->docParagraphStyles[hl->cab].BaseAdj)
											CurY += Doc->typographicSetttings.valueBaseGrid;
										else
											CurY += Doc->docParagraphStyles[hl->cab].LineSpa;
										if ((CurY+desc+BExtra+lineCorr > Height) && (CurrCol+1 == Cols))
										{
											goNoRoom = true;
											break;
										}
										if (AbsHasDrop)
										{
											if ((CurY > maxDY) && (CurY - asce > maxDY))
											{
												AbsHasDrop = false;
												CurX = ColBound.x();
											}
											else
												CurX = maxDX;
										}
										else
											CurX = ColBound.x();
										if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || (hl->ch == QChar(26)))
										{
											if (hl->ch == QChar(13))
												CurY += Doc->docParagraphStyles[hl->cab].gapAfter;
											if (BuPos3 > 0)
												BuPos3 -= 1;
											HyphenCount = 0;
										}
										break;
									}
									pt1 = QPoint(qRound(CurX+RExtra), static_cast<int>(CurY+BotOffset));
									pt2 = QPoint(qRound(CurX+RExtra), static_cast<int>(ceil(CurY-asce)));
								}
								if (fromOut)
								{
									if ((CurY+desc+BExtra+lineCorr > Height) && (CurrCol+1 == Cols))
									{
										goNoRoom = true;
										break;
									}
									CurX--;
									CurX = QMAX(CurX, 0);
								}
							}
							else
							{
								if (((hl->ch == QChar(13)) || (hl->ch == QChar(28))) && (AbsHasDrop))
								{
									AbsHasDrop = false;
									if (CurY < maxDY)
										CurY = maxDY;
								}
								if (Doc->docParagraphStyles[hl->cab].BaseAdj)
									CurY += Doc->typographicSetttings.valueBaseGrid;
								else
									CurY += Doc->docParagraphStyles[hl->cab].LineSpa;
								if (AbsHasDrop)
								{
									if ((CurY > maxDY) && (CurY - asce > maxDY))
									{
										AbsHasDrop = false;
										CurX = ColBound.x();
									}
									else
										CurX = maxDX;
								}
								else
									CurX = ColBound.x();
								if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || (hl->ch == QChar(26)))
								{
									if (hl->ch == QChar(13))
										CurY += Doc->docParagraphStyles[hl->cab].gapAfter;
									if (BuPos3 > 0)
										BuPos3 -= 1;
									HyphenCount = 0;
								}
							}
						}
						hl->xp = CurX;
						hl->yp = CurY;
						LiList.at(LiList.count()-1)->xco = hl->xp;
						LiList.at(LiList.count()-1)->yco = hl->yp;
						if (LiList.count() != 0)
						{
							if ((!AbsHasDrop) && (StartOfCol) && (!Doc->docParagraphStyles[hl->cab].BaseAdj))
							{
								Zli2 = LiList.at(0);
								double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
								double currasce;
								if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
									currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
								else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
									currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
								else
									currasce = RealCAscent(Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
								for (uint zc = 0; zc < LiList.count(); ++zc)
								{
									Zli2 = LiList.at(zc);
									if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10)) 
										|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
										|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
										|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
										continue;
									if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
										currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
									else
										currasce = QMAX(currasce, RealCAscent(Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
								}
								double adj = firstasce - currasce;
								for (uint zc = 0; zc < LiList.count(); ++zc)
								{
									LiList.at(zc)->yco -= adj;
								}
								CurY -= adj;
							}
							if ((!StartOfCol) && (!Doc->docParagraphStyles[hl->cab].BaseAdj) && (Doc->docParagraphStyles[hl->cab].LineSpaMode == 1))
							{
								Zli2 = LiList.at(0);
								double firstasce = Doc->docParagraphStyles[hl->cab].LineSpa;
								double currasce;
								if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
									currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
								else
									currasce = RealFHeight(Doc, Zli2->ZFo, Zli2->realSiz);
								for (uint zc = 0; zc < LiList.count(); ++zc)
								{
									Zli2 = LiList.at(zc);
									if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10)) 
										|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
										|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
										|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
										continue;
									if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
										currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
									else
									currasce = QMAX(currasce, RealFHeight(Doc, Zli2->ZFo, Zli2->realSiz));
								}
								double adj = firstasce - currasce;
								for (uint zc = 0; zc < LiList.count(); ++zc)
								{
									LiList.at(zc)->yco -= adj;
								}
								CurY -= adj;
							}
						}
						StartOfCol = false;
						tabDist = ColBound.x();
						uint tabCc = 0;
						uint loopC = BuPos3;
						if (Doc->guidesSettings.showControls)
							loopC++;
						for (uint zc = 0; zc<loopC; ++zc)
						{
							double wide2 = 0;
							Zli2 = LiList.at(zc);
							double xcoZli = Zli2->xco;
							itemText.at(startLin+zc)->xp = Zli2->xco;
							itemText.at(startLin+zc)->yp = Zli2->yco;
							if (itemText.at(startLin+zc)->cab < 5)
								tTabValues = TabValues;
							else
								tTabValues = Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].TabValues;
							if (Zli2->Farb != "None")
							{
								QColor tmp;
								SetFarbe(&tmp, Zli2->Farb, Zli2->shade);
								p->setBrush(tmp);
							}
							desc = Zli2->ZFo->numDescender * (-Zli2->Siz / 10.0);
							asce = Zli2->ZFo->numAscent * (Zli2->Siz / 10.0);
							if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (Doc->appMode == EditMode))
							{
								wide = Zli2->wide;
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
									p->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
								p->setBrush(white);
							}
							if (Zli2->Farb2 != "None")
							{
								QColor tmp;
								SetFarbe(&tmp, Zli2->Farb2, Zli2->shade2);
								p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
							}
							if (!Doc->RePos)
							{
								if ((Zli2->Zeich == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
								{
									double wt = Cwidth(Doc, Zli2->ZFo, QString(tTabValues[tabCc].tabFillChar), Zli2->Siz);
									int coun = static_cast<int>((Zli2->xco - tabDist) / wt);
									double sPos = Zli2->xco - (Zli2->xco - tabDist) + 1;
									Zli = new ZZ;
									Zli->Zeich = QString(tTabValues[tabCc].tabFillChar);
									Zli->Farb = Zli2->Farb;
									Zli->Farb2 = Zli2->Farb2;
									Zli->shade = Zli2->shade;
									Zli->shade2 = Zli2->shade2;
									Zli->yco = Zli2->yco;
									Zli->Sele = Zli2->Sele;
									Zli->Siz = Zli2->Siz;
									Zli->realSiz = Zli2->realSiz;
									Zli->Style = Zli2->Style;
									Zli->ZFo = Zli2->ZFo;
									Zli->wide = wt;
									Zli->kern = 0;
									Zli->scale = 1000;
									Zli->scalev = 1000;
									Zli->base = Zli2->base;
									Zli->shadowX = Zli2->shadowX;
									Zli->shadowY = Zli2->shadowY;
									Zli->outline = Zli2->outline;
									Zli->underpos = Zli2->underpos;
									Zli->underwidth = Zli2->underwidth;
									Zli->strikepos = Zli2->strikepos;
									Zli->strikewidth = Zli2->strikewidth;
									Zli->embedded = 0;
									for (int cx = 0; cx < coun; ++cx)
									{
										Zli->xco = sPos + wt * cx;
										if (e2.intersects(pf2.xForm(QRect(qRound(Zli->xco),qRound(Zli->yco-asce), qRound(Zli->wide+1), qRound(asce+desc)))))
											DrawZeichenS(p, Zli);
									}
									delete Zli;
								}
								if (Zli2->Zeich == QChar(9))
									tabCc++;
								if (((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28))) && (Doc->guidesSettings.showControls) && (LiList.count() != 0))
								{
									if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco+Zli2->wide),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
									{
										FPointArray points;
										QWMatrix chma, chma2, chma4, chma5;
										double ytrans, xtrans;
										if (Zli2->Zeich == QChar(13))
										{
											points = Doc->symReturn.copy();
											if (zc > 0)
												ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.8);
											else
												ytrans = Zli2->yco-Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.8);
										}
										else
										{
											points = Doc->symNewLine.copy();
											if (zc > 0)
												ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
											else
												ytrans = Zli2->yco-Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.4);
										}
										if (zc > 0)
											xtrans =  LiList.at(zc-1)->xco + Cwidth(Doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
										else
										{
											if (startLin > 0)
												xtrans = itemText.at(startLin-1)->xp + Cwidth(Doc, itemText.at(startLin-1)->cfont, itemText.at(startLin-1)->ch, itemText.at(startLin-1)->csize);
											else
												xtrans = Zli2->xco;
										}
										chma4.translate(xtrans, ytrans);
										chma.scale(Zli2->Siz / 100.0, Zli2->Siz / 100.0);
										chma2.scale(Zli2->scale / 1000.0, Zli2->scalev / 1000.0);
										chma5.scale(p->zoomFactor(), p->zoomFactor());
										points.map(chma * chma2 * chma4 * chma5);
										p->setupTextPolygon(&points);
										p->setFillMode(1);
										p->fillPath();
									}
								}
								if ((Doc->guidesSettings.showControls) && (zc == BuPos3))
									break;
								if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
								{
									if (Zli2->Zeich == QChar(25))
										DrawObj_Embedded(p, e, Zli2);
									else
										DrawZeichenS(p, Zli2);
								}
							}
							tabDist = Zli2->xco+Zli2->wide;
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
						if (goNextColumn)
						{
							goNextColumn = false;
							StartOfCol = true;
							CurrCol++;
							if (CurrCol < Cols)
							{
								ColWidth = (Width - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
								ColBound = FPoint((ColWidth + ColGap) * CurrCol + Extra+lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
								CurX = ColBound.x();
								ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
							}
							else
							{
								nrc = a;
								goto NoRoom;
							}
						}
					}
				}
				if (Doc->docParagraphStyles[absa].textAlignment != 0)
				{
					EndX = CurX;
					do
					{
						pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
						pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
						EndX++;
					}
					while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
					if (Doc->docParagraphStyles[absa].textAlignment == 2)
						OFs = EndX - CurX;
					if (Doc->docParagraphStyles[absa].textAlignment == 1)
						OFs = (EndX - CurX) / 2;
					if (Doc->docParagraphStyles[absa].textAlignment == 3)
						OFs = 0;
					if (Doc->docParagraphStyles[absa].textAlignment == 4)
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
				if (LiList.count() != 0)
				{
					if ((!AbsHasDrop) && (StartOfCol) && (!Doc->docParagraphStyles[hl->cab].BaseAdj))
					{
						Zli2 = LiList.at(0);
						double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						double currasce;
						if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
							currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
						else
							currasce = RealCAscent(Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							Zli2 = LiList.at(zc);
							if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10)) 
								|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
								|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
								|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
								continue;
							if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
								currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
							else
								currasce = QMAX(currasce, RealCAscent(Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
						}
						double adj = firstasce - currasce;
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							LiList.at(zc)->yco -= adj;
						}
						CurY -= adj;
					}
					if ((!StartOfCol) && (!Doc->docParagraphStyles[hl->cab].BaseAdj) && (Doc->docParagraphStyles[hl->cab].LineSpaMode == 1))
					{
						Zli2 = LiList.at(0);
						double firstasce = Doc->docParagraphStyles[hl->cab].LineSpa;
						double currasce;
						if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
						else
							currasce = RealFHeight(Doc, Zli2->ZFo, Zli2->realSiz);
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							Zli2 = LiList.at(zc);
							if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10)) 
								|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
								|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
								|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
								continue;
							if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
								currasce = QMAX(currasce, (Zli2->embedded->Height + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
							else
								currasce = QMAX(currasce, RealFHeight(Doc, Zli2->ZFo, Zli2->realSiz));
						}
						double adj = firstasce - currasce;
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							LiList.at(zc)->yco -= adj;
						}
						CurY -= adj;
					}
				}
				StartOfCol = false;
				tabDist = ColBound.x();
				uint tabCc = 0;
				for (uint zc = 0; zc<LiList.count(); ++zc)
				{
					double wide2 = 0;
					Zli2 = LiList.at(zc);
					double xcoZli = Zli2->xco;
					itemText.at(startLin+zc)->xp = Zli2->xco;
					itemText.at(startLin+zc)->yp = Zli2->yco;
					if (itemText.at(startLin+zc)->cab < 5)
						tTabValues = TabValues;
					else
						tTabValues = Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].TabValues;
					if (Zli2->Farb != "None")
					{
						QColor tmp;
						SetFarbe(&tmp, Zli2->Farb, Zli2->shade);
						p->setBrush(tmp);
					}
					desc = Zli2->ZFo->numDescender * (-Zli2->Siz / 10.0);
					asce = Zli2->ZFo->numAscent * (Zli2->Siz / 10.0);
					if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (Doc->appMode == EditMode))
					{
						wide = Zli2->wide;
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
							p->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
						p->setBrush(white);
					}
					if (Zli2->Farb2 != "None")
					{
						QColor tmp;
						SetFarbe(&tmp, Zli2->Farb2, Zli2->shade2);
						p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
					}
					if (!Doc->RePos)
					{
						if ((Zli2->Zeich == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
						{
							double wt = Cwidth(Doc, Zli2->ZFo, QString(tTabValues[tabCc].tabFillChar), Zli2->Siz);
							int coun = static_cast<int>((Zli2->xco - tabDist) / wt);
							double sPos = Zli2->xco - (Zli2->xco - tabDist) + 1;
							Zli = new ZZ;
							Zli->Zeich = QString(tTabValues[tabCc].tabFillChar);
							Zli->Farb = Zli2->Farb;
							Zli->Farb2 = Zli2->Farb2;
							Zli->shade = Zli2->shade;
							Zli->shade2 = Zli2->shade2;
							Zli->yco = Zli2->yco;
							Zli->Sele = Zli2->Sele;
							Zli->Siz = Zli2->Siz;
							Zli->realSiz = Zli2->realSiz;
							Zli->Style = Zli2->Style;
							Zli->ZFo = Zli2->ZFo;
							Zli->wide = wt;
							Zli->kern = 0;
							Zli->scale = 1000;
							Zli->scalev = 1000;
							Zli->base = Zli2->base;
							Zli->shadowX = Zli2->shadowX;
							Zli->shadowY = Zli2->shadowY;
							Zli->outline = Zli2->outline;
							Zli->underpos = Zli2->underpos;
							Zli->underwidth = Zli2->underwidth;
							Zli->strikepos = Zli2->strikepos;
							Zli->strikewidth = Zli2->strikewidth;
							Zli->embedded = 0;
							for (int cx = 0; cx < coun; ++cx)
							{
								Zli->xco =  sPos + wt * cx;
								if (e2.intersects(pf2.xForm(QRect(qRound(Zli->xco),qRound(Zli->yco-asce), qRound(Zli->wide+1), qRound(asce+desc)))))
									DrawZeichenS(p, Zli);
							}
							delete Zli;
						}
						if (Zli2->Zeich == QChar(9))
							tabCc++;
						if (((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28))) && (Doc->guidesSettings.showControls) && (LiList.count() != 0))
						{
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco+Zli2->wide),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
							{
								FPointArray points;
								QWMatrix chma, chma2, chma4, chma5;
								double ytrans, xtrans;
								if (Zli2->Zeich == QChar(13))
								{
									points = Doc->symReturn.copy();
									if (zc > 0)
										ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.8);
									else
										ytrans = Zli2->yco-Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.8);
								}
								else
								{
									points = Doc->symNewLine.copy();
									if (zc > 0)
										ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
									else
										ytrans = Zli2->yco-Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.4);
								}
								if (zc > 0)
									xtrans =  LiList.at(zc-1)->xco + Cwidth(Doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
								else
								{
									if (startLin > 0)
										xtrans = itemText.at(startLin-1)->xp + Cwidth(Doc, itemText.at(startLin-1)->cfont, itemText.at(startLin-1)->ch, itemText.at(startLin-1)->csize);
									else
										xtrans = Zli2->xco;
								}
								chma4.translate(xtrans, ytrans);
								chma.scale(Zli2->Siz / 100.0, Zli2->Siz / 100.0);
								chma2.scale(Zli2->scale / 1000.0, Zli2->scalev / 1000.0);
								chma5.scale(p->zoomFactor(), p->zoomFactor());
								points.map(chma * chma2 * chma4 * chma5);
								p->setupTextPolygon(&points);
								p->setFillMode(1);
								p->fillPath();
							}
						}
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
						{
							if (Zli2->Zeich == QChar(25))
								DrawObj_Embedded(p, e, Zli2);
							else
								DrawZeichenS(p, Zli2);
						}
					}
					tabDist = Zli2->xco+Zli2->wide;
				}
				goNextColumn = false;
				LiList.clear();
				BuPos = 0;
				LastSP = 0;
				outs = false;
			}
			MaxChars = itemText.count();
			Redrawn = true;
			Dirty = false;
			pf2.end();
			p->restore();
			break;
			NoRoom: pf2.end();
			if (NextBox != 0)
			{
				nrc2 = itemText.count();
				for (uint ss=nrc; ss<nrc2; ++ss)
				{
					NextBox->itemText.append(itemText.take(nrc));
				}
				if (uint(CPos) > nrc)
				{
					int nCP = CPos - nrc;
					CPos = nrc;
					if ((Doc->appMode == EditMode) && (Tinput))
					{
//							OwnPage->Deselect(true);
						NextBox->CPos = QMAX(nCP, 1);
//							Doc->currentPage = NextBox->OwnPage;
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
					double scp1 = 1.0/QMAX(ScApp->view->getScale(), 1);
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
		}
			break;
		default:
			break;
	}
	Dirty = false;
}

/** Zeichnet das Item */
void PageItem::DrawObj_Line(ScPainter *p)
{
	if (!Doc->RePos)
	{
			if (NamedLStyle == "")
				p->drawLine(FPoint(0, 0), FPoint(Width, 0));
			else
			{
				multiLine ml = Doc->MLineStyles[NamedLStyle];
				QColor tmp;
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
				p->setBrushOpacity(1.0 - lineTransparency());
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
				p->setBrushOpacity(1.0 - lineTransparency());
				p->setLineWidth(0);
				p->setFillMode(ScPainter::Solid);
				p->setupPolygon(&arrow);
				p->fillPath();
			}
	}
}

/** Zeichnet das Item */
void PageItem::DrawObj_Polygon(ScPainter *p)
{
	if (!Doc->RePos)
	{
		p->setupPolygon(&PoLine);
		p->fillPath();
	}
}

/** Zeichnet das Item */
void PageItem::DrawObj_PolyLine(ScPainter *p)
{
	if (!Doc->RePos && PoLine.size()>=4)
	{
		if ((fillColor() != "None") || (GrType != 0))
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
			QColor tmp;
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
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					QWMatrix arrowTrans;
					FPointArray arrow = (*Doc->arrowStyles.at(startArrowIndex-1)).points.copy();
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(Pwidth, Pwidth);
					arrow.map(arrowTrans);
					p->setBrush(p->pen());
					p->setBrushOpacity(1.0 - lineTransparency());
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
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					QWMatrix arrowTrans;
					FPointArray arrow = (*Doc->arrowStyles.at(endArrowIndex-1)).points.copy();
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(Pwidth, Pwidth);
					arrow.map(arrowTrans);
					p->setBrush(p->pen());
					p->setBrushOpacity(1.0 - lineTransparency());
					p->setLineWidth(0);
					p->setFillMode(ScPainter::Solid);
					p->setupPolygon(&arrow);
					p->fillPath();
					break;
				}
			}
		}
	}
}

/** Zeichnet das Item */
void PageItem::DrawObj_PathText(ScPainter *p, double sc)
{
	uint a;
	int chs;
	double wide;
	QString chx, chx2, chx3;
	struct ScText *hl;
	struct ZZ *Zli;
	double dx;
	double sp = 0;
	double oldSp = 0;
	double oCurX = 0;
	FPoint point = FPoint(0, 0);
	FPoint normal = FPoint(0, 0);
	FPoint tangent = FPoint(0, 0);
	FPoint extPoint = FPoint(0, 0);
	bool ext = false;
	bool first = true;
	double fsx = 0;
	uint seg = 0;
	double segLen = 0;
	double distCurX;
	CurX = Extra;
	if (itemText.count() != 0)
		CurX += itemText.at(0)->csize * itemText.at(0)->cextra / 10000.0;
	segLen = PoLine.lenPathSeg(seg);
	for (a = 0; a < itemText.count(); ++a)
	{
		CurY = 0;
		hl = itemText.at(a);
		chx = hl->ch;
		if ((chx == QChar(30)) || (chx == QChar(13)) || (chx == QChar(9)) || (chx == QChar(28)))
			continue;
		chs = hl->csize;
		SetZeichAttr(hl, &chs, &chx);
		if (chx == QChar(29))
			chx2 = " ";
		else if (chx == QChar(24))
			chx2 = "-";
		else
			chx2 = chx;
		if (a < itemText.count()-1)
		{
			if (itemText.at(a+1)->ch == QChar(29))
				chx3 = " ";
			else if (itemText.at(a+1)->ch == QChar(24))
				chx3 = "-";
			else
				chx3 = itemText.at(a+1)->ch;
			wide = Cwidth(Doc, hl->cfont, chx2, chs, chx3);
		}
		else
			wide = Cwidth(Doc, hl->cfont, chx2, chs);
		wide = wide * (hl->cscale / 1000.0);
		dx = wide / 2.0;
		CurX += dx;
		ext = false;
		while ( (seg < PoLine.size()-3) && (CurX > fsx + segLen))
		{
			fsx += segLen;
			seg += 4;
			if (seg > PoLine.size()-3)
				break;
			segLen = PoLine.lenPathSeg(seg);
			ext = true;
		}
		if (seg > PoLine.size()-3)
			break;
		if (CurX > fsx + segLen)
			break;
		if (ext)
		{
			sp = 0;
			distCurX = PoLine.lenPathDist(seg, 0, sp);
			while (distCurX <= ((CurX - oCurX) - (fsx - oCurX)))
			{
				sp += 0.001;
				distCurX = PoLine.lenPathDist(seg, 0, sp);
			}
			PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
			CurX = (CurX - (CurX - fsx)) + distCurX;
			oldSp = sp;
			ext = false;
		}
		else
		{
			if( seg < PoLine.size()-3 )
			{
				if (CurX > fsx + segLen)
					break;
				distCurX = PoLine.lenPathDist(seg, oldSp, sp);
				while (distCurX <= (CurX - oCurX))
				{
					sp += 0.001;
					if (sp >= 1.0)
					{
						sp = 0.9999;
						break;
					}
					distCurX = PoLine.lenPathDist(seg, oldSp, sp);
				}
				PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
				CurX = oCurX + distCurX;
				oldSp = sp;
			}
			else
				break;
		}
		hl->xp = point.x();
		hl->yp = point.y();
		hl->PtransX = tangent.x();
		hl->PtransY = tangent.y();
		hl->PRot = dx;
		QWMatrix trafo = QWMatrix( 1, 0, 0, -1, -dx*sc, 0 );
		trafo *= QWMatrix( tangent.x(), tangent.y(), tangent.y(), -tangent.x(), point.x()*sc, point.y()*sc );
		QWMatrix sca = p->worldMatrix();
		trafo *= sca;
		p->save();
		QWMatrix savWM = p->worldMatrix();
		p->setWorldMatrix(trafo);
		Zli = new ZZ;
		Zli->Zeich = chx;
		if (hl->ccolor != "None")
		{
			QColor tmp;
			SetFarbe(&tmp, hl->ccolor, hl->cshade);
			p->setBrush(tmp);
		}
		if (hl->cstroke != "None")
		{
			QColor tmp;
			SetFarbe(&tmp, hl->cstroke, hl->cshade2);
			p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
		}
		Zli->Farb = hl->ccolor;
		Zli->Farb2 = hl->cstroke;
		Zli->shade = hl->cshade;
		Zli->shade2 = hl->cshade2;
		Zli->xco = 0;
		Zli->yco = BaseOffs;
		Zli->Sele = hl->cselect;
		Zli->Siz = chs;
		Zli->realSiz = hl->csize;
		Zli->Style = hl->cstyle;
		Zli->ZFo = hl->cfont;
		Zli->wide = wide;
		Zli->kern = hl->csize * hl->cextra / 10000.0;
		Zli->scale = hl->cscale;
		Zli->scalev = hl->cscalev;
		Zli->base = hl->cbase;
		Zli->shadowX = hl->cshadowx;
		Zli->shadowY = hl->cshadowx;
		Zli->outline = hl->coutline;
		Zli->underpos = hl->cunderpos;
		Zli->underwidth = hl->cunderwidth;
		Zli->strikepos = hl->cstrikepos;
		Zli->strikewidth = hl->cstrikewidth;
		Zli->embedded = 0;
		if (!Doc->RePos)
			DrawZeichenS(p, Zli);
		delete Zli;
		p->setWorldMatrix(savWM);
		p->restore();
		p->setZoomFactor(sc);
		MaxChars = a+1;
		oCurX = CurX;
		CurX -= dx;
		CurX += wide+hl->csize * hl->cextra / 10000.0;
		first = false;
	}
}


void PageItem::paintObj(QRect e, QPixmap *ppX)
{
	if (!Doc->DoDrawing)
	{
		Redrawn = true;
		Tinput = false;
		FrameOnly = false;
		return;
	}
	//qDebug("paintObj(QRect e, QPixmap *ppX)");
	QPainter p;
	double sc = ScApp->view->getScale();
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
						if (itemType() != Line)
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
// 	checkChanges(); // Check changes for undo actions
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
		while (itemText.at(za2)->ch == QChar(30));
		if (itemText.at(za2)->ch != QChar(30))
			za2++;
		while (itemText.at(za2+zae)->ch == QChar(30))
		{
			zae++;
			if (za2+zae == itemText.count())
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
	*tmp = Doc->PageColors[farbe].getShadeColorProof(shad);
}

double PageItem::SetZeichAttr(struct ScText *hl, int *chs, QString *chx)
{
	double retval = 0.0;
	double asce = hl->cfont->numAscent * (hl->csize / 10.0);
	int chst = hl->cstyle & 1919;
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
		if (chst & 32)
		{
			if (chx->upper() != *chx)
				*chx = chx->upper();
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
	double csi = static_cast<double>(hl->Siz) / 100.0;
	QString ccx = hl->Zeich;
	if ((Doc->guidesSettings.showControls) && ((ccx == QChar(9)) || (ccx == QChar(29)) || ((ccx == QChar(26)) && (Cols > 1))|| (ccx == QChar(27)) || (ccx == QChar(32))))
	{
		QWMatrix chma, chma2, chma4, chma5;
		FPointArray points;
		if (ccx == QChar(9))
		{
			points = Doc->symTab.copy();
			chma4.translate(hl->xco-((hl->Siz / 100.0) * 7.0), hl->yco-((hl->Siz / 10.0) * 0.5));
		}
		else if (ccx == QChar(26))
		{
			points = Doc->symNewCol.copy();
			chma4.translate(hl->xco, hl->yco-((hl->Siz / 10.0) * 0.6));
		}
		else if (ccx == QChar(27))
		{
			points = Doc->symNewFrame.copy();
			chma4.translate(hl->xco, hl->yco-((hl->Siz / 10.0) * 0.6));
		}
		else
		{
			points = Doc->symNonBreak.copy();
			chma4.translate(hl->xco, hl->yco-((hl->Siz / 10.0) * 0.4));
		}
		chma.scale(csi, csi);
		chma2.scale(hl->scale / 1000.0, hl->scalev / 1000.0);
		chma5.scale(p->zoomFactor(), p->zoomFactor());
		points.map(chma * chma2 * chma4 * chma5);
		p->setupTextPolygon(&points);
		if (ccx == QChar(32))
		{
			QColor tmp = p->pen();
			p->setPen(p->brush(), 1, SolidLine, FlatCap, MiterJoin);
			p->setLineWidth(hl->Siz / 200.0);
			p->strokePath();
			p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
		}
		else
		{
			p->setFillMode(1);
			p->fillPath();
		}
	}
	if ((ccx == QChar(13)) || (ccx == QChar(9)) || (ccx == QChar(28)) || (ccx == QChar(27)) || (ccx == QChar(26)))
		return;
	if (ccx == QChar(29))
		ccx = " ";
	if (ccx == QChar(24))
		ccx = "-";
	uint chr = ccx[0].unicode();
	if (hl->ZFo->CharWidth.contains(chr))
	{
		QWMatrix chma, chma2, chma3, chma4, chma5, chma6;
		chma.scale(csi, csi);
		chma5.scale(p->zoomFactor(), p->zoomFactor());
		FPointArray gly = hl->ZFo->GlyphArray[chr].Outlines.copy();
		if (gly.size() > 3)
		{
			chma2.scale(hl->scale / 1000.0, hl->scalev / 1000.0);
			if (Reverse)
			{
				chma3.scale(-1, 1);
				chma3.translate(-hl->wide, 0);
				chma4.translate(hl->xco, hl->yco-((hl->Siz / 10.0) * (hl->scalev / 1000.0)));
			}
			else
				chma4.translate(hl->xco, hl->yco-((hl->Siz / 10.0) * (hl->scalev / 1000.0)));
			if (hl->base != 0)
				chma6.translate(0, -(hl->Siz / 10.0) * (hl->base / 1000.0) * p->zoomFactor());
			gly.map(chma * chma2 * chma3 * chma4 * chma5 * chma6);
			p->setFillMode(1);
			bool fr = p->fillRule();
			p->setFillRule(false);
			p->setupTextPolygon(&gly);
			if ((hl->ZFo->isStroked) && ((hl->Siz * hl->outline / 10000.0) != 0))
			{
				QColor tmp = p->brush();
				p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
				p->setLineWidth(hl->Siz * hl->outline / 10000.0);
				p->strokePath();
			}
			else
			{
				if ((hl->Style & 256) && (hl->Farb2 != "None"))
				{
					p->save();
					p->translate((hl->Siz * hl->shadowX / 10000.0) * p->zoomFactor(), -(hl->Siz * hl->shadowY / 10000.0) * p->zoomFactor());
					QColor tmp = p->brush();
					p->setBrush(p->pen());
					p->fillPath();
					p->setBrush(tmp);
					p->restore();
				}
				if (hl->Farb != "None")
					p->fillPath();
				if ((hl->Style & 4) && (hl->Farb2 != "None") && ((hl->Siz * hl->outline / 10000.0) != 0))
				{
					p->setLineWidth(hl->Siz * hl->outline / 10000.0);
					p->strokePath();
				}
			}
			p->setFillRule(fr);
		}
		if (hl->Style & 16)
		{
			double st, lw;
			if ((hl->strikepos != -1) || (hl->strikewidth != -1))
			{
				if (hl->strikepos != -1)
					st = (hl->strikepos / 1000.0) * (hl->ZFo->numAscent * (hl->realSiz / 10.0));
				else
					st = hl->ZFo->strikeout_pos * (hl->realSiz / 10.0);
				if (hl->strikewidth != -1)
					lw = (hl->strikewidth / 1000.0) * (hl->realSiz / 10.0);
				else
					lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			else
			{
				st = hl->ZFo->strikeout_pos * (hl->realSiz / 10.0);
				lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			if (hl->base != 0)
				st += (hl->Siz / 10.0) * (hl->base / 1000.0);
			p->setPen(p->brush());
			p->setLineWidth(lw);
			p->drawLine(FPoint(hl->xco-hl->kern, hl->yco-st), FPoint(hl->xco+hl->wide, hl->yco-st));
		}
		if ((hl->Style & 8) || ((hl->Style & 512) && (!ccx[0].isSpace())))
		{
			double st, lw;
			if ((hl->underpos != -1) || (hl->underwidth != -1))
			{
				if (hl->underpos != -1)
					st = (hl->underpos / 1000.0) * (hl->ZFo->numDescender * (hl->realSiz / 10.0));
				else
					st = hl->ZFo->underline_pos * (hl->realSiz / 10.0);
				if (hl->underwidth != -1)
					lw = (hl->underwidth / 1000.0) * (hl->realSiz / 10.0);
				else
					lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			else
			{
				st = hl->ZFo->underline_pos * (hl->realSiz / 10.0);
				lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			if (hl->base != 0)
				st += (hl->Siz / 10.0) * (hl->base / 1000.0);
			p->setPen(p->brush());
			p->setLineWidth(lw);
			p->drawLine(FPoint(hl->xco-hl->kern, hl->yco-st), FPoint(hl->xco+hl->wide, hl->yco-st));
		}
	}
	else
	{
		p->setLineWidth(1);
		p->setPen(red);
		p->setBrush(red);
		p->setFillMode(1);
		p->drawRect(hl->xco, hl->yco-(hl->Siz / 10.0), (hl->Siz / 10.0)*(hl->scale / 1000.0), (hl->Siz / 10.0));
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
									 QMAX(static_cast<int>(ml[it].Width*ScApp->view->getScale()), 1),
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
								 QMAX(static_cast<int>(ml[it].Width*ScApp->view->getScale()), 1),
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
								 QMAX(static_cast<int>(ml[it].Width*ScApp->view->getScale()), 1),
								 static_cast<PenStyle>(ml[it].Dash),
								 static_cast<PenCapStyle>(ml[it].LineEnd),
								 static_cast<PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolyline(pts);
			}
		}
	}
}

QString PageItem::itemName() const
{
	return AnName;
}

void PageItem::setItemName(const QString& newName)
{
	if (AnName == newName)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Rename, QString(Um::FromTo).arg(AnName).arg(newName));
		ss->set("OLD_NAME", AnName);
		ss->set("NEW_NAME", newName);
		undoManager->action(this, ss);
	}
	AnName = newName;
	setUName(newName); // set the name for the UndoObject too
}

QString PageItem::fillColor() const
{
	return fillColorVal;
}

void PageItem::setFillColor(const QString &newColor)
{
	if (fillColorVal == newColor)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFill,
										  QString(Um::ColorFromTo).arg(fillColorVal).arg(newColor),
                                          Um::IFill);
		ss->set("FILL", "fill");
		ss->set("OLD_FILL", fillColorVal);
		ss->set("NEW_FILL", newColor);
		undoManager->action(this, ss);
	}
	fillColorVal = newColor;
}

int PageItem::fillShade() const
{
	return fillShadeVal;
}

void PageItem::setFillShade(int newShade)
{
	if (fillShadeVal == newShade)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetShade,
										  QString(Um::FromTo).arg(fillShadeVal).arg(newShade),
										  Um::IShade);
		ss->set("SHADE", "shade");
		ss->set("OLD_SHADE", fillShadeVal);
		ss->set("NEW_SHADE", newShade);
		undoManager->action(this, ss);
	}
	fillShadeVal = newShade;
}

double PageItem::fillTransparency() const
{
	return fillTransparencyVal;
}

void PageItem::setFillTransparency(double newTransparency)
{
	if (fillTransparencyVal == newTransparency)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Transparency,
										  QString(Um::FromTo).arg(fillTransparencyVal).arg(newTransparency),
										  Um::ITransparency);
		ss->set("TRANSPARENCY", "transparency");
		ss->set("OLD_TP", fillTransparencyVal);
		ss->set("NEW_TP", newTransparency);
		undoManager->action(this, ss);
	}
	fillTransparencyVal = newTransparency;
}

QString PageItem::lineColor() const
{
	return lineColorVal;
}

void PageItem::setLineColor(const QString &newColor)
{
	if (lineColorVal == newColor)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetLineColor,
										  QString(Um::ColorFromTo).arg(lineColorVal).arg(newColor),
										  Um::IFill);
		ss->set("LINE_COLOR", "line_color");
		ss->set("OLD_COLOR", lineColorVal);
		ss->set("NEW_COLOR", newColor);
		undoManager->action(this, ss);
	}
	lineColorVal = newColor;
}

int PageItem::lineShade() const
{
	return lineShadeVal;
}

void PageItem::setLineShade(int newShade)
{
	if (lineShadeVal == newShade)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetLineShade,
										  QString(Um::FromTo).arg(lineShadeVal).arg(newShade),
										  Um::IShade);
		ss->set("LINE_SHADE", "line_shade");
		ss->set("OLD_SHADE", lineShadeVal);
		ss->set("NEW_SHADE", newShade);
		undoManager->action(this, ss);
	}
	lineShadeVal = newShade;
}


double PageItem::lineTransparency() const
{
	return lineTransparencyVal;
}

void PageItem::setLineTransparency(double newTransparency)
{
	if (lineTransparencyVal == newTransparency)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LineTransparency,
										  QString(Um::FromTo).arg(lineTransparencyVal).arg(newTransparency),
										  Um::ITransparency);
		ss->set("LINE_TRANSPARENCY", "transparency");
		ss->set("OLD_TP", lineTransparencyVal);
		ss->set("NEW_TP", newTransparency);
		undoManager->action(this, ss);
	}
	lineTransparencyVal = newTransparency;
}

Qt::PenStyle PageItem::lineStyle() const
{
	return PLineArt;
}

void PageItem::setLineStyle(PenStyle newStyle)
{
	if (PLineArt == newStyle)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LineStyle,"",Um::ILineStyle);
		ss->set("LINE_STYLE", "line_style");
		ss->set("OLD_STYLE", static_cast<int>(PLineArt));
		ss->set("NEW_STYLE", static_cast<int>(newStyle));
		undoManager->action(this, ss);
	}
	PLineArt = newStyle;
}

double PageItem::lineWidth() const
{
	return Pwidth;
}

void PageItem::setLineWidth(double newWidth)
{
	if (Pwidth == newWidth)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LineWidth,
						QString(Um::FromTo).arg(Pwidth).arg(newWidth),Um::ILineStyle);
		ss->set("LINE_WIDTH", "line_width");
		ss->set("OLD_WIDTH", Pwidth);
		ss->set("NEW_WIDTH", newWidth);
		undoManager->action(this, ss);
	}
	Pwidth = newWidth;
}

Qt::PenCapStyle PageItem::lineEnd() const
{
	return PLineEnd;
}

void PageItem::setLineEnd(PenCapStyle newStyle)
{
	if (PLineEnd == newStyle)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LineEnd,"",Um::ILineStyle);
		ss->set("LINE_END", "line_end");
		ss->set("OLD_STYLE", static_cast<int>(PLineEnd));
		ss->set("NEW_STYLE", static_cast<int>(newStyle));
		undoManager->action(this, ss);
	}
	PLineEnd = newStyle;
}

Qt::PenJoinStyle PageItem::lineJoin() const
{
	return PLineJoin;
}

void PageItem::setLineJoin(PenJoinStyle newStyle)
{
	if (PLineJoin == newStyle)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LineJoin,"",Um::ILineStyle);
		ss->set("LINE_JOIN", "line_join");
		ss->set("OLD_STYLE", static_cast<int>(PLineJoin));
		ss->set("NEW_STYLE", static_cast<int>(newStyle));
		undoManager->action(this, ss);
	}
	PLineJoin = newStyle;
}
QString PageItem::customLineStyle() const
{
	return NamedLStyle;
}

void PageItem::setCustomLineStyle(const QString& newStyle)
{
	if (NamedLStyle == newStyle)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		QString oldStyle = NamedLStyle == "" ? Um::NoStyle : NamedLStyle;
		QString nStyle   = newStyle == "" ? Um::NoStyle : newStyle;
		QString action   = newStyle == "" ? Um::NoLineStyle : Um::CustomLineStyle;
		SimpleState *ss = new SimpleState(action,
				QString(Um::FromTo).arg(oldStyle).arg(nStyle),Um::ILineStyle);
		ss->set("CUSTOM_LINE_STYLE", "customlinestyle");
		ss->set("OLD_STYLE", NamedLStyle);
		ss->set("NEW_STYLE", newStyle);
		undoManager->action(this, ss);
	}
	NamedLStyle = newStyle;
}

int PageItem::getStartArrowIndex() const
{
	return startArrowIndex;
}

void PageItem::setStartArrowIndex(int newIndex)
{
	if (startArrowIndex == newIndex)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::StartArrow,"",Um::IArrow);
		ss->set("START_ARROW", "startarrow");
		ss->set("OLD_INDEX", startArrowIndex);
		ss->set("NEW_INDEX", newIndex);
		undoManager->action(this, ss);
	}
	startArrowIndex = newIndex;
}

int PageItem::getEndArrowIndex() const
{
	return endArrowIndex;
}

void PageItem::setEndArrowIndex(int newIndex)
{
	if (endArrowIndex == newIndex)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::EndArrow,"",Um::IArrow);
		ss->set("END_ARROW", "endarrow");
		ss->set("OLD_INDEX", endArrowIndex);
		ss->set("NEW_INDEX", newIndex);
		undoManager->action(this, ss);
	}
	endArrowIndex = newIndex;
}

bool PageItem::imageFlippedH() const
{
	return imageIsFlippedH;
}

void PageItem::setImageFlippedH(bool flipped)
{
	if (flipped != imageIsFlippedH)
		flipImageH();
}

void PageItem::flipImageH()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, 0, Um::IFlipH);
		ss->set("IMAGEFLIPH", "imagefliph");
		undoManager->action(this, ss);
	}
	imageIsFlippedH = !imageIsFlippedH;
}

bool PageItem::imageFlippedV() const
{
	return imageIsFlippedV;
}

void PageItem::setImageFlippedV(bool flipped)
{
	if (flipped != imageIsFlippedV)
		flipImageV();
}

void PageItem::flipImageV()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipV, 0, Um::IFlipV);
		ss->set("IMAGEFLIPV", "imageflipv");
		undoManager->action(this, ss);
	}
	imageIsFlippedV = !imageIsFlippedV;
}

void PageItem::setImageScalingMode(bool freeScale, bool keepRatio)
{
	if (ScaleType == freeScale && AspectRatio == keepRatio)
		return;
	if (UndoManager::undoEnabled())
	{
		QString from = ScaleType ? Um::FreeScaling : Um::FrameSize;
		from += ", ";
		from += AspectRatio ? Um::KeepRatio : Um::BreakRatio;
		QString to = freeScale ? Um::FreeScaling : Um::FrameSize;
		to += ", ";
		to += keepRatio ? Um::KeepRatio : Um::BreakRatio;
		SimpleState *ss = new SimpleState(Um::ImageScaling, QString(Um::FromTo).arg(from).arg(to), Um::IImageScaling);
		if (freeScale != ScaleType)
			ss->set("SCALE_TYPE", freeScale);
		if (keepRatio != AspectRatio)
			ss->set("ASPECT_RATIO", keepRatio);
		undoManager->action(this, ss);
	}
	ScaleType = freeScale;
	AspectRatio = keepRatio;
	ScApp->view->AdjustPictScale(this);
	ScApp->view->RefreshItem(this);
}

void PageItem::toggleLock()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss;
		if (Locked)
			ss = new SimpleState(Um::UnLock, 0, Um::IUnLock);
		else
			ss = new SimpleState(Um::Lock, 0, Um::ILock);
		ss->set("LOCK", "lock");
		undoManager->action(this, ss);
	}
	Locked = !Locked;
}

bool PageItem::locked() const
{
	return Locked;
}

void PageItem::setLocked(bool isLocked)
{
	if (isLocked != Locked)
		toggleLock();
}

void PageItem::toggleSizeLock()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss;
		if (Locked)
			ss = new SimpleState(Um::SizeUnLock, 0, Um::IUnLock);
		else
			ss = new SimpleState(Um::SizeLock, 0, Um::ILock);
		ss->set("SIZE_LOCK", "size_lock");
		undoManager->action(this, ss);
	}
	LockRes = !LockRes;
}

bool PageItem::sizeLocked() const
{
	return LockRes;
}

void PageItem::setSizeLocked(bool isLocked)
{
	if (isLocked == LockRes)
		return; // nothing to do return
	else
		toggleSizeLock();
}

QString PageItem::font() const
{
	return IFont;
}

void PageItem::setFont(const QString& newFont)
{
	if (IFont == newFont)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFont,
										  QString(Um::FromTo).arg(IFont).arg(newFont), Um::IFont);
		ss->set("SET_FONT", "setfont");
		ss->set("OLD_FONT", IFont);
		ss->set("NEW_FONT", newFont);
		undoManager->action(this, ss);
	}
	IFont = newFont;
}

int PageItem::fontSize() const
{
	return ISize;
}

void PageItem::setFontSize(int newSize)
{
	if (ISize == newSize)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontSize,
										  QString(Um::FromTo).arg(ISize/10.0).arg(newSize/10.0), Um::IFont);
		ss->set("SET_FONT_SIZE", "setfontsize");
		ss->set("OLD_SIZE", ISize);
		ss->set("NEW_SIZE", newSize);
		undoManager->action(this, ss);
	}
	ISize = newSize;
}

int PageItem::fontHeight() const
{
	return TxtScaleV;
}

void PageItem::setFontHeight(int newHeight)
{
	if (TxtScaleV == newHeight)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontHeight,
										  QString(Um::FromTo).arg(TxtScaleV).arg(newHeight), Um::IFont);
		ss->set("SET_FONT_HEIGHT", "setfontheight");
		ss->set("OLD_HEIGHT", TxtScaleV);
		ss->set("NEW_HEIGHT", newHeight);
		undoManager->action(this, ss);
	}
	TxtScaleV = newHeight;
}

int PageItem::fontWidth() const
{
	return TxtScale;
}

void PageItem::setFontWidth(int newWidth)
{
	if (TxtScale == newWidth)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontWidth,
										  QString(Um::FromTo).arg(TxtScale).arg(newWidth), Um::IFont);
		ss->set("SET_FONT_WIDTH", "setfontwidth");
		ss->set("OLD_WIDTH", TxtScale);
		ss->set("NEW_WIDTH", newWidth);
		undoManager->action(this, ss);
	}
	TxtScale = newWidth;
}

QString PageItem::fontFillColor() const
{
	return TxtFill;
}

void PageItem::setFontFillColor(const QString& newColor)
{
	if (TxtFill == newColor)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontFill,
										  QString(Um::FromTo).arg(TxtFill).arg(newColor), Um::IFont);
		ss->set("SET_FONT_FILL", "setfontfill");
		ss->set("OLD_FILL", TxtFill);
		ss->set("NEW_FILL", newColor);
		undoManager->action(this, ss);
	}
	TxtFill = newColor;
}

QString PageItem::fontStrokeColor() const
{
	return TxtStroke;
}

void PageItem::setFontStrokeColor(const QString& newColor)
{
	if (TxtStroke == newColor)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontStroke,
										  QString(Um::FromTo).arg(TxtStroke).arg(newColor), Um::IFont);
		ss->set("SET_FONT_STROKE", "setfontstroke");
		ss->set("OLD_STROKE", TxtStroke);
		ss->set("NEW_STROKE", newColor);
		undoManager->action(this, ss);
	}
	TxtStroke = newColor;
}

int PageItem::fontFillShade() const
{
	return ShTxtFill;
}

void PageItem::setFontFillShade(int newShade)
{
	if (ShTxtFill == newShade)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontFillShade,
										  QString(Um::FromTo).arg(ShTxtFill).arg(newShade),
										  Um::IFont);
		ss->set("FONT_FILL_SHADE", "line_shade");
		ss->set("OLD_SHADE", ShTxtFill);
		ss->set("NEW_SHADE", newShade);
		undoManager->action(this, ss);
	}
	ShTxtFill = newShade;
}

int PageItem::fontStrokeShade() const
{
	return ShTxtStroke;
}

void PageItem::setFontStrokeShade(int newShade)
{
	if (ShTxtStroke == newShade)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontStrokeShade,
										  QString(Um::FromTo).arg(ShTxtStroke).arg(newShade),
										  Um::IFont);
		ss->set("FONT_STROKE_SHADE", "line_shade");
		ss->set("OLD_SHADE", ShTxtStroke);
		ss->set("NEW_SHADE", newShade);
		undoManager->action(this, ss);
	}
	ShTxtStroke = newShade;
}

int PageItem::fontEffects() const
{
	return TxTStyle;
}

void PageItem::setFontEffects(int newEffects)
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFontEffect, "", Um::IFont);
		ss->set("FONT_EFFECTS", "fonteffects");
		ss->set("OLD_EFFECT", TxTStyle);
		ss->set("NEW_EFFECT", newEffects);
		undoManager->action(this, ss);
	}
	TxTStyle &= ~1919;
	TxTStyle |= newEffects;
}

int PageItem::kerning() const
{
	return ExtraV;
}

void PageItem::setKerning(int newKerning)
{
	if (ExtraV == newKerning)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetKerning,
										  QString(Um::FromTo).arg(ExtraV).arg(newKerning),
										  Um::IFont);
		ss->set("KERNING", "kerning");
		ss->set("OLD_KERNING", ExtraV);
		ss->set("NEW_KERNING", newKerning);
		undoManager->action(this, ss);
	}
	ExtraV = newKerning;
}

double PageItem::lineSpacing() const
{
	return LineSp;
}

void PageItem::setLineSpacing(double newSpacing)
{
	if (LineSp == newSpacing)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetLineSpacing,
										  QString(Um::FromTo).arg(LineSp).arg(newSpacing),
										  Um::IFont);
		ss->set("SPACING", "spacing");
		ss->set("OLD_SPACING", LineSp);
		ss->set("NEW_SPACING", newSpacing);
		undoManager->action(this, ss);
	}
	LineSp = newSpacing;
}

QString PageItem::language() const
{
	return Language;
}

void PageItem::setLanguage(const QString& newLanguage)
{
	if (Language == newLanguage)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetLanguage,
										  QString(Um::FromTo).arg(Language).arg(newLanguage),
										  Um::IFont);
		ss->set("LANGUAGE", "lang");
		ss->set("OLD_LANG", Language);
		ss->set("NEW_LANG", newLanguage);
		undoManager->action(this, ss);
	}
	Language = newLanguage;
}

bool PageItem::textFlowsAroundFrame() const
{
	return this->textFlowsAroundFrameVal;
}

void PageItem::setTextFlowsAroundFrame(bool isFlowing)
{
	if (textFlowsAroundFrameVal == isFlowing)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(isFlowing ? Um::TextFlow : Um::NoTextFlow, "", Um::IFont);
		ss->set("TEXT_FLOW", isFlowing);
		undoManager->action(this, ss);
	}
	textFlowsAroundFrameVal = isFlowing;
}

bool PageItem::textFlowUsesBoundingBox() const
{
	return this->textFlowUsesBoundingBoxVal;
}

void PageItem::setTextFlowUsesBoundingBox(bool useBounding)
{
	if (textFlowUsesBoundingBoxVal == useBounding)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(useBounding ? Um::BoundingBox : Um::NoBoundingBox, "", Um::IFont);
		ss->set("TEXT_FLOW", textFlowsAroundFrame());
		ss->set("BOUNDING_BOX", useBounding);
		undoManager->action(this, ss);
	}
	if (useBounding && textFlowUsesContourLineVal)
		textFlowUsesContourLineVal = false;
	textFlowUsesBoundingBoxVal = useBounding;
}

bool PageItem::textFlowUsesContourLine() const
{
	return this->textFlowUsesContourLineVal;
}

void PageItem::setTextFlowUsesContourLine(bool useContour)
{
	if (textFlowUsesContourLineVal == useContour)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(useContour ? Um::ContourLine : Um::NoContourLine, "", Um::IFont);
		ss->set("TEXT_FLOW", textFlowsAroundFrame());
		ss->set("CONTOUR_LINE", useContour);
		undoManager->action(this, ss);
	}
	if (useContour && textFlowUsesBoundingBoxVal)
		textFlowUsesBoundingBoxVal = false;
	textFlowUsesContourLineVal = useContour;
}

PageItem::ItemType PageItem::itemType() const
{
	return itemTypeVal;
}

void PageItem::convertTo(ItemType newType)
{
	if (itemTypeVal == newType)
		return; // nothing to do -> return
	assert(newType != 1);	//DEBUG CR 2005-02-06
	assert(newType != 3);	//DEBUG CR 2005-02-06
	QString fromType = "", toType = "";
	switch (itemTypeVal)
	{
		case ImageFrame:
			fromType = Um::ImageFrame;
			break;
		case TextFrame:
			fromType = Um::TextFrame;
			break;
		case Polygon:
			fromType = Um::Polygon;
			break;
		default:
			fromType = "";
			break;
	}
	switch (newType)
	{
		case ImageFrame:
			toType = Um::ImageFrame;
			setUPixmap(Um::IImageFrame);
			break;
		case TextFrame:
			toType = Um::TextFrame;
			setUPixmap(Um::ITextFrame);
			break;
		case Polygon:
			toType = Um::Polygon;
			setUPixmap(Um::IPolygon);
			break;
		case PolyLine:
			toType = Um::Polyline;
			setUPixmap(Um::IPolyline);
			break;
		default:
			toType = "";
			setUPixmap(NULL);
			break;
	}
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ConvertTo + " " + toType,
										  QString(Um::FromTo).arg(fromType).arg(toType));
		ss->set("CONVERT", "convert");
		ss->set("OLD_TYPE", itemTypeVal);
		ss->set("NEW_TYPE", newType);
		undoManager->action(this, ss);
	}
	itemTypeVal = newType;
}

void PageItem::setLayer(int layerId)
{
	if (LayerNr == layerId)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SendToLayer,
										  QString(Um::FromTo).arg(LayerNr).arg(layerId),
										  Um::ILayerAction);
		ss->set("SEND_TO_LAYER", "send_to_layer");
		ss->set("OLD_LAYER", LayerNr);
		ss->set("NEW_LAYER", layerId);
		undoManager->action(this, ss);
	}
	LayerNr = layerId;
}

void PageItem::checkChanges(bool force)
{
	// has the item been resized
	if ((force) || ((oldWidth != Width || oldHeight != Height) && (shouldCheck())))
		resizeUndoAction();
	// has the item been rotated
	if ((force) || ((oldRot != Rot) && (shouldCheck())))
		rotateUndoAction();
	// has the item been moved
	if ((force) || ((oldXpos != Xpos || oldYpos != Ypos) && (shouldCheck())))
		moveUndoAction();
}

bool PageItem::shouldCheck()
{
	return ((!ScApp->view->mousePressed()) &&
			(!ScApp->arrowKeyDown()) &&
			(!ScApp->propertiesPalette->userActionOn()));
}

void PageItem::moveUndoAction()
{
	if (!shouldCheck())
		return;
	if (oldXpos == Xpos && oldYpos == Ypos)
		return;
	if (UndoManager::undoEnabled())
	{
		QString oldp;
		QString newp;
		if (oldOwnPage == -1)
			oldp = Um::ScratchSpace;
		else
			oldp = QString(Um::PageNmbr).arg(ScApp->doc->FirstPnum + oldOwnPage);
		if (OwnPage == -1)
			newp = Um::ScratchSpace;
		else
			newp = QString(Um::PageNmbr).arg(ScApp->doc->FirstPnum + OwnPage);
		SimpleState *ss = new SimpleState(Um::Move,
                                          QString(Um::MoveFromTo).arg(oldXpos).arg(oldYpos).arg(oldp).
                                                                  arg(Xpos).arg(Ypos).arg(newp), Um::IMove);
		ss->set("OLD_XPOS", oldXpos);
		ss->set("OLD_YPOS", oldYpos);
		ss->set("NEW_XPOS", Xpos);
		ss->set("NEW_YPOS", Ypos);
		undoManager->action(this, ss);
	}
	oldXpos = Xpos;
	oldYpos = Ypos;
	oldOwnPage = OwnPage;
}

void PageItem::resizeUndoAction()
{
	if (!shouldCheck())
		return;
	if (oldHeight == Height && oldWidth == Width)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Resize,
                           QString(Um::ResizeFromTo).arg(oldWidth).arg(oldHeight).arg(Width).arg(Height),
                                          Um::IResize);
		ss->set("OLD_WIDTH", oldWidth);
		ss->set("OLD_HEIGHT", oldHeight);
		ss->set("NEW_WIDTH", Width);
		ss->set("NEW_HEIGHT", Height);
		ss->set("OLD_RXPOS", oldXpos);
		ss->set("OLD_RYPOS", oldYpos);
		ss->set("NEW_RXPOS", Xpos);
		ss->set("NEW_RYPOS", Ypos);
		ss->set("OLD_RROT", oldRot);
		ss->set("NEW_RROT", Rot);
		undoManager->action(this, ss);
	}
	oldXpos = Xpos;
	oldYpos = Ypos;
	oldHeight = Height;
	oldWidth = Width;
	oldOwnPage = OwnPage;
	oldRot = Rot;
}

void PageItem::rotateUndoAction()
{
	if (!shouldCheck())
		return;
	if (oldRot == Rot)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Rotate,
                                          QString(Um::FromTo).arg(oldRot).arg(Rot),
                                          Um::IRotate);
		ss->set("OLD_ROT", oldRot);
		ss->set("NEW_ROT", Rot);
		ss->set("OLD_RXPOS", oldXpos);
		ss->set("OLD_RYPOS", oldYpos);
		ss->set("NEW_RXPOS", Xpos);
		ss->set("NEW_RYPOS", Ypos);
		ss->set("OLD_RWIDTH", oldWidth);
		ss->set("OLD_RHEIGHT", oldHeight);
		ss->set("NEW_RWIDTH", Width);
		ss->set("NEW_RHEIGHT", Height);
		undoManager->action(this, ss);
	}
	oldRot = Rot;
	oldXpos = Xpos;
	oldYpos = Ypos;
	oldOwnPage = OwnPage;
	oldWidth = Width;
	oldHeight = Height;
}

void PageItem::restore(UndoState *state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
		if (ss->contains("OLD_XPOS"))
			restoreMove(ss, isUndo);
		else if (ss->contains("OLD_HEIGHT"))
			restoreResize(ss, isUndo);
		else if (ss->contains("OLD_ROT"))
			restoreRotate(ss, isUndo);
		else if (ss->contains("FILL"))
			restoreFill(ss, isUndo);
		else if (ss->contains("SHADE"))
			restoreShade(ss, isUndo);
		else if (ss->contains("LINE_COLOR"))
			restoreLineColor(ss, isUndo);
		else if (ss->contains("LINE_SHADE"))
			restoreLineShade(ss, isUndo);
		else if (ss->contains("IMAGEFLIPH"))
		{
			select();
			ScApp->view->FlipImageH();
		}
		else if (ss->contains("IMAGEFLIPV"))
		{
			select();
			ScApp->view->FlipImageV();
		}
		else if (ss->contains("LOCK"))
		{
			select();
			ScApp->view->ToggleLock();
		}
		else if (ss->contains("SIZE_LOCK"))
		{
			select();
			ScApp->view->ToggleSizeLock();
		}
		else if (ss->contains("NEW_NAME"))
			restoreName(ss, isUndo);
		else if (ss->contains("TRANSPARENCY"))
			restoreFillTP(ss, isUndo);
		else if (ss->contains("LINE_TRANSPARENCY"))
			restoreLineTP(ss, isUndo);
		else if (ss->contains("LINE_STYLE"))
			restoreLineStyle(ss, isUndo);
		else if (ss->contains("LINE_END"))
			restoreLineEnd(ss, isUndo);
		else if (ss->contains("LINE_JOIN"))
			restoreLineJoin(ss, isUndo);
		else if (ss->contains("LINE_WIDTH"))
			restoreLineWidth(ss, isUndo);
		else if (ss->contains("CUSTOM_LINE_STYLE"))
			restoreCustomLineStyle(ss, isUndo);
		else if (ss->contains("START_ARROW"))
			restoreArrow(ss, isUndo, true);
		else if (ss->contains("END_ARROW"))
			restoreArrow(ss, isUndo, false);
		else if (ss->contains("SET_FONT"))
			restoreFont(ss, isUndo);
		else if (ss->contains("SET_FONT_SIZE"))
			restoreFontSize(ss, isUndo);
		else if (ss->contains("SET_FONT_WIDTH"))
			restoreFontWidth(ss, isUndo);
		else if (ss->contains("SET_FONT_FILL"))
			restoreFontFill(ss, isUndo);
		else if (ss->contains("SET_FONT_STROKE"))
			restoreFontStroke(ss, isUndo);
		else if (ss->contains("FONT_FILL_SHADE"))
			restoreFontFillShade(ss, isUndo);
		else if (ss->contains("FONT_STROKE_SHADE"))
			restoreFontStrokeShade(ss, isUndo);
		else if (ss->contains("LANGUAGE"))
			restoreLanguage(ss, isUndo);
		else if (ss->contains("KERNING"))
			restoreKerning(ss, isUndo);
		else if (ss->contains("SPACING"))
			restoreLineSpacing(ss, isUndo);
		else if (ss->contains("PSTYLE"))
			restorePStyle(ss, isUndo);
		else if (ss->contains("FONT_EFFECTS"))
			restoreFontEffect(ss, isUndo);
		else if (ss->contains("CONVERT"))
			restoreType(ss, isUndo);
		else if (ss->contains("TEXT_FLOW"))
			restoreTextFlowing(ss, isUndo);
		else if (ss->contains("SCALE_TYPE"))
			restoreImageScaling(ss, isUndo);
		else if (ss->contains("ASPECT_RATIO"))
			restoreImageScaling(ss, isUndo);
		else if (ss->contains("EDIT_CONTOUR"))
			restorePoly(ss, isUndo, true);
		else if (ss->contains("EDIT_SHAPE"))
			restorePoly(ss, isUndo, false);
		else if (ss->contains("RESET_CONTOUR"))
			restoreContourLine(ss, isUndo);
		else if (ss->contains("MIRROR_PATH_H"))
		{
			bool editContour = ScApp->view->EditContour;
			ScApp->view->EditContour = ss->getBool("IS_CONTOUR");
			select();
			ScApp->view->MirrorPolyH();
			ScApp->view->EditContour = editContour;
		}
		else if (ss->contains("MIRROR_PATH_V"))
		{
			bool editContour = ScApp->view->EditContour;
			ScApp->view->EditContour = ss->getBool("IS_CONTOUR");
			select();
			ScApp->view->MirrorPolyV();
			ScApp->view->EditContour = editContour;
		}
		else if (ss->contains("SEND_TO_LAYER"))
			restoreLayer(ss, isUndo);
		else if (ss->contains("GET_IMAGE"))
			restoreGetImage(ss, isUndo);
	}
}

void PageItem::restoreMove(SimpleState *state, bool isUndo)
{
	double ox = state->getDouble("OLD_XPOS");
	double oy = state->getDouble("OLD_YPOS");
	double  x = state->getDouble("NEW_XPOS");
	double  y = state->getDouble("NEW_YPOS");
	double mx = ox - x;
	double my = oy - y;
	if (!isUndo)
	{
		mx = x - ox;
		my = y - oy;
	}
	ScApp->view->MoveItem(mx, my, this, false);
	oldXpos = Xpos;
	oldYpos = Ypos;
	oldOwnPage = OwnPage;
}

void PageItem::restoreResize(SimpleState *state, bool isUndo)
{
	double  ow = state->getDouble("OLD_WIDTH");
	double  oh = state->getDouble("OLD_HEIGHT");
	double   w = state->getDouble("NEW_WIDTH");
	double   h = state->getDouble("NEW_HEIGHT");
	double  ox = state->getDouble("OLD_RXPOS");
	double  oy = state->getDouble("OLD_RYPOS");
	double   x = state->getDouble("NEW_RXPOS");
	double   y = state->getDouble("NEW_RYPOS");
	double ort = state->getDouble("OLD_RROT");
	double  rt = state->getDouble("NEW_RROT");
	double  mx = ox - x;
	double  my = oy - y;
	if (isUndo)
	{
		ScApp->view->SizeItem(ow, oh, this, false, true, true);
		ScApp->view->MoveItem(mx, my, this, false);
		ScApp->view->RotateItem(ort, this);
	}
	else
	{
		mx = x - ox;
		my = y - oy;
		ScApp->view->SizeItem(w, h, this, false, true, true);
		ScApp->view->MoveItem(mx, my, this, false);
		ScApp->view->RotateItem(rt, this);
	}
	oldWidth = Width;
	oldHeight = Height;
	oldXpos = Xpos;
	oldYpos = Ypos;
	oldOwnPage = OwnPage;
	oldRot = Rot;
}

void PageItem::restoreRotate(SimpleState *state, bool isUndo)
{
	double ort = state->getDouble("OLD_ROT");
	double  rt = state->getDouble("NEW_ROT");
	double  ox = state->getDouble("OLD_RXPOS");
	double  oy = state->getDouble("OLD_RYPOS");
	double   x = state->getDouble("NEW_RXPOS");
	double   y = state->getDouble("NEW_RYPOS");
	double  ow = state->getDouble("OLD_RWIDTH");
	double  oh = state->getDouble("OLD_RHEIGHT");
	double   w = state->getDouble("NEW_RWIDTH");
	double   h = state->getDouble("NEW_RHEIGHT");
	double mx = ox - x;
	double my = oy - y;
	if (isUndo)
	{
		ScApp->view->RotateItem(ort, this);
		ScApp->view->MoveItem(mx, my, this, false);
		ScApp->view->SizeItem(ow, oh, this, false, true, true);
	}
	else
	{
		mx = x - ox;
		my = y - oy;
		ScApp->view->RotateItem(rt, this);
		ScApp->view->MoveItem(mx, my, this, false);
		ScApp->view->SizeItem(w, h, this, false, true, true);
	}
	oldRot = Rot;
	oldXpos = Xpos;
	oldYpos = Ypos;
	oldOwnPage = OwnPage;
	oldWidth = Width;
	oldHeight = Height;
}

void PageItem::restoreFill(SimpleState *state, bool isUndo)
{
	QString fill = state->get("OLD_FILL");
	if (!isUndo)
		fill = state->get("NEW_FILL");
	select();
	ScApp->view->ItemBrush(fill);
}

void PageItem::restoreShade(SimpleState *state, bool isUndo)
{
	int shade = state->getInt("OLD_SHADE");
	if (!isUndo)
		shade = state->getInt("NEW_SHADE");
	select();
	ScApp->view->ItemBrushShade(shade);
}

void PageItem::restoreLineColor(SimpleState *state, bool isUndo)
{
	QString fill = state->get("OLD_COLOR");
	if (!isUndo)
		fill = state->get("NEW_COLOR");
	select();
	ScApp->view->ItemPen(fill);
}

void PageItem::restoreLineShade(SimpleState *state, bool isUndo)
{
	int shade = state->getInt("OLD_SHADE");
	if (!isUndo)
		shade = state->getInt("NEW_SHADE");
	select();
	ScApp->view->ItemPenShade(shade);
}

void PageItem::restoreFillTP(SimpleState *state, bool isUndo)
{
	double tp = state->getDouble("OLD_TP");
	if (!isUndo)
		tp = state->getDouble("NEW_TP");
	select();
	ScApp->setItemFillTransparency(tp);
}

void PageItem::restoreLineTP(SimpleState *state, bool isUndo)
{
	double tp = state->getDouble("OLD_TP");
	if (!isUndo)
		tp = state->getDouble("NEW_TP");
	select();
	ScApp->setItemLineTransparency(tp);
}


void PageItem::restoreLineStyle(SimpleState *state, bool isUndo)
{
	PenStyle ps = static_cast<PenStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		ps = static_cast<PenStyle>(state->getInt("NEW_STYLE"));
	select();
	ScApp->view->ChLineArt(ps);
}

void PageItem::restoreLineEnd(SimpleState *state, bool isUndo)
{
	PenCapStyle pcs = static_cast<PenCapStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		pcs = static_cast<PenCapStyle>(state->getInt("NEW_STYLE"));
	select();
	ScApp->view->ChLineEnd(pcs);
}

void PageItem::restoreLineJoin(SimpleState *state, bool isUndo)
{
	PenJoinStyle pjs = static_cast<PenJoinStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		pjs = static_cast<PenJoinStyle>(state->getInt("NEW_STYLE"));
	select();
	ScApp->view->ChLineJoin(pjs);
}

void PageItem::restoreLineWidth(SimpleState *state, bool isUndo)
{
	double w = state->getDouble("OLD_WIDTH");
	if (!isUndo)
		w = state->getDouble("NEW_WIDTH");
	select();
	ScApp->view->ChLineWidth(w);
}

void PageItem::restoreCustomLineStyle(SimpleState *state, bool isUndo)
{
	QString style = state->get("OLD_STYLE");
	if (!isUndo)
		style = state->get("NEW_STYLE");
	setCustomLineStyle(style);
}

void PageItem::restoreName(SimpleState *state, bool isUndo)
{
	QString oldName = state->get("OLD_NAME");
	QString newName = state->get("NEW_NAME");
	if (isUndo)
		setItemName(oldName);
	else
		setItemName(newName);
}

void PageItem::restoreArrow(SimpleState *state, bool isUndo, bool isStart)
{
	int i = state->getInt("OLD_INDEX");
	if (!isUndo)
		i = state->getInt("NEW_INDEX");
	if (isStart)
		setStartArrowIndex(i);
	else
		setEndArrowIndex(i);
}

void PageItem::restoreFont(SimpleState *state, bool isUndo)
{
	QString font = state->get("OLD_FONT");
	if (!isUndo)
		font = state->get("NEW_FONT");
	select();
	ScApp->view->ItemFont(font);
}

void PageItem::restoreFontSize(SimpleState *state, bool isUndo)
{
	int size = state->getInt("OLD_SIZE");
	if (!isUndo)
		size = state->getInt("NEW_SIZE");
	select();
	ScApp->view->chFSize(size);
}

void PageItem::restoreFontWidth(SimpleState *state, bool isUndo)
{
	int width = state->getInt("OLD_WIDTH");
	if (!isUndo)
		width = state->getInt("NEW_WIDTH");
	select();
	ScApp->view->ItemTextScale(width);
}

void PageItem::restoreFontFill(SimpleState *state, bool isUndo)
{
	QString color = state->get("OLD_FILL");
	if (!isUndo)
		color = state->get("NEW_FILL");
	select();
	ScApp->view->ItemTextBrush(color);
}

void PageItem::restoreFontStroke(SimpleState *state, bool isUndo)
{
	QString color = state->get("OLD_STROKE");
	if (!isUndo)
		color = state->get("NEW_STROKE");
	select();
	ScApp->view->ItemTextPen(color);
}

void PageItem::restoreFontFillShade(SimpleState *state, bool isUndo)
{
	int shade = state->getInt("OLD_SHADE");
	if (!isUndo)
		shade = state->getInt("NEW_SHADE");
	select();
	ScApp->view->ItemTextBrushS(shade);
}

void PageItem::restoreFontStrokeShade(SimpleState *state, bool isUndo)
{
	int shade = state->getInt("OLD_SHADE");
	if (!isUndo)
		shade = state->getInt("NEW_SHADE");
	select();
	ScApp->view->ItemTextPenS(shade);
}

void PageItem::restoreKerning(SimpleState *state, bool isUndo)
{
	int kerning = state->getInt("OLD_KERNING");
	if (!isUndo)
		kerning = state->getInt("NEW_KERNING");
	select();
	ScApp->view->chKerning(kerning);
}

void PageItem::restoreLineSpacing(SimpleState *state, bool isUndo)
{
	double lsp = state->getDouble("OLD_SPACING");
	if (!isUndo)
		lsp = state->getDouble("NEW_SPACING");
	select();
	ScApp->view->ChLineSpa(lsp);
}

void PageItem::restoreLanguage(SimpleState *state, bool isUndo)
{
	QString lang = state->get("OLD_LANG");
	if (!isUndo)
		lang = state->get("NEW_LANG");
	setLanguage(lang);
}

void PageItem::restorePStyle(SimpleState *state, bool isUndo)
{
	int styleid = state->getInt("OLD_STYLE");
	if (!isUndo)
		styleid = state->getInt("NEW_STYLE");
	ScApp->view->chAbStyle(this, styleid);
}

void PageItem::restoreFontEffect(SimpleState *state, bool isUndo)
{
	int effect = state->getInt("OLD_EFFECT");
	if (!isUndo)
		effect = state->getInt("NEW_EFFECT");
	select();
	ScApp->view->chTyStyle(effect);
}

void PageItem::restoreType(SimpleState *state, bool isUndo)
{
	int type = state->getInt("OLD_TYPE");
	if (!isUndo)
		type = state->getInt("NEW_TYPE");
	select();
	switch (type) {
		case ImageFrame: ScApp->view->ToPicFrame(); break;
		case TextFrame: ScApp->view->ToTextFrame(); break;
		case Polygon: ScApp->view->ToPolyFrame(); break;
		case PolyLine: ScApp->view->ToBezierFrame(); break;
	}
	ScApp->setAppMode(NormalMode);
}

void PageItem::restoreTextFlowing(SimpleState *state, bool isUndo)
{
	if (state->contains("BOUNDING_BOX"))
	{
		if (isUndo)
			textFlowUsesBoundingBoxVal = !state->getBool("BOUNDING_BOX");
		else
			textFlowUsesBoundingBoxVal = state->getBool("BOUNDING_BOX");
	}
	else if (state->contains("CONTOUR_LINE"))
	{
		if (isUndo)
			textFlowUsesContourLineVal = !state->getBool("CONTOUR_LINE");
		else
			textFlowUsesContourLineVal = state->getBool("CONTOUR_LINE");
	}
	else
	{
		if (isUndo)
			textFlowsAroundFrameVal = !state->getBool("TEXT_FLOW");
		else
			textFlowsAroundFrameVal = state->getBool("TEXT_FLOW");
	}
}

void PageItem::restoreImageScaling(SimpleState *state, bool isUndo)
{
	bool type, ratio;
	if (state->contains("SCALE_TYPE"))
	{
		if (isUndo)
			type = !state->getBool("SCALE_TYPE");
		else
			type = state->getBool("SCALE_TYPE");
	}
	else
		type = ScaleType;
	if (state->contains("ASPECT_RATIO"))
	{
		if (isUndo)
			ratio = !state->getBool("ASPECT_RATIO");
		else
			ratio = state->getBool("ASPECT_RATIO");
	}
	else
		ratio = AspectRatio;

	setImageScalingMode(type, ratio);
}

void PageItem::restorePoly(SimpleState *state, bool isUndo, bool isContour)
{
	int mode    = state->getInt("MODE");
	int rot     = state->getInt("ROT");
	double scaling = state->getDouble("SCALING");
	bool editContour = ScApp->view->EditContour;
	ScApp->view->EditContour = isContour;
	select();
	if (isUndo)
	{
		if (mode % 2 != 0 && mode != 0)
			--mode;
		else
			++mode;
		if (mode == 2)
			scaling = (1.0 - (100.0 / (100.0 + scaling))) * 100.0;
		else if (mode == 3)
			scaling = ((100.0 / (100.0 - scaling)) - 1.0) * 100.0;
		ScApp->view->TransformPoly(mode, rot, scaling);
	}
	else
		ScApp->view->TransformPoly(mode, rot, scaling);

	ScApp->view->EditContour = editContour;
}

void PageItem::restoreContourLine(SimpleState *state, bool isUndo)
{
	ItemState<FPointArray> *is = dynamic_cast<ItemState<FPointArray>*>(state);
	if (is)
	{
		if (isUndo)
		{
			ContourLine = is->getItem();
			ClipEdited = true;
		}
		else
		{
			ContourLine = PoLine.copy();
			ClipEdited = true;
		}
	}
}

void PageItem::restoreLayer(SimpleState *state, bool isUndo)
{
	setLayer(isUndo ? state->getInt("OLD_LAYER") : state->getInt("NEW_LAYER"));
	ScApp->view->Deselect(true);
	ScApp->view->updateContents();
}

void PageItem::restoreGetImage(SimpleState *state, bool isUndo)
{
	QString fn = state->get("OLD_IMAGE_PATH");
	if (!isUndo)
		fn = state->get("NEW_IMAGE_PATH");
	if (fn == "")
	{
		select();
		ScApp->view->ClearItem();
	}
	else
		ScApp->view->loadPict(fn, this, false);
}

void PageItem::select()
{
	ScApp->view->Deselect(false);
	ScApp->view->SelectItem(this, false);
}

ObjAttrVector* PageItem::getObjectAttributes()
{
	return &pageItemAttributes;
}

ObjectAttribute PageItem::getObjectAttribute(QString attributeName)
{
	int countFound=0;
	ObjAttrVector::Iterator foundIt;
	for(ObjAttrVector::Iterator objAttrIt = pageItemAttributes.begin() ; objAttrIt != pageItemAttributes.end(); ++objAttrIt )
	{
		if ((*objAttrIt).name==attributeName)
		{
			++countFound;
			foundIt=objAttrIt;
		}
		
	}
	ObjectAttribute returnAttribute;
	if(countFound==1)
		returnAttribute=(*foundIt);
	else
		returnAttribute.name=QString::null;
	return returnAttribute;
}


void PageItem::setObjectAttributes(ObjAttrVector* map)
{
	pageItemAttributes=*map;
}

bool PageItem::LoremIpsum()
{
	if (itemText.count() != 0)
	{
		QString text = tr("Do you really want to replace all your text\nin the frame named %1 with sample text?");
		int t = QMessageBox::warning(ScApp, tr("Warning"),
								QString(text).arg(AnName),
								QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
		if (t == QMessageBox::No)
			return false;
	}
	QString pfad = ScPaths::instance().sampleScriptDir();
	QString pfad2;
	pfad2 = pfad + "LoremIpsum.txt";
	Serializer *ss = new Serializer(pfad2);
	if (ss!=NULL)
	{
		if (ss->Read(""))
		{
			int st = Doc->currentParaStyle;
			if (st > 5)
				ss->GetText(this, st, Doc->docParagraphStyles[st].Font, Doc->docParagraphStyles[st].FontSize, true);
			else
				ss->GetText(this, st, IFont, ISize, true);
		}
		delete ss;
	}
	if (Doc->docHyphenator->AutoCheck)
		Doc->docHyphenator->slotHyphenate(this);
	return true;
}

bool PageItem::nameExists(const QString itemName) const
{
	bool found = false;
	for (uint c = 0; c < Doc->Items.count(); ++c)
	{
		if (itemName == Doc->Items.at(c)->itemName())
		{
			found = true;
			break;
		}
	}
	return found;
}

QString PageItem::generateUniqueCopyName(const QString originalName) const
{
	if (!nameExists(originalName))
		return originalName;

	// Start embellishing the name until we get an acceptable unique name
	// first we prefix `Copy of' if it's not already there
	QString newname(originalName);
	if (!originalName.startsWith( tr("Copy of")))
		newname.prepend( tr("Copy of")+" ");

	// See if the name prefixed by "Copy of " is free
	if (nameExists(newname))
	{
		// Search the string for (number) at the end and capture
		// both the number and the text leading up to it sans brackets.
		//     Copy of fred (5)
		//     ^^^^^^^^^^^^  ^   (where ^ means captured)
		QRegExp rx("^(.*)\\s+\\((\\d+)\\)$");
		int numMatches = rx.searchRev(newname);
		// Add a (number) suffix to the end of the name. We start at the
		// old suffix's value if there was one, or at 2 if there was not.
		int suffixnum = 2;
		QString prefix(newname);
		if (numMatches != -1)
		{
			// Already had a suffix; use the name w/o suffix for prefix and
			// grab the old suffix value as a starting point.
			QStringList matches = rx.capturedTexts();
			prefix = matches[1];
			suffixnum = matches[2].toInt();
		}
		// Keep on incrementing the suffix 'till we find a free name
		do
		{
			newname = prefix + " (" + QString::number(suffixnum) + ")";
			suffixnum ++;
		}
		while (nameExists(newname));
	}
	assert(!nameExists(newname));
	return newname;
}

bool PageItem::printable() const
{
	return isPrintable;
}
 
void PageItem::setPrintable(bool toPrint)
{
	isPrintable=toPrint;
}
