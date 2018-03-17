/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tabpdfoptions.h"


#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStandardItem>
#include <QToolButton>
#include <QVBoxLayout>


#include "ui/createrange.h"
#include "pdfoptions.h"
#include "prefsmanager.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "scconfig.h"
#include "units.h"
#include "usertaskstructs.h"
#include "ui/scrspinbox.h"
#include "iconmanager.h"
#include "scribuscore.h"
#include "util.h"

TabPDFOptions::TabPDFOptions(QWidget* parent, PDFOptions & Optionen,
								const SCFonts &AllFonts,
								const ProfilesL & PDFXProfiles,
								const QMap<QString, int> & DocFonts,
								ScribusDoc * doc )
	: QTabWidget( parent ),
	// Private members
	unitRatio(unitGetRatioFromIndex(doc->unitIndex())),
	m_Doc(doc),
	AllFonts(AllFonts),
	Opts(Optionen),
	cms(false)
{
	setupUi(this);

	// General tab
	rangeGroupLayout->setAlignment( Qt::AlignTop );
	pageNrButton->setIcon(IconManager::instance()->loadIcon("ellipsis.png"));
	MirrorH->setIcon(IconManager::instance()->loadIcon("16/flip-object-horizontal.png"));
	MirrorH->setCheckable( true );
	MirrorV->setIcon(IconManager::instance()->loadIcon("16/flip-object-vertical.png"));
	MirrorV->setCheckable( true );

	fileOptionsLayout->setAlignment( Qt::AlignTop );
	cms = (ScCore->haveCMS() && m_Doc->HasCMS);
	PDFVersionCombo->setPDFXEnabled(cms && (!PDFXProfiles.isEmpty()));
	Resolution->setMaximum( 4000 );
	Resolution->setMinimum( 35 );
	Resolution->setSuffix( tr( " dpi" ) );
#ifndef HAVE_PODOFO
	EmbedPDF->setEnabled(false);
#endif

	imageQualityLayout->setAlignment( Qt::AlignTop );
	ValC->setSuffix( tr( " dpi" ) );
	ValC->setMaximum( 4000 );
	ValC->setMinimum( 35 );

	// Fonts tab
	groupFontLayout->setAlignment( Qt::AlignTop );
	EmbedList->setMaximumHeight(300);
	EmbedList->setSelectionMode(QListWidget::ExtendedSelection);
	SubsetList->setMaximumHeight(300);
	SubsetList->setSelectionMode(QListWidget::ExtendedSelection);
	ToSubset->setIcon(IconManager::instance()->loadIcon("22/go-next.png"));
	FromSubset->setIcon(IconManager::instance()->loadIcon("22/go-previous.png"));

	// Presentation tab
	effectsLayout->setAlignment( Qt::AlignTop );
	PageTime->setSuffix( tr( " sec" ) );
	PageTime->setMaximum( 3600 );
	PageTime->setMinimum( 0 );
	EffectTime->setSuffix( tr( " sec" ) );
	EffectTime->setMaximum( 3600 );
	EffectTime->setMinimum( 1 );
	
	// Viewer tab
	groupDisplayLayout->setAlignment( Qt::AlignTop );
	pageLayoutLayout->setAlignment( Qt::AlignTop );
	groupNavigationLayout->setAlignment( Qt::AlignTop );
	groupJavaLayout->setAlignment( Qt::AlignTop );

	// Security tab
	groupPassLayout->setAlignment( Qt::AlignTop );
	groupSecSetLayout->setAlignment( Qt::AlignTop );

	// Color tab
	colorGroupLayout->setAlignment( Qt::AlignTop );
	lpiGroupLayout->setAlignment( Qt::AlignTop );

	LPIfreq->setMinimum(10);
	LPIfreq->setMaximum(1000);

	LPIangle->setMinimum(-180);
	LPIangle->setMaximum(180);

	SelLPIcolor = LPIcolor->currentText();
	
	solidColorsGroupLayout->setAlignment( Qt::AlignTop );
	imageProfileGroupLayout->setAlignment( Qt::AlignTop );

	// Prepress tab
	markGroupLayout->setAlignment( Qt::AlignTop );
	markLength->setNewUnit(m_Doc->unitIndex());
	markLength->setMinimum(1 * unitRatio);
	markLength->setMaximum(3000 * unitRatio);

	markOffset->setNewUnit(m_Doc->unitIndex());
	markOffset->setMinimum(0);
	markOffset->setMaximum(3000 * unitRatio);

	bleedGroupLayout->setAlignment( Qt::AlignTop );

	BleedTop->setNewUnit(m_Doc->unitIndex());
	BleedTop->setMinimum(0);
	BleedTop->setMaximum(3000 * unitRatio);

	BleedBottom->setNewUnit(m_Doc->unitIndex());
	BleedBottom->setMinimum(0);
	BleedBottom->setMaximum(3000 * unitRatio);

	BleedLeft->setNewUnit(m_Doc->unitIndex());
	BleedLeft->setMinimum(0);
	BleedLeft->setMaximum(3000 * unitRatio);

	BleedRight->setNewUnit(m_Doc->unitIndex());
	BleedRight->setMinimum(0);
	BleedRight->setMaximum(3000 * unitRatio);

	X3GroupLayout->setAlignment( Qt::AlignTop );

	restoreDefaults(Optionen, AllFonts, PDFXProfiles, DocFonts);

	connect(fontEmbeddingCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(EmbeddingModeChange()));
	connect(EmbedFonts, SIGNAL(clicked()), this, SLOT(EmbedAll()));
	connect(EmbedList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelEFont(QListWidgetItem*)));
	connect(SubsetFonts, SIGNAL(clicked()), this, SLOT(SubsetAll()));
	connect(SubsetList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelSFont(QListWidgetItem*)));
	connect(ToSubset, SIGNAL(clicked()), this, SLOT(PutToSubset()));
	connect(FromSubset, SIGNAL(clicked()), this, SLOT(RemoveSubset()));
	connect(PagePrev, SIGNAL(clicked()), this, SLOT(PagePr()));
	connect(Pages, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetPgEff(QListWidgetItem*, QListWidgetItem*)));
	connect(EffectType, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
	connect(EDirection_2_2, SIGNAL(activated(int)), this, SLOT(ValidDI(int)));
	connect(CheckBox10, SIGNAL(clicked()), this, SLOT(DoEffects()));
	connect(EonAllPg, SIGNAL(clicked()), this, SLOT(EffectOnAll()));
	connect(InfoString, SIGNAL(textChanged(const QString &)), this, SLOT(checkInfo()));
	connect(InfoString, SIGNAL(editingFinished()), this, SLOT(checkInfo()));
	connect(docBleeds, SIGNAL(clicked()), this, SLOT(doDocBleeds()));
	
	connect(AllPages, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect(pageNrButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect(DSColor, SIGNAL(clicked()), this, SLOT(DoDownsample()));
	connect(MirrorH, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(MirrorV, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(RotateDeg, SIGNAL(activated(int)), this, SLOT(Rotation(int)));
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
	connect(EmbedProfs, SIGNAL(clicked()), this, SLOT(EnablePG()));
	connect(EmbedProfs2, SIGNAL(clicked()), this, SLOT(EnablePGI()));
	connect(NoEmbedded, SIGNAL(clicked()), this, SLOT(EnablePGI2()));
	connect(PDFVersionCombo, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
	connect(Encry, SIGNAL(clicked()), this, SLOT(ToggleEncr()));
	connect(UseLPI, SIGNAL(clicked()), this, SLOT(EnableLPI2()));
	connect(LPIcolor, SIGNAL(activated(int)), this, SLOT(SelLPIcol(int)));
	connect(CMethod, SIGNAL(activated(int)), this, SLOT(handleCompressionMethod(int)));

	// Tooltips : General tab
	RotateDeg->setToolTip( "<qt>" + tr( "Automatically rotate the exported pages" ) + "</qt>" );
	AllPages->setToolTip( "<qt>" + tr( "Export all pages to PDF" ) + "</qt>" );
	OnlySome->setToolTip( "<qt>" + tr( "Export a range of pages to PDF" ) );
	PageNr->setToolTip( "<qt>" + tr( "Insert a comma separated list of tokens where "
											"a token can be * for all the pages, 1-5 for "
											"a range of pages or a single page number") + "</qt>" );
	pageNrButton->setToolTip( "<qt>" + tr( "Create a range of pages" ) + "</qt>");
	ClipMarg->setToolTip( "<qt>" + tr( "Do not show objects outside the margins in the exported file" ) + "</qt>" );
	MirrorH->setToolTip( "<qt>" + tr( "Mirror Page(s) horizontally" ) + "</qt>" );
	MirrorV->setToolTip( "<qt>" + tr( "Mirror Page(s) vertically" ) + "</qt>" );
	PDFVersionCombo->setToolTip( "<qt>" + tr( "Determines the PDF compatibility.<br/>The default is <b>PDF 1.3</b> which gives the widest compatibility.<br/>Choose <b>PDF 1.4</b> if your file uses features such as transparency or you require 128 bit encryption.<br/><b>PDF 1.5</b> is necessary when you wish to preserve objects in separate layers within the PDF.<br/><b>PDF/X-1a</b> is for blind exchange with colors strictly specified in CMYK or spot colors.<br/><b>PDF/X-3</b> is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.<br/><b>PDF/X-4</b> is an extension of PDF/X-3 to support transparency and layering." ) + "</qt>");
	ComboBind->setToolTip( "<qt>" + tr( "Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left." ) + "</qt>" );
	CheckBox1->setToolTip( "<qt>" + tr( "Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation." ) + "</qt>" );
	Article->setToolTip( "<qt>" + tr( "Generate PDF Articles, which is useful for navigating linked articles in a PDF" ) + "</qt>" );
	useLayers->setToolTip( "<qt>" + tr( "Layers in your document are exported to the PDF. Only available if PDF 1.5 is chosen." ) + "</qt>" );
	CheckBM->setToolTip( "<qt>" + tr( "Embed the bookmarks you created in your document. These are useful for navigating long PDF documents." ) + "</qt>" );
	Resolution->setToolTip( "<qt>" + tr( "Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos." ) + "</qt>" );
	EmbedPDF->setToolTip( "<qt>" + tr( "Export PDFs in image frames as embedded PDFs. This does *not* yet take care of colorspaces, so you should know what you are doing before setting this to 'true'." ) + "</qt>" );
	Compression->setToolTip( "<qt>" + tr( "Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size." ) + "</qt>" );
	CMethod->setToolTip( "<qt>" + tr( "Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options." ) + "</qt>");
	CQuality->setToolTip( "<qt>" + tr( "Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg." ) + "</qt>");
	DSColor->setToolTip( "<qt>" + tr( "Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export." ) + "</qt>" );
	ValC->setToolTip( "<qt>" + tr( "DPI (Dots Per Inch) for image export") + "</qt>" );

	// Tooltips : Fonts tab
	EmbedFonts->setToolTip( "<qt>" + tr( "Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document." ) + "</qt>");
	CheckBox10->setToolTip( "<qt>" + tr( "Enables presentation effects when using Adobe&#174; Reader&#174; and other PDF viewers which support this in full screen mode." ) + "</qt>");
	SubsetFonts->setToolTip( "<qt>" + tr("Embed only subset fonts with glyphs used in the document into the PDF.") + "</qt>");

	// Tooltips : Presentation tab
	PagePrev->setToolTip( "<qt>" + tr( "Show page previews of each page listed above." ) + "</qt>");
	PageTime->setToolTip( "<qt>" + tr( "Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition." ) + "</qt>" );
	EffectTime->setToolTip( "<qt>" + tr( "Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down." ) + "</qt>" );
	EffectType->setToolTip( "<qt>" + tr( "Type of the display effect." ) + "</qt>" );
	EDirection->setToolTip( "<qt>" + tr( "Direction of the effect of moving lines for the split and blind effects." ) + "</qt>" );
	EDirection_2->setToolTip( "<qt>" + tr( "Starting position for the box and split effects." ) + "</qt>" );
	EDirection_2_2->setToolTip( "<qt>" + tr( "Direction of the glitter or wipe effects." ) + "</qt>" );
	EonAllPg->setToolTip( "<qt>" + tr( "Apply the selected effect to all pages." ) + "</qt>" );

	// Tooltips : Viewer tab
	singlePage->setToolTip( "<qt>" + tr( "Show the document in single page mode" ) + "</qt>" );
	continuousPages->setToolTip( "<qt>" + tr( "Show the document in single page mode with the pages displayed continuously end to end like a scroll" ) + "</qt>" );
	facingPagesLeft->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the left" ) + "</qt>" );
	facingPagesRight->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the right" ) + "</qt>" );
	useViewDefault->setToolTip( "<qt>" + tr( "Use the viewer's defaults or the user's preferences if set differently from the viewer defaults" ) + "</qt>" );
	useFullScreen->setToolTip( "<qt>" + tr( "Enables viewing the document in full screen" ) + "</qt>" );
	useBookmarks->setToolTip( "<qt>" + tr( "Display the bookmarks upon opening" ) + "</qt>" );
	useThumbnails->setToolTip( "<qt>" + tr( "Display the page thumbnails upon opening" ) + "</qt>" );
	useLayers2->setToolTip( "<qt>" + tr( "Forces the displaying of layers. Useful only for PDF 1.5+." ) + "</qt>" );
	hideToolBar->setToolTip( "<qt>" + tr( "Hides the Tool Bar which has selection and other editing capabilities" ) + "</qt>" );
	hideMenuBar->setToolTip( "<qt>" + tr( "Hides the Menu Bar for the viewer, the PDF will display in a plain window" ) + "</qt>" );
	fitWindow->setToolTip( "<qt>" + tr( "Fit the document page or pages to the available space in the viewer window" ) + "</qt>" );

	// Tooltips : Security Tab
	Encry->setToolTip( "<qt>" + tr( "Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations." ) + "</qt>" );
	PassOwner->setToolTip( "<qt>" + tr( "Choose an owner password which enables or disables all the security features in your exported PDF" ) + "</qt>" );
	PassUser->setToolTip( "<qt>" + tr( "Choose a password for users to be able to read your PDF" ) + "</qt>" );
	PrintSec->setToolTip( "<qt>" + tr( "Allow printing of the PDF. If unchecked, printing is prevented." ) + "</qt>" );
	ModifySec->setToolTip( "<qt>" + tr( "Allow modifying of the PDF. If unchecked, modifying the PDF is prevented." ) + "</qt>" );
	CopySec->setToolTip( "<qt>" + tr( "Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied." ) + "</qt>" );
	AddSec->setToolTip( "<qt>" + tr( "Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented." ) + "</qt>" );

	// Tooltips : Color tab
	OutCombo->setToolTip( "<qt>" + tr( "Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF." ) + "</qt>" );
	UseLPI->setToolTip( "<qt>" + tr( "This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems." ) + "</qt>" );
	EmbedProfs->setToolTip( "<qt>" + tr( "Embed a color profile for solid colors" ) + "</qt>" );
	SolidPr->setToolTip( "<qt>" + tr( "Color profile for solid colors" ) + "</qt>" );
	IntendS->setToolTip( "<qt>" + tr( "Rendering intent for solid colors" ) + "</qt>" );
	EmbedProfs2->setToolTip( "<qt>" + tr( "Embed a color profile for images" ) + "</qt>" );
	NoEmbedded->setToolTip( "<qt>" + tr( "Do not use color profiles that are embedded in source images" ) + "</qt>" );
	ImageP->setToolTip( "<qt>" + tr( "Color profile for images" ) + "</qt>" );
	IntendI->setToolTip( "<qt>" + tr( "Rendering intent for images" ) + "</qt>" );
	useSpot->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");

	// Tooltips : PrePress tab 
	cropMarks->setToolTip( "<qt>" + tr( "Creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing" ) + "</qt>" );
	bleedMarks->setToolTip( "<qt>" + tr( "This creates bleed marks which are indicated by  _ . _ and show the bleed limit" ) + "</qt>" );
	registrationMarks->setToolTip( "<qt>" + tr( "Add registration marks to each separation" ) + "</qt>" );
	colorMarks->setToolTip( "<qt>" + tr( "Add color calibration bars" ) + "</qt>" );
	docInfoMarks->setToolTip( "<qt>" + tr( "Add document information which includes the document title and page numbers" ) + "</qt>" );
	markLength->setToolTip( "<qt>" + tr( "Indicate the size of crops'marks" ) + "</qt>" );
	markOffset->setToolTip( "<qt>" + tr( "Indicate the distance offset for the registration marks" ) + "</qt>" );
	BleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
	BleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
	BleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
	BleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	docBleeds->setToolTip( "<qt>" + tr( "Use the existing bleed settings from the document preferences" ) + "</qt>" );
	PrintProfC->setToolTip( "<qt>" + tr( "Output profile for printing. If possible, get some guidance from your printer on profile selection." ) + "</qt>" );
	InfoString->setToolTip( "<qt>" + tr( "Mandatory string for PDF/X or the PDF will fail PDF/X conformance. We recommend you use the title of the document." ) + "</qt>" );
}

PDFOptions::PDFFontEmbedding TabPDFOptions::fontEmbeddingMode()
{
	return fontEmbeddingCombo->embeddingMode();
}

QStringList TabPDFOptions::fontsToEmbed()
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();
	if (embeddingMode != PDFOptions::EmbedFonts)
		return QStringList();

	QStringList fonts;
	for (int i = 0; i < EmbedList->count(); ++i)
		fonts.append(EmbedList->item(i)->text());
	return fonts;
}

QStringList TabPDFOptions::fontsToSubset()
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();
	if (embeddingMode != PDFOptions::EmbedFonts)
		return QStringList();

	QStringList fonts;
	for (int i = 0; i < SubsetList->count(); ++i)
		fonts.append(SubsetList->item(i)->text());
	return fonts;
}

QStringList TabPDFOptions::fontsToOutline()
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();
	if (embeddingMode != PDFOptions::OutlineFonts)
		return QStringList();

	return m_docFonts;
}

void TabPDFOptions::restoreDefaults(PDFOptions & Optionen,
									const SCFonts &AllFonts,
									const ProfilesL & PDFXProfiles,
									const QMap<QString, int> & DocFonts)
{
	AllPages->setChecked( true );
	PageNr->setEnabled(false);
	pageNrButton->setEnabled(false);

	AllPages->setChecked(Opts.pageRangeSelection == 0);
	OnlySome->setChecked(Opts.pageRangeSelection != 0);
	if (OnlySome->isChecked())
	{
		PageNr->setEnabled(true);
		PageNr->setText(Opts.pageRangeString);
		pageNrButton->setEnabled(true);
	}

	RotateDeg->setCurrentIndex(Opts.RotateDeg / 90);
	MirrorH->setChecked(Opts.MirrorH);
	MirrorV->setChecked(Opts.MirrorV);
	ClipMarg->setChecked(Opts.doClip);
	bool cmsUse = (ScCore->haveCMS() && m_Doc->HasCMS);
	if (!cmsUse)
		PDFVersionCombo->setVersion(PDFOptions::PDFVersion_14);
	PDFVersionCombo->setVersion(Opts.Version);
	ComboBind->setCurrentIndex(Opts.Binding);
	CheckBox1->setChecked(Opts.Thumbnails);
	Article->setChecked(Opts.Articles);
	CheckBM->setChecked(Opts.Bookmarks);
	useLayers->setChecked(Opts.useLayers);
	if (Opts.Version == PDFOptions::PDFVersion_15 || Opts.Version == PDFOptions::PDFVersion_X4)
		useLayers->setEnabled(true);
	else
		useLayers->setEnabled(false);
	Resolution->setValue(Opts.Resolution);
	EmbedPDF->setChecked(Opts.embedPDF);
	Compression->setChecked( Opts.Compress );
	CMethod->setCurrentIndex(Opts.CompressMethod);
	CQuality->setCurrentIndex(Opts.Quality);
	if (Opts.CompressMethod == 3)
		CQuality->setEnabled(false);
	DSColor->setChecked(Opts.RecalcPic);
	ValC->setValue(Opts.PicRes);
	ValC->setEnabled(DSColor->isChecked() ? true : false);

	m_docFonts = DocFonts.keys();
	if (Opts.Version == PDFOptions::PDFVersion_X1a ||
		Opts.Version == PDFOptions::PDFVersion_X3  ||
		Opts.Version == PDFOptions::PDFVersion_X4 )
	{
		if (Opts.FontEmbedding != PDFOptions::EmbedFonts &&
			Opts.FontEmbedding != PDFOptions::OutlineFonts)
		{
			Opts.FontEmbedding = PDFOptions::EmbedFonts;
		}
	}
	fontEmbeddingCombo->setCurrentIndex((int) Opts.FontEmbedding);

	EmbedList->clearSelection();
	EmbedList->setEnabled(Opts.FontEmbedding == PDFOptions::EmbedFonts);
	EmbedFonts->setEnabled(Opts.FontEmbedding == PDFOptions::EmbedFonts);
	SubsetList->clearSelection();
	SubsetList->setEnabled(Opts.FontEmbedding == PDFOptions::EmbedFonts);
	SubsetFonts->setEnabled(Opts.FontEmbedding == PDFOptions::EmbedFonts);
	ToSubset->setEnabled(false); // Will be enabled when user select a font in appropriate list
	FromSubset->setEnabled(false); // Will be enabled when user select a font in appropriate list

//	Build a list of all Fonts used in Annotations;
	PageItem *pgit;
	QList<PageItem*> allItems;
	for (QHash<int, PageItem*>::iterator it = m_Doc->FrameItems.begin(); it != m_Doc->FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			pgit = allItems.at(ii);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()))
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
				if (pgit->itemText.length() > 0 || mustEmbed)
					m_annotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < m_Doc->MasterItems.count(); ++a)
	{
		PageItem *currItem = m_Doc->MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			pgit = allItems.at(ii);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()))
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
				if (pgit->itemText.length() > 0 || mustEmbed)
					m_annotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < m_Doc->DocItems.count(); ++a)
	{
		PageItem *currItem = m_Doc->DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			pgit = allItems.at(ii);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()))
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
				if (pgit->itemText.length() > 0 || mustEmbed)
					m_annotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
			}
		}
		allItems.clear();
	}
	ToSubset->setEnabled(false);
	FromSubset->setEnabled(false);

	if ((Opts.EmbedList.count() == 0) && (Opts.SubsetList.count() == 0) && (Opts.firstUse))
		EmbedAll();
	else
	{
		EmbedList->clear();
		SubsetList->clear();
		for (int fe = 0; fe < m_docFonts.count(); ++ fe)
		{
			QString fontName = m_docFonts.at(fe);
			const ScFace fontFace = AllFonts[fontName];
			if (Opts.EmbedList.contains(fontName) && (!fontFace.isOTF()))
				addFontItem(fontName, EmbedList);
			else
			{
				addFontItem(fontName, SubsetList);
				if (!Opts.SubsetList.contains(fontName))
					Opts.SubsetList.append(fontName);
			}
		}
		QMap<QString, QString>::Iterator itAnn;
		for (itAnn = m_annotationFonts.begin(); itAnn != m_annotationFonts.end(); ++itAnn)
		{
			QList<QListWidgetItem *> itEmbed = EmbedList->findItems(itAnn.key(), Qt::MatchExactly);
			if (itEmbed.count() == 0)
			{
				QListWidgetItem* item = addFontItem(itAnn.key(), EmbedList);
				item->setFlags(Qt::ItemIsEnabled);
			}
			QList<QListWidgetItem *> itSubset = SubsetList->findItems(itAnn.key(), Qt::MatchExactly);
			for (int itOut = 0; itOut < itSubset.count(); ++itOut)
			{
				QListWidgetItem* item = itSubset[itOut];
				delete SubsetList->takeItem(SubsetList->row(item));
			}
		}
	}
	CheckBox10->setChecked(Opts.PresentMode);
	PagePrev->setChecked(false);
	Pages->clear();
	QString tmp;
	for (int pg = 0; pg < m_Doc->Pages->count(); ++pg)
	{
		Pages->addItem( tr("Page")+" "+tmp.setNum(pg+1));
		EffVal.append(m_Doc->Pages->at(pg)->PresentVals);
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
	for (itja = m_Doc->JavaScripts.begin(); itja != m_Doc->JavaScripts.end(); ++itja)
		actionCombo->addItem(itja.key());
	if (m_Doc->JavaScripts.contains(Opts.openAction))
		setCurrentComboItem(actionCombo, Opts.openAction);
	if (Opts.PageLayout == PDFOptions::SinglePage)
		singlePage->setChecked(true);
	else if (Opts.PageLayout == PDFOptions::OneColumn)
		continuousPages->setChecked(true);
	else if (Opts.PageLayout == PDFOptions::TwoColumnLeft)
		facingPagesLeft->setChecked(true);
	else if (Opts.PageLayout == PDFOptions::TwoColumnRight)
		facingPagesRight->setChecked(true);
	if ((Opts.Version == PDFOptions::PDFVersion_15) || (Opts.Version == PDFOptions::PDFVersion_X4))
		useLayers2->setEnabled(true);
	else
		useLayers2->setEnabled(false);

	Encry->setChecked( Opts.Encrypt );
	PassOwner->setText(Opts.PassOwner);
	PassUser->setText(Opts.PassUser);
	PrintSec->setChecked( Opts.Permissions & 4 );
	ModifySec->setChecked( Opts.Permissions & 8 );
	CopySec->setChecked( Opts.Permissions & 16 );
	AddSec->setChecked( Opts.Permissions & 32 );
	if (!Encry->isChecked())
	{
		groupSecSet->setEnabled(false);
		groupPass->setEnabled(false);
	}

	if (Opts.UseRGB)
		OutCombo->setCurrentIndex(0);
	else
	{
		if (Opts.isGrayscale)
			OutCombo->setCurrentIndex(2);
		else
			OutCombo->setCurrentIndex(1);
	}
	useSpot->setChecked(!Opts.UseSpotColors);
	UseLPI->setChecked(Opts.UseLPI);
	QMap<QString,LPIData>::Iterator itlp;
	LPIcolor->clear();
	for (itlp = Opts.LPISettings.begin(); itlp != Opts.LPISettings.end(); ++itlp)
		LPIcolor->addItem( itlp.key() );
	LPIcolor->setCurrentIndex(0);

	LPIfreq->setValue(Opts.LPISettings[LPIcolor->currentText()].Frequency);
	LPIangle->setValue(Opts.LPISettings[LPIcolor->currentText()].Angle);
	LPIfunc->setCurrentIndex(Opts.LPISettings[LPIcolor->currentText()].SpotFunc);
	EmbedProfs->setChecked(Opts.UseProfiles);
	EmbedProfs2->setChecked(Opts.UseProfiles2);
	NoEmbedded->setChecked(Opts.EmbeddedI);
	if ((Opts.UseRGB) || (Opts.isGrayscale))
	{
		solidsProfileGroup->setEnabled(false);
		imageProfileGroup->setEnabled(false);
		EnablePr(0);
	}
	else
		EnablePr(1);
	EnablePG();
	EnablePGI();
	QString tp = Opts.SolidProf;
	if (!ScCore->InputProfiles.contains(tp))
		tp = m_Doc->cmsSettings().DefaultSolidColorRGBProfile;
	ProfilesL::Iterator itp;
	ProfilesL::Iterator itpend=ScCore->InputProfiles.end();
	SolidPr->clear();
	for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
	{
		SolidPr->addItem(itp.key());
		if (itp.key() == tp)
		{
			if (cmsUse)
				SolidPr->setCurrentIndex(SolidPr->count()-1);
		}
	}
	if (cmsUse)
		IntendS->setCurrentIndex(Opts.Intent);
	QString tp1 = Opts.ImageProf;
	if (!ScCore->InputProfiles.contains(tp1))
		tp1 = m_Doc->cmsSettings().DefaultSolidColorRGBProfile;
	ProfilesL::Iterator itp2;
	ProfilesL::Iterator itp2end=ScCore->InputProfiles.end();
	ImageP->clear();
	for (itp2 = ScCore->InputProfiles.begin(); itp2 != itp2end; ++itp2)
	{
		ImageP->addItem(itp2.key());
		if (itp2.key() == tp1)
		{
			if (cmsUse)
				ImageP->setCurrentIndex(ImageP->count()-1);
		}
	}
	if (cmsUse)
		IntendI->setCurrentIndex(Opts.Intent2);
	if (!cmsUse)
	{
		solidsProfileGroup->hide();
		imageProfileGroup->hide();
	}

	ProfilesL::const_iterator itp3;
	QString tp3 = Opts.PrintProf;
	if (!PDFXProfiles.contains(tp3))
		tp3 = m_Doc->cmsSettings().DefaultPrinterProfile;
	PrintProfC->clear();
	for (itp3 = PDFXProfiles.constBegin(); itp3 != PDFXProfiles.constEnd(); ++itp3)
	{
		PrintProfC->addItem(itp3.key());
		if (itp3.key() == tp3)
			PrintProfC->setCurrentIndex(PrintProfC->count()-1);
	}
	if (!Opts.Info.isEmpty())
		InfoString->setText(Opts.Info);
	else
	{
		QFileInfo fi(m_Doc->DocName);
		InfoString->setText(fi.fileName());
	}

	BleedTop->setValue(Opts.bleeds.top()*unitRatio);
	BleedBottom->setValue(Opts.bleeds.bottom()*unitRatio);
	BleedRight->setValue(Opts.bleeds.right()*unitRatio);
	BleedLeft->setValue(Opts.bleeds.left()*unitRatio);
	docBleeds->setChecked(Opts.useDocBleeds);
	doDocBleeds();

	markLength->setValue(Opts.markLength*unitRatio);
	markOffset->setValue(Opts.markOffset*unitRatio);
	cropMarks->setChecked(Opts.cropMarks);
	bleedMarks->setChecked(Opts.bleedMarks);
	registrationMarks->setChecked(Opts.registrationMarks);
	colorMarks->setChecked(Opts.colorMarks);
	docInfoMarks->setChecked(Opts.docInfoMarks);
	if (!cmsUse)
		X3Group->setEnabled(false);
	if (cmsUse && (Opts.Version == PDFOptions::PDFVersion_X1a) && (!PDFXProfiles.isEmpty()))
		EnablePDFX(3);
	else if (cmsUse && (Opts.Version == PDFOptions::PDFVersion_X3) && (!PDFXProfiles.isEmpty()))
		EnablePDFX(4);
	else if (cmsUse && (Opts.Version == PDFOptions::PDFVersion_X4) && (!PDFXProfiles.isEmpty()))
		EnablePDFX(5);
	else
		X3Group->setEnabled(false);

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
	if (m_Doc->pagePositioning() != 0)
	{
		bleedLeftLabel->setText( tr( "Inside:" ) );
		bleedRightLabel->setText( tr( "Outside:" ) );
	}
}

void TabPDFOptions::handleCompressionMethod(int ind)
{
	CQuality->setDisabled(ind == 3);
}

void TabPDFOptions::storeValues(PDFOptions& pdfOptions)
{
	pdfOptions.Thumbnails = CheckBox1->isChecked();
	pdfOptions.Compress = Compression->isChecked();
	pdfOptions.CompressMethod = (PDFOptions::PDFCompression) CMethod->currentIndex();
	pdfOptions.Quality = CQuality->currentIndex();
	pdfOptions.Resolution = Resolution->value();
	pdfOptions.RecalcPic = DSColor->isChecked();
	pdfOptions.PicRes = ValC->value();
	pdfOptions.Bookmarks = CheckBM->isChecked();
	pdfOptions.Binding = ComboBind->currentIndex();
	pdfOptions.MirrorH = MirrorH->isChecked();
	pdfOptions.MirrorV = MirrorV->isChecked();
	pdfOptions.RotateDeg = RotateDeg->currentIndex() * 90;
	pdfOptions.pageRangeSelection = AllPages->isChecked() ? 0 : 1;
	pdfOptions.pageRangeString = PageNr->text();
	pdfOptions.Articles = Article->isChecked();
	pdfOptions.FontEmbedding =  fontEmbeddingCombo->embeddingMode();
	pdfOptions.Encrypt = Encry->isChecked();
	pdfOptions.UseLPI = UseLPI->isChecked();
	pdfOptions.UseSpotColors = !useSpot->isChecked();
	pdfOptions.doMultiFile = false;
	pdfOptions.cropMarks  = cropMarks->isChecked();
	pdfOptions.bleedMarks = bleedMarks->isChecked();
	pdfOptions.registrationMarks = registrationMarks->isChecked();
	pdfOptions.colorMarks = colorMarks->isChecked();
	pdfOptions.docInfoMarks = docInfoMarks->isChecked();
	pdfOptions.markLength = markLength->value() / unitRatio;
	pdfOptions.markOffset = markOffset->value() / unitRatio;
	pdfOptions.useDocBleeds = docBleeds->isChecked();
	pdfOptions.bleeds.setBottom(BleedBottom->value() / unitRatio);
	pdfOptions.bleeds.setTop(BleedTop->value() / unitRatio);
	pdfOptions.bleeds.setLeft(BleedLeft->value() / unitRatio);
	pdfOptions.bleeds.setRight(BleedRight->value() / unitRatio);
	pdfOptions.doClip = ClipMarg->isChecked();
	if (Encry->isChecked())
	{
		int Perm = -64;
		if (PDFVersionCombo->currentIndex() == 1)
			Perm &= ~0x00240000;
		if (PrintSec->isChecked())
			Perm += 4;
		if (ModifySec->isChecked())
			Perm += 8;
		if (CopySec->isChecked())
			Perm += 16;
		if (AddSec->isChecked())
			Perm += 32;
		pdfOptions.Permissions = Perm;
		pdfOptions.PassOwner = PassOwner->text();
		pdfOptions.PassUser = PassUser->text();
	}
	pdfOptions.Version = PDFVersionCombo->version();
	if (OutCombo->currentIndex() == 0)
	{
		pdfOptions.isGrayscale = false;
		pdfOptions.UseRGB = true;
		pdfOptions.UseProfiles = false;
		pdfOptions.UseProfiles2 = false;
	}
	else
	{
		if (OutCombo->currentIndex() == 2)
		{
			pdfOptions.isGrayscale = true;
			pdfOptions.UseRGB = false;
			pdfOptions.UseProfiles = false;
			pdfOptions.UseProfiles2 = false;
		}
		else
		{
			pdfOptions.isGrayscale = false;
			pdfOptions.UseRGB = false;
			if (/*CMSuse*/ ScCore->haveCMS())
			{
				pdfOptions.UseProfiles = EmbedProfs->isChecked();
				pdfOptions.UseProfiles2 = EmbedProfs2->isChecked();
				pdfOptions.Intent = IntendS->currentIndex();
				pdfOptions.Intent2 = IntendI->currentIndex();
				pdfOptions.EmbeddedI = NoEmbedded->isChecked();
				pdfOptions.SolidProf = SolidPr->currentText();
				pdfOptions.ImageProf = ImageP->currentText();
				pdfOptions.PrintProf = PrintProfC->currentText();
			}
		}
	}
}

void TabPDFOptions::doDocBleeds()
{
	if (docBleeds->isChecked())
	{
		Opts.bleeds.setTop(BleedTop->value() / unitRatio);
		Opts.bleeds.setBottom(BleedBottom->value() / unitRatio);
		Opts.bleeds.setRight(BleedRight->value() / unitRatio);
		Opts.bleeds.setLeft(BleedLeft->value() / unitRatio);
		BleedTop->setValue(m_Doc->bleeds()->top()*unitRatio);
		BleedBottom->setValue(m_Doc->bleeds()->bottom()*unitRatio);
		BleedRight->setValue(m_Doc->bleeds()->right()*unitRatio);
		BleedLeft->setValue(m_Doc->bleeds()->left()*unitRatio);
		BleedTop->setEnabled(false);
		BleedBottom->setEnabled(false);
		BleedRight->setEnabled(false);
		BleedLeft->setEnabled(false);
	}
	else
	{
		BleedTop->setValue(Opts.bleeds.top()*unitRatio);
		BleedBottom->setValue(Opts.bleeds.bottom()*unitRatio);
		BleedRight->setValue(Opts.bleeds.right()*unitRatio);
		BleedLeft->setValue(Opts.bleeds.left()*unitRatio);
		BleedTop->setEnabled(true);
		BleedBottom->setEnabled(true);
		BleedRight->setEnabled(true);
		BleedLeft->setEnabled(true);
	}
}

void TabPDFOptions::checkInfo()
{
	if ((PDFVersionCombo->versionIsPDFX()) && (InfoString->text().isEmpty()))
		emit noInfo();
	else
		emit hasInfo();
}

void TabPDFOptions::ToggleEncr()
{
	bool setter = Encry->isChecked() ? true : false;
	groupSecSet->setEnabled(setter);
	groupPass->setEnabled(setter);
}

void TabPDFOptions::enableCMS(bool enable)
{
	QSignalBlocker blocker(PDFVersionCombo);
	PDFOptions::PDFVersion currVersion = PDFVersionCombo->version();
	PDFVersionCombo->setPDFXEnabled(enable);
	cms = enable;
	if (!enable)
		currVersion = qMax(PDFOptions::PDFVersion_13, qMin(currVersion, PDFOptions::PDFVersion_15));
	PDFVersionCombo->setVersion(currVersion);
	EnablePr(1);
}

void TabPDFOptions::EnablePDFX(int a)
{
	useLayers->setEnabled((a == 2) || (a == 5));
	if (useLayers2)
		useLayers2->setEnabled((a == 2) || (a == 5));

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
	if (a == 2)
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
			for (int pg = 0; pg < m_Doc->Pages->count(); ++pg)
			{
				if (EffVal[pg].effectType > 6)
					EffVal[pg].effectType = 0;
			}
		}
		else
			EffectType->setCurrentIndex(currentEff);
	}
	connect(EffectType, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));

	if (a < 3)  // not PDF/X
	{
		fontEmbeddingCombo->setNoEmbeddingEnabled(true);
		X3Group->setEnabled(false);
		setTabEnabled(indexOf(tabSecurity), true);
		OutCombo->setEnabled(true);
		EmbedProfs->setEnabled(true);
		EmbedProfs2->setEnabled(true);
		emit hasInfo();

		CheckBox10->setEnabled(true);
		EmbedFonts->setEnabled(true);
		EnablePr(OutCombo->currentIndex());
		return;
	}
	// PDF/X is selected
	disconnect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
	OutCombo->setCurrentIndex(1);
	OutCombo->setEnabled(false);
	EnablePr(1);
	if (a == 3) // X1, no profile embedding
	{
		EmbedProfs->setChecked(false);
		EmbedProfs->setEnabled(false);
		EmbedProfs2->setChecked(false);
		EmbedProfs2->setEnabled(false);
	}
	if ((a == 4) || (a == 5)) // X3 or X4, enforcing color profiles on images
	{
		EmbedProfs->setEnabled(true);
		EmbedProfs2->setChecked(true);
		EmbedProfs2->setEnabled(false);
	}

	PDFOptions::PDFFontEmbedding oldEmbeddingMode = fontEmbeddingCombo->embeddingMode();
	fontEmbeddingCombo->setNoEmbeddingEnabled(false);
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();

	if (oldEmbeddingMode != embeddingMode)
		EmbedList->clearSelection();
	EmbedList->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	EmbedFonts->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	if (oldEmbeddingMode != embeddingMode)
		SubsetList->clearSelection();
	SubsetList->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	SubsetFonts->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	ToSubset->setEnabled(false); // Will be enabled when user select a font in appropriate list
	FromSubset->setEnabled(false); // Will be enabled when user select a font in appropriate list

	EmbedAll();
	CheckBox10->setChecked(false);
	CheckBox10->setEnabled(false);
	if (InfoString->text().isEmpty())
		emit noInfo();
	else
		emit hasInfo();

	EnablePGI();
	X3Group->setEnabled(true);
	setTabEnabled(indexOf(tabSecurity), false);
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
}

void TabPDFOptions::EnablePGI()
{
	if (EmbedProfs2->isChecked())
	{
		NoEmbedded->setEnabled(true);
		bool setter = NoEmbedded->isChecked() ? true : false;
		imageProfileLabel->setEnabled(setter);
		imageIntentLabel->setEnabled(setter);
		ImageP->setEnabled(setter);
		IntendI->setEnabled(setter);
	}
	else
	{
		imageProfileLabel->setEnabled(false);
		imageIntentLabel->setEnabled(false);
		ImageP->setEnabled(false);
		IntendI->setEnabled(false);
		NoEmbedded->setEnabled(false);
	}
}

void TabPDFOptions::EnablePGI2()
{
	bool setter = NoEmbedded->isChecked() ? true : false;
	imageProfileLabel->setEnabled(setter);
	imageIntentLabel->setEnabled(setter);
	ImageP->setEnabled(setter);
	IntendI->setEnabled(setter);
}

void TabPDFOptions::EnablePG()
{
	bool setter = EmbedProfs->isChecked() ? true : false;
	solidsProfileLabel->setEnabled(setter);
	solidsIntentLabel->setEnabled(setter);
	SolidPr->setEnabled(setter);
	IntendS->setEnabled(setter);
}

void TabPDFOptions::EnablePr(int a)
{
	EnableLPI(a);
	bool setter = false;
	if (a == 1)
		setter = !PDFVersionCombo->versionIs(PDFOptions::PDFVersion_X1a);

	solidsProfileGroup->setEnabled(setter);
	imageProfileGroup->setEnabled(setter);
}

void TabPDFOptions::EnableLPI(int a)
{
	if (a == 1)
	{
		QString tp = Opts.SolidProf;
		if (!ScCore->InputProfiles.contains(tp))
			tp = m_Doc->cmsSettings().DefaultSolidColorRGBProfile;
		SolidPr->clear();
		ProfilesL::Iterator itp;
		ProfilesL::Iterator itpend=ScCore->InputProfiles.end();
		for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
		{
			SolidPr->addItem(itp.key());
			if (itp.key() == tp)
			{
				if (cms)
					SolidPr->setCurrentIndex(SolidPr->count()-1);
			}
		}
		if (cms)
			IntendS->setCurrentIndex(Opts.Intent);
		QString tp1 = Opts.ImageProf;
		if (!ScCore->InputProfiles.contains(tp1))
			tp1 = m_Doc->cmsSettings().DefaultSolidColorRGBProfile;
		ImageP->clear();
		ProfilesL::Iterator itp2;
		ProfilesL::Iterator itp2end=ScCore->InputProfiles.end();
		for (itp2 = ScCore->InputProfiles.begin(); itp2 != itp2end; ++itp2)
		{
			ImageP->addItem(itp2.key());
			if (itp2.key() == tp1)
			{
				if (cms)
					ImageP->setCurrentIndex(ImageP->count()-1);
			}
		}
		if (cms)
			IntendI->setCurrentIndex(Opts.Intent2);
		if (cms)
		{
			solidsProfileGroup->show();
			imageProfileGroup->show();
		}
		else
		{
			solidsProfileGroup->hide();
			imageProfileGroup->hide();
		}
		useSpot->show();
		UseLPI->show();
		if (UseLPI->isChecked())
			LPIgroup->show();
		else
			LPIgroup->hide();	
	}
	else
	{
		useSpot->hide();
		UseLPI->hide();
		LPIgroup->hide();
	}
}

void TabPDFOptions::EnableLPI2()
{
	if (UseLPI->isChecked())
		LPIgroup->show();
	else
		LPIgroup->hide();
}

void TabPDFOptions::SelLPIcol(int c)
{
	// XXX Optionen or Opts changed here
	Opts.LPISettings[SelLPIcolor].Frequency = LPIfreq->value();
	Opts.LPISettings[SelLPIcolor].Angle = LPIangle->value();
	Opts.LPISettings[SelLPIcolor].SpotFunc = LPIfunc->currentIndex();
	LPIfreq->setValue(Opts.LPISettings[LPIcolor->itemText(c)].Frequency);
	LPIangle->setValue(Opts.LPISettings[LPIcolor->itemText(c)].Angle);
	LPIfunc->setCurrentIndex(Opts.LPISettings[LPIcolor->itemText(c)].SpotFunc);
	SelLPIcolor = LPIcolor->itemText(c);
}

void TabPDFOptions::SelRange(bool e)
{
	bool setter = e ? false : true;
	PageNr->setEnabled( setter );
	pageNrButton->setEnabled( setter );
	if (setter == false)
		CheckBM->setChecked(false);
}

void TabPDFOptions::EffectOnAll()
{
	for (int pg = 0; pg < m_Doc->Pages->count(); ++pg)
	{
		EffVal[pg].pageViewDuration = PageTime->value();
		EffVal[pg].pageEffectDuration = EffectTime->value();
		EffVal[pg].effectType = EffectType->currentIndex();
		EffVal[pg].Dm = EDirection->currentIndex();
		EffVal[pg].M = EDirection_2->currentIndex();
		EffVal[pg].Di = EDirection_2_2->currentIndex();
	}
}

void TabPDFOptions::PDFMirror()
{
	// XXX Optionen or Opts changed here
	Opts.MirrorH = MirrorH->isChecked();
	Opts.MirrorV = MirrorV->isChecked();
}

void TabPDFOptions::Rotation( int value )
{
	Opts.RotateDeg = value * 90;
}

void TabPDFOptions::DoEffects()
{
	bool setter = CheckBox10->isChecked() ? true : false;
	Pages->setEnabled(setter);
	Effects->setEnabled(setter);
	PagePrev->setEnabled(setter);
}

void TabPDFOptions::ValidDI(int nr)
{
	// Qt4 if (!EDirection_2_2->listBox()->item(nr)->isSelectable())
	QStandardItem* si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(nr));
	if ( !(si && si->isSelectable()) )
		EDirection_2_2->setCurrentIndex(0);
}

void TabPDFOptions::SetPgEff(QListWidgetItem* /*current*/, QListWidgetItem* previous)
{
	int currentRow = Pages->currentRow();
	if (currentRow < 0)
		return;

	int previousRow = 0;
	if (previous)
		previousRow = Pages->row(previous);

	EffVal[previousRow].pageViewDuration = PageTime->value();
	EffVal[previousRow].pageEffectDuration = EffectTime->value();
	EffVal[previousRow].effectType = EffectType->currentIndex();
	EffVal[previousRow].Dm = EDirection->currentIndex();
	EffVal[previousRow].M = EDirection_2->currentIndex();
	EffVal[previousRow].Di = EDirection_2_2->currentIndex();
	SetEffOpts(EffVal[currentRow].effectType);
	PageTime->setValue(EffVal[currentRow].pageViewDuration);
	EffectTime->setValue(EffVal[currentRow].pageEffectDuration);
	EffectType->setCurrentIndex(EffVal[currentRow].effectType);
	EDirection->setCurrentIndex(EffVal[currentRow].Dm);
	EDirection_2->setCurrentIndex(EffVal[currentRow].M);
	EDirection_2_2->setCurrentIndex(EffVal[currentRow].Di);
}

void TabPDFOptions::SetEffOpts(int nr)
{
	// Qt4
// 	EDirection_2_2->listBox()->item(2)->setSelectable(false);
// 	EDirection_2_2->listBox()->item(3)->setSelectable(false);
// 	EDirection_2_2->listBox()->item(4)->setSelectable(false);
	QStandardItem* si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(2));
	if (si) si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(3));
	if (si) si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(4));
	if (si) si->setSelectable(false);
	switch (nr)
	{
	case 0:
	case 3:
	case 10:
		EDirection->setEnabled(false);
		EDirection_2->setEnabled(false);
		EDirection_2_2->setEnabled(false);
		break;
	case 1:
		EDirection->setEnabled(true);
		EDirection_2->setEnabled(false);
		EDirection_2_2->setEnabled(false);
		break;
	case 2:
		EDirection->setEnabled(false);
		EDirection_2->setEnabled(true);
		EDirection_2_2->setEnabled(false);
		break;
	case 4:
	case 6:
	case 7:
	case 8:
	case 9:
		EDirection->setEnabled(false);
		EDirection_2->setEnabled(false);
		EDirection_2_2->setEnabled(true);
		if (nr == 6)
		{
			// Qt4
/*			EDirection_2_2->listBox()->item(2)->setSelectable(true);
			EDirection_2_2->listBox()->item(3)->setSelectable(true);*/
			si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(2));
			if (si) si->setSelectable(true);
			si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(3));
			if (si) si->setSelectable(true);
		}
		else {
			// Qt4
			// 			EDirection_2_2->listBox()->item(4)->setSelectable(true);
			si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(4));
			if (si) si->setSelectable(true);
		}
		break;
	case 5:
		EDirection->setEnabled(true);
		EDirection_2->setEnabled(true);
		EDirection_2_2->setEnabled(false);
		break;
	}
}

void TabPDFOptions::PagePr()
{
	connect(Pages, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetPgEff(QListWidgetItem*, QListWidgetItem*)));
	QString tmp;
	QPixmap pm;
	int ci = Pages->currentRow();
	int pgMaxX = 0;
	int pgMaxY = 0;
	Pages->clear();
	if (PagePrev->isChecked())
	{
		for (int pg = 0; pg < m_Doc->Pages->count(); ++pg)
		{
			pm=QPixmap::fromImage(m_Doc->view()->PageToPixmap(pg, 70));
			pgMaxX = qMax(pgMaxX, pm.width());
			pgMaxY = qMax(pgMaxY, pm.height());
			new QListWidgetItem( pm, tr("Page")+" "+tmp.setNum(pg+1), Pages);
		}
		Pages->setIconSize(QSize(pgMaxX, pgMaxY));
	}
	else
	{
		for (int pg = 0; pg < m_Doc->Pages->count(); ++pg)
		{
			new QListWidgetItem( tr("Page")+" "+tmp.setNum(pg+1), Pages);
		}
	}
	if (ci != -1)
		Pages->setCurrentRow(ci);
	else
		Pages->clearSelection();
	connect(Pages, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetPgEff(QListWidgetItem*, QListWidgetItem*)));
}

void TabPDFOptions::DoDownsample()
{
	if (DSColor->isChecked())
	{
		ValC->setEnabled(true);
		if (ValC->value() > Resolution->value())
			ValC->setValue(Resolution->value());
//		ValC->setMaximum(Resolution->value());
//		ValC->setMinimum(35);
	}
	else
		ValC->setEnabled(false);
}

void TabPDFOptions::EmbeddingModeChange()
{
	PDFOptions::PDFFontEmbedding embeddingMode = fontEmbeddingCombo->embeddingMode();

	EmbedList->clearSelection();
	EmbedList->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	EmbedFonts->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	SubsetList->clearSelection();
	SubsetList->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	SubsetFonts->setEnabled(embeddingMode == PDFOptions::EmbedFonts);
	ToSubset->setEnabled(false); // Will be enabled when user select a font in appropriate list
	FromSubset->setEnabled(false); // Will be enabled when user select a font in appropriate list
}

void TabPDFOptions::RemoveSubset()
{
	QList<QListWidgetItem*> selection = SubsetList->selectedItems();
	for (int i = 0; i < selection.count() ; ++i)
	{
		QListWidgetItem* fontItem = selection[i];
		QString currentFont = fontItem->text();
		const ScFace fontFace = AllFonts[currentFont];
		if (fontFace.isOTF() || fontFace.subset())
			continue;
		addFontItem(currentFont, EmbedList);
		int currentRow = SubsetList->row(fontItem);
		delete SubsetList->takeItem(currentRow);
	}

	SubsetList->clearSelection();
	FromSubset->setEnabled(false);
}

void TabPDFOptions::PutToSubset()
{
	QList<QListWidgetItem*> selection = EmbedList->selectedItems();
	for (int i = 0; i < selection.count() ; ++i)
	{
		QListWidgetItem* fontItem = selection[i];
		QString currentFont = fontItem->text();
		if (SubsetList->count() != 0)
		{
			if (SubsetList->findItems(currentFont, Qt::MatchExactly).count() == 0)
				addFontItem(currentFont, SubsetList);
		}
		else
		{
			addFontItem(currentFont, SubsetList);
		}
		int itemRow = EmbedList->row(fontItem);
		delete EmbedList->takeItem(itemRow);
	}

	EmbedList->clearSelection();
	ToSubset->setEnabled(false);
}

void TabPDFOptions::SelEFont(QListWidgetItem*)
{
	QList<QListWidgetItem*> selection = EmbedList->selectedItems();
	bool enableToSubset = (selection.count() > 0);

	ToSubset->setEnabled(enableToSubset);
	FromSubset->setEnabled(false);
	SubsetList->clearSelection();
}

void TabPDFOptions::SelSFont(QListWidgetItem*)
{
	QList<QListWidgetItem*> selection = SubsetList->selectedItems();
	int enabledForEmbedding = selection.count();

	for (int i = 0; i < selection.count(); ++i)
	{
		const QListWidgetItem* item = selection.at(i);
		const ScFace face = AllFonts[item->text()];
		if (face.isOTF() || face.subset())
			enabledForEmbedding--;
	}

	FromSubset->setEnabled(enabledForEmbedding > 0);
	ToSubset->setEnabled(false);
	EmbedList->clearSelection();
}

void TabPDFOptions::EmbedAll()
{
	EmbedList->clear();
	SubsetList->clear();
	ToSubset->setEnabled(false);
	FromSubset->setEnabled(false);
	for (int a = 0; a < m_docFonts.count(); ++a)
	{
		QString fontName = m_docFonts.at(a);
		const ScFace fontFace = AllFonts[fontName];
		if (!fontFace.subset() && !fontFace.isOTF())
		{
			QListWidgetItem* item = addFontItem(fontName, EmbedList);
			if (m_annotationFonts.contains(item->text()))
				item->setFlags(Qt::ItemIsEnabled);
		}
		else if (m_annotationFonts.contains(fontName))
		{
			QListWidgetItem* item = addFontItem(fontName, EmbedList);
			item->setFlags(Qt::ItemIsEnabled);
		}
		else
		{
			addFontItem(fontName, SubsetList);
		}
	}
}

void TabPDFOptions::SubsetAll()
{
	EmbedList->clear();
	SubsetList->clear();
	ToSubset->setEnabled(false);
	FromSubset->setEnabled(false);
	for (int a = 0; a < m_docFonts.count(); ++a)
	{
		QString fontName = m_docFonts.at(a);
		if (m_annotationFonts.contains(fontName))
		{
			QListWidgetItem* item = addFontItem(fontName, EmbedList);
			item->setFlags(Qt::ItemIsEnabled);
		}
		else
		{
			addFontItem(fontName, SubsetList);
		}
	}
}

QListWidgetItem* TabPDFOptions::addFontItem(QString fontName, QListWidget* fontList)
{
	QListWidgetItem* item = 0;
	if (!AllFonts.contains(fontName))
		return 0;

	const ScFace& face = AllFonts.value(fontName);
	if (face.isReplacement())
		item = new QListWidgetItem( IconManager::instance()->loadIcon("font_subst16.png"), fontName, fontList );
	else if (face.type() == ScFace::TYPE1)
		item = new QListWidgetItem( IconManager::instance()->loadIcon("font_type1_16.png"), fontName, fontList );
	else if (face.type() == ScFace::TTF)
		item = new QListWidgetItem( IconManager::instance()->loadIcon("font_truetype16.png"), fontName, fontList );
	else if (face.type() == ScFace::OTF)
		item = new QListWidgetItem( IconManager::instance()->loadIcon("font_otf16.png"), fontName, fontList );

	return item;
}

void TabPDFOptions::unitChange(int docUnitIndex)
{
	BleedBottom->setNewUnit(docUnitIndex);
	BleedTop->setNewUnit(docUnitIndex);
	BleedRight->setNewUnit(docUnitIndex);
	BleedLeft->setNewUnit(docUnitIndex);
	markOffset->setNewUnit(docUnitIndex);
}

void TabPDFOptions::createPageNumberRange( )
{
	CreateRange cr(PageNr->text(), m_Doc->DocPages.count(), this);
	if (cr.exec())
	{
		CreateRangeData crData;
		cr.getCreateRangeData(crData);
		PageNr->setText(crData.pageRange);
		return;
	}

	PageNr->setText(QString::null);
}
