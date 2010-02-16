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

	i = pdfVersionComboBox->currentIndex();
	pdfVersionComboBox->clear();
	pdfVersionComboBox->addItem("PDF 1.3 (Acrobat 4)");
	pdfVersionComboBox->addItem("PDF 1.4 (Acrobat 5)");
	pdfVersionComboBox->addItem("PDF 1.5 (Acrobat 6)");
	//if (cmsEnabled)// && (!PDFXProfiles.isEmpty()))
	{
		pdfVersionComboBox->addItem("PDF/X-1a");
		pdfVersionComboBox->addItem("PDF/X-3");
		pdfVersionComboBox->addItem("PDF/X-4");
	}
	pdfVersionComboBox->setCurrentIndex(i);

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
	int unitIndex = prefsData->docSetupPrefs.docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	unitChange(unitIndex);
	Opts=prefsData->pdfPrefs;
	defaultSolidColorRGBProfile=prefsData->colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
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

	registrationMarkOffsetSpinBox->setValue(prefsData->pdfPrefs.markOffset*unitRatio);
	printCropMarksCheckBox->setChecked(prefsData->pdfPrefs.cropMarks);
	printBleedMarksCheckBox->setChecked(prefsData->pdfPrefs.bleedMarks);
	printRegistrationMarksCheckBox->setChecked(prefsData->pdfPrefs.registrationMarks);
	printColorBarsCheckBox->setChecked(prefsData->pdfPrefs.colorMarks);
	printPageInfoCheckBox->setChecked(prefsData->pdfPrefs.docInfoMarks);

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

	setSolidsImagesWidgetsEnabled(setter);
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
		setSolidsImagesWidgetsShown(cmsEnabled);
		convertSpotsToProcessCheckBox->show();
		if (m_doc!=0)
		{
			useCustomRenderingCheckBox->show();
			setCustomRenderingWidgetsShown(useCustomRenderingCheckBox->isChecked());
		}
	}
	else
	{
		convertSpotsToProcessCheckBox->hide();
		useCustomRenderingCheckBox->hide();
		setCustomRenderingWidgetsShown(false);
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


void Prefs_PDFExport::setSolidsImagesWidgetsShown(bool visible)
{
	useSolidColorProfileCheckBox->setShown(visible);
	solidColorProfileComboBox->setShown(visible);
	solidColorRenderingIntentComboBox->setShown(visible);
	useImageProfileCheckBox->setShown(visible);
	doNotUseEmbeddedImageProfileCheckBox->setShown(visible);
	imageProfileComboBox->setShown(visible);
	imageRenderingIntentComboBox->setShown(visible);
}
void Prefs_PDFExport::setSolidsImagesWidgetsEnabled(bool enabled)
{
	useSolidColorProfileCheckBox->setEnabled(enabled);
	solidColorProfileComboBox->setEnabled(enabled);
	solidColorRenderingIntentComboBox->setEnabled(enabled);
	useImageProfileCheckBox->setEnabled(enabled);
	doNotUseEmbeddedImageProfileCheckBox->setEnabled(enabled);
	imageProfileComboBox->setEnabled(enabled);
	imageRenderingIntentComboBox->setEnabled(enabled);
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
		pdfx3OutputProfileComboBox->setEnabled(false);
		pdfx3InfoStringLineEdit->setEnabled(false);
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
