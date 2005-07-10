#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include "margindialog.h"
#include "margindialog.moc"
#include "marginWidget.h"
#include "scribusstructs.h"
#include "page.h"
#include "scribusdoc.h"
#include "units.h"

extern QPixmap loadIcon(QString nam);

MarginDialog::MarginDialog( QWidget* parent, ScribusDoc* doc ) : QDialog( parent, "MarginDialog", true, 0 )
{
	setCaption( tr( "Manage Margins" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 10, 5);
	GroupRand = new MarginWidget(this,  tr( "Margin Guides" ), &doc->currentPage->Margins, unitGetDecimalsFromIndex(doc->docUnitIndex), doc->unitRatio, unitGetSuffixFromIndex(doc->docUnitIndex));
	GroupRand->setPageHeight(doc->currentPage->Height * doc->unitRatio);
	GroupRand->setPageWidth(doc->currentPage->Width * doc->unitRatio);
	GroupRand->setFacingPages(doc->PageFP );
	dialogLayout->addWidget( GroupRand );

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( tr( "&OK" ), this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget(okButton);
	cancelButton = new QPushButton( tr( "&Cancel" ), this, "cancelButton" );
	cancelButton->setDefault( false );
	okCancelLayout->addWidget(cancelButton);
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}
