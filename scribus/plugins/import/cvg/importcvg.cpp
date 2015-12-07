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
#include <QRegExp>
#include <QStack>
#include <QDebug>

#include <cstdlib>

#include "importcvg.h"

#include "commonstrings.h"
#include "loadsaveplugin.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "selection.h"
#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"


extern SCRIBUS_API ScribusQApp * ScQApp;

CvgPlug::CvgPlug(ScribusDoc* doc, int flags)
{
	m_tmpSel=new Selection(this, false);
	m_Doc=doc;
	m_importerFlags = flags;
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_progressDialog = NULL;
}

QImage CvgPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	m_baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b, h;
	parseHeader(fName, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	m_docWidth = b;
	m_docHeight = h;
	m_progressDialog = NULL;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	m_baseX = m_Doc->currentPage()->xOffset();
	m_baseY = m_Doc->currentPage()->yOffset();
	m_Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		m_tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (m_Elements.count() > 1)
			m_Doc->groupObjectsList(m_Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
		if (m_Elements.count() > 0)
		{
			for (int dre=0; dre<m_Elements.count(); ++dre)
			{
				m_tmpSel->addItem(m_Elements.at(dre), true);
			}
			m_tmpSel->setGroupRect();
			double xs = m_tmpSel->width();
			double ys = m_tmpSel->height();
			tmpImage = m_Elements.at(0)->DrawObj_toImage(500);
			tmpImage.setText("XSize", QString("%1").arg(xs));
			tmpImage.setText("YSize", QString("%1").arg(ys));
		}
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		m_Doc->m_Selection->delaySignalsOff();
		delete m_Doc;
		return tmpImage;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		delete m_Doc;
	}
	return QImage();
}

bool CvgPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_importerFlags = flags;
	m_cancel = false;
	double b, h;
	bool ret = false;
	m_CustColors.clear();
	QFileInfo fi = QFileInfo(fName);
	if ( !ScCore->usingGUI() )
	{
		m_interactive = false;
		showProgress = false;
	}
	m_baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if ( showProgress )
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		m_progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw );
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing File:");
		QList<bool> barsNumeric;
		barsNumeric << false;
		m_progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		m_progressDialog->setOverallTotalSteps(3);
		m_progressDialog->setOverallProgress(0);
		m_progressDialog->setProgress("GI", 0);
		m_progressDialog->show();
		connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
		qApp->processEvents();
	}
	else
		m_progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	b = 0.0;
	h = 0.0;
	if (m_progressDialog)
	{
		m_progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	parseHeader(fName, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	m_docWidth = b;
	m_docHeight = h;
	m_baseX = 0;
	m_baseY = 0;
	if (!m_interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		m_baseX = 0;
		m_baseY = 0;
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			m_baseX = 0;
			m_baseY = 0;
			m_baseX = m_Doc->currentPage()->xOffset();
			m_baseY = m_Doc->currentPage()->yOffset();
		}
	}
	if ((!ret) && (m_interactive))
	{
		m_baseX = m_Doc->currentPage()->xOffset();
		m_baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!m_interactive))
	{
		if (m_docWidth > m_docHeight)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != NULL))
		m_Doc->view()->Deselect();
	m_Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != NULL))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		m_tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if ((m_Elements.count() > 1) && (!(m_importerFlags & LoadSavePlugin::lfCreateDoc)))
			m_Doc->groupObjectsList(m_Elements);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if ((m_Elements.count() > 0) && (!ret) && (m_interactive))
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
					for (int dre=0; dre<m_Elements.count(); ++dre)
					{
						m_Doc->m_Selection->addItem(m_Elements.at(dre), true);
					}
					m_Doc->m_Selection->delaySignalsOff();
					m_Doc->m_Selection->setGroupRect();
					if (m_Doc->view() != NULL)
						m_Doc->view()->updatesOn(true);
				}
			}
			else
			{
				m_Doc->DragP = true;
				m_Doc->DraggedElem = 0;
				m_Doc->DragElements.clear();
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<m_Elements.count(); ++dre)
				{
					m_tmpSel->addItem(m_Elements.at(dre), true);
				}
				m_tmpSel->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_Doc, m_tmpSel);
				m_Doc->itemSelection_DeleteItem(m_tmpSel);
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
		if (!(flags & LoadSavePlugin::lfLoadAsPattern))
			m_Doc->view()->updatesOn(true);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	if (m_interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
	{
		if ((showProgress) && (!m_interactive))
			m_Doc->view()->DrawNew();
	}
	qApp->restoreOverrideCursor();
	return success;
}

CvgPlug::~CvgPlug()
{
	if (m_progressDialog)
		delete m_progressDialog;
	delete m_tmpSel;
}

void CvgPlug::parseHeader(QString fName, double &b, double &h)
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
		m_scPg = h / b;
		b = pgY / 72.0;
		h = pgY / 72.0 * m_scPg;
		f.close();
	}
}

bool CvgPlug::convert(QString fn)
{
	QString tmp;
	m_CurrColorFill = "Black";
	m_CurrFillShade = 100.0;
	m_CurrColorStroke = "Black";
	m_CurrStrokeShade = 100.0;
	m_Coords.resize(0);
	m_Coords.svgInit();
	m_importedColors.clear();
	QList<PageItem*> gElements;
	m_groupStack.push(gElements);
	m_currentItemNr = 0;
	if(m_progressDialog)
	{
		m_progressDialog->setOverallProgress(2);
		m_progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		m_oldDocItemCount = m_Doc->Items->count();
		int fSize = (int) f.size();
		if (m_progressDialog)
		{
			m_progressDialog->setTotalSteps("GI", fSize);
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
			if (m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
		if (m_Elements.count() == 0)
		{
			if (m_importedColors.count() != 0)
			{
				for (int cd = 0; cd < m_importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(m_importedColors[cd]);
				}
			}
		}
		f.close();
	}
	if (m_progressDialog)
		m_progressDialog->close();
	return true;
}

void CvgPlug::getObjects(QDataStream &ts, bool color, quint32 lenData)
{
	quint16 obX, obY, obW, obH, fillFlag, lineWidth;
	quint32 counter = 0;
	quint32 colorFill, colorLine;
	int z;
	PageItem *ite;
	m_Coords.resize(0);
	m_Coords.svgInit();
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
			m_Coords.svgMoveTo(x1 / 72.0 * scX, y1 / 72.0 * scY * m_scPg);
		}
		else if (opCode == 0x0001)	// lineTo
		{
			ts >> x1 >> y1;
			counter += 4;
			m_Coords.svgLineTo(x1 / 72.0 * scX, y1 / 72.0 * scY * m_scPg);
		}
		else if (opCode == 0x0002)	// curveTo
		{
			ts >> cx1 >> cy1 >> cx2 >> cy2 >> x1 >> y1;
			counter += 12;
			m_Coords.svgCurveToCubic(cx1 / 72.0 * scX, cy1 / 72.0 * scY * m_scPg, cx2 / 72.0 * scX, cy2 / 72.0 * scY * m_scPg, x1 / 72.0 * scX, y1 / 72.0 * scY * m_scPg);
		}
		else if (opCode == 0x000f)
			break;
	}
	if (m_Coords.size() > 0)
	{
		m_Coords.svgClosePath();
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX + obX / 72.0, m_baseY + obY / 72.0 * m_scPg, 10, 10, lineWidth / 72.0, m_CurrColorFill, m_CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = m_Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(m_CurrFillShade);
		ite->setLineShade(m_CurrStrokeShade);
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->AdjustItemSize(ite);
		ite->OldB2 = ite->width();
		ite->OldH2 = ite->height();
		ite->updateClip();
		m_Elements.append(ite);
		if (m_groupStack.count() != 0)
			m_groupStack.top().append(ite);
	}
}

void CvgPlug::parseColor(quint32 dataF, quint32 dataS, bool color, quint16 flag)
{
	ScColor tmp;
	m_CurrColorFill = CommonStrings::None;
	m_CurrFillShade = 100.0;
	m_CurrColorStroke = CommonStrings::None;
	m_CurrStrokeShade = 100.0;
	QColor c;
	if (color)
	{
		if ((flag == 0x0080) || (flag == 0x0200))
		{
			c.setRgb(dataF);
			tmp.fromQColor(c);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
			QString newColorName = "FromCVG"+c.name();
			QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
			if (fNam == newColorName)
				m_importedColors.append(fNam);
			m_CurrColorFill = fNam;
		}
		if ((flag == 0x0080) || (flag == 0x0100))
		{
			c.setRgb(dataS);
			tmp.fromQColor(c);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
			QString newColorName = "FromCVG"+c.name();
			QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
			if (fNam == newColorName)
				m_importedColors.append(fNam);
			m_CurrColorStroke = fNam;
		}
	}
	else
	{
		if (flag == 0x0080)
		{
			m_CurrColorFill = "Black";
			m_CurrFillShade = dataF & 0x000000FF;
			m_CurrColorStroke = "Black";
			m_CurrStrokeShade = dataS & 0x000000FF;
		}
		else if (flag == 0x0100)
		{
			m_CurrColorFill = CommonStrings::None;
			m_CurrFillShade = dataF & 0x000000FF;
			m_CurrColorStroke = "Black";
			m_CurrStrokeShade = dataS & 0x000000FF;
		}
		else
		{
			m_CurrColorFill = "Black";
			m_CurrFillShade = dataF & 0x000000FF;
			m_CurrColorStroke = CommonStrings::None;
			m_CurrStrokeShade = dataS & 0x000000FF;
		}
	}
}
