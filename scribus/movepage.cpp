#include <qdialog.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>

#include "movepage.h"
#include "movepage.moc"
extern QPixmap loadIcon(QString nam);

/*
 *  Constructs a MovePages which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
MovePages::MovePages( QWidget* parent, int currentPage, int maxPages, bool moving )
		: QDialog( parent, "MovePages", true, 0 )
{
	move = moving;	
	setCaption (move ? tr("Move Pages") : tr("Copy Page"));
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 10, 5 );
	fromToLayout = new QGridLayout;
	fromToLayout->setSpacing( 6 );
	fromToLayout->setMargin( 5 );
	moveQLabel = new QLabel( this, "moveQLabel" );
	moveQLabel->setText(move ? tr("Move Page(s):") : tr("Copy Page") + ":");
	fromToLayout->addWidget( moveQLabel, 0, 0);

	fromPageQSBox = new QSpinBox( 1, maxPages, 1, this, "fromPageQSBox" );
	fromPageQSBox->setValue( currentPage );
	fromToLayout->addWidget( fromPageQSBox, 0, 1);

	if (move)
	{
		toQLabel = new QLabel( tr("to:"), this, "toQLabel" );
		fromToLayout->addWidget( toQLabel, 0, 2);
		toPageQSBox = new QSpinBox( 1, maxPages, 1, this, "toPageQSBox" );
		toPageQSBox->setValue( currentPage );
		fromToLayout->addWidget( toPageQSBox, 0, 3);
	}

	mvWhereQCBox = new QComboBox( false, this, "mvWhereQCBox" );
	mvWhereQCBox->insertItem( tr("Before Page"));
	mvWhereQCBox->insertItem( tr("After Page"));
	mvWhereQCBox->insertItem( tr("At End"));
	mvWhereQCBox->setCurrentItem(2);
	fromToLayout->addMultiCellWidget( mvWhereQCBox, 1, 1, 0, 1 );
	mvWherePageQSBox = new QSpinBox( 1, maxPages, 1, this, "mvWherePageQSBox" );
	mvWherePageQSBox->setValue( currentPage );
	mvWherePageQSBox->setDisabled( true );
	fromToLayout->addWidget( mvWherePageQSBox, 1, 3 );
	fromToLayout->addColSpacing(0, moveQLabel->fontMetrics().width( tr( "Move Page(s):" )));
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
	okCancelLayout->addWidget(cancelQPButton);
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	if (move)
		connect( toPageQSBox, SIGNAL( valueChanged(int) ), this, SLOT( toChanged(int) ) );
	connect( mvWhereQCBox, SIGNAL( activated(int) ), this, SLOT( mvWherePageQSBoxDisable(int) ) );
	connect( fromPageQSBox, SIGNAL( valueChanged(int) ), this, SLOT( fromChanged(int) ) );
	connect( okQPButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelQPButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void MovePages::fromChanged(int pageNumber)
{
	if (move)
	{
		if (pageNumber > toPageQSBox->value())
			toPageQSBox->setValue(pageNumber);
		if ((pageNumber == 1) && (toPageQSBox->value() == toPageQSBox->maxValue()))
			toPageQSBox->setValue(toPageQSBox->maxValue()-1);
	}
}

void MovePages::toChanged(int pageNumber)
{
	if (pageNumber < fromPageQSBox->value())
		fromPageQSBox->setValue(pageNumber);
	if ((fromPageQSBox->value() == 1) && (pageNumber == toPageQSBox->maxValue()))
		fromPageQSBox->setValue(2);
}

void MovePages::mvWherePageQSBoxDisable(int index)
{
    mvWherePageQSBox->setDisabled((index==2));
}


const int MovePages::getFromPage()
{
	return fromPageQSBox->value();
}

const int MovePages::getToPage()
{
	return toPageQSBox->value();
}

const int MovePages::getWhere()
{
	return mvWhereQCBox->currentItem();
}

const int MovePages::getWherePage()
{
	return mvWherePageQSBox->value();
}

