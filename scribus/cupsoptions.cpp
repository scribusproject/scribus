/***************************************************************************
                          cupsoptions.cpp  -  description
                             -------------------
    begin                : Fre Jan 3 2003
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

#include "cupsoptions.h"
#include "cupsoptions.moc"

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include <qstringlist.h>
#ifdef HAVE_CUPS
#include <cups/cups.h>
#endif
#include <qtooltip.h>
extern QPixmap loadIcon(QString nam);

CupsOptions::CupsOptions(QWidget* parent, QString Geraet) : QDialog( parent, "prin", true, 0 )
{
	FlagsOpt.clear();
	setCaption( tr( "Printer Options" ) );
 	setIcon(loadIcon("AppIcon.png"));
	setSizeGripEnabled(true);
	CupsOptionsLayout = new QVBoxLayout( this );
	CupsOptionsLayout->setSpacing( 5 );
	CupsOptionsLayout->setMargin( 10 );
	Table = new QTable( this, "Table1" );
	Table->setNumCols( 2 );
	Table->setSorting(false);
	Table->setSelectionMode(QTable::NoSelection);
	Table->setLeftMargin(0);
	Table->verticalHeader()->hide();
	Table->setMinimumSize(300, 100);
#ifdef HAVE_CUPS
	int i;
	cups_dest_t *dests;
	cups_dest_t *dest;
	int num_dests;
	const char	*filename;	/* PPD filename */
	ppd_file_t	*ppd;				/* PPD data */
	ppd_group_t	*group;			/* Current group */
	num_dests = cupsGetDests(&dests);
	dest = cupsGetDest(Geraet, NULL, num_dests, dests);
	if (dest == NULL || (filename = cupsGetPPD(dest->name)) == NULL || 
		  (ppd = ppdOpenFile(filename)) == NULL)
		return;
	ppdMarkDefaults(ppd);
	cupsMarkOptions(ppd, dest->num_options, dest->options);
	QStringList opts;
	QString Marked = "";
	KeyToText.clear();
	KeyToDefault.clear();
	for (i = ppd->num_groups, group = ppd->groups; i > 0; i --, ++group)
	{
		int ix;
		ppd_option_t	*option;	/* Current option */
		ppd_choice_t	*choice;	/* Current choice */
  		for (ix = group->num_options, option = group->options; ix > 0; ix --, ++option)
  		{
			int j;
			Marked = "";
			struct OpData Daten;
			opts.clear();
			for (j = option->num_choices, choice = option->choices; 
					j > 0; j --, ++choice)
			{
				opts.append(QString(choice->choice));
				if (choice->marked)
					Marked = QString(choice->choice);
  			}
			if (Marked != "")
			{
				Table->setNumRows(Table->numRows()+1);
				Table->setText(Table->numRows()-1, 0, QString(option->text));
    			QComboBox *item = new QComboBox( true, this, "Replace" );
				item->setEditable(false);
    			FlagsOpt.append(item);
				Daten.Cnum = static_cast<int>(FlagsOpt.count()-1);
				Daten.KeyW = QString(option->keyword);
				KeyToText[QString(option->text)] = Daten;
				item->insertStringList(opts);
				item->setCurrentText(Marked);
				KeyToDefault[QString(option->text)] = Marked;
    			Table->setCellWidget(Table->numRows()-1, 1, item);
			}
		}
	}
	ppdClose(ppd);
	cupsFreeDests(num_dests, dests);
	struct OpData Daten;

	Table->setNumRows(Table->numRows()+1);
	Table->setText(Table->numRows()-1, 0, QString( tr("Page Set")));
	QComboBox *item4 = new QComboBox( true, this, "Replace" );
	item4->setEditable(false);
	FlagsOpt.append(item4);
	Daten.Cnum = static_cast<int>(FlagsOpt.count()-1);
	Daten.KeyW = "page-set";
	KeyToText["Page Set"] = Daten;
	item4->insertItem( tr("All Pages"));
	item4->insertItem( tr("Even Pages only"));
	item4->insertItem( tr("Odd Pages only"));
	item4->setCurrentItem(0);
	KeyToDefault["Page Set"] = tr("All Pages");
	Table->setCellWidget(Table->numRows()-1, 1, item4);
	Table->setNumRows(Table->numRows()+1);
	Table->setText(Table->numRows()-1, 0, QString( tr("Mirror")));
	QComboBox *item2 = new QComboBox( true, this, "Replace" );
	item2->setEditable(false);
	FlagsOpt.append(item2);
	Daten.Cnum = static_cast<int>(FlagsOpt.count()-1);
	Daten.KeyW = "mirror";
	KeyToText["Mirror"] = Daten;
	item2->insertItem( tr("No"));
	item2->insertItem( tr("Yes"));
	item2->setCurrentItem(0);
	KeyToDefault["Mirror"] = tr("No");
	Table->setCellWidget(Table->numRows()-1, 1, item2);
	Table->setNumRows(Table->numRows()+1);
	Table->setText(Table->numRows()-1, 0, QString( tr("Orientation")));
	QComboBox *item5 = new QComboBox( true, this, "Replace" );
	item5->setEditable(false);
	FlagsOpt.append(item5);
	Daten.Cnum = static_cast<int>(FlagsOpt.count()-1);
	Daten.KeyW = "orientation";
	KeyToText["Orientation"] = Daten;
	item5->insertItem( tr("Portrait"));
	item5->insertItem( tr("Landscape"));
	item5->setCurrentItem(0);
	KeyToDefault["Orientation"] = tr("Portrait");
	Table->setCellWidget(Table->numRows()-1, 1, item5);
	Table->setNumRows(Table->numRows()+1);
	Table->setText(Table->numRows()-1, 0, QString( tr("N-Up Printing")));
	QComboBox *item3 = new QComboBox( true, this, "Replace" );
	item3->setEditable(false);
	FlagsOpt.append(item3);
	Daten.Cnum = static_cast<int>(FlagsOpt.count()-1);
	Daten.KeyW = "number-up";
	KeyToText["N-Up Printing"] = Daten;
	item3->insertItem("1 "+ tr("Page per Sheet"));
	item3->insertItem("2 "+ tr("Pages per Sheet"));
	item3->insertItem("4 "+ tr("Pages per Sheet"));
	item3->insertItem("6 "+ tr("Pages per Sheet"));
	item3->insertItem("9 "+ tr("Pages per Sheet"));
	item3->insertItem("16 "+ tr("Pages per Sheet"));
	item3->setCurrentItem(0);
	KeyToDefault["N-Up Printing"] = "1 "+ tr("Page per Sheet");
	Table->setCellWidget(Table->numRows()-1, 1, item3);
#endif
	Table->adjustColumn(0);
	Table->setColumnStretchable(1, true);
	Table->adjustColumn(1);
	Header = Table->horizontalHeader();
	Header->setLabel(0, tr("Option"));
	Header->setLabel(1, tr("Value"));
	CupsOptionsLayout->addWidget( Table );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );
	PushButton1 = new QPushButton( tr("&OK"), this, "PushButton1" );
	PushButton1->setDefault( true );
	Layout2->addWidget( PushButton1 );
	PushButton2 = new QPushButton( tr("&Cancel"), this, "PushButton1_2" );
	PushButton2->setDefault( false );
	PushButton1->setFocus();
	Layout2->addWidget( PushButton2 );
	CupsOptionsLayout->addLayout( Layout2 );
	setMinimumSize( sizeHint() );
	resize(minimumSizeHint());

//tooltips
	QToolTip::add( Table, tr( "This panel displays various CUPS options when printing. \nThe exact parameters available will depend on your printer driver.\nYou can confirm CUPS support by selecting Help > About.\nLook for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.\nMissing library support is indicated by a *" ) );

    // signals and slots connections
	connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

