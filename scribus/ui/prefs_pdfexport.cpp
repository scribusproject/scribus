/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <array>

#include <QStandardItem>
#include <QAbstractItemView>

#include "iconmanager.h"
#include "pageitemiterator.h"
#include "prefsstructs.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "ui/createrange.h"
#include "ui/prefs_pdfexport.h"
#include "ui/scrspinbox.h"
#include "units.h"
#include "util.h"

Prefs_PDFExport::Prefs_PDFExport(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	  m_doc(doc)
{
	setupUi(this);

	scrollArea->viewport()->setAutoFillBackground(false);
	scrollArea->widget()->setAutoFillBackground(false);
	scrollArea_2->viewport()->setAutoFillBackground(false);
	scrollArea_2->widget()->setAutoFillBackground(false);
	scrollArea_3->viewport()->setAutoFillBackground(false);
	scrollArea_3->widget()->setAutoFillBackground(false);

	exportRangeMorePushButton->setIcon(IconManager::instance().loadIcon("ellipsis.png"));
	pageMirrorHorizontalToolButton->setIcon(IconManager::instance().loadIcon("16/flip-object-horizontal.png"));
	pageMirrorVerticalToolButton->setIcon(IconManager::instance().loadIcon("16/flip-object-vertical.png"));

	unitChange(0);
	languageChange();

	m_caption = tr("PDF Export");
	m_icon = "acroread16.png";

	connect(fontEmbeddingCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(EmbeddingModeChange()));
	connect(exportChosenPagesRadioButton, SIGNAL(toggled(bool)), this, SLOT(enableRangeControls(bool)));
	connect(exportRangeMorePushButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect(maxResolutionLimitCheckBox, SIGNAL(clicked()), this, SLOT(setMaximumResolution()));
	connect(outputIntentionComboBox, SIGNAL(activated(int)), this, SLOT(enableProfiles(int)));
	connect(useSolidColorProfileCheckBox, SIGNAL(clicked()), this, SLOT(enablePG()));
	connect(useImageProfileCheckBox, SIGNAL(clicked()), this, SLOT(enablePGI()));
	connect(doNotUseEmbeddedImageProfileCheckBox, SIGNAL(clicked()), this, SLOT(enablePGI2()));
	connect(pdfVersionComboBox, SIGNAL(activated(int)), this, SLOT(enablePDFX(int)));
	connect(useEncryptionCheckBox, SIGNAL(clicked(bool)), this, SLOT(enableSecurityControls(bool)));
	connect(useCustomRenderingCheckBox, SIGNAL(clicked()), this, SLOT(enableLPI2()));
	connect(customRenderingColorComboBox, SIGNAL(activated(int)), this, SLOT(SelLPIcol(int)));

	rotationComboBox->setToolTip( "<qt>" + tr( "Automatically rotate the exported pages" ) + "</qt>" );
	exportAllPagesRadioButton->setToolTip( "<qt>" + tr( "Export all pages to PDF" ) + "</qt>" );
	exportChosenPagesRadioButton->setToolTip( "<qt>" + tr( "Export a range of pages to PDF" ) );
	exportPageListLineEdit->setToolTip( "<qt>" + tr( "Insert a comma separated list of tokens where "
											"a token can be * for all the pages, 1-5 for "
											"a range of pages or a single page number") + "</qt>" );
	exportRangeMorePushButton->setToolTip( "<qt>" + tr( "Create a range of pages" ) + "</qt>");

	pdfVersionComboBox->setToolTip( "<qt>" + tr( "Determines the PDF compatibility.<br/><b>PDF 1.3</b>, which is the earliest supported version, gives the widest compatibility with printers' RIPS.<br/><b>PDF 1.4</b>, which is the default version, supports features such as transparency and 128 bit encryption.<br/><b>PDF 1.5</b> is necessary when you wish to preserve objects in separate layers within the PDF.<br/><b>PDF/X-1a</b> is for blind exchange with colors strictly specified in CMYK or spot colors.<br/><b>PDF/X-3</b> is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.<br/><b>PDF/X-4</b> is an extension of PDF/X-3 to support transparency and layering." ) + "</qt>");
	pageBindingComboBox->setToolTip( "<qt>" + tr( "Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left." ) + "</qt>" );
	generateThumbnailsCheckBox->setToolTip( "<qt>" + tr( "Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation." ) + "</qt>" );
	saveLinkedTextFramesAsArticlesCheckBox->setToolTip( "<qt>" + tr( "Generate PDF Articles, which is useful for navigating linked articles in a PDF" ) + "</qt>" );
	includeLayersCheckBox->setToolTip( "<qt>" + tr( "Layers in your document are exported to the PDF. Only available if PDF 1.5 is chosen." ) + "</qt>" );
	includeBookmarksCheckBox->setToolTip( "<qt>" + tr( "Embed the bookmarks you created in your document. These are useful for navigating long PDF documents." ) + "</qt>" );
	epsExportResolutionSpinBox->setToolTip( "<qt>" + tr( "Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos." ) + "</qt>" );
	embedPDFAndEPSFilesCheckBox->setToolTip( "<qt>" + tr( "Export PDFs in image frames as embedded PDFs. This does *not* yet take care of colorspaces, so you should know what you are doing before setting this to 'true'." ) + "</qt>" );
	compressTextAndVectorGraphicsCheckBox->setToolTip( "<qt>" + tr( "Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size." ) + "</qt>" );
	imageCompressionMethodComboBox->setToolTip( "<qt>" + tr( "Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options." ) + "</qt>");
	imageCompressionQualityComboBox->setToolTip( "<qt>" + tr( "Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg." ) + "</qt>");
	maxResolutionLimitCheckBox->setToolTip( "<qt>" + tr( "Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export." ) + "</qt>" );
	maxExportResolutionSpinBox->setToolTip( "<qt>" + tr( "DPI (Dots Per Inch) for image export") + "</qt>" );
	useEncryptionCheckBox->setToolTip( "<qt>" + tr( "Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations." ) + "</qt>" );
	passwordOwnerLineEdit->setToolTip( "<qt>" + tr( "Choose an owner password which enables or disables all the security features in your exported PDF" ) + "</qt>" );
	passwordUserLineEdit->setToolTip( "<qt>" + tr( "Choose a password for users to be able to read your PDF" ) + "</qt>" );
	allowPrintingCheckBox->setToolTip( "<qt>" + tr( "Allow printing of the PDF. If unchecked, printing is prevented." ) + "</qt>" );
	allowChangingCheckBox->setToolTip( "<qt>" + tr( "Allow modifying of the PDF. If unchecked, modifying the PDF is prevented." ) + "</qt>" );
	allowCopyingCheckBox->setToolTip( "<qt>" + tr( "Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied." ) + "</qt>" );
	allowAnnotatingCheckBox->setToolTip( "<qt>" + tr( "Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented." ) + "</qt>" );
	outputIntentionComboBox->setToolTip( "<qt>" + tr( "Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF." ) + "</qt>" );
	useCustomRenderingCheckBox->setToolTip( "<qt>" + tr( "This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems." ) + "</qt>" );
	useSolidColorProfileCheckBox->setToolTip( "<qt>" + tr( "Embed a color profile for solid colors" ) + "</qt>" );
	solidColorProfileComboBox->setToolTip( "<qt>" + tr( "Color profile for solid colors" ) + "</qt>" );
	solidColorRenderingIntentComboBox->setToolTip( "<qt>" + tr( "Rendering intent for solid colors" ) + "</qt>" );
	useImageProfileCheckBox->setToolTip( "<qt>" + tr( "Embed a color profile for images" ) + "</qt>" );
	doNotUseEmbeddedImageProfileCheckBox->setToolTip( "<qt>" + tr( "Do not use color profiles that are embedded in source images" ) + "</qt>" );
	imageProfileComboBox->setToolTip( "<qt>" + tr( "Color profile for images" ) + "</qt>" );
	imageRenderingIntentComboBox->setToolTip( "<qt>" + tr( "Rendering intent for images" ) + "</qt>" );
	pageMirrorHorizontalToolButton->setToolTip( "<qt>" + tr( "Mirror Page(s) horizontally" ) + "</qt>" );
	pageMirrorVerticalToolButton->setToolTip( "<qt>" + tr( "Mirror Page(s) vertically" ) + "</qt>" );
	convertSpotsToProcessCheckBox->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
	clipToPrinterMarginsCheckBox->setToolTip( "<qt>" + tr( "Do not show objects outside the margins in the exported file" ) + "</qt>" );
	//PrePress tab
	printCropMarksCheckBox->setToolTip( "<qt>" + tr( "Creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing" ) + "</qt>" );
	printBleedMarksCheckBox->setToolTip( "<qt>" + tr( "This creates bleed marks which are indicated by  _ . _ and show the bleed limit" ) + "</qt>" );
	printRegistrationMarksCheckBox->setToolTip( "<qt>" + tr( "Add registration marks to each separation" ) + "</qt>" );
	printColorBarsCheckBox->setToolTip( "<qt>" + tr( "Add color calibration bars" ) + "</qt>" );
	printPageInfoCheckBox->setToolTip( "<qt>" + tr( "Add document information which includes the document title and page numbers" ) + "</qt>" );
	markLengthSpinBox->setToolTip( "<qt>" + tr( "Length of crop marks" ) + "</qt>" );
	markOffsetSpinBox->setToolTip( "<qt>" + tr( "Indicate the distance offset for the registration marks" ) + "</qt>" );
//	BleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
//	BleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
//	BleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
//	BleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	useDocumentBleedsCheckBox->setToolTip( "<qt>" + tr( "Use the existing bleed settings from the document preferences" ) + "</qt>" );
	pdfx3OutputProfileComboBox->setToolTip( "<qt>" + tr( "Output profile for printing. If possible, get some guidance from your printer on profile selection." ) + "</qt>" );
	pdfx3InfoStringLineEdit->setToolTip( "<qt>" + tr( "Mandatory string for PDF/X or the PDF will fail PDF/X conformance. We recommend you use the title of the document." ) + "</qt>" );

}

Prefs_PDFExport::~Prefs_PDFExport() = default;

PDFOptions::PDFFontEmbedding Prefs_PDFExport::fontEmbeddingMode() const
{
	return fontEmbeddingCombo->embeddingMode();
}

QStringList Prefs_PDFExport::fontsToEmbed() const
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();
	if (embeddingMode != PDFOptions::EmbedFonts)
		return QStringList();

	QStringList fonts;
	for (int i = 0; i < embeddedFontsListWidget->count(); ++i)
		fonts.append(embeddedFontsListWidget->item(i)->text());
	return fonts;
}

QStringList Prefs_PDFExport::fontsToSubset() const
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();
	if (embeddingMode != PDFOptions::EmbedFonts)
		return QStringList();

	QStringList fonts;
	for (int i = 0; i < subsettedFontsListWidget->count(); ++i)
		fonts.append(subsettedFontsListWidget->item(i)->text());
	return fonts;
}

QStringList Prefs_PDFExport::fontsToOutline() const
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();
	if (embeddingMode != PDFOptions::OutlineFonts)
		return QStringList();

	if (m_doc)
		return m_doc->usedFonts().keys();
	return QStringList();
}

QListWidgetItem* Prefs_PDFExport::addFontItem(const QString& fontName, QListWidget* fontList)
{
	QListWidgetItem* item = nullptr;
	if (!AllFonts.contains(fontName))
		return nullptr;

	const ScFace face = AllFonts.value(fontName);
	if (face.isReplacement())
		item = new QListWidgetItem( IconManager::instance().loadIcon("font_subst16.png"), fontName, fontList );
	else if (face.type() == ScFace::TYPE1)
		item = new QListWidgetItem( IconManager::instance().loadIcon("font_type1_16.png"), fontName, fontList );
	else if (face.type() == ScFace::TTF)
		item = new QListWidgetItem( IconManager::instance().loadIcon("font_truetype16.png"), fontName, fontList );
	else if (face.type() == ScFace::OTF)
		item = new QListWidgetItem( IconManager::instance().loadIcon("font_otf16.png"), fontName, fontList );

	return item;
}

void Prefs_PDFExport::unitChange(int unitIndex)
{
	unitRatio = unitGetRatioFromIndex(unitIndex);

	markLengthSpinBox->setNewUnit(unitIndex);
	markLengthSpinBox->setMinimum(0);
	markLengthSpinBox->setMaximum(3000 * unitRatio);

	markOffsetSpinBox->setNewUnit(unitIndex);
	markOffsetSpinBox->setMinimum(0);
	markOffsetSpinBox->setMaximum(3000 * unitRatio);

	bleedsWidget->setNewUnit(unitIndex);
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
	SelLPIcolor = customRenderingColorComboBox->currentText();

	i = solidColorRenderingIntentComboBox->currentIndex();
	int j = imageRenderingIntentComboBox->currentIndex();
	solidColorRenderingIntentComboBox->clear();
	imageRenderingIntentComboBox->clear();
	
	std::array<QString, 4> tmp_ip = { tr("Perceptual"), tr("Relative Colorimetric"), tr("Saturation"), tr("Absolute Colorimetric")};
	for (size_t a = 0; a < tmp_ip.size(); ++a)
		solidColorRenderingIntentComboBox->addItem(tmp_ip[a]);
	for (size_t a = 0; a < tmp_ip.size(); ++a)
		imageRenderingIntentComboBox->addItem(tmp_ip[a]);
	solidColorRenderingIntentComboBox->setCurrentIndex(i);
	imageRenderingIntentComboBox->setCurrentIndex(j);
}

void Prefs_PDFExport::restoreDefaults(struct ApplicationPrefs *prefsData)
{
}

void Prefs_PDFExport::restoreDefaults(struct ApplicationPrefs *prefsData, const ProfilesL & PDFXProfiles, bool exporting)
{
	exportingPDF = exporting;
	enablePDFExportTabs(exportingPDF);
	AllFonts = prefsData->fontPrefs.AvailFonts;
	int unitIndex = prefsData->docSetupPrefs.docUnitIndex;
	unitRatio = unitGetRatioFromIndex(unitIndex);
	unitChange(unitIndex);
	Opts = prefsData->pdfPrefs;
	defaultSolidColorRGBProfile = prefsData->colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
	defaultPrinterProfile = prefsData->colorPrefs.DCMSset.DefaultPrinterProfile;
	exportAllPagesRadioButton->setChecked(true);
	enableRangeControls(false);
	rotationComboBox->setCurrentIndex(prefsData->pdfPrefs.RotateDeg / 90);
	pageMirrorHorizontalToolButton->setChecked(prefsData->pdfPrefs.MirrorH);
	pageMirrorVerticalToolButton->setChecked(prefsData->pdfPrefs.MirrorV);
	clipToPrinterMarginsCheckBox->setChecked(prefsData->pdfPrefs.doClip);
	bool cmsUse = m_doc ? (ScCore->haveCMS() && m_doc->HasCMS) : false;
	if (!cmsUse)
		pdfVersionComboBox->setVersion(PDFVersion::PDF_14);
	pdfVersionComboBox->setVersion(prefsData->pdfPrefs.Version);
	pageBindingComboBox->setCurrentIndex(prefsData->pdfPrefs.Binding);
	generateThumbnailsCheckBox->setChecked(prefsData->pdfPrefs.Thumbnails);
	saveLinkedTextFramesAsArticlesCheckBox->setChecked(prefsData->pdfPrefs.Articles);
	includeBookmarksCheckBox->setChecked(prefsData->pdfPrefs.Bookmarks);
	includeLayersCheckBox->setChecked(prefsData->pdfPrefs.useLayers);
	includeLayersCheckBox->setEnabled(prefsData->pdfPrefs.Version.supportsOCGs());

	epsExportResolutionSpinBox->setValue(prefsData->pdfPrefs.Resolution);
	embedPDFAndEPSFilesCheckBox->setChecked(prefsData->pdfPrefs.embedPDF);
	compressTextAndVectorGraphicsCheckBox->setChecked( prefsData->pdfPrefs.Compress );
	imageCompressionMethodComboBox->setCurrentIndex(prefsData->pdfPrefs.CompressMethod);
	imageCompressionQualityComboBox->setCurrentIndex(prefsData->pdfPrefs.Quality);
	maxResolutionLimitCheckBox->setChecked(prefsData->pdfPrefs.RecalcPic);
	maxExportResolutionSpinBox->setValue(prefsData->pdfPrefs.PicRes);
	maxExportResolutionSpinBox->setEnabled(prefsData->pdfPrefs.RecalcPic);

	fontEmbeddingCombo->setEmbeddingMode(prefsData->pdfPrefs.FontEmbedding);
	if (m_doc != nullptr && exportingPDF)
	{
		//	Build a list of all Fonts used in Annotations
		int pageItOptions = PageItemIterator::IterateInGroups | PageItemIterator::IterateInDocItems | PageItemIterator::IterateInMasterItems | PageItemIterator::IterateInFrameItems;
		for (PageItemIterator it(m_doc, pageItOptions); *it; ++it)
		{
			PageItem *currItem = *it;
			if (((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText)) && (currItem->isAnnotation()) && (currItem->itemText.length() > 0))
				AnnotationFonts.insert(currItem->itemText.defaultStyle().charStyle().font().replacementName(), QString());
		}
		toSubsetButton->setEnabled(false);
		fromSubsetButton->setEnabled(false);

		if ((Opts.EmbedList.count() == 0) && (Opts.SubsetList.count() == 0) && (Opts.firstUse))
			EmbedAll();
		else
		{
			QStringList docFonts = m_doc->usedFonts().keys();
			embeddedFontsListWidget->clear();
			subsettedFontsListWidget->clear();
			for (int fe = 0; fe < docFonts.count(); ++fe)
			{
				const QString& fontName = docFonts.at(fe);
				const ScFace fontFace = AllFonts[fontName];
				if (Opts.EmbedList.contains(fontName) && (!fontFace.isOTF() || Opts.supportsEmbeddedOpenTypeFonts()) && !fontFace.subset())
					addFontItem(fontName, embeddedFontsListWidget);
				else
				{
					addFontItem(fontName, subsettedFontsListWidget);
					if (!Opts.SubsetList.contains(fontName))
						Opts.SubsetList.append(fontName);
				}
			}
			QMap<QString, QString>::Iterator itAnn;
			for (itAnn = AnnotationFonts.begin(); itAnn != AnnotationFonts.end(); ++itAnn)
			{
				QList<QListWidgetItem *> itEmbed = embeddedFontsListWidget->findItems(itAnn.key(), Qt::MatchExactly);
				if (itEmbed.count() == 0)
				{
					QListWidgetItem* item = addFontItem(itAnn.key(), embeddedFontsListWidget);
					item->setFlags(Qt::ItemIsEnabled);
				}
				QList<QListWidgetItem *> itSubset = subsettedFontsListWidget->findItems(itAnn.key(), Qt::MatchExactly);
				for (int itOut = 0; itOut < itSubset.count(); ++itOut)
				{
					QListWidgetItem* item = itSubset[itOut];
					delete subsettedFontsListWidget->takeItem(subsettedFontsListWidget->row(item));
				}
			}
		}
		enabledEffectsCheckBox->setChecked(Opts.PresentMode);
		showPagePreviewsCheckBox->setChecked(false);
		effectsPageListWidget->clear();
		QString tmp;
		for (int pg = 0; pg < m_doc->Pages->count(); ++pg)
		{
			effectsPageListWidget->addItem( tr("Page")+" "+tmp.setNum(pg+1));
			EffVal.append(m_doc->Pages->at(pg)->PresentVals);
		}
		displayDurationSpinBox->setValue(EffVal[0].pageViewDuration);
		effectDurationSpinBox->setValue(EffVal[0].pageEffectDuration);
		bool df = true;
		if ((Opts.displayBookmarks) || (Opts.displayFullscreen) || (Opts.displayLayers) || (Opts.displayThumbs))
			df = false;
		if (df)
			useViewerDefaultsRadioButton->setChecked(df);
		useFullScreenRadioButton->setChecked(Opts.displayFullscreen);
		useBookmarksRadioButton->setChecked(Opts.displayBookmarks);
		useThumbnailsRadioButton->setChecked(Opts.displayThumbs);
		useLayersRadioButton->setChecked(Opts.displayLayers);
		hideViewerToolBarCheckBox->setChecked(Opts.hideToolBar);
		hideViewerMenuBarCheckBox->setChecked(Opts.hideMenuBar);
		fitViewerWindowCheckBox->setChecked(Opts.fitWindow);
		QMap<QString,QString>::Iterator itja;
		startupJavascriptComboBox->clear();
		startupJavascriptComboBox->addItem( tr("No Script"));
		for (itja = m_doc->JavaScripts.begin(); itja != m_doc->JavaScripts.end(); ++itja)
			startupJavascriptComboBox->addItem(itja.key());
		if (m_doc->JavaScripts.contains(Opts.openAction))
			setCurrentComboItem(startupJavascriptComboBox, Opts.openAction);
		if (Opts.PageLayout == PDFOptions::SinglePage)
			singlePageRadioButton->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::OneColumn)
			continuousPagesRadioButton->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::TwoColumnLeft)
			facingPagesLeftRadioButton->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::TwoColumnRight)
			facingPagesRightRadioButton->setChecked(true);
		useLayersRadioButton->setEnabled(Opts.Version.supportsOCGs());
	}

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

	bleedsWidget->setup(prefsData->pdfPrefs.bleeds, prefsData->docSetupPrefs.pagePositioning, prefsData->docSetupPrefs.docUnitIndex, NewMarginWidget::BleedWidgetFlags);
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
		//enableSolidsImagesWidgets(false);
		enableProfiles(0);
	}
	else
		enableProfiles(1);
	enablePG();
	enablePGI();
	QString tp(prefsData->pdfPrefs.SolidProf);
	if (!ScCore->InputProfiles.contains(tp))
	{
		if (m_doc != nullptr && exportingPDF)
			tp = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
		else
			tp = defaultSolidColorRGBProfile;
	}
	ProfilesL::Iterator itp;
	ProfilesL::Iterator itpend = ScCore->InputProfiles.end();
	solidColorProfileComboBox->clear();
	for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
	{
		solidColorProfileComboBox->addItem(itp.key());
		if (itp.key() == tp)
			solidColorProfileComboBox->setCurrentIndex(solidColorProfileComboBox->count() - 1);
	}
	int solidIntent = Opts.Intent;
	if (Opts.Intent < 0)
		solidIntent = 1;
	solidColorRenderingIntentComboBox->setCurrentIndex(solidIntent);
	QString tp1 = Opts.ImageProf;
	if (!ScCore->InputProfiles.contains(tp1))
	{
		if (m_doc != nullptr && exportingPDF)
			tp1 = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
		else
			tp1 = defaultSolidColorRGBProfile;
	}
	ProfilesL::Iterator itp2;
	ProfilesL::Iterator itp2end = ScCore->InputProfiles.end();
	imageProfileComboBox->clear();
	for (itp2 = ScCore->InputProfiles.begin(); itp2 != itp2end; ++itp2)
	{
		imageProfileComboBox->addItem(itp2.key());
		if (itp2.key() == tp1)
			imageProfileComboBox->setCurrentIndex(imageProfileComboBox->count()-1);
	}
	int imageIntent = Opts.Intent2;
	if (imageIntent < 0)
		imageIntent = 0;
	imageRenderingIntentComboBox->setCurrentIndex(imageIntent);
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
		if (m_doc != nullptr && exportingPDF)
			tp3 = m_doc->cmsSettings().DefaultPrinterProfile;
		else
			tp3 = defaultPrinterProfile;
	}
	pdfx3OutputProfileComboBox->clear();
	for (itp3 = PDFXProfiles.constBegin(); itp3 != PDFXProfiles.constEnd(); ++itp3)
	{
		pdfx3OutputProfileComboBox->addItem(itp3.key());
		if (itp3.key() == tp3)
			pdfx3OutputProfileComboBox->setCurrentIndex(pdfx3OutputProfileComboBox->count()-1);
	}

	if (!Opts.Info.isEmpty())
		pdfx3InfoStringLineEdit->setText(Opts.Info);
	else
	{
		if (m_doc != nullptr && exporting)
		{
			QFileInfo fi(m_doc->documentFileName());
			pdfx3InfoStringLineEdit->setText(fi.fileName());
		}
		else
			pdfx3InfoStringLineEdit->setText( tr("InfoString"));
	}

	if (m_doc != nullptr && exportingPDF)
	{
		useDocumentBleedsCheckBox->setChecked(Opts.useDocBleeds);
		doDocBleeds();
	}
	else
		useDocumentBleedsCheckBox->hide();

	markLengthSpinBox->setValue(prefsData->pdfPrefs.markLength*unitRatio);
	markOffsetSpinBox->setValue(prefsData->pdfPrefs.markOffset*unitRatio);
	printCropMarksCheckBox->setChecked(prefsData->pdfPrefs.cropMarks);
	printBleedMarksCheckBox->setChecked(prefsData->pdfPrefs.bleedMarks);
	printRegistrationMarksCheckBox->setChecked(prefsData->pdfPrefs.registrationMarks);
	printColorBarsCheckBox->setChecked(prefsData->pdfPrefs.colorMarks);
	printPageInfoCheckBox->setChecked(prefsData->pdfPrefs.docInfoMarks);
	if (!cmsUse)
		enablePDFXWidgets(false);
	if (cmsUse && (Opts.Version == PDFVersion::PDF_X1a) && (!PDFXProfiles.isEmpty()))
		enablePDFX(3);
	else if (cmsUse && (Opts.Version == PDFVersion::PDF_X3) && (!PDFXProfiles.isEmpty()))
		enablePDFX(4);
	else if (cmsUse && (Opts.Version == PDFVersion::PDF_X4) && (!PDFXProfiles.isEmpty()))
		enablePDFX(5);
	else
		enablePDFXWidgets(false);

	if (m_doc != nullptr  && exportingPDF)
	{
		effectTypeComboBox->clear();
		effectTypeComboBox->addItem( tr("No Effect"));
		effectTypeComboBox->addItem( tr("Blinds"));
		effectTypeComboBox->addItem( tr("Box"));
		effectTypeComboBox->addItem( tr("Dissolve"));
		effectTypeComboBox->addItem( tr("Glitter"));
		effectTypeComboBox->addItem( tr("Split"));
		effectTypeComboBox->addItem( tr("Wipe"));
		if (Opts.Version.supportsPDF15PresentationEffects())
		{
			effectTypeComboBox->addItem( tr("Push"));
			effectTypeComboBox->addItem( tr("Cover"));
			effectTypeComboBox->addItem( tr("Uncover"));
			effectTypeComboBox->addItem( tr("Fade"));
		}
		effectsPageListWidget->setCurrentRow(0);
		SetEffOpts(0);
		effectsPageListWidget->setEnabled(false);
		enableEffects(false);
		showPagePreviewsCheckBox->setEnabled(false);
		if (enabledEffectsCheckBox->isChecked())
		{
			displayDurationSpinBox->setValue(EffVal[0].pageViewDuration);
			effectDurationSpinBox->setValue(EffVal[0].pageEffectDuration);
			effectTypeComboBox->setCurrentIndex(EffVal[0].effectType);
			effectMovingDirectionComboBox->setCurrentIndex(EffVal[0].Dm);
			effectInOutComboBox->setCurrentIndex(EffVal[0].M);
			effectDirectionComboBox->setCurrentIndex(EffVal[0].Di);
			SetEffOpts(effectTypeComboBox->currentIndex());
		}
		/* Using margin widget, is this necessary?
		if (m_doc->currentPageLayout != 0)
		{
			BleedTxt3->setText( tr( "Inside:" ) );
			BleedTxt4->setText( tr( "Outside:" ) );
		}
		*/
	}

	if (m_doc != nullptr && exportingPDF)
	{
		connect(embedAllButton, SIGNAL(clicked()), this, SLOT(EmbedAll()));
		connect(embeddedFontsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelEFont(QListWidgetItem*)));
		connect(subsetAllButton, SIGNAL(clicked()), this, SLOT(SubsetAll()));
		connect(subsettedFontsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelSFont(QListWidgetItem*)));
		connect(toSubsetButton, SIGNAL(clicked()), this, SLOT(PutToSubset()));
		connect(fromSubsetButton, SIGNAL(clicked()), this, SLOT(RemoveSubset()));
		connect(showPagePreviewsCheckBox, SIGNAL(clicked()), this, SLOT(PagePr()));
		connect(effectsPageListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SetPgEff()));
		connect(effectTypeComboBox, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
		connect(effectDirectionComboBox, SIGNAL(activated(int)), this, SLOT(ValidDI(int)));
		connect(enabledEffectsCheckBox, SIGNAL(clicked()), this, SLOT(DoEffects()));
		connect(applyEffectToAllPagesPushButton, SIGNAL(clicked()), this, SLOT(EffectOnAll()));
//		connect(pdfx3InfoStringLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(checkInfo()));
//		connect(pdfx3InfoStringLineEdit, SIGNAL(editingFinished()), this, SLOT(checkInfo()));
		connect(useDocumentBleedsCheckBox, SIGNAL(clicked()), this, SLOT(doDocBleeds()));

		embedAllButton->setToolTip( "<qt>" + tr( "Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document." ) + "</qt>");
		enabledEffectsCheckBox->setToolTip( "<qt>" + tr( "Enables presentation effects when using Adobe&#174; Reader&#174; and other PDF viewers which support this in full screen mode." ) + "</qt>");
		showPagePreviewsCheckBox->setToolTip( "<qt>" + tr( "Show page previews of each page listed above." ) + "</qt>");
		effectDurationSpinBox->setToolTip( "<qt>" + tr( "Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition." ) + "</qt>" );
		effectDurationSpinBox->setToolTip( "<qt>" + tr( "Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down." ) + "</qt>" );
		effectTypeComboBox->setToolTip( "<qt>" + tr( "Type of the display effect" ) + "</qt>" );
		effectMovingDirectionComboBox->setToolTip( "<qt>" + tr( "Direction of the effect of moving lines for the split and blind effects" ) + "</qt>" );
		effectInOutComboBox->setToolTip( "<qt>" + tr( "Starting position for the box and split effects" ) + "</qt>" );
		effectDirectionComboBox->setToolTip( "<qt>" + tr( "Direction of the glitter or wipe effects" ) + "</qt>" );
		applyEffectToAllPagesPushButton->setToolTip( "<qt>" + tr( "Apply the selected effect to all pages" ) + "</qt>" );
		subsetAllButton->setToolTip( "<qt>" + tr("Embed only subset fonts with glyphs used in the document into the PDF") + "</qt>");
		singlePageRadioButton->setToolTip( "<qt>" + tr( "Show the document in single page mode" ) + "</qt>" );
		continuousPagesRadioButton->setToolTip( "<qt>" + tr( "Show the document in single page mode with the pages displayed continuously end to end like a scroll" ) + "</qt>" );
		facingPagesLeftRadioButton->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the left" ) + "</qt>" );
		facingPagesRightRadioButton->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the right" ) + "</qt>" );
		useViewerDefaultsRadioButton->setToolTip( "<qt>" + tr( "Use the viewer's defaults or the user's preferences if set differently from the viewer defaults" ) + "</qt>" );
		useFullScreenRadioButton->setToolTip( "<qt>" + tr( "Enables viewing the document in full screen" ) + "</qt>" );
		useBookmarksRadioButton->setToolTip( "<qt>" + tr( "Display the bookmarks upon opening" ) + "</qt>" );
		useThumbnailsRadioButton->setToolTip( "<qt>" + tr( "Display the page thumbnails upon opening" ) + "</qt>" );
		useLayersRadioButton->setToolTip( "<qt>" + tr( "Forces the displaying of layers. Useful only for PDF 1.5+." ) + "</qt>" );
		hideViewerToolBarCheckBox->setToolTip( "<qt>" + tr( "Hides the Tool Bar which has selection and other editing capabilities" ) + "</qt>" );
		hideViewerMenuBarCheckBox->setToolTip( "<qt>" + tr( "Hides the Menu Bar for the viewer, the PDF will display in a plain window" ) + "</qt>" );
		fitViewerWindowCheckBox->setToolTip( "<qt>" + tr( "Fit the document page or pages to the available space in the viewer window" ) + "</qt>" );
	}
}


void Prefs_PDFExport::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->pdfPrefs.Thumbnails = generateThumbnailsCheckBox->isChecked();
	prefsData->pdfPrefs.Compress = compressTextAndVectorGraphicsCheckBox->isChecked();
	prefsData->pdfPrefs.CompressMethod = (PDFOptions::PDFCompression) imageCompressionMethodComboBox->currentIndex();
	prefsData->pdfPrefs.Quality = imageCompressionQualityComboBox->currentIndex();
	prefsData->pdfPrefs.Resolution = epsExportResolutionSpinBox->value();
	prefsData->pdfPrefs.RecalcPic = maxResolutionLimitCheckBox->isChecked();
	prefsData->pdfPrefs.PicRes = maxExportResolutionSpinBox->value();
	prefsData->pdfPrefs.embedPDF = embedPDFAndEPSFilesCheckBox->isChecked();
	prefsData->pdfPrefs.Bookmarks = includeBookmarksCheckBox->isChecked();
	prefsData->pdfPrefs.Binding = pageBindingComboBox->currentIndex();
	prefsData->pdfPrefs.MirrorH = pageMirrorHorizontalToolButton->isChecked();
	prefsData->pdfPrefs.MirrorV = pageMirrorVerticalToolButton->isChecked();
	prefsData->pdfPrefs.RotateDeg = rotationComboBox->currentIndex() * 90;
	prefsData->pdfPrefs.Articles = saveLinkedTextFramesAsArticlesCheckBox->isChecked();
	prefsData->pdfPrefs.FontEmbedding = fontEmbeddingCombo->embeddingMode();
	prefsData->pdfPrefs.Encrypt = useEncryptionCheckBox->isChecked();
	prefsData->pdfPrefs.UseLPI = useCustomRenderingCheckBox->isChecked();
	prefsData->pdfPrefs.UseSpotColors = !convertSpotsToProcessCheckBox->isChecked();
	prefsData->pdfPrefs.doMultiFile = false;
	prefsData->pdfPrefs.cropMarks  = printCropMarksCheckBox->isChecked();
	prefsData->pdfPrefs.bleedMarks = printBleedMarksCheckBox->isChecked();
	prefsData->pdfPrefs.registrationMarks = printRegistrationMarksCheckBox->isChecked();
	prefsData->pdfPrefs.colorMarks = printColorBarsCheckBox->isChecked();
	prefsData->pdfPrefs.docInfoMarks = printPageInfoCheckBox->isChecked();
	prefsData->pdfPrefs.markLength   = markLengthSpinBox->value() / unitRatio;
	prefsData->pdfPrefs.markOffset   = markOffsetSpinBox->value() / unitRatio;
	prefsData->pdfPrefs.useDocBleeds = useDocumentBleedsCheckBox->isChecked();
	prefsData->pdfPrefs.bleeds = bleedsWidget->margins();
	prefsData->pdfPrefs.doClip = clipToPrinterMarginsCheckBox->isChecked();
	if (useEncryptionCheckBox->isChecked())
	{
		int Perm = -64;
		if (pdfVersionComboBox->version() == PDFVersion::PDF_14)
			Perm &= ~0x00240000;
		if (allowPrintingCheckBox->isChecked())
			Perm += 4;
		if (allowChangingCheckBox->isChecked())
			Perm += 8;
		if (allowCopyingCheckBox->isChecked())
			Perm += 16;
		if (allowAnnotatingCheckBox->isChecked())
			Perm += 32;
		prefsData->pdfPrefs.Permissions = Perm;
		prefsData->pdfPrefs.PassOwner = passwordOwnerLineEdit->text();
		prefsData->pdfPrefs.PassUser = passwordUserLineEdit->text();
	}
	prefsData->pdfPrefs.Version = pdfVersionComboBox->version();
	if (outputIntentionComboBox->currentIndex() == 0)
	{
		prefsData->pdfPrefs.isGrayscale = false;
		prefsData->pdfPrefs.UseRGB = true;
		prefsData->pdfPrefs.UseProfiles = false;
		prefsData->pdfPrefs.UseProfiles2 = false;
	}
	else
	{
		if (outputIntentionComboBox->currentIndex() == 2)
		{
			prefsData->pdfPrefs.isGrayscale = true;
			prefsData->pdfPrefs.UseRGB = false;
			prefsData->pdfPrefs.UseProfiles = false;
			prefsData->pdfPrefs.UseProfiles2 = false;
		}
		else
		{
			prefsData->pdfPrefs.isGrayscale = false;
			prefsData->pdfPrefs.UseRGB = false;
			if (/*CMSuse*/ ScCore->haveCMS())
			{
				prefsData->pdfPrefs.UseProfiles = useSolidColorProfileCheckBox->isChecked();
				prefsData->pdfPrefs.UseProfiles2 = useImageProfileCheckBox->isChecked();
				prefsData->pdfPrefs.Intent = solidColorRenderingIntentComboBox->currentIndex();
				prefsData->pdfPrefs.Intent2 = imageRenderingIntentComboBox->currentIndex();
				prefsData->pdfPrefs.EmbeddedI = doNotUseEmbeddedImageProfileCheckBox->isChecked();
				prefsData->pdfPrefs.SolidProf = solidColorProfileComboBox->currentText();
				prefsData->pdfPrefs.ImageProf = imageProfileComboBox->currentText();
				prefsData->pdfPrefs.PrintProf = pdfx3OutputProfileComboBox->currentText();
			}
		}
	}
}


void Prefs_PDFExport::SelLPIcol(int c)
{
	// XXX Optionen or Opts changed here
	Opts.LPISettings[SelLPIcolor].Frequency = customRenderingFrequencySpinBox->value();
	Opts.LPISettings[SelLPIcolor].Angle = customRenderingAngleSpinBox->value();
	Opts.LPISettings[SelLPIcolor].SpotFunc = customRenderingSpotFunctionComboBox->currentIndex();
	customRenderingFrequencySpinBox->setValue(Opts.LPISettings[customRenderingColorComboBox->itemText(c)].Frequency);
	customRenderingAngleSpinBox->setValue(Opts.LPISettings[customRenderingColorComboBox->itemText(c)].Angle);
	customRenderingSpotFunctionComboBox->setCurrentIndex(Opts.LPISettings[customRenderingColorComboBox->itemText(c)].SpotFunc);
	SelLPIcolor = customRenderingColorComboBox->itemText(c);
}

void Prefs_PDFExport::enableRangeControls(bool enabled)
{
	exportPageListLineEdit->setEnabled( enabled );
	exportRangeMorePushButton->setEnabled( enabled );
	if (!enabled)
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
	cmsEnabled = enabled;
	addPDFVersions(enabled);
	enableProfiles(outputIntentionComboBox->currentIndex());
}

void Prefs_PDFExport::enablePDFExportTabs(bool enabled)
{
	if (!enabled)
	{
		tabWidget->removeTab(tabWidget->indexOf(tabFonts));//Fonts
		tabWidget->removeTab(tabWidget->indexOf(tabExtras));//Extras
		tabWidget->removeTab(tabWidget->indexOf(tabViewer));//Viewer
	}
}

void Prefs_PDFExport::createPageNumberRange()
{
	if (m_doc != nullptr)
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
	exportPageListLineEdit->setText(QString());
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
	if (i == 1 && (!pdfVersionComboBox->versionIs(PDFVersion::PDF_X1a)))
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
			if (m_doc != nullptr)
				tp = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
			else
				tp = defaultSolidColorRGBProfile;
		}
		solidColorProfileComboBox->clear();
		ProfilesL::Iterator itp;
		ProfilesL::Iterator itpend = ScCore->InputProfiles.end();
		for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
		{
			solidColorProfileComboBox->addItem(itp.key());
			if (itp.key() == tp)
			{
				if (cmsEnabled)
					solidColorProfileComboBox->setCurrentIndex(solidColorProfileComboBox->count() - 1);
			}
		}
		if (cmsEnabled)
			solidColorRenderingIntentComboBox->setCurrentIndex(Opts.Intent);
		QString tp1 = Opts.ImageProf;
		if (!ScCore->InputProfiles.contains(tp1))
		{
			if (m_doc != nullptr)
				tp1 = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
			else
				tp1 = defaultSolidColorRGBProfile;
		}
		imageProfileComboBox->clear();
		ProfilesL::Iterator itp2;
		ProfilesL::Iterator itp2end = ScCore->InputProfiles.end();
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
		//Disabling vs hiding
		enableSolidsImagesWidgets(cmsEnabled);
		convertSpotsToProcessCheckBox->setEnabled(true);
		if (m_doc != nullptr)
		{
			useCustomRenderingCheckBox->setEnabled(true);
			enableCustomRenderingWidgets(useCustomRenderingCheckBox->isChecked());
		}
	}
	else
	{
		convertSpotsToProcessCheckBox->setEnabled(false);
		useCustomRenderingCheckBox->setEnabled(false);
		enableCustomRenderingWidgets(false);
	}
}


void Prefs_PDFExport::enableLPI2()
{
	if (m_doc != nullptr)
		enableCustomRenderingWidgets(useCustomRenderingCheckBox->isChecked());
}

void Prefs_PDFExport::setCustomRenderingWidgetsShown(bool visible)
{
	useCustomRenderingCheckBox->setVisible(visible);
	customRenderingColorComboBox->setVisible(visible);
	customRenderingFrequencySpinBox->setVisible(visible);
	customRenderingAngleSpinBox->setVisible(visible);
	customRenderingSpotFunctionComboBox->setVisible(visible);
}

void Prefs_PDFExport::enableCustomRenderingWidgets(bool enabled)
{
	customRenderingColorComboBox->setEnabled(enabled);
	customRenderingFrequencySpinBox->setEnabled(enabled);
	customRenderingAngleSpinBox->setEnabled(enabled);
	customRenderingSpotFunctionComboBox->setEnabled(enabled);
}

void Prefs_PDFExport::setSolidsImagesWidgetsShown(bool visible)
{
	solidColorsLabel->setVisible(visible);
	solidColorsLine->setVisible(visible);
	useSolidColorProfileCheckBox->setVisible(visible);
	solidColorProfileComboBox->setVisible(visible);
	solidColorRenderingIntentComboBox->setVisible(visible);
	imagesLabel->setVisible(visible);
	imagesLine->setVisible(visible);
	useImageProfileCheckBox->setVisible(visible);
	doNotUseEmbeddedImageProfileCheckBox->setVisible(visible);
	imageProfileComboBox->setVisible(visible);
	imageRenderingIntentComboBox->setVisible(visible);
}

void Prefs_PDFExport::enableSolidsImagesWidgets(bool enabled)
{
	//if we want to show/hide instead
	//setSolidsImagesWidgetsShown(enabled);
	bool enableSolidWidgets = false;
	useSolidColorProfileCheckBox->setEnabled(enabled);
	if (enabled)
		enableSolidWidgets = useSolidColorProfileCheckBox->isChecked();
	solidColorProfileComboBox->setEnabled(enableSolidWidgets);
	solidColorRenderingIntentComboBox->setEnabled(enableSolidWidgets);

	bool enableImageWidgets = false;
	useImageProfileCheckBox->setEnabled(enabled);
	if (enabled)
		enableImageWidgets = useImageProfileCheckBox->isChecked();
	doNotUseEmbeddedImageProfileCheckBox->setEnabled(enableImageWidgets);
	if (enableImageWidgets)
		enableImageWidgets = doNotUseEmbeddedImageProfileCheckBox->isChecked();
	imageProfileComboBox->setEnabled(enableImageWidgets);
	imageRenderingIntentComboBox->setEnabled(enableImageWidgets);
}

void Prefs_PDFExport::enablePDFXWidgets(bool enabled)
{
	pdfx3OutputProfileComboBox->setEnabled(enabled);
	pdfx3InfoStringLineEdit->setEnabled(enabled);
}


void Prefs_PDFExport::enablePGI()
{
	bool setter = false;
	if (useImageProfileCheckBox->isChecked())
		setter = doNotUseEmbeddedImageProfileCheckBox->isChecked();
	imageProfileComboBox->setEnabled(setter);
	imageRenderingIntentComboBox->setEnabled(setter);
	doNotUseEmbeddedImageProfileCheckBox->setEnabled(useImageProfileCheckBox->isChecked());
}

void Prefs_PDFExport::enablePGI2()
{
	bool setter = doNotUseEmbeddedImageProfileCheckBox->isChecked();
	imageProfileComboBox->setEnabled(setter);
	imageRenderingIntentComboBox->setEnabled(setter);
}

void Prefs_PDFExport::enablePG()
{
	bool setter = useSolidColorProfileCheckBox->isChecked();
	solidColorProfileComboBox->setEnabled(setter);
	solidColorRenderingIntentComboBox->setEnabled(setter);
}

void Prefs_PDFExport::enablePDFX(int)
{
	PDFVersion pdfVer = pdfVersionComboBox->version();

	includeLayersCheckBox->setEnabled(pdfVer.supportsOCGs());
	if (useLayersRadioButton)
		useLayersRadioButton->setEnabled(pdfVer.supportsOCGs());

	if (m_doc != nullptr && exportingPDF)
	{
		int currentEff = effectTypeComboBox->currentIndex();
		disconnect(effectTypeComboBox, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
		effectTypeComboBox->clear();
		effectTypeComboBox->addItem( tr("No Effect"));
		effectTypeComboBox->addItem( tr("Blinds"));
		effectTypeComboBox->addItem( tr("Box"));
		effectTypeComboBox->addItem( tr("Dissolve"));
		effectTypeComboBox->addItem( tr("Glitter"));
		effectTypeComboBox->addItem( tr("Split"));
		effectTypeComboBox->addItem( tr("Wipe"));
		if (pdfVer.supportsPDF15PresentationEffects())
		{
			effectTypeComboBox->addItem( tr("Push"));
			effectTypeComboBox->addItem( tr("Cover"));
			effectTypeComboBox->addItem( tr("Uncover"));
			effectTypeComboBox->addItem( tr("Fade"));
			effectTypeComboBox->setCurrentIndex(currentEff);
		}
		else
		{
			if (currentEff > 6)
			{
				currentEff = 0;
				effectTypeComboBox->setCurrentIndex(0);
				SetEffOpts(0);
				for (int pg = 0; pg<m_doc->Pages->count(); ++pg)
				{
					if (EffVal[pg].effectType > 6)
						EffVal[pg].effectType = 0;
				}
			}
			else
				effectTypeComboBox->setCurrentIndex(currentEff);
		}
		connect(effectTypeComboBox, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
	}

	if (!pdfVer.isPDFX())  // not PDF/X
	{
		checkEmbeddableFonts();
		fontEmbeddingCombo->setNoEmbeddingEnabled(true);
		enablePDFXWidgets(false);
		tabWidget->setTabEnabled(2, true);
		outputIntentionComboBox->setEnabled(true);
		useImageProfileCheckBox->setEnabled(true);
		emit hasInfo();
		if (m_doc != nullptr && exportingPDF)
		{
			enabledEffectsCheckBox->setEnabled(true);
			embedAllButton->setEnabled(true);
		}

		return;
	}
	// PDF/X is selected
	disconnect(outputIntentionComboBox, SIGNAL(activated(int)), this, SLOT(enableProfiles(int)));
	outputIntentionComboBox->setCurrentIndex(1);
	outputIntentionComboBox->setEnabled(false);
	enableProfiles(1);
	if (pdfVer == PDFVersion::PDF_X3 || pdfVer == PDFVersion::PDF_X4) // X3 or X4, enforcing color profiles on images
	{
		useImageProfileCheckBox->setChecked(true);
		useImageProfileCheckBox->setEnabled(false);
	}
	if (m_doc != nullptr && exportingPDF)
	{
		EmbedAll();
		enabledEffectsCheckBox->setChecked(false);
		enabledEffectsCheckBox->setEnabled(false);
		if (pdfx3InfoStringLineEdit->text().isEmpty())
			emit noInfo();
		else
			emit hasInfo();
	}

	PDFOptions::PDFFontEmbedding oldEmbeddingMode = fontEmbeddingCombo->embeddingMode();
	fontEmbeddingCombo->setNoEmbeddingEnabled(false);
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();

	if (oldEmbeddingMode != embeddingMode)
		embeddedFontsListWidget->clearSelection();
	embeddedFontsListWidget->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	embedAllButton->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	if (oldEmbeddingMode != embeddingMode)
		subsettedFontsListWidget->clearSelection();
	subsettedFontsListWidget->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	subsetAllButton->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	toSubsetButton->setEnabled(true);
	fromSubsetButton->setEnabled(true);

	enablePGI();
	pdfx3OutputProfileComboBox->setEnabled(true);
	pdfx3InfoStringLineEdit->setEnabled(true);
	tabWidget->setTabEnabled(2, false);
	connect(outputIntentionComboBox, SIGNAL(activated(int)), this, SLOT(enableProfiles(int)));
}

void Prefs_PDFExport::addPDFVersions(bool addPDFXStrings)
{
	disconnect(pdfVersionComboBox, SIGNAL(activated(int)), this, SLOT(enablePDFX(int)));
	PDFVersion currVersion = pdfVersionComboBox->version();
	pdfVersionComboBox->setPDFXEnabled(addPDFXStrings);
	if (!addPDFXStrings)
		currVersion = qMax(PDFVersion::PDF_13, qMin((PDFVersion::Version) currVersion, PDFVersion::PDF_16));
	pdfVersionComboBox->setVersion(currVersion);
	connect(pdfVersionComboBox, SIGNAL(activated(int)), this, SLOT(enablePDFX(int)));
}


void Prefs_PDFExport::enableEffects(bool enabled)
{
	effectsPageListWidget->setEnabled(enabled);
	showPagePreviewsCheckBox->setEnabled(enabled);
	displayDurationSpinBox->setEnabled(enabled);
	effectDurationSpinBox->setEnabled(enabled);
	effectTypeComboBox->setEnabled(enabled);
	if (enabled)
		SetEffOpts(effectTypeComboBox->currentIndex());
	else
	{
		effectMovingDirectionComboBox->setEnabled(false);
		effectInOutComboBox->setEnabled(false);
		effectDirectionComboBox->setEnabled(false);
		applyEffectToAllPagesPushButton->setEnabled(false);
	}
}

void Prefs_PDFExport::EmbedAll()
{
	PDFVersion pdfVer = pdfVersionComboBox->version();

	embeddedFontsListWidget->clear();
	subsettedFontsListWidget->clear();
	toSubsetButton->setEnabled(false);
	fromSubsetButton->setEnabled(false);

	QStringList docFonts;
	if (m_doc)
		docFonts = m_doc->usedFonts().keys();

	for (int i = 0; i < docFonts.count(); ++i)
	{
		const QString& fontName = docFonts.at(i);
		const ScFace fontFace = AllFonts[fontName];
		if (!fontFace.subset() && (!fontFace.isOTF() || pdfVer.supportsEmbeddedOpenTypeFonts()))
		{
			QListWidgetItem* item = addFontItem(fontName, embeddedFontsListWidget);
			if (AnnotationFonts.contains(item->text()))
				item->setFlags(Qt::ItemIsEnabled);
		}
		else if (AnnotationFonts.contains(fontName))
		{
			QListWidgetItem* item = addFontItem(fontName, embeddedFontsListWidget);
			item->setFlags(Qt::ItemIsEnabled);
		}
		else
		{
			addFontItem(fontName, subsettedFontsListWidget);
		}
	}
}

void Prefs_PDFExport::SubsetAll()
{
	embeddedFontsListWidget->clear();
	subsettedFontsListWidget->clear();
	toSubsetButton->setEnabled(false);
	fromSubsetButton->setEnabled(false);

	QStringList docFonts;
	if (m_doc)
		docFonts = m_doc->usedFonts().keys();

	for (int a = 0; a < docFonts.count(); ++a)
	{
		const QString& fontName = docFonts.at(a);
		if (AnnotationFonts.contains(fontName))
		{
			QListWidgetItem* item = addFontItem(fontName, embeddedFontsListWidget);
			item->setFlags(Qt::ItemIsEnabled);
		}
		else
		{
			addFontItem(fontName, subsettedFontsListWidget);
		}
	}
}

void Prefs_PDFExport::OutlineAll()
{
	// Nothing to do at this point
}

void Prefs_PDFExport::checkEmbeddableFonts()
{
	PDFVersion pdfVer = pdfVersionComboBox->version();

	for (int i = 0; i < embeddedFontsListWidget->count(); ++i)
	{
		QListWidgetItem* item = embeddedFontsListWidget->item(i);
		QString fontName = item->text();
		if (AnnotationFonts.contains(fontName))
			continue;
		const ScFace fontFace = AllFonts[fontName];
		if (fontFace.isOTF() && !pdfVer.supportsEmbeddedOpenTypeFonts())
		{
			delete embeddedFontsListWidget->takeItem(i);
			addFontItem(fontName, subsettedFontsListWidget);
			--i;
		}
	}
}

void Prefs_PDFExport::doDocBleeds()
{
	if (useDocumentBleedsCheckBox->isChecked())
	{
		Opts.bleeds = bleedsWidget->margins();
		bleedsWidget->setNewValues(m_doc->bleedsVal());
		bleedsWidget->setEnabled(false);
	}
	else
	{
		bleedsWidget->setNewValues(Opts.bleeds);
		bleedsWidget->setEnabled(true);
	}
}


void Prefs_PDFExport::SetEffOpts(int nr)
{
	QStandardItem* si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(2));
	if (si)
		si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(3));
	if (si)
		si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(4));
	if (si)
		si->setSelectable(false);
	switch (nr)
	{
	case 0:
	case 3:
	case 10:
		effectMovingDirectionComboBox->setEnabled(false);
		effectInOutComboBox->setEnabled(false);
		effectDirectionComboBox->setEnabled(false);
		break;
	case 1:
		effectMovingDirectionComboBox->setEnabled(true);
		effectInOutComboBox->setEnabled(false);
		effectDirectionComboBox->setEnabled(false);
		break;
	case 2:
		effectMovingDirectionComboBox->setEnabled(false);
		effectInOutComboBox->setEnabled(true);
		effectDirectionComboBox->setEnabled(false);
		break;
	case 4:
	case 6:
	case 7:
	case 8:
	case 9:
		effectMovingDirectionComboBox->setEnabled(false);
		effectInOutComboBox->setEnabled(false);
		effectDirectionComboBox->setEnabled(true);
		if (nr == 6)
		{
			si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(2));
			if (si)
				si->setSelectable(true);
			si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(3));
			if (si)
				si->setSelectable(true);
		}
		else
		{
			si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(4));
			if (si)
				si->setSelectable(true);
		}
		break;
	case 5:
		effectMovingDirectionComboBox->setEnabled(true);
		effectInOutComboBox->setEnabled(true);
		effectDirectionComboBox->setEnabled(false);
		break;
	}
}

void Prefs_PDFExport::SelEFont(QListWidgetItem *c)
{
	if (c != nullptr)
	{
		if (c->flags() & Qt::ItemIsSelectable)
			toSubsetButton->setEnabled(true);
		fromSubsetButton->setEnabled(false);
		subsettedFontsListWidget->clearSelection();
	}
}

void Prefs_PDFExport::SelSFont(QListWidgetItem *c)
{
	if (!c)
		return;

	if (pdfVersionComboBox->versionIsPDFX())
	{
		if ((AllFonts[c->text()].type() == ScFace::OTF) || (AllFonts[c->text()].subset()))
			fromSubsetButton->setEnabled(false);
		else
			fromSubsetButton->setEnabled(true);
	}
	else
		fromSubsetButton->setEnabled(true);
	toSubsetButton->setEnabled(false);
	embeddedFontsListWidget->clearSelection();
}

void Prefs_PDFExport::PagePr()
{
	disconnect(effectsPageListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetPgEff()));
	QString tmp;
	QPixmap pm;
	int ci = effectsPageListWidget->currentRow();
	int pgMaxX = 0;
	int pgMaxY = 0;
	effectsPageListWidget->clear();
	if (showPagePreviewsCheckBox->isChecked())
	{
		for (int pg = 0; pg < m_doc->Pages->count(); ++pg)
		{
			pm = QPixmap::fromImage(m_doc->view()->PageToPixmap(pg, 70));
			pgMaxX = qMax(pgMaxX, pm.width());
			pgMaxY = qMax(pgMaxY, pm.height());
			new QListWidgetItem( pm, tr("Page")+" "+tmp.setNum(pg+1), effectsPageListWidget);
		}
		effectsPageListWidget->setIconSize(QSize(pgMaxX, pgMaxY));
	}
	else
	{
		for (int pg = 0; pg < m_doc->Pages->count(); ++pg)
		{
			new QListWidgetItem( tr("Page")+" "+tmp.setNum(pg+1), effectsPageListWidget);
		}
	}
	if (ci != -1)
		effectsPageListWidget->setCurrentRow(ci);
	else
		effectsPageListWidget->clearSelection();
	connect(effectsPageListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SetPgEff()));
}

void Prefs_PDFExport::DoDownsample()
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

void Prefs_PDFExport::EmbeddingModeChange()
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();

	embeddedFontsListWidget->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	embedAllButton->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	subsettedFontsListWidget->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	subsetAllButton->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	toSubsetButton->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	fromSubsetButton->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
}

void Prefs_PDFExport::RemoveSubset()
{
	PDFVersion pdfVer = pdfVersionComboBox->version();

	QList<QListWidgetItem*> selection = subsettedFontsListWidget->selectedItems();
	for (int i = 0; i < selection.count() ; ++i)
	{
		QListWidgetItem* fontItem = selection[i];
		QString currentFont = fontItem->text();
		const ScFace fontFace = AllFonts[currentFont];
		if (fontFace.isOTF() && !pdfVer.supportsEmbeddedOpenTypeFonts())
			continue;
		if (fontFace.subset())
			continue;
		addFontItem(currentFont, embeddedFontsListWidget);
		int currentRow = subsettedFontsListWidget->row(fontItem);
		delete subsettedFontsListWidget->takeItem(currentRow);
	}

	subsettedFontsListWidget->clearSelection();
	fromSubsetButton->setEnabled(false);
}

void Prefs_PDFExport::PutToSubset()
{
	QList<QListWidgetItem*> selection = embeddedFontsListWidget->selectedItems();
	for (int i = 0; i < selection.count() ; ++i)
	{
		QListWidgetItem* fontItem = selection[i];
		QString currentFont = fontItem->text();
		if (subsettedFontsListWidget->count() != 0)
		{
			if (subsettedFontsListWidget->findItems(currentFont, Qt::MatchExactly).count() == 0)
				addFontItem(currentFont, subsettedFontsListWidget);
		}
		else
		{
			addFontItem(currentFont, subsettedFontsListWidget);
		}
		int itemRow = embeddedFontsListWidget->row(fontItem);
		delete embeddedFontsListWidget->takeItem(itemRow);
	}

	embeddedFontsListWidget->clearSelection();
	toSubsetButton->setEnabled(false);
}
