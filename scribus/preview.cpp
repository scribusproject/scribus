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
#include "preview.moc"
#include <qimage.h>
#include <cstdlib>
#include <qcursor.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qtooltip.h>
#include <qfile.h>
#include <qspinbox.h>
#include <qtable.h>
#include "pslib.h"
#include "checkDocument.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "util.h"
#include "sccolor.h"
#include "scribusview.h"
#include "scribusdoc.h"
#include "scpaths.h"
#include "pageselector.h"
#include "printerutil.h"

#if defined(_WIN32)
#include "scwinprint.h"
#endif

extern bool printDinUse;

/*!
 \fn PPreview::PPreview( QWidget* parent, ScribusMainWindow *pl)
 \author Franz Schmid
 \date
 \brief Create the Print Preview window
 \param parent QWidget *
 \param vin ScribusView *
 \param docu ScribusDoc *
 \param pngAlpha int
 \retval PPreview window
 */
PPreview::PPreview( QWidget* parent, ScribusView *vin, ScribusDoc *docu, int pngAlpha, int tiffSep, QString printer ) : QDialog( parent, "Preview", true, 0 )
{
	Q_ASSERT(!docu->masterPageMode());
	prefsManager=PrefsManager::instance();
	QString tmp;
	postscriptPreview = usePostscriptPreview(printer);
	QString caption = tr("Print Preview");
#ifdef _WIN32
	if (postscriptPreview)
		caption += " (Postscript)";
	else
		caption += " (GDI)";
#endif
	setCaption( caption );
	doc = docu;
	view = vin;
	HavePngAlpha = pngAlpha;
	HaveTiffSep = postscriptPreview ? tiffSep : 1;
	APage = -1;
	CMode = false;
	TxtAl = false;
	GrAl = false;
	Trans = false;
	GMode = true;
	scaleFactor = 1.0;
	SMode = 1;

	setIcon(loadIcon("AppIcon.png"));
	PLayout = new QVBoxLayout(this, 0, 0, "PLayout");

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing(5);
	Layout1->setMargin(5);
	PGSel = new PageSelector(this, doc->DocPages.count());
	PGSel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, PGSel->sizePolicy().hasHeightForWidth() ) );
	Layout1->addWidget(PGSel);

	Layout2 = new QVBoxLayout();
	Layout2->setSpacing(0);
	Layout2->setMargin(0);
	AliasText = new QCheckBox(this, "TextAntiAlias");
	AliasText->setText( tr("Anti-alias &Text"));
	AliasText->setChecked( postscriptPreview ? prefsManager->appPrefs.PrPr_AlphaText : false);
	AliasText->setEnabled( postscriptPreview );
	Layout2->addWidget(AliasText);
	AliasGr = new QCheckBox(this, "GraphicsAntiAlias");
	AliasGr->setText( tr("Anti-alias &Graphics"));
	AliasGr->setChecked( postscriptPreview ? prefsManager->appPrefs.PrPr_AlphaGraphics : false);
	AliasGr->setEnabled( postscriptPreview );
	Layout2->addWidget(AliasGr);
	Layout1->addLayout(Layout2);

	Layout3 = new QVBoxLayout();
	Layout3->setSpacing(0);
	Layout3->setMargin(0);
	AliasTr = new QCheckBox(this, "DisplayTransparency");
	AliasTr->setText( tr("Display Trans&parency"));
	AliasTr->setChecked(prefsManager->appPrefs.PrPr_Transparency);
	AliasTr->setEnabled( postscriptPreview );
	Layout3->addWidget(AliasTr);
	EnableGCR = new QCheckBox(this, "DisplayGCR");
	EnableGCR->setText( tr("&Under Color Removal"));
	EnableGCR->setChecked( postscriptPreview ? prefsManager->appPrefs.Gcr_Mode : false);
	EnableGCR->setEnabled( postscriptPreview );
	Layout3->addWidget(EnableGCR);
	Layout1->addLayout(Layout3);

	QSpacerItem* spacerC = new QSpacerItem( 5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacerC );

	Layout4 = new QVBoxLayout();
	Layout4->setSpacing(0);
	Layout4->setMargin(0);
	EnableCMYK = new QCheckBox(this, "DisplayCMYK");
	EnableCMYK->setText( tr("&Display CMYK"));
	EnableCMYK->setChecked( postscriptPreview ? prefsManager->appPrefs.PrPr_Mode : false);
	EnableCMYK->setEnabled( postscriptPreview );
	Layout4->addWidget(EnableCMYK);
	if (HaveTiffSep != 0)
	{
		Layout5 = new QHBoxLayout();
		Layout5->setSpacing(0);
		Layout5->setMargin(0);
		EnableCMYK_C = new QCheckBox(this, "DisplayCMYK_C");
		EnableCMYK_C->setText( tr("&C"));
		EnableCMYK_C->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_C : true);
		EnableCMYK_C->setEnabled(postscriptPreview);
		Layout5->addWidget(EnableCMYK_C);
		EnableCMYK_M = new QCheckBox(this, "DisplayCMYK_M");
		EnableCMYK_M->setText( tr("&M"));
		EnableCMYK_M->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_M : true);
		EnableCMYK_M->setEnabled(postscriptPreview);
		Layout5->addWidget(EnableCMYK_M);
		EnableCMYK_Y = new QCheckBox(this, "DisplayCMYK_Y");
		EnableCMYK_Y->setText( tr("&Y"));
		EnableCMYK_Y->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_Y : true);
		EnableCMYK_Y->setEnabled(postscriptPreview);
		Layout5->addWidget(EnableCMYK_Y);
		EnableCMYK_K = new QCheckBox(this, "DisplayCMYK_K");
		EnableCMYK_K->setText( tr("&K"));
		EnableCMYK_K->setChecked(postscriptPreview ? prefsManager->appPrefs.PrPr_K : true);
		EnableCMYK_K->setEnabled(postscriptPreview);
		Layout5->addWidget(EnableCMYK_K);
		Layout4->addLayout(Layout5);
	}
	Layout1->addLayout(Layout4);
	PLayout->addLayout(Layout1);
	int tbWidth = 0;
	if (HaveTiffSep != 0)
	{
		Anzeige = new QScrollView(this);
		PLayout->addWidget(Anzeige);
	}
	else
	{
		Layout5 = new QHBoxLayout();
		Layout5->setSpacing(0);
		Layout5->setMargin(0);
		Anzeige = new QScrollView(this);
		Anzeige->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, Anzeige->sizePolicy().hasHeightForWidth() ) );
		Layout5->addWidget(Anzeige);
		ColorList usedSpots;
		doc->getUsedColors(usedSpots, true);
		QStringList spots = usedSpots.keys();
		Table = new QTable( this );
		Table->setNumRows( spots.count()+4 );
		Table->setNumCols( 2 );
		Table->horizontalHeader()->setLabel(0, loadIcon("Layervisible.xpm"), "");
		Table->horizontalHeader()->setLabel(1, tr("Separation Name"));
		Table->setColumnReadOnly(0, true);
		Table->setColumnReadOnly(1, true);
		Table->setColumnWidth(0, 24);
		Table->setRowMovingEnabled(false);
		Table->setSorting(false);
		Table->setSelectionMode( QTable::NoSelection );
		Table->setFocusStyle( QTable::FollowStyle );
		Table->setLeftMargin(0);
		Table->verticalHeader()->hide();
		flagsVisible.clear();
		QCheckBox *cp;
		Table->setText(0, 1, tr("Cyan"));
		cp = new QCheckBox(this, "");
		cp->setChecked(prefsManager->appPrefs.PrPr_C);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(0, 0, cp);
		flagsVisible.insert("Cyan", cp);
		Table->setText(1, 1, tr("Magenta"));
		cp = new QCheckBox(this, "");
		cp->setChecked(prefsManager->appPrefs.PrPr_M);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(1, 0, cp);
		flagsVisible.insert("Magenta", cp);
		Table->setText(2, 1, tr("Yellow"));
		cp = new QCheckBox(this, "");
		cp->setChecked(prefsManager->appPrefs.PrPr_Y);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(2, 0, cp);
		flagsVisible.insert("Yellow", cp);
		Table->setText(3, 1, tr("Black"));
		cp = new QCheckBox(this, "");
		cp->setChecked(prefsManager->appPrefs.PrPr_K);
		connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		Table->setCellWidget(3, 0, cp);
		flagsVisible.insert("Black", cp);
		for (uint sp = 0; sp < spots.count(); ++sp)
		{
			Table->setText(sp+4, 1, spots[sp]);
			cp = new QCheckBox(this, "");
			cp->setChecked(true);
			connect(cp, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
			Table->setCellWidget(sp+4, 0, cp);
			flagsVisible.insert(spots[sp], cp);
		}
		Table->adjustColumn(1);
		Table->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, Table->sizePolicy().hasHeightForWidth() ) );
		Layout5->addWidget(Table);
		PLayout->addLayout(Layout5);
		tbWidth = Table->columnWidth(1);
	}
	Layout6 = new QHBoxLayout();
	Layout6->setSpacing(0);
	Layout6->setMargin(0);
	/* scaling */
	scaleLabel = new QLabel(tr("Scaling:"), this, "scaleLabel");
	scaleBox = new QComboBox( true, this, "unitSwitcher" );
	scaleBox->setEditable(false);
	scaleBox->setFocusPolicy(QWidget::NoFocus);
	scaleBox->insertItem("50%");
	scaleBox->insertItem("100%");
	scaleBox->insertItem("150%");
	scaleBox->insertItem("200%");
	scaleBox->setCurrentItem(1);
	Layout6->addWidget(scaleLabel);
	Layout6->addWidget(scaleBox);
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout6->addItem( spacer );
	closeButton = new QPushButton( tr("Close"), this, "closeButton" );
	closeButton->setAutoDefault(false);
	Layout6->addWidget( closeButton );
	printButton = new QPushButton( tr("Print..."), this, "printButton" );
	printButton->setAutoDefault(false);	
	printButton->setEnabled(!printDinUse);
	Layout6->addWidget( printButton );
	PLayout->addLayout(Layout6);
	Anz = new QLabel(Anzeige->viewport());
	Anz->setPixmap(CreatePreview(0, 72));
	Anzeige->addChild(Anz, 0, 0);
	int w = Anz->width() + tbWidth + 50;
	resize(QMIN(QApplication::desktop()->width()-30,w), 500);
	if (!PrefsManager::instance()->appPrefs.PrPr_Mode)
	{
		if (HaveTiffSep != 0)
		{
			EnableCMYK_C->setEnabled(false);
			EnableCMYK_M->setEnabled(false);
			EnableCMYK_Y->setEnabled(false);
			EnableCMYK_K->setEnabled(false);
		}
		else
			Table->setEnabled(false);
	}
	// tooltips
	QToolTip::add( AliasText, "<qt>" + tr( "Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts" ) + "</qt>" );
	QToolTip::add( AliasGr, "<qt>" + tr( "Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing" ) + "</qt>" );
	QToolTip::add( AliasTr, "<qt>" + tr( "Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later" ) + "</qt>");
	QToolTip::add( EnableCMYK, "<qt>" + tr( "Gives a print preview using simulations of generic CMYK inks, instead of RGB colors" ) + "</qt>");
	QToolTip::add( EnableGCR, "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>" );
	QToolTip::add(scaleBox, "<qt>" + tr("Resize the scale of the page.") + "</qt>");
	//signals and slots
	connect(AliasText, SIGNAL(clicked()), this, SLOT(ToggleTextAA()));
	connect(AliasGr, SIGNAL(clicked()), this, SLOT(ToggleGr()));
	connect(AliasTr, SIGNAL(clicked()), this, SLOT(ToggleTr()));
	connect(EnableCMYK, SIGNAL(clicked()), this, SLOT(ToggleCMYK()));
	connect(EnableGCR, SIGNAL(clicked()), this, SLOT(ToggleGCR()));
	if (HaveTiffSep != 0)
	{
		connect(EnableCMYK_C, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		connect(EnableCMYK_M, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		connect(EnableCMYK_Y, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		connect(EnableCMYK_K, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
		QToolTip::add( EnableCMYK_C, tr( "Enable/disable the C (Cyan) ink plate" ) );
		QToolTip::add( EnableCMYK_M, tr( "Enable/disable the M (Magenta) ink plate" ) );
		QToolTip::add( EnableCMYK_Y, tr( "Enable/disable the Y (Yellow) ink plate" ) );
		QToolTip::add( EnableCMYK_K, tr( "Enable/disable the K (Black) ink plate" ) );
	}
	connect(PGSel, SIGNAL(GotoPage(int)), this, SLOT(ToSeite(int)));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(printButton, SIGNAL(clicked()), this, SIGNAL(doPrint()));
	connect(scaleBox, SIGNAL(activated(int)), this, SLOT(scaleBox_valueChanged(int)));
}

/*!
 \fn void PPreview::ToSeite(int num)
 \author Franz Schmid
 \date
 \brief Jump to newly selected page and create the new preview
 \param num int Page Number
 \retval None
 */
void PPreview::ToSeite(int num)
{
	int n = num-1;
	if (n == APage)
		return;
	Anz->setPixmap(CreatePreview(n, qRound(72 * scaleFactor)));
}

/*!
 \fn void PPreview::ToggleTextAA()
 \author Franz Schmid
 \date
 \brief Toggle Text anti-aliasing and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleTextAA()
{
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
}

/*!
 \fn void PPreview::ToggleGr()
 \author Franz Schmid
 \date
 \brief Toggle Graphics anti-aliasing and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleGr()
{
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
}

/*!
 \fn void PPreview::ToggleTr()
 \author Franz Schmid
 \date
 \brief Toggle transparency and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleTr()
{
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
}

/*!
 \fn void PPreview::ToggleCMYK()
 \author Craig Bradney
 \date
 \brief When CMYK preview is toggled, (dis)enable the CMYK controls and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleCMYK()
{
	bool c = EnableCMYK->isChecked() ? true : false;
	if (HaveTiffSep != 0)
	{
		EnableCMYK_C->setEnabled(c);
		EnableCMYK_M->setEnabled(c);
		EnableCMYK_Y->setEnabled(c);
		EnableCMYK_K->setEnabled(c);
	}
	else
		Table->setEnabled(c);
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
}

void PPreview::ToggleGCR()
{
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
}
/*!
 \fn void PPreview::ToggleCMYK_Colour()
 \author Craig Bradney
 \date
 \brief If CMYK preview is enabled, create a new preview with the new CMYK plate settings
 \param None
 \retval None
 */
void PPreview::ToggleCMYK_Colour()
{
	if (EnableCMYK->isChecked())
		Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
}

/*!
\fn void PPreview::scaleBox_valueChanged(int value)
\author Petr Vanek
\date 09/03/2005
\brief Recompute scaling factor of the preview image
\param value spinbox value from signal
 */
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
		default:
			scaleFactor = 1.0;
			break;
	}
	Anz->setPixmap(CreatePreview(APage, qRound(72 * scaleFactor)));
}

/*!
 \fn void PPreview::RenderPreview(int Seite, int Res)
 \author Franz Schmid
 \date
 \brief Renders the Preview to a file on Disk
 \param Seite int page number
 \param Res int
 \retval int Flag indicating error
 */
int PPreview::RenderPreview(int Seite, int Res)
{
	int ret = -1;
	QString cmd1, cmd2, cmd3;
	QMap<QString,int> ReallyUsed;
#if defined _WIN32
	if ( !postscriptPreview )
	{
		QImage image;
		Page* page;
		ScWinPrint winPrint;
		PrintOptions options;
		page = doc->Pages->at( Seite );
		options.copies = 1;
		options.doGCR = false;
		options.mirrorH = options.mirrorV = false;
		options.outputSeparations = false;
		options.pageNumbers.push_back( Seite );
		options.PSLevel = 0;
		options.separationName = "All";
		options.toFile = false;
		options.useColor = true;
		options.useICC = false;
		options.useSpotColors = false;
		bool done = winPrint.gdiPrintPreview(doc, page, &image, options, Res / 72.0);
		if ( done )
			image.save( prefsManager->preferencesLocation()+"/sc.png", "PNG" );
		return (done ? 0 : 1);
	}
#endif
	// Recreate Postscript-File only when the actual Page has changed
	if ((Seite != APage)  || (EnableGCR->isChecked() != GMode))
	{
		ReallyUsed.clear();
		doc->getUsedFonts(&ReallyUsed);
		PSLib *dd = new PSLib(true, prefsManager->appPrefs.AvailFonts, ReallyUsed, doc->PageColors, false, true);
		if (dd != NULL)
		{
			dd->PS_set_file(prefsManager->preferencesLocation()+"/tmp.ps");
			std::vector<int> pageNs;
			pageNs.push_back(Seite+1);
			QStringList spots;
			dd->CreatePS(doc, /*view, */pageNs, false, tr("All"), spots, true, false, false, false, EnableGCR->isChecked(), false);
			delete dd;
		}
		else
			return ret;
	}
	QStringList args;
	QString tmp, tmp2, tmp3;
	double b = doc->Pages->at(Seite)->width() * Res / 72.0;
	double h = doc->Pages->at(Seite)->height() * Res / 72.0;
	args.append( getShortPathName(prefsManager->ghostscriptExecutable()) );
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( QString("-r%1").arg(tmp.setNum(qRound(Res))) );
	args.append( QString("-g%1x%2").arg(tmp2.setNum(qRound(b))).arg(tmp3.setNum(qRound(h))) );
	if (EnableCMYK->isChecked())
	{
		if (HaveTiffSep == 0)
			args.append( "-sDEVICE=tiffsep" );
		else
		{
			args.append( "-sDEVICE=bitcmyk" );
			args.append( "-dGrayValues=256" ); 
		}
	}
	else
	{
		if ((!AliasTr->isChecked()) || (HavePngAlpha != 0))
			args.append( "-sDEVICE=png16m" );
		else
			args.append( "-sDEVICE=pngalpha" );
	}
	if (AliasText->isChecked())
		args.append( "-dTextAlphaBits=4" );
	if (AliasGr->isChecked())
		args.append( "-dGraphicsAlphaBits=4" );
	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = prefsManager->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd1 = QString("-sFONTPATH=%1").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd1 += QString("%1%2").arg(sep).arg(extraFonts->get(i,0));
	if( !cmd1.isEmpty() )
		args.append( cmd1 );
	// then add any final args and call gs
	if ((EnableCMYK->isChecked()) && (HaveTiffSep == 0))
		args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(prefsManager->preferencesLocation()+"/sc.tif")) );
	else
		args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(prefsManager->preferencesLocation()+"/sc.png")) );
	args.append( QDir::convertSeparators(prefsManager->preferencesLocation()+"/tmp.ps") );
	args.append( "-c" );
	args.append( "showpage" );
	args.append( "-c" );
	args.append( "quit" );
	ret = System( args );
	return ret;
}

int PPreview::RenderPreviewSep(int Seite, int Res)
{
	int ret = -1;
	QString cmd;
	QStringList args, args1, args2, args3;
	QMap<QString,int> ReallyUsed;
	// Recreate Postscript-File only when the actual Page has changed
	if ((Seite != APage)  || (EnableGCR->isChecked() != GMode))
	{
		ReallyUsed.clear();
		doc->getUsedFonts(&ReallyUsed);
		PSLib *dd = new PSLib(true, prefsManager->appPrefs.AvailFonts, ReallyUsed, doc->PageColors, false, true);
		if (dd != NULL)
		{
			dd->PS_set_file(prefsManager->preferencesLocation()+"/tmp.ps");
			std::vector<int> pageNs;
			pageNs.push_back(Seite+1);
			QStringList spots;
			dd->CreatePS(doc, /*view, */pageNs, false, tr("All"), spots, true, false, false, false, EnableGCR->isChecked(), false);
			delete dd;
		}
		else
			return ret;
	}
	QString tmp, tmp2, tmp3;
	double b = doc->Pages->at(Seite)->width() * Res / 72.0;
	double h = doc->Pages->at(Seite)->height() * Res / 72.0;

	args1.append( getShortPathName(prefsManager->ghostscriptExecutable()) );
	args1.append( "-q" );
	args1.append( "-dNOPAUSE" );
	args1.append( "-dPARANOIDSAFER" );
	args1.append( QString("-r%1").arg(tmp.setNum(qRound(Res))) );
	args1.append( QString("-g%1x%2").arg(tmp2.setNum(qRound(b))).arg(tmp3.setNum(qRound(h))) ); 
	if (AliasText->isChecked())
		args1.append("-dTextAlphaBits=4");
	if (AliasGr->isChecked())
		args1.append("-dGraphicsAlphaBits=4");
	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = prefsManager->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(extraFonts->get(i,0));
	if( !cmd.isEmpty() )
		args1.append( cmd );
	args1.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(prefsManager->preferencesLocation()+"/sc.tif")) );

	args2.append( QDir::convertSeparators(prefsManager->preferencesLocation()+"/tmp.ps") );
	args2.append("-c");
	args2.append("quit");

	ColorList usedSpots;
	doc->getUsedColors(usedSpots, true);
	QStringList spots = usedSpots.keys();
	args3.append( "-sDEVICE=tiffsep" );

	args3.append( "-c" );
	cmd = "<< /SeparationColorNames ";
	QString allSeps ="[ /Cyan /Magenta /Yellow /Black ";
	for (uint sp = 0; sp < spots.count(); ++sp)
	{
		allSeps += "("+spots[sp]+") ";
	}
	allSeps += "]";
	cmd += allSeps + " /SeparationOrder [ /Cyan /Magenta /Yellow /Black] >> setpagedevice";
	args3.append(cmd);

	args3.append("-f");
	ret = System(args1 + args3 + args2, prefsManager->preferencesLocation()+"/sc.tif.txt" );

	QFile sepInfo(QDir::convertSeparators(prefsManager->preferencesLocation()+"/sc.tif.txt"));
	sepsToFileNum.clear();
	if (sepInfo.open(IO_ReadOnly))
	{
		QString Sname;
		QTextStream tsC(&sepInfo);
		int counter = 0;
		while (!tsC.atEnd())
		{
			Sname = tsC.readLine();
			QString tt = Sname.remove("%%SeparationName:").stripWhiteSpace();
			sepsToFileNum.insert(tt, counter);
			counter++;
		}
	}
	sepInfo.close();
	QString currSeps = "";
	uint spc = 0;
	for (uint sp = 0; sp < spots.count(); ++sp)
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
			ret = System(args1 + args3 + args2);
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
		ret = System(args1 + args3 + args2);
	}
	return ret;
}

void PPreview::blendImages(QImage &target, ScImage &source, ScColor col)
{
	//FIXME: if source and target have different sizesomething went wrong.
	// eg. loadPicture() failed and returned a 1x1 image
	int h = QMIN(target.height(),source.height());
	int w = QMIN(target.width(),source.width());
	int cyan, c, m, yc, k, cc, mm, yy, kk;
	col.getCMYK(&c, &m, &yc, &k);
	for (int y=0; y < h; ++y )
	{
		QRgb *p = (QRgb *)target.scanLine( y );
		QRgb *pq = (QRgb *)source.scanLine( y );
		for (int x=0; x < w; ++x )
		{
			cyan = 255 - qRed(*pq);
			if (cyan != 0)
			{
				(c == 0) ? cc = qRed(*p) : cc = QMIN(c * cyan / 255 + qRed(*p), 255);
				(m == 0) ? mm = qGreen(*p) : mm = QMIN(m * cyan / 255 + qGreen(*p), 255);
				(yc == 0) ? yy = qBlue(*p) : yy = QMIN(yc * cyan / 255 + qBlue(*p), 255);
				(k == 0) ? kk = qAlpha(*p) : kk = QMIN(k * cyan / 255 + qAlpha(*p), 255);
				*p = qRgba(cc, mm, yy, kk);
			}
			p++;
			pq++;
		}
	}
}

/*!
 \fn QPixmap PPreview::CreatePreview(int Seite, int Res)
 \author Franz Schmid
 \date
 \brief Creates the Preview of the Actual Page
 \param Seite int page number
 \param Res int
 \retval Bild QPixmap print preview
 */
QPixmap PPreview::CreatePreview(int Seite, int Res)
{
	int ret = -1;
	QPixmap Bild;
	double b = doc->Pages->at(Seite)->width() * Res / 72.0;
	double h = doc->Pages->at(Seite)->height() * Res / 72.0;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	if ((Seite != APage) || (EnableCMYK->isChecked() != CMode) || (SMode != scaleBox->currentItem())
	        || (AliasText->isChecked() != TxtAl) || (AliasGr->isChecked() != GrAl) || (EnableGCR->isChecked() != GMode)
	        || ((AliasTr->isChecked() != Trans) && (!EnableCMYK->isChecked())))
	{
		if (!EnableCMYK->isChecked() || (HaveTiffSep != 0))
		{
			ret = RenderPreview(Seite, Res);
			if (ret != 0)
			{
				Bild.resize(1,1);
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				APage = Seite;
				return Bild;
			}
		}
	}
	QImage image;
	if (EnableCMYK->isChecked())
	{
		int cyan, magenta, yellow, black, alpha;
		uint *p;
		if (HaveTiffSep == 0)
		{
			if ((Seite != APage) || (EnableCMYK->isChecked() != CMode) || (SMode != scaleBox->currentItem())
	       	 || (AliasText->isChecked() != TxtAl) || (AliasGr->isChecked() != GrAl) || (EnableGCR->isChecked() != GMode)
	       	 || ((AliasTr->isChecked() != Trans) && (!EnableCMYK->isChecked())))
			{
				ret = RenderPreviewSep(Seite, Res);
			}
			ScImage im;
			bool mode;
			int w = qRound(b);
			int h2 = qRound(h);
			image = QImage(w, h2, 32);
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
			if (flagsVisible["Cyan"]->isChecked())
			{
				im.LoadPicture(prefsManager->preferencesLocation()+"/sc.tif.Cyan.tif", "", 0, false, false, 1, 72, &mode);
				blendImages(image, im, ScColor(255, 0, 0, 0));
			}
			if (flagsVisible["Magenta"]->isChecked())
			{
				im.LoadPicture(prefsManager->preferencesLocation()+"/sc.tif.Magenta.tif", "", 0, false, false, 1, 72, &mode);
				blendImages(image, im, ScColor(0, 255, 0, 0));
			}
			if (flagsVisible["Yellow"]->isChecked())
			{
				im.LoadPicture(prefsManager->preferencesLocation()+"/sc.tif.Yellow.tif", "", 0, false, false, 1, 72, &mode);
				blendImages(image, im, ScColor(0, 0, 255, 0));
			}
			QMap<QString, int>::Iterator sepit;
			for (sepit = sepsToFileNum.begin(); sepit != sepsToFileNum.end(); ++sepit)
			{
				if (flagsVisible[sepit.key()]->isChecked())
				{
					QString fnam = QString(prefsManager->preferencesLocation()+"/sc.tif.s%1.tif").arg(sepit.data());
					im.LoadPicture(fnam, "", 0, false, false, 1, 72, &mode);
					blendImages(image, im, doc->PageColors[sepit.key()]);
				}
			}
			if (flagsVisible["Black"]->isChecked())
			{
				im.LoadPicture(prefsManager->preferencesLocation()+"/sc.tif.Black.tif", "", 0, false, false, 1, 72, &mode);
				blendImages(image, im, ScColor(0, 0, 0, 255));
			}
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
						*q = qRgba(255-QMIN(255, cyan+black), 255-QMIN(255,magenta+black), 255-QMIN(255,yellow+black), 255);
					else
					{
						if (!AliasTr->isChecked())
							*q = qRgba(255, 255, 255, 255);
					}
					q++;
				}
			}
		}
		else
		{
			int w = qRound(b);
			int w2 = 4*w;
			int h2 = qRound(h);
			image = QImage(w, h2, 32);
			QByteArray imgc(w2);
			QFile f(prefsManager->preferencesLocation()+"/sc.png");
			if (f.open(IO_ReadOnly))
			{
				for (int y=0; y < h2; ++y )
				{
					p = (uint *)image.scanLine( y );
					f.readBlock(imgc.data(), w2);
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
						*p = qRgba(255-QMIN(255, cyan+black), 255-QMIN(255,magenta+black), 255-QMIN(255,yellow+black), alpha);
						p++;
					}
				}
				f.close();
			}
		}
	}
	else
	{
		image.load(prefsManager->preferencesLocation()+"/sc.png");
		image = image.convertDepth(32);
		if ((AliasTr->isChecked()) && (HavePngAlpha == 0))
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
	image.setAlphaBuffer(true);
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
		Bild.convertFromImage(image);
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	APage = Seite;
	CMode = EnableCMYK->isChecked();
	TxtAl = AliasText->isChecked();
	GrAl = AliasGr->isChecked();
	Trans = AliasTr->isChecked();
	GMode = EnableGCR->isChecked();
	SMode = scaleBox->currentItem();
	return Bild;
}

//-------------------------------------------------------------------------------------------------

bool PPreview::usePostscriptPreview(QString printerName)
{
#ifdef _WIN32
	if ( printerName == tr("File") )
		return true;
	else if( printerName.isEmpty() )
		return PrinterUtil::isPostscriptPrinter( ScWinPrint::getDefaultPrinter() );
	else
		return PrinterUtil::isPostscriptPrinter( printerName );
#else
	return true;
#endif
}

//-------------------------------------------------------------------------------------------------

