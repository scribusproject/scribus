#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>

#include "margindialog.h"
#include "margindialog.moc"
#include "marginWidget.h"
#include "scribusstructs.h"
#include "page.h"
#include "scribusdoc.h"
#include "units.h"
#include "mspinbox.h"

extern QPixmap loadIcon(QString nam);

MarginDialog::MarginDialog( QWidget* parent, ScribusDoc* doc ) : QDialog( parent, "MarginDialog", true, 0 )
{
	setCaption( tr( "Manage Page Properties" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 10, 5);
	
	dsGroupBox7 = new QGroupBox( this, "GroupBox7" );
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7->setColumnLayout(0, Qt::Vertical );
	dsGroupBox7->layout()->setSpacing( 0 );
	dsGroupBox7->layout()->setMargin( 0 );
	dsGroupBox7Layout = new QGridLayout( dsGroupBox7->layout() );
	dsGroupBox7Layout->setAlignment( Qt::AlignTop );
	dsGroupBox7Layout->setSpacing( 5 );
	dsGroupBox7Layout->setMargin( 10 );
	widthMSpinBox = new MSpinBox( 1, 100000, dsGroupBox7, unitGetDecimalsFromIndex(doc->docUnitIndex) );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthMSpinBox->setSuffix(unitGetSuffixFromIndex(doc->docUnitIndex));
	widthMSpinBox->setValue(doc->currentPage->Width * doc->unitRatio);
	widthQLabel->setBuddy(widthMSpinBox);
	dsGroupBox7Layout->addWidget( widthQLabel, 0, 0 );
	dsGroupBox7Layout->addWidget( widthMSpinBox, 0, 1 );
	heightMSpinBox = new MSpinBox( 1, 100000, dsGroupBox7, unitGetDecimalsFromIndex(doc->docUnitIndex) );
	heightMSpinBox->setSuffix(unitGetSuffixFromIndex(doc->docUnitIndex));
	heightMSpinBox->setValue(doc->currentPage->Height * doc->unitRatio);
	heightQLabel = new QLabel(heightMSpinBox,  tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	dsGroupBox7Layout->addWidget( heightQLabel, 0, 2 );
	dsGroupBox7Layout->addWidget( heightMSpinBox, 0, 3 );
	dialogLayout->addWidget( dsGroupBox7 );
	
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
