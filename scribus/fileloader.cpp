#include <qdom.h>
#include <qprogressbar.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtl.h>
#include <qcursor.h>
#include <qregexp.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <qcheckbox.h>
#include <cstdlib>
#include <cmath>
#include <qtextstream.h>
#include "fileloader.h"
#include "scribus.h"
#include "scribusXml.h"
#include "missing.h"
#include "fontreplacedialog.h"
#include "units.h"
#include "pluginmanager.h"
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
extern double QStodouble(QString in);
extern int QStoInt(QString in);
extern QColor SetColor(ScribusDoc *currentDoc, QString color, int shad);
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);

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
	havePS = app->pluginManager->DLLexists(6);
	haveSVG = app->pluginManager->DLLexists(10);
	haveSXD = app->pluginManager->DLLexists(12);
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
	if (ff.endsWith(QChar(10)) || ff.endsWith(QChar(13)))
		ff.truncate(ff.length()-1);
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
	newReplacement = false;
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
	dummyFois.clear();
	dummyFois.setAutoDelete(true);
	switch (FileType)
	{
		case 0:
			{
				ScriXmlDoc *ss = new ScriXmlDoc();
				QObject::connect(ss, SIGNAL(NewPage(int)), app, SLOT(slotNewPage(int)));
				ss->ReplacedFonts.clear();
				ss->newReplacement = false;
				ss->dummyFois.clear();
				ret = ss->ReadDoc(FileName, app->Prefs.AvailFonts, app->doc, app->view, app->FProg);
				QObject::disconnect(ss, SIGNAL(NewPage(int)), app, SLOT(slotNewPage(int)));
				ReplacedFonts = ss->ReplacedFonts;
				newReplacement = ss->newReplacement;
				dummyFois = ss->dummyFois;
				delete ss;
			}
			break;
		case 1:
			ret = ReadDoc(app, FileName, app->Prefs.AvailFonts, app->doc, app->view, app->FProg);
			break;
		case 2:
			app->pluginManager->dllInput = FileName;
			app->pluginManager->callDLL( 6 );
			ret = true;
			break;
		case 3:
			app->pluginManager->dllInput = FileName;
			app->pluginManager->callDLL( 10 );
			ret = true;
			break;
		case 5:
			app->pluginManager->dllInput = FileName;
			app->pluginManager->callDLL( 12 );
			ret = true;
			break;
		default:
			ret = false;
			break;
	}
	if (ReplacedFonts.count() != 0)
	{
		if ((app->Prefs.askBeforeSubstituite) || (newReplacement))
		{
			qApp->setOverrideCursor(QCursor(Qt::arrowCursor), true);
			FontReplaceDialog *dia = new FontReplaceDialog(0, &app->Prefs, &ReplacedFonts);
			dia->exec();
			QMap<QString,QString>::Iterator itfsu;
			for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
			{
				if (dia->stickyReplacements->isChecked())
					app->Prefs.GFontSub[itfsu.key()] = itfsu.data();
			}
			delete dia;
		}
		for (uint d = 0; d < app->doc->MasterItems.count(); ++d)
		{
			PageItem *it = app->doc->MasterItems.at(d);
			if ((!app->doc->UsedFonts.contains(it->IFont)) && (it->IFont != ""))
				it->IFont = ReplacedFonts[it->IFont];
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
				if (!app->doc->UsedFonts.contains(it->itemText.at(e)->cfont->SCName))
					it->itemText.at(e)->cfont = (*app->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->SCName]];
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
				if (!app->doc->UsedFonts.contains(it->itemText.at(e)->cfont->SCName))
					it->itemText.at(e)->cfont = (*app->doc->AllFonts)[ReplacedFonts[it->itemText.at(e)->cfont->SCName]];
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
		for (uint fe = 0; fe <  app->doc->PDF_Options.EmbedList.count(); ++fe)
		{
			if (ReplacedFonts.contains(app->doc->PDF_Options.EmbedList[fe]))
				tmpList.append(ReplacedFonts[app->doc->PDF_Options.EmbedList[fe]]);
			else
				tmpList.append(app->doc->PDF_Options.EmbedList[fe]);
		}
		app->doc->PDF_Options.EmbedList = tmpList;
		tmpList.clear();
		for (uint fe = 0; fe <  app->doc->PDF_Options.SubsetList.count(); ++fe)
		{
			if (ReplacedFonts.contains(app->doc->PDF_Options.SubsetList[fe]))
				tmpList.append(ReplacedFonts[app->doc->PDF_Options.SubsetList[fe]]);
			else
				tmpList.append(app->doc->PDF_Options.SubsetList[fe]);
		}
		app->doc->PDF_Options.SubsetList = tmpList;
		QMap<QString,QString>::Iterator itfsu;
		for (itfsu = ReplacedFonts.begin(); itfsu != ReplacedFonts.end(); ++itfsu)
		{
			if (!app->doc->UsedFonts.contains(itfsu.data()))
			{
				QFont fo = app->Prefs.AvailFonts[itfsu.data()]->Font;
				fo.setPointSize(qRound(app->doc->toolSettings.defSize / 10.0));
				app->doc->AddFont(itfsu.data(), fo);
			}
		}
		if (app->Prefs.askBeforeSubstituite)
			ReplacedFonts.clear();
		dummyFois.clear();
	}
	app->pluginManager->dllInput = "";
	return ret;
}

bool FileLoader::ReadDoc(ScribusApp* app, QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, QProgressBar *dia2)
{
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
	int a;
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
		//CB Add this in to set this in the file in memory. Its saved, why not load it. 
		//Will of course be replaced by per page settings although we still probably need a document default
		doc->PageSize = dc.attribute("PAGESIZE");
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
		// FIXME A typo in early 1.3cvs (MAR 05) means we must support loading of
		// FIXME 'ScatchLeft' for a while too. This can be removed in a few months.
		if (dc.hasAttribute("ScatchLeft"))
			doc->ScratchLeft = QStodouble(dc.attribute("ScatchLeft", "100"));
		else
			doc->ScratchLeft = QStodouble(dc.attribute("ScratchLeft", "100"));
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
					{
						newReplacement = true;
						ReplacedFonts.insert(tmpf, view->Prefs->toolSettings.defFont);
					}
					else
						ReplacedFonts.insert(tmpf, view->Prefs->GFontSub[tmpf]);
				}
				else
				{
					if (!doc->UsedFonts.contains(tmpf))
					{
						QFont fo = avail[tmpf]->Font;
						fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
						doc->AddFont(tmpf, fo);
					}
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
			if(pg.tagName()=="PDF")
			{
				doc->PDF_Options.Articles = static_cast<bool>(QStoInt(pg.attribute("Articles")));
				doc->PDF_Options.Thumbnails = static_cast<bool>(QStoInt(pg.attribute("Thumbnails")));
				doc->PDF_Options.Compress = static_cast<bool>(QStoInt(pg.attribute("Compress")));
				doc->PDF_Options.CompressMethod = QStoInt(pg.attribute("CMethod","0"));
				doc->PDF_Options.Quality = QStoInt(pg.attribute("Quality","0"));
				doc->PDF_Options.RecalcPic = static_cast<bool>(QStoInt(pg.attribute("RecalcPic")));
				doc->PDF_Options.Bookmarks = static_cast<bool>(QStoInt(pg.attribute("Bookmarks")));
				if (pg.hasAttribute("MirrorH"))
					doc->PDF_Options.MirrorH = static_cast<bool>(QStoInt(pg.attribute("MirrorH")));
				else
					doc->PDF_Options.MirrorH = false;
				if (pg.hasAttribute("MirrorV"))
					doc->PDF_Options.MirrorV = static_cast<bool>(QStoInt(pg.attribute("MirrorV")));
				else
					doc->PDF_Options.MirrorV = false;
				if (pg.hasAttribute("RotateDeg"))
					doc->PDF_Options.RotateDeg = QStoInt(pg.attribute("RotateDeg","0"));
				else
					doc->PDF_Options.RotateDeg = 0;
				doc->PDF_Options.PresentMode = static_cast<bool>(QStoInt(pg.attribute("PresentMode")));
				doc->PDF_Options.PicRes = QStoInt(pg.attribute("PicRes"));
				doc->PDF_Options.Version = QStoInt(pg.attribute("Version"));
				doc->PDF_Options.Resolution = QStoInt(pg.attribute("Resolution"));
				doc->PDF_Options.Binding = QStoInt(pg.attribute("Binding"));
				doc->PDF_Options.Datei = "";
				doc->PDF_Options.isGrayscale = static_cast<bool>(QStoInt(pg.attribute("Grayscale","0")));
				doc->PDF_Options.UseRGB = static_cast<bool>(QStoInt(pg.attribute("RGBMode","0")));
				doc->PDF_Options.UseProfiles = static_cast<bool>(QStoInt(pg.attribute("UseProfiles","0")));
				doc->PDF_Options.UseProfiles2 = static_cast<bool>(QStoInt(pg.attribute("UseProfiles2","0")));
				doc->PDF_Options.Intent = QStoInt(pg.attribute("Intent","1"));
				doc->PDF_Options.Intent2 = QStoInt(pg.attribute("Intent2","1"));
				doc->PDF_Options.SolidProf = pg.attribute("SolidP", "");
				doc->PDF_Options.ImageProf = pg.attribute("ImageP", "");
				doc->PDF_Options.PrintProf = pg.attribute("PrintP", "");
				doc->PDF_Options.Info = pg.attribute("InfoString", "");
				doc->PDF_Options.BleedTop = QStodouble(pg.attribute("BTop","0"));
				doc->PDF_Options.BleedLeft = QStodouble(pg.attribute("BLeft","0"));
				doc->PDF_Options.BleedRight = QStodouble(pg.attribute("BRight","0"));
				doc->PDF_Options.BleedBottom = QStodouble(pg.attribute("BBottom","0"));
				doc->PDF_Options.EmbeddedI = static_cast<bool>(QStoInt(pg.attribute("ImagePr","0")));
				doc->PDF_Options.PassOwner = pg.attribute("PassOwner", "");
				doc->PDF_Options.PassUser = pg.attribute("PassUser", "");
				doc->PDF_Options.Permissions = QStoInt(pg.attribute("Permissions","-4"));
				doc->PDF_Options.Encrypt = static_cast<bool>(QStoInt(pg.attribute("Encrypt","0")));
				doc->PDF_Options.useLayers = static_cast<bool>(QStoInt(pg.attribute("UseLayers","0")));
				doc->PDF_Options.UseLPI = static_cast<bool>(QStoInt(pg.attribute("UseLpi","0")));
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
						ef.pageEffectDuration = QStoInt(pdfF.attribute("pageEffectDuration"));
						ef.pageViewDuration = QStoInt(pdfF.attribute("pageViewDuration"));
						ef.effectType = QStoInt(pdfF.attribute("effectType"));
						ef.Dm = QStoInt(pdfF.attribute("Dm"));
						ef.M = QStoInt(pdfF.attribute("M"));
						ef.Di = QStoInt(pdfF.attribute("Di"));
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
						tocsetup.style=tocElem.attribute("Style");
						doc->docToCSetups.append(tocsetup);
					}
					TOC = TOC.nextSibling();
				}
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
				if (doc->PageFP)
				{
					if (doc->MasterP)
					{
						if (Apage->LeftPg)
						{
							Apage->Margins.Right = QStodouble(pg.attribute("BORDERLEFT"));
							Apage->Margins.Left = QStodouble(pg.attribute("BORDERRIGHT"));
						}
						else
						{
							Apage->Margins.Left = QStodouble(pg.attribute("BORDERLEFT"));
							Apage->Margins.Right = QStodouble(pg.attribute("BORDERRIGHT"));
						}
					}
					else
					{
						if (a % 2 == 0)
						{
							if (doc->FirstPageLeft)
							{
								Apage->Margins.Right = QStodouble(pg.attribute("BORDERLEFT"));
								Apage->Margins.Left = QStodouble(pg.attribute("BORDERRIGHT"));
							}
							else
							{
								Apage->Margins.Left = QStodouble(pg.attribute("BORDERLEFT"));
								Apage->Margins.Right = QStodouble(pg.attribute("BORDERRIGHT"));
							}
						}
						else
						{
							if (doc->FirstPageLeft)
							{
								Apage->Margins.Left = QStodouble(pg.attribute("BORDERLEFT"));
								Apage->Margins.Right = QStodouble(pg.attribute("BORDERRIGHT"));
							}
							else
							{
								Apage->Margins.Right = QStodouble(pg.attribute("BORDERLEFT"));
								Apage->Margins.Left = QStodouble(pg.attribute("BORDERRIGHT"));
							}
						}
					}
				}
				else
				{
					Apage->Margins.Left = QStodouble(pg.attribute("BORDERLEFT"));
					Apage->Margins.Right = QStodouble(pg.attribute("BORDERRIGHT"));
				}
				doc->MasterP = false;
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
					if ((pg.attribute("OnMasterPage") != "") && (pg.tagName()=="MASTEROBJECT"))
						doc->currentPage = doc->MasterPages.at(doc->MasterNames[pg.attribute("OnMasterPage")]);
					if ((QStoInt(pg.attribute("NEXTITEM")) != -1) || (static_cast<bool>(QStoInt(pg.attribute("AUTOTEXT")))))
					{
						if (QStoInt(pg.attribute("BACKITEM")) == -1)
							LFrames.append(doc->Items.count());
					}
					int docGc = doc->GroupCounter;
					doc->GroupCounter = 0;
					Neu = PasteItem(&pg, doc, view);
					if (pg.tagName()=="PAGEOBJECT")
						Neu->OnMasterPage = "";
					doc->GroupCounter = docGc;
					tmpf = pg.attribute("IFONT", doc->toolSettings.defFont);
					if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
					{
						if ((!view->Prefs->GFontSub.contains(tmpf)) || (!avail[view->Prefs->GFontSub[tmpf]]->UseFont))
						{
							newReplacement = true;
							ReplacedFonts.insert(tmpf, view->Prefs->toolSettings.defFont);
						}
						else
							ReplacedFonts.insert(tmpf, view->Prefs->GFontSub[tmpf]);
					}
					else
					{
						if (!doc->UsedFonts.contains(tmpf))
						{
							QFont fo = avail[tmpf]->Font;
							fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
							doc->AddFont(tmpf, fo);
						}
					}
					Neu->IFont = tmpf;
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
							Neu->fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							GetItemText(&it, doc, view->Prefs, Neu);
						
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
					/* CB #1817 Remove this as its only a fix for old docs that will be loaded via the scribusXml.cpp loaded anyway
					//Leaving in here for the moment
					if ((Neu->itemType() == 5) && (Neu->Height != 0))
					{
						Neu->Rot += atan2(Neu->Height,Neu->Width)*(180.0/M_PI);
						Neu->Width = sqrt(pow(Neu->Width,2)+pow(Neu->Height,2));
						Neu->Height = 0;
						Neu->Clip.setPoints(4, -1,-1, static_cast<int>(Neu->Width+1),-1, static_cast<int>(Neu->Width+1),
											 static_cast<int>(Neu->Height+1), -1, static_cast<int>(Neu->Height+1));
					}
					*/
					Neu->Language = app->GetLang(pg.attribute("LANGUAGE", doc->Language));
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
//	view->reformPages();
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
	view->UN->setText(unitGetStrFromIndex(doc->docUnitIndex));
	dia2->setProgress(DOC.childNodes().count());
	return true;
}

void FileLoader::GetItemText(QDomElement *it, ScribusDoc *doc, ApplicationPrefs *Prefs, PageItem* obj)
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
	if ((!Prefs->AvailFonts.find(tmpf)) || (!Prefs->AvailFonts[tmpf]->UseFont))
	{
		bool isThere = false;
		for (uint dl = 0; dl < dummyFois.count(); ++dl)
		{
			if (dummyFois.at(dl)->SCName == tmpf)
			{
				isThere = true;
				dummy = dummyFois.at(dl);
				break;
			}
		}
		if (!isThere)
		{
			dummy = new Foi(tmpf, "", false);
			dummyFois.append(dummy);
		}
		unknown = true;
		if ((!Prefs->GFontSub.contains(tmpf)) || (!Prefs->AvailFonts[Prefs->GFontSub[tmpf]]->UseFont))
		{
			newReplacement = true;
			ReplacedFonts.insert(tmpf, Prefs->toolSettings.defFont);
		}
		else
			ReplacedFonts.insert(tmpf, Prefs->GFontSub[tmpf]);
	}
	else
	{
		if (!doc->UsedFonts.contains(tmpf))
		{
			QFont fo = Prefs->AvailFonts[tmpf]->Font;
			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			doc->AddFont(tmpf, fo);
		}
	}
	int size = qRound(QStodouble(it->attribute("CSIZE")) * 10);
	QString fcolor = it->attribute("CCOLOR");
	double extra = QStodouble(it->attribute("CEXTRA"));
	int shade = QStoInt(it->attribute("CSHADE"));
	int style = QStoInt(it->attribute("CSTYLE"));
	int ab = QStoInt(it->attribute("CAB","0"));
	QString stroke = it->attribute("CSTROKE","None");
	int shade2 = QStoInt(it->attribute("CSHADE2","100"));
	int scale = QStoInt(it->attribute("CSCALE","100"));
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
		hg->cab = ab;
		hg->cstroke = stroke;
		hg->cshade2 = shade2;
		hg->cscale = QMIN(QMAX(scale, 25), 400);
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		obj->itemText.append(hg);
	}
	return;
}

PageItem* FileLoader::PasteItem(QDomElement *obj, ScribusDoc *doc, ScribusView *view)
{
	int z = 0;
	PageItem::ItemType pt = static_cast<PageItem::ItemType>(QStoInt(obj->attribute("PTYPE")));
	double x = QStodouble(obj->attribute("XPOS"));
	double y = QStodouble(obj->attribute("YPOS"));
	double w = QStodouble(obj->attribute("WIDTH"));
	double h = QStodouble(obj->attribute("HEIGHT"));
	double pw = QStodouble(obj->attribute("PWIDTH"));
	double scx = QStodouble(obj->attribute("LOCALSCX"));
	double scy = QStodouble(obj->attribute("LOCALSCY"));
	QString Pcolor = obj->attribute("PCOLOR");
	QString Pcolor2 = obj->attribute("PCOLOR2");
	QColor tmpc;
	PageItem* b;
	QString tmp;
	int xi;
	double xf, yf;
	switch (pt)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::FrameType1:
		z = view->PaintEllipse(x, y, w, h, pw, Pcolor, Pcolor2);
		b = doc->Items.at(z);
		break;
	//
	case PageItem::ImageFrame:
		z = view->PaintPict(x, y, w, h);
		b = doc->Items.at(z);
		b->LocalScX = scx;
		b->LocalScY = scy;
		b->LocalX = QStodouble(obj->attribute("LOCALX"));
		b->LocalY = QStodouble(obj->attribute("LOCALY"));
		b->Pfile = obj->attribute("PFILE");
		b->IProfile = obj->attribute("PRFILE","");
		b->EmProfile = obj->attribute("EPROF","");
		b->IRender = QStoInt(obj->attribute("IRENDER","1"));
		b->UseEmbedded = QStoInt(obj->attribute("EMBEDDED","1"));
		if (b->Pfile != "")
			view->LoadPict(b->Pfile, z);
		b->LocalScX = scx;
		b->LocalScY = scy;
		b->PicArt = QStoInt(obj->attribute("PICART"));
		b->BBoxX = QStodouble(obj->attribute("BBOXX"));
		b->BBoxH = QStodouble(obj->attribute("BBOXH"));
		b->ScaleType = QStoInt(obj->attribute("SCALETYPE","1"));
		b->AspectRatio = QStoInt(obj->attribute("RATIO","0"));
		b->Pwidth = pw;
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::FrameType3:
		z = view->PaintRect(x, y, w, h, pw, Pcolor, Pcolor2);
		b = doc->Items.at(z);
		break;
	//
	case PageItem::PathText:
	case PageItem::TextFrame:
		z = view->PaintText(x, y, w, h, pw, Pcolor);
		b = doc->Items.at(z);
		if ((QStoInt(obj->attribute("ANNOTATION","0"))) && (static_cast<bool>(QStoInt(obj->attribute("ANICON","0")))))
		{
			b->LocalScX = scx;
			b->LocalScY = scy;
			b->LocalX = QStodouble(obj->attribute("LOCALX"));
			b->LocalY = QStodouble(obj->attribute("LOCALY"));
			b->Pfile = obj->attribute("PFILE");
			b->Pfile2 = obj->attribute("PFILE2","");
			b->Pfile3 = obj->attribute("PFILE3","");
			b->IProfile = obj->attribute("PRFILE","");
			b->EmProfile = obj->attribute("EPROF","");
			b->IRender = QStoInt(obj->attribute("IRENDER","1"));
			b->UseEmbedded = QStoInt(obj->attribute("EMBEDDED","1"));
			view->LoadPict(b->Pfile, z);
			b->LocalScX = scx;
			b->LocalScY = scy;
			b->PicArt = QStoInt(obj->attribute("PICART"));
			b->BBoxX = QStodouble(obj->attribute("BBOXX"));
			b->BBoxH = QStodouble(obj->attribute("BBOXH"));
			b->ScaleType = QStoInt(obj->attribute("SCALETYPE","1"));
			b->AspectRatio = QStoInt(obj->attribute("RATIO","0"));
		}
		b->LineSp = QStodouble(obj->attribute("LINESP"));
		b->convertTo(pt);
		break;
	case PageItem::Line:
		z = view->PaintLine(x, y, w, h, pw, Pcolor2);
		b = doc->Items.at(z);
		break;
	case PageItem::Polygon:
		z = view->PaintPoly(x, y, w, h, pw, Pcolor, Pcolor2);
		b = doc->Items.at(z);
		break;
	case PageItem::PolyLine:
		z = view->PaintPolyLine(x, y, w, h, pw, Pcolor, Pcolor2);
		b = doc->Items.at(z);
		break;
	}
	b->FrameType = QStoInt(obj->attribute("FRTYPE", "0"));
	b->startArrowIndex =  QStoInt(obj->attribute("startArrowIndex","0"));
	b->endArrowIndex =  QStoInt(obj->attribute("endArrowIndex","0"));
	b->NamedLStyle = obj->attribute("NAMEDLST", "");
	b->isBookmark = QStoInt(obj->attribute("BOOKMARK"));
	if ((b->isBookmark) && (doc->BookMarks.count() == 0))
		doc->OldBM = true;
	b->BMnr = QStoInt(obj->attribute("BookNr","0"));
	b->textAlignment = QStoInt(obj->attribute("ALIGN","0"));
	b->setImageFlippedH(QStoInt(obj->attribute("FLIPPEDH")));
	b->setImageFlippedV(QStoInt(obj->attribute("FLIPPEDV")));
	b->RadRect = QStodouble(obj->attribute("RADRECT","0"));
	b->ClipEdited = QStoInt(obj->attribute("CLIPEDIT", "0"));
	b->setFillColor(Pcolor);
	b->setLineColor(Pcolor2);
	b->setFillShade(QStoInt(obj->attribute("SHADE")));
	b->setLineShade(QStoInt(obj->attribute("SHADE2")));
	b->TxtStroke = obj->attribute("TXTSTROKE", "None");
	b->TxtFill = obj->attribute("TXTFILL", "Black");
	b->ShTxtStroke = QStoInt(obj->attribute("TXTSTRSH", "100"));
	b->ShTxtFill = QStoInt(obj->attribute("TXTFILLSH", "100"));
	b->TxtScale = QStoInt(obj->attribute("TXTSCALE", "100"));
	b->TxTStyle = QStoInt(obj->attribute("TXTSTYLE", "0"));
	b->Rot = QStodouble(obj->attribute("ROT"));
	b->Extra = QStodouble(obj->attribute("EXTRA"));
	b->TExtra = QStodouble(obj->attribute("TEXTRA", "1"));
	b->BExtra = QStodouble(obj->attribute("BEXTRA", "1"));
	b->RExtra = QStodouble(obj->attribute("REXTRA", "1"));
	b->PLineArt = Qt::PenStyle(QStoInt(obj->attribute("PLINEART")));
	b->PLineEnd = Qt::PenCapStyle(QStoInt(obj->attribute("PLINEEND","0")));
	b->PLineJoin = Qt::PenJoinStyle(QStoInt(obj->attribute("PLINEJOIN","0")));
	b->isPrintable = QStoInt(obj->attribute("PRINTABLE"));
	b->isAnnotation = QStoInt(obj->attribute("ANNOTATION","0"));
	b->AnType = QStoInt(obj->attribute("ANTYPE","0"));
	QString AnName = obj->attribute("ANNAME","");
	if (AnName != "")
	{
		if (b->itemName() == AnName)
			b->AutoName = true;
		else
		{
			b->setItemName(AnName);
			b->AutoName = false;
		}
	}
	b->AnAction = obj->attribute("ANACTION","");
	b->An_E_act = obj->attribute("ANEACT","");
	b->An_X_act = obj->attribute("ANXACT","");
	b->An_D_act = obj->attribute("ANDACT","");
	b->An_Fo_act = obj->attribute("ANFOACT","");
	b->An_Bl_act = obj->attribute("ANBLACT","");
	b->An_K_act = obj->attribute("ANKACT","");
	b->An_F_act = obj->attribute("ANFACT","");
	b->An_V_act = obj->attribute("ANVACT","");
	b->An_C_act = obj->attribute("ANCACT","");
	b->AnActType = QStoInt(obj->attribute("ANACTYP","0"));
	b->An_Extern = obj->attribute("ANEXTERN","");
	if ((b->An_Extern != "") && (b->AnActType != 8))
	{
		QFileInfo efp(b->An_Extern);
		b->An_Extern = efp.absFilePath();
	}
	b->AnZiel = QStoInt(obj->attribute("ANZIEL","0"));
	b->AnToolTip = obj->attribute("ANTOOLTIP","");
	b->AnRollOver = obj->attribute("ANROLL","");
	b->AnDown = obj->attribute("ANDOWN","");
	b->AnBwid = QStoInt(obj->attribute("ANBWID","1"));
	b->AnBsty = QStoInt(obj->attribute("ANBSTY","0"));
	b->AnFeed = QStoInt(obj->attribute("ANFEED","1"));
	b->AnFlag = QStoInt(obj->attribute("ANFLAG","0"));
	b->AnFont = QStoInt(obj->attribute("ANFONT","4"));
	b->AnFormat = QStoInt(obj->attribute("ANFORMAT","0"));
	b->AnVis = QStoInt(obj->attribute("ANVIS","0"));
	b->AnIsChk = static_cast<bool>(QStoInt(obj->attribute("ANCHK","0")));
	b->AnAAact = static_cast<bool>(QStoInt(obj->attribute("ANAA","0")));
	b->AnHTML = static_cast<bool>(QStoInt(obj->attribute("ANHTML","0")));
	b->AnUseIcons = static_cast<bool>(QStoInt(obj->attribute("ANICON","0")));
	b->AnChkStil = QStoInt(obj->attribute("ANCHKS","0"));
	b->AnMaxChar = QStoInt(obj->attribute("ANMC","-1"));
	b->AnBColor = obj->attribute("ANBCOL","None");
	b->AnIPlace = QStoInt(obj->attribute("ANPLACE","1"));
	b->AnScaleW = QStoInt(obj->attribute("ANSCALE","0"));
	b->TopLine = static_cast<bool>(QStoInt(obj->attribute("TopLine","0")));
	b->LeftLine = static_cast<bool>(QStoInt(obj->attribute("LeftLine","0")));
	b->RightLine = static_cast<bool>(QStoInt(obj->attribute("RightLine","0")));
	b->BottomLine = static_cast<bool>(QStoInt(obj->attribute("BottomLine","0")));
	b->isTableItem = static_cast<bool>(QStoInt(obj->attribute("isTableItem","0")));
	b->TopLinkID =  QStoInt(obj->attribute("TopLINK","-1"));
	b->LeftLinkID =  QStoInt(obj->attribute("LeftLINK","-1"));
	b->RightLinkID =  QStoInt(obj->attribute("RightLINK","-1"));
	b->BottomLinkID =  QStoInt(obj->attribute("BottomLINK","-1"));
	b->PoShow = QStoInt(obj->attribute("PLTSHOW","0"));
	b->BaseOffs = QStodouble(obj->attribute("BASEOF","0"));
	b->setTextFlowsAroundFrame(QStoInt(obj->attribute("TEXTFLOW")));
	b->setTextFlowUsesBoundingBox(QStoInt(obj->attribute("TEXTFLOW2","0")));
	b->ISize = qRound(QStodouble(obj->attribute("ISIZE","12")) * 10);
	b->ExtraV=QStodouble(obj->attribute("EXTRAV","0"));
	b->DashOffset = QStodouble(obj->attribute("DASHOFF","0.0"));
	b->Reverse = static_cast<bool>(QStoInt(obj->attribute("REVERS","0")));
	b->InvPict = static_cast<bool>(QStoInt(obj->attribute("INVERS","0")));
	b->setLocked(static_cast<bool>(QStoInt(obj->attribute("LOCK","0"))));
	b->setSizeLocked(static_cast<bool>(QStoInt(obj->attribute("LOCKR","0"))));
	b->setFillTransparency(QStodouble(obj->attribute("TransValue","0.0")));
	if (obj->hasAttribute("TransValueS"))
		b->setLineTransparency(QStodouble(obj->attribute("TransValueS","0.0")));
	else
		b->setLineTransparency(QStodouble(obj->attribute("TransValue","0.0")));
	if (QStoInt(obj->attribute("TRANSPARENT","0")) == 1)
		b->setFillColor("None");
	b->Cols = QStoInt(obj->attribute("COLUMNS","1"));
	b->ColGap = QStodouble(obj->attribute("COLGAP","0.0"));
	if (QStoInt(obj->attribute("LAYER","0")) != -1)
		b->LayerNr = QStoInt(obj->attribute("LAYER","0"));
	b->setTextFlowUsesContourLine(QStoInt(obj->attribute("TEXTFLOW3","0")));
	tmp = "";
	if ((obj->hasAttribute("GROUPS")) && (QStoInt(obj->attribute("NUMGROUP","0")) != 0))
	{
		tmp = obj->attribute("GROUPS");
		QTextStream fg(&tmp, IO_ReadOnly);
		b->Groups.clear();
		for (int cx = 0; cx < QStoInt(obj->attribute("NUMGROUP","0")); ++cx)
		{
			fg >> xi;
			b->Groups.push(xi);
		}
		tmp = "";
	}
	else
		b->Groups.clear();
	tmp = "";
	if ((obj->hasAttribute("NUMTAB")) && (QStoInt(obj->attribute("NUMTAB","0")) != 0))
	{
		tmp = obj->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		b->TabValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMTAB","0")); ++cxv)
		{
			tgv >> xf;
			b->TabValues.append(xf);
		}
		tmp = "";
	}
	else
		b->TabValues.clear();
	if ((obj->hasAttribute("NUMDASH")) && (QStoInt(obj->attribute("NUMDASH","0")) != 0))
	{
		tmp = obj->attribute("DASHS");
		QTextStream dgv(&tmp, IO_ReadOnly);
		b->DashValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMDASH","0")); ++cxv)
		{
			dgv >> xf;
			b->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		b->DashValues.clear();
	tmp = "";
	if (obj->hasAttribute("NUMPO"))
	{
		b->PoLine.resize(obj->attribute("NUMPO").toUInt());
		tmp = obj->attribute("POCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMPO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			b->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		b->PoLine.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMCO"))
	{
		b->ContourLine.resize(obj->attribute("NUMCO").toUInt());
		tmp = obj->attribute("COCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMCO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			b->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		b->ContourLine = b->PoLine.copy();
	if (b->itemType() != PageItem::Line)
		b->Clip = FlattenPath(b->PoLine, b->Segments);
	else
	{
		int ph = static_cast<int>(QMAX(1.0, b->Pwidth / 2.0));
		b->Segments.clear();
		b->PoLine.resize(0);
		b->Clip.setPoints(4, -ph,-ph, static_cast<int>(b->Width+ph),-ph,
		                  static_cast<int>(b->Width+ph),static_cast<int>(b->Height+ph),
		                  -ph,static_cast<int>(b->Height+ph));
		b->Height = 1;
	}
	if (b->itemType() == PageItem::ImageFrame)
	{
		view->AdjustPictScale(b);
		view->AdjustPreview(b, false);
	}
	if ((b->itemType() != PageItem::TextFrame) && (b->itemType() != PageItem::PathText))
		b->IFont = doc->toolSettings.defFont;
	b->GrType = QStoInt(obj->attribute("GRTYP","0"));
	QString GrColor;
	QString GrColor2;
	int GrShade;
	int GrShade2;
	if (b->GrType != 0)
	{
		b->GrStartX = QStodouble(obj->attribute("GRSTARTX","0.0"));
		b->GrStartY = QStodouble(obj->attribute("GRSTARTY","0.0"));
		b->GrEndX = QStodouble(obj->attribute("GRENDX","0.0"));
		b->GrEndY = QStodouble(obj->attribute("GRENDY","0.0"));
		GrColor = obj->attribute("GRCOLOR","");
		if (GrColor != "")
		{
			GrColor2 = obj->attribute("GRCOLOR2","");
			GrShade = QStoInt(obj->attribute("GRSHADE","100"));
			GrShade2 = QStoInt(obj->attribute("GRSHADE2","100"));
		}
	}
	if (b->GrType != 0)
	{
		b->fill_gradient.clearStops();
		if ((GrColor != "") && (GrColor2 != ""))
		{
			if (b->GrType == 5)
			{
				if ((GrColor != "None") && (GrColor != ""))
					b->SetFarbe(&tmpc, GrColor, GrShade);
				b->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor, GrShade);
				if ((GrColor2 != "None") && (GrColor2 != ""))
					b->SetFarbe(&tmpc, GrColor2, GrShade2);
				b->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor2, GrShade2);
			}
			else
			{
				if ((GrColor2 != "None") && (GrColor2 != ""))
					b->SetFarbe(&tmpc, GrColor2, GrShade2);
				b->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor2, GrShade2);
				if ((GrColor != "None") && (GrColor != ""))
					b->SetFarbe(&tmpc, GrColor, GrShade);
				b->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor, GrShade);
			}
		}
		view->updateGradientVectors(b);
	}
	view->setRedrawBounding(b);
	b->OwnPage = view->OnPage(b);
	return b;
}
