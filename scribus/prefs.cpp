/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "prefs.h"
#include "prefs.moc"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "units.h"
#include "langmgr.h"
#include "hysettings.h"
#include "cmsprefs.h"
#include "tabtools.h"
#include "undomanager.h"
#include "tabcheckdoc.h"
#include "tabpdfoptions.h"
#include "fontprefs.h"
#include "units.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "docitemattrprefs.h"
#include "marginWidget.h"
#include "prefsmanager.h"
#include "polygonwidget.h"
#include "fontcombo.h"
#include "linecombo.h"
#include "arrowchooser.h"
#include "pagelayout.h"
#include "pluginmanager.h"
#include "pluginmanagerprefsgui.h"
#include "scplugin.h"
#include "sccombobox.h"
#include "colorcombo.h"
#include "commonstrings.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "tabtypography.h"
#include "tabguides.h"
#include "tabexternaltoolswidget.h"
#include "tabkeyboardshortcutswidget.h"
#include "tocindexprefs.h"
#include "hyphenator.h"
#include "tabgeneral.h"
#include "tabdisplay.h"
#include "tabdocument.h"
#include "tabmiscellaneous.h"
#include "tabscrapbook.h"

using namespace std;

extern QPixmap loadIcon(QString nam);
extern ScribusQApp* ScQApp;

Preferences::Preferences( QWidget* parent) : PrefsDialogBase( parent )
{
	prefsManager=PrefsManager::instance();
	ApplicationPrefs* prefsData=&(prefsManager->appPrefs);
	ap = (ScribusMainWindow*)parent;
	docUnitIndex = prefsData->docUnitIndex;
	unitRatio = unitGetRatioFromIndex(docUnitIndex);

	setCaption( tr( "Preferences" ) );

	tabGeneral = new TabGeneral( prefsWidgets, "tabGeneral" );
	addItem( tr("General"), loadIcon("scribusicon.png"), tabGeneral);

	tabDocument = new TabDocument( prefsWidgets, "tabDocument" );
	addItem( tr("Document"), loadIcon("scribusdoc.png"), tabDocument);

	tabGuides = new TabGuides(prefsWidgets, &prefsData->guidesSettings, &prefsData->typographicSettings, docUnitIndex);
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);

	tabTypo = new TabTypograpy(  prefsWidgets, &prefsData->typographicSettings);
	addItem( tr("Typography"), loadIcon("typography.png"), tabTypo);

	tabTools = new TabTools(prefsWidgets, &prefsData->toolSettings, docUnitIndex, ap->doc);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ap->LangTransl);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	tabFonts = new FontPrefs(prefsWidgets, false, prefsManager->preferencesLocation(), ap->doc);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(  prefsWidgets, prefsData->checkerProfiles, prefsData->curCheckProfile);
	addItem( tr("Preflight Verifier"), loadIcon("checkdoc.png"), tabDocChecker);

	if (ScCore->haveCMS())
	{
		tabColorManagement = new CMSPrefs(prefsWidgets, &prefsData->DCMSset, &ScCore->InputProfiles, &ScCore->InputProfilesCMYK, &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
		addItem( tr("Color Management"), loadIcon("blend.png"), tabColorManagement);
	}

	QMap<QString, int> DocFonts;
	DocFonts.clear();
	tabPDF = new TabPDFOptions( prefsWidgets,
								prefsData->PDF_Options,
								prefsData->AvailFonts,
								ScCore->PDFXProfiles,
								DocFonts,
								prefsData->PDF_Options.PresentVals,
								docUnitIndex,
								prefsData->PageHeight,
								prefsData->PageWidth,
								0 );
	addItem( tr("PDF Export"), loadIcon("acroread.png"), tabPDF);

	tabDefaultItemAttributes = new DocumentItemAttributes( prefsWidgets);
	QStringList defaultAttributesList=tabDefaultItemAttributes->getDocAttributesNames();
	tabDefaultItemAttributes->setup(&prefsData->defaultItemAttributes);
	addItem( tr("Document Item Attributes"), loadIcon("docattributes.png"), tabDefaultItemAttributes);

	tabDefaultTOCIndexPrefs = new TOCIndexPrefs( prefsWidgets );
	tabDefaultTOCIndexPrefs->setupItemAttrs( defaultAttributesList );
	tabDefaultTOCIndexPrefs->setup(&prefsData->defaultToCSetups, NULL);
	addItem( tr("Table of Contents and Indexes"), loadIcon("tabtocindex.png"), tabDefaultTOCIndexPrefs);

	tabKeyboardShortcuts = new TabKeyboardShortcutsWidget(prefsData->KeyActions, prefsWidgets);
	addItem( tr("Keyboard Shortcuts"), loadIcon("key_bindings.png"), tabKeyboardShortcuts);

	tabScrapbook = new TabScrapbook( prefsWidgets, "tabScrapbook" );
	addItem(  tr("Scrapbook"), loadIcon("scrap.png"), tabScrapbook);

	tabView = new TabDisplay(prefsWidgets, "tabView");
	addItem( tr("Display"), loadIcon("screen.png"), tabView);

	tabExtTools = new TabExternalToolsWidget( prefsData, prefsWidgets );
	addItem(  tr("External Tools"), loadIcon("externaltools.png"), tabExtTools);

	tabMiscellaneous = new TabMiscellaneous( prefsWidgets, "tabMiscellaneous" );
	addItem(  tr("Miscellaneous"), loadIcon("misc.png"), tabMiscellaneous);

	// plugin manager. pv.
	pluginManagerPrefsGui = new PluginManagerPrefsGui(prefsWidgets);
	addItem( tr("Plugins"), loadIcon("plugins.png"), pluginManagerPrefsGui );

	setupGui();
	addPlugins();

	setDS(prefsData->FacingPages);

	resize( minimumSizeHint() );
	arrangeIcons();
	prefsSelection->setSelected(prefsSelection->firstItem(), true);
	itemSelected(prefsSelection->firstItem());
	clearWState( WState_Polished );
}

void Preferences::enableSignals(bool on)
{
	if (on)
	{
		connect(tabDocument->unitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));
 		connect(tabDocument->docLayout, SIGNAL( selectedLayout(int) ), this, SLOT( setDS(int) ) );
		connect(buttonOk, SIGNAL(clicked()), this, SLOT(setActionHistoryLength()));
		if (ScCore->haveCMS())
			connect(tabColorManagement, SIGNAL(cmsOn(bool )), this, SLOT(switchCMS(bool )));
		connect(applyChangesButton, SIGNAL(clicked()), this, SLOT(applyChangesButton_clicked()));
		connect(backToDefaults, SIGNAL(clicked()), this, SLOT(backToDefaults_clicked()));
		connect( prefsWidgets, SIGNAL(aboutToShow(QWidget *)), this, SLOT(showWidgetInStack(QWidget *)));
		connect(this, SIGNAL(accepted()), pluginManagerPrefsGui, SLOT(apply()));
	}
	else
	{
		disconnect(tabDocument->unitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));
		disconnect(tabDocument->docLayout, SIGNAL( selectedLayout(int) ), this, SLOT( setDS(int) ) );
		disconnect(buttonOk, SIGNAL(clicked()), this, SLOT(setActionHistoryLength()));
		if (ScCore->haveCMS())
			disconnect(tabColorManagement, SIGNAL(cmsOn(bool )), this, SLOT(switchCMS(bool )));
		disconnect(applyChangesButton, SIGNAL(clicked()), this, SLOT(applyChangesButton_clicked()));
		disconnect(backToDefaults, SIGNAL(clicked()), this, SLOT(backToDefaults_clicked()));
		disconnect( prefsWidgets, SIGNAL(aboutToShow(QWidget *)), this, SLOT(showWidgetInStack(QWidget *)));
		disconnect(this, SIGNAL(accepted()), pluginManagerPrefsGui, SLOT(apply()));
	}
}

void Preferences::restoreDefaults()
{
	prefsManager->initDefaults();
	setupGui();
}

void Preferences::setupGui()
{
	enableSignals(false);
	ApplicationPrefs* prefsData=&(prefsManager->appPrefs);

	tabGeneral->restoreDefaults(prefsData);
	tabDocument->restoreDefaults(prefsData);
	tabView->restoreDefaults(prefsData, &prefsData->guidesSettings);
	tabScrapbook->restoreDefaults(prefsData);
	tabHyphenator->restoreDefaults(prefsData);
	tabGuides->restoreDefaults(&prefsData->guidesSettings, &prefsData->typographicSettings, docUnitIndex);
	tabTypo->restoreDefaults(&prefsData->typographicSettings);
	tabTools->restoreDefaults(&prefsData->toolSettings, docUnitIndex);
	// main performance issue in availFonts->GetFonts(HomeP)!
	// no prefsData here
	tabFonts->restoreDefaults();
	tabDocChecker->restoreDefaults(&prefsData->checkerProfiles, prefsData->curCheckProfile);

	QMap<QString, int> DocFonts;
	DocFonts.clear();
	tabPDF->restoreDefaults(prefsData->PDF_Options,
							prefsData->AvailFonts,
							ScCore->PDFXProfiles,
							DocFonts,
							prefsData->PDF_Options.PresentVals,
							docUnitIndex,
							prefsData->PageHeight,
							prefsData->PageWidth,
							0, false);

	tabColorManagement->restoreDefaults(&prefsData->DCMSset, &ScCore->InputProfiles,
										 &ScCore->InputProfilesCMYK,
										 &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
	QStringList defaultAttributesList=tabDefaultItemAttributes->getDocAttributesNames();
	tabDefaultItemAttributes->setup(&prefsData->defaultItemAttributes);
	tabDefaultTOCIndexPrefs->setupItemAttrs( defaultAttributesList );
	tabDefaultTOCIndexPrefs->setup(&prefsData->defaultToCSetups, NULL);
	tabKeyboardShortcuts->restoreDefaults();
	tabExtTools->restoreDefaults(prefsData);
	tabMiscellaneous->restoreDefaults(prefsData);
	enableSignals(true);
	unitChange();
}

void Preferences::addPlugins()
{
	// Scan for plugins that provide a prefs widget, and add it to the
	// prefs dialog.
	// For each plugin, enabled or not:
	PluginManager& pluginManager = PluginManager::instance();
	QValueList<QCString> pluginNames(pluginManager.pluginNames(true));
	QValueList<QCString>::Iterator itEnd(pluginNames.end());
	for (QValueList<QCString>::Iterator it(pluginNames.begin()); it != itEnd ; ++it )
	{
		// Ask the plugin manager for a plugin (skipping disabled plugins).
		ScPlugin* plugin = pluginManager.getPlugin(*it, false);
		// If we got a plugin (which we know is enabled):
		if (plugin)
		{
			// Ask the plugin for a prefs widget
			PrefsPanel* panel;
			QString panelCaption;
			QPixmap panelIcon;
			bool wantPanel = plugin->newPrefsPanelWidget(
					prefsWidgets, panel, panelCaption, panelIcon);
			// If it gave us one...
			if (wantPanel)
			{
				// Ensure that we got sane return values
				Q_ASSERT(panel);
				Q_ASSERT(!panelIcon.isNull());
				Q_ASSERT(!panelCaption.isNull());
				// plug it in to the dialog,
				addItem(panelCaption, panelIcon, panel);
				// and connect a signal to tell it to save its
				// settings.
				connect(this, SIGNAL(accepted()), panel, SLOT(apply()));
			}
		}
	}
}

void Preferences::accept()
{
	emit accepted();
	return PrefsDialogBase::accept();
}

void Preferences::setDS(int layout)
{
	tabDocument->marginGroup->setFacingPages(!(layout == singlePage));
	tabDocument->choosenLayout = layout;
	tabDocument->docLayout->firstPage->setCurrentItem(prefsManager->appPrefs.pageSets[tabDocument->choosenLayout].FirstPage);
	tabView->gapHorizontal->setValue(prefsManager->appPrefs.pageSets[tabDocument->choosenLayout].GapHorizontal * unitRatio);
	tabView->gapVertical->setValue(prefsManager->appPrefs.pageSets[tabDocument->choosenLayout].GapVertical * unitRatio);
}

void Preferences::unitChange()
{
	double oldUnitRatio = unitRatio;
	docUnitIndex = tabDocument->unitCombo->currentItem();
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);
	QString suffix = unitGetSuffixFromIndex(docUnitIndex);
	double invUnitConversion = 1.0 / oldUnitRatio * unitRatio;
	
	tabDocument->unitChange();
	tabGuides->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
	tabView->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
	tabTools->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
	tabPDF->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
}


void Preferences::setActionHistoryLength()
{
	UndoManager::instance()->setHistoryLength(tabDocument->urSpinBox->value());
}

void Preferences::switchCMS(bool enable)
{
	tabPDF->enableCMS(enable);
}

void Preferences::showWidgetInStack(QWidget *widgetToShow)
{
	//Update the attributes list in TOC setup
	if (widgetToShow==tabDefaultTOCIndexPrefs)
		tabDefaultTOCIndexPrefs->setupItemAttrs( tabDefaultItemAttributes->getDocAttributesNames() );
	if (widgetToShow == tabTools)
		tabTools->enableFontPreview(true);
}

void Preferences::updatePreferences()
{
	double prefsUnitRatio = unitGetRatioFromIndex(tabDocument->unitCombo->currentItem());

	prefsManager->appPrefs.AppFontSize = tabGeneral->GFsize->value();
	prefsManager->appPrefs.PaletteFontSize = tabGeneral->GTFsize->value();
	ScQApp->neverSplash(!tabMiscellaneous->showSplashCheckBox->isChecked());
	prefsManager->appPrefs.Wheelval = tabGeneral->wheelJumpSpin->value();
	prefsManager->appPrefs.moveTimeout = tabGeneral->spinTimeout->value();
	prefsManager->appPrefs.RecentDCount = tabGeneral->recentDocs->value();
	prefsManager->appPrefs.DocDir = tabGeneral->Docs->text();
	prefsManager->appPrefs.ProfileDir = tabGeneral->ProPfad->text();
	prefsManager->appPrefs.ScriptDir = tabGeneral->ScriptPfad->text();
	prefsManager->appPrefs.documentTemplatesDir = tabGeneral->DocumentTemplateDir->text();
	prefsManager->appPrefs.guiLanguage=tabGeneral->selectedGUILang;
	prefsManager->appPrefs.GUI = tabGeneral->GUICombo->currentText();

	prefsManager->appPrefs.pageSets[tabDocument->choosenLayout].GapHorizontal = tabView->gapHorizontal->value() / prefsUnitRatio;
	prefsManager->appPrefs.pageSets[tabDocument->choosenLayout].GapVertical = tabView->gapVertical->value() / prefsUnitRatio;
	prefsManager->appPrefs.marginColored = tabView->checkUnprintable->isChecked();
	prefsManager->appPrefs.ScratchBottom = tabView->bottomScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.ScratchLeft = tabView->leftScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.ScratchRight = tabView->rightScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.ScratchTop = tabView->topScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.DpapColor = tabView->colorPaper;
	prefsManager->appPrefs.DFrameColor = tabView->colorFrame;
	prefsManager->appPrefs.guidesSettings.framesShown = tabView->checkFrame->isChecked();
	prefsManager->appPrefs.guidesSettings.layerMarkersShown = tabView->checkLayerM->isChecked();
	prefsManager->appPrefs.guidesSettings.rulerMode = tabView->checkRuler->isChecked();
	prefsManager->appPrefs.guidesSettings.showPic = tabView->checkPictures->isChecked();
	prefsManager->appPrefs.guidesSettings.linkShown = tabView->checkLink->isChecked();
	prefsManager->appPrefs.guidesSettings.showControls = tabView->checkControl->isChecked();
	prefsManager->appPrefs.DisScale = tabView->DisScale;

	prefsManager->appPrefs.doCopyToScrapbook = tabScrapbook->useScrapBookasExtension->isChecked();
	prefsManager->appPrefs.persistentScrapbook = tabScrapbook->persistentScrapbook->isChecked();
	prefsManager->appPrefs.numScrapbookCopies = tabScrapbook->numScrapCopies->value();

	tabTools->polyWidget->getValues(&prefsManager->appPrefs.toolSettings.polyC, &prefsManager->appPrefs.toolSettings.polyFd, &prefsManager->appPrefs.toolSettings.polyF, &prefsManager->appPrefs.toolSettings.polyS, &prefsManager->appPrefs.toolSettings.polyR);

	prefsManager->appPrefs.pageSize = tabDocument->prefsPageSizeName;
	prefsManager->appPrefs.pageOrientation = tabDocument->pageOrientationComboBox->currentItem();
	prefsManager->appPrefs.PageWidth = tabDocument->pageW;
	prefsManager->appPrefs.PageHeight = tabDocument->pageH;

	prefsManager->appPrefs.RandOben = tabDocument->marginGroup->top();
	prefsManager->appPrefs.RandUnten = tabDocument->marginGroup->bottom();
	prefsManager->appPrefs.RandLinks = tabDocument->marginGroup->left();
	prefsManager->appPrefs.RandRechts = tabDocument->marginGroup->right();
	prefsManager->appPrefs.FacingPages  = tabDocument->choosenLayout;
	prefsManager->appPrefs.pageSets[tabDocument->choosenLayout].FirstPage = tabDocument->docLayout->firstPage->currentItem();

	prefsManager->setImageEditorExecutable(tabExtTools->newImageTool());
	prefsManager->setExtBrowserExecutable(tabExtTools->newExtBrowserTool());
	prefsManager->appPrefs.gs_AntiAliasGraphics = tabExtTools->newAntialiasGraphics();
	prefsManager->appPrefs.gs_AntiAliasText = tabExtTools->newAntialiasText();
	prefsManager->setGhostscriptExecutable(tabExtTools->newPSTool());
	prefsManager->appPrefs.gs_Resolution = tabExtTools->newPSToolResolution();

	prefsManager->appPrefs.guidesSettings.before = tabGuides->inBackground->isChecked();
	
	prefsManager->appPrefs.askBeforeSubstituite = tabMiscellaneous->AskForSubs->isChecked();
	prefsManager->appPrefs.haveStylePreview = tabMiscellaneous->stylePreview->isChecked();
	prefsManager->appPrefs.showStartupDialog = tabMiscellaneous->startUpDialog->isChecked();
	// lorem ipsum
	prefsManager->appPrefs.useStandardLI = tabMiscellaneous->useStandardLI->isChecked();
	prefsManager->appPrefs.paragraphsLI = tabMiscellaneous->paragraphsLI->value();

	prefsManager->appPrefs.docUnitIndex = tabDocument->unitCombo->currentItem();

	prefsManager->appPrefs.toolSettings.defFont = tabTools->fontComboText->currentText();
	prefsManager->appPrefs.toolSettings.defSize = tabTools->sizeComboText->currentText().left(2).toInt() * 10;

	prefsManager->appPrefs.guidesSettings.marginsShown = tabGuides->marginBox->isChecked();
	prefsManager->appPrefs.guidesSettings.gridShown = tabGuides->checkGrid->isChecked();
	prefsManager->appPrefs.guidesSettings.guidesShown = tabGuides->guideBox->isChecked();
	prefsManager->appPrefs.guidesSettings.baseShown = tabGuides->baselineBox->isChecked();
	prefsManager->appPrefs.guidesSettings.grabRad = tabGuides->grabDistance->value();
	prefsManager->appPrefs.guidesSettings.guideRad = tabGuides->snapDistance->value() / prefsUnitRatio;
	prefsManager->appPrefs.guidesSettings.minorGrid = tabGuides->minorSpace->value() / prefsUnitRatio;
	prefsManager->appPrefs.guidesSettings.majorGrid = tabGuides->majorSpace->value() / prefsUnitRatio;
	prefsManager->appPrefs.guidesSettings.minorColor = tabGuides->colorMinorGrid;
	prefsManager->appPrefs.guidesSettings.majorColor = tabGuides->colorMajorGrid;
	prefsManager->appPrefs.guidesSettings.margColor = tabGuides->colorMargin;
	prefsManager->appPrefs.guidesSettings.guideColor = tabGuides->colorGuides;
	prefsManager->appPrefs.guidesSettings.baseColor = tabGuides->colorBaselineGrid;
	prefsManager->appPrefs.checkerProfiles = tabDocChecker->checkerProfile;
	prefsManager->appPrefs.curCheckProfile = tabDocChecker->curCheckProfile->currentText();
	prefsManager->appPrefs.typographicSettings.valueSuperScript = tabTypo->superDisplacement->value();
	prefsManager->appPrefs.typographicSettings.scalingSuperScript = tabTypo->superScaling->value();
	prefsManager->appPrefs.typographicSettings.valueSubScript = tabTypo->subDisplacement->value();
	prefsManager->appPrefs.typographicSettings.scalingSubScript = tabTypo->subScaling->value();
	prefsManager->appPrefs.typographicSettings.valueSmallCaps = tabTypo->capsScaling->value();
	prefsManager->appPrefs.typographicSettings.autoLineSpacing = tabTypo->autoLine->value();
	prefsManager->appPrefs.typographicSettings.valueBaseGrid = tabGuides->baseGrid->value() / prefsUnitRatio;
	prefsManager->appPrefs.typographicSettings.offsetBaseGrid = tabGuides->baseOffset->value() / prefsUnitRatio;
	prefsManager->appPrefs.typographicSettings.valueUnderlinePos = qRound(tabTypo->underlinePos->value() * 10);
	prefsManager->appPrefs.typographicSettings.valueUnderlineWidth = qRound(tabTypo->underlineWidth->value() * 10);
	prefsManager->appPrefs.typographicSettings.valueStrikeThruPos = qRound(tabTypo->strikethruPos->value() * 10);
	prefsManager->appPrefs.typographicSettings.valueStrikeThruWidth = qRound(tabTypo->strikethruWidth->value() * 10);
	prefsManager->appPrefs.toolSettings.dTextBackGround = tabTools->colorComboTextBackground->currentText();
	if (prefsManager->appPrefs.toolSettings.dTextBackGround == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dTextBackGround = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.dTextLineColor = tabTools->colorComboTextLine->currentText();
	if (prefsManager->appPrefs.toolSettings.dTextLineColor == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dTextLineColor = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.dTextBackGroundShade = tabTools->shadingTextBack->value();
	prefsManager->appPrefs.toolSettings.dTextLineShade = tabTools->shadingTextLine->value();
	prefsManager->appPrefs.toolSettings.dTextPenShade = tabTools->shadingText->value();
	prefsManager->appPrefs.toolSettings.dTextStrokeShade = tabTools->shadingTextStroke->value();
	prefsManager->appPrefs.toolSettings.dPen = tabTools->colorComboLineShape->currentText();
	if (prefsManager->appPrefs.toolSettings.dPen == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dPen = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.dPenText = tabTools->colorComboText->currentText();
	if (prefsManager->appPrefs.toolSettings.dPenText == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dPenText = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.dStrokeText = tabTools->colorComboStrokeText->currentText();
	if (prefsManager->appPrefs.toolSettings.dStrokeText == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dStrokeText = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.dCols = tabTools->columnsText->value();
	prefsManager->appPrefs.toolSettings.dGap = tabTools->gapText->value() / prefsUnitRatio;
	prefsManager->appPrefs.toolSettings.dTabWidth = tabTools->gapTab->value() / prefsUnitRatio;
	prefsManager->appPrefs.toolSettings.dBrush = tabTools->comboFillShape->currentText();
	if (prefsManager->appPrefs.toolSettings.dBrush == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dBrush = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.dShade = tabTools->shadingFillShape->value();
	prefsManager->appPrefs.toolSettings.dShade2 = tabTools->shadingLineShape->value();
	switch (tabTools->tabFillCombo->currentItem())
	{
		case 0:
			prefsManager->appPrefs.toolSettings.tabFillChar = "";
			break;
		case 1:
			prefsManager->appPrefs.toolSettings.tabFillChar = ".";
			break;
		case 2:
			prefsManager->appPrefs.toolSettings.tabFillChar = "-";
			break;
		case 3:
			prefsManager->appPrefs.toolSettings.tabFillChar = "_";
			break;
		case 4:
			prefsManager->appPrefs.toolSettings.tabFillChar = tabTools->tabFillCombo->currentText().right(1);
			break;
	}
	switch (tabTools->comboStyleShape->currentItem())
	{
		case 0:
			prefsManager->appPrefs.toolSettings.dLineArt = SolidLine;
			break;
		case 1:
			prefsManager->appPrefs.toolSettings.dLineArt = DashLine;
			break;
		case 2:
			prefsManager->appPrefs.toolSettings.dLineArt = DotLine;
			break;
		case 3:
			prefsManager->appPrefs.toolSettings.dLineArt = DashDotLine;
			break;
		case 4:
			prefsManager->appPrefs.toolSettings.dLineArt = DashDotDotLine;
			break;
	}
	prefsManager->appPrefs.toolSettings.dWidth = tabTools->lineWidthShape->value();
	prefsManager->appPrefs.toolSettings.dPenLine = tabTools->colorComboLine->currentText();
	if (prefsManager->appPrefs.toolSettings.dPenLine == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dPenLine = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.dShadeLine = tabTools->shadingLine->value();
	switch (tabTools->comboStyleLine->currentItem())
	{
	case 0:
		prefsManager->appPrefs.toolSettings.dLstyleLine = SolidLine;
		break;
	case 1:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DashLine;
		break;
	case 2:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DotLine;
		break;
	case 3:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DashDotLine;
		break;
	case 4:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DashDotDotLine;
		break;
	}
	prefsManager->appPrefs.toolSettings.dWidthLine = tabTools->lineWidthLine->value();
	prefsManager->appPrefs.toolSettings.dStartArrow = tabTools->startArrow->currentItem();
	prefsManager->appPrefs.toolSettings.dEndArrow = tabTools->endArrow->currentItem();
	prefsManager->appPrefs.toolSettings.magMin = tabTools->minimumZoom->value();
	prefsManager->appPrefs.toolSettings.magMax = tabTools->maximumZoom->value();
	prefsManager->appPrefs.toolSettings.magStep = tabTools->zoomStep->value();
	prefsManager->appPrefs.toolSettings.dBrushPict = tabTools->comboFillImage->currentText();
	if (prefsManager->appPrefs.toolSettings.dBrushPict == CommonStrings::NoneColor)
		prefsManager->appPrefs.toolSettings.dBrushPict = CommonStrings::None;
	prefsManager->appPrefs.toolSettings.shadePict = tabTools->shadingFillImage->value();
	prefsManager->appPrefs.toolSettings.scaleX = static_cast<double>(tabTools->scalingHorizontal->value()) / 100.0;
	prefsManager->appPrefs.toolSettings.scaleY = static_cast<double>(tabTools->scalingVertical->value()) / 100.0;
	prefsManager->appPrefs.toolSettings.scaleType = tabTools->buttonGroup3->isChecked();
	prefsManager->appPrefs.toolSettings.aspectRatio = tabTools->checkRatioImage->isChecked();
	prefsManager->appPrefs.toolSettings.useEmbeddedPath = tabTools->embeddedPath->isChecked();
	int haRes = 0;
	if (tabTools->checkFullRes->isChecked())
		haRes = 0;
	if (tabTools->checkNormalRes->isChecked())
		haRes = 1;
	if (tabTools->checkHalfRes->isChecked())
		haRes = 2;
	prefsManager->appPrefs.toolSettings.lowResType = haRes;
	prefsManager->appPrefs.AutoSave = tabDocument->GroupAS->isChecked();
	prefsManager->appPrefs.AutoSaveTime = tabDocument->ASTime->value() * 60 * 1000;
	prefsManager->appPrefs.MinWordLen = tabHyphenator->wordLen->value();
	prefsManager->appPrefs.Language = ScCore->primaryMainWindow()->GetLang(tabHyphenator->language->currentText());
	prefsManager->appPrefs.Automatic = !tabHyphenator->verbose->isChecked();
	prefsManager->appPrefs.AutoCheck = tabHyphenator->input->isChecked();
	prefsManager->appPrefs.HyCount = tabHyphenator->maxCount->value();
	if (ScCore->haveCMS())
		tabColorManagement->setValues();
	uint a = 0;
	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
	for ( ; it.hasNext() ; it.next())
	{
		it.current().embedPs(tabFonts->fontFlags[it.currentKey()].FlagPS);
		it.current().usable(tabFonts->fontFlags[it.currentKey()].FlagUse);
		it.current().subset(tabFonts->fontFlags[it.currentKey()].FlagSub);
	}
	a = 0;
	QMap<QString,QString>::Iterator itfsuend=tabFonts->RList.end();
	prefsManager->appPrefs.GFontSub.clear();
	for (QMap<QString,QString>::Iterator itfsu = tabFonts->RList.begin(); itfsu != itfsuend; ++itfsu)
		prefsManager->appPrefs.GFontSub[itfsu.key()] = tabFonts->FlagsRepl.at(a++)->currentText();
	prefsManager->appPrefs.PDF_Options.Thumbnails = tabPDF->CheckBox1->isChecked();
	prefsManager->appPrefs.PDF_Options.Compress = tabPDF->Compression->isChecked();
	prefsManager->appPrefs.PDF_Options.CompressMethod = tabPDF->CMethod->currentItem();
	prefsManager->appPrefs.PDF_Options.Quality = tabPDF->CQuality->currentItem();
	prefsManager->appPrefs.PDF_Options.Resolution = tabPDF->Resolution->value();
	prefsManager->appPrefs.PDF_Options.RecalcPic = tabPDF->DSColor->isChecked();
	prefsManager->appPrefs.PDF_Options.PicRes = tabPDF->ValC->value();
	prefsManager->appPrefs.PDF_Options.Bookmarks = tabPDF->CheckBM->isChecked();
	prefsManager->appPrefs.PDF_Options.Binding = tabPDF->ComboBind->currentItem();
	prefsManager->appPrefs.PDF_Options.MirrorH = tabPDF->MirrorH->isOn();
	prefsManager->appPrefs.PDF_Options.MirrorV = tabPDF->MirrorV->isOn();
	prefsManager->appPrefs.PDF_Options.RotateDeg = tabPDF->RotateDeg->currentItem() * 90;
	prefsManager->appPrefs.PDF_Options.Articles = tabPDF->Article->isChecked();
	prefsManager->appPrefs.PDF_Options.Encrypt = tabPDF->Encry->isChecked();
	prefsManager->appPrefs.PDF_Options.UseLPI = tabPDF->UseLPI->isChecked();
	prefsManager->appPrefs.PDF_Options.UseSpotColors = tabPDF->overprintMode->isChecked();
	prefsManager->appPrefs.PDF_Options.doOverprint = !tabPDF->useSpot->isChecked();
	prefsManager->appPrefs.PDF_Options.doMultiFile = false;
	prefsManager->appPrefs.PDF_Options.BleedBottom = tabPDF->BleedBottom->value() / prefsUnitRatio;
	prefsManager->appPrefs.PDF_Options.BleedTop = tabPDF->BleedTop->value() / prefsUnitRatio;
	prefsManager->appPrefs.PDF_Options.BleedLeft = tabPDF->BleedLeft->value() / prefsUnitRatio;
	prefsManager->appPrefs.PDF_Options.BleedRight = tabPDF->BleedRight->value() / prefsUnitRatio;
	prefsManager->appPrefs.PDF_Options.doClip = tabPDF->ClipMarg->isChecked();
	if (tabPDF->Encry->isChecked())
	{
		int Perm = -64;
		if (tabPDF->PDFVersionCombo->currentItem() == 1)
			Perm &= ~0x00240000;
		if (tabPDF->PrintSec->isChecked())
			Perm += 4;
		if (tabPDF->ModifySec->isChecked())
			Perm += 8;
		if (tabPDF->CopySec->isChecked())
			Perm += 16;
		if (tabPDF->AddSec->isChecked())
			Perm += 32;
		prefsManager->appPrefs.PDF_Options.Permissions = Perm;
		prefsManager->appPrefs.PDF_Options.PassOwner = tabPDF->PassOwner->text();
		prefsManager->appPrefs.PDF_Options.PassUser = tabPDF->PassUser->text();
	}
	if (tabPDF->PDFVersionCombo->currentItem() == 0)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_13;
	if (tabPDF->PDFVersionCombo->currentItem() == 1)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_14;
	if (tabPDF->PDFVersionCombo->currentItem() == 2)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_15;
	if (tabPDF->PDFVersionCombo->currentItem() == 3)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_X3;
	if (tabPDF->OutCombo->currentItem() == 0)
	{
		prefsManager->appPrefs.PDF_Options.isGrayscale = false;
		prefsManager->appPrefs.PDF_Options.UseRGB = true;
		prefsManager->appPrefs.PDF_Options.UseProfiles = false;
		prefsManager->appPrefs.PDF_Options.UseProfiles2 = false;
	}
	else
	{
		if (tabPDF->OutCombo->currentItem() == 2)
		{
			prefsManager->appPrefs.PDF_Options.isGrayscale = true;
			prefsManager->appPrefs.PDF_Options.UseRGB = false;
			prefsManager->appPrefs.PDF_Options.UseProfiles = false;
			prefsManager->appPrefs.PDF_Options.UseProfiles2 = false;
		}
		else
		{
			prefsManager->appPrefs.PDF_Options.isGrayscale = false;
			prefsManager->appPrefs.PDF_Options.UseRGB = false;
#ifdef HAVE_CMS
			if (/*CMSuse*/ ScCore->haveCMS())
			{
				prefsManager->appPrefs.PDF_Options.UseProfiles = tabPDF->EmbedProfs->isChecked();
				prefsManager->appPrefs.PDF_Options.UseProfiles2 = tabPDF->EmbedProfs2->isChecked();
				prefsManager->appPrefs.PDF_Options.Intent = tabPDF->IntendS->currentItem();
				prefsManager->appPrefs.PDF_Options.Intent2 = tabPDF->IntendI->currentItem();
				prefsManager->appPrefs.PDF_Options.EmbeddedI = tabPDF->NoEmbedded->isChecked();
				prefsManager->appPrefs.PDF_Options.SolidProf = tabPDF->SolidPr->currentText();
				prefsManager->appPrefs.PDF_Options.ImageProf = tabPDF->ImageP->currentText();
				prefsManager->appPrefs.PDF_Options.PrintProf = tabPDF->PrintProfC->currentText();
			}
#endif
		}
	}
	prefsManager->appPrefs.defaultItemAttributes = *(tabDefaultItemAttributes->getNewAttributes());
	prefsManager->appPrefs.defaultToCSetups = *(tabDefaultTOCIndexPrefs->getNewToCs());
// 	prefsManager->appPrefs.KeyActions = tabKeys->getNewKeyMap();
	prefsManager->appPrefs.KeyActions = tabKeyboardShortcuts->getNewKeyMap();
}

void Preferences::applyChangesButton_clicked()
{
	ScCore->primaryMainWindow()->prefsOrg(this);
}

void Preferences::backToDefaults_clicked()
{
	prefsManager->initDefaults();
	restoreDefaults();
}
