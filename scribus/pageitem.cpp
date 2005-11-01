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
#include <qregexp.h>
#include <qmessagebox.h>
#include <cmath>
#include <cassert>
#include "scpaths.h"
#include "page.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "undomanager.h"
#include "undostate.h"
#include "mpalette.h"

#include "scconfig.h"

#include <ft2build.h>
#include FT_GLYPH_H

#include "scfontmetrics.h"
#include "util.h"

using namespace std;

PageItem::PageItem(const PageItem & other)
	: QObject(other.parent()),

// 200 attributes! That is madness, or to quote some famous people from Kriquet:
// "THAT ALL HAS TO GO!"
	Reverse(other.Reverse),
 	Xpos(other.Xpos),
	oldXpos(other.oldXpos),
	Ypos(other.Ypos),
	oldYpos(other.oldYpos),
	Width(other.Width),
	oldWidth(other.oldWidth),
	Height(other.Height),
	oldHeight(other.oldHeight),
	gXpos(other.gXpos),
	gYpos(other.gYpos),
	gWidth(other.gWidth),
	gHeight(other.gHeight),
 	RadRect(other.RadRect),
 	Rot(other.Rot),
	oldRot(other.oldRot),
 	Doc(other.Doc),
	GrType(other.GrType),
	GrStartX(other.GrStartX),
	GrStartY(other.GrStartY),
	GrEndX(other.GrEndX),
	GrEndY(other.GrEndY),
	TxtStroke(other.TxtStroke),
	TxtFill(other.TxtFill),
	ShTxtStroke(other.ShTxtStroke),
	ShTxtFill(other.ShTxtFill),
	TxtScale(other.TxtScale),
	TxtScaleV(other.TxtScaleV),
	TxTStyle(other.TxTStyle),
	TxtBase(other.TxtBase),
	TxtShadowX(other.TxtShadowX),
	TxtShadowY(other.TxtShadowY),
	TxtOutline(other.TxtOutline),
	TxtUnderPos(other.TxtUnderPos),
	TxtUnderWidth(other.TxtUnderWidth),
	TxtStrikePos(other.TxtStrikePos),
	TxtStrikeWidth(other.TxtStrikeWidth),
	Cols(other.Cols),
	ColGap(other.ColGap),
  	Pwidth(other.Pwidth),
	OldPwidth(other.OldPwidth),
 	PLineArt(other.PLineArt),
	PLineEnd(other.PLineEnd),
	PLineJoin(other.PLineJoin),
	NamedLStyle(other.NamedLStyle),
	Select(other.Select),
	Clip(other.Clip),
	PoLine(other.PoLine),
	ContourLine(other.ContourLine),
	imageClip(other.imageClip),
	Segments(other.Segments),
	effectsInUse(other.effectsInUse),
	PoShow(other.PoShow),
	BaseOffs(other.BaseOffs),
	ClipEdited(other.ClipEdited),
	FrameType(other.FrameType),
	ItemNr(other.ItemNr),
	Frame(other.Frame),
	OwnPage(other.OwnPage),
	savedOwnPage(other.savedOwnPage),
	oldOwnPage(other.oldOwnPage),
 	pixm(other.pixm),
  	Pfile(other.Pfile),
	Pfile2(other.Pfile2),
	Pfile3(other.Pfile3),
	IProfile(other.IProfile),
	UseEmbedded(other.UseEmbedded),
	EmProfile(other.EmProfile),
	IRender(other.IRender),
  	PicArt(other.PicArt),
   	PicAvail(other.PicAvail),
   	LocalScX(other.LocalScX),
   	LocalScY(other.LocalScY),
   	LocalX(other.LocalX),
   	LocalY(other.LocalY),
	OrigW(other.OrigW),
	OrigH(other.OrigH),
   	BBoxX(other.BBoxX),
 	BBoxH(other.BBoxH),
 	Extra(other.Extra),
	TExtra(other.TExtra),
	BExtra(other.BExtra),
	RExtra(other.RExtra),
  	LineSp(other.LineSp),
	LineSpMode(other.LineSpMode),
	CurX(other.CurX),
	CurY(other.CurY),
	CPos(other.CPos),
	itemText(other.itemText),
  	isBookmark(other.isBookmark),
	BMnr(other.BMnr),
   	isAnnotation(other.isAnnotation),
	AnType(other.AnType),
	AnActType(other.AnActType),
	AnAction(other.AnAction),
	An_E_act(other.An_E_act),
	An_X_act(other.An_X_act),
	An_D_act(other.An_D_act),
	An_Fo_act(other.An_Fo_act),
	An_Bl_act(other.An_Bl_act),
	An_K_act(other.An_K_act),
	An_F_act(other.An_F_act),
	An_V_act(other.An_V_act),
	An_C_act(other.An_C_act),
	AutoName(other.AutoName),
	AnToolTip(other.AnToolTip),
	AnRollOver(other.AnRollOver),
	AnDown(other.AnDown),
	AnBColor(other.AnBColor),
	An_Extern(other.An_Extern),
	AnBsty(other.AnBsty),
	AnBwid(other.AnBwid),
	AnFeed(other.AnFeed),
	AnZiel(other.AnZiel),
	AnFlag(other.AnFlag),
	AnMaxChar(other.AnVis),
	AnVis(other.AnVis),
	AnChkStil(other.AnChkStil),
	AnFont(other.AnFont),
	AnIsChk(other.AnIsChk),
	AnAAact(other.AnAAact),
	AnHTML(other.AnHTML),
	AnUseIcons(other.AnUseIcons),
	AnIPlace(other.AnIPlace),
	AnScaleW(other.AnScaleW),
	AnFormat(other.AnFormat),
 	IFont(other.IFont),
  	ISize(other.ISize),
 	HasSel(other.HasSel),
 	FrameOnly(other.FrameOnly),
	BackBox(other.BackBox),
	NextBox(other.NextBox),
	NextIt(other.NextIt),
	NextPg(other.NextPg),
	Tinput(other.Tinput),
	isAutoText(other.isAutoText),
	textAlignment(other.textAlignment),
	MaxChars(other.MaxChars),
	Redrawn(other.Redrawn),
	ExtraV(other.ExtraV),
	isRaster(other.isRaster),
	OldB(other.OldB),
	OldH(other.OldH),
	OldB2(other.OldB2),
	OldH2(other.OldH2),
	Sizing(other.Sizing),
	toPixmap(other.toPixmap),
	LayerNr(other.LayerNr),
	ScaleType(other.ScaleType),
	AspectRatio(other.AspectRatio),
	Groups(other.Groups),
	DashValues(other.DashValues),
	TabValues(other.TabValues),
	DashOffset(other.DashOffset),
	fill_gradient(other.fill_gradient),
	fillRule(other.fillRule),
	Language(other.Language),
	LeftLink(other.LeftLink),
	RightLink(other.RightLink),
	TopLink(other.TopLink),
	BottomLink(other.BottomLink),
	LeftLinkID(other.LeftLinkID),
	RightLinkID(other.RightLinkID),
	TopLinkID(other.TopLinkID),
	BottomLinkID(other.BottomLinkID),
	LeftLine(other.LeftLine),
	RightLine(other.RightLine),
	TopLine(other.TopLine),
	BottomLine(other.BottomLine),
	isTableItem(other.isTableItem),
	isSingleSel(other.isSingleSel),
	BoundingX(other.BoundingX),
	BoundingY(other.BoundingY),
	BoundingW(other.BoundingW),
	BoundingH(other.BoundingH),
	ChangedMasterItem(other.ChangedMasterItem),
	OnMasterPage(other.OnMasterPage),
	startArrowIndex(other.startArrowIndex),
	endArrowIndex(other.endArrowIndex),
	isEmbedded(other.isEmbedded),
	fillQColor(other.fillQColor),
	strokeQColor(other.strokeQColor),
	undoManager(other.undoManager),

	// protected
	
	AnName(other.AnName),
	fillColorVal(other.fillColorVal),
	lineColorVal(other.lineColorVal),
	lineShadeVal(other.lineShadeVal),
	fillShadeVal(other.fillShadeVal),
	fillTransparencyVal(other.fillTransparencyVal),
	lineTransparencyVal(other.fillTransparencyVal),
	imageIsFlippedH(other.imageIsFlippedH),
	imageIsFlippedV(other.imageIsFlippedV),
	Locked(other.Locked),
	LockRes(other.LockRes),
	textFlowsAroundFrameVal(other.textFlowsAroundFrameVal),
	textFlowUsesBoundingBoxVal(other.textFlowUsesBoundingBoxVal),
	textFlowUsesContourLineVal(other.textFlowUsesContourLineVal),
	pageItemAttributes(other.pageItemAttributes),
	isPrintable(other.isPrintable),
	tagged(other.tagged)
{
}


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
	TxtUnderWidth = Doc->typographicSettings.valueUnderlineWidth;
	TxtUnderPos = Doc->typographicSettings.valueUnderlinePos;
	TxtStrikePos = Doc->typographicSettings.valueStrikeThruPos;
	TxtStrikeWidth = Doc->typographicSettings.valueStrikeThruWidth;
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
	LineSp = ((Doc->toolSettings.defSize / 10.0) * static_cast<double>(Doc->typographicSettings.autoLineSpacing) / 100) + (Doc->toolSettings.defSize / 10.0);
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
	OwnPage = Doc->currentPage->pageNr();
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
	LayerNr = Doc->activeLayer();
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
		case TextFrame:
			DrawObj_Item(p, e, sc);
			break;
		case ImageFrame:
		case PathText:
			DrawObj_Item(p, sc);
			break;
		default:
			DrawObj_Item(p);
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
//		p->rotate(Rot);
	}
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
		if (fillColor() != "None")
		{
			p->setBrush(fillQColor);
			p->setFillMode(ScPainter::Solid);
		}
		else
			p->setFillMode(ScPainter::None);
	}
	if (lineColor() != "None")
	{
		if ((Pwidth == 0) && ! asLine())
			p->setLineWidth(0);
		else
		{
			p->setPen(strokeQColor, Pwidth, PLineArt, PLineEnd, PLineJoin);
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
	if (itemType()==PathText || itemType()==PolyLine || itemType()==Line)
		doStroke=false;
	if ((doStroke) && (!Doc->RePos))
	{
		if (lineColor() != "None")
		{
			p->setPen(strokeQColor, Pwidth, PLineArt, PLineEnd, PLineJoin);
			if (DashValues.count() != 0)
				p->setDash(DashValues, DashOffset);
		}
		else
			p->setLineWidth(0);
		if (!isTableItem)
		{
			p->setupPolygon(&PoLine);
			if (NamedLStyle.isEmpty())
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
	QPtrList<PageItem> emG;
	emG.clear();
	if (hl->embedded != 0)
	{
		if (!Doc->DoDrawing)
		{
			hl->embedded->Redrawn = true;
			hl->embedded->Tinput = false;
			hl->embedded->FrameOnly = false;
			return;
		}
		emG.append(hl->embedded);
		if (hl->embedded->Groups.count() != 0)
		{
			for (uint ga=0; ga<Doc->FrameItems.count(); ++ga)
			{
				if (Doc->FrameItems.at(ga)->Groups.count() != 0)
				{
					if (Doc->FrameItems.at(ga)->Groups.top() == hl->embedded->Groups.top())
					{
						if (Doc->FrameItems.at(ga)->ItemNr != hl->embedded->ItemNr)
						{
							if (emG.find(Doc->FrameItems.at(ga)) == -1)
								emG.append(Doc->FrameItems.at(ga));
						}
					}
				}
			}
		}
		for (uint em = 0; em < emG.count(); ++em)
		{
			PageItem* embedded = emG.at(em);
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
			embedded->Xpos = Xpos + hl->xco + embedded->gXpos;
			embedded->Ypos = Ypos + (hl->yco - (embedded->gHeight * (hl->scalev / 1000.0))) + embedded->gYpos;
			p->translate((hl->xco + embedded->gXpos * (hl->scale / 1000.0)) * p->zoomFactor(), (hl->yco - (embedded->gHeight * (hl->scalev / 1000.0)) + embedded->gYpos * (hl->scalev / 1000.0)) * p->zoomFactor());
			if (hl->base != 0)
			{
				p->translate(0, -embedded->gHeight * (hl->base / 1000.0) * p->zoomFactor());
				embedded->Ypos -= embedded->gHeight * (hl->base / 1000.0);
			}
			p->scale(hl->scale / 1000.0, hl->scalev / 1000.0);
			embedded->Dirty = Dirty;
			double sc;
			double pws = embedded->Pwidth;
			embedded->DrawObj_Pre(p, sc);
			switch(embedded->itemType())
			{
				case ImageFrame:
					//embedded->DrawObj_ImageFrame(p, sc);
					embedded->DrawObj_Item(p, sc);
					break;
				case TextFrame:
					//embedded->DrawObj_TextFrame(p, e, sc);
					embedded->DrawObj_Item(p, e, sc);
					break;
				case Line:
					embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
					//embedded->DrawObj_Line(p);
					embedded->DrawObj_Item(p);
					break;
				case Polygon:
					//embedded->DrawObj_Polygon(p);
					embedded->DrawObj_Item(p);
					break;
				case PolyLine:
					embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
					//embedded->DrawObj_PolyLine(p);
					embedded->DrawObj_Item(p);
					break;
				case PathText:
					//embedded->DrawObj_PathText(p, sc);
					embedded->DrawObj_Item(p, sc);
					break;
				default:
					break;
			}
			embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
			embedded->DrawObj_Post(p);
			p->restore();
			embedded->Pwidth = pws;
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
	QPoint in  = QPoint(qRound((Xpos-Doc->minCanvasCoordinate.x())*sc), qRound((Ypos-Doc->minCanvasCoordinate.y())*sc));
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
						if (! asLine())
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
	QString chx("#");
	if ((!Doc->masterPageMode()) && (OwnPage != -1))
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
		QString out("%1");
		QString out2;
		//CB Section numbering
		//out2 = out.arg(OwnPage+Doc->FirstPnum, -zae);
		out2=out.arg(Doc->getSectionPageNumberForPageIndex(OwnPage), -zae);
		//out2=out.arg(out2, -zae);
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
			retval -= asce * Doc->typographicSettings.valueSuperScript / 100;
			*chs = QMAX(static_cast<int>(hl->csize * Doc->typographicSettings.scalingSuperScript / 100), 1);
		}
		if (chst & 2)
		{
			retval += asce * Doc->typographicSettings.valueSubScript / 100;
			*chs = QMAX(static_cast<int>(hl->csize * Doc->typographicSettings.scalingSubScript / 100), 1);
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
				*chs = QMAX(static_cast<int>(hl->csize * Doc->typographicSettings.valueSmallCaps / 100), 1);
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
#ifdef HAVE_CAIRO
					p->setupTextPolygon(&gly);
#endif
					p->fillPath();
					p->setBrush(tmp);
					p->restore();
#ifdef HAVE_CAIRO
					p->setupTextPolygon(&gly);
#endif
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
		QValueList<uint>::Iterator it2end=Segments.end();
		uint FirstVal = 0;
		for (QValueList<uint>::Iterator it2 = Segments.begin(); it2 != it2end; ++it2)
		{
			if (NamedLStyle.isEmpty())
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
		if (NamedLStyle.isEmpty())
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
		if (NamedLStyle.isEmpty())
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
		QString oldStyle = NamedLStyle.isEmpty() ? Um::NoStyle : NamedLStyle;
		QString nStyle   = newStyle.isEmpty() ? Um::NoStyle : newStyle;
		QString action   = newStyle.isEmpty() ? Um::NoLineStyle : Um::CustomLineStyle;
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
	AdjustPictScale();
	//FIXME emit here instead
	ScApp->propertiesPalette->setLvalue(LocalScX, LocalScY, LocalX, LocalY);
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
	return textFlowsAroundFrameVal;
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
	return textFlowUsesBoundingBoxVal;
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
	return textFlowUsesContourLineVal;
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
	/*
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ConvertTo + " " + toType,
										  QString(Um::FromTo).arg(fromType).arg(toType));
		ss->set("CONVERT", "convert");
		ss->set("PAGEITEM", reinterpret_cast<int>(this));
		ss->set("OLD_TYPE", itemTypeVal);
		ss->set("NEW_TYPE", newType);
		undoManager->action(this, ss);
	}
	*/
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
	if (force || ((oldWidth != Width || oldHeight != Height) && shouldCheck()))
		resizeUndoAction();
	// has the item been rotated
	if (force || ((oldRot != Rot) && (shouldCheck())))
		rotateUndoAction();
	// has the item been moved
	if (force || ((oldXpos != Xpos || oldYpos != Ypos) && shouldCheck()))
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
		mx = -mx;
		my = -my;
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
		mx = -mx;
		my = -my;
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
		mx = -mx;
		my = -my;
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
	QString name = state->get("OLD_NAME");
	if (!isUndo)
		name = state->get("NEW_NAME");
	setItemName(name);
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


// This must go into class ScribusDoc! 
// For now we'll just make it independent of 'this' -- AV
void PageItem::restoreType(SimpleState *state, bool isUndo)
{
	// well, probably not the best way to handle pointers...
	PageItem * item = reinterpret_cast<PageItem *>(state->getInt("PAGEITEM"));
	int type = state->getInt("OLD_TYPE");
	if (!isUndo)
		type = state->getInt("NEW_TYPE");
	ScApp->view->Deselect(false);
	ScApp->view->SelectItem(item, false);
	switch (type) {
		case ImageFrame: ScApp->view->ToPicFrame(); break;
		case TextFrame: ScApp->view->ToTextFrame(); break;
		case Polygon: ScApp->view->ToPolyFrame(); break;
		case PolyLine: ScApp->view->ToBezierFrame(); break;
	}
	ScApp->setAppMode(modeNormal);
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
	bool type=ScaleType;
	if (state->contains("SCALE_TYPE"))
	{
		if (isUndo)
			type = !state->getBool("SCALE_TYPE");
		else
			type = state->getBool("SCALE_TYPE");
	}
	
	bool ratio=AspectRatio;
	if (state->contains("ASPECT_RATIO"))
	{
		if (isUndo)
			ratio = !state->getBool("ASPECT_RATIO");
		else
			ratio = state->getBool("ASPECT_RATIO");
	}

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
	}
	ScApp->view->TransformPoly(mode, rot, scaling);
	ScApp->view->EditContour = editContour;
}

void PageItem::restoreContourLine(SimpleState *state, bool isUndo)
{
	ItemState<FPointArray> *is = dynamic_cast<ItemState<FPointArray>*>(state);
	if (is)
	{
		if (isUndo)
			ContourLine = is->getItem();
		else
			ContourLine = PoLine.copy();
		ClipEdited = true;
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
	if (fn.isEmpty())
	{
		select();
		ScApp->view->ClearItem();
	}
	else
		loadImage(fn, false);
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

QString PageItem::generateUniqueCopyName(const QString originalName) const
{
	if (!Doc->itemNameExists(originalName))
		return originalName;

	// Start embellishing the name until we get an acceptable unique name
	// first we prefix `Copy of' if it's not already there
	QString newname(originalName);
	if (!originalName.startsWith( tr("Copy of")))
		newname.prepend( tr("Copy of")+" ");

	// See if the name prefixed by "Copy of " is free
	if (Doc->itemNameExists(newname))
	{
		// Search the string for (number) at the end and capture
		// both the number and the text leading up to it sans brackets.
		//     Copy of fred (5)
		//     ^^^^^^^^^^^^  ^   (where ^ means captured)
		static QRegExp rx("^(.*)\\s+\\((\\d+)\\)$");
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
		while (Doc->itemNameExists(newname));
	}
	assert(!Doc->itemNameExists(newname));
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

bool PageItem::isTagged() const
{
	return tagged;
}

void PageItem::setTagged(bool tag)
{
	tagged=tag;
}

void PageItem::copyToCopyPasteBuffer(struct CopyPasteBuffer *Buffer)
{
	Buffer->PType = itemType();
	Buffer->Xpos = Xpos;
	Buffer->Ypos = Ypos;
	Buffer->Width = Width;
	Buffer->Height = Height;
	Buffer->RadRect = RadRect;
	Buffer->FrameType = FrameType;
	Buffer->ClipEdited = ClipEdited;
	Buffer->Pwidth = Pwidth;
	Buffer->Pcolor = fillColor();
	Buffer->Pcolor2 = lineColor();
	Buffer->Shade = fillShade();
	Buffer->Shade2 = lineShade();
	Buffer->GrColor = "";
	Buffer->GrColor2 = "";
	Buffer->GrShade = 100;
	Buffer->GrShade2 = 100;
	Buffer->fill_gradient = fill_gradient;
	Buffer->GrType = GrType;
	Buffer->GrStartX = GrStartX;
	Buffer->GrStartY = GrStartY;
	Buffer->GrEndX = GrEndX;
	Buffer->GrEndY = GrEndY;
	Buffer->TxtStroke = TxtStroke;
	Buffer->TxtFill = TxtFill;
	Buffer->ShTxtStroke = ShTxtStroke;
	Buffer->ShTxtFill = ShTxtFill;
	Buffer->TxtScale = TxtScale;
	Buffer->TxtScaleV = TxtScaleV;
	Buffer->TxTBase = TxtBase;
	Buffer->TxTStyle = TxTStyle;
	Buffer->TxtShadowX = TxtShadowX;
	Buffer->TxtShadowY = TxtShadowY;
	Buffer->TxtOutline = TxtOutline;
	Buffer->TxtUnderPos = TxtUnderPos;
	Buffer->TxtUnderWidth = TxtUnderWidth;
	Buffer->TxtStrikePos = TxtStrikePos;
	Buffer->TxtStrikeWidth = TxtStrikeWidth;
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
	Buffer->flippedH = imageFlippedH();
	Buffer->flippedV = imageFlippedV();
	Buffer->BBoxX = BBoxX;
	Buffer->BBoxH = BBoxH;
	Buffer->isPrintable = printable();
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
	Buffer->AnName = itemName();
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
	uint itemTextCount=itemText.count();
	if (itemTextCount != 0)
	{
		for (uint a=0; a<itemTextCount; ++a)
		{
			if( (itemText.at(a)->ch == "\n") || (itemText.at(a)->ch == "\r"))
				Text += QString(QChar(5))+"\t";
			else if(itemText.at(a)->ch == "\t")
				Text += QString(QChar(4))+"\t";
			else
				Text += itemText.at(a)->ch+"\t";
			Text += itemText.at(a)->cfont->scName()+"\t";
			Text += QString::number(itemText.at(a)->csize / 10.0)+"\t";
			Text += itemText.at(a)->ccolor+"\t";
			Text += QString::number(itemText.at(a)->cextra)+"\t";
			Text += QString::number(itemText.at(a)->cshade)+'\t';
			Text += QString::number(itemText.at(a)->cstyle)+'\t';
			Text += QString::number(itemText.at(a)->cab)+'\t';
			Text += itemText.at(a)->cstroke+"\t";
			Text += QString::number(itemText.at(a)->cshade2)+'\t';
			Text += QString::number(itemText.at(a)->cscale)+'\t';
			Text += QString::number(itemText.at(a)->cscalev)+'\t';
			Text += QString::number(itemText.at(a)->cbase)+'\t';
			Text += QString::number(itemText.at(a)->cshadowx)+'\t';
			Text += QString::number(itemText.at(a)->cshadowy)+'\t';
			Text += QString::number(itemText.at(a)->coutline)+'\t';
			Text += QString::number(itemText.at(a)->cunderpos)+'\t';
			Text += QString::number(itemText.at(a)->cunderwidth)+'\t';
			Text += QString::number(itemText.at(a)->cstrikepos)+'\t';
			Text += QString::number(itemText.at(a)->cstrikewidth)+'\n';
		}
	}
	Buffer->itemText = Text;
	Buffer->Clip = Clip.copy();
	Buffer->PoLine = PoLine.copy();
	Buffer->ContourLine = ContourLine.copy();
	Buffer->UseContour = textFlowUsesContourLine();
	Buffer->TabValues = TabValues;
	Buffer->DashValues = DashValues;
	Buffer->DashOffset = DashOffset;
	Buffer->PoShow = PoShow;
	Buffer->BaseOffs = BaseOffs;
	Buffer->Textflow = textFlowsAroundFrame();
	Buffer->Textflow2 = textFlowUsesBoundingBox();
	Buffer->textAlignment = textAlignment;
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
	Buffer->Locked = locked();
	Buffer->LockRes = sizeLocked();
	Buffer->Transparency = fillTransparency();
	Buffer->TranspStroke = lineTransparency();
	Buffer->Reverse = Reverse;
	Buffer->NamedLStyle = NamedLStyle;
	Buffer->Language = Language;
	Buffer->Cols = Cols;
	Buffer->ColGap = ColGap;
	Buffer->isTableItem = isTableItem;
	Buffer->TopLine = TopLine;
	Buffer->LeftLine = LeftLine;
	Buffer->RightLine = RightLine;
	Buffer->BottomLine = BottomLine;
	if (isTableItem)
	{
		if (TopLink != 0)
			Buffer->TopLinkID = TopLink->ItemNr;
		else
			Buffer->TopLinkID = -1;
		if (LeftLink != 0)
			Buffer->LeftLinkID = LeftLink->ItemNr;
		else
			Buffer->LeftLinkID = -1;
		if (RightLink != 0)
			Buffer->RightLinkID = RightLink->ItemNr;
		else
			Buffer->RightLinkID = -1;
		if (BottomLink != 0)
			Buffer->BottomLinkID = BottomLink->ItemNr;
		else
			Buffer->BottomLinkID = -1;
	}
	Buffer->startArrowIndex = startArrowIndex;
	Buffer->endArrowIndex = endArrowIndex;
}


//Moved from View
void PageItem::SetFrameShape(int count, double *vals)
{
	PoLine.resize(0);
	for (int a = 0; a < count-3; a += 4)
	{
		if (vals[a] < 0)
		{
			PoLine.setMarker();
			continue;
		}
		double x1 = Width * vals[a] / 100.0;
		double y1 = Height * vals[a+1] / 100.0;
		double x2 = Width * vals[a+2] / 100.0;
		double y2 = Height * vals[a+3] / 100.0;
		PoLine.addPoint(x1, y1);
		PoLine.addPoint(x2, y2);
	}
	Clip = FlattenPath(PoLine, Segments);
	ClipEdited = true;
}

void PageItem::SetRectFrame()
{
	static double rect[] = {  0.0,   0.0,   0.0,   0.0,
							100.0,   0.0, 100.0,   0.0,
							100.0,   0.0, 100.0,   0.0,
							100.0, 100.0, 100.0, 100.0,
							100.0, 100.0, 100.0, 100.0,
							  0.0, 100.0,   0.0, 100.0,
							  0.0, 100.0,   0.0, 100.0,
							  0.0,   0.0,   0.0,   0.0};
	SetFrameShape(32, rect);
	ClipEdited = false;
	FrameType = 0;
}

void PageItem::SetOvalFrame()
{
	static double rect[] = {   100.0,  50.0, 100.0,       77.615235,
								50.0, 100.0,  77.615235, 100.0,
								50.0, 100.0,  22.385765, 100.0,
								 0.0,  50.0,   0.0,       77.615235,
								 0.0,  50.0,   0.0,       22.385765,
								50.0,   0.0,  22.385765,   0.0,
								50.0,   0.0,  77.615235,   0.0,
							   100.0,  50.0, 100.0,       22.385765};
	SetFrameShape(32, rect);
	FrameType = 1;
	ClipEdited = false;
}

void PageItem::SetFrameRound()
{
	RadRect = QMIN(RadRect, QMIN(Width,Height)/2);
	PoLine.resize(0);
	double rr = fabs(RadRect);
	double bezierFactor=0.552284749; //Bezier Control Point Factor: 8/3*(sin(45)-0.5)
	double rrxBezierFactor=rr*bezierFactor;
	double Width_rr=Width-rr;
	double Height_rr=Height-rr;
	if (RadRect > 0)
	{
		PoLine.addQuadPoint(rr, 0, rr, 0, Width_rr, 0, Width_rr, 0);
		PoLine.addQuadPoint(Width_rr, 0, Width_rr+rrxBezierFactor, 0, Width, rr, Width, rrxBezierFactor);
		PoLine.addQuadPoint(Width, rr, Width, rr, Width, Height_rr, Width, Height_rr);
		PoLine.addQuadPoint(Width, Height_rr, Width, Height_rr+rrxBezierFactor, Width_rr, Height, Width_rr+rrxBezierFactor, Height);
		PoLine.addQuadPoint(Width_rr, Height, Width_rr, Height, rr, Height, rr, Height);
		PoLine.addQuadPoint(rr, Height, rrxBezierFactor, Height, 0, Height_rr, 0, Height_rr+rrxBezierFactor);
		PoLine.addQuadPoint(0, Height_rr, 0, Height_rr, 0, rr, 0, rr);
		PoLine.addQuadPoint(0, rr, 0, rrxBezierFactor, rr, 0, rr*bezierFactor, 0);
	}
	else
	{
		PoLine.addQuadPoint(rr, 0, rr, 0, Width_rr, 0, Width_rr, 0);
		PoLine.addQuadPoint(Width_rr, 0, Width_rr, rrxBezierFactor, Width, rr, Width-rrxBezierFactor, rr);
		PoLine.addQuadPoint(Width, rr, Width, rr, Width, Height_rr, Width, Height_rr);
		PoLine.addQuadPoint(Width, Height_rr, Width-rrxBezierFactor, Height_rr, Width_rr, Height, Width_rr, Height-rrxBezierFactor);
		PoLine.addQuadPoint(Width_rr, Height, Width_rr, Height, rr, Height, rr, Height);
		PoLine.addQuadPoint(rr, Height, rr, Height-rrxBezierFactor, 0, Height_rr, rrxBezierFactor, Height_rr);
		PoLine.addQuadPoint(0, Height_rr, 0, Height_rr, 0, rr, 0, rr);
		PoLine.addQuadPoint(0, rr, rrxBezierFactor, rr, rr, 0, rr, rr*bezierFactor);
	}
	Clip = FlattenPath(PoLine, Segments);
	ClipEdited = false;
	FrameType = 2;
}

void PageItem::getBoundingRect(double *x1, double *y1, double *x2, double *y2)
{
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	if (Rot != 0)
	{
		FPointArray pb;
		pb.resize(0);
		pb.addPoint(FPoint(Xpos, Ypos));
		pb.addPoint(FPoint(Width,    0.0, Xpos, Ypos, Rot, 1.0, 1.0));
		pb.addPoint(FPoint(Width, Height, Xpos, Ypos, Rot, 1.0, 1.0));
		pb.addPoint(FPoint(  0.0, Height, Xpos, Ypos, Rot, 1.0, 1.0));
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
		*x1 = Xpos;
		*y1 = Ypos;
		*x2 = Xpos + Width;
		*y2 = Ypos + Height;
	}
}

bool PageItem::loadImage(const QString& filename, const bool reload, const int gsResolution)
{
	if (! asImageFrame())
		return false;
	QFileInfo fi(filename);
	QString clPath(pixm.imgInfo.usedPath);
	pixm.imgInfo.valid = false;
	pixm.imgInfo.clipPath="";
	pixm.imgInfo.PDSpathData.clear();
	pixm.imgInfo.layerInfo.clear();
	pixm.imgInfo.usedPath="";
	imageClip.resize(0);
	int gsRes=gsResolution;
	if (gsResolution==-1) //If it wasn't supplied, get it from PrefsManager.
		gsRes=PrefsManager::instance()->gsResolution();
	bool dummy;
	if (!pixm.LoadPicture(filename, IProfile, IRender, UseEmbedded, true, 2, gsRes, &dummy))
	{
		Pfile = fi.absFilePath();
		PicAvail = false;
		PicArt = false;
		return false;
	}
	else
	{
		if (UndoManager::undoEnabled() && !reload)
		{
			SimpleState *ss = new SimpleState(Um::GetImage, filename, Um::IGetImage);
			ss->set("GET_IMAGE", "get_image");
			ss->set("OLD_IMAGE_PATH", Pfile);
			ss->set("NEW_IMAGE_PATH", filename);
			undoManager->action(this, ss);
		}
		double xres = pixm.imgInfo.xres;
		double yres = pixm.imgInfo.yres;
		PicAvail = true;
		PicArt = true;
		BBoxX = 0;
		if (Pfile != filename)
		{
			LocalScX = 72.0 / xres;
			LocalScY = 72.0 / yres;
			LocalX = 0;
			LocalY = 0;
			if ((Doc->toolSettings.useEmbeddedPath) && (!pixm.imgInfo.clipPath.isEmpty()))
			{
				pixm.imgInfo.usedPath = pixm.imgInfo.clipPath;
				clPath = pixm.imgInfo.clipPath;
				if (pixm.imgInfo.PDSpathData.contains(clPath))
				{
					imageClip = pixm.imgInfo.PDSpathData[clPath].copy();
					pixm.imgInfo.usedPath = clPath;
					QWMatrix cl;
					cl.translate(LocalX*LocalScX, LocalY*LocalScY);
					cl.scale(LocalScX, LocalScY);
					imageClip.map(cl);
				}
			}
		}
		Pfile = fi.absFilePath();
		if (reload && pixm.imgInfo.PDSpathData.contains(clPath))
		{
			imageClip = pixm.imgInfo.PDSpathData[clPath].copy();
			pixm.imgInfo.usedPath = clPath;
			QWMatrix cl;
			cl.translate(LocalX*LocalScX, LocalY*LocalScY);
			cl.scale(LocalScX, LocalScY);
			imageClip.map(cl);
		}
		BBoxH = pixm.height();
		OrigW = pixm.width();
		OrigH = pixm.height();
		QString ext = fi.extension(false).lower();
		isRaster = !(ext == "pdf" || ext == "ps" || ext == "eps");
		UseEmbedded=pixm.imgInfo.isEmbedded;
		if (pixm.imgInfo.isEmbedded)
		{
			IProfile = "Embedded " + pixm.imgInfo.profileName;
			EmProfile = "Embedded " + pixm.imgInfo.profileName;
		}
		else
			IProfile = pixm.imgInfo.profileName;
	}
	if (PicAvail)
	{
		if (pixm.imgInfo.lowResType != 0)
		{
			double scaling = pixm.imgInfo.xres / 36.0;
			if (pixm.imgInfo.lowResType == 1)
				scaling = pixm.imgInfo.xres / 72.0;
			pixm.createLowRes(scaling);
			pixm.imgInfo.lowResScale = scaling;
		}
		pixm.applyEffect(effectsInUse, Doc->PageColors, false);
	}
	return true;
}


void PageItem::DrawObj_Item(ScPainter *p)
{
}

void PageItem::DrawObj_Item(ScPainter *p, double sc)
{
}

void PageItem::DrawObj_Item(ScPainter *p, QRect e, double sc)
{
}


void PageItem::AdjustPictScale()
{
	if (itemType()!=PageItem::ImageFrame)
		return;
	if (ScaleType)
		return;
	if (OrigW == 0 || OrigH == 0)
		return;
	LocalX = 0;
	LocalY = 0;
	double xs = Width / static_cast<double>(OrigW);
	double ys = Height / static_cast<double>(OrigH);
	if (AspectRatio)
	{
		LocalScX = QMIN(xs, ys);
		LocalScY = QMIN(xs, ys);
	}
	else
	{
		LocalScX = xs;
		LocalScY = ys;
	}
	if (imageClip.size() != 0)
	{
		imageClip = pixm.imgInfo.PDSpathData[pixm.imgInfo.usedPath].copy();
		QWMatrix cl;
		cl.translate(LocalX*LocalScX, LocalY*LocalScY);
		cl.scale(LocalScX, LocalScY);
		imageClip.map(cl);
	}
	//FIXME Make this emit here, currently pasted after calls to this function in the view
	//emit SetLocalValues(LocalScX, LocalScY, LocalX, LocalY );
}

QRect PageItem::getRedrawBounding(const double viewScale)
{
	int x = qRound(floor(BoundingX - OldPwidth / 2.0 - 5) * viewScale);
	int y = qRound(floor(BoundingY - OldPwidth / 2.0 - 5) * viewScale);
	int w = qRound(ceil(BoundingW + OldPwidth + 10) * viewScale);
	int h = qRound(ceil(BoundingH + OldPwidth + 10) * viewScale);
	QRect ret = QRect(x, y, w, h);
	ret.moveBy(qRound(-Doc->minCanvasCoordinate.x() * viewScale), qRound(-Doc->minCanvasCoordinate.y() * viewScale));
	return ret;
}


void PageItem::setRedrawBounding()
{
	double bw, bh;
	getBoundingRect(&BoundingX, &BoundingY, &bw, &bh);
	BoundingW = bw - BoundingX;
	BoundingH = bh - BoundingY;
	if (asLine())
		BoundingH = QMAX(BoundingH, 1);
}

void PageItem::updateGradientVectors()
{
	switch (GrType)
	{
		case 0:
		case 1:
			GrStartX = 0;
			GrStartY = Height / 2.0;
			GrEndX = Width;
			GrEndY = Height / 2.0;
			break;
		case 2:
			GrStartX = Width / 2.0;
			GrStartY = 0;
			GrEndX = Width / 2.0;
			GrEndY = Height;
			break;
		case 3:
			GrStartX = 0;
			GrStartY = 0;
			GrEndX = Width;
			GrEndY = Height;
			break;
		case 4:
			GrStartX = 0;
			GrStartY = Height;
			GrEndX = Width;
			GrEndY = 0;
			break;
		case 5:
			GrStartX = Width / 2.0;
			GrStartY = Height / 2.0;
			if (Width >= Height)
			{
				GrEndX = Width;
				GrEndY = Height / 2.0;
			}
			else
			{
				GrEndX = Width / 2.0;
				GrEndY = Height;
			}
			break;
		default:
			break;
	}
	GrEndX = QMIN(QMAX(GrEndX, 0), Width);
	GrEndY = QMIN(QMAX(GrEndY, 0), Height);
	GrStartX = QMIN(QMAX(GrStartX, 0), Width);
	GrStartY = QMIN(QMAX(GrStartY, 0), Height);
}

void PageItem::SetPolyClip(int up)
{
	Clip.resize(0);
	if (PoLine.size() < 4)
		return;
	double rot;
	QPoint np, np2;
	QPointArray cl, cl1, cl2;
	cl = FlattenPath(PoLine, Segments);
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
	Clip.putPoints(Clip.size(), cl1.size(), cl1);
	for (int a2 = cl2.size()-1; a2 > -1; a2--)
	{
		Clip.resize(Clip.size()+1);
		Clip.setPoint(Clip.size()-1, cl2.point(a2));
	}
}

void PageItem::UpdatePolyClip()
{
	struct ScText *hl;
	int asce = 1;
	int desc = 1;
	uint itemTextCount=itemText.count();
	for (uint a = 0; a < itemTextCount; ++a)
	{
		hl = itemText.at(a);
		int des = static_cast<int>(hl->cfont->numDescender * (-hl->csize / 10.0));
		int asc = static_cast<int>(hl->cfont->numAscent * (hl->csize / 10.0));
		if (asc > asce)
			asce = asc;
		if (des > desc)
			desc = des;
	}
	SetPolyClip(static_cast<int>(asce+BaseOffs));
}

