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
extern QPixmap loadIcon(QString nam);

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

PicStatus::PicStatus(QWidget* parent, ScribusDoc *docu, ScribusView *viewi)
		: QDialog( parent, "pic", true, 0 )
{
	uint p, i;
	QString tmp;
	setCaption( tr( "Pictures" ) );
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
	for (p=0; p<view->MasterPages.count(); ++p)
	{
		for (i=0; i<view->MasterPages.at(p)->Items.count(); ++i)
		{
			if (view->MasterPages.at(p)->Items.at(i)->PType == 2)
			{
				Zeilen++;
				ItemNrs.append(i);
			}
		}
	}
	for (p=0; p<view->Pages.count(); ++p)
	{
		for (i=0; i<view->Pages.at(p)->Items.count(); ++i)
		{
			if (view->Pages.at(p)->Items.at(i)->PType == 2)
			{
				Zeilen++;
				ItemNrs.append(i);
			}
		}
	}
	PicTable->setNumRows(Zeilen);
	int Zeilen2 = 0;
	for (p=0; p<view->MasterPages.count(); ++p)
	{
		for (i=0; i<view->MasterPages.at(p)->Items.count(); ++i)
		{
			if (view->MasterPages.at(p)->Items.at(i)->PType == 2)
			{
				QFileInfo fi = QFileInfo(view->MasterPages.at(p)->Items.at(i)->Pfile);
				PicTable->setText(Zeilen2, 0, fi.fileName());
				PicTable->setText(Zeilen2, 1, fi.dirPath());
				PicTable->setText(Zeilen2, 2, tmp.setNum(p+1));
				QToolButton *tb2 = new QToolButton(this, tmp.setNum(Zeilen2));
				tb2->setText( tr("Goto"));
				tb2->setEraseColor(white);
				tb2->setEnabled(false);
				PicTable->setColumnWidth(3, tb2->fontMetrics().width( tr("Goto"))+10);
				PicTable->setCellWidget(Zeilen2, 3, tb2);
				connect(tb2, SIGNAL(clicked()), this, SLOT(GotoPic()));
				QCheckBox *cp2 = new QCheckBox(this, tmp.setNum(Zeilen2));
				cp2->setText( tr("Yes"));
				cp2->setChecked(view->MasterPages.at(p)->Items.at(i)->isPrintable);
				cp2->setEraseColor(white);
				FlagsPic.append(cp2);
				PicTable->setCellWidget(Zeilen2, 4, cp2);
				connect(cp2, SIGNAL(clicked()), this, SLOT(PrintPic()));
				if (view->MasterPages.at(p)->Items.at(i)->PicAvail)
					PicTable->setText(Zeilen2, 5, tr("OK"));
				else
					PicTable->setText(Zeilen2, 5, tr("Missing"));
				QToolButton *tb = new QToolButton(this, tmp.setNum(Zeilen2));
				tb->setText( tr("Search"));
				PicTable->setColumnWidth(6, tb2->fontMetrics().width( tr("Search"))+10);
				tb->setEraseColor(white);
				PicTable->setCellWidget(Zeilen2, 6, tb);
				connect(tb, SIGNAL(clicked()), this, SLOT(SearchPic()));
				Zeilen2++;
			}
		}
	}
	for (p=0; p<view->Pages.count(); ++p)
	{
		for (i=0; i<view->Pages.at(p)->Items.count(); ++i)
		{
			if (view->Pages.at(p)->Items.at(i)->PType == 2)
			{
				QFileInfo fi = QFileInfo(view->Pages.at(p)->Items.at(i)->Pfile);
				PicTable->setText(Zeilen2, 0, fi.fileName());
				PicTable->setText(Zeilen2, 1, fi.dirPath());
				PicTable->setText(Zeilen2, 2, tmp.setNum(p+1));
				QToolButton *tb2 = new QToolButton(this, tmp.setNum(Zeilen2));
				tb2->setText( tr("Goto"));
				tb2->setEraseColor(white);
				PicTable->setColumnWidth(3, tb2->fontMetrics().width( tr("Goto"))+10);
				PicTable->setCellWidget(Zeilen2, 3, tb2);
				connect(tb2, SIGNAL(clicked()), this, SLOT(GotoPic()));
				QCheckBox *cp2 = new QCheckBox(this, tmp.setNum(Zeilen2));
				cp2->setText( tr("Yes"));
				cp2->setChecked(view->Pages.at(p)->Items.at(i)->isPrintable);
				cp2->setEraseColor(white);
				FlagsPic.append(cp2);
				PicTable->setCellWidget(Zeilen2, 4, cp2);
				connect(cp2, SIGNAL(clicked()), this, SLOT(PrintPic()));
				if (view->Pages.at(p)->Items.at(i)->PicAvail)
					PicTable->setText(Zeilen2, 5, tr("OK"));
				else
					PicTable->setText(Zeilen2, 5, tr("Missing"));
				QToolButton *tb = new QToolButton(this, tmp.setNum(Zeilen2));
				tb->setText( tr("Search"));
				PicTable->setColumnWidth(6, tb2->fontMetrics().width( tr("Search"))+10);
				tb->setEraseColor(white);
				PicTable->setCellWidget(Zeilen2, 6, tb);
				connect(tb, SIGNAL(clicked()), this, SLOT(SearchPic()));
				Zeilen2++;
			}
		}
	}

	PicTable->adjustColumn(0);
	PicTable->adjustColumn(1);
	PicTable->adjustColumn(2);
	PicTable->adjustColumn(4);
	PicTable->adjustColumn(5);
	PicTable->setSorting(false);
	PicTable->setSelectionMode(QTable::NoSelection);
	PicTable->setColumnMovingEnabled(false);
	PicTable->setRowMovingEnabled(false);
	Header->setMovingEnabled(false);
	PicStatusLayout->addWidget( PicTable );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );

	OkB = new QPushButton( tr( "&OK" ), this, "OkB" );
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
	emit GotoSeite(PicTable->text(QString(sender()->name()).toInt(), 2).toInt()-1);
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
	uint ZNr = QString(sender()->name()).toUInt();
	uint ItNr = ItemNrs[ZNr];
	uint PgNr = PicTable->text(ZNr, 2).toInt()-1;
	QString BildNam =	PicTable->text(ZNr, 0);
	QString OldPfad =	PicTable->text(ZNr, 1);
	QStringList Pfade;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	qApp->processEvents();
	QString home = QDir::homeDirPath();
	FILE *fp = popen("find "+home+" -name " + BildNam, "r");
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	if (fp == NULL)
		return;
	QTextStream ts(fp, IO_ReadOnly);
	QString tmp = ts.read();
	Pfade = QStringList::split("\n", tmp);
	if (Pfade.count() > 1)
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
							view->Pages.at(PgNr)->LoadPict(fileName, ItNr);
							PicTable->setText(zz, 1, view->Pages.at(PgNr)->Items.at(ItNr)->Pfile);
							PicTable->setText(zz, 5, view->Pages.at(PgNr)->Items.at(ItNr)->PicAvail ?
							                  tr("OK") : tr("Missing"));
						}
						else
						{
							view->MasterPages.at(PgNr)->LoadPict(fileName, ItNr);
							PicTable->setText(zz, 1, view->MasterPages.at(PgNr)->Items.at(ItNr)->Pfile);
							PicTable->setText(zz, 5, view->Pages.at(PgNr)->Items.at(ItNr)->PicAvail ?
							                  tr("OK") : tr("Missing"));
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
						view->Pages.at(PgNr)->LoadPict(Pfade[0], ItNr);
						PicTable->setText(zz, 1, view->Pages.at(PgNr)->Items.at(ItNr)->Pfile);
						PicTable->setText(zz, 5, view->Pages.at(PgNr)->Items.at(ItNr)->PicAvail ?
						                  tr("OK") : tr("Missing"));
					}
					else
					{
						view->MasterPages.at(PgNr)->LoadPict(Pfade[0], ItNr);
						PicTable->setText(zz, 1, view->MasterPages.at(PgNr)->Items.at(ItNr)->Pfile);
						PicTable->setText(zz, 5, view->Pages.at(PgNr)->Items.at(ItNr)->PicAvail ?
						                  tr("OK") : tr("Missing"));
					}
				}
			}
			view->DrawNew();
		}
	}
	pclose(fp);
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
	uint PgNr = PicTable->text(ZNr, 2).toInt()-1;
	if (PicTable->cellWidget(ZNr, 3)->isEnabled())
		view->Pages.at(PgNr)->Items.at(ItNr)->isPrintable = FlagsPic.at(ZNr)->isChecked();
	else
		view->MasterPages.at(PgNr)->Items.at(ItNr)->isPrintable = FlagsPic.at(ZNr)->isChecked();
}
