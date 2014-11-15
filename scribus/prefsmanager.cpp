/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QColor>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QList>
#include <QMatrix>
#include <QString>
#include <QStringList>
#include <QStyleFactory>

#include "prefsmanager.h"

#include "actionmanager.h"
#include "colormgmt/sccolormgmtstructs.h"
#include "colorsetmanager.h"
#include "commonstrings.h"
#include "filewatcher.h"
#include "latexhelpers.h"
#include "missing.h"
#include "tabkeyboardshortcutswidget.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "pdfoptions.h"
#include "prefsfile.h"
#include "scclocale.h"
#include "scfonts.h"
#include "scmessagebox.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "ui/modetoolbar.h"
#include "util_file.h"
#include "util_ghostscript.h"

extern bool emergencyActivated;

PrefsManager* PrefsManager::_instance = 0;

PrefsManager::PrefsManager(QObject *parent)
 : QObject(parent),
 firstTimeIgnoreOldPrefs(false)
{
}

PrefsManager::~PrefsManager()
{
	appPrefs.AvailFonts.clear();
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

bool PrefsManager::importingFrom12x()
{
	return importingFrom12;
}


void PrefsManager::setup()
{
	setupPreferencesLocation();

	importingFrom12=copyOldPreferences();
	prefsFile = new PrefsFile( prefsLocation + "/prefs140.xml" );
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
	bool goodFont = false;
	for ( SCFontsIterator itf(appPrefs.AvailFonts); itf.hasNext(); itf.next())
	{
		if ((itf.currentKey() == "Arial Regular") || (itf.currentKey() == "Times New Roman Regular"))
		{
			appPrefs.toolSettings.defFont = itf.currentKey();
			goodFont = true;
			break;
		}
	}
	if (!goodFont)
		appPrefs.toolSettings.defFont = it.currentKey();
	appPrefs.toolSettings.defSize = 120;

	/** Default colours **/
	appPrefs.DColors.clear();
	
	ColorSetManager csm;
	csm.initialiseDefaultPrefs(appPrefs);
	csm.findPaletteLocations();
	csm.findPalettes();
	/*
	QString pfadC = ScPaths::instance().shareDir()+"swatches/";
	QString pfadC2 = pfadC + "Scribus_X11.txt";
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
	*/

	appPrefs.Wheelval = 40;
	/** Set Default window position and size to sane default values which should work on every screen */
//	appPrefs.mainWinSettings.xPosition = 0;
//	appPrefs.mainWinSettings.yPosition = 0;
	appPrefs.mainWinSettings.width = 640;
	appPrefs.mainWinSettings.height = 480;
	QDesktopWidget *d = QApplication::desktop();
	appPrefs.mainWinSettings.xPosition=(d->availableGeometry().width()-appPrefs.mainWinSettings.width)/2;
	appPrefs.mainWinSettings.yPosition=(d->availableGeometry().height()-appPrefs.mainWinSettings.height)/2;
	appPrefs.mainWinSettings.maximized = false;
	appPrefs.mainWinState = QByteArray();
	appPrefs.guidesSettings.marginsShown = true;
	appPrefs.guidesSettings.framesShown = true;
	appPrefs.guidesSettings.layerMarkersShown = false;
	appPrefs.guidesSettings.gridShown = false;
	appPrefs.guidesSettings.guidesShown = true;
	appPrefs.guidesSettings.colBordersShown = true;
	appPrefs.guidesSettings.baseShown = false;
	appPrefs.guidesSettings.showPic = true;
	appPrefs.guidesSettings.showControls = false;
	appPrefs.guidesSettings.linkShown = false;
	appPrefs.guidesSettings.rulersShown = true;
	appPrefs.guidesSettings.showBleed = true;
	appPrefs.guidesSettings.rulerMode = true;
	appPrefs.guidesSettings.grabRad = 4;
	appPrefs.guidesSettings.guideRad = 10;
	appPrefs.guidesSettings.minorGrid = 20;
	appPrefs.guidesSettings.majorGrid = 100;
	appPrefs.guidesSettings.minorColor = QColor(Qt::green);
	appPrefs.guidesSettings.majorColor = QColor(Qt::green);
	appPrefs.guidesSettings.margColor = QColor(Qt::blue);
	appPrefs.guidesSettings.guideColor = QColor(Qt::darkBlue);
	appPrefs.guidesSettings.baseColor = QColor(Qt::lightGray);
	appPrefs.typographicSettings.valueSuperScript = 33;
	appPrefs.typographicSettings.scalingSuperScript = 66;
	appPrefs.typographicSettings.valueSubScript = 33;
	appPrefs.typographicSettings.scalingSubScript = 66;
	appPrefs.typographicSettings.valueSmallCaps = 75;
	appPrefs.typographicSettings.autoLineSpacing = 20;
	appPrefs.typographicSettings.valueUnderlinePos = -1;
	appPrefs.typographicSettings.valueUnderlineWidth = -1;
	appPrefs.typographicSettings.valueStrikeThruPos = -1;
	appPrefs.typographicSettings.valueStrikeThruWidth = -1;
	appPrefs.typographicSettings.valueBaseGrid = 14.4;
	appPrefs.typographicSettings.offsetBaseGrid = 0.0;
	appPrefs.GUI = "";
	appPrefs.grayscaleIcons = false; // can be a little slower on startup.. but its a nice effect to play with
	appPrefs.showToolTips = true;
	appPrefs.showMouseCoordinates = true;
	appPrefs.moveTimeout = 150;
	appPrefs.stickyTools = false;
	//FIXME
	//Black here causes issues when a colour set is loaded without "Black" in it.
	//"Black" is created with wrong values. Eg SVG colour set
	appPrefs.toolSettings.dPen = "Black";
	appPrefs.toolSettings.dBrush = CommonStrings::None;
	appPrefs.toolSettings.dShade = 100;
	appPrefs.toolSettings.dShade2 = 100;
	appPrefs.toolSettings.dLineArt = Qt::SolidLine;
	appPrefs.toolSettings.dWidth = 1;
	appPrefs.toolSettings.dPenLine = "Black";
	appPrefs.toolSettings.dPenText = "Black";
	appPrefs.toolSettings.dStrokeText = "Black";
	appPrefs.toolSettings.dTextBackGround = CommonStrings::None;
	appPrefs.toolSettings.dTextLineColor = CommonStrings::None;
	appPrefs.toolSettings.dTextBackGroundShade = 100;
	appPrefs.toolSettings.dTextLineShade = 100;
	appPrefs.toolSettings.dTextPenShade = 100;
	appPrefs.toolSettings.dTextStrokeShade = 100;
	appPrefs.toolSettings.tabFillChar = "";
	appPrefs.toolSettings.dTabWidth = 36.0;
	appPrefs.toolSettings.dispX = 10.0;
	appPrefs.toolSettings.dispY = 10.0;
	appPrefs.toolSettings.constrain = 15.0;
	appPrefs.DpapColor = QColor(Qt::white);
	appPrefs.showPageShadow = true;
	appPrefs.DFrameColor = QColor(Qt::red);
	appPrefs.DFrameNormColor = QColor(Qt::black);
	appPrefs.DFrameGroupColor = QColor(Qt::darkCyan);
	appPrefs.DFrameLockColor = QColor(Qt::darkRed);
	appPrefs.DFrameLinkColor = QColor(Qt::red);
	appPrefs.DFrameAnnotationColor = QColor(Qt::blue);
	appPrefs.DPageBorderColor = QColor(Qt::red);
	appPrefs.DControlCharColor = QColor(Qt::darkRed);
	appPrefs.toolSettings.dCols = 1;
	appPrefs.toolSettings.dGap = 0.0;
	appPrefs.toolSettings.dShadeLine = 100;
	appPrefs.toolSettings.dLstyleLine = Qt::SolidLine;
	appPrefs.toolSettings.dWidthLine = 1;
	appPrefs.toolSettings.dStartArrow = 0;
	appPrefs.toolSettings.dEndArrow = 0;
	appPrefs.toolSettings.magMin = 10;
	appPrefs.toolSettings.magMax = 3200;
	appPrefs.toolSettings.magStep = 200;
	appPrefs.toolSettings.dBrushPict = CommonStrings::None;
	appPrefs.toolSettings.shadePict = 100;
	appPrefs.toolSettings.scaleX = 1;
	appPrefs.toolSettings.scaleY = 1;
	appPrefs.guidesSettings.before = true;
	appPrefs.docUnitIndex = 0;
	appPrefs.toolSettings.polyC = 4;
	appPrefs.toolSettings.polyF = 0.5;
	appPrefs.toolSettings.polyS = false;
	appPrefs.toolSettings.polyFd = 0;
	appPrefs.toolSettings.polyR = 0.0;
	appPrefs.toolSettings.polyCurvature = 0.0;
//	appPrefs.PSize = 40;
	appPrefs.ClipMargin = false;
	appPrefs.GCRMode = false;
	appPrefs.RecentDocs.clear();
	appPrefs.RecentScrapbooks.clear();
	appPrefs.RecentDCount = 5;
	appPrefs.doCopyToScrapbook = true;
	appPrefs.persistentScrapbook = false;
	appPrefs.numScrapbookCopies = 10;
	appPrefs.marginColored = false;
	appPrefs.pageSize = "A4";
	appPrefs.pageOrientation = 0;
	PageSize a4("A4");
	appPrefs.PageWidth = a4.width();
	appPrefs.PageHeight = a4.height();
	appPrefs.margins.Top = 40;
	appPrefs.margins.Bottom = 40;
	appPrefs.margins.Left = 40;
	appPrefs.margins.Right = 40;
	appPrefs.marginPreset = 0;
	appPrefs.bleeds.Top = 0;
	appPrefs.bleeds.Left = 0;
	appPrefs.bleeds.Right = 0;
	appPrefs.bleeds.Bottom = 0;
	appPrefs.toolSettings.scaleType = true;
	appPrefs.toolSettings.aspectRatio = true;
	appPrefs.toolSettings.lowResType = 1;
	appPrefs.toolSettings.useEmbeddedPath = false;
	appPrefs.MinWordLen = 3;
	appPrefs.HyCount = 2;
	appPrefs.Language = "";
	appPrefs.specialWords.clear();
	appPrefs.ignoredWords.clear();
	appPrefs.Automatic = true;
	appPrefs.AutoCheck = false;
	appPrefs.AutoSave = true;
	appPrefs.AutoSaveTime = 600000;

	int dpi = qApp->desktop()->logicalDpiX();
	if ((dpi < 60) || (dpi > 200))
		dpi = 72;
	appPrefs.DisScale = dpi / 72.0;

//	appPrefs.DisScale = 1.0;
	appPrefs.DocDir = ScPaths::getUserDocumentDir();
	appPrefs.ProfileDir = "";
	appPrefs.ScriptDir = "";
	appPrefs.documentTemplatesDir = "";
	appPrefs.CustomColorSets.clear();
	appPrefs.PrPr_Mode = false;
	appPrefs.Gcr_Mode = true;
	appPrefs.PrPr_AntiAliasing = false;
	appPrefs.PrPr_Transparency = false;
	appPrefs.PrPr_C = true;
	appPrefs.PrPr_M = true;
	appPrefs.PrPr_Y = true;
	appPrefs.PrPr_K = true;
	appPrefs.PrPr_InkCoverage = false;
	appPrefs.PrPr_InkThreshold = 250;
	appPrefs.imageEditorExecutable = "gimp";
	appPrefs.extBrowserExecutable = "";
	appPrefs.latexConfigs = LatexConfigCache::defaultConfigs();
	appPrefs.latexCommands.clear();
	appPrefs.latexEditorExecutable = "";
	appPrefs.latexResolution = 72;
	appPrefs.latexForceDpi = true;
	appPrefs.latexStartWithEmptyFrames = false;
	appPrefs.gs_AntiAliasGraphics = true;
	appPrefs.gs_AntiAliasText = true;
	appPrefs.gs_exe = getGSDefaultExeName();
	appPrefs.gs_Resolution = 72;
	appPrefs.STEcolor = QColor(Qt::white);
	appPrefs.DCMSset.DefaultMonitorProfile = "";
	appPrefs.DCMSset.DefaultPrinterProfile = "";
	appPrefs.DCMSset.DefaultImageRGBProfile = "";
	appPrefs.DCMSset.DefaultSolidColorRGBProfile = "";
	appPrefs.DCMSset.DefaultSolidColorCMYKProfile = "";
	appPrefs.DCMSset.CMSinUse = false;
	appPrefs.DCMSset.SoftProofOn = false;
	appPrefs.DCMSset.SoftProofFullOn = false;
	appPrefs.DCMSset.GamutCheck = false;
	appPrefs.DCMSset.BlackPoint = true;
	appPrefs.DCMSset.DefaultIntentColors = Intent_Relative_Colorimetric;
	appPrefs.DCMSset.DefaultIntentImages = Intent_Perceptual;
	appPrefs.GFontSub.clear();
	appPrefs.scratch.Left = 100;
	appPrefs.scratch.Right = 100;
	appPrefs.scratch.Top = 20;
	appPrefs.scratch.Bottom = 20;
	appPrefs.GapHorizontal = 0.0;
	appPrefs.GapVertical = 40.0;
	struct PageSet pageS;
	pageS.Name = CommonStrings::pageSet1;
	pageS.FirstPage = 0;
	pageS.Rows = 1;
	pageS.Columns = 1;
//	pageS.GapHorizontal = 0.0;
//	pageS.GapVertical = 0.0;
//	pageS.GapBelow = 40.0;
	pageS.pageNames.clear();
	appPrefs.pageSets.append(pageS);
	pageS.Name = CommonStrings::pageSet2;
	pageS.FirstPage = 1;
	pageS.Columns = 2;
	pageS.pageNames.clear();
	pageS.pageNames.append(CommonStrings::pageLocLeft);
	pageS.pageNames.append(CommonStrings::pageLocRight);
	appPrefs.pageSets.append(pageS);
	pageS.Name = CommonStrings::pageSet3;
	pageS.FirstPage = 0;
	pageS.Columns = 3;
	pageS.pageNames.clear();
	pageS.pageNames.append(CommonStrings::pageLocLeft);
	pageS.pageNames.append(CommonStrings::pageLocMiddle);
	pageS.pageNames.append(CommonStrings::pageLocRight);
	appPrefs.pageSets.append(pageS);
	pageS.Name = CommonStrings::pageSet4;
	pageS.FirstPage = 0;
	pageS.Columns = 4;
	pageS.pageNames.clear();
	pageS.pageNames.append(CommonStrings::pageLocLeft);
	pageS.pageNames.append(CommonStrings::pageLocMiddleLeft);
	pageS.pageNames.append(CommonStrings::pageLocMiddleRight);
	pageS.pageNames.append(CommonStrings::pageLocRight);
	appPrefs.pageSets.append(pageS);
	appPrefs.FacingPages = singlePage;
	appPrefs.askBeforeSubstitute = true;
	appPrefs.haveStylePreview = true;
	// lorem ipsum defaults
	appPrefs.useStandardLI = false;
	appPrefs.paragraphsLI = 10;
	appPrefs.showStartupDialog = true;
	appPrefs.useSmallWidgets = false;
	initDefaultCheckerPrefs(&appPrefs.checkerProfiles);
	appPrefs.curCheckProfile = CommonStrings::PostScript;
	appPrefs.PDF_Options.Thumbnails = false;
	appPrefs.PDF_Options.Articles = false;
	appPrefs.PDF_Options.useLayers = false;
	appPrefs.PDF_Options.Compress = true;
	appPrefs.PDF_Options.CompressMethod = PDFOptions::Compression_Auto;
	appPrefs.PDF_Options.Quality = 0;
	appPrefs.PDF_Options.RecalcPic = false;
	appPrefs.PDF_Options.embedPDF  = false;
	appPrefs.PDF_Options.Bookmarks = false;
	appPrefs.PDF_Options.PicRes = 300;
	appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_14;
	appPrefs.PDF_Options.Resolution = 300;
	appPrefs.PDF_Options.Binding = 0;
	appPrefs.PDF_Options.EmbedList.clear();
	appPrefs.PDF_Options.SubsetList.clear();
	appPrefs.PDF_Options.MirrorH = false;
	appPrefs.PDF_Options.MirrorV = false;
	appPrefs.PDF_Options.doClip = false;
	appPrefs.PDF_Options.RotateDeg = 0;
	appPrefs.PDF_Options.PresentMode = false;
	appPrefs.PDF_Options.fileName = "";
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
	appPrefs.PDF_Options.bleeds.Top = 0;
	appPrefs.PDF_Options.bleeds.Left = 0;
	appPrefs.PDF_Options.bleeds.Right = 0;
	appPrefs.PDF_Options.bleeds.Bottom = 0;
	appPrefs.PDF_Options.useDocBleeds = true;
	appPrefs.PDF_Options.cropMarks = false;
	appPrefs.PDF_Options.bleedMarks = false;
	appPrefs.PDF_Options.registrationMarks = false;
	appPrefs.PDF_Options.colorMarks = false;
	appPrefs.PDF_Options.docInfoMarks = false;
	appPrefs.PDF_Options.markOffset = 0;
	appPrefs.PDF_Options.EmbeddedI = false;
	appPrefs.PDF_Options.Encrypt = false;
	appPrefs.PDF_Options.PassOwner = "";
	appPrefs.PDF_Options.PassUser = "";
	appPrefs.PDF_Options.Permissions = -4;
	appPrefs.PDF_Options.UseLPI = false;
	appPrefs.PDF_Options.LPISettings.clear();
	appPrefs.PDF_Options.UseSpotColors = true;
	appPrefs.PDF_Options.doMultiFile = false;
	appPrefs.PDF_Options.displayBookmarks = false;
	appPrefs.PDF_Options.displayFullscreen = false;
	appPrefs.PDF_Options.displayLayers = false;
	appPrefs.PDF_Options.displayThumbs = false;
	appPrefs.PDF_Options.hideMenuBar = false;
	appPrefs.PDF_Options.hideToolBar = false;
	appPrefs.PDF_Options.fitWindow = false;
	appPrefs.PDF_Options.PageLayout = PDFOptions::SinglePage;
	appPrefs.PDF_Options.openAction = "";

	//Attribute setup
	appPrefs.defaultItemAttributes.clear();
	appPrefs.defaultToCSetups.clear();
	
	initDefaultActionKeys();
}

void PrefsManager::initDefaultActionKeys()
{
	ActionManager::createDefaultShortcuts();
	QMap<QString, QKeySequence > *map=ActionManager::defaultShortcuts();
	for( QMap<QString, QKeySequence >::ConstIterator it = map->begin(); it!=map->end(); ++it )
	{
		appPrefs.KeyActions[it.key()].actionName = it.key();
		appPrefs.KeyActions[it.key()].keySequence = it.value();
	}
}

void PrefsManager::applyLoadedShortCuts()
{
	for (QMap<QString,Keys>::Iterator it = appPrefs.KeyActions.begin(); it != appPrefs.KeyActions.end(); ++it )
	{
		if (!it.value().actionName.isEmpty())
		{
			if (ScCore->primaryMainWindow()->scrActions[it.value().actionName])
				ScCore->primaryMainWindow()->scrActions[it.value().actionName]->setShortcut(it.value().keySequence);
		}
	}
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
	QMatrix arrowScaling;
	arrowScaling.scale(0.5, 0.5);
	QMatrix arrowRotate;
	arrowRotate.rotate(180);
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
	arrow.name = "DiamondL";
	points.resize(0);
	points.parseSVG("M 0, -7.0710768 L -7.0710894, 0 L 0, 7.0710589 L 7.0710462, 0 L 0,-7.0710768 z");
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "DiamondM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "DiamondS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	points.resize(0);
	arrow.name = "DotL";
	points.parseSVG("M -2.5,-1.0 C -2.5,1.7600000 -4.7400000,4.0 -7.5,4.0 C -10.260000,4.0 -12.5,1.7600000 -12.5,-1.0 C -12.5,-3.7600000 -10.260000,-6.0 -7.5,-6.0 C -4.7400000,-6.0 -2.5,-3.7600000 -2.5,-1.0 z");
	points.translate(7.4, 1.0);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "DotM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "DotS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	points.resize(0);
	arrow.name = "SemiCircleInL";
	points.parseSVG("M -0.37450702,-0.045692580 C -0.37450702,2.7143074 1.8654930,4.9543074 4.6254930,4.9543074 L 4.6254930,-5.0456926 C 1.8654930,-5.0456926 -0.37450702,-2.8056926 -0.37450702,-0.045692580 z");
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "SemiCircleInM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "SemiCircleInS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	points.resize(0);
	arrow.name = "SemiCircleOutL";
	points.parseSVG("M -2.5,-0.80913858 C -2.5,1.9508614 -4.7400000,4.1908614 -7.5,4.1908614 L -7.5,-5.8091386 C -4.7400000,-5.8091386 -2.5,-3.5691386 -2.5,-0.80913858 z");
	points.translate(7.125493, 0.763446);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "SemiCircleOutM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "SemiCircleOutS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	points.resize(0);
	arrow.name = "Arrow2L";
	points.parseSVG("M 8.7185878,4.0337352 L -2.2072895,0.016013256 L 8.7185884,-4.0017078 C 6.9730900,-1.6296469 6.9831476,1.6157441 8.7185878,4.0337352 z");
	points.translate(-6.0, -0.016013256);
	points.map(arrowRotate);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "Arrow2M";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "Arrow2S";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	points.resize(0);
	arrow.name = "ScissorsL";
	points.parseSVG("M 9.0898857,-3.6061018 C 8.1198849,-4.7769976 6.3697607,-4.7358294 5.0623558,-4.2327734 L -3.1500488,-1.1548705 C -5.5383421,-2.4615840 -7.8983361,-2.0874077 -7.8983361,-2.7236578 C -7.8983361,-3.2209742 -7.4416699,-3.1119800 -7.5100293,-4.4068519 C -7.5756648,-5.6501286 -8.8736064,-6.5699315 -10.100428,-6.4884954 C -11.327699,-6.4958500 -12.599867,-5.5553341 -12.610769,-4.2584343 C -12.702194,-2.9520479 -11.603560,-1.7387447 -10.304005,-1.6532027 C -8.7816644,-1.4265411 -6.0857470,-2.3487593 -4.8210600,-0.082342643 C -5.7633447,1.6559151 -7.4350844,1.6607341 -8.9465707,1.5737277 C -10.201445,1.5014928 -11.708664,1.8611256 -12.307219,3.0945882 C -12.885586,4.2766744 -12.318421,5.9591904 -10.990470,6.3210002 C -9.6502788,6.8128279 -7.8098011,6.1912892 -7.4910978,4.6502760 C -7.2454393,3.4624530 -8.0864637,2.9043186 -7.7636052,2.4731223 C -7.5199917,2.1477623 -5.9728246,2.3362771 -3.2164999,1.0982979 L 5.6763468,4.2330688 C 6.8000164,4.5467672 8.1730685,4.5362646 9.1684433,3.4313614 L -0.051640930,-0.053722219 L 9.0898857,-3.6061018 z M -9.2179159,-5.5066058 C -7.9233569,-4.7838060 -8.0290767,-2.8230356 -9.3743431,-2.4433169 C -10.590861,-2.0196559 -12.145370,-3.2022863 -11.757521,-4.5207817 C -11.530373,-5.6026336 -10.104134,-6.0014137 -9.2179159,-5.5066058 z M -9.1616516,2.5107591 C -7.8108215,3.0096239 -8.0402087,5.2951947 -9.4138723,5.6023681 C -10.324932,5.9187072 -11.627422,5.4635705 -11.719569,4.3902287 C -11.897178,3.0851737 -10.363484,1.9060805 -9.1616516,2.5107591 z");
	points.map(arrowRotate);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "ScissorsM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "ScissorsS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	points.resize(0);
	arrow.name = "ClubL";
	points.parseSVG("M -1.5971367,-7.0977635 C -3.4863874,-7.0977635 -5.0235187,-5.5606321 -5.0235187,-3.6713813 C -5.0235187,-3.0147015 -4.7851656,-2.4444556 -4.4641095,-1.9232271 C -4.5028609,-1.8911157 -4.5437814,-1.8647646 -4.5806531,-1.8299921 C -5.2030765,-2.6849849 -6.1700514,-3.2751330 -7.3077730,-3.2751330 C -9.1970245,-3.2751331 -10.734155,-1.7380016 -10.734155,0.15124914 C -10.734155,2.0404999 -9.1970245,3.5776313 -7.3077730,3.5776313 C -6.3143268,3.5776313 -5.4391540,3.1355702 -4.8137404,2.4588126 C -4.9384274,2.8137041 -5.0235187,3.1803000 -5.0235187,3.5776313 C -5.0235187,5.4668819 -3.4863874,7.0040135 -1.5971367,7.0040135 C 0.29211394,7.0040135 1.8292454,5.4668819 1.8292454,3.5776313 C 1.8292454,2.7842354 1.5136868,2.0838028 1.0600576,1.5031550 C 2.4152718,1.7663868 3.7718375,2.2973711 4.7661444,3.8340272 C 4.0279463,3.0958289 3.5540908,1.7534117 3.5540908,-0.058529361 L 2.9247554,-0.10514681 L 3.5074733,-0.12845553 C 3.5074733,-1.9403966 3.9580199,-3.2828138 4.6962183,-4.0210121 C 3.7371277,-2.5387813 2.4390549,-1.9946496 1.1299838,-1.7134486 C 1.5341802,-2.2753578 1.8292454,-2.9268556 1.8292454,-3.6713813 C 1.8292454,-5.5606319 0.29211394,-7.0977635 -1.5971367,-7.0977635 z");
	points.map(arrowRotate);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "ClubM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "ClubS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "LineL";
	points.resize(0);
	points.parseSVG("M 0, -5 L 1, -5 L 1, 5 L 0, 5 z");
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "LineM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	arrow.name = "LineS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	appPrefs.arrowStyles.append(arrow);
	points.resize(0);
}

QString PrefsManager::setupPreferencesLocation()
{
	QString appDataDir(QDir::toNativeSeparators(ScPaths::getApplicationDataDir()));
	QFileInfo fiAppDataDir = QFileInfo(appDataDir);
	QString prefsLoc;
	//If we are using the ScPaths default prefs location
	if (fiAppDataDir.exists())
	{
		if (fiAppDataDir.isDir())
			prefsLoc = appDataDir;
		else
			prefsLoc = QDir::homePath();
	}
	else // Move to using the ScPaths default prefs location/scribus.* from ~/.scribus.*
	{
		QDir prefsDirectory = QDir();
		prefsDirectory.mkdir(appDataDir);
		prefsLoc = appDataDir;
		QString oldPR = QDir::toNativeSeparators(QDir::homePath()+"/.scribus.rc");
		QFileInfo oldPi = QFileInfo(oldPR);
		if (oldPi.exists())
			moveFile(oldPR, appDataDir+"scribus.rc");
		QString oldPR2 = QDir::toNativeSeparators(QDir::homePath()+"/.scribusfont.rc");
		QFileInfo oldPi2 = QFileInfo(oldPR2);
		if (oldPi2.exists())
			moveFile(oldPR2, appDataDir+"scribusfont.rc");
		QString oldPR3 = QDir::toNativeSeparators(QDir::homePath()+"/.scribusscrap.scs");
		QFileInfo oldPi3 = QFileInfo(oldPR3);
		if (oldPi3.exists())
			moveFile(oldPR3, appDataDir+"scrap.scs");
	}
	QString scB = QDir::toNativeSeparators(appDataDir+"scrapbook");
	QFileInfo scBi = QFileInfo(scB);
	if (!scBi.exists())
	{
		QDir scrapDirectory = QDir();
		scrapDirectory.mkdir(scB);
		QDir scrapMainDirectory = QDir();
		scrapDirectory.mkdir(QDir::toNativeSeparators(scB+"/main"));
	}
	QFileInfo scTmp = QFileInfo(QDir::toNativeSeparators(scB+"/tmp"));
	if (!scTmp.exists())
	{
		QDir scrapDirectoryT = QDir();
		scrapDirectoryT.mkdir(QDir::toNativeSeparators(scB+"/tmp"));
	}
	QFileInfo scSwatch = QFileInfo(ScPaths::getApplicationDataDir()+"swatches");
	if (!scSwatch.exists())
	{
		QDir swatchDir = QDir();
		swatchDir.mkpath(ScPaths::getApplicationDataDir()+"swatches");
		swatchDir.mkpath(ScPaths::getApplicationDataDir()+"swatches/locked");
	}
	prefsLocation = prefsLoc;
	return prefsLoc;
}

const QString PrefsManager::preferencesLocation()
{
	return prefsLocation;
}

bool PrefsManager::copyOldPreferences()
{
	//Now make copies for 1.4.0 use and leave the old ones alone for <1.3.0 usage
	QString prefs12[4], prefs130[4], prefs134[4], prefs135[4], prefs140[4];

	// Special case for scribus.rc - if found, use scribus123.rc,
	// otherwise fall back to the possibly mis-encoded scribus.rc .
	prefs12[0]=QDir::toNativeSeparators(prefsLocation+"scribus123.rc");
	if (!QFile::exists(prefs12[0]))
		prefs12[0] = prefsLocation+"scribus.rc";
	prefs12[1]=QDir::toNativeSeparators(prefsLocation+"scrap.scs");
	prefs12[2]=QDir::toNativeSeparators(prefsLocation+"prefs.xml");
	prefs12[3]=QDir::toNativeSeparators(prefsLocation+"scripter.rc");
	prefs130[0]=QDir::toNativeSeparators(prefsLocation+"scribus13.rc");
	prefs130[1]=QDir::toNativeSeparators(prefsLocation+"scrap13.scs");
	prefs130[2]=QDir::toNativeSeparators(prefsLocation+"prefs13.xml");
	prefs130[3]=QDir::toNativeSeparators(prefsLocation+"scripter13.rc");
	prefs134[0]=QDir::toNativeSeparators(prefsLocation+"scribus134.rc");
	prefs134[1]=QDir::toNativeSeparators(prefsLocation+"scrap134.scs");
	prefs134[2]=QDir::toNativeSeparators(prefsLocation+"prefs134.xml");
	prefs134[3]=QDir::toNativeSeparators(prefsLocation+"scripter134.rc");
	prefs135[0]=QDir::toNativeSeparators(prefsLocation+"scribus135.rc");
	prefs135[1]=QDir::toNativeSeparators(prefsLocation+"scrap135.scs");
	prefs135[2]=QDir::toNativeSeparators(prefsLocation+"prefs135.xml");
	prefs135[3]=QDir::toNativeSeparators(prefsLocation+"scripter135.rc");
	prefs140[0]=QDir::toNativeSeparators(prefsLocation+"scribus140.rc");
	prefs140[1]=QDir::toNativeSeparators(prefsLocation+"scrap140.scs");
	prefs140[2]=QDir::toNativeSeparators(prefsLocation+"prefs140.xml");
	prefs140[3]=QDir::toNativeSeparators(prefsLocation+"scripter140.rc");

	bool existsPrefs12[4], existsPrefs130[4], existsPrefs134[4], existsPrefs135[4], existsPrefs140[4];
	for (uint i=0;i<4;++i)
	{
		existsPrefs12[i]=QFile::exists(prefs12[i]);
		existsPrefs130[i]=QFile::exists(prefs130[i]);
		existsPrefs134[i]=QFile::exists(prefs134[i]);
		existsPrefs135[i]=QFile::exists(prefs135[i]);
		existsPrefs140[i]=QFile::exists(prefs140[i]);
	}

	bool retVal=false;
	if (existsPrefs140[0] && existsPrefs140[2])
		return retVal;

	//Only check for these three as they will be autocreated if they dont exist.
	if( (existsPrefs12[0] && !existsPrefs130[0] && !existsPrefs134[0] && !existsPrefs135[0]) || (existsPrefs12[2] && !existsPrefs130[2] && !existsPrefs134[2] && !existsPrefs135[2]) )
	{
		retVal=true; // converting from 1.2 prefs
		if (ScCore->usingGUI())
		{
			bool splashShown=ScCore->splashShowing();
			if (splashShown)
				ScCore->showSplash(false);
			if ( (ScMessageBox::question( ScCore->primaryMainWindow(), tr("Migrate Old Scribus Settings?"),
				tr("Scribus has detected existing Scribus 1.2 preferences files.\n"
						"Do you want to migrate them to the new Scribus version?"),
				QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::NoButton))==QMessageBox::Yes )
			{
				for (uint i=0;i<4;++i)
				{
					if (existsPrefs12[i] && !existsPrefs140[i])
						copyFile(prefs12[i], prefs140[i]);
				}
			}
			if (splashShown)
				ScCore->showSplash(true);
		}
	}
	else
	if(existsPrefs135[0])
	{
		for (uint i=0;i<4;++i)
		{
			if (existsPrefs135[i] && !existsPrefs140[i])
				copyFile(prefs135[i], prefs140[i]);
		}
	}
	else
	if(existsPrefs134[0])
	{
		for (uint i=0;i<4;++i)
		{
			if (existsPrefs134[i] && !existsPrefs140[i])
				copyFile(prefs134[i], prefs140[i]);
		}
	}
	else
	if(existsPrefs130[0])
	{
		for (uint i=0;i<4;++i)
		{
			if (existsPrefs130[i] && !existsPrefs140[i])
				copyFile(prefs130[i], prefs140[i]);
		}
	}
	return retVal;
}

void PrefsManager::convert12Preferences()
{
	// Import 1.2 font search path prefs
	QFile fontPrefsFile12(QDir::toNativeSeparators(prefsLocation+"/scribusfont.rc"));
	if (fontPrefsFile12.open(QIODevice::ReadOnly))
	{
		PrefsContext *pc = prefsFile->getContext("Fonts");
		PrefsTable *fontPrefs = pc->getTable("ExtraFontDirs");
		QTextStream tsx(&fontPrefsFile12);
		QString extraPath = tsx.readAll();
		fontPrefsFile12.close();
		QStringList extraFonts = extraPath.split("\n", QString::SkipEmptyParts);
		for (int i = 0; i < extraFonts.count(); ++i)
			fontPrefs->set(i, 0, extraFonts[i]);
	}
}

void PrefsManager::ReadPrefs(const QString & fname)
{
	QString realFile;
	if (fname.isNull())
		realFile = prefsLocation + "/scribus140.rc";
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
}

void PrefsManager::setupMainWindow(ScribusMainWindow* mw)
{
	mw->setDefaultPrinter(appPrefs.PrinterName, appPrefs.PrinterFile, appPrefs.PrinterCommand);

	uint max = qMin(appPrefs.RecentDCount, appPrefs.RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		QFileInfo fd(appPrefs.RecentDocs[m]);
		if (fd.exists())
		{
			mw->RecentDocs.append(appPrefs.RecentDocs[m]);
			//#9845: ScCore->fileWatcher->addFile(appPrefs.RecentDocs[m]);
		}
	}
	mw->rebuildRecentFileMenu();
	mw->move(appPrefs.mainWinSettings.xPosition, appPrefs.mainWinSettings.yPosition);
	mw->resize(appPrefs.mainWinSettings.width, appPrefs.mainWinSettings.height);
	if (appPrefs.mainWinSettings.maximized)
		mw->setWindowState((ScCore->primaryMainWindow()->windowState() & ~Qt::WindowMinimized) | Qt::WindowMaximized);
	//For 1.3.5, we dump prefs first time around.
	if (!firstTimeIgnoreOldPrefs)
		ReadPrefsXML();
	if (appPrefs.checkerProfiles.count() == 0)
	{
		initDefaultCheckerPrefs(&appPrefs.checkerProfiles);
		appPrefs.curCheckProfile = CommonStrings::PostScript;
	}
	if (!appPrefs.mainWinState.isEmpty())
	{
		mw->restoreState(appPrefs.mainWinState);
	}
}

void PrefsManager::ReadPrefsXML()
{
	if (prefsFile)
	{
		PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
		if (userprefsContext)
		{
			appPrefs.guiLanguage = userprefsContext->get("gui_language","");
			appPrefs.mainWinState = QByteArray::fromBase64(userprefsContext->get("mainwinstate","").toAscii());
			//continue here...
			//Prefs."blah blah" =...
		}
		if (prefsFile->hasContext("print_options"))
		{
			// Reset copies number to 1 when user start new session
			PrefsContext* printOptionsContext = prefsFile->getContext("print_options");
			if (printOptionsContext)
				printOptionsContext->set("Copies", 1);
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
	appPrefs.mainWinSettings.xPosition = abs(ScCore->primaryMainWindow()->pos().x());
	appPrefs.mainWinSettings.yPosition = abs(ScCore->primaryMainWindow()->pos().y());
	appPrefs.mainWinSettings.width = ScCore->primaryMainWindow()->size().width();
	appPrefs.mainWinSettings.height = ScCore->primaryMainWindow()->size().height();
	appPrefs.mainWinSettings.maximized = ScCore->primaryMainWindow()->isMaximized();
	appPrefs.mainWinState = ScCore->primaryMainWindow()->saveState();
	appPrefs.RecentDocs.clear();
	uint max = qMin(appPrefs.RecentDCount, ScCore->primaryMainWindow()->RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		appPrefs.RecentDocs.append(ScCore->primaryMainWindow()->RecentDocs[m]);
	}
	ScCore->primaryMainWindow()->getDefaultPrinter(appPrefs.PrinterName, appPrefs.PrinterFile, appPrefs.PrinterCommand);
	SavePrefsXML();
	QString realFile;
	if (fname.isNull())
		realFile = prefsLocation+"/scribus140.rc";
	else
		realFile = fname;
	if (!WritePref(realFile))
		alertSavePrefsFailed();
	emit prefsChanged();
}

void PrefsManager::SavePrefsXML()
{
	if (prefsFile)
	{
		PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
		if (userprefsContext)
		{
			userprefsContext->set("gui_language", appPrefs.guiLanguage);
			userprefsContext->set("mainwinstate", QString::fromAscii(appPrefs.mainWinState.toBase64()));
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

void PrefsManager::setExtBrowserExecutable(const QString& executableName)
{
	appPrefs.extBrowserExecutable=executableName;
}

void PrefsManager::setLatexConfigs(const QStringList& configs)
{
	appPrefs.latexConfigs=configs;
}

void PrefsManager::setLatexEditorExecutable(const QString& executableName)
{
	appPrefs.latexEditorExecutable=executableName;
}

const QString PrefsManager::documentDir()
{
	return appPrefs.DocDir;
}

void PrefsManager::setDocumentDir(const QString& dirname)
{
	appPrefs.DocDir = dirname;
}

int PrefsManager::mouseWheelValue() const
{
	return appPrefs.Wheelval;
}

//Changed to return false when we have no fonts
bool PrefsManager::GetAllFonts(bool showFontInfo)
{
	appPrefs.AvailFonts.GetFonts(prefsLocation, showFontInfo);
	return !appPrefs.AvailFonts.isEmpty();
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

bool PrefsManager::isToolColor(const QString& name)
{
	return isToolColor(appPrefs.toolSettings, name);
}

bool PrefsManager::isToolColor(const struct toolPrefs& settings, const QString& name)
{
	if (settings.dPenText == name)
		return true;
	if (settings.dStrokeText == name)
		return true;
	if (settings.dTextBackGround == name)
		return true;
	if (settings.dTextLineColor == name)
		return true;
	if (settings.dPen == name)
		return true;
	if (settings.dBrush == name)
		return true;
	if (settings.dPenLine == name)
		return true;
	if (settings.dBrushPict == name)
		return true;
	return false;
}

QStringList PrefsManager::toolColorNames()
{
	return toolColorNames(appPrefs.toolSettings);
}

QStringList PrefsManager::toolColorNames(const struct toolPrefs& settings)
{
	QStringList names;
	names.append(settings.dPenText);
	if (!names.contains(settings.dStrokeText))
		names.append(settings.dStrokeText);
	if (!names.contains(settings.dTextBackGround))
		names.append(settings.dTextBackGround);
	if (!names.contains(settings.dTextLineColor))
		names.append(settings.dTextLineColor);
	if (!names.contains(settings.dPen))
		names.append(settings.dPen);
	if (!names.contains(settings.dBrush))
		names.append(settings.dBrush);
	if (!names.contains(settings.dPenLine))
		names.append(settings.dPenLine);
	if (!names.contains(settings.dBrushPict))
		names.append(settings.dBrushPict);
	return names;
}

void PrefsManager::replaceToolColors(const QMap<QString, QString> replaceMap)
{
	replaceToolColors(appPrefs.toolSettings, replaceMap);
}

void PrefsManager::replaceToolColors(struct toolPrefs& settings, const QMap<QString, QString> replaceMap)
{
	if (replaceMap.contains(settings.dPenText))
		settings.dPenText = replaceMap[settings.dPenText];
	if (replaceMap.contains(settings.dStrokeText))
		settings.dStrokeText = replaceMap[settings.dStrokeText];
	if (replaceMap.contains(settings.dTextBackGround))
		settings.dTextBackGround = replaceMap[settings.dTextBackGround];
	if (replaceMap.contains(settings.dTextLineColor))
		settings.dTextLineColor = replaceMap[settings.dTextLineColor];
	if (replaceMap.contains(settings.dPen))
		settings.dPen = replaceMap[settings.dPen];
	if (replaceMap.contains(settings.dBrush))
		settings.dBrush = replaceMap[settings.dBrush];
	if (replaceMap.contains(settings.dPenLine))
		settings.dPenLine = replaceMap[settings.dPenLine];
	if (replaceMap.contains(settings.dBrushPict))
		settings.dBrushPict = replaceMap[settings.dBrushPict];
}

void PrefsManager::setColorSet(const ColorList& colorSet)
{
	// Color set may have changed and tools color not be present in the new color set
	ColorList tmpSet = colorSet;
	QString penText = appPrefs.toolSettings.dPenText;
	if (!tmpSet.contains(penText) && penText != CommonStrings::None)
		tmpSet[penText] = appPrefs.DColors[penText];
	QString strokeText = appPrefs.toolSettings.dStrokeText;
	if (!tmpSet.contains(strokeText) && strokeText != CommonStrings::None)
		tmpSet[strokeText] = appPrefs.DColors[strokeText];
	QString textBackGround = appPrefs.toolSettings.dTextBackGround;
	if (!tmpSet.contains(textBackGround) && textBackGround != CommonStrings::None)
		tmpSet[textBackGround] = appPrefs.DColors[textBackGround];
	QString textLineColor = appPrefs.toolSettings.dTextLineColor;
	if (!tmpSet.contains(textLineColor) && textLineColor != CommonStrings::None)
		tmpSet[textLineColor] = appPrefs.DColors[textLineColor];
	QString pen = appPrefs.toolSettings.dPen;
	if (!tmpSet.contains(pen) && pen != CommonStrings::None)
		tmpSet[pen] = appPrefs.DColors[pen];
	QString brush = appPrefs.toolSettings.dBrush;
	if (!tmpSet.contains(brush) && brush != CommonStrings::None)
		tmpSet[brush] = appPrefs.DColors[brush];
	QString penLine = appPrefs.toolSettings.dPenLine;
	if (!tmpSet.contains(penLine) && penLine != CommonStrings::None)
		tmpSet[penLine] = appPrefs.DColors[penLine];
	QString brushPict = appPrefs.toolSettings.dBrushPict;
	if (!tmpSet.contains(brushPict) && brushPict != CommonStrings::None)
		tmpSet[brushPict] = appPrefs.DColors[brushPict];
	appPrefs.DColors = tmpSet;
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
		if (ScCore->primaryMainWindow()->scrActions[actName])
		{
			ke.actionName=actName;
			ke.keySequence = keyseq;
			ke.cleanMenuText=cleanMenuText;
			ke.tableRow=rowNumber;
			appPrefs.KeyActions.insert(actName, ke);
		}
		else
			qDebug("%s", QString("Action Name: %1 does not exist").arg(actName).toAscii().constData());
	}
}

double PrefsManager::displayScale() const
{
	return appPrefs.DisScale;
}

const QString& PrefsManager::guiLanguage() const
{
	return appPrefs.guiLanguage;
}

const QString& PrefsManager::guiStyle() const
{
	return appPrefs.GUI;
}

const QString& PrefsManager::guiSystemStyle() const
{
	return appPrefs.GUIsystem;
}

const int& PrefsManager::guiFontSize() const
{
	return appPrefs.AppFontSize;
}

const int& PrefsManager::paletteFontSize() const
{
	return appPrefs.PaletteFontSize;
}

bool PrefsManager::showPageShadow() const
{
	return appPrefs.showPageShadow;
}

bool PrefsManager::WritePref(QString filename)
{
	QDomDocument docu("scribusrc");
	QString st="<SCRIBUSRC></SCRIBUSRC>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	elem.setAttribute("VERSION","1.4.0");
	QDomElement dc=docu.createElement("GUI");
	dc.setAttribute("STILT",appPrefs.GUI);
	dc.setAttribute("RAD",appPrefs.Wheelval);
	dc.setAttribute("MOVT", appPrefs.moveTimeout);
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
	dc.setAttribute("showcolborders", static_cast<int>(appPrefs.guidesSettings.colBordersShown));
	dc.setAttribute("FRV", static_cast<int>(appPrefs.guidesSettings.framesShown));
	dc.setAttribute("SHOWLAYERM", static_cast<int>(appPrefs.guidesSettings.layerMarkersShown));
	dc.setAttribute("SHOWMARGIN", static_cast<int>(appPrefs.guidesSettings.marginsShown));
	dc.setAttribute("SHOWBASE", static_cast<int>(appPrefs.guidesSettings.baseShown));
	dc.setAttribute("SHOWLINK", static_cast<int>(appPrefs.guidesSettings.linkShown));
	dc.setAttribute("SHOWPICT", static_cast<int>(appPrefs.guidesSettings.showPic));
	dc.setAttribute("SHOWControl", static_cast<int>(appPrefs.guidesSettings.showControls));
	dc.setAttribute("rulersShown", static_cast<int>(appPrefs.guidesSettings.rulersShown));
	dc.setAttribute("showBleed", static_cast<int>(appPrefs.guidesSettings.showBleed));
	dc.setAttribute("rulerMode", static_cast<int>(appPrefs.guidesSettings.rulerMode));
	dc.setAttribute("ScratchBottom", ScCLocale::toQStringC(appPrefs.scratch.Bottom));
	dc.setAttribute("ScratchLeft", ScCLocale::toQStringC(appPrefs.scratch.Left));
	dc.setAttribute("ScratchRight", ScCLocale::toQStringC(appPrefs.scratch.Right));
	dc.setAttribute("ScratchTop", ScCLocale::toQStringC(appPrefs.scratch.Top));
	dc.setAttribute("GapHorizontal", ScCLocale::toQStringC(appPrefs.GapHorizontal));
	dc.setAttribute("GapVertical", ScCLocale::toQStringC(appPrefs.GapVertical));
	dc.setAttribute("STECOLOR", appPrefs.STEcolor.name());
	dc.setAttribute("STEFONT", appPrefs.STEfont);
	dc.setAttribute("STYLEPREVIEW", static_cast<int>(appPrefs.haveStylePreview));
	dc.setAttribute("StartUp", static_cast<int>(appPrefs.showStartupDialog));
	dc.setAttribute("UseSmallWidgets", static_cast<int>(appPrefs.useSmallWidgets));
	dc.setAttribute("ToolTips", static_cast<int>(appPrefs.showToolTips));
	dc.setAttribute("showMouseCoordinates", static_cast<int>(appPrefs.showMouseCoordinates));
	dc.setAttribute("stickyTools", static_cast<int>(appPrefs.stickyTools));
	elem.appendChild(dc);
	QDomElement dc1=docu.createElement("GRID");
	dc1.setAttribute("MINOR",ScCLocale::toQStringC(appPrefs.guidesSettings.minorGrid));
	dc1.setAttribute("MAJOR",ScCLocale::toQStringC(appPrefs.guidesSettings.majorGrid));
	dc1.setAttribute("MINORC",appPrefs.guidesSettings.minorColor.name());
	dc1.setAttribute("MAJORC",appPrefs.guidesSettings.majorColor.name());
	dc1.setAttribute("GuideC", appPrefs.guidesSettings.guideColor.name());
	dc1.setAttribute("BaseC", appPrefs.guidesSettings.baseColor.name());
	dc1.setAttribute("GuideZ", ScCLocale::toQStringC(appPrefs.guidesSettings.guideRad));
	dc1.setAttribute("BACKG", static_cast<int>(appPrefs.guidesSettings.before));
	dc1.setAttribute("SHOW", static_cast<int>(appPrefs.guidesSettings.gridShown));
	elem.appendChild(dc1);
	QDomElement dc1a=docu.createElement("PAGE");
	dc1a.setAttribute("ShowPageShadow",static_cast<int>(appPrefs.showPageShadow));
	dc1a.setAttribute("PAGEC",appPrefs.DpapColor.name());
	dc1a.setAttribute("SELEC",appPrefs.DFrameColor.name());
	dc1a.setAttribute("DFrameNormColor",appPrefs.DFrameNormColor.name());
	dc1a.setAttribute("DFrameGroupColor",appPrefs.DFrameGroupColor.name());
	dc1a.setAttribute("DFrameLockColor",appPrefs.DFrameLockColor.name());
	dc1a.setAttribute("DFrameLinkColor",appPrefs.DFrameLinkColor.name());
	dc1a.setAttribute("DFrameAnnotationColor",appPrefs.DFrameAnnotationColor.name());
	dc1a.setAttribute("DPageBorderColor",appPrefs.DPageBorderColor.name());
	dc1a.setAttribute("DControlCharColor",appPrefs.DControlCharColor.name());
	dc1a.setAttribute("MARGC",appPrefs.guidesSettings.margColor.name());
	dc1a.setAttribute("RANDF", static_cast<int>(appPrefs.marginColored));
	dc1a.setAttribute("DScale", ScCLocale::toQStringC(appPrefs.DisScale, 6));
	elem.appendChild(dc1a);
	// Font information must be written before FONTS element so that face "usable"
	// member is set properly before one try to set default font. Allows to check
	// that default font is indeed usable, problems expected otherwise
	for ( SCFontsIterator itf(appPrefs.AvailFonts); itf.hasNext(); itf.next())
	{
		if (!itf.currentKey().isEmpty())
		{
			QDomElement fn=docu.createElement("FONT");
			fn.setAttribute("NAME",itf.currentKey());
			fn.setAttribute("EMBED",static_cast<int>(itf.current().embedPs()));
			fn.setAttribute("USE", static_cast<int>(itf.current().usable()));
			fn.setAttribute("SUBSET", static_cast<int>(itf.current().subset()));
			elem.appendChild(fn);
		}
	}
	QMap<QString,QString>::Iterator itfsu;
	for (itfsu = appPrefs.GFontSub.begin(); itfsu != appPrefs.GFontSub.end(); ++itfsu)
	{
		QDomElement fosu = docu.createElement("Substitute");
		fosu.setAttribute("Name",itfsu.key());
		fosu.setAttribute("Replace",itfsu.value());
		elem.appendChild(fosu);
	}
	QDomElement dc2=docu.createElement("FONTS");
	dc2.setAttribute("FACE",appPrefs.toolSettings.defFont);
	dc2.setAttribute("SIZE",appPrefs.toolSettings.defSize / 10.0);
	dc2.setAttribute("AutomaticSubst", static_cast<int>(appPrefs.askBeforeSubstitute));
	elem.appendChild(dc2);
	QDomElement dc3=docu.createElement("TYPO");
	dc3.setAttribute("TIEF",appPrefs.typographicSettings.valueSubScript);
	dc3.setAttribute("TIEFSC",appPrefs.typographicSettings.scalingSubScript);
	dc3.setAttribute("HOCH",appPrefs.typographicSettings.valueSuperScript);
	dc3.setAttribute("HOCHSC",appPrefs.typographicSettings.scalingSuperScript);
	dc3.setAttribute("SMCAPS",appPrefs.typographicSettings.valueSmallCaps);
	dc3.setAttribute("AUTOL", appPrefs.typographicSettings.autoLineSpacing);
	dc3.setAttribute("BASE", ScCLocale::toQStringC(appPrefs.typographicSettings.valueBaseGrid));
	dc3.setAttribute("BASEO", ScCLocale::toQStringC(appPrefs.typographicSettings.offsetBaseGrid));
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
	dc9.setAttribute("TEXTGAP",ScCLocale::toQStringC(appPrefs.toolSettings.dGap));
	dc9.setAttribute("TabWidth",ScCLocale::toQStringC(appPrefs.toolSettings.dTabWidth));
	dc9.setAttribute("TabFill",appPrefs.toolSettings.tabFillChar);
	dc9.setAttribute("STIL",appPrefs.toolSettings.dLineArt);
	dc9.setAttribute("STILLINE",appPrefs.toolSettings.dLstyleLine);
	dc9.setAttribute("WIDTH",ScCLocale::toQStringC(appPrefs.toolSettings.dWidth));
	dc9.setAttribute("WIDTHLINE",ScCLocale::toQStringC(appPrefs.toolSettings.dWidthLine));
	dc9.setAttribute("PENSHADE",appPrefs.toolSettings.dShade2);
	dc9.setAttribute("LINESHADE",appPrefs.toolSettings.dShadeLine);
	dc9.setAttribute("BRUSHSHADE",appPrefs.toolSettings.dShade);
	dc9.setAttribute("MAGMIN",appPrefs.toolSettings.magMin);
	dc9.setAttribute("MAGMAX",appPrefs.toolSettings.magMax);
	dc9.setAttribute("MAGSTEP",appPrefs.toolSettings.magStep);
	dc9.setAttribute("CPICT",appPrefs.toolSettings.dBrushPict);
	dc9.setAttribute("PICTSHADE",appPrefs.toolSettings.shadePict);
	dc9.setAttribute("PICTSCX",ScCLocale::toQStringC(appPrefs.toolSettings.scaleX));
	dc9.setAttribute("PICTSCY",ScCLocale::toQStringC(appPrefs.toolSettings.scaleY));
	dc9.setAttribute("POLYC", appPrefs.toolSettings.polyC);
	dc9.setAttribute("POLYF", ScCLocale::toQStringC(appPrefs.toolSettings.polyF));
	dc9.setAttribute("POLYR", ScCLocale::toQStringC(appPrefs.toolSettings.polyR));
	dc9.setAttribute("POLYFD", appPrefs.toolSettings.polyFd);
	dc9.setAttribute("POLYCUR", ScCLocale::toQStringC(appPrefs.toolSettings.polyCurvature));
	dc9.setAttribute("POLYS", static_cast<int>(appPrefs.toolSettings.polyS));
	dc9.setAttribute("PSCALE", static_cast<int>(appPrefs.toolSettings.scaleType));
	dc9.setAttribute("PASPECT", static_cast<int>(appPrefs.toolSettings.aspectRatio));
	dc9.setAttribute("EmbeddedPath", static_cast<int>(appPrefs.toolSettings.useEmbeddedPath));
	dc9.setAttribute("HalfRes", appPrefs.toolSettings.lowResType);
	dc9.setAttribute("StartArrow", appPrefs.toolSettings.dStartArrow);
	dc9.setAttribute("EndArrow", appPrefs.toolSettings.dEndArrow);
	dc9.setAttribute("dispX", ScCLocale::toQStringC(appPrefs.toolSettings.dispX));
	dc9.setAttribute("dispY", ScCLocale::toQStringC(appPrefs.toolSettings.dispY));
	dc9.setAttribute("constrain", ScCLocale::toQStringC(appPrefs.toolSettings.constrain));
	elem.appendChild(dc9);
	QDomElement dc4=docu.createElement("MAINWINDOW");
	dc4.setAttribute("XPOS",appPrefs.mainWinSettings.xPosition);
	dc4.setAttribute("YPOS",appPrefs.mainWinSettings.yPosition);
	dc4.setAttribute("WIDTH",appPrefs.mainWinSettings.width);
	dc4.setAttribute("HEIGHT",appPrefs.mainWinSettings.height);
	dc4.setAttribute("MAXIMIZED",static_cast<int>(appPrefs.mainWinSettings.maximized));
	elem.appendChild(dc4);
	QDomElement dc73=docu.createElement("SCRAPBOOK");
	dc73.setAttribute("CopyToScrapbook",static_cast<int>(appPrefs.doCopyToScrapbook));
	dc73.setAttribute("persistentScrapbook",static_cast<int>(appPrefs.persistentScrapbook));
	dc73.setAttribute("numScrapbookCopies",appPrefs.numScrapbookCopies);
	for (int rd=0; rd<appPrefs.RecentScrapbooks.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",appPrefs.RecentScrapbooks[rd]);
		dc73.appendChild(rde);
	}
	elem.appendChild(dc73);
	QDomElement dc75=docu.createElement("PAGEPALETTE");
	dc75.setAttribute("THUMBS", static_cast<int>(appPrefs.SepalT));
	dc75.setAttribute("NAMES", static_cast<int>(appPrefs.SepalN));
	elem.appendChild(dc75);
	QDomElement dc76=docu.createElement("DOKUMENT");
	dc76.setAttribute("PAGESIZE",appPrefs.pageSize);
	dc76.setAttribute("AUSRICHTUNG",appPrefs.pageOrientation);
	dc76.setAttribute("BREITE",ScCLocale::toQStringC(appPrefs.PageWidth));
	dc76.setAttribute("HOEHE",ScCLocale::toQStringC(appPrefs.PageHeight));
	dc76.setAttribute("RANDO",ScCLocale::toQStringC(appPrefs.margins.Top));
	dc76.setAttribute("RANDU",ScCLocale::toQStringC(appPrefs.margins.Bottom));
	dc76.setAttribute("RANDL",ScCLocale::toQStringC(appPrefs.margins.Left));
	dc76.setAttribute("RANDR",ScCLocale::toQStringC(appPrefs.margins.Right));
	dc76.setAttribute("PRESET",appPrefs.marginPreset);
	dc76.setAttribute("DOPPEL", appPrefs.FacingPages);
	dc76.setAttribute("AutoSave", static_cast<int>(appPrefs.AutoSave));
	dc76.setAttribute("AutoSaveTime", appPrefs.AutoSaveTime);
	dc76.setAttribute("BleedTop", ScCLocale::toQStringC(appPrefs.bleeds.Top));
	dc76.setAttribute("BleedLeft", ScCLocale::toQStringC(appPrefs.bleeds.Left));
	dc76.setAttribute("BleedRight", ScCLocale::toQStringC(appPrefs.bleeds.Right));
	dc76.setAttribute("BleedBottom", ScCLocale::toQStringC(appPrefs.bleeds.Bottom));
	elem.appendChild(dc76);
	QDomElement pageSetAttr = docu.createElement("PageSets");
	QList<PageSet>::Iterator itpgset;
	for(itpgset = appPrefs.pageSets.begin(); itpgset != appPrefs.pageSets.end(); ++itpgset )
	{
		QDomElement pgst = docu.createElement("Set");
		pgst.setAttribute("Name", (*itpgset).Name);
		pgst.setAttribute("FirstPage", (*itpgset).FirstPage);
		pgst.setAttribute("Rows", (*itpgset).Rows);
		pgst.setAttribute("Columns", (*itpgset).Columns);
//		pgst.setAttribute("GapHorizontal", (*itpgset).GapHorizontal);
//		pgst.setAttribute("GapVertical", (*itpgset).GapVertical);
//		pgst.setAttribute("GapBelow", (*itpgset).GapBelow);
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
		dc79a.setAttribute("ignoreErrors", static_cast<int>(itcp.value().ignoreErrors));
		dc79a.setAttribute("autoCheck", static_cast<int>(itcp.value().autoCheck));
		dc79a.setAttribute("checkGlyphs", static_cast<int>(itcp.value().checkGlyphs));
		dc79a.setAttribute("checkOrphans", static_cast<int>(itcp.value().checkOrphans));
		dc79a.setAttribute("checkOverflow", static_cast<int>(itcp.value().checkOverflow));
		dc79a.setAttribute("checkPictures", static_cast<int>(itcp.value().checkPictures));
		dc79a.setAttribute("checkResolution", static_cast<int>(itcp.value().checkResolution));
		dc79a.setAttribute("checkTransparency", static_cast<int>(itcp.value().checkTransparency));
		dc79a.setAttribute("checkAnnotations", static_cast<int>(itcp.value().checkAnnotations));
		dc79a.setAttribute("checkRasterPDF", static_cast<int>(itcp.value().checkRasterPDF));
		dc79a.setAttribute("checkForGIF", static_cast<int>(itcp.value().checkForGIF));
		dc79a.setAttribute("ignoreOffLayers", static_cast<int>(itcp.value().ignoreOffLayers));
		dc79a.setAttribute("checkOffConflictLayers", static_cast<int>(itcp.value().checkOffConflictLayers));
		dc79a.setAttribute("minResolution",ScCLocale::toQStringC(itcp.value().minResolution));
		dc79a.setAttribute("maxResolution",ScCLocale::toQStringC(itcp.value().maxResolution));
		elem.appendChild(dc79a);
	}
	QDomElement dc81=docu.createElement("CMS");
	dc81.setAttribute("DPSo", static_cast<int>(appPrefs.DCMSset.SoftProofOn));
	dc81.setAttribute("DPSFo", static_cast<int>(appPrefs.DCMSset.SoftProofFullOn));
	dc81.setAttribute("DPuse", static_cast<int>(appPrefs.DCMSset.CMSinUse));
	dc81.setAttribute("DPgam", static_cast<int>(appPrefs.DCMSset.GamutCheck));
	dc81.setAttribute("DPbla", static_cast<int>(appPrefs.DCMSset.BlackPoint));
	dc81.setAttribute("DPMo",appPrefs.DCMSset.DefaultMonitorProfile);
	dc81.setAttribute("DPPr",appPrefs.DCMSset.DefaultPrinterProfile);
	dc81.setAttribute("DPIn",appPrefs.DCMSset.DefaultImageRGBProfile);
	dc81.setAttribute("DPInCMYK",appPrefs.DCMSset.DefaultImageCMYKProfile);
	dc81.setAttribute("DPIn2",appPrefs.DCMSset.DefaultSolidColorRGBProfile);
	dc81.setAttribute("DPIn3",appPrefs.DCMSset.DefaultSolidColorCMYKProfile);
	//dc81.setAttribute("DIPr",appPrefs.DCMSset.DefaultIntentPrinter);
	//dc81.setAttribute("DIMo",appPrefs.DCMSset.DefaultIntentMonitor);
	dc81.setAttribute("DISc",appPrefs.DCMSset.DefaultIntentColors);
	dc81.setAttribute("DIIm",appPrefs.DCMSset.DefaultIntentImages);
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
	dc8Pr.setAttribute("AntiAliasing", static_cast<int>(appPrefs.PrPr_AntiAliasing));
	dc8Pr.setAttribute("Transparency", static_cast<int>(appPrefs.PrPr_Transparency));
	dc8Pr.setAttribute("Cyan", static_cast<int>(appPrefs.PrPr_C));
	dc8Pr.setAttribute("Magenta", static_cast<int>(appPrefs.PrPr_M));
	dc8Pr.setAttribute("Yellow", static_cast<int>(appPrefs.PrPr_Y));
	dc8Pr.setAttribute("Black", static_cast<int>(appPrefs.PrPr_K));
	dc8Pr.setAttribute("InkCoverage", static_cast<int>(appPrefs.PrPr_InkCoverage));
	dc8Pr.setAttribute("InkThreshold", appPrefs.PrPr_InkThreshold);
	elem.appendChild(dc8Pr);
	QDomElement dc8Ex = docu.createElement("EXTERNAL");
	dc8Ex.setAttribute("GIMP", imageEditorExecutable());
	dc8Ex.setAttribute("GS", ghostscriptExecutable());
	dc8Ex.setAttribute("WebBrowser", extBrowserExecutable());
	dc8Ex.setAttribute("AlphaGraphics", static_cast<int>(appPrefs.gs_AntiAliasGraphics));
	dc8Ex.setAttribute("AlphaText", static_cast<int>(appPrefs.gs_AntiAliasText));
	dc8Ex.setAttribute("Resolution", appPrefs.gs_Resolution);
	dc8Ex.setAttribute("LatexEditor", latexEditorExecutable());
	dc8Ex.setAttribute("LatexResolution", latexResolution());
	dc8Ex.setAttribute("LatexForceDpi", static_cast<int>(appPrefs.latexForceDpi));
	dc8Ex.setAttribute("LatexStartWithEmptyFrames", static_cast<int>(appPrefs.latexStartWithEmptyFrames));
	QStringList configs = latexConfigs();
	foreach (QString config, configs) {
		QDomElement domConfig = docu.createElement("LatexConfig");
		domConfig.setAttribute("file", config);
		domConfig.setAttribute("command", appPrefs.latexCommands[config]);
		dc8Ex.appendChild(domConfig);
	}
	elem.appendChild(dc8Ex);
	QDomElement rde=docu.createElement("HYPHEN");
	rde.setAttribute("LANG", appPrefs.Language);
	rde.setAttribute("WORDLEN", appPrefs.MinWordLen);
	rde.setAttribute("HYCOUNT", appPrefs.HyCount);
	rde.setAttribute("MODE", static_cast<int>(appPrefs.Automatic));
	rde.setAttribute("INMODE", static_cast<int>(appPrefs.AutoCheck));
	for (QHash<QString, QString>::Iterator hyit = appPrefs.specialWords.begin(); hyit != appPrefs.specialWords.end(); ++hyit)
	{
		QDomElement hyelm = docu.createElement("EXCEPTION");
		hyelm.setAttribute("WORD", hyit.key());
		hyelm.setAttribute("HYPHENATED", hyit.value());
		rde.appendChild(hyelm);
	}
	for (QSet<QString>::Iterator hyit2 = appPrefs.ignoredWords.begin(); hyit2 != appPrefs.ignoredWords.end(); ++hyit2)
	{
		QDomElement hyelm2 = docu.createElement("IGNORE");
		hyelm2.setAttribute("WORD", (*hyit2));
		rde.appendChild(hyelm2);
	}
	elem.appendChild(rde);
	ColorList::Iterator itc;
	for (itc = appPrefs.DColors.begin(); itc != appPrefs.DColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		if (itc.value().getColorModel() == colorModelRGB)
			co.setAttribute("RGB", itc.value().nameRGB());
		else
			co.setAttribute("CMYK", itc.value().nameCMYK());
		co.setAttribute("Spot", static_cast<int>(itc.value().isSpotColor()));
		co.setAttribute("Register", static_cast<int>(itc.value().isRegistrationColor()));
		elem.appendChild(co);
	}
	for (int rd=0; rd<appPrefs.RecentDocs.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",appPrefs.RecentDocs[rd]);
		elem.appendChild(rde);
	}
	for (QMap<QString,Keys>::Iterator ksc=appPrefs.KeyActions.begin(); ksc!=appPrefs.KeyActions.end(); ++ksc)
	{
		if (ksc.value().actionName.isEmpty())
			continue;
		QDomElement kscc=docu.createElement("SHORTCUT");
		kscc.setAttribute("ACTION",ksc.value().actionName);
		kscc.setAttribute("SEQUENCE",TabKeyboardShortcutsWidget::getKeyText(ksc.value().keySequence));
		elem.appendChild(kscc);
	}
	for (int ccs=0; ccs<appPrefs.CustomColorSets.count(); ++ccs)
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
	pdf.setAttribute("EmbedPDF", static_cast<int>(appPrefs.PDF_Options.embedPDF));
	pdf.setAttribute("MirrorH", static_cast<int>(appPrefs.PDF_Options.MirrorH));
	pdf.setAttribute("MirrorV", static_cast<int>(appPrefs.PDF_Options.MirrorV));
	pdf.setAttribute("Clip", static_cast<int>(appPrefs.PDF_Options.doClip));
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
	pdf.setAttribute("BTop", ScCLocale::toQStringC(appPrefs.PDF_Options.bleeds.Top));
	pdf.setAttribute("BLeft", ScCLocale::toQStringC(appPrefs.PDF_Options.bleeds.Left));
	pdf.setAttribute("BRight", ScCLocale::toQStringC(appPrefs.PDF_Options.bleeds.Right));
	pdf.setAttribute("BBottom", ScCLocale::toQStringC(appPrefs.PDF_Options.bleeds.Bottom));
	pdf.setAttribute("useDocBleeds", static_cast<int>(appPrefs.PDF_Options.useDocBleeds));
	pdf.setAttribute("cropMarks", static_cast<int>(appPrefs.PDF_Options.cropMarks));
	pdf.setAttribute("bleedMarks", static_cast<int>(appPrefs.PDF_Options.bleedMarks));
	pdf.setAttribute("registrationMarks", static_cast<int>(appPrefs.PDF_Options.registrationMarks));
	pdf.setAttribute("colorMarks", static_cast<int>(appPrefs.PDF_Options.colorMarks));
	pdf.setAttribute("docInfoMarks", static_cast<int>(appPrefs.PDF_Options.docInfoMarks));
	pdf.setAttribute("markOffset", appPrefs.PDF_Options.markOffset);
	pdf.setAttribute("ImagePr", static_cast<int>(appPrefs.PDF_Options.EmbeddedI));
	pdf.setAttribute("PassOwner", appPrefs.PDF_Options.PassOwner);
	pdf.setAttribute("PassUser", appPrefs.PDF_Options.PassUser);
	pdf.setAttribute("Permissions", appPrefs.PDF_Options.Permissions);
	pdf.setAttribute("Encrypt", static_cast<int>(appPrefs.PDF_Options.Encrypt));
	pdf.setAttribute("UseLayers", static_cast<int>(appPrefs.PDF_Options.useLayers));
	pdf.setAttribute("UseLpi", static_cast<int>(appPrefs.PDF_Options.UseLPI));
	pdf.setAttribute("UseSpotColors", static_cast<int>(appPrefs.PDF_Options.UseSpotColors));
	pdf.setAttribute("doMultiFile", static_cast<int>(appPrefs.PDF_Options.doMultiFile));
	pdf.setAttribute("displayBookmarks", static_cast<int>(appPrefs.PDF_Options.displayBookmarks));
	pdf.setAttribute("displayFullscreen", static_cast<int>(appPrefs.PDF_Options.displayFullscreen));
	pdf.setAttribute("displayLayers", static_cast<int>(appPrefs.PDF_Options.displayLayers));
	pdf.setAttribute("displayThumbs", static_cast<int>(appPrefs.PDF_Options.displayThumbs));
	pdf.setAttribute("PageLayout", appPrefs.PDF_Options.PageLayout);
	pdf.setAttribute("openAction", appPrefs.PDF_Options.openAction);
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = appPrefs.PDF_Options.LPISettings.begin(); itlp != appPrefs.PDF_Options.LPISettings.end(); ++itlp)
	{
		QDomElement pdf4 = docu.createElement("LPI");
		pdf4.setAttribute("Color", itlp.key());
		pdf4.setAttribute("Frequency", itlp.value().Frequency);
		pdf4.setAttribute("Angle", itlp.value().Angle);
		pdf4.setAttribute("SpotFunction", itlp.value().SpotFunc);
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
	QFile f(filename);
	if(!f.open(QIODevice::WriteOnly))
	{
		m_lastError = tr("Could not open preferences file \"%1\" for writing: %2")
			.arg(filename).arg(qApp->translate("QFile",f.errorString().toLatin1().constData()));
	}
	else
	{
		QTextStream s(&f);
		s.setCodec("UTF-8");
		s<<docu.toString();
		if (f.error()==QFile::NoError)
			result = true;
		else
			m_lastError = tr("Writing to preferences file \"%1\" failed: "
				             "QIODevice status code %2").arg(filename).arg(f.errorString());
	}
	if (f.isOpen())
		f.close();
	return result;
}

bool PrefsManager::ReadPref(QString ho)
{
	QDomDocument docu("scridoc");
	QFile f(ho);
	if(!f.open(QIODevice::ReadOnly))
	{
		m_lastError = tr("Failed to open prefs file \"%1\": %2")
			.arg(ho).arg( qApp->translate("QFile",f.errorString().toLatin1().constData()) );
		return false;
	}
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	QString errorMsg;
	int errorLine = 0, errorColumn = 0;
	if( !docu.setContent(ts.readAll(), &errorMsg, &errorLine, &errorColumn) )
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
	//Ignore scribus*.rc files prior to 1.3.5 due to changes
	bool prefs135140FileFound=false;
	if (elem.hasAttribute("VERSION"))
	{
		if (elem.attribute("VERSION") == "1.3.5" || elem.attribute("VERSION") == "1.4.0")
			prefs135140FileFound=true;
	}
	firstTimeIgnoreOldPrefs=!prefs135140FileFound;
	if (!prefs135140FileFound)
		return false;
	appPrefs.DColors.clear();
	appPrefs.latexCommands.clear();
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
			appPrefs.Wheelval = dc.attribute("RAD").toInt();
			appPrefs.moveTimeout = dc.attribute("MOVT", "150").toInt();
			appPrefs.guidesSettings.grabRad = dc.attribute("GRAB", "4").toInt();
			appPrefs.docUnitIndex = dc.attribute("UNIT", "0").toInt();
			appPrefs.AppFontSize = dc.attribute("APF", "12").toInt();
			appPrefs.PaletteFontSize = dc.attribute("PFS", "10").toInt();
			appPrefs.RecentDCount = dc.attribute("RCD","5").toUInt();
			appPrefs.DocDir = dc.attribute("DOC","");
			appPrefs.ProfileDir = dc.attribute("PROFILES","");
			appPrefs.ScriptDir = dc.attribute("SCRIPTS","");
			appPrefs.documentTemplatesDir = dc.attribute("TEMPLATES","");
			appPrefs.guidesSettings.guidesShown = static_cast<bool>(dc.attribute("SHOWGUIDES", "1").toInt());
			appPrefs.guidesSettings.colBordersShown = static_cast<bool>(dc.attribute("showcolborders", "0").toInt());
			appPrefs.guidesSettings.framesShown = static_cast<bool>(dc.attribute("FRV", "1").toInt());
			appPrefs.guidesSettings.layerMarkersShown = static_cast<bool>(dc.attribute("SHOWLAYERM", "0").toInt());
			appPrefs.guidesSettings.marginsShown = static_cast<bool>(dc.attribute("SHOWMARGIN", "1").toInt());
			appPrefs.guidesSettings.baseShown = static_cast<bool>(dc.attribute("SHOWBASE", "1").toInt());
			appPrefs.guidesSettings.linkShown = static_cast<bool>(dc.attribute("SHOWLINK", "0").toInt());
			appPrefs.guidesSettings.showPic = static_cast<bool>(dc.attribute("SHOWPICT", "1").toInt());
			appPrefs.guidesSettings.showControls = static_cast<bool>(dc.attribute("SHOWControl", "0").toInt());
			appPrefs.guidesSettings.rulersShown = static_cast<bool>(dc.attribute("rulersShown", "1").toInt());
			appPrefs.guidesSettings.showBleed = static_cast<bool>(dc.attribute("showBleed", "1").toInt());
			appPrefs.guidesSettings.rulerMode = static_cast<bool>(dc.attribute("rulerMode", "1").toInt());
			appPrefs.haveStylePreview = static_cast<bool>(dc.attribute("STYLEPREVIEW", "1").toInt());
			appPrefs.showStartupDialog = static_cast<bool>(dc.attribute("StartUp", "1").toInt());
			appPrefs.useSmallWidgets = static_cast<bool>(dc.attribute("UseSmallWidgets", "0").toInt());
			appPrefs.scratch.Bottom = ScCLocale::toDoubleC(dc.attribute("ScratchBottom"), 20.0);
			appPrefs.scratch.Left   = ScCLocale::toDoubleC(dc.attribute("ScratchLeft"), 100.0);
			appPrefs.scratch.Right  = ScCLocale::toDoubleC(dc.attribute("ScratchRight"), 100.0);
			appPrefs.scratch.Top    = ScCLocale::toDoubleC(dc.attribute("ScratchTop"), 20.0);
			appPrefs.GapHorizontal  = ScCLocale::toDoubleC(dc.attribute("GapHorizontal"), 0.0);
			appPrefs.GapVertical    = ScCLocale::toDoubleC(dc.attribute("GapVertical"), 40.0);
			if (dc.hasAttribute("STECOLOR"))
				appPrefs.STEcolor = QColor(dc.attribute("STECOLOR"));
			if (dc.hasAttribute("STEFONT"))
				appPrefs.STEfont = dc.attribute("STEFONT");
			appPrefs.showToolTips = static_cast<bool>(dc.attribute("ToolTips", "1").toInt());
			appPrefs.showMouseCoordinates = static_cast<bool>(dc.attribute("showMouseCoordinates", "1").toInt());
			appPrefs.stickyTools = static_cast<bool>(dc.attribute("stickyTools", "0").toInt());
		}
		if (dc.tagName()=="GRID")
		{
			appPrefs.guidesSettings.minorGrid  = ScCLocale::toDoubleC(dc.attribute("MINOR"), 20.0);
			appPrefs.guidesSettings.majorGrid  = ScCLocale::toDoubleC(dc.attribute("MAJOR"), 100.0);
			appPrefs.guidesSettings.minorColor = QColor(dc.attribute("MINORC"));
			appPrefs.guidesSettings.majorColor = QColor(dc.attribute("MAJORC"));
			appPrefs.guidesSettings.before = static_cast<bool>(dc.attribute("BACKG", "1").toInt());
			appPrefs.guidesSettings.gridShown = static_cast<bool>(dc.attribute("SHOW", "0").toInt());
			if (dc.hasAttribute("GuideC"))
				appPrefs.guidesSettings.guideColor = QColor(dc.attribute("GuideC"));
			if (dc.hasAttribute("GuideZ"))
				appPrefs.guidesSettings.guideRad = ScCLocale::toDoubleC(dc.attribute("GuideZ"), 10.0);
			if (dc.hasAttribute("BaseC"))
				appPrefs.guidesSettings.baseColor = QColor(dc.attribute("BaseC"));
		}
		if (dc.tagName()=="PAGE")
		{
			appPrefs.showPageShadow = static_cast<bool>(dc.attribute("ShowPageShadow", "1").toInt());
			appPrefs.DpapColor = QColor(dc.attribute("PAGEC"));
			appPrefs.DFrameColor = QColor(dc.attribute("SELEC","#ff0000"));
			appPrefs.DFrameNormColor = QColor(dc.attribute("DFrameNormColor","#000000"));
			appPrefs.DFrameGroupColor = QColor(dc.attribute("DFrameGroupColor","#008080"));
			appPrefs.DFrameLockColor = QColor(dc.attribute("DFrameLockColor","#800000"));
			appPrefs.DFrameLinkColor = QColor(dc.attribute("DFrameLinkColor","#ff0000"));
			appPrefs.DFrameAnnotationColor = QColor(dc.attribute("DFrameAnnotationColor","#0000ff"));
			appPrefs.DPageBorderColor = QColor(dc.attribute("DPageBorderColor","#ff0000"));
			appPrefs.DControlCharColor = QColor(dc.attribute("DControlCharColor","#800000"));
			appPrefs.guidesSettings.margColor = QColor(dc.attribute("MARGC","#0000ff"));
			appPrefs.marginColored = static_cast<bool>(dc.attribute("RANDF", "0").toInt());
			appPrefs.DisScale = qRound(ScCLocale::toDoubleC(dc.attribute("DScale"), appPrefs.DisScale)*72)/72.0;
		}
		if (dc.tagName()=="TYPO")
		{
			appPrefs.typographicSettings.valueSuperScript = dc.attribute("HOCH").toInt();
			appPrefs.typographicSettings.scalingSuperScript = dc.attribute("HOCHSC").toInt();
			appPrefs.typographicSettings.valueSubScript = dc.attribute("TIEF").toInt();
			appPrefs.typographicSettings.scalingSubScript = dc.attribute("TIEFSC").toInt();
			appPrefs.typographicSettings.valueSmallCaps  = dc.attribute("SMCAPS").toInt();
			appPrefs.typographicSettings.valueBaseGrid   = ScCLocale::toDoubleC(dc.attribute("BASE"), 12.0);
			appPrefs.typographicSettings.offsetBaseGrid  = ScCLocale::toDoubleC(dc.attribute("BASEO"), 0.0);
			appPrefs.typographicSettings.autoLineSpacing = dc.attribute("AUTOL", "20").toInt();
			double ulp = ScCLocale::toDoubleC(dc.attribute("UnderlinePos"), -1.0);
			if (ulp != -1)
				appPrefs.typographicSettings.valueUnderlinePos = qRound(ulp * 10);
			else
				appPrefs.typographicSettings.valueUnderlinePos = -1;
			double ulw = ScCLocale::toDoubleC(dc.attribute("UnderlineWidth"), -1.0);
			if (ulw != -1)
				appPrefs.typographicSettings.valueUnderlineWidth = qRound(ulw * 10);
			else
				appPrefs.typographicSettings.valueUnderlineWidth = -1;
			double stp = ScCLocale::toDoubleC(dc.attribute("StrikeThruPos"), -1.0);
			if (stp != -1)
				appPrefs.typographicSettings.valueStrikeThruPos = qRound(ulp * 10);
			else
				appPrefs.typographicSettings.valueStrikeThruPos = -1;
			double stw = ScCLocale::toDoubleC(dc.attribute("StrikeThruWidth"), -1.0);
			if (stw != -1)
				appPrefs.typographicSettings.valueStrikeThruWidth = qRound(stw * 10);
			else
				appPrefs.typographicSettings.valueStrikeThruWidth = -1;
		}
		if (dc.tagName()=="TOOLS")
		{
			appPrefs.toolSettings.dPen = dc.attribute("PEN");
			appPrefs.toolSettings.dBrush = dc.attribute("BRUSH", CommonStrings::None);
			appPrefs.toolSettings.dPenLine = dc.attribute("PENLINE");
			appPrefs.toolSettings.dPenText = dc.attribute("PENTEXT");
			appPrefs.toolSettings.dStrokeText = dc.attribute("StrokeText", appPrefs.toolSettings.dPenText);
			appPrefs.toolSettings.dTextBackGround = dc.attribute("TextBackGround", CommonStrings::None);
			appPrefs.toolSettings.dTextLineColor = dc.attribute("TextLineColor", CommonStrings::None);
			appPrefs.toolSettings.dTextBackGroundShade = dc.attribute("TextBackGroundShade", "100").toInt();
			appPrefs.toolSettings.dTextLineShade = dc.attribute("TextLineShade", "100").toInt();
			appPrefs.toolSettings.dTextPenShade = dc.attribute("TextPenShade", "100").toInt();
			appPrefs.toolSettings.dTextStrokeShade = dc.attribute("TextStrokeShade", "100").toInt();
			appPrefs.toolSettings.dCols = dc.attribute("TEXTCOL", "1").toInt();
			appPrefs.toolSettings.dGap  = ScCLocale::toDoubleC(dc.attribute("TEXTGAP"), 0.0);
			appPrefs.toolSettings.tabFillChar = dc.attribute("TabFill", "");
			appPrefs.toolSettings.dTabWidth   = ScCLocale::toDoubleC(dc.attribute("TabWidth"), 36.0);
			appPrefs.toolSettings.dLineArt = dc.attribute("STIL").toInt();
			appPrefs.toolSettings.dLstyleLine = dc.attribute("STILLINE").toInt();
			appPrefs.toolSettings.dWidth     = ScCLocale::toDoubleC(dc.attribute("WIDTH"), 1.0);
			appPrefs.toolSettings.dWidthLine = ScCLocale::toDoubleC(dc.attribute("WIDTHLINE"), 1.0);
			appPrefs.toolSettings.dShade2 = dc.attribute("PENSHADE").toInt();
			appPrefs.toolSettings.dShadeLine = dc.attribute("LINESHADE").toInt();
			appPrefs.toolSettings.dShade  = dc.attribute("BRUSHSHADE").toInt();
			appPrefs.toolSettings.magMin  = dc.attribute("MAGMIN", "10").toInt();
			appPrefs.toolSettings.magMax  = dc.attribute("MAGMAX", "3200").toInt();
			appPrefs.toolSettings.magStep = dc.attribute("MAGSTEP", "200").toInt();
			//CB Reset prefs zoom step value to 200% instead of old values.
			if (appPrefs.toolSettings.magStep <= 100)
				appPrefs.toolSettings.magStep = 200;
			appPrefs.toolSettings.dBrushPict = dc.attribute("CPICT", CommonStrings::None);
			appPrefs.toolSettings.shadePict = dc.attribute("PICTSHADE", "100").toInt();
			appPrefs.toolSettings.scaleX = ScCLocale::toDoubleC(dc.attribute("PICTSCX"), 1.0);
			appPrefs.toolSettings.scaleY = ScCLocale::toDoubleC(dc.attribute("PICTSCY"), 1.0);
			appPrefs.toolSettings.scaleType = static_cast<bool>(dc.attribute("PSCALE", "1").toInt());
			appPrefs.toolSettings.aspectRatio = static_cast<bool>(dc.attribute("PASPECT", "0").toInt());
			appPrefs.toolSettings.useEmbeddedPath = static_cast<bool>(dc.attribute("EmbeddedPath", "0").toInt());
			appPrefs.toolSettings.lowResType = dc.attribute("HalfRes", "1").toInt();
			appPrefs.toolSettings.polyC = dc.attribute("POLYC", "4").toInt();
			appPrefs.toolSettings.polyF = ScCLocale::toDoubleC(dc.attribute("POLYF"), 0.5);
			appPrefs.toolSettings.polyR = ScCLocale::toDoubleC(dc.attribute("POLYR"), 0.0);
			appPrefs.toolSettings.polyCurvature = ScCLocale::toDoubleC(dc.attribute("POLYCUR"), 0.0);
			appPrefs.toolSettings.polyFd = dc.attribute("POLYFD", "0").toInt();
			appPrefs.toolSettings.polyS  = static_cast<bool>(dc.attribute("POLYS", "0").toInt());
			appPrefs.toolSettings.dStartArrow = dc.attribute("StartArrow", "0").toInt();
			appPrefs.toolSettings.dEndArrow   = dc.attribute("EndArrow", "0").toInt();
			appPrefs.toolSettings.dispX = ScCLocale::toDoubleC(dc.attribute("dispX"), 10.0);
			appPrefs.toolSettings.dispY = ScCLocale::toDoubleC(dc.attribute("dispY"), 10.0);
			appPrefs.toolSettings.constrain = ScCLocale::toDoubleC(dc.attribute("constrain"), 15.0);
		}
		if (dc.tagName()=="MAINWINDOW")
		{
			appPrefs.mainWinSettings.xPosition = dc.attribute("XPOS", "0").toInt();
			appPrefs.mainWinSettings.yPosition = dc.attribute("YPOS", "0").toInt();
			appPrefs.mainWinSettings.width = dc.attribute("WIDTH", "640").toInt();
			appPrefs.mainWinSettings.height = dc.attribute("HEIGHT", "480").toInt();
			appPrefs.mainWinSettings.maximized = static_cast<bool>(dc.attribute("MAXIMIZED", "0").toInt());
			QDesktopWidget *d = QApplication::desktop();
			QSize gStrut = QApplication::globalStrut();
			int minX = 0;
			int minY = 0;
#ifdef Q_OS_MAC
			// on Mac you're dead if the titlebar is not on screen
			minY = 22;
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
			appPrefs.SepalT = static_cast<bool>(dc.attribute("THUMBS").toInt());
			appPrefs.SepalN = static_cast<bool>(dc.attribute("NAMES").toInt());
		}
		if (dc.tagName()=="SCRAPBOOK")
		{
			appPrefs.doCopyToScrapbook = static_cast<bool>(dc.attribute("CopyToScrapbook", "1").toInt());
			appPrefs.persistentScrapbook = static_cast<bool>(dc.attribute("persistentScrapbook", "0").toInt());
			appPrefs.numScrapbookCopies = dc.attribute("numScrapbookCopies", "10").toInt();
			QDomNode scrp = dc.firstChild();
			while(!scrp.isNull())
			{
				QDomElement scrpElem = scrp.toElement();
				if (scrpElem.tagName()=="RECENT")
				{
					QString nam = scrpElem.attribute("NAME");
					QFileInfo fd(nam);
					if (fd.exists())
						appPrefs.RecentScrapbooks.append(nam);
				}
				scrp = scrp.nextSibling();
			}
		}
		if (dc.tagName() == "DOKUMENT")
		{
			appPrefs.pageSize = dc.attribute("PAGESIZE","A4");
			appPrefs.pageOrientation = dc.attribute("AUSRICHTUNG", "0").toInt();
			appPrefs.PageWidth   = ScCLocale::toDoubleC(dc.attribute("BREITE"), 595.0);
			appPrefs.PageHeight  = ScCLocale::toDoubleC(dc.attribute("HOEHE"), 842.0);
			appPrefs.margins.Top = ScCLocale::toDoubleC(dc.attribute("RANDO"), 9.0);
			appPrefs.margins.Bottom = ScCLocale::toDoubleC(dc.attribute("RANDU"), 40.0);
			appPrefs.margins.Left   = ScCLocale::toDoubleC(dc.attribute("RANDL"), 9.0);
			appPrefs.margins.Right  = ScCLocale::toDoubleC(dc.attribute("RANDR"), 9.0);
			appPrefs.marginPreset   = dc.attribute("PRESET", "0").toInt();
			appPrefs.FacingPages    = dc.attribute("DOPPEL", "0").toInt();
			appPrefs.AutoSave      = static_cast<bool>(dc.attribute("AutoSave", "0").toInt());
			appPrefs.AutoSaveTime  = dc.attribute("AutoSaveTime", "600000").toInt();
			appPrefs.bleeds.Top    = ScCLocale::toDoubleC(dc.attribute("BleedTop"), 0.0);
			appPrefs.bleeds.Left   = ScCLocale::toDoubleC(dc.attribute("BleedLeft"), 0.0);
			appPrefs.bleeds.Right  = ScCLocale::toDoubleC(dc.attribute("BleedRight"), 0.0);
			appPrefs.bleeds.Bottom = ScCLocale::toDoubleC(dc.attribute("BleedBottom"), 0.0);
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
						pageS.FirstPage = PgsAttr.attribute("FirstPage", "0").toInt();
						pageS.Rows = PgsAttr.attribute("Rows", "1").toInt();
						pageS.Columns = PgsAttr.attribute("Columns", "1").toInt();
//						pageS.GapHorizontal = PgsAttr.attribute("GapHorizontal", "0").toDouble();
//						pageS.GapVertical = PgsAttr.attribute("GapVertical", "0").toDouble();
//						pageS.GapBelow = PgsAttr.attribute("GapBelow", "0").toDouble();
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
						if ((appPrefs.pageSets.count() == appPrefs.FacingPages) && ((appPrefs.GapHorizontal < 0) && (appPrefs.GapVertical < 0)))
						{
							appPrefs.GapHorizontal = ScCLocale::toDoubleC(PgsAttr.attribute("GapHorizontal"), 0.0);
							appPrefs.GapVertical   = ScCLocale::toDoubleC(PgsAttr.attribute("GapBelow"), 40.0);
						}
					}
					PGS = PGS.nextSibling();
				}
			}
		}
		if (dc.tagName()=="CMS")
		{
			appPrefs.DCMSset.SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
			appPrefs.DCMSset.SoftProofFullOn = static_cast<bool>(dc.attribute("DPSFo", "0").toInt());
			appPrefs.DCMSset.CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
			appPrefs.DCMSset.GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
			appPrefs.DCMSset.BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
			appPrefs.DCMSset.DefaultMonitorProfile = dc.attribute("DPMo","");
			appPrefs.DCMSset.DefaultPrinterProfile = dc.attribute("DPPr","");
			appPrefs.DCMSset.DefaultImageRGBProfile = dc.attribute("DPIn","");
			appPrefs.DCMSset.DefaultImageCMYKProfile = dc.attribute("DPInCMYK","");
			appPrefs.DCMSset.DefaultSolidColorRGBProfile = dc.attribute("DPIn2","");
			appPrefs.DCMSset.DefaultSolidColorCMYKProfile = dc.attribute("DPIn3","");
			appPrefs.DCMSset.DefaultIntentColors = (eRenderIntent) dc.attribute("DISc", "1").toInt();
			appPrefs.DCMSset.DefaultIntentImages = (eRenderIntent) dc.attribute("DIIm", "0").toInt();
		}
		if (!importingFrom12 && dc.tagName()=="SHORTCUT")
		{
//			if (appPrefs.KeyActions.contains(dc.attribute("ACTION")))
//			{
				appPrefs.KeyActions[dc.attribute("ACTION")].actionName = dc.attribute("ACTION");
				QKeySequence newKeySequence = QKeySequence(dc.attribute("SEQUENCE"));
				appPrefs.KeyActions[dc.attribute("ACTION")].keySequence = newKeySequence;
//			}
		}
		if (dc.tagName()=="RECENT")
			appPrefs.RecentDocs.append(dc.attribute("NAME"));
		if (dc.tagName()=="Checker")
		{
			appPrefs.curCheckProfile = dc.attribute("currentProfile", CommonStrings::PostScript);
			//#2516 work around old values until people wont have them anymore, not that these
			//translated strings should be going into prefs anyway!
			if ((appPrefs.curCheckProfile == tr("PostScript")) || ((appPrefs.curCheckProfile == tr("Postscript")) || 
				(appPrefs.curCheckProfile == "Postscript")))
			{
				appPrefs.curCheckProfile = CommonStrings::PostScript;
			}
		}
		if (dc.tagName()=="CheckProfile")
		{
			QString name=dc.attribute("Name");
			if ((name == tr("PostScript")) ||  (name == tr("Postscript")) || (name == "Postscript"))
				name = CommonStrings::PostScript;
			struct checkerPrefs checkerSettings;
			checkerSettings.ignoreErrors = static_cast<bool>(dc.attribute("ignoreErrors", "0").toInt());
			checkerSettings.autoCheck = static_cast<bool>(dc.attribute("autoCheck", "1").toInt());
			checkerSettings.checkGlyphs = static_cast<bool>(dc.attribute("checkGlyphs", "1").toInt());
			checkerSettings.checkOrphans = static_cast<bool>(dc.attribute("checkOrphans", "1").toInt());
			checkerSettings.checkOverflow = static_cast<bool>(dc.attribute("checkOverflow", "1").toInt());
			checkerSettings.checkPictures = static_cast<bool>(dc.attribute("checkPictures", "1").toInt());
			checkerSettings.checkResolution = static_cast<bool>(dc.attribute("checkResolution", "1").toInt());
			checkerSettings.checkTransparency = static_cast<bool>(dc.attribute("checkTransparency", "1").toInt());
			checkerSettings.minResolution = ScCLocale::toDoubleC(dc.attribute("minResolution"), 144.0);
			checkerSettings.maxResolution = ScCLocale::toDoubleC(dc.attribute("maxResolution"), 4800.0);
			checkerSettings.checkAnnotations = static_cast<bool>(dc.attribute("checkAnnotations", "0").toInt());
			checkerSettings.checkRasterPDF = static_cast<bool>(dc.attribute("checkRasterPDF", "1").toInt());
			checkerSettings.checkForGIF = static_cast<bool>(dc.attribute("checkForGIF", "1").toInt());
			checkerSettings.ignoreOffLayers = static_cast<bool>(dc.attribute("ignoreOffLayers", "0").toInt());
			checkerSettings.checkOffConflictLayers = static_cast<bool>(dc.attribute("checkOffConflictLayers", "0").toInt());
			appPrefs.checkerProfiles[name] = checkerSettings;
		}
		if (dc.tagName()=="PRINTER")
		{
			appPrefs.PrinterName = dc.attribute("NAME");
			appPrefs.PrinterFile = dc.attribute("FILE");
			appPrefs.PrinterCommand = dc.attribute("COMMAND");
			appPrefs.ClipMargin = static_cast<bool>(dc.attribute("CLIPMARGIN", "1").toInt());
			appPrefs.GCRMode = static_cast<bool>(dc.attribute("GMODE", "1").toInt());
		}
		if (dc.tagName()=="PRINTPREVIEW")
		{
			appPrefs.PrPr_Mode = static_cast<bool>(dc.attribute("Mode", "0").toInt());
			appPrefs.Gcr_Mode = static_cast<bool>(dc.attribute("GcrMode", "1").toInt());
			appPrefs.PrPr_AntiAliasing = static_cast<bool>(dc.attribute("AntiAliasing", "0").toInt());
			appPrefs.PrPr_Transparency = static_cast<bool>(dc.attribute("Transparency", "0").toInt());
			appPrefs.PrPr_C = static_cast<bool>(dc.attribute("Cyan", "1").toInt());
			appPrefs.PrPr_M = static_cast<bool>(dc.attribute("Magenta", "1").toInt());
			appPrefs.PrPr_Y = static_cast<bool>(dc.attribute("Yellow", "1").toInt());
			appPrefs.PrPr_K = static_cast<bool>(dc.attribute("Black", "1").toInt());
			appPrefs.PrPr_InkCoverage = static_cast<bool>(dc.attribute("InkCoverage", "0").toInt());
			appPrefs.PrPr_InkThreshold = dc.attribute("InkThreshold", "250").toInt();
		}
		if (dc.tagName()=="EXTERNAL")
		{
			bool gsa1 = testGSAvailability(dc.attribute("GS", "gs"));
			bool gsa2 = testGSAvailability(ghostscriptExecutable());
			if( (gsa1 == true) || (gsa2 == false) )
				setGhostscriptExecutable(dc.attribute("GS", "gs"));
			appPrefs.gs_AntiAliasText = static_cast<bool>(dc.attribute("AlphaText", "0").toInt());
			appPrefs.gs_AntiAliasGraphics = static_cast<bool>(dc.attribute("AlphaGraphics", "0").toInt());
			appPrefs.gs_Resolution = dc.attribute("Resolution", "72").toInt();
			appPrefs.latexResolution = dc.attribute("LatexResolution", "72").toInt();
			appPrefs.latexForceDpi = static_cast<bool>(dc.attribute("LatexForceDpi", "1").toInt());
			appPrefs.latexStartWithEmptyFrames = static_cast<bool>(dc.attribute("LatexStartWithEmptyFrames", "0").toInt());
			setImageEditorExecutable(dc.attribute("GIMP", "gimp"));
			setExtBrowserExecutable(dc.attribute("WebBrowser", ""));
			setLatexEditorExecutable(dc.attribute("LatexEditor", ""));
			QStringList configs;
			QDomNodeList configNodes = dc.elementsByTagName("LatexConfig");
			QString latexBase = LatexConfigParser::configBase();
			for (int i=0; i < configNodes.size(); i++)
			{
				QString confFile = configNodes.at(i).toElement().attribute("file", "");
				QString command  = configNodes.at(i).toElement().attribute("command", "");
				bool configExists = !confFile.isEmpty() && (QFile::exists(confFile) || QFile::exists(latexBase+confFile));
				if (!configs.contains(confFile) && configExists) {
					configs.append(confFile);
					appPrefs.latexCommands[confFile] = command;
				}
			}
			if (!configs.isEmpty()) {
				setLatexConfigs(configs);
			} else {
				qWarning() << tr("No valid renderframe config found. Using defaults!");
				setLatexConfigs(LatexConfigCache::defaultConfigs());
			}
		}
		if (dc.tagName()=="HYPHEN")
		{
			if (!dc.attribute("LANG", "").isEmpty())
				appPrefs.Language = dc.attribute("LANG");
			appPrefs.MinWordLen = dc.attribute("WORDLEN", "3").toInt();
			appPrefs.HyCount = dc.attribute("HYCOUNT", "2").toInt();
			appPrefs.Automatic = static_cast<bool>(dc.attribute("MODE", "1").toInt());
			appPrefs.AutoCheck = static_cast<bool>(dc.attribute("INMODE", "1").toInt());
			QDomNode hyelm = dc.firstChild();
			while(!hyelm.isNull())
			{
				QDomElement hyElem = hyelm.toElement();
				if (hyElem.tagName()=="EXCEPTION")
				{
					QString word = hyElem.attribute("WORD");
					QString hyph = hyElem.attribute("HYPHENATED");
					appPrefs.specialWords.insert(word, hyph);
				}
				else if (hyElem.tagName()=="IGNORE")
				{
					QString word = hyElem.attribute("WORD");
					appPrefs.ignoredWords.insert(word);
				}
				hyelm = hyelm.nextSibling();
			}
		}
		if (dc.tagName()=="FONTS")
		{
			QString tmpf(dc.attribute("FACE"));
			if (!tmpf.isEmpty())
			{
				QString newFont = "";
				if (!appPrefs.AvailFonts.contains(tmpf) || !appPrefs.AvailFonts[tmpf].usable())
				{
					ScCore->showSplash(false);
					MissingFont *dia = new MissingFont(0, tmpf, 0);
					dia->exec();
					newFont = dia->getReplacementFont();
					delete dia;
				}
				else
					newFont = tmpf;
				if (!newFont.isEmpty())
					appPrefs.toolSettings.defFont = newFont;
				appPrefs.toolSettings.defSize = qRound( ScCLocale::toDoubleC(dc.attribute("SIZE"), 12.0) * 10.0 );
				appPrefs.askBeforeSubstitute = static_cast<bool>(dc.attribute("AutomaticSubst", "1").toInt());
			}
		}
		if (dc.tagName()=="FONT")
		{
			QString tmpf(dc.attribute("NAME"));
			if (!tmpf.isEmpty() && appPrefs.AvailFonts.contains(tmpf))
			{
				ScFace& face(appPrefs.AvailFonts[tmpf]);
				face.embedPs(static_cast<bool>(dc.attribute("EMBED").toInt()));
				face.usable(face.usable() && static_cast<bool>(dc.attribute("USE", "1").toInt()));
				face.subset(static_cast<bool>(dc.attribute("SUBSET", "0").toInt()));
			}
		}
		if (dc.tagName()=="COLOR")
		{
			if (dc.hasAttribute("CMYK"))
				lf.setNamedColor(dc.attribute("CMYK"));
			else
				lf.fromQColor(QColor(dc.attribute("RGB")));
			if (dc.hasAttribute("Spot"))
				lf.setSpotColor(static_cast<bool>(dc.attribute("Spot").toInt()));
			else
				lf.setSpotColor(false);
			if (dc.hasAttribute("Register"))
				lf.setRegistrationColor(static_cast<bool>(dc.attribute("Register").toInt()));
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
			appPrefs.PDF_Options.Articles = static_cast<bool>(dc.attribute("Articles").toInt());
			appPrefs.PDF_Options.Thumbnails = static_cast<bool>(dc.attribute("Thumbnails").toInt());
			appPrefs.PDF_Options.Compress = static_cast<bool>(dc.attribute("Compress").toInt());
			appPrefs.PDF_Options.CompressMethod = (PDFOptions::PDFCompression) dc.attribute("CMethod", "0").toInt();
			appPrefs.PDF_Options.Quality = dc.attribute("Quality", "0").toInt();
			appPrefs.PDF_Options.embedPDF  = dc.attribute("EmbedPDF", "0").toInt();
			appPrefs.PDF_Options.RecalcPic = static_cast<bool>(dc.attribute("RecalcPic").toInt());
			appPrefs.PDF_Options.Bookmarks = static_cast<bool>(dc.attribute("Bookmarks").toInt());
			appPrefs.PDF_Options.MirrorH = static_cast<bool>(dc.attribute("MirrorH").toInt());
			appPrefs.PDF_Options.MirrorV = static_cast<bool>(dc.attribute("MirrorV").toInt());
			appPrefs.PDF_Options.doClip = static_cast<bool>(dc.attribute("Clip").toInt());
			appPrefs.PDF_Options.RotateDeg = dc.attribute("RotateDeg", "0").toInt();
			appPrefs.PDF_Options.PresentMode = static_cast<bool>(dc.attribute("PresentMode").toInt());
			appPrefs.PDF_Options.PicRes = dc.attribute("PicRes").toInt();
			appPrefs.PDF_Options.Version = (PDFOptions::PDFVersion)dc.attribute("Version").toInt();
			appPrefs.PDF_Options.Resolution = dc.attribute("Resolution").toInt();
			appPrefs.PDF_Options.Binding = dc.attribute("Binding").toInt();
			appPrefs.PDF_Options.fileName = "";
			appPrefs.PDF_Options.isGrayscale = static_cast<bool>(dc.attribute("Grayscale", "0").toInt());
			appPrefs.PDF_Options.UseRGB = static_cast<bool>(dc.attribute("RGBMode", "0").toInt());
			appPrefs.PDF_Options.UseProfiles = static_cast<bool>(dc.attribute("UseProfiles", "0").toInt());
			appPrefs.PDF_Options.UseProfiles2 = static_cast<bool>(dc.attribute("UseProfiles2", "0").toInt());
			appPrefs.PDF_Options.Intent = dc.attribute("Intent", "1").toInt();
			appPrefs.PDF_Options.Intent2 = dc.attribute("Intent2", "1").toInt();
			appPrefs.PDF_Options.SolidProf = dc.attribute("SolidP", "");
			appPrefs.PDF_Options.ImageProf = dc.attribute("ImageP", "");
			appPrefs.PDF_Options.PrintProf = dc.attribute("PrintP", "");
			appPrefs.PDF_Options.Info = dc.attribute("InfoString", "");
			appPrefs.PDF_Options.bleeds.Top    = ScCLocale::toDoubleC(dc.attribute("BTop"), 0.0);
			appPrefs.PDF_Options.bleeds.Left   = ScCLocale::toDoubleC(dc.attribute("BLeft"), 0.0);
			appPrefs.PDF_Options.bleeds.Right  = ScCLocale::toDoubleC(dc.attribute("BRight"), 0.0);
			appPrefs.PDF_Options.bleeds.Bottom = ScCLocale::toDoubleC(dc.attribute("BBottom"), 0.0);
			appPrefs.PDF_Options.useDocBleeds = static_cast<bool>(dc.attribute("useDocBleeds", "1").toInt());
			appPrefs.PDF_Options.cropMarks = static_cast<bool>(dc.attribute("cropMarks", "0").toInt());
			appPrefs.PDF_Options.bleedMarks = static_cast<bool>(dc.attribute("bleedMarks", "0").toInt());
			appPrefs.PDF_Options.registrationMarks = static_cast<bool>(dc.attribute("registrationMarks", "0").toInt());
			appPrefs.PDF_Options.colorMarks = static_cast<bool>(dc.attribute("colorMarks", "0").toInt());
			appPrefs.PDF_Options.docInfoMarks = static_cast<bool>(dc.attribute("docInfoMarks", "0").toInt());
			appPrefs.PDF_Options.markOffset = ScCLocale::toDoubleC(dc.attribute("markOffset"), 0.0);
			appPrefs.PDF_Options.EmbeddedI  = static_cast<bool>(dc.attribute("ImagePr", "0").toInt());
			appPrefs.PDF_Options.PassOwner  = dc.attribute("PassOwner", "");
			appPrefs.PDF_Options.PassUser   = dc.attribute("PassUser", "");
			appPrefs.PDF_Options.Permissions = dc.attribute("Permissions", "-4").toInt();
			appPrefs.PDF_Options.Encrypt = static_cast<bool>(dc.attribute("Encrypt", "0").toInt());
			appPrefs.PDF_Options.useLayers = static_cast<bool>(dc.attribute("UseLayers", "0").toInt());
			appPrefs.PDF_Options.UseLPI = static_cast<bool>(dc.attribute("UseLpi", "0").toInt());
			appPrefs.PDF_Options.UseSpotColors = static_cast<bool>(dc.attribute("UseSpotColors", "1").toInt());
			appPrefs.PDF_Options.doMultiFile = static_cast<bool>(dc.attribute("doMultiFile", "0").toInt());
			appPrefs.PDF_Options.displayBookmarks = static_cast<bool>(dc.attribute("displayBookmarks", "0").toInt());
			appPrefs.PDF_Options.displayFullscreen = static_cast<bool>(dc.attribute("displayFullscreen", "0").toInt());
			appPrefs.PDF_Options.displayLayers = static_cast<bool>(dc.attribute("displayLayers", "0").toInt());
			appPrefs.PDF_Options.displayThumbs = static_cast<bool>(dc.attribute("displayThumbs", "0").toInt());
			appPrefs.PDF_Options.PageLayout = dc.attribute("PageLayout", "0").toInt();
			appPrefs.PDF_Options.openAction = dc.attribute("openAction", "");
			QDomNode PFO = DOC.firstChild();
			appPrefs.PDF_Options.LPISettings.clear();
			while(!PFO.isNull())
			{
				QDomElement pdfF = PFO.toElement();
				if(pdfF.tagName() == "LPI")
				{
					struct LPIData lpo;
					lpo.Angle = pdfF.attribute("Angle").toInt();
					lpo.Frequency = pdfF.attribute("Frequency").toInt();
					lpo.SpotFunc = pdfF.attribute("SpotFunction").toInt();
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
					tocsetup.listNonPrintingFrames=static_cast<bool>(tocElem.attribute("ListNonPrinting").toInt());
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
			appPrefs.useStandardLI = static_cast<bool>(dc.attribute("useStandardLI", "0").toInt());
			appPrefs.paragraphsLI = dc.attribute("paragraphsLI", "10").toInt();
		}
		DOC=DOC.nextSibling();
	}
	// Some sanity checks
	appPrefs.DColors.ensureDefaultColors();
	if ((appPrefs.FacingPages  < 0) || (appPrefs.FacingPages >= appPrefs.pageSets.count()))
		appPrefs.FacingPages = 0;
	if ((appPrefs.docUnitIndex < UNITMIN) || (appPrefs.docUnitIndex > UNITMAX))
		appPrefs.docUnitIndex = int(SC_POINTS);
	// Configure GUI
	appPrefs.GUIsystem = qApp->style()->objectName();
	if (appPrefs.GUI.length() > 0)
	{
		qApp->setStyle(QStyleFactory::create(appPrefs.GUI));
		// Plain wrong, a style may set a palette different from the standard palette
		// Eg : Windows XP and Windows Vista styles
		// qApp->setPalette(qApp->style()->standardPalette());
	}
	QFont apf = qApp->font();
	apf.setPointSize(appPrefs.AppFontSize);
	qApp->setFont(apf);
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
		checkerSettings.checkForGIF = true;
		checkerSettings.ignoreOffLayers = false;
		checkerSettings.checkOffConflictLayers = false;
		checkerSettings.minResolution = 144.0;
		checkerSettings.maxResolution = 2400.0;
		//TODO Stop translating these into settings!!!!!!!!!
		cp->insert( CommonStrings::PostScript, checkerSettings);
		cp->insert( CommonStrings::PDF_1_3   , checkerSettings);
		checkerSettings.checkTransparency = false;
		cp->insert( CommonStrings::PDF_1_4   , checkerSettings);
		cp->insert( CommonStrings::PDF_1_5   , checkerSettings);
		checkerSettings.checkTransparency = true;
		checkerSettings.checkAnnotations = true;
		checkerSettings.minResolution = 144.0;
		cp->insert( CommonStrings::PDF_X1a  , checkerSettings);
		cp->insert( CommonStrings::PDF_X3   , checkerSettings);
	}
}

const QString & PrefsManager::lastError() const
{
	return m_lastError;
}

// It's hard to say whether this should be here and called from SavePrefs, or
// triggered by a signal sent from here and displayed by ScribusMainWindow.
void PrefsManager::alertSavePrefsFailed() const
{
	QMessageBox::critical(ScCore->primaryMainWindow(), tr("Error Writing Preferences"),
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
// triggered by a signal sent from here and displayed by ScribusMainWindow.
void PrefsManager::alertLoadPrefsFailed() const
{
	bool splashShowing = ScCore->splashShowing();
	if (splashShowing)
		ScCore->showSplash(false);
	QMessageBox::critical(ScCore->primaryMainWindow(), tr("Error Loading Preferences"),
			"<qt>" +
			tr("Scribus was not able to load its preferences:<br>"
			   "%1<br>"
			   "Default settings will be loaded.")
			   .arg(lastError())
			+ "</qt>",
			QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
			QMessageBox::NoButton);
	ScCore->showSplash(splashShowing);
}

int PrefsManager::gsResolution()
{
	return appPrefs.gs_Resolution;
}

void PrefsManager::languageChange()
{
	if (appPrefs.toolSettings.dTextBackGround == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dTextBackGround = CommonStrings::None;
	if (appPrefs.toolSettings.dTextLineColor == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dTextLineColor = CommonStrings::None;
	if (appPrefs.toolSettings.dPen == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dPen = CommonStrings::None;
	if (appPrefs.toolSettings.dPenText == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dPenText = CommonStrings::None;
	if (appPrefs.toolSettings.dStrokeText == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dStrokeText = CommonStrings::None;
	if (appPrefs.toolSettings.dBrush == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dBrush = CommonStrings::None;
	if (appPrefs.toolSettings.dPenLine == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dPenLine = CommonStrings::None;
	if (appPrefs.toolSettings.dBrushPict == CommonStrings::tr_NoneColor)
		appPrefs.toolSettings.dBrushPict = CommonStrings::None;
}
