/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "reformdoc.h"
#include "reformdoc.moc"

#include <qcolordialog.h>
#include <qcursor.h>
#include <qspinbox.h>
#include <qtooltip.h>

#include "docsections.h"
#include "tabtypography.h"
#include "docinfo.h"
#include "tabguides.h"
#include "tabtools.h"
#include "tabcheckdoc.h"
#include "hysettings.h"
#include "cmsprefs.h"
#include "units.h"
#include "mpalette.h"
#include "mspinbox.h"
#include "scribus.h"
#include "scribuscore.h"
#include "tabpdfoptions.h"
#include "fontprefs.h"
#include "units.h"
#include "pagesize.h"
#include "docitemattrprefs.h"
#include "tocindexprefs.h"
#include "marginWidget.h"
#include "page.h"
#include "prefsmanager.h"
#include "pagelayout.h"
#include "colorcombo.h"
#include "fontcombo.h"
#include "linecombo.h"
#include "arrowchooser.h"
#include "polygonwidget.h"
#include "hyphenator.h"
#include "sccombobox.h"
#include "commonstrings.h"
#include "tabdisplay.h"
#include "tabdocument.h"

extern QPixmap loadIcon(QString nam);


ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc ) : PrefsDialogBase( parent )
{
	saveButton->hide();
	docUnitIndex = doc->unitIndex();
	currDoc = doc;
	ScMW = (ScribusMainWindow*)parent;
	unitRatio = doc->unitRatio();
	QString ein = unitGetSuffixFromIndex(docUnitIndex);
	decimals = unitGetDecimalsFromIndex(docUnitIndex);

	setCaption( tr( "Document Setup" ) );
	tabPage = new TabDocument( prefsWidgets, "tab", true );
	tabPage->hideReform();
	addItem( tr("Document"), loadIcon("scribusdoc.png"), tabPage);

	docInfos = new DocInfos(prefsWidgets, doc->documentInfo);
	addItem( tr("Document Information"), loadIcon("documentinfo32.png"), docInfos);

	tabGuides = new TabGuides(prefsWidgets, &doc->guidesSettings, &doc->typographicSettings, docUnitIndex);
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);

	tabView = new TabDisplay( prefsWidgets, "tabView" );
	tabView->hideReform();
	addItem( tr("Display"), loadIcon("screen.png"), tabView);

	tabTypo = new TabTypograpy(  prefsWidgets, &doc->typographicSettings);
	addItem( tr("Typography"), loadIcon("typography.png"), tabTypo);

	tabTools = new TabTools(  prefsWidgets, &doc->toolSettings, docUnitIndex, doc);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ScMW->LangTransl);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	tabFonts = new FontPrefs(prefsWidgets, true, PrefsManager::instance()->preferencesLocation(), doc);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(  prefsWidgets, doc->checkerProfiles, doc->curCheckProfile);
	addItem( tr("Preflight Verifier"), loadIcon("checkdoc.png"), tabDocChecker);

	tabPDF = new TabPDFOptions( prefsWidgets, doc->PDF_Options, PrefsManager::instance()->appPrefs.AvailFonts,
								ScCore->PDFXProfiles, doc->UsedFonts, doc->PDF_Options.PresentVals,
								docUnitIndex, doc->pageHeight, doc->pageWidth, doc );
	addItem( tr("PDF Export"), loadIcon("acroread.png"), tabPDF);

	tabDocItemAttributes = new DocumentItemAttributes( prefsWidgets);
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
	connect(prefsWidgets, SIGNAL(aboutToShow(QWidget *)), this, SLOT(showWidgetInStack(QWidget *)));

	if (ScCore->haveCMS())
	{
		prefsWidgets->raiseWidget(cmsTab);
		connect(tabColorManagement, SIGNAL(cmsOn(bool )), this, SLOT(switchCMS(bool )));
	}

	arrangeIcons();
	resize( minimumSizeHint() );
	clearWState( WState_Polished );
	prefsSelection->setSelected(prefsSelection->firstItem(), true);
	itemSelected(prefsSelection->firstItem());
	restoreDefaults();
}

void ReformDoc::restoreDefaults()
{
	ApplicationPrefs* prefsData=&(PrefsManager::instance()->appPrefs);
	//QWidget* current = prefsWidgets->visibleWidget();
	//if (current == tabPage)
	//{
	tabPage->restoreDefaults(currDoc);
	//}
	//else if (current == tabView)
	//{
	tabView->restoreDefaults(prefsData, &currDoc->guidesSettings);
	//}
	//else if (current == tabHyphenator)
	//{
	tabHyphenator->restoreDefaults(currDoc);
	//}
	//else if (current == tabGuides)
	tabGuides->restoreDefaults(&currDoc->guidesSettings, &currDoc->typographicSettings, docUnitIndex);
	//else if (current == tabTypo)
	tabTypo->restoreDefaults(&currDoc->typographicSettings);
	//else if (current == tabTools)
	tabTools->restoreDefaults(&currDoc->toolSettings, docUnitIndex);
	//else if (current == tabFonts)
	tabFonts->restoreDefaults();
	//else if (current == tabDocChecker)
	tabDocChecker->restoreDefaults(&currDoc->checkerProfiles, currDoc->curCheckProfile);
	//else if (current == tabPDF)
	tabPDF->restoreDefaults(currDoc->PDF_Options, PrefsManager::instance()->appPrefs.AvailFonts,
							ScCore->PDFXProfiles, currDoc->UsedFonts, currDoc->PDF_Options.PresentVals,
							docUnitIndex, currDoc->pageHeight, currDoc->pageWidth, currDoc, false);
	//else if (current == tabColorManagement)
	tabColorManagement->restoreDefaults(&currDoc->CMSSettings, &ScCore->InputProfiles,
										 &ScCore->InputProfilesCMYK,
										 &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
	//else if (current == docInfos)
	docInfos->restoreDefaults();

	unitChange();
}

void ReformDoc::unitChange()
{
	double oldUnitRatio = unitRatio;
	//double oldMin, oldMax, oldB, oldBM, oldH, oldHM, val;
	docUnitIndex = tabPage->unitCombo->currentItem();
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);
	QString suffix = unitGetSuffixFromIndex(docUnitIndex);
	double invUnitConversion = 1.0 / oldUnitRatio * unitRatio;
	
	tabPage->unitChange();
	tabGuides->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
	tabView->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
	tabTools->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
	tabPDF->unitChange(suffix, docUnitIndex, decimals, invUnitConversion);
}

void ReformDoc::setDS(int layout)
{
	tabPage->marginGroup->setFacingPages(!(layout == singlePage));
	tabPage->choosenLayout = layout;
	tabPage->docLayout->firstPage->setCurrentItem(currDoc->pageSets[tabPage->choosenLayout].FirstPage);
	tabView->gapHorizontal->setValue(currDoc->pageSets[tabPage->choosenLayout].GapHorizontal * unitRatio);
	tabView->gapVertical->setValue(currDoc->pageSets[tabPage->choosenLayout].GapBelow * unitRatio);
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
	return tabPage->unitCombo->currentItem();
}

const bool ReformDoc::colorManagementSettingsChanged()
{
	return tabColorManagement->changed;
}

const bool ReformDoc::imageResolutionChanged()
{
	return viewToRecalcPictureRes;
}

void ReformDoc::updateDocumentSettings()
{
	double tpr2, lr2, rr2, br2;
	tpr2 = tabPage->marginGroup->top();
	br2 = tabPage->marginGroup->bottom();
	lr2 = tabPage->marginGroup->left();
	rr2 = tabPage->marginGroup->right();
	int fp = tabPage->choosenLayout;
	currDoc->pageSets[fp].FirstPage = tabPage->docLayout->firstPage->currentItem();
	currDoc->pageSets[fp].GapHorizontal = tabView->gapHorizontal->value() / currDoc->unitRatio();
	currDoc->pageSets[fp].GapBelow = tabView->gapVertical->value() / currDoc->unitRatio();
	//currDoc->FirstPnum = pageNumber->value();
	currDoc->resetPage(tpr2, lr2, rr2, br2, fp);
	currDoc->PageOri = tabPage->pageOrientationComboBox->currentItem();
	currDoc->m_pageSize = tabPage->prefsPageSizeName;
	currDoc->pageWidth = tabPage->pageW;
	currDoc->pageHeight = tabPage->pageH;
	double TopD = tabView->topScratch->value() / currDoc->unitRatio() - currDoc->ScratchTop;
	double LeftD = tabView->leftScratch->value() / currDoc->unitRatio() - currDoc->ScratchLeft;
	currDoc->ScratchBottom = tabView->bottomScratch->value() / currDoc->unitRatio();
	currDoc->ScratchLeft = tabView->leftScratch->value() / currDoc->unitRatio();
	currDoc->ScratchRight = tabView->rightScratch->value() / currDoc->unitRatio();
	currDoc->ScratchTop = tabView->topScratch->value() / currDoc->unitRatio();
	for (uint p = 0; p < currDoc->Pages->count(); ++p)
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
			pp->initialMargins.Left = lr2;
			pp->initialMargins.Right = rr2;
			pp->initialMargins.Top = tpr2;
			pp->initialMargins.Bottom = br2;
		}
	}
	for (uint p = 0; p < currDoc->MasterPages.count(); ++p)
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
			pp->initialMargins.Left = lr2;
			pp->initialMargins.Right = rr2;
			pp->initialMargins.Top = tpr2;
			pp->initialMargins.Bottom = br2;
		}
		pp->setXOffset(currDoc->ScratchLeft);
		pp->setYOffset(currDoc->ScratchTop);
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
	currDoc->guidesSettings.guideRad = tabGuides->snapDistance->value() / currDoc->unitRatio();
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
	currDoc->typographicSettings.valueBaseGrid = tabGuides->baseGrid->value() / currDoc->unitRatio();
	currDoc->typographicSettings.offsetBaseGrid = tabGuides->baseOffset->value() / currDoc->unitRatio();
	currDoc->typographicSettings.valueUnderlinePos = qRound(tabTypo->underlinePos->value() * 10);
	currDoc->typographicSettings.valueUnderlineWidth = qRound(tabTypo->underlineWidth->value() * 10);
	currDoc->typographicSettings.valueStrikeThruPos = qRound(tabTypo->strikethruPos->value() * 10);
	currDoc->typographicSettings.valueStrikeThruWidth = qRound(tabTypo->strikethruWidth->value() * 10);
	currDoc->toolSettings.defFont = tabTools->fontComboText->currentText();
	currDoc->toolSettings.defSize = tabTools->sizeComboText->currentText().left(2).toInt() * 10;
	currDoc->toolSettings.dStrokeText = tabTools->colorComboStrokeText->currentText();
	switch (tabTools->tabFillCombo->currentItem())
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
	if (currDoc->toolSettings.dStrokeText == CommonStrings::NoneColor)
		currDoc->toolSettings.dStrokeText = CommonStrings::None;
	currDoc->toolSettings.dPenText = tabTools->colorComboText->currentText();
	if (currDoc->toolSettings.dPenText == CommonStrings::NoneColor)
		currDoc->toolSettings.dPenText = CommonStrings::None;
	currDoc->toolSettings.dTextBackGround = tabTools->colorComboTextBackground->currentText();
	if (currDoc->toolSettings.dTextBackGround == CommonStrings::NoneColor)
		currDoc->toolSettings.dTextBackGround = CommonStrings::None;
	currDoc->toolSettings.dTextLineColor = tabTools->colorComboTextLine->currentText();
	if (currDoc->toolSettings.dTextLineColor == CommonStrings::NoneColor)
		currDoc->toolSettings.dTextLineColor = CommonStrings::None;
	currDoc->toolSettings.dTextBackGroundShade = tabTools->shadingTextBack->value();
	currDoc->toolSettings.dTextLineShade = tabTools->shadingTextLine->value();
	currDoc->toolSettings.dTextPenShade = tabTools->shadingText->value();
	currDoc->toolSettings.dTextStrokeShade = tabTools->shadingTextStroke->value();
	currDoc->toolSettings.dCols = tabTools->columnsText->value();
	currDoc->toolSettings.dGap = tabTools->gapText->value() / currDoc->unitRatio();
	currDoc->toolSettings.dTabWidth = tabTools->gapTab->value() / currDoc->unitRatio();
	currDoc->toolSettings.dPen = tabTools->colorComboLineShape->currentText();
	if (currDoc->toolSettings.dPen == CommonStrings::NoneColor)
		currDoc->toolSettings.dPen = CommonStrings::None;
	currDoc->toolSettings.dBrush = tabTools->comboFillShape->currentText();
	if (currDoc->toolSettings.dBrush == CommonStrings::NoneColor)
		currDoc->toolSettings.dBrush = CommonStrings::None;
	currDoc->toolSettings.dShade = tabTools->shadingFillShape->value();
	currDoc->toolSettings.dShade2 = tabTools->shadingLineShape->value();
	switch (tabTools->comboStyleShape->currentItem())
	{
	case 0:
		currDoc->toolSettings.dLineArt = SolidLine;
		break;
	case 1:
		currDoc->toolSettings.dLineArt = DashLine;
		break;
	case 2:
		currDoc->toolSettings.dLineArt = DotLine;
		break;
	case 3:
		currDoc->toolSettings.dLineArt = DashDotLine;
		break;
	case 4:
		currDoc->toolSettings.dLineArt = DashDotDotLine;
		break;
	}
	currDoc->toolSettings.dWidth = tabTools->lineWidthShape->value();
	currDoc->toolSettings.dStartArrow = tabTools->startArrow->currentItem();
	currDoc->toolSettings.dEndArrow = tabTools->endArrow->currentItem();
	currDoc->toolSettings.magMin = tabTools->minimumZoom->value();
	currDoc->toolSettings.magMax = tabTools->maximumZoom->value();
	currDoc->toolSettings.magStep = tabTools->zoomStep->value();
	currDoc->toolSettings.dPenLine = tabTools->colorComboLine->currentText();
	if (currDoc->toolSettings.dPenLine == CommonStrings::NoneColor)
		currDoc->toolSettings.dPenLine = CommonStrings::None;
	currDoc->toolSettings.dShadeLine = tabTools->shadingLine->value();
	switch (tabTools->comboStyleLine->currentItem())
	{
	case 0:
		currDoc->toolSettings.dLstyleLine = SolidLine;
		break;
	case 1:
		currDoc->toolSettings.dLstyleLine = DashLine;
		break;
	case 2:
		currDoc->toolSettings.dLstyleLine = DotLine;
		break;
	case 3:
		currDoc->toolSettings.dLstyleLine = DashDotLine;
		break;
	case 4:
		currDoc->toolSettings.dLstyleLine = DashDotDotLine;
		break;
	}
	currDoc->toolSettings.dWidthLine = tabTools->lineWidthLine->value();
	currDoc->toolSettings.dBrushPict = tabTools->comboFillImage->currentText();
	if (currDoc->toolSettings.dBrushPict == CommonStrings::NoneColor)
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
	tabTools->polyWidget->getValues(&currDoc->toolSettings.polyC, &currDoc->toolSettings.polyFd, &currDoc->toolSettings.polyF, &currDoc->toolSettings.polyS, &currDoc->toolSettings.polyR);
	currDoc->AutoSave = tabPage->GroupAS->isChecked();
	currDoc->AutoSaveTime = tabPage->ASTime->value() * 60 * 1000;
	if (currDoc->AutoSave)
	{
		currDoc->autoSaveTimer->stop();
		currDoc->autoSaveTimer->start(currDoc->AutoSaveTime);
	}
	currDoc->docHyphenator->slotNewDict(ScMW->GetLang(tabHyphenator->language->currentText()));
	currDoc->docHyphenator->slotNewSettings(tabHyphenator->wordLen->value(),
																!tabHyphenator->verbose->isChecked(),
																tabHyphenator->input->isChecked(),
																tabHyphenator->maxCount->value());
	if (ScCore->haveCMS())
	{
		bool oldCM = currDoc->CMSSettings.CMSinUse;
		tabColorManagement->updateDocSettings(currDoc);
		if (tabColorManagement->changed)
		{
			ScMW->setStatusBarInfoText( tr("Adjusting Colors"));
			ScMW->mainWindowProgressBar->reset();
			int cc = currDoc->PageColors.count() + currDoc->Items->count();
			ScMW->mainWindowProgressBar->setTotalSteps(cc);
#ifdef HAVE_CMS
			currDoc->HasCMS = currDoc->CMSSettings.CMSinUse;
			currDoc->SoftProofing = currDoc->CMSSettings.SoftProofOn;
			currDoc->Gamut = currDoc->CMSSettings.GamutCheck;
			currDoc->IntentColors = currDoc->CMSSettings.DefaultIntentColors;
			currDoc->IntentImages = currDoc->CMSSettings.DefaultIntentImages;
			qApp->setOverrideCursor(QCursor(waitCursor), true);
			bool newCM = currDoc->CMSSettings.CMSinUse;
			currDoc->CMSSettings.CMSinUse = oldCM;
			currDoc->CloseCMSProfiles();
			currDoc->CMSSettings.CMSinUse = newCM;
			if (!currDoc->CMSSettings.CMSinUse)
			{
				currDoc->HasCMS = false;
				if	(oldCM)
				{
					ScMW->recalcColors(ScMW->mainWindowProgressBar);
					currDoc->RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, ScMW->mainWindowProgressBar);
				}
			}
			else if ( currDoc->OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles) )
			{
				currDoc->HasCMS = true;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputRGBProf)) == icSigRgbData)
					currDoc->CMSSettings.ComponentsInput2 = 3;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputRGBProf)) == icSigCmykData)
					currDoc->CMSSettings.ComponentsInput2 = 4;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputRGBProf)) == icSigCmyData)
					currDoc->CMSSettings.ComponentsInput2 = 3;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputCMYKProf)) == icSigRgbData)
					currDoc->CMSSettings.ComponentsInput3 = 3;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputCMYKProf)) == icSigCmykData)
					currDoc->CMSSettings.ComponentsInput3 = 4;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputCMYKProf)) == icSigCmyData)
					currDoc->CMSSettings.ComponentsInput3 = 3;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocPrinterProf)) == icSigRgbData)
					currDoc->CMSSettings.ComponentsPrinter = 3;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocPrinterProf)) == icSigCmykData)
					currDoc->CMSSettings.ComponentsPrinter = 4;
				if (static_cast<int>(cmsGetColorSpace(currDoc->DocPrinterProf)) == icSigCmyData)
					currDoc->CMSSettings.ComponentsPrinter = 3;
				currDoc->PDF_Options.SComp = currDoc->CMSSettings.ComponentsInput2;
				currDoc->PDF_Options.SolidProf = currDoc->CMSSettings.DefaultSolidColorRGBProfile;
				currDoc->PDF_Options.ImageProf = currDoc->CMSSettings.DefaultImageRGBProfile;
				currDoc->PDF_Options.PrintProf = currDoc->CMSSettings.DefaultPrinterProfile;
				currDoc->PDF_Options.Intent = currDoc->CMSSettings.DefaultIntentColors;
				ScMW->recalcColors(ScMW->mainWindowProgressBar);
				currDoc->RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, ScMW->mainWindowProgressBar);
			}
			else
				currDoc->HasCMS = false;
#endif
			ScMW->mainWindowProgressBar->setProgress(cc);
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			ScMW->setStatusBarInfoText("");
			ScMW->mainWindowProgressBar->reset();
		}
	}
	PrefsManager* prefsManager=PrefsManager::instance();
	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
	for ( ; it.hasNext() ; it.next())
	{
		it.current().embedPs(tabFonts->fontFlags[it.currentKey()].FlagPS);
		it.current().usable(tabFonts->fontFlags[it.currentKey()].FlagUse);
		it.current().subset(tabFonts->fontFlags[it.currentKey()].FlagSub);
	}
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

	currDoc->PDF_Options.Thumbnails = tabPDF->CheckBox1->isChecked();
	currDoc->PDF_Options.Compress = tabPDF->Compression->isChecked();
	currDoc->PDF_Options.CompressMethod = tabPDF->CMethod->currentItem();
	currDoc->PDF_Options.Quality = tabPDF->CQuality->currentItem();
	currDoc->PDF_Options.Resolution = tabPDF->Resolution->value();
	currDoc->PDF_Options.RecalcPic = tabPDF->DSColor->isChecked();
	currDoc->PDF_Options.PicRes = tabPDF->ValC->value();
	currDoc->PDF_Options.Bookmarks = tabPDF->CheckBM->isChecked();
	currDoc->PDF_Options.Binding = tabPDF->ComboBind->currentItem();
	currDoc->PDF_Options.MirrorH = tabPDF->MirrorH->isOn();
	currDoc->PDF_Options.MirrorV = tabPDF->MirrorV->isOn();
	currDoc->PDF_Options.RotateDeg = tabPDF->RotateDeg->currentItem() * 90;
	currDoc->PDF_Options.Articles = tabPDF->Article->isChecked();
	currDoc->PDF_Options.Encrypt = tabPDF->Encry->isChecked();
	currDoc->PDF_Options.UseLPI = tabPDF->UseLPI->isChecked();
	currDoc->PDF_Options.useLayers = tabPDF->useLayers->isChecked();
	currDoc->PDF_Options.UseSpotColors = !tabPDF->useSpot->isChecked();
	currDoc->PDF_Options.doOverprint = tabPDF->overprintMode->isChecked();
	currDoc->PDF_Options.doMultiFile = false;
	currDoc->PDF_Options.BleedBottom = tabPDF->BleedBottom->value() / currDoc->unitRatio();
	currDoc->PDF_Options.BleedTop = tabPDF->BleedTop->value() / currDoc->unitRatio();
	currDoc->PDF_Options.BleedLeft = tabPDF->BleedLeft->value() / currDoc->unitRatio();
	currDoc->PDF_Options.BleedRight = tabPDF->BleedRight->value() / currDoc->unitRatio();
	currDoc->PDF_Options.doClip = tabPDF->ClipMarg->isChecked();
	/* DISABLED CR 2006-12-07 Work around #2964
	currDoc->PDF_Options.displayBookmarks = tabPDF->useBookmarks->isChecked();
	currDoc->PDF_Options.displayFullscreen = tabPDF->useFullScreen->isChecked();
	currDoc->PDF_Options.displayLayers = tabPDF->useLayers2->isChecked();
	currDoc->PDF_Options.displayThumbs = tabPDF->useThumbnails->isChecked();
	int pgl = PDFOptions::SinglePage;
	if (tabPDF->singlePage->isChecked())
		pgl = PDFOptions::SinglePage;
	else if (tabPDF->continuousPages->isChecked())
		pgl = PDFOptions::OneColumn;
	else if (tabPDF->doublePageLeft->isChecked())
		pgl = PDFOptions::TwoColumnLeft;
	else if (tabPDF->doublePageRight->isChecked())
		pgl = PDFOptions::TwoColumnRight;
	currDoc->PDF_Options.PageLayout = pgl;
	*/
	currDoc->PDF_Options.openAction = "";
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
		currDoc->PDF_Options.Permissions = Perm;
		currDoc->PDF_Options.PassOwner = tabPDF->PassOwner->text();
		currDoc->PDF_Options.PassUser = tabPDF->PassUser->text();
	}
	if (tabPDF->PDFVersionCombo->currentItem() == 0)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_13;
	if (tabPDF->PDFVersionCombo->currentItem() == 1)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_14;
	if (tabPDF->PDFVersionCombo->currentItem() == 2)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_15;
	if (tabPDF->PDFVersionCombo->currentItem() == 3)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_X3;
	if (tabPDF->OutCombo->currentItem() == 0)
	{
		currDoc->PDF_Options.isGrayscale = false;
		currDoc->PDF_Options.UseRGB = true;
		currDoc->PDF_Options.UseProfiles = false;
		currDoc->PDF_Options.UseProfiles2 = false;
	}
	else
	{
		if (tabPDF->OutCombo->currentItem() == 3)
		{
			currDoc->PDF_Options.isGrayscale = true;
			currDoc->PDF_Options.UseRGB = false;
			currDoc->PDF_Options.UseProfiles = false;
			currDoc->PDF_Options.UseProfiles2 = false;
		}
		else
		{
			currDoc->PDF_Options.isGrayscale = false;
			currDoc->PDF_Options.UseRGB = false;
#ifdef HAVE_CMS
			if (currDoc->HasCMS)
			{
				currDoc->PDF_Options.UseProfiles = tabPDF->EmbedProfs->isChecked();
				currDoc->PDF_Options.UseProfiles2 = tabPDF->EmbedProfs2->isChecked();
				currDoc->PDF_Options.Intent = tabPDF->IntendS->currentItem();
				currDoc->PDF_Options.Intent2 = tabPDF->IntendI->currentItem();
				currDoc->PDF_Options.EmbeddedI = tabPDF->NoEmbedded->isChecked();
				currDoc->PDF_Options.SolidProf = tabPDF->SolidPr->currentText();
				currDoc->PDF_Options.ImageProf = tabPDF->ImageP->currentText();
				currDoc->PDF_Options.PrintProf = tabPDF->PrintProfC->currentText();
			}
#endif
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
