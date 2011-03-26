/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QByteArray>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QStack>

#include <cstdlib>

#include "commonstrings.h"
#include "importps.h"
#include "loadsaveplugin.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "scconfig.h"
#include "scclocale.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "ui/customfdialog.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"
#ifdef HAVE_PODOFO
	#include <podofo/podofo.h>
#endif


extern SCRIBUS_API ScribusQApp * ScQApp;

EPSPlug::EPSPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc  = doc;
	progressDialog = NULL;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

bool EPSPlug::import(QString fName, const TransactionSettings &trSettings, int flags, bool showProgress)
{
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	bool found = false;
	CustColors.clear();
	QFileInfo fi = QFileInfo(fName);
	QString ext = fi.suffix().toLower();
	if ( !ScCore->usingGUI() ) {
		interactive = false;
		showProgress = false;
	}
	if ( showProgress ) 
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw);
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing PostScript:");
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
	else {
		progressDialog = NULL;
	}
	
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	if (extensionIndicatesEPSorPS(ext))
	{
		QString tmp, BBox, tmp2, FarNam;
		ScColor cc;
		QFile f(fName);
		if (f.open(QIODevice::ReadOnly))
		{
/* Try to find Bounding Box */
			QDataStream ts(&f);
			while (!ts.atEnd())
			{
				tmp = readLinefromDataStream(ts);
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
				if (tmp.startsWith("%%EndComments"))
					break; 
			}
			f.close();
			if (found)
			{
				QStringList bb = BBox.split(" ", QString::SkipEmptyParts);
				if (bb.count() == 4)
				{
					x = ScCLocale::toDoubleC(bb[0]);
					y = ScCLocale::toDoubleC(bb[1]);
					b = ScCLocale::toDoubleC(bb[2]);
					h = ScCLocale::toDoubleC(bb[3]);
				}
			}
		}
		importColorsFromFile(fName, CustColors);
	}
#ifdef HAVE_PODOFO
	else if (extensionIndicatesPDF(ext))
	{
		try
		{
			PoDoFo::PdfError::EnableDebug( false );
#if (PODOFO_VERSION == 0 && PODOFO_MINOR > 6)
		PoDoFo::PdfError::EnableLogging( false );
#endif
#if (PODOFO_VERSION == 0 && PODOFO_MINOR == 5 && PODOFO_REVISION == 99) || PODOFO_MINOR > 5
			PoDoFo::PdfMemDocument doc( fName.toLocal8Bit().data() );
#else
			PoDoFo::PdfDocument doc( fName.toLocal8Bit().data() );
#endif
			PoDoFo::PdfPage *curPage = doc.GetPage(0);
			if (curPage != NULL)
			{
				PoDoFo::PdfRect rect = curPage->GetMediaBox();
				b = rect.GetWidth() - rect.GetLeft();
				h = rect.GetHeight() - rect.GetBottom();
			}
		}
		catch(PoDoFo::PdfError& e)
		{
			qDebug("%s", "PoDoFo error while reading page size!");
			e.PrintErrorMsg();
		}
	}
#endif
	baseX = 0;
	baseY = 0;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		baseX = 0;
		baseY = 0;
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			baseX = 0;
			baseY = 0;
		}
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!interactive))
	{
		if (b-x > h-y)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	ColorList::Iterator it;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
			m_Doc->PageColors.insert(it.key(), it.value());
	}
	boundingBoxRect.addRect(0, 0, b-x, h-y);
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName, x, y, b, h))
	{
// 		m_Doc->m_Selection->clear();
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
//		if ((Elements.count() > 1) && (interactive))
		if (Elements.count() > 1)
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
/*#ifndef Q_WS_MAC*/
// see #2196
				m_Doc->itemSelection_DeleteItem(tmpSel);
/*#else
				qDebug() << "psimport: leaving items on page";
#endif*/
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

EPSPlug::~EPSPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}
	

bool EPSPlug::convert(QString fn, double x, double y, double b, double h)
{
	QStringList args;
	QString cmd, cmd1, cmd2, cmd3, tmp, tmp2, tmp3, tmp4;
	// import.prolog do not cope with filenames containing blank spaces
	// so take care that output filename does not (win32 compatibility)
	QString tmpFile = getShortPathName(ScPaths::getTempFileDir())+ "/ps.out";
	QString errFile = getShortPathName(ScPaths::getTempFileDir())+ "/ps.err";
	QString pfad = ScPaths::instance().libDir();
	QString pfad2 = QDir::convertSeparators(pfad + "import.prolog");
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	
	if (progressDialog) {
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
/*
// Destill the eps with ghostscript to get a clean eps file
	QString cleanFile = getShortPathName(ScPaths::getTempFileDir())+ "/clean.eps";
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-sDEVICE=epswrite" );
	args.append( "-dBATCH" );
	args.append( "-dSAFER" );
	args.append( "-dDEVICEWIDTH=250000" );
	args.append( "-dDEVICEHEIGHT=250000" );
	args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(cleanFile)) );
	args.append( QDir::convertSeparators(fn) );
	System(getShortPathName(PrefsManager::instance()->ghostscriptExecutable()), args, errFile, errFile, &cancel);
	args.clear();
*/
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dNODISPLAY" );
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
	args.append( QString("-g%1x%2").arg(tmp2.setNum(qRound((b-x)*4))).arg(tmp3.setNum(qRound((h-y)*4))) );
	args.append( "-r288");
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
	//qDebug() << QString("using export path %1").arg(exportFi.absFilePath());
	args.append( QString("-sExportFiles=%1").arg(QDir::convertSeparators(exportFi.absoluteFilePath())) );
	args.append( pfad2 );
	args.append( QDir::convertSeparators(fn) );
	args.append( "-c" );
	args.append( "flush" );
	args.append( "cfile" );
	args.append( "closefile" );
	args.append( "quit" );
	QByteArray finalCmd = args.join(" ").toLocal8Bit();
	int ret = System(getShortPathName(PrefsManager::instance()->ghostscriptExecutable()), args, errFile, errFile, &cancel);
	if (ret != 0 && !cancel)
	{
		qDebug("PostScript import failed when calling gs as: \n%s\n", finalCmd.data());
		qDebug("%s", "Ghostscript diagnostics:\n");
		QFile diag(errFile);
		if (diag.open(QIODevice::ReadOnly) && !diag.atEnd() ) {
			char buf[121];
			while (diag.readLine(buf, 120) > 0) {
				qDebug("\t%s", buf);
			}
			diag.close();
		}
		else {
			qDebug("%s", "-- no output --");
		}
		if (progressDialog)
			progressDialog->close();
		QString mess = tr("Importing File:\n%1\nfailed!").arg(fn);
		QMessageBox::critical(0, tr("Fatal Error"), mess, 1, 0, 0);
		return false;
	}
	if(progressDialog && !cancel) {
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	if (!cancel) {
		parseOutput(tmpFile, extensionIndicatesEPSorPS(ext));
	}
	QFile::remove(tmpFile);
//	QFile::remove(cleanFile);
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
	QStack<PageItem*> groupStack;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int>  gsStack;
	QStack<uint> gsStackMarks;
	QFile f(fn);
	lasttoken = "";
	pagecount = 1;
	if (f.open(QIODevice::ReadOnly))
	{
		int fProgress = 0;
		int fSize = (int) f.size();
		if (progressDialog) {
			progressDialog->setTotalSteps("GI", fSize);
			qApp->processEvents();
		}
		lastPath = "";
		currPath = "";
		LineW = 0;
		Opacity = 1;
		CurrColor = CommonStrings::None;
		JoinStyle = Qt::MiterJoin;
		CapStyle = Qt::FlatCap;
		DashPattern.clear();
		ScTextStream ts(&f);
		int line_cnt = 0;
		while (!ts.atEnd() && !cancel)
		{
			tmp = "";
			tmp = ts.readLine();
			if (progressDialog && (++line_cnt % 100 == 0)) {
				int fPos = f.pos();
				int progress = static_cast<int>(ceil(fPos / (double) fSize * 100));
				if (progress > fProgress)
				{
					progressDialog->setProgress("GI", fPos);
					qApp->processEvents();
					fProgress = progress;
				}
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
						lastPath = "";
					}
					else
					{
						if (ClosedPath)
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColor, CommonStrings::None, true);
						else
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColor, CommonStrings::None, true);
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
						ite->setTextFlowMode(PageItem::TextFlowDisabled);
						m_Doc->AdjustItemSize(ite);
						if (ite->itemType() == PageItem::Polygon)
							ite->ContourLine = ite->PoLine.copy();
						if ((groupStack.count() != 0) && (groupStackP.count() != 0))
							groupStackP.top().append(ite);
						Elements.append(ite);
						lastPath = currPath;
					}
					currPath = "";
				}
			}
			else if (token == "s")
			{
				if (Coords.size() != 0)
				{
				//	LineW = qMax(LineW, 0.01); // Set Linewidth to be a least 0.01 pts, a Stroke without a Linewidth makes no sense
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
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColor, true);
						else
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColor, true);
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
						m_Doc->AdjustItemSize(ite);
						if (ite->itemType() == PageItem::Polygon)
							ite->ContourLine = ite->PoLine.copy();
						ite->setLineWidth(LineW);
						ite->setTextFlowMode(PageItem::TextFlowDisabled);
						if ((groupStack.count() != 0) && (groupStackP.count() != 0))
							groupStackP.top().append(ite);
						Elements.append(ite);
					}
					lastPath = "";
					currPath = "";
				}
			}
			else if (token == "co")
				CurrColor = parseColor(params, eps);
			else if (token == "corgb")
				CurrColor = parseColor(params, eps, colorModelRGB);
			else if (token == "ci")
			{
				if (Coords.size() != 0)
				{
					QString vers = QString(qVersion()).left(5);
					if (vers < "4.3.3")
					{
						clipCoords = Coords;
						z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, baseX, baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
						ite = m_Doc->Items->at(z);
						ite->PoLine = Coords.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
						ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
						ite->ClipEdited = true;
						ite->FrameType = 3;
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(),wh.y());
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						m_Doc->AdjustItemSize(ite, true);
						ite->ContourLine = ite->PoLine.copy();
						ite->setItemName( tr("Group%1").arg(m_Doc->GroupCounter));
						ite->setTextFlowMode(PageItem::TextFlowDisabled);
						Elements.append(ite);
						if ((groupStack.count() != 0) && (groupStackP.count() != 0))
							groupStackP.top().append(ite);
						groupStack.push(ite);
						QList<PageItem*> gElements;
						groupStackP.push(gElements);
						gsStackMarks.push(gsStack.count());
						m_Doc->GroupCounter++;
					}
					else
					{
						QPainterPath tmpPath = Coords.toQPainterPath(true);
						tmpPath = boundingBoxRect.intersected(tmpPath);
						if ((tmpPath.boundingRect().width() != 0) && (tmpPath.boundingRect().height() != 0))
						{
							clipCoords.fromQPainterPath(tmpPath);
							z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, baseX, baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
							ite = m_Doc->Items->at(z);
							ite->PoLine = clipCoords.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
							ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
							ite->ClipEdited = true;
							ite->FrameType = 3;
							FPoint wh = getMaxClipF(&ite->PoLine);
							ite->setWidthHeight(wh.x(),wh.y());
							ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
							m_Doc->AdjustItemSize(ite, true);
							ite->ContourLine = ite->PoLine.copy();
							ite->setItemName( tr("Group%1").arg(m_Doc->GroupCounter));
							ite->setTextFlowMode(PageItem::TextFlowDisabled);
							Elements.append(ite);
							if ((groupStack.count() != 0) && (groupStackP.count() != 0))
								groupStackP.top().append(ite);
							groupStack.push(ite);
							QList<PageItem*> gElements;
							groupStackP.push(gElements);
							gsStackMarks.push(gsStack.count());
							m_Doc->GroupCounter++;
						}
					}
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
				// #6834 : self defense against incorrectly balanced save/restore
				if (gsStack.count() > 0)
					gsStack.pop();
				if ((groupStack.count() != 0) && (groupStackP.count() != 0))
				{
					if (gsStack.count() < static_cast<int>(gsStackMarks.top()))
					{
						PageItem *ite = groupStack.pop();
						QList<PageItem*> gList = groupStackP.pop();
						for (int d = 0; d < gList.count(); d++)
						{
							Elements.removeAll(gList.at(d));
						}
						m_Doc->groupObjectsToItem(ite, gList);
						gsStackMarks.pop();
					}
				}
			}
			else if (token == "w")
			{
				ScTextStream Lw(&params, QIODevice::ReadOnly);
				Lw >> LineW;
			}
			else if (token == "ld")
			{
				ScTextStream Lw(&params, QIODevice::ReadOnly);
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
			}
			else if (token == "lc")
			{
				ScTextStream Lw(&params, QIODevice::ReadOnly);
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
			}
			else if (token == "lj")
			{
				ScTextStream Lw(&params, QIODevice::ReadOnly);
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
				QList<PageItem*> gList = groupStackP.pop();
				for (int d = 0; d < gList.count(); d++)
				{
					Elements.removeAll(gList.at(d));
				}
				m_Doc->groupObjectsToItem(ite, gList);
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
	ScTextStream Code(&vals, QIODevice::ReadOnly);
	Code >> x;
	Code >> y;
	Code >> w;
	Code >> h;
	Code >> angle;
	Code >> horpix;
	Code >> verpix;
	Code >> device;
	filename = Code.readAll().trimmed();
	if (device.startsWith("psd")) {
		filename = filename.mid(0, filename.length()-3) + "psd";
	}
		
	qDebug("%s", QString("import %7 image %1: %2x%3 @ (%4,%5) °%6").arg(filename).arg(w).arg(h).arg(x).arg(y).arg(angle).arg(device).toLocal8Bit().data());
	QString rawfile = filename.mid(0, filename.length()-3) + "dat";
	QStringList args;
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
	QByteArray finalCmd = args.join(" ").toLocal8Bit();
	int ret = System(getShortPathName(PrefsManager::instance()->ghostscriptExecutable()), args);
	if (ret != 0)
	{
		qDebug("PostScript image conversion failed when calling gs as: \n%s\n", finalCmd.data());
		qDebug("Ghostscript diagnostics: %d\n", ret);
		QFile diag(filename);
		if (diag.open(QIODevice::ReadOnly)) {
			char buf[121];
			long int len;
			bool gs_error = false;
			do {
				len = diag.readLine(buf, 120);
				gs_error |= (strstr(buf,"Error")==NULL);
				if (gs_error)
					qDebug("\t%s", buf);
			}
			while (len > 0);
			diag.close();
			}
		else {
			qDebug("%s", "-- no output --");
		}
		qDebug("%s", "Failed file was:\n");
		QFile dat(rawfile);
		if (dat.open(QIODevice::ReadOnly)) {
			char buf[121];
			long int len;
			do {
				len = dat.readLine(buf, 120);
				qDebug("\t%s", buf);
			}
			while ( len > 0 && !(strstr(buf, "image")==NULL) );
			dat.close();
		}
		else {
			qDebug("%s", "-- empty --");
		}
	}
	QFile::remove(rawfile);
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset(), w, h, LineW, CommonStrings::None, CurrColor, true);
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
	x1 = ScCLocale::toDoubleC(vals.section(' ', 0, 0, QString::SectionSkipEmpty));
	y1 = ScCLocale::toDoubleC(vals.section(' ', 1, 1, QString::SectionSkipEmpty));
	x2 = ScCLocale::toDoubleC(vals.section(' ', 2, 2, QString::SectionSkipEmpty));
	y2 = ScCLocale::toDoubleC(vals.section(' ', 3, 3, QString::SectionSkipEmpty));
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
	x1 = ScCLocale::toDoubleC(vals.section(' ', 0, 0, QString::SectionSkipEmpty));
	y1 = ScCLocale::toDoubleC(vals.section(' ', 1, 1, QString::SectionSkipEmpty));
	x2 = ScCLocale::toDoubleC(vals.section(' ', 2, 2, QString::SectionSkipEmpty));
	y2 = ScCLocale::toDoubleC(vals.section(' ', 3, 3, QString::SectionSkipEmpty));
	x3 = ScCLocale::toDoubleC(vals.section(' ', 4, 4, QString::SectionSkipEmpty));
	y3 = ScCLocale::toDoubleC(vals.section(' ', 5, 5, QString::SectionSkipEmpty));
	x4 = ScCLocale::toDoubleC(vals.section(' ', 6, 6, QString::SectionSkipEmpty));
	y4 = ScCLocale::toDoubleC(vals.section(' ', 7, 7, QString::SectionSkipEmpty));
	if ((!FirstM) && (WasM))
		i->setMarker();
	FirstM = false;
	WasM = false;
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x2, y2));
	i->addPoint(FPoint(x4, y4));
	i->addPoint(FPoint(x3, y3));
}

QString EPSPlug::parseColor(QString vals, bool eps, colorModel model)
{
	QString ret = CommonStrings::None;
	if (vals.isEmpty())
		return ret;
	double c, m, y, k, r, g, b;
	ScColor tmp;
	ScTextStream Code(&vals, QIODevice::ReadOnly);
	if (model == colorModelRGB)
	{
		Code >> r;
		Code >> g;
		Code >> b;
		Code >> Opacity;
// Why adding 0.5 here color values range from 0 to 255 not 1 to 256 ??
/*		int Rc = static_cast<int>(r * 255 + 0.5);
		int Gc = static_cast<int>(g * 255 + 0.5);
		int Bc = static_cast<int>(b * 255 + 0.5); */
		int Rc = qRound(r * 255);
		int Gc = qRound(g * 255);
		int Bc = qRound(b * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
	}
	else
	{
		Code >> c;
		Code >> m;
		Code >> y;
		Code >> k;
		Code >> Opacity;
		int Cc = qRound(c * 255);
		int Mc = qRound(m * 255);
		int Yc = qRound(y * 255);
		int Kc = qRound(k * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
	}
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString namPrefix = "FromEPS";
	if (!eps)
		namPrefix = "FromPS";
	QString fNam = m_Doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	ret = fNam;
	return ret;
}
