/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "svgplugin.h"
#include "svgplugin.moc"

#include "scconfig.h"

#include "customfdialog.h"
#include "color.h"
#include "scribus.h"
#include "scribusXml.h"
#include "mpalette.h"
#include "prefsfile.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qdragobject.h>
#include <qregexp.h>
#include <qcursor.h>
#include <cmath>
#ifdef HAVE_LIBZ
#include <zlib.h>
#endif
#include "commonstrings.h"
#include "fpointarray.h"
#include "menumanager.h"
#include "prefsmanager.h"
#include "pageitem.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"
#include "loadsaveplugin.h"
#include "util.h"
#include "fonts/scfontmetrics.h"
#include "sccolorengine.h"

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
	importAction(new ScrAction(ScrAction::DLL, QIconSet(), "", QKeySequence(), this, "ImportSVG"))
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file format
	// support.
	languageChange();
}

void SVGImportPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect( importAction, SIGNAL(activated()), SLOT(import()) );
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}

SVGImportPlugin::~SVGImportPlugin()
{
	unregisterAll();
};

void SVGImportPlugin::languageChange()
{
	importAction->setMenuText( tr("Import &SVG..."));
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
	QString svgName = tr("Scalable Vector Graphics");
	FileFormat fmt(this);
	fmt.trName = svgName;
	fmt.formatId = FORMATID_SVGIMPORT;
#ifdef HAVE_LIBZ
	fmt.filter = svgName + " (*.svg *.SVG *.svgz *.SVGZ)";
	fmt.nameMatch = QRegExp("\\.(svg|svgz)$", false);
#else
	fmt.filter = svgName + " (*.svg *.SVG)";
	fmt.nameMatch = QRegExp("\\.svg$", false);
#endif
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = QStringList("image/svg+xml");
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
#ifdef HAVE_LIBZ
		CustomFDialog diaf(mw, wdir, QObject::tr("Open"), QObject::tr("SVG-Images (*.svg *.svgz);;All Files (*)"));
#else
		CustomFDialog diaf(mw, wdir, QObject::tr("Open"), QObject::tr("SVG-Images (*.svg);;All Files (*)"));
#endif
		if (diaf.exec())
		{
			filename = diaf.selectedFile();
			prefs->set("wdir", filename.left(filename.findRev("/")));
		}
		else
			return true;
	}
	
	if (UndoManager::undoEnabled() && m_Doc)
	{
		UndoManager::instance()->beginTransaction(m_Doc->currentPage()->getUName(),Um::IImageFrame,Um::ImportSVG, filename, Um::ISVG);
	}
	else if (UndoManager::undoEnabled() && !m_Doc)
		UndoManager::instance()->setUndoEnabled(false);
	SVGPlug *dia = new SVGPlug(mw, filename, flags);
	Q_CHECK_PTR(dia);
	if (UndoManager::undoEnabled())
		UndoManager::instance()->commit();
	else
		UndoManager::instance()->setUndoEnabled(true);

	if (dia->unsupported)
	{
		QMessageBox::warning(mw, CommonStrings::trWarning, tr("SVG file contains some unsupported features"), 1, 0, 0);
	}

	delete dia;
	return true;
}

SVGPlug::SVGPlug( ScribusMainWindow* mw, QString fName, int flags ) :
	QObject(mw)
{
	m_Doc=mw->doc;
	unsupported = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	QString f = "";
#ifdef HAVE_LIBZ
	if(fName.right(2) == "gz")
	{
		gzFile gzDoc;
		char buff[4097];
		int i;
		gzDoc = gzopen(fName.latin1(),"rb");
		if(gzDoc == NULL)
			return;
		while((i = gzread(gzDoc,&buff,4096)) > 0)
		{
			buff[i] = '\0';
			f.append(buff);
		}
		gzclose(gzDoc);
	}
	else
		loadText(fName, &f);
#else
	loadText(fName, &f);
#endif
	if(!inpdoc.setContent(f))
		return;
	m_gc.setAutoDelete( true );
	QString CurDirP = QDir::currentDirPath();
	QFileInfo efp(fName);
	QDir::setCurrent(efp.dirPath());
	convert(flags);
	QDir::setCurrent(CurDirP);
}

void SVGPlug::convert(int flags)
{
	bool ret = false;
	SvgStyle *gc = new SvgStyle;
	Conversion = 0.8;
	QDomElement docElem = inpdoc.documentElement();
	double width = !docElem.attribute("width").isEmpty() ? parseUnit(docElem.attribute( "width" )) : 550.0;
	double height = !docElem.attribute("height").isEmpty() ? parseUnit(docElem.attribute( "height" )) : 841.0;
	Conversion = 0.8;
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
	m_Doc->view()->Deselect();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	gc->Family = m_Doc->toolSettings.defFont;
	if (!m_Doc->PageColors.contains("Black"))
		m_Doc->PageColors.insert("Black", ScColor(0, 0, 0, 255));
	m_gc.push( gc );
	viewTransformX = 0;
	viewTransformY = 0;
	viewScaleX = 1;
	viewScaleY = 1;
	if( !docElem.attribute( "viewBox" ).isEmpty() )
	{
		Conversion = 1.0;
		double w2 = !docElem.attribute("width").isEmpty() ? parseUnit(docElem.attribute( "width" )) : 550.0;
		double h2 = !docElem.attribute("height").isEmpty() ? parseUnit(docElem.attribute( "height" )) : 841.0;
		Conversion = 0.8;
		QWMatrix matrix;
		addGraphicContext();
		QString viewbox( docElem.attribute( "viewBox" ) );
		QStringList points = QStringList::split( ' ', viewbox.replace( QRegExp(","), " ").simplifyWhiteSpace() );
		viewTransformX = points[0].toDouble();
		viewTransformY = points[1].toDouble();
		viewScaleX = w2 / points[2].toDouble();
		viewScaleY = h2 / points[3].toDouble();
		matrix.translate(viewTransformX, viewTransformY);
		matrix.scale(viewScaleX, viewScaleY);
		m_gc.current()->matrix = matrix;
	}
	QPtrList<PageItem> Elements = parseGroup( docElem );
	m_Doc->m_Selection->clear();
	if (Elements.count() > 1)
	{
		bool isGroup = true;
		int firstElem = -1;
		if (Elements.at(0)->Groups.count() != 0)
			firstElem = Elements.at(0)->Groups.top();
		for (uint bx = 0; bx < Elements.count(); ++bx)
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
			for (uint a = 0; a < Elements.count(); ++a)
			{
				Elements.at(a)->Groups.push(m_Doc->GroupCounter);
				PageItem* currItem = Elements.at(a);
				lowestItem = QMIN(lowestItem, currItem->ItemNr);
				highestItem = QMAX(highestItem, currItem->ItemNr);
				double lw = currItem->lineWidth() / 2.0;
				if (currItem->rotation() != 0)
				{
					FPointArray pb;
					pb.resize(0);
					pb.addPoint(FPoint(currItem->xPos()-lw, currItem->yPos()-lw));
					pb.addPoint(FPoint(currItem->width()+lw*2.0, -lw, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					pb.addPoint(FPoint(currItem->width()+lw*2.0, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					pb.addPoint(FPoint(-lw, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					for (uint pc = 0; pc < 4; ++pc)
					{
						minx = QMIN(minx, pb.point(pc).x());
						miny = QMIN(miny, pb.point(pc).y());
						maxx = QMAX(maxx, pb.point(pc).x());
						maxy = QMAX(maxy, pb.point(pc).y());
					}
				}
				else
				{
					minx = QMIN(minx, currItem->xPos()-lw);
					miny = QMIN(miny, currItem->yPos()-lw);
					maxx = QMAX(maxx, currItem->xPos()-lw + currItem->width()+lw*2.0);
					maxy = QMAX(maxy, currItem->yPos()-lw + currItem->height()+lw*2.0);
				}
			}
			double gx = minx;
			double gy = miny;
			double gw = maxx - minx;
			double gh = maxy - miny;
			PageItem *high = m_Doc->Items->at(highestItem);
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
			PageItem *neu = m_Doc->Items->take(z);
			m_Doc->Items->insert(lowestItem, neu);
			neu->Groups.push(m_Doc->GroupCounter);
			neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
			neu->isGroupControl = true;
			neu->groupsLastItem = high;
			for (uint a = 0; a < m_Doc->Items->count(); ++a)
			{
				m_Doc->Items->at(a)->ItemNr = a;
			}
			Elements.prepend(neu);
			m_Doc->GroupCounter++;
		}
	}
	m_Doc->DoDrawing = true;
	m_Doc->scMW()->ScriptRunning = false;
	if (interactive)
		m_Doc->setLoading(false);
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	if ((Elements.count() > 0) && (!ret) && (interactive))
	{
		m_Doc->DragP = true;
		m_Doc->DraggedElem = 0;
		m_Doc->DragElements.clear();
		for (uint dre=0; dre<Elements.count(); ++dre)
		{
			m_Doc->DragElements.append(Elements.at(dre)->ItemNr);
			m_Doc->m_Selection->addItem(Elements.at(dre), true);
		}
		ScriXmlDoc *ss = new ScriXmlDoc();
		m_Doc->view()->setGroupRect();
		//QDragObject *dr = new QTextDrag(ss->WriteElem(&m_Doc->view()->SelItem, m_Doc, m_Doc->view()), m_Doc->view()->viewport());
		QDragObject *dr = new QTextDrag(ss->WriteElem(m_Doc, m_Doc->view(), m_Doc->m_Selection), m_Doc->view());
#ifndef QT_MAC
// see #2526
		m_Doc->itemSelection_DeleteItem();
#endif
		m_Doc->view()->resizeContents(qRound((maxSize.x() - minSize.x()) * m_Doc->view()->scale()), qRound((maxSize.y() - minSize.y()) * m_Doc->view()->scale()));
		m_Doc->view()->scrollBy(qRound((m_Doc->minCanvasCoordinate.x() - minSize.x()) * m_Doc->view()->scale()), qRound((m_Doc->minCanvasCoordinate.y() - minSize.y()) * m_Doc->view()->scale()));
		m_Doc->minCanvasCoordinate = minSize;
		m_Doc->maxCanvasCoordinate = maxSize;
		m_Doc->view()->updatesOn(true);
		dr->setPixmap(loadIcon("DragPix.xpm"));
		if (!dr->drag())
			qDebug("svgimport: could not start drag operation!");
		delete ss;
		m_Doc->DragP = false;
		m_Doc->DraggedElem = 0;
		m_Doc->DragElements.clear();
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
	if ( m_gc.current() )
		*gc = *( m_gc.current() );
	m_gc.push( gc );
}

void SVGPlug::setupTransform( const QDomElement &e )
{
	SvgStyle *gc = m_gc.current();
	QWMatrix mat = parseTransform( e.attribute( "transform" ) );
	if (!e.attribute("transform").isEmpty())
		gc->matrix = mat * gc->matrix;
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
		if (!svgStyle.Display) 
			continue;
		QString STag2 = b.tagName();
		if ( STag2 == "linearGradient" || STag2 == "radialGradient" )
			parseGradient( b );
		else if ( b.hasAttribute("id") )
		{
			QString id = b.attribute("id");
			if (!id.isEmpty())
				m_nodeMap.insert(id, b);
		}
	}
}

QPtrList<PageItem> SVGPlug::parseGroup(const QDomElement &e)
{
	QPtrList<PageItem> GElements;
	FPointArray ImgClip;
	ImgClip.resize(0);
//	double BaseX = m_Doc->currentPage()->xOffset();
//	double BaseY = m_Doc->currentPage()->yOffset();
	for ( QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement b = n.toElement();
		if( b.isNull() )
			continue;
		SvgStyle svgStyle;
		parseStyle( &svgStyle, b );
		if (!svgStyle.Display) 
			continue;
		QPtrList<PageItem> el = parseElement(b);
		for (uint ec = 0; ec < el.count(); ++ec)
			GElements.append(el.at(ec));
	}
	return GElements;
}

QPtrList<PageItem> SVGPlug::parseElement(const QDomElement &e)
{
	int z = -1;
	QPtrList<PageItem> GElements;
	FPointArray ImgClip;
	ImgClip.resize(0);
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	if (e.hasAttribute("id"))
		m_nodeMap.insert(e.attribute("id"), e);
	QString STag = e.tagName();
	if( STag == "g" )
	{
		addGraphicContext();
		SvgStyle *gc = m_gc.current();
		setupTransform( e );
		parseStyle( gc, e );
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, 1, 1, 0, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
		PageItem *neu = m_Doc->Items->at(z);
		GElements.append(neu);
		QPtrList<PageItem> gElements = parseGroup( e );
		if (gElements.count() == 0)
		{
			GElements.removeLast();
			m_Doc->Items->take(z);
			delete neu;
		}
		else
		{
			double minx = 99999.9;
			double miny = 99999.9;
			double maxx = -99999.9;
			double maxy = -99999.9;
			for (uint gr = 0; gr < gElements.count(); ++gr)
			{
				PageItem* currItem = gElements.at(gr);
				double lw = currItem->lineWidth() / 2.0;
				if (currItem->rotation() != 0)
				{
					FPointArray pb;
					pb.resize(0);
					pb.addPoint(FPoint(currItem->xPos()-lw, currItem->yPos()-lw));
					pb.addPoint(FPoint(currItem->width()+lw*2.0, -lw, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					pb.addPoint(FPoint(currItem->width()+lw*2.0, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					pb.addPoint(FPoint(-lw, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
					for (uint pc = 0; pc < 4; ++pc)
					{
						minx = QMIN(minx, pb.point(pc).x());
						miny = QMIN(miny, pb.point(pc).y());
						maxx = QMAX(maxx, pb.point(pc).x());
						maxy = QMAX(maxy, pb.point(pc).y());
					}
				}
				else
				{
					minx = QMIN(minx, currItem->xPos()-lw);
					miny = QMIN(miny, currItem->yPos()-lw);
					maxx = QMAX(maxx, currItem->xPos()-lw + currItem->width()+lw*2.0);
					maxy = QMAX(maxy, currItem->yPos()-lw + currItem->height()+lw*2.0);
				}
			}
			double gx = minx;
			double gy = miny;
			double gw = maxx - minx;
			double gh = maxy - miny;
			neu->setXYPos(gx, gy);
			neu->setWidthHeight(gw, gh);
			if (ImgClip.size() != 0)
				neu->PoLine = ImgClip.copy();
			else
				neu->SetRectFrame();
			ImgClip.resize(0);
			neu->Clip = FlattenPath(neu->PoLine, neu->Segments);
			neu->Groups.push(m_Doc->GroupCounter);
			neu->isGroupControl = true;
			neu->groupsLastItem = gElements.at(gElements.count()-1);
			if( !e.attribute("id").isEmpty() )
				neu->setItemName(e.attribute("id"));
			else
				neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
			neu->setFillTransparency(1 - gc->Opacity);
			for (uint gr = 0; gr < gElements.count(); ++gr)
			{
				gElements.at(gr)->Groups.push(m_Doc->GroupCounter);
				GElements.append(gElements.at(gr));
			}
		}
		delete( m_gc.pop() );
		m_Doc->GroupCounter++;
		return GElements;
	}
	if( e.tagName() == "defs" )
	{
		parseDefs(e);
		return GElements;
	}
	else if( e.tagName() == "switch" )
	{
		GElements = parseSwitch(e);
		return GElements;
	}
	else if( e.tagName() == "use" )
	{
		GElements = parseUse(e);
		return GElements;
	}
	else if( STag == "linearGradient" || STag == "radialGradient" )
	{
		parseGradient( e );
		return GElements;
	}
	else if( STag == "rect" )
	{
		addGraphicContext();
		double x = parseUnit( e.attribute( "x" ) );
		double y = parseUnit( e.attribute( "y" ) );
		double width = parseUnit( e.attribute( "width" ));
		double height = parseUnit( e.attribute( "height" ) );
		double rx = e.attribute( "rx" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "rx" ) );
		double ry = e.attribute( "ry" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "ry" ) );
		SvgStyle *gc = m_gc.current();
		parseStyle( gc, e );
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, width, height, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		PageItem* ite = m_Doc->Items->at(z);
		if ((rx != 0) || (ry != 0))
		{
			ite->setCornerRadius(QMAX(rx, ry));
			ite->SetFrameRound();
			m_Doc->setRedrawBounding(ite);
		}
		QWMatrix mm = QWMatrix();
		mm.translate(x, y);
		ite->PoLine.map(mm);
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(), wh.y());
	}
	else if( STag == "ellipse" )
	{
		addGraphicContext();
		double rx = parseUnit( e.attribute( "rx" ) );
		double ry = parseUnit( e.attribute( "ry" ) );
		double x = parseUnit( e.attribute( "cx" ) ) - rx;
		double y = parseUnit( e.attribute( "cy" ) ) - ry;
		SvgStyle *gc = m_gc.current();
		parseStyle( gc, e );
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, rx * 2.0, ry * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		PageItem* ite = m_Doc->Items->at(z);
		QWMatrix mm = QWMatrix();
		mm.translate(x, y);
		ite->PoLine.map(mm);
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(), wh.y());
	}
	else if( STag == "circle" )
	{
		addGraphicContext();
		double r = parseUnit( e.attribute( "r" ) );
		double x = parseUnit( e.attribute( "cx" ) ) - r;
		double y = parseUnit( e.attribute( "cy" ) ) - r;
		SvgStyle *gc = m_gc.current();
		parseStyle( gc, e );
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, r * 2.0, r * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		PageItem* ite = m_Doc->Items->at(z);
		QWMatrix mm = QWMatrix();
		mm.translate(x, y);
		ite->PoLine.map(mm);
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(), wh.y());
	}
	else if( STag == "line" )
	{
		addGraphicContext();
		double x1 = e.attribute( "x1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "x1" ) );
		double y1 = e.attribute( "y1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "y1" ) );
		double x2 = e.attribute( "x2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "x2" ) );
		double y2 = e.attribute( "y2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "y2" ) );
		SvgStyle *gc = m_gc.current();
		parseStyle( gc, e );
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine.resize(4);
		ite->PoLine.setPoint(0, FPoint(x1, y1));
		ite->PoLine.setPoint(1, FPoint(x1, y1));
		ite->PoLine.setPoint(2, FPoint(x2, y2));
		ite->PoLine.setPoint(3, FPoint(x2, y2));
	}
	else if( STag == "path" )
	{
		addGraphicContext();
		SvgStyle *gc = m_gc.current();
		parseStyle( gc, e );
		FPointArray pArray;
		PageItem::ItemType itype = parseSVG(e.attribute("d"), &pArray) ? PageItem::PolyLine : PageItem::Polygon; 
		z = m_Doc->itemAdd(itype, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->fillRule = (gc->fillRule != "nonzero"); 
		ite->PoLine = pArray;
		if (ite->PoLine.size() < 4)
		{
			m_Doc->m_Selection->addItem(ite);
			m_Doc->itemSelection_DeleteItem();
			z = -1;
		}
	}
	else if( STag == "polyline" || e.tagName() == "polygon" )
	{
		addGraphicContext();
		SvgStyle *gc = m_gc.current();
		parseStyle( gc, e );
		if( e.tagName() == "polygon" )
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->fillRule = (gc->fillRule != "nonzero"); 
		ite->PoLine.resize(0);
		bool bFirst = true;
		double x = 0.0;
		double y = 0.0;
		QString points = e.attribute( "points" ).simplifyWhiteSpace().replace(',', " ");
		/*
		points.replace( QRegExp( "," ), " " );
		points.replace( QRegExp( "\r" ), "" );
		points.replace( QRegExp( "\n" ), "" );
		*/
		QStringList pointList = QStringList::split( ' ', points );
		FirstM = true;
		for( QStringList::Iterator it = pointList.begin(); it != pointList.end(); it++ )
		{
			if( bFirst )
			{
				x = (*(it++)).toDouble();
				y = (*it).toDouble();
				svgMoveTo(x * Conversion, y * Conversion);
				bFirst = false;
				WasM = true;
			}
			else
			{
				x = (*(it++)).toDouble();
				y = (*it).toDouble();
				svgLineTo(&ite->PoLine, x * Conversion, y * Conversion);
			}
		}
		if( STag == "polygon" )
			svgClosePath(&ite->PoLine);
		else
			ite->convertTo(PageItem::PolyLine);
	}
	else if( STag == "text" )
	{
		addGraphicContext();
		SvgStyle *gc = m_gc.current();
		parseStyle(gc, e);
		QPtrList<PageItem> el = parseText(e);
		for (uint ec = 0; ec < el.count(); ++ec)
		{
			GElements.append(el.at(ec));
		}
		z = -1;
	}
/*	else if( STag == "clipPath" )
	{
		QDomNode n2 = e.firstChild();
		QDomElement b2 = n2.toElement();
		parseSVG( b2.attribute( "d" ), &ImgClip );
		return GElements;
	}
	else if( STag == "image" )
	{
		addGraphicContext();
		QString fname = e.attribute("xlink:href");
		double x = e.attribute( "x" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "x" ) );
		double y = e.attribute( "y" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "y" ) );
		double w = e.attribute( "width" ).isEmpty() ? 1.0 : parseUnit( e.attribute( "width" ) );
		double h = e.attribute( "height" ).isEmpty() ? 1.0 : parseUnit( e.attribute( "height" ) );
		//z = m_Doc->view()->PaintPict(x+BaseX, y+BaseY, w, h);
		z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x+BaseX, y+BaseY, w, h, 1, m_Doc->toolSettings.dBrushPict, CommonStrings::None, true);
		if (!fname.isEmpty())
			m_Doc->LoadPict(fname, z);
		PageItem* ite = m_Doc->Items->at(z);
		if (ImgClip.size() != 0)
			ite->PoLine = ImgClip.copy();
		ImgClip.resize(0);
		ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	} */
	else
	{
		// warn if unsupported SVG feature are encountered
		qDebug(QString("unsupported SVG feature: %1").arg(STag));
		unsupported = true;
		return GElements;
	}
	if (z != -1)
	{
		setupTransform( e );
		SvgStyle *gc = m_gc.current();
		PageItem* ite = m_Doc->Items->at(z);
		QWMatrix gcm = gc->matrix;
		double coeff1 = sqrt(gcm.m11() * gcm.m11() + gcm.m12() * gcm.m12());
		double coeff2 = sqrt(gcm.m21() * gcm.m21() + gcm.m22() * gcm.m22());
		switch (ite->itemType())
		{
		case PageItem::ImageFrame:
			{
				QWMatrix mm = gc->matrix;
				ite->moveBy(mm.dx(), mm.dy());
				ite->setWidthHeight(ite->width() * mm.m11(), ite->height() * mm.m22());
				ite->setLineWidth(ite->lineWidth() * (coeff1 + coeff2) / 2.0);
				if (ite->PicAvail)
					ite->setImageXYScale(ite->width() / ite->pixm.width(), ite->height() / ite->pixm.height());
				break;
			}
		case PageItem::TextFrame:
			{
				QWMatrix mm = gc->matrix;
				ite->setLineWidth(ite->lineWidth() * (coeff1 + coeff2) / 2.0);
			}
			break;
		default:
			{
				ite->ClipEdited = true;
				ite->FrameType = 3;
				QWMatrix mm = gc->matrix;
				ite->PoLine.map(mm);
				/*if (haveViewBox)
				{
					QWMatrix mv;
					mv.translate(viewTransformX, viewTransformY);
					mv.scale(viewScaleX, viewScaleY);
					ite->PoLine.map(mv);
				}*/
				ite->setLineWidth(ite->lineWidth() * (coeff1 + coeff2) / 2.0);
				FPoint wh = getMaxClipF(&ite->PoLine);
				ite->setWidthHeight(wh.x(), wh.y());
				ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
				m_Doc->view()->AdjustItemSize(ite);
				break;
			}
		}
		ite->OwnPage = m_Doc->OnPage(ite);
		if( !e.attribute("id").isEmpty() )
			ite->setItemName(" "+e.attribute("id"));
		ite->setFillTransparency( 1 - gc->FillOpacity * gc->Opacity );
		ite->setLineTransparency( 1 - gc->StrokeOpacity * gc->Opacity );
		ite->PLineEnd = gc->PLineEnd;
		ite->PLineJoin = gc->PLineJoin;
		//ite->setTextFlowsAroundFrame(false);
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		ite->DashOffset = gc->dashOffset;
		ite->DashValues = gc->dashArray;
		if (gc->Gradient != 0)
		{
			if (gc->GradCo.Stops() > 1)
			{
				ite->fill_gradient = gc->GradCo;
				if (!gc->CSpace)
				{
					ite->GrStartX = gc->GX1 * ite->width();
					ite->GrStartY = gc->GY1 * ite->height();
					ite->GrEndX = gc->GX2 * ite->width();
					ite->GrEndY = gc->GY2 * ite->height();
					double angle1 = atan2(gc->GY2-gc->GY1,gc->GX2-gc->GX1)*(180.0/M_PI);
					double angle2 = atan2(ite->GrEndY-ite->GrStartX,ite->GrEndX-ite->GrStartX)*(180.0/M_PI);
					double dx = ite->GrStartX + (ite->GrEndX-ite->GrStartX) / 2.0;
					double dy = ite->GrStartY + (ite->GrEndY-ite->GrStartY) / 2.0;
					QWMatrix mm, mm2;
					if ((gc->GY1 < gc->GY2) && (gc->GX1 < gc->GX2))
					{
						mm.rotate(-angle2);
						mm2.rotate(angle1);
					}
					FPointArray gra;
					gra.setPoints(2, ite->GrStartX-dx, ite->GrStartY-dy, ite->GrEndX-dx, ite->GrEndY-dy);
					gra.map(mm*mm2);
					gra.translate(dx, dy);
					ite->GrStartX = gra.point(0).x();
					ite->GrStartY = gra.point(0).y();
					ite->GrEndX = gra.point(1).x();
					ite->GrEndY = gra.point(1).y();
				}
				else
				{
					QWMatrix mm = gc->matrix;
					mm = gc->matrixg * mm;
					FPointArray gra;
					gra.setPoints(2, gc->GX1, gc->GY1, gc->GX2, gc->GY2);
					gra.map(mm);
					gc->GX1 = gra.point(0).x();
					gc->GY1 = gra.point(0).y();
					gc->GX2 = gra.point(1).x();
					gc->GY2 = gra.point(1).y();
					ite->GrStartX = gc->GX1 - ite->xPos() + BaseX;
					ite->GrStartY = gc->GY1 - ite->yPos() + BaseY;
					ite->GrEndX = gc->GX2 - ite->xPos() + BaseX;
					ite->GrEndY = gc->GY2 - ite->yPos() + BaseY;
				}
				ite->GrType = gc->Gradient;
			}
			else
			{
				ite->GrType = 0;
				QPtrVector<VColorStop> cstops = gc->GradCo.colorStops();
				ite->setFillColor(cstops.at(0)->name);
				ite->setFillShade(cstops.at(0)->shade);
			}
		}
		GElements.append(ite);
	}
	delete( m_gc.pop() );
	return GElements;
}

QPtrList<PageItem> SVGPlug::parseText(const QDomElement &e)
{
	QPtrList<PageItem> GElements;
	setupTransform(e);
	QDomNode c = e.firstChild();
//	double BaseX = m_Doc->currentPage()->xOffset();
//	double BaseY = m_Doc->currentPage()->yOffset();
	double x = e.attribute( "x" ).isEmpty() ? 0.0 : parseUnit(e.attribute("x"));
	double y = e.attribute( "y" ).isEmpty() ? 0.0 : parseUnit(e.attribute("y"));
	if ((!c.isNull()) && (c.toElement().tagName() == "tspan"))
	{
		for(QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
		{
			QDomElement tspan = n.toElement();
			addGraphicContext();
			SvgStyle *gc = m_gc.current();
			parseStyle(gc, tspan);
			if (!gc->Display)
				continue;
			QPtrList<PageItem> el = parseTextElement(x, y, tspan);
			for (uint ec = 0; ec < el.count(); ++ec)
				GElements.append(el.at(ec));
			delete( m_gc.pop() );
		}
	}
	else
	{
//		SvgStyle *gc = m_gc.current();
		QPtrList<PageItem> el = parseTextElement(x, y, e);
		for (uint ec = 0; ec < el.count(); ++ec)
			GElements.append(el.at(ec));
	}
	return GElements;
}

QPtrList<PageItem> SVGPlug::parseTextElement(double x, double y, const QDomElement &e)
{
	QPtrList<PageItem> GElements;
//	QFont ff(m_Doc->UsedFonts[m_gc.current()->Family]);
	QFont ff(m_gc.current()->Family);
	ff.setPointSize(QMAX(qRound(m_gc.current()->FontSize / 10.0), 1));
	QFontMetrics fontMetrics(ff);
	int desc = fontMetrics.descent();
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	QString Text = QString::fromUtf8(e.text()).stripWhiteSpace();
	QDomNode c = e.firstChild();
	if ( e.tagName() == "tspan" && e.text().isNull() )
			Text = " ";

	double maxWidth = 0, maxHeight = 0;
	double tempW = 0, tempH = 0;
	SvgStyle *gc = m_gc.current();
	double ity = (e.tagName() == "tspan") ? y : (y - qRound(gc->FontSize / 10.0));
	int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, ity, 10, 10, gc->LWidth, CommonStrings::None, gc->FillCol, true);
	PageItem* ite = m_Doc->Items->at(z);
	ite->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
//FIXME:av			ite->setLineSpacing(gc->FontSize / 10.0 + 2);
	const double lineSpacing = gc->FontSize / 10.0 + 2;
	ite->setHeight(lineSpacing +desc+2);
	m_Doc->scMW()->SetNewFont(gc->Family);
	QWMatrix mm = gc->matrix;
	double scalex = sqrt(mm.m11() * mm.m11() + mm.m12() * mm.m12());
	double scaley = sqrt(mm.m21() * mm.m21() + mm.m22() * mm.m22());
	if( (!e.attribute("x").isEmpty()) && (!e.attribute("y").isEmpty()) )
	{
		double x1 = parseUnit( e.attribute( "x", "0" ) );
		double y1 = parseUnit( e.attribute( "y", "0" ) );
		double mx = mm.m11() * x1 + mm.m21() * y1 + mm.dx();
		double my = mm.m12() * x1 + mm.m22() * y1 + mm.dy();
		ite->setXPos(mx + BaseX);
		ite->setYPos(my + BaseY);
	}
	else
	{
		double mx = mm.m11() * x + mm.m21() * y + mm.dx();
		double my = mm.m12() * x + mm.m22() * y + mm.dy();
		ite->setXPos(mx + BaseX);
		ite->setYPos(my + BaseY);
	}
	ite->setFillColor(CommonStrings::None);
	ite->setLineColor(CommonStrings::None);
	/*
	ite->setFont(gc->Family);
	ite->TxtFill = gc->FillCol;
	ite->ShTxtFill = 100;
	ite->TxtStroke = gc->StrokeCol;
	ite->ShTxtStroke = 100;
	ite->setFontSize(gc->FontSize);
	ite->TxTStyle = 0;
	ite->TxtScale = 1000;
	ite->TxtScaleV = 1000;
	ite->TxtBase = 0;
	ite->TxtShadowX = 50;
	ite->TxtShadowY = -50;
	ite->TxtOutline = 10;
	ite->TxtUnderPos = -1;
	ite->TxtUnderWidth = -1;
	ite->TxtStrikePos = -1;
	ite->TxtStrikeWidth = -1;
		*/
	for (uint tt = 0; tt < Text.length(); ++tt)
	{
		CharStyle nstyle;
		QString ch = Text.mid(tt,1);
		nstyle.setFont((*m_Doc->AllFonts)[gc->Family]);
		nstyle.setFontSize(gc->FontSize);
		nstyle.setFillColor(gc->FillCol);
		nstyle.setTracking(0);
		nstyle.setFillShade(100);
		nstyle.setStrokeColor(gc->StrokeCol);
		nstyle.setStrokeShade(100);
		nstyle.setScaleH(1000);
		nstyle.setScaleV(1000);
		nstyle.setBaselineOffset(0);
		nstyle.setShadowXOffset(50);
		nstyle.setShadowYOffset(-50);
		nstyle.setOutlineWidth(10);
		nstyle.setUnderlineOffset(-1);
		nstyle.setUnderlineWidth(-1);
		nstyle.setStrikethruOffset(-1);
		nstyle.setStrikethruWidth(-1);
		if( !e.attribute( "stroke" ).isEmpty() )
			nstyle.setEffects(ScStyle_Outline);
		else
			nstyle.setEffects(ScStyle_Default);
		int pos = ite->itemText.length();
		ite->itemText.insertChars(pos, ch);
		ite->itemText.applyCharStyle(pos, 1, nstyle);
		tempW += nstyle.font().realCharWidth(ch[0], nstyle.fontSize() / 10.0)+1;
		tempH  = nstyle.font().realCharHeight(ch[0], nstyle.fontSize() / 10.0);
		maxWidth  = (tempW > maxWidth) ? tempW : maxWidth;
		maxHeight = (tempH > maxHeight) ? tempH : maxHeight;
		if (ch == SpecialChars::PARSEP)
		{
			ite->setWidthHeight(QMAX(ite->width(), tempW), ite->height() + lineSpacing+desc);
			tempW = 0;
		}
	}
	double xpos = ite->xPos();
	double ypos = ite->yPos();
	ite->setWidthHeight(QMAX(ite->width(), maxWidth), QMAX(ite->height(), maxHeight));
	double xoffset = 0.0, yoffset = 0.0;
	if( gc->textAnchor == "middle" )
	{
		m_Doc->m_Selection->clear();
		m_Doc->m_Selection->addItem(ite, true);
		m_Doc->itemSelection_SetAlignment(1);
		xoffset = -ite->width() / 2;
	}
	else if( gc->textAnchor == "end")
	{
		m_Doc->m_Selection->clear();
		m_Doc->m_Selection->addItem(ite, true);
		m_Doc->itemSelection_SetAlignment(2);
		xoffset = -ite->width();
	}
	double rotation = getRotationFromMatrix(gc->matrix, 0.0);
	if (rotation != 0.0)
	{
		double temp = xoffset;
		xoffset = cos(-rotation) * temp;
		yoffset = sin(-rotation) * temp;
	}
	ite->setXPos(xpos + xoffset);
	ite->setYPos(ypos + yoffset);
	ite->setRotation(-rotation * 180 / M_PI);
	ite->SetRectFrame();
	m_Doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	m_Doc->m_Selection->addItem(ite);
	m_Doc->view()->frameResizeHandle = 1;
	m_Doc->view()->setGroupRect();
	m_Doc->view()->scaleGroup(scalex, scaley);
	// scaleGroup scale may modify position... weird...
	ite->setXYPos(xpos + xoffset, ypos + yoffset);
	m_Doc->view()->Deselect();
	// Probably some scalex and scaley to add somewhere
	ite->moveBy(maxHeight * sin(-rotation) * scaley, -maxHeight * cos(-rotation) * scaley);
	if( !e.attribute("id").isEmpty() )
		ite->setItemName(" "+e.attribute("id"));
	ite->setFillTransparency( 1 - gc->FillOpacity * gc->Opacity);
	ite->setLineTransparency( 1 - gc->StrokeOpacity * gc->Opacity);
	ite->PLineEnd = gc->PLineEnd;
	ite->PLineJoin = gc->PLineJoin;
	//ite->setTextFlowsAroundFrame(false);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->DashOffset = gc->dashOffset;
	ite->DashValues = gc->dashArray;
	/*			if (gc->Gradient != 0)
				{
					ite->fill_gradient = gc->GradCo;
					m_Doc->view()->SelItem.append(ite);
					m_Doc->view()->ItemGradFill(gc->Gradient, gc->GCol2, 100, gc->GCol1, 100);
					m_Doc->view()->SelItem.clear();
				} */
	GElements.append(ite);
	return GElements;
}

QPtrList<PageItem> SVGPlug::parseSwitch(const QDomElement &e)
{
	QString href;
	QStringList hrefs;
	QPtrList<PageItem> SElements;
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

QPtrList<PageItem> SVGPlug::parseUse(const QDomElement &e)
{
	QPtrList<PageItem> UElements;
	QDomElement ue = getNodeFromUseElement(e);
	if (!ue.isNull())
		UElements = parseElement(ue);
	return UElements;
}

QDomElement SVGPlug::getNodeFromUseElement(const QDomElement &e)
{
	QDomElement ret;
	QMap<QString, QDomElement>::Iterator it;
	QString href = e.attribute("xlink:href").mid(1);
	it = m_nodeMap.find(href);
	if (it != m_nodeMap.end())
	{
		QString attrName;
		QDomNode clone = it.data().cloneNode();
		QDomNamedNodeMap attributes = e.attributes();
		ret = clone.toElement();
		for (uint i = 0; i < attributes.count(); ++i)
		{
			QDomAttr attr = attributes.item(i).toAttr();
			attrName = attr.name();
			if (attrName == "transform")
			{
				QString trans = attr.value();
				if (ret.hasAttribute("transform"))
					trans += QString(" %1").arg(ret.attribute("transform"));
				ret.setAttribute(attr.name(), trans);
			}
			else
				ret.setAttribute(attr.name(), attr.value());
		}
	}
	return ret;
}

double SVGPlug::fromPercentage( const QString &s )
{
	if (s.endsWith( "%" ))
	{
		QString s1 = s.left(s.length() - 1);
		return s1.toDouble() / 100.0;
	}
	else
		return s.toDouble();
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
	double value = unitval.toDouble();
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
		value = value * Conversion;
	return value;
}

QWMatrix SVGPlug::parseTransform( const QString &transform )
{
	QWMatrix ret;
	// Split string for handling 1 transform statement at a time
	QStringList subtransforms = QStringList::split(')', transform);
	QStringList::ConstIterator it = subtransforms.begin();
	QStringList::ConstIterator end = subtransforms.end();
	for(; it != end; ++it)
	{
		QWMatrix result;
		QStringList subtransform = QStringList::split('(', (*it));
		subtransform[0] = subtransform[0].stripWhiteSpace().lower();
		subtransform[1] = subtransform[1].simplifyWhiteSpace();
		QRegExp reg("[,( ]");
		QStringList params = QStringList::split(reg, subtransform[1]);
		if(subtransform[0].startsWith(";") || subtransform[0].startsWith(","))
			subtransform[0] = subtransform[0].right(subtransform[0].length() - 1);
		if(subtransform[0] == "rotate")
		{
			if(params.count() == 3)
			{
				double x = params[1].toDouble() * Conversion;
				double y = params[2].toDouble() * Conversion;
				result.translate(x, y);
				result.rotate(params[0].toDouble());
				result.translate(-x, -y);
			}
			else
				result.rotate(params[0].toDouble());
		}
		else if(subtransform[0] == "translate")
		{
			if(params.count() == 2)
				result.translate(params[0].toDouble() * Conversion, params[1].toDouble() * Conversion);
			else    // Spec : if only one param given, assume 2nd param to be 0
				result.translate(params[0].toDouble() * Conversion, 0);
		}
		else if(subtransform[0] == "scale")
		{
			if(params.count() == 2)
				result.scale(params[0].toDouble(), params[1].toDouble());
			else    // Spec : if only one param given, assume uniform scaling
				result.scale(params[0].toDouble(), params[0].toDouble());
		}
		else if(subtransform[0] == "skewx")
			result.shear(tan(params[0].toDouble() * 0.01745329251994329576), 0.0F);
		else if(subtransform[0] == "skewy")
			result.shear(0.0F, tan(params[0].toDouble() * 0.01745329251994329576));
		else if(subtransform[0] == "matrix")
		{
			if(params.count() >= 6)
			{
				double sx = params[0].toDouble();
				double sy = params[3].toDouble();
				result.setMatrix(sx, params[1].toDouble(), params[2].toDouble(), sy, params[4].toDouble() * Conversion, params[5].toDouble() * Conversion);
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
	QString d = s;
	d = d.replace( QRegExp( "," ), " ");
	bool ret = false;
	if( !d.isEmpty() )
	{
		d = d.simplifyWhiteSpace();
		const char *ptr = d.latin1();
		const char *end = d.latin1() + d.length() + 1;
		double contrlx, contrly, curx, cury, subpathx, subpathy, tox, toy, x1, y1, x2, y2, xc, yc;
		double px1, py1, px2, py2, px3, py3;
		bool relative;
		FirstM = true;
		char command = *(ptr++), lastCommand = ' ';
		subpathx = subpathy = curx = cury = contrlx = contrly = 0.0;
		while( ptr < end )
		{
			if( *ptr == ' ' )
				ptr++;
			relative = false;
			switch( command )
			{
			case 'm':
				relative = true;
			case 'M':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					WasM = true;
					subpathx = curx = relative ? curx + tox : tox;
					subpathy = cury = relative ? cury + toy : toy;
					svgMoveTo(curx, cury );
					break;
				}
			case 'l':
				relative = true;
			case 'L':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'h':
				{
					ptr = getCoord( ptr, tox );
					tox *= Conversion;
					curx = curx + tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'H':
				{
					ptr = getCoord( ptr, tox );
					tox *= Conversion;
					curx = tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'v':
				{
					ptr = getCoord( ptr, toy );
					toy *= Conversion;
					cury = cury + toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'V':
				{
					ptr = getCoord( ptr, toy );
					toy *= Conversion;
					cury = toy;
					svgLineTo(ite,  curx, cury );
					break;
				}
			case 'z':
			case 'Z':
				{
					curx = subpathx;
					cury = subpathy;
					svgClosePath(ite);
					break;
				}
			case 'c':
				relative = true;
			case 'C':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x1 *= Conversion;
					y1 *= Conversion;
					x2 *= Conversion;
					y2 *= Conversion;
					px1 = relative ? curx + x1 : x1;
					py1 = relative ? cury + y1 : y1;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 's':
				relative = true;
			case 'S':
				{
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x2 *= Conversion;
					y2 *= Conversion;
					px1 = 2 * curx - contrlx;
					py1 = 2 * cury - contrly;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 'q':
				relative = true;
			case 'Q':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x1 *= Conversion;
					y1 *= Conversion;
					px1 = relative ? (curx + 2 * (x1 + curx)) * (1.0 / 3.0) : (curx + 2 * x1) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * (y1 + cury)) * (1.0 / 3.0) : (cury + 2 * y1) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * (x1 + curx)) * (1.0 / 3.0) : (tox + 2 * x1) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * (y1 + cury)) * (1.0 / 3.0) : (toy + 2 * y1) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x1 : (tox + 2 * x1) * (1.0 / 3.0);
					contrly = relative ? cury + y1 : (toy + 2 * y1) * (1.0 / 3.0);
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 't':
				relative = true;
			case 'T':
				{
					ptr = getCoord(ptr, tox);
					ptr = getCoord(ptr, toy);
					tox *= Conversion;
					toy *= Conversion;
					xc = 2 * curx - contrlx;
					yc = 2 * cury - contrly;
					px1 = relative ? (curx + 2 * xc) * (1.0 / 3.0) : (curx + 2 * xc) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * yc) * (1.0 / 3.0) : (cury + 2 * yc) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * xc) * (1.0 / 3.0) : (tox + 2 * xc) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * yc) * (1.0 / 3.0) : (toy + 2 * yc) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = xc;
					contrly = yc;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 'a':
				relative = true;
			case 'A':
				{
					bool largeArc, sweep;
					double angle, rx, ry;
					ptr = getCoord( ptr, rx );
					ptr = getCoord( ptr, ry );
					ptr = getCoord( ptr, angle );
					ptr = getCoord( ptr, tox );
					ry *= Conversion;
					rx *= Conversion;
					largeArc = tox == 1;
					ptr = getCoord( ptr, tox );
					sweep = tox == 1;
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					calculateArc(ite, relative, curx, cury, angle, tox, toy, rx, ry, largeArc, sweep );
				}
			}
			lastCommand = command;
			if(*ptr == '+' || *ptr == '-' || (*ptr >= '0' && *ptr <= '9'))
			{
				// there are still coords in this command
				if(command == 'M')
					command = 'L';
				else if(command == 'm')
					command = 'l';
			}
			else
				command = *(ptr++);

			if( lastCommand != 'C' && lastCommand != 'c' &&
			        lastCommand != 'S' && lastCommand != 's' &&
			        lastCommand != 'Q' && lastCommand != 'q' &&
			        lastCommand != 'T' && lastCommand != 't')
			{
				contrlx = curx;
				contrly = cury;
			}
		}
		if ((lastCommand != 'z') && (lastCommand != 'Z'))
			ret = true;
		if (ite->size() > 2)
		{
			if ((ite->point(0).x() == ite->point(ite->size()-2).x()) && (ite->point(0).y() == ite->point(ite->size()-2).y()))
				ret = false;
		}
	}
	return ret;
}

void SVGPlug::calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag)
{
	double sin_th, cos_th;
	double a00, a01, a10, a11;
	double x0, y0, x1, y1, xc, yc;
	double d, sfactor, sfactor_sq;
	double th0, th1, th_arc;
	int i, n_segs;
	sin_th = sin(angle * (M_PI / 180.0));
	cos_th = cos(angle * (M_PI / 180.0));
	double dx;
	if(!relative)
		dx = (curx - x) / 2.0;
	else
		dx = -x / 2.0;
	double dy;
	if(!relative)
		dy = (cury - y) / 2.0;
	else
		dy = -y / 2.0;
	double _x1 =  cos_th * dx + sin_th * dy;
	double _y1 = -sin_th * dx + cos_th * dy;
	double Pr1 = r1 * r1;
	double Pr2 = r2 * r2;
	double Px = _x1 * _x1;
	double Py = _y1 * _y1;
	// Spec : check if radii are large enough
	double check = Px / Pr1 + Py / Pr2;
	if(check > 1)
	{
		r1 = r1 * sqrt(check);
		r2 = r2 * sqrt(check);
	}
	a00 = cos_th / r1;
	a01 = sin_th / r1;
	a10 = -sin_th / r2;
	a11 = cos_th / r2;
	x0 = a00 * curx + a01 * cury;
	y0 = a10 * curx + a11 * cury;
	if(!relative)
		x1 = a00 * x + a01 * y;
	else
		x1 = a00 * (curx + x) + a01 * (cury + y);
	if(!relative)
		y1 = a10 * x + a11 * y;
	else
		y1 = a10 * (curx + x) + a11 * (cury + y);
	/* (x0, y0) is current point in transformed coordinate space.
	   (x1, y1) is new point in transformed coordinate space.

	   The arc fits a unit-radius circle in this space.
	    */
	d = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
	sfactor_sq = 1.0 / d - 0.25;
	if(sfactor_sq < 0)
		sfactor_sq = 0;
	sfactor = sqrt(sfactor_sq);
	if(sweepFlag == largeArcFlag)
		sfactor = -sfactor;
	xc = 0.5 * (x0 + x1) - sfactor * (y1 - y0);
	yc = 0.5 * (y0 + y1) + sfactor * (x1 - x0);

	/* (xc, yc) is center of the circle. */
	th0 = atan2(y0 - yc, x0 - xc);
	th1 = atan2(y1 - yc, x1 - xc);
	th_arc = th1 - th0;
	if(th_arc < 0 && sweepFlag)
		th_arc += 2 * M_PI;
	else if(th_arc > 0 && !sweepFlag)
		th_arc -= 2 * M_PI;
	n_segs = static_cast<int>(ceil(fabs(th_arc / (M_PI * 0.5 + 0.001))));
	for(i = 0; i < n_segs; i++)
	{
		{
			double sin_th, cos_th;
			double a00, a01, a10, a11;
			double x1, y1, x2, y2, x3, y3;
			double t;
			double th_half;
			double _th0 = th0 + i * th_arc / n_segs;
			double _th1 = th0 + (i + 1) * th_arc / n_segs;
			sin_th = sin(angle * (M_PI / 180.0));
			cos_th = cos(angle * (M_PI / 180.0));
			/* inverse transform compared with rsvg_path_arc */
			a00 = cos_th * r1;
			a01 = -sin_th * r2;
			a10 = sin_th * r1;
			a11 = cos_th * r2;
			th_half = 0.5 * (_th1 - _th0);
			t = (8.0 / 3.0) * sin(th_half * 0.5) * sin(th_half * 0.5) / sin(th_half);
			x1 = xc + cos(_th0) - t * sin(_th0);
			y1 = yc + sin(_th0) + t * cos(_th0);
			x3 = xc + cos(_th1);
			y3 = yc + sin(_th1);
			x2 = x3 + t * sin(_th1);
			y2 = y3 - t * cos(_th1);
			svgCurveToCubic(ite, a00 * x1 + a01 * y1, a10 * x1 + a11 * y1, a00 * x2 + a01 * y2, a10 * x2 + a11 * y2, a00 * x3 + a01 * y3, a10 * x3 + a11 * y3 );
		}
	}
	if(!relative)
		curx = x;
	else
		curx += x;
	if(!relative)
		cury = y;
	else
		cury += y;
}

void SVGPlug::svgMoveTo(double x1, double y1)
{
	CurrX = x1;
	CurrY = y1;
	StartX = x1;
	StartY = y1;
	PathLen = 0;
}

void SVGPlug::svgLineTo(FPointArray *i, double x1, double y1)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (i->size() > 3)
	{
		FPoint b1 = i->point(i->size()-4);
		FPoint b2 = i->point(i->size()-3);
		FPoint b3 = i->point(i->size()-2);
		FPoint b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		if ((b1 == n1) && (b2 == n1) && (b3 == n2) && (b4 == n2))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x1, y1));
	CurrX = x1;
	CurrY = y1;
	PathLen += 4;
}

void SVGPlug::svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (PathLen > 3)
	{
		FPoint b1 = i->point(i->size()-4);
		FPoint b2 = i->point(i->size()-3);
		FPoint b3 = i->point(i->size()-2);
		FPoint b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		FPoint n3 = FPoint(x3, y3);
		FPoint n4 = FPoint(x2, y2);
		if ((b1 == n1) && (b2 == n2) && (b3 == n3) && (b4 == n4))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x3, y3));
	i->addPoint(FPoint(x2, y2));
	CurrX = x3;
	CurrY = y3;
	PathLen += 4;
}

void SVGPlug::svgClosePath(FPointArray *i)
{
	if (PathLen > 2)
	{
		if ((PathLen == 4) || (i->point(i->size()-2).x() != StartX) || (i->point(i->size()-2).y() != StartY))
		{
			i->addPoint(i->point(i->size()-2));
			i->addPoint(i->point(i->size()-3));
			i->addPoint(FPoint(StartX, StartY));
			i->addPoint(FPoint(StartX, StartY));
		}
	}
}

QColor SVGPlug::parseColorN( const QString &rgbColor )
{
	int r, g, b;
	keywordToRGB( rgbColor, r, g, b );
	return QColor( r, g, b );
}


QString SVGPlug::parseColor( const QString &s )
{
	QColor c;
	QString ret = CommonStrings::None;
	if( s.startsWith( "rgb(" ) )
	{
		QString parse = s.stripWhiteSpace();
		QStringList colors = QStringList::split( ',', parse );
		QString r = colors[0].right( ( colors[0].length() - 4 ) );
		QString g = colors[1];
		QString b = colors[2].left( ( colors[2].length() - 1 ) );
		if( r.contains( "%" ) )
		{
			r = r.left( r.length() - 1 );
			r = QString::number( static_cast<int>( ( static_cast<double>( 255 * r.toDouble() ) / 100.0 ) ) );
		}
		if( g.contains( "%" ) )
		{
			g = g.left( g.length() - 1 );
			g = QString::number( static_cast<int>( ( static_cast<double>( 255 * g.toDouble() ) / 100.0 ) ) );
		}
		if( b.contains( "%" ) )
		{
			b = b.left( b.length() - 1 );
			b = QString::number( static_cast<int>( ( static_cast<double>( 255 * b.toDouble() ) / 100.0 ) ) );
		}
		c = QColor(r.toInt(), g.toInt(), b.toInt());
	}
	else
	{
		QString rgbColor = s.stripWhiteSpace();
		if( rgbColor.startsWith( "#" ) )
			c.setNamedColor( rgbColor );
		else
			c = parseColorN( rgbColor );
	}
	ColorList::Iterator it;
	bool found = false;
	int r, g, b;
	QColor tmpR;
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.data().getColorModel() == colorModelRGB)
		{
			it.data().getRGB(&r, &g, &b);
			tmpR.setRgb(r, g, b);
			if (c == tmpR)
			{
				ret = it.key();
				found = true;
			}
		}
	}
	if (!found)
	{
		ScColor tmp;
		tmp.fromQColor(c);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		m_Doc->PageColors.insert("FromSVG"+c.name(), tmp);
		m_Doc->scMW()->propertiesPalette->updateColorList();
		ret = "FromSVG"+c.name();
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
		if ((obj->InherCol) && (params == "currentColor"))
			obj->FillCol = obj->CurCol;
		else if (params == "none")
		{
			obj->FillCol = CommonStrings::None;
		}
		else if( params.startsWith( "url(" ) )
		{
			unsigned int start = params.find("#") + 1;
			unsigned int end = params.findRev(")");
			QString key = params.mid(start, end - start);
			while (!m_gradients[key].reference.isEmpty())
			{
				QString key2 = m_gradients[key].reference;
				if (m_gradients[key2].typeValid)
					obj->Gradient = m_gradients[key2].Type;
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
				key = m_gradients[key].reference;
			}
			key = params.mid(start, end - start);
			if (m_gradients[key].typeValid)
				obj->Gradient = m_gradients[key].Type;
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
		{
			obj->CurCol = CommonStrings::None;
		}
		else
		{
			obj->CurCol = parseColor(params);
		}
	}
	else if( command == "stroke" )
	{
		if ((obj->InherCol) && (params == "currentColor"))
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
					unsigned int end = params.findRev(")");
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
		QValueList<double> array;
		if(params != "none")
		{
			QStringList dashes = QStringList::split( ' ', params );
			for( QStringList::Iterator it = dashes.begin(); it != dashes.end(); ++it )
				array.append( (*it).toDouble() );
		}
		obj->dashArray = array;
	}
	else if( command == "stroke-dashoffset" )
		obj->dashOffset = params.toDouble();
	else if( command == "font-family" )
	{
		QString family = params;
		QString ret = "";
		family.replace( QRegExp( "'" ) , QChar( ' ' ) );
		obj->Family = m_Doc->toolSettings.defFont; // family;
		bool found = false;
		SCFontsIterator it(PrefsManager::instance()->appPrefs.AvailFonts);
		for ( ; it.hasNext(); it.next())
		{
			QString fam;
			QString fn = it.current().scName();
			int	pos=fn.find(" ");
			fam = fn.left(pos);
			if (fam == family)
			{
				found = true;
				ret = fn;
			}
		}
		if (found)
			obj->Family = ret;
		else
			obj->Family = m_Doc->toolSettings.defFont;
	}
	else if( command == "font-size" )
		obj->FontSize = static_cast<int>(parseUnit(params) * 10.0);
	else if( command == "text-anchor" )
		obj->textAnchor = params;
	else
		unsupported = true;
}

void SVGPlug::parseStyle( SvgStyle *obj, const QDomElement &e )
{
	SvgStyle *gc = m_gc.current();
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
	if( !e.attribute( "fill-opacity" ).isEmpty() )
		parsePA( obj, "fill-opacity", e.attribute( "fill-opacity" ) );
	if( !e.attribute( "opacity" ).isEmpty() )
		parsePA( obj, "opacity", e.attribute( "opacity" ) );
	if( !e.attribute( "font-family" ).isEmpty() )
		parsePA( obj, "font-family", e.attribute( "font-family" ) );
	if( !e.attribute( "font-size" ).isEmpty() )
		parsePA( obj, "font-size", e.attribute( "font-size" ) );
	if( !e.attribute( "text-anchor" ).isEmpty() )
		parsePA( obj, "text-anchor", e.attribute( "text-anchor" ) );
	QString style = e.attribute( "style" ).simplifyWhiteSpace();
	QStringList substyles = QStringList::split( ';', style );
	for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
	{
		QStringList substyle = QStringList::split( ':', (*it) );
		QString command	= substyle[0].stripWhiteSpace();
		QString params	= substyle[1].stripWhiteSpace();
		parsePA( obj, command, params );
	}
	return;
}

void SVGPlug::parseColorStops(GradientHelper *gradient, const QDomElement &e)
{
	QString Col = "Black";
	double offset = 0;
	double opa;
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
				offset = temp.toDouble() / 100.0;
			}
			else
				offset = temp.toDouble();
			if( !stop.attribute( "stop-opacity" ).isEmpty() )
				opa = fromPercentage(stop.attribute("stop-opacity"));
			if( !stop.attribute( "stop-color" ).isEmpty() )
				Col = parseColor(stop.attribute("stop-color"));
			else
			{
				QString style = stop.attribute( "style" ).simplifyWhiteSpace();
				QStringList substyles = QStringList::split( ';', style );
				for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
				{
					QStringList substyle = QStringList::split( ':', (*it) );
					QString command	= substyle[0].stripWhiteSpace();
					QString params	= substyle[1].stripWhiteSpace();
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
{}
