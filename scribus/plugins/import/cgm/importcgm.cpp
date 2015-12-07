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
#include "importcgm.h"
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

ScBitReader::ScBitReader(QByteArray &data)
{
	m_buffer = data;
	m_actBit = 7;
	m_actByte = 0;
}

ScBitReader::~ScBitReader()
{
}

quint32 ScBitReader::getUInt(uint size)
{
	quint32 ret = 0;
	if (size > 32)
		return 0;
	quint8 dat = m_buffer[m_actByte];
	for (uint c = 0; c < size; c++)
	{
		ret = (ret << 1) | ((dat & (0x01 << m_actBit)) >> m_actBit);
		m_actBit--;
		if (m_actBit < 0)
		{
			m_actBit = 7;
			m_actByte++;
			if (m_actByte >= m_buffer.count())
				break;
			dat = m_buffer[m_actByte];
		}
	}
	return ret;
}

void ScBitReader::alignToWord()
{
	if (m_actByte < m_buffer.count() - 1)
	{
		m_actByte++;
		m_actByte += m_actByte % 2;
		m_actBit = 7;
	}
}

CgmPlug::CgmPlug(ScribusDoc* doc, int flags)
{
	m_tmpSel=new Selection(this, false);
	m_Doc=doc;
	m_importerFlags = flags;
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_progressDialog = NULL;
}

QImage CgmPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	m_baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b, h;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
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

bool CgmPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
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
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
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
		{
			PageItem* group = m_Doc->groupObjectsList(m_Elements);
			if (!m_pictName.isEmpty())
				group->setItemName(group->generateUniqueCopyName(m_pictName, false).replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%\\.]"), "_" ));
		}
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

CgmPlug::~CgmPlug()
{
	if (m_progressDialog)
		delete m_progressDialog;
	delete m_tmpSel;
}

void CgmPlug::parseHeader(QString fName, double &b, double &h)
{
}

bool CgmPlug::convert(QString fn)
{
	QString tmp;
	m_Coords.resize(0);
	m_Coords.svgInit();
	m_importedColors.clear();
	QList<PageItem*> gElements;
	m_groupStack.push(gElements);
	m_currentItemNr = 0;
	m_importRunning = true;
	m_firstPage = true;
	m_vcdSet = false;
	m_metaFileVersion = 1;
	m_vdcType = 0;
	m_vdcInt = 16;
	m_vdcReal = 1;
	m_vdcMantissa = 16;
	m_vcdFlippedH = false;
	m_vcdFlippedV = true;
	m_intPrecision = 16;
	m_realPrecision = 1;
	m_realMantissa = 16;
	m_realFraction = 16;
	m_realPrecisionSet = false;
	m_indexPrecision = 16;
	m_colorPrecision = 8;
	m_colorIndexPrecision = 8;
	m_maxColorIndex = 63;
	m_colorModel = 1;
	m_colorMode = 0;
	m_namePrecision = 16;
	m_metaFileScaleMode = 0;
	m_metaFileScale = 1.0;
	m_metaScale = 400.0 / 32768.0;
	m_lineWidthMode = 1;
	m_edgeWidthMode = 1;
	m_markerSizeMode = 1;
	m_viewPortScale = 1.0;
	m_viewPortScaleMode = 0;
	m_lineType = Qt::SolidLine;
	m_lineCap = Qt::FlatCap;
	m_lineJoin = Qt::MiterJoin;
	m_lineWidth = 1.0;
	m_lineColor = "Black";
	m_edgeType = Qt::SolidLine;
	m_edgeCap = Qt::FlatCap;
	m_edgeJoin = Qt::MiterJoin;
	m_edgeWidth = 0.0;
	m_edgeColor = "Black";
	m_fillColor = "Black";
	m_backgroundColor = "White";
	m_patternIndex = 1;
	m_patternTable.clear();
	m_patternScaleX = -1;
	m_patternScaleY = -1;
	m_backgroundSet = false;
	m_fillType = 1;
	m_minColor = 0;
	m_maxColor = 255;
	m_clipRect = QRectF();
	m_useClipRect = true;
	m_clipSet = false;
	m_lineVisible = true;
	m_recordRegion = false;
	m_wasEndPic = false;
	m_recordFigure = false;
	m_fontID_Map.clear();
	m_fontIndex = 1;
	m_textSize = 12;
	m_textColor = "Black";
	m_textAlignH = 0;
	m_textScaleMode = 1;
	m_currentRegion = 0;
	m_pictName = "";
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
		ts.setByteOrder(QDataStream::BigEndian);
		quint32 magic;
		ts >> magic;
		ts.device()->seek(0);
		if (magic == 0x4265674D)
			decodeText(f);
		else
		{
			while (!ts.atEnd() && (m_importRunning))
			{
				quint16 data, elemClass, elemID, paramLen;
				ts >> data;
				elemClass = (data & 0xF000) >> 12;
				elemID    = (data & 0x0FE0) >>  5;
				paramLen  = data & 0x001F;
				if (paramLen == 31)
					ts >> paramLen;
			//	qDebug() << "CGM Command Class" << elemClass << "ID" << elemID << "ParamLen" << paramLen;
				decodeBinary(ts, elemClass, elemID, paramLen);
				if (m_progressDialog)
				{
					m_progressDialog->setProgress("GI", ts.device()->pos());
					qApp->processEvents();
				}
			}
		}
		f.close();
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
		else
		{
			if (m_backgroundSet)
			{
				m_tmpSel->clear();
				m_tmpSel->delaySignalsOn();
				for (int dre=0; dre<m_Elements.count(); ++dre)
				{
					m_tmpSel->addItem(m_Elements.at(dre), true);
				}
				m_tmpSel->setGroupRect();
				double gx, gy, gw, gh;
				m_tmpSel->getVisualGroupRect(&gx, &gy, &gw, &gh);
				m_tmpSel->clear();
				m_tmpSel->delaySignalsOff();
				int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_backgroundColor, CommonStrings::None, true);
				PageItem *ite = m_Doc->Items->takeAt(z);
				m_Elements.prepend(ite);
				m_Doc->Items->insert(m_oldDocItemCount, ite);
			}
		}
	}
	if (m_progressDialog)
		m_progressDialog->close();
	return true;
}

void CgmPlug::decodeText(QFile &f)
{
	qDebug() << "Parsing Text Data not supported yet";
}

/* Start binary Decoder */
void CgmPlug::decodeBinary(QDataStream &ts, quint16 elemClass, quint16 elemID, quint16 paramLen)
{
	qint64 pos = ts.device()->pos();
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
		m_importRunning = false;
		qDebug() << "Class" << elemClass << "ID" << elemID << "Len" << paramLen << "at" << ts.device()->pos();
	}
	ts.device()->seek(pos);
	alignStreamToWord(ts, paramLen);
	alignStreamToWord(ts, 0);
}

void CgmPlug::decodeClass0(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 0)
	{
		qDebug() << "NO OP";
	}
	else if (elemID == 1)
		handleStartMetaFile(getBinaryText(ts));
	else if (elemID == 2)
	{
		m_importRunning = false;
		// qDebug() << "END METAFILE";
	}
	else if (elemID == 3)
		handleStartPicture(getBinaryText(ts));
	else if (elemID == 4)
	{
		if (m_vcdSet)
		{
			double w = m_vdcWidth * m_metaScale;
			double h = m_vdcHeight * m_metaScale;
			handleStartPictureBody(w, h);
		}
		else
		{
			handleStartPictureBody(m_docWidth, m_docHeight);
			m_firstPage = true;
		}
	//	qDebug() << "BEGIN PICTURE BODY";
	}
	else if (elemID == 5)
	{
		if (m_vcdSet)
		{
			if (m_firstPage)
			{
				double w = m_vdcWidth * m_metaScale;
				double h = m_vdcHeight * m_metaScale;
				handleStartPictureBody(w, h);
			}
		}
		else
		{
			if (m_firstPage)
				handleStartPictureBody(m_docWidth, m_docHeight);
		}
		m_wasEndPic = true;
	//	qDebug() << "END PICTURE";
	}
	else if (elemID == 6)
	{
		qDebug() << "BEGIN SEGMENT";
	}
	else if (elemID == 7)
	{
		qDebug() << "END SEGMENT";
	}
	else if (elemID == 8)
	{
		m_recordFigure = true;
		m_figurePath = QPainterPath();
		m_figClose = false;
		m_figDocIndex = m_Doc->Items->count();
		m_figElemIndex = m_Elements.count();
		m_figGstIndex = 0;
		m_figFillColor = m_fillColor;
		if (m_groupStack.count() != 0)
			m_figGstIndex = m_groupStack.top().count();
	//	qDebug() << "BEGIN FIGURE";
	}
	else if (elemID == 9)
	{
		m_recordFigure = false;
		if (!m_figurePath.isEmpty())
		{
			m_figurePath.closeSubpath();
			m_Coords.fromQPainterPath(m_figurePath);
			PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, 0, m_figFillColor, CommonStrings::None);
			ite->PoLine = m_Coords.copy();
			ite->ClipEdited = true;
			ite->FrameType = 3;
			FPoint wh = getMaxClipF(&ite->PoLine);
			ite->setWidthHeight(wh.x(),wh.y());
			ite->setTextFlowMode(PageItem::TextFlowDisabled);
			m_Doc->AdjustItemSize(ite);
			ite->OldB2 = ite->width();
			ite->OldH2 = ite->height();
			ite->updateClip();
			m_Doc->Items->takeLast();
			m_Doc->Items->insert(m_figDocIndex, ite);
			m_Elements.insert(m_figElemIndex, ite);
			if (m_groupStack.count() != 0)
				m_groupStack.top().insert(m_figGstIndex, ite);
		}
		m_figurePath = QPainterPath();
	//	qDebug() << "END FIGURE";
	}
	else if (elemID == 13)
	{
		uint type = getBinaryUInt(ts, m_indexPrecision);
		m_currentRegion = type;
		m_recordRegion = true;
		m_regionPath = QPainterPath();
		// qDebug() << "BEGIN PROTECTION REGION" << type;
	}
	else if (elemID == 14)
	{
		m_recordRegion = false;
		m_regionMap.insert(m_currentRegion, m_regionPath);
		// qDebug() << "END PROTECTION REGION";
	}
	else if (elemID == 15)
	{
		qDebug() << "BEGIN COMPOUND LINE";
	}
	else if (elemID == 16)
	{
		qDebug() << "END COMPOUND LINE";
	}
	else if (elemID == 17)
	{
		qDebug() << "BEGIN COMPOUND TEXT PATH";
	}
	else if (elemID == 18)
	{
		qDebug() << "END COMPOUND TEXT PATH";
	}
	else if (elemID == 19)
	{
		qDebug() << "BEGIN TILE ARRAY";
	}
	else if (elemID == 20)
	{
		qDebug() << "END TILE ARRAY";
	}
	else if (elemID == 21)
	{
		qDebug() << "BEGIN APPLICATION STRUCTURE";
	}
	else if (elemID == 22)
	{
		qDebug() << "BEGIN APPLICATION STRUCTURE BODY";
	}
	else if (elemID == 23)
	{
		qDebug() << "END APPLICATION STRUCTURE";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 0 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass1(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	quint16 data;
	if (elemID == 1)
	{
		ts >> data;
		m_metaFileVersion = data;
		//qDebug() << "METAFILE VERSION" << data;
	}
	else if (elemID == 2)
		handleMetaFileDescription(getBinaryText(ts));
	else if (elemID == 3)
	{
		ts >> data;
		m_vdcType = data;
		//qDebug() << "VDC TYPE" << data;
	}
	else if (elemID == 4)
	{
		ts >> data;
		m_intPrecision = data;
	//	qDebug() << "INTEGER PRECISION" << data;
	}
	else if (elemID == 5)
	{
		ts >> data;
		m_realPrecision = data;
		ts >> data;
		m_realMantissa = data;
		ts >> data;
		m_realFraction = data;
		if (m_realPrecision == 0)
			m_realPrecisionSet = true;
	//	qDebug() << "REAL PRECISION" << realPrecision << realMantissa << realFraction;
	}
	else if (elemID == 6)
	{
		ts >> data;
		m_indexPrecision = data;
	//	qDebug() << "INDEX PRECISION" << indexPrecision;
	}
	else if (elemID == 7)
	{
		ts >> data;
		m_colorPrecision = data;
	//	qDebug() << "COLOUR PRECISION" << colorPrecision;
	}
	else if (elemID == 8)
	{
		ts >> data;
		m_colorIndexPrecision = data;
		//qDebug() << "COLOUR INDEX PRECISION" << colorIndexPrecision;
	}
	else if (elemID == 9)
	{
		ts >> data;
		m_maxColorIndex = data;
		// qDebug() << "MAXIMUM COLOUR INDEX" << maxColorIndex;
	}
	else if (elemID == 10)
	{
		if (m_colorModel == 1)		// RGB
		{
			if (m_colorPrecision == 8)
			{
				quint8 r, g, b;
				ts >> r >> g >> b;
				m_minColor = r;
				ts >> r >> g >> b;
				m_maxColor = r;
			}
			else if (m_colorPrecision == 16)
			{
				quint16 r, g, b;
				ts >> r >> g >> b;
				m_minColor = r;
				ts >> r >> g >> b;
				m_maxColor = r;
			}
		}
		else if (m_colorModel == 4)	// CMYK
		{
			if (m_colorPrecision == 8)
			{
				quint8 c, m, y, k;
				ts >> c >> m >> y >> k;
				m_minColor = c;
				ts >> c >> m >> y >> k;
				m_maxColor = c;
			}
			else if (m_colorPrecision == 16)
			{
				quint16 c, m, y, k;
				ts >> c >> m >> y >> k;
				m_minColor = c;
				ts >> c >> m >> y >> k;
				m_maxColor = c;
			}
		}
		// qDebug() << "COLOUR VALUE EXTENT" << minColor << maxColor;
	}
	else if (elemID == 11)
	{
	//	qDebug() << "METAFILE ELEMENT LIST";
	}
	else if (elemID == 12)
	{
		qDebug() << "METAFILE DEFAULTS REPLACEMENT" << paramLen;
	/*	quint16 data, elemClass, elemID, paramLenN;
		ts >> data;
		elemClass = (data & 0xF000) >> 12;
		elemID    = (data & 0x0FE0) >>  5;
		paramLenN  = data & 0x001F;
		if (paramLenN == 31)
			ts >> paramLenN;
		qDebug() << "CGM Command Class" << elemClass << "ID" << elemID << "ParamLen" << paramLenN;
		decodeBinary(ts, elemClass, elemID, paramLenN);*/
	}
	else if (elemID == 13)
	{
		quint16 bytesRead = 0;
		int fontID = 1;
		while (bytesRead < paramLen)
		{
			int posA = ts.device()->pos();
			QString p = getBinaryText(ts);
			int posN = ts.device()->pos();
			bytesRead += posN - posA;
			m_fontID_Map.insert(fontID, p);
		}
	//	qDebug() << "FONT LIST" << fontID_Map;
	}
	else if (elemID == 14)
	{
		qDebug() << "CHARACTER SET LIST";
	}
	else if (elemID == 15)
	{
		qDebug() << "CHARACTER CODING ANNOUNCER";
	}
	else if (elemID == 16)
	{
		ts >> data;
		m_namePrecision = data;
 		// qDebug() << "NAME PRECISION" << namePrecision;
	}
	else if (elemID == 17)
	{
		QPointF max, min;
		max = getBinaryCoords(ts);
		min = getBinaryCoords(ts);
	//	qDebug() << "MAXIMUM VDC EXTENT" << min.x() << min.y() << max.x() << max.y();
	}
	else if (elemID == 18)
	{
		qDebug() << "SEGMENT PRIORITY EXTENT";
	}
	else if (elemID == 19)
	{
		ts >> data;
		m_colorModel = data;
	//	qDebug() << "COLOUR MODEL" << colorModel;
	}
	else if (elemID == 20)
	{
		qDebug() << "COLOUR CALIBRATION";
	}
	else if (elemID == 21)
	{
		qDebug() << "FONT PROPERTIES";
	}
	else if (elemID == 22)
	{
		qDebug() << "GLYPH MAPPING";
	}
	else if (elemID == 23)
	{
		qDebug() << "SYMBOL LIBRARY LIST";
	}
	else if (elemID == 24)
	{
		qDebug() << "PICTURE DIRECTORY";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 1 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass2(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	quint16 data;
	if (elemID == 1)
	{
		ts >> data;
		m_metaFileScaleMode = data;
		double sc = 1.0;
		if (m_realPrecisionSet)
			sc = getBinaryReal(ts, 0, m_realMantissa);
		else
			sc = getBinaryReal(ts, 0, 9);
		if (m_metaFileScaleMode != 0)
			m_metaFileScale = sc;
	//	qDebug() << "SCALING MODE" << metaFileScaleMode << metaFileScale;
	}
	else if (elemID == 2)
	{
		ts >> data;
		m_colorMode = data;
//		qDebug() << "COLOUR SELECTION MODE" << colorMode;
	}
	else if (elemID == 3)
	{
		ts >> data;
		m_lineWidthMode = data;
		if (m_lineWidthMode == 0)
			m_lineWidth = 0; // qMax(vdcWidth, vdcHeight) / 1000;
		else if (m_lineWidthMode == 1)
			m_lineWidth = 1.0;
		else if (m_lineWidthMode == 2)
			m_lineWidth = 0.001;
		else if (m_lineWidthMode == 3)
			m_lineWidth = 0.35;
	//	qDebug() << "LINE WIDTH SPECIFICATION MODE" << lineWidthMode;
	}
	else if (elemID == 4)
	{
		ts >> data;
		m_markerSizeMode = data;
		// qDebug() << "MARKER SIZE SPECIFICATION MODE" << markerSizeMode;
	}
	else if (elemID == 5)
	{
		ts >> data;
		m_edgeWidthMode = data;
		if (m_edgeWidthMode == 0)
			m_edgeWidth = 0; // qMax(vdcWidth, vdcHeight) / 1000;
		else if (m_edgeWidthMode == 1)
			m_edgeWidth = 1.0;
		else if (m_edgeWidthMode == 2)
			m_edgeWidth = 0.001;
		else if (m_edgeWidthMode == 3)
			m_edgeWidth = 0.35;
	//	qDebug() << "EDGE WIDTH SPECIFICATION MODE" << edgeWidthMode;
	}
	else if (elemID == 6)
	{
		QPointF max, min;
		max = getBinaryCoords(ts, true);
		min = getBinaryCoords(ts, true);
		QRectF vd = QRectF(max, min);
		m_vcdFlippedV = (vd.height() > 0);
		m_vcdFlippedH = (vd.width() < 0);
		vd = vd.normalized();
		m_vdcWidth = vd.width();
		m_vdcHeight = vd.height();
		m_metaScale = 400.0 / qMax(m_vdcWidth, m_vdcHeight);
		if (m_lineWidthMode == 0)
			m_lineWidth = 0; // qMax(vdcWidth, vdcHeight) / 1000;
		else if (m_lineWidthMode == 1)
			m_lineWidth = 1.0;
		else if (m_lineWidthMode == 2)
			m_lineWidth = 0.001;
		else if (m_lineWidthMode == 3)
			m_lineWidth = 0.35;
		m_baseX = -vd.left() * m_metaScale;
		m_baseY = vd.top() * m_metaScale;
		m_vcdSet = true;
		if (!m_clipSet)
		{
			m_clipRect = QRectF(vd.left() * m_metaScale, vd.top() * m_metaScale, m_vdcWidth * m_metaScale, m_vdcHeight * m_metaScale);
			m_clipSet = true;
		}
	//	qDebug() << "VDC EXTENT" << vd.left() << vd.top() << vdcWidth << vdcHeight << vcdFlippedV;
	}
	else if (elemID == 7)
	{
		ScColor color = getBinaryDirectColor(ts);
		m_backgroundColor = handleColor(color, "FromCGM"+color.name());
		if (m_colorMode == 1)
			m_backgroundSet = true;
		else
			m_ColorTableMap.insert(0, m_backgroundColor);
//		qDebug() << "BACKGROUND COLOUR" << backgroundColor;
	}
	else if (elemID == 8)
	{
		QPointF max, min;
		max = getBinaryCoords(ts);
		min = getBinaryCoords(ts);
	//	qDebug() << "DEVICE VIEWPORT" << min.x() << min.y() << max.x() << max.y();
	}
	else if (elemID == 9)
	{
		ts >> data;
		m_viewPortScaleMode = data;
		if (m_realPrecisionSet)
			m_viewPortScale = getBinaryReal(ts, 0, m_realMantissa);
		else
			m_viewPortScale = getBinaryReal(ts, 0, 9);
		// qDebug() << "DEVICE VIEWPORT SPECIFICATION MODE" << viewPortScaleMode << viewPortScale;
	}
	else if (elemID == 10)
	{
		qDebug() << "DEVICE VIEWPORT MAPPING";
	}
	else if (elemID == 11)
	{
		qDebug() << "LINE REPRESENTATION";
	}
	else if (elemID == 12)
	{
		qDebug() << "MARKER REPRESENTATION";
	}
	else if (elemID == 13)
	{
		qDebug() << "TEXT REPRESENTATION";
	}
	else if (elemID == 14)
	{
		qDebug() << "FILL REPRESENTATION";
	}
	else if (elemID == 15)
	{
		qDebug() << "EDGE REPRESENTATION";
	}
	else if (elemID == 16)
	{
		qDebug() << "INTERIOR STYLE SPECIFICATION MODE";
	}
	else if (elemID == 17)
	{
		qDebug() << "LINE AND EDGE TYPE DEFINITION";
	}
	else if (elemID == 18)
	{
		qDebug() << "HATCH STYLE DEFINITION";
	}
	else if (elemID == 19)
	{
		qDebug() << "GEOMETRIC PATTERN DEFINITION";
	}
	else if (elemID == 20)
	{
		qDebug() << "APPLICATION STRUCTURE DIRECTORY";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 2 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass3(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	quint16 data;
	if (elemID == 1)
	{
		ts >> data;
		m_vdcInt = data;
 		// qDebug() << "VDC INTEGER PRECISION" << vdcInt;
	}
	else if (elemID == 2)
	{
		ts >> data;
		m_vdcReal = data;
		ts >> data;
		m_vdcMantissa = data;
		ts >> data;
 		// qDebug() << "VDC REAL PRECISION" << vdcReal << vdcMantissa;
	}
	else if (elemID == 3)
	{
		qDebug() << "AUXILIARY COLOUR";
	}
	else if (elemID == 4)
	{
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
		m_clipRect = QRectF(x, y, w, h);
		m_clipSet = true;
	//	qDebug() << "CLIP RECTANGLE" << clipRect;
	}
	else if (elemID == 6)
	{
		ts >> data;
		if (data == 0)
			m_useClipRect = false;
		else
			m_useClipRect = true;
		// qDebug() << "CLIP INDICATOR" << useClipRect;
	}
	else if (elemID == 7)
	{
		qDebug() << "LINE CLIPPING MODE";
	}
	else if (elemID == 8)
	{
		qDebug() << "MARKER CLIPPING MODE";
	}
	else if (elemID == 9)
	{
		qDebug() << "EDGE CLIPPING MODE";
	}
	else if (elemID == 10)
	{
		if (m_recordRegion)
			m_regionPath.closeSubpath();
		if (m_recordFigure)
			m_figurePath.closeSubpath();
		m_figClose = true;
	//	qDebug() << "NEW REGION";
	}
	else if (elemID == 11)
	{
		qDebug() << "SAVE PRIMITIVE CONTEXT";
	}
	else if (elemID == 12)
	{
		qDebug() << "RESTORE PRIMITIVE CONTEXT";
	}
	else if (elemID == 17)
	{
		uint index = getBinaryUInt(ts, m_indexPrecision);
		uint type = getBinaryUInt(ts, m_indexPrecision);
		if (type == 1)
		{
			if (m_groupStack.count() != 0)
			{
				QList<PageItem*> gElements = m_groupStack.pop();
				m_tmpSel->clear();
				if (gElements.count() > 0)
				{
					for (int dre = 0; dre < gElements.count(); ++dre)
					{
						m_tmpSel->addItem(gElements.at(dre), true);
						m_Elements.removeAll(gElements.at(dre));
					}
					PageItem *ite = m_Doc->itemSelection_GroupObjects(false, false, m_tmpSel);
					QPainterPath clip = m_regionMap[index];
					if (!clip.isEmpty())
					{
						ite->PoLine.fromQPainterPath(clip);
						ite->PoLine.translate(-ite->xPos(), -ite->yPos());
						ite->PoLine.translate(m_baseX, m_baseY);
					}
					m_tmpSel->clear();
					m_tmpSel->addItem(ite, true);
					m_Elements.append(ite);
				}
				if (m_groupStack.count() != 0)
				{
					for (int as = 0; as < m_tmpSel->count(); ++as)
					{
						m_groupStack.top().append(m_tmpSel->itemAt(as));
					}
				}
				m_tmpSel->clear();
			}
		}
		else if ((type == 2) || (type == 3))
		{
			QList<PageItem*> gElements;
			m_groupStack.push(gElements);
		}
		// qDebug() << "PROTECTION REGION INDICATOR" << index << type;
	}
	else if (elemID == 18)
	{
		qDebug() << "GENERALIZED TEXT PATH MODE";
	}
	else if (elemID == 19)
	{
		qDebug() << "MITRE LIMIT";
	}
	else if (elemID == 20)
	{
		qDebug() << "TRANSPARENT CELL COLOUR";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 3 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass4(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		getBinaryPath(ts, paramLen);
		if (m_Coords.size() > 3)
		{
			m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (m_recordRegion)
				m_regionPath.connectPath(m_Coords.toQPainterPath(false));
			else
			{
				if (m_recordFigure)
				{
					if (m_figClose)
					{
						QPainterPath ell = m_Coords.toQPainterPath(false);
						appendPath(m_figurePath, ell);
						m_figClose = false;
					}
					else
						m_figurePath.connectPath(m_Coords.toQPainterPath(false));
				}
				int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_lineWidth, CommonStrings::None, m_lineColor, true);
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = m_Coords.copy();
				finishItem(ite);
			}
		}
		// qDebug() << "POLYLINE";
	}
	else if (elemID == 2)
	{
		getBinaryPath(ts, paramLen, true);
		if (m_Coords.size() > 3)
		{
			m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (m_recordRegion)
				m_regionPath.connectPath(m_Coords.toQPainterPath(false));
			else
			{
				if (m_recordFigure)
				{
					if (m_figClose)
					{
						QPainterPath ell = m_Coords.toQPainterPath(false);
						appendPath(m_figurePath, ell);
						m_figClose = false;
					}
					else
						m_figurePath.connectPath(m_Coords.toQPainterPath(false));
				}
				int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_lineWidth, CommonStrings::None, m_lineColor, true);
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = m_Coords.copy();
				finishItem(ite);
			}
		}
		// qDebug() << "DISJOINT POLYLINE";
	}
	else if (elemID == 3)
	{
		qDebug() << "POLYMARKER";
	}
	else if (elemID == 4)
	{
		QPointF center = getBinaryCoords(ts);
		double txX = convertCoords(center.x());
		double txY = convertCoords(center.y());
		quint16 flag;
		ts >> flag;
		QString txt = getBinaryText(ts);
		QPainterPath ell;
		ell.addText(0, 0, QFont(m_fontID_Map[m_fontIndex], m_textSize), txt);
		ell.translate(txX, txY);
		if (m_textAlignH == 2)
			ell.translate(-ell.boundingRect().width() / 2.0, 0);
		else if (m_textAlignH == 3)
			ell.translate(-ell.boundingRect().width(), 0);
		ell.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		if (m_recordRegion)
			m_regionPath.addPath(ell);
		else
		{
			if (m_recordFigure)
				m_figurePath.addPath(ell);
			m_Coords.fromQPainterPath(ell, true);
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, 0, m_textColor, CommonStrings::None, true);
			PageItem *ite = m_Doc->Items->at(z);
			ite->PoLine = m_Coords.copy();
			finishItem(ite, false);
		}
	//	qDebug() << "TEXT Len" << textAlignH;
	}
	else if (elemID == 5)
	{
		double sx = convertCoords(getBinaryDistance(ts));
		double sy = convertCoords(getBinaryDistance(ts));
		QPointF center = getBinaryCoords(ts);
		double txX = convertCoords(center.x());
		double txY = convertCoords(center.y());
		quint16 flag;
		ts >> flag;
		QString txt = getBinaryText(ts);
		QPainterPath ell;
		ell.addText(0, 0, QFont(m_fontID_Map[m_fontIndex], m_textSize), txt);
		double scx = sx / ell.boundingRect().width();
		double scy = sy / ell.boundingRect().height();
		if ((m_textScaleMode > 1) || ((ell.boundingRect().width() > sx) || (ell.boundingRect().height() > sy)))
		{
			QTransform mm;
			mm.scale(scx, scy);
			ell = mm.map(ell);
		}
		ell.translate(txX, txY);
		if (m_textAlignH == 2)
			ell.translate(-ell.boundingRect().width() / 2.0, 0);
		else if (m_textAlignH == 3)
			ell.translate(-ell.boundingRect().width(), 0);
		ell.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		if (m_recordRegion)
			m_regionPath.addPath(ell);
		else
		{
			if (m_recordFigure)
				m_figurePath.addPath(ell);
			m_Coords.fromQPainterPath(ell, true);
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, 0, m_textColor, CommonStrings::None, true);
			PageItem *ite = m_Doc->Items->at(z);
			ite->PoLine = m_Coords.copy();
			finishItem(ite, false);
		}
	//	qDebug() << "RESTRICTED TEXT";
	}
	else if (elemID == 6)
	{
		qDebug() << "APPEND TEXT";
	}
	else if (elemID == 7)
	{
		getBinaryPath(ts, paramLen);
		if (m_Coords.size() > 3)
		{
			m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (m_recordRegion)
				m_regionPath.addPath(m_Coords.toQPainterPath(true));
			else
			{
				if (m_recordFigure)
					m_figurePath.addPath(m_Coords.toQPainterPath(true));
				PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_edgeWidth, m_fillColor, m_edgeColor);
				ite->PoLine = m_Coords.copy();
				finishItem(ite, false);
			}
		}
		// qDebug() << "POLYGON";
	}
	else if (elemID == 8)
	{
	//	qDebug() << "POLYGON SET" << "Fill Type" << fillType;
		quint16 bytesRead = 0;
		bool first = true;
		m_Coords.resize(0);
		m_Coords.svgInit();
		quint16 flag;
		paramLen = paramLen & 0x7FFF;
		QPainterPath polySetPath;
		QPointF startPoint;
		while (bytesRead < paramLen)
		{
			int posA = ts.device()->pos();
			QPointF p = getBinaryCoords(ts);
			ts >> flag;
			int posN = ts.device()->pos();
			bytesRead += posN - posA;
			if (first)
			{
				polySetPath.moveTo(convertCoords(p.x()), convertCoords(p.y()));
				startPoint = p;
				first = false;
			}
			else
				polySetPath.lineTo(convertCoords(p.x()), convertCoords(p.y()));
			if ((flag == 2) || (flag == 3))
			{
				polySetPath.lineTo(convertCoords(startPoint.x()), convertCoords(startPoint.y()));
				polySetPath.closeSubpath();
				first = true;
			}
		}
		polySetPath.lineTo(convertCoords(startPoint.x()), convertCoords(startPoint.y()));
		polySetPath.closeSubpath();
		if (m_recordFigure)
		{
			polySetPath.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			m_figurePath.addPath(polySetPath);
		}
		else
		{
		//	qDebug() << "POLYGON SET" << "Fill Color" << fillColor;
			m_Coords.fromQPainterPath(polySetPath, true);
			PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_edgeWidth, m_fillColor, m_edgeColor);
			ite->PoLine = m_Coords.copy();
			ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(ite, false);
		}
	}
	else if (elemID == 9)
	{
		int pos = ts.device()->pos();
		QPointF p, q, r;
		int nx, ny;
		quint16 mode;
		p = convertCoords(getBinaryCoords(ts));
		q = convertCoords(getBinaryCoords(ts));
		r = convertCoords(getBinaryCoords(ts));
		nx = getBinaryUInt(ts, m_intPrecision);
		ny = getBinaryUInt(ts, m_intPrecision);
		int t_colorPrecision = m_colorPrecision;
		int t_colorIndexPrecision = m_colorIndexPrecision;
		m_colorPrecision = getBinaryUInt(ts, m_intPrecision);
		m_colorIndexPrecision = m_colorPrecision;
//		qDebug() << "CELL ARRAY at" << pos << "Size" << nx << ny << "Compression" << mode << "Color Prec" << colorPrecision;
		if (m_colorPrecision == 0)
		{
			m_colorPrecision = t_colorPrecision;
			m_colorIndexPrecision = t_colorIndexPrecision;
		}
		ts >> mode;
		int bytesRead = ts.device()->pos() - pos;
		QLineF pr = QLineF(p, r);
		QLineF rq = QLineF(r, q);
		double originX = p.x();
		double originY = p.y();
		bool flipX = false;
		bool flipY = false;
		if (p.x() > r.x())
		{
			flipX = true;
			originX = r.x();
		}
		if (p.y() > q.y())
		{
			flipY = true;
			originY = q.y();
		}
		int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, m_baseX + originX, m_baseY + originY, pr.length(), rq.length(), m_edgeWidth, CommonStrings::None, CommonStrings::None, true);
		PageItem *ite = m_Doc->Items->at(z);
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite, false);
		QImage image = QImage(nx, ny, QImage::Format_ARGB32);
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
		if (m_colorPrecision < 8)
		{
			ScBitReader *breader = new ScBitReader(imageData);
			for (int yy = 0; yy < ny; yy++)
			{
				ScColor color;
				QRgb *s = (QRgb*)(image.scanLine(yy));
				if (mode == 1)
				{
					for (int xx = 0; xx < nx; xx++)
					{
						if (m_colorMode == 0)
							color = m_Doc->PageColors[getBinaryIndexedColor(breader)];
						else
							color = getBinaryDirectColor(breader);
						QColor co = color.getRawRGBColor();
						*s++ = qRgba(co.red(), co.green(), co.blue(), 255);
					}
				}
				else
				{
					int xx = 0;
					while (xx < nx)
					{
						int counter = breader->getUInt(m_intPrecision);
						if ((counter > nx) || (counter == 0))
						{
							m_importRunning = false;
							return;
						}
						if (m_colorMode == 0)
							color = m_Doc->PageColors[getBinaryIndexedColor(breader)];
						else
							color = getBinaryDirectColor(breader);
						QColor co = color.getRawRGBColor();
						for (int xc = 0; xc < counter; xc++)
						{
							*s++ = qRgba(co.red(), co.green(), co.blue(), 255);
							xx++;
							if (xx >= nx)
								break;
						}
					}
				}
				breader->alignToWord();
			}
		}
		else
		{
			QDataStream istr(imageData);
			istr.setByteOrder(QDataStream::BigEndian);
			for (int yy = 0; yy < ny; yy++)
			{
				ScColor color;
				QRgb *s = (QRgb*)(image.scanLine(yy));
				if (mode == 1)
				{
					for (int xx = 0; xx < nx; xx++)
					{
						if (m_colorMode == 0)
							color = m_Doc->PageColors[getBinaryIndexedColor(istr)];
						else
							color = getBinaryDirectColor(istr);
						QColor co = color.getRawRGBColor();
						*s++ = qRgba(co.red(), co.green(), co.blue(), 255);
					}
				}
				else
				{
					int xx = 0;
					while (xx < nx)
					{
						int counter = getBinaryUInt(istr, m_intPrecision);
						if ((counter > nx) || (counter == 0))
						{
							m_importRunning = false;
							return;
						}
						if (m_colorMode == 0)
							color = m_Doc->PageColors[getBinaryIndexedColor(istr)];
						else
							color = getBinaryDirectColor(istr);
						QColor co = color.getRawRGBColor();
						for (int xc = 0; xc < counter; xc++)
						{
							*s++ = qRgba(co.red(), co.green(), co.blue(), 255);
							xx++;
							if (xx >= nx)
								break;
						}
					}
				}
				uint adj = istr.device()->pos() % 2;
				if (adj != 0)
					istr.skipRawData(1);
			}
		}
		QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_cgm_XXXXXX.png");
		tempFile->setAutoRemove(false);
		tempFile->open();
		QString fileName = getLongPathName(tempFile->fileName());
		tempFile->close();
		ite->isInlineImage = true;
		ite->isTempFile = true;
		image.save(fileName, "PNG");
		if ((image.width() < 20) || image.height() < 20)
			ite->pixm.imgInfo.lowResType = 0;
		m_Doc->loadPict(fileName, ite);
		delete tempFile;
		ite->setImageFlippedH(flipX);
		ite->setImageFlippedV(flipY);
		ite->setImageScalingMode(false, false);
		ite->AdjustPictScale();
		m_colorPrecision = t_colorPrecision;
		m_colorIndexPrecision = t_colorIndexPrecision;
	}
	else if (elemID == 10)
	{
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
		if (m_recordRegion)
			m_regionPath.addRect(QRectF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset(), w, h));
		else
		{
			if (m_recordFigure)
				m_figurePath.addRect(QRectF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset(), w, h));
			PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Rectangle, m_baseX + x, m_baseY + y, w, h, m_edgeWidth, m_fillColor, m_edgeColor);
			ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(ite, false);
		}
	//	qDebug() << "RECTANGLE";
	}
	else if (elemID == 12)
	{
		QPointF max = getBinaryCoords(ts);
		double x = convertCoords(max.x());
		double y = convertCoords(max.y());
		double r = convertCoords(getBinaryDistance(ts));
		x = x - r;
		y = y - r;
		if (m_recordRegion)
			m_regionPath.addEllipse(QPointF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset()), r * 2.0, r * 2.0);
		else
		{
			if (m_recordFigure)
				m_figurePath.addEllipse(QRectF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset(), r * 2.0, r * 2.0));
			PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Ellipse, m_baseX + x, m_baseY + y, r * 2.0, r * 2.0, m_edgeWidth, m_fillColor, m_edgeColor);
			ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(ite, false);
		}
	//	qDebug() << "CIRCLE";
	}
	else if (elemID == 13)
	{
		QPointF pStart = convertCoords(getBinaryCoords(ts));
		QPointF pInter = convertCoords(getBinaryCoords(ts));
		QPointF pEnd = convertCoords(getBinaryCoords(ts));
		QLineF s_e = QLineF(pStart, pEnd);
		QLineF n_s = s_e.normalVector();
		n_s.translate(s_e.pointAt(0.5) - s_e.p1());
		QLineF s_i = QLineF(pStart, pInter);
		QLineF n_i = s_i.normalVector();
		n_i.translate(s_i.pointAt(0.5) - s_i.p1());
		QPointF center;
		if (n_s.intersect(n_i, &center) != QLineF::NoIntersection)
		{
			QLineF rad1 = QLineF(center, pStart);
			QLineF rad3 = QLineF(center, pInter);
			double radius = rad1.length();
			m_Coords.resize(0);
			m_Coords.svgInit();
			m_Coords.svgMoveTo(pStart.x(), pStart.y());
			m_Coords.svgArcTo(radius, radius, 0, 0, rad1.angle() < rad3.angle() ? 0 : 1, pInter.x(), pInter.y());
			m_Coords.svgArcTo(radius, radius, 0, 0, rad1.angle() < rad3.angle() ? 0 : 1, pEnd.x(), pEnd.y());
			m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (m_recordRegion)
				m_regionPath.connectPath(m_Coords.toQPainterPath(false));
			else
			{
				if (m_recordFigure)
				{
					if (m_figClose)
					{
						QPainterPath ell = m_Coords.toQPainterPath(false);
						appendPath(m_figurePath, ell);
						m_figClose = false;
					}
					else
						m_figurePath.connectPath(m_Coords.toQPainterPath(false));
				}
				int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_lineWidth, CommonStrings::None, m_lineColor, true);
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = m_Coords.copy();
				finishItem(ite);
			}
		}
	//	qDebug() << "CIRCULAR ARC 3 POINT";
	}
	else if (elemID == 14)
	{
		QPointF pStart = convertCoords(getBinaryCoords(ts));
		QPointF pInter = convertCoords(getBinaryCoords(ts));
		QPointF pEnd = convertCoords(getBinaryCoords(ts));
		quint16 mode;
		ts >> mode;
		QLineF s_e = QLineF(pStart, pEnd);
		QLineF n_s = s_e.normalVector();
		n_s.translate(s_e.pointAt(0.5) - s_e.p1());
		QLineF s_i = QLineF(pStart, pInter);
		QLineF n_i = s_i.normalVector();
		n_i.translate(s_i.pointAt(0.5) - s_i.p1());
		QPointF center;
		if (n_s.intersect(n_i, &center) != QLineF::NoIntersection)
		{
			QLineF rad1 = QLineF(center, pStart);
			QLineF rad3 = QLineF(center, pInter);
			double radius = rad1.length();
			m_Coords.resize(0);
			m_Coords.svgInit();
			if (mode == 0)
			{
				m_Coords.svgMoveTo(center.x(), center.y());
				m_Coords.svgLineTo(pStart.x(), pStart.y());
			}
			else
				m_Coords.svgMoveTo(pStart.x(), pStart.y());
			m_Coords.svgArcTo(radius, radius, 0, 0, rad1.angle() < rad3.angle() ? 0 : 1, pInter.x(), pInter.y());
			m_Coords.svgArcTo(radius, radius, 0, 0, rad1.angle() < rad3.angle() ? 0 : 1, pEnd.x(), pEnd.y());
			if (mode == 0)
				m_Coords.svgLineTo(center.x(), center.y());
			else
				m_Coords.svgLineTo(pStart.x(), pStart.y());
			m_Coords.svgClosePath();
			m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (m_recordRegion)
				m_regionPath.addPath(m_Coords.toQPainterPath(false));
			else
			{
				if (m_recordFigure)
					m_figurePath.addPath(m_Coords.toQPainterPath(false));
				PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_edgeWidth, m_fillColor, m_edgeColor);
				ite->PoLine = m_Coords.copy();
				finishItem(ite, false);
			}
		}
	//	qDebug() << "CIRCULAR ARC 3 POINT CLOSE";
	}
	else if (elemID == 15)
	{
		QPointF center = getBinaryCoords(ts);
		double sx = convertCoords(getBinaryDistance(ts));
		double sy = convertCoords(getBinaryDistance(ts));
		double ex = convertCoords(getBinaryDistance(ts));
		double ey = convertCoords(getBinaryDistance(ts));
		double r = convertCoords(getBinaryDistance(ts));
		double cx = convertCoords(center.x()) + m_Doc->currentPage()->xOffset();
		double cy = convertCoords(center.y()) + m_Doc->currentPage()->yOffset();
		if (m_vcdFlippedV)
		{
			sy *= -1;
			ey *= -1;
		}
		if (m_vcdFlippedH)
		{
			sx *= -1;
			ex *= -1;
		}
		QLineF stv = QLineF(cx, cy, cx + sx, cy + sy);
		QLineF env = QLineF(cx, cy, cx + ex, cy + ey);
		QPainterPath ell;
		if (qFuzzyCompare(sx, ex) && qFuzzyCompare(sy, ey))
		{
			ell.addEllipse(QPointF(cx, cy), r, r);
		}
		else
		{
			stv.setLength(r);
			ell.moveTo(stv.p2().x(), stv.p2().y());
			ell.arcTo(cx - r, cy - r, r * 2.0, r * 2.0, stv.angle(), stv.angleTo(env));
		}
		if (m_recordRegion)
			m_regionPath.connectPath(ell);
		else
		{
			if (m_recordFigure)
			{
				if (m_figClose)
				{
					appendPath(m_figurePath, ell);
					m_figClose = false;
				}
				else
					m_figurePath.connectPath(ell);
			}
			m_Coords.fromQPainterPath(ell, false);
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_lineWidth, CommonStrings::None, m_lineColor, true);
			PageItem *ite = m_Doc->Items->at(z);
			ite->PoLine = m_Coords.copy();
			finishItem(ite);
		}
	//	qDebug() << "CIRCULAR ARC CENTRE";
	}
	else if (elemID == 16)
	{
		quint16 mode;
		QPointF center = getBinaryCoords(ts);
		double sx = convertCoords(getBinaryDistance(ts));
		double sy = convertCoords(getBinaryDistance(ts));
		double ex = convertCoords(getBinaryDistance(ts));
		double ey = convertCoords(getBinaryDistance(ts));
		double r = convertCoords(getBinaryDistance(ts));
		ts >> mode;
		double cx = convertCoords(center.x()) + m_Doc->currentPage()->xOffset();
		double cy = convertCoords(center.y()) + m_Doc->currentPage()->yOffset();
		if (m_vcdFlippedV)
		{
			sy *= -1;
			ey *= -1;
		}
		if (m_vcdFlippedH)
		{
			sx *= -1;
			ex *= -1;
		}
		QLineF stv = QLineF(cx, cy, cx + sx, cy + sy);
		QLineF env = QLineF(cx, cy, cx + ex, cy + ey);
		QPainterPath ell;
		if (qFuzzyCompare(sx, ex) && qFuzzyCompare(sy, ey))
		{
			ell.addEllipse(QPointF(cx, cy), r, r);
		}
		else
		{
			stv.setLength(r);
			if (mode == 0)
			{
				ell.moveTo(cx, cy);
				ell.arcTo(cx - r, cy - r, r * 2.0, r * 2.0, stv.angle(), stv.angleTo(env));
				ell.lineTo(cx, cy);
				ell.closeSubpath();
			}
			else
			{
				ell.moveTo(stv.p2().x(), stv.p2().y());
				ell.arcTo(cx - r, cy - r, r * 2.0, r * 2.0, stv.angle(), stv.angleTo(env));
				ell.lineTo(stv.p2().x(), stv.p2().y());
				ell.closeSubpath();
			}
		}
		if (m_recordRegion)
			m_regionPath.addPath(ell);
		else
		{
			if (m_recordFigure)
				m_figurePath.addPath(ell);
			m_Coords.fromQPainterPath(ell, true);
			PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_edgeWidth, m_fillColor, m_edgeColor);
			ite->PoLine = m_Coords.copy();
			finishItem(ite, false);
		}
	//	qDebug() << "CIRCULAR ARC CENTRE CLOSE";
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
		ell.addEllipse(QPointF(0, 0), distX, distY);
		QTransform mm;
		mm.rotate(rotB);
		ell = mm.map(ell);
		ell.translate(cx, cy);
		if (m_recordRegion)
		{
			ell.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			m_regionPath.addPath(ell);
		}
		else
		{
			ell.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (m_recordFigure)
				m_figurePath.addPath(ell);
			m_Coords.fromQPainterPath(ell, true);
			PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_edgeWidth, m_fillColor, m_edgeColor);
			ite->PoLine = m_Coords.copy();
			finishItem(ite, false);
		}
	//	qDebug() << "ELLIPSE";
	}
	else if (elemID == 18)
	{
		QPointF center = getBinaryCoords(ts);
		double cx = convertCoords(center.x());
		double cy = convertCoords(center.y());
		QPointF r1 = getBinaryCoords(ts);
		double r1x = convertCoords(r1.x());
		double r1y = convertCoords(r1.y());
		QPointF r2 = getBinaryCoords(ts);
		double r2x = convertCoords(r2.x());
		double r2y = convertCoords(r2.y());
		QLineF dstX = QLineF(cx, cy, r1x, r1y);
		QLineF dstY = QLineF(cx, cy, r2x, r2y);
		double distX = dstX.length();
		double distY = dstY.length();
		double rotB = dstX.angle();
		double sx = convertCoords(getBinaryDistance(ts));
		double sy = convertCoords(getBinaryDistance(ts));
		double ex = convertCoords(getBinaryDistance(ts));
		double ey = convertCoords(getBinaryDistance(ts));
		if (m_vcdFlippedV)
		{
			sy *= -1;
			ey *= -1;
		}
		if (m_vcdFlippedH)
		{
			sx *= -1;
			ex *= -1;
		}
		QLineF stv = QLineF(cx, cy, cx + sx, cy + sy);
		QLineF env = QLineF(cx, cy, cx + ex, cy + ey);
		QPainterPath ell;
		ell.addEllipse(QPointF(cx, cy), distX, distY);
		ell.translate(-cx, -cy);
		QTransform mm;
		mm.rotate(rotB);
		ell = mm.map(ell);
		ell.translate(cx, cy);
		QPolygonF elPo = ell.toFillPolygon();
		QPointF stP = stv.p2();
		QPointF enP = env.p2();
		if (qFuzzyCompare(sx, ex) && qFuzzyCompare(sy, ey))
		{
			ell.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			m_Coords.fromQPainterPath(ell);
		}
		else
		{
			for (int a = 0; a < elPo.size() - 1; a++)
			{
				QPointF intersect;
				if (QLineF(elPo[a], elPo[a+1]).intersect(stv, &intersect) == QLineF::BoundedIntersection)
				{
					stP = intersect;
					break;
				}
			}
			for (int a = 0; a < elPo.size() - 1; a++)
			{
				QPointF intersect;
				if (QLineF(elPo[a], elPo[a+1]).intersect(env, &intersect) == QLineF::BoundedIntersection)
				{
					enP = intersect;
					break;
				}
			}
			m_Coords.resize(0);
			m_Coords.svgInit();
			if (dstX.angleTo(dstY) > 180)
			{
				m_Coords.svgMoveTo(stP.x(), stP.y());
				m_Coords.svgArcTo(distX, distY, rotB, stv.angleTo(env) < 180 ? 1 : 0, dstX.angleTo(dstY) > 180 ? 1 : 0, enP.x(), enP.y());
			}
			else
			{
				m_Coords.svgMoveTo(stP.x(), stP.y());
				m_Coords.svgArcTo(distX, distY, rotB, stv.angleTo(env) > 180 ? 1 : 0, dstX.angleTo(dstY) > 180 ? 1 : 0, enP.x(), enP.y());
			}
			m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			ell = m_Coords.toQPainterPath(false);
		}
		if (m_recordRegion)
			m_regionPath.connectPath(ell);
		else
		{
			if (m_recordFigure)
			{
				if (m_figClose)
				{
					appendPath(m_figurePath, ell);
					m_figClose = false;
				}
				else
					m_figurePath.connectPath(ell);
			}
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_lineWidth, CommonStrings::None, m_lineColor, true);
			PageItem *ite = m_Doc->Items->at(z);
			ite->PoLine = m_Coords.copy();
			finishItem(ite);
		}
	//	qDebug() << "ELLIPTICAL ARC";
	}
	else if (elemID == 19)
	{
		quint16 mode;
		QPointF center = getBinaryCoords(ts);
		double cx = convertCoords(center.x());
		double cy = convertCoords(center.y());
		QPointF r1 = getBinaryCoords(ts);
		double r1x = convertCoords(r1.x());
		double r1y = convertCoords(r1.y());
		QPointF r2 = getBinaryCoords(ts);
		double r2x = convertCoords(r2.x());
		double r2y = convertCoords(r2.y());
		QLineF dstX = QLineF(cx, cy, r1x, r1y);
		QLineF dstY = QLineF(cx, cy, r2x, r2y);
		double distX = dstX.length();
		double distY = dstY.length();
		double rotB = dstX.angle();
		double sx = convertCoords(getBinaryDistance(ts));
		double sy = convertCoords(getBinaryDistance(ts));
		double ex = convertCoords(getBinaryDistance(ts));
		double ey = convertCoords(getBinaryDistance(ts));
		ts >> mode;
		if (m_vcdFlippedV)
		{
			sy *= -1;
			ey *= -1;
		}
		if (m_vcdFlippedH)
		{
			sx *= -1;
			ex *= -1;
		}
		QLineF stv = QLineF(cx, cy, cx + sx, cy + sy);
		QLineF env = QLineF(cx, cy, cx + ex, cy + ey);
		QPainterPath ell;
		ell.addEllipse(QPointF(cx, cy), distX, distY);
		ell.translate(-cx, -cy);
		QTransform mm;
		mm.rotate(rotB);
		ell = mm.map(ell);
		ell.translate(cx, cy);
		QPolygonF elPo = ell.toFillPolygon();
		QPointF stP = stv.p2();
		for (int a = 0; a < elPo.size() - 1; a++)
		{
			QPointF intersect;
			if (QLineF(elPo[a], elPo[a+1]).intersect(stv, &intersect) == QLineF::BoundedIntersection)
			{
				stP = intersect;
				break;
			}
		}
		QPointF enP = env.p2();
		for (int a = 0; a < elPo.size() - 1; a++)
		{
			QPointF intersect;
			if (QLineF(elPo[a], elPo[a+1]).intersect(env, &intersect) == QLineF::BoundedIntersection)
			{
				enP = intersect;
				break;
			}
		}
		m_Coords.resize(0);
		m_Coords.svgInit();
		if (mode == 0)
		{
			m_Coords.svgMoveTo(cx, cy);
			if (dstX.angleTo(dstY) > 180)
			{
				m_Coords.svgLineTo(stP.x(), stP.y());
				m_Coords.svgArcTo(distX, distY, rotB, stv.angleTo(env) < 180 ? 1 : 0, dstX.angleTo(dstY) > 180 ? 1 : 0, enP.x(), enP.y());
			}
			else
			{
				m_Coords.svgLineTo(stP.x(), stP.y());
				m_Coords.svgArcTo(distX, distY, rotB, stv.angleTo(env) > 180 ? 1 : 0, dstX.angleTo(dstY) > 180 ? 1 : 0, enP.x(), enP.y());
			}
			m_Coords.svgLineTo(cx, cy);
			m_Coords.svgClosePath();
		}
		else
		{
			if (dstX.angleTo(dstY) > 180)
			{
				m_Coords.svgMoveTo(stP.x(), stP.y());
				m_Coords.svgArcTo(distX, distY, rotB, stv.angleTo(env) < 180 ? 1 : 0, dstX.angleTo(dstY) > 180 ? 1 : 0, enP.x(), enP.y());
			}
			else
			{
				m_Coords.svgMoveTo(stP.x(), stP.y());
				m_Coords.svgArcTo(distX, distY, rotB, stv.angleTo(env) > 180 ? 1 : 0, dstX.angleTo(dstY) > 180 ? 1 : 0, enP.x(), enP.y());
			}
			m_Coords.svgLineTo(stP.x(), stP.y());
			m_Coords.svgClosePath();
		}
		m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		if (m_recordRegion)
			m_regionPath.addPath(m_Coords.toQPainterPath(false));
		else
		{
			if (m_recordFigure)
				m_figurePath.addPath(m_Coords.toQPainterPath(false));
			PageItem *ite = itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_edgeWidth, m_fillColor, m_edgeColor);
			ite->PoLine = m_Coords.copy();
			finishItem(ite, false);
		}
	//	qDebug() << "ELLIPTICAL ARC CLOSE";
	}
	else if (elemID == 20)
	{
		qDebug() << "CIRCULAR ARC CENTRE REVERSED";
	}
	else if (elemID == 21)
	{
		qDebug() << "CONNECTING EDGE";
	}
	else if (elemID == 22)
	{
		qDebug() << "HYPERBOLIC ARC";
	}
	else if (elemID == 23)
	{
		qDebug() << "PARABOLIC ARC";
	}
	else if (elemID == 24)
	{
		qDebug() << "NON-UNIFORM B-SPLINE";
	}
	else if (elemID == 25)
	{
		qDebug() << "NON-UNIFORM RATIONAL B-SPLINE";
	}
	else if (elemID == 26)
	{
		getBinaryBezierPath(ts, paramLen);
		if (m_Coords.size() > 3)
		{
			m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			if (m_recordRegion)
				m_regionPath.addPath(m_Coords.toQPainterPath(false));
			else
			{
				if (m_recordFigure)
				{
					if (m_figClose)
					{
						QPainterPath ell = m_Coords.toQPainterPath(false);
						appendPath(m_figurePath, ell);
						m_figClose = false;
					}
					else
						m_figurePath.connectPath(m_Coords.toQPainterPath(false));
				}
				int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_lineWidth, CommonStrings::None, m_lineColor, true);
				PageItem *ite = m_Doc->Items->at(z);
				ite->PoLine = m_Coords.copy();
				finishItem(ite);
			}
		}
		// qDebug() << "POLYBEZIER";
	}
	else if (elemID == 27)
	{
		qDebug() << "POLYSYMBOL";
	}
	else if (elemID == 28)
	{
		uint comp = getBinaryUInt(ts, m_indexPrecision);
		uint pad = getBinaryUInt(ts, m_intPrecision);
		QString backColor = getBinaryColor(ts);
		QString foreColor = getBinaryColor(ts);
		uint prec = getBinaryUInt(ts, m_intPrecision);
		qDebug() << "BITONAL TILE  Compression" << comp << "Padding" << pad << "Background" << backColor << "Foreground" << foreColor << "Precision" << prec;
	}
	else if (elemID == 29)
	{
		qDebug() << "TILE";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 4 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass5(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		m_lineBundleIndex = getBinaryUInt(ts, m_indexPrecision);
 		// qDebug() << "LINE BUNDLE INDEX" << lineBundleIndex;
	}
	else if (elemID == 2)
	{
		uint type = getBinaryUInt(ts, m_indexPrecision);
		if (type == 1)
			m_lineType = Qt::SolidLine;
		else if (type == 2)
			m_lineType = Qt::DashLine;
		else if (type == 3)
			m_lineType = Qt::DotLine;
		else if (type == 4)
			m_lineType = Qt::DashDotLine;
		else if (type == 5)
			m_lineType = Qt::DashDotDotLine;
		else
			m_lineType = Qt::SolidLine;
 		// qDebug() << "LINE TYPE" << lineType;
	}
	else if (elemID == 3)
	{
		m_lineWidth = getBinaryDistance(ts);
		if (m_lineWidthMode == 0)
			m_lineWidth *= m_metaScale;
		else if (m_lineWidthMode == 1)
			m_lineWidth *= 1.0;
		else if (m_lineWidthMode == 2)
			m_lineWidth *= 0.001;
		else if (m_lineWidthMode == 3)
			m_lineWidth *= 0.35;
	//	if (lineWidth < 1)
	//		lineWidth = 0;
 		// qDebug() << "LINE WIDTH" << lineWidth;
	}
	else if (elemID == 4)
	{
		m_lineColor = getBinaryColor(ts);
		// qDebug() << "LINE COLOUR" << lineColor;
	}
	else if (elemID == 5)
	{
 		qDebug() << "MARKER BUNDLE INDEX";
	}
	else if (elemID == 6)
	{
 		qDebug() << "MARKER TYPE";
	}
	else if (elemID == 7)
	{
 		qDebug() << "MARKER SIZE";
	}
	else if (elemID == 8)
	{
 		qDebug() << "MARKER COLOUR";
	}
	else if (elemID == 9)
	{
		qDebug() << "TEXT BUNDLE INDEX";
	}
	else if (elemID == 10)
	{
		m_fontIndex = getBinaryUInt(ts, m_indexPrecision);
	//	qDebug() << "TEXT FONT INDEX" << m_fontIndex;
	}
	else if (elemID == 11)
	{
		qDebug() << "TEXT PRECISION";
	}
	else if (elemID == 12)
	{
		qDebug() << "CHARACTER EXPANSION FACTOR";
	}
	else if (elemID == 13)
	{
		qDebug() << "CHARACTER SPACING";
	}
	else if (elemID == 14)
	{
		m_textColor = getBinaryColor(ts);
	//	qDebug() << "TEXT COLOUR" << textColor;
	}
	else if (elemID == 15)
	{
		m_textSize = getBinaryDistance(ts);
		m_textSize *= m_metaScale;
	//	qDebug() << "CHARACTER HEIGHT" << textSize;
	}
	else if (elemID == 16)
	{
		qDebug() << "CHARACTER ORIENTATION";
	}
	else if (elemID == 17)
	{
		qDebug() << "TEXT PATH";
	}
	else if (elemID == 18)
	{
		quint16 hFlag;
		ts >> hFlag;
		m_textAlignH = hFlag;
	//	qDebug() << "TEXT ALIGNMENT" << hFlag;
	}
	else if (elemID == 19)
	{
		qDebug() << "CHARACTER SET INDEX";
	}
	else if (elemID == 20)
	{
		qDebug() << "ALTERNATE CHARACTER SET INDEX";
	}
	else if (elemID == 21)
	{
 		qDebug() << "FILL BUNDLE INDEX";
	}
	else if (elemID == 22)
	{
		quint16 data;
		ts >> data;
		m_fillType = data;
	//	qDebug() << "INTERIOR STYLE" << fillType;
	}
	else if (elemID == 23)
	{
		m_fillColor = getBinaryColor(ts);
	//	qDebug() << "Fill COLOUR" << fillColor;
	}
	else if (elemID == 24)
	{
 		qDebug() << "HATCH INDEX";
	}
	else if (elemID == 25)
	{
		m_patternIndex = getBinaryUInt(ts, m_indexPrecision);
		// Hack to fix some broken(?) CGM files
		//fillType = 2;
	//	qDebug() << "PATTERN INDEX" << patternIndex;
	}
	else if (elemID == 26)
	{
 		qDebug() << "EDGE BUNDLE INDEX";
	}
	else if (elemID == 27)
	{
		uint type = getBinaryUInt(ts, m_indexPrecision);
		if (type == 1)
			m_edgeType = Qt::SolidLine;
		else if (type == 2)
			m_edgeType = Qt::DashLine;
		else if (type == 3)
			m_edgeType = Qt::DotLine;
		else if (type == 4)
			m_edgeType = Qt::DashDotLine;
		else if (type == 5)
			m_edgeType = Qt::DashDotDotLine;
		else
			m_edgeType = Qt::SolidLine;
 		// qDebug() << "EDGE TYPE";
	}
	else if (elemID == 28)
	{
		m_edgeWidth = getBinaryDistance(ts);
		if (m_edgeWidthMode == 0)
			m_edgeWidth *= m_metaScale;
		else if (m_edgeWidthMode == 1)
			m_edgeWidth *= 1.0;
		else if (m_edgeWidthMode == 2)
			m_edgeWidth *= 0.001;
		else if (m_edgeWidthMode == 3)
			m_edgeWidth *= 0.35;
	//	if (edgeWidth < 1)
	//		edgeWidth = 0;
 		// qDebug() << "EDGE WIDTH" << edgeWidth;
	}
	else if (elemID == 29)
	{
		m_edgeColor = getBinaryColor(ts);
		// qDebug() << "EDGE COLOUR" << edgeColor;
	}
	else if (elemID == 30)
	{
		quint16 data;
		ts >> data;
		if (data == 0)
			m_lineVisible = false;
		else
			m_lineVisible = true;
 		// qDebug() << "EDGE VISIBILITY";
	}
	else if (elemID == 31)
	{
		QPointF p = getBinaryCoords(ts);
		double x = convertCoords(p.x());
		double y = convertCoords(p.y());
		m_fillRefPoint = QPointF(x + m_Doc->currentPage()->xOffset(), y + m_Doc->currentPage()->yOffset());
	//	qDebug() << "FILL REFERENCE POINT" << fillRefPoint;
	}
	else if (elemID == 32)
	{
		uint index = getBinaryUInt(ts, m_indexPrecision);
		uint nx = getBinaryUInt(ts, m_intPrecision);
		uint ny = getBinaryUInt(ts, m_intPrecision);
		int t_colorPrecision = m_colorPrecision;
		m_colorPrecision = getBinaryUInt(ts, m_intPrecision);
		QImage tmpImg = QImage(nx, ny, QImage::Format_ARGB32);
		for (uint a = 0; a < ny; a++)
		{
			QRgb *s = (QRgb*)tmpImg.scanLine(a);
			for (uint b = 0; b < nx; b++)
			{
				ScColor color;
				if (m_colorMode == 0)
					color = m_Doc->PageColors[getBinaryIndexedColor(ts)];
				else
					color = getBinaryDirectColor(ts);
				QColor co = color.getRawRGBColor();
				*s = qRgba(co.red(), co.green(), co.blue(), 255);
				s++;
			}
		}
		int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, 0, 0, tmpImg.width(), tmpImg.height(), 0, CommonStrings::None, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->SetRectFrame();
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->AdjustItemSize(ite);
		ite->OldB2 = ite->width();
		ite->OldH2 = ite->height();
		QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_cgm_XXXXXX.png");
		tempFile->setAutoRemove(false);
		if (tempFile->open())
		{
			QString fileName = getLongPathName(tempFile->fileName());
			if (!fileName.isEmpty())
			{
				tempFile->close();
				ite->isInlineImage = true;
				ite->isTempFile = true;
				tmpImg.save(fileName, "PNG");
				m_Doc->loadPict(fileName, ite);
				ite->setImageScalingMode(false, true);
				ScPattern pat = ScPattern();
				pat.setDoc(m_Doc);
				pat.pattern = tmpImg;
				pat.xoffset = 0;
				pat.yoffset = 0;
				pat.width = ite->width();
				pat.height = ite->height();
				ite->gXpos = 0;
				ite->gYpos = 0;
				ite->setXYPos(ite->gXpos, ite->gYpos, true);
				pat.items.append(ite);
				m_Doc->Items->removeAll(ite);
				QString id = QString("Pattern_from_CGM_%1").arg(m_Doc->docPatterns.count() + 1);
				m_Doc->addPattern(id, pat);
				m_patternTable.insert(index, id);
			}
			else
			{
				m_Doc->Items->removeAll(ite);
				delete ite;
			}
		}
		else
		{
			m_Doc->Items->removeAll(ite);
			delete ite;
		}
		delete tempFile;
		m_colorPrecision = t_colorPrecision;
	//	qDebug() << "PATTERN TABLE" << "Index" << index << "NX" << nx << "NY" << ny;
	}
	else if (elemID == 33)
	{
		double phx = convertCoords(getBinaryDistance(ts));
		double phy = convertCoords(getBinaryDistance(ts));
		double pwx = convertCoords(getBinaryDistance(ts));
		double pwy = convertCoords(getBinaryDistance(ts));
		QLineF hp = QLineF(0, 0, phx, phy);
		QLineF wp = QLineF(0, 0, pwx, pwy);
		m_patternScaleX = wp.length();
		m_patternScaleY = hp.length();
	//	qDebug() << "PATTERN SIZE" << wp.length() << hp.length();
	}
	else if (elemID == 34)
	{
//		qDebug() << "COLOUR TABLE" << "Starting at" << ts.device()->pos();
		getBinaryColorTable(ts, paramLen);
	}
	else if (elemID == 35)
	{
		qDebug() << "ASPECT SOURCE FLAGS";
	}
	else if (elemID == 36)
	{
		qDebug() << "PICK IDENTIFIER";
	}
	else if (elemID == 37)
	{
		uint type = getBinaryUInt(ts, m_indexPrecision);
		if (type == 1)
			m_lineCap = Qt::FlatCap;
		else if (type == 2)
			m_lineCap = Qt::RoundCap;
		else if (type == 3)
			m_lineCap = Qt::SquareCap;
		else
			m_lineCap = Qt::FlatCap;
		type = getBinaryUInt(ts, m_indexPrecision);		// dummy reading unsupported parameter
		// qDebug() << "LINE CAP";
	}
	else if (elemID == 38)
	{
		uint type = getBinaryUInt(ts, m_indexPrecision);
		if (type == 1)
			m_lineJoin = Qt::MiterJoin;
		else if (type == 2)
			m_lineJoin = Qt::RoundJoin;
		else if (type == 3)
			m_lineJoin = Qt::BevelJoin;
		else
			m_lineJoin = Qt::MiterJoin;
		// qDebug() << "LINE JOIN";
	}
	else if (elemID == 39)
	{
		qDebug() << "LINE TYPE CONTINUATION";
	}
	else if (elemID == 40)
	{
		qDebug() << "LINE TYPE INITIAL OFFSET";
	}
	else if (elemID == 41)
	{
		qDebug() << "TEXT SCORE TYPE";
	}
	else if (elemID == 42)
	{
		m_textScaleMode = getBinaryUInt(ts, m_indexPrecision);
		qDebug() << "RESTRICTED TEXT TYPE" << m_textScaleMode;
	}
	else if (elemID == 43)
	{
		int posI = ts.device()->pos();
		uint type = getBinaryUInt(ts, m_indexPrecision);
		QPointF p1 = convertCoords(getBinaryCoords(ts));
		QPointF p2 = convertCoords(getBinaryCoords(ts));
		uint index = getBinaryUInt(ts, m_intPrecision);
	//	qDebug() << "INTERPOLATED INTERIOR  Type" << type << "from" << p1 << "to" << p2 << "Stages" << index << "at" << posI << realPrecision << realMantissa;
		for (uint s = 0; s < index; s++)
		{
			double s1 = getBinaryReal(ts, m_realPrecision, m_realMantissa);
			qDebug() << "Stages " << s1;
		}
		Q_UNUSED(posI);
		Q_UNUSED(type);
		Q_UNUSED(p1);
		Q_UNUSED(p2);
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
	//	qDebug() << "INTERPOLATED INTERIOR  Type" << type << "from" << p1 << "to" << p2 << "Stages" << index << "at" << posI;
	}
	else if (elemID == 44)
	{
		uint type = getBinaryUInt(ts, m_indexPrecision);
		if (type == 1)
			m_edgeCap = Qt::FlatCap;
		else if (type == 2)
			m_edgeCap = Qt::RoundCap;
		else if (type == 3)
			m_edgeCap = Qt::SquareCap;
		else
			m_edgeCap = Qt::FlatCap;
		type = getBinaryUInt(ts, m_indexPrecision);		// dummy reading unsupported parameter
 		// qDebug() << "EDGE CAP";
	}
	else if (elemID == 45)
	{
		uint type = getBinaryUInt(ts, m_indexPrecision);
		if (type == 1)
			m_edgeJoin = Qt::MiterJoin;
		else if (type == 2)
			m_edgeJoin = Qt::RoundJoin;
		else if (type == 3)
			m_edgeJoin = Qt::BevelJoin;
		else
			m_edgeJoin = Qt::MiterJoin;
		// qDebug() << "EDGE JOIN";
	}
	else if (elemID == 46)
	{
		qDebug() << "EDGE TYPE CONTINUATION";
	}
	else if (elemID == 47)
	{
		qDebug() << "EDGE TYPE INITIAL OFFSET";
	}
	else if (elemID == 48)
	{
		qDebug() << "SYMBOL LIBRARY INDEX";
	}
	else if (elemID == 49)
	{
		qDebug() << "SYMBOL COLOUR";
	}
	else if (elemID == 50)
	{
		qDebug() << "SYMBOL SIZE";
	}
	else if (elemID == 51)
	{
		qDebug() << "SYMBOL ORIENTATION";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 5 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass6(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		qDebug() << "ESCAPE";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 6 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass7(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		qDebug() << "MESSAGE";
	}
	else if (elemID == 2)
	{
		qDebug() << "APPLICATION DATA" << paramLen << "at" << ts.device()->pos();
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 7 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass8(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		qDebug() << "COPY SEGMENT";
	}
	else if (elemID == 2)
	{
		qDebug() << "INHERITANCE FILTER";
	}
	else if (elemID == 3)
	{
		qDebug() << "CLIP INHERITANCE";
	}
	else if (elemID == 4)
	{
		qDebug() << "SEGMENT TRANSFORMATION";
	}
	else if (elemID == 5)
	{
		qDebug() << "SEGMENT HIGHLIGHTING";
	}
	else if (elemID == 6)
	{
		qDebug() << "SEGMENT DISPLAY PRIORITY";
	}
	else if (elemID == 7)
	{
		qDebug() << "SEGMENT PICK PRIORITY";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 8 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::decodeClass9(QDataStream &ts, quint16 elemID, quint16 paramLen)
{
	if (elemID == 1)
	{
		qDebug() << "APPLICATION STRUCTURE ATTRIBUTE";
	}
	else
	{
		m_importRunning = false;
		qDebug() << "Class 9 ID" << elemID << "Len" << paramLen;
	}
}

void CgmPlug::getBinaryBezierPath(QDataStream &ts, quint16 paramLen)
{
	quint16 bytesRead = 0;
	bool first = true;
	m_Coords.resize(0);
	m_Coords.svgInit();
	quint16 flag;
	flag = paramLen & 0x8000;
	paramLen = paramLen & 0x7FFF;
	uint type = getBinaryUInt(ts, m_indexPrecision);
	while (bytesRead < paramLen - 2)
	{
		int posA = ts.device()->pos();
		if ((first) || (type == 1))
		{
			QPointF p = getBinaryCoords(ts);
			m_Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
			first = false;
		}
		QPointF p1 = getBinaryCoords(ts);
		QPointF p2 = getBinaryCoords(ts);
		QPointF p3 = getBinaryCoords(ts);
		m_Coords.svgCurveToCubic(convertCoords(p1.x()), convertCoords(p1.y()), convertCoords(p2.x()), convertCoords(p2.y()), convertCoords(p3.x()), convertCoords(p3.y()));
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
				m_Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
			}
			QPointF p1 = getBinaryCoords(ts);
			QPointF p2 = getBinaryCoords(ts);
			QPointF p3 = getBinaryCoords(ts);
			m_Coords.svgCurveToCubic(convertCoords(p1.x()), convertCoords(p1.y()), convertCoords(p2.x()), convertCoords(p2.y()), convertCoords(p3.x()), convertCoords(p3.y()));
			int posN = ts.device()->pos();
			bytesRead += posN - posA;
		}
	}
}

void CgmPlug::getBinaryPath(QDataStream &ts, quint16 paramLen, bool disjoint)
{
	quint16 bytesRead = 0;
	bool first = true;
	m_Coords.resize(0);
	m_Coords.svgInit();
	quint16 flag;
	flag = paramLen & 0x8000;
	paramLen = paramLen & 0x7FFF;
	while (bytesRead < paramLen)
	{
		int posA = ts.device()->pos();
		QPointF p = getBinaryCoords(ts);
		if (first)
		{
			m_Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
			first = false;
		}
		else
		{
			m_Coords.svgLineTo(convertCoords(p.x()), convertCoords(p.y()));
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
					m_Coords.svgMoveTo(convertCoords(p.x()), convertCoords(p.y()));
					first = false;
				}
				else
				{
					m_Coords.svgLineTo(convertCoords(p.x()), convertCoords(p.y()));
					if (disjoint)
						first = true;
				}
			}
			else
			{
				QPointF p = getBinaryCoords(ts);
				m_Coords.svgLineTo(convertCoords(p.x()), convertCoords(p.y()));
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
	uint c = getBinaryUInt(ts, m_colorIndexPrecision);
	int posN = ts.device()->pos();
	bytesRead += posN - posA;
	QString tmpName = CommonStrings::None;
	while (bytesRead < paramLen)
	{
		posA = ts.device()->pos();
		ScColor cc = getBinaryDirectColor(ts);
		tmpName = handleColor(cc, "FromCGM"+cc.name());
		m_ColorTableMap.insert(c, tmpName);
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
			//ScColor cc = getBinaryDirectColor(ts);
			m_ColorTableMap.insert(c, tmpName);
			c++;
			posN = ts.device()->pos();
			bytesRead += posN - posA;
		}
	}
}

ScColor CgmPlug::getBinaryDirectColor(ScBitReader *breader)
{
	ScColor ret;
	if (m_colorModel == 1)		// RGB
	{
		int r = breader->getUInt(m_colorPrecision);
		int g = breader->getUInt(m_colorPrecision);
		int b = breader->getUInt(m_colorPrecision);
		r = qRound(r * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
		g = qRound(g * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
		b = qRound(b * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
		ret = ScColor(r, g, b);
	}
	else if (m_colorModel == 4)	// CMYK
	{
		uint c = breader->getUInt(m_colorPrecision);
		uint m = breader->getUInt(m_colorPrecision);
		uint y = breader->getUInt(m_colorPrecision);
		uint k = breader->getUInt(m_colorPrecision);
		c = qRound(c * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
		m = qRound(m * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
		y = qRound(y * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
		k = qRound(k * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
		ret = ScColor(c, m, y, k);
	}
	return ret;
}

ScColor CgmPlug::getBinaryDirectColor(QDataStream &ts)
{
	ScColor ret;
	if (m_colorModel == 1)		// RGB
	{
		if (m_colorPrecision == 8)
		{
			quint8 ri, gi, bi;
			ts >> ri >> gi >> bi;
			int r = ri;
			int g = gi;
			int b = bi;
			r = qRound(r * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
			g = qRound(g * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
			b = qRound(b * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
			ret = ScColor(r, g, b);
		}
		else if (m_colorPrecision == 16)
		{
			quint16 ri, gi, bi;
			ts >> ri >> gi >> bi;
			int r = ri;
			int g = gi;
			int b = bi;
			r = qRound((r * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			g = qRound((g * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			b = qRound((b * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			ret = ScColor(r, g, b);
		}
		else if (m_colorPrecision == 24)
		{
			quint8  p1;
			quint16 p2;
			quint32 ri = 0;
			quint32 gi = 0;
			quint32 bi = 0;
			ts >> p2;
			ts >> p1;
			ri = p2 << 8;
			ri |= p1;
			ts >> p2;
			ts >> p1;
			gi = p2 << 8;
			gi |= p1;
			ts >> p2;
			ts >> p1;
			bi = p2 << 8;
			bi |= p1;
			int r = ri;
			int g = gi;
			int b = bi;
			r = qRound((r * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			g = qRound((g * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			b = qRound((b * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			ret = ScColor(r, g, b);
		}
		else if (m_colorPrecision == 32)
		{
			quint32 ri, gi, bi;
			ts >> ri >> gi >> bi;
			int r = ri;
			int g = gi;
			int b = bi;
			r = qRound((r * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			g = qRound((g * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			b = qRound((b * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			ret = ScColor(r, g, b);
		}
	}
	else if (m_colorModel == 4)	// CMYK
	{
		if (m_colorPrecision == 8)
		{
			quint8 ci, mi, yi, ki;
			ts >> ci >> mi >> yi >> ki;
			uint c = ci;
			uint m = mi;
			uint y = yi;
			uint k = ki;
			c = qRound(c * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
			m = qRound(m * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
			y = qRound(y * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
			k = qRound(k * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor));
			ret = ScColor(c, m, y, k);
		}
		else if (m_colorPrecision == 16)
		{
			quint16 ci, mi, yi, ki;
			ts >> ci >> mi >> yi >> ki;
			uint c = ci;
			uint m = mi;
			uint y = yi;
			uint k = ki;
			c = qRound((c * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			m = qRound((m * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			y = qRound((y * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			k = qRound((k * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			ret = ScColor(c, m, y, k);
		}
		else if (m_colorPrecision == 24)
		{
			quint8  p1;
			quint16 p2;
			quint32 ci = 0;
			ts >> p2;
			ts >> p1;
			ci = p2 << 8;
			ci |= p1;
			quint32 mi = 0;
			ts >> p2;
			ts >> p1;
			mi = p2 << 8;
			mi |= p1;
			quint32 yi = 0;
			ts >> p2;
			ts >> p1;
			yi = p2 << 8;
			yi |= p1;
			quint32 ki = 0;
			ts >> p2;
			ts >> p1;
			ki = p2 << 8;
			ki |= p1;
			uint c = ci;
			uint m = mi;
			uint y = yi;
			uint k = ki;
			c = qRound((c * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			m = qRound((m * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			y = qRound((y * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			k = qRound((k * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			ret = ScColor(c, m, y, k);
		}
		else if (m_colorPrecision == 32)
		{
			quint32 ci, mi, yi, ki;
			ts >> ci >> mi >> yi >> ki;
			uint c = ci;
			uint m = mi;
			uint y = yi;
			uint k = ki;
			c = qRound((c * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			m = qRound((m * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			y = qRound((y * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			k = qRound((k * (m_maxColor - m_minColor) / static_cast<double>(m_maxColor)) / static_cast<double>(m_maxColor) * 255.0);
			ret = ScColor(c, m, y, k);
		}
	}
	return ret;
}

QString CgmPlug::getBinaryIndexedColor(ScBitReader *breader)
{
	QString ret = "Black";
	uint c = breader->getUInt(m_colorIndexPrecision);
	if (m_ColorTableMap.contains(c) && (c <= m_maxColorIndex))
		ret = m_ColorTableMap[c];
	return ret;
}

QString CgmPlug::getBinaryIndexedColor(QDataStream &ts)
{
	QString ret = "Black";
	uint c = getBinaryUInt(ts, m_colorIndexPrecision);
	if (m_ColorTableMap.contains(c) && (c <= m_maxColorIndex)) 
		ret = m_ColorTableMap[c];
	return ret;
}

QString CgmPlug::getBinaryColor(QDataStream &ts)
{
	QString ret;
	ScColor color;
	if (m_colorMode == 0)
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
	if (m_vdcType == 0)				// integer coords
	{
		int x = getBinaryInt(ts, m_vdcInt);
		ret = x;
	}
	else
		ret = getBinaryReal(ts, m_vdcReal, m_vdcMantissa);
	return ret;
}

QPointF CgmPlug::getBinaryCoords(QDataStream &ts, bool raw)
{
	QPointF ret = QPointF(0.0, 0.0);
	if (m_vdcType == 0)				// integer coords
	{
		int x = getBinaryInt(ts, m_vdcInt);
		int y = getBinaryInt(ts, m_vdcInt);
		if (!raw)
		{
			if (m_vcdFlippedV)
				y = m_vdcHeight - y;
			if (m_vcdFlippedH)
				x = m_vdcWidth - x;
		}
		ret = QPointF(x, y);
	}
	else
	{
		double x = getBinaryReal(ts, m_vdcReal, m_vdcMantissa);
		double y = getBinaryReal(ts, m_vdcReal, m_vdcMantissa);
		if (!raw)
		{
			if (m_vcdFlippedV)
				y = m_vdcHeight - y;
			if (m_vcdFlippedH)
				x = m_vdcWidth - x;
		}
		ret = QPointF(x, y);
	}
	return ret;
}

uint CgmPlug::getBinaryUInt(QDataStream &ts, int intP)
{
	uint val = 0;
	if (intP == 1)
	{
		quint8 data;
		ts >> data;
		val = data >> 7;
	}
	else if (intP == 8)
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
			ts.setFloatingPointPrecision(QDataStream::SinglePrecision);
			float data;
			ts >> data;
			val = data;
		}
		else
		{
			ts.setFloatingPointPrecision(QDataStream::DoublePrecision);
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
	if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
		m_Doc->documentInfo().setTitle(value);
	// qDebug() << "Start Metafile" << value;
}

void CgmPlug::handleStartPicture(QString value)
{
	m_pictName = value;
//	qDebug() << "Start Picture" << value;
}

void CgmPlug::handleStartPictureBody(double width, double height)
{
	if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (m_firstPage)
		{
			m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
			if (width > height)
				m_Doc->setPageOrientation(1);
			else
				m_Doc->setPageOrientation(0);
			m_Doc->setPageSize("Custom");
			m_Doc->changePageProperties(0, 0, 0, 0, height, width, height, width, m_Doc->pageOrientation(), m_Doc->pageSize(), m_Doc->currentPage()->pageNr(), 0);
		}
		else
		{
			if (m_wasEndPic)
			{
				m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
				m_Doc->addPage(m_Doc->currentPage()->pageNr()+1);
				m_Doc->view()->addPage(m_Doc->currentPage()->pageNr(), true);
			}
		}
		m_wasEndPic = false;
		m_firstPage = false;
	}
}

void CgmPlug::handleMetaFileDescription(QString value)
{
	if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
		m_Doc->documentInfo().setComments(value);
	// qDebug() << "Metafile Description" << value;
}

QString CgmPlug::handleColor(ScColor &color, QString proposedName)
{
	QString tmpName = m_Doc->PageColors.tryAddColor(proposedName, color);
	if (tmpName == proposedName)
		m_importedColors.append(tmpName);
	return tmpName;
}

double CgmPlug::convertCoords(double input)
{
	return input * m_metaScale;
}

QPointF CgmPlug::convertCoords(QPointF input)
{
	return input * m_metaScale;
}

void CgmPlug::appendPath(QPainterPath &path1, QPainterPath &path2)
{
	for (int i = 0; i < path2.elementCount(); ++i)
	{
		const QPainterPath::Element &elm = path2.elementAt(i);
		switch (elm.type)
		{
			case QPainterPath::MoveToElement:
				path1.moveTo(elm.x, elm.y);
				break;
			case QPainterPath::LineToElement:
				path1.lineTo(elm.x, elm.y);
				break;
			case QPainterPath::CurveToElement:
				path1.cubicTo(elm.x, elm.y, path2.elementAt(i+1).x, path2.elementAt(i+1).y, path2.elementAt(i+2).x, path2.elementAt(i+2).y );
				break;
			default:
				break;
		}
	}
}

PageItem* CgmPlug::itemAdd(PageItem::ItemType itemType, PageItem::ItemFrameType frameType, double x, double y, double b, double h, double w, QString fill, QString stroke)
{
	int z;
	if (m_lineVisible)
	{
		if (m_fillType == 0)
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, CommonStrings::None, stroke, true);
		else if ((m_fillType == 1) || (m_fillType == 3))
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, fill, stroke, true);
		else if (m_fillType == 2)
		{
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, fill, stroke, true);
			if (m_patternTable.contains(m_patternIndex))
			{
				PageItem *ite = m_Doc->Items->at(z);
				ite->setPattern(m_patternTable[m_patternIndex]);
				ScPattern pat = m_Doc->docPatterns[m_patternTable[m_patternIndex]];
				double patSX = 100;
				double patSY = 100;
				if (m_patternScaleX > -1)
					patSX = m_patternScaleX / pat.width * 100;
				if (m_patternScaleY > -1)
					patSY = m_patternScaleY / pat.height * 100;
				ite->setPatternTransform(patSX, patSY, 0, 0, 0, 0.0, 0.0);
				ite->GrType = 8;
			}
		}
		else if (m_fillType == 4)
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, CommonStrings::None, stroke, true);
		else
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, fill, stroke, true);
	}
	else
	{
		if (m_fillType == 0)
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, CommonStrings::None, fill, true);
		else if ((m_fillType == 1) || (m_fillType == 3))
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, fill, CommonStrings::None, true);
		else if (m_fillType == 2)
		{
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, fill, CommonStrings::None, true);
			if (m_patternTable.contains(m_patternIndex))
			{
				PageItem *ite = m_Doc->Items->at(z);
				ite->setPattern(m_patternTable[m_patternIndex]);
				ScPattern pat = m_Doc->docPatterns[m_patternTable[m_patternIndex]];
				double patSX = 100;
				double patSY = 100;
				if (m_patternScaleX > -1)
					patSX = m_patternScaleX / pat.width * 100;
				if (m_patternScaleY > -1)
					patSY = m_patternScaleY / pat.height * 100;
				ite->setPatternTransform(patSX, patSY, 0, 0, 0, 0.0, 0.0);
				ite->GrType = 8;
			}
		}
		else
			z = m_Doc->itemAdd(itemType, frameType, x, y, b, h, w, CommonStrings::None, CommonStrings::None, true);
	}
	return m_Doc->Items->at(z);
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
		ite->setLineStyle(m_lineType);
		ite->setLineEnd(m_lineCap);
		ite->setLineJoin(m_lineJoin);
	}
	else
	{
		ite->setLineStyle(m_edgeType);
		ite->setLineEnd(m_edgeCap);
		ite->setLineJoin(m_edgeJoin);
	}
	ite->updateClip();
	m_Elements.append(ite);
	if (m_groupStack.count() != 0)
		m_groupStack.top().append(ite);
	m_Coords.resize(0);
	m_Coords.svgInit();
}
