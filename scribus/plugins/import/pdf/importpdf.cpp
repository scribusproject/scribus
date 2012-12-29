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
#include <QInputDialog>
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QStack>
#include <QDebug>
#include "slaoutput.h"
#include <poppler/ErrorCodes.h>
#include <poppler/GlobalParams.h>
#include <poppler/PageTransition.h>
#include <poppler/ViewerPreferences.h>
#include <poppler/poppler-config.h>
#include <poppler/SplashOutputDev.h>
#include <poppler/splash/SplashBitmap.h>

#include "importpdf.h"

#include <cstdlib>

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
//#include "util_ghostscript.h"
#include "util_icon.h"
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
	progressDialog = NULL;
	m_pdfDoc = NULL;
}

QImage PdfPlug::readThumbnail(QString fName)
{
	QString pdfFile = QDir::toNativeSeparators(fName);
	globalParams = new GlobalParams();
	if (globalParams)
	{
		GooString *fname = new GooString(QFile::encodeName(pdfFile).data());
		globalParams->setErrQuiet(gTrue);
		PDFDoc *pdfDoc = new PDFDoc(fname, NULL, NULL, NULL);
		if (pdfDoc)
		{
			if (pdfDoc->getErrorCode() == errEncrypted)
			{
				delete pdfDoc;
				delete globalParams;
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
				SplashOutputDev *dev = new SplashOutputDev(splashModeXBGR8, 4, gFalse, bgColor, gTrue, gTrue);
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
				delete globalParams;
				return image;
			}
			else
			{
				delete pdfDoc;
				delete globalParams;
				return QImage();
			}
		}
		else
			return QImage();
	}
	return QImage();
}
/*	Old Code to be backed up
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
*/

bool PdfPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double b, h;
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
			m_Doc=ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, 0, 0, 0, 0, 1, "Custom", true);
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
		if ((Elements.count() == 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
		{
			PageItem *gr = Elements[0];
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
	bool firstPg = true;
	int currentLayer = m_Doc->activeLayer();
	int baseLayer = m_Doc->activeLayer();
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
	GooString *userPW = NULL;
	if (globalParams)
	{
		GooString *fname = new GooString(QFile::encodeName(fn).data());
		globalParams->setErrQuiet(gTrue);
		GBool hasOcg = gFalse;
		QList<OptionalContentGroup*> ocgGroups;
//		globalParams->setPrintCommands(gTrue);
		PDFDoc *pdfDoc = new PDFDoc(fname, NULL, NULL, NULL);
		if (pdfDoc)
		{
			if (pdfDoc->getErrorCode() == errEncrypted)
			{
				delete pdfDoc;
				pdfDoc = NULL;
				if (progressDialog)
					progressDialog->hide();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				ScribusMainWindow* mw = (m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
				bool ok;
				QString text = QInputDialog::getText(mw, tr("Open PDF-File"), tr("Password"), QLineEdit::Normal, "", &ok);
				if (ok && !text.isEmpty())
				{
					fname = new GooString(QFile::encodeName(fn).data());
					userPW = new GooString(text.toLocal8Bit().data());
					pdfDoc = new PDFDoc(fname, userPW, userPW, NULL);
					qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
				}
				if ((!pdfDoc) || (pdfDoc->getErrorCode() != errNone))
				{
					if (progressDialog)
						progressDialog->close();
					delete pdfDoc;
					delete globalParams;
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
				GBool crop = gFalse;
				GBool printing = gFalse;
				PDFRectangle *mediaBox = pdfDoc->getPage(1)->getMediaBox();
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
						delete globalParams;
						return false;
					}
					pageString = optImp->getPagesString();
					int cb = optImp->getCropBox();
					if (cb > Media_Box)
					{
						cropped = true;
						contentRect = cb;
					}
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
									Object orderItem;
									order->get(i, &orderItem);
									if (orderItem.isDict())
									{
										Object ref;
										order->getNF(i, &ref);
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
										ref.free();
									}
									else
									{
										GooList *ocgs;
										int i;
										ocgs = ocg->getOCGs ();
										for (i = 0; i < ocgs->getLength (); ++i)
										{
											OptionalContentGroup *oc = (OptionalContentGroup *)ocgs->get(i);
											QString ocgName = UnicodeParsedString(oc->getName());
											if (!ocgNames.contains(ocgName))
											{
												ocgGroups.prepend(oc);
												ocgNames.append(ocgName);
											}
										}
									}
								}
							}
							else
							{
								GooList *ocgs;
								int i;
								ocgs = ocg->getOCGs ();
								for (i = 0; i < ocgs->getLength (); ++i)
								{
									OptionalContentGroup *oc = (OptionalContentGroup *)ocgs->get(i);
									QString ocgName = UnicodeParsedString(oc->getName());
									if (!ocgNames.contains(ocgName))
									{
										ocgGroups.prepend(oc);
										ocgNames.append(ocgName);
									}
								}
							}
						}
					}
					bool rotated = false;
					dev->startDoc(pdfDoc, pdfDoc->getXRef(), pdfDoc->getCatalog());
					int rotate = pdfDoc->getPageRotate(firstPage);
					if ((rotate == 90) || (rotate == 270))
						rotated = true;
					rotate = 0;
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
// POPPLER_VERSION appeared in 0.19.0 first
#ifdef POPPLER_VERSION
						if (hasOcg)
						{
							QString actL = m_Doc->activeLayerName();
							for (int a = 0; a < ocgGroups.count(); a++)
							{
								OptionalContentGroup *oc = ocgGroups[a];
								if (actL != UnicodeParsedString(oc->getName()))
									currentLayer = m_Doc->addLayer(UnicodeParsedString(oc->getName()), false);
								else
									currentLayer = m_Doc->layerIDFromName(UnicodeParsedString(oc->getName()));
// POPPLER_VERSION appeared in 0.19.0 first
#ifdef POPPLER_VERSION
								if ((oc->getViewState() == OptionalContentGroup::ocUsageOn) || (oc->getViewState() == OptionalContentGroup::ocUsageUnset))
									m_Doc->setLayerVisible(currentLayer, true);
								else
									m_Doc->setLayerVisible(currentLayer, false);
								if ((oc->getPrintState() == OptionalContentGroup::ocUsageOn) || (oc->getPrintState() == OptionalContentGroup::ocUsageUnset))
									m_Doc->setLayerPrintable(currentLayer, true);
								else
									m_Doc->setLayerPrintable(currentLayer, false);
#else
								if (oc->getState() == OptionalContentGroup::On)
								{
									m_Doc->setLayerVisible(currentLayer, true);
									m_Doc->setLayerPrintable(currentLayer, true);
								}
								else
								{
									m_Doc->setLayerVisible(currentLayer, false);
									m_Doc->setLayerPrintable(currentLayer, false);
								}
#endif
								oc->setState(OptionalContentGroup::Off);
							}
							dev->layersSetByOCG = true;
						}
#endif
						Object info;
						pdfDoc->getDocInfo(&info);
						if (info.isDict())
						{
							Object obj;
							GooString *s1;
							Dict *infoDict = info.getDict();
							if (infoDict->lookup((char*)"Title", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setTitle(UnicodeParsedString(obj.getString()));
								obj.free();
							}
							if (infoDict->lookup((char*)"Author", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setAuthor(UnicodeParsedString(obj.getString()));
								obj.free();
							}
							if (infoDict->lookup((char*)"Subject", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setSubject(UnicodeParsedString(obj.getString()));
								obj.free();
							}
							if (infoDict->lookup((char*)"Keywords", &obj )->isString())
							{
								s1 = obj.getString();
								m_Doc->documentInfo().setKeywords(UnicodeParsedString(obj.getString()));
								obj.free();
							}
						}
						info.free();
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
						m_Doc->pdfOptions().PresentVals.clear();
						for (uint ap = 0; ap < pageNs.size(); ++ap)
						{
							int pp = pageNs[ap];
							m_Doc->setActiveLayer(baseLayer);
							if (firstPg)
								firstPg = false;
							else
								m_Doc->addPage(ap);
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
							m_Doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
							m_Doc->currentPage()->m_pageSize = "Custom";
							m_Doc->reformPages(true);
							if (hasOcg)
							{
								for (int a = 0; a < ocgGroups.count(); a++)
								{
									OptionalContentGroup *oc = ocgGroups[a];
								//	m_Doc->setActiveLayer(UnicodeParsedString(oc->getName()));
								//	currentLayer = m_Doc->activeLayer();
									oc->setState(OptionalContentGroup::On);
								//	pdfDoc->displayPage(dev, pp + 1, hDPI, vDPI, rotate, useMediaBox, crop, printing);
								//	oc->setState(OptionalContentGroup::Off);
								}
								if (cropped)
									pdfDoc->displayPageSlice(dev, pp, hDPI, vDPI, rotate, useMediaBox, crop, printing, crBox.x(), crBox.y(), crBox.width(), crBox.height(), NULL, NULL, dev->annotations_callback, dev);
								else
									pdfDoc->displayPage(dev, pp, hDPI, vDPI, rotate, useMediaBox, crop, printing, NULL, NULL, dev->annotations_callback, dev);
							}
							else
							{
								if (cropped)
									pdfDoc->displayPageSlice(dev, pp, hDPI, vDPI, rotate, useMediaBox, crop, printing, crBox.x(), crBox.y(), crBox.width(), crBox.height(), NULL, NULL, dev->annotations_callback, dev);
								else
									pdfDoc->displayPage(dev, pp, hDPI, vDPI, rotate, useMediaBox, crop, printing, NULL, NULL, dev->annotations_callback, dev);
							}
							PDFPresentationData ef;
							Object trans;
							Object *transi = pdfDoc->getPage(pp)->getTrans(&trans);
							if (transi->isDict())
							{
								m_Doc->pdfOptions().PresentMode = true;
								PageTransition *pgTrans = new PageTransition(transi);
								ef.pageViewDuration = pdfDoc->getPage(pp + 1)->getDuration();
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
							m_Doc->pdfOptions().PresentVals.append(ef);
							trans.free();
							transi->free();
						}
						int numjs = pdfDoc->getCatalog()->numJS();
						if (numjs > 0)
						{
							NameTree *jsNameTreeP = new NameTree();
							Object catDict;
							pdfDoc->getXRef()->getCatalog(&catDict);
							if (catDict.isDict())
							{
								Object names;
								catDict.dictLookup("Names", &names);
								if (names.isDict())
								{
									Object obj;
									names.dictLookup("JavaScript", &obj);
									jsNameTreeP->init(pdfDoc->getXRef(), &obj);
									obj.free();
								}
								for (int a = 0; a < numjs; a++)
								{
									m_Doc->JavaScripts.insert(UnicodeParsedString(jsNameTreeP->getName(a)), UnicodeParsedString(pdfDoc->getCatalog()->getJS(a)));
								}
								names.free();
								catDict.dictLookup("OpenAction", &names);
								if (names.isDict())
								{
									LinkAction *linkAction = NULL;
									linkAction = LinkAction::parseAction(&names, pdfDoc->getCatalog()->getBaseURI());
									if (linkAction)
									{
										LinkJavaScript *jsa = (LinkJavaScript*)linkAction;
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
								names.free();
							}
							catDict.free();
							delete jsNameTreeP;
						}
						m_Doc->pdfOptions().Version = (PDFOptions::PDFVersion)qMin(15, qMax(13, pdfDoc->getPDFMajorVersion() * 10 + pdfDoc->getPDFMinorVersion()));
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
						else if (pgl == Catalog::pageLayoutTwoColumnLeft)
							m_Doc->pdfOptions().PageLayout = PDFOptions::TwoColumnLeft;
						else if (pgl == Catalog::pageLayoutTwoColumnRight)
							m_Doc->pdfOptions().PageLayout = PDFOptions::TwoColumnRight;
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
						pdfDoc->displayPage(dev, firstPage, hDPI, vDPI, rotate, useMediaBox, crop, printing, NULL, NULL, dev->annotations_callback, dev);
					}
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
	SplashOutputDev *dev = new SplashOutputDev(splashModeXBGR8, 4, gFalse, bgColor, gTrue, gTrue);
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
		QPainter pp;
		pp.begin(&image);
		pp.setBrush(Qt::NoBrush);
		pp.setPen(QPen(Qt::red, 1.0));
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
	PDFRectangle *cBox;
	if (box == Bleed_Box)
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
