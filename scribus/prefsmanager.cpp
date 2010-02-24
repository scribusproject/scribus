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
#include <QTransform>
#include <QList>
#include <QString>
#include <QStringList>
#include <QStyleFactory>

#include "prefsmanager.h"

#include "actionmanager.h"
#include "colorsetmanager.h"
#include "commonstrings.h"
#include "filewatcher.h"
#include "latexhelpers.h"
#include "ui/missing.h"
#include "ui/tabkeyboardshortcutswidget.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "pdfoptions.h"
#include "prefsfile.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scfonts.h"
#include "ui/scmessagebox.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "scribuscore.h"
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
	appPrefs.fontPrefs.AvailFonts.clear();
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

void PrefsManager::setNewPrefs(ApplicationPrefs& newPrefs)
{
	appPrefs=newPrefs;
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
	prefsFile = new PrefsFile( prefsLocation + "/prefs150.xml" );
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
	SCFontsIterator it(appPrefs.fontPrefs.AvailFonts);
	bool goodFont = false;
	for ( SCFontsIterator itf(appPrefs.fontPrefs.AvailFonts); itf.hasNext(); itf.next())
	{
		if ((itf.currentKey() == "Arial Regular") || (itf.currentKey() == "Times New Roman Regular"))
		{
			appPrefs.itemToolPrefs.textFont = itf.currentKey();
			goodFont = true;
			break;
		}
	}
	if (!goodFont)
		appPrefs.itemToolPrefs.textFont = it.currentKey();
	appPrefs.itemToolPrefs.textSize = 120;

	/** Default colours **/
	appPrefs.defaultGradients.clear();
	appPrefs.colorPrefs.DColors.clear();

	ColorSetManager csm;
	csm.initialiseDefaultPrefs(appPrefs);
	csm.findPaletteLocations();
	csm.findPalettes();
	/*
	QString pfadC = ScPaths::instance().libDir()+"swatches/";
	QString pfadC2 = pfadC + "Scribus_X11.txt";
	QFile fiC(pfadC2);
	if (!fiC.exists())
	{
		appPrefs.colorPrefs.DColors.insert("White", ScColor(0, 0, 0, 0));
		appPrefs.colorPrefs.DColors.insert("Black", ScColor(0, 0, 0, 255));
		appPrefs.colorPrefs.DColors.insert("Blue", ScColor(255, 255, 0, 0));
		appPrefs.colorPrefs.DColors.insert("Cyan", ScColor(255, 0, 0, 0));
		appPrefs.colorPrefs.DColors.insert("Green", ScColor(255, 0, 255, 0));
		appPrefs.colorPrefs.DColors.insert("Red", ScColor(0, 255, 255, 0));
		appPrefs.colorPrefs.DColors.insert("Yellow", ScColor(0, 0, 255, 0));
		appPrefs.colorPrefs.DColors.insert("Magenta", ScColor(0, 255, 0, 0));
		appPrefs.colorPrefs.DColorSet = "Scribus-Small";
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
				appPrefs.colorPrefs.DColors.insert(Cname, tmp);
			}
			fiC.close();
		}
		appPrefs.colorPrefs.DColorSet = "X11 RGB-Set";
	}
	*/

	appPrefs.uiPrefs.wheelJump = 40;
	/** Set Default window position and size to sane default values which should work on every screen */
//	appPrefs.uiPrefs.mainWinSettings.xPosition = 0;
//	appPrefs.uiPrefs.mainWinSettings.yPosition = 0;
	appPrefs.uiPrefs.mainWinSettings.width = 640;
	appPrefs.uiPrefs.mainWinSettings.height = 480;
	QDesktopWidget *d = QApplication::desktop();
	appPrefs.uiPrefs.mainWinSettings.xPosition=(d->availableGeometry().width()-appPrefs.uiPrefs.mainWinSettings.width)/2;
	appPrefs.uiPrefs.mainWinSettings.yPosition=(d->availableGeometry().height()-appPrefs.uiPrefs.mainWinSettings.height)/2;
	appPrefs.uiPrefs.mainWinSettings.maximized = false;
	appPrefs.uiPrefs.mainWinState = QByteArray();
	appPrefs.guidesPrefs.marginsShown = true;
	appPrefs.guidesPrefs.framesShown = true;
	appPrefs.guidesPrefs.layerMarkersShown = false;
	appPrefs.guidesPrefs.gridShown = false;
	appPrefs.guidesPrefs.guidesShown = true;
	appPrefs.guidesPrefs.colBordersShown = true;
	appPrefs.guidesPrefs.baselineGridShown = false;
	appPrefs.guidesPrefs.showPic = true;
	appPrefs.guidesPrefs.showControls = false;
	appPrefs.guidesPrefs.linkShown = false;
	appPrefs.guidesPrefs.rulersShown = true;
	appPrefs.guidesPrefs.showBleed = true;
	appPrefs.guidesPrefs.rulerMode = true;
	appPrefs.guidesPrefs.grabRadius = 4;
	appPrefs.guidesPrefs.guideRad = 10;
	appPrefs.guidesPrefs.minorGridSpacing = 20;
	appPrefs.guidesPrefs.majorGridSpacing = 100;
	appPrefs.guidesPrefs.minorGridColor = QColor(Qt::green);
	appPrefs.guidesPrefs.majorGridColor = QColor(Qt::green);
	appPrefs.guidesPrefs.marginColor = QColor(Qt::blue);
	appPrefs.guidesPrefs.guideColor = QColor(Qt::darkBlue);
	appPrefs.guidesPrefs.baselineGridColor = QColor(Qt::lightGray);
	appPrefs.typoPrefs.valueSuperScript = 33;
	appPrefs.typoPrefs.scalingSuperScript = 66;
	appPrefs.typoPrefs.valueSubScript = 33;
	appPrefs.typoPrefs.scalingSubScript = 66;
	appPrefs.typoPrefs.valueSmallCaps = 75;
	appPrefs.typoPrefs.autoLineSpacing = 20;
	appPrefs.typoPrefs.valueUnderlinePos = -1;
	appPrefs.typoPrefs.valueUnderlineWidth = -1;
	appPrefs.typoPrefs.valueStrikeThruPos = -1;
	appPrefs.typoPrefs.valueStrikeThruWidth = -1;
	appPrefs.guidesPrefs.valueBaselineGrid = 14.4;
	appPrefs.guidesPrefs.offsetBaselineGrid = 0.0;
	appPrefs.uiPrefs.style = "";
	appPrefs.uiPrefs.grayscaleIcons = false; // can be a little slower on startup.. but its a nice effect to play with
	appPrefs.displayPrefs.showToolTips = true;
	appPrefs.displayPrefs.showMouseCoordinates = true;
	appPrefs.uiPrefs.mouseMoveTimeout = 150;
	appPrefs.uiPrefs.stickyTools = false;
	//FIXME
	//Black here causes issues when a colour set is loaded without "Black" in it.
	//"Black" is created with wrong values. Eg SVG colour set
	appPrefs.itemToolPrefs.shapeLineColor = "Black";
	appPrefs.itemToolPrefs.shapeFillColor = "Black";
	appPrefs.itemToolPrefs.shapeLineColorShade = 100;
	appPrefs.itemToolPrefs.shapeFillColorShade = 100;
	appPrefs.itemToolPrefs.shapeLineStyle = Qt::SolidLine;
	appPrefs.itemToolPrefs.shapeLineWidth = 0;
	appPrefs.itemToolPrefs.lineColor = "Black";
	appPrefs.itemToolPrefs.textColor = "Black";
	appPrefs.itemToolPrefs.textStrokeColor = "Black";
	appPrefs.itemToolPrefs.textFillColor = CommonStrings::None;
	appPrefs.itemToolPrefs.textLineColor = CommonStrings::None;
	appPrefs.itemToolPrefs.textFillColorShade = 100;
	appPrefs.itemToolPrefs.textLineColorShade = 100;
	appPrefs.itemToolPrefs.textShade = 100;
	appPrefs.itemToolPrefs.textStrokeShade = 100;
	appPrefs.itemToolPrefs.textTabFillChar = "";
	appPrefs.itemToolPrefs.textTabWidth = 36.0;
	appPrefs.opToolPrefs.dispX = 10.0;
	appPrefs.opToolPrefs.dispY = 10.0;
	appPrefs.opToolPrefs.constrain = 15.0;
	appPrefs.displayPrefs.paperColor = QColor(Qt::white);
	appPrefs.displayPrefs.showPageShadow = true;
	appPrefs.displayPrefs.showVerifierWarningsOnCanvas = true;
	appPrefs.displayPrefs.frameColor = QColor(Qt::red);
	appPrefs.displayPrefs.frameNormColor = QColor(Qt::black);
	appPrefs.displayPrefs.frameGroupColor = QColor(Qt::darkCyan);
	appPrefs.displayPrefs.frameLockColor = QColor(Qt::darkRed);
	appPrefs.displayPrefs.frameLinkColor = QColor(Qt::red);
	appPrefs.displayPrefs.frameAnnotationColor = QColor(Qt::blue);
	appPrefs.displayPrefs.pageBorderColor = QColor(Qt::red);
	appPrefs.displayPrefs.controlCharColor = QColor(Qt::darkRed);
	appPrefs.itemToolPrefs.textColumns = 1;
	appPrefs.itemToolPrefs.textColumnGap = 0.0;
	appPrefs.itemToolPrefs.lineColorShade = 100;
	appPrefs.itemToolPrefs.lineStyle = Qt::SolidLine;
	appPrefs.itemToolPrefs.lineWidth = 1;
	appPrefs.itemToolPrefs.lineStartArrow = 0;
	appPrefs.itemToolPrefs.lineEndArrow = 0;
	appPrefs.opToolPrefs.magMin = 1;
	appPrefs.opToolPrefs.magMax = 3200;
	appPrefs.opToolPrefs.magStep = 200;
	appPrefs.itemToolPrefs.imageFillColor = CommonStrings::None;
	appPrefs.itemToolPrefs.imageFillColorShade = 100;
	appPrefs.itemToolPrefs.imageScaleX = 1;
	appPrefs.itemToolPrefs.imageScaleY = 1;
	appPrefs.guidesPrefs.guidePlacement = true;
	appPrefs.docSetupPrefs.docUnitIndex = 0;
	appPrefs.itemToolPrefs.polyCorners = 4;
	appPrefs.itemToolPrefs.polyFactor = 0.5;
	appPrefs.itemToolPrefs.polyUseFactor = false;
	appPrefs.itemToolPrefs.polyFactorGuiVal = 0;
	appPrefs.itemToolPrefs.polyRotation = 0.0;
	appPrefs.itemToolPrefs.polyCurvature = 0.0;
//	appPrefs.PSize = 40;
	appPrefs.printerPrefs.ClipMargin = false;
	appPrefs.printerPrefs.GCRMode = false;
	appPrefs.uiPrefs.RecentDocs.clear();
	appPrefs.scrapbookPrefs.RecentScrapbooks.clear();
	appPrefs.uiPrefs.recentDocCount = 5;
	appPrefs.scrapbookPrefs.doCopyToScrapbook = true;
	appPrefs.scrapbookPrefs.persistentScrapbook = false;
	appPrefs.scrapbookPrefs.writePreviews = true;
	appPrefs.scrapbookPrefs.numScrapbookCopies = 10;
	appPrefs.displayPrefs.marginColored = false;
	appPrefs.docSetupPrefs.pageSize = "A4";
	appPrefs.docSetupPrefs.pageOrientation = 0;
	PageSize a4("A4");
	appPrefs.docSetupPrefs.pageWidth = a4.width();
	appPrefs.docSetupPrefs.pageHeight = a4.height();
	appPrefs.docSetupPrefs.margins.Top = 40;
	appPrefs.docSetupPrefs.margins.Bottom = 40;
	appPrefs.docSetupPrefs.margins.Left = 40;
	appPrefs.docSetupPrefs.margins.Right = 40;
	appPrefs.docSetupPrefs.marginPreset = 0;
	appPrefs.docSetupPrefs.bleeds.Top = 0;
	appPrefs.docSetupPrefs.bleeds.Left = 0;
	appPrefs.docSetupPrefs.bleeds.Right = 0;
	appPrefs.docSetupPrefs.bleeds.Bottom = 0;
	appPrefs.itemToolPrefs.imageScaleType = true;
	appPrefs.itemToolPrefs.imageAspectRatio = true;
	appPrefs.itemToolPrefs.imageLowResType = 1;
	appPrefs.itemToolPrefs.imageUseEmbeddedPath = false;
	appPrefs.hyphPrefs.MinWordLen = 3;
	appPrefs.hyphPrefs.HyCount = 2;
	appPrefs.hyphPrefs.Language = "";
	appPrefs.hyphPrefs.specialWords.clear();
	appPrefs.hyphPrefs.ignoredWords.clear();
	appPrefs.hyphPrefs.Automatic = true;
	appPrefs.hyphPrefs.AutoCheck = false;
	appPrefs.docSetupPrefs.AutoSave = true;
	appPrefs.docSetupPrefs.AutoSaveTime = 600000;
	appPrefs.docSetupPrefs.saveCompressed = false;
	int dpi = qApp->desktop()->logicalDpiX();
	if ((dpi < 60) || (dpi > 200))
		dpi = 72;
	appPrefs.displayPrefs.displayScale = dpi / 72.0;

	appPrefs.pathPrefs.documents = ScPaths::getUserDocumentDir();
	appPrefs.pathPrefs.colorProfiles = "";
	appPrefs.pathPrefs.scripts = "";
	appPrefs.pathPrefs.documentTemplates = "";
	appPrefs.colorPrefs.CustomColorSets.clear();
	appPrefs.printPreviewPrefs.PrPr_Mode = false;
	//appPrefs.Gcr_Mode = true;
	appPrefs.printPreviewPrefs.PrPr_AntiAliasing = false;
	appPrefs.printPreviewPrefs.PrPr_Transparency = false;
	appPrefs.printPreviewPrefs.PrPr_C = true;
	appPrefs.printPreviewPrefs.PrPr_M = true;
	appPrefs.printPreviewPrefs.PrPr_Y = true;
	appPrefs.printPreviewPrefs.PrPr_K = true;
	appPrefs.printPreviewPrefs.PrPr_InkCoverage = false;
	appPrefs.printPreviewPrefs.PrPr_InkThreshold = 250;
	appPrefs.extToolPrefs.imageEditorExecutable = "gimp";
	appPrefs.extToolPrefs.extBrowserExecutable = "";
	appPrefs.extToolPrefs.uniconvExecutable = "uniconv";
	appPrefs.extToolPrefs.latexConfigs = LatexConfigCache::defaultConfigs();
	appPrefs.extToolPrefs.latexCommands.clear();
	appPrefs.extToolPrefs.latexEditorExecutable = "";
	appPrefs.extToolPrefs.latexResolution = 72;
	appPrefs.extToolPrefs.latexForceDpi = true;
	appPrefs.extToolPrefs.latexStartWithEmptyFrames = false;
	appPrefs.extToolPrefs.gs_AntiAliasGraphics = true;
	appPrefs.extToolPrefs.gs_AntiAliasText = true;
	appPrefs.extToolPrefs.gs_exe = getGSDefaultExeName();
	appPrefs.extToolPrefs.gs_Resolution = 72;
	appPrefs.storyEditorPrefs.guiFontColor = QColor(Qt::white);
	appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile = "";
	appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile = "";
	appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile = "";
	appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile = "";
	appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile = "";
	appPrefs.colorPrefs.DCMSset.CMSinUse = false;
	appPrefs.colorPrefs.DCMSset.SoftProofOn = false;
	appPrefs.colorPrefs.DCMSset.SoftProofFullOn = false;
	appPrefs.colorPrefs.DCMSset.GamutCheck = false;
	appPrefs.colorPrefs.DCMSset.BlackPoint = true;
	appPrefs.colorPrefs.DCMSset.DefaultIntentColors = Intent_Relative_Colorimetric;
	appPrefs.colorPrefs.DCMSset.DefaultIntentImages = Intent_Perceptual;
	appPrefs.fontPrefs.GFontSub.clear();
	appPrefs.displayPrefs.scratch.Left = 100;
	appPrefs.displayPrefs.scratch.Right = 100;
	appPrefs.displayPrefs.scratch.Top = 20;
	appPrefs.displayPrefs.scratch.Bottom = 20;
	appPrefs.displayPrefs.pageGapHorizontal = 0.0;
	appPrefs.displayPrefs.pageGapVertical = 40.0;
	struct PageSet pageS;
	pageS.Name = CommonStrings::pageSet1;
	pageS.FirstPage = 0;
	pageS.Rows = 1;
	pageS.Columns = 1;
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
	appPrefs.docSetupPrefs.pagePositioning = singlePage;
	appPrefs.fontPrefs.askBeforeSubstitute = true;
	appPrefs.miscPrefs.haveStylePreview = true;
	// lorem ipsum defaults
	appPrefs.miscPrefs.useStandardLI = false;
	appPrefs.miscPrefs.paragraphsLI = 10;
	appPrefs.uiPrefs.showStartupDialog = true;
	appPrefs.uiPrefs.showSplashOnStartup = true;
	appPrefs.uiPrefs.useSmallWidgets = false;
	appPrefs.uiPrefs.useTabs = false;
	initDefaultCheckerPrefs(&appPrefs.checkerPrefsList);
	appPrefs.curCheckProfile = CommonStrings::PostScript;
	appPrefs.pdfPrefs.Thumbnails = false;
	appPrefs.pdfPrefs.Articles = false;
	appPrefs.pdfPrefs.useLayers = false;
	appPrefs.pdfPrefs.Compress = true;
	appPrefs.pdfPrefs.CompressMethod = PDFOptions::Compression_Auto;
	appPrefs.pdfPrefs.Quality = 0;
	appPrefs.pdfPrefs.RecalcPic = false;
	appPrefs.pdfPrefs.embedPDF  = false;
	appPrefs.pdfPrefs.Bookmarks = false;
	appPrefs.pdfPrefs.PicRes = 300;
	appPrefs.pdfPrefs.Version = PDFOptions::PDFVersion_14;
	appPrefs.pdfPrefs.Resolution = 300;
	appPrefs.pdfPrefs.Binding = 0;
	appPrefs.pdfPrefs.EmbedList.clear();
	appPrefs.pdfPrefs.SubsetList.clear();
	appPrefs.pdfPrefs.MirrorH = false;
	appPrefs.pdfPrefs.MirrorV = false;
	appPrefs.pdfPrefs.doClip = false;
	appPrefs.pdfPrefs.RotateDeg = 0;
	appPrefs.pdfPrefs.PresentMode = false;
	appPrefs.pdfPrefs.fileName = "";
	appPrefs.pdfPrefs.PresentVals.clear();
	appPrefs.pdfPrefs.isGrayscale = false;
	appPrefs.pdfPrefs.UseRGB = true;
	appPrefs.pdfPrefs.UseProfiles = false;
	appPrefs.pdfPrefs.UseProfiles2 = false;
	appPrefs.pdfPrefs.SolidProf = "";
	appPrefs.pdfPrefs.SComp = 3;
	appPrefs.pdfPrefs.ImageProf = "";
	appPrefs.pdfPrefs.PrintProf = "";
	appPrefs.pdfPrefs.Info = "";
	appPrefs.pdfPrefs.Intent = 0;
	appPrefs.pdfPrefs.Intent2 = 0;
	appPrefs.pdfPrefs.bleeds.Top = 0;
	appPrefs.pdfPrefs.bleeds.Left = 0;
	appPrefs.pdfPrefs.bleeds.Right = 0;
	appPrefs.pdfPrefs.bleeds.Bottom = 0;
	appPrefs.pdfPrefs.useDocBleeds = true;
	appPrefs.pdfPrefs.cropMarks = false;
	appPrefs.pdfPrefs.bleedMarks = false;
	appPrefs.pdfPrefs.registrationMarks = false;
	appPrefs.pdfPrefs.colorMarks = false;
	appPrefs.pdfPrefs.docInfoMarks = false;
	appPrefs.pdfPrefs.markOffset = 0;
	appPrefs.pdfPrefs.EmbeddedI = false;
	appPrefs.pdfPrefs.Encrypt = false;
	appPrefs.pdfPrefs.PassOwner = "";
	appPrefs.pdfPrefs.PassUser = "";
	appPrefs.pdfPrefs.Permissions = -4;
	appPrefs.pdfPrefs.UseLPI = false;
	appPrefs.pdfPrefs.LPISettings.clear();
	appPrefs.pdfPrefs.UseSpotColors = true;
	appPrefs.pdfPrefs.doMultiFile = false;
	appPrefs.pdfPrefs.displayBookmarks = false;
	appPrefs.pdfPrefs.displayFullscreen = false;
	appPrefs.pdfPrefs.displayLayers = false;
	appPrefs.pdfPrefs.displayThumbs = false;
	appPrefs.pdfPrefs.hideMenuBar = false;
	appPrefs.pdfPrefs.hideToolBar = false;
	appPrefs.pdfPrefs.fitWindow = false;
	appPrefs.pdfPrefs.PageLayout = PDFOptions::SinglePage;
	appPrefs.pdfPrefs.openAction = "";
	appPrefs.imageCachePrefs.cacheEnabled = false;
	appPrefs.imageCachePrefs.maxCacheSizeMiB = 1000;
	appPrefs.imageCachePrefs.maxCacheEntries = 1000;
	appPrefs.imageCachePrefs.compressionLevel = 1;

	//Attribute setup
	appPrefs.itemAttrPrefs.defaultItemAttributes.clear();
	appPrefs.tocPrefs.defaultToCSetups.clear();

	initDefaultActionKeys();
}

void PrefsManager::initDefaultActionKeys()
{
	ActionManager::createDefaultShortcuts();
	QMap<QString, QKeySequence > *map=ActionManager::defaultShortcuts();
	for( QMap<QString, QKeySequence >::ConstIterator it = map->begin(); it!=map->end(); ++it )
	{
		appPrefs.keyShortcutPrefs.KeyActions[it.key()].actionName = it.key();
		appPrefs.keyShortcutPrefs.KeyActions[it.key()].keySequence = it.value();
	}
}

void PrefsManager::applyLoadedShortCuts()
{
	for (QMap<QString,Keys>::Iterator it = appPrefs.keyShortcutPrefs.KeyActions.begin(); it != appPrefs.keyShortcutPrefs.KeyActions.end(); ++it )
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
	appPrefs.uiPrefs.applicationFontSize = guiFont.pointSize();
	appPrefs.storyEditorPrefs.guiFont = guiFont.toString();
	appPrefs.uiPrefs.paletteFontSize = appPrefs.uiPrefs.applicationFontSize;
}

void PrefsManager::initArrowStyles()
{
	struct ArrowDesc arrow;
	FPointArray points;
	QTransform arrowScaling;
	arrowScaling.scale(0.5, 0.5);
	QTransform arrowRotate;
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
	QString Pff = QDir::convertSeparators(ScPaths::getApplicationDataDir());
	QFileInfo Pffi = QFileInfo(Pff);
	QString PrefsPfad;
	//If we are using the ScPaths default prefs location
	if (Pffi.exists())
	{
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QDir::homePath();
	}
	else // Move to using the ScPaths default prefs location/scribus.* from ~/.scribus.*
	{
		QDir prefsDirectory = QDir();
		prefsDirectory.mkdir(Pff);
		PrefsPfad = Pff;
		QString oldPR = QDir::convertSeparators(QDir::homePath()+"/.scribus.rc");
		QFileInfo oldPi = QFileInfo(oldPR);
		if (oldPi.exists())
			moveFile(oldPR, Pff+"/scribus.rc");
		QString oldPR2 = QDir::convertSeparators(QDir::homePath()+"/.scribusfont.rc");
		QFileInfo oldPi2 = QFileInfo(oldPR2);
		if (oldPi2.exists())
			moveFile(oldPR2, Pff+"/scribusfont.rc");
		QString oldPR3 = QDir::convertSeparators(QDir::homePath()+"/.scribusscrap.scs");
		QFileInfo oldPi3 = QFileInfo(oldPR3);
		if (oldPi3.exists())
			moveFile(oldPR3, Pff+"/scrap.scs");
	}
	QString scP = QDir::convertSeparators(ScPaths::getPluginDataDir());
	QFileInfo scPi = QFileInfo(scP);
	if (!scPi.exists())
	{
		QDir pluginDataDirectory = QDir();
		pluginDataDirectory.mkdir(scP);
	}
	QString scB = QDir::convertSeparators(Pff+"/scrapbook");
	QFileInfo scBi = QFileInfo(scB);
	if (!scBi.exists())
	{
		QDir scrapDirectory = QDir();
		scrapDirectory.mkdir(scB);
		QDir scrapMainDirectory = QDir();
		scrapDirectory.mkdir(QDir::convertSeparators(scB+"/main"));
	}
	QFileInfo scTmp = QFileInfo(QDir::convertSeparators(scB+"/tmp"));
	if (!scTmp.exists())
	{
		QDir scrapDirectoryT = QDir();
		scrapDirectoryT.mkdir(QDir::convertSeparators(scB+"/tmp"));
	}
	prefsLocation=PrefsPfad;
	return PrefsPfad;
}

const QString PrefsManager::preferencesLocation()
{
	return prefsLocation;
}

bool PrefsManager::copyOldPreferences()
{
	//Now make copies for 1.3 use and leave the old ones alone for <1.3.0 usage
	QString prefs135[4], prefs140[4], prefs150[4];

	prefs135[0]=QDir::convertSeparators(prefsLocation+"/scribus135.rc");
	prefs135[1]=QDir::convertSeparators(prefsLocation+"/scrap135.scs");
	prefs135[2]=QDir::convertSeparators(prefsLocation+"/prefs135.xml");
	prefs135[3]=QDir::convertSeparators(prefsLocation+"/scripter135.rc");
	prefs140[0]=QDir::convertSeparators(prefsLocation+"/scribus140.rc");
	prefs140[1]=QDir::convertSeparators(prefsLocation+"/scrap140.scs");
	prefs140[2]=QDir::convertSeparators(prefsLocation+"/prefs140.xml");
	prefs140[3]=QDir::convertSeparators(prefsLocation+"/scripter140.rc");
	prefs150[0]=QDir::convertSeparators(prefsLocation+"/scribus150.rc");
	prefs150[1]=QDir::convertSeparators(prefsLocation+"/scrap150.scs");
	prefs150[2]=QDir::convertSeparators(prefsLocation+"/prefs150.xml");
	prefs150[3]=QDir::convertSeparators(prefsLocation+"/scripter150.rc");

	bool existsPrefs135[4], existsPrefs140[4], existsPrefs150[4];
	for (uint i=0;i<4;++i)
	{
		existsPrefs135[i]=QFile::exists(prefs135[i]);
		existsPrefs140[i]=QFile::exists(prefs140[i]);
		existsPrefs150[i]=QFile::exists(prefs150[i]);
	}

	bool retVal=false;
	if (existsPrefs150[0] && existsPrefs150[2])
		return retVal;
	//Only check for these three as they will be autocreated if they dont exist.
	if( (existsPrefs135[0] && !existsPrefs140[0]) || (existsPrefs135[2] && !existsPrefs140[2]) )
	{
		// Now always return false
		// retVal=true; // converting from 1.2 prefs
		if (ScCore->usingGUI())
		{
			bool splashShown=ScCore->splashShowing();
			if (splashShown)
				ScCore->showSplash(false);
			if ( (ScMessageBox::question( ScCore->primaryMainWindow(), tr("Migrate Old Scribus Settings?"),
				tr("Scribus has detected existing Scribus 1.3.5 preferences files.\n"
						"Do you want to migrate them to the new Scribus version?"),
				QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::NoButton))==QMessageBox::Yes )
			{
				for (uint i=0;i<4;++i)
				{
					if (existsPrefs135[i] && !existsPrefs150[i])
						copyFile(prefs135[i], prefs150[i]);
				}
			}
			if (splashShown)
				ScCore->showSplash(true);
		}
	}
	else if(existsPrefs140[0])
	{
		for (uint i=0;i<4;++i)
		{
			if (existsPrefs140[i] && !existsPrefs150[i])
				copyFile(prefs140[i], prefs150[i]);
		}
	}
	else if(existsPrefs135[0])
	{
		for (uint i=0;i<4;++i)
		{
			if (existsPrefs135[i] && !existsPrefs150[i])
				copyFile(prefs135[i], prefs150[i]);
		}
	}
	return retVal;
}

void PrefsManager::convert12Preferences()
{
	// Import 1.2 font search path prefs
	QFile fontPrefsFile12(QDir::convertSeparators(prefsLocation+"/scribusfont.rc"));
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
		realFile = prefsLocation + "/scribus150.rc";
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
	mw->setDefaultPrinter(appPrefs.printerPrefs.PrinterName, appPrefs.printerPrefs.PrinterFile, appPrefs.printerPrefs.PrinterCommand);

	uint max = qMin(appPrefs.uiPrefs.recentDocCount, appPrefs.uiPrefs.RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		QFileInfo fd(appPrefs.uiPrefs.RecentDocs[m]);
		if (fd.exists())
		{
			mw->RecentDocs.append(appPrefs.uiPrefs.RecentDocs[m]);
			ScCore->fileWatcher->addFile(appPrefs.uiPrefs.RecentDocs[m]);
		}
	}
	mw->rebuildRecentFileMenu();
	mw->move(appPrefs.uiPrefs.mainWinSettings.xPosition, appPrefs.uiPrefs.mainWinSettings.yPosition);
	mw->resize(appPrefs.uiPrefs.mainWinSettings.width, appPrefs.uiPrefs.mainWinSettings.height);
	if (appPrefs.uiPrefs.mainWinSettings.maximized)
		mw->setWindowState((ScCore->primaryMainWindow()->windowState() & ~Qt::WindowMinimized) | Qt::WindowMaximized);
	//For 1.3.5, we dump prefs first time around.
	if (!firstTimeIgnoreOldPrefs)
		ReadPrefsXML();
	if (appPrefs.checkerPrefsList.count() == 0)
	{
		initDefaultCheckerPrefs(&appPrefs.checkerPrefsList);
		appPrefs.curCheckProfile = CommonStrings::PostScript;
	}
	if (!appPrefs.uiPrefs.mainWinState.isEmpty())
	{
		mw->restoreState(appPrefs.uiPrefs.mainWinState);
	}
}

void PrefsManager::ReadPrefsXML()
{
	if (prefsFile)
	{
		PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
		if (userprefsContext)
		{
			appPrefs.uiPrefs.language = userprefsContext->get("gui_language","");
			appPrefs.uiPrefs.mainWinState = QByteArray::fromBase64(userprefsContext->get("mainwinstate","").toAscii());
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
	appPrefs.uiPrefs.mainWinSettings.xPosition = abs(ScCore->primaryMainWindow()->pos().x());
	appPrefs.uiPrefs.mainWinSettings.yPosition = abs(ScCore->primaryMainWindow()->pos().y());
	appPrefs.uiPrefs.mainWinSettings.width = ScCore->primaryMainWindow()->size().width();
	appPrefs.uiPrefs.mainWinSettings.height = ScCore->primaryMainWindow()->size().height();
	appPrefs.uiPrefs.mainWinSettings.maximized = ScCore->primaryMainWindow()->isMaximized();
	appPrefs.uiPrefs.mainWinState = ScCore->primaryMainWindow()->saveState();
	appPrefs.uiPrefs.RecentDocs.clear();
	uint max = qMin(appPrefs.uiPrefs.recentDocCount, ScCore->primaryMainWindow()->RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		appPrefs.uiPrefs.RecentDocs.append(ScCore->primaryMainWindow()->RecentDocs[m]);
	}
	ScCore->primaryMainWindow()->getDefaultPrinter(appPrefs.printerPrefs.PrinterName, appPrefs.printerPrefs.PrinterFile, appPrefs.printerPrefs.PrinterCommand);
	SavePrefsXML();
	QString realFile;
	if (fname.isNull())
		realFile = prefsLocation+"/scribus150.rc";
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
			userprefsContext->set("gui_language", appPrefs.uiPrefs.language);
			userprefsContext->set("mainwinstate", QString::fromAscii(appPrefs.uiPrefs.mainWinState.toBase64()));
			//continue here...
			//Prefs."blah blah" =...
		}
		prefsFile->write();
	}
}

void PrefsManager::setGhostscriptExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.gs_exe=executableName;
}

void PrefsManager::setImageEditorExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.imageEditorExecutable=executableName;
}

void PrefsManager::setExtBrowserExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.extBrowserExecutable=executableName;
}

void PrefsManager::setUniconvExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.uniconvExecutable=executableName;
}

void PrefsManager::setLatexConfigs(const QStringList& configs)
{
	appPrefs.extToolPrefs.latexConfigs=configs;
}

void PrefsManager::setLatexEditorExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.latexEditorExecutable=executableName;
}

const QString PrefsManager::documentDir()
{
	return appPrefs.pathPrefs.documents;
}

void PrefsManager::setDocumentDir(const QString& dirname)
{
	appPrefs.pathPrefs.documents = dirname;
}

int PrefsManager::mouseWheelJump() const
{
	return appPrefs.uiPrefs.wheelJump;
}

//Changed to return false when we have no fonts
bool PrefsManager::GetAllFonts(bool showFontInfo)
{
	appPrefs.fontPrefs.AvailFonts.GetFonts(prefsLocation, showFontInfo);
	return !appPrefs.fontPrefs.AvailFonts.isEmpty();
}

void PrefsManager::setShowStartupDialog(const bool showDialog)
{
	appPrefs.uiPrefs.showStartupDialog=showDialog;
}

const ColorList& PrefsManager::colorSet()
{
	return appPrefs.colorPrefs.DColors;
}

ColorList* PrefsManager::colorSetPtr()
{
	return &appPrefs.colorPrefs.DColors;
}

const QString& PrefsManager::colorSetName()
{
	return appPrefs.colorPrefs.DColorSet;
}

bool PrefsManager::isToolColor(const QString& name)
{
	return isToolColor(appPrefs.itemToolPrefs, name);
}

bool PrefsManager::isToolColor(const struct ItemToolPrefs& settings, const QString& name)
{
	if (settings.textColor == name)
		return true;
	if (settings.textStrokeColor == name)
		return true;
	if (settings.textFillColor == name)
		return true;
	if (settings.textLineColor == name)
		return true;
	if (settings.shapeLineColor == name)
		return true;
	if (settings.shapeFillColor == name)
		return true;
	if (settings.lineColor == name)
		return true;
	if (settings.imageFillColor == name)
		return true;
	return false;
}

QStringList PrefsManager::toolColorNames()
{
	return toolColorNames(appPrefs.itemToolPrefs);
}

QStringList PrefsManager::toolColorNames(const struct ItemToolPrefs& settings)
{
	QStringList names;
	names.append(settings.textColor);
	if (!names.contains(settings.textStrokeColor))
		names.append(settings.textStrokeColor);
	if (!names.contains(settings.textFillColor))
		names.append(settings.textFillColor);
	if (!names.contains(settings.textLineColor))
		names.append(settings.textLineColor);
	if (!names.contains(settings.shapeLineColor))
		names.append(settings.shapeLineColor);
	if (!names.contains(settings.shapeFillColor))
		names.append(settings.shapeFillColor);
	if (!names.contains(settings.lineColor))
		names.append(settings.lineColor);
	if (!names.contains(settings.imageFillColor))
		names.append(settings.imageFillColor);
	return names;
}

void PrefsManager::replaceToolColors(const QMap<QString, QString> replaceMap)
{
	replaceToolColors(appPrefs.itemToolPrefs, replaceMap);
}

void PrefsManager::replaceToolColors(struct ItemToolPrefs& settings, const QMap<QString, QString> replaceMap)
{
	if (replaceMap.contains(settings.textColor))
		settings.textColor = replaceMap[settings.textColor];
	if (replaceMap.contains(settings.textStrokeColor))
		settings.textStrokeColor = replaceMap[settings.textStrokeColor];
	if (replaceMap.contains(settings.textFillColor))
		settings.textFillColor = replaceMap[settings.textFillColor];
	if (replaceMap.contains(settings.textLineColor))
		settings.textLineColor = replaceMap[settings.textLineColor];
	if (replaceMap.contains(settings.shapeLineColor))
		settings.shapeLineColor = replaceMap[settings.shapeLineColor];
	if (replaceMap.contains(settings.shapeFillColor))
		settings.shapeFillColor = replaceMap[settings.shapeFillColor];
	if (replaceMap.contains(settings.lineColor))
		settings.lineColor = replaceMap[settings.lineColor];
	if (replaceMap.contains(settings.imageFillColor))
		settings.imageFillColor = replaceMap[settings.imageFillColor];
}

void PrefsManager::setColorSet(const ColorList& colorSet)
{
	// Color set may have changed and tools color not be present in the new color set
	ColorList tmpSet = colorSet;
	QString penText = appPrefs.itemToolPrefs.textColor;
	if (!tmpSet.contains(penText) && penText != CommonStrings::None)
		tmpSet[penText] = appPrefs.colorPrefs.DColors[penText];
	QString strokeText = appPrefs.itemToolPrefs.textStrokeColor;
	if (!tmpSet.contains(strokeText) && strokeText != CommonStrings::None)
		tmpSet[strokeText] = appPrefs.colorPrefs.DColors[strokeText];
	QString textFillColor = appPrefs.itemToolPrefs.textFillColor;
	if (!tmpSet.contains(textFillColor) && textFillColor != CommonStrings::None)
		tmpSet[textFillColor] = appPrefs.colorPrefs.DColors[textFillColor];
	QString textLineColor = appPrefs.itemToolPrefs.textLineColor;
	if (!tmpSet.contains(textLineColor) && textLineColor != CommonStrings::None)
		tmpSet[textLineColor] = appPrefs.colorPrefs.DColors[textLineColor];
	QString pen = appPrefs.itemToolPrefs.shapeLineColor;
	if (!tmpSet.contains(pen) && pen != CommonStrings::None)
		tmpSet[pen] = appPrefs.colorPrefs.DColors[pen];
	QString brush = appPrefs.itemToolPrefs.shapeFillColor;
	if (!tmpSet.contains(brush) && brush != CommonStrings::None)
		tmpSet[brush] = appPrefs.colorPrefs.DColors[brush];
	QString penLine = appPrefs.itemToolPrefs.lineColor;
	if (!tmpSet.contains(penLine) && penLine != CommonStrings::None)
		tmpSet[penLine] = appPrefs.colorPrefs.DColors[penLine];
	QString brushPict = appPrefs.itemToolPrefs.imageFillColor;
	if (!tmpSet.contains(brushPict) && brushPict != CommonStrings::None)
		tmpSet[brushPict] = appPrefs.colorPrefs.DColors[brushPict];
	appPrefs.colorPrefs.DColors = tmpSet;
}

void PrefsManager::setColorSetName(const QString& colorSetName)
{
	appPrefs.colorPrefs.DColorSet=colorSetName;
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
			appPrefs.keyShortcutPrefs.KeyActions.insert(actName, ke);
		}
		else
			qDebug("%s", QString("Action Name: %1 does not exist").arg(actName).toAscii().constData());
	}
}

double PrefsManager::displayScale() const
{
	return appPrefs.displayPrefs.displayScale;
}

const QString& PrefsManager::uiLanguage() const
{
	return appPrefs.uiPrefs.language;
}

const QString& PrefsManager::guiStyle() const
{
	return appPrefs.uiPrefs.style;
}

const QString& PrefsManager::guiSystemStyle() const
{
	return appPrefs.ui_SystemTheme;
}

const int& PrefsManager::guiFontSize() const
{
	return appPrefs.uiPrefs.applicationFontSize;
}

const int& PrefsManager::paletteFontSize() const
{
	return appPrefs.uiPrefs.paletteFontSize;
}

bool PrefsManager::showPageShadow() const
{
	return appPrefs.displayPrefs.showPageShadow;
}

bool PrefsManager::WritePref(QString ho)
{
	QDomDocument docu("scribusrc");
	QString st="<SCRIBUSRC></SCRIBUSRC>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	elem.setAttribute("VERSION","1.3.5");
	QDomElement dc=docu.createElement("GUI");
	dc.setAttribute("UI_THEME",appPrefs.uiPrefs.style);
	dc.setAttribute("UI_WHEELJUMP",appPrefs.uiPrefs.wheelJump);
	dc.setAttribute("UI_MOUSEMOVETIMEOUT", appPrefs.uiPrefs.mouseMoveTimeout);
	dc.setAttribute("UI_APPLICATIONFONTSIZE",appPrefs.uiPrefs.applicationFontSize);
	dc.setAttribute("UI_PALETTEFONTSIZE",appPrefs.uiPrefs.paletteFontSize);
	dc.setAttribute("GRAB",appPrefs.guidesPrefs.grabRadius);
	dc.setAttribute("UNIT",appPrefs.docSetupPrefs.docUnitIndex);
	dc.setAttribute("UI_RECENTDOCCOUNT", appPrefs.uiPrefs.recentDocCount);
	dc.setAttribute("DOC", appPrefs.pathPrefs.documents);
	dc.setAttribute("PROFILES", appPrefs.pathPrefs.colorProfiles);
	dc.setAttribute("SCRIPTS", appPrefs.pathPrefs.scripts);
	dc.setAttribute("TEMPLATES", appPrefs.pathPrefs.documentTemplates);
	dc.setAttribute("SHOWGUIDES", static_cast<int>(appPrefs.guidesPrefs.guidesShown));
	dc.setAttribute("showcolborders", static_cast<int>(appPrefs.guidesPrefs.colBordersShown));
	dc.setAttribute("FRV", static_cast<int>(appPrefs.guidesPrefs.framesShown));
	dc.setAttribute("SHOWLAYERM", static_cast<int>(appPrefs.guidesPrefs.layerMarkersShown));
	dc.setAttribute("SHOWMARGIN", static_cast<int>(appPrefs.guidesPrefs.marginsShown));
	dc.setAttribute("SHOWBASE", static_cast<int>(appPrefs.guidesPrefs.baselineGridShown));
	dc.setAttribute("SHOWLINK", static_cast<int>(appPrefs.guidesPrefs.linkShown));
	dc.setAttribute("SHOWPICT", static_cast<int>(appPrefs.guidesPrefs.showPic));
	dc.setAttribute("SHOWControl", static_cast<int>(appPrefs.guidesPrefs.showControls));
	dc.setAttribute("rulersShown", static_cast<int>(appPrefs.guidesPrefs.rulersShown));
	dc.setAttribute("showBleed", static_cast<int>(appPrefs.guidesPrefs.showBleed));
	dc.setAttribute("rulerMode", static_cast<int>(appPrefs.guidesPrefs.rulerMode));
	dc.setAttribute("ScratchBottom", appPrefs.displayPrefs.scratch.Bottom);
	dc.setAttribute("ScratchLeft", appPrefs.displayPrefs.scratch.Left);
	dc.setAttribute("ScratchRight", appPrefs.displayPrefs.scratch.Right);
	dc.setAttribute("ScratchTop", appPrefs.displayPrefs.scratch.Top);
	dc.setAttribute("GapHorizontal", ScCLocale::toQStringC(appPrefs.displayPrefs.pageGapHorizontal));
	dc.setAttribute("GapVertical", ScCLocale::toQStringC(appPrefs.displayPrefs.pageGapVertical));
	dc.setAttribute("STECOLOR", appPrefs.storyEditorPrefs.guiFontColor.name());
	dc.setAttribute("STEFONT", appPrefs.storyEditorPrefs.guiFont);
	dc.setAttribute("STYLEPREVIEW", static_cast<int>(appPrefs.miscPrefs.haveStylePreview));
	dc.setAttribute("UI_SHOWSTARTUPDIALOG", static_cast<int>(appPrefs.uiPrefs.showStartupDialog));
	dc.setAttribute("UI_SHOWSPLASHSCREEN", static_cast<int>(appPrefs.uiPrefs.showSplashOnStartup));
	dc.setAttribute("UI_USESMALLWIDGETS", static_cast<int>(appPrefs.uiPrefs.useSmallWidgets));
	dc.setAttribute("UI_USESTABS", static_cast<int>(appPrefs.uiPrefs.useTabs));
	dc.setAttribute("ToolTips", static_cast<int>(appPrefs.displayPrefs.showToolTips));
	dc.setAttribute("showMouseCoordinates", static_cast<int>(appPrefs.displayPrefs.showMouseCoordinates));
	dc.setAttribute("stickyTools", static_cast<int>(appPrefs.uiPrefs.stickyTools));
	elem.appendChild(dc);
	QDomElement dc1=docu.createElement("GRID");
	dc1.setAttribute("MINOR",ScCLocale::toQStringC(appPrefs.guidesPrefs.minorGridSpacing));
	dc1.setAttribute("MAJOR",ScCLocale::toQStringC(appPrefs.guidesPrefs.majorGridSpacing));
	dc1.setAttribute("MINORC",appPrefs.guidesPrefs.minorGridColor.name());
	dc1.setAttribute("MAJORC",appPrefs.guidesPrefs.majorGridColor.name());
	dc1.setAttribute("GuideC", appPrefs.guidesPrefs.guideColor.name());
	dc1.setAttribute("BaseC", appPrefs.guidesPrefs.baselineGridColor.name());
	dc1.setAttribute("GuideZ", ScCLocale::toQStringC(appPrefs.guidesPrefs.guideRad));
	dc1.setAttribute("BACKG", static_cast<int>(appPrefs.guidesPrefs.guidePlacement));
	dc1.setAttribute("SHOW", static_cast<int>(appPrefs.guidesPrefs.gridShown));
	elem.appendChild(dc1);
	QDomElement dc1a=docu.createElement("PAGE");
	dc1a.setAttribute("ShowPageShadow",static_cast<int>(appPrefs.displayPrefs.showPageShadow));
	dc1a.setAttribute("PAGEC",appPrefs.displayPrefs.paperColor.name());
	dc1a.setAttribute("SELEC",appPrefs.displayPrefs.frameColor.name());
	dc1a.setAttribute("DFrameNormColor",appPrefs.displayPrefs.frameNormColor.name());
	dc1a.setAttribute("DFrameGroupColor",appPrefs.displayPrefs.frameGroupColor.name());
	dc1a.setAttribute("DFrameLockColor",appPrefs.displayPrefs.frameLockColor.name());
	dc1a.setAttribute("DFrameLinkColor",appPrefs.displayPrefs.frameLinkColor.name());
	dc1a.setAttribute("DFrameAnnotationColor",appPrefs.displayPrefs.frameAnnotationColor.name());
	dc1a.setAttribute("DPageBorderColor",appPrefs.displayPrefs.pageBorderColor.name());
	dc1a.setAttribute("DControlCharColor",appPrefs.displayPrefs.controlCharColor.name());
	dc1a.setAttribute("MARGC",appPrefs.guidesPrefs.marginColor.name());
	dc1a.setAttribute("RANDF", static_cast<int>(appPrefs.displayPrefs.marginColored));
	dc1a.setAttribute("DScale", ScCLocale::toQStringC(appPrefs.displayPrefs.displayScale));
	dc1a.setAttribute("ShowVerifierWarningsOnCanvas",static_cast<int>(appPrefs.displayPrefs.showVerifierWarningsOnCanvas));

	elem.appendChild(dc1a);
	// Font information must be written before FONTS element so that face "usable"
	// member is set properly before one try to set default font. Allows to check
	// that default font is indeed usable, problems expected otherwise
	for ( SCFontsIterator itf(appPrefs.fontPrefs.AvailFonts); itf.hasNext(); itf.next())
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
	for (itfsu = appPrefs.fontPrefs.GFontSub.begin(); itfsu != appPrefs.fontPrefs.GFontSub.end(); ++itfsu)
	{
		QDomElement fosu = docu.createElement("Substitute");
		fosu.setAttribute("Name",itfsu.key());
		fosu.setAttribute("Replace",itfsu.value());
		elem.appendChild(fosu);
	}
	QDomElement dc2=docu.createElement("FONTS");
	dc2.setAttribute("FACE",appPrefs.itemToolPrefs.textFont);
	dc2.setAttribute("SIZE",appPrefs.itemToolPrefs.textSize / 10.0);
	dc2.setAttribute("AutomaticSubst", static_cast<int>(appPrefs.fontPrefs.askBeforeSubstitute));
	elem.appendChild(dc2);
	QDomElement dc3=docu.createElement("TYPO");
	dc3.setAttribute("TIEF",appPrefs.typoPrefs.valueSubScript);
	dc3.setAttribute("TIEFSC",appPrefs.typoPrefs.scalingSubScript);
	dc3.setAttribute("HOCH",appPrefs.typoPrefs.valueSuperScript);
	dc3.setAttribute("HOCHSC",appPrefs.typoPrefs.scalingSuperScript);
	dc3.setAttribute("SMCAPS",appPrefs.typoPrefs.valueSmallCaps);
	dc3.setAttribute("AUTOL", appPrefs.typoPrefs.autoLineSpacing);
	dc3.setAttribute("BASE", ScCLocale::toQStringC(appPrefs.guidesPrefs.valueBaselineGrid));
	dc3.setAttribute("BASEO", ScCLocale::toQStringC(appPrefs.guidesPrefs.offsetBaselineGrid));
	if (appPrefs.typoPrefs.valueUnderlinePos == -1)
		dc3.setAttribute("UnderlinePos", appPrefs.typoPrefs.valueUnderlinePos);
	else
		dc3.setAttribute("UnderlinePos", appPrefs.typoPrefs.valueUnderlinePos / 10.0);
	if (appPrefs.typoPrefs.valueUnderlineWidth == -1)
		dc3.setAttribute("UnderlineWidth", appPrefs.typoPrefs.valueUnderlineWidth);
	else
		dc3.setAttribute("UnderlineWidth", appPrefs.typoPrefs.valueUnderlineWidth / 10.0);
	if (appPrefs.typoPrefs.valueStrikeThruPos == -1)
		dc3.setAttribute("StrikeThruPos", appPrefs.typoPrefs.valueStrikeThruPos);
	else
		dc3.setAttribute("StrikeThruPos", appPrefs.typoPrefs.valueStrikeThruPos / 10.0);
	if (appPrefs.typoPrefs.valueStrikeThruWidth == -1)
		dc3.setAttribute("StrikeThruWidth", appPrefs.typoPrefs.valueStrikeThruWidth);
	else
		dc3.setAttribute("StrikeThruWidth", appPrefs.typoPrefs.valueStrikeThruWidth / 10.0);
	elem.appendChild(dc3);
	QDomElement dc9=docu.createElement("TOOLS");
	dc9.setAttribute("PEN",appPrefs.itemToolPrefs.shapeLineColor);
	dc9.setAttribute("BRUSH",appPrefs.itemToolPrefs.shapeFillColor);
	dc9.setAttribute("PENLINE",appPrefs.itemToolPrefs.lineColor);
	dc9.setAttribute("PENTEXT",appPrefs.itemToolPrefs.textColor);
	dc9.setAttribute("StrokeText",appPrefs.itemToolPrefs.textStrokeColor);
	dc9.setAttribute("TextBackGround", appPrefs.itemToolPrefs.textFillColor);
	dc9.setAttribute("TextLineColor", appPrefs.itemToolPrefs.textLineColor);
	dc9.setAttribute("TextBackGroundShade", appPrefs.itemToolPrefs.textFillColorShade);
	dc9.setAttribute("TextLineShade", appPrefs.itemToolPrefs.textLineColorShade);
	dc9.setAttribute("TextPenShade", appPrefs.itemToolPrefs.textShade);
	dc9.setAttribute("TextStrokeShade", appPrefs.itemToolPrefs.textStrokeShade);
	dc9.setAttribute("TEXTCOL",appPrefs.itemToolPrefs.textColumns);
	dc9.setAttribute("TEXTGAP",ScCLocale::toQStringC(appPrefs.itemToolPrefs.textColumnGap));
	dc9.setAttribute("TabWidth",ScCLocale::toQStringC(appPrefs.itemToolPrefs.textTabWidth));
	dc9.setAttribute("TabFill",appPrefs.itemToolPrefs.textTabFillChar);
	dc9.setAttribute("STIL",appPrefs.itemToolPrefs.shapeLineStyle);
	dc9.setAttribute("STILLINE",appPrefs.itemToolPrefs.lineStyle);
	dc9.setAttribute("WIDTH",ScCLocale::toQStringC(appPrefs.itemToolPrefs.shapeLineWidth));
	dc9.setAttribute("WIDTHLINE",ScCLocale::toQStringC(appPrefs.itemToolPrefs.lineWidth));
	dc9.setAttribute("PENSHADE",appPrefs.itemToolPrefs.shapeLineColorShade);
	dc9.setAttribute("LINESHADE",appPrefs.itemToolPrefs.lineColorShade);
	dc9.setAttribute("BRUSHSHADE",appPrefs.itemToolPrefs.shapeFillColorShade);
	dc9.setAttribute("MAGMIN",appPrefs.opToolPrefs.magMin);
	dc9.setAttribute("MAGMAX",appPrefs.opToolPrefs.magMax);
	dc9.setAttribute("MAGSTEP",appPrefs.opToolPrefs.magStep);
	dc9.setAttribute("CPICT",appPrefs.itemToolPrefs.imageFillColor);
	dc9.setAttribute("PICTSHADE",appPrefs.itemToolPrefs.imageFillColorShade);
	dc9.setAttribute("PICTSCX",ScCLocale::toQStringC(appPrefs.itemToolPrefs.imageScaleX));
	dc9.setAttribute("PICTSCY",ScCLocale::toQStringC(appPrefs.itemToolPrefs.imageScaleY));
	dc9.setAttribute("POLYC", appPrefs.itemToolPrefs.polyCorners);
	dc9.setAttribute("POLYF", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyFactor));
	dc9.setAttribute("POLYR", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyRotation));
	dc9.setAttribute("POLYFD", appPrefs.itemToolPrefs.polyFactorGuiVal);
	dc9.setAttribute("POLYCUR", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyCurvature));
	dc9.setAttribute("POLYS", static_cast<int>(appPrefs.itemToolPrefs.polyUseFactor));
	dc9.setAttribute("PSCALE", static_cast<int>(appPrefs.itemToolPrefs.imageScaleType));
	dc9.setAttribute("PASPECT", static_cast<int>(appPrefs.itemToolPrefs.imageAspectRatio));
	dc9.setAttribute("EmbeddedPath", static_cast<int>(appPrefs.itemToolPrefs.imageUseEmbeddedPath));
	dc9.setAttribute("HalfRes", appPrefs.itemToolPrefs.imageLowResType);
	dc9.setAttribute("StartArrow", appPrefs.itemToolPrefs.lineStartArrow);
	dc9.setAttribute("EndArrow", appPrefs.itemToolPrefs.lineEndArrow);
	dc9.setAttribute("dispX", ScCLocale::toQStringC(appPrefs.opToolPrefs.dispX));
	dc9.setAttribute("dispY", ScCLocale::toQStringC(appPrefs.opToolPrefs.dispY));
	dc9.setAttribute("constrain", ScCLocale::toQStringC(appPrefs.opToolPrefs.constrain));
	elem.appendChild(dc9);
	QDomElement dc4=docu.createElement("MAINWINDOW");
	dc4.setAttribute("XPOS",appPrefs.uiPrefs.mainWinSettings.xPosition);
	dc4.setAttribute("YPOS",appPrefs.uiPrefs.mainWinSettings.yPosition);
	dc4.setAttribute("WIDTH",appPrefs.uiPrefs.mainWinSettings.width);
	dc4.setAttribute("HEIGHT",appPrefs.uiPrefs.mainWinSettings.height);
	dc4.setAttribute("MAXIMIZED",static_cast<int>(appPrefs.uiPrefs.mainWinSettings.maximized));
	elem.appendChild(dc4);
	QDomElement dc73=docu.createElement("SCRAPBOOK");
	dc73.setAttribute("CopyToScrapbook",static_cast<int>(appPrefs.scrapbookPrefs.doCopyToScrapbook));
	dc73.setAttribute("persistentScrapbook",static_cast<int>(appPrefs.scrapbookPrefs.persistentScrapbook));
	dc73.setAttribute("writePreviews",static_cast<int>(appPrefs.scrapbookPrefs.writePreviews));
	dc73.setAttribute("numScrapbookCopies",appPrefs.scrapbookPrefs.numScrapbookCopies);
	for (int rd=0; rd<appPrefs.scrapbookPrefs.RecentScrapbooks.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",appPrefs.scrapbookPrefs.RecentScrapbooks[rd]);
		dc73.appendChild(rde);
	}
	elem.appendChild(dc73);
	QDomElement dc75=docu.createElement("PAGEPALETTE");
	dc75.setAttribute("THUMBS", static_cast<int>(appPrefs.uiPrefs.SepalT));
	dc75.setAttribute("NAMES", static_cast<int>(appPrefs.uiPrefs.SepalN));
	elem.appendChild(dc75);
	QDomElement dc76=docu.createElement("DOKUMENT");
	dc76.setAttribute("PAGESIZE",appPrefs.docSetupPrefs.pageSize);
	dc76.setAttribute("AUSRICHTUNG",appPrefs.docSetupPrefs.pageOrientation);
	dc76.setAttribute("BREITE",ScCLocale::toQStringC(appPrefs.docSetupPrefs.pageWidth));
	dc76.setAttribute("HOEHE",ScCLocale::toQStringC(appPrefs.docSetupPrefs.pageHeight));
	dc76.setAttribute("RANDO",ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.Top));
	dc76.setAttribute("RANDU",ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.Bottom));
	dc76.setAttribute("RANDL",ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.Left));
	dc76.setAttribute("RANDR",ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.Right));
	dc76.setAttribute("PRESET",appPrefs.docSetupPrefs.marginPreset);
	dc76.setAttribute("DOPPEL", appPrefs.docSetupPrefs.pagePositioning);
	dc76.setAttribute("AutoSave", static_cast<int>(appPrefs.docSetupPrefs.AutoSave));
	dc76.setAttribute("AutoSaveTime", appPrefs.docSetupPrefs.AutoSaveTime);
	dc76.setAttribute("SaveCompressed", static_cast<int>(appPrefs.docSetupPrefs.saveCompressed));
	dc76.setAttribute("BleedTop", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.Top));
	dc76.setAttribute("BleedLeft", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.Left));
	dc76.setAttribute("BleedRight", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.Right));
	dc76.setAttribute("BleedBottom", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.Bottom));
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
	CheckerPrefsList::Iterator itcpend=appPrefs.checkerPrefsList.end();
	for (itcp = appPrefs.checkerPrefsList.begin(); itcp != itcpend; ++itcp)
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
		dc79a.setAttribute("checkNotCMYKOrSpot", static_cast<int>(itcp.value().checkNotCMYKOrSpot));
		dc79a.setAttribute("checkDeviceColorsAndOutputIntend", static_cast<int>(itcp.value().checkDeviceColorsAndOutputIntend));
		dc79a.setAttribute("checkFontNotEmbedded", static_cast<int>(itcp.value().checkFontNotEmbedded));
		dc79a.setAttribute("checkFontIsOpenType", static_cast<int>(itcp.value().checkFontIsOpenType));
		elem.appendChild(dc79a);
	}
	QDomElement dc81=docu.createElement("CMS");
	dc81.setAttribute("DPSo", static_cast<int>(appPrefs.colorPrefs.DCMSset.SoftProofOn));
	dc81.setAttribute("DPSFo", static_cast<int>(appPrefs.colorPrefs.DCMSset.SoftProofFullOn));
	dc81.setAttribute("DPuse", static_cast<int>(appPrefs.colorPrefs.DCMSset.CMSinUse));
	dc81.setAttribute("DPgam", static_cast<int>(appPrefs.colorPrefs.DCMSset.GamutCheck));
	dc81.setAttribute("DPbla", static_cast<int>(appPrefs.colorPrefs.DCMSset.BlackPoint));
	dc81.setAttribute("DPMo",appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile);
	dc81.setAttribute("DPPr",appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile);
	dc81.setAttribute("DPIn",appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile);
	dc81.setAttribute("DPInCMYK",appPrefs.colorPrefs.DCMSset.DefaultImageCMYKProfile);
	dc81.setAttribute("DPIn2",appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile);
	dc81.setAttribute("DPIn3",appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile);
	//dc81.setAttribute("DIPr",appPrefs.colorPrefs.DCMSset.DefaultIntentPrinter);
	//dc81.setAttribute("DIMo",appPrefs.colorPrefs.DCMSset.DefaultIntentMonitor);
	dc81.setAttribute("DISc",appPrefs.colorPrefs.DCMSset.DefaultIntentColors);
	dc81.setAttribute("DIIm",appPrefs.colorPrefs.DCMSset.DefaultIntentImages);
	elem.appendChild(dc81);
	QDomElement dc82=docu.createElement("PRINTER");
	dc82.setAttribute("NAME",appPrefs.printerPrefs.PrinterName);
	dc82.setAttribute("FILE",appPrefs.printerPrefs.PrinterFile);
	dc82.setAttribute("COMMAND",appPrefs.printerPrefs.PrinterCommand);
	dc82.setAttribute("CLIPMARGIN", static_cast<int>(appPrefs.printerPrefs.ClipMargin));
	dc82.setAttribute("GMODE", static_cast<int>(appPrefs.printerPrefs.GCRMode));
	elem.appendChild(dc82);
	QDomElement dc8Pr=docu.createElement("PRINTPREVIEW");
	dc8Pr.setAttribute("Mode", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_Mode));
	//dc8Pr.setAttribute("GcrMode", static_cast<int>(appPrefs.Gcr_Mode));
	dc8Pr.setAttribute("AntiAliasing", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_AntiAliasing));
	dc8Pr.setAttribute("Transparency", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_Transparency));
	dc8Pr.setAttribute("Cyan", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_C));
	dc8Pr.setAttribute("Magenta", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_M));
	dc8Pr.setAttribute("Yellow", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_Y));
	dc8Pr.setAttribute("Black", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_K));
	dc8Pr.setAttribute("InkCoverage", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_InkCoverage));
	dc8Pr.setAttribute("InkThreshold", appPrefs.printPreviewPrefs.PrPr_InkThreshold);
	elem.appendChild(dc8Pr);
	QDomElement dc8Ex = docu.createElement("EXTERNAL");
	dc8Ex.setAttribute("GIMP", imageEditorExecutable());
	dc8Ex.setAttribute("GS", ghostscriptExecutable());
	dc8Ex.setAttribute("WebBrowser", extBrowserExecutable());
	dc8Ex.setAttribute("AlphaGraphics", static_cast<int>(appPrefs.extToolPrefs.gs_AntiAliasGraphics));
	dc8Ex.setAttribute("AlphaText", static_cast<int>(appPrefs.extToolPrefs.gs_AntiAliasText));
	dc8Ex.setAttribute("Resolution", appPrefs.extToolPrefs.gs_Resolution);
	dc8Ex.setAttribute("Uniconv", uniconvExecutable());
	dc8Ex.setAttribute("LatexEditor", latexEditorExecutable());
	dc8Ex.setAttribute("LatexResolution", latexResolution());
	dc8Ex.setAttribute("LatexForceDpi", static_cast<int>(appPrefs.extToolPrefs.latexForceDpi));
	dc8Ex.setAttribute("LatexStartWithEmptyFrames", static_cast<int>(appPrefs.extToolPrefs.latexStartWithEmptyFrames));
	QStringList configs = latexConfigs();
	foreach (QString config, configs) {
		QDomElement domConfig = docu.createElement("LatexConfig");
		domConfig.setAttribute("file", config);
		domConfig.setAttribute("command", appPrefs.extToolPrefs.latexCommands[config]);
		dc8Ex.appendChild(domConfig);
	}
	elem.appendChild(dc8Ex);
	QDomElement rde=docu.createElement("HYPHEN");
	rde.setAttribute("LANG", appPrefs.hyphPrefs.Language);
	rde.setAttribute("WORDLEN", appPrefs.hyphPrefs.MinWordLen);
	rde.setAttribute("HYCOUNT", appPrefs.hyphPrefs.HyCount);
	rde.setAttribute("MODE", static_cast<int>(appPrefs.hyphPrefs.Automatic));
	rde.setAttribute("INMODE", static_cast<int>(appPrefs.hyphPrefs.AutoCheck));
	for (QHash<QString, QString>::Iterator hyit = appPrefs.hyphPrefs.specialWords.begin(); hyit != appPrefs.hyphPrefs.specialWords.end(); ++hyit)
	{
		QDomElement hyelm = docu.createElement("EXCEPTION");
		hyelm.setAttribute("WORD", hyit.key());
		hyelm.setAttribute("HYPHENATED", hyit.value());
		rde.appendChild(hyelm);
	}
	for (QSet<QString>::Iterator hyit2 = appPrefs.hyphPrefs.ignoredWords.begin(); hyit2 != appPrefs.hyphPrefs.ignoredWords.end(); ++hyit2)
	{
		QDomElement hyelm2 = docu.createElement("IGNORE");
		hyelm2.setAttribute("WORD", (*hyit2));
		rde.appendChild(hyelm2);
	}
	elem.appendChild(rde);
	ColorList::Iterator itc;
	for (itc = appPrefs.colorPrefs.DColors.begin(); itc != appPrefs.colorPrefs.DColors.end(); ++itc)
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
	QMap<QString, VGradient>::Iterator itGrad;
	for (itGrad = appPrefs.defaultGradients.begin(); itGrad != appPrefs.defaultGradients.end(); ++itGrad)
	{
		QDomElement grad = docu.createElement("Gradient");
		grad.setAttribute("Name",itGrad.key());
		VGradient gra = itGrad.value();
		QList<VColorStop*> cstops = gra.colorStops();
		for (uint cst = 0; cst < gra.Stops(); ++cst)
		{
			QDomElement stop = docu.createElement("CSTOP");
			stop.setAttribute("NAME", cstops.at(cst)->name);
			stop.setAttribute("RAMP", ScCLocale::toQStringC(cstops.at(cst)->rampPoint));
			stop.setAttribute("TRANS", ScCLocale::toQStringC(cstops.at(cst)->opacity));
			stop.setAttribute("SHADE", cstops.at(cst)->shade);
			grad.appendChild(stop);
		}
		elem.appendChild(grad);
	}
	for (int rd=0; rd<appPrefs.uiPrefs.RecentDocs.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",appPrefs.uiPrefs.RecentDocs[rd]);
		elem.appendChild(rde);
	}
	for (QMap<QString,Keys>::Iterator ksc=appPrefs.keyShortcutPrefs.KeyActions.begin(); ksc!=appPrefs.keyShortcutPrefs.KeyActions.end(); ++ksc)
	{
		if (ksc.value().actionName.isEmpty())
			continue;
		QDomElement kscc=docu.createElement("SHORTCUT");
		kscc.setAttribute("ACTION",ksc.value().actionName);
		kscc.setAttribute("SEQUENCE",TabKeyboardShortcutsWidget::getKeyText(ksc.value().keySequence));
		elem.appendChild(kscc);
	}
	for (int ccs=0; ccs<appPrefs.colorPrefs.CustomColorSets.count(); ++ccs)
	{
		QDomElement cos=docu.createElement("COLORSET");
		cos.setAttribute("NAME",appPrefs.colorPrefs.CustomColorSets[ccs]);
		elem.appendChild(cos);
	}
	QDomElement cosd=docu.createElement("DCOLORSET");
	cosd.setAttribute("NAME",appPrefs.colorPrefs.DColorSet);
	elem.appendChild(cosd);
	QDomElement pdf = docu.createElement("PDF");
	pdf.setAttribute("Thumbnails", static_cast<int>(appPrefs.pdfPrefs.Thumbnails));
	pdf.setAttribute("Articles", static_cast<int>(appPrefs.pdfPrefs.Articles));
	pdf.setAttribute("Bookmarks", static_cast<int>(appPrefs.pdfPrefs.Bookmarks));
	pdf.setAttribute("Compress", static_cast<int>(appPrefs.pdfPrefs.Compress));
	pdf.setAttribute("CMethod", appPrefs.pdfPrefs.CompressMethod);
	pdf.setAttribute("Quality", appPrefs.pdfPrefs.Quality);
	pdf.setAttribute("EmbedPDF", static_cast<int>(appPrefs.pdfPrefs.embedPDF));
	pdf.setAttribute("MirrorH", static_cast<int>(appPrefs.pdfPrefs.MirrorH));
	pdf.setAttribute("MirrorV", static_cast<int>(appPrefs.pdfPrefs.MirrorV));
	pdf.setAttribute("Clip", static_cast<int>(appPrefs.pdfPrefs.doClip));
	pdf.setAttribute("RotateDeg", static_cast<int>(appPrefs.pdfPrefs.RotateDeg));
	pdf.setAttribute("PresentMode", static_cast<int>(appPrefs.pdfPrefs.PresentMode));
	pdf.setAttribute("RecalcPic", static_cast<int>(appPrefs.pdfPrefs.RecalcPic));
	pdf.setAttribute("Grayscale", static_cast<int>(appPrefs.pdfPrefs.isGrayscale));
	pdf.setAttribute("RGBMode", static_cast<int>(appPrefs.pdfPrefs.UseRGB));
	pdf.setAttribute("UseProfiles", static_cast<int>(appPrefs.pdfPrefs.UseProfiles));
	pdf.setAttribute("UseProfiles2", static_cast<int>(appPrefs.pdfPrefs.UseProfiles2));
	pdf.setAttribute("Binding", appPrefs.pdfPrefs.Binding);
	pdf.setAttribute("PicRes", appPrefs.pdfPrefs.PicRes);
	pdf.setAttribute("Resolution", appPrefs.pdfPrefs.Resolution);
	pdf.setAttribute("Version", appPrefs.pdfPrefs.Version);
	pdf.setAttribute("Intent", appPrefs.pdfPrefs.Intent);
	pdf.setAttribute("Intent2", appPrefs.pdfPrefs.Intent2);
	pdf.setAttribute("SolidP", appPrefs.pdfPrefs.SolidProf);
	pdf.setAttribute("ImageP", appPrefs.pdfPrefs.ImageProf);
	pdf.setAttribute("PrintP", appPrefs.pdfPrefs.PrintProf);
	pdf.setAttribute("InfoString", appPrefs.pdfPrefs.Info);
	pdf.setAttribute("BTop", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.Top));
	pdf.setAttribute("BLeft", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.Left));
	pdf.setAttribute("BRight", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.Right));
	pdf.setAttribute("BBottom", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.Bottom));
	pdf.setAttribute("useDocBleeds", static_cast<int>(appPrefs.pdfPrefs.useDocBleeds));
	pdf.setAttribute("cropMarks", static_cast<int>(appPrefs.pdfPrefs.cropMarks));
	pdf.setAttribute("bleedMarks", static_cast<int>(appPrefs.pdfPrefs.bleedMarks));
	pdf.setAttribute("registrationMarks", static_cast<int>(appPrefs.pdfPrefs.registrationMarks));
	pdf.setAttribute("colorMarks", static_cast<int>(appPrefs.pdfPrefs.colorMarks));
	pdf.setAttribute("docInfoMarks", static_cast<int>(appPrefs.pdfPrefs.docInfoMarks));
	pdf.setAttribute("markOffset", appPrefs.pdfPrefs.markOffset);
	pdf.setAttribute("ImagePr", static_cast<int>(appPrefs.pdfPrefs.EmbeddedI));
	pdf.setAttribute("PassOwner", appPrefs.pdfPrefs.PassOwner);
	pdf.setAttribute("PassUser", appPrefs.pdfPrefs.PassUser);
	pdf.setAttribute("Permissions", appPrefs.pdfPrefs.Permissions);
	pdf.setAttribute("Encrypt", static_cast<int>(appPrefs.pdfPrefs.Encrypt));
	pdf.setAttribute("UseLayers", static_cast<int>(appPrefs.pdfPrefs.useLayers));
	pdf.setAttribute("UseLpi", static_cast<int>(appPrefs.pdfPrefs.UseLPI));
	pdf.setAttribute("UseSpotColors", static_cast<int>(appPrefs.pdfPrefs.UseSpotColors));
	pdf.setAttribute("doMultiFile", static_cast<int>(appPrefs.pdfPrefs.doMultiFile));
	pdf.setAttribute("displayBookmarks", static_cast<int>(appPrefs.pdfPrefs.displayBookmarks));
	pdf.setAttribute("displayFullscreen", static_cast<int>(appPrefs.pdfPrefs.displayFullscreen));
	pdf.setAttribute("displayLayers", static_cast<int>(appPrefs.pdfPrefs.displayLayers));
	pdf.setAttribute("displayThumbs", static_cast<int>(appPrefs.pdfPrefs.displayThumbs));
	pdf.setAttribute("PageLayout", appPrefs.pdfPrefs.PageLayout);
	pdf.setAttribute("openAction", appPrefs.pdfPrefs.openAction);
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = appPrefs.pdfPrefs.LPISettings.begin(); itlp != appPrefs.pdfPrefs.LPISettings.end(); ++itlp)
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
	for(ObjAttrVector::Iterator objAttrIt = appPrefs.itemAttrPrefs.defaultItemAttributes.begin() ; objAttrIt != appPrefs.itemAttrPrefs.defaultItemAttributes.end(); ++objAttrIt )
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
	for(ToCSetupVector::Iterator tocSetupIt = appPrefs.tocPrefs.defaultToCSetups.begin() ; tocSetupIt != appPrefs.tocPrefs.defaultToCSetups.end(); ++tocSetupIt )
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
	liElem.setAttribute("useStandardLI", static_cast<int>(appPrefs.miscPrefs.useStandardLI));
	liElem.setAttribute("paragraphsLI", appPrefs.miscPrefs.paragraphsLI);
	elem.appendChild(liElem);
	// image cache
	QDomElement icElem = docu.createElement("ImageCache");
	icElem.setAttribute("cacheEnabled", appPrefs.imageCachePrefs.cacheEnabled);
	icElem.setAttribute("maxCacheSizeMiB", appPrefs.imageCachePrefs.maxCacheSizeMiB);
	icElem.setAttribute("maxCacheEntries", appPrefs.imageCachePrefs.maxCacheEntries);
	icElem.setAttribute("compressionLevel", appPrefs.imageCachePrefs.compressionLevel);
	elem.appendChild(icElem);
	// write file
	bool result = false;
	QFile f(ho);
	if(!f.open(QIODevice::WriteOnly))
	{
		m_lastError = tr("Could not open preferences file \"%1\" for writing: %2")
			.arg(ho).arg(qApp->translate("QFile",f.errorString().toLatin1().constData()));
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
							 "QIODevice status code %2")
				.arg(ho).arg(f.errorString());
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
	bool prefs135FileFound=false;
	if (elem.hasAttribute("VERSION"))
	{
		if (elem.attribute("VERSION") == "1.3.5")
			prefs135FileFound=true;
	}
	firstTimeIgnoreOldPrefs=!prefs135FileFound;
	if (!prefs135FileFound)
		return false;
	appPrefs.colorPrefs.DColors.clear();
	appPrefs.extToolPrefs.latexCommands.clear();
	ScColor lf = ScColor();
	QDomNode DOC=elem.firstChild();
	if (!DOC.namedItem("CheckProfile").isNull())
		appPrefs.checkerPrefsList.clear();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="GUI")
		{
			if (dc.hasAttribute("STILT"))
				appPrefs.uiPrefs.style = dc.attribute("STILT");
			else
				appPrefs.uiPrefs.style = dc.attribute("UI_THEME","Default");
			if (dc.hasAttribute("RAD"))
				appPrefs.uiPrefs.wheelJump = dc.attribute("RAD").toInt();
			else
				appPrefs.uiPrefs.wheelJump = dc.attribute("UI_WHEELJUMP").toInt();
			if (dc.hasAttribute("MOVT"))
				appPrefs.uiPrefs.mouseMoveTimeout = dc.attribute("MOVT").toInt();
			else
				appPrefs.uiPrefs.mouseMoveTimeout = dc.attribute("UI_MOUSEMOVETIMEOUT", "150").toInt();
			if (dc.hasAttribute("APF"))
				appPrefs.uiPrefs.applicationFontSize = dc.attribute("APF").toInt();
			else
				appPrefs.uiPrefs.applicationFontSize = dc.attribute("UI_APPLICATIONFONTSIZE", "12").toInt();
			if (dc.hasAttribute("PFS"))
				appPrefs.uiPrefs.paletteFontSize = dc.attribute("PFS").toInt();
			else
				appPrefs.uiPrefs.paletteFontSize = dc.attribute("UI_PALETTEFONTSIZE", "10").toInt();
			if (dc.hasAttribute("RCD"))
				appPrefs.uiPrefs.recentDocCount = dc.attribute("RCD").toInt();
			else
				appPrefs.uiPrefs.recentDocCount = dc.attribute("UI_RECENTDOCCOUNT","5").toUInt();
			if (dc.hasAttribute("StartUp"))
				appPrefs.uiPrefs.showStartupDialog = dc.attribute("StartUp").toInt();
			else
				appPrefs.uiPrefs.showStartupDialog = static_cast<bool>(dc.attribute("UI_SHOWSTARTUPDIALOG", "1").toInt());
			appPrefs.uiPrefs.showSplashOnStartup = static_cast<bool>(dc.attribute("UI_SHOWSPLASHSCREEN", "1").toInt());
			if (dc.hasAttribute("UseSmallWidgets"))
				appPrefs.uiPrefs.useSmallWidgets = dc.attribute("UseSmallWidgets").toInt();
			else
				appPrefs.uiPrefs.useSmallWidgets = static_cast<bool>(dc.attribute("UI_USESMALLWIDGETS", "0").toInt());
			appPrefs.uiPrefs.useTabs = static_cast<bool>(dc.attribute("UI_USESTABS", "0").toInt());
			appPrefs.pathPrefs.documents = dc.attribute("DOC","");
			appPrefs.pathPrefs.colorProfiles = dc.attribute("PROFILES","");
			appPrefs.pathPrefs.scripts = dc.attribute("SCRIPTS","");
			appPrefs.pathPrefs.documentTemplates = dc.attribute("TEMPLATES","");
			appPrefs.docSetupPrefs.docUnitIndex = dc.attribute("UNIT", "0").toInt();
			appPrefs.guidesPrefs.grabRadius = dc.attribute("GRAB", "4").toInt();
			appPrefs.guidesPrefs.guidesShown = static_cast<bool>(dc.attribute("SHOWGUIDES", "1").toInt());
			appPrefs.guidesPrefs.colBordersShown = static_cast<bool>(dc.attribute("showcolborders", "0").toInt());
			appPrefs.guidesPrefs.framesShown = static_cast<bool>(dc.attribute("FRV", "1").toInt());
			appPrefs.guidesPrefs.layerMarkersShown = static_cast<bool>(dc.attribute("SHOWLAYERM", "0").toInt());
			appPrefs.guidesPrefs.marginsShown = static_cast<bool>(dc.attribute("SHOWMARGIN", "1").toInt());
			appPrefs.guidesPrefs.baselineGridShown = static_cast<bool>(dc.attribute("SHOWBASE", "1").toInt());
			appPrefs.guidesPrefs.linkShown = static_cast<bool>(dc.attribute("SHOWLINK", "0").toInt());
			appPrefs.guidesPrefs.showPic = static_cast<bool>(dc.attribute("SHOWPICT", "1").toInt());
			appPrefs.guidesPrefs.showControls = static_cast<bool>(dc.attribute("SHOWControl", "0").toInt());
			appPrefs.guidesPrefs.rulersShown = static_cast<bool>(dc.attribute("rulersShown", "1").toInt());
			appPrefs.guidesPrefs.showBleed = static_cast<bool>(dc.attribute("showBleed", "1").toInt());
			appPrefs.guidesPrefs.rulerMode = static_cast<bool>(dc.attribute("rulerMode", "1").toInt());
			appPrefs.miscPrefs.haveStylePreview = static_cast<bool>(dc.attribute("STYLEPREVIEW", "1").toInt());

			appPrefs.displayPrefs.scratch.Bottom = ScCLocale::toDoubleC(dc.attribute("ScratchBottom"), 20.0);
			appPrefs.displayPrefs.scratch.Left   = ScCLocale::toDoubleC(dc.attribute("ScratchLeft"), 100.0);
			appPrefs.displayPrefs.scratch.Right  = ScCLocale::toDoubleC(dc.attribute("ScratchRight"), 100.0);
			appPrefs.displayPrefs.scratch.Top    = ScCLocale::toDoubleC(dc.attribute("ScratchTop"), 20.0);
			appPrefs.displayPrefs.pageGapHorizontal  = ScCLocale::toDoubleC(dc.attribute("GapHorizontal"), 0.0);
			appPrefs.displayPrefs.pageGapVertical    = ScCLocale::toDoubleC(dc.attribute("GapVertical"), 40.0);
			if (dc.hasAttribute("STECOLOR"))
				appPrefs.storyEditorPrefs.guiFontColor = QColor(dc.attribute("STECOLOR"));
			if (dc.hasAttribute("STEFONT"))
				appPrefs.storyEditorPrefs.guiFont = dc.attribute("STEFONT");
			appPrefs.displayPrefs.showToolTips = static_cast<bool>(dc.attribute("ToolTips", "1").toInt());
			appPrefs.displayPrefs.showMouseCoordinates = static_cast<bool>(dc.attribute("showMouseCoordinates", "1").toInt());
			appPrefs.uiPrefs.stickyTools = static_cast<bool>(dc.attribute("stickyTools", "0").toInt());
		}
		if (dc.tagName()=="GRID")
		{
			appPrefs.guidesPrefs.minorGridSpacing  = ScCLocale::toDoubleC(dc.attribute("MINOR"), 20.0);
			appPrefs.guidesPrefs.majorGridSpacing  = ScCLocale::toDoubleC(dc.attribute("MAJOR"), 100.0);
			appPrefs.guidesPrefs.minorGridColor = QColor(dc.attribute("MINORC"));
			appPrefs.guidesPrefs.majorGridColor = QColor(dc.attribute("MAJORC"));
			appPrefs.guidesPrefs.guidePlacement = static_cast<bool>(dc.attribute("BACKG", "1").toInt());
			appPrefs.guidesPrefs.gridShown = static_cast<bool>(dc.attribute("SHOW", "0").toInt());
			if (dc.hasAttribute("GuideC"))
				appPrefs.guidesPrefs.guideColor = QColor(dc.attribute("GuideC"));
			if (dc.hasAttribute("GuideZ"))
				appPrefs.guidesPrefs.guideRad = ScCLocale::toDoubleC(dc.attribute("GuideZ"), 10.0);
			if (dc.hasAttribute("BaseC"))
				appPrefs.guidesPrefs.baselineGridColor = QColor(dc.attribute("BaseC"));
		}
		if (dc.tagName()=="PAGE")
		{
			appPrefs.displayPrefs.showPageShadow = static_cast<bool>(dc.attribute("ShowPageShadow", "1").toInt());
			appPrefs.displayPrefs.paperColor = QColor(dc.attribute("PAGEC"));
			appPrefs.displayPrefs.frameColor = QColor(dc.attribute("SELEC","#ff0000"));
			appPrefs.displayPrefs.frameNormColor = QColor(dc.attribute("DFrameNormColor","#000000"));
			appPrefs.displayPrefs.frameGroupColor = QColor(dc.attribute("DFrameGroupColor","#008080"));
			appPrefs.displayPrefs.frameLockColor = QColor(dc.attribute("DFrameLockColor","#800000"));
			appPrefs.displayPrefs.frameLinkColor = QColor(dc.attribute("DFrameLinkColor","#ff0000"));
			appPrefs.displayPrefs.frameAnnotationColor = QColor(dc.attribute("DFrameAnnotationColor","#0000ff"));
			appPrefs.displayPrefs.pageBorderColor = QColor(dc.attribute("DPageBorderColor","#ff0000"));
			appPrefs.displayPrefs.controlCharColor = QColor(dc.attribute("DControlCharColor","#800000"));
			appPrefs.guidesPrefs.marginColor = QColor(dc.attribute("MARGC","#0000ff"));
			appPrefs.displayPrefs.marginColored = static_cast<bool>(dc.attribute("RANDF", "0").toInt());
			appPrefs.displayPrefs.displayScale = ScCLocale::toDoubleC(dc.attribute("DScale"), appPrefs.displayPrefs.displayScale);
			appPrefs.displayPrefs.showVerifierWarningsOnCanvas = static_cast<bool>(dc.attribute("ShowVerifierWarningsOnCanvas", "1").toInt());
		}
		if (dc.tagName()=="TYPO")
		{
			appPrefs.typoPrefs.valueSuperScript = dc.attribute("HOCH").toInt();
			appPrefs.typoPrefs.scalingSuperScript = dc.attribute("HOCHSC").toInt();
			appPrefs.typoPrefs.valueSubScript = dc.attribute("TIEF").toInt();
			appPrefs.typoPrefs.scalingSubScript = dc.attribute("TIEFSC").toInt();
			appPrefs.typoPrefs.valueSmallCaps  = dc.attribute("SMCAPS").toInt();
			appPrefs.guidesPrefs.valueBaselineGrid   = ScCLocale::toDoubleC(dc.attribute("BASE"), 12.0);
			appPrefs.guidesPrefs.offsetBaselineGrid  = ScCLocale::toDoubleC(dc.attribute("BASEO"), 0.0);
			appPrefs.typoPrefs.autoLineSpacing = dc.attribute("AUTOL", "20").toInt();
			double ulp = ScCLocale::toDoubleC(dc.attribute("UnderlinePos"), -1.0);
			if (ulp != -1)
				appPrefs.typoPrefs.valueUnderlinePos = qRound(ulp * 10);
			else
				appPrefs.typoPrefs.valueUnderlinePos = -1;
			double ulw = ScCLocale::toDoubleC(dc.attribute("UnderlineWidth"), -1.0);
			if (ulw != -1)
				appPrefs.typoPrefs.valueUnderlineWidth = qRound(ulw * 10);
			else
				appPrefs.typoPrefs.valueUnderlineWidth = -1;
			double stp = ScCLocale::toDoubleC(dc.attribute("StrikeThruPos"), -1.0);
			if (stp != -1)
				appPrefs.typoPrefs.valueStrikeThruPos = qRound(ulp * 10);
			else
				appPrefs.typoPrefs.valueStrikeThruPos = -1;
			double stw = ScCLocale::toDoubleC(dc.attribute("StrikeThruWidth"), -1.0);
			if (stw != -1)
				appPrefs.typoPrefs.valueStrikeThruWidth = qRound(stw * 10);
			else
				appPrefs.typoPrefs.valueStrikeThruWidth = -1;
		}
		if (dc.tagName()=="TOOLS")
		{
			appPrefs.itemToolPrefs.shapeLineColor = dc.attribute("PEN");
			appPrefs.itemToolPrefs.shapeFillColor = dc.attribute("BRUSH");
			appPrefs.itemToolPrefs.lineColor = dc.attribute("PENLINE");
			appPrefs.itemToolPrefs.textColor = dc.attribute("PENTEXT");
			appPrefs.itemToolPrefs.textStrokeColor = dc.attribute("StrokeText", appPrefs.itemToolPrefs.textColor);
			appPrefs.itemToolPrefs.textFillColor = dc.attribute("TextBackGround", CommonStrings::None);
			appPrefs.itemToolPrefs.textLineColor = dc.attribute("TextLineColor", CommonStrings::None);
			appPrefs.itemToolPrefs.textFillColorShade = dc.attribute("TextBackGroundShade", "100").toInt();
			appPrefs.itemToolPrefs.textLineColorShade = dc.attribute("TextLineShade", "100").toInt();
			appPrefs.itemToolPrefs.textShade = dc.attribute("TextPenShade", "100").toInt();
			appPrefs.itemToolPrefs.textStrokeShade = dc.attribute("TextStrokeShade", "100").toInt();
			appPrefs.itemToolPrefs.textColumns = dc.attribute("TEXTCOL", "1").toInt();
			appPrefs.itemToolPrefs.textColumnGap  = ScCLocale::toDoubleC(dc.attribute("TEXTGAP"), 0.0);
			appPrefs.itemToolPrefs.textTabFillChar = dc.attribute("TabFill", "");
			appPrefs.itemToolPrefs.textTabWidth   = ScCLocale::toDoubleC(dc.attribute("TabWidth"), 36.0);
			appPrefs.itemToolPrefs.shapeLineStyle = dc.attribute("STIL").toInt();
			appPrefs.itemToolPrefs.lineStyle = dc.attribute("STILLINE").toInt();
			appPrefs.itemToolPrefs.shapeLineWidth     = ScCLocale::toDoubleC(dc.attribute("WIDTH"), 0.0);
			appPrefs.itemToolPrefs.lineWidth = ScCLocale::toDoubleC(dc.attribute("WIDTHLINE"), 1.0);
			appPrefs.itemToolPrefs.shapeLineColorShade = dc.attribute("PENSHADE").toInt();
			appPrefs.itemToolPrefs.lineColorShade = dc.attribute("LINESHADE").toInt();
			appPrefs.itemToolPrefs.shapeFillColorShade  = dc.attribute("BRUSHSHADE").toInt();
			appPrefs.opToolPrefs.magMin  = dc.attribute("MAGMIN", "1").toInt();
			appPrefs.opToolPrefs.magMax  = dc.attribute("MAGMAX", "3200").toInt();
			appPrefs.opToolPrefs.magStep = dc.attribute("MAGSTEP", "200").toInt();
			//CB Reset prefs zoom step value to 200% instead of old values.
			if (appPrefs.opToolPrefs.magStep <= 100)
				appPrefs.opToolPrefs.magStep = 200;
			appPrefs.itemToolPrefs.imageFillColor = dc.attribute("CPICT", CommonStrings::None);
			appPrefs.itemToolPrefs.imageFillColorShade = dc.attribute("PICTSHADE", "100").toInt();
			appPrefs.itemToolPrefs.imageScaleX = ScCLocale::toDoubleC(dc.attribute("PICTSCX"), 1.0);
			appPrefs.itemToolPrefs.imageScaleY = ScCLocale::toDoubleC(dc.attribute("PICTSCY"), 1.0);
			appPrefs.itemToolPrefs.imageScaleType = static_cast<bool>(dc.attribute("PSCALE", "1").toInt());
			appPrefs.itemToolPrefs.imageAspectRatio = static_cast<bool>(dc.attribute("PASPECT", "0").toInt());
			appPrefs.itemToolPrefs.imageUseEmbeddedPath = static_cast<bool>(dc.attribute("EmbeddedPath", "0").toInt());
			appPrefs.itemToolPrefs.imageLowResType = dc.attribute("HalfRes", "1").toInt();
			appPrefs.itemToolPrefs.polyCorners = dc.attribute("POLYC", "4").toInt();
			appPrefs.itemToolPrefs.polyFactor = ScCLocale::toDoubleC(dc.attribute("POLYF"), 0.5);
			appPrefs.itemToolPrefs.polyRotation = ScCLocale::toDoubleC(dc.attribute("POLYR"), 0.0);
			appPrefs.itemToolPrefs.polyCurvature = ScCLocale::toDoubleC(dc.attribute("POLYCUR"), 0.0);
			appPrefs.itemToolPrefs.polyFactorGuiVal = dc.attribute("POLYFD", "0").toInt();
			appPrefs.itemToolPrefs.polyUseFactor  = static_cast<bool>(dc.attribute("POLYS", "0").toInt());
			appPrefs.itemToolPrefs.lineStartArrow = dc.attribute("StartArrow", "0").toInt();
			appPrefs.itemToolPrefs.lineEndArrow   = dc.attribute("EndArrow", "0").toInt();
			appPrefs.opToolPrefs.dispX = ScCLocale::toDoubleC(dc.attribute("dispX"), 10.0);
			appPrefs.opToolPrefs.dispY = ScCLocale::toDoubleC(dc.attribute("dispY"), 10.0);
			appPrefs.opToolPrefs.constrain = ScCLocale::toDoubleC(dc.attribute("constrain"), 15.0);
		}
		if (dc.tagName()=="MAINWINDOW")
		{
			appPrefs.uiPrefs.mainWinSettings.xPosition = dc.attribute("XPOS", "0").toInt();
			appPrefs.uiPrefs.mainWinSettings.yPosition = dc.attribute("YPOS", "0").toInt();
			appPrefs.uiPrefs.mainWinSettings.width = dc.attribute("WIDTH", "640").toInt();
			appPrefs.uiPrefs.mainWinSettings.height = dc.attribute("HEIGHT", "480").toInt();
			appPrefs.uiPrefs.mainWinSettings.maximized = static_cast<bool>(dc.attribute("MAXIMIZED", "0").toInt());
			QDesktopWidget *d = QApplication::desktop();
			QSize gStrut = QApplication::globalStrut();
			int minX = 0;
			int minY = 0;
#ifdef Q_OS_MAC
			// on Mac you're dead if the titlebar is not on screen
			minY = 22;
#endif
			if (appPrefs.uiPrefs.mainWinSettings.xPosition < minX )
				appPrefs.uiPrefs.mainWinSettings.xPosition = minX;
			if (appPrefs.uiPrefs.mainWinSettings.yPosition <  minY)
				appPrefs.uiPrefs.mainWinSettings.yPosition = minY;
			int minWidth = 5*gStrut.width();
			int minHeight = 5*gStrut.width();
			int maxWidth = d->width();
			int maxHeight = d->height();
			if (appPrefs.uiPrefs.mainWinSettings.width > maxWidth)
				appPrefs.uiPrefs.mainWinSettings.width = maxWidth;
			if (appPrefs.uiPrefs.mainWinSettings.width < minWidth)
				appPrefs.uiPrefs.mainWinSettings.width = minWidth;
			if (appPrefs.uiPrefs.mainWinSettings.height > maxHeight)
				appPrefs.uiPrefs.mainWinSettings.height = maxHeight;
			if (appPrefs.uiPrefs.mainWinSettings.height < minHeight)
				appPrefs.uiPrefs.mainWinSettings.height = minHeight;
			int maxX = d->width() - minWidth;
			int maxY = d->height() - minHeight;
			if (appPrefs.uiPrefs.mainWinSettings.xPosition >= maxX)
				appPrefs.uiPrefs.mainWinSettings.xPosition = maxX;
			if (appPrefs.uiPrefs.mainWinSettings.yPosition >= maxY)
				appPrefs.uiPrefs.mainWinSettings.yPosition = maxY;
		}
		if (dc.tagName()=="PAGEPALETTE")
		{
			appPrefs.uiPrefs.SepalT = static_cast<bool>(dc.attribute("THUMBS").toInt());
			appPrefs.uiPrefs.SepalN = static_cast<bool>(dc.attribute("NAMES").toInt());
		}
		if (dc.tagName()=="SCRAPBOOK")
		{
			appPrefs.scrapbookPrefs.doCopyToScrapbook = static_cast<bool>(dc.attribute("CopyToScrapbook", "1").toInt());
			appPrefs.scrapbookPrefs.persistentScrapbook = static_cast<bool>(dc.attribute("persistentScrapbook", "0").toInt());
			appPrefs.scrapbookPrefs.writePreviews = static_cast<bool>(dc.attribute("writePreviews", "1").toInt());
			appPrefs.scrapbookPrefs.numScrapbookCopies = dc.attribute("numScrapbookCopies", "10").toInt();
			QDomNode scrp = dc.firstChild();
			while(!scrp.isNull())
			{
				QDomElement scrpElem = scrp.toElement();
				if (scrpElem.tagName()=="RECENT")
				{
					QString nam = scrpElem.attribute("NAME");
					QFileInfo fd(nam);
					if (fd.exists())
						appPrefs.scrapbookPrefs.RecentScrapbooks.append(nam);
				}
				scrp = scrp.nextSibling();
			}
		}
		if (dc.tagName() == "DOKUMENT")
		{
			appPrefs.docSetupPrefs.pageSize = dc.attribute("PAGESIZE","A4");
			appPrefs.docSetupPrefs.pageOrientation = dc.attribute("AUSRICHTUNG", "0").toInt();
			appPrefs.docSetupPrefs.pageWidth   = ScCLocale::toDoubleC(dc.attribute("BREITE"), 595.0);
			appPrefs.docSetupPrefs.pageHeight  = ScCLocale::toDoubleC(dc.attribute("HOEHE"), 842.0);
			appPrefs.docSetupPrefs.margins.Top = ScCLocale::toDoubleC(dc.attribute("RANDO"), 9.0);
			appPrefs.docSetupPrefs.margins.Bottom = ScCLocale::toDoubleC(dc.attribute("RANDU"), 40.0);
			appPrefs.docSetupPrefs.margins.Left   = ScCLocale::toDoubleC(dc.attribute("RANDL"), 9.0);
			appPrefs.docSetupPrefs.margins.Right  = ScCLocale::toDoubleC(dc.attribute("RANDR"), 9.0);
			appPrefs.docSetupPrefs.marginPreset   = dc.attribute("PRESET", "0").toInt();
			appPrefs.docSetupPrefs.pagePositioning    = dc.attribute("DOPPEL", "0").toInt();
			appPrefs.docSetupPrefs.AutoSave      = static_cast<bool>(dc.attribute("AutoSave", "0").toInt());
			appPrefs.docSetupPrefs.AutoSaveTime  = dc.attribute("AutoSaveTime", "600000").toInt();
			appPrefs.docSetupPrefs.saveCompressed = static_cast<bool>(dc.attribute("SaveCompressed", "0").toInt());
			appPrefs.docSetupPrefs.bleeds.Top    = ScCLocale::toDoubleC(dc.attribute("BleedTop"), 0.0);
			appPrefs.docSetupPrefs.bleeds.Left   = ScCLocale::toDoubleC(dc.attribute("BleedLeft"), 0.0);
			appPrefs.docSetupPrefs.bleeds.Right  = ScCLocale::toDoubleC(dc.attribute("BleedRight"), 0.0);
			appPrefs.docSetupPrefs.bleeds.Bottom = ScCLocale::toDoubleC(dc.attribute("BleedBottom"), 0.0);
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
						if ((appPrefs.pageSets.count() == appPrefs.docSetupPrefs.pagePositioning) && ((appPrefs.displayPrefs.pageGapHorizontal < 0) && (appPrefs.displayPrefs.pageGapVertical < 0)))
						{
							appPrefs.displayPrefs.pageGapHorizontal = ScCLocale::toDoubleC(PgsAttr.attribute("GapHorizontal"), 0.0);
							appPrefs.displayPrefs.pageGapVertical   = ScCLocale::toDoubleC(PgsAttr.attribute("GapBelow"), 40.0);
						}
					}
					PGS = PGS.nextSibling();
				}
			}
		}
		if (dc.tagName()=="CMS")
		{
			appPrefs.colorPrefs.DCMSset.SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
			appPrefs.colorPrefs.DCMSset.SoftProofFullOn = static_cast<bool>(dc.attribute("DPSFo", "0").toInt());
			appPrefs.colorPrefs.DCMSset.CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
			appPrefs.colorPrefs.DCMSset.GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
			appPrefs.colorPrefs.DCMSset.BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
			appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile = dc.attribute("DPMo","");
			appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile = dc.attribute("DPPr","");
			appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile = dc.attribute("DPIn","");
			appPrefs.colorPrefs.DCMSset.DefaultImageCMYKProfile = dc.attribute("DPInCMYK","");
			appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile = dc.attribute("DPIn2","");
			appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile = dc.attribute("DPIn3","");
			appPrefs.colorPrefs.DCMSset.DefaultIntentColors = (eRenderIntent) dc.attribute("DISc", "1").toInt();
			appPrefs.colorPrefs.DCMSset.DefaultIntentImages = (eRenderIntent) dc.attribute("DIIm", "0").toInt();
		}
		if (!importingFrom12 && dc.tagName()=="SHORTCUT")
		{
//			if (appPrefs.keyShortcutPrefs.KeyActions.contains(dc.attribute("ACTION")))
//			{
				appPrefs.keyShortcutPrefs.KeyActions[dc.attribute("ACTION")].actionName = dc.attribute("ACTION");
				QKeySequence newKeySequence = QKeySequence(dc.attribute("SEQUENCE"));
				appPrefs.keyShortcutPrefs.KeyActions[dc.attribute("ACTION")].keySequence = newKeySequence;
//			}
		}
		if (dc.tagName()=="RECENT")
			appPrefs.uiPrefs.RecentDocs.append(dc.attribute("NAME"));
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
			struct CheckerPrefs checkerSettings;
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
			checkerSettings.checkNotCMYKOrSpot = static_cast<bool>(dc.attribute("checkNotCMYKOrSpot", "0").toInt());
			checkerSettings.checkDeviceColorsAndOutputIntend = static_cast<bool>(dc.attribute("checkDeviceColorsAndOutputIntend", "0").toInt());
			checkerSettings.checkFontNotEmbedded = static_cast<bool>(dc.attribute("checkFontNotEmbedded", "0").toInt());
			checkerSettings.checkFontIsOpenType = static_cast<bool>(dc.attribute("checkFontIsOpenType", "0").toInt());
			appPrefs.checkerPrefsList[name] = checkerSettings;
		}
		if (dc.tagName()=="PRINTER")
		{
			appPrefs.printerPrefs.PrinterName = dc.attribute("NAME");
			appPrefs.printerPrefs.PrinterFile = dc.attribute("FILE");
			appPrefs.printerPrefs.PrinterCommand = dc.attribute("COMMAND");
			appPrefs.printerPrefs.ClipMargin = static_cast<bool>(dc.attribute("CLIPMARGIN", "1").toInt());
			appPrefs.printerPrefs.GCRMode = static_cast<bool>(dc.attribute("GMODE", "1").toInt());
		}
		if (dc.tagName()=="PRINTPREVIEW")
		{
			appPrefs.printPreviewPrefs.PrPr_Mode = static_cast<bool>(dc.attribute("Mode", "0").toInt());
			//appPrefs.Gcr_Mode = static_cast<bool>(dc.attribute("GcrMode", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_AntiAliasing = static_cast<bool>(dc.attribute("AntiAliasing", "0").toInt());
			appPrefs.printPreviewPrefs.PrPr_Transparency = static_cast<bool>(dc.attribute("Transparency", "0").toInt());
			appPrefs.printPreviewPrefs.PrPr_C = static_cast<bool>(dc.attribute("Cyan", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_M = static_cast<bool>(dc.attribute("Magenta", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_Y = static_cast<bool>(dc.attribute("Yellow", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_K = static_cast<bool>(dc.attribute("Black", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_InkCoverage = static_cast<bool>(dc.attribute("InkCoverage", "0").toInt());
			appPrefs.printPreviewPrefs.PrPr_InkThreshold = dc.attribute("InkThreshold", "250").toInt();
		}
		if (dc.tagName()=="EXTERNAL")
		{
			bool gsa1 = testGSAvailability(dc.attribute("GS", "gs"));
			bool gsa2 = testGSAvailability(ghostscriptExecutable());
			if( (gsa1 == true) || (gsa2 == false) )
				setGhostscriptExecutable(dc.attribute("GS", "gs"));
			appPrefs.extToolPrefs.gs_AntiAliasText = static_cast<bool>(dc.attribute("AlphaText", "0").toInt());
			appPrefs.extToolPrefs.gs_AntiAliasGraphics = static_cast<bool>(dc.attribute("AlphaGraphics", "0").toInt());
			appPrefs.extToolPrefs.gs_Resolution = dc.attribute("Resolution", "72").toInt();
			appPrefs.extToolPrefs.latexResolution = dc.attribute("LatexResolution", "72").toInt();
			appPrefs.extToolPrefs.latexForceDpi = static_cast<bool>(dc.attribute("LatexForceDpi", "1").toInt());
			appPrefs.extToolPrefs.latexStartWithEmptyFrames = static_cast<bool>(dc.attribute("LatexStartWithEmptyFrames", "0").toInt());
			setImageEditorExecutable(dc.attribute("GIMP", "gimp"));
			setExtBrowserExecutable(dc.attribute("WebBrowser", ""));
			setUniconvExecutable(dc.attribute("Uniconv", "uniconv"));
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
					appPrefs.extToolPrefs.latexCommands[confFile] = command;
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
				appPrefs.hyphPrefs.Language = dc.attribute("LANG");
			appPrefs.hyphPrefs.MinWordLen = dc.attribute("WORDLEN", "3").toInt();
			appPrefs.hyphPrefs.HyCount = dc.attribute("HYCOUNT", "2").toInt();
			appPrefs.hyphPrefs.Automatic = static_cast<bool>(dc.attribute("MODE", "1").toInt());
			appPrefs.hyphPrefs.AutoCheck = static_cast<bool>(dc.attribute("INMODE", "1").toInt());
			QDomNode hyelm = dc.firstChild();
			while(!hyelm.isNull())
			{
				QDomElement hyElem = hyelm.toElement();
				if (hyElem.tagName()=="EXCEPTION")
				{
					QString word = hyElem.attribute("WORD");
					QString hyph = hyElem.attribute("HYPHENATED");
					appPrefs.hyphPrefs.specialWords.insert(word, hyph);
				}
				else if (hyElem.tagName()=="IGNORE")
				{
					QString word = hyElem.attribute("WORD");
					appPrefs.hyphPrefs.ignoredWords.insert(word);
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
				if (!appPrefs.fontPrefs.AvailFonts.contains(tmpf) || !appPrefs.fontPrefs.AvailFonts[tmpf].usable())
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
					appPrefs.itemToolPrefs.textFont = newFont;
				appPrefs.itemToolPrefs.textSize = qRound( ScCLocale::toDoubleC(dc.attribute("SIZE"), 12.0) * 10.0 );
				appPrefs.fontPrefs.askBeforeSubstitute = static_cast<bool>(dc.attribute("AutomaticSubst", "1").toInt());
			}
		}
		if (dc.tagName()=="FONT")
		{
			QString tmpf(dc.attribute("NAME"));
			if (!tmpf.isEmpty() && appPrefs.fontPrefs.AvailFonts.contains(tmpf))
			{
				ScFace& face(appPrefs.fontPrefs.AvailFonts[tmpf]);
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
			appPrefs.colorPrefs.DColors[dc.attribute("NAME")] = lf;
		}
		if (dc.tagName() == "Gradient")
		{
			VGradient gra = VGradient(VGradient::linear);
			gra.clearStops();
			QDomNode grad = dc.firstChild();
			while(!grad.isNull())
			{
				QDomElement stop = grad.toElement();
				QString name = stop.attribute("NAME");
				double ramp  = ScCLocale::toDoubleC(stop.attribute("RAMP"), 0.0);
				int shade    = stop.attribute("SHADE", "100").toInt();
				double opa   = ScCLocale::toDoubleC(stop.attribute("TRANS"), 1.0);
				QColor color;
				if (name == CommonStrings::None)
					color = QColor(255, 255, 255, 0);
				else
				{
					const ScColor& col = appPrefs.colorPrefs.DColors[name];
					color = ScColorEngine::getShadeColorProof(col, NULL, shade);
				}
				gra.addStop(color, ramp, 0.5, opa, name, shade);
				grad = grad.nextSibling();
			}
			appPrefs.defaultGradients.insert(dc.attribute("Name"), gra);
		}
		if (dc.tagName()=="Substitute")
		  appPrefs.fontPrefs.GFontSub[dc.attribute("Name")] = dc.attribute("Replace");
		if (dc.tagName()=="COLORSET")
			appPrefs.colorPrefs.CustomColorSets.append(dc.attribute("NAME"));
		if (dc.tagName()=="DCOLORSET")
			appPrefs.colorPrefs.DColorSet = dc.attribute("NAME");
		if(dc.tagName()=="PDF")
		{
			appPrefs.pdfPrefs.Articles = static_cast<bool>(dc.attribute("Articles").toInt());
			appPrefs.pdfPrefs.Thumbnails = static_cast<bool>(dc.attribute("Thumbnails").toInt());
			appPrefs.pdfPrefs.Compress = static_cast<bool>(dc.attribute("Compress").toInt());
			appPrefs.pdfPrefs.CompressMethod = (PDFOptions::PDFCompression) dc.attribute("CMethod", "0").toInt();
			appPrefs.pdfPrefs.Quality = dc.attribute("Quality", "0").toInt();
			appPrefs.pdfPrefs.embedPDF  = dc.attribute("EmbedPDF", "0").toInt();
			appPrefs.pdfPrefs.RecalcPic = static_cast<bool>(dc.attribute("RecalcPic").toInt());
			appPrefs.pdfPrefs.Bookmarks = static_cast<bool>(dc.attribute("Bookmarks").toInt());
			appPrefs.pdfPrefs.MirrorH = static_cast<bool>(dc.attribute("MirrorH").toInt());
			appPrefs.pdfPrefs.MirrorV = static_cast<bool>(dc.attribute("MirrorV").toInt());
			appPrefs.pdfPrefs.doClip = static_cast<bool>(dc.attribute("Clip").toInt());
			appPrefs.pdfPrefs.RotateDeg = dc.attribute("RotateDeg", "0").toInt();
			appPrefs.pdfPrefs.PresentMode = static_cast<bool>(dc.attribute("PresentMode").toInt());
			appPrefs.pdfPrefs.PicRes = dc.attribute("PicRes").toInt();
			appPrefs.pdfPrefs.Version = (PDFOptions::PDFVersion)dc.attribute("Version").toInt();
			appPrefs.pdfPrefs.Resolution = dc.attribute("Resolution").toInt();
			appPrefs.pdfPrefs.Binding = dc.attribute("Binding").toInt();
			appPrefs.pdfPrefs.fileName = "";
			appPrefs.pdfPrefs.isGrayscale = static_cast<bool>(dc.attribute("Grayscale", "0").toInt());
			appPrefs.pdfPrefs.UseRGB = static_cast<bool>(dc.attribute("RGBMode", "0").toInt());
			appPrefs.pdfPrefs.UseProfiles = static_cast<bool>(dc.attribute("UseProfiles", "0").toInt());
			appPrefs.pdfPrefs.UseProfiles2 = static_cast<bool>(dc.attribute("UseProfiles2", "0").toInt());
			appPrefs.pdfPrefs.Intent = dc.attribute("Intent", "1").toInt();
			appPrefs.pdfPrefs.Intent2 = dc.attribute("Intent2", "1").toInt();
			appPrefs.pdfPrefs.SolidProf = dc.attribute("SolidP", "");
			appPrefs.pdfPrefs.ImageProf = dc.attribute("ImageP", "");
			appPrefs.pdfPrefs.PrintProf = dc.attribute("PrintP", "");
			appPrefs.pdfPrefs.Info = dc.attribute("InfoString", "");
			appPrefs.pdfPrefs.bleeds.Top    = ScCLocale::toDoubleC(dc.attribute("BTop"), 0.0);
			appPrefs.pdfPrefs.bleeds.Left   = ScCLocale::toDoubleC(dc.attribute("BLeft"), 0.0);
			appPrefs.pdfPrefs.bleeds.Right  = ScCLocale::toDoubleC(dc.attribute("BRight"), 0.0);
			appPrefs.pdfPrefs.bleeds.Bottom = ScCLocale::toDoubleC(dc.attribute("BBottom"), 0.0);
			appPrefs.pdfPrefs.useDocBleeds = static_cast<bool>(dc.attribute("useDocBleeds", "1").toInt());
			appPrefs.pdfPrefs.cropMarks = static_cast<bool>(dc.attribute("cropMarks", "0").toInt());
			appPrefs.pdfPrefs.bleedMarks = static_cast<bool>(dc.attribute("bleedMarks", "0").toInt());
			appPrefs.pdfPrefs.registrationMarks = static_cast<bool>(dc.attribute("registrationMarks", "0").toInt());
			appPrefs.pdfPrefs.colorMarks = static_cast<bool>(dc.attribute("colorMarks", "0").toInt());
			appPrefs.pdfPrefs.docInfoMarks = static_cast<bool>(dc.attribute("docInfoMarks", "0").toInt());
			appPrefs.pdfPrefs.markOffset = ScCLocale::toDoubleC(dc.attribute("markOffset"), 0.0);
			appPrefs.pdfPrefs.EmbeddedI  = static_cast<bool>(dc.attribute("ImagePr", "0").toInt());
			appPrefs.pdfPrefs.PassOwner  = dc.attribute("PassOwner", "");
			appPrefs.pdfPrefs.PassUser   = dc.attribute("PassUser", "");
			appPrefs.pdfPrefs.Permissions = dc.attribute("Permissions", "-4").toInt();
			appPrefs.pdfPrefs.Encrypt = static_cast<bool>(dc.attribute("Encrypt", "0").toInt());
			appPrefs.pdfPrefs.useLayers = static_cast<bool>(dc.attribute("UseLayers", "0").toInt());
			appPrefs.pdfPrefs.UseLPI = static_cast<bool>(dc.attribute("UseLpi", "0").toInt());
			appPrefs.pdfPrefs.UseSpotColors = static_cast<bool>(dc.attribute("UseSpotColors", "1").toInt());
			appPrefs.pdfPrefs.doMultiFile = static_cast<bool>(dc.attribute("doMultiFile", "0").toInt());
			appPrefs.pdfPrefs.displayBookmarks = static_cast<bool>(dc.attribute("displayBookmarks", "0").toInt());
			appPrefs.pdfPrefs.displayFullscreen = static_cast<bool>(dc.attribute("displayFullscreen", "0").toInt());
			appPrefs.pdfPrefs.displayLayers = static_cast<bool>(dc.attribute("displayLayers", "0").toInt());
			appPrefs.pdfPrefs.displayThumbs = static_cast<bool>(dc.attribute("displayThumbs", "0").toInt());
			appPrefs.pdfPrefs.PageLayout = dc.attribute("PageLayout", "0").toInt();
			appPrefs.pdfPrefs.openAction = dc.attribute("openAction", "");
			QDomNode PFO = DOC.firstChild();
			appPrefs.pdfPrefs.LPISettings.clear();
			while(!PFO.isNull())
			{
				QDomElement pdfF = PFO.toElement();
				if(pdfF.tagName() == "LPI")
				{
					struct LPIData lpo;
					lpo.Angle = pdfF.attribute("Angle").toInt();
					lpo.Frequency = pdfF.attribute("Frequency").toInt();
					lpo.SpotFunc = pdfF.attribute("SpotFunction").toInt();
					appPrefs.pdfPrefs.LPISettings[pdfF.attribute("Color")] = lpo;
				}
				PFO = PFO.nextSibling();
			}
		}
		if(dc.tagName()=="DefaultItemAttributes")
		{
			QDomNode DIA = DOC.firstChild();
			appPrefs.itemAttrPrefs.defaultItemAttributes.clear();
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
					appPrefs.itemAttrPrefs.defaultItemAttributes.append(objattr);
				}
				DIA = DIA.nextSibling();
			}
		}
		if(dc.tagName()=="TablesOfContents")
		{
			QDomNode TOC = DOC.firstChild();
			appPrefs.tocPrefs.defaultToCSetups.clear();
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
					appPrefs.tocPrefs.defaultToCSetups.append(tocsetup);
				}
				TOC = TOC.nextSibling();
			}
		}
		// lorem ispum
		if (dc.tagName() == "LoremIpsum")
		{
			appPrefs.miscPrefs.useStandardLI = static_cast<bool>(dc.attribute("useStandardLI", "0").toInt());
			appPrefs.miscPrefs.paragraphsLI = dc.attribute("paragraphsLI", "10").toInt();
		}
		// cache manager
		if (dc.tagName() == "ImageCache")
		{
			appPrefs.imageCachePrefs.cacheEnabled = static_cast<bool>(dc.attribute("cacheEnabled", "0").toInt());
			appPrefs.imageCachePrefs.maxCacheSizeMiB = dc.attribute("maxCacheSizeMiB", "1000").toInt();
			appPrefs.imageCachePrefs.maxCacheEntries = dc.attribute("maxCacheEntries", "1000").toInt();
			appPrefs.imageCachePrefs.compressionLevel = dc.attribute("compressionLevel", "1").toInt();
		}
		DOC=DOC.nextSibling();
	}
	// Some sanity checks
	appPrefs.colorPrefs.DColors.ensureBlackAndWhite();
	if ((appPrefs.docSetupPrefs.pagePositioning  < 0) || (appPrefs.docSetupPrefs.pagePositioning >= appPrefs.pageSets.count()))
		appPrefs.docSetupPrefs.pagePositioning = 0;
	if ((appPrefs.docSetupPrefs.docUnitIndex < UNITMIN) || (appPrefs.docSetupPrefs.docUnitIndex > UNITMAX))
		appPrefs.docSetupPrefs.docUnitIndex = int(SC_POINTS);
	// Configure GUI
	appPrefs.ui_SystemTheme = qApp->style()->objectName();
	if (appPrefs.uiPrefs.style.length() > 0)
	{
		qApp->setStyle(QStyleFactory::create(appPrefs.uiPrefs.style));
		// Plain wrong, a style may set a palette different from the standard palette
		// Eg : Windows XP and Windows Vista styles
		// qApp->setPalette(qApp->style()->standardPalette());
	}
	QFont apf = qApp->font();
	apf.setPointSize(appPrefs.uiPrefs.applicationFontSize);
	qApp->setFont(apf);
	return true;
}

void PrefsManager::initDefaultCheckerPrefs(CheckerPrefsList* cp)
{
	if (cp!=NULL)
	{
		struct CheckerPrefs checkerSettings;
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
		checkerSettings.checkNotCMYKOrSpot = false;
		checkerSettings.checkDeviceColorsAndOutputIntend = false;
		checkerSettings.checkFontNotEmbedded = false;
		checkerSettings.checkFontIsOpenType = false;
		//TODO Stop translating these into settings!!!!!!!!!
		cp->insert( CommonStrings::PostScript, checkerSettings);
		checkerSettings.checkFontNotEmbedded = true;
		checkerSettings.checkFontIsOpenType = true;
		cp->insert( CommonStrings::PDF_1_3   , checkerSettings);
		checkerSettings.checkTransparency = false;
		cp->insert( CommonStrings::PDF_1_4   , checkerSettings);
		cp->insert( CommonStrings::PDF_1_5   , checkerSettings);
		checkerSettings.checkTransparency = true;
		checkerSettings.checkAnnotations = true;
		checkerSettings.minResolution = 144.0;
		checkerSettings.checkDeviceColorsAndOutputIntend = true;
		cp->insert( CommonStrings::PDF_X3    , checkerSettings);
		checkerSettings.checkNotCMYKOrSpot = true;
		checkerSettings.checkDeviceColorsAndOutputIntend = false;
		cp->insert( CommonStrings::PDF_X1a    , checkerSettings);
		checkerSettings.checkNotCMYKOrSpot = false;
		checkerSettings.checkDeviceColorsAndOutputIntend = true;
		checkerSettings.checkTransparency = false;
		checkerSettings.checkFontIsOpenType = false;
		cp->insert( CommonStrings::PDF_X4    , checkerSettings);
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
	return appPrefs.extToolPrefs.gs_Resolution;
}
