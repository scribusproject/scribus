/****************************************************************************
** Form implementation generated from reading ui file 'tabmanager.ui'
**
** Created: Don Jan 1 12:57:21 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "tabmanager.h"
#include "tabmanager.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "tabruler.h"
extern QPixmap loadIcon(QString nam);

TabManager::TabManager( QWidget* parent, int dEin, QValueList<double> inTab, double wid) : QDialog( parent, "TabManager", true, 0 )
{
	setCaption( tr( "Manage Tabulators" ) );
	setIcon(loadIcon("AppIcon.png"));
	tmpTab = inTab;
	TabManagerLayout = new QVBoxLayout( this, 5, 5, "TabManagerLayout");
	TabList = new Tabruler(this, false, dEin, inTab, wid);
	TabManagerLayout->addWidget( TabList );
	layout10 = new QHBoxLayout;
	layout10->setSpacing( 6 );
	layout10->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer );
	OKButton = new QPushButton( tr( "&OK" ), this, "OKButton" );
	OKButton->setDefault( true );
	layout10->addWidget( OKButton );
	QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer_2 );
	CancelButton = new QPushButton( tr( "&Cancel" ), this, "CancelB" );
	layout10->addWidget( CancelButton );
	QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer_3 );
	TabManagerLayout->addLayout( layout10 );
	resize( minimumSizeHint() );
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( exitOK() ) );
	connect( CancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void TabManager::exitOK()
{
	tmpTab = TabList->getTabVals();
	accept();
}
