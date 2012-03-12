#include "slaoutput.h"
#include <GlobalParams.h>
#include <QApplication>
#include <QFile>
#include "commonstrings.h"
#include "loadsaveplugin.h"
#include "sccolorengine.h"
#include "util.h"
#include "util_math.h"


SlaOutputDev::SlaOutputDev(ScribusDoc* doc, QList<PageItem*> *Elements, QStringList *importedColors, int flags)
{
	m_doc = doc;
	m_Elements = Elements;
	m_groupStack.clear();
	pushGroup();
	m_clipStack.clear();
	m_currentMask = "";
	m_importedColors = importedColors;
	CurrColorStroke = "Black";
	CurrColorFill = "Black";
	Coords = "";
	pathIsClosed = false;
	tmpSel = new Selection(m_doc, false);
	grStackDepth = 0;
	firstLayer = true;
	layerNum = 1;
	importerFlags = flags;
	currentLayer = m_doc->activeLayer();
	xref = NULL;
	m_fontEngine = 0;
	m_font = 0;
	firstPage = true;
	pagecount = 1;
	updateGUICounter = 0;
}

SlaOutputDev::~SlaOutputDev()
{
	m_groupStack.clear();
	tmpSel->clear();
	delete tmpSel;
	delete m_fontEngine;
}


void SlaOutputDev::startDoc(XRef *xrefA, Catalog *catA)
{
	xref = xrefA;
	catalog = catA;
	firstPage = true;
	pagecount = 1;
	updateGUICounter = 0;
	m_fontEngine = new SplashFontEngine(
#if HAVE_T1LIB_H
	globalParams->getEnableT1lib(),
#endif
#if HAVE_FREETYPE_H
	globalParams->getEnableFreeType(),
	true,
	true,
#endif
	true);
}

void SlaOutputDev::startPage(int pageNum, GfxState *state)
{
//    qDebug() << "starting page" << pageNum;
	if (firstPage)
		firstPage = false;
	else
	{
		if (importerFlags & LoadSavePlugin::lfCreateDoc)
		{
			m_doc->addPage(pagecount);
			m_doc->currentPage()->setInitialHeight(state->getPageHeight());
			m_doc->currentPage()->setInitialWidth(state->getPageWidth());
			m_doc->currentPage()->setHeight(state->getPageHeight());
			m_doc->currentPage()->setWidth(state->getPageWidth());
			m_doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
			m_doc->currentPage()->m_pageSize = "Custom";
			m_doc->view()->addPage(pagecount, true);
			pagecount++;
		}
	}
 //   qDebug() << "page size =" << pageSize;
}

void SlaOutputDev::endPage()
{
//	qDebug() << "ending page";
}

void SlaOutputDev::saveState(GfxState *state)
{
//	qDebug() << "saveState" << grStackDepth;
	grStackDepth++;
}

void SlaOutputDev::restoreState(GfxState *state)
{
//	qDebug() << "restoreState" << grStackDepth;
	grStackDepth--;
	if (m_groupStack.count() != 0)
		m_groupStack.top().maskName = "";
	if (m_clipStack.count() != 0)
	{
		while (m_clipStack.top().grStackDepth > grStackDepth)
		{
			clipEntry clp = m_clipStack.pop();
			PageItem *ite = clp.ClipItem;
			if (m_groupStack.count() != 0)
			{
				groupEntry gElements = m_groupStack.pop();
				if (gElements.Items.count() > 0)
				{
					for (int d = 0; d < gElements.Items.count(); d++)
					{
						m_Elements->removeAll(gElements.Items.at(d));
					}
					m_doc->groupObjectsToItem(ite, gElements.Items);
					ite->setFillTransparency(1.0 - state->getFillOpacity());
					ite->setFillBlendmode(getBlendMode(state));
				}
				else
				{
					m_Elements->removeAll(ite);
					m_doc->Items->removeAll(ite);
					m_groupStack.top().Items.removeAll(ite);
				}
			}
			if (m_clipStack.count() == 0)
				break;
		}
	}
}
/*
void SlaOutputDev::updateFillColor(GfxState *state)
{
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
}
*/
void SlaOutputDev::beginTransparencyGroup(GfxState *state, double *bbox, GfxColorSpace * /*blendingColorSpace*/, GBool /*isolated*/, GBool /*knockout*/, GBool forSoftMask)
{
//	qDebug() << "Start Group";
	pushGroup("", forSoftMask);
}

void SlaOutputDev::endTransparencyGroup(GfxState *state)
{
//	qDebug() << "End Group";
	if (m_groupStack.count() != 0)
	{
		groupEntry gElements = m_groupStack.pop();
		tmpSel->clear();
		if (gElements.Items.count() > 0)
		{
			for (int dre = 0; dre < gElements.Items.count(); ++dre)
			{
				tmpSel->addItem(gElements.Items.at(dre), true);
				m_Elements->removeAll(gElements.Items.at(dre));
			}
			PageItem *ite = m_doc->groupObjectsSelection(tmpSel);
			ite->setFillTransparency(1.0 - state->getFillOpacity());
			ite->setFillBlendmode(getBlendMode(state));
			if (gElements.forSoftMask)
			{
				ScPattern pat = ScPattern();
				pat.setDoc(m_doc);
				m_doc->DoDrawing = true;
				pat.pattern = ite->DrawObj_toImage(qMax(ite->width(), ite->height()));
				pat.xoffset = 0;
				pat.yoffset = 0;
				m_doc->DoDrawing = false;
				pat.width = ite->width();
				pat.height = ite->height();
				ite->gXpos = 0;
				ite->gYpos = 0;
				ite->setXYPos(ite->gXpos, ite->gYpos, true);
				pat.items.append(ite);
				m_doc->Items->removeAll(ite);
				QString id = QString("Pattern_from_PDF_%1S").arg(m_doc->docPatterns.count() + 1);
				m_doc->addPattern(id, pat);
				m_currentMask = id;
				tmpSel->clear();
				return;
			}
			else
			{
				for (int as = 0; as < tmpSel->count(); ++as)
				{
					m_Elements->append(tmpSel->itemAt(as));
				}
			}
			if (m_groupStack.count() != 0)
				applyMask(ite);
		}
		if (m_groupStack.count() != 0)
		{
			for (int as = 0; as < tmpSel->count(); ++as)
			{
				m_groupStack.top().Items.append(tmpSel->itemAt(as));
			}
		}
		tmpSel->clear();
	}
}

void SlaOutputDev::setSoftMask(GfxState * /*state*/, double * /*bbox*/, GBool alpha, Function *transferFunc, GfxColor * /*backdropColor*/)
{
	if (m_groupStack.count() != 0)
	{
		double lum = 0;
		double lum2 = 0;
		if (transferFunc)
			transferFunc->transform(&lum, &lum2);
		else
			lum2 = lum;
		if (lum == lum2)
			m_groupStack.top().inverted = false;
		else
			m_groupStack.top().inverted = true;
//		qDebug() << "Inverted Softmask" << m_groupStack.top().inverted;
		m_groupStack.top().maskName = m_currentMask;
		m_groupStack.top().alpha = alpha;
	}
}

void SlaOutputDev::clearSoftMask(GfxState * /*state*/)
{
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().maskName = "";
	}
}

void SlaOutputDev::clip(GfxState *state)
{
//	qDebug() << "Clip";
	double *ctm;
	ctm = state->getCTM();
	QString output = convertPath(state->getPath());
	FPointArray out;
	out.parseSVG(output);
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	out.map(m_ctm);
	double xmin, ymin, xmax, ymax;
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	int z = m_doc->itemAdd(PageItem::Group, PageItem::Rectangle, crect.x() + m_doc->currentPage()->xOffset(), crect.y() + m_doc->currentPage()->yOffset(), crect.width(), crect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *ite = m_doc->Items->at(z);
	FPoint wh(getMinClipF(&out));
	out.translate(-wh.x(), -wh.y());
	ite->PoLine = out.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillEvenOdd(false);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->ContourLine = ite->PoLine.copy();
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	clipEntry clp;
	clp.ClipCoords = out.copy();
	clp.ClipItem = ite;
	clp.grStackDepth = grStackDepth;
	m_clipStack.push(clp);
	m_doc->GroupCounter++;
	pushGroup();
}

void SlaOutputDev::eoClip(GfxState *state)
{
//	qDebug() << "EoClip";
	double *ctm;
	ctm = state->getCTM();
	QString output = convertPath(state->getPath());
	FPointArray out;
	out.parseSVG(output);
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	out.map(m_ctm);
	double xmin, ymin, xmax, ymax;
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	int z = m_doc->itemAdd(PageItem::Group, PageItem::Rectangle, crect.x() + m_doc->currentPage()->xOffset(), crect.y() + m_doc->currentPage()->yOffset(), crect.width(), crect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *ite = m_doc->Items->at(z);
	FPoint wh(getMinClipF(&out));
	out.translate(-wh.x(), -wh.y());
	ite->PoLine = out.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillEvenOdd(true);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->ContourLine = ite->PoLine.copy();
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	clipEntry clp;
	clp.ClipCoords = out.copy();
	clp.ClipItem = ite;
	clp.grStackDepth = grStackDepth;
	m_clipStack.push(clp);
	m_doc->GroupCounter++;
	pushGroup();
}

void SlaOutputDev::stroke(GfxState *state)
{
//	qDebug() << "Stroke";
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	int shade = 100;
	CurrColorStroke = getColor(state->getStrokeColorSpace(), state->getStrokeColor(), &shade);
	QString output = convertPath(state->getPath());
	getPenState(state);
	if ((m_Elements->count() != 0) && (output == Coords))			// Path is the same as in last fill
	{
		PageItem* ite = m_Elements->last();
		ite->setLineColor(CurrColorStroke);
		ite->setLineShade(shade);
		ite->setLineEnd(PLineEnd);
		ite->setLineJoin(PLineJoin);
		ite->setLineWidth(state->getTransformedLineWidth());
		ite->setDashes(DashValues);
		ite->setDashOffset(DashOffset);
		ite->setLineTransparency(1.0 - state->getStrokeOpacity());
	}
	else
	{
		FPointArray out;
		out.parseSVG(output);
		m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
		out.map(m_ctm);
		FPoint wh = out.WidthHeight();
		if ((out.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
		{
			int z;
			if (pathIsClosed)
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, state->getTransformedLineWidth(), CommonStrings::None, CurrColorStroke, true);
			else
				z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, xCoor, yCoor, 10, 10, state->getTransformedLineWidth(), CommonStrings::None, CurrColorStroke, true);
			PageItem* ite = m_doc->Items->at(z);
			ite->PoLine = out.copy();
			ite->ClipEdited = true;
			ite->FrameType = 3;
			ite->setLineShade(shade);
			ite->setLineTransparency(1.0 - state->getStrokeOpacity());
			ite->setLineBlendmode(getBlendMode(state));
			ite->setLineEnd(PLineEnd);
			ite->setLineJoin(PLineJoin);
			ite->setDashes(DashValues);
			ite->setDashOffset(DashOffset);
			ite->setWidthHeight(wh.x(),wh.y());
			ite->setTextFlowMode(PageItem::TextFlowDisabled);
			m_doc->AdjustItemSize(ite);
			m_Elements->append(ite);
			if (m_groupStack.count() != 0)
				m_groupStack.top().Items.append(ite);
		}
	}
}

void SlaOutputDev::fill(GfxState *state)
{
//	qDebug() << "Fill";
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	FPointArray out;
	QString output = convertPath(state->getPath());
	out.parseSVG(output);
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	out.map(m_ctm);
	Coords = output;
	FPoint wh = out.WidthHeight();
	if ((out.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
	{
		int z;
		if (pathIsClosed)
			z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		else
			z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_doc->Items->at(z);
		ite->PoLine = out.copy();
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(shade);
		ite->setLineShade(100);
		ite->setFillEvenOdd(false);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		ite->setLineEnd(PLineEnd);
		ite->setLineJoin(PLineJoin);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_doc->AdjustItemSize(ite);
		m_Elements->append(ite);
		if (m_groupStack.count() != 0)
		{
			m_groupStack.top().Items.append(ite);
			applyMask(ite);
		}
	}
}

void SlaOutputDev::eoFill(GfxState *state)
{
//	qDebug() << "EoFill";
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	FPointArray out;
	QString output = convertPath(state->getPath());
	out.parseSVG(output);
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	out.map(m_ctm);
	Coords = output;
	FPoint wh = out.WidthHeight();
	if ((out.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
	{
		int z;
		if (pathIsClosed)
			z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		else
			z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_doc->Items->at(z);
		ite->PoLine = out.copy();
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(shade);
		ite->setLineShade(100);
		ite->setFillEvenOdd(true);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		ite->setLineEnd(PLineEnd);
		ite->setLineJoin(PLineJoin);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_doc->AdjustItemSize(ite);
		m_Elements->append(ite);
		if (m_groupStack.count() != 0)
		{
			m_groupStack.top().Items.append(ite);
			applyMask(ite);
		}
	}
}

GBool SlaOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax)
{
	double GrStartX;
	double GrStartY;
	double GrEndX;
	double GrEndY;
	int shade = 100;
	Function *func = shading->getFunc(0);
	VGradient FillGradient = VGradient(VGradient::linear);
	FillGradient.clearStops();
	GfxColorSpace *color_space = shading->getColorSpace();
	if (func->getType() == 3)
	{
		StitchingFunction *stitchingFunc = (StitchingFunction*)func;
		double *bounds = stitchingFunc->getBounds();
		int num_funcs = stitchingFunc->getNumFuncs();
		// Add stops from all the stitched functions
		for ( int i = 0 ; i < num_funcs ; i++ )
		{
			GfxColor temp;
			((GfxAxialShading*)shading)->getColor(bounds[i], &temp);
			QString stopColor = getColor(color_space, &temp, &shade);
			FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor], m_doc), bounds[i], 0.5, 1.0, stopColor, shade );
			if (i == num_funcs - 1)
			{
				((GfxAxialShading*)shading)->getColor(bounds[i+1], &temp);
				QString stopColor = getColor(color_space, &temp, &shade);
				FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor], m_doc), bounds[i+1], 0.5, 1.0, stopColor, shade );
			}
		}
	}
	else if ((func->getType() == 2) || (func->getType() == 0))
	{
		GfxColor stop1;
		((GfxAxialShading*)shading)->getColor(0.0, &stop1);
		QString stopColor1 = getColor(color_space, &stop1, &shade);
		FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor1], m_doc), 0.0, 0.5, 1.0, stopColor1, shade );
		GfxColor stop2;
		((GfxAxialShading*)shading)->getColor(1.0, &stop2);
		QString stopColor2 = getColor(color_space, &stop2, &shade);
		FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor2], m_doc), 1.0, 0.5, 1.0, stopColor2, shade );
	}
	((GfxAxialShading*)shading)->getCoords(&GrStartX, &GrStartY, &GrEndX, &GrEndY);
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	FPointArray gr;
	gr.addPoint(GrStartX, GrStartY);
	gr.addPoint(GrEndX, GrEndY);
	gr.map(m_ctm);
	GrStartX = gr.point(0).x() - crect.x();
	GrStartY = gr.point(0).y() - crect.y();
	GrEndX = gr.point(1).x() - crect.x();
	GrEndY = gr.point(1).y() - crect.y();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(shade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->GrType = 6;
	ite->fill_gradient = FillGradient;
	ite->setGradientVector(GrStartX, GrStartY, GrEndX, GrEndY, 0, 0, 1, 0);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	return gTrue;
}

GBool SlaOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax)
{
	double GrStartX;
	double GrStartY;
	double GrEndX;
	double GrEndY;
	int shade = 100;
	Function *func = shading->getFunc(0);
	VGradient FillGradient = VGradient(VGradient::linear);
	FillGradient.clearStops();
	GfxColorSpace *color_space = shading->getColorSpace();
	if (func->getType() == 3)
	{
		StitchingFunction *stitchingFunc = (StitchingFunction*)func;
		double *bounds = stitchingFunc->getBounds();
		int num_funcs = stitchingFunc->getNumFuncs();
		// Add stops from all the stitched functions
		for ( int i = 0 ; i < num_funcs ; i++ )
		{
			GfxColor temp;
			((GfxRadialShading*)shading)->getColor(bounds[i], &temp);
			QString stopColor = getColor(color_space, &temp, &shade);
			FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor], m_doc), bounds[i], 0.5, 1.0, stopColor, shade );
			if (i == num_funcs - 1)
			{
				((GfxRadialShading*)shading)->getColor(bounds[i+1], &temp);
				QString stopColor = getColor(color_space, &temp, &shade);
				FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor], m_doc), bounds[i+1], 0.5, 1.0, stopColor, shade );
			}
		}
	}
	else if ((func->getType() == 2) || (func->getType() == 0))
	{
		GfxColor stop1;
		((GfxRadialShading*)shading)->getColor(0.0, &stop1);
		QString stopColor1 = getColor(color_space, &stop1, &shade);
		FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor1], m_doc), 0.0, 0.5, 1.0, stopColor1, shade );
		GfxColor stop2;
		((GfxRadialShading*)shading)->getColor(1.0, &stop2);
		QString stopColor2 = getColor(color_space, &stop2, &shade);
		FillGradient.addStop( ScColorEngine::getRGBColor(m_doc->PageColors[stopColor2], m_doc), 1.0, 0.5, 1.0, stopColor2, shade );
	}
	double r0, x1, y1, r1;
	((GfxRadialShading*)shading)->getCoords(&GrStartX, &GrStartY, &r0, &x1, &y1, &r1);
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	double GrFocalX = x1;
	double GrFocalY = y1;
	GrEndX = GrFocalX + r1;
	GrEndY = GrFocalY;
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	FPointArray gr;
	gr.addPoint(GrStartX, GrStartY);
	gr.addPoint(GrEndX, GrEndY);
	gr.addPoint(GrFocalX, GrFocalY);
	gr.map(m_ctm);
	GrStartX = gr.point(0).x() - crect.x();
	GrStartY = gr.point(0).y() - crect.y();
	GrEndX = gr.point(1).x() - crect.x();
	GrEndY = gr.point(1).y() - crect.y();
	GrFocalX = gr.point(2).x() - crect.x();
	GrFocalY = gr.point(2).y() - crect.y();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(shade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->GrType = 7;
	ite->fill_gradient = FillGradient;
	ite->setGradientVector(GrStartX, GrStartY, GrEndX, GrEndY, GrFocalX, GrFocalY, 1, 0);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	return gTrue;
}

GBool SlaOutputDev::gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading)
{
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	GfxColor color[3];
	double x0, y0, x1, y1, x2, y2;
	for (int i = 0; i < shading->getNTriangles(); i++)
	{
		meshGradientPatch patchM;
		shading->getTriangle(i, &x0, &y0, &color[0],  &x1, &y1, &color[1],  &x2, &y2, &color[2]);
		patchM.BL.resetTo(FPoint(x0, y0));
		patchM.BL.shade = 100;
		patchM.BL.transparency = 1.0;
		patchM.BL.colorName = getColor(shading->getColorSpace(), &color[0], &shade);
		patchM.BL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BL.colorName], m_doc, shade);
		patchM.TL.resetTo(FPoint(x1, y1));
		patchM.TL.shade = 100;
		patchM.TL.transparency = 1.0;
		patchM.TL.colorName = getColor(shading->getColorSpace(), &color[1], &shade);
		patchM.TL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TL.colorName], m_doc, shade);
		patchM.TR.resetTo(FPoint(x2, y2));
		patchM.TR.shade = 100;
		patchM.TR.transparency = 1.0;
		patchM.TR.colorName = getColor(shading->getColorSpace(), &color[2], &shade);
		patchM.TR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TR.colorName], m_doc, shade);
		patchM.BR.resetTo(FPoint(x0, y0));
		patchM.BR.shade = 100;
		patchM.BR.transparency = 1.0;
		patchM.BR.colorName = getColor(shading->getColorSpace(), &color[0], &shade);
		patchM.BR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BR.colorName], m_doc, shade);
		patchM.TL.transform(m_ctm);
		patchM.TL.moveRel(-crect.x(), -crect.y());
		patchM.TR.transform(m_ctm);
		patchM.TR.moveRel(-crect.x(), -crect.y());
		patchM.BR.transform(m_ctm);
		patchM.BR.moveRel(-crect.x(), -crect.y());
		patchM.BL.transform(m_ctm);
		patchM.BL.moveRel(-crect.x(), -crect.y());
		ite->meshGradientPatches.append(patchM);
	}
	ite->GrType = 12;
	return gTrue;
}

GBool SlaOutputDev::patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading)
{
//	qDebug() << "mesh shaded fill";
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(shade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
//	qDebug() << "Mesh of" << shading->getNPatches() << "Patches";
	ite->meshGradientPatches.clear();
	for (int i = 0; i < shading->getNPatches(); i++)
	{
		GfxPatch *patch = shading->getPatch(i);
		GfxColor color;
		meshGradientPatch patchM;
		int u, v;
		patchM.BL.resetTo(FPoint(patch->x[0][0], patch->y[0][0]));
		patchM.BL.controlTop = FPoint(patch->x[0][1], patch->y[0][1]);
		patchM.BL.controlRight = FPoint(patch->x[1][0], patch->y[1][0]);
		patchM.BL.controlColor = FPoint(patch->x[1][1], patch->y[1][1]);
		u = 0;
		v = 0;
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.BL.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.BL.shade = 100;
		patchM.BL.transparency = 1.0;
		patchM.BL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BL.colorName], m_doc, shade);

		u = 0;
		v = 1;
		patchM.TL.resetTo(FPoint(patch->x[0][3], patch->y[0][3]));
		patchM.TL.controlRight = FPoint(patch->x[1][3], patch->y[1][3]);
		patchM.TL.controlBottom = FPoint(patch->x[0][2], patch->y[0][2]);
		patchM.TL.controlColor = FPoint(patch->x[1][2], patch->y[1][2]);
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.TL.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.TL.shade = 100;
		patchM.TL.transparency = 1.0;
		patchM.TL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TL.colorName], m_doc, shade);

		u = 1;
		v = 1;
		patchM.TR.resetTo(FPoint(patch->x[3][3], patch->y[3][3]));
		patchM.TR.controlBottom = FPoint(patch->x[3][2], patch->y[3][2]);
		patchM.TR.controlLeft = FPoint(patch->x[2][3], patch->y[2][3]);
		patchM.TR.controlColor = FPoint(patch->x[2][2], patch->y[2][2]);
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.TR.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.TR.shade = 100;
		patchM.TR.transparency = 1.0;
		patchM.TR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TR.colorName], m_doc, shade);

		u = 1;
		v = 0;
		patchM.BR.resetTo(FPoint(patch->x[3][0], patch->y[3][0]));
		patchM.BR.controlLeft = FPoint(patch->x[2][0], patch->y[2][0]);
		patchM.BR.controlTop = FPoint(patch->x[3][1], patch->y[3][1]);
		patchM.BR.controlColor = FPoint(patch->x[2][1], patch->y[2][1]);
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.BR.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.BR.shade = 100;
		patchM.BR.transparency = 1.0;
		patchM.BR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BR.colorName], m_doc, shade);

		patchM.TL.transform(m_ctm);
		patchM.TL.moveRel(-crect.x(), -crect.y());
		patchM.TR.transform(m_ctm);
		patchM.TR.moveRel(-crect.x(), -crect.y());
		patchM.BR.transform(m_ctm);
		patchM.BR.moveRel(-crect.x(), -crect.y());
		patchM.BL.transform(m_ctm);
		patchM.BL.moveRel(-crect.x(), -crect.y());
		ite->meshGradientPatches.append(patchM);
	}
	ite->GrType = 12;
	return gTrue;
}

GBool SlaOutputDev::tilingPatternFill(GfxState *state, Catalog *cat, Object *str, double *pmat, int paintType, Dict *resDict, double *mat, double *bbox, int x0, int y0, int x1, int y1, double xStep, double yStep)
{
	PDFRectangle box;
	Gfx *gfx;
	QString id;
	PageItem *ite;
	groupEntry gElements;
	gElements.forSoftMask = gFalse;
	gElements.alpha = gFalse;
	gElements.inverted = false;
	gElements.maskName = "";
	gElements.Items.clear();
	m_groupStack.push(gElements);
	double width, height;
	width = bbox[2] - bbox[0];
	height = bbox[3] - bbox[1];
	if (xStep != width || yStep != height)
		return gFalse;
	box.x1 = bbox[0];
	box.y1 = bbox[1];
	box.x2 = bbox[2];
	box.y2 = bbox[3];
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	QTransform mm = QTransform(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5]);
	QTransform mmx = mm * m_ctm;
	gfx = new Gfx(xref, this, resDict, catalog, &box, NULL);
	gfx->display(str);
	gElements = m_groupStack.pop();
	tmpSel->clear();
	double pwidth = 0;
	double pheight = 0;
	if (gElements.Items.count() > 0)
	{
		for (int dre = 0; dre < gElements.Items.count(); ++dre)
		{
			tmpSel->addItem(gElements.Items.at(dre), true);
			m_Elements->removeAll(gElements.Items.at(dre));
		}
		ite = m_doc->groupObjectsSelection(tmpSel);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		m_doc->m_Selection->addItem(ite, true);
		m_doc->itemSelection_FlipV();
		m_doc->m_Selection->clear();
		ScPattern pat = ScPattern();
		pat.setDoc(m_doc);
		m_doc->DoDrawing = true;
		pat.pattern = ite->DrawObj_toImage(qMax(ite->width(), ite->height()));
		pat.xoffset = 0;
		pat.yoffset = 0;
		m_doc->DoDrawing = false;
		pat.width = ite->width();
		pat.height = ite->height();
		pwidth = ite->width();
		pheight = ite->height();
		ite->gXpos = 0;
		ite->gYpos = 0;
		ite->setXYPos(ite->gXpos, ite->gYpos, true);
		pat.items.append(ite);
		m_doc->Items->removeAll(ite);
		id = QString("Pattern_from_PDF_%1").arg(m_doc->docPatterns.count() + 1);
		m_doc->addPattern(id, pat);
		tmpSel->clear();
	}
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(shade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->GrType = 8;
	ite->setPattern(id);
	ite->setPatternTransform(fabs(pmat[0]) * 100, fabs(pmat[3]) * 100, mmx.dx() - ctm[4], mmx.dy() - ctm[5], 0, -1 * pmat[1], pmat[2]);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	delete gfx;
	return gTrue;
}

void SlaOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool interpolate, GBool inlineImg)
{
//	qDebug() << "Draw Image Mask";
	QImage * image = 0;
	int invert_bit;
	int row_stride;
	int x, y, i, bit;
	unsigned char *dest = 0;
	unsigned char *buffer;
	Guchar *pix;
	ImageStream * imgStr = new ImageStream(str, width, 1, 1);
	imgStr->reset();
#ifdef WORDS_BIGENDIAN
	image = new QImage(width, height, QImage::Format_Mono);
#else
	image = new QImage(width, height, QImage::Format_MonoLSB);
#endif
	if (image == NULL || image->isNull())
	{
		delete imgStr;
		delete image;
		return;
	}
	invert_bit = invert ? 1 : 0;
	buffer = image->bits();
	row_stride = image->bytesPerLine();
	for (y = 0; y < height; y++)
	{
		pix = imgStr->getLine();
		dest = buffer + y * row_stride;
		i = 0;
		bit = 0;
		for (x = 0; x < width; x++)
		{
			if (bit == 0)
				dest[i] = 0;
			if (!(pix[x] ^ invert_bit))
			{
#ifdef WORDS_BIGENDIAN
				dest[i] |= (1 << (7 - bit));
#else
				dest[i] |= (1 << bit);
#endif
			}
			bit++;
			if (bit > 7)
			{
				bit = 0;
				i++;
			}
		}
	}
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	QColor backColor = ScColorEngine::getShadeColorProof(m_doc->PageColors[CurrColorFill], m_doc, shade);
	QImage res = QImage(width, height, QImage::Format_ARGB32);
	res.fill(backColor.rgb());
	unsigned char cc, cm, cy, ck;
	for( int yi = 0; yi < res.height(); ++yi )
	{
		QRgb *t = (QRgb*)(res.scanLine( yi ));
		for(int xi = 0; xi < res.width(); ++xi )
		{
			cc = qRed(*t);
			cm = qGreen(*t);
			cy = qBlue(*t);
			ck = image->pixel(xi, yi);
			if (ck == 0)
				(*t) = qRgba(cc, cm, cy, 0);
			else
				(*t) = qRgba(cc, cm, cy, 255);
			t++;
		}
	}
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QRectF crect = QRectF(0, 0, width, height);
	m_ctm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, ctm[2] + ctm[4], ctm[3] + ctm[5]);
	QLineF cline = QLineF(0, 0, width, 0);
	QLineF tline = m_ctm.map(cline);
	QRectF trect = m_ctm.mapRect(crect);
	double sx = m_ctm.m11();
	double sy = m_ctm.m22();
	QTransform mm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, 0, 0);
	if ((mm.type() == QTransform::TxShear) || (mm.type() == QTransform::TxRotate))
	{
		mm.reset();
		mm.rotate(-tline.angle());
	}
	else
	{
		mm.reset();
		if (sx < 0)
			mm.scale(-1, 1);
		if (sy < 0)
			mm.scale(1, -1);
	}
	res = res.transformed(mm);
	if (res.isNull())
	{
		delete imgStr;
		delete image;
		return;
	}
	int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, xCoor + trect.x(), yCoor + trect.y(), trect.width(), trect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->SetRectFrame();
	m_doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.png");
	if (ite->tempImageFile->open())
	{
		QString fileName = getLongPathName(ite->tempImageFile->fileName());
		if (!fileName.isEmpty())
		{
			ite->tempImageFile->close();
			ite->isInlineImage = true;
			res.save(fileName, "PNG");
			m_doc->loadPict(fileName, ite);
			ite->setImageScalingMode(false, true);
			m_doc->AdjustItemSize(ite);
			m_Elements->append(ite);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(ite);
				applyMask(ite);
			}
		}
		else
			m_doc->Items->removeAll(ite);
	}
	else
		m_doc->Items->removeAll(ite);
	imgStr->close();
	delete imgStr;
	delete image;
}

void SlaOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool interpolate, Stream *maskStr, int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap, GBool maskInterpolate)
{
	ImageStream * imgStr = new ImageStream(str, width, colorMap->getNumPixelComps(), colorMap->getBits());
	imgStr->reset();
	unsigned int *dest = 0;
	unsigned char * buffer = new unsigned char[width * height * 4];
	QImage * image = 0;
	for (int y = 0; y < height; y++)
	{
		dest = (unsigned int *)(buffer + y * 4 * width);
		Guchar * pix = imgStr->getLine();
		colorMap->getRGBLine(pix, dest, width);
	}
	image = new QImage(buffer, width, height, QImage::Format_RGB32);
	if (image == NULL || image->isNull())
	{
		delete imgStr;
		delete[] buffer;
		delete image;
		return;
	}
	ImageStream *mskStr = new ImageStream(maskStr, maskWidth, maskColorMap->getNumPixelComps(), maskColorMap->getBits());
	mskStr->reset();
	Guchar *mdest = 0;
	unsigned char * mbuffer = new unsigned char[maskWidth * maskHeight];
	for (int y = 0; y < maskHeight; y++)
	{
		mdest = (Guchar *)(mbuffer + y * maskWidth);
		Guchar * pix = mskStr->getLine();
		maskColorMap->getGrayLine(pix, mdest, maskWidth);
	}
	if ((maskWidth != width) || (maskHeight != height))
	{
		delete imgStr;
		delete[] buffer;
		delete image;
		delete mskStr;
		delete[] mbuffer;
		return;
	}
	QImage res = image->convertToFormat(QImage::Format_ARGB32);
	unsigned char cc, cm, cy, ck;
	int s = 0;
	for( int yi=0; yi < res.height(); ++yi )
	{
		QRgb *t = (QRgb*)(res.scanLine( yi ));
		for(int xi=0; xi < res.width(); ++xi )
		{
			cc = qRed(*t);
			cm = qGreen(*t);
			cy = qBlue(*t);
			ck = mbuffer[s];
			(*t) = qRgba(cc, cm, cy, ck);
			s++;
			t++;
		}
	}
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QRectF crect = QRectF(0, 0, width, height);
	m_ctm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, ctm[2] + ctm[4], ctm[3] + ctm[5]);
	QLineF cline = QLineF(0, 0, width, 0);
	QLineF tline = m_ctm.map(cline);
	QRectF trect = m_ctm.mapRect(crect);
	double sx = m_ctm.m11();
	double sy = m_ctm.m22();
	QTransform mm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, 0, 0);
	if ((mm.type() == QTransform::TxShear) || (mm.type() == QTransform::TxRotate))
	{
		mm.reset();
		mm.rotate(-tline.angle());
	}
	else
	{
		mm.reset();
		if (sx < 0)
			mm.scale(-1, 1);
		if (sy < 0)
			mm.scale(1, -1);
	}
	res = res.transformed(mm);
	int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, xCoor + trect.x(), yCoor + trect.y(), trect.width(), trect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->SetRectFrame();
	m_doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.png");
	if (ite->tempImageFile->open())
	{
		QString fileName = getLongPathName(ite->tempImageFile->fileName());
		if (!fileName.isEmpty())
		{
			ite->tempImageFile->close();
			ite->isInlineImage = true;
			res.save(fileName, "PNG");
			m_doc->loadPict(fileName, ite);
			ite->setImageScalingMode(false, true);
			m_doc->AdjustItemSize(ite);
			m_Elements->append(ite);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(ite);
				applyMask(ite);
			}
		}
		else
			m_doc->Items->removeAll(ite);
	}
	else
		m_doc->Items->removeAll(ite);
	delete imgStr;
	delete[] buffer;
	delete image;
	delete mskStr;
	delete[] mbuffer;
}

void SlaOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool interpolate, int *maskColors, GBool inlineImg)
{
	ImageStream * imgStr = new ImageStream(str, width, colorMap->getNumPixelComps(), colorMap->getBits());
	imgStr->reset();
	unsigned int *dest = 0;
	unsigned char * buffer = new unsigned char[width * height * 4];
	QImage * image = 0;
	if (maskColors)
	{
		for (int y = 0; y < height; y++)
		{
			dest = (unsigned int *)(buffer + y * 4 * width);
			Guchar * pix = imgStr->getLine();
			colorMap->getRGBLine(pix, dest, width);
			for (int x = 0; x < width; x++)
			{
				for (int i = 0; i < colorMap->getNumPixelComps(); ++i)
				{
					if (pix[i] < maskColors[2*i] * 255 || pix[i] > maskColors[2*i+1] * 255)
					{
						*dest = *dest | 0xff000000;
						break;
					}
				}
				pix += colorMap->getNumPixelComps();
				dest++;
			}
		}
		image = new QImage(buffer, width, height, QImage::Format_ARGB32);
	}
	else
	{
		for (int y = 0; y < height; y++)
		{
			dest = (unsigned int *)(buffer + y * 4 * width);
			Guchar * pix = imgStr->getLine();
			colorMap->getRGBLine(pix, dest, width);
		}
		image = new QImage(buffer, width, height, QImage::Format_RGB32);
	}
	if (image == NULL || image->isNull())
	{
		delete imgStr;
		delete[] buffer;
		delete image;
		return;
	}
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QRectF crect = QRectF(0, 0, width, height);
	m_ctm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, ctm[2] + ctm[4], ctm[3] + ctm[5]);
	QLineF cline = QLineF(0, 0, width, 0);
	QLineF tline = m_ctm.map(cline);
	QRectF trect = m_ctm.mapRect(crect);
	double sx = m_ctm.m11();
	double sy = m_ctm.m22();
	QTransform mm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, 0, 0);
	if ((mm.type() == QTransform::TxShear) || (mm.type() == QTransform::TxRotate))
	{
		mm.reset();
		mm.rotate(-tline.angle());
	}
	else
	{
		mm.reset();
		if (sx < 0)
			mm.scale(-1, 1);
		if (sy < 0)
			mm.scale(1, -1);
	}
	QImage img = image->transformed(mm);
	int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, xCoor + trect.x(), yCoor + trect.y(), trect.width(), trect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->SetRectFrame();
	m_doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.png");
	if (ite->tempImageFile->open())
	{
		QString fileName = getLongPathName(ite->tempImageFile->fileName());
		if (!fileName.isEmpty())
		{
			ite->tempImageFile->close();
			ite->isInlineImage = true;
			img.save(fileName, "PNG");
			m_doc->loadPict(fileName, ite);
			ite->setImageScalingMode(false, true);
			m_doc->AdjustItemSize(ite);
			m_Elements->append(ite);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(ite);
				applyMask(ite);
			}
		}
		else
			m_doc->Items->removeAll(ite);
	}
	else
		m_doc->Items->removeAll(ite);
	delete imgStr;
	delete[] buffer;
	delete image;
}

void SlaOutputDev::beginMarkedContent(char *name, Dict *properties)
{
//	qDebug() << "Begin Marked Content with Name " << QString(name);
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (QString(name) == "Layer")		// Handle Adobe Illustrator Layer command
		{
			layerNum++;
			if (!firstLayer)
				currentLayer = m_doc->addLayer(QString("Layer_%1").arg(layerNum), true);
			firstLayer = false;
			Object obj;
			if (properties->lookup((char*)"Title", &obj))
			{
				if (obj.isString())
				{
					QString lName =  QString(obj.getString()->getCString());
					m_doc->changeLayerName(currentLayer, lName);
				}
				obj.free();
			}
			if (properties->lookup((char*)"Visible", &obj))
			{
				if (obj.isBool())
					m_doc->setLayerVisible(currentLayer, obj.getBool());
				obj.free();
			}
			if (properties->lookup((char*)"Editable", &obj))
			{
				if (obj.isBool())
					m_doc->setLayerLocked(currentLayer, !obj.getBool());
				obj.free();
			}
			if (properties->lookup((char*)"Printed", &obj))
			{
				if (obj.isBool())
					m_doc->setLayerPrintable(currentLayer, obj.getBool());
				obj.free();
			}
			if (properties->lookup((char*)"Color", &obj))
			{
				if (obj.isArray())
				{
					Object obj1;
					obj.arrayGet(0, &obj1);
					int r = obj1.getNum() / 256;
					obj1.free();
					obj.arrayGet(1, &obj1);
					int g = obj1.getNum() / 256;
					obj1.free();
					obj.arrayGet(2, &obj1);
					int b = obj1.getNum() / 256;
					obj1.free();
					m_doc->setLayerMarker(currentLayer, QColor(r, g, b));
				}
				obj.free();
			}
		}
	}
}

void SlaOutputDev::endMarkedContent(GfxState *state)
{
//	qDebug() << "End Marked Content";
}

void SlaOutputDev::updateFont(GfxState *state)
{
	GfxFont *gfxFont;
	GfxFontType fontType;
	SplashOutFontFileID *id;
	SplashFontFile *fontFile;
	SplashFontSrc *fontsrc = NULL;
	FoFiTrueType *ff;
	Ref embRef;
	Object refObj, strObj;
	GooString *fileName;
	char *tmpBuf;
	int tmpBufLen;
	Gushort *codeToGID;
	DisplayFontParam *dfp;
	double *textMat;
	double m11, m12, m21, m22, fontSize;
	SplashCoord mat[4];
	int n;
	int faceIndex = 0;
	SplashCoord matrix[6];

	m_font = NULL;
	fileName = NULL;
	tmpBuf = NULL;

	if (!(gfxFont = state->getFont()))
		goto err1;
	fontType = gfxFont->getType();
	if (fontType == fontType3)
		goto err1;

  // check the font file cache
	id = new SplashOutFontFileID(gfxFont->getID());
	if ((fontFile = m_fontEngine->getFontFile(id)))
	{
		delete id;
	}
	else
	{
		// if there is an embedded font, write it to disk
		if (gfxFont->getEmbeddedFontID(&embRef))
		{
			tmpBuf = gfxFont->readEmbFontFile(xref, &tmpBufLen);
			if (!tmpBuf)
				goto err2;
		// if there is an external font file, use it
		}
		else if (!(fileName = gfxFont->getExtFontFile()))
		{
	  // look for a display font mapping or a substitute font
			dfp = NULL;
			if (gfxFont->getName())
			{
				dfp = globalParams->getDisplayFont(gfxFont);
			}
			if (!dfp)
			{
		//		error(-1, "Couldn't find a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			switch (dfp->kind)
			{
				case displayFontT1:
					fileName = dfp->t1.fileName;
					fontType = gfxFont->isCIDFont() ? fontCIDType0 : fontType1;
					break;
				case displayFontTT:
					fileName = dfp->tt.fileName;
					fontType = gfxFont->isCIDFont() ? fontCIDType2 : fontTrueType;
					faceIndex = dfp->tt.faceIndex;
					break;
			}
		}
		fontsrc = new SplashFontSrc;
		if (fileName)
			fontsrc->setFile(fileName, gFalse);
		else
			fontsrc->setBuf(tmpBuf, tmpBufLen, gTrue);
	// load the font file
		switch (fontType)
		{
			case fontType1:
				if (!(fontFile = m_fontEngine->loadType1Font( id, fontsrc, ((Gfx8BitFont *)gfxFont)->getEncoding())))
				{
			//		error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
				break;
			case fontType1C:
				if (!(fontFile = m_fontEngine->loadType1CFont( id, fontsrc, ((Gfx8BitFont *)gfxFont)->getEncoding())))
				{
		//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
				break;
			case fontType1COT:
				if (!(fontFile = m_fontEngine->loadOpenTypeT1CFont( id, fontsrc, ((Gfx8BitFont *)gfxFont)->getEncoding())))
				{
		//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
				break;
			case fontTrueType:
			case fontTrueTypeOT:
				if (fileName)
					ff = FoFiTrueType::load(fileName->getCString());
				else
					ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
				if (ff)
				{
					codeToGID = ((Gfx8BitFont *)gfxFont)->getCodeToGIDMap(ff);
					n = 256;
					delete ff;
				}
				else
				{
					codeToGID = NULL;
					n = 0;
				}
				if (!(fontFile = m_fontEngine->loadTrueTypeFont( id, fontsrc, codeToGID, n)))
				{
	//				error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
			break;
		case fontCIDType0:
		case fontCIDType0C:
			if (!(fontFile = m_fontEngine->loadCIDFont( id, fontsrc)))
			{
	//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			break;
		case fontCIDType0COT:
			if (!(fontFile = m_fontEngine->loadOpenTypeCFFFont( id, fontsrc)))
			{
	//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			break;
		case fontCIDType2:
		case fontCIDType2OT:
			codeToGID = NULL;
			n = 0;
			if (((GfxCIDFont *)gfxFont)->getCIDToGID())
			{
				n = ((GfxCIDFont *)gfxFont)->getCIDToGIDLen();
				if (n)
				{
					codeToGID = (Gushort *)gmallocn(n, sizeof(Gushort));
					memcpy(codeToGID, ((GfxCIDFont *)gfxFont)->getCIDToGID(), n * sizeof(Gushort));
				}
			}
			else
			{
				if (fileName)
					ff = FoFiTrueType::load(fileName->getCString());
				else
					ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
				if (!ff)
					goto err2;
				codeToGID = ((GfxCIDFont *)gfxFont)->getCodeToGIDMap(ff, &n);
				delete ff;
			}
			if (!(fontFile = m_fontEngine->loadTrueTypeFont( id, fontsrc, codeToGID, n, faceIndex)))
			{
	//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			break;
		default:
	  // this shouldn't happen
			goto err2;
		}
	}
	// get the font matrix
	textMat = state->getTextMat();
	fontSize = state->getFontSize();
	m11 = textMat[0] * fontSize * state->getHorizScaling();
	m12 = textMat[1] * fontSize * state->getHorizScaling();
	m21 = textMat[2] * fontSize;
	m22 = textMat[3] * fontSize;
	matrix[0] = 1;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 1;
	matrix[4] = 0;
	matrix[5] = 0;
  // create the scaled font
	mat[0] = m11;
	mat[1] = -m12;
	mat[2] = m21;
	mat[3] = -m22;
	m_font = m_fontEngine->getFont(fontFile, mat, matrix);
	if (fontsrc && !fontsrc->isFile)
		fontsrc->unref();
	return;

err2:
	delete id;
err1:
	if (fontsrc && !fontsrc->isFile)
		fontsrc->unref();
	return;
}

void SlaOutputDev::drawChar(GfxState *state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode *u, int uLen)
{
	double x1, y1, x2, y2;
	int render;
	updateFont(state);
	if (!m_font)
		return;
  // check for invisible text -- this is used by Acrobat Capture
	render = state->getRender();
	if (render == 3)
		return;
	if (!(render & 1))
	{
		SplashPath * fontPath;
		fontPath = m_font->getGlyphPath(code);
		if (fontPath)
		{
			QPainterPath qPath;
			qPath.setFillRule(Qt::WindingFill);
			for (int i = 0; i < fontPath->getLength(); ++i)
			{
				Guchar f;
				fontPath->getPoint(i, &x1, &y1, &f);
				if (f & splashPathFirst)
					qPath.moveTo(x1,y1);
				else if (f & splashPathCurve)
				{
					double x3, y3;
					++i;
					fontPath->getPoint(i, &x2, &y2, &f);
					++i;
					fontPath->getPoint(i, &x3, &y3, &f);
					qPath.cubicTo(x1,y1,x2,y2,x3,y3);
				}
				else
					qPath.lineTo(x1,y1);
				if (f & splashPathLast)
					qPath.closeSubpath();
			}
			double *ctm;
			ctm = state->getCTM();
			m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
			double xCoor = m_doc->currentPage()->xOffset();
			double yCoor = m_doc->currentPage()->yOffset();
			int shade = 100;
			CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
			FPointArray textPath;
			textPath.fromQPainterPath(qPath);
			FPoint wh = textPath.WidthHeight();
			if ((textPath.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
			{
				int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
				PageItem* ite = m_doc->Items->at(z);
				FPoint tp2(getMinClipF(&textPath));
				QTransform mm;
				mm.scale(1, -1);
				mm.translate(x, -y);
				textPath.map(mm);
				textPath.map(m_ctm);
				ite->PoLine = textPath.copy();
				ite->ClipEdited = true;
				ite->FrameType = 3;
				ite->setFillShade(shade);
				ite->setFillEvenOdd(false);
				ite->setFillTransparency(1.0 - state->getFillOpacity());
				ite->setFillBlendmode(getBlendMode(state));
				ite->setLineEnd(PLineEnd);
				ite->setLineJoin(PLineJoin);
				ite->setTextFlowMode(PageItem::TextFlowDisabled);
				m_doc->AdjustItemSize(ite);
				if ((render & 3) == 1 || (render & 3) == 2)
				{
					CurrColorStroke = getColor(state->getStrokeColorSpace(), state->getStrokeColor(), &shade);
					ite->setLineColor(CurrColorStroke);
					ite->setLineWidth(state->getTransformedLineWidth());
					ite->setLineTransparency(1.0 - state->getStrokeOpacity());
					ite->setLineBlendmode(getBlendMode(state));
					ite->setLineShade(shade);
				}
				m_Elements->append(ite);
				if (m_groupStack.count() != 0)
				{
					m_groupStack.top().Items.append(ite);
					applyMask(ite);
				}
				delete fontPath;
			}
		}
	}
}

GBool SlaOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen)
{
//	qDebug() << "beginType3Char";
	double *ctm;
	ctm = state->getCTM();
	QTransform orig_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	GfxFont *gfxFont;
	GfxFontType fontType;
	if (!(gfxFont = state->getFont()))
		return gTrue;
	fontType = gfxFont->getType();
	if (fontType != fontType3)
		return gTrue;
	Ref* fref = gfxFont->getID();
	QString fRefID = QString("Font_%1_%2").arg(fref->num).arg(code);
	if (m_Font_Pattern_Map.contains(fRefID))
	{
		QLineF cline = QLineF(0, 0, 1, 0);
		QLineF tline = orig_ctm.map(cline);
		double xCoor = m_doc->currentPage()->xOffset();
		double yCoor = m_doc->currentPage()->yOffset();
		ScPattern pat = m_doc->docPatterns[m_Font_Pattern_Map[fRefID].pattern];
		QTransform mm;
		mm.translate(0, -pat.height * tline.length());
		mm = orig_ctm * mm;
		int shade = 100;
		CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
		int z = 0;
		if (m_Font_Pattern_Map[fRefID].colored)
			z = m_doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CommonStrings::None, CommonStrings::None, true);
		else
			z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CurrColorFill, CommonStrings::None, true);
		PageItem *b = m_doc->Items->at(z);
		b->setWidth(pat.width * tline.length());
		b->setHeight(pat.height * tline.length());
		b->OldB2 = b->width();
		b->OldH2 = b->height();
		m_doc->RotMode(3);
		m_doc->RotateItem(-tline.angle(), b);
		m_doc->RotMode(0);
		b->setFillShade(shade);
		b->setFillEvenOdd(false);
		b->setFillTransparency(1.0 - state->getFillOpacity());
		b->setFillBlendmode(getBlendMode(state));
		b->setLineEnd(PLineEnd);
		b->setLineJoin(PLineJoin);
		b->setTextFlowMode(PageItem::TextFlowDisabled);
		if (m_Font_Pattern_Map[fRefID].colored)
			b->setPattern(m_Font_Pattern_Map[fRefID].pattern);
		else
		{
			b->setFillShade(shade);
			b->setPatternMask(m_Font_Pattern_Map[fRefID].pattern);
			b->setMaskTransform(b->width() / pat.width * 100, b->height() / pat.height * 100, 0, 0, 0, 0, 0);
			b->setMaskType(3);
		}
		m_Elements->append(b);
		if (m_groupStack.count() != 0)
		{
			m_groupStack.top().Items.append(b);
		}
		return gTrue;
	}
	else
	{
		F3Entry f3e;
		f3e.ctm = orig_ctm;
		f3e.glyphRef = fRefID;
		f3e.colored = false;
		m_F3Stack.push(f3e);
		ctm = state->getTextMat();
		state->setCTM(ctm[0], ctm[1], ctm[2], ctm[3], 0, 0);
		pushGroup();
		return gFalse;
	}
}

void SlaOutputDev::endType3Char(GfxState *state)
{
//	qDebug() << "endType3Char";
	double *ctm;
	ctm = state->getCTM();
	groupEntry gElements = m_groupStack.pop();
	tmpSel->clear();
	if (gElements.Items.count() > 0)
	{
		for (int dre = 0; dre < gElements.Items.count(); ++dre)
		{
			tmpSel->addItem(gElements.Items.at(dre), true);
			m_Elements->removeAll(gElements.Items.at(dre));
		}
		PageItem *ite = m_doc->groupObjectsSelection(tmpSel);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		m_doc->m_Selection->addItem(ite, true);
		m_doc->itemSelection_FlipV();
		m_doc->m_Selection->clear();
		ScPattern pat = ScPattern();
		pat.setDoc(m_doc);
		m_doc->DoDrawing = true;
		pat.pattern = ite->DrawObj_toImage(qMax(ite->width(), ite->height()));
		pat.xoffset = 0;
		pat.yoffset = 0;
		m_doc->DoDrawing = false;
		pat.width = ite->width();
		pat.height = ite->height();
		ite->gXpos = 0;
		ite->gYpos = 0;
		ite->setXYPos(ite->gXpos, ite->gYpos, true);
		pat.items.append(ite);
		m_doc->Items->removeAll(ite);
		QString id = QString("Pattern_from_PDF_%1T").arg(m_doc->docPatterns.count() + 1);
		m_doc->addPattern(id, pat);
		tmpSel->clear();
		if (m_F3Stack.count() > 0)
		{
			F3Entry f3e = m_F3Stack.pop();
			state->setCTM(f3e.ctm.m11(), f3e.ctm.m12(), f3e.ctm.m21(), f3e.ctm.m22(), f3e.ctm.dx(), f3e.ctm.dy());
			QLineF cline = QLineF(0, 0, 1, 0);
			QLineF tline = f3e.ctm.map(cline);
			double xCoor = m_doc->currentPage()->xOffset();
			double yCoor = m_doc->currentPage()->yOffset();
			QTransform mm;
			mm.translate(0, -pat.height * tline.length());
			mm = f3e.ctm * mm;
			int shade = 100;
			CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
			int z = 0;
			if (f3e.colored)
				z = m_doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CommonStrings::None, CommonStrings::None, true);
			else
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CurrColorFill, CommonStrings::None, true);
			PageItem *b = m_doc->Items->at(z);
			b->setWidth(pat.width * tline.length());
			b->setHeight(pat.height * tline.length());
			b->OldB2 = b->width();
			b->OldH2 = b->height();
			m_doc->RotMode(3);
			m_doc->RotateItem(-tline.angle(), b);
			m_doc->RotMode(0);
			b->setFillEvenOdd(false);
			b->setFillTransparency(1.0 - state->getFillOpacity());
			b->setFillBlendmode(getBlendMode(state));
			b->setLineEnd(PLineEnd);
			b->setLineJoin(PLineJoin);
			b->setTextFlowMode(PageItem::TextFlowDisabled);
			if (f3e.colored)
				b->setPattern(id);
			else
			{
				b->setFillShade(shade);
				b->setPatternMask(id);
				b->setMaskTransform(b->width() / pat.width * 100, b->height() / pat.height * 100, 0, 0, 0, 0, 0);
				b->setMaskType(3);
			}
			m_Elements->append(b);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(b);
			}
			F3GlyphEntry gid;
			gid.colored = f3e.colored;
			gid.pattern = id;
			m_Font_Pattern_Map.insert(f3e.glyphRef, gid);
		}
	}
}

void SlaOutputDev::type3D0(GfxState * /*state*/, double /*wx*/, double /*wy*/)
{
//	qDebug() << "type3D0";
	if (m_F3Stack.count() > 0)
		m_F3Stack.top().colored = true;
}

void SlaOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
//	qDebug() << "type3D1";
	if (m_F3Stack.count() > 0)
		m_F3Stack.top().colored = false;
}

void SlaOutputDev::beginTextObject(GfxState *state)
{
	pushGroup();
}

void SlaOutputDev::endTextObject(GfxState *state)
{
//	qDebug() << "End Text Object";
	if (m_groupStack.count() != 0)
	{
		groupEntry gElements = m_groupStack.pop();
		tmpSel->clear();
		if (gElements.Items.count() > 0)
		{
			for (int dre = 0; dre < gElements.Items.count(); ++dre)
			{
				tmpSel->addItem(gElements.Items.at(dre), true);
				m_Elements->removeAll(gElements.Items.at(dre));
			}
			PageItem *ite = m_doc->groupObjectsSelection(tmpSel);
			ite->setFillTransparency(1.0 - state->getFillOpacity());
			ite->setFillBlendmode(getBlendMode(state));
			for (int as = 0; as < tmpSel->count(); ++as)
			{
				m_Elements->append(tmpSel->itemAt(as));
			}
			if (m_groupStack.count() != 0)
				applyMask(ite);
		}
		if (m_groupStack.count() != 0)
		{
			for (int as = 0; as < tmpSel->count(); ++as)
			{
				m_groupStack.top().Items.append(tmpSel->itemAt(as));
			}
		}
		tmpSel->clear();
	}
}

QString SlaOutputDev::getColor(GfxColorSpace *color_space, GfxColor *color, int *shade)
{
	QString fNam;
	QString namPrefix = "FromPDF";
	ScColor tmp;
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	*shade = 100;
	if (m_F3Stack.count() > 0)
	{
		if (!m_F3Stack.top().colored)
			return "Black";
	}
	if ((color_space->getMode() == csDeviceRGB) || (color_space->getMode() == csCalRGB))
	{
		GfxRGB rgb;
		color_space->getRGB(color, &rgb);
		int Rc = qRound(colToDbl(rgb.r) * 255);
		int Gc = qRound(colToDbl(rgb.g) * 255);
		int Bc = qRound(colToDbl(rgb.b) * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color_space->getMode() == csDeviceCMYK)
	{
		GfxCMYK cmyk;
		color_space->getCMYK(color, &cmyk);
		int Cc = qRound(colToDbl(cmyk.c) * 255);
		int Mc = qRound(colToDbl(cmyk.m) * 255);
		int Yc = qRound(colToDbl(cmyk.y) * 255);
		int Kc = qRound(colToDbl(cmyk.k) * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if ((color_space->getMode() == csCalGray) || (color_space->getMode() == csDeviceGray))
	{
		GfxGray gray;
		color_space->getGray(color, &gray);
		int Kc = 255 - qRound(colToDbl(gray) * 255);
		tmp.setColor(0, 0, 0, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color_space->getMode() == csSeparation)
	{
		GfxCMYK cmyk;
		color_space->getCMYK(color, &cmyk);
		int Cc = qRound(colToDbl(cmyk.c) * 255);
		int Mc = qRound(colToDbl(cmyk.m) * 255);
		int Yc = qRound(colToDbl(cmyk.y) * 255);
		int Kc = qRound(colToDbl(cmyk.k) * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		tmp.setSpotColor(true);
		QString nam = QString(((GfxSeparationColorSpace*)color_space)->getName()->getCString());
		fNam = m_doc->PageColors.tryAddColor(nam, tmp);
		*shade = qRound(colToDbl(color->c[0]) * 100);
	}
	else
	{
		GfxRGB rgb;
		color_space->getRGB(color, &rgb);
		int Rc = qRound(colToDbl(rgb.r) * 255);
		int Gc = qRound(colToDbl(rgb.g) * 255);
		int Bc = qRound(colToDbl(rgb.b) * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	//	qDebug() << "update fill color other colorspace" << color_space->getMode() << "treating as rgb" << Rc << Gc << Bc;
	}
	if (fNam == namPrefix+tmp.name())
		m_importedColors->append(fNam);
	return fNam;
}

QString SlaOutputDev::convertPath(GfxPath *path)
{
    if (! path)
		return QString();

    QString output;
	pathIsClosed = false;

	for (int i = 0; i < path->getNumSubpaths(); ++i)
	{
        GfxSubpath * subpath = path->getSubpath(i);
		if (subpath->getNumPoints() > 0)
		{
			output += QString("M %1 %2").arg(subpath->getX(0)).arg(subpath->getY(0));
            int j = 1;
			while (j < subpath->getNumPoints())
			{
				if (subpath->getCurve(j))
				{
					output += QString("C %1 %2 %3 %4 %5 %6")
                              .arg(subpath->getX(j)).arg(subpath->getY(j))
                              .arg(subpath->getX(j + 1)).arg(subpath->getY(j + 1))
                              .arg(subpath->getX(j + 2)).arg(subpath->getY(j + 2));
                    j += 3;
				}
				else
				{
					output += QString("L %1 %2").arg(subpath->getX(j)).arg(subpath->getY(j));
                    ++j;
                }
            }
			if (subpath->isClosed())
			{
                output += QString("Z");
				pathIsClosed = true;
            }
        }
	}
	return output;
}

void SlaOutputDev::getPenState(GfxState *state)
{
	switch (state->getLineCap())
	{
		case 0:
			PLineEnd = Qt::FlatCap;
			break;
		case 1:
			PLineEnd = Qt::RoundCap;
			break;
		case 2:
			PLineEnd = Qt::SquareCap;
			break;
	}
	switch (state->getLineJoin())
	{
		case 0:
			PLineJoin = Qt::MiterJoin;
			break;
		case 1:
			PLineJoin = Qt::RoundJoin;
			break;
		case 2:
			PLineJoin = Qt::BevelJoin;
			break;
	}
	double *dashPattern;
	int dashLength;
	state->getLineDash(&dashPattern, &dashLength, &DashOffset);
	QVector<double> pattern(dashLength);
	for (int i = 0; i < dashLength; ++i)
	{
		pattern[i] = dashPattern[i];
	}
	DashValues = pattern;
}

int SlaOutputDev::getBlendMode(GfxState *state)
{
	int mode = 0;
	switch (state->getBlendMode())
	{
		default:
		case gfxBlendNormal:
			mode = 0;
			break;
		case gfxBlendDarken:
			mode = 1;
			break;
		case gfxBlendLighten:
			mode = 2;
			break;
		case gfxBlendMultiply:
			mode = 3;
			break;
		case gfxBlendScreen:
			mode = 4;
			break;
		case gfxBlendOverlay:
			mode = 5;
			break;
		case gfxBlendHardLight:
			mode = 6;
			break;
		case gfxBlendSoftLight:
			mode = 7;
			break;
		case gfxBlendDifference:
			mode = 8;
			break;
		case gfxBlendExclusion:
			mode = 9;
			break;
		case gfxBlendColorDodge:
			mode = 10;
			break;
		case gfxBlendColorBurn:
			mode = 11;
			break;
		case gfxBlendHue:
			mode = 12;
			break;
		case gfxBlendSaturation:
			mode = 13;
			break;
		case gfxBlendColor:
			mode = 14;
			break;
		case gfxBlendLuminosity:
			mode = 15;
			break;
	}
	return mode;
}

void SlaOutputDev::applyMask(PageItem *ite)
{
	if (m_groupStack.count() != 0)
	{
		if (!m_groupStack.top().maskName.isEmpty())
		{
			ite->setPatternMask(m_groupStack.top().maskName);
			if (m_groupStack.top().alpha)
			{
				if (m_groupStack.top().inverted)
					ite->setMaskType(8);
				else
					ite->setMaskType(3);
			}
			else
			{
				if (m_groupStack.top().inverted)
					ite->setMaskType(7);
				else
					ite->setMaskType(6);
			}
		}
	}
	// Code for updating our Progressbar, needs to be called this way, as we have no
	// possibility to get the current fileposition.
	updateGUICounter++;
	if (updateGUICounter > 20)
	{
		qApp->processEvents();
		updateGUICounter = 0;
	}
}

void SlaOutputDev::pushGroup(QString maskName, GBool forSoftMask, GBool alpha, bool inverted)
{
	groupEntry gElements;
	gElements.forSoftMask = forSoftMask;
	gElements.alpha = alpha;
	gElements.inverted = inverted;
	gElements.maskName = maskName;
	m_groupStack.push(gElements);
}
