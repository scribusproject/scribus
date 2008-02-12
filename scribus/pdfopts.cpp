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
#include "pdfopts.h"

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

#include "commonstrings.h"
#include "customfdialog.h"
#include "pdfoptions.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scribusview.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"

PDFExportDialog::PDFExportDialog( QWidget* parent, const QString & docFileName,
								  const QMap<QString, int > & DocFonts,
								  ScribusView *currView, PDFOptions & pdfOptions,
								  const QList<PDFPresentationData> & Eff,
								  const ProfilesL & PDFXProfiles, const SCFonts &AllFonts,
								  double unitRatio, const ProfilesL & printerProfiles)
	: QDialog( parent ),
	doc(currView->Doc),
	EffVal(Eff),
	Opts(pdfOptions),
	docUnitRatio(unitRatio),
	cmsDescriptorName(""),
	components(3),
	appPrinterProfiles(printerProfiles)
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
	if (!Opts.Datei.isEmpty())
		fileNameLineEdit->setText( QDir::convertSeparators(Opts.Datei) );
	else
	{
		QFileInfo fi(docFileName);
		if (fi.exists())
		{
			QString fileName(fi.path()+"/"+fi.baseName()+".pdf");
			fileNameLineEdit->setText( QDir::convertSeparators(fileName) );
		}
		else
		{
			PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
			QString pdfdir = dirs->get("pdf", fi.path());
			if (pdfdir.right(1) != "/")
				pdfdir += "/";
			QString fileName(pdfdir+fi.baseName()+".pdf");
			fileNameLineEdit->setText( QDir::convertSeparators(fileName) );
		}
	}
	NameLayout->addWidget( fileNameLineEdit, 0, 0 );
	FileC = new QToolButton( Name );
	FileC->setText( tr( "Cha&nge..." ) );
	FileC->setMinimumSize( QSize( 88, 24 ) );
	NameLayout->addWidget( FileC, 0, 1 );
	multiFile = new QCheckBox( tr( "Output one file for eac&h page" ), Name );
	multiFile->setChecked(Opts.doMultiFile);
	NameLayout->addWidget( multiFile, 1, 0 );
	PDFExportLayout->addWidget( Name );
	Options = new TabPDFOptions( this, pdfOptions, AllFonts, PDFXProfiles, DocFonts,
								Eff, currView->Doc->unitIndex(), currView->Doc->pageHeight,
								currView->Doc->pageWidth, currView->Doc, true );
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
	if ((Opts.Version == PDFOptions::PDFVersion_X3) && (Options->InfoString->text().isEmpty()))
		OK->setEnabled(false);
	resize(sizeHint());
//	setMaximumSize( sizeHint() );
//tooltips
	multiFile->setToolTip( "<qt>" + tr( "This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.") + "</qt>" );
	OK->setToolTip( "<qt>" + tr( "The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.") + "</qt>" );
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
	QString fn = QDir::fromNativeSeparators(fileNameLineEdit->text());
	// Checking if the path exists
	QFileInfo fi(fn);
	QString dirPath = QDir::convertSeparators(fi.absolutePath());
	if (!QFile::exists(fi.absolutePath()))
	{
		if (QMessageBox::question(this, tr( "Save as PDF" ),
									tr("%1 does not exists and will be created, continue?").arg(dirPath),
									QMessageBox::Ok | QMessageBox::Cancel)
				  == QMessageBox::Cancel)
		{
			return;
		}
	}
	// NOTE: Qt4 contains QDir::mkpath()
	QDir d(fn);
	QStringList dirList = dirPath.split(QDir::separator(), QString::SkipEmptyParts);
	QString existingPath;
#ifndef _WIN32
	existingPath = QDir::separator();
#endif
	for (QStringList::Iterator it = dirList.begin(); it != dirList.end(); ++it )
	{
		existingPath += (*it) + QDir::separator();
		if (!d.exists(existingPath))
		{
			if (!d.mkdir(existingPath))
			{
				QMessageBox::warning(this,
									 CommonStrings::trWarning,
									 tr("Cannot create directory: \n%1").arg(existingPath),
									 CommonStrings::tr_OK);
				return;
			}
		}
	} // end of Qt4 QDir::mkpath() simulation

	bool doIt = false;
	if (multiFile->isChecked())
		doIt = true;
	else
		doIt = overwrite(this, fn);
	if (doIt)
	{
		EffVal = Options->EffVal;
		EffVal[Options->PgSel].pageViewDuration = Options->PageTime->value();
		EffVal[Options->PgSel].pageEffectDuration = Options->EffectTime->value();
		EffVal[Options->PgSel].effectType = Options->EffectType->currentIndex();
		EffVal[Options->PgSel].Dm = Options->EDirection->currentIndex();
		EffVal[Options->PgSel].M = Options->EDirection_2->currentIndex();
		EffVal[Options->PgSel].Di = Options->EDirection_2_2->currentIndex();
		Opts.LPISettings[Options->SelLPIcolor].Frequency = Options->LPIfreq->value();
		Opts.LPISettings[Options->SelLPIcolor].Angle = Options->LPIangle->value();
		Opts.LPISettings[Options->SelLPIcolor].SpotFunc = Options->LPIfunc->currentIndex();
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
	CustomFDialog dia(this, wdir, tr("Save as"), tr("PDF Files (*.pdf);;All Files (*)"), fdNone);
	if (!fileNameLineEdit->text().isEmpty())
		dia.setSelection(fileNameLineEdit->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		dirs->set("pdf", fn.left(fn.lastIndexOf("/")));
	}
	else
		return;
	fileNameLineEdit->setText( QDir::convertSeparators(fn) );
}

void PDFExportDialog::fileNameChanged()
{
	QString fileName = checkFileExtension(fileNameLineEdit->text(),"pdf");
	fileNameLineEdit->setText( QDir::convertSeparators(fileName) );
}

void PDFExportDialog::updateDocOptions()
{
	Opts.Datei = QDir::fromNativeSeparators(fileNameLineEdit->text());
	Opts.doMultiFile = multiFile->isChecked();
	Opts.Thumbnails = Options->CheckBox1->isChecked();
	Opts.Compress = Options->Compression->isChecked();
	Opts.CompressMethod = (PDFOptions::PDFCompression) Options->CMethod->currentIndex();
	Opts.Quality = Options->CQuality->currentIndex();
	Opts.Resolution = Options->Resolution->value();
	Opts.EmbedList = Options->FontsToEmbed;
	Opts.SubsetList = Options->FontsToOutline;
	Opts.RecalcPic = Options->DSColor->isChecked();
	Opts.PicRes = Options->ValC->value();
	Opts.Bookmarks = Options->CheckBM->isChecked();
	Opts.Binding = Options->ComboBind->currentIndex();
	Opts.MirrorH = Options->MirrorH->isChecked();
	Opts.MirrorV = Options->MirrorV->isChecked();
	Opts.doClip = Options->ClipMarg->isChecked();
	Opts.RotateDeg = Options->RotateDeg->currentIndex() * 90;
	Opts.PresentMode = Options->CheckBox10->isChecked();
	Opts.PresentVals = EffVal;
	Opts.Articles = Options->Article->isChecked();
	Opts.Encrypt = Options->Encry->isChecked();
	Opts.UseLPI = Options->UseLPI->isChecked();
	Opts.useLayers = Options->useLayers->isChecked();
	Opts.UseSpotColors = !Options->useSpot->isChecked();
	Opts.doOverprint = Options->overprintMode->isChecked();
	Opts.displayBookmarks = Options->useBookmarks->isChecked();
	Opts.displayFullscreen = Options->useFullScreen->isChecked();
	Opts.displayLayers = Options->useLayers2->isChecked();
	Opts.displayThumbs = Options->useThumbnails->isChecked();
	Opts.hideMenuBar = Options->hideMenuBar->isChecked();
	Opts.hideToolBar = Options->hideToolBar->isChecked();
	Opts.fitWindow = Options->fitWindow->isChecked();
	Opts.useDocBleeds = Options->docBleeds->isChecked();
	if (!Options->docBleeds->isChecked())
	{
		Opts.bleeds.Top = Options->BleedTop->value()/docUnitRatio;
		Opts.bleeds.Left = Options->BleedLeft->value()/docUnitRatio;
		Opts.bleeds.Right = Options->BleedRight->value()/docUnitRatio;
		Opts.bleeds.Bottom = Options->BleedBottom->value()/docUnitRatio;
	}
	Opts.markOffset = Options->markOffset->value()/docUnitRatio;
	Opts.cropMarks = Options->cropMarks->isChecked();
	Opts.bleedMarks = Options->bleedMarks->isChecked();
	Opts.registrationMarks = Options->registrationMarks->isChecked();
	Opts.colorMarks = Options->colorMarks->isChecked();
	Opts.docInfoMarks = Options->docInfoMarks->isChecked();
	int pgl = PDFOptions::SinglePage;
	if (Options->singlePage->isChecked())
		pgl = PDFOptions::SinglePage;
	else if (Options->continuousPages->isChecked())
		pgl = PDFOptions::OneColumn;
	else if (Options->doublePageLeft->isChecked())
		pgl = PDFOptions::TwoColumnLeft;
	else if (Options->doublePageRight->isChecked())
		pgl = PDFOptions::TwoColumnRight;
	Opts.PageLayout = pgl;
	if (Options->actionCombo->currentIndex() != 0)
		Opts.openAction = Options->actionCombo->currentText();
	else
		Opts.openAction = "";
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
		Opts.Permissions = Perm;
		Opts.PassOwner = Options->PassOwner->text();
		Opts.PassUser = Options->PassUser->text();
	}
	if (Options->PDFVersionCombo->currentIndex() == 0)
		Opts.Version = PDFOptions::PDFVersion_13;
	if (Options->PDFVersionCombo->currentIndex() == 1)
		Opts.Version = PDFOptions::PDFVersion_14;
	if (Options->PDFVersionCombo->currentIndex() == 2)
		Opts.Version = PDFOptions::PDFVersion_15;
	if (Options->PDFVersionCombo->currentIndex() == 3)
		Opts.Version = PDFOptions::PDFVersion_X3;
	if (Options->OutCombo->currentIndex() == 0)
	{
		Opts.UseRGB = true;
		Opts.isGrayscale = false;
		Opts.UseProfiles = false;
		Opts.UseProfiles2 = false;
	}
	else
	{
		if (Options->OutCombo->currentIndex() == 2)
		{
			Opts.isGrayscale = true;
			Opts.UseRGB = false;
			Opts.UseProfiles = false;
			Opts.UseProfiles2 = false;
		}
		else
		{
			Opts.isGrayscale = false;
			Opts.UseRGB = false;
			if (doc->HasCMS)
			{
				Opts.UseProfiles = Options->EmbedProfs->isChecked();
				Opts.UseProfiles2 = Options->EmbedProfs2->isChecked();
				Opts.Intent = Options->IntendS->currentIndex();
				Opts.Intent2 = Options->IntendI->currentIndex();
				Opts.EmbeddedI = Options->NoEmbedded->isChecked();
				Opts.SolidProf = Options->SolidPr->currentText();
				Opts.ImageProf = Options->ImageP->currentText();
				Opts.PrintProf = Options->PrintProfC->currentText();
				if (Opts.Version == PDFOptions::PDFVersion_X3)
				{
					cmsHPROFILE hIn;
					QByteArray profilePath( appPrinterProfiles[Opts.PrintProf].toLocal8Bit() );
					hIn = cmsOpenProfileFromFile(profilePath.data(), "r");
					const char *Descriptor = cmsTakeProductDesc(hIn);
					cmsDescriptorName = QString(Descriptor);
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
						components = 3;
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
						components = 4;
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmyData)
						components = 3;
					cmsCloseProfile(hIn);
					Opts.Info = Options->InfoString->text();
					Opts.Encrypt = false;
					Opts.MirrorH = false;
					Opts.MirrorV = false;
					Opts.RotateDeg = 0;
					Opts.PresentMode = false;
					Opts.Encrypt = false;
				}
			}
			else
			{
				Opts.UseProfiles = false;
				Opts.UseProfiles2 = false;
			}
		}
	}
}

const QString PDFExportDialog::cmsDescriptor()
{
	return cmsDescriptorName;
}

const int PDFExportDialog::colorSpaceComponents()
{
	return components;
}

const QString PDFExportDialog::getPagesString()
{
	if (Options->AllPages->isChecked())
		return "*";
	return Options->PageNr->text();
}
