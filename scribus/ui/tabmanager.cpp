/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QPushButton>
#include "tabruler.h"
#include "units.h"
#include "scribusstructs.h"
#include "commonstrings.h"
#include "util_icon.h"

TabManager::TabManager( QWidget* parent, int dEin, QList<ParagraphStyle::TabRecord> inTab, double) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Manage Tabulators" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	docUnitRatio = unitGetRatioFromIndex(dEin);
	tmpTab = inTab;
	TabManagerLayout = new QVBoxLayout( this );
	TabManagerLayout->setMargin(5);
	TabManagerLayout->setSpacing(5);
	TabList = new Tabruler(this, false, dEin, inTab, -1);
	TabManagerLayout->addWidget( TabList );
	layout10 = new QHBoxLayout;
	layout10->setSpacing( 5 );
	layout10->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer );
	OKButton = new QPushButton( CommonStrings::tr_OK, this);
	OKButton->setDefault( true );
	layout10->addWidget( OKButton );
	CancelButton = new QPushButton( CommonStrings::tr_Cancel, this);
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
