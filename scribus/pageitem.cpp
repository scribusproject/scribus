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

#include <QDebug>
#include <QFileInfo>
#include <QFont>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QPolygon>
#include <QRegExp>
#include <QRegion>
#include <QRegularExpression>
#include <QScopedPointer>
#include <cairo.h>
#include <cassert>
#include <qdrawutil.h>
#include <sstream>

#include "appmodes.h"
#include "canvas.h"
#include "canvasmode.h"
#include "cmsettings.h"
#include "colorblind.h"
#include "desaxe/saxXML.h"
#include "iconmanager.h"
#include "marks.h"
#include "pageitem_arc.h"
#include "pageitem_group.h"
#include "pageitem_latexframe.h"
#include "pageitem_line.h"
#include "pageitem_noteframe.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_spiral.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
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
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribuswin.h"
#include "sctextstream.h"
#include "selection.h"
#include "text/storytext.h"
#include "ui/contentpalette.h"
#include "ui/guidemanager.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "util.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_math.h"
#include "util_text.h"




using namespace std;

PageItem::PageItem(const PageItem & other)
	: QObject(other.parent()),
	 UndoObject(other),
	 SingleObservable<PageItem>(other.m_Doc->itemsChanged()),
	 TextContext(this),

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
	GrExtend(other.GrExtend),
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
	m_columns(other.m_columns),
	m_columnGap(other.m_columnGap),
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
	OwnPage(other.OwnPage),
	oldOwnPage(other.oldOwnPage),
	pixm(other.pixm),
	Pfile(other.Pfile),
	Pfile2(other.Pfile2),
	Pfile3(other.Pfile3),
	ImageProfile(other.ImageProfile),
	UseEmbedded(other.UseEmbedded),
	EmbeddedProfile(other.EmbeddedProfile),
	ImageIntent(other.ImageIntent),
	OverrideCompressionMethod(other.OverrideCompressionMethod),
	CompressionMethodIndex(other.CompressionMethodIndex),
	OverrideCompressionQuality(other.OverrideCompressionQuality),
	CompressionQualityIndex(other.CompressionQualityIndex),

	imageIsAvailable(other.imageIsAvailable),
	OrigW(other.OrigW),
	OrigH(other.OrigH),
	BBoxX(other.BBoxX),
	BBoxH(other.BBoxH),
	CurX(other.CurX),
	CurY(other.CurY),
	itemText(other.itemText),
	textLayout(&itemText, this),
	isBookmark(other.isBookmark),
	HasSel(other.HasSel),
	isAutoText(other.isAutoText),
	inPdfArticle(other.inPdfArticle),
	isRaster(other.isRaster),
	OldB(other.OldB),
	OldH(other.OldH),
	OldB2(other.OldB2),
	OldH2(other.OldH2),
	Sizing(other.Sizing),
	m_layerID(other.m_layerID),
	ScaleType(other.ScaleType),
	AspectRatio(other.AspectRatio),
	DashValues(other.DashValues),
	DashOffset(other.DashOffset),
	fill_gradient(other.fill_gradient),
	fillRule(other.fillRule),
	doOverprint(other.doOverprint),
	m_leftLink(other.m_leftLink),
	m_rightLink(other.m_rightLink),
	m_topLink(other.m_topLink),
	m_bottomLink(other.m_bottomLink),
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
	m_roundedCornerRadius(other.m_roundedCornerRadius),
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
	m_lineWidth(other.m_lineWidth),
	m_oldLineWidth(other.m_oldLineWidth),
	patternStrokeVal(other.patternStrokeVal),
	patternStrokeTransfrm(other.patternStrokeTransfrm),
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
	patternMaskTransfrm(other.patternMaskTransfrm),
	patternMaskMirrorX(other.patternMaskMirrorX),
	patternMaskMirrorY(other.patternMaskMirrorY),
	patternMaskVal(other.patternMaskVal),
	mask_gradient(other.mask_gradient),
	hatchAngle(other.hatchAngle),
	hatchDistance(other.hatchDistance),
	hatchType(other.hatchType),
	hatchUseBackground(other.hatchUseBackground),
	hatchBackground(other.hatchBackground),
	hatchForeground(other.hatchForeground),
	hatchBackgroundQ(other.hatchBackgroundQ),
	hatchForegroundQ(other.hatchForegroundQ),
	// protected
	undoManager(other.undoManager),
	m_textDistanceMargins(other.m_textDistanceMargins),
	verticalAlign(other.verticalAlign),
	m_itemType(other.m_itemType),
	m_itemName(other.m_itemName),
	m_isAnnotation(other.m_isAnnotation),
	m_annotation(other.m_annotation),
	m_gradientName(other.m_gradientName),
	m_patternName(other.m_patternName),
	m_patternTransform(other.m_patternTransform),
	m_patternMirrorX(other.m_patternMirrorX),
	m_patternMirrorY(other.m_patternMirrorY),
	m_fillColor(other.m_fillColor),
	m_lineColor(other.m_lineColor),
	m_lineShade(other.m_lineShade),
	m_fillShade(other.m_fillShade),
	m_fillTransparency(other.m_fillTransparency),
	m_lineTransparency(other.m_lineTransparency),
	m_fillBlendMode(other.m_fillBlendMode),
	m_lineBlendMode(other.m_lineBlendMode),
	m_ImageIsFlippedH(other.m_ImageIsFlippedH),
	m_ImageIsFlippedV(other.m_ImageIsFlippedV),
	m_Locked(other.m_Locked),
	m_SizeLocked(other.m_SizeLocked),
	m_SizeHLocked(other.m_SizeHLocked),
	m_SizeVLocked(other.m_SizeVLocked),
	m_textFlowMode(other.m_textFlowMode),
	pageItemAttributes(other.pageItemAttributes),
	m_PrintEnabled(other.m_PrintEnabled),
	m_fillQColor(other.m_fillQColor),
	m_strokeQColor(other.m_strokeQColor),
	m_grQColorP1(other.m_grQColorP1),
	m_grQColorP2(other.m_grQColorP2),
	m_grQColorP3(other.m_grQColorP3),
	m_grQColorP4(other.m_grQColorP4),
	m_xPos(other.m_xPos),
	m_yPos(other.m_yPos),
	m_width(other.m_width),
	m_height(other.m_height),
	m_rotation(other.m_rotation),
	m_isSelected(other.m_isSelected),
	m_imageVisible(other.m_imageVisible),
	m_imageXScale(other.m_imageXScale),
	m_imageYScale(other.m_imageYScale),
	m_imageXOffset(other.m_imageXOffset),
	m_imageYOffset(other.m_imageYOffset),
	m_imageRotation(other.m_imageRotation),
	m_firstLineOffset(other.m_firstLineOffset),
	m_groupClips(other.m_groupClips),
	m_startArrowIndex(other.m_startArrowIndex),
	m_endArrowIndex(other.m_endArrowIndex),
	m_startArrowScale(other.m_startArrowScale),
	m_endArrowScale(other.m_endArrowScale),
	m_hasSoftShadow(other.m_hasSoftShadow),
	m_softShadowColor(other.m_softShadowColor),
	m_softShadowShade(other.m_softShadowShade),
	m_softShadowBlurRadius(other.m_softShadowBlurRadius),
	m_softShadowXOffset(other.m_softShadowXOffset),
	m_softShadowYOffset(other.m_softShadowYOffset),
	m_softShadowOpacity(other.m_softShadowOpacity),
	m_softShadowBlendMode(other.m_softShadowBlendMode),
	m_softShadowErasedByObject(other.m_softShadowErasedByObject),
	m_softShadowHasObjectTransparency(other.m_softShadowHasObjectTransparency)
{
	QString tmp;
	m_imageVisible = m_Doc->guidesPrefs().showPic;
	m_Doc->TotalItems++;
	
	QString oldName(m_itemName);
	int nameNum = m_Doc->TotalItems;
	m_itemName += tmp.setNum(m_Doc->TotalItems);
	while (m_Doc->itemNameExists(m_itemName))
	{
		++nameNum;
		m_itemName = oldName + tmp.setNum(nameNum);
	}
	
	uniqueNr = m_Doc->TotalItems;
	invalid = true;
	if (other.isInlineImage)
	{
		QFileInfo inlFi(Pfile);
		QString ext = inlFi.suffix();
		QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + ext);
		tempFile->setAutoRemove(false);
		tempFile->open();
		QString fileName = getLongPathName(tempFile->fileName());
		tempFile->close();
		copyFile(Pfile, fileName);
		Pfile = fileName;
		delete tempFile;
		isInlineImage = true;
		isTempFile = true;
	}
	else
	{
		isInlineImage = false;
		isTempFile = false;
	}
	Parent = nullptr;
	unWeld();
}


PageItem::PageItem(ScribusDoc *doc, ItemType newType, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline)
	// Initialize superclass(es)
	: QObject(doc), SingleObservable<PageItem>(doc->itemsChanged()), TextContext(this),
	// Initialize member variables
	itemText(doc),
	textLayout(&itemText, this),
	undoManager(UndoManager::instance())
{
	m_Doc = doc;
	QString tmp;
	gXpos = oldXpos = m_xPos = x;
	gYpos = oldYpos = m_yPos = y;
	//CB Surely we can remove some of these?
	OldB2 = OldB = oldWidth = m_width = w;
	OldH2 = OldH = oldHeight = m_height = h;
	BoundingX = x;
	BoundingY = y;
	BoundingW = w;
	BoundingH = h;
	m_itemType = newType;
	m_fillColor = fill;
	m_lineColor = m_itemType == PageItem::TextFrame ? fill : outline;
	GrEndX = w;

	GrControl2 = FPoint(w, 0);
	GrControl3 = FPoint(w, h);
	GrControl4 = FPoint(0, h);
	GrControl5 = FPoint(w / 2.0, h / 2.0);
	GrStrokeEndX = w;
	m_lineWidth = w2;
	m_oldLineWidth = w2;
	PLineArt = Qt::PenStyle(m_Doc->itemToolPrefs().shapeLineStyle);
	PLineEnd = Qt::FlatCap;
	PLineJoin = Qt::MiterJoin;
	m_textDistanceMargins = m_Doc->itemToolPrefs().textDistances;
	pixm.imgInfo.lowResType = m_Doc->itemToolPrefs().imageLowResType;
	switch (m_itemType)
	{
		case Polygon:
			Clip.setPoints(4, static_cast<int>(w / 2), 0, static_cast<int>(w), static_cast<int>(h / 2),
								static_cast<int>(w / 2), static_cast<int>(h), 0, static_cast<int>(h / 2));
			break;
		default:
			Clip.setPoints(4, 0, 0, static_cast<int>(w), 0, static_cast<int>(w), static_cast<int>(h), 0, static_cast<int>(h));
			break;
	}
	OwnPage = m_Doc->currentPage() ? m_Doc->currentPage()->pageNr() : -1;
	oldOwnPage = OwnPage;
	savedOwnPage = OwnPage;
	m_imageVisible = m_Doc->guidesPrefs().showPic;
	
	switch (m_itemType)
	{
		case ImageFrame:
		case OSGFrame:
		case LatexFrame:
			//We can't determine if this is a latex frame here
			// because c++'s typeinfos are still saying it's
			// a plain pageitem
			// This is fixed in the PageItem_LatexFrame constructor
			m_itemName = tr("Image");
			setUPixmap(Um::IImageFrame);
			break;
		case TextFrame:
			m_itemName = tr("Text");
			setUPixmap(Um::ITextFrame);
			break;
		case Line:
			m_itemName = tr("Line");
			setUPixmap(Um::ILine);
			break;
		case Polygon:
			m_itemName = tr("Polygon");
			setUPixmap(Um::IPolygon);
			break;
		case PolyLine:
			m_itemName = tr("Polyline");
			setUPixmap(Um::IPolyline);
			break;
		case PathText:
			m_itemName = tr("PathText");
			setUPixmap(Um::IPathText);
			break;
		case Symbol:
			m_itemName = tr("Symbol");
			setUPixmap(Um::IPolygon);
			break;
		case Group:
			m_itemName = tr("Group");
			setUPixmap(Um::IPolygon);
			break;
		case RegularPolygon:
			m_itemName = tr("RegularPolygon");
			setUPixmap(Um::IPolygon);
			break;
		case Arc:
			m_itemName = tr("Arc");
			setUPixmap(Um::IPolygon);
			break;
		case Spiral:
			m_itemName = tr("Spiral");
			setUPixmap(Um::IPolygon);
			break;
		case Table:
			m_itemName = tr("Table");
			//setUPixmap(Um::IPolygon); // TODO: Fix this.
			break;
		default:
			m_itemName = "Item";
			break;
	}
	m_Doc->TotalItems++;
	
	QString oldName(m_itemName);
	int nameNum = m_Doc->TotalItems;
	m_itemName += tmp.setNum(m_Doc->TotalItems);
	while (m_Doc->itemNameExists(m_itemName))
	{
		++nameNum;
		m_itemName = oldName + tmp.setNum(nameNum);
	}
	
	uniqueNr = m_Doc->TotalItems;
	setUName(m_itemName);
	m_annotation.setBorderColor(outline);

	ImageIntent = Intent_Relative_Colorimetric;
	m_layerID = m_Doc->activeLayer();
	stroke_gradient = VGradient(VGradient::linear);
	stroke_gradient.clearStops();
	if (m_lineColor != CommonStrings::None)
	{
		const ScColor& col = m_Doc->PageColors[m_lineColor];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_lineColor, 100);
		stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_lineColor, 100);
	}
	else if (m_Doc->itemToolPrefs().shapeLineColor != CommonStrings::None)
	{
		const ScColor& col = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeLineColor];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
		stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
	}
	else if (m_fillColor != CommonStrings::None)
	{
		const ScColor& col = m_Doc->PageColors[m_fillColor];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		stroke_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_fillColor, 100);
		stroke_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_fillColor, 100);
	}
	else if (m_Doc->itemToolPrefs().shapeFillColor != CommonStrings::None)
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
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();
	if (m_fillColor != CommonStrings::None)
	{
		const ScColor& col = m_Doc->PageColors[m_fillColor];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_fillColor, 100);
		fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_fillColor, 100);
		GrColorP1 = m_fillColor;
		GrColorP2 = m_fillColor;
		GrColorP3 = m_fillColor;
		GrColorP4 = m_fillColor;
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
			if (m_lineColor != CommonStrings::None)
			{
				const ScColor& col = m_Doc->PageColors[m_lineColor];
				QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
				fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, m_lineColor, 100);
				fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, m_lineColor, 100);
				GrColorP1 = m_lineColor;
				GrColorP2 = m_lineColor;
				GrColorP3 = m_lineColor;
				GrColorP4 = m_lineColor;
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
	GrMaskEndX = w;
	mask_gradient = VGradient(VGradient::linear);
	mask_gradient.clearStops();
	const ScColor& col = m_Doc->PageColors["Black"];
	QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
	mask_gradient.addStop(qcol, 0.0, 0.5, 1.0, "Black", 100);
	mask_gradient.addStop(qcol, 1.0, 0.5, 1.0, "Black", 100);

	QList<MeshPoint> mgList;
	MeshPoint mgP;
	meshGradientPatch patch;
	mgP.resetTo(FPoint(0.0, 0.0));
	mgP.transparency = 1.0;
	mgP.shade = 100;
	mgP.colorName = "Black";
	mgP.color = qcol;
	mgList.append(mgP);
	patch.TL = mgP;
	mgP.resetTo(FPoint(m_width / 2.0, 0.0));
	mgList.append(mgP);
	mgP.resetTo(FPoint(m_width, 0.0));
	mgList.append(mgP);
	patch.TR = mgP;
	meshGradientArray.append(mgList);
	mgList.clear();
	mgP.resetTo(FPoint(0.0, m_height / 2.0));
	mgList.append(mgP);
	mgP.resetTo(FPoint(m_width / 2.0, m_height / 2.0));
	mgList.append(mgP);
	mgP.resetTo(FPoint(m_width, m_height / 2.0));
	mgList.append(mgP);
	meshGradientArray.append(mgList);
	mgList.clear();
	mgP.resetTo(FPoint(0.0, m_height));
	mgList.append(mgP);
	patch.BL = mgP;
	mgP.resetTo(FPoint(m_width / 2.0, m_height));
	mgList.append(mgP);
	mgP.resetTo(FPoint(m_width, m_height));
	mgList.append(mgP);
	patch.BR = mgP;
	meshGradientArray.append(mgList);
	meshGradientPatches.append(patch);

	m_firstLineOffset = m_Doc->itemToolPrefs().firstLineOffset;
	m_columns = m_Doc->itemToolPrefs().textColumns;
	m_columnGap = m_Doc->itemToolPrefs().textColumnGap;

	OnMasterPage = m_Doc->currentPage() ? m_Doc->currentPage()->pageName() : QString();
	m_startArrowIndex = m_Doc->itemToolPrefs().lineStartArrow;
	m_endArrowIndex = m_Doc->itemToolPrefs().lineEndArrow;
	effectsInUse.clear();

	//Page Item Attributes
	pageItemAttributes.clear();
	for (const ObjectAttribute& objAttr : qAsConst(m_Doc->itemAttributes()))
	{
		if ((objAttr.autoaddto == "textframes" && m_itemType == TextFrame) ||
			(objAttr.autoaddto == "imageframes" && m_itemType == ImageFrame))
		{
			pageItemAttributes.append(objAttr);
		}
	}

	hatchBackground = CommonStrings::None;
	hatchBackgroundQ = QColor();
	hatchForeground = "Black";
	hatchForegroundQ = qcol;
}

PageItem::~PageItem()
{
	if (isTempFile && !Pfile.isEmpty())
		QFile::remove(Pfile);
	//remove marks

	if (isTextFrame())
	{
		if (!asTextFrame()->isInChain() && itemText.length() > 0)
		{
			for (int pos = 0; pos < itemText.length(); ++pos)
			{
				if (itemText.hasMark(pos))
				{
					Mark* mrk = itemText.mark(pos);
					if (!mrk->isType(MARKBullNumType))
						m_Doc->eraseMark(mrk);
				}
			}
		}
	}
//		if (isWeld())
//			unWeldFromMaster(true);
//		if (isWelded())
//			unWeldChild();
}

bool PageItem::isMasterItem() const
{
	if (Parent == nullptr)
		return !OnMasterPage.isEmpty();

	PageItem* parentItem = Parent;
	while (parentItem && parentItem->Parent)
		parentItem = parentItem->Parent;
	return !parentItem->OnMasterPage.isEmpty();
}

bool PageItem::isGroupChild() const
{
	return (dynamic_cast<PageItem_Group*>(Parent) != nullptr);
}

bool PageItem::isTableCell() const
{
	return (dynamic_cast<PageItem_Table*>(Parent) != nullptr);
}

void PageItem::setXPos(const double newXPos, bool drawingOnly)
{
	m_xPos = newXPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setYPos(const double newYPos, bool drawingOnly)
{
	m_yPos = newYPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setXYPos(const double newXPos, const double newYPos, bool drawingOnly)
{
	m_xPos = newXPos;
	m_yPos = newYPos;
	if (drawingOnly || m_Doc->isLoading())
		return;
	checkChanges();
}

int PageItem::level() const
{
	PageItem* thisItem = const_cast<PageItem*>(this);
	if (isGroupChild())
		return (Parent->asGroupFrame()->groupItemList.indexOf(thisItem) + 1);
	if (!m_Doc)
		return 0;
	const auto* items = OnMasterPage.isEmpty() ? &m_Doc->DocItems : &m_Doc->MasterItems;
	return (items->indexOf(thisItem) + 1);
}

void PageItem::moveBy(const double dX, const double dY, bool drawingOnly)
{
	//qDebug() << "pageitem::moveby" << dX << dY;
	if (dX == 0.0 && dY == 0.0)
		return;
	invalid = true;
	if (dX != 0.0)
	{
		m_xPos += dX;
		gXpos += dX;
		BoundingX += dX;
	}
	if (dY != 0.0)
	{
		m_yPos += dY;
		gYpos += dY;
		BoundingY += dY;
	}
	if (drawingOnly || m_Doc->isLoading())
		return;
	moveWelded(dX, dY);
	checkChanges();
}

void PageItem::setWidth(double newWidth)
{
	m_width = newWidth;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setHeight(double newHeight)
{
	m_height = newHeight;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setWidthHeight(double newWidth, double newHeight, bool drawingOnly)
{
	m_width = newWidth;
	m_height = newHeight;
	updateConstants();
	if (drawingOnly)
		return;
	checkChanges();
}

void PageItem::setWidthHeight(const double newWidth, const double newHeight)
{
	m_width = newWidth;
	m_height = newHeight;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::resizeBy(const double dH, const double dW)
{
	if (dH == 0.0 && dW == 0.0)
		return;
	if (dH != 0.0)
		m_width += dH;
	if (dW != 0.0)
		m_height += dW;
	updateConstants();
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setRotation(const double newRotation, bool drawingOnly)
{
	double dR = newRotation - m_rotation;
	double oldRot = m_rotation;
	m_rotation = newRotation;
	while (m_rotation < 0.0)
		m_rotation += 360.0;
	while (m_rotation > 360.0)
		m_rotation -= 360.0;
	if (drawingOnly || m_Doc->isLoading())
		return;
	rotateWelded(dR, oldRot);
	checkChanges();
}

void PageItem::rotateBy(const double dR)
{
	if (dR==0.0)
		return;
	m_rotation += dR;
	while (m_rotation < 0.0)
		m_rotation += 360.0;
	while (m_rotation > 360.0)
		m_rotation -= 360.0;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setSelected(const bool toSelect)
{
	m_isSelected = toSelect;
}

void PageItem::setImageXScale(const double newImageXScale)
{
	m_imageXScale = newImageXScale;
	if (m_Doc->isLoading())
	{
		oldLocalScX = m_imageXScale;
		return;
	}
	checkChanges();
}

void PageItem::setImageYScale(const double newImageYScale)
{
	m_imageYScale = newImageYScale;
	if (m_Doc->isLoading())
	{
		oldLocalScY = m_imageYScale;
		return;
	}
	checkChanges();
}

void PageItem::setImageXYScale(const double newImageXScale, const double newImageYScale)
{
	m_imageXScale = newImageXScale;
	m_imageYScale = newImageYScale;
	if (m_Doc->isLoading())
	{
		oldLocalScX = m_imageXScale;
		oldLocalScY = m_imageYScale;
		return;
	}
	checkChanges();
}

void PageItem::setImageXOffset(const double newImageXOffset)
{
	m_imageXOffset = newImageXOffset;
	if (m_Doc->isLoading())
	{
		oldLocalX = m_imageXOffset;
		return;
	}
	checkChanges();
}

void PageItem::setImageYOffset(const double newImageYOffset)
{
	m_imageYOffset = newImageYOffset;
	if (m_Doc->isLoading())
	{
		oldLocalY = m_imageYOffset;
		return;
	}
	checkChanges();
}

void PageItem::setImageXYOffset(const double newImageXOffset, const double newImageYOffset)
{
	m_imageXOffset = newImageXOffset;
	m_imageYOffset = newImageYOffset;
	if (m_Doc->isLoading())
	{
		oldLocalX = m_imageXOffset;
		oldLocalY = m_imageYOffset;
		return;
	}
	checkChanges();
}

void PageItem::moveImageXYOffsetBy(const double dX, const double dY)
{
	if (dX==0.0 && dY==0.0)
		return;
	if (dX != 0.0)
		m_imageXOffset += dX;
	if (dY != 0.0)
		m_imageYOffset += dY;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}

void PageItem::setImageRotation(const double newRotation)
{
	if (m_imageRotation == newRotation)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::Rotate, QString(), Um::IRotate);
		ss->set("IMAGE_ROTATION");
		ss->set("OLD_ROT", m_imageRotation);
		ss->set("NEW_ROT", newRotation);
		undoManager->action(this, ss);
	}
	m_imageRotation = newRotation;
	if (m_Doc->isLoading())
		return;
	checkChanges();
}


/// tests if a character is displayed by this frame
bool PageItem::frameDisplays(int textpos) const
{
	return 0 <= textpos && textpos < signed(m_maxChars) && textpos < itemText.length();
}

PageItem* PageItem::frameOfChar(int textPos)
{
	PageItem* firstFrame = this->firstInChain();
	PageItem* nextFrame = firstFrame;

	while (nextFrame)
	{
		if (nextFrame->invalid)
			nextFrame->layout();
		if (nextFrame->frameDisplays(textPos))
			return nextFrame;
		nextFrame = nextFrame->nextInChain();
	}

	return nullptr;
}

//return frame where is text end
PageItem * PageItem::frameTextEnd()
{
	PageItem * LastBox = this;
	if (frameOverflows() && m_nextBox)
	{ // text ending in some next frame
		LastBox = m_nextBox;
		while (LastBox != nullptr && !LastBox->frameDisplays(itemText.length()-1))
			LastBox = LastBox->nextInChain();
	}
	else if (frameUnderflows() && m_backBox)
	{ //text ending in some previous frame
		LastBox = m_backBox;
		while (LastBox != nullptr && !LastBox->frameDisplays(itemText.length()-1))
			LastBox = LastBox->prevInChain();
	}
	return LastBox;
}

/// returns true if text overflows
bool PageItem::frameOverflows() const
{
	// Fix #6991 : "Text overflow" warning when there is a text underflow in fact
	/*return NextBox == nullptr && itemText.length() > static_cast<int>(MaxChars);*/
	return ( m_nextBox == nullptr )
		   && ( firstChar < itemText.length() )
		   // Fix #7766 : scribus.textOverflows() returns 0 if there is no place for the overflow mark
		   /*&& ( firstChar < MaxChars )*/
		   && ( firstChar <= m_maxChars )
		   && ( itemText.length() > m_maxChars );
}

int PageItem::frameOverflowCount() const
{
	if (frameOverflows())
		return itemText.length()-m_maxChars;
	return 0;
}

int PageItem::frameOverflowBlankCount() const
{
	if (frameOverflows())
		return itemText.plainText().right(itemText.length() - m_maxChars).count(QRegularExpression("\\s+"));
	return 0;
}

int PageItem::maxCharsInFrame() const
{
	return m_maxChars;
}

/// returns true if text is ending before that frame
bool PageItem::frameUnderflows() const
{
	if (m_backBox == nullptr)
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
	qreal left = m_width - sideLength-offset;// / 2;
	qreal right = left + sideLength;
	qreal top = m_height - sideLength-offset;// * 1.5;
	qreal bottom = top + sideLength;

	QColor color(PrefsManager::instance().appPrefs.displayPrefs.frameNormColor);
	if (isBookmark || m_isAnnotation)
		color = PrefsManager::instance().appPrefs.displayPrefs.frameAnnotationColor;
	if ((m_backBox != nullptr) || (m_nextBox != nullptr))
		color = PrefsManager::instance().appPrefs.displayPrefs.frameLinkColor;
	if (m_Locked)
		color = PrefsManager::instance().appPrefs.displayPrefs.frameLockColor;
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
	return qMin(signed(m_maxChars), itemText.length()) - 1;
}

bool PageItem::canBeLinkedTo(const PageItem* nxt) const
{
	if (this->nextInChain() )
		return false;
	if (!nxt || nxt->prevInChain() )
		return false;
	for (const PageItem* ff = nxt; ff; ff = ff->nextInChain())
	{
		if (ff == this)
			return false;
	}
	// If object is placed on a master page, it can be linked only to objects placed on same master page
	// Same for objects placed on standard pages : they can only be linked to objects placed on standard pages
	return OnMasterPage == nxt->OnMasterPage;
}

void PageItem::link(PageItem* nxt, bool addPARSEP)
{
	if (nxt == nullptr)
		return;

	assert( !nextInChain() );
	assert( !nxt->prevInChain() );
	for (PageItem* ff = nxt; ff; ff = ff->nextInChain())
	{
		assert(ff != this);
	}

	// Append only if necessary to avoid the
	// charstyle: access at end of text warning
	bool first = false;
	bool createUndo = addPARSEP;

	if (nxt->prevInChain() == nullptr)
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
	else
		addPARSEP = false;
	m_nextBox = nxt;
	nxt->m_backBox = this;
	// update AutoText
	if (isAutoText)
	{
		PageItem* after = nxt;
		while (after)
		{
			after->isAutoText = true;
			m_Doc->LastAuto = after;
			after = after->m_nextBox;
		}
	}
	else if (nxt->isAutoText)
	{
		PageItem* before = this;
		while (before)
		{
			before->isAutoText = true;
			m_Doc->FirstAuto = before;
			before = before->m_backBox;
		}
	}
	invalid = true;
	PageItem* prev = this;
	while (prev->m_backBox && !prev->m_backBox->frameOverflows())
	{
		prev->m_backBox->invalid = true;
		prev = prev->m_backBox;
	}
	while (nxt)
	{
		nxt->itemText = itemText;
		nxt->invalid = true;
		nxt->firstChar = 0;
		nxt = nxt->m_nextBox;
	}
	if (UndoManager::undoEnabled() && createUndo) //addPARESEP is false only if linking is invoked from undo action for unlinkWithText
	{
		auto *is = new ScItemState<QPair<PageItem*, PageItem*> >(Um::LinkTextFrame);
		is->set("LINK_TEXT_FRAME");
		is->set("FIRST", first);
		is->set("JOIN_POS", textLen);
		is->set("ADDPARSEP", addPARSEP);
		is->setItem(qMakePair(this, m_nextBox));
		undoManager->action(this, is);
	}
}

void PageItem::unlink(bool createUndo)
{
	if (m_nextBox)
	{
		PageItem *undoNextBox = m_nextBox;
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
				before = before->m_backBox;
			}
			m_Doc->FirstAuto = m_nextBox;
		}
		// link following frames to new text
		m_nextBox->firstChar = 0;
		m_nextBox->m_backBox = nullptr;
		while (m_nextBox)
		{
			m_nextBox->itemText = follow;
			m_nextBox->invalid = true;
			m_nextBox->firstChar = 0;
			m_nextBox = m_nextBox->m_nextBox;
		}
		// NextBox == nullptr now
		m_nextBox = nullptr;
		if (UndoManager::undoEnabled() && createUndo)
		{
			auto *is = new ScItemState<QPair<PageItem*, PageItem*> >(Um::UnlinkTextFrame);
			is->set("UNLINK_TEXT_FRAME");
			is->setItem(qMakePair(this, undoNextBox));
			undoManager->action(this, is);
		}
	}
}

void PageItem::dropLinks()
{
	// update auto pointers
	if (isAutoText && m_nextBox == nullptr)
		m_Doc->LastAuto = m_backBox;
	if (isAutoText && m_backBox == nullptr)
		m_Doc->FirstAuto = m_nextBox;
	isAutoText = false;

	// leave text in remaining chain
	PageItem* before = m_backBox;
	PageItem* after = m_nextBox;
	if (after == nullptr && before == nullptr)
		return;

	itemText = StoryText(m_Doc);
	
	int afterChar = 0;
	if (before)
	{
		before->m_nextBox = after;
		afterChar = qMin((int) before->m_maxChars, before->itemText.length());
	}
	if (after) 
	{
		after->m_backBox = before;
		while (after)
		{ 
			after->invalid = true;
			after->firstChar = afterChar;
			after = after->m_nextBox;
		}
	}

	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<QPair<PageItem*, PageItem*> >(Um::UnlinkTextFrame);
		is->set("DROP_LINKS");
		is->setItem(qMakePair(m_backBox, m_nextBox));
		undoManager->action(this, is);
	}

	// JG we should set BackBox and NextBox to nullptr at a point
	m_backBox = m_nextBox = nullptr;
}

//unlink selected frame from text chain
//but copy or cut its content from itemText
void PageItem::unlinkWithText()
{
	PageItem* next = m_nextBox;
	PageItem* prev = m_backBox;
	int length = itemText.length();

	if (this->invalid)
		layout();

	//unlink first frame in chain
	if (prev == nullptr)
	{
		if (next->invalid)
			next->layout();
		if (lastInFrame() < length -1)
		{
			StoryText content(m_Doc);
			itemText.select(lastInFrame() + 1, length - (lastInFrame() + 1));
			content.insert(0, itemText, true);
			itemText.removeSelection();
			unlink(false);
			next->itemText.insert(0, content);
			next->update();
		}
		else
		{
			unlink(false);
		}
	}
	else
	{
		if (prev->invalid)
			prev->layout();
		itemText.select(firstInFrame(), length - firstInFrame());
		StoryText content(m_Doc);
		content.insert(0, itemText, true);
		itemText.removeSelection();
		prev->unlink(false);
		itemText.insert(0, content);
		update();
	}
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<QPair<PageItem*, PageItem*> >(Um::UnlinkTextFrame);
		is->set("UNLINK_TEXT_FRAME");
		is->set("CUT_TEXT", true);
		is->setItem(qMakePair(prev, next));
		undoManager->action(this, is);
	}
}

/// returns the style at the current charpos
const ParagraphStyle& PageItem::currentStyle() const
{
	int cursorPosition = itemText.cursorPosition();
	if (itemText.hasSelection())
	{
		int firstSelected = itemText.startOfSelection();
		int lastSelected  = qMax(itemText.endOfSelection() - 1, 0);
		cursorPosition = qMax(firstSelected, qMin(cursorPosition, lastSelected));
	}
	// Note: cursor position can be past last characters, don't use frameDisplays() here
	if (cursorPosition >= 0 && cursorPosition <= itemText.length())
		return itemText.paragraphStyle(cursorPosition);
	return itemText.defaultStyle();
}

/// returns the style at the current charpos for changing
ParagraphStyle& PageItem::changeCurrentStyle()
{
	int cursorPosition = itemText.cursorPosition();
	if (itemText.hasSelection())
	{
		int firstSelected = itemText.startOfSelection();
		int lastSelected  = qMax(itemText.endOfSelection() - 1, 0);
		cursorPosition = qMax(firstSelected, qMin(cursorPosition, lastSelected));
	}
	// Note: cursor position can be past last characters, don't use frameDisplays() here
	if (cursorPosition >= 0 && cursorPosition <= itemText.length())
		return const_cast<ParagraphStyle&>(itemText.paragraphStyle(cursorPosition));
	return const_cast<ParagraphStyle&>(itemText.defaultStyle());
}

/// returns the style at the current charpos
const CharStyle& PageItem::currentCharStyle() const
{
	int cursorPosition = itemText.cursorPosition();
	if (itemText.hasSelection())
	{
		int firstSelected = itemText.startOfSelection();
		int lastSelected  = qMax(itemText.endOfSelection() - 1, 0);
		cursorPosition = qMax(firstSelected, qMin(cursorPosition, lastSelected));
	}
	// Note: cursor position can be past last characters, don't use frameDisplays() here
	if (cursorPosition >= 0 && cursorPosition <= itemText.length())
		return itemText.charStyle(cursorPosition);
	return itemText.defaultStyle().charStyle();
}

void PageItem::currentTextProps(ParagraphStyle& parStyle) const
{
	const ParagraphStyle& curStyle = this->currentStyle();
	parStyle.setContext(curStyle.context());
	parStyle = curStyle;

	int position = itemText.cursorPosition();
	if (itemText.hasSelection())
		position = qMin(qMax(itemText.endOfSelection() - 1, 0), qMax(position, itemText.startOfSelection()));

	// Note: cursor position can be past last characters, don't use frameDisplays() here
	if (position >= 0 && position <= itemText.length())
	{
		// Do not use setStyle here otherwise char style properties explicitly
		// set at paragraph level without using styles might get lost
		parStyle.charStyle().applyCharStyle( itemText.charStyle(position) );
	}
	else
		parStyle.charStyle().setStyle( itemText.defaultStyle().charStyle() );
}

void PageItem::setTextToFrameDistLeft(double newLeft)
{
	if (m_textDistanceMargins.left() == newLeft)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TextFrameDist, QString(), Um::ITextFrame);
		ss->set("LEFT_TEXTFRAMEDIST");
		ss->set("OLD_DIST", m_textDistanceMargins.left());
		ss->set("NEW_DIST", newLeft);
		undoManager->action(this, ss);
	}
	m_textDistanceMargins.setLeft(newLeft);
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistRight(double newRight)
{
	if (m_textDistanceMargins.right() == newRight)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TextFrameDist, QString(), Um::ITextFrame);
		ss->set("RIGHT_TEXTFRAMEDIST");
		ss->set("OLD_DIST", m_textDistanceMargins.right());
		ss->set("NEW_DIST", newRight);
		undoManager->action(this, ss);
	}
	m_textDistanceMargins.setRight(newRight);
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistTop(double newTop)
{
	if (m_textDistanceMargins.top() == newTop)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TextFrameDist, QString(), Um::ITextFrame);
		ss->set("TOP_TEXTFRAMEDIST");
		ss->set("OLD_DIST", m_textDistanceMargins.top());
		ss->set("NEW_DIST", newTop);
		undoManager->action(this, ss);
	}
	m_textDistanceMargins.setTop(newTop);
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDistBottom(double newBottom)
{
	if (m_textDistanceMargins.bottom() == newBottom)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TextFrameDist, QString(), Um::ITextFrame);
		ss->set("BOTTOM_TEXTFRAMEDIST");
		ss->set("OLD_DIST", m_textDistanceMargins.bottom());
		ss->set("NEW_DIST", newBottom);
		undoManager->action(this, ss);
	}
	m_textDistanceMargins.setBottom(newBottom);
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

void PageItem::setTextToFrameDist(double newLeft, double newRight, double newTop, double newBottom)
{
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = undoManager->beginTransaction(Um::TextFrame, Um::IDocument, Um::TextFrameDist, QString(), Um::ITextFrame);
	setTextToFrameDistLeft(newLeft);
	setTextToFrameDistRight(newRight);
	setTextToFrameDistTop(newTop);
	setTextToFrameDistBottom(newBottom);
	if (activeTransaction)
		activeTransaction.commit();
	//emit textToFrameDistances(Extra, TExtra, BExtra, RExtra);
}

double PageItem::gridOffset() const
{
	return m_Doc->guidesPrefs().offsetBaselineGrid;
}
double PageItem::gridDistance() const
{
	return m_Doc->guidesPrefs().valueBaselineGrid;
}

void PageItem::setGridOffset(double) { } // FIXME
void PageItem::setGridDistance(double) { } // FIXME

void PageItem::setColumns(int newColumnCount)
{
	if (m_columns == newColumnCount)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::Columns, QString(), Um::IBorder);
		ss->set("COLUMNS");
		ss->set("OLD_COLUMNS", m_columns);
		ss->set("NEW_COLUMNS", newColumnCount);
		undoManager->action(this, ss);
	}
	m_columns = qMax(1, newColumnCount);
}

void PageItem::setColumnGap(double newColumnGap)
{
	if (m_columnGap == newColumnGap)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::Columns, QString(), Um::IBorder);
		ss->set("COLUMNSGAP");
		ss->set("OLD_COLUMNS", m_columnGap);
		ss->set("NEW_COLUMNS", newColumnGap);
		undoManager->action(this, ss);
	}
	m_columnGap = newColumnGap;
}

int PageItem::verticalAlignment() const
{
	return verticalAlign;
}

void PageItem::setVerticalAlignment(int val)
{
	if (val == verticalAlign)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::AlignText, QString(), Um::ITextFrame);
		ss->set("VERTICAL_ALIGN");
		ss->set("OLD_VERTALIGN", verticalAlign);
		ss->set("NEW_VERTALIGN", val);
		undoManager->action(this, ss);
	}
	verticalAlign = val;
}

void PageItem::setCornerRadius(double newRadius)
{
	if (m_roundedCornerRadius == newRadius)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *state = new SimpleState(Um::RoundCorner, QString(), Um::IBorder);
		state->set("CORNER_RADIUS");
		state->set("OLD_RADIUS", m_roundedCornerRadius);
		state->set("NEW_RADIUS", newRadius);
		undoManager->action(this,state);
	}
	m_roundedCornerRadius = newRadius;
	//emit cornerRadius(RadRect);
}



/** Paints the item.
    CHANGE: cullingArea is in doc coordinates!
 */
void PageItem::DrawObj(ScPainter *p, QRectF cullingArea)
{
	// #12698: Prevent drawing of line items
	/*if (PoLine.isEmpty())
		return;*/
	if (cullingArea.isNull())
	{
		cullingArea = QRectF(QPointF(m_Doc->minCanvasCoordinate.x(), m_Doc->minCanvasCoordinate.y()),
							 QPointF(m_Doc->maxCanvasCoordinate.x(), m_Doc->maxCanvasCoordinate.y())).toAlignedRect();
	}

	no_fill = false;
	no_stroke = false;
	DrawObj_Pre(p);
	if (m_Doc->layerOutline(m_layerID))
	{
		if (itemType() == TextFrame || itemType() == ImageFrame || itemType() == PathText || itemType() == Line || itemType() == PolyLine || itemType() == Group || itemType() == Symbol)
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
	double lwCorr = m_lineWidth;
	double sc = p->zoomFactor();
	if ((m_lineWidth * sc) < 1)
		lwCorr = 0;
	if (!isEmbedded)
		p->translate(m_xPos, m_yPos);
	p->rotate(m_rotation);

	if (m_Doc->layerOutline(m_layerID))
	{
		p->setPen(m_Doc->layerMarker(m_layerID), 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::None);
		p->setBrushOpacity(1.0);
		p->setPenOpacity(1.0);
		return;
	}

	if ((hasSoftShadow()) && (m_Doc->appMode != modeEdit))
		DrawSoftShadow(p);
	if (isGroup())
		return;

	p->setBlendModeFill(fillBlendmode());
	p->setLineWidth(lwCorr);

	if (GrType == Gradient_None)
	{
		p->fill_gradient = VGradient(VGradient::linear);
		if (fillColor() != CommonStrings::None)
		{
			p->setBrush(m_fillQColor);
			p->setFillMode(ScPainter::Solid);
		}
		else
		{
			no_fill = true;
			p->setFillMode(ScPainter::None);
		}
	}
	else if (GrType == Gradient_Pattern)
	{
		ScPattern *pattern = m_Doc->checkedPattern(m_patternName);
		if (!pattern)
		{
			p->fill_gradient = VGradient(VGradient::linear);
			p->fill_gradient.setRepeatMethod(GrExtend);
			if (fillColor() != CommonStrings::None)
			{
				p->setBrush(m_fillQColor);
				p->setFillMode(ScPainter::Solid);
			}
			else
			{
				no_fill = true;
				p->setFillMode(ScPainter::None);
			}
			if ((!m_patternName.isEmpty()) && (!m_Doc->docPatterns.contains(m_patternName)))
			{
				GrType = 0;
				m_patternName = "";
			}
		}
		else
		{
			p->setPattern(pattern, m_patternTransform, m_patternMirrorX, m_patternMirrorY);
			p->setFillMode(ScPainter::Pattern);
		}
	}
	else if (GrType == Gradient_4Colors)
	{
		p->setFillMode(ScPainter::Gradient);
		FPoint pG1(0, 0);
		FPoint pG2(width(), 0);
		FPoint pG3(width(), height());
		FPoint pG4(0, height());
		p->set4ColorGeometry(pG1, pG2, pG3, pG4, GrControl1, GrControl2, GrControl3, GrControl4);
		p->set4ColorColors(m_grQColorP1, m_grQColorP2, m_grQColorP3, m_grQColorP4);
	}
	else if (GrType == Gradient_Hatch)
	{
		if (fillColor() != CommonStrings::None)
			p->setBrush(m_fillQColor);
		p->setFillMode(ScPainter::Hatch);
		p->setHatchParameters(hatchType, hatchDistance, hatchAngle, hatchUseBackground, hatchBackgroundQ, hatchForegroundQ, width(), height());
	}
	else
	{
		if ((!m_gradientName.isEmpty()) && (!m_Doc->docGradients.contains(m_gradientName)))
			m_gradientName.clear();
		if (!(m_gradientName.isEmpty()) && (m_Doc->docGradients.contains(m_gradientName)))
			fill_gradient = m_Doc->docGradients[m_gradientName];
		if ((fill_gradient.stops() < 2) && (GrType < Gradient_4Colors)) // fall back to solid filling if there are not enough colorstops in the gradient.
		{
			if (fillColor() != CommonStrings::None)
			{
				p->setBrush(m_fillQColor);
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
			p->fill_gradient.setRepeatMethod(GrExtend);
			switch (GrType)
			{
				case Gradient_LinearLegacy1:
				case Gradient_LinearLegacy2:
				case Gradient_LinearLegacy3:
				case Gradient_LinearLegacy4:
				case Gradient_Linear:
					p->setGradient(VGradient::linear, FPoint(GrStartX, GrStartY), FPoint(GrEndX, GrEndY), FPoint(GrStartX, GrStartY), GrScale, GrSkew);
					break;
				case Gradient_RadialLegacy5:
				case Gradient_Radial:
					p->setGradient(VGradient::radial, FPoint(GrStartX, GrStartY), FPoint(GrEndX, GrEndY), FPoint(GrFocalX, GrFocalY), GrScale, GrSkew);
					break;
				case Gradient_Diamond:
					p->setFillMode(ScPainter::Gradient);
					p->setDiamondGeometry(FPoint(0, 0), FPoint(width(), 0), FPoint(width(), height()), FPoint(0, height()), GrControl1, GrControl2, GrControl3, GrControl4, GrControl5);
					break;
				case Gradient_Mesh:
				case Gradient_Conical:
					p->setFillMode(ScPainter::Gradient);
					p->setMeshGradient(FPoint(0, 0), FPoint(width(), 0), FPoint(width(), height()), FPoint(0, height()), meshGradientArray);
					break;
				case Gradient_PatchMesh:
					p->setFillMode(ScPainter::Gradient);
					p->setMeshGradient(FPoint(0, 0), FPoint(width(), 0), FPoint(width(), height()), FPoint(0, height()), meshGradientPatches);
					break;
			}
		}
	}

	if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
	{
		p->setPen(m_strokeQColor, lwCorr, PLineArt, PLineEnd, PLineJoin);
		if (!DashValues.isEmpty())
			p->setDash(DashValues, DashOffset);
	}
	else
		p->setLineWidth(0);

	p->setBrushOpacity(1.0 - fillTransparency());
	p->setPenOpacity(1.0 - lineTransparency());
	p->setFillRule(fillRule);

	if ((GrMask == GradMask_Linear) || (GrMask == GradMask_Radial) || (GrMask == GradMask_LinearLumAlpha) || (GrMask == GradMask_RadialLumAlpha))
	{
		if ((GrMask == GradMask_Linear) || (GrMask == GradMask_Radial))
			p->setMaskMode(1);
		else
			p->setMaskMode(3);
		if ((!gradientMaskVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientMaskVal)))
			gradientMaskVal.clear();
		if (!(gradientMaskVal.isEmpty()) && (m_Doc->docGradients.contains(gradientMaskVal)))
			mask_gradient = m_Doc->docGradients[gradientMaskVal];
		p->mask_gradient = mask_gradient;
		if ((GrMask == GradMask_Linear) || (GrMask == GradMask_LinearLumAlpha))
			p->setGradientMask(VGradient::linear, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskStartX, GrMaskStartY), GrMaskScale, GrMaskSkew);
		else
			p->setGradientMask(VGradient::radial, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskFocalX, GrMaskFocalY), GrMaskScale, GrMaskSkew);
	}
	else if ((GrMask == GradMask_Pattern) || (GrMask == GradMask_PatternLumAlpha) || (GrMask == GradMask_PatternLumAlphaInverted) || (GrMask == GradMask_PatternInverted))
	{
		ScPattern *patternMask = m_Doc->checkedPattern(patternMaskVal);
		if (patternMask)
		{
			p->setPatternMask(patternMask, patternMaskTransfrm, patternMaskMirrorX, patternMaskMirrorY);
			if (GrMask == GradMask_Pattern)
				p->setMaskMode(2);
			else if (GrMask == GradMask_PatternLumAlpha)
				p->setMaskMode(4);
			else if (GrMask == GradMask_PatternLumAlphaInverted)
				p->setMaskMode(5);
			else
				p->setMaskMode(6);
		}
		else
		{
			p->setMaskMode(0);
		}
	}
	else
		p->setMaskMode(0);
}

void PageItem::DrawObj_Post(ScPainter *p)
{
	bool doStroke = true;
	double lwCorr = m_lineWidth;
	if ((m_lineWidth * p->zoomFactor()) < 1)
		lwCorr = 0;
	if (m_Doc->layerOutline(m_layerID))
	{
		if (itemType() != Line)
		{
			p->setPen(m_Doc->layerMarker(m_layerID), 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setFillMode(ScPainter::None);
			p->setBrushOpacity(1.0);
			p->setPenOpacity(1.0);
			if (itemType() == PolyLine)
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
			if (itemType() == ImageFrame)
			{
				if (!imageClip.empty())
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
			p->setBlendModeFill(0);
			p->setMaskMode(0);
			// TODO: Investigate whether itemType() == Table should really be here. I got artifacts without it so keeping it here for now. /estan
			if (itemType() == PathText || itemType() == PolyLine || itemType() == Spiral || itemType() == Line || itemType() == Symbol || itemType() == Group || itemType() == Table)
				doStroke = false;
			if (doStroke && !m_Doc->RePos)
			{
				p->setBlendModeStroke(lineBlendmode());
				p->setPenOpacity(1.0 - lineTransparency());
				if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
				{
					p->setPen(m_strokeQColor, lwCorr, PLineArt, PLineEnd, PLineJoin);
					if (DashValues.count() != 0)
						p->setDash(DashValues, DashOffset);
				}
				else
					p->setLineWidth(0);
				if ((itemType() == LatexFrame) || (itemType() == ImageFrame) || (itemType() == OSGFrame))
					p->setupPolygon(&PoLine);
				if (NamedLStyle.isEmpty())
				{
					ScPattern *strokePattern = m_Doc->checkedPattern(patternStrokeVal);
					if (strokePattern)
					{
						if (patternStrokePath)
						{
							QPainterPath guidePath = PoLine.toQPainterPath(false);
							DrawStrokePattern(p, guidePath);
						}
						else
						{
							p->setPattern(strokePattern, patternStrokeTransfrm, patternStrokeMirrorX, patternStrokeMirrorY);
							p->setStrokeMode(ScPainter::Pattern);
							p->strokePath();
						}
					}
					else if (GrTypeStroke > 0)
					{
						if ((!gradientStrokeVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientStrokeVal)))
							gradientStrokeVal.clear();
						if (!(gradientStrokeVal.isEmpty()) && (m_Doc->docGradients.contains(gradientStrokeVal)))
							stroke_gradient = m_Doc->docGradients[gradientStrokeVal];
						if (stroke_gradient.stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
						{
							if (lineColor() != CommonStrings::None)
							{
								p->setBrush(m_strokeQColor);
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
							p->stroke_gradient.setRepeatMethod(GrStrokeExtend);
							if (GrTypeStroke == Gradient_Linear)
								p->setGradient(VGradient::linear, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeStartX, GrStrokeStartY), GrStrokeScale, GrStrokeSkew);
							else
								p->setGradient(VGradient::radial, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeFocalX, GrStrokeFocalY), GrStrokeScale, GrStrokeSkew);
						}
						p->strokePath();
					}
					else if (lineColor() != CommonStrings::None)
					{
						p->setStrokeMode(ScPainter::Solid);
						p->setPen(m_strokeQColor, lwCorr, PLineArt, PLineEnd, PLineJoin);
						if (DashValues.count() != 0)
							p->setDash(DashValues, DashOffset);
						p->strokePath();
					}
					else
						no_stroke = true;
					if (lineTransparency() > 0.9)
						no_stroke = true;
				}
				else
				{
					p->setStrokeMode(ScPainter::Solid);
					multiLine ml = m_Doc->docLineStyles[NamedLStyle];
					QColor tmp;
					for (int it = ml.size()-1; it > -1; it--)
					{
						const SingleLine& sl = ml[it];
						// Qt4 if ((!sl.Color != CommonStrings::None) && (sl.Width != 0))
						if (sl.Color != CommonStrings::None) // && (sl.Width != 0))
						{
							SetQColor(&tmp, sl.Color, sl.Shade);
							p->setPen(tmp, sl.Width, static_cast<Qt::PenStyle>(sl.Dash), static_cast<Qt::PenCapStyle>(sl.LineEnd), static_cast<Qt::PenJoinStyle>(sl.LineJoin));
							p->strokePath();
						}
					}
				}
				p->setBlendModeStroke(0);
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
//	p->setAntialiasing(false);
	if (!isEmbedded)
		p->translate(m_xPos, m_yPos);
	p->rotate(m_rotation);
	if ((!isEmbedded) && (!m_Doc->RePos))
	{
		double scpInv = 0;
		if (!isGroup())
		{
			if ((drawFrame()) && (m_Doc->guidesPrefs().framesShown) && ((itemType() == ImageFrame) || (itemType() == LatexFrame) || (itemType() == OSGFrame) || (itemType() == PathText)) && no_stroke)
			{
				p->setPen(PrefsManager::instance().appPrefs.displayPrefs.frameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if (isBookmark || m_isAnnotation)
					p->setPen(PrefsManager::instance().appPrefs.displayPrefs.frameAnnotationColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if ((m_backBox != nullptr) || (m_nextBox != nullptr))
					p->setPen(PrefsManager::instance().appPrefs.displayPrefs.frameLinkColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if (m_Locked)
					p->setPen(PrefsManager::instance().appPrefs.displayPrefs.frameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->setFillMode(ScPainter::None);
				if (itemType() == PathText)
				{
					if (Clip.count() != 0)
					{
						FPointArray tclip;
						FPoint np(Clip.point(0));
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
					p->setupSharpPolygon(&PoLine);
				p->strokePath();
			}
		}
		if ((m_Doc->guidesPrefs().framesShown) && textFlowUsesContourLine() && (!ContourLine.empty()))
		{
			p->setPen(Qt::darkGray, 0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			p->setupSharpPolygon(&ContourLine);
			p->strokePath();
		}
		if (itemType() == ImageFrame)
		{
			double minres = m_Doc->checkerProfiles()[m_Doc->curCheckProfile()].minResolution;
			double maxres = m_Doc->checkerProfiles()[m_Doc->curCheckProfile()].maxResolution;
			bool checkres = m_Doc->checkerProfiles()[m_Doc->curCheckProfile()].checkResolution;
			if  ((((72.0 / imageXScale()) < minres)
				|| ((72.0 / imageYScale()) < minres) 
				|| ((72.0 / imageXScale()) > maxres) 
				|| ((72.0 / imageYScale()) > maxres)) 
				&& isRaster && checkres && (!m_Doc->drawAsPreview) && (PrefsManager::instance().appPrefs.displayPrefs.showVerifierWarningsOnCanvas))
			{
				double ofx = m_width - 22.0;
				double ofy = m_height - 22.0;
				if ((m_width > 40) && (m_height > 40))
				{
					p->save();
					p->translate(ofx, ofy);
					QImage ico = IconManager::instance().loadPixmap("22/dialog-warning.png").toImage();
					p->drawImage(&ico);
					p->restore();
				}
			}
		}
		if ((m_Doc->guidesPrefs().layerMarkersShown) &&
			(m_Doc->layerCount() > 1) &&
			(!m_Doc->layerOutline(m_layerID)) &&
			(!isGroupChild()) &&
			(!m_Doc->drawAsPreview))
		{
			p->setPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setPenOpacity(1.0);
			p->setBrush(m_Doc->layerMarker(m_layerID));
			p->setBrushOpacity(1.0);
			p->setFillMode(ScPainter::Solid);
			double ofwh = 10;
			double ofx = m_width - ofwh/2;
			double ofy = m_height - ofwh*3;
			p->drawSharpRect(ofx, ofy, ofwh, ofwh);
		}
		if (no_fill && no_stroke && m_Doc->guidesPrefs().framesShown)
		{
			p->setPen(PrefsManager::instance().appPrefs.displayPrefs.frameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if (m_Locked)
				p->setPen(PrefsManager::instance().appPrefs.displayPrefs.frameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setFillMode(ScPainter::None);
			p->drawSharpRect(0, 0, m_width, m_height);
			no_fill = false;
			no_stroke = false;
		}
		//CB disabled for now
		//if (m_Doc->m_Selection->findItem(this)!=-1)
		//	drawLockedMarker(p);
	}
	FrameOnly = false;
//	p->setAntialiasing(true);
	p->restore();
}

void PageItem::DrawObj_Embedded(ScPainter *p, const QRectF& cullingArea, const CharStyle& style, PageItem* cembedded)
{
	if (!cembedded)
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
		embedded->m_xPos = embedded->gXpos;
		embedded->m_yPos = (embedded->gHeight * (style.scaleV() / 1000.0)) + embedded->gYpos;
		p->translate((embedded->gXpos * (style.scaleH() / 1000.0)), ( - (embedded->gHeight * (style.scaleV() / 1000.0)) + embedded->gYpos * (style.scaleV() / 1000.0)));
		if (style.baselineOffset() != 0)
		{
			p->translate(0, -embedded->gHeight * (style.baselineOffset() / 1000.0));
			embedded->m_yPos -= embedded->gHeight * (style.baselineOffset() / 1000.0);
		}
		p->scale(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
		embedded->invalid = true;
		double pws = embedded->m_lineWidth;
		embedded->DrawObj_Pre(p);
		switch (embedded->itemType())
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
		embedded->m_xPos = x;
		embedded->m_yPos = y;
		p->restore();
		embedded->m_lineWidth = pws;
	}
	if (m_Doc->guidesPrefs().framesShown)
	{
		p->save();
		p->translate(0, -(cembedded->m_height * (style.scaleV() / 1000.0)));
		if (style.baselineOffset() != 0)
			p->translate(0, -cembedded->m_height * (style.baselineOffset() / 1000.0));
		p->scale(style.scaleH() / 1000.0, style.scaleV() / 1000.0);
		p->setPen(PrefsManager::instance().appPrefs.displayPrefs.frameNormColor, 0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::None);
		p->drawSharpRect(0, 0, cembedded->m_width, cembedded->m_height);
		p->restore();
	}
}

void PageItem::DrawStrokePattern(ScPainter *p, const QPainterPath &path)
{
	ScPattern pat = m_Doc->docPatterns[patternStrokeVal];
	double pLen = path.length() - ((pat.width / 2.0) * patternStrokeTransfrm.scaleX);
	double adv = pat.width * patternStrokeTransfrm.scaleX * patternStrokeTransfrm.space;
	double xpos = patternStrokeTransfrm.offsetX * patternStrokeTransfrm.scaleX;
	while (xpos < pLen)
	{
		double currPerc = path.percentAtLength(xpos);
		double currAngle = path.angleAtPercent(currPerc);
		if (currAngle <= 180.0)
			currAngle *= -1.0;
		else
			currAngle = 360.0 - currAngle;
		QPointF currPoint = path.pointAtPercent(currPerc);
		p->save();
		p->translate(currPoint.x(), currPoint.y());
		p->rotate(currAngle);
		QTransform savWM = p->worldMatrix();
		QTransform trans;
		trans.translate(0.0, patternStrokeTransfrm.offsetY);
		trans.rotate(patternStrokeTransfrm.rotation);
		trans.shear(-patternStrokeTransfrm.skewX, patternStrokeTransfrm.skewY);
		trans.scale(patternStrokeTransfrm.scaleX, patternStrokeTransfrm.scaleY);
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

void PageItem::DrawSoftShadow(ScPainter *p)
{
	if (m_softShadowColor == CommonStrings::None)
		return;
	double lwCorr = m_lineWidth;
	double sc = p->zoomFactor();
	if ((m_lineWidth * sc) < 1)
		lwCorr = 0;
	const ScColor& col = m_Doc->PageColors[m_softShadowColor];
	QColor tmp = ScColorEngine::getShadeColorProof(col, m_Doc, m_softShadowShade);
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		tmp = defect.convertDefect(tmp, m_Doc->previewVisual);
	}
	p->save();
	if (m_softShadowHasObjectTransparency)
		p->beginLayer(1.0 - fillTransparency(), m_softShadowBlendMode);
	else
		p->beginLayer(1.0 - m_softShadowOpacity, m_softShadowBlendMode);
	if (!hasFill())
	{
		double xOffset = isEmbedded ? m_softShadowXOffset : (m_softShadowXOffset - m_xPos);
		double yOffset = isEmbedded ? m_softShadowYOffset : (m_softShadowYOffset - m_yPos);
		double fillTrans_Old = fillTransparency();
		double lineTrans_Old = lineTransparency();
		bool   hasSoftShadow_Old = hasSoftShadow();
		double rotation_Old = m_rotation;
		m_fillTransparency = 0.0;
		m_lineTransparency = 0.0;
		m_rotation = 0;
		m_hasSoftShadow = false;
		p->save();
		p->translate(xOffset, yOffset);
		DrawObj(p, QRectF());
		p->colorizeAlpha(tmp);
		if (m_softShadowBlurRadius > 0)
			p->blur(m_softShadowBlurRadius * sc);
		p->restore();
		if (m_softShadowErasedByObject)
		{
			p->save();
			FPointArray sh = PoLine.copy();
			p->beginLayer(1.0, 18, &sh);
			p->translate(xOffset - m_softShadowXOffset, yOffset - m_softShadowYOffset);
			DrawObj(p, QRectF());
			p->endLayer();
			p->restore();
		}
		m_fillTransparency = fillTrans_Old;
		m_lineTransparency = lineTrans_Old;
		m_hasSoftShadow = hasSoftShadow_Old;
		m_rotation = rotation_Old;
	}
	else
	{
		FPointArray sh = PoLine.copy();
		sh.translate(m_softShadowXOffset, m_softShadowYOffset);
		p->setupPolygon(&sh);
		p->setBrush(tmp);
		p->setFillMode(ScPainter::Solid);
		p->fillPath();
		if (hasStroke())
		{
			p->setStrokeMode(ScPainter::Solid);
			p->setPen(tmp, lwCorr, PLineArt, PLineEnd, PLineJoin);
			p->strokePath();
		}
		if (m_softShadowBlurRadius > 0)
			p->blur(m_softShadowBlurRadius * sc);
		if (m_softShadowErasedByObject)
		{
			sh = PoLine.copy();
			p->setupPolygon(&sh);
			p->setBrush(tmp);
			p->setFillMode(ScPainter::Solid);
			p->setBlendModeFill(19);
			p->fillPath();
			if (hasStroke())
			{
				p->setBlendModeStroke(19);
				p->setStrokeMode(ScPainter::Solid);
				p->setPen(tmp, lwCorr, PLineArt, PLineEnd, PLineJoin);
				p->strokePath();
			}
		}
	}
	p->endLayer();
	p->restore();
}

QImage PageItem::DrawObj_toImage(double maxSize, int options)
{
	bool isEmbedded_Old = isEmbedded;
	double rotation_Old = m_rotation;
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	double x1, x2, y1, y2;
	if (options & NoRotation)
		m_rotation = 0.0;
	getVisualBoundingRect(&x1, &y1, &x2, &y2);
	m_rotation = rotation_Old;
	double maxAdd = 0;
	if (hasSoftShadow() && !(options & NoSoftShadow))
		maxAdd = qMax(fabs(softShadowXOffset()), fabs(softShadowYOffset())) + softShadowBlurRadius();
	minx = qMin(minx, x1) - maxAdd;
	miny = qMin(miny, y1) - maxAdd;
	maxx = qMax(maxx, x2) + maxAdd;
	maxy = qMax(maxy, y2) + maxAdd;
	double igXpos = xPos() - minx;
	double igYpos = yPos() - miny;
	double igWidth = maxx - minx;
	double igHeight = maxy - miny;
	double sc = maxSize / qMax(igWidth, igHeight);
	bool savedFlag = m_Doc->guidesPrefs().framesShown;
	m_Doc->guidesPrefs().framesShown = false;
	QImage retImg(qMax(qRound(igWidth * sc), 1), qMax(qRound(igHeight * sc), 1), QImage::Format_ARGB32_Premultiplied);
	retImg.fill( qRgba(0, 0, 0, 0) );
	ScPainter *painter = new ScPainter(&retImg, retImg.width(), retImg.height(), 1, 0);
	painter->setZoomFactor(sc);
	painter->save();
	painter->translate(igXpos, igYpos);
	if (options & NoRotation)
		painter->rotate(-m_rotation);
	if (invalid)
		layout();
	isEmbedded = true;
	DrawObj(painter, QRectF());
	painter->restore();
	painter->end();
	delete painter;
	m_Doc->guidesPrefs().framesShown = savedFlag;
	isEmbedded = isEmbedded_Old;
	if ((isEmbedded_Old != isEmbedded) && (isTextFrame() || isPathText()))
	{
		invalid = true;
		layout();
	}
	return retImg;
}

QImage PageItem::DrawObj_toImage(QList<PageItem*> &emG, double scaling)
{
	bool isEmbedded_Old = isEmbedded;
	bool savedFlag = m_Doc->guidesPrefs().framesShown;
	m_Doc->guidesPrefs().framesShown = false;
	QImage retImg(qRound(gWidth * scaling), qRound(gHeight * scaling), QImage::Format_ARGB32_Premultiplied);
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

void PageItem::SetQColor(QColor *tmp, const QString& colorName, double shad)
{
	if (colorName == CommonStrings::None)
		return;

	const ScColor& col = m_Doc->PageColors[colorName];
	*tmp = ScColorEngine::getShadeColorProof(col, m_Doc, shad);
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		*tmp = defect.convertDefect(*tmp, m_Doc->previewVisual);
	}
}

void PageItem::DrawPolyL(QPainter *p, const QPolygon& pts)
{
	if (Segments.count() == 0)
	{
		p->drawPolygon(pts);
		return;
	}

	QList<uint>::Iterator it2end = Segments.end();
	uint firstVal = 0;
	for (QList<uint>::Iterator it2 = Segments.begin(); it2 != it2end; ++it2)
	{
		p->drawPolygon(pts.constData() + firstVal, (*it2) - firstVal);
		firstVal = (*it2);
	}
	p->drawPolygon(pts.constData() + firstVal, pts.size() - firstVal);
}

void PageItem::setItemName(const QString& newName)
{
	if (m_itemName == newName || newName.isEmpty())
		return;
	QString oldName = m_itemName;
	m_itemName = generateUniqueCopyName(newName);
	AutoName = false;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::Rename, QString(Um::FromTo).arg(oldName, newName));
		ss->set("OLD_NAME", oldName);
		ss->set("NEW_NAME", newName);
		undoManager->action(this, ss);
	}
	setUName(m_itemName); // set the name for the UndoObject too
}

void PageItem::setGradient(const QString &newGradient)
{
	if (m_gradientName == newGradient)
		return;
	m_gradientName = newGradient;
	if (m_Doc->docGradients.contains(m_gradientName))
		fill_gradient = m_Doc->docGradients[m_gradientName];
}

void PageItem::setMaskGradient(const VGradient& grad)
{
	if (mask_gradient == grad)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScOldNewState<VGradient>(Um::GradVal);
		is->set("MASK_GRAD");
		is->setStates(mask_gradient, grad);
		undoManager->action(this, is);
	}
	mask_gradient = grad;
}

void PageItem::setFillGradient(const VGradient& grad)
{
	if (fill_gradient == grad)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScOldNewState<VGradient>(Um::GradVal);
		is->set("FILL_GRAD");
		is->setStates(fill_gradient, grad);
		undoManager->action(this, is);
	}
	fill_gradient = grad;
}

void PageItem::setStrokeGradient(const VGradient& grad)
{
	if (stroke_gradient == grad)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScOldNewState<VGradient>(Um::GradVal);
		is->set("STROKE_GRAD");
		is->setStates(stroke_gradient, grad);
		undoManager->action(this, is);
	}
	stroke_gradient = grad;
}

void PageItem::setPattern(const QString &newPattern)
{
	if (m_patternName == newPattern)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<QString>(Um::PatternVal);
		is->set("FILL_PATTERN");
		is->setStates(m_patternName, newPattern);
		undoManager->action(this, is);
	}
	m_patternName = newPattern;
}

void PageItem::set4ColorGeometry(const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4)
{
	GrControl1 = c1;
	GrControl2 = c2;
	GrControl3 = c3;
	GrControl4 = c4;
}

void PageItem::setDiamondGeometry(const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4, const FPoint& c5)
{
	GrControl1 = c1;
	GrControl2 = c2;
	GrControl3 = c3;
	GrControl4 = c4;
	GrControl5 = c5;
}

void PageItem::set4ColorTransparency(double t1, double t2, double t3, double t4)
{
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(Um::Selection, Um::IFill, Um::GradVal,QString(), Um::IFill);
	setGradientTransp1(t1);
	setGradientTransp2(t2);
	setGradientTransp3(t3);
	setGradientTransp4(t4);
	if (trans)
		trans.commit();
}

void PageItem::set4ColorShade(int t1, int t2, int t3, int t4)
{
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(Um::Selection, Um::IFill, Um::GradVal, QString(), Um::IFill);
	setGradientShade1(t1);
	setGradientShade2(t2);
	setGradientShade3(t3);
	setGradientShade4(t4);
	if (trans)
		trans.commit();
}

void PageItem::set4ColorColors(const QString& col1, const QString& col2, const QString& col3, const QString& col4)
{
	QColor tmp;
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(Um::Selection,Um::IFill, Um::GradVal, QString(), Um::IFill);
	setGradientCol1(col1);
	if (GrColorP1 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP1))
		{
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol1(m_Doc->itemToolPrefs().imageFillColor);
					break;
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
		setGradientColor1(QColor(0, 0, 0, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor1(defect.convertDefect(m_grQColorP1, m_Doc->previewVisual));
	}
	setGradientCol2(col2);
	if (GrColorP2 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP2))
		{
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol2(m_Doc->itemToolPrefs().imageFillColor);
					break;
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
		setGradientColor2(QColor(0, 0, 0, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor2(defect.convertDefect(m_grQColorP2, m_Doc->previewVisual));
	}
	setGradientCol3(col3);
	if (GrColorP3 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP3))
		{
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol3(m_Doc->itemToolPrefs().imageFillColor);
					break;
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
		setGradientColor3(QColor(0, 0, 0, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor3(defect.convertDefect(m_grQColorP3, m_Doc->previewVisual));
	}
	setGradientCol4(col4);
	if (GrColorP4 != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(GrColorP4))
		{
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					setGradientCol4(m_Doc->itemToolPrefs().imageFillColor);
					break;
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
		setGradientColor4(QColor(0, 0, 0, 0));
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		setGradientColor4(defect.convertDefect(m_grQColorP4, m_Doc->previewVisual));
	}
	if (trans)
		trans.commit();
}

void PageItem::get4ColorGeometry(FPoint& c1, FPoint& c2, FPoint& c3, FPoint& c4) const
{
	c1 = GrControl1;
	c2 = GrControl2;
	c3 = GrControl3;
	c4 = GrControl4;
}

void PageItem::get4ColorTransparency(double &t1, double &t2, double &t3, double &t4) const
{
	t1 = GrCol1transp;
	t2 = GrCol2transp;
	t3 = GrCol3transp;
	t4 = GrCol4transp;
}

void PageItem::get4ColorColors(QString &col1, QString &col2, QString &col3, QString &col4) const
{
	col1 = GrColorP1;
	col2 = GrColorP2;
	col3 = GrColorP3;
	col4 = GrColorP4;
}

void PageItem::setMeshPointColor(int x, int y, const QString& color, int shade, double transparency, bool forPatch)
{
	QString MColor(color);
	QColor MQColor;
	if (MColor != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(MColor))
		{
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					MColor = m_Doc->itemToolPrefs().imageFillColor;
					break;
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
		MQColor = QColor(0, 0, 0, 0);
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		MQColor = defect.convertDefect(MQColor, m_Doc->previewVisual);
	}
	MeshPoint *mp = nullptr;
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

		UndoTransaction trans;
		if (UndoManager::undoEnabled())
		{
			trans = undoManager->beginTransaction(Um::Selection, Um::IFill, Um::GradVal, QString(), Um::IFill);
			auto *ss = new ScOldNewState<QColor>(Um::GradVal);
			ss->set("GRAD_MESH_COLOR");
			ss->set("X", x);
			ss->set("Y", y);
			ss->set("PATCH", true);
			ss->set("OLD_COLOR_NAME", mp->colorName);
			ss->set("NEW_COLOR_NAME", MColor);
			ss->setStates(mp->color, MQColor);
			ss->set("OLD_SHADE", mp->shade);
			ss->set("NEW_SHADE", shade);
			ss->set("OLD_TRANSP", mp->transparency);
			ss->set("NEW_TRANSP", transparency);
			undoManager->action(this, ss);
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
					if (UndoManager::undoEnabled())
					{
						auto *ss = new ScOldNewState<QColor>(Um::GradVal);
						ss->set("GRAD_MESH_COLOR");
						ss->set("X", col);
						ss->set("Y", 1);
						ss->set("PATCH", true);
						ss->set("OLD_COLOR_NAME", mp->colorName);
						ss->set("NEW_COLOR_NAME", MColor);
						ss->setStates(mp->color, MQColor);
						ss->set("OLD_SHADE", mp->shade);
						ss->set("NEW_SHADE", shade);
						ss->set("OLD_TRANSP", mp->transparency);
						ss->set("NEW_TRANSP", transparency);
						undoManager->action(this, ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
				if (meshGradientPatches[col].TR.gridPoint == xx)
				{
					mp = &(meshGradientPatches[col].TR);
					if (UndoManager::undoEnabled())
					{
						auto *ss = new ScOldNewState<QColor>(Um::GradVal);
						ss->set("GRAD_MESH_COLOR");
						ss->set("X", col);
						ss->set("Y", 2);
						ss->set("PATCH", true);
						ss->set("OLD_COLOR_NAME", mp->colorName);
						ss->set("NEW_COLOR_NAME", MColor);
						ss->setStates(mp->color, MQColor);
						ss->set("OLD_SHADE", mp->shade);
						ss->set("NEW_SHADE", shade);
						ss->set("OLD_TRANSP", mp->transparency);
						ss->set("NEW_TRANSP", transparency);
						undoManager->action(this, ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
				if (meshGradientPatches[col].BR.gridPoint == xx)
				{
					mp = &(meshGradientPatches[col].BR);
					if (UndoManager::undoEnabled())
					{
						auto *ss = new ScOldNewState<QColor>(Um::GradVal);
						ss->set("GRAD_MESH_COLOR");
						ss->set("X", col);
						ss->set("Y", 3);
						ss->set("PATCH", true);
						ss->set("OLD_COLOR_NAME", mp->colorName);
						ss->set("NEW_COLOR_NAME", MColor);
						ss->setStates(mp->color, MQColor);
						ss->set("OLD_SHADE", mp->shade);
						ss->set("NEW_SHADE", shade);
						ss->set("OLD_TRANSP", mp->transparency);
						ss->set("NEW_TRANSP", transparency);
						undoManager->action(this, ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
				if (meshGradientPatches[col].BL.gridPoint == xx)
				{
					mp = &(meshGradientPatches[col].BL);
					if (UndoManager::undoEnabled())
					{
						auto *ss = new ScOldNewState<QColor>(Um::GradVal);
						ss->set("GRAD_MESH_COLOR");
						ss->set("X", col);
						ss->set("Y", 4);
						ss->set("PATCH", true);
						ss->set("OLD_COLOR_NAME", mp->colorName);
						ss->set("NEW_COLOR_NAME", MColor);
						ss->setStates(mp->color, MQColor);
						ss->set("OLD_SHADE", mp->shade);
						ss->set("NEW_SHADE", shade);
						ss->set("OLD_TRANSP", mp->transparency);
						ss->set("NEW_TRANSP", transparency);
						undoManager->action(this, ss);
					}
					mp->colorName = MColor;
					mp->color = MQColor;
					mp->shade = shade;
					mp->transparency = transparency;
				}
			}
		}
		if (trans)
			trans.commit();
	}
	else
	{
		if ((x > -1) && (y > -1))
		{
			mp = &(meshGradientArray[x][y]);
			if (UndoManager::undoEnabled())
			{
				auto *ss = new ScOldNewState<QColor>(Um::GradVal);
				ss->set("GRAD_MESH_COLOR");
				ss->set("X", x);
				ss->set("Y", y);
				ss->set("PATCH", false);
				ss->set("OLD_COLOR_NAME", mp->colorName);
				ss->set("NEW_COLOR_NAME", MColor);
				ss->setStates(mp->color, MQColor);
				ss->set("OLD_SHADE", mp->shade);
				ss->set("NEW_SHADE", shade);
				ss->set("OLD_TRANSP", mp->transparency);
				ss->set("NEW_TRANSP", transparency);
				undoManager->action(this, ss);
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
	QString MColor(m_fillColor);
	QColor MQColor;
	if (m_fillColor != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(m_fillColor))
		{
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					MColor = m_Doc->itemToolPrefs().imageFillColor;
					break;
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
		MQColor = QColor(0, 0, 0, 0);
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		MQColor = defect.convertDefect(MQColor, m_Doc->previewVisual);
	}
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<QList<QList<MeshPoint> > >(Um::MeshGradient);
		is->set("CREATE_MESH_GRAD");
		is->set("ROW", rows);
		is->set("COL", cols);
		is->setItem(meshGradientArray);
		undoManager->action(this, is);
	}
	meshGradientArray.clear();
	if (rows == 0 || cols == 0)
		qFatal("PageItem::createGradientMesh: rows or cols is 0");
	double xoffs = m_width / static_cast<double>(cols);
	double yoffs = m_height / static_cast<double>(rows);
	for (int x = 0; x < rows + 1; x++)
	{
		QList<MeshPoint> mgList;
		mgList.reserve(cols + 1);
		for (int y = 0; y < cols + 1; y++)
		{
			MeshPoint mgP;
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
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<QList<QList<MeshPoint> > >(Um::ResetControlPoints);
		is->set("RESET_MESH_GRAD");
		is->setItem(meshGradientArray);
		undoManager->action(this, is);
	}
	double xoffs = m_width / static_cast<double>(cols - 1);
	double yoffs = m_height / static_cast<double>(rows - 1);
	for (int x = 0; x < rows; x++)
		for (int y = 0; y < cols; y++)
			meshGradientArray[x][y].resetTo(FPoint(y * xoffs, x * yoffs));
}

void PageItem::meshToShape()
{
	FPointArray Coords;
	Coords.svgInit();
	int rows = meshGradientArray.count() - 1;
	int cols = meshGradientArray[0].count() - 1;
	QList<MeshPoint> mpList;
	mpList = meshGradientArray[0];
	Coords.svgMoveTo(mpList[0].gridPoint.x(), mpList[0].gridPoint.y());
	for (int m = 0; m < mpList.count() - 1; ++m)
	{
		Coords.svgCurveToCubic(mpList[m].controlRight.x(), mpList[m].controlRight.y(),
		                       mpList[m + 1].controlLeft.x(), mpList[m + 1].controlLeft.y(),
		                       mpList[m + 1].gridPoint.x(), mpList[m + 1].gridPoint.y());
	}
	for (int m = 0; m < rows; ++m)
	{
		Coords.svgCurveToCubic(meshGradientArray[m][cols].controlBottom.x(), meshGradientArray[m][cols].controlBottom.y(),
		                       meshGradientArray[m + 1][cols].controlTop.x(), meshGradientArray[m + 1][cols].controlTop.y(),
		                       meshGradientArray[m + 1][cols].gridPoint.x(), meshGradientArray[m + 1][cols].gridPoint.y());
	}
	mpList = meshGradientArray[rows];
	for (int m = cols; m > 0; --m)
	{
		Coords.svgCurveToCubic(mpList[m].controlLeft.x(), mpList[m].controlLeft.y(),
		                       mpList[m - 1].controlRight.x(), mpList[m - 1].controlRight.y(),
		                       mpList[m - 1].gridPoint.x(), mpList[m - 1].gridPoint.y());
	}
	for (int m = rows; m > 0; --m)
	{
		Coords.svgCurveToCubic(meshGradientArray[m][0].controlTop.x(), meshGradientArray[m][0].controlTop.y(),
		                       meshGradientArray[m - 1][0].controlBottom.x(), meshGradientArray[m - 1][0].controlBottom.y(),
		                       meshGradientArray[m - 1][0].gridPoint.x(), meshGradientArray[m - 1][0].gridPoint.y());
	}
	
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
	{
		trans = undoManager->beginTransaction(Um::Selection, Um::IFill, Um::ChangeMeshGradient, QString(), Um::IFill);
		auto *ism = new ScItemState<QPair<QList<QList<MeshPoint> >, FPointArray> >(Um::ChangeMeshGradient, QString(), Um::IFill);
		ism->set("MOVE_MESH_GRAD");
		ism->setItem(qMakePair(meshGradientArray, PoLine));
		ism->set("OLDB", OldB2);
		ism->set("OLDH", OldH2);
		ism->set("FRAME_TYPE", FrameType);
		undoManager->action(this, ism);
	}
	QList<QList<MeshPoint> > meshGradientArrayOld = meshGradientArray;
	PoLine = Coords.copy();
	double oldX = m_xPos;
	double oldY = m_yPos;
	ClipEdited = true;
	FrameType = 3;
	FPoint wh = getMaxClipF(&PoLine);
	setWidthHeight(wh.x(),wh.y());
	m_Doc->adjustItemSize(this);
	OldB2 = width();
	OldH2 = height();
	updateClip();
	meshGradientArray = meshGradientArrayOld;
	double dx = oldX - m_xPos;
	double dy = oldY - m_yPos;
	for (int x = 0; x < rows + 1; x++)
		for (int y = 0; y < cols + 1; y++)
			meshGradientArray[x][y].moveRel(dx, dy);
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<FPointArray>(Um::ResetContourLine, QString(), Um::IBorder);
		is->set("RESET_CONTOUR");
		is->setItem(ContourLine);
		undoManager->action(this, is);
	}
	ContourLine = PoLine.copy();
	if (trans)
		trans.commit();
}

void PageItem::createConicalMesh()
{
	VGradient gradient;
	gradient.clearStops();
	if (fill_gradient.stops() < 2)
	{
		fill_gradient.clearStops();
		const ScColor& col = m_Doc->PageColors["Black"];
		QColor qcol = ScColorEngine::getRGBColor(col, m_Doc);
		fill_gradient.addStop(qcol, 0.0, 0.5, 1.0, "Black", 100);
		const ScColor& col2 = m_Doc->PageColors["White"];
		qcol = ScColorEngine::getRGBColor(col2, m_Doc);
		fill_gradient.addStop(qcol, 1.0, 0.5, 1.0, "White", 100);
	}
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	double lastStop = -1.0;
	double actualStop = 0.0;
	bool isFirst = true;
	for (int cst = 0; cst < fill_gradient.stops(); ++cst)
	{
		actualStop = cstops.at(cst)->rampPoint;
		if ((actualStop == lastStop) && (!isFirst))
			continue;
		if (isFirst)
		{
			if (actualStop != 0.0)
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
				gradient.addStop(computeInBetweenStop(cstops.at(cst - 1), cstops.at(cst), actDist));
				actDist += 0.25;
			}
			while (actualStop > actDist);
			gradient.addStop(cstops.at(cst)->color, cstops.at(cst)->rampPoint, cstops.at(cst)->midPoint, cstops.at(cst)->opacity, cstops.at(cst)->name, cstops.at(cst)->shade);
		}
		if ((cst == fill_gradient.stops() - 1) && (actualStop < 1.0))
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
	QList<MeshPoint> mgList1;
	QList<MeshPoint> mgList2;
	MeshPoint mgP1, mgP2, mgP3, mgP4;
	QPainterPath path;
	FPointArray arcPath;
	QLineF angLin(QPointF(GrStartX, GrStartY), QPointF(GrEndX, GrEndY));
	double startAngle = angLin.angle();
	double radius = angLin.length();
	double radius2 = radius * 2.0;
	QList<VColorStop*> rstops = gradient.colorStops();
	if (rstops.count() < 2)
	{
		const VColorStop* firstStop = rstops.at(0);
		if (firstStop->rampPoint > 0)
		{
			VColorStop* newStop = new VColorStop(*firstStop);
			newStop->rampPoint = 0;
			rstops.prepend(newStop);
		}
		if (firstStop->rampPoint < 1.0)
		{
			VColorStop* newStop = new VColorStop(*firstStop);
			newStop->rampPoint = 1.0;
			rstops.append(newStop);
		}
	}
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
	QPointF foc(GrFocalX, GrFocalY);
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
	for (int rst = 2; rst < gradient.stops(); ++rst)
	{
		stepAngle = 360 * (rstops.at(rst)->rampPoint - rstops.at(rst - 1)->rampPoint);
		if (stepAngle <= 0)
			continue;
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

VColorStop PageItem::computeInBetweenStop(const VColorStop* last, const VColorStop* actual, double t) const
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
	if (gradientStrokeVal == newGradient)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<QString>(Um::GradVal);
		is->set("STROKE_GRADIENT_NAME");
		is->setStates(gradientStrokeVal, newGradient);
		undoManager->action(this, is);
	}
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

void PageItem::setPatternTransform(const ScPatternTransform& trans)
{
	if (m_patternTransform == trans)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<ScPatternTransform>(Um::PatternTransform);
		is->set("FILL_PATTERN_TRANSFORM");
		is->setStates(m_patternTransform, trans);
		undoManager->action(this, is);
	}
	m_patternTransform = trans;
}

void PageItem::setPatternTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY)
{
	ScPatternTransform patternTrans;
	patternTrans.scaleX = scaleX;
	patternTrans.scaleY = scaleY;
	patternTrans.offsetX = offsetX;
	patternTrans.offsetY = offsetY;
	patternTrans.rotation = rotation;
	patternTrans.skewX = skewX;
	patternTrans.skewY = skewY;
	setPatternTransform(patternTrans);
}

void PageItem::setPatternFlip(bool flipX, bool flipY)
{
	if (m_patternMirrorX == flipX && m_patternMirrorY == flipY)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState< QPair<bool, bool> >(Um::PatternFlip);
		is->set("FILL_PATTERN_FLIP");
		is->setStates(qMakePair(m_patternMirrorX, m_patternMirrorY), qMakePair(flipX, flipY));
		undoManager->action(this, is);
	}
	m_patternMirrorX = flipX;
	m_patternMirrorY = flipY;
}

void PageItem::patternFlip(bool &flipX, bool &flipY) const
{
	flipX = m_patternMirrorX;
	flipY = m_patternMirrorY;
}

void PageItem::setMaskType(int val)
{
	if (GrMask == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradTypeMask, QString(), Um::IFill);
		ss->set("MASKTYPE");
		ss->set("NEW", val);
		ss->set("OLD", GrMask);
		undoManager->action(this, ss);
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
	if (patternMaskVal == newMask)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<QString>(Um::PatternValMask);
		is->set("MASK_PATTERN");
		is->setStates(patternMaskVal, newMask);
		undoManager->action(this, is);
	}
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

void PageItem::setMaskTransform(const ScMaskTransform& maskTrans)
{
	if (patternMaskTransfrm == maskTrans)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<ScPatternTransform>(Um::PatternTransform);
		is->set("MASK_TRANSFORM");
		is->setStates(patternMaskTransfrm, maskTrans);
		undoManager->action(this, is);
	}
	patternMaskTransfrm = maskTrans;
}

void PageItem::setMaskTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY)
{
	ScMaskTransform maskTransform;
	maskTransform.scaleX = scaleX;
	maskTransform.scaleY = scaleY;
	maskTransform.offsetX = offsetX;
	maskTransform.offsetY = offsetY;
	maskTransform.rotation = rotation;
	maskTransform.skewX = skewX;
	maskTransform.skewY = skewY;
	setMaskTransform(maskTransform);
}

void PageItem::setMaskFlip(bool flipX, bool flipY)
{
	if (patternMaskMirrorX == flipX && patternMaskMirrorY == flipY)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState< QPair<bool, bool> >(Um::PatternFlip);
		is->set("MASK_FLIP");
		is->setStates(qMakePair(patternMaskMirrorX, patternMaskMirrorY), qMakePair(flipX, flipY));
		undoManager->action(this, is);
	}
	patternMaskMirrorX = flipX;
	patternMaskMirrorY = flipY;
}

void PageItem::maskFlip(bool &flipX, bool &flipY) const
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
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					tmp = m_Doc->itemToolPrefs().imageFillColor;
					break;
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
	if (m_fillColor == tmp)
	{
		setFillQColor();
		return;
	}
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SetFill, QString(Um::ColorFromTo).arg(m_fillColor, tmp),
                                   Um::IFill);
		ss->set("FILL");
		ss->set("OLD_FILL", m_fillColor);
		ss->set("NEW_FILL", tmp);
		undoManager->action(this, ss);
	}
	m_fillColor = tmp;
	setFillQColor();
}

void PageItem::setFillShade(double newShade)
{
	if (m_fillShade == newShade)
	{
		setFillQColor();
		return;
	}
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SetShade, QString(Um::FromTo).arg(m_fillShade).arg(newShade),
		                           Um::IShade);
		ss->set("SHADE");
		ss->set("OLD_SHADE", m_fillShade);
		ss->set("NEW_SHADE", newShade);
		undoManager->action(this, ss);
	}
	m_fillShade = newShade;
	setFillQColor();
}

void PageItem::setFillTransparency(double newTransparency)
{
	if (m_fillTransparency == newTransparency)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::Transparency, QString(Um::FromTo).arg(m_fillTransparency).arg(newTransparency),
		                           Um::ITransparency);
		ss->set("TRANSPARENCY");
		ss->set("OLD_TP", m_fillTransparency);
		ss->set("NEW_TP", newTransparency);
		undoManager->action(this, ss);
	}
	m_fillTransparency = newTransparency;
}

void PageItem::setFillBlendmode(int newBlendmode)
{
	if (m_fillBlendMode == newBlendmode)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::BlendMode, nullptr, Um::IGroup);
		ss->set("FILLBLENDMODE", newBlendmode);
		ss->set("FILLBLENDMODE_OLD", m_fillBlendMode);
		undoManager->action(this, ss);
	}
	m_fillBlendMode = newBlendmode;
}

void PageItem::setLineColor(const QString &newColor)
{
	QString tmp = newColor;
	if (tmp != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(newColor))
		{
			switch (itemType())
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
	if (m_lineColor == tmp)
	{
		setLineQColor();
		return;
	}
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SetLineColor, QString(Um::ColorFromTo).arg(m_lineColor, tmp),
		                           Um::IFill);
		ss->set("LINE_COLOR");
		ss->set("OLD_COLOR", m_lineColor);
		ss->set("NEW_COLOR", tmp);
		undoManager->action(this, ss);
	}
	m_lineColor = tmp;
	setLineQColor();
}

void PageItem::setLineShade(double newShade)
{
	if (m_lineShade == newShade)
	{
		setLineQColor();
		return;
	}
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SetLineShade, QString(Um::FromTo).arg(m_lineShade).arg(newShade),
		                           Um::IShade);
		ss->set("LINE_SHADE");
		ss->set("OLD_SHADE", m_lineShade);
		ss->set("NEW_SHADE", newShade);
		undoManager->action(this, ss);
	}
	m_lineShade = newShade;
	setLineQColor();
}

void PageItem::setStrokePattern(const QString &newPattern)
{
	if (patternStrokeVal == newPattern)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<QString>(Um::PatternValStroke);
		is->set("STROKE_PATTERN");
		is->setStates(patternStrokeVal, newPattern);
		undoManager->action(this, is);
	}
	patternStrokeVal = newPattern;
}

void PageItem::setStrokePatternToPath(bool enable)
{
	if (patternStrokePath == enable)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<bool>(Um::PatternStrokeToPath);
		is->set("STROKE_PATTERN_TO_PATH");
		is->setStates(patternStrokePath, enable);
		undoManager->action(this, is);
	}
	patternStrokePath = enable;
}

bool PageItem::isStrokePatternToPath() const
{
	return patternStrokePath;
}

void PageItem::setStrokePatternTransform(const ScStrokePatternTransform& trans)
{
	if (patternStrokeTransfrm == trans)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState<ScStrokePatternTransform>(Um::PatternTransform);
		is->set("STROKE_PATTERN_TRANSFORM");
		is->setStates(patternStrokeTransfrm, trans);
		undoManager->action(this, is);
	}
	patternStrokeTransfrm = trans;
}

void PageItem::setStrokePatternTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, double space)
{
	ScStrokePatternTransform strokePatternTrans;
	strokePatternTrans.scaleX = scaleX;
	strokePatternTrans.scaleY = scaleY;
	strokePatternTrans.offsetX = offsetX;
	strokePatternTrans.offsetY = offsetY;
	strokePatternTrans.rotation = rotation;
	strokePatternTrans.skewX = skewX;
	strokePatternTrans.skewY = skewY;
	strokePatternTrans.space = space;
	setStrokePatternTransform(strokePatternTrans);
}

void PageItem::setStrokePatternFlip(bool flipX, bool flipY)
{
	if (patternStrokeMirrorX == flipX && patternStrokeMirrorY == flipY)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* is = new ScOldNewState< QPair<bool, bool> >(Um::PatternFlip);
		is->set("STROKE_PATTERN_FLIP");
		is->setStates(qMakePair(patternStrokeMirrorX, patternStrokeMirrorY), qMakePair(flipX, flipY));
		undoManager->action(this, is);
	}
	patternStrokeMirrorX = flipX;
	patternStrokeMirrorY = flipY;
}

void PageItem::strokePatternFlip(bool &flipX, bool &flipY) const
{
	flipX = patternStrokeMirrorX;
	flipY = patternStrokeMirrorY;
}

void PageItem::setLineQColor()
{
	if (m_lineColor != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(m_lineColor))
		{
			switch (itemType())
			{
				case TextFrame:
				case PathText:
					m_lineColor = m_Doc->itemToolPrefs().textLineColor;
					break;
				case Line:
					m_lineColor = m_Doc->itemToolPrefs().lineColor;
					break;
				case PolyLine:
				case Polygon:
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
				case RegularPolygon:
				case Arc:
				case Spiral:
					m_lineColor = m_Doc->itemToolPrefs().shapeLineColor;
					break;
				default:
					break;
			}
		}
		if (!m_Doc->PageColors.contains(m_lineColor))
			m_lineColor = m_Doc->itemToolPrefs().shapeLineColor;
		const ScColor& col = m_Doc->PageColors[m_lineColor];
		m_strokeQColor = ScColorEngine::getShadeColorProof(col, m_Doc, m_lineShade);
	}
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		m_strokeQColor = defect.convertDefect(m_strokeQColor, m_Doc->previewVisual);
	}
}

void PageItem::setFillQColor()
{
	if (m_fillColor != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(m_fillColor))
		{
			switch (itemType())
			{
				case ImageFrame:
				case LatexFrame:
				case OSGFrame:
					m_fillColor = m_Doc->itemToolPrefs().imageFillColor;
					break;
				case TextFrame:
				case PathText:
					m_fillColor = m_Doc->itemToolPrefs().textFillColor;
					break;
				case Line:
				case PolyLine:
				case Polygon:
				case RegularPolygon:
				case Arc:
				case Spiral:
					m_fillColor = m_Doc->itemToolPrefs().shapeFillColor;
					break;
				default:
					break;
			}
		}
		const ScColor& col = m_Doc->PageColors[m_fillColor];
		m_fillQColor = ScColorEngine::getShadeColorProof(col, m_Doc, m_fillShade);
	}
	if (m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		m_fillQColor = defect.convertDefect(m_fillQColor, m_Doc->previewVisual);
	}
}

void PageItem::setHatchParameters(int mode, double distance, double angle, bool useBackground, const QString& background, const QString& foreground)
{
	hatchType = mode;
	hatchDistance = distance;
	hatchAngle = angle;
	hatchUseBackground = useBackground;
	hatchBackground = background;
	hatchForeground = foreground;
	if (background != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(background))
			hatchBackgroundQ = QColor();
		else
		{
			const ScColor& col = m_Doc->PageColors[background];
			hatchBackgroundQ = ScColorEngine::getShadeColorProof(col, m_Doc, 100);
			if (m_Doc->viewAsPreview)
			{
				VisionDefectColor defect;
				hatchBackgroundQ = defect.convertDefect(hatchBackgroundQ, m_Doc->previewVisual);
			}
		}
	}
	if (foreground != CommonStrings::None)
	{
		if (!m_Doc->PageColors.contains(foreground))
			hatchForegroundQ = QColor();
		else
		{
			const ScColor& col = m_Doc->PageColors[foreground];
			hatchForegroundQ = ScColorEngine::getShadeColorProof(col, m_Doc, 100);
			if (m_Doc->viewAsPreview)
			{
				VisionDefectColor defect;
				hatchForegroundQ = defect.convertDefect(hatchForegroundQ, m_Doc->previewVisual);
			}
		}
	}
}

void PageItem::setLineTransparency(double newTransparency)
{
	if (m_lineTransparency == newTransparency)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::LineTransparency, QString(Um::FromTo).arg(m_lineTransparency).arg(newTransparency),
		                           Um::ITransparency);
		ss->set("LINE_TRANSPARENCY");
		ss->set("OLD_TP", m_lineTransparency);
		ss->set("NEW_TP", newTransparency);
		undoManager->action(this, ss);
	}
	m_lineTransparency = newTransparency;
}

void PageItem::setLineBlendmode(int newBlendmode)
{
	if (m_lineBlendMode == newBlendmode)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::BlendMode, nullptr, Um::IGroup);
		ss->set("LINEBLENDMODE", newBlendmode);
		ss->set("LINEBLENDMODE_OLD", m_lineBlendMode);
		undoManager->action(this, ss);
	}
	m_lineBlendMode = newBlendmode;
}

void PageItem::setLineStyle(Qt::PenStyle newStyle)
{
	if (PLineArt == newStyle)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::LineStyle, QString(), Um::ILineStyle);
		ss->set("LINE_STYLE");
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
		auto *ss = new SimpleState(Um::LineWidth, QString(Um::FromTo).arg(m_lineWidth).arg(newWidth),
		                           Um::ILineStyle);
		ss->set("LINE_WIDTH");
		ss->set("OLD_LINEWIDTH", m_lineWidth);
		ss->set("NEW_LINEWIDTH", newWidth);
		undoManager->action(this, ss);
	}
	m_oldLineWidth = m_lineWidth;
	m_lineWidth = newWidth;
}

void PageItem::setLineEnd(Qt::PenCapStyle newStyle)
{
	if (PLineEnd == newStyle)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::LineEnd, QString(), Um::ILineStyle);
		ss->set("LINE_END");
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
		auto *ss = new SimpleState(Um::LineJoin, QString(), Um::ILineStyle);
		ss->set("LINE_JOIN");
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
		auto *ss = new SimpleState(action, QString(Um::FromTo).arg(oldStyle, nStyle), Um::ILineStyle);
		ss->set("CUSTOM_LINE_STYLE");
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
		auto *ss = new SimpleState(Um::StartArrow, QString(), Um::IArrow);
		ss->set("START_ARROW");
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
		auto *ss = new SimpleState(Um::EndArrow, QString(), Um::IArrow);
		ss->set("END_ARROW");
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
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::StartArrowScale, QString(), Um::IArrow);
		ss->set("START_ARROWSCALE");
		ss->set("OLD_SCALE", m_startArrowScale);
		ss->set("NEW_SCALE", newScale);
		undoManager->action(this, ss);
	}
	m_startArrowScale = newScale;
}

void PageItem::setEndArrowScale(int newScale)
{
	if (m_endArrowScale == newScale)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::EndArrowScale, QString(), Um::IArrow);
		ss->set("END_ARROWSCALE");
		ss->set("OLD_SCALE", m_endArrowScale);
		ss->set("NEW_SCALE", newScale);
		undoManager->action(this, ss);
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
		auto *ss = new SimpleState(Um::FlipH, nullptr, Um::IFlipH);
		ss->set("IMAGEFLIPH");
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
		auto *ss = new SimpleState(Um::FlipV, nullptr, Um::IFlipV);
		ss->set("IMAGEFLIPV");
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
		auto *ss = new SimpleState(Um::ImageScaling, QString(Um::FromTo).arg(from, to), Um::IImageScaling);
		ss->set("SCALE_MODE");
		if (freeScale != ScaleType)
		{
			ss->set("SCALE_TYPE", freeScale);
			if (!freeScale)
			{
				//if switching from free scaling to frame size
				//in undo must be offset and scale saved
				ss->set("OLD_IMAGEXOFFSET", m_imageXOffset);
				ss->set("OLD_IMAGEYOFFSET", m_imageYOffset);
				ss->set("OLD_IMAGEXSCALE", m_imageXScale);
				ss->set("OLD_IMAGEYSCALE", m_imageYScale);
			}
		}
		if (keepRatio != AspectRatio)
			ss->set("ASPECT_RATIO", keepRatio);
		undoManager->action(this, ss);
	}
	ScaleType = freeScale;
	AspectRatio = keepRatio;
	adjustPictScale();
	update();
}

void PageItem::setOverprint(bool val)
{
	if (doOverprint == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::Overprint, nullptr, Um::IGroup);
		ss->set("OVERPRINT", val);
		undoManager->action(this, ss);
	}
	doOverprint = val;
}

void PageItem::setHasSoftShadow(bool val)
{
	if (m_hasSoftShadow == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadow, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_hasSoftShadow);
		undoManager->action(this, ss);
	}
	m_hasSoftShadow = val;
}

void PageItem::setSoftShadowColor(const QString &val)
{
	if (m_softShadowColor == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowColor, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_COLOR");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowColor);
		undoManager->action(this, ss);
	}
	m_softShadowColor = val;
}

void PageItem::setSoftShadowShade(int val)
{
	if (m_softShadowShade == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowShade, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_SHADE");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowShade);
		undoManager->action(this, ss);
	}
	m_softShadowShade = val;
}

void PageItem::setSoftShadowBlurRadius(double val)
{
	if (m_softShadowBlurRadius == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowBlurRadius, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_BLUR_RADIUS");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowBlurRadius);
		undoManager->action(this, ss);
	}
	m_softShadowBlurRadius = val;
}

void PageItem::setSoftShadowXOffset(double val)
{
	if (m_softShadowXOffset == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowXOffset, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_XOFFSET");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowXOffset);
		undoManager->action(this, ss);
	}
	m_softShadowXOffset = val;
}

void PageItem::setSoftShadowYOffset(double val)
{
	if (m_softShadowYOffset == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowYOffset, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_YOFFSET");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowYOffset);
		undoManager->action(this, ss);
	}
	m_softShadowYOffset = val;
}

void PageItem::setSoftShadowOpacity(double val)
{
	if (m_softShadowOpacity == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowOpacity, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_OPACITY");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowOpacity);
		undoManager->action(this, ss);
	}
	m_softShadowOpacity = val;
}

void PageItem::setSoftShadowBlendMode(int val)
{
	if (m_softShadowBlendMode == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowBlendMode, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_BLEND_MODE");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowBlendMode);
		undoManager->action(this, ss);
	}
	m_softShadowBlendMode = val;
}

void PageItem::setSoftShadowErasedByObject(bool val)
{
	if (m_softShadowErasedByObject == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowErase, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_ERASE");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowErasedByObject);
		undoManager->action(this, ss);
	}
	m_softShadowErasedByObject = val;
}

void PageItem::setSoftShadowHasObjectTransparency(bool val)
{
	if (m_softShadowHasObjectTransparency == val)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SoftShadowObjectTrans, nullptr, Um::IGroup);
		ss->set("SOFT_SHADOW_OBJTRANS");
		ss->set("NEW_VALUE", val);
		ss->set("OLD_VALUE", m_softShadowHasObjectTransparency);
		undoManager->action(this, ss);
	}
	m_softShadowHasObjectTransparency = val;
}

void PageItem::toggleLock()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss;
		if (m_Locked)
			ss = new SimpleState(Um::UnLock, nullptr, Um::IUnLock);
		else
			ss = new SimpleState(Um::Lock, nullptr, Um::ILock);
		ss->set("LOCK");
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

QList<PageItem*> PageItem::getChildren() const
{
	QList<PageItem*> ret;
	return ret;
}

QList<PageItem*> PageItem::getAllChildren() const
{
	QList<PageItem*> ret;
	return ret;
}

void PageItem::toggleSizeLock()
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss;
		if (m_SizeLocked)
			ss = new SimpleState(Um::SizeUnLock, nullptr, Um::IUnLock);
		else
			ss = new SimpleState(Um::SizeLock, nullptr, Um::ILock);
		ss->set("SIZE_LOCK");
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
			ss = new SimpleState(Um::DisablePrint, nullptr, Um::IDisablePrint);
		else
			ss = new SimpleState(Um::EnablePrint, nullptr, Um::IEnablePrint);
		ss->set("PRINT_ENABLED");
		undoManager->action(this, ss);
	}
	m_PrintEnabled=!m_PrintEnabled;
}

void PageItem::setTextFlowMode(TextFlowMode mode)
{
	if (m_textFlowMode == mode)
		return;
	if (UndoManager::undoEnabled())
	{
		QString stateMessage;
		if (mode == TextFlowUsesFrameShape)
			stateMessage = Um::ObjectFrame;
		else if (mode == TextFlowUsesBoundingBox)
			stateMessage = Um::BoundingBox;
		else if (mode == TextFlowUsesContourLine)
			stateMessage = Um::ContourLine;
		else if (mode == TextFlowUsesImageClipping)
			stateMessage = Um::ImageClip;
		else
			stateMessage = Um::NoTextFlow;
		auto *ss = new SimpleState(stateMessage, QString(), Um::IFont);
		ss->set("TEXTFLOW_OLDMODE", (int) m_textFlowMode);
		ss->set("TEXTFLOW_NEWMODE", (int) mode);
		undoManager->action(this, ss);
	}
	m_textFlowMode = mode;
	
	checkTextFlowInteractions();
}

void PageItem::checkTextFlowInteractions(bool allItems)
{	
	if (m_Doc->isLoading())
		return;

	QRectF baseRect(getBoundingRect());
	checkTextFlowInteractions(baseRect, allItems);
}

void PageItem::checkTextFlowInteractions(const QRectF& baseRect, bool allItems)
{
	if (m_Doc->isLoading())
		return;

	const QList<PageItem*>* items = OnMasterPage.isEmpty() ? &m_Doc->DocItems : &m_Doc->MasterItems;

	int ids = allItems ? items->count() : items->indexOf(this);
	for (int idx = ids - 1; idx >= 0 ; --idx)
	{
		PageItem* item = items->at(idx);
		if (!item->isTextFrame() || (item == this)) // do not bother with no text frames
			continue;
		QRectF uRect(item->getBoundingRect());
		if (baseRect.intersects(uRect))
			item->update();
	}
}

void PageItem::convertTo(ItemType newType)
{
	if (m_itemType == newType)
		return; // nothing to do -> return
	assert(newType != 1);	//DEBUG CR 2005-02-06
	assert(newType != 3);	//DEBUG CR 2005-02-06
	QString fromType;
	switch (m_itemType)
	{
		case ImageFrame:
			if (isLatexFrame())
				fromType = Um::LatexFrame;
			else
				fromType = Um::ImageFrame;
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
			break;
	}
	QString toType;
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
			setUPixmap(nullptr);
			break;
	}
	/*
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::ConvertTo + " " + toType,
										  QString(Um::FromTo).arg(fromType).arg(toType));
		ss->set("CONVERT", "convert");
		ss->set("PAGEITEM", reinterpret_cast<void*>(this));
		ss->set("OLD_TYPE", m_itemType);
		ss->set("NEW_TYPE", newType);
		undoManager->action(this, ss);
	}
	*/
	m_itemType = newType;
	emit frameType(m_itemType);
}

void PageItem::setLayer(int newLayerID)
{
	if (m_layerID == newLayerID)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::SendToLayer,
		                           QString(Um::FromTo).arg(m_layerID).arg(newLayerID),
		                           Um::ILayerAction);
		ss->set("SEND_TO_LAYER");
		ss->set("OLD_LAYER", m_layerID);
		ss->set("NEW_LAYER", newLayerID);
		undoManager->action(this, ss);
	}
	m_layerID = newLayerID;
}

void PageItem::checkChanges(bool force)
{
	if (m_Doc->view() == nullptr)
		return;
	bool spreadChanges(false);

	QRectF textFlowCheckRect;
	if (force || ((textFlowMode() != TextFlowDisabled) && shouldCheck()))
	{
		if ((oldXpos  != m_xPos  || oldYpos != m_yPos) ||
			(oldWidth != m_width || oldHeight != m_height) ||
			(oldRot != m_rotation))
		{
			textFlowCheckRect = getOldBoundingRect();
			QRectF rect1 = textInteractionRegion(0.0, 0.0).boundingRect().adjusted(-1, -1, 1, 1);
			QRectF rect2 = rect1.translated(oldXpos - m_xPos, oldYpos - m_yPos);
			rect2.setWidth(qMax(1.0, rect1.width() + oldWidth - m_width));
			rect2.setHeight(qMax(1.0, rect1.height() + oldHeight - m_height));
			textFlowCheckRect = textFlowCheckRect.united(rect1.united(rect2));
		}
	}

	// has the item been resized
	if (force || ((oldWidth != m_width || oldHeight != m_height) && shouldCheck()))
	{
		resizeUndoAction();
		spreadChanges = (textFlowMode() != TextFlowDisabled);
	}
	// has the item been rotated
	if (force || ((oldRot != m_rotation) && (shouldCheck())))
	{
		rotateUndoAction();
		spreadChanges = (textFlowMode() != TextFlowDisabled);
	}
	// has the item been moved
	if (force || ((oldXpos != m_xPos || oldYpos != m_yPos) && shouldCheck()))
	{
		moveUndoAction();
		spreadChanges = (textFlowMode() != TextFlowDisabled);
	}
	// has the item's image been moved
	if (force || ((oldLocalX != m_imageXOffset || oldLocalY != m_imageYOffset) && shouldCheck()))
		changeImageOffsetUndoAction();
	// has the item's image been scaled
	if (force || ((oldLocalScX != m_imageXScale || oldLocalScY != m_imageYScale) && shouldCheck()))
		changeImageScaleUndoAction();
	
	if (spreadChanges)
	{
		checkTextFlowInteractions(textFlowCheckRect);
	}
}

bool PageItem::shouldCheck() const
{
	return ((!m_Doc->view()->mousePressed()) &&
			(!m_Doc->view()->canvasMode()->arrowKeyDown()) &&
			(!ScCore->primaryMainWindow()->propertiesPalette->userActionOn()));
}

void PageItem::moveUndoAction()
{
	if (oldXpos == m_xPos && oldYpos == m_yPos)
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

		QString unitSuffix = unitGetStrFromIndex(m_Doc->unitIndex());
		int unitPrecision  = unitGetPrecisionFromIndex(m_Doc->unitIndex());
		double unitRatio   = m_Doc->unitRatio();
		QString oxString = QString::number(oldXpos * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString oyString = QString::number(oldYpos * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nxString = QString::number(m_xPos * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nyString = QString::number(m_yPos * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString tooltip  =  QString(Um::MoveFromTo).arg(oxString, oyString, oldp, nxString, nyString, newp);
		auto *ss = new SimpleState(Um::Move, tooltip, Um::IMove);
		ss->set("ITEM_MOVE");
		ss->set("OLD_XPOS", oldXpos);
		ss->set("OLD_YPOS", oldYpos);
		ss->set("NEW_XPOS", m_xPos);
		ss->set("NEW_YPOS", m_yPos);
		undoManager->action(this, ss);
	}
	oldXpos = m_xPos;
	oldYpos = m_yPos;
	oldOwnPage = OwnPage;
}

void PageItem::resizeUndoAction()
{
	if (oldHeight == m_height && oldWidth == m_width)
		return;
	bool doUndo = true;
	if (isNoteFrame()
		&& ((asNoteFrame()->isAutoHeight() && asNoteFrame()->isAutoWidth())
			|| ((oldHeight == m_height) && asNoteFrame()->isAutoWidth())
			|| ((oldWidth == m_width) && asNoteFrame()->isAutoHeight())))
		doUndo = false;
	if (doUndo && UndoManager::undoEnabled())
	{
		QString unitSuffix = unitGetStrFromIndex(m_Doc->unitIndex());
		int unitPrecision  = unitGetPrecisionFromIndex(m_Doc->unitIndex());
		double unitRatio   = m_Doc->unitRatio();
		QString owString  = QString::number(oldWidth * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString ohString  = QString::number(oldHeight * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nwString  = QString::number(m_width * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nhString  = QString::number(m_height * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString tooltip   = QString(Um::ResizeFromTo).arg(owString, ohString, nwString, nhString);
		auto *ss = new SimpleState(Um::Resize, tooltip, Um::IResize);
		ss->set("ITEM_RESIZE");
		if (!isNoteFrame() || !asNoteFrame()->isAutoWidth())
		{
			ss->set("OLD_WIDTH", oldWidth);
			ss->set("NEW_WIDTH", m_width);
		}
		if (!isNoteFrame() || !asNoteFrame()->isAutoHeight())
		{
			ss->set("OLD_HEIGHT", oldHeight);
			ss->set("NEW_HEIGHT", m_height);
		}
		if (!isNoteFrame() || !asNoteFrame()->isAutoWelded())
		{
			ss->set("OLD_RXPOS", oldXpos);
			ss->set("OLD_RYPOS", oldYpos);
			ss->set("NEW_RXPOS", m_xPos);
			ss->set("NEW_RYPOS", m_yPos);
		}
		ss->set("OLD_RROT", oldRot);
		ss->set("NEW_RROT", m_rotation);
		undoManager->action(this, ss);
	}
	if (!isNoteFrame() || !asNoteFrame()->isAutoWidth())
		oldWidth = m_width;
	if (!isNoteFrame() || !asNoteFrame()->isAutoHeight())
		oldHeight = m_height;
	if (!isNoteFrame() || !asNoteFrame()->isAutoWelded())
	{
		oldXpos = m_xPos;
		oldYpos = m_yPos;
	}
	oldOwnPage = OwnPage;
	oldRot = m_rotation;
}

void PageItem::rotateUndoAction()
{
	if (oldRot == m_rotation)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::Rotate, QString(Um::FromTo).arg(oldRot).arg(m_rotation),
		                           Um::IRotate);
		ss->set("ITEM_ROTATE");
		ss->set("OLD_ROT", oldRot);
		ss->set("NEW_ROT", m_rotation);
		if (!isNoteFrame() || !asNoteFrame()->isAutoWelded())
		{
			ss->set("OLD_RXPOS", oldXpos);
			ss->set("OLD_RYPOS", oldYpos);
			ss->set("NEW_RXPOS", m_xPos);
			ss->set("NEW_RYPOS", m_yPos);
		}
		if (!isNoteFrame() || !asNoteFrame()->isAutoHeight())
		{
			ss->set("OLD_RHEIGHT", oldHeight);
			ss->set("NEW_RHEIGHT", m_height);
		}
		if (!isNoteFrame() || !asNoteFrame()->isAutoWidth())
		{
			ss->set("NEW_RWIDTH", m_width);
			ss->set("OLD_RWIDTH", oldWidth);
		}
		undoManager->action(this, ss);
	}
	oldRot = m_rotation;
	oldXpos = m_xPos;
	oldYpos = m_yPos;
	oldOwnPage = OwnPage;
	oldWidth = m_width;
	oldHeight = m_height;
}

void PageItem::changeImageOffsetUndoAction()
{
	if (oldLocalX == m_imageXOffset && oldLocalY == m_imageYOffset)
		return;
	if (UndoManager::undoEnabled())
	{
		QString unitSuffix = unitGetStrFromIndex(m_Doc->unitIndex());
		int unitPrecision  = unitGetPrecisionFromIndex(m_Doc->unitIndex());
		double unitRatio   = m_Doc->unitRatio();
		QString olxString  = QString::number(oldLocalX * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString olyString  = QString::number(oldLocalY * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nlxString  = QString::number(m_imageXOffset * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nlyString  = QString::number(m_imageYOffset * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString tooltip   =  QString(Um::ImageOffsetFromTo).arg(olxString, olyString, nlxString, nlyString);
		auto *ss = new SimpleState(Um::ImageOffset, tooltip, Um::IMove);
		ss->set("IMAGE_OFFSET");
		ss->set("OLD_IMAGEXOFFSET", oldLocalX);
		ss->set("OLD_IMAGEYOFFSET", oldLocalY);
		ss->set("NEW_IMAGEXOFFSET", m_imageXOffset);
		ss->set("NEW_IMAGEYOFFSET", m_imageYOffset);
		undoManager->action(this, ss);
	}
	oldLocalX = m_imageXOffset;
	oldLocalY = m_imageYOffset;
}

void PageItem::changeImageScaleUndoAction()
{
	if (oldLocalScX == m_imageXScale && oldLocalScY == m_imageYScale)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::ImageScale,
			QString(Um::ImageScaleFromTo).arg(oldLocalScX).arg(oldLocalScY).arg(m_imageXScale).arg(m_imageYScale), Um::IMove);
		ss->set("IMAGE_SCALE");
		ss->set("OLD_IMAGEXSCALE", oldLocalScX);
		ss->set("OLD_IMAGEYSCALE", oldLocalScY);
		ss->set("NEW_IMAGEXSCALE", m_imageXScale);
		ss->set("NEW_IMAGEYSCALE", m_imageYScale);
		undoManager->action(this, ss);
	}
	oldLocalScX = m_imageXScale;
	oldLocalScY = m_imageYScale;
}

void PageItem::restore(UndoState *state, bool isUndo)
{
	auto* ss = dynamic_cast<SimpleState*>(state);
	if (!ss)
	{
		qFatal("PageItem::restore: dynamic cast failed");
		return;
	}

	bool SnapGridBackup = m_Doc->SnapGrid;
	bool SnapGuidesBackup = m_Doc->SnapGuides;
	bool SnapElementBackup = m_Doc->SnapElement;
	m_Doc->SnapElement = false;
	m_Doc->SnapGrid = false;
	m_Doc->SnapGuides = false;
	bool oldMPMode = m_Doc->masterPageMode();
	m_Doc->setMasterPageMode(!OnMasterPage.isEmpty());
	ScPage *oldCurrentPage = m_Doc->currentPage();
	if (!OnMasterPage.isEmpty())
	{
		oldCurrentPage = m_Doc->currentPage();
		m_Doc->setCurrentPage(m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]));
	}

	bool actionFound = checkGradientUndoRedo(ss, isUndo);
	if (!actionFound)
	{
		if (ss->contains("ARC"))
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
		else if (ss->contains("ITEM_RESIZE"))
			restoreResize(ss, isUndo);
		else if (ss->contains("ITEM_ROTATE"))
			restoreRotate(ss, isUndo);
		else if (ss->contains("ITEM_MOVE"))
			restoreMove(ss, isUndo);
		else if (ss->contains("FILL"))
			restoreFill(ss, isUndo);
		else if (ss->contains("SHADE"))
			restoreShade(ss, isUndo);
		else if (ss->contains("LINE_COLOR"))
			restoreLineColor(ss, isUndo);
		else if (ss->contains("VERTICAL_ALIGN"))
			restoreVerticalAlign(ss, isUndo);
		else if (ss->contains("COLUMNS"))
			restoreColumns(ss, isUndo);
		else if (ss->contains("COLUMNSGAP"))
			restoreColumnsGap(ss, isUndo);
		else if (ss->contains("LINE_SHADE"))
			restoreLineShade(ss, isUndo);
		else if (ss->contains("DELETE_FRAMETEXT"))
			restoreDeleteFrameText(ss, isUndo);
		else if (ss->contains("DELETE_FRAMEPARA"))
			restoreDeleteFrameParagraph(ss, isUndo);
		else if (ss->contains("INSERT_FRAMETEXT"))
			restoreInsertFrameText(ss, isUndo);
		else if (ss->contains("INSERT_FRAMEPARA"))
			restoreInsertFrameParagraph(ss, isUndo);
		else if (ss->contains("LOREM_FRAMETEXT"))
			restoreInsertFrameText(ss, isUndo);
		else if (ss->contains("APPLY_CHARSTYLE"))
			restoreCharStyle(ss, isUndo);
		else if (ss->contains("SET_CHARSTYLE"))
			restoreSetCharStyle(ss, isUndo);
		else if (ss->contains("SET_PARASTYLE"))
			restoreSetParagraphStyle(ss, isUndo);
		else if (ss->contains("APPLY_PARASTYLE"))
			restoreParagraphStyle(ss, isUndo);
		else if (ss->contains("APPLY_DEFAULTPARASTYLE"))
			restoreDefaultParagraphStyle(ss, isUndo);
		else if (ss->contains("LEFT_TEXTFRAMEDIST"))
			restoreLeftTextFrameDist(ss, isUndo);
		else if (ss->contains("RIGHT_TEXTFRAMEDIST"))
			restoreRightTextFrameDist(ss, isUndo);
		else if (ss->contains("TOP_TEXTFRAMEDIST"))
			restoreTopTextFrameDist(ss, isUndo);
		else if (ss->contains("BOTTOM_TEXTFRAMEDIST"))
			restoreBottomTextFrameDist(ss, isUndo);
		else if (ss->contains("FIRSTLINEOFFSET"))
			restoreFirstLineOffset(ss, isUndo);
		else if (ss->contains("PASTE_PLAINTEXT"))
			restorePastePlainText(ss, isUndo);
		else if (ss->contains("PASTE_TEXT"))
			restorePasteText(ss, isUndo);
		else if (ss->contains("CORNER_RADIUS"))
			restoreCornerRadius(ss, isUndo);
		else if (ss->contains("IMAGEFLIPH"))
			flipImageH();
		else if (ss->contains("IMAGEFLIPV"))
			flipImageV();
		else if (ss->contains("OVERPRINT"))
		{
			if (isUndo)
				doOverprint = !ss->getBool("OVERPRINT");
			else
				doOverprint = ss->getBool("OVERPRINT");
		}
		else if (ss->contains("FILLBLENDMODE"))
		{
			if (isUndo)
				m_fillBlendMode = ss->getInt("FILLBLENDMODE_OLD");
			else
				m_fillBlendMode = ss->getInt("FILLBLENDMODE");
		}
		else if (ss->contains("ACTIONPDFANNOTATION"))
		{
			if (isUndo)
				m_isAnnotation = !ss->getBool("ACTIONPDFANNOTATION");
			else
				m_isAnnotation = ss->getBool("ACTIONPDFANNOTATION");
		}
		else if (ss->contains("ACTIONPDFBOOKMARK"))
		{
			if (isUndo)
				isBookmark = !ss->getBool("ACTIONPDFBOOKMARK");
			else
				isBookmark = ss->getBool("ACTIONPDFBOOKMARK");
		}
		else if (ss->contains("LINEBLENDMODE"))
		{
			if (isUndo)
				m_lineBlendMode = ss->getInt("LINEBLENDMODE_OLD");
			else
				m_lineBlendMode = ss->getInt("LINEBLENDMODE");
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
			restoreFillTransparency(ss, isUndo);
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
			bool editContour = m_Doc->nodeEdit.isContourLine();
			m_Doc->nodeEdit.setIsContourLine(ss->getBool("IS_CONTOUR"));
			select();
			m_Doc->MirrorPolyH(m_Doc->m_Selection->itemAt(0));
			m_Doc->nodeEdit.setIsContourLine(editContour);
		}
		else if (ss->contains("MIRROR_PATH_V"))
		{
			bool editContour = m_Doc->nodeEdit.isContourLine();
			m_Doc->nodeEdit.setIsContourLine(ss->getBool("IS_CONTOUR"));
			select();
			m_Doc->MirrorPolyV(m_Doc->m_Selection->itemAt(0));
			m_Doc->nodeEdit.setIsContourLine(editContour);
		}
		else if (ss->contains("SEND_TO_LAYER"))
			restoreLayer(ss, isUndo);
		else if (ss->contains("GET_IMAGE"))
			restoreGetImage(ss, isUndo);
		else if (ss->contains("EDIT_SHAPE_OR_CONTOUR"))
			restoreShapeContour(ss, isUndo);
		else if (ss->contains("APPLY_IMAGE_EFFECTS"))
			restoreImageEffects(ss, isUndo);
		else if (ss->contains("DROP_LINKS"))
			restoreDropLinks(ss, isUndo);
		else if (ss->contains("LINK_TEXT_FRAME"))
			restoreLinkTextFrame(ss, isUndo);
		else if (ss->contains("UNLINK_TEXT_FRAME"))
			restoreUnlinkTextFrame(ss, isUndo);
		else if (ss->contains("CLEAR_IMAGE"))
			restoreClearImage(ss, isUndo);
		else if (ss->contains("PASTE_INLINE"))
			restorePasteInline(ss, isUndo);
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
		else if (ss->contains("WELD_ITEMS"))
			restoreWeldItems(ss, isUndo);
		else if (ss->contains("UNWELD_ITEM"))
			restoreUnWeldItem(ss, isUndo);
		else if (ss->contains("CLEARMARKSTRING"))
			restoreMarkString(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW"))
			restoreSoftShadow(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_COLOR"))
			restoreSoftShadowColor(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_SHADE"))
			restoreSoftShadowShade(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_BLUR_RADIUS"))
			restoreSoftShadowBlurRadius(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_XOFFSET"))
			restoreSoftShadowXOffset(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_YOFFSET"))
			restoreSoftShadowYOffset(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_OPACITY"))
			restoreSoftShadowOpacity(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_BLEND_MODE"))
			restoreSoftShadowBlendMode(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_ERASE"))
			restoreSoftShadowErasedByObject(ss, isUndo);
		else if (ss->contains("SOFT_SHADOW_OBJTRANS"))
			restoreSoftShadowHasObjectTransparency(ss, isUndo);
	}

	if (!OnMasterPage.isEmpty())
		m_Doc->setCurrentPage(oldCurrentPage);
	m_Doc->setMasterPageMode(oldMPMode);
	m_Doc->SnapElement = SnapElementBackup;
	m_Doc->SnapGrid = SnapGridBackup;
	m_Doc->SnapGuides = SnapGuidesBackup;
	if (state->transactionCode == 0 || state->transactionCode == 2)
		this->update();
}

void PageItem::restoreConnectPath(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<FPointArray> *>(state);
	if (isUndo)
	{
		PoLine = is->getOldState();
		int oldRotMode = m_Doc->rotationMode();
		m_Doc->setRotationMode(0);
		m_Doc->adjustItemSize(this);
		m_Doc->setRotationMode(oldRotMode);
		moveBy(is->getDouble("OLDX") - xPos(),is->getDouble("OLDY") - yPos());
	}
	else
	{
		PoLine = is->getNewState();
		int oldRotMode = m_Doc->rotationMode();
		m_Doc->setRotationMode(0);
		m_Doc->adjustItemSize(this);
		m_Doc->setRotationMode(oldRotMode);
		moveBy(is->getDouble("NEWX") - xPos(),is->getDouble("NEWY") - yPos());
	}
	OldB2 = width();
	OldH2 = height();
	updateClip();
	ContourLine = PoLine.copy();
}

void PageItem::restoreUnWeldItem(SimpleState *state, bool isUndo)
{
	if (isUndo)
	{
		const auto *is = dynamic_cast<ScItemState<PageItem*>*>(state);
		PageItem* wIt = is->getItem();
		{
			WeldingInfo wInf;
			wInf.weldItem = wIt;
			wInf.weldID = is->getInt("thisID");
			wInf.weldPoint = FPoint(is->getDouble("thisPoint_x"), is->getDouble("thisPoint_y"));
			weldList.append(wInf);
		}
		{
			WeldingInfo wInf;
			wInf.weldItem = this;
			wInf.weldID = is->getInt("ID");
			wInf.weldPoint = FPoint(is->getDouble("Point_x"), is->getDouble("Point_y"));
			wIt->weldList.append(wInf);
		}
	}
	else
	{
		unWeld();
	}
	m_Doc->changed();
	m_Doc->regionsChanged()->update(QRectF());
}

void PageItem::restoreWeldItems(SimpleState *state, bool isUndo)
{
	if (isUndo)
	{
		unWeld();
	}
	else
	{
		const auto *is = dynamic_cast<ScItemState<PageItem*>*>(state);
		PageItem* wIt = is->getItem();
		weldTo(wIt);
	}
	m_Doc->changed();
	m_Doc->regionsChanged()->update(QRectF());
}

void PageItem::restoreSoftShadow(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_hasSoftShadow = state->getBool("OLD_VALUE");
	else
		m_hasSoftShadow = state->getBool("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowColor(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowColor = state->get("OLD_VALUE");
	else
		m_softShadowColor = state->get("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowShade(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowShade = state->getInt("OLD_VALUE");
	else
		m_softShadowShade = state->getInt("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowBlurRadius(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowBlurRadius = state->getDouble("OLD_VALUE");
	else
		m_softShadowBlurRadius = state->getDouble("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowXOffset(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowXOffset = state->getDouble("OLD_VALUE");
	else
		m_softShadowXOffset = state->getDouble("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowYOffset(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowYOffset = state->getDouble("OLD_VALUE");
	else
		m_softShadowYOffset = state->getDouble("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowOpacity(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowOpacity = state->getDouble("OLD_VALUE");
	else
		m_softShadowOpacity = state->getDouble("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowErasedByObject(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowErasedByObject = state->getInt("OLD_VALUE");
	else
		m_softShadowErasedByObject = state->getInt("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowHasObjectTransparency(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowHasObjectTransparency = state->getInt("OLD_VALUE");
	else
		m_softShadowHasObjectTransparency = state->getInt("NEW_VALUE");
	update();
}

void PageItem::restoreSoftShadowBlendMode(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_softShadowBlendMode = state->getInt("OLD_VALUE");
	else
		m_softShadowBlendMode = state->getInt("NEW_VALUE");
	update();
}

void PageItem::restoreMarkString(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState< QPair<int,QString> >*>(state);
	if (!is)
		return;
	Mark* mark = itemText.mark(is->getItem().first);
	if (!itemText.hasMark(is->getItem().first))
		return;
	if (isUndo)
		mark->setString(is->getItem().second);
	else
		mark->setString(QString());
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
	if (ss->contains("FILL_PATTERN"))
	{
		restoreFillPattern(ss, isUndo);
		return true;
	}
	if (ss->contains("FILL_PATTERN_FLIP"))
	{
		restoreFillPatternFlip(ss, isUndo);
		return true;
	}
	if (ss->contains("FILL_PATTERN_TRANSFORM"))
	{
		restoreFillPatternTransform(ss, isUndo);
		return true;
	}
	if (ss->contains("STROKE_GRAD"))
	{
		restoreStrokeGradient(ss, isUndo);
		return true;
	}
	if (ss->contains("STROKE_GRADIENT_NAME"))
	{
		restoreStrokeGradientName(ss, isUndo);
		return true;
	}
	if (ss->contains("STROKE_PATTERN"))
	{
		restoreStrokePattern(ss, isUndo);
		return true;
	}
	if (ss->contains("STROKE_PATTERN_FLIP"))
	{
		restoreStrokePatternFlip(ss, isUndo);
		return true;
	}
	if (ss->contains("STROKE_PATTERN_TRANSFORM"))
	{
		restoreStrokePatternTransform(ss, isUndo);
		return true;
	}
	if (ss->contains("STROKE_PATTERN_TO_PATH"))
	{
		restoreStrokePatternToPath(ss, isUndo);
		return true;
	}
	if (ss->contains("MASK_GRAD"))
	{
		restoreMaskGradient(ss, isUndo);
		return true;
	}
	if (ss->contains("MASK_PATTERN"))
	{
		restoreMaskPattern(ss, isUndo);
		return true;
	}
	if (ss->contains("MASK_FLIP"))
	{
		restoreMaskFlip(ss, isUndo);
		return true;
	}
	if (ss->contains("MASK_TRANSFORM"))
	{
		restoreMaskTransform(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MESH_COLOR"))
	{
		restoreGradientMeshColor(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_START"))
	{
		restoreGradientStrokeStart(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_END"))
	{
		restoreGradientStrokeEnd(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_FOCAL"))
	{
		restoreGradientStrokeFocal(ss, isUndo);
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
	if (ss->contains("GRAD_START"))
	{
		restoreGradientStart(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_END"))
	{
		restoreGradientEnd(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_FOCAL"))
	{
		restoreGradientFocal(ss, isUndo);
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
	if (ss->contains("GRAD_MASKSTART"))
	{
		restoreGradientMaskStart(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKEND"))
	{
		restoreGradientMaskEnd(ss, isUndo);
		return true;
	}
	if (ss->contains("GRAD_MASKFOCAL"))
	{
		restoreGradientMaskFocal(ss, isUndo);
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
	if (ss->contains("GRAD_EXTENT"))
	{
		restoreGradientExtent(ss, isUndo);
		return true;
	}
	if (ss->contains("GRADSTROKE_EXTENT"))
	{
		restoreGradientExtentStroke(ss, isUndo);
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
	if (isUndo)
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
	const auto *ss = dynamic_cast<ScOldNewState<FPointArray> *>(state);
	if (!ss)
	{
		qFatal("PageItem::restoreArc: dynamic cast failed");
		return;
	}

	PageItem_Arc *item = asArc();
	if (isUndo)
	{
		item->arcHeight = ss->getDouble("OLD_HEIGHT");
		item->arcWidth = ss->getDouble("OLD_WIDTH");
		item->arcStartAngle = ss->getDouble("OLD_START");
		item->arcSweepAngle = ss->getDouble("OLD_SWEEP");
		item->PoLine = ss->getOldState();
		doc()->adjustItemSize(item);
		moveBy(ss->getDouble("OLD_XPOS") - xPos(),ss->getDouble("OLD_YPOS") - yPos());
	}
	else
	{
		item->arcHeight = ss->getDouble("NEW_HEIGHT");
		item->arcWidth = ss->getDouble("NEW_WIDTH");
		item->arcStartAngle = ss->getDouble("NEW_START");
		item->arcSweepAngle = ss->getDouble("NEW_SWEEP");
		item->PoLine = ss->getNewState();
		doc()->adjustItemSize(item);
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
	const auto *is = dynamic_cast<ScItemState<QList<QTransform > >*>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreTransform: dynamic cast failed");
		return;
	}

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
		doc()->adjustItemSize(this);
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
		doc()->adjustItemSize(this);
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
	const auto *is = dynamic_cast<ScItemState<QList<FPoint> > *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradPos: dynamic cast failed");
		return;
	}

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
	if (GrType == Gradient_Conical)
		createConicalMesh();
	update();
}

void PageItem::restoreGradientColor1(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<QColor> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientColor1: dynamic cast failed");
		return;
	}
	if (isUndo)
		m_grQColorP1 = is->getOldState();
	else
		m_grQColorP1 = is->getNewState();
	update();
}

void PageItem::restoreGradientColor2(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<QColor> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientColor2: dynamic cast failed");
		return;
	}
	if (isUndo)
		m_grQColorP2 = is->getOldState();
	else
		m_grQColorP2 = is->getNewState();
	update();
}

void PageItem::restoreGradientColor3(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<QColor> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientColor3: dynamic cast failed");
		return;
	}
	if (isUndo)
		m_grQColorP3 = is->getOldState();
	else
		m_grQColorP3 = is->getNewState();
	update();
}

void PageItem::restoreGradientColor4(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<QColor> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientColor4: dynamic cast failed");
		return;
	}
	if (isUndo)
		m_grQColorP4 = is->getOldState();
	else
		m_grQColorP4 = is->getNewState();
	update();
}

void PageItem::restoreRemoveMeshPatch(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<meshGradientPatch> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreRemoveMeshPatch: dynamic cast failed");
		return;
	}
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
	const auto *is = dynamic_cast<ScItemState<QList<QList<MeshPoint> > > *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreCreateMeshGrad: dynamic cast failed");
		return;
	}
	if (isUndo)
		meshGradientArray = is->getItem();
	else
		createGradientMesh(is->getInt("ROW"), is->getInt("COL"));
	update();
}

void PageItem::restoreMoveMeshGrad(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<QPair<QList<QList<MeshPoint> >,FPointArray> > *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreMoveMeshGrad: dynamic cast failed");
		return;
	}
	if (isUndo)
	{
		PoLine = is->getItem().second;
		ClipEdited = true;
		FrameType = is->getInt("FRAME_TYPE");
		FPoint wh = getMaxClipF(&PoLine);
		setWidthHeight(wh.x(),wh.y());
		m_Doc->adjustItemSize(this);
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
	const auto *is = dynamic_cast<ScItemState<QList<QList<MeshPoint> > > *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreResetMeshGrad: dynamic cast failed");
		return;
	}
	if (isUndo)
		meshGradientArray = is->getItem();
	else
		resetGradientMesh();
	update();
}

void PageItem::restoreMoveMeshPatch(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<MeshPoint> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreMoveMeshPatch: dynamic cast failed");
		return;
	}

	int x = is->getInt("X");
	int y = is->getInt("Y");
	if (isUndo)
	{
		if (is->getBool("ARRAY"))
			meshGradientArray[x][y] = is->getOldState();
		else
			switch (y)
			{
				case 1:
					meshGradientPatches[x].TL = is->getOldState();
					break;
				case 2:
					meshGradientPatches[x].TR = is->getOldState();
					break;
				case 3:
					meshGradientPatches[x].BR = is->getOldState();
					break;
				case 4:
					meshGradientPatches[x].BL = is->getOldState();
					break;
			}
	}
	else
	{
		if (is->getBool("ARRAY"))
			meshGradientArray[x][y] = is->getNewState();
		else
			switch (y)
			{
				case 1:
					meshGradientPatches[x].TL = is->getNewState();
					break;
				case 2:
					meshGradientPatches[x].TR = is->getNewState();
					break;
				case 3:
					meshGradientPatches[x].BR = is->getNewState();
					break;
				case 4:
					meshGradientPatches[x].BL = is->getNewState();
					break;
			}
	}
	update();
}

void PageItem::restoreFillGradient(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<VGradient> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreFillGradient: dynamic cast failed");
		return;
	}

	if (isUndo)
		fill_gradient = is->getOldState();
	else
		fill_gradient = is->getNewState();
	if (gradientType() == 13)
		createConicalMesh();
	update();
}

void PageItem::restoreFillPattern(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<QString> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreFillPattern: dynamic cast failed");
		return;
	}

	if (isUndo)
		m_patternName = is->getOldState();
	else
		m_patternName = is->getNewState();
	update();
}

void PageItem::restoreFillPatternFlip(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState< QPair<bool, bool> > *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreFillPatternFlip: dynamic cast failed");
		return;
	}

	if (isUndo)
	{
		const auto& oldFlips = is->getOldState();
		m_patternMirrorX = oldFlips.first;
		m_patternMirrorY = oldFlips.second;
	}
	else
	{
		const auto& newFlips = is->getNewState();
		m_patternMirrorX = newFlips.first;
		m_patternMirrorY = newFlips.second;
	}
	update();
}

void PageItem::restoreFillPatternTransform(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<ScPatternTransform> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreFillPatternTransform: dynamic cast failed");
		return;
	}

	if (isUndo)
		m_patternTransform = is->getOldState();
	else
		m_patternTransform = is->getNewState();
	update();
}

void PageItem::restoreMaskGradient(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<VGradient> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreMaskGradient: dynamic cast failed");
		return;
	}
	if (isUndo)
		mask_gradient = is->getOldState();
	else
		mask_gradient = is->getNewState();
	update();
}

void PageItem::restoreMaskPattern(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<QString> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreMaskPattern: dynamic cast failed");
		return;
	}
	if (isUndo)
		patternMaskVal = is->getOldState();
	else
		patternMaskVal = is->getNewState();
	update();
}

void PageItem::restoreMaskFlip(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState< QPair<bool, bool> > *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreMaskFlip: dynamic cast failed");
		return;
	}

	if (isUndo)
	{
		const auto& oldFlips = is->getOldState();
		patternMaskMirrorX = oldFlips.first;
		patternMaskMirrorY = oldFlips.second;
	}
	else
	{
		const auto& newFlips = is->getNewState();
		patternMaskMirrorX = newFlips.first;
		patternMaskMirrorY = newFlips.second;
	}
	update();
}

void PageItem::restoreMaskTransform(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<ScPatternTransform> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreMaskTransform: dynamic cast failed");
		return;
	}

	if (isUndo)
		patternMaskTransfrm = is->getOldState();
	else
		patternMaskTransfrm = is->getNewState();
	update();
}

void PageItem::restoreStrokeGradient(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<VGradient> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreStrokeGradient: dynamic cast failed");
		return;
	}
	if (isUndo)
		stroke_gradient = is->getOldState();
	else
		stroke_gradient = is->getNewState();
	update();
}

void PageItem::restoreStrokeGradientName(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<QString> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreStrokeGradientName: dynamic cast failed");
		return;
	}
	if (isUndo)
		gradientStrokeVal = is->getOldState();
	else
		gradientStrokeVal = is->getNewState();
	update();
}

void PageItem::restoreStrokePattern(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<QString> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreStrokePattern: dynamic cast failed");
		return;
	}
	if (isUndo)
		patternStrokeVal = is->getOldState();
	else
		patternStrokeVal = is->getNewState();
	update();
}

void PageItem::restoreStrokePatternFlip(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState< QPair<bool, bool> > *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreStrokePatternFlip: dynamic cast failed");
		return;
	}

	if (isUndo)
	{
		const auto& oldFlips = is->getOldState();
		patternStrokeMirrorX = oldFlips.first;
		patternStrokeMirrorY = oldFlips.second;
	}
	else
	{
		const auto& newFlips = is->getNewState();
		patternStrokeMirrorX = newFlips.first;
		patternStrokeMirrorY = newFlips.second;
	}
	update();
}

void PageItem::restoreStrokePatternTransform(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<ScStrokePatternTransform> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreStrokePatternTransform: dynamic cast failed");
		return;
	}

	if (isUndo)
		patternStrokeTransfrm = is->getOldState();
	else
		patternStrokeTransfrm = is->getNewState();
	update();
}

void PageItem::restoreStrokePatternToPath(SimpleState* state, bool isUndo)
{
	const auto* is = dynamic_cast<ScOldNewState<bool> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreStrokePatternToPath: dynamic cast failed");
		return;
	}

	if (isUndo)
		patternStrokePath = is->getOldState();
	else
		patternStrokePath = is->getNewState();
	update();
}

void PageItem::restoreGradientMeshColor(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<QColor> *>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreGradientMeshColor: dynamic cast failed");
		return;
	}

	int x = is->getInt("X");
	int y = is->getInt("Y");
	MeshPoint *mp = nullptr;
	if (is->getBool("PATCH"))
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

	if (mp == nullptr)
	{
		qFatal("PageItem::restoreGradientMeshColor: mp is a nullptr");
		return;
	}

	if (isUndo)
	{
		mp->colorName = is->get("OLD_COLOR_NAME");
		mp->color = is->getOldState();
		mp->shade = is->getInt("OLD_SHADE");
		mp->transparency = is->getDouble("OLD_TRANS");
	}
	else
	{
		mp->colorName = is->get("NEW_COLOR_NAME");
		mp->color = is->getNewState();
		mp->shade = is->getInt("NEW_SHADE");
		mp->transparency = is->getDouble("NEW_TRANS");
	}
}

void PageItem::restoreGradientStart(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrStartX = is->getDouble("OLDX");
		GrStartY = is->getDouble("OLDY");
	}
	else
	{
		GrStartX = is->getDouble("NEWX");
		GrStartY = is->getDouble("NEWY");
	}
}

void PageItem::restoreGradientEnd(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrEndX = is->getDouble("OLDX");
		GrEndY = is->getDouble("OLDY");
	}
	else
	{
		GrEndX = is->getDouble("NEWX");
		GrEndY = is->getDouble("NEWY");
	}
}

void PageItem::restoreGradientFocal(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrFocalX = is->getDouble("OLDX");
		GrFocalY = is->getDouble("OLDY");
	}
	else
	{
		GrFocalX = is->getDouble("NEWX");
		GrFocalY = is->getDouble("NEWY");
	}
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

void PageItem::restoreGradientMaskStart(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrMaskStartX = is->getDouble("OLDX");
		GrMaskStartY = is->getDouble("OLDY");
	}
	else
	{
		GrMaskStartX = is->getDouble("NEWX");
		GrMaskStartY = is->getDouble("NEWY");
	}
}

void PageItem::restoreGradientMaskEnd(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrMaskEndX = is->getDouble("OLDX");
		GrMaskEndY = is->getDouble("OLDY");
	}
	else
	{
		GrMaskEndX = is->getDouble("NEWX");
		GrMaskEndY = is->getDouble("NEWY");
	}
}

void PageItem::restoreGradientMaskFocal(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrMaskFocalX = is->getDouble("OLDX");
		GrMaskFocalY = is->getDouble("OLDY");
	}
	else
	{
		GrMaskFocalX = is->getDouble("NEWX");
		GrMaskFocalY = is->getDouble("NEWY");
	}
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
	const auto *is = dynamic_cast<ScOldNewState<FPoint> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientControl1: dynamic cast failed");
		return;
	}
	if (isUndo)
		GrControl1 = is->getOldState();
	else
		GrControl1 = is->getNewState();
}

void PageItem::restoreGradientControl2(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<FPoint> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientControl2: dynamic cast failed");
		return;
	}
	if (isUndo)
		GrControl2 = is->getOldState();
	else
		GrControl2 = is->getNewState();
}

void PageItem::restoreGradientControl3(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<FPoint> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientControl3: dynamic cast failed");
		return;
	}
	if (isUndo)
		GrControl3 = is->getOldState();
	else
		GrControl3 = is->getNewState();
}

void PageItem::restoreGradientControl4(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<FPoint> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientControl4: dynamic cast failed");
		return;
	}
	if (isUndo)
		GrControl4 = is->getOldState();
	else
		GrControl4 = is->getNewState();
}

void PageItem::restoreGradientControl5(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<FPoint> *>(state);
	if (!is)
	{
		qFatal("PageItem::restoreGradientControl5: dynamic cast failed");
		return;
	}
	if (isUndo)
		GrControl5 = is->getOldState();
	else
		GrControl5 = is->getNewState();
}

void PageItem::restoreGradientStrokeScale(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeScale = is->getDouble("OLD");
	else
		GrStrokeScale = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeSkew(SimpleState *is, bool isUndo)
{
	if (isUndo)
		GrStrokeSkew = is->getDouble("OLD");
	else
		GrStrokeSkew = is->getDouble("NEW");
}

void PageItem::restoreGradientStrokeFocal(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrStrokeFocalX = is->getDouble("OLDX");
		GrStrokeFocalY = is->getDouble("OLDY");
	}
	else
	{
		GrStrokeFocalX = is->getDouble("NEWX");
		GrStrokeFocalY = is->getDouble("NEWY");
	}
}

void PageItem::restoreGradientStrokeStart(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrStrokeStartX = is->getDouble("OLDX");
		GrStrokeStartY = is->getDouble("OLDY");
	}
	else
	{
		GrStrokeStartX = is->getDouble("NEWX");
		GrStrokeStartY = is->getDouble("NEWY");
	}
}

void PageItem::restoreGradientStrokeEnd(SimpleState *is, bool isUndo)
{
	if (isUndo)
	{
		GrStrokeEndX = is->getDouble("OLDX");
		GrStrokeEndY = is->getDouble("OLDY");
	}
	else
	{
		GrStrokeEndX = is->getDouble("NEWX");
		GrStrokeEndY = is->getDouble("NEWY");
	}
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

void PageItem::restoreGradientExtent(SimpleState* is, bool isUndo)
{
	const auto* ss = dynamic_cast<ScOldNewState<VGradient::VGradientRepeatMethod> *>(is);
	if (!ss)
	{
		qFatal("PageItem::restoreGradientExtent: dynamic cast failed");
		return;
	}

	if (isUndo)
		GrExtend = ss->getOldState();
	else
		GrExtend = ss->getNewState();
}

void PageItem::restoreGradientExtentStroke(SimpleState* is, bool isUndo)
{
	const auto* ss = dynamic_cast<ScOldNewState<VGradient::VGradientRepeatMethod> *>(is);
	if (!ss)
	{
		qFatal("PageItem::restoreGradientExtentStroke: dynamic cast failed");
		return;
	}

	if (isUndo)
		GrStrokeExtend = ss->getOldState();
	else
		GrStrokeExtend = ss->getNewState();
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
		m_imageRotation = is->getInt("OLD_ROT");
	else
		m_imageRotation = is->getInt("NEW_ROT");
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

void PageItem::restorePastePlainText(SimpleState *ss, bool isUndo)
{
	int start = ss->getInt("START");
	QString text(ss->get("TEXT"));

	itemText.deselectAll();
	if (isUndo)
	{
		itemText.select(start, text.length());
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	else
	{
		itemText.insertChars(start, text, true);
		itemText.setCursorPosition(start + text.length());
	}
}

void PageItem::restorePasteText(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<StoryText>*>(ss);
	if (!is)
	{
		qFatal("PageItem::restorePasteText: dynamic cast failed");
		return;
	}

	int start = is->getInt("START");
	itemText.deselectAll();
	if (isUndo)
	{
		itemText.select(start, is->getItem().length());
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	else
	{
		itemText.insert(start, is->getItem());
		itemText.setCursorPosition(start + is->getItem().length());
	}
}

void PageItem::restoreColumnsGap(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		m_columnGap = ss->getDouble("OLD_COLUMNS");
	else
		m_columnGap = ss->getDouble("NEW_COLUMNS");
	update();
}

void PageItem::restoreLeftTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		m_textDistanceMargins.setLeft(ss->getDouble("OLD_DIST"));
	else
		m_textDistanceMargins.setLeft(ss->getDouble("NEW_DIST"));
	update();
}

void PageItem::restoreRightTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		m_textDistanceMargins.setRight(ss->getDouble("OLD_DIST"));
	else
		m_textDistanceMargins.setRight(ss->getDouble("NEW_DIST"));
	update();
}

void PageItem::restoreTopTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		m_textDistanceMargins.setTop(ss->getDouble("OLD_DIST"));
	else
		m_textDistanceMargins.setTop(ss->getDouble("NEW_DIST"));
	update();
}

void PageItem::restoreBottomTextFrameDist(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		m_textDistanceMargins.setBottom(ss->getDouble("OLD_DIST"));
	else
		m_textDistanceMargins.setBottom(ss->getDouble("NEW_DIST"));
	update();
}

void PageItem::restoreColumns(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		m_columns = ss->getInt("OLD_COLUMNS");
	else
		m_columns = ss->getInt("NEW_COLUMNS");
	update();
}

void PageItem::restoreFirstLineOffset(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<FirstLineOffsetPolicy>*>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreFirstLineOffset: dynamic cast failed");
		return;
	}
	if (isUndo)
		m_firstLineOffset = is->getOldState();
	else
		m_firstLineOffset = is->getNewState();
	update();
}

void PageItem::restoreDefaultParagraphStyle(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<ParagraphStyle>*>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreDefaultParagraphStyle: dynamic cast failed");
		return;
	}
	if (isUndo)
		itemText.setDefaultStyle(is->getOldState());
	else
		itemText.setDefaultStyle(is->getNewState());
}

void PageItem::restoreParagraphStyle(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<ParagraphStyle> *>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreParagraphStyle: dynamic cast failed");
		return;
	}

	int pos = is->getInt("POS");
	if (isUndo)
	{
		itemText.eraseStyle(pos, is->getNewState());
		itemText.applyStyle(pos, is->getOldState());
	}
	else
		itemText.applyStyle(pos, is->getNewState());
}

void PageItem::restoreCharStyle(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<CharStyle>*>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreCharStyle: dynamic cast failed");
		return;
	}
	int length = is->getInt("LENGTH");
	int start = is->getInt("START");
	if (isUndo)
	{
		itemText.eraseCharStyle(start,length, is->getNewState());
		itemText.applyCharStyle(start,length, is->getOldState());
	}
	else
		itemText.applyCharStyle(start,length, is->getNewState());
}

void PageItem::restoreSetCharStyle(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<CharStyle>*>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreSetCharStyle: dynamic cast failed");
		return;
	}
	int length = is->getInt("LENGTH");
	int start = is->getInt("START");
	if (isUndo)
		itemText.setCharStyle(start, length, is->getOldState());
	else
		itemText.setCharStyle(start, length, is->getNewState());
}

void PageItem::restoreSetParagraphStyle(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScOldNewState<ParagraphStyle>*>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreSetParagraphStyle: dynamic cast failed");
		return;
	}
	int pos = is->getInt("POS");
	if (isUndo)
		itemText.setStyle(pos, is->getOldState());
	else
		itemText.setStyle(pos, is->getNewState());
}

void PageItem::restoreDeleteFrameText(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<CharStyle> *>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreDeleteFrameText: dynamic cast failed");
		return;
	}

	QString text(is->get("TEXT_STR"));
	int start = is->getInt("START");
	itemText.deselectAll();
	if (isUndo)
	{
		itemText.insertChars(start, text);
		itemText.applyCharStyle(start, text.length(), is->getItem());
		itemText.setCursorPosition(start + text.length());
		invalid = true;
		invalidateLayout();
	}
	else
	{
		itemText.select(start, text.length());
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	update();
}

void PageItem::restoreDeleteFrameParagraph(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<ParagraphStyle> *>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreDeleteFrameParagraph: dynamic cast failed");
		return;
	}

	int start = is->getInt("START");
	itemText.deselectAll();
	if (isUndo)
	{
		itemText.insertChars(start, SpecialChars::PARSEP);
		itemText.applyStyle(start, is->getItem());
		itemText.setCursorPosition(start + 1);
		invalid = true;
		invalidateLayout();
	}
	else
	{
		itemText.select(start, 1);
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	update();
}

void PageItem::restoreInsertFrameText(SimpleState *ss, bool isUndo)
{
	QString text(ss->get("TEXT_STR"));
	int start = ss->getInt("START");

	itemText.deselectAll();
	if (isUndo)
	{
		itemText.select(start, text.length());
		asTextFrame()->deleteSelectedTextFromFrame();
	}
	else
	{
		itemText.insertChars(start, text);
		itemText.setCursorPosition(start + text.length());
	}
}

void PageItem::restoreInsertFrameParagraph(SimpleState *ss, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<ParagraphStyle> *>(ss);
	if (!is)
	{
		qFatal("PageItem::restoreInsertFrameParagraph: dynamic cast failed");
		return;
	}

	int start = is->getInt("START");
	itemText.deselectAll();
	if (isUndo)
	{
		itemText.select(start, 1);
		asTextFrame()->deleteSelectedTextFromFrame();
		itemText.applyStyle(start, is->getItem());
	}
	else
	{
		itemText.insertChars(start, SpecialChars::PARSEP);
		itemText.setCursorPosition(start + 1);
		invalid = true;
		invalidateLayout();
	}
	update();
}

void PageItem::restoreCornerRadius(SimpleState *state, bool isUndo)
{
	if (isUndo)
		m_roundedCornerRadius = state->getDouble("OLD_RADIUS");
	else
		m_roundedCornerRadius = state->getDouble("NEW_RADIUS");
	Selection tmpSelection = *(doc()->m_Selection);
	doc()->m_Selection->clear();
	doc()->m_Selection->addItem(this);
	doc()->setFrameRounded();
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
	m_Doc->moveItem(mx, my, this);
	oldXpos = m_xPos;
	oldYpos = m_yPos;
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
		m_Doc->sizeItem(ow, oh, this, false, true, redraw);
		m_Doc->moveItem(mx, my, this);
		m_Doc->rotateItem(ort, this);
	}
	else
	{
		mx = -mx;
		my = -my;
		m_Doc->sizeItem(w, h, this, false, true, redraw);
		m_Doc->moveItem(mx, my, this);
		m_Doc->rotateItem(rt, this);
	}
	oldWidth = m_width;
	oldHeight = m_height;
	oldXpos = m_xPos;
	oldYpos = m_yPos;
	oldOwnPage = OwnPage;
	oldRot = m_rotation;
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
		m_Doc->rotateItem(ort, this);
		m_Doc->moveItem(ox - m_xPos, oy - m_yPos, this);
		m_Doc->sizeItem(ow, oh, this, false, true, redraw);
	}
	else
	{
		m_Doc->rotateItem(rt, this);
		m_Doc->moveItem(x - m_xPos, y - m_yPos, this);
		m_Doc->sizeItem(w, h, this, false, true, redraw);
	}
	oldRot = m_rotation;
	oldXpos = m_xPos;
	oldYpos = m_yPos;
	oldOwnPage = OwnPage;
	oldWidth = m_width;
	oldHeight = m_height;
}

void PageItem::restoreShowImage(SimpleState *state, bool isUndo)
{
	bool old = state->getBool("OLD");
	if (isUndo)
		m_imageVisible = old;
	else
		m_imageVisible = !old;
	update();
}

void PageItem::restoreFill(SimpleState *state, bool isUndo)
{
	QString fill(state->get("OLD_FILL"));
	if (!isUndo)
		fill = state->get("NEW_FILL");
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetItemBrush(fill, &tempSelection);
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
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetItemBrushShade(shade, &tempSelection);
}

void PageItem::restoreLineColor(SimpleState *state, bool isUndo)
{
	QString fill(state->get("OLD_COLOR"));
	if (!isUndo)
		fill = state->get("NEW_COLOR");
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetItemPen(fill, &tempSelection);
}

void PageItem::restoreLineShade(SimpleState *state, bool isUndo)
{
	int shade = state->getInt("OLD_SHADE");
	if (!isUndo)
		shade = state->getInt("NEW_SHADE");
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetItemPenShade(shade, &tempSelection);
}

void PageItem::restoreFillTransparency(SimpleState *state, bool isUndo)
{
	double tp = state->getDouble("OLD_TP");
	if (!isUndo)
		tp = state->getDouble("NEW_TP");
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetItemFillTransparency(tp, &tempSelection);
}

void PageItem::restoreLineTP(SimpleState *state, bool isUndo)
{
	double tp = state->getDouble("OLD_TP");
	if (!isUndo)
		tp = state->getDouble("NEW_TP");
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetItemLineTransparency(tp, &tempSelection);
}


void PageItem::restoreLineStyle(SimpleState *state, bool isUndo)
{
	Qt::PenStyle ps = static_cast<Qt::PenStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		ps = static_cast<Qt::PenStyle>(state->getInt("NEW_STYLE"));
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetLineArt(ps, &tempSelection);
}

void PageItem::restoreLineEnd(SimpleState *state, bool isUndo)
{
	Qt::PenCapStyle pcs = static_cast<Qt::PenCapStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		pcs = static_cast<Qt::PenCapStyle>(state->getInt("NEW_STYLE"));
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetLineEnd(pcs, &tempSelection);
}

void PageItem::restoreLineJoin(SimpleState *state, bool isUndo)
{
	Qt::PenJoinStyle pjs = static_cast<Qt::PenJoinStyle>(state->getInt("OLD_STYLE"));
	if (!isUndo)
		pjs = static_cast<Qt::PenJoinStyle>(state->getInt("NEW_STYLE"));
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetLineJoin(pjs, &tempSelection);
}

void PageItem::restoreLineWidth(SimpleState *state, bool isUndo)
{
	double w = state->getDouble("OLD_LINEWIDTH");
	if (!isUndo)
		w = state->getDouble("NEW_LINEWIDTH");
	Selection tempSelection(nullptr, false);
	tempSelection.addItem(this);
	m_Doc->itemSelection_SetLineWidth(w, &tempSelection);
}

void PageItem::restoreCustomLineStyle(SimpleState *state, bool isUndo)
{
	QString style(state->get("OLD_STYLE"));
	if (!isUndo)
		style = state->get("NEW_STYLE");
	setCustomLineStyle(style);
}

void PageItem::restoreName(SimpleState *state, bool isUndo)
{
	QString name(state->get("OLD_NAME"));
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
	Q_UNUSED(state);
	Q_UNUSED(isUndo);
	//int styleid = state->getInt("OLD_STYLE");
	//if (!isUndo)
	//	styleid = state->getInt("NEW_STYLE");
	//will be done later with other text-undo:
	//	m_Doc->chAbStyle(this, styleid);
}


// FIXME: This must go into class ScribusDoc!
// For now we'll just make it independent of 'this' -- AV
void PageItem::restoreType(SimpleState *state, bool isUndo)
{
	PageItem * item = reinterpret_cast<PageItem *>(state->getVoidPtr("PAGEITEM"));
	int type = state->getInt("OLD_TYPE");
	if (!isUndo)
		type = state->getInt("NEW_TYPE");
	ScribusView* view = m_Doc->view();
	view->deselectItems(false);
	view->selectItem(item, false);
	switch (type)
	{
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
		m_textFlowMode = oldMode;
	else
		m_textFlowMode = newMode;
	
	QList<PageItem*> pList;
	int id = m_Doc->Items->indexOf(this) - 1;
	for (int i = id; i >= 0 ; --i)
		pList << m_Doc->Items->at(i);
		
	QRectF baseRect(getBoundingRect());
	for (int i = 0; i < pList.count(); ++i)
	{
		QRectF uRect(pList.at(i)->getBoundingRect());
		if (baseRect.intersects(uRect))
			pList.at(i)->update();
	}
}

void PageItem::restoreImageScaleMode(SimpleState *state, bool isUndo)
{
	bool type = ScaleType;
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
			state->set("OLD_IMAGEXOFFSET", m_imageXOffset);
			state->set("OLD_IMAGEYOFFSET", m_imageYOffset);
			state->set("OLD_IMAGEXSCALE", m_imageXScale);
			state->set("OLD_IMAGEYSCALE", m_imageYScale);
		}
	}

	bool ratio = AspectRatio;
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
		const auto *is = dynamic_cast<ScItemState<ScImageEffectList>*>(state);
		if (!is)
		{
			qFatal("PageItem::restoreClearImage: dynamic cast failed");
			return;
		}
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


void PageItem::restoreDropLinks(UndoState *state, bool isUndo)
{
	if (!isTextFrame())
		return;
	const auto *is = dynamic_cast<ScItemState<QPair<PageItem*, PageItem*> >*>(state);
	if (!is)
	{
		qFatal("PageItem::restoreDropLinks: dynamic cast failed");
		return;
	}
	if (isUndo)
	{
		PageItem* prev = is->getItem().first;
		PageItem* next = is->getItem().second;

		m_backBox = prev;
		m_nextBox = next;
		invalid = true;

		if (prev)
		{
			this->itemText  = prev->itemText;
			this->isAutoText |= prev->isAutoText;
			prev->m_nextBox = this;
			while (prev)
			{
				prev->invalid = true;
				prev = prev->m_backBox;
			}
		}
		if (next)
		{
			this->itemText = next->itemText;
			this->isAutoText |= next->isAutoText;

			next->m_backBox = this;
			while (next)
			{
				next->invalid = true;
				next = next->m_nextBox;
			}
		}

		// update auto pointers
		if (isAutoText && m_nextBox == nullptr)
		{
			m_Doc->LastAuto = this;
		}
		if (isAutoText && m_backBox == nullptr)
		{
			m_Doc->FirstAuto = this;
		}
	}
	else
	{
		dropLinks();
	}
}

void PageItem::restoreLinkTextFrame(UndoState *state, bool isUndo)
{
	if (!isTextFrame())
		return;

	const auto* is = dynamic_cast<ScItemState<QPair<PageItem*, PageItem*> >*>(state);
	if (!is)
	{
		qFatal("PageItem::restoreLinkTextFrame: dynamic cast failed");
		return;
	}

	if (isUndo)
	{
		unlink(false);
		//restore properly text if frame was linked at beginning of chain
		int joinPos = is->getInt("JOIN_POS");
		int parSep = is->getBool("ADDPARSEP") ? 1 : 0;
		if (is->getBool("FIRST"))
		{
			if (joinPos == 0)
			{
				is->getItem().second->itemText.append(itemText);
				itemText.select(0, itemText.length());
				itemText.removeSelection();
			}
			else
			{
				StoryText content(m_Doc);
				itemText.select(joinPos + parSep, itemText.length() - (joinPos + parSep));
				content.insert(0, itemText, true);
				if (parSep)
					itemText.select(joinPos, itemText.length() - joinPos);
				itemText.removeSelection();
				is->getItem().second->itemText.append(content);
			}
		}
		else
		{
			StoryText content(m_Doc);
			PageItem* prev = is->getItem().second;
			prev->itemText.select(joinPos + parSep, prev->itemText.length() - (joinPos + parSep));
			content.insert(0, prev->itemText, true);
			if (parSep)
				prev->itemText.select(joinPos, prev->itemText.length() - joinPos);
			prev->itemText.removeSelection();
			itemText.append(content);
		}
	}
	else
	{
		asTextFrame()->link(is->getItem().second->asTextFrame());
	}
}

void PageItem::restoreUnlinkTextFrame(UndoState *state, bool isUndo)
{
	if (!isTextFrame())
		return;

	const auto *is = dynamic_cast<ScItemState<QPair<PageItem*, PageItem*> >*>(state);
	if (!is)
	{
		qFatal("PageItem::restoreUnlinkTextFrame: dynamic cast failed");
		return;
	}

	if (is->contains("CUT_TEXT"))
	{
		if (isUndo)
		{
			PageItem* prev = is->getItem().first;
			PageItem* next  = is->getItem().second;
			if (prev != nullptr)
				prev->link(this, false);
			else if (next != nullptr)
				this->link(next, false);
			else
				Q_ASSERT(prev || next);
		}
		else
			unlinkWithText();
	}
	else
	{
		if (isUndo)
			asTextFrame()->link(is->getItem().second->asTextFrame());
		else
			unlink();
	}
}

void PageItem::restoreVerticalAlign(SimpleState *ss, bool isUndo)
{
	if (isUndo)
		verticalAlign = ss->getInt("OLD_VERTALIGN");
	else
		verticalAlign = ss->getInt("NEW_VERTALIGN");
	update();
}

void PageItem::restorePathOperation(UndoState *state, bool isUndo)
{
	//PATH_OPERATION
	const auto *is = dynamic_cast<ScOldNewState< QPair<FPointArray, FPointArray> > *>(state);
	if (!is)
	{
		qFatal("PageItem::restorePathOperation: dynamic cast failed");
		return;
	}

	if (isUndo)
	{
		this->ClipEdited = is->getBool("PATH_OP_OLD_CLIPEDITED");
		this->FrameType = is->getInt("PATH_OP_OLD_FRAMETYPE");
		this->OldB2 = is->getDouble("PATH_OP_OLD_OLDB2");
		this->OldH2 = is->getDouble("PATH_OP_OLD_OLDH2");
		const QPair<FPointArray, FPointArray>& oldLines = is->getOldState();
		this->PoLine = oldLines.first;
		this->ContourLine = oldLines.second;
	}
	else
	{
		this->ClipEdited = is->getBool("PATH_OP_NEW_CLIPEDITED");
		this->FrameType = is->getInt("PATH_OP_NEW_FRAMETYPE");
		this->OldB2 = is->getDouble("PATH_OP_NEW_OLDB2");
		this->OldH2 = is->getDouble("PATH_OP_NEW_OLDH2");
		const QPair<FPointArray, FPointArray>& newLines = is->getNewState();
		this->PoLine = newLines.first;
		this->ContourLine = newLines.second;
	}
	this->updateClip();
}

void PageItem::restorePoly(SimpleState *state, bool isUndo, bool isContour)
{
	int mode = state->getInt("MODE");
	int rot  = state->getInt("ROT");
	ScribusView* view = m_Doc->view();
	double scaling = state->getDouble("SCALING");
	bool editContour = m_Doc->nodeEdit.isContourLine();
	m_Doc->nodeEdit.setIsContourLine(isContour);
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
	m_Doc->nodeEdit.setIsContourLine(editContour);
}

void PageItem::restoreUniteItem(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState< QPair<QList<PageItem*>,QList<QTransform> > >*>(state);
	if (!is)
	{
		qFatal("PageItem::restoreUniteItem: dynamic cast failed");
		return;
	}

	m_Doc->view()->deselectItems(true);
	if (isUndo)
	{
		int pts = 0;
		select();
		for (int i = 0; i < is->getItem().first.size(); ++i)
		{
			PageItem* myItem = is->getItem().first.at(i);
			myItem->PoLine.map(is->getItem().second.at(i).inverted());
			pts += myItem->PoLine.size();
			pts += 4;
			doc()->m_Selection->addItem(myItem);
		}
		PoLine.resize(PoLine.size()-pts);
		Segments.clear();
		FrameType = is->getInt("FRAMETYPE");
		ClipEdited = is->getBool("CLIPEDITED");
		int oldRotMode = doc()->rotationMode();
		doc()->setRotationMode(0);
		doc()->adjustItemSize(this);
		doc()->setRotationMode(oldRotMode);
	}
	else
	{
		select();
		for (int i = 0; i < is->getItem().first.size(); ++i)
			doc()->view()->selectItem(is->getItem().first.at(i));
		doc()->itemSelection_UniteItems();
		select();
	}
}

void PageItem::restoreSplitItem(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState< QList<int> >*>(state);
	if (!is)
	{
		qFatal("PageItem::restoreSplitItem: dynamic cast failed");
		return;
	}
	
	QList<int> itemsList = is->getItem();
	select();
	if (isUndo)
	{
		for (int i = 0; i < itemsList.size(); ++i)
			doc()->view()->selectItem(doc()->Items->at(itemsList.at(i)));
		doc()->itemSelection_UniteItems();
		select();
	}
	else
		doc()->itemSelection_SplitItems();
}

void PageItem::restoreContourLine(SimpleState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<FPointArray>*>(state);
	if (!is)
	{
		qFatal("PageItem::restoreContourLine: dynamic cast failed");
		return;
	}
	if (isUndo)
		ContourLine = is->getItem();
	else
		ContourLine = PoLine.copy();
	ClipEdited = true;
}

void PageItem::restoreShapeType(SimpleState *state, bool isUndo)
{
	// Store shape info in this form:
	// CHANGE_SHAPE_TYPE - ID of the undo operation
	// OLD_FRAME_TYPE - original frame type
	// NEW_FRAME_TYPE - change of frame type
	// binary QPair<FPointArray, FPointArray> - .first original shape, .second new shape
	const auto *is = dynamic_cast<ScOldNewState<FPointArray>*>(state);
	if (!is)
	{
		qFatal("PageItem::restoreShapeType: dynamic cast failed");
		return;
	}

	if (isUndo)
	{
		this->FrameType = is->getInt("OLD_FRAME_TYPE");
		this->PoLine = is->getOldState();
		ClipEdited = !(FrameType == 0 || FrameType == 1);
	}
	else
	{
		this->FrameType = is->getInt("NEW_FRAME_TYPE");
		this->PoLine = is->getNewState();
		ClipEdited = (FrameType == 0 || FrameType == 1);
	}
	Clip = flattenPath(PoLine,Segments);
}

void PageItem::restoreLayer(SimpleState *state, bool isUndo)
{
	ScribusView* view = m_Doc->view();
	setLayer(isUndo ? state->getInt("OLD_LAYER") : state->getInt("NEW_LAYER"));
	view->deselectItems(true);
	m_Doc->regionsChanged()->update(QRectF());
}

void PageItem::restoreGetImage(UndoState *state, bool isUndo)
{
	const auto *is = dynamic_cast<ScItemState<ScImageEffectList>*>(state);
	QString fn = is->get("OLD_IMAGE_PATH");
	if (!isUndo)
		fn = is->get("NEW_IMAGE_PATH");
	if (fn.isEmpty())
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(this, true);
		m_Doc->itemSelection_ClearItem(&tempSelection);
		if (isUndo)
		{
			setImageFlippedH(is->getBool("FLIPPH"));
			setImageFlippedV(is->getBool("FLIPPV"));
			setImageScalingMode(is->getBool("SCALING"), is->getBool("ASPECT"));
		}
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
	const auto *istate = dynamic_cast<ScOldNewState<FPointArray>*>(state);
	if (istate)
	{
		FPointArray oldClip = istate->getOldState();
		FPointArray newClip = istate->getNewState();
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
			m_Doc->adjustItemSize(this);
			m_Doc->moveItem(oldX - xPos(), oldY - yPos(), this);
		}
		else
		{
			if (isContour)
				ContourLine = newClip;
			else
				PoLine = newClip;
			m_Doc->adjustItemSize(this);
			m_Doc->moveItem(newX - xPos(), newY - yPos(), this);
		}
		if (oldClip.count() != newClip.count())
			m_Doc->nodeEdit.deselect();
		m_Doc->regionsChanged()->update(QRectF());
	}
}

void PageItem::restoreImageEffects(UndoState *state, bool isUndo)
{
	const auto *istate = dynamic_cast<ScOldNewState<ScImageEffectList>*>(state);
	if (istate)
	{
		if (isUndo)
			effectsInUse = istate->getOldState();
		else
			effectsInUse = istate->getNewState();

		select();
		m_Doc->updatePic();
	}
}

void PageItem::select()
{
	m_Doc->view()->deselectItems(false);
	//CB #2969 add this true parm to addItem so we don't connectToGUI, the rest of view->SelectItem isn't needed anyway
	m_Doc->m_Selection->addItem(this, true);
}

ObjAttrVector* PageItem::getObjectAttributes()
{
	return &pageItemAttributes;
}

QList<ObjectAttribute> PageItem::getObjectAttributes(const QString& attributeName) const
{
	QList<ObjectAttribute> attributes;
	for (ObjAttrVector::const_iterator objAttrIt = pageItemAttributes.begin() ; objAttrIt != pageItemAttributes.end(); ++objAttrIt)
	{
		if (objAttrIt->name == attributeName)
			attributes.append(*objAttrIt);
	}
	return attributes;
}

ObjectAttribute PageItem::getObjectAttribute(const QString& attributeName) const
{
	int countFound = 0;
	ObjAttrVector::const_iterator foundIt = pageItemAttributes.begin();
	for (auto objAttrIt = pageItemAttributes.begin(); objAttrIt != pageItemAttributes.end(); ++objAttrIt)
	{
		if (objAttrIt->name == attributeName)
		{
			++countFound;
			foundIt = objAttrIt;
		}
	}
	ObjectAttribute returnAttribute;
	if (countFound == 1)
		returnAttribute = (*foundIt);
	else
		returnAttribute.name.clear();
	return returnAttribute;
}


void PageItem::setObjectAttributes(const ObjAttrVector* map)
{
	pageItemAttributes = *map;
}

//if not `prependCopy` then string "Copy of" will not be prepended
QString PageItem::generateUniqueCopyName(const QString& originalName, bool prependCopy) const
{
	if (!m_Doc->itemNameExists(originalName))
		return originalName;

	// Start embellishing the name until we get an acceptable unique name
	// first we prefix `Copy of' if it's not already there
	QString newname(originalName);
	if (prependCopy && !originalName.startsWith( tr("Copy of")))
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

void PageItem::replaceNamedResources(ResourceCollection& newNames)
{
	QMap<QString, QString>::ConstIterator it;
	
	it = newNames.colors().find(softShadowColor());
	if (it != newNames.colors().end())
		setSoftShadowColor(*it);
	
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
	for (int cst = 0; cst < fill_gradient.stops(); ++cst)
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
			MeshPoint mp = meshGradientArray[grow][gcol];
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
	for (int cst = 0; cst < stroke_gradient.stops(); ++cst)
	{
		it = newNames.colors().find(cstops.at(cst)->name);
		if (it != newNames.colors().end())
		{
			if (*it != CommonStrings::None)
				cstops.at(cst)->name = *it;
		}
	}
	cstops = mask_gradient.colorStops();
	for (int cst = 0; cst < mask_gradient.stops(); ++cst)
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
			QString tmpstr2;
			ScTextStream fp(&tmpstr, QIODevice::ReadOnly);
			switch (effectsInUse.at(a).effectCode)
			{
				case ImageEffect::EF_QUADTONE:
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
				case ImageEffect::EF_TRITONE:
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
				case ImageEffect::EF_DUOTONE:
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
				case ImageEffect::EF_COLORIZE:
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

	if (prevInChain() == nullptr)
		itemText.replaceNamedResources(newNames);
}

void PageItem::setGradientType(int val)
{
	if (GrType == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradType, QString(), Um::IFill);
		ss->set("GRAD_TYPE");
		ss->set("OLD", GrType);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrType = val;
}

void PageItem::setStrokeGradientType(int val)
{
	if (GrTypeStroke == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradTypeStroke, QString(), Um::ILineStyle);
		ss->set("GRAD_TYPESTROKE");
		ss->set("OLD", GrTypeStroke);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrTypeStroke = val;
}

void PageItem::setGradientCol1(const QString& val)
{
	if (GrColorP1 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_COL1");
		ss->set("OLD", GrColorP1);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrColorP1 = val;
}

void PageItem::setGradientCol2(const QString& val)
{
	if (GrColorP2 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_COL2");
		ss->set("OLD", GrColorP2);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrColorP2 = val;
}

void PageItem::setGradientCol3(const QString& val)
{
	if (GrColorP3 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_COL3");
		ss->set("OLD", GrColorP3);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrColorP3 = val;
}

void PageItem::setGradientCol4(const QString& val)
{
	if (GrColorP4 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_COL4");
		ss->set("OLD", GrColorP4);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrColorP4 = val;
}

void PageItem::setGradientShade1(int val)
{
	if (GrCol1Shade == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_SHADE1");
		ss->set("OLD", GrCol1Shade);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol1Shade = val;
}

void PageItem::setGradientShade2(int val)
{
	if (GrCol2Shade == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_SHADE2");
		ss->set("OLD", GrCol2Shade);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol2Shade = val;
}

void PageItem::setGradientShade3(int val)
{
	if (GrCol3Shade == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_SHADE3");
		ss->set("OLD", GrCol3Shade);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol3Shade = val;
}

void PageItem::setGradientShade4(int val)
{
	if (GrCol4Shade == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_SHADE4");
		ss->set("OLD", GrCol4Shade);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol4Shade = val;
}

void PageItem::setGradientTransp1(double val)
{
	if (GrCol1transp == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_TRANSP1");
		ss->set("OLD", GrCol1transp);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol1transp = val;
}

void PageItem::setGradientTransp2(double val)
{
	if (GrCol2transp == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_TRANSP2");
		ss->set("OLD", GrCol2transp);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol2transp = val;
}

void PageItem::setGradientTransp3(double val)
{
	if (GrCol3transp == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_TRANSP3");
		ss->set("OLD", GrCol3transp);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol3transp = val;
}

void PageItem::setGradientTransp4(double val)
{
	if (GrCol4transp == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_TRANSP4");
		ss->set("OLD", GrCol4transp);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrCol4transp = val;
}

void PageItem::setGradientColor1(const QColor& val)
{
	if (m_grQColorP1 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<QColor>(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_QCOLOR1");
		ss->setStates(m_grQColorP1, val);
		undoManager->action(this, ss);
	}
	m_grQColorP1 = val;
}

void PageItem::setGradientColor2(const QColor& val)
{
	if (m_grQColorP2 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<QColor>(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_QCOLOR2");
		ss->setStates(m_grQColorP2, val);
		undoManager->action(this, ss);
	}
	m_grQColorP2 = val;
}

void PageItem::setGradientColor3(const QColor& val)
{
	if (m_grQColorP3 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<QColor>(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_QCOLOR3");
		ss->setStates(m_grQColorP3, val);
		undoManager->action(this, ss);
	}
	m_grQColorP3 = val;
}

void PageItem::setGradientColor4(const QColor& val)
{
	if (m_grQColorP4 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<QColor>(Um::GradCol, QString(), Um::IFill);
		ss->set("GRAD_QCOLOR4");
		ss->setStates(m_grQColorP4, val);
		undoManager->action(this, ss);
	}
	m_grQColorP4 = val;
}

void PageItem::setGradientExtend(VGradient::VGradientRepeatMethod val)
{
	if (GrExtend == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* ss = new ScOldNewState<VGradient::VGradientRepeatMethod>(Um::GradExtent, QString(), Um::IFill);
		ss->set("GRAD_EXTENT");
		ss->setStates(GrExtend, val);
		undoManager->action(this, ss);
	}
	GrExtend = val;
}

void PageItem::setStrokeGradientExtend(VGradient::VGradientRepeatMethod val)
{
	if (GrStrokeExtend == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto* ss = new ScOldNewState<VGradient::VGradientRepeatMethod>(Um::GradExtentStroke, QString(), Um::ILine);
		ss->set("GRADSTROKE_EXTENT");
		ss->setStates(GrStrokeExtend, val);
		undoManager->action(this, ss);
	}
	GrStrokeExtend = val;
}

VGradient::VGradientRepeatMethod PageItem::getGradientExtend() const
{
	return GrExtend;
}

VGradient::VGradientRepeatMethod PageItem::getStrokeGradientExtend() const
{
	return GrStrokeExtend;
}


void PageItem::setSnapToPatchGrid(bool val)
{
	if (snapToPatchGrid == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("SNAP_TO_PATCH");
		ss->set("OLD", val);
		undoManager->action(this, ss);
	}
	snapToPatchGrid = val;
}

void PageItem::setGradientStart(double x, double y)
{
	if ((GrStartX == x) && (GrStartY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_START");
		ss->set("OLDX", GrStartX);
		ss->set("OLDY", GrStartY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrStartX = x;
	GrStartY = y;
}

void PageItem::setGradientEnd(double x, double y)
{
	if ((GrEndX == x) && (GrEndY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_END");
		ss->set("OLDX", GrEndX);
		ss->set("OLDY", GrEndY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrEndX = x;
	GrEndY = y;
}

void PageItem::setGradientFocal(double x, double y)
{
	if ((GrFocalX == x) && (GrFocalY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_FOCAL");
		ss->set("OLDX", GrFocalX);
		ss->set("OLDY", GrFocalY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrFocalX = x;
	GrFocalY = y;
}

void PageItem::setGradientScale(double val)
{
	if (GrScale == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_SCALE");
		ss->set("OLD", GrScale);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrScale = val;
}

void PageItem::setGradientSkew(double val)
{
	if (GrSkew == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_SKEW");
		ss->set("OLD", GrSkew);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrSkew = val;
}

void PageItem::setGradientMaskStart(double x, double y)
{
	if ((GrMaskStartX == x) && (GrMaskStartY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_MASKSTART");
		ss->set("OLDX", GrMaskStartX);
		ss->set("OLDY", GrMaskStartY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrMaskStartX = x;
	GrMaskStartY = y;
}

void PageItem::setGradientMaskEnd(double x, double y)
{
	if ((GrMaskEndX == x) && (GrMaskEndY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_MASKEND");
		ss->set("OLDX", GrMaskEndX);
		ss->set("OLDY", GrMaskEndY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrMaskEndX = x;
	GrMaskEndY = y;
}

void PageItem::setGradientMaskFocal(double x, double y)
{
	if ((GrMaskFocalX == x) && (GrMaskFocalY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_MASKFOCAL");
		ss->set("OLDX", GrMaskFocalX);
		ss->set("OLDY", GrMaskFocalY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrMaskFocalX = x;
	GrMaskFocalY = y;
}

void PageItem::setGradientMaskScale(double val)
{
	if (GrMaskScale == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_MASKSCALE");
		ss->set("OLD", GrMaskScale);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrMaskScale = val;
}

void PageItem::setGradientMaskSkew(double val)
{
	if (GrMaskSkew == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_MASKSKEW");
		ss->set("OLD", GrMaskSkew);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrMaskSkew = val;
}

void PageItem::setGradientControl1(const FPoint& val)
{
	if (GrControl1 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<FPoint>(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_CONTROL1");
		ss->setStates(GrControl1, val);
		undoManager->action(this, ss);
	}
	GrControl1 = val;
}

void PageItem::setGradientControl2(const FPoint& val)
{
	if (GrControl2 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<FPoint>(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_CONTROL2");
		ss->setStates(GrControl2, val);
		undoManager->action(this, ss);
	}
	GrControl2 = val;
}

void PageItem::setGradientControl3(const FPoint& val)
{
	if (GrControl3 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<FPoint>(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_CONTROL3");
		ss->setStates(GrControl3, val);
		undoManager->action(this, ss);
	}
	GrControl3 = val;
}

void PageItem::setGradientControl4(const FPoint& val)
{
	if (GrControl4 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<FPoint>(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_CONTROL4");
		ss->setStates(GrControl4, val);
		undoManager->action(this, ss);
	}
	GrControl4 = val;
}

void PageItem::setGradientControl5(const FPoint& val)
{
	if (GrControl5 == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<FPoint>(Um::GradPos, QString(), Um::IFill);
		ss->set("GRAD_CONTROL5");
		ss->setStates(GrControl5, val);
		undoManager->action(this, ss);
	}
	GrControl5 = val;
}

void PageItem::setGradientStrokeScale(double val)
{
	if (GrStrokeScale == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::ILine);
		ss->set("GRADSTROKE_SCALE");
		ss->set("OLD", GrStrokeScale);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrStrokeScale = val;
}

void PageItem::setGradientStrokeSkew(double val)
{
	if (GrStrokeSkew == val)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::ILine);
		ss->set("GRADSTROKE_SKEW");
		ss->set("OLD", GrStrokeSkew);
		ss->set("NEW", val);
		undoManager->action(this, ss);
	}
	GrStrokeSkew = val;
}

void PageItem::setGradientStrokeFocal(double x, double y)
{
	if ((GrStrokeFocalX == x) && (GrStrokeFocalY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::ILine);
		ss->set("GRADSTROKE_FOCAL");
		ss->set("OLDX", GrStrokeFocalX);
		ss->set("OLDY", GrStrokeFocalY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrStrokeFocalX = x;
	GrStrokeFocalY = y;
}

void PageItem::setGradientStrokeStart(double x, double y)
{
	if ((GrStrokeStartX == x) && (GrStrokeStartY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::ILine);
		ss->set("GRADSTROKE_START");
		ss->set("OLDX", GrStrokeStartX);
		ss->set("OLDY", GrStrokeStartY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrStrokeStartX = x;
	GrStrokeStartY = y;
}

void PageItem::setGradientStrokeEnd(double x, double y)
{
	if ((GrStrokeEndX == x) && (GrStrokeEndY == y))
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::GradPos, QString(), Um::ILine);
		ss->set("GRADSTROKE_END");
		ss->set("OLDX", GrStrokeEndX);
		ss->set("OLDY", GrStrokeEndY);
		ss->set("NEWX", x);
		ss->set("NEWY", y);
		undoManager->action(this, ss);
	}
	GrStrokeEndX = x;
	GrStrokeEndY = y;
}

void PageItem::getNamedResources(ResourceCollection& lists) const
{
	if (hasSoftShadow())
		lists.collectColor(softShadowColor());

	if (GrType == 0)
		lists.collectColor(fillColor());
	else if ((GrType < Gradient_Pattern) || (GrType == Gradient_Diamond))
	{
		if ((!m_gradientName.isEmpty()) && (m_Doc->docGradients.contains(m_gradientName)))
			lists.collectGradient(m_gradientName);
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		for (int cst = 0; cst < fill_gradient.stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
	else if (GrType == Gradient_4Colors)
	{
		lists.collectColor(GrColorP1);
		lists.collectColor(GrColorP2);
		lists.collectColor(GrColorP3);
		lists.collectColor(GrColorP4);
	}
	else if (GrType == Gradient_Mesh)
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
	else if (GrType == Gradient_PatchMesh)
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
	else if (GrTypeStroke < Gradient_Pattern)
	{
		if ((!gradientStrokeVal.isEmpty()) && (m_Doc->docGradients.contains(gradientStrokeVal)))
			lists.collectGradient(gradientStrokeVal);
		QList<VColorStop*> cstops = stroke_gradient.colorStops();
		for (int cst = 0; cst < stroke_gradient.stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
	if ((GrMask == GradMask_Linear) || (GrMask == GradMask_Radial) || (GrMask == GradMask_LinearLumAlpha) || (GrMask == GradMask_RadialLumAlpha))
	{
		QList<VColorStop*> cstops = mask_gradient.colorStops();
		for (int cst = 0; cst < mask_gradient.stops(); ++cst)
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
				case ImageEffect::EF_QUADTONE:
					lists.collectColor(fp.readLine());
				case ImageEffect::EF_TRITONE:
					lists.collectColor(fp.readLine());
				case ImageEffect::EF_DUOTONE:
					lists.collectColor(fp.readLine());
				case ImageEffect::EF_COLORIZE:
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
	if (prevInChain() == nullptr)
		itemText.getNamedResources(lists);
}

//Moved from View
void PageItem::SetFrameShape(int count, const double *vals)
{
	PoLine.resize(0);
	for (int i = 0; i < count - 3; i += 4)
	{
		if (vals[i] < 0)
		{
			PoLine.setMarker();
			continue;
		}
		double x1 = m_width * vals[i] / 100.0;
		double y1 = m_height * vals[i + 1] / 100.0;
		double x2 = m_width * vals[i + 2] / 100.0;
		double y2 = m_height * vals[i + 3] / 100.0;
		PoLine.addPoint(x1, y1);
		PoLine.addPoint(x2, y2);
	}
	Clip = flattenPath(PoLine, Segments);
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
	setCornerRadius(qMin(m_roundedCornerRadius, qMin(m_width, m_height)/2));
	PoLine.resize(0);
	double rr = fabs(m_roundedCornerRadius);
	if (m_roundedCornerRadius > 0.0)
	{
		QPainterPath path;
		path.addRoundedRect(0, 0, m_width, m_height, rr, rr);
		PoLine.fromQPainterPath(path);
	}
	else
	{
		double bezierFactor = 0.552284749; //Bezier Control Point Factor: 8/3*(sin(45)-0.5)
		double rrxBezierFactor = rr*bezierFactor;
		double Width_rr = m_width-rr;
		double Height_rr = m_height-rr;
		PoLine.addQuadPoint(rr, 0, rr, 0, Width_rr, 0, Width_rr, 0);
		PoLine.addQuadPoint(Width_rr, 0, Width_rr, rrxBezierFactor, m_width, rr, m_width-rrxBezierFactor, rr);
		PoLine.addQuadPoint(m_width, rr, m_width, rr, m_width, Height_rr, m_width, Height_rr);
		PoLine.addQuadPoint(m_width, Height_rr, m_width-rrxBezierFactor, Height_rr, Width_rr, m_height, Width_rr, m_height-rrxBezierFactor);
		PoLine.addQuadPoint(Width_rr, m_height, Width_rr, m_height, rr, m_height, rr, m_height);
		PoLine.addQuadPoint(rr, m_height, rr, m_height-rrxBezierFactor, 0, Height_rr, rrxBezierFactor, Height_rr);
		PoLine.addQuadPoint(0, Height_rr, 0, Height_rr, 0, rr, 0, rr);
		PoLine.addQuadPoint(0, rr, rrxBezierFactor, rr, rr, 0, rr, rr*bezierFactor);
	}
	Clip = flattenPath(PoLine, Segments);
	ClipEdited = false;
	FrameType = 2;
}

QTransform PageItem::getGroupTransform() const
{
	QTransform result;
	result.translate(gXpos, gYpos);
	result.rotate(m_rotation);
	return result;
}

void PageItem::getTransform(QTransform& mat) const
{
	mat.translate(m_xPos, m_yPos);
	mat.rotate(m_rotation);
}

QTransform PageItem::getTransform() const
{
	QTransform result;
	if (isGroupChild())
	{
		QList<const PageItem*> itList;
		const PageItem* ite = this;
		while (ite->isGroupChild())
		{
			itList.prepend(ite);
			ite = ite->Parent;
		}
		result.translate(ite->xPos(), ite->yPos());
		result.rotate(ite->rotation());
	/*	if (ite->isGroup() || ite->isSymbol())
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
		}*/
		if (ite == this)
			return result;
		if (ite->isGroup())
			result.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
		for (int aa = 0; aa < itList.count(); aa++)
		{
			ite = itList.at(aa);
			result.translate(ite->gXpos, ite->gYpos);
			result.rotate(ite->rotation());
		/*	if (ite->isGroup() || ite->isSymbol())
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
			}*/
			if (ite == this)
				return result;
			if (ite->isGroup())
				result.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
		}
	}
	else
	{
		result.translate(m_xPos, m_yPos);
		result.rotate(m_rotation);
	}
	return result;
}

QTransform PageItem::getTransform(double deltaX, double deltaY) const
{
	QTransform result;
	if (isGroupChild())
	{
		QList<const PageItem*> itList;
		const PageItem* ite = this;
		while (ite->isGroupChild())
		{
			itList.prepend(ite);
			ite = ite->Parent;
		}
		result.translate(ite->xPos(), ite->yPos());
		result.rotate(ite->rotation());
	/*	if (ite->isGroup() || ite->isSymbol())
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
		}*/
		if (ite == this)
			return result;
		if (ite->isGroup())
			result.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
		for (int i = 0; i < itList.count(); i++)
		{
			ite = itList.at(i);
			result.translate(ite->gXpos, ite->gYpos);
			if (ite == this)
				result.translate(deltaX, deltaY);
			result.rotate(ite->rotation());
		/*	if (ite->isGroup() || ite->isSymbol())
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
			}*/
			if (ite == this)
				return result;
			if (ite->isGroup())
				result.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
		}
	}
	else
	{
		result.translate(m_xPos + deltaX, m_yPos + deltaY);
		result.rotate(m_rotation);
	}
	return result;
}

QRectF PageItem::getBoundingRect() const
{
	double x, y, x2, y2;
	getBoundingRect(&x, &y, &x2, &y2);
	return QRectF(x, y, x2 - x, y2 - y);
}

QRectF PageItem::getCurrentBoundingRect(double moreSpace) const
{
	double x = BoundingX - moreSpace / 2.0;
	double y = BoundingY - moreSpace / 2.0;
	double w = BoundingW + moreSpace;
	double h = BoundingH + moreSpace;
	QRectF ret(x, y, w, h);
	return ret;
}

QRectF PageItem::getOldBoundingRect() const
{
	double x, y, x2, y2;
	getOldBoundingRect(&x, &y, &x2, &y2);
	return QRectF(x, y, x2 - x, y2 - y);
}

QRectF PageItem::getVisualBoundingRect() const
{
	double x, y, x2, y2;
	getVisualBoundingRect(&x, &y, &x2, &y2);
	return QRectF(x, y, x2 - x, y2 - y);
}

void PageItem::getBoundingRect(double *x1, double *y1, double *x2, double *y2) const
{
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	if (m_rotation != 0)
	{
		FPointArray pb;
		pb.addPoint(FPoint(m_xPos, m_yPos));
		pb.addPoint(FPoint(m_width,    0.0, m_xPos, m_yPos, m_rotation, 1.0, 1.0));
		pb.addPoint(FPoint(m_width, m_height, m_xPos, m_yPos, m_rotation, 1.0, 1.0));
		pb.addPoint(FPoint(  0.0, m_height, m_xPos, m_yPos, m_rotation, 1.0, 1.0));
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
		*x1 = m_xPos;
		*y1 = m_yPos;
		*x2 = m_xPos + qMax(1.0, qMax(m_width, m_lineWidth));
		*y2 = m_yPos + qMax(1.0, qMax(m_height, m_lineWidth));
	}
}

void PageItem::getOldBoundingRect(double *x1, double *y1, double *x2, double *y2) const
{
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	if (oldRot != 0)
	{
		FPointArray pb;
		pb.addPoint(FPoint(oldXpos, oldYpos));
		pb.addPoint(FPoint(oldWidth,    0.0, oldXpos, oldYpos, oldRot, 1.0, 1.0));
		pb.addPoint(FPoint(oldWidth, oldHeight, oldXpos, oldYpos, oldRot, 1.0, 1.0));
		pb.addPoint(FPoint(  0.0, oldHeight, oldXpos, oldYpos, oldRot, 1.0, 1.0));
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
		*x1 = oldXpos;
		*y1 = oldYpos;
		*x2 = oldXpos + qMax(1.0, qMax(oldWidth, m_lineWidth));
		*y2 = oldYpos + qMax(1.0, qMax(oldHeight, m_lineWidth));
	}
}

void PageItem::getVisualBoundingRect(double * x1, double * y1, double * x2, double * y2) const
{
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	double extraSpace = visualLineWidth() / 2.0;
	if (m_rotation != 0)
	{
		FPointArray pb;
		pb.addPoint(FPoint(-extraSpace,					-extraSpace,				xPos(), yPos(), m_rotation, 1.0, 1.0));
		pb.addPoint(FPoint(visualWidth()-extraSpace,	-extraSpace,				xPos(), yPos(), m_rotation, 1.0, 1.0));
		pb.addPoint(FPoint(visualWidth()-extraSpace,	visualHeight()-extraSpace,	xPos(), yPos(), m_rotation, 1.0, 1.0));
		pb.addPoint(FPoint(-extraSpace, 				visualHeight()-extraSpace,	xPos(), yPos(), m_rotation, 1.0, 1.0));
		for (int pc = 0; pc < 4; ++pc)
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
}

double PageItem::visualXPos() const
{
	double extraSpace = visualLineWidth() / 2.0;
	return m_xPos - extraSpace;
}

double PageItem::visualYPos() const
{
	double extraSpace = visualLineWidth() / 2.0;
	return m_yPos - extraSpace;
}

double PageItem::visualWidth() const
{
	double extraSpace = visualLineWidth();
	return m_width + extraSpace;
}

double PageItem::visualHeight() const
{
	double extraSpace = visualLineWidth();
	return m_height + extraSpace;
}

QSizeF PageItem::visualSize() const
{
	double extraSpace = visualLineWidth();
	return QSizeF(m_width + extraSpace, m_height + extraSpace);
}

double PageItem::visualLineWidth() const
{
	double extraSpace = 0.0;
	if (NamedLStyle.isEmpty())
	{
		if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
		{
			extraSpace = m_lineWidth;
			if ((extraSpace == 0.0) && m_Doc->view()) // Hairline case
				extraSpace = 1.0 / m_Doc->view()->scale();
		}
		if (patternStrokePath && !patternStrokeVal.isEmpty() && m_Doc->docPatterns.contains(patternStrokeVal))
		{
			const ScPattern *pat = &m_Doc->docPatterns[patternStrokeVal];
			QTransform mat;
			mat.rotate(patternStrokeTransfrm.rotation);
			mat.scale(patternStrokeTransfrm.scaleX, patternStrokeTransfrm.scaleY);
			QRectF p1R(0, 0, pat->width, pat->height);
			QRectF p2R = mat.map(p1R).boundingRect();
			extraSpace = p2R.height();
		}
	}
	else
	{
		multiLine ml = m_Doc->docLineStyles[NamedLStyle];
		const SingleLine& sl = ml.last();
		if (sl.Color != CommonStrings::None)
		{
			extraSpace = sl.Width;
			if ((extraSpace == 0.0) && m_Doc->view()) // Hairline case
				extraSpace = 1.0 / m_Doc->view()->scale();
		}
	}
	return extraSpace;
}

QRectF PageItem::getStartArrowBoundingRect() const
{
	QRectF arrowRect;
	if (m_startArrowIndex != 0 && !PoLine.empty())
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_startArrowIndex - 1).points.copy();
		arrowTrans.translate(m_xPos, m_yPos);
		arrowTrans.rotate(m_rotation);
		const FPoint& Start = PoLine.point(0);
		for (int xx = 1; xx < PoLine.size(); xx += 2)
		{
			const FPoint& Vector = PoLine.point(xx);
			if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
			{
				arrowTrans.translate(Start.x(), Start.y());
				arrowTrans.rotate(atan2(Start.y() - Vector.y(), Start.x() - Vector.x()) * (180.0 / M_PI));
				arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
				if (NamedLStyle.isEmpty())
				{
					if (m_lineWidth != 0.0)
						arrowTrans.scale(m_lineWidth, m_lineWidth);
				}
				else
				{
					const multiLine ml = m_Doc->docLineStyles[NamedLStyle];
					const SingleLine& sl = ml.last();
					if (sl.Width != 0.0)
						arrowTrans.scale(sl.Width, sl.Width);
				}
				arrow.map(arrowTrans);
				break;
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		arrowRect = QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y()));
	}

	return arrowRect;
}

QRectF PageItem::getStartArrowOldBoundingRect() const
{
	QRectF arrowRect;
	if (m_startArrowIndex != 0 && !PoLine.empty())
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_startArrowIndex - 1).points.copy();
		arrowTrans.translate(oldXpos, oldYpos);
		arrowTrans.rotate(oldRot);
		FPoint Start = PoLine.point(0);
		for (int xx = 1; xx < PoLine.size(); xx += 2)
		{
			FPoint Vector = PoLine.point(xx);
			if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
			{
				arrowTrans.translate(Start.x(), Start.y());
				arrowTrans.rotate(atan2(Start.y() - Vector.y(), Start.x() - Vector.x()) * (180.0 / M_PI));
				arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
				if (NamedLStyle.isEmpty())
				{
					if (m_oldLineWidth != 0.0)
						arrowTrans.scale(m_oldLineWidth, m_oldLineWidth);
				}
				else
				{
					const multiLine ml = m_Doc->docLineStyles[NamedLStyle];
					const SingleLine& sl = ml.last();
					if (sl.Width != 0.0)
						arrowTrans.scale(sl.Width, sl.Width);
				}
				arrow.map(arrowTrans);
				break;
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		arrowRect = QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y()));
	}

	return arrowRect;
}

QRectF PageItem::getEndArrowBoundingRect() const
{
	QRectF arrowRect;
	if (m_endArrowIndex != 0 && PoLine.size() >= 2)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_endArrowIndex - 1).points.copy();
		arrowTrans.translate(m_xPos, m_yPos);
		arrowTrans.rotate(m_rotation);
		FPoint End = PoLine.point(PoLine.size() - 2);
		for (uint xx = PoLine.size() - 1; xx > 0; xx -= 2)
		{
			FPoint Vector = PoLine.point(xx);
			if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
			{
				arrowTrans.translate(End.x(), End.y());
				arrowTrans.rotate(atan2(End.y() - Vector.y(), End.x() - Vector.x()) * (180.0 / M_PI));
				arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
				if (NamedLStyle.isEmpty())
				{
					if (m_lineWidth != 0.0)
						arrowTrans.scale(m_lineWidth, m_lineWidth);
				}
				else
				{
					const multiLine ml = m_Doc->docLineStyles[NamedLStyle];
					const SingleLine& sl = ml.last();
					if (sl.Width != 0.0)
						arrowTrans.scale(sl.Width, sl.Width);
				}
				arrow.map(arrowTrans);
				break;
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		arrowRect = QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y()));
	}

	return arrowRect;
}

QRectF PageItem::getEndArrowOldBoundingRect() const
{
	QRectF arrowRect;
	if (m_endArrowIndex != 0 && PoLine.size() >= 2)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(m_endArrowIndex - 1).points.copy();
		arrowTrans.translate(oldXpos, oldYpos);
		arrowTrans.rotate(oldRot);
		FPoint End = PoLine.point(PoLine.size() - 2);
		for (uint xx = PoLine.size()-1; xx > 0; xx -= 2)
		{
			FPoint Vector = PoLine.point(xx);
			if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
			{
				arrowTrans.translate(End.x(), End.y());
				arrowTrans.rotate(atan2(End.y() - Vector.y(), End.x() - Vector.x()) * (180.0 / M_PI));
				arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
				if (NamedLStyle.isEmpty())
				{
					if (m_oldLineWidth != 0.0)
						arrowTrans.scale(m_oldLineWidth, m_oldLineWidth);
				}
				else
				{
					const multiLine ml = m_Doc->docLineStyles[NamedLStyle];
					const SingleLine& sl = ml.last();
					if (sl.Width != 0.0)
						arrowTrans.scale(sl.Width, sl.Width);
				}
				arrow.map(arrowTrans);
				break;
			}
		}
		FPoint minAr = getMinClipF(&arrow);
		FPoint maxAr = getMaxClipF(&arrow);
		arrowRect = QRectF(QPointF(minAr.x(), minAr.y()), QPointF(maxAr.x(), maxAr.y()));
	}

	return arrowRect;
}

QRegion PageItem::textInteractionRegion(double xOffset, double yOffset) const
{
	QRegion res;
	if (m_textFlowMode == TextFlowDisabled)
		return res;

	QTransform pp;
	if (this->isGroupChild())
		pp.translate(gXpos, gYpos);
	else
		pp.translate(m_xPos - xOffset, m_yPos - yOffset);
	pp.rotate(m_rotation);

	if (textFlowUsesBoundingBox())
	{
		QRectF bb = getVisualBoundingRect();
		if (isGroupChild())
		{
			bb.translate(-m_xPos, -m_yPos);
			bb.translate(gXpos, gYpos);
		}
		res = QRegion(bb.toRect());
	}
	else if ((textFlowUsesImageClipping()) && (!imageClip.empty()))
	{
		QList<uint> Segs;
		QPolygon Clip2 = flattenPath(imageClip, Segs);
		res = QRegion(pp.map(Clip2)).intersected(QRegion(pp.map(Clip)));
	}
	else if ((textFlowUsesContourLine()) && (!ContourLine.empty()))
	{
		QList<uint> Segs;
		QPolygon Clip2 = flattenPath(ContourLine, Segs);
		res = QRegion(pp.map(Clip2));
	}
	else
	{
		if (isSymbol() || isGroup())
		{
			if (imageFlippedH())
			{
				pp.translate(m_width, 0);
				pp.scale(-1, 1);
			}
			if (imageFlippedV())
			{
				pp.translate(0, m_height);
				pp.scale(1, -1);
			}
		}
		if ((((m_lineColor != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0)) && (m_lineWidth > 1)) || (!NamedLStyle.isEmpty()))
		{
			QPainterPath ppa;
			QPainterPath result;
			if (itemType() == PageItem::PolyLine)
				ppa = PoLine.toQPainterPath(false);
			else
				ppa = PoLine.toQPainterPath(true);
			if (NamedLStyle.isEmpty())
			{
				QPainterPathStroker stroke;
				stroke.setCapStyle(PLineEnd);
				stroke.setJoinStyle(PLineJoin);
				stroke.setDashPattern(Qt::SolidLine);
				stroke.setWidth(m_lineWidth);
				result = stroke.createStroke(ppa);
			}
			else
			{
				multiLine ml = doc()->docLineStyles[NamedLStyle];
				int ind = ml.size()-1;
				if ((ml[ind].Color != CommonStrings::None) && (ml[ind].Width != 0))
				{
					QPainterPathStroker stroke;
					stroke.setCapStyle(static_cast<Qt::PenCapStyle>(ml[ind].LineEnd));
					stroke.setJoinStyle(static_cast<Qt::PenJoinStyle>(ml[ind].LineJoin));
					stroke.setDashPattern(Qt::SolidLine);
					stroke.setWidth(ml[ind].Width);
					result = stroke.createStroke(ppa);
				}
			}
			res = QRegion(pp.map(Clip));
			QList<QPolygonF> pl = result.toSubpathPolygons();
			for (int b = 0; b < pl.count(); b++)
			{
				res = res.united(QRegion(pp.map(pl[b].toPolygon())));
			}
		}
		else
			res = QRegion(pp.map(Clip));
	}
	return  res;
}

bool PageItem::pointWithinItem(int x, const int y) const
{
	const_cast<PageItem*>(this)->setRedrawBounding();
	// FIXME: We should be rounding or truncating here, not letting the compiler do it.
	// Should we be rounding, truncating up, or truncating down?
	//CB is this now correct?
	QRect itemRect( static_cast<int>(floor(BoundingX)), static_cast<int>(floor(BoundingY)), static_cast<int>(ceil(BoundingW)), static_cast<int>(ceil(BoundingH)) );
	return itemRect.contains(x, y);
}

bool PageItem::mouseWithinItem(int x, int y, double scale) const
{
	QTransform p;
	p.translate(m_xPos*scale, m_yPos*scale);
	p.scale(scale, scale);
	p.rotate(rotation());
	QRectF transRect(p.mapRect(QRectF(0.0, 0.0, width(), height())));
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
	bool useImage = (asImageFrame() != nullptr);
	useImage |= (isAnnotation() && annotation().UseIcons());
	if (!useImage)
		return false;
	QFileInfo fi(filename);
	QString clPath(pixm.imgInfo.usedPath);
	pixm.imgInfo.valid = false;
	pixm.imgInfo.clipPath.clear();
	pixm.imgInfo.PDSpathData.clear();
	pixm.imgInfo.layerInfo.clear();
	pixm.imgInfo.usedPath.clear();
	imageClip.resize(0);
	int lowResTypeBack = pixm.imgInfo.lowResType;
	int gsRes = gsResolution;
	if (gsResolution == -1) //If it wasn't supplied, get it from PrefsManager.
		gsRes = PrefsManager::instance().gsResolution();
	bool dummy;

	CMSettings cms(m_Doc, ImageProfile, ImageIntent);
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
		imageIsAvailable = false;
		return false;
	}

	QString ext = fi.suffix().toLower();
	if (UndoManager::undoEnabled() && !reload)
	{
		auto *is = new ScItemState<ScImageEffectList>(Um::GetImage, filename, Um::IGetImage);
		is->set("GET_IMAGE");
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
	imageIsAvailable = true;
		
	if (Pfile != filename)
	{
		oldLocalScX = m_imageXScale = 72.0 / xres;
		oldLocalScY = m_imageYScale = 72.0 / yres;
		oldLocalX = m_imageXOffset = 0;
		oldLocalY = m_imageYOffset = 0;
		if ((m_Doc->itemToolPrefs().imageUseEmbeddedPath) && (!pixm.imgInfo.clipPath.isEmpty()))
		{
			pixm.imgInfo.usedPath = pixm.imgInfo.clipPath;
			clPath = pixm.imgInfo.clipPath;
			if (pixm.imgInfo.PDSpathData.contains(clPath))
			{
				imageClip = pixm.imgInfo.PDSpathData[clPath].copy();
				pixm.imgInfo.usedPath = clPath;
				QTransform cl;
				cl.translate(m_imageXOffset*m_imageXScale, m_imageYOffset*m_imageYScale);
				cl.scale(m_imageXScale, m_imageYScale);
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
		cl.translate(m_imageXOffset*m_imageXScale, m_imageYOffset*m_imageYScale);
		cl.scale(m_imageXScale, m_imageYScale);
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

	UseEmbedded = pixm.imgInfo.isEmbedded;
	if (pixm.imgInfo.isEmbedded)
		ImageProfile = "Embedded " + pixm.imgInfo.profileName;
	else
		ImageProfile = pixm.imgInfo.profileName;
	if (!pixm.imgInfo.embeddedProfileName.isEmpty())
		EmbeddedProfile = "Embedded " + pixm.imgInfo.embeddedProfileName;
	else
		EmbeddedProfile.clear();

	adjustPictScale();

	// #12408 : we set the old* variables to avoid creation of unwanted undo states
	// when user perform actions such as double clicking image. We might want to
	// create an undo transaction in this function if this does not work properly.
	oldLocalScX = m_imageXScale;
	oldLocalScY = m_imageYScale;

	if (imageIsAvailable && !fromCache)
	{
		if ((pixm.imgInfo.colorspace == ColorSpaceDuotone) && (pixm.imgInfo.duotoneColors.count() != 0) && (!reload))
		{
			QString efVal;
			for (int cc = 0; cc < pixm.imgInfo.duotoneColors.count(); cc++)
			{
				if (!m_Doc->PageColors.contains(pixm.imgInfo.duotoneColors[cc].Name))
					m_Doc->PageColors.insert(pixm.imgInfo.duotoneColors[cc].Name, pixm.imgInfo.duotoneColors[cc].Color);
				efVal += pixm.imgInfo.duotoneColors[cc].Name + "\n";
			}
			m_Doc->scMW()->propertiesPalette->updateColorList();
			m_Doc->scMW()->contentPalette->updateColorList();
			struct ImageEffect ef;
			if (pixm.imgInfo.duotoneColors.count() == 1)
			{
				efVal += "100";
				ef.effectCode = ImageEffect::EF_COLORIZE;
				ef.effectParameters = efVal;
			}
			else if (pixm.imgInfo.duotoneColors.count() == 2)
			{
				efVal += "100 100";
				QString tmp;
				FPointArray Vals = pixm.imgInfo.duotoneColors[0].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[1].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				ef.effectCode = ImageEffect::EF_DUOTONE;
				ef.effectParameters = efVal;
			}
			else if (pixm.imgInfo.duotoneColors.count() == 3)
			{
				efVal += "100 100 100";
				QString tmp;
				FPointArray Vals = pixm.imgInfo.duotoneColors[0].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[1].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[2].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				ef.effectCode = ImageEffect::EF_TRITONE;
				ef.effectParameters = efVal;
			}
			else if (pixm.imgInfo.duotoneColors.count() == 4)
			{
				efVal += "100 100 100 100";
				QString tmp;
				FPointArray Vals = pixm.imgInfo.duotoneColors[0].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[1].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[2].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				Vals = pixm.imgInfo.duotoneColors[3].Curve;
				tmp.setNum(Vals.size());
				efVal += " "+tmp;
				for (int p = 0; p < Vals.size(); p++)
				{
					const FPoint& pv = Vals.point(p);
					efVal += QString(" %1 %2").arg(pv.x()).arg(pv.y());
				}
				efVal += " 0";
				ef.effectCode = ImageEffect::EF_QUADTONE;
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
	if (imageIsAvailable && m_Doc->viewAsPreview)
	{
		VisionDefectColor defect;
		QColor tmpC;
		int h = pixm.qImagePtr()->height();
		int w = pixm.qImagePtr()->width();
		int r, g, b, a;
		QRgb *s;
		QRgb rgb;
		for (int yi=0; yi < h; ++yi)
		{
			s = (QRgb*)(pixm.qImagePtr()->scanLine( yi ));
			for (int xi = 0; xi < w; ++xi)
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
	return true;
}


void PageItem::drawLockedMarker(ScPainter *p) const
{
	//TODO: CB clean
	double scp1 = p->zoomFactor() ;
	double ofwh = 6 * scp1;
	double ofx = m_width - ofwh/2;
	double ofy = m_height - ofwh*1.5;
	double bx1 = ofx + scp1;
	double by1 = ofy + 3 * scp1;
	double bw = 4 * scp1;
	double bh = 2 * scp1;
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
		p->drawLine(FPoint(bx1 + scp1 / 2, ofy + scp1), FPoint(bx1 + scp1 / 2, by1));
	p->drawLine(FPoint(bx1 + scp1 * 3.5, ofy + scp1), FPoint(bx1 + scp1 * 3.5, by1));
	p->drawLine(FPoint(bx1 + scp1 / 2, ofy + scp1), FPoint(bx1 + scp1 * 3.5, ofy + scp1));
}

void PageItem::drawArrow(ScPainter *p, QTransform &arrowTrans, int arrowIndex)
{
	FPointArray arrow = m_Doc->arrowStyles().at(arrowIndex - 1).points.copy();
	if (NamedLStyle.isEmpty())
	{
		if (m_lineWidth != 0.0)
			arrowTrans.scale(m_lineWidth, m_lineWidth);
	}
	else
	{
		multiLine ml = m_Doc->docLineStyles[NamedLStyle];
		if (ml[ml.size()-1].Width != 0.0)
			arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size() - 1].Width);
	}
	arrow.map(arrowTrans);

	p->setupPolygon(&arrow);
	if (m_Doc->layerOutline(m_layerID))
	{
		p->strokePath();
		return;
	}

	p->setBlendModeStroke(lineBlendmode());
	p->setBlendModeFill(lineBlendmode()); // needed for fill in arrow shapes
	if (NamedLStyle.isEmpty())
	{
		ScPattern *strokePattern = m_Doc->checkedPattern(patternStrokeVal);
		if (strokePattern)
		{
			p->setPattern(strokePattern, patternStrokeTransfrm, patternStrokeMirrorX, patternStrokeMirrorY);
			p->setFillMode(ScPainter::Pattern);
			p->fillPath();
		}
		else if (GrTypeStroke > 0)
		{
			if ((!gradientStrokeVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientStrokeVal)))
				gradientStrokeVal.clear();
			if (!(gradientStrokeVal.isEmpty()) && (m_Doc->docGradients.contains(gradientStrokeVal)))
				stroke_gradient = m_Doc->docGradients[gradientStrokeVal];
			if (stroke_gradient.stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
			{
				if (lineColor() != CommonStrings::None)
				{
					p->setBrush(m_strokeQColor);
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
				if (GrTypeStroke == Gradient_Linear)
					p->setGradient(VGradient::linear, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeStartX, GrStrokeStartY), GrStrokeScale, GrStrokeSkew);
				else
					p->setGradient(VGradient::radial, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeFocalX, GrStrokeFocalY), GrStrokeScale, GrStrokeSkew);
			}
			p->fillPath();
		}
		else if (lineColor() != CommonStrings::None)
		{
			p->setBrush(m_strokeQColor);
			p->setBrushOpacity(1.0 - lineTransparency());
			p->setLineWidth(0);
			p->setFillMode(ScPainter::Solid);
			p->fillPath();
		}
	}
	else
	{
		multiLine ml = m_Doc->docLineStyles[NamedLStyle];
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
	p->setBlendModeStroke(0);
	p->setBlendModeFill(0);
}

void PageItem::adjustPictScale()
{
	if (itemType() != PageItem::ImageFrame)
		return;
	if (ScaleType)
		return;
	if (OrigW == 0 || OrigH == 0)
		return;
	double xs = m_width / static_cast<double>(OrigW);
	double ys = m_height / static_cast<double>(OrigH);
	double imgXOffs = m_imageXOffset;
	double imgYOffs = m_imageYOffset; 
	double imageRot = fmod(m_imageRotation, 360);
	if (imageRot != 0.0)
	{
		QRectF br(0, 0, OrigW, OrigH);
		QTransform m;
		m.rotate(m_imageRotation);
		br = m.mapRect(br);
		xs = m_width / br.width();
		ys = m_height / br.height();
		double xs2 = AspectRatio ? qMin(xs, ys) : xs;
		double ys2 = AspectRatio ? qMin(xs, ys) : ys;
		QLineF wL(0, 0, OrigW, 0);
		QLineF hL(0, 0, 0, OrigH);
		QTransform mm;
		mm.scale(xs2, ys2);
		mm.rotate(-m_imageRotation);
		hL = mm.map(hL);
		wL = mm.map(wL);
		xs = wL.length() / static_cast<double>(OrigW);
		ys = hL.length() / static_cast<double>(OrigH);
	}
	if (AspectRatio)
	{
		m_imageXScale = qMin(xs, ys);
		m_imageYScale = qMin(xs, ys);
	}
	else
	{
		m_imageXScale = xs;
		m_imageYScale = ys;
	}
	if (imageRot != 0.0)
	{
		QRectF br(0, 0, OrigW * xs, OrigH * ys);
		QTransform m;
		m.scale(1.0 / xs, 1.0 / ys);
		m.rotate(m_imageRotation);
		br = m.mapRect(br);
		m_imageXOffset = -br.x();
		m_imageYOffset = -br.y();
	}
	else
	{
		m_imageXOffset = 0.0;
		m_imageYOffset = 0.0;
	}
	if (m_Doc && m_Doc->isLoading())
	{
		m_imageXOffset = imgXOffs;
		m_imageYOffset = imgYOffs;
	}
	if (!imageClip.empty())
	{
		imageClip = pixm.imgInfo.PDSpathData[pixm.imgInfo.usedPath].copy();
		QTransform cl;
		cl.translate(m_imageXOffset*m_imageXScale, m_imageYOffset*m_imageYScale);
		cl.scale(m_imageXScale, m_imageYScale);
		imageClip.map(cl);
	}
}

void PageItem::setExternalFile(const QString& filename, const QString& baseDir)
{
	Pfile = filename;
	if (Pfile.isEmpty())
		return;

	QString test = Pfile;
	if (QDir::isRelativePath(test))
	{
		if (baseDir.isEmpty())
			test = QDir::homePath() + "/" + Pfile;
		else
			test = baseDir + "/" + Pfile;
	}
	test = QDir::cleanPath(QDir::toNativeSeparators(test));
	QFileInfo pfi2(test);
	Pfile = pfi2.absoluteFilePath();
}

void PageItem::setFileIconPressed(const QString& filename, const QString& baseDir)
{
	Pfile2 = filename;
	if (Pfile2.isEmpty())
		return;

	QString test = Pfile2;
	if (QDir::isRelativePath(test))
	{
		if (baseDir.isEmpty())
			test = QDir::homePath() + "/" + Pfile2;
		else
			test = baseDir + "/" + Pfile2;
	}
	test = QDir::cleanPath(QDir::toNativeSeparators(test));
	QFileInfo pfi2(test);
	Pfile2 = pfi2.absoluteFilePath();
}

void PageItem::setFileIconRollover(const QString& filename, const QString& baseDir)
{
	Pfile3 = filename;
	if (Pfile3.isEmpty())
		return;

	QString test = Pfile3;
	if (QDir::isRelativePath(test))
	{
		if (baseDir.isEmpty())
			test = QDir::homePath() + "/" + Pfile3;
		else
			test = baseDir + "/" + Pfile3;
	}
	test = QDir::cleanPath(QDir::toNativeSeparators(test));
	QFileInfo pfi2(test);
	Pfile3 = pfi2.absoluteFilePath();
}

PageItem* PageItem::firstInChain()
{
	//huh? Q_ASSERT(this != nullptr);
	PageItem* first = this;
	while (first->prevInChain() != nullptr)
		first = first->prevInChain();
	return first;
}

PageItem* PageItem::lastInChain()
{
	//huh? Q_ASSERT(this != nullptr);
	PageItem* last = this;
	while (last->nextInChain() != nullptr)
		last = last->nextInChain();
	return last;
}

PageItem* PageItem::lastInChainSamePage()
{
	//huh? Q_ASSERT(this != nullptr);
	PageItem* last = this;
	while (last->OwnPage != OwnPage && last->nextInChain() != nullptr)
		last = last->nextInChain();
	return last;
}

QRect PageItem::getRedrawBounding(double viewScale) const
{
	double maxLineWidth = qMax(m_lineWidth, m_oldLineWidth);
	QRectF boundingRect(-maxLineWidth / 2.0 - 5 / viewScale,
	                    -maxLineWidth / 2.0 - 5 / viewScale,
	                    BoundingW + maxLineWidth + 10 / viewScale,
	                    BoundingH + maxLineWidth + 10 / viewScale);
	QTransform t = getTransform();
	boundingRect = t.mapRect(boundingRect);

	QRectF redrawBoundingF((boundingRect.x() - m_Doc->minCanvasCoordinate.x()) * viewScale,
	                       (boundingRect.y() - m_Doc->minCanvasCoordinate.y()) * viewScale,
	                        boundingRect.width() * viewScale,
	                        boundingRect.height() * viewScale);
	redrawBoundingF = redrawBoundingF.normalized();

	int x = floor(redrawBoundingF.left());
	int y = floor(redrawBoundingF.top());
	int w = ceil(redrawBoundingF.right()) - x;
	int h = ceil(redrawBoundingF.bottom()) - y;
	return QRect(x, y, w, h);
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
		case Gradient_None:
		case Gradient_LinearLegacy1:
			setGradientStart(0.0, m_height / 2.0);
			setGradientEnd(m_width, m_height / 2.0);
			break;
		case Gradient_LinearLegacy2:
			setGradientStart(m_width / 2.0, 0.0);
			setGradientEnd(m_width / 2.0, m_height);
			break;
		case Gradient_LinearLegacy3:
			setGradientStart(0.0, 0.0);
			setGradientEnd(m_width, m_height);
			break;
		case Gradient_LinearLegacy4:
			setGradientStart(0.0, m_height);
			setGradientEnd(m_width, 0.0);
			break;
		case Gradient_RadialLegacy5:
			setGradientStart(m_width / 2.0, m_height / 2.0);
			if (m_width >= m_height)
				setGradientEnd(m_width, m_height / 2.0);
			else
				setGradientEnd(m_width / 2.0, m_height);
			break;
		default:
			break;
	}
	//if (ScMW->view->SelItem.count() != 0 && this == ScMW->view->SelItem.at(0))
	//if (m_Doc->m_Selection->count() != 0 && m_Doc->m_Selection->primarySelectionIsMyself(this))
	//	ScMW->propertiesPalette->updateColorSpecialGradient();
	//CB Will only emit if connected, ie is first in GUI selection
	//unused double dur = m_Doc->unitRatio();
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
	for (int i = 0; i < polist.count(); i++)
	{
		QPolygon cli = polist[i].toPolygon();
		cl += cli;
		Segments.append(cl.size());
	}
	if (cl.size() > 1)
	{
		Clip.resize(0);
		for (int i = 0; i < cl.size() - 1; ++i)
		{
			rot = xy2Deg(cl.point(i + 1).x() - cl.point(i).x(), cl.point(i + 1).y() - cl.point(i).y());
			QTransform ma;
			ma.rotate(rot);
			np = QPoint(0, -upval) * ma;
			np2 = QPoint(0, -downval) * ma;
			cl1.resize(cl1.size() + 1);
			cl1.setPoint(cl1.size() - 1, np+cl.point(i));
			cl1.resize(cl1.size() + 1);
			cl1.setPoint(cl1.size() - 1, np + cl.point(i + 1));
			cl2.resize(cl2.size() + 1);
			cl2.setPoint(cl2.size() - 1, np2 + cl.point(i));
			cl2.resize(cl2.size() + 1);
			cl2.setPoint(cl2.size() - 1, np2 + cl.point(i + 1));
		}
		cl1.resize(cl1.size() + 1);
		cl1.setPoint(cl1.size() - 1, np + cl.point(cl.size() - 1));
		cl2.resize(cl2.size()+1);
		cl2.setPoint(cl2.size() - 1, np2 + cl.point(cl.size() - 1));
		Clip.putPoints(Clip.size(), cl1.size(), cl1);
		for (int i2 = cl2.size() - 1; i2 > -1; i2--)
		{
			Clip.resize(Clip.size() + 1);
			Clip.setPoint(Clip.size() - 1, cl2.point(i2));
		}
	}
}

void PageItem::updatePolyClip()
{
	int asce = 1;
	int desc = 1;
	int itemTextCount = itemText.length();
	for (int i = 0; i < itemTextCount; ++i)
	{
		const CharStyle& hl (itemText.charStyle(i));
		int des = static_cast<int>(hl.font().descent(hl.fontSize() / 10.0));
		int asc = static_cast<int>(hl.font().ascent(hl.fontSize() / 10.0));
		asce = qMax(asce, asc);
		desc = qMax(desc, des);
	}
	setPolyClip(static_cast<int>(asce - BaseOffs), static_cast<int>(desc - BaseOffs));
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

	connect(this, SIGNAL(frameType(int)), m_Doc->scMW(), SLOT(HaveNewSel()), Qt::UniqueConnection);
	connect(this, SIGNAL(frameType(int)), m_Doc, SLOT(selectionChanged()), Qt::UniqueConnection);
	connect(this, SIGNAL(textStyle(int)), m_Doc->scMW(), SLOT(setStyleEffects(int)), Qt::UniqueConnection);

	return true;
}

bool PageItem::disconnectFromGUI()
{
	if (!ScCore->usingGUI())
		return false;
	// Disconnecting only signals from PP will leave some remaining connections
	// and cause progressive slowdowns
	// PropertiesPalette* pp = m_Doc->scMW()->propertiesPalette;
	// disconnect(this, 0, pp, 0);
	this->disconnect();
	return true;
}

void PageItem::emitAllToGUI()
{
	updateConstants();

	emit myself(this);
	emit frameType(m_itemType);

//CB unused in 135
//	double dur = m_Doc->unitRatio();
//	emit blendmode(m_fillBlendMode, m_lineBlendMode);
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
	if (m_isAnnotation == isAnnot)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::ActionPDF, nullptr, Um::IGroup);
		ss->set("ACTIONPDFANNOTATION", isAnnot);
		undoManager->action(this, ss);
	}
	m_isAnnotation = isAnnot;
}

void PageItem::setIsBookMark(bool isBM)
{
	if (isBookmark == isBM)
		return; // nothing to do -> return
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::ActionPDF, nullptr, Um::IGroup);
		ss->set("ACTIONPDFBOOKMARK", isBM);
		undoManager->action(this, ss);
	}
	isBookmark = isBM;
}

void PageItem::setResolution(int id)
{
	if (pixm.imgInfo.lowResType == id)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::ResTyp, QString(), Um::IImageFrame);
		ss->set("RES_TYP");
		ss->set("OLD_RES",pixm.imgInfo.lowResType);
		ss->set("NEW_RES",id);
		undoManager->action(this, ss);
	}
	pixm.imgInfo.lowResType = id;
}

void PageItem::setAnnotation(const Annotation& ad)
{
	m_annotation = ad;
}

void PageItem::setImageVisible(bool isShown)
{
	if (m_imageVisible == isShown)
		return;
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::ResTyp, QString(), Um::IImageFrame);
		ss->set("SHOW_IMAGE");
		ss->set("OLD", m_imageVisible);
		undoManager->action(this, ss);
	}
	m_imageVisible = isShown;
}

void PageItem::updateConstants()
{
	m_Doc->constants().insert("width", m_width);
	m_Doc->constants().insert("height", m_height);
}

//CB Old ScribusView MoveItemI
void PageItem::moveImageInFrame(double newX, double newY)
{
	if (m_itemType != PageItem::ImageFrame)
		return;
	if (locked())// || (!ScaleType))
		return;
	double dX=0.0, dY=0.0;
	if (imageFlippedH())
		dX = -newX;
	else
		dX = newX;
	if (imageFlippedV())
		dY = -newY;
	else
		dY = newY;
	moveImageXYOffsetBy(dX, dY);
	if (!imageClip.empty())
	{
		imageClip = pixm.imgInfo.PDSpathData[pixm.imgInfo.usedPath].copy();
		QTransform cl;
		cl.translate(imageXOffset() * imageXScale(), imageYOffset() * imageYScale());
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
		Clip = flattenPath(PoLine, Segments);
	}
	else
	{
		SetRectFrame();
		m_Doc->setRedrawBounding(this);
	}
}

//udateWelded determine if welded items should be updated as well (default behaviour)
void PageItem::updateClip(bool updateWelded)
{
	if (m_Doc->appMode == modeDrawBezierLine)
		return;
	if (ContourLine.empty())
		ContourLine = PoLine.copy();
//	int ph = static_cast<int>(qMax(1.0, lineWidth() / 2.0));
	bool clipBackup = ClipEdited;
	switch (itemType())
	{
	case PageItem::Line:
		{
			PageItem_Line* lineItem = asLine();
			if (lineItem)
				lineItem->setLineClip();
		}
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
						Clip = flattenPath(PoLine, Segments);
				}
				if (updateWelded)
				{
					for (int i = 0 ; i < weldList.count(); i++)
					{
						WeldingInfo wInf = weldList.at(i);
						if (!wInf.weldItem)
							continue;
						if (wInf.weldItem->isNoteFrame())
						{
							PageItem_NoteFrame* noteFrame = wInf.weldItem->asNoteFrame();
							if (noteFrame->notesStyle()->isAutoWeldNotesFrames())
							{
								if (noteFrame->notesStyle()->isAutoNotesWidth())
								{
									if (noteFrame->width() != width())
									{
										noteFrame->setWidth(width());
										noteFrame->updateClip();
									}
								}
								noteFrame->setXYPos(xPos(),yPos() + height());
								setWeldPoint(0, height(), noteFrame);
								noteFrame->setWeldPoint(0,0, this);
								continue;
							}
						}
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
			OldB2 = (OldB2 == 0) ? 1 : OldB2;
			OldH2 = (OldH2 == 0) ? 1 : OldH2;
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
				Clip = flattenPath(PoLine, Segments);
			OldB2 = width();
			OldH2 = height();
			if (updateWelded)
			{
				for (int i = 0 ; i < weldList.count(); i++)
				{
					WeldingInfo wInf = weldList.at(i);
					if (wInf.weldItem->isNoteFrame())
					{
						PageItem_NoteFrame* noteFrame = wInf.weldItem->asNoteFrame();
						if (noteFrame->notesStyle()->isAutoWeldNotesFrames())
						{
							if (noteFrame->notesStyle()->isAutoNotesWidth())
							{
								if (noteFrame->width() != width())
								{
									noteFrame->setWidth(width());
									noteFrame->updateClip();
								}
							}
							setWeldPoint(0, height(), noteFrame);
							noteFrame->setWeldPoint(0,0, this);
							continue;
						}
					}
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
		}
		break;
	}
	updateGradientVectors();
}

QString PageItem::infoDescription() const
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
	return m_firstLineOffset;
}

void PageItem::setFirstLineOffset(FirstLineOffsetPolicy flop)
{
	if (m_firstLineOffset == flop)
		return;

	if (UndoManager::undoEnabled())
	{
		auto *is = new ScOldNewState<FirstLineOffsetPolicy>(Um::FirstLineOffset);
		is->set("FIRSTLINEOFFSET");
		is->setStates(m_firstLineOffset, flop);
		undoManager->action(this, is);
	}
	m_firstLineOffset = flop;
}

void PageItem::setInlineData(const QString& data)
{
	QByteArray inlineImageData;
	inlineImageData.append(data.toUtf8());
	if (inlineImageData.size() > 0)
	{
		auto *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + inlineExt);
		tempFile->setAutoRemove(false);
		tempFile->open();
		QString fileName = getLongPathName(tempFile->fileName());
		tempFile->close();
		inlineImageData = qUncompress(QByteArray::fromBase64(inlineImageData));
		QFile outFil(fileName);
		if (outFil.open(QIODevice::WriteOnly))
		{
			outFil.write(inlineImageData);
			outFil.close();
			isInlineImage = true;
			isTempFile = true;
			Pfile = fileName;
			delete tempFile;
		}
	}
}

void PageItem::makeImageInline()
{
	QFileInfo fi(Pfile);
	QString ext = fi.suffix();

	QScopedPointer<QTemporaryFile> tempFile(new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + ext));
	tempFile->setAutoRemove(false);
	if (!tempFile->open())
		return;
	QString fileName = getLongPathName(tempFile->fileName());
	tempFile->close();

	if (copyFile(Pfile, fileName))
	{
		Pfile = QDir::fromNativeSeparators(fileName);
		isInlineImage = true;
		isTempFile = true;
	}
	else
	{
		tempFile->setAutoRemove(true);
	}
}

void PageItem::makeImageExternal(const QString& path)
{
	if (isTempFile && isInlineImage && !path.isEmpty())
	{
		QString oldF = Pfile;
		if (copyFile(Pfile, path))
		{
			Pfile = path;
			QFile::remove(oldF);
			isInlineImage = false;
			isTempFile = false;
		}
	}
}

void PageItem::addWelded(PageItem* item)
{
	FPoint centerI(xPos() + (width() / 2.0), yPos() + (height() / 2.0));
	FPoint centerP(item->xPos() + (item->width() / 2.0), item->yPos() + (item->height() / 2.0));
	WeldingInfo wInf;
	wInf.weldItem = item;
	wInf.weldPoint = FPoint((width() / 2.0) + ((centerP.x() - centerI.x()) / 2.0), (height() / 2.0) + ((centerP.y() - centerI.y()) / 2.0));
	weldList.append(wInf);
}

//welded frames
void PageItem::weldTo(PageItem* item)
{
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = undoManager->beginTransaction(Um::WeldItems + "/" + Um::Selection, Um::IGroup,
														  Um::WeldItems, QString(), Um::IGroup);
	for (int i = 0 ; i <  weldList.count(); i++)
	{
		PageItem::WeldingInfo wInf = weldList.at(i);
		if (wInf.weldItem == item)
			return;
	}
	QList<PageItem*> weldItems = item->itemsWeldedTo();
	if (weldItems.contains(this))
		return;
	addWelded(item);
	item->addWelded(this);
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<PageItem*>(Um::WeldItems, QString(), Um::IGroup);
		is->set("WELD_ITEMS");
		is->setItem(item);
		undoManager->action(this, is, getUPixmap());
	}
	update();
	item->update();
	if (activeTransaction)
		activeTransaction.commit();
}

void PageItem::moveWelded(double dX, double dY, int weld)
{
	WeldingInfo wInf = weldList.at(weld);
	PageItem *item = wInf.weldItem;
	item->setXPos(item->xPos() + dX);
	item->setYPos(item->yPos() + dY);
	item->update();
	item->moveWelded(dX, dY, this);
}

void PageItem::moveWelded(double dX, double dY, const PageItem* except)
{
	//qDebug()<<"PageItem::moveWelded"<<dX<<dY;
	if ((dX == 0) && (dY == 0))
		return;
	//do not save undo for auto-welded notes frames
	UndoManager::instance()->setUndoEnabled(false);
	for (int i = 0 ; i < weldList.count(); i++)
	{
		WeldingInfo wInf = weldList.at(i);
		PageItem *item = wInf.weldItem;
		if (item != except)
		{
			item->setXPos(item->xPos() + dX);
			item->setYPos(item->yPos() + dY);
			item->update();
			item->moveWelded(dX, dY, this);
		}
	}
	UndoManager::instance()->setUndoEnabled(true);
}

void PageItem::rotateWelded(double dR, double oldRot)
{
	UndoManager::instance()->setUndoEnabled(false);
	QTransform ma;
	ma.translate(xPos(), yPos());
	ma.scale(1, 1);
	ma.rotate(oldRot);
	switch (m_Doc->rotationMode())
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
	for (int i = 0; i < itemList.count(); i++)
	{
		PageItem *item = itemList.at(i);
		QLineF lin(rotCenter, QPointF(item->xPos(), item->yPos()));
		lin.setAngle(lin.angle() - dR);
		item->setXYPos(lin.p2().x(), lin.p2().y());
		item->rotateBy(dR);
	}
	UndoManager::instance()->setUndoEnabled(true);
}

QList<PageItem*> PageItem::itemsWeldedTo(PageItem* except)
{
	QList<PageItem*> pageItemList;
	for (int i = 0 ; i < weldList.count(); i++)
	{
		WeldingInfo wInf = weldList.at(i);
		PageItem *item = wInf.weldItem;
		if (item != except)
		{
			pageItemList.append(item);
			if (item->isWelded())
				pageItemList.append(item->itemsWeldedTo(this));
		}
	}
	return pageItemList;
}

void PageItem::setWeldPoint(double dX, double dY, const PageItem *pItem)
{
	for (int i = 0 ; i < weldList.count(); i++)
	{
		const PageItem *item = weldList[i].weldItem;
		if (item == pItem)
		{
			weldList[i].weldPoint = FPoint(dX, dY);
			return;
		}
	}
}

void PageItem::unWeld()
{
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = undoManager->beginTransaction(Um::UnweldItems + "/" + Um::Selection, Um::IGroup,
														  Um::UnweldItems, QString(), Um::IDelete);
	for (int i = 0 ; i < weldList.count(); i++)
	{
		WeldingInfo wInf = weldList.at(i);
		PageItem *item = wInf.weldItem;
		if (item == nullptr)
		{
			qDebug() << "unWeld - null pointer in weldList";
			continue;
		}
		for (int j = 0 ; j < item->weldList.count(); j++)
		{
			WeldingInfo wInf2 = item->weldList.at(j);
			const PageItem *item2 = wInf2.weldItem;
			if (item2 == this)
			{
				item->weldList.removeAt(j);
				if (UndoManager::undoEnabled())
				{
					auto *is = new ScItemState<PageItem*>(Um::UnweldItems, QString(), Um::IGroup);
					is->set("UNWELD_ITEM");
					is->setItem(item);
					is->set("thisPoint_x", wInf.weldPoint.x());
					is->set("thisPoint_y", wInf.weldPoint.y());
					is->set("thisID", wInf.weldID);
					is->set("Point_x", wInf2.weldPoint.x());
					is->set("Point_y", wInf2.weldPoint.y());
					is->set("ID", wInf2.weldID);
					undoManager->action(this, is, getUPixmap());
				}
				break;
			}
		}
	}
	if (activeTransaction)
		activeTransaction.commit();
	weldList.clear();
}

QString PageItem::getItemTextSaxed(int selStart, int selLength)
{
	if (selStart < 0 || selLength < 0)
		return QString();

	StoryText it(m_Doc);
	it.setDefaultStyle(itemText.defaultStyle());

	if (selLength == 0)
		selLength = 1;
	itemText.deselectAll();
	itemText.select(selStart, selLength);
	it.insert(0, itemText, true);
	itemText.deselectAll();

	//saxing text
	std::ostringstream xmlString;
	SaxXML xmlStream(xmlString);
	xmlStream.beginDoc();
	it.saxx(xmlStream, "SCRIBUSTEXT");
	xmlStream.endDoc();
	std::string xml(xmlString.str());
	return QString(xml.c_str());
}

bool compareItemLevel(const PageItem* item1, const PageItem* item2)
{
	int level1 = item1->level();
	int level2 = item2->level();
	return (level1 < level2);
}
