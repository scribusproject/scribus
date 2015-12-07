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

#include "importai.h"

#include "commonstrings.h"
#include "loadsaveplugin.h"
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
#include "scpattern.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
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
#include "util_ghostscript.h"
#include "util_math.h"

#include <cairo.h>

#ifdef HAVE_PODOFO
	#include <podofo/podofo.h>
#endif

extern SCRIBUS_API ScribusQApp * ScQApp;

AIPlug::AIPlug(ScribusDoc* doc, int flags)
{
	m_tmpSel=new Selection(this, false);
	m_Doc=doc;
	m_importerFlags = flags;
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_progressDialog = NULL;
}

QImage AIPlug::readThumbnail(QString fNameIn)
{
	QString fName = fNameIn;
	double x, y, b, h;
	m_CustColors.clear();
	m_importedColors.clear();
	m_importedGradients.clear();
	m_importedPatterns.clear();
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
			QString pdfFile = QDir::toNativeSeparators(fName);
			QString tmpFile = QDir::toNativeSeparators(ScPaths::getTempFileDir() + "sc.png");
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
	m_progressDialog = NULL;
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
	m_docX = x;
	m_docY = y;
	m_docWidth = b - x;
	m_docHeight = h - y;
	m_baseX = 0;
	m_baseY = 0;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	m_baseX = m_Doc->currentPage()->xOffset();
	m_baseY = m_Doc->currentPage()->yOffset();
	ColorList::Iterator it;
	for (it = m_CustColors.begin(); it != m_CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
		{
			m_Doc->PageColors.insert(it.key(), it.value());
			m_importedColors.append(it.key());
		}
	}
	m_Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	QImage tmpImage;
	if (convert(fName))
	{
		m_tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (m_Elements.count() > 1)
			m_Doc->groupObjectsList(m_Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		for (int dre=0; dre<m_Elements.count(); ++dre)
		{
			m_tmpSel->addItem(m_Elements.at(dre), true);
		}
		m_tmpSel->setGroupRect();
		double xs = m_tmpSel->width();
		double ys = m_tmpSel->height();
		if (m_Elements.count() > 0)
			tmpImage = m_Elements.at(0)->DrawObj_toImage(500);
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
	m_cancel = false;
	double x, y, b, h;
	m_convertedPDF = false;
	m_CustColors.clear();
	m_importedColors.clear();
	m_importedGradients.clear();
	m_importedPatterns.clear();
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
			m_convertedPDF = true;
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
	m_progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	parseHeader(fName, x, y, b, h);
	m_docX = x;
	m_docY = y;
	m_docWidth = b - x;
	m_docHeight = h - y;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	m_baseX = m_Doc->currentPage()->xOffset();
	m_baseY = m_Doc->currentPage()->yOffset();
	ColorList::Iterator it;
	for (it = m_CustColors.begin(); it != m_CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
		{
			m_Doc->PageColors.insert(it.key(), it.value());
			m_importedColors.append(it.key());
		}
	}
	m_Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	convert(fName);
	if (m_importedColors.count() != 0)
	{
		colors = m_Doc->PageColors;
		success = true;
	}
	m_Doc->scMW()->setScriptRunning(false);
	m_Doc->setLoading(false);
	delete m_Doc;
	QDir::setCurrent(CurDirP);
	if (m_convertedPDF)
		QFile::remove(fName);
	return success;
}

bool AIPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_importerFlags = flags;
	m_cancel = false;
	double x, y, b, h;
	bool ret = false;
	m_convertedPDF = false;
	m_CustColors.clear();
	m_importedColors.clear();
	m_importedGradients.clear();
	m_importedPatterns.clear();
	QFileInfo fi = QFileInfo(fName);
//	QString ext = fi.suffix().toLower();
	if ( !ScCore->usingGUI() )
	{
		m_interactive = false;
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
			m_convertedPDF = true;
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
	x = 0.0;
	y = 0.0;
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	if (m_progressDialog)
	{
		m_progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	parseHeader(fName, x, y, b, h);
	if (b == 0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	m_docX = x;
	m_docY = y;
	m_docWidth = b - x;
	m_docHeight = h - y;
	m_baseX = 0;
	m_baseY = 0;
	if (!m_interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		m_baseX = 0;
		m_baseY = 0;
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			m_baseX = 0;
			m_baseY = 0;
		}
	}
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->documentInfo().setAuthor(m_docCreator);
		m_Doc->documentInfo().setPublisher(m_docOrganisation);
		m_Doc->documentInfo().setTitle(m_docTitle);
		m_Doc->documentInfo().setDate(m_docDate+" "+m_docTime);
	}
	if ((!ret) && (m_interactive))
	{
		m_baseX = m_Doc->currentPage()->xOffset();
		m_baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!m_interactive))
	{
		if (b-x > h-y)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	ColorList::Iterator it;
	for (it = m_CustColors.begin(); it != m_CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
		{
			m_Doc->PageColors.insert(it.key(), it.value());
			m_importedColors.append(it.key());
		}
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
		if (m_Elements.count() == 0)
		{
			if ((m_importedColors.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns))))
			{
				for (int cd = 0; cd < m_importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(m_importedColors[cd]);
				}
			}
			if ((m_importedGradients.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients || (flags & LoadSavePlugin::lfKeepPatterns)))))
			{
				for (int cd = 0; cd < m_importedGradients.count(); cd++)
				{
					m_Doc->docGradients.remove(m_importedGradients[cd]);
				}
			}
			if ((m_importedPatterns.count() != 0) && (!(flags & LoadSavePlugin::lfKeepPatterns)))
			{
				for (int cd = 0; cd < m_importedPatterns.count(); cd++)
				{
					m_Doc->docPatterns.remove(m_importedPatterns[cd]);
				}
			}
		}
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
				if ((m_importedColors.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns))))
				{
					for (int cd = 0; cd < m_importedColors.count(); cd++)
					{
						m_Doc->PageColors.remove(m_importedColors[cd]);
					}
				}
				if ((m_importedGradients.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients || (flags & LoadSavePlugin::lfKeepPatterns)))))
				{
					for (int cd = 0; cd < m_importedGradients.count(); cd++)
					{
						m_Doc->docGradients.remove(m_importedGradients[cd]);
					}
				}
				if ((m_importedPatterns.count() != 0) && (!(flags & LoadSavePlugin::lfKeepPatterns)))
				{
					for (int cd = 0; cd < m_importedPatterns.count(); cd++)
					{
						m_Doc->docPatterns.remove(m_importedPatterns[cd]);
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
	if (m_interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
	{
		if ((showProgress) && (!m_interactive))
			m_Doc->view()->DrawNew();
	}
	if (m_convertedPDF)
		QFile::remove(fName);
	qApp->restoreOverrideCursor();
	return success;
}

AIPlug::~AIPlug()
{
	if (m_progressDialog)
		delete m_progressDialog;
	delete m_tmpSel;
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
		PoDoFo::PdfError::EnableLogging( false );
		PoDoFo::PdfMemDocument doc( infile.toLocal8Bit().data() );
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
	FILE *source, *dest;
	int ret;
	unsigned have;
	z_stream strm;
	char in[4096];
	char out[4096];

	source = fopen(fName.toLocal8Bit().constData(), "rb");
	if (!source)
		return false;
	fseek(source, 20, SEEK_SET);
	dest = fopen(f2.toLocal8Bit().constData(), "wb");
	if (!dest)
	{
		fclose(source);
		return false;
	}

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);

	if (ret != Z_OK)
	{
		fclose(source);
		fclose(dest);
		return false;
	}
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
	if (!m_convertedPDF)
	{
		QFileInfo bF2(fName);
		QString tmpFile = ScPaths::getTempFileDir()+ "/"+bF2.baseName()+"_tmp.ai";
		moveFile(f2, tmpFile);
		fName = tmpFile;
		m_convertedPDF = true;
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
					m_docCreator = res[0];
					m_docOrganisation = res[1];
				}
			}
			if (tmp.startsWith("%%CreationDate:"))
			{
				QStringList res = getStrings(tmp);
				if (res.count() > 1)
				{
					m_docDate = res[0];
					m_docTime = res[1];
				}
				else
				{
					m_docDate = tmp.remove("%%CreationDate: ");
					m_docTime = "";
				}
			}
			if (tmp.startsWith("%%Title"))
			{
				QStringList res = getStrings(tmp);
				if (res.count() > 0)
					m_docTitle = res[0];
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
						m_CustColors.tryAddColor(FarNam, cc);
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
							m_CustColors.tryAddColor(FarNam, cc);
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
						m_CustColors.tryAddColor(FarNam, cc);
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
							m_CustColors.tryAddColor(FarNam, cc);
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
							m_CustColors.tryAddColor(FarNam, cc);
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
		m_importedColors.append(fNam);
	ret = fNam;
	m_meshColorMode = 0;
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
		m_importedColors.append(fNam);
	ret = fNam;
	m_meshColorMode = 2;
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
		m_importedColors.append(fNam);
	ret = fNam;
	m_meshColorMode = 1;
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
		m_importedColors.append(FarNam);
	ret = fNam;
	m_meshColorMode = 0;
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
		m_meshColorMode = 1;
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
		m_meshColorMode = 0;
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
		m_importedColors.append(FarNam);
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
	//bool arra = false;
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
		//	arra = true;
			tmp2 += tmp;
			continue;
		}
		if (tmp == "]")
		{
		//	arra = false;
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
				if (m_commandList.contains(tmp2))
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
	if (m_dataMode && m_fObjectMode)
	{
		if (data.contains("~>"))
		{
			m_dataString += data.mid(1);
			m_dataMode = false;
			QByteArray fData;
			decodeA85(fData, m_dataString);
			m_dataString = "";
			if (m_fObjectMode)
			{
				FPoint wh = m_currentSpecialPath.WidthHeight();
				if ((m_currentSpecialPath.size() > 3) && (wh.x() != 0.0) && (wh.y() != 0.0))
				{
					z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
					ite = m_Doc->Items->at(z);
					ite->PoLine = m_currentSpecialPath.copy();
					ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->ClipEdited = true;
					ite->FrameType = 3;
					ite->setFillShade(m_CurrFillShade);
					ite->setLineShade(m_CurrStrokeShade);
					ite->setFillEvenOdd(m_fillRule);
					ite->setFillTransparency(1.0 - m_Opacity);
					ite->setLineTransparency(1.0 - m_Opacity);
					ite->setFillBlendmode(m_blendMode);
					ite->setLineBlendmode(m_blendMode);
					ite->setLineEnd(m_CapStyle);
					ite->setLineJoin(m_JoinStyle);
					wh = getMaxClipF(&ite->PoLine);
					ite->setWidthHeight(wh.x(),wh.y());
					ite->setTextFlowMode(PageItem::TextFlowDisabled);
					m_Doc->AdjustItemSize(ite);
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_ai_XXXXXX.pdf");
					tempFile->setAutoRemove(false);
					tempFile->open();
					tempFile->write(fData);
					QString imgName = getLongPathName(tempFile->fileName());
					tempFile->close();
					ite->isInlineImage = true;
					ite->isTempFile = true;
					m_Doc->loadPict(imgName, ite);
					delete tempFile;
					if (ite->imageIsAvailable)
						ite->setImageXYScale(ite->width() / ite->pixm.width(), ite->height() / ite->pixm.height());
					ite->setImageFlippedV(true);
					ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
					ite->setRedrawBounding();
					if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(m_itemLocked);
					if (m_patternMode)
						m_PatternElements.append(ite);
					else
						m_Elements.append(ite);
					if (m_groupStack.count() != 0)
						m_groupStack.top().append(ite);
				}
			}
			m_fObjectMode = false;
			m_currentSpecialPath.resize(0);
			m_currentSpecialPath.svgInit();
		}
		else
		{
			m_dataString += data.mid(1);
		}
		return;
	}
	getCommands(data, da);
	for (int a = 0; a < da.count(); a++)
	{
		Cdata = da[a];
		if (((Cdata.startsWith("%")) || (Cdata.startsWith(" %"))) && (!m_meshMode))
			continue;
		if (Cdata.contains("SymbolInstance"))
		{
			m_symbolMode = true;
			return;
		}
		if (m_symbolMode)
		{
			if (Cdata.contains("SymbolRef"))
			{
				int an = Cdata.indexOf("(");
				int en = Cdata.lastIndexOf(")");
				if ((an != -1) && (en != -1))
				{
					m_currentSymbolName = Cdata.mid(an+1, en-an-1);
					m_currentSymbolName.remove("\\");
					m_currentSymbolName = "S_"+m_currentSymbolName.trimmed().simplified().replace(" ", "_");
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
				QPointF pos1 = m_importedSymbols[m_currentSymbolName];
				pos1 = symT.map(pos1);
				double xp = pos1.x();
				double yp = pos1.y();
			//	xp += m_Doc->currentPage()->xOffset();
			//	yp += m_Doc->currentPage()->yOffset();
				int z = m_Doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, m_baseX + xp, m_baseY + yp, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
				PageItem *b = m_Doc->Items->at(z);
				b->LayerID = m_Doc->activeLayer();
				ScPattern pat = m_Doc->docPatterns[m_currentSymbolName];
				b->setWidth(pat.width * symTrans.m11());
				b->setHeight(pat.height * symTrans.m22());
				b->OldB2 = b->width();
				b->OldH2 = b->height();
				b->setPattern(m_currentSymbolName);
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
				b->setFillTransparency(1.0 - m_Opacity);
				b->setLineTransparency(1.0 - m_Opacity);
				b->setFillBlendmode(m_blendMode);
				b->setLineBlendmode(m_blendMode);
				b->updateClip();
				if (m_patternMode)
					m_PatternElements.append(b);
				else
					m_Elements.append(b);
				if (m_groupStack.count() != 0)
					m_groupStack.top().append(b);
				m_symbolMode = false;
			}
		}
		QStringList da2 = Cdata.split(" ", QString::SkipEmptyParts);
		if (da2.count() == 0)
			return;
		command = da2.last();
/* Start Path construction commands */
		if (command == "m")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y;
			m_Coords.svgMoveTo(x - m_docX, m_docHeight - (y - m_docY));
			m_currentPoint = FPoint(x - m_docX, m_docHeight - (y - m_docY));
		}
		else if ((command == "L") || (command == "l"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y;
			m_Coords.svgLineTo(x - m_docX, m_docHeight - (y - m_docY));
			m_currentPoint = FPoint(x - m_docX, m_docHeight - (y - m_docY));
		}
		else if ((command == "C") || (command == "c"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y >> x1 >> y1 >> x2 >> y2;
			m_Coords.svgCurveToCubic(x - m_docX, m_docHeight - (y - m_docY),
								   x1 - m_docX, m_docHeight - (y1 - m_docY),
								   x2 - m_docX, m_docHeight - (y2 - m_docY));
			m_currentPoint = FPoint(x2 - m_docX, m_docHeight - (y2 - m_docY));
		}
		else if ((command == "Y") || (command == "y"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x1 >> y1 >> x2 >> y2;
			m_Coords.svgCurveToCubic(x1 - m_docX, m_docHeight - (y1 - m_docY), x2 - m_docX, m_docHeight - (y2 - m_docY), x2 - m_docX, m_docHeight - (y2 - m_docY));
			m_currentPoint = FPoint(x2 - m_docX, m_docHeight - (y2 - m_docY));
		}
		else if ((command == "V") || (command == "v"))
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y >> x2 >> y2;
			m_Coords.svgCurveToCubic(m_currentPoint.x(), m_currentPoint.y(), x - m_docX, m_docHeight - (y - m_docY), x2 - m_docX, m_docHeight - (y2 - m_docY));
			m_currentPoint = FPoint(x2 - m_docX, m_docHeight - (y2 - m_docY));
		}
/* End Path construction commands */
/* Start Object creation commands */
		else if ((command == "b") || (command == "B") || (command == "f") || (command == "F") || (command == "s") || (command == "S"))
		{
			FPoint wh = m_Coords.WidthHeight();
			if ((m_Coords.size() > 3) && (wh.x() != 0.0) && (wh.y() != 0.0))
			{
				if ((!m_WasU) || ((m_WasU) && (m_FirstU)))
				{
					if ((command == "B") || (command == "F") || (command == "S"))
					{
						if (command == "F")
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, m_CurrColorFill, CommonStrings::None, true);
						else if (command == "B")
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
						else
							z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, CommonStrings::None, m_CurrColorStroke, true);
					}
					else
					{
						if (command == "f")
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, m_CurrColorFill, CommonStrings::None, true);
						else if (command == "b")
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
						else
							z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, CommonStrings::None, m_CurrColorStroke, true);
					}
					ite = m_Doc->Items->at(z);
					ite->PoLine = m_Coords.copy();
					ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->ClipEdited = true;
					ite->FrameType = 3;
					ite->setFillShade(m_CurrFillShade);
					ite->setLineShade(m_CurrStrokeShade);
					ite->setFillEvenOdd(m_fillRule);
					ite->setFillTransparency(1.0 - m_Opacity);
					ite->setLineTransparency(1.0 - m_Opacity);
					ite->setFillBlendmode(m_blendMode);
					ite->setLineBlendmode(m_blendMode);
					if (!m_currentPatternName.isEmpty())
					{
						ite->setPattern(m_currentPatternName);
						ite->setPatternTransform(m_currentPatternXScale * 100, m_currentPatternYScale * 100, m_currentPatternX, m_currentPatternY, m_currentPatternRotation, 0.0, 0.0);
						ite->GrType = 8;
						m_currentPatternName = "";
					}
					if (!m_currentStrokePatternName.isEmpty())
					{
						ite->setStrokePattern(m_currentStrokePatternName);
						ite->setStrokePatternTransform(m_currentStrokePatternXScale * 100, m_currentStrokePatternYScale * 100, m_currentStrokePatternX, m_currentStrokePatternY, m_currentStrokePatternRotation, 0.0, 0.0, 1.0);
						m_currentStrokePatternName = "";
					}
					ite->setLineEnd(m_CapStyle);
					ite->setLineJoin(m_JoinStyle);
					if (!m_WasU)
					{
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(),wh.y());
						ite->setTextFlowMode(PageItem::TextFlowDisabled);
						m_Doc->AdjustItemSize(ite);
					}
					if (m_patternMode)
						m_PatternElements.append(ite);
					else
						m_Elements.append(ite);
					if (m_groupStack.count() != 0)
						m_groupStack.top().append(ite);
					if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(m_itemLocked);
					
				}
				else
				{
					ite = m_Doc->Items->last();
					ite->PoLine.setMarker();
					m_Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->PoLine.putPoints(ite->PoLine.size(), m_Coords.size(), m_Coords);
				}
				m_FirstU = false;
				m_itemRendered = true;
				m_CurrFillShade = 100.0;
				m_CurrStrokeShade = 100.0;
			}
			m_Coords.resize(0);
			m_Coords.svgInit();
		}
		else if (command == "*u")
		{
			m_FirstU = true;
			m_WasU = true;
		}
		else if (command == "*U")
		{
			if (m_Doc->Items->count() > 0)
			{
				m_WasU = false;
				ite = m_Doc->Items->last();
				FPoint wh = getMaxClipF(&ite->PoLine);
				ite->setWidthHeight(wh.x(),wh.y());
				m_Doc->AdjustItemSize(ite);
			}
		}
		else if ((command == "u") || (command == "q"))
		{
			QList<PageItem*> gElements;
			m_groupStack.push(gElements);
			m_clipStack.push(m_clipCoords);
		}
		else if ((command == "U") || (command == "Q"))
		{
			if (m_groupStack.count() != 0)
			{
				QList<PageItem*> gElements = m_groupStack.pop();
				m_clipCoords = m_clipStack.pop();
				m_tmpSel->clear();
				if (gElements.count() > 0)
				{
					for (int dre = 0; dre < gElements.count(); ++dre)
					{
						m_tmpSel->addItem(gElements.at(dre), true);
						if (m_patternMode)
							m_PatternElements.removeAll(gElements.at(dre));
						else
							m_Elements.removeAll(gElements.at(dre));
					}
					ite = m_Doc->groupObjectsSelection(m_tmpSel);
					if ((m_clipCoords.size() > 4) && (command == "Q"))
					{
						m_clipCoords.translate(m_Doc->currentPage()->xOffset()-ite->xPos(), m_Doc->currentPage()->yOffset()-ite->yPos());
						ite->PoLine = m_clipCoords.copy();
						ite->PoLine.translate(m_baseX, m_baseY);
					}
					for (int as = 0; as < m_tmpSel->count(); ++as)
					{
						if (m_patternMode)
							m_PatternElements.append(m_tmpSel->itemAt(as));
						else
							m_Elements.append(m_tmpSel->itemAt(as));
					}
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
			if (command == "Q")
			{
				m_clipCoords.resize(0);
				m_clipCoords.svgInit();
			}
		}
		else if (command == "W")
		{
			if (m_clipStack.count() != 0)
			{
				if (m_clipStack.top().size() > 3)
				{
					m_clipStack.top().setMarker();
					m_clipStack.top().putPoints(m_clipStack.top().size(), m_Coords.size(), m_Coords);
				}
				else
					m_clipStack.top() = m_Coords.copy();
			}
		}
		else if ((command == "N") || (command == "n"))
		{
			if (command == "n")
				m_Coords.svgClosePath();
			m_currentSpecialPath = m_Coords.copy();
			m_Coords.resize(0);
			m_Coords.svgInit();
		}
/* End Object construction commands */
/* Start Graphics state commands */
		else if (command == "A")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 1)
				m_itemLocked = true;
			else
				m_itemLocked = false;
		}
		else if (command == "w")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> m_LineW;
		}
		else if (command == "j")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 0)
				m_JoinStyle = Qt::MiterJoin;
			else if (tmpInt == 1)
				m_JoinStyle = Qt::RoundJoin;
			else if (tmpInt == 1)
				m_JoinStyle = Qt::BevelJoin;
		}
		else if (command == "J")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 0)
				m_CapStyle = Qt::FlatCap;
			else if (tmpInt == 1)
				m_CapStyle = Qt::RoundCap;
			else if (tmpInt == 1)
				m_CapStyle = Qt::SquareCap;
		}
		/* undocumented Command Xy
			- has up to 5 Parameters
			- first Parameter might be the Blendmode
			- second Parameter is the Opacity
		*/
		else if (command == "Xy")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			int mode = 0;
			ts2 >> mode >> m_Opacity;
			// Adjusting blendmodes, taken from the PDF importer
			switch (mode)
			{
				default:
				case 0:
					m_blendMode = 0;
					break;
				case 4:
					m_blendMode = 1;
					break;
				case 5:
					m_blendMode = 2;
					break;
				case 1:
					m_blendMode = 3;
					break;
				case 2:
					m_blendMode = 4;
					break;
				case 3:
					m_blendMode = 5;
					break;
				case 8:
					m_blendMode = 6;
					break;
				case 9:
					m_blendMode = 7;
					break;
				case 10:
					m_blendMode = 8;
					break;
				case 11:
					m_blendMode = 9;
					break;
				case 6:
					m_blendMode = 10;
					break;
				case 7:
					m_blendMode = 11;
					break;
				case 12:
					m_blendMode = 12;
					break;
				case 13:
					m_blendMode = 13;
					break;
				case 14:
					m_blendMode = 14;
					break;
				case 15:
					m_blendMode = 15;
					break;
			}
		}
		else if (command == "XR")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 1)
				m_fillRule = true;
			else
				m_fillRule = false;
		}
		else if (command == "Bb")
		{
			m_gradientMode = true;
			m_wasBC = false;
			m_itemRendered = false;
			m_startMatrix = QTransform();
			m_endMatrix = QTransform();
		}
		else if (command == "Xm")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			double m1, m2, m3, m4, m5, m6;
			gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			m_startMatrix.translate(m5, -m6);
			m_endMatrix.scale(m1, m4);
			m_wasBC = true;
		}
		else if (command == "Bm")
		{
			if (m_gradients[m_currentGradientName].type() == 1)
			{
				ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
				double m1, m2, m3, m4, m5, m6;
				gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
				m_startMatrix.translate(m5, -m6);
//				endMatrix.scale(m1, m4);
				m_endMatrix *= QTransform(m1, m2, m3, m4, 0, 0);
//				endMatrix = QTransform(m1, m2, m3, m4, m5, m6);
				m_wasBC = true;
			}
		}
		else if (command == "BB")
		{
			if (m_itemRendered)
			{
				m_gradientMode = false;
				ite = m_Doc->Items->last();
				ite->fill_gradient = m_gradients[m_currentGradientName];
				ite->setGradient(m_currentGradientName);
				if (ite->fill_gradient.type() == 0)
					ite->GrType = 6;
				else
					ite->GrType = 7;
				QTransform m1;
				m1.translate(m_currentGradientOrigin.x() - ite->xPos(), m_currentGradientOrigin.y() - ite->yPos());
				m1.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
				m1.rotate(-m_currentGradientAngle);
				ite->GrStartX = m_currentGradientOrigin.x() - ite->xPos() + m_Doc->currentPage()->xOffset();
				ite->GrStartY = m_currentGradientOrigin.y() - ite->yPos() + m_Doc->currentPage()->yOffset();
				QPointF target = m1.map(QPointF(m_currentGradientLenght, 0.0));
				ite->GrEndX = target.x();
				ite->GrEndY = target.y();
				if (m_wasBC)
				{
					QPointF newS = m_startMatrix.map(QPointF(ite->GrStartX, ite->GrStartY));
					ite->GrStartX = newS.x();
					ite->GrStartY = newS.y();
					QTransform m2;
					m2.rotate(-m_currentGradientAngle);
					m2 *= m_endMatrix;
					QPointF target = m2.map(QPointF(m_currentGradientLenght, 0.0));
					ite->GrEndX = target.x();
					ite->GrEndY = target.y();
				}
			}
			m_wasBC = false;
			m_currentGradientMatrix = QTransform();
			m_currentGradientOrigin = QPointF(0.0, 0.0);
			m_currentGradientAngle = 0.0;
			m_currentGradientLenght = 1.0;
			m_itemRendered = false;
		}
		else if (command == "Bg")
		{
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			m_currentGradientName = Cdata.mid(an+1, en-an-1);
			m_currentGradientName.remove("\\");
			QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
			ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
			double xOrig, yOrig, m1, m2, m3, m4, m5, m6;
			gVals >> xOrig >> yOrig >> m_currentGradientAngle >> m_currentGradientLenght >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			m_currentGradientOrigin = QPointF(xOrig - m_docX, m_docHeight - (yOrig - m_docY));
			m_currentGradientMatrix = QTransform(m1, m2, m3, m4, m5, m6);
		}
/* End Graphics state commands */
/* Start Color commands */
		else if ((command == "G") || (command == "g"))
		{
			if (command == "G")
				m_CurrColorStroke = parseColorGray(Cdata);
			else
				m_CurrColorFill = parseColorGray(Cdata);
			m_meshColorMode = 2;
		}
		else if ((command == "K") || (command == "k"))
		{
			if (command == "K")
				m_CurrColorStroke = parseColor(Cdata);
			else
				m_CurrColorFill = parseColor(Cdata);
		}
		else if ((command == "XA") || (command == "Xa"))
		{
			QString Xdata = da2[da2.count()-4] + " " + da2[da2.count()-3] + " " + da2[da2.count()-2];
			if (command == "XA")
				m_CurrColorStroke = parseColorRGB(Xdata);
			else
				m_CurrColorFill = parseColorRGB(Xdata);
			m_meshColorMode = 1;
		}
		else if ((command == "XX") || (command == "Xx") || (command == "Xk"))
		{
			if (command == "XX")
				m_CurrColorStroke = parseCustomColorX(Cdata, m_CurrStrokeShade, da2[da2.count()-2]);
			else
				m_CurrColorFill = parseCustomColorX(Cdata, m_CurrFillShade, da2[da2.count()-2]);
		}
		else if ((command == "X") || (command == "x"))
		{
			if (command == "X")
				m_CurrColorStroke = parseCustomColor(Cdata, m_CurrStrokeShade);
			else
				m_CurrColorFill = parseCustomColor(Cdata, m_CurrFillShade);
		}
		else if (command == "p")
		{
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			m_currentPatternName = Cdata.mid(an+1, en-an-1);
			m_currentPatternName.remove("\\");
			m_currentPatternName = m_currentPatternName.trimmed().simplified().replace(" ", "_");
			QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
			ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
			gVals >> m_currentPatternX >> m_currentPatternY >> m_currentPatternXScale >> m_currentPatternYScale >> m_currentPatternRotation;
		}
		else if (command == "P")
		{
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			m_currentStrokePatternName = Cdata.mid(an+1, en-an-1);
			m_currentStrokePatternName.remove("\\");
			m_currentStrokePatternName = m_currentPatternName.trimmed().simplified().replace(" ", "_");
			QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
			ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
			gVals >> m_currentStrokePatternX >> m_currentStrokePatternY >> m_currentStrokePatternXScale >> m_currentStrokePatternYScale >> m_currentStrokePatternRotation;
		}
		else if (command == "X!")
		{
			if (Cdata.contains("/Mesh"))
			{
				m_meshMode = true;
				m_meshNodeCounter = 0;
//				meshColorMode = 0;
				m_meshGradientArray.clear();
			}
			if (Cdata.contains("/End"))
			{
				m_meshMode = false;
				if (m_meshGradientArray.count() != 0)
				{
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
					ite = m_Doc->Items->at(z);
					for (int x = 0; x < m_meshGradientArray.count(); x++)
					{
						for (int y = 0; y < m_meshGradientArray[x].count(); y++)
						{
							m_meshGradientArray[x][y].moveRel(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
						}
					}
					ite->meshGradientArray = m_meshGradientArray;
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
					ite->setFillShade(m_CurrFillShade);
					ite->setLineShade(m_CurrFillShade);
					ite->setFillEvenOdd(m_fillRule);
					ite->setFillTransparency(1.0 - m_Opacity);
					ite->setLineTransparency(1.0 - m_Opacity);
					ite->setFillBlendmode(m_blendMode);
					ite->setLineBlendmode(m_blendMode);
					ite->setLineEnd(m_CapStyle);
					ite->setLineJoin(m_JoinStyle);
					if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(m_itemLocked);
					if (m_patternMode)
						m_PatternElements.append(ite);
					else
						m_Elements.append(ite);
					if (m_groupStack.count() != 0)
						m_groupStack.top().append(ite);
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
				m_meshGradientArray.clear();
				int ans = cmdLine.indexOf("[");
				int ens = cmdLine.lastIndexOf("]");
				QString sizeVals = cmdLine.mid(ans+1, ens-ans-1);
				ScTextStream mVals2(&sizeVals, QIODevice::ReadOnly);
				mVals2 >> m_meshXSize >> m_meshYSize;
				for (int mgr = 0; mgr < m_meshYSize+1; mgr++)
				{
					QList<meshPoint> ml;
					for (int mgc = 0; mgc < m_meshXSize+1; mgc++)
					{
						meshPoint mp;
						ml.append(mp);
					}
					m_meshGradientArray.append(ml);
				}
			}
			if (mKey == "P")
			{
				int ans = cmdLine.indexOf("[");
				int ens = cmdLine.lastIndexOf("]");
				QString posVals = cmdLine.mid(ans+1, ens-ans-1);
				ScTextStream mVals3(&posVals, QIODevice::ReadOnly);
				mVals3 >> m_currentMeshXPos >> m_currentMeshYPos;
			}
			if (mKey == "CS")
			{
				if (Cdata.contains("CMYK"))
					m_meshColorMode = 0;
				else if (Cdata.contains("RGB"))
					m_meshColorMode = 1;
				else if (Cdata.contains("Gray"))
					m_meshColorMode = 2;
			}
			if (mKey == "E")
			{
				int indY = m_meshYSize - m_currentMeshYPos - 1;
				int indX = m_currentMeshXPos;
				m_meshGradientArray[indY+1][indX+1].gridPoint   = FPoint(m_meshNode2PointX, m_meshNode2PointY);
				m_meshGradientArray[indY+1][indX+1].controlTop  = FPoint(m_meshNode2Control2X, m_meshNode2Control2Y);
				m_meshGradientArray[indY+1][indX+1].controlLeft = FPoint(m_meshNode2Control1X, m_meshNode2Control1Y);
				m_meshGradientArray[indY+1][indX+1].colorName = m_meshColor2;
				m_meshGradientArray[indY+1][indX+1].shade = 100;
				m_meshGradientArray[indY+1][indX+1].transparency = 1.0;
				m_meshGradientArray[indY+1][indX].gridPoint    = FPoint(m_meshNode1PointX, m_meshNode1PointY);
				m_meshGradientArray[indY+1][indX].controlRight = FPoint(m_meshNode1Control2X, m_meshNode1Control2Y);
				m_meshGradientArray[indY+1][indX].controlTop   = FPoint(m_meshNode1Control1X, m_meshNode1Control1Y);
				m_meshGradientArray[indY+1][indX].colorName = m_meshColor1;
				m_meshGradientArray[indY+1][indX].shade = 100;
				m_meshGradientArray[indY+1][indX].transparency = 1.0;
				m_meshGradientArray[indY][indX].gridPoint     = FPoint(m_meshNode4PointX, m_meshNode4PointY);
				m_meshGradientArray[indY][indX].controlBottom = FPoint(m_meshNode4Control2X, m_meshNode4Control2Y);
				m_meshGradientArray[indY][indX].controlRight  = FPoint(m_meshNode4Control1X, m_meshNode4Control1Y);
				m_meshGradientArray[indY][indX].colorName = m_meshColor4;
				m_meshGradientArray[indY][indX].shade = 100;
				m_meshGradientArray[indY][indX].transparency = 1.0;
				m_meshGradientArray[indY][indX+1].gridPoint     = FPoint(m_meshNode3PointX, m_meshNode3PointY);
				m_meshGradientArray[indY][indX+1].controlLeft   = FPoint(m_meshNode3Control2X, m_meshNode3Control2Y);
				m_meshGradientArray[indY][indX+1].controlBottom = FPoint(m_meshNode3Control1X, m_meshNode3Control1Y);
				m_meshGradientArray[indY][indX+1].colorName = m_meshColor3;
				m_meshGradientArray[indY][indX+1].shade = 100;
				m_meshGradientArray[indY][indX+1].transparency = 1.0;
				m_meshNodeCounter = 0;
			}
			if (mKey == "N")
			{
				double cVal, mVal, yVal, kVal, coorX1, coorY1, coorX2, coorY2, coorX3, coorY3;
				int dummy;
				m_meshNodeCounter++;
				int ans = cmdLine.indexOf("[");
				int ens = cmdLine.lastIndexOf("]");
				QString nodeVals = cmdLine.mid(ans+1, ens-ans-1);
				ScTextStream mVals4(&nodeVals, QIODevice::ReadOnly);
				cVal = 0.0;
				mVal = 0.0;
				yVal = 0.0;
				kVal = 0.0;
				if (m_meshColorMode == 0)
					mVals4 >> cVal >> mVal >> yVal >> kVal >> coorX1 >> coorY1 >> coorX2 >> coorY2 >> dummy >> coorX3 >> coorY3;
				else if (m_meshColorMode == 1)
					mVals4 >> cVal >> mVal >> yVal >> coorX1 >> coorY1 >> coorX2 >> coorY2 >> dummy >> coorX3 >> coorY3;
				else if (m_meshColorMode == 2)
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
				if (m_meshColorMode == 0)
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
				else if (m_meshColorMode == 1)
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
				else if (m_meshColorMode == 2)
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
				if (m_meshNodeCounter == 1)
				{
					m_meshNode1PointX = coorX1 - m_docX;
					m_meshNode1PointY = m_docHeight - (coorY1 - m_docY);
					m_meshNode1Control2X = coorX2 - m_docX;
					m_meshNode1Control2Y = m_docHeight - (coorY2 - m_docY);
					m_meshNode1Control1X = coorX3 - m_docX;
					m_meshNode1Control1Y = m_docHeight - (coorY3 - m_docY);
					m_meshColor1 = nodeColor;
				}
				if (m_meshNodeCounter == 2)
				{
					m_meshNode2PointX = coorX1 - m_docX;
					m_meshNode2PointY = m_docHeight - (coorY1 - m_docY);
					m_meshNode2Control2X = coorX2 - m_docX;
					m_meshNode2Control2Y = m_docHeight - (coorY2 - m_docY);
					m_meshNode2Control1X = coorX3 - m_docX;
					m_meshNode2Control1Y = m_docHeight - (coorY3 - m_docY);
					m_meshColor2 = nodeColor;
				}
				if (m_meshNodeCounter == 3)
				{
					m_meshNode3PointX = coorX1 - m_docX;
					m_meshNode3PointY = m_docHeight - (coorY1 - m_docY);
					m_meshNode3Control2X = coorX2 - m_docX;
					m_meshNode3Control2Y = m_docHeight - (coorY2 - m_docY);
					m_meshNode3Control1X = coorX3 - m_docX;
					m_meshNode3Control1Y = m_docHeight - (coorY3 - m_docY);
					m_meshColor3 = nodeColor;
				}
				if (m_meshNodeCounter == 4)
				{
					m_meshNode4PointX = coorX1 - m_docX;
					m_meshNode4PointY = m_docHeight - (coorY1 - m_docY);
					m_meshNode4Control2X = coorX2 - m_docX;
					m_meshNode4Control2Y = m_docHeight - (coorY2 - m_docY);
					m_meshNode4Control1X = coorX3 - m_docX;
					m_meshNode4Control1Y = m_docHeight - (coorY3 - m_docY);
					m_meshColor4 = nodeColor;
				}
			}
		}
/* End Color commands */
/* Start Layer commands */
		else if (command == "Lb")
		{
			if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				int visible, preview, enabled, printing, dummy, rc, gc, bc;
				ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
				ts2 >> visible >> preview >> enabled >> printing >> dummy >> dummy >> dummy >> rc >> gc >> bc;
				if (!m_firstLayer)
					m_currentLayer = m_Doc->addLayer("Layer", true);
				m_Doc->setLayerVisible(m_currentLayer, static_cast<bool>(visible));
				m_Doc->setLayerOutline(m_currentLayer, static_cast<bool>(!preview));
				m_Doc->setLayerLocked(m_currentLayer, static_cast<bool>(!enabled));
				m_Doc->setLayerPrintable(m_currentLayer, static_cast<bool>(printing));
				m_Doc->setLayerMarker(m_currentLayer, QColor(rc, gc, bc));
				QList<PageItem*> gElements;
				m_groupStack.push(gElements);
				m_clipStack.push(m_clipCoords);
				m_firstLayer = false;
			}
			m_Coords.resize(0);
			m_Coords.svgInit();
		}
		else if (command == "LB")
		{
			if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				if (m_groupStack.count() != 0)
				{
					QList<PageItem*> gElements = m_groupStack.pop();
					m_clipStack.pop();
					m_tmpSel->clear();
					if (gElements.count() > 0)
					{
						for (int dre = 0; dre < gElements.count(); ++dre)
						{
							m_tmpSel->addItem(gElements.at(dre), true);
							if (m_patternMode)
								m_PatternElements.removeAll(gElements.at(dre));
							else
								m_Elements.removeAll(gElements.at(dre));
						}
						m_Doc->groupObjectsSelection(m_tmpSel);
						ite = m_tmpSel->itemAt(0);
						if (m_Coords.size() > 3)
						{
							m_Coords.translate(m_Doc->currentPage()->xOffset()-ite->xPos(), m_Doc->currentPage()->yOffset()-ite->yPos());
							ite->PoLine = m_Coords.copy();
							ite->PoLine.translate(m_baseX, m_baseY);
						}
						for (int as = 0; as < m_tmpSel->count(); ++as)
						{
							if (m_patternMode)
								m_PatternElements.append(m_tmpSel->itemAt(as));
							else
								m_Elements.append(m_tmpSel->itemAt(as));
						}
						ite->setItemName( tr("Group%1").arg(m_Doc->layerName(m_currentLayer)));
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
			m_Coords.resize(0);
			m_Coords.svgInit();
		}
		else if (command == "Ln")
		{
			if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				int an = Cdata.indexOf("(");
				int en = Cdata.lastIndexOf(")");
				QString LayerNam = Cdata.mid(an+1, en-an-1);
				LayerNam.remove("\\");
				m_Doc->changeLayerName(m_currentLayer, LayerNam);
			}
		}
/* End Layer commands */
/* Start Text commands */
		else if (command == "To")
		{
			ScTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> m_textMode;
			m_textData.clear();
			m_textMatrix = QTransform();
			m_maxWidth = 0;
			m_tempW = 0;
			m_maxHeight = 0;
			m_textKern = 0;
			m_startCurrentTextRange = 0;
			m_endCurrentTextRange = 0;
			m_textScaleH = 1000;
			m_textScaleV = 1000;
		}
		else if (command == "Tp")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			double m1, m2, m3, m4, m5, m6;
			gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			m_textMatrix = QTransform(m1, m2, m3, m4, m5, m6);
		}
		else if (command == "Tx") // || (command == "TX"))
		{
			QStringList res = getStrings(Cdata);
			if (res.count() > 0)
			{
				QString tex = res[0];
				double tempH = 0;
				m_startCurrentTextRange = m_textData.length();
				for (int tt = 0; tt < tex.length(); ++tt)
				{
					CharStyle nstyle;
					QString ch = tex.mid(tt,1);
					nstyle.setFont((*m_Doc->AllFonts)[m_textFont]);
					nstyle.setFontSize(m_textSize);
					nstyle.setFillColor(m_CurrColorFill);
					nstyle.setTracking(m_textKern);
					nstyle.setFillShade(100);
					nstyle.setStrokeColor(m_CurrColorStroke);
					nstyle.setStrokeShade(100);
					nstyle.setScaleH(m_textScaleH);
					nstyle.setScaleV(m_textScaleV);
					nstyle.setBaselineOffset(0);
					nstyle.setShadowXOffset(50);
					nstyle.setShadowYOffset(-50);
					nstyle.setOutlineWidth(10);
					nstyle.setUnderlineOffset(-1);
					nstyle.setUnderlineWidth(-1);
					nstyle.setStrikethruOffset(-1);
					nstyle.setStrikethruWidth(-1);
					nstyle.setFeatures(StyleFlag(ScStyle_Default).featureList());
					int pot = m_textData.length();
					m_textData.insertChars(pot, ch);
					m_textData.applyCharStyle(pot, 1, nstyle);
					m_tempW += nstyle.font().realCharWidth(ch[0], nstyle.fontSize() / 10.0)+1;
					tempH  = qMax(tempH, nstyle.font().height(nstyle.fontSize() / 10.0) + 2.0);
					m_maxWidth  = qMax(m_tempW, m_maxWidth);
					m_maxHeight = qMax(tempH, m_maxHeight);
					if ((ch == SpecialChars::PARSEP) || (ch == SpecialChars::LINEBREAK))
					{
						m_maxHeight += nstyle.font().height(nstyle.fontSize() / 10.0);
						m_tempW = 0;
					}
					m_endCurrentTextRange = pot;
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
			double oldval = m_textData.charStyle(m_startCurrentTextRange).tracking();
			CharStyle nstyle = m_textData.charStyle(m_startCurrentTextRange);
			nstyle.setTracking(oldval + val);
			m_textData.applyCharStyle(m_startCurrentTextRange, 1, nstyle);
		}
		else if (command == "Tc")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> m_textKern;
			m_textKern *= 100.0;
		}
		else if (command == "Tz")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> m_textScaleH >> m_textScaleV;
			m_textScaleH *= 10.0;
			m_textScaleV *= 10.0;
		}
		else if (command == "T*")
		{
			CharStyle nstyle;
			QString ch = SpecialChars::LINEBREAK;
			nstyle.setFont((*m_Doc->AllFonts)[m_textFont]);
			nstyle.setFontSize(m_textSize);
			nstyle.setFillColor(m_CurrColorFill);
			nstyle.setTracking(m_textKern);
			nstyle.setFillShade(100);
			nstyle.setStrokeColor(m_CurrColorStroke);
			nstyle.setStrokeShade(100);
			nstyle.setScaleH(m_textScaleH);
			nstyle.setScaleV(m_textScaleV);
			nstyle.setBaselineOffset(0);
			nstyle.setShadowXOffset(50);
			nstyle.setShadowYOffset(-50);
			nstyle.setOutlineWidth(10);
			nstyle.setUnderlineOffset(-1);
			nstyle.setUnderlineWidth(-1);
			nstyle.setStrikethruOffset(-1);
			nstyle.setStrikethruWidth(-1);
			nstyle.setFeatures(StyleFlag(ScStyle_Default).featureList());
			int pot = m_textData.length();
			m_textData.insertChars(pot, ch);
			m_textData.applyCharStyle(pot, 1, nstyle);
			m_maxHeight += nstyle.font().height(nstyle.fontSize() / 10.0) + 2.0;
			m_tempW = 0;
		}
		else if (command == "Tf")
		{
			ScTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> m_textFont >> m_textSize;
			m_textFont.remove(0, 2);
			QString family = m_textFont;
			QString ret = "";
			family.replace( QRegExp( "'" ) , QChar( ' ' ) );
			m_textFont = m_Doc->itemToolPrefs().textFont;
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
				m_textFont = family;
			else
			{
				if (m_importerFlags & LoadSavePlugin::lfCreateThumbnail)
					m_textFont = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
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
						m_textFont = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family];
				}
			}
			m_textSize *= 10.0;
		}
		else if (command == "TO")
		{
			if (m_textData.length() > 0)
			{
				if (!((m_textData.length() == 1) && (m_textData.text(0) == SpecialChars::PARSEP)))
				{
					QPointF pos = QPointF(m_textMatrix.dx(), m_textMatrix.dy());
					pos += QPointF(m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
					pos += QPointF(0.0, m_textSize / 10.0 + 2.0);
					z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, pos.x() - m_docX, m_docHeight - (pos.y() - m_docY), 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
					ite = m_Doc->Items->at(z);
					ite->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
					ite->itemText.append(m_textData);
					ite->itemText.trim();
					double xpos = ite->xPos();
					double ypos = ite->yPos();
					ite->setWidthHeight(qMax(ite->width(), m_maxWidth), qMax(ite->height(), m_maxHeight));
					double xoffset = 0.0, yoffset = 0.0;
					double rotation = getRotationFromMatrix(m_textMatrix, 0.0);
					if (rotation != 0.0)
					{
						double temp = m_textSize / 10.0 + 2.0;
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
					ite->setFillShade(m_CurrFillShade);
					ite->setLineShade(m_CurrStrokeShade);
					ite->setFillEvenOdd(m_fillRule);
					ite->setFillTransparency(1.0 - m_Opacity);
					ite->setLineTransparency(1.0 - m_Opacity);
					ite->setLineEnd(m_CapStyle);
					ite->setLineJoin(m_JoinStyle);
					if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(m_itemLocked);
					if (m_patternMode)
						m_PatternElements.append(ite);
					else
						m_Elements.append(ite);
					if (m_groupStack.count() != 0)
						m_groupStack.top().append(ite);
				}
			}
		}
/* End Text commands */
/* Start special Commands */
		else if (command == "*")
		{
			m_Coords.resize(0);
			m_Coords.svgInit();
		}
		else if (command == "[")
		{
			m_Coords.resize(0);
			m_Coords.svgInit();
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			if ((an != -1) && (en != -1))
			{
				m_patternMode = true;
				m_currentPatternDefName = Cdata.mid(an+1, en-an-1);
				m_currentPatternDefName.remove("\\");
				m_currentPatternDefName = m_currentPatternDefName.trimmed().simplified().replace(" ", "_");
				QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
				ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
				gVals >> m_patternX1 >> m_patternY1 >> m_patternX2 >> m_patternY2;
			}
		}
		else if (command == ",")
		{
			if (Cdata.contains("/Data"))
			{
				m_dataMode = true;
				m_dataString = "";
			}
		}
		else if (command == ":")
		{
			m_fObjectMode = true;
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
		command = da2.last();
		if (command == "Bd")
		{
			QString tmpS = Cdata;
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			m_currentGradientName = Cdata.mid(an+1, en-an-1);
			m_currentGradientName.remove("\\");
			if (da2[da2.count()-3] == "0")
				m_currentGradient = VGradient(VGradient::linear);
			else
				m_currentGradient = VGradient(VGradient::radial);
			m_currentGradient.clearStops();
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
				m_currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
			else if (colortype == 1)
			{
				stopName = parseColor(Cdata);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				m_currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
			else if (colortype == 2)
			{
				stopName = parseColor(Cdata);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				m_currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
			else if (colortype == 3)
			{
				stopName = parseCustomColor(Cdata, colorShade);
				int stopShade = qRound(colorShade);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				m_currentGradient.addStop( ScColorEngine::getShadeColor(gradC, m_Doc, stopShade), stop, 0.5, 1.0, stopName, stopShade);
			}
			else if (colortype == 4)
			{
				stopName = parseCustomColorX(Cdata, colorShade, "0");
				int stopShade = qRound(colorShade);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				m_currentGradient.addStop( ScColorEngine::getShadeColor(gradC, m_Doc, stopShade), stop, 0.5, 1.0, stopName, stopShade);
			}
			else if (colortype == 6)
			{
				stopName = parseColor(Cdata);
				const ScColor& gradC = m_Doc->PageColors[stopName];
				m_currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop, 0.5, 1.0, stopName, 100 );
			}
		}
		else if (command == "BD")
		{
			m_gradients.insert(m_currentGradientName, m_currentGradient);
			if (m_Doc->addGradient(m_currentGradientName, m_currentGradient))
				m_importedGradients.append(m_currentGradientName);
			m_currentGradient = VGradient(VGradient::linear);
			m_currentGradient.clearStops();
			m_currentGradient.setRepeatMethod( VGradient::none );
			m_currentGradientName = "";
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
		if (m_importerFlags & LoadSavePlugin::lfKeepPatterns)
		{
			if (tmp.startsWith("%_"))
				tmp.remove(0, 2);
		}
		if (m_patternMode)
		{
			if (tmp == "EndPattern")
			{
				m_tmpSel->clear();
				if (m_PatternElements.count() > 0)
				{
					for (int dre = 0; dre < m_PatternElements.count(); ++dre)
					{
						m_tmpSel->addItem(m_PatternElements.at(dre), true);
						if (m_groupStack.count() != 0)
							m_groupStack.top().removeAll(m_PatternElements.at(dre));
					}
					if (m_PatternElements.count() > 1)
						m_Doc->itemSelection_GroupObjects(false, false, m_tmpSel);
					if ((m_tmpSel->width() > 1) && (m_tmpSel->height() > 1))
					{
						ScPattern pat = ScPattern();
						pat.setDoc(m_Doc);
						PageItem* currItem = m_tmpSel->itemAt(0);
						currItem->setItemName(m_currentPatternDefName);
						m_Doc->DoDrawing = true;
						QImage tmpImg = currItem->DrawObj_toImage(qMin(qMax(qRound(m_patternX2 - m_patternX1), qRound(m_patternY2 - m_patternY1)), 500));
						if (!tmpImg.isNull())
						{
							QImage retImg = QImage(qRound(m_patternX2 - m_patternX1), qRound(m_patternY2 - m_patternY1), QImage::Format_ARGB32_Premultiplied);
							retImg.fill( qRgba(255, 255, 255, 0) );
							QPainter p;
							p.begin(&retImg);
							if (m_PatternElements.count() > 1)
								p.drawImage(qRound(-m_patternX1), qRound(-m_patternY1), tmpImg);
							else
								p.drawImage(0, 0, tmpImg);
							p.end();
							pat.pattern = retImg;
							m_Doc->DoDrawing = false;
							pat.width = m_patternX2 - m_patternX1;
							pat.height = m_patternY2 - m_patternY1;
							pat.xoffset = -m_patternX1;
							pat.yoffset = -m_patternY1;
							for (int as = 0; as < m_tmpSel->count(); ++as)
							{
								PageItem* Neu = m_tmpSel->itemAt(as);
								Neu->moveBy(-m_patternX1, -m_patternY1, true);
								Neu->gXpos -= m_patternX1;
								Neu->gYpos -= m_patternY1;
								pat.items.append(Neu);
							}
							m_Doc->addPattern(m_currentPatternDefName, pat);
							m_importedPatterns.append(m_currentPatternDefName);
						}
					}
					m_Doc->itemSelection_DeleteItem(m_tmpSel);
				}
				m_PatternElements.clear();
				m_currentPatternDefName = "";
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
			m_PatternElements.clear();
			m_currentPatternDefName = "";
			break;
		}
		else if (tmp.contains("BeginRaster") && (tmp.startsWith("%")))
		{
			while (!ts.atEnd())
			{
				tmp = readLinefromDataStream(ts);
				if (tmp.contains("EndRaster"))
					break;
				if(m_progressDialog)
				{
					m_progressDialog->setProgress("GI", ts.device()->pos());
					qApp->processEvents();
				}
			}
		}
		else
		{
			m_Coords.resize(0);
			m_Coords.svgInit();
			int an = tmp.indexOf("(");
			int en = tmp.lastIndexOf(")");
			if ((an != -1) && (en != -1))
			{
				m_patternMode = true;
				m_currentPatternDefName = tmp.mid(an+1, en-an-1);
				m_currentPatternDefName.remove("\\");
				m_currentPatternDefName = m_currentPatternDefName.trimmed().simplified().replace(" ", "_");
				QString tmpS = tmp.mid(en+1, tmp.size() - en);
				ScTextStream gVals(&tmpS, QIODevice::ReadOnly);
				gVals >> m_patternX1 >> m_patternY1 >> m_patternX2 >> m_patternY2;
			}
		}
	//		processData(tmp);
	}
	m_patternMode = false;
}

void AIPlug::processSymbol(QDataStream &ts, bool sym)
{
	QString tmp = "";
	QString tmpData = "";
	while (!ts.atEnd())
	{
		tmp = removeAIPrefix(readLinefromDataStream(ts));
		if (!m_patternMode)
		{
			int an = tmp.indexOf("(");
			int en = tmp.lastIndexOf(")");
			if ((an != -1) && (en != -1))
			{
				m_patternMode = true;
				m_currentPatternDefName = tmp.mid(an+1, en-an-1);
				m_currentPatternDefName.remove("\\");
				if (sym)
					m_currentPatternDefName = "S_"+m_currentPatternDefName.trimmed().simplified().replace(" ", "_");
				else
					m_currentPatternDefName = m_currentPatternDefName.trimmed().simplified().replace(" ", "_");
			}
		}
		else if ((tmp == "EndSymbol") || (tmp == "EndBrushPattern"))
		{
			m_tmpSel->clear();
			if (m_PatternElements.count() > 0)
			{
				for (int dre = 0; dre < m_PatternElements.count(); ++dre)
				{
					m_tmpSel->addItem(m_PatternElements.at(dre), true);
					m_groupStack.top().removeAll(m_PatternElements.at(dre));
				}
				if (m_PatternElements.count() > 1)
					m_Doc->itemSelection_GroupObjects(false, false, m_tmpSel);
				if ((m_tmpSel->width() > 1) && (m_tmpSel->height() > 1))
				{
					ScPattern pat = ScPattern();
					pat.setDoc(m_Doc);
					PageItem* currItem = m_tmpSel->itemAt(0);
					currItem->setItemName(m_currentPatternDefName);
					m_Doc->DoDrawing = true;
					pat.pattern = currItem->DrawObj_toImage(qMin(qMax(m_tmpSel->width(), m_tmpSel->height()), 500.0));
					if (!pat.pattern.isNull())
					{
						pat.width = m_tmpSel->width();
						pat.height = m_tmpSel->height();
						m_Doc->DoDrawing = false;
						for (int as = 0; as < m_tmpSel->count(); ++as)
						{
							PageItem* Neu = m_tmpSel->itemAt(as);
							pat.items.append(Neu);
						}
						m_importedPatterns.append(m_currentPatternDefName);
						m_importedSymbols.insert(m_currentPatternDefName, QPointF(currItem->xPos(), currItem->yPos()));
						m_Doc->addPattern(m_currentPatternDefName, pat);
					}
				}
				m_Doc->itemSelection_DeleteItem(m_tmpSel);
			}
			m_PatternElements.clear();
			m_currentPatternDefName = "";
			break;
		}
		else if (tmp.contains("BeginRaster") && (tmp.startsWith("%")))
		{
			while (!ts.atEnd())
			{
				tmp = readLinefromDataStream(ts);
				if (tmp.contains("EndRaster"))
					break;
				if(m_progressDialog)
				{
					m_progressDialog->setProgress("GI", ts.device()->pos());
					qApp->processEvents();
				}
			}
		}
		else
		{
			processData(tmp);
		}
	}
	m_patternMode = false;
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
	Cdata = da.last();
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
//	bool cmyk = false;
//	if (type == 4)
//		cmyk = true;
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
	pos += QPointF(m_baseX, -m_baseY);
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, pos.x() - m_docX, m_docHeight - (pos.y() - m_docY), 10, 10, 0, m_CurrColorFill, m_CurrColorStroke, true);
	PageItem* ite = m_Doc->Items->at(z);
	ite->setWidthHeight(fabs(w * m1), fabs(h * m4));
	double rotation = getRotationFromMatrix(imgMatrix, 0.0);
	ite->setRotation(rotation * 180 / M_PI);
	ite->SetRectFrame();
	m_Doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(m_CurrFillShade);
	ite->setLineShade(m_CurrStrokeShade);
	ite->setFillEvenOdd(m_fillRule);
	ite->setFillTransparency(1.0 - m_Opacity);
	ite->setLineTransparency(1.0 - m_Opacity);
	ite->setFillBlendmode(m_blendMode);
	ite->setLineBlendmode(m_blendMode);
	ite->setLineEnd(m_CapStyle);
	ite->setLineJoin(m_JoinStyle);
	uchar *p;
	uint yCount = 0;
	quint16 eTag = EXTRASAMPLE_UNASSALPHA;
	QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_ai_XXXXXX.tif");
	tempFile->setAutoRemove(false);
	tempFile->open();
	QString imgName = getLongPathName(tempFile->fileName());
	tempFile->close();
	ite->isInlineImage = true;
	ite->isTempFile = true;
	delete tempFile;
	TIFF* tif = TIFFOpen(imgName.toLocal8Bit().data(), "w");
	if (tif)
	{
		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, m_image.channels());
		if (alpha == 1)
			TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, 1, &eTag);
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
	m_Doc->loadPict(imgName, ite);
	if (ite->imageIsAvailable)
		ite->setImageXYScale(ite->width() / ite->pixm.width(), ite->height() / ite->pixm.height());
	if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
		ite->setLocked(m_itemLocked);
	if (m_patternMode)
		m_PatternElements.append(ite);
	else
		m_Elements.append(ite);
	if (m_groupStack.count() != 0)
		m_groupStack.top().append(ite);
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
	else if (tmp.startsWith("BeginRaster"))
	{
		processRaster(ts);
		if(m_progressDialog)
		{
			m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
	}
}

bool AIPlug::convert(QString fn)
{
	QString tmp;
	m_LineW = 1.0;
	m_Opacity = 1.0;
	m_blendMode = 0;
	m_CurrColorFill = "White";
	m_CurrFillShade = 100.0;
	m_CurrColorStroke = "Black";
	m_CurrStrokeShade = 100.0;
	m_JoinStyle = Qt::MiterJoin;
	m_CapStyle = Qt::FlatCap;
	m_DashPattern.clear();
	m_DashOffset = 0.0;
	m_fillRule = false;
	m_FirstU = false;
	m_WasU = false;
	m_firstLayer = true;
	m_patternMode = false;
	m_symbolMode = false;
	m_meshMode = false;
	m_dataMode = false;
	m_fObjectMode = false;
	m_dataString = "";
	m_itemLocked = false;
	m_patternX1 = 0.0;
	m_patternY1 = 0.0;
	m_patternX2 = 0.0;
	m_patternY2 = 0.0;
	m_Coords.resize(0);
	m_Coords.svgInit();
	m_clipCoords.resize(0);
	m_clipCoords.svgInit();
	m_currentSpecialPath.resize(0);
	m_currentSpecialPath.svgInit();
	m_currentPoint = FPoint(0.0, 0.0);
	m_currentLayer = 0;
	m_currentGradient = VGradient(VGradient::linear);
	m_currentGradient.clearStops();
	m_currentGradient.setRepeatMethod( VGradient::none );
	m_currentGradientName = "";
	m_currentGradientMatrix = QTransform();
	m_currentGradientOrigin = QPointF(0.0, 0.0);
	m_currentGradientAngle = 0.0;
	m_currentGradientLenght = 1.0;
	m_currentPatternName = "";
	m_currentPatternX = 0.0;
	m_currentPatternY = 0.0;
	m_currentPatternXScale = 1.0;
	m_currentPatternYScale = 1.0;
	m_currentPatternRotation = 0.0;
	m_currentStrokePatternName = "";
	m_currentStrokePatternX = 0.0;
	m_currentStrokePatternY = 0.0;
	m_currentStrokePatternXScale = 1.0;
	m_currentStrokePatternYScale = 1.0;
	m_currentStrokePatternRotation = 0.0;
	QList<PageItem*> gElements;
	m_groupStack.push(gElements);
	m_clipStack.push(m_clipCoords);
	m_commandList << "m" << "l" << "L" << "c" << "C" << "v" << "V" << "y" << "Y";		// Path construction
	m_commandList << "b" << "B" << "f" << "F" << "s" << "S" << "*u" << "*U";			// Object creation
	m_commandList << "u" << "U" << "W" << "q" << "Q";									// Object creation
	m_commandList << "A" << "w" << "j" << "J" << "Xy" << "XR";						// Graphic state
	m_commandList << "k" << "K" << "Xa" << "XA" << "x" << "X" << "XX" << "Xx";		// Color commands
	m_commandList << "Xk" << "g" << "G" << "p" << "P";								// Color commands
	m_commandList << "Ln" << "Lb" << "LB";											// Layer commands
	m_commandList << "Bd" << "BD" << "%_Bs" << "Bg" << "Bb" << "BB" << "Bm" << "Xm";	// Gradient commands
	m_commandList << "To" << "TO" << "Tf" << "Tp" << "Tx" << "TX" << "T*" << "Tk";	// Text commands
	m_commandList << "Tc" << "Tz";													// Text commands
	m_commandList << "XI" << "XG" << "Xh";											// Image commands
	m_commandList << "n" << "N" << "*" << "[";										// Special commands
	m_commandList << "X!" << "X#";													// Mesh commands
	m_commandList << "M" << "d" << "D" << "E";										// unimplemented
	m_commandList << "h" << "H" << "i" << "I" << "Np" << "O";							// unimplemented
	m_commandList << "P" << "R";														// unimplemented
	m_commandList << "XI" << "XF" << "XG" << "XT" << "Z" << "`" << "~" << "_" << "@";	// unimplemented
	m_commandList << "&" << "*w" << "*W" << "Ap" << "Ar";								// unimplemented
	if(m_progressDialog)
	{
		m_progressDialog->setOverallProgress(2);
		m_progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		int fSize = (int) f.size();
		if (m_progressDialog)
		{
			m_progressDialog->setTotalSteps("GI", fSize);
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
			if(m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
		}
		f.close();
	}
	if (m_progressDialog)
		m_progressDialog->close();
	return true;
}
