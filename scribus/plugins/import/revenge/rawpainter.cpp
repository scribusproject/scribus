/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QByteArray>
#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QStack>
#include <QTextDocument>
#include <QDebug>

#include <cstdlib>

#include "color.h"
#include "commonstrings.h"
#include "fileloader.h"
#include "loadsaveplugin.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "selection.h"
#include "ui/missing.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"
#include "rawpainter.h"


extern SCRIBUS_API ScribusQApp * ScQApp;

#if HAVE_REVENGE

struct RawPainterPrivate
{
	RawPainterPrivate();
};

RawPainterPrivate::RawPainterPrivate()
{
}

RawPainter::RawPainter(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel, QString fTyp) : m_pImpl(new RawPainterPrivate())
{
	m_Doc = Doc;
	baseX = x;
	baseY = y;
	docWidth = w;
	docHeight = h;
	importerFlags = iflags;
	Elements = Elem;
	importedColors = iColors;
	importedPatterns = iPatterns;
	tmpSel = tSel;
	doProcessing = true;
	CurrColorFill = "Black";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	CurrStrokeTrans = 0.0;
	CurrFillTrans = 0.0;
	Coords.resize(0);
	Coords.svgInit();
	LineW = 1.0;
	lineJoin = Qt::MiterJoin;
	lineEnd = Qt::FlatCap;
	fillrule = true;
	gradientAngle = 0.0;
	isGradient = false;
	lineSpSet = false;
	currentGradient = VGradient(VGradient::linear);
	currentGradient.clearStops();
	currentGradient.setRepeatMethod( VGradient::none );
	dashArray.clear();
	firstPage = true;
	actPage = 0;
	actTextItem = NULL;
	fileType = fTyp;
}

RawPainter::~RawPainter()
{
	delete m_pImpl;
}

void RawPainter::startDocument(const librevenge::RVNGPropertyList &propList)
{
}

void RawPainter::endDocument()
{
}

void RawPainter::setDocumentMetaData(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "setDocumentMetaData";
}

void RawPainter::defineEmbeddedFont(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "defineEmbeddedFont";
}

void RawPainter::startPage(const librevenge::RVNGPropertyList &propList)
{
	if (propList["svg:width"])
		docWidth = valueAsPoint(propList["svg:width"]);
	if (propList["svg:height"])
		docHeight = valueAsPoint(propList["svg:height"]);
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (!firstPage)
		{
			m_Doc->addPage(actPage);
			m_Doc->setActiveLayer(baseLayer);
		}
		else
			baseLayer = m_Doc->activeLayerName();
		m_Doc->setPageSize("Custom");
		m_Doc->currentPage()->setInitialWidth(docWidth);
		m_Doc->currentPage()->setInitialHeight(docHeight);
		m_Doc->currentPage()->setWidth(docWidth);
		m_Doc->currentPage()->setHeight(docHeight);
		m_Doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
		m_Doc->currentPage()->m_pageSize = "Custom";
		m_Doc->reformPages(true);
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	firstPage = false;
	actPage++;
}

void RawPainter::endPage()
{
	if ((importerFlags & LoadSavePlugin::lfCreateThumbnail) || (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
		doProcessing = false;
}

void RawPainter::startMasterPage(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "startMasterPage";
}

void RawPainter::endMasterPage()
{
	if (!doProcessing)
		return;
	qDebug() << "endMasterPage";
}

void RawPainter::startLayer(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	FPointArray clip;
	if (propList["svg:clip-path"])
	{
		QString svgString = QString(propList["svg:clip-path"]->getStr().cstr());
		clip.resize(0);
		clip.svgInit();
		svgString.replace(",", ".");
		clip.parseSVG(svgString);
		QTransform m;
		m.scale(72.0, 72.0);
		clip.map(m);
	}
	QList<PageItem*> gElements;
	groupEntry gr;
	gr.clip = clip.copy();
	gr.Items = gElements;
	groupStack.push(gr);
}

void RawPainter::endLayer()
{
	if (!doProcessing)
		return;
	if (groupStack.count() != 0)
	{
		PageItem *ite;
		groupEntry gr = groupStack.pop();
		QList<PageItem*> gElements = gr.Items;
		tmpSel->clear();
		if (gElements.count() > 0)
		{
			bool groupClip = true;
			for (int dre = 0; dre < gElements.count(); ++dre)
			{
				tmpSel->addItem(gElements.at(dre), true);
				Elements->removeAll(gElements.at(dre));
				if (gElements.at(dre)->hasSoftShadow())
					groupClip = false;
			}
			ite = m_Doc->groupObjectsSelection(tmpSel);
			ite->setGroupClipping(groupClip);
			ite->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
			if (!gr.clip.isEmpty())
			{
				double oldX = ite->xPos();
				double oldY = ite->yPos();
				double oldW = ite->width();
				double oldH = ite->height();
				double oldgW = ite->groupWidth;
				double oldgH = ite->groupHeight;
				ite->PoLine = gr.clip.copy();
				ite->PoLine.translate(baseX, baseY);
				FPoint xy = getMinClipF(&ite->PoLine);
				ite->setXYPos(xy.x(), xy.y(), true);
				ite->PoLine.translate(-xy.x(), -xy.y());
				FPoint wh = getMaxClipF(&ite->PoLine);
				ite->setWidthHeight(wh.x(),wh.y());
				ite->groupWidth = oldgW * (ite->width() / oldW);
				ite->groupHeight = oldgH * (ite->height() / oldH);
				double dx = (ite->xPos() - oldX) / (ite->width() / ite->groupWidth);
				double dy = (ite->yPos() - oldY) / (ite->height() / ite->groupHeight);
				for (int em = 0; em < ite->groupItemList.count(); ++em)
				{
					PageItem* embedded = ite->groupItemList.at(em);
					embedded->moveBy(-dx, -dy, true);
					m_Doc->setRedrawBounding(embedded);
					embedded->OwnPage = m_Doc->OnPage(embedded);
				}
				ite->ClipEdited = true;
				ite->OldB2 = ite->width();
				ite->OldH2 = ite->height();
				ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
				ite->updateGradientVectors();
			}
			Elements->append(ite);
			if (groupStack.count() != 0)
				groupStack.top().Items.append(ite);
		}
		tmpSel->clear();
	}
}

void RawPainter::startEmbeddedGraphics(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "startEmbeddedGraphics";
}

void RawPainter::endEmbeddedGraphics()
{
	if (!doProcessing)
		return;
	qDebug() << "endEmbeddedGraphics";
}

void RawPainter::openGroup(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	startLayer(propList);
}

void RawPainter::closeGroup()
{
	if (!doProcessing)
		return;
	endLayer();
}

void RawPainter::setStyle(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	CurrColorFill = "Black";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	CurrStrokeTrans = 0.0;
	CurrFillTrans = 0.0;
	Coords.resize(0);
	Coords.svgInit();
	LineW = 1.0;
	lineJoin = Qt::MiterJoin;
	lineEnd = Qt::FlatCap;
	fillrule = true;
	gradientAngle = 0.0;
	isGradient = false;
	lineSpSet = false;
	currentGradient = VGradient(VGradient::linear);
	currentGradient.clearStops();
	currentGradient.setRepeatMethod( VGradient::none );
	dashArray.clear();
	m_style.clear();
	m_style = propList;
	isGradient = false;
	if(propList["draw:fill"] && propList["draw:fill"]->getStr() == "none")
		CurrColorFill = CommonStrings::None;
	else if(propList["draw:fill"] && propList["draw:fill"]->getStr() == "solid")
	{
		if (propList["draw:fill-color"])
		{
			CurrColorFill = parseColor(QString(propList["draw:fill-color"]->getStr().cstr()));
			if(propList["draw:opacity"])
				CurrFillTrans = 1.0 - qMin(1.0, qMax(fromPercentage(QString(propList["draw:opacity"]->getStr().cstr())), 0.0));
		}
	}
	if(propList["draw:fill"] && propList["draw:fill"]->getStr() == "gradient")
	{
	//	double angle = 0;
	//	if (propList["draw:angle"])
	//		angle = propList["draw:angle"]->getDouble();
		double opacity = 1.0;
		librevenge::RVNGPropertyListVector gradient;
		if (propList.child("svg:linearGradient"))
		{
			currentGradient = VGradient(VGradient::linear);
			gradient = *propList.child("svg:linearGradient");
		}
		else if (propList.child("svg:radialGradient"))
		{
			currentGradient = VGradient(VGradient::radial);
			gradient = *propList.child("svg:radialGradient");
		}
		if (gradient.count() > 1)
		{
			currentGradient.clearStops();
			currentGradient.setRepeatMethod( VGradient::pad );
			double dr = 1.0 / static_cast<double>(gradient.count());
			for (unsigned c = 0; c < gradient.count(); c++)
			{
				librevenge::RVNGPropertyList grad = gradient[c];
				if (grad["svg:stop-color"])
				{
					QString stopName = parseColor(QString(grad["svg:stop-color"]->getStr().cstr()));
					double rampPoint = dr * c;
					if(grad["svg:offset"])
						rampPoint = fromPercentage(QString(grad["svg:offset"]->getStr().cstr()));
					const ScColor& gradC = m_Doc->PageColors[stopName];
					if(grad["svg:stop-opacity"])
						opacity = qMin(1.0, qMax(fromPercentage(QString(grad["svg:stop-opacity"]->getStr().cstr())), 0.0));
					currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), rampPoint, 0.5, opacity, stopName, 100 );
					if (c == 0)
					{
						gradColor1Str = stopName;
						gradColor1 = ScColorEngine::getRGBColor(gradC, m_Doc);
						gradColor1Trans = opacity;
					}
					else
					{
						gradColor2Str = stopName;
						gradColor2 = ScColorEngine::getRGBColor(gradC, m_Doc);
						gradColor2Trans = opacity;
					}
				}
			}
			if (currentGradient.Stops() > 1)
				isGradient = true;
		}
		else
		{
			gradColor1Str = "Black";
			gradColor2Str = "Black";
			if (propList["draw:start-color"])
				gradColor2Str = parseColor(QString(propList["draw:start-color"]->getStr().cstr()));
			if (propList["draw:end-color"])
				gradColor1Str = parseColor(QString(propList["draw:end-color"]->getStr().cstr()));
			double opacity = 1.0;
			currentGradient = VGradient(VGradient::linear);
			currentGradient.clearStops();
			currentGradient.setRepeatMethod( VGradient::none );

			if (propList["draw:style"])
			{
				if (QString(propList["draw:style"]->getStr().cstr()) == "axial")
				{
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor1Str], m_Doc), 0.0, 0.5, opacity, gradColor1Str, 100 );
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor2Str], m_Doc), 0.5, 0.5, opacity, gradColor2Str, 100 );
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor1Str], m_Doc), 1.0, 0.5, opacity, gradColor1Str, 100 );
				}
				else
				{
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor1Str], m_Doc), 0.0, 0.5, opacity, gradColor1Str, 100 );
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor2Str], m_Doc), 1.0, 0.5, opacity, gradColor2Str, 100 );
				}
				isGradient = true;
			}
		}
	}
	if(propList["svg:fill-rule"])
	{
		if (QString(propList["svg:fill-rule"]->getStr().cstr()) == "nonzero")
			fillrule = false;
		else
			fillrule = true;
	}
	if (propList["svg:stroke-width"])
		LineW = valueAsPoint(propList["svg:stroke-width"]);
	if (propList["draw:stroke"])
	{
		if (propList["draw:stroke"]->getStr() == "none")
			CurrColorStroke = CommonStrings::None;
		else if ((propList["draw:stroke"]->getStr() == "solid") || (propList["draw:stroke"]->getStr() == "dash"))
		{
			if (propList["svg:stroke-color"])
			{
				CurrColorStroke = parseColor(QString(propList["svg:stroke-color"]->getStr().cstr()));
				if(propList["svg:stroke-opacity"])
					CurrStrokeTrans = 1.0 - qMin(1.0, qMax(fromPercentage(QString(propList["svg:stroke-opacity"]->getStr().cstr())), 0.0));
			}
			if (propList["draw:stroke"]->getStr() == "dash")
			{
				dashArray.clear();
				double gap = LineW;
				if (propList["draw:distance"])
					gap = valueAsPoint(propList["draw:distance"]);
				int dots1 = 0;
				if (propList["draw:dots1"])
					dots1 = propList["draw:dots1"]->getInt();
				double dots1len = LineW;
				if (propList["draw:dots1-length"])
					dots1len = valueAsPoint(propList["draw:dots1-length"]);
				int dots2 = 0;
				if (propList["draw:dots2"])
					dots2 = propList["draw:dots2"]->getInt();
				double dots2len = LineW;
				if (propList["draw:dots2-length"])
					dots2len = valueAsPoint(propList["draw:dots2-length"]);
				for (int i = 0; i < dots1; i++)
				{
					dashArray << qMax(dots1len, 0.1) << qMax(gap, 0.1);
				}
				for (int j = 0; j < dots2; j++)
				{
					dashArray << qMax(dots2len, 0.1) << qMax(gap, 0.1);
				}
			}
			else
				dashArray.clear();
		}
	}
	if (propList["svg:stroke-linecap"])
	{
		QString params = QString(propList["svg:stroke-linecap"]->getStr().cstr());
		if( params == "butt" )
			lineEnd = Qt::FlatCap;
		else if( params == "round" )
			lineEnd = Qt::RoundCap;
		else if( params == "square" )
			lineEnd = Qt::SquareCap;
		else
			lineEnd = Qt::FlatCap;
	}
	if (propList["svg:stroke-linejoin"])
	{
		QString params = QString(propList["svg:stroke-linejoin"]->getStr().cstr());
		if( params == "miter" )
			lineJoin = Qt::MiterJoin;
		else if( params == "round" )
			lineJoin = Qt::RoundJoin;
		else if( params == "bevel" )
			lineJoin = Qt::BevelJoin;
		else
			lineJoin = Qt::MiterJoin;
	}
}

void RawPainter::drawRectangle(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		if ((fileType == "pmd") || (fileType == "pm5") || (fileType == "p65"))
			setStyle(propList);
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x, baseY + y, w, h, LineW, CurrColorFill, CurrColorStroke, true);
		PageItem *ite = m_Doc->Items->at(z);
		finishItem(ite);
		applyFill(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
}

void RawPainter::drawEllipse(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		if ((fileType == "pmd") || (fileType == "pm5") || (fileType == "p65"))
			setStyle(propList);
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, w, h, LineW, CurrColorFill, CurrColorStroke, true);
		PageItem *ite = m_Doc->Items->at(z);
		finishItem(ite);
		applyFill(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
}

void RawPainter::drawPolyline(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	librevenge::RVNGPropertyListVector vertices = *propList.child("svg:points");
	if(vertices.count() < 2)
		return;
	if ((fileType == "pmd") || (fileType == "pm5") || (fileType == "p65"))
		setStyle(propList);
	Coords.resize(0);
	Coords.svgInit();
	PageItem *ite;
	Coords.svgMoveTo(valueAsPoint(vertices[0]["svg:x"]), valueAsPoint(vertices[0]["svg:y"]));
	for(unsigned i = 1; i < vertices.count(); i++)
	{
		Coords.svgLineTo(valueAsPoint(vertices[i]["svg:x"]), valueAsPoint(vertices[i]["svg:y"]));
	}
	if (Coords.size() > 0)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		finishItem(ite);
		applyArrows(ite);
	}
}

void RawPainter::drawPolygon(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	librevenge::RVNGPropertyListVector vertices = *propList.child("svg:points");
	if(vertices.count() < 2)
		return;
	if ((fileType == "pmd") || (fileType == "pm5") || (fileType == "p65"))
		setStyle(propList);
	Coords.resize(0);
	Coords.svgInit();
	PageItem *ite;
	int z;
	Coords.svgMoveTo(valueAsPoint(vertices[0]["svg:x"]), valueAsPoint(vertices[0]["svg:y"]));
	for(unsigned i = 1; i < vertices.count(); i++)
	{
		Coords.svgLineTo(valueAsPoint(vertices[i]["svg:x"]), valueAsPoint(vertices[i]["svg:y"]));
	}
	Coords.svgClosePath();
	if (Coords.size() > 0)
	{
		if(m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap" && m_style["style:repeat"] && m_style["style:repeat"]->getStr() == "stretch")
		{
		  if (m_style["draw:fill-image"] && m_style["librevenge:mime-type"])
		  {
			  QByteArray ba(m_style["draw:fill-image"]->getStr().cstr());
			  QByteArray imageData = QByteArray::fromBase64(ba);
			  QString imgExt = "";
			  if (m_style["librevenge:mime-type"]->getStr() == "image/png")
				  imgExt = "png";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/jpeg")
				  imgExt = "jpg";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/bmp")
				  imgExt = "bmp";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/pict")
				  imgExt = "pict";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/tiff")
				  imgExt = "tif";
			  if (!imgExt.isEmpty())
			  {
				  z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
				  ite = m_Doc->Items->at(z);
				  ite->PoLine = Coords.copy();
				  finishItem(ite);
				  insertImage(ite, imgExt, imageData);
			  }
			  else if ((m_style["librevenge:mime-type"]->getStr() == "image/wmf") || (m_style["librevenge:mime-type"]->getStr() == "image/emf"))
			  {
				  if (m_style["librevenge:mime-type"]->getStr() == "image/wmf")
					  imgExt = "wmf";
				  else
					  imgExt = "emf";
				  QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
				  if (tempFile->open())
				  {
					  tempFile->write(imageData);
					  QString fileName = getLongPathName(tempFile->fileName());
					  tempFile->close();
					  FileLoader *fileLoader = new FileLoader(fileName);
					  int testResult = fileLoader->testFile();
					  delete fileLoader;
					  if (testResult != -1)
					  {
						  const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
						  if( fmt )
						  {
							  fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
							  fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
							  if (m_Doc->m_Selection->count() > 0)
							  {
								  ite = m_Doc->groupObjectsSelection();
								  double rot = 0;
								  if (m_style["librevenge:rotate"])
									  rot = m_style["librevenge:rotate"]->getDouble();
								  QPainterPath ba = Coords.toQPainterPath(true);
								  QRectF baR = ba.boundingRect();
								  if (rot != 0)
								  {
									  QTransform mm;
									  mm.translate(baR.x(), baR.y());
									  mm.translate(baR.width() / 2.0, baR.height() / 2.0);
									  mm.rotate(rot);
									  mm.translate(-baR.width() / 2.0, -baR.height() / 2.0);
									  mm.translate(-baR.x(), -baR.y());
									  ba = mm.map(ba);
									  baR = ba.boundingRect();
									  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									  ite->setWidthHeight(baR.width(), baR.height(), true);
									  Coords.fromQPainterPath(ba, true);
									  FPoint tp2(getMinClipF(&Coords));
									  Coords.translate(-tp2.x(), -tp2.y());
									  ite->PoLine = Coords.copy();
									  int rm = m_Doc->RotMode();
									  m_Doc->RotMode(2);
									  m_Doc->RotateItem(-rot, ite);
									  m_Doc->RotMode(rm);
								  }
								  else
								  {
									  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									  ite->setWidthHeight(baR.width(), baR.height(), true);
									  FPoint tp2(getMinClipF(&Coords));
									  Coords.translate(-tp2.x(), -tp2.y());
									  ite->PoLine = Coords.copy();
								  }
								  finishItem(ite);
								  if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
								  {
									  int r = qRound(m_style["draw:red"]->getDouble() * 255);
									  int g = qRound(m_style["draw:green"]->getDouble() * 255);
									  int b = qRound(m_style["draw:blue"]->getDouble() * 255);
									  QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
									  QString efVal = parseColor(colVal);
									  recolorItem(ite, efVal);
								  }
							  }
						  }
					  }
				  }
				  delete tempFile;
			  }
		  }
		}
		else
		{
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
			ite = m_Doc->Items->at(z);
			ite->PoLine = Coords.copy();
			finishItem(ite);
			applyFill(ite);
		}
		applyFlip(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
}

void RawPainter::drawPath(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	if ((fileType == "pmd") || (fileType == "pm5") || (fileType == "p65"))
		setStyle(propList);
	librevenge::RVNGPropertyListVector path = *propList.child("svg:d");
	bool isClosed = false;
	QString svgString = "";
	for(unsigned i=0; i < path.count(); i++)
	{
		librevenge::RVNGPropertyList pList = path[i];
		if (pList["librevenge:path-action"] && pList["librevenge:path-action"]->getStr() == "M")
			svgString += QString("M %1 %2 ").arg(valueAsPoint(pList["svg:x"])).arg(valueAsPoint(pList["svg:y"]));
		else if (pList["librevenge:path-action"] && pList["librevenge:path-action"]->getStr() == "L")
			svgString += QString("L %1 %2 ").arg(valueAsPoint(pList["svg:x"])).arg(valueAsPoint(pList["svg:y"]));
		else if (pList["librevenge:path-action"] && pList["librevenge:path-action"]->getStr() == "C")
			svgString += QString("C %1 %2 %3 %4 %5 %6 ").arg(valueAsPoint(pList["svg:x1"])).arg(valueAsPoint(pList["svg:y1"])).arg(valueAsPoint(pList["svg:x2"])).arg(valueAsPoint(pList["svg:y2"])).arg(valueAsPoint(pList["svg:x"])).arg(valueAsPoint(pList["svg:y"]));
		else if (propList["librevenge:path-action"] && propList["librevenge:path-action"]->getStr() == "Q")
			svgString += QString("Q %1 %2 %3 %4 ").arg(valueAsPoint(pList["svg:x1"])).arg(valueAsPoint(pList["svg:y1"])).arg(valueAsPoint(pList["svg:x"])).arg(valueAsPoint(pList["svg:y"]));
		else if (pList["librevenge:path-action"] && pList["librevenge:path-action"]->getStr() == "A")
			svgString += QString("A %1 %2 %3 %4 %5 %6 %7") .arg(valueAsPoint(pList["svg:rx"])) .arg(valueAsPoint(pList["svg:ry"])).arg(pList["librevenge:rotate"] ? pList["librevenge:rotate"]->getDouble() : 0).arg(pList["librevenge:large-arc"] ? pList["librevenge:large-arc"]->getInt() : 1).arg(pList["librevenge:sweep"] ? pList["librevenge:sweep"]->getInt() : 1).arg(valueAsPoint(pList["svg:x"])).arg(valueAsPoint(pList["svg:y"]));
		else if ((i >= path.count()-1 && i > 2) && pList["librevenge:path-action"] && pList["librevenge:path-action"]->getStr() == "Z" )
		{
			isClosed = true;
			svgString += "Z";
		}
	}
	Coords.resize(0);
	Coords.svgInit();
	Coords.parseSVG(svgString);
	PageItem *ite;
	int z;
	if (isClosed)
	{
		if(m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap" && m_style["style:repeat"] && m_style["style:repeat"]->getStr() == "stretch")
		{
		  if (m_style["draw:fill-image"] && m_style["librevenge:mime-type"])
		  {
			  QByteArray ba(m_style["draw:fill-image"]->getStr().cstr());
			  QByteArray imageData = QByteArray::fromBase64(ba);
			  QString imgExt = "";
			  if (m_style["librevenge:mime-type"]->getStr() == "image/png")
				  imgExt = "png";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/jpeg")
				  imgExt = "jpg";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/bmp")
				  imgExt = "bmp";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/pict")
				  imgExt = "pict";
			  else if (m_style["librevenge:mime-type"]->getStr() == "image/tiff")
				  imgExt = "tif";
			  if (!imgExt.isEmpty())
			  {
				  z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
				  ite = m_Doc->Items->at(z);
				  ite->PoLine = Coords.copy();
				  finishItem(ite);
				  insertImage(ite, imgExt, imageData);
			  }
			  else if ((m_style["librevenge:mime-type"]->getStr() == "image/wmf") || (m_style["librevenge:mime-type"]->getStr() == "image/emf"))
			  {
				  if (m_style["librevenge:mime-type"]->getStr() == "image/wmf")
					  imgExt = "wmf";
				  else
					  imgExt = "emf";
				  QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
				  if (tempFile->open())
				  {
					  tempFile->write(imageData);
					  QString fileName = getLongPathName(tempFile->fileName());
					  tempFile->close();
					  FileLoader *fileLoader = new FileLoader(fileName);
					  int testResult = fileLoader->testFile();
					  delete fileLoader;
					  if (testResult != -1)
					  {
						  const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
						  if( fmt )
						  {
							  fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
							  fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
							  if (m_Doc->m_Selection->count() > 0)
							  {
								  ite = m_Doc->groupObjectsSelection();
								  double rot = 0;
								  if (m_style["librevenge:rotate"])
									  rot = m_style["librevenge:rotate"]->getDouble();
								  QPainterPath ba = Coords.toQPainterPath(true);
								  QRectF baR = ba.boundingRect();
								  if (rot != 0)
								  {
									  QTransform mm;
									  mm.translate(baR.x(), baR.y());
									  mm.translate(baR.width() / 2.0, baR.height() / 2.0);
									  mm.rotate(rot);
									  mm.translate(-baR.width() / 2.0, -baR.height() / 2.0);
									  mm.translate(-baR.x(), -baR.y());
									  ba = mm.map(ba);
									  baR = ba.boundingRect();
									  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									  ite->setWidthHeight(baR.width(), baR.height(), true);
									  Coords.fromQPainterPath(ba, true);
									  FPoint tp2(getMinClipF(&Coords));
									  Coords.translate(-tp2.x(), -tp2.y());
									  ite->PoLine = Coords.copy();
									  int rm = m_Doc->RotMode();
									  m_Doc->RotMode(2);
									  m_Doc->RotateItem(-rot, ite);
									  m_Doc->RotMode(rm);
								  }
								  else
								  {
									  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									  ite->setWidthHeight(baR.width(), baR.height(), true);
									  FPoint tp2(getMinClipF(&Coords));
									  Coords.translate(-tp2.x(), -tp2.y());
									  ite->PoLine = Coords.copy();
								  }
								  finishItem(ite);
								  if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
								  {
									  int r = qRound(m_style["draw:red"]->getDouble() * 255);
									  int g = qRound(m_style["draw:green"]->getDouble() * 255);
									  int b = qRound(m_style["draw:blue"]->getDouble() * 255);
									  QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
									  QString efVal = parseColor(colVal);
									  recolorItem(ite, efVal);
								  }
							  }
						  }
					  }
				  }
				  delete tempFile;
			  }
		  }
		}
		else
		{
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
			ite = m_Doc->Items->at(z);
			ite->PoLine = Coords.copy();
			finishItem(ite);
			applyFill(ite);
		}
		applyFlip(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
	else
	{
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		finishItem(ite);
		applyArrows(ite);
	}
}

void RawPainter::drawGraphicObject(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	if (!propList["librevenge:mime-type"] || propList["librevenge:mime-type"]->getStr().len() <= 0)
		return;
	if (!propList["office:binary-data"])
		return;
	if ((fileType == "pmd") || (fileType == "pm5") || (fileType == "p65"))
		setStyle(propList);
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		PageItem *ite;
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		QByteArray ba(propList["office:binary-data"]->getStr().cstr());
		QByteArray imageData = QByteArray::fromBase64(ba);
		QString imgExt = "";
		if (propList["librevenge:mime-type"]->getStr() == "image/png")
			imgExt = "png";
		else if (propList["librevenge:mime-type"]->getStr() == "image/jpeg")
			imgExt = "jpg";
		else if (propList["librevenge:mime-type"]->getStr() == "image/bmp")
			imgExt = "bmp";
		else if (propList["librevenge:mime-type"]->getStr() == "image/pict")
			imgExt = "pict";
		else if (propList["librevenge:mime-type"]->getStr() == "image/tiff")
			imgExt = "tif";
		if (!imgExt.isEmpty())
		{
			int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, baseX + x, baseY + y, w, h, 0, CurrColorFill, CurrColorStroke, true);
			ite = m_Doc->Items->at(z);
			finishItem(ite);
			insertImage(ite, imgExt, imageData);
		}
		else
		{
			if ((propList["librevenge:mime-type"]->getStr() == "image/wmf") || (propList["librevenge:mime-type"]->getStr() == "image/emf"))
			{
				QString imgExt = "";
				if (propList["librevenge:mime-type"]->getStr() == "image/wmf")
					imgExt = "wmf";
				else
					imgExt = "emf";
				QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
				if (tempFile->open())
				{
					tempFile->write(imageData);
					QString fileName = getLongPathName(tempFile->fileName());
					tempFile->close();
					FileLoader *fileLoader = new FileLoader(fileName);
					int testResult = fileLoader->testFile();
					delete fileLoader;
					if (testResult != -1)
					{
						const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
						if( fmt )
						{
							fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
							fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
							if (m_Doc->m_Selection->count() > 0)
							{
								ite = m_Doc->groupObjectsSelection();
								double rot = 0;
								if (m_style["librevenge:rotate"])
									rot = m_style["librevenge:rotate"]->getDouble();
								QPainterPath ba;
								ba.addRect(QRectF(x, y, w, h));
								QRectF baR = QRectF(x, y, w, h);
								if (rot != 0)
								{
									QTransform mm;
									mm.translate(baR.x(), baR.y());
									mm.translate(baR.width() / 2.0, baR.height() / 2.0);
									mm.rotate(rot);
									mm.translate(-baR.width() / 2.0, -baR.height() / 2.0);
									mm.translate(-baR.x(), -baR.y());
									ba = mm.map(ba);
									baR = ba.boundingRect();
									ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									ite->setWidthHeight(baR.width(), baR.height(), true);
									ite->updateClip();
									int rm = m_Doc->RotMode();
									m_Doc->RotMode(2);
									m_Doc->RotateItem(-rot, ite);
									m_Doc->RotMode(rm);
								}
								else
								{
									ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									ite->setWidthHeight(baR.width(), baR.height(), true);
									ite->updateClip();
								}
								finishItem(ite);
								if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
								{
									int r = qRound(m_style["draw:red"]->getDouble() * 255);
									int g = qRound(m_style["draw:green"]->getDouble() * 255);
									int b = qRound(m_style["draw:blue"]->getDouble() * 255);
									QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
									QString efVal = parseColor(colVal);
									recolorItem(ite, efVal);
								}
							}
						}
					}
				}
				delete tempFile;
			}
		}
		applyFlip(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
}

void RawPainter::drawConnector(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "drawConnector";
}

void RawPainter::startTextObject(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	actTextItem = NULL;
	lineSpSet = false;
	lineSpIsPT = false;
	if ((fileType == "pmd") || (fileType == "pm5") || (fileType == "p65"))
		setStyle(propList);
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		double minTxtH = 1;
		double minTxtW = 1;
		double rot = 0;
		if (propList["librevenge:rotate"])
			rot = propList["librevenge:rotate"]->getDouble();
		int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, baseX + x, baseY + y, w, qMax(h, 2.0), 0, CurrColorFill, CurrColorStroke, true);
		PageItem *ite = m_Doc->Items->at(z);
		finishItem(ite);
		applyShadow(ite);
		if (rot != 0)
		{
			int rm = m_Doc->RotMode();
			m_Doc->RotMode(2);
			m_Doc->RotateItem(rot, ite);
			m_Doc->RotMode(rm);
		}
		if (propList["draw-mirror-horizontal"])
			ite->flipImageH();
		if (propList["draw-mirror-vertical"])
			ite->flipImageV();
		if (propList["fo:padding-left"])
		{
			ite->setTextToFrameDistLeft(valueAsPoint(propList["fo:padding-left"]));
			minTxtW += valueAsPoint(propList["fo:padding-left"]);
		}
		if (propList["fo:padding-right"])
		{
			ite->setTextToFrameDistRight(valueAsPoint(propList["fo:padding-right"]));
			minTxtW += valueAsPoint(propList["fo:padding-right"]);
		}
		if (propList["fo:padding-top"])
		{
			ite->setTextToFrameDistTop(valueAsPoint(propList["fo:padding-top"]));
			minTxtH += valueAsPoint(propList["fo:padding-top"]);
		}
		if (propList["fo:padding-bottom"])
		{
			ite->setTextToFrameDistBottom(valueAsPoint(propList["fo:padding-bottom"]));
			minTxtH += valueAsPoint(propList["fo:padding-bottom"]);
		}
		if (propList["fo:column-count"])
			ite->setColumns(propList["fo:column-count"]->getInt());
		if (propList["fo:column-gap"])
		{
			ite->setColumnGap(valueAsPoint(propList["fo:column-gap"]));
			minTxtW += valueAsPoint(propList["fo:column-gap"]);
		}
		if (propList["draw:textarea-vertical-align"])
		{
			QString align = QString(propList["draw:textarea-vertical-align"]->getStr().cstr());
			if (align == "middle")
				ite->setVerticalAlignment(1);
			else if (align == "bottom")
				ite->setVerticalAlignment(2);
		}
		ite->setFirstLineOffset(FLOPLineSpacing);
		actTextItem = ite;
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		textStyle = newStyle;
		if (h == 0)
		{
			minTxtH += textStyle.charStyle().fontSize() / 10.0;
			ite->setHeight(minTxtH);
		}
		if (w == 0)
			ite->setWidth(minTxtW);
	}
}

void RawPainter::endTextObject()
{
	if (!doProcessing)
		return;
	if (actTextItem)
		actTextItem->itemText.trim();
	actTextItem = NULL;
	lineSpSet = false;
	lineSpIsPT = false;
}

void RawPainter::startTableObject(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "startTableObject";
}

void RawPainter::openTableRow(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "openTableRow";
}

void RawPainter::closeTableRow()
{
	if (!doProcessing)
		return;
	qDebug() << "closeTableRow";
}

void RawPainter::openTableCell(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "openTableCell";
}

void RawPainter::closeTableCell()
{
	if (!doProcessing)
		return;
	qDebug() << "closeTableCell";
}

void RawPainter::insertCoveredTableCell(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "insertCoveredTableCell";
}

void RawPainter::endTableObject()
{
	if (!doProcessing)
		return;
	qDebug() << "endTableObject";
}

void RawPainter::openOrderedListLevel(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "openOrderedListLevel";
}

void RawPainter::closeOrderedListLevel()
{
	if (!doProcessing)
		return;
	qDebug() << "closeOrderedListLevel";
}

void RawPainter::openUnorderedListLevel(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "openUnorderedListLevel";
}

void RawPainter::closeUnorderedListLevel()
{
	if (!doProcessing)
		return;
	qDebug() << "closeUnorderedListLevel";
}

void RawPainter::openListElement(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "openListElement";
}

void RawPainter::closeListElement()
{
	if (!doProcessing)
		return;
	qDebug() << "closeListElement";
}

void RawPainter::defineParagraphStyle(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "defineParagraphStyle";
}

void RawPainter::openParagraph(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	textStyle = newStyle;
	if (propList["fo:text-align"])
	{
		QString align = QString(propList["fo:text-align"]->getStr().cstr());
		if (align == "left")
			textStyle.setAlignment(ParagraphStyle::Leftaligned);
		else if (align == "center")
			textStyle.setAlignment(ParagraphStyle::Centered);
		else if (align == "right")
			textStyle.setAlignment(ParagraphStyle::Rightaligned);
		else if (align == "justify")
			textStyle.setAlignment(ParagraphStyle::Justified);
	}
	if (propList["fo:margin-left"])
		textStyle.setLeftMargin(valueAsPoint(propList["fo:margin-left"]));
	if (propList["fo:margin-right"])
		textStyle.setRightMargin(valueAsPoint(propList["fo:margin-right"]));
	if (propList["fo:text-indent"])
		textStyle.setFirstIndent(valueAsPoint(propList["fo:text-indent"]));
	if (propList["style:drop-cap"])
	{
		textStyle.setDropCapLines(propList["style:drop-cap"]->getInt());
		textStyle.setHasDropCap(true);
	}
	if (propList["fo:margin-bottom"])
		textStyle.setGapAfter(valueAsPoint(propList["fo:margin-bottom"]));
	if (propList["fo:margin-top"])
		textStyle.setGapBefore(valueAsPoint(propList["fo:margin-top"]));
//	m_maxFontSize = textStyle.charStyle().fontSize() / 10.0;
	m_maxFontSize = 1.0;
	if (propList["fo:line-height"])
	{
		m_linespace = propList["fo:line-height"]->getDouble();
		QString lsp = QString(propList["fo:line-height"]->getStr().cstr());
		lineSpIsPT = lsp.endsWith("pt");
		lineSpSet = true;
	}
}

void RawPainter::closeParagraph()
{
	if (!doProcessing)
		return;
	if (actTextItem == NULL)
		return;
	int posT = actTextItem->itemText.length();
	if (posT > 0)
	{
		if ((actTextItem->itemText.text(posT - 1) != SpecialChars::PARSEP))
		{
			actTextItem->itemText.insertChars(posT, SpecialChars::PARSEP);
			actTextItem->itemText.applyStyle(posT, textStyle);
		}
	}
}

void RawPainter::defineCharacterStyle(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "defineCharacterStyle";
}

void RawPainter::openSpan(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	textCharStyle = textStyle.charStyle();
	if (propList["fo:font-size"])
	{
		textCharStyle.setFontSize(valueAsPoint(propList["fo:font-size"]) * 10.0);
		m_maxFontSize = qMax(m_maxFontSize, valueAsPoint(propList["fo:font-size"]));
	}
	if (propList["style:text-scale"])
		textCharStyle.setFontSize(textCharStyle.fontSize() * fromPercentage(QString(propList["style:text-scale"]->getStr().cstr())));
	if (propList["fo:color"])
		textCharStyle.setFillColor(parseColor(QString(propList["fo:color"]->getStr().cstr())));
	if (propList["style:font-name"])
	{
		QString fontVari = "";
		if (propList["fo:font-weight"])
			fontVari = QString(propList["fo:font-weight"]->getStr().cstr());
		QString fontName = QString(propList["style:font-name"]->getStr().cstr());
		QString realFontName = constructFontName(fontName, fontVari);
		textCharStyle.setFont((*m_Doc->AllFonts)[realFontName]);
	}
	if (propList["fo:font-name"])
	{
		QString fontVari = "";
		if (propList["fo:font-weight"])
			fontVari = QString(propList["fo:font-weight"]->getStr().cstr());
		QString fontName = QString(propList["fo:font-name"]->getStr().cstr());
		QString realFontName = constructFontName(fontName, fontVari);
		textCharStyle.setFont((*m_Doc->AllFonts)[realFontName]);
	}
	StyleFlag styleEffects = textCharStyle.effects();
	if (propList["style:text-underline-type"])
		styleEffects |= ScStyle_Underline;
	if (propList["style:text-position"])
	{
		if (propList["style:text-position"]->getStr() == "50% 67%")
			styleEffects |= ScStyle_Superscript;
		else
			styleEffects |= ScStyle_Subscript;
	}
	textCharStyle.setFeatures(styleEffects.featureList());
}

void RawPainter::closeSpan()
{
}

void RawPainter::openLink(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "openLink";
}

void RawPainter::closeLink()
{
	if (!doProcessing)
		return;
	qDebug() << "closeLink";
}

void RawPainter::insertTab()
{
	if (!doProcessing)
		return;
	if (actTextItem == NULL)
		return;
	int posT = actTextItem->itemText.length();
	actTextItem->itemText.insertChars(posT, SpecialChars::TAB);
	actTextItem->itemText.applyStyle(posT, textStyle);
}

void RawPainter::insertSpace()
{
	if (!doProcessing)
		return;
	if (actTextItem == NULL)
		return;
	int posT = actTextItem->itemText.length();
	actTextItem->itemText.insertChars(posT, SpecialChars::BLANK);
	actTextItem->itemText.applyStyle(posT, textStyle);
}

void RawPainter::insertText(const librevenge::RVNGString &text)
{
	if (!doProcessing)
		return;
	if (actTextItem == NULL)
		return;
	if (lineSpSet)
	{
		textStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
		if (lineSpIsPT)
			textStyle.setLineSpacing(m_linespace);
		else
			textStyle.setLineSpacing(m_maxFontSize * m_linespace);
	}
	else
		textStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	librevenge::RVNGString tempUTF8(text);
	QString actText = QString(tempUTF8.cstr());
	if (actTextItem)
	{
		int posC = actTextItem->itemText.length();
		if (actText.count() > 0)
		{
			actText.replace(QChar(10), SpecialChars::LINEBREAK);
			actText.replace(QChar(12), SpecialChars::FRAMEBREAK);
			actText.replace(QChar(30), SpecialChars::NBHYPHEN);
			actText.replace(QChar(160), SpecialChars::NBSPACE);
			QTextDocument texDoc;
			texDoc.setHtml(actText);
			actText = texDoc.toPlainText();
			actText = actText.trimmed();
			actTextItem->itemText.insertChars(posC, actText);
			actTextItem->itemText.applyStyle(posC, textStyle);
			actTextItem->itemText.applyCharStyle(posC, actText.length(), textCharStyle);
		}
	}
}

void RawPainter::insertLineBreak()
{
	if (!doProcessing)
		return;
	if (actTextItem == NULL)
		return;
	int posT = actTextItem->itemText.length();
	actTextItem->itemText.insertChars(posT, SpecialChars::LINEBREAK);
	actTextItem->itemText.applyStyle(posT, textStyle);
}

void RawPainter::insertField(const librevenge::RVNGPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "insertField";
}

double RawPainter::valueAsPoint(const librevenge::RVNGProperty *prop)
{
	double value = 0.0;
	QString str = QString(prop->getStr().cstr()).toLower();
	if (str.endsWith("in"))
		value = prop->getDouble() * 72.0;
	else
		value = prop->getDouble();
	return value;
}

void RawPainter::applyFill(PageItem* ite)
{
	if(isGradient)
	{
		QString gradMode = "normal";
		if (m_style["libmspub:shade"])
			gradMode = QString(m_style["libmspub:shade"]->getStr().cstr());
		else if (m_style["draw:style"])
			gradMode = QString(m_style["draw:style"]->getStr().cstr());
		if ((gradMode == "normal") || (gradMode == "linear"))
		{
			int angle = 0;
			if (m_style["draw:angle"])
				angle = qRound(m_style["draw:angle"]->getDouble());
			double h = ite->height();
			double w = ite->width();
			if (angle == 0)
				ite->setGradientVector(w / 2.0, h, w / 2.0, 0, 0, 0, 1, 0);
			else if (angle == -225)
				ite->setGradientVector(w, 0, 0, h, 0, 0, 1, 0);
			else if (angle == 45)
				ite->setGradientVector(w, h, 0, 0, 0, 0, 1, 0);
			else if (angle == 90)
				ite->setGradientVector(w, h / 2.0, 0, h / 2.0, 0, 0, 1, 0);
			else if (angle == 180)
				ite->setGradientVector(w / 2.0, 0, w / 2.0, h, 0, 0, 1, 0);
			else if (angle == 270)
				ite->setGradientVector(0, h / 2.0, w, h / 2.0, 0, 0, 1, 0);
			ite->fill_gradient = currentGradient;
			ite->GrType = 6;
		}
		else if (gradMode == "radial")
		{
			double h = ite->height();
			double w = ite->width();
			double cx = 0.0;
			double cy = 0.0;
			if (m_style["svg:cx"])
				cx = m_style["svg:cx"]->getDouble();
			if (m_style["svg:cy"])
				cy = m_style["svg:cy"]->getDouble();
			ite->setGradientVector(cx, cy, w, h / 2.0, cx, cy, 1, 0);
			ite->fill_gradient = currentGradient;
			ite->GrType = 7;
		}
		else if (gradMode == "square")
		{
			double cx = 0.0;
			double cy = 0.0;
			if (m_style["svg:cx"])
				cx = m_style["svg:cx"]->getDouble();
			if (m_style["svg:cy"])
				cy = m_style["svg:cy"]->getDouble();
			FPoint cp = FPoint(cx, cy);
			ite->setDiamondGeometry(FPoint(0, 0), FPoint(ite->width(), 0), FPoint(ite->width(), ite->height()), FPoint(0, ite->height()), cp);
			ite->fill_gradient.clearStops();
			QList<VColorStop*> colorStops = currentGradient.colorStops();
			for( int a = 0; a < colorStops.count() ; a++ )
			{
				ite->fill_gradient.addStop(colorStops[a]->color, 1.0 - colorStops[a]->rampPoint, colorStops[a]->midPoint, colorStops[a]->opacity, colorStops[a]->name, colorStops[a]->shade);
			}
			ite->GrType = 10;
		}
		else if (gradMode == "center")
		{
			QString center = "top-left";
			FPoint cp = FPoint(0, 0);
			if (m_style["libmspub:shade-ref-point"])
				center = QString(m_style["libmspub:shade-ref-point"]->getStr().cstr());
			if (center == "top-left")
				cp = FPoint(0, 0);
			else if (center == "top-right")
				cp = FPoint(ite->width(), 0);
			else if (center == "bottom-left")
				cp = FPoint(0, ite->height());
			else if (center == "bottom-right")
				cp = FPoint(ite->width(), ite->height());
			ite->setDiamondGeometry(FPoint(0, 0), FPoint(ite->width(), 0), FPoint(ite->width(), ite->height()), FPoint(0, ite->height()), cp);
			ite->fill_gradient.clearStops();
			QList<VColorStop*> colorStops = currentGradient.colorStops();
			for( int a = 0; a < colorStops.count() ; a++ )
			{
				ite->fill_gradient.addStop(colorStops[a]->color, 1.0 - colorStops[a]->rampPoint, colorStops[a]->midPoint, colorStops[a]->opacity, colorStops[a]->name, colorStops[a]->shade);
			}
			ite->GrType = 10;
		}
		else if (gradMode == "shape")
		{
			FPointArray gpath = ite->PoLine.copy();
			for (int sub = 0; sub < 2; sub++)
			{
				FPointArray points;
				double nearT = 0.5;
				uint psize = gpath.size();
				for (uint a = 0; a < psize-3; a += 4)
				{
					if (gpath.isMarker(a))
					{
						points.setMarker();
						continue;
					}
					FPoint base = gpath.point(a);
					FPoint c1 = gpath.point(a+1);
					FPoint base2 =  gpath.point(a+2);
					FPoint c2 = gpath.point(a+3);
					FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
					FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
					FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
					FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
					FPoint bp1 = (1.0 - nearT) * cn2 + nearT * cn3;
					if ((base == c1) && (base2 == c2))
					{
						points.addPoint(base);
						points.addPoint(c1);
						points.addPoint(bp1);
						points.addPoint(bp1);
						points.addPoint(bp1);
						points.addPoint(bp1);
						points.addPoint(base2);
						points.addPoint(c2);
					}
					else
					{
						points.addPoint(base);
						points.addPoint(cn1);
						points.addPoint(bp1);
						points.addPoint(cn2);
						points.addPoint(bp1);
						points.addPoint(cn3);
						points.addPoint(base2);
						points.addPoint(cn4);
					}
				}
				gpath = points;
			}
			ite->meshGradientPatches.clear();
			FPoint center = FPoint(ite->width() / 2.0, ite->height() / 2.0);
			QList<VColorStop*> colorStops = currentGradient.colorStops();
			if (colorStops.count() == 2)
			{
				int endC = colorStops.count() - 1;
				meshPoint cP;
				cP.resetTo(center);
				cP.transparency = colorStops[0]->opacity;
				cP.shade = 100;
				cP.colorName = colorStops[0]->name;
				cP.color = colorStops[0]->color;
				for (int poi = 0; poi < gpath.size()-3; poi += 4)
				{
					meshGradientPatch patch;
					patch.BL = cP;
					patch.BR = cP;
					if (gpath.isMarker(poi))
						continue;
					meshPoint tL;
					tL.resetTo(gpath.point(poi));
					tL.controlRight = gpath.point(poi + 1);
					tL.transparency = colorStops[endC]->opacity;
					tL.shade = 100;
					tL.colorName = colorStops[endC]->name;
					tL.color = colorStops[endC]->color;
					meshPoint tR;
					tR.resetTo(gpath.point(poi + 2));
					tR.controlLeft = gpath.point(poi + 3);
					tR.transparency = colorStops[endC]->opacity;
					tR.shade = 100;
					tR.colorName = colorStops[endC]->name;
					tR.color = colorStops[endC]->color;
					patch.TL = tL;
					patch.TR = tR;
					ite->meshGradientPatches.append(patch);
				}
			}
			else
			{
				FPointArray gpath2 = gpath.copy();
				QTransform mm;
				mm.translate(center.x(), center.y());
				mm.scale(colorStops[1]->rampPoint, colorStops[1]->rampPoint);
				mm.translate(-center.x(), -center.y());
				gpath2.map(mm);
				meshPoint cP;
				cP.resetTo(center);
				cP.transparency = colorStops[0]->opacity;
				cP.shade = 100;
				cP.colorName = colorStops[0]->name;
				cP.color = colorStops[0]->color;
				for (int poi = 0; poi < gpath2.size()-3; poi += 4)
				{
					meshGradientPatch patch;
					patch.BL = cP;
					patch.BR = cP;
					if (gpath.isMarker(poi))
						continue;
					meshPoint tL;
					tL.resetTo(gpath2.point(poi));
					tL.controlRight = gpath2.point(poi + 1);
					tL.transparency = colorStops[1]->opacity;
					tL.shade = 100;
					tL.colorName = colorStops[1]->name;
					tL.color = colorStops[1]->color;
					meshPoint tR;
					tR.resetTo(gpath2.point(poi + 2));
					tR.controlLeft = gpath2.point(poi + 3);
					tR.transparency = colorStops[1]->opacity;
					tR.shade = 100;
					tR.colorName = colorStops[1]->name;
					tR.color = colorStops[1]->color;
					patch.TL = tL;
					patch.TR = tR;
					ite->meshGradientPatches.append(patch);
				}
				for (int cstp = 2; cstp < colorStops.count(); cstp++)
				{
					FPointArray gpath3 = gpath2.copy();
					gpath2 = gpath.copy();
					QTransform mm;
					mm.translate(center.x(), center.y());
					mm.scale(colorStops[cstp]->rampPoint, colorStops[cstp]->rampPoint);
					mm.translate(-center.x(), -center.y());
					gpath2.map(mm);
					for (int poi = 0; poi < gpath2.size()-3; poi += 4)
					{
						if (gpath.isMarker(poi))
							continue;
						meshGradientPatch patch;
						meshPoint bL;
						bL.resetTo(gpath3.point(poi));
						bL.controlRight = gpath3.point(poi + 1);
						bL.transparency = colorStops[cstp - 1]->opacity;
						bL.shade = 100;
						bL.colorName = colorStops[cstp - 1]->name;
						bL.color = colorStops[cstp - 1]->color;
						patch.BL = bL;
						meshPoint bR;
						bR.resetTo(gpath3.point(poi + 2));
						bR.controlLeft = gpath3.point(poi + 3);
						bR.transparency = colorStops[cstp - 1]->opacity;
						bR.shade = 100;
						bR.colorName = colorStops[cstp - 1]->name;
						bR.color = colorStops[cstp - 1]->color;
						patch.BR = bR;
						meshPoint tL;
						tL.resetTo(gpath2.point(poi));
						tL.controlRight = gpath2.point(poi + 1);
						tL.transparency = colorStops[cstp]->opacity;
						tL.shade = 100;
						tL.colorName = colorStops[cstp]->name;
						tL.color = colorStops[cstp]->color;
						meshPoint tR;
						tR.resetTo(gpath2.point(poi + 2));
						tR.controlLeft = gpath2.point(poi + 3);
						tR.transparency = colorStops[cstp]->opacity;
						tR.shade = 100;
						tR.colorName = colorStops[cstp]->name;
						tR.color = colorStops[cstp]->color;
						patch.TL = tL;
						patch.TR = tR;
						ite->meshGradientPatches.append(patch);
					}
				}
			}
			/*
			meshPoint cP;
			cP.resetTo(center);
			cP.transparency = gradColor2Trans;
			cP.shade = 100;
			cP.colorName = gradColor2Str;
			cP.color = gradColor2;
			for (int poi = 0; poi < ite->PoLine.size()-3; poi += 4)
			{
				meshGradientPatch patch;
				patch.BL = cP;
				patch.BR = cP;
				if (ite->PoLine.isMarker(poi))
					continue;
				meshPoint tL;
				tL.resetTo(ite->PoLine.point(poi));
				tL.controlRight = ite->PoLine.point(poi + 1);
				tL.transparency = gradColor1Trans;
				tL.shade = 100;
				tL.colorName = gradColor1Str;
				tL.color = gradColor1;
				meshPoint tR;
				tR.resetTo(ite->PoLine.point(poi + 2));
				tR.controlLeft = ite->PoLine.point(poi + 3);
				tR.transparency = gradColor1Trans;
				tR.shade = 100;
				tR.colorName = gradColor1Str;
				tR.color = gradColor1;
				patch.TL = tL;
				patch.TR = tR;
				ite->meshGradientPatches.append(patch);
			}
			*/
			ite->GrType = 12;
		}
	}
	if(m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap")
	{
		QByteArray ba(m_style["draw:fill-image"]->getStr().cstr());
		QByteArray imageData = QByteArray::fromBase64(ba);
		QString imgExt = "";
		if (m_style["librevenge:mime-type"]->getStr() == "image/png")
			imgExt = "png";
		else if (m_style["librevenge:mime-type"]->getStr() == "image/jpeg")
			imgExt = "jpg";
		else if (m_style["librevenge:mime-type"]->getStr() == "image/bmp")
			imgExt = "bmp";
		else if (m_style["librevenge:mime-type"]->getStr() == "image/pict")
			imgExt = "pict";
		else if (m_style["librevenge:mime-type"]->getStr() == "image/tiff")
			imgExt = "tif";
		if (!imgExt.isEmpty())
		{
			QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
			tempFile->setAutoRemove(false);
			if (tempFile->open())
			{
				tempFile->write(imageData);
				QString fileName = getLongPathName(tempFile->fileName());
				tempFile->close();
				ScPattern pat = ScPattern();
				pat.setDoc(m_Doc);
				int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
				PageItem* newItem = m_Doc->Items->at(z);
				if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
				{
					int r = qRound(m_style["draw:red"]->getDouble() * 255);
					int g = qRound(m_style["draw:green"]->getDouble() * 255);
					int b = qRound(m_style["draw:blue"]->getDouble() * 255);
					QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
					QString efVal = parseColor(colVal);
					efVal += "\n";
					struct ImageEffect ef;
					efVal += "100";
					ef.effectCode = ScImage::EF_COLORIZE;
					ef.effectParameters = efVal;
					ite->effectsInUse.append(ef);
				}
				m_Doc->loadPict(fileName, newItem);
				m_Doc->Items->takeAt(z);
				newItem->isInlineImage = true;
				newItem->isTempFile = true;
				pat.width = newItem->pixm.qImage().width();
				pat.height = newItem->pixm.qImage().height();
				pat.scaleX = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
				pat.scaleY = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
				pat.pattern = newItem->pixm.qImage().copy();
				newItem->setWidth(pat.pattern.width());
				newItem->setHeight(pat.pattern.height());
				newItem->SetRectFrame();
				newItem->gXpos = 0.0;
				newItem->gYpos = 0.0;
				newItem->gWidth = pat.pattern.width();
				newItem->gHeight = pat.pattern.height();
				pat.items.append(newItem);
				QString patternName = "Pattern_"+ite->itemName();
				patternName = patternName.trimmed().simplified().replace(" ", "_");
				m_Doc->addPattern(patternName, pat);
				importedPatterns->append(patternName);
				ite->setPattern(patternName);
				ite->GrType = 8;
			}
			delete tempFile;
		}
	}
}

#else
RawPainter::RawPainter(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel, QString fTyp): libwpg::WPGPaintInterface()
{
	m_Doc = Doc;
	baseX = x;
	baseY = y;
	docWidth = w;
	docHeight = h;
	importerFlags = iflags;
	Elements = Elem;
	importedColors = iColors;
	importedPatterns = iPatterns;
	tmpSel = tSel;
	CurrColorFill = "Black";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	CurrStrokeTrans = 0.0;
	CurrFillTrans = 0.0;
	Coords.resize(0);
	Coords.svgInit();
	LineW = 1.0;
	lineJoin = Qt::MiterJoin;
	lineEnd = Qt::FlatCap;
	fillrule = true;
	gradientAngle = 0.0;
	isGradient = false;
	lineSpSet = false;
	currentGradient = VGradient(VGradient::linear);
	currentGradient.clearStops();
	currentGradient.setRepeatMethod( VGradient::none );
	dashArray.clear();
	firstPage = true;
	actPage = 0;
	actTextItem = NULL;
	doProcessing = true;
	fileType = fTyp;
}

void RawPainter::startGraphics(const ::WPXPropertyList &propList)
{
	if (propList["svg:width"])
		docWidth = valueAsPoint(propList["svg:width"]);
	if (propList["svg:height"])
		docHeight = valueAsPoint(propList["svg:height"]);
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (!firstPage)
		{
			m_Doc->addPage(actPage);
			m_Doc->setActiveLayer(baseLayer);
		}
		else
			baseLayer = m_Doc->activeLayerName();
		m_Doc->setPageSize("Custom");
		m_Doc->currentPage()->setInitialWidth(docWidth);
		m_Doc->currentPage()->setInitialHeight(docHeight);
		m_Doc->currentPage()->setWidth(docWidth);
		m_Doc->currentPage()->setHeight(docHeight);
		m_Doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
		m_Doc->currentPage()->m_pageSize = "Custom";
		m_Doc->reformPages(true);
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	firstPage = false;
	actPage++;
}

void RawPainter::endGraphics()
{
	if (importerFlags & LoadSavePlugin::lfCreateThumbnail)
		doProcessing = false;
//	qDebug() << "endGraphics";
//  printf("RawPainter::endGraphics\n");
}

void RawPainter::startLayer(const ::WPXPropertyList &propList)
{
	if (!doProcessing)
		return;
	FPointArray clip;
	if (propList["svg:clip-path"])
	{
		QString svgString = QString(propList["svg:clip-path"]->getStr().cstr());
		clip.resize(0);
		clip.svgInit();
		svgString.replace(",", ".");
		clip.parseSVG(svgString);
		QTransform m;
		m.scale(72.0, 72.0);
		clip.map(m);
	}
	QList<PageItem*> gElements;
	groupEntry gr;
	gr.clip = clip.copy();
	gr.Items = gElements;
	groupStack.push(gr);
}

void RawPainter::endLayer()
{
	if (!doProcessing)
		return;
	if (groupStack.count() != 0)
	{
		PageItem *ite;
		groupEntry gr = groupStack.pop();
		QList<PageItem*> gElements = gr.Items;
		tmpSel->clear();
		if (gElements.count() > 0)
		{
			bool groupClip = true;
			for (int dre = 0; dre < gElements.count(); ++dre)
			{
				tmpSel->addItem(gElements.at(dre), true);
				Elements->removeAll(gElements.at(dre));
				if (gElements.at(dre)->hasSoftShadow())
					groupClip = false;
			}
			ite = m_Doc->groupObjectsSelection(tmpSel);
			ite->setGroupClipping(groupClip);
			ite->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
			if (!gr.clip.isEmpty())
			{
				double oldX = ite->xPos();
				double oldY = ite->yPos();
				double oldW = ite->width();
				double oldH = ite->height();
				double oldgW = ite->groupWidth;
				double oldgH = ite->groupHeight;
				ite->PoLine = gr.clip.copy();
				ite->PoLine.translate(baseX, baseY);
				FPoint xy = getMinClipF(&ite->PoLine);
				ite->setXYPos(xy.x(), xy.y(), true);
				ite->PoLine.translate(-xy.x(), -xy.y());
				FPoint wh = getMaxClipF(&ite->PoLine);
				ite->setWidthHeight(wh.x(),wh.y());
				ite->groupWidth = oldgW * (ite->width() / oldW);
				ite->groupHeight = oldgH * (ite->height() / oldH);
				double dx = (ite->xPos() - oldX) / (ite->width() / ite->groupWidth);
				double dy = (ite->yPos() - oldY) / (ite->height() / ite->groupHeight);
				for (int em = 0; em < ite->groupItemList.count(); ++em)
				{
					PageItem* embedded = ite->groupItemList.at(em);
					embedded->moveBy(-dx, -dy, true);
					m_Doc->setRedrawBounding(embedded);
					embedded->OwnPage = m_Doc->OnPage(embedded);
				}
				ite->ClipEdited = true;
				ite->OldB2 = ite->width();
				ite->OldH2 = ite->height();
				ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
				ite->updateGradientVectors();
			}
			Elements->append(ite);
			if (groupStack.count() != 0)
				groupStack.top().Items.append(ite);
		}
		tmpSel->clear();
	}
}

void RawPainter::startEmbeddedGraphics(const ::WPXPropertyList &propList)
{
	if (!doProcessing)
		return;
	qDebug() << "startEmbeddedGraphics";
//  printf("RawPainter::startEmbeddedGraphics (%s)\n", getPropString(propList).cstr());
}

void RawPainter::endEmbeddedGraphics()
{
	if (!doProcessing)
		return;
//	qDebug() << "endEmbeddedGraphics";
//  printf("RawPainter::endEmbeddedGraphics \n");
}

void RawPainter::setStyle(const ::WPXPropertyList &propList, const ::WPXPropertyListVector &gradient)
{
	if (!doProcessing)
		return;
	m_style.clear();
	m_style = propList;
	isGradient = false;
	if(propList["draw:fill"] && propList["draw:fill"]->getStr() == "none")
		CurrColorFill = CommonStrings::None;
	else if(propList["draw:fill"] && propList["draw:fill"]->getStr() == "solid")
	{
		if (propList["draw:fill-color"])
		{
			CurrColorFill = parseColor(QString(propList["draw:fill-color"]->getStr().cstr()));
			if(propList["draw:opacity"])
				CurrFillTrans = 1.0 - qMin(1.0, qMax(fromPercentage(QString(propList["draw:opacity"]->getStr().cstr())), 0.0));
		}
	}
	if(propList["draw:fill"] && propList["draw:fill"]->getStr() == "gradient")
	{
		double angle = 0;
		if (propList["draw:angle"])
			angle = propList["draw:angle"]->getDouble();
		if (gradient.count() > 1)
		{
			double opacity = 1.0;
			currentGradient = VGradient(VGradient::linear);
			currentGradient.clearStops();
			currentGradient.setRepeatMethod( VGradient::none );
			double dr = 1.0 / static_cast<double>(gradient.count());
			for (unsigned c = 0; c < gradient.count(); c++)
			{
				WPXPropertyList grad = gradient[c];
				if (grad["svg:stop-color"])
				{
					QString stopName = parseColor(QString(grad["svg:stop-color"]->getStr().cstr()));
					double rampPoint = dr * c;
					if(grad["svg:offset"])
						rampPoint = fromPercentage(QString(grad["svg:offset"]->getStr().cstr()));
					const ScColor& gradC = m_Doc->PageColors[stopName];
					if(grad["svg:stop-opacity"])
						opacity = qMin(1.0, qMax(fromPercentage(QString(grad["svg:stop-opacity"]->getStr().cstr())), 0.0));
					currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), rampPoint, 0.5, opacity, stopName, 100 );
					if (c == 0)
					{
						gradColor1Str = stopName;
						gradColor1 = ScColorEngine::getRGBColor(gradC, m_Doc);
						gradColor1Trans = opacity;
					}
					else
					{
						gradColor2Str = stopName;
						gradColor2 = ScColorEngine::getRGBColor(gradC, m_Doc);
						gradColor2Trans = opacity;
					}
				}
			}
			if (currentGradient.Stops() > 1)
				isGradient = true;
		}
		else
		{
			gradColor1Str = "Black";
			gradColor2Str = "Black";
			if (propList["draw:start-color"])
				gradColor2Str = parseColor(QString(propList["draw:start-color"]->getStr().cstr()));
			if (propList["draw:end-color"])
				gradColor1Str = parseColor(QString(propList["draw:end-color"]->getStr().cstr()));
			double opacity = 1.0;
			currentGradient = VGradient(VGradient::linear);
			currentGradient.clearStops();
			currentGradient.setRepeatMethod( VGradient::none );

			if (propList["draw:style"])
			{
				if (QString(propList["draw:style"]->getStr().cstr()) == "axial")
				{
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor1Str], m_Doc), 0.0, 0.5, opacity, gradColor1Str, 100 );
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor2Str], m_Doc), 0.5, 0.5, opacity, gradColor2Str, 100 );
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor1Str], m_Doc), 1.0, 0.5, opacity, gradColor1Str, 100 );
				}
				else
				{
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor1Str], m_Doc), 0.0, 0.5, opacity, gradColor1Str, 100 );
					currentGradient.addStop( ScColorEngine::getRGBColor(m_Doc->PageColors[gradColor2Str], m_Doc), 1.0, 0.5, opacity, gradColor2Str, 100 );
				}
				isGradient = true;
			}
		}
	}
	if(propList["svg:fill-rule"])
	{
		if (QString(propList["svg:fill-rule"]->getStr().cstr()) == "nonzero")
			fillrule = false;
		else
			fillrule = true;
	}
	if (propList["svg:stroke-width"])
		LineW = valueAsPoint(propList["svg:stroke-width"]);
	if (propList["draw:stroke"])
	{
		if (propList["draw:stroke"]->getStr() == "none")
			CurrColorStroke = CommonStrings::None;
		else if ((propList["draw:stroke"]->getStr() == "solid") || (propList["draw:stroke"]->getStr() == "dash"))
		{
			if (propList["svg:stroke-color"])
			{
				CurrColorStroke = parseColor(QString(propList["svg:stroke-color"]->getStr().cstr()));
				if(propList["svg:stroke-opacity"])
					CurrStrokeTrans = 1.0 - qMin(1.0, qMax(fromPercentage(QString(propList["svg:stroke-opacity"]->getStr().cstr())), 0.0));
			}
			if (propList["draw:stroke"]->getStr() == "dash")
			{
				dashArray.clear();
				double gap = LineW;
				if (propList["draw:distance"])
					gap = valueAsPoint(propList["draw:distance"]);
				int dots1 = 0;
				if (propList["draw:dots1"])
					dots1 = propList["draw:dots1"]->getInt();
				double dots1len = LineW;
				if (propList["draw:dots1-length"])
					dots1len = valueAsPoint(propList["draw:dots1-length"]);
				int dots2 = 0;
				if (propList["draw:dots2"])
					dots2 = propList["draw:dots2"]->getInt();
				double dots2len = LineW;
				if (propList["draw:dots2-length"])
					dots2len = valueAsPoint(propList["draw:dots2-length"]);
				for (int i = 0; i < dots1; i++)
				{
					dashArray << qMax(dots1len, 0.1) << qMax(gap, 0.1);
				}
				for (int j = 0; j < dots2; j++)
				{
					dashArray << qMax(dots2len, 0.1) << qMax(gap, 0.1);
				}
			}
			else
				dashArray.clear();
		}
	}
	if (propList["svg:stroke-linecap"])
	{
		QString params = QString(propList["svg:stroke-linecap"]->getStr().cstr());
		if( params == "butt" )
			lineEnd = Qt::FlatCap;
		else if( params == "round" )
			lineEnd = Qt::RoundCap;
		else if( params == "square" )
			lineEnd = Qt::SquareCap;
		else
			lineEnd = Qt::FlatCap;
	}
	if (propList["svg:stroke-linejoin"])
	{
		QString params = QString(propList["svg:stroke-linejoin"]->getStr().cstr());
		if( params == "miter" )
			lineJoin = Qt::MiterJoin;
		else if( params == "round" )
			lineJoin = Qt::RoundJoin;
		else if( params == "bevel" )
			lineJoin = Qt::BevelJoin;
		else
			lineJoin = Qt::MiterJoin;
	}
//	qDebug() << "setStyle";
//  printf("RawPainter::setStyle(%s, gradient: (%s))\n", getPropString(propList).cstr(), getPropString(gradient).cstr());
}

void RawPainter::drawRectangle(const ::WPXPropertyList &propList)
{
	if (!doProcessing)
		return;
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x, baseY + y, w, h, LineW, CurrColorFill, CurrColorStroke, true);
		PageItem *ite = m_Doc->Items->at(z);
		finishItem(ite);
		applyFill(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
//	qDebug() << "drawRectangle";
//  printf("RawPainter::drawRectangle (%s)\n", getPropString(propList).cstr());
}

void RawPainter::drawEllipse(const ::WPXPropertyList &propList)
{
	if (!doProcessing)
		return;
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, w, h, LineW, CurrColorFill, CurrColorStroke, true);
		PageItem *ite = m_Doc->Items->at(z);
		finishItem(ite);
		applyFill(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
//	qDebug() << "drawEllipse";
//  printf("RawPainter::drawEllipse (%s)\n", getPropString(propList).cstr());
}

void RawPainter::drawPolyline(const ::WPXPropertyListVector &vertices)
{
	if (!doProcessing)
		return;
	Coords.resize(0);
	Coords.svgInit();
	PageItem *ite;
	Coords.svgMoveTo(valueAsPoint(vertices[0]["svg:x"]), valueAsPoint(vertices[0]["svg:y"]));
	for(unsigned i = 1; i < vertices.count(); i++)
	{
		Coords.svgLineTo(valueAsPoint(vertices[i]["svg:x"]), valueAsPoint(vertices[i]["svg:y"]));
	}
	if (Coords.size() > 0)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		finishItem(ite);
		applyArrows(ite);
	}
}

void RawPainter::drawPolygon(const ::WPXPropertyListVector &vertices)
{
	if (!doProcessing)
		return;
	if(vertices.count() < 2)
		return;
	Coords.resize(0);
	Coords.svgInit();
	Coords.svgMoveTo(valueAsPoint(vertices[0]["svg:x"]), valueAsPoint(vertices[0]["svg:y"]));
	for(unsigned i = 1; i < vertices.count(); i++)
	{
		Coords.svgLineTo(valueAsPoint(vertices[i]["svg:x"]), valueAsPoint(vertices[i]["svg:y"]));
	}
	Coords.svgClosePath();
	if (Coords.size() <= 0)
		return;
	PageItem *ite = NULL;
	int z = 0;
	if(m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap" && m_style["style:repeat"] && m_style["style:repeat"]->getStr() == "stretch")
	{
		if (m_style["draw:fill-image"] && m_style["libwpg:mime-type"])
		{
		  QByteArray ba(m_style["draw:fill-image"]->getStr().cstr());
		  QByteArray imageData = QByteArray::fromBase64(ba);
		  QString imgExt = "";
		  if (m_style["libwpg:mime-type"]->getStr() == "image/png")
			  imgExt = "png";
		  else if (m_style["libwpg:mime-type"]->getStr() == "image/jpeg")
			  imgExt = "jpg";
		  else if (m_style["libwpg:mime-type"]->getStr() == "image/bmp")
			  imgExt = "bmp";
		  else if (m_style["libwpg:mime-type"]->getStr() == "image/pict")
			  imgExt = "pict";
		  else if (m_style["libwpg:mime-type"]->getStr() == "image/tiff")
			  imgExt = "tif";
		  if (!imgExt.isEmpty())
		  {
			  z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
			  ite = m_Doc->Items->at(z);
			  ite->PoLine = Coords.copy();
			  finishItem(ite);
			  insertImage(ite, imgExt, imageData);
		  }
		  else if ((m_style["libwpg:mime-type"]->getStr() == "image/wmf") || (m_style["libwpg:mime-type"]->getStr() == "image/emf"))
		  {
			  if (m_style["libwpg:mime-type"]->getStr() == "image/wmf")
				  imgExt = "wmf";
			  else
				  imgExt = "emf";
			  QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
			  if (tempFile->open())
			  {
				  tempFile->write(imageData);
				  QString fileName = getLongPathName(tempFile->fileName());
				  tempFile->close();
				  FileLoader *fileLoader = new FileLoader(fileName);
				  int testResult = fileLoader->testFile();
				  delete fileLoader;
				  if (testResult != -1)
				  {
					  const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
					  if( fmt )
					  {
						  fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
						  fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
						  if (m_Doc->m_Selection->count() > 0)
						  {
							  ite = m_Doc->groupObjectsSelection();
							  double rot = 0;
							  if (m_style["libwpg:rotate"])
								  rot = m_style["libwpg:rotate"]->getDouble();
							  QPainterPath ba = Coords.toQPainterPath(true);
							  QRectF baR = ba.boundingRect();
							  if (rot != 0)
							  {
								  QTransform mm;
								  mm.translate(baR.x(), baR.y());
								  mm.translate(baR.width() / 2.0, baR.height() / 2.0);
								  mm.rotate(rot);
								  mm.translate(-baR.width() / 2.0, -baR.height() / 2.0);
								  mm.translate(-baR.x(), -baR.y());
								  ba = mm.map(ba);
								  baR = ba.boundingRect();
								  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
								  ite->setWidthHeight(baR.width(), baR.height(), true);
								  Coords.fromQPainterPath(ba, true);
								  FPoint tp2(getMinClipF(&Coords));
								  Coords.translate(-tp2.x(), -tp2.y());
								  ite->PoLine = Coords.copy();
								  int rm = m_Doc->RotMode();
								  m_Doc->RotMode(2);
								  m_Doc->RotateItem(-rot, ite);
								  m_Doc->RotMode(rm);
							  }
							  else
							  {
								  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
								  ite->setWidthHeight(baR.width(), baR.height(), true);
								  FPoint tp2(getMinClipF(&Coords));
								  Coords.translate(-tp2.x(), -tp2.y());
								  ite->PoLine = Coords.copy();
							  }
							  finishItem(ite);
							  if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
							  {
								  int r = qRound(m_style["draw:red"]->getDouble() * 255);
								  int g = qRound(m_style["draw:green"]->getDouble() * 255);
								  int b = qRound(m_style["draw:blue"]->getDouble() * 255);
								  QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
								  QString efVal = parseColor(colVal);
								  recolorItem(ite, efVal);
							  }
						  }
					  }
				  }
			  }
			  delete tempFile;
		  }
	  }
	}
	else
	{
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		finishItem(ite);
		applyFill(ite);
	}
	applyFlip(ite);
	if (CurrColorFill != CommonStrings::None)
		applyShadow(ite);

}

void RawPainter::drawPath(const ::WPXPropertyListVector &path)
{
	if (!doProcessing)
		return;
	bool isClosed = false;
	QString svgString = "";
	for(unsigned i=0; i < path.count(); i++)
	{
		WPXPropertyList propList = path[i];
		if (propList["libwpg:path-action"] && propList["libwpg:path-action"]->getStr() == "M")
			svgString += QString("M %1 %2 ").arg(valueAsPoint(propList["svg:x"])).arg(valueAsPoint(propList["svg:y"]));
		else if (propList["libwpg:path-action"] && propList["libwpg:path-action"]->getStr() == "L")
			svgString += QString("L %1 %2 ").arg(valueAsPoint(propList["svg:x"])).arg(valueAsPoint(propList["svg:y"]));
		else if (propList["libwpg:path-action"] && propList["libwpg:path-action"]->getStr() == "C")
			svgString += QString("C %1 %2 %3 %4 %5 %6 ").arg(valueAsPoint(propList["svg:x1"])).arg(valueAsPoint(propList["svg:y1"])).arg(valueAsPoint(propList["svg:x2"])).arg(valueAsPoint(propList["svg:y2"])).arg(valueAsPoint(propList["svg:x"])).arg(valueAsPoint(propList["svg:y"]));
		else if (propList["libwpg:path-action"] && propList["libwpg:path-action"]->getStr() == "Q")
			svgString += QString("Q %1 %2 %3 %4 ").arg(valueAsPoint(propList["svg:x1"])).arg(valueAsPoint(propList["svg:y1"])).arg(valueAsPoint(propList["svg:x"])).arg(valueAsPoint(propList["svg:y"]));
		else if (propList["libwpg:path-action"] && propList["libwpg:path-action"]->getStr() == "A")
			svgString += QString("A %1 %2 %3 %4 %5 %6 %7") .arg(valueAsPoint(propList["svg:rx"])) .arg(valueAsPoint(propList["svg:ry"])).arg(propList["libwpg:rotate"] ? propList["libwpg:rotate"]->getDouble() : 0).arg(propList["libwpg:large-arc"] ? propList["libwpg:large-arc"]->getInt() : 1).arg(propList["libwpg:sweep"] ? propList["libwpg:sweep"]->getInt() : 1).arg(valueAsPoint(propList["svg:x"])).arg(valueAsPoint(propList["svg:y"]));
		else if ((i >= path.count()-1 && i > 2) && propList["libwpg:path-action"] && propList["libwpg:path-action"]->getStr() == "Z" )
		{
			isClosed = true;
			svgString += "Z";
		}
	}
	Coords.resize(0);
	Coords.svgInit();
	Coords.parseSVG(svgString);
	PageItem *ite;
	int z;
	if (isClosed)
	{
		if(m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap" && m_style["style:repeat"] && m_style["style:repeat"]->getStr() == "stretch")
		{
		  if (m_style["draw:fill-image"] && m_style["libwpg:mime-type"])
		  {
			  QByteArray ba(m_style["draw:fill-image"]->getStr().cstr());
			  QByteArray imageData = QByteArray::fromBase64(ba);
			  QString imgExt = "";
			  if (m_style["libwpg:mime-type"]->getStr() == "image/png")
				  imgExt = "png";
			  else if (m_style["libwpg:mime-type"]->getStr() == "image/jpeg")
				  imgExt = "jpg";
			  else if (m_style["libwpg:mime-type"]->getStr() == "image/bmp")
				  imgExt = "bmp";
			  else if (m_style["libwpg:mime-type"]->getStr() == "image/pict")
				  imgExt = "pict";
			  else if (m_style["libwpg:mime-type"]->getStr() == "image/tiff")
				  imgExt = "tif";
			  if (!imgExt.isEmpty())
			  {
				  z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
				  ite = m_Doc->Items->at(z);
				  ite->PoLine = Coords.copy();
				  finishItem(ite);
				  insertImage(ite, imgExt, imageData);
			  }
			  else if ((m_style["libwpg:mime-type"]->getStr() == "image/wmf") || (m_style["libwpg:mime-type"]->getStr() == "image/emf"))
			  {
				  if (m_style["libwpg:mime-type"]->getStr() == "image/wmf")
					  imgExt = "wmf";
				  else
					  imgExt = "emf";
				  QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
				  if (tempFile->open())
				  {
					  tempFile->write(imageData);
					  QString fileName = getLongPathName(tempFile->fileName());
					  tempFile->close();
					  FileLoader *fileLoader = new FileLoader(fileName);
					  int testResult = fileLoader->testFile();
					  delete fileLoader;
					  if (testResult != -1)
					  {
						  const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
						  if( fmt )
						  {
							  fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
							  fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
							  if (m_Doc->m_Selection->count() > 0)
							  {
								  ite = m_Doc->groupObjectsSelection();
								  double rot = 0;
								  if (m_style["libwpg:rotate"])
									  rot = m_style["libwpg:rotate"]->getDouble();
								  QPainterPath ba = Coords.toQPainterPath(true);
								  QRectF baR = ba.boundingRect();
								  if (rot != 0)
								  {
									  QTransform mm;
									  mm.translate(baR.x(), baR.y());
									  mm.translate(baR.width() / 2.0, baR.height() / 2.0);
									  mm.rotate(rot);
									  mm.translate(-baR.width() / 2.0, -baR.height() / 2.0);
									  mm.translate(-baR.x(), -baR.y());
									  ba = mm.map(ba);
									  baR = ba.boundingRect();
									  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									  ite->setWidthHeight(baR.width(), baR.height(), true);
									  Coords.fromQPainterPath(ba, true);
									  FPoint tp2(getMinClipF(&Coords));
									  Coords.translate(-tp2.x(), -tp2.y());
									  ite->PoLine = Coords.copy();
									  int rm = m_Doc->RotMode();
									  m_Doc->RotMode(2);
									  m_Doc->RotateItem(-rot, ite);
									  m_Doc->RotMode(rm);
								  }
								  else
								  {
									  ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									  ite->setWidthHeight(baR.width(), baR.height(), true);
									  FPoint tp2(getMinClipF(&Coords));
									  Coords.translate(-tp2.x(), -tp2.y());
									  ite->PoLine = Coords.copy();
								  }
								  finishItem(ite);
								  if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
								  {
									  int r = qRound(m_style["draw:red"]->getDouble() * 255);
									  int g = qRound(m_style["draw:green"]->getDouble() * 255);
									  int b = qRound(m_style["draw:blue"]->getDouble() * 255);
									  QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
									  QString efVal = parseColor(colVal);
									  recolorItem(ite, efVal);
								  }
							  }
						  }
					  }
				  }
				  delete tempFile;
			  }
		  }
		}
		else
		{
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
			ite = m_Doc->Items->at(z);
			ite->PoLine = Coords.copy();
			finishItem(ite);
			applyFill(ite);
		}
		applyFlip(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
	else
	{
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		finishItem(ite);
		applyArrows(ite);
	}
}

void RawPainter::drawGraphicObject(const ::WPXPropertyList &propList, const ::WPXBinaryData &binaryData)
{
	if (!doProcessing)
		return;
	if (!propList["libwpg:mime-type"] || propList["libwpg:mime-type"]->getStr().len() <= 0)
		return;
	WPXString base64 = binaryData.getBase64Data();
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		PageItem *ite;
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		QByteArray ba(base64.cstr());
		QByteArray imageData = QByteArray::fromBase64(ba);
		QString imgExt = "";
		if (propList["libwpg:mime-type"]->getStr() == "image/png")
			imgExt = "png";
		else if (propList["libwpg:mime-type"]->getStr() == "image/jpeg")
			imgExt = "jpg";
		else if (propList["libwpg:mime-type"]->getStr() == "image/bmp")
			imgExt = "bmp";
		else if (propList["libwpg:mime-type"]->getStr() == "image/pict")
			imgExt = "pict";
		else if (propList["libwpg:mime-type"]->getStr() == "image/tiff")
			imgExt = "tif";
		if (!imgExt.isEmpty())
		{
			int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, baseX + x, baseY + y, w, h, 0, CurrColorFill, CurrColorStroke, true);
			ite = m_Doc->Items->at(z);
			finishItem(ite);
			insertImage(ite, imgExt, imageData);
		}
		else
		{
			if ((m_style["libwpg:mime-type"]->getStr() == "image/wmf") || (m_style["libwpg:mime-type"]->getStr() == "image/emf"))
			{
				if (m_style["libwpg:mime-type"]->getStr() == "image/wmf")
					imgExt = "wmf";
				else
					imgExt = "emf";
				QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
				if (tempFile->open())
				{
					tempFile->write(imageData);
					QString fileName = getLongPathName(tempFile->fileName());
					tempFile->close();
					FileLoader *fileLoader = new FileLoader(fileName);
					int testResult = fileLoader->testFile();
					delete fileLoader;
					if (testResult != -1)
					{
						const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
						if( fmt )
						{
							fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
							fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
							if (m_Doc->m_Selection->count() > 0)
							{
								ite = m_Doc->groupObjectsSelection();
								double rot = 0;
								if (m_style["libwpg:rotate"])
									rot = m_style["libwpg:rotate"]->getDouble();
								QPainterPath ba = Coords.toQPainterPath(true);
								QRectF baR = ba.boundingRect();
								if (rot != 0)
								{
									QTransform mm;
									mm.translate(baR.x(), baR.y());
									mm.translate(baR.width() / 2.0, baR.height() / 2.0);
									mm.rotate(rot);
									mm.translate(-baR.width() / 2.0, -baR.height() / 2.0);
									mm.translate(-baR.x(), -baR.y());
									ba = mm.map(ba);
									baR = ba.boundingRect();
									ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									ite->setWidthHeight(baR.width(), baR.height(), true);
									Coords.fromQPainterPath(ba, true);
									FPoint tp2(getMinClipF(&Coords));
									Coords.translate(-tp2.x(), -tp2.y());
									ite->PoLine = Coords.copy();
									int rm = m_Doc->RotMode();
									m_Doc->RotMode(2);
									m_Doc->RotateItem(-rot, ite);
									m_Doc->RotMode(rm);
								}
								else
								{
									ite->setXYPos(baseX + baR.x(), baseY + baR.y(), true);
									ite->setWidthHeight(baR.width(), baR.height(), true);
									FPoint tp2(getMinClipF(&Coords));
									Coords.translate(-tp2.x(), -tp2.y());
									ite->PoLine = Coords.copy();
								}
								finishItem(ite);
								if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
								{
									int r = qRound(m_style["draw:red"]->getDouble() * 255);
									int g = qRound(m_style["draw:green"]->getDouble() * 255);
									int b = qRound(m_style["draw:blue"]->getDouble() * 255);
									QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
									QString efVal = parseColor(colVal);
									recolorItem(ite, efVal);
								}
							}
						}
					}
				}
				delete tempFile;
			}
		}
		applyFlip(ite);
		if (CurrColorFill != CommonStrings::None)
			applyShadow(ite);
	}
//	qDebug() << "drawGraphicObject";
//  printf("RawPainter::drawGraphicObject (%s)\n", getPropString(propList).cstr());
}

void RawPainter::startTextObject(const ::WPXPropertyList &propList, const ::WPXPropertyListVector &path)
{
	if (!doProcessing)
		return;
	actTextItem = NULL;
	lineSpSet = false;
	lineSpIsPT = false;
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		double x = valueAsPoint(propList["svg:x"]);
		double y = valueAsPoint(propList["svg:y"]);
		double w = valueAsPoint(propList["svg:width"]);
		double h = valueAsPoint(propList["svg:height"]);
		double rot = 0;
		if (propList["libwpg:rotate"])
			rot = propList["libwpg:rotate"]->getDouble();
		int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, baseX + x, baseY + y, w, h, 0, CurrColorFill, CurrColorStroke, true);
		PageItem *ite = m_Doc->Items->at(z);
		finishItem(ite);
		applyShadow(ite);
		if (rot != 0)
		{
			int rm = m_Doc->RotMode();
			m_Doc->RotMode(2);
			m_Doc->RotateItem(rot, ite);
			m_Doc->RotMode(rm);
		}
		if (propList["draw-mirror-horizontal"])
			ite->flipImageH();
		if (propList["draw-mirror-vertical"])
			ite->flipImageV();
		if (propList["fo:padding-left"])
			ite->setTextToFrameDistLeft(valueAsPoint(propList["fo:padding-left"]));
		if (propList["fo:padding-right"])
			ite->setTextToFrameDistRight(valueAsPoint(propList["fo:padding-right"]));
		if (propList["fo:padding-top"])
			ite->setTextToFrameDistTop(valueAsPoint(propList["fo:padding-top"]));
		if (propList["fo:padding-bottom"])
			ite->setTextToFrameDistBottom(valueAsPoint(propList["fo:padding-bottom"]));
		if (propList["fo:column-count"])
			ite->setColumns(propList["fo:column-count"]->getInt());
		if (propList["fo:column-gap"])
			ite->setColumnGap(valueAsPoint(propList["fo:column-gap"]));
		ite->setFirstLineOffset(FLOPFontAscent);
		actTextItem = ite;
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		textStyle = newStyle;
	}
}

void RawPainter::endTextObject()
{
	if (!doProcessing)
		return;
	if (actTextItem)
		actTextItem->itemText.trim();
	actTextItem = NULL;
	lineSpSet = false;
	lineSpIsPT = false;
}

void RawPainter::startTextLine(const ::WPXPropertyList &propList)
{
	if (!doProcessing)
		return;
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	textStyle = newStyle;
	if (propList["fo:text-align"])
	{
		QString align = QString(propList["fo:text-align"]->getStr().cstr());
		if (align == "left")
			textStyle.setAlignment(ParagraphStyle::Leftaligned);
		else if (align == "center")
			textStyle.setAlignment(ParagraphStyle::Centered);
		else if (align == "right")
			textStyle.setAlignment(ParagraphStyle::Rightaligned);
		else if (align == "justify")
			textStyle.setAlignment(ParagraphStyle::Justified);
	}
	if (propList["fo:margin-left"])
		textStyle.setLeftMargin(valueAsPoint(propList["fo:margin-left"]));
	if (propList["fo:margin-right"])
		textStyle.setRightMargin(valueAsPoint(propList["fo:margin-right"]));
	if (propList["fo:text-indent"])
		textStyle.setFirstIndent(valueAsPoint(propList["fo:text-indent"]));
	if (propList["style:drop-cap"])
	{
		textStyle.setDropCapLines(propList["style:drop-cap"]->getInt());
		textStyle.setHasDropCap(true);
	}
	if (propList["fo:margin-bottom"])
		textStyle.setGapAfter(valueAsPoint(propList["fo:margin-bottom"]));
	if (propList["fo:margin-top"])
		textStyle.setGapBefore(valueAsPoint(propList["fo:margin-top"]));
//	m_maxFontSize = textStyle.charStyle().fontSize() / 10.0;
	m_maxFontSize = 1.0;
	if (propList["fo:line-height"])
	{
		m_linespace = propList["fo:line-height"]->getDouble();
		QString lsp = QString(propList["fo:line-height"]->getStr().cstr());
		lineSpIsPT = lsp.endsWith("pt");
		lineSpSet = true;
	}
}

void RawPainter::endTextLine()
{
	if (!doProcessing)
		return;
	if (actTextItem == NULL)
		return;
	int posT = actTextItem->itemText.length();
	if (posT > 0)
	{
		if ((actTextItem->itemText.text(posT - 1) != SpecialChars::PARSEP))
		{
			actTextItem->itemText.insertChars(posT, SpecialChars::PARSEP);
			actTextItem->itemText.applyStyle(posT, textStyle);
		}
	}
}

void RawPainter::startTextSpan(const ::WPXPropertyList &propList)
{
	if (!doProcessing)
		return;
	textCharStyle = textStyle.charStyle();
	if (propList["fo:font-size"])
	{
		textCharStyle.setFontSize(valueAsPoint(propList["fo:font-size"]) * 10.0);
		m_maxFontSize = qMax(m_maxFontSize, valueAsPoint(propList["fo:font-size"]));
	}
	if (propList["fo:color"])
		textCharStyle.setFillColor(parseColor(QString(propList["fo:color"]->getStr().cstr())));
	if (propList["style:font-name"])
	{
		QString fontVari = "";
		if (propList["fo:font-weight"])
			fontVari = QString(propList["fo:font-weight"]->getStr().cstr());
		QString fontName = QString(propList["style:font-name"]->getStr().cstr());
		QString realFontName = constructFontName(fontName, fontVari);
		textCharStyle.setFont((*m_Doc->AllFonts)[realFontName]);
	}
	StyleFlag styleEffects = textCharStyle.effects();
	if (propList["style:text-underline-type"])
		styleEffects |= ScStyle_Underline;
	if (propList["style:text-position"])
	{
		if (propList["style:text-position"]->getStr() == "50% 67%")
			styleEffects |= ScStyle_Superscript;
		else
			styleEffects |= ScStyle_Subscript;
	}
	textCharStyle.setFeatures(styleEffects.featureList());
}

void RawPainter::endTextSpan()
{
}

void RawPainter::insertText(const ::WPXString &str)
{
	if (!doProcessing)
		return;
	if (actTextItem == NULL)
		return;
	if (lineSpSet)
	{
		textStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
		if (lineSpIsPT)
			textStyle.setLineSpacing(m_linespace);
		else
			textStyle.setLineSpacing(m_maxFontSize * m_linespace);
	}
	else
		textStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	WPXString tempUTF8(str, true);
	QString actText = QString(tempUTF8.cstr());
	if (actTextItem)
	{
		int posC = actTextItem->itemText.length();
		if (actText.count() > 0)
		{
			actText.replace(QChar(10), SpecialChars::LINEBREAK);
			actText.replace(QChar(12), SpecialChars::FRAMEBREAK);
			actText.replace(QChar(30), SpecialChars::NBHYPHEN);
			actText.replace(QChar(160), SpecialChars::NBSPACE);
			QTextDocument texDoc;
			texDoc.setHtml(actText);
			actText = texDoc.toPlainText();
			actText = actText.trimmed();
			actTextItem->itemText.insertChars(posC, actText);
			actTextItem->itemText.applyStyle(posC, textStyle);
			actTextItem->itemText.applyCharStyle(posC, actText.length(), textCharStyle);
		}
	}
}

double RawPainter::valueAsPoint(const WPXProperty *prop)
{
	double value = 0.0;
	QString str = QString(prop->getStr().cstr()).toLower();
	if (str.endsWith("in"))
		value = prop->getDouble() * 72.0;
	else
		value = prop->getDouble();
	return value;
}

void RawPainter::applyFill(PageItem* ite)
{
	if(isGradient)
	{
		QString gradMode = "normal";
		if (m_style["libmspub:shade"])
			gradMode = QString(m_style["libmspub:shade"]->getStr().cstr());
		else if (m_style["draw:style"])
			gradMode = QString(m_style["draw:style"]->getStr().cstr());
		if ((gradMode == "normal") || (gradMode == "linear"))
		{
			int angle = 0;
			if (m_style["draw:angle"])
				angle = qRound(m_style["draw:angle"]->getDouble());
			double h = ite->height();
			double w = ite->width();
			if (angle == 0)
				ite->setGradientVector(w / 2.0, h, w / 2.0, 0, 0, 0, 1, 0);
			else if (angle == -225)
				ite->setGradientVector(w, 0, 0, h, 0, 0, 1, 0);
			else if (angle == 45)
				ite->setGradientVector(w, h, 0, 0, 0, 0, 1, 0);
			else if (angle == 90)
				ite->setGradientVector(w, h / 2.0, 0, h / 2.0, 0, 0, 1, 0);
			else if (angle == 180)
				ite->setGradientVector(w / 2.0, 0, w / 2.0, h, 0, 0, 1, 0);
			else if (angle == 270)
				ite->setGradientVector(0, h / 2.0, w, h / 2.0, 0, 0, 1, 0);
			ite->fill_gradient = currentGradient;
			ite->GrType = 6;
		}
		else if (gradMode == "radial")
		{
			double h = ite->height();
			double w = ite->width();
			double cx = 0.0;
			double cy = 0.0;
			if (m_style["svg:cx"])
				cx = m_style["svg:cx"]->getDouble();
			if (m_style["svg:cy"])
				cy = m_style["svg:cy"]->getDouble();
			ite->setGradientVector(cx, cy, w, h / 2.0, cx, cy, 1, 0);
			ite->fill_gradient = currentGradient;
			ite->GrType = 7;
		}
		else if (gradMode == "square")
		{
			double cx = 0.0;
			double cy = 0.0;
			if (m_style["svg:cx"])
				cx = m_style["svg:cx"]->getDouble();
			if (m_style["svg:cy"])
				cy = m_style["svg:cy"]->getDouble();
			FPoint cp = FPoint(cx, cy);
			ite->setDiamondGeometry(FPoint(0, 0), FPoint(ite->width(), 0), FPoint(ite->width(), ite->height()), FPoint(0, ite->height()), cp);
			ite->fill_gradient.clearStops();
			QList<VColorStop*> colorStops = currentGradient.colorStops();
			for( int a = 0; a < colorStops.count() ; a++ )
			{
				ite->fill_gradient.addStop(colorStops[a]->color, 1.0 - colorStops[a]->rampPoint, colorStops[a]->midPoint, colorStops[a]->opacity, colorStops[a]->name, colorStops[a]->shade);
			}
			ite->GrType = 10;
		}
		else if (gradMode == "center")
		{
			QString center = "top-left";
			FPoint cp = FPoint(0, 0);
			if (m_style["libmspub:shade-ref-point"])
				center = QString(m_style["libmspub:shade-ref-point"]->getStr().cstr());
			if (center == "top-left")
				cp = FPoint(0, 0);
			else if (center == "top-right")
				cp = FPoint(ite->width(), 0);
			else if (center == "bottom-left")
				cp = FPoint(0, ite->height());
			else if (center == "bottom-right")
				cp = FPoint(ite->width(), ite->height());
			ite->setDiamondGeometry(FPoint(0, 0), FPoint(ite->width(), 0), FPoint(ite->width(), ite->height()), FPoint(0, ite->height()), cp);
			ite->fill_gradient.clearStops();
			QList<VColorStop*> colorStops = currentGradient.colorStops();
			for( int a = 0; a < colorStops.count() ; a++ )
			{
				ite->fill_gradient.addStop(colorStops[a]->color, 1.0 - colorStops[a]->rampPoint, colorStops[a]->midPoint, colorStops[a]->opacity, colorStops[a]->name, colorStops[a]->shade);
			}
			ite->GrType = 10;
		}
		else if (gradMode == "shape")
		{
			ite->meshGradientPatches.clear();
			FPoint center = FPoint(ite->width() / 2.0, ite->height() / 2.0);
			meshPoint cP;
			cP.resetTo(center);
			cP.transparency = gradColor2Trans;
			cP.shade = 100;
			cP.colorName = gradColor2Str;
			cP.color = gradColor2;
			for (int poi = 0; poi < ite->PoLine.size()-3; poi += 4)
			{
				meshGradientPatch patch;
				patch.BL = cP;
				patch.BR = cP;
				if (ite->PoLine.isMarker(poi))
					continue;
				meshPoint tL;
				tL.resetTo(ite->PoLine.point(poi));
				tL.controlRight = ite->PoLine.point(poi + 1);
				tL.transparency = gradColor1Trans;
				tL.shade = 100;
				tL.colorName = gradColor1Str;
				tL.color = gradColor1;
				meshPoint tR;
				tR.resetTo(ite->PoLine.point(poi + 2));
				tR.controlLeft = ite->PoLine.point(poi + 3);
				tR.transparency = gradColor1Trans;
				tR.shade = 100;
				tR.colorName = gradColor1Str;
				tR.color = gradColor1;
				patch.TL = tL;
				patch.TR = tR;
				ite->meshGradientPatches.append(patch);
			}
			ite->GrType = 12;
		}
	}
	if(m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap")
	{
		QByteArray ba(m_style["draw:fill-image"]->getStr().cstr());
		QByteArray imageData = QByteArray::fromBase64(ba);
		QString imgExt = "";
		if (m_style["libwpg:mime-type"]->getStr() == "image/png")
			imgExt = "png";
		else if (m_style["libwpg:mime-type"]->getStr() == "image/jpeg")
			imgExt = "jpg";
		else if (m_style["libwpg:mime-type"]->getStr() == "image/bmp")
			imgExt = "bmp";
		else if (m_style["libwpg:mime-type"]->getStr() == "image/pict")
			imgExt = "pict";
		else if (m_style["libwpg:mime-type"]->getStr() == "image/tiff")
			imgExt = "tif";
		if (!imgExt.isEmpty())
		{
			QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
			tempFile->setAutoRemove(false);
			if (tempFile->open())
			{
				tempFile->write(imageData);
				QString fileName = getLongPathName(tempFile->fileName());
				tempFile->close();
				ScPattern pat = ScPattern();
				pat.setDoc(m_Doc);
				int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
				PageItem* newItem = m_Doc->Items->at(z);
				if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
				{
					int r = qRound(m_style["draw:red"]->getDouble() * 255);
					int g = qRound(m_style["draw:green"]->getDouble() * 255);
					int b = qRound(m_style["draw:blue"]->getDouble() * 255);
					QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
					QString efVal = parseColor(colVal);
					efVal += "\n";
					struct ImageEffect ef;
					efVal += "100";
					ef.effectCode = ScImage::EF_COLORIZE;
					ef.effectParameters = efVal;
					ite->effectsInUse.append(ef);
				}
				m_Doc->loadPict(fileName, newItem);
				m_Doc->Items->takeAt(z);
				newItem->isInlineImage = true;
				newItem->isTempFile = true;
				pat.width = newItem->pixm.qImage().width();
				pat.height = newItem->pixm.qImage().height();
				pat.scaleX = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
				pat.scaleY = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
				pat.pattern = newItem->pixm.qImage().copy();
				newItem->setWidth(pat.pattern.width());
				newItem->setHeight(pat.pattern.height());
				newItem->SetRectFrame();
				newItem->gXpos = 0.0;
				newItem->gYpos = 0.0;
				newItem->gWidth = pat.pattern.width();
				newItem->gHeight = pat.pattern.height();
				pat.items.append(newItem);
				QString patternName = "Pattern_"+ite->itemName();
				patternName = patternName.trimmed().simplified().replace(" ", "_");
				m_Doc->addPattern(patternName, pat);
				importedPatterns->append(patternName);
				ite->setPattern(patternName);
				ite->GrType = 8;
			}
			delete tempFile;
		}
	}
}
#endif

QString RawPainter::constructFontName(QString fontBaseName, QString fontStyle)
{
	QString fontName = "";
	bool found = false;
	SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (fontBaseName.toLower() == it.current().family().toLower())
		{
			// found the font family, now go for the style
			QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[it.current().family()];
			slist.sort();
			if (slist.count() > 0)
			{
				for (int a = 0; a < slist.count(); a++)
				{
					if (fontStyle.toLower() == slist[a].toLower())
					{
						found = true;
						fontName = it.current().family() + " " + slist[a];
						break;
					}
				}
				if (!found)
				{
					int reInd = slist.indexOf("Regular");
					if (reInd < 0)
						fontName = it.current().family() + " " + slist[0];
					else
						fontName = it.current().family() + " " + slist[reInd];
					found = true;
				}
			}
			else
			{
				fontName = it.current().family();
				found = true;
			}
			break;
		}
	}
	if (!found)
	{
		if (importerFlags & LoadSavePlugin::lfCreateThumbnail)
			fontName = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
		else
		{
			QString family = fontBaseName;
			if (!fontStyle.isEmpty())
				family += " " + fontStyle;
			if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(family))
			{
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				MissingFont *dia = new MissingFont(0, family, m_Doc);
				dia->exec();
				fontName = dia->getReplacementFont();
				delete dia;
				qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
				PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family] = fontName;
			}
			else
				fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family];
		}
	}
	return fontName;
}

double RawPainter::fromPercentage( const QString &s )
{
	QString s1 = s;
	if (s1.endsWith( ";" ))
		s1.chop(1);
	if (s1.endsWith( "%" ))
	{
		s1.chop(1);
		return ScCLocale::toDoubleC(s1) / 100.0;
	}
	else
		return ScCLocale::toDoubleC(s1) / 100.0;
}

QColor RawPainter::parseColorN( const QString &rgbColor )
{
	int r, g, b;
	keywordToRGB( rgbColor.toLower(), r, g, b );
	return QColor( r, g, b );
}

QString RawPainter::parseColor( const QString &s )
{
	QColor c;
	QString ret = CommonStrings::None;
	if (s.startsWith( "rgb(" ) )
	{
		QString parse = s.trimmed();
		QStringList colors = parse.split(',', QString::SkipEmptyParts);
		QString r = colors[0].right( ( colors[0].length() - 4 ) );
		QString g = colors[1];
		QString b = colors[2].left( ( colors[2].length() - 1 ) );
		if (r.contains( "%" ))
		{
			r.chop(1);
			r = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(r) ) / 100.0 ) ) );
		}
		if (g.contains( "%" ))
		{
			g.chop(1);
			g = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(g) ) / 100.0 ) ) );
		}
		if (b.contains( "%" ))
		{
			b.chop(1);
			b = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(b) ) / 100.0 ) ) );
		}
		c = QColor(r.toInt(), g.toInt(), b.toInt());
	}
	else
	{
		QString rgbColor = s.trimmed();
		if (rgbColor.startsWith( "#" ))
		{
			rgbColor = rgbColor.left(7);
			c.setNamedColor( rgbColor );
		}
		else
			c = parseColorN( rgbColor );
	}
	ScColor tmp;
	tmp.fromQColor(c);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString newColorName = QString("From%1").arg(fileType.toUpper())+c.name();
	QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
	if (fNam == newColorName)
		importedColors->append(newColorName);
	ret = fNam;
	return ret;
}

void RawPainter::insertImage(PageItem* ite, QString imgExt, QByteArray &imageData)
{
	QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_%1_XXXXXX.").arg(fileType) + imgExt);
	tempFile->setAutoRemove(false);
	if (tempFile->open())
	{
		tempFile->write(imageData);
		QString fileName = getLongPathName(tempFile->fileName());
		tempFile->close();
		ite->isTempFile = true;
		ite->isInlineImage = true;
		if (m_style["draw:red"] && m_style["draw:green"] && m_style["draw:blue"])
		{
			int r = qRound(m_style["draw:red"]->getDouble() * 255);
			int g = qRound(m_style["draw:green"]->getDouble() * 255);
			int b = qRound(m_style["draw:blue"]->getDouble() * 255);
			QString colVal = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
			QString efVal = parseColor(colVal);
			efVal += "\n";
			struct ImageEffect ef;
			efVal += "100";
			ef.effectCode = ScImage::EF_COLORIZE;
			ef.effectParameters = efVal;
			ite->effectsInUse.append(ef);
		}
		if (m_style["draw:luminance"])
		{
			double per = m_style["draw:luminance"]->getDouble();
			struct ImageEffect ef;
			ef.effectCode = ScImage::EF_BRIGHTNESS;
			ef.effectParameters = QString("%1").arg(qRound((per - 0.5) * 255));
			ite->effectsInUse.append(ef);
		}
		m_Doc->loadPict(fileName, ite);
#if HAVE_REVENGE
		if (m_style["librevenge:rotate"])
		{
			int rot = QString(m_style["librevenge:rotate"]->getStr().cstr()).toInt();
			ite->setImageRotation(rot);
			ite->AdjustPictScale();
		}
#else
		if (m_style["libwpg:rotate"])
		{
			int rot = QString(m_style["libwpg:rotate"]->getStr().cstr()).toInt();
			ite->setImageRotation(rot);
			ite->AdjustPictScale();
		}
#endif
	}
	delete tempFile;
}

void RawPainter::applyShadow(PageItem* ite)
{
	if (ite == NULL)
		return;
	if(m_style["draw:shadow"] && m_style["draw:shadow"]->getStr() == "visible")
	{
		double xof = 0.0;
		double yof = 0.0;
		if (m_style["draw:shadow-offset-x"])
			xof = valueAsPoint(m_style["draw:shadow-offset-x"]);
		if (m_style["draw:shadow-offset-y"])
			yof = valueAsPoint(m_style["draw:shadow-offset-y"]);
		QString shadowColor = CurrColorFill;
		double shadowTrans = 1.0;
		if (m_style["draw:shadow-color"])
		{
			shadowColor = parseColor(QString(m_style["draw:shadow-color"]->getStr().cstr()));
			if(m_style["draw:shadow-opacity"])
				shadowTrans = 1.0 - qMin(1.0, qMax(fromPercentage(QString(m_style["draw:shadow-opacity"]->getStr().cstr())), 0.0));
		}
		ite->setHasSoftShadow(true);
		ite->setSoftShadowColor(shadowColor);
		ite->setSoftShadowXOffset(xof);
		ite->setSoftShadowYOffset(yof);
		ite->setSoftShadowBlurRadius(5);
		ite->setSoftShadowShade(100);
		ite->setSoftShadowOpacity(shadowTrans);
		ite->setSoftShadowBlendMode(0);
		ite->setSoftShadowErasedByObject(false);
		ite->setSoftShadowHasObjectTransparency(false);

	}
}

void RawPainter::applyFlip(PageItem* ite)
{
	if (m_style["draw:mirror-horizontal"])
	{
		ite->setImageFlippedH(true);
	}
	if (m_style["draw:mirror-vertical"])
	{
		ite->setImageFlippedV(true);
	}
}

void RawPainter::recolorItem(PageItem* ite, QString efVal)
{
	if (ite->itemType() != PageItem::Group)
	{
		if (ite->fillColor() != CommonStrings::None)
		{
			QColor fill = ScColorEngine::getShadeColorProof(m_Doc->PageColors[ite->fillColor()], m_Doc, ite->fillShade());
			double k = 100.0 - qMin((0.3 * fill.redF() + 0.59 * fill.greenF() + 0.11 * fill.blueF()) * 100.0, 100.0);
			ite->setFillColor(efVal);
			ite->setFillShade(k);
		}
		if (ite->lineColor() != CommonStrings::None)
		{
			QColor line = ScColorEngine::getShadeColorProof(m_Doc->PageColors[ite->lineColor()], m_Doc, ite->lineShade());
			double k2 = 100.0 - qMin((0.3 * line.redF() + 0.59 * line.greenF() + 0.11 * line.blueF()) * 100.0, 100.0);
			ite->setLineColor(efVal);
			ite->setLineShade(k2);
		}
	}
	else
	{
		PageItem* grItem = ite->asGroupFrame();
		for (int a = 0; a < grItem->groupItemList.count(); a++)
		{
			ite = grItem->groupItemList[a];
			recolorItem(ite, efVal);
		}
	}
}

void RawPainter::applyArrows(PageItem* ite)
{
	if (m_style["draw:marker-end-path"])
	{
		FPointArray EndArrow;
		double EndArrowWidth;
		QString params = QString(m_style["draw:marker-end-path"]->getStr().cstr());
		EndArrowWidth = LineW;
		EndArrow.resize(0);
		EndArrow.svgInit();
		EndArrow.parseSVG(params);
		QPainterPath pa = EndArrow.toQPainterPath(true);
		QRectF br = pa.boundingRect();
		if (m_style["draw:marker-end-width"])
			EndArrowWidth = valueAsPoint(m_style["draw:marker-end-width"]);
		if (EndArrowWidth > 0)
		{
			FPoint End = ite->PoLine.point(ite->PoLine.size()-2);
			for (uint xx = ite->PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = ite->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					QPointF refP = QPointF(br.width() / 2.0, 0);
					QTransform m;
					m.translate(br.width() / 2.0, br.height() / 2.0);
					m.rotate(r + 90);
					m.translate(-br.width() / 2.0, -br.height() / 2.0);
					m.scale(EndArrowWidth / br.width(), EndArrowWidth / br.width());
					EndArrow.map(m);
					refP = m.map(refP);
					QPainterPath pa2 = EndArrow.toQPainterPath(true);
					//QRectF br2 = pa2.boundingRect();
					QTransform m2;
					FPoint grOffset2(getMinClipF(&EndArrow));
					m2.translate(-grOffset2.x(), -grOffset2.y());
					EndArrow.map(m2);
					refP = m2.map(refP);
					EndArrow.translate(-refP.x(), -refP.y());
					QTransform arrowTrans;
					arrowTrans.translate(-m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
					arrowTrans.translate(End.x() + ite->xPos(), End.y() + ite->yPos());
					EndArrow.map(arrowTrans);
					int zE = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, CurrColorStroke, CommonStrings::None, true);
					PageItem *iteE = m_Doc->Items->at(zE);
					iteE->PoLine = EndArrow.copy();
					finishItem(iteE);
					break;
				}
			}
		}
	}
	if (m_style["draw:marker-start-path"])
	{
		FPointArray EndArrow;
		double EndArrowWidth;
		QString params = QString(m_style["draw:marker-start-path"]->getStr().cstr());
		EndArrowWidth = LineW;
		EndArrow.resize(0);
		EndArrow.svgInit();
		EndArrow.parseSVG(params);
		QPainterPath pa = EndArrow.toQPainterPath(true);
		QRectF br = pa.boundingRect();
		if (m_style["draw:marker-start-width"])
			EndArrowWidth = valueAsPoint(m_style["draw:marker-start-width"]);
		if (EndArrowWidth > 0)
		{
			FPoint Start = ite->PoLine.point(0);
			for (int xx = 1; xx < ite->PoLine.size(); xx += 2)
			{
				FPoint Vector = ite->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					QPointF refP = QPointF(br.width() / 2.0, 0);
					QTransform m;
					m.translate(br.width() / 2.0, br.height() / 2.0);
					m.rotate(r + 90);
					m.translate(-br.width() / 2.0, -br.height() / 2.0);
					m.scale(EndArrowWidth / br.width(), EndArrowWidth / br.width());
					EndArrow.map(m);
					refP = m.map(refP);
					QPainterPath pa2 = EndArrow.toQPainterPath(true);
					//QRectF br2 = pa2.boundingRect();
					QTransform m2;
					FPoint grOffset2(getMinClipF(&EndArrow));
					m2.translate(-grOffset2.x(), -grOffset2.y());
					EndArrow.map(m2);
					refP = m2.map(refP);
					EndArrow.translate(-refP.x(), -refP.y());
					QTransform arrowTrans;
					arrowTrans.translate(-m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
					arrowTrans.translate(Start.x() + ite->xPos(), Start.y() + ite->yPos());
					EndArrow.map(arrowTrans);
					int zS = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, CurrColorStroke, CommonStrings::None, true);
					PageItem *iteS = m_Doc->Items->at(zS);
					iteS->PoLine = EndArrow.copy();
					finishItem(iteS);
					break;
				}
			}
		}
	}
}

void RawPainter::finishItem(PageItem* ite)
{
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setFillEvenOdd(fillrule);
	ite->setLineShade(CurrStrokeShade);
	ite->setLineJoin(lineJoin);
	ite->setLineEnd(lineEnd);
	if (dashArray.count() > 0)
	{
		ite->DashValues = dashArray;
	}
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y(), true);
	ite->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->setFillTransparency(CurrFillTrans);
	ite->setLineTransparency(CurrStrokeTrans);
	ite->updateClip();
	Elements->append(ite);
	if (groupStack.count() != 0)
		groupStack.top().Items.append(ite);
	Coords.resize(0);
	Coords.svgInit();
}

