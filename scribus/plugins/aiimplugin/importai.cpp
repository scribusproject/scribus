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
#include <QStack>
#include <QTextStream>
#include <QDebug>

#include <cmath>
#include <cstdlib>
#include <tiffio.h>
#include <zlib.h>

#include "commonstrings.h"
#include "customfdialog.h"
#include "importai.h"
#include "loadsaveplugin.h"
#include "missing.h"
#include "multiprogressdialog.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "propertiespalette.h"
#include "rawimage.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"
#include "util_color.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"


#ifdef HAVE_PODOFO
	#include <podofo/podofo.h>
#endif

extern SCRIBUS_API ScribusQApp * ScQApp;

AIPlug::AIPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = NULL;
}

bool AIPlug::import(QString fNameIn, int flags, bool showProgress)
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
	QFileInfo bF(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(bF.absolutePath()+"/"+bF.baseName()));
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
	b = PrefsManager::instance()->appPrefs.PageWidth;
	h = PrefsManager::instance()->appPrefs.PageHeight;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	parseHeader(fName, x, y, b, h);
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
		m_Doc->documentInfo.setAuthor(docCreator);
		m_Doc->documentInfo.setPublisher(docOrganisation);
		m_Doc->documentInfo.setTitle(docTitle);
		m_Doc->documentInfo.setDate(docDate+" "+docTime);
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!interactive))
	{
		if (b-x > h-y)
			m_Doc->PageOri = 1;
		else
			m_Doc->PageOri = 0;
		m_Doc->m_pageSize = "Custom";
	}
	ColorList::Iterator it;
	for (it = CustColors.begin(); it != CustColors.end(); ++it)
	{
		if (!m_Doc->PageColors.contains(it.key()))
			m_Doc->PageColors.insert(it.key(), it.value());
	}
	Elements.clear();
	FPoint minSize = m_Doc->minCanvasCoordinate;
	FPoint maxSize = m_Doc->maxCanvasCoordinate;
	FPoint cOrigin = m_Doc->view()->canvasOrigin();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->ScriptRunning = true;
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
		{
			bool isGroup = true;
			int firstElem = -1;
			if (Elements.at(0)->Groups.count() != 0)
				firstElem = Elements.at(0)->Groups.top();
			for (int bx = 0; bx < Elements.count(); ++bx)
			{
				PageItem* bxi = Elements.at(bx);
				if (bxi->Groups.count() != 0)
				{
					if (bxi->Groups.top() != firstElem)
						isGroup = false;
				}
				else
					isGroup = false;
			}
			if (!isGroup)
			{
				double minx = 99999.9;
				double miny = 99999.9;
				double maxx = -99999.9;
				double maxy = -99999.9;
				uint lowestItem = 999999;
				uint highestItem = 0;
				for (int a = 0; a < Elements.count(); ++a)
				{
					Elements.at(a)->Groups.push(m_Doc->GroupCounter);
					PageItem* currItem = Elements.at(a);
					lowestItem = qMin(lowestItem, currItem->ItemNr);
					highestItem = qMax(highestItem, currItem->ItemNr);
					double x1, x2, y1, y2;
					currItem->getBoundingRect(&x1, &y1, &x2, &y2);
					minx = qMin(minx, x1);
					miny = qMin(miny, y1);
					maxx = qMax(maxx, x2);
					maxy = qMax(maxy, y2);
/*					double lw = currItem->lineWidth() / 2.0;
					if (currItem->rotation() != 0)
					{
						FPointArray pb;
						pb.resize(0);
						pb.addPoint(FPoint(currItem->xPos()-lw, currItem->yPos()-lw));
						pb.addPoint(FPoint(currItem->width()+lw*2.0, -lw, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
						pb.addPoint(FPoint(currItem->width()+lw*2.0, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
						pb.addPoint(FPoint(-lw, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
						for (uint pc = 0; pc < 4; ++pc)
						{
							minx = qMin(minx, pb.point(pc).x());
							miny = qMin(miny, pb.point(pc).y());
							maxx = qMax(maxx, pb.point(pc).x());
							maxy = qMax(maxy, pb.point(pc).y());
						}
					}
					else
					{
						minx = qMin(minx, currItem->xPos()-lw);
						miny = qMin(miny, currItem->yPos()-lw);
						maxx = qMax(maxx, currItem->xPos()-lw + currItem->width()+lw*2.0);
						maxy = qMax(maxy, currItem->yPos()-lw + currItem->height()+lw*2.0);
					} */
				}
				double gx = minx;
				double gy = miny;
				double gw = maxx - minx;
				double gh = maxy - miny;
				PageItem *high = m_Doc->Items->at(highestItem);
				int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
				PageItem *neu = m_Doc->Items->takeAt(z);
				m_Doc->Items->insert(lowestItem, neu);
				neu->Groups.push(m_Doc->GroupCounter);
				neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
				neu->isGroupControl = true;
				neu->groupsLastItem = high;
				neu->setTextFlowMode(PageItem::TextFlowDisabled);
				for (int a = 0; a < m_Doc->Items->count(); ++a)
				{
					m_Doc->Items->at(a)->ItemNr = a;
				}
				Elements.prepend(neu);
				m_Doc->GroupCounter++;
			}
		}
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->ScriptRunning = false;
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
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					m_Doc->m_Selection->addItem(Elements.at(dre), true);
				}
				m_Doc->m_Selection->delaySignalsOff();
				m_Doc->m_Selection->setGroupRect();
				m_Doc->view()->updatesOn(true);
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
				QMimeData* md = new QMimeData();
				md->setText(ss->WriteElem(m_Doc, m_Doc->view(), tmpSel));
				QDrag* dr = new QDrag(m_Doc->view()->viewport());
				dr->setMimeData(md);
#ifndef Q_WS_MAC
// see #2196
				m_Doc->itemSelection_DeleteItem(tmpSel);
#else
				qDebug("aiimport: leaving items on page");
#endif
//				m_Doc->view()->resizeContents(qRound((maxSize.x() - minSize.x()) * m_Doc->view()->scale()), qRound((maxSize.y() - minSize.y()) * m_Doc->view()->scale()));
//				m_Doc->view()->scrollBy(qRound((m_Doc->minCanvasCoordinate.x() - minSize.x()) * m_Doc->view()->scale()), qRound((m_Doc->minCanvasCoordinate.y() - minSize.y()) * m_Doc->view()->scale()));
//				m_Doc->minCanvasCoordinate = minSize;
//				m_Doc->maxCanvasCoordinate = maxSize;
//				m_Doc->adjustCanvas(qRound((maxSize.x() - minSize.x()) * m_Doc->view()->scale()), 
//									qRound((maxSize.y() - minSize.y()) * m_Doc->view()->scale()), 0, 0);
//				m_Doc->adjustCanvas(minSize, maxSize, true);
//				m_Doc->view()->setCanvasOrigin(cOrigin.x(), cOrigin.y());
				m_Doc->view()->updatesOn(true);
//				m_Doc->view()->updateCanvas();
				m_Doc->m_Selection->delaySignalsOff();
				const QPixmap& dragCursor = loadIcon("DragPix.xpm");
				dr->setDragCursor(dragCursor, Qt::CopyAction);
				dr->setDragCursor(dragCursor, Qt::MoveAction);
				dr->setDragCursor(dragCursor, Qt::LinkAction);
				dr->exec();
				delete ss;
				m_Doc->DragP = false;
				m_Doc->DraggedElem = 0;
				m_Doc->DragElements.clear();
			}
		}
		else
		{
			m_Doc->changed();
			m_Doc->reformPages();
			m_Doc->view()->updatesOn(true);
		}
		success = true;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->ScriptRunning = false;
		m_Doc->view()->updatesOn(true);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	if (interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if ((showProgress) && (!interactive))
		m_Doc->view()->DrawNew();
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
#if PODOFO_VERSION == 0 && PODOFO_MINOR == 5 && PODOFO_REVISION == 99
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
							char *Buffer;
							long bLen;
							stream->GetFilteredCopy(&Buffer, &bLen);
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
								char *Buffer;
								long bLen;
								stream->GetFilteredCopy(&Buffer, &bLen);
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
	QFileInfo bF(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(bF.absolutePath()+"/"+bF.baseName()));
	return true;
}

bool AIPlug::parseHeader(QString fName, double &x, double &y, double &b, double &h)
{
	QString tmp, BBox, tmp2, FarNam;
	ScColor cc;
//	double c, m, yc, k;
	bool found = false;
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
			if (tmp.startsWith("%%HiResBoundingBox:"))
			{
				found = true;
				BBox = tmp.remove("%%HiResBoundingBox:");
			}
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
			if (tmp.startsWith("%%EndComments"))
				break;
		}
		f.close();
		if (found)
		{
			QStringList bb = BBox.split(" ", QString::SkipEmptyParts);
			if (bb.count() == 4)
			{
				QTextStream ts2(&BBox, QIODevice::ReadOnly);
				ts2 >> x >> y >> b >> h;
			}
		}
		importColorsFromFile(fName, CustColors);
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
	ColorList::Iterator it;
	QTextStream Code(&data, QIODevice::ReadOnly);
	bool found = false;
	Code >> c;
	Code >> m;
	Code >> y;
	Code >> k;
	int Cc = qRound(c * 255);
	int Mc = qRound(m * 255);
	int Yc = qRound(y * 255);
	int Kc = qRound(k * 255);
	int hC, hM, hY, hK;
	tmp.setColor(Cc, Mc, Yc, Kc);
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelCMYK)
		{
			it.value().getCMYK(&hC, &hM, &hY, &hK);
			if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
			{
				ret = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString namPrefix = "FromAI";
		m_Doc->PageColors.insert(namPrefix+tmp.name(), tmp);
//		importedColors.append(namPrefix+tmp.name());
		ret = namPrefix+tmp.name();
	}
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
	QTextStream Code(&data, QIODevice::ReadOnly);
	bool found = false;
	Code >> k;
	int Kc = 255 - qRound(k * 255);
	int hC, hM, hY, hK;
	tmp.setColor(0, 0, 0, Kc);
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelCMYK)
		{
			it.value().getCMYK(&hC, &hM, &hY, &hK);
			if ((hC == 0) && (hM == 0) && (hY == 0) && (Kc == hK))
			{
				ret = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString namPrefix = "FromAI";
		m_Doc->PageColors.insert(namPrefix+tmp.name(), tmp);
//		importedColors.append(namPrefix+tmp.name());
		ret = namPrefix+tmp.name();
	}
	return ret;
}

QString AIPlug::parseColorRGB(QString data)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double r, g, b;
	ScColor tmp;
	ColorList::Iterator it;
	QTextStream Code(&data, QIODevice::ReadOnly);
	bool found = false;
	Code >> r;
	Code >> g;
	Code >> b;
	int Rc = qRound(r * 255);
	int Gc = qRound(g * 255);
	int Bc = qRound(b * 255);
	int hR, hG, hB;
	tmp.setColorRGB(Rc, Gc, Bc);
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&hR, &hG, &hB);
			if ((Rc == hR) && (Gc == hG) && (Bc == hB))
			{
				ret = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString namPrefix = "FromAI";
		m_Doc->PageColors.insert(namPrefix+tmp.name(), tmp);
//		importedColors.append(namPrefix+tmp.name());
		ret = namPrefix+tmp.name();
	}
	return ret;
}

QString AIPlug::parseCustomColor(QString data, double &shade)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double c, m, y, k, sh;
	ScColor tmp;
	ColorList::Iterator it;
	QTextStream Code(&data, QIODevice::ReadOnly);
	bool found = false;
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
	QTextStream Val(&FarSha, QIODevice::ReadOnly);
	Val >> sh;
	shade = (1.0 - sh) * 100.0;
	int Cc = qRound(c * 255);
	int Mc = qRound(m * 255);
	int Yc = qRound(y * 255);
	int Kc = qRound(k * 255);
	int hC, hM, hY, hK;
	tmp.setColor(Cc, Mc, Yc, Kc);
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelCMYK)
		{
			it.value().getCMYK(&hC, &hM, &hY, &hK);
			if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
			{
				ret = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		tmp.setSpotColor(true);
		tmp.setRegistrationColor(false);
		m_Doc->PageColors.insert(FarNam, tmp);
//		importedColors.append(FarNam);
		ret = FarNam;
	}
	return ret;
}

QString AIPlug::parseCustomColorX(QString data, double &shade, QString type)
{
	QString ret = CommonStrings::None;
	if (data.isEmpty())
		return ret;
	double c, m, y, k, sh, r, g, b;
	ScColor tmp;
	ColorList::Iterator it;
	QTextStream Code(&data, QIODevice::ReadOnly);
	bool found = false;
	if (type == "1")
	{
		Code >> r;
		Code >> g;
		Code >> b;
		int Rc = qRound(r * 255);
		int Gc = qRound(g * 255);
		int Bc = qRound(b * 255);
		int hR, hG, hB;
		tmp.setColorRGB(Rc, Gc, Bc);
		for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
		{
			if (it.value().getColorModel() == colorModelRGB)
			{
				it.value().getRGB(&hR, &hG, &hB);
				if ((Rc == hR) && (Gc == hG) && (Bc == hB))
				{
					ret = it.key();
					found = true;
					break;
				}
			}
		}
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
		int hC, hM, hY, hK;
		tmp.setColor(Cc, Mc, Yc, Kc);
		for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
		{
			if (it.value().getColorModel() == colorModelCMYK)
			{
				it.value().getCMYK(&hC, &hM, &hY, &hK);
				if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
				{
					ret = it.key();
					found = true;
					break;
				}
			}
		}
	}
	QString tmpS = data;
	int an = data.indexOf("(");
	int en = data.lastIndexOf(")");
	QString FarNam = data.mid(an+1, en-an-1);
	FarNam.remove("\\");
	QString FarSha = data.mid(en+1, data.size() - en);
	QTextStream Val(&FarSha, QIODevice::ReadOnly);
	Val >> sh;
	shade = (1.0 - sh) * 100.0;
	if (!found)
	{
		if (type == "0")
			tmp.setSpotColor(true);
		tmp.setRegistrationColor(false);
		m_Doc->PageColors.insert(FarNam, tmp);
//		importedColors.append(FarNam);
		ret = FarNam;
	}
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

void AIPlug::processData(QString data)
{
	double x, y, x1, y1, x2, y2;
	int z, tmpInt;
	PageItem *ite;
	QString command = "";
	QString Cdata = "";
	QStringList da;
	getCommands(data, da);
	for (int a = 0; a < da.count(); a++)
	{
		Cdata = da[a];
		if ((Cdata.startsWith("%")) || (Cdata.startsWith(" %")))
			continue;
		QStringList da2 = Cdata.split(" ", QString::SkipEmptyParts);
		if (da2.count() == 0)
			return;
		command = da2[da2.count()-1];
/* Start Path construction commands */
		if (command == "m")
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y;
			Coords.svgMoveTo(x - docX, docHeight - (y - docY));
			currentPoint = FPoint(x - docX, docHeight - (y - docY));
		}
		else if ((command == "L") || (command == "l"))
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y;
			Coords.svgLineTo(x - docX, docHeight - (y - docY));
			currentPoint = FPoint(x - docX, docHeight - (y - docY));
		}
		else if ((command == "C") || (command == "c"))
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y >> x1 >> y1 >> x2 >> y2;
			Coords.svgCurveToCubic(x - docX, docHeight - (y - docY),
								   x1 - docX, docHeight - (y1 - docY),
								   x2 - docX, docHeight - (y2 - docY));
			currentPoint = FPoint(x2 - docX, docHeight - (y2 - docY));
		}
		else if ((command == "Y") || (command == "y"))
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x1 >> y1 >> x2 >> y2;
			Coords.svgCurveToCubic(currentPoint.x(), currentPoint.y(),
								   x1 - docX, docHeight - (y1 - docY),
								   x2 - docX, docHeight - (y2 - docY));
			currentPoint = FPoint(x2 - docX, docHeight - (y2 - docY));
		}
		else if ((command == "V") || (command == "v"))
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> x >> y >> x2 >> y2;
			Coords.svgCurveToCubic(x - docX, docHeight - (y - docY),
								   currentPoint.x(), currentPoint.y(),
								   x2 - docX, docHeight - (y2 - docY));
			currentPoint = FPoint(x2 - docX, docHeight - (y2 - docY));
		}
/* End Path construction commands */
/* Start Object creation commands */
		else if ((command == "b") || (command == "B") || (command == "f") || (command == "F") || (command == "s") || (command == "S"))
		{
			if (Coords.size() > 3)
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
	//				ite->setFillBlendmode(blendMode);
	//				ite->setLineBlendmode(blendMode);
					if (!currentPatternName.isEmpty())
					{
						ite->setPattern(currentPatternName);
						ite->setPatternTransform(currentPatternXScale, currentPatternYScale, currentPatternX, currentPatternY, currentPatternRotation);
						currentPatternName = "";
					}
					ite->setLineEnd(CapStyle);
					ite->setLineJoin(JoinStyle);
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
						ite->setLocked(itemLocked);
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
					
				}
				else
				{
					ite = m_Doc->Items->at(m_Doc->Items->count()-1);
					ite->PoLine.setMarker();
					Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					ite->PoLine.putPoints(ite->PoLine.size(), Coords.size(), Coords);
				}
				Coords.resize(0);
				Coords.svgInit();
				FirstU = false;
				itemRendered = true;
			}
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
		}
		else if ((command == "U") || (command == "Q"))
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
					ite = tmpSel->itemAt(0);
					if ((clipCoords.size() > 4) && (command == "Q"))
					{
						ite = tmpSel->itemAt(0);
						clipCoords.translate(m_Doc->currentPage()->xOffset()-ite->xPos(), m_Doc->currentPage()->yOffset()-ite->yPos());
						ite->PoLine = clipCoords.copy();
						ite->PoLine.translate(baseX, baseY);
					}
				if (patternMode)
					PatternElements.append(ite);
				else
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
			if (command == "Q")
			{
				clipCoords.resize(0);
				clipCoords.svgInit();
			}
		}
		else if (command == "W")
		{
			if (clipCoords.size() > 0)
				clipCoords.setMarker();
			clipCoords.putPoints(clipCoords.size(), Coords.size(), Coords);
		}
/* End Object construction commands */
/* Start Graphics state commands */
		else if (command == "A")
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> tmpInt;
			if (tmpInt == 1)
				itemLocked = true;
			else
				itemLocked = false;
		}
		else if (command == "w")
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> LineW;
		}
		else if (command == "j")
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
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
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
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
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> blendMode >> Opacity;
		}
		else if (command == "XR")
		{
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
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
			startMatrix = QMatrix();
			endMatrix = QMatrix();
		}
		else if (command == "Xm")
		{
			QTextStream gVals(&Cdata, QIODevice::ReadOnly);
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
				QTextStream gVals(&Cdata, QIODevice::ReadOnly);
				double m1, m2, m3, m4, m5, m6;
				gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
				startMatrix.translate(m5, -m6);
//				endMatrix.scale(m1, m4);
				endMatrix *= QMatrix(m1, m2, m3, m4, 0, 0);
//				endMatrix = QMatrix(m1, m2, m3, m4, m5, m6);
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
				if (ite->fill_gradient.type() == 0)
					ite->GrType = 6;
				else
					ite->GrType = 7;
				QMatrix m1;
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
					QMatrix m2;
//					m2.translate(ite->GrStartX, ite->GrStartY);
					m2.rotate(-currentGradientAngle);
					m2 *= endMatrix;
					QPointF target = m2.map(QPointF(currentGradientLenght, 0.0));
					ite->GrEndX = target.x();
					ite->GrEndY = target.y();
				}
			}
			wasBC = false;
			currentGradientMatrix = QMatrix();
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
			QTextStream gVals(&tmpS, QIODevice::ReadOnly);
			double xOrig, yOrig, m1, m2, m3, m4, m5, m6;
			gVals >> xOrig >> yOrig >> currentGradientAngle >> currentGradientLenght >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			currentGradientOrigin = QPointF(xOrig - docX, docHeight - (yOrig - docY));
			currentGradientMatrix = QMatrix(m1, m2, m3, m4, m5, m6);
		}
/* End Graphics state commands */
/* Start Color commands */
		else if ((command == "G") || (command == "g"))
		{
			if (command == "G")
				CurrColorStroke = parseColorGray(Cdata);
			else
				CurrColorFill = parseColorGray(Cdata);
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
			QTextStream gVals(&tmpS, QIODevice::ReadOnly);
			gVals >> currentPatternX >> currentPatternY >> currentPatternXScale >> currentPatternYScale >> currentPatternRotation;
		}
/* End Color commands */
/* Start Layer commands */
		else if (command == "Lb")
		{
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				int visible, preview, enabled, printing, dummy, rc, gc, bc;
				QTextStream ts2(&Cdata, QIODevice::ReadOnly);
				ts2 >> visible >> preview >> enabled >> printing >> dummy >> dummy >> dummy >> rc >> gc >> bc;
				if (!firstLayer)
					currentLayer = m_Doc->addLayer("Layer", true);
				m_Doc->setLayerVisible(currentLayer, static_cast<bool>(visible));
				m_Doc->setLayerOutline(currentLayer, static_cast<bool>(!preview));
				m_Doc->setLayerLocked(currentLayer, static_cast<bool>(!enabled));
				m_Doc->setLayerPrintable(currentLayer, static_cast<bool>(printing));
				m_Doc->setLayerMarker(currentLayer, QColor(rc, gc, bc));
				firstLayer = false;
			}
			Coords.resize(0);
			Coords.svgInit();
		}
		else if (command == "LB")
		{
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
			QTextStream ts2(&Cdata, QIODevice::ReadOnly);
			ts2 >> textMode;
			textData.clear();
			textMatrix = QMatrix();
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
			QTextStream gVals(&Cdata, QIODevice::ReadOnly);
			double m1, m2, m3, m4, m5, m6;
			gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6;
			textMatrix = QMatrix(m1, m2, m3, m4, m5, m6);
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
			QTextStream gVals(&Cdata, QIODevice::ReadOnly);
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
			QTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> textKern;
			textKern *= 100.0;
		}
		else if (command == "Tz")
		{
			QTextStream gVals(&Cdata, QIODevice::ReadOnly);
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
			QTextStream gVals(&Cdata, QIODevice::ReadOnly);
			gVals >> textFont >> textSize;
			textFont.remove(0, 2);
			QString family = textFont;
			QString ret = "";
			family.replace( QRegExp( "'" ) , QChar( ' ' ) );
			textFont = m_Doc->toolSettings.defFont;
			bool found = false;
			SCFontsIterator it(PrefsManager::instance()->appPrefs.AvailFonts);
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
				if (!PrefsManager::instance()->appPrefs.GFontSub.contains(family))
				{
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					MissingFont *dia = new MissingFont(0, family, m_Doc);
					dia->exec();
					QString tmpf = dia->getReplacementFont();
					delete dia;
					qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
					PrefsManager::instance()->appPrefs.GFontSub[family] = tmpf;
				}
				else
					textFont = PrefsManager::instance()->appPrefs.GFontSub[family];
			}
			textSize *= 10.0;
		}
		else if (command == "TO")
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
/* End Text commands */
/* Start special Commands */
		else if (command == "*")
		{
			Coords.resize(0);
			Coords.svgInit();
		}
		else if ((command == "N") || (command == "n"))
		{
			Coords.resize(0);
			Coords.svgInit();
		}
		else if (command == "[")
		{
			int an = Cdata.indexOf("(");
			int en = Cdata.lastIndexOf(")");
			if ((an != -1) && (en != -1))
			{
				patternMode = true;
				currentPatternDefName = Cdata.mid(an+1, en-an-1);
				currentPatternDefName.remove("\\");
				currentPatternDefName = currentPatternDefName.trimmed().simplified().replace(" ", "_");
				QString tmpS = Cdata.mid(en+1, Cdata.size() - en);
				QTextStream gVals(&tmpS, QIODevice::ReadOnly);
				gVals >> patternX1 >> patternY1 >> patternX2 >> patternY2;
			}
		}
/* End special Commands */
/* Skip everything else */
//		else
//			qDebug(command);
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
			double stop = da2[da2.count()-2].toDouble() / 100.0;
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
		}
		else if (command == "BD")
		{
			m_gradients.insert(currentGradientName, currentGradient);
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
					}
					if (PatternElements.count() > 1)
						m_Doc->itemSelection_GroupObjects(false, false, tmpSel);
					ScPattern pat = ScPattern();
					pat.setDoc(m_Doc);
					PageItem* currItem = tmpSel->itemAt(0);
					currItem->setItemName(currentPatternDefName);
					currItem->AutoName = false;
					m_Doc->DoDrawing = true;
					QImage tmpImg = currItem->DrawObj_toImage();
					QImage retImg = QImage(qRound(patternX2 - patternX1), qRound(patternY2 - patternY1), QImage::Format_ARGB32);
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
					for (int as = 0; as < tmpSel->count(); ++as)
					{
						pat.items.append(tmpSel->itemAt(as));
					}
					m_Doc->itemSelection_DeleteItem(tmpSel);
					m_Doc->addPattern(currentPatternDefName, pat);
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
			else
			{
				if (tmp.startsWith("("))
					tmp.remove(0, 1);
				tmpData += " "+tmp;
			}
		}
		else if (tmp == "EndPattern")
		{
			PatternElements.clear();
			currentPatternDefName = "";
			break;
		}
		else
			processData(tmp);
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
	QTextStream gVals(&Cdata, QIODevice::ReadOnly);
	gVals >> m1 >> m2 >> m3 >> m4 >> m5 >> m6 >> x1 >> y1 >> x2 >> y2 >> w >> h >> bits >> type >> alpha >> dummy >> bin;
//	qDebug(QString("Matrix: %1 %2 %3 %4 %5 %6").arg(m1).arg(m2).arg(m3).arg(m4).arg(m5).arg(m6));
//	qDebug(QString("Bounds: %1 %2 %3 %4").arg(x1).arg(y1).arg(x2).arg(y2));
//	qDebug(QString("Size: %1 %2").arg(w).arg(h));
//	qDebug(QString("Bits: %1").arg(bits));
//	qDebug(QString("Typ: %1").arg(type));
//	qDebug(QString("Alpha-Channels: %1").arg(alpha));
//	qDebug(QString("Encoding: %1").arg(bin));
	uint dataSize = w * h * (type + alpha);
	uint alphaData = w * h * type;
	bool cmyk = false;
	if (type == 4)
		cmyk = true;
	if (tmp.startsWith("%%BeginData"))
	{
		QString dummyS;
		QTextStream gVals2(&tmp, QIODevice::ReadOnly);
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
	uchar *p;
	uint yCount = 0;
	QString imgName = baseFile + QString("-Img-%1").arg(imgNum) + ".tif";
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
	QMatrix imgMatrix = QMatrix(m1, m2, m3, m4, m5, m6);
	QPointF pos = QPointF(imgMatrix.dx(), imgMatrix.dy());
	pos += QPointF(m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
	pos += QPointF(baseX, -baseY);
	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, pos.x() - docX, docHeight - (pos.y() - docY), 10, 10, 0, CurrColorFill, CurrColorStroke, true);
	PageItem* ite = m_Doc->Items->at(z);
	ite->setWidthHeight(w * m1, h * m4);
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
	ite->setLineEnd(CapStyle);
	ite->setLineJoin(JoinStyle);
	m_Doc->LoadPict(imgName, z);
	if (ite->PicAvail)
		ite->setImageXYScale(ite->width() / ite->pixm.width(), ite->height() / ite->pixm.height());
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
		ite->setLocked(itemLocked);
					if (patternMode)
						PatternElements.append(ite);
					else
	Elements.append(ite);
//	if (importerFlags & LoadSavePlugin::lfCreateDoc)
//	{
		if (groupStack.count() != 0)
			groupStack.top().append(ite);
//	}
	imgNum++;
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
			if (tmp.startsWith("End_NonPrinting"))
				break;
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
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
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.startsWith("BeginPalette"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndPalette"))
				break;
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.startsWith("BeginSymbol"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndSymbol"))
				break;
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.startsWith("BeginDocumentData"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("EndDocumentData"))
				break;
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.startsWith("%%BeginProlog"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("%%EndProlog"))
				break;
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.startsWith("%%BeginData"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("%%EndData"))
				break;
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.startsWith("%%BeginCrops"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("%%EndCrops"))
				break;
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	else if (tmp.startsWith("BeginRaster"))
	{
		processRaster(ts);
		progressDialog->setProgress("GI", ts.device()->pos());
		qApp->processEvents();
	}
	else if (tmp.startsWith("BeginLayer"))
	{
		while (!ts.atEnd())
		{
			tmp = removeAIPrefix(readLinefromDataStream(ts));
			if (tmp.startsWith("BeginRaster"))
			{
				processRaster(ts);
				continue;
			}
			if (tmp.startsWith("EndLayer"))
				break;
			else
				processData(tmp);
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
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
	itemLocked = false;
	patternX1 = 0.0;
	patternY1 = 0.0;
	patternX2 = 0.0;
	patternY2 = 0.0;
	Coords.resize(0);
	Coords.svgInit();
	clipCoords.resize(0);
	clipCoords.svgInit();
	currentPoint = FPoint(0.0, 0.0);
	currentLayer = 0;
	currentGradient = VGradient(VGradient::linear);
	currentGradient.clearStops();
	currentGradient.setRepeatMethod( VGradient::none );
	currentGradientName = "";
	currentGradientMatrix = QMatrix();
	currentGradientOrigin = QPointF(0.0, 0.0);
	currentGradientAngle = 0.0;
	currentGradientLenght = 1.0;
	currentPatternName = "";
	currentPatternX = 0.0;
	currentPatternY = 0.0;
	currentPatternXScale = 1.0;
	currentPatternYScale = 1.0;
	currentPatternRotation = 0.0;
	QList<PageItem*> gElements;
	groupStack.push(gElements);
	imgNum = 0;
	commandList << "m" << "l" << "L" << "c" << "C" << "v" << "V" << "y" << "Y";		// Path construction
	commandList << "b" << "B" << "f" << "F" << "s" << "S" << "*u" << "*U";			// Object creation
	commandList << "u" << "U" << "W" << "q" << "Q";									// Object creation
	commandList << "A" << "w" << "j" << "J" << "Xy" << "XR";						// Graphic state
	commandList << "k" << "K" << "Xa" << "XA" << "x" << "X" << "XX" << "Xx";		// Color commands
	commandList << "Xk" << "g" << "G" << "p";										// Color commands
	commandList << "Ln" << "Lb" << "LB";											// Layer commands
	commandList << "Bd" << "BD" << "%_Bs" << "Bg" << "Bb" << "BB" << "Bm" << "Xm";	// Gradient commands
	commandList << "To" << "TO" << "Tf" << "Tp" << "Tx" << "TX" << "T*" << "Tk";	// Text commands
	commandList << "Tc" << "Tz";													// Text commands
	commandList << "XI" << "XG" << "Xh";											// Image commands
	commandList << "n" << "N" << "*" << "[";										// Special commands
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
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}
