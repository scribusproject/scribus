#include "importps.h"
#include "importps.moc"

#include "scconfig.h"

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
#include <cmath>
#include <cstdlib>
#include "undomanager.h"
#include "pluginmanager.h"
#include "scpaths.h"
#include "scfontmetrics.h"
#include "util.h"
#include "prefsmanager.h"

/*!
 \fn QString Name()
 \author Franz Schmid
 \date
 \brief Returns name of plugin
 \param None
 \retval QString containing name of plugin: Import EPS/PDF/PS...
 */
QString name()
{
	return QObject::tr("Import &EPS/PS...");
}

/*!
 \fn int Type()
 \author Franz Schmid
 \date
 \brief Returns type of plugin
 \param None
 \retval int containing type of plugin (1: Extra, 2: Import, 3: Export, 4: Resident Plugin)
 */
PluginManager::PluginType type()
{
	return PluginManager::Import;
}

int ID()
{
	return 6;
}


QString actionName()
{
	return "ImportPS";
}

QString actionKeySequence()
{
	return "";
}

QString actionMenu()
{
	return "FileImport";
}

QString actionMenuAfterName()
{
	return "";
}

bool actionEnabledOnStartup()
{
	return true;
}

/*!
 \fn void Run(QWidget *d, ScribusApp *plug)
 \author Franz Schmid
 \date
 \brief Run the EPS import
 \param d QWidget *
 \param plug ScribusApp *
 \retval None
 */
void run(QWidget *d, ScribusApp *plug)
{
	QString fileName;
	if (!plug->pluginManager->dllInput.isEmpty())
	{
		fileName = plug->pluginManager->dllInput;
		UndoManager::instance()->setUndoEnabled(false);
	}
	else
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importps");
		QString wdir = prefs->get("wdir", ".");
		QString formats = QObject::tr("All Supported Formats (*.eps *.EPS *.ps *.PS);;");
		formats += "EPS (*.eps *.EPS);;PS (*.ps *.PS);;" + QObject::tr("All Files (*)");
		CustomFDialog diaf(d, wdir, QObject::tr("Open"), formats);
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.findRev("/")));
		}
		else
			return;
	}
	if (UndoManager::undoEnabled() && plug->HaveDoc)
	{
		UndoManager::instance()->beginTransaction(plug->doc->currentPage->getUName(),Um::IImageFrame,Um::ImportEPS, fileName, Um::IEPS);
	}
	else if (UndoManager::undoEnabled() && !plug->HaveDoc)
		UndoManager::instance()->setUndoEnabled(false);
	EPSPlug *dia = new EPSPlug(plug, fileName);
	if (UndoManager::undoEnabled())
		UndoManager::instance()->commit();
	else
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
}

/*!
 \fn EPSPlug::EPSPlug( QWidget* parent, ScribusApp *plug, QString fName )
 \author Franz Schmid
 \date
 \brief Create the EPS importer window.
 \param parent QWidget *
 \param plug ScribusApp *
 \param fName QString
 \retval EPSPlug plugin
 */
EPSPlug::EPSPlug( ScribusApp *plug, QString fName )
{
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
		QString tmp, BBox, tmp2, dummy, FarNam;
		QChar tc;
		CMYKColor cc;
		QFile f(fName);
		if (f.open(IO_ReadOnly))
		{
/* Try to find Bounding Box */
			QTextStream ts(&f);
			while (!ts.atEnd())
			{
				tc = ' ';
				tmp = "";
				while ((tc != '\n') && (tc != '\r'))
				{
					ts >> tc;
					if ((tc != '\n') && (tc != '\r'))
						tmp += tc;
				}
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
					QTextStream ts2(&tmp, IO_ReadOnly);
					ts2 >> dummy >> c >> m >> y >> k;
					FarNam = ts2.read();
					FarNam = FarNam.stripWhiteSpace();
					FarNam = FarNam.remove(0,1);
					FarNam = FarNam.remove(FarNam.length()-1,1);
					cc = CMYKColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
					CustColors.insert(FarNam, cc);
					while (!ts.atEnd())
					{
						tc = ' ';
						tmp = "";
						while ((tc != '\n') && (tc != '\r'))
						{
							ts >> tc;
							if ((tc != '\n') && (tc != '\r'))
								tmp += tc;
						}
						if (!tmp.startsWith("%%+"))
							break;
						QTextStream ts2(&tmp, IO_ReadOnly);
						ts2 >> dummy >> c >> m >> y >> k;
						FarNam = ts2.read();
						FarNam = FarNam.stripWhiteSpace();
						FarNam = FarNam.remove(0,1);
						FarNam = FarNam.remove(FarNam.length()-1,1);
						cc = CMYKColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
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
	Prog = plug;
	if (!plug->pluginManager->dllInput.isEmpty())
	{
		Prog->doc->setPage(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false);
		Prog->view->addPage(0);
	}
	else
	{
		if (!Prog->HaveDoc)
		{
			Prog->doFileNew(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom");
			ret = true;
		}
	}
	if ((ret) || (!Prog->pluginManager->dllInput.isEmpty()))
	{
		if (b-x > h-y)
			Prog->doc->PageOri = 1;
		else
			Prog->doc->PageOri = 0;
		Prog->doc->PageSize = "Custom";
	}
	Doku = plug->doc;
	ColorList::Iterator it;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!Doku->PageColors.contains(it.key()))
			Doku->PageColors.insert(it.key(), it.data());
	}
	Elements.clear();
	FPoint minSize = Doku->minCanvasCoordinate;
	FPoint maxSize = Doku->maxCanvasCoordinate;
	Doku->loading = true;
	Doku->DoDrawing = false;
	Prog->view->setUpdatesEnabled(false);
	Prog->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QString CurDirP = QDir::currentDirPath();
	QDir::setCurrent(fi.dirPath());
	if (convert(fName, x, y, b, h))
	{
		Prog->view->SelItem.clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (plug->pluginManager->dllInput.isEmpty()))
		{
			for (uint a = 0; a < Elements.count(); ++a)
			{
				Elements.at(a)->Groups.push(Doku->GroupCounter);
			}
			Doku->GroupCounter++;
		}
		Doku->DoDrawing = true;
		Prog->view->setUpdatesEnabled(true);
		Prog->ScriptRunning = false;
		Doku->loading = false;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		if ((Elements.count() > 0) && (!ret) && (plug->pluginManager->dllInput.isEmpty()))
		{
			Doku->DragP = true;
			Doku->DraggedElem = 0;
			Doku->DragElements.clear();
			for (uint dre=0; dre<Elements.count(); ++dre)
			{
				Doku->DragElements.append(Elements.at(dre)->ItemNr);
				Prog->view->SelItem.append(Elements.at(dre));
			}
			ScriXmlDoc *ss = new ScriXmlDoc();
			Prog->view->setGroupRect();
			QDragObject *dr = new QTextDrag(ss->WriteElem(&Prog->view->SelItem, Doku, Prog->view), Prog->view->viewport());
			Prog->view->DeleteItem();
			Prog->view->resizeContents(qRound((maxSize.x() - minSize.x()) * Prog->view->getScale()), qRound((maxSize.y() - minSize.y()) * Prog->view->getScale()));
			Prog->view->scrollBy(qRound((Doku->minCanvasCoordinate.x() - minSize.x()) * Prog->view->getScale()), qRound((Doku->minCanvasCoordinate.y() - minSize.y()) * Prog->view->getScale()));
			Doku->minCanvasCoordinate = minSize;
			Doku->maxCanvasCoordinate = maxSize;
			Prog->view->updateContents();
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
	else
	{
		QDir::setCurrent(CurDirP);
		Doku->DoDrawing = true;
		Prog->view->setUpdatesEnabled(true);
		Prog->ScriptRunning = false;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	if (plug->pluginManager->dllInput.isEmpty())
		Doku->loading = false;
	plug->pluginManager->dllInput = "";
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
	QString cmd1, cmd2, tmp, tmp2, tmp3, tmp4;
	QString tmpFile = Prog->PrefsPfad+"/ps.out";
	QString pfad = ScPaths::instance().libDir();
	QString pfad2;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	cmd1 = PrefsManager::instance()->ghostscriptExecutable();
	pfad2 = QDir::convertSeparators(pfad + "import.prolog");
	cmd1 += " -q -dNOPAUSE -dNODISPLAY";
	cmd1 += " -dBATCH -g"+tmp2.setNum(qRound(b-x))+"x"+tmp3.setNum(qRound(h-y))+" -c "+tmp4.setNum(-x)+" "+tmp.setNum(-y)+" translate";
	// Add any extra font paths being used by Scribus to gs's font search
	// path We have to use Scribus's prefs context, not a plugin context, to
	// get to the required information.
	PrefsContext *pc = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	if (extraFonts->getRowCount() >= 1)
		cmd1 += QString(" -sFONTPATH='%1'").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd1 += QString(":'%1'").arg(extraFonts->get(i,0));
	// then finish building the command and call gs
	cmd1 += " -sOutputFile="+tmpFile+" "+pfad2+" ";
	cmd2 = " -c flush cfile closefile quit";
	QCString finalCmd = QString(cmd1 + "\""+fn+"\"" + cmd2).local8Bit();
	bool ret = system(finalCmd);
	if (ret != 0)
	{
		qDebug("PostScript import failed when calling gs as: \n%s\n", finalCmd.data());
		QString mess = tr("Importing File:\n%1\nfailed!").arg(fn);
		QMessageBox::critical(0, tr("Fatal Error"), mess, 1, 0, 0);
		return false;
	}
	if (ext == "eps")
		parseOutput(tmpFile, true);
	else
		parseOutput(tmpFile, false);
	system("rm -f "+tmpFile);
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
			if ((lasttoken == "sp") && (!Prog->pluginManager->dllInput.isEmpty()) && (!eps))
			{
				Prog->view->addPage(pagecount);
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
							z = Prog->view->PaintPoly(0, 0, 10, 10, LineW, CurrColor, "None");
						else
							z = Prog->view->PaintPolyLine(0, 0, 10, 10, LineW, CurrColor, "None");
						ite = Doku->Items.at(z);
						ite->PoLine = Coords.copy();
						ite->PoLine.translate(Doku->currentPage->Xoffset, Doku->currentPage->Yoffset);
						ite->ClipEdited = true;
						ite->FrameType = 3;
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->Width = wh.x();
						ite->Height = wh.y();
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->setFillTransparency(1.0 - Opacity);
						Prog->view->AdjustItemSize(ite);
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
						if (ClosedPath)
							z = Prog->view->PaintPoly(0, 0, 10, 10, LineW, "None", CurrColor);
						else
							z = Prog->view->PaintPolyLine(0, 0, 10, 10, LineW, "None", CurrColor);
						ite = Doku->Items.at(z);
						ite->PoLine = Coords.copy();
						ite->PoLine.translate(Doku->currentPage->Xoffset, Doku->currentPage->Yoffset);
						ite->ClipEdited = true;
						ite->FrameType = 3;
						ite->PLineEnd = CapStyle;
						ite->PLineJoin = JoinStyle;
						ite->DashOffset = DashOffset;
						ite->DashValues = DashPattern;
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->Width = wh.x();
						ite->Height = wh.y();
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->setLineTransparency(1.0 - Opacity);
						Prog->view->AdjustItemSize(ite);
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
	QString ret = "None";
	if (vals.isEmpty())
		return ret;
	double c, m, y, k, r, g, b;
	CMYKColor tmp;
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
		Prog->propertiesPalette->Cpal->SetColors(Doku->PageColors);
		ret = "FromEPS"+tmp.name();
	}
	return ret;
}
