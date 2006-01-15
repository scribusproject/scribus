#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtl.h>
#include <qcursor.h>
#include <qregexp.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qprogressbar.h>
#include <cstdlib>
#include <cmath>
#include <qtextstream.h>

#include "commonstrings.h"
#include "fileloader.h"
#include "fileloader.moc"
#include "fontreplacedialog.h"
#include "missing.h"
#include "page.h"
#include "pluginmanager.h"
#include "pagestructs.h"
#include "scconfig.h"
#include "scribus.h"
#include "scribusXml.h"
#include "units.h"
#include "loadsaveplugin.h"

// We need to include the headers for the plugins we support until we start
// using LoadSavePlugin to pick them for us. We only use these headers to
// get the format IDs, NOTHING ELSE.
#include "plugins/svgimplugin/svgplugin.h"
#include "plugins/psimport/importpsplugin.h"
#include "plugins/fileloader/oodraw/oodrawimp.h"

#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
#include "util.h"
#include "prefsmanager.h"

/*!
 \fn FileLoader::FileLoader(QString fileName)
 \author Franz Schmid
 \date
 \brief Constructor, sets the variable "FileName" to the input parameter fileName
 \param QString fileName
 \retval None
 */
FileLoader::FileLoader(const QString & fileName) :
	QObject(0, "FileLoader"),
	FileName(fileName),
	FileType(-1),
	formatPS(LoadSavePlugin::getFormatById(FORMATID_PSIMPORT)),
	formatSVG(LoadSavePlugin::getFormatById(FORMATID_SVGIMPORT)),
	formatSXD(LoadSavePlugin::getFormatById(FORMATID_SXDIMPORT)),
	formatODG(LoadSavePlugin::getFormatById(FORMATID_ODGIMPORT)),
	prefsManager(PrefsManager::instance())
{
}

// FIXME: This static method is here as a temporary transitional
// measure during the process of converting to file loader plugins.
const QString FileLoader::getLoadFilterString()
{
	/*
	PluginManager& pluginManager(PluginManager::instance());
	QString formats;
#ifdef HAVE_LIBZ
	formats += tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;");
#else
	formats += tr("Documents (*.sla *.scd);;");
#endif
	if (pluginManager.DLLexists("importps"))
		formats += tr("PostScript Files (*.eps *.EPS *.ps *.PS);;");
	if (pluginManager.DLLexists("svgimplugin"))
#ifdef HAVE_LIBZ
		formats += tr("SVG Images (*.svg *.svgz);;");
#else
		formats += tr("SVG Images (*.svg);;");
#endif
	if (pluginManager.DLLexists("oodrawimp"))
		formats += tr("OpenOffice.org Draw (*.sxd);;");
	formats += tr("All Files (*)");
	return formats;
	*/
	return LoadSavePlugin::fileDialogLoadFilter().join(";;");
}

/*!
 \fn int FileLoader::TestFile()
 \author Franz Schmid
 \date
 \brief Tests if the file "FileName" exists and determines the type of the file.
 \param None
 \retval int -1 if the file doesn't exist or any other error has occured, 0 for the old Format, 1 for the new Format, 2 for EPS and PS files, 3 for SVG files and 4 for PDF files
 */
int FileLoader::TestFile()
{
	QFileInfo fi = QFileInfo(FileName);
	int ret = -1;
	if (!fi.exists())
		ret = -1;
	QString ext = fi.extension(true).lower();
	if ((ext.endsWith("sla.gz")) || (ext.endsWith("sla")) || (ext.endsWith("scd.gz")) || (ext.endsWith("scd")))
		ret = CheckScribus();
	else if (((ext.endsWith("ps")) || (ext.endsWith("eps"))) && (formatPS))
		ret = 2;
	else if (((ext.endsWith("svg")) || (ext.endsWith("svgz"))) && (formatSVG))
		ret = 3;
	else if ((ext.endsWith("sxd")) && (formatSXD))
		ret = 5;
	else if ((ext.endsWith("odg")) && (formatODG))
		ret = 6;
/*	if (ext == "pdf")
		ret = 4; */
	FileType = ret;
	return ret;
}

/*!
 \fn int FileLoader::CheckScribus()
 \author Franz Schmid
 \date
 \brief Checks which Scribus fileformat the File in FileName has.
 \param None
 \retval 0 for old Scribus files, 1 for Scribus files created by Scribus 1.3 and above
 */
int FileLoader::CheckScribus()
{
	int ret = -1;
	QString fText(readSLA(FileName));
	if (fText.isEmpty())
		return ret;
	if ((fText.startsWith("<SCRIBUSUTF8NEW")) && (fText.contains("<PAGE ", true) != 0))
		return 1;
	if ((fText.startsWith("<SCRIBUS")) && (fText.contains("<PAGE ", true) != 0))
		return 0;
	return ret;
}

/*!
 \fn QString FileLoader::readSLA(QString fileName)
 \author Franz Schmid
 \date
 \brief Auxiliary function loads the file "fileName" into a QString, taking care of any encoding issues
 \param QString fileName
 \retval QString Contents of the file

 Note that this method will use the system text encoding when loading .sla /
 .scd files from before the conversion to utf-8 storage. There is presently no
 override for the encoding used.
 */
QString FileLoader::readSLA(const QString & fileName)
{
	QCString docBytes("");
	if(fileName.right(2) == "gz")
	{
#ifdef HAVE_LIBZ
		static const int gzipExpansionFactor=8;
		// The file is gzip encoded and we can load gzip files.
		/**
		  * added to support gz docs
		  * 2.7.2002 C.Toepp
		  * <c.toepp@gmx.de>
		  */
		// Set up to read the gzip file
		gzFile gzDoc;
		int i;
		gzDoc = gzopen(fileName.latin1(),"rb");
		if(gzDoc == NULL)
		{
			// FIXME: Needs better error return
			return "";
		}
		// Allocate a buffer of a multiple of the compressed size of the file
		// as a starting point for loading. We'll expand this buffer by powers
		// of two if we run out of space.
		const QFileInfo fi(fileName);
		uint bufSize = fi.size()*gzipExpansionFactor;
		docBytes = QCString(bufSize);
		char* buf = docBytes.data();
		uint bytesRead = 0;
		// While there's free space, read into the buffer....
		while ((i = gzread(gzDoc,buf,bufSize-bytesRead-1)) > 0)
		{
			// Ensure the string is null-terminated and move the
			// write pointer to the current position.
			buf[i]=0;
			buf+=i;
			bytesRead += i;
			// And check that there's free space to work with, expanding the
			// buffer if there's not.
			if (bufSize - bytesRead < 4096)
			{
				bufSize *= 2;
				docBytes.resize(bufSize);
				buf = docBytes.data() + bytesRead;
			}
		}
		gzclose(gzDoc);
#else
		// The file is gzip encoded but we can't load gzip files.
		// Leave `f' empty, since we have no way to
		// report a failure condition from here.
#endif
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	QString docText("");
	if (docBytes.left(12) == "<SCRIBUSUTF8")
		docText = QString::fromUtf8(docBytes);
	else if (docBytes.left(8) == "<SCRIBUS")
		docText = QString::fromLocal8Bit(docBytes);
	if (docText.endsWith(QChar(10)) || docText.endsWith(QChar(13)))
		docText.truncate(docText.length()-1);
	return docText;
}

bool FileLoader::LoadPage(int PageToLoad, bool Mpage)
{
	bool ret = false;
	newReplacement = false;
	ReplacedFonts.clear();
	dummyFois.clear();
	dummyFois.setAutoDelete(true);
	switch (FileType)
	{
		case 0:
			{
				ScriXmlDoc ss;
				ss.ReplacedFonts.clear();
				ss.newReplacement = false;
				ss.dummyFois.clear();
				ret = ss.ReadPage(FileName, prefsManager->appPrefs.AvailFonts, ScMW->doc, ScMW->view, PageToLoad, Mpage);
				ReplacedFonts = ss.ReplacedFonts;
				newReplacement = ss.newReplacement;
				dummyFois = ss.dummyFois;
			}
			break;
		case 1:
			ret = ReadPage(FileName, prefsManager->appPrefs.AvailFonts, ScMW->doc, PageToLoad, Mpage);
			break;
		default:
			ret = false;
			break;
	}
	if (ReplacedFonts.count() != 0)
	{
		if ((prefsManager->appPrefs.askBeforeSubstituite) || (newReplacement))
		{
			qApp->setOverrideCursor(QCursor(Qt::arrowCursor), true);
			FontReplaceDialog dia(0, &ReplacedFonts);
			if (dia.exec())
			{
				QMap<QString,QString>::Iterator itfsu;
				for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
				{
					if (dia.stickyReplacements->isChecked())
						prefsManager->appPrefs.GFontSub[itfsu.key()] = itfsu.data();
				}
			}
			else
			{
				return false;
			}
		}
		for (uint d = 0; d < ScMW->doc->MasterItems.count(); ++d)
		{
			PageItem *it = ScMW->doc->MasterItems.at(d);
			if ((!ScMW->doc->UsedFonts.contains(it->font())) && (!it->font().isEmpty()))
				it->setFont(ReplacedFonts[it->font()]);
			if ((it->asTextFrame()) || (it->asPathText()))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!ScMW->doc->UsedFonts.contains(it->itemText.at(e)->cfont->scName()))
					it->itemText.at(e)->cfont = (*ScMW->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->scName()]];
				}
			}
		}
		for (uint d = 0; d < ScMW->doc->DocItems.count(); ++d)
		{
			PageItem *it = ScMW->doc->DocItems.at(d);
			if ((!ScMW->doc->UsedFonts.contains(it->font())) && (!it->font().isEmpty()))
				it->setFont(ReplacedFonts[it->font()]);
			if ((it->asTextFrame()) || (it->asPathText()))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!ScMW->doc->UsedFonts.contains(it->itemText.at(e)->cfont->scName()))
					it->itemText.at(e)->cfont = (*ScMW->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->scName()]];
				}
			}
		}
		for (uint d = 0; d < ScMW->doc->FrameItems.count(); ++d)
		{
			PageItem *it = ScMW->doc->FrameItems.at(d);
			if ((!ScMW->doc->UsedFonts.contains(it->font())) && (!it->font().isEmpty()))
				it->setFont(ReplacedFonts[it->font()]);
			if ((it->asTextFrame()) || (it->asPathText()))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!ScMW->doc->UsedFonts.contains(it->itemText.at(e)->cfont->scName()))
					it->itemText.at(e)->cfont = (*ScMW->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->scName()]];
				}
			}
		}
		for (uint a = 0; a < ScMW->doc->docParagraphStyles.count(); ++a)
		{
			if ((!ScMW->doc->UsedFonts.contains(ScMW->doc->docParagraphStyles[a].Font)) && (!ScMW->doc->docParagraphStyles[a].Font.isEmpty()))
				ScMW->doc->docParagraphStyles[a].Font = ReplacedFonts[ScMW->doc->docParagraphStyles[a].Font];
		}
		QMap<QString,QString>::Iterator itfsu;
		for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
		{
			if (!ScMW->doc->UsedFonts.contains(itfsu.data()))
			{
//				QFont fo = prefsManager->appPrefs.AvailFonts[itfsu.data()]->Font;
//				fo.setPointSize(qRound(ScMW->doc->toolSettings.defSize / 10.0));
				ScMW->doc->AddFont(itfsu.data(), qRound(ScMW->doc->toolSettings.defSize / 10.0));
			}
		}
		if (prefsManager->appPrefs.askBeforeSubstituite)
			ReplacedFonts.clear();
		dummyFois.clear();
	}
	return ret;
}

/*!
 \fn bool FileLoader::LoadFile()
 \author Franz Schmid
 \date
 \brief Loads the file "FileName" as a Scribus document
 \retval bool true when loading is succsessful, false otherwise
 */
bool FileLoader::LoadFile()
{
	bool ret = false;
	newReplacement = false;
	ScMW->doc->guidesSettings.marginsShown = prefsManager->appPrefs.guidesSettings.marginsShown;
	ScMW->doc->guidesSettings.framesShown = prefsManager->appPrefs.guidesSettings.framesShown;
	ScMW->doc->guidesSettings.gridShown = prefsManager->appPrefs.guidesSettings.gridShown;
	ScMW->doc->guidesSettings.guidesShown = prefsManager->appPrefs.guidesSettings.guidesShown;
	ScMW->doc->guidesSettings.baseShown = prefsManager->appPrefs.guidesSettings.baseShown;
	ScMW->doc->guidesSettings.linkShown = prefsManager->appPrefs.guidesSettings.linkShown;
	ScMW->doc->toolSettings.polyC = prefsManager->appPrefs.toolSettings.polyC;
	ScMW->doc->toolSettings.polyF = prefsManager->appPrefs.toolSettings.polyF;
	ScMW->doc->toolSettings.polyR = prefsManager->appPrefs.toolSettings.polyR;
	ScMW->doc->toolSettings.polyFd = prefsManager->appPrefs.toolSettings.polyFd;
	ScMW->doc->toolSettings.polyS = prefsManager->appPrefs.toolSettings.polyS;
	ScMW->doc->AutoSave = prefsManager->appPrefs.AutoSave;
	ScMW->doc->AutoSaveTime = prefsManager->appPrefs.AutoSaveTime;
	ReplacedFonts.clear();
	dummyFois.clear();
	dummyFois.setAutoDelete(true);
	switch (FileType)
	{
		case 0:
			{
				ScriXmlDoc ss;
				//CB: WTF? We use a slot to make the new pages? hello!
				//QObject::connect(ss, SIGNAL(NewPage(int)), ScMW, SLOT(slotNewPage(int)));
				ss.ReplacedFonts.clear();
				ss.newReplacement = false;
				ss.dummyFois.clear();
				ret = ss.ReadDoc(FileName, prefsManager->appPrefs.AvailFonts, ScMW->doc, ScMW->view, ScMW->mainWindowProgressBar);
				//QObject::disconnect(ss, SIGNAL(NewPage(int)), ScMW, SLOT(slotNewPage(int)));
				ReplacedFonts = ss.ReplacedFonts;
				newReplacement = ss.newReplacement;
				dummyFois = ss.dummyFois;
			}
			break;
		case 1:
			ret = ReadDoc(FileName, prefsManager->appPrefs.AvailFonts, ScMW->doc, ScMW->mainWindowProgressBar);
			break;
		case 2:
			ret = formatPS->loadFile(FileName);
			break;
		case 3:
			ret = formatSVG->loadFile(FileName);
			break;
		case 5:
			ret = formatSXD->loadFile(FileName);
			break;
		case 6:
			ret = formatODG->loadFile(FileName);
			break;
		default:
			ret = false;
			break;
	}
	return ret;
}

bool FileLoader::ReadPage(const QString & fileName, SCFonts &avail, ScribusDoc *doc, int PageToLoad, bool Mpage)
{
	struct ParagraphStyle vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	PageItem *Neu;
	Page* Apage;
	LFrames.clear();
	QString tmV, tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QFont fo;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int a, counter, baseobj;
	double xf, pageX, pageY;
	bool newVersion = false;
	bool VorLFound = false;
	QMap<int,int> layerTrans;
	int maxLayer = 0;
	int maxLevel = 0;
	layerTrans.clear();
	uint layerCount=doc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		maxLayer = QMAX(doc->Layers[la2].LNr, maxLayer);
		maxLevel = QMAX(doc->Layers[la2].Level, maxLevel);
	}
	DoVorl.clear();
	DoVorl[0] = "0";
	DoVorl[1] = "1";
	DoVorl[2] = "2";
	DoVorl[3] = "3";
	DoVorl[4] = "4";
	VorlC = 5;
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	counter = doc->Items->count();
	baseobj = counter;
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!="None")
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
				if (pg.hasAttribute("Spot"))
					lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
				else
					lf.setSpotColor(false);
				if (pg.hasAttribute("Register"))
					lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
				else
					lf.setRegistrationColor(false);
			  doc->PageColors[pg.attribute("NAME")] = lf;
			}
			if(pg.tagName()=="STYLE")
			{
				GetStyle(&pg, &vg, doc->docParagraphStyles, doc, true);
				VorLFound = true;
			}
			if(pg.tagName()=="JAVA")
				doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = pg.attribute("NUMMER").toInt();
				la.Level = pg.attribute("LEVEL").toInt();
				la.Name = pg.attribute("NAME");
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
				bool laex = false;
				uint layerCount=doc->layerCount();
				for (uint la2 = 0; la2 < layerCount; ++la2)
				{
					if (doc->Layers[la2].Name == la.Name)
					{
						laex = true;
						layerTrans.insert(la.LNr, doc->Layers[la2].LNr);
					}
				}
				if (!laex)
				{
					maxLayer++;
					maxLevel++;
					layerTrans.insert(la.LNr, maxLayer);
					la.LNr = maxLayer;
					la.Level = maxLevel;
					doc->Layers.append(la);
				}
			}
			if(pg.tagName()=="Bookmark")
			{
				bok.Title = pg.attribute("Title");
				bok.Text = pg.attribute("Text");
				bok.Aktion = pg.attribute("Aktion");
				bok.ItemNr = pg.attribute("ItemNr").toInt();
				bok.Seite = pg.attribute("Seite").toInt();
				bok.Element = pg.attribute("Element").toInt();
				bok.First = pg.attribute("First").toInt();
				bok.Last = pg.attribute("Last").toInt();
				bok.Prev = pg.attribute("Prev").toInt();
				bok.Next = pg.attribute("Next").toInt();
				bok.Parent = pg.attribute("Parent").toInt();
				doc->BookMarks.append(bok);
			}
			if(pg.tagName()=="MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = PAGE.firstChild();
				while(!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = MuL.attribute("Width").toDouble();
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				QString Nam = pg.attribute("Name");
				QString Nam2 = Nam;
				int copyC = 1;
				while (doc->MLineStyles.contains(Nam2))
				{
					Nam2 = QObject::tr("Copy #%1 of ").arg(copyC)+Nam;
					copyC++;
				}
				doc->MLineStyles.insert(Nam2, ml);
			}
			if(pg.tagName()=="Arrows")
			{
				struct ArrowDesc arrow;
				arrow.name = pg.attribute("Name");
				arrow.userArrow = true;
				double xa, ya;
				QString tmp = pg.attribute("Points");
				QTextStream fp(&tmp, IO_ReadOnly);
				for (uint cx = 0; cx < pg.attribute("NumPoints").toUInt(); ++cx)
				{
					fp >> xa;
					fp >> ya;
					arrow.points.addPoint(xa, ya);
				}
				doc->arrowStyles.append(arrow);
			}
			if (((pg.tagName()=="PAGE") || (pg.tagName()=="MASTERPAGE")) && (pg.attribute("NUM").toInt() == PageToLoad))
			{
				a = doc->currentPage->pageNr();
				Apage = doc->Pages->at(a);
				if ((pg.tagName()!="MASTERPAGE") && (Mpage))
				{
					PAGE=PAGE.nextSibling();
					continue;
				}
				if (Mpage)
				{
					Apage->LeftPg=pg.attribute("LEFT", "0").toInt();
					Apage->setPageName(pg.attribute("NAM",""));
				}
				if (pg.hasAttribute("Size"))
					Apage->PageSize = pg.attribute("Size");
				if (pg.hasAttribute("Orientation"))
					Apage->PageOri = pg.attribute("Orientation").toInt();
				if (pg.hasAttribute("PAGEWIDTH"))
					Apage->setWidth(pg.attribute("PAGEWIDTH").toDouble());
				else
					Apage->setWidth(pg.attribute("PAGEWITH").toDouble());
				Apage->setHeight(pg.attribute("PAGEHEIGHT").toDouble());
				Apage->setInitialHeight(Apage->height());
				Apage->setInitialWidth(Apage->width());
				Apage->initialMargins.Top = pg.attribute("BORDERTOP").toDouble();
				Apage->initialMargins.Bottom = pg.attribute("BORDERBOTTOM").toDouble();
				Apage->initialMargins.Left = pg.attribute("BORDERLEFT").toDouble();
				Apage->initialMargins.Right = pg.attribute("BORDERRIGHT").toDouble();
				Apage->Margins.Top = Apage->initialMargins.Top;
				Apage->Margins.Bottom = Apage->initialMargins.Bottom;
				pageX = pg.attribute("PAGEXPOS").toDouble();
				pageY = pg.attribute("PAGEYPOS").toDouble();
				if ((pg.hasAttribute("NumVGuides")) && (pg.attribute("NumVGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("VerticalGuides");
					QTextStream fgv(&tmp, IO_ReadOnly);
					Apage->YGuides.clear();
					for (int cxv = 0; cxv < pg.attribute("NumVGuides", "0").toInt(); ++cxv)
					{
						fgv >> xf;
						Apage->YGuides.append(xf);
					}
					qHeapSort(Apage->YGuides);
					tmp = "";
				}
				if ((pg.hasAttribute("NumHGuides")) && (pg.attribute("NumHGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("HorizontalGuides");
					QTextStream fgh(&tmp, IO_ReadOnly);
					Apage->XGuides.clear();
					for (int cxh = 0; cxh < pg.attribute("NumHGuides", "0").toInt(); ++cxh)
					{
						fgh >> xf;
						Apage->XGuides.append(xf);
					}
					qHeapSort(Apage->XGuides);
					tmp = "";
				}
			}
			if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="MASTEROBJECT") || (pg.tagName()=="FRAMEOBJECT"))
			{
				if ((pg.tagName()!="MASTEROBJECT") && (Mpage))
				{
					PAGE=PAGE.nextSibling();
					continue;
				}
				if (pg.attribute("OwnPage").toInt() == PageToLoad)
				{
					if (pg.attribute("NEXTITEM").toInt() != -1)
					{
						if (pg.attribute("BACKITEM").toInt() == -1)
							LFrames.append(doc->Items->count());
					}
					int docGc = doc->GroupCounter;
					doc->GroupCounter = 0;
					Neu = PasteItem(&pg, doc);
					Neu->moveBy(-pageX + Apage->xOffset(), - pageY + Apage->yOffset());
					//view->setRedrawBounding(Neu);
					Neu->setRedrawBounding();
					//Neu->OwnPage = view->OnPage(Neu);
					Neu->OwnPage = PageToLoad;
					if (pg.tagName()=="PAGEOBJECT")
						Neu->OnMasterPage = "";
					doc->GroupCounter = docGc;
					tmpf = pg.attribute("IFONT", doc->toolSettings.defFont);
					if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
					{
						if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
						{
							newReplacement = true;
							ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
						}
						else
							ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
					}
					else
					{
						if (!doc->UsedFonts.contains(tmpf))
						{
//							QFont fo = avail[tmpf]->Font;
//							fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
							doc->AddFont(tmpf, qRound(doc->toolSettings.defSize / 10.0));
						}
					}
					Neu->setFont(tmpf);
					QDomNode IT=pg.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = it.attribute("RAMP", "0.0").toDouble();
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = it.attribute("TRANS", "1").toDouble();
							Neu->fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							GetItemText(&it, doc, Neu, true, VorLFound);
						if(it.tagName()=="PageItemAttributes")
						{
							QDomNode PIA = it.firstChild();
							ObjAttrVector pageItemAttributes;
							while(!PIA.isNull())
							{
								QDomElement itemAttr = PIA.toElement();
								if(itemAttr.tagName() == "ItemAttribute")
								{
									ObjectAttribute objattr;
									objattr.name=itemAttr.attribute("Name");
									objattr.type=itemAttr.attribute("Type");
									objattr.value=itemAttr.attribute("Value");
									objattr.parameter=itemAttr.attribute("Parameter");
									objattr.relationship=itemAttr.attribute("Relationship");
									objattr.relationshipto=itemAttr.attribute("RelationshipTo");
									objattr.autoaddto=itemAttr.attribute("AutoAddTo");
									pageItemAttributes.append(objattr);
								}
								PIA = PIA.nextSibling();
							}
							Neu->setObjectAttributes(&pageItemAttributes);
						}
						IT=IT.nextSibling();
					}
					if (Neu->fill_gradient.Stops() == 0)
					{
						Neu->fill_gradient.addStop(doc->PageColors[doc->toolSettings.dBrush].getRGBColor(), 0.0, 0.5, 1.0, doc->toolSettings.dBrush, 100);
						Neu->fill_gradient.addStop(doc->PageColors[doc->toolSettings.dPen].getRGBColor(), 1.0, 0.5, 1.0, doc->toolSettings.dPen, 100);
					}
//					Neu->Language = ScMW->GetLang(pg.attribute("LANGUAGE", doc->Language));
					Neu->Language = doc->Language;
					Neu->isAutoText = static_cast<bool>(pg.attribute("AUTOTEXT").toInt());
					Neu->isEmbedded = static_cast<bool>(pg.attribute("isInline", "0").toInt());
					Neu->gXpos = pg.attribute("gXpos", "0.0").toDouble();
					Neu->gYpos = pg.attribute("gYpos", "0.0").toDouble();
					QString defaultVal;
					defaultVal.setNum(Neu->width());
					Neu->gWidth = pg.attribute("gWidth",defaultVal).toDouble();
					defaultVal.setNum(Neu->height());
					Neu->gHeight = pg.attribute("gHeight",defaultVal).toDouble();
					if (Neu->LineSpMode == 3)
					{
						doc->docParagraphStyles[0].BaseAdj = true;
						Neu->LineSp = doc->typographicSettings.valueBaseGrid-1;
					}
					if (Neu->isAutoText)
						doc->LastAuto = Neu;
					Neu->NextIt = baseobj + pg.attribute("NEXTITEM").toInt();
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
					}
					if (pg.tagName()=="FRAMEOBJECT")
					{
						doc->FrameItems.append(doc->Items->take(Neu->ItemNr));
						Neu->ItemNr = doc->FrameItems.count()-1;
					}
				}
				counter++;
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	if (TableItems.count() != 0)
	{
		for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (LFrames.count() != 0)
	{
		PageItem *Its;
		PageItem *Itn;
		PageItem *Itr;
		QValueList<int>::Iterator lc;
		for (lc = LFrames.begin(); lc != LFrames.end(); ++lc)
		{
			Its = doc->Items->at((*lc));
			Itr = Its;
			Its->BackBox = 0;
			while (Its->NextIt != -1)
			{
				if (Its->NextIt < static_cast<int>(doc->Items->count()))
				{
					Itn = doc->Items->at(Its->NextIt);
					Its->NextBox = Itn;
					Itn->BackBox = Its;
					Its = Itn;
				}
				else
					Its->NextIt = -1;
			}
			Its->NextBox = 0;
		}
	}
	return true;
}

bool FileLoader::ReadDoc(const QString & fileName, SCFonts &avail, ScribusDoc *doc, QProgressBar *dia2)
{
	struct ParagraphStyle vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	int counter;//, Pgc;
	//bool AtFl;
	bool newVersion = false;
	QString tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QFont fo;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int a;
	double xf, xf2;
	PageItem *Neu;
	Page* Apage;
	LFrames.clear();
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	/* 2004/10/02 - petr vanek - bug #1092 - missing <PAGE> crash Scribus. The check constraint moved into IsScribus()
	FIXME: I've add test on containig tag PAGE but returning false freezes S. in scribus.cpp need some hack too...  */
	if (!docu.setContent(f))
		return false;
	doc->PageColors.clear();
	doc->Layers.clear();
	int layerToSetActive=0;
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	dia2->setTotalSteps(DOC.childNodes().count());
	dia2->setProgress(0);
	int ObCount = 0;
	TableItems.clear();
	TableID.clear();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
	/*
	* Attribute von DOCUMENT auslesen
	*/
		//CB Add this in to set this in the file in memory. Its saved, why not load it. 
		//Will of course be replaced by per page settings although we still probably need a document default
		doc->PageSize = dc.attribute("PAGESIZE");
		doc->PageOri = dc.attribute("ORIENTATION", "0").toInt();
		doc->FirstPnum = dc.attribute("FIRSTNUM", "1").toInt();
		doc->currentPageLayout=dc.attribute("BOOK", "0").toInt();
		int fp;
		if (doc->currentPageLayout == 0)
			fp = 0;
		else
		{
			if (dc.attribute("FIRSTLEFT", "0").toInt() == 1)
				fp = 0;
			else
				fp = 1;
		}
		if (DOC.namedItem("PageSets").isNull())
		{
			doc->pageSets[doc->currentPageLayout].FirstPage = fp;
			doc->pageSets[doc->currentPageLayout].GapHorizontal = dc.attribute("GapHorizontal", "0").toDouble();
			doc->pageSets[doc->currentPageLayout].GapVertical = 0.0;
			doc->pageSets[doc->currentPageLayout].GapBelow = dc.attribute("GapVertical", "40").toDouble();
		}
		doc->setUsesAutomaticTextFrames(dc.attribute("AUTOTEXT").toInt());
		doc->PageSp=dc.attribute("AUTOSPALTEN").toInt();
		doc->PageSpa=dc.attribute("ABSTSPALTEN").toDouble();
		doc->setUnitIndex(dc.attribute("UNITS", "0").toInt());
		doc->toolSettings.defSize=qRound(dc.attribute("DSIZE").toDouble() * 10);
		Defont=dc.attribute("DFONT");
		if ((!avail.find(Defont)) || (!avail[Defont]->UseFont))
		{
			ReplacedFonts.insert(Defont, prefsManager->appPrefs.toolSettings.defFont);
			Defont = prefsManager->appPrefs.toolSettings.defFont;
		}
		else
		{
//			QFont fo = avail[Defont]->Font;
//			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			doc->AddFont(Defont, qRound(doc->toolSettings.defSize / 10.0));
		}
		doc->toolSettings.defFont = Defont;
		doc->toolSettings.dCols=dc.attribute("DCOL", "1").toInt();
		doc->toolSettings.dGap=dc.attribute("DGAP", "0.0").toDouble();
		doc->documentInfo.setAuthor(dc.attribute("AUTHOR"));
		doc->documentInfo.setComments(dc.attribute("COMMENTS"));
		doc->documentInfo.setKeywords(dc.attribute("KEYWORDS",""));
		doc->documentInfo.setTitle(dc.attribute("TITLE"));
		doc->documentInfo.setPublisher(dc.attribute("PUBLISHER", ""));
		doc->documentInfo.setDate(dc.attribute("DOCDATE", ""));
		doc->documentInfo.setType(dc.attribute("DOCTYPE", ""));
		doc->documentInfo.setFormat(dc.attribute("DOCFORMAT", ""));
		doc->documentInfo.setIdent(dc.attribute("DOCIDENT", ""));
		doc->documentInfo.setSource(dc.attribute("DOCSOURCE", ""));
		doc->documentInfo.setLangInfo(dc.attribute("DOCLANGINFO", ""));
		doc->documentInfo.setRelation(dc.attribute("DOCRELATION", ""));
		doc->documentInfo.setCover(dc.attribute("DOCCOVER", ""));
		doc->documentInfo.setRights(dc.attribute("DOCRIGHTS", ""));
		doc->documentInfo.setContrib(dc.attribute("DOCCONTRIB", ""));
		doc->typographicSettings.valueSuperScript = dc.attribute("VHOCH").toInt();
		doc->typographicSettings.scalingSuperScript = dc.attribute("VHOCHSC").toInt();
		doc->typographicSettings.valueSubScript = dc.attribute("VTIEF").toInt();
		doc->typographicSettings.scalingSubScript = dc.attribute("VTIEFSC").toInt();
		doc->typographicSettings.valueSmallCaps = dc.attribute("VKAPIT").toInt();
		doc->typographicSettings.valueBaseGrid = dc.attribute("BASEGRID", "12").toDouble();
		doc->typographicSettings.offsetBaseGrid = dc.attribute("BASEO", "0").toDouble();
		doc->typographicSettings.autoLineSpacing = dc.attribute("AUTOL", "20").toInt();
		doc->typographicSettings.valueUnderlinePos = dc.attribute("UnderlinePos", "-1").toInt();
		doc->typographicSettings.valueUnderlineWidth = dc.attribute("UnderlineWidth", "-1").toInt();
		doc->typographicSettings.valueStrikeThruPos = dc.attribute("StrikeThruPos", "-1").toInt();
		doc->typographicSettings.valueStrikeThruWidth = dc.attribute("StrikeThruWidth", "-1").toInt();
		doc->GroupCounter=dc.attribute("GROUPC", "1").toInt();
		doc->HasCMS = static_cast<bool>(dc.attribute("HCMS", "0").toInt());
		doc->CMSSettings.SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
		doc->CMSSettings.CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
		doc->CMSSettings.GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
		doc->CMSSettings.BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
		doc->CMSSettings.DefaultMonitorProfile = dc.attribute("DPMo","");
		doc->CMSSettings.DefaultPrinterProfile = dc.attribute("DPPr","");
		doc->CMSSettings.DefaultImageRGBProfile = dc.attribute("DPIn","");
		doc->CMSSettings.DefaultImageCMYKProfile = dc.attribute("DPInCMYK","");
		doc->CMSSettings.DefaultSolidColorProfile = dc.attribute("DPIn2","");
		doc->CMSSettings.DefaultIntentPrinter = dc.attribute("DIPr", "0").toInt();
		doc->CMSSettings.DefaultIntentMonitor = dc.attribute("DIMo", "1").toInt();
		doc->CMSSettings.DefaultIntentImages = dc.attribute("DIMo2", "1").toInt();
		layerToSetActive=dc.attribute("ALAYER", "0").toInt();
		doc->Language = dc.attribute("LANGUAGE", "");
		doc->MinWordLen = dc.attribute("MINWORDLEN", "3").toInt();
		doc->HyCount = dc.attribute("HYCOUNT", "2").toInt();
		if (dc.hasAttribute("PAGEWIDTH"))
			doc->pageWidth=dc.attribute("PAGEWIDTH").toDouble();
		else
			doc->pageWidth=dc.attribute("PAGEWITH").toDouble();
		doc->pageHeight=dc.attribute("PAGEHEIGHT").toDouble();
		doc->pageMargins.Left=dc.attribute("BORDERLEFT").toDouble();
		doc->pageMargins.Right=dc.attribute("BORDERRIGHT").toDouble();
		doc->pageMargins.Top=dc.attribute("BORDERTOP").toDouble();
		doc->pageMargins.Bottom=dc.attribute("BORDERBOTTOM").toDouble();
		doc->Automatic = static_cast<bool>(dc.attribute("AUTOMATIC", "1").toInt());
		doc->AutoCheck = static_cast<bool>(dc.attribute("AUTOCHECK", "0").toInt());
		doc->GuideLock = static_cast<bool>(dc.attribute("GUIDELOCK", "0").toInt());
		doc->guidesSettings.minorGrid = dc.attribute("MINGRID", tmp.setNum(prefsManager->appPrefs.guidesSettings.minorGrid)).toDouble();
		doc->guidesSettings.majorGrid = dc.attribute("MAJGRID", tmp.setNum(prefsManager->appPrefs.guidesSettings.majorGrid)).toDouble();
		doc->guidesSettings.gridShown = static_cast<bool>(dc.attribute("SHOWGRID", "0").toInt());
		doc->guidesSettings.guidesShown = static_cast<bool>(dc.attribute("SHOWGUIDES", "1").toInt());
		doc->guidesSettings.framesShown = static_cast<bool>(dc.attribute("SHOWFRAME", "1").toInt());
		doc->guidesSettings.marginsShown = static_cast<bool>(dc.attribute("SHOWMARGIN", "1").toInt());
		doc->guidesSettings.baseShown = static_cast<bool>(dc.attribute("SHOWBASE", "0").toInt());
		doc->guidesSettings.showPic = static_cast<bool>(dc.attribute("SHOWPICT", "1").toInt());
		doc->guidesSettings.linkShown = static_cast<bool>(dc.attribute("SHOWLINK", "0").toInt());
		doc->guidesSettings.showControls = static_cast<bool>(dc.attribute("SHOWControl", "0").toInt());
		doc->guidesSettings.rulerMode = static_cast<bool>(dc.attribute("rulerMode", "1").toInt());
		doc->rulerXoffset = dc.attribute("rulerXoffset", "0").toDouble();
		doc->rulerYoffset =dc.attribute("rulerYoffset", "0").toDouble();
		doc->SnapGuides = static_cast<bool>(dc.attribute("SnapToGuides", "0").toInt());
		doc->useRaster = static_cast<bool>(dc.attribute("SnapToGrid", "0").toInt());
		doc->toolSettings.polyC = dc.attribute("POLYC", "4").toInt();
		doc->toolSettings.polyF = dc.attribute("POLYF", "0.5").toDouble();
		doc->toolSettings.polyR = dc.attribute("POLYR", "0").toDouble();
		doc->toolSettings.polyFd = dc.attribute("POLYFD", "0").toInt();
		doc->toolSettings.polyS = static_cast<bool>(dc.attribute("POLYS", "0").toInt());
		doc->AutoSave = static_cast<bool>(dc.attribute("AutoSave", "0").toInt());
		doc->AutoSaveTime = dc.attribute("AutoSaveTime", "600000").toInt();
		doc->ScratchBottom = dc.attribute("ScratchBottom", "20").toDouble();
		// FIXME A typo in early 1.3cvs (MAR 05) means we must support loading of
		// FIXME 'ScatchLeft' for a while too. This can be removed in a few months.
		if (dc.hasAttribute("ScatchLeft"))
			doc->ScratchLeft = dc.attribute("ScatchLeft", "100").toDouble();
		else
			doc->ScratchLeft = dc.attribute("ScratchLeft", "100").toDouble();
		doc->ScratchRight = dc.attribute("ScratchRight", "100").toDouble();
		doc->ScratchTop = dc.attribute("ScratchTop", "20").toDouble();
		doc->toolSettings.dStartArrow = dc.attribute("StartArrow", "0").toInt();
		doc->toolSettings.dEndArrow = dc.attribute("EndArrow", "0").toInt();
		doc->toolSettings.scaleX = dc.attribute("PICTSCX", "1").toDouble();
		doc->toolSettings.scaleY = dc.attribute("PICTSCY", "1").toDouble();
		doc->toolSettings.scaleType = static_cast<bool>(dc.attribute("PSCALE", "1").toInt());
		doc->toolSettings.aspectRatio = static_cast<bool>(dc.attribute("PASPECT", "0").toInt());
		doc->toolSettings.lowResType = dc.attribute("HalfRes", "1").toInt();
		doc->toolSettings.useEmbeddedPath = static_cast<bool>(dc.attribute("EmbeddedPath", "0").toInt());
		if (dc.hasAttribute("PEN"))
			doc->toolSettings.dPen = dc.attribute("PEN");
		if (dc.hasAttribute("BRUSH"))
			doc->toolSettings.dBrush = dc.attribute("BRUSH");
		if (dc.hasAttribute("PENLINE"))
			doc->toolSettings.dPenLine = dc.attribute("PENLINE");
		if (dc.hasAttribute("PENTEXT"))
			doc->toolSettings.dPenText = dc.attribute("PENTEXT");
		if (dc.hasAttribute("StrokeText"))
			doc->toolSettings.dStrokeText = dc.attribute("StrokeText");
		doc->toolSettings.dTextBackGround = dc.attribute("TextBackGround", "None");
		doc->toolSettings.dTextLineColor = dc.attribute("TextLineColor", "None");
		doc->toolSettings.dTextBackGroundShade = dc.attribute("TextBackGroundShade", "100").toInt();
		doc->toolSettings.dTextLineShade = dc.attribute("TextLineShade", "100").toInt();
		doc->toolSettings.dTextPenShade = dc.attribute("TextPenShade", "100").toInt();
		doc->toolSettings.dTextStrokeShade = dc.attribute("TextStrokeShade", "100").toInt();
		doc->toolSettings.dLineArt = static_cast<Qt::PenStyle>(dc.attribute("STIL").toInt());
		doc->toolSettings.dLstyleLine = static_cast<Qt::PenStyle>(dc.attribute("STILLINE").toInt());
		doc->toolSettings.dWidth = dc.attribute("WIDTH", "1").toDouble();
		doc->toolSettings.dWidthLine = dc.attribute("WIDTHLINE", "1").toDouble();
		doc->toolSettings.dShade2 = dc.attribute("PENSHADE", "100").toInt();
		doc->toolSettings.dShadeLine = dc.attribute("LINESHADE", "100").toInt();
		doc->toolSettings.dShade = dc.attribute("BRUSHSHADE", "100").toInt();
		doc->toolSettings.magMin = dc.attribute("MAGMIN", "10").toInt();
		doc->toolSettings.magMax = dc.attribute("MAGMAX", "3200").toInt();
		doc->toolSettings.magStep = dc.attribute("MAGSTEP", "200").toInt();
		//CB Reset doc zoom step value to 200% instead of old values.
		if (doc->toolSettings.magStep<100)
			doc->toolSettings.magStep=200;
		doc->toolSettings.tabFillChar = dc.attribute("TabFill","");
		doc->toolSettings.dTabWidth=dc.attribute("TabWidth", "36.0").toDouble();
		if (dc.hasAttribute("CPICT"))
			doc->toolSettings.dBrushPict = dc.attribute("CPICT");
		doc->toolSettings.shadePict = dc.attribute("PICTSHADE", "100").toInt();
		if (dc.hasAttribute("PAGEC"))
			doc->papColor = QColor(dc.attribute("PAGEC"));
		if (dc.hasAttribute("MARGC"))
			doc->guidesSettings.margColor = QColor(dc.attribute("MARGC"));
		if (dc.hasAttribute("MINORC"))
			doc->guidesSettings.minorColor = QColor(dc.attribute("MINORC"));
		if (dc.hasAttribute("MAJORC"))
			doc->guidesSettings.majorColor = QColor(dc.attribute("MAJORC"));
		if (dc.hasAttribute("GuideC"))
			doc->guidesSettings.guideColor = QColor(dc.attribute("GuideC"));
		if (dc.hasAttribute("BaseC"))
			doc->guidesSettings.baseColor = QColor(dc.attribute("BaseC"));
		doc->marginColored = static_cast<bool>(dc.attribute("RANDF", "0").toInt());
		doc->guidesSettings.before = static_cast<bool>(dc.attribute("BACKG", "1").toInt());
		doc->guidesSettings.guideRad = dc.attribute("GuideRad", "10").toDouble();
		doc->guidesSettings.grabRad = dc.attribute("GRAB", "4").toInt();
		if (dc.hasAttribute("currentProfile"))
		{
			doc->checkerProfiles.clear();
			doc->curCheckProfile = dc.attribute("currentProfile");
		}
		doc->LastAuto = 0;
		QDomNode PAGE=DOC.firstChild();
		counter = 0;
		while(!PAGE.isNull())
		{
			ObCount++;
			dia2->setProgress(ObCount);
			QDomElement pg=PAGE.toElement();
			if (pg.tagName()=="PageSets")
			{
				QDomNode PGS = PAGE.firstChild();
				doc->pageSets.clear();
				while(!PGS.isNull())
				{
					QDomElement PgsAttr = PGS.toElement();
					if(PgsAttr.tagName() == "Set")
					{
						struct PageSet pageS;
						pageS.Name = PgsAttr.attribute("Name");
						pageS.FirstPage = PgsAttr.attribute("FirstPage", "0").toInt();
						pageS.Rows = PgsAttr.attribute("Rows", "1").toInt();
						pageS.Columns = PgsAttr.attribute("Columns", "1").toInt();
						pageS.GapHorizontal = PgsAttr.attribute("GapHorizontal", "0").toDouble();
						pageS.GapVertical = PgsAttr.attribute("GapVertical", "0").toDouble();
						pageS.GapBelow = PgsAttr.attribute("GapBelow", "0").toDouble();
						pageS.pageNames.clear();
						QDomNode PGSN = PGS.firstChild();
						while(!PGSN.isNull())
						{
							QDomElement PgsAttrN = PGSN.toElement();
							if(PgsAttrN.tagName() == "PageNames")
								pageS.pageNames.append(PgsAttrN.attribute("Name"));
							PGSN = PGSN.nextSibling();
						}
						doc->pageSets.append(pageS);
					}
					PGS = PGS.nextSibling();
				}
			}
			if (pg.tagName()=="CheckProfile")
			{
				struct checkerPrefs checkerSettings;
				checkerSettings.ignoreErrors = static_cast<bool>(pg.attribute("ignoreErrors", "0").toInt());
				checkerSettings.autoCheck = static_cast<bool>(pg.attribute("autoCheck", "1").toInt());
				checkerSettings.checkGlyphs = static_cast<bool>(pg.attribute("checkGlyphs", "1").toInt());
				checkerSettings.checkOrphans = static_cast<bool>(pg.attribute("checkOrphans", "1").toInt());
				checkerSettings.checkOverflow = static_cast<bool>(pg.attribute("checkOverflow", "1").toInt());
				checkerSettings.checkPictures = static_cast<bool>(pg.attribute("checkPictures", "1").toInt());
				checkerSettings.checkResolution = static_cast<bool>(pg.attribute("checkResolution", "1").toInt());
				checkerSettings.checkTransparency = static_cast<bool>(pg.attribute("checkTransparency", "1").toInt());
				checkerSettings.minResolution = pg.attribute("minResolution", "72").toDouble();
				checkerSettings.checkAnnotations = static_cast<bool>(pg.attribute("checkAnnotations", "0").toInt());
				checkerSettings.checkRasterPDF = static_cast<bool>(pg.attribute("checkRasterPDF", "1").toInt());
				doc->checkerProfiles[pg.attribute("Name")] = checkerSettings;
			}
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!="None")
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
				if (pg.hasAttribute("Spot"))
					lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
				else
					lf.setSpotColor(false);
				if (pg.hasAttribute("Register"))
					lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
				else
					lf.setRegistrationColor(false);
			  doc->PageColors[pg.attribute("NAME")] = lf;
			}
			if(pg.tagName()=="STYLE")
			{
				vg.Vname = pg.attribute("NAME");
				vg.LineSpaMode = pg.attribute("LINESPMode", "0").toInt();
				vg.LineSpa = pg.attribute("LINESP").toDouble();
				vg.Indent = pg.attribute("INDENT", "0").toDouble();
				vg.First = pg.attribute("FIRST", "0").toDouble();
				vg.textAlignment = pg.attribute("ALIGN").toInt();
				vg.gapBefore = pg.attribute("VOR", "0").toDouble();
				vg.gapAfter = pg.attribute("NACH", "0").toDouble();
				tmpf = pg.attribute("FONT", doc->toolSettings.defFont);
				if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
				{
					if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
					{
						newReplacement = true;
						ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
					}
					else
						ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
				}
				else
				{
					if (!doc->UsedFonts.contains(tmpf))
					{
//						QFont fo = avail[tmpf]->Font;
//						fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
						doc->AddFont(tmpf, qRound(doc->toolSettings.defSize / 10.0));
					}
				}
				vg.Font = tmpf;
				vg.FontSize = qRound(pg.attribute("FONTSIZE", "12").toDouble() * 10.0);
				vg.Drop = static_cast<bool>(pg.attribute("DROP", "0").toInt());
				vg.DropLin = pg.attribute("DROPLIN", "2").toInt();
				vg.DropDist = pg.attribute("DROPDIST", "0").toDouble();
				vg.FontEffect = pg.attribute("EFFECT", "0").toInt();
				vg.FColor = pg.attribute("FCOLOR", doc->toolSettings.dBrush);
				vg.FShade = pg.attribute("FSHADE", "100").toInt();
				vg.SColor = pg.attribute("SCOLOR", doc->toolSettings.dPen);
				vg.SShade = pg.attribute("SSHADE", "100").toInt();
				vg.BaseAdj = static_cast<bool>(pg.attribute("BASE", "0").toInt());
				vg.txtShadowX=qRound(pg.attribute("TXTSHX", "5").toDouble() * 10);
				vg.txtShadowY=qRound(pg.attribute("TXTSHY", "-5").toDouble() * 10);
				vg.txtOutline=qRound(pg.attribute("TXTOUT", "1").toDouble() * 10);
				vg.txtUnderPos=qRound(pg.attribute("TXTULP", "-0.1").toDouble() * 10);
				vg.txtUnderWidth=qRound(pg.attribute("TXTULW", "-0.1").toDouble() * 10);
				vg.txtStrikePos=qRound(pg.attribute("TXTSTP", "-0.1").toDouble() * 10);
				vg.txtStrikeWidth=qRound(pg.attribute("TXTSTW", "-0.1").toDouble() * 10);
				vg.scaleH = qRound(pg.attribute("SCALEH", "100").toDouble() * 10);
				vg.scaleV = qRound(pg.attribute("SCALEV", "100").toDouble() * 10);
				vg.baseOff = qRound(pg.attribute("BASEO", "0").toDouble() * 10);
				vg.kernVal = qRound(pg.attribute("KERN", "0").toDouble() * 10);
				vg.TabValues.clear();
				if ((pg.hasAttribute("NUMTAB")) && (pg.attribute("NUMTAB", "0").toInt() != 0))
				{
					struct PageItem::TabRecord tb;
					tmp = pg.attribute("TABS");
					QTextStream tgv(&tmp, IO_ReadOnly);
					for (int cxv = 0; cxv < pg.attribute("NUMTAB", "0").toInt(); cxv += 2)
					{
						tgv >> xf;
						tgv >> xf2;
						tb.tabPosition = xf2;
						tb.tabType = static_cast<int>(xf);
						tb.tabFillChar =  QChar();
						vg.TabValues.append(tb);
					}
					tmp = "";
				}
				else
				{
					QDomNode IT = pg.firstChild();
					while(!IT.isNull())
					{
						QDomElement it = IT.toElement();
						if (it.tagName()=="Tabs")
						{
							struct PageItem::TabRecord tb;
							tb.tabPosition = it.attribute("Pos").toDouble();
							tb.tabType = it.attribute("Type").toInt();
							QString tbCh = "";
							tbCh = it.attribute("Fill","");
							if (tbCh.isEmpty())
								tb.tabFillChar = QChar();
							else
								tb.tabFillChar = tbCh[0];
							vg.TabValues.append(tb);
						}
						IT=IT.nextSibling();
					}
				}
				doc->docParagraphStyles.append(vg);
			}
			if(pg.tagName()=="JAVA")
				doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = pg.attribute("NUMMER").toInt();
				la.Level = pg.attribute("LEVEL").toInt();
				la.Name = pg.attribute("NAME");
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
				doc->Layers.append(la);
			}
			if(pg.tagName()=="Bookmark")
			{
				bok.Title = pg.attribute("Title");
				bok.Text = pg.attribute("Text");
				bok.Aktion = pg.attribute("Aktion");
				bok.ItemNr = pg.attribute("ItemNr").toInt();
				bok.Seite = pg.attribute("Seite").toInt();
				bok.Element = pg.attribute("Element").toInt();
				bok.First = pg.attribute("First").toInt();
				bok.Last = pg.attribute("Last").toInt();
				bok.Prev = pg.attribute("Prev").toInt();
				bok.Next = pg.attribute("Next").toInt();
				bok.Parent = pg.attribute("Parent").toInt();
				doc->BookMarks.append(bok);
			}
			if(pg.tagName()=="MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = PAGE.firstChild();
				while(!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = MuL.attribute("Width").toDouble();
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				doc->MLineStyles.insert(pg.attribute("Name"), ml);
			}
			if(pg.tagName()=="Arrows")
			{
				struct ArrowDesc arrow;
				arrow.name = pg.attribute("Name");
				arrow.userArrow = true;
				double xa, ya;
				QString tmp = pg.attribute("Points");
				QTextStream fp(&tmp, IO_ReadOnly);
				for (uint cx = 0; cx < pg.attribute("NumPoints").toUInt(); ++cx)
				{
					fp >> xa;
					fp >> ya;
					arrow.points.addPoint(xa, ya);
				}
				doc->arrowStyles.append(arrow);
			}
			if(pg.tagName()=="PDF")
			{
				doc->PDF_Options.Articles = static_cast<bool>(pg.attribute("Articles").toInt());
				doc->PDF_Options.Thumbnails = static_cast<bool>(pg.attribute("Thumbnails").toInt());
				doc->PDF_Options.Compress = static_cast<bool>(pg.attribute("Compress").toInt());
				doc->PDF_Options.CompressMethod = pg.attribute("CMethod", "0").toInt();
				doc->PDF_Options.Quality = pg.attribute("Quality", "0").toInt();
				doc->PDF_Options.RecalcPic = static_cast<bool>(pg.attribute("RecalcPic").toInt());
				doc->PDF_Options.Bookmarks = static_cast<bool>(pg.attribute("Bookmarks").toInt());
				if (pg.hasAttribute("MirrorH"))
					doc->PDF_Options.MirrorH = static_cast<bool>(pg.attribute("MirrorH").toInt());
				else
					doc->PDF_Options.MirrorH = false;
				if (pg.hasAttribute("MirrorV"))
					doc->PDF_Options.MirrorV = static_cast<bool>(pg.attribute("MirrorV").toInt());
				else
					doc->PDF_Options.MirrorV = false;
				if (pg.hasAttribute("RotateDeg"))
					doc->PDF_Options.RotateDeg = pg.attribute("RotateDeg", "0").toInt();
				else
					doc->PDF_Options.RotateDeg = 0;
				doc->PDF_Options.PresentMode = static_cast<bool>(pg.attribute("PresentMode").toInt());
				doc->PDF_Options.PicRes = pg.attribute("PicRes").toInt();
				// Fixme: check input pdf version
				doc->PDF_Options.Version = (PDFOptions::PDFVersion)pg.attribute("Version").toInt();
				doc->PDF_Options.Resolution = pg.attribute("Resolution").toInt();
				doc->PDF_Options.Binding = pg.attribute("Binding").toInt();
				doc->PDF_Options.Datei = "";
				doc->PDF_Options.isGrayscale = static_cast<bool>(pg.attribute("Grayscale", "0").toInt());
				doc->PDF_Options.UseRGB = static_cast<bool>(pg.attribute("RGBMode", "0").toInt());
				doc->PDF_Options.UseProfiles = static_cast<bool>(pg.attribute("UseProfiles", "0").toInt());
				doc->PDF_Options.UseProfiles2 = static_cast<bool>(pg.attribute("UseProfiles2", "0").toInt());
				doc->PDF_Options.Intent = pg.attribute("Intent", "1").toInt();
				doc->PDF_Options.Intent2 = pg.attribute("Intent2", "1").toInt();
				doc->PDF_Options.SolidProf = pg.attribute("SolidP", "");
				doc->PDF_Options.ImageProf = pg.attribute("ImageP", "");
				doc->PDF_Options.PrintProf = pg.attribute("PrintP", "");
				doc->PDF_Options.Info = pg.attribute("InfoString", "");
				doc->PDF_Options.BleedTop = pg.attribute("BTop", "0").toDouble();
				doc->PDF_Options.BleedLeft = pg.attribute("BLeft", "0").toDouble();
				doc->PDF_Options.BleedRight = pg.attribute("BRight", "0").toDouble();
				doc->PDF_Options.BleedBottom = pg.attribute("BBottom", "0").toDouble();
				doc->PDF_Options.EmbeddedI = static_cast<bool>(pg.attribute("ImagePr", "0").toInt());
				doc->PDF_Options.PassOwner = pg.attribute("PassOwner", "");
				doc->PDF_Options.PassUser = pg.attribute("PassUser", "");
				doc->PDF_Options.Permissions = pg.attribute("Permissions", "-4").toInt();
				doc->PDF_Options.Encrypt = static_cast<bool>(pg.attribute("Encrypt", "0").toInt());
				doc->PDF_Options.useLayers = static_cast<bool>(pg.attribute("UseLayers", "0").toInt());
				doc->PDF_Options.UseLPI = static_cast<bool>(pg.attribute("UseLpi", "0").toInt());
				doc->PDF_Options.UseSpotColors = static_cast<bool>(pg.attribute("UseSpotColors", "1").toInt());
				doc->PDF_Options.doMultiFile = static_cast<bool>(pg.attribute("doMultiFile", "0").toInt());
				doc->PDF_Options.displayBookmarks = static_cast<bool>(pg.attribute("displayBookmarks", "0").toInt());
				doc->PDF_Options.displayFullscreen = static_cast<bool>(pg.attribute("displayFullscreen", "0").toInt());
				doc->PDF_Options.displayLayers = static_cast<bool>(pg.attribute("displayLayers", "0").toInt());
				doc->PDF_Options.displayThumbs = static_cast<bool>(pg.attribute("displayThumbs", "0").toInt());
				doc->PDF_Options.hideMenuBar = static_cast<bool>(pg.attribute("hideMenuBar", "0").toInt());
				doc->PDF_Options.hideToolBar = static_cast<bool>(pg.attribute("hideToolBar", "0").toInt());
				doc->PDF_Options.fitWindow = static_cast<bool>(pg.attribute("fitWindow", "0").toInt());
				doc->PDF_Options.PageLayout = pg.attribute("PageLayout", "0").toInt();
				doc->PDF_Options.openAction = pg.attribute("openAction", "");
				QDomNode PFO = PAGE.firstChild();
				while(!PFO.isNull())
				{
					QDomElement pdfF = PFO.toElement();
					if(pdfF.tagName() == "LPI")
					{
						struct LPIData lpo;
						lpo.Angle = pdfF.attribute("Angle").toInt();
						lpo.Frequency = pdfF.attribute("Frequency").toInt();
						lpo.SpotFunc = pdfF.attribute("SpotFunction").toInt();
						doc->PDF_Options.LPISettings[pdfF.attribute("Color")] = lpo;
					}
					if(pdfF.tagName() == "Fonts")
					{
						if (!doc->PDF_Options.EmbedList.contains(pdfF.attribute("Name")))
							doc->PDF_Options.EmbedList.append(pdfF.attribute("Name"));
					}
					if(pdfF.tagName() == "Subset")
					{
						if (!doc->PDF_Options.SubsetList.contains(pdfF.attribute("Name")))
							doc->PDF_Options.SubsetList.append(pdfF.attribute("Name"));
					}
					if(pdfF.tagName() == "Effekte")
					{
						struct PDFPresentationData ef;
						ef.pageEffectDuration = pdfF.attribute("pageEffectDuration").toInt();
						ef.pageViewDuration = pdfF.attribute("pageViewDuration").toInt();
						ef.effectType = pdfF.attribute("effectType").toInt();
						ef.Dm = pdfF.attribute("Dm").toInt();
						ef.M = pdfF.attribute("M").toInt();
						ef.Di = pdfF.attribute("Di").toInt();
						doc->PDF_Options.PresentVals.append(ef);
					}
					PFO = PFO.nextSibling();
				}
			}
			if(pg.tagName()=="DocItemAttributes")
			{
				QDomNode DIA = PAGE.firstChild();
				doc->docItemAttributes.clear();
				while(!DIA.isNull())
				{
					QDomElement itemAttr = DIA.toElement();
					if(itemAttr.tagName() == "ItemAttribute")
					{
						ObjectAttribute objattr;
						objattr.name=itemAttr.attribute("Name");
						objattr.type=itemAttr.attribute("Type");
						objattr.value=itemAttr.attribute("Value");
						objattr.parameter=itemAttr.attribute("Parameter");
						objattr.relationship=itemAttr.attribute("Relationship");
						objattr.relationshipto=itemAttr.attribute("RelationshipTo");
						objattr.autoaddto=itemAttr.attribute("AutoAddTo");
						doc->docItemAttributes.append(objattr);
					}
					DIA = DIA.nextSibling();
				}
			}
			if(pg.tagName()=="TablesOfContents")
			{
				QDomNode TOC = PAGE.firstChild();
				doc->docToCSetups.clear();
				while(!TOC.isNull())
				{
					QDomElement tocElem = TOC.toElement();
					if(tocElem.tagName() == "TableOfContents")
					{
						ToCSetup tocsetup;
						tocsetup.name=tocElem.attribute("Name");
						tocsetup.itemAttrName=tocElem.attribute("ItemAttributeName");
						tocsetup.frameName=tocElem.attribute("FrameName");
						tocsetup.listNonPrintingFrames=tocElem.attribute("ListNonPrinting");
						tocsetup.textStyle=tocElem.attribute("Style");
						QString numberPlacement=tocElem.attribute("NumberPlacement");
						if (numberPlacement=="Beginning")
							tocsetup.pageLocation=Beginning;
						if (numberPlacement=="End")
							tocsetup.pageLocation=End;
						if (numberPlacement=="NotShown")
							tocsetup.pageLocation=NotShown;
						doc->docToCSetups.append(tocsetup);
					}
					TOC = TOC.nextSibling();
				}
			}
			if(pg.tagName()=="Sections")
			{
				QDomNode Section = PAGE.firstChild();
				while(!Section.isNull())
				{
					QDomElement sectionElem = Section.toElement();
					if(sectionElem.tagName() == "Section")
					{
						struct DocumentSection newSection;
						newSection.number=sectionElem.attribute("Number").toInt();
						newSection.name=sectionElem.attribute("Name");
						newSection.fromindex=sectionElem.attribute("From").toInt();
						newSection.toindex=sectionElem.attribute("To").toInt();
						if (sectionElem.attribute("Type")=="Type_1_2_3")
							newSection.type=Type_1_2_3;
						if (sectionElem.attribute("Type")=="Type_i_ii_iii")
							newSection.type=Type_i_ii_iii;
						if (sectionElem.attribute("Type")=="Type_I_II_III")
							newSection.type=Type_I_II_III;
						if (sectionElem.attribute("Type")=="Type_a_b_c")
							newSection.type=Type_a_b_c;
						if (sectionElem.attribute("Type")=="Type_A_B_C")
							newSection.type=Type_A_B_C;
						newSection.sectionstartindex=sectionElem.attribute("Start").toInt();
						newSection.reversed=static_cast<bool>(sectionElem.attribute("Reversed").toInt());
						newSection.active=static_cast<bool>(sectionElem.attribute("Active").toInt());
						doc->sections.insert(newSection.number, newSection);
					}
					Section = Section.nextSibling();
				}
			}
			if ((pg.tagName()=="PAGE") || (pg.tagName()=="MASTERPAGE"))
			{
				a = pg.attribute("NUM").toInt();
				PgNam = "";
				PgNam = pg.attribute("NAM", "");
				//Pgc = doc->pageCount;
				//AtFl = doc->usesAutomaticTextFrames();
				if (PgNam.isEmpty())
				{
					//doc->pageCount = Pgc;
					//doc->Pages = &doc->DocPages;
					//doc->setUsesAutomaticTextFrames(AtFl);
					doc->setMasterPageMode(false);
				}
				else
				{
					//doc->pageCount = 0;
					//doc->setUsesAutomaticTextFrames(false);
					//doc->Pages = &doc->MasterPages;
					doc->setMasterPageMode(true);
				}
				//CB: Stop calling damn GUI code in loading docs! IT doesnt *look* like 
				//this makes a difference apart from being faster, of course.
				//ScMW->slotNewPage(a);
				//Apage = doc->Pages.at(a);
				if (PgNam.isEmpty())
				{
					Apage = doc->addPage(a);
					//doc->DocPages = doc->Pages;
					//++doc->pageCount;
				}
				else
				{
					Apage = doc->addMasterPage(a, PgNam);
					//Apage->setPageName(PgNam);
					//doc->MasterNames[PgNam] = a;
					//doc->MasterPages = doc->Pages;
					//doc->pageCount = Pgc;
				}
				//doc->setUsesAutomaticTextFrames(AtFl);
				Apage->LeftPg=pg.attribute("LEFT", "0").toInt();
				QString Mus = "";
				Mus = pg.attribute("MNAM","Normal");
				if (!doc->masterPageMode())
					Apage->MPageNam = Mus;
				else
					Apage->MPageNam = "";
				if (pg.hasAttribute("Size"))
					Apage->PageSize = pg.attribute("Size");
				if (pg.hasAttribute("Orientation"))
					Apage->PageOri = pg.attribute("Orientation").toInt();
				Apage->setXOffset(pg.attribute("PAGEXPOS").toDouble());
				Apage->setYOffset(pg.attribute("PAGEYPOS").toDouble());
				if (pg.hasAttribute("PAGEWIDTH"))
					Apage->setWidth(pg.attribute("PAGEWIDTH").toDouble());
				else
					Apage->setWidth(pg.attribute("PAGEWITH").toDouble());
				Apage->setHeight(pg.attribute("PAGEHEIGHT").toDouble());
				Apage->setInitialHeight(Apage->height());
				Apage->setInitialWidth(Apage->width());
				Apage->initialMargins.Top = pg.attribute("BORDERTOP").toDouble();
				Apage->initialMargins.Bottom = pg.attribute("BORDERBOTTOM").toDouble();
				Apage->initialMargins.Left = pg.attribute("BORDERLEFT").toDouble();
				Apage->initialMargins.Right = pg.attribute("BORDERRIGHT").toDouble();
				Apage->Margins.Top = Apage->initialMargins.Top;
				Apage->Margins.Bottom = Apage->initialMargins.Bottom;
				doc->setMasterPageMode(false);
				//doc->Pages=&doc->DocPages;
				if ((pg.hasAttribute("NumVGuides")) && (pg.attribute("NumVGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("VerticalGuides");
					QTextStream fgv(&tmp, IO_ReadOnly);
					Apage->YGuides.clear();
					for (int cxv = 0; cxv < pg.attribute("NumVGuides", "0").toInt(); ++cxv)
					{
						fgv >> xf;
						Apage->YGuides.append(xf);
					}
					qHeapSort(Apage->YGuides);
					tmp = "";
				}
				else
					Apage->YGuides.clear();
				if ((pg.hasAttribute("NumHGuides")) && (pg.attribute("NumHGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("HorizontalGuides");
					QTextStream fgh(&tmp, IO_ReadOnly);
					Apage->XGuides.clear();
					for (int cxh = 0; cxh < pg.attribute("NumHGuides", "0").toInt(); ++cxh)
					{
						fgh >> xf;
						Apage->XGuides.append(xf);
					}
					qHeapSort(Apage->XGuides);
					tmp = "";
				}
				else
					Apage->XGuides.clear();
			}
			if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="MASTEROBJECT") || (pg.tagName()=="FRAMEOBJECT"))
			{
					if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="FRAMEOBJECT"))
					{
						//doc->Items = doc->DocItems;
						//doc->Pages = &doc->DocPages;
						doc->setMasterPageMode(false);
					}
					else
					{
						//doc->Items = doc->MasterItems;
						//doc->Pages = &doc->MasterPages;
						doc->setMasterPageMode(true);
					}
					if ((!pg.attribute("OnMasterPage").isEmpty()) && (pg.tagName()=="MASTEROBJECT"))
						doc->currentPage = doc->MasterPages.at(doc->MasterNames[pg.attribute("OnMasterPage")]);
					if ((pg.attribute("NEXTITEM").toInt() != -1) || (static_cast<bool>(pg.attribute("AUTOTEXT").toInt())))
					{
						if (pg.attribute("BACKITEM").toInt() == -1)
							LFrames.append(doc->Items->count());
					}
					int docGc = doc->GroupCounter;
					doc->GroupCounter = 0;
					Neu = PasteItem(&pg, doc);
					Neu->setRedrawBounding();
					Neu->OwnPage = pg.attribute("OwnPage").toInt();
					if (pg.tagName()=="PAGEOBJECT")
						Neu->OnMasterPage = "";
					doc->GroupCounter = docGc;
					tmpf = pg.attribute("IFONT", doc->toolSettings.defFont);
					if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
					{
						if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
						{
							newReplacement = true;
							ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
						}
						else
							ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
					}
					else
					{
						if (!doc->UsedFonts.contains(tmpf))
						{
//							QFont fo = avail[tmpf]->Font;
//							fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
							doc->AddFont(tmpf, qRound(doc->toolSettings.defSize / 10.0));
						}
					}
					Neu->setFont(tmpf);
					QDomNode IT=pg.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = it.attribute("RAMP", "0.0").toDouble();
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = it.attribute("TRANS", "1").toDouble();
							Neu->fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							GetItemText(&it, doc, Neu);
						
						//CB PageItemAttributes
						if(it.tagName()=="PageItemAttributes")
						{
							QDomNode PIA = it.firstChild();
							ObjAttrVector pageItemAttributes;
							while(!PIA.isNull())
							{
								QDomElement itemAttr = PIA.toElement();
								if(itemAttr.tagName() == "ItemAttribute")
								{
									ObjectAttribute objattr;
									objattr.name=itemAttr.attribute("Name");
									objattr.type=itemAttr.attribute("Type");
									objattr.value=itemAttr.attribute("Value");
									objattr.parameter=itemAttr.attribute("Parameter");
									objattr.relationship=itemAttr.attribute("Relationship");
									objattr.relationshipto=itemAttr.attribute("RelationshipTo");
									objattr.autoaddto=itemAttr.attribute("AutoAddTo");
									pageItemAttributes.append(objattr);
								}
								PIA = PIA.nextSibling();
							}
							Neu->setObjectAttributes(&pageItemAttributes);
						}
						IT=IT.nextSibling();
					}
					if (Neu->fill_gradient.Stops() == 0)
					{
						Neu->fill_gradient.addStop(doc->PageColors[doc->toolSettings.dBrush].getRGBColor(), 0.0, 0.5, 1.0, doc->toolSettings.dBrush, 100);
						Neu->fill_gradient.addStop(doc->PageColors[doc->toolSettings.dPen].getRGBColor(), 1.0, 0.5, 1.0, doc->toolSettings.dPen, 100);
					}
					Neu->Language = ScMW->GetLang(pg.attribute("LANGUAGE", doc->Language));
					Neu->isAutoText = static_cast<bool>(pg.attribute("AUTOTEXT").toInt());
					Neu->isEmbedded = static_cast<bool>(pg.attribute("isInline", "0").toInt());
					Neu->gXpos = pg.attribute("gXpos", "0.0").toDouble();
					Neu->gYpos = pg.attribute("gYpos", "0.0").toDouble();
					QString defaultVal;
					defaultVal.setNum(Neu->width());
					Neu->gWidth = pg.attribute("gWidth",defaultVal).toDouble();
					defaultVal.setNum(Neu->height());
					Neu->gHeight = pg.attribute("gHeight",defaultVal).toDouble();
					if (Neu->LineSpMode == 3)
					{
						doc->docParagraphStyles[0].BaseAdj = true;
						Neu->LineSp = doc->typographicSettings.valueBaseGrid-1;
					}
					if (Neu->isAutoText)
						doc->LastAuto = Neu;
					Neu->NextIt = pg.attribute("NEXTITEM").toInt();
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
					}
					if (pg.tagName()=="FRAMEOBJECT")
					{
						doc->FrameItems.append(doc->Items->take(Neu->ItemNr));
						Neu->ItemNr = doc->FrameItems.count()-1;
					}
					/*
					if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="FRAMEOBJECT"))
					{
						//doc->DocItems = doc->Items;
						//doc->DocPages = doc->Pages;
					}
					else
					{
						//doc->MasterItems = doc->Items;
						//doc->MasterPages = doc->Pages;
					}
					*/
					doc->setMasterPageMode(false);
					//doc->Pages=&doc->DocPages;
					counter++;
				}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	if (TableItems.count() != 0)
	{
		for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	doc->setActiveLayer(layerToSetActive);
	//doc->Pages = &doc->DocPages;
	doc->setMasterPageMode(false);
	//doc->pageCount = doc->Pages->count();
	//doc->Items = doc->DocItems;
	//ScMW->view->reformPages();
	doc->reformPages(maximumX, maximumY);
	if (doc->Layers.count() == 0)
	{
		la.LNr = 0;
		la.Level = 0;
		la.Name = QObject::tr("Background");
		la.isViewable = true;
		la.isPrintable = true;
		doc->Layers.append(la);
	}
	if (LFrames.count() != 0)
	{
		PageItem *Its;
		PageItem *Itn;
		PageItem *Itr;
		QValueList<int>::Iterator lc;
		for (lc = LFrames.begin(); lc != LFrames.end(); ++lc)
		{
			Its = doc->Items->at((*lc));
			Itr = Its;
			Its->BackBox = 0;
			if (Its->isAutoText)
				doc->FirstAuto = Its;
			while (Its->NextIt != -1)
			{
				Itn = doc->Items->at(Its->NextIt);
				Its->NextBox = Itn;
				Itn->BackBox = Its;
				Its = Itn;
			}
			Its->NextBox = 0;
		}
	}
	dia2->setProgress(DOC.childNodes().count());
	return true;
}

void FileLoader::GetItemText(QDomElement *it, ScribusDoc *doc, PageItem* obj, bool impo, bool VorLFound)
{
	struct ScText *hg;
	Foi* dummy;
	bool unknown = false;
	QString tmp2, tmpf;
	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(13));
	tmp2.replace(QRegExp("\n"), QChar(13));
	tmp2.replace(QRegExp("\t"), QChar(9));
	tmpf = it->attribute("CFONT", doc->toolSettings.defFont);
	if ((!prefsManager->appPrefs.AvailFonts.find(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf]->UseFont))
	{
		bool isThere = false;
		for (uint dl = 0; dl < dummyFois.count(); ++dl)
		{
			if (dummyFois.at(dl)->scName() == tmpf)
			{
				isThere = true;
				dummy = dummyFois.at(dl);
				break;
			}
		}
		if (!isThere)
		{
			dummy = new Foi(tmpf, "", tmpf, "", "", 1, false);
			dummyFois.append(dummy);
		}
		unknown = true;
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
		{
			newReplacement = true;
			ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
		}
		else
			ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
	}
	else
	{
		if (!doc->UsedFonts.contains(tmpf))
		{
//			QFont fo = prefsManager->appPrefs.AvailFonts[tmpf]->Font;
//			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			doc->AddFont(tmpf, qRound(doc->toolSettings.defSize / 10.0));
		}
	}
	int size = qRound(it->attribute("CSIZE").toDouble() * 10);
	QString fcolor = it->attribute("CCOLOR");
	int extra;
	if (it->hasAttribute("CEXTRA"))
		extra = qRound(it->attribute("CEXTRA").toDouble() / it->attribute("CSIZE").toDouble() * 1000.0);
	else
		extra = it->attribute("CKERN").toInt();
	int shade = it->attribute("CSHADE").toInt();
	int style = it->attribute("CSTYLE").toInt();
	int ab = it->attribute("CAB", "0").toInt();
	QString stroke = it->attribute("CSTROKE","None");
	int shade2 = it->attribute("CSHADE2", "100").toInt();
	int scale = qRound(it->attribute("CSCALE", "100").toDouble() * 10);
	int scalev = qRound(it->attribute("CSCALEV", "100").toDouble() * 10);
	int base = qRound(it->attribute("CBASE", "0").toDouble() * 10);
	int shX = qRound(it->attribute("CSHX", "5").toDouble() * 10);
	int shY = qRound(it->attribute("CSHY", "-5").toDouble() * 10);
	int outL = qRound(it->attribute("COUT", "1").toDouble() * 10);
	int ulp = qRound(it->attribute("CULP", "-0.1").toDouble() * 10);
	int ulw = qRound(it->attribute("CULW", "-0.1").toDouble() * 10);
	int stp = qRound(it->attribute("CSTP", "-0.1").toDouble() * 10);
	int stw = qRound(it->attribute("CSTW", "-0.1").toDouble() * 10);
	int iobj = it->attribute("COBJ", "-1").toInt();
	for (uint cxx=0; cxx<tmp2.length(); ++cxx)
	{
		hg = new ScText;
		hg->ch = tmp2.at(cxx);
		if (hg->ch == QChar(5))
			hg->ch = QChar(13);
		if (hg->ch == QChar(4))
			hg->ch = QChar(9);
		if (unknown)
			hg->cfont = dummy;
		else
			hg->cfont = (*doc->AllFonts)[tmpf];
		hg->csize = size;
		hg->ccolor = fcolor;
		hg->cextra = extra;
		hg->cshade = shade;
		hg->cselect = false;
		hg->cstyle = style;
		if (impo)
		{
			if (VorLFound)
				hg->cab = DoVorl[ab].toUInt();
			else
			{
				if (ab < 5)
					hg->cab = ab;
				else
					hg->cab = 0;
			}
		}
		else
			hg->cab = ab;
		hg->cstroke = stroke;
		hg->cshade2 = shade2;
		hg->cscale = QMIN(QMAX(scale, 100), 4000);
		hg->cscalev = QMIN(QMAX(scalev, 100), 4000);
		hg->cbase = base;
		hg->cshadowx = shX;
		hg->cshadowy = shY;
		hg->coutline = outL;
		hg->cunderpos = ulp;
		hg->cunderwidth = ulw;
		hg->cstrikepos = stp;
		hg->cstrikewidth = stw;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		if ((hg->ch == QChar(25)) && (iobj != -1))
			hg->cembedded = doc->FrameItems.at(iobj);
		else
			hg->cembedded = 0;
		obj->itemText.append(hg);
	}
	return;
}

PageItem* FileLoader::PasteItem(QDomElement *obj, ScribusDoc *doc)
{
	struct ScImage::LoadRequest loadingInfo;
	int z = 0;
	PageItem::ItemType pt = static_cast<PageItem::ItemType>(obj->attribute("PTYPE").toInt());
	double x = obj->attribute("XPOS").toDouble();
	double y = obj->attribute("YPOS").toDouble();
	double w = obj->attribute("WIDTH").toDouble();
	double h = obj->attribute("HEIGHT").toDouble();
	double pw = obj->attribute("PWIDTH").toDouble();
	double scx = obj->attribute("LOCALSCX").toDouble();
	double scy = obj->attribute("LOCALSCY").toDouble();
	QString Pcolor = obj->attribute("PCOLOR");
	QString Pcolor2 = obj->attribute("PCOLOR2");
	QColor tmpc;
	PageItem *currItem;
	QString tmp;
	int xi;
	double xf, yf, xf2;
	QString clPath;
	QDomNode IT;
	switch (pt)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		break;
	//
	case PageItem::ImageFrame:
		z = doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x, y, w, h, 1, doc->toolSettings.dBrushPict, "None", true);
		currItem = doc->Items->at(z);
		currItem->setImageXYScale(scx, scy);
		currItem->setImageXYOffset(obj->attribute("LOCALX").toDouble(), obj->attribute("LOCALY").toDouble());
		currItem->Pfile = obj->attribute("PFILE");
		currItem->IProfile = obj->attribute("PRFILE","");
		currItem->EmProfile = obj->attribute("EPROF","");
		currItem->IRender = obj->attribute("IRENDER", "1").toInt();
		currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
		currItem->pixm.imgInfo.lowResType = obj->attribute("ImageRes", "1").toInt();
		IT = obj->firstChild();
		while(!IT.isNull())
		{
			QDomElement it = IT.toElement();
			if (it.tagName()=="ImageEffect")
			{
				struct ScImage::imageEffect ef;
				ef.effectParameters = it.attribute("Param");
				ef.effectCode = it.attribute("Code").toInt();
				currItem->effectsInUse.append(ef);
			}
			IT=IT.nextSibling();
		}
		if (!currItem->Pfile.isEmpty())
			doc->loadPict(currItem->Pfile, currItem, false);
		currItem->IProfile = obj->attribute("PRFILE","");
		currItem->EmProfile = obj->attribute("EPROF","");
		currItem->IRender = obj->attribute("IRENDER", "1").toInt();
		currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
		currItem->setImageXYScale(scx, scy);
		clPath = obj->attribute("ImageClip", "");
		if (currItem->pixm.imgInfo.PDSpathData.contains(clPath))
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[clPath].copy();
			currItem->pixm.imgInfo.usedPath = clPath;
			QWMatrix cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		currItem->PicArt = obj->attribute("PICART").toInt();
/*		currItem->BBoxX = obj->attribute("BBOXX").toDouble();
		currItem->BBoxH = obj->attribute("BBOXH").toDouble(); */
		currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
		currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
		currItem->Pwidth = pw;
		if (currItem->pixm.imgInfo.layerInfo.count() != 0)
		{
			bool found = false;
			IT = obj->firstChild();
			while(!IT.isNull())
			{
				QDomElement it = IT.toElement();
				if (it.tagName() == "PSDLayer")
				{
					found = true;
					loadingInfo.blend = it.attribute("Blend");
					loadingInfo.opacity = it.attribute("Opacity").toInt();
					loadingInfo.visible = static_cast<bool>(it.attribute("Visible").toInt());
					currItem->pixm.imgInfo.RequestProps.insert(it.attribute("Layer").toInt(), loadingInfo);
				}
				IT=IT.nextSibling();
			}
			if (found)
			{
				currItem->pixm.imgInfo.isRequest = true;
				doc->loadPict(currItem->Pfile, currItem, true);
			}
		}
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType3:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		break;
	//
	case PageItem::PathText:
		z = doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, "None", Pcolor, true);
		currItem = doc->Items->at(z);
		if ((obj->attribute("ANNOTATION", "0").toInt()) && (static_cast<bool>(obj->attribute("ANICON", "0").toInt())))
		{
			currItem->setImageXYScale(scx, scy);
			currItem->setImageXYOffset(obj->attribute("LOCALX").toDouble(), obj->attribute("LOCALY").toDouble());
			currItem->Pfile = obj->attribute("PFILE");
			currItem->Pfile2 = obj->attribute("PFILE2","");
			currItem->Pfile3 = obj->attribute("PFILE3","");
			currItem->IProfile = obj->attribute("PRFILE","");
			currItem->EmProfile = obj->attribute("EPROF","");
			currItem->IRender = obj->attribute("IRENDER", "1").toInt();
			currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
			doc->LoadPict(currItem->Pfile, z);
			currItem->setImageXYScale(scx, scy);
			currItem->PicArt = obj->attribute("PICART").toInt();
/*			currItem->BBoxX = obj->attribute("BBOXX").toDouble();
			currItem->BBoxH = obj->attribute("BBOXH").toDouble(); */
			currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
			currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
		}
		currItem->LineSp = obj->attribute("LINESP").toDouble();
		currItem->LineSpMode = obj->attribute("LINESPMode", "0").toInt();
		//currItem->convertTo(pt);
		break;
	case PageItem::TextFrame:
		z = doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, "None", Pcolor, true);
		currItem = doc->Items->at(z);
		if ((obj->attribute("ANNOTATION", "0").toInt()) && (static_cast<bool>(obj->attribute("ANICON", "0").toInt())))
		{
			currItem->setImageXYScale(scx, scy);
			currItem->setImageXYOffset(obj->attribute("LOCALX").toDouble(), obj->attribute("LOCALY").toDouble());
			currItem->Pfile = obj->attribute("PFILE");
			currItem->Pfile2 = obj->attribute("PFILE2","");
			currItem->Pfile3 = obj->attribute("PFILE3","");
			currItem->IProfile = obj->attribute("PRFILE","");
			currItem->EmProfile = obj->attribute("EPROF","");
			currItem->IRender = obj->attribute("IRENDER", "1").toInt();
			currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
			doc->LoadPict(currItem->Pfile, z);
			currItem->setImageXYScale(scx, scy);
			currItem->PicArt = obj->attribute("PICART").toInt();
/*			currItem->BBoxX = obj->attribute("BBOXX").toDouble();
			currItem->BBoxH = obj->attribute("BBOXH").toDouble(); */
			currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
			currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
		}
		currItem->LineSp = obj->attribute("LINESP").toDouble();
		currItem->LineSpMode = obj->attribute("LINESPMode", "0").toInt();
		//currItem->convertTo(pt);
		break;
	case PageItem::Line:
		z = doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w, h, pw, "None", Pcolor2, true);
		currItem = doc->Items->at(z);
		break;
	case PageItem::Polygon:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		break;
	case PageItem::PolyLine:
		z = doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		break;
	}
	currItem->FrameType = obj->attribute("FRTYPE", "0").toInt();
	currItem->setStartArrowIndex(obj->attribute("startArrowIndex", "0").toInt());
	currItem->setEndArrowIndex(obj->attribute("endArrowIndex", "0").toInt());
	currItem->NamedLStyle = obj->attribute("NAMEDLST", "");
	currItem->isBookmark = obj->attribute("BOOKMARK").toInt();
	if ((currItem->isBookmark) && (doc->BookMarks.count() == 0))
		doc->OldBM = true;
	currItem->BMnr = obj->attribute("BookNr", "0").toInt();
	currItem->textAlignment = obj->attribute("ALIGN", "0").toInt();
	currItem->setImageFlippedH(obj->attribute("FLIPPEDH").toInt());
	currItem->setImageFlippedV(obj->attribute("FLIPPEDV").toInt());
	currItem->setCornerRadius(obj->attribute("RADRECT", "0").toDouble());
	currItem->ClipEdited = obj->attribute("CLIPEDIT", "0").toInt();
	currItem->setFillColor(Pcolor);
	currItem->setLineColor(Pcolor2);
	currItem->setFillShade(obj->attribute("SHADE").toInt());
	currItem->setLineShade(obj->attribute("SHADE2").toInt());
	//CB Moved to setFillColor and setFillShade
	//if (currItem->fillColor() != "None")
	//	currItem->fillQColor = doc->PageColors[currItem->fillColor()].getShadeColorProof(currItem->fillShade());
	//if (currItem->lineColor() != "None")
	//	currItem->strokeQColor = doc->PageColors[currItem->lineColor()].getShadeColorProof(currItem->lineShade());

	currItem->TxtStroke = obj->attribute("TXTSTROKE", "None");
	currItem->TxtFill = obj->attribute("TXTFILL", "Black");
	currItem->ShTxtStroke = obj->attribute("TXTSTRSH", "100").toInt();
	currItem->ShTxtFill = obj->attribute("TXTFILLSH", "100").toInt();
	currItem->TxtScale=qRound(obj->attribute("TXTSCALE", "100").toDouble() * 10);
	currItem->TxtScaleV=qRound(obj->attribute("TXTSCALEV", "100").toDouble() * 10);
	currItem->TxtBase=qRound(obj->attribute("TXTBASE", "0").toDouble() * 10);
	currItem->TxtShadowX=qRound(obj->attribute("TXTSHX", "5").toDouble() * 10);
	currItem->TxtShadowY=qRound(obj->attribute("TXTSHY", "-5").toDouble() * 10);
	currItem->TxtOutline=qRound(obj->attribute("TXTOUT", "1").toDouble() * 10);
	currItem->TxtUnderPos=qRound(obj->attribute("TXTULP", "-0.1").toDouble() * 10);
	currItem->TxtUnderWidth=qRound(obj->attribute("TXTULW", "-0.1").toDouble() * 10);
	currItem->TxtStrikePos=qRound(obj->attribute("TXTSTP", "-0.1").toDouble() * 10);
	currItem->TxtStrikeWidth=qRound(obj->attribute("TXTSTW", "-0.1").toDouble() * 10);
	currItem->TxTStyle = obj->attribute("TXTSTYLE", "0").toInt();
	currItem->setRotation(obj->attribute("ROT").toDouble());
	currItem->setTextToFrameDist(obj->attribute("EXTRA").toDouble(),
								obj->attribute("REXTRA", "1").toDouble(),
								obj->attribute("TEXTRA", "1").toDouble(),
								obj->attribute("BEXTRA", "1").toDouble());
	
	currItem->PLineArt = Qt::PenStyle(obj->attribute("PLINEART").toInt());
	currItem->PLineEnd = Qt::PenCapStyle(obj->attribute("PLINEEND", "0").toInt());
	currItem->PLineJoin = Qt::PenJoinStyle(obj->attribute("PLINEJOIN", "0").toInt());
	currItem->setPrintable(obj->attribute("PRINTABLE").toInt());
	currItem->setIsAnnotation(obj->attribute("ANNOTATION", "0").toInt());
	currItem->annotation().setType(obj->attribute("ANTYPE", "0").toInt());
	QString AnName = obj->attribute("ANNAME","");
	if (!AnName.isEmpty())
	{
		if (currItem->itemName() == AnName)
			currItem->AutoName = true;
		else
		{
			currItem->setItemName(AnName);
			currItem->AutoName = false;
		}
	}
	currItem->annotation().setAction(obj->attribute("ANACTION",""));
	currItem->annotation().setE_act(obj->attribute("ANEACT",""));
	currItem->annotation().setX_act(obj->attribute("ANXACT",""));
	currItem->annotation().setD_act(obj->attribute("ANDACT",""));
	currItem->annotation().setFo_act(obj->attribute("ANFOACT",""));
	currItem->annotation().setBl_act(obj->attribute("ANBLACT",""));
	currItem->annotation().setK_act(obj->attribute("ANKACT",""));
	currItem->annotation().setF_act(obj->attribute("ANFACT",""));
	currItem->annotation().setV_act(obj->attribute("ANVACT",""));
	currItem->annotation().setC_act(obj->attribute("ANCACT",""));
	currItem->annotation().setActionType(obj->attribute("ANACTYP", "0").toInt());
	currItem->annotation().setExtern(obj->attribute("ANEXTERN",""));
	if ((!currItem->annotation().Extern().isEmpty()) && (currItem->annotation().ActionType() != 8))
	{
		QFileInfo efp(currItem->annotation().Extern());
		currItem->annotation().setExtern(efp.absFilePath());
	}
	currItem->annotation().setZiel(obj->attribute("ANZIEL", "0").toInt());
	currItem->annotation().setToolTip(obj->attribute("ANTOOLTIP",""));
	currItem->annotation().setRollOver(obj->attribute("ANROLL",""));
	currItem->annotation().setDown(obj->attribute("ANDOWN",""));
	currItem->annotation().setBwid(obj->attribute("ANBWID", "1").toInt());
	currItem->annotation().setBsty(obj->attribute("ANBSTY", "0").toInt());
	currItem->annotation().setFeed(obj->attribute("ANFEED", "1").toInt());
	currItem->annotation().setFlag(obj->attribute("ANFLAG", "0").toInt());
	currItem->annotation().setFont(obj->attribute("ANFONT", "4").toInt());
	currItem->annotation().setFormat(obj->attribute("ANFORMAT", "0").toInt());
	currItem->annotation().setVis(obj->attribute("ANVIS", "0").toInt());
	currItem->annotation().setIsChk(static_cast<bool>(obj->attribute("ANCHK", "0").toInt()));
	currItem->annotation().setAAact(static_cast<bool>(obj->attribute("ANAA", "0").toInt()));
	currItem->annotation().setHTML(static_cast<bool>(obj->attribute("ANHTML", "0").toInt()));
	currItem->annotation().setUseIcons(static_cast<bool>(obj->attribute("ANICON", "0").toInt()));
	currItem->annotation().setChkStil(obj->attribute("ANCHKS", "0").toInt());
	currItem->annotation().setMaxChar(obj->attribute("ANMC", "-1").toInt());
	currItem->annotation().setBorderColor(obj->attribute("ANBCOL","None"));
	currItem->annotation().setIPlace(obj->attribute("ANPLACE", "1").toInt());
	currItem->annotation().setScaleW(obj->attribute("ANSCALE", "0").toInt());
	currItem->TopLine = static_cast<bool>(obj->attribute("TopLine", "0").toInt());
	currItem->LeftLine = static_cast<bool>(obj->attribute("LeftLine", "0").toInt());
	currItem->RightLine = static_cast<bool>(obj->attribute("RightLine", "0").toInt());
	currItem->BottomLine = static_cast<bool>(obj->attribute("BottomLine", "0").toInt());
	currItem->isTableItem = static_cast<bool>(obj->attribute("isTableItem", "0").toInt());
	currItem->TopLinkID =  obj->attribute("TopLINK", "-1").toInt();
	currItem->LeftLinkID =  obj->attribute("LeftLINK", "-1").toInt();
	currItem->RightLinkID =  obj->attribute("RightLINK", "-1").toInt();
	currItem->BottomLinkID =  obj->attribute("BottomLINK", "-1").toInt();
	currItem->PoShow = obj->attribute("PLTSHOW", "0").toInt();
	currItem->BaseOffs = obj->attribute("BASEOF", "0").toDouble();
	currItem->setTextFlowsAroundFrame(obj->attribute("TEXTFLOW").toInt());
	currItem->setTextFlowUsesBoundingBox(obj->attribute("TEXTFLOW2", "0").toInt());
	currItem->setFontSize(qRound(obj->attribute("ISIZE", "12").toDouble() * 10));
	if (obj->hasAttribute("EXTRAV"))
		currItem->ExtraV = qRound(obj->attribute("EXTRAV", "0").toDouble() / obj->attribute("ISIZE", "12").toDouble() * 1000.0);
	else
		currItem->ExtraV = obj->attribute("TXTKERN").toInt();
	currItem->DashOffset = obj->attribute("DASHOFF", "0.0").toDouble();
	currItem->setReversed(static_cast<bool>(obj->attribute("REVERS", "0").toInt()));
	currItem->setLocked(static_cast<bool>(obj->attribute("LOCK", "0").toInt()));
	currItem->setSizeLocked(static_cast<bool>(obj->attribute("LOCKR", "0").toInt()));
	currItem->setFillTransparency(obj->attribute("TransValue", "0.0").toDouble());
	if (obj->hasAttribute("TransValueS"))
		currItem->setLineTransparency(obj->attribute("TransValueS", "0.0").toDouble());
	else
		currItem->setLineTransparency(obj->attribute("TransValue", "0.0").toDouble());
	if (obj->attribute("TRANSPARENT", "0").toInt() == 1)
		currItem->setFillColor("None");
	currItem->Cols = obj->attribute("COLUMNS", "1").toInt();
	currItem->ColGap = obj->attribute("COLGAP", "0.0").toDouble();
	if (obj->attribute("LAYER", "0").toInt() != -1)
		currItem->LayerNr = obj->attribute("LAYER", "0").toInt();
	currItem->setTextFlowUsesContourLine(obj->attribute("TEXTFLOW3", "0").toInt());
	tmp = "";
	if ((obj->hasAttribute("GROUPS")) && (obj->attribute("NUMGROUP", "0").toInt() != 0))
	{
		tmp = obj->attribute("GROUPS");
		QTextStream fg(&tmp, IO_ReadOnly);
		currItem->Groups.clear();
		for (int cx = 0; cx < obj->attribute("NUMGROUP", "0").toInt(); ++cx)
		{
			fg >> xi;
			currItem->Groups.push(xi);
		}
		tmp = "";
	}
	else
		currItem->Groups.clear();
	tmp = "";
	currItem->TabValues.clear();
	if ((obj->hasAttribute("NUMTAB")) && (obj->attribute("NUMTAB", "0").toInt() != 0))
	{
		struct PageItem::TabRecord tb;
		tmp = obj->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		for (int cxv = 0; cxv < obj->attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			currItem->TabValues.append(tb);
		}
		tmp = "";
	}
	else
	{
		IT = obj->firstChild();
		while(!IT.isNull())
		{
			QDomElement it = IT.toElement();
			if (it.tagName()=="Tabs")
			{
				struct PageItem::TabRecord tb;
				tb.tabPosition = it.attribute("Pos").toDouble();
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				currItem->TabValues.append(tb);
			}
			IT=IT.nextSibling();
		}
	}
	if ((obj->hasAttribute("NUMDASH")) && (obj->attribute("NUMDASH", "0").toInt() != 0))
	{
		tmp = obj->attribute("DASHS");
		QTextStream dgv(&tmp, IO_ReadOnly);
		currItem->DashValues.clear();
		for (int cxv = 0; cxv < obj->attribute("NUMDASH", "0").toInt(); ++cxv)
		{
			dgv >> xf;
			currItem->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		currItem->DashValues.clear();
	tmp = "";
	if (obj->hasAttribute("NUMPO"))
	{
		currItem->PoLine.resize(obj->attribute("NUMPO").toUInt());
		tmp = obj->attribute("POCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMPO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			currItem->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		currItem->PoLine.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMCO"))
	{
		currItem->ContourLine.resize(obj->attribute("NUMCO").toUInt());
		tmp = obj->attribute("COCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMCO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			currItem->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		currItem->ContourLine = currItem->PoLine.copy();
	if (!currItem->asLine())
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	else
	{
		int ph = static_cast<int>(QMAX(1.0, currItem->Pwidth / 2.0));
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
		currItem->setHeight(1.0);
	}
	if (currItem->asImageFrame())
		currItem->AdjustPictScale();
	if (!(currItem->asTextFrame()) && !(currItem->asPathText()))
		currItem->setFont(doc->toolSettings.defFont);
	if (currItem->asPathText())
	{
		currItem->UpdatePolyClip();
		currItem->Frame = true;
	}
	currItem->GrType = obj->attribute("GRTYP", "0").toInt();
	QString GrColor;
	QString GrColor2;
	int GrShade;
	int GrShade2;
	if (currItem->GrType != 0)
	{
		currItem->GrStartX = obj->attribute("GRSTARTX", "0.0").toDouble();
		currItem->GrStartY = obj->attribute("GRSTARTY", "0.0").toDouble();
		currItem->GrEndX = obj->attribute("GRENDX", "0.0").toDouble();
		currItem->GrEndY = obj->attribute("GRENDY", "0.0").toDouble();
		GrColor = obj->attribute("GRCOLOR","");
		if (!GrColor.isEmpty())
		{
			GrColor2 = obj->attribute("GRCOLOR2","");
			GrShade = obj->attribute("GRSHADE", "100").toInt();
			GrShade2 = obj->attribute("GRSHADE2", "100").toInt();
		}
	}
	if (currItem->GrType != 0)
	{
		currItem->fill_gradient.clearStops();
		if ((!GrColor.isEmpty()) && (!GrColor2.isEmpty()))
		{
			if (currItem->GrType == 5)
			{
				if ((GrColor != "None") && (!GrColor.isEmpty()))
					currItem->SetFarbe(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor, GrShade);
				if ((GrColor2 != "None") && (!GrColor2.isEmpty()))
					currItem->SetFarbe(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor2, GrShade2);
			}
			else
			{
				if ((GrColor2 != "None") && (!GrColor2.isEmpty()))
					currItem->SetFarbe(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor2, GrShade2);
				if ((GrColor != "None") && (!GrColor.isEmpty()))
					currItem->SetFarbe(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor, GrShade);
			}
		}
		//ScMW->view->updateGradientVectors(currItem);
		currItem->updateGradientVectors();
	}
	//currItem->setRedrawBounding();
	//currItem->OwnPage = view->OnPage(currItem);
	return currItem;
}

void FileLoader::GetStyle(QDomElement *pg, struct ParagraphStyle *vg, QValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl)
{
	bool fou;
	QString tmpf, tmf, tmV;
	double xf, xf2;
	fou = false;
	bool tabEQ = false;
	vg->Vname = pg->attribute("NAME");
	vg->LineSpaMode = pg->attribute("LINESPMode", "0").toInt();
	vg->LineSpa = pg->attribute("LINESP").toDouble();
	vg->Indent = pg->attribute("INDENT", "0").toDouble();
	vg->First = pg->attribute("FIRST", "0").toDouble();
	vg->textAlignment = pg->attribute("ALIGN").toInt();
	vg->gapBefore = pg->attribute("VOR", "0").toDouble();
	vg->gapAfter = pg->attribute("NACH", "0").toDouble();
	tmpf = pg->attribute("FONT", doc->toolSettings.defFont);
	if ((!prefsManager->appPrefs.AvailFonts.find(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf]->UseFont))
	{
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
		{
			newReplacement = true;
			ReplacedFonts.insert(tmpf, doc->toolSettings.defFont);
		}
		else
			ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
	}
	else
	{
		if (!doc->UsedFonts.contains(tmpf))
		{
//			QFont fo = prefsManager->appPrefs.AvailFonts[tmpf]->Font;
//			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			doc->AddFont(tmpf, qRound(doc->toolSettings.defSize / 10.0));
		}
	}
	vg->Font = tmpf;
	vg->FontSize = qRound(pg->attribute("FONTSIZE", "12").toDouble() * 10.0);
	vg->Drop = static_cast<bool>(pg->attribute("DROP", "0").toInt());
	vg->DropLin = pg->attribute("DROPLIN", "2").toInt();
	vg->DropDist = pg->attribute("DROPDIST", "0").toDouble();
	vg->FontEffect = pg->attribute("EFFECT", "0").toInt();
	vg->FColor = pg->attribute("FCOLOR", doc->toolSettings.dBrush);
	vg->FShade = pg->attribute("FSHADE", "100").toInt();
	vg->SColor = pg->attribute("SCOLOR", doc->toolSettings.dPen);
	vg->SShade = pg->attribute("SSHADE", "100").toInt();
	vg->BaseAdj = static_cast<bool>(pg->attribute("BASE", "0").toInt());
	vg->txtShadowX = qRound(pg->attribute("TXTSHX", "5").toDouble() * 10);
	vg->txtShadowY = qRound(pg->attribute("TXTSHY", "-5").toDouble() * 10);
	vg->txtOutline = qRound(pg->attribute("TXTOUT", "1").toDouble() * 10);
	vg->txtUnderPos = qRound(pg->attribute("TXTULP", "-0.1").toDouble() * 10);
	vg->txtUnderWidth = qRound(pg->attribute("TXTULW", "-0.1").toDouble() * 10);
	vg->txtStrikePos = qRound(pg->attribute("TXTSTP", "-0.1").toDouble() * 10);
	vg->txtStrikeWidth = qRound(pg->attribute("TXTSTW", "-0.1").toDouble() * 10);
	vg->scaleH = qRound(pg->attribute("SCALEH", "100").toDouble() * 10);
	vg->scaleV = qRound(pg->attribute("SCALEV", "100").toDouble() * 10);
	vg->baseOff = qRound(pg->attribute("BASEO", "0").toDouble() * 10);
	vg->kernVal = qRound(pg->attribute("KERN", "0").toDouble() * 10);
	vg->TabValues.clear();
	if ((pg->hasAttribute("NUMTAB")) && (pg->attribute("NUMTAB", "0").toInt() != 0))
	{
		struct PageItem::TabRecord tb;
		QString tmp = pg->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		vg->TabValues.clear();
		for (int cxv = 0; cxv < pg->attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			vg->TabValues.append(tb);
		}
		tmp = "";
	}
	else
	{
		QDomNode IT = pg->firstChild();
		while(!IT.isNull())
		{
			QDomElement it = IT.toElement();
			if (it.tagName()=="Tabs")
			{
				struct PageItem::TabRecord tb;
				tb.tabPosition = it.attribute("Pos").toDouble();
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				vg->TabValues.append(tb);
			}
			IT=IT.nextSibling();
		}
	}
	for (uint xx=0; xx<docParagraphStyles.count(); ++xx)
	{
		if (vg->Vname == docParagraphStyles[xx].Vname)
		{
			struct PageItem::TabRecord tb;
			tabEQ = false;
			if ((docParagraphStyles[xx].TabValues.count() == 0) && (vg->TabValues.count() == 0))
				tabEQ = true;
			else
			{
				for (uint t1 = 0; t1 < docParagraphStyles[xx].TabValues.count(); t1++)
				{
					tb.tabPosition = docParagraphStyles[xx].TabValues[t1].tabPosition;
					tb.tabType = docParagraphStyles[xx].TabValues[t1].tabType;
					tb.tabFillChar = docParagraphStyles[xx].TabValues[t1].tabFillChar;
					for (uint t2 = 0; t2 < vg->TabValues.count(); t2++)
					{
						struct PageItem::TabRecord tb2;
						tb2.tabPosition = vg->TabValues[t2].tabPosition;
						tb2.tabType = vg->TabValues[t2].tabType;
						tb2.tabFillChar = vg->TabValues[t2].tabFillChar;
						if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
						{
							tabEQ = true;
							break;
						}
					}
					if (tabEQ)
						break;
				}
			}
			if ((vg->LineSpa == docParagraphStyles[xx].LineSpa) &&
					(vg->LineSpaMode == docParagraphStyles[xx].LineSpaMode) &&
					(vg->Indent == docParagraphStyles[xx].Indent) &&
					(vg->First == docParagraphStyles[xx].First) &&
					(vg->textAlignment == docParagraphStyles[xx].textAlignment) &&
					(vg->gapBefore == docParagraphStyles[xx].gapBefore) &&
					(vg->gapAfter == docParagraphStyles[xx].gapAfter) &&
					(vg->Font == docParagraphStyles[xx].Font) && (tabEQ) &&
					(vg->Drop == docParagraphStyles[xx].Drop) &&
					(vg->DropLin == docParagraphStyles[xx].DropLin) &&
					(vg->DropDist == docParagraphStyles[xx].DropDist) &&
					(vg->FontEffect == docParagraphStyles[xx].FontEffect) &&
					(vg->FColor == docParagraphStyles[xx].FColor) &&
					(vg->FShade == docParagraphStyles[xx].FShade) &&
					(vg->SColor == docParagraphStyles[xx].SColor) &&
					(vg->SShade == docParagraphStyles[xx].SShade) &&
					(vg->BaseAdj == docParagraphStyles[xx].BaseAdj) &&
					(vg->txtShadowX == docParagraphStyles[xx].txtShadowX) &&
					(vg->txtShadowY == docParagraphStyles[xx].txtShadowY) &&
					(vg->txtOutline == docParagraphStyles[xx].txtOutline) &&
					(vg->txtUnderPos == docParagraphStyles[xx].txtUnderPos) &&
					(vg->txtUnderWidth == docParagraphStyles[xx].txtUnderWidth) &&
					(vg->txtStrikePos == docParagraphStyles[xx].txtStrikePos) &&
					(vg->txtStrikeWidth == docParagraphStyles[xx].txtStrikeWidth) &&
					(vg->scaleH == docParagraphStyles[xx].scaleH) &&
					(vg->scaleV == docParagraphStyles[xx].scaleV) &&
					(vg->baseOff == docParagraphStyles[xx].baseOff) &&
					(vg->kernVal == docParagraphStyles[xx].kernVal) &&
					(vg->FontSize == docParagraphStyles[xx].FontSize))
			{
				if (fl)
				{
					DoVorl[VorlC] = tmV.setNum(xx);
					VorlC++;
				}
				fou = true;
			}
			else
			{
				vg->Vname = "Copy of "+docParagraphStyles[xx].Vname;
				fou = false;
			}
			break;
		}
	}
	if (!fou)
	{
		for (uint xx=0; xx< docParagraphStyles.count(); ++xx)
		{
			struct PageItem::TabRecord tb;
			tabEQ = false;
			for (uint t1 = 0; t1 < docParagraphStyles[xx].TabValues.count(); t1++)
			{
				tb.tabPosition = docParagraphStyles[xx].TabValues[t1].tabPosition;
				tb.tabType = docParagraphStyles[xx].TabValues[t1].tabType;
				tb.tabFillChar = docParagraphStyles[xx].TabValues[t1].tabFillChar;
				for (uint t2 = 0; t2 < vg->TabValues.count(); t2++)
				{
					struct PageItem::TabRecord tb2;
					tb2.tabPosition = vg->TabValues[t2].tabPosition;
					tb2.tabType = vg->TabValues[t2].tabType;
					tb2.tabFillChar = vg->TabValues[t2].tabFillChar;
					if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
					{
						tabEQ = true;
						break;
					}
				}
				if (tabEQ)
					break;
			}
			if ((vg->LineSpa == docParagraphStyles[xx].LineSpa) &&
				(vg->LineSpaMode == docParagraphStyles[xx].LineSpaMode) &&
				(vg->Indent == docParagraphStyles[xx].Indent) &&
				(vg->First == docParagraphStyles[xx].First) &&
				(vg->textAlignment == docParagraphStyles[xx].textAlignment) &&
				(vg->gapBefore == docParagraphStyles[xx].gapBefore) &&
				(vg->gapAfter == docParagraphStyles[xx].gapAfter) &&
				(vg->Font == docParagraphStyles[xx].Font) && (tabEQ) &&
				(vg->Drop == docParagraphStyles[xx].Drop) &&
				(vg->DropLin == docParagraphStyles[xx].DropLin) &&
				(vg->DropDist == docParagraphStyles[xx].DropDist) &&
				(vg->FontEffect == docParagraphStyles[xx].FontEffect) &&
				(vg->FColor == docParagraphStyles[xx].FColor) &&
				(vg->FShade == docParagraphStyles[xx].FShade) &&
				(vg->SColor == docParagraphStyles[xx].SColor) &&
				(vg->SShade == docParagraphStyles[xx].SShade) &&
				(vg->BaseAdj == docParagraphStyles[xx].BaseAdj) &&
				(vg->txtShadowX == docParagraphStyles[xx].txtShadowX) &&
				(vg->txtShadowY == docParagraphStyles[xx].txtShadowY) &&
				(vg->txtOutline == docParagraphStyles[xx].txtOutline) &&
				(vg->txtUnderPos == docParagraphStyles[xx].txtUnderPos) &&
				(vg->txtUnderWidth == docParagraphStyles[xx].txtUnderWidth) &&
				(vg->txtStrikePos == docParagraphStyles[xx].txtStrikePos) &&
				(vg->txtStrikeWidth == docParagraphStyles[xx].txtStrikeWidth) &&
				(vg->scaleH == docParagraphStyles[xx].scaleH) &&
				(vg->scaleV == docParagraphStyles[xx].scaleV) &&
				(vg->baseOff == docParagraphStyles[xx].baseOff) &&
				(vg->kernVal == docParagraphStyles[xx].kernVal) &&
				(vg->FontSize == docParagraphStyles[xx].FontSize))
			{
				vg->Vname = docParagraphStyles[xx].Vname;
				fou = true;
				if (fl)
				{
					DoVorl[VorlC] = tmV.setNum(xx);
					VorlC++;
				}
				break;
			}
		}
	}
	if (!fou)
	{
		docParagraphStyles.append(*vg);
		if (fl)
		{
			DoVorl[VorlC] = tmV.setNum(docParagraphStyles.count()-1);
			VorlC++;
		}
	}
}

bool FileLoader::postLoad(bool is12doc)
{
/*	for (uint d = 0; d < ScMW->doc->MasterItems.count(); ++d)
	{
		PageItem *it = ScMW->doc->MasterItems.at(d);
		if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				ScText *hl = it->itemText.at(e);
				if ((hl->ch == QChar(25)) && ((int)hl->cembedded != -1))
					hl->cembedded = ScMW->doc->FrameItems.at((int)hl->cembedded);
				else
					hl->cembedded = 0;
			}
		}
	}
	for (uint d = 0; d < ScMW->doc->DocItems.count(); ++d)
	{
		PageItem *it = ScMW->doc->DocItems.at(d);
		if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				ScText *hl = it->itemText.at(e);
				if ((hl->ch == QChar(25)) && ((int)hl->cembedded != -1))
					hl->cembedded = ScMW->doc->FrameItems.at((int)hl->cembedded);
				else
					hl->cembedded = 0;
			}
		}
	} */
	if (ReplacedFonts.count() != 0)
	{
		if ((prefsManager->appPrefs.askBeforeSubstituite) || (newReplacement))
		{
			qApp->setOverrideCursor(QCursor(Qt::arrowCursor), true);
			FontReplaceDialog dia(0, &ReplacedFonts);
			if (dia.exec())
			{
				QMap<QString,QString>::Iterator itfsu;
				for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
				{
					if (dia.stickyReplacements->isChecked())
						prefsManager->appPrefs.GFontSub[itfsu.key()] = itfsu.data();
				}
			}
			else
			{
				return false;
			}
		}
		for (uint d = 0; d < ScMW->doc->MasterItems.count(); ++d)
		{
			PageItem *it = ScMW->doc->MasterItems.at(d);
			if ((!ScMW->doc->UsedFonts.contains(it->font())) && (!it->font().isEmpty()))
				it->setFont(ReplacedFonts[it->font()]);
			if ((it->asTextFrame()) || (it->asPathText()))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!ScMW->doc->UsedFonts.contains(it->itemText.at(e)->cfont->scName()))
					it->itemText.at(e)->cfont = (*ScMW->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->scName()]];
				}
			}
		}
		for (uint d = 0; d < ScMW->doc->DocItems.count(); ++d)
		{
			PageItem *it = ScMW->doc->DocItems.at(d);
			if ((!ScMW->doc->UsedFonts.contains(it->font())) && (!it->font().isEmpty()))
				it->setFont(ReplacedFonts[it->font()]);
			if ((it->asTextFrame()) || (it->asPathText()))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!ScMW->doc->UsedFonts.contains(it->itemText.at(e)->cfont->scName()))
					it->itemText.at(e)->cfont = (*ScMW->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->scName()]];
				}
			}
		}
		for (uint d = 0; d < ScMW->doc->FrameItems.count(); ++d)
		{
			PageItem *it = ScMW->doc->FrameItems.at(d);
			if ((!ScMW->doc->UsedFonts.contains(it->font())) && (!it->font().isEmpty()))
				it->setFont(ReplacedFonts[it->font()]);
			if ((it->asTextFrame()) || (it->asPathText()))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!ScMW->doc->UsedFonts.contains(it->itemText.at(e)->cfont->scName()))
					it->itemText.at(e)->cfont = (*ScMW->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->scName()]];
				}
			}
		}
		for (uint a = 0; a < ScMW->doc->docParagraphStyles.count(); ++a)
		{
			if ((!ScMW->doc->UsedFonts.contains(ScMW->doc->docParagraphStyles[a].Font)) && (!ScMW->doc->docParagraphStyles[a].Font.isEmpty()))
				ScMW->doc->docParagraphStyles[a].Font = ReplacedFonts[ScMW->doc->docParagraphStyles[a].Font];
		}
		QValueList<QString> tmpList;
		tmpList.clear();
		for (uint fe = 0; fe <  ScMW->doc->PDF_Options.EmbedList.count(); ++fe)
		{
			if (ReplacedFonts.contains(ScMW->doc->PDF_Options.EmbedList[fe]))
				tmpList.append(ReplacedFonts[ScMW->doc->PDF_Options.EmbedList[fe]]);
			else
				tmpList.append(ScMW->doc->PDF_Options.EmbedList[fe]);
		}
		ScMW->doc->PDF_Options.EmbedList = tmpList;
		tmpList.clear();
		for (uint fe = 0; fe <  ScMW->doc->PDF_Options.SubsetList.count(); ++fe)
		{
			if (ReplacedFonts.contains(ScMW->doc->PDF_Options.SubsetList[fe]))
				tmpList.append(ReplacedFonts[ScMW->doc->PDF_Options.SubsetList[fe]]);
			else
				tmpList.append(ScMW->doc->PDF_Options.SubsetList[fe]);
		}
		ScMW->doc->PDF_Options.SubsetList = tmpList;
		QMap<QString,QString>::Iterator itfsu;
		for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
		{
			if (!ScMW->doc->UsedFonts.contains(itfsu.data()))
			{
//				QFont fo = prefsManager->appPrefs.AvailFonts[itfsu.data()]->Font;
//				fo.setPointSize(qRound(ScMW->doc->toolSettings.defSize / 10.0));
				ScMW->doc->AddFont(itfsu.data(), qRound(ScMW->doc->toolSettings.defSize / 10.0));
			}
		}
		if (prefsManager->appPrefs.askBeforeSubstituite)
			ReplacedFonts.clear();
		dummyFois.clear();
	}
	
	//Calculate the canvas size
	if (!is12doc)
	{
		FPoint mincp, maxcp;
		ScMW->doc->canvasMinMax(mincp, maxcp);
		FPoint maximumCanvas(QMAX(maxcp.x(), maximumX), QMAX(maxcp.y(), maximumY));
		ScMW->view->adjustCanvas(mincp, maximumCanvas);
	}
	
	return true;
}

void FileLoader::informReplacementFonts()
{
	if (ReplacedFonts.count() != 0)
	{
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		QString mess = tr("Some fonts used by this document have been substituted:")+"\n\n";
		QMap<QString,QString>::Iterator it;
		for (it = ReplacedFonts.begin(); it != ReplacedFonts.end(); ++it)
		{
			mess += it.key() + tr(" was replaced by: ")+ it.data() +"\n";
		}
		QMessageBox::warning(ScMW, CommonStrings::trWarning, mess, 1, 0, 0);
	}
}
