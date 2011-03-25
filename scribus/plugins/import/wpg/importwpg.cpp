/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QByteArray>
#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QStack>
#include <QDebug>

#include <cstdlib>

#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "importwpg.h"
#include "loadsaveplugin.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "ui/propertiespalette.h"
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
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"
#include <stdio.h>
#include "third_party/wpg/WPGStreamImplementation.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

ScrPainter::ScrPainter(): libwpg::WPGPaintInterface()
{
}

void ScrPainter::startGraphics(double width, double height)
{
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
	fillSet = false;
	strokeSet = false;
	currentGradient = VGradient(VGradient::linear);
	currentGradient.clearStops();
	currentGradient.setRepeatMethod( VGradient::none );
	dashArray.clear();
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->setPage(72 * width, 72 * height, 0, 0, 0, 0, 0, 0, false, false);
		if (width > height)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
		m_Doc->changePageMargins(0, 0, 0, 0, 72 * height, 72 * width, 72 * height, 72 * width, m_Doc->pageOrientation(), m_Doc->pageSize(), m_Doc->currentPage()->pageNr(), 0);
	}
	firstLayer = true;
}

void ScrPainter::endGraphics()
{
}

void ScrPainter::startLayer(unsigned int id)
{
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		if (!firstLayer)
			m_Doc->addLayer(QString("Layer %1").arg(id), true);
		firstLayer = false;
	}
}

void ScrPainter::endLayer(unsigned int id)
{
}

void ScrPainter::setPen(const libwpg::WPGPen& pen)
{
	LineW = 72 * pen.width;
	ScColor tmp;
	ColorList::Iterator it;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	int Rc, Gc, Bc;
	Rc = pen.foreColor.red;
	Gc = pen.foreColor.green;
	Bc = pen.foreColor.blue;
	tmp.setColorRGB(Rc, Gc, Bc);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString newColorName = "FromWPG"+tmp.name();
	QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
	if (fNam == newColorName)
		importedColors.append(newColorName);
	CurrColorStroke = fNam;
	CurrStrokeTrans = pen.foreColor.alpha / 255.0;
	if(!pen.solid)
	{
		dashArray.clear();
		for(unsigned i = 0; i < pen.dashArray.count(); i++)
		{
			dashArray.append(pen.dashArray.at(i)*LineW);
		}
	}
	switch (pen.joinstyle)
	{
		case 1:
			lineJoin = Qt::BevelJoin;
			break;
		case 2:
			lineJoin = Qt::MiterJoin;
			break;
		case 3:
			lineJoin = Qt::RoundJoin;
			break;
		default:
			lineJoin = Qt::MiterJoin;
			break;
	}
	switch (pen.capstyle)
	{
		case 0:
			lineEnd = Qt::FlatCap;
			break;
		case 1:
			lineEnd = Qt::RoundCap;
			break;
		case 2:
			lineEnd = Qt::SquareCap;
			break;
		default:
			lineEnd = Qt::FlatCap;
			break;
	}
	strokeSet = true;
}

void ScrPainter::setBrush(const libwpg::WPGBrush& brush)
{
	ScColor tmp;
	ColorList::Iterator it;
	CurrColorFill = "Black";
	CurrFillShade = 100.0;
	int Rc, Gc, Bc;
	if(brush.style == libwpg::WPGBrush::Solid)
	{
		Rc = brush.foreColor.red;
		Gc = brush.foreColor.green;
		Bc = brush.foreColor.blue;
		tmp.setColorRGB(Rc, Gc, Bc);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString newColorName = "FromWPG"+tmp.name();
		QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
		if (fNam == newColorName)
			importedColors.append(newColorName);
		CurrColorFill = fNam;
		CurrFillTrans = brush.foreColor.alpha / 255.0;
	}
	else if (brush.style == libwpg::WPGBrush::Gradient)
	{
		gradientAngle = brush.gradient.angle();
		isGradient = true;
		currentGradient = VGradient(VGradient::linear);
		currentGradient.clearStops();
		for(unsigned c = 0; c < brush.gradient.count(); c++)
		{
			QString currStopColor = CommonStrings::None;
			Rc = brush.gradient.stopColor(c).red;
			Gc = brush.gradient.stopColor(c).green;
			Bc = brush.gradient.stopColor(c).blue;
			tmp.setColorRGB(Rc, Gc, Bc);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
			QString newColorName = "FromWPG"+tmp.name();
			QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
			if (fNam == newColorName)
				importedColors.append(newColorName);
			currStopColor = fNam;
			const ScColor& gradC = m_Doc->PageColors[currStopColor];
			double pos = qBound(0.0, fabs(brush.gradient.stopOffset(c)), 1.0);
			currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), pos, 0.5, 1.0, currStopColor, 100 );
		}
	}
	else if (brush.style == libwpg::WPGBrush::NoBrush)
		CurrColorFill = CommonStrings::None;
	fillSet = true;
}

void ScrPainter::setFillRule(FillRule rule)
{
	if(rule == libwpg::WPGPaintInterface::WindingFill)
		fillrule = false;
	else
		fillrule = true;
//	qDebug() << "Fill Rule " << fillrule;
}

void ScrPainter::drawRectangle(const libwpg::WPGRect& rect, double rx, double ry)
{
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX, baseY, rect.width() * 72.0, rect.height() * 72.0, LineW, CurrColorFill, CurrColorStroke, true);
	PageItem *ite = m_Doc->Items->at(z);
	if ((rx > 0) && (ry > 0))
	{
		ite->setCornerRadius(qMax(72*rx, 72*ry));
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
	QTransform mm = QTransform();
	mm.translate(72*rect.x1, 72*rect.y1);
	ite->PoLine.map(mm);
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	finishItem(ite);
//	qDebug() << "draw Rect";
}

void ScrPainter::drawEllipse(const libwpg::WPGPoint& center, double rx, double ry)
{
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX, baseY, rx * 144.0, ry * 144.0, LineW, CurrColorFill, CurrColorStroke, true);
	PageItem *ite = m_Doc->Items->at(z);
	QTransform mm = QTransform();
	mm.translate(72*(center.x - rx), 72*(center.y - ry));
	ite->PoLine.map(mm);
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	finishItem(ite);
//	qDebug() << "draw Ellipse";
}

void ScrPainter::drawPolygon(const libwpg::WPGPointArray& vertices, bool closed)
{
	if(vertices.count() < 2)
		return;
	Coords.resize(0);
	Coords.svgInit();
	PageItem *ite;
	Coords.svgMoveTo(72 * vertices[0].x, 72 * vertices[0].y);
	for(unsigned i = 1; i < vertices.count(); i++)
	{
		Coords.svgLineTo(72 * vertices[i].x, 72 * vertices[i].y);
	}
	if (closed)
		Coords.svgClosePath();
	if (Coords.size() > 0)
	{
		int z;
		if (closed)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite);
	}
//	qDebug() << "draw Polygon";
}

void ScrPainter::drawPath(const libwpg::WPGPath& path)
{
	Coords.resize(0);
	Coords.svgInit();
	PageItem *ite;
	for(unsigned i = 0; i < path.count(); i++)
	{
		libwpg::WPGPathElement element = path.element(i);
		libwpg::WPGPoint point = element.point;
		switch(element.type)
		{
			case libwpg::WPGPathElement::MoveToElement:
				Coords.svgMoveTo(72 * point.x, 72 * point.y);
				break;
			case libwpg::WPGPathElement::LineToElement:
				Coords.svgLineTo(72 * point.x, 72 * point.y);
				break;
			case libwpg::WPGPathElement::CurveToElement:
				Coords.svgCurveToCubic(72*element.extra1.x, 72*element.extra1.y, 72*element.extra2.x, 72*element.extra2.y, 72 * point.x, 72 * point.y);
				break;
			default:
				break;
		}
	}
	if (Coords.size() > 0)
	{
		int z;
		if (fillSet)
		{
			if (!path.filled)
				CurrColorFill = CommonStrings::None;
		}
		if (strokeSet)
		{
			if (!path.framed)
				CurrColorStroke = CommonStrings::None;
		}
		if(path.closed)
		{
			Coords.svgClosePath();
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		}
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite);
	}
//	qDebug() << "draw Path" << CurrFillTrans;
}

void ScrPainter::finishItem(PageItem* ite)
{
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setFillEvenOdd(fillrule);
	ite->setLineShade(CurrStrokeShade);
	ite->setLineJoin(lineJoin);
	ite->setLineEnd(lineEnd);
	ite->DashValues = dashArray;
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	if (isGradient)
	{
		ite->fill_gradient = currentGradient;
		ite->GrType = 6;
		QTransform m1;
		m1.rotate(-gradientAngle);
		ite->GrStartX = 0;
		ite->GrStartY = 0;
		QPointF target = m1.map(QPointF(0.0, ite->height()));
		ite->GrEndX = target.x();
		ite->GrEndY = target.y();
	}
	else
	{
		ite->setFillTransparency(CurrFillTrans);
		ite->setLineTransparency(CurrStrokeTrans);
	}
	ite->updateClip();
	Elements.append(ite);
	Coords.resize(0);
	Coords.svgInit();
}

void ScrPainter::drawBitmap(const libwpg::WPGBitmap& bitmap, double hres, double vres)
{
	QImage image = QImage(bitmap.width(), bitmap.height(), QImage::Format_RGB32);
	for(int x = 0; x < bitmap.width(); x++)
	{
		for(int y = 0; y < bitmap.height(); y++)
		{
			libwpg::WPGColor color = bitmap.pixel(x, y);
			image.setPixel(x, y, qRgb(color.red, color.green, color.blue));
		}
	}
	double w = (bitmap.rect.x2 - bitmap.rect.x1) * 72.0;
	double h = (bitmap.rect.y2 - bitmap.rect.y1) * 72.0;
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, bitmap.rect.x1 * 72 + baseX, bitmap.rect.y1 * 72 + baseY, w, h, 1, m_Doc->itemToolPrefs().imageFillColor, CommonStrings::None, true);
	PageItem *ite = m_Doc->Items->at(z);
	ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_wpg_XXXXXX.png");
	ite->tempImageFile->open();
	QString fileName = getLongPathName(ite->tempImageFile->fileName());
	ite->tempImageFile->close();
	ite->isInlineImage = true;
	image.setDotsPerMeterX ((int) (hres / 0.0254));
	image.setDotsPerMeterY ((int) (vres / 0.0254));
	image.save(fileName, "PNG");
	m_Doc->LoadPict(fileName, z);
	ite->setImageScalingMode(false, false);
	ite->moveBy(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	finishItem(ite);
//	qDebug() << "drawBitmap";
}

void ScrPainter::drawImageObject(const libwpg::WPGBinaryData& /*binaryData*/)
{
//	qDebug() << "drawBinaryData";
}


WpgPlug::WpgPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

QImage WpgPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	double b, h;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	progressDialog = NULL;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	baseX = m_Doc->currentPage()->xOffset();
	baseY = m_Doc->currentPage()->yOffset();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (Elements.count() > 1)
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
		if (Elements.count() > 0)
		{
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				tmpSel->addItem(Elements.at(dre), true);
			}
			tmpSel->setGroupRect();
			double xs = tmpSel->width();
			double ys = tmpSel->height();
			tmpImage = Elements.at(0)->DrawObj_toImage(500);
			tmpImage.setText("XSize", QString("%1").arg(xs));
			tmpImage.setText("YSize", QString("%1").arg(ys));
		}
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		m_Doc->m_Selection->delaySignalsOff();
		delete m_Doc;
		return tmpImage;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		delete m_Doc;
	}
	return QImage();
}

bool WpgPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	QFileInfo fi = QFileInfo(fName);
	if ( !ScCore->usingGUI() )
	{
		interactive = false;
		showProgress = false;
	}
	if ( showProgress )
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw );
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing File:");
		QList<bool> barsNumeric;
		barsNumeric << false;
		progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		progressDialog->setOverallTotalSteps(3);
		progressDialog->setOverallProgress(0);
		progressDialog->setProgress("GI", 0);
		progressDialog->show();
		connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
		qApp->processEvents();
	}
	else
		progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = 0.0;
	h = 0.0;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	baseX = 0;
	baseY = 0;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		baseX = 0;
		baseY = 0;
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			baseX = 0;
			baseY = 0;
			baseX = m_Doc->currentPage()->xOffset();
			baseY = m_Doc->currentPage()->yOffset();
		}
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!interactive))
	{
		if (docWidth > docHeight)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if ((Elements.count() > 0) && (!ret) && (interactive))
		{
			if (flags & LoadSavePlugin::lfScripted)
			{
				bool loadF = m_Doc->isLoading();
				m_Doc->setLoading(false);
				m_Doc->changed();
				m_Doc->setLoading(loadF);
				if (!(flags & LoadSavePlugin::lfLoadAsPattern))
				{
					m_Doc->m_Selection->delaySignalsOn();
					for (int dre=0; dre<Elements.count(); ++dre)
					{
						m_Doc->m_Selection->addItem(Elements.at(dre), true);
					}
					m_Doc->m_Selection->delaySignalsOff();
					m_Doc->m_Selection->setGroupRect();
					m_Doc->view()->updatesOn(true);
				}
			}
			else
			{
				m_Doc->DragP = true;
				m_Doc->DraggedElem = 0;
				m_Doc->DragElements.clear();
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					m_Doc->DragElements.append(Elements.at(dre)->ItemNr);
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				ScriXmlDoc *ss = new ScriXmlDoc();
				ScElemMimeData* md = new ScElemMimeData();
				md->setScribusElem(ss->WriteElem(m_Doc, tmpSel));
				delete ss;
				m_Doc->itemSelection_DeleteItem(tmpSel);
				m_Doc->view()->updatesOn(true);
				m_Doc->m_Selection->delaySignalsOff();
				// We must copy the TransationSettings object as it is owned
				// by handleObjectImport method afterwards
				TransactionSettings* transacSettings = new TransactionSettings(trSettings);
				m_Doc->view()->handleObjectImport(md, transacSettings);
				m_Doc->DragP = false;
				m_Doc->DraggedElem = 0;
				m_Doc->DragElements.clear();
			}
		}
		else
		{
			m_Doc->changed();
			m_Doc->reformPages();
			if (!(flags & LoadSavePlugin::lfLoadAsPattern))
				m_Doc->view()->updatesOn(true);
		}
		success = true;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->view()->updatesOn(true);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	if (interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
	{
		if ((showProgress) && (!interactive))
			m_Doc->view()->DrawNew();
	}
	return success;
}

WpgPlug::~WpgPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

bool WpgPlug::convert(QString fn)
{
	QString tmp;
	importedColors.clear();

	QFile file(fn);
	if ( !file.exists() )
	{
		qDebug() << "File " << QFile::encodeName(fn).data() << " does not exist" << endl;
		return false;
	}
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "Cannot open file " << QFile::encodeName(fn).data() << endl;
		return false;
	}
	QByteArray ba = file.readAll();
	file.close();

	libwpg::WPGMemoryStream input(ba.constData(), ba.size());
	if (!libwpg::WPGraphics::isSupported(&input))
	{
		fprintf(stderr, "ERROR: Unsupported file format (unsupported version) or file is encrypted!\n");
		return false;
	}
	ScrPainter painter;
	painter.m_Doc = m_Doc;
	painter.baseX = baseX;
	painter.baseY = baseY;
	painter.flags = importerFlags;
	libwpg::WPGraphics::parse(&input, &painter);
	Elements = painter.Elements;
	importedColors = painter.importedColors;
	if (Elements.count() == 0)
	{
		if (importedColors.count() != 0)
		{
			for (int cd = 0; cd < importedColors.count(); cd++)
			{
				m_Doc->PageColors.remove(importedColors[cd]);
			}
		}
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}
