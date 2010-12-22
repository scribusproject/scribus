/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QStandardItem>
#include <QAbstractItemView>

#include "ui/prefs_pdfexport.h"
#include "ui/createrange.h"
#include "prefsstructs.h"

#include "scribuscore.h"
#include "scribusdoc.h"
#include "ui/scrspinbox.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"

Prefs_PDFExport::Prefs_PDFExport(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	cmsEnabled(false),
	m_doc(doc),
	exportingPDF(false)
{
	setupUi(this);
	pageMirrorHorizontalToolButton->setIcon(QIcon(loadIcon("16/flip-object-horizontal.png")));
	pageMirrorVerticalToolButton->setIcon(QIcon(loadIcon("16/flip-object-vertical.png")));
	unitChange(0);
	languageChange();

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
											"a range of pages or a single page number.") + "</qt>" );

	pdfVersionComboBox->setToolTip( "<qt>" + tr( "Determines the PDF compatibility.<br/>The default is <b>PDF 1.3</b> which gives the widest compatibility.<br/>Choose <b>PDF 1.4</b> if your file uses features such as transparency or you require 128 bit encryption.<br/><b>PDF 1.5</b> is necessary when you wish to preserve objects in separate layers within the PDF.<br/><b>PDF/X-3</b> is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.<br/><b>PDF/X-1a</b> is for blind exchange with colors strictly specified in CMYK or spot colors.<br/><b>PDF/X-4</b> is an extension of PDF/X-3 to support transparancy and layering." ) + "</qt>");
	pageBindingComboBox->setToolTip( "<qt>" + tr( "Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left." ) + "</qt>" );
	generateThumbnailsCheckBox->setToolTip( "<qt>" + tr( "Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation." ) + "</qt>" );
	saveLinkedTextFramesAsArticlesCheckBox->setToolTip( "<qt>" + tr( "Generate PDF Articles, which is useful for navigating linked articles in a PDF." ) + "</qt>" );
	includeLayersCheckBox->setToolTip( "<qt>" + tr( "Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen." ) + "</qt>" );
	includeBookmarksCheckBox->setToolTip( "<qt>" + tr( "Embed the bookmarks you created in your document. These are useful for navigating long PDF documents." ) + "</qt>" );
	epsExportResolutionSpinBox->setToolTip( "<qt>" + tr( "Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos." ) + "</qt>" );
	embedPDFAndEPSFilesCheckBox->setToolTip( "<qt>" + tr( "Export PDFs in image frames as embedded PDFs. This does *not* yet take care of colorspaces, so you should know what you are doing before setting this to 'true'." ) + "</qt>" );
	compressTextAndVectorGraphicsCheckBox->setToolTip( "<qt>" + tr( "Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size." ) + "</qt>" );
	imageCompressionMethodComboBox->setToolTip( "<qt>" + tr( "Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options." ) + "</qt>");
	imageCompressionQualityComboBox->setToolTip( "<qt>" + tr( "Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg." ) + "</qt>");
	maxResolutionLimitCheckBox->setToolTip( "<qt>" + tr( "Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export." ) + "</qt>" );
	maxExportResolutionSpinBox->setToolTip( "<qt>" + tr( "DPI (Dots Per Inch) for image export.") + "</qt>" );
	useEncryptionCheckBox->setToolTip( "<qt>" + tr( "Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations." ) + "</qt>" );
	passwordOwnerLineEdit->setToolTip( "<qt>" + tr( "Choose a master password which enables or disables all the security features in your exported PDF" ) + "</qt>" );
	passwordUserLineEdit->setToolTip( "<qt>" + tr( "Choose a password for users to be able to read your PDF." ) + "</qt>" );
	allowPrintingCheckBox->setToolTip( "<qt>" + tr( "Allow printing of the PDF. If un-checked, printing is prevented. " ) + "</qt>" );
	allowChangingCheckBox->setToolTip( "<qt>" + tr( "Allow modifying of the PDF. If un-checked, modifying the PDF is prevented." ) + "</qt>" );
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
	registrationMarkOffsetSpinBox->setToolTip( "<qt>" + tr( "Indicate the distance offset for the registration marks" ) + "</qt>" );
//	BleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
//	BleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
//	BleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
//	BleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	useDocumentBleedsCheckBox->setToolTip( "<qt>" + tr( "Use the existing bleed settings from the document preferences" ) + "</qt>" );
	pdfx3OutputProfileComboBox->setToolTip( "<qt>" + tr( "Output profile for printing. If possible, get some guidance from your printer on profile selection." ) + "</qt>" );
	pdfx3InfoStringLineEdit->setToolTip( "<qt>" + tr( "Mandatory string for PDF/X or the PDF will fail PDF/X conformance. We recommend you use the title of the document." ) + "</qt>" );

}

Prefs_PDFExport::~Prefs_PDFExport()
{
}


void Prefs_PDFExport::unitChange(int unitIndex)
{
	unitRatio = unitGetRatioFromIndex(unitIndex);
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
	SelLPIcolor = customRenderingColorComboBox->currentText();

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

void Prefs_PDFExport::restoreDefaults(struct ApplicationPrefs *prefsData, const ProfilesL & PDFXProfiles, bool exporting)
{
	exportingPDF=exporting;
	enablePDFExportTabs(exportingPDF);
	AllFonts=prefsData->fontPrefs.AvailFonts;
	int unitIndex = prefsData->docSetupPrefs.docUnitIndex;
	unitRatio = unitGetRatioFromIndex(unitIndex);
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
	embedPDFAndEPSFilesCheckBox->setChecked(prefsData->pdfPrefs.embedPDF);
	compressTextAndVectorGraphicsCheckBox->setChecked( prefsData->pdfPrefs.Compress );
	imageCompressionMethodComboBox->setCurrentIndex(prefsData->pdfPrefs.CompressMethod);
	imageCompressionQualityComboBox->setCurrentIndex(prefsData->pdfPrefs.Quality);
	maxResolutionLimitCheckBox->setChecked(prefsData->pdfPrefs.RecalcPic);
	maxExportResolutionSpinBox->setValue(prefsData->pdfPrefs.PicRes);
	maxExportResolutionSpinBox->setEnabled(prefsData->pdfPrefs.RecalcPic);

	if (m_doc != 0 && exportingPDF)
	{
//	Build a list of all Fonts used in Annotations;
		PageItem *pgit;
		QList<PageItem*> allItems;
		for (int a = 0; a < m_doc->FrameItems.count(); ++a)
		{
			PageItem *currItem = m_doc->FrameItems.at(a);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				pgit = allItems.at(ii);
				if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()) && (pgit->itemText.length() > 0))
					AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
			}
			allItems.clear();
		}
		for (int a = 0; a < m_doc->MasterItems.count(); ++a)
		{
			PageItem *currItem = m_doc->MasterItems.at(a);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				pgit = allItems.at(ii);
				if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()) && (pgit->itemText.length() > 0))
					AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
			}
			allItems.clear();
		}
		for (int a = 0; a < m_doc->DocItems.count(); ++a)
		{
			PageItem *currItem = m_doc->DocItems.at(a);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				pgit = allItems.at(ii);
				if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()) && (pgit->itemText.length() > 0))
					AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
			}
			allItems.clear();
		}
		QMap<QString,int>::const_iterator it;
		availableFontsListWidget->clear();
		for (it = m_doc->usedFonts().constBegin(); it != m_doc->usedFonts().constEnd(); ++it)
		{
			if (AllFonts[it.key()].isReplacement())
				new QListWidgetItem( QIcon(loadIcon("font_subst16.png")), it.key(), availableFontsListWidget );
			else if (AllFonts[it.key()].type() == ScFace::TYPE1)
				new QListWidgetItem( QIcon(loadIcon("font_type1_16.png")), it.key(), availableFontsListWidget );
			else if (AllFonts[it.key()].type() == ScFace::TTF)
				new QListWidgetItem( QIcon(loadIcon("font_truetype16.png")), it.key(), availableFontsListWidget );
			else if (AllFonts[it.key()].type() == ScFace::OTF)
				new QListWidgetItem( QIcon(loadIcon("font_otf16.png")), it.key(), availableFontsListWidget );
		}
		toEmbedButton->setEnabled(false);
		fromEmbedButton->setEnabled(false);
		toOutlineButton->setEnabled(false);
		fromOutlineButton->setEnabled(false);
		if ((Opts.EmbedList.count() == 0) && (Opts.SubsetList.count() == 0) && (Opts.firstUse))
			EmbedAll();
		else
		{
			embeddedFontsListWidget->clear();
			FontsToEmbed.clear();
			for (int fe = 0; fe < Opts.EmbedList.count(); ++fe)
			{
				embeddedFontsListWidget->addItem(Opts.EmbedList[fe]);
				FontsToEmbed.append(Opts.EmbedList[fe]);
			}
			if (Opts.SubsetList.count() != 0)
			{
				outlinedFontsListWidget->clear();
				FontsToOutline.clear();
				for (int fe = 0; fe < Opts.SubsetList.count(); ++fe)
				{
					outlinedFontsListWidget->addItem(Opts.SubsetList[fe]);
					FontsToOutline.append(Opts.SubsetList[fe]);
				}
			}
			QMap<QString, QString>::Iterator itAnn;
			for (itAnn = AnnotationFonts.begin(); itAnn != AnnotationFonts.end(); ++itAnn)
			{
				if (FontsToEmbed.contains(itAnn.key()) == 0)
				{
					embeddedFontsListWidget->addItem(itAnn.key());
					embeddedFontsListWidget->item(embeddedFontsListWidget->count()-1)->setFlags(Qt::ItemIsEnabled);
					FontsToEmbed.append(itAnn.key());
				}
				if (FontsToOutline.contains(itAnn.key()) != 0)
				{
					FontsToOutline.removeAll(itAnn.key());
					QList<QListWidgetItem *> itR = outlinedFontsListWidget->findItems(itAnn.key(), Qt::MatchExactly);
					delete outlinedFontsListWidget->takeItem(outlinedFontsListWidget->row(itR.at(0)));
				}
			}
		}
		enabledEffectsCheckBox->setChecked(Opts.PresentMode);
		showPagePreviewsCheckBox->setChecked(false);
		effectsPageListWidget->clear();
		QString tmp;
		struct PDFPresentationData ef;
		if (EffVal.count() != 0)
		{
			for (int pg2 = 0; pg2 < m_doc->Pages->count(); ++pg2)
			{
				effectsPageListWidget->addItem( tr("Page")+" "+tmp.setNum(pg2+1));
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
			for (int pg = 0; pg < m_doc->Pages->count(); ++pg)
			{
				effectsPageListWidget->addItem( tr("Page")+" "+tmp.setNum(pg+1));
				ef.pageEffectDuration = 1;
				ef.pageViewDuration = 1;
				ef.effectType = 0;
				ef.Dm = 0;
				ef.M = 0;
				ef.Di = 0;
				EffVal.append(ef);
			}
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
			doublePageLeftRadioButton->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::TwoColumnRight)
			doublePageRightRadioButton->setChecked(true);
		if ((Opts.Version == PDFOptions::PDFVersion_15) || (Opts.Version == PDFOptions::PDFVersion_X4))
			useLayersRadioButton->setEnabled(true);
		else
			useLayersRadioButton->setEnabled(false);
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
		if (m_doc != 0 && exportingPDF)
			tp = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
		else
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
		if (m_doc != 0 && exportingPDF)
			tp1 = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
		else
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
		if (m_doc != 0 && exportingPDF)
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
		if (m_doc != 0 && exporting)
		{
			QFileInfo fi(m_doc->DocName);
			pdfx3InfoStringLineEdit->setText(fi.fileName());
		}
		else
			pdfx3InfoStringLineEdit->setText( tr("InfoString"));
	}

	if (m_doc != 0 && exportingPDF)
	{
		useDocumentBleedsCheckBox->setChecked(Opts.useDocBleeds);
		doDocBleeds();
	}
	else
		useDocumentBleedsCheckBox->hide();

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

	if (m_doc != 0  && exportingPDF)
	{
		effectTypeComboBox->clear();
		effectTypeComboBox->addItem( tr("No Effect"));
		effectTypeComboBox->addItem( tr("Blinds"));
		effectTypeComboBox->addItem( tr("Box"));
		effectTypeComboBox->addItem( tr("Dissolve"));
		effectTypeComboBox->addItem( tr("Glitter"));
		effectTypeComboBox->addItem( tr("Split"));
		effectTypeComboBox->addItem( tr("Wipe"));
		if (Opts.Version == PDFOptions::PDFVersion_15)
		{
			effectTypeComboBox->addItem( tr("Push"));
			effectTypeComboBox->addItem( tr("Cover"));
			effectTypeComboBox->addItem( tr("Uncover"));
			effectTypeComboBox->addItem( tr("Fade"));
		}
		PgSel = 0;
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

	if (m_doc != 0 && exportingPDF)
	{
		connect(embedAllButton, SIGNAL(clicked()), this, SLOT(EmbedAll()));
		connect(availableFontsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelAFont(QListWidgetItem*)));
		connect(embeddedFontsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelEFont(QListWidgetItem*)));
		connect(toEmbedButton, SIGNAL(clicked()), this, SLOT(PutToEmbed()));
		connect(fromEmbedButton, SIGNAL(clicked()), this, SLOT(RemoveEmbed()));
		connect(outlineAllButton, SIGNAL(clicked()), this, SLOT(OutlineAll()));
		connect(outlinedFontsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelSFont(QListWidgetItem*)));
		connect(toOutlineButton, SIGNAL(clicked()), this, SLOT(PutToOutline()));
		connect(fromOutlineButton, SIGNAL(clicked()), this, SLOT(RemoveOutline()));
		connect(showPagePreviewsCheckBox, SIGNAL(clicked()), this, SLOT(PagePr()));
		connect(effectsPageListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetPgEff()));
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
		effectTypeComboBox->setToolTip( "<qt>" + tr( "Type of the display effect." ) + "</qt>" );
		effectMovingDirectionComboBox->setToolTip( "<qt>" + tr( "Direction of the effect of moving lines for the split and blind effects." ) + "</qt>" );
		effectInOutComboBox->setToolTip( "<qt>" + tr( "Starting position for the box and split effects." ) + "</qt>" );
		effectDirectionComboBox->setToolTip( "<qt>" + tr( "Direction of the glitter or wipe effects." ) + "</qt>" );
		applyEffectToAllPagesPushButton->setToolTip( "<qt>" + tr( "Apply the selected effect to all pages." ) + "</qt>" );
		outlineAllButton->setToolTip( "<qt>" + tr("Convert all glyphs in the document to outlines.") + "</qt>");
		singlePageRadioButton->setToolTip( "<qt>" + tr( "Show the document in single page mode" ) + "</qt>" );
		continuousPagesRadioButton->setToolTip( "<qt>" + tr( "Show the document in single page mode with the pages displayed continuously end to end like a scroll" ) + "</qt>" );
		doublePageLeftRadioButton->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the left" ) + "</qt>" );
		doublePageRightRadioButton->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the right" ) + "</qt>" );
		useViewerDefaultsRadioButton->setToolTip( "<qt>" + tr( "Use the viewer's defaults or the user's preferences if set differently from the viewer defaults" ) + "</qt>" );
		useFullScreenRadioButton->setToolTip( "<qt>" + tr( "Enables viewing the document in full screen" ) + "</qt>" );
		useBookmarksRadioButton->setToolTip( "<qt>" + tr( "Display the bookmarks upon opening" ) + "</qt>" );
		useThumbnailsRadioButton->setToolTip( "<qt>" + tr( "Display the page thumbnails upon opening" ) + "</qt>" );
		useLayersRadioButton->setToolTip( "<qt>" + tr( "Forces the displaying of layers. Useful only for PDF 1.5+." ) + "</qt>" );
		hideViewerToolBarCheckBox->setToolTip( "<qt>" + tr( "Hides the Tool Bar which has selection and other editing capabilities" ) + "</qt>" );
		hideViewerMenuBarCheckBox->setToolTip( "<qt>" + tr( "Hides the Menu Bar for the viewer, the PDF will display in a plain window. " ) + "</qt>" );
		fitViewerWindowCheckBox->setToolTip( "<qt>" + tr( "Fit the document page or pages to the available space in the viewer window." ) + "</qt>" );
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
	prefsData->pdfPrefs.Bookmarks = includeBookmarksCheckBox->isChecked();
	prefsData->pdfPrefs.Binding = pageBindingComboBox->currentIndex();
	prefsData->pdfPrefs.MirrorH = pageMirrorHorizontalToolButton->isChecked();
	prefsData->pdfPrefs.MirrorV = pageMirrorVerticalToolButton->isChecked();
	prefsData->pdfPrefs.RotateDeg = rotationComboBox->currentIndex() * 90;
	prefsData->pdfPrefs.Articles = saveLinkedTextFramesAsArticlesCheckBox->isChecked();
	prefsData->pdfPrefs.Encrypt = useEncryptionCheckBox->isChecked();
	prefsData->pdfPrefs.UseLPI = useCustomRenderingCheckBox->isChecked();
	prefsData->pdfPrefs.UseSpotColors = !convertSpotsToProcessCheckBox->isChecked();
	prefsData->pdfPrefs.doMultiFile = false;
	prefsData->pdfPrefs.cropMarks  = printCropMarksCheckBox->isChecked();
	prefsData->pdfPrefs.bleedMarks = printBleedMarksCheckBox->isChecked();
	prefsData->pdfPrefs.registrationMarks = printRegistrationMarksCheckBox->isChecked();
	prefsData->pdfPrefs.colorMarks = printColorBarsCheckBox->isChecked();
	prefsData->pdfPrefs.docInfoMarks = printPageInfoCheckBox->isChecked();
	prefsData->pdfPrefs.markOffset = registrationMarkOffsetSpinBox->value() / unitRatio;
	prefsData->pdfPrefs.useDocBleeds = useDocumentBleedsCheckBox->isChecked();
	prefsData->pdfPrefs.bleeds=bleedsWidget->margins();
	prefsData->pdfPrefs.doClip = clipToPrinterMarginsCheckBox->isChecked();
	if (useEncryptionCheckBox->isChecked())
	{
		int Perm = -64;
		if (pdfVersionComboBox->currentIndex() == 1)
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
	if (pdfVersionComboBox->currentIndex() == 0)
		prefsData->pdfPrefs.Version = PDFOptions::PDFVersion_13;
	if (pdfVersionComboBox->currentIndex() == 1)
		prefsData->pdfPrefs.Version = PDFOptions::PDFVersion_14;
	if (pdfVersionComboBox->currentIndex() == 2)
		prefsData->pdfPrefs.Version = PDFOptions::PDFVersion_15;
	if (pdfVersionComboBox->currentIndex() == 3)
		prefsData->pdfPrefs.Version = PDFOptions::PDFVersion_X1a;
	if (pdfVersionComboBox->currentIndex() == 4)
		prefsData->pdfPrefs.Version = PDFOptions::PDFVersion_X3;
	if (pdfVersionComboBox->currentIndex() == 5)
		prefsData->pdfPrefs.Version = PDFOptions::PDFVersion_X4;
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
	enableProfiles(enabled);
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
				tp = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
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
				tp1 = m_doc->cmsSettings().DefaultSolidColorRGBProfile;
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
		//Disabling vs hiding
		enableSolidsImagesWidgets(cmsEnabled);
		convertSpotsToProcessCheckBox->setEnabled(true);
		if (m_doc!=0)
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
	if (m_doc!=0)
		enableCustomRenderingWidgets(useCustomRenderingCheckBox->isChecked());
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
	//if we want to show/hide instead
	//setSolidsImagesWidgetsShown(enabled);
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
	if (useLayersRadioButton)
		useLayersRadioButton->setEnabled((i == 2) || (i == 5));
	if (m_doc != 0 && exportingPDF)
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
		if (i == 2)
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

	if (i < 3)  // not PDF/X
	{
		enablePDFXWidgets(false);
		tabWidget->setTabEnabled(2, true);
		outputIntentionComboBox->setEnabled(true);
		useImageProfileCheckBox->setEnabled(true);
		emit hasInfo();
		if (m_doc != 0 && exportingPDF)
		{
			enabledEffectsCheckBox->setEnabled(true);
			embedAllButton->setEnabled(true);
			if (embeddedFontsListWidget->count() != 0)
				fromEmbedButton->setEnabled(true);
			toEmbedButton->setEnabled(true);
		}

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
	if (m_doc != 0 && exportingPDF)
	{
//		EmbedFonts->setChecked(true);
		EmbedAll();
		enabledEffectsCheckBox->setChecked(false);
		enabledEffectsCheckBox->setEnabled(false);
//		EmbedFonts->setEnabled(false);
		fromEmbedButton->setEnabled(false);
		toEmbedButton->setEnabled(false);
		if (pdfx3InfoStringLineEdit->text().isEmpty())
			emit noInfo();
		else
			emit hasInfo();
	}
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
	embeddedFontsListWidget->clear();
	FontsToEmbed.clear();
	outlinedFontsListWidget->clear();
	FontsToOutline.clear();
	fromEmbedButton->setEnabled(false);
	toEmbedButton->setEnabled(false);
	toOutlineButton->setEnabled(false);
	fromOutlineButton->setEnabled(false);
	for (int a=0; a < availableFontsListWidget->count(); ++a)
	{
		if (availableFontsListWidget->item(a)->flags() & Qt::ItemIsSelectable)
		{
			if (!AllFonts[availableFontsListWidget->item(a)->text()].subset())
			{
				FontsToEmbed.append(availableFontsListWidget->item(a)->text());
				embeddedFontsListWidget->addItem(availableFontsListWidget->item(a)->text());
				if (AnnotationFonts.contains(availableFontsListWidget->item(a)->text()))
					embeddedFontsListWidget->item(embeddedFontsListWidget->count()-1)->setFlags(Qt::ItemIsEnabled);
			}
			else
			{
				if (AnnotationFonts.contains(availableFontsListWidget->item(a)->text()))
				{
					FontsToEmbed.append(availableFontsListWidget->item(a)->text());
					embeddedFontsListWidget->addItem(availableFontsListWidget->item(a)->text());
					embeddedFontsListWidget->item(embeddedFontsListWidget->count()-1)->setFlags(Qt::ItemIsEnabled);
				}
				else
				{
					FontsToOutline.append(availableFontsListWidget->item(a)->text());
					outlinedFontsListWidget->addItem(availableFontsListWidget->item(a)->text());
				}
			}
		}
	}
}

void Prefs_PDFExport::OutlineAll()
{
	embeddedFontsListWidget->clear();
	FontsToEmbed.clear();
	outlinedFontsListWidget->clear();
	FontsToOutline.clear();
	fromEmbedButton->setEnabled(false);
	toEmbedButton->setEnabled(false);
	toOutlineButton->setEnabled(false);
	fromOutlineButton->setEnabled(false);
	for (int a=0; a < availableFontsListWidget->count(); ++a)
	{
		if (availableFontsListWidget->item(a)->flags() & Qt::ItemIsSelectable)
		{
			if (AnnotationFonts.contains(availableFontsListWidget->item(a)->text()))
			{
				FontsToEmbed.append(availableFontsListWidget->item(a)->text());
				embeddedFontsListWidget->addItem(availableFontsListWidget->item(a)->text());
				embeddedFontsListWidget->item(embeddedFontsListWidget->count()-1)->setFlags(Qt::ItemIsEnabled);
			}
			else
			{
				FontsToOutline.append(availableFontsListWidget->item(a)->text());
				outlinedFontsListWidget->addItem(availableFontsListWidget->item(a)->text());
			}
		}
	}
}

void Prefs_PDFExport::doDocBleeds()
{
	if (useDocumentBleedsCheckBox->isChecked())
	{
		Opts.bleeds=bleedsWidget->margins();
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
	if (si) si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(3));
	if (si) si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(4));
	if (si) si->setSelectable(false);
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
			if (si) si->setSelectable(true);
			si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(3));
			if (si) si->setSelectable(true);
		}
		else
		{
			si = dynamic_cast<QStandardItem*>(effectDirectionComboBox->view()->children().at(4));
			if (si) si->setSelectable(true);
		}
		break;
	case 5:
		effectMovingDirectionComboBox->setEnabled(true);
		effectInOutComboBox->setEnabled(true);
		effectDirectionComboBox->setEnabled(false);
		break;
	}
}

void Prefs_PDFExport::SelAFont(QListWidgetItem *c)
{
	if (c != NULL)
	{
		fromEmbedButton->setEnabled(false);
		if (c->flags() & Qt::ItemIsSelectable)
			toEmbedButton->setEnabled(true);
		toOutlineButton->setEnabled(false);
		fromOutlineButton->setEnabled(false);
		embeddedFontsListWidget->clearSelection();
		outlinedFontsListWidget->clearSelection();
	}
}

void Prefs_PDFExport::SelEFont(QListWidgetItem *c)
{
	if (c != NULL)
	{
		if ((pdfVersionComboBox->currentIndex() < 3) && (c->flags() & Qt::ItemIsSelectable))
			fromEmbedButton->setEnabled(true);
		else
			fromEmbedButton->setEnabled(false);
		toEmbedButton->setEnabled(false);
		if (c->flags() & Qt::ItemIsSelectable)
			toOutlineButton->setEnabled(true);
		fromOutlineButton->setEnabled(false);
		availableFontsListWidget->clearSelection();
		outlinedFontsListWidget->clearSelection();
	}
}

void Prefs_PDFExport::SelSFont(QListWidgetItem *c)
{
	if (c != NULL)
	{
		if (pdfVersionComboBox->currentIndex() == 4)
		{
			if ((AllFonts[c->text()].type() == ScFace::OTF) || (AllFonts[c->text()].subset()))
				fromOutlineButton->setEnabled(false);
			else
				fromOutlineButton->setEnabled(true);
		}
		else
			fromOutlineButton->setEnabled(true);
		toOutlineButton->setEnabled(false);
		toEmbedButton->setEnabled(false);
		fromEmbedButton->setEnabled(false);
		embeddedFontsListWidget->clearSelection();
		availableFontsListWidget->clearSelection();
	}
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
			pm=QPixmap::fromImage(m_doc->view()->PageToPixmap(pg, 70));
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
	{
		PgSel = ci;
		effectsPageListWidget->setCurrentRow(ci);
	}
	else
	{
		PgSel = 0;
		effectsPageListWidget->clearSelection();
	}
	connect(effectsPageListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetPgEff()));
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

void Prefs_PDFExport::RemoveEmbed()
{
	FontsToEmbed.removeAll(embeddedFontsListWidget->currentItem()->text());
	delete embeddedFontsListWidget->takeItem(embeddedFontsListWidget->currentRow());
	embeddedFontsListWidget->clearSelection();
	if (embeddedFontsListWidget->count() == 0)
	{
		fromEmbedButton->setEnabled(false);
		toOutlineButton->setEnabled(false);
	}
	else
	{
		if (!embeddedFontsListWidget->item(embeddedFontsListWidget->currentRow())->flags() & Qt::ItemIsSelectable)
		{
			fromEmbedButton->setEnabled(false);
			toOutlineButton->setEnabled(false);
		}
	}
}

void Prefs_PDFExport::PutToEmbed()
{
	if (embeddedFontsListWidget->count() != 0)
	{
		if (!AllFonts[availableFontsListWidget->currentItem()->text()].subset())
		{
			if (embeddedFontsListWidget->findItems(availableFontsListWidget->currentItem()->text(), Qt::MatchExactly).count() == 0)
			{
				FontsToEmbed.append(availableFontsListWidget->currentItem()->text());
				embeddedFontsListWidget->addItem(availableFontsListWidget->currentItem()->text());
				if (AnnotationFonts.contains(availableFontsListWidget->currentItem()->text()))
					embeddedFontsListWidget->item(embeddedFontsListWidget->count()-1)->setFlags(Qt::ItemIsEnabled);
			}
		}
		else
		{
			if (outlinedFontsListWidget->count() != 0)
			{
				if (outlinedFontsListWidget->findItems(availableFontsListWidget->currentItem()->text(), Qt::MatchExactly).count() == 0)
				{
					FontsToOutline.append(availableFontsListWidget->currentItem()->text());
					outlinedFontsListWidget->addItem(availableFontsListWidget->currentItem()->text());
				}
			}
			else
			{
				FontsToOutline.append(availableFontsListWidget->currentItem()->text());
				outlinedFontsListWidget->addItem(availableFontsListWidget->currentItem()->text());
			}
		}
	}
	else
	{
		if ((AllFonts[availableFontsListWidget->currentItem()->text()].type() != ScFace::OTF) && (!AllFonts[availableFontsListWidget->currentItem()->text()].subset()))
		{
			FontsToEmbed.append(availableFontsListWidget->currentItem()->text());
			embeddedFontsListWidget->addItem(availableFontsListWidget->currentItem()->text());
			if (AnnotationFonts.contains(availableFontsListWidget->currentItem()->text()))
				embeddedFontsListWidget->item(embeddedFontsListWidget->count()-1)->setFlags(Qt::ItemIsEnabled);
		}
		else
		{
			if (outlinedFontsListWidget->count() != 0)
			{
				if (outlinedFontsListWidget->findItems(availableFontsListWidget->currentItem()->text(), Qt::MatchExactly).count() == 0)
				{
					FontsToOutline.append(availableFontsListWidget->currentItem()->text());
					outlinedFontsListWidget->addItem(availableFontsListWidget->currentItem()->text());
				}
			}
			else
			{
				FontsToOutline.append(availableFontsListWidget->currentItem()->text());
				outlinedFontsListWidget->addItem(availableFontsListWidget->currentItem()->text());
			}
		}
	}
}

void Prefs_PDFExport::RemoveOutline()
{
	FontsToOutline.removeAll(outlinedFontsListWidget->currentItem()->text());
	if ((AllFonts[outlinedFontsListWidget->currentItem()->text()].type() != ScFace::OTF) && (!AllFonts[outlinedFontsListWidget->currentItem()->text()].subset()))
	{
		FontsToEmbed.append(outlinedFontsListWidget->currentItem()->text());
		embeddedFontsListWidget->addItem(outlinedFontsListWidget->currentItem()->text());
	}
	delete outlinedFontsListWidget->takeItem(outlinedFontsListWidget->currentRow());
	outlinedFontsListWidget->clearSelection();
	if (outlinedFontsListWidget->count() == 0)
		fromOutlineButton->setEnabled(false);
}

void Prefs_PDFExport::PutToOutline()
{
	if (outlinedFontsListWidget->count() != 0)
	{
		if (outlinedFontsListWidget->findItems(embeddedFontsListWidget->currentItem()->text(), Qt::MatchExactly).count() == 0)
		{
			FontsToOutline.append(embeddedFontsListWidget->currentItem()->text());
			outlinedFontsListWidget->addItem(embeddedFontsListWidget->currentItem()->text());
		}
	}
	else
	{
		FontsToOutline.append(embeddedFontsListWidget->currentItem()->text());
		outlinedFontsListWidget->addItem(embeddedFontsListWidget->currentItem()->text());
	}
	FontsToEmbed.removeAll(embeddedFontsListWidget->currentItem()->text());
	delete embeddedFontsListWidget->takeItem(embeddedFontsListWidget->currentRow());
	embeddedFontsListWidget->clearSelection();
	if (embeddedFontsListWidget->count() == 0)
	{
		fromEmbedButton->setEnabled(false);
		toOutlineButton->setEnabled(false);
	}
	else
	{
		if (!(embeddedFontsListWidget->currentItem()->flags() & Qt::ItemIsSelectable))
		{
			fromEmbedButton->setEnabled(false);
			toOutlineButton->setEnabled(false);
		}
	}
}
