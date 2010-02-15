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
//	connect(DSColor, SIGNAL(clicked()), this, SLOT(DoDownsample()));
//	connect(MirrorH, SIGNAL(clicked()), this, SLOT(PDFMirror()));
//	connect(MirrorV, SIGNAL(clicked()), this, SLOT(PDFMirror()));
//	connect(RotateDeg, SIGNAL(activated(int)), this, SLOT(Rotation(int)));
//	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
//	connect(EmbedProfs, SIGNAL(clicked()), this, SLOT(EnablePG()));
//	connect(EmbedProfs2, SIGNAL(clicked()), this, SLOT(EnablePGI()));
//	connect(NoEmbedded, SIGNAL(clicked()), this, SLOT(EnablePGI2()));
//	connect(PDFVersionCombo, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
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

	i = outputIntentionCheckBox->currentIndex();
	outputIntentionCheckBox->clear();
	outputIntentionCheckBox->addItem( tr( "Screen / Web" ) );
	outputIntentionCheckBox->addItem( tr( "Printer" ) );
	outputIntentionCheckBox->addItem( tr( "Grayscale" ) );
	outputIntentionCheckBox->setCurrentIndex(i);

//	LPIfunc->clear();
//	LPIfunc->addItem( tr( "Simple Dot" ) );
//	LPIfunc->addItem( tr( "Line" ) );
//	LPIfunc->addItem( tr( "Round" ) );
//	LPIfunc->addItem( tr( "Ellipse" ) );

//	QString tmp_ip[] = { tr("Perceptual"), tr("Relative Colorimetric"), tr("Saturation"), tr("Absolute Colorimetric")};
//	size_t ar_ip = sizeof(tmp_ip) / sizeof(*tmp_ip);
//	for (uint a = 0; a < ar_ip; ++a)
//		IntendS->addItem(tmp_ip[a]);
//	for (uint a = 0; a < ar_ip; ++a)
//		IntendI->addItem(tmp_ip[a]);
}

void Prefs_PDFExport::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	int unitIndex = prefsData->docSetupPrefs.docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	unitChange(unitIndex);

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
		outputIntentionCheckBox->setCurrentIndex(0);
	else
	{
		if (prefsData->pdfPrefs.isGrayscale)
			outputIntentionCheckBox->setCurrentIndex(2);
		else
			outputIntentionCheckBox->setCurrentIndex(1);
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
