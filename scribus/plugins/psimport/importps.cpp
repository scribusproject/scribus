#include "importps.h"
#include "importps.moc"
#if (_MSC_VER >= 1200)
	#include "win-config.h"
#else
	#include "config.h"
#endif
#include "customfdialog.h"
#include "mpalette.h"
#include "prefsfile.h"
#include "scribusXml.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qcursor.h>
#include <cmath>
#include <cstdlib>

extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern QPixmap loadIcon(QString nam);
extern FPoint GetMaxClipF(FPointArray Clip);
extern PrefsFile* prefsFile;

/*!
 \fn QString Name()
 \author Franz Schmid
 \date
 \brief Returns name of plugin
 \param None
 \retval QString containing name of plugin: Import EPS/PDF/PS...
 */
QString Name()
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
int Type()
{
	return 2;
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
void Run(QWidget *d, ScribusApp *plug)
{
	QString fileName;
	if (plug->DLLinput != "")
		fileName = plug->DLLinput;
	else
	{
		PrefsContext* prefs = prefsFile->getPluginContext("importps");
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
	EPSPlug *dia = new EPSPlug(plug, fileName);
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
	b = plug->Prefs.PageBreite;
	h = plug->Prefs.PageHoehe;
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
	if (plug->DLLinput != "")
	{
		Prog->doc->setPage(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false);
		Prog->view->addPage(0);
	}
	else
	{
		if (!Prog->HaveDoc)
		{
			Prog->doFileNew(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1);
			ret = true;
		}
	}
	Doku = plug->doc;
	CListe::Iterator it;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!Doku->PageColors.contains(it.key()))
			Doku->PageColors.insert(it.key(), it.data());
	}
	Elements.clear();
	Doku->loading = true;
	Doku->DoDrawing = false;
	Prog->view->setUpdatesEnabled(false);
	Prog->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QString CurDirP = QDir::currentDirPath();
	QDir::setCurrent(fi.dirPath());
	if (convert(fName, x, y, b, h))
	{
		QDir::setCurrent(CurDirP);
		if (Elements.count() > 0)
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
		Doku->loading = false;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		if ((Elements.count() > 0) && (!ret) && (plug->DLLinput == ""))
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
	if (plug->DLLinput == "")
		Doku->loading = false;
	plug->DLLinput = "";
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
	QString pfad = PREL;
	QString pfad2;
	cmd1 = Prog->Prefs.gs_exe;
	pfad2 = QDir::convertSeparators(pfad + "/lib/scribus/import.prolog");
	cmd1 += " -q -dNOPAUSE -dNODISPLAY";
	cmd1 += " -dBATCH -g"+tmp2.setNum(qRound(b-x))+"x"+tmp3.setNum(qRound(h-y))+" -c "+tmp4.setNum(-x)+" "+tmp.setNum(-y)+" translate";
	cmd1 += " -sOutputFile="+tmpFile+" "+pfad2+" ";
	cmd2 = " -c flush cfile closefile quit";
	bool ret = system(cmd1 + "\""+fn+"\"" + cmd2);
	if (ret != 0)
	{
		QString mess = tr("Importing File:\n%1\nfailed!").arg(fn);
		QMessageBox::critical(0, tr("Fatal Error"), mess, 1, 0, 0);
		return false;
	}
	parseOutput(tmpFile);
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
void EPSPlug::parseOutput(QString fn)
{
	QString tmp, token, params, lasttoken, lastPath, currPath;
	int z, lcap, ljoin, dc;
	double dcp;
	PageItem* ite;
	QFile f(fn);
	lasttoken = "";
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
						ite->Pcolor = CurrColor;
						ite->Transparency = 1.0 - Opacity;
					}
					else
					{
						if (ClosedPath)
							z = Prog->view->PaintPoly(0, 0, 10, 10, LineW, CurrColor, "None");
						else
							z = Prog->view->PaintPolyLine(0, 0, 10, 10, LineW, CurrColor, "None");
						ite = Doku->Items.at(z);
						ite->PoLine = Coords.copy();
						ite->PoLine.translate(Doku->ActPage->Xoffset, Doku->ActPage->Yoffset);
						ite->ClipEdited = true;
						ite->FrameType = 3;
						FPoint wh = GetMaxClipF(ite->PoLine);
						ite->Width = wh.x();
						ite->Height = wh.y();
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->Transparency = 1.0 - Opacity;
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
						ite->Pcolor2 = CurrColor;
						ite->Pwidth = LineW;
						ite->PLineEnd = CapStyle;
						ite->PLineJoin = JoinStyle;
						ite->TranspStroke = 1.0 - Opacity;
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
						ite->PoLine.translate(Doku->ActPage->Xoffset, Doku->ActPage->Yoffset);
						ite->ClipEdited = true;
						ite->FrameType = 3;
						ite->PLineEnd = CapStyle;
						ite->PLineJoin = JoinStyle;
						ite->DashOffset = DashOffset;
						ite->DashValues = DashPattern;
						FPoint wh = GetMaxClipF(ite->PoLine);
						ite->Width = wh.x();
						ite->Height = wh.y();
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->TranspStroke = 1.0 - Opacity;
						Prog->view->AdjustItemSize(ite);
						Elements.append(ite);
					}
					lastPath = currPath;
					currPath = "";
				}
			}
			else if (token == "co")
				CurrColor = parseColor(params);
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
			else if (token == "sp")
				break;
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
	if (vals == "")
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
	if (vals == "")
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
QString EPSPlug::parseColor(QString vals)
{
	QString ret = "None";
	if (vals == "")
		return ret;
	double c, m, y, k;
	QTextStream Code(&vals, IO_ReadOnly);
	Code >> c;
	Code >> m;
	Code >> y;
	Code >> k;
	Code >> Opacity;
	int Cc = static_cast<int>(c*255);
	int Mc = static_cast<int>(m*255);
	int Yc = static_cast<int>(y*255);
	int Kc = static_cast<int>(k*255);
	int hC, hM, hY, hK;
	CMYKColor tmp = CMYKColor(Cc, Mc, Yc, Kc);
	CListe::Iterator it;
	bool found = false;
	for (it = Doku->PageColors.begin(); it != Doku->PageColors.end(); ++it)
	{
		Doku->PageColors[it.key()].getCMYK(&hC, &hM, &hY, &hK);
		if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
		{
			ret = it.key();
			found = true;
		}
	}
	if (!found)
	{
		Doku->PageColors.insert("FromEPS"+tmp.name(), tmp);
		Prog->Mpal->Cpal->SetColors(Doku->PageColors);
		ret = "FromEPS"+tmp.name();
	}
	return ret;
}

