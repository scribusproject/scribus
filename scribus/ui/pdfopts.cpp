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
#include "scconfig.h"
#include "pdfopts.h"

#include "commonstrings.h"
#include "pdfoptions.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribusview.h"
#include "ui/customfdialog.h"
#include "ui/scrspinbox.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"

#include <QByteArray>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

PDFExportDialog::PDFExportDialog( QWidget* parent, const QString & docFileName,
								  const QMap<QString, int > & DocFonts,
								  ScribusView *currView, PDFOptions & pdfOptions,
								  const ProfilesL & PDFXProfiles, const SCFonts &AllFonts,
								  const ProfilesL & printerProfiles)
	: QDialog( parent ),
	m_doc(currView->Doc),
	m_opts(pdfOptions),
	m_unitRatio(currView->Doc->unitRatio()),
	m_cmsDescriptor(""),
	m_components(3),
	m_printerProfiles(printerProfiles)
{
	setModal(true);
	setWindowTitle( tr( "Save as PDF" ) );
	setWindowIcon(loadIcon("AppIcon.png"));
	PDFExportLayout = new QVBoxLayout( this );
	PDFExportLayout->setSpacing( 5 );
	PDFExportLayout->setMargin( 10 );
	Name = new QGroupBox( this );
	Name->setTitle( tr( "O&utput to File:" ) );
	NameLayout = new QGridLayout( Name );
	NameLayout->setSpacing( 5 );
	NameLayout->setMargin( 10 );
	NameLayout->setAlignment( Qt::AlignTop );
	fileNameLineEdit = new QLineEdit( Name );
	fileNameLineEdit->setMinimumSize( QSize( 268, 22 ) );
	if (!m_opts.fileName.isEmpty())
		fileNameLineEdit->setText( QDir::toNativeSeparators(m_opts.fileName) );
	else
	{
		QFileInfo fi(docFileName);
		QString completeBaseName = fi.completeBaseName();
		if (completeBaseName.endsWith(".sla", Qt::CaseInsensitive))
			if (completeBaseName.length() > 4) completeBaseName.chop(4);
		if (completeBaseName.endsWith(".gz", Qt::CaseInsensitive))
			if (completeBaseName.length() > 3) completeBaseName.chop(3);
		if (fi.exists())
		{
			QString fileName(fi.path() + "/" + completeBaseName + ".pdf");
			fileNameLineEdit->setText( QDir::toNativeSeparators(fileName) );
		}
		else
		{
			PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
			QString pdfdir = dirs->get("pdf", fi.path());
			if (pdfdir.right(1) != "/")
				pdfdir += "/";
			QString fileName(pdfdir + completeBaseName + ".pdf");
			fileNameLineEdit->setText( QDir::toNativeSeparators(fileName) );
		}
	}
	NameLayout->addWidget( fileNameLineEdit, 0, 0 );
	FileC = new QToolButton( Name );
	FileC->setText( tr( "Cha&nge..." ) );
	FileC->setMinimumSize( QSize( 88, 24 ) );
	NameLayout->addWidget( FileC, 0, 1 );
	multiFile = new QCheckBox( tr( "Output one file for eac&h page" ), Name );
	multiFile->setChecked(m_opts.doMultiFile);
	NameLayout->addWidget( multiFile, 1, 0 );
	openAfterExportCheckBox = new QCheckBox( tr( "Open PDF after Export" ), Name );
	openAfterExportCheckBox->setChecked(m_opts.openAfterExport);
	NameLayout->addWidget( openAfterExportCheckBox, 2, 0 );
	PDFExportLayout->addWidget( Name );

	Options = new TabPDFOptions( this, pdfOptions, AllFonts, PDFXProfiles, DocFonts,
								currView->Doc->unitIndex(), currView->Doc->pageHeight(),
								currView->Doc->pageWidth(), currView->Doc, true );
	PDFExportLayout->addWidget( Options );
	Layout7 = new QHBoxLayout;
	Layout7->setSpacing( 5 );
	Layout7->setMargin( 0 );
	QSpacerItem* spacer_2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout7->addItem( spacer_2 );
	OK = new QPushButton( tr( "&Save" ), this );
	OK->setAutoDefault( true );
	OK->setDefault( true );
	Layout7->addWidget( OK );
	Cancel = new QPushButton( CommonStrings::tr_Cancel, this );
	Layout7->addWidget( Cancel );
	PDFExportLayout->addLayout( Layout7 );
	if ((m_opts.Version == PDFOptions::PDFVersion_X3) && (Options->InfoString->text().isEmpty()))
		OK->setEnabled(false);
	resize(sizeHint());
//	setMaximumSize( sizeHint() );
//tooltips
	multiFile->setToolTip( "<qt>" + tr( "This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.") + "</qt>" );
	openAfterExportCheckBox->setToolTip( "<qt>" + tr( "Open the exported PDF with the PDF viewer as set in External Tools preferences, when not exporting to a multi-file export destination.") + "</qt>" );
	OK->setToolTip( "<qt>" + tr( "The save button will be disabled if you are trying to export PDF/X and the info string is missing from the PDF/X tab.") + "</qt>" );
	// signals and slots connections
	connect( FileC, SIGNAL( clicked() ), this, SLOT( ChangeFile() ) );
	connect( OK, SIGNAL( clicked() ), this, SLOT( DoExport() ) );
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( fileNameLineEdit, SIGNAL( editingFinished() ), this, SLOT( fileNameChanged() ) );
	connect( Options, SIGNAL(noInfo()), this, SLOT(disableSave()));
	connect( Options, SIGNAL(hasInfo()), this, SLOT(enableSave()));
}

void PDFExportDialog::enableSave()
{
	OK->setEnabled(true);
}

void PDFExportDialog::disableSave()
{
	OK->setEnabled(false);
}

void PDFExportDialog::DoExport()
{
	bool createPath = false;
	QString fn = QDir::fromNativeSeparators(fileNameLineEdit->text());
	// Checking if the path exists
	QFileInfo fi(fn);
	QString dirPath = QDir::toNativeSeparators(fi.absolutePath());
	if (!QFile::exists(fi.absolutePath()))
	{
		if (QMessageBox::question(this, tr( "Save as PDF" ),
									tr("%1 does not exists and will be created, continue?").arg(dirPath),
									QMessageBox::Ok | QMessageBox::Cancel)
				  == QMessageBox::Cancel)
		{
			return;
		}
		createPath = true;
	}
	
	// NOTE: Qt4 contains QDir::mkpath()
	QDir d(fn);
	if (createPath)
	{
		if (!d.mkpath(fi.absolutePath()))
		{
			QMessageBox::warning(this,
								 CommonStrings::trWarning,
								 tr("Cannot create directory: \n%1").arg(dirPath),
								 CommonStrings::tr_OK);
			return;
		}
	}

	bool doIt = false;
	if (multiFile->isChecked())
		doIt = true;
	else
		doIt = overwrite(this, fn);
	if (doIt)
	{
		m_presEffects = Options->EffVal;
		m_presEffects[Options->PgSel].pageViewDuration = Options->PageTime->value();
		m_presEffects[Options->PgSel].pageEffectDuration = Options->EffectTime->value();
		m_presEffects[Options->PgSel].effectType = Options->EffectType->currentIndex();
		m_presEffects[Options->PgSel].Dm = Options->EDirection->currentIndex();
		m_presEffects[Options->PgSel].M = Options->EDirection_2->currentIndex();
		m_presEffects[Options->PgSel].Di = Options->EDirection_2_2->currentIndex();
		m_opts.LPISettings[Options->SelLPIcolor].Frequency = Options->LPIfreq->value();
		m_opts.LPISettings[Options->SelLPIcolor].Angle = Options->LPIangle->value();
		m_opts.LPISettings[Options->SelLPIcolor].SpotFunc = Options->LPIfunc->currentIndex();
		accept();
	}
	else
		return;
}

void PDFExportDialog::ChangeFile()
{
	QString fn;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("pdf", ".");
	CustomFDialog dia(this, wdir, tr("Save As"), tr("PDF Files (*.pdf);;All Files (*)"), fdNone);
	if (!fileNameLineEdit->text().isEmpty())
	{
		QString fileName = QDir::fromNativeSeparators(fileNameLineEdit->text()); 
		dia.setSelection(fileName);
	}
	if (dia.exec() == QDialog::Accepted)
	{
		// selectedFile() may return path with native separators
		fn = QDir::fromNativeSeparators(dia.selectedFile());
		dirs->set("pdf", fn.left(fn.lastIndexOf("/")));
		fileNameLineEdit->setText( QDir::toNativeSeparators(fn) );
	}	
}

void PDFExportDialog::fileNameChanged()
{
	QString fileName = checkFileExtension(fileNameLineEdit->text(),"pdf");
	fileNameLineEdit->setText( QDir::toNativeSeparators(fileName) );
}

void PDFExportDialog::updateDocOptions()
{
	m_opts.fileName = QDir::fromNativeSeparators(fileNameLineEdit->text());
	m_opts.doMultiFile = multiFile->isChecked();
	m_opts.openAfterExport = openAfterExportCheckBox->isChecked();
	m_opts.Thumbnails = Options->CheckBox1->isChecked();
	m_opts.Compress = Options->Compression->isChecked();
	m_opts.CompressMethod = (PDFOptions::PDFCompression) Options->CMethod->currentIndex();
	m_opts.Quality = Options->CQuality->currentIndex();
	m_opts.Resolution = Options->Resolution->value();
	m_opts.EmbedList = Options->FontsToEmbed;
	m_opts.SubsetList = Options->FontsToOutline;
	m_opts.RecalcPic = Options->DSColor->isChecked();
	m_opts.PicRes = Options->ValC->value();
	m_opts.embedPDF = Options->EmbedPDF->isChecked();
	m_opts.Bookmarks = Options->CheckBM->isChecked();
	m_opts.Binding = Options->ComboBind->currentIndex();
	m_opts.MirrorH = Options->MirrorH->isChecked();
	m_opts.MirrorV = Options->MirrorV->isChecked();
	m_opts.doClip = Options->ClipMarg->isChecked();
	m_opts.RotateDeg = Options->RotateDeg->currentIndex() * 90;
	m_opts.pageRangeSelection = Options->AllPages->isChecked() ? 0 : 1;
	m_opts.pageRangeString = Options->PageNr->text();
	m_opts.PresentMode = Options->CheckBox10->isChecked();
	if (m_opts.PresentMode)
	{
		for (int pg = 0; pg < m_doc->Pages->count(); ++pg)
		{
			m_doc->Pages->at(pg)->PresentVals = m_presEffects[pg];
		}
	}
	m_opts.Articles = Options->Article->isChecked();
	m_opts.Encrypt = Options->Encry->isChecked();
	m_opts.UseLPI = Options->UseLPI->isChecked();
	m_opts.useLayers = Options->useLayers->isChecked();
	m_opts.UseSpotColors = !Options->useSpot->isChecked();
	m_opts.displayBookmarks = Options->useBookmarks->isChecked();
	m_opts.displayFullscreen = Options->useFullScreen->isChecked();
	m_opts.displayLayers = Options->useLayers2->isChecked();
	m_opts.displayThumbs = Options->useThumbnails->isChecked();
	m_opts.hideMenuBar = Options->hideMenuBar->isChecked();
	m_opts.hideToolBar = Options->hideToolBar->isChecked();
	m_opts.fitWindow = Options->fitWindow->isChecked();
	m_opts.useDocBleeds = Options->docBleeds->isChecked();
	if (!Options->docBleeds->isChecked())
	{
		m_opts.bleeds.Top = Options->BleedTop->value() / m_unitRatio;
		m_opts.bleeds.Left = Options->BleedLeft->value() / m_unitRatio;
		m_opts.bleeds.Right = Options->BleedRight->value() / m_unitRatio;
		m_opts.bleeds.Bottom = Options->BleedBottom->value()/ m_unitRatio;
	}
	m_opts.markLength = Options->markLength->value() / m_unitRatio;
	m_opts.markOffset = Options->markOffset->value() / m_unitRatio;
	m_opts.cropMarks = Options->cropMarks->isChecked();
	m_opts.bleedMarks = Options->bleedMarks->isChecked();
	m_opts.registrationMarks = Options->registrationMarks->isChecked();
	m_opts.colorMarks = Options->colorMarks->isChecked();
	m_opts.docInfoMarks = Options->docInfoMarks->isChecked();
	int pgl = PDFOptions::SinglePage;
	if (Options->singlePage->isChecked())
		pgl = PDFOptions::SinglePage;
	else if (Options->continuousPages->isChecked())
		pgl = PDFOptions::OneColumn;
	else if (Options->doublePageLeft->isChecked())
		pgl = PDFOptions::TwoColumnLeft;
	else if (Options->doublePageRight->isChecked())
		pgl = PDFOptions::TwoColumnRight;
	m_opts.PageLayout = pgl;
	if (Options->actionCombo->currentIndex() != 0)
		m_opts.openAction = Options->actionCombo->currentText();
	else
		m_opts.openAction = "";
	if (Options->Encry->isChecked())
	{
		int Perm = -64;
		if (Options->PDFVersionCombo->currentIndex() == 1)
			Perm &= ~0x00240000;
		if (Options->PrintSec->isChecked())
			Perm += 4;
		if (Options->ModifySec->isChecked())
			Perm += 8;
		if (Options->CopySec->isChecked())
			Perm += 16;
		if (Options->AddSec->isChecked())
			Perm += 32;
		m_opts.Permissions = Perm;
		m_opts.PassOwner = Options->PassOwner->text();
		m_opts.PassUser = Options->PassUser->text();
	}
	if (Options->PDFVersionCombo->currentIndex() == 0)
		m_opts.Version = PDFOptions::PDFVersion_13;
	if (Options->PDFVersionCombo->currentIndex() == 1)
		m_opts.Version = PDFOptions::PDFVersion_14;
	if (Options->PDFVersionCombo->currentIndex() == 2)
		m_opts.Version = PDFOptions::PDFVersion_15;
	if (Options->PDFVersionCombo->currentIndex() == 3)
		m_opts.Version = PDFOptions::PDFVersion_X1a;
	if (Options->PDFVersionCombo->currentIndex() == 4)
		m_opts.Version = PDFOptions::PDFVersion_X3;
	if (Options->PDFVersionCombo->currentIndex() == 5)
		m_opts.Version = PDFOptions::PDFVersion_X4;
	if (Options->OutCombo->currentIndex() == 0)
	{
		m_opts.UseRGB = true;
		m_opts.isGrayscale = false;
		m_opts.UseProfiles = false;
		m_opts.UseProfiles2 = false;
	}
	else
	{
		if (Options->OutCombo->currentIndex() == 2)
		{
			m_opts.isGrayscale = true;
			m_opts.UseRGB = false;
			m_opts.UseProfiles = false;
			m_opts.UseProfiles2 = false;
		}
		else
		{
			m_opts.isGrayscale = false;
			m_opts.UseRGB = false;
			if (m_doc->HasCMS)
			{
				m_opts.UseProfiles = Options->EmbedProfs->isChecked();
				m_opts.UseProfiles2 = Options->EmbedProfs2->isChecked();
				if (m_opts.Version != PDFOptions::PDFVersion_X1a)
				{
					m_opts.Intent = Options->IntendS->currentIndex();
					m_opts.Intent2 = Options->IntendI->currentIndex();
					m_opts.EmbeddedI = Options->NoEmbedded->isChecked();
					m_opts.SolidProf = Options->SolidPr->currentText();
					m_opts.ImageProf = Options->ImageP->currentText();
				}
				m_opts.PrintProf = Options->PrintProfC->currentText();
				if ((m_opts.Version == PDFOptions::PDFVersion_X3) || (m_opts.Version == PDFOptions::PDFVersion_X1a) || (m_opts.Version == PDFOptions::PDFVersion_X4))
				{
					ScColorProfile hIn = m_doc->colorEngine.openProfileFromFile( m_printerProfiles[m_opts.PrintProf] );
					m_cmsDescriptor = hIn.productDescription();
					if (hIn.colorSpace() == ColorSpace_Rgb)
						m_components = 3;
					if (hIn.colorSpace() == ColorSpace_Cmyk)
						m_components = 4;
					if (hIn.colorSpace() == ColorSpace_Cmy)
						m_components = 3;
					m_opts.Info = Options->InfoString->text();
					m_opts.Encrypt = false;
					m_opts.MirrorH = false;
					m_opts.MirrorV = false;
					//#8306 : PDF/X-3 export ignores rotation setting
					//m_opts.RotateDeg = 0;
					m_opts.PresentMode = false;
				}
			}
			else
			{
				m_opts.UseProfiles = false;
				m_opts.UseProfiles2 = false;
			}
		}
	}
}

const QString PDFExportDialog::cmsDescriptor()
{
	return m_cmsDescriptor;
}

const int PDFExportDialog::colorSpaceComponents()
{
	return m_components;
}

const QString PDFExportDialog::getPagesString()
{
	if (Options->AllPages->isChecked())
		return "*";
	return Options->PageNr->text();
}
