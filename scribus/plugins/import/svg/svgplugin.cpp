/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QMimeData>
#include <QPainterPath>
#include <QRegExp>
#include <QTemporaryFile>

#include "svgplugin.h"

#include "commonstrings.h"
#include "fonts/scfontmetrics.h"
#include "fpointarray.h"
#include "loadsaveplugin.h"
#include "pageitem.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "qtiocompressor.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "sclimits.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scraction.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/customfdialog.h"
#include "ui/propertiespalette.h"
#include "ui/scmessagebox.h"
#include "ui/scmwmenumanager.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"


using namespace std;

int svgimplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* svgimplugin_getPlugin()
{
	SVGImportPlugin* plug = new SVGImportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void svgimplugin_freePlugin(ScPlugin* plugin)
{
	SVGImportPlugin* plug = dynamic_cast<SVGImportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

SVGImportPlugin::SVGImportPlugin() :
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file format
	// support.
	registerFormats();
	languageChange();
}
/*
void SVGImportPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect(importAction, SIGNAL(triggered()), SLOT(import()));
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}
*/
SVGImportPlugin::~SVGImportPlugin()
{
	unregisterAll();
}

void SVGImportPlugin::languageChange()
{
	importAction->setText( tr("Import &SVG..."));
	FileFormat* fmt = getFormatByExt("svg");
	fmt->trName = FormatsManager::instance()->nameOfFormat(FormatsManager::SVG);
	fmt->filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::SVG);
}

QString SVGImportPlugin::fullTrName() const
{
	return QObject::tr("SVG Import");
}

const ScActionPlugin::AboutData* SVGImportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports SVG Files");
	about->description = tr("Imports most SVG files into the current document, converting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void SVGImportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void SVGImportPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::SVG);
	fmt.formatId = 0;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::SVG);
	fmt.fileExtensions = QStringList() << "svg" << "svgz";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::SVG);
	fmt.priority = 64;
	registerFormat(fmt);
}

bool SVGImportPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	// TODO: identify valid SVG
	return true;
}

bool SVGImportPlugin::loadFile(const QString & fileName, const FileFormat & /* fmt */, int flags, int /*index*/)
{
	// For now, "load file" and import are the same thing for this plugin
	return import(fileName, flags);
}

bool SVGImportPlugin::import(QString filename, int flags)
{
	if (!checkFlags(flags))
		return false;
	m_Doc = ScCore->primaryMainWindow()->doc;
	ScribusMainWindow* mw=(m_Doc==nullptr) ? ScCore->primaryMainWindow() : m_Doc->scMW();
	if (filename.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("SVGPlugin");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(mw, wdir, QObject::tr("Open"), FormatsManager::instance()->fileDialogFormatList(FormatsManager::SVG));
		if (diaf.exec())
		{
			filename = diaf.selectedFile();
			prefs->set("wdir", filename.left(filename.lastIndexOf("/")));
		}
		else
			return true;
	}
	
	UndoTransaction activeTransaction;
	bool emptyDoc = (m_Doc == nullptr);
	bool hasCurrentPage = (m_Doc && m_Doc->currentPage());
	TransactionSettings trSettings;
	trSettings.targetName   = hasCurrentPage ? m_Doc->currentPage()->getUName() : "";
	trSettings.targetPixmap = Um::IImageFrame;
	trSettings.actionName   = Um::ImportSVG;
	trSettings.description  = filename;
	trSettings.actionPixmap = Um::ISVG;
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(false);
	if (UndoManager::undoEnabled())
		activeTransaction = UndoManager::instance()->beginTransaction(trSettings);
	SVGPlug *dia = new SVGPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	dia->import(filename, trSettings, flags);
	if (activeTransaction)
		activeTransaction.commit();
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	if (dia->importCanceled)
	{
		if (dia->importFailed)
			ScMessageBox::warning(mw, CommonStrings::trWarning, tr("The file could not be imported"));
	//	else if (dia->unsupported)
	//		ScMessageBox::warning(mw, CommonStrings::trWarning, tr("SVG file contains some unsupported features"));
	}

	delete dia;
	return true;
}

QImage SVGImportPlugin::readThumbnail(const QString& fileName)
{
	if (fileName.isEmpty())
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	SVGPlug *dia = new SVGPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}

SVGPlug::SVGPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc = doc;
	unsupported = false;
	importFailed = false;
	importCanceled = true;
	firstLayer = true;
	importedColors.clear();
	importedGradients.clear();
	importedPatterns.clear();
	docDesc = "";
	docTitle = "";
	groupLevel = 0;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
//	m_gc.setAutoDelete(true);
}

QImage SVGPlug::readThumbnail(const QString& fName)
{
	if (!loadData(fName))
		return QImage();
	QString CurDirP = QDir::currentPath();
	QFileInfo efp(fName);
	QDir::setCurrent(efp.path());
	SvgStyle *gc = new SvgStyle;
	QDomElement docElem = inpdoc.documentElement();
	QSizeF wh = parseWidthHeight(docElem);
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(wh.width(), wh.height(), 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), nullptr);
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	gc->FontFamily = m_Doc->itemToolPrefs().textFont;
	if (!m_Doc->PageColors.contains("Black"))
		m_Doc->PageColors.insert("Black", ScColor(0, 0, 0, 255));
	m_gc.push(gc);
	viewTransformX = 0;
	viewTransformY = 0;
	viewScaleX = 1;
	viewScaleY = 1;
	if (!docElem.attribute("viewBox").isEmpty())
	{
		QString viewbox(docElem.attribute("viewBox"));
		QStringList points = viewbox.replace(QRegExp(","), " ").simplified().split(' ', QString::SkipEmptyParts);
		if (points.size() > 3)
		{
			QTransform matrix;
			QSizeF wh2 = parseWidthHeight(docElem);
			double w2 = wh2.width();
			double h2 = wh2.height();
			addGraphicContext();
			viewTransformX = ScCLocale::toDoubleC(points[0]);
			viewTransformY = ScCLocale::toDoubleC(points[1]);
			viewScaleX = w2 / ScCLocale::toDoubleC(points[2]);
			viewScaleY = h2 / ScCLocale::toDoubleC(points[3]);
			matrix.translate(-viewTransformX * viewScaleX, -viewTransformY * viewScaleY);
			matrix.scale(viewScaleX, viewScaleY);
			m_gc.top()->matrix = matrix;
		}
	}
	QList<PageItem*> Elements = parseGroup(docElem);
	tmpSel->clear();
	QImage tmpImage = QImage();
	if (Elements.count() > 0)
	{
		m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
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
		m_Doc->m_Selection->delaySignalsOff();
	}
	m_Doc->scMW()->setScriptRunning(false);
	m_Doc->setLoading(false);
	delete m_Doc;
	QDir::setCurrent(CurDirP);
	return tmpImage;
}

bool SVGPlug::import(const QString& fname, const TransactionSettings& trSettings, int flags)
{
	if (!loadData(fname))
	{
		importFailed = true;
		return false;
	}
	QString CurDirP = QDir::currentPath();
	QFileInfo efp(fname);
	QDir::setCurrent(efp.path());
	convert(trSettings, flags);
	QDir::setCurrent(CurDirP);
	return true;
}

bool SVGPlug::loadData(const QString& fName)
{
	bool isCompressed = false, success = false;
	QByteArray bb(3, ' ');
	QFile fi(fName);
	if (fi.open(QIODevice::ReadOnly))
	{
		fi.read(bb.data(), 2);
		fi.close();
		// Qt4 bb[0]->QChar(bb[0])
		if ((QChar(bb[0]) == QChar(0x1F)) && (QChar(bb[1]) == QChar(0x8B)))
			isCompressed = true;
	}
	if ((fName.right(2) == "gz") || (isCompressed))
	{
		QFile file(fName);
		QtIOCompressor compressor(&file);
		compressor.setStreamFormat(QtIOCompressor::GzipFormat);
		if (!compressor.open(QIODevice::ReadOnly))
			return false;
		success = inpdoc.setContent(&compressor);
		compressor.close();
	}
	else
	{
		QFile file(fName);
		if (!file.open(QIODevice::ReadOnly))
			return false;
		success = inpdoc.setContent(&file);
		file.close();
	}
	return success;
}

void SVGPlug::convert(const TransactionSettings& trSettings, int flags)
{
	bool ret = false;
	SvgStyle *gc = new SvgStyle;
	QDomElement docElem = inpdoc.documentElement();
	QSizeF wh = parseWidthHeight(docElem);
	double width = wh.width();
	double height = wh.height();
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
		if (m_Doc->Pages->count() == 0)
		{
			m_Doc->addPage(0);
			m_Doc->view()->addPage(0);
		}
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(width, height, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
		}
	}
	if ((ret) || (!interactive))
	{
		if (width > height)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != nullptr))
		m_Doc->view()->deselectItems();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != nullptr))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	gc->FontFamily = m_Doc->itemToolPrefs().textFont;
	if (!m_Doc->PageColors.contains("Black"))
		m_Doc->PageColors.insert("Black", ScColor(0, 0, 0, 255));
	m_gc.push(gc);
	viewTransformX = 0;
	viewTransformY = 0;
	viewScaleX = 1;
	viewScaleY = 1;
	if (!docElem.attribute("viewBox").isEmpty())
	{
		QString viewbox(docElem.attribute("viewBox"));
		QStringList points = viewbox.replace(QRegExp(","), " ").simplified().split(' ', QString::SkipEmptyParts);
		if (points.size() > 3)
		{
			QTransform matrix;
			QSizeF wh2 = parseWidthHeight(docElem);
			double w2 = wh2.width();
			double h2 = wh2.height();
			addGraphicContext();
			viewTransformX = ScCLocale::toDoubleC(points[0]);
			viewTransformY = ScCLocale::toDoubleC(points[1]);
			viewScaleX = w2 / ScCLocale::toDoubleC(points[2]);
			viewScaleY = h2 / ScCLocale::toDoubleC(points[3]);
			matrix.translate(-viewTransformX * viewScaleX, -viewTransformY * viewScaleY);
			matrix.scale(viewScaleX, viewScaleY);
			m_gc.top()->matrix = matrix;
		}
	}
	Elements += parseDoc(docElem);
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->documentInfo().setTitle(docTitle);
		m_Doc->documentInfo().setComments(docDesc);
	}
	tmpSel->clear();
	if (Elements.count() == 0)
	{
		importFailed = true;
		if ((importedColors.count() != 0) && ((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns)))
			importFailed = false;
		if ((importedGradients.count() != 0) && ((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepPatterns)))
			importFailed = false;
		if ((importedPatterns.count() != 0) && (flags & LoadSavePlugin::lfKeepPatterns))
			importFailed = false;
		if ((importedColors.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns))))
		{
			for (int cd = 0; cd < importedColors.count(); cd++)
			{
				m_Doc->PageColors.remove(importedColors[cd]);
			}
		}
		if ((importedGradients.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients || (flags & LoadSavePlugin::lfKeepPatterns)))))
		{
			for (int cd = 0; cd < importedGradients.count(); cd++)
			{
				m_Doc->docGradients.remove(importedGradients[cd]);
			}
		}
		if ((importedPatterns.count() != 0) && (!(flags & LoadSavePlugin::lfKeepPatterns)))
		{
			for (int cd = 0; cd < importedPatterns.count(); cd++)
			{
				m_Doc->docPatterns.remove(importedPatterns[cd]);
			}
		}
	}
	if ((Elements.count() > 1) && (!(flags & LoadSavePlugin::lfCreateDoc)))
	{
		m_Doc->groupObjectsList(Elements);
	}
	m_Doc->DoDrawing = true;
	m_Doc->scMW()->setScriptRunning(false);
	if (interactive)
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
				if (m_Doc->view() != nullptr)
					m_Doc->view()->updatesOn(true);
			}
			importCanceled = false;
		}
		else
		{
			m_Doc->DragP = true;
			m_Doc->DraggedElem = nullptr;
			m_Doc->DragElements.clear();
			m_Doc->m_Selection->delaySignalsOn();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				tmpSel->addItem(Elements.at(dre), true);
			}
			tmpSel->setGroupRect();
			ScElemMimeData* md = ScriXmlDoc::writeToMimeData(m_Doc, tmpSel);
			m_Doc->itemSelection_DeleteItem(tmpSel);
			m_Doc->view()->updatesOn(true);
			if ((importedColors.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns))))
			{
				for (int cd = 0; cd < importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(importedColors[cd]);
				}
			}
			if ((importedGradients.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepPatterns))))
			{
				for (int cd = 0; cd < importedGradients.count(); cd++)
				{
					m_Doc->docGradients.remove(importedGradients[cd]);
				}
			}
			if ((importedPatterns.count() != 0) && (!(flags & LoadSavePlugin::lfKeepPatterns)))
			{
				for (int cd = 0; cd < importedPatterns.count(); cd++)
				{
					m_Doc->docPatterns.remove(importedPatterns[cd]);
				}
			}
			m_Doc->m_Selection->delaySignalsOff();
			// We must copy the TransationSettings object as it is owned
			// by handleObjectImport method afterwards
			TransactionSettings* transacSettings = new TransactionSettings(trSettings);
			m_Doc->view()->handleObjectImport(md, transacSettings);
			m_Doc->DragP = false;
			m_Doc->DraggedElem = nullptr;
			m_Doc->DragElements.clear();
		}
	}
	else
	{
		bool loadF = m_Doc->isLoading();
		m_Doc->setLoading(false);
		m_Doc->changed();
		m_Doc->reformPages();
		if (!(flags & LoadSavePlugin::lfLoadAsPattern))
			m_Doc->view()->updatesOn(true);
		m_Doc->setLoading(loadF);
	}
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
	{
		if (interactive)
			m_Doc->view()->DrawNew();
	}
	qApp->restoreOverrideCursor();
}

void SVGPlug::addGraphicContext()
{
	SvgStyle *gc = new SvgStyle;
	if (m_gc.top())
	{
		*gc = *(m_gc.top());
		gc->Opacity = 1.0; // opacity is not inheritable contrary to fill-opacity or stroke-opacity
		gc->filter.clear(); // filter is not inheritable
	}
	m_gc.push(gc);
}

void SVGPlug::setupNode(const QDomElement &e)
{
	addGraphicContext();
	setupTransform(e);
	parseStyle(m_gc.top(), e);
}

void SVGPlug::setupTransform(const QDomElement &e)
{
	SvgStyle *gc = m_gc.top();
	QTransform mat = parseTransform(e.attribute("transform"));
	if (!e.attribute("transform").isEmpty())
		gc->matrix = mat * gc->matrix;
}

PageItem *SVGPlug::finishNode(const QDomNode &e, PageItem* item)
{
	PageItem* startArrow = nullptr;
	PageItem* endArrow = nullptr;
	SvgStyle *gc = m_gc.top();
	QTransform gcm = gc->matrix;
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	double coeff1 = sqrt(gcm.m11() * gcm.m11() + gcm.m12() * gcm.m12());
	double coeff2 = sqrt(gcm.m21() * gcm.m21() + gcm.m22() * gcm.m22());
	switch (item->itemType())
	{
	case PageItem::ImageFrame:
		{
			item->ClipEdited = true;
			item->FrameType = 3;
			QTransform mm = gc->matrix;
			item->PoLine.map(mm);
			item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
			FPoint wh = getMaxClipF(&item->PoLine);
			item->setWidthHeight(wh.x(), wh.y());
			m_Doc->adjustItemSize(item);
			item->ContourLine = item->PoLine.copy();
//			item->moveBy(mm.dx(), mm.dy());
//			item->setWidthHeight(item->width() * mm.m11(), item->height() * mm.m22());
//			item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
			if (item->imageIsAvailable)
			{
				item->setImageXYOffset(0.0, 0.0);
				item->setImageXYScale(item->width() / (item->pixm.width() * (item->pixm.imgInfo.xres / 72.0)),
									  item->height() / (item->pixm.height() * (item->pixm.imgInfo.yres / 72.0)));
				item->setImageScalingMode(false, false); // fit to frame
			}
			break;
		}
	case PageItem::TextFrame:
		{
			//QTransform mm = gc->matrix;
			item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
			item->itemText.trim();
		}
		break;
	default:
		{
			item->ClipEdited = true;
			item->FrameType = 3;
			QTransform mm = gc->matrix;
			item->PoLine.map(mm);
			/*if (haveViewBox)
			{
				QTransform mv;
				mv.translate(viewTransformX, viewTransformY);
				mv.scale(viewScaleX, viewScaleY);
				ite->PoLine.map(mv);
			}*/
			item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
			FPoint wh = getMaxClipF(&item->PoLine);
			item->setWidthHeight(wh.x(), wh.y());
//			if (item->asPolyLine())
//				item->setPolyClip(qRound(qMax(item->lineWidth() / 2.0, 1)));
//			else
//				item->Clip = flattenPath(item->PoLine, item->Segments);
			FPoint wx = getMinClipF(&item->PoLine);
			inGroupXOrigin = qMin(inGroupXOrigin, wx.x());
			inGroupYOrigin = qMin(inGroupYOrigin, wx.y());
			m_Doc->adjustItemSize(item);
			break;
		}
	}
	item->setRedrawBounding();
	item->OwnPage = m_Doc->OnPage(item);
	if (e.isElement())
	{
		if (!(importerFlags & LoadSavePlugin::lfCreateDoc))
		{
			QString nodeId = e.toElement().attribute("id");
			if (!nodeId.isEmpty())
				item->setItemName(" "+nodeId);
		}
	}
	item->setFillTransparency(1 - gc->FillOpacity * gc->Opacity);
	item->setLineTransparency(1 - gc->StrokeOpacity * gc->Opacity);
	item->PLineEnd = gc->PLineEnd;
	item->PLineJoin = gc->PLineJoin;
	if (item->fillColor() == CommonStrings::None)
		item->setTextFlowMode(PageItem::TextFlowDisabled);
	else
		item->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
	item->DashOffset = gc->dashOffset;
	item->DashValues = gc->dashArray;
	if (gc->FillGradientType != 0)
	{
		if (gc->FillGradientType == 8)
		{
			item->GrType = gc->FillGradientType;
			item->setPattern(importedPattTrans[gc->GFillCol1]);
			QTransform mm = gc->matrixgf;
			double rot = getRotationFromMatrix(mm, 0.0) * 180 / M_PI;
			mm.rotate(rot);
			double patDx = (item->xPos() - baseX) - mm.dx();
			double patDy = (item->yPos() - baseY) - mm.dy();
			item->setPatternTransform(mm.m11() * 100.0, mm.m22() * 100.0, patDx, patDy, -rot, 0, 0);
		}
		else
		{
			if (gc->FillGradient.stops() > 1)
			{
				item->fill_gradient = gc->FillGradient;
				item->setGradient(importedGradTrans[gc->GFillCol1]);
				item->setGradientExtend(VGradient::pad);
				if (!gc->FillCSpace)
				{
					item->GrStartX = gc->GradFillX1 * item->width();
					item->GrStartY = gc->GradFillY1 * item->height();
					item->GrEndX   = gc->GradFillX2 * item->width();
					item->GrEndY   = gc->GradFillY2 * item->height();
					item->GrFocalX = gc->GradFillFX * item->width();
					item->GrFocalY = gc->GradFillFY * item->height(); 
					double angle1 = atan2(gc->GradFillY2-gc->GradFillY1,gc->GradFillX2-gc->GradFillX1)*(180.0/M_PI);
					double angle2 = atan2(item->GrEndY - item->GrStartX, item->GrEndX - item->GrStartX)*(180.0/M_PI);
					double dx = item->GrStartX + (item->GrEndX - item->GrStartX) / 2.0;
					double dy = item->GrStartY + (item->GrEndY - item->GrStartY) / 2.0;
					QTransform mm, mm2;
					if ((gc->GradFillY1 < gc->GradFillY2) && (gc->GradFillX1 < gc->GradFillX2))
					{
						mm.rotate(-angle2);
						mm2.rotate(angle1);
					}
					FPointArray gra;
					gra.setPoints(3, item->GrStartX-dx, item->GrStartY-dy, item->GrEndX-dx, item->GrEndY-dy, item->GrFocalX-dx, item->GrFocalY-dy);
					gra.map(mm*mm2);
					gra.translate(dx, dy);
					item->GrStartX = gra.point(0).x();
					item->GrStartY = gra.point(0).y();
					item->GrEndX   = gra.point(1).x();
					item->GrEndY   = gra.point(1).y();
					item->GrFocalX = gra.point(2).x();
					item->GrFocalY = gra.point(2).y();
				}
				else
				{
					QTransform mm = gc->matrix;
					mm = gc->matrixgf * mm;
					FPointArray gra;
					gra.setPoints(3, gc->GradFillX1, gc->GradFillY1, gc->GradFillX2, gc->GradFillY2, gc->GradFillFX, gc->GradFillFY);
					gra.map(mm);
					item->GrStartX = gra.point(0).x() - item->xPos() + baseX;
					item->GrStartY = gra.point(0).y() - item->yPos() + baseY;
					item->GrEndX   = gra.point(1).x() - item->xPos() + baseX;
					item->GrEndY   = gra.point(1).y() - item->yPos() + baseY;
					item->GrFocalX = gra.point(2).x() - item->xPos() + baseX;
					item->GrFocalY = gra.point(2).y() - item->yPos() + baseY;
					double ScaleX = 1.0;
					double ScaleY = 1.0;
					getScaleFromMatrix(mm, ScaleX, ScaleY);
					if (ScaleX != ScaleY)
						item->GrScale = ScaleY / ScaleX;
				}
				item->GrType = gc->FillGradientType;
			}
			else
			{
				item->GrType = 0;
				QList<VColorStop*> cstops = gc->FillGradient.colorStops();
				item->setFillColor(cstops.at(0)->name);
				item->setFillShade(cstops.at(0)->shade);
			}
		}
	}
	if (gc->StrokeGradientType != 0)
	{
		if (gc->StrokeGradientType == 8)
		{
			item->GrTypeStroke = gc->StrokeGradientType;
			item->setPattern(importedPattTrans[gc->GStrokeCol1]);
			QTransform mm = gc->matrixgs;
			double rot = getRotationFromMatrix(mm, 0.0) * 180 / M_PI;
			mm.rotate(rot);
			double patDx = (item->xPos() - baseX) - mm.dx();
			double patDy = (item->yPos() - baseY) - mm.dy();
			item->setStrokePatternTransform(mm.m11() * 100.0, mm.m22() * 100.0, patDx, patDy, -rot, 0, 0, 1);
		}
		else
		{
			if (gc->StrokeGradient.stops() > 1)
			{
				item->stroke_gradient = gc->StrokeGradient;
				item->setStrokeGradient(importedGradTrans[gc->GStrokeCol1]);
				item->setStrokeGradientExtend(VGradient::pad);
				if (!gc->StrokeCSpace)
				{
					item->GrStrokeStartX = gc->GradStrokeX1 * item->width();
					item->GrStrokeStartY = gc->GradStrokeY1 * item->height();
					item->GrStrokeEndX   = gc->GradStrokeX2 * item->width();
					item->GrStrokeEndY   = gc->GradStrokeY2 * item->height();
					item->GrStrokeFocalX = gc->GradStrokeFX * item->width();
					item->GrStrokeFocalY = gc->GradStrokeFY * item->height();
					double angle1 = atan2(gc->GradStrokeY2 - gc->GradStrokeY1, gc->GradStrokeX2 - gc->GradStrokeX1)*(180.0/M_PI);
					double angle2 = atan2(item->GrStrokeEndY - item->GrStrokeStartX, item->GrStrokeEndX - item->GrStrokeStartX)*(180.0/M_PI);
					double dx = item->GrStrokeStartX + (item->GrStrokeEndX - item->GrStrokeStartX) / 2.0;
					double dy = item->GrStrokeStartY + (item->GrStrokeEndY - item->GrStrokeStartY) / 2.0;
					QTransform mm, mm2;
					if ((gc->GradStrokeY1 < gc->GradStrokeY2) && (gc->GradStrokeX1 < gc->GradStrokeX2))
					{
						mm.rotate(-angle2);
						mm2.rotate(angle1);
					}
					FPointArray gra;
					gra.setPoints(3, item->GrStrokeStartX - dx, item->GrStrokeStartY - dy, item->GrStrokeEndX-dx, item->GrStrokeEndY-dy, item->GrStrokeFocalX-dx, item->GrStrokeFocalY-dy);
					gra.map(mm*mm2);
					gra.translate(dx, dy);
					item->GrStrokeStartX = gra.point(0).x();
					item->GrStrokeStartY = gra.point(0).y();
					item->GrStrokeEndX   = gra.point(1).x();
					item->GrStrokeEndY   = gra.point(1).y();
					item->GrStrokeFocalX = gra.point(2).x();
					item->GrStrokeFocalY = gra.point(2).y();
				}
				else
				{
					QTransform mm = gc->matrix;
					mm = gc->matrixgs * mm;
					FPointArray gra;
					gra.setPoints(3, gc->GradStrokeX1, gc->GradStrokeY1, gc->GradStrokeX2, gc->GradStrokeY2, gc->GradStrokeFX, gc->GradStrokeFY);
					gra.map(mm);
					item->GrStrokeStartX = gra.point(0).x() - item->xPos() + baseX;
					item->GrStrokeStartY = gra.point(0).y() - item->yPos() + baseY;
					item->GrStrokeEndX   = gra.point(1).x() - item->xPos() + baseX;
					item->GrStrokeEndY   = gra.point(1).y() - item->yPos() + baseY;
					item->GrStrokeFocalX = gra.point(2).x() - item->xPos() + baseX;
					item->GrStrokeFocalY = gra.point(2).y() - item->yPos() + baseY;
				}
				item->GrTypeStroke = gc->StrokeGradientType;
			}
			else
			{
				item->GrTypeStroke = 0;
				QList<VColorStop*> cstops = gc->StrokeGradient.colorStops();
				item->setLineColor(cstops.at(0)->name);
				item->setLineShade(cstops.at(0)->shade);
			}
		}
	}
	if (!gc->filter.isEmpty())
	{
		if (filters.contains(gc->filter))
		{
			filterSpec filter = filters[gc->filter];
			item->setFillBlendmode(filter.blendMode);
		}
	}
	if (!gc->endMarker.isEmpty())
	{
		QString marker = gc->endMarker;
		if (importedPattTrans.contains(marker))
			marker = importedPattTrans[marker];
		if (markers.contains(marker))
		{
			FPoint End = item->PoLine.point(item->PoLine.size()-2);
			for (uint xx = item->PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					QTransform arrowTrans;
					double bX = item->xPos() + End.x();
					double bY = item->yPos() + End.y();
					ScPattern pat = m_Doc->docPatterns[marker];
					double dX = (pat.width * item->lineWidth()) / 2.0;
					double dY = (pat.height * item->lineWidth()) / 2.0;
					arrowTrans.translate(bX, bY);
					arrowTrans.rotate(r);
					arrowTrans.translate(-dX, -dY);
					FPoint ba = FPoint(0.0, 0.0).transformPoint(arrowTrans, false);
					int z = m_Doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, ba.x(), ba.y(), dX * 2.0, dY * 2.0, 0, CommonStrings::None, CommonStrings::None);
					endArrow = m_Doc->Items->at(z);
					endArrow->setPattern(marker);
					endArrow->setRotation(r, true);
					endArrow->setRedrawBounding();
					endArrow->OwnPage = m_Doc->OnPage(endArrow);
				//	Elements.append(ite);
					break;
				}
			}
		}
	}
	if (!gc->startMarker.isEmpty())
	{
		QString marker = gc->endMarker;
		if (importedPattTrans.contains(marker))
			marker = importedPattTrans[marker];
		if (markers.contains(marker))
		{
			FPoint End = item->PoLine.point(0);
			for (int xx = 1; xx < item->PoLine.size(); xx += 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);// - 180.0;
					QTransform arrowTrans;
					double bX = item->xPos() + End.x();
					double bY = item->yPos() + End.y();
					ScPattern pat = m_Doc->docPatterns[marker];
					double dX = (pat.width * item->lineWidth()) / 2.0;
					double dY = (pat.height * item->lineWidth()) / 2.0;
					arrowTrans.translate(bX, bY);
					arrowTrans.rotate(r);
					arrowTrans.translate(-dX, -dY);
					FPoint ba = FPoint(0.0, 0.0).transformPoint(arrowTrans, false);
					int z = m_Doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, ba.x(), ba.y(), dX * 2.0, dY * 2.0, 0, CommonStrings::None, CommonStrings::None);
					startArrow = m_Doc->Items->at(z);
					startArrow->setPattern(marker);
					startArrow->setRotation(r, true);
					startArrow->setRedrawBounding();
					startArrow->OwnPage = m_Doc->OnPage(startArrow);
				//	Elements.append(ite);
					break;
				}
			}
		}
	}
	if ((endArrow != nullptr) || (startArrow != nullptr))
	{
		QList<PageItem*> aElements;
		aElements.append(item);
		if (startArrow != nullptr)
			aElements.append(startArrow);
		if (endArrow != nullptr)
			aElements.append(endArrow);
		return m_Doc->groupObjectsList(aElements);
	}
	return item;
}

bool SVGPlug::isIgnorableNode(const QDomElement &e)
{
	QString nodeName(e.tagName());
	return isIgnorableNodeName(nodeName);
}

bool SVGPlug::isIgnorableNodeName(const QString &n)
{
	return n.startsWith("sodipodi") || n.startsWith("inkscape") || n == "metadata";
}

FPoint SVGPlug::parseTextPosition(const QDomElement &e, const FPoint* pos)
{
	// FIXME According to spec, we should in fact return a point list
	double  x = pos ? pos->x() : 0.0;
	double  y = pos ? pos->y() : 0.0;

	if (e.hasAttribute("x"))
	{
		QString xatt =  e.attribute("x" , "0");
		if (xatt.contains(',') || xatt.contains(' '))
		{
			xatt.replace(QChar(','), QChar(' '));
			QStringList xl(xatt.split(QChar(' '), QString::SkipEmptyParts));
			xatt = xl.first();
		}
		x = parseUnit(xatt); 
	}

	if (e.hasAttribute("y"))
	{
		QString yatt =  e.attribute("y" , "0");
		if (yatt.contains(',') || yatt.contains(' '))
		{
			yatt.replace(QChar(','), QChar(' '));
			QStringList yl(yatt.split(QChar(' '), QString::SkipEmptyParts));
			yatt = yl.first();
		}
		y = parseUnit(yatt);
	}

	if (e.hasAttribute("dx"))
	{
		QString dxatt =  e.attribute("dx" , "0");
		if (dxatt.contains(',') || dxatt.contains(' '))
		{
			dxatt.replace(QChar(','), QChar(' '));
			QStringList xl(dxatt.split(QChar(' '), QString::SkipEmptyParts));
			dxatt = xl.first();
		}
		x += parseUnit(dxatt); 
	}

	if (e.hasAttribute("dy"))
	{
		QString dyatt =  e.attribute("dy" , "0");
		if (dyatt.contains(',') || dyatt.contains(' '))
		{
			dyatt.replace(QChar(','), QChar(' '));
			QStringList xl(dyatt.split(QChar(' '), QString::SkipEmptyParts));
			dyatt = xl.first();
		}
		y += parseUnit(dyatt); 
	}

	return FPoint(x, y);
}

QSizeF SVGPlug::parseWidthHeight(const QDomElement &e)
{
	QSizeF size(550, 841);
	QString sw = e.attribute("width", "100%");
	QString sh = e.attribute("height", "100%");
	double w =  550, h = 841;
	if (!sw.isEmpty())
		w = sw.endsWith("%") ? fromPercentage(sw) : parseUnit(sw);
	if (!sh.isEmpty())
		h = sh.endsWith("%") ? fromPercentage(sh) : parseUnit(sh);
	if (!e.attribute("viewBox").isEmpty())
	{
		QRectF viewBox = parseViewBox(e);
		double scw = (viewBox.width() > 0 && viewBox.height() > 0) ? viewBox.width()  : size.width();
		double sch = (viewBox.width() > 0 && viewBox.height() > 0) ? viewBox.height() : size.height();
		w *= (sw.endsWith("%") ? scw : 1.0);
		h *= (sh.endsWith("%") ? sch : 1.0);
	}
	else
	{
		w *= (sw.endsWith("%") ? size.width() : 1.0);
		h *= (sh.endsWith("%") ? size.height() : 1.0);
	}
	// OpenOffice files may not have width and height attributes, so avoid unnecessary large dimensions
	if (w > 10000 || h > 10000)
	{
		double m = max(w, h);
		w = w / m * 842;
		h = h / m * 842;
	}
	size.setWidth(w);
	size.setHeight(h);
	return size;
}

QRectF SVGPlug::parseViewBox(const QDomElement &e)
{
	QRectF box(0, 0, 0, 0);
	if (!e.attribute("viewBox").isEmpty())
	{
		QString viewbox(e.attribute("viewBox"));
		QStringList points = viewbox.replace(QRegExp(","), " ").simplified().split(' ', QString::SkipEmptyParts);
		if (points.size() > 3)
		{
			double left   = ScCLocale::toDoubleC(points[0]);
			double bottom = ScCLocale::toDoubleC(points[1]);
			double width  = ScCLocale::toDoubleC(points[2]);
			double height = ScCLocale::toDoubleC(points[3]);
			box.setCoords((int) left, (int) bottom, (int) (left + width), (int) (bottom + height));
		}
	}
	return box;
}

void SVGPlug::parseDefs(const QDomElement &e)
{
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement b = n.toElement();
		if (b.isNull())
			continue;
		SvgStyle svgStyle;
		parseStyle(&svgStyle, b);
		if (!svgStyle.Display) 
			continue;
		QString STag2 = parseTagName(b);
		if (STag2 == "g")
		{
			QString id = b.attribute("id", "");
			if (!id.isEmpty())
				m_nodeMap.insert(id, b);
			parseDefs(b);
		}
		else if (STag2 == "linearGradient" || STag2 == "radialGradient")
			parseGradient(b);
		else if (STag2 == "clipPath")
			parseClipPath(b);
		else if (STag2 == "pattern")
			parsePattern(b);
		else if (STag2 == "marker")
			parseMarker(b);
		else if (STag2 == "filter")
			parseFilter(b);
		else if (b.hasAttribute("id"))
		{
			QString id = b.attribute("id");
			if (!id.isEmpty())
				m_nodeMap.insert(id, b);
		}
	}
}

void SVGPlug::parseClipPath(const QDomElement &e)
{
	QString id(e.attribute("id"));
	if (!id.isEmpty())
	{
		FPointArray clip;
		QDomNode n2 = e.firstChild();
		QDomElement b2 = n2.toElement();
		while (b2.nodeName() == "use")
			b2 = getReferencedNode(b2);
		if (b2.nodeName() == "path")
			clip.parseSVG(b2.attribute("d"));
		else if (b2.nodeName() == "rect")
		{
			double x = parseUnit(b2.attribute("x", "0.0"));
			double y = parseUnit(b2.attribute("y", "0.0"));
			double width  = parseUnit(b2.attribute("width"));
			double height = parseUnit(b2.attribute("height"));
			clip.addQuadPoint(x, y, x, y, width+x, y, width+x, y);
			clip.addQuadPoint(width+x, y, width+x, y, width+x, height+y, width+x, height+y);
			clip.addQuadPoint(width+x, height+y, width+x, height+y, x, height+y, x, height+y);
			clip.addQuadPoint(x, height+y, x, height+y, x, y, x, y);
		}
		if (b2.hasAttribute("transform"))
		{
			QTransform transform = parseTransform(b2.attribute("transform"));
			clip.map(transform);
		}
		if (clip.size() >= 2)
			m_clipPaths.insert(id, clip);
	}
}

void SVGPlug::parseClipPathAttr(const QDomElement &e, FPointArray& clipPath)
{
	clipPath.resize(0);
/*	if (e.hasAttribute("style"))
	{
		QString style = e.attribute("style").simplified();
		QStringList substyles = style.split(';', QString::SkipEmptyParts);
		for (QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it)
		{
			QStringList substyle = (*it).split(':', QString::SkipEmptyParts);
			QString command(substyle[0].trimmed());
			QString params(substyle[1].trimmed());
			if (command == "clip-path")
			{
				if (params.startsWith( "url("))
				{
					unsigned int start = params.indexOf("#") + 1;
					unsigned int end = params.lastIndexOf(")");
					QString key = params.mid(start, end - start);
					QMap<QString, FPointArray>::iterator it = m_clipPaths.find(key);
					if (it != m_clipPaths.end())
						clipPath = it.value().copy();
				}
			}
		}
	} */
	if (e.hasAttribute("clip-path"))
	{
		QString attr = e.attribute("clip-path");
		if (attr.startsWith( "url("))
		{
			unsigned int start = attr.indexOf("#") + 1;
			unsigned int end = attr.lastIndexOf(")");
			QString key = attr.mid(start, end - start);
			QMap<QString, FPointArray>::iterator it = m_clipPaths.find(key);
			if (it != m_clipPaths.end())
				clipPath = it.value().copy();
		}
	}
}

void SVGPlug::parseFilterAttr(const QDomElement &e, PageItem* item)
{
	QString filterName;
	if (e.hasAttribute("filter"))
	{
		QString attr = e.attribute("filter");
		if (attr.startsWith( "url("))
		{
			unsigned int start = attr.indexOf("#") + 1;
			unsigned int end = attr.lastIndexOf(")");
			filterName = attr.mid(start, end - start);
			if (filterName.isEmpty())
				return;
		}
		if (filters.contains(filterName))
		{
			filterSpec spec = filters[filterName];
			item->setFillBlendmode(spec.blendMode);
		}
	}
}

QList<PageItem*> SVGPlug::parseA(const QDomElement &e)
{
	QList<PageItem*> aElements;
	setupNode(e);
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement b = n.toElement();
		if (b.isNull() || isIgnorableNode(b))
			continue;
		SvgStyle svgStyle;
		parseStyle(&svgStyle, b);
		if (!svgStyle.Display) 
			continue;
		QList<PageItem*> el = parseElement(b);
		for (int ec = 0; ec < el.count(); ++ec)
			aElements.append(el.at(ec));
	}
	delete (m_gc.pop());
	return aElements;
}

QList<PageItem*> SVGPlug::parseGroup(const QDomElement &e)
{
	FPointArray clipPath;
	QList<PageItem*> GElements, gElements;

	if ((importerFlags & LoadSavePlugin::lfCreateDoc) && (e.hasAttribute("inkscape:groupmode")) && (e.attribute("inkscape:groupmode") == "layer"))
	{
		setupNode(e);
		QString layerName = e.attribute("inkscape:label", "Layer");
		double trans = m_gc.top()->Opacity;
		int currentLayer = 0;
		if (!firstLayer)
			currentLayer = m_Doc->addLayer(layerName, true);
		else
			m_Doc->changeLayerName(currentLayer, layerName);
		m_Doc->setLayerVisible(currentLayer, true);
		m_Doc->setLayerLocked(currentLayer, false);
		m_Doc->setLayerPrintable(currentLayer, true);
		m_Doc->setLayerTransparency(currentLayer, trans);
		firstLayer = false;
		for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
		{
			QDomElement b = n.toElement();
			if (b.isNull() || isIgnorableNode(b))
				continue;
			SvgStyle svgStyle;
			parseStyle(&svgStyle, b);
			if (!svgStyle.Display) 
				continue;
			QList<PageItem*> el = parseElement(b);
			for (int ec = 0; ec < el.count(); ++ec)
				GElements.append(el.at(ec));
		}
		delete (m_gc.pop());
		return GElements;
	}

	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	groupLevel++;
	setupNode(e);
	parseClipPathAttr(e, clipPath);
	int z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, baseX, baseY, 1, 1, 0, CommonStrings::None, CommonStrings::None);
	PageItem *neu = m_Doc->Items->at(z);
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement b = n.toElement();
		if (b.isNull() || isIgnorableNode(b))
			continue;
		SvgStyle svgStyle;
		parseStyle(&svgStyle, b);
		if (!svgStyle.Display) 
			continue;
		QList<PageItem*> el = parseElement(b);
		for (int ec = 0; ec < el.count(); ++ec)
			gElements.append(el.at(ec));
	}
	groupLevel--;
	SvgStyle *gc = m_gc.top();
	if (clipPath.empty())
	{
		if (!gc->clipPath.empty())
			clipPath = gc->clipPath.copy();
	}
	parseFilterAttr(e, neu);

	if (gElements.count() == 0 || (gElements.count() < 2 && (clipPath.empty()) && (gc->Opacity == 1.0)))
	{
		// Unfortunately we have to take the long route here, or we risk crash on undo/redo
		// FIXME : create group object after parsing grouped objects
		/*m_Doc->Items->takeAt(z);
		delete neu;*/
		Selection tmpSelection(m_Doc, false);
		tmpSelection.addItem(neu);
		m_Doc->itemSelection_DeleteItem(&tmpSelection);
		for (int gr = 0; gr < gElements.count(); ++gr)
		{
			GElements.append(gElements.at(gr));
		}
		delete (m_gc.pop());
		return GElements;
	}

	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	GElements.append(neu);
	for (int gr = 0; gr < gElements.count(); ++gr)
	{
		PageItem* currItem = gElements.at(gr);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	double gx = minx;
	double gy = miny;
	double gw = maxx - minx;
	double gh = maxy - miny;
	if (((gx > -9999999) && (gx < 9999999)) && ((gy > -9999999) && (gy < 9999999)) && ((gw > 0) && (gw < 9999999)) && ((gh > 0) && (gh < 9999999)))
	{
		neu->setXYPos(gx, gy);
		neu->setWidthHeight(gw, gh);
		if (!clipPath.empty())
		{
			QTransform mm = gc->matrix;
			neu->PoLine = clipPath.copy();
			neu->PoLine.map(mm);
			neu->PoLine.translate(-gx + baseX, -gy + baseY);
			clipPath.resize(0);
			neu->Clip = flattenPath(neu->PoLine, neu->Segments);
			neu->ClipEdited = true;
			neu->FrameType = 3;
		}
		else
			neu->SetRectFrame();
		if (!e.attribute("id").isEmpty())
			neu->setItemName(e.attribute("id"));
		else
			neu->setItemName( tr("Group%1").arg(m_Doc->GroupCounter));
		neu->setFillTransparency(1 - gc->Opacity);
		neu->gXpos = neu->xPos() - gx;
		neu->gYpos = neu->yPos() - gy;
		neu->groupWidth = gw;
		neu->groupHeight = gh;
		for (int gr = 0; gr < gElements.count(); ++gr)
		{
			PageItem* currItem = gElements.at(gr);
			currItem->gXpos = currItem->xPos() - gx;
			currItem->gYpos = currItem->yPos() - gy;
			currItem->gWidth = gw;
			currItem->gHeight = gh;
			currItem->Parent = neu;
			neu->groupItemList.append(currItem);
			m_Doc->Items->removeAll(currItem);
		}
		neu->setRedrawBounding();
		neu->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->GroupCounter++;
	}
	else
	{
		// Group is out of valid coordinates, remove it
		GElements.removeAll(neu);
		Selection tmpSelection(m_Doc, false);
		tmpSelection.addItem(neu);
		for (int gr = 0; gr < gElements.count(); ++gr)
		{
			tmpSelection.addItem(gElements.at(gr));
		}
		m_Doc->itemSelection_DeleteItem(&tmpSelection);
	}
	delete (m_gc.pop());

	return GElements;
}

QList<PageItem*> SVGPlug::parseDoc(const QDomElement &e)
{
	QList<PageItem*> GElements;
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement b = n.toElement();
		if (b.isNull() || isIgnorableNode(b))
			continue;
		SvgStyle svgStyle;
		parseStyle(&svgStyle, b);
		if (!svgStyle.Display) 
			continue;
		QList<PageItem*> el = parseElement(b);
		for (int ec = 0; ec < el.count(); ++ec)
			GElements.append(el.at(ec));
	}
	return GElements;
}

QList<PageItem*> SVGPlug::parseElement(const QDomElement &e)
{
	QList<PageItem*> GElements;
	if (e.hasAttribute("id"))
		m_nodeMap.insert(e.attribute("id"), e);
	QString STag = parseTagName(e);
	if (STag.startsWith("svg:"))
		STag = STag.mid(4, - 1);
	if (STag == "g" )
	{
		GElements = parseGroup(e);
		return GElements;
	}
	if (STag == "defs")
		parseDefs(e);
	else if (STag == "a")
		GElements = parseA(e);
	else if (STag == "switch")
		GElements = parseSwitch(e);
	else if (STag == "symbol")
		GElements = parseSymbol(e);
	else if (STag == "use")
		GElements = parseUse(e);
	else if (STag == "linearGradient" || STag == "radialGradient")
		parseGradient(e);
	else if (STag == "rect")
		GElements = parseRect(e);
	else if (STag == "ellipse")
		GElements = parseEllipse(e);
	else if (STag == "circle")
		GElements = parseCircle(e);
	else if (STag == "line")
		GElements = parseLine(e);
	else if (STag == "path")
		GElements = parsePath(e);
	else if (STag == "polyline" || STag == "polygon")
		GElements = parsePolyline(e);
	else if (STag == "text")
		GElements = parseText(e);
	else if (STag == "clipPath")
		parseClipPath(e);
	else if (STag == "desc")
	{
		if (groupLevel == 1)
			docDesc = e.text();
	}
	else if (STag == "title")
	{
		if (groupLevel == 1)
			docTitle = e.text();
	}
	else if (STag == "image")
		GElements = parseImage(e);
/*	else if (STag == "i:pgf")
	{
		QByteArray cdat;
		QByteArray ddat;
		cdat = e.text().simplified();
		QList<QByteArray> cdlist = cdat.split(' ');
		for (int cd = 0; cd < cdlist.count(); cd++)
		{
			ddat += QByteArray::fromBase64(cdlist[cd]);
		}
		QFile outf("/home/franz/testdata.txt");
		outf.open(QIODevice::WriteOnly);
		outf.write(ddat);
		outf.close();
		QString f2 = "/home/franz/testdata_decom.ai";
		FILE *source = fopen("/home/franz/testdata.txt", "rb");
		FILE *dest = fopen(f2, "wb");
		int ret;
		unsigned have;
		z_stream strm;
		char in[4096];
		char out[4096];
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		ret = inflateInit(&strm);
		if (ret != Z_OK)
			return GElements;
		do
		{
			strm.avail_in = fread(in, 1, 4096, source);
			if (ferror(source))
			{
				(void)inflateEnd(&strm);
				return GElements;
			}
			if (strm.avail_in == 0)
				break;
			strm.next_in = (Bytef*)in;
			do
			{
				strm.avail_out = 4096;
				strm.next_out = (Bytef*)out;
				ret = inflate(&strm, Z_NO_FLUSH);
				assert(ret != Z_STREAM_ERROR);
				switch (ret)
				{
					case Z_NEED_DICT:
						ret = Z_DATA_ERROR;
					case Z_DATA_ERROR:
					case Z_MEM_ERROR:
						(void)inflateEnd(&strm);
						return GElements;
				}
				have = 4096 - strm.avail_out;
				if (fwrite(out, 1, have, dest) != have || ferror(dest))
				{
					(void)inflateEnd(&strm);
					return GElements;
				}
			}
			while (strm.avail_out == 0);
		}
		while (ret != Z_STREAM_END);
		(void)inflateEnd(&strm);
		fclose(source);
		fclose(dest);
	} */
/*	else if (STag == "image")
		GElements = parseImage(e);
	} */
	else if (!isIgnorableNodeName(STag))
	{
		// warn if unsupported SVG feature are encountered
		if (!m_unsupportedFeatures.contains(STag))
		{
			m_unsupportedFeatures.insert(STag, STag);
	//		qDebug() << QString("unsupported SVG feature: %1").arg(STag);
			unsupported = true;
		}
	}
	return GElements;
}

QList<PageItem*> SVGPlug::parseCircle(const QDomElement &e)
{
	QList<PageItem*> CElements;
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	double r = parseUnit(e.attribute("r"));
	double x = parseUnit(e.attribute("cx")) - r;
	double y = parseUnit(e.attribute("cy")) - r;
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX, baseY, r * 2.0, r * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol);
	PageItem* ite = m_Doc->Items->at(z);
	QTransform mm = QTransform();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	finishNode(e, ite);
	CElements.append(ite);
	delete (m_gc.pop());
	return CElements;
}

QList<PageItem*> SVGPlug::parseEllipse(const QDomElement &e)
{
	QList<PageItem*> EElements;
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	double rx = parseUnit(e.attribute("rx"));
	double ry = parseUnit(e.attribute("ry"));
	double x = parseUnit(e.attribute("cx")) - rx;
	double y = parseUnit(e.attribute("cy")) - ry;
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX, baseY, rx * 2.0, ry * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol);
	PageItem* ite = m_Doc->Items->at(z);
	QTransform mm = QTransform();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	finishNode(e, ite);
	EElements.append(ite);
	delete (m_gc.pop());
	return EElements;
}

QList<PageItem*> SVGPlug::parseImage(const QDomElement &e)
{
	FPointArray clipPath;
	QList<PageItem*> IElements;
	QString fname = e.attribute("xlink:href");
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	double x = e.attribute("x").isEmpty() ? 0.0 : parseUnit(e.attribute("x"));
	double y = e.attribute("y").isEmpty() ? 0.0 : parseUnit(e.attribute("y"));
	double w = e.attribute("width").isEmpty() ? 1.0 : parseUnit(e.attribute("width"));
	double h = e.attribute("height").isEmpty() ? 1.0 : parseUnit(e.attribute("height"));
	setupNode(e);
	parseClipPathAttr(e, clipPath);
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, w, h, 1, m_Doc->itemToolPrefs().imageFillColor, m_Doc->itemToolPrefs().imageStrokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	if (!fname.isEmpty())
	{
		if (!fname.startsWith("data:"))
			m_Doc->loadPict(fname, ite);
		else
		{
			int startData = fname.indexOf(",");
			QString dataType = fname.left(startData);
			fname.remove(0, startData+1);
			QByteArray ba;
			ba.append(fname);
			if (dataType.contains("base64"))
				ba = QByteArray::fromBase64(ba);
			QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_svg_XXXXXX.png");
			tempFile->setAutoRemove(false);
			tempFile->open();
			QString fileName = getLongPathName(tempFile->fileName());
			tempFile->close();
			delete tempFile;
			ite->isTempFile = true;
			ite->isInlineImage = true;
			QImage img;
			img.loadFromData(ba);
			img.save(fileName, "PNG");
			m_Doc->loadPict(fileName, ite);
		}
	}
	if (!clipPath.empty())
		ite->PoLine = clipPath.copy();
	clipPath.resize(0);
	ite->PoLine.map(QTransform(1.0, 0.0, 0.0, 1.0, x, y));
	ite->Clip = flattenPath(ite->PoLine, ite->Segments);
	finishNode(e, ite);
	IElements.append(ite);
	delete (m_gc.pop());
	return IElements;
}

QList<PageItem*> SVGPlug::parseLine(const QDomElement &e)
{
	QList<PageItem*> LElements;
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	double x1 = e.attribute("x1").isEmpty() ? 0.0 : parseUnit(e.attribute("x1"));
	double y1 = e.attribute("y1").isEmpty() ? 0.0 : parseUnit(e.attribute("y1"));
	double x2 = e.attribute("x2").isEmpty() ? 0.0 : parseUnit(e.attribute("x2"));
	double y2 = e.attribute("y2").isEmpty() ? 0.0 : parseUnit(e.attribute("y2"));
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
	PageItem* ite = m_Doc->Items->at(z);
	ite->PoLine.resize(4);
	ite->PoLine.setPoint(0, FPoint(x1, y1));
	ite->PoLine.setPoint(1, FPoint(x1, y1));
	ite->PoLine.setPoint(2, FPoint(x2, y2));
	ite->PoLine.setPoint(3, FPoint(x2, y2));
	ite = finishNode(e, ite);
	LElements.append(ite);
	delete (m_gc.pop());
	return LElements;
}

QVector<double> SVGPlug::parseNumbersList(const QString& numbersStr)
{
	QVector<double> numbers;
	if (numbersStr.isEmpty())
		return numbers;
	numbers.reserve(8);

	const QChar* str = numbersStr.data();

	while (str->isSpace())
		++str;
	while (ScCLocale::isDigit(str->unicode()) ||
	       *str == QLatin1Char('-') || *str == QLatin1Char('+') ||
	       *str == QLatin1Char('.'))
	{

		numbers.append(ScCLocale::toDoubleC(str));

		while (str->isSpace())
			++str;
		if (*str == QLatin1Char(','))
			++str;

		//eat the rest of space
		while (str->isSpace())
			++str;
	}

	return numbers;
}

QList<PageItem*> SVGPlug::parsePath(const QDomElement &e)
{
	FPointArray pArray;
	QList<PageItem*> PElements;
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	PageItem::ItemType itype = pArray.parseSVG(e.attribute("d")) ? PageItem::PolyLine : PageItem::Polygon; 
	int z = m_Doc->itemAdd(itype, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
	PageItem* ite = m_Doc->Items->at(z);
	ite->fillRule = (gc->fillRule != "nonzero");
	ite->PoLine = pArray;
	if (ite->PoLine.size() < 4)
	{
		tmpSel->addItem(ite);
		m_Doc->itemSelection_DeleteItem(tmpSel);
	}
	else
	{
		ite = finishNode(e, ite);
		PElements.append(ite);
	}
	delete (m_gc.pop());
	return PElements;
}

QList<PageItem*> SVGPlug::parsePolyline(const QDomElement &e)
{
	int z;
	QList<PageItem*> PElements;
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	QString points = e.attribute("points");
	if (!points.isEmpty())
	{
		QString STag = parseTagName(e);
		points = points.simplified().replace(',', " ");
		QStringList pointList = points.split(' ', QString::SkipEmptyParts);
		if ((STag == "polygon" ) && (pointList.count() > 4))
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
		PageItem* ite = m_Doc->Items->at(z);
		ite->fillRule = (gc->fillRule != "nonzero"); 
		ite->PoLine.resize(0);
		ite->PoLine.svgInit();
		bool bFirst = true;
		double x = 0.0;
		double y = 0.0;
		for (QStringList::Iterator it = pointList.begin(); it != pointList.end(); it++)
		{
			x = ScCLocale::toDoubleC(*(it++));
			y = ScCLocale::toDoubleC(*it);
			if (bFirst)
			{
				ite->PoLine.svgMoveTo(x, y);
				bFirst = false;
			}
			else
			{
				ite->PoLine.svgLineTo(x, y);
			}
		}
		if ((STag == "polygon") && (pointList.count() > 4))
			ite->PoLine.svgClosePath();
		else
			ite->convertTo(PageItem::PolyLine);
		if (ite->PoLine.size() < 4)
		{
			tmpSel->addItem(ite);
			m_Doc->itemSelection_DeleteItem(tmpSel);
		}
		else
		{
			ite = finishNode(e, ite);
			PElements.append(ite);
		}
	}
	delete (m_gc.pop());
	return PElements;
}

QList<PageItem*> SVGPlug::parseRect(const QDomElement &e)
{
	QList<PageItem*> RElements;
	double baseX = m_Doc->currentPage()->xOffset();
	double baseY = m_Doc->currentPage()->yOffset();
	double x = parseUnit(e.attribute("x"));
	double y = parseUnit(e.attribute("y"));
	double width = parseUnit(e.attribute("width"));
	double height = parseUnit(e.attribute("height"));
	double rx = e.attribute("rx").isEmpty() ? 0.0 : parseUnit(e.attribute("rx"));
	double ry = e.attribute("ry").isEmpty() ? 0.0 : parseUnit(e.attribute("ry"));
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX, baseY, width, height, gc->LWidth, gc->FillCol, gc->StrokeCol);
	PageItem* ite = m_Doc->Items->at(z);
	if ((rx != 0) || (ry != 0))
	{
		ite->setCornerRadius(qMax(rx, ry));
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
	QTransform mm = QTransform();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	finishNode(e, ite);
	RElements.append(ite);
	delete (m_gc.pop());
	return RElements;
}

QList<PageItem*> SVGPlug::parseText(const QDomElement &e)
{
	QList<PageItem*> GElements;
	setupNode(e);
	double chunkWidth = 0;
	FPoint currentPos = parseTextPosition(e);
	SvgStyle *gc      = m_gc.top();
	if (gc->textAnchor != "start")
		getTextChunkWidth(e, chunkWidth);
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		if (n.isElement())
		{
			if (parseTagName(n.toElement()) == "tspan")
			{
				QList<PageItem*> el = parseTextSpan(n.toElement(), currentPos, chunkWidth);
				for (int ec = 0; ec < el.count(); ++ec)
					GElements.append(el.at(ec));
			}
			else if (parseTagName(n.toElement()) == "textPath")
			{
				GElements = parseText(n.toElement());
				break;
			}
		}
		if (n.isText())
		{
			QList<PageItem*> el = parseTextNode(n.toText(), currentPos, chunkWidth);
			for (int ec = 0; ec < el.count(); ++ec)
				GElements.append(el.at(ec));
		}
	}
	delete (m_gc.pop());
	return GElements;
}

QList<PageItem*> SVGPlug::parseTextSpan(const QDomElement& e, FPoint& currentPos, double chunkW)
{
	QList<PageItem*> GElements;
	setupNode(e);
	currentPos   = parseTextPosition(e, &currentPos);
	SvgStyle *gc = m_gc.top();
	if ((e.hasAttribute("x") || e.hasAttribute("y")) && (gc->textAnchor != "start"))
	{
		chunkW = 0;
		getTextChunkWidth(e, chunkW);
	}
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		if (n.isElement() && (parseTagName(n.toElement()) == "tspan"))
		{
			QList<PageItem*> el = parseTextSpan(n.toElement(), currentPos, chunkW);
			for (int ec = 0; ec < el.count(); ++ec)
				GElements.append(el.at(ec));
		}
		if (n.isText())
		{
			QList<PageItem*> el = parseTextNode(n.toText(), currentPos, chunkW);
			for (int ec = 0; ec < el.count(); ++ec)
				GElements.append(el.at(ec));
		}
	}
	delete (m_gc.pop());
	return GElements;
}

QList<PageItem*> SVGPlug::parseTextNode(const QDomText& e, FPoint& currentPos, double chunkW)
{
	QList<PageItem*> GElements;
	double baseX  = m_Doc->currentPage()->xOffset();
	double baseY  = m_Doc->currentPage()->yOffset();
	double startX = currentPos.x(), startY = currentPos.y();
	
	QString textString = e.data().simplified();
	if (textString.isEmpty())
		return GElements;
	
	SvgStyle *gc   = m_gc.top();
	QFont textFont = getFontFromStyle(*gc);
	QFontMetrics fm(textFont);
	double width   = fm.horizontalAdvance(textString);

	if (gc->textAnchor == "middle")
		startX -= chunkW / 2.0;
	else if (gc->textAnchor == "end")
		startX -= chunkW;

	FPointArray textPath;
	QString textFillColor   = gc->FillCol;
	QString textStrokeColor = gc->StrokeCol;

	// Text outline is generated using a big font size
	// and scaled afterwards. This is done in order to
	// overcome some QPainterPath issues when using
	// small font sizes, especially bad glyph advances
	QFont painterFont = textFont;
	painterFont.setPointSizeF(100.0);
	double fontScale = textFont.pointSizeF() / 100.0;

	QPainterPath painterPath;
	painterPath.addText(0.0, 0.0, painterFont, textString);

	QTransform textTrans;
	textTrans.translate(startX, startY);
	textTrans.scale(fontScale, fontScale);
	painterPath = textTrans.map(painterPath);

	textPath.fromQPainterPath(painterPath);
	if (!textPath.empty())
	{
//		double  lineWidth = 0.0;
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, textFillColor, textStrokeColor);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = textPath;
		finishNode(e, ite);
		GElements.append(ite);
	}
	currentPos.setX(currentPos.x() + width);
	return GElements;
}

QList<PageItem*> SVGPlug::parseSwitch(const QDomElement &e)
{
	QString href;
	QStringList hrefs;
	QList<PageItem*> SElements;
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement de = n.toElement();
		QString STag = parseTagName(de);
		if (STag == "foreignObject")
		{
			if (de.hasAttribute("xlink:href"))
			{
				href = de.attribute("xlink:href").mid(1);
				if (!href.isEmpty())
					hrefs.append(href);
			}
			for (QDomNode n1 = de.firstChild(); !n1.isNull(); n1 = n1.nextSibling())
			{
				QDomElement de1 = n1.toElement();
				if (de1.hasAttribute("xlink:href"))
				{
					href = de1.attribute("xlink:href").mid(1);
					if (!href.isEmpty())
						hrefs.append(href);
				}
			}
		}
		else 
		{
			if (de.hasAttribute("requiredExtensions") || de.hasAttribute("requiredFeatures"))
				continue;
			if (de.hasAttribute("id") && hrefs.contains(de.attribute("id")))
				continue;
			SElements = parseElement(de);
			if (SElements.count() > 0)
				break;
		}
	}
	return SElements;
}

QList<PageItem*> SVGPlug::parseSymbol(const QDomElement &e)
{
	QList<PageItem*> SElements;
	QString id = e.attribute("id");
	if (!id.isEmpty())
		m_nodeMap.insert(id, e);
	return SElements;
}

QList<PageItem*> SVGPlug::parseUse(const QDomElement &e)
{
	QList<PageItem*> UElements;
	setupNode(e);
	if (e.hasAttribute("x") || e.hasAttribute("y"))
	{
		QTransform matrix;
		double  xAtt = ScCLocale::toDoubleC(e.attribute("x", "0.0"));
		double  yAtt = ScCLocale::toDoubleC(e.attribute("y", "0.0"));
		SvgStyle *gc = m_gc.top();
		gc->matrix   = QTransform(1.0, 0.0, 0.0, 1.0, xAtt, yAtt) * gc->matrix;
	}
	QString href = e.attribute("xlink:href").mid(1);
	QMap<QString, QDomElement>::Iterator it = m_nodeMap.find(href);
	if (it != m_nodeMap.end())
	{
		QDomElement elem = it.value().toElement();
		if (parseTagName(elem) == "symbol")
			UElements = parseGroup(elem);
		else
			UElements = parseElement(elem);
	}
	delete (m_gc.pop());
	return UElements;
}

QFont SVGPlug::getFontFromStyle(SvgStyle& style)
{
	QFont font(QApplication::font());
	font.setStyleStrategy(QFont::PreferOutline);

	if (!style.FontFamily.isEmpty())
		font.setFamily(style.FontFamily);

	if (!style.FontStyle.isEmpty())
	{
		if (style.FontStyle == "normal")
			font.setStyle(QFont::StyleNormal);
		else if (style.FontStyle == "italic")
			font.setStyle(QFont::StyleItalic);
		else if (style.FontStyle == "oblique")
			font.setStyle(QFont::StyleOblique);
	}

	if (!style.FontWeight.isEmpty())
	{
		if (style.FontWeight == "normal")
			font.setWeight(QFont::Normal);
		else if (style.FontWeight == "bold")
			font.setWeight(QFont::Bold);
		else if (style.FontWeight == "bolder")
			font.setWeight(QFont::DemiBold);
		else if (style.FontWeight == "lighter")
			font.setWeight(QFont::Light);
		else
		{
			bool weightIsNum = false;
			int  fontWeight  = style.FontWeight.toInt(&weightIsNum);
			if (weightIsNum)
			{
				if (fontWeight == 100 || fontWeight == 200)
					font.setWeight(QFont::Light);
				else if (fontWeight == 300 || fontWeight == 400)
					font.setWeight(QFont::Normal);
				else if (fontWeight == 500 || fontWeight == 600)
					font.setWeight(QFont::DemiBold);
				else if (fontWeight == 700 || fontWeight == 800)
					font.setWeight(QFont::Bold);
				else if (fontWeight == 900)
					font.setWeight(QFont::Black);
			}
		}
	}

	if (!style.FontStretch.isEmpty())
	{
		if (style.FontStretch == "normal")
			font.setStretch(QFont::Unstretched);
		else if (style.FontStretch == "ultra-condensed")
			font.setStretch(QFont::UltraCondensed);
		else if (style.FontStretch == "extra-condensed")
			font.setStretch(QFont::ExtraCondensed);
		else if (style.FontStretch == "condensed")
			font.setStretch(QFont::Condensed);
		else if (style.FontStretch == "semi-condensed")
			font.setStretch(QFont::SemiCondensed);
		else if (style.FontStretch == "semi-expanded")
			font.setStretch(QFont::SemiExpanded);
		else if (style.FontStretch == "expanded")
			font.setStretch(QFont::Expanded);
		else if (style.FontStretch == "extra-expanded")
			font.setStretch(QFont::ExtraExpanded);
		else if (style.FontStretch == "ultra-expanded")
			font.setStretch(QFont::UltraExpanded);
		else if (style.FontStretch == "narrower")
			font.setStretch(QFont::SemiCondensed);
		else if (style.FontStretch == "wider")
			font.setStretch(QFont::SemiExpanded);
	}
	if (!style.textDecoration.isEmpty())
	{
		bool underline = false, overline  = false;
		bool strikeOut = false;
		if (style.textDecoration == "underline")
			underline = true;
		else if (style.textDecoration == "overline")
			overline = true;
		else if (style.textDecoration == "line-through")
			strikeOut = true;
		font.setUnderline(underline);
		font.setOverline(overline);
		font.setStrikeOut(strikeOut);
	}
	font.setPointSizeF(style.FontSize);
	return font;
}

QDomElement SVGPlug::getReferencedNode(const QDomElement &e)
{
	QDomElement ret;
	QMap<QString, QDomElement>::Iterator it;
	QString href = e.attribute("xlink:href").mid(1);
	it = m_nodeMap.find(href);
	if (it != m_nodeMap.end())
		ret = it.value().toElement();
	return ret;
}

bool SVGPlug::getTextChunkWidth(const QDomElement &e, double& width)
{
	bool doBreak = false;
	setupNode(e);
	QDomNode c = e.firstChild();
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		if (n.isElement() && (parseTagName(n.toElement()) == "tspan"))
		{
			QDomElement elem = n.toElement();
			if (elem.hasAttribute("x") || elem.hasAttribute("y"))
			{
				doBreak = true;
				break;
			}
			doBreak = getTextChunkWidth(n.toElement(), width);
			if (doBreak) break;
		}
		if (n.isText())
		{
			QDomText text = n.toText();
			QString  textString = text.data().simplified();
			if (textString.length() > 0)
			{
				SvgStyle *gc   = m_gc.top();
				QFont textFont = getFontFromStyle(*gc);

				// This is to match the scaling done in
				// parseTextNode()
				double fontSize = textFont.pointSizeF();
				textFont.setPointSizeF(100.0);

				QFontMetrics fm(textFont);
				width += fm.horizontalAdvance(textString) * (fontSize / 100.0);
			}
		}
	}
	delete (m_gc.pop());
	return doBreak;
}

double SVGPlug::fromPercentage(const QString &s)
{
	QString s1 = s;
	if (s1.endsWith(";"))
		s1.chop(1);
	if (s1.endsWith("%"))
	{
		s1.chop(1);
		return ScCLocale::toDoubleC(s1) / 100.0;
	}
	return ScCLocale::toDoubleC(s1);
}

double SVGPlug::parseFontSize(const QString& fsize)
{
	bool noUnit  = true;
	QString unit = fsize.right(2);
	if (unit == "pt" || unit == "cm" || unit == "mm" || 
		unit == "in" || unit == "px")
	{
		noUnit = false;
	}
	double value = parseUnit(fsize);
	if (noUnit)
		value *= 0.8;
	return value;
}

double SVGPlug::parseUnit(const QString &unit)
{
	bool noUnit = false;
	QString unitval=unit;
	if (unit.right(2) == "pt")
		unitval.replace("pt", "");
	else if (unit.right(2) == "cm")
		unitval.replace("cm", "" );
	else if (unit.right(2) == "mm")
		unitval.replace("mm" , "");
	else if (unit.right(2) == "in")
		unitval.replace("in", "" );
	else if (unit.right(2) == "px")
		unitval.replace("px", "" );
	if (unitval == unit)
		noUnit = true;
	double value = ScCLocale::toDoubleC(unitval);
	if (unit.right(2) == "pt")
		{}/* value = value; */ //no change
	else if (unit.right(2) == "cm")
		value = (value / 2.54) * 72;
	else if (unit.right(2) == "mm")
		value = (value / 25.4) * 72;
	else if (unit.right(2) == "in")
		value = value * 72;
	else if (unit.right(2) == "px")
		value = value * 0.8;
	else if (noUnit)
		{}/* value = value; */ //no change
	return value;
}

QTransform SVGPlug::parseTransform(const QString &transform)
{
	QTransform ret;
	// Workaround for QString::split() bug when string ends with space
	QString trans = transform.simplified();
	// Split string for handling 1 transform statement at a time
	QStringList subtransforms = trans.split(')', QString::SkipEmptyParts);
	QStringList::ConstIterator it = subtransforms.begin();
	QStringList::ConstIterator end = subtransforms.end();
	for (; it != end; ++it)
	{
		QTransform result;
		QStringList subtransform = it->split('(', QString::SkipEmptyParts);
		subtransform[0] = subtransform[0].trimmed().toLower();
		subtransform[1] = subtransform[1].simplified();
		QVector<double> params = parseNumbersList(subtransform[1]);
		if (subtransform[0].startsWith(";") || subtransform[0].startsWith(","))
			subtransform[0] = subtransform[0].right(subtransform[0].length() - 1);
		if (subtransform[0] == "rotate")
		{
			if (params.count() == 3)
			{
				double x = params[1];
				double y = params[2];
				result.translate(x, y);
				result.rotate(params[0]);
				result.translate(-x, -y);
			}
			else
				result.rotate(params[0]);
		}
		else if (subtransform[0] == "translate")
		{
			if (params.count() == 2)
				result.translate(params[0], params[1]);
			else    // Spec : if only one param given, assume 2nd param to be 0
				result.translate(params[0], 0);
		}
		else if (subtransform[0] == "scale")
		{
			if (params.count() == 2)
				result.scale(params[0], params[1]);
			else    // Spec : if only one param given, assume uniform scaling
				result.scale(params[0], params[0]);
		}
		else if (subtransform[0] == "skewx")
			result.shear(tan(params[0] * 0.01745329251994329576), 0.0F);
		else if (subtransform[0] == "skewy")
			result.shear(0.0F, tan(params[0] * 0.01745329251994329576));
		else if (subtransform[0] == "matrix")
		{
			if (params.count() >= 6)
			{
				double sx = params[0];
				double sy = params[3];
				double p1 = params[1];
				double p2 = params[2];
				double p4 = params[4];
				double p5 = params[5];
				result = QTransform(sx, p1, p2, sy, p4, p5);
			}
		}
		ret = result * ret;
	}
	return ret;
}

const char * SVGPlug::getCoord(const char *ptr, double &number)
{
	int integer, exponent;
	double decimal, frac;
	int sign, expsign;

	exponent = 0;
	integer = 0;
	frac = 1.0;
	decimal = 0;
	sign = 1;
	expsign = 1;

	// read the sign
	if (*ptr == '+')
		ptr++;
	else if (*ptr == '-')
	{
		ptr++;
		sign = -1;
	}

	// read the integer part
	while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		integer = (integer * 10) + *(ptr++) - '0';
	if (*ptr == '.') // read the decimals
	{
		ptr++;
		while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
			decimal += (*(ptr++) - '0') * (frac *= 0.1);
	}

	if (*ptr == 'e' || *ptr == 'E') // read the exponent part
	{
		ptr++;

		// read the sign of the exponent
		if (*ptr == '+')
			ptr++;
		else if (*ptr == '-')
		{
			ptr++;
			expsign = -1;
		}

		exponent = 0;
		while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		{
			exponent *= 10;
			exponent += *ptr - '0';
			ptr++;
		}
	}
	number = integer + decimal;
	number *= sign * pow(static_cast<double>(10), static_cast<double>(expsign * exponent));
	// skip the following space
	if (*ptr == ' ')
		ptr++;

	return ptr;
}

QString SVGPlug::parseColor(const QString &s)
{
	QColor c;
	QString ret = CommonStrings::None;
	if (s.length() > 11) // icc-color()
	{
		int iccColorIndex  = s.indexOf("icc-color");
		if (iccColorIndex >= 0)
		{
			QString iccColorName = parseIccColor(s);
			if (iccColorName.length() > 0)
				return iccColorName;
		}
	}
	if (s.startsWith( "rgb(" ) )
	{
		QString parse = s.trimmed();
		QStringList colors = parse.split(',', QString::SkipEmptyParts);
		QString r = colors[0].right(colors[0].length() - 4);
		QString g = colors[1];
		QString b = colors[2].left(colors[2].length() - 1);
		if (r.contains("%"))
		{
			r.chop(1);
			r = QString::number( static_cast<int>((255.0 * ScCLocale::toDoubleC(r)) / 100.0) );
		}
		if (g.contains("%"))
		{
			g.chop(1);
			g = QString::number( static_cast<int>((255.0 * ScCLocale::toDoubleC(g)) / 100.0) );
		}
		if (b.contains("%"))
		{
			b.chop(1);
			b = QString::number( static_cast<int>((255.0 * ScCLocale::toDoubleC(b)) / 100.0) );
		}
		c = QColor(r.toInt(), g.toInt(), b.toInt());
	}
	else
		c.setNamedColor(s.trimmed());

	ScColor tmp;
	tmp.fromQColor(c);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString newColorName = "FromSVG"+c.name();
	QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
	if (fNam == newColorName)
		importedColors.append(newColorName);
	ret = fNam;
	return ret;
}

QString SVGPlug::parseIccColor(const QString &s)
{
	QColor color;
	QString ret;
	bool iccColorFound = false;
	int iccColorIndex  = s.indexOf("icc-color");
	if (iccColorIndex < 0)
		return ret;
	int iccFirst = s.indexOf("(", iccColorIndex);
	int iccLast  = s.indexOf(")", iccColorIndex);
	if (iccFirst >= 0 && iccLast >= 0)
	{
		QString iccColor = s.mid(iccFirst + 1, iccLast - iccFirst - 1);
		iccColor = iccColor.trimmed();
		QStringList colors = iccColor.split(',', QString::SkipEmptyParts);
		if (colors.count() == 5) // then we assume this is a cmyk color
		{
			QString cs = colors[1], ms = colors[2], ys = colors[3], ks = colors[4];
			if (cs.contains("%"))
			{
				cs.chop(1);
				cs = QString::number(ScCLocale::toDoubleC(cs) / 100);
			}
			if (ms.contains("%"))
			{
				ms.chop(1);
				ms = QString::number(ScCLocale::toDoubleC(ms) / 100);
			}
			if (ys.contains("%"))
			{
				ys.chop(1);
				ys = QString::number(ScCLocale::toDoubleC(ys) / 100);
			}
			if (ks.contains("%"))
			{
				ks.chop(1);
				ks = QString::number(ScCLocale::toDoubleC(ks) / 100);
			}
			double cv = ScCLocale::toDoubleC(cs);
			double mv = ScCLocale::toDoubleC(ms);
			double yv = ScCLocale::toDoubleC(ys);
			double kv = ScCLocale::toDoubleC(ks);
			color.setCmykF(cv, mv, yv, kv);
			iccColorFound = true;
		}
	}
	if (!iccColorFound)
		return ret;
	ScColor tmp;
	tmp.fromQColor(color);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString newColorName = "FromSVG"+tmp.name();
	QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
	if (fNam == newColorName)
		importedColors.append(newColorName);
	ret = fNam;
	return ret;
}

void SVGPlug::parsePA(SvgStyle *obj, const QString &command, const QString &params)
{
	if (command == "display")
		obj->Display = params != "none";
	else if (command == "stroke-opacity")
		obj->StrokeOpacity  = fromPercentage(params);
	else if (command == "fill-opacity")
		obj->FillOpacity = fromPercentage(params);
	else if (command == "opacity")
		obj->Opacity = fromPercentage(params);
	else if (command == "fill")
	{
//		if ((obj->InherCol) && (params == "currentColor"))
		if (params == "currentColor")
			obj->FillCol = obj->CurCol;
		else if (params == "none")
		{
			obj->FillCol = CommonStrings::None;
			obj->FillGradientType = 0;
		}
		else if (params.startsWith( "url(" ))
		{
			unsigned int start = params.indexOf("#") + 1;
			unsigned int end = params.lastIndexOf(")");
			QString key = params.mid(start, end - start);
			obj->FillGradientType = 0;
			obj->matrixgf = QTransform();
			bool firstMatrixValid = false;
			if (m_gradients[key].matrixValid)
			{
				firstMatrixValid = true;
				obj->matrixgf = m_gradients[key].matrix;
			}
			while (!m_gradients[key].reference.isEmpty())
			{
				QString key2 = m_gradients[key].reference;
				const GradientHelper& gradientHelper(m_gradients[key2]);
				if (gradientHelper.typeValid)
					obj->FillGradientType = gradientHelper.type;
				if (obj->FillGradientType != 8)
				{
					obj->GFillCol1 = key2;
					if (gradientHelper.gradientValid)
						obj->FillGradient = gradientHelper.gradient;
					if (gradientHelper.cspaceValid)
						obj->FillCSpace = gradientHelper.cspace;
					if (gradientHelper.x1Valid)
						obj->GradFillX1 = gradientHelper.x1;
					if (gradientHelper.y1Valid)
						obj->GradFillY1 = gradientHelper.y1;
					if (gradientHelper.x2Valid)
						obj->GradFillX2 = gradientHelper.x2;
					if (gradientHelper.y2Valid)
						obj->GradFillY2 = gradientHelper.y2;
					if (gradientHelper.fxValid)
						obj->GradFillFX = gradientHelper.fx;
					if (gradientHelper.fyValid)
						obj->GradFillFY = gradientHelper.fy;
					if (gradientHelper.matrixValid)
						obj->matrixgf = gradientHelper.matrix;
				}
				else
				{
					obj->GFillCol1 = key2;
					if ((m_gradients[key2].matrixValid) && (!firstMatrixValid))
						obj->matrixgf *= m_gradients[key2].matrix;
				}
				key = m_gradients[key].reference;
			}
			if (obj->FillGradientType != 8)
			{
				key = params.mid(start, end - start);
				const GradientHelper& gradientHelper(m_gradients[key]);
				if (gradientHelper.typeValid)
					obj->FillGradientType = m_gradients[key].type;
				key = m_gradients[key].reference;
				if (obj->FillGradientType != 8)
				{
					if (gradientHelper.gradientValid)
						obj->FillGradient = gradientHelper.gradient;
					if (gradientHelper.cspaceValid)
						obj->FillCSpace = gradientHelper.cspace;
					if (gradientHelper.x1Valid)
						obj->GradFillX1 = gradientHelper.x1;
					if (gradientHelper.y1Valid)
						obj->GradFillY1 = gradientHelper.y1;
					if (gradientHelper.x2Valid)
						obj->GradFillX2 = gradientHelper.x2;
					if (gradientHelper.y2Valid)
						obj->GradFillY2 = gradientHelper.y2;
					if (gradientHelper.fxValid)
						obj->GradFillFX = gradientHelper.fx;
					if (gradientHelper.fyValid)
						obj->GradFillFY = gradientHelper.fy;
					if (gradientHelper.matrixValid)
						obj->matrixgf = gradientHelper.matrix;
					obj->GFillCol1 = key;
				}
				else
				{
					obj->GFillCol1 = key;
					if (m_gradients[key].matrixValid)
						obj->matrixgf = m_gradients[key].matrix;
				}
			}
			obj->FillCol = CommonStrings::None;
		}
		else
		{
			obj->FillCol = parseColor(params);
			obj->FillGradientType = 0;
		}
	}
	else if (command == "fill-rule")
	{
		obj->fillRule = params;
	}
	else if (command == "color")
	{
		if (params == "none")
			obj->CurCol = CommonStrings::None;
		else if (params.startsWith( "url(" ) )
			obj->CurCol = CommonStrings::None;
		else if (params == "currentColor")
			obj->CurCol = obj->CurCol;
		else
			obj->CurCol = parseColor(params);
	}
	else if (command == "stroke")
	{
//		if ((obj->InherCol) && (params == "currentColor"))
		if (params == "currentColor")
			obj->StrokeCol = obj->CurCol;
		else if (params == "none")
		{
			obj->StrokeCol = CommonStrings::None;
			obj->StrokeGradientType = 0;
		}
		else if (params.startsWith( "url(" ) )
		{
			unsigned int start = params.indexOf("#") + 1;
			unsigned int end = params.lastIndexOf(")");
			QString key = params.mid(start, end - start);
			obj->StrokeGradientType = 0;
			obj->matrixgs = QTransform();
			bool firstMatrixValid = false;
			if (m_gradients[key].matrixValid)
			{
				firstMatrixValid = true;
				obj->matrixgs = m_gradients[key].matrix;
			}
			while (!m_gradients[key].reference.isEmpty())
			{
				QString key2 = m_gradients[key].reference;
				const GradientHelper& gradientHelper(m_gradients[key2]);
				if (gradientHelper.typeValid)
					obj->StrokeGradientType = gradientHelper.type;
				if (obj->StrokeGradientType != 8)
				{
					obj->GStrokeCol1 = key2;
					if (gradientHelper.gradientValid)
						obj->StrokeGradient = gradientHelper.gradient;
					if (gradientHelper.cspaceValid)
						obj->StrokeCSpace = gradientHelper.cspace;
					if (gradientHelper.x1Valid)
						obj->GradStrokeX1 = gradientHelper.x1;
					if (gradientHelper.y1Valid)
						obj->GradStrokeY1 = gradientHelper.y1;
					if (gradientHelper.x2Valid)
						obj->GradStrokeX2 = gradientHelper.x2;
					if (gradientHelper.y2Valid)
						obj->GradStrokeY2 = gradientHelper.y2;
					if (gradientHelper.fxValid)
						obj->GradStrokeFX = gradientHelper.fx;
					if (gradientHelper.fyValid)
						obj->GradStrokeFY = gradientHelper.fy;
					if (gradientHelper.matrixValid)
						obj->matrixgs = gradientHelper.matrix;
				}
				else
				{
					obj->GStrokeCol1 = key2;
					if ((m_gradients[key2].matrixValid) && (!firstMatrixValid))
						obj->matrixgs *= m_gradients[key2].matrix;
				}
				key = m_gradients[key].reference;
			}
			if (obj->StrokeGradientType != 8)
			{
				key = params.mid(start, end - start);
				const GradientHelper& gradientHelper(m_gradients[key]);
				if (gradientHelper.typeValid)
					obj->StrokeGradientType = gradientHelper.type;
				key = m_gradients[key].reference;
				if (obj->StrokeGradientType != 8)
				{
					if (gradientHelper.gradientValid)
						obj->StrokeGradient = gradientHelper.gradient;
					if (gradientHelper.cspaceValid)
						obj->StrokeCSpace = gradientHelper.cspace;
					if (gradientHelper.x1Valid)
						obj->GradStrokeX1 = gradientHelper.x1;
					if (gradientHelper.y1Valid)
						obj->GradStrokeY1 = gradientHelper.y1;
					if (gradientHelper.x2Valid)
						obj->GradStrokeX2 = gradientHelper.x2;
					if (gradientHelper.y2Valid)
						obj->GradStrokeY2 = gradientHelper.y2;
					if (gradientHelper.fxValid)
						obj->GradStrokeFX = gradientHelper.fx;
					if (gradientHelper.fyValid)
						obj->GradStrokeFY = gradientHelper.fy;
					if (gradientHelper.matrixValid)
						obj->matrixgs = gradientHelper.matrix;
					obj->GStrokeCol1 = key;
				}
				else
				{
					obj->GStrokeCol1 = key;
					if (m_gradients[key].matrixValid)
						obj->matrixgs = m_gradients[key].matrix;
				}
			}
			obj->StrokeCol = CommonStrings::None;
		}
		else
		{
			obj->StrokeCol = parseColor(params);
			obj->StrokeGradientType = 0;
		}
	}
	else if (command == "stroke-width")
		obj->LWidth = parseUnit(params);
	else if (command == "stroke-linejoin")
	{
		if (params == "miter")
			obj->PLineJoin = Qt::MiterJoin;
		else if (params == "round")
			obj->PLineJoin = Qt::RoundJoin;
		else if (params == "bevel")
			obj->PLineJoin = Qt::BevelJoin;
	}
	else if (command == "stroke-linecap")
	{
		if (params == "butt")
			obj->PLineEnd = Qt::FlatCap;
		else if (params == "round")
			obj->PLineEnd = Qt::RoundCap;
		else if (params == "square")
			obj->PLineEnd = Qt::SquareCap;
	}
	//	else if (command == "stroke-miterlimit" )
	//		gc->stroke.setMiterLimit(params.todouble());
	else if (command == "stroke-dasharray")
	{
		QVector<double> array;
		if (params != "none")
		{
			QString params2 = params.simplified().replace(',', " ");
			QStringList dashes = params2.split(' ', QString::SkipEmptyParts);
			if ((dashes.count() > 0) && (parseUnit(dashes[0]) != 0.0))
			{
				for (QStringList::Iterator it = dashes.begin(); it != dashes.end(); ++it)
					array.append(parseUnit(*it));
			}
		}
		obj->dashArray = array;
	}
	else if (command == "stroke-dashoffset")
		obj->dashOffset = ScCLocale::toDoubleC(params);
	else if (command == "font-family")
		obj->FontFamily = params;
	else if (command == "font-style")
		obj->FontStyle = params;
	else if (command == "font-weight")
		obj->FontWeight = params;
	else if (command == "font-stretch")
		obj->FontStretch = params;
	else if (command == "font-size")
		obj->FontSize = parseFontSize(params);
	else if (command == "text-anchor")
		obj->textAnchor = params;
	else if (command == "text-decoration")
		obj->textDecoration = params;
	else if (command == "clip-path")
	{
		if (params.startsWith( "url("))
		{
			unsigned int start = params.indexOf("#") + 1;
			unsigned int end = params.lastIndexOf(")");
			QString key = params.mid(start, end - start);
			QMap<QString, FPointArray>::iterator it = m_clipPaths.find(key);
			if (it != m_clipPaths.end())
				obj->clipPath = it.value().copy();
		}
	}
	else if (command == "filter")
	{
		if (params.startsWith( "url("))
		{
			unsigned int start = params.indexOf("#") + 1;
			unsigned int end = params.lastIndexOf(")");
			obj->filter = params.mid(start, end - start);
		}
	}
	else if (command == "marker-end")
	{
		if (params.startsWith( "url("))
		{
			unsigned int start = params.indexOf("#") + 1;
			unsigned int end = params.lastIndexOf(")");
			obj->endMarker = params.mid(start, end - start);
		}
	}
	else if (command == "marker-start")
	{
		if (params.startsWith( "url("))
		{
			unsigned int start = params.indexOf("#") + 1;
			unsigned int end = params.lastIndexOf(")");
			obj->startMarker = params.mid(start, end - start);
		}
	}
	else if (!isIgnorableNodeName(command))
	{
		if (!m_unsupportedFeatures.contains(command))
		{
			m_unsupportedFeatures.insert(command, command);
			qDebug() << QString("unsupported SVG feature: %1").arg(command);
			unsupported = true;
		}
	}
}

void SVGPlug::parseStyle(SvgStyle *obj, const QDomElement &e)
{
	SvgStyle *gc = m_gc.top();
	if (!gc)
		return;
	if (!e.attribute("display").isEmpty())
		parsePA(obj, "display", e.attribute("display"));
	if (!e.attribute("color").isEmpty())
	{
		if (e.attribute("color") == "inherit")
			gc->InherCol = true;
		else
			parsePA(obj, "color", e.attribute("color"));
	}
	if (!e.attribute("fill").isEmpty())
		parsePA(obj, "fill", e.attribute("fill"));
	if (!e.attribute("stroke").isEmpty())
		parsePA(obj, "stroke", e.attribute("stroke"));
	if (!e.attribute("stroke-width").isEmpty())
		parsePA(obj, "stroke-width", e.attribute("stroke-width"));
	if (!e.attribute("stroke-linejoin").isEmpty())
		parsePA(obj, "stroke-linejoin", e.attribute("stroke-linejoin"));
	if (!e.attribute("stroke-linecap").isEmpty())
		parsePA(obj, "stroke-linecap", e.attribute("stroke-linecap"));
	if (!e.attribute("stroke-dasharray").isEmpty())
		parsePA(obj, "stroke-dasharray", e.attribute("stroke-dasharray"));
	if (!e.attribute("stroke-dashoffset").isEmpty())
		parsePA(obj, "stroke-dashoffset", e.attribute("stroke-dashoffset"));
	if (!e.attribute("stroke-opacity").isEmpty())
		parsePA(obj, "stroke-opacity", e.attribute("stroke-opacity"));
	/*	if (!e.attribute("stroke-miterlimit").isEmpty())
			parsePA(obj, "stroke-miterlimit", e.attribute("stroke-miterlimit"));   */
	if (!e.attribute("fill-rule").isEmpty())
		parsePA(obj, "fill-rule", e.attribute("fill-rule"));
	if (!e.attribute("fill-opacity").isEmpty())
		parsePA(obj, "fill-opacity", e.attribute("fill-opacity"));
	if (!e.attribute("opacity").isEmpty())
		parsePA(obj, "opacity", e.attribute("opacity"));
	if (!e.attribute("font-family").isEmpty())
		parsePA(obj, "font-family", e.attribute("font-family"));
	if (!e.attribute("font-style").isEmpty())
		parsePA(obj, "font-style", e.attribute("font-style"));
	if (!e.attribute("font-weight").isEmpty())
		parsePA(obj, "font-weight", e.attribute("font-weight"));
	if (!e.attribute("font-stretch").isEmpty())
		parsePA(obj, "font-stretch", e.attribute("font-stretch"));
	if (!e.attribute("font-size").isEmpty())
		parsePA(obj, "font-size", e.attribute("font-size"));
	if (!e.attribute("text-anchor").isEmpty())
		parsePA(obj, "text-anchor", e.attribute("text-anchor"));
	if (!e.attribute("text-decoration").isEmpty())
		parsePA(obj, "text-decoration", e.attribute("text-decoration"));
	if (!e.attribute("filter").isEmpty())
		parsePA(obj, "filter", e.attribute("filter"));
	if (!e.attribute("marker-end").isEmpty())
		parsePA(obj, "marker-end", e.attribute("marker-end"));
	if (!e.attribute("marker-start").isEmpty())
		parsePA(obj, "marker-start", e.attribute("marker-start"));
	QString style = e.attribute("style").simplified();
	QStringList substyles = style.split(';', QString::SkipEmptyParts);
	for (QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it)
	{
		QStringList substyle = it->split(':', QString::SkipEmptyParts);
		if (substyle.count() >= 2)
		{
			QString command(substyle.at(0).trimmed());
			QString params(substyle.at(1).trimmed());
			parsePA(obj, command, params);
		}
	}
}

void SVGPlug::parseColorStops(GradientHelper *gradient, const QDomElement &e)
{
	QString Col = "Black";
	double offset = 0;
	double opa;
	SvgStyle svgStyle;
	parseStyle(&svgStyle, e);
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		opa = 1.0;
		QDomElement stop = n.toElement();
		if (parseTagName(stop) == "stop")
		{
			QString temp = stop.attribute("offset");
			if (temp.contains('%'))
			{
				temp.chop(1);
				offset = ScCLocale::toDoubleC(temp) / 100.0;
			}
			else
				offset = ScCLocale::toDoubleC(temp);
			if (stop.hasAttribute("stop-opacity"))
				opa = fromPercentage(stop.attribute("stop-opacity"));
			if (stop.hasAttribute("stop-color"))
			{
				if (stop.attribute("stop-color") == "currentColor")
					Col = svgStyle.CurCol;
				else
					Col = parseColor(stop.attribute("stop-color"));
			}
			else if (stop.hasAttribute("style"))
			{
				QString style = stop.attribute("style").simplified();
				QStringList substyles = style.split(';', QString::SkipEmptyParts);
				for (QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it)
				{
					QStringList substyle = it->split(':', QString::SkipEmptyParts);
					if (substyle.count() >= 2)
					{
						QString command(substyle.at(0).trimmed());
						QString params(substyle.at(1).trimmed());
						if (command == "stop-color")
							Col = parseColor(params);
						if (command == "stop-opacity")
							opa = fromPercentage(params);
					}
				}
			}
			else
				Col = "Black";
		}
		const ScColor& gradC = m_Doc->PageColors[Col];
		gradient->gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), offset, 0.5, opa, Col, 100);
		gradient->gradientValid = true;
	}
	if (gradient->gradientValid)
		gradient->gradient.filterStops();
}

void SVGPlug::parseFilter(const QDomElement &b)
{
	QString id = b.attribute("id", "");
	QString origName = id;
	if (id.isEmpty())
		return;

	filterSpec fspec;
	fspec.blendMode = 0;

	QDomElement child = b.firstChildElement();
	if (child.isNull() || (child.tagName() != "feBlend"))
	{
		filters.insert(id, fspec);
		m_nodeMap.insert(origName, b);
		return;
	}

	QString blendModeStr(child.attribute("mode"));
	if (blendModeStr == "normal")
		fspec.blendMode = 0;
	if (blendModeStr == "darken")
		fspec.blendMode = 1;
	if (blendModeStr == "lighten")
		fspec.blendMode = 2;
	if (blendModeStr == "multiply")
		fspec.blendMode = 3;
	if (blendModeStr == "screen")
		fspec.blendMode = 4;

	filters.insert(id, fspec);
	m_nodeMap.insert(origName, b);
}

void SVGPlug::parseMarker(const QDomElement &b)
{
	QString id = b.attribute("id", "");
	QString origName = id;
	if (!id.isEmpty())
	{
		inGroupXOrigin = 999999;
		inGroupYOrigin = 999999;
		markerDesc mark;
		mark.xref = parseUnit(b.attribute("refX", "0"));
		mark.yref = parseUnit(b.attribute("refY", "0"));
		mark.wpat = parseUnit(b.attribute("markerWidth", "3"));
		mark.hpat = parseUnit(b.attribute("markerHeight", "3"));
		QList<PageItem*> GElements;
		GElements = parseGroup(b);
		if (GElements.count() > 0)
		{
			ScPattern pat = ScPattern();
			pat.setDoc(m_Doc);
			PageItem* currItem = GElements.at(0);
			m_Doc->DoDrawing = true;
			double minx =  std::numeric_limits<double>::max();
			double miny =  std::numeric_limits<double>::max();
			double maxx = -std::numeric_limits<double>::max();
			double maxy = -std::numeric_limits<double>::max();
			double x1, x2, y1, y2;
			currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
			minx = qMin(minx, x1);
			miny = qMin(miny, y1);
			maxx = qMax(maxx, x2);
			maxy = qMax(maxy, y2);
			currItem->gXpos = currItem->xPos() - minx;
			currItem->gYpos = currItem->yPos() - miny;
			currItem->setXYPos(currItem->gXpos, currItem->gYpos, true);
			pat.pattern = currItem->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
			pat.width = maxx - minx;
			pat.height = maxy - miny;
			m_Doc->DoDrawing = false;
			pat.items.append(currItem);
			m_Doc->Items->removeAll(currItem);
			m_Doc->addPattern(id, pat);
			importedPatterns.append(id);
			importedPattTrans.insert(origName, id);
			markers.insert(id, mark);
		}
		m_nodeMap.insert(origName, b);
	}
}

void SVGPlug::parsePattern(const QDomElement &b)
{
	GradientHelper gradhelper;
	QString href = b.attribute("xlink:href").mid(1);
	if (!href.isEmpty())
	{
		if (m_gradients.contains(href))
		{
			gradhelper.type = m_gradients[href].type;
			gradhelper.gradientValid = m_gradients[href].gradientValid;
			gradhelper.typeValid = m_gradients[href].typeValid;
			gradhelper.matrix = m_gradients[href].matrix;
			gradhelper.matrixValid = m_gradients[href].matrixValid;
		}
		gradhelper.reference = href;
	}
	QString id = b.attribute("id", "");
	QString origName = id;
	if (!id.isEmpty())
	{
		inGroupXOrigin = 999999;
		inGroupYOrigin = 999999;
		double wpat = parseUnit(b.attribute("width", "0"));
		double hpat = parseUnit(b.attribute("height", "0"));
		QList<PageItem*> GElements;
		GElements = parseGroup(b);
		if (GElements.count() > 0)
		{
			ScPattern pat = ScPattern();
			pat.setDoc(m_Doc);
			PageItem* currItem = GElements.at(0);
			m_Doc->DoDrawing = true;
			pat.pattern = currItem->DrawObj_toImage(qMin(qMax(wpat, hpat), 500.0));
			double xOrg = 0.0;
			double yOrg = 0.0;
			if (inGroupXOrigin < 0.0)
				xOrg = inGroupXOrigin;
			if (inGroupYOrigin < 0.0)
				yOrg = inGroupYOrigin;
			if ((xOrg != 0.0) || (yOrg != 0.0))
				pat.pattern = pat.pattern.copy(-xOrg, -yOrg, wpat, hpat);
			pat.xoffset = xOrg;
			pat.yoffset = yOrg;
			pat.width = wpat;
			pat.height = hpat;
			m_Doc->DoDrawing = false;
			pat.items.append(currItem);
			m_Doc->Items->removeAll(currItem);
			m_Doc->addPattern(id, pat);
			importedPatterns.append(id);
			importedPattTrans.insert(origName, id);
		}
		m_nodeMap.insert(origName, b);
		QString transf = b.attribute("patternTransform");
		if (!transf.isEmpty())
		{
			gradhelper.matrix = parseTransform(b.attribute("patternTransform"));
			gradhelper.matrixValid = true;
		}
		else
			gradhelper.matrixValid = false;
		gradhelper.gradientValid = true;
		gradhelper.gradient.clearStops();
		gradhelper.gradient.setRepeatMethod(VGradient::none);
		gradhelper.type = 8;
		gradhelper.typeValid = true;
		m_gradients.insert(origName, gradhelper);
	}
}

void SVGPlug::parseGradient(const QDomElement &e)
{
	GradientHelper gradhelper;
	gradhelper.gradientValid = false;
	gradhelper.gradient.clearStops();
	gradhelper.gradient.setRepeatMethod(VGradient::none);

	QString href = e.attribute("xlink:href").mid(1);
//	double x1=0, y1=0, x2=0, y2=0, fx=0, fy=0;
	double x1=0, y1=0, x2=0, fx=0, fy=0;
	if (!href.isEmpty())
	{
		if (m_gradients.contains(href))
		{
			gradhelper.type = m_gradients[href].type;
			gradhelper.gradient = m_gradients[href].gradient;
			gradhelper.x1 = m_gradients[href].x1;
			gradhelper.y1 = m_gradients[href].y1;
			gradhelper.x2 = m_gradients[href].x2;
			gradhelper.y2 = m_gradients[href].y2;
			gradhelper.fx = m_gradients[href].fx;
			gradhelper.fy = m_gradients[href].fy;
			gradhelper.cspace = m_gradients[href].cspace;
			gradhelper.matrix = m_gradients[href].matrix;
			gradhelper.x1Valid = m_gradients[href].x1Valid;
			gradhelper.x2Valid = m_gradients[href].x2Valid;
			gradhelper.y1Valid = m_gradients[href].y1Valid;
			gradhelper.y2Valid = m_gradients[href].y2Valid;
			gradhelper.fxValid = m_gradients[href].fxValid;
			gradhelper.fyValid = m_gradients[href].fyValid;
			gradhelper.cspaceValid = m_gradients[href].cspaceValid;
			gradhelper.matrixValid = m_gradients[href].matrixValid;
			gradhelper.gradientValid = m_gradients[href].gradientValid;
			gradhelper.typeValid = m_gradients[href].typeValid;
		}
		gradhelper.reference = href;
	}
	QString id = e.attribute("id", "");
	QString origName = id;
	if (parseTagName(e) == "linearGradient")
	{
		if (e.hasAttribute("x1"))
		{
			gradhelper.x1 = parseUnit(e.attribute("x1", "0"));
			gradhelper.x1Valid = true;
		}
		if (e.hasAttribute("y1"))
		{
			gradhelper.y1 = parseUnit(e.attribute("y1", "0"));
			gradhelper.y1Valid = true;
		}
		if (e.hasAttribute("x2"))
		{
			gradhelper.x2 = parseUnit(e.attribute("x2", "1"));
			gradhelper.x2Valid = true;
		}
		if (e.hasAttribute("y2"))
		{
			gradhelper.y2 = parseUnit(e.attribute("y2", "0"));
			gradhelper.y2Valid = true;
		}
		gradhelper.fx = x1;
		gradhelper.fy = y1;
		gradhelper.type = 6;
		gradhelper.typeValid = true;
	}
	else
	{
		if (e.hasAttribute("cx"))
		{
			x1 = parseUnit(e.attribute("cx", "0.5"));
			gradhelper.x1Valid = true;
		}
		if (e.hasAttribute("cy"))
		{
			y1 = parseUnit(e.attribute("cy", "0.5"));
			gradhelper.y1Valid = true;
		}
		if (e.hasAttribute("r"))
		{
			x2 = parseUnit(e.attribute("r", "0.5"));
			gradhelper.x2Valid = true;
		}
	//	y2 = y1;
		fx = e.hasAttribute("fx") ? parseUnit(e.attribute("fx", ScCLocale::toQStringC(x1))) : x1;
		gradhelper.fxValid = true;
		fy = e.hasAttribute("fy") ? parseUnit(e.attribute("fy", ScCLocale::toQStringC(y1))) : y1;
		gradhelper.fyValid = true;
		double squareRadius = (fx - x1) * (fx - x1) + (fy - y1) * (fy - y1);
		if (squareRadius > (x2 * x2))
		{
			fx = x1 + x2 * (fx - x1) / sqrt(squareRadius);
			fy = y1 + x2 * (fy - y1) / sqrt(squareRadius);
		}
		gradhelper.y2Valid = true;
		gradhelper.x1 = x1;
		gradhelper.y1 = y1;
		gradhelper.x2 = x1 + x2;
		gradhelper.y2 = y1;
		gradhelper.fx = fx;
		gradhelper.fy = fy;
		gradhelper.type = 7;
		gradhelper.typeValid = true;
	}
	if (!e.attribute("gradientUnits").isEmpty())
	{
		QString uni = e.attribute("gradientUnits");
		gradhelper.cspace = (uni == "userSpaceOnUse");
		gradhelper.cspaceValid = true;
	}
	else
	{
		gradhelper.cspace = false;
		gradhelper.cspaceValid = false;
	}
	QString transf = e.attribute("gradientTransform");
	if (!transf.isEmpty())
	{
		gradhelper.matrix = parseTransform(e.attribute("gradientTransform"));
		gradhelper.matrixValid = true;
	}
	else
		gradhelper.matrixValid = false;
	QString spreadMethod = e.attribute("spreadMethod");
	if (!spreadMethod.isEmpty())
	{
		if (spreadMethod == "reflect")
			gradhelper.gradient.setRepeatMethod(VGradient::reflect);
		else if (spreadMethod == "repeat")
			gradhelper.gradient.setRepeatMethod(VGradient::repeat);
	}
	parseColorStops(&gradhelper, e);
	m_gradients.insert(id, gradhelper);
	if (gradhelper.gradientValid)
	{
		if (m_Doc->addGradient(id, gradhelper.gradient))
			importedGradients.append(id);
	}
	importedGradTrans.insert(origName, id);
}

QString SVGPlug::parseTagName(const QDomElement& element)
{
	QString tagName(element.tagName());
	if (tagName.startsWith("svg:"))
		return tagName.mid(4, -1);
	return tagName;
}

SVGPlug::~SVGPlug()
{
	delete tmpSel;
}
