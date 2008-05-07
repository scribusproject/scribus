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
#include "preview.h"

#include <QImage>
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QComboBox>
#include <QPixmap>
#include <QCursor>
#include <QPainter>
#include <QColor>
#include <QToolTip>
#include <QFile>
#include <QTextStream>

#include <cstdlib>
#include "pslib.h"
#include "checkDocument.h"
#include "cmsettings.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "util.h"
#include "util_icon.h"
#include "sccolor.h"
#include "scribusview.h"
#include "scribusdoc.h"
#include "scpaths.h"
#include "pageselector.h"
#include "util_printer.h"
#include "util_ghostscript.h"
#include "sccolorengine.h"
#include "scribuscore.h"

#if defined(_WIN32)
#include "scprintengine_gdi.h"
#endif

extern bool printDinUse;

PPreview::PPreview( QWidget* parent, ScribusView *vin, ScribusDoc *docu, QString printer, PrintEngine engine ) : QDialog( parent )
{
	setModal(true);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	Q_ASSERT(!docu->masterPageMode());
	prefsManager=PrefsManager::instance();
	QString tmp;
	postscriptPreview = usePostscriptPreview(printer, engine);
	QString caption = tr("Print Preview");
#ifdef _WIN32
	if (postscriptPreview)
		caption += " (Postscript)";
	else
		caption += " (GDI)";
#endif
	setWindowTitle( caption );
	doc = docu;
	view = vin;
	HavePngAlpha = ScCore->havePNGAlpha();
	HaveTiffSep  = postscriptPreview ? ScCore->haveTIFFSep() : false;
	APage = -1;
	CMode = false;
	GsAl = false;
	Trans = false;
	GMode = true;
	OMode = false;
	mHor = false;
	mVer = false;
	fClip = false;
	fSpot = true;
	fGray = false;
	fICC = false;
	scaleFactor = 1.0;
	SMode = 1;
	getNumericGSVersion(GsMajor, GsMinor);
	PLayout = new QVBoxLayout(this);
	PLayout->setMargin(0);
	PLayout->setSpacing(0);
	int tbWidth = 0;
	Layout5 = new QHBoxLayout;
	Layout5->setSpacing(3);
	Layout5->setMargin(0);
	Anzeige = new QScrollArea(this);
	Anzeige->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	Layout5->addWidget(Anzeige);
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
	AntiAlias = new QCheckBox(devTitle);
	AntiAlias->setText( tr("Enable &Antialiasing"));
	AntiAlias->setChecked( postscriptPreview ? prefsManager->appPrefs.PrPr_AntiAliasing : false);
	AntiAlias->setEnabled( postscriptPreview );
	Layout2->addWidget(AntiAlias);
	AliasTr = new QCheckBox(devTitle);
	AliasTr->setText( tr("Display Trans&parency"));
	AliasTr->setChecked(prefsManager->appPrefs.PrPr_Transparency);
	AliasTr->setEnabled( postscriptPreview );
	Layout2->addWidget(AliasTr);
	EnableCMYK = new QCheckBox(devTitle);
	EnableCMYK->setText( tr("&Display CMYK"));
	EnableCMYK->setChecked( postscriptPreview ? prefsManager->appPrefs.PrPr_Mode : false);
	EnableCMYK->setEnabled( postscriptPreview );
	Layout2->addWidget(EnableCMYK);
	if (HaveTiffSep)
	{
		ColorList usedSpots;
		doc->getUsedColors(usedSpots, true);
		QStringList spots = usedSpots.keys();

		Table = new QTableWidget(spots.count()+4, 2, devTitle );
		Table->setHorizontalHeaderItem(0, new QTableWidgetItem(QIcon(loadIcon("16/show-object.png")), ""));
		Table->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Separation Name")));
		QHeaderView *header = Table->horizontalHeader();
		header->setStretchLastSection(true);
		header->setMovable(false);
		header->setClickable(false);
		header->setResizeMode(QHeaderView::Fixed);
		Table->setColumnWidth(0, 24);
		Table->verticalHeader()->hide();
		Table->setSelectionMode( QAbstractItemView::NoSelection );
		Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
		Table->setFocusPolicy(Qt::NoFocus);
		flagsVisible.clear();
		Table->setItem(0, 1, new QTableWidgetItem( tr("Cyan")));
		QCheckBox *cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(0, 0, cp);
		cp->setChecked(prefsManager->appPrefs.PrPr_C);
		flagsVisible.insert("Cyan", cp);
		Table->setItem(1, 1, new QTableWidgetItem( tr("Magenta")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(1, 0, cp);
		cp->setChecked(prefsManager->appPrefs.PrPr_M);
		flagsVisible.insert("Magenta", cp);
		Table->setItem(2, 1, new QTableWidgetItem( tr("Yellow")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(2, 0, cp);
		cp->setChecked(prefsManager->appPrefs.PrPr_Y);
		flagsVisible.insert("Yellow", cp);
		Table->setItem(3, 1, new QTableWidgetItem( tr("Black")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(3, 0, cp);
		cp->setChecked(prefsManager->appPrefs.PrPr_K);
		flagsVisible.insert("Black", cp);
		for (int sp = 0; sp < spots.count(); ++sp)
		{
			Table->setItem(sp+4, 1, new QTableWidgetItem(spots[sp]));
			cp = new QCheckBox(this);
			cp->setFocusPolicy(Qt::NoFocus);
			connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
			Table->setCellWidget(sp+4, 0, cp);
			cp->setChecked(true);
			flagsVisible.insert(spots[sp], cp);
		}
		Layout2->addWidget(Table);
		tbWidth = Table->columnWidth(1);
	}
	else
	{
		EnableCMYK_C = new QCheckBox(devTitle);
		EnableCMYK_C->setText( tr("&C"));
		EnableCMYK_C->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_C : true);
		EnableCMYK_C->setEnabled(postscriptPreview);
		Layout2->addWidget(EnableCMYK_C);
		EnableCMYK_M = new QCheckBox(devTitle);
		EnableCMYK_M->setText( tr("&M"));
		EnableCMYK_M->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_M : true);
		EnableCMYK_M->setEnabled(postscriptPreview);
		Layout2->addWidget(EnableCMYK_M);
		EnableCMYK_Y = new QCheckBox(devTitle);
		EnableCMYK_Y->setText( tr("&Y"));
		EnableCMYK_Y->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_Y : true);
		EnableCMYK_Y->setEnabled(postscriptPreview);
		Layout2->addWidget(EnableCMYK_Y);
		EnableCMYK_K = new QCheckBox(devTitle);
		EnableCMYK_K->setText( tr("&K"));
		EnableCMYK_K->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_K : true);
		EnableCMYK_K->setEnabled(postscriptPreview);
		Layout2->addWidget(EnableCMYK_K);
	}
	
	settingsBarLayout->addWidget(devTitle);
	jobTitle = new QGroupBox( this );
	jobTitle->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	jobTitle->setTitle( tr( "Print Settings" ) );
	Layout1 = new QVBoxLayout( jobTitle );
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 5 );
	Layout1->setAlignment( Qt::AlignTop );

	MirrorHor = new QCheckBox( tr( "Mirror Page(s) Horizontal" ), jobTitle );
	Layout1->addWidget( MirrorHor );

	MirrorVert = new QCheckBox( tr( "Mirror Page(s) Vertical" ), jobTitle );
	Layout1->addWidget( MirrorVert );

	ClipMarg = new QCheckBox( tr( "Clip to Page Margins" ), jobTitle );
	Layout1->addWidget( ClipMarg );

	useGray = new QCheckBox( tr("Print in Grayscale"), jobTitle);
	Layout1->addWidget(useGray);

	EnableGCR = new QCheckBox( tr("&Under Color Removal"), jobTitle);
	Layout1->addWidget(EnableGCR);

	EnableOverprint = new QCheckBox( tr("Force Overprint Mode"), jobTitle);
	Layout1->addWidget(EnableOverprint);

	spotColors = new QCheckBox( tr( "Convert Spot Colors" ), jobTitle );
	Layout1->addWidget( spotColors );

	UseICC = new QCheckBox( tr( "Apply Color Profiles" ), jobTitle );
	Layout1->addWidget( UseICC );
	if (!doc->HasCMS)
		UseICC->setEnabled(false);

	settingsBarLayout->addWidget(jobTitle);
	QSpacerItem* spacerC = new QSpacerItem( 5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding );
	settingsBarLayout->addItem( spacerC );
	Layout5->addLayout(settingsBarLayout);
	PLayout->addLayout(Layout5);

	Layout6 = new QHBoxLayout;
	Layout6->setSpacing(0);
	Layout6->setMargin(0);
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
	PGSel = new PageSelector(this, doc->DocPages.count());
	PGSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	Layout6->addWidget(PGSel);
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
	Anz = new QLabel(this);
	Anz->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//	APage = doc->currentPage()->pageNr();
	Anz->setPixmap(CreatePreview(doc->currentPage()->pageNr(), 72));
	Anz->resize(Anz->pixmap()->size());
	Anzeige->setWidget(Anz);
	int w = Anz->width() + tbWidth + 50;
	resize(qMin(QApplication::desktop()->width()-30,w), 500);
	if (!PrefsManager::instance()->appPrefs.PrPr_Mode)
	{
		if (HaveTiffSep)
			Table->setEnabled(false);
		else
		{
			EnableCMYK_C->setEnabled(false);
			EnableCMYK_M->setEnabled(false);
			EnableCMYK_Y->setEnabled(false);
			EnableCMYK_K->setEnabled(false);
		}
	}
	PGSel->GotoPg(doc->currentPage()->pageNr());
	// tooltips
	AntiAlias->setToolTip( "<qt>" + tr( "Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing" ) + "</qt>" );
	AliasTr->setToolTip( "<qt>" + tr( "Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later" ) + "</qt>");
	EnableCMYK->setToolTip( "<qt>" + tr( "Gives a print preview using simulations of generic CMYK inks, instead of RGB colors" ) + "</qt>");
	EnableGCR->setToolTip( "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>" );
	scaleBox->setToolTip( "<qt>" + tr("Resize the scale of the page.") + "</qt>");
	spotColors->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
	EnableOverprint->setToolTip( "<qt>"+ tr("Enables global Overprint Mode for this document, overrides object settings") + "<qt>");
	UseICC->setToolTip("<qt>" + tr( "Allows you to embed color profiles in the print stream when color management is enabled" ) + "</qt>");

	//signals and slots
	connect(AntiAlias, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(AliasTr, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(EnableCMYK, SIGNAL(clicked()), this, SLOT(ToggleCMYK()));
	connect(EnableGCR, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(EnableOverprint, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(MirrorHor, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(MirrorVert, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(ClipMarg, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(spotColors, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(useGray, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(UseICC, SIGNAL(clicked()), this, SLOT(redisplay()));
	if (!HaveTiffSep)
	{
		connect(EnableCMYK_C, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		connect(EnableCMYK_M, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		connect(EnableCMYK_Y, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		connect(EnableCMYK_K, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		EnableCMYK_C->setToolTip( tr( "Enable/disable the C (Cyan) ink plate" ) );
		EnableCMYK_M->setToolTip( tr( "Enable/disable the M (Magenta) ink plate" ) );
		EnableCMYK_Y->setToolTip( tr( "Enable/disable the Y (Yellow) ink plate" ) );
		EnableCMYK_K->setToolTip( tr( "Enable/disable the K (Black) ink plate" ) );
	}
	connect(PGSel, SIGNAL(GotoPage(int)), this, SLOT(ToSeite(int)));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(printButton, SIGNAL(clicked()), this, SIGNAL(doPrint()));
	connect(scaleBox, SIGNAL(activated(int)), this, SLOT(scaleBox_valueChanged(int)));
}

void PPreview::setValues()
{
	if ((printDinUse) || (!doc->Print_Options.firstUse))
	{
		EnableGCR->setChecked( postscriptPreview ? doc->Print_Options.doGCR : false);
		EnableGCR->setEnabled( postscriptPreview );
		EnableOverprint->setChecked( postscriptPreview ? doc->Print_Options.doOverprint : false);
		EnableOverprint->setEnabled( postscriptPreview );
		MirrorHor->setChecked(doc->Print_Options.mirrorH);
		MirrorVert->setChecked(doc->Print_Options.mirrorV);
		ClipMarg->setChecked(doc->Print_Options.doClip);
		spotColors->setChecked(!doc->Print_Options.useSpotColors);
		useGray->setChecked(!doc->Print_Options.useColor);
		if (doc->HasCMS)
		{
			UseICC->setChecked( postscriptPreview ? doc->Print_Options.useICC : false);
			UseICC->setEnabled( postscriptPreview );
		}
	}
	else
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
		EnableGCR->setChecked( postscriptPreview ? prefs->getBool("DoGCR", false) : false);
		EnableGCR->setEnabled( postscriptPreview );
		EnableOverprint->setChecked( postscriptPreview ? prefs->getBool("doOverprint", false) : false);
		EnableOverprint->setEnabled( postscriptPreview );
		MirrorHor->setChecked(prefs->getBool("MirrorH", false));
		MirrorVert->setChecked(prefs->getBool("MirrorV", false));
		ClipMarg->setChecked(prefs->getBool("Clip", false));
		spotColors->setChecked(!prefs->getBool("doSpot", true));
		if (prefs->getInt("PrintColor", 0) == 1)
			useGray->setChecked(true);
		else
			useGray->setChecked(false);
		if (doc->HasCMS)
		{
			UseICC->setChecked( postscriptPreview ? prefs->getBool("ICCinUse", false) : false);
			UseICC->setEnabled( postscriptPreview );
		}
	}
}

void PPreview::ToSeite(int num)
{
	int n = num-1;
	if (n == APage)
		return;
	Anz->setPixmap(CreatePreview(n, qRound(72 * scaleFactor)));
	Anz->resize(Anz->pixmap()->size());
}

void PPreview::redisplay()
{
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
	Anz->resize(Anz->pixmap()->size());
}

void PPreview::ToggleCMYK()
{
	bool c = EnableCMYK->isChecked() ? true : false;
	if (HaveTiffSep)
		Table->setEnabled(c);
	else
	{
		EnableCMYK_C->setEnabled(c);
		EnableCMYK_M->setEnabled(c);
		EnableCMYK_Y->setEnabled(c);
		EnableCMYK_K->setEnabled(c);
	}
		
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
	Anz->resize(Anz->pixmap()->size());
}

void PPreview::ToggleCMYK_Colour()
{
	if (EnableCMYK->isChecked())
		Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
	Anz->resize(Anz->pixmap()->size());
}

void PPreview::scaleBox_valueChanged(int value)
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
			scaleFactor = Anzeige->viewport()->width() / doc->Pages->at(APage)->width();
			break;
		case 5:
			scaleFactor = Anzeige->viewport()->height() / doc->Pages->at(APage)->height();
			break;
		case 6:
			scaleFactor = qMin(Anzeige->viewport()->height() / doc->Pages->at(APage)->height(), Anzeige->viewport()->width() / doc->Pages->at(APage)->width());
			break;
		default:
			scaleFactor = 1.0;
			break;
	}
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
	Anz->resize(Anz->pixmap()->size());
}

int PPreview::RenderPreview(int Seite, int Res)
{
	int ret = -1;
	QString cmd1, cmd2, cmd3;
	QMap<QString,QMap<uint, FPointArray> > ReallyUsed;
#if defined _WIN32
	if ( !postscriptPreview )
	{
		QImage image;
		Page* page;
		ScPrintEngine_GDI winPrint;
		PrintOptions options;
		page = doc->Pages->at( Seite );
		options.copies = 1;
		options.doGCR = false;
		//options.mirrorH = options.mirrorV = false;
		options.mirrorH = MirrorHor->isChecked();
		options.mirrorV = MirrorVert->isChecked();
		options.outputSeparations = false;
		options.pageNumbers.push_back( Seite );
		options.prnEngine = WindowsGDI;
		options.separationName = "All";
		options.toFile = false;
		options.useColor = !useGray->isChecked();
		options.useICC = false;
		options.useSpotColors = false;
		options.doOverprint = false;
		bool done = winPrint.gdiPrintPreview(doc, page, &image, options, Res / 72.0);
		if ( done )
			image.save( ScPaths::getTempFileDir() + "/sc.png", "PNG" );
		return (done ? 0 : 1);
	}
#endif
	// Recreate Postscript-File only when the actual Page has changed
	if ((Seite != APage)  || (EnableGCR->isChecked() != GMode)  || (EnableOverprint->isChecked() != OMode)
		|| (useGray->isChecked() != fGray) || (MirrorHor->isChecked() != mHor) || (MirrorVert->isChecked() != mVer)
		|| (ClipMarg->isChecked() != fClip) || (UseICC->isChecked() != fICC) || (spotColors->isChecked() != fSpot))
	{
		ReallyUsed.clear();
		doc->getUsedFonts(ReallyUsed);
		bool useIC = UseICC->isChecked();
		if (!doc->HasCMS)
			useIC = false;
		PrintOptions options;
		options.pageNumbers.push_back(Seite+1);
		options.outputSeparations = false;
		options.separationName = tr("All");
		options.allSeparations = QStringList();
		options.useColor = !useGray->isChecked();
		options.mirrorH = MirrorHor->isChecked();
		options.mirrorV = MirrorVert->isChecked();
		options.useICC = useIC;
		options.doGCR = EnableGCR->isChecked();
		options.setDevParam = false;
		options.doClip = ClipMarg->isChecked();
		options.doOverprint = EnableOverprint->isChecked();
		options.cropMarks = false;
		options.bleedMarks = false;
		options.registrationMarks = false;
		options.colorMarks = false;
		options.markOffset = 0.0;
		options.bleeds.Top = 0.0;
		options.bleeds.Left = 0.0;
		options.bleeds.Right = 0.0;
		options.bleeds.Bottom = 0.0;
		PSLib *dd = new PSLib(options, true, prefsManager->appPrefs.AvailFonts, ReallyUsed, doc->PageColors, false, !spotColors->isChecked());
		if (dd != NULL)
		{
			dd->PS_set_file( ScPaths::getTempFileDir() + "/tmp.ps");
			ret = dd->CreatePS(doc, options);
			delete dd;
			if (ret != 0) return 1;
		}
		else
			return ret;
	}
	QStringList args;
	QString tmp, tmp2, tmp3;
	double b = doc->Pages->at(Seite)->width() * Res / 72.0;
	double h = doc->Pages->at(Seite)->height() * Res / 72.0;
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( QString("-r%1").arg(tmp.setNum(qRound(Res))) );
	args.append( QString("-g%1x%2").arg(tmp2.setNum(qRound(b))).arg(tmp3.setNum(qRound(h))) );
	if (EnableCMYK->isChecked())
	{
		if (HaveTiffSep)
			args.append( "-sDEVICE=tiffsep" );
		else
		{
			args.append( "-sDEVICE=bitcmyk" );
			args.append( "-dGrayValues=256" ); 
		}
	}
	else
	{
		if ((!AliasTr->isChecked()) || (!HavePngAlpha))
			args.append( "-sDEVICE=png16m" );
		else
			args.append( "-sDEVICE=pngalpha" );
	}
	if (AntiAlias->isChecked())
	{
		args.append( "-dTextAlphaBits=4" );
		args.append( "-dGraphicsAlphaBits=4" );
	}
	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = prefsManager->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd1 = QString("-sFONTPATH=%1").arg(QDir::convertSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd1 += QString("%1%2").arg(sep).arg(QDir::convertSeparators(extraFonts->get(i,0)));
	if( !cmd1.isEmpty() )
		args.append( cmd1 );
	// then add any final args and call gs
	if ((EnableCMYK->isChecked()) && HaveTiffSep)
		args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(ScPaths::getTempFileDir()+"/sc.tif")) );
	else
		args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(ScPaths::getTempFileDir()+"/sc.png")) );
	args.append( QDir::convertSeparators(ScPaths::getTempFileDir()+"/tmp.ps") );
	args.append( "-c" );
	args.append( "showpage" );
	args.append( "-c" );
	args.append( "quit" );
	ret = System(getShortPathName(prefsManager->ghostscriptExecutable()), args);
	return ret;
}

int PPreview::RenderPreviewSep(int Seite, int Res)
{
	int ret = -1;
	QString cmd;
	QStringList args, args1, args2, args3;
	QMap<QString, QMap<uint, FPointArray> > ReallyUsed;
	// Recreate Postscript-File only when the actual Page has changed
	if ((Seite != APage)  || (EnableGCR->isChecked() != GMode)  || (EnableOverprint->isChecked() != OMode)
		|| (useGray->isChecked() != fGray) || (MirrorHor->isChecked() != mHor) || (MirrorVert->isChecked() != mVer)
		|| (ClipMarg->isChecked() != fClip) || (UseICC->isChecked() != fICC) || (spotColors->isChecked() != fSpot))
	{
		ReallyUsed.clear();
		doc->getUsedFonts(ReallyUsed);
		bool useIC = UseICC->isChecked();
		if (!doc->HasCMS)
			useIC = false;
		PrintOptions options;
		options.pageNumbers.push_back(Seite+1);
		options.outputSeparations = false;
		options.separationName = tr("All");
		options.allSeparations = QStringList();
		options.useColor = !useGray->isChecked();
		options.mirrorH = MirrorHor->isChecked();
		options.mirrorV = MirrorVert->isChecked();
		options.useICC = useIC;
		options.doGCR = EnableGCR->isChecked();
		options.setDevParam = false;
		options.doClip = ClipMarg->isChecked();
		options.doOverprint = EnableOverprint->isChecked();
		options.cropMarks = false;
		options.bleedMarks = false;
		options.registrationMarks = false;
		options.colorMarks = false;
		options.markOffset = 0.0;
		options.bleeds.Top = 0.0;
		options.bleeds.Left = 0.0;
		options.bleeds.Right = 0.0;
		options.bleeds.Bottom = 0.0;
		PSLib *dd = new PSLib(options, true, prefsManager->appPrefs.AvailFonts, ReallyUsed, doc->PageColors, false, !spotColors->isChecked());
		if (dd != NULL)
		{
			dd->PS_set_file(ScPaths::getTempFileDir()+"/tmp.ps");
			ret = dd->CreatePS(doc, options);
			delete dd;
			if (ret != 0) return 1;
		}
		else
			return ret;
	}
	QString tmp, tmp2, tmp3;
	double b = doc->Pages->at(Seite)->width() * Res / 72.0;
	double h = doc->Pages->at(Seite)->height() * Res / 72.0;

	args1.append( "-q" );
	args1.append( "-dNOPAUSE" );
	args1.append( "-dPARANOIDSAFER" );
	args1.append( QString("-r%1").arg(tmp.setNum(qRound(Res))) );
	args1.append( QString("-g%1x%2").arg(tmp2.setNum(qRound(b))).arg(tmp3.setNum(qRound(h))) ); 
	if (AntiAlias->isChecked())
	{
		args1.append("-dTextAlphaBits=4");
		args1.append("-dGraphicsAlphaBits=4");
	}
	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = prefsManager->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(QDir::convertSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(QDir::convertSeparators(extraFonts->get(i,0)));
	if( !cmd.isEmpty() )
		args1.append( cmd );
	args1.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(ScPaths::getTempFileDir()+"/sc.tif")) );

	args2.append( QDir::convertSeparators(ScPaths::getTempFileDir()+"/tmp.ps") );
	args2.append("-c");
	args2.append("quit");

	ColorList usedSpots;
	doc->getUsedColors(usedSpots, true);
	QStringList spots = usedSpots.keys();
	args3.append( "-sDEVICE=tiffsep" );

	args3.append( "-c" );
	cmd = "<< /SeparationColorNames ";
	QString allSeps ="[ /Cyan /Magenta /Yellow /Black ";
	for (int sp = 0; sp < spots.count(); ++sp)
	{
		allSeps += "("+spots[sp]+") ";
	}
	allSeps += "]";
	cmd += allSeps + " /SeparationOrder [ /Cyan /Magenta /Yellow /Black] >> setpagedevice";
	args3.append(cmd);

	args3.append("-f");
	QString gsExe(getShortPathName(prefsManager->ghostscriptExecutable()));
	ret = System(gsExe, args1 + args3 + args2, ScPaths::getTempFileDir()+"/sc.tif.txt" );

	QFile sepInfo(QDir::convertSeparators(ScPaths::getTempFileDir()+"/sc.tif.txt"));
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
			args3.append("-c");
			args3.append("<< /SeparationColorNames "+allSeps+" /SeparationOrder [ "+currSeps+" ] >> setpagedevice");
			args3.append("-f");
			ret = System(gsExe, args1 + args3 + args2);
			currSeps = "";
			spc = 0;
		}
	}
	if (spc != 0)
	{
		args3.clear();
		args3.append("-sDEVICE=tiffsep");
		args3.append("-c");
		args3.append("<< /SeparationColorNames "+allSeps+" /SeparationOrder [ "+currSeps+" ] >> setpagedevice");
		args3.append("-f");
		ret = System(gsExe, args1 + args3 + args2);
	}
	return ret;
}

// this should move to scimage.cpp!
void PPreview::blendImages(QImage &target, ScImage &scsource, ScColor col)
{
	QImage source = scsource.qImage(); // FIXME: this will not work once qImage always returns ARGB!
	
	//FIXME: if source and target have different sizesomething went wrong.
	// eg. loadPicture() failed and returned a 1x1 image
	CMYKColor cmykValues;
	int h = qMin(target.height(),source.height());
	int w = qMin(target.width(),source.width());
	int cyan, c, m, yc, k, cc, mm, yy, kk;
	ScColorEngine::getCMYKValues(col, doc, cmykValues);
	cmykValues.getValues(c, m, yc, k);
	for (int y=0; y < h; ++y )
	{
		QRgb *p = (QRgb *)target.scanLine( y );
		QRgb *pq = (QRgb *)source.scanLine( y );
		for (int x=0; x < w; ++x )
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

QPixmap PPreview::CreatePreview(int Seite, int Res)
{
	int ret = -1;
	QPixmap Bild;
	double b = doc->Pages->at(Seite)->width() * Res / 72.0;
	double h = doc->Pages->at(Seite)->height() * Res / 72.0;
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	if ((Seite != APage) || (EnableCMYK->isChecked() != CMode) || (SMode != scaleBox->currentIndex())
	        || (AntiAlias->isChecked() != GsAl) || ((AliasTr->isChecked() != Trans) || (EnableGCR->isChecked() != GMode)
			|| (EnableOverprint->isChecked() != OMode) && (!EnableCMYK->isChecked()))
			 || (useGray->isChecked() != fGray) || (MirrorHor->isChecked() != mHor) || (MirrorVert->isChecked() != mVer)
			 || (ClipMarg->isChecked() != fClip) || (UseICC->isChecked() != fICC) || (spotColors->isChecked() != fSpot))
	{
		if (!EnableCMYK->isChecked() || (!HaveTiffSep))
		{
			ret = RenderPreview(Seite, Res);
			if (ret > 0)
			{
				imageLoadError(Bild, Seite);
				return Bild;
			}
		}
	}
	QImage image;
	if (EnableCMYK->isChecked())
	{
		int cyan, magenta, yellow, black, alpha;
		uint *p;
		bool loaderror;
		if (HaveTiffSep)
		{
			if ((Seite != APage) || (EnableCMYK->isChecked() != CMode) || (SMode != scaleBox->currentIndex())
	       	 || (AntiAlias->isChecked() != GsAl) || (AliasTr->isChecked() != Trans) || (EnableGCR->isChecked() != GMode)
	       	 || (EnableOverprint->isChecked() != OMode) || (useGray->isChecked() != fGray) || (MirrorHor->isChecked() != mHor)
	       	 || (MirrorVert->isChecked() != mVer) || (ClipMarg->isChecked() != fClip) || (UseICC->isChecked() != fICC) || (spotColors->isChecked() != fSpot))
			{
				ret = RenderPreviewSep(Seite, Res);
				if (ret > 0)
				{
					imageLoadError(Bild, Seite);
					return Bild;
				}
			}
			ScImage im;
			bool mode;
			int w = qRound(b);
			int h2 = qRound(h);
			image = QImage(w, h2, QImage::Format_ARGB32);
			QRgb clean = qRgba(0, 0, 0, 0);
			for( int yi=0; yi < h2; ++yi )
			{
				QRgb *q = (QRgb*)(image.scanLine( yi ));
				for(int xi=0; xi < w; ++xi )
				{
					*q = clean;
					q++;
				}
			}
			CMSettings cms(doc, "", 0);
			if (flagsVisible["Cyan"]->isChecked())
			{
				if (GsMinor < 54)
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.tif.Cyan.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.Cyan.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(Bild, Seite);
					return Bild;
				}
				blendImages(image, im, ScColor(255, 0, 0, 0));
			}
			if (flagsVisible["Magenta"]->isChecked())
			{
				if (GsMinor < 54)
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.tif.Magenta.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.Magenta.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(Bild, Seite);
					return Bild;
				}
				blendImages(image, im, ScColor(0, 255, 0, 0));
			}
			if (flagsVisible["Yellow"]->isChecked())
			{
				if (GsMinor < 54)
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.tif.Yellow.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.Yellow.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(Bild, Seite);
					return Bild;
				}
				blendImages(image, im, ScColor(0, 0, 255, 0));
			}
			if (!sepsToFileNum.isEmpty())
			{
				QMap<QString, int>::Iterator sepit;
				for (sepit = sepsToFileNum.begin(); sepit != sepsToFileNum.end(); ++sepit)
				{
					if (flagsVisible[sepit.key()]->isChecked())
					{
						QString fnam;
						if (GsMinor < 54)
							fnam = QString(ScPaths::getTempFileDir()+"/sc.tif.s%1.tif").arg(sepit.value());
						else
							fnam = QString(ScPaths::getTempFileDir()+"/sc.s%1.tif").arg(sepit.value());
						if (!im.LoadPicture(fnam, cms, false, false, ScImage::RGBData, 72, &mode))
						{
							imageLoadError(Bild, Seite);
							return Bild;
						}
						blendImages(image, im, doc->PageColors[sepit.key()]);
					}
				}
			}
			if (flagsVisible["Black"]->isChecked())
			{
				CMSettings cms(doc, "", 0);
				if (GsMinor < 54)
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.tif.Black.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				else
					loaderror = im.LoadPicture(ScPaths::getTempFileDir()+"/sc.Black.tif", cms, false, false, ScImage::RGBData, 72, &mode);
				if (!loaderror)
				{
					imageLoadError(Bild, Seite);
					return Bild;
				}
				blendImages(image, im, ScColor(0, 0, 0, 255));
			}
			if (doc->HasCMS)
			{
				QRgb alphaFF = qRgba(0,0,0,255);
				QRgb alphaOO = qRgba(255,255,255,0);
				cmsHTRANSFORM transCMYK = cmsCreateTransform(doc->DocPrinterProf, (COLORSPACE_SH(PT_CMYK)|CHANNELS_SH(4)|BYTES_SH(1)|DOSWAP_SH(1)|SWAPFIRST_SH(1)), doc->DocOutputProf, TYPE_BGRA_8, INTENT_RELATIVE_COLORIMETRIC, cmsFLAGS_LOWRESPRECALC);
				for( int yi=0; yi < h2; ++yi )
				{
					LPBYTE ptr = image.scanLine( yi );
					cmsDoTransform(transCMYK, ptr, ptr, image.width());
					QRgb *q = (QRgb *) ptr;
					for (int xi = 0; xi < image.width(); xi++, q++)
					{
						if (AliasTr->isChecked())
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
				cmsDeleteTransform (transCMYK);
			}
			else
			{
				for( int yi=0; yi < h2; ++yi )
				{
					QRgb *q = (QRgb*)(image.scanLine( yi ));
					for(int xi=0; xi < w; ++xi )
					{
						cyan = qRed(*q);
						magenta = qGreen(*q);
						yellow = qBlue(*q);
						black = qAlpha(*q);
						if ((cyan != 0) || (magenta != 0) || (yellow != 0 ) || (black != 0))
							*q = qRgba(255-qMin(255, cyan+black), 255-qMin(255,magenta+black), 255-qMin(255,yellow+black), 255);
						else
						{
							if (!AliasTr->isChecked())
								*q = qRgba(255, 255, 255, 255);
						}
						q++;
					}
				}
			}
		}
		else
		{
			int w = qRound(b);
			int w2 = 4*w;
			int h2 = qRound(h);
			image = QImage(w, h2, QImage::Format_ARGB32);
			QByteArray imgc(w2, ' ');
			QFile f(ScPaths::getTempFileDir()+"/sc.png");
			if (f.open(QIODevice::ReadOnly))
			{
				if (doc->HasCMS)
				{
					QRgb alphaFF = qRgba(0,0,0,255);
					QRgb alphaOO = qRgba(255,255,255,0);
					cmsHTRANSFORM transCMYK = cmsCreateTransform(doc->DocPrinterProf, (COLORSPACE_SH(PT_CMYK)|CHANNELS_SH(4)|BYTES_SH(1)|DOSWAP_SH(1)|SWAPFIRST_SH(1)), doc->DocOutputProf, TYPE_BGRA_8, INTENT_RELATIVE_COLORIMETRIC, cmsFLAGS_LOWRESPRECALC);
					for (int y=0; y < h2; ++y )
					{
						LPBYTE ptr = image.scanLine( y );
						f.read(imgc.data(), w2);
						p = (uint *)image.scanLine( y );
						for (int x=0; x < w2; x += 4 )
						{
							cyan = uchar(imgc[x]);
							magenta = uchar(imgc[x + 1]);
							yellow = uchar(imgc[x + 2]);
							black = uchar(imgc[x + 3]);
							if (!EnableCMYK_C->isChecked())
								cyan = 0;
							if (!EnableCMYK_M->isChecked())
								magenta = 0;
							if (!EnableCMYK_Y->isChecked())
								yellow = 0;
							if (!EnableCMYK_K->isChecked())
								black = 0;
							*p = qRgba(cyan, magenta, yellow, black);
							p++;
						}
						cmsDoTransform(transCMYK, ptr, ptr, image.width());
						QRgb *q = (QRgb *) ptr;
						for (int xi = 0; xi < image.width(); xi++, q++)
						{
							if (AliasTr->isChecked())
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
					cmsDeleteTransform (transCMYK);
				}
				else
				{
					for (int y=0; y < h2; ++y )
					{
						p = (uint *)image.scanLine( y );
						f.read(imgc.data(), w2);
						for (int x=0; x < w2; x += 4 )
						{
							cyan = uchar(imgc[x]);
							magenta = uchar(imgc[x + 1]);
							yellow = uchar(imgc[x + 2]);
							black = uchar(imgc[x + 3]);
							if (!EnableCMYK_C->isChecked())
								cyan = 0;
							if (!EnableCMYK_M->isChecked())
								magenta = 0;
							if (!EnableCMYK_Y->isChecked())
								yellow = 0;
							if (!EnableCMYK_K->isChecked())
								black = 0;
							if (AliasTr->isChecked() && ((cyan == 0) && (magenta == 0) && (yellow == 0 ) && (black == 0)))
								alpha = 0;
							else
								alpha = 255;
							*p = qRgba(255-qMin(255, cyan+black), 255-qMin(255,magenta+black), 255-qMin(255,yellow+black), alpha);
							p++;
						}
					}
				}
				f.close();
			}
			else
			{
				imageLoadError(Bild, Seite);
				return Bild;
			}
		}
	}
	else
	{
		if (!image.load(ScPaths::getTempFileDir()+"/sc.png"))
		{
			imageLoadError(Bild, Seite);
			return Bild;
		}
		image = image.convertToFormat(QImage::Format_ARGB32);
		if ((AliasTr->isChecked()) && (HavePngAlpha))
		{
			int wi = image.width();
			int hi = image.height();
			for( int yi=0; yi < hi; ++yi )
			{
				QRgb *s = (QRgb*)(image.scanLine( yi ));
				for(int xi=0; xi < wi; ++xi )
				{
					if((*s) == 0xffffffff)
						(*s) &= 0x00ffffff;
					s++;
				}
			}
		}
	}
	if (AliasTr->isChecked())
	{
		Bild = QPixmap(image.width(), image.height());
		QPainter p;
		QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
		p.begin(&Bild);
		p.fillRect(0, 0, image.width(), image.height(), b);
		p.drawImage(0, 0, image);
		p.end();
	}
	else
		Bild = QPixmap::fromImage(image);
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	getUserSelection(Seite);
	return Bild;
}

//-------------------------------------------------------------------------------------------------

bool PPreview::usePostscriptPreview(QString printerName, PrintEngine engine)
{
#ifdef _WIN32
	if ( printerName == tr("File") )
		return true;
	else if( printerName.isEmpty() )
		return PrinterUtil::isPostscriptPrinter( ScPrintEngine_GDI::getDefaultPrinter() );
	else if( engine >= PostScript1 && engine <= PostScript3 )
		return PrinterUtil::isPostscriptPrinter( printerName );
	return false;
#else
	return true;
#endif
}

//-------------------------------------------------------------------------------------------------

void PPreview::getUserSelection(int page)
{
	APage = page;
	CMode = EnableCMYK->isChecked();
	GsAl = AntiAlias->isChecked();
	Trans = AliasTr->isChecked();
	GMode = EnableGCR->isChecked();
	SMode = scaleBox->currentIndex();
	OMode = EnableOverprint->isChecked();
	mHor = MirrorHor->isChecked();
	mVer = MirrorVert->isChecked();
	fClip = ClipMarg->isChecked();
	fSpot = spotColors->isChecked();
	fGray = useGray->isChecked();
	fICC = UseICC->isChecked();
}

void PPreview::imageLoadError(QPixmap &Bild, int page)
{
	Bild = QPixmap(1,1);
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	getUserSelection(page);
}

void PPreview::resizeEvent(QResizeEvent * event)
{
	QDialog::resizeEvent(event);
	int cx = scaleBox->currentIndex();
	// repaint only for "fit to" options in the combo box
	if (cx > 3)
	{
		// HACK: SMode is reset to insane value to force redraw
		// as the value is checked for change.
		SMode = -1;
		scaleBox_valueChanged(cx);
	}
}
