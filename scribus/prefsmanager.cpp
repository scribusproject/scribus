/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	copyright			: (C) 2005 by Craig Bradney
	email				: cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*																		 *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or	 *
*   (at your option) any later version.								   *
*																		 *
***************************************************************************/

#include <QColor>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QScreen>
#include <QString>
#include <QStringList>
#include <QStyleFactory>
#include <QTransform>
#include <QWindow>

#include "prefsmanager.h"

#include "actionmanager.h"
#include "colorsetmanager.h"
#include "commonstrings.h"
#include "filewatcher.h"
#include "latexhelpers.h"
#include "langmgr.h"
#include "localemgr.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "pdfoptions.h"
#include "prefsfile.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scdomelement.h"
#include "scfonts.h"
#include "scpaths.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "sctextstream.h"
#include "units.h"
#include "util.h"
#include "util_color.h"
#include "util_file.h"
#include "util_ghostscript.h"

#include "ui/missing.h"
#include "ui/modetoolbar.h"
#include "ui/prefs_keyboardshortcuts.h"

extern bool emergencyActivated;

PrefsManager::PrefsManager(QObject *parent) : QObject(parent)
{
}

PrefsManager::~PrefsManager()
{
	appPrefs.fontPrefs.AvailFonts.clear();
	delete prefsFile;
}

bool PrefsManager::firstTimeIgnoreOldPrefs() const
{
	return m_firstTimeIgnoreOldPrefs;
}

PrefsManager& PrefsManager::instance()
{
	static PrefsManager m_instance;
	return m_instance;
}

ApplicationPrefs* PrefsManager::applicationPrefs()
{
	return &appPrefs;
}

void PrefsManager::setNewPrefs(const ApplicationPrefs& newPrefs)
{
	appPrefs = newPrefs;
}

PrefsFile* PrefsManager::applicationPrefsFile()
{
	return prefsFile;
}

void PrefsManager::setup()
{
	setupPreferencesLocation();

	copyOldAppConfigAndData();
	prefsFile = new PrefsFile( m_prefsLocation + "prefs150.xml" );

	//<<CB TODO Reset keyboard shortcuts of all 1.3 users as too many
	//	 have conflicts if they don't nuke their settings.
	// - Remove for 1.3.0 release: importingFrom12 = true;
	//>>CB
}

void PrefsManager::initDefaults()
{
	/** Default font and size **/
	QStringList defaultFonts;
	defaultFonts << "Arial Regular";
	defaultFonts << "Times New Roman Regular";
	defaultFonts << "Helvetica Regular";
	defaultFonts << "Helvetica Neue Regular";
	defaultFonts << "DejaVu Sans Book";
	defaultFonts << "DejaVu Sans Condensed";
	defaultFonts << "Open Sans Regular";
	defaultFonts << "Liberation Sans Regular";

	bool goodFont = false;
	const SCFonts& availableFonts = appPrefs.fontPrefs.AvailFonts;
	for (int i = 0; i < defaultFonts.count(); ++i)
	{
		const QString& defCandidate = defaultFonts.at(i);
		if (availableFonts.contains(defCandidate))
		{
			appPrefs.itemToolPrefs.textFont = defCandidate;
			goodFont = true;
			break;
		}
	}
	if (!goodFont)
		appPrefs.itemToolPrefs.textFont = availableFonts.firstKey();
	appPrefs.itemToolPrefs.textSize = 120;

	/** Default colours **/
	appPrefs.defaultPatterns.clear();
	appPrefs.defaultGradients.clear();
	appPrefs.colorPrefs.DColors.clear();

	ColorSetManager csm;
	csm.initialiseDefaultPrefs(appPrefs);

	appPrefs.uiPrefs.mouseMoveTimeout = 150;
	appPrefs.uiPrefs.wheelJump = 40;
	appPrefs.uiPrefs.style = "";
	/** Set Default window position and size to sane default values which should work on every screen */
	const QScreen* s = QGuiApplication::primaryScreen();
	appPrefs.uiPrefs.mainWinSettings.width = s->availableGeometry().width() * (4.0 / 5.0);
	appPrefs.uiPrefs.mainWinSettings.height = s->availableGeometry().height() * (4.0 / 5.0);
	appPrefs.uiPrefs.mainWinSettings.xPosition = (s->availableGeometry().width() - appPrefs.uiPrefs.mainWinSettings.width) / 2;
	appPrefs.uiPrefs.mainWinSettings.yPosition = (s->availableGeometry().height() - appPrefs.uiPrefs.mainWinSettings.height) / 2;
	appPrefs.uiPrefs.mainWinSettings.maximized = false;
	appPrefs.uiPrefs.mainWinState = QByteArray();
	appPrefs.uiPrefs.RecentDocs.clear();
	appPrefs.uiPrefs.recentDocCount = 5;
	appPrefs.uiPrefs.language = ScQApp->currGUILanguage();
	if (appPrefs.uiPrefs.language.isEmpty())
		appPrefs.uiPrefs.language = "en_GB";
	appPrefs.uiPrefs.userPreferredLocale = "System";
	appPrefs.uiPrefs.showStartupDialog = true;
	appPrefs.uiPrefs.showSplashOnStartup = true;
	appPrefs.uiPrefs.useSmallWidgets = false;
	appPrefs.uiPrefs.useTabs = false;
	appPrefs.uiPrefs.stickyTools = false;
	appPrefs.uiPrefs.grayscaleIcons = false;
	appPrefs.uiPrefs.iconSet = "1_5_0";
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
	appPrefs.guidesPrefs.renderStackOrder.clear();
	appPrefs.guidesPrefs.renderStackOrder << 2 << 0 << 4 << 1 << 3;
	appPrefs.guidesPrefs.gridType = 0;
	appPrefs.typoPrefs.valueSuperScript = 33;
	appPrefs.typoPrefs.scalingSuperScript = 66;
	appPrefs.typoPrefs.valueSubScript = 33;
	appPrefs.typoPrefs.scalingSubScript = 66;
	appPrefs.typoPrefs.valueSmallCaps = 75;
	appPrefs.typoPrefs.autoLineSpacing = 100;
	appPrefs.typoPrefs.valueUnderlinePos = -1;
	appPrefs.typoPrefs.valueUnderlineWidth = -1;
	appPrefs.typoPrefs.valueStrikeThruPos = -1;
	appPrefs.typoPrefs.valueStrikeThruWidth = -1;
	appPrefs.guidesPrefs.valueBaselineGrid = 14.4;
	appPrefs.guidesPrefs.offsetBaselineGrid = 0.0;
	appPrefs.displayPrefs.showToolTips = true;
	appPrefs.displayPrefs.showMouseCoordinates = true;
	//FIXME
	//Black here causes issues when a colour set is loaded without "Black" in it.
	//"Black" is created with wrong values. Eg SVG colour set
	appPrefs.itemToolPrefs.shapeLineColor = "Black";
	appPrefs.itemToolPrefs.shapeFillColor = CommonStrings::None;
	appPrefs.itemToolPrefs.shapeLineColorShade = 100;
	appPrefs.itemToolPrefs.shapeFillColorShade = 100;
	appPrefs.itemToolPrefs.shapeLineStyle = Qt::SolidLine;
	appPrefs.itemToolPrefs.shapeLineWidth = 1;
	appPrefs.itemToolPrefs.lineColor = "Black";
	appPrefs.itemToolPrefs.textColor = "Black";
	appPrefs.itemToolPrefs.textStrokeColor = "Black";
	appPrefs.itemToolPrefs.textFillColor = CommonStrings::None;
	appPrefs.itemToolPrefs.textLineColor = CommonStrings::None;
	appPrefs.itemToolPrefs.textFillColorShade = 100;
	appPrefs.itemToolPrefs.textLineColorShade = 100;
	appPrefs.itemToolPrefs.textShade = 100;
	appPrefs.itemToolPrefs.textStrokeShade = 100;
	appPrefs.itemToolPrefs.textTabFillChar.clear();
	appPrefs.itemToolPrefs.textTabWidth = 36.0;
	appPrefs.itemToolPrefs.firstLineOffset = FLOPFontAscent;
	// defaults for calligraphic pen
	appPrefs.itemToolPrefs.calligraphicPenFillColor = "Black";
	appPrefs.itemToolPrefs.calligraphicPenLineColor = "Black";
	appPrefs.itemToolPrefs.calligraphicPenFillColorShade = 100;
	appPrefs.itemToolPrefs.calligraphicPenLineColorShade = 100;
	appPrefs.itemToolPrefs.calligraphicPenLineWidth = 1.0;
	appPrefs.itemToolPrefs.calligraphicPenAngle = 0.0;
	appPrefs.itemToolPrefs.calligraphicPenWidth = 10.0;
	appPrefs.itemToolPrefs.calligraphicPenStyle = Qt::SolidLine;

	appPrefs.opToolPrefs.dispX = 10.0;
	appPrefs.opToolPrefs.dispY = 10.0;
	appPrefs.opToolPrefs.constrain = 15.0;
	appPrefs.displayPrefs.paperColor = QColor(Qt::white);
	appPrefs.displayPrefs.scratchColor = QApplication::palette().color(QPalette::Active, QPalette::Window);
	appPrefs.displayPrefs.showPageShadow = true;
	appPrefs.displayPrefs.showVerifierWarningsOnCanvas = true;
	appPrefs.displayPrefs.showAutosaveClockOnCanvas = false;
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
	appPrefs.opToolPrefs.magMax = 32000;
	appPrefs.opToolPrefs.magStep = 25;
	appPrefs.docSetupPrefs.docUnitIndex = unitIndexFromString(LocaleManager::instance().unitForLocale(ScQApp->currGUILanguage()));
	appPrefs.itemToolPrefs.polyCorners = 4;
	appPrefs.itemToolPrefs.polyFactor = 0.5;
	appPrefs.itemToolPrefs.polyUseFactor = false;
	appPrefs.itemToolPrefs.polyRotation = 0.0;
	appPrefs.itemToolPrefs.polyInnerRot = 0.0;
	appPrefs.itemToolPrefs.polyCurvature = 0.0;
	appPrefs.itemToolPrefs.polyOuterCurvature = 0.0;
	appPrefs.itemToolPrefs.arcStartAngle = 30.0;
	appPrefs.itemToolPrefs.arcSweepAngle = 300.0;
	appPrefs.itemToolPrefs.spiralStartAngle = 0.0;
	appPrefs.itemToolPrefs.spiralEndAngle = 1080.0;
	appPrefs.itemToolPrefs.spiralFactor = 1.2;
	// Image item prefs
	appPrefs.itemToolPrefs.imageFillColor = CommonStrings::None;
	appPrefs.itemToolPrefs.imageFillColorShade = 100;
	appPrefs.itemToolPrefs.imageStrokeColor = CommonStrings::None;
	appPrefs.itemToolPrefs.imageStrokeColorShade = 100;
	appPrefs.itemToolPrefs.imageScaleX = 1;
	appPrefs.itemToolPrefs.imageScaleY = 1;
	appPrefs.itemToolPrefs.imageScaleType = true;
	appPrefs.itemToolPrefs.imageAspectRatio = true;
	appPrefs.itemToolPrefs.imageLowResType = 1;
	appPrefs.itemToolPrefs.imageUseEmbeddedPath = false;
//	appPrefs.PSize = 40;
	appPrefs.printerPrefs.ClipMargin = false;
	appPrefs.printerPrefs.GCRMode = false;
	appPrefs.scrapbookPrefs.RecentScrapbooks.clear();
	appPrefs.scrapbookPrefs.doCopyToScrapbook = true;
	appPrefs.scrapbookPrefs.persistentScrapbook = false;
	appPrefs.scrapbookPrefs.writePreviews = true;
	appPrefs.scrapbookPrefs.numScrapbookCopies = 10;
	appPrefs.displayPrefs.marginColored = false;
	appPrefs.docSetupPrefs.language = LanguageManager::instance()->getShortAbbrevFromAbbrevDecomposition(ScQApp->currGUILanguage());
	if (appPrefs.docSetupPrefs.language.isEmpty())
		appPrefs.docSetupPrefs.language = "en_GB";
	appPrefs.docSetupPrefs.pageSize = LocaleManager::instance().pageSizeForLocale(ScQApp->currGUILanguage());
	appPrefs.docSetupPrefs.pageOrientation = 0;
	PageSize defaultPageSize(appPrefs.docSetupPrefs.pageSize);
	appPrefs.docSetupPrefs.pageWidth = defaultPageSize.width();
	appPrefs.docSetupPrefs.pageHeight = defaultPageSize.height();
	appPrefs.docSetupPrefs.margins.set(40, 40, 40, 40);
	appPrefs.docSetupPrefs.marginPreset = 0;
	appPrefs.docSetupPrefs.bleeds.set(0, 0, 0, 0);
	appPrefs.hyphPrefs.specialWords.clear();
	appPrefs.hyphPrefs.ignoredWords.clear();
	appPrefs.hyphPrefs.Automatic = true;
	appPrefs.hyphPrefs.AutoCheck = false;
	appPrefs.docSetupPrefs.AutoSave = true;
	appPrefs.docSetupPrefs.AutoSaveTime = 600000;
	appPrefs.docSetupPrefs.AutoSaveCount = 1;
	appPrefs.docSetupPrefs.AutoSaveKeep = false;
	appPrefs.docSetupPrefs.saveCompressed = false;
	appPrefs.docSetupPrefs.AutoSaveLocation = true;
	appPrefs.docSetupPrefs.AutoSaveDir = "";
	appPrefs.miscPrefs.saveEmergencyFile = true;
	qreal dpi = s->physicalDotsPerInchX();
	if ((dpi < 60) || (dpi > 400))
		dpi = 72;
	appPrefs.displayPrefs.displayScale = dpi / 72.0;

	appPrefs.pathPrefs.documents = ScPaths::userDocumentDir();
	appPrefs.pathPrefs.colorProfiles.clear();
	appPrefs.pathPrefs.scripts.clear();
	appPrefs.pathPrefs.documentTemplates.clear();

	appPrefs.printPreviewPrefs.PrPr_Mode = false;
	appPrefs.printPreviewPrefs.PrPr_AntiAliasing = true;
	appPrefs.printPreviewPrefs.PrPr_Transparency = false;
	appPrefs.printPreviewPrefs.PrPr_C = true;
	appPrefs.printPreviewPrefs.PrPr_M = true;
	appPrefs.printPreviewPrefs.PrPr_Y = true;
	appPrefs.printPreviewPrefs.PrPr_K = true;
	appPrefs.printPreviewPrefs.PrPr_InkCoverage = false;
	appPrefs.printPreviewPrefs.PrPr_InkThreshold = 250;

	appPrefs.pdfOutputPreviewPrefs.enableAntiAliasing = true;
	appPrefs.pdfOutputPreviewPrefs.showTransparency = false;
	appPrefs.pdfOutputPreviewPrefs.cmykPreviewMode = false ;
	appPrefs.pdfOutputPreviewPrefs.isCyanVisible = true;
	appPrefs.pdfOutputPreviewPrefs.isMagentaVisible = true;
	appPrefs.pdfOutputPreviewPrefs.isYellowVisible = true;
	appPrefs.pdfOutputPreviewPrefs.isBlackVisible = true;
	appPrefs.pdfOutputPreviewPrefs.displayInkCoverage = false;
	appPrefs.pdfOutputPreviewPrefs.inkCoverageThreshold = 250;

	appPrefs.psOutputPreviewPrefs.psLevel = 3;
	appPrefs.psOutputPreviewPrefs.enableAntiAliasing = true;
	appPrefs.psOutputPreviewPrefs.showTransparency = false;
	appPrefs.psOutputPreviewPrefs.cmykPreviewMode = false ;
	appPrefs.psOutputPreviewPrefs.isCyanVisible = true;
	appPrefs.psOutputPreviewPrefs.isMagentaVisible = true;
	appPrefs.psOutputPreviewPrefs.isYellowVisible = true;
	appPrefs.psOutputPreviewPrefs.isBlackVisible = true;
	appPrefs.psOutputPreviewPrefs.displayInkCoverage = false;
	appPrefs.psOutputPreviewPrefs.inkCoverageThreshold = 250;

	appPrefs.extToolPrefs.imageEditorExecutable = ScPaths::defaultImageEditorApp();
	appPrefs.extToolPrefs.extBrowserExecutable = "";
	appPrefs.extToolPrefs.uniconvExecutable = "uniconv";
	appPrefs.extToolPrefs.latexConfigs = LatexConfigCache::defaultConfigs();
	appPrefs.extToolPrefs.latexCommands = LatexConfigCache::defaultCommands();
	appPrefs.extToolPrefs.latexEditorExecutable = "";
	appPrefs.extToolPrefs.latexResolution = 72;
	appPrefs.extToolPrefs.latexForceDpi = true;
	appPrefs.extToolPrefs.latexStartWithEmptyFrames = false;
	appPrefs.extToolPrefs.gs_AntiAliasGraphics = true;
	appPrefs.extToolPrefs.gs_AntiAliasText = true;
	appPrefs.extToolPrefs.gs_exe = getGSDefaultExeName();
	appPrefs.extToolPrefs.gs_Resolution = 72;

	// Keep SE background color for now, we still load this value and save it in prefs
	// so as not to break prefs for user also using older versions of Scribus
	appPrefs.storyEditorPrefs.guiFontColorBackground = QColor();
	appPrefs.storyEditorPrefs.smartTextSelection = false;

	appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile.clear();
	appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile.clear();
	appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile.clear();
	appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile.clear();
	appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile.clear();
	appPrefs.colorPrefs.DCMSset.CMSinUse = false;
	appPrefs.colorPrefs.DCMSset.SoftProofOn = false;
	appPrefs.colorPrefs.DCMSset.SoftProofFullOn = false;
	appPrefs.colorPrefs.DCMSset.GamutCheck = false;
	appPrefs.colorPrefs.DCMSset.BlackPoint = true;
	appPrefs.colorPrefs.DCMSset.DefaultIntentColors = Intent_Relative_Colorimetric;
	appPrefs.colorPrefs.DCMSset.DefaultIntentImages = Intent_Perceptual;

	appPrefs.fontPrefs.GFontSub.clear();
	appPrefs.displayPrefs.scratch.setLeft(100);
	appPrefs.displayPrefs.scratch.setRight(100);
	appPrefs.displayPrefs.scratch.setTop(20);
	appPrefs.displayPrefs.scratch.setBottom(20);
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
	appPrefs.miscPrefs.saveEmergencyFile = true;
	// lorem ipsum defaults
	appPrefs.miscPrefs.useStandardLI = false;
	appPrefs.miscPrefs.paragraphsLI = 10;
	initDefaultCheckerPrefs(appPrefs.verifierPrefs.checkerPrefsList);
	appPrefs.verifierPrefs.curCheckProfile = CommonStrings::PDF_1_4;
	appPrefs.verifierPrefs.showPagesWithoutErrors = false;
	appPrefs.verifierPrefs.showNonPrintingLayerErrors = false;
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
	appPrefs.pdfPrefs.Version = PDFVersion::PDF_14;
	appPrefs.pdfPrefs.Resolution = 300;
	appPrefs.pdfPrefs.Binding = 0;
	appPrefs.pdfPrefs.FontEmbedding = PDFOptions::EmbedFonts;
	appPrefs.pdfPrefs.EmbedList.clear();
	appPrefs.pdfPrefs.SubsetList.clear();
	appPrefs.pdfPrefs.MirrorH = false;
	appPrefs.pdfPrefs.MirrorV = false;
	appPrefs.pdfPrefs.doClip = false;
	appPrefs.pdfPrefs.pageRangeSelection = 0;
	appPrefs.pdfPrefs.pageRangeString = "";
	appPrefs.pdfPrefs.RotateDeg = 0;
	appPrefs.pdfPrefs.PresentMode = false;
	appPrefs.pdfPrefs.fileName = "";
	appPrefs.pdfPrefs.isGrayscale = false;
	appPrefs.pdfPrefs.UseRGB = true;
	appPrefs.pdfPrefs.UseProfiles = false;
	appPrefs.pdfPrefs.UseProfiles2 = false;
	appPrefs.pdfPrefs.SolidProf = "";
	appPrefs.pdfPrefs.SComp = 3;
	appPrefs.pdfPrefs.ImageProf = "";
	appPrefs.pdfPrefs.PrintProf = "";
	appPrefs.pdfPrefs.Info = "";
	appPrefs.pdfPrefs.Intent2 = 0;
	appPrefs.pdfPrefs.Intent = 0;
	appPrefs.pdfPrefs.bleeds.set(0, 0, 0, 0);
	appPrefs.pdfPrefs.useDocBleeds = true;
	appPrefs.pdfPrefs.cropMarks = false;
	appPrefs.pdfPrefs.bleedMarks = false;
	appPrefs.pdfPrefs.registrationMarks = false;
	appPrefs.pdfPrefs.colorMarks = false;
	appPrefs.pdfPrefs.docInfoMarks = false;
	appPrefs.pdfPrefs.markLength = 20;
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
	appPrefs.pdfPrefs.openAfterExport = false;
	appPrefs.pdfPrefs.PageLayout = PDFOptions::SinglePage;
	appPrefs.pdfPrefs.openAction = "";
	appPrefs.imageCachePrefs.cacheEnabled = false;
	appPrefs.imageCachePrefs.maxCacheSizeMiB = 1000;
	appPrefs.imageCachePrefs.maxCacheEntries = 1000;
	appPrefs.imageCachePrefs.compressionLevel = 1;
	appPrefs.activePageSizes.clear();
	appPrefs.activePageSizes << "A3" << "A4" << "A5" << "A6" << "Letter";

	//Attribute setup
	appPrefs.itemAttrPrefs.defaultItemAttributes.clear();
	appPrefs.tocPrefs.defaultToCSetups.clear();

	//Experimental Features
	appPrefs.experimentalFeaturePrefs.notesEnabled = false;

	initDefaultActionKeys();
}

void PrefsManager::initDefaultActionKeys()
{
	ActionManager::createDefaultShortcuts();
	const QMap<QString, QKeySequence > *map = ActionManager::defaultShortcuts();
	for (auto it = map->constBegin(); it != map->constEnd(); ++it)
	{
		Keys& keyAction = appPrefs.keyShortcutPrefs.KeyActions[it.key()];
		keyAction.actionName = it.key();
		keyAction.keySequence = it.value();
	}
}

void PrefsManager::applyLoadedShortCuts()
{
	const auto &actions = ScCore->primaryMainWindow()->scrActions;

	for (auto it = appPrefs.keyShortcutPrefs.KeyActions.begin(); it != appPrefs.keyShortcutPrefs.KeyActions.end(); ++it)
	{
		if (it.value().actionName.isEmpty())
			continue;
		QPointer<ScrAction> action = actions.value(it.value().actionName, QPointer<ScrAction>());
		if (action.isNull())
			continue;
		action->setShortcut(it.value().keySequence);
		action->setToolTipFromTextAndShortcut();
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

const QString& PrefsManager::preferencesLocation() const
{
	return m_prefsLocation;
}

void PrefsManager::setupPreferencesLocation()
{
	m_prefsLocation = ScPaths::preferencesDir(true);
}

void PrefsManager::copyOldAppConfigAndData()
{
	if (QFile::exists(m_prefsLocation + "scribus150.rc") && QFile::exists(m_prefsLocation + "prefs150.xml"))
		return;

	//Move to using the ScPaths default prefs location/scribus.* from ~/.scribus.*
	QString oldPR = QDir::toNativeSeparators(QDir::homePath() + "/.scribus.rc");
	QString newPR = QDir::toNativeSeparators(m_prefsLocation + "/scribus.rc");
	QFileInfo oldPi(oldPR);
	if (oldPi.exists() && !QFile::exists(newPR))
		moveFile(oldPR, m_prefsLocation + "scribus.rc");

	QString oldPR2 = QDir::toNativeSeparators(QDir::homePath() + "/.scribusfont.rc");
	QString newPR2 = QDir::toNativeSeparators(m_prefsLocation + "/scribusfont.rc");
	QFileInfo oldPi2(oldPR2);
	if (oldPi2.exists() && !QFile::exists(newPR2))
		moveFile(oldPR2, m_prefsLocation + "scribusfont.rc");

	QString oldPR3 = QDir::toNativeSeparators(QDir::homePath() + "/.scribusscrap.scs");
	QString newPR3 = QDir::toNativeSeparators(m_prefsLocation + "/scrap.scs");
	QFileInfo oldPi3(oldPR3);
	if (oldPi3.exists() && !QFile::exists(newPR3))
		moveFile(oldPR3, m_prefsLocation + "scrap.scs");

	QString oldPrefsLocation(ScPaths::oldApplicationDataDir());
	QString oldPR4 = QDir::toNativeSeparators(oldPrefsLocation + "scribus150.rc");
	QString newPR4 = QDir::toNativeSeparators(m_prefsLocation + "scribus150.rc");
	QFileInfo oldPi4(oldPR4);
	if (oldPi4.exists() && !QFile::exists(newPR4))
		moveFile(oldPR4, m_prefsLocation + "scribus150.rc");

	QString oldPR5 = QDir::toNativeSeparators(oldPrefsLocation + "scrap150.scs");
	QString newPR5 = QDir::toNativeSeparators(m_prefsLocation + "scrap150.scs");
	QFileInfo oldPi5(oldPR5);
	if (oldPi5.exists() && !QFile::exists(newPR5))
		moveFile(oldPR5, m_prefsLocation + "scrap150.scs");

	QString oldPR6 = QDir::toNativeSeparators(oldPrefsLocation + "prefs150.xml");
	QString newPR6 = QDir::toNativeSeparators(m_prefsLocation + "prefs150.xml");
	QFileInfo oldPi6(oldPR6);
	if (oldPi6.exists() && !QFile::exists(newPR6))
		moveFile(oldPR6, m_prefsLocation + "prefs150.xml");

	QString oldPR7 = QDir::toNativeSeparators(oldPrefsLocation + "scripter150.rc");
	QString newPR7 = QDir::toNativeSeparators(m_prefsLocation + "scripter150.rc");
	QFileInfo oldPi7(oldPR7);
	if (oldPi7.exists() && !QFile::exists(newPR7))
		moveFile(oldPR7, m_prefsLocation + "scripter150.rc");

	QString oldPR8 = QDir::toNativeSeparators(oldPrefsLocation + "checkfonts.xml");
	QString newPR8 = QDir::toNativeSeparators(m_prefsLocation + "checkfonts150.xml");
	QFileInfo oldPi8(oldPR8);
	if (oldPi8.exists() && !QFile::exists(newPR8))
		moveFile(oldPR8, m_prefsLocation + "checkfonts150.xml");


	//Move plugin data files to new plugin data file directory
	QDir oldPluginData(ScPaths::preferencesDir() + "/plugins");
	const QFileInfoList filPluginData(oldPluginData.entryInfoList());
	for (const QFileInfo &fiP : filPluginData)
		moveFile(fiP.absoluteFilePath(), ScPaths::pluginDataDir(true) + fiP.fileName());

	//Move scrapbook files to new scrapbook directory
	QDir oldScrapData(ScPaths::preferencesDir() + "/scrapbook");
	const QFileInfoList filScrapData(oldScrapData.entryInfoList());
	for (const QFileInfo &fiS : filScrapData)
		moveFile(fiS.absoluteFilePath(), ScPaths::scrapbookDir(true) + fiS.fileName());

	//Move swatch files to new palette directory
	QDir oldPaletteData(ScPaths::preferencesDir() + "/swatches");
	const QFileInfoList filPaletteData(oldPaletteData.entryInfoList());
	for (const QFileInfo &fiPal : filPaletteData)
		moveFile(fiPal.absoluteFilePath(), ScPaths::userPaletteFilesDir(true) + fiPal.fileName());
	QDir oldPaletteData2 = ScPaths::preferencesDir() + "/palettes";
	const QFileInfoList filPaletteData2(oldPaletteData2.entryInfoList());
	for (const QFileInfo &fiPal : filPaletteData2)
		moveFile(fiPal.absoluteFilePath(), ScPaths::userPaletteFilesDir(true) + fiPal.fileName());

	//Now make copies for 1.3 use and leave the old ones alone for <1.3.0 usage
	QString prefs135[5];
	QString prefs140[5];
	QString prefs150[5];

	prefs135[0] = QDir::toNativeSeparators(m_prefsLocation + "scribus135.rc");
	prefs135[1] = QDir::toNativeSeparators(m_prefsLocation + "scrap135.scs");
	prefs135[2] = QDir::toNativeSeparators(m_prefsLocation + "prefs135.xml");
	prefs135[3] = QDir::toNativeSeparators(m_prefsLocation + "scripter135.rc");
	prefs135[4] = QDir::toNativeSeparators(m_prefsLocation + "checkfonts.xml");

	prefs140[0] = QDir::toNativeSeparators(m_prefsLocation + "scribus140.rc");
	prefs140[1] = QDir::toNativeSeparators(m_prefsLocation + "scrap140.scs");
	prefs140[2] = QDir::toNativeSeparators(m_prefsLocation + "prefs140.xml");
	prefs140[3] = QDir::toNativeSeparators(m_prefsLocation + "scripter140.rc");
	prefs140[4] = QDir::toNativeSeparators(m_prefsLocation + "checkfonts.xml");

	prefs150[0] = QDir::toNativeSeparators(m_prefsLocation + "scribus150.rc");
	prefs150[1] = QDir::toNativeSeparators(m_prefsLocation + "scrap150.scs");
	prefs150[2] = QDir::toNativeSeparators(m_prefsLocation + "prefs150.xml");
	prefs150[3] = QDir::toNativeSeparators(m_prefsLocation + "scripter150.rc");
	prefs150[4] = QDir::toNativeSeparators(m_prefsLocation + "checkfonts150.xml");

	bool existsPrefs135[5];
	bool existsPrefs140[5];
	bool existsPrefs150[5];
	for (uint i = 0; i < 5; ++i)
	{
		existsPrefs135[i] = QFile::exists(prefs135[i]);
		existsPrefs140[i] = QFile::exists(prefs140[i]);
		existsPrefs150[i] = QFile::exists(prefs150[i]);
	}

	if (existsPrefs150[0] && existsPrefs150[2])
		return;

	//Only check for these three as they will be autocreated if they don't exist.
	if ((existsPrefs135[0] && !existsPrefs140[0]) || (existsPrefs135[2] && !existsPrefs140[2]))
	{
		if (ScCore->usingGUI())
		{
			bool splashShown = ScCore->splashShowing();
			if (splashShown)
				ScCore->showSplash(false);
			if ( ScMessageBox::question( ScCore->primaryMainWindow(), tr("Migrate Old Scribus Settings?"),
					tr("Scribus has detected existing Scribus 1.3.5 preferences files.\n"
					"Do you want to migrate them to the new Scribus version?"),
					QMessageBox::Yes | QMessageBox::No,
					QMessageBox::NoButton,	// GUI default
					QMessageBox::Yes	// batch default
				) == QMessageBox::Yes )
			{
				for (uint i = 0; i < 5; ++i)
				{
					if (existsPrefs135[i] && !existsPrefs150[i])
						copyFile(prefs135[i], prefs150[i]);
				}
			}
			if (splashShown)
				ScCore->showSplash(true);
		}
	}
	else if (existsPrefs140[0])
	{
		for (uint i = 0; i < 5; ++i)
		{
			if (existsPrefs140[i] && !existsPrefs150[i])
				copyFile(prefs140[i], prefs150[i]);
		}
	}
	else if (existsPrefs135[0])
	{
		for (uint i = 0; i < 5; ++i)
		{
			if (existsPrefs135[i] && !existsPrefs150[i])
				copyFile(prefs135[i], prefs150[i]);
		}
	}
}

void PrefsManager::readPrefs()
{
	QString prefsFile(m_prefsLocation + "scribus150.rc");
	if (QFile::exists(prefsFile))
	{
		if (!readPref(prefsFile))
		{
			alertLoadPrefsFailed();
			return;
		}
	}
}

void PrefsManager::readPrefsXML()
{
	if (!prefsFile)
		return;
	PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
	if (userprefsContext)
	{
		QString guiLanguage = userprefsContext->get("gui_language", "");
		if (!guiLanguage.isEmpty())
			appPrefs.uiPrefs.language = guiLanguage;
		if (appPrefs.uiPrefs.language.isEmpty())
		{
			appPrefs.uiPrefs.language = ScQApp->currGUILanguage();
			if (appPrefs.uiPrefs.language.isEmpty())
				appPrefs.uiPrefs.language = "en_GB"; // If we get here, Houston, we have a problem!
		}
		if (!LanguageManager::instance()->isAvailableTranslation(appPrefs.uiPrefs.language))
		{
			appPrefs.uiPrefs.language = ScQApp->currGUILanguage();
			if (!LanguageManager::instance()->isAvailableTranslation(appPrefs.uiPrefs.language))
				appPrefs.uiPrefs.language = "en_GB"; // If we get here, Houston, we have a problem!
		}
		appPrefs.uiPrefs.mainWinState = QByteArray::fromBase64(userprefsContext->get("mainwinstate", "").toLatin1());
		appPrefs.uiPrefs.tabbedPalettes.clear();
		PrefsTable *tabsTable = userprefsContext->getTable("tabbedPalettes");
		PrefsTable *actTabsTable = userprefsContext->getTable("activeTabs");
		if (tabsTable)
		{
			for (int r = 0; r < tabsTable->getRowCount(); r++)
			{
				tabPrefs tabs;
				for (int c = 0; c < tabsTable->getColCount(); c++)
				{
					QString tabName = tabsTable->get(r, c);
					if (!tabName.isEmpty())
						tabs.palettes.append(tabsTable->get(r, c));
				}
				if (actTabsTable)
					tabs.activeTab = actTabsTable->getInt(r, 0);
				else
					tabs.activeTab = -1;
				appPrefs.uiPrefs.tabbedPalettes.append(tabs);
			}
		}
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


void PrefsManager::savePrefs()
{
	// If closing because of a crash don't save prefs as we can
	// accidentally nuke the settings if the crash is before prefs are loaded
	// The caller is responsible for ensuring we aren't called under those
	// conditions.
	Q_ASSERT(!emergencyActivated);
	int currentScreen = ScCore->primaryMainWindow()->getScreenNumber();
	int currentScreenXPos = 0;
	int currentScreenYPos = 0;
	ScCore->primaryMainWindow()->getScreenPosition(currentScreenXPos, currentScreenYPos);
	appPrefs.uiPrefs.mainWinSettings.xPosition = ScCore->primaryMainWindow()->pos().x() - currentScreenXPos;
	appPrefs.uiPrefs.mainWinSettings.yPosition = ScCore->primaryMainWindow()->pos().y() - currentScreenYPos;
	appPrefs.uiPrefs.mainWinSettings.width = ScCore->primaryMainWindow()->size().width();
	appPrefs.uiPrefs.mainWinSettings.height = ScCore->primaryMainWindow()->size().height();
	appPrefs.uiPrefs.mainWinSettings.maximized = ScCore->primaryMainWindow()->isMaximized();
	appPrefs.uiPrefs.mainWinSettings.screenNumber = currentScreen;
	appPrefs.uiPrefs.mainWinState = ScCore->primaryMainWindow()->saveState();
	appPrefs.uiPrefs.RecentDocs.clear();
	uint max = qMin(appPrefs.uiPrefs.recentDocCount, ScCore->primaryMainWindow()->m_recentDocsList.count());
	for (uint m = 0; m < max; ++m)
	{
		appPrefs.uiPrefs.RecentDocs.append(ScCore->primaryMainWindow()->m_recentDocsList[m]);
	}
	ScCore->primaryMainWindow()->getDefaultPrinter(appPrefs.printerPrefs.PrinterName, appPrefs.printerPrefs.PrinterFile, appPrefs.printerPrefs.PrinterCommand);
	savePrefsXML();
	if (!writePref(m_prefsLocation + "scribus150.rc"))
		alertSavePrefsFailed();
	emit prefsChanged();
}

void PrefsManager::savePrefsXML()
{
	if (prefsFile)
	{
		PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
		if (userprefsContext)
		{
			userprefsContext->set("gui_language", appPrefs.uiPrefs.language);
			userprefsContext->set("mainwinstate", QString::fromLatin1(appPrefs.uiPrefs.mainWinState.toBase64()));
			if (!appPrefs.uiPrefs.tabbedPalettes.isEmpty())
			{
				int maxCols = 0;
				for (int a = 0; a < appPrefs.uiPrefs.tabbedPalettes.count(); a++)
				{
					maxCols = qMax(maxCols, appPrefs.uiPrefs.tabbedPalettes[a].palettes.count());
				}
				PrefsTable *tabsTable = userprefsContext->getTable("tabbedPalettes");
				tabsTable->clear();
				PrefsTable *actTabsTable = userprefsContext->getTable("activeTabs");
				actTabsTable->clear();
				for (int a = 0; a < appPrefs.uiPrefs.tabbedPalettes.count(); a++)
				{
					QStringList actTab = appPrefs.uiPrefs.tabbedPalettes[a].palettes;
					for (int i = 0; i < actTab.count(); i++)
					{
						tabsTable->set(a, i, actTab[i]);
					}
					actTabsTable->set(a, 0, appPrefs.uiPrefs.tabbedPalettes[a].activeTab);
					if (actTab.count() < maxCols)
					{
						for (int i = actTab.count(); i < maxCols; i++)
						{
							tabsTable->set(a, i, "dummy");
						}
					}

				}
			}
			//continue here...
			//Prefs."blah blah" =...
		}
		prefsFile->write();
	}
}

void PrefsManager::setGhostscriptExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.gs_exe = executableName;
}

void PrefsManager::setImageEditorExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.imageEditorExecutable = executableName;
}

void PrefsManager::setExtBrowserExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.extBrowserExecutable = executableName;
}

void PrefsManager::setUniconvExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.uniconvExecutable = executableName;
}

void PrefsManager::setLatexConfigs(const QStringList& configs)
{
	appPrefs.extToolPrefs.latexConfigs = configs;
}

void PrefsManager::setLatexCommands(const QMap<QString, QString>& commands)
{
	appPrefs.extToolPrefs.latexCommands = commands;
}

bool PrefsManager::renderFrameConfigured()
{
	if (appPrefs.extToolPrefs.latexConfigs.isEmpty())
		return false;
	if (appPrefs.extToolPrefs.latexCommands.isEmpty())
		return false;
	bool foundAny = false;
	foreach (QString cmd, appPrefs.extToolPrefs.latexCommands)
	{
		QStringList args = splitCommandLineArgs(cmd);
		if (args.isEmpty())
			continue;
		if (fileInPath(args.at(0)))
		{
			foundAny = true;
			break;
		}
	}
	return foundAny;
}


void PrefsManager::setLatexEditorExecutable(const QString& executableName)
{
	appPrefs.extToolPrefs.latexEditorExecutable = executableName;
}

const QString& PrefsManager::documentDir() const
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
	appPrefs.fontPrefs.AvailFonts.getFonts(m_prefsLocation, showFontInfo);
	return !appPrefs.fontPrefs.AvailFonts.isEmpty();
}

void PrefsManager::setShowStartupDialog(const bool showDialog)
{
	appPrefs.uiPrefs.showStartupDialog = showDialog;
}

const ColorList& PrefsManager::colorSet() const
{
	return appPrefs.colorPrefs.DColors;
}

ColorList* PrefsManager::colorSetPtr()
{
	return &appPrefs.colorPrefs.DColors;
}

const QString& PrefsManager::colorSetName() const
{
	return appPrefs.colorPrefs.DColorSet;
}

bool PrefsManager::isToolColor(const QString& name) const
{
	return isToolColor(appPrefs.itemToolPrefs, name);
}

bool PrefsManager::isToolColor(const ItemToolPrefs& settings, const QString& name)
{
	if (settings.calligraphicPenLineColor == name)
		return true;
	if (settings.calligraphicPenFillColor == name)
		return true;
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
	if (settings.imageStrokeColor == name)
		return true;
	return false;
}

QStringList PrefsManager::toolColorNames() const
{
	return toolColorNames(appPrefs.itemToolPrefs);
}

QStringList PrefsManager::toolColorNames(const ItemToolPrefs& settings)
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
	if (!names.contains(settings.imageStrokeColor))
		names.append(settings.imageStrokeColor);
	if (!names.contains(settings.calligraphicPenFillColor))
		names.append(settings.calligraphicPenFillColor);
	if (!names.contains(settings.calligraphicPenLineColor))
		names.append(settings.calligraphicPenLineColor);
	return names;
}

void PrefsManager::replaceToolColors(const QMap<QString, QString>& replaceMap)
{
	replaceToolColors(appPrefs.itemToolPrefs, replaceMap);
}

void PrefsManager::replaceToolColors(ItemToolPrefs& settings, const QMap<QString, QString>& replaceMap)
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
	if (replaceMap.contains(settings.imageStrokeColor))
		settings.imageStrokeColor = replaceMap[settings.imageStrokeColor];
	if (replaceMap.contains(settings.calligraphicPenFillColor))
		settings.calligraphicPenFillColor = replaceMap[settings.calligraphicPenFillColor];
	if (replaceMap.contains(settings.calligraphicPenLineColor))
		settings.calligraphicPenLineColor = replaceMap[settings.calligraphicPenLineColor];
}

void PrefsManager::replaceToolResources(const ResourceCollection& newNames)
{
	replaceToolResources(appPrefs.itemToolPrefs, newNames);
}

void PrefsManager::replaceToolResources(ItemToolPrefs& settings, const ResourceCollection& newNames)
{
	const auto& newFonts = newNames.fonts();

	auto it = newFonts.find(settings.textFont);
	if (it != newFonts.cend())
	{
		const ScFace& newFace = newNames.availableFonts->findFont(it.value(), nullptr);
		if (!newFace.isNone())
			settings.textFont = it.value();
	}

	PrefsManager::replaceToolColors(settings, newNames.colors());
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
	QString penPict = appPrefs.itemToolPrefs.imageStrokeColor;
	if (!tmpSet.contains(penPict) && penPict != CommonStrings::None)
		tmpSet[penPict] = appPrefs.colorPrefs.DColors[penPict];
	QString brushCpen = appPrefs.itemToolPrefs.calligraphicPenFillColor;
	if (!tmpSet.contains(brushCpen) && brushCpen != CommonStrings::None)
		tmpSet[brushCpen] = appPrefs.colorPrefs.DColors[brushCpen];
	QString brushCpen2 = appPrefs.itemToolPrefs.calligraphicPenLineColor;
	if (!tmpSet.contains(brushCpen2) && brushCpen2 != CommonStrings::None)
		tmpSet[brushCpen2] = appPrefs.colorPrefs.DColors[brushCpen2];
	appPrefs.colorPrefs.DColors = tmpSet;
	appPrefs.colorPrefs.DColors.ensureDefaultColors();
}

void PrefsManager::setColorSetName(const QString& colorSetName)
{
	appPrefs.colorPrefs.DColorSet = colorSetName;
}


void PrefsManager::setKeyEntry(const QString& actName, const QString& cleanMenuText, const QKeySequence& keyseq, const int& rowNumber)
{
	Keys ke;
	if (!actName.isEmpty())
	{
		if (ScCore->primaryMainWindow()->scrActions[actName])
		{
			ke.actionName = actName;
			ke.keySequence = keyseq;
			ke.cleanMenuText = cleanMenuText;
			ke.tableRow = rowNumber;
			appPrefs.keyShortcutPrefs.KeyActions.insert(actName, ke);
		}
		else
			qDebug("%s", QString("Action Name: %1 does not exist").arg(actName).toLatin1().constData());
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

const QString& PrefsManager::guiIconSet() const
{
	return appPrefs.uiPrefs.iconSet;
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

bool PrefsManager::writePref(const QString& filePath)
{
	QDomDocument docu("scribusrc");
	QString st = "<SCRIBUSRC></SCRIBUSRC>";
	docu.setContent(st);
	QDomElement elem = docu.documentElement();
	elem.setAttribute("VERSION", "1.5.0");

	QDomElement dcUI = docu.createElement("UI");
	dcUI.setAttribute("ShowStartupDialog", static_cast<int>(appPrefs.uiPrefs.showStartupDialog));
	dcUI.setAttribute("ShowSplashOnStartup", static_cast<int>(appPrefs.uiPrefs.showSplashOnStartup));
	dcUI.setAttribute("UseSmallWidgets", static_cast<int>(appPrefs.uiPrefs.useSmallWidgets));
	dcUI.setAttribute("UseDocumentTabs", static_cast<int>(appPrefs.uiPrefs.useTabs));
	dcUI.setAttribute("StickyTools", static_cast<int>(appPrefs.uiPrefs.stickyTools));
	dcUI.setAttribute("Theme", appPrefs.uiPrefs.style);
	dcUI.setAttribute("ScrollWheelJump", appPrefs.uiPrefs.wheelJump);
	dcUI.setAttribute("MouseMoveTimeout", appPrefs.uiPrefs.mouseMoveTimeout);
	dcUI.setAttribute("ApplicationFontSize", appPrefs.uiPrefs.applicationFontSize);
	dcUI.setAttribute("PaletteFontSize", appPrefs.uiPrefs.paletteFontSize);
	dcUI.setAttribute("RecentDocumentCount", appPrefs.uiPrefs.recentDocCount);
	dcUI.setAttribute("UseGrayscaleIcons", appPrefs.uiPrefs.grayscaleIcons);
	dcUI.setAttribute("IconSet", appPrefs.uiPrefs.iconSet);
	dcUI.setAttribute("UserPreferredLocale", appPrefs.uiPrefs.userPreferredLocale);
	elem.appendChild(dcUI);

	QDomElement deDocumentSetup = docu.createElement("DocumentSetup");
	deDocumentSetup.setAttribute("Language", appPrefs.docSetupPrefs.language);
	deDocumentSetup.setAttribute("UnitIndex", appPrefs.docSetupPrefs.docUnitIndex);
	deDocumentSetup.setAttribute("PageSize", appPrefs.docSetupPrefs.pageSize);
	deDocumentSetup.setAttribute("PageOrientation", appPrefs.docSetupPrefs.pageOrientation);
	deDocumentSetup.setAttribute("PageWidth", ScCLocale::toQStringC(appPrefs.docSetupPrefs.pageWidth));
	deDocumentSetup.setAttribute("PageHeight", ScCLocale::toQStringC(appPrefs.docSetupPrefs.pageHeight));
	deDocumentSetup.setAttribute("MarginTop", ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.top()));
	deDocumentSetup.setAttribute("MarginBottom", ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.bottom()));
	deDocumentSetup.setAttribute("MarginLeft", ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.left()));
	deDocumentSetup.setAttribute("MarginRight", ScCLocale::toQStringC(appPrefs.docSetupPrefs.margins.right()));
	deDocumentSetup.setAttribute("MarginPreset", appPrefs.docSetupPrefs.marginPreset);
	deDocumentSetup.setAttribute("PagePositioning", appPrefs.docSetupPrefs.pagePositioning);
	deDocumentSetup.setAttribute("AutoSave", static_cast<int>(appPrefs.docSetupPrefs.AutoSave));
	deDocumentSetup.setAttribute("AutoSaveTime", appPrefs.docSetupPrefs.AutoSaveTime);
	deDocumentSetup.setAttribute("AutoSaveCount", appPrefs.docSetupPrefs.AutoSaveCount);
	deDocumentSetup.setAttribute("AutoSaveKeep", static_cast<int>(appPrefs.docSetupPrefs.AutoSaveKeep));
	deDocumentSetup.setAttribute("AutoSaveLoc", static_cast<int>(appPrefs.docSetupPrefs.AutoSaveLocation));
	deDocumentSetup.setAttribute("AutoSaveDir", appPrefs.docSetupPrefs.AutoSaveDir);
	deDocumentSetup.setAttribute("SaveCompressed", static_cast<int>(appPrefs.docSetupPrefs.saveCompressed));
	deDocumentSetup.setAttribute("BleedTop", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.top()));
	deDocumentSetup.setAttribute("BleedLeft", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.left()));
	deDocumentSetup.setAttribute("BleedRight", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.right()));
	deDocumentSetup.setAttribute("BleedBottom", ScCLocale::toQStringC(appPrefs.docSetupPrefs.bleeds.bottom()));
	elem.appendChild(deDocumentSetup);

	QDomElement dePaths = docu.createElement("Paths");
	dePaths.setAttribute("Documents", appPrefs.pathPrefs.documents);
	dePaths.setAttribute("Profiles", appPrefs.pathPrefs.colorProfiles);
	dePaths.setAttribute("Scripts", appPrefs.pathPrefs.scripts);
	dePaths.setAttribute("Templates", appPrefs.pathPrefs.documentTemplates);
	elem.appendChild(dePaths);

	QDomElement deGuides = docu.createElement("Guides");
	deGuides.setAttribute("GrabRadius", appPrefs.guidesPrefs.grabRadius);
	deGuides.setAttribute("ShowGuides", static_cast<int>(appPrefs.guidesPrefs.guidesShown));
	deGuides.setAttribute("ShowColumnBorders", static_cast<int>(appPrefs.guidesPrefs.colBordersShown));
	deGuides.setAttribute("ShowFrames", static_cast<int>(appPrefs.guidesPrefs.framesShown));
	deGuides.setAttribute("ShowLayerMarkers", static_cast<int>(appPrefs.guidesPrefs.layerMarkersShown));
	deGuides.setAttribute("ShowMargins", static_cast<int>(appPrefs.guidesPrefs.marginsShown));
	deGuides.setAttribute("ShowBaselineGrid", static_cast<int>(appPrefs.guidesPrefs.baselineGridShown));
	deGuides.setAttribute("ShowLinks", static_cast<int>(appPrefs.guidesPrefs.linkShown));
	deGuides.setAttribute("ShowImages", static_cast<int>(appPrefs.guidesPrefs.showPic));
	deGuides.setAttribute("ShowControls", static_cast<int>(appPrefs.guidesPrefs.showControls));
	deGuides.setAttribute("ShowRulers", static_cast<int>(appPrefs.guidesPrefs.rulersShown));
	deGuides.setAttribute("ShowBleed", static_cast<int>(appPrefs.guidesPrefs.showBleed));
	deGuides.setAttribute("RulerMode", static_cast<int>(appPrefs.guidesPrefs.rulerMode));
	deGuides.setAttribute("MinorGridSpacing", ScCLocale::toQStringC(appPrefs.guidesPrefs.minorGridSpacing));
	deGuides.setAttribute("MajorGridSpacing", ScCLocale::toQStringC(appPrefs.guidesPrefs.majorGridSpacing));
	deGuides.setAttribute("MinorGridColor", appPrefs.guidesPrefs.minorGridColor.name());
	deGuides.setAttribute("MajorGridColor", appPrefs.guidesPrefs.majorGridColor.name());
	deGuides.setAttribute("GuidesColor", appPrefs.guidesPrefs.guideColor.name());
	deGuides.setAttribute("MarginColor", appPrefs.guidesPrefs.marginColor.name());
	deGuides.setAttribute("BaselineGridColor", appPrefs.guidesPrefs.baselineGridColor.name());
	deGuides.setAttribute("ObjectToGuideSnapRadius", ScCLocale::toQStringC(appPrefs.guidesPrefs.guideRad));
	QString renderStack;
	int renderStackCount = appPrefs.guidesPrefs.renderStackOrder.count();
	for (int r = 0; r < renderStackCount; r++)
	{
		renderStack += QString("%1 ").arg(appPrefs.guidesPrefs.renderStackOrder[r]);
	}
	renderStack = renderStack.trimmed();
	deGuides.setAttribute("renderStack", renderStack);
	deGuides.setAttribute("GridType", appPrefs.guidesPrefs.gridType);
	deGuides.setAttribute("ShowGrid", static_cast<int>(appPrefs.guidesPrefs.gridShown));
	deGuides.setAttribute("BaselineGridDistance", ScCLocale::toQStringC(appPrefs.guidesPrefs.valueBaselineGrid));
	deGuides.setAttribute("BaselineGridOffset", ScCLocale::toQStringC(appPrefs.guidesPrefs.offsetBaselineGrid));
	elem.appendChild(deGuides);

	QDomElement deMiscellaneous = docu.createElement("Miscellaneous");
	deMiscellaneous.setAttribute("ShowStylePreview", static_cast<int>(appPrefs.miscPrefs.haveStylePreview));
	deMiscellaneous.setAttribute("LoremIpsumUseStandard", static_cast<int>(appPrefs.miscPrefs.useStandardLI));
	deMiscellaneous.setAttribute("LoremIpsumParagraphs", appPrefs.miscPrefs.paragraphsLI);
	deMiscellaneous.setAttribute("saveEmergencyFile", static_cast<int>(appPrefs.miscPrefs.saveEmergencyFile));
	elem.appendChild(deMiscellaneous);

	QDomElement deSE = docu.createElement("StoryEditor");
	deSE.setAttribute("Font", appPrefs.storyEditorPrefs.guiFont);
	if (appPrefs.storyEditorPrefs.guiFontColorBackground.isValid())
		deSE.setAttribute("FontColorBackground", appPrefs.storyEditorPrefs.guiFontColorBackground.name());
	deSE.setAttribute("SmartTextSelection", static_cast<int>(appPrefs.storyEditorPrefs.smartTextSelection));
	elem.appendChild(deSE);

	QDomElement deDisplay = docu.createElement("Display");
	deDisplay.setAttribute("ScratchBottom", ScCLocale::toQStringC(appPrefs.displayPrefs.scratch.bottom()));
	deDisplay.setAttribute("ScratchLeft", ScCLocale::toQStringC(appPrefs.displayPrefs.scratch.left()));
	deDisplay.setAttribute("ScratchRight", ScCLocale::toQStringC(appPrefs.displayPrefs.scratch.right()));
	deDisplay.setAttribute("ScratchTop", ScCLocale::toQStringC(appPrefs.displayPrefs.scratch.top()));
	deDisplay.setAttribute("PageGapHorizontal", ScCLocale::toQStringC(appPrefs.displayPrefs.pageGapHorizontal));
	deDisplay.setAttribute("PageGapVertical", ScCLocale::toQStringC(appPrefs.displayPrefs.pageGapVertical));
	deDisplay.setAttribute("ShowPageShadow", static_cast<int>(appPrefs.displayPrefs.showPageShadow));
	deDisplay.setAttribute("PageColor", appPrefs.displayPrefs.paperColor.name());
	deDisplay.setAttribute("ScratchColor", appPrefs.displayPrefs.scratchColor.name());
	deDisplay.setAttribute("FrameSelectedColor", appPrefs.displayPrefs.frameColor.name());
	deDisplay.setAttribute("FrameNormColor", appPrefs.displayPrefs.frameNormColor.name());
	deDisplay.setAttribute("FrameGroupColor", appPrefs.displayPrefs.frameGroupColor.name());
	deDisplay.setAttribute("FrameLockColor", appPrefs.displayPrefs.frameLockColor.name());
	deDisplay.setAttribute("FrameLinkColor", appPrefs.displayPrefs.frameLinkColor.name());
	deDisplay.setAttribute("FrameAnnotationColor", appPrefs.displayPrefs.frameAnnotationColor.name());
	deDisplay.setAttribute("PageBorderColor", appPrefs.displayPrefs.pageBorderColor.name());
	deDisplay.setAttribute("ControlCharColor", appPrefs.displayPrefs.controlCharColor.name());
	deDisplay.setAttribute("ShowMarginsFilled", static_cast<int>(appPrefs.displayPrefs.marginColored));
	deDisplay.setAttribute("DisplayScale", ScCLocale::toQStringC(appPrefs.displayPrefs.displayScale, 8));
	deDisplay.setAttribute("ShowVerifierWarningsOnCanvas", static_cast<int>(appPrefs.displayPrefs.showVerifierWarningsOnCanvas));
	deDisplay.setAttribute("ShowAutosaveClockOnCanvas", static_cast<int>(appPrefs.displayPrefs.showAutosaveClockOnCanvas));
	deDisplay.setAttribute("ToolTips", static_cast<int>(appPrefs.displayPrefs.showToolTips));
	deDisplay.setAttribute("ShowMouseCoordinates", static_cast<int>(appPrefs.displayPrefs.showMouseCoordinates));
	elem.appendChild(deDisplay);

	// Font information must be written before Fonts element so that face "usable"
	// member is set properly before one try to set default font. Allows to check
	// that default font is indeed usable, problems expected otherwise
	for (SCFontsIterator itf(appPrefs.fontPrefs.AvailFonts); itf.hasNext(); itf.next())
	{
		if (!itf.currentKey().isEmpty())
		{
			QDomElement fn = docu.createElement("Font");
			fn.setAttribute("Name",itf.currentKey());
			fn.setAttribute("Embed", static_cast<int>(itf.current().embedPs()));
			fn.setAttribute("Use", static_cast<int>(itf.current().usable()));
			fn.setAttribute("Subset", static_cast<int>(itf.current().subset()));
			elem.appendChild(fn);
		}
	}
	for (auto itfsu = appPrefs.fontPrefs.GFontSub.begin(); itfsu != appPrefs.fontPrefs.GFontSub.end(); ++itfsu)
	{
		QDomElement fosu = docu.createElement("Substitute");
		fosu.setAttribute("Name", itfsu.key());
		fosu.setAttribute("Replace", itfsu.value());
		elem.appendChild(fosu);
	}

	QDomElement dcFonts = docu.createElement("Fonts");
	dcFonts.setAttribute("AutomaticSubstitution", static_cast<int>(appPrefs.fontPrefs.askBeforeSubstitute));
	elem.appendChild(dcFonts);

	QDomElement dcTypography = docu.createElement("Typography");
	dcTypography.setAttribute("SubScriptDistance", appPrefs.typoPrefs.valueSubScript);
	dcTypography.setAttribute("SubScriptScaling", appPrefs.typoPrefs.scalingSubScript);
	dcTypography.setAttribute("SuperScriptDistance", appPrefs.typoPrefs.valueSuperScript);
	dcTypography.setAttribute("SuperScriptScaling", appPrefs.typoPrefs.scalingSuperScript);
	dcTypography.setAttribute("SmallCapsScaling", appPrefs.typoPrefs.valueSmallCaps);
	dcTypography.setAttribute("AutomaticLineSpacing", appPrefs.typoPrefs.autoLineSpacing);
	if (appPrefs.typoPrefs.valueUnderlinePos == -1)
		dcTypography.setAttribute("UnderlineDistance", appPrefs.typoPrefs.valueUnderlinePos);
	else
		dcTypography.setAttribute("UnderlineDistance", appPrefs.typoPrefs.valueUnderlinePos / 10.0);
	if (appPrefs.typoPrefs.valueUnderlineWidth == -1)
		dcTypography.setAttribute("UnderlineWidth", appPrefs.typoPrefs.valueUnderlineWidth);
	else
		dcTypography.setAttribute("UnderlineWidth", appPrefs.typoPrefs.valueUnderlineWidth / 10.0);
	if (appPrefs.typoPrefs.valueStrikeThruPos == -1)
		dcTypography.setAttribute("StrikeThruDistance", appPrefs.typoPrefs.valueStrikeThruPos);
	else
		dcTypography.setAttribute("StrikeThruDistance", appPrefs.typoPrefs.valueStrikeThruPos / 10.0);
	if (appPrefs.typoPrefs.valueStrikeThruWidth == -1)
		dcTypography.setAttribute("StrikeThruWidth", appPrefs.typoPrefs.valueStrikeThruWidth);
	else
		dcTypography.setAttribute("StrikeThruWidth", appPrefs.typoPrefs.valueStrikeThruWidth / 10.0);
	elem.appendChild(dcTypography);

	QDomElement dcItemTools = docu.createElement("ItemTools");
	dcItemTools.setAttribute("ShapeLineColor", appPrefs.itemToolPrefs.shapeLineColor);
	dcItemTools.setAttribute("ShapeFillColor", appPrefs.itemToolPrefs.shapeFillColor);
	dcItemTools.setAttribute("LineColor", appPrefs.itemToolPrefs.lineColor);
	dcItemTools.setAttribute("TextColor", appPrefs.itemToolPrefs.textColor);
	dcItemTools.setAttribute("TextStrokeColor", appPrefs.itemToolPrefs.textStrokeColor);
	dcItemTools.setAttribute("TextBackgroundColor", appPrefs.itemToolPrefs.textFillColor);
	dcItemTools.setAttribute("TextLineColor", appPrefs.itemToolPrefs.textLineColor);
	dcItemTools.setAttribute("TextBackgroundColorShade", appPrefs.itemToolPrefs.textFillColorShade);
	dcItemTools.setAttribute("TextLineColorShade", appPrefs.itemToolPrefs.textLineColorShade);
	dcItemTools.setAttribute("TextColorShade", appPrefs.itemToolPrefs.textShade);
	dcItemTools.setAttribute("TextStrokeColorShade", appPrefs.itemToolPrefs.textStrokeShade);
	dcItemTools.setAttribute("TextColumnCount", appPrefs.itemToolPrefs.textColumns);
	dcItemTools.setAttribute("TextColumnGap", ScCLocale::toQStringC(appPrefs.itemToolPrefs.textColumnGap));
	dcItemTools.setAttribute("TextTabWidth", ScCLocale::toQStringC(appPrefs.itemToolPrefs.textTabWidth));
	dcItemTools.setAttribute("TextDistanceTop", ScCLocale::toQStringC(appPrefs.itemToolPrefs.textDistances.top()));
	dcItemTools.setAttribute("TextDistanceBottom", ScCLocale::toQStringC(appPrefs.itemToolPrefs.textDistances.bottom()));
	dcItemTools.setAttribute("TextDistanceLeft", ScCLocale::toQStringC(appPrefs.itemToolPrefs.textDistances.left()));
	dcItemTools.setAttribute("TextDistanceRight", ScCLocale::toQStringC(appPrefs.itemToolPrefs.textDistances.right()));
	dcItemTools.setAttribute("TabFillCharacter", appPrefs.itemToolPrefs.textTabFillChar);
	dcItemTools.setAttribute("FirstLineOffset", (int) appPrefs.itemToolPrefs.firstLineOffset);
	dcItemTools.setAttribute("ShapeLineStyle", appPrefs.itemToolPrefs.shapeLineStyle);
	dcItemTools.setAttribute("LineStyle", appPrefs.itemToolPrefs.lineStyle);
	dcItemTools.setAttribute("ShapeLineWidth", ScCLocale::toQStringC(appPrefs.itemToolPrefs.shapeLineWidth));
	dcItemTools.setAttribute("LineWidth", ScCLocale::toQStringC(appPrefs.itemToolPrefs.lineWidth));
	dcItemTools.setAttribute("ShapeLineColorShade", appPrefs.itemToolPrefs.shapeLineColorShade);
	dcItemTools.setAttribute("LineColorShade", appPrefs.itemToolPrefs.lineColorShade);
	dcItemTools.setAttribute("ShapeFillColorShade", appPrefs.itemToolPrefs.shapeFillColorShade);
	dcItemTools.setAttribute("ImageFillColor", appPrefs.itemToolPrefs.imageFillColor);
	dcItemTools.setAttribute("ImageFillColorShade", appPrefs.itemToolPrefs.imageFillColorShade);
	dcItemTools.setAttribute("ImageStrokeColor", appPrefs.itemToolPrefs.imageStrokeColor);
	dcItemTools.setAttribute("ImageStrokeColorShade", appPrefs.itemToolPrefs.imageStrokeColorShade);
	dcItemTools.setAttribute("ImageScaleX", ScCLocale::toQStringC(appPrefs.itemToolPrefs.imageScaleX));
	dcItemTools.setAttribute("ImageScaleY", ScCLocale::toQStringC(appPrefs.itemToolPrefs.imageScaleY));
	dcItemTools.setAttribute("PolygonCorners", appPrefs.itemToolPrefs.polyCorners);
	dcItemTools.setAttribute("PolygonFactor", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyFactor));
	dcItemTools.setAttribute("PolygonRotation", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyRotation));
	dcItemTools.setAttribute("PolygonInnerRotation", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyInnerRot));
	dcItemTools.setAttribute("PolygonCurvature", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyCurvature));
	dcItemTools.setAttribute("PolygonOuterCurvature", ScCLocale::toQStringC(appPrefs.itemToolPrefs.polyOuterCurvature));
	dcItemTools.setAttribute("ArcStartAngle", ScCLocale::toQStringC(appPrefs.itemToolPrefs.arcStartAngle));
	dcItemTools.setAttribute("ArcSweepAngle", ScCLocale::toQStringC(appPrefs.itemToolPrefs.arcSweepAngle));
	dcItemTools.setAttribute("SpiralStartAngle", ScCLocale::toQStringC(appPrefs.itemToolPrefs.spiralStartAngle));
	dcItemTools.setAttribute("SpiralEndAngle", ScCLocale::toQStringC(appPrefs.itemToolPrefs.spiralEndAngle));
	dcItemTools.setAttribute("SpiralFactor", ScCLocale::toQStringC(appPrefs.itemToolPrefs.spiralFactor));
	dcItemTools.setAttribute("PolygonUseFactor", static_cast<int>(appPrefs.itemToolPrefs.polyUseFactor));
	dcItemTools.setAttribute("ImageScaleType", static_cast<int>(appPrefs.itemToolPrefs.imageScaleType));
	dcItemTools.setAttribute("ImageAspectRatio", static_cast<int>(appPrefs.itemToolPrefs.imageAspectRatio));
	dcItemTools.setAttribute("ImageUseEmbeddedPath", static_cast<int>(appPrefs.itemToolPrefs.imageUseEmbeddedPath));
	dcItemTools.setAttribute("ImageLowResType", appPrefs.itemToolPrefs.imageLowResType);
	dcItemTools.setAttribute("LineStartArrow", appPrefs.itemToolPrefs.lineStartArrow);
	dcItemTools.setAttribute("LineEndArrow", appPrefs.itemToolPrefs.lineEndArrow);
	dcItemTools.setAttribute("FontFace", appPrefs.itemToolPrefs.textFont);
	dcItemTools.setAttribute("FontSize", appPrefs.itemToolPrefs.textSize / 10.0);
	dcItemTools.setAttribute("CalligraphicPenFillColor", appPrefs.itemToolPrefs.calligraphicPenFillColor);
	dcItemTools.setAttribute("CalligraphicPenLineColor", appPrefs.itemToolPrefs.calligraphicPenLineColor);
	dcItemTools.setAttribute("CalligraphicPenFillColorShade", appPrefs.itemToolPrefs.calligraphicPenFillColorShade);
	dcItemTools.setAttribute("CalligraphicPenLineColorShade", appPrefs.itemToolPrefs.calligraphicPenLineColorShade);
	dcItemTools.setAttribute("CalligraphicPenLineWidth", appPrefs.itemToolPrefs.calligraphicPenLineWidth);
	dcItemTools.setAttribute("CalligraphicPenAngle", appPrefs.itemToolPrefs.calligraphicPenAngle);
	dcItemTools.setAttribute("CalligraphicPenWidth", appPrefs.itemToolPrefs.calligraphicPenWidth);
	dcItemTools.setAttribute("CalligraphicPenStyle", appPrefs.itemToolPrefs.calligraphicPenStyle);
	elem.appendChild(dcItemTools);

	QDomElement dcOperatorTools = docu.createElement("OperatorTools");
	dcOperatorTools.setAttribute("MinimumMagnification", appPrefs.opToolPrefs.magMin);
	dcOperatorTools.setAttribute("MaximumMagnification", appPrefs.opToolPrefs.magMax);
	dcOperatorTools.setAttribute("MagnificationStep", appPrefs.opToolPrefs.magStep);
	dcOperatorTools.setAttribute("DisplayOffsetX", ScCLocale::toQStringC(appPrefs.opToolPrefs.dispX));
	dcOperatorTools.setAttribute("DisplayOffsetY", ScCLocale::toQStringC(appPrefs.opToolPrefs.dispY));
	dcOperatorTools.setAttribute("RotationConstrainAngle", ScCLocale::toQStringC(appPrefs.opToolPrefs.constrain));
	elem.appendChild(dcOperatorTools);

	QDomElement dcMainWindow = docu.createElement("MainWindow");
	dcMainWindow.setAttribute("XPosition", appPrefs.uiPrefs.mainWinSettings.xPosition);
	dcMainWindow.setAttribute("YPosition", appPrefs.uiPrefs.mainWinSettings.yPosition);
	dcMainWindow.setAttribute("Width", appPrefs.uiPrefs.mainWinSettings.width);
	dcMainWindow.setAttribute("Height", appPrefs.uiPrefs.mainWinSettings.height);
	dcMainWindow.setAttribute("Maximized", static_cast<int>(appPrefs.uiPrefs.mainWinSettings.maximized));
	dcMainWindow.setAttribute("ScreenNumber", appPrefs.uiPrefs.mainWinSettings.screenNumber);
	elem.appendChild(dcMainWindow);

	QDomElement dcScrapbook = docu.createElement("ScrapBook");
	dcScrapbook.setAttribute("CopyToScrapbook", static_cast<int>(appPrefs.scrapbookPrefs.doCopyToScrapbook));
	dcScrapbook.setAttribute("PersistentScrapbook", static_cast<int>(appPrefs.scrapbookPrefs.persistentScrapbook));
	dcScrapbook.setAttribute("WritePreviews", static_cast<int>(appPrefs.scrapbookPrefs.writePreviews));
	dcScrapbook.setAttribute("ScrapbookCopies", appPrefs.scrapbookPrefs.numScrapbookCopies);
	for (int i=0; i<appPrefs.scrapbookPrefs.RecentScrapbooks.count(); ++i)
	{
		QDomElement rde = docu.createElement("Recent");
		rde.setAttribute("Name", appPrefs.scrapbookPrefs.RecentScrapbooks[i]);
		dcScrapbook.appendChild(rde);
	}
	elem.appendChild(dcScrapbook);


	QDomElement pageSetAttr = docu.createElement("PageSets");
	for (const PageSet& pageSet : appPrefs.pageSets)
	{
		QDomElement pgst = docu.createElement("Set");
		pgst.setAttribute("Name", pageSet.Name);
		pgst.setAttribute("FirstPage", pageSet.FirstPage);
		pgst.setAttribute("Rows", pageSet.Rows);
		pgst.setAttribute("Columns", pageSet.Columns);
		for (const QString& pageName : pageSet.pageNames)
		{
			QDomElement pgstN = docu.createElement("PageNames");
			pgstN.setAttribute("Name", pageName);
			pgst.appendChild(pgstN);
		}
		pageSetAttr.appendChild(pgst);
	}
	elem.appendChild(pageSetAttr);

	QDomElement dcPreflightVerifier = docu.createElement("PreflightVerifier");
	dcPreflightVerifier.setAttribute("CurrentProfile", appPrefs.verifierPrefs.curCheckProfile);
	dcPreflightVerifier.setAttribute("ShowPagesWithoutErrors", appPrefs.verifierPrefs.showPagesWithoutErrors);
	dcPreflightVerifier.setAttribute("ShowNonPrintingLayerErrors", appPrefs.verifierPrefs.showNonPrintingLayerErrors);
	elem.appendChild(dcPreflightVerifier);

	CheckerPrefsList::Iterator itcpend = appPrefs.verifierPrefs.checkerPrefsList.end();
	for (auto itcp = appPrefs.verifierPrefs.checkerPrefsList.begin(); itcp != itcpend; ++itcp)
	{
		QDomElement dcVerifierProfile = docu.createElement("VerifierProfile");
		const CheckerPrefs& checkerProfile = itcp.value();
		dcVerifierProfile.setAttribute("Name", itcp.key());
		dcVerifierProfile.setAttribute("IgnoreErrors", static_cast<int>(checkerProfile.ignoreErrors));
		dcVerifierProfile.setAttribute("AutoCheck", static_cast<int>(checkerProfile.autoCheck));
		dcVerifierProfile.setAttribute("CheckGlyphs", static_cast<int>(checkerProfile.checkGlyphs));
		dcVerifierProfile.setAttribute("CheckOrphans", static_cast<int>(checkerProfile.checkOrphans));
		dcVerifierProfile.setAttribute("CheckOverflow", static_cast<int>(checkerProfile.checkOverflow));
		dcVerifierProfile.setAttribute("CheckPictures", static_cast<int>(checkerProfile.checkPictures));
		dcVerifierProfile.setAttribute("CheckResolution", static_cast<int>(checkerProfile.checkResolution));
		dcVerifierProfile.setAttribute("CheckPartFilledImageFrames", static_cast<int>(checkerProfile.checkPartFilledImageFrames));
		dcVerifierProfile.setAttribute("CheckTransparency", static_cast<int>(checkerProfile.checkTransparency));
		dcVerifierProfile.setAttribute("CheckAnnotations", static_cast<int>(checkerProfile.checkAnnotations));
		dcVerifierProfile.setAttribute("CheckRasterPDF", static_cast<int>(checkerProfile.checkRasterPDF));
		dcVerifierProfile.setAttribute("CheckForGIF", static_cast<int>(checkerProfile.checkForGIF));
		dcVerifierProfile.setAttribute("IgnoreOffLayers", static_cast<int>(checkerProfile.ignoreOffLayers));
		dcVerifierProfile.setAttribute("CheckOffConflictLayers", static_cast<int>(checkerProfile.checkOffConflictLayers));
		dcVerifierProfile.setAttribute("MinimumResolution", ScCLocale::toQStringC(checkerProfile.minResolution));
		dcVerifierProfile.setAttribute("MaximumResolution", ScCLocale::toQStringC(checkerProfile.maxResolution));
		dcVerifierProfile.setAttribute("CheckNotCMYKOrSpot", static_cast<int>(checkerProfile.checkNotCMYKOrSpot));
		dcVerifierProfile.setAttribute("CheckDeviceColorsAndOutputIntent", static_cast<int>(checkerProfile.checkDeviceColorsAndOutputIntent));
		dcVerifierProfile.setAttribute("CheckFontNotEmbedded", static_cast<int>(checkerProfile.checkFontNotEmbedded));
		dcVerifierProfile.setAttribute("CheckFontIsOpenType", static_cast<int>(checkerProfile.checkFontIsOpenType));
		dcVerifierProfile.setAttribute("CheckAppliedMasterDifferentSide", static_cast<int>(checkerProfile.checkAppliedMasterDifferentSide));
		dcVerifierProfile.setAttribute("CheckEmptyTextFrames", static_cast<int>(checkerProfile.checkEmptyTextFrames));
		elem.appendChild(dcVerifierProfile);
	}

	QDomElement dcColorManagement = docu.createElement("ColorManagement");
	dcColorManagement.setAttribute("SoftProofOn", static_cast<int>(appPrefs.colorPrefs.DCMSset.SoftProofOn));
	dcColorManagement.setAttribute("SoftProofFullOn", static_cast<int>(appPrefs.colorPrefs.DCMSset.SoftProofFullOn));
	dcColorManagement.setAttribute("ColorManagementActive", static_cast<int>(appPrefs.colorPrefs.DCMSset.CMSinUse));
	dcColorManagement.setAttribute("GamutCheck", static_cast<int>(appPrefs.colorPrefs.DCMSset.GamutCheck));
	dcColorManagement.setAttribute("BlackPoint", static_cast<int>(appPrefs.colorPrefs.DCMSset.BlackPoint));
	dcColorManagement.setAttribute("DefaultMonitorProfile", appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile);
	dcColorManagement.setAttribute("DefaultPrinterProfile", appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile);
	dcColorManagement.setAttribute("DefaultImageRGBProfile", appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile);
	dcColorManagement.setAttribute("DefaultImageCMYKProfile", appPrefs.colorPrefs.DCMSset.DefaultImageCMYKProfile);
	dcColorManagement.setAttribute("DefaultSolidColorRGBProfile", appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile);
	dcColorManagement.setAttribute("DefaultSolorColorCMYKProfile", appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile);
	dcColorManagement.setAttribute("DefaultIntentColors", appPrefs.colorPrefs.DCMSset.DefaultIntentColors);
	dcColorManagement.setAttribute("DefaultIntentImages", appPrefs.colorPrefs.DCMSset.DefaultIntentImages);
	elem.appendChild(dcColorManagement);

	QDomElement dcPrinter = docu.createElement("Printer");
	dcPrinter.setAttribute("Name", appPrefs.printerPrefs.PrinterName);
	dcPrinter.setAttribute("File", appPrefs.printerPrefs.PrinterFile);
	dcPrinter.setAttribute("Command", appPrefs.printerPrefs.PrinterCommand);
	dcPrinter.setAttribute("ClipToMargins", static_cast<int>(appPrefs.printerPrefs.ClipMargin));
	dcPrinter.setAttribute("GCRMode", static_cast<int>(appPrefs.printerPrefs.GCRMode));
	elem.appendChild(dcPrinter);

	QDomElement dc8Pr = docu.createElement("PrintPreview");
	dc8Pr.setAttribute("Mode", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_Mode));
	dc8Pr.setAttribute("AntiAliasing", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_AntiAliasing));
	dc8Pr.setAttribute("Transparency", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_Transparency));
	dc8Pr.setAttribute("Cyan", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_C));
	dc8Pr.setAttribute("Magenta", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_M));
	dc8Pr.setAttribute("Yellow", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_Y));
	dc8Pr.setAttribute("Black", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_K));
	dc8Pr.setAttribute("InkCoverage", static_cast<int>(appPrefs.printPreviewPrefs.PrPr_InkCoverage));
	dc8Pr.setAttribute("InkThreshold", appPrefs.printPreviewPrefs.PrPr_InkThreshold);
	elem.appendChild(dc8Pr);

	QDomElement dcPdfOutputPrev = docu.createElement("PDFOutputPreview");
	dcPdfOutputPrev.setAttribute("AntiAliasing", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.enableAntiAliasing));
	dcPdfOutputPrev.setAttribute("Transparency", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.showTransparency));
	dcPdfOutputPrev.setAttribute("CMYKMode", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.cmykPreviewMode));
	dcPdfOutputPrev.setAttribute("Cyan", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.isCyanVisible));
	dcPdfOutputPrev.setAttribute("Magenta", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.isMagentaVisible));
	dcPdfOutputPrev.setAttribute("Yellow", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.isYellowVisible));
	dcPdfOutputPrev.setAttribute("Black", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.isBlackVisible));
	dcPdfOutputPrev.setAttribute("InkCoverage", static_cast<int>(appPrefs.pdfOutputPreviewPrefs.displayInkCoverage));
	dcPdfOutputPrev.setAttribute("InkThreshold", appPrefs.pdfOutputPreviewPrefs.inkCoverageThreshold);
	elem.appendChild(dcPdfOutputPrev);

	QDomElement dcPSOutputPrev = docu.createElement("PSOutputPreview");
	dcPSOutputPrev.setAttribute("PSLevel", appPrefs.psOutputPreviewPrefs.psLevel);
	dcPSOutputPrev.setAttribute("AntiAliasing", static_cast<int>(appPrefs.psOutputPreviewPrefs.enableAntiAliasing));
	dcPSOutputPrev.setAttribute("Transparency", static_cast<int>(appPrefs.psOutputPreviewPrefs.showTransparency));
	dcPSOutputPrev.setAttribute("CMYKMode", static_cast<int>(appPrefs.psOutputPreviewPrefs.cmykPreviewMode));
	dcPSOutputPrev.setAttribute("Cyan", static_cast<int>(appPrefs.psOutputPreviewPrefs.isCyanVisible));
	dcPSOutputPrev.setAttribute("Magenta", static_cast<int>(appPrefs.psOutputPreviewPrefs.isMagentaVisible));
	dcPSOutputPrev.setAttribute("Yellow", static_cast<int>(appPrefs.psOutputPreviewPrefs.isYellowVisible));
	dcPSOutputPrev.setAttribute("Black", static_cast<int>(appPrefs.psOutputPreviewPrefs.isBlackVisible));
	dcPSOutputPrev.setAttribute("InkCoverage", static_cast<int>(appPrefs.psOutputPreviewPrefs.displayInkCoverage));
	dcPSOutputPrev.setAttribute("InkThreshold", appPrefs.psOutputPreviewPrefs.inkCoverageThreshold);
	elem.appendChild(dcPSOutputPrev);

	QDomElement dcExternalTools = docu.createElement("ExternalTools");
	dcExternalTools.setAttribute("ImageEditor", imageEditorExecutable());
	dcExternalTools.setAttribute("Ghostscript", ghostscriptExecutable());
	dcExternalTools.setAttribute("WebBrowser", extBrowserExecutable());
	dcExternalTools.setAttribute("GhostscriptAntiAliasGraphics", static_cast<int>(appPrefs.extToolPrefs.gs_AntiAliasGraphics));
	dcExternalTools.setAttribute("GhostscriptAntiAliasText", static_cast<int>(appPrefs.extToolPrefs.gs_AntiAliasText));
	dcExternalTools.setAttribute("Resolution", appPrefs.extToolPrefs.gs_Resolution);
	dcExternalTools.setAttribute("Uniconvertor", uniconvExecutable());
	dcExternalTools.setAttribute("PDFViewer", appPrefs.extToolPrefs.pdfViewerExecutable);
	dcExternalTools.setAttribute("LatexEditor", latexEditorExecutable());
	dcExternalTools.setAttribute("LatexResolution", latexResolution());
	dcExternalTools.setAttribute("LatexForceDpi", static_cast<int>(appPrefs.extToolPrefs.latexForceDpi));
	dcExternalTools.setAttribute("LatexStartWithEmptyFrames", static_cast<int>(appPrefs.extToolPrefs.latexStartWithEmptyFrames));
	const QStringList configs = latexConfigs();
	for (const QString& config : configs)
	{
		QDomElement domConfig = docu.createElement("LatexConfig");
		domConfig.setAttribute("file", config);
		domConfig.setAttribute("command", appPrefs.extToolPrefs.latexCommands[config]);
		dcExternalTools.appendChild(domConfig);
	}
	elem.appendChild(dcExternalTools);

	QDomElement rde = docu.createElement("Hyphenator");
	rde.setAttribute("Automatic", static_cast<int>(appPrefs.hyphPrefs.Automatic));
	rde.setAttribute("AutomaticCheck", static_cast<int>(appPrefs.hyphPrefs.AutoCheck));
	for (auto hyit = appPrefs.hyphPrefs.specialWords.begin(); hyit != appPrefs.hyphPrefs.specialWords.end(); ++hyit)
	{
		QDomElement hyElem = docu.createElement("Exception");
		hyElem.setAttribute("Word", hyit.key());
		hyElem.setAttribute("Hyphenated", hyit.value());
		rde.appendChild(hyElem);
	}
	for (const auto& hyWord : appPrefs.hyphPrefs.ignoredWords)
	{
		QDomElement hyElem2 = docu.createElement("Ignore");
		hyElem2.setAttribute("Word", hyWord);
		rde.appendChild(hyElem2);
	}
	elem.appendChild(rde);

	for (int i = 0; i < appPrefs.uiPrefs.RecentDocs.count(); ++i)
	{
		QDomElement rcElem = docu.createElement("Recent");
		rcElem.setAttribute("Name", appPrefs.uiPrefs.RecentDocs[i]);
		elem.appendChild(rcElem);
	}

	for (auto ksc = appPrefs.keyShortcutPrefs.KeyActions.begin(); ksc != appPrefs.keyShortcutPrefs.KeyActions.end(); ++ksc)
	{
		if (ksc.value().actionName.isEmpty())
			continue;
		QDomElement kscc = docu.createElement("Shortcut");
		kscc.setAttribute("Action",ksc.value().actionName);
		kscc.setAttribute("KeySequence", Prefs_KeyboardShortcuts::getKeyText(ksc.value().keySequence));
		elem.appendChild(kscc);
	}

	QDomElement cosd = docu.createElement("DefaultColorSet");
	cosd.setAttribute("Name", appPrefs.colorPrefs.DColorSet);
	elem.appendChild(cosd);

	QDomElement pdf = docu.createElement("PDF");
	pdf.setAttribute("Thumbnails", static_cast<int>(appPrefs.pdfPrefs.Thumbnails));
	pdf.setAttribute("Articles", static_cast<int>(appPrefs.pdfPrefs.Articles));
	pdf.setAttribute("Bookmarks", static_cast<int>(appPrefs.pdfPrefs.Bookmarks));
	pdf.setAttribute("Compress", static_cast<int>(appPrefs.pdfPrefs.Compress));
	pdf.setAttribute("CompressionMethod", appPrefs.pdfPrefs.CompressMethod);
	pdf.setAttribute("Quality", appPrefs.pdfPrefs.Quality);
	pdf.setAttribute("EmbedPDF", static_cast<int>(appPrefs.pdfPrefs.embedPDF));
	pdf.setAttribute("MirrorPagesHorizontal", static_cast<int>(appPrefs.pdfPrefs.MirrorH));
	pdf.setAttribute("MirrorPagesVertical", static_cast<int>(appPrefs.pdfPrefs.MirrorV));
	pdf.setAttribute("DoClip", static_cast<int>(appPrefs.pdfPrefs.doClip));
	pdf.setAttribute("RotateDeg", appPrefs.pdfPrefs.RotateDeg);
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
	pdf.setAttribute("FontEmbedding", static_cast<int>(appPrefs.pdfPrefs.FontEmbedding));
	pdf.setAttribute("Intent", appPrefs.pdfPrefs.Intent);
	pdf.setAttribute("Intent2", appPrefs.pdfPrefs.Intent2);
	pdf.setAttribute("SolidProfile", appPrefs.pdfPrefs.SolidProf);
	pdf.setAttribute("ImageProfile", appPrefs.pdfPrefs.ImageProf);
	pdf.setAttribute("PrintProfile", appPrefs.pdfPrefs.PrintProf);
	pdf.setAttribute("InfoString", appPrefs.pdfPrefs.Info);
	pdf.setAttribute("BleedTop", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.top()));
	pdf.setAttribute("BleedLeft", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.left()));
	pdf.setAttribute("BleedRight", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.right()));
	pdf.setAttribute("BleedBottom", ScCLocale::toQStringC(appPrefs.pdfPrefs.bleeds.bottom()));
	pdf.setAttribute("UseDocBleeds", static_cast<int>(appPrefs.pdfPrefs.useDocBleeds));
	pdf.setAttribute("CropMarks", static_cast<int>(appPrefs.pdfPrefs.cropMarks));
	pdf.setAttribute("BleedMarks", static_cast<int>(appPrefs.pdfPrefs.bleedMarks));
	pdf.setAttribute("RegistrationMarks", static_cast<int>(appPrefs.pdfPrefs.registrationMarks));
	pdf.setAttribute("ColorMarks", static_cast<int>(appPrefs.pdfPrefs.colorMarks));
	pdf.setAttribute("DocInfoMarks", static_cast<int>(appPrefs.pdfPrefs.docInfoMarks));
	pdf.setAttribute("MarkLength", appPrefs.pdfPrefs.markLength);
	pdf.setAttribute("MarkOffset", appPrefs.pdfPrefs.markOffset);
	pdf.setAttribute("ImagePr", static_cast<int>(appPrefs.pdfPrefs.EmbeddedI));
	pdf.setAttribute("PassOwner", appPrefs.pdfPrefs.PassOwner);
	pdf.setAttribute("PassUser", appPrefs.pdfPrefs.PassUser);
	pdf.setAttribute("Permissions", appPrefs.pdfPrefs.Permissions);
	pdf.setAttribute("Encrypt", static_cast<int>(appPrefs.pdfPrefs.Encrypt));
	pdf.setAttribute("UseLayers", static_cast<int>(appPrefs.pdfPrefs.useLayers));
	pdf.setAttribute("UseLpi", static_cast<int>(appPrefs.pdfPrefs.UseLPI));
	pdf.setAttribute("UseSpotColors", static_cast<int>(appPrefs.pdfPrefs.UseSpotColors));
	pdf.setAttribute("DoMultiFile", static_cast<int>(appPrefs.pdfPrefs.doMultiFile));
	pdf.setAttribute("DisplayBookmarks", static_cast<int>(appPrefs.pdfPrefs.displayBookmarks));
	pdf.setAttribute("DisplayFullscreen", static_cast<int>(appPrefs.pdfPrefs.displayFullscreen));
	pdf.setAttribute("DisplayLayers", static_cast<int>(appPrefs.pdfPrefs.displayLayers));
	pdf.setAttribute("DisplayThumbs", static_cast<int>(appPrefs.pdfPrefs.displayThumbs));
	pdf.setAttribute("hideMenuBar", static_cast<int>(appPrefs.pdfPrefs.hideMenuBar));
	pdf.setAttribute("hideToolBar", static_cast<int>(appPrefs.pdfPrefs.hideToolBar));
	pdf.setAttribute("fitWindow", static_cast<int>(appPrefs.pdfPrefs.fitWindow));
	pdf.setAttribute("openAfterExport", static_cast<int>(appPrefs.pdfPrefs.openAfterExport));
	pdf.setAttribute("PageLayout", appPrefs.pdfPrefs.PageLayout);
	pdf.setAttribute("OpenAction", appPrefs.pdfPrefs.openAction);
	for (auto itlp = appPrefs.pdfPrefs.LPISettings.begin(); itlp != appPrefs.pdfPrefs.LPISettings.end(); ++itlp)
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
	for (const auto& objAttr : appPrefs.itemAttrPrefs.defaultItemAttributes)
	{
		QDomElement itemAttr = docu.createElement("ItemAttribute");
		itemAttr.setAttribute("Name", objAttr.name);
		itemAttr.setAttribute("Type", objAttr.type);
		itemAttr.setAttribute("Value", objAttr.value);
		itemAttr.setAttribute("Parameter", objAttr.parameter);
		itemAttr.setAttribute("Relationship", objAttr.relationship);
		itemAttr.setAttribute("RelationshipTo", objAttr.relationshipto);
		itemAttr.setAttribute("AutoAddTo", objAttr.autoaddto);
		docItemAttrs.appendChild(itemAttr);
	}
	elem.appendChild(docItemAttrs);

	QDomElement tocElem = docu.createElement("TablesOfContents");
	for (const auto& tocSetup : appPrefs.tocPrefs.defaultToCSetups)
	{
		QDomElement tocsetup = docu.createElement("TableOfContents");
		tocsetup.setAttribute("Name", tocSetup.name);
		tocsetup.setAttribute("ItemAttributeName", tocSetup.itemAttrName);
		tocsetup.setAttribute("FrameName", tocSetup.frameName);
		tocsetup.setAttribute("ListNonPrinting", tocSetup.listNonPrintingFrames);
		tocsetup.setAttribute("Style", tocSetup.textStyle);
		tocsetup.setAttribute("NumberPlacement", tocSetup.pageLocation);
		tocElem.appendChild(tocsetup);
	}
	elem.appendChild(tocElem);

	// image cache
	QDomElement icElem = docu.createElement("ImageCache");
	icElem.setAttribute("Enabled", appPrefs.imageCachePrefs.cacheEnabled);
	icElem.setAttribute("MaximumCacheSizeMiB", appPrefs.imageCachePrefs.maxCacheSizeMiB);
	icElem.setAttribute("MaximumCacheEntries", appPrefs.imageCachePrefs.maxCacheEntries);
	icElem.setAttribute("CompressionLevel", appPrefs.imageCachePrefs.compressionLevel);
	elem.appendChild(icElem);

	// active page sizes
	QDomElement apsElem = docu.createElement("ActivePageSizes");
	apsElem.setAttribute("Names", appPrefs.activePageSizes.join(","));
	elem.appendChild(apsElem);

	// experimental features
	QDomElement experimentalElem = docu.createElement("ExperimentalFeatures");
	experimentalElem.setAttribute("NotesEnabled", appPrefs.experimentalFeaturePrefs.notesEnabled);
	elem.appendChild(experimentalElem);

	// write file
	bool result = false;
	QFile f(filePath);
	if (!f.open(QIODevice::WriteOnly))
	{
		m_lastError = tr("Could not open preferences file \"%1\" for writing: %2").arg(filePath, QApplication::translate("QFile", f.errorString().toLatin1().constData()));
	}
	else
	{
		QTextStream s(&f);
		s.setCodec("UTF-8");
		s<<docu.toString();
		if (f.error() == QFile::NoError)
			result = true;
		else
			m_lastError = tr("Writing to preferences file \"%1\" failed: QIODevice status code %2").arg(filePath, f.errorString());
	}
	if (f.isOpen())
		f.close();
	return result;
}

bool PrefsManager::readPref(const QString& filePath)
{
	QDomDocument docu("scridoc");
	QFile f(filePath);
	if (!f.open(QIODevice::ReadOnly))
	{
		m_lastError = tr("Failed to open prefs file \"%1\": %2").arg(filePath, QApplication::translate("QFile",f.errorString().toLatin1().constData()) );
		return false;
	}
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	QString errorMsg;
	int errorLine = 0;
	int errorColumn = 0;
	if (!docu.setContent(ts.readAll(), &errorMsg, &errorLine, &errorColumn))
	{
		m_lastError = tr("Failed to read prefs XML from \"%1\": %2 at line %3, col %4").arg(filePath, errorMsg).arg(errorLine).arg(errorColumn);
		f.close();
		return false;
	}
	f.close();

	ScDomElement elem = docu.documentElement();
	if (elem.tagName() != "SCRIBUSRC")
		return false;
	//Ignore scribus*.rc files prior to 1.5.0 due to changes
	bool prefs150FileFound = false;
	if (elem.hasAttribute("VERSION"))
	{
		if (elem.attribute("VERSION") == "1.5.0")
			prefs150FileFound = true;
	}
	m_firstTimeIgnoreOldPrefs = !prefs150FileFound;
	if (!prefs150FileFound)
		return false;
	appPrefs.colorPrefs.DColors.clear();
	ColorSetManager csm;
	csm.initialiseDefaultPrefs(appPrefs);
	csm.findPaletteLocations();
	csm.findPalettes();
	csm.findUserPalettes();
	QDomNode DOC = elem.firstChild();
	if (!DOC.namedItem("CheckProfile").isNull())
		appPrefs.verifierPrefs.checkerPrefsList.clear();
	while (!DOC.isNull())
	{
		ScDomElement dc = DOC.toElement();

		if (dc.tagName() == "UI")
		{
			appPrefs.uiPrefs.style = dc.attribute("Theme", "Default");
			appPrefs.uiPrefs.wheelJump = dc.attribute("ScrollWheelJump").toInt();
			appPrefs.uiPrefs.mouseMoveTimeout = dc.attribute("MouseMoveTimeout", "150").toInt();
			appPrefs.uiPrefs.applicationFontSize = dc.attribute("ApplicationFontSize", "12").toInt();
			appPrefs.uiPrefs.paletteFontSize = dc.attribute("PaletteFontSize", "10").toInt();
			appPrefs.uiPrefs.recentDocCount = dc.attribute("RecentDocumentCount", "5").toUInt();
			appPrefs.uiPrefs.showStartupDialog = static_cast<bool>(dc.attribute("ShowStartupDialog", "1").toInt());
			appPrefs.uiPrefs.showSplashOnStartup = static_cast<bool>(dc.attribute("UI_SHOWSPLASHSCREEN", "1").toInt());
			appPrefs.uiPrefs.useSmallWidgets = dc.attribute("UseSmallWidgets").toInt();
			appPrefs.uiPrefs.useTabs = static_cast<bool>(dc.attribute("UseDocumentTabs", "0").toInt());
			appPrefs.uiPrefs.stickyTools = static_cast<bool>(dc.attribute("StickyTools", "0").toInt());
			appPrefs.uiPrefs.grayscaleIcons = static_cast<bool>(dc.attribute("UseGrayscaleIcons",nullptr).toInt());
			appPrefs.uiPrefs.iconSet = dc.attribute("IconSet", "1_5_0");
			appPrefs.uiPrefs.userPreferredLocale = dc.attribute("UserPreferredLocale", "System");
		}

		if (dc.tagName() == "DocumentSetup")
		{
			appPrefs.docSetupPrefs.language = dc.attribute("Language", "en_GB");
			appPrefs.docSetupPrefs.language = LanguageManager::instance()->getShortAbbrevFromAbbrevDecomposition(appPrefs.docSetupPrefs.language);
			if (appPrefs.docSetupPrefs.language.isEmpty())
				appPrefs.docSetupPrefs.language = "en_GB";
			appPrefs.docSetupPrefs.docUnitIndex = dc.attribute("UnitIndex", "0").toInt();
			appPrefs.docSetupPrefs.pageSize = dc.attribute("PageSize", "A4");
			appPrefs.docSetupPrefs.pageOrientation = dc.attribute("PageOrientation", "0").toInt();
			appPrefs.docSetupPrefs.pageWidth   = ScCLocale::toDoubleC(dc.attribute("PageWidth"), 595.0);
			appPrefs.docSetupPrefs.pageHeight  = ScCLocale::toDoubleC(dc.attribute("PageHeight"), 842.0);
			appPrefs.docSetupPrefs.margins.setTop(ScCLocale::toDoubleC(dc.attribute("MarginTop"), 9.0));
			appPrefs.docSetupPrefs.margins.setBottom(ScCLocale::toDoubleC(dc.attribute("MarginBottom"), 40.0));
			appPrefs.docSetupPrefs.margins.setLeft(ScCLocale::toDoubleC(dc.attribute("MarginLeft"), 9.0));
			appPrefs.docSetupPrefs.margins.setRight(ScCLocale::toDoubleC(dc.attribute("MarginRight"), 9.0));
			appPrefs.docSetupPrefs.marginPreset   = dc.attribute("MarginPreset", "0").toInt();
			appPrefs.docSetupPrefs.pagePositioning	= dc.attribute("PagePositioning", "0").toInt();
			appPrefs.docSetupPrefs.AutoSave	  = static_cast<bool>(dc.attribute("AutoSave", "0").toInt());
			appPrefs.docSetupPrefs.AutoSaveTime  = dc.attribute("AutoSaveTime", "600000").toInt();
			appPrefs.docSetupPrefs.AutoSaveCount  = dc.attribute("AutoSaveCount", "1").toInt();
			appPrefs.docSetupPrefs.AutoSaveKeep = static_cast<bool>(dc.attribute("AutoSaveKeep", "0").toInt());
			appPrefs.docSetupPrefs.AutoSaveLocation = static_cast<bool>(dc.attribute("AutoSaveLoc", "1").toInt());
			appPrefs.docSetupPrefs.AutoSaveDir = dc.attribute("AutoSaveDir", "");
			appPrefs.docSetupPrefs.saveCompressed = static_cast<bool>(dc.attribute("SaveCompressed", "0").toInt());
			appPrefs.docSetupPrefs.bleeds.setTop(ScCLocale::toDoubleC(dc.attribute("BleedTop"), 0.0));
			appPrefs.docSetupPrefs.bleeds.setLeft(ScCLocale::toDoubleC(dc.attribute("BleedLeft"), 0.0));
			appPrefs.docSetupPrefs.bleeds.setRight(ScCLocale::toDoubleC(dc.attribute("BleedRight"), 0.0));
			appPrefs.docSetupPrefs.bleeds.setBottom(ScCLocale::toDoubleC(dc.attribute("BleedBottom"), 0.0));

		}

		if (dc.tagName() == "Miscellaneous")
		{
			appPrefs.miscPrefs.haveStylePreview = static_cast<bool>(dc.attribute("ShowStylePreview", "1").toInt());
			appPrefs.miscPrefs.useStandardLI = static_cast<bool>(dc.attribute("LoremIpsumUseStandard", "0").toInt());
			appPrefs.miscPrefs.paragraphsLI = dc.attribute("LoremIpsumParagraphs", "10").toInt();
			appPrefs.miscPrefs.saveEmergencyFile = static_cast<bool>(dc.attribute("saveEmergencyFile", "1").toInt());
		}

		if (dc.tagName() == "Display")
		{
			appPrefs.displayPrefs.scratch.setBottom(ScCLocale::toDoubleC(dc.attribute("ScratchBottom"), 20.0));
			appPrefs.displayPrefs.scratch.setLeft(ScCLocale::toDoubleC(dc.attribute("ScratchLeft"), 100.0));
			appPrefs.displayPrefs.scratch.setRight(ScCLocale::toDoubleC(dc.attribute("ScratchRight"), 100.0));
			appPrefs.displayPrefs.scratch.setTop(ScCLocale::toDoubleC(dc.attribute("ScratchTop"), 20.0));
			appPrefs.displayPrefs.pageGapHorizontal  = ScCLocale::toDoubleC(dc.attribute("PageGapHorizontal"), 0.0);
			appPrefs.displayPrefs.pageGapVertical	= ScCLocale::toDoubleC(dc.attribute("PageGapVertical"), 40.0);
			appPrefs.displayPrefs.showPageShadow = static_cast<bool>(dc.attribute("ShowPageShadow", "1").toInt());
			appPrefs.displayPrefs.paperColor = QColor(dc.attribute("PageColor"));
			if (dc.hasAttribute("ScratchColor"))
				appPrefs.displayPrefs.scratchColor = QColor(dc.attribute("ScratchColor"));
			else
				appPrefs.displayPrefs.scratchColor = QApplication::palette().color(QPalette::Active, QPalette::Window);
			appPrefs.displayPrefs.frameColor = QColor(dc.attribute("FrameSelectedColor", "#ff0000"));
			appPrefs.displayPrefs.frameNormColor = QColor(dc.attribute("FrameNormColor", "#000000"));
			appPrefs.displayPrefs.frameGroupColor = QColor(dc.attribute("FrameGroupColor", "#008080"));
			appPrefs.displayPrefs.frameLockColor = QColor(dc.attribute("FrameLockColor", "#800000"));
			appPrefs.displayPrefs.frameLinkColor = QColor(dc.attribute("FrameLinkColor", "#ff0000"));
			appPrefs.displayPrefs.frameAnnotationColor = QColor(dc.attribute("FrameAnnotationColor", "#0000ff"));
			appPrefs.displayPrefs.pageBorderColor = QColor(dc.attribute("PageBorderColor", "#ff0000"));
			appPrefs.displayPrefs.controlCharColor = QColor(dc.attribute("ControlCharColor", "#800000"));
			appPrefs.displayPrefs.marginColored = static_cast<bool>(dc.attribute("ShowMarginsFilled", "0").toInt());
			appPrefs.displayPrefs.displayScale = qRound(ScCLocale::toDoubleC(dc.attribute("DisplayScale"), appPrefs.displayPrefs.displayScale)*72)/72.0;
			appPrefs.displayPrefs.showVerifierWarningsOnCanvas = static_cast<bool>(dc.attribute("ShowVerifierWarningsOnCanvas", "1").toInt());
			appPrefs.displayPrefs.showAutosaveClockOnCanvas = static_cast<bool>(dc.attribute("ShowAutosaveClockOnCanvas", "0").toInt());
			appPrefs.displayPrefs.showToolTips = static_cast<bool>(dc.attribute("ToolTips", "1").toInt());
			appPrefs.displayPrefs.showMouseCoordinates = static_cast<bool>(dc.attribute("ShowMouseCoordinates", "1").toInt());
		}
		if (dc.tagName() == "Paths")
		{
			appPrefs.pathPrefs.documents = dc.attribute("Documents", "");
			appPrefs.pathPrefs.colorProfiles = dc.attribute("Profiles", "");
			appPrefs.pathPrefs.scripts = dc.attribute("Scripts", "");
			appPrefs.pathPrefs.documentTemplates = dc.attribute("Templates", "");
		}
		if (dc.tagName() == "Guides")
		{
			appPrefs.guidesPrefs.grabRadius = dc.attribute("GrabRadius", "4").toInt();
			appPrefs.guidesPrefs.guidesShown = static_cast<bool>(dc.attribute("ShowGuides", "1").toInt());
			appPrefs.guidesPrefs.colBordersShown = static_cast<bool>(dc.attribute("ShowColumnBorders", "0").toInt());
			appPrefs.guidesPrefs.framesShown = static_cast<bool>(dc.attribute("ShowFrames", "1").toInt());
			appPrefs.guidesPrefs.layerMarkersShown = static_cast<bool>(dc.attribute("ShowLayerMarkers", "0").toInt());
			appPrefs.guidesPrefs.marginsShown = static_cast<bool>(dc.attribute("ShowMargins", "1").toInt());
			appPrefs.guidesPrefs.baselineGridShown = static_cast<bool>(dc.attribute("ShowBaselineGrid", "1").toInt());
			appPrefs.guidesPrefs.linkShown = static_cast<bool>(dc.attribute("ShowLinks", "0").toInt());
			appPrefs.guidesPrefs.showPic = static_cast<bool>(dc.attribute("ShowImages", "1").toInt());
			appPrefs.guidesPrefs.showControls = static_cast<bool>(dc.attribute("ShowControls", "0").toInt());
			appPrefs.guidesPrefs.rulersShown = static_cast<bool>(dc.attribute("ShowRulers", "1").toInt());
			appPrefs.guidesPrefs.showBleed = static_cast<bool>(dc.attribute("ShowBleed", "1").toInt());
			appPrefs.guidesPrefs.rulerMode = static_cast<bool>(dc.attribute("RulerMode", "1").toInt());
			appPrefs.guidesPrefs.minorGridSpacing  = ScCLocale::toDoubleC(dc.attribute("MinorGridSpacing"), 20.0);
			appPrefs.guidesPrefs.majorGridSpacing  = ScCLocale::toDoubleC(dc.attribute("MajorGridSpacing"), 100.0);
			appPrefs.guidesPrefs.minorGridColor = QColor(dc.attribute("MinorGridColor"));
			appPrefs.guidesPrefs.majorGridColor = QColor(dc.attribute("MajorGridColor"));
			if (dc.hasAttribute("GuidePlacement"))
			{
				appPrefs.guidesPrefs.renderStackOrder.clear();
				if (static_cast<bool>(dc.attribute("GuidePlacement", "1").toInt()))
					appPrefs.guidesPrefs.renderStackOrder << 0 << 1 << 2 << 3 << 4;
				else
					appPrefs.guidesPrefs.renderStackOrder << 4 << 0 << 1 << 2 << 3;
			}
			if (dc.hasAttribute("renderStack"))
			{
				appPrefs.guidesPrefs.renderStackOrder.clear();
				QString renderStack = dc.attribute("renderStack", "2 0 4 1 3");
				ScTextStream fp(&renderStack, QIODevice::ReadOnly);
				QString val;
				while (!fp.atEnd())
				{
					fp >> val;
					appPrefs.guidesPrefs.renderStackOrder << val.toInt();
				}
			}
			appPrefs.guidesPrefs.gridType = dc.attribute("GridType", "0").toInt();
			appPrefs.guidesPrefs.gridShown = static_cast<bool>(dc.attribute("ShowGrid", "0").toInt());
			if (dc.hasAttribute("GuideC"))
				appPrefs.guidesPrefs.guideColor = QColor(dc.attribute("GuideC"));
			if (dc.hasAttribute("GuideZ"))
				appPrefs.guidesPrefs.guideRad = ScCLocale::toDoubleC(dc.attribute("ObjectToGuideSnapRadius"), 10.0);
			if (dc.hasAttribute("BaseC"))
				appPrefs.guidesPrefs.baselineGridColor = QColor(dc.attribute("BaselineGridColor"));
			appPrefs.guidesPrefs.marginColor = QColor(dc.attribute("MarginColor", "#0000ff"));
			appPrefs.guidesPrefs.valueBaselineGrid   = ScCLocale::toDoubleC(dc.attribute("BaselineGridDistance"), 12.0);
			appPrefs.guidesPrefs.offsetBaselineGrid  = ScCLocale::toDoubleC(dc.attribute("BaselineGridOffset"), 0.0);
		}
		if (dc.tagName() == "StoryEditor")
		{
			appPrefs.storyEditorPrefs.guiFont  = dc.attribute("Font", "");
			if (dc.hasAttribute("FontColorBackground"))
			{
				QString colorName = dc.attribute("FontColorBackground");
				if (QColor::isValidColor(colorName))
					appPrefs.storyEditorPrefs.guiFontColorBackground  = QColor(colorName);
			}
			appPrefs.storyEditorPrefs.smartTextSelection = static_cast<bool>(dc.attribute("SmartTextSelection", "0").toInt());
		}
		if (dc.tagName() == "Typography")
		{
			appPrefs.typoPrefs.valueSuperScript = dc.attribute("SuperScriptDistance").toInt();
			appPrefs.typoPrefs.scalingSuperScript = dc.attribute("SuperScriptScaling").toInt();
			appPrefs.typoPrefs.valueSubScript = dc.attribute("SubScriptDistance").toInt();
			appPrefs.typoPrefs.scalingSubScript = dc.attribute("SubScriptScaling").toInt();
			appPrefs.typoPrefs.valueSmallCaps  = dc.attribute("SmallCapsScaling").toInt();
			appPrefs.typoPrefs.autoLineSpacing = dc.valueAsInt("AutomaticLineSpacing", 1, 500, 100);
			double ulp = ScCLocale::toDoubleC(dc.attribute("UnderlineDistance"), -1.0);
			if (ulp != -1)
				appPrefs.typoPrefs.valueUnderlinePos = qRound(ulp * 10);
			else
				appPrefs.typoPrefs.valueUnderlinePos = -1;
			double ulw = ScCLocale::toDoubleC(dc.attribute("UnderlineWidth"), -1.0);
			if (ulw != -1)
				appPrefs.typoPrefs.valueUnderlineWidth = qRound(ulw * 10);
			else
				appPrefs.typoPrefs.valueUnderlineWidth = -1;
			double stp = ScCLocale::toDoubleC(dc.attribute("StrikeThruDistance"), -1.0);
			if (stp != -1)
				appPrefs.typoPrefs.valueStrikeThruPos = qRound(stp * 10);
			else
				appPrefs.typoPrefs.valueStrikeThruPos = -1;
			double stw = ScCLocale::toDoubleC(dc.attribute("StrikeThruWidth"), -1.0);
			if (stw != -1)
				appPrefs.typoPrefs.valueStrikeThruWidth = qRound(stw * 10);
			else
				appPrefs.typoPrefs.valueStrikeThruWidth = -1;
		}

		if (dc.tagName() == "ItemTools")
		{
			appPrefs.itemToolPrefs.calligraphicPenFillColor = dc.attribute("CalligraphicPenFillColor", "Black");
			appPrefs.itemToolPrefs.calligraphicPenLineColor = dc.attribute("CalligraphicPenLineColor", "Black");
			appPrefs.itemToolPrefs.calligraphicPenFillColorShade = dc.attribute("CalligraphicPenFillColorShade", "100").toInt();
			appPrefs.itemToolPrefs.calligraphicPenLineColorShade = dc.attribute("CalligraphicPenLineColorShade", "100").toInt();
			appPrefs.itemToolPrefs.calligraphicPenLineWidth = ScCLocale::toDoubleC(dc.attribute("CalligraphicPenLineWidth"), 1.0);
			appPrefs.itemToolPrefs.calligraphicPenAngle = ScCLocale::toDoubleC(dc.attribute("CalligraphicPenAngle"), 0.0);
			appPrefs.itemToolPrefs.calligraphicPenWidth = ScCLocale::toDoubleC(dc.attribute("CalligraphicPenWidth"), 10.0);
			appPrefs.itemToolPrefs.calligraphicPenStyle = dc.attribute("CalligraphicPenStyle").toInt();
			appPrefs.itemToolPrefs.shapeLineColor = dc.attribute("ShapeLineColor");
			appPrefs.itemToolPrefs.shapeFillColor = dc.attribute("ShapeFillColor", CommonStrings::None);
			appPrefs.itemToolPrefs.lineColor = dc.attribute("LineColor");
			appPrefs.itemToolPrefs.textColor = dc.attribute("TextColor");
			appPrefs.itemToolPrefs.textStrokeColor = dc.attribute("TextStrokeColor", appPrefs.itemToolPrefs.textColor);
			appPrefs.itemToolPrefs.textFillColor = dc.attribute("TextBackgroundColor", CommonStrings::None);
			appPrefs.itemToolPrefs.textLineColor = dc.attribute("TextLineColor", CommonStrings::None);
			appPrefs.itemToolPrefs.textFillColorShade = dc.attribute("TextBackgroundColorShade", "100").toInt();
			appPrefs.itemToolPrefs.textLineColorShade = dc.attribute("TextLineColorShade", "100").toInt();
			appPrefs.itemToolPrefs.textShade = dc.attribute("TextColorShade", "100").toInt();
			appPrefs.itemToolPrefs.textStrokeShade = dc.attribute("TextStrokeColorShade", "100").toInt();
			appPrefs.itemToolPrefs.textColumns = dc.attribute("TextColumnCount", "1").toInt();
			appPrefs.itemToolPrefs.textColumnGap  = ScCLocale::toDoubleC(dc.attribute("TextColumnGap"), 0.0);
			appPrefs.itemToolPrefs.textTabFillChar = dc.attribute("TabFillCharacter", "");
			appPrefs.itemToolPrefs.textTabWidth   = ScCLocale::toDoubleC(dc.attribute("TextTabWidth"), 36.0);
			appPrefs.itemToolPrefs.textDistances.setTop(ScCLocale::toDoubleC(dc.attribute("TextDistanceTop"), 0.0));
			appPrefs.itemToolPrefs.textDistances.setBottom(ScCLocale::toDoubleC(dc.attribute("TextDistanceBottom"), 0.0));
			appPrefs.itemToolPrefs.textDistances.setLeft(ScCLocale::toDoubleC(dc.attribute("TextDistanceLeft"), 0.0));
			appPrefs.itemToolPrefs.textDistances.setRight(ScCLocale::toDoubleC(dc.attribute("TextDistanceRight"), 0.0));
			appPrefs.itemToolPrefs.firstLineOffset = (FirstLineOffsetPolicy) dc.attribute("FirstLineOffset", "1").toInt(); // Default to FLOPFontAscent
			appPrefs.itemToolPrefs.firstLineOffset = qMax(FLOPRealGlyphHeight, qMin(appPrefs.itemToolPrefs.firstLineOffset, FLOPBaselineGrid));
			appPrefs.itemToolPrefs.shapeLineStyle = dc.attribute("ShapeLineStyle").toInt();
			appPrefs.itemToolPrefs.lineStyle = dc.attribute("LineStyle").toInt();
			appPrefs.itemToolPrefs.shapeLineWidth	 = ScCLocale::toDoubleC(dc.attribute("ShapeLineWidth"), 1.0);
			appPrefs.itemToolPrefs.lineWidth = ScCLocale::toDoubleC(dc.attribute("LineWidth"), 1.0);
			appPrefs.itemToolPrefs.shapeLineColorShade = dc.attribute("ShapeLineColorShade").toInt();
			appPrefs.itemToolPrefs.lineColorShade = dc.attribute("LineColorShade").toInt();
			appPrefs.itemToolPrefs.shapeFillColorShade  = dc.attribute("ShapeFillColorShade").toInt();
			appPrefs.itemToolPrefs.imageFillColor = dc.attribute("ImageFillColor", CommonStrings::None);
			appPrefs.itemToolPrefs.imageFillColorShade = dc.attribute("ImageFillColorShade", "100").toInt();
			appPrefs.itemToolPrefs.imageStrokeColor = dc.attribute("ImageStrokeColor", CommonStrings::None);
			appPrefs.itemToolPrefs.imageStrokeColorShade = dc.attribute("ImageStrokeColorShade", "100").toInt();
			appPrefs.itemToolPrefs.imageScaleX = ScCLocale::toDoubleC(dc.attribute("ImageScaleX"), 1.0);
			appPrefs.itemToolPrefs.imageScaleY = ScCLocale::toDoubleC(dc.attribute("ImageScaleY"), 1.0);
			appPrefs.itemToolPrefs.imageScaleType = static_cast<bool>(dc.attribute("ImageScaleType", "1").toInt());
			appPrefs.itemToolPrefs.imageAspectRatio = static_cast<bool>(dc.attribute("ImageAspectRatio", "1").toInt());
			appPrefs.itemToolPrefs.imageUseEmbeddedPath = static_cast<bool>(dc.attribute("ImageUseEmbeddedPath", "0").toInt());
			appPrefs.itemToolPrefs.imageLowResType = dc.attribute("ImageLowResType", "1").toInt();
			appPrefs.itemToolPrefs.polyCorners = dc.attribute("PolygonCorners", "4").toInt();
			appPrefs.itemToolPrefs.polyFactor = ScCLocale::toDoubleC(dc.attribute("PolygonFactor"), 0.5);
			appPrefs.itemToolPrefs.polyRotation = ScCLocale::toDoubleC(dc.attribute("PolygonRotation"), 0.0);
			appPrefs.itemToolPrefs.polyInnerRot = ScCLocale::toDoubleC(dc.attribute("PolygonInnerRotation"), 0.0);
			appPrefs.itemToolPrefs.polyCurvature = ScCLocale::toDoubleC(dc.attribute("PolygonCurvature"), 0.0);
			appPrefs.itemToolPrefs.polyOuterCurvature = ScCLocale::toDoubleC(dc.attribute("PolygonOuterCurvature"), 0.0);
			appPrefs.itemToolPrefs.polyUseFactor  = static_cast<bool>(dc.attribute("PolygonUseFactor", "0").toInt());
			appPrefs.itemToolPrefs.lineStartArrow = dc.attribute("LineStartArrow", "0").toInt();
			appPrefs.itemToolPrefs.lineEndArrow   = dc.attribute("LineEndArrow", "0").toInt();
			appPrefs.itemToolPrefs.arcStartAngle = ScCLocale::toDoubleC(dc.attribute("ArcStartAngle"), 30.0);
			appPrefs.itemToolPrefs.arcSweepAngle = ScCLocale::toDoubleC(dc.attribute("ArcSweepAngle"), 300.0);
			appPrefs.itemToolPrefs.spiralStartAngle = ScCLocale::toDoubleC(dc.attribute("SpiralStartAngle"), 0.0);
			appPrefs.itemToolPrefs.spiralEndAngle = ScCLocale::toDoubleC(dc.attribute("SpiralEndAngle"), 1080.0);
			appPrefs.itemToolPrefs.spiralFactor = ScCLocale::toDoubleC(dc.attribute("SpiralFactor"), 1.2);
			if (dc.hasAttribute("FontFace"))
			{
				QString tmpf = dc.attribute("FontFace");
				QString newFont;
				if (!appPrefs.fontPrefs.AvailFonts.contains(tmpf) || !appPrefs.fontPrefs.AvailFonts[tmpf].usable())
				{
					ScCore->showSplash(false);
					MissingFont *dia = new MissingFont(nullptr, tmpf, nullptr);
					dia->exec();
					newFont = dia->getReplacementFont();
					delete dia;
				}
				else
					newFont = tmpf;
				if (!newFont.isEmpty())
					appPrefs.itemToolPrefs.textFont = newFont;
				appPrefs.itemToolPrefs.textSize = qRound( ScCLocale::toDoubleC(dc.attribute("FontSize"), 12.0) * 10.0 );
			}
		}

		if (dc.tagName() == "OperatorTools")
		{
			appPrefs.opToolPrefs.magMin  = dc.attribute("MinimumMagnification", "1").toInt();
			appPrefs.opToolPrefs.magMax  = dc.attribute("MaximumMagnification", "32000").toInt();
			appPrefs.opToolPrefs.magStep = dc.attribute("MagnificationStep", "25").toInt();
			if (appPrefs.opToolPrefs.magStep < 0)
				appPrefs.opToolPrefs.magStep = 25;
			appPrefs.opToolPrefs.dispX = ScCLocale::toDoubleC(dc.attribute("DisplayOffsetX"), 10.0);
			appPrefs.opToolPrefs.dispY = ScCLocale::toDoubleC(dc.attribute("DisplayOffsetY"), 10.0);
			appPrefs.opToolPrefs.constrain = ScCLocale::toDoubleC(dc.attribute("RotationConstrainAngle"), 15.0);
		}

		if (dc.tagName() == "MainWindow")
		{
			appPrefs.uiPrefs.mainWinSettings.xPosition = dc.attribute("XPosition", "0").toInt();
			appPrefs.uiPrefs.mainWinSettings.yPosition = dc.attribute("YPosition", "0").toInt();
			appPrefs.uiPrefs.mainWinSettings.width = dc.attribute("Width", "640").toInt();
			appPrefs.uiPrefs.mainWinSettings.height = dc.attribute("Height", "480").toInt();
			appPrefs.uiPrefs.mainWinSettings.maximized = static_cast<bool>(dc.attribute("Maximized", "0").toInt());
			appPrefs.uiPrefs.mainWinSettings.screenNumber = dc.attribute("ScreenNumber", "0").toInt();
			int minX = 0;
			int minY = 0;
#ifdef Q_OS_MACOS
			// on Mac you're dead if the titlebar is not on screen
			minY = 22;
#endif
			if (QGuiApplication::screens().count() == 1)
			{
				if (appPrefs.uiPrefs.mainWinSettings.xPosition < minX )
					appPrefs.uiPrefs.mainWinSettings.xPosition = minX;
				if (appPrefs.uiPrefs.mainWinSettings.yPosition <  minY)
					appPrefs.uiPrefs.mainWinSettings.yPosition = minY;
			}
			int minWidth = 0;
			int minHeight = 0;
			const QScreen* s = QGuiApplication::screens().at(qMin(appPrefs.uiPrefs.mainWinSettings.screenNumber, QGuiApplication::screens().count() - 1));
			int maxWidth = s->availableSize().width();
			int maxHeight = s->availableSize().height();
			if (appPrefs.uiPrefs.mainWinSettings.width > maxWidth)
				appPrefs.uiPrefs.mainWinSettings.width = maxWidth;
			if (appPrefs.uiPrefs.mainWinSettings.width < minWidth)
				appPrefs.uiPrefs.mainWinSettings.width = minWidth;
			if (appPrefs.uiPrefs.mainWinSettings.height > maxHeight)
				appPrefs.uiPrefs.mainWinSettings.height = maxHeight;
			if (appPrefs.uiPrefs.mainWinSettings.height < minHeight)
				appPrefs.uiPrefs.mainWinSettings.height = minHeight;
			int maxX = s->availableSize().width() - minWidth;
			int maxY = s->availableSize().height() - minHeight;
			if (appPrefs.uiPrefs.mainWinSettings.xPosition >= maxX)
				appPrefs.uiPrefs.mainWinSettings.xPosition = maxX;
			if (appPrefs.uiPrefs.mainWinSettings.yPosition >= maxY)
				appPrefs.uiPrefs.mainWinSettings.yPosition = maxY;
		}

		if (dc.tagName() == "ScrapBook")
		{
			appPrefs.scrapbookPrefs.doCopyToScrapbook = static_cast<bool>(dc.attribute("CopyToScrapbook", "1").toInt());
			appPrefs.scrapbookPrefs.persistentScrapbook = static_cast<bool>(dc.attribute("PersistentScrapbook", "0").toInt());
			appPrefs.scrapbookPrefs.writePreviews = static_cast<bool>(dc.attribute("WritePreviews", "1").toInt());
			appPrefs.scrapbookPrefs.numScrapbookCopies = dc.attribute("ScrapbookCopies", "10").toInt();
			QDomNode scrp = dc.firstChild();
			while (!scrp.isNull())
			{
				QDomElement scrpElem = scrp.toElement();
				if (scrpElem.tagName() == "Recent")
				{
					QString nam = scrpElem.attribute("Name");
					QFileInfo fd(nam);
					if (fd.exists())
						appPrefs.scrapbookPrefs.RecentScrapbooks.append(nam);
				}
				scrp = scrp.nextSibling();
			}
		}

		if (dc.tagName() == "PageSets")
		{
			QList<PageSet> newPageSets;
			for (QDomNode pgs = DOC.firstChild(); !pgs.isNull(); pgs = pgs.nextSibling())
			{
				QDomElement pgsAttr = pgs.toElement();
				if (pgsAttr.tagName() != "Set")
					continue;

				struct PageSet pageS;
				pageS.Name = pgsAttr.attribute("Name");
				pageS.FirstPage = pgsAttr.attribute("FirstPage", "0").toInt();
				pageS.Rows = pgsAttr.attribute("Rows", "1").toInt();
				pageS.Columns = pgsAttr.attribute("Columns", "1").toInt();
				pageS.pageNames.clear();

				for (QDomNode pgsN = pgs.firstChild(); !pgsN.isNull(); pgsN = pgsN.nextSibling())
				{
					QDomElement pgsAttrN = pgsN.toElement();
					if (pgsAttrN.tagName() == "PageNames")
						pageS.pageNames.append(pgsAttrN.attribute("Name"));
				}

				int pagesPerSpread = pageS.Rows * pageS.Columns;
				if (pagesPerSpread < 0)
					continue;
				if ((pagesPerSpread == 1) && (pageS.pageNames.count() > 1))
					continue;
				if ((pagesPerSpread > 1) && (pagesPerSpread != pageS.pageNames.count()))
					continue;
				newPageSets.append(pageS);

				if ((newPageSets.count() == appPrefs.docSetupPrefs.pagePositioning) && ((appPrefs.displayPrefs.pageGapHorizontal < 0) && (appPrefs.displayPrefs.pageGapVertical < 0)))
				{
					appPrefs.displayPrefs.pageGapHorizontal = ScCLocale::toDoubleC(pgsAttr.attribute("GapHorizontal"), 0.0);
					appPrefs.displayPrefs.pageGapVertical   = ScCLocale::toDoubleC(pgsAttr.attribute("GapBelow"), 40.0);
				}
			}
			if (!newPageSets.isEmpty())
				appPrefs.pageSets = newPageSets;
		}

		if (dc.tagName() == "ColorManagement")
		{
			appPrefs.colorPrefs.DCMSset.SoftProofOn = static_cast<bool>(dc.attribute("SoftProofOn", "0").toInt());
			appPrefs.colorPrefs.DCMSset.SoftProofFullOn = static_cast<bool>(dc.attribute("SoftProofFullOn", "0").toInt());
			appPrefs.colorPrefs.DCMSset.CMSinUse = static_cast<bool>(dc.attribute("ColorManagementActive", "0").toInt());
			appPrefs.colorPrefs.DCMSset.GamutCheck = static_cast<bool>(dc.attribute("GamutCheck", "0").toInt());
			appPrefs.colorPrefs.DCMSset.BlackPoint = static_cast<bool>(dc.attribute("BlackPoint", "1").toInt());
			appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile = dc.attribute("DefaultMonitorProfile", "");
			appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile = dc.attribute("DefaultPrinterProfile", "");
			appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile = dc.attribute("DefaultImageRGBProfile", "");
			appPrefs.colorPrefs.DCMSset.DefaultImageCMYKProfile = dc.attribute("DefaultImageCMYKProfile", "");
			appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile = dc.attribute("DefaultSolidColorRGBProfile", "");
			appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile = dc.attribute("DefaultSolorColorCMYKProfile", "");
			appPrefs.colorPrefs.DCMSset.DefaultIntentColors = (eRenderIntent) dc.attribute("DefaultIntentColors", "1").toInt();
			appPrefs.colorPrefs.DCMSset.DefaultIntentImages = (eRenderIntent) dc.attribute("DefaultIntentImages", "0").toInt();
		}
		if (dc.tagName() == "Shortcut")
		{
			appPrefs.keyShortcutPrefs.KeyActions[dc.attribute("Action")].actionName = dc.attribute("Action");
			QKeySequence newKeySequence(dc.attribute("KeySequence"));
			appPrefs.keyShortcutPrefs.KeyActions[dc.attribute("Action")].keySequence = newKeySequence;
		}
		if (dc.tagName() == "Recent")
			appPrefs.uiPrefs.RecentDocs.append(dc.attribute("Name"));
		if (dc.tagName() == "PreflightVerifier")
		{
			appPrefs.verifierPrefs.curCheckProfile = dc.attribute("CurrentProfile", CommonStrings::PDF_1_4);
			appPrefs.verifierPrefs.showPagesWithoutErrors = static_cast<bool>(dc.attribute("ShowPagesWithoutErrors", "0").toInt());
			appPrefs.verifierPrefs.showNonPrintingLayerErrors = static_cast<bool>(dc.attribute("ShowNonPrintingLayerErrors", "0").toInt());
			//#2516 work around old values until people won't have them anymore, not that these
			//translated strings should be going into prefs anyway!
			if ((appPrefs.verifierPrefs.curCheckProfile == tr("PostScript")) ||
				((appPrefs.verifierPrefs.curCheckProfile == tr("Postscript")) ||
				(appPrefs.verifierPrefs.curCheckProfile == "Postscript")))
			{
				appPrefs.verifierPrefs.curCheckProfile = CommonStrings::PostScript;
			}
		}
		if (dc.tagName() == "VerifierProfile")
		{
			QString name = dc.attribute("Name");
			if ((name == tr("PostScript")) ||  (name == tr("Postscript")) || (name == "Postscript"))
				name = CommonStrings::PostScript;
			struct CheckerPrefs checkerSettings;
			checkerSettings.ignoreErrors = static_cast<bool>(dc.attribute("IgnoreErrors", "0").toInt());
			checkerSettings.autoCheck = static_cast<bool>(dc.attribute("AutoCheck", "1").toInt());
			checkerSettings.checkGlyphs = static_cast<bool>(dc.attribute("CheckGlyphs", "1").toInt());
			checkerSettings.checkOrphans = static_cast<bool>(dc.attribute("CheckOrphans", "1").toInt());
			checkerSettings.checkOverflow = static_cast<bool>(dc.attribute("CheckOverflow", "1").toInt());
			checkerSettings.checkPictures = static_cast<bool>(dc.attribute("CheckPictures", "1").toInt());
			checkerSettings.checkResolution = static_cast<bool>(dc.attribute("CheckResolution", "1").toInt());
			checkerSettings.checkPartFilledImageFrames = static_cast<bool>(dc.attribute("CheckPartFilledImageFrames", "0").toInt());
			checkerSettings.checkTransparency = static_cast<bool>(dc.attribute("CheckTransparency", "1").toInt());
			checkerSettings.minResolution = ScCLocale::toDoubleC(dc.attribute("MinimumResolution"), 144.0);
			checkerSettings.maxResolution = ScCLocale::toDoubleC(dc.attribute("MaximumResolution"), 4800.0);
			checkerSettings.checkAnnotations = static_cast<bool>(dc.attribute("CheckAnnotations", "0").toInt());
			checkerSettings.checkRasterPDF = static_cast<bool>(dc.attribute("CheckRasterPDF", "1").toInt());
			checkerSettings.checkForGIF = static_cast<bool>(dc.attribute("CheckForGIF", "1").toInt());
			checkerSettings.ignoreOffLayers = static_cast<bool>(dc.attribute("IgnoreOffLayers", "0").toInt());
			checkerSettings.checkOffConflictLayers = static_cast<bool>(dc.attribute("CheckOffConflictLayers", "0").toInt());
			checkerSettings.checkNotCMYKOrSpot = static_cast<bool>(dc.attribute("CheckNotCMYKOrSpot", "0").toInt());
			checkerSettings.checkDeviceColorsAndOutputIntent = static_cast<bool>(dc.attribute("CheckDeviceColorsAndOutputIntent", "0").toInt());
			checkerSettings.checkFontNotEmbedded = static_cast<bool>(dc.attribute("CheckFontNotEmbedded", "0").toInt());
			checkerSettings.checkFontIsOpenType = static_cast<bool>(dc.attribute("CheckFontIsOpenType", "0").toInt());
			checkerSettings.checkAppliedMasterDifferentSide = static_cast<bool>(dc.attribute("CheckAppliedMasterDifferentSide", "1").toInt());
			checkerSettings.checkEmptyTextFrames = static_cast<bool>(dc.attribute("CheckEmptyTextFrames", "1").toInt());
			appPrefs.verifierPrefs.checkerPrefsList[name] = checkerSettings;
		}
		if (dc.tagName() == "Printer")
		{
			appPrefs.printerPrefs.PrinterName = dc.attribute("Name");
			appPrefs.printerPrefs.PrinterFile = dc.attribute("File");
			appPrefs.printerPrefs.PrinterCommand = dc.attribute("Command");
			appPrefs.printerPrefs.ClipMargin = static_cast<bool>(dc.attribute("ClipToMargins", "1").toInt());
			appPrefs.printerPrefs.GCRMode = static_cast<bool>(dc.attribute("GCRMode", "1").toInt());
		}
		if (dc.tagName() == "PrintPreview")
		{
			appPrefs.printPreviewPrefs.PrPr_Mode = static_cast<bool>(dc.attribute("Mode", "0").toInt());
			appPrefs.printPreviewPrefs.PrPr_AntiAliasing = static_cast<bool>(dc.attribute("AntiAliasing", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_Transparency = static_cast<bool>(dc.attribute("Transparency", "0").toInt());
			appPrefs.printPreviewPrefs.PrPr_C = static_cast<bool>(dc.attribute("Cyan", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_M = static_cast<bool>(dc.attribute("Magenta", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_Y = static_cast<bool>(dc.attribute("Yellow", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_K = static_cast<bool>(dc.attribute("Black", "1").toInt());
			appPrefs.printPreviewPrefs.PrPr_InkCoverage = static_cast<bool>(dc.attribute("InkCoverage", "0").toInt());
			appPrefs.printPreviewPrefs.PrPr_InkThreshold = dc.attribute("InkThreshold", "250").toInt();
		}
		if (dc.tagName() == "PDFOutputPreview")
		{
			appPrefs.pdfOutputPreviewPrefs.enableAntiAliasing = static_cast<bool>(dc.attribute("AntiAliasing", "1").toInt());
			appPrefs.pdfOutputPreviewPrefs.showTransparency = static_cast<bool>(dc.attribute("Transparency", "0").toInt());
			appPrefs.pdfOutputPreviewPrefs.cmykPreviewMode = static_cast<bool>(dc.attribute("CMYKMode", "0").toInt());
			appPrefs.pdfOutputPreviewPrefs.isCyanVisible = static_cast<bool>(dc.attribute("Cyan", "1").toInt());
			appPrefs.pdfOutputPreviewPrefs.isMagentaVisible = static_cast<bool>(dc.attribute("Magenta", "1").toInt());
			appPrefs.pdfOutputPreviewPrefs.isYellowVisible = static_cast<bool>(dc.attribute("Yellow", "1").toInt());
			appPrefs.pdfOutputPreviewPrefs.isBlackVisible = static_cast<bool>(dc.attribute("Black", "1").toInt());
			appPrefs.pdfOutputPreviewPrefs.displayInkCoverage = static_cast<bool>(dc.attribute("InkCoverage", "0").toInt());
			appPrefs.pdfOutputPreviewPrefs.inkCoverageThreshold = dc.attribute("InkThreshold", "250").toInt();
		}
		if (dc.tagName() == "PSOutputPreview")
		{
			appPrefs.psOutputPreviewPrefs.psLevel = qMax(1, qMin(dc.attribute("PSLevel", "3").toInt(), 3));
			appPrefs.psOutputPreviewPrefs.enableAntiAliasing = static_cast<bool>(dc.attribute("AntiAliasing", "1").toInt());
			appPrefs.psOutputPreviewPrefs.showTransparency = static_cast<bool>(dc.attribute("Transparency", "0").toInt());
			appPrefs.psOutputPreviewPrefs.cmykPreviewMode = static_cast<bool>(dc.attribute("CMYKMode", "0").toInt());
			appPrefs.psOutputPreviewPrefs.isCyanVisible = static_cast<bool>(dc.attribute("Cyan", "1").toInt());
			appPrefs.psOutputPreviewPrefs.isMagentaVisible = static_cast<bool>(dc.attribute("Magenta", "1").toInt());
			appPrefs.psOutputPreviewPrefs.isYellowVisible = static_cast<bool>(dc.attribute("Yellow", "1").toInt());
			appPrefs.psOutputPreviewPrefs.isBlackVisible = static_cast<bool>(dc.attribute("Black", "1").toInt());
			appPrefs.psOutputPreviewPrefs.displayInkCoverage = static_cast<bool>(dc.attribute("InkCoverage", "0").toInt());
			appPrefs.psOutputPreviewPrefs.inkCoverageThreshold = dc.attribute("InkThreshold", "250").toInt();
		}
		if (dc.tagName() == "ExternalTools")
		{
			bool gsa1 = testGSAvailability(dc.attribute("Ghostscript", "gs"));
			bool gsa2 = testGSAvailability(ghostscriptExecutable());
			if (gsa1 || !gsa2)
				setGhostscriptExecutable(dc.attribute("Ghostscript", "gs"));
			appPrefs.extToolPrefs.gs_AntiAliasText = static_cast<bool>(dc.attribute("GhostscriptAntiAliasText", "0").toInt());
			appPrefs.extToolPrefs.gs_AntiAliasGraphics = static_cast<bool>(dc.attribute("GhostscriptAntiAliasGraphics", "0").toInt());
			appPrefs.extToolPrefs.gs_Resolution = dc.attribute("Resolution", "72").toInt();
			appPrefs.extToolPrefs.latexResolution = dc.attribute("LatexResolution", "72").toInt();
			appPrefs.extToolPrefs.latexForceDpi = static_cast<bool>(dc.attribute("LatexForceDpi", "1").toInt());
			appPrefs.extToolPrefs.latexStartWithEmptyFrames = static_cast<bool>(dc.attribute("LatexStartWithEmptyFrames", "0").toInt());
			setImageEditorExecutable(dc.attribute("ImageEditor", ScPaths::defaultImageEditorApp()));
			setExtBrowserExecutable(dc.attribute("WebBrowser", ""));
			setUniconvExecutable(dc.attribute("Uniconvertor", "uniconv"));
			setLatexEditorExecutable(dc.attribute("LatexEditor", ""));
			appPrefs.extToolPrefs.pdfViewerExecutable = dc.attribute("PDFViewer", "");
			QStringList configs = appPrefs.extToolPrefs.latexConfigs;
			QDomNodeList configNodes = dc.elementsByTagName("LatexConfig");
			QString latexBase = LatexConfigParser::configBase();
			for (int i = 0; i < configNodes.size(); ++i)
			{
				QString confFile = configNodes.at(i).toElement().attribute("file", "");
				QString command  = configNodes.at(i).toElement().attribute("command", "");
				bool configExists = !confFile.isEmpty() && (QFile::exists(confFile) || QFile::exists(latexBase+confFile));
				if (configExists)
				{
					if (!configs.contains(confFile))
						configs.append(confFile);
					appPrefs.extToolPrefs.latexCommands[confFile] = command;
				}
			}
			if (!configs.isEmpty())
				setLatexConfigs(configs);
			else
			{
				qWarning() << tr("No valid renderframe config found. Using defaults!");
				appPrefs.extToolPrefs.latexConfigs = LatexConfigCache::defaultConfigs();
				appPrefs.extToolPrefs.latexCommands = LatexConfigCache::defaultCommands();
			}
		}
		if (dc.tagName() == "Hyphenator")
		{
			appPrefs.hyphPrefs.Automatic = static_cast<bool>(dc.attribute("Automatic", "1").toInt());
			appPrefs.hyphPrefs.AutoCheck = static_cast<bool>(dc.attribute("AutomaticCheck", "1").toInt());
			QDomNode hyNode = dc.firstChild();
			while (!hyNode.isNull())
			{
				QDomElement hyElem = hyNode.toElement();
				if (hyElem.tagName() == "Exception")
				{
					QString word = hyElem.attribute("Word");
					QString hyph = hyElem.attribute("Hyphenated");
					appPrefs.hyphPrefs.specialWords.insert(word, hyph);
				}
				else if (hyElem.tagName() == "Ignore")
				{
					QString word = hyElem.attribute("Word");
					appPrefs.hyphPrefs.ignoredWords.insert(word);
				}
				hyNode = hyNode.nextSibling();
			}
		}
		if (dc.tagName() == "Fonts")
		{
			appPrefs.fontPrefs.askBeforeSubstitute = static_cast<bool>(dc.attribute("AutomaticSubstitution", "1").toInt());
		}
		if (dc.tagName() == "Font")
		{
			QString tmpf(dc.attribute("Name"));
			if (!tmpf.isEmpty() && appPrefs.fontPrefs.AvailFonts.contains(tmpf))
			{
				ScFace& face(appPrefs.fontPrefs.AvailFonts[tmpf]);
				face.embedPs(static_cast<bool>(dc.attribute("Embed").toInt()));
				face.usable(face.usable() && static_cast<bool>(dc.attribute("Use", "1").toInt()));
				face.subset(static_cast<bool>(dc.attribute("Subset", "0").toInt()));
			}
		}
		if (dc.tagName() == "Substitute")
		  appPrefs.fontPrefs.GFontSub[dc.attribute("Name")] = dc.attribute("Replace");
		if (dc.tagName() == "DefaultColorSet")
		{
			appPrefs.colorPrefs.DColorSet = dc.attribute("Name");
			if (appPrefs.colorPrefs.DColorSet == "Scribus Small")
			{
				appPrefs.colorPrefs.DColors.clear();
				appPrefs.colorPrefs.DColors.insert("White", ScColor(0, 0, 0, 0));
				appPrefs.colorPrefs.DColors.insert("Black", ScColor(0, 0, 0, 255));
				ScColor cc(255, 255, 255, 255);
				cc.setRegistrationColor(true);
				appPrefs.colorPrefs.DColors.insert("Registration", cc);
				appPrefs.colorPrefs.DColors.insert("Blue", ScColor(255, 255, 0, 0));
				appPrefs.colorPrefs.DColors.insert("Cyan", ScColor(255, 0, 0, 0));
				appPrefs.colorPrefs.DColors.insert("Green", ScColor(255, 0, 255, 0));
				appPrefs.colorPrefs.DColors.insert("Red", ScColor(0, 255, 255, 0));
				appPrefs.colorPrefs.DColors.insert("Yellow", ScColor(0, 0, 255, 0));
				appPrefs.colorPrefs.DColors.insert("Magenta", ScColor(0, 255, 0, 0));
			}
		}
		if (dc.tagName() == "PDF")
		{
			appPrefs.pdfPrefs.Articles = static_cast<bool>(dc.attribute("Articles").toInt());
			appPrefs.pdfPrefs.Thumbnails = static_cast<bool>(dc.attribute("Thumbnails").toInt());
			appPrefs.pdfPrefs.Compress = static_cast<bool>(dc.attribute("Compress").toInt());
			appPrefs.pdfPrefs.CompressMethod = (PDFOptions::PDFCompression) dc.attribute("CompressMethod", "0").toInt();
			appPrefs.pdfPrefs.Quality = dc.attribute("Quality", "0").toInt();
			appPrefs.pdfPrefs.embedPDF  = dc.attribute("EmbedPDF", "0").toInt();
			appPrefs.pdfPrefs.RecalcPic = static_cast<bool>(dc.attribute("RecalcPic").toInt());
			appPrefs.pdfPrefs.Bookmarks = static_cast<bool>(dc.attribute("Bookmarks").toInt());
			appPrefs.pdfPrefs.MirrorH = static_cast<bool>(dc.attribute("MirrorPagesHorizontal").toInt());
			appPrefs.pdfPrefs.MirrorV = static_cast<bool>(dc.attribute("MirrorPagesVertical").toInt());
			appPrefs.pdfPrefs.doClip = static_cast<bool>(dc.attribute("DoClip").toInt());
			appPrefs.pdfPrefs.RotateDeg = dc.attribute("RotateDeg", "0").toInt();
			appPrefs.pdfPrefs.PresentMode = static_cast<bool>(dc.attribute("PresentMode").toInt());
			appPrefs.pdfPrefs.PicRes = dc.attribute("PicRes").toInt();
			appPrefs.pdfPrefs.Version = (PDFVersion::Version) dc.attribute("Version").toInt();
			appPrefs.pdfPrefs.Resolution = dc.attribute("Resolution").toInt();
			appPrefs.pdfPrefs.Binding = dc.attribute("Binding").toInt();
			appPrefs.pdfPrefs.fileName = "";
			appPrefs.pdfPrefs.FontEmbedding = (PDFOptions::PDFFontEmbedding)  dc.attribute("FontEmbedding", "0").toInt();
			appPrefs.pdfPrefs.isGrayscale = static_cast<bool>(dc.attribute("Grayscale", "0").toInt());
			appPrefs.pdfPrefs.UseRGB = static_cast<bool>(dc.attribute("RGBMode", "0").toInt());
			appPrefs.pdfPrefs.UseProfiles = static_cast<bool>(dc.attribute("UseProfiles", "0").toInt());
			appPrefs.pdfPrefs.UseProfiles2 = static_cast<bool>(dc.attribute("UseProfiles2", "0").toInt());
			appPrefs.pdfPrefs.Intent = dc.attribute("Intent", "1").toInt();
			appPrefs.pdfPrefs.Intent2 = dc.attribute("Intent2", "1").toInt();
			appPrefs.pdfPrefs.SolidProf = dc.attribute("SolidProfile", "");
			appPrefs.pdfPrefs.ImageProf = dc.attribute("ImageProfile", "");
			appPrefs.pdfPrefs.PrintProf = dc.attribute("PrintProfile", "");
			appPrefs.pdfPrefs.Info = dc.attribute("InfoString", "");
			appPrefs.pdfPrefs.bleeds.setTop(ScCLocale::toDoubleC(dc.attribute("BleedTop"), 0.0));
			appPrefs.pdfPrefs.bleeds.setLeft(ScCLocale::toDoubleC(dc.attribute("BleedLeft"), 0.0));
			appPrefs.pdfPrefs.bleeds.setRight(ScCLocale::toDoubleC(dc.attribute("BleedRight"), 0.0));
			appPrefs.pdfPrefs.bleeds.setBottom(ScCLocale::toDoubleC(dc.attribute("BleedBottom"), 0.0));
			appPrefs.pdfPrefs.useDocBleeds = static_cast<bool>(dc.attribute("UseDocBleeds", "1").toInt());
			appPrefs.pdfPrefs.cropMarks = static_cast<bool>(dc.attribute("CropMarks", "0").toInt());
			appPrefs.pdfPrefs.bleedMarks = static_cast<bool>(dc.attribute("BleedMarks", "0").toInt());
			appPrefs.pdfPrefs.registrationMarks = static_cast<bool>(dc.attribute("RegistrationMarks", "0").toInt());
			appPrefs.pdfPrefs.colorMarks = static_cast<bool>(dc.attribute("ColorMarks", "0").toInt());
			appPrefs.pdfPrefs.docInfoMarks = static_cast<bool>(dc.attribute("DocInfoMarks", "0").toInt());
			appPrefs.pdfPrefs.markLength = ScCLocale::toDoubleC(dc.attribute("MarkLength"), 20.0);
			appPrefs.pdfPrefs.markOffset = ScCLocale::toDoubleC(dc.attribute("MarkOffset"), 0.0);
			appPrefs.pdfPrefs.EmbeddedI  = static_cast<bool>(dc.attribute("ImagePr", "0").toInt());
			appPrefs.pdfPrefs.PassOwner  = dc.attribute("PassOwner", "");
			appPrefs.pdfPrefs.PassUser   = dc.attribute("PassUser", "");
			appPrefs.pdfPrefs.Permissions = dc.attribute("Permissions", "-4").toInt();
			appPrefs.pdfPrefs.Encrypt = static_cast<bool>(dc.attribute("Encrypt", "0").toInt());
			appPrefs.pdfPrefs.useLayers = static_cast<bool>(dc.attribute("UseLayers", "0").toInt());
			appPrefs.pdfPrefs.UseLPI = static_cast<bool>(dc.attribute("UseLpi", "0").toInt());
			appPrefs.pdfPrefs.UseSpotColors = static_cast<bool>(dc.attribute("UseSpotColors", "1").toInt());
			appPrefs.pdfPrefs.doMultiFile = static_cast<bool>(dc.attribute("DoMultiFile", "0").toInt());
			appPrefs.pdfPrefs.displayBookmarks = static_cast<bool>(dc.attribute("DisplayBookmarks", "0").toInt());
			appPrefs.pdfPrefs.displayFullscreen = static_cast<bool>(dc.attribute("DisplayFullscreen", "0").toInt());
			appPrefs.pdfPrefs.displayLayers = static_cast<bool>(dc.attribute("DisplayLayers", "0").toInt());
			appPrefs.pdfPrefs.displayThumbs = static_cast<bool>(dc.attribute("DisplayThumbs", "0").toInt());
			appPrefs.pdfPrefs.hideMenuBar = static_cast<bool>(dc.attribute("hideMenuBar", "0").toInt());
			appPrefs.pdfPrefs.hideToolBar = static_cast<bool>(dc.attribute("hideToolBar", "0").toInt());
			appPrefs.pdfPrefs.fitWindow = static_cast<bool>(dc.attribute("fitWindow", "0").toInt());
			appPrefs.pdfPrefs.openAfterExport = static_cast<bool>(dc.attribute("openAfterExport", "0").toInt());
			appPrefs.pdfPrefs.PageLayout = dc.attribute("PageLayout", "0").toInt();
			appPrefs.pdfPrefs.openAction = dc.attribute("OpenAction", "");
			QDomNode pfoNode = DOC.firstChild();
			appPrefs.pdfPrefs.LPISettings.clear();
			while (!pfoNode.isNull())
			{
				QDomElement pdfF = pfoNode.toElement();
				if (pdfF.tagName() == "LPI")
				{
					struct LPIData lpo;
					lpo.Angle = pdfF.attribute("Angle").toInt();
					lpo.Frequency = pdfF.attribute("Frequency").toInt();
					lpo.SpotFunc = pdfF.attribute("SpotFunction").toInt();
					appPrefs.pdfPrefs.LPISettings[pdfF.attribute("Color")] = lpo;
				}
				pfoNode = pfoNode.nextSibling();
			}
		}
		if (dc.tagName() == "DefaultItemAttributes")
		{
			QDomNode diaNode = DOC.firstChild();
			appPrefs.itemAttrPrefs.defaultItemAttributes.clear();
			while (!diaNode.isNull())
			{
				QDomElement itemAttr = diaNode.toElement();
				if (itemAttr.tagName() == "ItemAttribute")
				{
					ObjectAttribute objAttr;
					objAttr.name = itemAttr.attribute("Name");
					objAttr.type = itemAttr.attribute("Type");
					objAttr.value = itemAttr.attribute("Value");
					objAttr.parameter = itemAttr.attribute("Parameter");
					objAttr.relationship = itemAttr.attribute("Relationship");
					objAttr.relationshipto = itemAttr.attribute("RelationshipTo");
					objAttr.autoaddto = itemAttr.attribute("AutoAddTo");
					appPrefs.itemAttrPrefs.defaultItemAttributes.append(objAttr);
				}
				diaNode = diaNode.nextSibling();
			}
		}
		if (dc.tagName() == "TablesOfContents")
		{
			QDomNode tocNode = DOC.firstChild();
			appPrefs.tocPrefs.defaultToCSetups.clear();
			while (!tocNode.isNull())
			{
				QDomElement tocElem = tocNode.toElement();
				if (tocElem.tagName() == "TableOfContents")
				{
					ToCSetup tocsetup;
					tocsetup.name = tocElem.attribute("Name");
					tocsetup.itemAttrName = tocElem.attribute("ItemAttributeName");
					tocsetup.frameName = tocElem.attribute("FrameName");
					tocsetup.listNonPrintingFrames = static_cast<bool>(tocElem.attribute("ListNonPrinting").toInt());
					tocsetup.textStyle = tocElem.attribute("Style");
					QString numberPlacement = tocElem.attribute("NumberPlacement");
					if (numberPlacement == "Beginning")
						tocsetup.pageLocation = Beginning;
					if (numberPlacement == "End")
						tocsetup.pageLocation = End;
					if (numberPlacement == "NotShown")
						tocsetup.pageLocation = NotShown;
					appPrefs.tocPrefs.defaultToCSetups.append(tocsetup);
				}
				tocNode = tocNode.nextSibling();
			}
		}
		// cache manager
		if (dc.tagName() == "ImageCache")
		{
			appPrefs.imageCachePrefs.cacheEnabled = static_cast<bool>(dc.attribute("Enabled", "0").toInt());
			appPrefs.imageCachePrefs.maxCacheSizeMiB = dc.attribute("MaximumCacheSizeMiB", "1000").toInt();
			appPrefs.imageCachePrefs.maxCacheEntries = dc.attribute("MaximumCacheEntries", "1000").toInt();
			appPrefs.imageCachePrefs.compressionLevel = dc.attribute("CompressionLevel", "1").toInt();
		}
		// active page sizes
		if (dc.tagName() == "ActivePageSizes")
		{
			appPrefs.activePageSizes = QString(dc.attribute("Names", "")).split(",");
		}
		// experimental features
		if (dc.tagName() == "ExperimentalFeatures")
		{
			appPrefs.experimentalFeaturePrefs.notesEnabled = static_cast<bool>(dc.attribute("NotesEnabled", "0").toInt());
		}

		//
		DOC = DOC.nextSibling();
	}
	// Some sanity checks
	appPrefs.colorPrefs.DColors.ensureDefaultColors();
	if ((appPrefs.docSetupPrefs.pagePositioning  < 0) || (appPrefs.docSetupPrefs.pagePositioning >= appPrefs.pageSets.count()))
		appPrefs.docSetupPrefs.pagePositioning = 0;
	if ((appPrefs.docSetupPrefs.docUnitIndex < UNITMIN) || (appPrefs.docSetupPrefs.docUnitIndex > UNITMAX))
		appPrefs.docSetupPrefs.docUnitIndex = int(SC_POINTS);
	// Configure GUI
	appPrefs.ui_SystemTheme = QApplication::style()->objectName();
	if (appPrefs.uiPrefs.style.length() > 0)
	{
		QStyle* qtStyle = nullptr;
		QStringList availableStyles = QStyleFactory::keys();
		if (availableStyles.contains(appPrefs.uiPrefs.style))
			qtStyle = QStyleFactory::create(appPrefs.uiPrefs.style);
		if (qtStyle)
			QApplication::setStyle(qtStyle);
		else
			appPrefs.uiPrefs.style.clear();
	}
	QFont apf = QApplication::font();
	apf.setPointSize(appPrefs.uiPrefs.applicationFontSize);
	QApplication::setFont(apf);
	return true;
}

void PrefsManager::initDefaultCheckerPrefs(CheckerPrefsList& cp)
{
	struct CheckerPrefs checkerSettings;
	checkerSettings.ignoreErrors = false;
	checkerSettings.autoCheck = true;
	checkerSettings.checkGlyphs = true;
	checkerSettings.checkOrphans = true;
	checkerSettings.checkOverflow = true;
	checkerSettings.checkPictures = true;
	checkerSettings.checkResolution = true;
	checkerSettings.checkPartFilledImageFrames = false;
	checkerSettings.checkTransparency = true;
	checkerSettings.checkAnnotations = false;
	checkerSettings.checkRasterPDF = true;
	checkerSettings.checkForGIF = true;
	checkerSettings.ignoreOffLayers = false;
	checkerSettings.checkOffConflictLayers = false;
	checkerSettings.minResolution = 144.0;
	checkerSettings.maxResolution = 2400.0;
	checkerSettings.checkNotCMYKOrSpot = false;
	checkerSettings.checkDeviceColorsAndOutputIntent = false;
	checkerSettings.checkFontNotEmbedded = false;
	checkerSettings.checkFontIsOpenType = false;
	checkerSettings.checkAppliedMasterDifferentSide = true;
	checkerSettings.checkEmptyTextFrames = true;
	//TODO Stop translating these into settings!!!!!!!!!
	cp.insert(CommonStrings::PostScript, checkerSettings);
	checkerSettings.checkFontNotEmbedded = true;
	checkerSettings.checkFontIsOpenType = true;
	cp.insert(CommonStrings::PDF_1_3   , checkerSettings);
	checkerSettings.checkTransparency = false;
	cp.insert(CommonStrings::PDF_1_4   , checkerSettings);
	cp.insert(CommonStrings::PDF_1_5   , checkerSettings);
	checkerSettings.checkFontIsOpenType = false;
	cp.insert(CommonStrings::PDF_1_6   , checkerSettings);
	checkerSettings.checkFontIsOpenType = true;
	checkerSettings.checkTransparency = true;
	checkerSettings.checkAnnotations = true;
	checkerSettings.minResolution = 144.0;
	checkerSettings.checkDeviceColorsAndOutputIntent = true;
	cp.insert(CommonStrings::PDF_X3	, checkerSettings);
	checkerSettings.checkNotCMYKOrSpot = true;
	checkerSettings.checkDeviceColorsAndOutputIntent = false;
	cp.insert(CommonStrings::PDF_X1a	, checkerSettings);
	checkerSettings.checkNotCMYKOrSpot = false;
	checkerSettings.checkDeviceColorsAndOutputIntent = true;
	checkerSettings.checkTransparency = false;
	checkerSettings.checkFontIsOpenType = false;
	cp.insert(CommonStrings::PDF_X4	, checkerSettings);
}

void PrefsManager::insertMissingCheckerProfiles(CheckerPrefsList& cp)
{
	CheckerPrefsList defaultList;
	initDefaultCheckerPrefs(defaultList);

	CheckerPrefsList::const_iterator it = defaultList.constBegin();
	for (; it != defaultList.constEnd(); ++it)
	{
		const QString& name = it.key();
		if (cp.contains(name))
			continue;
		cp.insert(name, it.value());
	}
}

const QString & PrefsManager::lastError() const
{
	return m_lastError;
}

// It's hard to say whether this should be here and called from savePrefs, or
// triggered by a signal sent from here and displayed by ScribusMainWindow.
void PrefsManager::alertSavePrefsFailed() const
{
	ScMessageBox::critical(ScCore->primaryMainWindow(), tr("Error Writing Preferences"),
			"<qt>" +
			tr("Scribus was not able to save its preferences:<br>"
			   "%1<br>"
			   "Please check file and directory permissions and "
			   "available disk space.", "scribus app error")
			   .arg(lastError())
			+ "</qt>");
}

// It's hard to say whether this should be here and called from readPrefs, or
// triggered by a signal sent from here and displayed by ScribusMainWindow.
void PrefsManager::alertLoadPrefsFailed() const
{
	bool splashShowing = ScCore->splashShowing();
	if (splashShowing)
		ScCore->showSplash(false);
	ScMessageBox::critical(ScCore->primaryMainWindow(), tr("Error Loading Preferences"),
			"<qt>" +
			tr("Scribus was not able to load its preferences:<br>"
			   "%1<br>"
			   "Default settings will be loaded.")
			   .arg(lastError())
			+ "</qt>");
	ScCore->showSplash(splashShowing);
}



void PrefsManager::languageChange()
{
	if (appPrefs.itemToolPrefs.textFillColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.textFillColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.textLineColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.textLineColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.shapeLineColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.shapeLineColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.textColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.textColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.textStrokeColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.textStrokeColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.shapeFillColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.shapeFillColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.lineColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.lineColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.imageFillColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.imageFillColor = CommonStrings::None;
	if (appPrefs.itemToolPrefs.imageStrokeColor == CommonStrings::tr_NoneColor)
		appPrefs.itemToolPrefs.imageStrokeColor = CommonStrings::None;
}

