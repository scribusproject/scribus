/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabmanager.h"
#include "tabmanager.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "tabruler.h"
#include "units.h"
#include "scribusstructs.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

TabManager::TabManager( QWidget* parent, int dEin, QValueList<PageItem::TabRecord> inTab, double) : QDialog( parent, "TabManager", true, 0 )
{
	setCaption( tr( "Manage Tabulators" ) );
	setIcon(loadIcon("AppIcon.png"));
	docUnitRatio=unitGetRatioFromIndex(dEin);
	tmpTab = inTab;
	TabManagerLayout = new QVBoxLayout( this, 5, 5, "TabManagerLayout");
	TabList = new Tabruler(this, false, dEin, inTab, -1);
	TabManagerLayout->addWidget( TabList );
	layout10 = new QHBoxLayout;
	layout10->setSpacing( 6 );
	layout10->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer );
	OKButton = new QPushButton( CommonStrings::tr_OK, this, "OKButton" );
	OKButton->setDefault( true );
	layout10->addWidget( OKButton );
	CancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "CancelB" );
	layout10->addWidget( CancelButton );
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
