/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "reformdoc.h"

#include <QColorDialog>
#include <QCursor>
#include <QPixmap>
#include <QSpinBox>
#include <QStackedWidget>
#include <QToolTip>

#include "arrowchooser.h"
#include "cmsprefs.h"
#include "colorcombo.h"
#include "commonstrings.h"
#include "docinfo.h"
#include "docitemattrprefs.h"
#include "docsections.h"
#include "fontcombo.h"
#include "fontprefs.h"
#include "hyphenator.h"
#include "hysettings.h"
#include "linecombo.h"
#include "marginwidget.h"
#include "page.h"
#include "pagelayout.h"
#include "pagesize.h"
#include "polygonwidget.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "sccombobox.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scrspinbox.h"
#include "tabcheckdoc.h"
#include "tabdisplay.h"
#include "tabdocument.h"
#include "tabguides.h"
#include "tabpdfoptions.h"
#include "tabtools.h"
#include "tabtypography.h"
#include "tocindexprefs.h"
#include "units.h"
#include "util_icon.h"



ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc ) : PrefsDialogBase( parent )
{
	saveButton->hide();
	docUnitIndex = doc->unitIndex();
	currDoc = doc;
	ScMW = (ScribusMainWindow*)parent;
	unitRatio = doc->unitRatio();
	QString ein = unitGetSuffixFromIndex(docUnitIndex);
	decimals = unitGetDecimalsFromIndex(docUnitIndex);

	setWindowTitle( tr( "Document Setup" ) );
	tabPage = new TabDocument( prefsWidgets, "tab", true );
	tabPage->hideReform();
	addItem( tr("Document"), loadIcon("scribusdoc.png"), tabPage);

	docInfos = new DocInfos(prefsWidgets, doc->documentInfo);
	addItem( tr("Document Information"), loadIcon("documentinfo32.png"), docInfos);

	tabGuides = new TabGuides(prefsWidgets, &doc->guidesSettings, &doc->typographicSettings, docUnitIndex);
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);

	tabView = new TabDisplay( prefsWidgets, "tabView" );
	tabView->setDocSetupMode();
	addItem( tr("Display"), loadIcon("screen.png"), tabView);

	tabTypo = new TabTypograpy( prefsWidgets, &doc->typographicSettings);
	addItem( tr("Typography"), loadIcon("typography.png"), tabTypo);

	tabTools = new TabTools( prefsWidgets, &doc->toolSettings, docUnitIndex, doc);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets/*, &ScMW->LangTransl*/);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	tabFonts = new FontPrefs(prefsWidgets, true, PrefsManager::instance()->preferencesLocation(), doc);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(prefsWidgets, doc->checkerProfiles, doc->curCheckProfile);
	addItem( tr("Preflight Verifier"), loadIcon("checkdoc.png"), tabDocChecker);

	tabPDF = new TabPDFOptions( prefsWidgets, doc->PDF_Options, PrefsManager::instance()->appPrefs.AvailFonts,
								ScCore->PDFXProfiles, doc->UsedFonts, doc->PDF_Options.PresentVals,
								docUnitIndex, doc->pageHeight, doc->pageWidth, doc );
	addItem( tr("PDF Export"), loadIcon("acroread32.png"), tabPDF);

	tabDocItemAttributes = new DocumentItemAttributes(prefsWidgets);
	docAttributesList=tabDocItemAttributes->getDocAttributesNames();
	tabDocItemAttributes->setup(&doc->docItemAttributes);
	addItem( tr("Document Item Attributes"), loadIcon("docattributes.png"), tabDocItemAttributes);

	tabTOCIndexPrefs = new TOCIndexPrefs( prefsWidgets );
	tabTOCIndexPrefs->setupItemAttrs( docAttributesList );
	tabTOCIndexPrefs->setup(&(currDoc->docToCSetups), currDoc);
	addItem( tr("Table of Contents and Indexes"), loadIcon("tabtocindex.png"), tabTOCIndexPrefs);

	tabDocSections = new DocSections(prefsWidgets);
	tabDocSections->setup(currDoc->sections, currDoc->DocPages.count()-1);
	addItem( tr("Sections"), loadIcon("tabtocindex.png"), tabDocSections);

	int cmsTab = 0;
	if (ScCore->haveCMS())
	{
		tabColorManagement = new CMSPrefs(prefsWidgets, &doc->CMSSettings, &ScCore->InputProfiles, &ScCore->InputProfilesCMYK, &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
		cmsTab = addItem( tr("Color Management"), loadIcon("blend.png"), tabColorManagement);
	}

	setDS(doc->currentPageLayout);

	// signals and slots connections
	connect(tabPage->docLayout, SIGNAL( selectedLayout(int) ), this, SLOT( setDS(int) ) );
	connect(tabPage->unitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));
	connect(backToDefaults, SIGNAL(clicked()), this, SLOT(restoreDefaults()));
	connect(applyChangesButton, SIGNAL(clicked()), this, SLOT(applyChangesButton_clicked()));
	connect(this, SIGNAL(aboutToShow(QWidget *)), this, SLOT(showWidgetInStack(QWidget *)));

	if (ScCore->haveCMS())
	{
		prefsWidgets->setCurrentIndex(cmsTab);
		connect(tabColorManagement, SIGNAL(cmsOn(bool )), this, SLOT(switchCMS(bool )));
	}
	resize( minimumSizeHint() );
	prefsSelection->arrangeIcons();
	prefsSelection->item(0)->setSelected(true);
	itemSelected(prefsSelection->item(0));
	restoreDefaults();
}

void ReformDoc::restoreDefaults()
{
	ApplicationPrefs* prefsData=&(PrefsManager::instance()->appPrefs);
	tabPage->restoreDefaults(currDoc);
	tabView->restoreDefaults(prefsData, currDoc->guidesSettings, currDoc->pageSets, currDoc->currentPageLayout, currDoc->scratch);
	tabView->gapHorizontal->setValue(currDoc->GapHorizontal); // * unitRatio);
	tabView->gapVertical->setValue(currDoc->GapVertical); // * unitRatio);
	tabView->setPaperColor(currDoc->papColor);
	tabView->setMarginColored(currDoc->marginColored);
	tabHyphenator->restoreDefaults(currDoc);
	tabGuides->restoreDefaults(&currDoc->guidesSettings, &currDoc->typographicSettings, docUnitIndex);
	tabTypo->restoreDefaults(&currDoc->typographicSettings);
	tabTools->restoreDefaults(&currDoc->toolSettings, docUnitIndex);
	tabFonts->restoreDefaults();
	tabDocChecker->restoreDefaults(&currDoc->checkerProfiles, currDoc->curCheckProfile);
	tabPDF->restoreDefaults(currDoc->PDF_Options, PrefsManager::instance()->appPrefs.AvailFonts,
							ScCore->PDFXProfiles, currDoc->UsedFonts, currDoc->PDF_Options.PresentVals,
							docUnitIndex, currDoc->pageHeight, currDoc->pageWidth, currDoc, false);
	if (ScCore->haveCMS())
		tabColorManagement->restoreDefaults(&currDoc->CMSSettings, &ScCore->InputProfiles,
										 &ScCore->InputProfilesCMYK,
										 &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
	docInfos->restoreDefaults();

	unitChange();
}

void ReformDoc::unitChange()
{
	double oldUnitRatio = unitRatio;
	docUnitIndex = tabPage->unitCombo->currentIndex();
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	QString suffix = unitGetSuffixFromIndex(docUnitIndex);
	double invUnitConversion = 1.0 / oldUnitRatio * unitRatio;
	tabPage->unitChange();
	tabGuides->unitChange(suffix, docUnitIndex, invUnitConversion);
	tabView->unitChange(docUnitIndex);
	tabTools->unitChange(docUnitIndex);
	tabPDF->unitChange(suffix, docUnitIndex, invUnitConversion);
}

void ReformDoc::setDS(int layout)
{
	tabPage->marginGroup->setFacingPages(!(layout == singlePage));
	tabPage->choosenLayout = layout;
	tabPage->docLayout->firstPage->setCurrentIndex(currDoc->pageSets[tabPage->choosenLayout].FirstPage);
//	tabView->gapHorizontal->setValue(currDoc->pageSets[tabPage->choosenLayout].GapHorizontal * unitRatio);
//	tabView->gapVertical->setValue(currDoc->pageSets[tabPage->choosenLayout].GapBelow * unitRatio);
//	tabView->gapHorizontal->setValue(currDoc->GapHorizontal * unitRatio);
//	tabView->gapVertical->setValue(currDoc->GapVertical * unitRatio);
}

void ReformDoc::switchCMS(bool enable)
{
	tabPDF->enableCMS(enable);
}

void ReformDoc::showWidgetInStack(QWidget *widgetToShow)
{
	//Update the attributes list in TOC setup
	if (widgetToShow==tabTOCIndexPrefs)
		tabTOCIndexPrefs->setupItemAttrs( tabDocItemAttributes->getDocAttributesNames() );
	if (widgetToShow == tabTools)
		tabTools->enableFontPreview(true);
}

const int ReformDoc::getSelectedUnit()
{
	return tabPage->unitCombo->currentIndex();
}

const bool ReformDoc::colorManagementSettingsChanged()
{
	return ScCore->haveCMS() && tabColorManagement->changed;
}

const bool ReformDoc::imageResolutionChanged()
{
	return viewToRecalcPictureRes;
}

void ReformDoc::updateDocumentSettings()
{
	MarginStruct updatedMargins(tabPage->marginGroup->margins());
	int fp = tabPage->choosenLayout;
	double oldBaseGridValue  = currDoc->typographicSettings.valueBaseGrid;
	double oldBaseGridOffset = currDoc->typographicSettings.offsetBaseGrid;

	currDoc->pageSets[fp].FirstPage = tabPage->docLayout->firstPage->currentIndex();
//	currDoc->pageSets[fp].GapHorizontal = tabView->gapHorizontal->value() / currDoc->unitRatio();
	currDoc->GapVertical = tabView->gapVertical->value() / currDoc->unitRatio();
	currDoc->GapHorizontal = tabView->gapHorizontal->value() / currDoc->unitRatio();
//	currDoc->pageSets[fp].GapBelow = tabView->gapVertical->value() / currDoc->unitRatio();
	//currDoc->FirstPnum = pageNumber->value();
	currDoc->resetPage(updatedMargins, fp);
	currDoc->PageOri = tabPage->pageOrientationComboBox->currentIndex();
	currDoc->m_pageSize = tabPage->prefsPageSizeName;
	currDoc->pageWidth = tabPage->pageW;
	currDoc->pageHeight = tabPage->pageH;
	currDoc->marginPreset = tabPage->marginGroup->getMarginPreset();
	double TopD = tabView->topScratch->value() / currDoc->unitRatio() - currDoc->scratch.Top;
	double LeftD = tabView->leftScratch->value() / currDoc->unitRatio() - currDoc->scratch.Left;
	currDoc->scratch.Bottom = tabView->bottomScratch->value() / currDoc->unitRatio();
	currDoc->scratch.Left = tabView->leftScratch->value() / currDoc->unitRatio();
	currDoc->scratch.Right = tabView->rightScratch->value() / currDoc->unitRatio();
	currDoc->scratch.Top = tabView->topScratch->value() / currDoc->unitRatio();
	currDoc->bleeds.Bottom = tabPage->marginGroup->bottomBleed();
	currDoc->bleeds.Top = tabPage->marginGroup->topBleed();
	currDoc->bleeds.Left = tabPage->marginGroup->leftBleed();
	currDoc->bleeds.Right = tabPage->marginGroup->rightBleed();
	for (int p = 0; p < currDoc->Pages->count(); ++p)
	{
		Page *pp = currDoc->Pages->at(p);
		if (tabPage->sizeAllPages->isChecked())
		{
			pp->setInitialWidth(currDoc->pageWidth);
			pp->setInitialHeight(currDoc->pageHeight);
			pp->setHeight(currDoc->pageHeight);
			pp->setWidth(currDoc->pageWidth);
			pp->m_pageSize = currDoc->m_pageSize;
			pp->PageOri = currDoc->PageOri;
		}
		if (tabPage->marginGroup->getMarginsForAllPages())
		{
			pp->initialMargins=updatedMargins;
			pp->marginPreset=currDoc->marginPreset;
		}
		else
		if (tabPage->marginGroup->getMarginsForAllMasterPages())
		{
			//CB #6796: find the master page (*mp) for the current page (*pp)
			//check if *pp's margins are the same as the *mp's current margins
			//apply new margins if same
			const int masterPageNumber = currDoc->MasterNames[pp->MPageNam];
			const Page* mp = currDoc->MasterPages.at(masterPageNumber);
			if (pp->initialMargins.Left == mp->initialMargins.Left &&
				pp->initialMargins.Right == mp->initialMargins.Right &&
				pp->initialMargins.Top == mp->initialMargins.Top &&
				pp->initialMargins.Bottom == mp->initialMargins.Bottom)
			{
				pp->initialMargins=updatedMargins;
				pp->marginPreset=currDoc->marginPreset;
			}
		}
	}
	for (int p = 0; p < currDoc->MasterPages.count(); ++p)
	{
		Page *pp = currDoc->MasterPages.at(p);
		if (tabPage->sizeAllMasterPages->isChecked())
		{
			pp->setInitialWidth(currDoc->pageWidth);
			pp->setInitialHeight(currDoc->pageHeight);
			pp->setHeight(currDoc->pageHeight);
			pp->setWidth(currDoc->pageWidth);
			pp->m_pageSize = currDoc->m_pageSize;
			pp->PageOri = currDoc->PageOri;
		}
		if (tabPage->marginGroup->getMarginsForAllMasterPages())
		{
			pp->initialMargins=updatedMargins;
			pp->marginPreset=currDoc->marginPreset;
		}
		pp->setXOffset(currDoc->scratch.Left);
		pp->setYOffset(currDoc->scratch.Top);
	}
	uint docItemsCount = currDoc->MasterItems.count();
	for (uint ite = 0; ite < docItemsCount; ++ite)
	{
		PageItem *item = currDoc->MasterItems.at(ite);
		item->moveBy(LeftD, TopD);
		item->setRedrawBounding();
	}
	currDoc->guidesSettings.before = tabGuides->inBackground->isChecked();
	currDoc->marginColored = tabView->checkUnprintable->isChecked();
	currDoc->papColor = tabView->colorPaper;
	currDoc->guidesSettings.marginsShown = tabGuides->marginBox->isChecked();
	currDoc->guidesSettings.showBleed = tabView->checkBleed->isChecked();
	currDoc->guidesSettings.framesShown = tabView->checkFrame->isChecked();
	currDoc->guidesSettings.layerMarkersShown = tabView->checkLayerM->isChecked();
	currDoc->guidesSettings.gridShown = tabGuides->checkGrid->isChecked();
	currDoc->guidesSettings.guidesShown = tabGuides->guideBox->isChecked();
	currDoc->guidesSettings.baseShown = tabGuides->baselineBox->isChecked();
	currDoc->guidesSettings.showPic = tabView->checkPictures->isChecked();
	currDoc->guidesSettings.linkShown = tabView->checkLink->isChecked();
	currDoc->guidesSettings.showControls = tabView->checkControl->isChecked();
	currDoc->guidesSettings.rulerMode = tabView->checkRuler->isChecked();
	currDoc->guidesSettings.grabRad = tabGuides->grabDistance->value();
	currDoc->guidesSettings.guideRad = tabGuides->snapDistance->value();
	currDoc->guidesSettings.minorGrid = tabGuides->minorSpace->value() / currDoc->unitRatio();
	currDoc->guidesSettings.majorGrid = tabGuides->majorSpace->value() / currDoc->unitRatio();
	currDoc->guidesSettings.minorColor = tabGuides->colorMinorGrid;
	currDoc->guidesSettings.majorColor = tabGuides->colorMajorGrid;
	currDoc->guidesSettings.margColor = tabGuides->colorMargin;
	currDoc->guidesSettings.guideColor = tabGuides->colorGuides;
	currDoc->guidesSettings.baseColor = tabGuides->colorBaselineGrid;
	currDoc->checkerProfiles = tabDocChecker->checkerProfile;
	currDoc->curCheckProfile = tabDocChecker->curCheckProfile->currentText();
	currDoc->typographicSettings.valueSuperScript = tabTypo->superDisplacement->value();
	currDoc->typographicSettings.scalingSuperScript = tabTypo->superScaling->value();
	currDoc->typographicSettings.valueSubScript = tabTypo->subDisplacement->value();
	currDoc->typographicSettings.scalingSubScript = tabTypo->subScaling->value();
	currDoc->typographicSettings.valueSmallCaps = tabTypo->capsScaling->value();
	currDoc->typographicSettings.autoLineSpacing = tabTypo->autoLine->value();
	currDoc->typographicSettings.valueBaseGrid = tabGuides->baseGrid->value(); // / currDoc->unitRatio();
	currDoc->typographicSettings.offsetBaseGrid = tabGuides->baseOffset->value(); // / currDoc->unitRatio();
	currDoc->typographicSettings.valueUnderlinePos = qRound(tabTypo->underlinePos->value() * 10);
	currDoc->typographicSettings.valueUnderlineWidth = qRound(tabTypo->underlineWidth->value() * 10);
	currDoc->typographicSettings.valueStrikeThruPos = qRound(tabTypo->strikethruPos->value() * 10);
	currDoc->typographicSettings.valueStrikeThruWidth = qRound(tabTypo->strikethruWidth->value() * 10);
	currDoc->toolSettings.defFont = tabTools->fontComboText->currentText();
	currDoc->toolSettings.defSize = tabTools->sizeComboText->currentText().left(2).toInt() * 10;
	currDoc->toolSettings.dStrokeText = tabTools->colorComboStrokeText->currentText();
	switch (tabTools->tabFillCombo->currentIndex())
	{
		case 0:
			currDoc->toolSettings.tabFillChar = "";
			break;
		case 1:
			currDoc->toolSettings.tabFillChar = ".";
			break;
		case 2:
			currDoc->toolSettings.tabFillChar = "-";
			break;
		case 3:
			currDoc->toolSettings.tabFillChar = "_";
			break;
		case 4:
			currDoc->toolSettings.tabFillChar = tabTools->tabFillCombo->currentText().right(1);
			break;
	}
	if (currDoc->toolSettings.dStrokeText == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dStrokeText = CommonStrings::None;
	currDoc->toolSettings.dPenText = tabTools->colorComboText->currentText();
	if (currDoc->toolSettings.dPenText == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dPenText = CommonStrings::None;
	currDoc->toolSettings.dTextBackGround = tabTools->colorComboTextBackground->currentText();
	if (currDoc->toolSettings.dTextBackGround == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dTextBackGround = CommonStrings::None;
	currDoc->toolSettings.dTextLineColor = tabTools->colorComboTextLine->currentText();
	if (currDoc->toolSettings.dTextLineColor == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dTextLineColor = CommonStrings::None;
	currDoc->toolSettings.dTextBackGroundShade = tabTools->shadingTextBack->value();
	currDoc->toolSettings.dTextLineShade = tabTools->shadingTextLine->value();
	currDoc->toolSettings.dTextPenShade = tabTools->shadingText->value();
	currDoc->toolSettings.dTextStrokeShade = tabTools->shadingTextStroke->value();
	currDoc->toolSettings.dCols = tabTools->columnsText->value();
	currDoc->toolSettings.dGap = tabTools->gapText->value() / currDoc->unitRatio();
	currDoc->toolSettings.dTabWidth = tabTools->gapTab->value() / currDoc->unitRatio();
	currDoc->toolSettings.dPen = tabTools->colorComboLineShape->currentText();
	if (currDoc->toolSettings.dPen == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dPen = CommonStrings::None;
	currDoc->toolSettings.dBrush = tabTools->comboFillShape->currentText();
	if (currDoc->toolSettings.dBrush == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dBrush = CommonStrings::None;
	currDoc->toolSettings.dShade = tabTools->shadingFillShape->value();
	currDoc->toolSettings.dShade2 = tabTools->shadingLineShape->value();
	currDoc->toolSettings.dLineArt = static_cast<Qt::PenStyle>(tabTools->comboStyleShape->currentIndex()) + 1;
	currDoc->toolSettings.dWidth = tabTools->lineWidthShape->value();
	currDoc->toolSettings.dStartArrow = tabTools->startArrow->currentIndex();
	currDoc->toolSettings.dEndArrow = tabTools->endArrow->currentIndex();
	currDoc->toolSettings.magMin = tabTools->minimumZoom->value();
	currDoc->toolSettings.magMax = tabTools->maximumZoom->value();
	currDoc->toolSettings.magStep = tabTools->zoomStep->value();
	currDoc->toolSettings.dPenLine = tabTools->colorComboLine->currentText();
	if (currDoc->toolSettings.dPenLine == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dPenLine = CommonStrings::None;
	currDoc->toolSettings.dShadeLine = tabTools->shadingLine->value();
	currDoc->toolSettings.dLstyleLine = static_cast<Qt::PenStyle>(tabTools->comboStyleLine->currentIndex()) + 1;
	currDoc->toolSettings.dWidthLine = tabTools->lineWidthLine->value();
	currDoc->toolSettings.dBrushPict = tabTools->comboFillImage->currentText();
	if (currDoc->toolSettings.dBrushPict == CommonStrings::tr_NoneColor)
		currDoc->toolSettings.dBrushPict = CommonStrings::None;
	currDoc->toolSettings.shadePict = tabTools->shadingFillImage->value();
	currDoc->toolSettings.scaleX = static_cast<double>(tabTools->scalingHorizontal->value()) / 100.0;
	currDoc->toolSettings.scaleY = static_cast<double>(tabTools->scalingVertical->value()) / 100.0;
	currDoc->toolSettings.scaleType = tabTools->buttonGroup3->isChecked();
	currDoc->toolSettings.aspectRatio = tabTools->checkRatioImage->isChecked();
	currDoc->toolSettings.useEmbeddedPath = tabTools->embeddedPath->isChecked();
	int haRes = 0;
	if (tabTools->checkFullRes->isChecked())
		haRes = 0;
	if (tabTools->checkNormalRes->isChecked())
		haRes = 1;
	if (tabTools->checkHalfRes->isChecked())
		haRes = 2;
	if (currDoc->toolSettings.lowResType != haRes)
	{
		currDoc->toolSettings.lowResType = haRes;
		viewToRecalcPictureRes=true;
	}
	else
		viewToRecalcPictureRes=false;
	currDoc->toolSettings.dispX = tabTools->genDispX->value();
	currDoc->toolSettings.dispY = tabTools->genDispY->value();
	currDoc->toolSettings.constrain = tabTools->genRot->value();
	tabTools->polyWidget->getValues(&currDoc->toolSettings.polyC,
									&currDoc->toolSettings.polyFd,
									&currDoc->toolSettings.polyF,
									&currDoc->toolSettings.polyS,
									&currDoc->toolSettings.polyR,
									&currDoc->toolSettings.polyCurvature);
	currDoc->AutoSave = tabPage->GroupAS->isChecked();
	currDoc->AutoSaveTime = tabPage->ASTime->value() * 60 * 1000;
	currDoc->autoSaveTimer->stop();
	if (currDoc->AutoSave)
		currDoc->autoSaveTimer->start(currDoc->AutoSaveTime);

/*	FIXME: scribus determines dict by charstyle now, so this setting should go into the doc's default charstyle
		currDoc->docHyphenator->slotNewDict(ScMW->GetLang(tabHyphenator->language->currentText()));
*/
	currDoc->Language = tabHyphenator->getLanguage();
	
	currDoc->docHyphenator->slotNewSettings(tabHyphenator->getWordLen(),
											!tabHyphenator->getVerbose(),
											tabHyphenator->getInput(),
											tabHyphenator->getMaxCount());
	currDoc->docHyphenator->ignoredWords = tabHyphenator->getIgnoreList();
	currDoc->docHyphenator->specialWords = tabHyphenator->getExceptionList();
	if (ScCore->haveCMS())
	{
		bool oldCM = currDoc->CMSSettings.CMSinUse;
		tabColorManagement->updateDocSettings(currDoc);
		if (tabColorManagement->changed)
		{
			ScMW->setStatusBarInfoText( tr("Adjusting Colors"));
			ScMW->mainWindowProgressBar->reset();
			int cc = currDoc->PageColors.count() + currDoc->Items->count();
			ScMW->mainWindowProgressBar->setMaximum(cc);
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
			bool newCM  = currDoc->CMSSettings.CMSinUse;
			bool updCol = false;
			currDoc->CMSSettings.CMSinUse = oldCM;
			currDoc->CloseCMSProfiles();
			currDoc->CMSSettings.CMSinUse = newCM;
			currDoc->HasCMS = currDoc->CMSSettings.CMSinUse;
			currDoc->SoftProofing = currDoc->CMSSettings.SoftProofOn;
			currDoc->Gamut = currDoc->CMSSettings.GamutCheck;
			currDoc->IntentColors = currDoc->CMSSettings.DefaultIntentColors;
			currDoc->IntentImages = currDoc->CMSSettings.DefaultIntentImages;
			if (!currDoc->CMSSettings.CMSinUse)
			{
				currDoc->HasCMS = false;
				if	(oldCM)
				{
					currDoc->SetDefaultCMSParams();
					updCol = true;
				}
			}
			else if ( currDoc->OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles) )
			{
				currDoc->HasCMS = true;
				currDoc->PDF_Options.SComp = currDoc->CMSSettings.ComponentsInput2;
				currDoc->PDF_Options.SolidProf = currDoc->CMSSettings.DefaultSolidColorRGBProfile;
				currDoc->PDF_Options.ImageProf = currDoc->CMSSettings.DefaultImageRGBProfile;
				currDoc->PDF_Options.PrintProf = currDoc->CMSSettings.DefaultPrinterProfile;
				currDoc->PDF_Options.Intent = currDoc->CMSSettings.DefaultIntentColors;
				updCol = true;
			}
			else
			{
				currDoc->SetDefaultCMSParams();
				currDoc->HasCMS = false;
			}
			if (updCol)
			{
				ScMW->recalcColors(ScMW->mainWindowProgressBar);
				currDoc->RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, ScMW->mainWindowProgressBar);
			}
			ScMW->mainWindowProgressBar->setValue(cc);
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			ScMW->setStatusBarInfoText("");
			ScMW->mainWindowProgressBar->reset();
		}
	}
	PrefsManager* prefsManager=PrefsManager::instance();
// 	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
// 	for ( ; it.hasNext() ; it.next())
// 	{
// 		it.current().embedPs(tabFonts->fontFlags[it.currentKey()].FlagPS);
// 		it.current().usable(tabFonts->fontFlags[it.currentKey()].FlagUse);
// 		it.current().subset(tabFonts->fontFlags[it.currentKey()].FlagSub);
// 	}
	uint a = 0;
	prefsManager->appPrefs.GFontSub.clear();
	QMap<QString,QString>::Iterator itfsu;
	QMap<QString,QString>::Iterator itfsuend=tabFonts->RList.end();
	for (itfsu = tabFonts->RList.begin(); itfsu != itfsuend; ++itfsu)
		prefsManager->appPrefs.GFontSub[itfsu.key()] = tabFonts->FlagsRepl.at(a++)->currentText();
	QStringList uf = currDoc->UsedFonts.keys();
	QMap<QString,int>::Iterator it3;
//	for (it3 = currDoc->UsedFonts.begin(); it3 != currDoc->UsedFonts.end(); ++it3)
//		FT_Done_Face(currDoc->FFonts[it3.key()]);
	currDoc->UsedFonts.clear();
	QStringList::Iterator it3a;
	QStringList::Iterator it3aend=uf.end();
	for (it3a = uf.begin(); it3a != it3aend; ++it3a)
		currDoc->AddFont(*it3a);

	tabPDF->updateDocumentSettings(currDoc);

	currDoc->documentInfo = docInfos->getDocInfo();
	currDoc->docItemAttributes = *(tabDocItemAttributes->getNewAttributes());
	currDoc->docToCSetups = *(tabTOCIndexPrefs->getNewToCs());
	currDoc->sections = tabDocSections->getNewSections();

	uint itemCount=currDoc->Items->count();
	for (uint b=0; b<itemCount; ++b)
	{
		if (currDoc->Items->at(b)->itemType() == PageItem::ImageFrame)
			currDoc->Items->at(b)->setImageShown(currDoc->guidesSettings.showPic);
	}

	if (oldBaseGridValue  != currDoc->typographicSettings.valueBaseGrid ||
		oldBaseGridOffset != currDoc->typographicSettings.offsetBaseGrid )
	{
		currDoc->invalidateAll();
	}
}

void ReformDoc::applyChangesButton_clicked()
{
	ScMW->docSetup(this);
}
