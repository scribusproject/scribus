#include "svgplugin.h"
#include "svgplugin.moc"
#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif
#include "customfdialog.h"
#include "color.h"
#include "scribusXml.h"
#include "mpalette.h"
#include "prefsfile.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qcursor.h>
#include <cmath>
#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

using namespace std;

extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern bool loadText(QString nam, QString *Buffer);
extern QPixmap loadIcon(QString nam);
extern double RealCWidth(ScribusDoc *doc, QString name, QString ch, int Siz);
extern FPoint GetMaxClipF(FPointArray Clip);
extern PrefsFile* prefsFile;

/*!
 \fn QString Name()
 \author Franz Schmid
 \date
 \brief Returns name of plugin
 \param None
 \retval QString containing name of plugin: Import SVG-Image...
 */
QString Name()
{
	return QObject::tr("Import &SVG...");
}

/*!
 \fn int Type()
 \author Franz Schmid
 \date
 \brief Returns type of plugin
 \param None
 \retval int containing type of plugin (1: Extra, 2: Import, 3: Export, 4: )
 */
int Type()
{
	return 2;
}

int ID()
{
	return 10;
}

/*!
 \fn void Run(QWidget *d, ScribusApp *plug)
 \author Franz Schmid
 \date
 \brief Run the SVG import
 \param d QWidget *
 \param plug ScribusApp *
 \retval None
 */
void Run(QWidget *d, ScribusApp *plug)
{
	QString fileName;
	if (plug->DLLinput != "")
		fileName = plug->DLLinput;
	else
	{
		PrefsContext* prefs = prefsFile->getPluginContext("SVGPlugin");
		QString wdir = prefs->get("wdir", ".");
#ifdef HAVE_LIBZ
		CustomFDialog diaf(d, wdir, QObject::tr("Open"), QObject::tr("SVG-Images (*.svg *.svgz);;All Files (*)"));
#else
		CustomFDialog diaf(d, wdir, QObject::tr("Open"), QObject::tr("SVG-Images (*.svg);;All Files (*)"));
#endif
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.findRev("/")));
		}
		else
			return;
	}
	SVGPlug *dia = new SVGPlug(plug, fileName);
	delete dia;
}

/*!
 \fn SVGPlug::SVGPlug( QWidget* parent, ScribusApp *plug, QString fName )
 \author Franz Schmid
 \date
 \brief Create the SVG importer window
 \param parent QWidget *
 \param plug ScribusApp *
 \param fName QString
 \retval SVGPlug plugin
 */
SVGPlug::SVGPlug( ScribusApp *plug, QString fName )
{
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
	Prog = plug;
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
	QDomElement docElem = inpdoc.documentElement();
	double width = !docElem.attribute("width").isEmpty() ? parseUnit(docElem.attribute( "width" )) : 550.0;
	double height = !docElem.attribute("height").isEmpty() ? parseUnit(docElem.attribute( "height" )) : 841.0;
	if (Prog->DLLinput != "")
	{
		Prog->doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
		Prog->view->addPage(0);
	}
	else
	{
		if (!Prog->HaveDoc)
		{
			Prog->doFileNew(width, height, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom");
			ret = true;
		}
	}
	if ((ret) || (Prog->DLLinput != ""))
	{
		if (width > height)
			Prog->doc->PageOri = 1;
		else
			Prog->doc->PageOri = 0;
		Prog->doc->PageSize = "Custom";
	}
	Doku = Prog->doc;
	Prog->view->Deselect();
	Elements.clear();
	Doku->loading = true;
	Doku->DoDrawing = false;
	Prog->view->setUpdatesEnabled(false);
	Prog->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	gc->Family = Doku->Dfont;
	if (!Doku->PageColors.contains("Black"))
		Doku->PageColors.insert("Black", CMYKColor(0, 0, 0, 255));
	m_gc.push( gc );
	parseGroup( docElem );
	if (Elements.count() > 1)
	{
		Prog->view->SelItem.clear();
		for (uint a = 0; a < Elements.count(); ++a)
		{
			Elements.at(a)->Groups.push(Doku->GroupCounter);
			if (!ret)
				Prog->view->SelItem.append(Elements.at(a));
		}
		Doku->GroupCounter++;
	}
	Doku->DoDrawing = true;
	Prog->view->setUpdatesEnabled(true);
	Prog->ScriptRunning = false;
	if (Prog->DLLinput == "")
		Doku->loading = false;
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	if ((Elements.count() > 0) && (!ret) && (Prog->DLLinput == ""))
	{
		Doku->DragP = true;
		Doku->DraggedElem = 0;
		Doku->DragElements.clear();
		for (uint dre=0; dre<Elements.count(); ++dre)
		{
			Doku->DragElements.append(Elements.at(dre)->ItemNr);
		}
		ScriXmlDoc *ss = new ScriXmlDoc();
		Prog->view->setGroupRect();
		QDragObject *dr = new QTextDrag(ss->WriteElem(&Prog->view->SelItem, Doku, Prog->view), Prog->view->viewport());
		Prog->view->DeleteItem();
		dr->setPixmap(loadIcon("DragPix.xpm"));
		dr->drag();
		delete ss;
		Doku->DragP = false;
		Doku->DraggedElem = 0;
		Doku->DragElements.clear();
	}
	else
	{
		Doku->setUnModified();
		Prog->slotDocCh();
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
void SVGPlug::parseGroup(const QDomElement &e)
{
	QPtrList<PageItem> GElements;
	FPointArray ImgClip;
	ImgClip.resize(0);
	double BaseX = Doku->ActPage->Xoffset;
	double BaseY = Doku->ActPage->Yoffset;
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
			parseGroup( b );
			for (uint gr = 0; gr < GElements.count(); ++gr)
			{
				GElements.at(gr)->Groups.push(Doku->GroupCounter);
			}
			delete( m_gc.pop() );
			GElements.clear();
			Doku->GroupCounter++;
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
			double x		= parseUnit( b.attribute( "x" ) );
			double y		= parseUnit( b.attribute( "y" ) );
			double width = parseUnit( b.attribute( "width" ));
			double height = parseUnit( b.attribute( "height" ) );
			double rx = b.attribute( "rx" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "rx" ) );
			double ry = b.attribute( "ry" ).isEmpty() ? 0.0 : parseUnit( b.attribute( "ry" ) );
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			z = Prog->view->PaintRect(BaseX, BaseY, width, height, gc->LWidth, gc->FillCol, gc->StrokeCol);
			PageItem* ite = Doku->Items.at(z);
			if ((rx != 0) || (ry != 0))
			{
				ite->RadRect = QMAX(rx, ry);
				Prog->view->SetFrameRound(ite);
			}
			QWMatrix mm = QWMatrix();
			mm.translate(x, y);
			ite->PoLine.map(mm);
			FPoint wh = GetMaxClipF(ite->PoLine);
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
			z = Prog->view->PaintEllipse(BaseX, BaseY, rx * 2.0, ry * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol);
			PageItem* ite = Doku->Items.at(z);
			QWMatrix mm = QWMatrix();
			mm.translate(x, y);
			ite->PoLine.map(mm);
			FPoint wh = GetMaxClipF(ite->PoLine);
			ite->Width = wh.x();
			ite->Height = wh.y();
		}
		else if( STag == "circle" )
		{
			addGraphicContext();
			double r		= parseUnit( b.attribute( "r" ) );
			double x		= parseUnit( b.attribute( "cx" ) ) - r;
			double y		= parseUnit( b.attribute( "cy" ) ) - r;
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			z = Prog->view->PaintEllipse(BaseX, BaseY, r * 2.0, r * 2.0, gc->LWidth, gc->FillCol, gc->StrokeCol);
			PageItem* ite = Doku->Items.at(z);
			QWMatrix mm = QWMatrix();
			mm.translate(x, y);
			ite->PoLine.map(mm);
			FPoint wh = GetMaxClipF(ite->PoLine);
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
			z = Prog->view->PaintPoly(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
			PageItem* ite = Doku->Items.at(z);
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
			z = Prog->view->PaintPoly(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
			PageItem* ite = Doku->Items.at(z);
			ite->PoLine.resize(0);
			if (parseSVG( b.attribute( "d" ), &ite->PoLine ))
				ite->PType = 7;
			if (ite->PoLine.size() < 4)
			{
				Prog->view->SelItem.append(ite);
				Prog->view->DeleteItem();
				z = -1;
			}
		}
		else if( STag == "polyline" || b.tagName() == "polygon" )
		{
			addGraphicContext();
			SvgStyle *gc = m_gc.current();
			parseStyle( gc, b );
			if( b.tagName() == "polygon" )
				z = Prog->view->PaintPoly(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
			else
				z = Prog->view->PaintPolyLine(BaseX, BaseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol);
			PageItem* ite = Doku->Items.at(z);
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
					svgMoveTo(x, y);
					bFirst = false;
					WasM = true;
				}
				else
				{
					x = (*(it++)).toDouble();
					y = (*it).toDouble();
					svgLineTo(&ite->PoLine, x, y);
				}
			}
			if( STag == "polygon" )
				svgClosePath(&ite->PoLine);
			else
				ite->PType = 7;
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
			z = Prog->view->PaintPict(x+BaseX, y+BaseY, w, h);
			if (fname != "")
				Prog->view->LoadPict(fname, z);
			PageItem* ite = Doku->Items.at(z);
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
			PageItem* ite = Doku->Items.at(z);
			switch (ite->PType)
			{
			case 2:
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
						ite->LocalViewX = ite->LocalScX;
						ite->LocalViewY = ite->LocalScY;
						Prog->view->AdjustPreview(ite);
					}
					break;
				}
			case 4:
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
					ite->Pwidth = ite->Pwidth * ((mm.m11() + mm.m22()) / 2.0);
					FPoint wh = GetMaxClipF(ite->PoLine);
					ite->Width = wh.x();
					ite->Height = wh.y();
					ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
					Prog->view->AdjustItemSize(ite);
					break;
				}
			}
			if( !b.attribute("id").isEmpty() )
				ite->AnName += " "+b.attribute("id");
			ite->Transparency = gc->Transparency;
			ite->TranspStroke = gc->TranspStroke;
			ite->PLineEnd = gc->PLineEnd;
			ite->PLineJoin = gc->PLineJoin;
			ite->Textflow = false;
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
					QWMatrix mm;
					if ((gc->GY1 < gc->GY2) && (gc->GX1 < gc->GX2))
					{
						mm.rotate(-angle2);
						mm.rotate(angle1);
					}
					FPointArray gra;
					gra.setPoints(2, ite->GrStartX-dx, ite->GrStartY-dy, ite->GrEndX-dx, ite->GrEndY-dy);
					gra.map(mm);
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
		value = value;
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
	QWMatrix result;

	// Split string for handling 1 transform statement at a time
	QStringList subtransforms = QStringList::split(')', transform);
	QStringList::ConstIterator it = subtransforms.begin();
	QStringList::ConstIterator end = subtransforms.end();
	for(; it != end; ++it)
	{
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
				double x = params[1].toDouble();
				double y = params[2].toDouble();

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
				result.translate(params[0].toDouble(), params[1].toDouble());
			else    // Spec : if only one param given, assume 2nd param to be 0
				result.translate(params[0].toDouble() , 0);
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
				if (sx == 0)
					sx = 1.0;
				double sy = params[3].toDouble();
				if (sy == 0)
					sy = 1.0;
				result.setMatrix(sx, params[1].toDouble(), params[2].toDouble(), sy, params[4].toDouble(), params[5].toDouble());
			}
		}
	}
	return result;
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
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'h':
				{
					ptr = getCoord( ptr, tox );
					curx = curx + tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'H':
				{
					ptr = getCoord( ptr, tox );
					curx = tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'v':
				{
					ptr = getCoord( ptr, toy );
					cury = cury + toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'V':
				{
					ptr = getCoord( ptr, toy );
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
					largeArc = tox == 1;
					ptr = getCoord( ptr, tox );
					sweep = tox == 1;
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
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
 \fn void SVGPlug::svgMoveTo(FPointArray *i, double x1, double y1)
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
	CListe::Iterator it;
	bool found = false;
	for (it = Doku->PageColors.begin(); it != Doku->PageColors.end(); ++it)
	{
		if (c == Doku->PageColors[it.key()].getRGBColor())
		{
			ret = it.key();
			found = true;
		}
	}
	if (!found)
	{
		CMYKColor tmp;
		tmp.fromQColor(c);
		Doku->PageColors.insert("FromSVG"+c.name(), tmp);
		Prog->Mpal->Cpal->SetColors(Doku->PageColors);
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
	if( command == "fill" )
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
			obj->Gradient = m_gradients[key].Type;
			obj->GradCo = m_gradients[key].gradient;
			obj->CSpace = m_gradients[key].CSpace;
			obj->GX1 = m_gradients[key].X1;
			obj->GY1 = m_gradients[key].Y1;
			obj->GX2 = m_gradients[key].X2;
			obj->GY2 = m_gradients[key].Y2;
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
	else if( command == "stroke-opacity" )
		obj->TranspStroke = 1.0 - fromPercentage(params);
	else if( command == "fill-opacity" )
		obj->Transparency = 1.0 - fromPercentage(params);
	else if( command == "opacity" )
	{
		obj->Transparency = 1.0 - fromPercentage(params);
		obj->TranspStroke = 1.0 - fromPercentage(params);
	}
	else if( command == "font-family" )
	{
		QString family = params;
		QString ret = "";
		family.replace( QRegExp( "'" ) , QChar( ' ' ) );
		obj->Family = Doku->Dfont; // family;
		bool found = false;
		SCFontsIterator it(Prog->Prefs.AvailFonts);
		for ( ; it.current(); ++it)
		{
			QString fam;
			QString &fn = it.current()->SCName;
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
			obj->Family = Doku->Dfont;
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
	double offset, opa;
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
		gradient->gradient.addStop( Doku->PageColors[Col].getRGBColor(), offset, 0.5, opa, Col, 100 );
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
	gradhelper.gradient.clearStops();
	gradhelper.gradient.setRepeatMethod( VGradient::none );

	QString href = e.attribute("xlink:href").mid(1);
	double x1, y1, x2, y2;
	if (!href.isEmpty())
	{
		gradhelper.Type = m_gradients[href].Type;
		gradhelper.gradient = m_gradients[href].gradient;
		gradhelper.X1 = m_gradients[href].X1;
		gradhelper.Y1 = m_gradients[href].Y1;
		gradhelper.X2 = m_gradients[href].X2;
		gradhelper.Y2 = m_gradients[href].Y2;
		gradhelper.CSpace = m_gradients[href].CSpace;
		gradhelper.matrix = m_gradients[href].matrix;
	}
	if (e.tagName() == "linearGradient")
	{
		x1 = e.attribute( "x1", "0").toDouble();
		y1 = e.attribute( "y1", "0" ).toDouble();
		x2 = e.attribute( "x2", "1" ).toDouble();
		y2 = e.attribute( "y2", "0" ).toDouble();
		gradhelper.X1 = x1;
		gradhelper.Y1 = y1;
		gradhelper.X2 = x2;
		gradhelper.Y2 = y2;
		gradhelper.Type = 6;
	}
	else
	{
		x1 = e.attribute( "cx", "0.5").toDouble();
		y1 = e.attribute( "cy", "0.5" ).toDouble();
		x2 = e.attribute( "r", "0.5" ).toDouble();
		y2 = y1;
		gradhelper.X1 = x1;
		gradhelper.Y1 = y1;
		gradhelper.X2 = x1 + x2;
		gradhelper.Y2 = y1;
		gradhelper.Type = 7;
	}
	if ( !e.attribute( "gradientUnits" ).isEmpty() )
	{
		QString uni = e.attribute( "gradientUnits");
		if (uni == "userSpaceOnUse")
			gradhelper.CSpace = true;
		else
			gradhelper.CSpace = false;
	}
	else
		gradhelper.CSpace = false;
	QString transf = e.attribute("gradientTransform");
	if( !transf.isEmpty() )
	{
		gradhelper.matrix = parseTransform( e.attribute( "gradientTransform" ) );
/*		QWMatrix mat = parseTransform( e.attribute( "gradientTransform" ) );
		SvgStyle *gc = m_gc.current();
		mat = mat * gc->matrix;
		FPointArray gra;
		gra.setPoints(2, gradhelper.X1, gradhelper.Y1, gradhelper.X2, gradhelper.Y2);
		gra.map(mat);
		gradhelper.X1 = gra.point(0).x();
		gradhelper.Y1 = gra.point(0).y();
		gradhelper.X2 = gra.point(1).x();
		gradhelper.Y2 = gra.point(1).y(); */
	}
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
	struct Pti *hg;
	QPainter p;
	QPtrList<PageItem> GElements;
	p.begin(Prog->view->viewport());
	QFont ff(Doku->UsedFonts[m_gc.current()->Family]);
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
			int z = Prog->view->PaintText(x, y, 10, 10, gc->LWidth, gc->FillCol);
			PageItem* ite = Doku->Items.at(z);
			ite->Extra = 0;
			ite->TExtra = 0;
			ite->BExtra = 0;
			ite->RExtra = 0;
			ite->LineSp = gc->FontSize / 10.0 + 2;
			ite->Height = ite->LineSp+desc+2;
			Prog->SetNewFont(gc->Family);
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
			ite->TxtScale = 100;
			for (uint tt = 0; tt < Text.length(); ++tt)
			{
				hg = new Pti;
				hg->ch = Text.at(tt);
				hg->cfont = gc->Family;
				hg->csize = gc->FontSize;
				hg->ccolor = gc->FillCol;
				hg->cextra = 0;
				hg->cshade = 100;
				hg->cstroke = gc->StrokeCol;
				hg->cshade2 = 100;
				hg->cscale = 100;
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
				ite->Ptext.append(hg);
				tempW += RealCWidth(Doku, hg->cfont, hg->ch, hg->csize)+1;
				if (hg->ch == QChar(13))
				{
					ite->Height += ite->LineSp+desc;
					ite->Width = QMAX(ite->Width, tempW);
					tempW = 0;
				}
			}
			ite->Width = QMAX(ite->Width, tempW);
			Prog->view->SetRectFrame(ite);
			ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
			Prog->view->SelItem.append(ite);
			Prog->view->HowTo = 1;
			Prog->view->setGroupRect();
			Prog->view->scaleGroup(mm.m11(), mm.m22());
			Prog->view->Deselect();
			ite->Ypos -= asce * mm.m22();
			if( !e.attribute("id").isEmpty() )
				ite->AnName += " "+e.attribute("id");
			ite->Transparency = gc->Transparency;
			ite->TranspStroke = gc->TranspStroke;
			ite->PLineEnd = gc->PLineEnd;
			ite->PLineJoin = gc->PLineJoin;
			ite->Textflow = false;
			ite->DashOffset = gc->dashOffset;
			ite->DashValues = gc->dashArray;
			/*			if (gc->Gradient != 0)
						{
							ite->fill_gradient = gc->GradCo;
							Prog->view->SelItem.append(ite);
							Prog->view->ItemGradFill(gc->Gradient, gc->GCol2, 100, gc->GCol1, 100);
							Prog->view->SelItem.clear();
						} */
			GElements.append(ite);
			Elements.append(ite);
			delete( m_gc.pop() );
		}
	}
	else
	{
		SvgStyle *gc = m_gc.current();
		int z = Prog->view->PaintText(x, y - qRound(gc->FontSize / 10.0), 10, 10, gc->LWidth, gc->FillCol);
		PageItem* ite = Doku->Items.at(z);
		ite->Extra = 0;
		ite->TExtra = 0;
		ite->BExtra = 0;
		ite->RExtra = 0;
		ite->LineSp = gc->FontSize / 10.0 + 2;
		Prog->SetNewFont(gc->Family);
		ite->IFont = gc->Family;
		ite->TxtFill = gc->FillCol;
		ite->ShTxtFill = 100;
		ite->TxtStroke = gc->StrokeCol;
		ite->ShTxtStroke = 100;
		ite->ISize = gc->FontSize;
		ite->TxTStyle = 0;
		ite->TxtScale = 100;
		for (uint cc = 0; cc<Text.length(); ++cc)
		{
			hg = new Pti;
			hg->ch = Text.at(cc);
			hg->cfont = gc->Family;
			hg->csize = gc->FontSize;
			hg->ccolor = gc->FillCol;
			hg->cextra = 0;
			hg->cshade = 100;
			hg->cstroke = gc->StrokeCol;
			hg->cshade2 = 100;
			hg->cscale = 100;
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
			ite->Ptext.append(hg);
			ite->Width += RealCWidth(Doku, hg->cfont, hg->ch, hg->csize)+1;
			ite->Height = ite->LineSp+desc+2;
		}
		Prog->view->SetRectFrame(ite);
		if( !e.attribute("id").isEmpty() )
			ite->AnName += " "+e.attribute("id");
		ite->Transparency = gc->Transparency;
		ite->TranspStroke = gc->TranspStroke;
		ite->PLineEnd = gc->PLineEnd;
		ite->PLineJoin = gc->PLineJoin;
		ite->Textflow = false;
		ite->DashOffset = gc->dashOffset;
		ite->DashValues = gc->dashArray;
		/*		if (gc->Gradient != 0)
				{
					ite->fill_gradient = gc->GradCo;
					Prog->view->SelItem.append(ite);
					Prog->view->ItemGradFill(gc->Gradient, gc->GCol2, 100, gc->GCol1, 100);
					Prog->view->SelItem.clear();
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

