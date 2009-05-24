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
#include <QMimeData>
#include <QPainterPath>
#include <QRegExp>
#include <QTemporaryFile>
#include <cmath>
#include "color.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "fonts/scfontmetrics.h"
#include "fpointarray.h"
#include "loadsaveplugin.h"
#include "menumanager.h"
#include "pageitem.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "ui/propertiespalette.h"
#include "sccolorengine.h"
#include "scclocale.h"
#include "scconfig.h"
#include "scgzfile.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "selection.h"
#include "svgplugin.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
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

SVGImportPlugin::SVGImportPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file format
	// support.
	languageChange();
}
/*
void SVGImportPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect( importAction, SIGNAL(triggered()), SLOT(import()) );
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}
*/
SVGImportPlugin::~SVGImportPlugin()
{
	unregisterAll();
};

void SVGImportPlugin::languageChange()
{
	importAction->setText( tr("Import &SVG..."));
	// (Re)register file format support.
	unregisterAll();
	registerFormats();
}

const QString SVGImportPlugin::fullTrName() const
{
	return QObject::tr("SVG Import");
}

const ScActionPlugin::AboutData* SVGImportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports SVG Files");
	about->description = tr("Imports most SVG files into the current document,\nconverting their vector data into Scribus objects.");
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
	fmt.formatId = FORMATID_SVGIMPORT;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::SVG);
 	fmt.nameMatch = QRegExp("\\."+FormatsManager::instance()->extensionListForFormat(FormatsManager::SVG, 1)+"$", Qt::CaseInsensitive);
	fmt.load = true;
	fmt.save = false;
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
	m_Doc=ScCore->primaryMainWindow()->doc;
	ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
	if (filename.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("SVGPlugin");
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
	
	UndoTransaction* activeTransaction = NULL;
	bool emptyDoc = (m_Doc == NULL);
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
		activeTransaction = new UndoTransaction(UndoManager::instance()->beginTransaction(trSettings));
	SVGPlug *dia = new SVGPlug(mw, flags);
	Q_CHECK_PTR(dia);
	dia->import(filename, trSettings, flags);
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	if (dia->importCanceled)
	{
		if (dia->importFailed)
			QMessageBox::warning(mw, CommonStrings::trWarning, tr("The file could not be imported"), 1, 0, 0);
		else if (dia->unsupported)
			QMessageBox::warning(mw, CommonStrings::trWarning, tr("SVG file contains some unsupported features"), 1, 0, 0);
	}

	delete dia;
	return true;
}

SVGPlug::SVGPlug( ScribusMainWindow* mw, int flags ) :
	QObject(mw)
{	
	tmpSel=new Selection(this, false);
	m_Doc=mw->doc;
	unsupported = false;
	importFailed = false;
	importCanceled = true;
	importedColors.clear();
	importedPatterns.clear();
	docDesc = "";
	docTitle = "";
	groupLevel = 0;
	interactive = (flags & LoadSavePlugin::lfInteractive);
//	m_gc.setAutoDelete( true );
}

bool SVGPlug::import(QString fname, const TransactionSettings& trSettings, int flags)
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

bool SVGPlug::loadData(QString fName)
{
	QString f("");
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
		ScGzFile file(fName);
		if (!file.open(QIODevice::ReadOnly))
			return false;
		success = inpdoc.setContent(&file);
		file.close();
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
	QSize wh = parseWidthHeight(docElem);
	double width = wh.width();
	double height = wh.height();
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0);
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
			m_Doc->PageOri = 1;
		else
			m_Doc->PageOri = 0;
		m_Doc->m_pageSize = "Custom";
	}
	FPoint minSize = m_Doc->minCanvasCoordinate;
	FPoint maxSize = m_Doc->maxCanvasCoordinate;
	FPoint cOrigin = m_Doc->view()->canvasOrigin();
	m_Doc->view()->Deselect();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->ScriptRunning = true;
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	gc->FontFamily = m_Doc->toolSettings.defFont;
	if (!m_Doc->PageColors.contains("Black"))
		m_Doc->PageColors.insert("Black", ScColor(0, 0, 0, 255));
	m_gc.push( gc );
	viewTransformX = 0;
	viewTransformY = 0;
	viewScaleX = 1;
	viewScaleY = 1;
	if( !docElem.attribute( "viewBox" ).isEmpty() )
	{
		QString viewbox( docElem.attribute( "viewBox" ) );
		QStringList points = viewbox.replace( QRegExp(","), " ").simplified().split( ' ', QString::SkipEmptyParts );
		if (points.size() > 3)
		{
			QMatrix matrix;
			QSize wh2 = parseWidthHeight(docElem);
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
	QList<PageItem*> Elements = parseGroup( docElem );
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->documentInfo.setTitle(docTitle);
		m_Doc->documentInfo.setComments(docDesc);
	}
	tmpSel->clear();
	if (Elements.count() == 0)
	{
		importFailed = true;
		if (importedColors.count() != 0)
		{
			for (int cd = 0; cd < importedColors.count(); cd++)
			{
				m_Doc->PageColors.remove(importedColors[cd]);
			}
		}
		if (importedPatterns.count() != 0)
		{
			for (int cd = 0; cd < importedPatterns.count(); cd++)
			{
				m_Doc->docPatterns.remove(importedPatterns[cd]);
			}
		}
	}
	if (Elements.count() > 1)
	{
		bool isGroup = true;
		int firstElem = -1;
		if (Elements.at(0)->Groups.count() != 0)
			firstElem = Elements.at(0)->Groups.top();
		for (int bx = 0; bx < Elements.count(); ++bx)
		{
			PageItem* bxi = Elements.at(bx);
			if (bxi->Groups.count() != 0)
			{
				if (bxi->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		if (!isGroup)
		{
			double minx = 99999.9;
			double miny = 99999.9;
			double maxx = -99999.9;
			double maxy = -99999.9;
			uint lowestItem = 999999;
			uint highestItem = 0;
			for (int a = 0; a < Elements.count(); ++a)
			{
				Elements.at(a)->Groups.push(m_Doc->GroupCounter);
				PageItem* currItem = Elements.at(a);
				lowestItem = qMin(lowestItem, currItem->ItemNr);
				highestItem = qMax(highestItem, currItem->ItemNr);
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
			PageItem *high = m_Doc->Items->at(highestItem);
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
			PageItem *neu = m_Doc->Items->takeAt(z);
			m_Doc->Items->insert(lowestItem, neu);
			neu->Groups.push(m_Doc->GroupCounter);
			neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
			neu->AutoName = false;
			neu->isGroupControl = true;
			neu->groupsLastItem = high;
			for (int a = 0; a < m_Doc->Items->count(); ++a)
			{
				m_Doc->Items->at(a)->ItemNr = a;
			}
			neu->setRedrawBounding();
			neu->setTextFlowMode(PageItem::TextFlowDisabled);
			Elements.prepend(neu);
			m_Doc->GroupCounter++;
		}
	}
	m_Doc->DoDrawing = true;
	m_Doc->scMW()->ScriptRunning = false;
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
			m_Doc->m_Selection->delaySignalsOn();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
 				m_Doc->m_Selection->addItem(Elements.at(dre), true);
			}
			m_Doc->m_Selection->delaySignalsOff();
	 		m_Doc->m_Selection->setGroupRect();
			m_Doc->view()->updatesOn(true);
			importCanceled = false;
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
			ScriXmlDoc *ss = new ScriXmlDoc();
			tmpSel->setGroupRect();
			ScElemMimeData* md = new ScElemMimeData();
			md->setScribusElem(ss->WriteElem(m_Doc, m_Doc->view(), tmpSel));
			delete ss;
/*#ifndef Q_WS_MAC*/
// see #2526
			m_Doc->itemSelection_DeleteItem(tmpSel);
/*#endif*/
			m_Doc->view()->updatesOn(true);
			if (importedColors.count() != 0)
			{
				for (int cd = 0; cd < importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(importedColors[cd]);
				}
			}
			if (importedPatterns.count() != 0)
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
			m_Doc->DraggedElem = 0;
			m_Doc->DragElements.clear();
		}
	}
	else
	{
		bool loadF = m_Doc->isLoading();
		m_Doc->setLoading(false);
		m_Doc->changed();
		m_Doc->reformPages();
		m_Doc->view()->updatesOn(true);
		m_Doc->setLoading(loadF);
	}
}

void SVGPlug::addGraphicContext()
{
	SvgStyle *gc = new SvgStyle;
	if ( m_gc.top() )
		*gc = *( m_gc.top() );
	m_gc.push( gc );
}

void SVGPlug::setupNode( const QDomElement &e )
{
	addGraphicContext();
	setupTransform( e );
	parseStyle(m_gc.top(), e);
}

void SVGPlug::setupTransform( const QDomElement &e )
{
	SvgStyle *gc = m_gc.top();
	QMatrix mat = parseTransform( e.attribute( "transform" ) );
	if (!e.attribute("transform").isEmpty())
		gc->matrix = mat * gc->matrix;
}

void SVGPlug::finishNode( const QDomNode &e, PageItem* item)
{
	SvgStyle *gc = m_gc.top();
	QMatrix gcm = gc->matrix;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double coeff1 = sqrt(gcm.m11() * gcm.m11() + gcm.m12() * gcm.m12());
	double coeff2 = sqrt(gcm.m21() * gcm.m21() + gcm.m22() * gcm.m22());
	switch (item->itemType())
	{
	case PageItem::ImageFrame:
		{
			item->ClipEdited = true;
			item->FrameType = 3;
			QMatrix mm = gc->matrix;
			item->PoLine.map(mm);
			item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
			FPoint wh = getMaxClipF(&item->PoLine);
			item->setWidthHeight(wh.x(), wh.y());
			m_Doc->AdjustItemSize(item);
//			item->moveBy(mm.dx(), mm.dy());
//			item->setWidthHeight(item->width() * mm.m11(), item->height() * mm.m22());
//			item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
			if (item->PictureIsAvailable)
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
			QMatrix mm = gc->matrix;
			item->setLineWidth(item->lineWidth() * (coeff1 + coeff2) / 2.0);
		}
		break;
	default:
		{
			item->ClipEdited = true;
			item->FrameType = 3;
			QMatrix mm = gc->matrix;
			item->PoLine.map(mm);
			/*if (haveViewBox)
			{
				QMatrix mv;
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
//				item->Clip = FlattenPath(item->PoLine, item->Segments);
			FPoint wx = getMinClipF(&item->PoLine);
			inGroupXOrigin = qMin(inGroupXOrigin, wx.x());
			inGroupYOrigin = qMin(inGroupYOrigin, wx.y());
			m_Doc->AdjustItemSize(item);
			break;
		}
	}
	item->setRedrawBounding();
	item->OwnPage = m_Doc->OnPage(item);
	if (e.isElement())
	{
		QString nodeId = e.toElement().attribute("id");
		if( !nodeId.isEmpty() )
		{
			item->setItemName(" "+nodeId);
			item->AutoName = false;
		}
	}
	item->setFillTransparency( 1 - gc->FillOpacity * gc->Opacity );
	item->setLineTransparency( 1 - gc->StrokeOpacity * gc->Opacity );
	item->PLineEnd = gc->PLineEnd;
	item->PLineJoin = gc->PLineJoin;
	if (item->fillColor() == CommonStrings::None)
		item->setTextFlowMode(PageItem::TextFlowDisabled);
	else
		item->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
	item->DashOffset = gc->dashOffset;
	item->DashValues = gc->dashArray;
	if (gc->Gradient != 0)
	{
		if (gc->Gradient == 8)
		{
			item->GrType = gc->Gradient;
			item->setPattern(importedPattTrans[gc->GCol1]);
			QMatrix mm = gc->matrixg;
			double rot = getRotationFromMatrix(mm, 0.0) * 180 / M_PI;
			mm.rotate(rot);
			double patDx = (item->xPos() - BaseX) - mm.dx();
			double patDy = (item->yPos() - BaseY) - mm.dy();
			item->setPatternTransform(mm.m11() * 100.0, mm.m22() * 100.0, patDx, patDy, -rot);
		}
		else
		{
			if (gc->GradCo.Stops() > 1)
			{
				item->fill_gradient = gc->GradCo;
				if (!gc->CSpace)
				{
					item->GrStartX = gc->GX1 * item->width();
					item->GrStartY = gc->GY1 * item->height();
					item->GrEndX = gc->GX2 * item->width();
					item->GrEndY = gc->GY2 * item->height();
					double angle1 = atan2(gc->GY2-gc->GY1,gc->GX2-gc->GX1)*(180.0/M_PI);
					double angle2 = atan2(item->GrEndY - item->GrStartX, item->GrEndX - item->GrStartX)*(180.0/M_PI);
					double dx = item->GrStartX + (item->GrEndX - item->GrStartX) / 2.0;
					double dy = item->GrStartY + (item->GrEndY - item->GrStartY) / 2.0;
					QMatrix mm, mm2;
					if ((gc->GY1 < gc->GY2) && (gc->GX1 < gc->GX2))
					{
						mm.rotate(-angle2);
						mm2.rotate(angle1);
					}
					FPointArray gra;
					gra.setPoints(2, item->GrStartX-dx, item->GrStartY-dy, item->GrEndX-dx, item->GrEndY-dy);
					gra.map(mm*mm2);
					gra.translate(dx, dy);
					item->GrStartX = gra.point(0).x();
					item->GrStartY = gra.point(0).y();
					item->GrEndX = gra.point(1).x();
					item->GrEndY = gra.point(1).y();
				}
				else
				{
					QMatrix mm = gc->matrix;
					mm = gc->matrixg * mm;
					FPointArray gra;
					gra.setPoints(2, gc->GX1, gc->GY1, gc->GX2, gc->GY2);
					gra.map(mm);
					gc->GX1 = gra.point(0).x();
					gc->GY1 = gra.point(0).y();
					gc->GX2 = gra.point(1).x();
					gc->GY2 = gra.point(1).y();
					item->GrStartX = gc->GX1 - item->xPos() + BaseX;
					item->GrStartY = gc->GY1 - item->yPos() + BaseY;
					item->GrEndX = gc->GX2 - item->xPos() + BaseX;
					item->GrEndY = gc->GY2 - item->yPos() + BaseY;
				}
				item->GrType = gc->Gradient;
			}
			else
			{
				item->GrType = 0;
				QList<VColorStop*> cstops = gc->GradCo.colorStops();
				item->setFillColor(cstops.at(0)->name);
				item->setFillShade(cstops.at(0)->shade);
			}
		}
	}
}

bool SVGPlug::isIgnorableNode( const QDomElement &e )
{
	QString nodeName(e.tagName());
	return isIgnorableNodeName(nodeName);
}

bool SVGPlug::isIgnorableNodeName( const QString &n )
{
	if (n.startsWith("sodipodi") || n.startsWith("inkscape") || n == "metadata")
		return true;
	return false;
}

FPoint SVGPlug::parseTextPosition(const QDomElement &e, const FPoint* pos)
{
	// FIXME According to spec, we should in fact return a point list
	double  x = pos ? pos->x() : 0.0;
	double  y = pos ? pos->y() : 0.0;

	if (e.hasAttribute( "x" ))
	{
		QString xatt =  e.attribute( "x" , "0" );
		if ( xatt.contains(',') || xatt.contains(' ') )
		{
			xatt.replace(QChar(','), QChar(' '));
			QStringList xl(xatt.split(QChar(' '), QString::SkipEmptyParts));
			xatt = xl.first();
		}
		x = parseUnit( xatt ); 
	}

	if (e.hasAttribute( "y" ))
	{
		QString yatt =  e.attribute( "y" , "0" );
		if ( yatt.contains(',') || yatt.contains(' ') )
		{
			yatt.replace(QChar(','), QChar(' '));
			QStringList yl(yatt.split(QChar(' '), QString::SkipEmptyParts));
			yatt = yl.first();
		}
		y = parseUnit( yatt );
	}

	if (e.hasAttribute( "dx" ))
	{
		QString dxatt =  e.attribute( "dx" , "0" );
		if ( dxatt.contains(',') || dxatt.contains(' ') )
		{
			dxatt.replace(QChar(','), QChar(' '));
			QStringList xl(dxatt.split(QChar(' '), QString::SkipEmptyParts));
			dxatt = xl.first();
		}
		x += parseUnit( dxatt ); 
	}

	if (e.hasAttribute( "dy" ))
	{
		QString dyatt =  e.attribute( "dy" , "0" );
		if ( dyatt.contains(',') || dyatt.contains(' ') )
		{
			dyatt.replace(QChar(','), QChar(' '));
			QStringList xl(dyatt.split(QChar(' '), QString::SkipEmptyParts));
			dyatt = xl.first();
		}
		y += parseUnit( dyatt ); 
	}

	return FPoint(x, y);
}

QSize SVGPlug::parseWidthHeight(const QDomElement &e)
{
	QSize size(550, 841);
	QString sw = e.attribute("width", "100%");
	QString sh = e.attribute("height", "100%");
	double w =  550, h = 841;
	if (!sw.isEmpty())
		w = sw.endsWith("%") ? fromPercentage(sw) : parseUnit(sw);
	if (!sh.isEmpty())
		h = sh.endsWith("%") ? fromPercentage(sh) : parseUnit(sh);
	if (!e.attribute("viewBox").isEmpty())
	{
		QRect viewBox = parseViewBox(e);
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
	size.setWidth(qRound(w));
	size.setHeight(qRound(h));
	return size;
}

QRect SVGPlug::parseViewBox(const QDomElement &e)
{
	QRect box(0, 0, 0, 0);
	if ( !e.attribute( "viewBox" ).isEmpty() )
	{
		QString viewbox( e.attribute( "viewBox" ) );
		QStringList points = viewbox.replace( QRegExp(","), " ").simplified().split( ' ', QString::SkipEmptyParts );
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
	for ( QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement b = n.toElement();
		if ( b.isNull() )
			continue;
		SvgStyle svgStyle;
		parseStyle( &svgStyle, b );
		if ( !svgStyle.Display) 
			continue;
		QString STag2 = b.tagName();
		if ( STag2 == "g" )
		{
			QString id = b.attribute("id", "");
			if (!id.isEmpty())
				m_nodeMap.insert(id, b);
			parseDefs(b);
		}
		else if ( STag2 == "linearGradient" || STag2 == "radialGradient" )
			parseGradient( b );
		else if (STag2 == "clipPath")
			parseClipPath(b);
		else if (STag2 == "pattern")
			parsePattern(b);
		else if ( b.hasAttribute("id") )
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
			parseSVG( b2.attribute( "d" ), &clip );
		else if (b2.nodeName() == "rect")
		{
			double x = parseUnit( b2.attribute( "x", "0.0" ));
			double y = parseUnit( b2.attribute( "y", "0.0" ));
			double width  = parseUnit( b2.attribute( "width" ));
			double height = parseUnit( b2.attribute( "height" ) );
			clip.addQuadPoint(x, y, x, y, width+x, y, width+x, y);
			clip.addQuadPoint(width+x, y, width+x, y, width+x, height+y, width+x, height+y);
			clip.addQuadPoint(width+x, height+y, width+x, height+y, x, height+y, x, height+y);
			clip.addQuadPoint(x, height+y, x, height+y, x, y, x, y);
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
		QString style = e.attribute( "style" ).simplified();
		QStringList substyles = style.split(';', QString::SkipEmptyParts);
		for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
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

QList<PageItem*> SVGPlug::parseGroup(const QDomElement &e)
{
	FPointArray clipPath;
	QList<PageItem*> GElements, gElements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	groupLevel++;
	setupNode(e);
	parseClipPathAttr(e, clipPath);
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *neu = m_Doc->Items->at(z);
	for ( QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement b = n.toElement();
		if( b.isNull() || isIgnorableNode(b) )
			continue;
		SvgStyle svgStyle;
		parseStyle( &svgStyle, b);
		if (!svgStyle.Display) 
			continue;
		QList<PageItem*> el = parseElement(b);
		for (int ec = 0; ec < el.count(); ++ec)
			gElements.append(el.at(ec));
	}
	groupLevel--;
	SvgStyle *gc = m_gc.top();
	if (clipPath.size() == 0)
	{
		if (gc->clipPath.size() != 0)
			clipPath = gc->clipPath.copy();
	}
	if (gElements.count() == 0 || (gElements.count() < 2 && (clipPath.size() == 0) && (gc->Opacity == 1.0)))
	{
		// Unfortunately we have to take the long route here, or we risk crash on undo/redo
		// FIXME : create group object after parsing grouped objects
		/*m_Doc->Items->takeAt(z);
		delete neu;*/
		Selection tmpSelection(m_Doc, false);
		tmpSelection.addItem(neu);
		m_Doc->itemSelection_DeleteItem(&tmpSelection);
		for (int a = 0; a < m_Doc->Items->count(); ++a)
		{
			m_Doc->Items->at(a)->ItemNr = a;
		}
		for (int gr = 0; gr < gElements.count(); ++gr)
		{
			GElements.append(gElements.at(gr));
		}
	}
	else
	{
		double minx = 99999.9;
		double miny = 99999.9;
		double maxx = -99999.9;
		double maxy = -99999.9;
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
		neu->setXYPos(gx, gy);
		neu->setWidthHeight(gw, gh);
		if (clipPath.size() != 0)
		{
			QMatrix mm = gc->matrix;
			neu->PoLine = clipPath.copy();
			neu->PoLine.map(mm);
			neu->PoLine.translate(-gx + BaseX, -gy + BaseY);
			clipPath.resize(0);

		}
		else
			neu->SetRectFrame();
		neu->Clip = FlattenPath(neu->PoLine, neu->Segments);
		neu->Groups.push(m_Doc->GroupCounter);
		neu->isGroupControl = true;
		neu->groupsLastItem = gElements.at(gElements.count()-1);
		if( !e.attribute("id").isEmpty() )
			neu->setItemName(e.attribute("id"));
		else
			neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
		neu->AutoName = false;
		neu->setFillTransparency(1 - gc->Opacity);
		neu->gXpos = neu->xPos() - gx;
		neu->gYpos = neu->yPos() - gy;
		neu->gWidth = gw;
		neu->gHeight = gh;
		for (int gr = 0; gr < gElements.count(); ++gr)
		{
			gElements.at(gr)->Groups.push(m_Doc->GroupCounter);
			gElements.at(gr)->gXpos = gElements.at(gr)->xPos() - gx;
			gElements.at(gr)->gYpos = gElements.at(gr)->yPos() - gy;
			gElements.at(gr)->gWidth = gw;
			gElements.at(gr)->gHeight = gh;
			GElements.append(gElements.at(gr));
		}
		neu->setRedrawBounding();
		neu->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->GroupCounter++;
	}
	delete( m_gc.pop() );
	return GElements;
}

QList<PageItem*> SVGPlug::parseElement(const QDomElement &e)
{
	QList<PageItem*> GElements;
	if (e.hasAttribute("id"))
		m_nodeMap.insert(e.attribute("id"), e);
	QString STag = e.tagName();
	if( STag == "g" )
	{
		GElements = parseGroup( e );
		return GElements;
	}
	if( STag == "defs" )
		parseDefs(e);
	else if( STag == "switch" )
		GElements = parseSwitch(e);
	else if( STag == "symbol" )
		GElements = parseSymbol(e);
	else if( STag == "use" )
		GElements = parseUse(e);
	else if( STag == "linearGradient" || STag == "radialGradient" )
		parseGradient( e );
	else if( STag == "rect" )
		GElements = parseRect(e);
	else if( STag == "ellipse" )
		GElements = parseEllipse(e);
	else if( STag == "circle" )
		GElements = parseCircle(e);
	else if( STag == "line" )
		GElements = parseLine(e);
	else if( STag == "path" )
		GElements = parsePath(e);
	else if( STag == "polyline" || e.tagName() == "polygon" )
		GElements = parsePolyline(e);
	else if( STag == "text" )
		GElements = parseText(e);
	else if( STag == "clipPath" )
		parseClipPath(e);
	else if( STag == "desc" )
	{
		if (groupLevel == 1)
			docDesc = e.text();
	}
	else if( STag == "title" )
	{
		if (groupLevel == 1)
			docTitle = e.text();
	}
	else if( STag == "image" )
		GElements = parseImage(e);
/*	else if( STag == "i:pgf" )
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
/*	else if( STag == "image" )
		GElements = parseImage(e);
	} */
	else if(!isIgnorableNodeName(STag) )
	{
		// warn if unsupported SVG feature are encountered
		if (!m_unsupportedFeatures.contains(STag))
		{
			m_unsupportedFeatures.insert(STag, STag);
			qDebug() << QString("unsupported SVG feature: %1").arg(STag);
			unsupported = true;
		}
	}
	return GElements;
}

QList<PageItem*> SVGPlug::parseCircle(const QDomElement &e)
{
	QList<PageItem*> CElements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double r = parseUnit( e.attribute( "r" ) );
	double x = parseUnit( e.attribute( "cx" ) ) - r;
	double y = parseUnit( e.attribute( "cy" ) ) - r;
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, r * 2.0, r * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
	PageItem* ite = m_Doc->Items->at(z);
	QMatrix mm = QMatrix();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	finishNode(e, ite);
	CElements.append(ite);
	delete( m_gc.pop() );
	return CElements;
}

QList<PageItem*> SVGPlug::parseEllipse(const QDomElement &e)
{
	QList<PageItem*> EElements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double rx = parseUnit( e.attribute( "rx" ) );
	double ry = parseUnit( e.attribute( "ry" ) );
	double x = parseUnit( e.attribute( "cx" ) ) - rx;
	double y = parseUnit( e.attribute( "cy" ) ) - ry;
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, rx * 2.0, ry * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
	PageItem* ite = m_Doc->Items->at(z);
	QMatrix mm = QMatrix();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	finishNode(e, ite);
	EElements.append(ite);
	delete( m_gc.pop() );
	return EElements;
}

QList<PageItem*> SVGPlug::parseImage(const QDomElement &e)
{
	FPointArray clipPath;
	QList<PageItem*> IElements;
	QString fname = e.attribute("xlink:href");
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x = e.attribute( "x" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "x" ) );
	double y = e.attribute( "y" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "y" ) );
	double w = e.attribute( "width" ).isEmpty() ? 1.0 : parseUnit( e.attribute( "width" ) );
	double h = e.attribute( "height" ).isEmpty() ? 1.0 : parseUnit( e.attribute( "height" ) );
	setupNode(e);
	parseClipPathAttr(e, clipPath);
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x+BaseX, y+BaseY, w, h, 1, m_Doc->toolSettings.dBrushPict, CommonStrings::None, true);
	PageItem* ite = m_Doc->Items->at(z);
	if (!fname.isEmpty())
	{
		if (!fname.startsWith("data:"))
			m_Doc->LoadPict(fname, z);
		else
		{
			int startData = fname.indexOf(",");
			QString dataType = fname.left(startData);
			fname.remove(0, startData+1);
			QByteArray ba;
			ba.append(fname);
			if (dataType.contains("base64"))
				ba = QByteArray::fromBase64(ba);
			ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_svg_XXXXXX.png");
			ite->tempImageFile->open();
			QString fileName = getLongPathName(ite->tempImageFile->fileName());
			ite->tempImageFile->close();
			ite->isInlineImage = true;
			QImage img;
			img.loadFromData(ba);
			img.save(fileName, "PNG");
			m_Doc->LoadPict(fileName, z);
		}
	}
	if (clipPath.size() != 0)
		ite->PoLine = clipPath.copy();
	clipPath.resize(0);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	finishNode(e, ite);
	IElements.append(ite);
	delete( m_gc.pop() );
	return IElements;
}

QList<PageItem*> SVGPlug::parseLine(const QDomElement &e)
{
	QList<PageItem*> LElements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x1 = e.attribute( "x1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "x1" ) );
	double y1 = e.attribute( "y1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "y1" ) );
	double x2 = e.attribute( "x2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "x2" ) );
	double y2 = e.attribute( "y2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "y2" ) );
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
	PageItem* ite = m_Doc->Items->at(z);
	ite->PoLine.resize(4);
	ite->PoLine.setPoint(0, FPoint(x1, y1));
	ite->PoLine.setPoint(1, FPoint(x1, y1));
	ite->PoLine.setPoint(2, FPoint(x2, y2));
	ite->PoLine.setPoint(3, FPoint(x2, y2));
	finishNode(e, ite);
	LElements.append(ite);
	delete( m_gc.pop() );
	return LElements;
}

QList<PageItem*> SVGPlug::parsePath(const QDomElement &e)
{
	FPointArray pArray;
	QList<PageItem*> PElements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	PageItem::ItemType itype = parseSVG(e.attribute("d"), &pArray) ? PageItem::PolyLine : PageItem::Polygon; 
	int z = m_Doc->itemAdd(itype, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
	PageItem* ite = m_Doc->Items->at(z);
	ite->fillRule = (gc->fillRule != "nonzero");
	ite->PoLine = pArray;
	if (ite->PoLine.size() < 4)
	{
// 			m_Doc->m_Selection->addItem(ite);
		tmpSel->addItem(ite);
// 			m_Doc->itemSelection_DeleteItem();
		m_Doc->itemSelection_DeleteItem(tmpSel);
// 			m_Doc->m_Selection->clear();
	}
	else
	{
		finishNode(e, ite);
		PElements.append(ite);
	}
	delete( m_gc.pop() );
	return PElements;
}

QList<PageItem*> SVGPlug::parsePolyline(const QDomElement &e)
{
	int z;
	QList<PageItem*> PElements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	QString points = e.attribute( "points" );
	if (!points.isEmpty())
	{
		QString STag = e.tagName();
		points = points.simplified().replace(',', " ");
		QStringList pointList = points.split( ' ', QString::SkipEmptyParts );
		if (( e.tagName() == "polygon" ) && (pointList.count() > 4))
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->fillRule = (gc->fillRule != "nonzero"); 
		ite->PoLine.resize(0);
		ite->PoLine.svgInit();
		bool bFirst = true;
		double x = 0.0;
		double y = 0.0;
		for( QStringList::Iterator it = pointList.begin(); it != pointList.end(); it++ )
		{
			if( bFirst )
			{
				x = ScCLocale::toDoubleC(*(it++));
				y = ScCLocale::toDoubleC(*it);
				ite->PoLine.svgMoveTo(x, y);
				bFirst = false;
			}
			else
			{
				x = ScCLocale::toDoubleC(*(it++));
				y = ScCLocale::toDoubleC(*it);
				ite->PoLine.svgLineTo(x, y);
			}
		}
		if (( STag == "polygon" ) && (pointList.count() > 4))
			ite->PoLine.svgClosePath();
		else
			ite->convertTo(PageItem::PolyLine);
		finishNode(e, ite);
		PElements.append(ite);
	}
	delete( m_gc.pop() );
	return PElements;
}

QList<PageItem*> SVGPlug::parseRect(const QDomElement &e)
{
	QList<PageItem*> RElements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x = parseUnit( e.attribute( "x" ) );
	double y = parseUnit( e.attribute( "y" ) );
	double width = parseUnit( e.attribute( "width" ));
	double height = parseUnit( e.attribute( "height" ) );
	double rx = e.attribute( "rx" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "rx" ) );
	double ry = e.attribute( "ry" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "ry" ) );
	setupNode(e);
	SvgStyle *gc = m_gc.top();
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, width, height, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
	PageItem* ite = m_Doc->Items->at(z);
	if ((rx != 0) || (ry != 0))
	{
		ite->setCornerRadius(qMax(rx, ry));
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
	QMatrix mm = QMatrix();
	mm.translate(x, y);
	ite->PoLine.map(mm);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	finishNode(e, ite);
	RElements.append(ite);
	delete( m_gc.pop() );
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
	for(QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		if (n.isElement() && (n.toElement().tagName() == "tspan"))
		{
			QList<PageItem*> el = parseTextSpan(n.toElement(), currentPos, chunkWidth);
			for (int ec = 0; ec < el.count(); ++ec)
				GElements.append(el.at(ec));
		}
		if (n.isText())
		{
			QList<PageItem*> el = parseTextNode(n.toText(), currentPos, chunkWidth);
			for (int ec = 0; ec < el.count(); ++ec)
				GElements.append(el.at(ec));
		}
	}
	delete( m_gc.pop() );
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
	for(QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		if (n.isElement() && (n.toElement().tagName() == "tspan"))
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
	delete( m_gc.pop() );
	return GElements;
}

QList<PageItem*> SVGPlug::parseTextNode(const QDomText& e, FPoint& currentPos, double chunkW)
{
	QList<PageItem*> GElements;
	double BaseX  = m_Doc->currentPage()->xOffset();
	double BaseY  = m_Doc->currentPage()->yOffset();
	double StartX = currentPos.x(), StartY = currentPos.y();
	
	QString textString = e.data().simplified();
	if ( textString.isEmpty() )
		return GElements;
	
	SvgStyle *gc   = m_gc.top();
	QFont textFont = getFontFromStyle(*gc);
	QFontMetrics fm(textFont);
    double width   = fm.width(textString);

	if( gc->textAnchor == "middle" )
		StartX -= chunkW / 2.0;
	else if( gc->textAnchor == "end")
		StartX -= chunkW;

	FPointArray textPath;
	QString textFillColor   = gc->FillCol;
	QString textStrokeColor = gc->StrokeCol;
	QPainterPath painterPath;
	painterPath.addText( StartX, StartY, textFont, textString );
	textPath.fromQPainterPath(painterPath);
	if (textPath.size() > 0)
	{
//		double  lineWidth = 0.0;
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, textFillColor, textStrokeColor, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = textPath;
		finishNode(e, ite);
		GElements.append(ite);
	}
	currentPos.setX( currentPos.x() + width );
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
		QString STag = de.tagName();
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
			else if (de.hasAttribute("id") && hrefs.contains(de.attribute("id")))
				continue;
			else
			{
				SElements = parseElement(de);
				if (SElements.count() > 0)
					break;
			}
		}
	}
	return SElements;
}

QList<PageItem*> SVGPlug::parseSymbol(const QDomElement &e)
{
	QList<PageItem*> SElements;
	QString id = e.attribute("id");
	if( !id.isEmpty() )
		m_nodeMap.insert(id, e);
	return SElements;
}

QList<PageItem*> SVGPlug::parseUse(const QDomElement &e)
{
	QList<PageItem*> UElements;
	setupNode(e);
	if( e.hasAttribute("x") || e.hasAttribute("y") )
	{
		QMatrix matrix;
		double  xAtt = ScCLocale::toDoubleC(e.attribute("x", "0.0"));
		double  yAtt = ScCLocale::toDoubleC(e.attribute("y", "0.0"));
		SvgStyle *gc = m_gc.top();
		gc->matrix   = QMatrix(1.0, 0.0, 0.0, 1.0, xAtt, yAtt) * gc->matrix;
	}
	QString href = e.attribute("xlink:href").mid(1);
	QMap<QString, QDomElement>::Iterator it = m_nodeMap.find(href);
	if (it != m_nodeMap.end())
	{
		QDomElement elem = it.value().toElement();
		if( elem.tagName() == "symbol" )
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
	font.setStyleStrategy( QFont::PreferOutline );

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
	font.setPointSize(style.FontSize / 10);
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
	for(QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		if (n.isElement() && (n.toElement().tagName() == "tspan"))
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
				QFontMetrics fm(textFont);
				width += fm.width(textString);
			}
		}
	}
	delete (m_gc.pop());
	return doBreak;
}

double SVGPlug::fromPercentage( const QString &s )
{
	QString s1 = s;
	if (s1.endsWith( ";" ))
		s1 = s1.left(s1.length() - 1);
	if (s1.endsWith( "%" ))
	{
		s1 = s1.left(s1.length() - 1);
		return ScCLocale::toDoubleC(s1) / 100.0;
	}
	else
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
	if( unit.right( 2 ) == "pt" )
		unitval.replace( "pt", "" );
	else if( unit.right( 2 ) == "cm" )
		unitval.replace( "cm", "" );
	else if( unit.right( 2 ) == "mm" )
		unitval.replace( "mm" , "" );
	else if( unit.right( 2 ) == "in" )
		unitval.replace( "in", "" );
	else if( unit.right( 2 ) == "px" )
		unitval.replace( "px", "" );
	if (unitval == unit)
		noUnit = true;
	double value = ScCLocale::toDoubleC(unitval);
	if( unit.right( 2 ) == "pt" )
		value = value;
	else if( unit.right( 2 ) == "cm" )
		value = ( value / 2.54 ) * 72;
	else if( unit.right( 2 ) == "mm" )
		value = ( value / 25.4 ) * 72;
	else if( unit.right( 2 ) == "in" )
		value = value * 72;
	else if( unit.right( 2 ) == "px" )
		value = value * 0.8;
	else if(noUnit)
		value = value;
	return value;
}

QMatrix SVGPlug::parseTransform( const QString &transform )
{
	QMatrix ret;
	// Workaround for QString::split() bug when string ends with space
	QString trans = transform.simplified();
	// Split string for handling 1 transform statement at a time
	QStringList subtransforms = trans.split(')', QString::SkipEmptyParts);
	QStringList::ConstIterator it = subtransforms.begin();
	QStringList::ConstIterator end = subtransforms.end();
	for(; it != end; ++it)
	{
		QMatrix result;
		QStringList subtransform = it->split('(', QString::SkipEmptyParts);
		subtransform[0] = subtransform[0].trimmed().toLower();
		subtransform[1] = subtransform[1].simplified();
		QRegExp reg("[,( ]");
		QStringList params = subtransform[1].split(reg, QString::SkipEmptyParts);
		if(subtransform[0].startsWith(";") || subtransform[0].startsWith(","))
			subtransform[0] = subtransform[0].right(subtransform[0].length() - 1);
		if(subtransform[0] == "rotate")
		{
			if(params.count() == 3)
			{
				double x = ScCLocale::toDoubleC(params[1]);
				double y = ScCLocale::toDoubleC(params[2]);
				result.translate(x, y);
				result.rotate(ScCLocale::toDoubleC(params[0]));
				result.translate(-x, -y);
			}
			else
				result.rotate(ScCLocale::toDoubleC(params[0]));
		}
		else if(subtransform[0] == "translate")
		{
			if(params.count() == 2)
				result.translate(ScCLocale::toDoubleC(params[0]), ScCLocale::toDoubleC(params[1]));
			else    // Spec : if only one param given, assume 2nd param to be 0
				result.translate(ScCLocale::toDoubleC(params[0]), 0);
		}
		else if(subtransform[0] == "scale")
		{
			if(params.count() == 2)
				result.scale(ScCLocale::toDoubleC(params[0]), ScCLocale::toDoubleC(params[1]));
			else    // Spec : if only one param given, assume uniform scaling
				result.scale(ScCLocale::toDoubleC(params[0]), ScCLocale::toDoubleC(params[0]));
		}
		else if(subtransform[0] == "skewx")
			result.shear(tan(ScCLocale::toDoubleC(params[0]) * 0.01745329251994329576), 0.0F);
		else if(subtransform[0] == "skewy")
			result.shear(0.0F, tan(ScCLocale::toDoubleC(params[0]) * 0.01745329251994329576));
		else if(subtransform[0] == "matrix")
		{
			if(params.count() >= 6)
			{
				double sx = ScCLocale::toDoubleC(params[0]);
				double sy = ScCLocale::toDoubleC(params[3]);
				double p1 = ScCLocale::toDoubleC(params[1]);
				double p2 = ScCLocale::toDoubleC(params[2]);
				double p4 = ScCLocale::toDoubleC(params[4]);
				double p5 = ScCLocale::toDoubleC(params[5]);
				result.setMatrix(sx, p1, p2, sy, p4, p5);
			}
		}
		ret = result * ret;
	}
	return ret;
}

const char * SVGPlug::getCoord( const char *ptr, double &number )
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
	if(*ptr == '+')
		ptr++;
	else if(*ptr == '-')
	{
		ptr++;
		sign = -1;
	}

	// read the integer part
	while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		integer = (integer * 10) + *(ptr++) - '0';
	if(*ptr == '.') // read the decimals
	{
		ptr++;
		while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
			decimal += (*(ptr++) - '0') * (frac *= 0.1);
	}

	if(*ptr == 'e' || *ptr == 'E') // read the exponent part
	{
		ptr++;

		// read the sign of the exponent
		if(*ptr == '+')
			ptr++;
		else if(*ptr == '-')
		{
			ptr++;
			expsign = -1;
		}

		exponent = 0;
		while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		{
			exponent *= 10;
			exponent += *ptr - '0';
			ptr++;
		}
	}
	number = integer + decimal;
	number *= sign * pow( static_cast<double>(10), static_cast<double>( expsign * exponent ) );
	// skip the following space
	if(*ptr == ' ')
		ptr++;

	return ptr;
}

bool SVGPlug::parseSVG( const QString &s, FPointArray *ite )
{
	return ite->parseSVG(s);
}


QColor SVGPlug::parseColorN( const QString &rgbColor )
{
	int r, g, b;
	keywordToRGB( rgbColor.toLower(), r, g, b );
	return QColor( r, g, b );
}


QString SVGPlug::parseColor( const QString &s )
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
		QString r = colors[0].right( ( colors[0].length() - 4 ) );
		QString g = colors[1];
		QString b = colors[2].left( ( colors[2].length() - 1 ) );
		if (r.contains( "%" ))
		{
			r = r.left( r.length() - 1 );
			r = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(r) ) / 100.0 ) ) );
		}
		if (g.contains( "%" ))
		{
			g = g.left( g.length() - 1 );
			g = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(g) ) / 100.0 ) ) );
		}
		if (b.contains( "%" ))
		{
			b = b.left( b.length() - 1 );
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
	ColorList::Iterator it;
	bool found = false;
	int r, g, b;
	QColor tmpR;
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&r, &g, &b);
			tmpR.setRgb(r, g, b);
			if (c == tmpR)
			{
				ret = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		ScColor tmp;
		tmp.fromQColor(c);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString newColorName = "FromSVG"+c.name();
		m_Doc->PageColors.insert(newColorName, tmp);
		importedColors.append(newColorName);
		ret = newColorName;
	}
	return ret;
}

QString SVGPlug::parseIccColor( const QString &s )
{
	QColor color, tmpR;
	QString ret;
	bool iccColorFound = false, found = false;
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
			QString cs = colors[1], ys = colors[2], ms = colors[3], ks = colors[4];
			if (cs.contains( "%" ))
			{
				cs = cs.left( cs.length() - 1 );
				cs = QString::number(ScCLocale::toDoubleC(cs) / 100);
			}
			if (ys.contains( "%" ))
			{
				ys = ys.left( ys.length() - 1 );
				ys = QString::number(ScCLocale::toDoubleC(ys) / 100);
			}
			if (ms.contains( "%" ))
			{
				ms = ms.left( ms.length() - 1 );
				ms = QString::number(ScCLocale::toDoubleC(ms) / 100);
			}
			if (ks.contains( "%" ))
			{
				ks = ks.left( ks.length() - 1 );
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
	if (iccColorFound == false)
		return ret;
	int  c, m, y, k;
	ColorList::Iterator it;
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		colorModel colorMod = it.value().getColorModel();
		if (colorMod == colorModelCMYK)
		{
			it.value().getCMYK(&c, &m, &y, &k);
			tmpR.setCmyk(c, m, y, k);
			if (color == tmpR)
			{
				ret = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		ScColor tmp;
		tmp.fromQColor(color);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString newColorName = "FromSVG"+tmp.name();
		m_Doc->PageColors.insert(newColorName, tmp);
		importedColors.append(newColorName);
		ret = newColorName;
	}
	return ret;
}

void SVGPlug::parsePA( SvgStyle *obj, const QString &command, const QString &params )
{
	if( command == "display" )
		obj->Display = (params == "none") ? false : true;
	else if( command == "stroke-opacity" )
		obj->StrokeOpacity  = fromPercentage(params);
	else if( command == "fill-opacity" )
		obj->FillOpacity = fromPercentage(params);
	else if( command == "opacity" )
		obj->Opacity = fromPercentage(params);
	else if( command == "fill" )
	{
//		if ((obj->InherCol) && (params == "currentColor"))
		if (params == "currentColor")
			obj->FillCol = obj->CurCol;
		else if (params == "none")
		{
			obj->FillCol = CommonStrings::None;
		}
		else if( params.startsWith( "url(" ) )
		{
			unsigned int start = params.indexOf("#") + 1;
			unsigned int end = params.lastIndexOf(")");
			QString key = params.mid(start, end - start);
			obj->Gradient = 0;
			obj->matrixg = QMatrix();
			bool firstMatrixValid = false;
			if (m_gradients[key].matrixValid)
			{
				firstMatrixValid = true;
				obj->matrixg = m_gradients[key].matrix;
			}
			while (!m_gradients[key].reference.isEmpty())
			{
				QString key2 = m_gradients[key].reference;
				if (m_gradients[key2].typeValid)
					obj->Gradient = m_gradients[key2].Type;
				if (obj->Gradient != 8)
				{
					if (m_gradients[key2].gradientValid)
						obj->GradCo = m_gradients[key2].gradient;
					if (m_gradients[key2].cspaceValid)
						obj->CSpace = m_gradients[key2].CSpace;
					if (m_gradients[key2].x1Valid)
						obj->GX1 = m_gradients[key2].X1;
					if (m_gradients[key2].y1Valid)
						obj->GY1 = m_gradients[key2].Y1;
					if (m_gradients[key2].x2Valid)
						obj->GX2 = m_gradients[key2].X2;
					if (m_gradients[key2].y2Valid)
						obj->GY2 = m_gradients[key2].Y2;
					if (m_gradients[key2].matrixValid)
						obj->matrixg = m_gradients[key2].matrix;
				}
				else
				{
					obj->GCol1 = key2;
					if ((m_gradients[key2].matrixValid) && (!firstMatrixValid))
						obj->matrixg *= m_gradients[key2].matrix;
				}
				key = m_gradients[key].reference;
			}
			if (obj->Gradient != 8)
			{
				key = params.mid(start, end - start);
				if (m_gradients[key].typeValid)
					obj->Gradient = m_gradients[key].Type;
				if (obj->Gradient != 8)
				{
					if (m_gradients[key].gradientValid)
						obj->GradCo = m_gradients[key].gradient;
					if (m_gradients[key].cspaceValid)
						obj->CSpace = m_gradients[key].CSpace;
					if (m_gradients[key].x1Valid)
						obj->GX1 = m_gradients[key].X1;
					if (m_gradients[key].y1Valid)
						obj->GY1 = m_gradients[key].Y1;
					if (m_gradients[key].x2Valid)
						obj->GX2 = m_gradients[key].X2;
					if (m_gradients[key].y2Valid)
						obj->GY2 = m_gradients[key].Y2;
					if (m_gradients[key].matrixValid)
						obj->matrixg = m_gradients[key].matrix;
				}
				else
				{
					obj->GCol1 = key;
					if (m_gradients[key].matrixValid)
						obj->matrixg = m_gradients[key].matrix;
				}
			}
			obj->FillCol = CommonStrings::None;
		}
		else
			obj->FillCol = parseColor(params);
	}
	else if( command == "fill-rule" )
	{
		obj->fillRule = params;
	}
	else if( command == "color" )
	{
		if (params == "none")
			obj->CurCol = CommonStrings::None;
		else if( params.startsWith( "url(" ) )
			obj->CurCol = CommonStrings::None;
		else if (params == "currentColor")
			obj->CurCol = obj->CurCol;
		else
			obj->CurCol = parseColor(params);
	}
	else if( command == "stroke" )
	{
//		if ((obj->InherCol) && (params == "currentColor"))
		if (params == "currentColor")
			obj->StrokeCol = obj->CurCol;
		else if (params == "none")
		{
			obj->StrokeCol = CommonStrings::None;
		}
		else if( params.startsWith( "url(" ) )
		{
			obj->StrokeCol = CommonStrings::None;
		}
		else
			obj->StrokeCol = parseColor(params);
		/*		if( params == "none" )
					gc->stroke.setType( VStroke::none );
				else if( params.startsWith( "url(" ) )
				{
					unsigned int start = params.find("#") + 1;
					unsigned int end = params.lastIndexOf(")");
					QString key = params.mid( start, end - start );
					gc->stroke.gradient() = m_gradients[ key ].gradient;
					gc->stroke.gradient().transform( m_gradients[ key ].gradientTransform );
					gc->stroke.gradient().transform( gc->matrix );
					gc->stroke.setType( VStroke::grad );
				}
				else
				{
					parseColor( strokecolor, params );
					gc->stroke.setType( VStroke::solid );
				} */
	}
	else if( command == "stroke-width" )
		obj->LWidth = parseUnit( params );
	else if( command == "stroke-linejoin" )
	{
		if( params == "miter" )
			obj->PLineJoin = Qt::MiterJoin;
		else if( params == "round" )
			obj->PLineJoin = Qt::RoundJoin;
		else if( params == "bevel" )
			obj->PLineJoin = Qt::BevelJoin;
	}
	else if( command == "stroke-linecap" )
	{
		if( params == "butt" )
			obj->PLineEnd = Qt::FlatCap;
		else if( params == "round" )
			obj->PLineEnd = Qt::RoundCap;
		else if( params == "square" )
			obj->PLineEnd = Qt::SquareCap;
	}
	//	else if( command == "stroke-miterlimit" )
	//		gc->stroke.setMiterLimit( params.todouble() );
	else if( command == "stroke-dasharray" )
	{
		QVector<double> array;
		if(params != "none")
		{
			QString params2 = params.simplified().replace(',', " ");
			QStringList dashes = params2.split(' ', QString::SkipEmptyParts);
			for( QStringList::Iterator it = dashes.begin(); it != dashes.end(); ++it )
				array.append( ScCLocale::toDoubleC(*it) );
		}
		obj->dashArray = array;
	}
	else if( command == "stroke-dashoffset" )
		obj->dashOffset = ScCLocale::toDoubleC(params);
	else if( command == "font-family" )
		obj->FontFamily = params;
	else if( command == "font-style" )
		obj->FontStyle = params;
	else if( command == "font-weight" )
		obj->FontWeight = params;
	else if( command == "font-stretch" )
		obj->FontStretch = params;
	else if( command == "font-size" )
		obj->FontSize = static_cast<int>(parseFontSize(params) * 10.0);
	else if( command == "text-anchor" )
		obj->textAnchor = params;
	else if( command == "text-decoration" )
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
	else if( !isIgnorableNodeName(command) )
	{
		if (!m_unsupportedFeatures.contains(command))
		{
			m_unsupportedFeatures.insert(command, command);
			qDebug() << QString("unsupported SVG feature: %1").arg(command);
			unsupported = true;
		}
	}
}

void SVGPlug::parseStyle( SvgStyle *obj, const QDomElement &e )
{
	SvgStyle *gc = m_gc.top();
	if (!gc)
		return;
	if( !e.attribute( "display" ).isEmpty() )
		parsePA( obj, "display", e.attribute( "display" ) );
	if( !e.attribute( "color" ).isEmpty() )
	{
		if (e.attribute( "color" ) == "inherit")
			gc->InherCol = true;
		else
			parsePA( obj, "color", e.attribute( "color" ) );
	}
	if( !e.attribute( "fill" ).isEmpty() )
		parsePA( obj, "fill", e.attribute( "fill" ) );
	if( !e.attribute( "stroke" ).isEmpty() )
		parsePA( obj, "stroke", e.attribute( "stroke" ) );
	if( !e.attribute( "stroke-width" ).isEmpty() )
		parsePA( obj, "stroke-width", e.attribute( "stroke-width" ) );
	if( !e.attribute( "stroke-linejoin" ).isEmpty() )
		parsePA( obj, "stroke-linejoin", e.attribute( "stroke-linejoin" ) );
	if( !e.attribute( "stroke-linecap" ).isEmpty() )
		parsePA( obj, "stroke-linecap", e.attribute( "stroke-linecap" ) );
	if( !e.attribute( "stroke-dasharray" ).isEmpty() )
		parsePA( obj, "stroke-dasharray", e.attribute( "stroke-dasharray" ) );
	if( !e.attribute( "stroke-dashoffset" ).isEmpty() )
		parsePA( obj, "stroke-dashoffset", e.attribute( "stroke-dashoffset" ) );
	if( !e.attribute( "stroke-opacity" ).isEmpty() )
		parsePA( obj, "stroke-opacity", e.attribute( "stroke-opacity" ) );
	/*	if( !e.attribute( "stroke-miterlimit" ).isEmpty() )
			parsePA( obj, "stroke-miterlimit", e.attribute( "stroke-miterlimit" ) );   */
	if( !e.attribute( "fill-rule" ).isEmpty() )
		parsePA( obj, "fill-rule", e.attribute( "fill-rule" ) );
	if( !e.attribute( "fill-opacity" ).isEmpty() )
		parsePA( obj, "fill-opacity", e.attribute( "fill-opacity" ) );
	if( !e.attribute( "opacity" ).isEmpty() )
		parsePA( obj, "opacity", e.attribute( "opacity" ) );
	if( !e.attribute( "font-family" ).isEmpty() )
		parsePA( obj, "font-family", e.attribute( "font-family" ) );
	if( !e.attribute( "font-style" ).isEmpty() )
		parsePA( obj, "font-style", e.attribute( "font-style" ) );
	if( !e.attribute( "font-weight" ).isEmpty() )
		parsePA( obj, "font-weight", e.attribute( "font-weight" ) );
	if( !e.attribute( "font-stretch" ).isEmpty() )
		parsePA( obj, "font-stretch", e.attribute( "font-stretch" ) );
	if( !e.attribute( "font-size" ).isEmpty() )
		parsePA( obj, "font-size", e.attribute( "font-size" ) );
	if( !e.attribute( "text-anchor" ).isEmpty() )
		parsePA( obj, "text-anchor", e.attribute( "text-anchor" ) );
	if( !e.attribute( "text-decoration" ).isEmpty() )
		parsePA( obj, "text-decoration", e.attribute( "text-decoration" ) );
	QString style = e.attribute( "style" ).simplified();
	QStringList substyles = style.split(';', QString::SkipEmptyParts);
	for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
	{
		QStringList substyle = (*it).split(':', QString::SkipEmptyParts);
		QString command(substyle[0].trimmed());
		QString params(substyle[1].trimmed());
		parsePA( obj, command, params );
	}
	return;
}

void SVGPlug::parseColorStops(GradientHelper *gradient, const QDomElement &e)
{
	QString Col = "Black";
	double offset = 0;
	double opa;
	SvgStyle svgStyle;
	parseStyle( &svgStyle, e );
	for(QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		opa = 1.0;
		QDomElement stop = n.toElement();
		if(stop.tagName() == "stop")
		{
			QString temp = stop.attribute( "offset" );
			if( temp.contains( '%' ) )
			{
				temp = temp.left( temp.length() - 1 );
				offset = ScCLocale::toDoubleC(temp) / 100.0;
			}
			else
				offset = ScCLocale::toDoubleC(temp);
			if( !stop.attribute( "stop-opacity" ).isEmpty() )
				opa = fromPercentage(stop.attribute("stop-opacity"));
			if( !stop.attribute( "stop-color" ).isEmpty() )
			{
				if (stop.attribute("stop-color") == "currentColor")
					Col = svgStyle.CurCol;
				else
					Col = parseColor(stop.attribute("stop-color"));
			}
			else
			{
				QString style = stop.attribute( "style" ).simplified();
				QStringList substyles = style.split(';', QString::SkipEmptyParts);
				for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
				{
					QStringList substyle = (*it).split(':', QString::SkipEmptyParts);
					QString command(substyle[0].trimmed());
					QString params(substyle[1].trimmed());
					if( command == "stop-color" )
						Col = parseColor(params);
					if( command == "stop-opacity" )
						opa = fromPercentage(params);
				}
			}
		}
		const ScColor& gradC = m_Doc->PageColors[Col];
		gradient->gradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), offset, 0.5, opa, Col, 100 );
		gradient->gradientValid = true;
	}
	if (gradient->gradientValid)
		gradient->gradient.filterStops();
}

void SVGPlug::parsePattern(const QDomElement &b)
{
	GradientHelper gradhelper;
	QString href = b.attribute("xlink:href").mid(1);
	if (!href.isEmpty())
	{
		if (m_gradients.contains(href))
		{
			gradhelper.Type = m_gradients[href].Type;
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
		int ac = m_Doc->Items->count();
		QList<PageItem*> GElements;
		GElements = parseGroup( b );
		int ae = m_Doc->Items->count();
		if (GElements.count() > 0)
		{
			ScPattern pat = ScPattern();
			pat.setDoc(m_Doc);
			PageItem* currItem = GElements.at(0);
			m_Doc->DoDrawing = true;
			pat.pattern = currItem->DrawObj_toImage();
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
			m_Doc->DoDrawing = false;
			pat.width = qMin(currItem->gWidth, wpat);
			pat.height = qMin(currItem->gHeight, hpat);
			bool more = false;
			for (int as = ac; as < ae; ++as)
			{
				PageItem* Neu = m_Doc->Items->takeAt(ac);
				if (more)
				{
					Neu->moveBy(xOrg, yOrg, true);
					Neu->gXpos += xOrg;
					Neu->gYpos += yOrg;
				}
				more = true;
				Neu->ItemNr = pat.items.count();
				pat.items.append(Neu);
			}
			m_Doc->addPattern(id, pat);
			importedPatterns.append(id);
			importedPattTrans.insert(origName, id);
		}
		m_nodeMap.insert(origName, b);
		QString transf = b.attribute("patternTransform");
		if( !transf.isEmpty() )
		{
			gradhelper.matrix = parseTransform( b.attribute("patternTransform") );
			gradhelper.matrixValid = true;
		}
		else
			gradhelper.matrixValid = false;
		gradhelper.gradientValid = true;
		gradhelper.gradient.clearStops();
		gradhelper.gradient.setRepeatMethod( VGradient::none );
		gradhelper.Type = 8;
		gradhelper.typeValid = true;
		m_gradients.insert(origName, gradhelper);
	}
}

void SVGPlug::parseGradient( const QDomElement &e )
{
	GradientHelper gradhelper;
	gradhelper.gradientValid = false;
	gradhelper.gradient.clearStops();
	gradhelper.gradient.setRepeatMethod( VGradient::none );

	QString href = e.attribute("xlink:href").mid(1);
	double x1=0, y1=0, x2=0, y2=0;
	if (!href.isEmpty())
	{
		if (m_gradients.contains(href))
		{
			gradhelper.Type = m_gradients[href].Type;
			gradhelper.gradient = m_gradients[href].gradient;
			gradhelper.X1 = m_gradients[href].X1;
			gradhelper.Y1 = m_gradients[href].Y1;
			gradhelper.X2 = m_gradients[href].X2;
			gradhelper.Y2 = m_gradients[href].Y2;
			gradhelper.CSpace = m_gradients[href].CSpace;
			gradhelper.matrix = m_gradients[href].matrix;
			gradhelper.x1Valid = m_gradients[href].x1Valid;
			gradhelper.x2Valid = m_gradients[href].x2Valid;
			gradhelper.y1Valid = m_gradients[href].y1Valid;
			gradhelper.y2Valid = m_gradients[href].y2Valid;
			gradhelper.cspaceValid = m_gradients[href].cspaceValid;
			gradhelper.matrixValid = m_gradients[href].matrixValid;
			gradhelper.gradientValid = m_gradients[href].gradientValid;
			gradhelper.typeValid = m_gradients[href].typeValid;
		}
		gradhelper.reference = href;
	}
	if (e.tagName() == "linearGradient")
	{
		if (e.hasAttribute("x1"))
		{
			gradhelper.X1 = parseUnit(e.attribute("x1", "0"));
			gradhelper.x1Valid = true;
		}
		if (e.hasAttribute("y1"))
		{
			gradhelper.Y1 = parseUnit(e.attribute("y1", "0"));
			gradhelper.y1Valid = true;
		}
		if (e.hasAttribute("x2"))
		{
			gradhelper.X2 = parseUnit(e.attribute("x2", "1"));
			gradhelper.x2Valid = true;
		}
		if (e.hasAttribute("y2"))
		{
			gradhelper.Y2 = parseUnit(e.attribute("y2", "0"));
			gradhelper.y2Valid = true;
		}
		gradhelper.Type = 6;
		gradhelper.typeValid = true;
	}
	else
	{
		if (e.hasAttribute("cx"))
		{
			x1 = parseUnit(e.attribute("cx","0.5"));
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
		y2 = y1;
		gradhelper.y2Valid = true;
		gradhelper.X1 = x1;
		gradhelper.Y1 = y1;
		gradhelper.X2 = x1 + x2;
		gradhelper.Y2 = y1;
		gradhelper.Type = 7;
		gradhelper.typeValid = true;
	}
	if ( !e.attribute( "gradientUnits" ).isEmpty() )
	{
		QString uni = e.attribute( "gradientUnits");
		if (uni == "userSpaceOnUse")
			gradhelper.CSpace = true;
		else
			gradhelper.CSpace = false;
		gradhelper.cspaceValid = true;
	}
	else
	{
		gradhelper.CSpace = false;
		gradhelper.cspaceValid = false;
	}
	QString transf = e.attribute("gradientTransform");
	if( !transf.isEmpty() )
	{
		gradhelper.matrix = parseTransform( e.attribute("gradientTransform") );
		gradhelper.matrixValid = true;
	}
	else
		gradhelper.matrixValid = false;
	QString spreadMethod = e.attribute( "spreadMethod" );
	if( !spreadMethod.isEmpty() )
	{
		if( spreadMethod == "reflect" )
			gradhelper.gradient.setRepeatMethod( VGradient::reflect );
		else if( spreadMethod == "repeat" )
			gradhelper.gradient.setRepeatMethod( VGradient::repeat );
	}
	parseColorStops(&gradhelper, e);
	m_gradients.insert(e.attribute("id"), gradhelper);
}

SVGPlug::~SVGPlug()
{
	delete tmpSel;
}
