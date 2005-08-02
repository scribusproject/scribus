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
	OKButton = new QPushButton( tr( "&OK" ), this, "OKButton" );
	OKButton->setDefault( true );
	layout10->addWidget( OKButton );
	CancelButton = new QPushButton( tr( "&Cancel" ), this, "CancelB" );
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
