#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qspinbox.h>


#include "delpages.h"
#include "delpages.moc"
extern QPixmap loadIcon(QString nam);

/*
 *  Constructs a DelPages which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DelPages::DelPages( QWidget* parent, int currentPage, int maxPage )
		: QDialog( parent, "DelPages", true, 0 )
{
	setCaption( tr( "Delete Pages" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 10, 5);
	fromToLayout = new QHBoxLayout;
	fromToLayout->setSpacing( 6 );
	fromToLayout->setMargin( 5 );
	fromQLabel = new QLabel( tr( "Delete From:" ), this, "fromQLabel" );
	fromToLayout->addWidget( fromQLabel );
	fromPageQSBox = new QSpinBox( 1, maxPage, 1, this, "fromPageQSBox" );
	fromPageQSBox->setValue( currentPage );
	fromToLayout->addWidget( fromPageQSBox );
	toQLabel = new QLabel( this, "toQLabel" );
	toQLabel->setText( tr( "to:" ) );
	fromToLayout->addWidget( toQLabel );
	toPageQSBox = new QSpinBox( 1, maxPage, 1, this, "toPageQSBox" );
	toPageQSBox->setValue( currentPage );
	fromToLayout->addWidget( toPageQSBox );
	dialogLayout->addLayout( fromToLayout );

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okQPButton = new QPushButton( tr( "&OK" ), this, "okQPButton" );
	okQPButton->setDefault( true );
	okCancelLayout->addWidget(okQPButton);
	cancelQPButton = new QPushButton( tr( "&Cancel" ), this, "cancelQPButton" );
	cancelQPButton->setDefault( false );
	okCancelLayout->addWidget(cancelQPButton);
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( okQPButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelQPButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( fromPageQSBox, SIGNAL( valueChanged(int) ), this, SLOT( fromChanged(int) ) );
	connect( toPageQSBox, SIGNAL( valueChanged(int) ), this, SLOT( toChanged(int) ) );
}

void DelPages::fromChanged(int pageNumber)
{
	if (pageNumber > toPageQSBox->value())
		toPageQSBox->setValue(pageNumber);
	if ((pageNumber == 1) && (toPageQSBox->value() == toPageQSBox->maxValue()))
		toPageQSBox->setValue(toPageQSBox->maxValue()-1);
}

void DelPages::toChanged(int pageNumber)
{
	if (pageNumber < fromPageQSBox->value())
		fromPageQSBox->setValue(pageNumber);
	if ((fromPageQSBox->value() == 1) && (pageNumber == toPageQSBox->maxValue()))
		fromPageQSBox->setValue(2);
}

const int DelPages::getFromPage()
{
    return fromPageQSBox->value();
}

const int DelPages::getToPage()
{
    return toPageQSBox->value();
}

