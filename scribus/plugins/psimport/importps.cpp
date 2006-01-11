#include "importps.h"
#include "importps.moc"

#include "scconfig.h"
#include "scribus.h"

#include "customfdialog.h"
#include "mpalette.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "scribusXml.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qcursor.h>
#include <qdragobject.h>
#include <qregexp.h>
#include <cmath>
#include <cstdlib>
#include "undomanager.h"
#include "scpaths.h"
#include "scfontmetrics.h"
#include "selection.h"
#include "util.h"
#include "prefsmanager.h"

/*!
 \fn EPSPlug::EPSPlug( QWidget* parent, ScribusMainWindow *plug, QString fName )
 \author Franz Schmid
 \date
 \brief Create the EPS importer window.
 \param parent QWidget *
 \param plug ScribusMainWindow *
 \param fName QString
 \retval EPSPlug plugin
 */
EPSPlug::EPSPlug(QString fName, bool isInteractive)
{
	interactive = isInteractive;
	double x, y, b, h, c, m, k;
	bool ret = false;
	bool found = false;
	CustColors.clear();
	QFileInfo fi = QFileInfo(fName);
	QString ext = fi.extension(false).lower();
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = PrefsManager::instance()->appPrefs.PageWidth;
	h = PrefsManager::instance()->appPrefs.PageHeight;
	if ((ext == "eps") || (ext == "ps"))
	{
		QString tmp, BBox, tmp2, FarNam;
		ScColor cc;
		QFile f(fName);
		if (f.open(IO_ReadOnly))
		{
/* Try to find Bounding Box */
			QTextStream ts(&f);
			while (!ts.atEnd())
			{
				tmp = ts.readLine();
				if (tmp.startsWith("%%BoundingBox:"))
				{
					found = true;
					BBox = tmp.remove("%%BoundingBox:");
				}
				if (!found)
				{
					if (tmp.startsWith("%%BoundingBox"))
					{
						found = true;
						BBox = tmp.remove("%%BoundingBox");
					}
				}
/* Read CustomColors if available */
				if (tmp.startsWith("%%CMYKCustomColor"))
				{
					tmp = tmp.remove(0,18);
					QTextStream ts2(&tmp, IO_ReadOnly);
					ts2 >> c >> m >> y >> k;
					FarNam = ts2.read();
					FarNam = FarNam.stripWhiteSpace();
					FarNam = FarNam.remove(0,1);
					FarNam = FarNam.remove(FarNam.length()-1,1);
					QRegExp badchars("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]");
					FarNam = FarNam.simplifyWhiteSpace().replace( badchars, "_" );
					cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
					cc.setSpotColor(true);
					CustColors.insert(FarNam, cc);
					while (!ts.atEnd())
					{
						tmp = ts.readLine();
						if (!tmp.startsWith("%%+"))
							break;
						tmp = tmp.remove(0,3);
						QTextStream ts2(&tmp, IO_ReadOnly);
						ts2 >> c >> m >> y >> k;
						FarNam = ts2.read();
						FarNam = FarNam.stripWhiteSpace();
						FarNam = FarNam.remove(0,1);
						FarNam = FarNam.remove(FarNam.length()-1,1);
						QRegExp badchars("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]");
						FarNam = FarNam.simplifyWhiteSpace().replace( badchars, "_" );
						cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
						cc.setSpotColor(true);
						CustColors.insert(FarNam, cc);
					}
				}
				if (tmp.startsWith("%%EndComments"))
					break;
			}
			f.close();
			if (found)
			{
				QStringList bb = QStringList::split(" ", BBox);
				if (bb.count() == 4)
				{
					QTextStream ts2(&BBox, IO_ReadOnly);
					ts2 >> x >> y >> b >> h;
				}
			}
		}
	}
	if (!interactive)
	{
		ScMW->doc->setPage(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false);
		ScMW->doc->addPage(0);
		ScMW->view->addPage(0, true);
	}
	else
	{
		if (!ScMW->HaveDoc)
		{
			ScMW->doFileNew(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom");
			ScMW->HaveNewDoc();
			ret = true;
		}
	}
	if ((ret) || (!interactive))
	{
		if (b-x > h-y)
			ScMW->doc->PageOri = 1;
		else
			ScMW->doc->PageOri = 0;
		ScMW->doc->PageSize = "Custom";
	}
	ColorList::Iterator it;
	ScribusDoc* Doku = ScMW->doc;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!Doku->PageColors.contains(it.key()))
			Doku->PageColors.insert(it.key(), it.data());
	}
	Elements.clear();
	FPoint minSize = Doku->minCanvasCoordinate;
	FPoint maxSize = Doku->maxCanvasCoordinate;
	Doku->setLoading(true);
	Doku->DoDrawing = false;
	ScMW->view->setUpdatesEnabled(false);
	ScMW->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QString CurDirP = QDir::currentDirPath();
	QDir::setCurrent(fi.dirPath());
	if (convert(fName, x, y, b, h))
	{
		//ScMW->view->SelItem.clear();
		Doku->selection->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (interactive))
		{
			for (uint a = 0; a < Elements.count(); ++a)
			{
				Elements.at(a)->Groups.push(Doku->GroupCounter);
			}
			Doku->GroupCounter++;
		}
		Doku->DoDrawing = true;
		ScMW->view->setUpdatesEnabled(true);
		ScMW->ScriptRunning = false;
		Doku->setLoading(false);
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		if ((Elements.count() > 0) && (!ret) && (interactive))
		{
			Doku->DragP = true;
			Doku->DraggedElem = 0;
			Doku->DragElements.clear();
			for (uint dre=0; dre<Elements.count(); ++dre)
			{
				Doku->DragElements.append(Elements.at(dre)->ItemNr);
				//ScMW->view->SelItem.append(Elements.at(dre));
				Doku->selection->addItem(Elements.at(dre));
			}
			ScMW->view->setGroupRect();
			ScriXmlDoc *ss = new ScriXmlDoc();
			//QDragObject *dr = new QTextDrag(ss->WriteElem(&ScMW->view->SelItem, Doku, ScMW->view), ScMW->view->viewport());
			QDragObject *dr = new QTextDrag(ss->WriteElem(Doku, ScMW->view, Doku->selection),ScMW->view->viewport());
#ifndef QT_MAC
// see #2196		
			ScMW->view->DeleteItem();
#endif
			ScMW->view->resizeContents(qRound((maxSize.x() - minSize.x()) * ScMW->view->getScale()), qRound((maxSize.y() - minSize.y()) * ScMW->view->getScale()));
			ScMW->view->scrollBy(qRound((Doku->minCanvasCoordinate.x() - minSize.x()) * ScMW->view->getScale()), qRound((Doku->minCanvasCoordinate.y() - minSize.y()) * ScMW->view->getScale()));
			Doku->minCanvasCoordinate = minSize;
			Doku->maxCanvasCoordinate = maxSize;
			ScMW->view->updateContents();
			dr->setPixmap(loadIcon("DragPix.xpm"));
#if 0
			qDebug("psimport: data");
			QString data(dr->encodedData("text/plain"));
			for (uint i=0; i <= data.length() / 4000; i++) {
				qDebug(data.mid(i*4000, 4000));
			}
			qDebug("psimport: enddata");
			qDebug(QString("psimport: drag type %1").arg(dr->format()));
#endif
			if (!dr->drag())
				qDebug("psimport: couldn't start dragging!");
			delete ss;
			Doku->DragP = false;
			Doku->DraggedElem = 0;
			Doku->DragElements.clear();
		}
		else
		{
			Doku->setModified(false);
			ScMW->slotDocCh();
		}
	}
	else
	{
		QDir::setCurrent(CurDirP);
		Doku->DoDrawing = true;
		ScMW->view->setUpdatesEnabled(true);
		ScMW->ScriptRunning = false;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	if (interactive)
		Doku->setLoading(false);
}

/*!
 \fn void EPSPlug::convert(QString fn, double b, double h)
 \author Franz Schmid
 \date
 \brief Does the conversion.
 \param fn QString
 \param b double
 \param h double
 \retval bool true if conversion was ok
 */
bool EPSPlug::convert(QString fn, double x, double y, double b, double h)
{
	QStringList args;
	QString cmd, cmd1, cmd2, cmd3, tmp, tmp2, tmp3, tmp4;
	// import.prolog do not cope with filenames containing blank spaces
	// so take care that output filename does not (win32 compatibility)
	QString tmpFile = getShortPathName(ScMW->PrefsPfad)+ "/ps.out";
	QString pfad = ScPaths::instance().libDir();
	QString pfad2 = QDir::convertSeparators(pfad + "import.prolog");
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	args.append( getShortPathName(PrefsManager::instance()->ghostscriptExecutable()) );
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dNODISPLAY" );
	args.append( "-dBATCH" );
	// Add any extra font paths being used by Scribus to gs's font search
	// path We have to use Scribus's prefs context, not a plugin context, to
	// get to the required information.
	PrefsContext *pc = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(extraFonts->get(i,0));
	if( !cmd.isEmpty() )
		args.append( cmd );
	// then finish building the command and call gs
	args.append( QString("-g%1x%2").arg(tmp2.setNum(qRound(b-x))).arg(tmp3.setNum(qRound(h-y))) );
	args.append( "-c" );
	args.append( tmp.setNum(-x) );
	args.append( tmp.setNum(-y) );
	args.append( "translate" );
	args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(tmpFile)) );
	args.append( pfad2 );
	args.append( QDir::convertSeparators(fn) );
	args.append( "-c" );
	args.append( "flush" );
	args.append( "cfile" );
	args.append( "closefile" );
	args.append( "quit" );
	QCString finalCmd = args.join(" ").local8Bit();
	int ret = System(args);
	if (ret != 0)
	{
		qDebug("PostScript import failed when calling gs as: \n%s\n", finalCmd.data());
		qDebug("Error code: %i", ret);
		QString mess = tr("Importing File:\n%1\nfailed!").arg(fn);
		QMessageBox::critical(0, tr("Fatal Error"), mess, 1, 0, 0);
		return false;
	}
	if (ext == "eps")
		parseOutput(tmpFile, true);
	else
		parseOutput(tmpFile, false);
	QFile::remove(tmpFile);
	return true;
}

/*!
 \fn void EPSPlug::parseOutput(QString fn)
 \author Franz Schmid
 \date
 \brief Parses the Output Ghostscript has created.
 \param None
 \retval None
 */
void EPSPlug::parseOutput(QString fn, bool eps)
{
	QString tmp, token, params, lasttoken, lastPath, currPath;
	int z, lcap, ljoin, dc, pagecount;
	double dcp;
	PageItem* ite;
	QFile f(fn);
	lasttoken = "";
	pagecount = 1;
	if (f.open(IO_ReadOnly))
	{
		lastPath = "";
		currPath = "";
		LineW = 0;
		Opacity = 1;
		CurrColor = "None";
		JoinStyle = MiterJoin;
		CapStyle = FlatCap;
		DashPattern.clear();
		QTextStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = "";
			tmp = ts.readLine();
			QTextStream Code(&tmp, IO_ReadOnly);
			Code >> token;
			params = Code.read();
			if ((lasttoken == "sp") && (!interactive) && (!eps))
			{
				ScMW->doc->addPage(pagecount);
				ScMW->view->addPage(pagecount, true);
				pagecount++;
			}
			if (token == "n")
			{
				Coords.resize(0);
				FirstM = true;
				WasM = false;
				ClosedPath = false;
			}
			else if (token == "m")
				WasM = true;
			else if (token == "c")
			{
				Curve(&Coords, params);
				currPath += params;
			}
			else if (token == "l")
			{
				LineTo(&Coords, params);
				currPath += params;
			}
			else if (token == "f")
			{
				if (Coords.size() != 0)
				{
					if ((Elements.count() != 0) && (lastPath == currPath))
					{
						ite = Elements.at(Elements.count()-1);
						ite->setFillColor(CurrColor);
						ite->setFillTransparency(1.0 - Opacity);
					}
					else
					{
						if (ClosedPath)
							//z = ScMW->view->PaintPoly(0, 0, 10, 10, LineW, CurrColor, "None");
							z = ScMW->doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, 0, 0, 10, 10, LineW, CurrColor, "None", true);
						else
							//z = ScMW->view->PaintPolyLine(0, 0, 10, 10, LineW, CurrColor, "None");
							z = ScMW->doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, 0, 0, 10, 10, LineW, CurrColor, "None", true);
						ite = ScMW->doc->Items->at(z);
						ite->PoLine = Coords.copy();
						ite->PoLine.translate(ScMW->doc->currentPage->xOffset(), ScMW->doc->currentPage->yOffset());
						ite->ClipEdited = true;
						ite->FrameType = 3;
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(),wh.y());
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->setFillTransparency(1.0 - Opacity);
						ScMW->view->AdjustItemSize(ite);
						Elements.append(ite);
					}
					lastPath = currPath;
					currPath = "";
				}
			}
			else if (token == "s")
			{
				if (Coords.size() != 0)
				{
					LineW = QMAX(LineW, 0.1); // Set Linewidth to be a least 0.1 pts, a Stroke without a Linewidth makes no sense
					if ((Elements.count() != 0) && (lastPath == currPath))
					{
						ite = Elements.at(Elements.count()-1);
						ite->setLineColor(CurrColor);
						ite->Pwidth = LineW;
						ite->PLineEnd = CapStyle;
						ite->PLineJoin = JoinStyle;
						ite->setLineTransparency(1.0 - Opacity);
						ite->DashOffset = DashOffset;
						ite->DashValues = DashPattern;
					}
					else
					{
						ScribusDoc* Doku = ScMW->doc;
						if (ClosedPath)
							//z = ScMW->view->PaintPoly(0, 0, 10, 10, LineW, "None", CurrColor);
							z = Doku->itemAdd(PageItem::Polygon, PageItem::Unspecified, 0, 0, 10, 10, LineW, "None", CurrColor, true);
						else
							//z = ScMW->view->PaintPolyLine(0, 0, 10, 10, LineW, "None", CurrColor);
							z = Doku->itemAdd(PageItem::PolyLine, PageItem::Unspecified, 0, 0, 10, 10, LineW, "None", CurrColor, true);
						ite = Doku->Items->at(z);
						ite->PoLine = Coords.copy();
						ite->PoLine.translate(Doku->currentPage->xOffset(), Doku->currentPage->yOffset());
						ite->ClipEdited = true;
						ite->FrameType = 3;
						ite->PLineEnd = CapStyle;
						ite->PLineJoin = JoinStyle;
						ite->DashOffset = DashOffset;
						ite->DashValues = DashPattern;
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(), wh.y());
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->setLineTransparency(1.0 - Opacity);
						ScMW->view->AdjustItemSize(ite);
						Elements.append(ite);
					}
					lastPath = currPath;
					currPath = "";
				}
			}
			else if (token == "co")
				CurrColor = parseColor(params);
			else if (token == "corgb")
				CurrColor = parseColor(params, colorModelRGB);
			else if (token == "ci")
			{
				Coords.resize(0);
				lastPath = "";
				currPath = "";
			}
			else if (token == "w")
			{
				QTextStream Lw(&params, IO_ReadOnly);
				Lw >> LineW;
				currPath += params;
			}
			else if (token == "ld")
			{
				QTextStream Lw(&params, IO_ReadOnly);
				Lw >> dc;
				Lw >> DashOffset;
				DashPattern.clear();
				if (dc != 0)
				{
					for (int dcc = 0; dcc < dc; ++dcc)
					{
						Lw >> dcp;
						DashPattern.append(dcp);
					}
				}
				currPath += params;
			}
			else if (token == "lc")
			{
				QTextStream Lw(&params, IO_ReadOnly);
				Lw >> lcap;
				switch (lcap)
				{
					case 0:
						CapStyle = Qt::FlatCap;
						break;
					case 1:
						CapStyle = Qt::RoundCap;
						break;
					case 2:
						CapStyle = Qt::SquareCap;
						break;
					default:
						CapStyle = Qt::FlatCap;
						break;
				}
				currPath += params;
			}
			else if (token == "lj")
			{
				QTextStream Lw(&params, IO_ReadOnly);
				Lw >> ljoin;
				switch (ljoin)
				{
					case 0:
						JoinStyle = Qt::MiterJoin;
						break;
					case 1:
						JoinStyle = Qt::RoundJoin;
						break;
					case 2:
						JoinStyle = Qt::BevelJoin;
						break;
					default:
						JoinStyle = Qt::MiterJoin;
						break;
				}
				currPath += params;
			}
			else if (token == "cp")
				ClosedPath = true;
			lasttoken = token;
		}
		f.close();
	}
}

/*!
 \fn void EPSPlug::LineTo(FPointArray *i, QString vals)
 \author Franz Schmid
 \date
 \brief
 \param i FPointArray *
 \param vals QString
 \retval none
*/

void EPSPlug::LineTo(FPointArray *i, QString vals)
{
	if (vals.isEmpty())
		return;
	double x1, x2, y1, y2;
	QTextStream Code(&vals, IO_ReadOnly);
	Code >> x1;
	Code >> y1;
	Code >> x2;
	Code >> y2;
	if ((!FirstM) && (WasM))
		i->setMarker();
	FirstM = false;
	WasM = false;
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x2, y2));
	i->addPoint(FPoint(x2, y2));
}

/*!
 \fn void EPSPlug::Curve(FPointArray *i, QString vals)
 \author Franz Schmid
 \date
 \brief
 \param i FPointArray *
 \param vals QString
 \retval None
 */
void EPSPlug::Curve(FPointArray *i, QString vals)
{
	if (vals.isEmpty())
		return;
	double x1, x2, y1, y2, x3, y3, x4, y4;
	QTextStream Code(&vals, IO_ReadOnly);
	Code >> x1;
	Code >> y1;
	Code >> x2;
	Code >> y2;
	Code >> x3;
	Code >> y3;
	Code >> x4;
	Code >> y4;
	if ((!FirstM) && (WasM))
		i->setMarker();
	FirstM = false;
	WasM = false;
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x2, y2));
	i->addPoint(FPoint(x4, y4));
	i->addPoint(FPoint(x3, y3));
}

/*!
 \fn QString EPSPlug::parseColor(QString vals)
 \author Franz Schmid
 \date
 \brief Returns a Color Name, if the Color doesn't exist it's created
 \param vals QString
 \retval QString Color Name
 */
QString EPSPlug::parseColor(QString vals, colorModel model)
{
	ScribusDoc* Doku = ScMW->doc;
	QString ret = "None";
	if (vals.isEmpty())
		return ret;
	double c, m, y, k, r, g, b;
	ScColor tmp;
	ColorList::Iterator it;
	QTextStream Code(&vals, IO_ReadOnly);
	bool found = false;
	if (model == colorModelRGB)
	{
		Code >> r;
		Code >> g;
		Code >> b;
		Code >> Opacity;
		int Rc = static_cast<int>(r * 255 + 0.5);
		int Gc = static_cast<int>(g * 255 + 0.5);
		int Bc = static_cast<int>(b * 255 + 0.5);
		int hR, hG, hB;
		tmp.setColorRGB(Rc, Gc, Bc);
		for (it = Doku->PageColors.begin(); it != Doku->PageColors.end(); ++it)
		{
			Doku->PageColors[it.key()].getRGB(&hR, &hG, &hB);
			if ((Rc == hR) && (Gc == hG) && (Bc == hB))
			{
				if (Doku->PageColors[it.key()].getColorModel() == colorModelRGB)
				{
					ret = it.key();
					found = true;
				}
			}
		}
	}
	else
	{
		Code >> c;
		Code >> m;
		Code >> y;
		Code >> k;
		Code >> Opacity;
		int Cc = static_cast<int>(c * 255 + 0.5);
		int Mc = static_cast<int>(m * 255 + 0.5);
		int Yc = static_cast<int>(y * 255 + 0.5);
		int Kc = static_cast<int>(k * 255 + 0.5);
		int hC, hM, hY, hK;
		tmp.setColor(Cc, Mc, Yc, Kc);
		for (it = Doku->PageColors.begin(); it != Doku->PageColors.end(); ++it)
		{
			Doku->PageColors[it.key()].getCMYK(&hC, &hM, &hY, &hK);
			if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
			{
				if (Doku->PageColors[it.key()].getColorModel() == colorModelCMYK)
				{
					ret = it.key();
					found = true;
				}
			}
		}
	}
	if (!found)
	{
		Doku->PageColors.insert("FromEPS"+tmp.name(), tmp);
		ScMW->propertiesPalette->updateColorList();
		ret = "FromEPS"+tmp.name();
	}
	return ret;
}
