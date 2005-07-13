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
extern ScribusApp* ScApp;

const unsigned short PicStatus::COL_FILENAME = 0;
const unsigned short PicStatus::COL_PATH = 1;
const unsigned short PicStatus::COL_PAGE = 2;
const unsigned short PicStatus::COL_GOTO = 3;
const unsigned short PicStatus::COL_PRINT = 4;
const unsigned short PicStatus::COL_STATUS = 5;
const unsigned short PicStatus::COL_SEARCH = 6;

const QString PicStatus::trOK = PicStatus::tr("OK");
const QString PicStatus::trMissing = PicStatus::tr("Missing");
const QString PicStatus::trSearch = PicStatus::tr("Search");
const QString PicStatus::trCancelSearch = PicStatus::tr("Cancel Search");
const QString PicStatus::trGoto = PicStatus::tr("Goto");

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
	QDialog( parent, "pic", true, 0 ),
	m_search(0)
{
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
	for (i=0; i<doc->Items.count(); ++i)
	{
		if (doc->Items.at(i)->itemType() == PageItem::ImageFrame)
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
	for (i=0; i< doc->Items.count(); ++i)
	{
		if (doc->Items.at(i)->itemType() == PageItem::ImageFrame)
		{
			QFileInfo fi = QFileInfo(doc->Items.at(i)->Pfile);
			PicTable->setText(Zeilen2, COL_FILENAME, fi.fileName());
			PicTable->setText(Zeilen2, COL_PATH, fi.dirPath());
			p = doc->Items.at(i)->OwnPage;
			PicTable->setText(Zeilen2, COL_PAGE, tmp.setNum(p+1));
			QToolButton *tb2 = new QToolButton(this, tmp.setNum(Zeilen2));
			tb2->setText( trGoto);
			tb2->setEraseColor(white);
			PicTable->setColumnWidth(COL_GOTO, tb2->fontMetrics().width( trGoto)+10);
			PicTable->setCellWidget(Zeilen2, COL_GOTO, tb2);
			connect(tb2, SIGNAL(clicked()), this, SLOT(GotoPic()));
			QCheckBox *cp2 = new QCheckBox(this, tmp.setNum(Zeilen2));
			cp2->setText( tr("Yes"));
			cp2->setChecked(doc->Items.at(i)->printable());
			cp2->setEraseColor(white);
			FlagsPic.append(cp2);
			PicTable->setCellWidget(Zeilen2, COL_PRINT, cp2);
			connect(cp2, SIGNAL(clicked()), this, SLOT(PrintPic()));
			if (doc->Items.at(i)->PicAvail)
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
	if (doc->masterPageMode)
		ScApp->ActWin->muster->close();
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
	unsigned int row = QString(sender()->name()).toUInt();
	QString fileName = PicTable->text(row, 0);
	// Set up the search, then return to the event loop until it notifies us
	// that it's done.
	// Note: m_search will be deleted when this PicStatus is, so there's no
	// need to worry about cleanup.
	m_search = new FileSearch(this, fileName);
	Q_CHECK_PTR(m_search);
	connect(m_search,
			SIGNAL(searchComplete(const QStringList&, const QString&)),
			SLOT(SearchPicFinished(const QStringList&, const QString&)));
	connect(m_search, SIGNAL(searchAborted(bool)), SLOT(SearchPicAborted(bool)));
	// Set up the UI to let the user cancel the search, then start it
	setSearchButton(row, true, m_search);
	m_search->start();
}

void PicStatus::SearchPicAborted(bool userCancelled)
{
	// Restore button to normal "search"
	unsigned int row = getRowByFileName(m_search->fileName());
	setSearchButton(row, false, m_search);
	// and inform user if it it wasn't them who asked to stop it.
	if (!userCancelled)
		// A running search failed
		QMessageBox::warning(this, tr("Scribus - Image Search"),
				tr("The search failed: %1").arg(m_search->failReason()),
				QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
				QMessageBox::NoButton);
}

void PicStatus::SearchPicFinished(const QStringList & matches, const QString & fileName)
{
	// First, find out what row the search result is about
	unsigned int ZNr = getRowByFileName(fileName);
	// Restore the gui to "search"
	setSearchButton(ZNr, false, m_search);
	// Then display a dialog for the user to select images from
	// if one or more were found
	uint ItNr = ItemNrs[ZNr];
	uint PgNr = PicTable->text(ZNr, 2).toInt()-1;
	QString BildNam = PicTable->text(ZNr, 0);
	QString OldPfad = PicTable->text(ZNr, 1);
	QStringList Pfade = matches;
	if (Pfade.count() > 0)
	{
		PicSearch *dia = new PicSearch(this, BildNam, Pfade);
		if (dia->exec())
		{
			QString fileName = dia->Bild;
			if (!fileName.isEmpty())
			{
				for (uint zz = 0; zz < static_cast<uint>(Zeilen); ++zz)
				{
					if (PicTable->text(zz, 1) == OldPfad)
					{
						PgNr = PicTable->text(zz, 2).toInt()-1;
						ItNr = ItemNrs[zz];
						if (PicTable->cellWidget(zz, 3)->isEnabled())
						{
							view->LoadPict(fileName, ItNr);
							PicTable->setText(zz, 1, doc->Items.at(ItNr)->Pfile);
							PicTable->setText(zz, 5, doc->Items.at(ItNr)->PicAvail ?
							                  trOK : trMissing);
						}
						else
						{
							view->LoadPict(fileName, ItNr);
							PicTable->setText(zz, 1, doc->MasterItems.at(ItNr)->Pfile);
							PicTable->setText(zz, 5, doc->MasterItems.at(ItNr)->PicAvail ?
							                  trOK : trMissing);
						}
					}
				}
				view->DrawNew();
			}
		}
		delete dia;
	}
	else
	{
		if (Pfade.count() == 1)
		{
			for (uint zz = 0; zz < static_cast<uint>(Zeilen); ++zz)
			{
				if (PicTable->text(zz, 1) == OldPfad)
				{
					PgNr = PicTable->text(zz, 2).toInt()-1;
					ItNr = ItemNrs[zz];
					if (PicTable->cellWidget(zz, 3)->isEnabled())
					{
						view->LoadPict(Pfade[0], ItNr);
						PicTable->setText(zz, 1, doc->Items.at(ItNr)->Pfile);
						PicTable->setText(zz, 5, doc->Items.at(ItNr)->PicAvail ?
						                  trOK : trMissing);
					}
					else
					{
						view->LoadPict(Pfade[0], ItNr);
						PicTable->setText(zz, 1, doc->MasterItems.at(ItNr)->Pfile);
						PicTable->setText(zz, 5, doc->MasterItems.at(ItNr)->PicAvail ?
						                  trOK : trMissing);
					}
				}
			}
			view->DrawNew();
		}
	}
}

void PicStatus::setSearchButton(int row, bool toCancel, FileSearch* searcher)
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
		doc->Items.at(ItNr)->setPrintable(FlagsPic.at(ZNr)->isChecked());
	else
		doc->MasterItems.at(ItNr)->setPrintable(FlagsPic.at(ZNr)->isChecked());
}
