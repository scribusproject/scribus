/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "pdfopts.h"
#include "pdfopts.moc"

#include "customfdialog.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "units.h"
#include "pdfoptions.h"
#include "util.h"

#include "scconfig.h"

extern ProfilesL InputProfiles;
#ifdef HAVE_CMS
extern bool CMSuse;
#endif
extern bool CMSavail;

PDF_Opts::PDF_Opts( QWidget* parent,  QString docFileName, QMap<QString,QFont> DocFonts, ScribusView *currView, PDFOptions *pdfOptions, QValueList<PDFPresentationData> Eff, ProfilesL *PDFXProfiles, SCFonts &AllFonts)
		: QDialog( parent, "pdf", true, 0 )
{
	setCaption( tr( "Save as PDF" ) );
	setIcon(loadIcon("AppIcon.png"));
	EffVal = Eff;
	Opts = pdfOptions;
	PDFOptsLayout = new QVBoxLayout( this );
	PDFOptsLayout->setSpacing( 5 );
	PDFOptsLayout->setMargin( 11 );
	Name = new QLabel( tr( "O&utput to File:" ), this, "Name" );
	Name->setFrameShape( QLabel::NoFrame );
	PDFOptsLayout->addWidget( Name );
	Layout5 = new QHBoxLayout;
	Layout5->setSpacing( 5 );
	Layout5->setMargin( 0 );
	fileNameLineEdit = new QLineEdit( this, "fileNameLineEdit" );
	fileNameLineEdit->setMinimumSize( QSize( 268, 22 ) );
	if (pdfOptions->Datei != "")
		fileNameLineEdit->setText(pdfOptions->Datei);
	else
	{
		PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
		QFileInfo fi = QFileInfo(docFileName);
		QString pdfdir = dirs->get("pdf", fi.dirPath());
		if (pdfdir.right(1) != "/")
			pdfdir += "/";
		fileNameLineEdit->setText(pdfdir+fi.baseName()+".pdf");
	}
	Name->setBuddy(fileNameLineEdit);
	Layout5->addWidget( fileNameLineEdit );
	FileC = new QToolButton( this, "FileC" );
	FileC->setText( tr( "Cha&nge..." ) );
	FileC->setMinimumSize( QSize( 88, 24 ) );
	Layout5->addWidget( FileC );
	PDFOptsLayout->addLayout( Layout5 );
	Options = new TabPDFOptions( this, pdfOptions, AllFonts, PDFXProfiles, DocFonts,
								Eff, currView->Doc->docUnitIndex, currView->Doc->pageHeight,
								currView->Doc->pageWidth, currView );
	PDFOptsLayout->addWidget( Options );
	Layout7 = new QHBoxLayout;
	Layout7->setSpacing( 5 );
	Layout7->setMargin( 0 );
	QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout7->addItem( spacer_2 );
	OK = new QPushButton( tr( "&Save" ), this, "OK" );
	OK->setAutoDefault( true );
	OK->setDefault( true );
	Layout7->addWidget( OK );
	Cancel = new QPushButton( tr( "&Cancel" ), this, "Cancel" );
	Layout7->addWidget( Cancel );
	PDFOptsLayout->addLayout( Layout7 );
	resize(sizeHint());
//	setMaximumSize( sizeHint() );


	// signals and slots connections
	connect( FileC, SIGNAL( clicked() ), this, SLOT( ChangeFile() ) );
	connect( OK, SIGNAL( clicked() ), this, SLOT( DoExport() ) );
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( fileNameLineEdit, SIGNAL( lostFocus() ), this, SLOT( fileNameChanged() ) );
	connect( fileNameLineEdit, SIGNAL( returnPressed() ), this, SLOT( fileNameChanged() ) );
}

void PDF_Opts::DoExport()
{
	QString fn = fileNameLineEdit->text();
	if (overwrite(this, fn))
	{
		EffVal = Options->EffVal;
		EffVal[Options->PgSel].pageViewDuration = Options->PageTime->value();
		EffVal[Options->PgSel].pageEffectDuration = Options->EffectTime->value();
		EffVal[Options->PgSel].effectType = Options->EffectType->currentItem();
		EffVal[Options->PgSel].Dm = Options->EDirection->currentItem();
		EffVal[Options->PgSel].M = Options->EDirection_2->currentItem();
		EffVal[Options->PgSel].Di = Options->EDirection_2_2->currentItem();
		Opts->LPISettings[Options->SelLPIcolor].Frequency = Options->LPIfreq->value();
		Opts->LPISettings[Options->SelLPIcolor].Angle = Options->LPIangle->value();
		Opts->LPISettings[Options->SelLPIcolor].SpotFunc = Options->LPIfunc->currentItem();
		accept();
	}
	else
		return;
}

void PDF_Opts::ChangeFile()
{
	QString fn;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("pdf", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), tr("PDF Files (*.pdf);;All Files (*)"), false, false);
	if (fileNameLineEdit->text() != "")
		dia.setSelection(fileNameLineEdit->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		dirs->set("pdf", fn.left(fn.findRev("/")));
	}
	else
		return;
	fileNameLineEdit->setText(fn);
}

void PDF_Opts::fileNameChanged()
{
	fileNameLineEdit->setText(checkFileExtension(fileNameLineEdit->text(),"pdf"));
}
