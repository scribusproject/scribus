#include <qfile.h>
#include <qfileinfo.h>
#include <qtl.h>
#include <qcursor.h>
#include <qregexp.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <cstdlib>
#include <cmath>
#include <qtextstream.h>
#include "fileloader.h"
#include "scribus.h"
#include "scribusXml.h"
#include "missing.h"
#include "fontreplacedialog.h"
#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif
#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
extern bool loadText(QString nam, QString *Buffer);
extern void GetItemProps(bool newVersion, QDomElement *obj, struct CopyPasteBuffer *OB);
extern double QStodouble(QString in);
extern int QStoInt(QString in);
extern QColor SetColor(ScribusDoc *currentDoc, QString color, int shad);

/*!
 \fn FileLoader::FileLoader(QString fileName, ScribusApp* app)
 \author Franz Schmid
 \date
 \brief Constructor, sets the variable "FileName" to the input parameter fileName
 \param QString fileName
 \retval None
 */
FileLoader::FileLoader(QString fileName, ScribusApp* app)
{
	FileName = fileName;
	FileType = -1;
	havePS = app->DLLexists(6);
	haveSVG = app->DLLexists(10);
	haveSXD = app->DLLexists(12);
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
	else if (((ext.endsWith("ps")) || (ext.endsWith("eps"))) && (havePS))
		ret = 2;
	else if (((ext.endsWith("svg")) || (ext.endsWith("svgz"))) && (haveSVG))
		ret = 3;
	else if ((ext.endsWith("sxd")) && (haveSXD))
		ret = 5;
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
	QString fText = ReadDatei(FileName);
	if (fText == "")
		return ret;
	if ((fText.startsWith("<SCRIBUSUTF8NEW")) && (fText.contains("<PAGE ", TRUE) != 0))
		return 1;
	if ((fText.startsWith("<SCRIBUS")) && (fText.contains("<PAGE ", TRUE) != 0))
		return 0;
	return ret;
}

/*!
 \fn QString FileLoader::ReadDatei(QString fileName)
 \author Franz Schmid
 \date
 \brief Auxiliary function loads the file "fileName" into a QString and does conversion from UTF8 if required
 \param QString fileName
 \retval QString Contents of the file, converted from UTF8
 */
QString FileLoader::ReadDatei(QString fileName)
{
/**
  * added to support gz docs
  * 2.7.2002 C.Toepp
  * <c.toepp@gmx.de>
  */
	QString f = "";
#ifdef HAVE_LIBZ
	if(fileName.right(2) == "gz")
	{
		gzFile gzDoc;
		char buff[4097];
		int i;
		gzDoc = gzopen(fileName.latin1(),"rb");
		if(gzDoc == NULL)
			return "";
		while((i = gzread(gzDoc,&buff,4096)) > 0)
		{
			buff[i] = '\0';
			f.append(buff);
		}
		gzclose(gzDoc);
	}
	else
		loadText(fileName, &f);
#else
	loadText(fileName, &f);
#endif
	QString ff = "";
	if (f.startsWith("<SCRIBUSUTF8"))
		ff = QString::fromUtf8(f);
	else if (f.startsWith("<SCRIBUS"))
		ff = f;
	return ff;
/** end changes */
}

/*!
 \fn bool FileLoader::LoadFile(ScribusApp* app)
 \author Franz Schmid
 \date
 \brief Loads the file "FileName" as a Scribus document
 \param ScribusApp* app, pointer to the main application class
 \retval bool true when loading is succsessful, false otherwise
 */
bool FileLoader::LoadFile(ScribusApp* app)
{
	bool ret = false;
	app->doc->guidesSettings.marginsShown = app->Prefs.guidesSettings.marginsShown;
	app->doc->guidesSettings.framesShown = app->Prefs.guidesSettings.framesShown;
	app->doc->guidesSettings.gridShown = app->Prefs.guidesSettings.gridShown;
	app->doc->guidesSettings.guidesShown = app->Prefs.guidesSettings.guidesShown;
	app->doc->guidesSettings.baseShown = app->Prefs.guidesSettings.baseShown;
	app->doc->guidesSettings.linkShown = app->Prefs.guidesSettings.linkShown;
	app->doc->toolSettings.polyC = app->Prefs.toolSettings.polyC;
	app->doc->toolSettings.polyF = app->Prefs.toolSettings.polyF;
	app->doc->toolSettings.polyR = app->Prefs.toolSettings.polyR;
	app->doc->toolSettings.polyFd = app->Prefs.toolSettings.polyFd;
	app->doc->toolSettings.polyS = app->Prefs.toolSettings.polyS;
	app->doc->AutoSave = app->Prefs.AutoSave;
	app->doc->AutoSaveTime = app->Prefs.AutoSaveTime;
	ReplacedFonts.clear();
	switch (FileType)
	{
		case 0:
			{
				ScriXmlDoc *ss = new ScriXmlDoc();
				QObject::connect(ss, SIGNAL(NewPage(int)), app, SLOT(slotNewPage(int)));
				ss->ReplacedFonts.clear();
				ret = ss->ReadDoc(FileName, app->Prefs.AvailFonts, app->doc, app->view, app->FProg);
				QObject::disconnect(ss, SIGNAL(NewPage(int)), app, SLOT(slotNewPage(int)));
				ReplacedFonts = ss->ReplacedFonts;
				delete ss;
			}
			break;
		case 1:
			ret = ReadDoc(app, FileName, app->Prefs.AvailFonts, app->doc, app->view, app->FProg);
			break;
		case 2:
			app->DLLinput = FileName;
			app->CallDLL( 6 );
			ret = true;
			break;
		case 3:
			app->DLLinput = FileName;
			app->CallDLL( 10 );
			ret = true;
			break;
		case 5:
			app->DLLinput = FileName;
			app->CallDLL( 12 );
			ret = true;
			break;
		default:
			ret = false;
			break;
	}
	if (ReplacedFonts.count() != 0)
	{
		qApp->setOverrideCursor(QCursor(Qt::arrowCursor), true);
		FontReplaceDialog *dia = new FontReplaceDialog(0, &app->Prefs, &ReplacedFonts);
		dia->exec();
		for (uint d = 0; d < app->doc->MasterItems.count(); ++d)
		{
			PageItem *it = app->doc->MasterItems.at(d);
			if ((!app->doc->UsedFonts.contains(it->IFont)) && (it->IFont != ""))
				it->IFont = ReplacedFonts[it->IFont];
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!app->doc->UsedFonts.contains(it->itemText.at(e)->cfont))
					it->itemText.at(e)->cfont = ReplacedFonts[it->itemText.at(e)->cfont];
				}
			}
		}
		for (uint d = 0; d < app->doc->DocItems.count(); ++d)
		{
			PageItem *it = app->doc->DocItems.at(d);
			if ((!app->doc->UsedFonts.contains(it->IFont)) && (it->IFont != ""))
				it->IFont = ReplacedFonts[it->IFont];
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!app->doc->UsedFonts.contains(it->itemText.at(e)->cfont))
					it->itemText.at(e)->cfont = ReplacedFonts[it->itemText.at(e)->cfont];
				}
			}
		}
		for (uint a = 0; a < app->doc->docParagraphStyles.count(); ++a)
		{
			if ((!app->doc->UsedFonts.contains(app->doc->docParagraphStyles[a].Font)) && (app->doc->docParagraphStyles[a].Font != ""))
				app->doc->docParagraphStyles[a].Font = ReplacedFonts[app->doc->docParagraphStyles[a].Font];
		}
		QValueList<QString> tmpList;
		tmpList.clear();
		for (uint fe = 0; fe <  app->doc->PDF_Optionen.EmbedList.count(); ++fe)
		{
			if (ReplacedFonts.contains(app->doc->PDF_Optionen.EmbedList[fe]))
				tmpList.append(ReplacedFonts[app->doc->PDF_Optionen.EmbedList[fe]]);
			else
				tmpList.append(app->doc->PDF_Optionen.EmbedList[fe]);
		}
		app->doc->PDF_Optionen.EmbedList = tmpList;
		tmpList.clear();
		for (uint fe = 0; fe <  app->doc->PDF_Optionen.SubsetList.count(); ++fe)
		{
			if (ReplacedFonts.contains(app->doc->PDF_Optionen.SubsetList[fe]))
				tmpList.append(ReplacedFonts[app->doc->PDF_Optionen.SubsetList[fe]]);
			else
				tmpList.append(app->doc->PDF_Optionen.SubsetList[fe]);
		}
		app->doc->PDF_Optionen.SubsetList = tmpList;
		QMap<QString,QString>::Iterator itfsu;
		for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
		{
			QFont fo = app->Prefs.AvailFonts[itfsu.data()]->Font;
			fo.setPointSize(qRound(app->doc->toolSettings.defSize / 10.0));
			app->doc->AddFont(itfsu.data(), fo);
			if (dia->stickyReplacements->isChecked())
				app->Prefs.GFontSub[itfsu.key()] = itfsu.data();
		}
		delete dia;
	}
	app->DLLinput = "";
	return ret;
}

bool FileLoader::ReadDoc(ScribusApp* app, QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, QProgressBar *dia2)
{
	struct CopyPasteBuffer OB;
	struct ParagraphStyle vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	int counter, Pgc;
	bool AtFl;
	bool newVersion = false;
	QString tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QFont fo;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int x, a;
	double xf;
	PageItem *Neu;
	Page* Apage;
	LFrames.clear();
	QDomDocument docu("scridoc");
	QString f = "";
	f = ReadDatei(fileName);
	/* 2004/10/02 - petr vanek - bug #1092 - missing <PAGE> crash Scribus. The check constraint moved into IsScribus()
	FIXME: I've add test on containig tag PAGE but returning FALSE freezes S. in scribus.cpp need some hack too...  */
	if (!docu.setContent(f))
		return false;
	doc->PageColors.clear();	
	doc->Layers.clear();
	CMYKColor lf = CMYKColor();
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
		doc->PageOri = QStoInt(dc.attribute("ORIENTATION","0"));
		doc->FirstPnum = QStoInt(dc.attribute("FIRSTNUM","1"));
		doc->PageFP=QStoInt(dc.attribute("BOOK", "0"));
		doc->FirstPageLeft=QStoInt(dc.attribute("FIRSTLEFT","0"));
		doc->PageAT=QStoInt(dc.attribute("AUTOTEXT"));
		doc->PageSp=QStoInt(dc.attribute("AUTOSPALTEN"));
		doc->PageSpa=QStodouble(dc.attribute("ABSTSPALTEN"));
		doc->docUnitIndex = QStoInt(dc.attribute("UNITS","0"));
		doc->toolSettings.defSize=qRound(QStodouble(dc.attribute("DSIZE")) * 10);
		Defont=dc.attribute("DFONT");
		if ((!avail.find(Defont)) || (!avail[Defont]->UseFont))
		{
			ReplacedFonts.insert(Defont, view->Prefs->toolSettings.defFont);
			Defont = view->Prefs->toolSettings.defFont;
		}
		else
		{
			QFont fo = avail[Defont]->Font;
			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			doc->AddFont(Defont, fo);
		}
		doc->toolSettings.defFont = Defont;
		doc->toolSettings.dCols=QStoInt(dc.attribute("DCOL", "1"));
		doc->toolSettings.dGap=QStodouble(dc.attribute("DGAP", "0.0"));
		doc->DocAutor=dc.attribute("AUTHOR");
		doc->DocComments=dc.attribute("COMMENTS");
		doc->DocKeyWords=dc.attribute("KEYWORDS","");
		doc->DocTitel=dc.attribute("TITLE");
		doc->DocPublisher = dc.attribute("PUBLISHER", "");
		doc->DocDate = dc.attribute("DOCDATE", "");
		doc->DocType = dc.attribute("DOCTYPE", "");
		doc->DocFormat = dc.attribute("DOCFORMAT", "");
		doc->DocIdent = dc.attribute("DOCIDENT", "");
		doc->DocSource = dc.attribute("DOCSOURCE", "");
		doc->DocLangInfo = dc.attribute("DOCLANGINFO", "");
		doc->DocRelation = dc.attribute("DOCRELATION", "");
		doc->DocCover = dc.attribute("DOCCOVER", "");
		doc->DocRights = dc.attribute("DOCRIGHTS", "");
		doc->DocContrib = dc.attribute("DOCCONTRIB", "");
		doc->typographicSetttings.valueSuperScript = QStoInt(dc.attribute("VHOCH"));
		doc->typographicSetttings.scalingSuperScript = QStoInt(dc.attribute("VHOCHSC"));
		doc->typographicSetttings.valueSubScript = QStoInt(dc.attribute("VTIEF"));
		doc->typographicSetttings.scalingSubScript = QStoInt(dc.attribute("VTIEFSC"));
		doc->typographicSetttings.valueSmallCaps = QStoInt(dc.attribute("VKAPIT"));
		doc->typographicSetttings.valueBaseGrid = QStodouble(dc.attribute("BASEGRID", "12"));
		doc->typographicSetttings.offsetBaseGrid = QStodouble(dc.attribute("BASEO", "0"));
		doc->typographicSetttings.autoLineSpacing = QStoInt(dc.attribute("AUTOL","20"));
		doc->GroupCounter=QStoInt(dc.attribute("GROUPC","1"));
		doc->HasCMS = static_cast<bool>(QStoInt(dc.attribute("HCMS","0")));
		doc->CMSSettings.SoftProofOn = static_cast<bool>(QStoInt(dc.attribute("DPSo","0")));
		doc->CMSSettings.CMSinUse = static_cast<bool>(QStoInt(dc.attribute("DPuse","0")));
		doc->CMSSettings.GamutCheck = static_cast<bool>(QStoInt(dc.attribute("DPgam","0")));
		doc->CMSSettings.BlackPoint = static_cast<bool>(QStoInt(dc.attribute("DPbla","1")));
		doc->CMSSettings.DefaultMonitorProfile = dc.attribute("DPMo","");
		doc->CMSSettings.DefaultPrinterProfile = dc.attribute("DPPr","");
		doc->CMSSettings.DefaultInputProfile = dc.attribute("DPIn","");
		doc->CMSSettings.DefaultInputProfile2 = dc.attribute("DPIn2","");
		doc->CMSSettings.DefaultIntentPrinter = QStoInt(dc.attribute("DIPr","0"));
		doc->CMSSettings.DefaultIntentMonitor = QStoInt(dc.attribute("DIMo","1"));
		doc->CMSSettings.DefaultIntentMonitor2 = QStoInt(dc.attribute("DIMo2","1"));
		doc->ActiveLayer = QStoInt(dc.attribute("ALAYER","0"));
		doc->Language = dc.attribute("LANGUAGE", "");
		doc->MinWordLen = QStoInt(dc.attribute("MINWORDLEN", "3"));
		doc->HyCount = QStoInt(dc.attribute("HYCOUNT", "2"));
		doc->PageB=QStodouble(dc.attribute("PAGEWITH"));
		doc->PageH=QStodouble(dc.attribute("PAGEHEIGHT"));
		doc->PageM.Left=QStodouble(dc.attribute("BORDERLEFT"));
		doc->PageM.Right=QStodouble(dc.attribute("BORDERRIGHT"));
		doc->PageM.Top=QStodouble(dc.attribute("BORDERTOP"));
		doc->PageM.Bottom=QStodouble(dc.attribute("BORDERBOTTOM"));
		doc->Automatic = static_cast<bool>(QStoInt(dc.attribute("AUTOMATIC", "1")));
		doc->AutoCheck = static_cast<bool>(QStoInt(dc.attribute("AUTOCHECK", "0")));
		doc->GuideLock = static_cast<bool>(QStoInt(dc.attribute("GUIDELOCK", "0")));
		doc->guidesSettings.minorGrid = QStodouble(dc.attribute("MINGRID", tmp.setNum(view->Prefs->guidesSettings.minorGrid)));
		doc->guidesSettings.majorGrid = QStodouble(dc.attribute("MAJGRID", tmp.setNum(view->Prefs->guidesSettings.majorGrid)));
		doc->guidesSettings.gridShown = static_cast<bool>(QStoInt(dc.attribute("SHOWGRID", "0")));
		doc->guidesSettings.guidesShown = static_cast<bool>(QStoInt(dc.attribute("SHOWGUIDES", "1")));
		doc->guidesSettings.framesShown = static_cast<bool>(QStoInt(dc.attribute("SHOWFRAME", "1")));
		doc->guidesSettings.marginsShown = static_cast<bool>(QStoInt(dc.attribute("SHOWMARGIN", "1")));
		doc->guidesSettings.baseShown = static_cast<bool>(QStoInt(dc.attribute("SHOWBASE", "0")));
		doc->guidesSettings.showPic = static_cast<bool>(QStoInt(dc.attribute("SHOWPICT", "1")));
		doc->guidesSettings.linkShown = static_cast<bool>(QStoInt(dc.attribute("SHOWLINK", "0")));
		doc->toolSettings.polyC = QStoInt(dc.attribute("POLYC", "4"));
		doc->toolSettings.polyF = QStodouble(dc.attribute("POLYF", "0.5"));
		doc->toolSettings.polyR = QStodouble(dc.attribute("POLYR", "0"));
		doc->toolSettings.polyFd = QStoInt(dc.attribute("POLYFD", "0"));
		doc->toolSettings.polyS = static_cast<bool>(QStoInt(dc.attribute("POLYS", "0")));
		doc->AutoSave = static_cast<bool>(QStoInt(dc.attribute("AutoSave","0")));
		doc->AutoSaveTime = QStoInt(dc.attribute("AutoSaveTime","600000"));
		doc->ScratchBottom = QStodouble(dc.attribute("ScratchBottom", "20"));
		doc->ScratchLeft = QStodouble(dc.attribute("ScatchLeft", "100"));
		doc->ScratchRight = QStodouble(dc.attribute("ScratchRight", "100"));
		doc->ScratchTop = QStodouble(dc.attribute("ScratchTop", "20"));
		doc->toolSettings.dStartArrow = QStoInt(dc.attribute("StartArrow", "0"));
		doc->toolSettings.dEndArrow = QStoInt(dc.attribute("EndArrow", "0"));
		doc->toolSettings.scaleX = QStodouble(dc.attribute("PICTSCX","1"));
		doc->toolSettings.scaleY = QStodouble(dc.attribute("PICTSCY","1"));
		doc->toolSettings.scaleType = static_cast<bool>(QStoInt(dc.attribute("PSCALE", "1")));
		doc->toolSettings.aspectRatio = static_cast<bool>(QStoInt(dc.attribute("PASPECT", "0")));
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
		doc->toolSettings.dLineArt = static_cast<Qt::PenStyle>(QStoInt(dc.attribute("STIL")));
		doc->toolSettings.dLstyleLine = static_cast<Qt::PenStyle>(QStoInt(dc.attribute("STILLINE")));
		doc->toolSettings.dWidth = QStodouble(dc.attribute("WIDTH", "1"));
		doc->toolSettings.dWidthLine = QStodouble(dc.attribute("WIDTHLINE", "1"));
		doc->toolSettings.dShade2 = QStoInt(dc.attribute("PENSHADE", "100"));
		doc->toolSettings.dShadeLine = QStoInt(dc.attribute("LINESHADE", "100"));
		doc->toolSettings.dShade = QStoInt(dc.attribute("BRUSHSHADE", "100"));
		doc->toolSettings.magMin = QStoInt(dc.attribute("MAGMIN","10"));
		doc->toolSettings.magMax = QStoInt(dc.attribute("MAGMAX","3200"));
		doc->toolSettings.magStep = QStoInt(dc.attribute("MAGSTEP","25"));
		if (dc.hasAttribute("CPICT"))
			doc->toolSettings.dBrushPict = dc.attribute("CPICT");
		doc->toolSettings.shadePict = QStoInt(dc.attribute("PICTSHADE","100"));
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
		doc->marginColored = static_cast<bool>(QStoInt(dc.attribute("RANDF","0")));
		doc->guidesSettings.before = static_cast<bool>(QStoInt(dc.attribute("BACKG","1")));
		doc->guidesSettings.guideRad = QStoInt(dc.attribute("GuideRad","10"));
		doc->guidesSettings.grabRad = QStoInt(dc.attribute("GRAB","4"));
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
			if (pg.tagName()=="CheckProfile")
			{
				struct checkerPrefs checkerSettings;
				checkerSettings.ignoreErrors = static_cast<bool>(QStoInt(pg.attribute("ignoreErrors", "0")));
				checkerSettings.autoCheck = static_cast<bool>(QStoInt(pg.attribute("autoCheck", "1")));
				checkerSettings.checkGlyphs = static_cast<bool>(QStoInt(pg.attribute("checkGlyphs", "1")));
				checkerSettings.checkOrphans = static_cast<bool>(QStoInt(pg.attribute("checkOrphans", "1")));
				checkerSettings.checkOverflow = static_cast<bool>(QStoInt(pg.attribute("checkOverflow", "1")));
				checkerSettings.checkPictures = static_cast<bool>(QStoInt(pg.attribute("checkPictures", "1")));
				checkerSettings.checkResolution = static_cast<bool>(QStoInt(pg.attribute("checkResolution", "1")));
				checkerSettings.checkTransparency = static_cast<bool>(QStoInt(pg.attribute("checkTransparency", "1")));
				checkerSettings.minResolution = QStodouble(pg.attribute("minResolution","72"));
				checkerSettings.checkAnnotations = static_cast<bool>(QStoInt(pg.attribute("checkAnnotations", "0")));
				checkerSettings.checkRasterPDF = static_cast<bool>(QStoInt(pg.attribute("checkRasterPDF", "1")));
				doc->checkerProfiles[pg.attribute("Name")] = checkerSettings;
			}
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!="None")
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
			  doc->PageColors[pg.attribute("NAME")] = lf;
			}
			if(pg.tagName()=="STYLE")
			{
				vg.Vname = pg.attribute("NAME");
				vg.LineSpa = QStodouble(pg.attribute("LINESP"));
				vg.Indent = QStodouble(pg.attribute("INDENT","0"));
				vg.First = QStodouble(pg.attribute("FIRST","0"));
				vg.textAlignment = QStoInt(pg.attribute("ALIGN"));
				vg.gapBefore = QStodouble(pg.attribute("VOR","0"));
				vg.gapAfter = QStodouble(pg.attribute("NACH","0"));
				tmpf = pg.attribute("FONT", doc->toolSettings.defFont);
				if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
				{
					if ((!view->Prefs->GFontSub.contains(tmpf)) || (!avail[view->Prefs->GFontSub[tmpf]]->UseFont))
						ReplacedFonts.insert(tmpf, view->Prefs->toolSettings.defFont);
					else
						ReplacedFonts.insert(tmpf, view->Prefs->GFontSub[tmpf]);
				}
				else
				{
					QFont fo = avail[tmpf]->Font;
					fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
					doc->AddFont(tmpf, fo);
				}
				vg.Font = tmpf;
				vg.FontSize = qRound(QStodouble(pg.attribute("FONTSIZE","12")) * 10.0);
				vg.Drop = static_cast<bool>(QStoInt(pg.attribute("DROP","0")));
				vg.DropLin = QStoInt(pg.attribute("DROPLIN","2"));
				vg.FontEffect = QStoInt(pg.attribute("EFFECT","0"));
				vg.FColor = pg.attribute("FCOLOR", doc->toolSettings.dBrush);
				vg.FShade = QStoInt(pg.attribute("FSHADE","100"));
				vg.SColor = pg.attribute("SCOLOR", doc->toolSettings.dPen);
				vg.SShade = QStoInt(pg.attribute("SSHADE","100"));
				vg.BaseAdj = static_cast<bool>(QStoInt(pg.attribute("BASE","0")));
				if ((pg.hasAttribute("NUMTAB")) && (QStoInt(pg.attribute("NUMTAB","0")) != 0))
				{
					tmp = pg.attribute("TABS");
					QTextStream tgv(&tmp, IO_ReadOnly);
					vg.TabValues.clear();
					for (int cxv = 0; cxv < QStoInt(pg.attribute("NUMTAB","0")); ++cxv)
					{
						tgv >> xf;
						vg.TabValues.append(xf);
					}
					tmp = "";
				}
				else
					vg.TabValues.clear();
				doc->docParagraphStyles.append(vg);
			}
			if(pg.tagName()=="JAVA")
				doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = QStoInt(pg.attribute("NUMMER"));
				la.Level = QStoInt(pg.attribute("LEVEL"));
				la.Name = pg.attribute("NAME");
				la.isViewable = QStoInt(pg.attribute("SICHTBAR"));
				la.isPrintable = QStoInt(pg.attribute("DRUCKEN"));
				doc->Layers.append(la);
			}
			if(pg.tagName()=="Bookmark")
			{
				bok.Title = pg.attribute("Title");
				bok.Text = pg.attribute("Text");
				bok.Aktion = pg.attribute("Aktion");
				bok.ItemNr = QStoInt(pg.attribute("ItemNr"));
				bok.Seite = QStoInt(pg.attribute("Seite"));
				bok.Element = QStoInt(pg.attribute("Element"));
				bok.First = QStoInt(pg.attribute("First"));
				bok.Last = QStoInt(pg.attribute("Last"));
				bok.Prev = QStoInt(pg.attribute("Prev"));
				bok.Next = QStoInt(pg.attribute("Next"));
				bok.Parent = QStoInt(pg.attribute("Parent"));
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
					sl.Dash = QStoInt(MuL.attribute("Dash"));
					sl.LineEnd = QStoInt(MuL.attribute("LineEnd"));
					sl.LineJoin = QStoInt(MuL.attribute("LineJoin"));
					sl.Shade = QStoInt(MuL.attribute("Shade"));
					sl.Width = QStodouble(MuL.attribute("Width"));
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
			if ((pg.tagName()=="PAGE") || (pg.tagName()=="MASTERPAGE"))
			{
				a = QStoInt(pg.attribute("NUM"));
				PgNam = "";
				PgNam = pg.attribute("NAM", "");
				Pgc = doc->PageC;
				AtFl = doc->PageAT;
				if (PgNam == "")
				{
					doc->PageC = Pgc;
					doc->Pages = doc->DocPages;
					doc->PageAT = AtFl;
					doc->MasterP = false;
				}
				else
				{
					doc->PageC = 0;
					doc->PageAT = false;
					doc->Pages = doc->MasterPages;
					doc->MasterP = true;
				}
				app->slotNewPage(a);
				Apage = doc->Pages.at(a);
				if (PgNam == "")
					doc->DocPages = doc->Pages;
				else
				{
				  Apage->setPageName(PgNam);
					doc->MasterNames[PgNam] = a;
					doc->MasterPages = doc->Pages;
				}
				doc->MasterP = false;
				doc->PageC = Pgc+1;
				doc->PageAT = AtFl;
				Apage->LeftPg=QStoInt(pg.attribute("LEFT","0"));
				QString Mus = "";
				Mus = pg.attribute("MNAM","Normal");
				if (!doc->MasterP)
					Apage->MPageNam = Mus;
				else
					Apage->MPageNam = "";
				Apage->Xoffset = QStodouble(pg.attribute("PAGEXPOS"));
				Apage->Yoffset = QStodouble(pg.attribute("PAGEYPOS"));
				Apage->Width = QStodouble(pg.attribute("PAGEWITH"));
				Apage->Height = QStodouble(pg.attribute("PAGEHEIGHT"));
				Apage->Margins.Left = QStodouble(pg.attribute("BORDERLEFT"));
				Apage->Margins.Right = QStodouble(pg.attribute("BORDERRIGHT"));
				Apage->Margins.Top = QStodouble(pg.attribute("BORDERTOP"));
				Apage->Margins.Bottom = QStodouble(pg.attribute("BORDERBOTTOM"));
				doc->PageB = Apage->Width;
				doc->PageH = Apage->Height;
				doc->PageM.Left = Apage->Margins.Left;
				doc->PageM.Right = Apage->Margins.Right;
				doc->PageM.Top = Apage->Margins.Top;
				doc->PageM.Bottom = Apage->Margins.Bottom;
				if ((pg.hasAttribute("NumVGuides")) && (QStoInt(pg.attribute("NumVGuides","0")) != 0))
				{
					tmp = pg.attribute("VerticalGuides");
					QTextStream fgv(&tmp, IO_ReadOnly);
					Apage->YGuides.clear();
					for (int cxv = 0; cxv < QStoInt(pg.attribute("NumVGuides","0")); ++cxv)
					{
						fgv >> xf;
						Apage->YGuides.append(xf);
					}
					qHeapSort(Apage->YGuides);
					tmp = "";
				}
				else
					Apage->YGuides.clear();
				if ((pg.hasAttribute("NumHGuides")) && (QStoInt(pg.attribute("NumHGuides","0")) != 0))
				{
					tmp = pg.attribute("HorizontalGuides");
					QTextStream fgh(&tmp, IO_ReadOnly);
					Apage->XGuides.clear();
					for (int cxh = 0; cxh < QStoInt(pg.attribute("NumHGuides","0")); ++cxh)
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
			if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="MASTEROBJECT"))
			{
					if (pg.tagName()=="PAGEOBJECT")
					{
						doc->Items = doc->DocItems;
						doc->Pages = doc->DocPages;
						doc->MasterP = false;
					}
					else
					{
						doc->Items = doc->MasterItems;
						doc->Pages = doc->MasterPages;
						doc->MasterP = true;
					}
					if (pg.attribute("OnMasterPage") != "")
						doc->currentPage = doc->MasterPages.at(doc->MasterNames[pg.attribute("OnMasterPage")]);
					if ((QStoInt(pg.attribute("NEXTITEM")) != -1) || (static_cast<bool>(QStoInt(pg.attribute("AUTOTEXT")))))
					{
						if (QStoInt(pg.attribute("BACKITEM")) == -1)
							LFrames.append(doc->Items.count());
					}
					GetItemProps(newVersion, &pg, &OB);
					OB.Xpos = QStodouble(pg.attribute("XPOS"));
					OB.Ypos=QStodouble(pg.attribute("YPOS"));
					OB.startArrowIndex =  QStoInt(pg.attribute("startArrowIndex","0"));
					OB.endArrowIndex =  QStoInt(pg.attribute("endArrowIndex","0"));
					OB.NamedLStyle = pg.attribute("NAMEDLST", "");
					OB.isBookmark=QStoInt(pg.attribute("BOOKMARK"));
					if ((OB.isBookmark) && (doc->BookMarks.count() == 0))
						doc->OldBM = true;
					OB.BMnr = QStoInt(pg.attribute("BookNr","0"));
					OB.textAlignment = QStoInt(pg.attribute("ALIGN","0"));
					tmpf = pg.attribute("IFONT", doc->toolSettings.defFont);
					if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
					{
						if ((!view->Prefs->GFontSub.contains(tmpf)) || (!avail[view->Prefs->GFontSub[tmpf]]->UseFont))
							ReplacedFonts.insert(tmpf, view->Prefs->toolSettings.defFont);
						else
							ReplacedFonts.insert(tmpf, view->Prefs->GFontSub[tmpf]);
					}
					else
					{
						QFont fo = avail[tmpf]->Font;
						fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
						doc->AddFont(tmpf, fo);
					}
					OB.IFont = tmpf;
					OB.LayerNr = QStoInt(pg.attribute("LAYER","0"));
					OB.Language = pg.attribute("LANGUAGE", doc->Language);
					tmp = "";
					if ((pg.hasAttribute("GROUPS")) && (QStoInt(pg.attribute("NUMGROUP","0")) != 0))
					{
						tmp = pg.attribute("GROUPS");
						QTextStream fg(&tmp, IO_ReadOnly);
						OB.Groups.clear();
						for (int cx = 0; cx < QStoInt(pg.attribute("NUMGROUP","0")); ++cx)
						{
							fg >> x;
							OB.Groups.push(x);
						}
						tmp = "";
					}
					else
						OB.Groups.clear();
					QDomNode IT=pg.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = QStodouble(it.attribute("RAMP","0.0"));
							int shade = QStoInt(it.attribute("SHADE","100"));
							double opa = QStodouble(it.attribute("TRANS","1"));
							OB.fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							tmp += GetItemText(&it, doc, view->Prefs);
						IT=IT.nextSibling();
					}
					OB.itemText = tmp;
					int docGc = doc->GroupCounter;
					doc->GroupCounter = 0;
					if ((OB.PType == 5) && (OB.Height != 0))
					{
						OB.Rot += atan2(OB.Height,OB.Width)*(180.0/3.1415927);
						OB.Width = sqrt(pow(OB.Width,2)+pow(OB.Height,2));
						OB.Height = 0;
						OB.Clip.setPoints(4, -1,-1, static_cast<int>(OB.Width+1),-1, static_cast<int>(OB.Width+1),
											 static_cast<int>(OB.Height+1), -1, static_cast<int>(OB.Height+1));
					}
					uint last = doc->Items.count();
					view->PasteItem(&OB, true);
					doc->GroupCounter = docGc;
					Neu = doc->Items.at(last);
					Neu->isAutoText=static_cast<bool>(QStoInt(pg.attribute("AUTOTEXT")));
					if (Neu->isAutoText)
						doc->LastAuto = Neu;
					Neu->NextIt = QStoInt(pg.attribute("NEXTITEM"));
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(QStoInt(pg.attribute("OwnLINK","0")), Neu->ItemNr);
					}
					if (pg.tagName()=="PAGEOBJECT")
					{
						doc->DocItems = doc->Items;
						doc->DocPages = doc->Pages;
					}
					else
					{
						doc->MasterItems = doc->Items;
						doc->MasterPages = doc->Pages;
					}
					doc->MasterP = false;
					counter++;
				}
			PAGE=PAGE.nextSibling();
		}
		PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="PDF")
			{
				doc->PDF_Optionen.Articles = static_cast<bool>(QStoInt(pg.attribute("Articles")));
				doc->PDF_Optionen.Thumbnails = static_cast<bool>(QStoInt(pg.attribute("Thumbnails")));
				doc->PDF_Optionen.Compress = static_cast<bool>(QStoInt(pg.attribute("Compress")));
				doc->PDF_Optionen.CompressMethod = QStoInt(pg.attribute("CMethod","0"));
				doc->PDF_Optionen.Quality = QStoInt(pg.attribute("Quality","0"));
				doc->PDF_Optionen.RecalcPic = static_cast<bool>(QStoInt(pg.attribute("RecalcPic")));
				doc->PDF_Optionen.Bookmarks = static_cast<bool>(QStoInt(pg.attribute("Bookmarks")));
				if (pg.hasAttribute("MirrorH"))
					doc->PDF_Optionen.MirrorH = static_cast<bool>(QStoInt(pg.attribute("MirrorH")));
				else
					doc->PDF_Optionen.MirrorH = false;
				if (pg.hasAttribute("MirrorV"))
					doc->PDF_Optionen.MirrorV = static_cast<bool>(QStoInt(pg.attribute("MirrorV")));
				else
					doc->PDF_Optionen.MirrorV = false;
				if (pg.hasAttribute("RotateDeg"))
					doc->PDF_Optionen.RotateDeg = QStoInt(pg.attribute("RotateDeg","0"));
				else
					doc->PDF_Optionen.RotateDeg = 0;
				doc->PDF_Optionen.PresentMode = static_cast<bool>(QStoInt(pg.attribute("PresentMode")));
				doc->PDF_Optionen.PicRes = QStoInt(pg.attribute("PicRes"));
				doc->PDF_Optionen.Version = QStoInt(pg.attribute("Version"));
				doc->PDF_Optionen.Resolution = QStoInt(pg.attribute("Resolution"));
				doc->PDF_Optionen.Binding = QStoInt(pg.attribute("Binding"));
				doc->PDF_Optionen.Datei = "";
				doc->PDF_Optionen.UseRGB = static_cast<bool>(QStoInt(pg.attribute("RGBMode","0")));
				doc->PDF_Optionen.UseProfiles = static_cast<bool>(QStoInt(pg.attribute("UseProfiles","0")));
				doc->PDF_Optionen.UseProfiles2 = static_cast<bool>(QStoInt(pg.attribute("UseProfiles2","0")));
				doc->PDF_Optionen.Intent = QStoInt(pg.attribute("Intent","1"));
				doc->PDF_Optionen.Intent2 = QStoInt(pg.attribute("Intent2","1"));
				doc->PDF_Optionen.SolidProf = pg.attribute("SolidP", "");
				doc->PDF_Optionen.ImageProf = pg.attribute("ImageP", "");
				doc->PDF_Optionen.PrintProf = pg.attribute("PrintP", "");
				doc->PDF_Optionen.Info = pg.attribute("InfoString", "");
				doc->PDF_Optionen.BleedTop = QStodouble(pg.attribute("BTop","0"));
				doc->PDF_Optionen.BleedLeft = QStodouble(pg.attribute("BLeft","0"));
				doc->PDF_Optionen.BleedRight = QStodouble(pg.attribute("BRight","0"));
				doc->PDF_Optionen.BleedBottom = QStodouble(pg.attribute("BBottom","0"));
				doc->PDF_Optionen.EmbeddedI = static_cast<bool>(QStoInt(pg.attribute("ImagePr","0")));
				doc->PDF_Optionen.PassOwner = pg.attribute("PassOwner", "");
				doc->PDF_Optionen.PassUser = pg.attribute("PassUser", "");
				doc->PDF_Optionen.Permissions = QStoInt(pg.attribute("Permissions","-4"));
				doc->PDF_Optionen.Encrypt = static_cast<bool>(QStoInt(pg.attribute("Encrypt","0")));
				doc->PDF_Optionen.UseLPI = static_cast<bool>(QStoInt(pg.attribute("UseLpi","0")));
				QDomNode PFO = PAGE.firstChild();
				while(!PFO.isNull())
				{
					QDomElement pdfF = PFO.toElement();
					if(pdfF.tagName() == "LPI")
					{
						struct LPIData lpo;
						lpo.Angle = QStoInt(pdfF.attribute("Angle"));
						lpo.Frequency = QStoInt(pdfF.attribute("Frequency"));
						lpo.SpotFunc = QStoInt(pdfF.attribute("SpotFunction"));
						doc->PDF_Optionen.LPISettings[pdfF.attribute("Color")] = lpo;
					}
					if(pdfF.tagName() == "Fonts")
					{
						if (!doc->PDF_Optionen.EmbedList.contains(pdfF.attribute("Name")))
							doc->PDF_Optionen.EmbedList.append(pdfF.attribute("Name"));
					}
					if(pdfF.tagName() == "Subset")
					{
						if (!doc->PDF_Optionen.SubsetList.contains(pdfF.attribute("Name")))
							doc->PDF_Optionen.SubsetList.append(pdfF.attribute("Name"));
					}
					if(pdfF.tagName() == "Effekte")
					{
    					struct PDFPresentationData ef;
    					ef.pageEffectDuration = QStoInt(pdfF.attribute("pageEffectDuration"));
    					ef.pageViewDuration = QStoInt(pdfF.attribute("pageViewDuration"));
    					ef.effectType = QStoInt(pdfF.attribute("effectType"));
    					ef.Dm = QStoInt(pdfF.attribute("Dm"));
    					ef.M = QStoInt(pdfF.attribute("M"));
		    			ef.Di = QStoInt(pdfF.attribute("Di"));
						doc->PDF_Optionen.PresentVals.append(ef);
					}
					PFO = PFO.nextSibling();
				}
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
				ta->TopLink = doc->Items.at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = doc->Items.at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = doc->Items.at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = doc->Items.at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	doc->Pages = doc->DocPages;
	doc->PageC = doc->Pages.count();
	doc->Items = doc->DocItems;
	doc->MasterP = false;
	view->reformPages();
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
			Its = doc->Items.at((*lc));
			Itr = Its;
			Its->BackBox = 0;
			if (Its->isAutoText)
				doc->FirstAuto = Its;
			while (Its->NextIt != -1)
			{
				Itn = doc->Items.at(Its->NextIt);
				Its->NextBox = Itn;
				Itn->BackBox = Its;
				Its = Itn;
			}
			Its->NextBox = 0;
		}
	}
	switch (doc->docUnitIndex)
	{
	case 0:
		view->UN->setText( QObject::tr("pt"));
		break;
	case 1:
		view->UN->setText( QObject::tr("mm"));
		break;
	case 2:
		view->UN->setText( QObject::tr("in"));
		break;
	case 3:
		view->UN->setText( QObject::tr("p"));
		break;
	}
	dia2->setProgress(DOC.childNodes().count());
	return true;
}

QString FileLoader::GetItemText(QDomElement *it, ScribusDoc *doc, ApplicationPrefs *Prefs)
{
	QString tmp2, tmf, tmpf, tmp3, tmp;
	tmp = "";
	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(5));
	tmp2.replace(QRegExp("\n"), QChar(5));
	tmp2.replace(QRegExp("\t"), QChar(4));
	tmpf = it->attribute("CFONT", doc->toolSettings.defFont);
	if ((!Prefs->AvailFonts.find(tmpf)) || (!Prefs->AvailFonts[tmpf]->UseFont))
	{
		if ((!Prefs->GFontSub.contains(tmpf)) || (!Prefs->AvailFonts[Prefs->GFontSub[tmpf]]->UseFont))
			ReplacedFonts.insert(tmpf, Prefs->toolSettings.defFont);
		else
			ReplacedFonts.insert(tmpf, Prefs->GFontSub[tmpf]);
	}
	else
	{
		QFont fo = Prefs->AvailFonts[tmpf]->Font;
		fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
		doc->AddFont(tmpf, fo);
	}
	tmp3 = "\t" + tmpf + "\t";
	tmp3 += it->attribute("CSIZE") + "\t";
	tmp3 += it->attribute("CCOLOR") + "\t";
	tmp3 += it->attribute("CEXTRA") + "\t";
	tmp3 += it->attribute("CSHADE") + "\t";
	tmp3 += it->attribute("CSTYLE") + "\t";
	tmp3 += it->attribute("CAB","0") + "\t";
	tmp3 += it->attribute("CSTROKE","None") + "\t";
	tmp3 += it->attribute("CSHADE2","100") + "\t";
	tmp3 += it->attribute("CSCALE","100") + "\n";
	for (uint cxx=0; cxx<tmp2.length(); ++cxx)
		tmp += tmp2.at(cxx)+tmp3;
	return tmp;
}
