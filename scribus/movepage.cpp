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
	moveLabel = new QLabel( (move ? tr("Move Page(s)") : tr("Copy Page")) + ":", this, "moveLabel" );
	fromToLayout->addWidget( moveLabel, 0, 0);

	fromPageData = new QSpinBox( 1, maxPages, 1, this, "fromPageData" );
	fromPageData->setValue( currentPage );
	fromToLayout->addWidget( fromPageData, 0, 1);

	if (move)
	{
		toLabel = new QLabel( tr("to:"), this, "toLabel" );
		fromToLayout->addWidget( toLabel, 0, 2);
		toPageData = new QSpinBox( 1, maxPages, 1, this, "toPageData" );
		toPageData->setValue( currentPage );
		fromToLayout->addWidget( toPageData, 0, 3);
	}

	mvWhereData = new QComboBox( false, this, "mvWhereData" );
	mvWhereData->insertItem( tr("Before Page"));
	mvWhereData->insertItem( tr("After Page"));
	mvWhereData->insertItem( tr("At End"));
	mvWhereData->setCurrentItem(2);
	fromToLayout->addMultiCellWidget( mvWhereData, 1, 1, 0, 1 );
	mvWherePageData = new QSpinBox( 1, maxPages, 1, this, "mvWherePageData" );
	mvWherePageData->setValue( currentPage );
	mvWherePageData->setDisabled( true );
	fromToLayout->addWidget( mvWherePageData, 1, 3 );
	fromToLayout->addColSpacing(0, moveLabel->fontMetrics().width( tr( "Move Page(s):" )));
	dialogLayout->addLayout( fromToLayout );

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( tr( "&OK" ), this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget(okButton);
	cancelButton = new QPushButton( tr( "&Cancel" ), this, "cancelButton" );
	okCancelLayout->addWidget(cancelButton);
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	if (move)
		connect( toPageData, SIGNAL( valueChanged(int) ), this, SLOT( toChanged(int) ) );
	connect( mvWhereData, SIGNAL( activated(int) ), this, SLOT( mvWherePageDataDisable(int) ) );
	connect( fromPageData, SIGNAL( valueChanged(int) ), this, SLOT( fromChanged(int) ) );
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void MovePages::fromChanged(int pageNumber)
{
	if (move)
	{
		if (pageNumber > toPageData->value())
			toPageData->setValue(pageNumber);
		if ((pageNumber == 1) && (toPageData->value() == toPageData->maxValue()))
			toPageData->setValue(toPageData->maxValue()-1);
	}
}

void MovePages::toChanged(int pageNumber)
{
	if (pageNumber < fromPageData->value())
		fromPageData->setValue(pageNumber);
	if ((fromPageData->value() == 1) && (pageNumber == toPageData->maxValue()))
		fromPageData->setValue(2);
}

void MovePages::mvWherePageDataDisable(int index)
{
    mvWherePageData->setDisabled((index==2));
}


const int MovePages::getFromPage()
{
	return fromPageData->value();
}

const int MovePages::getToPage()
{
	return toPageData->value();
}

const int MovePages::getWhere()
{
	return mvWhereData->currentItem();
}

const int MovePages::getWherePage()
{
	return mvWherePageData->value();
}

