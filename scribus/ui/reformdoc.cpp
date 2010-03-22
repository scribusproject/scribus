/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "reformdoc.h"

#include <QColorDialog>
#include <QCursor>
#include <QSpinBox>
#include <QToolTip>
#include <QPixmap>
#include <QStackedWidget>

#include "arrowchooser.h"
#include "ui/cmsprefs.h"
#include "colorcombo.h"
#include "commonstrings.h"
#include "docinfo.h"
#include "ui/docitemattrprefs.h"
#include "ui/docsections.h"
#include "fontcombo.h"
#include "fontprefs.h"
#include "hyphenator.h"
#include "ui/hysettings.h"
#include "linecombo.h"
#include "marginwidget.h"
#include "page.h"
#include "pagelayout.h"
#include "pagesize.h"
#include "ui/polygonwidget.h"
#include "prefsmanager.h"
#include "ui/propertiespalette.h"
#include "sccombobox.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scrspinbox.h"
#include "ui/tabcheckdoc.h"
#include "ui/tabdisplay.h"
#include "ui/tabdocument.h"
#include "ui/tabguides.h"
#include "ui/tabpdfoptions.h"
#include "ui/tabtools.h"
#include "ui/tabtypography.h"
#include "ui/tocindexprefs.h"
#include "units.h"
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

	tabTools = new TabTools( prefsWidgets, &doc->itemToolPrefs, docUnitIndex, doc);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets/*, &ScMW->LangTransl*/);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	tabFonts = new FontPrefsTab(prefsWidgets, true, PrefsManager::instance()->preferencesLocation(), doc);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(prefsWidgets, doc->checkerProfiles, doc->curCheckProfile);
	addItem( tr("Preflight Verifier"), loadIcon("checkdoc.png"), tabDocChecker);

	tabPDF = new TabPDFOptions( prefsWidgets, doc->pdfOptions(), PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts,
								ScCore->PDFXProfiles, doc->UsedFonts, doc->pdfOptions().PresentVals,
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
	tabView->restoreDefaults(prefsData, currDoc->guidesSettings, currDoc->currentPageLayout, *currDoc->scratch());
	tabView->gapHorizontal->setValue(currDoc->pageGapHorizontal()); // * unitRatio);
	tabView->gapVertical->setValue(currDoc->pageGapVertical()); // * unitRatio);
	tabView->setPaperColor(currDoc->paperColor());
	tabView->setMarginColored(currDoc->marginColored);
	tabHyphenator->restoreDefaults(currDoc);
	tabGuides->restoreDefaults(&currDoc->guidesSettings, &currDoc->typographicSettings, docUnitIndex);
	tabTypo->restoreDefaults(&currDoc->typographicSettings);
	tabTools->restoreDefaults(&currDoc->itemToolPrefs, &currDoc->opToolPrefs, docUnitIndex);
	tabFonts->restoreDefaults();
	tabDocChecker->restoreDefaults(&currDoc->checkerProfiles, currDoc->curCheckProfile);
	tabPDF->restoreDefaults(currDoc->pdfOptions(), PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts,
							ScCore->PDFXProfiles, currDoc->UsedFonts, currDoc->pdfOptions().PresentVals,
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
	tabPage->docLayout->firstPage->setCurrentIndex(currDoc->pageSets()[tabPage->choosenLayout].FirstPage);
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
	currDoc->setPageSetFirstPage(fp, tabPage->docLayout->firstPage->currentIndex());
//	currDoc->pageSets[fp].GapHorizontal = tabView->gapHorizontal->value() / currDoc->unitRatio();
	currDoc->setPageGapHorizontal(tabView->gapVertical->value() / currDoc->unitRatio());
	currDoc->setPageGapVertical(tabView->gapHorizontal->value() / currDoc->unitRatio());
//	currDoc->pageSets[fp].GapBelow = tabView->gapVertical->value() / currDoc->unitRatio();
	//currDoc->FirstPnum = pageNumber->value();
	currDoc->resetPage(updatedMargins, fp);
	currDoc->PageOri = tabPage->pageOrientationComboBox->currentIndex();
	currDoc->m_pageSize = tabPage->prefsPageSizeName;
	currDoc->pageWidth = tabPage->pageW;
	currDoc->pageHeight = tabPage->pageH;
	currDoc->marginPreset = tabPage->marginGroup->getMarginPreset();
	double TopD = tabView->topScratch->value() / currDoc->unitRatio() - currDoc->scratch()->Top;
	double LeftD = tabView->leftScratch->value() / currDoc->unitRatio() - currDoc->scratch()->Left;
	currDoc->scratch()->set(tabView->topScratch->value() / currDoc->unitRatio(), tabView->leftScratch->value() / currDoc->unitRatio(), tabView->bottomScratch->value() / currDoc->unitRatio(), tabView->rightScratch->value() / currDoc->unitRatio());
//	currDoc->scratch.Bottom = tabView->bottomScratch->value() / currDoc->unitRatio();
//	currDoc->scratch.Left = tabView->leftScratch->value() / currDoc->unitRatio();
//	currDoc->scratch.Right = tabView->rightScratch->value() / currDoc->unitRatio();
//	currDoc->scratch.Top = tabView->topScratch->value() / currDoc->unitRatio();
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
			pp->setOrientation(currDoc->PageOri);
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
			pp->setOrientation(currDoc->PageOri);
		}
		if (tabPage->marginGroup->getMarginsForAllMasterPages())
		{
			pp->initialMargins=updatedMargins;
			pp->marginPreset=currDoc->marginPreset;
		}
		pp->setXOffset(currDoc->scratch()->Left);
		pp->setYOffset(currDoc->scratch()->Top);
	}
	uint docItemsCount = currDoc->MasterItems.count();
	for (uint ite = 0; ite < docItemsCount; ++ite)
	{
		PageItem *item = currDoc->MasterItems.at(ite);
		item->moveBy(LeftD, TopD);
		item->setRedrawBounding();
	}
	currDoc->guidesSettings.guidePlacement = tabGuides->inBackground->isChecked();
	currDoc->marginColored = tabView->checkUnprintable->isChecked();
	currDoc->setPaperColor(tabView->colorPaper);
	currDoc->guidesSettings.marginsShown = tabGuides->marginBox->isChecked();
	currDoc->guidesSettings.showBleed = tabView->checkBleed->isChecked();
	currDoc->guidesSettings.framesShown = tabView->checkFrame->isChecked();
	currDoc->guidesSettings.layerMarkersShown = tabView->checkLayerM->isChecked();
	currDoc->guidesSettings.gridShown = tabGuides->checkGrid->isChecked();
	currDoc->guidesSettings.guidesShown = tabGuides->guideBox->isChecked();
	currDoc->guidesSettings.baselineGridShown = tabGuides->baselineBox->isChecked();
	currDoc->guidesSettings.showPic = tabView->checkPictures->isChecked();
	currDoc->guidesSettings.linkShown = tabView->checkLink->isChecked();
	currDoc->guidesSettings.showControls = tabView->checkControl->isChecked();
	currDoc->guidesSettings.rulerMode = tabView->checkRuler->isChecked();
	currDoc->guidesSettings.grabRadius = tabGuides->grabDistance->value();
	currDoc->guidesSettings.guideRad = tabGuides->snapDistance->value();
	currDoc->guidesSettings.minorGridSpacing = tabGuides->minorSpace->value() / currDoc->unitRatio();
	currDoc->guidesSettings.majorGridSpacing = tabGuides->majorSpace->value() / currDoc->unitRatio();
	currDoc->guidesSettings.minorGridColor = tabGuides->colorMinorGrid;
	currDoc->guidesSettings.majorGridColor = tabGuides->colorMajorGrid;
	currDoc->guidesSettings.marginColor = tabGuides->colorMargin;
	currDoc->guidesSettings.guideColor = tabGuides->colorGuides;
	currDoc->guidesSettings.baselineGridColor = tabGuides->colorBaselineGrid;
	currDoc->checkerProfiles = tabDocChecker->checkerProfile;
	currDoc->curCheckProfile = tabDocChecker->curCheckProfile->currentText();
	currDoc->typographicSettings.valueSuperScript = tabTypo->superDisplacement->value();
	currDoc->typographicSettings.scalingSuperScript = tabTypo->superScaling->value();
	currDoc->typographicSettings.valueSubScript = tabTypo->subDisplacement->value();
	currDoc->typographicSettings.scalingSubScript = tabTypo->subScaling->value();
	currDoc->typographicSettings.valueSmallCaps = tabTypo->capsScaling->value();
	currDoc->typographicSettings.autoLineSpacing = tabTypo->autoLine->value();
	currDoc->guidesSettings.valueBaselineGrid = tabGuides->baseGrid->value(); // / currDoc->unitRatio();
	currDoc->guidesSettings.offsetBaselineGrid = tabGuides->baseOffset->value(); // / currDoc->unitRatio();
	currDoc->typographicSettings.valueUnderlinePos = qRound(tabTypo->underlinePos->value() * 10);
	currDoc->typographicSettings.valueUnderlineWidth = qRound(tabTypo->underlineWidth->value() * 10);
	currDoc->typographicSettings.valueStrikeThruPos = qRound(tabTypo->strikethruPos->value() * 10);
	currDoc->typographicSettings.valueStrikeThruWidth = qRound(tabTypo->strikethruWidth->value() * 10);
	currDoc->itemToolPrefs.textFont = tabTools->fontComboText->currentText();
	currDoc->itemToolPrefs.textSize = tabTools->sizeComboText->currentText().left(2).toInt() * 10;
	currDoc->itemToolPrefs.textStrokeColor = tabTools->colorComboStrokeText->currentText();
	switch (tabTools->tabFillCombo->currentIndex())
	{
		case 0:
			currDoc->itemToolPrefs.textTabFillChar = "";
			break;
		case 1:
			currDoc->itemToolPrefs.textTabFillChar = ".";
			break;
		case 2:
			currDoc->itemToolPrefs.textTabFillChar = "-";
			break;
		case 3:
			currDoc->itemToolPrefs.textTabFillChar = "_";
			break;
		case 4:
			currDoc->itemToolPrefs.textTabFillChar = tabTools->tabFillCombo->currentText().right(1);
			break;
	}
	if (currDoc->itemToolPrefs.textStrokeColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.textStrokeColor = CommonStrings::None;
	currDoc->itemToolPrefs.textColor = tabTools->colorComboText->currentText();
	if (currDoc->itemToolPrefs.textColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.textColor = CommonStrings::None;
	currDoc->itemToolPrefs.textFillColor = tabTools->colorComboTextBackground->currentText();
	if (currDoc->itemToolPrefs.textFillColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.textFillColor = CommonStrings::None;
	currDoc->itemToolPrefs.textLineColor = tabTools->colorComboTextLine->currentText();
	if (currDoc->itemToolPrefs.textLineColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.textLineColor = CommonStrings::None;
	currDoc->itemToolPrefs.textFillColorShade = tabTools->shadingTextBack->value();
	currDoc->itemToolPrefs.textLineColorShade = tabTools->shadingTextLine->value();
	currDoc->itemToolPrefs.textShade = tabTools->shadingText->value();
	currDoc->itemToolPrefs.textStrokeShade = tabTools->shadingTextStroke->value();
	currDoc->itemToolPrefs.textColumns = tabTools->columnsText->value();
	currDoc->itemToolPrefs.textColumnGap = tabTools->gapText->value() / currDoc->unitRatio();
	currDoc->itemToolPrefs.textTabWidth = tabTools->gapTab->value() / currDoc->unitRatio();
	currDoc->itemToolPrefs.shapeLineColor = tabTools->colorComboLineShape->currentText();
	if (currDoc->itemToolPrefs.shapeLineColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.shapeLineColor = CommonStrings::None;
	currDoc->itemToolPrefs.shapeFillColor = tabTools->comboFillShape->currentText();
	if (currDoc->itemToolPrefs.shapeFillColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.shapeFillColor = CommonStrings::None;
	currDoc->itemToolPrefs.shapeFillColorShade = tabTools->shadingFillShape->value();
	currDoc->itemToolPrefs.shapeLineColorShade = tabTools->shadingLineShape->value();
	currDoc->itemToolPrefs.shapeLineStyle = static_cast<Qt::PenStyle>(tabTools->comboStyleShape->currentIndex()) + 1;
	currDoc->itemToolPrefs.shapeLineWidth = tabTools->lineWidthShape->value();
	currDoc->itemToolPrefs.lineStartArrow = tabTools->startArrow->currentIndex();
	currDoc->itemToolPrefs.lineEndArrow = tabTools->endArrow->currentIndex();
	currDoc->opToolPrefs.magMin = tabTools->minimumZoom->value();
	currDoc->opToolPrefs.magMax = tabTools->maximumZoom->value();
	currDoc->opToolPrefs.magStep = tabTools->zoomStep->value();
	currDoc->itemToolPrefs.lineColor = tabTools->colorComboLine->currentText();
	if (currDoc->itemToolPrefs.lineColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.lineColor = CommonStrings::None;
	currDoc->itemToolPrefs.lineColorShade = tabTools->shadingLine->value();
	currDoc->itemToolPrefs.lineStyle = static_cast<Qt::PenStyle>(tabTools->comboStyleLine->currentIndex()) + 1;
	currDoc->itemToolPrefs.lineWidth = tabTools->lineWidthLine->value();
	currDoc->itemToolPrefs.imageFillColor = tabTools->comboFillImage->currentText();
	if (currDoc->itemToolPrefs.imageFillColor == CommonStrings::tr_NoneColor)
		currDoc->itemToolPrefs.imageFillColor = CommonStrings::None;
	currDoc->itemToolPrefs.imageFillColorShade = tabTools->shadingFillImage->value();
	currDoc->itemToolPrefs.imageScaleX = static_cast<double>(tabTools->scalingHorizontal->value()) / 100.0;
	currDoc->itemToolPrefs.imageScaleY = static_cast<double>(tabTools->scalingVertical->value()) / 100.0;
	currDoc->itemToolPrefs.imageScaleType = tabTools->buttonGroup3->isChecked();
	currDoc->itemToolPrefs.imageAspectRatio = tabTools->checkRatioImage->isChecked();
	currDoc->itemToolPrefs.imageUseEmbeddedPath = tabTools->embeddedPath->isChecked();
	int haRes = 0;
	if (tabTools->checkFullRes->isChecked())
		haRes = 0;
	if (tabTools->checkNormalRes->isChecked())
		haRes = 1;
	if (tabTools->checkHalfRes->isChecked())
		haRes = 2;
	if (currDoc->itemToolPrefs.imageLowResType != haRes)
	{
		currDoc->itemToolPrefs.imageLowResType = haRes;
		viewToRecalcPictureRes=true;
	}
	else
		viewToRecalcPictureRes=false;
	currDoc->opToolPrefs.dispX = tabTools->genDispX->value();
	currDoc->opToolPrefs.dispY = tabTools->genDispY->value();
	currDoc->opToolPrefs.constrain = tabTools->genRot->value();
	tabTools->polyWidget->getValues(&currDoc->itemToolPrefs.polyCorners,
									&currDoc->itemToolPrefs.polyFactorGuiVal,
									&currDoc->itemToolPrefs.polyFactor,
									&currDoc->itemToolPrefs.polyUseFactor,
									&currDoc->itemToolPrefs.polyRotation,
									&currDoc->itemToolPrefs.polyCurvature);
	currDoc->setAutoSave(tabPage->GroupAS->isChecked());
	currDoc->setAutoSaveTime(tabPage->ASTime->value() * 60 * 1000);
	currDoc->autoSaveTimer->stop();
	if (currDoc->autoSave())
		currDoc->autoSaveTimer->start(currDoc->autoSaveTime());

/*	FIXME: scribus determines dict by charstyle now, so this setting should go into the doc's default charstyle
		currDoc->docHyphenator->slotNewDict(ScMW->GetLang(tabHyphenator->language->currentText()));
*/
	currDoc->setHyphLanguage(tabHyphenator->getLanguage());
	
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
				currDoc->pdfOptions().SComp = currDoc->CMSSettings.ComponentsInput2;
				currDoc->pdfOptions().SolidProf = currDoc->CMSSettings.DefaultSolidColorRGBProfile;
				currDoc->pdfOptions().ImageProf = currDoc->CMSSettings.DefaultImageRGBProfile;
				currDoc->pdfOptions().PrintProf = currDoc->CMSSettings.DefaultPrinterProfile;
				currDoc->pdfOptions().Intent = currDoc->CMSSettings.DefaultIntentColors;
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
	prefsManager->appPrefs.fontPrefs.GFontSub.clear();
	QMap<QString,QString>::Iterator itfsu;
	QMap<QString,QString>::Iterator itfsuend=tabFonts->RList.end();
	for (itfsu = tabFonts->RList.begin(); itfsu != itfsuend; ++itfsu)
		prefsManager->appPrefs.fontPrefs.GFontSub[itfsu.key()] = tabFonts->FlagsRepl.at(a++)->currentText();
	QStringList uf = currDoc->UsedFonts.keys();
	QMap<QString,int>::Iterator it3;
//	for (it3 = currDoc->UsedFonts.begin(); it3 != currDoc->UsedFonts.end(); ++it3)
//		FT_Done_Face(currDoc->FFonts[it3.key()]);
	currDoc->UsedFonts.clear();
	QStringList::Iterator it3a;
	QStringList::Iterator it3aend=uf.end();
	for (it3a = uf.begin(); it3a != it3aend; ++it3a)
		currDoc->AddFont(*it3a);

	currDoc->pdfOptions().Thumbnails = tabPDF->CheckBox1->isChecked();
	currDoc->pdfOptions().Compress = tabPDF->Compression->isChecked();
	currDoc->pdfOptions().CompressMethod = (PDFOptions::PDFCompression) tabPDF->CMethod->currentIndex();
	currDoc->pdfOptions().Quality = tabPDF->CQuality->currentIndex();
	currDoc->pdfOptions().embedPDF = tabPDF->EmbedPDF->isChecked();
	currDoc->pdfOptions().Resolution = tabPDF->Resolution->value();
	currDoc->pdfOptions().RecalcPic = tabPDF->DSColor->isChecked();
	currDoc->pdfOptions().PicRes = tabPDF->ValC->value();
	currDoc->pdfOptions().Bookmarks = tabPDF->CheckBM->isChecked();
	currDoc->pdfOptions().Binding = tabPDF->ComboBind->currentIndex();
	currDoc->pdfOptions().MirrorH = tabPDF->MirrorH->isChecked();
	currDoc->pdfOptions().MirrorV = tabPDF->MirrorV->isChecked();
	currDoc->pdfOptions().RotateDeg = tabPDF->RotateDeg->currentIndex() * 90;
	currDoc->pdfOptions().Articles = tabPDF->Article->isChecked();
	currDoc->pdfOptions().Encrypt = tabPDF->Encry->isChecked();
	currDoc->pdfOptions().UseLPI = tabPDF->UseLPI->isChecked();
	currDoc->pdfOptions().useLayers = tabPDF->useLayers->isChecked();
	currDoc->pdfOptions().UseSpotColors = !tabPDF->useSpot->isChecked();
	currDoc->pdfOptions().doMultiFile = false;
	currDoc->pdfOptions().bleeds.Bottom = tabPDF->BleedBottom->value() / currDoc->unitRatio();
	currDoc->pdfOptions().bleeds.Top = tabPDF->BleedTop->value() / currDoc->unitRatio();
	currDoc->pdfOptions().bleeds.Left = tabPDF->BleedLeft->value() / currDoc->unitRatio();
	currDoc->pdfOptions().bleeds.Right = tabPDF->BleedRight->value() / currDoc->unitRatio();
	currDoc->pdfOptions().doClip = tabPDF->ClipMarg->isChecked();
	/* DISABLED CR 2006-12-07 Work around #2964
	currDoc->pdfOptions().displayBookmarks = tabPDF->useBookmarks->isChecked();
	currDoc->pdfOptions().displayFullscreen = tabPDF->useFullScreen->isChecked();
	currDoc->pdfOptions().displayLayers = tabPDF->useLayers2->isChecked();
	currDoc->pdfOptions().displayThumbs = tabPDF->useThumbnails->isChecked();
	int pgl = PDFOptions::SinglePage;
	if (tabPDF->singlePage->isChecked())
		pgl = PDFOptions::SinglePage;
	else if (tabPDF->continuousPages->isChecked())
		pgl = PDFOptions::OneColumn;
	else if (tabPDF->doublePageLeft->isChecked())
		pgl = PDFOptions::TwoColumnLeft;
	else if (tabPDF->doublePageRight->isChecked())
		pgl = PDFOptions::TwoColumnRight;
	currDoc->pdfOptions().PageLayout = pgl;
	*/
	currDoc->pdfOptions().openAction = "";
	if (tabPDF->Encry->isChecked())
	{
		int Perm = -64;
		if (tabPDF->PDFVersionCombo->currentIndex() == 1)
			Perm &= ~0x00240000;
		if (tabPDF->PrintSec->isChecked())
			Perm += 4;
		if (tabPDF->ModifySec->isChecked())
			Perm += 8;
		if (tabPDF->CopySec->isChecked())
			Perm += 16;
		if (tabPDF->AddSec->isChecked())
			Perm += 32;
		currDoc->pdfOptions().Permissions = Perm;
		currDoc->pdfOptions().PassOwner = tabPDF->PassOwner->text();
		currDoc->pdfOptions().PassUser = tabPDF->PassUser->text();
	}
	if (tabPDF->PDFVersionCombo->currentIndex() == 0)
		currDoc->pdfOptions().Version = PDFOptions::PDFVersion_13;
	if (tabPDF->PDFVersionCombo->currentIndex() == 1)
		currDoc->pdfOptions().Version = PDFOptions::PDFVersion_14;
	if (tabPDF->PDFVersionCombo->currentIndex() == 2)
		currDoc->pdfOptions().Version = PDFOptions::PDFVersion_15;
	if (tabPDF->PDFVersionCombo->currentIndex() == 3)
		currDoc->pdfOptions().Version = PDFOptions::PDFVersion_X3;
	if (tabPDF->OutCombo->currentIndex() == 0)
	{
		currDoc->pdfOptions().isGrayscale = false;
		currDoc->pdfOptions().UseRGB = true;
		currDoc->pdfOptions().UseProfiles = false;
		currDoc->pdfOptions().UseProfiles2 = false;
	}
	else
	{
		if (tabPDF->OutCombo->currentIndex() == 3)
		{
			currDoc->pdfOptions().isGrayscale = true;
			currDoc->pdfOptions().UseRGB = false;
			currDoc->pdfOptions().UseProfiles = false;
			currDoc->pdfOptions().UseProfiles2 = false;
		}
		else
		{
			currDoc->pdfOptions().isGrayscale = false;
			currDoc->pdfOptions().UseRGB = false;
			if (currDoc->HasCMS)
			{
				currDoc->pdfOptions().UseProfiles = tabPDF->EmbedProfs->isChecked();
				currDoc->pdfOptions().UseProfiles2 = tabPDF->EmbedProfs2->isChecked();
				currDoc->pdfOptions().Intent = tabPDF->IntendS->currentIndex();
				currDoc->pdfOptions().Intent2 = tabPDF->IntendI->currentIndex();
				currDoc->pdfOptions().EmbeddedI = tabPDF->NoEmbedded->isChecked();
				currDoc->pdfOptions().SolidProf = tabPDF->SolidPr->currentText();
				currDoc->pdfOptions().ImageProf = tabPDF->ImageP->currentText();
				currDoc->pdfOptions().PrintProf = tabPDF->PrintProfC->currentText();
			}
		}
	}

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
}

void ReformDoc::applyChangesButton_clicked()
{
	ScMW->docSetup(this);
}
