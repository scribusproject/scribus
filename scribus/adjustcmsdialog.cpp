/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "adjustcmsdialog.h"
#include <QPushButton>
#include "commonstrings.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "util_icon.h"

AdjustCmsDialog::AdjustCmsDialog( QWidget* parent, ScribusDoc* doc ) : QDialog( parent )
{
	setWindowTitle( tr("CMS Settings") );
	setWindowIcon(loadIcon("AppIcon.png"));
	setModal(true);
	queryLayout = new QVBoxLayout( this );
	queryLayout->setMargin(5);
	queryLayout->setSpacing(10);
	tabColorManagement = new CMSPrefs(this, &doc->CMSSettings, &ScCore->InputProfiles, &ScCore->InputProfilesCMYK, &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
	queryLayout->addWidget( tabColorManagement );
	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this);
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	okCancelLayout->addWidget( cancelButton );
	queryLayout->addLayout( okCancelLayout );
	setMaximumSize(minimumSizeHint());
	resize( minimumSizeHint() );

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

}

