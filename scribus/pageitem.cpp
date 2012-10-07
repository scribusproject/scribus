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
#include <QPointF>
#include <QFileInfo>
#include <qdrawutil.h>
#include <QRegExp>
#include <QMessageBox>
#include <QPolygon>
#include <cassert>
#include <QDebug>

#include "canvas.h"
#include "cmsettings.h"
#include "colorblind.h"
#include "commonstrings.h"

#include "pageitem_group.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_arc.h"
#include "pageitem_spiral.h"
#include "pageitem_textframe.h"
#include "pageitem_latexframe.h"
#include "prefsmanager.h"

#include "resourcecollection.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scimagecacheproxy.h"
#include "sclimits.h"
#include "scpage.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "scribuswin.h"
#include "sctextstream.h"
#include "selection.h"
#include "text/nlsconfig.h"
#include "text/storytext.h"
#include "undomanager.h"
#include "undostate.h"
#include "util.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"
#include "util_text.h"

#include "ui/guidemanager.h"
#include "ui/propertiespalette.h"

#include <cairo.h>

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
	GrFocalX(other.GrFocalX),
	GrFocalY(other.GrFocalY),
	GrScale(other.GrScale),
	GrSkew(other.GrSkew),
	GrControl1(other.GrControl1),
	GrControl2(other.GrControl2),
	GrControl3(other.GrControl3),
	GrControl4(other.GrControl4),
	GrControl5(other.GrControl5),
	GrColorP1(other.GrColorP1),
	GrColorP2(other.GrColorP2),
	GrColorP3(other.GrColorP3),
	GrColorP4(other.GrColorP4),
	GrCol1transp(other.GrCol1transp),
	GrCol2transp(other.GrCol2transp),
	GrCol3transp(other.GrCol3transp),
	GrCol4transp(other.GrCol4transp),
	GrCol1Shade(other.GrCol1Shade),
	GrCol2Shade(other.GrCol2Shade),
	GrCol3Shade(other.GrCol3Shade),
	GrCol4Shade(other.GrCol4Shade),
	meshGradientPatches(other.meshGradientPatches),
	meshGradientArray(other.meshGradientArray),
	selectedMeshPointX(other.selectedMeshPointX),
	selectedMeshPointY(other.selectedMeshPointY),
	selectedMeshControlPoint(other.selectedMeshControlPoint),
	snapToPatchGrid(other.snapToPatchGrid),
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
	OverrideCompressionMethod(other.OverrideCompressionMethod),
	CompressionMethodIndex(other.CompressionMethodIndex),
	OverrideCompressionQuality(other.OverrideCompressionQuality),
	CompressionQualityIndex(other.CompressionQualityIndex),

	PictureIsAvailable(other.PictureIsAvailable),
	OrigW(other.OrigW),
	OrigH(other.OrigH),
	BBoxX(other.BBoxX),
	BBoxH(other.BBoxH),
	CurX(other.CurX),
	CurY(other.CurY),
	itemText(other.itemText),
	isBookmark(other.isBookmark),
	HasSel(other.HasSel),
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
//	toPixmap(other.toPixmap),
	LayerID(other.LayerID),
	ScaleType(other.ScaleType),
	AspectRatio(other.AspectRatio),
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
	groupItemList(other.groupItemList),
	groupWidth(other.groupWidth),
	groupHeight(other.groupHeight),
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
	gradientVal(other.gradientVal),
	patternVal(other.patternVal),
	patternScaleX(other.patternScaleX),
	patternScaleY(other.patternScaleY),
	patternOffsetX(other.patternOffsetX),
	patternOffsetY(other.patternOffsetY),
	patternRotation(other.patternRotation),
	patternSkewX(other.patternSkewX),
	patternSkewY(other.patternSkewY),
	patternMirrorX(other.patternMirrorX),
	patternMirrorY(other.patternMirrorY),
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
	GrColorP1QColor(other.GrColorP1QColor),
	GrColorP2QColor(other.GrColorP2QColor),
	GrColorP3QColor(other.GrColorP3QColor),
	GrColorP4QColor(other.GrColorP4QColor),
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
	LocalRot(other.LocalRot),
	Reverse(other.Reverse),
	m_startArrowIndex(other.m_startArrowIndex),
	m_endArrowIndex(other.m_endArrowIndex),
	m_startArrowScale(other.m_startArrowScale),
	m_endArrowScale(other.m_endArrowScale),
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
	Oldm_lineWidth(other.Oldm_lineWidth),
	patternStrokeVal(other.patternStrokeVal),
	patternStrokeScaleX(other.patternStrokeScaleX),
	patternStrokeScaleY(other.patternStrokeScaleY),
	patternStrokeOffsetX(other.patternStrokeOffsetX),
	patternStrokeOffsetY(other.patternStrokeOffsetY),
	patternStrokeRotation(other.patternStrokeRotation),
	patternStrokeSkewX(other.patternStrokeSkewX),
	patternStrokeSkewY(other.patternStrokeSkewY),
	patternStrokeSpace(other.patternStrokeSpace),
	patternStrokeMirrorX(other.patternStrokeMirrorX),
	patternStrokeMirrorY(other.patternStrokeMirrorY),
	patternStrokePath(other.patternStrokePath),
	gradientStrokeVal(other.gradientStrokeVal),
	stroke_gradient(other.stroke_gradient),
	GrTypeStroke(other.GrTypeStroke),
	GrStrokeStartX(other.GrStrokeStartX),
	GrStrokeStartY(other.GrStrokeStartY),
	GrStrokeEndX(other.GrStrokeEndX),
	GrStrokeEndY(other.GrStrokeEndY),
	GrStrokeFocalX(other.GrStrokeFocalX),
	GrStrokeFocalY(other.GrStrokeFocalY),
	GrStrokeScale(other.GrStrokeScale),
	GrStrokeSkew(other.GrStrokeSkew),
	GrMask(other.GrMask),
	GrMaskStartX(other.GrMaskStartX),
	GrMaskStartY(other.GrMaskStartY),
	GrMaskEndX(other.GrMaskEndX),
	GrMaskEndY(other.GrMaskEndY),
	GrMaskFocalX(other.GrMaskFocalX),
	GrMaskFocalY(other.GrMaskFocalY),
	GrMaskScale(other.GrMaskScale),
	GrMaskSkew(other.GrMaskSkew),
	patternMaskScaleX(other.patternMaskScaleX),
	patternMaskScaleY(other.patternMaskScaleY),
	patternMaskOffsetX(other.patternMaskOffsetX),
	patternMaskOffsetY(other.patternMaskOffsetY),
	patternMaskRotation(other.patternMaskRotation),
	patternMaskSkewX(other.patternMaskSkewX),
	patternMaskSkewY(other.patternMaskSkewY),
	patternMaskMirrorX(other.patternMaskMirrorX),
	patternMaskMirrorY(other.patternMaskMirrorY),
	patternMaskVal(other.patternMaskVal),
	mask_gradient(other.mask_gradient)
{
	QString tmp;
	m_Doc->TotalItems++;
	AnName += tmp.setNum(m_Doc->TotalItems);
	uniqueNr = m_Doc->TotalItems;
	invalid = true;
	if (other.isInlineImage)
	{
		QFileInfo inlFi(Pfile);
		QString ext = inlFi.suffix();
		tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + ext);
		tempImageFile->open();
		QString fileName = getLongPathName(tempImageFile->fileName());
		tempImageFile->close();
		copyFile(Pfile, fileName);
		Pfile = fileName;
		isInlineImage = true;
	}
	else
	{
		tempImageFile = NULL;
		isInlineImage = false;
	}
	Parent = NULL;
	unWeld();
}


PageItem::PageItem(ScribusDoc *pa, ItemType newType, double x, double y, double w, double h, double w2, QString fill, QString outline)
	// Initialize superclass(es)
	: QObject(pa), SingleObservable<PageItem>(pa->itemsChanged()),
	// Initialize member variables
	OverrideCompressionMethod(false),
	CompressionMethodIndex(0),
	OverrideCompressionQuality(false),
	CompressionQualityIndex(0),
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
	Parent = NULL;
	m_Doc = pa;
	QString tmp;
	BackBox = 0;
	NextBox = 0;
	gXpos = oldXpos = Xpos = x;
	gYpos = oldYpos = Ypos = y;
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
	gWidth = gHeight = 0;
	GrType = 0;
	GrStartX = 0;
	GrStartY = 0;
	GrEndX = w;
	GrEndY = 0;
	GrFocalX = 0;
	GrFocalY = 0;
	GrScale = 1;
	GrSkew = 0;
	GrControl1 = FPoint(0,0);
	GrControl2 = FPoint(w, 0);
	GrControl3 = FPoint(w, h);
	GrControl4 = FPoint(0, h);
	GrControl5 = FPoint(w / 2.0, h / 2.0);
	GrCol1transp = 1.0;
	GrCol2transp = 1.0;
	GrCol3transp = 1.0;
	GrCol4transp = 1.0;
	GrCol1Shade = 100;
	GrCol2Shade = 100;
	GrCol3Shade = 100;
	GrCol4Shade = 100;
	meshGradientPatches.clear();
	meshGradientArray.clear();
	selectedMeshPointX = -1;
	selectedMeshPointY = -1;
	gradientVal = "";
	GrTypeStroke = 0;
	GrStrokeStartX = 0;
	GrStrokeStartY = 0;
	GrStrokeEndX = w;
	GrStrokeEndY = 0;
	GrStrokeFocalX = 0;
	GrStrokeFocalY = 0;
	GrStrokeScale = 1;
	GrStrokeSkew = 0;
	gradientStrokeVal = "";
	patternVal = "";
	patternScaleX = 100;
	patternScaleY = 100;
	patternOffsetX = 0;
	patternOffsetY = 0;
	patternRotation = 0;
	patternSkewX = 0;
	patternSkewY = 0;
	patternMirrorX = false;
	patternMirrorY = false;
	patternStrokeVal = "";
	patternStrokeScaleX = 100;
	patternStrokeScaleY = 100;
	patternStrokeOffsetX = 0;
	patternStrokeOffsetY = 0;
	patternStrokeRotation = 0;
	patternStrokeSkewX = 0;
	patternStrokeSkewY = 0;
	patternStrokeSpace = 1.0;
	patternStrokeMirrorX = false;
	patternStrokeMirrorY = false;
	patternStrokePath = false;
	m_lineWidth = w2;
	Oldm_lineWidth = w2;
	PLineArt = Qt::PenStyle(m_Doc->itemToolPrefs().shapeLineStyle);
	PLineEnd = Qt::FlatCap;
	PLineJoin = Qt::MiterJoin;
	Select = false;
	ClipEdited = false;
	FrameType = 0;
	CurX = 0;
	CurY = 0;
	Extra = m_Doc->itemToolPrefs().textDistances.Left;
	TExtra = m_Doc->itemToolPrefs().textDistances.Top;
	BExtra = m_Doc->itemToolPrefs().textDistances.Bottom;
	RExtra = m_Doc->itemToolPrefs().textDistances.Right;
	firstChar = 0;
	MaxChars = 0;
	Pfile = "";
	pixm = ScImage();
	pixm.imgInfo.lowResType = m_Doc->itemToolPrefs().imageLowResType;
	Pfile2 = "";
	Pfile3 = "";
	oldLocalScX = LocalScX = 1;
	oldLocalScY = LocalScY = 1;
	OrigW = 0;
	OrigH = 0;
	oldLocalX = LocalX = 0;
	oldLocalY = LocalY = 0;
	LocalRot = 0;
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
	OwnPage = m_Doc->currentPage() ? m_Doc->currentPage()->pageNr() : -1;
	oldOwnPage = OwnPage;
	savedOwnPage = OwnPage;
	PicArt = true;
	PictureIsAvailable = false;
	m_PrintEnabled = true;
	isBookmark = false;
	m_isAnnotation = false;
	weldList.clear();
	
	switch (m_ItemType)
	{
	case ImageFrame:
	case OSGFrame:
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
	case Symbol:
		AnName = tr("Symbol");
		setUPixmap(Um::IPolygon);
		break;
	case Group:
		AnName = tr("Group");
		setUPixmap(Um::IPolygon);
		break;
	case RegularPolygon:
		AnName = tr("RegularPolygon");
		setUPixmap(Um::IPolygon);
		break;
	case Arc:
		AnName = tr("Arc");
		setUPixmap(Um::IPolygon);
		break;
	case Spiral:
		AnName = tr("Spiral");
		setUPixmap(Um::IPolygon);
		break;
	case Table:
		AnName = tr("Table");
		//setUPixmap(Um::IPolygon); // TODO: Fix this.
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
	isAutoText = false;
	inPdfArticle = false;
	isRaster = false;
	Sizing = false;
//	toPixmap = false;
	UseEmbedded = true;
	IRender = Intent_Relative_Colorimetric;
	EmProfile = "";
	groupItemList.clear();
	groupWidth = 1.0;
	groupHeight = 1.0;
	LayerID = m_Doc->activeLayer();
	ScaleType = true;
	AspectRatio = true;
	Reverse = false;
	NamedLStyle = "";
	DashValues.clear();
	DashOffset = 0;
	fillRule = true;
	doOverprint = false;
	stroke_gradient = VGradient(VGradient::linear);
	stroke_gradient.clearStops();
	if (lineColorVal != CommonStrings::None)
	{
		const ScColor& col = m_Doc->PageColors[lineColorVal];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, lineColorVal, 100);
		stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, lineColorVal, 100);
	}
	else
	{
		if (m_Doc->itemToolPrefs().shapeLineColor != CommonStrings::None)
		{
			const ScColor& col = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeLineColor];
			QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
			stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
			stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
		}
		else
		{
			if (fillColorVal != CommonStrings::None)
			{
				const ScColor& col = m_Doc->PageColors[fillColorVal];
				QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
				stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, fillColorVal, 100);
				stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, fillColorVal, 100);
			}
			else
			{
				if (m_Doc->itemToolPrefs().shapeLineColor != CommonStrings::None)
				{
					const ScColor& col = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeFillColor];
					QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
					stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeFillColor, 100);
					stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeFillColor, 100);
				}
				else if (m_Doc->PageColors.contains("Black"))
				{
					const ScColor& col = m_Doc->PageColors["Black"];
					QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
					stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, "Black", 100);
					stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, "Black", 100);
				}
			}
		}
	}
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();
	if (fillColorVal != CommonStrings::None)
	{
		const ScColor& col = m_Doc->PageColors[fillColorVal];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, fillColorVal, 100);
		fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, fillColorVal, 100);
		GrColorP1 = fillColorVal;
		GrColorP2 = fillColorVal;
		GrColorP3 = fillColorVal;
		GrColorP4 = fillColorVal;
	}
	else
	{
		if (m_Doc->itemToolPrefs().shapeFillColor != CommonStrings::None)
		{
			const ScColor& col = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeFillColor];
			QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
			fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeFillColor, 100);
			fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeFillColor, 100);
			GrColorP1 = m_Doc->itemToolPrefs().shapeFillColor;
			GrColorP2 = m_Doc->itemToolPrefs().shapeFillColor;
			GrColorP3 = m_Doc->itemToolPrefs().shapeFillColor;
			GrColorP4 = m_Doc->itemToolPrefs().shapeFillColor;
		}
		else
		{
			if (lineColorVal != CommonStrings::None)
			{
				const ScColor& col = m_Doc->PageColors[lineColorVal];
				QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
				fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, lineColorVal, 100);
				fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, lineColorVal, 100);
				GrColorP1 = lineColorVal;
				GrColorP2 = lineColorVal;
				GrColorP3 = lineColorVal;
				GrColorP4 = lineColorVal;
			}
			else
			{
				if (m_Doc->itemToolPrefs().shapeLineColor != CommonStrings::None)
				{
					const ScColor& col = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeLineColor];
					QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
					fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
					fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
					GrColorP1 = m_Doc->itemToolPrefs().shapeLineColor;
					GrColorP2 = m_Doc->itemToolPrefs().shapeLineColor;
					GrColorP3 = m_Doc->itemToolPrefs().shapeLineColor;
					GrColorP4 = m_Doc->itemToolPrefs().shapeLineColor;
				}
				else if (m_Doc->PageColors.contains("Black"))
				{
					const ScColor& col = m_Doc->PageColors["Black"];
					QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
					fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, "Black", 100);
					fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, "Black", 100);
					GrColorP1 = "Black";
					GrColorP2 = "Black";
					GrColorP3 = "Black";
					GrColorP4 = "Black";
				}
			}
		}
	}
	GrMask = 0;
	GrMaskStartX = 0;
	GrMaskStartY = 0;
	GrMaskEndX = w;
	GrMaskEndY = 0;
	GrMaskFocalX = 0;
	GrMaskFocalY = 0;
	GrMaskScale = 1;
	GrMaskSkew = 0;
	patternMaskScaleX = 100;
	patternMaskScaleY = 100;
	patternMaskOffsetX = 0;
	patternMaskOffsetY = 0;
	patternMaskRotation = 0;
	patternMaskSkewX = 0;
	patternMaskSkewY = 0;
	patternMaskMirrorX = false;
	patternMaskMirrorY = false;
	patternMaskVal = "";
	gradientMaskVal = "";
	mask_gradient = VGradient(VGradient::linear);
	mask_gradient.clearStops();
	const ScColor& col = m_Doc->PageColors["Black"];
	QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
	mask_gradient.addStop(qcol, 0.0, 0.5, 1.0, "Black", 100);
	mask_gradient.addStop(qcol, 1.0, 0.5, 1.0, "Black", 100);

	QList<meshPoint> mgList;
	meshPoint mgP;
	meshGradientPatch patch;
	mgP.resetTo(FPoint(0.0, 0.0));
	mgP.transparency = 1.0;
	mgP.shade = 100;
	mgP.colorName = "Black";
	mgP.color = qcol;
	mgList.append(mgP);
	patch.TL = mgP;
	mgP.resetTo(FPoint(Width / 2.0, 0.0));
	mgList.append(mgP);
	mgP.resetTo(FPoint(Width, 0.0));
	mgList.append(mgP);
	patch.TR = mgP;
	meshGradientArray.append(mgList);
	mgList.clear();
	mgP.resetTo(FPoint(0.0, Height / 2.0));
	mgList.append(mgP);
	mgP.resetTo(FPoint(Width / 2.0, Height / 2.0));
	mgList.append(mgP);
	mgP.resetTo(FPoint(Width, Height / 2.0));
	mgList.append(mgP);
	meshGradientArray.append(mgList);
	mgList.clear();
	mgP.resetTo(FPoint(0.0, Height));
	mgList.append(mgP);
	patch.BL = mgP;
	mgP.resetTo(FPoint(Width / 2.0, Height));
	mgList.append(mgP);
	mgP.resetTo(FPoint(Width, Height));
	mgList.append(mgP);
	patch.BR = mgP;
	meshGradientArray.append(mgList);
	meshGradientPatches.append(patch);

	firstLineOffsetP = FLOPRealGlyphHeight;
	Cols = m_Doc->itemToolPrefs().textColumns;
	ColGap = m_Doc->itemToolPrefs().textColumnGap;
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
	ChangedMasterItem = false;
	isEmbedded = false;
	OnMasterPage = m_Doc->currentPage() ? m_Doc->currentPage()->pageName() : QString();
	m_startArrowIndex = m_Doc->itemToolPrefs().lineStartArrow;
	m_endArrowIndex = m_Doc->itemToolPrefs().lineEndArrow;
	m_startArrowScale = 100;
	m_endArrowScale = 100;
	effectsInUse.clear();
	//Page Item Attributes
	pageItemAttributes.clear();
	for(ObjAttrVector::Iterator objAttrIt = m_Doc->itemAttributes().begin() ; objAttrIt != m_Doc->itemAttributes().end(); ++objAttrIt )
	{
		if (((*objAttrIt).autoaddto=="textframes" && m_ItemType==TextFrame) ||
			((*objAttrIt).autoaddto=="imageframes" && m_ItemType==ImageFrame)
			)
			pageItemAttributes.append(*objAttrIt);
	}
	tempImageFile = NULL;
	isInlineImage = false;
}

void PageItem::setXPos(const double newXPos, bool drawingOnly)
{
	Xpos = newXPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setYPos(const double newYPos, bool drawingOnly)
{
	Ypos = newYPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setXYPos(const double newXPos, const double newYPos, bool drawingOnly)
{
	Xpos = newXPos;
	Ypos = newYPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
}

int PageItem::level()
{
	if (Parent == NULL)
	{
		if (m_Doc)
		{
			QList<PageItem*>* items = OnMasterPage.isEmpty() ? &m_Doc->DocItems : &m_Doc->MasterItems;
			return (items->indexOf(this) + 1);
		}
		return 0;
	}
	return (Parent->asGroupFrame()->groupItemList.indexOf(this) + 1);
}

void PageItem::moveBy(const double dX, const double dY, bool drawingOnly)
{
	if (dX==0.0 && dY==0.0)
		return;
	invalid = true;
	if (dX!=0.0)
	{
		Xpos+=dX;
		gXpos+=dX;
	}
	if (dY!=0.0)
	{
		Ypos+=dY;
		gYpos+=dY;
	}
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
	moveWelded(dX, dY);
}

void PageItem::setWidth(const double newWidth)
{
	Width = newWidth;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setHeight(const double newHeight)
{
	Height = newHeight;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setWidthHeight(const double newWidth, const double newHeight, bool drawingOnly)
{
	Width = newWidth;
	Height = newHeight;
	updateConstants();
	if (drawingOnly)
		return;
	checkChanges();
}

void PageItem::setWidthHeight(const double newWidth, const double newHeight)
{
	Width = newWidth;
	Height = newHeight;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::resizeBy(const double dH, const double dW)
{
	if (dH==0.0 && dW==0.0)
		return;
	if (dH!=0.0)
		Width+=dH;
	if (dW!=0.0)
		Height+=dW;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setRotation(const double newRotation, bool drawingOnly)
{
	double dR = newRotation - Rot;
	double oldRot = Rot;
	Rot = newRotation;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
	rotateWelded(dR, oldRot);
}

void PageItem::rotateBy(const double dR)
{
	if (dR==0.0)
		return;
	Rot+=dR;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setSelected(const bool toSelect)
{
	Select=toSelect;
}

void PageItem::setImageXScale(const double newImageXScale)
{
	LocalScX=newImageXScale;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setImageYScale(const double newImageYScale)
{
	LocalScY=newImageYScale;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setImageXYScale(const double newImageXScale, const double newImageYScale)
{
	LocalScX=newImageXScale;
	LocalScY=newImageYScale;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setImageXOffset(const double newImageXOffset)
{
	LocalX=newImageXOffset;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setImageYOffset(const double newImageYOffset)
{
	LocalY=newImageYOffset;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setImageXYOffset(const double newImageXOffset, const double newImageYOffset)
{
	LocalX=newImageXOffset;
	LocalY=newImageYOffset;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::moveImageXYOffsetBy(const double dX, const double dY)
{
	if (dX==0.0 && dY==0.0)
		return;
	if (dX != 0.0)
		LocalX += dX;
	if (dY != 0.0)
		LocalY += dY;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setImageRotation(const double newRotation)
{
	if(LocalRot == newRotation)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Rotate,"",Um::IRotate);
		ss->set("IMAGE_ROTATION","image_rotation");
		ss->set("OLD_ROT",LocalRot);
		ss->set("NEW_ROT",newRotation);
		undoManager->action(this,ss);
	}
	LocalRot = newRotation;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setReversed(bool newReversed)
{
	if (Reverse == newReversed)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, 0, Um::IFlipH);
		ss->set("REVERSE_TEXT", newReversed);
		undoManager->action(this, ss);
	}
	Reverse = newReversed;
}

//return frame where is text end
PageItem * PageItem::frameTextEnd()
{
	PageItem * LastBox = this;
	if (frameOverflows() && NextBox)
	{ // text ending in some next frame
		LastBox = NextBox;
		while (LastBox != 0 && !LastBox->frameDisplays(itemText.length()-1))
			LastBox = LastBox->nextInChain();
	}
	else if (frameUnderflows() && BackBox)
	{ //text ending in some previous frame
		LastBox = BackBox;
		while (LastBox != 0 && !LastBox->frameDisplays(itemText.length()-1))
			LastBox = LastBox->prevInChain();
	}
	return LastBox;
}

/// returns true if text overflows
bool PageItem::frameOverflows() const
{
#ifndef NLS_PROTO
	// Fix #6991 : "Text overflow" warning when there is a text underflow in fact
	/*return NextBox == NULL && itemText.length() > static_cast<int>(MaxChars);*/
	return ( NextBox == NULL )
	       && ( static_cast<int> ( firstChar ) < itemText.length() )
		   // Fix #7766 : scribus.textOverflows() returns 0 if there is no place for the overflow mark
	       /*&& ( firstChar < MaxChars )*/
		   && ( firstChar <= MaxChars )
	       && ( itemText.length() > static_cast<int> ( MaxChars ) );
#else
	return false; // FIXME:NLS
#endif
}

int PageItem::frameOverflowCount() const
{
	if (frameOverflows())
		return itemText.length()-MaxChars;
	return 0;
}

int PageItem::maxCharsInFrame()
{
	return MaxChars;
}

/// returns true if text is ending before that frame
bool PageItem::frameUnderflows() const
{
	if (BackBox == NULL)
		return false;
	//FIX ME - I have found that condition if frame is empty
	//and has been linked with previous frame
	//if you will find any better solution - fix that function
	return (firstInFrame() > lastInFrame());
}

void PageItem::drawOverflowMarker(ScPainter *p)
{
	qreal sideLength = 10 / qMax(p->zoomFactor(), 1.0);
	qreal offset = 1 / qMax(p->zoomFactor(), 1.0);
	qreal left = Width - sideLength-offset;// / 2;
	qreal right = left + sideLength;
	qreal top = Height - sideLength-offset;// * 1.5;
	qreal bottom = top + sideLength;

	QColor color(PrefsManager::instance()->appPrefs.displayPrefs.frameNormColor);
	if ((isBookmark) || (m_isAnnotation))
		color = PrefsManager::instance()->appPrefs.displayPrefs.frameAnnotationColor;
	if ((BackBox != 0) || (NextBox != 0))
		color = PrefsManager::instance()->appPrefs.displayPrefs.frameLinkColor;
	if (m_Locked)
		color = PrefsManager::instance()->appPrefs.displayPrefs.frameLockColor;
	if (m_Doc->m_Selection->containsItem(this))
		color = Qt::red;

	p->save();

	p->setPen(color, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->drawSharpRect(left, top, sideLength, sideLength);
	p->drawSharpLine(QPointF(left, top), QPointF(right, bottom));
	p->drawSharpLine(QPointF(left, bottom), QPointF(right, top));

	p->restore();
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

bool PageItem::testLinkCandidate(PageItem* nxt)
{
	if (this->nextInChain() )
		return false;
	if (nxt->prevInChain() )
		return false;
	for (PageItem* ff=nxt; ff; ff=ff->nextInChain())
	{
		if (ff == this) return false;
	}
	return true;
}

void PageItem::link(PageItem* nxt, bool addPARSEP)
{
	assert( !nextInChain() );
	assert( !nxt->prevInChain() );
	for (PageItem* ff=nxt; ff; ff=ff->nextInChain())
	{
		assert (ff != this);
	}
	// Append only if necessary to avoid the
	// charstyle: access at end of text warning
	bool first = false;
	bool createUndo = addPARSEP;

	if (nxt->prevInChain() == NULL)
		first = true;
	int textLen = itemText.length();
	if (nxt->itemText.length() > 0)
	{   //case when text will be joined with next frame text
		//do not add PARSEP if first frame has no text or text ends already with PARSEP
		if (addPARSEP && (textLen > 0) && (itemText.text(textLen-1) != SpecialChars::PARSEP))
			itemText.insertChars(textLen, SpecialChars::PARSEP);
		else
			addPARSEP = false;
		itemText.append(nxt->itemText);
	}
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
	if (UndoManager::undoEnabled() && createUndo) //addPARESEP is false only if linking is invoked from undo action for unlinkWithText
	{
		ScItemState<QPair<PageItem*, PageItem*> > *is = new ScItemState<QPair<PageItem*, PageItem*> >(Um::LinkTextFrame);
		is->set("LINK_TEXT_FRAME", "linkTextFrame");
		is->set("FIRST", first);
		is->set("JOIN_POS", textLen);
		is->set("ADDPARSEP", addPARSEP);
		is->setItem(qMakePair(this, NextBox));
		undoManager->action(this, is);
	}
}

void PageItem::unlink(bool createUndo)
{
	if( NextBox )
	{
		PageItem *undoNextBox=NextBox;
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
		if (UndoManager::undoEnabled() && createUndo)
		{
			ScItemState<QPair<PageItem*, PageItem*> > *is = new ScItemState<QPair<PageItem*, PageItem*> >(Um::UnlinkTextFrame);
			is->set("UNLINK_TEXT_FRAME", "unlinkTextFrame");
			is->setItem(qMakePair(this, undoNextBox));
			undoManager->action(this, is);
		}
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

//unlink selected frame from text chain
//but copy or cut its content from itemText
void PageItem::unlinkWithText(bool cutText)
{
	PageItem * Next = NextBox;
	PageItem * Prev = BackBox;
	int length = itemText.length();

	//unlink first frame in chain
	if (Prev == NULL)
	{
		if (lastInFrame() < length -1)
		{
			StoryText content(m_Doc);
			itemText.select(lastInFrame() +1, length - (lastInFrame() +1));
			content.insert(0, itemText, cutText);
			itemText.removeSelection();
			unlink(false);
			Next->itemText.insert(0, content);
			Next->update();
		}
		else
		{
			unlink(false);
			if (!cutText)
			{
				Next->itemText.insert(0, itemText);
				Next->update();
			}
		}
	}
	else
	{
		itemText.select(firstInFrame(), length - firstInFrame());
		StoryText content(m_Doc);
		content.insert(0, itemText, true);
		if (cutText)
			itemText.removeSelection();
		Prev->unlink(false);
		itemText.insert(0, content);
		update();
	}
	if (UndoManager::undoEnabled())
	{
		ScItemState<QPair<PageItem*, PageItem*> > *is = new ScItemState<QPair<PageItem*, PageItem*> >(Um::UnlinkTextFrame);
		is->set("UNLINK_TEXT_FRAME", "unlinkTextFrame");
		is->set("CUT_TEXT", cutText);
		is->setItem(qMakePair(Prev, Next));
		undoManager->action(this, is);
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
	if (frameDisplays(itemText.cursorPosition()))
		return itemText.paragraphStyle(itemText.cursorPosition());
	else
		return itemText.defaultStyle();
}

/// returns the style at the current charpos for changing
ParagraphStyle& PageItem::changeCurrentStyle()
{
	if (frameDisplays(itemText.cursorPosition()))
		return const_cast<ParagraphStyle&>(itemText.paragraphStyle(itemText.cursorPosition()));
	else
		return const_cast<ParagraphStyle&>(itemText.defaultStyle());
}

/// returns the style at the current charpos
const CharStyle& PageItem::currentCharStyle() const
{
	if (frameDisplays(itemText.cursorPosition()))
		return itemText.charStyle(itemText.cursorPosition());
	else
		return itemText.defaultStyle().charStyle();
}

void PageItem::currentTextProps(ParagraphStyle& parStyle) const
{
	parStyle = this->currentStyle();
	int position = itemText.cursorPosition();
	if (itemText.lengthOfSelection() > 0)
		position = qMin(qMax(itemText.endOfSelection() - 1, 0), qMax(position, itemText.startOfSelection()));

	if (frameDisplays(position))
		parStyle.charStyle().setStyle( itemText.charStyle(position) );
	else
		parStyle.charStyle().setStyle( itemText.defaultStyle().charStyle() );
}

void PageItem::setTextToFrameDistLeft(double newLeft)
{
	if(Extra==newLeft)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TextFrameDist, "", Um::ITextFrame);
		ss->set("LEFT_TEXTFRAMEDIST", "left_textframedist");
		ss->set("OLD_DIST",Extra);
		ss->set("NEW_DIST",newLeft);
		undoManager->action(this, ss);
	}
	Extra=newLeft;
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistRight(double newRight)
{
	if(RExtra==newRight)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TextFrameDist, "", Um::ITextFrame);
		ss->set("RIGHT_TEXTFRAMEDIST", "right_textframedist");
		ss->set("OLD_DIST",RExtra);
		ss->set("NEW_DIST",newRight);
		undoManager->action(this, ss);
	}
	RExtra=newRight;
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistTop(double newTop)
{
	if(TExtra==newTop)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TextFrameDist, "", Um::ITextFrame);
		ss->set("TOP_TEXTFRAMEDIST", "top_textframedist");
		ss->set("OLD_DIST",TExtra);
		ss->set("NEW_DIST",newTop);
		undoManager->action(this, ss);
	}
	TExtra=newTop;
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistBottom(double newBottom)
{
	if(BExtra==newBottom)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TextFrameDist, "", Um::ITextFrame);
		ss->set("BOTTOM_TEXTFRAMEDIST", "bottom_textframedist");
		ss->set("OLD_DIST",BExtra);
		ss->set("NEW_DIST",newBottom);
		undoManager->action(this, ss);
	}
	BExtra=newBottom;
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDist(double newLeft, double newRight, double newTop, double newBottom)
{
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::TextFrame, Um::IDocument, Um::TextFrameDist, "", Um::ITextFrame));
	setTextToFrameDistLeft(newLeft);
	setTextToFrameDistRight(newRight);
	setTextToFrameDistTop(newTop);
	setTextToFrameDistBottom(newBottom);
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

double PageItem::gridOffset() const { return m_Doc->guidesPrefs().offsetBaselineGrid; }
double PageItem::gridDistance() const { return m_Doc->guidesPrefs().valueBaselineGrid; }

void PageItem::setGridOffset(double) { } // FIXME
void PageItem::setGridDistance(double) { } // FIXME
void PageItem::setColumns(int n) 
{
	if(Cols==n)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Columns, "", Um::IBorder);
		ss->set("COLUMNS", "columns");
		ss->set("OLD_COLUMNS", Cols);
		ss->set("NEW_COLUMNS",n);
		undoManager->action(this, ss);
	}
	Cols = qMax(1, n);
}
void PageItem::setColumnGap(double gap)
{
	if(ColGap==gap)
		return;
	if(UndoManager::undoEnabled()){
		SimpleState *ss = new SimpleState(Um::Columns, "", Um::IBorder);
		ss->set("COLUMNSGAP", "columnsgap");
		ss->set("OLD_COLUMNS", ColGap);
		ss->set("NEW_COLUMNS",gap);
		undoManager->action(this, ss);
	}
	ColGap = gap;
}

void PageItem::setCornerRadius(double newRadius)
{
	if(RadRect==newRadius)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *state = new SimpleState(Um::RoundCorner,"",Um::IBorder);
		state->set("CORNER_RADIUS","corner_radius");
		state->set("OLD_RADIUS",RadRect);
		state->set("NEW_RADIUS",newRadius);
		undoManager->action(this,state);
	}
	RadRect=newRadius;
	//emit cornerRadius(RadRect);
}



/** Paints the item.
    CHANGE: cullingArea is in doc coordinates!
 */
void PageItem::DrawObj(ScPainter *p, QRectF cullingArea)
{
	if (!m_Doc->DoDrawing)
		return;
	if (cullingArea.isNull())
	{
		cullingArea = QRectF(QPointF(m_Doc->minCanvasCoordinate.x(), m_Doc->minCanvasCoordinate.y()), 
							 QPointF(m_Doc->maxCanvasCoordinate.x(), m_Doc->maxCanvasCoordinate.y())).toAlignedRect();
	}

	no_fill = false;
	no_stroke = false;
	DrawObj_Pre(p);
	if (m_Doc->layerOutline(LayerID))
	{
		if ((itemType()==TextFrame || itemType()==ImageFrame || itemType()==PathText || itemType()==Line || itemType()==PolyLine || itemType()==Group || itemType()==Symbol))
			DrawObj_Item(p, cullingArea);
	}
	else
	{
		DrawObj_Item(p, cullingArea);
	}
	DrawObj_Post(p);
}

void PageItem::DrawObj_Pre(ScPainter *p)
{
	p->save();
	if (!isEmbedded)
		p->translate(Xpos, Ypos);
	p->rotate(Rot);
	if (m_Doc->layerOutline(LayerID))
	{
		p->setPen(m_Doc->layerMarker(LayerID), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::None);
		p->setBrushOpacity(1.0);
		p->setPenOpacity(1.0);
	}
	else
	{
		if (!isGroup())
		{
	#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
			p->setBlendModeFill(fillBlendmode());
	#else
			if (fillBlendmode() != 0)
				p->beginLayer(1.0 - fillTransparency(), fillBlendmode());
	#endif
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
						{
							no_fill = true;
							p->setFillMode(ScPainter::None);
						}
						if ((!patternVal.isEmpty()) && (!m_Doc->docPatterns.contains(patternVal)))
						{
							GrType = 0;
							patternVal = "";
						}
					}
					else
					{
						p->setPattern(&m_Doc->docPatterns[patternVal], patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternMirrorX, patternMirrorY);
						p->setFillMode(ScPainter::Pattern);
					}
				}
				else
				{
					if (GrType == 9)
					{
						p->setFillMode(ScPainter::Gradient);
						FPoint pG1 = FPoint(0, 0);
						FPoint pG2 = FPoint(width(), 0);
						FPoint pG3 = FPoint(width(), height());
						FPoint pG4 = FPoint(0, height());
						p->set4ColorGeometry(pG1, pG2, pG3, pG4, GrControl1, GrControl2, GrControl3, GrControl4);
						p->set4ColorColors(GrColorP1QColor, GrColorP2QColor, GrColorP3QColor, GrColorP4QColor);
					}
					else
					{
						if ((!gradientVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientVal)))
							gradientVal = "";
						if (!(gradientVal.isEmpty()) && (m_Doc->docGradients.contains(gradientVal)))
							fill_gradient = m_Doc->docGradients[gradientVal];
						if ((fill_gradient.Stops() < 2) && (GrType < 9)) // fall back to solid filling if there are not enough colorstops in the gradient.
						{
							if (fillColor() != CommonStrings::None)
							{
								p->setBrush(fillQColor);
								p->setFillMode(ScPainter::Solid);
							}
							else
							{
								no_fill = true;
								p->setFillMode(ScPainter::None);
							}
						}
						else
						{
							p->setFillMode(ScPainter::Gradient);
							p->fill_gradient = fill_gradient;
							switch (GrType)
							{
								case 1:
								case 2:
								case 3:
								case 4:
								case 6:
									p->setGradient(VGradient::linear, FPoint(GrStartX, GrStartY), FPoint(GrEndX, GrEndY), FPoint(GrStartX, GrStartY), GrScale, GrSkew);
									break;
								case 5:
								case 7:
									p->setGradient(VGradient::radial, FPoint(GrStartX, GrStartY), FPoint(GrEndX, GrEndY), FPoint(GrFocalX, GrFocalY), GrScale, GrSkew);
									break;
								case 10:
									p->setFillMode(ScPainter::Gradient);
									p->setDiamondGeometry(FPoint(0, 0), FPoint(width(), 0), FPoint(width(), height()), FPoint(0, height()), GrControl1, GrControl2, GrControl3, GrControl4, GrControl5);
									break;
								case 11:
								case 13:
									p->setFillMode(ScPainter::Gradient);
									p->setMeshGradient(FPoint(0, 0), FPoint(width(), 0), FPoint(width(), height()), FPoint(0, height()), meshGradientArray);
									break;
								case 12:
									p->setFillMode(ScPainter::Gradient);
									p->setMeshGradient(FPoint(0, 0), FPoint(width(), 0), FPoint(width(), height()), FPoint(0, height()), meshGradientPatches);
									break;
							}
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
				{
					no_fill = true;
					p->setFillMode(ScPainter::None);
				}
			}
			if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
			{
				p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
				if (DashValues.count() != 0)
					p->setDash(DashValues, DashOffset);
			}
			else
				p->setLineWidth(0);
	#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
			p->setBrushOpacity(1.0 - fillTransparency());
			p->setPenOpacity(1.0 - lineTransparency());
	#else
			if (fillBlendmode() == 0)
				p->setBrushOpacity(1.0 - fillTransparency());
			if (lineBlendmode() == 0)
				p->setPenOpacity(1.0 - lineTransparency());
	#endif
			p->setFillRule(fillRule);
			if ((GrMask == 1) || (GrMask == 2) || (GrMask == 4) || (GrMask == 5))
			{
				if ((GrMask == 1) || (GrMask == 2))
					p->setMaskMode(1);
				else
					p->setMaskMode(3);
				if ((!gradientMaskVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientMaskVal)))
					gradientMaskVal = "";
				if (!(gradientMaskVal.isEmpty()) && (m_Doc->docGradients.contains(gradientMaskVal)))
					mask_gradient = m_Doc->docGradients[gradientMaskVal];
				p->mask_gradient = mask_gradient;
				if ((GrMask == 1) || (GrMask == 4))
					p->setGradientMask(VGradient::linear, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskStartX, GrMaskStartY), GrMaskScale, GrMaskSkew);
				else
					p->setGradientMask(VGradient::radial, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskFocalX, GrMaskFocalY), GrMaskScale, GrMaskSkew);
			}
			else if ((GrMask == 3) || (GrMask == 6) || (GrMask == 7) || (GrMask == 8))
			{
				if ((patternMaskVal.isEmpty()) || (!m_Doc->docPatterns.contains(patternMaskVal)))
					p->setMaskMode(0);
				else
				{
					p->setPatternMask(&m_Doc->docPatterns[patternMaskVal], patternMaskScaleX, patternMaskScaleY, patternMaskOffsetX, patternMaskOffsetY, patternMaskRotation, patternMaskSkewX, patternMaskSkewY, patternMaskMirrorX, patternMaskMirrorY);
					if (GrMask == 3)
						p->setMaskMode(2);
					else if (GrMask == 6)
						p->setMaskMode(4);
					else if (GrMask == 7)
						p->setMaskMode(5);
					else
						p->setMaskMode(6);
				}
			}
			else
				p->setMaskMode(0);
		}
	}
}

void PageItem::DrawObj_Post(ScPainter *p)
{
	bool doStroke=true;
	if (m_Doc->layerOutline(LayerID))
	{
		if (itemType()!=Line)
		{
			p->setPen(m_Doc->layerMarker(LayerID), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
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
		if (!isGroup())
		{
#if (CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 9, 4))
			if (fillBlendmode() != 0)
				p->endLayer();
#else
			p->setBlendModeFill(0);
#endif
			p->setMaskMode(0);
			// TODO: Investigate whether itemType()==Table should really be here. I got artifacts without it so keeping it here for now. /estan
			if (itemType()==PathText || itemType()==PolyLine || itemType()==Spiral || itemType()==Line || itemType()==Symbol || itemType()==Group || itemType()==Table)
				doStroke=false;
			if ((doStroke) && (!m_Doc->RePos))
			{
#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
				p->setBlendModeStroke(lineBlendmode());
				p->setPenOpacity(1.0 - lineTransparency());
#else
				if (lineBlendmode() != 0)
					p->beginLayer(1.0 - lineTransparency(), lineBlendmode());
#endif
				if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
				{
					p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
					if (DashValues.count() != 0)
						p->setDash(DashValues, DashOffset);
				}
				else
					p->setLineWidth(0);
				if ((itemType() == LatexFrame) || (itemType() == ImageFrame) || (itemType() == OSGFrame))
					p->setupPolygon(&PoLine);
				if (NamedLStyle.isEmpty())
				{
					if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)))
					{
						if (patternStrokePath)
						{
							QPainterPath guidePath = PoLine.toQPainterPath(false);
							DrawStrokePattern(p, guidePath);
						}
						else
						{
							p->setPattern(&m_Doc->docPatterns[patternStrokeVal], patternStrokeScaleX, patternStrokeScaleY, patternStrokeOffsetX, patternStrokeOffsetY, patternStrokeRotation, patternStrokeSkewX, patternStrokeSkewY, patternStrokeMirrorX, patternStrokeMirrorY);
							p->setStrokeMode(ScPainter::Pattern);
							p->strokePath();
						}
					}
					else if (GrTypeStroke > 0)
					{
						if ((!gradientStrokeVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientStrokeVal)))
							gradientStrokeVal = "";
						if (!(gradientStrokeVal.isEmpty()) && (m_Doc->docGradients.contains(gradientStrokeVal)))
							stroke_gradient = m_Doc->docGradients[gradientStrokeVal];
						if (stroke_gradient.Stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
						{
							if (lineColor() != CommonStrings::None)
							{
								p->setBrush(strokeQColor);
								p->setStrokeMode(ScPainter::Solid);
							}
							else
							{
								no_stroke = true;
								p->setStrokeMode(ScPainter::None);
							}
						}
						else
						{
							p->setStrokeMode(ScPainter::Gradient);
							p->stroke_gradient = stroke_gradient;
							if (GrTypeStroke == 6)
								p->setGradient(VGradient::linear, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeStartX, GrStrokeStartY), GrStrokeScale, GrStrokeSkew);
							else
								p->setGradient(VGradient::radial, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeFocalX, GrStrokeFocalY), GrStrokeScale, GrStrokeSkew);
						}
						p->strokePath();
					}
					else if (lineColor() != CommonStrings::None)
					{
						p->setStrokeMode(ScPainter::Solid);
						p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
						if (DashValues.count() != 0)
							p->setDash(DashValues, DashOffset);
						p->strokePath();
					}
					else
						no_stroke = true;
				}
				else
				{
					p->setStrokeMode(ScPainter::Solid);
					multiLine ml = m_Doc->MLineStyles[NamedLStyle];
					QColor tmp;
					for (int it = ml.size()-1; it > -1; it--)
					{
						struct SingleLine& sl = ml[it];
						// Qt4 if ((!sl.Color != CommonStrings::None) && (sl.Width != 0))
						if (sl.Color != CommonStrings::None) // && (sl.Width != 0))
						{
							SetQColor(&tmp, sl.Color, sl.Shade);
							p->setPen(tmp, sl.Width, static_cast<Qt::PenStyle>(sl.Dash), static_cast<Qt::PenCapStyle>(sl.LineEnd), static_cast<Qt::PenJoinStyle>(sl.LineJoin));
							p->strokePath();
						}
					}
				}
#if (CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 9, 4))
				if (lineBlendmode() != 0)
					p->endLayer();
#else
				p->setBlendModeStroke(0);
#endif
			}
		}
	}
	p->setFillMode(ScPainter::Solid);
	p->setBlendModeFill(0);
	p->setStrokeMode(ScPainter::Solid);
	p->setBlendModeStroke(0);
	p->restore();
}

void PageItem::DrawObj_Decoration(ScPainter *p)
{
	p->save();
	if (!isEmbedded)
		p->translate(Xpos, Ypos);
	p->rotate(Rot);
	if ((!isEmbedded) && (!m_Doc->RePos))
	{
		double scpInv = 0;
		if (!isGroup())
		{
			if ((Frame) && (m_Doc->guidesPrefs().framesShown) && ((itemType() == ImageFrame) || (itemType() == LatexFrame) || (itemType() == OSGFrame) || (itemType() == PathText)))
			{
				p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if ((isBookmark) || (m_isAnnotation))
					p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameAnnotationColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if ((BackBox != 0) || (NextBox != 0))
					p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameLinkColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if (m_Locked)
					p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
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
// Ugly Hack to fix rendering problems with cairo >=1.5.10 && <1.8.0 follows
	#if ((CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 5, 10)) && (CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 8, 0)))
					p->setupSharpPolygon(&PoLine, false);
	#else
					p->setupSharpPolygon(&PoLine);
	#endif
				p->strokePath();
			}
		}
		if ((m_Doc->guidesPrefs().framesShown) && textFlowUsesContourLine() && (ContourLine.size() != 0))
		{
			p->setPen(Qt::darkGray, 0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
// Ugly Hack to fix rendering problems with cairo >=1.5.10 && <1.8.0 follows
	#if ((CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 5, 10)) && (CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 8, 0)))
			p->setupSharpPolygon(&ContourLine, false);
	#else
			p->setupSharpPolygon(&ContourLine);
	#endif
			p->strokePath();
		}
		if (itemType()==ImageFrame)
		{
			double minres = m_Doc->checkerProfiles()[m_Doc->curCheckProfile()].minResolution;
			double maxres = m_Doc->checkerProfiles()[m_Doc->curCheckProfile()].maxResolution;
			bool checkres = m_Doc->checkerProfiles()[m_Doc->curCheckProfile()].checkResolution;
			if  ((((72.0 / imageXScale()) < minres)
				|| ((72.0 / imageYScale()) < minres) 
				|| ((72.0 / imageXScale()) > maxres) 
				|| ((72.0 / imageYScale()) > maxres)) 
				&& (isRaster) && (checkres) && (!m_Doc->drawAsPreview) && (PrefsManager::instance()->appPrefs.displayPrefs.showVerifierWarningsOnCanvas))
			{
				double ofx = Width - 22.0;
				double ofy = Height - 22.0;
				p->save();
				p->translate(ofx, ofy);
				QImage ico = loadIcon("22/dialog-warning.png").toImage();
				p->drawImage(&ico);
				p->restore();
			}
		}
		if ((m_Doc->guidesPrefs().layerMarkersShown) && (m_Doc->layerCount() > 1) && (!m_Doc->layerOutline(LayerID)) && (isGroup()) && (!m_Doc->drawAsPreview))
		{
			p->setPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setPenOpacity(1.0);
			p->setBrush(m_Doc->layerMarker(LayerID));
			p->setBrushOpacity(1.0);
			p->setFillMode(ScPainter::Solid);
			double ofwh = 10;
			double ofx = Width - ofwh/2;
			double ofy = Height - ofwh*3;
			p->drawSharpRect(ofx, ofy, ofwh, ofwh);
		}
		if (no_fill && no_stroke && m_Doc->guidesPrefs().framesShown)
		{
			p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if (m_Locked)
				p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setFillMode(ScPainter::None);
			p->drawSharpRect(0, 0, Width, Height);
			no_fill = false;
			no_stroke = false;
		}
		//CB disabled for now
		//if (m_Doc->m_Selection->findItem(this)!=-1)
		//	drawLockedMarker(p);
	}
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
	emG.clear();
	emG.append(cembedded);
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
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
		double pws = embedded->m_lineWidth;
		embedded->DrawObj_Pre(p);
		switch(embedded->itemType())
		{
			case ImageFrame:
			case TextFrame:
			case LatexFrame:
			case OSGFrame:
			case Polygon:
			case PathText:
			case Symbol:
			case Group:
			case RegularPolygon:
			case Arc:
			case Table:
				embedded->DrawObj_Item(p, cullingArea);
				break;
			case Line:
			case PolyLine:
			case Spiral:
				embedded->m_lineWidth = pws * qMin(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
				embedded->DrawObj_Item(p, cullingArea);
				break;
			default:
				break;
		}
		embedded->m_lineWidth = pws * qMin(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
		embedded->DrawObj_Post(p);
		embedded->Xpos = x;
		embedded->Ypos = y;
		p->restore();
		embedded->m_lineWidth = pws;
	}
}

void PageItem::DrawStrokePattern(ScPainter *p, QPainterPath &path)
{
	ScPattern pat = m_Doc->docPatterns[patternStrokeVal];
	double pLen = path.length() - ((pat.width / 2.0) * (patternStrokeScaleX / 100.0));
	double adv = pat.width * patternStrokeScaleX / 100.0 * patternStrokeSpace;
	double xpos = patternStrokeOffsetX * patternStrokeScaleX / 100.0;
	while (xpos < pLen)
	{
		double currPerc = path.percentAtLength(xpos);
		double currAngle = path.angleAtPercent(currPerc);
#if QT_VERSION  >= 0x040400
		if (currAngle <= 180.0)
			currAngle *= -1.0;
		else
			currAngle = 360.0 - currAngle;
#endif
		QPointF currPoint = path.pointAtPercent(currPerc);
		p->save();
		p->translate(currPoint.x(), currPoint.y());
		p->rotate(currAngle);
		QTransform savWM = p->worldMatrix();
		QTransform trans;
		trans.translate(0.0, patternStrokeOffsetY);
		trans.rotate(patternStrokeRotation);
		trans.shear(-patternStrokeSkewX, patternStrokeSkewY);
		trans.scale(patternStrokeScaleX / 100.0, patternStrokeScaleY / 100.0);
		trans.translate(-pat.width / 2.0, -pat.height / 2.0);
		if (patternStrokeMirrorX)
		{
			trans.translate(pat.width, 0);
			trans.scale(-1, 1);
		}
		if (patternStrokeMirrorY)
		{
			trans.translate(0, pat.height);
			trans.scale(1, -1);
		}
		trans *= savWM;
		p->setWorldMatrix(trans);
		for (int em = 0; em < pat.items.count(); ++em)
		{
			PageItem* embedded = pat.items.at(em);
			p->save();
			p->translate(embedded->gXpos, embedded->gYpos);
			embedded->isEmbedded = true;
			embedded->invalid = true;
			embedded->DrawObj(p, QRectF());
			embedded->isEmbedded = false;
			p->restore();
		}
		p->setWorldMatrix(savWM);
		xpos += adv;
		p->restore();
	}
	p->newPath();
}

QImage PageItem::DrawObj_toImage(double maxSize)
{
	bool isEmbedded_Old = isEmbedded;
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	double x1, x2, y1, y2;
	getVisualBoundingRect(&x1, &y1, &x2, &y2);
	minx = qMin(minx, x1);
	miny = qMin(miny, y1);
	maxx = qMax(maxx, x2);
	maxy = qMax(maxy, y2);
	double igXpos = xPos() - minx;
	double igYpos = yPos() - miny;
	double igWidth = maxx - minx;
	double igHeight = maxy - miny;
	double sc = maxSize / qMax(igWidth, igHeight);
	bool savedFlag = m_Doc->guidesPrefs().framesShown;
	m_Doc->guidesPrefs().framesShown = false;
	QImage retImg = QImage(qRound(igWidth * sc), qRound(igHeight * sc), QImage::Format_ARGB32_Premultiplied);
	retImg.fill( qRgba(0, 0, 0, 0) );
	ScPainter *painter = new ScPainter(&retImg, retImg.width(), retImg.height(), 1, 0);
	painter->setZoomFactor(sc);
	painter->save();
	painter->translate(igXpos, igYpos);
	isEmbedded = true;
	invalid = true;
	DrawObj(painter, QRectF());
	isEmbedded = false;
	painter->restore();
	painter->end();
	delete painter;
	m_Doc->guidesPrefs().framesShown = savedFlag;
	isEmbedded = isEmbedded_Old;
	return retImg;
}

QImage PageItem::DrawObj_toImage(QList<PageItem*> &emG, double scaling)
{
	bool isEmbedded_Old = isEmbedded;
	bool savedFlag = m_Doc->guidesPrefs().framesShown;
	m_Doc->guidesPrefs().framesShown = false;
	QImage retImg = QImage(qRound(gWidth * scaling), qRound(gHeight * scaling), QImage::Format_ARGB32_Premultiplied);
	retImg.fill( qRgba(0, 0, 0, 0) );
	ScPainter *painter = new ScPainter(&retImg, retImg.width(), retImg.height(), 1, 0);
	painter->setZoomFactor(scaling);
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		painter->save();
		painter->translate(embedded->gXpos, embedded->gYpos);
		embedded->isEmbedded = true;
		embedded->invalid = true;
		embedded->DrawObj(painter, QRectF());
		embedded->isEmbedded = false;
		painter->restore();
	}
	painter->end();
	delete painter;
	m_Doc->guidesPrefs().framesShown = savedFlag;
	isEmbedded = isEmbedded_Old;
	return retImg;
}

QString PageItem::ExpandToken(uint base)
{
	//uint zae = 0;
	QChar ch = itemText.text(base);
	QString chstr = ch;
	if (ch == SpecialChars::PAGENUMBER)
	{
		// compatibility mode: ignore subsequent pagenumber chars
		if (base > 0 && itemText.text(base-1) == SpecialChars::PAGENUMBER)
			return "";
		if ((!m_Doc->masterPageMode()) && (OwnPage != -1))
		{
			//CB Section numbering
			//chstr = out.arg(m_Doc->getSectionPageNumberForPageIndex(OwnPage), -(int)zae);
			chstr = QString("%1").arg(m_Doc->getSectionPageNumberForPageIndex(OwnPage),
							m_Doc->getSectionPageNumberWidthForPageIndex(OwnPage),
							m_Doc->getSectionPageNumberFillCharForPageIndex(OwnPage));
		}
		else
			return "#";
	}
	else if (ch == SpecialChars::PAGECOUNT)
	{
		if (!m_Doc->masterPageMode())
		{
			int key = m_Doc->getSectionKeyForPageIndex(OwnPage);
			if (key == -1)
				return "%";
			chstr = QString("%1").arg(getStringFromSequence(m_Doc->sections()[key].type, m_Doc->sections()[key].toindex - m_Doc->sections()[key].fromindex + 1));
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
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		*tmp = defect.convertDefect(*tmp, m_Doc->previewVisual);
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
	const ScFace font = style.font();
	double asce = font.ascent(style.fontSize() / 10.0);
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
		layout.glyph = font.char2CMap(chars[0].unicode());
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
			retval -= asce * m_Doc->typographicPrefs().valueSuperScript / 100.0;
			layout.yoffset -= asce * m_Doc->typographicPrefs().valueSuperScript / 100.0;
			layout.scaleV = layout.scaleH = qMax(m_Doc->typographicPrefs().scalingSuperScript / 100.0, 10.0 / style.fontSize());
		}
		else if (chst & ScStyle_Subscript)
		{
			retval += asce * m_Doc->typographicPrefs().valueSubScript / 100.0;
			layout.yoffset += asce * m_Doc->typographicPrefs().valueSubScript / 100.0;
			layout.scaleV = layout.scaleH = qMax(m_Doc->typographicPrefs().scalingSubScript / 100.0, 10.0 / style.fontSize());
		}
		else {
			layout.scaleV = layout.scaleH = 1.0;
		}
		layout.scaleH *= style.scaleH() / 1000.0;
		layout.scaleV *= style.scaleV() / 1000.0;
		if (chst & ScStyle_AllCaps)
		{
			layout.glyph = font.char2CMap(chars[0].toUpper().unicode());
		}
		if (chst & ScStyle_SmallCaps)
		{
			double smallcapsScale = m_Doc->typographicPrefs().valueSmallCaps / 100.0;
			QChar uc = chars[0].toUpper();
			if (uc != chars[0])
			{
				layout.glyph = font.char2CMap(chars[0].toUpper().unicode());
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
		uint replGlyph = font.char2CMap(QChar(' '));
		layout.xadvance = font.glyphWidth(replGlyph, style.fontSize() / 10) * layout.scaleH;
		layout.yadvance = font.glyphBBox(replGlyph, style.fontSize() / 10).ascent * layout.scaleV;
	}
	else if (layout.glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBHYPHEN.unicode())) {
		uint replGlyph = font.char2CMap(QChar('-'));
		layout.xadvance = font.glyphWidth(replGlyph, style.fontSize() / 10) * layout.scaleH;
		layout.yadvance = font.glyphBBox(replGlyph, style.fontSize() / 10).ascent * layout.scaleV;
	}
	else if (layout.glyph >= ScFace::CONTROL_GLYPHS) {
		layout.xadvance = 0;
		layout.yadvance = 0;
	}
	else */
	{
		layout.xadvance = font.glyphWidth(layout.glyph, style.fontSize() / 10) * layout.scaleH;
		layout.yadvance = font.glyphBBox(layout.glyph, style.fontSize() / 10).ascent * layout.scaleV;
	}
	if (layout.xadvance > 0)
		layout.xadvance += tracking;

	if (chars.length() > 1) {
		layout.grow();
		layoutGlyphs(style, chars.mid(1), *layout.more);
		layout.xadvance += font.glyphKerning(layout.glyph, layout.more->glyph, style.fontSize() / 10) * layout.scaleH;
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
	const ScFace font = style.font();
	if ((m_Doc->guidesPrefs().showControls) &&
		(glyph == font.char2CMap(QChar(' ')) || glyph >=  ScFace::CONTROL_GLYPHS))
	{
		bool stroke = false;
		if (glyph >=  ScFace::CONTROL_GLYPHS)
			glyph -= ScFace::CONTROL_GLYPHS;
		else
			glyph = 32;
		QTransform chma, chma4, chma5;
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
			points = font.glyphOutline(font.char2CMap(QChar('-')), style.fontSize() / 100);
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
					: PrefsManager::instance()->appPrefs.displayPrefs.controlCharColor);
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
		glyph = font.char2CMap(QChar(' '));
	else if (glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBHYPHEN.unicode()))
		glyph = font.char2CMap(QChar('-'));
	
	if (glyph >= ScFace::CONTROL_GLYPHS || (style.effects() & ScStyle_SuppressSpace)) {
//		qDebug("drawGlyphs: skipping %d", glyph);
		// all those are empty
		if (glyphs.more)
		{
			p->translate(glyphs.xadvance, 0);
			drawGlyphs(p, style, *glyphs.more);
		}			
		return;
	}
//	if (font.canRender(QChar(glyph)))
	{
		FPointArray gly = font.glyphOutline(glyph);
		// Do underlining first so you can get typographically correct
		// underlines when drawing a white outline
		if (((style.effects() & ScStyle_Underline) || ((style.effects() & ScStyle_UnderlineWords) && glyph != font.char2CMap(QChar(' ')))) && (style.strokeColor() != CommonStrings::None))
		{
			double st, lw;
			if ((style.underlineOffset() != -1) || (style.underlineWidth() != -1))
			{
				if (style.underlineOffset() != -1)
					st = (style.underlineOffset() / 1000.0) * (font.descent(style.fontSize() / 10.0));
				else
					st = font.underlinePos(style.fontSize() / 10.0);
				if (style.underlineWidth() != -1)
					lw = (style.underlineWidth() / 1000.0) * (style.fontSize() / 10.0);
				else
					lw = qMax(font.strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			else
			{
				st = font.underlinePos(style.fontSize() / 10.0);
				lw = qMax(font.strokeWidth(style.fontSize() / 10.0), 1.0);
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
//				qDebug() << QString("glyph 0: (%1,%2) * %3 %4 + %5").arg(glyphs.xoffset).arg(glyphs.yoffset).arg(glyphs.scaleH).arg(glyphs.scaleV).arg(glyphs.xadvance));
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
//			qDebug() << QString("drawglyphs: %1 (%2,%3) (%4,%5) scaled %6,%7 trans %8,%9")
//				   .arg(gly.size()).arg(a).arg(b).arg(c).arg(d)
//				   .arg(p->worldMatrix().m11()).arg(p->worldMatrix().m22()).arg(p->worldMatrix().dx()).arg(p->worldMatrix().dy());
			p->setupPolygon(&gly, true);
			if (m_Doc->layerOutline(LayerID))
			{
				p->save();
				p->setPen(m_Doc->layerMarker(LayerID), 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
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
				p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.controlCharColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->setLineWidth(style.fontSize() * glyphs.scaleV * style.outlineWidth() * 2 / 10000.0);
				p->strokePath();
			}
			else if ((font.isStroked()) && (style.strokeColor() != CommonStrings::None) && ((style.fontSize() * glyphs.scaleV * style.outlineWidth() / 10000.0) != 0))
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
//			qDebug() << "drawGlyphs: empty glyph" << glyph;
		}
		if ((style.effects() & ScStyle_Strikethrough) && (style.strokeColor() != CommonStrings::None))
		{
			double st, lw;
			if ((style.strikethruOffset() != -1) || (style.strikethruWidth() != -1))
			{
				if (style.strikethruOffset() != -1)
					st = (style.strikethruOffset() / 1000.0) * (font.ascent(style.fontSize() / 10.0));
				else
					st = font.strikeoutPos(style.fontSize() / 10.0);
				if (style.strikethruWidth() != -1)
					lw = (style.strikethruWidth() / 1000.0) * (style.fontSize() / 10.0);
				else
					lw = qMax(font.strokeWidth(style.fontSize() / 10.0), 1.0);
			}
			else
			{
				st = font.strikeoutPos(style.fontSize() / 10.0);
				lw = qMax(font.strokeWidth(style.fontSize() / 10.0), 1.0);
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
	if (Segments.count() != 0)
	{
		QList<uint>::Iterator it2end=Segments.end();
		uint FirstVal = 0;
		for (QList<uint>::Iterator it2 = Segments.begin(); it2 != it2end; ++it2)
		{
				p->drawPolygon(pts.constData() + FirstVal, (*it2)-FirstVal);
			FirstVal = (*it2);
		}
		p->drawPolygon(pts.constData() + FirstVal, pts.size() - FirstVal);
	}
	else
		p->drawPolygon(pts);
}

void PageItem::setItemName(const QString& newName)
{
	if (AnName == newName)
		return; // nothing to do -> return
	if (newName.isEmpty())
		return;
	QString oldName = AnName;
	AnName = generateUniqueCopyName(newName);
	AutoName=false;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Rename, QString(Um::FromTo).arg(AnName).arg(newName));
		ss->set("OLD_NAME", oldName);
		ss->set("NEW_NAME", newName);
		undoManager->action(this, ss);
	}
	setUName(AnName); // set the name for the UndoObject too
}

void PageItem::setGradient(const QString &newGradient)
{
	if (gradientVal != newGradient)
		gradientVal = newGradient;
}

void PageItem::setMaskGradient(VGradient grad){
	if(mask_gradient==grad)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<VGradient,VGradient> > *is = new ScItemState<QPair<VGradient,VGradient> >(Um::GradVal);
		is->set("MASK_GRAD","mask_grad");
		is->setItem(qMakePair(mask_gradient,grad));
		undoManager->action(this,is);
	}
	mask_gradient=grad;
}

void PageItem::setFillGradient(VGradient grad){
	if(fill_gradient==grad)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<VGradient,VGradient> > *is = new ScItemState<QPair<VGradient,VGradient> >(Um::GradVal);
		is->set("FILL_GRAD","fill_grad");
		is->setItem(qMakePair(fill_gradient,grad));
		undoManager->action(this,is);
	}
	fill_gradient=grad;
}

void PageItem::setStrokeGradient(VGradient grad){
	if(stroke_gradient==grad)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<VGradient,VGradient> > *is = new ScItemState<QPair<VGradient,VGradient> >(Um::GradVal);
		is->set("STROKE_GRAD","STROKE_grad");
		is->setItem(qMakePair(stroke_gradient,grad));
		undoManager->action(this,is);
	}
	stroke_gradient=grad;
}

void PageItem::setPattern(const QString &newPattern)
{
	if (patternVal != newPattern)
		patternVal = newPattern;
}

void PageItem::set4ColorGeometry(FPoint c1, FPoint c2, FPoint c3, FPoint c4)
{
	GrControl1 = c1;
	GrControl2 = c2;
	GrControl3 = c3;
	GrControl4 = c4;
}

void PageItem::setDiamondGeometry(FPoint c1, FPoint c2, FPoint c3, FPoint c4, FPoint c5)
{
	GrControl1 = c1;
	GrControl2 = c2;
	GrControl3 = c3;
	GrControl4 = c4;
	GrControl5 = c5;
}

void PageItem::set4ColorTransparency(double t1, double t2, double t3, double t4)
{
	UndoTransaction *trans = NULL;
	if(UndoManager::undoEnabled())
		trans = new UndoTransaction(undoManager->beginTransaction(Um::Selection,Um::IFill,Um::GradVal,"",Um::IFill));
	setGradientTransp1(t1);
	setGradientTransp2(t2);
	setGradientTransp3(t3);
	setGradientTransp4(t4);
	if(trans)
	{
		trans->commit();
		delete trans;
		trans = NULL;
	}
}

void PageItem::set4ColorShade(int t1, int t2, int t3, int t4)
{
	UndoTransaction *trans = NULL;
	if(UndoManager::undoEnabled())
		trans = new UndoTransaction(undoManager->beginTransaction(Um::Selection,Um::IFill,Um::GradVal,"",Um::IFill));
	setGradientShade1(t1);
	setGradientShade2(t2);
	setGradientShade3(t3);
	setGradientShade4(t4);
	if(trans)
	{
		trans->commit();
		delete trans;
		trans = NULL;
	}
}

void PageItem::set4ColorColors(QString col1, QString col2, QString col3, QString col4)
{
	QColor tmp;
	UndoTransaction *trans = NULL;
	if(UndoManager::undoEnabled())
		trans = new UndoTransaction(undoManager->beginTransaction(Um::Selection,Um::IFill,Um::GradVal,"",Um::IFill));
	setGradientCol1(col1);
	if (GrColorP1 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP1))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol1(m_Doc->itemToolPrefs().imageFillColor);
				case TextFrame:
				case PathText:
					setGradientCol1(m_Doc->itemToolPrefs().textFillColor);
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					setGradientCol1(m_Doc->itemToolPrefs().shapeFillColor);
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[GrColorP1];
		tmp = ScColorEngine::getShadeColorProof(col, m_Doc, GrCol1Shade);
		tmp.setAlphaF(GrCol1transp);
		setGradientColor1(tmp);
	}
	else
		setGradientColor1(QColor(255, 255, 255, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor1(defect.convertDefect(GrColorP1QColor, m_Doc->previewVisual));
	}
	setGradientCol2(col2);
	if (GrColorP2 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP2))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol2(m_Doc->itemToolPrefs().imageFillColor);
				case TextFrame:
				case PathText:
					setGradientCol2(m_Doc->itemToolPrefs().textFillColor);
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					setGradientCol2(m_Doc->itemToolPrefs().shapeFillColor);
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[GrColorP2];
		tmp = ScColorEngine::getShadeColorProof(col, m_Doc, GrCol2Shade);
		tmp.setAlphaF(GrCol2transp);
		setGradientColor2(tmp);
	}
	else
		setGradientColor2(QColor(255, 255, 255, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor2(defect.convertDefect(GrColorP2QColor, m_Doc->previewVisual));
	}
	setGradientCol3(col3);
	if (GrColorP3 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP3))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol3(m_Doc->itemToolPrefs().imageFillColor);
				case TextFrame:
				case PathText:
					setGradientCol3(m_Doc->itemToolPrefs().textFillColor);
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					setGradientCol3(m_Doc->itemToolPrefs().shapeFillColor);
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[GrColorP3];
		tmp = ScColorEngine::getShadeColorProof(col, m_Doc, GrCol3Shade);
		tmp.setAlphaF(GrCol3transp);
		setGradientColor3(tmp);
	}
	else
		setGradientColor3(QColor(255, 255, 255, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor3(defect.convertDefect(GrColorP3QColor, m_Doc->previewVisual));
	}
	setGradientCol4(col4);
	if (GrColorP4 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP4))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol4(m_Doc->itemToolPrefs().imageFillColor);
				case TextFrame:
				case PathText:
					setGradientCol4(m_Doc->itemToolPrefs().textFillColor);
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					setGradientCol4(m_Doc->itemToolPrefs().shapeFillColor);
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[GrColorP4];
		tmp = ScColorEngine::getShadeColorProof(col, m_Doc, GrCol4Shade);
		tmp.setAlphaF(GrCol4transp);
		setGradientColor4(tmp);
	}
	else
		setGradientColor4(QColor(255, 255, 255, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor4(defect.convertDefect(GrColorP4QColor, m_Doc->previewVisual));
	}
	if(trans)
	{
		trans->commit();
		delete trans;
		trans = NULL;
	}
}

void PageItem::get4ColorGeometry(FPoint &c1, FPoint &c2, FPoint &c3, FPoint &c4)
{
	c1 = GrControl1;
	c2 = GrControl2;
	c3 = GrControl3;
	c4 = GrControl4;
}

void PageItem::get4ColorTransparency(double &t1, double &t2, double &t3, double &t4)
{
	t1 = GrCol1transp;
	t2 = GrCol2transp;
	t3 = GrCol3transp;
	t4 = GrCol4transp;
}

void PageItem::get4ColorColors(QString &col1, QString &col2, QString &col3, QString &col4)
{
	col1 = GrColorP1;
	col2 = GrColorP2;
	col3 = GrColorP3;
	col4 = GrColorP4;
}

void PageItem::setMeshPointColor(int x, int y, QString color, int shade, double transparency, bool forPatch)
{
	QString MColor = color;
	QColor MQColor;
	if (MColor != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(MColor))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					MColor = m_Doc->itemToolPrefs().imageFillColor;
				case TextFrame:
				case PathText:
					MColor = m_Doc->itemToolPrefs().textFillColor;
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					MColor = m_Doc->itemToolPrefs().shapeFillColor;
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[MColor];
		MQColor = ScColorEngine::getShadeColorProof(col, m_Doc, shade);
		MQColor.setAlphaF(transparency);
	}
	else
		MQColor = QColor(255, 255, 255, 0);
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		MQColor = defect.convertDefect(MQColor, m_Doc->previewVisual);
	}
	meshPoint *mp;
	if (forPatch)
	{
		meshGradientPatch *patch = &meshGradientPatches[x];
		switch (y)
		{
			case 1:
				mp = &(patch->TL);
				break;
			case 2:
				mp = &(patch->TR);
				break;
			case 3:
				mp = &(patch->BR);
				break;
			case 4:
				mp = &(patch->BL);
				break;
		}

		UndoTransaction *trans = NULL;
		if(UndoManager::undoEnabled())
		{
			trans = new UndoTransaction(undoManager->beginTransaction(Um::Selection,Um::IFill,Um::GradVal,"",Um::IFill));
			ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradVal);
			ss->set("GRAD_MESH_COLOR","grad_mesh_color");
			ss->set("X",x);
			ss->set("Y",y);
			ss->set("PATCH",true);
			ss->set("OLD_COLOR_NAME",mp->colorName);
			ss->set("NEW_COLOR_NAME",MColor);
			ss->setItem(qMakePair(mp->color,MQColor));
			ss->set("OLD_SHADE",mp->shade);
			ss->set("NEW_SHADE",shade);
			ss->set("OLD_TRANSP",mp->transparency);
			ss->set("NEW_TRANSP",transparency);
			undoManager->action(this,ss);
		}
		mp->colorName = MColor;
		mp->color = MQColor;
		mp->shade = shade;
		mp->transparency = transparency;
		FPoint xx = mp->gridPoint;
		for (int col = 0; col < meshGradientPatches.count(); col++)
		{
			if (col != x)
			{
				if (meshGradientPatches[col].TL.gridPoint == xx)
				{
					mp = &(meshGradientPatches[col].TL);
					if(UndoManager::undoEnabled())
					{
						ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradVal);
						ss->set("GRAD_MESH_COLOR","grad_mesh_color");
						ss->set("X",col);
						ss->set("Y",1);
						ss->set("PATCH",true);
						ss->set("OLD_COLOR_NAME",mp->colorName);
						ss->set("NEW_COLOR_NAME",MColor);
						ss->setItem(qMakePair(mp->color,MQColor));
						ss->set("OLD_SHADE",mp->shade);
						ss->set("NEW_SHADE",shade);
						ss->set("OLD_TRANSP",mp->transparency);
						ss->set("NEW_TRANSP",transparency);
						undoManager->action(this,ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
				if (meshGradientPatches[col].TR.gridPoint == xx)
				{
					mp = &(meshGradientPatches[col].TR);
					if(UndoManager::undoEnabled())
					{
						ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradVal);
						ss->set("GRAD_MESH_COLOR","grad_mesh_color");
						ss->set("X",col);
						ss->set("Y",2);
						ss->set("PATCH",true);
						ss->set("OLD_COLOR_NAME",mp->colorName);
						ss->set("NEW_COLOR_NAME",MColor);
						ss->setItem(qMakePair(mp->color,MQColor));
						ss->set("OLD_SHADE",mp->shade);
						ss->set("NEW_SHADE",shade);
						ss->set("OLD_TRANSP",mp->transparency);
						ss->set("NEW_TRANSP",transparency);
						undoManager->action(this,ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
				if (meshGradientPatches[col].BR.gridPoint == xx)
				{
					mp = &(meshGradientPatches[col].BR);
					if(UndoManager::undoEnabled())
					{
						ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradVal);
						ss->set("GRAD_MESH_COLOR","grad_mesh_color");
						ss->set("X",col);
						ss->set("Y",3);
						ss->set("PATCH",true);
						ss->set("OLD_COLOR_NAME",mp->colorName);
						ss->set("NEW_COLOR_NAME",MColor);
						ss->setItem(qMakePair(mp->color,MQColor));
						ss->set("OLD_SHADE",mp->shade);
						ss->set("NEW_SHADE",shade);
						ss->set("OLD_TRANSP",mp->transparency);
						ss->set("NEW_TRANSP",transparency);
						undoManager->action(this,ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
				if (meshGradientPatches[col].BL.gridPoint == xx)
				{
					mp = &(meshGradientPatches[col].BL);
					if(UndoManager::undoEnabled())
					{
						ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradVal);
						ss->set("GRAD_MESH_COLOR","grad_mesh_color");
						ss->set("X",col);
						ss->set("Y",4);
						ss->set("PATCH",true);
						ss->set("OLD_COLOR_NAME",mp->colorName);
						ss->set("NEW_COLOR_NAME",MColor);
						ss->setItem(qMakePair(mp->color,MQColor));
						ss->set("OLD_SHADE",mp->shade);
						ss->set("NEW_SHADE",shade);
						ss->set("OLD_TRANSP",mp->transparency);
						ss->set("NEW_TRANSP",transparency);
						undoManager->action(this,ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
			}
		}
		if(trans)
		{
			trans->commit();
			delete trans;
			trans = NULL;
		}
	}
	else
	{
		if ((x > -1) && (y > -1))
		{
			mp = &(meshGradientArray[x][y]);
			if(UndoManager::undoEnabled())
			{
				ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradVal);
				ss->set("GRAD_MESH_COLOR","grad_mesh_color");
				ss->set("X",x);
				ss->set("Y",y);
				ss->set("PATCH",false);
				ss->set("OLD_COLOR_NAME",mp->colorName);
				ss->set("NEW_COLOR_NAME",MColor);
				ss->setItem(qMakePair(mp->color,MQColor));
				ss->set("OLD_SHADE",mp->shade);
				ss->set("NEW_SHADE",shade);
				ss->set("OLD_TRANSP",mp->transparency);
				ss->set("NEW_TRANSP",transparency);
				undoManager->action(this,ss);
			}
			mp->colorName = MColor;
			mp->color = MQColor;
			mp->shade = shade;
			mp->transparency = transparency;
		}
	}
}

void PageItem::createGradientMesh(int rows, int cols)
{
	QString MColor = fillColorVal;
	QColor MQColor;
	if (fillColorVal != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(fillColorVal))
		{
			switch(itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					MColor = m_Doc->itemToolPrefs().imageFillColor;
				case TextFrame:
				case PathText:
					MColor = m_Doc->itemToolPrefs().textFillColor;
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					MColor = m_Doc->itemToolPrefs().shapeFillColor;
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[MColor];
		MQColor = ScColorEngine::getShadeColorProof(col, m_Doc, 100);
		MQColor.setAlphaF(1.0);
	}
	else
		MQColor = QColor(255, 255, 255, 0);
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		MQColor = defect.convertDefect(MQColor, m_Doc->previewVisual);
	}
	if(UndoManager::undoEnabled())
	{
		ScItemState<QList<QList<meshPoint> > > *is = new ScItemState<QList<QList<meshPoint> > >(Um::MeshGradient);
		is->set("CREATE_MESH_GRAD","create_mesh_grad");
		is->set("ROW",rows);
		is->set("COL",cols);
		is->setItem(meshGradientArray);
		undoManager->action(this,is);
	}
	meshGradientArray.clear();
	double xoffs = Width / static_cast<double>(cols);
	double yoffs = Height / static_cast<double>(rows);
	for (int x = 0; x < rows + 1; x++)
	{
		QList<meshPoint> mgList;
		for (int y = 0; y < cols + 1; y++)
		{
			meshPoint mgP;
			mgP.resetTo(FPoint(y * xoffs, x * yoffs));
			mgP.transparency = 1.0;
			mgP.shade = 100;
			mgP.colorName = MColor;
			mgP.color = MQColor;
			mgList.append(mgP);
		}
		meshGradientArray.append(mgList);
	}
}

void PageItem::resetGradientMesh()
{
	int rows = meshGradientArray.count();
	int cols = meshGradientArray[0].count();
	if(UndoManager::undoEnabled())
	{
		ScItemState<QList<QList<meshPoint> > > *is = new ScItemState<QList<QList<meshPoint> > >(Um::ResetControlPoints);
		is->set("RESET_MESH_GRAD","reset_mesh_grad");
		is->setItem(meshGradientArray);
		undoManager->action(this,is);
	}
	double xoffs = Width / static_cast<double>(cols-1);
	double yoffs = Height / static_cast<double>(rows-1);
	for (int x = 0; x < rows; x++)
		for (int y = 0; y < cols; y++)
			meshGradientArray[x][y].resetTo(FPoint(y * xoffs, x * yoffs));
}

void PageItem::meshToShape()
{
	FPointArray Coords;
	Coords.resize(0);
	Coords.svgInit();
	int rows = meshGradientArray.count() - 1;
	int cols = meshGradientArray[0].count() - 1;
	QList<meshPoint> mpList;
	mpList = meshGradientArray[0];
	Coords.svgMoveTo(mpList[0].gridPoint.x(), mpList[0].gridPoint.y());
	for (int m = 0; m < mpList.count()-1; m++)
	{
		Coords.svgCurveToCubic(mpList[m].controlRight.x(), mpList[m].controlRight.y(),
								mpList[m+1].controlLeft.x(), mpList[m+1].controlLeft.y(),
								mpList[m+1].gridPoint.x(), mpList[m+1].gridPoint.y());
	}
	for (int m = 0; m < rows; m++)
	{
		Coords.svgCurveToCubic(meshGradientArray[m][cols].controlBottom.x(), meshGradientArray[m][cols].controlBottom.y(),
								meshGradientArray[m+1][cols].controlTop.x(), meshGradientArray[m+1][cols].controlTop.y(),
								meshGradientArray[m+1][cols].gridPoint.x(), meshGradientArray[m+1][cols].gridPoint.y());
	}
	mpList = meshGradientArray[rows];
	for (int m = cols; m > 0; m--)
	{
		Coords.svgCurveToCubic(mpList[m].controlLeft.x(), mpList[m].controlLeft.y(),
								mpList[m-1].controlRight.x(), mpList[m-1].controlRight.y(),
								mpList[m-1].gridPoint.x(), mpList[m-1].gridPoint.y());
	}
	for (int m = rows; m > 0; m--)
	{
		Coords.svgCurveToCubic(meshGradientArray[m][0].controlTop.x(), meshGradientArray[m][0].controlTop.y(),
								meshGradientArray[m-1][0].controlBottom.x(), meshGradientArray[m-1][0].controlBottom.y(),
								meshGradientArray[m-1][0].gridPoint.x(), meshGradientArray[m-1][0].gridPoint.y());
	}
	
	UndoTransaction *trans =  NULL;
	if(UndoManager::undoEnabled())
	{
		trans = new UndoTransaction(undoManager->beginTransaction(Um::Selection,Um::IFill,Um::ChangeMeshGradient,"",Um::IFill));
		ScItemState<QPair<QList<QList<meshPoint> >,FPointArray> > *ism = new ScItemState<QPair<QList<QList<meshPoint> >,FPointArray> >(Um::ChangeMeshGradient, "",Um::IFill);
		ism->set("MOVE_MESH_GRAD", "move_mesh_grad");
		ism->setItem(qMakePair(meshGradientArray,PoLine));
		ism->set("OLDB",OldB2);
		ism->set("OLDH",OldH2);
		ism->set("FRAME_TYPE",FrameType);
		undoManager->action(this, ism);
	}
	QList<QList<meshPoint> > meshGradientArrayOld = meshGradientArray;
	PoLine = Coords.copy();
	double oldX = Xpos;
	double oldY = Ypos;
	ClipEdited = true;
	FrameType = 3;
	FPoint wh = getMaxClipF(&PoLine);
	setWidthHeight(wh.x(),wh.y());
	m_Doc->AdjustItemSize(this);
	OldB2 = width();
	OldH2 = height();
	updateClip();
	meshGradientArray = meshGradientArrayOld;
	double dx = oldX - Xpos;
	double dy = oldY - Ypos;
	for (int x = 0; x < rows+1; x++)
		for (int y = 0; y < cols+1; y++)
			meshGradientArray[x][y].moveRel(dx, dy);
	if(UndoManager::undoEnabled())
	{
		ScItemState<FPointArray> *is = new ScItemState<FPointArray>(Um::ResetContourLine, "",Um::IBorder);
		is->set("RESET_CONTOUR", "reset_contour");
		is->setItem(ContourLine);
		undoManager->action(this, is);
	}
	ContourLine = PoLine.copy();
	if(trans)
	{
		trans->commit();
		delete trans;
		trans = NULL;
	}
}

void PageItem::createConicalMesh()
{
	VGradient gradient;
	gradient.clearStops();
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	double lastStop = -1.0;
	double actualStop = 0.0;
	bool   isFirst = true;
	for (uint cst = 0; cst < fill_gradient.Stops(); ++cst)
	{
		actualStop = cstops.at(cst)->rampPoint;
		if ((actualStop == lastStop) && (!isFirst))
			continue;
		if (isFirst)
		{
			if (actualStop != 0)
			{
				gradient.addStop(cstops.at(cst)->color, 0, 0, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
				if (actualStop <= 0.25)
					gradient.addStop(cstops.at(cst)->color, cstops.at(cst)->rampPoint, cstops.at(cst)->midPoint, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
				else
				{
					double actDist = 0.25;
					do
					{
						gradient.addStop(cstops.at(cst)->color, actDist, actDist, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
						actDist += 0.25;
					}
					while (actualStop > actDist);
					gradient.addStop(cstops.at(cst)->color, cstops.at(cst)->rampPoint, cstops.at(cst)->midPoint, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
				}
			}
			else
				gradient.addStop(cstops.at(cst)->color, cstops.at(cst)->rampPoint, cstops.at(cst)->midPoint, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
			isFirst = false;
			lastStop = actualStop;
			continue;
		}
		isFirst = false;
		if (actualStop - lastStop <= 0.25)
			gradient.addStop(cstops.at(cst)->color, cstops.at(cst)->rampPoint, cstops.at(cst)->midPoint, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
		else
		{
			double actDist = lastStop + 0.25;
			do
			{
				gradient.addStop(computeInBetweenStop(cstops.at(cst-1), cstops.at(cst), actDist));
				actDist += 0.25;
			}
			while (actualStop > actDist);
			gradient.addStop(cstops.at(cst)->color, cstops.at(cst)->rampPoint, cstops.at(cst)->midPoint, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
		}
		if ((cst == fill_gradient.Stops()-1) && (actualStop < 1.0))
		{
			double distToGo = 1.0 - actualStop;
			if (distToGo <= 0.25)
				gradient.addStop(cstops.at(cst)->color, 1.0, 1.0, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
			else
			{
				do
				{
					double actDist = actualStop + 0.25;
					gradient.addStop(cstops.at(cst)->color, actDist, actDist, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
					distToGo -= 0.25;
					actDist += 0.25;
				}
				while (distToGo >= 0.25);
				gradient.addStop(cstops.at(cst)->color, 1.0, 1.0, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
			}
			break;
		}
		lastStop = actualStop;
	}
	meshGradientArray.clear();
	QList<meshPoint> mgList1;
	QList<meshPoint> mgList2;
	meshPoint mgP1, mgP2, mgP3, mgP4;
	QPainterPath path;
	FPointArray arcPath;
	QLineF angLin = QLineF(QPointF(GrStartX, GrStartY), QPointF(GrEndX, GrEndY));
	double startAngle = angLin.angle();
	double radius = angLin.length();
	double radius2 = radius * 2.0;
	QList<VColorStop*> rstops = gradient.colorStops();
	double stepAngle = 360 * rstops.at(1)->rampPoint;
	path.arcMoveTo(-radius, -radius, radius2, radius2, 0);
	path.arcTo(-radius, -radius, radius2, radius2, 0, -stepAngle);
	QTransform mm;
	mm.rotate(-startAngle);
	path = mm.map(path);
	QTransform mr;
	mr.scale(1.0, GrScale);
	path = mr.map(path);
	arcPath.fromQPainterPath(path);
	arcPath.translate(GrStartX, GrStartY);
	QPointF foc = QPointF(GrFocalX, GrFocalY);
	QTransform qmatrix;
	qmatrix.translate(GrStartX, GrStartY);
	qmatrix.rotate(-startAngle);
	qmatrix.translate(0, GrStartY * (1.0 - GrScale));
	qmatrix.translate(-GrStartX, -GrStartY);
	qmatrix.scale(1, GrScale);
	foc = qmatrix.map(foc);
	mgP1.resetTo(arcPath.point(0));
	mgP1.controlRight = arcPath.point(1);
	mgP1.transparency = rstops.at(0)->opacity;
	mgP1.shade = rstops.at(0)->shade;
	mgP1.colorName = rstops.at(0)->name;
	mgP1.color = rstops.at(0)->color;
	mgP1.color.setAlphaF(mgP1.transparency);
	mgList1.append(mgP1);

	mgP2.resetTo(arcPath.point(2));
	mgP2.controlLeft = arcPath.point(3);
	mgP2.transparency = rstops.at(1)->opacity;
	mgP2.shade = rstops.at(1)->shade;
	mgP2.colorName = rstops.at(1)->name;
	mgP2.color = rstops.at(1)->color;
	mgP2.color.setAlphaF(mgP2.transparency);

	mgP3.resetTo(FPoint(foc.x(), foc.y()));
	mgP3.transparency = rstops.at(0)->opacity;
	mgP3.shade = rstops.at(0)->shade;
	mgP3.colorName = rstops.at(0)->name;
	mgP3.color = rstops.at(0)->color;
	mgP3.color.setAlphaF(mgP3.transparency);

	mgList2.append(mgP3);
	mgP3.transparency = rstops.at(1)->opacity;
	mgP3.shade = rstops.at(1)->shade;
	mgP3.colorName = rstops.at(1)->name;
	mgP3.color = rstops.at(1)->color;
	mgP3.color.setAlphaF(mgP3.transparency);
	mgList2.append(mgP3);
	startAngle -= stepAngle;
	for (uint rst = 2; rst < gradient.Stops(); ++rst)
	{
		stepAngle = 360 * (rstops.at(rst)->rampPoint - rstops.at(rst-1)->rampPoint);
		path = QPainterPath();
		arcPath.resize(0);
		path.arcMoveTo(-radius, -radius, radius2, radius2, 0);
		path.arcTo(-radius, -radius, radius2, radius2, 0, -stepAngle);
		QTransform mm;
		mm.rotate(-startAngle);
		QTransform mr;
		mr.scale(1.0, GrScale);
		path = mm.map(path);
		path = mr.map(path);
		arcPath.fromQPainterPath(path);
		arcPath.translate(GrStartX, GrStartY);
		mgP2.controlRight = arcPath.point(1);
		mgList1.append(mgP2);

		mgP2.resetTo(arcPath.point(2));
		mgP2.controlLeft = arcPath.point(3);
		mgP2.transparency = rstops.at(rst)->opacity;
		mgP2.shade = rstops.at(rst)->shade;
		mgP2.colorName = rstops.at(rst)->name;
		mgP2.color = rstops.at(rst)->color;
		mgP2.color.setAlphaF(mgP2.transparency);

		mgP3.transparency = rstops.at(rst)->opacity;
		mgP3.shade = rstops.at(rst)->shade;
		mgP3.colorName = rstops.at(rst)->name;
		mgP3.color = rstops.at(rst)->color;
		mgP3.color.setAlphaF(mgP3.transparency);
		mgList2.append(mgP3);
		startAngle -= stepAngle;
	}
	mgList1.append(mgP2);
	meshGradientArray.append(mgList1);
	meshGradientArray.append(mgList2);
}

VColorStop PageItem::computeInBetweenStop(VColorStop* last, VColorStop* actual, double t)
{
	double dist = actual->rampPoint - last->rampPoint;
	double perc = (t - last->rampPoint) / dist;
	double Rn = ((actual->color.redF() - last->color.redF()) * perc) + last->color.redF();
	double Gn = ((actual->color.greenF() - last->color.greenF()) * perc) + last->color.greenF();
	double Bn = ((actual->color.blueF() - last->color.blueF()) * perc) + last->color.blueF();
	QColor color;
	color.setRgbF(Rn, Gn, Bn);
	double opacity = ((actual->opacity - last->opacity) * perc) + last->opacity;
	int shade = qRound(((actual->shade - last->shade) * perc) + last->shade);
	QString name = last->name + actual->name + QString("%1").arg(perc);
	return VColorStop(t, t, color, opacity, name, shade);
}

void PageItem::gradientVector(double& startX, double& startY, double& endX, double& endY, double &focalX, double &focalY, double &scale, double &skew) const
{
	startX = GrStartX;
	startY = GrStartY;
	endX   = GrEndX;
	endY   = GrEndY;
	focalX = GrFocalX;
	focalY = GrFocalY;
	scale  = GrScale;
	skew   = GrSkew;
}

void PageItem::setGradientVector(double startX, double startY, double endX, double endY, double focalX, double focalY, double scale, double skew)
{
	GrStartX = startX;
	GrStartY = startY;
	GrEndX   = endX;
	GrEndY   = endY;
	GrFocalX = focalX;
	GrFocalY = focalY;
	GrScale  = scale;
	GrSkew   = skew;
}

void PageItem::setStrokeGradient(const QString &newGradient)
{
	if (gradientStrokeVal != newGradient)
		gradientStrokeVal = newGradient;
}

void PageItem::strokeGradientVector(double& startX, double& startY, double& endX, double& endY, double &focalX, double &focalY, double &scale, double &skew) const
{
	startX = GrStrokeStartX;
	startY = GrStrokeStartY;
	endX   = GrStrokeEndX;
	endY   = GrStrokeEndY;
	focalX = GrStrokeFocalX;
	focalY = GrStrokeFocalY;
	scale  = GrStrokeScale;
	skew   = GrStrokeSkew;
}

void PageItem::setStrokeGradientVector(double startX, double startY, double endX, double endY, double focalX, double focalY, double scale, double skew)
{
	GrStrokeStartX = startX;
	GrStrokeStartY = startY;
	GrStrokeEndX   = endX;
	GrStrokeEndY   = endY;
	GrStrokeFocalX = focalX;
	GrStrokeFocalY = focalY;
	GrStrokeScale  = scale;
	GrStrokeSkew   = skew;
}

void PageItem::setPatternTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY)
{
	patternScaleX = scaleX;
	patternScaleY = scaleY;
	patternOffsetX = offsetX;
	patternOffsetY = offsetY;
	patternRotation = rotation;
	patternSkewX = skewX;
	patternSkewY = skewY;
}

void PageItem::patternTransform(double &scaleX, double &scaleY, double &offsetX, double &offsetY, double &rotation, double &skewX, double &skewY) const
{
	 scaleX = patternScaleX;
	 scaleY = patternScaleY;
	 offsetX = patternOffsetX;
	 offsetY = patternOffsetY;
	 rotation = patternRotation;
	 skewX = patternSkewX;
	 skewY = patternSkewY;
}

void PageItem::setPatternFlip(bool flipX, bool flipY)
{
	patternMirrorX = flipX;
	patternMirrorY = flipY;
}

void PageItem::patternFlip(bool &flipX, bool &flipY)
{
	flipX = patternMirrorX;
	flipY = patternMirrorY;
}

void PageItem::setMaskType(int val)
{
	if(GrMask==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradTypeMask,"",Um::IFill);
		ss->set("MASKTYPE","masktype");
		ss->set("NEW",val);
		ss->set("OLD",GrMask);
		undoManager->action(this,ss);
	}
	GrMask = val;
}

void PageItem::setGradientMask(const QString &newMask)
{
	if (gradientMaskVal != newMask)
		gradientMaskVal = newMask;
}

void PageItem::setPatternMask(const QString &newMask)
{
	if (patternMaskVal != newMask)
		patternMaskVal = newMask;
}

void PageItem::maskVector(double& startX, double& startY, double& endX, double& endY, double &focalX, double &focalY, double &scale, double &skew) const
{
	startX = GrMaskStartX;
	startY = GrMaskStartY;
	endX   = GrMaskEndX;
	endY   = GrMaskEndY;
	focalX = GrMaskFocalX;
	focalY = GrMaskFocalY;
	scale  = GrMaskScale;
	skew   = GrMaskSkew;
}

void PageItem::setMaskVector(double startX, double startY, double endX, double endY, double focalX, double focalY, double scale, double skew)
{
	GrMaskStartX = startX;
	GrMaskStartY = startY;
	GrMaskEndX   = endX;
	GrMaskEndY   = endY;
	GrMaskFocalX = focalX;
	GrMaskFocalY = focalY;
	GrMaskScale  = scale;
	GrMaskSkew   = skew;
}

void PageItem::maskTransform(double &scaleX, double &scaleY, double &offsetX, double &offsetY, double &rotation, double &skewX, double &skewY) const
{
	 scaleX = patternMaskScaleX;
	 scaleY = patternMaskScaleY;
	 offsetX = patternMaskOffsetX;
	 offsetY = patternMaskOffsetY;
	 rotation = patternMaskRotation;
	 skewX = patternMaskSkewX;
	 skewY = patternMaskSkewY;
}

void PageItem::setMaskTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY)
{
	patternMaskScaleX = scaleX;
	patternMaskScaleY = scaleY;
	patternMaskOffsetX = offsetX;
	patternMaskOffsetY = offsetY;
	patternMaskRotation = rotation;
	patternMaskSkewX = skewX;
	patternMaskSkewY = skewY;
}

void PageItem::setMaskFlip(bool flipX, bool flipY)
{
	patternMaskMirrorX = flipX;
	patternMaskMirrorY = flipY;
}

void PageItem::maskFlip(bool &flipX, bool &flipY)
{
	flipX = patternMaskMirrorX;
	flipY = patternMaskMirrorY;
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
				case OSGFrame:
					tmp = m_Doc->itemToolPrefs().imageFillColor;
				case TextFrame:
				case PathText:
					tmp = m_Doc->itemToolPrefs().textFillColor;
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					tmp = m_Doc->itemToolPrefs().shapeFillColor;
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
	setFillQColor();
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
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::BlendMode, 0, Um::IGroup);
		ss->set("FILLBLENDMODE", newBlendmode);
		ss->set("FILLBLENDMODE_OLD", fillBlendmodeVal);
		undoManager->action(this, ss);
	}
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
					tmp = m_Doc->itemToolPrefs().textLineColor;
					break;
				case Line:
					tmp = m_Doc->itemToolPrefs().lineColor;
					break;
				case PolyLine:
				case Polygon:
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
				case RegularPolygon:
				case Arc:
				case Spiral:
					tmp = m_Doc->itemToolPrefs().shapeLineColor;
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
}

void PageItem::setStrokePattern(const QString &newPattern)
{
	if (patternStrokeVal != newPattern)
		patternStrokeVal = newPattern;
}

void PageItem::setStrokePatternToPath(bool enable)
{
	patternStrokePath = enable;
}

bool PageItem::isStrokePatternToPath()
{
	return patternStrokePath;
}

void PageItem::setStrokePatternTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, double space)
{
	patternStrokeScaleX = scaleX;
	patternStrokeScaleY = scaleY;
	patternStrokeOffsetX = offsetX;
	patternStrokeOffsetY = offsetY;
	patternStrokeRotation = rotation;
	patternStrokeSkewX = skewX;
	patternStrokeSkewY = skewY;
	patternStrokeSpace = space;
}

void PageItem::setStrokePatternFlip(bool flipX, bool flipY)
{
	patternStrokeMirrorX = flipX;
	patternStrokeMirrorY = flipY;
}

void PageItem::strokePatternFlip(bool &flipX, bool &flipY)
{
	flipX = patternStrokeMirrorX;
	flipY = patternStrokeMirrorY;
}

void PageItem::strokePatternTransform(double &scaleX, double &scaleY, double &offsetX, double &offsetY, double &rotation, double &skewX, double &skewY, double &space) const
{
	scaleX = patternStrokeScaleX;
	scaleY = patternStrokeScaleY;
	offsetX = patternStrokeOffsetX;
	offsetY = patternStrokeOffsetY;
	rotation = patternStrokeRotation;
	skewX = patternStrokeSkewX;
	skewY = patternStrokeSkewY;
	space = patternStrokeSpace;
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
					lineColorVal = m_Doc->itemToolPrefs().textLineColor;
					break;
				case Line:
					lineColorVal = m_Doc->itemToolPrefs().lineColor;
					break;
				case PolyLine:
				case Polygon:
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
				case RegularPolygon:
				case Arc:
				case Spiral:
					lineColorVal = m_Doc->itemToolPrefs().shapeLineColor;
					break;
				default:
					break;
			}
		}
		if (!m_Doc->PageColors.contains(lineColorVal))
			lineColorVal = m_Doc->itemToolPrefs().shapeLineColor;
		const ScColor& col = m_Doc->PageColors[lineColorVal];
		strokeQColor = ScColorEngine::getShadeColorProof(col, m_Doc, lineShadeVal);
	}
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		strokeQColor = defect.convertDefect(strokeQColor, m_Doc->previewVisual);
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
				case OSGFrame:
					fillColorVal = m_Doc->itemToolPrefs().imageFillColor;
				case TextFrame:
				case PathText:
					fillColorVal = m_Doc->itemToolPrefs().textFillColor;
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					fillColorVal = m_Doc->itemToolPrefs().shapeFillColor;
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[fillColorVal];
		fillQColor = ScColorEngine::getShadeColorProof(col, m_Doc, fillShadeVal);
	}
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		fillQColor = defect.convertDefect(fillQColor, m_Doc->previewVisual);
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
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::BlendMode, 0, Um::IGroup);
		ss->set("LINEBLENDMODE", newBlendmode);
		ss->set("LINEBLENDMODE_OLD", lineBlendmodeVal);
		undoManager->action(this, ss);
	}
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
	if ((m_lineWidth == newWidth) || (isGroup()))
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

void PageItem::setStartArrowScale(int newScale)
{
	if (m_startArrowScale == newScale)
		return; // nothing to do -> return
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::StartArrowScale,"",Um::IArrow);
		ss->set("START_ARROWSCALE","start_arrowscale");
		ss->set("OLD_SCALE",m_startArrowScale);
		ss->set("NEW_SCALE",newScale);
		undoManager->action(this,ss);
	}
	m_startArrowScale = newScale;
}

void PageItem::setEndArrowScale(int newScale)
{
	if (m_endArrowScale == newScale)
		return; // nothing to do -> return
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::EndArrowScale,"",Um::IArrow);
		ss->set("END_ARROWSCALE","end_arrowscale");
		ss->set("OLD_SCALE",m_endArrowScale);
		ss->set("NEW_SCALE",newScale);
		undoManager->action(this,ss);
	}
	m_endArrowScale = newScale;
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
		ss->set("SCALE_MODE", "scaling_mode");
		if (freeScale != ScaleType)
		{
			ss->set("SCALE_TYPE", freeScale);
			if (!freeScale)
			{
				//if switching from free scaling to frame size
				//in undo must be offset and scale saved
				ss->set("OLD_IMAGEXOFFSET", LocalX);
				ss->set("OLD_IMAGEYOFFSET", LocalY);
				ss->set("OLD_IMAGEXSCALE", LocalScX);
				ss->set("OLD_IMAGEYSCALE", LocalScY);
			}
		}
		if (keepRatio != AspectRatio)
			ss->set("ASPECT_RATIO", keepRatio);
		undoManager->action(this, ss);
	}
	ScaleType = freeScale;
	AspectRatio = keepRatio;
	AdjustPictScale();
	update();
}

void PageItem::setOverprint(bool val) {
	if(doOverprint==val)
			return;

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::Overprint, 0, Um::IGroup);
		ss->set("OVERPRINT", val);
		undoManager->action(this, ss);
	}
	doOverprint = val;
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
	//emit frameLocked(m_Locked);
}

void PageItem::setLocked(bool isLocked)
{
	if (isLocked != m_Locked)
		toggleLock();
}

QList<PageItem*> PageItem::getItemList()
{
	QList<PageItem*> ret;
	ret.clear();
	return ret;
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
	//emit frameSizeLocked(m_SizeLocked);
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
	
	checkTextFlowInteractions();
}

void PageItem::checkTextFlowInteractions(bool allItems)
{	
	if(!m_Doc->isLoading())
	{
		QRectF baseRect(getBoundingRect());
		QList<PageItem*>* items = OnMasterPage.isEmpty() ? &m_Doc->DocItems : &m_Doc->MasterItems;
		if (!allItems)
		{
			int ids = items->indexOf(this) - 1;
			for(int idx = ids; idx >= 0 ; --idx)
			{
				if(items->at(idx)->asTextFrame()) // do not bother with no text frames
				{
					QRectF uRect(items->at(idx)->getBoundingRect());
					if(baseRect.intersects(uRect))
					{
						items->at(idx)->update();
					}
				}
			}
		}
		else
		{
			for(int idx = items->count() - 1; idx >= 0 ; --idx)
			{
				if(items->at(idx) != this) // avoids itself
				{
					if(items->at(idx)->asTextFrame()) // do not bother with no text frames
					{
						QRectF uRect(items->at(idx)->getBoundingRect());
						if(baseRect.intersects(uRect))
						{
							items->at(idx)->update();
						}
					}
				}
			}
		}
	}
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
		case RegularPolygon:
		case Arc:
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
		case RegularPolygon:
		case Arc:
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

void PageItem::setLayer(int newLayerID)
{
	if (LayerID == newLayerID)
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SendToLayer,
										  QString(Um::FromTo).arg(LayerID).arg(newLayerID),
										  Um::ILayerAction);
		ss->set("SEND_TO_LAYER", "send_to_layer");
		ss->set("OLD_LAYER", LayerID);
		ss->set("NEW_LAYER", newLayerID);
		undoManager->action(this, ss);
	}
	LayerID = newLayerID;
}

void PageItem::checkChanges(bool force)
{
	bool spreadChanges(false);
	// has the item been resized
	if (force || ((oldWidth != Width || oldHeight != Height) && shouldCheck()))
	{
		resizeUndoAction();
		spreadChanges = (textFlowMode() != TextFlowDisabled );
	}
	// has the item been rotated
	if (force || ((oldRot != Rot) && (shouldCheck())))
	{
		rotateUndoAction();
		spreadChanges = (textFlowMode() != TextFlowDisabled );
	}
	// has the item been moved
	if (force || ((oldXpos != Xpos || oldYpos != Ypos) && shouldCheck()))
	{
		moveUndoAction();
		spreadChanges = (textFlowMode() != TextFlowDisabled );
	}
	// has the item's image been moved
	if (force || ((oldLocalX != LocalX || oldLocalY != LocalY) && shouldCheck()))
		changeImageOffsetUndoAction();
	// has the item's image been scaled
	if (force || ((oldLocalScX != LocalScX || oldLocalScY != LocalScY) && shouldCheck()))
		changeImageScaleUndoAction();
	
	if(spreadChanges)
	{
		checkTextFlowInteractions();
	}
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
		ss->set("IMAGE_OFFSET", "image_offset");
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
		ss->set("IMAGE_SCALE", "image_scale");
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
	bool SnapElementBackup = m_Doc->SnapElement;
	int dummy = 0;
	m_Doc->SnapElement = false;
	m_Doc->useRaster = false;
	m_Doc->SnapGuides = false;
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	bool oldMPMode=m_Doc->masterPageMode();
	m_Doc->setMasterPageMode(!OnMasterPage.isEmpty());
	ScPage *oldCurrentPage = m_Doc->currentPage();
	if (!OnMasterPage.isEmpty())
	{
		oldCurrentPage = m_Doc->currentPage();
		m_Doc->setCurrentPage(m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]));
	}
	if (ss)
	{
		bool actionFound = checkGradientUndoRedo(ss, isUndo);
		if (actionFound)
			dummy = 0;
		else if (ss->contains("ARC"))
			restoreArc(ss, isUndo);
		else if (ss->contains("MASKTYPE"))
			restoreMaskType(ss, isUndo);
		else if (ss->contains("POLYGON"))
			restorePolygon(ss, isUndo);
		else if (ss->contains("END_ARROWSCALE"))
			restoreEndArrowScale(ss, isUndo);
		else if (ss->contains("START_ARROWSCALE"))
			restoreStartArrowScale(ss, isUndo);
		else if (ss->contains("IMAGE_ROTATION"))
			restoreImageRotation(ss, isUndo);
		else if (ss->contains("OLD_HEIGHT"))
			restoreResize(ss, isUndo);
		else if (ss->contains("OLD_ROT"))
			restoreRotate(ss, isUndo);
		else if (ss->contains("OLD_XPOS"))
			restoreMove(ss, isUndo);
		else if (ss->contains("FILL"))
			restoreFill(ss, isUndo);
		else if (ss->contains("SHADE"))
			restoreShade(ss, isUndo);
		else if (ss->contains("LINE_COLOR"))
			restoreLineColor(ss, isUndo);
		else if (ss->contains("COLUMNS"))
			restoreColumns(ss, isUndo);
		else if (ss->contains("COLUMNSGAP"))
			restoreColumnsGap(ss, isUndo);
		else if (ss->contains("LINE_SHADE"))
			restoreLineShade(ss, isUndo);
		else if (ss->contains("DELETE_FRAMETEXT"))
			restoreDeleteFrameText(ss, isUndo);
		else if (ss->contains("INSERT_FRAMETEXT"))
			restoreInsertFrameText(ss,isUndo);
		else if (ss->contains("LOREM_FRAMETEXT"))
			restoreLoremIpsum(ss,isUndo);
		else if (ss->contains("APPLY_CHARSTYLE"))
			restoreCharStyle(ss,isUndo);
		else if (ss->contains("SET_CHARSTYLE"))
			restoreSetCharStyle(ss,isUndo);
		else if (ss->contains("SET_PARASTYLE"))
			restoreSetParagraphStyle(ss,isUndo);
		else if (ss->contains("APPLY_PARASTYLE"))
			restoreParagraphStyle(ss,isUndo);
		else if (ss->contains("APPLY_DEFAULTPARASTYLE"))
			restoreDefaultParagraphStyle(ss,isUndo);
		else if (ss->contains("LEFT_TEXTFRAMEDIST"))
			restoreLeftTextFrameDist(ss,isUndo);
		else if (ss->contains("RIGHT_TEXTFRAMEDIST"))
			restoreRightTextFrameDist(ss,isUndo);
		else if (ss->contains("TOP_TEXTFRAMEDIST"))
			restoreTopTextFrameDist(ss,isUndo);
		else if (ss->contains("BOTTOM_TEXTFRAMEDIST"))
			restoreBottomTextFrameDist(ss,isUndo);
		else if (ss->contains("FIRSTLINEOFFSET"))
			restoreFirstLineOffset(ss,isUndo);
		else if (ss->contains("PASTE_TEXT"))
			restorePasteText(ss,isUndo);
		else if (ss->contains("CORNER_RADIUS"))
			restoreCornerRadius(ss,isUndo);
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
		else if (ss->contains("OVERPRINT"))
		{
			if (isUndo)
				doOverprint=!ss->getBool("OVERPRINT");
			else
				doOverprint=ss->getBool("OVERPRINT");
		}
		else if (ss->contains("FILLBLENDMODE"))
		{
			if (isUndo)
				fillBlendmodeVal=ss->getInt("FILLBLENDMODE_OLD");
			else
				fillBlendmodeVal=ss->getInt("FILLBLENDMODE");
		}
		else if (ss->contains("ACTIONPDFANNOTATION"))
		{
			if (isUndo)
				m_isAnnotation=!ss->getBool("ACTIONPDFANNOTATION");
			else
				m_isAnnotation=ss->getBool("ACTIONPDFANNOTATION");
		}
		else if (ss->contains("ACTIONPDFBOOKMARK"))
		{
			if (isUndo)
				isBookmark=!ss->getBool("ACTIONPDFBOOKMARK");
			else
				isBookmark=ss->getBool("ACTIONPDFBOOKMARK");
		}
		else if (ss->contains("LINEBLENDMODE"))
		{
			if (isUndo)
				lineBlendmodeVal=ss->getInt("LINEBLENDMODE_OLD");
			else
				lineBlendmodeVal=ss->getInt("LINEBLENDMODE");
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
		else if (ss->contains("SHOW_IMAGE"))
			restoreShowImage(ss, isUndo);
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
		else if (ss->contains("CONVERT"))
			restoreType(ss, isUndo);
		else if (ss->contains("TEXTFLOW_OLDMODE"))
			restoreTextFlowing(ss, isUndo);
		else if (ss->contains("SCALE_MODE"))
			restoreImageScaleMode(ss, isUndo);
		else if (ss->contains("IMAGE_SCALE"))
			restoreImageScaleChange(ss, isUndo);
		else if (ss->contains("IMAGE_OFFSET"))
			restoreImageOffsetChange(ss, isUndo);	
		else if (ss->contains("EDIT_CONTOUR"))
			restorePoly(ss, isUndo, true);
		else if (ss->contains("EDIT_SHAPE"))
			restorePoly(ss, isUndo, false);
		else if (ss->contains("RES_TYP"))
			restoreResTyp(ss, isUndo);
		else if (ss->contains("RESET_CONTOUR"))
			restoreContourLine(ss, isUndo);
		else if (ss->contains("CHANGE_SHAPE_TYPE"))
			restoreShapeType(ss, isUndo);
		else if (ss->contains("UNITEITEM"))
			restoreUniteItem(ss, isUndo);
		else if (ss->contains("SPLITITEM"))
			restoreSplitItem(ss, isUndo);
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
		else if (ss->contains("LINK_TEXT_FRAME"))
			restoreLinkTextFrame(ss,isUndo);
		else if (ss->contains("UNLINK_TEXT_FRAME"))
			restoreUnlinkTextFrame(ss,isUndo);
		else if (ss->contains("REVERSE_TEXT"))
			restoreReverseText(ss, isUndo);
		else if (ss->contains("CLEAR_IMAGE"))
			restoreClearImage(ss,isUndo);
		else if (ss->contains("PASTE_INLINE"))
			restorePasteInline(ss,isUndo);
		else if (ss->contains("TRANSFORM"))
			restoreTransform(ss, isUndo);
		else if (ss->contains("PATH_OPERATION"))
			restorePathOperation(ss, isUndo);
		else if (ss->contains("IMAGE_NBR"))
			restoreImageNbr(ss, isUndo);
		else if (ss->contains("CHANGE_MODE"))
			restoreAppMode(ss, isUndo);
		else if (ss->contains("CONNECT_PATH"))
			restoreConnectPath(ss, isUndo);
	}
	if (!OnMasterPage.isEmpty())
		m_Doc->setCurrentPage(oldCurrentPage);
	m_Doc->setMasterPageMode(oldMPMode);
	m_Doc->SnapElement = SnapElementBackup;
	m_Doc->useRaster = useRasterBackup;
	m_Doc->SnapGuides = SnapGuidesBackup;
}

void PageItem::restoreConnectPath(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<FPointArray, FPointArray> > *is = dynamic_cast<ScItemState<QPair<FPointArray, FPointArray> > *>(state);
	if(isUndo)
	{
		PoLine = is->getItem().first;
		doc()->AdjustItemSize(this);
		moveBy(is->getDouble("OLDX") - xPos(),is->getDouble("OLDY") - yPos());
	}
	else
	{
		PoLine = is->getItem().second;
		doc()->AdjustItemSize(this);
		moveBy(is->getDouble("NEWX") - xPos(),is->getDouble("NEWY") - yPos());
	}
	OldB2 = width();
	OldH2 = height();
	updateClip();
	ContourLine = PoLine.copy();
}

bool PageItem::checkGradientUndoRedo(SimpleState *ss, bool isUndo)
{
	if (ss->contains("SNAP_TO_PATCH"))
	{
		restoreSnapToPatchGrid(ss, isUndo);
		return true;
	}
	if ((ss->contains("UNDO_UPDATE_CONICAL") && isUndo) || (ss->contains("REDO_UPDATE_CONICAL") && !isUndo))
	{
		createConicalMesh();
		return true;
	}
	if (ss->contains("GRAD_COL1"))
	{
		restoreGradientCol1(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_COL2"))
	{
		restoreGradientCol2(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_COL3"))
	{
		restoreGradientCol3(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_COL4"))
	{
		restoreGradientCol4(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_POS"))
	{
		restoreGradPos(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_QCOLOR1"))
	{
		restoreGradientColor1(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_QCOLOR2"))
	{
		restoreGradientColor2(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_QCOLOR3"))
	{
		restoreGradientColor3(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_QCOLOR4"))
	{
		restoreGradientColor4(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_TRANSP1"))
	{
		restoreGradientTrans1(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_TRANSP2"))
	{
		restoreGradientTrans2(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_TRANSP3"))
	{
		restoreGradientTrans3(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_TRANSP4"))
	{
		restoreGradientTrans4(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_SHADE1"))
	{
		restoreGradientShade1(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_SHADE2"))
	{
		restoreGradientShade2(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_SHADE3"))
	{
		restoreGradientShade3(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_SHADE4"))
	{
		restoreGradientShade4(ss, isUndo);
		return true;
	}
	if (ss->contains("MOVE_MESH_PATCH"))
	{
		restoreMoveMeshPatch(ss, isUndo);
		return true;
	}
	if (ss->contains("FILL_GRAD"))
	{
		restoreFillGradient(ss, isUndo);
		return true;
	}
	if (ss->contains("MASK_GRAD"))
	{
		restoreMaskGradient(ss, isUndo);
		return true;
	}
	if (ss->contains("STROKE_GRAD"))
	{
		restoreStrokeGradient(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MESH_COLOR"))
	{
		restoreGradientMeshColor(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_STARTX"))
	{
		restoreGradientStrokeStartX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_ENDX"))
	{
		restoreGradientStrokeEndX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_STARTY"))
	{
		restoreGradientStrokeStartY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_ENDY"))
	{
		restoreGradientStrokeEndY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_FOCALX"))
	{
		restoreGradientStrokeFocalX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_FOCALY"))
	{
		restoreGradientStrokeFocalY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_FOCALX"))
	{
		restoreGradientFocalX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_FOCALY"))
	{
		restoreGradientFocalY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKFOCALX"))
	{
		restoreGradientMaskFocalX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKFOCALY"))
	{
		restoreGradientMaskFocalY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_SCALE"))
	{
		restoreGradientStrokeScale(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_SKEW"))
	{
		restoreGradientStrokeSkew(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_CONTROL1"))
	{
		restoreGradientControl1(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_CONTROL2"))
	{
		restoreGradientControl2(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_CONTROL3"))
	{
		restoreGradientControl3(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_CONTROL4"))
	{
		restoreGradientControl4(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_CONTROL5"))
	{
		restoreGradientControl5(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_SCALE"))
	{
		restoreGradientScale(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_SKEW"))
	{
		restoreGradientSkew(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_STARTX"))
	{
		restoreGradientStartX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_ENDX"))
	{
		restoreGradientEndX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_STARTY"))
	{
		restoreGradientStartY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_ENDY"))
	{
		restoreGradientEndY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKSCALE"))
	{
		restoreGradientMaskScale(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKSKEW"))
	{
		restoreGradientMaskSkew(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKSTARTX"))
	{
		restoreGradientMaskStartX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKENDX"))
	{
		restoreGradientMaskEndX(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKSTARTY"))
	{
		restoreGradientMaskStartY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKENDY"))
	{
		restoreGradientMaskEndY(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_TYPE"))
	{
		restoreGradientType(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_TYPESTROKE"))
	{
		restoreGradientTypeStroke(ss, isUndo);
		return true;
	}
	if (ss->contains("CREATE_MESH_GRAD"))
	{
		restoreCreateMeshGrad(ss, isUndo);
		return true;
	}
	if (ss->contains("MOVE_MESH_GRAD"))
	{
		restoreMoveMeshGrad(ss, isUndo);
		return true;
	}
	if (ss->contains("RESET_MESH_GRAD"))
	{
		restoreResetMeshGrad(ss, isUndo);
		return true;
	}
	if (ss->contains("REMOVE_MESH_PATCH"))
	{
		restoreRemoveMeshPatch(ss, isUndo);
		return true;
	}
	return false;
}

void PageItem::restoreAppMode(SimpleState *state, bool isUndo)
{
	doc()->m_Selection->clear();
	select();
	if(isUndo)
		doc()->view()->requestMode(state->getInt("OLD"));
	else
		doc()->view()->requestMode(state->getInt("NEW"));
}

void PageItem::restoreSpiral(SimpleState *ss, bool isUndo)
{
	PageItem_Spiral *item = asSpiral();
	if (isUndo)
	{
		item->spiralStartAngle = ss->getDouble("OLD_START");
		item->spiralEndAngle = ss->getDouble("OLD_END");
		item->spiralFactor = ss->getDouble("OLD_FACTOR");
	}
	else
	{
		item->spiralStartAngle = ss->getDouble("NEW_START");
		item->spiralEndAngle = ss->getDouble("NEW_END");
		item->spiralFactor = ss->getDouble("NEW_FACTOR");
	}
	item->recalcPath();
	update();
	doc()->changed();
}

void PageItem::restorePolygon(SimpleState *ss, bool isUndo)
{
	PageItem_RegularPolygon *item = asRegularPolygon();
	if (isUndo)
	{
		item->polyCorners = ss->getInt("OLD_CORNER");
		item->polyUseFactor = ss->getBool("OLD_USEFACTOR");
		item->polyFactor = ss->getDouble("OLD_FACTOR");
		item->polyRotation = ss->getDouble("OLD_ROTATION");
		item->polyCurvature = ss->getDouble("OLD_CURV");
		item->polyInnerRot = ss->getDouble("OLD_INNER");
		item->polyOuterCurvature = ss->getDouble("OLD_OUTER");
	}
	else
	{
		item->polyCorners = ss->getInt("NEW_CORNER");
		item->polyUseFactor = ss->getBool("NEW_USEFACTOR");
		item->polyFactor = ss->getDouble("NEW_FACTOR");
		item->polyRotation = ss->getDouble("NEW_ROTATION");
		item->polyCurvature = ss->getDouble("NEW_CURV");
		item->polyInnerRot = ss->getDouble("NEW_INNER");
		item->polyOuterCurvature = ss->getDouble("NEW_OUTER");
	}
	item->recalcPath();
	update();
	doc()->changed();
}

void PageItem::restoreArc(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<FPointArray, FPointArray> > *ss = dynamic_cast<ScItemState<QPair<FPointArray, FPointArray> > *>(state);
	PageItem_Arc *item = asArc();
	if (isUndo)
	{
		item->arcHeight = ss->getDouble("OLD_HEIGHT");
		item->arcWidth = ss->getDouble("OLD_WIDTH");
		item->arcStartAngle = ss->getDouble("OLD_START");
		item->arcSweepAngle = ss->getDouble("OLD_SWEEP");
		item->PoLine = ss->getItem().first;
		doc()->AdjustItemSize(item);
		moveBy(ss->getDouble("OLD_XPOS") - xPos(),ss->getDouble("OLD_YPOS") - yPos());
	}
	else
	{
		item->arcHeight = ss->getDouble("NEW_HEIGHT");
		item->arcWidth = ss->getDouble("NEW_WIDTH");
		item->arcStartAngle = ss->getDouble("NEW_START");
		item->arcSweepAngle = ss->getDouble("NEW_SWEEP");
		item->PoLine = ss->getItem().second;
		doc()->AdjustItemSize(item);
		moveBy(ss->getDouble("NEW_XPOS") - xPos(),ss->getDouble("NEW_YPOS") - yPos());
	}
	update();
	//doc()->changed();
}

void PageItem::restoreImageNbr(SimpleState *state, bool isUndo)
{
	if (isUndo)
		pixm.imgInfo.actualPageNumber = state->getInt("OLD");
	else
		pixm.imgInfo.actualPageNumber = state->getInt("NEW");
	loadImage(externalFile(), true);
	update();
}

void PageItem::restoreTransform(SimpleState *ss, bool isUndo)
{
	ScItemState<QList<QTransform> > *is = dynamic_cast<ScItemState<QList<QTransform > >*>(ss);
	int x = is->getDouble("DX");
	int y = is->getDouble("DY");
	if (isUndo)
	{
		PoLine.translate(x,y);
		PoLine.map(is->getItem().at(2).inverted());
		PoLine.map(is->getItem().at(1).inverted());
		PoLine.map(is->getItem().at(0).inverted());
		PoLine.translate(-x,-y);
		ContourLine.translate(x,y);
		ContourLine.map(is->getItem().at(2).inverted());
		ContourLine.map(is->getItem().at(1).inverted());
		ContourLine.map(is->getItem().at(0).inverted());
		ContourLine.translate(-x,-y);
		doc()->AdjustItemSize(this);
		moveBy(is->getDouble("POSX") - xPos(),is->getDouble("POSY") - yPos());
	}
	else
	{
		PoLine.translate(x,y);
		PoLine.map(is->getItem().at(0));
		PoLine.map(is->getItem().at(1));
		PoLine.map(is->getItem().at(2));
		PoLine.translate(-x,-y);
		ContourLine.translate(x,y);
		ContourLine.map(is->getItem().at(0));
		ContourLine.map(is->getItem().at(1));
		ContourLine.map(is->getItem().at(2));
		ContourLine.translate(-x,-y);
		doc()->AdjustItemSize(this);
	}
}

void PageItem::restoreSnapToPatchGrid(SimpleState *state, bool isUndo)
{
	if (isUndo)
		snapToPatchGrid = state->getBool("OLD");
	else
		snapToPatchGrid = !state->getBool("OLD");
	update();
}


void PageItem::restoreMaskType(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrMask = state->getInt("OLD");
	else
		GrMask = state->getInt("NEW");
	update();
}

void PageItem::restoreGradientCol1(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrColorP1 = state->get("OLD");
	else
		GrColorP1 = state->get("NEW");
	update();
}

void PageItem::restoreGradientCol2(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrColorP2 = state->get("OLD");
	else
		GrColorP2 = state->get("NEW");
	update();
}

void PageItem::restoreGradientCol3(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrColorP3 = state->get("OLD");
	else
		GrColorP3 = state->get("NEW");
	update();
}

void PageItem::restoreGradientCol4(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrColorP4 = state->get("OLD");
	else
		GrColorP4 = state->get("NEW");
	update();
}

void PageItem::restoreGradientShade1(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol1Shade = state->getInt("OLD");
	else
		GrCol1Shade = state->getInt("NEW");
	update();
}

void PageItem::restoreGradientShade2(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol2Shade = state->getInt("OLD");
	else
		GrCol2Shade = state->getInt("NEW");
	update();
}

void PageItem::restoreGradientShade3(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol3Shade = state->getInt("OLD");
	else
		GrCol3Shade = state->getInt("NEW");
	update();
}

void PageItem::restoreGradientShade4(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol4Shade = state->getInt("OLD");
	else
		GrCol4Shade = state->getInt("NEW");
	update();
}

void PageItem::restoreGradientTrans1(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol1transp = state->getDouble("OLD");
	else
		GrCol1transp = state->getDouble("NEW");
	update();
}

void PageItem::restoreGradientTrans2(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol2transp = state->getDouble("OLD");
	else
		GrCol2transp = state->getDouble("NEW");
	update();
}

void PageItem::restoreGradientTrans3(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol3transp = state->getDouble("OLD");
	else
		GrCol3transp = state->getDouble("NEW");
	update();
}

void PageItem::restoreGradientTrans4(SimpleState *state, bool isUndo)
{
	if (isUndo)
		GrCol4transp = state->getDouble("OLD");
	else
		GrCol4transp = state->getDouble("NEW");
	update();
}

void PageItem::restoreGradPos(SimpleState *state, bool isUndo)
{
	ScItemState<QList<FPoint> > *is = dynamic_cast<ScItemState<QList<FPoint> > *>(state);
	if (isUndo)
	{
		GrStartX = is->getDouble("OLDSTARTX");
		GrStartY = is->getDouble("OLDSTARTY");
		GrEndX = is->getDouble("OLDENDX");
		GrEndY = is->getDouble("OLDENDY");
		GrFocalX = is->getDouble("OLDFOCALX");
		GrFocalY = is->getDouble("OLDFOCALY");
		GrScale = is->getDouble("OLDSCALE");
		GrSkew = is->getDouble("OLDSKEW");
		GrControl1 = is->getItem().at(0);
		GrControl2 = is->getItem().at(2);
		GrControl3 = is->getItem().at(4);
		GrControl4 = is->getItem().at(6);
		GrControl5 = is->getItem().at(8);
		GrStrokeStartX = is->getDouble("OLDSTROKESTARTX");
		GrStrokeStartY = is->getDouble("OLDSTROKESTARTY");
		GrStrokeEndX = is->getDouble("OLDSTROKEENDX");
		GrStrokeEndY = is->getDouble("OLDSTROKEENDY");
		GrStrokeFocalX = is->getDouble("OLDSTROKEFOCALX");
		GrStrokeFocalY = is->getDouble("OLDSTROKEFOCALY");
		GrStrokeScale = is->getDouble("OLDSTROKESCALE");
		GrStrokeSkew = is->getDouble("OLDSTROKESKEW");
		GrMaskStartX = is->getDouble("OLDMARKSTARTX");
		GrMaskStartY = is->getDouble("OLDMARKSTARTY");
		GrMaskEndX = is->getDouble("OLDMARKENDX");
		GrMaskEndY = is->getDouble("OLDMARKENDY");
		GrMaskFocalX = is->getDouble("OLDMARKFOCALX");
		GrMaskFocalY = is->getDouble("OLDMARKFOCALY");
		GrMaskScale = is->getDouble("OLDMARKSCALE");
		GrMaskSkew = is->getDouble("OLDMARKSKEW");
	}
	else
	{
		GrStartX = is->getDouble("STARTX");
		GrStartY = is->getDouble("STARTY");
		GrEndX = is->getDouble("ENDX");
		GrEndY = is->getDouble("ENDY");
		GrFocalX = is->getDouble("FOCALX");
		GrFocalY = is->getDouble("FOCALY");
		GrScale = is->getDouble("SCALE");
		GrSkew = is->getDouble("SKEW");
		GrControl1 = is->getItem().at(1);
		GrControl2 = is->getItem().at(3);
		GrControl3 = is->getItem().at(5);
		GrControl4 = is->getItem().at(7);
		GrControl5 = is->getItem().at(9);
		GrStrokeStartX = is->getDouble("STROKESTARTX");
		GrStrokeStartY = is->getDouble("STROKESTARTY");
		GrStrokeEndX = is->getDouble("STROKEENDX");
		GrStrokeEndY = is->getDouble("STROKEENDY");
		GrStrokeFocalX = is->getDouble("STROKEFOCALX");
		GrStrokeFocalY = is->getDouble("STROKEFOCALY");
		GrStrokeScale = is->getDouble("STROKESCALE");
		GrStrokeSkew = is->getDouble("STROKESKEW");
		GrMaskStartX = is->getDouble("MARKSTARTX");
		GrMaskStartY = is->getDouble("MARKSTARTY");
		GrMaskEndX = is->getDouble("MARKENDX");
		GrMaskEndY = is->getDouble("MARKENDY");
		GrMaskFocalX = is->getDouble("MARKFOCALX");
		GrMaskFocalY = is->getDouble("MARKFOCALY");
		GrMaskScale = is->getDouble("MARKSCALE");
		GrMaskSkew = is->getDouble("MARKSKEW");
	}
	if (GrType == 13)
		createConicalMesh();
	update();
}

void PageItem::restoreGradientColor1(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<QColor,QColor> > *is = dynamic_cast<ScItemState<QPair<QColor,QColor> > *>(state);
	if (isUndo)
		GrColorP1QColor = is->getItem().first;
	else
		GrColorP1QColor = is->getItem().second;
	update();
}

void PageItem::restoreRemoveMeshPatch(SimpleState *state, bool isUndo)
{
	ScItemState<meshGradientPatch> *is = dynamic_cast<ScItemState<meshGradientPatch> *>(state);
	if (isUndo)
	{
		selectedMeshPointX = is->getInt("POS");
		meshGradientPatches.insert(selectedMeshPointX,is->getItem());
	}
	else
	{
		meshGradientPatches.removeAt(selectedMeshPointX);
		selectedMeshPointX = -1;
	}
	update();
}

void PageItem::restoreCreateMeshGrad(SimpleState *state, bool isUndo)
{
	ScItemState<QList<QList<meshPoint> > > *is = dynamic_cast<ScItemState<QList<QList<meshPoint> > > *>(state);
	if (isUndo)
		meshGradientArray = is->getItem();
	else
		createGradientMesh(is->getInt("ROW"),is->getInt("COL"));
	update();
}

void PageItem::restoreMoveMeshGrad(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<QList<QList<meshPoint> >,FPointArray> > *is = dynamic_cast<ScItemState<QPair<QList<QList<meshPoint> >,FPointArray> > *>(state);
	if(isUndo)
	{
		PoLine = is->getItem().second;
		ClipEdited = true;
		FrameType = is->getInt("FRAME_TYPE");
		FPoint wh = getMaxClipF(&PoLine);
		setWidthHeight(wh.x(),wh.y());
		m_Doc->AdjustItemSize(this);
		OldB2 = is->getInt("OLDB");
		OldH2 = is->getInt("OLDH");
		updateClip();
		meshGradientArray = is->getItem().first;
	}
	else
		meshToShape();
	update();
}

void PageItem::restoreResetMeshGrad(SimpleState *state, bool isUndo)
{
	ScItemState<QList<QList<meshPoint> > > *is = dynamic_cast<ScItemState<QList<QList<meshPoint> > > *>(state);
	if (isUndo)
		meshGradientArray = is->getItem();
	else
		resetGradientMesh();
	update();
}

void PageItem::restoreGradientColor2(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<QColor,QColor> > *is = dynamic_cast<ScItemState<QPair<QColor,QColor> > *>(state);
	if (isUndo)
		GrColorP2QColor = is->getItem().first;
	else
		GrColorP2QColor = is->getItem().second;
	update();
}

void PageItem::restoreGradientColor3(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<QColor,QColor> > *is = dynamic_cast<ScItemState<QPair<QColor,QColor> > *>(state);
	if (isUndo)
		GrColorP3QColor = is->getItem().first;
	else
		GrColorP3QColor = is->getItem().second;
	update();
}

void PageItem::restoreGradientColor4(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<QColor,QColor> > *is = dynamic_cast<ScItemState<QPair<QColor,QColor> > *>(state);
	if (isUndo)
		GrColorP4QColor = is->getItem().first;
	else
		GrColorP4QColor = is->getItem().second;
	update();
}

void PageItem::restoreMoveMeshPatch(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<meshPoint,meshPoint> > *is = dynamic_cast<ScItemState<QPair<meshPoint,meshPoint> > *>(state);
	int x = is->getInt("X");
	int y = is->getInt("Y");
	if (isUndo)
	{
		if(is->getBool("ARRAY"))
			meshGradientArray[x][y] = is->getItem().first;
		else
			switch(y){
				case 1:
					meshGradientPatches[x].TL = is->getItem().first;
					break;
				case 2:
					meshGradientPatches[x].TR = is->getItem().first;
					break;
				case 3:
					meshGradientPatches[x].BR = is->getItem().first;
					break;
				case 4:
					meshGradientPatches[x].BL = is->getItem().first;
					break;
			}
	}
	else
	{
		if(is->getBool("ARRAY"))
			meshGradientArray[x][y] = is->getItem().second;
		else
			switch(y){
				case 1:
					meshGradientPatches[x].TL = is->getItem().second;
					break;
				case 2:
					meshGradientPatches[x].TR = is->getItem().second;
					break;
				case 3:
					meshGradientPatches[x].BR = is->getItem().second;
					break;
				case 4:
					meshGradientPatches[x].BL = is->getItem().second;
					break;
			}
	}
	update();
}

void PageItem::restoreFillGradient(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<VGradient,VGradient> > *is = dynamic_cast<ScItemState<QPair<VGradient,VGradient> > *>(state);
	if (isUndo)
		fill_gradient = is->getItem().first;
	else
		fill_gradient = is->getItem().second;
	if(gradientType()==13)
		createConicalMesh();
	update();
}

void PageItem::restoreMaskGradient(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<VGradient,VGradient> > *is = dynamic_cast<ScItemState<QPair<VGradient,VGradient> > *>(state);
	if (isUndo)
		mask_gradient = is->getItem().first;
	else
		mask_gradient = is->getItem().second;
	update();
}


void PageItem::restoreStrokeGradient(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<VGradient,VGradient> > *is = dynamic_cast<ScItemState<QPair<VGradient,VGradient> > *>(state);
	if (isUndo)
		stroke_gradient = is->getItem().first;
	else
		stroke_gradient = is->getItem().second;
	update();
}

void PageItem::restoreGradientMeshColor(SimpleState *ss, bool isUndo)
{
	ScItemState<QPair<QColor,QColor> > *is = dynamic_cast<ScItemState<QPair<QColor,QColor> > *>(ss);
	int x = is->getInt("X");
	int y = is->getInt("Y");
	meshPoint *mp;
	if(is->getBool("PATCH"))
	{
		meshGradientPatch *patch = &meshGradientPatches[x];
		switch (y)
		{
			case 1:
				mp = &(patch->TL);
				break;
			case 2:
				mp = &(patch->TR);
				break;
			case 3:
				mp = &(patch->BR);
				break;
			case 4:
				mp = &(patch->BL);
				break;
		}
	}
	else
		mp = &meshGradientArray[x][y];
	if (isUndo)
	{
		mp->colorName = is->get("OLD_COLOR_NAME");
		mp->color = is->getItem().first;
		mp->shade = is->getInt("OLD_SHADE");
		mp->transparency = is->getDouble("OLD_TRANS");
	}
	else
	{
		mp->colorName = is->get("NEW_COLOR_NAME");
		mp->color = is->getItem().second;
		mp->shade = is->getInt("NEW_SHADE");
		mp->transparency = is->getDouble("NEW_TRANS");
	}
}

void PageItem::restoreGradientStartX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStartX = is->getDouble("OLD");
	else
		GrStartX = is->getDouble("NEW");
}

void PageItem::restoreGradientStartY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStartY = is->getDouble("OLD");
	else
		GrStartY = is->getDouble("NEW");
}

void PageItem::restoreGradientEndX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrEndX = is->getDouble("OLD");
	else
		GrEndX = is->getDouble("NEW");
}

void PageItem::restoreGradientEndY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrEndY= is->getDouble("OLD");
	else
		GrEndY = is->getDouble("NEW");
}

void PageItem::restoreGradientFocalX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrFocalX= is->getDouble("OLD");
	else
		GrFocalX = is->getDouble("NEW");
}

void PageItem::restoreGradientFocalY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrFocalY= is->getDouble("OLD");
	else
		GrFocalY = is->getDouble("NEW");
}

void PageItem::restoreGradientScale(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrScale= is->getDouble("OLD");
	else
		GrScale = is->getDouble("NEW");
}

void PageItem::restoreGradientSkew(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrSkew = is->getDouble("OLD");
	else
		GrSkew = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskStartX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskStartX = is->getDouble("OLD");
	else
		GrMaskStartX = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskStartY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskStartY = is->getDouble("OLD");
	else
		GrMaskStartY = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskEndX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskEndX = is->getDouble("OLD");
	else
		GrMaskEndX = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskEndY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskEndY= is->getDouble("OLD");
	else
		GrMaskEndY = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskFocalX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskFocalX= is->getDouble("OLD");
	else
		GrMaskFocalX = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskFocalY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskFocalY= is->getDouble("OLD");
	else
		GrMaskFocalY = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskScale(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskScale= is->getDouble("OLD");
	else
		GrMaskScale = is->getDouble("NEW");
}

void PageItem::restoreGradientMaskSkew(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrMaskSkew = is->getDouble("OLD");
	else
		GrMaskSkew = is->getDouble("NEW");
}

void PageItem::restoreGradientControl1(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<FPoint,FPoint> > *is = dynamic_cast<ScItemState<QPair<FPoint,FPoint> > *>(state);
	if (isUndo)
		GrControl1= is->getItem().first;
	else
		GrControl1= is->getItem().second;
}

void PageItem::restoreGradientControl2(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<FPoint,FPoint> > *is = dynamic_cast<ScItemState<QPair<FPoint,FPoint> > *>(state);
	if (isUndo)
		GrControl2= is->getItem().first;
	else
		GrControl2= is->getItem().second;
}

void PageItem::restoreGradientControl3(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<FPoint,FPoint> > *is = dynamic_cast<ScItemState<QPair<FPoint,FPoint> > *>(state);
	if (isUndo)
		GrControl3= is->getItem().first;
	else
		GrControl3= is->getItem().second;
}

void PageItem::restoreGradientControl4(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<FPoint,FPoint> > *is = dynamic_cast<ScItemState<QPair<FPoint,FPoint> > *>(state);
	if (isUndo)
		GrControl4= is->getItem().first;
	else
		GrControl4= is->getItem().second;
}

void PageItem::restoreGradientControl5(SimpleState *state, bool isUndo)
{
	ScItemState<QPair<FPoint,FPoint> > *is = dynamic_cast<ScItemState<QPair<FPoint,FPoint> > *>(state);
	if (isUndo)
		GrControl5= is->getItem().first;
	else
		GrControl5= is->getItem().second;
}

void PageItem::restoreGradientStrokeScale(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeScale= is->getDouble("OLD");
	else
		GrStrokeScale = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeSkew(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeSkew= is->getDouble("OLD");
	else
		GrStrokeSkew = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeFocalX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeFocalX= is->getDouble("OLD");
	else
		GrStrokeFocalX = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeFocalY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeFocalY= is->getDouble("OLD");
	else
		GrStrokeFocalY = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeStartX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeStartX = is->getDouble("OLD");
	else
		GrStrokeStartX = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeStartY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeStartY = is->getDouble("OLD");
	else
		GrStrokeStartY = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeEndX(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeEndX = is->getDouble("OLD");
	else
		GrStrokeEndX = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeEndY(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeEndY= is->getDouble("OLD");
	else
		GrStrokeEndY = is->getDouble("NEW");
}

void PageItem::restoreGradientType(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrType = is->getInt("OLD");
	else
		GrType = is->getInt("NEW");
}

void PageItem::restoreGradientTypeStroke(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrTypeStroke = is->getInt("OLD");
	else
		GrTypeStroke = is->getInt("NEW");
}

void PageItem::restoreEndArrowScale(SimpleState *is, bool isUndo)
{
	if (isUndo)
		m_endArrowScale = is->getInt("OLD_SCALE");
	else
		m_endArrowScale = is->getInt("NEW_SCALE");
}

void PageItem::restoreStartArrowScale(SimpleState *is, bool isUndo)
{
	if (isUndo)
		m_startArrowScale = is->getInt("OLD_SCALE");
	else
		m_startArrowScale = is->getInt("NEW_SCALE");
}

void PageItem::restoreImageRotation(SimpleState *is, bool isUndo)
{
	if (isUndo)
		LocalRot = is->getInt("OLD_ROT");
	else
		LocalRot = is->getInt("NEW_ROT");
}

void PageItem::restorePasteInline(SimpleState *is, bool isUndo)
{
	int start = is->getInt("START");
	if (isUndo)
	{
		itemText.select(start,1);
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	else
		itemText.insertObject(is->getInt("INDEX"));
}

void PageItem::restorePasteText(SimpleState *ss, bool isUndo)
{
	ScItemState<StoryText> *is = dynamic_cast<ScItemState<StoryText>*>(ss);
	int start = is->getInt("START");
	if (isUndo){
		itemText.select(start,is->getItem().length());
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	else
		itemText.insert(is->getItem());
}

void PageItem::restoreColumnsGap(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		ColGap = ss->getInt("OLD_COLUMNS");
	else
		ColGap = ss->getInt("NEW_COLUMNS");
	update();
}

void PageItem::restoreLeftTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		Extra = ss->getInt("OLD_DIST");
	else
		Extra = ss->getInt("NEW_DIST");
	update();
}

void PageItem::restoreRightTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		RExtra = ss->getInt("OLD_DIST");
	else
		RExtra = ss->getInt("NEW_DIST");
	update();
}

void PageItem::restoreTopTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		TExtra = ss->getInt("OLD_DIST");
	else
		TExtra = ss->getInt("NEW_DIST");
	update();
}

void PageItem::restoreBottomTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		BExtra = ss->getInt("OLD_DIST");
	else
		BExtra = ss->getInt("NEW_DIST");
	update();
}

void PageItem::restoreColumns(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		Cols = ss->getInt("OLD_COLUMNS");
	else
		Cols = ss->getInt("NEW_COLUMNS");
	update();
}

void PageItem::restoreFirstLineOffset(SimpleState *ss, bool isUndo)
{
	ScItemState<QPair<FirstLineOffsetPolicy, FirstLineOffsetPolicy > > *is = dynamic_cast<ScItemState<QPair<FirstLineOffsetPolicy, FirstLineOffsetPolicy> >*>(ss);
	if (isUndo)
		firstLineOffsetP = is->getItem().first;
	else
		firstLineOffsetP = is->getItem().second;
	update();
}

void PageItem::restoreDefaultParagraphStyle(SimpleState *ss, bool isUndo)
{
	ScItemState<QPair<ParagraphStyle, ParagraphStyle > > *is = dynamic_cast<ScItemState<QPair<ParagraphStyle, ParagraphStyle> >*>(ss);
	if (isUndo)
		itemText.setDefaultStyle(is->getItem().second);
	else
		itemText.setDefaultStyle(is->getItem().first);
}

void PageItem::restoreParagraphStyle(SimpleState *ss, bool isUndo)
{
	ScItemState<QPair<ParagraphStyle, ParagraphStyle > > *is = dynamic_cast<ScItemState<QPair<ParagraphStyle, ParagraphStyle> >*>(ss);
	int pos = is->getInt("POS");
	if (isUndo)
	{
		itemText.eraseStyle(pos,is->getItem().first);
		itemText.applyStyle(pos,is->getItem().second);
	}
	else
		itemText.applyStyle(pos,is->getItem().first);
}

void PageItem::restoreCharStyle(SimpleState *ss, bool isUndo)
{
	ScItemState<QPair<CharStyle, CharStyle > > *is = dynamic_cast<ScItemState<QPair<CharStyle, CharStyle> >*>(ss);
	int length = is->getInt("LENGTH");
	int start = is->getInt("START");
	if (isUndo)
	{
		itemText.eraseCharStyle(start,length,is->getItem().first);
		itemText.applyCharStyle(start,length,is->getItem().second);
	}
	else
		itemText.applyCharStyle(start,length,is->getItem().first);
}

void PageItem::restoreSetCharStyle(SimpleState *ss, bool isUndo)
{
	ScItemState<QPair<CharStyle, CharStyle > > *is = dynamic_cast<ScItemState<QPair<CharStyle, CharStyle> >*>(ss);
	int length = is->getInt("LENGTH");
	int start = is->getInt("START");
	if (isUndo)
		itemText.setCharStyle(start,length,is->getItem().second);
	else
		itemText.setCharStyle(start,length,is->getItem().first);
}

void PageItem::restoreSetParagraphStyle(SimpleState *ss, bool isUndo)
{
	ScItemState<QPair<ParagraphStyle, ParagraphStyle > > *is = dynamic_cast<ScItemState<QPair<ParagraphStyle, ParagraphStyle> >*>(ss);
	int pos = is->getInt("POS");
	if (isUndo)
		itemText.setStyle(pos,is->getItem().second);
	else
		itemText.setStyle(pos,is->getItem().first);
}

void PageItem::restoreLoremIpsum(SimpleState *ss, bool isUndo)
{
	QString sampleText = ss->get("TEXT_STR");
	if (isUndo)
	{
		itemText.selectAll();
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	else
		itemText.insertChars(0,sampleText);
}

void PageItem::restoreDeleteFrameText(SimpleState *ss, bool isUndo)
{
	ScItemState<CharStyle> *is = dynamic_cast<ScItemState<CharStyle> *>(ss);
	QString text = is->get("TEXT_STR");
	int start = is->getInt("START");
	if (isUndo){
		itemText.insertChars(start,text);
		itemText.applyCharStyle(start, text.length(), is->getItem());
		invalid = true;
		invalidateLayout();
	} else {
		itemText.select(start,text.length());
		asTextFrame()->deleteSelectedTextFromFrame();
	}
}

void PageItem::restoreInsertFrameText(SimpleState *ss, bool isUndo)
{
	QString text = ss->get("TEXT_STR");
	int start = ss->getInt("START");
	if (isUndo)
	{
		itemText.select(start,text.length());
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	else
		itemText.insertChars(start,text);
}

void PageItem::restoreCornerRadius(SimpleState *state, bool isUndo)
{
	if (isUndo)
		RadRect=state->getDouble("OLD_RADIUS");
	else
		RadRect=state->getDouble("NEW_RADIUS");
	Selection tmpSelection(doc()->m_Selection);
	doc()->m_Selection->clear();
	doc()->m_Selection->addItem(this);
	doc()->scMW()->view->SetFrameRounded();
	*(doc()->m_Selection) = tmpSelection;
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
	int  stateCode = state->transactionCode;
	bool redraw = ((stateCode != 1) && (stateCode != 3));
	if (isUndo)
	{
		m_Doc->SizeItem(ow, oh, this, false, true, redraw);
		m_Doc->MoveItem(mx, my, this, false);
		m_Doc->RotateItem(ort, this);
	}
	else
	{
		mx = -mx;
		my = -my;
		m_Doc->SizeItem(w, h, this, false, true, redraw);
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
	int  stateCode = state->transactionCode;
	bool redraw = ((stateCode != 1) && (stateCode != 3));
	if (isUndo)
	{
		m_Doc->RotateItem(ort, this);
		m_Doc->MoveItem(ox - Xpos, oy - Ypos, this, false);
		m_Doc->SizeItem(ow, oh, this, false, true, redraw);
	}
	else
	{
		m_Doc->RotateItem(rt, this);
		m_Doc->MoveItem(x - Xpos, y - Ypos, this, false);
		m_Doc->SizeItem(w, h, this, false, true, redraw);
	}
	oldRot = Rot;
	oldXpos = Xpos;
	oldYpos = Ypos;
	oldOwnPage = OwnPage;
	oldWidth = Width;
	oldHeight = Height;
}

void PageItem::restoreShowImage(SimpleState *state, bool isUndo)
{
	bool old = state->getBool("OLD");
	if (isUndo)
		PicArt = old;
	else
		PicArt = !old;
	update();
}

void PageItem::restoreFill(SimpleState *state, bool isUndo)
{
	QString fill = state->get("OLD_FILL");
	if (!isUndo)
		fill = state->get("NEW_FILL");
	select();
	m_Doc->itemSelection_SetItemBrush(fill);
}

void PageItem::restoreResTyp(SimpleState *state, bool isUndo)
{
	if (isUndo)
		setResolution(state->getInt("OLD_RES"));
	else
		setResolution(state->getInt("NEW_RES"));
	doc()->updatePic();
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
	view->requestMode(modeNormal);
}

void PageItem::restoreTextFlowing(SimpleState *state, bool isUndo)
{
	TextFlowMode oldMode = (TextFlowMode) state->getInt("TEXTFLOW_OLDMODE");
	TextFlowMode newMode = (TextFlowMode) state->getInt("TEXTFLOW_NEWMODE");
	if (isUndo)
		textFlowModeVal = oldMode;
	else
		textFlowModeVal = newMode;
	
	QList<PageItem*> pList;
	int id = m_Doc->Items->indexOf(this) - 1;
	for(int idx = id; idx >= 0 ; --idx)
	{
		pList << m_Doc->Items->at(idx);
	}
		
	QRectF baseRect(getBoundingRect());
	for(int idx(0); idx < pList.count(); ++idx)
	{
		QRectF uRect(pList.at(idx)->getBoundingRect());
		if(baseRect.intersects(uRect))
			pList.at(idx)->update();
	}
}

void PageItem::restoreImageScaleMode(SimpleState *state, bool isUndo)
{
	bool type=ScaleType;
	if (state->contains("SCALE_TYPE"))
	{
		if (isUndo)
			type = !state->getBool("SCALE_TYPE");
		else
			type = state->getBool("SCALE_TYPE");
		//if restoring free scaling
		//old offset and scale ratio must be restored
		if (type)
		{
			double oscx = state->getDouble("OLD_IMAGEXSCALE");
			double oscy = state->getDouble("OLD_IMAGEYSCALE");
			double ox = state->getDouble("OLD_IMAGEXOFFSET");
			double oy = state->getDouble("OLD_IMAGEYOFFSET");
			Selection tempSelection(this, false);
			tempSelection.addItem(this, true);
			m_Doc->itemSelection_SetImageScale(oscx, oscy, &tempSelection);
			m_Doc->itemSelection_SetImageOffset(ox, oy, &tempSelection);
		}
		else
		{
			state->set("OLD_IMAGEXOFFSET", LocalX);
			state->set("OLD_IMAGEYOFFSET", LocalY);
			state->set("OLD_IMAGEXSCALE", LocalScX);
			state->set("OLD_IMAGEYSCALE", LocalScY);
		}
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

void PageItem::restoreClearImage(UndoState *state, bool isUndo)
{
	if (!isImageFrame())
		return;
	if (isUndo)
	{
		ScItemState<ScImageEffectList> *is = dynamic_cast<ScItemState<ScImageEffectList>*>(state);
		Pfile = is->get("CI_PFILE");
		loadImage(Pfile, false);
		effectsInUse = is->getItem();
		setImageFlippedH(is->getBool("CI_FLIPPH"));
		setImageFlippedV(is->getBool("CI_FLIPPV"));
		setImageScalingMode(is->getBool("CI_SCALING"),is->getBool("CI_ASPECT"));
		setImageXOffset(is->getDouble("CI_XOFF"));
		setImageXScale(is->getDouble("CI_XSCALE"));
		setImageYOffset(is->getDouble("CI_YOFF"));
		setImageYScale(is->getDouble("CI_YSCALE"));
		setFillTransparency(is->getDouble("CI_FILLT"));
		setLineTransparency(is->getDouble("CI_LINET"));

		select();
		m_Doc->updatePic();
	}
	else
		asImageFrame()->clearContents();
}

void PageItem::restoreLinkTextFrame(UndoState *state, bool isUndo)
{
	if (!isTextFrame())
		return;
	if (isUndo)
	{
		unlink(false);
		//restore properly text if frame was linked at beginning of chain
		ScItemState<QPair<PageItem*, PageItem*> > *is = dynamic_cast<ScItemState<QPair<PageItem*, PageItem*> >*>(state);
		int joinPos = is->getInt("JOIN_POS");
		int ParSep = is->getBool("ADDPARSEP")?1:0;
		if (is->getBool("FIRST"))
		{
			if (joinPos == 0)
			{
				is->getItem().second->itemText.append(itemText);
				itemText = StoryText(m_Doc);
			}
			else
			{
				StoryText content(m_Doc);
				itemText.select(joinPos + ParSep, itemText.length() - (joinPos + ParSep));
				content.insert(0, itemText, true);
				if (ParSep)
					itemText.select(joinPos, itemText.length() - joinPos);
				itemText.removeSelection();
				is->getItem().second->itemText.append(content);
			}
		}
		else
		{
			StoryText content(m_Doc);
			PageItem* prev = is->getItem().second;
			prev->itemText.select(joinPos + ParSep, prev->itemText.length() - (joinPos + ParSep));
			content.insert(0, prev->itemText, true);
			if (ParSep)
				prev->itemText.select(joinPos, prev->itemText.length() - joinPos);
			prev->itemText.removeSelection();
			itemText.append(content);
		}
	}
	else
	{
		ScItemState<QPair<PageItem*, PageItem*> > *is = dynamic_cast<ScItemState<QPair<PageItem*, PageItem*> >*>(state);
		asTextFrame()->link(is->getItem().second->asTextFrame());
	}
}

void PageItem::restoreUnlinkTextFrame(UndoState *state, bool isUndo)
{
	if (!isTextFrame())
		return;
	ScItemState<QPair<PageItem*, PageItem*> > *is = dynamic_cast<ScItemState<QPair<PageItem*, PageItem*> >*>(state);
	if (is->contains("CUT_TEXT"))
	{
		bool cutText = is->getBool("CUT_TEXT");
		if (isUndo)
		{
			PageItem* prev = is->getItem().first;
			PageItem* next  = is->getItem().second;
			if (prev != NULL)
			{
				if (!cutText)
					itemText = StoryText(m_Doc);
				prev->link(this, false);
			}
			else if (next != NULL)
			{
				if (!cutText)
					this->itemText = StoryText(m_Doc);
				this->link(next, false);
			}
			else
				Q_ASSERT(prev || next);
		}
		else
			unlinkWithText(cutText);
	}
	else
	{
		if (isUndo)
			asTextFrame()->link(is->getItem().second->asTextFrame());
		else
			unlink();
	}
}

void PageItem::restoreReverseText(UndoState *state, bool /*isUndo*/)
{
	if (!isTextFrame())
		return;
	Reverse = !Reverse;
}

void PageItem::restorePathOperation(UndoState *state, bool isUndo)
{//PATH_OPERATION
	ScItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >*is = dynamic_cast<ScItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >*>(state);
	if (is)
	{
		if (isUndo)
		{
			this->ClipEdited=is->getBool("PATH_OP_OLD_CLIPEDITED");
			this->Frame=is->getBool("PATH_OP_OLD_FRAME");
			this->FrameType=is->getInt("PATH_OP_OLD_FRAMETYPE");
			this->OldB2=is->getDouble("PATH_OP_OLD_OLDB2");
			this->OldH2=is->getDouble("PATH_OP_OLD_OLDH2");
			QPair<FPointArray, FPointArray> oldLines=is->getItem().first;
			this->PoLine = oldLines.first;
			this->ContourLine = oldLines.second;
		}
		else
		{
			this->ClipEdited=is->getBool("PATH_OP_NEW_CLIPEDITED");
			this->Frame=is->getBool("PATH_OP_NEW_FRAME");
			this->FrameType=is->getInt("PATH_OP_NEW_FRAMETYPE");
			this->OldB2=is->getDouble("PATH_OP_NEW_OLDB2");
			this->OldH2=is->getDouble("PATH_OP_NEW_OLDH2");
			QPair<FPointArray, FPointArray> newLines=is->getItem().second;
			this->PoLine = newLines.first;
			this->ContourLine = newLines.second;
		}
		this->updateClip();
	}
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

void PageItem::restoreUniteItem(SimpleState *state, bool isUndo)
{
	ScItemState< QPair<QList<PageItem*>,QList<QTransform> > > *is = dynamic_cast<ScItemState< QPair<QList<PageItem*>,QList<QTransform> > >*>(state);
	if (is)
	{
		m_Doc->view()->Deselect(true);
		if (isUndo){
			int pts = 0;
			select();
			for (int i = 0; i < is->getItem().first.size(); ++i)
			{
				PageItem* myItem = is->getItem().first.at(i);
				myItem->PoLine.map(is->getItem().second.at(i).inverted());
				pts += myItem->PoLine.size();
				pts+=4;
				doc()->m_Selection->addItem(myItem);
			}
			PoLine.resize(PoLine.size()-pts);
			Segments.clear();
			Frame = is->getBool("FRAME");
			FrameType = is->getInt("FRAMETYPE");
			ClipEdited = is->getBool("CLIPEDITED");
			doc()->AdjustItemSize(this);
		} else {
			select();
			for (int i = 0; i < is->getItem().first.size(); ++i)
				doc()->view()->SelectItem(is->getItem().first.at(i));
			doc()->itemSelection_UniteItems();
			select();
		}
	}
}

void PageItem::restoreSplitItem(SimpleState *state, bool isUndo)
{
	ScItemState< QList<int> > *is = dynamic_cast<ScItemState< QList<int> >*>(state);
	if (is)
	{
		QList<int> itemsList = is->getItem();
		select();
		if (isUndo)
		{
			for (int i = 0; i < itemsList.size(); ++i)
				doc()->view()->SelectItem(doc()->Items->at(itemsList.at(i)));
			doc()->itemSelection_UniteItems();
			select();
		}
		else
			doc()->itemSelection_SplitItems();
	}
}

void PageItem::restoreContourLine(SimpleState *state, bool isUndo)
{
	ScItemState<FPointArray> *is = dynamic_cast<ScItemState<FPointArray>*>(state);
	if (is)
	{
		if (isUndo)
			ContourLine = is->getItem();
		else
			ContourLine = PoLine.copy();
		ClipEdited = true;
	}
}

void PageItem::restoreShapeType(SimpleState *state, bool isUndo)
{
	// Store shape info in this form:
	// CHANGE_SHAPE_TYPE - ID of the undo operation
	// OLD_FRAME_TYPE - original frame type
	// NEW_FRAME_TYPE - change of frame type
	// binary QPair<FPointArray, FPointArray> - .first original shape, .second new shape
	ScItemState<QPair<FPointArray,FPointArray> > *is = dynamic_cast<ScItemState<QPair<FPointArray,FPointArray> >*>(state);
	if (is)
	{
		if (isUndo)
		{
			this->FrameType = is->getInt("OLD_FRAME_TYPE");
			this->PoLine = is->getItem().first;
			ClipEdited = !(FrameType == 0 || FrameType == 1);
		}
		else
		{
			this->FrameType = is->getInt("NEW_FRAME_TYPE");
			this->PoLine = is->getItem().second;
			ClipEdited = (FrameType == 0 || FrameType == 1);
		}
		Clip = FlattenPath(PoLine,Segments);
	}
}

void PageItem::restoreLayer(SimpleState *state, bool isUndo)
{
	ScribusView* view = m_Doc->view();
	setLayer(isUndo ? state->getInt("OLD_LAYER") : state->getInt("NEW_LAYER"));
	view->Deselect(true);
	m_Doc->regionsChanged()->update(QRectF());
}

void PageItem::restoreGetImage(UndoState *state, bool isUndo)
{
	ScItemState<ScImageEffectList> *is = dynamic_cast<ScItemState<ScImageEffectList>*>(state);
	QString fn = is->get("OLD_IMAGE_PATH");
	if (!isUndo)
		fn = is->get("NEW_IMAGE_PATH");
	if (fn.isEmpty())
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(this, true);
		m_Doc->itemSelection_ClearItem(&tempSelection);
	}
	else
	{
		loadImage(fn, false);
		if (isUndo)
		{
			//restore old image settings
			effectsInUse = is->getItem();
			setImageFlippedH(is->getBool("FLIPPH"));
			setImageFlippedV(is->getBool("FLIPPV"));
			setImageScalingMode(is->getBool("SCALING"), is->getBool("ASPECT"));
			setImageXOffset(is->getDouble("XOFF"));
			setImageXScale(is->getDouble("XSCALE"));
			setImageYOffset(is->getDouble("YOFF"));
			setImageYScale(is->getDouble("YSCALE"));
			setFillTransparency(is->getDouble("FILLT"));
			setLineTransparency(is->getDouble("LINET"));
		}
		select();
		m_Doc->updatePic();
	}
}

void PageItem::restoreShapeContour(UndoState *state, bool isUndo)
{
	ScItemState<QPair<FPointArray,FPointArray> > *istate =
			dynamic_cast<ScItemState<QPair<FPointArray,FPointArray> >*>(state);
	if (istate)
	{
		FPointArray oldClip = istate->getItem().first;
		FPointArray newClip = istate->getItem().second;
		bool isContour = istate->getBool("IS_CONTOUR");
		double oldX = istate->getDouble("OLD_X");
		double oldY = istate->getDouble("OLD_Y");
		double newX = istate->getDouble("NEW_X");
		double newY = istate->getDouble("NEW_Y");

		if (isUndo)
		{
			if (isContour)
				ContourLine = oldClip;
			else
				PoLine = oldClip;
			m_Doc->AdjustItemSize(this);
			m_Doc->MoveItem(oldX - xPos(), oldY - yPos(), this, false);
		}
		else
		{
			if (isContour)
				ContourLine = newClip;
			else
				PoLine = newClip;
			m_Doc->AdjustItemSize(this);
			m_Doc->MoveItem(newX - xPos(), newY - yPos(), this, false);
		}
		m_Doc->regionsChanged()->update(QRectF());
	}

}

void PageItem::restoreImageEffects(UndoState *state, bool isUndo)
{
	ScItemState<QPair<ScImageEffectList, ScImageEffectList> > *istate =
	dynamic_cast<ScItemState<QPair<ScImageEffectList,ScImageEffectList> >*>(state);
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

QList<ObjectAttribute> PageItem::getObjectAttributes(QString attributeName) const
{
	QList<ObjectAttribute> attributes;
	for(ObjAttrVector::const_iterator objAttrIt = pageItemAttributes.begin() ; objAttrIt != pageItemAttributes.end(); ++objAttrIt )
	{
		if (objAttrIt->name == attributeName)
		{
			attributes.append(*objAttrIt);
		}
	}
	return attributes;
}

ObjectAttribute PageItem::getObjectAttribute(QString attributeName) const
{
	int countFound=0;
	ObjAttrVector::const_iterator foundIt = pageItemAttributes.begin();
	for(ObjAttrVector::const_iterator objAttrIt = pageItemAttributes.begin(); objAttrIt != pageItemAttributes.end(); ++objAttrIt )
	{
		if (objAttrIt->name==attributeName)
		{
			++countFound;
			foundIt=objAttrIt;
		}
	}
	ObjectAttribute returnAttribute;
	if(countFound==1)
		returnAttribute = (*foundIt);
	else
		returnAttribute.name = QString::null;
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
		
	it = newNames.colors().find(GrColorP1);
	if (it != newNames.colors().end())
		GrColorP1 = *it;
	it = newNames.colors().find(GrColorP2);
	if (it != newNames.colors().end())
		GrColorP2 = *it;
	it = newNames.colors().find(GrColorP3);
	if (it != newNames.colors().end())
		GrColorP3 = *it;
	it = newNames.colors().find(GrColorP4);
	if (it != newNames.colors().end())
		GrColorP4 = *it;

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
	for (int grow = 0; grow < meshGradientArray.count(); grow++)
	{
		for (int gcol = 0; gcol < meshGradientArray[grow].count(); gcol++)
		{
			meshPoint mp = meshGradientArray[grow][gcol];
			it = newNames.colors().find(mp.colorName);
			if (it != newNames.colors().end())
				meshGradientArray[grow][gcol].colorName = *it;
		}
	}
	for (int col = 0; col < meshGradientPatches.count(); col++)
	{
		it = newNames.colors().find(meshGradientPatches[col].TL.colorName);
		if (it != newNames.colors().end())
			meshGradientPatches[col].TL.colorName = *it;
		it = newNames.colors().find(meshGradientPatches[col].TR.colorName);
		if (it != newNames.colors().end())
			meshGradientPatches[col].TR.colorName = *it;
		it = newNames.colors().find(meshGradientPatches[col].BL.colorName);
		if (it != newNames.colors().end())
			meshGradientPatches[col].BL.colorName = *it;
		it = newNames.colors().find(meshGradientPatches[col].BR.colorName);
		if (it != newNames.colors().end())
			meshGradientPatches[col].BR.colorName = *it;
	}
	cstops = stroke_gradient.colorStops();
	for (uint cst = 0; cst < stroke_gradient.Stops(); ++cst)
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
			ScTextStream fp(&tmpstr, QIODevice::ReadOnly);
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
	
	it = newNames.patterns().find(strokePattern());
	if (it != newNames.patterns().end())
		setStrokePattern(*it);
	it = newNames.patterns().find(patternMask());
	if (it != newNames.patterns().end())
		setPatternMask(*it);
	
	it = newNames.gradients().find(strokeGradient());
	if (it != newNames.gradients().end())
		setStrokeGradient(*it);
	
	it = newNames.gradients().find(gradient());
	if (it != newNames.gradients().end())
		setGradient(*it);
	it = newNames.gradients().find(gradientMask());
	if (it != newNames.gradients().end())
		setGradientMask(*it);

	it = newNames.lineStyles().find(customLineStyle());
	if (it != newNames.lineStyles().end())
		setCustomLineStyle(*it);

	if (prevInChain() == NULL)
		itemText.replaceNamedResources(newNames);
}

void PageItem::setGradientType(int val)
{
	if(GrType==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradType,"",Um::IFill);
		ss->set("GRAD_TYPE","grad_type");
		ss->set("OLD",GrType);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrType = val;
}

void PageItem::setStrokeGradientType(int val)
{
	if(GrTypeStroke==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradTypeStroke,"",Um::ILineStyle);
		ss->set("GRAD_TYPESTROKE","grad_typestroke");
		ss->set("OLD",GrTypeStroke);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrTypeStroke = val;
}

void PageItem::setGradientCol1(QString val)
{
	if(GrColorP1==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_COL1","grad_col1");
		ss->set("OLD",GrColorP1);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrColorP1 = val;
}

void PageItem::setGradientCol2(QString val)
{
	if(GrColorP2==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_COL2","grad_col2");
		ss->set("OLD",GrColorP2);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrColorP2 = val;
}

void PageItem::setGradientCol3(QString val)
{
	if(GrColorP3==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_COL3","grad_col3");
		ss->set("OLD",GrColorP3);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrColorP3 = val;
}

void PageItem::setGradientCol4(QString val)
{
	if(GrColorP4==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_COL4","grad_col4");
		ss->set("OLD",GrColorP4);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrColorP4 = val;
}

void PageItem::setGradientShade1(int val)
{
	if(GrCol1Shade==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_SHADE1","grad_shade1");
		ss->set("OLD",GrCol1Shade);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol1Shade = val;
}

void PageItem::setGradientShade2(int val)
{
	if(GrCol1Shade==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_SHADE2","grad_shade2");
		ss->set("OLD",GrCol2Shade);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol2Shade = val;
}

void PageItem::setGradientShade3(int val)
{
	if(GrCol1Shade==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_SHADE3","grad_shade3");
		ss->set("OLD",GrCol3Shade);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol3Shade = val;
}

void PageItem::setGradientShade4(int val)
{
	if(GrCol1Shade==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_SHADE4","grad_shade4");
		ss->set("OLD",GrCol4Shade);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol4Shade = val;
}

void PageItem::setGradientTransp1(double val)
{
	if(GrCol1transp==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_TRANSP1","grad_transp1");
		ss->set("OLD",GrCol1transp);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol1transp = val;
}

void PageItem::setGradientTransp2(double val)
{
	if(GrCol2transp==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_TRANSP2","grad_transp2");
		ss->set("OLD",GrCol2transp);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol2transp = val;
}

void PageItem::setGradientTransp3(double val)
{
	if(GrCol3transp==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_TRANSP3","grad_transp3");
		ss->set("OLD",GrCol3transp);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol3transp = val;
}

void PageItem::setGradientTransp4(double val)
{
	if(GrCol4transp==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_TRANSP4","grad_transp4");
		ss->set("OLD",GrCol4transp);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrCol4transp = val;
}

void PageItem::setGradientColor1(QColor val)
{
	if(GrColorP1QColor ==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_QCOLOR1","grad_qcolor1");
		ss->setItem(qMakePair(GrColorP1QColor,val));
		undoManager->action(this,ss);
	}
	GrColorP1QColor = val;
}

void PageItem::setGradientColor2(QColor val)
{
	if(GrColorP2QColor ==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_QCOLOR2","grad_qcolor2");
		ss->setItem(qMakePair(GrColorP2QColor,val));
		undoManager->action(this,ss);
	}
	GrColorP2QColor = val;
}

void PageItem::setGradientColor3(QColor val)
{
	if(GrColorP3QColor ==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_QCOLOR3","grad_qcolor3");
		ss->setItem(qMakePair(GrColorP3QColor,val));
		undoManager->action(this,ss);
	}
	GrColorP3QColor = val;
}

void PageItem::setGradientColor4(QColor val)
{
	if(GrColorP4QColor ==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<QColor,QColor> > *ss = new ScItemState<QPair<QColor,QColor> >(Um::GradCol,"",Um::IFill);
		ss->set("GRAD_QCOLOR4","grad_qcolor4");
		ss->setItem(qMakePair(GrColorP4QColor,val));
		undoManager->action(this,ss);
	}
	GrColorP4QColor = val;
}

void PageItem::setSnapToPatchGrid(bool val)
{
	if(snapToPatchGrid ==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("SNAP_TO_PATCH","snap_to_patch");
		ss->set("OLD",val);
		undoManager->action(this,ss);
	}
	snapToPatchGrid = val;
}

void PageItem::setGradientStartX(double val){
	if(GrStartX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_STARTX","grad_startx");
		ss->set("OLD",GrStartX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStartX = val;
}

void PageItem::setGradientStartY(double val){
	if(GrStartY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_STARTY","grad_starty");
		ss->set("OLD",GrStartY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStartY = val;
}

void PageItem::setGradientEndX(double val){
	if(GrEndX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_ENDX","grad_endx");
		ss->set("OLD",GrEndX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrEndX = val;
}

void PageItem::setGradientEndY(double val){
	if(GrEndY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_ENDY","grad_endy");
		ss->set("OLD",GrEndY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrEndY = val;
}

void PageItem::setGradientFocalX(double val){
	if(GrFocalX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_FOCALX","grad_focalx");
		ss->set("OLD",GrFocalX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrFocalX = val;
}

void PageItem::setGradientFocalY(double val){
	if(GrFocalY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_FOCALY","grad_focaly");
		ss->set("OLD",GrFocalY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrFocalY = val;
}

void PageItem::setGradientScale(double val){
	if(GrScale==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_SCALE","grad_scale");
		ss->set("OLD",GrScale);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrScale = val;
}

void PageItem::setGradientSkew(double val){
	if(GrSkew==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_SKEW","grad_skew");
		ss->set("OLD",GrSkew);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrSkew = val;
}

void PageItem::setGradientMaskStartX(double val){
	if(GrMaskStartX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKSTARTX","grad_maskstartx");
		ss->set("OLD",GrMaskStartX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskStartX = val;
}

void PageItem::setGradientMaskStartY(double val){
	if(GrMaskStartY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKSTARTY","grad_maskstarty");
		ss->set("OLD",GrMaskStartY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskStartY = val;
}

void PageItem::setGradientMaskEndX(double val){
	if(GrMaskEndX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKENDX","grad_maskendx");
		ss->set("OLD",GrMaskEndX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskEndX = val;
}

void PageItem::setGradientMaskEndY(double val){
	if(GrMaskEndY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKENDY","grad_maskendy");
		ss->set("OLD",GrMaskEndY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskEndY = val;
}

void PageItem::setGradientMaskFocalX(double val){
	if(GrMaskFocalX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKFOCALX","grad_maskfocalx");
		ss->set("OLD",GrMaskFocalX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskFocalX = val;
}

void PageItem::setGradientMaskFocalY(double val){
	if(GrMaskFocalY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKFOCALY","grad_maskfocaly");
		ss->set("OLD",GrMaskFocalY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskFocalY = val;
}

void PageItem::setGradientMaskScale(double val){
	if(GrMaskScale==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKSCALE","grad_maskscale");
		ss->set("OLD",GrMaskScale);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskScale = val;
}

void PageItem::setGradientMaskSkew(double val){
	if(GrMaskSkew==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_MASKSKEW","grad_maskskew");
		ss->set("OLD",GrMaskSkew);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrMaskSkew = val;
}

void PageItem::setGradientControl1(FPoint val){
	if(GrControl1==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<FPoint,FPoint> > *ss = new ScItemState<QPair<FPoint,FPoint> >(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_CONTROL1","grad_control1");
		ss->setItem(qMakePair(GrControl1,val));
		undoManager->action(this,ss);
	}
	GrControl1 = val;
}

void PageItem::setGradientControl2(FPoint val){
	if(GrControl2==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<FPoint,FPoint> > *ss = new ScItemState<QPair<FPoint,FPoint> >(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_CONTROL2","grad_control2");
		ss->setItem(qMakePair(GrControl2,val));
		undoManager->action(this,ss);
	}
	GrControl2 = val;
}

void PageItem::setGradientControl3(FPoint val){
	if(GrControl3==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<FPoint,FPoint> > *ss = new ScItemState<QPair<FPoint,FPoint> >(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_CONTROL3","grad_control3");
		ss->setItem(qMakePair(GrControl3,val));
		undoManager->action(this,ss);
	}
	GrControl3 = val;
}

void PageItem::setGradientControl4(FPoint val){
	if(GrControl4==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<FPoint,FPoint> > *ss = new ScItemState<QPair<FPoint,FPoint> >(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_CONTROL4","grad_control4");
		ss->setItem(qMakePair(GrControl4,val));
		undoManager->action(this,ss);
	}
	GrControl4 = val;
}

void PageItem::setGradientControl5(FPoint val){
	if(GrControl5==val)
		return;
	if(UndoManager::undoEnabled())
	{
		ScItemState<QPair<FPoint,FPoint> > *ss = new ScItemState<QPair<FPoint,FPoint> >(Um::GradPos,"",Um::IFill);
		ss->set("GRAD_CONTROL5","grad_control5");
		ss->setItem(qMakePair(GrControl5,val));
		undoManager->action(this,ss);
	}
	GrControl5 = val;
}

void PageItem::setGradientStrokeScale(double val){
	if(GrStrokeScale==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_SCALE","gradstroke_scale");
		ss->set("OLD",GrStrokeScale);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeScale = val;
}

void PageItem::setGradientStrokeSkew(double val){
	if(GrStrokeSkew==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_SKEW","gradstroke_skew");
		ss->set("OLD",GrStrokeSkew);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeSkew = val;
}

void PageItem::setGradientStrokeFocalX(double val){
	if(GrStrokeFocalX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_FOCALX","gradstroke_focalx");
		ss->set("OLD",GrStrokeFocalX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeFocalX = val;
}

void PageItem::setGradientStrokeFocalY(double val){
	if(GrStrokeFocalY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_FOCALY","gradstroke_focaly");
		ss->set("OLD",GrStrokeFocalY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeFocalY = val;
}

void PageItem::setGradientStrokeStartX(double val){
	if(GrStrokeStartX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_STARTX","gradstroke_startx");
		ss->set("OLD",GrStrokeStartX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeStartX = val;
}

void PageItem::setGradientStrokeStartY(double val){
	if(GrStrokeStartY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_STARTY","gradstroke_starty");
		ss->set("OLD",GrStrokeStartY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeStartY = val;
}

void PageItem::setGradientStrokeEndX(double val){
	if(GrStrokeEndX==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_ENDX","gradstroke_endx");
		ss->set("OLD",GrStrokeEndX);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeEndX = val;
}

void PageItem::setGradientStrokeEndY(double val){
	if(GrStrokeEndY==val)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::GradPos,"",Um::ILine);
		ss->set("GRADSTROKE_ENDY","gradstroke_endy");
		ss->set("OLD",GrStrokeEndY);
		ss->set("NEW",val);
		undoManager->action(this,ss);
	}
	GrStrokeEndY = val;
}

void PageItem::getNamedResources(ResourceCollection& lists) const
{
	if (GrType == 0)
		lists.collectColor(fillColor());
	else if ((GrType < 8) || (GrType == 10))
	{
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		for (uint cst = 0; cst < fill_gradient.Stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
	else if (GrType == 9)
	{
		lists.collectColor(GrColorP1);
		lists.collectColor(GrColorP2);
		lists.collectColor(GrColorP3);
		lists.collectColor(GrColorP4);
	}
	else if (GrType == 11)
	{
		for (int grow = 0; grow < meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < meshGradientArray[grow].count(); gcol++)
			{
				if (m_Doc->PageColors.contains(meshGradientArray[grow][gcol].colorName))
					lists.collectColor(meshGradientArray[grow][gcol].colorName);
			}
		}
	}
	else if (GrType == 12)
	{
		for (int col = 0; col < meshGradientPatches.count(); col++)
		{
			lists.collectColor(meshGradientPatches[col].TL.colorName);
			lists.collectColor(meshGradientPatches[col].TR.colorName);
			lists.collectColor(meshGradientPatches[col].BL.colorName);
			lists.collectColor(meshGradientPatches[col].BR.colorName);
		}
	}
	if (GrTypeStroke == 0)
		lists.collectColor(lineColor());
	else if (GrTypeStroke < 8)
	{
		QList<VColorStop*> cstops = stroke_gradient.colorStops();
		for (uint cst = 0; cst < stroke_gradient.Stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
	if ((GrMask == 1) || (GrMask == 2) || (GrMask == 4) || (GrMask == 5))
	{
		QList<VColorStop*> cstops = mask_gradient.colorStops();
		for (uint cst = 0; cst < mask_gradient.Stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
	if (effectsInUse.count() != 0)
	{
		for (int a = 0; a < effectsInUse.count(); ++a)
		{
			QString tmpstr = effectsInUse.at(a).effectParameters;
			ScTextStream fp(&tmpstr, QIODevice::ReadOnly);
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
	lists.collectPattern(strokePattern());
	lists.collectLineStyle(customLineStyle());
	if (prevInChain() == NULL)
		itemText.getNamedResources(lists);
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
	if (RadRect > 0.0)
	{
		QPainterPath path;
		path.addRoundedRect(0, 0, Width, Height, rr, rr);
		PoLine.fromQPainterPath(path);
	}
	else
	{
		double bezierFactor = 0.552284749; //Bezier Control Point Factor: 8/3*(sin(45)-0.5)
		double rrxBezierFactor = rr*bezierFactor;
		double Width_rr = Width-rr;
		double Height_rr = Height-rr;
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

QTransform PageItem::getGroupTransform() const
{
	QTransform result;
	result.translate(gXpos, gYpos);
	result.rotate(Rot);
	return result;
}

void PageItem::getTransform(QTransform& mat) const
{
	mat.translate(Xpos, Ypos);
	mat.rotate(Rot);
}

QTransform PageItem::getTransform() const
{
//	QTransform result;
//	getTransform(result);
	return getCombinedTransform();
}

QTransform PageItem::getCombinedTransform() const
{
	QTransform result;
	if (Parent != NULL)
	{
		QList<const PageItem*> itList;
		const PageItem* ite = this;
		while (ite->Parent != NULL)
		{
			itList.prepend(ite);
			ite = ite->Parent;
		}
		result.translate(ite->xPos(), ite->yPos());
		result.rotate(ite->rotation());
		if (ite->isGroup() || ite->isSymbol())
		{
			if (ite->imageFlippedH())
			{
				result.translate(ite->width(), 0);
				result.scale(-1, 1);
			}
			if (ite->imageFlippedV())
			{
				result.translate(0, ite->height());
				result.scale(1, -1);
			}
		}
		if (ite == this)
			return result;
		if (ite->isGroup())
			result.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
		for (int aa = 0; aa < itList.count(); aa++)
		{
			ite = itList.at(aa);
			result.translate(ite->gXpos, ite->gYpos);
			result.rotate(ite->rotation());
			if (ite->isGroup() || ite->isSymbol())
			{
				if (ite->imageFlippedH())
				{
					result.translate(ite->width(), 0);
					result.scale(-1, 1);
				}
				if (ite->imageFlippedV())
				{
					result.translate(0, ite->height());
					result.scale(1, -1);
				}
			}
			if (ite == this)
				return result;
			if (ite->isGroup())
				result.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
		}
	}
	else
	{
		result.translate(Xpos, Ypos);
		result.rotate(Rot);
	}
	return result;
}

QRectF PageItem::getBoundingRect() const
{
	double x,y,x2,y2;
	getBoundingRect(&x, &y, &x2, &y2);
	return QRectF(x,y,x2-x,y2-y);
}

QRectF PageItem::getCurrentBoundingRect(double moreSpace) const
{
	double x = BoundingX - moreSpace / 2.0;
	double y = BoundingY - moreSpace / 2.0;
	double w = BoundingW + moreSpace;
	double h = BoundingH + moreSpace;
	QRectF ret = QRectF(x, y, w, h);
	return ret;
}

QRectF PageItem::getVisualBoundingRect() const
{
	double x,y,x2,y2;
	getVisualBoundingRect(&x, &y, &x2, &y2);
	return QRectF(x,y,x2-x,y2-y);
}

void PageItem::getBoundingRect(double *x1, double *y1, double *x2, double *y2) const
{
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
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
		*x2 = Xpos + qMax(1.0, qMax(Width, m_lineWidth));
		*y2 = Ypos + qMax(1.0, qMax(Height, m_lineWidth));
	}
	QRectF totalRect = QRectF(QPointF(*x1, *y1), QPointF(*x2, *y2));
	if (m_startArrowIndex != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_startArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(0, 0);
			arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
			if (NamedLStyle.isEmpty())
			{
				if (m_lineWidth != 0.0)
					arrowTrans.scale(m_lineWidth, m_lineWidth);
			}
			else
			{
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
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
					arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
					if (NamedLStyle.isEmpty())
					{
						if (m_lineWidth != 0.0)
							arrowTrans.scale(m_lineWidth, m_lineWidth);
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
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
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_endArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(Width, 0);
			arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
			if (NamedLStyle.isEmpty())
			{
				if (m_lineWidth != 0.0)
					arrowTrans.scale(m_lineWidth, m_lineWidth);
			}
			else
			{
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
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
					arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
					if (NamedLStyle.isEmpty())
					{
						if (m_lineWidth != 0.0)
							arrowTrans.scale(m_lineWidth, m_lineWidth);
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
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
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	double extraSpace = 0.0;
	if (NamedLStyle.isEmpty())
	{
		if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
		{
			if (isLine() && (PLineEnd == Qt::FlatCap))
				extraSpace = 0.0;
			else
				extraSpace = m_lineWidth / 2.0;
		}
		if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)) && (patternStrokePath))
		{
			ScPattern *pat = &m_Doc->docPatterns[patternStrokeVal];
			QTransform mat;
			mat.rotate(patternStrokeRotation);
			mat.scale(patternStrokeScaleX / 100.0, patternStrokeScaleY / 100.0);
			QRectF p1R = QRectF(0, 0, pat->width / 2.0, pat->height / 2.0);
			QRectF p2R = mat.map(p1R).boundingRect();
			extraSpace = p2R.height();
		}
	}
	else
	{
		multiLine ml = m_Doc->MLineStyles[NamedLStyle];
		struct SingleLine& sl = ml[ml.size()-1];
		if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
			extraSpace = sl.Width / 2.0;
	}
	if (Rot != 0)
	{
		FPointArray pb;
		pb.resize(0);
		pb.addPoint(FPoint(-extraSpace,				-extraSpace,			xPos(), yPos(), Rot, 1.0, 1.0));
		pb.addPoint(FPoint(visualWidth()-extraSpace,	-extraSpace,			xPos(), yPos(), Rot, 1.0, 1.0));
		pb.addPoint(FPoint(visualWidth()-extraSpace,	visualHeight()-extraSpace,	xPos(), yPos(), Rot, 1.0, 1.0));
		pb.addPoint(FPoint(-extraSpace, 				visualHeight()-extraSpace,	xPos(), yPos(), Rot, 1.0, 1.0));
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
		*x2 = *x1 + qMax(visualWidth(), extraSpace);
		*y2 = *y1 + qMax(visualHeight(), extraSpace);
	}
	QRectF totalRect(QPointF(*x1, *y1), QPointF(*x2, *y2));
	if (m_startArrowIndex != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_startArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(0, 0);
			arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
			if (NamedLStyle.isEmpty())
			{
				if (m_lineWidth != 0.0)
					arrowTrans.scale(m_lineWidth, m_lineWidth);
			}
			else
			{
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
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
					arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
					if (NamedLStyle.isEmpty())
					{
						if (m_lineWidth != 0.0)
							arrowTrans.scale(m_lineWidth, m_lineWidth);
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
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
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_endArrowIndex-1).points.copy();
		arrowTrans.translate(Xpos, Ypos);
		arrowTrans.rotate(Rot);
		if (itemType() == Line)
		{
			arrowTrans.translate(Width, 0);
			arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
			if (NamedLStyle.isEmpty())
			{
				if (m_lineWidth != 0.0)
					arrowTrans.scale(m_lineWidth, m_lineWidth);
			}
			else
			{
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
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
					arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
					if (NamedLStyle.isEmpty())
					{
						if (m_lineWidth != 0.0)
							arrowTrans.scale(m_lineWidth, m_lineWidth);
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
					arrow.map(arrowTrans);
					break;
				}
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		totalRect = totalRect.united(QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y())));
	}
	if (isPathText())
	{
		QTransform clipTrans;
		clipTrans.translate(Xpos, Ypos);
		clipTrans.rotate(Rot);
		totalRect = totalRect.united(QRectF(clipTrans.mapRect(Clip.boundingRect())));
	}
	totalRect.getCoords(x1, y1, x2, y2);
}

double PageItem::visualXPos() const
{
	double extraSpace = 0.0;
	if (!isLine())
	{
		if (NamedLStyle.isEmpty())
		{
			if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
				extraSpace = m_lineWidth / 2.0;
			if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)) && (patternStrokePath))
			{
				ScPattern *pat = &m_Doc->docPatterns[patternStrokeVal];
				QTransform mat;
				mat.rotate(patternStrokeRotation);
				mat.scale(patternStrokeScaleX / 100.0, patternStrokeScaleY / 100.0);
				QRectF p1R = QRectF(0, 0, pat->width / 2.0, pat->height / 2.0);
				QRectF p2R = mat.map(p1R).boundingRect();
				extraSpace = p2R.height();
			}
		}
		else
		{
			multiLine ml = m_Doc->MLineStyles[NamedLStyle];
			struct SingleLine& sl = ml[ml.size()-1];
			if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
				extraSpace = sl.Width / 2.0;
		}
	}
	if (isPathText())
		return qMin(Xpos + QRectF(Clip.boundingRect()).x(), Xpos - extraSpace);
	return Xpos - extraSpace;
}

double PageItem::visualYPos() const
{
	double extraSpace = 0.0;
	if (NamedLStyle.isEmpty())
	{
		if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
			extraSpace = m_lineWidth / 2.0;
		if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)) && (patternStrokePath))
		{
			ScPattern *pat = &m_Doc->docPatterns[patternStrokeVal];
			QTransform mat;
			mat.rotate(patternStrokeRotation);
			mat.scale(patternStrokeScaleX / 100.0, patternStrokeScaleY / 100.0);
			QRectF p1R = QRectF(0, 0, pat->width / 2.0, pat->height / 2.0);
			QRectF p2R = mat.map(p1R).boundingRect();
			extraSpace = p2R.height();
		}
	}
	else
	{
		multiLine ml = m_Doc->MLineStyles[NamedLStyle];
		struct SingleLine& sl = ml[ml.size()-1];
		if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
			extraSpace = sl.Width / 2.0;
	}
	if (isPathText())
		return qMin(Ypos + QRectF(Clip.boundingRect()).y(), Ypos - extraSpace);
	return Ypos - extraSpace;
}

double PageItem::visualWidth() const
{
	double extraSpace = 0.0;
	if (!isLine())
	{
		if (NamedLStyle.isEmpty())
		{
			if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
				extraSpace = m_lineWidth;
			if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)) && (patternStrokePath))
			{
				ScPattern *pat = &m_Doc->docPatterns[patternStrokeVal];
				QTransform mat;
				mat.rotate(patternStrokeRotation);
				mat.scale(patternStrokeScaleX / 100.0, patternStrokeScaleY / 100.0);
				QRectF p1R = QRectF(0, 0, pat->width, pat->height);
				QRectF p2R = mat.map(p1R).boundingRect();
				extraSpace = p2R.height();
			}
		}
		else
		{
			multiLine ml = m_Doc->MLineStyles[NamedLStyle];
			struct SingleLine& sl = ml[ml.size()-1];
			if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
				extraSpace = sl.Width;
		}
	}
	if (isPathText())
		return qMax(QRectF(Clip.boundingRect()).width(), Width + extraSpace);
	return Width + extraSpace;
}

double PageItem::visualHeight() const
{
	double extraSpace = 0.0;
	if (NamedLStyle.isEmpty())
	{
		if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
			extraSpace = m_lineWidth;
		if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)) && (patternStrokePath))
		{
			ScPattern *pat = &m_Doc->docPatterns[patternStrokeVal];
			QTransform mat;
			mat.rotate(patternStrokeRotation);
			mat.scale(patternStrokeScaleX / 100.0, patternStrokeScaleY / 100.0);
			QRectF p1R = QRectF(0, 0, pat->width, pat->height);
			QRectF p2R = mat.map(p1R).boundingRect();
			extraSpace = p2R.height();
		}
	}
	else
	{
		multiLine ml = m_Doc->MLineStyles[NamedLStyle];
		struct SingleLine& sl = ml[ml.size()-1];
		if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
			extraSpace = sl.Width;
	}
	if (isPathText())
		return qMax(QRectF(Clip.boundingRect()).height(), Height + extraSpace);
	return isLine() ? extraSpace : Height + extraSpace;
}

double PageItem::visualLineWidth()
{
	double extraSpace = 0.0;
	if (NamedLStyle.isEmpty())
	{
		if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
			extraSpace = m_lineWidth;
		if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)) && (patternStrokePath))
		{
			ScPattern *pat = &m_Doc->docPatterns[patternStrokeVal];
			QTransform mat;
			mat.rotate(patternStrokeRotation);
			mat.scale(patternStrokeScaleX / 100.0, patternStrokeScaleY / 100.0);
			QRectF p1R = QRectF(0, 0, pat->width, pat->height);
			QRectF p2R = mat.map(p1R).boundingRect();
			extraSpace = p2R.height();
		}
	}
	else
	{
		multiLine ml = m_Doc->MLineStyles[NamedLStyle];
		struct SingleLine& sl = ml[ml.size()-1];
		if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
			extraSpace = sl.Width;
	}
	return extraSpace;
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
	QTransform p;
	QRectF transRect;
	p.translate(Xpos * scale, Ypos*scale);
	p.scale(scale, scale);
	p.rotate(rotation());
	transRect = p.mapRect(QRectF(0.0, 0.0, width(), height()));
	return transRect.contains(x, y);
}

QString PageItem::getImageEffectsModifier() const
{
	bool first = true;
	QString buffer;
	QTextStream ts(&buffer);
	ScImageEffectList::const_iterator i = effectsInUse.begin();
	while (i != effectsInUse.end())
	{
		if (first)
			first = false;
		else
			ts << "/";
		ts << i->effectCode << ":" << i->effectParameters;
		i++;
	}
	return buffer;
}

bool PageItem::loadImage(const QString& filename, const bool reload, const int gsResolution, bool showMsg)
{
	bool useImage = (asImageFrame() != NULL);
	useImage |= (isAnnotation() && annotation().UseIcons());
	if (!useImage)
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
	cms.setUseEmbeddedProfile(UseEmbedded);
	cms.allowSoftProofing(true);
	ScImageCacheProxy imgcache(filename);
	imgcache.addModifier("lowResType", QString::number(pixm.imgInfo.lowResType));
	if (!effectsInUse.isEmpty())
		imgcache.addModifier("effectsInUse", getImageEffectsModifier());
	bool fromCache = false;
	if (!pixm.loadPicture(imgcache, fromCache, pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, gsRes, &dummy, showMsg))
	{
		Pfile = fi.absoluteFilePath();
		PictureIsAvailable = false;
//		PicArt = false;
		return false;
	}
	else
	{
		QString ext = fi.suffix().toLower();
		if (UndoManager::undoEnabled() && !reload)
		{
			ScItemState<ScImageEffectList> *is = new ScItemState<ScImageEffectList>(Um::GetImage, filename, Um::IGetImage);
			is->set("GET_IMAGE", "get_image");
			is->set("OLD_IMAGE_PATH", Pfile);
			is->set("NEW_IMAGE_PATH", filename);
			is->set("FLIPPH",imageFlippedH());
			is->set("FLIPPV",imageFlippedV());
			is->set("SCALING",ScaleType);
			is->set("ASPECT",AspectRatio);
			is->set("XOFF",imageXOffset());
			is->set("XSCALE",imageXScale());
			is->set("YOFF",imageYOffset());
			is->set("YSCALE",imageYScale());
			is->set("FILLT", fillTransparency());
			is->set("LINET", lineTransparency());
			is->setItem(effectsInUse);
			undoManager->action(this, is);
		}
		double xres = pixm.imgInfo.xres;
		double yres = pixm.imgInfo.yres;
		PictureIsAvailable = true;
//		PicArt = true;
		
		if (Pfile != filename)
		{
			oldLocalScX = LocalScX = 72.0 / xres;
			oldLocalScY = LocalScY = 72.0 / yres;
			oldLocalX = LocalX = 0;
			oldLocalY = LocalY = 0;
			if ((m_Doc->itemToolPrefs().imageUseEmbeddedPath) && (!pixm.imgInfo.clipPath.isEmpty()))
			{
				pixm.imgInfo.usedPath = pixm.imgInfo.clipPath;
				clPath = pixm.imgInfo.clipPath;
				if (pixm.imgInfo.PDSpathData.contains(clPath))
				{
					imageClip = pixm.imgInfo.PDSpathData[clPath].copy();
					pixm.imgInfo.usedPath = clPath;
					QTransform cl;
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
			QTransform cl;
			cl.translate(LocalX*LocalScX, LocalY*LocalScY);
			cl.scale(LocalScX, LocalScY);
			imageClip.map(cl);
		}
		BBoxX = pixm.imgInfo.BBoxX;
		BBoxH = pixm.imgInfo.BBoxH;
		if (fromCache)
		{
			OrigW = imgcache.getInfo("OrigW").toInt();
			OrigH = imgcache.getInfo("OrigH").toInt();
		}
		else
		{
			OrigW = pixm.width();
			OrigH = pixm.height();
			imgcache.addInfo("OrigW", QString::number(OrigW));
			imgcache.addInfo("OrigH", QString::number(OrigH));
		}
		isRaster = !(extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext));
		if (!isRaster)
		{
			effectsInUse.clear();
			imgcache.delModifier("effectsInUse");
		}
		UseEmbedded=pixm.imgInfo.isEmbedded;
		if (pixm.imgInfo.isEmbedded)
		{
			IProfile = "Embedded " + pixm.imgInfo.profileName;
			EmProfile = "Embedded " + pixm.imgInfo.profileName;
		}
		else
			IProfile = pixm.imgInfo.profileName;
		AdjustPictScale();
	}
	if (PictureIsAvailable && !fromCache)
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
			imgcache.addModifier("effectsInUse", getImageEffectsModifier());
		}
		pixm.applyEffect(effectsInUse, m_Doc->PageColors, false);
//		if (reload)
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
			if (pixm.createLowRes(scaling))
			{
				pixm.imgInfo.lowResScale = scaling;
				pixm.saveCache(imgcache);
			}
			else
				pixm.imgInfo.lowResScale = 1.0;
		}
	}
	if (PictureIsAvailable)
	{
		if (m_Doc->viewAsPreview)
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
					tmpC = defect.convertDefect(tmpC, m_Doc->previewVisual);
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
	double scp1 = p->zoomFactor() ;// / ScMW->view->scale();
	double ofwh = 6 * scp1;
	double ofx = Width - ofwh/2;
	double ofy = Height - ofwh*1.5;
	double bx1= ofx+ scp1;
	double by1= ofy+3 * scp1;
	double bw= 4*scp1;
	double bh= 2*scp1;
	p->setPen(Qt::black, 0.5 / p->zoomFactor(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->drawRect(ofx, ofy, ofwh, ofwh);
	p->setBrush(Qt::black);
	p->drawRect(bx1, by1, bw, bh);
	p->setPen(Qt::black, 1.5 / p->zoomFactor(), Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
	if (m_Locked)
		p->drawLine(FPoint(bx1+scp1/2, ofy+scp1), FPoint(bx1+scp1/2, by1));
	p->drawLine(FPoint(bx1+scp1*3.5, ofy+scp1), FPoint(bx1+scp1*3.5, by1));
	p->drawLine(FPoint(bx1+scp1/2, ofy+scp1), FPoint(bx1+scp1*3.5, ofy+scp1));
}

void PageItem::drawArrow(ScPainter *p, QTransform &arrowTrans, int arrowIndex)
{
	FPointArray arrow = m_Doc->arrowStyles().at(arrowIndex-1).points.copy();
	if (NamedLStyle.isEmpty())
	{
		if (m_lineWidth != 0.0)
			arrowTrans.scale(m_lineWidth, m_lineWidth);
	}
	else
	{
		multiLine ml = m_Doc->MLineStyles[NamedLStyle];
		if (ml[ml.size()-1].Width != 0.0)
			arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
	}
	arrow.map(arrowTrans);
	p->setupPolygon(&arrow);
	if (m_Doc->layerOutline(LayerID))
		p->strokePath();
	else
	{
		if (NamedLStyle.isEmpty())
		{
			if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)))
			{
				p->setPattern(&m_Doc->docPatterns[patternStrokeVal], patternStrokeScaleX, patternStrokeScaleY, patternStrokeOffsetX, patternStrokeOffsetY, patternStrokeRotation, patternStrokeSkewX, patternStrokeSkewY, patternStrokeMirrorX, patternStrokeMirrorY);
				p->setFillMode(ScPainter::Pattern);
				p->fillPath();
			}
			else if (GrTypeStroke > 0)
			{
				if ((!gradientStrokeVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientStrokeVal)))
					gradientStrokeVal = "";
				if (!(gradientStrokeVal.isEmpty()) && (m_Doc->docGradients.contains(gradientStrokeVal)))
					stroke_gradient = m_Doc->docGradients[gradientStrokeVal];
				if (stroke_gradient.Stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
				{
					if (lineColor() != CommonStrings::None)
					{
						p->setBrush(strokeQColor);
						p->setBrushOpacity(1.0 - lineTransparency());
						p->setLineWidth(0);
						p->setFillMode(ScPainter::Solid);
					}
					else
						p->setFillMode(ScPainter::None);
				}
				else
				{
					p->setFillMode(ScPainter::Gradient);
					p->fill_gradient = stroke_gradient;
					if (GrTypeStroke == 6)
						p->setGradient(VGradient::linear, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeStartX, GrStrokeStartY), GrStrokeScale, GrStrokeSkew);
					else
						p->setGradient(VGradient::radial, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeFocalX, GrStrokeFocalY), GrStrokeScale, GrStrokeSkew);
				}
				p->fillPath();
			}
			else if (lineColor() != CommonStrings::None)
			{
				p->setBrush(strokeQColor);
				p->setBrushOpacity(1.0 - lineTransparency());
				p->setLineWidth(0);
				p->setFillMode(ScPainter::Solid);
				p->fillPath();
			}
		}
		else
		{
			multiLine ml = m_Doc->MLineStyles[NamedLStyle];
			QColor tmp;
			if (ml[0].Color != CommonStrings::None)
			{
				SetQColor(&tmp, ml[0].Color, ml[0].Shade);
				p->setBrush(tmp);
				p->setLineWidth(0);
				p->setFillMode(ScPainter::Solid);
				p->fillPath();
			}
			for (int it = ml.size()-1; it > 0; it--)
			{
				if (ml[it].Color != CommonStrings::None)
				{
					SetQColor(&tmp, ml[it].Color, ml[it].Shade);
					p->setPen(tmp, ml[it].Width, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					p->strokePath();
				}
			}
		}
	}
}

void PageItem::AdjustPictScale()
{
	if (itemType() != PageItem::ImageFrame)
		return;
	if (ScaleType)
		return;
	if (OrigW == 0 || OrigH == 0)
		return;
//	LocalX = 0;
//	LocalY = 0;
//	LocalRot = 0;
	double xs = Width / static_cast<double>(OrigW);
	double ys = Height / static_cast<double>(OrigH);
	if (LocalRot != 0.0)
	{
		QRectF br = QRectF(0, 0, OrigW, OrigH);
		QTransform m;
		m.rotate(LocalRot);
		br = m.mapRect(br);
		xs = Width / br.width();
		ys = Height / br.height();
		QLineF wL = QLineF(0, 0, OrigW, 0);
		wL.setAngle(-LocalRot);
		QLineF hL = QLineF(0, 0, 0, OrigH);
		hL.setAngle(-LocalRot-90);
		QTransform mm;
		mm.scale(xs, ys);
		hL = mm.map(hL);
		wL = mm.map(wL);
		xs = wL.length() / static_cast<double>(OrigW);
		ys = hL.length() / static_cast<double>(OrigH);
		LocalX = -br.x();
		LocalY = -br.y();
	}
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
	// Disable broken code. Code must be independent from doc in that function
	/*switch (m_Doc->RotMode)
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
	}*/
	if (imageClip.size() != 0)
	{
		imageClip = pixm.imgInfo.PDSpathData[pixm.imgInfo.usedPath].copy();
		QTransform cl;
		cl.translate(LocalX*LocalScX, LocalY*LocalScY);
		cl.scale(LocalScX, LocalScY);
		imageClip.map(cl);
	}
}

void PageItem::setExternalFile(QString val)
{
	Pfile = val;
	if (!Pfile.isEmpty())
	{
		QString test = Pfile;
		if (QDir::isRelativePath(test))
			test = QDir::homePath()+"/"+Pfile;
		test = QDir::cleanPath(QDir::toNativeSeparators(test));
		QFileInfo pfi2(test);
		Pfile = pfi2.absoluteFilePath();
	}
}

void PageItem::setFileIconPressed(QString val)
{
	Pfile2 = val;
	if (!Pfile2.isEmpty())
	{
		QString test = QDir::cleanPath(QDir::toNativeSeparators(QDir::homePath()+"/"+Pfile2));
		QFileInfo pfi2(test);
		Pfile2 = pfi2.absoluteFilePath();
	}
}

void PageItem::setFileIconRollover(QString val)
{
	Pfile3 = val;
	if (!Pfile3.isEmpty())
	{
		QString test = QDir::cleanPath(QDir::toNativeSeparators(QDir::homePath()+"/"+Pfile3));
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
	QRect ret = QRect(0, 0, w - x, h - y);
	QTransform t = getCombinedTransform();
	ret = t.mapRect(ret);
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
			setGradientStartX(0);
			setGradientStartY(Height / 2.0);
			setGradientEndX(Width);
			setGradientEndY(Height / 2.0);
			break;
		case 2:
			setGradientStartX(Width / 2.0);
			setGradientStartY(0);
			setGradientEndX(Width / 2.0);
			setGradientEndY(Height);
			break;
		case 3:
			setGradientStartX(0);
			setGradientStartY(0);
			setGradientEndX(Width);
			setGradientEndY(Height);
			break;
		case 4:
			setGradientStartX(0);
			setGradientStartY(Height);
			setGradientEndX(Width);
			setGradientEndY(0);
			break;
		case 5:
			setGradientStartX(Width / 2.0);
			setGradientStartY(Height / 2.0);
			if (Width >= Height)
			{
				setGradientEndX(Width);
				setGradientEndY(Height / 2.0);
			}
			else
			{
				setGradientEndX(Width / 2.0);
				setGradientEndY(Height);
			}
			break;
		default:
			break;
	}
	//if (ScMW->view->SelItem.count()!=0 && this==ScMW->view->SelItem.at(0))
	//if (m_Doc->m_Selection->count()!=0 && m_Doc->m_Selection->primarySelectionIsMyself(this))
	//	ScMW->propertiesPalette->updateColorSpecialGradient();
	//CB Will only emit if connected, ie is first in GUI selection
	//unused double dur=m_Doc->unitRatio();
}

void PageItem::setPolyClip(int up, int down)
{
	if (PoLine.size() < 3)
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
	Segments.clear();
	QPainterPath pa = PoLine.toQPainterPath(false);
	QList<QPolygonF> polist = pa.toSubpathPolygons();
	for (int a = 0; a < polist.count(); a++)
	{
		QPolygon cli = polist[a].toPolygon();
		cl += cli;
		Segments.append(cl.size());
	}
	if (cl.size() > 1)
	{
		Clip.resize(0);
		for (int a = 0; a < cl.size()-1; ++a)
		{
			rot = xy2Deg(cl.point(a+1).x()-cl.point(a).x(),cl.point(a+1).y()-cl.point(a).y());
			QTransform ma;
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
	//unused PropertiesPalette* pp=m_Doc->scMW()->propertiesPalette;
	//connect(this, SIGNAL(myself(PageItem *)), pp, SLOT(setCurrentItem(PageItem *)));
	connect(this, SIGNAL(frameType(int)), m_Doc->scMW(), SLOT(HaveNewSel(int)));
	connect(this, SIGNAL(frameType(int)), m_Doc, SLOT(selectionChanged()));
	//connect(this, SIGNAL(frameType(int)), pp, SLOT(handleSelectionChanged()));
	//connect(this, SIGNAL(frameLocked(bool)), pp, SLOT(setLocked(bool)));
	//connect(this, SIGNAL(frameSizeLocked(bool)), pp, SLOT(setSizeLocked(bool)));
	//Shape signals
	//connect(this, SIGNAL(cornerRadius(double)), pp, SLOT(setRR(double)));
	//	connect(view, SIGNAL(ItemTextCols(int, double)), propertiesPalette, SLOT(setCols(int, double)));
	//Line signals
	//connect(this, SIGNAL(lineWidth(double)), pp, SLOT(setLineWidth(double)));
	//connect(this, SIGNAL(lineStyleCapJoin(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)), pp, SLOT( setLIvalue(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)));
	//Frame text signals
	//connect(this, SIGNAL(lineSpacing(double)), pp, SLOT(setLsp(double)));
	//connect(this, SIGNAL(textToFrameDistances(double, double, double, double)), pp, SLOT(setTextToFrameDistances(double, double, double, double)));
	//connect(this, SIGNAL(textKerning(double)), pp, SLOT(setExtra(double)));
	//connect(this, SIGNAL(textStyle(int)), pp, SLOT(setStil(int)));
	connect(this, SIGNAL(textStyle(int)), m_Doc->scMW(), SLOT(setStyleEffects(int)));
	//connect(this, SIGNAL(textFont(const QString&)), pp, SLOT(setFontFace(const QString&)));
	//connect(this, SIGNAL(textSize(double)), pp, SLOT(setSize(double)));

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

//CB unused in 135
//	double dur=m_Doc->unitRatio();
//	emit blendmode(fillBlendmodeVal, lineBlendmodeVal);
/*CB using the emit myself* instead of all of these
	emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
*/
	if (m_Doc->appMode != modeEdit)
	{
//TODO remove and use the emit myself
		//emit lineSpacing(itemText.defaultStyle().lineSpacing());
		//emit textKerning(itemText.defaultStyle().charStyle().tracking());
		emit textStyle(itemText.defaultStyle().charStyle().effects());
		//emit textFont(itemText.defaultStyle().charStyle().font().scName());
		//emit textSize(itemText.defaultStyle().charStyle().fontSize());
//		emit textFormatting(itemText.defaultStyle().alignment());
	}
}

void PageItem::setIsAnnotation(bool isAnnot)
{
	if (m_isAnnotation==isAnnot)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ActionPDF, 0, Um::IGroup);
		ss->set("ACTIONPDFANNOTATION", isAnnot);
		undoManager->action(this, ss);
	}
	m_isAnnotation=isAnnot;
}

void PageItem::setIsBookMark(bool isBM)
{
	if (isBookmark==isBM)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ActionPDF, 0, Um::IGroup);
		ss->set("ACTIONPDFBOOKMARK", isBM);
		undoManager->action(this, ss);
	}
	isBookmark=isBM;
}

void PageItem::setResolution(int id){
	if(pixm.imgInfo.lowResType==id)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ResTyp,"",Um::IImageFrame);
		ss->set("RES_TYP","res_typ");
		ss->set("OLD_RES",pixm.imgInfo.lowResType);
		ss->set("NEW_RES",id);
		undoManager->action(this,ss);
	}
	pixm.imgInfo.lowResType = id;
}

void PageItem::setAnnotation(const Annotation& ad)
{
	m_annotation=ad;
}

void PageItem::setImageShown(bool isShown)
{
	if(PicArt==isShown)
		return;
	if(UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ResTyp,"",Um::IImageFrame);
		ss->set("SHOW_IMAGE","show_image");
		ss->set("OLD",PicArt);
		undoManager->action(this,ss);
	}
	PicArt=isShown;
}

void PageItem::updateConstants()
{
	if (OwnPage!=-1)
	{
		m_Doc->constants().insert("pagewidth", m_Doc->Pages->at(OwnPage)->width());
		m_Doc->constants().insert("pageheight", m_Doc->Pages->at(OwnPage)->height());
		m_Doc->constants().insert("marginleft", m_Doc->Pages->at(OwnPage)->Margins.Left);
		m_Doc->constants().insert("marginright", m_Doc->Pages->at(OwnPage)->width() - m_Doc->Pages->at(OwnPage)->Margins.Right);
		m_Doc->constants().insert("margintop", m_Doc->Pages->at(OwnPage)->Margins.Top);
		m_Doc->constants().insert("marginbottom", m_Doc->Pages->at(OwnPage)->height() - m_Doc->Pages->at(OwnPage)->Margins.Bottom);
	}
	m_Doc->constants().insert("width", Width);
	m_Doc->constants().insert("height", Height);
}

//CB Old ScribusView MoveItemI
void PageItem::moveImageInFrame(double newX, double newY)
{
	if (m_ItemType!=PageItem::ImageFrame)
		return;
	if (locked())// || (!ScaleType))
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
		QTransform cl;
		cl.translate(imageXOffset()*imageXScale(), imageYOffset()*imageYScale());
		cl.rotate(imageRotation());
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
				QTransform ma;
				ma.scale(scx, scy);
				FPointArray gr;
				gr.addPoint(GrStartX, GrStartY);
				gr.addPoint(GrEndX, GrEndY);
				gr.addPoint(GrFocalX, GrFocalY);
				gr.addPoint(GrControl1);
				gr.addPoint(GrControl2);
				gr.addPoint(GrControl3);
				gr.addPoint(GrControl4);
				gr.addPoint(GrControl5);
				gr.map(ma);
				for (int grow = 0; grow < meshGradientArray.count(); grow++)
				{
					for (int gcol = 0; gcol < meshGradientArray[grow].count(); gcol++)
					{
						meshGradientArray[grow][gcol].transform(ma);
					}
				}
				for (int col = 0; col < meshGradientPatches.count(); col++)
				{
					meshGradientPatches[col].TL.transform(ma);
					meshGradientPatches[col].TR.transform(ma);
					meshGradientPatches[col].BL.transform(ma);
					meshGradientPatches[col].BR.transform(ma);
				}
				GrStartX = gr.point(0).x();
				GrStartY = gr.point(0).y();
				GrEndX = gr.point(1).x();
				GrEndY = gr.point(1).y();
				GrFocalX = gr.point(2).x();
				GrFocalY = gr.point(2).y();
				GrControl1 = gr.point(3);
				GrControl2 = gr.point(4);
				GrControl3 = gr.point(5);
				GrControl4 = gr.point(6);
				GrControl5 = gr.point(7);
				FPointArray gr2;
				gr2.addPoint(GrStrokeStartX, GrStrokeStartY);
				gr2.addPoint(GrStrokeEndX, GrStrokeEndY);
				gr2.addPoint(GrStrokeFocalX, GrStrokeFocalY);
				gr2.map(ma);
				GrStrokeStartX = gr2.point(0).x();
				GrStrokeStartY = gr2.point(0).y();
				GrStrokeEndX = gr2.point(1).x();
				GrStrokeEndY = gr2.point(1).y();
				GrStrokeFocalX = gr2.point(2).x();
				GrStrokeFocalY = gr2.point(2).y();
				FPointArray gr3;
				gr3.addPoint(GrMaskStartX, GrMaskStartY);
				gr3.addPoint(GrMaskEndX, GrMaskEndY);
				gr3.addPoint(GrMaskFocalX, GrMaskFocalY);
				gr3.map(ma);
				GrMaskStartX = gr3.point(0).x();
				GrMaskStartY = gr3.point(0).y();
				GrMaskEndX = gr3.point(1).x();
				GrMaskEndY = gr3.point(1).y();
				GrMaskFocalX = gr3.point(2).x();
				GrMaskFocalY = gr3.point(2).y();
				ContourLine.map(ma);
				if (FrameType > 2)
				{
					PoLine.map(ma);
					if (asPathText())
						updatePolyClip();
					else
						Clip = FlattenPath(PoLine, Segments);
				}
				for (int i = 0 ; i < weldList.count(); i++)
				{
					weldingInfo wInf = weldList.at(i);
					FPointArray gr4;
					FPoint wp = wInf.weldPoint;
					gr4.addPoint(wp);
					gr4.map(ma);
					double dx = gr4.point(0).x() - wp.x();
					double dy = gr4.point(0).y() - wp.y();
					moveWelded(dx, dy, i);
					wInf.weldPoint = gr4.point(0);
					weldList[i] = wInf;
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
			OldB2 = (OldB2==0)?1:OldB2;
			OldH2 = (OldH2==0)?1:OldH2;
			double scx = width() / OldB2;
			double scy = height() / OldH2;
			QTransform ma;
			ma.scale(scx, scy);
			FPointArray gr;
			gr.addPoint(GrStartX, GrStartY);
			gr.addPoint(GrEndX, GrEndY);
			gr.addPoint(GrFocalX, GrFocalY);
			gr.addPoint(GrControl1);
			gr.addPoint(GrControl2);
			gr.addPoint(GrControl3);
			gr.addPoint(GrControl4);
			gr.addPoint(GrControl5);
			gr.map(ma);
			for (int grow = 0; grow < meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < meshGradientArray[grow].count(); gcol++)
				{
					meshGradientArray[grow][gcol].transform(ma);
				}
			}
			for (int col = 0; col < meshGradientPatches.count(); col++)
			{
				meshGradientPatches[col].TL.transform(ma);
				meshGradientPatches[col].TR.transform(ma);
				meshGradientPatches[col].BL.transform(ma);
				meshGradientPatches[col].BR.transform(ma);
			}
			GrStartX = gr.point(0).x();
			GrStartY = gr.point(0).y();
			GrEndX = gr.point(1).x();
			GrEndY = gr.point(1).y();
			GrFocalX = gr.point(2).x();
			GrFocalY = gr.point(2).y();
			GrControl1 = gr.point(3);
			GrControl2 = gr.point(4);
			GrControl3 = gr.point(5);
			GrControl4 = gr.point(6);
			GrControl5 = gr.point(7);
			FPointArray gr2;
			gr2.addPoint(GrStrokeStartX, GrStrokeStartY);
			gr2.addPoint(GrStrokeEndX, GrStrokeEndY);
			gr2.addPoint(GrStrokeFocalX, GrStrokeFocalY);
			gr2.map(ma);
			GrStrokeStartX = gr2.point(0).x();
			GrStrokeStartY = gr2.point(0).y();
			GrStrokeEndX = gr2.point(1).x();
			GrStrokeEndY = gr2.point(1).y();
			GrStrokeFocalX = gr2.point(2).x();
			GrStrokeFocalY = gr2.point(2).y();
			FPointArray gr3;
			gr3.addPoint(GrMaskStartX, GrMaskStartY);
			gr3.addPoint(GrMaskEndX, GrMaskEndY);
			gr3.addPoint(GrMaskFocalX, GrMaskFocalY);
			gr3.map(ma);
			GrMaskStartX = gr3.point(0).x();
			GrMaskStartY = gr3.point(0).y();
			GrMaskEndX = gr3.point(1).x();
			GrMaskEndY = gr3.point(1).y();
			GrMaskFocalX = gr3.point(2).x();
			GrMaskFocalY = gr3.point(2).y();
			PoLine.map(ma);
			ContourLine.map(ma);
			if (asPathText())
				updatePolyClip();
			else
				Clip = FlattenPath(PoLine, Segments);
			OldB2 = width();
			OldH2 = height();
			for (int i = 0 ; i < weldList.count(); i++)
			{
				weldingInfo wInf = weldList.at(i);
				FPointArray gr4;
				FPoint wp = wInf.weldPoint;
				gr4.addPoint(wp);
				gr4.map(ma);
				double dx = gr4.point(0).x() - wp.x();
				double dy = gr4.point(0).y() - wp.y();
				moveWelded(dx, dy, i);
				wInf.weldPoint = gr4.point(0);
				weldList[i] = wInf;
			}
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
		if (UndoManager::undoEnabled())
		{
			ScItemState<QPair<FirstLineOffsetPolicy,FirstLineOffsetPolicy> > *is = new ScItemState<QPair <FirstLineOffsetPolicy,FirstLineOffsetPolicy> >(Um::FirstLineOffset);
			is->set("FIRSTLINEOFFSET", "fisrtlineoffset");
			is->setItem(qMakePair(firstLineOffsetP, flop));
			undoManager->action(this, is);
		}
		firstLineOffsetP = flop;
	}
}

void PageItem::setInlineData(QString data)
{
	QByteArray inlineImageData;
	inlineImageData.append(data);
	if (inlineImageData.size() > 0)
	{
		tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + inlineExt);
		tempImageFile->open();
		QString fileName = getLongPathName(tempImageFile->fileName());
		tempImageFile->close();
		inlineImageData = qUncompress(QByteArray::fromBase64(inlineImageData));
		QFile outFil(fileName);
		if (outFil.open(QIODevice::WriteOnly))
		{
			outFil.write(inlineImageData);
			outFil.close();
			isInlineImage = true;
			Pfile = fileName;
		}
	}
}

void PageItem::makeImageInline()
{
	QFileInfo fi(Pfile);
	QString ext = fi.suffix();
	tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + ext);
	tempImageFile->open();
	QString fileName = getLongPathName(tempImageFile->fileName());
	tempImageFile->close();
	isInlineImage = true;
	copyFile(Pfile, fileName);
	Pfile = fileName;
}

void PageItem::makeImageExternal(QString path)
{
	if ((tempImageFile) && (isInlineImage) && (!path.isEmpty()))
	{
		copyFile(Pfile, path);
		Pfile = path;
		isInlineImage = false;
		delete tempImageFile;
		tempImageFile = NULL;
	}
}

void PageItem::addWelded(PageItem* iPt)
{
	FPoint centerI = FPoint(xPos() + (width() / 2.0), yPos() + (height() / 2.0));
	FPoint centerP = FPoint(iPt->xPos() + (iPt->width() / 2.0), iPt->yPos() + (iPt->height() / 2.0));
	weldingInfo wInf;
	wInf.weldItem = iPt;
	wInf.weldPoint = FPoint((width() / 2.0) + ((centerP.x() - centerI.x()) / 2.0), (height() / 2.0) + ((centerP.y() - centerI.y()) / 2.0));
	weldList.append(wInf);
}

//welded frames
void PageItem::weldTo(PageItem* pIt)
{
	for (int i = 0 ; i <  weldList.count(); i++)
	{
		PageItem::weldingInfo wInf = weldList.at(i);
		if (wInf.weldItem == pIt)
			return;
	}
	QList<PageItem*> ret = pIt->itemsWeldedTo();
	if (ret.contains(this))
		return;
	addWelded(pIt);
	pIt->addWelded(this);
	update();
	pIt->update();
}

void PageItem::moveWelded(double DX, double DY, int weld)
{
	weldingInfo wInf = weldList.at(weld);
	PageItem *pIt = wInf.weldItem;
	pIt->setXPos(pIt->xPos() + DX);
	pIt->setYPos(pIt->yPos() + DY);
	pIt->update();
	pIt->moveWelded(DX, DY, this);
}

void PageItem::moveWelded(double DX, double DY, PageItem* except)
{
	for (int i = 0 ; i < weldList.count(); i++)
	{
		weldingInfo wInf = weldList.at(i);
		PageItem *pIt = wInf.weldItem;
		if (pIt != except)
		{
			pIt->setXPos(pIt->xPos() + DX);
			pIt->setYPos(pIt->yPos() + DY);
			pIt->update();
			pIt->moveWelded(DX, DY, this);
		}
	}
}

void PageItem::rotateWelded(double dR, double oldRot)
{
	QTransform ma;
	ma.translate(xPos(), yPos());
	ma.scale(1, 1);
	ma.rotate(oldRot);
	switch (m_Doc->RotMode())
	{
		case 2:
			ma.translate(width()/2.0, height()/2.0);
			break;
		case 4:
			ma.translate(width(), height());
			break;
		case 3:
			ma.translate(0, height());
			break;
		case 1:
			ma.translate(width(), 0);
			break;
	}
	QPointF rotCenter = ma.map(QPointF(0,0));
	QList<PageItem*> itemList = itemsWeldedTo(this);
	for (int a = 0; a < itemList.count(); a++)
	{
		PageItem *pIt = itemList.at(a);
		QLineF lin = QLineF(rotCenter, QPointF(pIt->xPos(), pIt->yPos()));
		lin.setAngle(lin.angle() - dR);
		pIt->setXYPos(lin.p2().x(), lin.p2().y());
		pIt->rotateBy(dR);
	}
}

QList<PageItem*> PageItem::itemsWeldedTo(PageItem* except)
{
	QList<PageItem*> ret;
	ret.clear();
	for (int i = 0 ; i < weldList.count(); i++)
	{
		weldingInfo wInf = weldList.at(i);
		PageItem *pIt = wInf.weldItem;
		if (pIt != except)
		{
			ret.append(pIt);
			if (pIt->isWelded())
				ret.append(pIt->itemsWeldedTo(this));
		}
	}
	return ret;
}

void PageItem::unWeld()
{
	for (int a = 0 ; a < weldList.count(); a++)
	{
		weldingInfo wInf = weldList.at(a);
		PageItem *pIt = wInf.weldItem;
		for (int b = 0 ; b < pIt->weldList.count(); b++)
		{
			weldingInfo wInf2 = pIt->weldList.at(b);
			PageItem *pIt2 = wInf2.weldItem;
			if (pIt2 == this)
			{
				pIt->weldList.removeAt(b);
				break;
			}
		}
	}
	weldList.clear();
}

