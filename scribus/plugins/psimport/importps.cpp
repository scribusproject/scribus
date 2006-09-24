/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "importps.h"
#include "importps.moc"

#include "scconfig.h"
#include "scribus.h"
#include "scribuscore.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "mpalette.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "scribusXml.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qcursor.h>
#include <qdragobject.h>
#include <qregexp.h>
#include <qptrstack.h>
#include <qvaluestack.h>
#include <cmath>
#include <cstdlib>

#include "multiprogressdialog.h"
#include "scpaths.h"
#include "selection.h"
#include "prefsmanager.h"
#include "undomanager.h"
#include "loadsaveplugin.h"
#include "util.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

EPSPlug::EPSPlug(ScribusDoc* doc, QString fName, int flags, bool showProgress)
{
	m_Doc=doc;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	cancel = false;
	double x, y, b, h, c, m, k;
	bool ret = false;
	bool found = false;
	CustColors.clear();
	QFileInfo fi = QFileInfo(fName);
	QString ext = fi.extension(false).lower();
	if ( !ScCore->usingGUI() ) {
		interactive = false;
		showProgress = false;
	}
	if ( showProgress ) 
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		progressDialog = new MultiProgressDialog( tr("Importing PostScript"), CommonStrings::tr_Cancel, mw, "psexportprogress");
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing PostScript:");
		QValueList<bool> barsNumeric;
		barsNumeric << false;
		progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		progressDialog->setOverallTotalSteps(3);
		progressDialog->setOverallProgress(0);
		progressDialog->setProgress("GI", 0);
		progressDialog->show();
		connect(progressDialog->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelRequested()));
		qApp->processEvents();
	}
	else {
		progressDialog = NULL;
	}
	
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
					cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k), m_Doc);
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
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
		}
	}
	if ((ret) || (!interactive))
	{
		if (b-x > h-y)
			m_Doc->PageOri = 1;
		else
			m_Doc->PageOri = 0;
		m_Doc->m_pageSize = "Custom";
	}
	ColorList::Iterator it;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
			m_Doc->PageColors.insert(it.key(), it.data());
	}
	Elements.clear();
	FPoint minSize = m_Doc->minCanvasCoordinate;
	FPoint maxSize = m_Doc->maxCanvasCoordinate;
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->view()->setUpdatesEnabled(false);
	m_Doc->scMW()->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QString CurDirP = QDir::currentDirPath();
	QDir::setCurrent(fi.dirPath());
	if (convert(fName, x, y, b, h))
	{
		m_Doc->m_Selection->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (interactive))
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
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, m_Doc->toolSettings.dWidth, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
			PageItem *neu = m_Doc->Items->take(z);
			m_Doc->Items->insert(lowestItem, neu);
			neu->Groups.push(m_Doc->GroupCounter);
			neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
			neu->isGroupControl = true;
			neu->groupsLastItem = high;
			for (uint a = 0; a < doc->Items->count(); ++a)
			{
				m_Doc->Items->at(a)->ItemNr = a;
			}
			Elements.prepend(neu);
			m_Doc->GroupCounter++;
		}
		m_Doc->DoDrawing = true;
		m_Doc->view()->setUpdatesEnabled(true);
		m_Doc->scMW()->ScriptRunning = false;
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
				m_Doc->m_Selection->addItem(Elements.at(dre));
			}
			m_Doc->view()->setGroupRect();
			ScriXmlDoc *ss = new ScriXmlDoc();
			QDragObject *dr = new QTextDrag(ss->WriteElem(m_Doc, m_Doc->view(), m_Doc->m_Selection),m_Doc->view()->viewport());
#ifndef QT_MAC
// see #2196
			m_Doc->itemSelection_DeleteItem();
#else
			qDebug("psimport: leaving items on page");
#endif
			m_Doc->view()->resizeContents(qRound((maxSize.x() - minSize.x()) * m_Doc->view()->scale()), qRound((maxSize.y() - minSize.y()) * m_Doc->view()->scale()));
			m_Doc->view()->scrollBy(qRound((m_Doc->minCanvasCoordinate.x() - minSize.x()) * m_Doc->view()->scale()), qRound((m_Doc->minCanvasCoordinate.y() - minSize.y()) * m_Doc->view()->scale()));
			m_Doc->minCanvasCoordinate = minSize;
			m_Doc->maxCanvasCoordinate = maxSize;
			m_Doc->view()->updateContents();
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
			m_Doc->DragP = false;
			m_Doc->DraggedElem = 0;
			m_Doc->DragElements.clear();
		}
		else
		{
			m_Doc->changed();
		}
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->view()->setUpdatesEnabled(true);
		m_Doc->scMW()->ScriptRunning = false;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	if (interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if (showProgress)
		m_Doc->view()->DrawNew();
}

bool EPSPlug::convert(QString fn, double x, double y, double b, double h)
{
	QStringList args;
	QString cmd, cmd1, cmd2, cmd3, tmp, tmp2, tmp3, tmp4;
	// import.prolog do not cope with filenames containing blank spaces
	// so take care that output filename does not (win32 compatibility)
	QString tmpFile = getShortPathName(PrefsManager::instance()->preferencesLocation())+ "/ps.out";
	QString errFile = getShortPathName(PrefsManager::instance()->preferencesLocation())+ "/ps.err";
	QString pfad = ScPaths::instance().libDir();
	QString pfad2 = QDir::convertSeparators(pfad + "import.prolog");
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	
	if (progressDialog) {
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	args.append( getShortPathName(PrefsManager::instance()->ghostscriptExecutable()) );
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-sDEVICE=nullpage" );
	args.append( "-dBATCH" );
	args.append( "-dDELAYBIND" );
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
	args.append( QString("-g%1x%2").arg(tmp2.setNum(qRound((b-x)))).arg(tmp3.setNum(qRound((h-y)))) );
	args.append( "-r72");
	args.append( "-dTextAlphaBits=4" );
	args.append( "-dGraphicsAlphaBits=4" );
	args.append( "-c" );
	args.append( tmp.setNum(-x) );
	args.append( tmp.setNum(-y) );
	args.append( "translate" );
	args.append( QString("-sTraceFile=%1").arg(QDir::convertSeparators(tmpFile)) );
	QString exportPath = m_Doc->DocName + "-" + fi.baseName();
	QFileInfo exportFi(exportPath);
	if ( !exportFi.isWritable() ) {
		PrefsContext* docContext = PrefsManager::instance()->prefsFile->getContext("docdirs", false);
		QString docDir = ".";
		QString prefsDocDir=PrefsManager::instance()->documentDir();
		if (!prefsDocDir.isEmpty())
			docDir = docContext->get("docsopen", prefsDocDir);
		else
			docDir = docContext->get("docsopen", ".");		
		exportFi.setFile(docDir + "/" + exportFi.baseName());
	}
	//qDebug(QString("using export path %1").arg(exportFi.absFilePath()));
	args.append( QString("-sExportFiles=%1").arg(QDir::convertSeparators(exportFi.absFilePath())) );
	args.append( pfad2 );
	args.append( QDir::convertSeparators(fn) );
	args.append( "-c" );
	args.append( "flush" );
	args.append( "cfile" );
	args.append( "closefile" );
	args.append( "quit" );
	QCString finalCmd = args.join(" ").local8Bit();
	int ret = System(args, errFile, errFile);
	if (ret != 0)
	{
		qDebug("PostScript import failed when calling gs as: \n%s\n", finalCmd.data());
		qDebug("Ghostscript diagnostics:\n");
		QFile diag(errFile);
		if (diag.open(IO_ReadOnly) && !diag.atEnd() ) {
			QString line;
			while (diag.readLine(line, 120) > 0) {
				qDebug("\t%s", line.ascii());
			}
			diag.close();
		}
		else {
			qDebug("-- no output --");
		}
		QString mess = tr("Importing File:\n%1\nfailed!").arg(fn);
		QMessageBox::critical(0, tr("Fatal Error"), mess, 1, 0, 0);
		return false;
	}
	if(progressDialog) {
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	if (!cancel) {
		parseOutput(tmpFile, ext == "eps");
	}
	QFile::remove(tmpFile);
	if (progressDialog)
		progressDialog->close();
	return true;
}

void EPSPlug::parseOutput(QString fn, bool eps)
{
	QString tmp, token, params, lasttoken, lastPath, currPath;
	int z, lcap, ljoin, dc, pagecount;
	int failedImages = 0;
	double dcp;
	bool fillRuleEvenOdd = true;
	PageItem* ite;
	QPtrStack<PageItem> groupStack;
	QValueStack<int> gsStack;
	QValueStack<int> gsStackMarks;
	QFile f(fn);
	lasttoken = "";
	pagecount = 1;
	if (f.open(IO_ReadOnly))
	{
		if (progressDialog) {
			progressDialog->setTotalSteps("GI", (int) f.size());
			qApp->processEvents();
		}
		lastPath = "";
		currPath = "";
		LineW = 0;
		Opacity = 1;
		CurrColor = CommonStrings::None;
		JoinStyle = MiterJoin;
		CapStyle = FlatCap;
		DashPattern.clear();
		QTextStream ts(&f);
		int line_cnt = 0;
		while (!ts.atEnd() && !cancel)
		{
			tmp = "";
			tmp = ts.readLine();
			if (progressDialog && (++line_cnt % 100 == 0)) {
				progressDialog->setProgress("GI", (int) f.at());
				qApp->processEvents();
			}
			token = tmp.section(' ', 0, 0);
			params = tmp.section(' ', 1, -1, QString::SectionIncludeTrailingSep);
			if (lasttoken == "sp"  && !eps && token != "sp" ) //av: messes up anyway: && (!interactive))
			{
				m_Doc->addPage(pagecount);
				m_Doc->view()->addPage(pagecount, true);
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
			else if (token == "fill-winding")
			{
				fillRuleEvenOdd = false;
			}
			else if (token == "fill-evenodd")
			{
				fillRuleEvenOdd = true;
			}
			else if (token == "f")
			{
				//TODO: pattern -> Imageframe + Clip
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
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, 0, 0, 10, 10, LineW, CurrColor, CommonStrings::None, true);
						else
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, 0, 0, 10, 10, LineW, CurrColor, CommonStrings::None, true);
						ite = m_Doc->Items->at(z);
						ite->PoLine = Coords.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
						ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
						ite->ClipEdited = true;
						ite->FrameType = 3;
						ite->fillRule = (fillRuleEvenOdd);
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(),wh.y());
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->setFillTransparency(1.0 - Opacity);
						m_Doc->view()->AdjustItemSize(ite);
						if (groupStack.count() != 0)
						{
							QValueStack<int> groupOld = groupStack.top()->Groups;
							for (uint gg = 0; gg < groupOld.count(); gg++)
							{
								ite->Groups.push(groupOld[gg]);
							}
						}
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
						ite->setLineWidth(LineW);
						ite->PLineEnd = CapStyle;
						ite->PLineJoin = JoinStyle;
						ite->setLineTransparency(1.0 - Opacity);
						ite->DashOffset = DashOffset;
						ite->DashValues = DashPattern;
					}
					else
					{
						if (ClosedPath)
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, 0, 0, 10, 10, LineW, CommonStrings::None, CurrColor, true);
						else
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, 0, 0, 10, 10, LineW, CommonStrings::None, CurrColor, true);
						ite = m_Doc->Items->at(z);
						ite->PoLine = Coords.copy(); //FIXME: try to avoid copy when FPointArray is properly shared
						ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
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
						m_Doc->view()->AdjustItemSize(ite);
						if (groupStack.count() != 0)
						{
							QValueStack<int> groupOld = groupStack.top()->Groups;
							for (uint gg = 0; gg < groupOld.count(); gg++)
							{
								ite->Groups.push(groupOld[gg]);
							}
						}
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
				clipCoords = Coords;
				if (Coords.size() != 0)
				{
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, 0, 0, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
					ite = m_Doc->Items->at(z);
					ite->PoLine = Coords.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
					ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->ClipEdited = true;
					ite->FrameType = 3;
//					ite->fillRule = (fillRuleEvenOdd);
					FPoint wh = getMaxClipF(&ite->PoLine);
					ite->setWidthHeight(wh.x(),wh.y());
					ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
//					ite->setFillTransparency(1.0 - Opacity);
					m_Doc->view()->AdjustItemSize(ite);
					ite->Groups.push(m_Doc->GroupCounter);
					if (groupStack.count() != 0)
					{
						QValueStack<int> groupOld = groupStack.top()->Groups;
						for (uint gg = 0; gg < groupOld.count(); gg++)
						{
							ite->Groups.push(groupOld[gg]);
						}
					}
					ite->isGroupControl = true;
					ite->setItemName( tr("Group%1").arg(ite->Groups.top()));
					Elements.append(ite);
					groupStack.push(ite);
					gsStackMarks.push(gsStack.count());
					m_Doc->GroupCounter++;
				}
				Coords   = FPointArray(0);
				lastPath = "";
				currPath = "";
			}
			else if (token == "gs")
			{
				gsStack.push(1);
			}
			else if (token == "gr")
			{
				gsStack.pop();
				if (groupStack.count() != 0)
				{
					if (gsStack.count() < gsStackMarks.top())
					{
						PageItem *ite = groupStack.pop();
						ite->groupsLastItem = Elements.at(Elements.count()-1);
						gsStackMarks.pop();
					}
				}
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
			else if (token == "cp") {
				ClosedPath = true;
			}
			else if (token == "im") {
				if ( !Image(params) )
					++failedImages;
			}
			lasttoken = token;
		}
		f.close();
		if (groupStack.count() != 0)
		{
			while (!groupStack.isEmpty())
			{
				PageItem *ite = groupStack.pop();
				ite->groupsLastItem = Elements.at(Elements.count()-1);
			}
		}
	}
	if (failedImages > 0)
	{
		QString mess = tr("Converting of %1 images failed!").arg(failedImages);
		QMessageBox::critical(0, tr("Error"), mess, 1, 0, 0);
	}
}

bool EPSPlug::Image(QString vals)
{
	double x, y, w, h, angle;
	int horpix, verpix;
	QString filename, device;
	QTextStream Code(&vals, IO_ReadOnly);
	Code >> x;
	Code >> y;
	Code >> w;
	Code >> h;
	Code >> angle;
	Code >> horpix;
	Code >> verpix;
	Code >> device;
	filename = Code.read().stripWhiteSpace();
	if (device.startsWith("psd")) {
		filename = filename.mid(0, filename.length()-3) + "psd";
	}
		
	qDebug(QString("import %6 image %1: %2x%3 @ (%4,%5) °%5").arg(filename).arg(w).arg(h).arg(x).arg(y).arg(angle).arg(device));
	QString rawfile = filename.mid(0, filename.length()-3) + "dat";
	QStringList args;
	args.append( getShortPathName(PrefsManager::instance()->ghostscriptExecutable()) );
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( QString("-sDEVICE=%1").arg(device) );    
	args.append( "-dBATCH" );
	args.append( QString("-g%1x%2").arg(horpix).arg(verpix) );
	args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(filename)) );
	args.append( QDir::convertSeparators(rawfile) );
	args.append( "-c" );
	args.append( "showpage" );
	args.append( "quit" );
	QCString finalCmd = args.join(" ").local8Bit();
	int ret = System(args);
	if (ret != 0)
	{
		qDebug("PostScript image conversion failed when calling gs as: \n%s\n", finalCmd.data());
		qDebug("Ghostscript diagnostics: %d\n", ret);
		QFile diag(filename);
		if (diag.open(IO_ReadOnly)) {
			QString line;
			long int len;
			bool gs_error = false;
			do {
				len = diag.readLine(line, 120);
				gs_error |= line.contains("Error");
				if (gs_error)
					qDebug("\t%s", line.ascii());
			}
			while (len > 0);
			diag.close();
			}
		else {
			qDebug("-- no output --");
		}
		qDebug("Failed file was:\n");
		QFile dat(rawfile);
		if (dat.open(IO_ReadOnly)) {
			QString line;
			long int len;
			do {
				len = dat.readLine(line, 120);
				qDebug("\t%s", line.ascii());
			}
			while ( len > 0 && !line.contains("image") );
			dat.close();
		}
		else {
			qDebug("-- empty --");
		}
	}
	QFile::remove(rawfile);
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, 0, 0, w, h, LineW, CommonStrings::None, CurrColor, true);
	PageItem * ite = m_Doc->Items->at(z);
	ite->setXYPos(m_Doc->currentPage()->xOffset() + x, m_Doc->currentPage()->yOffset() + y);
	ite->setWidthHeight(w, h);
	ite->clearContents();
/*	FPoint a(x, y);
	FPoint b(x+w, y);
	FPoint c(x+w, y-h);
	FPoint d(x, y-h);
	ite->PoLine.resize(0);
	ite->PoLine.addQuadPoint(a, a, b, b);
	ite->PoLine.addQuadPoint(b, b, c, c);
	ite->PoLine.addQuadPoint(c, c, d, d);
	ite->PoLine.addQuadPoint(d, d, a, a);
	ite->PoLine.translate(m_Doc->currentPage->xOffset() - x, m_Doc->currentPage->yOffset() - y);
	ite->ClipEdited = true;
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
*/
	m_Doc->loadPict(filename, ite, -1);
	ite->setRotation(angle);
	ite->setImageScalingMode(false, true); // fit to frame, keep ratio
//	m_Doc->view()->AdjustItemSize(ite);
	Elements.append(ite);
	return ret == 0;
}


void EPSPlug::LineTo(FPointArray *i, QString vals)
{
	if (vals.isEmpty())
		return;
	double x1, x2, y1, y2;
	x1 = vals.section(' ', 0, 0, QString::SectionSkipEmpty).toDouble();
	y1 = vals.section(' ', 1, 1, QString::SectionSkipEmpty).toDouble();
	x2 = vals.section(' ', 2, 2, QString::SectionSkipEmpty).toDouble();
	y2 = vals.section(' ', 3, 3, QString::SectionSkipEmpty).toDouble();
	if ((!FirstM) && (WasM))
		i->setMarker();
	FirstM = false;
	WasM = false;
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x2, y2));
	i->addPoint(FPoint(x2, y2));
}

void EPSPlug::Curve(FPointArray *i, QString vals)
{
	if (vals.isEmpty())
		return;
	double x1, x2, y1, y2, x3, y3, x4, y4;
	x1 = vals.section(' ', 0, 0, QString::SectionSkipEmpty).toDouble();
	y1 = vals.section(' ', 1, 1, QString::SectionSkipEmpty).toDouble();
	x2 = vals.section(' ', 2, 2, QString::SectionSkipEmpty).toDouble();
	y2 = vals.section(' ', 3, 3, QString::SectionSkipEmpty).toDouble();
	x3 = vals.section(' ', 4, 4, QString::SectionSkipEmpty).toDouble();
	y3 = vals.section(' ', 5, 5, QString::SectionSkipEmpty).toDouble();
	x4 = vals.section(' ', 6, 6, QString::SectionSkipEmpty).toDouble();
	y4 = vals.section(' ', 7, 7, QString::SectionSkipEmpty).toDouble();
	if ((!FirstM) && (WasM))
		i->setMarker();
	FirstM = false;
	WasM = false;
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x2, y2));
	i->addPoint(FPoint(x4, y4));
	i->addPoint(FPoint(x3, y3));
}

QString EPSPlug::parseColor(QString vals, colorModel model)
{
	QString ret = CommonStrings::None;
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
		for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
		{
			m_Doc->PageColors[it.key()].getRGB(&hR, &hG, &hB);
			if ((Rc == hR) && (Gc == hG) && (Bc == hB))
			{
				if (m_Doc->PageColors[it.key()].getColorModel() == colorModelRGB)
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
		for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
		{
			m_Doc->PageColors[it.key()].getCMYK(&hC, &hM, &hY, &hK);
			if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
			{
				if (m_Doc->PageColors[it.key()].getColorModel() == colorModelCMYK)
				{
					ret = it.key();
					found = true;
				}
			}
		}
	}
	if (!found)
	{
		m_Doc->PageColors.insert("FromEPS"+tmp.name(), tmp);
		m_Doc->scMW()->propertiesPalette->updateColorList();
		ret = "FromEPS"+tmp.name();
	}
	return ret;
}
