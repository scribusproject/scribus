/***************************************************************************
                          picstatus.cpp  -  description
                             -------------------
    begin                : Fri Nov 29 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include "picstatus.h"
#include "picstatus.moc"
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtoolbutton.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qcursor.h>
#include <cstdio>
#include "picsearch.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "pageitem.h"
#include "filesearch.h"
#include "scribus.h"
extern QPixmap loadIcon(QString nam);


const unsigned short PicStatus::COL_FILENAME = 0;
const unsigned short PicStatus::COL_PATH = 1;
const unsigned short PicStatus::COL_PAGE = 2;
const unsigned short PicStatus::COL_GOTO = 3;
const unsigned short PicStatus::COL_PRINT = 4;
const unsigned short PicStatus::COL_STATUS = 5;
const unsigned short PicStatus::COL_SEARCH = 6;

QString PicStatus::trOK = "";
QString PicStatus::trMissing = "";
QString PicStatus::trSearch = "";
QString PicStatus::trCancelSearch = "";
QString PicStatus::trGoto = "";

/*!
 \fn void PicStatus::PicStatus(QWidget* parent, ScribusDoc *docu, ScribusView *viewi)
 \author Franz Schmid
 \date
 \brief Constructs a Dialog, which list all Images in the current Document. In this Dialog it is possible
to search for missing Images.
 \param parent Pointer to parent window
 \param docu Pointer to the current Document
 \param viewi Pointer to the current View
 \retval None
 */

PicStatus::PicStatus(QWidget* parent, ScribusDoc *docu, ScribusView *viewi) :
	QDialog( parent, "pic", true, 0 )
{
	languageChange();
	uint p, i;
	QString tmp;
	setCaption( tr( "Manage Pictures" ) );
	setIcon(loadIcon("AppIcon.png"));
	doc = docu;
	view = viewi;
	ItemNrs.clear();
	FlagsPic.clear();
	PicStatusLayout = new QVBoxLayout( this );
	PicStatusLayout->setSpacing( 6 );
	PicStatusLayout->setMargin( 11 );

	PicTable = new QTable( this, "PicTable" );
	PicTable->setLeftMargin(0);
	PicTable->verticalHeader()->hide();
	PicTable->setNumCols( 7 );
	Header = PicTable->horizontalHeader();
	QString tmpc[] = { tr("Name"),  tr("Path"),  tr("Page"), " ",  tr("Print"),  tr("Status"), " "};
	size_t ar = sizeof(tmpc) / sizeof(*tmpc);
	for (uint a = 0; a < ar; ++a)
		Header->setLabel(a, tmpc[a]);
	Zeilen = 0;
	for (i=0; i < doc->MasterItems.count(); ++i)
	{
		if (doc->MasterItems.at(i)->itemType() == PageItem::ImageFrame)
		{
			Zeilen++;
			ItemNrs.append(i);
		}
	}
	for (i=0; i<doc->Items->count(); ++i)
	{
		if (doc->Items->at(i)->itemType() == PageItem::ImageFrame)
		{
			Zeilen++;
			ItemNrs.append(i);
		}
	}
	PicTable->setNumRows(Zeilen);
	int Zeilen2 = 0;
	for (i=0; i < doc->MasterItems.count(); ++i)
	{
		if (doc->MasterItems.at(i)->itemType() == PageItem::ImageFrame)
		{
			QFileInfo fi = QFileInfo(doc->MasterItems.at(i)->Pfile);
			PicTable->setText(Zeilen2, COL_FILENAME, fi.fileName());
			PicTable->setText(Zeilen2, COL_PATH, fi.dirPath());
			PicTable->setText(Zeilen2, COL_PAGE, doc->MasterItems.at(i)->OnMasterPage);
			QToolButton *tb2 = new QToolButton(this, tmp.setNum(Zeilen2));
			tb2->setText( trGoto);
			tb2->setEraseColor(white);
			PicTable->setColumnWidth(COL_GOTO, tb2->fontMetrics().width( trGoto)+10);
			PicTable->setCellWidget(Zeilen2, COL_GOTO, tb2);
			connect(tb2, SIGNAL(clicked()), this, SLOT(GotoPic()));
			QCheckBox *cp2 = new QCheckBox(this, tmp.setNum(Zeilen2));
			cp2->setText( tr("Yes"));
			cp2->setChecked(doc->MasterItems.at(i)->printable());
			cp2->setEraseColor(white);
			FlagsPic.append(cp2);
			PicTable->setCellWidget(Zeilen2, COL_PRINT, cp2);
			connect(cp2, SIGNAL(clicked()), this, SLOT(PrintPic()));
			if (doc->MasterItems.at(i)->PicAvail)
				PicTable->setText(Zeilen2, COL_STATUS, trOK);
			else
				PicTable->setText(Zeilen2, COL_STATUS, trMissing);
			QToolButton *tb = new QToolButton(this, tmp.setNum(Zeilen2));
			tb->setText( trSearch);
			PicTable->setColumnWidth(COL_SEARCH, tb->fontMetrics().width( trCancelSearch)+10);
			tb->setEraseColor(white);
			PicTable->setCellWidget(Zeilen2, COL_SEARCH, tb);
			connect(tb, SIGNAL(clicked()), this, SLOT(SearchPic()));
			Zeilen2++;
		}
	}
	for (i=0; i< doc->Items->count(); ++i)
	{
		if (doc->Items->at(i)->itemType() == PageItem::ImageFrame)
		{
			QFileInfo fi = QFileInfo(doc->Items->at(i)->Pfile);
			PicTable->setText(Zeilen2, COL_FILENAME, fi.fileName());
			PicTable->setText(Zeilen2, COL_PATH, fi.dirPath());
			p = doc->Items->at(i)->OwnPage;
			PicTable->setText(Zeilen2, COL_PAGE, tmp.setNum(p+1));
			QToolButton *tb2 = new QToolButton(this, tmp.setNum(Zeilen2));
			tb2->setText( trGoto);
			tb2->setEraseColor(white);
			PicTable->setColumnWidth(COL_GOTO, tb2->fontMetrics().width( trGoto)+10);
			PicTable->setCellWidget(Zeilen2, COL_GOTO, tb2);
			connect(tb2, SIGNAL(clicked()), this, SLOT(GotoPic()));
			QCheckBox *cp2 = new QCheckBox(this, tmp.setNum(Zeilen2));
			cp2->setText( tr("Yes"));
			cp2->setChecked(doc->Items->at(i)->printable());
			cp2->setEraseColor(white);
			FlagsPic.append(cp2);
			PicTable->setCellWidget(Zeilen2, COL_PRINT, cp2);
			connect(cp2, SIGNAL(clicked()), this, SLOT(PrintPic()));
			if (doc->Items->at(i)->PicAvail)
				PicTable->setText(Zeilen2, COL_STATUS, trOK);
			else
				PicTable->setText(Zeilen2, COL_STATUS, trMissing);
			QToolButton *tb = new QToolButton(this, tmp.setNum(Zeilen2));
			tb->setText( trSearch);
			PicTable->setColumnWidth(COL_SEARCH, tb->fontMetrics().width( trCancelSearch)+10);
			tb->setEraseColor(white);
			PicTable->setCellWidget(Zeilen2, COL_SEARCH, tb);
			connect(tb, SIGNAL(clicked()), this, SLOT(SearchPic()));
			Zeilen2++;
		}
	}

	PicTable->adjustColumn(COL_FILENAME);
	PicTable->adjustColumn(COL_PATH);
	PicTable->adjustColumn(COL_PAGE);
	PicTable->adjustColumn(COL_PRINT);
	PicTable->adjustColumn(COL_STATUS);
	PicTable->setSorting(false);
	PicTable->setSelectionMode(QTable::NoSelection);
	PicTable->setColumnMovingEnabled(false);
	PicTable->setRowMovingEnabled(false);
	Header->setMovingEnabled(false);
	PicTable->setColumnReadOnly(COL_FILENAME, true);
	PicTable->setColumnReadOnly(COL_PATH, true);
	PicTable->setColumnReadOnly(COL_PAGE, true);
	PicTable->setColumnReadOnly(COL_STATUS, true);
	PicStatusLayout->addWidget( PicTable );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 3 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );

	OkB = new QPushButton( tr( "OK" ), this, "OkB" );
	Layout2->addWidget( OkB );
	PicStatusLayout->addLayout( Layout2 );

	// signals and slots connections
	connect( OkB, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

void PicStatus::languageChange()
{
	PicStatus::trOK = tr("OK");
	PicStatus::trMissing = tr("Missing");
	PicStatus::trSearch = tr("Search");
	PicStatus::trCancelSearch = tr("Cancel Search");
	PicStatus::trGoto = tr("Goto");
}

/*!
 \fn void PicStatus::GotoPic()
 \author Franz Schmid
 \date
 \brief Moves to the Page containing the selected Image.
 \param None
 \retval None
 */
void PicStatus::GotoPic()
{
	QString pageText = PicTable->text(QString(sender()->name()).toInt(), 2);
	bool ok = false;
	int pageNum = pageText.toInt(&ok);
	ScMW->closeActiveWindowMasterPageEditor();
	if (!ok)
		emit selectMasterPage(pageText);
	else
		emit selectPage(pageNum-1);
}

/*!
 \fn void PicStatus::SearchPic()
 \author Franz Schmid
 \date
 \brief Searches for the given Picture. Displays a Dialog when more than one Picture is found.
 \param None
 \retval None
 */
void PicStatus::SearchPic()
{
	// FIXME: This is a pretty ugly hack IMO - carried over from the old
	// SearchPic. Table handling needs work.
	QString workDir;
	QString searchBase;
	unsigned int row = QString(sender()->name()).toUInt();
	QString fileName = PicTable->text(row, 0);
#ifndef _WIN32
	workDir = QDir::homeDirPath();
#endif
	// Pictures may be located completely outside home or documents directory
	// so ask base search directory first
	searchBase = QFileDialog::getExistingDirectory( workDir, NULL, NULL, tr("Select a base directory for search"));
	if( searchBase.isEmpty() || !QDir().exists(searchBase) )
		return;
	// Set up the search, then return to the event loop until it notifies us
	// that it's done.
	// Note: search will be deleted when this PicStatus is, so there's no
	// need to worry about cleanup.
	FileSearch* search = new FileSearch(this, fileName, searchBase);
	Q_CHECK_PTR(search);
	connect(search,
			SIGNAL(searchComplete(const QStringList&, const QString&)),
			SLOT(SearchPicFinished(const QStringList&, const QString&)));
	connect(search, SIGNAL(aborted(bool)), SLOT(SearchPicAborted(bool)));
	// Set up the UI to let the user cancel the search, then start it
	setSearchButton(row, true, search);
	search->start();
}

void PicStatus::SearchPicAborted(bool userCancelled)
{
	const FileSearch* search = dynamic_cast<const FileSearch*>(sender());
	Q_ASSERT(search);
	// Restore button to normal "search"
	unsigned int row = getRowByFileName(search->fileName());
	setSearchButton(row, false, search);
	// and inform user if it it wasn't them who asked to stop it.
	if (!userCancelled)
		// A running search failed
		QMessageBox::warning(this, tr("Scribus - Image Search"),
				tr("The search failed: %1").arg(search->lastError()),
				QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
				QMessageBox::NoButton);
}

void PicStatus::SearchPicFinished(const QStringList & matches, const QString & fileName)
{
	const FileSearch* search = dynamic_cast<const FileSearch*>(sender());
	Q_ASSERT(search);
	// First, find out what row the search result is about
	unsigned int row = getRowByFileName(fileName);
	// Restore the gui to "search"
	setSearchButton(row, false, search);
	// Then display a dialog for the user to select images from
	// if one or more were found
	if (matches.count() == 0)
	{
		QMessageBox::information(this, tr("Scribus - Image Search"),
				tr("No images named \"%1\" were found.").arg(fileName),
				QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
				QMessageBox::NoButton);
	}
	else
	{
		PicSearch *dia = new PicSearch(this, fileName, matches);
		if (dia->exec())
		{
			Q_ASSERT(!dia->Bild.isEmpty());
			loadPictByRow(dia->Bild, row);
			view->DrawNew();
		}
		delete dia;
	}
}

bool PicStatus::loadPictByRow(const QString & newFilePath, unsigned int row)
{
	unsigned int itemNumber = ItemNrs[row];
	// FIXME: error checking
	doc->LoadPict(newFilePath, itemNumber);
	// WTF?
	bool isMaster = PicTable->cellWidget(row, COL_GOTO)->isEnabled();
	PageItem* item = isMaster ? doc->DocItems.at(itemNumber) : doc->MasterItems.at(itemNumber);
	// Set missing flag again. Since we do no error checking of the load,
	// missing will generally mean "failed to load".
	PicTable->setText(row, COL_STATUS, item->PicAvail ? trOK : trMissing);
	PicTable->setText(row, COL_PATH, QFileInfo(newFilePath).dirPath(true));
	return item->PicAvail;
}

void PicStatus::setSearchButton(int row, bool toCancel, const FileSearch* searcher)
{
	QWidget* item = PicTable->cellWidget(row, COL_SEARCH);
	Q_ASSERT(item);
	QToolButton* tb = dynamic_cast<QToolButton*>(item);
	Q_ASSERT(tb);
	if (toCancel)
	{
		tb->setText( trCancelSearch );
		disconnect(tb, SIGNAL(clicked()), this, SLOT(SearchPic()));
		connect(tb, SIGNAL(clicked()), searcher, SLOT(cancel()));
	}
	else
	{
		tb->setText(trSearch);
		disconnect(tb, SIGNAL(clicked()), searcher, SLOT(cancel()));
		connect(tb, SIGNAL(clicked()), SLOT(SearchPic()));
	}
}

int PicStatus::getRowByFileName(const QString & fileName)
{
	bool found = false;
	unsigned int row;
	// This cast is OK since numRows must logically return >=0
	for (row = 0; row < static_cast<unsigned int>(PicTable->numRows()); ++row)
	{
		if ( PicTable->text(row, 0) == fileName )
		{
			found = true;
			break;
		}
	}
	Q_ASSERT(found);
	return row;
}

/*!
 \fn void PicStatus::PrintPic()
 \author Franz Schmid
 \date
 \brief Enables or disables printing of the selected Image.
 \param None
 \retval None
 */
void PicStatus::PrintPic()
{
	uint ZNr = QString(sender()->name()).toUInt();
	uint ItNr = ItemNrs[ZNr];
//	uint PgNr = PicTable->text(ZNr, 2).toInt()-1;
	if (PicTable->cellWidget(ZNr, 3)->isEnabled())
		doc->DocItems.at(ItNr)->setPrintable(FlagsPic.at(ZNr)->isChecked());
	else
		doc->MasterItems.at(ItNr)->setPrintable(FlagsPic.at(ZNr)->isChecked());
}
