/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scpageoutput.h"

#include <QList>
#include <QPointF>
#include <QRectF>
#include <QStack>
#include <pageitem_textframe.h>
#include "cmsettings.h"
#include "collapsedtablepainterex.h"
#include "commonstrings.h"
#include "pageitem.h"
#include "pageitem_arc.h"
#include "pageitem_group.h"
#include "pageitem_imageframe.h"
#include "pageitem_line.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_spiral.h"
#include "pageitem_table.h"
#include "prefsmanager.h"
#include "scfonts.h"
#include "scimage.h"
#include "scpage.h"
#include "scpattern.h"
#include "text/textlayoutpainter.h"

#include "scribusdoc.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"
#include "text/boxes.h"

MarksOptions::MarksOptions(const PrintOptions& opt)
{
	markLength = opt.markLength;
	markOffset = opt.markOffset;
	bleedTop = opt.bleeds.top();
	bleedLeft = opt.bleeds.left();
	bleedRight = opt.bleeds.right();
	bleedBottom = opt.bleeds.bottom();
	cropMarks = opt.cropMarks;
	bleedMarks = opt.bleedMarks;
	registrationMarks = opt.registrationMarks;
	colorMarks = opt.colorMarks;
	docInfoMarks = true;
}

ScPageOutput::ScPageOutput(ScribusDoc* doc, bool reloadImages, int resolution, bool useProfiles) 
			: m_marksOptions()
{
	m_doc = doc;
	m_reloadImages = reloadImages;
	m_imageRes = resolution;
	m_useProfiles = useProfiles;
}

ScImage::RequestType ScPageOutput::translateImageModeToRequest(ScPainterExBase::ImageMode mode) const
{
	ScImage::RequestType value = ScImage::RGBData;
	if (mode == ScPainterExBase::cmykImages)
		value = ScImage::CMYKData;
	else if (mode == ScPainterExBase::rgbImages)
		value = ScImage::RGBData;
	else if (mode == ScPainterExBase::rawImages)
		value = ScImage::RawData;
	return value;
}

void ScPageOutput::drawPage(ScPage* page, ScPainterExBase* painter)
{
	int clipx = static_cast<int>(page->xOffset());
	int clipy = static_cast<int>(page->yOffset());
	int clipw = qRound(page->width());
	int cliph = qRound(page->height());

	int layerCount = m_doc->layerCount();
	for (int i = 0; i < layerCount; ++i)
	{
		const ScLayer* pLayer = m_doc->Layers.layerByLevel(i);
		if (!pLayer)
			continue;

		if ((layerCount > 1) && ((pLayer->blendMode != 0) || (pLayer->transparency != 1.0)))
			painter->beginLayer(pLayer->transparency, pLayer->blendMode);

		drawMasterItems(painter, page, *pLayer, QRect(clipx, clipy, clipw, cliph));
		drawPageItems(painter, page, *pLayer, QRect(clipx, clipy, clipw, cliph));

		if ((layerCount > 1) && ((pLayer->blendMode != 0) || (pLayer->transparency != 1.0)))
			painter->endLayer();
	}
	drawMarks(page, painter, m_marksOptions);
}

void ScPageOutput::drawMasterItems(ScPainterExBase *painter, ScPage *page, const ScLayer& layer, const QRect& clip)
{
	if (page->masterPageNameEmpty())
		return;
	if (page->FromMaster.count() <= 0)
		return;
	if (!layer.isViewable || !layer.isPrintable)
		return;

	ScPage* masterPage = m_doc->MasterPages.at(m_doc->MasterNames[page->masterPageName()]);
	int pageFromMasterCount = page->FromMaster.count();
	for (int i = 0; i < pageFromMasterCount; ++i)
	{
		PageItem* currItem = page->FromMaster.at(i);
		if (currItem->m_layerID != layer.ID)
			continue;
		if ((currItem->OwnPage != -1) && (currItem->OwnPage != masterPage->pageNr()))
			continue;
		if (!currItem->printEnabled())
			continue;
		int savedOwnPage = currItem->OwnPage;
		double OldX = currItem->xPos();
		double OldY = currItem->yPos();
		double OldBX = currItem->BoundingX;
		double OldBY = currItem->BoundingY;
		currItem->OwnPage = page->pageNr();
		if (!currItem->ChangedMasterItem)
		{
			currItem->moveBy(-masterPage->xOffset() + page->xOffset(), -masterPage->yOffset() + page->yOffset(), true);
			currItem->BoundingX = OldBX - masterPage->xOffset() + page->xOffset();
			currItem->BoundingY = OldBY - masterPage->yOffset() + page->yOffset();
		}
		QRectF oldR(currItem->getBoundingRect().adjusted(0.0, 0.0, 1.0, 1.0));
		if (clip.intersects(oldR.toRect()))
			drawItem(currItem, painter, clip);
		currItem->OwnPage = savedOwnPage;
		if (!currItem->ChangedMasterItem)
		{
			currItem->setXYPos(OldX, OldY, true);
			currItem->BoundingX = OldBX;
			currItem->BoundingY = OldBY;
		}
	}
}

void ScPageOutput::drawPageItems(ScPainterExBase *painter, ScPage *page, const ScLayer& layer, const QRect& clip)
{
	if (m_doc->Items->count() <= 0)
		return;
	if (!layer.isViewable || !layer.isPrintable)
		return;

	int currentPageNr = page->pageNr();
	for (int it = 0; it < m_doc->Items->count(); ++it)
	{
		PageItem* currItem = m_doc->Items->at(it);
		if (currItem->m_layerID != layer.ID)
			continue;
		if (!currItem->printEnabled())
			continue;
		if ((m_doc->masterPageMode()) && ((currItem->OwnPage != -1) && (currItem->OwnPage != currentPageNr)))
			continue;
		if (!m_doc->masterPageMode() && !currItem->OnMasterPage.isEmpty())
		{
			if (currItem->OnMasterPage != page->pageName())
				continue;
		}
		QRectF oldR(currItem->getBoundingRect().adjusted(0.0, 0.0, 1.0, 1.0));
		if (clip.intersects(oldR.toRect()))
		{
			drawItem(currItem, painter, clip);
		}
	}
}

void ScPageOutput::drawItem(PageItem* item, ScPainterExBase* painter, QRect clip)
{
	if (clip.isNull())
	{
		clip = QRectF(QPointF(m_doc->minCanvasCoordinate.x(), m_doc->minCanvasCoordinate.y()), 
		              QPointF(m_doc->maxCanvasCoordinate.x(), m_doc->maxCanvasCoordinate.y())).toAlignedRect();
	}

	drawItem_Pre(item, painter);
	PageItem::ItemType itemType = item->itemType();
	if (itemType == PageItem::Arc)
		drawItem_Arc((PageItem_Arc*) item, painter, clip);
	else if (itemType == PageItem::Group)
		drawItem_Group((PageItem_Group*) item, painter, clip);
	else if (itemType == PageItem::ImageFrame)
		drawItem_ImageFrame((PageItem_ImageFrame*) item, painter, clip);
	else if (itemType == PageItem::Line)
		drawItem_Line((PageItem_Line*) item, painter, clip);
	else if (itemType == PageItem::Polygon)
		drawItem_Polygon((PageItem_Polygon*) item, painter, clip);
	else if (itemType == PageItem::PolyLine)
		drawItem_PolyLine((PageItem_PolyLine*) item, painter, clip);
	else if (itemType == PageItem::RegularPolygon)
		drawItem_RegularPolygon((PageItem_RegularPolygon*) item, painter, clip);
	else if (itemType == PageItem::Spiral)
		drawItem_Spiral((PageItem_Spiral*) item, painter, clip);
	else if (itemType == PageItem::Table)
		drawItem_Table((PageItem_Table*) item, painter, clip);
	else if (itemType == PageItem::TextFrame || itemType == PageItem::PathText)
		drawItem_Text(item, painter, clip);
	drawItem_Post(item, painter);
}

void ScPageOutput::drawItem_Pre(PageItem* item, ScPainterExBase* painter)
{
	painter->save();
	if (!item->isEmbedded)
		painter->translate(item->xPos(), item->yPos());
	painter->rotate(item->rotation());
	painter->setBlendModeFill(item->fillBlendmode());
	painter->setLineWidth(item->lineWidth());
	if (item->isGroup())
		return;

	if (item->GrType == Gradient_None)
	{
		painter->m_fillGradient = VGradientEx(VGradientEx::linear);
		if (item->fillColor() != CommonStrings::None)
		{
			painter->setBrush(ScColorShade(m_doc->PageColors[item->fillColor()], (int) item->fillShade()));
			painter->setFillMode(ScPainterExBase::Solid);
		}
		else
			painter->setFillMode(ScPainterExBase::None);
	}
	else if (item->GrType == Gradient_Pattern)
	{
		QString pat = item->pattern();
		ScPattern *pattern = m_doc->checkedPattern(pat);
		if (!pattern)
		{
			painter->m_fillGradient = VGradientEx(VGradientEx::linear);
			if (item->fillColor() != CommonStrings::None)
			{
				painter->setBrush(ScColorShade(m_doc->PageColors[item->fillColor()], (int) item->fillShade()));
				painter->setFillMode(ScPainterExBase::Solid);
			}
			else
				painter->setFillMode(ScPainterExBase::None);
		}
		else
		{
			QTransform patternTransform;
			bool   patternMirrorX, patternMirrorY;
			const ScPatternTransform& patternTrans = item->patternTransform();
			item->patternFlip(patternMirrorX, patternMirrorY);
			painter->setPattern(pattern, patternTrans, patternMirrorX, patternMirrorY);
			painter->setFillMode(ScPainterExBase::Pattern);
		}
	}
	else if (item->GrType == Gradient_4Colors)
	{
		painter->setFillMode(ScPainterExBase::Gradient);
		FPoint pG1(0, 0);
		FPoint pG2(item->width(), 0);
		FPoint pG3(item->width(), item->height());
		FPoint pG4(0, item->height());
		ScColorShade col1(m_doc->PageColors[item->GrColorP1], item->GrCol1Shade);
		ScColorShade col2(m_doc->PageColors[item->GrColorP2], item->GrCol2Shade);
		ScColorShade col3(m_doc->PageColors[item->GrColorP3], item->GrCol3Shade);
		ScColorShade col4(m_doc->PageColors[item->GrColorP4], item->GrCol4Shade);
		painter->set4ColorGeometry(pG1, pG2, pG3, pG4, item->GrControl1, item->GrControl2, item->GrControl3, item->GrControl4);
		painter->set4ColorColors(col1, col2, col3, col4);
	}
	else if (item->GrType == Gradient_Hatch)
	{
		if (item->fillColor() != CommonStrings::None)
			painter->setBrush(ScColorShade(m_doc->PageColors[item->fillColor()], (int) item->fillShade()));
		painter->setFillMode(ScPainterExBase::Hatch);
		bool hatchUseBackground = item->hatchUseBackground;
		if ((item->hatchBackground == CommonStrings::None) || !m_doc->PageColors.contains(item->hatchBackground))
			hatchUseBackground = false;
		ScColorShade hatchBackgroundSh;
		ScColorShade hatchForegroundSh(m_doc->PageColors[item->hatchForeground], 100.0);
		if (hatchUseBackground)
			hatchBackgroundSh = ScColorShade(m_doc->PageColors[item->hatchBackground], 100.0);
		painter->setHatchParameters(item->hatchType, item->hatchDistance, item->hatchAngle, hatchUseBackground, hatchBackgroundSh, hatchForegroundSh, item->width(), item->height());
	}
	else
	{
		QString gradientVal = item->gradient();
		if ((!gradientVal.isEmpty()) && (!m_doc->docGradients.contains(gradientVal)))
			gradientVal.clear();
		if (!(gradientVal.isEmpty()) && (m_doc->docGradients.contains(gradientVal)))
			painter->m_fillGradient = VGradientEx(m_doc->docGradients[gradientVal], *m_doc);
		if ((painter->m_fillGradient.stops() < 2) && (item->GrType < Gradient_4Colors)) // fall back to solid filling if there are not enough colorstops in the gradient.
		{
			if (item->fillColor() != CommonStrings::None)
			{
				painter->setBrush(ScColorShade(m_doc->PageColors[item->fillColor()], (int) item->fillShade()));
				painter->setFillMode(ScPainterExBase::Solid);
			}
			else
				painter->setFillMode(ScPainterExBase::None);
		}
		else
		{
			FPoint fpStart(item->GrStartX, item->GrStartY);
			FPoint fpEnd(item->GrEndX, item->GrEndY);
			FPoint fpFocal(item->GrFocalX, item->GrFocalY);
			painter->setFillMode(ScPainterExBase::Gradient);
			painter->m_fillGradient = VGradientEx(item->fill_gradient, *m_doc);
			switch (item->GrType)
			{
				case 1:
				case 2:
				case 3:
				case 4:
				case 6:
					painter->setGradient(VGradientEx::linear, fpStart, fpEnd, fpStart, item->GrScale, item->GrSkew);
					break;
				case 5:
				case 7:
					painter->setGradient(VGradientEx::radial, fpStart, fpEnd, fpFocal, item->GrScale, item->GrSkew);
					break;
				case 10:
					painter->setFillMode(ScPainterExBase::Gradient);
					painter->setDiamondGeometry(FPoint(0, 0), FPoint(item->width(), 0), FPoint(item->width(), item->height()), FPoint(0, item->height()), 
					                            item->GrControl1, item->GrControl2, item->GrControl3, item->GrControl4, item->GrControl5);
					break;
				case 11:
				case 13:
					painter->setFillMode(ScPainterExBase::Gradient);
					painter->setMeshGradient(FPoint(0, 0), FPoint(item->width(), 0), FPoint(item->width(), item->height()), FPoint(0, item->height()), item->meshGradientArray);
					break;
				case 12:
					painter->setFillMode(ScPainterExBase::Gradient);
					painter->setMeshGradient(FPoint(0, 0), FPoint(item->width(), 0), FPoint(item->width(), item->height()), FPoint(0, item->height()), item->meshGradientPatches);
					break;
			}
		}
	}

	if (item->lineColor() != CommonStrings::None)
	{
		if ((item->lineWidth() == 0) && !item->asLine())
			painter->setLineWidth(0);
		else
		{
			ScColorShade tmp(m_doc->PageColors[item->lineColor()], (int) item->lineShade());
			painter->setPen(tmp , item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
			if (!item->DashValues.isEmpty())
				painter->setDash(item->DashValues, item->DashOffset);
		}
	}
	else
		painter->setLineWidth(0);

	painter->setBrushOpacity(1.0 - item->fillTransparency());
	painter->setPenOpacity(1.0 - item->lineTransparency());
	painter->setFillRule(item->fillRule);

	if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_Radial) || (item->GrMask == GradMask_LinearLumAlpha) || (item->GrMask == GradMask_RadialLumAlpha))
	{
		QString gradientMaskVal = item->gradientMaskVal;
		FPoint fpMaskStart(item->GrMaskStartX, item->GrMaskStartY);
		FPoint fpMaskEnd(item->GrMaskEndX, item->GrMaskEndY);
		FPoint fpMaskFocal(item->GrMaskFocalX, item->GrMaskFocalY);
		if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_Radial))
			painter->setMaskMode(1);
		else
			painter->setMaskMode(3);
		if ((!gradientMaskVal.isEmpty()) && (!m_doc->docGradients.contains(gradientMaskVal)))
			gradientMaskVal.clear();
		if (!(gradientMaskVal.isEmpty()) && (m_doc->docGradients.contains(gradientMaskVal)))
			painter->m_maskGradient = VGradientEx(m_doc->docGradients[gradientMaskVal], *m_doc);
		else
			painter->m_maskGradient = VGradientEx(item->mask_gradient, *m_doc);
		if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_LinearLumAlpha))
			painter->setGradientMask(VGradientEx::linear, fpMaskStart, fpMaskEnd, fpMaskStart, item->GrMaskScale, item->GrMaskSkew);
		else
			painter->setGradientMask(VGradientEx::radial, fpMaskStart, fpMaskEnd, fpMaskFocal, item->GrMaskScale, item->GrMaskSkew);
	}
	else if ((item->GrMask == GradMask_Pattern) || (item->GrMask == GradMask_PatternLumAlpha) || (item->GrMask == GradMask_PatternLumAlphaInverted) || (item->GrMask == GradMask_PatternInverted))
	{
		QString patternMaskVal = item->patternMaskVal;
		ScPattern *patternMask = m_doc->checkedPattern(patternMaskVal);
		if (patternMask)
		{
			painter->setPatternMask(patternMask, item->patternMaskTransfrm, item->patternMaskMirrorX, item->patternMaskMirrorY);
			if (item->GrMask == GradMask_Pattern)
				painter->setMaskMode(2);
			else if (item->GrMask == GradMask_PatternLumAlpha)
				painter->setMaskMode(4);
			else if (item->GrMask == GradMask_PatternLumAlphaInverted)
				painter->setMaskMode(5);
			else
				painter->setMaskMode(6);
		}
		else
		{
			painter->setMaskMode(0);
		}
	}
	else
		painter->setMaskMode(0);
}

void ScPageOutput::drawItem_Post(PageItem* item, ScPainterExBase* painter)
{
	bool doStroke = true;
	if (!item->isGroup())
	{
		painter->setMaskMode(0);
		if (item->isGroup() || item->isLine() || item->isPathText() || item->isPolyLine() || item->isSpiral() || item->isSymbol() || item->isTable())
			doStroke = false;
		if (doStroke)
		{
			painter->setBlendModeStroke(item->lineBlendmode());
			painter->setPenOpacity(1.0 - item->lineTransparency());
			if ((item->lineColor() != CommonStrings::None)|| (!item->strokePattern().isEmpty()) || (item->strokeGradientType() > 0))
			{
				ScColorShade tmp(m_doc->PageColors[item->lineColor()], (int) item->lineShade());
				painter->setPen(tmp, item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
				if (item->DashValues.count() != 0)
					painter->setDash(item->DashValues, item->DashOffset);
			}
			else
				painter->setLineWidth(0);
			if ((item->itemType() == PageItem::LatexFrame) || (item->itemType() == PageItem::ImageFrame) || (item->itemType() == PageItem::TextFrame) || (item->itemType() == PageItem::OSGFrame))
				painter->setupPolygon(&item->PoLine);
			if (item->NamedLStyle.isEmpty())
			{
				QString patternStrokeVal = item->strokePattern();
				ScPattern *strokePattern = m_doc->checkedPattern(patternStrokeVal);
				if (strokePattern)
				{
					if (item->patternStrokePath)
					{
						QPainterPath guidePath = item->PoLine.toQPainterPath(false);
						drawStrokePattern(item, painter, guidePath);
					}
					else
					{
						painter->setPattern(strokePattern, item->patternStrokeTransfrm, item->patternStrokeMirrorX, item->patternStrokeMirrorY);
						painter->setStrokeMode(ScPainterExBase::Pattern);
						painter->strokePath();
					}
				}
				else if (item->strokeGradientType() > 0)
				{
					QString gradientStrokeVal = item->strokeGradient();
					if ((!gradientStrokeVal.isEmpty()) && (!m_doc->docGradients.contains(gradientStrokeVal)))
						gradientStrokeVal.clear();
					if (!(gradientStrokeVal.isEmpty()) && (m_doc->docGradients.contains(gradientStrokeVal)))
						painter->m_strokeGradient = VGradientEx(m_doc->docGradients[gradientStrokeVal], *m_doc);
					if (painter->m_strokeGradient.stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
					{
						if (item->lineColor() != CommonStrings::None)
						{
							ScColorShade strokeColor(m_doc->PageColors[item->lineColor()], item->lineShade());
							painter->setBrush(strokeColor);
							painter->setStrokeMode(ScPainterExBase::Solid);
						}
						else
							painter->setStrokeMode(ScPainterExBase::None);
					}
					else
					{
						FPoint fpStart(item->GrStrokeStartX, item->GrStrokeStartY);
						FPoint fpEnd(item->GrStrokeEndX, item->GrStrokeEndY);
						FPoint fpFocal(item->GrStrokeFocalX, item->GrStrokeFocalY);
						painter->setStrokeMode(ScPainterExBase::Gradient);
						painter->m_strokeGradient = VGradientEx(item->stroke_gradient, *m_doc);
						if (item->GrTypeStroke == Gradient_Linear)
							painter->setGradient(VGradientEx::linear, fpStart, fpEnd, fpStart, item->GrStrokeScale, item->GrStrokeSkew);
						else
							painter->setGradient(VGradientEx::radial, fpStart, fpEnd, fpFocal, item->GrStrokeScale, item->GrStrokeSkew);
					}
					painter->strokePath();
				}
				else if (item->lineColor() != CommonStrings::None)
				{
					ScColorShade scColor(m_doc->PageColors[item->lineColor()], item->lineShade());
					painter->setStrokeMode(ScPainterExBase::Solid);
					painter->setPen(scColor, item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
					if (item->DashValues.count() != 0)
						painter->setDash(item->DashValues, item->DashOffset);
					painter->strokePath();
				}
			}
			else
			{
				multiLine ml = m_doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size() - 1; it > -1; it--)
				{
					const SingleLine& sl = ml[it];
					if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
					{
						ScColorShade tmp(m_doc->PageColors[sl.Color], sl.Shade);
						painter->setPen(tmp, sl.Width, static_cast<Qt::PenStyle>(sl.Dash),
										static_cast<Qt::PenCapStyle>(sl.LineEnd),
										static_cast<Qt::PenJoinStyle>(sl.LineJoin));
						painter->strokePath();
					}
				}
			}
			painter->setBlendModeStroke(0);
		}
	}
	painter->setFillMode(ScPainterExBase::Solid);
	painter->setBlendModeFill(0);
	painter->setStrokeMode(ScPainterExBase::Solid);
	painter->setBlendModeStroke(0);
	painter->restore();
}

void ScPageOutput::drawPattern(PageItem* item, ScPainterExBase* painter, const QRect& clip)
{
	const ScPattern& pattern = m_doc->docPatterns[item->pattern()];
	const ScPatternTransform& patternTrans = item->patternTransform();

	// Compute pattern tansformation matrix and its inverse for converting pattern coordinates
	// to pageitem coordinates 
	QTransform matrix;
	matrix.translate(patternTrans.offsetX, patternTrans.offsetY);
	matrix.rotate(patternTrans.rotation);
	matrix.shear(-patternTrans.skewX, patternTrans.skewY);
	matrix.scale(pattern.scaleX, pattern.scaleY);
	matrix.scale(patternTrans.scaleX, patternTrans.scaleY);

	QTransform invMat;
	invMat.scale((patternTrans.scaleX != 0) ? (1.0 / patternTrans.scaleX) : 1.0, (patternTrans.scaleY != 0) ? (1.0 / patternTrans.scaleY) : 1.0);
	invMat.scale((pattern.scaleX != 0) ? (1.0 / pattern.scaleX) : 1.0, (pattern.scaleY != 0) ? (1.0 / pattern.scaleY) : 1.0);
	invMat.shear(patternTrans.skewX, -patternTrans.skewY);
	invMat.rotate(-patternTrans.rotation);
	invMat.translate(-patternTrans.offsetX, -patternTrans.offsetY);

	// Compute bounding box in which pattern item will be drawn
	double width  = item->width();
	double height = item->height();
	double rot    = patternTrans.rotation - floor(patternTrans.rotation / 90) * 90;
	double ctheta = cos(rot * M_PI / 180);
	double stheta = sin(rot * M_PI / 180);
	QRectF  itemRect(0.0, 0.0, item->width(), item->height());
	QPointF pa(width * stheta * stheta, -width * stheta * ctheta);
	QPointF pb(width + height * ctheta * stheta, height * stheta * stheta);
	QPointF pc(-height * ctheta * stheta, height * ctheta * ctheta);
	QPointF pd(width * ctheta * ctheta, height + width * ctheta * stheta);
	QPointF ipa = invMat.map(pa);
	QPointF ipb = invMat.map(pb);
	QPointF ipc = invMat.map(pc);
	QPointF ipd = invMat.map(pd);

	painter->save();
	if (!item->imageClip.empty())
	{
		painter->setupPolygon(&item->imageClip);
		painter->setClipPath();
	}
	painter->setupPolygon(&item->PoLine);
	painter->setClipPath();

	for (int index = 0; index < pattern.items.count(); index++)
	{
		PageItem* it = pattern.items.at(index);

		painter->save();
		painter->translate(patternTrans.offsetX, patternTrans.offsetY);
		painter->rotate(patternTrans.rotation);
		painter->shear(-patternTrans.skewX, patternTrans.skewY);
		painter->scale(pattern.scaleX, pattern.scaleY);
		painter->scale(patternTrans.scaleX, patternTrans.scaleY);

		double patWidth  = (pattern.width != 0.0) ? pattern.width : 1.0;
		double patHeight = (pattern.height != 0.0) ? pattern.height : 1.0;
		double kxa = (ipa.x() - it->gXpos) / patWidth;
		double kxb = (ipb.x() - it->gXpos) / patWidth;
		double kxc = (ipc.x() - it->gXpos) / patWidth;
		double kxd = (ipd.x() - it->gXpos) / patWidth;
		double kya = (ipa.y() - it->gYpos) / patHeight;
		double kyb = (ipb.y() - it->gYpos) / patHeight;
		double kyc = (ipc.y() - it->gYpos) / patHeight;
		double kyd = (ipd.y() - it->gYpos) / patHeight;
		int kxMin  = (int) floor(qMin(qMin(kxa, kxb), qMin(kxc, kxd)));
		int kxMax  = (int) ceil (qMax(qMax(kxa, kxb), qMax(kxc, kxd)));
		int kyMin  = (int) floor(qMin(qMin(kya, kyb), qMin(kyc, kyd)));
		int kyMax  = (int) ceil (qMax(qMax(kya, kyb), qMax(kyc, kyd)));

		double itx = it->xPos();
		double ity = it->yPos();
		double itPosX = it->gXpos;
		double itPosY = it->gYpos;
		for (int kx = kxMin; kx <= kxMax; kx++)
		{
			for (int ky = kyMin; ky <= kyMax; ky++)
			{
				painter->save();
				itPosX = it->gXpos + kx * pattern.width;
				itPosY = it->gYpos + ky * pattern.height;
				it->setXYPos(itPosX, itPosY, true);
				drawItem(it, painter, QRect());
				painter->restore();
			}
		}
		it->setXYPos(itx, ity, true);
		painter->restore();
	}
	painter->restore();
}

void ScPageOutput::drawStrokePattern(PageItem* item, ScPainterExBase* painter, const QPainterPath& path)
{

}

void ScPageOutput::drawItem_Arc(PageItem_Arc* item , ScPainterExBase* painter, const QRect& clip)
{
	painter->setupPolygon(&item->PoLine);
	fillPath(item, painter, clip);
}

void ScPageOutput::drawItem_Group(PageItem_Group* item, ScPainterExBase* painter, const QRect& clip)
{
	if (item->groupItemList.isEmpty())
		return;

	painter->save();

	if (item->imageFlippedH())
	{
		painter->translate(item->width(), 0);
		painter->scale(-1, 1);
	}
	if (item->imageFlippedV())
	{
		painter->translate(0, item->height());
		painter->scale(1, -1);
	}

	if ((item->maskType() == GradMask_Linear) || (item->maskType() == GradMask_Radial) || (item->maskType() == GradMask_LinearLumAlpha) || (item->maskType() == GradMask_RadialLumAlpha))
	{
		QString gradientMask = item->gradientMask();
		FPoint fpMaskStart(item->GrMaskStartX, item->GrMaskStartY);
		FPoint fpMaskEnd(item->GrMaskEndX, item->GrMaskEndY);
		FPoint fpMaskFocal(item->GrMaskFocalX, item->GrMaskFocalY);
		if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_Radial))
			painter->setMaskMode(1);
		else
			painter->setMaskMode(3);
		if ((!gradientMask.isEmpty()) && (!m_doc->docGradients.contains(gradientMask)))
			gradientMask.clear();
		if (!(gradientMask.isEmpty()) && (m_doc->docGradients.contains(gradientMask)))
			painter->m_maskGradient = VGradientEx(m_doc->docGradients[gradientMask], *m_doc);
		else
			painter->m_maskGradient = VGradientEx(item->mask_gradient, *m_doc);
		if ((item->maskType() == GradMask_Linear) || (item->maskType() == GradMask_LinearLumAlpha))
			painter->setGradientMask(VGradientEx::linear, fpMaskStart, fpMaskEnd, fpMaskStart, item->GrMaskScale, item->GrMaskSkew);
		else
			painter->setGradientMask(VGradientEx::radial, fpMaskStart, fpMaskEnd, fpMaskFocal, item->GrMaskScale, item->GrMaskSkew);
	}
	else if ((item->maskType() == GradMask_Pattern) || (item->maskType() == GradMask_PatternLumAlpha) || (item->maskType() == GradMask_PatternLumAlphaInverted) || (item->maskType() == GradMask_PatternInverted))
	{
		QString patternMaskVal = item->patternMask();
		ScPattern* patternMask = m_doc->checkedPattern(patternMaskVal);
		if (patternMask)
		{
			double scw = item->width() / item->groupWidth;
			double sch = item->height() / item->groupHeight;
			ScMaskTransform patternMaskTrans = item->patternMaskTransfrm;
			patternMaskTrans.scaleX *= scw;
			patternMaskTrans.scaleY *= sch;
			painter->setPatternMask(patternMask, patternMaskTrans, item->patternMaskMirrorX, item->patternMaskMirrorY);
			if (item->GrMask == GradMask_Pattern)
				painter->setMaskMode(2);
			else if (item->GrMask == GradMask_PatternLumAlpha)
				painter->setMaskMode(4);
			else if (item->GrMask == GradMask_PatternLumAlphaInverted)
				painter->setMaskMode(5);
			else
				painter->setMaskMode(6);
		}
		else
		{
			painter->setMaskMode(0);
		}
	}
	else
		painter->setMaskMode(0);

	painter->setFillRule(item->fillRule);
	if (item->fillTransparency() != 0.0 || item->fillBlendmode() != 0 || item->maskType() != GradMask_None)
	{
		// Disable this for now, this does not work properly with cairo win32 printing surface
		/*if (item->groupClipping())
			painter->beginLayer(1.0 - item->fillTransparency(), item->fillBlendmode(), &item->PoLine);
		else*/
			painter->beginLayer(1.0 - item->fillTransparency(), item->fillBlendmode());
	}
	painter->setMaskMode(0);
	painter->scale(item->width() / item->groupWidth, item->height() / item->groupHeight);

	for (int em = 0; em < item->groupItemList.count(); ++em)
	{
		PageItem* embedded = item->groupItemList.at(em);
		painter->save();
		painter->translate(embedded->gXpos, embedded->gYpos);
		embedded->savedOwnPage = embedded->OwnPage;
		embedded->OwnPage = item->OwnPage;
		embedded->isEmbedded = true;
		embedded->invalidateLayout();
		drawItem(embedded, painter, QRect());
		embedded->isEmbedded = false;
		embedded->OwnPage = embedded->savedOwnPage;
		painter->restore();
	}
	
	if (item->fillTransparency() != 0.0 || item->fillBlendmode() != 0 || item->maskType() != GradMask_None)
		painter->endLayer();
	painter->restore();
}

void ScPageOutput::drawItem_ImageFrame(PageItem_ImageFrame* item, ScPainterExBase* painter, const QRect& clip)
{
	ScPainterExBase::ImageMode mode = ScPainterExBase::rgbImages;
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		painter->setupPolygon(&item->PoLine);
		fillPath(item, painter, clip);
	}
	if (item->Pfile.isEmpty())
	{
		/*painter->setPen(ScColorShade(Qt::black, 100), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->drawLine(FPoint(0, 0), FPoint(item->width(), item->height()));
		painter->drawLine(FPoint(0, item->height()), FPoint(item->width(), 0));*/
	}
	else if ((!item->imageVisible()) || (!item->imageIsAvailable))
	{
		/*painter->setPen(ScColorShade(Qt::red, 100), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->drawLine(FPoint(0, 0), FPoint(item->width(), item->height()));
		painter->drawLine(FPoint(0, item->height()), FPoint(item->width(), 0));*/
	}
	else
	{
		ScImage scImg;
		ScImage* pImage = nullptr;
		double imScaleX = item->imageXScale();
		double imScaleY = item->imageYScale();
		if (m_reloadImages)
		{
			bool dummy;
			bool useCmyk = false;
			ScPainterExBase::ImageMode imageMode = painter->imageMode();
			if (imageMode == ScPainterExBase::cmykImages)
				useCmyk = true;
			QFileInfo fInfo(item->Pfile);
			QString ext = fInfo.suffix();
			CMSettings cmsSettings(item->doc(), item->ImageProfile, item->ImageIntent);
			cmsSettings.allowColorManagement(m_useProfiles);
			cmsSettings.setUseEmbeddedProfile(item->UseEmbedded);
			scImg.imgInfo.valid = false;
			scImg.imgInfo.clipPath = "";
			scImg.imgInfo.PDSpathData.clear();
			scImg.imgInfo.layerInfo.clear();
			scImg.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
			scImg.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
			scImg.loadPicture(item->Pfile, item->pixm.imgInfo.actualPageNumber, cmsSettings, translateImageModeToRequest(imageMode), m_imageRes, &dummy);
			if (extensionIndicatesEPSorPS(ext) || extensionIndicatesPDF(ext))
			{
				imScaleX *= (PrefsManager::instance().appPrefs.extToolPrefs.gs_Resolution / (double) m_imageRes);
				imScaleY *= (PrefsManager::instance().appPrefs.extToolPrefs.gs_Resolution / (double) m_imageRes);
			}
			scImg.applyEffect(item->effectsInUse, m_doc->PageColors, useCmyk);
			mode = imageMode;
			pImage = &scImg;
		}
		else
			pImage = &item->pixm;

		painter->save();
		if (!item->imageClip.empty())
		{
			painter->setupPolygon(&item->imageClip);
			painter->setClipPath();
		}
		painter->setupPolygon(&item->PoLine);
		painter->setClipPath();
		if (item->imageFlippedH())
		{
			painter->translate(item->width(), 0);
			painter->scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			painter->translate(0, item->height());
			painter->scale(1, -1);
		}
		painter->translate(item->imageXOffset() * item->imageXScale(), item->imageYOffset() * item->imageYScale());
		painter->rotate(item->imageRotation());
		painter->scale(imScaleX, imScaleY);
		if (pImage->imgInfo.lowResType != 0)
			painter->scale(pImage->imgInfo.lowResScale, pImage->imgInfo.lowResScale);
		painter->drawImage(pImage, mode);
		painter->restore();
	}
}

void ScPageOutput::drawItem_Line(PageItem_Line* item, ScPainterExBase* painter, const QRect& clip)
{
	int startArrowIndex = item->startArrowIndex();
	int endArrowIndex = item->endArrowIndex();

	painter->setBlendModeStroke(item->lineBlendmode());

	if (item->NamedLStyle.isEmpty())
	{
		QString patternStrokeVal = item->strokePattern();
		ScPattern *strokePattern = m_doc->checkedPattern(patternStrokeVal);
		if (strokePattern)
		{
			if (item->patternStrokePath)
			{
				QPainterPath guidePath = item->PoLine.toQPainterPath(false);
				guidePath.moveTo(0, 0);
				guidePath.lineTo(item->width(), 0);
				drawStrokePattern(item, painter, guidePath);
			}
			else
			{
				painter->setPattern(strokePattern, item->patternStrokeTransfrm, item->patternStrokeMirrorX, item->patternStrokeMirrorY);
				painter->setStrokeMode(ScPainterExBase::Pattern);
				painter->strokePath();
			}
		}
		else if (item->strokeGradientType() > 0)
		{
			QString gradientStrokeVal = item->strokeGradient();
			if ((!gradientStrokeVal.isEmpty()) && (!m_doc->docGradients.contains(gradientStrokeVal)))
				gradientStrokeVal.clear();
			if (!(gradientStrokeVal.isEmpty()) && (m_doc->docGradients.contains(gradientStrokeVal)))
				painter->m_strokeGradient = VGradientEx(m_doc->docGradients[gradientStrokeVal], *m_doc);
			if (painter->m_strokeGradient.stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
			{
				if (item->lineColor() != CommonStrings::None)
				{
					ScColorShade strokeColor(m_doc->PageColors[item->lineColor()], item->lineShade());
					painter->setBrush(strokeColor);
					painter->setStrokeMode(ScPainterExBase::Solid);
				}
				else
					painter->setStrokeMode(ScPainterExBase::None);
			}
			else
			{
				FPoint fpStart(item->GrStrokeStartX, item->GrStrokeStartY);
				FPoint fpEnd(item->GrStrokeEndX, item->GrStrokeEndY);
				FPoint fpFocal(item->GrStrokeFocalX, item->GrStrokeFocalY);
				painter->setStrokeMode(ScPainterExBase::Gradient);
				painter->m_strokeGradient = VGradientEx(item->stroke_gradient, *m_doc);
				if (item->GrTypeStroke == Gradient_Linear)
					painter->setGradient(VGradientEx::linear, fpStart, fpEnd, fpStart, item->GrStrokeScale, item->GrStrokeSkew);
				else
					painter->setGradient(VGradientEx::radial, fpStart, fpEnd, fpFocal, item->GrStrokeScale, item->GrStrokeSkew);
			}
			painter->drawLine(FPoint(0, 0), FPoint(item->width(), 0));
		}
		else if (item->lineColor() != CommonStrings::None)
		{
			ScColorShade scColor(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setStrokeMode(ScPainterExBase::Solid);
			painter->setPen(scColor, item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
			if (item->DashValues.count() != 0)
				painter->setDash(item->DashValues, item->DashOffset);
			painter->drawLine(FPoint(0, 0), FPoint(item->width(), 0));
		}
	}
	else
	{
		painter->setStrokeMode(ScPainterExBase::Solid);
		multiLine ml = m_doc->docLineStyles[item->NamedLStyle];
		for (int it = ml.size() - 1; it > -1; it--)
		{
			const SingleLine& sl = ml[it];
			if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
			{
				ScColorShade tmp(m_doc->PageColors[sl.Color], sl.Shade);
				painter->setPen(tmp, sl.Width, static_cast<Qt::PenStyle>(sl.Dash),
						static_cast<Qt::PenCapStyle>(sl.LineEnd),
						static_cast<Qt::PenJoinStyle>(sl.LineJoin));
				painter->drawLine(FPoint(0, 0), FPoint(item->width(), 0));
			}
		}
	}

	painter->setBlendModeStroke(0);

	if (startArrowIndex != 0)
	{
		QTransform arrowTrans;
		arrowTrans.translate(0, 0);
		arrowTrans.scale(-1, 1);
		drawArrow(painter, item, arrowTrans, startArrowIndex);
	}
	if (endArrowIndex != 0)
	{
		QTransform arrowTrans;
		arrowTrans.translate(item->width(), 0);
		drawArrow(painter, item, arrowTrans, endArrowIndex);
	}
}

class ScPageOutputPainter: public TextLayoutPainter
{
private:
	PageItem* m_item { nullptr };
	ScPainterExBase* m_painter { nullptr };
	ScPageOutput* m_scpage { nullptr };

	void setupState()
	{
		m_painter->setLineWidth(strokeWidth());
		ScColorShade fill(m_item->doc()->PageColors[fillColor().color], fillColor().shade);
		m_painter->setBrush(fill);
		ScColorShade stroke(m_item->doc()->PageColors[strokeColor().color], strokeColor().shade);
		m_painter->setPen(stroke, strokeWidth(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

		if (matrix() != QTransform())
		{
			m_painter->setWorldMatrix(matrix() * m_painter->worldMatrix());
		}
	}

public:
	ScPageOutputPainter(PageItem* item, ScPainterExBase* painter, ScPageOutput* scpage)
		: m_item(item)
		, m_painter(painter)
		, m_scpage(scpage)
	{}

	void translate(double xp, double yp) override
	{
		TextLayoutPainter::translate(xp, yp);
		m_painter->translate(xp, yp);
	}

	void save() override
	{
		TextLayoutPainter::save();
		m_painter->save();
	}

	void restore() override
	{
		TextLayoutPainter::restore();
		m_painter->restore();
	}

	void setScale(double h, double v) override
	{
		TextLayoutPainter::setScale(h, v);
		m_painter->scale(h, v);
	}

	void drawGlyph(const GlyphCluster& gc) override
	{
		if (gc.isControlGlyphs())
			return;

		m_painter->save();
		bool fr = m_painter->fillRule();
		int fm = m_painter->fillMode();

		m_painter->setFillRule(false);
		m_painter->setFillMode(ScPainterExBase::Solid);

		setupState();

		double sizeFactor = fontSize() / 10.0;
		QVector<FPointArray> outlines = gc.glyphClusterOutline();
		const QList<GlyphLayout>& glyphs = gc.glyphs();
		for (int i = 0; i < glyphs.count(); ++i)
		{
			const FPointArray& outline = outlines.at(i);
			const GlyphLayout& gl = glyphs.at(i);
			m_painter->save();
			m_painter->translate(gl.xoffset, - (fontSize() * gl.scaleV) + gl.yoffset);
			m_painter->scale(gl.scaleH * sizeFactor, gl.scaleV * sizeFactor);
			m_painter->setupPolygon(&outline, true);
			if (outline.size() > 3)
				m_painter->fillPath();
			m_painter->restore();
			m_painter->translate(gl.xadvance * gl.scaleH, 0.0);
		}

		m_painter->setFillMode(fm);
		m_painter->setFillRule(fr);
		m_painter->restore();
	}

	void drawGlyphOutline(const GlyphCluster& gc, bool fill) override
	{
		if (gc.isControlGlyphs())
			return;

		if (fill)
			drawGlyph(gc);

		m_painter->save();
		bool fr = m_painter->fillRule();
		m_painter->setFillRule(false);

		setupState();
		double current_x = 0.0;
		for (const GlyphLayout& gl : gc.glyphs())
		{
			m_painter->save();
			m_painter->translate(gl.xoffset + current_x, -(fontSize() * gc.scaleV()) + gl.yoffset);

			FPointArray outline = font().glyphOutline(gl.glyph);
			double scaleH = gl.scaleH * fontSize() / 10.0;
			double scaleV = gl.scaleV * fontSize() / 10.0;
			QTransform trans;
			trans.scale(scaleH, scaleV);
			outline.map(trans);
			m_painter->setupPolygon(&outline, true);
			if (outline.size() > 3)
			{
				m_painter->setLineWidth(strokeWidth());
				m_painter->setStrokeMode(ScPainterExBase::Solid);
				m_painter->strokePath();
			}
			m_painter->restore();
			current_x += gl.xadvance * gl.scaleH;
		}

		m_painter->setFillRule(fr);
		m_painter->restore();
	}

	void drawLine(const QPointF& start, const QPointF& end) override
	{
		m_painter->save();
		setupState();
		m_painter->setStrokeMode(ScPainterExBase::Solid);
		m_painter->drawLine(start, end);
		m_painter->restore();
	}

	void drawRect(const QRectF& rect) override
	{
		m_painter->save();
		setupState();
		m_painter->setFillMode(ScPainterExBase::Solid);
		m_painter->setStrokeMode(ScPainterExBase::Solid);
		m_painter->drawRect(rect.x(), rect.y(), rect.width(), rect.height());
		m_painter->restore();
	}

	void drawObject(PageItem* embedded) override
	{
		if (!embedded)
			return;

		m_painter->save();

		embedded->invalid = true;
		m_scpage->drawItem(embedded, m_painter, QRect());

		m_painter->restore();
	}

	void drawObjectDecoration(PageItem* item) override {}
};

void ScPageOutput::drawItem_Polygon(PageItem_Polygon* item , ScPainterExBase* painter, const QRect& clip)
{
	painter->setupPolygon(&item->PoLine);
	fillPath(item, painter, clip);
}

void ScPageOutput::drawItem_PolyLine(PageItem_PolyLine* item, ScPainterExBase* painter, const QRect& clip)
{
	if (item->PoLine.size() < 4)
		return;

	int startArrowIndex = item->startArrowIndex();
	int endArrowIndex = item->endArrowIndex();

	painter->setBlendModeStroke(item->lineBlendmode());

	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		FPointArray cli;
		FPoint Start;
		bool firstp = true;
		for (int n = 0; n < item->PoLine.size()-3; n += 4)
		{
			if (firstp)
			{
				Start = item->PoLine.point(n);
				firstp = false;
			}
			if (item->PoLine.isMarker(n))
			{
				if (n >= 2)
				{
					cli.addPoint(item->PoLine.point(n - 2));
					cli.addPoint(item->PoLine.point(n - 2));
					cli.addPoint(Start);
					cli.addPoint(Start);
					cli.setMarker();
				}
				firstp = true;
				continue;
			}
			cli.addPoint(item->PoLine.point(n));
			cli.addPoint(item->PoLine.point(n+1));
			cli.addPoint(item->PoLine.point(n+2));
			cli.addPoint(item->PoLine.point(n+3));
		}
		if (cli.size() > 2)
		{
			FPoint l1 = cli.point(cli.size()-2);
			cli.addPoint(l1);
			cli.addPoint(l1);
			cli.addPoint(Start);
			cli.addPoint(Start);
		}
		painter->setupPolygon(&cli);
		fillPath(item, painter, clip);
	}
	painter->setupPolygon(&item->PoLine, false);
	if (item->NamedLStyle.isEmpty())
	{
		QString patternStrokeVal = item->strokePattern();
		ScPattern *strokePattern = m_doc->checkedPattern(patternStrokeVal);
		if (strokePattern)
		{
			if (item->patternStrokePath)
			{
				QPainterPath guidePath = item->PoLine.toQPainterPath(false);
				guidePath.moveTo(0, 0);
				guidePath.lineTo(item->width(), 0);
				drawStrokePattern(item, painter, guidePath);
			}
			else
			{
				painter->setPattern(strokePattern, item->patternStrokeTransfrm, item->patternStrokeMirrorX, item->patternStrokeMirrorY);
				painter->setStrokeMode(ScPainterExBase::Pattern);
				painter->strokePath();
			}
		}
		else if (item->strokeGradientType() > 0)
		{
			QString gradientStrokeVal = item->strokeGradient();
			if ((!gradientStrokeVal.isEmpty()) && (!m_doc->docGradients.contains(gradientStrokeVal)))
				gradientStrokeVal.clear();
			if (!(gradientStrokeVal.isEmpty()) && (m_doc->docGradients.contains(gradientStrokeVal)))
				painter->m_strokeGradient = VGradientEx(m_doc->docGradients[gradientStrokeVal], *m_doc);
			if (painter->m_strokeGradient.stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
			{
				if (item->lineColor() != CommonStrings::None)
				{
					ScColorShade strokeColor(m_doc->PageColors[item->lineColor()], item->lineShade());
					painter->setBrush(strokeColor);
					painter->setStrokeMode(ScPainterExBase::Solid);
				}
				else
					painter->setStrokeMode(ScPainterExBase::None);
			}
			else
			{
				FPoint fpStart(item->GrStrokeStartX, item->GrStrokeStartY);
				FPoint fpEnd(item->GrStrokeEndX, item->GrStrokeEndY);
				FPoint fpFocal(item->GrStrokeFocalX, item->GrStrokeFocalY);
				painter->setStrokeMode(ScPainterExBase::Gradient);
				painter->m_strokeGradient = VGradientEx(item->stroke_gradient, *m_doc);
				if (item->GrTypeStroke == Gradient_Linear)
					painter->setGradient(VGradientEx::linear, fpStart, fpEnd, fpStart, item->GrStrokeScale, item->GrStrokeSkew);
				else
					painter->setGradient(VGradientEx::radial, fpStart, fpEnd, fpFocal, item->GrStrokeScale, item->GrStrokeSkew);
			}
			painter->strokePath();
		}
		else if (item->lineColor() != CommonStrings::None)
		{
			ScColorShade scColor(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setStrokeMode(ScPainterExBase::Solid);
			painter->setPen(scColor, item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
			if (item->DashValues.count() != 0)
				painter->setDash(item->DashValues, item->DashOffset);
			painter->strokePath();
		}
	}
	else
	{
		multiLine ml = m_doc->docLineStyles[item->NamedLStyle];
		for (int it = ml.size() - 1; it > -1; it--)
		{
			const SingleLine& sl = ml[it];
			if (sl.Color != CommonStrings::None)
			{
				ScColorShade tmp(m_doc->PageColors[sl.Color], sl.Shade);
				painter->setPen(tmp, sl.Width, static_cast<Qt::PenStyle>(sl.Dash),
						static_cast<Qt::PenCapStyle>(sl.LineEnd),
						static_cast<Qt::PenJoinStyle>(sl.LineJoin));
				painter->strokePath();
			}
		}
	}

	painter->setBlendModeStroke(0);

	if (startArrowIndex != 0)
	{
		FPoint Start = item->PoLine.point(0);
		for (int xx = 1; xx < item->PoLine.size(); xx += 2)
		{
			FPoint Vector = item->PoLine.point(xx);
			if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
			{
				double r = atan2(Start.y() - Vector.y(), Start.x() - Vector.x()) * (180.0 / M_PI);
				QTransform arrowTrans;
				arrowTrans.translate(Start.x(), Start.y());
				arrowTrans.rotate(r);
				drawArrow(painter, item, arrowTrans, startArrowIndex);
				break;
			}
		}
	}
	if (endArrowIndex != 0)
	{
		FPoint End = item->PoLine.point(item->PoLine.size()-2);
		for (uint xx = item->PoLine.size() - 1; xx > 0; xx -= 2)
		{
			FPoint Vector = item->PoLine.point(xx);
			if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
			{
				double r = atan2(End.y() - Vector.y(), End.x() - Vector.x()) * (180.0 / M_PI);
				QTransform arrowTrans;
				arrowTrans.translate(End.x(), End.y());
				arrowTrans.rotate(r);
				drawArrow(painter, item, arrowTrans, endArrowIndex);
				break;
			}
		}
	}
}

void ScPageOutput::drawItem_RegularPolygon(PageItem_RegularPolygon* item, ScPainterExBase* painter, const QRect& clip)
{
	painter->setupPolygon(&item->PoLine);
	painter->fillPath();
}

void ScPageOutput::drawItem_Spiral(PageItem_Spiral* item, ScPainterExBase* painter, const QRect& clip)
{
	if (item->PoLine.size() < 4)
		return;

	int startArrowIndex = item->startArrowIndex();
	int endArrowIndex = item->endArrowIndex();

	painter->setBlendModeStroke(item->lineBlendmode());

	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		FPointArray cli;
		FPoint Start;
		bool firstp = true;
		for (int n = 0; n < item->PoLine.size()-3; n += 4)
		{
			if (firstp)
			{
				Start = item->PoLine.point(n);
				firstp = false;
			}
			if (item->PoLine.isMarker(n))
			{
				cli.addPoint(item->PoLine.point(n-2));
				cli.addPoint(item->PoLine.point(n-2));
				cli.addPoint(Start);
				cli.addPoint(Start);
				cli.setMarker();
				firstp = true;
				continue;
			}
			cli.addPoint(item->PoLine.point(n));
			cli.addPoint(item->PoLine.point(n+1));
			cli.addPoint(item->PoLine.point(n+2));
			cli.addPoint(item->PoLine.point(n+3));
		}
		if (cli.size() > 2)
		{
			FPoint l1 = cli.point(cli.size()-2);
			cli.addPoint(l1);
			cli.addPoint(l1);
			cli.addPoint(Start);
			cli.addPoint(Start);
		}
		painter->setupPolygon(&cli);
		fillPath(item, painter, clip);
	}
	painter->setupPolygon(&item->PoLine, false);
	if (item->NamedLStyle.isEmpty())
	{
		QString patternStrokeVal = item->strokePattern();
		ScPattern *strokePattern = m_doc->checkedPattern(patternStrokeVal);
		if (strokePattern)
		{
			if (item->patternStrokePath)
			{
				QPainterPath guidePath = item->PoLine.toQPainterPath(false);
				guidePath.moveTo(0, 0);
				guidePath.lineTo(item->width(), 0);
				drawStrokePattern(item, painter, guidePath);
			}
			else
			{
				painter->setPattern(strokePattern, item->patternStrokeTransfrm, item->patternStrokeMirrorX, item->patternStrokeMirrorY);
				painter->setStrokeMode(ScPainterExBase::Pattern);
				painter->strokePath();
			}
		}
		else if (item->strokeGradientType() > 0)
		{
			QString gradientStrokeVal = item->strokeGradient();
			if ((!gradientStrokeVal.isEmpty()) && (!m_doc->docGradients.contains(gradientStrokeVal)))
				gradientStrokeVal.clear();
			if (!(gradientStrokeVal.isEmpty()) && (m_doc->docGradients.contains(gradientStrokeVal)))
				painter->m_strokeGradient = VGradientEx(m_doc->docGradients[gradientStrokeVal], *m_doc);
			if (painter->m_strokeGradient.stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
			{
				if (item->lineColor() != CommonStrings::None)
				{
					ScColorShade strokeColor(m_doc->PageColors[item->lineColor()], item->lineShade());
					painter->setBrush(strokeColor);
					painter->setStrokeMode(ScPainterExBase::Solid);
				}
				else
					painter->setStrokeMode(ScPainterExBase::None);
			}
			else
			{
				FPoint fpStart(item->GrStrokeStartX, item->GrStrokeStartY);
				FPoint fpEnd(item->GrStrokeEndX, item->GrStrokeEndY);
				FPoint fpFocal(item->GrStrokeFocalX, item->GrStrokeFocalY);
				painter->setStrokeMode(ScPainterExBase::Gradient);
				painter->m_strokeGradient = VGradientEx(item->stroke_gradient, *m_doc);
				if (item->GrTypeStroke == Gradient_Linear)
					painter->setGradient(VGradientEx::linear, fpStart, fpEnd, fpStart, item->GrStrokeScale, item->GrStrokeSkew);
				else
					painter->setGradient(VGradientEx::radial, fpStart, fpEnd, fpFocal, item->GrStrokeScale, item->GrStrokeSkew);
			}
			painter->strokePath();
		}
		else if (item->lineColor() != CommonStrings::None)
		{
			ScColorShade scColor(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setStrokeMode(ScPainterExBase::Solid);
			painter->setPen(scColor, item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
			if (item->DashValues.count() != 0)
				painter->setDash(item->DashValues, item->DashOffset);
			painter->strokePath();
		}
	}
	else
	{
		multiLine ml = m_doc->docLineStyles[item->NamedLStyle];
		for (int it = ml.size() - 1; it > -1; it--)
		{
			const SingleLine& sl = ml[it];
			if (sl.Color != CommonStrings::None)
			{
				ScColorShade tmp(m_doc->PageColors[sl.Color], sl.Shade);
				painter->setPen(tmp, sl.Width, static_cast<Qt::PenStyle>(sl.Dash),
						static_cast<Qt::PenCapStyle>(sl.LineEnd),
						static_cast<Qt::PenJoinStyle>(sl.LineJoin));
				painter->strokePath();
			}
		}
	}

	painter->setBlendModeStroke(0);

	if (startArrowIndex != 0)
	{
		FPoint Start = item->PoLine.point(0);
		for (int xx = 1; xx < item->PoLine.size(); xx += 2)
		{
			FPoint Vector = item->PoLine.point(xx);
			if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
			{
				double r = atan2(Start.y() - Vector.y(), Start.x() - Vector.x()) * (180.0 / M_PI);
				QTransform arrowTrans;
				arrowTrans.translate(Start.x(), Start.y());
				arrowTrans.rotate(r);
				drawArrow(painter, item, arrowTrans, startArrowIndex);
				break;
			}
		}
	}
	if (endArrowIndex != 0)
	{
		FPoint End = item->PoLine.point(item->PoLine.size()-2);
		for (uint xx = item->PoLine.size() - 1; xx > 0; xx -= 2)
		{
			FPoint Vector = item->PoLine.point(xx);
			if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
			{
				double r = atan2(End.y() - Vector.y(), End.x() - Vector.x()) * (180.0 / M_PI);
				QTransform arrowTrans;
				arrowTrans.translate(End.x(), End.y());
				arrowTrans.rotate(r);
				drawArrow(painter, item, arrowTrans, endArrowIndex);
				break;
			}
		}
	}
}

void ScPageOutput::drawItem_Table(PageItem_Table* item, ScPainterExBase* painter, const QRect& clip)
{
	painter->save();

	// Set the clip path.
	painter->setupPolygon(&item->PoLine);
	painter->setClipPath();

	// Paint the table.
	CollapsedTablePainterEx tablePainter(this, item);
	tablePainter.paintTable(painter);

	painter->restore();
}

void ScPageOutput::drawItem_Text(PageItem* item, ScPainterExBase* painter, const QRect& clip)
{
	if (item->pathTextShowFrame())
	{
		painter->setupPolygon(&item->PoLine, false);
		if (item->NamedLStyle.isEmpty())
		{
			if (item->lineColor() != CommonStrings::None)
				painter->strokePath();
		}
		else
		{
			multiLine ml = m_doc->docLineStyles[item->NamedLStyle];
			for (int it = ml.size() - 1; it > -1; it--)
			{
				const SingleLine& sl = ml[it];
				if ((sl.Color != CommonStrings::None) && (sl.Width != 0))
				{
					ScColorShade tmp(m_doc->PageColors[sl.Color], sl.Shade);
					painter->setPen(tmp, sl.Width,  static_cast<Qt::PenStyle>(sl.Dash), 
							 static_cast<Qt::PenCapStyle>(sl.LineEnd), 
							 static_cast<Qt::PenJoinStyle>(sl.LineJoin));
					painter->drawLine(FPoint(0, 0), FPoint(item->width(), 0));
				}
			}
		}
	}
	painter->save();

	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		painter->setupPolygon(&item->PoLine);
		fillPath(item, painter, clip);
	}
	if ((item->isAnnotation()) && (item->annotation().Type() == Annotation::Button) && (!item->Pfile.isEmpty()) && (item->imageIsAvailable) && (item->imageVisible()) && (item->annotation().UseIcons()))
	{
		painter->save();
		painter->setupPolygon(&item->PoLine);
		painter->setClipPath();
		painter->scale(item->imageXScale(), item->imageYScale());
		painter->translate(static_cast<int>(item->imageXOffset() * item->imageXScale()), static_cast<int>(item->imageYOffset()  * item->imageYScale()));
		if (!item->pixm.qImage().isNull())
			painter->drawImage(&item->pixm, ScPainterExBase::rgbImages);
		painter->restore();
	}

	if (item->isMasterItem())
	{
		// relayout necessary to get page number ok
		item->invalidateLayout();
		item->layout();
	}

	ScPageOutputPainter p(item, painter, this);
	item->textLayout.renderBackground(&p);
	item->textLayout.render(&p);

	painter->restore();
}

void ScPageOutput::drawArrow(ScPainterExBase* painter, PageItem* item, QTransform &arrowTrans, int arrowIndex)
{
	FPointArray arrow = m_doc->arrowStyles().at(arrowIndex - 1).points.copy();
	if (item->NamedLStyle.isEmpty())
	{
		if (item->lineWidth() != 0.0)
			arrowTrans.scale(item->lineWidth(), item->lineWidth());
	}
	else
	{
		multiLine ml = m_doc->docLineStyles[item->NamedLStyle];
		if (ml[ml.size() - 1].Width != 0.0)
			arrowTrans.scale(ml[ml.size() - 1].Width, ml[ml.size() - 1].Width);
	}
	arrow.map(arrowTrans);

	painter->setBlendModeStroke(item->lineBlendmode());
	painter->setBlendModeFill(item->lineBlendmode()); // needed for fill in arrow shapes

	painter->setupPolygon(&arrow);
	if (item->NamedLStyle.isEmpty())
	{
		if (item->lineColor() != CommonStrings::None)
		{
			ScColorShade tmp(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setBrush(tmp);
			painter->setBrushOpacity(1.0 - item->lineTransparency());
			painter->setLineWidth(0);
			painter->setFillMode(ScPainterExBase::Solid);
			painter->fillPath();
		}
	}
	else
	{
		multiLine ml = m_doc->docLineStyles[item->NamedLStyle];
		if (ml[0].Color != CommonStrings::None)
		{
			ScColorShade tmp(m_doc->PageColors[ml[0].Color], ml[0].Shade);
			painter->setBrush(tmp);
			painter->setLineWidth(0);
			painter->setFillMode(ScPainterExBase::Solid);
			painter->fillPath();
		}
		for (int it = ml.size() - 1; it > 0; it--)
		{
			if (ml[it].Color != CommonStrings::None)
			{
				ScColorShade tmp(m_doc->PageColors[ml[it].Color], ml[it].Shade);
				painter->setPen(tmp, ml[it].Width, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				painter->strokePath();
			}
		}
	}

	painter->setBlendModeStroke(0);
	painter->setBlendModeFill(0);
}

void ScPageOutput::drawMarks(ScPage* page, ScPainterExBase* painter, const MarksOptions& options)
{
	double markLength = options.markLength;
	double markOffs   = options.markOffset;
	double bleedLeft = 0.0, bleedRight = 0.0;
	double bleedBottom = options.bleedBottom;
	double bleedTop = options.bleedTop;
	if (!options.cropMarks && !options.bleedMarks && !options.registrationMarks && !options.colorMarks)
		return;
	if (m_doc->locationOfPage(page->pageNr()) == LeftPage)
	{
		bleedRight = options.bleedRight;
		bleedLeft  = options.bleedLeft;
	}
	else if (m_doc->locationOfPage(page->pageNr()) == RightPage)
	{
		bleedRight = options.bleedLeft;
		bleedLeft  = options.bleedRight;
	}
	else
	{
		bleedRight = options.bleedLeft;
		bleedLeft  = options.bleedLeft;
	}
	double width = page->width();
	double height = page->height();
	double offsetX = page->xOffset();
	double offsetY = page->yOffset();
	QPointF bleedTopLeft(offsetX - bleedLeft, offsetY - bleedTop);
	QPointF bleedBottomRight(offsetX + width + bleedRight, offsetY + height + bleedBottom);
	QRectF  bleedBox(bleedTopLeft, bleedBottomRight);
	painter->save();
	painter->setLineWidth(0.5);
	painter->setPen(ScColorShade(Qt::black, 100), 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	if (options.cropMarks)
	{
		double left = offsetX, right = offsetX + width;
		double bottom = offsetY + height, top = offsetY;
		drawBoxMarks(painter, QRectF(QPointF(left, top), QPointF(right, bottom)), bleedBox, markOffs, markLength);
	}
	if (options.bleedMarks)
	{
		double left = offsetX - bleedLeft, right = offsetX + width + bleedRight;
		double bottom = offsetY + height + bleedBottom, top = offsetY - bleedTop;
		drawBoxMarks(painter, QRectF(QPointF(left, top), QPointF(right, bottom)), bleedBox, markOffs, markLength);
	}
	if (options.registrationMarks)
	{
		double posX = (2* offsetX + width) / 2.0 - 7.0;
		double posY = (offsetY + height + bleedBottom + markOffs + 3.0);
		painter->save();
		painter->translate(posX, posY);
		drawRegistrationCross(painter);
		painter->restore();
		posX = (2 * offsetX + width) / 2.0 - 7.0;
		posY = (offsetY - bleedTop - markOffs - 17);
		painter->save();
		painter->translate(posX, posY);
		drawRegistrationCross(painter);
		painter->restore();

		posX = (offsetX - bleedLeft - markOffs - 17);
		posY = (2 * offsetY + height) / 2.0 - 7.0;
		painter->save();
		painter->translate(posX, posY);
		drawRegistrationCross(painter);
		painter->restore();
		posX = (offsetX + width + bleedRight + markOffs + 3.0);
		posY = (2 * offsetY + height) / 2.0 - 7.0;
		painter->save();
		painter->translate(posX, posY);
		drawRegistrationCross(painter);
		painter->restore();
	}
	if (options.colorMarks)
	{
		int shade = 100;
		double startX = offsetX + 6.0;
		double startY = offsetY - bleedTop - markOffs - 16.0;
		ScColorShade strokecolor(ScColor(0, 0, 0, 255), 100);
		painter->setPen(strokecolor, 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->setFillMode(ScPainterExBase::Solid);
		for (int i = 0; i < 11; i++)
		{
			ScColorShade fillcolor(ScColor(0, 0, 0, 255), shade);
			painter->setBrush(fillcolor);
			painter->drawRect(startX + i * 14, startY, 14, 14);
			shade -= 10;
		}
		startX = offsetX + width - 20.0;
		painter->setBrush(ScColorShade(ScColor(0, 0, 0, 255), 50));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(0, 0, 255, 0), 50));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(0, 255, 0, 0), 50));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(255, 0, 0, 0), 50));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(255, 255, 0, 0), 100));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(255, 0, 255, 0), 100));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(0, 255, 255, 0), 100));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(0, 0, 0, 255), 100));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(0, 0, 255, 0), 100));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(0, 255, 0, 0), 100));
		painter->drawRect(startX, startY, 14, 14);
		startX -= 14;
		painter->setBrush(ScColorShade(ScColor(255, 0, 0, 0), 100));
		painter->drawRect(startX, startY, 14, 14);
	}
	painter->restore();
}

void ScPageOutput::drawBoxMarks(ScPainterExBase* painter, const QRectF& box, const QRectF& bleedBox, double offset , double markSize)
{
	FPoint start, end;
	double left = box.left();
	double right = box.right();
	double bottom = box.bottom();
	double top = box.top();
	double bleedLeft = bleedBox.left();
	double bleedRight = bleedBox.right();
	double bleedBottom = bleedBox.bottom();
	double bleedTop = bleedBox.top();
	// Top Left
	start.setXY(bleedLeft - offset, top);
	end.setXY  (bleedLeft - offset - markSize, top);
	painter->drawLine(start, end);
	start.setXY(left, bleedTop - offset);
	end.setXY  (left, bleedTop - offset - markSize);
	painter->drawLine(start, end);
	// Top Right
	start.setXY(bleedRight + offset, top);
	end.setXY  (bleedRight + offset + markSize, top);
	painter->drawLine(start, end);
	start.setXY(right, bleedTop - offset);
	end.setXY  (right, bleedTop - offset - markSize);
	painter->drawLine(start, end);
	// Bottom Left
	start.setXY(bleedLeft - offset, bottom);
	end.setXY  (bleedLeft - offset - markSize, bottom);
	painter->drawLine(start, end);
	start.setXY(left, bleedBottom + offset);
	end.setXY  (left, bleedBottom + offset + markSize);
	painter->drawLine(start, end);
	// Bottom Right
	start.setXY(bleedRight + offset, bottom);
	end.setXY  (bleedRight + offset + markSize, bottom);
	painter->drawLine(start, end);
	start.setXY(right, bleedBottom + offset);
	end.setXY  (right, bleedBottom + offset + markSize);
	painter->drawLine(start, end);
}

void ScPageOutput::drawRegistrationCross(ScPainterExBase* painter)
{
	painter->save();

	painter->newPath();
	painter->moveTo(0.0, 7.0);
	painter->lineTo(14.0, 7.0);
	painter->strokePath();

	painter->newPath();
	painter->moveTo(7.0, 0.0);
	painter->lineTo(7.0, 14.0);
	painter->strokePath();

	painter->newPath();
	painter->moveTo(13.0, 7.0);
	painter->curveTo(FPoint(13.0, 10.31383), FPoint(10.31383, 13.0), FPoint(7.0, 13.0));
	painter->curveTo(FPoint(3.68629, 13.0) , FPoint(1.0, 10.31383) , FPoint(1.0, 7.0));
	painter->curveTo(FPoint(1.0, 3.68629)  , FPoint(3.68629, 1.0) , FPoint(7.0, 1.0));
	painter->curveTo(FPoint(10.31383, 1.0) , FPoint(13.0, 3.68629) , FPoint(13.0, 7.0));
	painter->strokePath();

	painter->newPath();
	painter->moveTo(10.5, 7.0);
	painter->curveTo(FPoint(10.5, 8.93307), FPoint(8.93307, 10.5), FPoint(7.0, 10.5));
	painter->curveTo(FPoint(5.067, 10.5)  , FPoint(3.5, 8.93307) , FPoint(3.5, 7.0));
	painter->curveTo(FPoint(3.5, 5.067)   , FPoint(5.067, 3.5)   , FPoint(7.0, 3.5));
	painter->curveTo(FPoint(8.93307, 3.5) , FPoint(10.5, 5.067)  , FPoint(10.5, 7.0));
	painter->strokePath();

	painter->restore();
}

void ScPageOutput::fillPath(PageItem* item, ScPainterExBase* painter, const QRect& clip)
{
	if (painter->fillMode() == ScPainterExBase::Pattern && !painter->hasCapability(ScPainterExBase::patterns))
		drawPattern(item, painter, clip);
	else
		painter->fillPath();
}

void ScPageOutput::strokePath(PageItem* item, ScPainterExBase* painter, const QRect& clip)
{
	painter->strokePath();
}
