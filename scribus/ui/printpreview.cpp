/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          preview.cpp  -  description
                             -------------------
    begin                : Sun Jun 1 08:00:00 CEST 2003
    copyright            : (C) 2003 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "printpreview.h"

#include <QApplication>
#include <QColor>
#include <QComboBox>
#include <QCursor>
#include <QDesktopWidget>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QToolTip>

#include <cstdlib>
#include <algorithm>

#include "checkDocument.h"
#include "cmsettings.h"
#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "pslib.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "ui/pageselector.h"
#include "util.h"
#include "util_ghostscript.h"
#include "util_printer.h"

#if defined(_WIN32)
#include "scprintengine_gdi.h"
#endif

extern bool printDinUse;

PrintPreview::PrintPreview(QWidget* parent, ScribusDoc *docu, const QString& printer, PrintEngine engine ) : QDialog( parent ),
	doc(docu),
	prefsManager(PrefsManager::instance())
{
	setModal(true);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	Q_ASSERT(!docu->masterPageMode());
	postscriptPreview = usePostscriptPreview(printer, engine);
	QString caption = tr("Print Preview");
#ifdef _WIN32
	if (postscriptPreview)
		caption += " (PostScript)";
	else
		caption += " (GDI)";
#endif
	setWindowTitle(caption);

	havePngAlpha = ScCore->havePNGAlpha();
	haveTiffSep  = postscriptPreview ? ScCore->haveTIFFSep() : false;
	getNumericGSVersion(m_gsVersion);

	PLayout = new QVBoxLayout(this);
	PLayout->setMargin(0);
	PLayout->setSpacing(0);
	int tbWidth = 0;
	Layout5 = new QHBoxLayout;
	Layout5->setSpacing(3);
	Layout5->setMargin(0);
	previewArea = new QScrollArea(this);
	previewArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	Layout5->addWidget(previewArea);
	settingsBarLayout = new QVBoxLayout;
	settingsBarLayout->setSpacing(3);
	settingsBarLayout->setMargin(0);
	devTitle = new QGroupBox( this );
	devTitle->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	devTitle->setTitle( tr( "Display Settings" ) );
	Layout2 = new QVBoxLayout( devTitle );
	Layout2->setSpacing(5);
	Layout2->setMargin(5);
	Layout2->setAlignment( Qt::AlignTop );
	antiAliasing = new QCheckBox(devTitle);
	antiAliasing->setText( tr("Enable &Antialiasing"));
	antiAliasing->setChecked(postscriptPreview ? prefsManager.appPrefs.printPreviewPrefs.PrPr_AntiAliasing : false);
	antiAliasing->setEnabled(postscriptPreview);
	Layout2->addWidget(antiAliasing);
	showTransparency = new QCheckBox(devTitle);
	showTransparency->setText( tr("Display Trans&parency"));
	showTransparency->setChecked(postscriptPreview ? prefsManager.appPrefs.printPreviewPrefs.PrPr_Transparency : false);
	showTransparency->setEnabled(postscriptPreview);
	Layout2->addWidget(showTransparency);
	enableCMYK = new QCheckBox(devTitle);
	enableCMYK->setText( tr("&Display CMYK"));
	enableCMYK->setChecked((haveTiffSep && postscriptPreview) ? prefsManager.appPrefs.printPreviewPrefs.PrPr_Mode : false);
	enableCMYK->setEnabled(haveTiffSep && postscriptPreview);
	Layout2->addWidget(enableCMYK);
	if (haveTiffSep)
	{
		ColorList usedSpots;
		doc->getUsedColors(usedSpots, true);
		QStringList spots = usedSpots.keys();

		inkTable = new QTableWidget(spots.count()+4, 2, devTitle );
		m_inkMax = (spots.count() + 4) * 255;
		inkTable->setHorizontalHeaderItem(0, new QTableWidgetItem(IconManager::instance().loadIcon("16/show-object.png"), ""));
		inkTable->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Separation Name")));
		QHeaderView *header = inkTable->horizontalHeader();
		header->setStretchLastSection(true);
		header->setSectionsMovable(false);
		header->setSectionResizeMode(QHeaderView::Fixed);
		inkTable->setColumnWidth(0, 24);
		inkTable->verticalHeader()->hide();
		inkTable->setSelectionMode( QAbstractItemView::NoSelection );
		inkTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		inkTable->setFocusPolicy(Qt::NoFocus);
		flagsVisible.clear();
		inkTable->setItem(0, 1, new QTableWidgetItem( tr("Cyan")));
		QCheckBox *cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		inkTable->setCellWidget(0, 0, cp);
		cp->setChecked(prefsManager.appPrefs.printPreviewPrefs.PrPr_C);
		flagsVisible.insert("Cyan", cp);
		inkTable->setItem(1, 1, new QTableWidgetItem( tr("Magenta")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		inkTable->setCellWidget(1, 0, cp);
		cp->setChecked(prefsManager.appPrefs.printPreviewPrefs.PrPr_M);
		flagsVisible.insert("Magenta", cp);
		inkTable->setItem(2, 1, new QTableWidgetItem( tr("Yellow")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		inkTable->setCellWidget(2, 0, cp);
		cp->setChecked(prefsManager.appPrefs.printPreviewPrefs.PrPr_Y);
		flagsVisible.insert("Yellow", cp);
		inkTable->setItem(3, 1, new QTableWidgetItem( tr("Black")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		inkTable->setCellWidget(3, 0, cp);
		cp->setChecked(prefsManager.appPrefs.printPreviewPrefs.PrPr_K);
		flagsVisible.insert("Black", cp);
		for (int sp = 0; sp < spots.count(); ++sp)
		{
			inkTable->setItem(sp+4, 1, new QTableWidgetItem(spots[sp]));
			cp = new QCheckBox(this);
			cp->setFocusPolicy(Qt::NoFocus);
			connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
			inkTable->setCellWidget(sp + 4, 0, cp);
			cp->setChecked(true);
			flagsVisible.insert(spots[sp], cp);
		}
		Layout2->addWidget(inkTable);
		tbWidth = inkTable->columnWidth(1);

		enableInkCover = new QCheckBox(devTitle);
		enableInkCover->setText( tr("Display Ink Coverage"));
		enableInkCover->setChecked(prefsManager.appPrefs.printPreviewPrefs.PrPr_InkCoverage);
		enableInkCover->setEnabled( postscriptPreview );
		Layout2->addWidget(enableInkCover);
		connect(enableInkCover, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		Layout7 = new QHBoxLayout;
		Layout7->setSpacing(3);
		Layout7->setMargin(0);
		thresholdLabel = new QLabel( tr("Threshold:"), devTitle);
		Layout7->addWidget(thresholdLabel);
		coverThresholdValue = new ScrSpinBox(devTitle);
		coverThresholdValue->setSuffix( tr(" %"));
		coverThresholdValue->setMaximum(600);
		coverThresholdValue->setDecimals(0);
		coverThresholdValue->setMinimum(0);
		coverThresholdValue->setSingleStep(10);
		coverThresholdValue->setSpecialValueText( tr("None"));
		coverThresholdValue->setValue(prefsManager.appPrefs.printPreviewPrefs.PrPr_InkThreshold);
		if ((enableCMYK->isChecked()) && (enableInkCover->isChecked()))
			coverThresholdValue->setEnabled(true);
		else
			coverThresholdValue->setEnabled(false);
		connect(coverThresholdValue, SIGNAL(valueChanged(double)), this, SLOT(toggleCMYK_Colour()));
		Layout7->addWidget(coverThresholdValue);
		Layout2->addLayout(Layout7);
		connect(inkTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(doSpotTable(int)));
		connect(header, SIGNAL(sectionClicked(int)), this, SLOT(toggleAllfromHeader()));
	}
	
	settingsBarLayout->addWidget(devTitle);
	jobTitle = new QGroupBox( this );
	jobTitle->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	jobTitle->setTitle( tr( "Preview Settings" ) );
	Layout1 = new QVBoxLayout( jobTitle );
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 5 );
	Layout1->setAlignment( Qt::AlignTop );

	mirrorHor = new QCheckBox( tr( "Mirror Page(s) Horizontal" ), jobTitle );
	Layout1->addWidget( mirrorHor );

	mirrorVert = new QCheckBox( tr( "Mirror Page(s) Vertical" ), jobTitle );
	Layout1->addWidget( mirrorVert );

	clipMargins = new QCheckBox( tr( "Clip to Printer Margins" ), jobTitle );
	Layout1->addWidget( clipMargins );

	useGray = new QCheckBox( tr("Print in Grayscale"), jobTitle);
	Layout1->addWidget(useGray);

	enableGCR = new QCheckBox( tr("&Under Color Removal"), jobTitle);
	Layout1->addWidget(enableGCR);

	spotColors = new QCheckBox( tr( "Convert Spot Colors" ), jobTitle );
	Layout1->addWidget( spotColors );

	settingsBarLayout->addWidget(jobTitle);
	QSpacerItem* spacerC = new QSpacerItem( 5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding );
	settingsBarLayout->addItem( spacerC );
	Layout5->addLayout(settingsBarLayout);
	PLayout->addLayout(Layout5);

	Layout6 = new QHBoxLayout;
	Layout6->setSpacing(0);
	Layout6->setMargin(6);
	/* scaling */
	scaleLabel = new QLabel( tr("Scaling:"), this);
	// NOTE: Be careful when you change scaleBox item order.
	// There is an index order related check in the resizeEvent().
	// resizeEvent() will re-create preview when is one of "Fit to ..."
	// options in use. - 20080204 PV
	scaleBox = new QComboBox(this);
	scaleBox->setEditable(false);
	scaleBox->setFocusPolicy(Qt::NoFocus);
	scaleBox->addItem("50%");
	scaleBox->addItem("100%");
	scaleBox->addItem("150%");
	scaleBox->addItem("200%");
	scaleBox->addItem( tr("Fit to Width"));
	scaleBox->addItem( tr("Fit to Height"));
	scaleBox->addItem( tr("Fit to Page"));
	scaleBox->setCurrentIndex(1);
	Layout6->addWidget(scaleLabel);
	Layout6->addWidget(scaleBox);
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout6->addItem( spacer );
	pageSelector = new PageSelector(this, doc->DocPages.count());
	pageSelector->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	Layout6->addWidget(pageSelector);
	QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout6->addItem( spacer2 );
	closeButton = new QPushButton( tr("Close"), this);
	closeButton->setAutoDefault(false);
	Layout6->addWidget( closeButton );
	printButton = new QPushButton( tr("Print..."), this );
	printButton->setAutoDefault(false);	
	printButton->setEnabled(!printDinUse);
	Layout6->addWidget( printButton );
	PLayout->addLayout(Layout6);
	setValues();
	previewLabel = new QLabel(this);
	previewLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	previewLabel->setPixmap(CreatePreview(doc->currentPage()->pageNr(), 72));
	previewLabel->resize(previewLabel->pixmap()->size());
	previewArea->setWidget(previewLabel);
	int w = previewLabel->width() + tbWidth + 50;
	resize(qMin(QApplication::desktop()->width() - 30, w), 500);
	if (!PrefsManager::instance().appPrefs.printPreviewPrefs.PrPr_Mode)
	{
		if (haveTiffSep)
			inkTable->setEnabled(false);
	}
	pageSelector->setGUIForPage(doc->currentPage()->pageNr());

	// tooltips
	antiAliasing->setToolTip( "<qt>" + tr( "Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing" ) + "</qt>" );
	showTransparency->setToolTip( "<qt>" + tr( "Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later." ) + "</qt>");
	enableCMYK->setToolTip( "<qt>" + tr( "Gives a print preview using simulations of generic CMYK inks, instead of RGB colors" ) + "</qt>");
	enableGCR->setToolTip( "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>" );
	scaleBox->setToolTip( "<qt>" + tr("Resize the scale of the page") + "</qt>");
	spotColors->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");

	//signals and slots
	connect(antiAliasing, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(showTransparency, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(enableCMYK, SIGNAL(clicked()), this, SLOT(toggleCMYK()));
	connect(enableGCR, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(mirrorHor, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(mirrorVert, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(clipMargins, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(spotColors, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(useGray, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(pageSelector, SIGNAL(pageChanged(int)), this, SLOT(jumpToPage(int)));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(printButton, SIGNAL(clicked()), this, SIGNAL(doPrint()));
	connect(scaleBox, SIGNAL(activated(int)), this, SLOT(scaleBox_valueChanged(int)));
}

PrintPreview::~PrintPreview()
{
	// Cleanup temporary files
	QString tempFileDir = ScPaths::tempFileDir();
	QFile::remove(tempFileDir + "/tmp.ps");
	QFile::remove(tempFileDir + "/sc.png");
	QDir d(tempFileDir + "/", "sc.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint i = 0; i < d.count(); i++)
			QFile::remove(tempFileDir + "/" + d[i]);
	}
}

void PrintPreview::setValues()
{
	if ((printDinUse) || (!doc->Print_Options.firstUse))
	{
		enableGCR->setChecked( postscriptPreview ? doc->Print_Options.doGCR : false);
		enableGCR->setEnabled( postscriptPreview );
		mirrorHor->setChecked(doc->Print_Options.mirrorH);
		mirrorVert->setChecked(doc->Print_Options.mirrorV);
		clipMargins->setChecked(doc->Print_Options.doClip);
		spotColors->setChecked(!doc->Print_Options.useSpotColors);
		useGray->setChecked(!doc->Print_Options.useColor);
	}
	else
	{
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getContext("print_options");
		enableGCR->setChecked( postscriptPreview ? prefs->getBool("DoGCR", false) : false);
		enableGCR->setEnabled( postscriptPreview );
		mirrorHor->setChecked(prefs->getBool("MirrorH", false));
		mirrorVert->setChecked(prefs->getBool("MirrorV", false));
		clipMargins->setChecked(prefs->getBool("Clip", false));
		spotColors->setChecked(!prefs->getBool("doSpot", true));
		if (prefs->getInt("PrintColor", 0) == 1)
			useGray->setChecked(true);
		else
			useGray->setChecked(false);
	}
}

void PrintPreview::jumpToPage(int num)
{
	int n = num-1;
	if (n == m_currentPage)
		return;
	QPixmap previewPix = CreatePreview(n, qRound(72 * scaleFactor));
	previewLabel->setPixmap(previewPix);
	previewLabel->resize(previewPix.size());
}

void PrintPreview::redisplay()
{
	QPixmap previewPix = CreatePreview(m_currentPage, qRound(72 * scaleFactor));
	previewLabel->setPixmap(previewPix);
	previewLabel->resize(previewPix.size());
}

void PrintPreview::toggleCMYK()
{
	bool c = enableCMYK->isChecked();
	if (haveTiffSep)
	{
		inkTable->setEnabled(c);
		enableInkCover->setEnabled(c);
		if (enableInkCover->isChecked())
			coverThresholdValue->setEnabled(c);
	}
		
	redisplay();
}

void PrintPreview::toggleCMYK_Colour()
{
	if (haveTiffSep)
	{
		if ((enableCMYK->isChecked()) && (enableInkCover->isChecked()))
			coverThresholdValue->setEnabled(true);
		else
			coverThresholdValue->setEnabled(false);
	}
	if (enableCMYK->isChecked())
		previewLabel->setPixmap(CreatePreview(m_currentPage, qRound(72 * scaleFactor)));
	previewLabel->resize(previewLabel->pixmap()->size());
}

void PrintPreview::doSpotTable(int row)
{
	if (haveTiffSep)
	{
		QMap<QString, QCheckBox*> ::Iterator sepit;
		for (sepit = flagsVisible.begin(); sepit != flagsVisible.end(); ++sepit)
		{
			sepit.value()->setChecked(false);
		}
		((QCheckBox*)(inkTable->cellWidget(row, 0)))->setChecked(true);
		if (enableCMYK->isChecked())
			previewLabel->setPixmap(CreatePreview(m_currentPage, qRound(72 * scaleFactor)));
		previewLabel->resize(previewLabel->pixmap()->size());
	}
}

void PrintPreview::toggleAllfromHeader()
{
	if (haveTiffSep)
	{
		QMap<QString, QCheckBox*> ::Iterator sepit;
		for (sepit = flagsVisible.begin(); sepit != flagsVisible.end(); ++sepit)
		{
			sepit.value()->setChecked(true);
		}
		if (enableCMYK->isChecked())
			previewLabel->setPixmap(CreatePreview(m_currentPage, qRound(72 * scaleFactor)));
		previewLabel->resize(previewLabel->pixmap()->size());
	}
}

void PrintPreview::scaleBox_valueChanged(int value)
{
	switch (value)
	{
		case 0:
			scaleFactor = 0.5;
			break;
		case 1:
			scaleFactor = 1.0;
			break;
		case 2:
			scaleFactor = 1.5;
			break;
		case 3:
			scaleFactor = 2.0;
			break;
		case 4:
			scaleFactor = previewArea->viewport()->width() / doc->Pages->at(m_currentPage)->width();
			break;
		case 5:
			scaleFactor = previewArea->viewport()->height() / doc->Pages->at(m_currentPage)->height();
			break;
		case 6:
			scaleFactor = qMin(previewArea->viewport()->height() / doc->Pages->at(m_currentPage)->height(), previewArea->viewport()->width() / doc->Pages->at(m_currentPage)->width());
			break;
		default:
			scaleFactor = 1.0;
			break;
	}
	redisplay();
}

int PrintPreview::RenderPreview(int pageIndex, int res)
{
	int ret = -1;
	QString cmd1;
#if defined _WIN32
	if (!postscriptPreview)
	{
		QImage image;
		ScPage* page;
		ScPrintEngine_GDI winPrint;
		PrintOptions options;
		page = doc->Pages->at(pageIndex);
		options.copies = 1;
		options.doGCR = false;
		//options.mirrorH = options.mirrorV = false;
		options.mirrorH = mirrorHor->isChecked();
		options.mirrorV = mirrorVert->isChecked();
		options.outputSeparations = false;
		options.pageNumbers.push_back(pageIndex);
		options.prnEngine = WindowsGDI;
		options.separationName = "All";
		options.toFile = false;
		options.useColor = !useGray->isChecked();
		options.useSpotColors = false;
		bool done = winPrint.gdiPrintPreview(doc, page, &image, options, res / 72.0);
		if (done)
			image.save( ScPaths::tempFileDir() + "/sc.png", "PNG" );
		return (done ? 0 : 1);
	}
#endif
	// Recreate Postscript-File only when the actual Page has changed
	if ((pageIndex != m_currentPage)  || (enableGCR->isChecked() != m_useGCR)  || (useGray->isChecked() != m_useGray)
		|| (mirrorHor->isChecked() != m_mirrorH) || (mirrorVert->isChecked() != m_mirrorV) || (clipMargins->isChecked() != m_clipToMargins)
		|| (spotColors->isChecked() != m_convertSpots))
	{
		PrintOptions options;
		options.pageNumbers.push_back(pageIndex + 1);
		options.outputSeparations = false;
		options.separationName = "All";
		options.allSeparations = QStringList();
		options.useSpotColors = !spotColors->isChecked();
		options.useColor = !useGray->isChecked();
		options.mirrorH = mirrorHor->isChecked();
		options.mirrorV = mirrorVert->isChecked();
		options.doGCR = enableGCR->isChecked();
		options.setDevParam = false;
		options.doClip = clipMargins->isChecked();
		options.cropMarks = false;
		options.bleedMarks = false;
		options.registrationMarks = false;
		options.colorMarks = false;
		options.includePDFMarks = false;
		options.markLength = 20.0;
		options.markOffset = 0.0;
		options.bleeds.set(0, 0, 0, 0);
		PSLib *dd = new PSLib(doc, options, PSLib::OutputPS, &doc->PageColors);
		if (!dd)
			return ret;
		ret = dd->createPS(ScPaths::tempFileDir() + "/tmp.ps");
		delete dd;
		if (ret != 0) return 1;
	}
	QStringList args;
	QString tmp, tmp2, tmp3;
	double b = doc->Pages->at(pageIndex)->width() * res / 72.0;
	double h = doc->Pages->at(pageIndex)->height() * res / 72.0;
	if (doc->Pages->at(pageIndex)->orientation() == 1)
		std::swap(b, h);
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( QString("-r%1").arg(tmp.setNum(res)) );
	args.append( QString("-g%1x%2").arg(tmp2.setNum(qRound(b)), tmp3.setNum(qRound(h))) );
	if (enableCMYK->isChecked())
	{
		if (haveTiffSep)
			args.append( "-sDEVICE=tiffsep" );
		else
			return 1;
	}
	else
	{
		if (showTransparency->isChecked() && havePngAlpha)
			args.append("-sDEVICE=pngalpha");
		else
			args.append("-sDEVICE=tiff24nc");	
	}
	if (antiAliasing->isChecked())
	{
		args.append( "-dTextAlphaBits=4" );
		args.append( "-dGraphicsAlphaBits=4" );
	}
	if ((doc->HasCMS) && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(doc->DocPrinterProf.profilePath()));
		if (enableCMYK->isChecked())
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(doc->DocPrinterProf.profilePath()));
		else
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(doc->DocDisplayProf.profilePath()));
	}
	else if (ScCore->haveCMS() && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
		if (enableCMYK->isChecked())
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
		else
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(ScCore->defaultRGBProfile.profilePath()));
	}
	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = prefsManager.prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd1 = QString("-sFONTPATH=%1").arg(QDir::toNativeSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd1 += QString("%1%2").arg(sep).arg(QDir::toNativeSeparators(extraFonts->get(i,0)));
	if (!cmd1.isEmpty())
		args.append( cmd1 );
	// then add any final args and call gs
	if (enableCMYK->isChecked())
		args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sc.tif")) );
	else if ((showTransparency->isChecked() && havePngAlpha) || !postscriptPreview)
		args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sc.png")) );
	else
		args.append(QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sc.tif")));
	args.append( QDir::toNativeSeparators(ScPaths::tempFileDir() + "/tmp.ps") );
	args.append( "-c" );
	args.append( "showpage" );
	args.append( "-c" );
	args.append( "quit" );
	ret = System(getShortPathName(prefsManager.ghostscriptExecutable()), args);
	return ret;
}

int PrintPreview::RenderPreviewSep(int pageIndex, int res)
{
	int ret = -1;
	QString cmd;
	QStringList args, args1, args2, args3;
	// Recreate Postscript-File only when the actual Page has changed
	if ((pageIndex != m_currentPage)  || (enableGCR->isChecked() != m_useGCR) || (useGray->isChecked() != m_useGray)
		|| (mirrorHor->isChecked() != m_mirrorH) || (mirrorVert->isChecked() != m_mirrorV) || (clipMargins->isChecked() != m_clipToMargins)
		|| (spotColors->isChecked() != m_convertSpots))
	{
		PrintOptions options;
		options.pageNumbers.push_back(pageIndex + 1);
		options.outputSeparations = false;
		options.separationName = "All";
		options.allSeparations = QStringList();
		options.useSpotColors = !spotColors->isChecked();
		options.useColor = !useGray->isChecked();
		options.mirrorH = mirrorHor->isChecked();
		options.mirrorV = mirrorVert->isChecked();
		options.doGCR = enableGCR->isChecked();
		options.setDevParam = false;
		options.doClip = clipMargins->isChecked();
		options.cropMarks = false;
		options.bleedMarks = false;
		options.registrationMarks = false;
		options.colorMarks = false;
		options.includePDFMarks = false;
		options.markLength = 20.0;
		options.markOffset = 0.0;
		options.bleeds.set(0, 0, 0, 0);
		PSLib *dd = new PSLib(doc, options, PSLib::OutputPS, &doc->PageColors);
		if (!dd)
			return -1;
		ret = dd->createPS(ScPaths::tempFileDir() + "/tmp.ps");
		delete dd;
		if (ret != 0) return 1;
	}
	QString tmp, tmp2, tmp3;
	double b = doc->Pages->at(pageIndex)->width() * res / 72.0;
	double h = doc->Pages->at(pageIndex)->height() * res / 72.0;
	if (doc->Pages->at(pageIndex)->orientation() == 1)
		std::swap(b, h);

	args1.append( "-q" );
	args1.append( "-dNOPAUSE" );
	args1.append( "-dPARANOIDSAFER" );
	args1.append( QString("-r%1").arg(tmp.setNum(res)) );
	args1.append( QString("-g%1x%2").arg(tmp2.setNum(qRound(b)), tmp3.setNum(qRound(h))) );
	if (antiAliasing->isChecked())
	{
		args1.append("-dTextAlphaBits=4");
		args1.append("-dGraphicsAlphaBits=4");
	}
	if ((doc->HasCMS) && (m_gsVersion >= 900))
	{
		args1.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(doc->DocPrinterProf.profilePath()));
		args1.append("-sOutputICCProfile=" + QDir::toNativeSeparators(doc->DocPrinterProf.profilePath()));
	}
	else if (ScCore->haveCMS() && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
		args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
	}
	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = prefsManager.prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(QDir::toNativeSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(QDir::toNativeSeparators(extraFonts->get(i,0)));
	if (!cmd.isEmpty())
		args1.append(cmd);
	args1.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sc.tif")) );

	args2.append( QDir::toNativeSeparators(ScPaths::tempFileDir() + "/tmp.ps") );
	args2.append("-c");
	args2.append("quit");

	ColorList usedSpots;
	doc->getUsedColors(usedSpots, true);
	QStringList spots = usedSpots.keys();
	args3.append( "-sDEVICE=tiffsep" );

//	args3.append( "-c" );
	cmd = "<< /SeparationColorNames ";
	QString allSeps ="[ /Cyan /Magenta /Yellow /Black ";
	for (int sp = 0; sp < spots.count(); ++sp)
	{
		allSeps += "("+spots[sp]+") ";
	}
	allSeps += "]";
	cmd += allSeps + " /SeparationOrder [ /Cyan /Magenta /Yellow /Black] >> setpagedevice";
	QFile fx(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sep.ps"));
	if (fx.open(QIODevice::WriteOnly))
	{
		QTextStream tsx(&fx);
		tsx << cmd;
		fx.close();
	}
//	args3.append("-f");
//	args3.append(QDir::toNativeSeparators(ScPaths::getTempFileDir() + "/sep.ps"));
//	args3.append(cmd);

//	args3.append("-f");
	QString gsExe(getShortPathName(prefsManager.ghostscriptExecutable()));
	ret = System(gsExe, args1 + args3 + args2, ScPaths::tempFileDir() + "/sc.tif.txt" );

	QFile sepInfo(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sc.tif.txt"));
	sepsToFileNum.clear();
	if (sepInfo.open(QIODevice::ReadOnly))
	{
		QString Sname;
		QTextStream tsC(&sepInfo);
		int counter = 0;
		while (!tsC.atEnd())
		{
			Sname = tsC.readLine();
			QString tt = Sname.remove("%%SeparationName:").trimmed();
			if (!tt.isEmpty())
			{
				sepsToFileNum.insert(tt, counter);
				counter++;
			}
		}
	}
	sepInfo.close();
	QString currSeps = "";
	uint spc = 0;
	for (int sp = 0; sp < spots.count(); ++sp)
	{
		currSeps += "("+spots[sp]+") ";
		spc++;
		if (sp > 6)
		{
			args3.clear();
			args3.append("-sDEVICE=tiffsep");
			QFile fx(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sep.ps"));
			if (fx.open(QIODevice::WriteOnly))
			{
				QTextStream tsx(&fx);
				tsx << QString("<< /SeparationColorNames "+allSeps+" /SeparationOrder [ "+currSeps+" ] >> setpagedevice");
				fx.close();
			}
			args3.append("-f");
			args3.append(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sep.ps"));
	//		args3.append("-c");
	//		args3.append("<< /SeparationColorNames "+allSeps+" /SeparationOrder [ "+currSeps+" ] >> setpagedevice");
	//		args3.append("-f");
			ret = System(gsExe, args1 + args3 + args2);
			currSeps = "";
			spc = 0;
		}
	}
	if (spc != 0)
	{
		args3.clear();
		args3.append("-sDEVICE=tiffsep");
		QFile fx(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sep.ps"));
		if (fx.open(QIODevice::WriteOnly))
		{
			QTextStream tsx(&fx);
			tsx << QString("<< /SeparationColorNames "+allSeps+" /SeparationOrder [ "+currSeps+" ] >> setpagedevice");
			fx.close();
		}
		args3.append("-f");
		args3.append(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/sep.ps"));
	//	args3.append("-c");
	//	args3.append("<< /SeparationColorNames "+allSeps+" /SeparationOrder [ "+currSeps+" ] >> setpagedevice");
	//	args3.append("-f");
		ret = System(gsExe, args1 + args3 + args2);
	}
	return ret;
}

// this should move to scimage.cpp!
void PrintPreview::blendImages(QImage &target, ScImage &scsource, ScColor col)
{
	QImage source = scsource.qImage(); // FIXME: this will not work once qImage always returns ARGB!
	
	//FIXME: if source and target have different sizesomething went wrong.
	// eg. loadPicture() failed and returned a 1x1 image
	CMYKColor cmykValues;
	int h = qMin(target.height(), source.height());
	int w = qMin(target.width(), source.width());
	int cyan, c, m, yc, k, cc, mm, yy, kk;
	ScColorEngine::getCMYKValues(col, doc, cmykValues);
	cmykValues.getValues(c, m, yc, k);
	for (int y = 0; y < h; ++y )
	{
		QRgb *p = (QRgb *) target.scanLine(y);
		QRgb *pq = (QRgb *) source.scanLine(y);
		for (int x = 0; x < w; ++x )
		{
			cyan = 255 - qRed(*pq);
			if (cyan != 0)
			{
				(c == 0) ? cc = qRed(*p) : cc = qMin(c * cyan / 255 + qRed(*p), 255);
				(m == 0) ? mm = qGreen(*p) : mm = qMin(m * cyan / 255 + qGreen(*p), 255);
				(yc == 0) ? yy = qBlue(*p) : yy = qMin(yc * cyan / 255 + qBlue(*p), 255);
				(k == 0) ? kk = qAlpha(*p) : kk = qMin(k * cyan / 255 + qAlpha(*p), 255);
				*p = qRgba(cc, mm, yy, kk);
			}
			p++;
			pq++;
		}
	}
}

void PrintPreview::blendImagesSumUp(QImage &target, ScImage &scsource)
{
	QImage source = scsource.qImage(); // FIXME: this will not work once qImage always returns ARGB!
	//FIXME: if source and target have different sizesomething went wrong.
	// eg. loadPicture() failed and returned a 1x1 image
	int h = qMin(target.height(), source.height());
	int w = qMin(target.width(), source.width());
	int cyan;
	for (int y = 0; y < h; ++y )
	{
		uint *p = (QRgb *) target.scanLine(y);
		QRgb *pq = (QRgb *) source.scanLine(y);
		for (int x = 0; x < w; ++x )
		{
			cyan = 255 - qRed(*pq);
			if (cyan != 0)
			{
				*p += cyan;
			}
			p++;
			pq++;
		}
	}
}

QPixmap PrintPreview::CreatePreview(int pageIndex, int res)
{
	int ret = -1;
	QPixmap pixmap;
	double b = doc->Pages->at(pageIndex)->width() * res / 72.0;
	double h = doc->Pages->at(pageIndex)->height() * res / 72.0;
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	if ((pageIndex != m_currentPage) || (enableCMYK->isChecked() != m_colorMode) || (m_scaleMode != scaleBox->currentIndex())
	        || (antiAliasing->isChecked() != m_useAntialiasing) || (((showTransparency->isChecked() != m_showTransparency) || (enableGCR->isChecked() != m_useGCR))
			&& (!enableCMYK->isChecked()))
			 || (useGray->isChecked() != m_useGray) || (mirrorHor->isChecked() != m_mirrorH) || (mirrorVert->isChecked() != m_mirrorV)
			 || (clipMargins->isChecked() != m_clipToMargins) || (spotColors->isChecked() != m_convertSpots))
	{
		if (!enableCMYK->isChecked() || (!haveTiffSep))
		{
			ret = RenderPreview(pageIndex, res);
			if (ret > 0)
			{
				imageLoadError(pixmap, pageIndex);
				return pixmap;
			}
		}
	}
	QImage image;
	if (enableCMYK->isChecked())
	{
		bool loaderror;
		int cyan, magenta, yellow, black;
		if (haveTiffSep)
		{
			if ((pageIndex != m_currentPage) || (enableCMYK->isChecked() != m_colorMode) || (m_scaleMode != scaleBox->currentIndex())
	       	 || (antiAliasing->isChecked() != m_useAntialiasing) || (showTransparency->isChecked() != m_showTransparency) || (enableGCR->isChecked() != m_useGCR)
	       	 || (useGray->isChecked() != m_useGray)  || (mirrorHor->isChecked() != m_mirrorH)|| (mirrorVert->isChecked() != m_mirrorV)
	       	 || (clipMargins->isChecked() != m_clipToMargins) || (spotColors->isChecked() != m_convertSpots))
			{
				ret = RenderPreviewSep(pageIndex, res);
				if (ret > 0)
				{
					imageLoadError(pixmap, pageIndex);
					return pixmap;
				}
			}
			ScImage im;
			bool mode;
			int w = qRound(b);
			int h2 = qRound(h);
			if (doc->Pages->at(pageIndex)->orientation() == 1)
				std::swap(w, h2);
			image = QImage(w, h2, QImage::Format_ARGB32);
			QRgb clean = qRgba(0, 0, 0, 0);
			for (int yi = 0; yi < h2; ++yi)
			{
				QRgb *q = (QRgb*) image.scanLine(yi);
				for (int xi = 0; xi < w; ++xi)
				{
					*q = clean;
					q++;
				}
			}
			CMSettings cms(doc, "", Intent_Perceptual);
			cms.allowColorManagement(false);
			if (flagsVisible["Cyan"]->isChecked())
			{
				if (m_gsVersion < 854)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.tif.Cyan.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else if (m_gsVersion <= 905)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.Cyan.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc(Cyan).tif", 1, cms, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(pixmap, pageIndex);
					return pixmap;
				}
				if (enableInkCover->isChecked())
					blendImagesSumUp(image, im);
				else
					blendImages(image, im, ScColor(255, 0, 0, 0));
			}
			if (flagsVisible["Magenta"]->isChecked())
			{
				if (m_gsVersion < 854)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.tif.Magenta.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else if (m_gsVersion <= 905)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.Magenta.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc(Magenta).tif", 1, cms, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(pixmap, pageIndex);
					return pixmap;
				}
				if (enableInkCover->isChecked())
					blendImagesSumUp(image, im);
				else
					blendImages(image, im, ScColor(0, 255, 0, 0));
			}
			if (flagsVisible["Yellow"]->isChecked())
			{
				if (m_gsVersion < 854)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.tif.Yellow.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else if (m_gsVersion <= 905)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.Yellow.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc(Yellow).tif", 1, cms, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(pixmap, pageIndex);
					return pixmap;
				}
				if (enableInkCover->isChecked())
					blendImagesSumUp(image, im);
				else
					blendImages(image, im, ScColor(0, 0, 255, 0));
			}
			if (!sepsToFileNum.isEmpty())
			{
				QMap<QString, int>::Iterator sepit;
				for (sepit = sepsToFileNum.begin(); sepit != sepsToFileNum.end(); ++sepit)
				{
					const QCheckBox* checkBox = flagsVisible.value(sepit.key(), nullptr);
					if (checkBox && checkBox->isChecked())
					{
						QString fnam;
						if (m_gsVersion < 854)
							fnam = QString(ScPaths::tempFileDir() + "/sc.tif.s%1.tif").arg(sepit.value());
						else if (m_gsVersion <= 905)
							fnam = QString(ScPaths::tempFileDir() + "/sc.s%1.tif").arg(sepit.value());
						else
							fnam = QString(ScPaths::tempFileDir() + "/sc(%1).tif").arg(sepit.key());
						if (!im.loadPicture(fnam, 1, cms, ScImage::RGBData, 72, &mode))
						{
							imageLoadError(pixmap, pageIndex);
							return pixmap;
						}
						if (enableInkCover->isChecked())
							blendImagesSumUp(image, im);
						else
							blendImages(image, im, doc->PageColors[sepit.key()]);
					}
				}
			}
			if (flagsVisible["Black"]->isChecked())
			{
				CMSettings cms(doc, "", Intent_Perceptual);
				cms.allowColorManagement(false);
				if (m_gsVersion < 854)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.tif.Black.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else if (m_gsVersion <= 905)
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc.Black.tif", 1, cms, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.loadPicture(ScPaths::tempFileDir() + "/sc(Black).tif", 1, cms, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(pixmap, pageIndex);
					return pixmap;
				}
				if (enableInkCover->isChecked())
					blendImagesSumUp(image, im);
				else
					blendImages(image, im, ScColor(0, 0, 0, 255));
			}
			if (enableInkCover->isChecked())
			{
				uint limitVal = (coverThresholdValue->value() * 255) / 100;
				for (int yi = 0; yi < h2; ++yi)
				{
					QRgb *q = (QRgb*) image.scanLine(yi);
					for (int xi = 0; xi < w; ++xi)
					{
						uint greyVal = *q;
						if (greyVal != 0)
						{
							if (limitVal == 0)
							{
								QColor tmpC;
								tmpC.setHsv((greyVal * 359) / m_inkMax, 255, 255);
								*q = tmpC.rgba();
							}
							else
							{
								int col = qMin(255 - static_cast<int>(((greyVal * 128) / m_inkMax) * 2), 255);
								if ((*q > 0) && (*q < limitVal))
									*q = qRgba(col, col, col, 255);
								else
									*q = qRgba(col, 0, 0, 255);
							}
						}
						else
						{
							if (!showTransparency->isChecked())
								*q = qRgba(255, 255, 255, 255);
						}
						q++;
					}
				}
			}
			else
			{
				if (doc->HasCMS || ScCore->haveCMS())
				{
					QRgb alphaFF = qRgba(0,0,0,255);
					QRgb alphaOO = qRgba(255,255,255,0);
					ScColorMgmtEngine engine = doc->colorEngine;
					ScColorProfile cmykProfile = doc->HasCMS ? doc->DocPrinterProf : ScCore->defaultCMYKProfile;
					ScColorProfile rgbProfile  = doc->HasCMS ? doc->DocDisplayProf : ScCore->defaultRGBProfile;
					ScColorTransform transCMYK = engine.createTransform(cmykProfile, Format_YMCK_8, rgbProfile, Format_BGRA_8, Intent_Relative_Colorimetric, 0);
					for (int yi = 0; yi < h2; ++yi)
					{
						uchar* ptr = image.scanLine( yi );
						transCMYK.apply(ptr, ptr, image.width());
						QRgb *q = (QRgb *) ptr;
						for (int xi = 0; xi < image.width(); xi++, q++)
						{
							if (showTransparency->isChecked())
							{
								cyan = qRed(*q);
								magenta = qGreen(*q);
								yellow = qBlue(*q);
								if	((cyan == 255) && (magenta == 255) && (yellow == 255))
									*q = alphaOO;
								else
									*q |= alphaFF;
							}
							else
								*q |= alphaFF;
						}
					}
				}
				else
				{
					for (int yi = 0; yi < h2; ++yi)
					{
						QRgb *q = (QRgb*) image.scanLine(yi);
						for (int xi = 0; xi < w; ++xi)
						{
							cyan = qRed(*q);
							magenta = qGreen(*q);
							yellow = qBlue(*q);
							black = qAlpha(*q);
							if ((cyan != 0) || (magenta != 0) || (yellow != 0 ) || (black != 0))
								*q = qRgba(255 - qMin(255, cyan+black), 255 - qMin(255, magenta+black), 255 - qMin(255, yellow+black), 255);
							else
							{
								if (!showTransparency->isChecked())
									*q = qRgba(255, 255, 255, 255);
							}
							q++;
						}
					}
				}
			}
		}
	}
	else
	{
		QString previewFile;
		if ((showTransparency->isChecked() && havePngAlpha) || !postscriptPreview)
			previewFile = ScPaths::tempFileDir() + "/sc.png";
		else
			previewFile = ScPaths::tempFileDir() + "/sc.tif";
		if (!image.load(previewFile))
		{
			imageLoadError(pixmap, pageIndex);
			return pixmap;
		}
		image = image.convertToFormat(QImage::Format_ARGB32);
		if ((showTransparency->isChecked()) && (havePngAlpha))
		{
			int wi = image.width();
			int hi = image.height();
			for (int yi = 0; yi < hi; ++yi)
			{
				QRgb *s = (QRgb*) image.scanLine(yi);
				for (int xi = 0; xi < wi; ++xi)
				{
					if ((*s) == 0xffffffff)
						(*s) &= 0x00ffffff;
					s++;
				}
			}
		}
	}
	const ScPage* page = doc->Pages->at(pageIndex);
	if ((page->orientation() == 1) && (image.width() < image.height()))
		image = image.transformed( QMatrix(0, 1, -1, 0, 0, 0) );
	if (showTransparency->isChecked())
	{
		pixmap = QPixmap(image.width(), image.height());
		QPainter p;
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
		p.begin(&pixmap);
		p.fillRect(0, 0, image.width(), image.height(), b);
		p.drawImage(0, 0, image);
		p.end();
	}
	else
		pixmap = QPixmap::fromImage(image);
	qApp->restoreOverrideCursor();
	getUserSelection(pageIndex);
	return pixmap;
}

//-------------------------------------------------------------------------------------------------

bool PrintPreview::usePostscriptPreview(const QString& printerName, PrintEngine engine)
{
#ifdef _WIN32
	if (printerName == tr("File"))
		return true;
	else if (printerName.isEmpty())
		return PrinterUtil::isPostscriptPrinter( ScPrintEngine_GDI::getDefaultPrinter() );
	else if (engine >= PostScript1 && engine <= PostScript3)
		return PrinterUtil::isPostscriptPrinter( printerName );
	return false;
#else
	return true;
#endif
}

//-------------------------------------------------------------------------------------------------

void PrintPreview::getUserSelection(int page)
{
	m_currentPage = page;
	m_colorMode = enableCMYK->isChecked();
	m_useAntialiasing = antiAliasing->isChecked();
	m_showTransparency = showTransparency->isChecked();
	m_useGCR = enableGCR->isChecked();
	m_scaleMode = scaleBox->currentIndex();
	m_mirrorH = mirrorHor->isChecked();
	m_mirrorV = mirrorVert->isChecked();
	m_clipToMargins = clipMargins->isChecked();
	m_convertSpots = spotColors->isChecked();
	m_useGray = useGray->isChecked();
}

void PrintPreview::imageLoadError(QPixmap &pixmap, int page)
{
	pixmap = QPixmap(1,1);
	qApp->restoreOverrideCursor();
	getUserSelection(page);
}

void PrintPreview::resizeEvent(QResizeEvent * event)
{
	QDialog::resizeEvent(event);
	int scaleIndex = scaleBox->currentIndex();
	// Repaint only for "fit to" options in the combo box
	if (scaleIndex <= 3)
		return;
	// HACK: m_scaleMode is reset to insane value to force redraw
	// as the value is checked for change.
	m_scaleMode = -1;
	scaleBox_valueChanged(scaleIndex);
}
