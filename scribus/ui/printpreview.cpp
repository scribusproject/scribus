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
#include "printpreviewcreator.h"
#include "printpreviewcreatorfactory.h"
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

	m_previewCreator = PrintPreviewCreatorFactory::create(docu, postscriptPreview ? PrintEngine::PostScript3 : PrintEngine::WindowsGDI);

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

		inkTable = new QTableWidget(spots.count() + 4, 2, devTitle );
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
	previewLabel->setPixmap(createPreview(doc->currentPage()->pageNr(), 72));
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

	if (m_previewCreator)
	{
		delete m_previewCreator;
		m_previewCreator = nullptr;
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
	QPixmap previewPix = createPreview(n, qRound(72 * scaleFactor));
	previewLabel->setPixmap(previewPix);
	previewLabel->resize(previewPix.size());
}

void PrintPreview::redisplay()
{
	QPixmap previewPix = createPreview(m_currentPage, qRound(72 * scaleFactor));
	previewLabel->setPixmap(previewPix);
	previewLabel->resize(previewPix.size());
}

void PrintPreview::toggleCMYK()
{
	if (!haveTiffSep)
		return;

	bool c = enableCMYK->isChecked();
	inkTable->setEnabled(c);
	enableInkCover->setEnabled(c);
	if (enableInkCover->isChecked())
		coverThresholdValue->setEnabled(c);
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
		previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * scaleFactor)));
	previewLabel->resize(previewLabel->pixmap()->size());
}

void PrintPreview::doSpotTable(int row)
{
	if (!haveTiffSep)
		return;

	for (auto sepIt = flagsVisible.begin(); sepIt != flagsVisible.end(); ++sepIt)
		sepIt.value()->setChecked(false);
	((QCheckBox*)(inkTable->cellWidget(row, 0)))->setChecked(true);

	if (enableCMYK->isChecked())
		previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * scaleFactor)));
	previewLabel->resize(previewLabel->pixmap()->size());
}

void PrintPreview::toggleAllfromHeader()
{
	if (!haveTiffSep)
		return;

	for (auto sepIt = flagsVisible.begin(); sepIt != flagsVisible.end(); ++sepIt)
		sepIt.value()->setChecked(true);

	if (enableCMYK->isChecked())
		previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * scaleFactor)));
	previewLabel->resize(previewLabel->pixmap()->size());
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

QPixmap PrintPreview::createPreview(int pageIndex, int res)
{
	QPixmap pixmap;

	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));

	// Recreate Postscript-File only when necessary
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
		m_previewCreator->setPrintOptions(options);
	}

	m_previewCreator->setPreviewResolution(res);
	m_previewCreator->setAntialisingEnabled(antiAliasing->isChecked());
	m_previewCreator->setShowTransparency(showTransparency->isChecked());
	
	SeparationPreviewCreator* sepPreviewCreator = dynamic_cast<SeparationPreviewCreator*>(m_previewCreator);
	if (sepPreviewCreator)
	{
		sepPreviewCreator->setSeparationPreviewEnabled(enableCMYK->isChecked());
		sepPreviewCreator->setShowInkCoverage(enableInkCover->isChecked());
		sepPreviewCreator->setInkCoverageThreshold(coverThresholdValue->value());

		QStringList separationNames = sepPreviewCreator->separationNames();
		for (int i = 0; i < separationNames.count(); ++i)
		{
			const QString& separationName = separationNames.at(i);
			const QCheckBox* checkBox = flagsVisible.value(separationName, nullptr);
			if (checkBox)
				sepPreviewCreator->setSeparationVisible(separationName, checkBox->isChecked());
		}
	}

	pixmap = m_previewCreator->createPreview(pageIndex);

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
	else if (engine >= PrintEngine::PostScript1 && engine <= PrintEngine::PostScript3)
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
