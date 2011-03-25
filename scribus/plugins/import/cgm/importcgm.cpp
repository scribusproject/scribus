/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          importcgm.cpp  -  description
                             -------------------
    begin                : Wed Dez 23 2009
    copyright            : (C) 2009 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

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

#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "importcgm.h"
#include "loadsaveplugin.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "ui/propertiespalette.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

CgmPlug::CgmPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

QImage CgmPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b, h;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	progressDialog = NULL;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
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
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		delete m_Doc;
	}
	return QImage();
}

bool CgmPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	CustColors.clear();
	QFileInfo fi = QFileInfo(fName);
	if ( !ScCore->usingGUI() )
	{
		interactive = false;
		showProgress = false;
	}
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if ( showProgress )
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
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
		progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = 0.0;
	h = 0.0;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
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
			m_Doc=ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
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
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
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
				m_Doc->itemSelection_DeleteItem(tmpSel);
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

CgmPlug::~CgmPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

void CgmPlug::parseHeader(QString fName, double &b, double &h)
{
}

bool CgmPlug::convert(QString fn)
{
	QString tmp;
	Coords.resize(0);
	Coords.svgInit();
	importedColors.clear();
	QList<PageItem*> gElements;
	groupStack.push(gElements);
	currentItemNr = 0;
	importRunning = true;
	firstPage = true;
	vcdSet = false;
	metaFileVersion = 1;
	vdcType = 0;
	vdcInt = 16;
	vdcReal = 1;
	vdcMantissa = 16;
	vcdFlippedH = false;
	vcdFlippedV = false;
	intPrecision = 16;
	realPrecision = 1;
	realMantissa = 16;
	realFraction = 16;
	realPrecisionSet = false;
	indexPrecision = 16;
	colorPrecision = 8;
	colorIndexPrecision = 8;
	maxColorIndex = 63;
	colorModel = 1;
	colorMode = 0;
	namePrecision = 16;
	metaFileScaleMode = 0;
	metaFileScale = 1.0;
	metaScale = 1.0;
	lineWidthMode = 1;
	edgeWidthMode = 1;
	markerSizeMode = 1;
	viewPortScale = 1.0;
	viewPortScaleMode = 0;
	lineType = Qt::SolidLine;
	lineCap = Qt::FlatCap;
	lineJoin = Qt::MiterJoin;
	lineWidth = 0.0;
	lineColor = "Black";
	edgeType = Qt::SolidLine;
	edgeCap = Qt::FlatCap;
	edgeJoin = Qt::MiterJoin;
	edgeWidth = 0.0;
	edgeColor = "Black";
	fillColor = "White";
	fillType = 1;
	minColor = 0;
	maxColor = 255;
	clipRect = QRectF();
	useClipRect = true;
	clipSet = false;
	lineVisible = true;
	recordRegion = false;
	wasEndPic = false;
	currentRegion = 0;
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		oldDocItemCount = m_Doc->Items->count();
		int fSize = (int) f.size();
		if (progressDialog)
		{
			progressDialog->setTotalSteps("GI", fSize);
			qApp->processEvents();
		}
		QDataStream ts(&f);
		ts.setByteOrder(QDataStream::BigEndian);
		quint32 magic;
		ts >> magic;
		ts.device()->seek(0);
		if (magic == 0x4265674D)
			decodeText(f);
		else
		{
			while (!ts.atEnd() && (importRunning))
			{
				quint16 data, elemClass, elemID, paramLen;
				ts >> data;
				elemClass = (data & 0xF000) >> 12;
				elemID    = (data & 0x0FE0) >>  5;
				paramLen  = data & 0x001F;
				if (paramLen == 31)
					ts >> paramLen;
				decodeBinary(ts, elemClass, elemID, paramLen);
				if (progressDialog)
				{
					progressDialog->setProgress("GI", ts.device()->pos());
					qApp->processEvents();
				}
			}
		}
		f.close();
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
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}

void CgmPlug::decodeText(QFile &f)
{
	qDebug() << "Parsing Text Data not supported yet";
}

/* Start binary Decoder */
void CgmPlug::decodeBinary(QDataStream &ts, quint16 elemClass, quint16 elemID, quint16 paramLen)
{
	if (elemClass == 0)
		decodeClass0(ts, elemID, paramLen);
	else if (elemClass == 1)
		decodeClass1(ts, elemID, paramLen);
	else if (elemClass == 2)
		decodeClass2(ts, elemID, paramLen);
	else if (elemClass == 3)
		decodeClass3(ts, elemID, paramLen);
	else if (elemClass == 4)
		decodeClass4(ts, elemID, paramLen);
	else if (elemClass == 5)
		decodeClass5(ts, elemID, paramLen);
	else if (elemClass == 6)
		decodeClass6(ts, elemID, paramLen);
	else if (elemClass == 7)
		decodeClass7(ts, elemID, paramLen);
	else if (elemClass == 8)
		decodeClass8(ts, elemID, paramLen);
	else if (elemClass == 9)
		decodeClass9(ts, elemID, paramLen);
	else
	{
		qDebug() << "Class" << elemClass << "ID" << elemID << "Len" << paramLen << "at" << ts.device()->pos();
		alignStreamToWord(ts, paramLen);
	}
	alignStreamToWord(ts, 0);
}
void CgmPlug::decodeClass0(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 0)
	{
		alignStreamToWord(ts, paramLen);
		// qDebug() << "NO OP";
	}
	else if (elemID == 1)
		handleStartMetaFile(getBinaryText(ts));
	else if (elemID == 2)
	{
		alignStreamToWord(ts, paramLen);
		importRunning = false;
		// qDebug() << "END METAFILE";
	}
	else if (elemID == 3)
		handleStartPicture(getBinaryText(ts));
	else if (elemID == 4)
	{
		alignStreamToWord(ts, paramLen);
		if (vcdSet)
		{
			double w = vdcWidth * metaScale;
			double h = vdcHeight * metaScale;
			handleStartPictureBody(w, h);
		}
		else
		{
			handleStartPictureBody(docWidth, docHeight);
			firstPage = true;
		}
		qDebug() << "BEGIN PICTURE BODY";
	}
	else if (elemID == 5)
	{
		alignStreamToWord(ts, paramLen);
		if (vcdSet)
		{
			if (firstPage)
			{
				double w = vdcWidth * metaScale;
				double h = vdcHeight * metaScale;
				handleStartPictureBody(w, h);
			}
		}
		else
		{
			if (firstPage)
				handleStartPictureBody(docWidth, docHeight);
		}
		wasEndPic = true;
		qDebug() << "END PICTURE";
	}
	else if (elemID == 6)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BEGIN SEGMENT";
	}
	else if (elemID == 7)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "END SEGMENT";
	}
	else if (elemID == 8)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BEGIN FIGURE";
	}
	else if (elemID == 9)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "END FIGURE";
	}
	else if (elemID == 13)
	{
		uint type = getBinaryUInt(ts, indexPrecision);
		currentRegion = type;
		recordRegion = true;
		regionPath = QPainterPath();
		// qDebug() << "BEGIN PROTECTION REGION" << type;
	}
	else if (elemID == 14)
	{
		recordRegion = false;
		regionMap.insert(currentRegion, regionPath);
		// qDebug() << "END PROTECTION REGION";
	}
	else if (elemID == 15)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BEGIN COMPOUND LINE";
	}
	else if (elemID == 16)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "END COMPOUND LINE";
	}
	else if (elemID == 17)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BEGIN COMPOUND TEXT PATH";
	}
	else if (elemID == 18)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "END COMPOUND TEXT PATH";
	}
	else if (elemID == 19)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BEGIN TILE ARRAY";
	}
	else if (elemID == 20)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "END TILE ARRAY";
	}
	else if (elemID == 21)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BEGIN APPLICATION STRUCTURE";
	}
	else if (elemID == 22)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BEGIN APPLICATION STRUCTURE BODY";
	}
	else if (elemID == 23)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "END APPLICATION STRUCTURE";
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 0 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass1(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	quint16 data;
	if (elemID == 1)
	{
		ts >> data;
		metaFileVersion = data;
 		qDebug() << "METAFILE VERSION" << data;
	}
	else if (elemID == 2)
		handleMetaFileDescription(getBinaryText(ts));
	else if (elemID == 3)
	{
		ts >> data;
		vdcType = data;
 		qDebug() << "VDC TYPE" << data;
	}
	else if (elemID == 4)
	{
		ts >> data;
		intPrecision = data;
 		qDebug() << "INTEGER PRECISION" << data;
	}
	else if (elemID == 5)
	{
		ts >> data;
		realPrecision = data;
		ts >> data;
		realMantissa = data;
		ts >> data;
		realFraction = data;
		if (realPrecision == 0)
			realPrecisionSet = true;
 		qDebug() << "REAL PRECISION" << realPrecision << realMantissa << realFraction;
	}
	else if (elemID == 6)
	{
		ts >> data;
		indexPrecision = data;
 		qDebug() << "INDEX PRECISION" << indexPrecision;
	}
	else if (elemID == 7)
	{
		ts >> data;
		colorPrecision = data;
		qDebug() << "COLOUR PRECISION" << colorPrecision;
	}
	else if (elemID == 8)
	{
		ts >> data;
		colorIndexPrecision = data;
		qDebug() << "COLOUR INDEX PRECISION" << colorIndexPrecision;
	}
	else if (elemID == 9)
	{
		ts >> data;
		maxColorIndex = data;
		// qDebug() << "MAXIMUM COLOUR INDEX" << maxColorIndex;
	}
	else if (elemID == 10)
	{
		if (colorModel == 1)		// RGB
		{
			if (colorPrecision == 8)
			{
				quint8 r, g, b;
				ts >> r >> g >> b;
				minColor = r;
				ts >> r >> g >> b;
				maxColor = r;
			}
			else if (colorPrecision == 16)
			{
				quint16 r, g, b;
				ts >> r >> g >> b;
				minColor = r;
				ts >> r >> g >> b;
				maxColor = r;
			}
		}
		else if (colorModel == 4)	// CMYK
		{
			if (colorPrecision == 8)
			{
				quint8 c, m, y, k;
				ts >> c >> m >> y >> k;
				minColor = c;
				ts >> c >> m >> y >> k;
				maxColor = c;
			}
			else if (colorPrecision == 16)
			{
				quint16 c, m, y, k;
				ts >> c >> m >> y >> k;
				minColor = c;
				ts >> c >> m >> y >> k;
				maxColor = c;
			}
		}
		else
			alignStreamToWord(ts, paramLen);
		// qDebug() << "COLOUR VALUE EXTENT" << minColor << maxColor;
	}
	else if (elemID == 11)
	{
		alignStreamToWord(ts, paramLen);
		// qDebug() << "METAFILE ELEMENT LIST";
	}
	else if (elemID == 12)
	{
		alignStreamToWord(ts, 0);
		// qDebug() << "METAFILE DEFAULTS REPLACEMENT" << paramLen;
	}
	else if (elemID == 13)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "FONT LIST";
	}
	else if (elemID == 14)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CHARACTER SET LIST";
	}
	else if (elemID == 15)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CHARACTER CODING ANNOUNCER";
	}
	else if (elemID == 16)
	{
		ts >> data;
		namePrecision = data;
 		// qDebug() << "NAME PRECISION" << namePrecision;
	}
	else if (elemID == 17)
	{
		QPointF max, min;
		max = getBinaryCoords(ts);
		min = getBinaryCoords(ts);
 		// qDebug() << "MAXIMUM VDC EXTENT" << min.x() << min.y() << max.x() << max.y();
	}
	else if (elemID == 18)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SEGMENT PRIORITY EXTENT";
	}
	else if (elemID == 19)
	{
		ts >> data;
		colorModel = data;
		// qDebug() << "COLOUR MODEL" << colorModel;
	}
	else if (elemID == 20)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "COLOUR CALIBRATION";
	}
	else if (elemID == 21)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "FONT PROPERTIES";
	}
	else if (elemID == 22)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "GLYPH MAPPING";
	}
	else if (elemID == 23)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SYMBOL LIBRARY LIST";
	}
	else if (elemID == 24)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "PICTURE DIRECTORY";
	}
	else
	{
		qDebug() << "Class 1 ID" << elemID << "Len" << paramLen;
		alignStreamToWord(ts, paramLen);
	}
}

void CgmPlug::decodeClass2(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	quint16 data;
	if (elemID == 1)
	{
		ts >> data;
		metaFileScaleMode = data;
		double sc = 1.0;
		if (realPrecisionSet)
			sc = getBinaryReal(ts, 0, realMantissa);
		else
			sc = getBinaryReal(ts, 0, 9);
		if (metaFileScaleMode != 0)
			metaFileScale = sc;
		qDebug() << "SCALING MODE" << metaFileScaleMode << metaFileScale;
	}
	else if (elemID == 2)
	{
		ts >> data;
		colorMode = data;
		qDebug() << "COLOUR SELECTION MODE" << colorMode;
	}
	else if (elemID == 3)
	{
		ts >> data;
		lineWidthMode = data;
		// qDebug() << "LINE WIDTH SPECIFICATION MODE" << lineWidthMode;
	}
	else if (elemID == 4)
	{
		ts >> data;
		markerSizeMode = data;
		// qDebug() << "MARKER SIZE SPECIFICATION MODE" << markerSizeMode;
	}
	else if (elemID == 5)
	{
		ts >> data;
		edgeWidthMode = data;
		// qDebug() << "EDGE WIDTH SPECIFICATION MODE" << edgeWidthMode;
	}
	else if (elemID == 6)
	{
		QPointF max, min;
		max = getBinaryCoords(ts, true);
		min = getBinaryCoords(ts, true);
		QRectF vd = QRectF(max, min);
		if (vd.height() > 0)
			vcdFlippedV = true;
		if (vd.width() < 0)
			vcdFlippedH = true;
		vd = vd.normalized();
		vdcWidth = vd.width();
		vdcHeight = vd.height();
		metaScale = 400.0 / qMax(vdcWidth, vdcHeight);
		lineWidth = qMax(vdcWidth, vdcHeight) / 1000;
		baseX = -vd.left() * metaScale;
		baseY = vd.top() * metaScale;
		vcdSet = true;
		if (!clipSet)
			clipRect = QRectF(vd.left() * metaScale, vd.top() * metaScale, vdcWidth * metaScale, vdcHeight * metaScale);
		// qDebug() << "VDC EXTENT" << vd.left() << vd.top() << vdcWidth << vdcHeight << metaScale;
	}
	else if (elemID == 7)
	{
		ScColor color = getBinaryDirectColor(ts);
		QString back = handleColor(color, "FromCGM"+color.name());
		// qDebug() << "BACKGROUND COLOUR" << back;
	}
	else if (elemID == 8)
	{
		QPointF max, min;
		max = getBinaryCoords(ts);
		min = getBinaryCoords(ts);
		// qDebug() << "DEVICE VIEWPORT" << min.x() << min.y() << max.x() << max.y();
	}
	else if (elemID == 9)
	{
		ts >> data;
		viewPortScaleMode = data;
		if (realPrecisionSet)
			viewPortScale = getBinaryReal(ts, 0, realMantissa);
		else
			viewPortScale = getBinaryReal(ts, 0, 9);
		// qDebug() << "DEVICE VIEWPORT SPECIFICATION MODE" << viewPortScaleMode << viewPortScale;
	}
	else if (elemID == 10)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "DEVICE VIEWPORT MAPPING";
	}
	else if (elemID == 11)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "LINE REPRESENTATION";
	}
	else if (elemID == 12)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "MARKER REPRESENTATION";
	}
	else if (elemID == 13)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT REPRESENTATION";
	}
	else if (elemID == 14)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "FILL REPRESENTATION";
	}
	else if (elemID == 15)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "EDGE REPRESENTATION";
	}
	else if (elemID == 16)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "INTERIOR STYLE SPECIFICATION MODE";
	}
	else if (elemID == 17)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "LINE AND EDGE TYPE DEFINITION";
	}
	else if (elemID == 18)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "HATCH STYLE DEFINITION";
	}
	else if (elemID == 19)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "GEOMETRIC PATTERN DEFINITION";
	}
	else if (elemID == 20)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "APPLICATION STRUCTURE DIRECTORY";
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 2 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass3(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	quint16 data;
	if (elemID == 1)
	{
		ts >> data;
		vdcInt = data;
 		// qDebug() << "VDC INTEGER PRECISION" << vdcInt;
	}
	else if (elemID == 2)
	{
		ts >> data;
		vdcReal = data;
		ts >> data;
		vdcMantissa = data;
		ts >> data;
 		// qDebug() << "VDC REAL PRECISION" << vdcReal << vdcMantissa;
	}
	else if (elemID == 3)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "AUXILIARY COLOUR";
	}
	else if (elemID == 4)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TRANSPARENCY";
	}
	else if (elemID == 5)
	{
		QPointF max, min;
		max = getBinaryCoords(ts);
		min = getBinaryCoords(ts);
		QRectF vd = QRectF(max, min);
		vd = vd.normalized();
		double w = convertCoords(vd.width());
		double h = convertCoords(vd.height());
		double x = convertCoords(vd.left());
		double y = convertCoords(vd.top());
		x += m_Doc->currentPage()->xOffset();
		y += m_Doc->currentPage()->yOffset();
		clipRect = QRectF(x, y, w, h);
		clipSet = true;
		// qDebug() << "CLIP RECTANGLE" << clipRect;
	}
	else if (elemID == 6)
	{
		ts >> data;
		if (data == 0)
			useClipRect = false;
		else
			useClipRect = true;
		// qDebug() << "CLIP INDICATOR" << useClipRect;
	}
	else if (elemID == 7)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "LINE CLIPPING MODE";
	}
	else if (elemID == 8)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "MARKER CLIPPING MODE";
	}
	else if (elemID == 9)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "EDGE CLIPPING MODE";
	}
	else if (elemID == 10)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "NEW REGION";
	}
	else if (elemID == 11)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SAVE PRIMITIVE CONTEXT";
	}
	else if (elemID == 12)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "RESTORE PRIMITIVE CONTEXT";
	}
	else if (elemID == 17)
	{
		uint index = getBinaryUInt(ts, indexPrecision);
		uint type = getBinaryUInt(ts, indexPrecision);
		if (type == 1)
		{
			if (groupStack.count() != 0)
			{
				QList<PageItem*> gElements = groupStack.pop();
				tmpSel->clear();
				if (gElements.count() > 0)
				{
					for (int dre = 0; dre < gElements.count(); ++dre)
					{
						tmpSel->addItem(gElements.at(dre), true);
					}
					m_Doc->itemSelection_GroupObjects(false, false, tmpSel);
					PageItem *ite = tmpSel->itemAt(0);
					QPainterPath clip = regionMap[index];
					if (!clip.isEmpty())
					{
						ite->PoLine.fromQPainterPath(clip);
						ite->PoLine.translate(-ite->xPos(), -ite->yPos());
						ite->PoLine.translate(baseX, baseY);
					}
					Elements.append(ite);
				}
				if (groupStack.count() != 0)
				{
					for (int as = 0; as < tmpSel->count(); ++as)
					{
						groupStack.top().append(tmpSel->itemAt(as));
					}
				}
				tmpSel->clear();
			}
		}
		else if ((type == 2) || (type == 3))
		{
			QList<PageItem*> gElements;
			groupStack.push(gElements);
		}
		// qDebug() << "PROTECTION REGION INDICATOR" << index << type;
	}
	else if (elemID == 18)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "GENERALIZED TEXT PATH MODE";
	}
	else if (elemID == 19)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "MITRE LIMIT";
	}
	else if (elemID == 20)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TRANSPARENT CELL COLOUR";
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 3 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass4(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		getBinaryPath(ts, paramLen);
		if (Coords.size() > 3)
		{
			Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (recordRegion)
				regionPath.addPath(Coords.toQPainterPath(false));
			else
			{
				int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, CommonStrings::None, lineColor, true);
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = Coords.copy();
				finishItem(ite);
			}
		}
		// qDebug() << "POLYLINE";
	}
	else if (elemID == 2)
	{
		getBinaryPath(ts, paramLen, true);
		if (Coords.size() > 3)
		{
			Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (recordRegion)
				regionPath.addPath(Coords.toQPainterPath(false));
			else
			{
				int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, CommonStrings::None, lineColor, true);
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = Coords.copy();
				finishItem(ite);
			}
		}
		// qDebug() << "DISJOINT POLYLINE";
	}
	else if (elemID == 3)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "POLYMARKER";
	}
	else if (elemID == 4)
	{
		qDebug() << "TEXT Len" << paramLen << "at" << ts.device()->pos();
		alignStreamToWord(ts, paramLen);
	}
	else if (elemID == 5)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "RESTRICTED TEXT";
	}
	else if (elemID == 6)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "APPEND TEXT";
	}
	else if (elemID == 7)
	{
		getBinaryPath(ts, paramLen);
		if (Coords.size() > 3)
		{
			Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (recordRegion)
				regionPath.addPath(Coords.toQPainterPath(true));
			else
			{
				int z;
				if (lineVisible)
				{
					if ((fillType != 0) || (fillType != 4))
						z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, fillColor, edgeColor, true);
					else
						z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, CommonStrings::None, edgeColor, true);
				}
				else
				{
					if ((fillType != 0) || (fillType != 4))
						z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, fillColor, CommonStrings::None, true);
					else
						z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, CommonStrings::None, CommonStrings::None, true);
				}
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = Coords.copy();
				finishItem(ite, false);
			}
		}
		// qDebug() << "POLYGON";
	}
	else if (elemID == 8)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "POLYGON SET";
	}
	else if (elemID == 9)
	{
		int pos = ts.device()->pos();
		QPointF p, q, r;
		int nx, ny, cp;
		quint16 mode;
		p = getBinaryCoords(ts);
		q = getBinaryCoords(ts);
		r = getBinaryCoords(ts);
		nx = getBinaryUInt(ts, intPrecision);
		ny = getBinaryUInt(ts, intPrecision);
		cp = getBinaryUInt(ts, intPrecision);
		ts >> mode;
		int bytesRead = ts.device()->pos() - pos;
		qDebug() << "CELL ARRAY at" << ts.device()->pos() << paramLen;
		qDebug() << "Size" << nx << ny << "Colormode" << cp << "Compression" << mode;
//		double distY = distance(q.x() - p.x(), q.y() - p.y());
		double distX = distance(r.x() - p.x(), r.y() - p.y());
		double distY = nx / distX * ny;
		int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, baseX + convertCoords(p.x()), baseY + convertCoords(p.y()), convertCoords(distX), convertCoords(distY), edgeWidth, CommonStrings::None, CommonStrings::None, true);
		PageItem *ite = m_Doc->Items->at(z);
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite, false);
		quint16 flag = paramLen & 0x8000;
		quint16 pLen = (paramLen & 0x7FFF) - bytesRead;
		QByteArray imageData;
		imageData.resize(0);
		QByteArray rD = ts.device()->read(pLen);
		imageData.append(rD);
		while (flag)
		{
			ts >> pLen;
			flag = pLen & 0x8000;
			pLen = pLen & 0x7FFF;
			QByteArray rD = ts.device()->read(pLen);
			imageData.append(rD);
		}
		ts.device()->seek(pos);
		alignStreamToWord(ts, paramLen);
		QImage image = QImage(nx, ny, QImage::Format_ARGB32);
		if (mode == 1)
		{
			if (cp == 24)
			{
				int baseAdr = 0;
				for (int yy = 0; yy < ny; yy++)
				{
					QRgb *q = (QRgb*)(image.scanLine(yy));
					int rowCount = 0;
					for (int xx = 0; xx < nx; xx++)
					{
						uchar r, g, b;
						r = imageData[baseAdr + rowCount];
						g = imageData[baseAdr + rowCount + 1];
						b = imageData[baseAdr + rowCount + 2];
						*q++ = qRgba(r, g, b, 255);
						rowCount += 3;
					}
					baseAdr += 3 * nx;
					int adj = baseAdr % 2;
					if (adj != 0)
						baseAdr++;
				}
			}
		}
		ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_cgm_XXXXXX.png");
		ite->tempImageFile->open();
		QString fileName = getLongPathName(ite->tempImageFile->fileName());
		ite->tempImageFile->close();
		ite->isInlineImage = true;
		image.save(fileName, "PNG");
		m_Doc->LoadPict(fileName, z);
		ite->setImageScalingMode(false, false);
	}
	else if (elemID == 10)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "GENERALIZED DRAWING PRIMITIVE";
	}
	else if (elemID == 11)
	{
		QPointF max, min;
		max = getBinaryCoords(ts);
		min = getBinaryCoords(ts);
		QRectF vd = QRectF(max, min);
		vd = vd.normalized();
		double w = convertCoords(vd.width());
		double h = convertCoords(vd.height());
		double x = convertCoords(vd.left());
		double y = convertCoords(vd.top());
		if (recordRegion)
			regionPath.addRect(QRectF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset(), w, h));
		else
		{
			int z;
			if (lineVisible)
			{
				if ((fillType != 0) || (fillType != 4))
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x, baseY + y, w, h, edgeWidth, fillColor, edgeColor, true);
				else
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x, baseY + y, w, h, edgeWidth, CommonStrings::None, edgeColor, true);
			}
			else
			{
				if ((fillType != 0) || (fillType != 4))
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x, baseY + y, w, h, edgeWidth, fillColor, CommonStrings::None, true);
				else
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x, baseY + y, w, h, edgeWidth, CommonStrings::None, CommonStrings::None, true);
			}
			PageItem *ite = m_Doc->Items->at(z);
			ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(ite, false);
		}
 		// qDebug() << "RECTANGLE";
	}
	else if (elemID == 12)
	{
		QPointF max, min;
		max = getBinaryCoords(ts);
		double x = convertCoords(max.x());
		double y = convertCoords(max.y());
		double r = convertCoords(getBinaryDistance(ts));
		x = x - r;
		y = y - r;
		if (recordRegion)
			regionPath.addEllipse(QPointF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset()), r, r);
		else
		{
			int z;
			if (lineVisible)
			{
				if ((fillType != 0) || (fillType != 4))
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, r * 2.0, r * 2.0, edgeWidth, fillColor, edgeColor, true);
				else
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, r * 2.0, r * 2.0, edgeWidth, CommonStrings::None, edgeColor, true);
			}
			else
			{
				if ((fillType != 0) || (fillType != 4))
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, r * 2.0, r * 2.0, edgeWidth, fillColor, CommonStrings::None, true);
				else
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, r * 2.0, r * 2.0, edgeWidth, CommonStrings::None, CommonStrings::None, true);
			}
			PageItem *ite = m_Doc->Items->at(z);
			ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(ite, false);
		}
 		// qDebug() << "CIRCLE";
	}
	else if (elemID == 13)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CIRCULAR ARC 3 POINT";
	}
	else if (elemID == 14)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CIRCULAR ARC 3 POINT CLOSE";
	}
	else if (elemID == 15)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CIRCULAR ARC CENTRE";
	}
	else if (elemID == 16)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CIRCULAR ARC CENTRE CLOSE";
	}
	else if (elemID == 17)
	{
		QPointF center, r1, r2;
		center = getBinaryCoords(ts);
		double cx = convertCoords(center.x());
		double cy = convertCoords(center.y());
		r1 = getBinaryCoords(ts);
		double r1x = convertCoords(r1.x());
		double r1y = convertCoords(r1.y());
		r2 = getBinaryCoords(ts);
		double r2x = convertCoords(r2.x());
		double r2y = convertCoords(r2.y());
		double distX = distance(r1x - cx, r1y - cy);
		double distY = distance(r2x - cx, r2y - cy);
		double rotB = xy2Deg(r1x - cx, r1y - cy);
		QPainterPath ell;
		ell.addEllipse(QPointF(cx, cy), distX, distY);
		QTransform mm;
		mm.rotate(rotB);
		ell = mm.map(ell);
		if (recordRegion)
			regionPath.addPath(ell);
		else
		{
			Coords.fromQPainterPath(ell);
			int z;
			if (lineVisible)
			{
				if ((fillType != 0) || (fillType != 4))
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, fillColor, edgeColor, true);
				else
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, CommonStrings::None, edgeColor, true);
			}
			else
			{
				if ((fillType != 0) || (fillType != 4))
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, fillColor, CommonStrings::None, true);
				else
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, edgeWidth, CommonStrings::None, CommonStrings::None, true);
			}
			PageItem *ite = m_Doc->Items->at(z);
			ite->PoLine = Coords.copy();
			ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(ite, false);
		}
		// qDebug() << "ELLIPSE";
	}
	else if (elemID == 18)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "ELLIPTICAL ARC";
	}
	else if (elemID == 19)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "ELLIPTICAL ARC CLOSE";
	}
	else if (elemID == 20)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CIRCULAR ARC CENTRE REVERSED";
	}
	else if (elemID == 21)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CONNECTING EDGE";
	}
	else if (elemID == 22)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "HYPERBOLIC ARC";
	}
	else if (elemID == 23)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "PARABOLIC ARC";
	}
	else if (elemID == 24)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "NON-UNIFORM B-SPLINE";
	}
	else if (elemID == 25)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "NON-UNIFORM RATIONAL B-SPLINE";
	}
	else if (elemID == 26)
	{
		getBinaryBezierPath(ts, paramLen);
		if (Coords.size() > 3)
		{
			Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (recordRegion)
				regionPath.addPath(Coords.toQPainterPath(false));
			else
			{
				int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, CommonStrings::None, lineColor, true);
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = Coords.copy();
				finishItem(ite);
			}
		}
		// qDebug() << "POLYBEZIER";
	}
	else if (elemID == 27)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "POLYSYMBOL";
	}
	else if (elemID == 28)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "BITONAL TILE";
	}
	else if (elemID == 29)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TILE";
	}
	else
	{
		qDebug() << "Class 4 ID" << elemID << "Len" << paramLen;
		alignStreamToWord(ts, paramLen);
	}
}

void CgmPlug::decodeClass5(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		lineBundleIndex = getBinaryUInt(ts, indexPrecision);
 		// qDebug() << "LINE BUNDLE INDEX" << lineBundleIndex;
	}
	else if (elemID == 2)
	{
		uint type = getBinaryUInt(ts, indexPrecision);
		if (type == 1)
			lineType = Qt::SolidLine;
		else if (type == 2)
			lineType = Qt::DashLine;
		else if (type == 3)
			lineType = Qt::DotLine;
		else if (type == 4)
			lineType = Qt::DashDotLine;
		else if (type == 5)
			lineType = Qt::DashDotDotLine;
		else
			lineType = Qt::SolidLine;
 		// qDebug() << "LINE TYPE" << lineType;
	}
	else if (elemID == 3)
	{
		lineWidth = getBinaryDistance(ts);
		lineWidth *= metaScale;
 		// qDebug() << "LINE WIDTH" << lineWidth;
	}
	else if (elemID == 4)
	{
		lineColor = getBinaryColor(ts);
		// qDebug() << "LINE COLOUR" << lineColor;
	}
	else if (elemID == 5)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "MARKER BUNDLE INDEX";
	}
	else if (elemID == 6)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "MARKER TYPE";
	}
	else if (elemID == 7)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "MARKER SIZE";
	}
	else if (elemID == 8)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "MARKER COLOUR";
	}
	else if (elemID == 9)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT BUNDLE INDEX";
	}
	else if (elemID == 10)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT FONT INDEX";
	}
	else if (elemID == 11)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT PRECISION";
	}
	else if (elemID == 12)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CHARACTER EXPANSION FACTOR";
	}
	else if (elemID == 13)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CHARACTER SPACING";
	}
	else if (elemID == 14)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT COLOUR";
	}
	else if (elemID == 15)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CHARACTER HEIGHT";
	}
	else if (elemID == 16)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CHARACTER ORIENTATION";
	}
	else if (elemID == 17)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT PATH";
	}
	else if (elemID == 18)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT ALIGNMENT";
	}
	else if (elemID == 19)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CHARACTER SET INDEX";
	}
	else if (elemID == 20)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "ALTERNATE CHARACTER SET INDEX";
	}
	else if (elemID == 21)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "FILL BUNDLE INDEX";
	}
	else if (elemID == 22)
	{
		quint16 data;
		ts >> data;
		fillType = data;
 		// qDebug() << "INTERIOR STYLE" << fillType;
	}
	else if (elemID == 23)
	{
		fillColor = getBinaryColor(ts);
		// qDebug() << "Fill COLOUR" << fillColor;
	}
	else if (elemID == 24)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "HATCH INDEX";
	}
	else if (elemID == 25)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "PATTERN INDEX";
	}
	else if (elemID == 26)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "EDGE BUNDLE INDEX";
	}
	else if (elemID == 27)
	{
		uint type = getBinaryUInt(ts, indexPrecision);
		if (type == 1)
			edgeType = Qt::SolidLine;
		else if (type == 2)
			edgeType = Qt::DashLine;
		else if (type == 3)
			edgeType = Qt::DotLine;
		else if (type == 4)
			edgeType = Qt::DashDotLine;
		else if (type == 5)
			edgeType = Qt::DashDotDotLine;
		else
			edgeType = Qt::SolidLine;
 		// qDebug() << "EDGE TYPE";
	}
	else if (elemID == 28)
	{
		edgeWidth = getBinaryDistance(ts);
		edgeWidth *= metaScale;
 		// qDebug() << "EDGE WIDTH" << edgeWidth;
	}
	else if (elemID == 29)
	{
		edgeColor = getBinaryColor(ts);
		// qDebug() << "EDGE COLOUR" << edgeColor;
	}
	else if (elemID == 30)
	{
		quint16 data;
		ts >> data;
		if (data == 0)
			lineVisible = false;
		else
			lineVisible = true;
 		// qDebug() << "EDGE VISIBILITY";
	}
	else if (elemID == 31)
	{
		QPointF p = getBinaryCoords(ts);
		double x = convertCoords(p.x());
		double y = convertCoords(p.y());
		fillRefPoint = QPointF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset());
 		// qDebug() << "FILL REFERENCE POINT" << fillRefPoint;
	}
	else if (elemID == 32)
	{
		alignStreamToWord(ts, paramLen);
 		qDebug() << "PATTERN TABLE";
	}
	else if (elemID == 33)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "PATTERN SIZE";
	}
	else if (elemID == 34)
	{
		getBinaryColorTable(ts, paramLen);
		// qDebug() << "COLOUR TABLE";
	}
	else if (elemID == 35)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "ASPECT SOURCE FLAGS";
	}
	else if (elemID == 36)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "PICK IDENTIFIER";
	}
	else if (elemID == 37)
	{
		uint type = getBinaryUInt(ts, indexPrecision);
		if (type == 1)
			lineCap = Qt::FlatCap;
		else if (type == 2)
			lineCap = Qt::RoundCap;
		else if (type == 3)
			lineCap = Qt::SquareCap;
		else
			lineCap = Qt::FlatCap;
		type = getBinaryUInt(ts, indexPrecision);		// dummy reading unsupported parameter
		// qDebug() << "LINE CAP";
	}
	else if (elemID == 38)
	{
		uint type = getBinaryUInt(ts, indexPrecision);
		if (type == 1)
			lineJoin = Qt::MiterJoin;
		else if (type == 2)
			lineJoin = Qt::RoundJoin;
		else if (type == 3)
			lineJoin = Qt::BevelJoin;
		else
			lineJoin = Qt::MiterJoin;
		// qDebug() << "LINE JOIN";
	}
	else if (elemID == 39)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "LINE TYPE CONTINUATION";
	}
	else if (elemID == 40)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "LINE TYPE INITIAL OFFSET";
	}
	else if (elemID == 41)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "TEXT SCORE TYPE";
	}
	else if (elemID == 42)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "RESTRICTED TEXT TYPE";
	}
	else if (elemID == 43)
	{
/*		int pos = ts.device()->pos();
		uint type = getBinaryUInt(ts, indexPrecision);
		QPointF p, p2;
		if (type == 1)
		{
			int posE = ts.device()->pos();
			p = QPointF(convertCoords(getBinaryDistance(ts)), convertCoords(getBinaryDistance(ts)));
			qDebug() << "End Point" << p << " at" << posE;
		}
		else if (type == 2)
		{
			int posE = ts.device()->pos();
			p = QPointF(convertCoords(getBinaryDistance(ts)), convertCoords(getBinaryDistance(ts)));
			p2 = QPointF(convertCoords(getBinaryDistance(ts)), convertCoords(getBinaryDistance(ts)));
			qDebug() << "Points" << p << p2 << " at" << posE;
		}
		int posI = ts.device()->pos();
		uint index = getBinaryUInt(ts, intPrecision);
		qDebug() << "Stages " << index << " at" << posI;
		for (uint s = 0; s < index; s++)
		{
			double s1 = getBinaryReal(ts, realPrecision, realMantissa);
			qDebug() << "first 2 Stages " << s1;
		}
		ts.device()->seek(pos); */
		alignStreamToWord(ts, paramLen);
		qDebug() << "INTERPOLATED INTERIOR";
	}
	else if (elemID == 44)
	{
		uint type = getBinaryUInt(ts, indexPrecision);
		if (type == 1)
			edgeCap = Qt::FlatCap;
		else if (type == 2)
			edgeCap = Qt::RoundCap;
		else if (type == 3)
			edgeCap = Qt::SquareCap;
		else
			edgeCap = Qt::FlatCap;
		type = getBinaryUInt(ts, indexPrecision);		// dummy reading unsupported parameter
 		// qDebug() << "EDGE CAP";
	}
	else if (elemID == 45)
	{
		uint type = getBinaryUInt(ts, indexPrecision);
		if (type == 1)
			edgeJoin = Qt::MiterJoin;
		else if (type == 2)
			edgeJoin = Qt::RoundJoin;
		else if (type == 3)
			edgeJoin = Qt::BevelJoin;
		else
			edgeJoin = Qt::MiterJoin;
		// qDebug() << "EDGE JOIN";
	}
	else if (elemID == 46)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "EDGE TYPE CONTINUATION";
	}
	else if (elemID == 47)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "EDGE TYPE INITIAL OFFSET";
	}
	else if (elemID == 48)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SYMBOL LIBRARY INDEX";
	}
	else if (elemID == 49)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SYMBOL COLOUR";
	}
	else if (elemID == 50)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SYMBOL SIZE";
	}
	else if (elemID == 51)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SYMBOL ORIENTATION";
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 5 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass6(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "ESCAPE";
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 6 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass7(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "MESSAGE";
	}
	else if (elemID == 2)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "APPLICATION DATA" << paramLen << "at" << ts.device()->pos();
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 7 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass8(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "COPY SEGMENT";
	}
	else if (elemID == 2)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "INHERITANCE FILTER";
	}
	else if (elemID == 3)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "CLIP INHERITANCE";
	}
	else if (elemID == 4)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SEGMENT TRANSFORMATION";
	}
	else if (elemID == 5)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SEGMENT HIGHLIGHTING";
	}
	else if (elemID == 6)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SEGMENT DISPLAY PRIORITY";
	}
	else if (elemID == 7)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "SEGMENT PICK PRIORITY";
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 8 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass9(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "APPLICATION STRUCTURE ATTRIBUTE";
	}
	else
	{
		alignStreamToWord(ts, paramLen);
		qDebug() << "Class 9 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::getBinaryBezierPath(QDataStream &ts, quint16 paramLen)
{
	quint16 bytesRead = 0;
	bool first = true;
	Coords.resize(0);
	Coords.svgInit();
	quint16 flag;
	flag = paramLen & 0x8000;
	paramLen = paramLen & 0x7FFF;
	uint type = getBinaryUInt(ts, indexPrecision);
	while (bytesRead < paramLen - 2)
	{
		int posA = ts.device()->pos();
		if ((first) || (type == 1))
		{
			QPointF p = getBinaryCoords(ts);
			Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
			first = false;
		}
		QPointF p1 = getBinaryCoords(ts);
		QPointF p2 = getBinaryCoords(ts);
		QPointF p3 = getBinaryCoords(ts);
		Coords.svgCurveToCubic(convertCoords(p1.x()), convertCoords(p1.y()), convertCoords(p2.x()), convertCoords(p2.y()), convertCoords(p3.x()), convertCoords(p3.y()));
		int posN = ts.device()->pos();
		bytesRead += posN - posA;
	}
	while (flag)
	{
		bytesRead = 0;
		ts >> paramLen;
		flag = paramLen & 0x8000;
		paramLen = paramLen & 0x7FFF;
		while (bytesRead < paramLen)
		{
			int posA = ts.device()->pos();
			if (type == 1)
			{
				QPointF p = getBinaryCoords(ts);
				Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
			}
			QPointF p1 = getBinaryCoords(ts);
			QPointF p2 = getBinaryCoords(ts);
			QPointF p3 = getBinaryCoords(ts);
			Coords.svgCurveToCubic(convertCoords(p1.x()), convertCoords(p1.y()), convertCoords(p2.x()), convertCoords(p2.y()), convertCoords(p3.x()), convertCoords(p3.y()));
			int posN = ts.device()->pos();
			bytesRead += posN - posA;
		}
	}
}

void CgmPlug::getBinaryPath(QDataStream &ts, quint16 paramLen, bool disjoint)
{
	quint16 bytesRead = 0;
	bool first = true;
	Coords.resize(0);
	Coords.svgInit();
	quint16 flag;
	flag = paramLen & 0x8000;
	paramLen = paramLen & 0x7FFF;
	while (bytesRead < paramLen)
	{
		int posA = ts.device()->pos();
		QPointF p = getBinaryCoords(ts);
		if (first)
		{
			Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
			first = false;
		}
		else
		{
			Coords.svgLineTo(convertCoords(p.x()), convertCoords(p.y()));
			if (disjoint)
				first = true;
		}
		int posN = ts.device()->pos();
		bytesRead += posN - posA;
	}
	while (flag)
	{
		bytesRead = 0;
		ts >> paramLen;
		flag = paramLen & 0x8000;
		paramLen = paramLen & 0x7FFF;
		while (bytesRead < paramLen)
		{
			int posA = ts.device()->pos();
			if (disjoint)
			{
				QPointF p = getBinaryCoords(ts);
				if (first)
				{
					Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
					first = false;
				}
				else
				{
					Coords.svgLineTo(convertCoords(p.x()), convertCoords(p.y()));
					if (disjoint)
						first = true;
				}
			}
			else
			{
				QPointF p = getBinaryCoords(ts);
				Coords.svgLineTo(convertCoords(p.x()), convertCoords(p.y()));
			}
			int posN = ts.device()->pos();
			bytesRead += posN - posA;
		}
	}
}

void CgmPlug::getBinaryColorTable(QDataStream &ts, quint16 paramLen)
{
	quint16 flag;
	flag = paramLen & 0x8000;
	paramLen = paramLen & 0x7FFF;
	quint16 bytesRead = 0;
	int posA = ts.device()->pos();
	uint c = getBinaryUInt(ts, colorIndexPrecision);
	int posN = ts.device()->pos();
	bytesRead += posN - posA;
	QString tmpName = CommonStrings::None;
	while (bytesRead < paramLen)
	{
		posA = ts.device()->pos();
		ScColor cc = getBinaryDirectColor(ts);
		tmpName = handleColor(cc, "FromCGM"+cc.name());
		ColorTableMap.insert(c, tmpName);
		c++;
		posN = ts.device()->pos();
		bytesRead += posN - posA;
	}
	while (flag)
	{
		bytesRead = 0;
		ts >> paramLen;
		flag = paramLen & 0x8000;
		paramLen = paramLen & 0x7FFF;
		while (bytesRead < paramLen)
		{
			posA = ts.device()->pos();
			ScColor cc = getBinaryDirectColor(ts);
			ColorTableMap.insert(c, tmpName);
			c++;
			posN = ts.device()->pos();
			bytesRead += posN - posA;
		}
	}
}

ScColor CgmPlug::getBinaryDirectColor(QDataStream &ts)
{
	ScColor ret;
	if (colorModel == 1)		// RGB
	{
		if (colorPrecision == 8)
		{
			quint8 ri, gi, bi;
			ts >> ri >> gi >> bi;
			uint r = ri;
			uint g = gi;
			uint b = bi;
			r = qRound(r * (maxColor - minColor) / static_cast<double>(maxColor));
			g = qRound(g * (maxColor - minColor) / static_cast<double>(maxColor));
			b = qRound(b * (maxColor - minColor) / static_cast<double>(maxColor));
			ret = ScColor(r, g, b);
		}
		else if (colorPrecision == 16)
		{
			quint16 ri, gi, bi;
			ts >> ri >> gi >> bi;
			uint r = ri;
			uint g = gi;
			uint b = bi;
			r = qRound((r * (maxColor - minColor) / static_cast<double>(maxColor)) / static_cast<double>(maxColor) * 255.0);
			g = qRound((g * (maxColor - minColor) / static_cast<double>(maxColor)) / static_cast<double>(maxColor) * 255.0);
			b = qRound((b * (maxColor - minColor) / static_cast<double>(maxColor)) / static_cast<double>(maxColor) * 255.0);
			ret = ScColor(r, g, b);
		}
	}
	else if (colorModel == 4)	// CMYK
	{
		if (colorPrecision == 8)
		{
			quint8 ci, mi, yi, ki;
			ts >> ci >> mi >> yi >> ki;
			uint c = ci;
			uint m = mi;
			uint y = yi;
			uint k = ki;
			c = qRound(c * (maxColor - minColor) / static_cast<double>(maxColor));
			m = qRound(m * (maxColor - minColor) / static_cast<double>(maxColor));
			y = qRound(y * (maxColor - minColor) / static_cast<double>(maxColor));
			k = qRound(k * (maxColor - minColor) / static_cast<double>(maxColor));
			ret = ScColor(c, m, y, k);
		}
		else if (colorPrecision == 16)
		{
			quint16 ci, mi, yi, ki;
			ts >> ci >> mi >> yi >> ki;
			uint c = ci;
			uint m = mi;
			uint y = yi;
			uint k = ki;
			c = qRound((c * (maxColor - minColor) / static_cast<double>(maxColor)) / static_cast<double>(maxColor) * 255.0);
			m = qRound((m * (maxColor - minColor) / static_cast<double>(maxColor)) / static_cast<double>(maxColor) * 255.0);
			y = qRound((y * (maxColor - minColor) / static_cast<double>(maxColor)) / static_cast<double>(maxColor) * 255.0);
			k = qRound((k * (maxColor - minColor) / static_cast<double>(maxColor)) / static_cast<double>(maxColor) * 255.0);
			ret = ScColor(c, m, y, k);
		}
	}
	return ret;
}

QString CgmPlug::getBinaryIndexedColor(QDataStream &ts)
{
	QString ret = "Black";
	uint c = getBinaryUInt(ts, colorIndexPrecision);
	if (ColorTableMap.contains(c) && (c <= maxColorIndex)) 
		ret = ColorTableMap[c];
	return ret;
}

QString CgmPlug::getBinaryColor(QDataStream &ts)
{
	QString ret;
	ScColor color;
	if (colorMode == 0)
	{
		ret = getBinaryIndexedColor(ts);
	}
	else
	{
		color = getBinaryDirectColor(ts);
		ret = handleColor(color, "FromCGM"+color.name());
	}
	return ret;
}

double CgmPlug::getBinaryDistance(QDataStream &ts)
{
	double ret = 0.0;
	if (vdcType == 0)				// integer coords
	{
		int x = getBinaryInt(ts, vdcInt);
		ret = x;
	}
	else
		ret = getBinaryReal(ts, vdcReal, vdcMantissa);
	return ret;
}

QPointF CgmPlug::getBinaryCoords(QDataStream &ts, bool raw)
{
	QPointF ret = QPointF(0.0, 0.0);
	if (vdcType == 0)				// integer coords
	{
		int x = getBinaryInt(ts, vdcInt);
		int y = getBinaryInt(ts, vdcInt);
		if (!raw)
		{
			if (vcdFlippedV)
				y = vdcHeight - y;
			if (vcdFlippedH)
				x = vdcWidth - x;
		}
		ret = QPointF(x, y);
	}
	else
	{
		double x = getBinaryReal(ts, vdcReal, vdcMantissa);
		double y = getBinaryReal(ts, vdcReal, vdcMantissa);
		if (!raw)
		{
			if (vcdFlippedV)
				y = vdcHeight - y;
			if (vcdFlippedH)
				x = vdcWidth - x;
		}
		ret = QPointF(x, y);
	}
	return ret;
}

uint CgmPlug::getBinaryUInt(QDataStream &ts, int intP)
{
	uint val = 0;
	if (intP == 8)
	{
		quint8 data;
		ts >> data;
		val = data;
	}
	else if (intP == 16)
	{
		quint16 data;
		ts >> data;
		val = data;
	}
	else if (intP == 24)
	{
		quint8  p1;
		quint16 p2;
		quint32 data = 0;
		ts >> p2;
		ts >> p1;
		data = p2 << 8;
		data |= p1;
		val = data;
	}
	else if (intP == 32)
	{
		quint32 data;
		ts >> data;
		val = data;
	}
	return val;
}

int CgmPlug::getBinaryInt(QDataStream &ts, int intP)
{
	int val = 0;
	if (intP == 8)
	{
		qint8 data;
		ts >> data;
		val = data;
	}
	else if (intP == 16)
	{
		qint16 data;
		ts >> data;
		val = data;
	}
	else if (intP == 24)
	{
		qint8  p1;
		qint16 p2;
		qint32 data = 0;
		ts >> p2;
		ts >> p1;
		data = p2 << 8;
		data |= p1;
		val = data;
	}
	else if (intP == 32)
	{
		qint32 data;
		ts >> data;
		val = data;
	}
	return val;
}

double CgmPlug::getBinaryReal(QDataStream &ts, int realP, int realM)
{
	double val = 0.0;
	if (realP == 0)			// real Format
	{
		if (realM == 9)		// 32bit
		{
#if QT_VERSION >= 0x040600
			ts.setFloatingPointPrecision(QDataStream::SinglePrecision);
#endif
			float data;
			ts >> data;
			val = data;
		}
		else
		{
#if QT_VERSION >= 0x040600
			ts.setFloatingPointPrecision(QDataStream::DoublePrecision);
#endif
			double data;
			ts >> data;
			val = data;
		}
	}
	else							// fixed Point Format
	{
		if (realM == 16)	// 32bit
		{
			quint16 fraction;
			qint16 whole;
			ts >> whole;
			ts >> fraction;
			int gpart = whole;
			val = gpart + (fraction / static_cast<double>(0xFFFF));
/*			quint16 flag = whole & 0x8000;
			whole = whole & 0x7FFF;
			if (flag)
				val = whole - (fraction / static_cast<double>(0xFFFF));
			else
				val = whole + (fraction / static_cast<double>(0xFFFF)); */
		}
		else
		{
			quint32 fraction;
			qint32 whole;
			ts >> whole;
			ts >> fraction;
			int gpart = whole;
			val = gpart + (fraction / static_cast<double>(0xFFFFFFFF));
/*			quint32 flag = whole & 0x80000000;
			whole = whole & 0x7FFFFFFF;
			if (flag)
				val = whole - (fraction / static_cast<double>(0xFFFFFFFF));
			else
				val = whole + (fraction / static_cast<double>(0xFFFFFFFF)); */
		}
	}
	return val;
}

QString CgmPlug::getBinaryText(QDataStream &ts)
{
	quint8 textLen;
	QByteArray text;
	ts >> textLen;
	if (textLen == 0)
		ts >> textLen;
	if (textLen < 255)
	{
		text.resize(textLen);
		ts.readRawData(text.data(), textLen);
	}
	else
	{
		quint16 extTextLen;
		quint16 flag;
		QByteArray textE;
		ts >> extTextLen;
		flag = extTextLen & 0x8000;
		extTextLen = extTextLen & 0x7FFF;
		textE.resize(extTextLen);
		ts.readRawData(textE.data(), extTextLen);
		alignStreamToWord(ts, 0);
		text += textE;
		while (flag)
		{
			ts >> extTextLen;
			flag = extTextLen & 0x8000;
			extTextLen = extTextLen & 0x7FFF;
			textE.resize(extTextLen);
			ts.readRawData(textE.data(), extTextLen);
			text += textE;
		}
	}
	return text;
}

void CgmPlug::alignStreamToWord(QDataStream &ts, uint len)
{
	quint16 flag;
	flag = len & 0x8000;
	quint16 paramLen = len & 0x7FFF;
	ts.skipRawData(paramLen);
	while (flag)
	{
		ts >> paramLen;
		flag = paramLen & 0x8000;
		paramLen = paramLen & 0x7FFF;
		ts.skipRawData(paramLen);
	}
	uint adj = ts.device()->pos() % 2;
	if (adj != 0)
		ts.skipRawData(1);
}
/* End binary Decoder */

/* Start of core Functions common to both Decoders */

void CgmPlug::handleStartMetaFile(QString value)
{
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
		m_Doc->documentInfo().setTitle(value);
	// qDebug() << "Start Metafile" << value;
}

void CgmPlug::handleStartPicture(QString value)
{
	// qDebug() << "Start Picture" << value;
}

void CgmPlug::handleStartPictureBody(double width, double height)
{
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (firstPage)
		{
			m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
			if (width > height)
				m_Doc->setPageOrientation(1);
			else
				m_Doc->setPageOrientation(0);
			m_Doc->setPageSize("Custom");
			m_Doc->changePageMargins(0, 0, 0, 0, height, width, height, width, m_Doc->pageOrientation(), m_Doc->pageSize(), m_Doc->currentPage()->pageNr(), 0);
		}
		else
		{
			if (wasEndPic)
			{
				m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
				m_Doc->addPage(m_Doc->currentPage()->pageNr()+1);
				m_Doc->view()->addPage(m_Doc->currentPage()->pageNr(), true);
			}
		}
		wasEndPic = false;
		firstPage = false;
	}
}

void CgmPlug::handleMetaFileDescription(QString value)
{
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
		m_Doc->documentInfo().setComments(value);
	// qDebug() << "Metafile Description" << value;
}

QString CgmPlug::handleColor(ScColor &color, QString proposedName)
{
	QString tmpName = m_Doc->PageColors.tryAddColor(proposedName, color);
	if (tmpName == proposedName)
		importedColors.append(tmpName);
	return tmpName;
}

double CgmPlug::convertCoords(double input)
{
	return input * metaScale;
}

void CgmPlug::finishItem(PageItem* ite, bool line)
{
	ite->ClipEdited = true;
	ite->FrameType = 3;
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	if (line)
	{
		ite->setLineStyle(lineType);
		ite->setLineEnd(lineCap);
		ite->setLineJoin(lineJoin);
	}
	else
	{
		ite->setLineStyle(edgeType);
		ite->setLineEnd(edgeCap);
		ite->setLineJoin(edgeJoin);
	}
	ite->updateClip();
	Elements.append(ite);
	if (groupStack.count() != 0)
		groupStack.top().append(ite);
	Coords.resize(0);
	Coords.svgInit();
}
