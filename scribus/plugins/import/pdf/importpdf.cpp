/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cstdlib>

#include <QByteArray>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QFile>
#include <QInputDialog>
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QStack>

#include <poppler/ErrorCodes.h>
#include <poppler/GlobalParams.h>
#include <poppler/OptionalContent.h>
#include <poppler/PageTransition.h>
#include <poppler/ViewerPreferences.h>
#include <poppler/poppler-config.h>
#include <poppler/cpp/poppler-version.h>
#include <poppler/SplashOutputDev.h>
#include <poppler/splash/SplashBitmap.h>

#include "importpdf.h"
#include "importpdfconfig.h"
#include "slaoutput.h"

#include "commonstrings.h"
#include "loadsaveplugin.h"
#include "pagesize.h"
#include "pdfimportoptions.h"
#include "pdfoptions.h"
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
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"

#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"

PdfPlug::PdfPlug(ScribusDoc* doc, int flags)
{
	tmpSele = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);

}

QImage PdfPlug::readThumbnail(const QString& fName)
{
	QString pdfFile = QDir::toNativeSeparators(fName);
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 83, 0)
	globalParams.reset(new GlobalParams());
#else
	globalParams = new GlobalParams();
#endif
	if (globalParams)
	{
#if defined(Q_OS_WIN32) && POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 62, 0)
		GooString *fname = new GooString(pdfFile.toUtf8().data());
#else
		GooString *fname = new GooString(QFile::encodeName(pdfFile).data());
#endif
		globalParams->setErrQuiet(gTrue);
		PDFDoc *pdfDoc = new PDFDoc(fname, nullptr, nullptr, nullptr);
		if (pdfDoc)
		{
			if (pdfDoc->getErrorCode() == errEncrypted)
			{
				delete pdfDoc;
#if POPPLER_ENCODED_VERSION < POPPLER_VERSION_ENCODE(0, 83, 0)
				delete globalParams;
#endif
				return QImage();
			}
			if (pdfDoc->isOk())
			{
				double h = pdfDoc->getPageMediaHeight(1);
				double w = pdfDoc->getPageMediaWidth(1);
				double scale = qMin(500.0 / h, 500.0 / w);
				double hDPI = 72.0 * scale;
				double vDPI = 72.0 * scale;
				SplashColor bgColor;
				bgColor[0] = 255;
				bgColor[1] = 255;
				bgColor[2] = 255;
				SplashOutputDev *dev = new SplashOutputDev(splashModeXBGR8, 4, gFalse, bgColor, gTrue);
				dev->setVectorAntialias(gTrue);
				dev->setFreeTypeHinting(gTrue, gFalse);
				dev->startDoc(pdfDoc);
				pdfDoc->displayPage(dev, 1, hDPI, vDPI, 0, gTrue, gFalse, gFalse);
				SplashBitmap *bitmap = dev->getBitmap();
				int bw = bitmap->getWidth();
				int bh = bitmap->getHeight();
				SplashColorPtr dataPtr = bitmap->getDataPtr();
				if (QSysInfo::BigEndian == QSysInfo::ByteOrder)
				{
					uchar c;
					int count = bw * bh * 4;
					for (int k = 0; k < count; k += 4)
					{
						c = dataPtr[k];
						dataPtr[k] = dataPtr[k+3];
						dataPtr[k+3] = c;
						c = dataPtr[k+1];
						dataPtr[k+1] = dataPtr[k+2];
						dataPtr[k+2] = c;
					}
				}
				// construct a qimage SHARING the raw bitmap data in memory
				QImage tmpimg( dataPtr, bw, bh, QImage::Format_ARGB32 );
				QImage image = tmpimg.copy();
				image.setText("XSize", QString("%1").arg(w));
				image.setText("YSize", QString("%1").arg(h));
				delete dev;
				delete pdfDoc;
#if POPPLER_ENCODED_VERSION < POPPLER_VERSION_ENCODE(0, 83, 0)
				delete globalParams;
#endif
				return image;
			}
			delete pdfDoc;
#if POPPLER_ENCODED_VERSION < POPPLER_VERSION_ENCODE(0, 83, 0)
			delete globalParams;
#endif
		}
	}
	return QImage();
}

bool PdfPlug::import(const QString& fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
#ifdef Q_OS_MACOS
	showProgress = false;
#endif
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	bool ret = false;
	QFileInfo fi = QFileInfo(fNameIn);
	if ( !ScCore->usingGUI() )
	{
		interactive = false;
		showProgress = false;
	}
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if ( showProgress )
	{
		ScribusMainWindow* mw = (m_Doc==nullptr) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw );
		QStringList barNames("GI");
		QStringList barTexts(tr("Analyzing File:"));
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
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	double docWidth = PrefsManager::instance().appPrefs.docSetupPrefs.pageWidth;
	double docHeight = PrefsManager::instance().appPrefs.docSetupPrefs.pageHeight;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, 0, 0, 0, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
		}
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
		tmpSele->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() == 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
		{
			PageItem *gr = Elements[0];
			if (gr->isGroup())
				m_Doc->resizeGroupToContents(gr);
		}
		if ((Elements.count() > 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
		{
			PageItem *gr = m_Doc->groupObjectsList(Elements);
			m_Doc->resizeGroupToContents(gr);
		}
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
					tmpSele->addItem(Elements.at(dre), true);
				}
				tmpSele->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::writeToMimeData(m_Doc, tmpSele);
				m_Doc->itemSelection_DeleteItem(tmpSele);
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
		success = false;
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

PdfPlug::~PdfPlug()
{
	delete progressDialog;
	delete tmpSele;
}

bool PdfPlug::convert(const QString& fn)
{
	bool firstPg = true;
	int currentLayer = m_Doc->activeLayer();
	int baseLayer = m_Doc->activeLayer();
	importedColors.clear();
	if (progressDialog)
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

#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 83, 0)
	globalParams.reset(new GlobalParams());
#else
	globalParams = new GlobalParams();
#endif
	GooString *userPW = nullptr;
	if (globalParams)
	{
#if defined(Q_OS_WIN32) && POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 62, 0)
		GooString *fname = new GooString(fn.toUtf8().data());
#else
		GooString *fname = new GooString(QFile::encodeName(fn).data());
#endif
		globalParams->setErrQuiet(gTrue);
		GBool hasOcg = gFalse;
		QList<OptionalContentGroup*> ocgGroups;
//		globalParams->setPrintCommands(gTrue);
		PDFDoc *pdfDoc = new PDFDoc(fname, nullptr, nullptr, nullptr);
		if (pdfDoc)
		{
			if (pdfDoc->getErrorCode() == errEncrypted)
			{
				delete pdfDoc;
				pdfDoc = nullptr;
				if (progressDialog)
					progressDialog->hide();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				ScribusMainWindow* mw = m_Doc->scMW();
				bool ok;
				QString text = QInputDialog::getText(mw, tr("Open PDF-File"), tr("Password"), QLineEdit::Normal, "", &ok);
				if (ok && !text.isEmpty())
				{
#if defined(Q_OS_WIN32) && POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 62, 0)
					fname = new GooString(fn.toUtf8().data());
#else
					fname = new GooString(QFile::encodeName(fn).data());
#endif
					userPW = new GooString(text.toLocal8Bit().data());
					pdfDoc = new PDFDoc(fname, userPW, userPW, nullptr);
					qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
				}
				if ((!pdfDoc) || (pdfDoc->getErrorCode() != errNone))
				{
					if (progressDialog)
						progressDialog->close();
					delete pdfDoc;
#if POPPLER_ENCODED_VERSION < POPPLER_VERSION_ENCODE(0, 83, 0)
					delete globalParams;
#endif
					return false;
				}
				if (progressDialog)
					progressDialog->show();
			}
			if (pdfDoc->isOk())
			{
				std::vector<int> pageNs;
				QString pageString = "*";
				m_pdfDoc = pdfDoc;
				double hDPI = 72.0;
				double vDPI = 72.0;
				int firstPage = 1;
				int lastPage = pdfDoc->getNumPages();
				GBool useMediaBox = gTrue;
				GBool crop = gTrue;
				GBool printing = gFalse;
				const PDFRectangle *mediaBox = pdfDoc->getPage(1)->getMediaBox();
				QRectF mediaRect = QRectF(QPointF(mediaBox->x1, mediaBox->y1), QPointF(mediaBox->x2, mediaBox->y2)).normalized();
				bool boxesAreDifferent = false;
				if (getCBox(Crop_Box, 1) != mediaRect)
					boxesAreDifferent = true;
				else if (getCBox(Trim_Box, 1) != mediaRect)
					boxesAreDifferent = true;
				else if (getCBox(Bleed_Box, 1) != mediaRect)
					boxesAreDifferent = true;
				else if (getCBox(Art_Box, 1) != mediaRect)
					boxesAreDifferent = true;
				bool cropped = false;
				int contentRect = Media_Box;
				if (((interactive) || (importerFlags & LoadSavePlugin::lfCreateDoc)) && ((lastPage > 1) || boxesAreDifferent))
				{
					if (progressDialog)
						progressDialog->hide();
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					PdfImportOptions *optImp = new PdfImportOptions(ScCore->primaryMainWindow());
					QFileInfo fi = QFileInfo(fn);
					optImp->setUpOptions(fi.fileName(), firstPage, lastPage, interactive, boxesAreDifferent, this);
					if (!optImp->exec())
					{
						if (progressDialog)
							progressDialog->close();
						delete optImp;
						delete pdfDoc;
#if POPPLER_ENCODED_VERSION < POPPLER_VERSION_ENCODE(0, 83, 0)
						delete globalParams;
#endif
						return false;
					}
					pageString = optImp->getPagesString();
					contentRect = optImp->getCropBox();
					cropped = optImp->croppingEnabled();
					if (!cropped)
						crop = cropped;
					// When displaying  pages slices, we should always set useMediaBox to true
					// in order to use MediaBox (x, y) as coordinate system
					if (contentRect != Media_Box)
						useMediaBox = gFalse;
					if (cropped)
						useMediaBox = gTrue;
					delete optImp;
					qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
					if (progressDialog)
						progressDialog->show();
				}
				parsePagesString(pageString, &pageNs, lastPage);
				firstPage = pageNs[0];
				SlaOutputDev *dev = new SlaOutputDev(m_Doc, &Elements, &importedColors, importerFlags);
				if (dev->isOk())
				{
					OCGs* ocg = pdfDoc->getOptContentConfig();
					if (ocg)
					{
						hasOcg = ocg->hasOCGs();
						if (hasOcg)
						{
							QStringList ocgNames;
							Array *order = ocg->getOrderArray();
							if (order)
							{
								for (int i = 0; i < order->getLength (); ++i)
								{
									Object orderItem = order->get(i);
									if (orderItem.isDict())
									{
										POPPLER_CONST_075 Object POPPLER_REF ref = order->getNF(i);
										if (ref.isRef())
										{
											OptionalContentGroup *oc = ocg->findOcgByRef(ref.getRef());
											QString ocgName = UnicodeParsedString(oc->getName());
											if (!ocgNames.contains(ocgName))
											{
												ocgGroups.prepend(oc);
												ocgNames.append(ocgName);
											}
										}
									}
									else
									{
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 69, 0)
										const auto& ocgs = ocg->getOCGs ();
										for (const auto& ocg : ocgs)
										{
											OptionalContentGroup *oc = ocg.second.get();
											QString ocgName = UnicodeParsedString(oc->getName());
											if (!ocgNames.contains(ocgName))
											{
												ocgGroups.prepend(oc);
												ocgNames.append(ocgName);
											}
										}
#else
										GooList *ocgs = ocg->getOCGs ();
										for (int i = 0; i < ocgs->getLength (); ++i)
										{
											OptionalContentGroup *oc = (OptionalContentGroup *)ocgs->get(i);
											QString ocgName = UnicodeParsedString(oc->getName());
											if (!ocgNames.contains(ocgName))
											{
												ocgGroups.prepend(oc);
												ocgNames.append(ocgName);
											}
										}
#endif
									}
								}
							}
							else
							{
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 69, 0)
								const auto& ocgs = ocg->getOCGs ();
								for (const auto& ocg : ocgs)
								{
									OptionalContentGroup *oc = ocg.second.get();
									QString ocgName = UnicodeParsedString(oc->getName());
									if (!ocgNames.contains(ocgName))
									{
										ocgGroups.prepend(oc);
										ocgNames.append(ocgName);
									}
								}
#else
								GooList *ocgs = ocg->getOCGs ();
								for (int i = 0; i < ocgs->getLength (); ++i)
								{
									OptionalContentGroup *oc = (OptionalContentGroup *)ocgs->get(i);
									QString ocgName = UnicodeParsedString(oc->getName());
									if (!ocgNames.contains(ocgName))
									{
										ocgGroups.prepend(oc);
										ocgNames.append(ocgName);
									}
								}
#endif
							}
						}
					}

					const int zeroRotate = 0;
					dev->startDoc(pdfDoc, pdfDoc->getXRef(), pdfDoc->getCatalog());
					dev->rotate = pdfDoc->getPageRotate(firstPage);
					bool rotated = dev->rotate == 90 || dev->rotate == 270;

					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						if (hasOcg)
						{
							QString actL(m_Doc->activeLayerName());
							for (int i = 0; i < ocgGroups.count(); i++)
							{
								OptionalContentGroup *oc = ocgGroups[i];
								if (actL != UnicodeParsedString(oc->getName()))
									currentLayer = m_Doc->addLayer(UnicodeParsedString(oc->getName()), false);
								else
									currentLayer = m_Doc->layerIDFromName(UnicodeParsedString(oc->getName()));
								if (oc->getState() == OptionalContentGroup::On)
									m_Doc->setLayerVisible(currentLayer, true);
								else if (oc->getViewState() == OptionalContentGroup::ocUsageOn)
									m_Doc->setLayerVisible(currentLayer, true);
								else
									m_Doc->setLayerVisible(currentLayer, false);
								if ((oc->getPrintState() == OptionalContentGroup::ocUsageOn) || (oc->getPrintState() == OptionalContentGroup::ocUsageUnset))
									m_Doc->setLayerPrintable(currentLayer, true);
								else
									m_Doc->setLayerPrintable(currentLayer, false);
								oc->setState(OptionalContentGroup::Off);
							}
							dev->layersSetByOCG = true;
						}

						Object info = pdfDoc->getDocInfo();
						if (info.isDict())
						{
							Object obj;
							Dict *infoDict = info.getDict();
							obj = infoDict->lookup((char*) "Title");
							if (obj.isString())
							{
								m_Doc->documentInfo().setTitle(UnicodeParsedString(obj.getString()));
							}
							obj = infoDict->lookup((char*) "Author");
							if (obj.isString())
							{
								m_Doc->documentInfo().setAuthor(UnicodeParsedString(obj.getString()));
							}
							obj = infoDict->lookup((char*) "Subject");
							if (obj.isString())
							{
								m_Doc->documentInfo().setSubject(UnicodeParsedString(obj.getString()));
							}
							obj = infoDict->lookup((char*) "Keywords");
							if (obj.isString())
							{
								//		s1 = obj.getString();
								m_Doc->documentInfo().setKeywords(UnicodeParsedString(obj.getString()));
							}
						}
						info = Object();

						if (cropped)
						{
							QRectF crBox = getCBox(contentRect, pageNs[0]);
							if (rotated)
							{
								m_Doc->setPageWidth(crBox.height());
								m_Doc->setPageHeight(crBox.width());
							}
							else
							{
								m_Doc->setPageHeight(crBox.height());
								m_Doc->setPageWidth(crBox.width());
							}
						}
						else
						{
							if (rotated)
							{
								m_Doc->setPageWidth(pdfDoc->getPageMediaHeight(pageNs[0]));
								m_Doc->setPageHeight(pdfDoc->getPageMediaWidth(pageNs[0]));
							}
							else
							{
								m_Doc->setPageHeight(pdfDoc->getPageMediaHeight(pageNs[0]));
								m_Doc->setPageWidth(pdfDoc->getPageMediaWidth(pageNs[0]));
							}
						}
						m_Doc->setPageSize("Custom");
					//	m_Doc->pdfOptions().PresentVals.clear();
						for (size_t i = 0; i < pageNs.size(); ++i)
						{
							int pp = pageNs[i];
							m_Doc->setActiveLayer(baseLayer);
							if (firstPg)
								firstPg = false;
							else
								m_Doc->addPage(i);
							QRectF mdBox = getCBox(0, pp);
							QRectF crBox = getCBox(contentRect, pp);
							if (cropped)
							{
								if (rotated)
								{
									m_Doc->currentPage()->setInitialWidth(crBox.height());
									m_Doc->currentPage()->setInitialHeight(crBox.width());
									m_Doc->currentPage()->setWidth(crBox.height());
									m_Doc->currentPage()->setHeight(crBox.width());
									dev->cropOffsetX = crBox.y();
									dev->cropOffsetY = crBox.x();
								}
								else
								{
									m_Doc->currentPage()->setInitialHeight(crBox.height());
									m_Doc->currentPage()->setInitialWidth(crBox.width());
									m_Doc->currentPage()->setHeight(crBox.height());
									m_Doc->currentPage()->setWidth(crBox.width());
									dev->cropOffsetX = crBox.x();
									dev->cropOffsetY = crBox.y();
								}
							}
							else
							{
								if (rotated)
								{
									m_Doc->currentPage()->setInitialWidth(pdfDoc->getPageMediaHeight(pp));
									m_Doc->currentPage()->setInitialHeight(pdfDoc->getPageMediaWidth(pp));
									m_Doc->currentPage()->setWidth(pdfDoc->getPageMediaHeight(pp));
									m_Doc->currentPage()->setHeight(pdfDoc->getPageMediaWidth(pp));
								}
								else
								{
									m_Doc->currentPage()->setInitialHeight(pdfDoc->getPageMediaHeight(pp));
									m_Doc->currentPage()->setInitialWidth(pdfDoc->getPageMediaWidth(pp));
									m_Doc->currentPage()->setHeight(pdfDoc->getPageMediaHeight(pp));
									m_Doc->currentPage()->setWidth(pdfDoc->getPageMediaWidth(pp));
								}
							}
							m_Doc->currentPage()->setMasterPageNameNormal();
							m_Doc->currentPage()->setSize("Custom");
							m_Doc->reformPages(true);
							if (hasOcg)
							{
								for (int j = 0; j < ocgGroups.count(); j++)
								{
									OptionalContentGroup *oc = ocgGroups[j];
									oc->setState(OptionalContentGroup::On);
									if (cropped)
										pdfDoc->displayPageSlice(dev, pp, hDPI, vDPI, zeroRotate, useMediaBox, crop, printing, crBox.x() - mdBox.x(), mdBox.bottom() - crBox.bottom(), crBox.width(), crBox.height(), nullptr, nullptr, dev->annotations_callback, dev);
									else
										pdfDoc->displayPage(dev, pp, hDPI, vDPI, zeroRotate, useMediaBox, crop, printing, nullptr, nullptr, dev->annotations_callback, dev);
									oc->setState(OptionalContentGroup::Off);
								}
							}
							else
							{
								if (cropped)
									pdfDoc->displayPageSlice(dev, pp, hDPI, vDPI, zeroRotate, useMediaBox, crop, printing, crBox.x() - mdBox.x(), mdBox.bottom() - crBox.bottom(), crBox.width(), crBox.height(), nullptr, nullptr, dev->annotations_callback, dev);
								else
									pdfDoc->displayPage(dev, pp, hDPI, vDPI, zeroRotate, useMediaBox, crop, printing, nullptr, nullptr, dev->annotations_callback, dev);
							}

							PDFPresentationData ef;
							Object trans = pdfDoc->getPage(pp)->getTrans();
							Object *transi = &trans;
							if (transi->isDict())
							{
								m_Doc->pdfOptions().PresentMode = true;
								PageTransition *pgTrans = new PageTransition(transi);
								ef.pageViewDuration = pdfDoc->getPage(pp)->getDuration();
								ef.pageEffectDuration = pgTrans->getDuration();
								ef.Dm = pgTrans->getAlignment() == transitionHorizontal ? 0 : 1;
								ef.M = pgTrans->getDirection() == transitionInward ? 0 : 1;
								int ang = pgTrans->getAngle();
								if (ang == 0)
									ef.Di = 0;
								else if (ang == 270)
									ef.Di = 1;
								else if (ang == 90)
									ef.Di = 2;
								else if (ang == 180)
									ef.Di = 3;
								else if (ang == 315)
									ef.Di = 4;
								PageTransitionType trType = pgTrans->getType();
								if (trType == transitionReplace)
									ef.effectType = 0;
								else if (trType == transitionBlinds)
									ef.effectType = 1;
								else if (trType == transitionBox)
									ef.effectType = 2;
								else if (trType == transitionDissolve)
									ef.effectType = 3;
								else if (trType == transitionGlitter)
									ef.effectType = 4;
								else if (trType == transitionSplit)
									ef.effectType = 5;
								else if (trType == transitionWipe)
									ef.effectType = 6;
								else if (trType == transitionPush)
									ef.effectType = 7;
								else if (trType == transitionCover)
									ef.effectType = 8;
								else if (trType == transitionUncover)
									ef.effectType = 9;
								else if (trType == transitionFade)
									ef.effectType = 10;
								delete pgTrans;
							}
							m_Doc->currentPage()->PresentVals = ef;
						}
						int numjs = pdfDoc->getCatalog()->numJS();
						if (numjs > 0)
						{
							NameTree *jsNameTreeP = new NameTree();
							Object catDict = pdfDoc->getXRef()->getCatalog();
							if (catDict.isDict())
							{
								Object names = catDict.dictLookup("Names");
								if (names.isDict())
								{
									Object obj = names.dictLookup("JavaScript");
									jsNameTreeP->init(pdfDoc->getXRef(), &obj);
								}
								for (int a = 0; a < numjs; a++)
								{
									m_Doc->JavaScripts.insert(UnicodeParsedString(jsNameTreeP->getName(a)), UnicodeParsedString(pdfDoc->getCatalog()->getJS(a)));
								}
								names = catDict.dictLookup("OpenAction");
								if (names.isDict())
								{
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 86, 0)
									std::unique_ptr<LinkAction> linkAction;
									linkAction = LinkAction::parseAction(&names, pdfDoc->getCatalog()->getBaseURI());
#else
									LinkAction *linkAction = nullptr;
									linkAction = LinkAction::parseAction(&names, pdfDoc->getCatalog()->getBaseURI());
#endif
									if (linkAction)
									{
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 86, 0)
										LinkJavaScript *jsa = (LinkJavaScript*) linkAction.get();
#else
										LinkJavaScript *jsa = (LinkJavaScript*) linkAction;
#endif
										if (jsa->isOk())
										{
											QString script = UnicodeParsedString(jsa->getScript());
											if (script.startsWith("this."))
											{
												script.remove(0, 5);
												script.remove("()");
												if (m_Doc->JavaScripts.contains(script))
													m_Doc->pdfOptions().openAction = script;
											}
										}
									}
								}
							}
							delete jsNameTreeP;
						}
						m_Doc->pdfOptions().Version = (PDFVersion::Version) qMin(16, qMax(13, pdfDoc->getPDFMajorVersion() * 10 + pdfDoc->getPDFMinorVersion()));
						ViewerPreferences *viewPrefs = pdfDoc->getCatalog()->getViewerPreferences();
						if (viewPrefs)
						{
							m_Doc->pdfOptions().Binding = viewPrefs->getDirection() == ViewerPreferences::directionL2R ? 0 : 1;
							m_Doc->pdfOptions().hideMenuBar = viewPrefs->getHideMenubar();
							m_Doc->pdfOptions().hideToolBar = viewPrefs->getHideToolbar();
							m_Doc->pdfOptions().fitWindow = viewPrefs->getFitWindow();
						}
						Catalog::PageMode pgm = pdfDoc->getCatalog()->getPageMode();
						m_Doc->pdfOptions().displayFullscreen = (pgm == Catalog::pageModeFullScreen);
						m_Doc->pdfOptions().displayThumbs = (pgm == Catalog::pageModeThumbs);
						m_Doc->pdfOptions().displayBookmarks = (pgm == Catalog::pageModeOutlines);
						m_Doc->pdfOptions().displayLayers = (pgm == Catalog::pageModeOC);
						Catalog::PageLayout pgl = pdfDoc->getCatalog()->getPageLayout();
						if (pgl == Catalog::pageLayoutSinglePage)
							m_Doc->pdfOptions().PageLayout = PDFOptions::SinglePage;
						else if (pgl == Catalog::pageLayoutOneColumn)
							m_Doc->pdfOptions().PageLayout = PDFOptions::OneColumn;
						else if ((pgl == Catalog::pageLayoutTwoColumnLeft) || (pgl == Catalog::pageLayoutTwoPageLeft))
						{
							m_Doc->setPagePositioning(1);
							m_Doc->setPageSetFirstPage(1, 0);
							m_Doc->pdfOptions().PageLayout = PDFOptions::TwoColumnLeft;
						}
						else if ((pgl == Catalog::pageLayoutTwoColumnRight) || (pgl == Catalog::pageLayoutTwoPageRight))
						{
							m_Doc->setPagePositioning(1);
							m_Doc->setPageSetFirstPage(1, 1);
							m_Doc->pdfOptions().PageLayout = PDFOptions::TwoColumnRight;
						}
					}
					else
					{
						if (hasOcg)
						{
							for (int a = 0; a < ocgGroups.count(); a++)
							{
								ocgGroups[a]->setState(OptionalContentGroup::On);
							}
						}
						pdfDoc->displayPage(dev, firstPage, hDPI, vDPI, zeroRotate, useMediaBox, crop, printing, nullptr, nullptr, dev->annotations_callback, dev);
					}
				}
				delete dev;
			}
		}
		delete pdfDoc;
	}
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 83, 0)
	globalParams.release();
#else
	delete globalParams;
	globalParams = nullptr;
#endif

//	qDebug() << "converting finished";
//	qDebug() << "Imported" << Elements.count() << "Elements";

	if (Elements.count() == 0)
	{
		if (importedColors.count() != 0)
		{
			for (int i = 0; i < importedColors.count(); i++)
			{
				m_Doc->PageColors.remove(importedColors[i]);
			}
		}
	}

	if (progressDialog)
		progressDialog->close();
	return true;
}

QImage PdfPlug::readPreview(int pgNum, int width, int height, int box)
{
	if (!m_pdfDoc)
		return QImage();
	
	double h = m_pdfDoc->getPageMediaHeight(pgNum);
	double w = m_pdfDoc->getPageMediaWidth(pgNum);
	double scale = qMin(height / h, width / w);
	double hDPI = 72.0 * scale;
	double vDPI = 72.0 * scale;
	SplashColor bgColor;
	bgColor[0] = 255;
	bgColor[1] = 255;
	bgColor[2] = 255;
	SplashOutputDev *dev = new SplashOutputDev(splashModeXBGR8, 4, gFalse, bgColor, gTrue);
	dev->setVectorAntialias(gTrue);
	dev->setFreeTypeHinting(gTrue, gFalse);
	dev->startDoc(m_pdfDoc);
	m_pdfDoc->displayPage(dev, pgNum, hDPI, vDPI, 0, gTrue, gFalse, gFalse);
	SplashBitmap *bitmap = dev->getBitmap();
	int bw = bitmap->getWidth();
	int bh = bitmap->getHeight();
	SplashColorPtr dataPtr = bitmap->getDataPtr();
	if (QSysInfo::BigEndian == QSysInfo::ByteOrder)
	{
		uchar c;
		int count = bw * bh * 4;
		for (int k = 0; k < count; k += 4)
		{
			c = dataPtr[k];
			dataPtr[k] = dataPtr[k+3];
			dataPtr[k+3] = c;
			c = dataPtr[k+1];
			dataPtr[k+1] = dataPtr[k+2];
			dataPtr[k+2] = c;
		}
	}
	// construct a qimage SHARING the raw bitmap data in memory
	QImage tmpimg( dataPtr, bw, bh, QImage::Format_ARGB32 );
	QImage image = tmpimg.copy();
	image.setText("XSize", QString("%1").arg(w));
	image.setText("YSize", QString("%1").arg(h));
	if (box > Media_Box)
	{
		QRectF cRect = getCBox(box, pgNum);
		QRectF mediaRect = getCBox(0, pgNum);
		cRect.moveTo(cRect.x() - mediaRect.x(), cRect.y() - mediaRect.y());
		QPainter pp;
		pp.begin(&image);
		pp.setBrush(Qt::NoBrush);
		pp.setPen(QPen(Qt::red, 3.0));
		pp.translate(0, bh);
		pp.scale(scale, -scale);
		pp.drawRect(cRect);
		pp.end();
	}
	delete dev;
	return image;
}

QRectF PdfPlug::getCBox(int box, int pgNum)
{
	const PDFRectangle *cBox = nullptr;
	if (box == Media_Box)
		cBox = m_pdfDoc->getPage(pgNum)->getMediaBox();
	else if (box == Bleed_Box)
		cBox = m_pdfDoc->getPage(pgNum)->getBleedBox();
	else if (box == Trim_Box)
		cBox = m_pdfDoc->getPage(pgNum)->getTrimBox();
	else if (box == Crop_Box)
		cBox = m_pdfDoc->getPage(pgNum)->getCropBox();
	else if (box == Art_Box)
		cBox = m_pdfDoc->getPage(pgNum)->getArtBox();
	QRectF cRect = QRectF(QPointF(cBox->x1, cBox->y1), QPointF(cBox->x2, cBox->y2)).normalized();
	return cRect;
}

QString PdfPlug::UnicodeParsedString(POPPLER_CONST GooString *s1)
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
		// #15616: imagemagick may write unicode strings incorrectly in PDF
		if (u == 0)
			continue;
		result += QChar( u );
	}
	return result;
}

QString PdfPlug::UnicodeParsedString(const std::string& s1)
{
	if (s1.length() == 0)
		return QString();
	GBool isUnicode;
	size_t i;
	Unicode u;
	QString result;
	if ((s1.at(0) & 0xff) == 0xfe && (s1.length() > 1 && (s1.at(1) & 0xff) == 0xff))
	{
		isUnicode = gTrue;
		i = 2;
		result.reserve((s1.length() - 2) / 2);
	}
	else
	{
		isUnicode = gFalse;
		i = 0;
		result.reserve(s1.length());
	}
	while (i < s1.length())
	{
		if (isUnicode)
		{
			u = ((s1.at(i) & 0xff) << 8) | (s1.at(i+1) & 0xff);
			i += 2;
		}
		else
		{
			u = s1.at(i) & 0xff;
			++i;
		}
		// #15616: imagemagick may write unicode strings incorrectly in PDF
		if (u == 0)
			continue;
		result += QChar( u );
	}
	return result;
}
