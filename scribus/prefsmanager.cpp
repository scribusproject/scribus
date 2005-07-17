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
#include <qfile.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qwmatrix.h>

#include "prefsmanager.h"

#include "pdfoptions.h"
#include "prefsfile.h"
#include "scfonts.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusapp.h"
#include "util.h"

extern ScribusApp* ScApp;
extern ScribusQApp* ScQApp;

PrefsManager* PrefsManager::_instance = 0;

PrefsManager::PrefsManager(QObject *parent, const char *name)
 : QObject(parent, name)
{
}

PrefsManager::~PrefsManager()
{
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
		appPrefs.DColors.insert("White", CMYKColor(0, 0, 0, 0));
		appPrefs.DColors.insert("Black", CMYKColor(0, 0, 0, 255));
		appPrefs.DColors.insert("Blue", CMYKColor(255, 255, 0, 0));
		appPrefs.DColors.insert("Cyan", CMYKColor(255, 0, 0, 0));
		appPrefs.DColors.insert("Green", CMYKColor(255, 0, 255, 0));
		appPrefs.DColors.insert("Red", CMYKColor(0, 255, 255, 0));
		appPrefs.DColors.insert("Yellow", CMYKColor(0, 0, 255, 0));
		appPrefs.DColors.insert("Magenta", CMYKColor(0, 255, 0, 0));
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
				CMYKColor tmp;
				tmp.setColorRGB(Rval, Gval, Bval);
				appPrefs.DColors.insert(Cname, tmp);
			}
			fiC.close();
		}
		appPrefs.DColorSet = "X11 RGB-Set";
	}

	appPrefs.Wheelval = 40;
	appPrefs.guidesSettings.marginsShown = true;
	appPrefs.guidesSettings.framesShown = true;
	appPrefs.guidesSettings.gridShown = false;
	appPrefs.guidesSettings.guidesShown = false;
	appPrefs.guidesSettings.baseShown = false;
	appPrefs.guidesSettings.showPic = true;
	appPrefs.guidesSettings.showControls = false;
	appPrefs.guidesSettings.linkShown = false;
	appPrefs.guidesSettings.grabRad = 4;
	appPrefs.guidesSettings.guideRad = 10;
	appPrefs.guidesSettings.minorGrid = 20;
	appPrefs.guidesSettings.majorGrid = 100;
	appPrefs.guidesSettings.minorColor = QColor(green);
	appPrefs.guidesSettings.majorColor = QColor(green);
	appPrefs.guidesSettings.margColor = QColor(blue);
	appPrefs.guidesSettings.guideColor = QColor(darkBlue);
	appPrefs.guidesSettings.baseColor = QColor(lightGray);
	appPrefs.typographicSetttings.valueSuperScript = 33;
	appPrefs.typographicSetttings.scalingSuperScript = 100;
	appPrefs.typographicSetttings.valueSubScript = 33;
	appPrefs.typographicSetttings.scalingSubScript = 100;
	appPrefs.typographicSetttings.valueSmallCaps = 75;
	appPrefs.typographicSetttings.autoLineSpacing = 20;
	appPrefs.typographicSetttings.valueUnderlinePos = -1;
	appPrefs.typographicSetttings.valueUnderlineWidth = -1;
	appPrefs.typographicSetttings.valueStrikeThruPos = -1;
	appPrefs.typographicSetttings.valueStrikeThruWidth = -1;
	appPrefs.typographicSetttings.valueBaseGrid = 14.4;
	appPrefs.typographicSetttings.offsetBaseGrid = 0.0;
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
	appPrefs.PageWidth = 595;
	appPrefs.PageHeight = 842;
	appPrefs.RandOben = 40;
	appPrefs.RandUnten = 40;
	appPrefs.RandLinks = 40;
	appPrefs.RandRechts = 40;
	appPrefs.FacingPages = false;
	appPrefs.LeftPageFirst = false;
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
#ifndef _WIN32
 	appPrefs.gs_exe = "gs";
#else
	appPrefs.gs_exe = "gswin32c.exe";
#endif
	appPrefs.gs_Resolution = 72;
	appPrefs.STEcolor = QColor(white);
	appPrefs.DCMSset.DefaultMonitorProfile = "";
	appPrefs.DCMSset.DefaultPrinterProfile = "";
	appPrefs.DCMSset.DefaultInputProfile = "";
	appPrefs.DCMSset.DefaultInputProfile2 = "";
	appPrefs.DCMSset.CMSinUse = false;
	appPrefs.DCMSset.SoftProofOn = false;
	appPrefs.DCMSset.GamutCheck = false;
	appPrefs.DCMSset.BlackPoint = true;
	appPrefs.DCMSset.DefaultIntentMonitor = 1;
	appPrefs.DCMSset.DefaultIntentMonitor2 = 1;
	appPrefs.DCMSset.DefaultIntentPrinter = 0;
	appPrefs.GFontSub.clear();
	appPrefs.ScratchLeft = 100;
	appPrefs.ScratchRight = 100;
	appPrefs.ScratchTop = 20;
	appPrefs.ScratchBottom = 20;
	appPrefs.askBeforeSubstituite = true;
	appPrefs.haveStylePreview = true;
	// lorem ipsum defaults
	appPrefs.useStandardLI = false;
	appPrefs.paragraphsLI = 10;
	appPrefs.showStartupDialog = true;
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
	appPrefs.checkerProfiles.insert( tr("Postscript"), checkerSettings);
	appPrefs.checkerProfiles.insert( tr("PDF 1.3"), checkerSettings);
	checkerSettings.checkTransparency = false;
	appPrefs.checkerProfiles.insert( tr("PDF 1.4"), checkerSettings);
	checkerSettings.checkTransparency = true;
	checkerSettings.checkAnnotations = true;
	checkerSettings.minResolution = 144.0;
	appPrefs.checkerProfiles.insert( tr("PDF/X-3"), checkerSettings);
	appPrefs.curCheckProfile = tr("Postscript");
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

	//Attribute setup
	appPrefs.defaultItemAttributes.clear();
	appPrefs.defaultToCSetups.clear();
}

void PrefsManager::initDefaultGUIFont(const QFont& guiFont)
{
	appPrefs.AppFontSize = guiFont.pointSize();	
	appPrefs.STEfont = guiFont.toString();	
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
	oldPR[0]=QDir::convertSeparators(prefsLocation+"/scribus.rc");
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
	if( (existsOldPR[0] && !existsNewPR[0]) || (existsOldPR[3] && !existsNewPR[3]) )
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
void PrefsManager::convert12Preferences(PrefsFile &prefsFile)
{
	// Import 1.2 font search path prefs
	QFile fontPrefsFile12(QDir::convertSeparators(prefsLocation+"/scribusfont.rc"));
	if (fontPrefsFile12.open(IO_ReadOnly))
	{
		PrefsContext *pc = prefsFile.getContext("Fonts");
		PrefsTable *fontPrefs = pc->getTable("ExtraFontDirs");
		QTextStream tsx(&fontPrefsFile12);
		QString extraPath = tsx.read();
		fontPrefsFile12.close();
		QStringList extraFonts = QStringList::split("\n",extraPath);
		for (uint i = 0; i < extraFonts.count(); ++i)
			fontPrefs->set(i, 0, extraFonts[i]);
	}
}
