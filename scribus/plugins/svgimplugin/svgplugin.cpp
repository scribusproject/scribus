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
#include "undomanager.h"
#include "util.h"
#include "scfontmetrics.h"
#include "prefsmanager.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "fpointarray.h"

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

SVGImportPlugin::SVGImportPlugin() : LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

SVGImportPlugin::~SVGImportPlugin() {};

void SVGImportPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ImportSVG";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Import &SVG...");
	// Menu
	m_actionInfo.menu = "FileImport";
	m_actionInfo.enabledOnStartup = true;
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

QValueList<LoadSavePlugin::FormatSupport> SVGImportPlugin::supportedFormats() const
{
	QValueList<FormatSupport> formats;
	QString svgName = tr("Scalable Vector Graphics");
	FormatSupport fmt;
	fmt.trName = svgName;
	fmt.internalName = "svgim";
	fmt.filter = svgName + " (*.svg *.svgz)";
	fmt.modes = Format_Import|Format_Load;
	fmt.mimeTypes = QStringList("image/svg+xml");
	fmt.priority = 64;
	formats.append(fmt);
	return formats;
}

bool SVGImportPlugin::fileSupported(QIODevice* /* file */) const
{
	// TODO: identify valid SVG
	return true;
}

/*!
 \fn void Run(QString filename)
 \author Franz Schmid
 \date
 \brief Run the SVG import
 \retval true for success
 */
bool SVGImportPlugin::run(QString filename)
{
	bool interactive = false;
	if (filename.isEmpty())
	{
		interactive = true;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("SVGPlugin");
		QString wdir = prefs->get("wdir", ".");
#ifdef HAVE_LIBZ
		CustomFDialog diaf(ScApp, wdir, QObject::tr("Open"), QObject::tr("SVG-Images (*.svg *.svgz);;All Files (*)"));
#else
		CustomFDialog diaf(ScApp, wdir, QObject::tr("Open"), QObject::tr("SVG-Images (*.svg);;All Files (*)"));
#endif
		if (diaf.exec())
		{
			filename = diaf.selectedFile();
			prefs->set("wdir", filename.left(filename.findRev("/")));
		}
		else
			return true;
	}
	if (UndoManager::undoEnabled() && ScApp->HaveDoc)
	{
		UndoManager::instance()->beginTransaction(ScApp->doc->currentPage->getUName(),Um::IImageFrame,Um::ImportSVG, filename, Um::ISVG);
	}
	else if (UndoManager::undoEnabled() && !ScApp->HaveDoc)
		UndoManager::instance()->setUndoEnabled(false);
	SVGPlug *dia = new SVGPlug(filename, interactive);
	Q_CHECK_PTR(dia);
	if (UndoManager::undoEnabled())
		UndoManager::instance()->commit();
	else
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return true;
}

/*!
 \fn SVGPlug::SVGPlug( QString fName )
 \author Franz Schmid
 \date
 \brief Create the SVG importer window
 \param fName QString
 \retval SVGPlug plugin
 */
SVGPlug::SVGPlug( QString fName, bool isInteractive ) :
	QObject(ScApp)
{
	interactive = isInteractive;
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
	convert();
	QDir::setCurrent(CurDirP);
}

/*!
 \fn void SVGPlug::convert()
 \author Franz Schmid
 \date
 \brief
 \param None
 \retval None
 */
void SVGPlug::convert()
{
	bool ret = false;
	SvgStyle *gc = new SvgStyle;
	Conversion = 0.8;
	QDomElement docElem = inpdoc.documentElement();
	double width = !docElem.attribute("width").isEmpty() ? parseUnit(docElem.attribute( "width" )) : 550.0;
	double height = !docElem.attribute("height").isEmpty() ? parseUnit(docElem.attribute( "height" )) : 841.0;
	Conversion = 0.8;
	if (!interactive)
	{
		ScApp->doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
		ScApp->view->addPage(0);
	}
	else
	{
		if (!ScApp->HaveDoc)
		{
			ScApp->doFileNew(width, height, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom");
			ret = true;
		}
	}
	if ((ret) || (!interactive))
	{
		if (width > height)
			ScApp->doc->PageOri = 1;
		else
			ScApp->doc->PageOri = 0;
		ScApp->doc->PageSize = "Custom";
	}
	currDoc = ScApp->doc;
	FPoint minSize = currDoc->minCanvasCoordinate;
	FPoint maxSize = currDoc->maxCanvasCoordinate;
	ScApp->view->Deselect();
	Elements.clear();
	currDoc->setLoading(true);
	currDoc->DoDrawing = false;
	ScApp->view->setUpdatesEnabled(false);
	ScApp->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	gc->Family = currDoc->toolSettings.defFont;
	if (!currDoc->PageColors.contains("Black"))
		currDoc->PageColors.insert("Black", ScColor(0, 0, 0, 255));
	m_gc.push( gc );
	viewTransformX = 0;
	viewTransformY = 0;
	viewScaleX = 1;
	viewScaleY = 1;
	haveViewBox = false;
	if( !docElem.attribute( "viewBox" ).isEmpty() )
	{
		Conversion = 1.0;
		double w2 = !docElem.attribute("width").isEmpty() ? parseUnit(docElem.attribute( "width" )) : 550.0;
		double h2 = !docElem.attribute("height").isEmpty() ? parseUnit(docElem.attribute( "height" )) : 841.0;
		Conversion = 0.8;
		QString viewbox( docElem.attribute( "viewBox" ) );
		QStringList points = QStringList::split( ' ', viewbox.replace( QRegExp(","), " ").simplifyWhiteSpace() );
		viewTransformX = points[0].toDouble();
		viewTransformY = points[1].toDouble();
		viewScaleX = w2 / points[2].toDouble();
		viewScaleY = h2 / points[3].toDouble();
		haveViewBox = true;
	}
	parseGroup( docElem );
	ScApp->view->SelItem.clear();
	if (Elements.count() > 1)
	{
		for (uint a = 0; a < Elements.count(); ++a)
		{
			Elements.at(a)->Groups.push(currDoc->GroupCounter);
		}
		currDoc->GroupCounter++;
	}
	currDoc->DoDrawing = true;
	ScApp->view->setUpdatesEnabled(true);
	ScApp->ScriptRunning = false;
	if (interactive)
		currDoc->setLoading(false);
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	if ((Elements.count() > 0) && (!ret) && (interactive))
	{
		currDoc->DragP = true;
		currDoc->DraggedElem = 0;
		currDoc->DragElements.clear();
		for (uint dre=0; dre<Elements.count(); ++dre)
		{
			currDoc->DragElements.append(Elements.at(dre)->ItemNr);
			ScApp->view->SelItem.append(Elements.at(dre));
		}
		ScriXmlDoc *ss = new ScriXmlDoc();
		ScApp->view->setGroupRect();
		QDragObject *dr = new QTextDrag(ss->WriteElem(&ScApp->view->SelItem, currDoc, ScApp->view), ScApp->view->viewport());
		ScApp->view->DeleteItem();
		ScApp->view->resizeContents(qRound((maxSize.x() - minSize.x()) * ScApp->view->getScale()), qRound((maxSize.y() - minSize.y()) * ScApp->view->getScale()));
		ScApp->view->scrollBy(qRound((currDoc->minCanvasCoordinate.x() - minSize.x()) * ScApp->view->getScale()), qRound((currDoc->minCanvasCoordinate.y() - minSize.y()) * ScApp->view->getScale()));
		currDoc->minCanvasCoordinate = minSize;
		currDoc->maxCanvasCoordinate = maxSize;
		dr->setPixmap(loadIcon("DragPix.xpm"));
		dr->drag();
		delete ss;
		currDoc->DragP = false;
		currDoc->DraggedElem = 0;
		currDoc->DragElements.clear();
	}
	else
	{
		currDoc->setModified(false);
		ScApp->slotDocCh();
	}
}

/*!
 \fn void SVGPlug::addGraphicContext()
 \author Franz Schmid
 \date
 \brief
 \param None
 \retval None
 */
void SVGPlug::addGraphicContext()
{
	SvgStyle *gc = new SvgStyle;
	if ( m_gc.current() )
		*gc = *( m_gc.current() );
	m_gc.push( gc );
}

/*!
 \fn void SVGPlug::setupTransform( const QDomElement &e )
 \author Franz Schmid
 \date
 \brief
 \param e const QDomElement &
 \retval None
 */
void SVGPlug::setupTransform( const QDomElement &e )
{
	SvgStyle *gc = m_gc.current();
	QWMatrix mat = parseTransform( e.attribute( "transform" ) );
	if (!e.attribute("transform").isEmpty())
		gc->matrix = mat * gc->matrix;
}

/*!
 \fn void SVGPlug::parseGroup(const QDomElement &e)
 \author Franz Schmid
 \date
 \brief
 \param e const QDomElement &
 \retval None
 */
QPtrList<PageItem> SVGPlug::parseGroup(const QDomElement &e)
{
	QPtrList<PageItem> GElements;
	FPointArray ImgClip;
	ImgClip.resize(0);
	double BaseX = currDoc->currentPage->xOffset();
	double BaseY = currDoc->currentPage->yOffset();
	for( QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		int z = -1;
		QDomElement b = n.toElement();
		if( b.isNull() )
			continue;
		QString STag = b.tagName();
		if( STag == "g" )
		{
			addGraphicContext();
			SvgStyle *gc = m_gc.current();
			setupTransform( b );
			parseStyle( gc, b );
			QPtrList<PageItem> gElements = parseGroup( b );
			for (uint gr = 0; gr < gElements.count(); ++gr)
			{
				gElements.at(gr)->Groups.push(currDoc->GroupCounter);
				GElements.append(gElements.at(gr));
			}
			delete( m_gc.pop() );
			currDoc->GroupCounter++;
			continue;
		}
		if( b.tagName() == "defs" )
		{
			parseGroup( b ); 	// try for gradients at least
			continue;
		}
		else if( STag == "linearGradient" || STag == "radialGradient" )
		{
			parseGradient( b );
			continue;
		}
		else if( STag == "rect" )
		{
			addGraphicContext();
			double x = parseUnit( b.attribute( "x" ) );
			double y = parseUnit( b.attribute( "y" ) );
			double width = parseUnit( b.attribute( "width" ));
			double height = parseUnit( b.attribute( "height" ) );
			double rx = b.attribute( "rx" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "rx" ) );
			double ry = b.attribute( "ry" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "ry" ) );
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			//z = ScApp->view->PaintRect(BaseX, BaseY, width, height, gc->LWidth, gc->FillCol, gc->StrokeCol);
			z = currDoc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, width, height, gc->LWidth, gc->FillCol, gc->StrokeCol, !ScApp->view->Mpressed);
			PageItem* ite = currDoc->Items.at(z);
			if ((rx != 0) || (ry != 0))
			{
				ite->RadRect = QMAX(rx, ry);
				ite->SetFrameRound();
				ScApp->view->setRedrawBounding(ite);
			}
			QWMatrix mm = QWMatrix();
			mm.translate(x, y);
			ite->PoLine.map(mm);
			FPoint wh = getMaxClipF(&ite->PoLine);
			ite->Width = wh.x();
			ite->Height = wh.y();
		}
		else if( STag == "ellipse" )
		{
			addGraphicContext();
			double rx = parseUnit( b.attribute( "rx" ) );
			double ry = parseUnit( b.attribute( "ry" ) );
			double x = parseUnit( b.attribute( "cx" ) ) - rx;
			double y = parseUnit( b.attribute( "cy" ) ) - ry;
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			//z = ScApp->view->PaintEllipse(BaseX, BaseY, rx * 2.0, ry * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol);
			z = currDoc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, rx * 2.0, ry * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol, !ScApp->view->Mpressed);
			PageItem* ite = currDoc->Items.at(z);
			QWMatrix mm = QWMatrix();
			mm.translate(x, y);
			ite->PoLine.map(mm);
			FPoint wh = getMaxClipF(&ite->PoLine);
			ite->Width = wh.x();
			ite->Height = wh.y();
		}
		else if( STag == "circle" )
		{
			addGraphicContext();
			double r = parseUnit( b.attribute( "r" ) );
			double x = parseUnit( b.attribute( "cx" ) ) - r;
			double y = parseUnit( b.attribute( "cy" ) ) - r;
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			//z = ScApp->view->PaintEllipse(BaseX, BaseY, r * 2.0, r * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol);
			z = currDoc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX, BaseY, r * 2.0, r * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol, !ScApp->view->Mpressed);
			PageItem* ite = currDoc->Items.at(z);
			QWMatrix mm = QWMatrix();
			mm.translate(x, y);
			ite->PoLine.map(mm);
			FPoint wh = getMaxClipF(&ite->PoLine);
			ite->Width = wh.x();
			ite->Height = wh.y();
		}
		else if( STag == "line" )
		{
			addGraphicContext();
			double x1 = b.attribute( "x1" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "x1" ) );
			double y1 = b.attribute( "y1" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "y1" ) );
			double x2 = b.attribute( "x2" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "x2" ) );
			double y2 = b.attribute( "y2" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "y2" ) );
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			//z = ScApp->view->PaintPoly(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
			z = currDoc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, !ScApp->view->Mpressed);
			PageItem* ite = currDoc->Items.at(z);
			ite->PoLine.resize(4);
			ite->PoLine.setPoint(0, FPoint(x1, y1));
			ite->PoLine.setPoint(1, FPoint(x1, y1));
			ite->PoLine.setPoint(2, FPoint(x2, y2));
			ite->PoLine.setPoint(3, FPoint(x2, y2));
		}
		else if( STag == "clipPath" )
		{
			QDomNode n2 = b.firstChild();
			QDomElement b2 = n2.toElement();
			parseSVG( b2.attribute( "d" ), &ImgClip );
			continue;
		}
		else if( STag == "path" )
		{
			addGraphicContext();
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			//z = ScApp->view->PaintPoly(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
			z = currDoc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, !ScApp->view->Mpressed);
			PageItem* ite = currDoc->Items.at(z);
			ite->PoLine.resize(0);
			if (parseSVG( b.attribute( "d" ), &ite->PoLine ))
				ite->convertTo(PageItem::PolyLine);
			if (ite->PoLine.size() < 4)
			{
				ScApp->view->SelItem.append(ite);
				ScApp->view->DeleteItem();
				z = -1;
			}
		}
		else if( STag == "polyline" || b.tagName() == "polygon" )
		{
			addGraphicContext();
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			if( b.tagName() == "polygon" )
				//z = ScApp->view->PaintPoly(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
				z = currDoc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, !ScApp->view->Mpressed);
			else
				//z = ScApp->view->PaintPolyLine(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
				z = currDoc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, !ScApp->view->Mpressed);
			PageItem* ite = currDoc->Items.at(z);
			ite->PoLine.resize(0);
			bool bFirst = true;
			double x = 0.0;
			double y = 0.0;
			QString points = b.attribute( "points" ).simplifyWhiteSpace();
			points.replace( QRegExp( "," ), " " );
			points.replace( QRegExp( "\r" ), "" );
			points.replace( QRegExp( "\n" ), "" );
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
			double x = b.attribute( "x" ).isEmpty() ? 0.0 : parseUnit(b.attribute("x"));
			double y = b.attribute( "y" ).isEmpty() ? 0.0 : parseUnit(b.attribute("y"));
			parseStyle(gc, b);
			QPtrList<PageItem> el = parseText(x+BaseX, y+BaseY, b);
			for (uint ec = 0; ec < el.count(); ++ec)
			{
				GElements.append(el.at(ec));
			}
			z = -1;
		}
		else if( STag == "image" )
		{
			addGraphicContext();
			QString fname = b.attribute("xlink:href");
			double x = b.attribute( "x" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "x" ) );
			double y = b.attribute( "y" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "y" ) );
			double w = b.attribute( "width" ).isEmpty() ? 1.0 : parseUnit( b.attribute( "width" ) );
			double h = b.attribute( "height" ).isEmpty() ? 1.0 : parseUnit( b.attribute( "height" ) );
			//z = ScApp->view->PaintPict(x+BaseX, y+BaseY, w, h);
			z = currDoc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x+BaseX, y+BaseY, w, h, 1, currDoc->toolSettings.dBrushPict, "None", !ScApp->view->Mpressed);
			if (!fname.isEmpty())
				currDoc->LoadPict(fname, z);
			PageItem* ite = currDoc->Items.at(z);
			if (ImgClip.size() != 0)
				ite->PoLine = ImgClip.copy();
			ImgClip.resize(0);
			ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
		}
		else
			continue;
		if (z != -1)
		{
			setupTransform( b );
			SvgStyle *gc = m_gc.current();
			PageItem* ite = currDoc->Items.at(z);
			switch (ite->itemType())
			{
			case PageItem::ImageFrame:
				{
					QWMatrix mm = gc->matrix;
					ite->Xpos += mm.dx();
					ite->Ypos += mm.dy();
					ite->Width = ite->Width * mm.m11();
					ite->Height = ite->Height * mm.m22();
					ite->Pwidth = ite->Pwidth * ((mm.m11() + mm.m22()) / 2.0);
					if (ite->PicAvail)
					{
						ite->LocalScX = ite->Width / ite->pixm.width();
						ite->LocalScY = ite->Height / ite->pixm.height();
					}
					break;
				}
			case PageItem::TextFrame:
				{
					QWMatrix mm = gc->matrix;
					ite->Pwidth = ite->Pwidth * ((mm.m11() + mm.m22()) / 2.0);
				}
				break;
			default:
				{
					ite->ClipEdited = true;
					ite->FrameType = 3;
					QWMatrix mm = gc->matrix;
					ite->PoLine.map(mm);
					if (haveViewBox)
					{
						QWMatrix mv;
						mv.translate(viewTransformX, viewTransformY);
						mv.scale(viewScaleX, viewScaleY);
						ite->PoLine.map(mv);
					}
					ite->Pwidth = ite->Pwidth * ((mm.m11() + mm.m22()) / 2.0);
					FPoint wh = getMaxClipF(&ite->PoLine);
					ite->Width = wh.x();
					ite->Height = wh.y();
					ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
					ScApp->view->AdjustItemSize(ite);
					break;
				}
			}
			if( !b.attribute("id").isEmpty() )
				ite->setItemName(" "+b.attribute("id"));
			ite->setFillTransparency(gc->Transparency);
			ite->setLineTransparency(gc->TranspStroke);
			ite->PLineEnd = gc->PLineEnd;
			ite->PLineJoin = gc->PLineJoin;
			ite->setTextFlowsAroundFrame(false);
			ite->DashOffset = gc->dashOffset;
			ite->DashValues = gc->dashArray;
			if (gc->Gradient != 0)
			{
				ite->fill_gradient = gc->GradCo;
				if (!gc->CSpace)
				{
					ite->GrStartX = gc->GX1 * ite->Width;
					ite->GrStartY = gc->GY1 * ite->Height;
					ite->GrEndX = gc->GX2 * ite->Width;
					ite->GrEndY = gc->GY2 * ite->Height;
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
					mm = mm * gc->matrixg;
					FPointArray gra;
					gra.setPoints(2, gc->GX1, gc->GY1, gc->GX2, gc->GY2);
					gra.map(mm);
					gc->GX1 = gra.point(0).x();
					gc->GY1 = gra.point(0).y();
					gc->GX2 = gra.point(1).x();
					gc->GY2 = gra.point(1).y();
					ite->GrStartX = gc->GX1 - ite->Xpos+BaseX;
					ite->GrStartY = gc->GY1 - ite->Ypos+BaseY;
					ite->GrEndX = gc->GX2 - ite->Xpos+BaseX;
					ite->GrEndY = gc->GY2 - ite->Ypos+BaseY;
				}
				ite->GrType = gc->Gradient;
			}
			GElements.append(ite);
			Elements.append(ite);
		}
		delete( m_gc.pop() );
	}
	return GElements;
}

/*!
 \fn double SVGPlug::fromPercentage( const QString &s )
 \author Franz Schmid
 \date
 \brief
 \param s const QString &
 \retval double
 */
double SVGPlug::fromPercentage( const QString &s )
{
	if( s.endsWith( "%" ) )
		return s.toDouble() / 100.0;
	else
		return s.toDouble();
}

/*!
 \fn double SVGPlug::parseUnit(const QString &unit)
 \author Franz Schmid
 \date
 \brief
 \param unit const QString &
 \retval double
 */
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

/*!
 \fn QWMatrix SVGPlug::parseTransform( const QString &transform )
 \author Franz Schmid
 \date
 \brief
 \param transform const QString
 \retval QWMatrix
 */
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
		ret = ret * result;
	}
	return ret;
}

/*!
 \fn const char * SVGPlug::getCoord( const char *ptr, double &number )
 \author Franz Schmid
 \date
 \brief
 \param ptr const char *
 \param number double &
 \retval const char *
 */
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

/*!
 \fn bool SVGPlug::parseSVG( const QString &s, FPointArray *ite )
 \author Franz Schmid
 \date
 \brief
 \param s const QString &
 \param ite FPointArray *
 \retval bool
 */
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

/*!
 \fn void SVGPlug::calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag)
 \author Franz Schmid
 \date
 \brief
 \param ite FPointArray *
 \param relative bool
 \param curx double &
 \param cury double &
 \param angle double
 \param x double
 \param y double
 \param r1 double
 \param r2 double
 \param largeArcFlag bool
 \param sweepFlag bool
 \retval None
 */
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

/*!
 \fn void SVGPlug::svgMoveTo(double x1, double y1)
 \author Franz Schmid
 \date
 \brief
 \param i FPointArray *
 \param x1 double
 \param y1 double
 \retval None
 */
void SVGPlug::svgMoveTo(double x1, double y1)
{
	CurrX = x1;
	CurrY = y1;
	StartX = x1;
	StartY = y1;
	PathLen = 0;
}

/*!
 \fn void SVGPlug::svgLineTo(FPointArray *i, double x1, double y1)
 \author Franz Schmid
 \date
 \brief
 \param i FPointArray *
 \param x1 double
 \param y1 double
 \retval None
 */
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

/*!
 \fn void SVGPlug::svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3)
 \author Franz Schmid
 \date
 \brief
 \param i FPointArray *
 \param x1 double
 \param y1 double
 \param x2 double
 \param y2 double
 \param x3 double
 \param y3 double
 \retval None
 */
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

/*!
 \fn void SVGPlug::svgClosePath(FPointArray *i)
 \author Franz Schmid
 \date
 \brief
 \param i FPointArray *
 \retval None
 */
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

/*!
 \fn QColor SVGPlug::parseColorN( const QString &rgbColor )
 \author Franz Schmid
 \date
 \brief
 \param rgbColor const QString &
 \retval Qcolor
 */
QColor SVGPlug::parseColorN( const QString &rgbColor )
{
	int r, g, b;
	keywordToRGB( rgbColor, r, g, b );
	return QColor( r, g, b );
}

/*!
 \fn QString SVGPlug::parseColor( const QString &s )
 \author Franz Schmid
 \date
 \brief
 \param s const QString &
 \retval QString
 */
QString SVGPlug::parseColor( const QString &s )
{
	QColor c;
	QString ret = "None";
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
	for (it = currDoc->PageColors.begin(); it != currDoc->PageColors.end(); ++it)
	{
		currDoc->PageColors[it.key()].getRGB(&r, &g, &b);
		tmpR.setRgb(r, g, b);
		if (c == tmpR && currDoc->PageColors[it.key()].getColorModel() == colorModelRGB)
		{
			ret = it.key();
			found = true;
		}
	}
	if (!found)
	{
		ScColor tmp;
		tmp.fromQColor(c);
		currDoc->PageColors.insert("FromSVG"+c.name(), tmp);
		ScApp->propertiesPalette->updateColorList();
		ret = "FromSVG"+c.name();
	}
	return ret;
}

/*!
 \fn void SVGPlug::parsePA( SvgStyle *obj, const QString &command, const QString &params )
 \author Franz Schmid
 \date
 \brief
 \param obj SvgStyle *
 \param command const QString &
 \param params const QString &
 \retval None
 */
void SVGPlug::parsePA( SvgStyle *obj, const QString &command, const QString &params )
{
	if( command == "stroke-opacity" )
		obj->TranspStroke  = 1.0 - fromPercentage(params);
	else if( command == "fill-opacity" )
		obj->Transparency = 1.0 - fromPercentage(params);
	else if( command == "opacity" )
	{
		obj->Transparency = 1.0 - fromPercentage(params);
		obj->TranspStroke = 1.0 - fromPercentage(params);
	}
	else if( command == "fill" )
	{
		if ((obj->InherCol) && (params == "currentColor"))
			obj->FillCol = obj->CurCol;
		else if (params == "none")
		{
			obj->FillCol = "None";
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
			obj->FillCol = "None";
		}
		else
			obj->FillCol = parseColor(params);
	}
	else if( command == "color" )
	{
		if (params == "none")
			obj->CurCol = "None";
		else if( params.startsWith( "url(" ) )
		{
			obj->CurCol = "None";
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
			obj->StrokeCol = "None";
		}
		else if( params.startsWith( "url(" ) )
		{
			obj->StrokeCol = "None";
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
		obj->Family = currDoc->toolSettings.defFont; // family;
		bool found = false;
		SCFontsIterator it(PrefsManager::instance()->appPrefs.AvailFonts);
		for ( ; it.current(); ++it)
		{
			QString fam;
			QString fn = it.current()->scName();
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
			obj->Family = currDoc->toolSettings.defFont;
	}
	else if( command == "font-size" )
		obj->FontSize = static_cast<int>(parseUnit(params) * 10.0);
}

/*!
 \fn void SVGPlug::parseStyle( SvgStyle *obj, const QDomElement &e )
 \author Franz Schmid
 \date
 \brief
 \param obj SvgStyle *
 \param e const QDomElement &
 \retval None
 */
void SVGPlug::parseStyle( SvgStyle *obj, const QDomElement &e )
{
	SvgStyle *gc = m_gc.current();
	if (!gc)
		return;
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

/*!
 \fn void SVGPlug::parseColorStops(GradientHelper *gradient, const QDomElement &e)
 \author Franz Schmid
 \date
 \brief
 \param gradient GradientHelper *
 \param e const QDomElement &
 \retval None
 */
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
		gradient->gradient.addStop( currDoc->PageColors[Col].getRGBColor(), offset, 0.5, opa, Col, 100 );
		gradient->gradientValid = true;
	}
}

/*!
 \fn void SVGPlug::parseGradient( const QDomElement &e )
 \author Franz Schmid
 \date
 \brief
 \param e const QDomElement &
 \retval None
 */
void SVGPlug::parseGradient( const QDomElement &e )
{
	GradientHelper gradhelper;
	gradhelper.gradientValid = false;
	gradhelper.gradient.clearStops();
	gradhelper.gradient.setRepeatMethod( VGradient::none );

	QString href = e.attribute("xlink:href").mid(1);
	double x1, y1, x2, y2;
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
			gradhelper.X1 = e.attribute( "x1", "0").toDouble();
			gradhelper.x1Valid = true;
		}
		if (e.hasAttribute("y1"))
		{
			gradhelper.Y1 = e.attribute( "y1", "0" ).toDouble();
			gradhelper.y1Valid = true;
		}
		if (e.hasAttribute("x2"))
		{
			gradhelper.X2 = e.attribute( "x2", "1" ).toDouble();
			gradhelper.x2Valid = true;
		}
		if (e.hasAttribute("y2"))
		{
			gradhelper.Y2 = e.attribute( "y2", "0" ).toDouble();
			gradhelper.y2Valid = true;
		}
		gradhelper.Type = 6;
		gradhelper.typeValid = true;
	}
	else
	{
		if (e.hasAttribute("x1"))
		{
			x1 = e.attribute( "cx", "0.5").toDouble();
			gradhelper.x1Valid = true;
		}
		if (e.hasAttribute("y1"))
		{
			y1 = e.attribute( "cy", "0.5" ).toDouble();
			gradhelper.y1Valid = true;
		}
		if (e.hasAttribute("x2"))
		{
			x2 = e.attribute( "r", "0.5" ).toDouble();
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
		gradhelper.matrix = parseTransform( e.attribute( "gradientTransform" ) );
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

/*!
 \fn void SVGPlug::parseText(PageItem *ite, const QDomElement &e)
 \author Franz Schmid
 \date
 \brief
 \param ite PageItem *
 \param e const QDomElement &
 \retval None
 */
QPtrList<PageItem> SVGPlug::parseText(double x, double y, const QDomElement &e)
{
	struct ScText *hg;
	QPainter p;
	QPtrList<PageItem> GElements;
	p.begin(ScApp->view->viewport());
//	QFont ff(currDoc->UsedFonts[m_gc.current()->Family]);
	QFont ff(m_gc.current()->Family);
	ff.setPointSize(QMAX(qRound(m_gc.current()->FontSize / 10.0), 1));
	p.setFont(ff);
	setupTransform(e);
	int desc = p.fontMetrics().descent();
	int asce = p.fontMetrics().ascent();
	QString Text = QString::fromUtf8(e.text()).stripWhiteSpace();
	QDomNode c = e.firstChild();
	if ((!c.isNull()) && (c.toElement().tagName() == "tspan"))
	{
		double tempW = 0;
		for(QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
		{
			tempW = 0;
			QDomElement tspan = n.toElement();
			addGraphicContext();
			SvgStyle *gc = m_gc.current();
			parseStyle(gc, tspan);
			//int z = ScApp->view->PaintText(x, y, 10, 10, gc->LWidth, gc->FillCol);
			int z = currDoc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, 10, 10, gc->LWidth, "None", gc->FillCol, !ScApp->view->Mpressed);
			PageItem* ite = currDoc->Items.at(z);
			ite->Extra = 0;
			ite->TExtra = 0;
			ite->BExtra = 0;
			ite->RExtra = 0;
			ite->LineSp = gc->FontSize / 10.0 + 2;
			ite->Height = ite->LineSp+desc+2;
			ScApp->SetNewFont(gc->Family);
			QWMatrix mm = gc->matrix;
			if( (!tspan.attribute("x").isEmpty()) && (!tspan.attribute("y").isEmpty()) )
			{
				double x1 = parseUnit( tspan.attribute( "x", "0" ) );
				double y1 = parseUnit( tspan.attribute( "y", "0" ) );
				double mx = mm.m11() * x1 + mm.m21() * y1 + mm.dx();
				double my = mm.m22() * y1 + mm.m12() * x1 + mm.dy();
				ite->Xpos = mx;
				ite->Ypos = my;
			}
			else
			{
				double mx = mm.m11() * x + mm.m21() * y + mm.dx();
				double my = mm.m22() * y + mm.m12() * x + mm.dy();
				ite->Xpos = mx;
				ite->Ypos = my;
			}
			if (!tspan.text().isNull())
				Text = QString::fromUtf8(tspan.text()).stripWhiteSpace();
			else
				Text = " ";
			ite->IFont = gc->Family;
			ite->TxtFill = gc->FillCol;
			ite->ShTxtFill = 100;
			ite->TxtStroke = gc->StrokeCol;
			ite->ShTxtStroke = 100;
			ite->ISize = gc->FontSize;
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
			for (uint tt = 0; tt < Text.length(); ++tt)
			{
				hg = new ScText;
				hg->ch = Text.at(tt);
				hg->cfont = (*currDoc->AllFonts)[gc->Family];
				hg->csize = gc->FontSize;
				hg->ccolor = gc->FillCol;
				hg->cextra = 0;
				hg->cshade = 100;
				hg->cstroke = gc->StrokeCol;
				hg->cshade2 = 100;
				hg->cscale = 1000;
				hg->cscalev = 1000;
				hg->cbase = 0;
				hg->cshadowx = 50;
				hg->cshadowy = -50;
				hg->coutline = 10;
				hg->cunderpos = -1;
				hg->cunderwidth = -1;
				hg->cstrikepos = -1;
				hg->cstrikewidth = -1;
				hg->cselect = false;
				if( !tspan.attribute( "stroke" ).isEmpty() )
					hg->cstyle = 4;
				else
					hg->cstyle = 0;
				hg->cab = 0;
				hg->xp = 0;
				hg->yp = 0;
				hg->PRot = 0;
				hg->PtransX = 0;
				hg->PtransY = 0;
				hg->cembedded = 0;
				ite->itemText.append(hg);
				tempW += RealCWidth(currDoc, hg->cfont, hg->ch, hg->csize)+1;
				if (hg->ch == QChar(13))
				{
					ite->Height += ite->LineSp+desc;
					ite->Width = QMAX(ite->Width, tempW);
					tempW = 0;
				}
			}
			ite->Width = QMAX(ite->Width, tempW);
			ite->SetRectFrame();
			ScApp->view->setRedrawBounding(ite);
			ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
			ScApp->view->SelItem.append(ite);
			ScApp->view->HowTo = 1;
			ScApp->view->setGroupRect();
			ScApp->view->scaleGroup(mm.m11(), mm.m22());
			ScApp->view->Deselect();
			ite->Ypos -= asce * mm.m22();
			if( !e.attribute("id").isEmpty() )
				ite->setItemName(" "+e.attribute("id"));
			ite->setFillTransparency(gc->Transparency);
			ite->setLineTransparency(gc->TranspStroke);
			ite->PLineEnd = gc->PLineEnd;
			ite->PLineJoin = gc->PLineJoin;
			ite->setTextFlowsAroundFrame(false);
			ite->DashOffset = gc->dashOffset;
			ite->DashValues = gc->dashArray;
			/*			if (gc->Gradient != 0)
						{
							ite->fill_gradient = gc->GradCo;
							ScApp->view->SelItem.append(ite);
							ScApp->view->ItemGradFill(gc->Gradient, gc->GCol2, 100, gc->GCol1, 100);
							ScApp->view->SelItem.clear();
						} */
			GElements.append(ite);
			Elements.append(ite);
			delete( m_gc.pop() );
		}
	}
	else
	{
		SvgStyle *gc = m_gc.current();
		//int z = ScApp->view->PaintText(x, y - qRound(gc->FontSize / 10.0), 10, 10, gc->LWidth, gc->FillCol);
		int z = currDoc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y - qRound(gc->FontSize / 10.0), 10, 10, gc->LWidth, "None", gc->FillCol, !ScApp->view->Mpressed);
		PageItem* ite = currDoc->Items.at(z);
		ite->Extra = 0;
		ite->TExtra = 0;
		ite->BExtra = 0;
		ite->RExtra = 0;
		ite->LineSp = gc->FontSize / 10.0 + 2;
		ScApp->SetNewFont(gc->Family);
		ite->IFont = gc->Family;
		ite->TxtFill = gc->FillCol;
		ite->ShTxtFill = 100;
		ite->TxtStroke = gc->StrokeCol;
		ite->ShTxtStroke = 100;
		ite->ISize = gc->FontSize;
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
		for (uint cc = 0; cc<Text.length(); ++cc)
		{
			hg = new ScText;
			hg->ch = Text.at(cc);
			hg->cfont = (*currDoc->AllFonts)[gc->Family];
			hg->csize = gc->FontSize;
			hg->ccolor = gc->FillCol;
			hg->cextra = 0;
			hg->cshade = 100;
			hg->cstroke = gc->StrokeCol;
			hg->cshade2 = 100;
			hg->cscalev = 1000;
			hg->cscale = 1000;
			hg->cbase = 0;
			hg->cshadowx = 50;
			hg->cshadowy = -50;
			hg->coutline = 10;
			hg->cunderpos = -1;
			hg->cunderwidth = -1;
			hg->cstrikepos = -1;
			hg->cstrikewidth = -1;
			hg->cselect = false;
			if( !e.attribute( "stroke" ).isEmpty() )
				hg->cstyle = 4;
			else
				hg->cstyle = 0;
			hg->cab = 0;
			hg->xp = 0;
			hg->yp = 0;
			hg->PRot = 0;
			hg->PtransX = 0;
			hg->PtransY = 0;
			hg->cembedded = 0;
			ite->itemText.append(hg);
			ite->Width += RealCWidth(currDoc, hg->cfont, hg->ch, hg->csize)+1;
			ite->Height = ite->LineSp+desc+2;
		}
		ite->SetRectFrame();
		ScApp->view->setRedrawBounding(ite);
		if( !e.attribute("id").isEmpty() )
			ite->setItemName(" "+e.attribute("id"));
		ite->setFillTransparency(gc->Transparency);
		ite->setLineTransparency(gc->TranspStroke);
		ite->PLineEnd = gc->PLineEnd;
		ite->PLineJoin = gc->PLineJoin;
		ite->setTextFlowsAroundFrame(false);
		ite->DashOffset = gc->dashOffset;
		ite->DashValues = gc->dashArray;
		/*		if (gc->Gradient != 0)
				{
					ite->fill_gradient = gc->GradCo;
					ScApp->view->SelItem.append(ite);
					ScApp->view->ItemGradFill(gc->Gradient, gc->GCol2, 100, gc->GCol1, 100);
					ScApp->view->SelItem.clear();
				} */
		GElements.append(ite);
		Elements.append(ite);
	}
	p.end();
	return GElements;
}

/*!
 \fn SVGPlug::~SVGPlug()
 \author Franz Schmid
 \date
 \brief Destructor
 \param None
 \retval None
 */
SVGPlug::~SVGPlug()
{}
