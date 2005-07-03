/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "pageselector.h"
#include "pageselector.moc"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

extern QPixmap loadIcon(QString nam);

PageSelector::PageSelector( QWidget* parent, int maxPg ) : QWidget( parent, "pgsel", 0 )
{
	LastPG = maxPg;
	APage = 1;
	PageSelectorLayout = new QHBoxLayout( this, 0, 1, "PageSelectorLayout");

	Start = new QPushButton( this, "Start" );
	Start->setDefault( false );
	Start->setAutoDefault( false );
	Start->setPixmap( loadIcon("start.png") );
	Start->setFocusPolicy(QWidget::NoFocus);
	PageSelectorLayout->addWidget( Start );

	Back = new QPushButton( this, "Back" );
	Back->setPixmap( loadIcon("back.png") );
	Back->setDefault( false );
	Back->setAutoDefault( false );
	Back->setFocusPolicy(QWidget::NoFocus);
	PageSelectorLayout->addWidget( Back );

	Label1 = new QLabel( this, "Label1" );
	Label1->setText( tr( "Page " ) );
	PageSelectorLayout->addWidget( Label1 );
	v = new QIntValidator(1, LastPG, this);
	PageCombo = new QComboBox( true, this, "PageCombo" );
	PageCombo->setEditable( true );
	PageCombo->setDuplicatesEnabled( false );
	QString tmp;
	for (int a = 0; a < LastPG; ++a)
	{
		PageCombo->insertItem(tmp.setNum(a+1));
	}
	PageCombo->setValidator(v);
	PageCombo->setFocusPolicy(QWidget::ClickFocus);
	PageSelectorLayout->addWidget( PageCombo );

	Label2 = new QLabel( this, "Label2" );
	Label2->setText( tr( " of %1" ).arg(LastPG) );
	PageSelectorLayout->addWidget( Label2 );

	Forward = new QPushButton( this, "Forward" );
	Forward->setPixmap( loadIcon("forward.png") );
	Forward->setDefault( false );
	Forward->setAutoDefault( false );
	Forward->setFocusPolicy(QWidget::NoFocus);
	PageSelectorLayout->addWidget( Forward );

	Last = new QPushButton( this, "Last" );
	Last->setPixmap( loadIcon("finish.png") );
	Last->setDefault( false );
	Last->setAutoDefault( false );
	Last->setFocusPolicy(QWidget::NoFocus);
	PageSelectorLayout->addWidget( Last );
	Forward->setEnabled(true);
	Last->setEnabled(true);
	Back->setEnabled(false);
	Start->setEnabled(false);
	if (APage == LastPG)
	{
		Forward->setEnabled(false);
		Last->setEnabled(false);
	}

	// signals and slots connections
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	connect( Back, SIGNAL( clicked() ), this, SLOT( goBk() ) );
	connect( Start, SIGNAL( clicked() ), this, SLOT( ToStart() ) );
	connect( Forward, SIGNAL( clicked() ), this, SLOT( goFw() ) );
	connect( Last, SIGNAL( clicked() ), this, SLOT( ToEnd() ) );
}

void PageSelector::GotoPgE(int a)
{
	GotoPg(a);
	emit GotoPage(a+1);
}

void PageSelector::GotoPg(int a)
{
	disconnect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	PageCombo->setCurrentItem(a);
	APage = a+1;
	Back->setEnabled(true);
	Start->setEnabled(true);
	Forward->setEnabled(true);
	Last->setEnabled(true);
	if (a == 0)
	{
		Back->setEnabled(false);
		Start->setEnabled(false);
	}
	if (a == LastPG-1)
	{
		Forward->setEnabled(false);
		Last->setEnabled(false);
	}
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
}

void PageSelector::setMaxValue(int a)
{
	disconnect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	LastPG = a;
	QString tmp;
	PageCombo->clear();
	v->setTop(LastPG);
	for (int b = 0; b < LastPG; ++b)
	{
		PageCombo->insertItem(tmp.setNum(b+1));
	}
	Label2->setText( tr( " of %1" ).arg(LastPG) );
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
}

void PageSelector::ToStart()
{
	if (APage == 1)
		return;
	GotoPgE(0);
}

void PageSelector::ToEnd()
{
	if (APage == LastPG-1)
		return;
	GotoPgE(LastPG-1);
}

void PageSelector::goBk()
{
	APage--;
	if (APage < 1)
		APage = 1;
	GotoPgE(APage-1);
}

void PageSelector::goFw()
{
	APage++;
	if (APage > LastPG)
		APage = LastPG;
	GotoPgE(APage-1);
}
