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
#include "slaoutput.h"
#include <GlobalParams.h>

#include "importpdf.h"

#include <cstdlib>

#include "commonstrings.h"
#include "ui/customfdialog.h"
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
#include "util_ghostscript.h"
#include "util_icon.h"
#include "util_math.h"


PdfPlug::PdfPlug(ScribusDoc* doc, int flags)
{
	tmpSele = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

QImage PdfPlug::readThumbnail(QString fName)
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
	return QImage();
}

bool PdfPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
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
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
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
		tmpSele->clear();
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
					tmpSele->addItem(Elements.at(dre), true);
				}
				tmpSele->setGroupRect();
				ScriXmlDoc *ss = new ScriXmlDoc();
				ScElemMimeData* md = new ScElemMimeData();
				md->setScribusElem(ss->WriteElem(m_Doc, tmpSele));
				delete ss;
				m_Doc->itemSelection_DeleteItem(tmpSele);
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

PdfPlug::~PdfPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSele;
}

bool PdfPlug::convert(QString fn)
{
	QString tmp;
	importedColors.clear();
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	oldDocItemCount = m_Doc->Items->count();
	if (progressDialog)
	{
		progressDialog->setBusyIndicator("GI");
		qApp->processEvents();
	}

	globalParams = new GlobalParams();
	if (globalParams)
	{
		GooString *fname = new GooString(QFile::encodeName(fn).data());
		globalParams->setErrQuiet(gTrue);
//		globalParams->setPrintCommands(gTrue);
		PDFDoc *pdfDoc = new PDFDoc(fname, 0, 0, 0);
		if (pdfDoc)
		{
			if (pdfDoc->isOk())
			{
				double hDPI = 72.0;
				double vDPI = 72.0;
				int firstPage = 1;
				int lastPage = pdfDoc->getNumPages();
	//			qDebug() << "converting page" << firstPage;
				SlaOutputDev *dev = new SlaOutputDev(m_Doc, &Elements, &importedColors, importerFlags);
				if (dev->isOk())
				{
					GBool useMediaBox = gTrue;
					GBool crop = gFalse;
					GBool printing = gFalse;
					dev->startDoc(pdfDoc->getXRef(), pdfDoc->getCatalog());
					int rotate = pdfDoc->getPageRotate(firstPage);
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						m_Doc->currentPage()->setInitialHeight(pdfDoc->getPageMediaHeight(firstPage));
						m_Doc->currentPage()->setInitialWidth(pdfDoc->getPageMediaWidth(firstPage));
						m_Doc->currentPage()->setHeight(pdfDoc->getPageMediaHeight(firstPage));
						m_Doc->currentPage()->setWidth(pdfDoc->getPageMediaWidth(firstPage));
						m_Doc->reformPages(true);
						Object info;
						pdfDoc->getDocInfo(&info);
						if (info.isDict())
						{
							Object obj;
							GooString *s1;
							Dict *infoDict = info.getDict();
							if (infoDict->lookup("Title", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setTitle(UnicodeParsedString(obj.getString()));
								obj.free();
							}
							if (infoDict->lookup("Author", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setAuthor(UnicodeParsedString(obj.getString()));
								obj.free();
							}
							if (infoDict->lookup("Subject", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setSubject(UnicodeParsedString(obj.getString()));
								obj.free();
							}
							if (infoDict->lookup("Keywords", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setKeywords(UnicodeParsedString(obj.getString()));
								obj.free();
							}
						}
						info.free();
						pdfDoc->displayPages(dev, firstPage, lastPage, hDPI, vDPI, rotate, useMediaBox, crop, printing);
					}
					else
						pdfDoc->displayPage(dev, firstPage, hDPI, vDPI, rotate, useMediaBox, crop, printing);
				}
				delete dev;
			}
		}
		delete pdfDoc;
	}
	delete globalParams;
	globalParams = 0;

//	qDebug() << "converting finished";
//	qDebug() << "Imported" << Elements.count() << "Elements";

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

	if (progressDialog)
		progressDialog->close();
	return true;
}

QString PdfPlug::UnicodeParsedString(GooString *s1)
{
	if ( !s1 || s1->getLength() == 0 )
		return QString();
	GBool isUnicode;
	int i;
	Unicode u;
	QString result;
	if ((s1->getChar(0) & 0xff) == 0xfe && (s1->getLength() > 1 && (s1->getChar(1) & 0xff) == 0xff))
	{
		isUnicode = gTrue;
		i = 2;
		result.reserve((s1->getLength() - 2) / 2);
	}
	else
	{
		isUnicode = gFalse;
		i = 0;
		result.reserve(s1->getLength());
	}
	while (i < s1->getLength())
	{
		if (isUnicode)
		{
			u = ((s1->getChar(i) & 0xff) << 8) | (s1->getChar(i+1) & 0xff);
			i += 2;
		}
		else
		{
			u = s1->getChar(i) & 0xff;
			++i;
		}
		result += QChar( u );
	}
	return result;
}
