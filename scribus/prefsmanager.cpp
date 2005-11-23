/***************************************************************************
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <qcolor.h>
#include <qdir.h>
#include <qdom.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qstylefactory.h>
#include <qwmatrix.h>

#include "prefsmanager.h"
#include "prefsmanager.moc"
#include "filewatcher.h"
#include "missing.h"
#include "keymanager.h"
#include "pagesize.h"
#include "pdfoptions.h"
#include "prefsfile.h"
#include "scfonts.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusapp.h"
#include "util.h"
#include "werktoolb.h"


extern ScribusQApp* ScQApp;
extern bool emergencyActivated;

PrefsManager* PrefsManager::_instance = 0;

PrefsManager::PrefsManager(QObject *parent, const char *name)
 : QObject(parent, name)
{
}

PrefsManager::~PrefsManager()
{
	appPrefs.AvailFonts.~SCFonts();
	delete prefsFile;
}

PrefsManager* PrefsManager::instance()
{
    if (_instance == 0)
        _instance = new PrefsManager();

    return _instance;
}

void PrefsManager::deleteInstance()
{
    if (_instance)
        delete _instance;
    _instance = 0;
}


ApplicationPrefs* PrefsManager::applicationPrefs()
{
	return &appPrefs;
}

PrefsFile* PrefsManager::applicationPrefsFile()
{
	return prefsFile;
}

const bool PrefsManager::importingFrom12x()
{
	return importingFrom12;
}


void PrefsManager::setup()
{
	setupPreferencesLocation();

	importingFrom12=copy12Preferences();
	prefsFile = new PrefsFile(QDir::convertSeparators(prefsLocation + "/prefs13.xml"));
	if (importingFrom12)
		convert12Preferences();
	//<<CB TODO Reset keyboard shortcuts of all 1.3 users as too many
	//     have conflicts if they dont nuke their settings.
	// - Remove for 1.3.0 release: importingFrom12=true;
	//>>CB
}

void PrefsManager::initDefaults()
{
	/** Default font and size **/
	SCFontsIterator it(appPrefs.AvailFonts);
	appPrefs.toolSettings.defFont = it.currentKey();
	appPrefs.toolSettings.defSize = 120;

	/** Default colours **/
	appPrefs.DColors.clear();
	QString pfadC = ScPaths::instance().libDir();
	QString pfadC2 = pfadC + "rgbscribus.txt";
	QFile fiC(pfadC2);
	if (!fiC.exists())
	{
		appPrefs.DColors.insert("White", ScColor(0, 0, 0, 0));
		appPrefs.DColors.insert("Black", ScColor(0, 0, 0, 255));
		appPrefs.DColors.insert("Blue", ScColor(255, 255, 0, 0));
		appPrefs.DColors.insert("Cyan", ScColor(255, 0, 0, 0));
		appPrefs.DColors.insert("Green", ScColor(255, 0, 255, 0));
		appPrefs.DColors.insert("Red", ScColor(0, 255, 255, 0));
		appPrefs.DColors.insert("Yellow", ScColor(0, 0, 255, 0));
		appPrefs.DColors.insert("Magenta", ScColor(0, 255, 0, 0));
		appPrefs.DColorSet = "Scribus-Small";
	}
	else
	{
		if (fiC.open(IO_ReadOnly))
		{
			QString ColorEn, Cname;
			int Rval, Gval, Bval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			while (!tsC.atEnd())
			{
				ColorEn = tsC.readLine();
				QTextStream CoE(&ColorEn, IO_ReadOnly);
				CoE >> Rval;
				CoE >> Gval;
				CoE >> Bval;
				CoE >> Cname;
				ScColor tmp;
				tmp.setColorRGB(Rval, Gval, Bval);
				appPrefs.DColors.insert(Cname, tmp);
			}
			fiC.close();
		}
		appPrefs.DColorSet = "X11 RGB-Set";
	}

	appPrefs.Wheelval = 40;
	/** Set Default window position and size to sane default values which should work on every screen */
	appPrefs.mainWinSettings.xPosition = 0;
	appPrefs.mainWinSettings.yPosition = 0;
	appPrefs.mainWinSettings.width = 640;
	appPrefs.mainWinSettings.height = 480;
	appPrefs.guidesSettings.marginsShown = true;
	appPrefs.guidesSettings.framesShown = true;
	appPrefs.guidesSettings.gridShown = false;
	appPrefs.guidesSettings.guidesShown = false;
	appPrefs.guidesSettings.baseShown = false;
	appPrefs.guidesSettings.showPic = true;
	appPrefs.guidesSettings.showControls = false;
	appPrefs.guidesSettings.linkShown = false;
	appPrefs.guidesSettings.rulerMode = true;
	appPrefs.guidesSettings.grabRad = 4;
	appPrefs.guidesSettings.guideRad = 10;
	appPrefs.guidesSettings.minorGrid = 20;
	appPrefs.guidesSettings.majorGrid = 100;
	appPrefs.guidesSettings.minorColor = QColor(green);
	appPrefs.guidesSettings.majorColor = QColor(green);
	appPrefs.guidesSettings.margColor = QColor(blue);
	appPrefs.guidesSettings.guideColor = QColor(darkBlue);
	appPrefs.guidesSettings.baseColor = QColor(lightGray);
	appPrefs.typographicSettings.valueSuperScript = 33;
	appPrefs.typographicSettings.scalingSuperScript = 100;
	appPrefs.typographicSettings.valueSubScript = 33;
	appPrefs.typographicSettings.scalingSubScript = 100;
	appPrefs.typographicSettings.valueSmallCaps = 75;
	appPrefs.typographicSettings.autoLineSpacing = 20;
	appPrefs.typographicSettings.valueUnderlinePos = -1;
	appPrefs.typographicSettings.valueUnderlineWidth = -1;
	appPrefs.typographicSettings.valueStrikeThruPos = -1;
	appPrefs.typographicSettings.valueStrikeThruWidth = -1;
	appPrefs.typographicSettings.valueBaseGrid = 14.4;
	appPrefs.typographicSettings.offsetBaseGrid = 0.0;
	appPrefs.GUI = "Default";
	//FIXME
	//Black here causes issues when a colour set is loaded without "Black" in it.
	//"Black" is created with wrong values. Eg SVG colour set
	appPrefs.toolSettings.dPen = "Black";
	appPrefs.toolSettings.dBrush = "Black";
	appPrefs.toolSettings.dShade = 100;
	appPrefs.toolSettings.dShade2 = 100;
	appPrefs.toolSettings.dLineArt = SolidLine;
	appPrefs.toolSettings.dWidth = 1;
	appPrefs.toolSettings.dPenLine = "Black";
	appPrefs.toolSettings.dPenText = "Black";
	appPrefs.toolSettings.dStrokeText = "Black";
	appPrefs.toolSettings.dTextBackGround = "None";
	appPrefs.toolSettings.dTextLineColor = "None";
	appPrefs.toolSettings.dTextBackGroundShade = 100;
	appPrefs.toolSettings.dTextLineShade = 100;
	appPrefs.toolSettings.dTextPenShade = 100;
	appPrefs.toolSettings.dTextStrokeShade = 100;
	appPrefs.toolSettings.tabFillChar = "";
	appPrefs.toolSettings.dTabWidth = 36.0;
	appPrefs.DpapColor = QColor(white);
	appPrefs.toolSettings.dCols = 1;
	appPrefs.toolSettings.dGap = 0.0;
	appPrefs.toolSettings.dShadeLine = 100;
	appPrefs.toolSettings.dLstyleLine = SolidLine;
	appPrefs.toolSettings.dWidthLine = 1;
	appPrefs.toolSettings.dStartArrow = 0;
	appPrefs.toolSettings.dEndArrow = 0;
	appPrefs.toolSettings.magMin = 10;
	appPrefs.toolSettings.magMax = 3200;
	appPrefs.toolSettings.magStep = 25;
	appPrefs.toolSettings.dBrushPict = "White";
	appPrefs.toolSettings.shadePict = 100;
	appPrefs.toolSettings.scaleX = 1;
	appPrefs.toolSettings.scaleY = 1;
	appPrefs.guidesSettings.before = true;
	appPrefs.docUnitIndex = 0;
	appPrefs.toolSettings.polyC = 4;
	appPrefs.toolSettings.polyF = 0.5;
	appPrefs.toolSettings.polyS = false;
	appPrefs.toolSettings.polyFd = 0;
	appPrefs.toolSettings.polyR = 0;
	appPrefs.mainToolBarSettings.visible = true;
	appPrefs.pdfToolBarSettings.visible = true;
	appPrefs.PSize = 40;
	appPrefs.SaveAtQ = true;
	appPrefs.ClipMargin = true;
	appPrefs.GCRMode = false;
	appPrefs.RecentDocs.clear();
	appPrefs.RecentDCount = 5;
	appPrefs.marginColored = false;
	appPrefs.pageSize = "A4";
	appPrefs.pageOrientation = 0;
	PageSize a4("A4");
	appPrefs.PageWidth = a4.getPageWidth();
	appPrefs.PageHeight = a4.getPageHeight();
	appPrefs.RandOben = 40;
	appPrefs.RandUnten = 40;
	appPrefs.RandLinks = 40;
	appPrefs.RandRechts = 40;
	appPrefs.toolSettings.scaleType = true;
	appPrefs.toolSettings.aspectRatio = true;
	appPrefs.toolSettings.lowResType = 1;
	appPrefs.toolSettings.useEmbeddedPath = false;
	appPrefs.MinWordLen = 3;
	appPrefs.HyCount = 2;
	appPrefs.Language = "";
	appPrefs.Automatic = true;
	appPrefs.AutoCheck = false;
	appPrefs.AutoSave = false;
	appPrefs.AutoSaveTime = 600000;
	appPrefs.DisScale = 1.0;
	appPrefs.DocDir = QDir::homeDirPath();
	appPrefs.ProfileDir = "";
	appPrefs.ScriptDir = "";
	appPrefs.documentTemplatesDir = "";
	appPrefs.CustomColorSets.clear();
	appPrefs.PrPr_Mode = false;
	appPrefs.Gcr_Mode = true;
	appPrefs.PrPr_AlphaText = false;
	appPrefs.PrPr_AlphaGraphics = false;
	appPrefs.PrPr_Transparency = false;
	appPrefs.PrPr_C = true;
	appPrefs.PrPr_M = true;
	appPrefs.PrPr_Y = true;
	appPrefs.PrPr_K = true;
	appPrefs.imageEditorExecutable = "gimp";
	appPrefs.gs_AntiAliasGraphics = true;
	appPrefs.gs_AntiAliasText = true;
	appPrefs.gs_exe = getGSDefaultExeName();
	appPrefs.gs_Resolution = 72;
	appPrefs.STEcolor = QColor(white);
	appPrefs.DCMSset.DefaultMonitorProfile = "";
	appPrefs.DCMSset.DefaultPrinterProfile = "";
	appPrefs.DCMSset.DefaultImageRGBProfile = "";
	appPrefs.DCMSset.DefaultSolidColorProfile = "";
	appPrefs.DCMSset.CMSinUse = false;
	appPrefs.DCMSset.SoftProofOn = false;
	appPrefs.DCMSset.GamutCheck = false;
	appPrefs.DCMSset.BlackPoint = true;
	appPrefs.DCMSset.DefaultIntentMonitor = 1;
	appPrefs.DCMSset.DefaultIntentImages = 1;
	appPrefs.DCMSset.DefaultIntentPrinter = 0;
	appPrefs.GFontSub.clear();
	appPrefs.ScratchLeft = 100;
	appPrefs.ScratchRight = 100;
	appPrefs.ScratchTop = 20;
	appPrefs.ScratchBottom = 20;
	struct PageSet pageS;
	pageS.Name = tr( "Single Page" );
	pageS.FirstPage = 0;
	pageS.Rows = 1;
	pageS.Columns = 1;
	pageS.GapHorizontal = 0.0;
	pageS.GapVertical = 0.0;
	pageS.GapBelow = 40.0;
	pageS.pageNames.clear();
	appPrefs.pageSets.append(pageS);
	pageS.Name = tr( "Double sided" );
	pageS.FirstPage = 1;
	pageS.Columns = 2;
	pageS.pageNames.clear();
	pageS.pageNames.append( tr("Left Page"));
	pageS.pageNames.append( tr("Right Page"));
	appPrefs.pageSets.append(pageS);
	pageS.Name = tr( "3-Fold" );
	pageS.FirstPage = 0;
	pageS.Columns = 3;
	pageS.pageNames.clear();
	pageS.pageNames.append( tr("Left Page"));
	pageS.pageNames.append( tr("Middle"));
	pageS.pageNames.append( tr("Right Page"));
	appPrefs.pageSets.append(pageS);
	pageS.Name = tr( "4-Fold" );
	pageS.FirstPage = 0;
	pageS.Columns = 4;
	pageS.pageNames.clear();
	pageS.pageNames.append( tr("Left Page"));
	pageS.pageNames.append( tr("Middle Left"));
	pageS.pageNames.append( tr("Middle Right"));
	pageS.pageNames.append( tr("Right Page"));
	appPrefs.pageSets.append(pageS);
	appPrefs.FacingPages = singlePage;
	appPrefs.askBeforeSubstituite = true;
	appPrefs.haveStylePreview = true;
	// lorem ipsum defaults
	appPrefs.useStandardLI = false;
	appPrefs.paragraphsLI = 10;
	appPrefs.showStartupDialog = true;
	initDefaultCheckerPrefs(&appPrefs.checkerProfiles);
	appPrefs.curCheckProfile = tr("PostScript");
	appPrefs.PDF_Options.Thumbnails = false;
	appPrefs.PDF_Options.Articles = false;
	appPrefs.PDF_Options.useLayers = false;
	appPrefs.PDF_Options.Compress = true;
	appPrefs.PDF_Options.CompressMethod = 0;
	appPrefs.PDF_Options.Quality = 0;
	appPrefs.PDF_Options.RecalcPic = false;
	appPrefs.PDF_Options.Bookmarks = false;
	appPrefs.PDF_Options.PicRes = 300;
	appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_14;
	appPrefs.PDF_Options.Resolution = 300;
	appPrefs.PDF_Options.Binding = 0;
	appPrefs.PDF_Options.EmbedList.clear();
	appPrefs.PDF_Options.SubsetList.clear();
	appPrefs.PDF_Options.MirrorH = false;
	appPrefs.PDF_Options.MirrorV = false;
	appPrefs.PDF_Options.RotateDeg = 0;
	appPrefs.PDF_Options.PresentMode = false;
	appPrefs.PDF_Options.Datei = "";
	appPrefs.PDF_Options.PresentVals.clear();
	appPrefs.PDF_Options.isGrayscale = false;
	appPrefs.PDF_Options.UseRGB = true;
	appPrefs.PDF_Options.UseProfiles = false;
	appPrefs.PDF_Options.UseProfiles2 = false;
	appPrefs.PDF_Options.SolidProf = "";
	appPrefs.PDF_Options.SComp = 3;
	appPrefs.PDF_Options.ImageProf = "";
	appPrefs.PDF_Options.PrintProf = "";
	appPrefs.PDF_Options.Info = "";
	appPrefs.PDF_Options.Intent = 0;
	appPrefs.PDF_Options.Intent2 = 0;
	appPrefs.PDF_Options.BleedTop = 0;
	appPrefs.PDF_Options.BleedLeft = 0;
	appPrefs.PDF_Options.BleedRight = 0;
	appPrefs.PDF_Options.BleedBottom = 0;
	appPrefs.PDF_Options.EmbeddedI = false;
	appPrefs.PDF_Options.Encrypt = false;
	appPrefs.PDF_Options.PassOwner = "";
	appPrefs.PDF_Options.PassUser = "";
	appPrefs.PDF_Options.Permissions = -4;
	appPrefs.PDF_Options.UseLPI = false;
	appPrefs.PDF_Options.LPISettings.clear();
	appPrefs.PDF_Options.UseSpotColors = true;
	appPrefs.PDF_Options.doMultiFile = false;

	//Attribute setup
	appPrefs.defaultItemAttributes.clear();
	appPrefs.defaultToCSetups.clear();
}

void PrefsManager::initDefaultGUIFont(const QFont& guiFont)
{
	appPrefs.AppFontSize = guiFont.pointSize();
	appPrefs.STEfont = guiFont.toString();
	appPrefs.PaletteFontSize = appPrefs.AppFontSize;
}

void PrefsManager::initArrowStyles()
{
	struct ArrowDesc arrow;
	FPointArray points;
	QWMatrix arrowScaling;
	arrowScaling.scale(0.5, 0.5);
	arrow.name = "Arrow1L";
	arrow.userArrow = false;
	points.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
	points.addQuadPoint(-5, -5, -5, -5, -5, -5, -5, -5);
	points.addQuadPoint(12, 0, 12, 0, 12, 0, 12, 0);
	points.addQuadPoint(-5, 5, -5, 5, -5, 5, -5, 5);
	points.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "Arrow1M";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "Arrow1S";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "SquareL";
	points.resize(0);
	points.addQuadPoint(-5, -5, -5, -5, -5, -5, -5, -5);
	points.addQuadPoint(5, -5, 5, -5, 5, -5, 5, -5);
	points.addQuadPoint(5, 5, 5, 5, 5, 5, 5, 5);
	points.addQuadPoint(-5, 5, -5, 5, -5, 5, -5, 5);
	points.addQuadPoint(-5, -5, -5, -5, -5, -5, -5, -5);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "SquareM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "SquareS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "TriangleInL";
	points.resize(0);
	points.addQuadPoint(5.77, 0, 5.77, 0, 5.77, 0, 5.77, 0);
	points.addQuadPoint(-2.88, 5, -2.88, 5, -2.88, 5, -2.88, 5);
	points.addQuadPoint(-2.88, -5, -2.88, -5, -2.88, -5, -2.88, -5);
	points.addQuadPoint(5.77, 0, 5.77, 0, 5.77, 0, 5.77, 0);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "TriangleInM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "TriangleInS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "TriangleOutL";
	points.resize(0);
	points.addQuadPoint(-5.77, 0, -5.77, 0, -5.77, 0, -5.77, 0);
	points.addQuadPoint(2.88, 5, 2.88, 5, 2.88, 5, 2.88, 5);
	points.addQuadPoint(2.88, -5, 2.88, -5, 2.88, -5, 2.88, -5);
	points.addQuadPoint(-5.77, 0, -5.77, 0, -5.77, 0, -5.77, 0);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "TriangleOutM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "TriangleOutS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
}

/*!
 \fn QString PrefsManager::getPreferencesLocation()
 \author Craig Bradney
 \date Thu 18 Nov 2004
 \brief Set the user's preference file location. Rename any existing old preferences files
 \retval QString Location of the user's preferences
 */

QString PrefsManager::setupPreferencesLocation()
{
	QString Pff = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus");
	QFileInfo Pffi = QFileInfo(Pff);
	QString PrefsPfad;
	//If we are using ~/.scribus
	if (Pffi.exists())
	{
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QDir::homeDirPath();
	}
	else // Move to using ~/.scribus/scribus.* from ~/.scribus.*
	{
		QDir prefsDirectory = QDir();
		prefsDirectory.mkdir(Pff);
		PrefsPfad = Pff;
		QString oldPR = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus.rc");
		QFileInfo oldPi = QFileInfo(oldPR);
		if (oldPi.exists())
			moveFile(oldPR, Pff+"/scribus.rc");
		QString oldPR2 = QDir::convertSeparators(QDir::homeDirPath()+"/.scribusfont.rc");
		QFileInfo oldPi2 = QFileInfo(oldPR2);
		if (oldPi2.exists())
			moveFile(oldPR2, Pff+"/scribusfont.rc");
		QString oldPR3 = QDir::convertSeparators(QDir::homeDirPath()+"/.scribusscrap.scs");
		QFileInfo oldPi3 = QFileInfo(oldPR3);
		if (oldPi3.exists())
			moveFile(oldPR3, Pff+"/scrap.scs");
	}
	prefsLocation=PrefsPfad;
	return PrefsPfad;
}

/*!
 \fn QString PrefsManager::getPreferencesLocation()
 \retval QString Location of the user's preferences
 */
const QString PrefsManager::preferencesLocation()
{
	return prefsLocation;
}

/*!
 \fn bool ScribusApp::copy12Preferences(const QString prefsLocation)
 \author Craig Bradney
 \date Sun 09 Jan 2005
 \brief Copy 1.2 prefs XML before loading, and copy rc files we don't yet convert
 \param prefsLocation Location of user preferences
 \retval bool true if prefs were imported
 */
bool PrefsManager::copy12Preferences()
{
	//Now make copies for 1.3 use and leave the old ones alone for <1.3.0 usage
	QString oldPR[4], newPR[4];

	// Special case for scribus.rc - if found, use scribus123.rc,
	// otherwise fall back to the possibly mis-encoded scribus.rc .
	oldPR[0]=QDir::convertSeparators(prefsLocation+"/scribus123.rc");
	if (!QFile::exists(oldPR[0]))
		oldPR[0] = prefsLocation+"/scribus.rc";

	oldPR[1]=QDir::convertSeparators(prefsLocation+"/scrap.scs");
	oldPR[2]=QDir::convertSeparators(prefsLocation+"/prefs.xml");
	oldPR[3]=QDir::convertSeparators(prefsLocation+"/scripter.rc");
	newPR[0]=QDir::convertSeparators(prefsLocation+"/scribus13.rc");
	newPR[1]=QDir::convertSeparators(prefsLocation+"/scrap13.scs");
	newPR[2]=QDir::convertSeparators(prefsLocation+"/prefs13.xml");
	newPR[3]=QDir::convertSeparators(prefsLocation+"/scripter13.rc");

	bool existsOldPR[4], existsNewPR[4];
	for (uint i=0;i<4;++i)
	{
		existsOldPR[i]=QFile::exists(oldPR[i]);
		existsNewPR[i]=QFile::exists(newPR[i]);
	}

	bool retVal=false;
	//Only check for these two as they will be autocreated if they dont exist.
	if( (existsOldPR[0] && !existsNewPR[0]) || (existsOldPR[2] && !existsNewPR[2]) )
	{
		retVal=true; // converting from 1.2 prefs
		if (ScQApp->usingGUI())
		{
			ScApp->showSplash(false);
			if ( (QMessageBox::question( ScApp, tr("Migrate Old Scribus Settings?"),
				tr("Scribus has detected existing Scribus 1.2 preferences files.\n"
						"Do you want to migrate them to the new Scribus version?"),
				QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::NoButton))==QMessageBox::Yes )
			{
				for (uint i=0;i<4;++i)
				{
					if (existsOldPR[i] && !existsNewPR[i])
						copyFile(oldPR[i], newPR[i]);
				}
			}
			ScApp->showSplash(true);
		}
	}
	return retVal;
}

/*!
 \fn void PrefsManager::convert12Preferences()
 \author Craig Ringer
 \date Sun 26 June 2005
 \brief Import 1.2.x prefs rc data into new prefs xml
 \retval None
 */
void PrefsManager::convert12Preferences()
{
	// Import 1.2 font search path prefs
	QFile fontPrefsFile12(QDir::convertSeparators(prefsLocation+"/scribusfont.rc"));
	if (fontPrefsFile12.open(IO_ReadOnly))
	{
		PrefsContext *pc = prefsFile->getContext("Fonts");
		PrefsTable *fontPrefs = pc->getTable("ExtraFontDirs");
		QTextStream tsx(&fontPrefsFile12);
		QString extraPath = tsx.read();
		fontPrefsFile12.close();
		QStringList extraFonts = QStringList::split("\n",extraPath);
		for (uint i = 0; i < extraFonts.count(); ++i)
			fontPrefs->set(i, 0, extraFonts[i]);
	}
}

void PrefsManager::ReadPrefs(const QString & fname)
{
	QString realFile;
	if (fname.isNull())
		realFile = prefsLocation + "/scribus13.rc";
	else
		realFile = fname;

	if (QFile::exists(realFile))
	{
		if (!ReadPref(realFile))
		{
			alertLoadPrefsFailed();
			return;
		}
	}

	ScApp->setDefaultPrinter(appPrefs.PrinterName, appPrefs.PrinterFile, appPrefs.PrinterCommand);

	uint max = QMIN(appPrefs.RecentDCount, appPrefs.RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		QFileInfo fd(appPrefs.RecentDocs[m]);
		if (fd.exists())
		{
			ScApp->RecentDocs.append(appPrefs.RecentDocs[m]);
			ScApp->fileWatcher->addFile(appPrefs.RecentDocs[m]);
		}
	}
	ScApp->rebuildRecentFileMenu();
	ScApp->move(appPrefs.mainWinSettings.xPosition, appPrefs.mainWinSettings.yPosition);
	ScApp->resize(appPrefs.mainWinSettings.width, appPrefs.mainWinSettings.height);
	ReadPrefsXML();
	if (appPrefs.checkerProfiles.count() == 0)
	{
		initDefaultCheckerPrefs(&appPrefs.checkerProfiles);
		appPrefs.curCheckProfile = tr("PostScript");
	}
}

void PrefsManager::ReadPrefsXML()
{
    if (prefsFile)
    {
        PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
        if (userprefsContext) {
            appPrefs.guiLanguage = userprefsContext->get("gui_language","");
            //continue here...
            //Prefs."blah blah" =...
        }
    }
}


void PrefsManager::SavePrefs(const QString & fname)
{
	// If closing because of a crash don't save prefs as we can
	// accidentally nuke the settings if the crash is before prefs are loaded
	// The caller is responsible for ensuring we aren't called under those
	// conditions.
	Q_ASSERT(!emergencyActivated);
	appPrefs.mainWinSettings.xPosition = abs(ScApp->pos().x());
	appPrefs.mainWinSettings.yPosition = abs(ScApp->pos().y());
	appPrefs.mainWinSettings.width = ScApp->size().width();
	appPrefs.mainWinSettings.height = ScApp->size().height();
	appPrefs.mainToolBarSettings.visible = ScApp->mainToolBarVisible();
	appPrefs.pdfToolBarSettings.visible = ScApp->pdfToolBarVisible();

	appPrefs.RecentDocs.clear();
	uint max = QMIN(appPrefs.RecentDCount, ScApp->RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		appPrefs.RecentDocs.append(ScApp->RecentDocs[m]);
	}
	ScApp->getDefaultPrinter(&appPrefs.PrinterName, &appPrefs.PrinterFile, &appPrefs.PrinterCommand);

	SavePrefsXML();
	QString realFile;
	if (fname.isNull())
		realFile = prefsLocation+"/scribus13.rc";
	else
		realFile = fname;
	if (!WritePref(realFile))
		alertSavePrefsFailed();
}

void PrefsManager::SavePrefsXML()
{
    if (prefsFile)
    {
        PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
        if (userprefsContext) {
            userprefsContext->set("gui_language",appPrefs.guiLanguage);
            //continue here...
            //Prefs."blah blah" =...
        }
        prefsFile->write();
    }
}

void PrefsManager::setGhostscriptExecutable(const QString& executableName)
{
	appPrefs.gs_exe=executableName;
}

void PrefsManager::setImageEditorExecutable(const QString& executableName)
{
	appPrefs.imageEditorExecutable=executableName;
}

const QString PrefsManager::ghostscriptExecutable()
{
	return appPrefs.gs_exe;
}

const QString PrefsManager::imageEditorExecutable()
{
	return appPrefs.imageEditorExecutable;
}

const QString PrefsManager::documentDir()
{
	return appPrefs.DocDir;
}

void PrefsManager::setDocumentDir(const QString& dirname)
{
	appPrefs.DocDir = dirname;
}

const int PrefsManager::mouseWheelValue()
{
	return appPrefs.Wheelval;
}

//Changed to return false when we have no fonts
const bool PrefsManager::GetAllFonts(bool showFontInfo)
{
	appPrefs.AvailFonts.GetFonts(prefsLocation, showFontInfo);
	if (appPrefs.AvailFonts.isEmpty())
		return false;
	return true;
}

void PrefsManager::setShowStartupDialog(const bool showDialog)
{
	appPrefs.showStartupDialog=showDialog;
}

const ColorList& PrefsManager::colorSet()
{
	return appPrefs.DColors;
}

ColorList* PrefsManager::colorSetPtr()
{
	return &appPrefs.DColors;
}

const QString& PrefsManager::colorSetName()
{
	return appPrefs.DColorSet;
}

void PrefsManager::setColorSet(const ColorList& colorSet)
{
	appPrefs.DColors=colorSet;
}

void PrefsManager::setColorSetName(const QString& colorSetName)
{
	appPrefs.DColorSet=colorSetName;
}


void PrefsManager::setKeyEntry(const QString& actName, const QString& cleanMenuText, const QKeySequence& keyseq, const int& rowNumber)
{
	Keys ke;
	if (!actName.isEmpty())
	{
		if (ScApp->scrActions[actName])
		{
			ke.actionName=actName;
			ke.keySequence = keyseq;
			ke.cleanMenuText=cleanMenuText;
			ke.tableRow=rowNumber;
			appPrefs.KeyActions.insert(actName, ke);
		}
		else
			qDebug("%s", QString("Action Name: %1 does not exist").arg(actName).ascii());
	}
}

const double PrefsManager::displayScale()
{
	return appPrefs.DisScale;
}

const QString& PrefsManager::guiLanguage()
{
	return appPrefs.guiLanguage;
}

const QString& PrefsManager::guiStyle()
{
	return appPrefs.GUI;
}

const int& PrefsManager::guiFontSize()
{
	return appPrefs.AppFontSize;
}

const int& PrefsManager::paletteFontSize()
{
	return appPrefs.PaletteFontSize;
}

bool PrefsManager::WritePref(QString ho)
{
	QDomDocument docu("scribusrc");
	QString st="<SCRIBUSRC></SCRIBUSRC>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	QDomElement dc=docu.createElement("GUI");
	dc.setAttribute("STILT",appPrefs.GUI);
	dc.setAttribute("RAD",appPrefs.Wheelval);
	dc.setAttribute("APF",appPrefs.AppFontSize);
	dc.setAttribute("PFS",appPrefs.PaletteFontSize);
	dc.setAttribute("GRAB",appPrefs.guidesSettings.grabRad);
	dc.setAttribute("UNIT",appPrefs.docUnitIndex);
	dc.setAttribute("RCD", appPrefs.RecentDCount);
	dc.setAttribute("DOC", appPrefs.DocDir);
	dc.setAttribute("PROFILES", appPrefs.ProfileDir);
	dc.setAttribute("SCRIPTS", appPrefs.ScriptDir);
	dc.setAttribute("TEMPLATES", appPrefs.documentTemplatesDir);
	dc.setAttribute("SHOWGUIDES", static_cast<int>(appPrefs.guidesSettings.guidesShown));
	dc.setAttribute("FRV", static_cast<int>(appPrefs.guidesSettings.framesShown));
	dc.setAttribute("SHOWMARGIN", static_cast<int>(appPrefs.guidesSettings.marginsShown));
	dc.setAttribute("SHOWBASE", static_cast<int>(appPrefs.guidesSettings.baseShown));
	dc.setAttribute("SHOWLINK", static_cast<int>(appPrefs.guidesSettings.linkShown));
	dc.setAttribute("SHOWPICT", static_cast<int>(appPrefs.guidesSettings.showPic));
	dc.setAttribute("SHOWControl", static_cast<int>(appPrefs.guidesSettings.showControls));
	dc.setAttribute("rulerMode", static_cast<int>(appPrefs.guidesSettings.rulerMode));
	dc.setAttribute("ScratchBottom", appPrefs.ScratchBottom);
	dc.setAttribute("ScratchLeft", appPrefs.ScratchLeft);
	dc.setAttribute("ScratchRight", appPrefs.ScratchRight);
	dc.setAttribute("ScratchTop", appPrefs.ScratchTop);
	dc.setAttribute("STECOLOR", appPrefs.STEcolor.name());
	dc.setAttribute("STEFONT", appPrefs.STEfont);
	dc.setAttribute("STYLEPREVIEW", static_cast<int>(appPrefs.haveStylePreview));
	dc.setAttribute("StartUp", static_cast<int>(appPrefs.showStartupDialog));
	elem.appendChild(dc);
	QDomElement dc1=docu.createElement("GRID");
	dc1.setAttribute("MINOR",appPrefs.guidesSettings.minorGrid);
	dc1.setAttribute("MAJOR",appPrefs.guidesSettings.majorGrid);
	dc1.setAttribute("MINORC",appPrefs.guidesSettings.minorColor.name());
	dc1.setAttribute("MAJORC",appPrefs.guidesSettings.majorColor.name());
	dc1.setAttribute("GuideC", appPrefs.guidesSettings.guideColor.name());
	dc1.setAttribute("BaseC", appPrefs.guidesSettings.baseColor.name());
	dc1.setAttribute("GuideZ", appPrefs.guidesSettings.guideRad);
	dc1.setAttribute("BACKG", static_cast<int>(appPrefs.guidesSettings.before));
	dc1.setAttribute("SHOW", static_cast<int>(appPrefs.guidesSettings.gridShown));
	elem.appendChild(dc1);
	QDomElement dc1a=docu.createElement("PAGE");
	dc1a.setAttribute("PAGEC",appPrefs.DpapColor.name());
	dc1a.setAttribute("MARGC",appPrefs.guidesSettings.margColor.name());
	dc1a.setAttribute("RANDF", static_cast<int>(appPrefs.marginColored));
	dc1a.setAttribute("DScale",appPrefs.DisScale);
	elem.appendChild(dc1a);
	QDomElement dc2=docu.createElement("FONTS");
	dc2.setAttribute("FACE",appPrefs.toolSettings.defFont);
	dc2.setAttribute("SIZE",appPrefs.toolSettings.defSize / 10.0);
	dc2.setAttribute("AutomaticSubst", static_cast<int>(appPrefs.askBeforeSubstituite));
	elem.appendChild(dc2);
	QDomElement dc3=docu.createElement("TYPO");
	dc3.setAttribute("TIEF",appPrefs.typographicSettings.valueSubScript);
	dc3.setAttribute("TIEFSC",appPrefs.typographicSettings.scalingSubScript);
	dc3.setAttribute("HOCH",appPrefs.typographicSettings.valueSuperScript);
	dc3.setAttribute("HOCHSC",appPrefs.typographicSettings.scalingSuperScript);
	dc3.setAttribute("SMCAPS",appPrefs.typographicSettings.valueSmallCaps);
	dc3.setAttribute("AUTOL", appPrefs.typographicSettings.autoLineSpacing);
	dc3.setAttribute("BASE", appPrefs.typographicSettings.valueBaseGrid);
	dc3.setAttribute("BASEO", appPrefs.typographicSettings.offsetBaseGrid);
	if (appPrefs.typographicSettings.valueUnderlinePos == -1)
		dc3.setAttribute("UnderlinePos", appPrefs.typographicSettings.valueUnderlinePos);
	else
		dc3.setAttribute("UnderlinePos", appPrefs.typographicSettings.valueUnderlinePos / 10.0);
	if (appPrefs.typographicSettings.valueUnderlineWidth == -1)
		dc3.setAttribute("UnderlineWidth", appPrefs.typographicSettings.valueUnderlineWidth);
	else
		dc3.setAttribute("UnderlineWidth", appPrefs.typographicSettings.valueUnderlineWidth / 10.0);
	if (appPrefs.typographicSettings.valueStrikeThruPos == -1)
		dc3.setAttribute("StrikeThruPos", appPrefs.typographicSettings.valueStrikeThruPos);
	else
		dc3.setAttribute("StrikeThruPos", appPrefs.typographicSettings.valueStrikeThruPos / 10.0);
	if (appPrefs.typographicSettings.valueStrikeThruWidth == -1)
		dc3.setAttribute("StrikeThruWidth", appPrefs.typographicSettings.valueStrikeThruWidth);
	else
		dc3.setAttribute("StrikeThruWidth", appPrefs.typographicSettings.valueStrikeThruWidth / 10.0);
	elem.appendChild(dc3);
	QDomElement dc9=docu.createElement("TOOLS");
	dc9.setAttribute("PEN",appPrefs.toolSettings.dPen);
	dc9.setAttribute("BRUSH",appPrefs.toolSettings.dBrush);
	dc9.setAttribute("PENLINE",appPrefs.toolSettings.dPenLine);
	dc9.setAttribute("PENTEXT",appPrefs.toolSettings.dPenText);
	dc9.setAttribute("StrokeText",appPrefs.toolSettings.dStrokeText);
	dc9.setAttribute("TextBackGround", appPrefs.toolSettings.dTextBackGround);
	dc9.setAttribute("TextLineColor", appPrefs.toolSettings.dTextLineColor);
	dc9.setAttribute("TextBackGroundShade", appPrefs.toolSettings.dTextBackGroundShade);
	dc9.setAttribute("TextLineShade", appPrefs.toolSettings.dTextLineShade);
	dc9.setAttribute("TextPenShade", appPrefs.toolSettings.dTextPenShade);
	dc9.setAttribute("TextStrokeShade", appPrefs.toolSettings.dTextStrokeShade);
	dc9.setAttribute("TEXTCOL",appPrefs.toolSettings.dCols);
	dc9.setAttribute("TEXTGAP",appPrefs.toolSettings.dGap);
	dc9.setAttribute("TabWidth",appPrefs.toolSettings.dTabWidth);
	dc9.setAttribute("TabFill",appPrefs.toolSettings.tabFillChar);
	dc9.setAttribute("STIL",appPrefs.toolSettings.dLineArt);
	dc9.setAttribute("STILLINE",appPrefs.toolSettings.dLstyleLine);
	dc9.setAttribute("WIDTH",appPrefs.toolSettings.dWidth);
	dc9.setAttribute("WIDTHLINE",appPrefs.toolSettings.dWidthLine);
	dc9.setAttribute("PENSHADE",appPrefs.toolSettings.dShade2);
	dc9.setAttribute("LINESHADE",appPrefs.toolSettings.dShadeLine);
	dc9.setAttribute("BRUSHSHADE",appPrefs.toolSettings.dShade);
	dc9.setAttribute("MAGMIN",appPrefs.toolSettings.magMin);
	dc9.setAttribute("MAGMAX",appPrefs.toolSettings.magMax);
	dc9.setAttribute("MAGSTEP",appPrefs.toolSettings.magStep);
	dc9.setAttribute("CPICT",appPrefs.toolSettings.dBrushPict);
	dc9.setAttribute("PICTSHADE",appPrefs.toolSettings.shadePict);
	dc9.setAttribute("PICTSCX",appPrefs.toolSettings.scaleX);
	dc9.setAttribute("PICTSCY",appPrefs.toolSettings.scaleY);
	dc9.setAttribute("POLYC", appPrefs.toolSettings.polyC);
	dc9.setAttribute("POLYF", appPrefs.toolSettings.polyF);
	dc9.setAttribute("POLYR", appPrefs.toolSettings.polyR);
	dc9.setAttribute("POLYFD", appPrefs.toolSettings.polyFd);
	dc9.setAttribute("POLYS", static_cast<int>(appPrefs.toolSettings.polyS));
	dc9.setAttribute("PSCALE", static_cast<int>(appPrefs.toolSettings.scaleType));
	dc9.setAttribute("PASPECT", static_cast<int>(appPrefs.toolSettings.aspectRatio));
	dc9.setAttribute("EmbeddedPath", static_cast<int>(appPrefs.toolSettings.useEmbeddedPath));
	dc9.setAttribute("HalfRes", appPrefs.toolSettings.lowResType);
	dc9.setAttribute("StartArrow", appPrefs.toolSettings.dStartArrow);
	dc9.setAttribute("EndArrow", appPrefs.toolSettings.dEndArrow);
	elem.appendChild(dc9);
	QDomElement dc4=docu.createElement("MAINWINDOW");
	dc4.setAttribute("XPOS",appPrefs.mainWinSettings.xPosition);
	dc4.setAttribute("YPOS",appPrefs.mainWinSettings.yPosition);
	dc4.setAttribute("WIDTH",appPrefs.mainWinSettings.width);
	dc4.setAttribute("HEIGHT",appPrefs.mainWinSettings.height);
	elem.appendChild(dc4);
	QDomElement dc5=docu.createElement("TOOLPALETTE");
	dc5.setAttribute("VISIBLE", static_cast<int>(appPrefs.mainToolBarSettings.visible));
	dc5.setAttribute("PDFVISIBLE", static_cast<int>(appPrefs.pdfToolBarSettings.visible));
	elem.appendChild(dc5);
	QDomElement dc73=docu.createElement("SCRAPBOOK");
	dc73.setAttribute("PREVIEW",appPrefs.PSize);
	dc73.setAttribute("SAVE", static_cast<int>(appPrefs.SaveAtQ));
	elem.appendChild(dc73);
	QDomElement dc75=docu.createElement("PAGEPALETTE");
	dc75.setAttribute("THUMBS", static_cast<int>(appPrefs.SepalT));
	dc75.setAttribute("NAMES", static_cast<int>(appPrefs.SepalN));
	elem.appendChild(dc75);
	QDomElement dc76=docu.createElement("DOKUMENT");
	dc76.setAttribute("PAGESIZE",appPrefs.pageSize);
	dc76.setAttribute("AUSRICHTUNG",appPrefs.pageOrientation);
	dc76.setAttribute("BREITE",appPrefs.PageWidth);
	dc76.setAttribute("HOEHE",appPrefs.PageHeight);
	dc76.setAttribute("RANDO",appPrefs.RandOben);
	dc76.setAttribute("RANDU",appPrefs.RandUnten);
	dc76.setAttribute("RANDL",appPrefs.RandLinks);
	dc76.setAttribute("RANDR",appPrefs.RandRechts);
	dc76.setAttribute("DOPPEL", appPrefs.FacingPages);
	dc76.setAttribute("AutoSave", static_cast<int>(appPrefs.AutoSave));
	dc76.setAttribute("AutoSaveTime", appPrefs.AutoSaveTime);
	elem.appendChild(dc76);
	QDomElement pageSetAttr = docu.createElement("PageSets");
	QValueList<PageSet>::Iterator itpgset;
	for(itpgset = appPrefs.pageSets.begin(); itpgset != appPrefs.pageSets.end(); ++itpgset )
	{
		QDomElement pgst = docu.createElement("Set");
		pgst.setAttribute("Name", (*itpgset).Name);
		pgst.setAttribute("FirstPage", (*itpgset).FirstPage);
		pgst.setAttribute("Rows", (*itpgset).Rows);
		pgst.setAttribute("Columns", (*itpgset).Columns);
		pgst.setAttribute("GapHorizontal", (*itpgset).GapHorizontal);
		pgst.setAttribute("GapVertical", (*itpgset).GapVertical);
		pgst.setAttribute("GapBelow", (*itpgset).GapBelow);
		QStringList pNames = (*itpgset).pageNames;
		QStringList::Iterator itpgsetN;
		for(itpgsetN = pNames.begin(); itpgsetN != pNames.end(); ++itpgsetN )
		{
			QDomElement pgstN = docu.createElement("PageNames");
			pgstN.setAttribute("Name", (*itpgsetN));
			pgst.appendChild(pgstN);
		}
		pageSetAttr.appendChild(pgst);
	}
	elem.appendChild(pageSetAttr);
	QDomElement dc79ac=docu.createElement("Checker");
	dc79ac.setAttribute("currentProfile", appPrefs.curCheckProfile);
	elem.appendChild(dc79ac);
	CheckerPrefsList::Iterator itcp;
	CheckerPrefsList::Iterator itcpend=appPrefs.checkerProfiles.end();
	for (itcp = appPrefs.checkerProfiles.begin(); itcp != itcpend; ++itcp)
	{
		QDomElement dc79a=docu.createElement("CheckProfile");
		dc79a.setAttribute("Name",itcp.key());
		dc79a.setAttribute("ignoreErrors", static_cast<int>(itcp.data().ignoreErrors));
		dc79a.setAttribute("autoCheck", static_cast<int>(itcp.data().autoCheck));
		dc79a.setAttribute("checkGlyphs", static_cast<int>(itcp.data().checkGlyphs));
		dc79a.setAttribute("checkOrphans", static_cast<int>(itcp.data().checkOrphans));
		dc79a.setAttribute("checkOverflow", static_cast<int>(itcp.data().checkOverflow));
		dc79a.setAttribute("checkPictures", static_cast<int>(itcp.data().checkPictures));
		dc79a.setAttribute("checkResolution", static_cast<int>(itcp.data().checkResolution));
		dc79a.setAttribute("checkTransparency", static_cast<int>(itcp.data().checkTransparency));
		dc79a.setAttribute("checkAnnotations", static_cast<int>(itcp.data().checkAnnotations));
		dc79a.setAttribute("checkRasterPDF", static_cast<int>(itcp.data().checkRasterPDF));
		dc79a.setAttribute("minResolution",itcp.data().minResolution);
		elem.appendChild(dc79a);
	}
	QDomElement dc81=docu.createElement("CMS");
	dc81.setAttribute("DPSo", static_cast<int>(appPrefs.DCMSset.SoftProofOn));
	dc81.setAttribute("DPuse", static_cast<int>(appPrefs.DCMSset.CMSinUse));
	dc81.setAttribute("DPgam", static_cast<int>(appPrefs.DCMSset.GamutCheck));
	dc81.setAttribute("DPbla", static_cast<int>(appPrefs.DCMSset.BlackPoint));
	dc81.setAttribute("DPMo",appPrefs.DCMSset.DefaultMonitorProfile);
	dc81.setAttribute("DPPr",appPrefs.DCMSset.DefaultPrinterProfile);
	dc81.setAttribute("DPIn",appPrefs.DCMSset.DefaultImageRGBProfile);
	dc81.setAttribute("DPInCMYK",appPrefs.DCMSset.DefaultImageCMYKProfile);
	dc81.setAttribute("DPIn2",appPrefs.DCMSset.DefaultSolidColorProfile);
	dc81.setAttribute("DIPr",appPrefs.DCMSset.DefaultIntentPrinter);
	dc81.setAttribute("DIMo",appPrefs.DCMSset.DefaultIntentMonitor);
	dc81.setAttribute("DIMo2",appPrefs.DCMSset.DefaultIntentImages);
	elem.appendChild(dc81);
	QDomElement dc82=docu.createElement("PRINTER");
	dc82.setAttribute("NAME",appPrefs.PrinterName);
	dc82.setAttribute("FILE",appPrefs.PrinterFile);
	dc82.setAttribute("COMMAND",appPrefs.PrinterCommand);
	dc82.setAttribute("CLIPMARGIN", static_cast<int>(appPrefs.ClipMargin));
	dc82.setAttribute("GMODE", static_cast<int>(appPrefs.GCRMode));
	elem.appendChild(dc82);
	QDomElement dc8Pr=docu.createElement("PRINTPREVIEW");
	dc8Pr.setAttribute("Mode", static_cast<int>(appPrefs.PrPr_Mode));
	dc8Pr.setAttribute("GcrMode", static_cast<int>(appPrefs.Gcr_Mode));
	dc8Pr.setAttribute("AlphaText", static_cast<int>(appPrefs.PrPr_AlphaText));
	dc8Pr.setAttribute("AlphaGraphics", static_cast<int>(appPrefs.PrPr_AlphaGraphics));
	dc8Pr.setAttribute("Transparency", static_cast<int>(appPrefs.PrPr_Transparency));
	dc8Pr.setAttribute("Cyan", static_cast<int>(appPrefs.PrPr_C));
	dc8Pr.setAttribute("Magenta", static_cast<int>(appPrefs.PrPr_M));
	dc8Pr.setAttribute("Yellow", static_cast<int>(appPrefs.PrPr_Y));
	dc8Pr.setAttribute("Black", static_cast<int>(appPrefs.PrPr_K));
	elem.appendChild(dc8Pr);
	QDomElement dc8Ex = docu.createElement("EXTERNAL");
	dc8Ex.setAttribute("GIMP", imageEditorExecutable());
	dc8Ex.setAttribute("GS", ghostscriptExecutable());
	dc8Ex.setAttribute("AlphaGraphics", static_cast<int>(appPrefs.gs_AntiAliasGraphics));
	dc8Ex.setAttribute("AlphaText", static_cast<int>(appPrefs.gs_AntiAliasText));
	dc8Ex.setAttribute("Resolution", appPrefs.gs_Resolution);
	elem.appendChild(dc8Ex);
	QDomElement rde=docu.createElement("HYPHEN");
	rde.setAttribute("LANG", appPrefs.Language);
	rde.setAttribute("WORDLEN", appPrefs.MinWordLen);
	rde.setAttribute("HYCOUNT", appPrefs.HyCount);
	rde.setAttribute("MODE", static_cast<int>(appPrefs.Automatic));
	rde.setAttribute("INMODE", static_cast<int>(appPrefs.AutoCheck));
	elem.appendChild(rde);
	ColorList::Iterator itc;
	for (itc = appPrefs.DColors.begin(); itc != appPrefs.DColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		co.setAttribute("RGB",appPrefs.DColors[itc.key()].getRGBColor().name());
		co.setAttribute("CMYK",appPrefs.DColors[itc.key()].name());
		co.setAttribute("Spot",static_cast<int>(appPrefs.DColors[itc.key()].isSpotColor()));
		co.setAttribute("Register",static_cast<int>(appPrefs.DColors[itc.key()].isRegistrationColor()));
		elem.appendChild(co);
	}
	for ( SCFontsIterator itf(appPrefs.AvailFonts); itf.current(); ++itf)
	{
		QDomElement fn=docu.createElement("FONT");
		fn.setAttribute("NAME",itf.currentKey());
		fn.setAttribute("EMBED",static_cast<int>(itf.current()->EmbedPS));
		fn.setAttribute("USE", static_cast<int>(itf.current()->UseFont));
		fn.setAttribute("SUBSET", static_cast<int>(itf.current()->Subset));
		elem.appendChild(fn);
	}
	for (uint rd=0; rd<appPrefs.RecentDocs.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",appPrefs.RecentDocs[rd]);
		elem.appendChild(rde);
	}
	for (QMap<QString,Keys>::Iterator ksc=appPrefs.KeyActions.begin(); ksc!=appPrefs.KeyActions.end(); ++ksc)
	{
		QDomElement kscc=docu.createElement("SHORTCUT");
		kscc.setAttribute("ACTION",ksc.data().actionName);
		kscc.setAttribute("SEQUENCE",KeyManager::getKeyText(ksc.data().keySequence));
		elem.appendChild(kscc);
	}
	QMap<QString,QString>::Iterator itfsu;
	for (itfsu = appPrefs.GFontSub.begin(); itfsu != appPrefs.GFontSub.end(); ++itfsu)
	{
		QDomElement fosu = docu.createElement("Substitute");
		fosu.setAttribute("Name",itfsu.key());
		fosu.setAttribute("Replace",itfsu.data());
		elem.appendChild(fosu);
	}
	for (uint ccs=0; ccs<appPrefs.CustomColorSets.count(); ++ccs)
	{
		QDomElement cos=docu.createElement("COLORSET");
		cos.setAttribute("NAME",appPrefs.CustomColorSets[ccs]);
		elem.appendChild(cos);
	}
	QDomElement cosd=docu.createElement("DCOLORSET");
	cosd.setAttribute("NAME",appPrefs.DColorSet);
	elem.appendChild(cosd);
	QDomElement pdf = docu.createElement("PDF");
	pdf.setAttribute("Thumbnails", static_cast<int>(appPrefs.PDF_Options.Thumbnails));
	pdf.setAttribute("Articles", static_cast<int>(appPrefs.PDF_Options.Articles));
	pdf.setAttribute("Bookmarks", static_cast<int>(appPrefs.PDF_Options.Bookmarks));
	pdf.setAttribute("Compress", static_cast<int>(appPrefs.PDF_Options.Compress));
	pdf.setAttribute("CMethod", appPrefs.PDF_Options.CompressMethod);
	pdf.setAttribute("Quality", appPrefs.PDF_Options.Quality);
	pdf.setAttribute("MirrorH", static_cast<int>(appPrefs.PDF_Options.MirrorH));
	pdf.setAttribute("MirrorV", static_cast<int>(appPrefs.PDF_Options.MirrorV));
	pdf.setAttribute("RotateDeg", static_cast<int>(appPrefs.PDF_Options.RotateDeg));
	pdf.setAttribute("PresentMode", static_cast<int>(appPrefs.PDF_Options.PresentMode));
	pdf.setAttribute("RecalcPic", static_cast<int>(appPrefs.PDF_Options.RecalcPic));
	pdf.setAttribute("Grayscale", static_cast<int>(appPrefs.PDF_Options.isGrayscale));
	pdf.setAttribute("RGBMode", static_cast<int>(appPrefs.PDF_Options.UseRGB));
	pdf.setAttribute("UseProfiles", static_cast<int>(appPrefs.PDF_Options.UseProfiles));
	pdf.setAttribute("UseProfiles2", static_cast<int>(appPrefs.PDF_Options.UseProfiles2));
	pdf.setAttribute("Binding", appPrefs.PDF_Options.Binding);
	pdf.setAttribute("PicRes", appPrefs.PDF_Options.PicRes);
	pdf.setAttribute("Resolution", appPrefs.PDF_Options.Resolution);
	pdf.setAttribute("Version", appPrefs.PDF_Options.Version);
	pdf.setAttribute("Intent", appPrefs.PDF_Options.Intent);
	pdf.setAttribute("Intent2", appPrefs.PDF_Options.Intent2);
	pdf.setAttribute("SolidP", appPrefs.PDF_Options.SolidProf);
	pdf.setAttribute("ImageP", appPrefs.PDF_Options.ImageProf);
	pdf.setAttribute("PrintP", appPrefs.PDF_Options.PrintProf);
	pdf.setAttribute("InfoString", appPrefs.PDF_Options.Info);
	pdf.setAttribute("BTop", appPrefs.PDF_Options.BleedTop);
	pdf.setAttribute("BLeft", appPrefs.PDF_Options.BleedLeft);
	pdf.setAttribute("BRight", appPrefs.PDF_Options.BleedRight);
	pdf.setAttribute("BBottom", appPrefs.PDF_Options.BleedBottom);
	pdf.setAttribute("ImagePr", static_cast<int>(appPrefs.PDF_Options.EmbeddedI));
	pdf.setAttribute("PassOwner", appPrefs.PDF_Options.PassOwner);
	pdf.setAttribute("PassUser", appPrefs.PDF_Options.PassUser);
	pdf.setAttribute("Permissions", appPrefs.PDF_Options.Permissions);
	pdf.setAttribute("Encrypt", static_cast<int>(appPrefs.PDF_Options.Encrypt));
	pdf.setAttribute("UseLayers", static_cast<int>(appPrefs.PDF_Options.useLayers));
	pdf.setAttribute("UseLpi", static_cast<int>(appPrefs.PDF_Options.UseLPI));
	pdf.setAttribute("UseSpotColors", static_cast<int>(appPrefs.PDF_Options.UseSpotColors));
	pdf.setAttribute("doMultiFile", static_cast<int>(appPrefs.PDF_Options.doMultiFile));
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = appPrefs.PDF_Options.LPISettings.begin(); itlp != appPrefs.PDF_Options.LPISettings.end(); ++itlp)
	{
		QDomElement pdf4 = docu.createElement("LPI");
		pdf4.setAttribute("Color", itlp.key());
		pdf4.setAttribute("Frequency", itlp.data().Frequency);
		pdf4.setAttribute("Angle", itlp.data().Angle);
		pdf4.setAttribute("SpotFunction", itlp.data().SpotFunc);
		pdf.appendChild(pdf4);
	}
	elem.appendChild(pdf);
	QDomElement docItemAttrs = docu.createElement("DefaultItemAttributes");
	for(ObjAttrVector::Iterator objAttrIt = appPrefs.defaultItemAttributes.begin() ; objAttrIt != appPrefs.defaultItemAttributes.end(); ++objAttrIt )
	{
		QDomElement itemAttr = docu.createElement("ItemAttribute");
		itemAttr.setAttribute("Name", (*objAttrIt).name);
		itemAttr.setAttribute("Type", (*objAttrIt).type);
		itemAttr.setAttribute("Value", (*objAttrIt).value);
		itemAttr.setAttribute("Parameter", (*objAttrIt).parameter);
		itemAttr.setAttribute("Relationship", (*objAttrIt).relationship);
		itemAttr.setAttribute("RelationshipTo", (*objAttrIt).relationshipto);
		itemAttr.setAttribute("AutoAddTo", (*objAttrIt).autoaddto);
		docItemAttrs.appendChild(itemAttr);
	}
	elem.appendChild(docItemAttrs);
	QDomElement tocElem = docu.createElement("TablesOfContents");
	for(ToCSetupVector::Iterator tocSetupIt = appPrefs.defaultToCSetups.begin() ; tocSetupIt != appPrefs.defaultToCSetups.end(); ++tocSetupIt )
	{
		QDomElement tocsetup = docu.createElement("TableOfContents");
		tocsetup.setAttribute("Name", (*tocSetupIt).name);
		tocsetup.setAttribute("ItemAttributeName", (*tocSetupIt).itemAttrName);
		tocsetup.setAttribute("FrameName", (*tocSetupIt).frameName);
		tocsetup.setAttribute("ListNonPrinting", (*tocSetupIt).listNonPrintingFrames);
		tocsetup.setAttribute("Style", (*tocSetupIt).textStyle);
		tocsetup.setAttribute("NumberPlacement", (*tocSetupIt).pageLocation);
		tocElem.appendChild(tocsetup);
	}
	elem.appendChild(tocElem);
	// lorem ipsum
	QDomElement liElem = docu.createElement("LoremIpsum");
	liElem.setAttribute("useStandardLI", static_cast<int>(appPrefs.useStandardLI));
	liElem.setAttribute("paragraphsLI", appPrefs.paragraphsLI);
	elem.appendChild(liElem);
	// write file
	bool result = false;
	QFile f(ho);
	if(!f.open(IO_WriteOnly))
	{
		m_lastError = tr("Could not open preferences file \"%1\" for writing: %2")
			.arg(ho).arg(qApp->translate("QFile",f.errorString()));
	}
	else
	{
		QTextStream s(&f);
		s.setEncoding(QTextStream::UnicodeUTF8);
		s<<docu.toString();
		if (f.status() == IO_Ok)
			result = true;
		else
			m_lastError = tr("Writing to preferences file \"%1\" failed: "
				             "QIODevice status code %2")
				.arg(ho).arg(f.status());
	}
	if (f.isOpen())
		f.close();
	return result;
}

// Returns false on error. It's the caller's job to make sure the prefs file
// actually exists.
bool PrefsManager::ReadPref(QString ho)
{
	QDomDocument docu("scridoc");
	QFile f(ho);
	if(!f.open(IO_ReadOnly))
	{
		m_lastError = tr("Failed to open prefs file \"%1\": %2")
			.arg(ho).arg( qApp->translate("QFile",f.errorString()) );
		return false;
	}
	QTextStream ts(&f);
	ts.setEncoding(QTextStream::UnicodeUTF8);
	QString errorMsg;
	int errorLine = 0, errorColumn = 0;
	if( !docu.setContent(ts.read(), &errorMsg, &errorLine, &errorColumn) )
	{
		m_lastError = tr("Failed to read prefs XML from \"%1\": %2 at line %3, col %4")
			.arg(ho).arg(errorMsg).arg(errorLine).arg(errorColumn);
		f.close();
		return false;
	}
	f.close();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSRC")
		return false;
	appPrefs.DColors.clear();
	ScColor lf = ScColor();
	QDomNode DOC=elem.firstChild();
	if (!DOC.namedItem("CheckProfile").isNull())
		appPrefs.checkerProfiles.clear();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="GUI")
		{
			appPrefs.GUI = dc.attribute("STILT","Default");
			appPrefs.Wheelval = QStoInt(dc.attribute("RAD"));
			appPrefs.guidesSettings.grabRad = QStoInt(dc.attribute("GRAB","4"));
			appPrefs.docUnitIndex = QStoInt(dc.attribute("UNIT","0"));
			appPrefs.AppFontSize = QStoInt(dc.attribute("APF","12"));
			appPrefs.PaletteFontSize = QStoInt(dc.attribute("PFS", "10"));
			appPrefs.RecentDCount = dc.attribute("RCD","5").toUInt();
			appPrefs.DocDir = dc.attribute("DOC","");
			appPrefs.ProfileDir = dc.attribute("PROFILES","");
			appPrefs.ScriptDir = dc.attribute("SCRIPTS","");
			appPrefs.documentTemplatesDir = dc.attribute("TEMPLATES","");
			appPrefs.guidesSettings.guidesShown = static_cast<bool>(QStoInt(dc.attribute("SHOWGUIDES","1")));
			appPrefs.guidesSettings.framesShown = static_cast<bool>(QStoInt(dc.attribute("FRV","1")));
			appPrefs.guidesSettings.marginsShown = static_cast<bool>(QStoInt(dc.attribute("SHOWMARGIN","1")));
			appPrefs.guidesSettings.baseShown = static_cast<bool>(QStoInt(dc.attribute("SHOWBASE","1")));
			appPrefs.guidesSettings.linkShown = static_cast<bool>(QStoInt(dc.attribute("SHOWLINK","0")));
			appPrefs.guidesSettings.showPic = static_cast<bool>(QStoInt(dc.attribute("SHOWPICT","1")));
			appPrefs.guidesSettings.showControls = static_cast<bool>(QStoInt(dc.attribute("SHOWControl","0")));
			appPrefs.guidesSettings.rulerMode = static_cast<bool>(QStoInt(dc.attribute("rulerMode","1")));
			appPrefs.haveStylePreview = static_cast<bool>(QStoInt(dc.attribute("STYLEPREVIEW","1")));
			appPrefs.showStartupDialog = static_cast<bool>(QStoInt(dc.attribute("StartUp","1")));
			appPrefs.ScratchBottom = QStodouble(dc.attribute("ScratchBottom", "20"));
			appPrefs.ScratchLeft = QStodouble(dc.attribute("ScratchLeft", "100"));
			appPrefs.ScratchRight = QStodouble(dc.attribute("ScratchRight", "100"));
			appPrefs.ScratchTop = QStodouble(dc.attribute("ScratchTop", "20"));
			if (dc.hasAttribute("STECOLOR"))
				appPrefs.STEcolor = QColor(dc.attribute("STECOLOR"));
			if (dc.hasAttribute("STEFONT"))
				appPrefs.STEfont = dc.attribute("STEFONT");
		}
		if (dc.tagName()=="GRID")
		{
			appPrefs.guidesSettings.minorGrid = QStodouble(dc.attribute("MINOR"));
			appPrefs.guidesSettings.majorGrid = QStodouble(dc.attribute("MAJOR"));
			appPrefs.guidesSettings.minorColor = QColor(dc.attribute("MINORC"));
			appPrefs.guidesSettings.majorColor = QColor(dc.attribute("MAJORC"));
			appPrefs.guidesSettings.before = static_cast<bool>(QStoInt(dc.attribute("BACKG","1")));
			appPrefs.guidesSettings.gridShown = static_cast<bool>(QStoInt(dc.attribute("SHOW","0")));
			if (dc.hasAttribute("GuideC"))
				appPrefs.guidesSettings.guideColor = QColor(dc.attribute("GuideC"));
			if (dc.hasAttribute("GuideZ"))
				appPrefs.guidesSettings.guideRad = QStodouble(dc.attribute("GuideZ"));
			if (dc.hasAttribute("BaseC"))
				appPrefs.guidesSettings.baseColor = QColor(dc.attribute("BaseC"));
		}
		if (dc.tagName()=="PAGE")
		{
			appPrefs.DpapColor = QColor(dc.attribute("PAGEC"));
			appPrefs.guidesSettings.margColor = QColor(dc.attribute("MARGC","#0000ff"));
			appPrefs.marginColored = static_cast<bool>(QStoInt(dc.attribute("RANDF","0")));
			appPrefs.DisScale = QStodouble(dc.attribute("DScale","1"));
		}
		if (dc.tagName()=="TYPO")
		{
			appPrefs.typographicSettings.valueSuperScript = QStoInt(dc.attribute("HOCH"));
			appPrefs.typographicSettings.scalingSuperScript = QStoInt(dc.attribute("HOCHSC"));
			appPrefs.typographicSettings.valueSubScript = QStoInt(dc.attribute("TIEF"));
			appPrefs.typographicSettings.scalingSubScript = QStoInt(dc.attribute("TIEFSC"));
			appPrefs.typographicSettings.valueSmallCaps = QStoInt(dc.attribute("SMCAPS"));
			appPrefs.typographicSettings.valueBaseGrid = QStodouble(dc.attribute("BASE", "12"));
			appPrefs.typographicSettings.offsetBaseGrid = QStodouble(dc.attribute("BASEO", "0"));
			appPrefs.typographicSettings.autoLineSpacing = QStoInt(dc.attribute("AUTOL","20"));
			double ulp = QStodouble(dc.attribute("UnderlinePos","-1"));
			if (ulp != -1)
				appPrefs.typographicSettings.valueUnderlinePos = qRound(ulp * 10);
			else
				appPrefs.typographicSettings.valueUnderlinePos = -1;
			double ulw = QStodouble(dc.attribute("UnderlineWidth","-1"));
			if (ulw != -1)
				appPrefs.typographicSettings.valueUnderlineWidth = qRound(ulw * 10);
			else
				appPrefs.typographicSettings.valueUnderlineWidth = -1;
			double stp = QStodouble(dc.attribute("StrikeThruPos","-1"));
			if (stp != -1)
				appPrefs.typographicSettings.valueStrikeThruPos = qRound(ulp * 10);
			else
				appPrefs.typographicSettings.valueStrikeThruPos = -1;
			double stw = QStodouble(dc.attribute("StrikeThruWidth","-1"));
			if (stw != -1)
				appPrefs.typographicSettings.valueStrikeThruWidth = qRound(stw * 10);
			else
				appPrefs.typographicSettings.valueStrikeThruWidth = -1;
		}
		if (dc.tagName()=="TOOLS")
		{
			appPrefs.toolSettings.dPen = dc.attribute("PEN");
			appPrefs.toolSettings.dBrush = dc.attribute("BRUSH");
			appPrefs.toolSettings.dPenLine = dc.attribute("PENLINE");
			appPrefs.toolSettings.dPenText = dc.attribute("PENTEXT");
			appPrefs.toolSettings.dStrokeText = dc.attribute("StrokeText", appPrefs.toolSettings.dPenText);
			appPrefs.toolSettings.dTextBackGround = dc.attribute("TextBackGround", "None");
			appPrefs.toolSettings.dTextLineColor = dc.attribute("TextLineColor", "None");
			appPrefs.toolSettings.dTextBackGroundShade = QStoInt(dc.attribute("TextBackGroundShade", "100"));
			appPrefs.toolSettings.dTextLineShade = QStoInt(dc.attribute("TextLineShade", "100"));
			appPrefs.toolSettings.dTextPenShade = QStoInt(dc.attribute("TextPenShade", "100"));
			appPrefs.toolSettings.dTextStrokeShade = QStoInt(dc.attribute("TextStrokeShade", "100"));
			appPrefs.toolSettings.dCols = QStoInt(dc.attribute("TEXTCOL", "1"));
			appPrefs.toolSettings.dGap = QStodouble(dc.attribute("TEXTGAP", "0.0"));
			appPrefs.toolSettings.tabFillChar = dc.attribute("TabFill", "");
			appPrefs.toolSettings.dTabWidth = QStodouble(dc.attribute("TabWidth", "36.0"));
			appPrefs.toolSettings.dLineArt = QStoInt(dc.attribute("STIL"));
			appPrefs.toolSettings.dLstyleLine = QStoInt(dc.attribute("STILLINE"));
			appPrefs.toolSettings.dWidth = QStodouble(dc.attribute("WIDTH"));
			appPrefs.toolSettings.dWidthLine = QStodouble(dc.attribute("WIDTHLINE"));
			appPrefs.toolSettings.dShade2 = QStoInt(dc.attribute("PENSHADE"));
			appPrefs.toolSettings.dShadeLine = QStoInt(dc.attribute("LINESHADE"));
			appPrefs.toolSettings.dShade = QStoInt(dc.attribute("BRUSHSHADE"));
			appPrefs.toolSettings.magMin = QStoInt(dc.attribute("MAGMIN","10"));
			appPrefs.toolSettings.magMax = QStoInt(dc.attribute("MAGMAX","3200"));
			appPrefs.toolSettings.magStep = QStoInt(dc.attribute("MAGSTEP","25"));
			appPrefs.toolSettings.dBrushPict = dc.attribute("CPICT");
			appPrefs.toolSettings.shadePict = QStoInt(dc.attribute("PICTSHADE","100"));
			appPrefs.toolSettings.scaleX = QStodouble(dc.attribute("PICTSCX","1"));
			appPrefs.toolSettings.scaleY = QStodouble(dc.attribute("PICTSCY","1"));
			appPrefs.toolSettings.scaleType = static_cast<bool>(QStoInt(dc.attribute("PSCALE", "1")));
			appPrefs.toolSettings.aspectRatio = static_cast<bool>(QStoInt(dc.attribute("PASPECT", "0")));
			appPrefs.toolSettings.useEmbeddedPath = static_cast<bool>(QStoInt(dc.attribute("EmbeddedPath", "0")));
			appPrefs.toolSettings.lowResType = QStoInt(dc.attribute("HalfRes", "1"));
			appPrefs.toolSettings.polyC = QStoInt(dc.attribute("POLYC", "4"));
			appPrefs.toolSettings.polyF = QStodouble(dc.attribute("POLYF", "0.5"));
			appPrefs.toolSettings.polyR = QStodouble(dc.attribute("POLYR", "0"));
			appPrefs.toolSettings.polyFd = QStoInt(dc.attribute("POLYFD", "0"));
			appPrefs.toolSettings.polyS = static_cast<bool>(QStoInt(dc.attribute("POLYS", "0")));
			appPrefs.toolSettings.dStartArrow = QStoInt(dc.attribute("StartArrow", "0"));
			appPrefs.toolSettings.dEndArrow = QStoInt(dc.attribute("EndArrow", "0"));
		}
		if (dc.tagName()=="MAINWINDOW")
		{
			appPrefs.mainWinSettings.xPosition = QStoInt(dc.attribute("XPOS", "0"));
			appPrefs.mainWinSettings.yPosition = QStoInt(dc.attribute("YPOS", "0"));
			appPrefs.mainWinSettings.width = QStoInt(dc.attribute("WIDTH", "640"));
			appPrefs.mainWinSettings.height = QStoInt(dc.attribute("HEIGHT", "480"));
			QDesktopWidget *d = QApplication::desktop();
			QSize gStrut = QApplication::globalStrut();
			int minX = 0;
#ifndef QT_MAC
			int minY = 0;
#else
			// on Mac you're dead if the titlebar is not on screen
			int minY = 22;
#endif
			if (appPrefs.mainWinSettings.xPosition < minX )
				appPrefs.mainWinSettings.xPosition = minX;
			if (appPrefs.mainWinSettings.yPosition <  minY)
				appPrefs.mainWinSettings.yPosition = minY;
			int minWidth = 5*gStrut.width();
			int minHeight = 5*gStrut.width();
			int maxWidth = d->width();
			int maxHeight = d->height();
			if (appPrefs.mainWinSettings.width > maxWidth)
				appPrefs.mainWinSettings.width = maxWidth;
			if (appPrefs.mainWinSettings.width < minWidth)
				appPrefs.mainWinSettings.width = minWidth;
			if (appPrefs.mainWinSettings.height > maxHeight)
				appPrefs.mainWinSettings.height = maxHeight;
			if (appPrefs.mainWinSettings.height < minHeight)
				appPrefs.mainWinSettings.height = minHeight;
			int maxX = d->width() - minWidth;
			int maxY = d->height() - minHeight;
			if (appPrefs.mainWinSettings.xPosition >= maxX)
				appPrefs.mainWinSettings.xPosition = maxX;
			if (appPrefs.mainWinSettings.yPosition >= maxY)
				appPrefs.mainWinSettings.yPosition = maxY;
		}
		if (dc.tagName()=="PAGEPALETTE")
		{
			appPrefs.SepalT = static_cast<bool>(QStoInt(dc.attribute("THUMBS")));
			appPrefs.SepalN = static_cast<bool>(QStoInt(dc.attribute("NAMES")));
		}
		if (dc.tagName()=="SCRAPBOOK")
		{
			appPrefs.PSize = QStoInt(dc.attribute("PREVIEW"));
			appPrefs.SaveAtQ = static_cast<bool>(QStoInt(dc.attribute("SAVE")));
		}
		if (dc.tagName() == "DOKUMENT")
		{
			appPrefs.pageSize = dc.attribute("PAGESIZE","A4");
			appPrefs.pageOrientation = QStoInt(dc.attribute("AUSRICHTUNG","0"));
			appPrefs.PageWidth = QStodouble(dc.attribute("BREITE","595"));
			appPrefs.PageHeight = QStodouble(dc.attribute("HOEHE","842"));
			appPrefs.RandOben = QStodouble(dc.attribute("RANDO","9"));
			appPrefs.RandUnten = QStodouble(dc.attribute("RANDU","40"));
			appPrefs.RandLinks = QStodouble(dc.attribute("RANDL","9"));
			appPrefs.RandRechts = QStodouble(dc.attribute("RANDR","9"));
			appPrefs.FacingPages = QStoInt(dc.attribute("DOPPEL","0"));
			appPrefs.AutoSave = static_cast<bool>(QStoInt(dc.attribute("AutoSave","0")));
			appPrefs.AutoSaveTime = QStoInt(dc.attribute("AutoSaveTime","600000"));
		}
		if (dc.tagName()=="PageSets")
		{
			QDomNode PGS = DOC.firstChild();
			if  (!PGS.namedItem("PageNames").isNull())
			{
				appPrefs.pageSets.clear();
				while(!PGS.isNull())
				{
					QDomElement PgsAttr = PGS.toElement();
					if(PgsAttr.tagName() == "Set")
					{
						struct PageSet pageS;
						pageS.Name = PgsAttr.attribute("Name");
						pageS.FirstPage = QStoInt(PgsAttr.attribute("FirstPage","0"));
						pageS.Rows = QStoInt(PgsAttr.attribute("Rows","1"));
						pageS.Columns = QStoInt(PgsAttr.attribute("Columns","1"));
						pageS.GapHorizontal = QStodouble(PgsAttr.attribute("GapHorizontal","0"));
						pageS.GapVertical = QStodouble(PgsAttr.attribute("GapVertical","0"));
						pageS.GapBelow = QStodouble(PgsAttr.attribute("GapBelow","0"));
						pageS.pageNames.clear();
						QDomNode PGSN = PGS.firstChild();
						while(!PGSN.isNull())
						{
							QDomElement PgsAttrN = PGSN.toElement();
							if(PgsAttrN.tagName() == "PageNames")
								pageS.pageNames.append(PgsAttrN.attribute("Name"));
							PGSN = PGSN.nextSibling();
						}
						appPrefs.pageSets.append(pageS);
					}
					PGS = PGS.nextSibling();
				}
			}
		}
		if (dc.tagName()=="CMS")
		{
			appPrefs.DCMSset.SoftProofOn = static_cast<bool>(QStoInt(dc.attribute("DPSo","0")));
			appPrefs.DCMSset.CMSinUse = static_cast<bool>(QStoInt(dc.attribute("DPuse","0")));
			appPrefs.DCMSset.GamutCheck = static_cast<bool>(QStoInt(dc.attribute("DPgam","0")));
			appPrefs.DCMSset.BlackPoint = static_cast<bool>(QStoInt(dc.attribute("DPbla","1")));
			appPrefs.DCMSset.DefaultMonitorProfile = dc.attribute("DPMo","");
			appPrefs.DCMSset.DefaultPrinterProfile = dc.attribute("DPPr","");
			appPrefs.DCMSset.DefaultImageRGBProfile = dc.attribute("DPIn","");
			appPrefs.DCMSset.DefaultImageCMYKProfile = dc.attribute("DPInCMYK","");
			appPrefs.DCMSset.DefaultSolidColorProfile = dc.attribute("DPIn2","");
			appPrefs.DCMSset.DefaultIntentPrinter = QStoInt(dc.attribute("DIPr","0"));
			appPrefs.DCMSset.DefaultIntentMonitor = QStoInt(dc.attribute("DIMo","3"));
			appPrefs.DCMSset.DefaultIntentImages = QStoInt(dc.attribute("DIMo2","3"));
		}
		if (!importingFrom12 && dc.tagName()=="SHORTCUT")
		{
			if (appPrefs.KeyActions.contains(dc.attribute("ACTION")))
			{
				appPrefs.KeyActions[dc.attribute("ACTION")].actionName = dc.attribute("ACTION");
				QKeySequence newKeySequence = QKeySequence(dc.attribute("SEQUENCE"));
//				qDebug(QString("reading shortcut for %2 %1").arg(QString(newKeySequence)).arg(dc.attribute("ACTION")));
				appPrefs.KeyActions[dc.attribute("ACTION")].keySequence = newKeySequence;
			}
		}
		if (dc.tagName()=="RECENT")
			appPrefs.RecentDocs.append(dc.attribute("NAME"));
		if (dc.tagName()=="Checker")
		{
			appPrefs.curCheckProfile = dc.attribute("currentProfile", tr("PostScript"));
			//#2516 work around old values until people wont have them anymore, not that these
			//translated strings should be going into prefs anyway!
			if (appPrefs.curCheckProfile == tr("Postscript"))
				appPrefs.curCheckProfile == tr("PostScript");
		}
		if (dc.tagName()=="CheckProfile")
		{
			QString name=dc.attribute("Name");
			struct checkerPrefs checkerSettings;
			checkerSettings.ignoreErrors = static_cast<bool>(QStoInt(dc.attribute("ignoreErrors", "0")));
			checkerSettings.autoCheck = static_cast<bool>(QStoInt(dc.attribute("autoCheck", "1")));
			checkerSettings.checkGlyphs = static_cast<bool>(QStoInt(dc.attribute("checkGlyphs", "1")));
			checkerSettings.checkOrphans = static_cast<bool>(QStoInt(dc.attribute("checkOrphans", "1")));
			checkerSettings.checkOverflow = static_cast<bool>(QStoInt(dc.attribute("checkOverflow", "1")));
			checkerSettings.checkPictures = static_cast<bool>(QStoInt(dc.attribute("checkPictures", "1")));
			checkerSettings.checkResolution = static_cast<bool>(QStoInt(dc.attribute("checkResolution", "1")));
			checkerSettings.checkTransparency = static_cast<bool>(QStoInt(dc.attribute("checkTransparency", "1")));
			checkerSettings.minResolution = QStodouble(dc.attribute("minResolution","72"));
			checkerSettings.checkAnnotations = static_cast<bool>(QStoInt(dc.attribute("checkAnnotations", "0")));
			checkerSettings.checkRasterPDF = static_cast<bool>(QStoInt(dc.attribute("checkRasterPDF", "1")));
			appPrefs.checkerProfiles[name] = checkerSettings;
		}
		if (dc.tagName()=="PRINTER")
		{
			appPrefs.PrinterName = dc.attribute("NAME");
			appPrefs.PrinterFile = dc.attribute("FILE");
			appPrefs.PrinterCommand = dc.attribute("COMMAND");
			appPrefs.ClipMargin = static_cast<bool>(QStoInt(dc.attribute("CLIPMARGIN", "1")));
			appPrefs.GCRMode = static_cast<bool>(QStoInt(dc.attribute("GMODE", "1")));
		}
		if (dc.tagName()=="PRINTPREVIEW")
		{
			appPrefs.PrPr_Mode = static_cast<bool>(QStoInt(dc.attribute("Mode", "0")));
			appPrefs.Gcr_Mode = static_cast<bool>(QStoInt(dc.attribute("GcrMode", "1")));
			appPrefs.PrPr_AlphaText = static_cast<bool>(QStoInt(dc.attribute("AlphaText", "0")));
			appPrefs.PrPr_AlphaGraphics = static_cast<bool>(QStoInt(dc.attribute("AlphaGraphics", "0")));
			appPrefs.PrPr_Transparency = static_cast<bool>(QStoInt(dc.attribute("Transparency", "0")));
			appPrefs.PrPr_C = static_cast<bool>(QStoInt(dc.attribute("Cyan", "1")));
			appPrefs.PrPr_M = static_cast<bool>(QStoInt(dc.attribute("Magenta", "1")));
			appPrefs.PrPr_Y = static_cast<bool>(QStoInt(dc.attribute("Yellow", "1")));
			appPrefs.PrPr_K = static_cast<bool>(QStoInt(dc.attribute("Black", "1")));
		}
		if (dc.tagName()=="EXTERNAL")
		{
			setGhostscriptExecutable(dc.attribute("GS", "gs"));
			appPrefs.gs_AntiAliasText = static_cast<bool>(QStoInt(dc.attribute("AlphaText", "0")));
			appPrefs.gs_AntiAliasGraphics = static_cast<bool>(QStoInt(dc.attribute("AlphaGraphics", "0")));
			appPrefs.gs_Resolution = QStoInt(dc.attribute("Resolution", "72"));
			setImageEditorExecutable(dc.attribute("GIMP", "gimp"));
		}
		if (dc.tagName()=="HYPHEN")
		{
			if (!dc.attribute("LANG", "").isEmpty())
				appPrefs.Language = dc.attribute("LANG");
			appPrefs.MinWordLen = QStoInt(dc.attribute("WORDLEN", "3"));
			appPrefs.HyCount = QStoInt(dc.attribute("HYCOUNT", "2"));
			appPrefs.Automatic = static_cast<bool>(QStoInt(dc.attribute("MODE", "1")));
			appPrefs.AutoCheck = static_cast<bool>(QStoInt(dc.attribute("INMODE", "1")));
		}
		if (dc.tagName()=="FONTS")
		{
			QString tmpf = dc.attribute("FACE");
			QString newFont = "";
			if (!appPrefs.AvailFonts.find(tmpf))
			{
				ScApp->showSplash(false);
				MissingFont *dia = new MissingFont(0, tmpf, 0);
				dia->exec();
				newFont = dia->getReplacementFont();
				delete dia;
			}
			else
				newFont = dc.attribute("FACE");
			if (!newFont.isEmpty())
				appPrefs.toolSettings.defFont = newFont;
			appPrefs.toolSettings.defSize = qRound(QStodouble(dc.attribute("SIZE")) * 10.0);
			appPrefs.askBeforeSubstituite = static_cast<bool>(QStoInt(dc.attribute("AutomaticSubst", "1")));
		}
		if (dc.tagName()=="FONT")
		{
			if (appPrefs.AvailFonts.find(dc.attribute("NAME")))
			{
				appPrefs.AvailFonts[dc.attribute("NAME")]->EmbedPS = static_cast<bool>(QStoInt(dc.attribute("EMBED")));
				appPrefs.AvailFonts[dc.attribute("NAME")]->UseFont &= static_cast<bool>(QStoInt(dc.attribute("USE","1")));
				appPrefs.AvailFonts[dc.attribute("NAME")]->Subset = static_cast<bool>(QStoInt(dc.attribute("SUBSET","0")));
			}
		}
		if (dc.tagName()=="COLOR")
		{
			if (dc.hasAttribute("CMYK"))
				lf.setNamedColor(dc.attribute("CMYK"));
			else
				lf.fromQColor(QColor(dc.attribute("RGB")));
			if (dc.hasAttribute("Spot"))
				lf.setSpotColor(static_cast<bool>(QStoInt(dc.attribute("Spot"))));
			else
				lf.setSpotColor(false);
			if (dc.hasAttribute("Register"))
				lf.setRegistrationColor(static_cast<bool>(QStoInt(dc.attribute("Register"))));
			else
				lf.setRegistrationColor(false);
		  appPrefs.DColors[dc.attribute("NAME")] = lf;
		}
		if (dc.tagName()=="Substitute")
		  appPrefs.GFontSub[dc.attribute("Name")] = dc.attribute("Replace");
		if (dc.tagName()=="COLORSET")
			appPrefs.CustomColorSets.append(dc.attribute("NAME"));
		if (dc.tagName()=="DCOLORSET")
			appPrefs.DColorSet = dc.attribute("NAME");
		if(dc.tagName()=="PDF")
		{
			appPrefs.PDF_Options.Articles = static_cast<bool>(QStoInt(dc.attribute("Articles")));
			appPrefs.PDF_Options.Thumbnails = static_cast<bool>(QStoInt(dc.attribute("Thumbnails")));
			appPrefs.PDF_Options.Compress = static_cast<bool>(QStoInt(dc.attribute("Compress")));
			appPrefs.PDF_Options.CompressMethod = QStoInt(dc.attribute("CMethod","0"));
			appPrefs.PDF_Options.Quality = QStoInt(dc.attribute("Quality","0"));
			appPrefs.PDF_Options.RecalcPic = static_cast<bool>(QStoInt(dc.attribute("RecalcPic")));
			appPrefs.PDF_Options.Bookmarks = static_cast<bool>(QStoInt(dc.attribute("Bookmarks")));
			appPrefs.PDF_Options.MirrorH = static_cast<bool>(QStoInt(dc.attribute("MirrorH")));
			appPrefs.PDF_Options.MirrorV = static_cast<bool>(QStoInt(dc.attribute("MirrorV")));
			appPrefs.PDF_Options.RotateDeg = QStoInt(dc.attribute("RotateDeg","0"));
			appPrefs.PDF_Options.PresentMode = static_cast<bool>(QStoInt(dc.attribute("PresentMode")));
			appPrefs.PDF_Options.PicRes = QStoInt(dc.attribute("PicRes"));
			appPrefs.PDF_Options.Version = (PDFOptions::PDFVersion)QStoInt(dc.attribute("Version"));
			appPrefs.PDF_Options.Resolution = QStoInt(dc.attribute("Resolution"));
			appPrefs.PDF_Options.Binding = QStoInt(dc.attribute("Binding"));
			appPrefs.PDF_Options.Datei = "";
			appPrefs.PDF_Options.isGrayscale = static_cast<bool>(QStoInt(dc.attribute("Grayscale","0")));
			appPrefs.PDF_Options.UseRGB = static_cast<bool>(QStoInt(dc.attribute("RGBMode","0")));
			appPrefs.PDF_Options.UseProfiles = static_cast<bool>(QStoInt(dc.attribute("UseProfiles","0")));
			appPrefs.PDF_Options.UseProfiles2 = static_cast<bool>(QStoInt(dc.attribute("UseProfiles2","0")));
			appPrefs.PDF_Options.Intent = QStoInt(dc.attribute("Intent","1"));
			appPrefs.PDF_Options.Intent2 = QStoInt(dc.attribute("Intent2","1"));
			appPrefs.PDF_Options.SolidProf = dc.attribute("SolidP", "");
			appPrefs.PDF_Options.ImageProf = dc.attribute("ImageP", "");
			appPrefs.PDF_Options.PrintProf = dc.attribute("PrintP", "");
			appPrefs.PDF_Options.Info = dc.attribute("InfoString", "");
			appPrefs.PDF_Options.BleedTop = QStodouble(dc.attribute("BTop","0"));
			appPrefs.PDF_Options.BleedLeft = QStodouble(dc.attribute("BLeft","0"));
			appPrefs.PDF_Options.BleedRight = QStodouble(dc.attribute("BRight","0"));
			appPrefs.PDF_Options.BleedBottom = QStodouble(dc.attribute("BBottom","0"));
			appPrefs.PDF_Options.EmbeddedI = static_cast<bool>(QStoInt(dc.attribute("ImagePr","0")));
			appPrefs.PDF_Options.PassOwner = dc.attribute("PassOwner", "");
			appPrefs.PDF_Options.PassUser = dc.attribute("PassUser", "");
			appPrefs.PDF_Options.Permissions = QStoInt(dc.attribute("Permissions","-4"));
			appPrefs.PDF_Options.Encrypt = static_cast<bool>(QStoInt(dc.attribute("Encrypt","0")));
			appPrefs.PDF_Options.useLayers = static_cast<bool>(QStoInt(dc.attribute("UseLayers","0")));
			appPrefs.PDF_Options.UseLPI = static_cast<bool>(QStoInt(dc.attribute("UseLpi","0")));
			appPrefs.PDF_Options.UseSpotColors = static_cast<bool>(QStoInt(dc.attribute("UseSpotColors","1")));
			appPrefs.PDF_Options.doMultiFile = static_cast<bool>(QStoInt(dc.attribute("doMultiFile","0")));
			QDomNode PFO = DOC.firstChild();
			while(!PFO.isNull())
			{
				QDomElement pdfF = PFO.toElement();
				if(pdfF.tagName() == "LPI")
				{
					struct LPIData lpo;
					lpo.Angle = QStoInt(pdfF.attribute("Angle"));
					lpo.Frequency = QStoInt(pdfF.attribute("Frequency"));
					lpo.SpotFunc = QStoInt(pdfF.attribute("SpotFunction"));
					appPrefs.PDF_Options.LPISettings[pdfF.attribute("Color")] = lpo;
				}
				PFO = PFO.nextSibling();
			}
		}
		if(dc.tagName()=="DefaultItemAttributes")
		{
			QDomNode DIA = DOC.firstChild();
			appPrefs.defaultItemAttributes.clear();
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
					appPrefs.defaultItemAttributes.append(objattr);
				}
				DIA = DIA.nextSibling();
			}
		}
		if(dc.tagName()=="TablesOfContents")
		{
			QDomNode TOC = DOC.firstChild();
			appPrefs.defaultToCSetups.clear();
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
					appPrefs.defaultToCSetups.append(tocsetup);
				}
				TOC = TOC.nextSibling();
			}
		}
		// lorem ispum
		if (dc.tagName() == "LoremIpsum")
		{
			appPrefs.useStandardLI = static_cast<bool>(QStoInt(dc.attribute("useStandardLI", "0")));
			appPrefs.paragraphsLI = QStoInt(dc.attribute("paragraphsLI", "10"));
		}
		DOC=DOC.nextSibling();
	}
	if (appPrefs.GUI != "Default")
		qApp->setStyle(QStyleFactory::create(appPrefs.GUI));
	QFont apf = qApp->font();
	apf.setPointSize(appPrefs.AppFontSize);
	qApp->setFont(apf,true);
	return true;
}

void PrefsManager::initDefaultCheckerPrefs(CheckerPrefsList* cp)
{
	if (cp!=NULL)
	{
		struct checkerPrefs checkerSettings;
		checkerSettings.ignoreErrors = false;
		checkerSettings.autoCheck = true;
		checkerSettings.checkGlyphs = true;
		checkerSettings.checkOrphans = true;
		checkerSettings.checkOverflow = true;
		checkerSettings.checkPictures = true;
		checkerSettings.checkResolution = true;
		checkerSettings.checkTransparency = true;
		checkerSettings.checkAnnotations = false;
		checkerSettings.checkRasterPDF = true;
		checkerSettings.minResolution = 72.0;
		//TODO Stop translating these into settings!!!!!!!!!
		cp->insert( QT_TR_NOOP("PostScript"), checkerSettings);
		cp->insert( QT_TR_NOOP("PDF 1.3"), checkerSettings);
		checkerSettings.checkTransparency = false;
		cp->insert( QT_TR_NOOP("PDF 1.4"), checkerSettings);
		checkerSettings.checkTransparency = true;
		checkerSettings.checkAnnotations = true;
		checkerSettings.minResolution = 144.0;
		cp->insert( QT_TR_NOOP("PDF/X-3"), checkerSettings);
	}
}

const QString & PrefsManager::lastError() const
{
	return m_lastError;
}

// It's hard to say whether this should be here and called from SavePrefs, or
// triggered by a signal sent from here and displayed by ScribusApp.
void PrefsManager::alertSavePrefsFailed() const
{
	QMessageBox::critical(ScApp, tr("Error Writing Preferences"),
			"<qt>" +
			tr("Scribus was not able to save its preferences:<br>"
			   "%1<br>"
			   "Please check file and directory permissions and "
			   "available disk space.", "scribus app error")
			   .arg(lastError())
			+ "</qt>",
			QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
			QMessageBox::NoButton);
}

// It's hard to say whether this should be here and called from ReadPrefs, or
// triggered by a signal sent from here and displayed by ScribusApp.
void PrefsManager::alertLoadPrefsFailed() const
{
	bool splashShowing = ScApp->splashShowing();
	if (splashShowing)
		ScApp->showSplash(false);
	QMessageBox::critical(ScApp, tr("Error Loading Preferences"),
			"<qt>" +
			tr("Scribus was not able to load its preferences:<br>"
			   "%1<br>"
			   "Default settings will be loaded.")
			   .arg(lastError())
			+ "</qt>",
			QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
			QMessageBox::NoButton);
	ScApp->showSplash(splashShowing);
}

const int PrefsManager::gsResolution()
{
	return appPrefs.gs_Resolution;
}
