/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QByteArray>
#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QStack>
#include <QDebug>

#include <cstdlib>

#include "importcvg.h"

#include "commonstrings.h"
#include "loadsaveplugin.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/multiprogressdialog.h"
#include "undomanager.h"
#include "util_math.h"

CvgPlug::CvgPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = nullptr;
}

QImage CvgPlug::readThumbnail(const QString& fName)
{
	QFileInfo fi(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath() + "/"));
	double b, h;
	parseHeader(fName, b, h);
	if (b == 0.0)
		b = PrefsManager::instance().appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance().appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	progressDialog = nullptr;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), nullptr);
	baseX = m_Doc->currentPage()->xOffset();
	baseY = m_Doc->currentPage()->yOffset();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (Elements.count() > 1)
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
		if (Elements.count() > 0)
		{
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
		}
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		m_Doc->m_Selection->delaySignalsOff();
		delete m_Doc;
		return tmpImage;
	}
	QDir::setCurrent(CurDirP);
	m_Doc->DoDrawing = true;
	m_Doc->scMW()->setScriptRunning(false);
	delete m_Doc;
	return QImage();
}

bool CvgPlug::import(const QString& fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	bool ret = false;
	CustColors.clear();
	QFileInfo fi(fNameIn);
	if (!ScCore->usingGUI())
	{
		interactive = false;
		showProgress = false;
	}
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath() + "/"));
	if (showProgress)
	{
		ScribusMainWindow* mw = (m_Doc == nullptr) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw );
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing File:");
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
	else
		progressDialog = nullptr;
/* Set default Page to size defined in Preferences */
	double b = 0.0;
	double h = 0.0;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	parseHeader(fNameIn, b, h);
	if (b == 0.0)
		b = PrefsManager::instance().appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance().appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	baseX = 0;
	baseY = 0;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		baseX = 0;
		baseY = 0;
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc = ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			baseX = 0;
			baseY = 0;
			baseX = m_Doc->currentPage()->xOffset();
			baseY = m_Doc->currentPage()->yOffset();
		}
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!interactive))
	{
		if (docWidth > docHeight)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != nullptr))
		m_Doc->view()->deselectItems();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != nullptr))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fNameIn))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
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
					if (m_Doc->view() != nullptr)
						m_Doc->view()->updatesOn(true);
				}
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
		if (!(flags & LoadSavePlugin::lfLoadAsPattern))
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
	qApp->restoreOverrideCursor();
	return success;
}

CvgPlug::~CvgPlug()
{
	delete progressDialog;
	delete tmpSel;
}

void CvgPlug::parseHeader(const QString& fName, double &b, double &h)
{
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ts(&f);
		ts.device()->seek(16);
		quint16 pgY, pgW, pgH;
		ts >> pgY >> pgW >> pgH;
		b = pgW / 72.0;
		h = pgH / 72.0;
		scPg = h / b;
		b = pgY / 72.0;
		h = pgY / 72.0 * scPg;
		f.close();
	}
}

bool CvgPlug::convert(const QString& fn)
{
	CurrColorFill = "Black";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	Coords.resize(0);
	Coords.svgInit();
	importedColors.clear();
	QList<PageItem*> gElements;
	groupStack.push(gElements);
	currentItemNr = 0;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		int fSize = (int) f.size();
		if (progressDialog)
		{
			progressDialog->setTotalSteps("GI", fSize);
			qApp->processEvents();
		}
		QDataStream ts(&f);
		ts.device()->seek(10);
		quint16 pgX, pgY, pgW, pgH, colorF, dummy;
		bool colorFlag = false;
		ts >> colorF >> dummy;
		if (colorF == 0x044c)
			colorFlag = true;
		ts >> pgX >> pgY >> pgW >> pgH;
		quint32 currentFilePos = 14;
		while (!ts.atEnd())
		{
			quint16 data;
			quint32 lenData;
			ts >> data;
			ts >> lenData;
			currentFilePos = ts.device()->pos();
			if (data == 0x0020)
				qDebug() << "Group" << "Len" << lenData;
			if (data == 0x0004)
			{
				getObjects(ts, colorFlag, lenData - 26);
			}
			ts.device()->seek(currentFilePos + lenData - 6);
			if (progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
		if (Elements.count() == 0)
		{
			if (importedColors.count() != 0)
			{
				for (int cd = 0; cd < importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(importedColors[cd]);
				}
			}
		}
		f.close();
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}

void CvgPlug::getObjects(QDataStream &ts, bool color, quint32 lenData)
{
	quint16 obX, obY, obW, obH, fillFlag, lineWidth;
	quint32 counter = 0;
	quint32 colorFill, colorLine;
	int z;
	PageItem *ite;
	Coords.resize(0);
	Coords.svgInit();
	ts >> obX >> obY >> obW >> obH;
	ts >> colorFill >> colorLine;
	ts >> fillFlag >> lineWidth;
	fillFlag = fillFlag & 0x0FFF;
	qreal scX = obW / 16384.0;
	qreal scY = obH / 16384.0;
	parseColor(colorFill, colorLine, color, fillFlag);
	while (counter < lenData)
	{
		quint16 opCode, x1, y1, cx1, cy1, cx2, cy2;
		ts >> opCode;
		counter += 2;
		if (opCode == 0x0000)		// moveTo
		{
			ts >> x1 >> y1;
			counter += 4;
			Coords.svgMoveTo(x1 / 72.0 * scX, y1 / 72.0 * scY * scPg);
		}
		else if (opCode == 0x0001)	// lineTo
		{
			ts >> x1 >> y1;
			counter += 4;
			Coords.svgLineTo(x1 / 72.0 * scX, y1 / 72.0 * scY * scPg);
		}
		else if (opCode == 0x0002)	// curveTo
		{
			ts >> cx1 >> cy1 >> cx2 >> cy2 >> x1 >> y1;
			counter += 12;
			Coords.svgCurveToCubic(cx1 / 72.0 * scX, cy1 / 72.0 * scY * scPg, cx2 / 72.0 * scX, cy2 / 72.0 * scY * scPg, x1 / 72.0 * scX, y1 / 72.0 * scY * scPg);
		}
		else if (opCode == 0x000f)
			break;
	}
	if (Coords.empty())
		return;
	Coords.svgClosePath();
	z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX + obX / 72.0, baseY + obY / 72.0 * scPg, 10, 10, lineWidth / 72.0, CurrColorFill, CurrColorStroke);
	ite = m_Doc->Items->at(z);
	ite->PoLine = Coords.copy();
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(CurrStrokeShade);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_Doc->adjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	Elements.append(ite);
	if (groupStack.count() != 0)
		groupStack.top().append(ite);
}

void CvgPlug::parseColor(quint32 dataF, quint32 dataS, bool color, quint16 flag)
{
	ScColor tmp;
	CurrColorFill = CommonStrings::None;
	CurrFillShade = 100.0;
	CurrColorStroke = CommonStrings::None;
	CurrStrokeShade = 100.0;
	QColor c;
	if (color)
	{
		if ((flag == 0x0080) || (flag == 0x0200) || (flag == 0x0400))
		{
			c.setRgb(dataF);
			tmp.fromQColor(c);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
			QString newColorName = "FromCVG"+c.name();
			QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
			if (fNam == newColorName)
				importedColors.append(fNam);
			CurrColorFill = fNam;
		}
		if ((flag == 0x0080) || (flag == 0x0100) || (flag == 0x0400))
		{
			c.setRgb(dataS);
			tmp.fromQColor(c);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
			QString newColorName = "FromCVG"+c.name();
			QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
			if (fNam == newColorName)
				importedColors.append(fNam);
			CurrColorStroke = fNam;
		}
	}
	else
	{
		if (flag == 0x0080)
		{
			CurrColorFill = "Black";
			CurrFillShade = dataF & 0x000000FF;
			CurrColorStroke = "Black";
			CurrStrokeShade = dataS & 0x000000FF;
		}
		else if (flag == 0x0100)
		{
			CurrColorFill = CommonStrings::None;
			CurrFillShade = dataF & 0x000000FF;
			CurrColorStroke = "Black";
			CurrStrokeShade = dataS & 0x000000FF;
		}
		else
		{
			CurrColorFill = "Black";
			CurrFillShade = dataF & 0x000000FF;
			CurrColorStroke = CommonStrings::None;
			CurrStrokeShade = dataS & 0x000000FF;
		}
	}
}
