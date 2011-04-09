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
#include <QTemporaryFile>
#include <QDebug>

#include <cstdlib>
#include <tiffio.h>
#include <zlib.h>

#include "commonstrings.h"

#include "importai.h"
#include "loadsaveplugin.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "rawimage.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scclocale.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "selection.h"
#include "text/specialchars.h"
#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "util.h"
#include "util_color.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"
#include "util_ghostscript.h"

#include <cairo.h>

#ifdef HAVE_PODOFO
	#include <podofo/podofo.h>
#endif

extern SCRIBUS_API ScribusQApp * ScQApp;

AIPlug::AIPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = NULL;
}

QImage AIPlug::readThumbnail(QString fNameIn)
{
	QString fName = fNameIn;
	double x, y, b, h;
	CustColors.clear();
	importedColors.clear();
	importedGradients.clear();
	importedPatterns.clear();
	QFileInfo fi = QFileInfo(fName);
/* Check if the file is an old style AI or one of the newer PDF wrapped ones */
	QFile fT(fName);
	if (fT.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(9, ' ');
		fT.read(tempBuf.data(), 8);
		fT.close();
		if (tempBuf.startsWith("%PDF"))
		{
			QString tmp, cmd1, cmd2;
			QString pdfFile = QDir::convertSeparators(fName);
			QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
			int ret = -1;
			tmp.setNum(1);
			QStringList args;
			args.append("-r72");
			args.append("-sOutputFile="+tmpFile);
			args.append("-dFirstPage="+tmp);
			args.append("-dLastPage="+tmp);
			args.append(pdfFile);
			ret = callGS(args);
			if (ret == 0)
			{
				QImage image;
				image.load(tmpFile);
				QFile::remove(tmpFile);
				image.setText("XSize", QString("%1").arg(image.width()));
				image.setText("YSize", QString("%1").arg(image.height()));
				return image;
			}
			else
				return QImage();
		}
	}
	QFile fT2(fName);
	if (fT2.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(25, ' ');
		fT2.read(tempBuf.data(), 20);
		fT2.close();
		/* Illustrator CS files might be compressed
			the compressed Data starts right after the "%AI12_CompressedData" comment
			Compression is a simple zlib compression */
		if (tempBuf.startsWith("%AI12_CompressedData"))
			decompressAIData(fName);
	}
	progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	parseHeader(fName, x, y, b, h);
	if (b == 0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docX = x;
	docY = y;
	docWidth = b - x;
	docHeight = h - y;
	baseX = 0;
	baseY = 0;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	baseX = m_Doc->currentPage()->xOffset();
	baseY = m_Doc->currentPage()->yOffset();
	ColorList::Iterator it;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
		{
			m_Doc->PageColors.insert(it.key(), it.value());
			importedColors.append(it.key());
		}
	}
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	QImage tmpImage;
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (Elements.count() > 1)
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
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
		m_Doc->m_Selection->delaySignalsOff();
	}
	else
		tmpImage = QImage();
	m_Doc->scMW()->setScriptRunning(false);
	m_Doc->setLoading(false);
	delete m_Doc;
	QDir::setCurrent(CurDirP);
	return tmpImage;
}

bool AIPlug::readColors(const QString& fNameIn, ColorList & colors)
{
	QString fName = fNameIn;
	bool success = false;
	cancel = false;
	double x, y, b, h;
	convertedPDF = false;
	CustColors.clear();
	importedColors.clear();
	importedGradients.clear();
	importedPatterns.clear();
	QFileInfo fi = QFileInfo(fName);
/* Check if the file is an old style AI or one of the newer PDF wrapped ones */
	QFile fT(fName);
	if (fT.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(9, ' ');
		fT.read(tempBuf.data(), 8);
		fT.close();
		if (tempBuf.startsWith("%PDF"))
		{
			QFileInfo bF2(fName);
			QString tmpFile = ScPaths::getTempFileDir()+ "/"+bF2.baseName()+"_tmp.ai";
			if (!extractFromPDF(fName, tmpFile))
				return false;
			convertedPDF = true;
			fName = tmpFile;
		}
	}
	QFile fT2(fName);
	if (fT2.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(25, ' ');
		fT2.read(tempBuf.data(), 20);
		fT2.close();
		/* Illustrator CS files might be compressed
			the compressed Data starts right after the "%AI12_CompressedData" comment
			Compression is a simple zlib compression */
		if (tempBuf.startsWith("%AI12_CompressedData"))
			decompressAIData(fName);
	}
	progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	parseHeader(fName, x, y, b, h);
	docX = x;
	docY = y;
	docWidth = b - x;
	docHeight = h - y;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	baseX = m_Doc->currentPage()->xOffset();
	baseY = m_Doc->currentPage()->yOffset();
	ColorList::Iterator it;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
		{
			m_Doc->PageColors.insert(it.key(), it.value());
			importedColors.append(it.key());
		}
	}
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	convert(fName);
	if (importedColors.count() != 0)
	{
		colors = m_Doc->PageColors;
		success = true;
	}
	m_Doc->scMW()->setScriptRunning(false);
	m_Doc->setLoading(false);
	delete m_Doc;
	QDir::setCurrent(CurDirP);
	if (convertedPDF)
		QFile::remove(fName);
	return success;
}

bool AIPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	convertedPDF = false;
	CustColors.clear();
	importedColors.clear();
	importedGradients.clear();
	importedPatterns.clear();
	QFileInfo fi = QFileInfo(fName);
//	QString ext = fi.suffix().toLower();
	if ( !ScCore->usingGUI() )
	{
		interactive = false;
		showProgress = false;
	}
	
/* Check if the file is an old style AI or one of the newer PDF wrapped ones */
	QFile fT(fName);
	if (fT.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(9, ' ');
		fT.read(tempBuf.data(), 8);
		fT.close();
		if (tempBuf.startsWith("%PDF"))
		{
			QFileInfo bF2(fName);
			QString tmpFile = ScPaths::getTempFileDir()+ "/"+bF2.baseName()+"_tmp.ai";
			if (!extractFromPDF(fName, tmpFile))
				return false;
			convertedPDF = true;
			fName = tmpFile;
		}
	}
	QFile fT2(fName);
	if (fT2.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(25, ' ');
		fT2.read(tempBuf.data(), 20);
		fT2.close();
		/* Illustrator CS files might be compressed
			the compressed Data starts right after the "%AI12_CompressedData" comment
			Compression is a simple zlib compression */
		if (tempBuf.startsWith("%AI12_CompressedData"))
			decompressAIData(fName);
	}
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
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	parseHeader(fName, x, y, b, h);
	if (b == 0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docX = x;
	docY = y;
	docWidth = b - x;
	docHeight = h - y;
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
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->documentInfo().setAuthor(docCreator);
		m_Doc->documentInfo().setPublisher(docOrganisation);
		m_Doc->documentInfo().setTitle(docTitle);
		m_Doc->documentInfo().setDate(docDate+" "+docTime);
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
		{
			m_Doc->PageColors.insert(it.key(), it.value());
			importedColors.append(it.key());
		}
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
		if (Elements.count() == 0)
		{
			if ((importedColors.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns))))
			{
				for (int cd = 0; cd < importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(importedColors[cd]);
				}
			}
			if ((importedGradients.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients || (flags & LoadSavePlugin::lfKeepPatterns)))))
			{
				for (int cd = 0; cd < importedGradients.count(); cd++)
				{
					m_Doc->docGradients.remove(importedGradients[cd]);
				}
			}
			if ((importedPatterns.count() != 0) && (!(flags & LoadSavePlugin::lfKeepPatterns)))
			{
				for (int cd = 0; cd < importedPatterns.count(); cd++)
				{
					m_Doc->docPatterns.remove(importedPatterns[cd]);
				}
			}
		}
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
/*#ifndef Q_WS_MAC*/
// see #2196
				m_Doc->itemSelection_DeleteItem(tmpSel);
/*#else
				qDebug("aiimport: leaving items on page");
#endif*/
				m_Doc->view()->updatesOn(true);
				if ((importedColors.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns))))
				{
					for (int cd = 0; cd < importedColors.count(); cd++)
					{
						m_Doc->PageColors.remove(importedColors[cd]);
					}
				}
				if ((importedGradients.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients || (flags & LoadSavePlugin::lfKeepPatterns)))))
				{
					for (int cd = 0; cd < importedGradients.count(); cd++)
					{
						m_Doc->docGradients.remove(importedGradients[cd]);
					}
				}
				if ((importedPatterns.count() != 0) && (!(flags & LoadSavePlugin::lfKeepPatterns)))
				{
					for (int cd = 0; cd < importedPatterns.count(); cd++)
					{
						m_Doc->docPatterns.remove(importedPatterns[cd]);
					}
				}
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
	if (convertedPDF)
		QFile::remove(fName);
	return success;
}

AIPlug::~AIPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

bool AIPlug::extractFromPDF(QString infile, QString outfile)
{
	bool ret = false;
#ifdef HAVE_PODOFO
	QFile outf(outfile);
	outf.open(QIODevice::WriteOnly);
	try
	{
		PoDoFo::PdfError::EnableDebug( false );
#if (PODOFO_VERSION == 0 && PODOFO_MINOR > 6)
		PoDoFo::PdfError::EnableLogging( false );
#endif
#if (PODOFO_VERSION == 0 && PODOFO_MINOR == 5 && PODOFO_REVISION == 99) || PODOFO_MINOR > 5
		PoDoFo::PdfMemDocument doc( infile.toLocal8Bit().data() );
#else
		PoDoFo::PdfDocument doc( infile.toLocal8Bit().data() );
#endif
		PoDoFo::PdfPage *curPage = doc.GetPage(0);
		if (curPage != NULL)
		{
			PoDoFo::PdfObject *piece = curPage->GetObject()->GetIndirectKey("PieceInfo");
			if (piece != NULL)
			{
				PoDoFo::PdfObject *illy = piece->GetIndirectKey("Illustrator");
				if (illy != NULL)
				{
					PoDoFo::PdfObject *priv = illy->GetIndirectKey("Private");
					if (priv == NULL)
						priv = illy;
					int num = 0;
					PoDoFo::PdfObject *numBl = priv->GetIndirectKey("NumBlock");
					if (numBl != NULL)
						num = numBl->GetNumber() + 1;
					if (num == 0)
						num = 99999;
					QString name = "AIPrivateData%1";
					QString Key = name.arg(1);
					PoDoFo::PdfObject *data = priv->GetIndirectKey(PoDoFo::PdfName(Key.toUtf8().data()));
					if (data == NULL)
					{
						name = "AIPDFPrivateData%1";
						Key = name.arg(1);
						data = priv->GetIndirectKey(PoDoFo::PdfName(Key.toUtf8().data()));
					}
					if (data != NULL)
					{
						if (num == 2)
						{
							Key = name.arg(1);
							data = priv->GetIndirectKey(PoDoFo::PdfName(Key.toUtf8().data()));
							PoDoFo::PdfStream const *stream = data->GetStream();
							PoDoFo::PdfMemoryOutputStream oStream(1);
							stream->GetFilteredCopy(&oStream);
							oStream.Close();
							long bLen = oStream.GetLength();
							char *Buffer = oStream.TakeBuffer();
							outf.write(Buffer, bLen);
							free( Buffer );
						}
						else
						{
							for (int a = 2; a < num; a++)
							{
								Key = name.arg(a);
								data = priv->GetIndirectKey(PoDoFo::PdfName(Key.toUtf8().data()));
								if (data == NULL)
									break;
								PoDoFo::PdfStream const *stream = data->GetStream();
								PoDoFo::PdfMemoryOutputStream oStream(1);
								stream->GetFilteredCopy(&oStream);
								oStream.Close();
								long bLen = oStream.GetLength();
								char *Buffer = oStream.TakeBuffer();
								outf.write(Buffer, bLen);
								free( Buffer );
							}
						}
					}
					ret = true;
				}
			}
		}
		outf.close();
	}
	catch (PoDoFo::PdfError& e)
	{
		outf.close();
		qDebug("Scribus caught and handled the following exception from PoDoFo while processing a PDF format ai file:\n----\n");
		e.PrintErrorMsg();
		qDebug("----\nThe ai file could not be imported.\n");
		QFile::remove(outfile);
		return false;
	}
#endif
	return ret;
}

bool AIPlug::decompressAIData(QString &fName)
{
	QString f2 = fName+"_decom.ai";
	FILE *source = fopen(fName.toLocal8Bit().constData(), "rb");
	fseek(source, 20, SEEK_SET);
	FILE *dest = fopen(f2.toLocal8Bit().constData(), "wb");
	int ret;
	unsigned have;
	z_stream strm;
	char in[4096];
	char out[4096];
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return false;
	do
	{
		strm.avail_in = fread(in, 1, 4096, source);
		if (ferror(source))
		{
			(void)inflateEnd(&strm);
			return false;
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = (Bytef*)in;
		do
		{
			strm.avail_out = 4096;
			strm.next_out = (Bytef*)out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);
			switch (ret)
			{
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					return false;
			}
			have = 4096 - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest))
			{
				(void)inflateEnd(&strm);
				return false;
			}
		}
		while (strm.avail_out == 0);
	}
	while (ret != Z_STREAM_END);
	(void)inflateEnd(&strm);
	fclose(source);
	fclose(dest);
	if (!convertedPDF)
	{
		QFileInfo bF2(fName);
		QString tmpFile = ScPaths::getTempFileDir()+ "/"+bF2.baseName()+"_tmp.ai";
		moveFile(f2, tmpFile);
		fName = tmpFile;
		convertedPDF = true;
	}
	else
	{
		QFile::remove(fName);
		fName = f2;
	}
	return true;
}

bool AIPlug::parseHeader(QString fName, double &x, double &y, double &b, double &h)
{
	QString tmp, BBox, tmp2, FarNam;
	ScColor cc;
	double c, m, yc, k;
	bool found = false;
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
/* Try to find Bounding Box */
		bool isAtend = false;
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
			if (tmp.startsWith("%%HiResBoundingBox:"))
			{
				found = true;
				BBox = tmp.remove("%%HiResBoundingBox:");
			}
		//	if (tmp.startsWith("%AI3_TileBox:"))
		//	{
		//		found = true;
		//		BBox = tmp.remove("%AI3_TileBox:");
		//	}
			if (tmp.startsWith("%%For"))
			{
				QStringList res = getStrings(tmp);
				if (res.count() > 1)
				{
					docCreator = res[0];
					docOrganisation = res[1];
				}
			}
			if (tmp.startsWith("%%CreationDate:"))
			{
				QStringList res = getStrings(tmp);
				if (res.count() > 1)
				{
					docDate = res[0];
					docTime = res[1];
				}
				else
				{
					docDate = tmp.remove("%%CreationDate: ");
					docTime = "";
				}
			}
			if (tmp.startsWith("%%Title"))
			{
				QStringList res = getStrings(tmp);
				if (res.count() > 0)
					docTitle = res[0];
			}
			if ((tmp.startsWith("%%CMYKCustomColor")) || (tmp.startsWith("%%CMYKProcessColor")))
			{
				if (tmp.contains("(atend)"))
					isAtend = true;
				else
				{
					if (tmp.startsWith("%%CMYKCustomColor"))
						tmp = tmp.remove(0,18);
					else if (tmp.startsWith("%%CMYKProcessColor"))
						tmp = tmp.remove(0,19);
					ScTextStream ts2(&tmp, QIODevice::ReadOnly);
					ts2 >> c >> m >> yc >> k;
					FarNam = ts2.readAll();
					FarNam = FarNam.trimmed();
					FarNam = FarNam.remove(0,1);
					FarNam = FarNam.remove(FarNam.length()-1,1);
					FarNam = FarNam.simplified();
					QByteArray farN;
					for (int a = 0; a < FarNam.length(); a++)
					{
						QChar ch = FarNam.at(a);
						uint chc = ch.unicode();
						if (chc > 255)
							farN.append(chc >> 8);
						farN.append(chc & 0x00FF);
					}
					FarNam = QString::fromUtf8(farN.constData());
					cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * yc), qRound(255 * k));
					cc.setSpotColor(true);
					if (!FarNam.isEmpty())
						CustColors.tryAddColor(FarNam, cc);
					while (!ts.atEnd())
					{
						quint64 oldPos = ts.device()->pos();
						tmp = readLinefromDataStream(ts);
						if (!tmp.startsWith("%%+"))
						{
							ts.device()->seek(oldPos);
							break;
						}
						tmp = tmp.remove(0,3);
						ScTextStream ts2(&tmp, QIODevice::ReadOnly);
						ts2 >> c >> m >> yc >> k;
						FarNam = ts2.readAll();
						FarNam = FarNam.trimmed();
						FarNam = FarNam.remove(0,1);
						FarNam = FarNam.remove(FarNam.length()-1,1);
						FarNam = FarNam.simplified();
						QByteArray farN;
						for (int a = 0; a < FarNam.length(); a++)
						{
							QChar ch = FarNam.at(a);
							uint chc = ch.unicode();
							if (chc > 255)
								farN.append(chc >> 8);
							farN.append(chc & 0x00FF);
						}
						FarNam = QString::fromUtf8(farN.constData());
						cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * yc), qRound(255 * k));
						cc.setSpotColor(true);
						if (!FarNam.isEmpty())
							CustColors.tryAddColor(FarNam, cc);
					}
				}
			}
			if ((tmp.startsWith("%%RGBCustomColor")) || (tmp.startsWith("%%RGBProcessColor")))
			{
				if (tmp.contains("(atend)"))
					isAtend = true;
				else
				{
					if (tmp.startsWith("%%RGBCustomColor"))
						tmp = tmp.remove(0,17);
					else if (tmp.startsWith("%%RGBProcessColor"))
						tmp = tmp.remove(0,18);
					ScTextStream ts2(&tmp, QIODevice::ReadOnly);
					ts2 >> c >> m >> yc;
					FarNam = ts2.readAll();
					FarNam = FarNam.trimmed();
					FarNam = FarNam.remove(0,1);
					FarNam = FarNam.remove(FarNam.length()-1,1);
					FarNam = FarNam.simplified();
					QByteArray farN;
					for (int a = 0; a < FarNam.length(); a++)
					{
						QChar ch = FarNam.at(a);
						uint chc = ch.unicode();
						if (chc > 255)
							farN.append(chc >> 8);
						farN.append(chc & 0x00FF);
					}
					FarNam = QString::fromUtf8(farN.constData());
					cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * yc));
					if (!FarNam.isEmpty())
						CustColors.tryAddColor(FarNam, cc);
					while (!ts.atEnd())
					{
						quint64 oldPos = ts.device()->pos();
						tmp = readLinefromDataStream(ts);
						if (!tmp.startsWith("%%+"))
						{
							ts.device()->seek(oldPos);
							break;
						}
						tmp = tmp.remove(0,3);
						ScTextStream ts2(&tmp, QIODevice::ReadOnly);
						ts2 >> c >> m >> yc;
						FarNam = ts2.readAll();
						FarNam = FarNam.trimmed();
						FarNam = FarNam.remove(0,1);
						FarNam = FarNam.remove(FarNam.length()-1,1);
						FarNam = FarNam.simplified();
						QByteArray farN;
						for (int a = 0; a < FarNam.length(); a++)
						{
							QChar ch = FarNam.at(a);
							uint chc = ch.unicode();
							if (chc > 255)
								farN.append(chc >> 8);
							farN.append(chc & 0x00FF);
						}
						FarNam = QString::fromUtf8(farN.constData());
						cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * yc));
						if (!FarNam.isEmpty())
							CustColors.tryAddColor(FarNam, cc);
					}
				}
			}
			if (tmp.startsWith("%%EndComments"))
			{
				while (!ts.atEnd())
				{
					bool isX = false;
					tmp = readLinefromDataStream(ts);
					if ((tmp.endsWith("Xa") || tmp.endsWith(" k") || tmp.endsWith(" x")) && (tmp.length() > 4))
					{
						ScTextStream ts2(&tmp, QIODevice::ReadOnly);
						ts2 >> c >> m >> yc >> k;
						if (tmp.endsWith(" x"))
						{
							isX = true;
							int an = tmp.indexOf("(");
							int en = tmp.lastIndexOf(")");
							FarNam = tmp.mid(an+1, en-an-1);
							FarNam = FarNam.simplified();
							QByteArray farN;
							for (int a = 0; a < FarNam.length(); a++)
							{
								QChar ch = FarNam.at(a);
								uint chc = ch.unicode();
								if (chc > 255)
									farN.append(chc >> 8);
								farN.append(chc & 0x00FF);
							}
							FarNam = QString::fromUtf8(farN.constData());
						}
						tmp = readLinefromDataStream(ts);
						if (tmp.endsWith("Pc"))
						{
							if (!isX)
							{
								tmp = tmp.trimmed();
								tmp = tmp.remove(0,1);
								int en = tmp.indexOf(")");
								FarNam = tmp.mid(0, en);
								FarNam = FarNam.simplified();
								QByteArray farN;
								for (int a = 0; a < FarNam.length(); a++)
								{
									QChar ch = FarNam.at(a);
									uint chc = ch.unicode();
									if (chc > 255)
										farN.append(chc >> 8);
									farN.append(chc & 0x00FF);
								}
								FarNam = QString::fromUtf8(farN.constData());
							}
							cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * yc), qRound(255 * k));
							cc.setSpotColor(true);
							CustColors.tryAddColor(FarNam, cc);
						}
					}
				}
				if (!isAtend)
					break;
			}
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
	return found;
}

QString AIPlug::removeAIPrefix(QString comment)
{
	QString tmp;
	if (comment.startsWith("%AI"))
	{
		int an = comment.indexOf("_");
		tmp = comment.remove(0, an+1);
	}
	else
		tmp = comment;
	return tmp;
}

QString AIPlug::parseColor(QString data)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double c, m, y, k;
	ScColor tmp;
	ScTextStream Code(&data, QIODevice::ReadOnly);
	Code >> c;
	Code >> m;
	Code >> y;
	Code >> k;
	int Cc = qRound(c * 255);
	int Mc = qRound(m * 255);
	int Yc = qRound(y * 255);
	int Kc = qRound(k * 255);
	tmp.setColor(Cc, Mc, Yc, Kc);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString namPrefix = "FromAI";
	QString fNam = m_Doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	if (fNam == namPrefix+tmp.name())
		importedColors.append(fNam);
	ret = fNam;
	meshColorMode = 0;
	return ret;
}

QString AIPlug::parseColorGray(QString data)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double k;
	ScColor tmp;
	ColorList::Iterator it;
	ScTextStream Code(&data, QIODevice::ReadOnly);
	Code >> k;
	int Kc = 255 - qRound(k * 255);
	tmp.setColor(0, 0, 0, Kc);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString namPrefix = "FromAI";
	QString fNam = m_Doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	if (fNam == namPrefix+tmp.name())
		importedColors.append(fNam);
	ret = fNam;
	meshColorMode = 2;
	return ret;
}

QString AIPlug::parseColorRGB(QString data)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double r, g, b;
	ScColor tmp;
	ScTextStream Code(&data, QIODevice::ReadOnly);
	Code >> r;
	Code >> g;
	Code >> b;
	int Rc = qRound(r * 255);
	int Gc = qRound(g * 255);
	int Bc = qRound(b * 255);
	tmp.setColorRGB(Rc, Gc, Bc);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString namPrefix = "FromAI";
	QString fNam = m_Doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	if (fNam == namPrefix+tmp.name())
		importedColors.append(fNam);
	ret = fNam;
	meshColorMode = 1;
	return ret;
}

QString AIPlug::parseCustomColor(QString data, double &shade)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double c, m, y, k, sh;
	ScColor tmp;
	ScTextStream Code(&data, QIODevice::ReadOnly);
	Code >> c;
	Code >> m;
	Code >> y;
	Code >> k;
	QString tmpS = data;
	int an = data.indexOf("(");
	int en = data.lastIndexOf(")");
	QString FarNam = data.mid(an+1, en-an-1);
	FarNam.remove("\\");
	QString FarSha = data.mid(en+1, data.size() - en);
	ScTextStream Val(&FarSha, QIODevice::ReadOnly);
	Val >> sh;
	shade = (1.0 - sh) * 100.0;
	int Cc = qRound(c * 255);
	int Mc = qRound(m * 255);
	int Yc = qRound(y * 255);
	int Kc = qRound(k * 255);
	tmp.setColor(Cc, Mc, Yc, Kc);
	tmp.setSpotColor(true);
	tmp.setRegistrationColor(false);
	QString fNam = m_Doc->PageColors.tryAddColor(FarNam, tmp);
	if (fNam == FarNam)
		importedColors.append(FarNam);
	ret = fNam;
	meshColorMode = 0;
	return ret;
}

QString AIPlug::parseCustomColorX(QString data, double &shade, QString type)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double c, m, y, k, sh, r, g, b;
	ScColor tmp;
	ScTextStream Code(&data, QIODevice::ReadOnly);
	if (type == "1")
	{
		Code >> r;
		Code >> g;
		Code >> b;
		int Rc = qRound(r * 255);
		int Gc = qRound(g * 255);
		int Bc = qRound(b * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		meshColorMode = 1;
	}
	else
	{
		Code >> c;
		Code >> m;
		Code >> y;
		Code >> k;
		int Cc = qRound(c * 255);
		int Mc = qRound(m * 255);
		int Yc = qRound(y * 255);
		int Kc = qRound(k * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		meshColorMode = 0;
	}
	QString tmpS = data;
	int an = data.indexOf("(");
	int en = data.lastIndexOf(")");
	QString FarNam = data.mid(an+1, en-an-1);
	FarNam.remove("\\");
	QString FarSha = data.mid(en+1, data.size() - en);
	ScTextStream Val(&FarSha, QIODevice::ReadOnly);
	Val >> sh;
	shade = (1.0 - sh) * 100.0;
	if (type == "0")
		tmp.setSpotColor(true);
	tmp.setRegistrationColor(false);
	QString fNam = m_Doc->PageColors.tryAddColor(FarNam, tmp);
	if (fNam == FarNam)
		importedColors.append(FarNam);
	ret = fNam;
	return ret;
}

QStringList AIPlug::getStrings(QString data)
{
	QStringList result;
	result.clear();
	QChar tmp;
	QString tmp2 = "";
	QString tmp3 = "";
	bool paran = false;
	bool skip = false;
	int digitCount = 0;
	for (int a = 0; a < data.count(); a++)
	{
		tmp = data[a];
		if (skip)
		{
			if (paran)
			{
				if (tmp.isDigit())
				{
					tmp3 += tmp;
					digitCount++;
					if (digitCount == 3)
					{
						bool ok = false;
						int code = tmp3.toInt(&ok, 8);
						if (ok)
							tmp2 += QChar(code);
						digitCount = 0;
						tmp3 = "";
						skip = false;
					}
				}
				else
				{
					if (tmp == 'r')
						tmp = SpecialChars::PARSEP;
					tmp2 += tmp;
					skip = false;
				}
			}
			continue;
		}
		if (tmp == '(')
		{
			paran = true;
			continue;
		}
		if (tmp == ')')
		{
			paran = false;
			result.append(tmp2);
			tmp2 = "";
			continue;
		}
		if (tmp == '\\')
		{
			skip = true;
			continue;
		}
		if (paran)
			tmp2 += tmp;
	}
	return result;
}

void AIPlug::getCommands(QString data, QStringList &commands)
{
	QString tmp;
	QString tmp2;
	QString tmp3;
	bool paran = false;
	bool arra = false;
	bool skip = false;
	for (int a = 0; a < data.count(); a++)
	{
		tmp = data[a];
		if (skip)
		{
			tmp2 += tmp;
			skip = false;
			continue;
		}
		if (tmp == "(")
		{
			paran = true;
			tmp2 += tmp;
			continue;
		}
		if (tmp == ")")
		{
			paran = false;
			tmp2 += tmp;
			continue;
		}
		if (tmp == "[")
		{
			arra = true;
			tmp2 += tmp;
			continue;
		}
		if (tmp == "]")
		{
			arra = false;
			tmp2 += tmp;
			continue;
		}
//		if (tmp == "\\")
//		{
//			skip = true;
//			continue;
//		}
		if (!paran)
		{
			if (tmp == " ")
			{
				tmp3 += " " + tmp2;
				if (commandList.contains(tmp2))
				{
					commands.append(tmp3);
					tmp3 = "";
				}
				tmp2 = "";
				continue;
			}
		}
		tmp2 += tmp;
	}
	if (!tmp2.isEmpty())
	{
		tmp3 += " " + tmp2;
		commands.append(tmp3);
	}
}

void AIPlug::decodeA85(QByteArray &psdata, QString tmp)
{
	uchar byte;
	ushort data;
	unsigned long sum = 0;
	int quintet = 0;
	for (int c = 0; c < tmp.length(); c++)
	{
		byte = QChar(tmp.at(c)).cell();
		if (byte >= '!' && byte <= 'u')
		{
			sum = sum * 85 + ((unsigned long)byte - '!');
			quintet++;
			if (quintet == 5)
			{
				psdata.resize(psdata.size()+4);
				data = (sum >> 24) & 0xFF;
				psdata[psdata.size()-4] = data;
				data = (sum >> 16) & 0xFF;
				psdata[psdata.size()-3] = data;
				data = (sum >> 8) & 0xFF;
				psdata[psdata.size()-2] = data;
				data = sum & 0xFF;
				psdata[psdata.size()-1] = data;
				quintet = 0;
				sum = 0;
			}
		}
		else if (byte == 'z')
		{
			psdata.resize(psdata.size()+4);
			psdata[psdata.size()-4] = 0;
			psdata[psdata.size()-3] = 0;
			psdata[psdata.size()-2] = 0;
			psdata[psdata.size()-1] = 0;
		}
		else if (byte == '~')
		{
			if (quintet)
			{
				int i;
				for (i = 0; i < 5 - quintet; i++)
					sum *= 85;
				if (quintet > 1)
					sum += (0xFFFFFF >> ((quintet - 2) * 8));
				for (i = 0; i < quintet - 1; i++)
				{
					data = (sum >> (24 - 8 * i)) & 0xFF;
					psdata.resize(psdata.size()+1);
					psdata[psdata.size()-1] = data;
				}
				quintet = 0;
			}
			break;
		}
	}
}

void AIPlug::processData(QString data)
{
	double x, y, x1, y1, x2, y2;
	int z, tmpInt;
	PageItem *ite;
	QString command = "";
	QString Cdata = "";
	QStringList da;
	if (dataMode && fObjectMode)
	{
		if (data.contains("~>"))
		{
			dataString += data.mid(1);
			dataMode = false;
			QByteArray fData;
			decodeA85(fData, dataString);
			dataString = "";
			if (fObjectMode)
			{
				FPoint wh = currentSpecialPath.WidthHeight();
				if ((currentSpecialPath.size() > 3) && (wh.x() != 0.0) && (wh.y() != 0.0))
				{
					z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
					ite = m_Doc->Items->at(z);
					ite->PoLine = currentSpecialPath.copy();
					ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->ClipEdited = true;
					ite->FrameType = 3;
					ite->setFillShade(CurrFillShade);
					ite->setLineShade(CurrStrokeShade);
					ite->setFillEvenOdd(fillRule);
					ite->setFillTransparency(1.0 - Opacity);
					ite->setLineTransparency(1.0 - Opacity);
#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
					ite->setFillBlendmode(blendMode);
					ite->setLineBlendmode(blendMode);
#endif
					ite->setLineEnd(CapStyle);
					ite->setLineJoin(JoinStyle);
					wh = getMaxClipF(&ite->PoLine);
					ite->setWidthHeight(wh.x(),wh.y());
					ite->setTextFlowMode(PageItem::TextFlowDisabled);
					m_Doc->AdjustItemSize(ite);
					ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_ai_XXXXXX.pdf");
					ite->tempImageFile->open();
					ite->tempImageFile->write(fData);
					QString imgName = getLongPathName(ite->tempImageFile->fileName());
					ite->tempImageFile->close();
					ite->isInlineImage = true;
					m_Doc->LoadPict(imgName, z);
					if (ite->PictureIsAvailable)
						ite->setImageXYScale(ite->width() / ite->pixm.width(), ite->height() / ite->pixm.height());
					ite->setImageFlippedV(true);
					ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
					ite->setRedrawBounding();
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(itemLocked);
					if (patternMode)
						PatternElements.append(ite);
					else
						Elements.append(ite);
					if (groupStack.count() != 0)
						groupStack.top().append(ite);
				}
			}
			fObjectMode = false;
			currentSpecialPath.resize(0);
			currentSpecialPath.svgInit();
		}
		else
		{
			dataString += data.mid(1);
		}
		return;
	}
	getCommands(data, da);
	for (int a = 0; a < da.count(); a++)
	{
		Cdata = da[a];
		if (((Cdata.startsWith("%")) || (Cdata.startsWith(" %"))) && (!meshMode))
			continue;
		if (Cdata.contains("SymbolInstance"))
		{
			symbolMode = true;
			return;
		}
		if (symbolMode)
		{
			if (Cdata.contains("SymbolRef"))
			{
				int an = Cdata.indexOf("(");
				int en = Cdata.lastIndexOf(")");
				if ((an != -1) && (en != -1))
				{
					currentSymbolName = Cdata.mid(an+1, en-an-1);
					currentSymbolName.remove("\\");
					currentSymbolName = "S_"+currentSymbolName.trimmed().simplified().replace(" ", "_");
				}
			}
			else if (Cdata.contains("TransformMatrix"))
			{
				ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
				ts2 >> x >> y >> x1 >> y1 >> x2 >> y2;
				QTransform symTrans = QTransform(x, y, x1, y1, x2, y2);
				double rotation = getRotationFromMatrix(symTrans, 0.0);
				QTransform symT;
				symT.translate(x2, y2);
				QPointF pos1 = importedSymbols[currentSymbolName];
				pos1 = symT.map(pos1);
				double xp = pos1.x();
				double yp = pos1.y();
			//	xp += m_Doc->currentPage()->xOffset();
			//	yp += m_Doc->currentPage()->yOffset();
				int z = m_Doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, baseX + xp, baseY + yp, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
				PageItem *b = m_Doc->Items->at(z);
				b->LayerID = m_Doc->activeLayer();
				ScPattern pat = m_Doc->docPatterns[currentSymbolName];
				b->setWidth(pat.width * symTrans.m11());
				b->setHeight(pat.height * symTrans.m22());
				b->OldB2 = b->width();
				b->OldH2 = b->height();
				b->setPattern(currentSymbolName);
				double xoffset = 0.0, yoffset = 0.0;
			//	if (rotation != 0.0)
			//	{
			//		double temp = -b->height();
			//		xoffset = sin(-rotation) * temp;
			//		yoffset = cos(-rotation) * temp;
			//	}
				b->setXPos(xp + xoffset);
				b->setYPos(yp + yoffset);
				m_Doc->RotMode(3);
				m_Doc->RotateItem(rotation * 180 / M_PI, b);
				m_Doc->RotMode(0);
//				b->setRotation(rotation * 180 / M_PI);
				b->setTextFlowMode(PageItem::TextFlowDisabled);
				b->setFillTransparency(1.0 - Opacity);
				b->setLineTransparency(1.0 - Opacity);
	#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
				b->setFillBlendmode(blendMode);
				b->setLineBlendmode(blendMode);
	#endif
				b->updateClip();
				if (patternMode)
					PatternElements.append(b);
				else
					Elements.append(b);
				if (groupStack.count() != 0)
					groupStack.top().append(b);
				symbolMode = false;
			}
		}
		QStringList da2 = Cdata.split(" ", QString::SkipEmptyParts);
		if (da2.count() == 0)
			return;
		command = da2[da2.count()-1];
/* Start Path construction commands */
		if (command == "m")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y;
			Coords.svgMoveTo(x - docX, docHeight - (y - docY));
			currentPoint = FPoint(x - docX, docHeight - (y - docY));
		}
		else if ((command == "L") || (command == "l"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y;
			Coords.svgLineTo(x - docX, docHeight - (y - docY));
			currentPoint = FPoint(x - docX, docHeight - (y - docY));
		}
		else if ((command == "C") || (command == "c"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y >> x1 >> y1 >> x2 >> y2;
			Coords.svgCurveToCubic(x - docX, docHeight - (y - docY),
								   x1 - docX, docHeight - (y1 - docY),
								   x2 - docX, docHeight - (y2 - docY));
			currentPoint = FPoint(x2 - docX, docHeight - (y2 - docY));
		}
		else if ((command == "Y") || (command == "y"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x1 >> y1 >> x2 >> y2;
			Coords.svgCurveToCubic(x1 - docX, docHeight - (y1 - docY), x2 - docX, docHeight - (y2 - docY), x2 - docX, docHeight - (y2 - docY));
			currentPoint = FPoint(x2 - docX, docHeight - (y2 - docY));
		}
		else if ((command == "V") || (command == "v"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y >> x2 >> y2;
			Coords.svgCurveToCubic(currentPoint.x(), currentPoint.y(), x - docX, docHeight - (y - docY), x2 - docX, docHeight - (y2 - docY));
			currentPoint = FPoint(x2 - docX, docHeight - (y2 - docY));
		}
/* End Path construction commands */
/* Start Object creation commands */
		else if ((command == "b") || (command == "B") || (command == "f") || (command == "F") || (command == "s") || (command == "S"))
		{
			FPoint wh = Coords.WidthHeight();
			if ((Coords.size() > 3) && (wh.x() != 0.0) && (wh.y() != 0.0))
			{
				if ((!WasU) || ((WasU) && (FirstU)))
				{
					if ((command == "B") || (command == "F") || (command == "S"))
					{
						if (command == "F")
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CommonStrings::None, true);
						else if (command == "B")
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
						else
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
					}
					else
					{
						if (command == "f")
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CommonStrings::None, true);
						else if (command == "b")
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
						else
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
					}
					ite = m_Doc->Items->at(z);
					ite->PoLine = Coords.copy();
					ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->ClipEdited = true;
					ite->FrameType = 3;
					ite->setFillShade(CurrFillShade);
					ite->setLineShade(CurrStrokeShade);
					ite->setFillEvenOdd(fillRule);
					ite->setFillTransparency(1.0 - Opacity);
					ite->setLineTransparency(1.0 - Opacity);
	#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
					ite->setFillBlendmode(blendMode);
					ite->setLineBlendmode(blendMode);
	#endif
					if (!currentPatternName.isEmpty())
					{
						ite->setPattern(currentPatternName);
						ite->setPatternTransform(currentPatternXScale * 100, currentPatternYScale * 100, currentPatternX, currentPatternY, currentPatternRotation, 0.0, 0.0);
						ite->GrType = 8;
						currentPatternName = "";
					}
					if (!currentStrokePatternName.isEmpty())
					{
						ite->setStrokePattern(currentStrokePatternName);
						ite->setStrokePatternTransform(currentStrokePatternXScale * 100, currentStrokePatternYScale * 100, currentStrokePatternX, currentStrokePatternY, currentStrokePatternRotation, 0.0, 0.0, 1.0);
						currentStrokePatternName = "";
					}
					ite->setLineEnd(CapStyle);
					ite->setLineJoin(JoinStyle);
					if (!WasU)
					{
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(),wh.y());
						ite->setTextFlowMode(PageItem::TextFlowDisabled);
						m_Doc->AdjustItemSize(ite);
					}
					if (patternMode)
						PatternElements.append(ite);
					else
						Elements.append(ite);
					if (groupStack.count() != 0)
						groupStack.top().append(ite);
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(itemLocked);
					
				}
				else
				{
					ite = m_Doc->Items->at(m_Doc->Items->count()-1);
					ite->PoLine.setMarker();
					Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->PoLine.putPoints(ite->PoLine.size(), Coords.size(), Coords);
				}
				FirstU = false;
				itemRendered = true;
				CurrFillShade = 100.0;
				CurrStrokeShade = 100.0;
			}
			Coords.resize(0);
			Coords.svgInit();
		}
		else if (command == "*u")
		{
			FirstU = true;
			WasU = true;
		}
		else if (command == "*U")
		{
			WasU = false;
			ite = m_Doc->Items->at(m_Doc->Items->count()-1);
			FPoint wh = getMaxClipF(&ite->PoLine);
			ite->setWidthHeight(wh.x(),wh.y());
			m_Doc->AdjustItemSize(ite);
		}
		else if ((command == "u") || (command == "q"))
		{
			QList<PageItem*> gElements;
			groupStack.push(gElements);
			clipStack.push(clipCoords);
		}
		else if ((command == "U") || (command == "Q"))
		{
			if (groupStack.count() != 0)
			{
				QList<PageItem*> gElements = groupStack.pop();
				clipCoords = clipStack.pop();
				tmpSel->clear();
				if (gElements.count() > 0)
				{
					for (int dre = 0; dre < gElements.count(); ++dre)
					{
						tmpSel->addItem(gElements.at(dre), true);
						if (patternMode)
							PatternElements.removeAll(gElements.at(dre));
						else
							Elements.removeAll(gElements.at(dre));
					}
					ite = m_Doc->groupObjectsSelection(tmpSel);
					if ((clipCoords.size() > 4) && (command == "Q"))
					{
						clipCoords.translate(m_Doc->currentPage()->xOffset()-ite->xPos(), m_Doc->currentPage()->yOffset()-ite->yPos());
						ite->PoLine = clipCoords.copy();
						ite->PoLine.translate(baseX, baseY);
					}
					for (int as = 0; as < tmpSel->count(); ++as)
					{
						if (patternMode)
							PatternElements.append(tmpSel->itemAt(as));
						else
							Elements.append(tmpSel->itemAt(as));
					}
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
			if (command == "Q")
			{
				clipCoords.resize(0);
				clipCoords.svgInit();
			}
		}
		else if (command == "W")
		{
			if (clipStack.count() != 0)
			{
				if (clipStack.top().size() > 3)
				{
					clipStack.top().setMarker();
					clipStack.top().putPoints(clipStack.top().size(), Coords.size(), Coords);
				}
				else
					clipStack.top() = Coords.copy();
			}
		}
		else if ((command == "N") || (command == "n"))
		{
			if (command == "n")
				Coords.svgClosePath();
			currentSpecialPath = Coords.copy();
			Coords.resize(0);
			Coords.svgInit();
		}
/* End Object construction commands */
/* Start Graphics state commands */
		else if (command == "A")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 1)
				itemLocked = true;
			else
				itemLocked = false;
		}
		else if (command == "w")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> LineW;
		}
		else if (command == "j")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 0)
				JoinStyle = Qt::MiterJoin;
			else if (tmpInt == 1)
				JoinStyle = Qt::RoundJoin;
			else if (tmpInt == 1)
				JoinStyle = Qt::BevelJoin;
		}
		else if (command == "J")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 0)
				CapStyle = Qt::FlatCap;
			else if (tmpInt == 1)
				CapStyle = Qt::RoundCap;
			else if (tmpInt == 1)
				CapStyle = Qt::SquareCap;
		}
		/* undocumented Command Xy
			- has up to 5 Parameters
			- first Parameter might be the Blendmode
			- second Parameter is the Opacity
		*/
		else if (command == "Xy")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> blendMode >> Opacity;
		}
		else if (command == "XR")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 1)
				fillRule = true;
			else
				fillRule = false;
		}
		else if (command == "Bb")
		{
			gradientMode = true;
			wasBC = false;
			itemRendered = false;
			startMatrix = QTransform();
			endMatrix = QTransform();
		}
		else if (command == "Xm")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			double m1, m2, m3, m4, m5, m6;
			gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			startMatrix.translate(m5, -m6);
			endMatrix.scale(m1, m4);
			wasBC = true;
		}
		else if (command == "Bm")
		{
			if (m_gradients[currentGradientName].type() == 1)
			{
				ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
				double m1, m2, m3, m4, m5, m6;
				gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
				startMatrix.translate(m5, -m6);
//				endMatrix.scale(m1, m4);
				endMatrix *= QTransform(m1, m2, m3, m4, 0, 0);
//				endMatrix = QTransform(m1, m2, m3, m4, m5, m6);
				wasBC = true;
			}
		}
		else if (command == "BB")
		{
			if (itemRendered)
			{
				gradientMode = false;
				ite = m_Doc->Items->at(m_Doc->Items->count()-1);
				ite->fill_gradient = m_gradients[currentGradientName];
				ite->setGradient(currentGradientName);
				if (ite->fill_gradient.type() == 0)
					ite->GrType = 6;
				else
					ite->GrType = 7;
				QTransform m1;
				m1.translate(currentGradientOrigin.x() - ite->xPos(), currentGradientOrigin.y() - ite->yPos());
				m1.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
				m1.rotate(-currentGradientAngle);
				ite->GrStartX = currentGradientOrigin.x() - ite->xPos() + m_Doc->currentPage()->xOffset();
				ite->GrStartY = currentGradientOrigin.y() - ite->yPos() + m_Doc->currentPage()->yOffset();
				QPointF target = m1.map(QPointF(currentGradientLenght, 0.0));
				ite->GrEndX = target.x();
				ite->GrEndY = target.y();
				if (wasBC)
				{
					QPointF newS = startMatrix.map(QPointF(ite->GrStartX, ite->GrStartY));
					ite->GrStartX = newS.x();
					ite->GrStartY = newS.y();
					QTransform m2;
					m2.rotate(-currentGradientAngle);
					m2 *= endMatrix;
					QPointF target = m2.map(QPointF(currentGradientLenght, 0.0));
					ite->GrEndX = target.x();
					ite->GrEndY = target.y();
				}
			}
			wasBC = false;
			currentGradientMatrix = QTransform();
			currentGradientOrigin = QPointF(0.0, 0.0);
			currentGradientAngle = 0.0;
			currentGradientLenght = 1.0;
			itemRendered = false;
		}
		else if (command == "Bg")
		{
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			currentGradientName = Cdata.mid(an+1, en-an-1);
			currentGradientName.remove("\\");
			QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
			ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
			double xOrig, yOrig, m1, m2, m3, m4, m5, m6;
			gVals >> xOrig >> yOrig >> currentGradientAngle >> currentGradientLenght >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			currentGradientOrigin = QPointF(xOrig - docX, docHeight - (yOrig - docY));
			currentGradientMatrix = QTransform(m1, m2, m3, m4, m5, m6);
		}
/* End Graphics state commands */
/* Start Color commands */
		else if ((command == "G") || (command == "g"))
		{
			if (command == "G")
				CurrColorStroke = parseColorGray(Cdata);
			else
				CurrColorFill = parseColorGray(Cdata);
			meshColorMode = 2;
		}
		else if ((command == "K") || (command == "k"))
		{
			if (command == "K")
				CurrColorStroke = parseColor(Cdata);
			else
				CurrColorFill = parseColor(Cdata);
		}
		else if ((command == "XA") || (command == "Xa"))
		{
			QString Xdata = da2[da2.count()-4] + " " + da2[da2.count()-3] + " " + da2[da2.count()-2];
			if (command == "XA")
				CurrColorStroke = parseColorRGB(Xdata);
			else
				CurrColorFill = parseColorRGB(Xdata);
			meshColorMode = 1;
		}
		else if ((command == "XX") || (command == "Xx") || (command == "Xk"))
		{
			if (command == "XX")
				CurrColorStroke = parseCustomColorX(Cdata, CurrStrokeShade, da2[da2.count()-2]);
			else
				CurrColorFill = parseCustomColorX(Cdata, CurrFillShade, da2[da2.count()-2]);
		}
		else if ((command == "X") || (command == "x"))
		{
			if (command == "X")
				CurrColorStroke = parseCustomColor(Cdata, CurrStrokeShade);
			else
				CurrColorFill = parseCustomColor(Cdata, CurrFillShade);
		}
		else if (command == "p")
		{
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			currentPatternName = Cdata.mid(an+1, en-an-1);
			currentPatternName.remove("\\");
			currentPatternName = currentPatternName.trimmed().simplified().replace(" ", "_");
			QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
			ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
			gVals >> currentPatternX >> currentPatternY >> currentPatternXScale >> currentPatternYScale >> currentPatternRotation;
		}
		else if (command == "P")
		{
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			currentStrokePatternName = Cdata.mid(an+1, en-an-1);
			currentStrokePatternName.remove("\\");
			currentStrokePatternName = currentPatternName.trimmed().simplified().replace(" ", "_");
			QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
			ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
			gVals >> currentStrokePatternX >> currentStrokePatternY >> currentStrokePatternXScale >> currentStrokePatternYScale >> currentStrokePatternRotation;
		}
		else if (command == "X!")
		{
			if (Cdata.contains("/Mesh"))
			{
				meshMode = true;
				meshNodeCounter = 0;
//				meshColorMode = 0;
				meshGradientArray.clear();
			}
			if (Cdata.contains("/End"))
			{
				meshMode = false;
				if (meshGradientArray.count() != 0)
				{
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
					ite = m_Doc->Items->at(z);
					for (int x = 0; x < meshGradientArray.count(); x++)
					{
						for (int y = 0; y < meshGradientArray[x].count(); y++)
						{
							meshGradientArray[x][y].moveRel(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
						}
					}
					ite->meshGradientArray = meshGradientArray;
					ite->GrType = 11;
					ite->meshToShape();
					for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
					{
						for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
						{
							meshPoint mp = ite->meshGradientArray[grow][gcol];
							ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
						}
					}
					ite->setFillShade(CurrFillShade);
					ite->setLineShade(CurrFillShade);
					ite->setFillEvenOdd(fillRule);
					ite->setFillTransparency(1.0 - Opacity);
					ite->setLineTransparency(1.0 - Opacity);
	#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
					ite->setFillBlendmode(blendMode);
					ite->setLineBlendmode(blendMode);
	#endif
					ite->setLineEnd(CapStyle);
					ite->setLineJoin(JoinStyle);
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(itemLocked);
					if (patternMode)
						PatternElements.append(ite);
					else
						Elements.append(ite);
					if (groupStack.count() != 0)
						groupStack.top().append(ite);
				}
			}
		}
		else if (command == "X#")
		{
			int an = Cdata.indexOf("_");
			QString cmdLine = Cdata.remove(0, an+1);
			an = cmdLine.lastIndexOf("/");
			QString tmpS = cmdLine.mid(an+1, Cdata.size());
			ScTextStream mVals(&tmpS, QIODevice::ReadOnly);
			QString mKey;
			mVals >> mKey;
			if (mKey == "Size")
			{
				meshGradientArray.clear();
				int ans = cmdLine.indexOf("[");
				int ens = cmdLine.lastIndexOf("]");
				QString sizeVals = cmdLine.mid(ans+1, ens-ans-1);
				ScTextStream mVals2(&sizeVals, QIODevice::ReadOnly);
				mVals2 >> meshXSize >> meshYSize;
				for (int mgr = 0; mgr < meshYSize+1; mgr++)
				{
					QList<meshPoint> ml;
					for (int mgc = 0; mgc < meshXSize+1; mgc++)
					{
						meshPoint mp;
						ml.append(mp);
					}
					meshGradientArray.append(ml);
				}
			}
			if (mKey == "P")
			{
				int ans = cmdLine.indexOf("[");
				int ens = cmdLine.lastIndexOf("]");
				QString posVals = cmdLine.mid(ans+1, ens-ans-1);
				ScTextStream mVals3(&posVals, QIODevice::ReadOnly);
				mVals3 >> currentMeshXPos >> currentMeshYPos;
			}
			if (mKey == "CS")
			{
				if (Cdata.contains("CMYK"))
					meshColorMode = 0;
				else if (Cdata.contains("RGB"))
					meshColorMode = 1;
				else if (Cdata.contains("Gray"))
					meshColorMode = 2;
			}
			if (mKey == "E")
			{
				int indY = meshYSize - currentMeshYPos - 1;
				int indX = currentMeshXPos;
				FPoint p2 = FPoint(meshNode1PointX, meshNode1PointY);
				FPoint p1 = FPoint(meshNode2PointX, meshNode2PointY);
				FPoint p4 = FPoint(meshNode3PointX, meshNode3PointY);
				FPoint p3 = FPoint(meshNode4PointX, meshNode4PointY);
				meshGradientArray[indY+1][indX+1].gridPoint   = FPoint(meshNode2PointX, meshNode2PointY);
				meshGradientArray[indY+1][indX+1].controlTop  = FPoint(meshNode2Control2X, meshNode2Control2Y);
				meshGradientArray[indY+1][indX+1].controlLeft = FPoint(meshNode2Control1X, meshNode2Control1Y);
				meshGradientArray[indY+1][indX+1].colorName = meshColor2;
				meshGradientArray[indY+1][indX+1].shade = 100;
				meshGradientArray[indY+1][indX+1].transparency = 1.0;
				meshGradientArray[indY+1][indX].gridPoint    = FPoint(meshNode1PointX, meshNode1PointY);
				meshGradientArray[indY+1][indX].controlRight = FPoint(meshNode1Control2X, meshNode1Control2Y);
				meshGradientArray[indY+1][indX].controlTop   = FPoint(meshNode1Control1X, meshNode1Control1Y);
				meshGradientArray[indY+1][indX].colorName = meshColor1;
				meshGradientArray[indY+1][indX].shade = 100;
				meshGradientArray[indY+1][indX].transparency = 1.0;
				meshGradientArray[indY][indX].gridPoint     = FPoint(meshNode4PointX, meshNode4PointY);
				meshGradientArray[indY][indX].controlBottom = FPoint(meshNode4Control2X, meshNode4Control2Y);
				meshGradientArray[indY][indX].controlRight  = FPoint(meshNode4Control1X, meshNode4Control1Y);
				meshGradientArray[indY][indX].colorName = meshColor4;
				meshGradientArray[indY][indX].shade = 100;
				meshGradientArray[indY][indX].transparency = 1.0;
				meshGradientArray[indY][indX+1].gridPoint     = FPoint(meshNode3PointX, meshNode3PointY);
				meshGradientArray[indY][indX+1].controlLeft   = FPoint(meshNode3Control2X, meshNode3Control2Y);
				meshGradientArray[indY][indX+1].controlBottom = FPoint(meshNode3Control1X, meshNode3Control1Y);
				meshGradientArray[indY][indX+1].colorName = meshColor3;
				meshGradientArray[indY][indX+1].shade = 100;
				meshGradientArray[indY][indX+1].transparency = 1.0;
				meshNodeCounter = 0;
			}
			if (mKey == "N")
			{
				double cVal, mVal, yVal, kVal, coorX1, coorY1, coorX2, coorY2, coorX3, coorY3;
				int dummy;
				meshNodeCounter++;
				int ans = cmdLine.indexOf("[");
				int ens = cmdLine.lastIndexOf("]");
				QString nodeVals = cmdLine.mid(ans+1, ens-ans-1);
				ScTextStream mVals4(&nodeVals, QIODevice::ReadOnly);
				kVal = 0.0;
				if (meshColorMode == 0)
					mVals4 >> cVal >> mVal >> yVal >> kVal >> coorX1 >> coorY1 >> coorX2 >> coorY2 >> dummy >> coorX3 >> coorY3;
				else if (meshColorMode == 1)
					mVals4 >> cVal >> mVal >> yVal >> coorX1 >> coorY1 >> coorX2 >> coorY2 >> dummy >> coorX3 >> coorY3;
				else if (meshColorMode == 2)
					mVals4 >> cVal >> coorX1 >> coorY1 >> coorX2 >> coorY2 >> dummy >> coorX3 >> coorY3;
				QString nodeColor;
				ScColor tmpColor;
				ColorList::Iterator it;
				int Cc = qRound(cVal * 255);
				int Mc = qRound(mVal * 255);
				int Yc = qRound(yVal * 255);
				int Kc = qRound(kVal * 255);
				int hC, hM, hY, hK;
				bool found = false;
				if (meshColorMode == 0)
				{
					tmpColor.setColor(Cc, Mc, Yc, Kc);
					for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
					{
						if (it.value().getColorModel() == colorModelCMYK)
						{
							it.value().getCMYK(&hC, &hM, &hY, &hK);
							if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
							{
								nodeColor = it.key();
								found = true;
								break;
							}
						}
					}
				}
				else if (meshColorMode == 1)
				{
					tmpColor.setColorRGB(Cc, Mc, Yc);
					for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
					{
						if (it.value().getColorModel() == colorModelRGB)
						{
							it.value().getRGB(&hC, &hM, &hY);
							if ((Cc == hC) && (Mc == hM) && (Yc == hY))
							{
								nodeColor = it.key();
								found = true;
								break;
							}
						}
					}
				}
				else if (meshColorMode == 2)
				{
					tmpColor.setColor(0, 0, 0, Cc);
					for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
					{
						if (it.value().getColorModel() == colorModelCMYK)
						{
							it.value().getCMYK(&hC, &hM, &hY, &hK);
							if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
							{
								nodeColor = it.key();
								found = true;
								break;
							}
						}
					}
				}
				if (!found)
				{
					tmpColor.setSpotColor(false);
					tmpColor.setRegistrationColor(false);
					QString namPrefix = "FromAI";
					m_Doc->PageColors.insert(namPrefix+tmpColor.name(), tmpColor);
					nodeColor = namPrefix+tmpColor.name();
				}
				if (meshNodeCounter == 1)
				{
					meshNode1PointX = coorX1 - docX;
					meshNode1PointY = docHeight - (coorY1 - docY);
					meshNode1Control2X = coorX2 - docX;
					meshNode1Control2Y = docHeight - (coorY2 - docY);
					meshNode1Control1X = coorX3 - docX;
					meshNode1Control1Y = docHeight - (coorY3 - docY);
					meshColor1 = nodeColor;
				}
				if (meshNodeCounter == 2)
				{
					meshNode2PointX = coorX1 - docX;
					meshNode2PointY = docHeight - (coorY1 - docY);
					meshNode2Control2X = coorX2 - docX;
					meshNode2Control2Y = docHeight - (coorY2 - docY);
					meshNode2Control1X = coorX3 - docX;
					meshNode2Control1Y = docHeight - (coorY3 - docY);
					meshColor2 = nodeColor;
				}
				if (meshNodeCounter == 3)
				{
					meshNode3PointX = coorX1 - docX;
					meshNode3PointY = docHeight - (coorY1 - docY);
					meshNode3Control2X = coorX2 - docX;
					meshNode3Control2Y = docHeight - (coorY2 - docY);
					meshNode3Control1X = coorX3 - docX;
					meshNode3Control1Y = docHeight - (coorY3 - docY);
					meshColor3 = nodeColor;
				}
				if (meshNodeCounter == 4)
				{
					meshNode4PointX = coorX1 - docX;
					meshNode4PointY = docHeight - (coorY1 - docY);
					meshNode4Control2X = coorX2 - docX;
					meshNode4Control2Y = docHeight - (coorY2 - docY);
					meshNode4Control1X = coorX3 - docX;
					meshNode4Control1Y = docHeight - (coorY3 - docY);
					meshColor4 = nodeColor;
				}
			}
		}
/* End Color commands */
/* Start Layer commands */
		else if (command == "Lb")
		{
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				int visible, preview, enabled, printing, dummy, rc, gc, bc;
				ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
				ts2 >> visible >> preview >> enabled >> printing >> dummy >> dummy >> dummy >> rc >> gc >> bc;
				if (!firstLayer)
					currentLayer = m_Doc->addLayer("Layer", true);
				m_Doc->setLayerVisible(currentLayer, static_cast<bool>(visible));
				m_Doc->setLayerOutline(currentLayer, static_cast<bool>(!preview));
				m_Doc->setLayerLocked(currentLayer, static_cast<bool>(!enabled));
				m_Doc->setLayerPrintable(currentLayer, static_cast<bool>(printing));
				m_Doc->setLayerMarker(currentLayer, QColor(rc, gc, bc));
				QList<PageItem*> gElements;
				groupStack.push(gElements);
				clipStack.push(clipCoords);
				firstLayer = false;
			}
			Coords.resize(0);
			Coords.svgInit();
		}
		else if (command == "LB")
		{
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				if (groupStack.count() != 0)
				{
					QList<PageItem*> gElements = groupStack.pop();
					clipStack.pop();
					tmpSel->clear();
					if (gElements.count() > 0)
					{
						for (int dre = 0; dre < gElements.count(); ++dre)
						{
							tmpSel->addItem(gElements.at(dre), true);
							if (patternMode)
								PatternElements.removeAll(gElements.at(dre));
							else
								Elements.removeAll(gElements.at(dre));
						}
						m_Doc->groupObjectsSelection(tmpSel);
						ite = tmpSel->itemAt(0);
						if (Coords.size() > 3)
						{
							Coords.translate(m_Doc->currentPage()->xOffset()-ite->xPos(), m_Doc->currentPage()->yOffset()-ite->yPos());
							ite->PoLine = Coords.copy();
							ite->PoLine.translate(baseX, baseY);
						}
						for (int as = 0; as < tmpSel->count(); ++as)
						{
							if (patternMode)
								PatternElements.append(tmpSel->itemAt(as));
							else
								Elements.append(tmpSel->itemAt(as));
						}
						ite->setItemName( tr("Group%1").arg(m_Doc->layerName(currentLayer)));
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
			Coords.resize(0);
			Coords.svgInit();
		}
		else if (command == "Ln")
		{
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				int an = Cdata.indexOf("(");
				int en = Cdata.lastIndexOf(")");
				QString LayerNam = Cdata.mid(an+1, en-an-1);
				LayerNam.remove("\\");
				m_Doc->changeLayerName(currentLayer, LayerNam);
			}
		}
/* End Layer commands */
/* Start Text commands */
		else if (command == "To")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> textMode;
			textData.clear();
			textMatrix = QTransform();
			maxWidth = 0;
			tempW = 0;
			maxHeight = 0;
			textKern = 0;
			startCurrentTextRange = 0;
			endCurrentTextRange = 0;
			textScaleH = 1000;
			textScaleV = 1000;
		}
		else if (command == "Tp")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			double m1, m2, m3, m4, m5, m6;
			gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			textMatrix = QTransform(m1, m2, m3, m4, m5, m6);
		}
		else if (command == "Tx") // || (command == "TX"))
		{
			QStringList res = getStrings(Cdata);
			if (res.count() > 0)
			{
				QString tex = res[0];
				double tempH = 0;
				startCurrentTextRange = textData.length();
				for (int tt = 0; tt < tex.length(); ++tt)
				{
					CharStyle nstyle;
					QString ch = tex.mid(tt,1);
					nstyle.setFont((*m_Doc->AllFonts)[textFont]);
					nstyle.setFontSize(textSize);
					nstyle.setFillColor(CurrColorFill);
					nstyle.setTracking(textKern);
					nstyle.setFillShade(100);
					nstyle.setStrokeColor(CurrColorStroke);
					nstyle.setStrokeShade(100);
					nstyle.setScaleH(textScaleH);
					nstyle.setScaleV(textScaleV);
					nstyle.setBaselineOffset(0);
					nstyle.setShadowXOffset(50);
					nstyle.setShadowYOffset(-50);
					nstyle.setOutlineWidth(10);
					nstyle.setUnderlineOffset(-1);
					nstyle.setUnderlineWidth(-1);
					nstyle.setStrikethruOffset(-1);
					nstyle.setStrikethruWidth(-1);
					nstyle.setFeatures(StyleFlag(ScStyle_Default).featureList());
					int pot = textData.length();
					textData.insertChars(pot, ch);
					textData.applyCharStyle(pot, 1, nstyle);
					tempW += nstyle.font().realCharWidth(ch[0], nstyle.fontSize() / 10.0)+1;
					tempH  = qMax(tempH, nstyle.font().height(nstyle.fontSize() / 10.0) + 2.0);
					maxWidth  = qMax(tempW, maxWidth);
					maxHeight = qMax(tempH, maxHeight);
					if ((ch == SpecialChars::PARSEP) || (ch == SpecialChars::LINEBREAK))
					{
						maxHeight += nstyle.font().height(nstyle.fontSize() / 10.0);
						tempW = 0;
					}
					endCurrentTextRange = pot;
				}
			}
		}
		else if (command == "Tk")
		{
			int flag;
			double val;
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> flag >> val;
			if (flag == 1)
				val = 0;
			double oldval = textData.charStyle(startCurrentTextRange).tracking();
			CharStyle nstyle = textData.charStyle(startCurrentTextRange);
			nstyle.setTracking(oldval + val);
			textData.applyCharStyle(startCurrentTextRange, 1, nstyle);
		}
		else if (command == "Tc")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> textKern;
			textKern *= 100.0;
		}
		else if (command == "Tz")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> textScaleH >> textScaleV;
			textScaleH *= 10.0;
			textScaleV *= 10.0;
		}
		else if (command == "T*")
		{
			CharStyle nstyle;
			QString ch = SpecialChars::LINEBREAK;
			nstyle.setFont((*m_Doc->AllFonts)[textFont]);
			nstyle.setFontSize(textSize);
			nstyle.setFillColor(CurrColorFill);
			nstyle.setTracking(textKern);
			nstyle.setFillShade(100);
			nstyle.setStrokeColor(CurrColorStroke);
			nstyle.setStrokeShade(100);
			nstyle.setScaleH(textScaleH);
			nstyle.setScaleV(textScaleV);
			nstyle.setBaselineOffset(0);
			nstyle.setShadowXOffset(50);
			nstyle.setShadowYOffset(-50);
			nstyle.setOutlineWidth(10);
			nstyle.setUnderlineOffset(-1);
			nstyle.setUnderlineWidth(-1);
			nstyle.setStrikethruOffset(-1);
			nstyle.setStrikethruWidth(-1);
			nstyle.setFeatures(StyleFlag(ScStyle_Default).featureList());
			int pot = textData.length();
			textData.insertChars(pot, ch);
			textData.applyCharStyle(pot, 1, nstyle);
			maxHeight += nstyle.font().height(nstyle.fontSize() / 10.0) + 2.0;
			tempW = 0;
		}
		else if (command == "Tf")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> textFont >> textSize;
			textFont.remove(0, 2);
			QString family = textFont;
			QString ret = "";
			family.replace( QRegExp( "'" ) , QChar( ' ' ) );
			textFont = m_Doc->itemToolPrefs().textFont;
			bool found = false;
			SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
			for ( ; it.hasNext(); it.next())
			{
				QString fam;
				QString fn = it.current().scName();
				int pos = fn.indexOf(" ");
				fam = fn.left(pos);
				if (fam == family)
				{
					found = true;
					ret = fn;
				}
			}
			if (found)
				textFont = family;
			else
			{
				if (importerFlags & LoadSavePlugin::lfCreateThumbnail)
					textFont = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
				else
				{
					if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(family))
					{
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						MissingFont *dia = new MissingFont(0, family, m_Doc);
						dia->exec();
						QString tmpf = dia->getReplacementFont();
						delete dia;
						qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
						PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family] = tmpf;
					}
					else
						textFont = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family];
				}
			}
			textSize *= 10.0;
		}
		else if (command == "TO")
		{
			if (textData.length() > 0)
			{
				if (!((textData.length() == 1) && (textData.text(0) == SpecialChars::PARSEP)))
				{
					QPointF pos = QPointF(textMatrix.dx(), textMatrix.dy());
					pos += QPointF(m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
					pos += QPointF(0.0, textSize / 10.0 + 2.0);
					z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, pos.x() - docX, docHeight - (pos.y() - docY), 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
					ite = m_Doc->Items->at(z);
					ite->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
					ite->itemText.append(textData);
					double xpos = ite->xPos();
					double ypos = ite->yPos();
					ite->setWidthHeight(qMax(ite->width(), maxWidth), qMax(ite->height(), maxHeight));
					double xoffset = 0.0, yoffset = 0.0;
					double rotation = getRotationFromMatrix(textMatrix, 0.0);
					if (rotation != 0.0)
					{
						double temp = textSize / 10.0 + 2.0;
						xoffset = sin(rotation) * temp;
						yoffset = cos(rotation) * temp;
					}
					ite->setXPos(xpos + xoffset);
					ite->setYPos(ypos + yoffset);
					ite->setRotation(rotation * 180 / M_PI);
					ite->SetRectFrame();
					m_Doc->setRedrawBounding(ite);
					ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
					ite->setTextFlowMode(PageItem::TextFlowDisabled);
					ite->setFillShade(CurrFillShade);
					ite->setLineShade(CurrStrokeShade);
					ite->setFillEvenOdd(fillRule);
					ite->setFillTransparency(1.0 - Opacity);
					ite->setLineTransparency(1.0 - Opacity);
					ite->setLineEnd(CapStyle);
					ite->setLineJoin(JoinStyle);
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(itemLocked);
					if (patternMode)
						PatternElements.append(ite);
					else
						Elements.append(ite);
					if (groupStack.count() != 0)
						groupStack.top().append(ite);
				}
			}
		}
/* End Text commands */
/* Start special Commands */
		else if (command == "*")
		{
			Coords.resize(0);
			Coords.svgInit();
		}
		else if (command == "[")
		{
			Coords.resize(0);
			Coords.svgInit();
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			if ((an != -1) && (en != -1))
			{
				patternMode = true;
				currentPatternDefName = Cdata.mid(an+1, en-an-1);
				currentPatternDefName.remove("\\");
				currentPatternDefName = currentPatternDefName.trimmed().simplified().replace(" ", "_");
				QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
				ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
				gVals >> patternX1 >> patternY1 >> patternX2 >> patternY2;
			}
		}
		else if (command == ",")
		{
			if (Cdata.contains("/Data"))
			{
				dataMode = true;
				dataString = "";
			}
		}
		else if (command == ":")
		{
			fObjectMode = true;
		}
/* End special Commands */
/* Skip everything else */
//		else
//			qDebug() << command;
	}
}

void AIPlug::processGradientData(QString data)
{
	QString command = "";
	QString Cdata = "";
	QStringList da;
	getCommands(data, da);
	for (int a = 0; a < da.count(); a++)
	{
		Cdata = da[a];
		QStringList da2 = Cdata.split(" ", QString::SkipEmptyParts);
		command = da2[da2.count()-1];
		if (command == "Bd")
		{
			QString tmpS = Cdata;
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			currentGradientName = Cdata.mid(an+1, en-an-1);
			currentGradientName.remove("\\");
			if (da2[da2.count()-3] == "0")
				currentGradient = VGradient(VGradient::linear);
			else
				currentGradient = VGradient(VGradient::radial);
			currentGradient.clearStops();
		}
		else if ((command == "%_Bs") || (command == "%_BS"))
		{
			QString stopName = "";
			double stop = ScCLocale::toDoubleC(da2[da2.count()-2]) / 100.0;
			double colorShade = 100.0;
			int colortype = da2[da2.count()-4].toInt();
			if (colortype == 0)
			{
				stopName = parseColorGray(Cdata);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
			else if (colortype == 1)
			{
				stopName = parseColor(Cdata);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
			else if (colortype == 2)
			{
				stopName = parseColor(Cdata);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
			else if (colortype == 3)
			{
				stopName = parseCustomColor(Cdata, colorShade);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, qRound(colorShade));
			}
			else if (colortype == 4)
			{
				stopName = parseCustomColorX(Cdata, colorShade, "0");
				const ScColor& gradC = m_Doc->PageColors[stopName];
				currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, qRound(colorShade));
			}
			else if (colortype == 6)
			{
				stopName = parseColor(Cdata);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
		}
		else if (command == "BD")
		{
			m_gradients.insert(currentGradientName, currentGradient);
			if (m_Doc->addGradient(currentGradientName, currentGradient))
				importedGradients.append(currentGradientName);
			currentGradient = VGradient(VGradient::linear);
			currentGradient.clearStops();
			currentGradient.setRepeatMethod( VGradient::none );
			currentGradientName = "";
		}
	}
}

void AIPlug::processPattern(QDataStream &ts)
{
	QString tmp = "";
	QString tmpData = "";
	while (!ts.atEnd())
	{
		tmp = removeAIPrefix(readLinefromDataStream(ts));
		if (importerFlags & LoadSavePlugin::lfKeepPatterns)
		{
			if (tmp.startsWith("%_"))
				tmp.remove(0, 2);
		}
		if (patternMode)
		{
			if (tmp == "EndPattern")
			{
				tmpSel->clear();
				if (PatternElements.count() > 0)
				{
					for (int dre = 0; dre < PatternElements.count(); ++dre)
					{
						tmpSel->addItem(PatternElements.at(dre), true);
						if (groupStack.count() != 0)
							groupStack.top().removeAll(PatternElements.at(dre));
					}
					if (PatternElements.count() > 1)
						m_Doc->itemSelection_GroupObjects(false, false, tmpSel);
					if ((tmpSel->width() > 1) && (tmpSel->height() > 1))
					{
						ScPattern pat = ScPattern();
						pat.setDoc(m_Doc);
						PageItem* currItem = tmpSel->itemAt(0);
						currItem->setItemName(currentPatternDefName);
						m_Doc->DoDrawing = true;
						QImage tmpImg = currItem->DrawObj_toImage(qMax(qRound(patternX2 - patternX1), qRound(patternY2 - patternY1)));
						if (!tmpImg.isNull())
						{
							QImage retImg = QImage(qRound(patternX2 - patternX1), qRound(patternY2 - patternY1), QImage::Format_ARGB32_Premultiplied);
							retImg.fill( qRgba(255, 255, 255, 0) );
							QPainter p;
							p.begin(&retImg);
							if (PatternElements.count() > 1)
								p.drawImage(qRound(-patternX1), qRound(-patternY1), tmpImg);
							else
								p.drawImage(0, 0, tmpImg);
							p.end();
							pat.pattern = retImg;
		//					pat.pattern = currItem->DrawObj_toImage();
							m_Doc->DoDrawing = false;
							//			pat.width = currItem->gWidth;
							//			pat.height = currItem->gHeight;
							pat.width = patternX2 - patternX1;
							pat.height = patternY2 - patternY1;
							pat.xoffset = -patternX1;
							pat.yoffset = -patternY1;
							for (int as = 0; as < tmpSel->count(); ++as)
							{
								PageItem* Neu = tmpSel->itemAt(as);
								Neu->moveBy(-patternX1, -patternY1, true);
								Neu->gXpos -= patternX1;
								Neu->gYpos -= patternY1;
								pat.items.append(Neu);
							}
							m_Doc->addPattern(currentPatternDefName, pat);
							importedPatterns.append(currentPatternDefName);
						}
					}
					m_Doc->itemSelection_DeleteItem(tmpSel);
				}
				PatternElements.clear();
				currentPatternDefName = "";
				break;
			}
			else if (tmp.startsWith("Tile"))
				continue;
			else if (tmp.contains(") @"))
			{
				tmpData += tmp;
				tmpData.remove(") @");
				processData(tmpData);
				tmpData = "";
			}
			else if (tmp.contains(") &"))
			{
				tmpData += tmp;
				tmpData.remove(") &");
				processData(tmpData);
				tmpData = "";
			}
			else if (tmp.startsWith("("))
			{
				if (tmp.startsWith("("))
					tmp.remove(0, 1);
				tmpData += " "+tmp;
			}
			else
				processData(tmp);
		}
		else if (tmp == "EndPattern")
		{
			PatternElements.clear();
			currentPatternDefName = "";
			break;
		}
		else if (tmp.contains("BeginRaster") && (tmp.startsWith("%")))
		{
			while (!ts.atEnd())
			{
				tmp = readLinefromDataStream(ts);
				if (tmp.contains("EndRaster"))
					break;
				if(progressDialog)
				{
					progressDialog->setProgress("GI", ts.device()->pos());
					qApp->processEvents();
				}
			}
		}
		else
		{
			Coords.resize(0);
			Coords.svgInit();
			int an = tmp.indexOf("(");
			int en = tmp.lastIndexOf(")");
			if ((an != -1) && (en != -1))
			{
				patternMode = true;
				currentPatternDefName = tmp.mid(an+1, en-an-1);
				currentPatternDefName.remove("\\");
				currentPatternDefName = currentPatternDefName.trimmed().simplified().replace(" ", "_");
				QString tmpS = tmp.mid(en+1, tmp.size() - en);
				ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
				gVals >> patternX1 >> patternY1 >> patternX2 >> patternY2;
			}
		}
	//		processData(tmp);
	}
	patternMode = false;
}

void AIPlug::processSymbol(QDataStream &ts, bool sym)
{
	QString tmp = "";
	QString tmpData = "";
	while (!ts.atEnd())
	{
		tmp = removeAIPrefix(readLinefromDataStream(ts));
		if (!patternMode)
		{
			int an = tmp.indexOf("(");
			int en = tmp.lastIndexOf(")");
			if ((an != -1) && (en != -1))
			{
				patternMode = true;
				currentPatternDefName = tmp.mid(an+1, en-an-1);
				currentPatternDefName.remove("\\");
				if (sym)
					currentPatternDefName = "S_"+currentPatternDefName.trimmed().simplified().replace(" ", "_");
				else
					currentPatternDefName = currentPatternDefName.trimmed().simplified().replace(" ", "_");
			}
		}
		else if ((tmp == "EndSymbol") || (tmp == "EndBrushPattern"))
		{
			tmpSel->clear();
			if (PatternElements.count() > 0)
			{
				for (int dre = 0; dre < PatternElements.count(); ++dre)
				{
					tmpSel->addItem(PatternElements.at(dre), true);
					groupStack.top().removeAll(PatternElements.at(dre));
				}
				if (PatternElements.count() > 1)
					m_Doc->itemSelection_GroupObjects(false, false, tmpSel);
				if ((tmpSel->width() > 1) && (tmpSel->height() > 1))
				{
					ScPattern pat = ScPattern();
					pat.setDoc(m_Doc);
					PageItem* currItem = tmpSel->itemAt(0);
					currItem->setItemName(currentPatternDefName);
					m_Doc->DoDrawing = true;
					pat.pattern = currItem->DrawObj_toImage(qMax(tmpSel->width(), tmpSel->height()));
					if (!pat.pattern.isNull())
					{
						pat.width = currItem->gWidth;
						pat.height = currItem->gHeight;
						m_Doc->DoDrawing = false;
						for (int as = 0; as < tmpSel->count(); ++as)
						{
							PageItem* Neu = tmpSel->itemAt(as);
							pat.items.append(Neu);
						}
						importedPatterns.append(currentPatternDefName);
						importedSymbols.insert(currentPatternDefName, QPointF(currItem->xPos(), currItem->yPos()));
						m_Doc->addPattern(currentPatternDefName, pat);
					}
				}
				m_Doc->itemSelection_DeleteItem(tmpSel);
			}
			PatternElements.clear();
			currentPatternDefName = "";
			break;
		}
		else if (tmp.contains("BeginRaster") && (tmp.startsWith("%")))
		{
			while (!ts.atEnd())
			{
				tmp = readLinefromDataStream(ts);
				if (tmp.contains("EndRaster"))
					break;
				if(progressDialog)
				{
					progressDialog->setProgress("GI", ts.device()->pos());
					qApp->processEvents();
				}
			}
		}
		else
		{
			processData(tmp);
		}
	}
	patternMode = false;
}

void AIPlug::processRaster(QDataStream &ts)
{
	double m1, m2, m3, m4, m5, m6, x1, y1, x2, y2, dummy;
	int w, h, type, alpha, bin, bits;
	QString tmp = "";
	QString cumulated = "";
	while (!ts.atEnd())
	{
		tmp = readLinefromDataStream(ts);
		if (tmp.startsWith("%"))
			break;
		tmp.remove("[");
		tmp.remove("]");
		if (!tmp.isEmpty())
			cumulated += " " + tmp;
	}
	QString Cdata = "";
	QStringList da;
	getCommands(cumulated, da);
	Cdata = da[da.count()-1];
	ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
	gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6 >> x1 >> y1 >> x2 >> y2 >> w >> h >> bits >> type >> alpha >> dummy >> bin;
//	qDebug() << QString("Matrix: %1 %2 %3 %4 %5 %6").arg(m1).arg(m2).arg(m3).arg(m4).arg(m5).arg(m6);
//	qDebug() << QString("Bounds: %1 %2 %3 %4").arg(x1).arg(y1).arg(x2).arg(y2);
//	qDebug() << QString("Size: %1 %2").arg(w).arg(h);
//	qDebug() << QString("Bits: %1").arg(bits);
//	qDebug() << QString("Typ: %1").arg(type);
//	qDebug() << QString("Alpha-Channels: %1").arg(alpha);
//	qDebug() << QString("Encoding: %1").arg(bin);
	uint dataSize = w * h * (type + alpha);
	uint alphaData = w * h * type;
	bool cmyk = false;
	if (type == 4)
		cmyk = true;
	if (tmp.startsWith("%%BeginData"))
	{
		QString dummyS;
		ScTextStream gVals2(&tmp, QIODevice::ReadOnly);
		tmp = readLinefromDataStream(ts);
	}
	QByteArray psdata;
	psdata.resize(dataSize);
	RawImage m_image;
	if (type == 4)
	{
		if (alpha > 0)
			m_image.create(w, 1, 5);
		else
			m_image.create(w, 1, 4);
	}
	else
	{
		if (alpha > 0)
			m_image.create(w, 1, 4);
		else
			m_image.create(w, 1, 3);
	}
	bool first = false;
	if (bin == 0) // 0 = ASCII encoded data
	{
		uint dataPointer = 0;
		while (!ts.atEnd())
		{
			if (first)
				tmp = readLinefromDataStream(ts);
			first = true;
			if (tmp.startsWith("%AI5_EndRaster"))
				break;
			for (int a = 1; a < tmp.length(); a += 2)
			{
				bool ok;
				ushort data = tmp.mid(a, 2).toUShort(&ok, 16);
				psdata[dataPointer++] = data;
			}
		}
	}
	else
	{
		psdata.resize(dataSize);
		ts.readRawData(psdata.data(), dataSize);
	}
	QTransform imgMatrix = QTransform(m1, m2, m3, m4, m5, m6);
	QPointF pos = QPointF(imgMatrix.dx(), imgMatrix.dy());
	pos += QPointF(m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
	pos += QPointF(baseX, -baseY);
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, pos.x() - docX, docHeight - (pos.y() - docY), 10, 10, 0, CurrColorFill, CurrColorStroke, true);
	PageItem* ite = m_Doc->Items->at(z);
	ite->setWidthHeight(fabs(w * m1), fabs(h * m4));
	double rotation = getRotationFromMatrix(imgMatrix, 0.0);
	ite->setRotation(rotation * 180 / M_PI);
	ite->SetRectFrame();
	m_Doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(CurrStrokeShade);
	ite->setFillEvenOdd(fillRule);
	ite->setFillTransparency(1.0 - Opacity);
	ite->setLineTransparency(1.0 - Opacity);
#if (CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4))
	ite->setFillBlendmode(blendMode);
	ite->setLineBlendmode(blendMode);
#endif
	ite->setLineEnd(CapStyle);
	ite->setLineJoin(JoinStyle);
	uchar *p;
	uint yCount = 0;
	ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_ai_XXXXXX.tif");
	ite->tempImageFile->open();
	QString imgName = getLongPathName(ite->tempImageFile->fileName());
	ite->tempImageFile->close();
	ite->isInlineImage = true;
	TIFF* tif = TIFFOpen(imgName.toLocal8Bit().data(), "w");
	if (tif)
	{
		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, m_image.channels());
		TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		if (type == 4)
			TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_SEPARATED);
		else
			TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
		for (int y = 0; y < h; ++y)
		{
			p = m_image.scanLine( 0 );
			for (int xh = 0; xh < m_image.width(); ++xh )
			{
				p[0] = psdata[yCount++];
				if (type > 1)
				{
					p[1] = psdata[yCount++];
					p[2] = psdata[yCount++];
					if (type == 4)
						p[3] = psdata[yCount++];
				}
				else
				{
					p[1] = p[0];
					p[2] = p[0];
				}
				if (alpha == 1)
				{
					if (type == 4)
						p[4] = psdata[alphaData++];
					else
						p[3] = psdata[alphaData++];
				}
				p += m_image.channels();
			}
			TIFFWriteScanline(tif, m_image.scanLine(0), y);
		}
		TIFFClose(tif);
	}
	m_Doc->LoadPict(imgName, z);
	if (ite->PictureIsAvailable)
		ite->setImageXYScale(ite->width() / ite->pixm.width(), ite->height() / ite->pixm.height());
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
		ite->setLocked(itemLocked);
	if (patternMode)
		PatternElements.append(ite);
	else
		Elements.append(ite);
	if (groupStack.count() != 0)
		groupStack.top().append(ite);
}

void AIPlug::processComment(QDataStream &ts, QString comment)
{
	QString tmp = removeAIPrefix(comment);
	if (tmp.startsWith("Begin_NonPrinting"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("BeginGradient"))
			{
				while (!ts.atEnd())
				{
					tmp = removeAIPrefix(readLinefromDataStream(ts));
					if (tmp.startsWith("EndGradient"))
						break;
					else
						processGradientData(tmp);
				}
			}
			if (tmp.startsWith("BeginPattern:"))
				processPattern(ts);
			if (tmp == "BeginSymbol")
				processSymbol(ts, true);
			if (tmp == "BeginBrushPattern")
				processSymbol(ts, false);
			if (tmp.startsWith("End_NonPrinting"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginPattern:"))
		processPattern(ts);
	else if (tmp.startsWith("BeginGradient"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndGradient"))
				break;
			else
				processGradientData(tmp);
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginPalette"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndPalette"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp == "BeginSymbol")
		processSymbol(ts, true);
	else if (tmp == "BeginBrushPattern")
		processSymbol(ts, false);
/*	{
		while (!ts.atEnd())
		{
			processSymbol(ts);
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndSymbol"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	} */
	else if (tmp.startsWith("BeginDocumentData"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndDocumentData"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginTextDocument"))
	{
		QString dataStringT = "";
		tmp = removeAIPrefix(readLinefromDataStream(ts));
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndTextDocument"))
			{
			//	QByteArray fData;
			//	decodeA85(fData, dataStringT);
				break;
			}
		//	else
		//		dataStringT += tmp.mid(1);
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("%%BeginProlog"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("%%EndProlog"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("%%BeginData"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("%%EndData"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("%%BeginCrops"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("%%EndCrops"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginRaster"))
	{
		processRaster(ts);
		if(progressDialog)
		{
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.contains("BeginRaster") && (tmp.startsWith("%")))
	{
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp.contains("EndRaster"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginSVGFilter"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndSVGFilter"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginArtStyles"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndArtStyles"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginPluginObject"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndPluginObject"))
				break;
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginLayer"))
	{
		while (!ts.atEnd())
		{
			QString rl = readLinefromDataStream(ts);
			tmp = removeAIPrefix(rl);
			if (tmp.startsWith("BeginRaster"))
			{
				processRaster(ts);
				continue;
			}
			if (tmp.startsWith("EndLayer"))
				break;
			else
				processData(rl);
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
}

bool AIPlug::convert(QString fn)
{
	QString tmp;
	LineW = 1.0;
	Opacity = 1.0;
	blendMode = 0;
	CurrColorFill = "White";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	JoinStyle = Qt::MiterJoin;
	CapStyle = Qt::FlatCap;
	DashPattern.clear();
	DashOffset = 0.0;
	fillRule = false;
	FirstU = false;
	WasU = false;
	firstLayer = true;
	patternMode = false;
	symbolMode = false;
	meshMode = false;
	dataMode = false;
	fObjectMode = false;
	dataString = "";
	itemLocked = false;
	patternX1 = 0.0;
	patternY1 = 0.0;
	patternX2 = 0.0;
	patternY2 = 0.0;
	Coords.resize(0);
	Coords.svgInit();
	clipCoords.resize(0);
	clipCoords.svgInit();
	currentSpecialPath.resize(0);
	currentSpecialPath.svgInit();
	currentPoint = FPoint(0.0, 0.0);
	currentLayer = 0;
	currentGradient = VGradient(VGradient::linear);
	currentGradient.clearStops();
	currentGradient.setRepeatMethod( VGradient::none );
	currentGradientName = "";
	currentGradientMatrix = QTransform();
	currentGradientOrigin = QPointF(0.0, 0.0);
	currentGradientAngle = 0.0;
	currentGradientLenght = 1.0;
	currentPatternName = "";
	currentPatternX = 0.0;
	currentPatternY = 0.0;
	currentPatternXScale = 1.0;
	currentPatternYScale = 1.0;
	currentPatternRotation = 0.0;
	currentStrokePatternName = "";
	currentStrokePatternX = 0.0;
	currentStrokePatternY = 0.0;
	currentStrokePatternXScale = 1.0;
	currentStrokePatternYScale = 1.0;
	currentStrokePatternRotation = 0.0;
	QList<PageItem*> gElements;
	groupStack.push(gElements);
	clipStack.push(clipCoords);
	commandList << "m" << "l" << "L" << "c" << "C" << "v" << "V" << "y" << "Y";		// Path construction
	commandList << "b" << "B" << "f" << "F" << "s" << "S" << "*u" << "*U";			// Object creation
	commandList << "u" << "U" << "W" << "q" << "Q";									// Object creation
	commandList << "A" << "w" << "j" << "J" << "Xy" << "XR";						// Graphic state
	commandList << "k" << "K" << "Xa" << "XA" << "x" << "X" << "XX" << "Xx";		// Color commands
	commandList << "Xk" << "g" << "G" << "p" << "P";								// Color commands
	commandList << "Ln" << "Lb" << "LB";											// Layer commands
	commandList << "Bd" << "BD" << "%_Bs" << "Bg" << "Bb" << "BB" << "Bm" << "Xm";	// Gradient commands
	commandList << "To" << "TO" << "Tf" << "Tp" << "Tx" << "TX" << "T*" << "Tk";	// Text commands
	commandList << "Tc" << "Tz";													// Text commands
	commandList << "XI" << "XG" << "Xh";											// Image commands
	commandList << "n" << "N" << "*" << "[";										// Special commands
	commandList << "X!" << "X#";													// Mesh commands
	commandList << "M" << "d" << "D" << "E";										// unimplemented
	commandList << "h" << "H" << "i" << "I" << "Np" << "O";							// unimplemented
	commandList << "P" << "R";														// unimplemented
	commandList << "XI" << "XF" << "XG" << "XT" << "Z" << "`" << "~" << "_" << "@";	// unimplemented
	commandList << "&" << "*w" << "*W" << "Ap" << "Ar";								// unimplemented
	if(progressDialog)
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
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp.startsWith("%"))
				processComment(ts, tmp);
			else
				processData(tmp);
			if(progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
		f.close();
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}
