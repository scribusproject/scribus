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
#include "prefsfile.h"
#include "units.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

extern QPixmap loadIcon(QString nam);
extern bool overwrite(QWidget *parent, QString filename);
extern double UmReFaktor;
extern ProfilesL InputProfiles;
#ifdef HAVE_CMS
extern bool CMSuse;
#endif
extern bool CMSavail;
extern PrefsFile* prefsFile;

PDF_Opts::PDF_Opts( QWidget* parent,  QString Fname, QMap<QString,QFont> DocFonts, ScribusView *vie, PDFOptions *Optionen, QValueList<PDFPresentationData> Eff, ProfilesL *PDFXProfiles, SCFonts &AllFonts)
		: QDialog( parent, "pdf", true, 0 )
{
	setCaption( tr( "Create PDF File" ) );
	setIcon(loadIcon("AppIcon.png"));
	EffVal = Eff;
	Opts = Optionen;
	PDFOptsLayout = new QVBoxLayout( this );
	PDFOptsLayout->setSpacing( 5 );
	PDFOptsLayout->setMargin( 11 );
	Name = new QLabel( tr( "O&utput to File:" ), this, "Name" );
	Name->setFrameShape( QLabel::NoFrame );
	PDFOptsLayout->addWidget( Name );
	Layout5 = new QHBoxLayout;
	Layout5->setSpacing( 5 );
	Layout5->setMargin( 0 );
	Datei = new QLineEdit( this, "Datei" );
	Datei->setMinimumSize( QSize( 268, 22 ) );
	if (Optionen->Datei != "")
		Datei->setText(Optionen->Datei);
	else
	{
		PrefsContext* dirs = prefsFile->getContext("dirs");
		QFileInfo fi = QFileInfo(Fname);
		QString pdfdir = dirs->get("pdf", fi.dirPath());
		if (pdfdir.right(1) != "/")
			pdfdir += "/";
		Datei->setText(pdfdir+fi.baseName()+".pdf");
	}
	Name->setBuddy(Datei);
	Layout5->addWidget( Datei );
	FileC = new QToolButton( this, "FileC" );
	FileC->setText( tr( "Cha&nge..." ) );
	FileC->setMinimumSize( QSize( 88, 24 ) );
	Layout5->addWidget( FileC );
	PDFOptsLayout->addLayout( Layout5 );
	Options = new TabPDFOptions( this,
																Optionen,
																AllFonts,
																PDFXProfiles,
																DocFonts,
																Eff,
																vie->Doc->docUnitIndex,
																vie->Doc->PageH,
																vie->Doc->PageB,
																vie );
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
}

void PDF_Opts::DoExport()
{
	QString fn = Datei->text();
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
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString wdir = dirs->get("pdf", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), tr("PDF Files (*.pdf);;All Files (*)"), false, false);
	if (Datei->text() != "")
		dia.setSelection(Datei->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		dirs->set("pdf", fn.left(fn.findRev("/")));
	}
	else
		return;
	Datei->setText(fn);
}
