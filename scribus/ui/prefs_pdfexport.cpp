/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_pdfexport.h"
#include "ui/createrange.h"
#include "prefsstructs.h"

#include "scribuscore.h"
#include "scribusdoc.h"
#include "ui/scrspinbox.h"
#include "units.h"
#include "util_icon.h"

Prefs_PDFExport::Prefs_PDFExport(QWidget* parent)
	: Prefs_Pane(parent),
	cmsEnabled(false),
	m_doc(NULL)
{
	setupUi(this);
	pageMirrorHorizontalToolButton->setIcon(QIcon(loadIcon("16/flip-object-horizontal.png")));
	pageMirrorVerticalToolButton->setIcon(QIcon(loadIcon("16/flip-object-vertical.png")));
	unitChange(0);
	languageChange();

	connect(exportChosenPagesRadioButton, SIGNAL(toggled(bool)), this, SLOT(enableRangeControls(bool)));
	connect(exportRangeMorePushButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect(maxResolutionLimitCheckBox, SIGNAL(clicked()), this, SLOT(setMaximumResolution()));
//	connect(MirrorH, SIGNAL(clicked()), this, SLOT(PDFMirror()));
//	connect(MirrorV, SIGNAL(clicked()), this, SLOT(PDFMirror()));
//	connect(RotateDeg, SIGNAL(activated(int)), this, SLOT(Rotation(int)));
	connect(outputIntentionComboBox, SIGNAL(activated(int)), this, SLOT(enableProfiles(int)));
	connect(useSolidColorProfileCheckBox, SIGNAL(clicked()), this, SLOT(enablePG()));
	connect(useImageProfileCheckBox, SIGNAL(clicked()), this, SLOT(enablePGI()));
	connect(doNotUseEmbeddedImageProfileCheckBox, SIGNAL(clicked()), this, SLOT(enablePGI2()));
	connect(pdfVersionComboBox, SIGNAL(activated(int)), this, SLOT(enablePDFX(int)));
	connect(useEncryptionCheckBox, SIGNAL(clicked(bool)), this, SLOT(enableSecurityControls(bool)));
}

Prefs_PDFExport::~Prefs_PDFExport()
{
}


void Prefs_PDFExport::unitChange(int unitIndex)
{
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	registrationMarkOffsetSpinBox->setNewUnit(unitIndex);
	registrationMarkOffsetSpinBox->setMinimum(0);
	registrationMarkOffsetSpinBox->setMaximum(3000 * unitRatio);
}

void Prefs_PDFExport::languageChange()
{
	int i=0;
	i = rotationComboBox->currentIndex();
	rotationComboBox->clear();
	rotationComboBox->addItem(QString::fromUtf8("0 °"));
	rotationComboBox->addItem(QString::fromUtf8("90 °"));
	rotationComboBox->addItem(QString::fromUtf8("180 °"));
	rotationComboBox->addItem(QString::fromUtf8("270 °"));
	rotationComboBox->setCurrentIndex(i);

	i = imageCompressionMethodComboBox->currentIndex();
	imageCompressionMethodComboBox->clear();
	imageCompressionMethodComboBox->addItem( tr( "Automatic" ) );
	imageCompressionMethodComboBox->addItem( tr( "Lossy - JPEG" ) );
	imageCompressionMethodComboBox->addItem( tr( "Lossless - Zip" ) );
	imageCompressionMethodComboBox->addItem( tr( "None" ) );
	imageCompressionMethodComboBox->setCurrentIndex(i);

	i = imageCompressionQualityComboBox->currentIndex();
	imageCompressionQualityComboBox->clear();
	imageCompressionQualityComboBox->addItem( tr( "Maximum" ) );
	imageCompressionQualityComboBox->addItem( tr( "High" ) );
	imageCompressionQualityComboBox->addItem( tr( "Medium" ) );
	imageCompressionQualityComboBox->addItem( tr( "Low" ) );
	imageCompressionQualityComboBox->addItem( tr( "Minimum" ) );
	imageCompressionQualityComboBox->setCurrentIndex(i);

	addPDFVersions(true);//if (cmsEnabled)// && (!PDFXProfiles.isEmpty()))

	i = pageBindingComboBox->currentIndex();
	pageBindingComboBox->clear();
	pageBindingComboBox->addItem( tr("Left Margin"));
	pageBindingComboBox->addItem( tr("Right Margin"));
	pageBindingComboBox->setCurrentIndex(i);

	i = outputIntentionComboBox->currentIndex();
	outputIntentionComboBox->clear();
	outputIntentionComboBox->addItem( tr( "Screen / Web" ) );
	outputIntentionComboBox->addItem( tr( "Printer" ) );
	outputIntentionComboBox->addItem( tr( "Grayscale" ) );
	outputIntentionComboBox->setCurrentIndex(i);

	i = customRenderingSpotFunctionComboBox->currentIndex();
	customRenderingSpotFunctionComboBox->clear();
	customRenderingSpotFunctionComboBox->addItem( tr( "Simple Dot" ) );
	customRenderingSpotFunctionComboBox->addItem( tr( "Line" ) );
	customRenderingSpotFunctionComboBox->addItem( tr( "Round" ) );
	customRenderingSpotFunctionComboBox->addItem( tr( "Ellipse" ) );
	customRenderingSpotFunctionComboBox->setCurrentIndex(i);

	i = solidColorRenderingIntentComboBox->currentIndex();
	int j=imageRenderingIntentComboBox->currentIndex();
	solidColorRenderingIntentComboBox->clear();
	imageRenderingIntentComboBox->clear();
	QString tmp_ip[] = { tr("Perceptual"), tr("Relative Colorimetric"), tr("Saturation"), tr("Absolute Colorimetric")};
	size_t ar_ip = sizeof(tmp_ip) / sizeof(*tmp_ip);
	for (uint a = 0; a < ar_ip; ++a)
		solidColorRenderingIntentComboBox->addItem(tmp_ip[a]);
	for (uint a = 0; a < ar_ip; ++a)
		imageRenderingIntentComboBox->addItem(tmp_ip[a]);
	solidColorRenderingIntentComboBox->setCurrentIndex(i);
	imageRenderingIntentComboBox->setCurrentIndex(j);
}

void Prefs_PDFExport::restoreDefaults(struct ApplicationPrefs *prefsData)
{
}

void Prefs_PDFExport::restoreDefaults(struct ApplicationPrefs *prefsData, ScribusDoc* doc, const ProfilesL & PDFXProfiles,
									  const QMap<QString, int> & DocFonts)
{
	m_doc=doc;
	int unitIndex = prefsData->docSetupPrefs.docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	unitChange(unitIndex);
	Opts=prefsData->pdfPrefs;
	defaultSolidColorRGBProfile=prefsData->colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
	defaultPrinterProfile=prefsData->colorPrefs.DCMSset.DefaultPrinterProfile;
	exportAllPagesRadioButton->setChecked(true);
	enableRangeControls(false);
	rotationComboBox->setCurrentIndex(prefsData->pdfPrefs.RotateDeg / 90);
	pageMirrorHorizontalToolButton->setChecked(prefsData->pdfPrefs.MirrorH);
	pageMirrorVerticalToolButton->setChecked(prefsData->pdfPrefs.MirrorV);
	clipToPrinterMarginsCheckBox->setChecked(prefsData->pdfPrefs.doClip);
	bool cmsUse = m_doc ? (ScCore->haveCMS() && m_doc->HasCMS) : false;
	int newCMSIndex=0;
	if (cmsUse)
	{
		if (prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_X1a)
			newCMSIndex=3;
		if (prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_X3)
			newCMSIndex=4;
		if (prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_X4)
			newCMSIndex=5;
	}
	if (prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_13)
		newCMSIndex=0;
	if (prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_14)
		newCMSIndex=1;
	if (prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_15)
		newCMSIndex=2;
	pdfVersionComboBox->setCurrentIndex(newCMSIndex);
	pageBindingComboBox->setCurrentIndex(prefsData->pdfPrefs.Binding);
	generateThumbnailsCheckBox->setChecked(prefsData->pdfPrefs.Thumbnails);
	saveLinkedTextFramesAsArticlesCheckBox->setChecked(prefsData->pdfPrefs.Articles);
	includeBookmarksCheckBox->setChecked(prefsData->pdfPrefs.Bookmarks);
	includeLayersCheckBox->setChecked(prefsData->pdfPrefs.useLayers);
	includeLayersCheckBox->setEnabled(prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_15 || prefsData->pdfPrefs.Version == PDFOptions::PDFVersion_X4);

	epsExportResolutionSpinBox->setValue(prefsData->pdfPrefs.Resolution);
	embedPDFAndEPSFiles->setChecked(prefsData->pdfPrefs.embedPDF);
	compressTextAndVectorGraphicsCheckBox->setChecked( prefsData->pdfPrefs.Compress );
	imageCompressionMethodComboBox->setCurrentIndex(prefsData->pdfPrefs.CompressMethod);
	imageCompressionQualityComboBox->setCurrentIndex(prefsData->pdfPrefs.Quality);
	maxResolutionLimitCheckBox->setChecked(prefsData->pdfPrefs.RecalcPic);
	maxExportResolutionSpinBox->setValue(prefsData->pdfPrefs.PicRes);
	maxExportResolutionSpinBox->setEnabled(prefsData->pdfPrefs.RecalcPic);
	/*
if (mdoc != 0 && exporting)
	{
//	Build a list of all Fonts used in Annotations;
		PageItem *pgit;
		for (int c = 0; c < doc->FrameItems.count(); ++c)
		{
			pgit = doc->FrameItems.at(c);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()) && (pgit->itemText.length() > 0))
				AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
		}
		for (int c = 0; c < doc->MasterItems.count(); ++c)
		{
			pgit = doc->MasterItems.at(c);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()) && (pgit->itemText.length() > 0))
				AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
		}
		for (int c = 0; c < doc->DocItems.count(); ++c)
		{
			pgit = doc->DocItems.at(c);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()) && (pgit->itemText.length() > 0))
				AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
		}
		QMap<QString,int>::const_iterator it;
		AvailFlist->clear();
		for (it = DocFonts.constBegin(); it != DocFonts.constEnd(); ++it)
		{
			if (AllFonts[it.key()].isReplacement())
				new QListWidgetItem( QIcon(loadIcon("font_subst16.png")), it.key(), AvailFlist );
			else if (AllFonts[it.key()].type() == ScFace::TYPE1)
				new QListWidgetItem( QIcon(loadIcon("font_type1_16.png")), it.key(), AvailFlist );
			else if (AllFonts[it.key()].type() == ScFace::TTF)
				new QListWidgetItem( QIcon(loadIcon("font_truetype16.png")), it.key(), AvailFlist );
			else if (AllFonts[it.key()].type() == ScFace::OTF)
				new QListWidgetItem( QIcon(loadIcon("font_otf16.png")), it.key(), AvailFlist );
		}
		ToEmbed->setEnabled(false);
		FromEmbed->setEnabled(false);
		ToOutline->setEnabled(false);
		FromOutline->setEnabled(false);
		if ((Opts.EmbedList.count() == 0) && (Opts.SubsetList.count() == 0) && (Opts.firstUse))
			EmbedAll();
		else
		{
			EmbedList->clear();
			FontsToEmbed.clear();
			for (int fe = 0; fe < Opts.EmbedList.count(); ++fe)
			{
				EmbedList->addItem(Opts.EmbedList[fe]);
				FontsToEmbed.append(Opts.EmbedList[fe]);
			}
			if (Opts.SubsetList.count() != 0)
			{
				OutlineList->clear();
				FontsToOutline.clear();
				for (int fe = 0; fe < Opts.SubsetList.count(); ++fe)
				{
					OutlineList->addItem(Opts.SubsetList[fe]);
					FontsToOutline.append(Opts.SubsetList[fe]);
				}
			}
			QMap<QString, QString>::Iterator itAnn;
			for (itAnn = AnnotationFonts.begin(); itAnn != AnnotationFonts.end(); ++itAnn)
			{
				if (FontsToEmbed.contains(itAnn.key()) == 0)
				{
					EmbedList->addItem(itAnn.key());
					EmbedList->item(EmbedList->count()-1)->setFlags(Qt::ItemIsEnabled);
					FontsToEmbed.append(itAnn.key());
				}
				if (FontsToOutline.contains(itAnn.key()) != 0)
				{
					FontsToOutline.removeAll(itAnn.key());
					QList<QListWidgetItem *> itR = OutlineList->findItems(itAnn.key(), Qt::MatchExactly);
					delete OutlineList->takeItem(OutlineList->row(itR.at(0)));
				}
			}
		}
		CheckBox10->setChecked(Opts.PresentMode);
		PagePrev->setChecked(false);
		Pages->clear();
		QString tmp;
		struct PDFPresentationData ef;
		if (EffVal.count() != 0)
		{
			for (int pg2 = 0; pg2 < doc->Pages->count(); ++pg2)
			{
				Pages->addItem( tr("Page")+" "+tmp.setNum(pg2+1));
				if (EffVal.count()-1 < pg2)
				{
					ef.pageEffectDuration = 1;
					ef.pageViewDuration = 1;
					ef.effectType = 0;
					ef.Dm = 0;
					ef.M = 0;
					ef.Di = 0;
					EffVal.append(ef);
				}
			}
		}
		else
		{
			for (int pg = 0; pg < doc->Pages->count(); ++pg)
			{
				Pages->addItem( tr("Page")+" "+tmp.setNum(pg+1));
				ef.pageEffectDuration = 1;
				ef.pageViewDuration = 1;
				ef.effectType = 0;
				ef.Dm = 0;
				ef.M = 0;
				ef.Di = 0;
				EffVal.append(ef);
			}
		}
		PageTime->setValue(EffVal[0].pageViewDuration);
		EffectTime->setValue(EffVal[0].pageEffectDuration);
		bool df = true;
		if ((Opts.displayBookmarks) || (Opts.displayFullscreen) || (Opts.displayLayers) || (Opts.displayThumbs))
			df = false;
		if (df)
			useViewDefault->setChecked(df);
		useFullScreen->setChecked(Opts.displayFullscreen);
		useBookmarks->setChecked(Opts.displayBookmarks);
		useThumbnails->setChecked(Opts.displayThumbs);
		useLayers2->setChecked(Opts.displayLayers);
		hideToolBar->setChecked(Opts.hideToolBar);
		hideMenuBar->setChecked(Opts.hideMenuBar);
		fitWindow->setChecked(Opts.fitWindow);
		QMap<QString,QString>::Iterator itja;
		actionCombo->clear();
		actionCombo->addItem( tr("No Script"));
		for (itja = doc->JavaScripts.begin(); itja != doc->JavaScripts.end(); ++itja)
			actionCombo->addItem(itja.key());
		if (doc->JavaScripts.contains(Opts.openAction))
			setCurrentComboItem(actionCombo, Opts.openAction);
		if (Opts.PageLayout == PDFOptions::SinglePage)
			singlePage->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::OneColumn)
			continuousPages->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::TwoColumnLeft)
			doublePageLeft->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::TwoColumnRight)
			doublePageRight->setChecked(true);
		if ((Opts.Version == PDFOptions::PDFVersion_15) || (Opts.Version == PDFOptions::PDFVersion_X4))
			useLayers2->setEnabled(true);
		else
			useLayers2->setEnabled(false);
	}
	*/
	useEncryptionCheckBox->setChecked( prefsData->pdfPrefs.Encrypt );
	passwordOwnerLineEdit->setText(prefsData->pdfPrefs.PassOwner);
	passwordUserLineEdit->setText(prefsData->pdfPrefs.PassUser);
	allowPrintingCheckBox->setChecked( prefsData->pdfPrefs.Permissions & 4 );
	allowChangingCheckBox->setChecked( prefsData->pdfPrefs.Permissions & 8 );
	allowCopyingCheckBox->setChecked( prefsData->pdfPrefs.Permissions & 16 );
	allowAnnotatingCheckBox->setChecked( prefsData->pdfPrefs.Permissions & 32 );
	enableSecurityControls(prefsData->pdfPrefs.Encrypt);

	if (prefsData->pdfPrefs.UseRGB)
		outputIntentionComboBox->setCurrentIndex(0);
	else
	{
		if (prefsData->pdfPrefs.isGrayscale)
			outputIntentionComboBox->setCurrentIndex(2);
		else
			outputIntentionComboBox->setCurrentIndex(1);
	}
	convertSpotsToProcessCheckBox->setChecked(!prefsData->pdfPrefs.UseSpotColors);

	bleedsWidget->setup(prefsData->pdfPrefs.bleeds, prefsData->docSetupPrefs.pagePositioning, prefsData->docSetupPrefs.docUnitIndex, false, false);
	bleedsWidget->setPageWidth(prefsData->docSetupPrefs.pageWidth);
	bleedsWidget->setPageHeight(prefsData->docSetupPrefs.pageHeight);
	bleedsWidget->setPageSize(prefsData->docSetupPrefs.pageSize);
	bleedsWidget->setMarginPreset(prefsData->docSetupPrefs.marginPreset);
//
	useCustomRenderingCheckBox->setChecked(prefsData->pdfPrefs.UseLPI);
	QMap<QString,LPIData>::Iterator itlp;
	customRenderingColorComboBox->clear();
	for (itlp = prefsData->pdfPrefs.LPISettings.begin(); itlp != prefsData->pdfPrefs.LPISettings.end(); ++itlp)
		customRenderingColorComboBox->addItem( itlp.key() );
	customRenderingColorComboBox->setCurrentIndex(0);

	customRenderingFrequencySpinBox->setValue(prefsData->pdfPrefs.LPISettings[customRenderingColorComboBox->currentText()].Frequency);
	customRenderingAngleSpinBox->setValue(prefsData->pdfPrefs.LPISettings[customRenderingColorComboBox->currentText()].Angle);
	customRenderingSpotFunctionComboBox->setCurrentIndex(prefsData->pdfPrefs.LPISettings[customRenderingColorComboBox->currentText()].SpotFunc);
	useSolidColorProfileCheckBox->setChecked(prefsData->pdfPrefs.UseProfiles);
	useImageProfileCheckBox->setChecked(prefsData->pdfPrefs.UseProfiles2);
	doNotUseEmbeddedImageProfileCheckBox->setChecked(prefsData->pdfPrefs.EmbeddedI);
	if ((prefsData->pdfPrefs.UseRGB) || (prefsData->pdfPrefs.isGrayscale))
	{
		enableSolidsImagesWidgets(false);
		enableProfiles(0);
	}
	else
		enableProfiles(1);
	enablePG();
	enablePGI();
	QString tp(prefsData->pdfPrefs.SolidProf);
	if (!ScCore->InputProfiles.contains(tp))
	{
/*		if (m_doc != 0 && exporting)
			tp = m_doc->CMSSettings.DefaultSolidColorRGBProfile;
		else*/
			tp = defaultSolidColorRGBProfile;
	}
	ProfilesL::Iterator itp;
	ProfilesL::Iterator itpend=ScCore->InputProfiles.end();
	solidColorProfileComboBox->clear();
	for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
	{
		solidColorProfileComboBox->addItem(itp.key());
		if (itp.key() == tp)
		{
			if (cmsUse)
				solidColorProfileComboBox->setCurrentIndex(solidColorProfileComboBox->count()-1);
		}
	}
	if (cmsUse)
		solidColorRenderingIntentComboBox->setCurrentIndex(Opts.Intent);
	QString tp1 = Opts.ImageProf;
	if (!ScCore->InputProfiles.contains(tp1))
	{
/*		if (m_doc != 0 && exporting)
			tp1 = m_doc->CMSSettings.DefaultSolidColorRGBProfile;
		else*/
			tp1 = defaultSolidColorRGBProfile;
	}
	ProfilesL::Iterator itp2;
	ProfilesL::Iterator itp2end=ScCore->InputProfiles.end();
	imageProfileComboBox->clear();
	for (itp2 = ScCore->InputProfiles.begin(); itp2 != itp2end; ++itp2)
	{
		imageProfileComboBox->addItem(itp2.key());
		if (itp2.key() == tp1)
		{
			if (cmsUse)
				imageProfileComboBox->setCurrentIndex(imageProfileComboBox->count()-1);
		}
	}
	if (cmsUse)
		imageRenderingIntentComboBox->setCurrentIndex(Opts.Intent2);
	if (!cmsUse)
	{
		//Disabling vis hiding
		//setSolidsImagesWidgetsShown(false);
		enableSolidsImagesWidgets(false);
	}

	ProfilesL::const_iterator itp3;
	QString tp3(Opts.PrintProf);
	if (!PDFXProfiles.contains(tp3))
	{
/*		if (m_doc != 0 && exporting)
			tp3 = m_doc->CMSSettings.DefaultPrinterProfile;
		else*/
			tp3 = defaultPrinterProfile;
	}
	pdfx3OutputProfileComboBox->clear();
	for (itp3 = PDFXProfiles.constBegin(); itp3 != PDFXProfiles.constEnd(); ++itp3)
	{
		pdfx3OutputProfileComboBox->addItem(itp3.key());
		if (itp3.key() == tp3)
			pdfx3OutputProfileComboBox->setCurrentIndex(pdfx3OutputProfileComboBox->count()-1);
	}
	pdfx3InfoStringLineEdit->setText(Opts.Info);


	/*
	if (m_doc != 0 && exporting)
	{
		docBleeds->setChecked(Opts.useDocBleeds);
		doDocBleeds();
	}
	*/
	registrationMarkOffsetSpinBox->setValue(prefsData->pdfPrefs.markOffset*unitRatio);
	printCropMarksCheckBox->setChecked(prefsData->pdfPrefs.cropMarks);
	printBleedMarksCheckBox->setChecked(prefsData->pdfPrefs.bleedMarks);
	printRegistrationMarksCheckBox->setChecked(prefsData->pdfPrefs.registrationMarks);
	printColorBarsCheckBox->setChecked(prefsData->pdfPrefs.colorMarks);
	printPageInfoCheckBox->setChecked(prefsData->pdfPrefs.docInfoMarks);
	if (!cmsUse)
		enablePDFXWidgets(false);
	if (cmsUse && (Opts.Version == PDFOptions::PDFVersion_X1a) && (!PDFXProfiles.isEmpty()))
		enablePDFX(3);
	else if (cmsUse && (Opts.Version == PDFOptions::PDFVersion_X3) && (!PDFXProfiles.isEmpty()))
		enablePDFX(4);
	else if (cmsUse && (Opts.Version == PDFOptions::PDFVersion_X4) && (!PDFXProfiles.isEmpty()))
		enablePDFX(5);
	else
		enablePDFXWidgets(false);
	/*
	if (mdoc != 0  && exporting)
	{
		EffectType->clear();
		EffectType->addItem( tr("No Effect"));
		EffectType->addItem( tr("Blinds"));
		EffectType->addItem( tr("Box"));
		EffectType->addItem( tr("Dissolve"));
		EffectType->addItem( tr("Glitter"));
		EffectType->addItem( tr("Split"));
		EffectType->addItem( tr("Wipe"));
		if (Opts.Version == PDFOptions::PDFVersion_15)
		{
			EffectType->addItem( tr("Push"));
			EffectType->addItem( tr("Cover"));
			EffectType->addItem( tr("Uncover"));
			EffectType->addItem( tr("Fade"));
		}
		PgSel = 0;
		Pages->setCurrentRow(0);
		SetEffOpts(0);
		Pages->setEnabled(false);
		Effects->setEnabled(false);
		PagePrev->setEnabled(false);
		DoEffects();
		if (CheckBox10->isChecked())
		{
			PageTime->setValue(EffVal[0].pageViewDuration);
			EffectTime->setValue(EffVal[0].pageEffectDuration);
			EffectType->setCurrentIndex(EffVal[0].effectType);
			EDirection->setCurrentIndex(EffVal[0].Dm);
			EDirection_2->setCurrentIndex(EffVal[0].M);
			EDirection_2_2->setCurrentIndex(EffVal[0].Di);
			SetEffOpts(EffectType->currentIndex());
		}
		if (mdoc->currentPageLayout != 0)
		{
			BleedTxt3->setText( tr( "Inside:" ) );
			BleedTxt4->setText( tr( "Outside:" ) );
		}

	}
	*/
	//


}


void Prefs_PDFExport::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
 //TODO
}

void Prefs_PDFExport::enableRangeControls(bool enabled)
{
	exportPageListLineEdit->setEnabled( enabled );
	exportRangeMorePushButton->setEnabled( enabled );
	if (enabled == false)
		includeBookmarksCheckBox->setChecked(false);
}

void Prefs_PDFExport::enableSecurityControls(bool enabled)
{
	passwordOwnerLineEdit->setEnabled(enabled);
	passwordUserLineEdit->setEnabled(enabled);
	allowPrintingCheckBox->setEnabled(enabled);
	allowChangingCheckBox->setEnabled(enabled);
	allowCopyingCheckBox->setEnabled(enabled);
	allowAnnotatingCheckBox->setEnabled(enabled);
}


void Prefs_PDFExport::enableCMS(bool enabled)
{
	cmsEnabled=enabled;
	addPDFVersions(enabled);
	enableProfiles(1);
}

void Prefs_PDFExport::createPageNumberRange()
{
	if (m_doc!=0)
	{
		CreateRange cr(exportPageListLineEdit->text(), m_doc->DocPages.count(), this);
		if (cr.exec())
		{
			CreateRangeData crData;
			cr.getCreateRangeData(crData);
			exportPageListLineEdit->setText(crData.pageRange);
			return;
		}
	}
	exportPageListLineEdit->setText(QString::null);
}

void Prefs_PDFExport::setMaximumResolution()
{
	if (maxResolutionLimitCheckBox->isChecked())
	{
		maxExportResolutionSpinBox->setEnabled(true);
		if (maxExportResolutionSpinBox->value() > epsExportResolutionSpinBox->value())
			maxExportResolutionSpinBox->setValue(epsExportResolutionSpinBox->value());
	}
	else
		maxExportResolutionSpinBox->setEnabled(false);
}

void Prefs_PDFExport::enableProfiles(int i)
{
	enableLPI(i);
	bool setter = false;
	if (i == 1 && pdfVersionComboBox->currentIndex() != 3)
		setter = true;

	enableSolidsImagesWidgets(setter);
}

void Prefs_PDFExport::enableLPI(int i)
{
	if (i == 1)
	{
		QString tp(Opts.SolidProf);
		if (!ScCore->InputProfiles.contains(tp))
		{
			if (m_doc != 0)
				tp = m_doc->CMSSettings.DefaultSolidColorRGBProfile;
			else
				tp = defaultSolidColorRGBProfile;
		}
		solidColorProfileComboBox->clear();
		ProfilesL::Iterator itp;
		ProfilesL::Iterator itpend=ScCore->InputProfiles.end();
		for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
		{
			solidColorProfileComboBox->addItem(itp.key());
			if (itp.key() == tp)
			{
				if (cmsEnabled)
					solidColorProfileComboBox->setCurrentIndex(solidColorProfileComboBox->count()-1);
			}
		}
		if (cmsEnabled)
			solidColorRenderingIntentComboBox->setCurrentIndex(Opts.Intent);
		QString tp1 = Opts.ImageProf;
		if (!ScCore->InputProfiles.contains(tp1))
		{
			if (m_doc != 0)
				tp1 = m_doc->CMSSettings.DefaultSolidColorRGBProfile;
			else
				tp1 = defaultSolidColorRGBProfile;
		}
		imageProfileComboBox->clear();
		ProfilesL::Iterator itp2;
		ProfilesL::Iterator itp2end=ScCore->InputProfiles.end();
		for (itp2 = ScCore->InputProfiles.begin(); itp2 != itp2end; ++itp2)
		{
			imageProfileComboBox->addItem(itp2.key());
			if (itp2.key() == tp1)
			{
				if (cmsEnabled)
					imageProfileComboBox->setCurrentIndex(imageProfileComboBox->count()-1);
			}
		}
		if (cmsEnabled)
			imageRenderingIntentComboBox->setCurrentIndex(Opts.Intent2);
		//Disabling vis hiding
		//setSolidsImagesWidgetsShown(cmsEnabled);
		enableSolidsImagesWidgets(cmsEnabled);
		convertSpotsToProcessCheckBox->show();
		if (m_doc!=0)
		{
			useCustomRenderingCheckBox->show();
			enableCustomRenderingWidgets(useCustomRenderingCheckBox->isChecked());
		}
	}
	else
	{
		convertSpotsToProcessCheckBox->hide();
		useCustomRenderingCheckBox->hide();
		enableCustomRenderingWidgets(false);
	}
}

void Prefs_PDFExport::setCustomRenderingWidgetsShown(bool visible)
{
	useCustomRenderingCheckBox->setShown(visible);
	customRenderingColorComboBox->setShown(visible);
	customRenderingFrequencySpinBox->setShown(visible);
	customRenderingAngleSpinBox->setShown(visible);
	customRenderingSpotFunctionComboBox->setShown(visible);
}

void Prefs_PDFExport::enableCustomRenderingWidgets(bool enabled)
{
	useCustomRenderingCheckBox->setEnabled(enabled);
	customRenderingColorComboBox->setEnabled(enabled);
	customRenderingFrequencySpinBox->setEnabled(enabled);
	customRenderingAngleSpinBox->setEnabled(enabled);
	customRenderingSpotFunctionComboBox->setEnabled(enabled);
}

void Prefs_PDFExport::setSolidsImagesWidgetsShown(bool visible)
{
	solidColorsLabel->setShown(visible);
	solidColorsLine->setShown(visible);
	useSolidColorProfileCheckBox->setShown(visible);
	solidColorProfileComboBox->setShown(visible);
	solidColorRenderingIntentComboBox->setShown(visible);
	imagesLabel->setShown(visible);
	imagesLine->setShown(visible);
	useImageProfileCheckBox->setShown(visible);
	doNotUseEmbeddedImageProfileCheckBox->setShown(visible);
	imageProfileComboBox->setShown(visible);
	imageRenderingIntentComboBox->setShown(visible);
}
void Prefs_PDFExport::enableSolidsImagesWidgets(bool enabled)
{
	useSolidColorProfileCheckBox->setEnabled(enabled);
	solidColorProfileComboBox->setEnabled(enabled);
	solidColorRenderingIntentComboBox->setEnabled(enabled);
	useImageProfileCheckBox->setEnabled(enabled);
	doNotUseEmbeddedImageProfileCheckBox->setEnabled(enabled);
	imageProfileComboBox->setEnabled(enabled);
	imageRenderingIntentComboBox->setEnabled(enabled);
}

void Prefs_PDFExport::enablePDFXWidgets(bool enabled)
{
	pdfx3OutputProfileComboBox->setEnabled(enabled);
	pdfx3InfoStringLineEdit->setEnabled(enabled);
}


void Prefs_PDFExport::enablePGI()
{
	bool setter=false;
	if (useImageProfileCheckBox->isChecked())
		setter = doNotUseEmbeddedImageProfileCheckBox->isChecked() ? true : false;
	imageProfileComboBox->setEnabled(setter);
	imageRenderingIntentComboBox->setEnabled(setter);
	doNotUseEmbeddedImageProfileCheckBox->setEnabled(useImageProfileCheckBox->isChecked());
}

void Prefs_PDFExport::enablePGI2()
{
	bool setter = doNotUseEmbeddedImageProfileCheckBox->isChecked() ? true : false;
	imageProfileComboBox->setEnabled(setter);
	imageRenderingIntentComboBox->setEnabled(setter);
}

void Prefs_PDFExport::enablePG()
{
	bool setter = useSolidColorProfileCheckBox->isChecked() ? true : false;

	solidColorProfileComboBox->setEnabled(setter);
	solidColorRenderingIntentComboBox->setEnabled(setter);
}

void Prefs_PDFExport::enablePDFX(int i)
{
	includeLayersCheckBox->setEnabled((i == 2) || (i == 5));
	/*
	if (useLayers2)
		useLayers2->setEnabled((i == 2) || (i == 5));
	if (doc != 0 && pdfExport)
	{
		int currentEff = EffectType->currentIndex();
		disconnect(EffectType, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
		EffectType->clear();
		EffectType->addItem( tr("No Effect"));
		EffectType->addItem( tr("Blinds"));
		EffectType->addItem( tr("Box"));
		EffectType->addItem( tr("Dissolve"));
		EffectType->addItem( tr("Glitter"));
		EffectType->addItem( tr("Split"));
		EffectType->addItem( tr("Wipe"));
		if (i == 2)
		{
			EffectType->addItem( tr("Push"));
			EffectType->addItem( tr("Cover"));
			EffectType->addItem( tr("Uncover"));
			EffectType->addItem( tr("Fade"));
			EffectType->setCurrentIndex(currentEff);
		}
		else
		{
			if (currentEff > 6)
			{
				currentEff = 0;
				EffectType->setCurrentIndex(0);
				SetEffOpts(0);
				for (int pg = 0; pg < doc->Pages->count(); ++pg)
				{
					if (EffVal[pg].effectType > 6)
						EffVal[pg].effectType = 0;
				}
			}
			else
				EffectType->setCurrentIndex(currentEff);
		}
		connect(EffectType, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
	}
	*/
	if (i < 3)  // not PDF/X
	{
		enablePDFXWidgets(false);
		tabWidget->setTabEnabled(2, true);
		outputIntentionComboBox->setEnabled(true);
		useImageProfileCheckBox->setEnabled(true);
		emit hasInfo();
		/*
		if (m_doc != 0 && pdfExport)
		{
			CheckBox10->setEnabled(true);
			EmbedFonts->setEnabled(true);
			if (EmbedList->count() != 0)
				FromEmbed->setEnabled(true);
			ToEmbed->setEnabled(true);
		}
		*/
		return;
	}
	// PDF/X is selected
	disconnect(outputIntentionComboBox, SIGNAL(activated(int)), this, SLOT(enableProfiles(int)));
	outputIntentionComboBox->setCurrentIndex(1);
	outputIntentionComboBox->setEnabled(false);
	enableProfiles(1);
	if ((i == 4) || (i == 5)) // X3 or X4, enforcing color profiles on images
	{
		useImageProfileCheckBox->setChecked(true);
		useImageProfileCheckBox->setEnabled(false);
	}
	/*
	if (m_doc != 0 && pdfExport)
	{
//		EmbedFonts->setChecked(true);
		EmbedAll();
		CheckBox10->setChecked(false);
		CheckBox10->setEnabled(false);
//		EmbedFonts->setEnabled(false);
		FromEmbed->setEnabled(false);
		ToEmbed->setEnabled(false);
		if (InfoString->text().isEmpty())
			emit noInfo();
		else
			emit hasInfo();
	}
	*/
	enablePGI();
	pdfx3OutputProfileComboBox->setEnabled(true);
	pdfx3InfoStringLineEdit->setEnabled(true);
	tabWidget->setTabEnabled(2, false);
	connect(outputIntentionComboBox, SIGNAL(activated(int)), this, SLOT(enableProfiles(int)));
}

void Prefs_PDFExport::addPDFVersions(bool addPDFXStrings)
{
	disconnect(pdfVersionComboBox, SIGNAL(activated(int)), this, SLOT(enablePDFX(int)));
	int i = pdfVersionComboBox->currentIndex();
	pdfVersionComboBox->clear();
	pdfVersionComboBox->addItem("PDF 1.3 (Acrobat 4)");
	pdfVersionComboBox->addItem("PDF 1.4 (Acrobat 5)");
	pdfVersionComboBox->addItem("PDF 1.5 (Acrobat 6)");
	if (addPDFXStrings)
	{
		pdfVersionComboBox->addItem("PDF/X-1a");
		pdfVersionComboBox->addItem("PDF/X-3");
		pdfVersionComboBox->addItem("PDF/X-4");
	}
	else
		i=qMin(i,2);
	pdfVersionComboBox->setCurrentIndex(i);
	connect(pdfVersionComboBox, SIGNAL(activated(int)), this, SLOT(enablePDFX(int)));
}
