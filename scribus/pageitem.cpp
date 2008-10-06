/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QRegion>
#include <QPoint>
#include <QFileInfo>
#include <qdrawutil.h>
#include <QRegExp>
#include <QMessageBox>
#include <QPolygon>
#include <cmath>
#include <cassert>
#include <QDebug>

#include "canvas.h"
#include "scpaths.h"
#include "cmsettings.h"
#include "colorblind.h"
#include "commonstrings.h"
#include "cpalette.h"
#include "guidemanager.h"
#include "page.h"
#include "pageitem_latexframe.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "resourcecollection.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "scribuswin.h"
#include "selection.h"
#include "text/nlsconfig.h"
#include "undomanager.h"
#include "undostate.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"
#include "util_text.h"
#ifdef HAVE_CAIRO
	#include <cairo.h>
#endif

using namespace std;

PageItem::PageItem(const PageItem & other)
	: QObject(other.parent()),
	 UndoObject(other), SingleObservable<PageItem>(other.m_Doc->itemsChanged()),

// 200 attributes! That is madness, or to quote some famous people from Kriquet:
// "THAT ALL HAS TO GO!"
	gXpos(other.gXpos),
	gYpos(other.gYpos),
	gWidth(other.gWidth),
	gHeight(other.gHeight),
	GrType(other.GrType),
	GrStartX(other.GrStartX),
	GrStartY(other.GrStartY),
	GrEndX(other.GrEndX),
	GrEndY(other.GrEndY),
	Cols(other.Cols),
	ColGap(other.ColGap),
	PLineArt(other.PLineArt),
	PLineEnd(other.PLineEnd),
	PLineJoin(other.PLineJoin),
	NamedLStyle(other.NamedLStyle),
	Clip(other.Clip),
	PoLine(other.PoLine),
	ContourLine(other.ContourLine),
	imageClip(other.imageClip),
	Segments(other.Segments),
	effectsInUse(other.effectsInUse),
	PoShow(other.PoShow),
	BaseOffs(other.BaseOffs),
	textPathType(other.textPathType),
	textPathFlipped(other.textPathFlipped),
	ClipEdited(other.ClipEdited),
	FrameType(other.FrameType),
	ItemNr(other.ItemNr),
	Frame(other.Frame),
	OwnPage(other.OwnPage),
	oldOwnPage(other.oldOwnPage),
	pixm(other.pixm),
	Pfile(other.Pfile),
	Pfile2(other.Pfile2),
	Pfile3(other.Pfile3),
	IProfile(other.IProfile),
	UseEmbedded(other.UseEmbedded),
	EmProfile(other.EmProfile),
	IRender(other.IRender),

	PicAvail(other.PicAvail),
	OrigW(other.OrigW),
	OrigH(other.OrigH),
	BBoxX(other.BBoxX),
	BBoxH(other.BBoxH),
	CurX(other.CurX),
	CurY(other.CurY),
	CPos(other.CPos),
	itemText(other.itemText),
	isBookmark(other.isBookmark),
	HasSel(other.HasSel),
//	Tinput(other.Tinput),
	isAutoText(other.isAutoText),
	BackBox(NULL),  // otherwise other.BackBox->NextBox would be inconsistent
	NextBox(NULL),  // otherwise other.NextBox->BackBox would be inconsistent
	firstChar(0),   // since this box is unlinked now
	MaxChars(0),   // since the layout is invalid now
	inPdfArticle(other.inPdfArticle),
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
	DashOffset(other.DashOffset),
	fill_gradient(other.fill_gradient),
	fillRule(other.fillRule),
	doOverprint(other.doOverprint),
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
	isGroupControl(other.isGroupControl),
	groupsLastItem(other.groupsLastItem),
	BoundingX(other.BoundingX),
	BoundingY(other.BoundingY),
	BoundingW(other.BoundingW),
	BoundingH(other.BoundingH),
	ChangedMasterItem(other.ChangedMasterItem),
	OnMasterPage(other.OnMasterPage),
	isEmbedded(other.isEmbedded),

	// protected
	undoManager(other.undoManager),
	m_ItemType(other.m_ItemType),
	AnName(other.AnName),
	patternVal(other.patternVal),
	patternScaleX(other.patternScaleX),
	patternScaleY(other.patternScaleY),
	patternOffsetX(other.patternOffsetX),
	patternOffsetY(other.patternOffsetY),
	patternRotation(other.patternRotation),
	fillColorVal(other.fillColorVal),
	lineColorVal(other.lineColorVal),
	lineShadeVal(other.lineShadeVal),
	fillShadeVal(other.fillShadeVal),
	fillTransparencyVal(other.fillTransparencyVal),
	lineTransparencyVal(other.lineTransparencyVal),
	fillBlendmodeVal(other.fillBlendmodeVal),
	lineBlendmodeVal(other.lineBlendmodeVal),
	m_ImageIsFlippedH(other.m_ImageIsFlippedH),
	m_ImageIsFlippedV(other.m_ImageIsFlippedV),
	m_Locked(other.m_Locked),
	m_SizeLocked(other.m_SizeLocked),
	textFlowModeVal(other.textFlowModeVal),
	pageItemAttributes(other.pageItemAttributes),
	m_PrintEnabled(other.m_PrintEnabled),
	tagged(other.tagged),
	fillQColor(other.fillQColor),
	strokeQColor(other.strokeQColor),
	Xpos(other.Xpos),
	Ypos(other.Ypos),
	Width(other.Width),
	Height(other.Height),
	Rot(other.Rot),
	Select(other.Select),
	LocalScX(other.LocalScX),
	LocalScY(other.LocalScY),
	LocalX(other.LocalX),
	LocalY(other.LocalY),
	Reverse(other.Reverse),
	m_startArrowIndex(other.m_startArrowIndex),
	m_endArrowIndex(other.m_endArrowIndex),
	Extra(other.Extra),
	TExtra(other.TExtra),
	BExtra(other.BExtra),
	RExtra(other.RExtra),
	firstLineOffsetP(other.firstLineOffsetP),
	RadRect(other.RadRect),
	oldXpos(other.oldXpos),
	oldYpos(other.oldYpos),
	oldWidth(other.oldWidth),
	oldHeight(other.oldHeight),
	oldRot(other.oldRot),
	oldLocalScX(other.oldLocalScX),
	oldLocalScY(other.oldLocalScY),
	oldLocalX(other.oldLocalX),
	oldLocalY(other.oldLocalY),
	m_Doc(other.m_Doc),
	m_isAnnotation(other.m_isAnnotation),
	m_annotation(other.m_annotation),
	PicArt(other.PicArt),
	m_lineWidth(other.m_lineWidth),
	Oldm_lineWidth(other.Oldm_lineWidth)
{
	QString tmp;
	m_Doc->TotalItems++;
	AnName += tmp.setNum(m_Doc->TotalItems);
	uniqueNr = m_Doc->TotalItems;
	invalid = true;
}


PageItem::PageItem(ScribusDoc *pa, ItemType newType, double x, double y, double w, double h, double w2, QString fill, QString outline)
	// Initialize superclass(es)
	: QObject(pa), SingleObservable<PageItem>(pa->itemsChanged()),
	// Initialize member variables
	itemText(pa),
	undoManager(UndoManager::instance()),
	lineShadeVal(100),
	fillShadeVal(100),
	fillTransparencyVal(0.0),
	lineTransparencyVal(0.0),
	fillBlendmodeVal(0),
	lineBlendmodeVal(0),
	m_ImageIsFlippedH(0),
	m_ImageIsFlippedV(0),
	m_Locked(false),
	m_SizeLocked(false),
	textFlowModeVal(TextFlowDisabled)
{
	m_Doc = pa;
	QString tmp;
	BackBox = 0;
	NextBox = 0;
	oldXpos = Xpos = x;
	oldYpos = Ypos = y;
	//CB Surely we can remove some of these?
	OldB2 = OldB = oldWidth = Width = w;
	OldH2 = OldH = oldHeight = Height = h;
	BoundingX = x;
	BoundingY = y;
	BoundingW = w;
	BoundingH = h;
	m_ItemType = newType;
	oldRot = Rot = 0;
	fillColorVal = fill;
	lineColorVal = m_ItemType == PageItem::TextFrame ? fill : outline;
	GrType = 0;
	GrStartX = 0;
	GrStartY = 0;
	GrEndX = w;
	GrEndY = 0;
	patternVal = "";
	patternScaleX = 100;
	patternScaleY = 100;
	patternOffsetX = 0;
	patternOffsetY = 0;
	patternRotation = 0;
	m_lineWidth = w2;
	Oldm_lineWidth = w2;
	PLineArt = Qt::PenStyle(m_Doc->toolSettings.dLineArt);
	PLineEnd = Qt::FlatCap;
	PLineJoin = Qt::MiterJoin;
	Select = false;
	ClipEdited = false;
	FrameType = 0;
	CurX = 0;
	CurY = 0;
	CPos = 0;
	Extra = 0;
	TExtra = 0;
	BExtra = 0;
	RExtra = 0;
	firstChar = 0;
	MaxChars = 0;
	Pfile = "";
	pixm = ScImage();
	pixm.imgInfo.lowResType = m_Doc->toolSettings.lowResType;
	Pfile2 = "";
	Pfile3 = "";
	oldLocalScX = LocalScX = 1;
	oldLocalScY = LocalScY = 1;
	OrigW = 0;
	OrigH = 0;
	oldLocalX = LocalX = 0;
	oldLocalY = LocalY = 0;
	BBoxX = 0;
	BBoxH = 0;
	RadRect = 0;
	if ((m_ItemType == TextFrame) || (m_ItemType == ImageFrame) || (m_ItemType == PathText))
		// TODO: Frame should become a read-only calculated property
		Frame = true;
	else
		Frame = false;
	switch (m_ItemType)
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
	textPathType = 0;
	textPathFlipped = false;
	OwnPage = m_Doc->currentPage()->pageNr();
	oldOwnPage = OwnPage;
	savedOwnPage = OwnPage;
	PicArt = true;
	PicAvail = false;
	m_PrintEnabled = true;
	isBookmark = false;
	m_isAnnotation = false;
	
	switch (m_ItemType)
	{
	case ImageFrame:
	case LatexFrame:
		//We can't determine if this is a latex frame here
		// because c++'s typeinfos are still saying it's 
		// a plain pageitem
		// This is fixed in the PageItem_LatexFrame constructor
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
	m_Doc->TotalItems++;
	AnName += tmp.setNum(m_Doc->TotalItems); // +" "+QDateTime::currentDateTime().toString();
	uniqueNr = m_Doc->TotalItems;
	AutoName = true;
	setUName(AnName);
	m_annotation.setBorderColor(outline);
	HasSel = false;
//	Tinput = false;
	isAutoText = false;
	inPdfArticle = false;
	isRaster = false;
	Sizing = false;
	toPixmap = false;
	UseEmbedded = true;
	IRender = 1;
	EmProfile = "";
	Groups.clear();
	LayerNr = m_Doc->activeLayer();
	ScaleType = true;
	AspectRatio = true;
	Reverse = false;
	NamedLStyle = "";
	DashValues.clear();
	DashOffset = 0;
	fillRule = true;
	doOverprint = false;
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();
	if (fillColorVal != CommonStrings::None)
	{
		const ScColor& col = m_Doc->PageColors[fillColorVal];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, fillColorVal, 100);
		fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, fillColorVal, 100);
	}
	else
	{
		if (m_Doc->toolSettings.dBrush != CommonStrings::None)
		{
			const ScColor& col = m_Doc->PageColors[m_Doc->toolSettings.dBrush];
			QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
			fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_Doc->toolSettings.dBrush, 100);
			fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_Doc->toolSettings.dBrush, 100);
		}
		else
		{
			if (lineColorVal != CommonStrings::None)
			{
				const ScColor& col = m_Doc->PageColors[lineColorVal];
				QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
				fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, lineColorVal, 100);
				fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, lineColorVal, 100);
			}
			else
			{
				if (m_Doc->toolSettings.dPen != CommonStrings::None)
				{
					const ScColor& col = m_Doc->PageColors[m_Doc->toolSettings.dPen];
					QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
					fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_Doc->toolSettings.dPen, 100);
					fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_Doc->toolSettings.dPen, 100);
				}
				else if (m_Doc->PageColors.contains("Black"))
				{
					const ScColor& col = m_Doc->PageColors["Black"];
					QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
					fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, "Black", 100);
					fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, "Black", 100);
				}
			}
		}
	}
	firstLineOffsetP = FLOPRealGlyphHeight;
	Cols = m_Doc->toolSettings.dCols;
	ColGap = m_Doc->toolSettings.dGap;
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
	invalid = true;
	isGroupControl = false;
	groupsLastItem = 0;
	ChangedMasterItem = false;
	isEmbedded = false;
	OnMasterPage = m_Doc->currentPage()->pageName();
	m_startArrowIndex = m_Doc->toolSettings.dStartArrow;
	m_endArrowIndex = m_Doc->toolSettings.dEndArrow;
	effectsInUse.clear();
	//Page Item Attributes
	pageItemAttributes.clear();
	for(ObjAttrVector::Iterator objAttrIt = m_Doc->docItemAttributes.begin() ; objAttrIt != m_Doc->docItemAttributes.end(); ++objAttrIt )
	{
		if (((*objAttrIt).autoaddto=="textframes" && m_ItemType==TextFrame) ||
			((*objAttrIt).autoaddto=="imageframes" && m_ItemType==ImageFrame)
			)
			pageItemAttributes.append(*objAttrIt);
	}
}

void PageItem::setXPos(const double newXPos, bool drawingOnly)
{
	Xpos = newXPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
	emit position(Xpos, Ypos);
}

void PageItem::setYPos(const double newYPos, bool drawingOnly)
{
	Ypos = newYPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
	emit position(Xpos, Ypos);
}

void PageItem::setXYPos(const double newXPos, const double newYPos, bool drawingOnly)
{
	Xpos = newXPos;
	Ypos = newYPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
	emit position(Xpos, Ypos);
}

void PageItem::moveBy(const double dX, const double dY, bool drawingOnly)
{
	if (dX==0.0 && dY==0.0)
		return;
	if (dX!=0.0)
		Xpos+=dX;
	if (dY!=0.0)
		Ypos+=dY;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
	emit position(Xpos, Ypos);
}

void PageItem::setWidth(const double newWidth)
{
	Width = newWidth;
	checkChanges();
	emit widthAndHeight(Width, Height);
}

void PageItem::setHeight(const double newHeight)
{
	Height = newHeight;
	checkChanges();
	emit widthAndHeight(Width, Height);
}

void PageItem::setWidthHeight(const double newWidth, const double newHeight, bool drawingOnly)
{
	Width = newWidth;
	Height = newHeight;
	if (drawingOnly)
		return;
	checkChanges();
	emit widthAndHeight(Width, Height);
}

void PageItem::setWidthHeight(const double newWidth, const double newHeight)
{
	Width = newWidth;
	Height = newHeight;
	checkChanges();
	emit widthAndHeight(Width, Height);
}

void PageItem::resizeBy(const double dH, const double dW)
{
	if (dH==0.0 && dW==0.0)
		return;
	if (dH!=0.0)
		Width+=dH;
	if (dW!=0.0)
		Height+=dW;
	checkChanges();
	emit widthAndHeight(Width, Height);
}

void PageItem::setRotation(const double newRotation, bool drawingOnly)
{
	Rot=newRotation;
	checkChanges();
	if (drawingOnly || m_Doc->isLoading())
		return;
	emit rotation(Rot);
}

void PageItem::rotateBy(const double dR)
{
	if (dR==0.0)
		return;
	Rot+=dR;
	checkChanges();
	emit rotation(Rot);
}

void PageItem::setSelected(const bool toSelect)
{
	Select=toSelect;
}

void PageItem::setImageXScale(const double newImageXScale)
{
	LocalScX=newImageXScale;
	checkChanges();
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
}

void PageItem::setImageYScale(const double newImageYScale)
{
	LocalScY=newImageYScale;
	checkChanges();
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
}

void PageItem::setImageXYScale(const double newImageXScale, const double newImageYScale)
{
	LocalScX=newImageXScale;
	LocalScY=newImageYScale;
	checkChanges();
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
}

void PageItem::setImageXOffset(const double newImageXOffset)
{
	LocalX=newImageXOffset;
	checkChanges();
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
}

void PageItem::setImageYOffset(const double newImageYOffset)
{
	LocalY=newImageYOffset;
	checkChanges();
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
}

void PageItem::setImageXYOffset(const double newImageXOffset, const double newImageYOffset)
{
	LocalX=newImageXOffset;
	LocalY=newImageYOffset;
	checkChanges();
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
}

void PageItem::moveImageXYOffsetBy(const double dX, const double dY)
{
	if (dX==0.0 && dY==0.0)
		return;
	if (dX!=0.0)
		LocalX+=dX;
	if (dY!=0.0)
		LocalY+=dY;
	checkChanges();
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
}

void PageItem::setReversed(bool newReversed)
{
	Reverse=newReversed;
}


/// returns true if text overflows
bool PageItem::frameOverflows() const
{
#ifndef NLS_PROTO
	// Fix #6991 : "Text overflow" warning when there is a text underflow in fact
	/*return NextBox == NULL && itemText.length() > static_cast<int>(MaxChars);*/
	return (NextBox == NULL) && (static_cast<int>(firstChar) < itemText.length()) && (firstChar < MaxChars)
		&& (itemText.length() > static_cast<int>(MaxChars));
#else
	return false; // FIXME:NLS
#endif
}

int PageItem::firstInFrame() const
{
	return firstChar;
}
int PageItem::lastInFrame() const
{
#ifndef NLS_PROTO
	return qMin(signed(MaxChars), itemText.length()) - 1;
#else
	return itemText.length() - 1;
#endif
}


void PageItem::link(PageItem* nxt)
{
	assert( !nextInChain() );
	assert( !nxt->prevInChain() );
	for (PageItem* ff=nxt; ff; ff=ff->nextInChain())
	{
		assert (ff != this);
	}
	itemText.append(nxt->itemText);
	NextBox = nxt;
	nxt->BackBox = this;
	// update AutoText
	if (isAutoText)
	{
		PageItem* after = nxt;
		while (after)
		{
			after->isAutoText = true;
			m_Doc->LastAuto = after;
			after = after->NextBox;
		}
	}
	else if (nxt->isAutoText)
	{
		PageItem* before = this;
		while (before)
		{
			before->isAutoText = true;
			m_Doc->FirstAuto = before;
			before = before->BackBox;
		}
	}
	invalid = true;
	while (nxt)
	{
		nxt->itemText = itemText;
		nxt->invalid = true;
		nxt->firstChar = 0;
		nxt = nxt->NextBox;
	}
}

void PageItem::unlink()
{
	if( NextBox )
	{
		// make sure lastInFrame is valid
		layout();
		/*
		//<< CB #6332: Stop the StoryText cut and break and act like other publishing apps
		// move following text to new StoryText
		itemText.select(lastInFrame()+1, itemText.length()-lastInFrame()-1);
		*/
		StoryText follow(m_Doc);
		/*
		follow.setDefaultStyle(itemText.defaultStyle());
		follow.insert(0, itemText, true);
		// remove following text from this chain
		itemText.removeSelection();
		//>> 
		*/
		// update auto pointers
		if (isAutoText)
		{
			PageItem* before = this;
			while (before) 
			{
				before->isAutoText = false;
				before = before->BackBox;
			}
			m_Doc->FirstAuto = NextBox;
		}
		// link following frames to new text
		NextBox->firstChar = 0;
		NextBox->BackBox = NULL;
		while (NextBox) {
			NextBox->itemText = follow;
			NextBox->invalid = true;
			NextBox->firstChar = 0;
			NextBox = NextBox->NextBox;
		}
		// NextBox == NULL now
		NextBox = NULL;
	}
}


void PageItem::dropLinks()
{
	// update auto pointers
	if (isAutoText && NextBox == 0)
	{
		m_Doc->LastAuto = BackBox;
	}
	if (isAutoText && BackBox == 0)
	{
		m_Doc->FirstAuto = NextBox;
	}
	isAutoText = false;

	// leave text in remaining chain
	PageItem* before = BackBox;
	PageItem* after = NextBox;
	if (after != 0 || before != 0)
	{
		itemText = StoryText(m_Doc);
		if (before)
			before->NextBox = after;
		if (after) 
		{
			after->BackBox = before;
			while (after)
			{ 
				after->invalid = true;
				after->firstChar = 0;
				after = after->NextBox;
			}
		}
		// JG we should set BackBox and NextBox to NULL at a point
		BackBox = NextBox = NULL;
	}
}

/// tests if a character is displayed by this frame
bool PageItem::frameDisplays(int textpos) const
{
#ifndef NLS_PROTO
	return 0 <= textpos && textpos < signed(MaxChars) &&  textpos < itemText.length();
#else
	return true; // FIXME:NLS
#endif
}


/// returns the style at the current charpos
const ParagraphStyle& PageItem::currentStyle() const
{
	if (frameDisplays(CPos))
		return itemText.paragraphStyle(CPos);
	else
		return itemText.defaultStyle();
}

/// returns the style at the current charpos for changing
ParagraphStyle& PageItem::changeCurrentStyle()
{
	if (frameDisplays(CPos))
		return const_cast<ParagraphStyle&>(itemText.paragraphStyle(CPos));
	else
		return const_cast<ParagraphStyle&>(itemText.defaultStyle());
}

/// returns the style at the current charpos
const CharStyle& PageItem::currentCharStyle() const
{
	if (frameDisplays(CPos))
		return itemText.charStyle(CPos);
	else
		return itemText.defaultStyle().charStyle();
}

void PageItem::setTextToFrameDistLeft(double newLeft)
{
	Extra=newLeft;
	emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistRight(double newRight)
{
	RExtra=newRight;
	emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistTop(double newTop)
{
	TExtra=newTop;
	emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistBottom(double newBottom)
{
	BExtra=newBottom;
	emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDist(double newLeft, double newRight, double newTop, double newBottom)
{
	Extra=newLeft;
	RExtra=newRight;
	TExtra=newTop;
	BExtra=newBottom;
	emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

double PageItem::gridOffset() const { return m_Doc->typographicSettings.offsetBaseGrid; }
double PageItem::gridDistance() const { return m_Doc->typographicSettings.valueBaseGrid; }

void PageItem::setGridOffset(double) { } // FIXME
void PageItem::setGridDistance(double) { } // FIXME
void PageItem::setColumns(int n) 
{
	Cols = qMax(1, n); //FIXME: undo
}
void PageItem::setColumnGap(double gap)
{
	ColGap = gap; //FIXME: undo
}

void PageItem::setCornerRadius(double newRadius)
{
	RadRect=newRadius;
	emit cornerRadius(RadRect);
}









/** Paints the item.
    CHANGE: cullingArea is in doc coordinates!
 */
void PageItem::DrawObj(ScPainter *p, QRectF cullingArea)
{
	double sc;
	if (!m_Doc->DoDrawing)
	{
//		Tinput = false;
		return;
	}
	if (cullingArea.isNull())
	{
		cullingArea = QRectF(QPointF(m_Doc->minCanvasCoordinate.x(), m_Doc->minCanvasCoordinate.y()), 
							 QPointF(m_Doc->maxCanvasCoordinate.x(), m_Doc->maxCanvasCoordinate.y())).toAlignedRect();
	}
	
	DrawObj_Pre(p, sc);
	if (m_Doc->layerOutline(LayerNr))
	{
		if ((itemType()==TextFrame || itemType()==ImageFrame || itemType()==PathText || itemType()==Line || itemType()==PolyLine) && (!isGroupControl))
			DrawObj_Item(p, cullingArea, sc);
	}
	else
	{
		if (!isGroupControl)
			DrawObj_Item(p, cullingArea, sc);
	}
	DrawObj_Post(p);
}

void PageItem::DrawObj_Pre(ScPainter *p, double &sc)
{
	ScribusView* view = m_Doc->view();
	sc = view->scale();
	p->save();
	if (!isEmbedded)
		p->translate(Xpos, Ypos);
	p->rotate(Rot);
	if (m_Doc->layerOutline(LayerNr))
	{
		p->setPen(m_Doc->layerMarker(LayerNr), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::None);
		p->setBrushOpacity(1.0);
		p->setPenOpacity(1.0);
	}
	else
	{
	if (!isGroupControl)
	{
		if (fillBlendmode() != 0)
			p->beginLayer(1.0 - fillTransparency(), fillBlendmode());

		p->setLineWidth(m_lineWidth);
		if (GrType != 0)
		{
			if (GrType == 8)
			{
				if ((patternVal.isEmpty()) || (!m_Doc->docPatterns.contains(patternVal)))
				{
					p->fill_gradient = VGradient(VGradient::linear);
					if (fillColor() != CommonStrings::None)
					{
						p->setBrush(fillQColor);
						p->setFillMode(ScPainter::Solid);
					}
					else
						p->setFillMode(ScPainter::None);
					if ((!patternVal.isEmpty()) && (!m_Doc->docPatterns.contains(patternVal)))
					{
						GrType = 0;
						patternVal = "";
					}
				}
				else
				{
					p->setPattern(&m_Doc->docPatterns[patternVal], patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
					p->setFillMode(ScPainter::Pattern);
				}
			}
			else
			{
				if (fill_gradient.Stops() < 2) // fall back to solid filling if there are not enough colorstops in the gradient.
				{
					if (fillColor() != CommonStrings::None)
					{
						p->setBrush(fillQColor);
						p->setFillMode(ScPainter::Solid);
					}
					else
						p->setFillMode(ScPainter::None);
				}
				else
				{
					p->setFillMode(ScPainter::Gradient);
					p->fill_gradient = fill_gradient;
					QMatrix grm;
					grm.rotate(Rot);
					FPointArray gra;
					switch (GrType)
					{
						case 1:
						case 2:
						case 3:
						case 4:
						case 6:
							p->setGradient(VGradient::linear, FPoint(GrStartX, GrStartY), FPoint(GrEndX, GrEndY));
							break;
						case 5:
						case 7:
							gra.setPoints(2, GrStartX, GrStartY, GrEndX, GrEndY);
							p->setGradient(VGradient::radial, gra.point(0), gra.point(1), gra.point(0));
							break;
					}
				}
			}
		}
		else
		{
			p->fill_gradient = VGradient(VGradient::linear);
			if (fillColor() != CommonStrings::None)
			{
				p->setBrush(fillQColor);
				p->setFillMode(ScPainter::Solid);
			}
			else
				p->setFillMode(ScPainter::None);
		}
		if (lineColor() != CommonStrings::None)
		{
			if ((m_lineWidth == 0) && ! asLine())
				p->setLineWidth(0);
			else
			{
				p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
				if (DashValues.count() != 0)
					p->setDash(DashValues, DashOffset);
			}
		}
		else
			p->setLineWidth(0);
		if (fillBlendmode() == 0)
			p->setBrushOpacity(1.0 - fillTransparency());
		if (lineBlendmode() == 0)
			p->setPenOpacity(1.0 - lineTransparency());
		p->setFillRule(fillRule);
		}
	}
}

void PageItem::DrawObj_Post(ScPainter *p)
{
	bool doStroke=true;
	ScribusView* view = m_Doc->view();
	if (!isGroupControl)
	{
		if (m_Doc->layerOutline(LayerNr))
		{
			if (itemType()!=Line)
			{
				p->setPen(m_Doc->layerMarker(LayerNr), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->setFillMode(ScPainter::None);
				p->setBrushOpacity(1.0);
				p->setPenOpacity(1.0);
				if (itemType()==PolyLine)
					p->setupPolygon(&PoLine, false);
				else if (itemType() == PathText)
				{
					if (PoShow)
						p->setupPolygon(&PoLine, false);
					else
						doStroke = false;
				}
				else
					p->setupPolygon(&PoLine);
				if (doStroke)
					p->strokePath();
				if (itemType()==ImageFrame)
				{
					if (imageClip.size() != 0)
					{
						p->setupPolygon(&imageClip);
						p->strokePath();
					}
				}
			}
		}
		else
		{
			if (fillBlendmode() != 0)
				p->endLayer();
			if (itemType()==PathText || itemType()==PolyLine || itemType()==Line)
				doStroke=false;
			if ((doStroke) && (!m_Doc->RePos))
			{
				if (lineBlendmode() != 0)
					p->beginLayer(1.0 - lineTransparency(), lineBlendmode());
				if (lineColor() != CommonStrings::None)
				{
					p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
					if (DashValues.count() != 0)
						p->setDash(DashValues, DashOffset);
				}
				else
					p->setLineWidth(0);
				if (!isTableItem)
				{
					if (itemType() == ImageFrame)
						p->setupPolygon(&PoLine);
					if (NamedLStyle.isEmpty())
						p->strokePath();
					else
					{
						multiLine ml = m_Doc->MLineStyles[NamedLStyle];
						QColor tmp;
						for (int it = ml.size()-1; it > -1; it--)
						{
							struct SingleLine& sl = ml[it];
							// Qt4 if ((!sl.Color != CommonStrings::None) && (sl.Width != 0))
							if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
							{
								SetQColor(&tmp, sl.Color, sl.Shade);
								p->setPen(tmp, sl.Width, static_cast<Qt::PenStyle>(sl.Dash), static_cast<Qt::PenCapStyle>(sl.LineEnd), static_cast<Qt::PenJoinStyle>(sl.LineJoin));
								p->strokePath();
							}
						}
					}
				}
				if (lineBlendmode() != 0)
					p->endLayer();
			}
		}
	}
	if ((!isEmbedded) && (!m_Doc->RePos))
	{
		double aestheticFactor(5.0);
		double scpInv = 1.0 / (qMax(view->scale(), 1.0) * aestheticFactor);
		if (!isGroupControl)
		{
			if ((Frame) && (m_Doc->guidesSettings.framesShown) && ((itemType() == ImageFrame) || (itemType() == PathText)))
			{
				p->setPen(PrefsManager::instance()->appPrefs.DFrameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if ((isBookmark) || (m_isAnnotation))
					p->setPen(PrefsManager::instance()->appPrefs.DFrameAnnotationColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if ((BackBox != 0) || (NextBox != 0))
					p->setPen(PrefsManager::instance()->appPrefs.DFrameLinkColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if (m_Locked)
					p->setPen(PrefsManager::instance()->appPrefs.DFrameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->setFillMode(0);
				if (itemType()==PathText)
				{
					if (Clip.count() != 0)
					{
						FPointArray tclip;
						FPoint np = FPoint(Clip.point(0));
						tclip.resize(2);
						tclip.setPoint(0, np);
						tclip.setPoint(1, np);
						for (int a = 1; a < Clip.size(); ++a)
						{
							np = FPoint(Clip.point(a));
							tclip.putPoints(tclip.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
						}
						np = FPoint(Clip.point(0));
						tclip.putPoints(tclip.size(), 2, np.x(), np.y(), np.x(), np.y());
						p->setupPolygon(&tclip);
					}
				}
				else
// Ugly Hack to fix rendering problems with cairo-1.5.10 and up follows
#ifdef HAVE_CAIRO
	#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 5, 10)
					p->setupPolygon(&PoLine, false);
	#else
					p->setupPolygon(&PoLine);
	#endif
#else
					p->setupPolygon(&PoLine);
#endif
				p->strokePath();
			}
		}
		if ((m_Doc->guidesSettings.framesShown) && textFlowUsesContourLine() && (ContourLine.size() != 0))
		{
			p->setPen(Qt::lightGray, scpInv, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
// Ugly Hack to fix rendering problems with cairo-1.5.10 and up follows
#ifdef HAVE_CAIRO
	#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 5, 10)
			p->setupPolygon(&ContourLine, false);
	#else
			p->setupPolygon(&ContourLine);
	#endif
#else
			p->setupPolygon(&ContourLine);
#endif
			p->strokePath();
		}
		if ((m_Doc->guidesSettings.layerMarkersShown) && (m_Doc->layerCount() > 1) && (!m_Doc->layerOutline(LayerNr)) && ((isGroupControl) || (Groups.count() == 0)) && (!view->m_canvas->isPreviewMode()))
		{
			p->setPen(Qt::black, 0.5/ m_Doc->view()->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setPenOpacity(1.0);
			p->setBrush(m_Doc->layerMarker(LayerNr));
			p->setBrushOpacity(1.0);
			p->setFillMode(ScPainter::Solid);
			double ofwh = 10;
			double ofx = Width - ofwh/2;
			double ofy = Height - ofwh*3;
			p->drawRect(ofx, ofy, ofwh, ofwh);
		}
		//CB disabled for now
		//if (m_Doc->m_Selection->findItem(this)!=-1)
		//	drawLockedMarker(p);
	}
//	Tinput = false;
	FrameOnly = false;
	p->restore();
}

void PageItem::DrawObj_Embedded(ScPainter *p, QRectF cullingArea, const CharStyle& style, PageItem* cembedded)
{
	if (!cembedded)
		return;
	if (!m_Doc->DoDrawing)
		return;
	QList<PageItem*> emG;
	QStack<PageItem*> groupStack;
	groupStack.clear();
	emG.clear();
	emG.append(cembedded);
	if (cembedded->Groups.count() != 0)
	{
		for (int ga=0; ga<m_Doc->FrameItems.count(); ++ga)
		{
			if (m_Doc->FrameItems.at(ga)->Groups.count() != 0)
			{
				if (m_Doc->FrameItems.at(ga)->Groups.top() == cembedded->Groups.top())
				{
					if (m_Doc->FrameItems.at(ga)->ItemNr != cembedded->ItemNr)
					{
						if (!emG.contains(m_Doc->FrameItems.at(ga)))
							emG.append(m_Doc->FrameItems.at(ga));
					}
				}
			}
		}
	}
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		if (embedded->isGroupControl)
		{
			p->save();
			FPointArray cl = embedded->PoLine.copy();
			QMatrix mm;
			mm.translate((embedded->gXpos * (style.scaleH() / 1000.0)), ( - (embedded->gHeight * (style.scaleV() / 1000.0)) + embedded->gYpos * (style.scaleV() / 1000.0)));
			if (style.baselineOffset() != 0)
				mm.translate(0, -embedded->gHeight * (style.baselineOffset() / 1000.0));
			mm.scale(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
			mm.rotate(embedded->rotation());
			cl.map( mm );
			p->beginLayer(1.0 - embedded->fillTransparency(), embedded->fillBlendmode(), &cl);
			groupStack.push(embedded->groupsLastItem);
			continue;
		}
		p->save();
		double x = embedded->xPos();
		double y = embedded->yPos();
		embedded->Xpos = embedded->gXpos;
		embedded->Ypos = (embedded->gHeight * (style.scaleV() / 1000.0)) + embedded->gYpos;
		p->translate((embedded->gXpos * (style.scaleH() / 1000.0)), ( - (embedded->gHeight * (style.scaleV() / 1000.0)) + embedded->gYpos * (style.scaleV() / 1000.0)));
		if (style.baselineOffset() != 0)
		{
			p->translate(0, -embedded->gHeight * (style.baselineOffset() / 1000.0));
			embedded->Ypos -= embedded->gHeight * (style.baselineOffset() / 1000.0);
		}
		p->scale(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
		embedded->Dirty = Dirty;
		embedded->invalid = true;
		double sc;
		double pws = embedded->m_lineWidth;
		embedded->DrawObj_Pre(p, sc);
		switch(embedded->itemType())
		{
			case ImageFrame:
			case TextFrame:
			case LatexFrame:
			case Polygon:
			case PathText:
				embedded->DrawObj_Item(p, cullingArea, sc);
				break;
			case Line:
			case PolyLine:
				embedded->m_lineWidth = pws * qMin(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
				embedded->DrawObj_Item(p, cullingArea, sc);
				break;
			default:
				break;
		}
		embedded->m_lineWidth = pws * qMin(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
		embedded->DrawObj_Post(p);
		embedded->Xpos = x;
		embedded->Ypos = y;
		p->restore();
		if (groupStack.count() != 0)
		{
			while (embedded == groupStack.top())
			{
				p->endLayer();
				p->restore();
				groupStack.pop();
				if (groupStack.count() == 0)
					break;
			}
		}
		embedded->m_lineWidth = pws;
	}
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		if (!embedded->isTableItem)
			continue;
		p->save();
		double x = embedded->xPos();
		double y = embedded->yPos();
		embedded->Xpos = embedded->gXpos;
		embedded->Ypos = (embedded->gHeight * (style.scaleV() / 1000.0)) + embedded->gYpos;
		p->translate((embedded->gXpos * (style.scaleH() / 1000.0)), ( - (embedded->gHeight * (style.scaleV() / 1000.0)) + embedded->gYpos * (style.scaleV() / 1000.0)));
		if (style.baselineOffset() != 0)
		{
			p->translate(0, -embedded->gHeight * (style.baselineOffset() / 1000.0));
			embedded->Ypos -= embedded->gHeight * (style.baselineOffset() / 1000.0);
		}
		p->scale(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
		p->rotate(embedded->rotation());
		double pws = embedded->m_lineWidth;
		embedded->m_lineWidth = pws * qMin(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
		if ((embedded->lineColor() != CommonStrings::None) && (embedded->lineWidth() != 0.0))
		{
			QColor tmp;
			embedded->SetQColor(&tmp, embedded->lineColor(), embedded->lineShade());
			if ((embedded->TopLine) || (embedded->RightLine) || (embedded->BottomLine) || (embedded->LeftLine))
			{
				p->setPen(tmp, embedded->lineWidth(), embedded->PLineArt, Qt::SquareCap, embedded->PLineJoin);
				if (embedded->TopLine)
					p->drawLine(FPoint(0.0, 0.0), FPoint(embedded->width(), 0.0));
				if (embedded->RightLine)
					p->drawLine(FPoint(embedded->width(), 0.0), FPoint(embedded->width(), embedded->height()));
				if (embedded->BottomLine)
					p->drawLine(FPoint(embedded->width(), embedded->height()), FPoint(0.0, embedded->height()));
				if (embedded->LeftLine)
					p->drawLine(FPoint(0.0, embedded->height()), FPoint(0.0, 0.0));
			}
		}
		embedded->m_lineWidth = pws;
		embedded->Xpos = x;
		embedded->Ypos = y;
		p->restore();
	}
}


void PageItem::paintObj(QPainter *p)
{
	if ((!m_Doc->DoDrawing) || (m_Doc->RePos))
	{
		FrameOnly = false;
		return;
	}
	double sc = m_Doc->view()->scale();
	double handleSize = 6.0 / sc;
	double halfSize = 3.0 / sc;
	if ((!FrameOnly) && (!m_Doc->RePos))
	{
		if (!m_Doc->m_Selection->isEmpty())
		{
//				qDebug() << "Item: " << ItemNr << "W: " << Width << "H: " << Height;
			if (Groups.count() == 0)
			{
				//Locked line colour selection
				if (m_Locked)
					p->setPen(QPen(PrefsManager::instance()->appPrefs.DFrameLockColor, 1.0 / sc, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				else
					p->setPen(QPen(PrefsManager::instance()->appPrefs.DFrameColor, 1.0 / sc, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				p->setBrush(Qt::NoBrush);
				double lw2 = 1.0 / sc;
				double lw = 1.0 / sc;
				Qt::PenCapStyle le = Qt::FlatCap;
				if (NamedLStyle.isEmpty())
				{
					lw2 = (m_lineWidth / 2.0)  / sc;
					lw = qMax(m_lineWidth, 1.0)  / sc;
					le = PLineEnd;
				}
				else
				{
					multiLine ml = m_Doc->MLineStyles[NamedLStyle];
					lw2 = (ml[ml.size()-1].Width / 2.0) / sc;
					lw = qMax(ml[ml.size()-1].Width, 1.0) / sc;
					le = static_cast<Qt::PenCapStyle>(ml[ml.size()-1].LineEnd);
				}
				//Draw our frame outline
				if (asLine())
				{
					if (le != Qt::FlatCap)
						p->drawRect(QRectF(-lw2, -lw2, Width+lw, lw));
					else
						p->drawRect(QRectF(-1 / sc, -lw2, Width, lw));
				}
				else
					p->drawRect(QRectF(0, 0, Width, Height));
//				p->setPen(QPen(PrefsManager::instance()->appPrefs.DFrameColor, 1.0 / sc, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				p->setBrush(PrefsManager::instance()->appPrefs.DFrameColor);
				p->setPen(Qt::NoPen);
				if ((!m_Locked) && (!m_SizeLocked))
				{
					if (! asLine())
					{
						p->drawRect(QRectF(0.0, 0.0, handleSize, handleSize));
						p->drawRect(QRectF(Width - handleSize, Height - handleSize, handleSize, handleSize));
						p->drawRect(QRectF(Width - handleSize, 0.0, handleSize, handleSize));
						p->drawRect(QRectF(0.0, Height - handleSize, handleSize, handleSize));
						if (Width > 6)
						{
							p->drawRect(QRectF(Width / 2.0 - halfSize, Height - handleSize, handleSize, handleSize));
							p->drawRect(QRectF(Width / 2.0 - halfSize, 0.0, handleSize, handleSize));
						}
						if (Height > 6)
						{
							p->drawRect(QRectF(Width - handleSize, Height / 2.0 - halfSize, handleSize, handleSize));
							p->drawRect(QRectF(0.0, Height / 2.0 - halfSize, handleSize, handleSize));
						}
					}
					else
					{
						p->drawRect(QRectF(-halfSize, -halfSize, handleSize, handleSize));
						p->drawRect(QRectF(Width + halfSize, -halfSize, -handleSize, handleSize));
					}
				}
			}
			else
			{
				p->setPen(QPen(PrefsManager::instance()->appPrefs.DFrameGroupColor, 1.0 / sc, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				p->setBrush(Qt::NoBrush);
				p->drawRect(QRectF(0.0, 0.0, Width, Height));
				if (m_Doc->m_Selection->count() == 1)
				{
					p->setPen(Qt::NoPen);
//					p->setPen(QPen(PrefsManager::instance()->appPrefs.DFrameGroupColor, 1.0 / sc, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
					p->setBrush(PrefsManager::instance()->appPrefs.DFrameGroupColor);
					p->drawRect(QRectF(0.0, 0.0, handleSize, handleSize));
					p->drawRect(QRectF(Width - handleSize, Height - handleSize, handleSize, handleSize));
					p->drawRect(QRectF(Width - handleSize, 0.0, handleSize, handleSize));
					p->drawRect(QRectF(0.0, Height - handleSize, handleSize, handleSize));
					if (Width > 6)
					{
						p->drawRect(QRectF(Width / 2.0 - halfSize, Height - handleSize, handleSize, handleSize));
						p->drawRect(QRectF(Width / 2.0 - halfSize, 0.0, handleSize, handleSize));
					}
					if (Height > 6)
					{
						p->drawRect(QRectF(Width - handleSize, Height / 2.0 - halfSize, handleSize, handleSize));
						p->drawRect(QRectF(0.0, Height / 2.0 - halfSize, handleSize, handleSize));
					}
				}
			}
		}
	}
	FrameOnly = false;
}

QImage PageItem::DrawObj_toImage()
{
	QList<PageItem*> emG;
	emG.clear();
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	if (Groups.count() != 0)
	{
		for (int ga=0; ga<m_Doc->Items->count(); ++ga)
		{
			if (m_Doc->Items->at(ga)->Groups.count() != 0)
			{
				if (m_Doc->Items->at(ga)->Groups.top() == Groups.top())
				{
					if (!emG.contains(m_Doc->Items->at(ga)))
					{
						emG.append(m_Doc->Items->at(ga));
						PageItem *currItem = m_Doc->Items->at(ga);
						double x1, x2, y1, y2;
						currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
						minx = qMin(minx, x1);
						miny = qMin(miny, y1);
						maxx = qMax(maxx, x2);
						maxy = qMax(maxy, y2);
					}
				}
			}
		}
		for (int em = 0; em < emG.count(); ++em)
		{
			PageItem* currItem = emG.at(em);
			currItem->gXpos = currItem->visualXPos() - minx;
			currItem->gYpos = currItem->visualYPos() - miny;
			currItem->gWidth = maxx - minx;
			currItem->gHeight = maxy - miny;
		}
	}
	else
	{
		double x1, x2, y1, y2;
		getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		gXpos = xPos() - minx;
		gYpos = yPos() - miny;
		gWidth = maxx - minx;
		gHeight = maxy - miny;
		emG.append(this);
	}
	return DrawObj_toImage(emG);
}

QImage PageItem::DrawObj_toImage(QList<PageItem*> &emG)
{
	QImage retImg = QImage(qRound(gWidth), qRound(gHeight), QImage::Format_ARGB32);
//	retImg.fill( qRgba(255, 255, 255, 0) );
	retImg.fill( qRgba(0, 0, 0, 0) );
	ScPainter *painter = new ScPainter(&retImg, retImg.width(), retImg.height(), 1, 0);
	painter->setZoomFactor(1.0);
	QStack<PageItem*> groupStack;
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		if (embedded->isGroupControl)
		{
			painter->save();
			FPointArray cl = embedded->PoLine.copy();
			QMatrix mm;
			mm.translate(embedded->gXpos, embedded->gYpos);
			mm.rotate(embedded->rotation());
			cl.map( mm );
			painter->beginLayer(1.0 - embedded->fillTransparency(), embedded->fillBlendmode(), &cl);
			groupStack.push(embedded->groupsLastItem);
			continue;
		}
		painter->save();
		double x = embedded->xPos();
		double y = embedded->yPos();
		embedded->Xpos = embedded->gXpos;
		embedded->Ypos = embedded->gYpos;
		painter->translate(embedded->gXpos, embedded->gYpos);
		embedded->isEmbedded = true;
		embedded->invalid = true;
		embedded->DrawObj(painter, QRectF());
		embedded->Xpos = x;
		embedded->Ypos = y;
		embedded->isEmbedded = false;
		painter->restore();
		if (groupStack.count() != 0)
		{
			while (embedded == groupStack.top())
			{
				painter->endLayer();
				painter->restore();
				groupStack.pop();
				if (groupStack.count() == 0)
					break;
			}
		}
	}
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		if (!embedded->isTableItem)
			continue;
		painter->save();
		double x = embedded->xPos();
		double y = embedded->yPos();
		embedded->Xpos = embedded->gXpos;
		embedded->Ypos = embedded->gYpos;
		painter->translate(embedded->gXpos, embedded->gYpos);
		painter->rotate(embedded->rotation());
		embedded->isEmbedded = true;
		embedded->invalid = true;
		if ((embedded->lineColor() != CommonStrings::None) && (embedded->lineWidth() != 0.0))
		{
			QColor tmp;
			embedded->SetQColor(&tmp, embedded->lineColor(), embedded->lineShade());
			if ((embedded->TopLine) || (embedded->RightLine) || (embedded->BottomLine) || (embedded->LeftLine))
			{
				painter->setPen(tmp, embedded->lineWidth(), embedded->PLineArt, Qt::SquareCap, embedded->PLineJoin);
				if (embedded->TopLine)
					painter->drawLine(FPoint(0.0, 0.0), FPoint(embedded->width(), 0.0));
				if (embedded->RightLine)
					painter->drawLine(FPoint(embedded->width(), 0.0), FPoint(embedded->width(), embedded->height()));
				if (embedded->BottomLine)
					painter->drawLine(FPoint(embedded->width(), embedded->height()), FPoint(0.0, embedded->height()));
				if (embedded->LeftLine)
					painter->drawLine(FPoint(0.0, embedded->height()), FPoint(0.0, 0.0));
			}
		}
		embedded->isEmbedded = false;
		embedded->Xpos = x;
		embedded->Ypos = y;
		painter->restore();
	}
	painter->end();
	delete painter;
	return retImg;
}

QString PageItem::ExpandToken(uint base)
{
	uint zae = 0;
	QChar ch = itemText.text(base);
	QString chstr = ch;
	if (ch == SpecialChars::PAGENUMBER)
	{
		// compatibility mode: ignore subsequent pagenumber chars
		if (base > 0 && itemText.text(base-1) == SpecialChars::PAGENUMBER)
			return "";
		if ((!m_Doc->masterPageMode()) && (OwnPage != -1))
		{
			QString out("%1");
			//CB Section numbering
			chstr = out.arg(m_Doc->getSectionPageNumberForPageIndex(OwnPage), -(int)zae);
		}
		else
			return "#";
	}
	else if (ch == SpecialChars::PAGECOUNT)
	{
		if (!m_Doc->masterPageMode())
		{
			QString out("%1");
			int key = m_Doc->getSectionKeyForPageIndex(OwnPage);
			if (key == -1)
				return "%";
			chstr = out.arg(getStringFromSequence(m_Doc->sections[key].type, m_Doc->sections[key].toindex - m_Doc->sections[key].fromindex + 1));
		}
		else
			return "%";
	}
	return chstr;
}

void PageItem::SetQColor(QColor *tmp, QString farbe, double shad)
{
	const ScColor& col = m_Doc->PageColors[farbe];
	*tmp = ScColorEngine::getShadeColorProof(col, m_Doc, shad);
	if ((m_Doc->view()) && (m_Doc->view()->m_canvas->usePreviewVisual()))
	{
		VisionDefectColor defect;
		*tmp = defect.convertDefect(*tmp, m_Doc->view()->m_canvas->previewVisual());
	}
}

/**
    layout glyphs translates the chars into a number of glyphs, applying the Charstyle
    'style'. The following fields are set in layout: glyph, more, scaleH, scaleV, xoffset, yoffset, xadvance.
    If the DropCap-bit in style.effects is set and yadvance is > 0, scaleH/V, x/yoffset and xadvance
    are modified to scale the glyphs to this height.
    Otherwise yadvance is set to the max ascender of all generated glyphs.
    It scales according to smallcaps and
    sets xadvance to the advance width without kerning. If more than one glyph
    is generated, kerning is included in all but the last xadvance.
*/
double PageItem::layoutGlyphs(const CharStyle& style, const QString& chars, GlyphLayout& layout)
{
	double retval = 0.0;
	double asce = style.font().ascent(style.fontSize() / 10.0);
	int chst = style.effects() & 1919;
/*	if (chars[0] == SpecialChars::ZWSPACE ||
		chars[0] == SpecialChars::ZWNBSPACE ||
		chars[0] == SpecialChars::NBSPACE ||
		chars[0] == SpecialChars::NBHYPHEN ||
		chars[0] == SpecialChars::SHYPHEN ||
		chars[0] == SpecialChars::PARSEP ||
		chars[0] == SpecialChars::COLBREAK ||
		chars[0] == SpecialChars::LINEBREAK ||
		chars[0] == SpecialChars::FRAMEBREAK ||
		chars[0] == SpecialChars::TAB)
	{
		layout.glyph = ScFace::CONTROL_GLYPHS + chars[0].unicode();
	}
	else */
	{
		layout.glyph = style.font().char2CMap(chars[0].unicode());
	}
	
	double tracking = 0.0;
	if ( (style.effects() & ScStyle_StartOfLine) == 0)
		tracking = style.fontSize() * style.tracking() / 10000.0;

	layout.xoffset = tracking;
	layout.yoffset = 0;
	if (chst != ScStyle_Default)
	{
		if (chst & ScStyle_Superscript)
		{
			retval -= asce * m_Doc->typographicSettings.valueSuperScript / 100.0;
			layout.yoffset -= asce * m_Doc->typographicSettings.valueSuperScript / 100.0;
			layout.scaleV = layout.scaleH = qMax(m_Doc->typographicSettings.scalingSuperScript / 100.0, 10.0 / style.fontSize());
		}
		else if (chst & ScStyle_Subscript)
		{
			retval += asce * m_Doc->typographicSettings.valueSubScript / 100.0;
			layout.yoffset += asce * m_Doc->typographicSettings.valueSubScript / 100.0;
			layout.scaleV = layout.scaleH = qMax(m_Doc->typographicSettings.scalingSubScript / 100.0, 10.0 / style.fontSize());
		}
		else {
			layout.scaleV = layout.scaleH = 1.0;
		}
		layout.scaleH *= style.scaleH() / 1000.0;
		layout.scaleV *= style.scaleV() / 1000.0;
		if (chst & ScStyle_AllCaps)
		{
			layout.glyph = style.font().char2CMap(chars[0].toUpper().unicode());
		}
		if (chst & ScStyle_SmallCaps)
		{
			double smallcapsScale = m_Doc->typographicSettings.valueSmallCaps / 100.0;
			QChar uc = chars[0].toUpper();
			if (uc != chars[0])
			{
				layout.glyph = style.font().char2CMap(chars[0].toUpper().unicode());
				layout.scaleV *= smallcapsScale;
				layout.scaleH *= smallcapsScale;
			}
		}
	}
	else {
		layout.scaleH = style.scaleH() / 1000.0;
		layout.scaleV = style.scaleV() / 1000.0;
	}	
	
/*	if (layout.glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBSPACE.unicode())) {
		uint replGlyph = style.font().char2CMap(QChar(' '));
		layout.xadvance = style.font().glyphWidth(replGlyph, style.fontSize() / 10) * layout.scaleH;
		layout.yadvance = style.font().glyphBBox(replGlyph, style.fontSize() / 10).ascent * layout.scaleV;
	}
	else if (layout.glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBHYPHEN.unicode())) {
		uint replGlyph = style.font().char2CMap(QChar('-'));
		layout.xadvance = style.font().glyphWidth(replGlyph, style.fontSize() / 10) * layout.scaleH;
		layout.yadvance = style.font().glyphBBox(replGlyph, style.fontSize() / 10).ascent * layout.scaleV;
	}
	else if (layout.glyph >= ScFace::CONTROL_GLYPHS) {
		layout.xadvance = 0;
		layout.yadvance = 0;
	}
	else */
	{
		layout.xadvance = style.font().glyphWidth(layout.glyph, style.fontSize() / 10) * layout.scaleH;
		layout.yadvance = style.font().glyphBBox(layout.glyph, style.fontSize() / 10).ascent * layout.scaleV;
	}
	if (layout.xadvance > 0)
		layout.xadvance += tracking;

	if (chars.length() > 1) {
		layout.grow();
		layoutGlyphs(style, chars.mid(1), *layout.more);
		layout.xadvance += style.font().glyphKerning(layout.glyph, layout.more->glyph, style.fontSize() / 10) * layout.scaleH;
		if (layout.more->yadvance > layout.yadvance)
			layout.yadvance = layout.more->yadvance;
	}
	else {
		layout.shrink();
	}
	return retval;
}

void PageItem::drawGlyphs(ScPainter *p, const CharStyle& style, GlyphLayout& glyphs)
{
	uint glyph = glyphs.glyph;
	if ((m_Doc->guidesSettings.showControls) && 
		(glyph == style.font().char2CMap(QChar(' ')) || glyph >=  ScFace::CONTROL_GLYPHS))
	{
		bool stroke = false;
		if (glyph >=  ScFace::CONTROL_GLYPHS)
			glyph -= ScFace::CONTROL_GLYPHS;
		else
			glyph = 32;
		QMatrix chma, chma4, chma5;
		FPointArray points;
		if (glyph == SpecialChars::TAB.unicode())
		{
			points = m_Doc->symTab.copy();
			chma4.translate(glyphs.xoffset + glyphs.xadvance - ((style.fontSize() / 10.0) * glyphs.scaleH * 0.7), glyphs.yoffset - ((style.fontSize() / 10.0) * glyphs.scaleV * 0.5));
		}
		else if (glyph == SpecialChars::COLBREAK.unicode())
		{
			points = m_Doc->symNewCol.copy();
			chma4.translate(glyphs.xoffset, glyphs.yoffset-((style.fontSize() / 10.0) * glyphs.scaleV * 0.6));
		}
		else if (glyph == SpecialChars::FRAMEBREAK.unicode())
		{
			points = m_Doc->symNewFrame.copy();
			chma4.translate(glyphs.xoffset, glyphs.yoffset-((style.fontSize() / 10.0) * glyphs.scaleV * 0.6));
		}
		else if (glyph == SpecialChars::PARSEP.unicode())
		{
			points = m_Doc->symReturn.copy();
			chma4.translate(glyphs.xoffset, glyphs.yoffset-((style.fontSize() / 10.0) * glyphs.scaleV * 0.8));
		}
		else if (glyph == SpecialChars::LINEBREAK.unicode())
		{
			points = m_Doc->symNewLine.copy();
			chma4.translate(glyphs.xoffset, glyphs.yoffset-((style.fontSize() / 10.0) * glyphs.scaleV * 0.4));
		}
		else if (glyph == SpecialChars::NBSPACE.unicode() ||
				 glyph == 32)
		{
			stroke = (glyph == 32);
			points = m_Doc->symNonBreak.copy();
			chma4.translate(glyphs.xoffset, glyphs.yoffset-((style.fontSize() / 10.0) * glyphs.scaleV * 0.4));
		}
		else if (glyph == SpecialChars::NBHYPHEN.unicode())
		{
			points = style.font().glyphOutline(style.font().char2CMap(QChar('-')), style.fontSize() / 100);
			chma4.translate(glyphs.xoffset, glyphs.yoffset-((style.fontSize() / 10.0) * glyphs.scaleV));
		}
		else if (glyph == SpecialChars::SHYPHEN.unicode())
		{
			points.resize(0);
			points.addQuadPoint(0, -10, 0, -10, 0, -6, 0, -6);
			stroke = true;
		}
		else // ???
		{
			points.resize(0);
			points.addQuadPoint(0, -10, 0, -10, 0, -9, 0, -9);			
			points.addQuadPoint(0, -9, 0, -9, 1, -9, 1, -9);			
			points.addQuadPoint(1, -9, 1, -9, 1, -10, 1, -10);			
			points.addQuadPoint(1, -10, 1, -10, 0, -10, 0, -10);			
		}
		chma.scale(glyphs.scaleH * style.fontSize() / 100.0, glyphs.scaleV * style.fontSize() / 100.0);
		points.map(chma * chma4);
		p->setupPolygon(&points, true);
		QColor oldBrush = p->brush();
		p->setBrush( (style.effects() & ScStyle_SuppressSpace) ? Qt::green
					: PrefsManager::instance()->appPrefs.DControlCharColor);
		if (stroke)
		{
			QColor tmp = p->pen();
			p->setPen(p->brush(), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setLineWidth(style.fontSize() * glyphs.scaleV / 200.0);
			p->strokePath();
			p->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		}
		else
		{
			p->setFillMode(1);
			p->fillPath();
		}
		p->setBrush(oldBrush);
		if (glyphs.more)
		{
			p->translate(glyphs.xadvance, 0);
			drawGlyphs(p, style, *glyphs.more);
		}			
		return;
	}
	else if (glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBSPACE.unicode()) ||
			 glyph == (ScFace::CONTROL_GLYPHS + 32)) 
		glyph = style.font().char2CMap(QChar(' '));
	else if (glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBHYPHEN.unicode()))
		glyph = style.font().char2CMap(QChar('-'));
	
	if (glyph >= ScFace::CONTROL_GLYPHS || (style.effects() & ScStyle_SuppressSpace)) {
//		qDebug(QString("drawGlyphs: skipping %1").arg(glyph));
		// all those are empty
		if (glyphs.more)
		{
			p->translate(glyphs.xadvance, 0);
			drawGlyphs(p, style, *glyphs.more);
		}			
		return;
	}
//	if (style.font().canRender(QChar(glyph)))
	{
		FPointArray gly = style.font().glyphOutline(glyph);
		// Do underlining first so you can get typographically correct
		// underlines when drawing a white outline
		if (((style.effects() & ScStyle_Underline) || ((style.effects() & ScStyle_UnderlineWords) && glyph != style.font().char2CMap(QChar(' ')))) && (style.strokeColor() != CommonStrings::None))
		{
			double st, lw;
			if ((style.underlineOffset() != -1) || (style.underlineWidth() != -1))
			{
				if (style.underlineOffset() != -1)
					st = (style.underlineOffset() / 1000.0) * (style.font().descent(style.fontSize() / 10.0));
				else
					st = style.font().underlinePos(style.fontSize() / 10.0);
				if (style.underlineWidth() != -1)
					lw = (style.underlineWidth() / 1000.0) * (style.fontSize() / 10.0);
				else
					lw = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			else
			{
				st = style.font().underlinePos(style.fontSize() / 10.0);
				lw = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			if (style.baselineOffset() != 0)
				st += (style.fontSize() / 10.0) * glyphs.scaleV * (style.baselineOffset() / 1000.0);
			QColor tmpC = p->pen();
			p->setPen(p->brush());
			p->setLineWidth(lw);
			if (style.effects() & ScStyle_Subscript)
				p->drawLine(FPoint(glyphs.xoffset, glyphs.yoffset - st), FPoint(glyphs.xoffset + glyphs.xadvance, glyphs.yoffset - st));
			else
				p->drawLine(FPoint(glyphs.xoffset, -st), FPoint(glyphs.xoffset + glyphs.xadvance, -st));
			p->setPen(tmpC);
		}
		if (gly.size() > 3)
		{
			if (glyph == 0)
			{
//				qDebug(QString("glyph 0: (%1,%2) * %3 %4 + %5").arg(glyphs.xoffset).arg(glyphs.yoffset).arg(glyphs.scaleH).arg(glyphs.scaleV).arg(glyphs.xadvance));
			}
			p->save();
			p->translate(glyphs.xoffset, glyphs.yoffset - ((style.fontSize() / 10.0) * glyphs.scaleV));
			if (Reverse)
			{
				p->scale(-1, 1);
				p->translate(-glyphs.xadvance, 0);
			}
			if (style.baselineOffset() != 0)
				p->translate(0, -(style.fontSize() / 10.0) * (style.baselineOffset() / 1000.0));
			double glxSc = glyphs.scaleH * style.fontSize() / 100.00;
			double glySc = glyphs.scaleV * style.fontSize() / 100.0;
			p->scale(glxSc, glySc);
//			p->setFillMode(1);
			bool fr = p->fillRule();
			p->setFillRule(false);
//			double	a = gly.point(0).x();
//			double	b = gly.point(0).y();
//			double	c = gly.point(3).x();
//			double	d = gly.point(3).y();
//			qDebug(QString("drawglyphs: %1 (%2,%3) (%4,%5) scaled %6,%7 trans %8,%9")
//				   .arg(gly.size()).arg(a).arg(b).arg(c).arg(d)
//				   .arg(p->worldMatrix().m11()).arg(p->worldMatrix().m22()).arg(p->worldMatrix().dx()).arg(p->worldMatrix().dy()));
			p->setupPolygon(&gly, true);
			if (m_Doc->layerOutline(LayerNr))
			{
				p->save();
				p->setPen(m_Doc->layerMarker(LayerNr), 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->setFillMode(ScPainter::None);
				p->setBrushOpacity(1.0);
				p->setPenOpacity(1.0);
				p->strokePath();
				p->restore();
				p->setFillRule(fr);
				p->restore();
				if (glyphs.more)
				{
					p->translate(glyphs.xadvance, 0);
					drawGlyphs(p, style, *glyphs.more);
				}
				return;
			}
			if (glyph == 0)
			{
				p->setPen(PrefsManager::instance()->appPrefs.DControlCharColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->setLineWidth(style.fontSize() * glyphs.scaleV * style.outlineWidth() * 2 / 10000.0);
				p->strokePath();
			}
			else if ((style.font().isStroked()) && (style.strokeColor() != CommonStrings::None) && ((style.fontSize() * glyphs.scaleV * style.outlineWidth() / 10000.0) != 0))
			{
				QColor tmp = p->brush();
				p->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->setLineWidth(style.fontSize() * glyphs.scaleV * style.outlineWidth() / 10000.0);
				p->strokePath();
			}
			else
			{
				if ((style.effects() & ScStyle_Shadowed) && (style.strokeColor() != CommonStrings::None))
				{
					p->save();
					p->translate((style.fontSize() * glyphs.scaleH * style.shadowXOffset() / 10000.0) / glxSc, -(style.fontSize() * glyphs.scaleV * style.shadowYOffset() / 10000.0) / glySc);
					QColor tmp = p->brush();
					p->setBrush(p->pen());
					p->setupPolygon(&gly, true);
					p->fillPath();
					p->setBrush(tmp);
					p->restore();
					p->setupPolygon(&gly, true);
				}
				if (style.fillColor() != CommonStrings::None)
					p->fillPath();
				if ((style.effects() & ScStyle_Outline) && (style.strokeColor() != CommonStrings::None) && ((style.fontSize() * glyphs.scaleV * style.outlineWidth() / 10000.0) != 0))
				{
					p->setLineWidth((style.fontSize() * glyphs.scaleV * style.outlineWidth() / 10000.0) / glySc);
					p->strokePath();
				}
			}
			p->setFillRule(fr);
			p->restore();
		}
		else {
//			qDebug(QString("drawGlyphs: empty glyph %1").arg(glyph));
		}
		if ((style.effects() & ScStyle_Strikethrough) && (style.strokeColor() != CommonStrings::None))
		{
			double st, lw;
			if ((style.strikethruOffset() != -1) || (style.strikethruWidth() != -1))
			{
				if (style.strikethruOffset() != -1)
					st = (style.strikethruOffset() / 1000.0) * (style.font().ascent(style.fontSize() / 10.0));
				else
					st = style.font().strikeoutPos(style.fontSize() / 10.0);
				if (style.strikethruWidth() != -1)
					lw = (style.strikethruWidth() / 1000.0) * (style.fontSize() / 10.0);
				else
					lw = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			else
			{
				st = style.font().strikeoutPos(style.fontSize() / 10.0);
				lw = qMax(style.font().strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			if (style.baselineOffset() != 0)
				st += (style.fontSize() / 10.0) * glyphs.scaleV * (style.baselineOffset() / 1000.0);
			p->setPen(p->brush());
			p->setLineWidth(lw);
			p->drawLine(FPoint(glyphs.xoffset, glyphs.yoffset - st), FPoint(glyphs.xoffset + glyphs.xadvance, glyphs.yoffset - st));
		}
	}
/*	else
	{
		p->setLineWidth(1);
		p->setPen(red);
		p->setBrush(red);
		p->setFillMode(1);
		p->drawRect(glyphs.xoffset, glyphs.yoffset - (style.fontSize() / 10.0) * glyphs.scaleV , (style.fontSize() / 10.0) * glyphs.scaleH, (style.fontSize() / 10.0) * glyphs.scaleV);
	}
	*/	
	if (glyphs.more)
	{
		p->translate(glyphs.xadvance, 0);
		drawGlyphs(p, style, *glyphs.more);
	}
}

void PageItem::DrawPolyL(QPainter *p, QPolygon pts)
{
	QColor tmp;
	ScribusView* view = m_Doc->view();
	if (Segments.count() != 0)
	{
		QList<uint>::Iterator it2end=Segments.end();
		uint FirstVal = 0;
		for (QList<uint>::Iterator it2 = Segments.begin(); it2 != it2end; ++it2)
		{
			if (NamedLStyle.isEmpty())
				p->drawPolygon(pts.constData() + FirstVal, (*it2)-FirstVal);
			else
			{
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					SetQColor(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(QPen(tmp,
									 qMax(static_cast<int>(ml[it].Width* view->scale()), 1),
									 static_cast<Qt::PenStyle>(ml[it].Dash),
									 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
									 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
					p->drawPolygon(pts.constData() + FirstVal, (*it2)-FirstVal);
				}
			}
			FirstVal = (*it2);
		}
		if (NamedLStyle.isEmpty())
			p->drawPolygon(pts.constData() + FirstVal, pts.size() - FirstVal);
		else
		{
			multiLine ml = m_Doc->MLineStyles[NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				SetQColor(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 qMax(static_cast<int>(ml[it].Width* view->scale()), 1),
								 static_cast<Qt::PenStyle>(ml[it].Dash),
								 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
								 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolygon(pts.constData() + FirstVal, pts.size() - FirstVal);
			}
		}
	}
	else
	{
		if (NamedLStyle.isEmpty())
			p->drawPolygon(pts);
		else
		{
			multiLine ml = m_Doc->MLineStyles[NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				SetQColor(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(QPen(tmp,
								 qMax(static_cast<int>(ml[it].Width*view->scale()), 1),
								 static_cast<Qt::PenStyle>(ml[it].Dash),
								 static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
								 static_cast<Qt::PenJoinStyle>(ml[it].LineJoin)));
				p->drawPolygon(pts);
			}
		}
	}
}

void PageItem::setItemName(const QString& newName)
{
	if (AnName == newName)
		return; // nothing to do -> return
	if (newName.isEmpty())
		return;
	AnName = generateUniqueCopyName(newName);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Rename, QString(Um::FromTo).arg(AnName).arg(newName));
		ss->set("OLD_NAME", AnName);
		ss->set("NEW_NAME", newName);
		undoManager->action(this, ss);
	}
	setUName(AnName); // set the name for the UndoObject too
}

void PageItem::setPattern(const QString &newPattern)
{
	if (patternVal != newPattern)
		patternVal = newPattern;
}

void PageItem::gradientVector(double& startX, double& startY, double& endX, double& endY) const
{
	startX = GrStartX;
	startY = GrStartY;
	endX   = GrEndX;
	endY   = GrEndY;
}

void PageItem::setGradientVector(double startX, double startY, double endX, double endY)
{
	GrStartX = startX;
	GrStartY = startY;
	GrEndX   = endX;
	GrEndY   = endY;
}

void PageItem::setPatternTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation)
{
	patternScaleX = scaleX;
	patternScaleY = scaleY;
	patternOffsetX = offsetX;
	patternOffsetY = offsetY;
	patternRotation = rotation;
}

void  PageItem::patternTransform(double &scaleX, double &scaleY, double &offsetX, double &offsetY, double &rotation) const
{
	 scaleX = patternScaleX;
	 scaleY = patternScaleY;
	 offsetX = patternOffsetX;
	 offsetY = patternOffsetY;
	 rotation = patternRotation;
}

void PageItem::setFillColor(const QString &newColor)
{
	QString tmp = newColor;
	if (tmp != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(newColor))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
					tmp = m_Doc->toolSettings.dBrushPict;
				case TextFrame:
				case PathText:
					tmp = m_Doc->toolSettings.dTextBackGround;
					break;
				case Line:
				case PolyLine:
				case Polygon:
					tmp = m_Doc->toolSettings.dBrush;
					break;
				default:
					break;
			}
		}
	}
	if (fillColorVal == tmp)
	{
		setFillQColor();
		return;
	}
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetFill,
										  QString(Um::ColorFromTo).arg(fillColorVal).arg(tmp),
                                          Um::IFill);
		ss->set("FILL", "fill");
		ss->set("OLD_FILL", fillColorVal);
		ss->set("NEW_FILL", tmp);
		undoManager->action(this, ss);
	}
	fillColorVal = tmp;
	if (GrType == 0)
	{
		fill_gradient = VGradient(VGradient::linear);
		fill_gradient.clearStops();
		if (fillColorVal != CommonStrings::None)
		{
			const ScColor& col = m_Doc->PageColors[fillColorVal];
			fill_gradient.addStop(ScColorEngine::getRGBColor(col, m_Doc), 0.0, 0.5, 1.0, fillColorVal, 100);
			fill_gradient.addStop(ScColorEngine::getRGBColor(col, m_Doc), 1.0, 0.5, 1.0, fillColorVal, 100);
		}
	}
	setFillQColor();
//CB unused in 135	emit colors(lineColorVal, fillColorVal, lineShadeVal, fillShadeVal);
}

void PageItem::setFillShade(double newShade)
{
	if (fillShadeVal == newShade)
	{
		setFillQColor();
		return;
	}
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
	setFillQColor();
//CB unused in 135	emit colors(lineColorVal, fillColorVal, lineShadeVal, fillShadeVal);
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

void PageItem::setFillBlendmode(int newBlendmode)
{
	if (fillBlendmodeVal == newBlendmode)
		return; // nothing to do -> return
	fillBlendmodeVal = newBlendmode;
}

void PageItem::setLineColor(const QString &newColor)
{
	QString tmp = newColor;
	if (tmp != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(newColor))
		{
			switch(itemType())
			{
				case TextFrame:
				case PathText:
					tmp = m_Doc->toolSettings.dTextLineColor;
					break;
				case Line:
					tmp = m_Doc->toolSettings.dPenLine;
					break;
				case PolyLine:
				case Polygon:
				case ImageFrame:
				case LatexFrame:
					tmp = m_Doc->toolSettings.dPen;
					break;
				default:
					break;
			}
		}
	}
	if (lineColorVal == tmp)
	{
		setLineQColor();
		return;
	}
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SetLineColor,
										  QString(Um::ColorFromTo).arg(lineColorVal).arg(tmp),
										  Um::IFill);
		ss->set("LINE_COLOR", "line_color");
		ss->set("OLD_COLOR", lineColorVal);
		ss->set("NEW_COLOR", tmp);
		undoManager->action(this, ss);
	}
	lineColorVal = tmp;
	setLineQColor();
//CB unused in 135	emit colors(lineColorVal, fillColorVal, lineShadeVal, fillShadeVal);
}

void PageItem::setLineShade(double newShade)
{
	if (lineShadeVal == newShade)
	{
		setLineQColor();
		return;
	}
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
	setLineQColor();
//CB unused in 135	emit colors(lineColorVal, fillColorVal, lineShadeVal, fillShadeVal);
}

void PageItem::setLineQColor()
{
	if (lineColorVal != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(lineColorVal))
		{
			switch(itemType())
			{
				case TextFrame:
				case PathText:
					lineColorVal = m_Doc->toolSettings.dTextLineColor;
					break;
				case Line:
					lineColorVal = m_Doc->toolSettings.dPenLine;
					break;
				case PolyLine:
				case Polygon:
				case ImageFrame:
				case LatexFrame:
					lineColorVal = m_Doc->toolSettings.dPen;
					break;
				default:
					break;
			}
		}
		if (!m_Doc->PageColors.contains(lineColorVal))
			lineColorVal = m_Doc->toolSettings.dPen;
		const ScColor& col = m_Doc->PageColors[lineColorVal];
		strokeQColor = ScColorEngine::getShadeColorProof(col, m_Doc, lineShadeVal);
	}
	if ((m_Doc->view()) && (m_Doc->view()->m_canvas->usePreviewVisual()))
	{
		VisionDefectColor defect;
		strokeQColor = defect.convertDefect(strokeQColor, m_Doc->view()->m_canvas->previewVisual());
	}
}

void PageItem::setFillQColor()
{
	if (fillColorVal != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(fillColorVal))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
					fillColorVal = m_Doc->toolSettings.dBrushPict;
				case TextFrame:
				case PathText:
					fillColorVal = m_Doc->toolSettings.dTextBackGround;
					break;
				case Line:
				case PolyLine:
				case Polygon:
					fillColorVal = m_Doc->toolSettings.dBrush;
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[fillColorVal];
		fillQColor = ScColorEngine::getShadeColorProof(col, m_Doc, fillShadeVal);
	}
	if ((m_Doc->view()) && (m_Doc->view()->m_canvas->usePreviewVisual()))
	{
		VisionDefectColor defect;
		fillQColor = defect.convertDefect(fillQColor, m_Doc->view()->m_canvas->previewVisual());
	}
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

void PageItem::setLineBlendmode(int newBlendmode)
{
	if (lineBlendmodeVal == newBlendmode)
		return; // nothing to do -> return
	lineBlendmodeVal = newBlendmode;
}

void PageItem::setLineStyle(Qt::PenStyle newStyle)
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

void PageItem::setLineWidth(double newWidth)
{
	if (m_lineWidth == newWidth)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LineWidth,
						QString(Um::FromTo).arg(m_lineWidth).arg(newWidth),Um::ILineStyle);
		ss->set("LINE_WIDTH", "line_width");
		ss->set("OLD_WIDTH", m_lineWidth);
		ss->set("NEW_WIDTH", newWidth);
		undoManager->action(this, ss);
	}
	Oldm_lineWidth=m_lineWidth;
	m_lineWidth = newWidth;
}

void PageItem::setLineEnd(Qt::PenCapStyle newStyle)
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

void PageItem::setLineJoin(Qt::PenJoinStyle newStyle)
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

void PageItem::setStartArrowIndex(int newIndex)
{
	if (m_startArrowIndex == newIndex)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::StartArrow,"",Um::IArrow);
		ss->set("START_ARROW", "startarrow");
		ss->set("OLD_INDEX", m_startArrowIndex);
		ss->set("NEW_INDEX", newIndex);
		undoManager->action(this, ss);
	}
	m_startArrowIndex = newIndex;
}

void PageItem::setEndArrowIndex(int newIndex)
{
	if (m_endArrowIndex == newIndex)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::EndArrow,"",Um::IArrow);
		ss->set("END_ARROW", "endarrow");
		ss->set("OLD_INDEX", m_endArrowIndex);
		ss->set("NEW_INDEX", newIndex);
		undoManager->action(this, ss);
	}
	m_endArrowIndex = newIndex;
}

void PageItem::setImageFlippedH(bool flipped)
{
	if (flipped != m_ImageIsFlippedH)
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
	m_ImageIsFlippedH = !m_ImageIsFlippedH;
	emit frameFlippedH(m_ImageIsFlippedH);
}

void PageItem::setImageFlippedV(bool flipped)
{
	if (flipped != m_ImageIsFlippedV)
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
	m_ImageIsFlippedV = !m_ImageIsFlippedV;
	emit frameFlippedV(m_ImageIsFlippedV);
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
	update();
}

void PageItem::toggleLock()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss;
		if (m_Locked)
			ss = new SimpleState(Um::UnLock, 0, Um::IUnLock);
		else
			ss = new SimpleState(Um::Lock, 0, Um::ILock);
		ss->set("LOCK", "lock");
		undoManager->action(this, ss);
	}
	m_Locked = !m_Locked;
	emit frameLocked(m_Locked);
}

void PageItem::setLocked(bool isLocked)
{
	if (isLocked != m_Locked)
		toggleLock();
}

void PageItem::toggleSizeLock()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss;
		if (m_SizeLocked)
			ss = new SimpleState(Um::SizeUnLock, 0, Um::IUnLock);
		else
			ss = new SimpleState(Um::SizeLock, 0, Um::ILock);
		ss->set("SIZE_LOCK", "size_lock");
		undoManager->action(this, ss);
	}
	m_SizeLocked = !m_SizeLocked;
	emit frameSizeLocked(m_SizeLocked);
}

void PageItem::setSizeLocked(bool isLocked)
{
	if (isLocked != m_SizeLocked)
		toggleSizeLock();
}


void PageItem::setPrintEnabled(bool toPrint)
{
	if (toPrint != m_PrintEnabled)
		togglePrintEnabled();
}

void PageItem::togglePrintEnabled()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss;
		if (m_PrintEnabled)
			ss = new SimpleState(Um::DisablePrint, 0, Um::IDisablePrint);
		else
			ss = new SimpleState(Um::EnablePrint, 0, Um::IEnablePrint);
		ss->set("PRINT_ENABLED", "print_enabled");
		undoManager->action(this, ss);
	}
	m_PrintEnabled=!m_PrintEnabled;
	emit printEnabled(m_PrintEnabled);
}

void PageItem::setTextFlowMode(TextFlowMode mode)
{
	if (textFlowModeVal == mode)
		return;
	if (UndoManager::undoEnabled())
	{
		QString stateMessage;
		if( mode == TextFlowUsesFrameShape )
			stateMessage = Um::ObjectFrame;
		else if( mode == TextFlowUsesBoundingBox )
			stateMessage = Um::BoundingBox;
		else if( mode == TextFlowUsesContourLine )
			stateMessage = Um::ContourLine;
		else if( mode == TextFlowUsesImageClipping )
			stateMessage = Um::ImageClip;
		else
			stateMessage = Um::NoTextFlow;
		SimpleState *ss = new SimpleState(stateMessage, "", Um::IFont);
		ss->set("TEXTFLOW_OLDMODE", (int) textFlowModeVal);
		ss->set("TEXTFLOW_NEWMODE", (int) mode);
		undoManager->action(this, ss);
	}
	textFlowModeVal = mode;
}

void PageItem::convertTo(ItemType newType)
{
	if (m_ItemType == newType)
		return; // nothing to do -> return
	assert(newType != 1);	//DEBUG CR 2005-02-06
	assert(newType != 3);	//DEBUG CR 2005-02-06
	QString fromType = "", toType = "";
	switch (m_ItemType)
	{
		case ImageFrame:
			if (asLatexFrame()) {
				fromType = Um::LatexFrame;
			} else {
				fromType = Um::ImageFrame;
			}
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
		case LatexFrame:
			toType = Um::LatexFrame;
			setUPixmap(Um::ILatexFrame);
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
		ss->set("OLD_TYPE", m_ItemType);
		ss->set("NEW_TYPE", newType);
		undoManager->action(this, ss);
	}
	*/
	m_ItemType = newType;
	emit frameType(m_ItemType);
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
	// has the item's image been moved
	if (force || ((oldLocalX != LocalX || oldLocalY != LocalY) && shouldCheck()))
		changeImageOffsetUndoAction();
	// has the item's image been scaled
	if (force || ((oldLocalScX != LocalScX || oldLocalScY != LocalScY) && shouldCheck()))
		changeImageScaleUndoAction();
}

bool PageItem::shouldCheck()
{
	return ((!m_Doc->view()->mousePressed()) &&
			(!ScCore->primaryMainWindow()->arrowKeyDown()) &&
			(!ScCore->primaryMainWindow()->propertiesPalette->userActionOn()));
}

void PageItem::moveUndoAction()
{
	if (oldXpos == Xpos && oldYpos == Ypos)
		return;
	if (UndoManager::undoEnabled())
	{
		QString oldp;
		QString newp;
		if (oldOwnPage == -1)
			oldp = Um::ScratchSpace;
		else
			oldp = QString(Um::PageNmbr).arg(m_Doc->FirstPnum + oldOwnPage);
		if (OwnPage == -1)
			newp = Um::ScratchSpace;
		else
			newp = QString(Um::PageNmbr).arg(m_Doc->FirstPnum + OwnPage);
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

void PageItem::changeImageOffsetUndoAction()
{
	if (oldLocalX == LocalX && oldLocalY == LocalY)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ImageOffset,
			QString(Um::ImageOffsetFromTo).arg(oldLocalX).arg(oldLocalY).arg(LocalX).arg(LocalY), Um::IMove);
		ss->set("OLD_IMAGEXOFFSET", oldLocalX);
		ss->set("OLD_IMAGEYOFFSET", oldLocalY);
		ss->set("NEW_IMAGEXOFFSET", LocalX);
		ss->set("NEW_IMAGEYOFFSET", LocalY);
		undoManager->action(this, ss);
	}
	oldLocalX = LocalX;
	oldLocalY = LocalY;
}

void PageItem::changeImageScaleUndoAction()
{
	if (oldLocalScX == LocalScX && oldLocalScY == LocalScY)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ImageScale,
			QString(Um::ImageScaleFromTo).arg(oldLocalScX).arg(oldLocalScY).arg(LocalScX).arg(LocalScY), Um::IMove);
		ss->set("OLD_IMAGEXSCALE", oldLocalScX);
		ss->set("OLD_IMAGEYSCALE", oldLocalScY);
		ss->set("NEW_IMAGEXSCALE", LocalScX);
		ss->set("NEW_IMAGEYSCALE", LocalScY);
		undoManager->action(this, ss);
	}
	oldLocalScX = LocalScX;
	oldLocalScY = LocalScY;
}

void PageItem::restore(UndoState *state, bool isUndo)
{
	bool useRasterBackup = m_Doc->useRaster;
	bool SnapGuidesBackup = m_Doc->SnapGuides;
	m_Doc->useRaster = false;
	m_Doc->SnapGuides = false;
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	bool oldMPMode=m_Doc->masterPageMode();
	m_Doc->setMasterPageMode(!OnMasterPage.isEmpty());
	Page *oldCurrentPage = m_Doc->currentPage();
	if (!OnMasterPage.isEmpty())
	{
		oldCurrentPage = m_Doc->currentPage();
		m_Doc->setCurrentPage(m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]));
	}
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
			m_Doc->itemSelection_FlipH();
		}
		else if (ss->contains("IMAGEFLIPV"))
		{
			select();
			m_Doc->itemSelection_FlipV();
		}
		else if (ss->contains("LOCK"))
		{
			select();
			m_Doc->itemSelection_ToggleLock();
		}
		else if (ss->contains("SIZE_LOCK"))
		{
			select();
			m_Doc->itemSelection_ToggleSizeLock();
		}
		else if (ss->contains("PRINT_ENABLED"))
		{
			select();
			m_Doc->itemSelection_TogglePrintEnabled();
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
		else if (ss->contains("PSTYLE"))
			restorePStyle(ss, isUndo);
		else if (ss->contains("CONVERT"))
			restoreType(ss, isUndo);
		else if (ss->contains("TEXTFLOW_OLDMODE"))
			restoreTextFlowing(ss, isUndo);
		else if (ss->contains("SCALE_TYPE"))
			restoreImageScaleType(ss, isUndo);
		else if (ss->contains("OLD_IMAGEXSCALE"))
			restoreImageScaleChange(ss, isUndo);
		else if (ss->contains("OLD_IMAGEXOFFSET"))
			restoreImageOffsetChange(ss, isUndo);	
		else if (ss->contains("ASPECT_RATIO"))
			restoreImageScaleType(ss, isUndo);
		else if (ss->contains("EDIT_CONTOUR"))
			restorePoly(ss, isUndo, true);
		else if (ss->contains("EDIT_SHAPE"))
			restorePoly(ss, isUndo, false);
		else if (ss->contains("RESET_CONTOUR"))
			restoreContourLine(ss, isUndo);
		else if (ss->contains("MIRROR_PATH_H"))
		{
			bool editContour = m_Doc->nodeEdit.isContourLine;
			m_Doc->nodeEdit.isContourLine = ss->getBool("IS_CONTOUR");
			select();
			m_Doc->MirrorPolyH(m_Doc->m_Selection->itemAt(0));
			m_Doc->nodeEdit.isContourLine = editContour;
		}
		else if (ss->contains("MIRROR_PATH_V"))
		{
			bool editContour = m_Doc->nodeEdit.isContourLine;
			m_Doc->nodeEdit.isContourLine = ss->getBool("IS_CONTOUR");
			select();
			m_Doc->MirrorPolyV(m_Doc->m_Selection->itemAt(0));
			m_Doc->nodeEdit.isContourLine = editContour;
		}
		else if (ss->contains("SEND_TO_LAYER"))
			restoreLayer(ss, isUndo);
		else if (ss->contains("GET_IMAGE"))
			restoreGetImage(ss, isUndo);
		else if (ss->contains("EDIT_SHAPE_OR_CONTOUR"))
			restoreShapeContour(ss, isUndo);
		else if (ss->contains("APPLY_IMAGE_EFFECTS"))
			restoreImageEffects(ss, isUndo);
	}
	if (!OnMasterPage.isEmpty())
		m_Doc->setCurrentPage(oldCurrentPage);
	m_Doc->setMasterPageMode(oldMPMode);
	m_Doc->useRaster = useRasterBackup;
	m_Doc->SnapGuides = SnapGuidesBackup;
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
	m_Doc->MoveItem(mx, my, this, false);
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
		m_Doc->SizeItem(ow, oh, this, false, true, true);
		m_Doc->MoveItem(mx, my, this, false);
		m_Doc->RotateItem(ort, this);
	}
	else
	{
		mx = -mx;
		my = -my;
		m_Doc->SizeItem(w, h, this, false, true, true);
		m_Doc->MoveItem(mx, my, this, false);
		m_Doc->RotateItem(rt, this);
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
	//CB Commented out test code
	//QRect oldR(getRedrawBounding(view->scale()));
	if (isUndo)
	{
		/*Rot=ort;
		Xpos+=mx;
		Ypos+=my;
		Width=ow;
		Height=oh;*/
		m_Doc->RotateItem(ort, this);
		m_Doc->MoveItem(mx, my, this, false);
		m_Doc->SizeItem(ow, oh, this, false, true, true);
	}
	else
	{
		mx = -mx;
		my = -my;
		/*
		Rot=rt;
		Xpos-=mx;
		Ypos-=my;
		Width=w;
		Height=h;
		*/
		m_Doc->RotateItem(rt, this);
		m_Doc->MoveItem(mx, my, this, false);
		m_Doc->SizeItem(w, h, this, false, true, true);
	}
	/*
	m_Doc->setRedrawBounding(this);
	QRect newR(getRedrawBounding(view->scale()));
	view->updateContents(newR.unite(oldR));
	OwnPage = m_Doc->OnPage(this);
	*/
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
	m_Doc->itemSelection_SetItemBrush(fill);
}

void PageItem::restoreShade(SimpleState *state, bool isUndo)
{
	int shade = state->getInt("OLD_SHADE");
	if (!isUndo)
		shade = state->getInt("NEW_SHADE");
	select();
	m_Doc->itemSelection_SetItemBrushShade(shade);
}

void PageItem::restoreLineColor(SimpleState *state, bool isUndo)
{
	QString fill = state->get("OLD_COLOR");
	if (!isUndo)
		fill = state->get("NEW_COLOR");
	select();
	m_Doc->itemSelection_SetItemPen(fill);
}

void PageItem::restoreLineShade(SimpleState *state, bool isUndo)
{
	int shade = state->getInt("OLD_SHADE");
	if (!isUndo)
		shade = state->getInt("NEW_SHADE");
	select();
	m_Doc->itemSelection_SetItemPenShade(shade);
}

void PageItem::restoreFillTP(SimpleState *state, bool isUndo)
{
	double tp = state->getDouble("OLD_TP");
	if (!isUndo)
		tp = state->getDouble("NEW_TP");
	select();
	m_Doc->itemSelection_SetItemFillTransparency(tp);
}

void PageItem::restoreLineTP(SimpleState *state, bool isUndo)
{
	double tp = state->getDouble("OLD_TP");
	if (!isUndo)
		tp = state->getDouble("NEW_TP");
	select();
	m_Doc->itemSelection_SetItemLineTransparency(tp);
}


void PageItem::restoreLineStyle(SimpleState *state, bool isUndo)
{
	Qt::PenStyle ps = static_cast<Qt::PenStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		ps = static_cast<Qt::PenStyle>(state->getInt("NEW_STYLE"));
	select();
	m_Doc->itemSelection_SetLineArt(ps);
}

void PageItem::restoreLineEnd(SimpleState *state, bool isUndo)
{
	Qt::PenCapStyle pcs = static_cast<Qt::PenCapStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		pcs = static_cast<Qt::PenCapStyle>(state->getInt("NEW_STYLE"));
	select();
	m_Doc->itemSelection_SetLineEnd(pcs);
}

void PageItem::restoreLineJoin(SimpleState *state, bool isUndo)
{
	Qt::PenJoinStyle pjs = static_cast<Qt::PenJoinStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		pjs = static_cast<Qt::PenJoinStyle>(state->getInt("NEW_STYLE"));
	select();
	m_Doc->itemSelection_SetLineJoin(pjs);
}

void PageItem::restoreLineWidth(SimpleState *state, bool isUndo)
{
	double w = state->getDouble("OLD_WIDTH");
	if (!isUndo)
		w = state->getDouble("NEW_WIDTH");
	select();
	m_Doc->itemSelection_SetLineWidth(w);
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


void PageItem::restorePStyle(SimpleState *state, bool isUndo)
{
	int styleid = state->getInt("OLD_STYLE");
	if (!isUndo)
		styleid = state->getInt("NEW_STYLE");
	//will be done later with other text-undo:
	//	m_Doc->chAbStyle(this, styleid);
}


// FIXME: This must go into class ScribusDoc!
// For now we'll just make it independent of 'this' -- AV
void PageItem::restoreType(SimpleState *state, bool isUndo)
{
	// well, probably not the best way to handle pointers...
	PageItem * item = reinterpret_cast<PageItem *>(state->getInt("PAGEITEM"));
	int type = state->getInt("OLD_TYPE");
	if (!isUndo)
		type = state->getInt("NEW_TYPE");
	ScribusView* view = m_Doc->view();
	view->Deselect(false);
	view->SelectItem(item, false);
	switch (type) {
		case ImageFrame: view->ToPicFrame(); break;
		case TextFrame: view->ToTextFrame(); break;
		case Polygon: view->ToPolyFrame(); break;
		case PolyLine: view->ToBezierFrame(); break;
	}
	m_Doc->scMW()->setAppMode(modeNormal);
}

void PageItem::restoreTextFlowing(SimpleState *state, bool isUndo)
{
	TextFlowMode oldMode = (TextFlowMode) state->getInt("TEXTFLOW_OLDMODE");
	TextFlowMode newMode = (TextFlowMode) state->getInt("TEXTFLOW_NEWMODE");
	if (isUndo)
		textFlowModeVal = oldMode;
	else
		textFlowModeVal = newMode;
}

void PageItem::restoreImageScaleType(SimpleState *state, bool isUndo)
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

void PageItem::restoreImageScaleChange(SimpleState *state, bool isUndo)
{
	double oscx = state->getDouble("OLD_IMAGEXSCALE");
	double oscy = state->getDouble("OLD_IMAGEYSCALE");
	double  scx = state->getDouble("NEW_IMAGEXSCALE");
	double  scy = state->getDouble("NEW_IMAGEYSCALE");
	Selection tempSelection(this, false);
	tempSelection.addItem(this, true);
	if (!isUndo)
		m_Doc->itemSelection_SetImageScale(scx, scy, &tempSelection);
	else
		m_Doc->itemSelection_SetImageScale(oscx, oscy, &tempSelection);
}

void PageItem::restoreImageOffsetChange(SimpleState *state, bool isUndo)
{
	double ox = state->getDouble("OLD_IMAGEXOFFSET");
	double oy = state->getDouble("OLD_IMAGEYOFFSET");
	double  x = state->getDouble("NEW_IMAGEXOFFSET");
	double  y = state->getDouble("NEW_IMAGEYOFFSET");
	Selection tempSelection(this, false);
	tempSelection.addItem(this, true);
	if (!isUndo)
		m_Doc->itemSelection_SetImageOffset(x, y, &tempSelection);
	else
		m_Doc->itemSelection_SetImageOffset(ox, oy, &tempSelection);
}

void PageItem::restorePoly(SimpleState *state, bool isUndo, bool isContour)
{
	int mode    = state->getInt("MODE");
	int rot     = state->getInt("ROT");
	ScribusView* view = m_Doc->view();
	double scaling = state->getDouble("SCALING");
	bool editContour = m_Doc->nodeEdit.isContourLine;
	m_Doc->nodeEdit.isContourLine = isContour;
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
	view->TransformPoly(mode, rot, scaling);
	m_Doc->nodeEdit.isContourLine = editContour;
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
	ScribusView* view = m_Doc->view();
	setLayer(isUndo ? state->getInt("OLD_LAYER") : state->getInt("NEW_LAYER"));
	view->Deselect(true);
	m_Doc->regionsChanged()->update(QRectF());
}

void PageItem::restoreGetImage(SimpleState *state, bool isUndo)
{
	QString fn = state->get("OLD_IMAGE_PATH");
	if (!isUndo)
		fn = state->get("NEW_IMAGE_PATH");
	if (fn.isEmpty())
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(this, true);
		m_Doc->itemSelection_ClearItem(&tempSelection);
	}
	else
		loadImage(fn, false);
}

void PageItem::restoreShapeContour(UndoState *state, bool isUndo)
{
	ItemState<QPair<FPointArray,FPointArray> > *istate =
			dynamic_cast<ItemState<QPair<FPointArray,FPointArray> >*>(state);
	if (istate)
	{
		FPointArray oldClip = istate->getItem().first;
		FPointArray newClip = istate->getItem().second;
		bool isContour = istate->getBool("IS_CONTOUR");
		double oldX = istate->getDouble("OLD_X");
		double oldY = istate->getDouble("OLD_Y");
		double newX = istate->getDouble("NEW_X");
		double newY = istate->getDouble("NEW_Y");
		double mx = oldX - newX;
		double my = oldY - newY;

		if (isUndo)
		{
			if (isContour)
				ContourLine = oldClip;
			else
				PoLine = oldClip;
		}
		else
		{
			mx = -mx;
			my = -my;
			if (isContour)
				ContourLine = newClip;
			else
				PoLine = newClip;
		}
		m_Doc->AdjustItemSize(this);
		m_Doc->MoveItem(mx, my, this, false);
		m_Doc->regionsChanged()->update(QRectF());
	}

}

void PageItem::restoreImageEffects(UndoState *state, bool isUndo)
{
	ItemState<QPair<ScImageEffectList, ScImageEffectList> > *istate =
	dynamic_cast<ItemState<QPair<ScImageEffectList,ScImageEffectList> >*>(state);
	if (istate)
	{
		if (isUndo)
			effectsInUse = istate->getItem().first;
		else
			effectsInUse = istate->getItem().second;

		select();
		m_Doc->updatePic();
	}
}

void PageItem::select()
{
	m_Doc->view()->Deselect(false);
	//CB #2969 add this true parm to addItem so we dont connectToGUI, the rest of view->SelectItem isnt needed anyway
	m_Doc->m_Selection->addItem(this, true);
}

ObjAttrVector* PageItem::getObjectAttributes()
{
	return &pageItemAttributes;
}

ObjectAttribute PageItem::getObjectAttribute(QString attributeName) const
{
	int countFound=0;
	ObjAttrVector::const_iterator foundIt = pageItemAttributes.begin();
	for(ObjAttrVector::const_iterator objAttrIt = pageItemAttributes.begin() ; objAttrIt != pageItemAttributes.end(); ++objAttrIt )
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
	if (!m_Doc->itemNameExists(originalName))
		return originalName;

	// Start embellishing the name until we get an acceptable unique name
	// first we prefix `Copy of' if it's not already there
	QString newname(originalName);
	if (!originalName.startsWith( tr("Copy of")))
		newname.prepend( tr("Copy of")+" ");

	// See if the name prefixed by "Copy of " is free
	if (m_Doc->itemNameExists(newname))
	{
		// Search the string for (number) at the end and capture
		// both the number and the text leading up to it sans brackets.
		//     Copy of fred (5)
		//     ^^^^^^^^^^^^  ^   (where ^ means captured)
		static QRegExp rx("^(.*)\\s+\\((\\d+)\\)$");
		int numMatches = rx.lastIndexIn(newname);
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
		while (m_Doc->itemNameExists(newname));
	}
	// Unnecessary assert, previous code ensure condition is always true - JG
	// assert(!m_Doc->itemNameExists(newname));
	return newname;
}

void PageItem::setTagged(bool tag)
{
	tagged=tag;
}

void PageItem::replaceNamedResources(ResourceCollection& newNames) 
{
	QMap<QString,QString>::ConstIterator it;

	it = newNames.colors().find(fillColor());
	if (it != newNames.colors().end())
		setFillColor(*it);

	it = newNames.colors().find(lineColor());
	if (it != newNames.colors().end())
		setLineColor(*it);

	QList<VColorStop*> cstops = fill_gradient.colorStops();
	for (uint cst = 0; cst < fill_gradient.Stops(); ++cst)
	{
		it = newNames.colors().find(cstops.at(cst)->name);
		if (it != newNames.colors().end())
		{
			if (*it != CommonStrings::None)
				cstops.at(cst)->name = *it;
		}
	}
	if (effectsInUse.count() != 0)
	{
		QString col1 = CommonStrings::None;
		QString col2 = CommonStrings::None;
		QString col3 = CommonStrings::None;
		QString col4 = CommonStrings::None;
		for (int a = 0; a < effectsInUse.count(); ++a)
		{
			QString tmpstr = effectsInUse.at(a).effectParameters;
			QString tmpstr2 = "";
			QTextStream fp(&tmpstr, QIODevice::ReadOnly);
			switch (effectsInUse.at(a).effectCode)
			{
				case ScImage::EF_QUADTONE:
					col1 = fp.readLine();
					col2 = fp.readLine();
					col3 = fp.readLine();
					col4 = fp.readLine();
					it = newNames.colors().find(col1);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col1 + "\n";
					}
					else
						tmpstr2 += col1 + "\n";
					it = newNames.colors().find(col2);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col2 + "\n";
					}
					else
						tmpstr2 += col2 + "\n";
					it = newNames.colors().find(col3);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col3 + "\n";
					}
					else
						tmpstr2 += col3 + "\n";
					it = newNames.colors().find(col4);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col4 + "\n";
					}
					else
						tmpstr2 += col4 + "\n";
					tmpstr2 += fp.readAll();
					break;
				case ScImage::EF_TRITONE:
					col1 = fp.readLine();
					col2 = fp.readLine();
					col3 = fp.readLine();
					it = newNames.colors().find(col1);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col1 + "\n";
					}
					else
						tmpstr2 += col1 + "\n";
					it = newNames.colors().find(col2);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col2 + "\n";
					}
					else
						tmpstr2 += col2 + "\n";
					it = newNames.colors().find(col3);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col3 + "\n";
					}
					else
						tmpstr2 += col3 + "\n";
					tmpstr2 += fp.readAll();
					break;
				case ScImage::EF_DUOTONE:
					col1 = fp.readLine();
					col2 = fp.readLine();
					it = newNames.colors().find(col1);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col1 + "\n";
					}
					else
						tmpstr2 += col1 + "\n";
					it = newNames.colors().find(col2);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n";
						else
							tmpstr2 += col2 + "\n";
					}
					else
						tmpstr2 += col2 + "\n";
					tmpstr2 += fp.readAll();
					break;
				case ScImage::EF_COLORIZE:
					col1 = fp.readLine();
					it = newNames.colors().find(col1);
					if (it != newNames.colors().end())
					{
						if (*it != CommonStrings::None)
							tmpstr2 += *it + "\n" + fp.readAll();
						else
							tmpstr2 += col1 + "\n" + fp.readAll();
					}
					else
						tmpstr2 += col1 + "\n" + fp.readAll();
					break;
				default:
					tmpstr2 = tmpstr;
					break;
			}
			effectsInUse[a].effectParameters = tmpstr2;
		}
	}
	
	it = newNames.patterns().find(pattern());
	if (it != newNames.patterns().end())
		setPattern(*it);

	it = newNames.lineStyles().find(customLineStyle());
	if (it != newNames.lineStyles().end())
		setCustomLineStyle(*it);

	if (prevInChain() == NULL)
		itemText.replaceNamedResources(newNames);
}


void PageItem::getNamedResources(ResourceCollection& lists) const
{
	lists.collectColor(lineColor());
	if (GrType == 0)
		lists.collectColor(fillColor());
	else if (GrType < 8)
	{
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		for (uint cst = 0; cst < fill_gradient.Stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
	if (effectsInUse.count() != 0)
	{
		for (int a = 0; a < effectsInUse.count(); ++a)
		{
			QString tmpstr = effectsInUse.at(a).effectParameters;
			QTextStream fp(&tmpstr, QIODevice::ReadOnly);
			switch (effectsInUse.at(a).effectCode)
			{
				case ScImage::EF_QUADTONE:
					lists.collectColor(fp.readLine());
				case ScImage::EF_TRITONE:
					lists.collectColor(fp.readLine());
				case ScImage::EF_DUOTONE:
					lists.collectColor(fp.readLine());
				case ScImage::EF_COLORIZE:
					lists.collectColor(fp.readLine());
					break;
				default:
					break;
			}
		}
	}
	lists.collectPattern(pattern());
	lists.collectLineStyle(customLineStyle());
	if (prevInChain() == NULL)
		itemText.getNamedResources(lists);
}

void PageItem::copyToCopyPasteBuffer(struct CopyPasteBuffer *Buffer)
{
	Buffer->PType = realItemType();
	Buffer->Xpos = Xpos;
	Buffer->Ypos = Ypos;
	Buffer->Width = Width;
	Buffer->Height = Height;
	Buffer->RadRect = RadRect;
	Buffer->FrameType = FrameType;
	Buffer->ClipEdited = ClipEdited;
	Buffer->Pwidth = m_lineWidth;
	Buffer->Pcolor = fillColor();
	Buffer->Pcolor2 = lineColor();
	Buffer->Shade = fillShade();
	Buffer->Shade2 = lineShade();
	Buffer->FillRule = fillRule;
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
	Buffer->Rot = Rot;
	Buffer->PLineArt = PLineArt;
	Buffer->PLineEnd = PLineEnd;
	Buffer->PLineJoin = PLineJoin;
	Buffer->LocalScX = LocalScX;
	Buffer->LocalScY = LocalScY;
	Buffer->LocalX = LocalX;
	Buffer->LocalY = LocalY;
	Buffer->PicArt = PicArt;
	Buffer->flippedH = imageFlippedH();
	Buffer->flippedV = imageFlippedV();
/*	Buffer->BBoxX = BBoxX;
	Buffer->BBoxH = BBoxH; */
	Buffer->isPrintable = printEnabled();
	Buffer->isBookmark = isBookmark;
//	Buffer->BMnr = BMnr;
	Buffer->m_isAnnotation = m_isAnnotation;
	Buffer->m_annotation = m_annotation;
	Buffer->Extra = Extra;
	Buffer->TExtra = TExtra;
	Buffer->BExtra = BExtra;
	Buffer->RExtra = RExtra;
	Buffer->firstLineOffsetP = firstLineOffsetP;
	Buffer->Pfile = Pfile;
	Buffer->Pfile2 = Pfile2;
	Buffer->Pfile3 = Pfile3;
	QString Text = "";
	uint itemTextCount=itemText.length();
	if (itemTextCount != 0)
	{
		for (uint a=0; a<itemTextCount; ++a)
		{
			if( (itemText.text(a) == '\n') || (itemText.text(a) == '\r'))
				Text += QString(QChar(5))+"\t";
			else if(itemText.text(a) == '\t')
				Text += QString(QChar(4))+"\t";
			else
				Text += itemText.text(a,1)+"\t";
			Text += itemText.charStyle(a).font().scName()+"\t";
			Text += QString::number(itemText.charStyle(a).fontSize() / 10.0)+"\t";
			Text += itemText.charStyle(a).fillColor()+"\t";
			Text += QString::number(itemText.charStyle(a).tracking())+"\t";
			Text += QString::number(itemText.charStyle(a).fillShade())+'\t';
			Text += QString::number(itemText.charStyle(a).effects())+'\t';
			Text += QString::number(findParagraphStyle(m_Doc, itemText.paragraphStyle(a)))+'\t';
			Text += itemText.charStyle(a).strokeColor()+"\t";
			Text += QString::number(itemText.charStyle(a).strokeShade())+'\t';
			Text += QString::number(itemText.charStyle(a).scaleH())+'\t';
			Text += QString::number(itemText.charStyle(a).scaleV())+'\t';
			Text += QString::number(itemText.charStyle(a).baselineOffset())+'\t';
			Text += QString::number(itemText.charStyle(a).shadowXOffset())+'\t';
			Text += QString::number(itemText.charStyle(a).shadowYOffset())+'\t';
			Text += QString::number(itemText.charStyle(a).outlineWidth())+'\t';
			Text += QString::number(itemText.charStyle(a).underlineOffset())+'\t';
			Text += QString::number(itemText.charStyle(a).underlineWidth())+'\t';
			Text += QString::number(itemText.charStyle(a).strikethruOffset())+'\t';
			Text += QString::number(itemText.charStyle(a).strikethruWidth())+'\n';
		}
	}
	if (asLatexFrame()) {
		Buffer->itemText = asLatexFrame()->formula();
	} else {
		Buffer->itemText = Text;
	}
	Buffer->Clip = Clip;
	Buffer->PoLine = PoLine.copy();
	Buffer->ContourLine = ContourLine.copy();
	//Buffer->UseContour = textFlowUsesContourLine();
	Buffer->DashValues = DashValues;
	Buffer->DashOffset = DashOffset;
	Buffer->PoShow = PoShow;
	Buffer->BaseOffs = BaseOffs;
	Buffer->textPathFlipped = textPathFlipped;
	Buffer->textPathType = textPathType;
	Buffer->TextflowMode = textFlowMode();
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
	Buffer->TransBlend = fillBlendmode();
	Buffer->TransBlendS = lineBlendmode();
	Buffer->Reverse = Reverse;
	Buffer->NamedLStyle = NamedLStyle;
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
	Buffer->startArrowIndex = m_startArrowIndex;
	Buffer->endArrowIndex = m_endArrowIndex;
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
	setCornerRadius(qMin(RadRect, qMin(Width,Height)/2));
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



void PageItem::getTransform(QMatrix& mat) const
{
	mat.translate(Xpos, Ypos);
	mat.rotate(Rot);
}

QMatrix PageItem::getTransform() const
{
	QMatrix result;
	getTransform(result);
	return result;
}

QRectF PageItem::getBoundingRect() const
{
	double x,y,x2,y2;
	getBoundingRect(&x, &y, &x2, &y2);
	return QRectF(x,y,x2-x,y2-y);
}

QRectF PageItem::getVisualBoundingRect() const
{
	double x,y,x2,y2;
	getVisualBoundingRect(&x, &y, &x2, &y2);
	return QRectF(x,y,x2-x,y2-y);
}

void PageItem::getBoundingRect(double *x1, double *y1, double *x2, double *y2) const
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
			minx = qMin(minx, pb.point(pc).x());
			miny = qMin(miny, pb.point(pc).y());
			maxx = qMax(maxx, pb.point(pc).x());
			maxy = qMax(maxy, pb.point(pc).y());
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
		*x2 = Xpos + qMax(Width, m_lineWidth);
		*y2 = Ypos + qMax(Height, m_lineWidth);
	}
	QRectF totalRect = QRectF(QPointF(*x1, *y1), QPointF(*x2, *y2));
	if (m_startArrowIndex != 0)
	{
		QMatrix arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles.at(m_startArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(0, 0);
			arrowTrans.scale(m_lineWidth, m_lineWidth);
			arrowTrans.scale(-1,1);
			arrow.map(arrowTrans);
		}
		else
		{
			FPoint Start = PoLine.point(0);
			for (uint xx = 1; xx < PoLine.size(); xx += 2)
			{
				FPoint Vector = PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI));
					arrowTrans.scale(m_lineWidth, m_lineWidth);
					arrow.map(arrowTrans);
					break;
				}
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		totalRect = totalRect.united(QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y())));
	}
	if (m_endArrowIndex != 0)
	{
		QMatrix arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles.at(m_endArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(Width, 0);
			arrowTrans.scale(m_lineWidth, m_lineWidth);
			arrow.map(arrowTrans);
		}
		else
		{
			FPoint End = PoLine.point(PoLine.size()-2);
			for (uint xx = PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI));
					arrowTrans.scale(m_lineWidth, m_lineWidth);
					arrow.map(arrowTrans);
					break;
				}
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		totalRect = totalRect.united(QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y())));
	}
	totalRect.getCoords(x1, y1, x2, y2);
}

void PageItem::getVisualBoundingRect(double * x1, double * y1, double * x2, double * y2) const
{
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	double extraSpace = m_lineWidth / 2.0;
	if (Rot != 0)
	{
		FPointArray pb;
		pb.resize(0);
		pb.addPoint(FPoint(-extraSpace,			-extraSpace,			xPos(), yPos(), Rot, 1.0, 1.0));
		pb.addPoint(FPoint(visualWidth()-extraSpace,	-extraSpace,			xPos(), yPos(), Rot, 1.0, 1.0));
		pb.addPoint(FPoint(visualWidth()-extraSpace,	visualHeight()-extraSpace,	xPos(), yPos(), Rot, 1.0, 1.0));
		pb.addPoint(FPoint(-extraSpace,			visualHeight()-extraSpace,	xPos(), yPos(), Rot, 1.0, 1.0));
		for (uint pc = 0; pc < 4; ++pc)
		{
			minx = qMin(minx, pb.point(pc).x());
			miny = qMin(miny, pb.point(pc).y());
			maxx = qMax(maxx, pb.point(pc).x());
			maxy = qMax(maxy, pb.point(pc).y());
		}
		*x1 = minx;
		*y1 = miny;
		*x2 = maxx;
		*y2 = maxy;
	}
	else
	{
		*x1 = visualXPos();
		*y1 = visualYPos();
		*x2 = *x1 + qMax(visualWidth(), m_lineWidth);
		*y2 = *y1 + qMax(visualHeight(), m_lineWidth);
	}
	QRectF totalRect = QRectF(QPointF(*x1, *y1), QPointF(*x2, *y2));
	if (m_startArrowIndex != 0)
	{
		QMatrix arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles.at(m_startArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(0, 0);
			arrowTrans.scale(m_lineWidth, m_lineWidth);
			arrowTrans.scale(-1,1);
			arrow.map(arrowTrans);
		}
		else
		{
			FPoint Start = PoLine.point(0);
			for (uint xx = 1; xx < PoLine.size(); xx += 2)
			{
				FPoint Vector = PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI));
					arrowTrans.scale(m_lineWidth, m_lineWidth);
					arrow.map(arrowTrans);
					break;
				}
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		totalRect = totalRect.united(QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y())));
	}
	if (m_endArrowIndex != 0)
	{
		QMatrix arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles.at(m_endArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(Width, 0);
			arrowTrans.scale(m_lineWidth, m_lineWidth);
			arrow.map(arrowTrans);
		}
		else
		{
			FPoint End = PoLine.point(PoLine.size()-2);
			for (uint xx = PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI));
					arrowTrans.scale(m_lineWidth, m_lineWidth);
					arrow.map(arrowTrans);
					break;
				}
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		totalRect = totalRect.united(QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y())));
	}
	totalRect.getCoords(x1, y1, x2, y2);
}

double PageItem::visualXPos() const
{ 
	return (lineColor() != CommonStrings::None) ? Xpos - (m_lineWidth / 2.0) : Xpos;
}

double PageItem::visualYPos() const
{
	return (lineColor() != CommonStrings::None) ? Ypos - (m_lineWidth / 2.0) : Ypos;
}

double PageItem::visualWidth() const
{
	return (lineColor() != CommonStrings::None) ? Width + m_lineWidth : Width;
}

double PageItem::visualHeight() const
{
	return (lineColor() != CommonStrings::None) ? Height + m_lineWidth : Height;
}

bool PageItem::pointWithinItem(const int x, const int y) const
{
	const_cast<PageItem*>(this)-> setRedrawBounding();
	// FIXME: We should be rounding or truncating here, not letting the compiler do it.
	// Should we be rounding, truncating up, or truncating down?
	//CB is this now correct?
	QRect itemRect( static_cast<int>(floor(BoundingX)), static_cast<int>(floor(BoundingY)), static_cast<int>(ceil(BoundingW)), static_cast<int>(ceil(BoundingH)) );
	return itemRect.contains(x, y);
}

bool PageItem::mouseWithinItem(const int x, const int y, double scale) const
{
	QMatrix p;
	QRectF transRect;
	p.translate(Xpos * scale, Ypos*scale);
	p.scale(scale, scale);
	p.rotate(rotation());
	transRect = p.mapRect(QRectF(0.0, 0.0, width(), height()));
	return transRect.contains(x, y);
}

bool PageItem::loadImage(const QString& filename, const bool reload, const int gsResolution, bool showMsg)
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
	int lowResTypeBack = pixm.imgInfo.lowResType;
	int gsRes=gsResolution;
	if (gsResolution==-1) //If it wasn't supplied, get it from PrefsManager.
		gsRes=PrefsManager::instance()->gsResolution();
	bool dummy;
	CMSettings cms(m_Doc, IProfile, IRender);
	if (!pixm.LoadPicture(filename, pixm.imgInfo.actualPageNumber, cms, UseEmbedded, true, ScImage::RGBProof, gsRes, &dummy, showMsg))
	{
		Pfile = fi.absoluteFilePath();
		PicAvail = false;
//		PicArt = false;
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
//		PicArt = true;
		
		if (Pfile != filename)
		{
			oldLocalScX = LocalScX = 72.0 / xres;
			oldLocalScY = LocalScY = 72.0 / yres;
			oldLocalX = LocalX = 0;
			oldLocalY = LocalY = 0;
			
			if ((m_Doc->toolSettings.useEmbeddedPath) && (!pixm.imgInfo.clipPath.isEmpty()))
			{
				pixm.imgInfo.usedPath = pixm.imgInfo.clipPath;
				clPath = pixm.imgInfo.clipPath;
				if (pixm.imgInfo.PDSpathData.contains(clPath))
				{
					imageClip = pixm.imgInfo.PDSpathData[clPath].copy();
					pixm.imgInfo.usedPath = clPath;
					QMatrix cl;
					cl.translate(LocalX*LocalScX, LocalY*LocalScY);
					cl.scale(LocalScX, LocalScY);
					imageClip.map(cl);
				}
			}
		}
		
		Pfile = fi.absoluteFilePath();
		if (reload && pixm.imgInfo.PDSpathData.contains(clPath))
		{
			imageClip = pixm.imgInfo.PDSpathData[clPath].copy();
			pixm.imgInfo.usedPath = clPath;
			QMatrix cl;
			cl.translate(LocalX*LocalScX, LocalY*LocalScY);
			cl.scale(LocalScX, LocalScY);
			imageClip.map(cl);
		}
		BBoxX = pixm.imgInfo.BBoxX;
		BBoxH = pixm.imgInfo.BBoxH;
		OrigW = pixm.width();
		OrigH = pixm.height();
		QString ext = fi.suffix().toLower();
		isRaster = !(extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext));
		if (!isRaster)
			effectsInUse.clear();
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
		if ((pixm.imgInfo.colorspace == ColorSpaceDuotone) && (pixm.imgInfo.duotoneColors.count() != 0) && (!reload))
		{
			QString efVal = "";
			for (int cc = 0; cc < pixm.imgInfo.duotoneColors.count(); cc++)
			{
				if (!m_Doc->PageColors.contains(pixm.imgInfo.duotoneColors[cc].Name))
					m_Doc->PageColors.insert(pixm.imgInfo.duotoneColors[cc].Name, pixm.imgInfo.duotoneColors[cc].Color);
				efVal += pixm.imgInfo.duotoneColors[cc].Name+"\n";
			}
			m_Doc->scMW()->propertiesPalette->updateColorList();
			struct ImageEffect ef;
			if (pixm.imgInfo.duotoneColors.count() == 1)
			{
				efVal += "100";
				ef.effectCode = ScImage::EF_COLORIZE;
				ef.effectParameters = efVal;
			}
			else if (pixm.imgInfo.duotoneColors.count() == 2)
			{
				efVal += "100 100";
				QString tmp;
				FPointArray Vals = pixm.imgInfo.duotoneColors[0].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[1].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				ef.effectCode = ScImage::EF_DUOTONE;
				ef.effectParameters = efVal;
			}
			else if (pixm.imgInfo.duotoneColors.count() == 3)
			{
				efVal += "100 100 100";
				QString tmp;
				FPointArray Vals = pixm.imgInfo.duotoneColors[0].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[1].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[2].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				ef.effectCode = ScImage::EF_TRITONE;
				ef.effectParameters = efVal;
			}
			else if (pixm.imgInfo.duotoneColors.count() == 4)
			{
				efVal += "100 100 100 100";
				QString tmp;
				FPointArray Vals = pixm.imgInfo.duotoneColors[0].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[1].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[2].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[3].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (uint p = 0; p < Vals.size(); p++)
				{
					FPoint pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				ef.effectCode = ScImage::EF_QUADTONE;
				ef.effectParameters = efVal;
			}
			effectsInUse.append(ef);
		}
		pixm.applyEffect(effectsInUse, m_Doc->PageColors, false);
		if (reload)
			pixm.imgInfo.lowResType = lowResTypeBack;
		if (pixm.imgInfo.lowResType != 0)
		{
			double scaling = pixm.imgInfo.xres / 36.0;
			if (pixm.imgInfo.lowResType == 1)
				scaling = pixm.imgInfo.xres / 72.0;
			// Prevent exagerately large images when using low res preview modes
			uint pixels = qRound(pixm.width() * pixm.height() / (scaling * scaling));
			if (pixels > 3000000)
			{
				double ratio = pixels / 3000000.0;
				scaling *= sqrt(ratio);
			}
			pixm.createLowRes(scaling);
			pixm.imgInfo.lowResScale = scaling;
		}
		if ((m_Doc->view()->m_canvas->usePreviewVisual()))
		{
			VisionDefectColor defect;
			QColor tmpC;
			int h = pixm.qImagePtr()->height();
			int w = pixm.qImagePtr()->width();
			int r, g, b, a;
			QRgb *s;
			QRgb rgb;
			for( int yi=0; yi < h; ++yi )
			{
				s = (QRgb*)(pixm.qImagePtr()->scanLine( yi ));
				for( int xi = 0; xi < w; ++xi )
				{
					rgb = *s;
					tmpC.setRgb(rgb);
					tmpC = defect.convertDefect(tmpC, m_Doc->view()->m_canvas->previewVisual());
					a = qAlpha(rgb);
					tmpC.getRgb(&r, &g, &b);
					*s = qRgba(r, g, b, a);
					s++;
				}
			}
		}
	}
	return true;
}


void PageItem::drawLockedMarker(ScPainter *p)
{
	//TODO: CB clean
	double scp1 = 1 ;// / ScMW->view->scale();
	double ofwh = 6 * scp1;
	double ofx = Width - ofwh/2;
	double ofy = Height - ofwh*1.5;
	double bx1= ofx+ scp1;
	double by1= ofy+3 * scp1;
	double bw= 4*scp1;
	double bh= 2*scp1;
	ScribusView* view = m_Doc->view();
	p->setPen(Qt::black, 0.5 / view->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->drawRect(ofx, ofy, ofwh, ofwh);
	p->setBrush(Qt::black);
	p->drawRect(bx1, by1, bw, bh);
	p->setPen(Qt::black, 1.5 / view->scale(), Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
	if (m_Locked)
		p->drawLine(FPoint(bx1+scp1/2, ofy+scp1), FPoint(bx1+scp1/2, by1));
	p->drawLine(FPoint(bx1+scp1*3.5, ofy+scp1), FPoint(bx1+scp1*3.5, by1));
	p->drawLine(FPoint(bx1+scp1/2, ofy+scp1), FPoint(bx1+scp1*3.5, ofy+scp1));
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
		LocalScX = qMin(xs, ys);
		LocalScY = qMin(xs, ys);
	}
	else
	{
		LocalScX = xs;
		LocalScY = ys;
	}
	switch (m_Doc->RotMode)
	{
		case 0:
			LocalX = 0;
			LocalY = 0;
			break;
		case 1:
			LocalX = (Width - static_cast<double>(OrigW) * LocalScX) / LocalScX;
			LocalY = 0;
			break;
		case 2:
			LocalX = ((Width - static_cast<double>(OrigW) * LocalScX) / LocalScX) / 2.0;
			LocalY = ((Height - static_cast<double>(OrigH) * LocalScY) / LocalScY) / 2.0;
			break;
		case 3:
			LocalX = 0;
			LocalY = (Height - static_cast<double>(OrigH) * LocalScY) / LocalScY;
			break;
		case 4:
			LocalX = (Width - static_cast<double>(OrigW) * LocalScX) / LocalScX;
			LocalY = (Height - static_cast<double>(OrigH) * LocalScY) / LocalScY;
			break;
	}
	if (imageClip.size() != 0)
	{
		imageClip = pixm.imgInfo.PDSpathData[pixm.imgInfo.usedPath].copy();
		QMatrix cl;
		cl.translate(LocalX*LocalScX, LocalY*LocalScY);
		cl.scale(LocalScX, LocalScY);
		imageClip.map(cl);
	}
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY );
}

void PageItem::setExternalFile(QString val)
{
	Pfile = val;
	if (!Pfile.isEmpty())
	{
		QString test = QDir::cleanPath(QDir::convertSeparators(QDir::homePath()+"/"+Pfile));
		QFileInfo pfi2(test);
		Pfile = pfi2.absoluteFilePath();
	}
}

void PageItem::setFileIconPressed(QString val)
{
	Pfile2 = val;
	if (!Pfile2.isEmpty())
	{
		QString test = QDir::cleanPath(QDir::convertSeparators(QDir::homePath()+"/"+Pfile2));
		QFileInfo pfi2(test);
		Pfile2 = pfi2.absoluteFilePath();
	}
}

void PageItem::setFileIconRollover(QString val)
{
	Pfile3 = val;
	if (!Pfile3.isEmpty())
	{
		QString test = QDir::cleanPath(QDir::convertSeparators(QDir::homePath()+"/"+Pfile3));
		QFileInfo pfi2(test);
		Pfile3 = pfi2.absoluteFilePath();
	}
}

QRect PageItem::getRedrawBounding(const double viewScale)
{
	int x = qRound(floor(BoundingX - Oldm_lineWidth / 2.0 - 5) * viewScale);
	int y = qRound(floor(BoundingY - Oldm_lineWidth / 2.0 - 5) * viewScale);
	int w = qRound(ceil(BoundingW + Oldm_lineWidth + 10) * viewScale);
	int h = qRound(ceil(BoundingH + Oldm_lineWidth + 10) * viewScale);
	QRect ret = QRect(x, y, w, h);
	ret.translate(qRound(-m_Doc->minCanvasCoordinate.x() * viewScale), qRound(-m_Doc->minCanvasCoordinate.y() * viewScale));
	return ret;
}


void PageItem::setRedrawBounding()
{
	double bw, bh;
	getBoundingRect(&BoundingX, &BoundingY, &bw, &bh);
	BoundingW = bw - BoundingX;
	BoundingH = bh - BoundingY;
	if (asLine())
		BoundingH = qMax(BoundingH, 1.0);
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
	//if (ScMW->view->SelItem.count()!=0 && this==ScMW->view->SelItem.at(0))
	//if (m_Doc->m_Selection->count()!=0 && m_Doc->m_Selection->primarySelectionIsMyself(this))
	//	ScMW->propertiesPalette->updateColorSpecialGradient();
	//CB Will only emit if connected, ie is first in GUI selection
	double dur=m_Doc->unitRatio();
	emit gradientColorUpdate(GrStartX*dur, GrStartY*dur, GrEndX*dur, GrEndY*dur, Width*dur, Height*dur);
}

void PageItem::setPolyClip(int up, int down)
{
	Clip.resize(0);
	if (PoLine.size() < 4)
		return;
	double rot;
	int upval = up;
	int downval = down;
	if (up == 0)
		upval = 1;
	if (textPathFlipped)
	{
		upval *= -1;
		downval *= -1;
	}
	QPoint np, np2;
	QPolygon cl, cl1, cl2;
	cl = FlattenPath(PoLine, Segments);
	for (int a = 0; a < cl.size()-1; ++a)
	{
		rot = xy2Deg(cl.point(a+1).x()-cl.point(a).x(),cl.point(a+1).y()-cl.point(a).y());
		QMatrix ma;
		ma.rotate(rot);
		np = QPoint(0, -upval) * ma;
		np2 = QPoint(0, -downval) * ma;
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

void PageItem::updatePolyClip()
{
	int asce = 1;
	int desc = 1;
	uint itemTextCount=itemText.length();
	for (uint a = 0; a < itemTextCount; ++a)
	{
		const CharStyle& hl (itemText.charStyle(a));
		int des = static_cast<int>(hl.font().descent(hl.fontSize() / 10.0));
		int asc = static_cast<int>(hl.font().ascent(hl.fontSize() / 10.0));
		asce = qMax(asce, asc);
		desc = qMax(desc, des);
	}
	setPolyClip(static_cast<int>(asce-BaseOffs), static_cast<int>(desc-BaseOffs));
}

void PageItem::handleModeEditKey(QKeyEvent * /* k */, bool & /* keyRepeat */)
{
}

bool PageItem::connectToGUI()
{
	if (!ScCore->usingGUI())
		return false;
	if (!m_Doc->m_Selection->primarySelectionIs(this))
		return false;
	PropertiesPalette* pp=m_Doc->scMW()->propertiesPalette;
	connect(this, SIGNAL(myself(PageItem *)), pp, SLOT(SetCurItem(PageItem *)));
	connect(this, SIGNAL(frameType(int)), m_Doc->scMW(), SLOT(HaveNewSel(int)));
	connect(this, SIGNAL(frameType(int)), m_Doc->view(), SLOT(selectionChanged()));
	connect(this, SIGNAL(frameType(int)), pp, SLOT(NewSel(int)));
	connect(this, SIGNAL(frameLocked(bool)), pp, SLOT(setLocked(bool)));
	connect(this, SIGNAL(frameSizeLocked(bool)), pp, SLOT(setSizeLocked(bool)));
	connect(this, SIGNAL(frameFlippedH(bool)), pp, SLOT(setFlippedH(bool)));
	connect(this, SIGNAL(frameFlippedV(bool)), pp, SLOT(setFlippedV(bool)));
	connect(this, SIGNAL(printEnabled(bool)), pp, SLOT(setPrintingEnabled(bool)));
	connect(this, SIGNAL(position(double, double)), pp, SLOT(setXY(double, double)));
	connect(this, SIGNAL(widthAndHeight(double, double)), pp, SLOT(setBH(double, double)));
//CB unused in 135 connect(this, SIGNAL(colors(QString, QString, double, double)), m_Doc->scMW(), SLOT(setCSMenu()));
//	connect(this, SIGNAL(colors(QString, QString, double, double)), pp->Cpal, SLOT(setActFarben(QString, QString, int, int)));
//	connect(this, SIGNAL(gradientType(int)), pp->Cpal, SLOT(setActGradient(int)));
	connect(this, SIGNAL(patternFill(QString, double, double, double, double, double)), pp->Cpal, SLOT(setActPattern(QString, double, double, double, double, double)));
//	connect(this, SIGNAL(gradientColorUpdate(double, double, double, double, double, double)), pp->Cpal, SLOT(setSpecialGradient(double, double, double, double, double, double)));
	connect(this, SIGNAL(rotation(double)), pp, SLOT(setR(double)));
//	connect(this, SIGNAL(transparency(double, double)), pp->Cpal, SLOT(setActTrans(double, double)));
//	connect(this, SIGNAL(blendmode(int, int)), pp->Cpal, SLOT(setActBlend(int, int)));
	//Shape signals
	//Not connected when transferring code: void columns(int, double); //Number, gap
	connect(this, SIGNAL(cornerRadius(double)), pp, SLOT(setRR(double)));
	//	connect(view, SIGNAL(ItemTextCols(int, double)), propertiesPalette, SLOT(setCols(int, double)));
	//Line signals
	connect(this, SIGNAL(lineWidth(double)), pp, SLOT(setLineWidth(double)));
	connect(this, SIGNAL(imageOffsetScale(double, double, double, double)), pp, SLOT(setScaleAndOffset(double, double, double, double)));
	connect(this, SIGNAL(lineStyleCapJoin(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)), pp, SLOT( setLIvalue(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)));
	//Frame text signals
	connect(this, SIGNAL(lineSpacing(double)), pp, SLOT(setLsp(double)));
	connect(this, SIGNAL(textToFrameDistances(double, double, double, double)), pp, SLOT(setTextToFrameDistances(double, double, double, double)));
	connect(this, SIGNAL(textKerning(double)), pp, SLOT(setExtra(double)));
	connect(this, SIGNAL(textStyle(int)), pp, SLOT(setStil(int)));
	connect(this, SIGNAL(textStyle(int)), m_Doc->scMW(), SLOT(setStilvalue(int)));
	connect(this, SIGNAL(textFont(const QString&)), pp, SLOT(setFontFace(const QString&)));
	connect(this, SIGNAL(textSize(double)), pp, SLOT(setSize(double)));
	connect(this, SIGNAL(textWidthScale(double)), pp, SLOT(setTScale(double)));
	connect(this, SIGNAL(textHeightScale(double)), pp, SLOT(setTScaleV(double)));
	connect(this, SIGNAL(textBaseLineOffset(double)), pp, SLOT(setTBase(double)));
	connect(this, SIGNAL(textOutline(double)), pp, SLOT(setOutlineW(double)));
	connect(this, SIGNAL(textShadow(double, double )), pp, SLOT(setShadowOffs(double, double )));
	connect(this, SIGNAL(textUnderline(double, double)), pp, SLOT(setUnderline(double, double)));
	connect(this, SIGNAL(textStrike(double, double)), pp, SLOT(setStrike(double, double)));
	connect(this, SIGNAL(textColor(QString, QString, double, double)), pp, SLOT(setActFarben(QString, QString, double, double)));
//	connect(this, SIGNAL(textFormatting(int)), pp, SLOT(setAli(int)));
//	connect(this, SIGNAL(textFormatting(int)), ScMW, SLOT(setAbsValue(int)));

	return true;
}

bool PageItem::disconnectFromGUI()
{
	if (!ScCore->usingGUI())
		return false;
	// Disconnecting only signals from PP will leave some remaining connections
	// and cause progressive slowdowns
	// PropertiesPalette* pp=m_Doc->scMW()->propertiesPalette;
	// disconnect(this, 0, pp, 0);
	disconnect(this, 0, 0, 0);
	return true;
}

void PageItem::emitAllToGUI()
{
	updateConstants();

	emit myself(this);
	emit frameType(m_ItemType);
/*CB using the emit myself* instead of all of these
	emit position(Xpos, Ypos);
	emit widthAndHeight(Width, Height);
	emit rotation(Rot);
	emit frameLocked(m_Locked);
	emit frameSizeLocked(m_SizeLocked);
	emit frameFlippedH(m_ImageIsFlippedH);
	emit frameFlippedV(m_ImageIsFlippedV);
	emit printEnabled(m_PrintEnabled);
	emit lineWidth(m_lineWidth);
	emit lineStyleCapJoin(PLineArt, PLineEnd, PLineJoin);
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY);
*/
//CB unused in 135 emit colors(lineColorVal, fillColorVal, lineShadeVal, fillShadeVal);
//	emit gradientType(GrType);
//	double dur=m_Doc->unitRatio();
//	emit gradientColorUpdate(GrStartX*dur, GrStartY*dur, GrEndX*dur, GrEndY*dur, Width*dur, Height*dur);
//	emit transparency(fillTransparencyVal, lineTransparencyVal);
//	emit blendmode(fillBlendmodeVal, lineBlendmodeVal);
/*CB using the emit myself* instead of all of these
	emit patternFill(patternVal, patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
	emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
	emit columns(Cols, ColGap);
*/
	if (m_Doc->appMode == modeEdit)
	{
/* maybe already done elsewhere? -- av
		emit lineSpacing(currentStyle().lineSpacing());
		emit textKerning(currentCharStyle().tracking());
		emit textStyle(currentCharStyle().effects());
		emit textFont(currentCharStyle().font()->scName());
		emit textSize(currentCharStyle().fontSize());
*/
		//		emit textFormatting(currentStyle().alignment());
	}
	else
	{
//TODO remove and use the emit myself
		emit lineSpacing(itemText.defaultStyle().lineSpacing());
		emit textKerning(itemText.defaultStyle().charStyle().tracking());
		emit textStyle(itemText.defaultStyle().charStyle().effects());
		emit textFont(itemText.defaultStyle().charStyle().font().scName());
		emit textSize(itemText.defaultStyle().charStyle().fontSize());
//		emit textFormatting(itemText.defaultStyle().alignment());
	}
}

void PageItem::setIsAnnotation(bool isAnnot)
{
	m_isAnnotation=isAnnot;
}

void PageItem::setAnnotation(const Annotation& ad)
{
	m_annotation=ad;
}

void PageItem::setImageShown(bool isShown)
{
	PicArt=isShown;
}

void PageItem::updateConstants()
{
	if (OwnPage!=-1)
	{
		m_Doc->constants().insert("pagewidth", m_Doc->Pages->at(OwnPage)->width());
		m_Doc->constants().insert("pageheight", m_Doc->Pages->at(OwnPage)->height());
	}
	m_Doc->constants().insert("width", Width);
	m_Doc->constants().insert("height", Height);
}

//CB Old ScribusView MoveItemI
void PageItem::moveImageInFrame(double newX, double newY)
{
	if (m_ItemType!=PageItem::ImageFrame)
		return;
	if ((locked()) || (!ScaleType))
		return;
	double dX=0.0, dY=0.0;
	if (imageFlippedH())
		dX=-newX;
	else
		dX=newX;
	if (imageFlippedV())
		dY=-newY;
	else
		dY=newY;
	moveImageXYOffsetBy(dX, dY);
	if (imageClip.size() != 0)
	{
		imageClip = pixm.imgInfo.PDSpathData[pixm.imgInfo.usedPath].copy();
		QMatrix cl;
		cl.translate(imageXOffset()*imageXScale(), imageYOffset()*imageYScale());
		cl.scale(imageXScale(), imageYScale());
		imageClip.map(cl);
	}
}



void PageItem::convertClip()
{
	if (Clip.count() != 0)
	{
		FPoint np(Clip.point(0));
		PoLine.resize(2);
		PoLine.setPoint(0, np);
		PoLine.setPoint(1, np);
		for (int a = 1; a < Clip.size(); ++a)
		{
			np = FPoint(Clip.point(a));
			PoLine.putPoints(PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
		}
		np = FPoint(Clip.point(0));
		PoLine.putPoints(PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
		Clip = FlattenPath(PoLine, Segments);
	}
	else
	{
		SetRectFrame();
		m_Doc->setRedrawBounding(this);
	}
}

void PageItem::updateClip()
{
	if (m_Doc->appMode == modeDrawBezierLine)
		return;
	if (ContourLine.size() == 0)
		ContourLine = PoLine.copy();
	int ph = static_cast<int>(qMax(1.0, lineWidth() / 2.0));
	bool clipBackup = ClipEdited;
	switch (itemType())
	{
	case PageItem::Line:
		Clip.setPoints(4, -ph,-ph, static_cast<int>(width()+ph),-ph,
		                  static_cast<int>(width()+ph),static_cast<int>(height()+ph),
		                  -ph,static_cast<int>(height()+ph));
		break;
	default:
		if (((!ClipEdited) || (FrameType < 3)) && !(asPathText()))
		{
			switch (FrameType)
			{
			case 0:
				SetRectFrame();
				m_Doc->setRedrawBounding(this);
				break;
			case 1:
				SetOvalFrame();
				m_Doc->setRedrawBounding(this);
				break;
			case 2:
				//CB FIXME: stop using nodeEdit.clre or move out of here
				m_Doc->nodeEdit.deselect();
				SetFrameRound();
				m_Doc->setRedrawBounding(this);
				break;
			default:
				break;
			}
			ClipEdited = clipBackup;
			if ((OldB2 != 0) && (OldH2 != 0) && (width() != 0) && (height() != 0))
			{
				double scx = width() / OldB2;
				double scy = height() / OldH2;
				QMatrix ma;
				ma.scale(scx, scy);
				FPointArray gr;
				gr.addPoint(GrStartX, GrStartY);
				gr.addPoint(GrEndX, GrEndY);
				gr.map(ma);
				GrStartX = gr.point(0).x();
				GrStartY = gr.point(0).y();
				GrEndX = gr.point(1).x();
				GrEndY = gr.point(1).y();
				ContourLine.map(ma);
				if (FrameType > 2)
				{
					PoLine.map(ma);
					if (asPathText())
						updatePolyClip();
					else
						Clip = FlattenPath(PoLine, Segments);
				}
			}
			OldB2 = width();
			OldH2 = height();
			if ((FrameType < 3) && (!ClipEdited))
				ContourLine = PoLine.copy();
		}
		else
		{
			if (m_Doc->SubMode != -1)
			{
				switch (m_Doc->SubMode)
				{
				case 0:
					SetRectFrame();
					m_Doc->setRedrawBounding(this);
					break;
				case 1:
					SetOvalFrame();
					m_Doc->setRedrawBounding(this);
					break;
				default:
					SetFrameShape(m_Doc->ValCount, m_Doc->ShapeValues);
					m_Doc->setRedrawBounding(this);
					break;
				}
				OldB2 = width();
				OldH2 = height();
				ContourLine = PoLine.copy();
			}
			if ((OldB2 == 0) || (OldH2 == 0) || (width() == 0) || (height() == 0))
				return;
			double scx = width() / OldB2;
			double scy = height() / OldH2;
			QMatrix ma;
			ma.scale(scx, scy);
			FPointArray gr;
			gr.addPoint(GrStartX, GrStartY);
			gr.addPoint(GrEndX, GrEndY);
			gr.map(ma);
			GrStartX = gr.point(0).x();
			GrStartY = gr.point(0).y();
			GrEndX = gr.point(1).x();
			GrEndY = gr.point(1).y();
			PoLine.map(ma);
			ContourLine.map(ma);
			if (asPathText())
				updatePolyClip();
			else
				Clip = FlattenPath(PoLine, Segments);
			OldB2 = width();
			OldH2 = height();
		}
		break;
	}
	updateGradientVectors();
}

QString PageItem::infoDescription()
{
	QString htmlText;
	htmlText.append(ScribusView::tr("Print: "));
	if (printEnabled())
		htmlText.append(ScribusView::tr("Enabled"));
	else
		htmlText.append(ScribusView::tr("Disabled"));
	htmlText.append("<br/>");
	return htmlText;
}

FirstLineOffsetPolicy PageItem::firstLineOffset() const
{
	return firstLineOffsetP;
}

void PageItem::setFirstLineOffset(FirstLineOffsetPolicy flop)
{
	if(firstLineOffsetP != flop)
	{
		firstLineOffsetP = flop;
	}
}


