/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

#include "delpages.h"
#include "util_icon.h"
#include "commonstrings.h"

DelPages::DelPages( QWidget* parent, int currentPage, int maxPage )
		: QDialog( parent, "DelPages", true, 0 )
{
	setCaption( tr( "Delete Pages" ) );
	setModal(true);
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this );
	dialogLayout->setSpacing( 5 );
	dialogLayout->setMargin( 10 );
	fromToLayout = new QHBoxLayout;
	fromToLayout->setSpacing( 5 );
	fromToLayout->setMargin( 5 );
	fromLabel = new QLabel( tr( "Delete From:" ), this, "fromLabel" );
	fromToLayout->addWidget( fromLabel );
	fromPageData = new QSpinBox( 1, maxPage, 1, this, "fromPageData" );
	fromPageData->setValue( currentPage );
	fromToLayout->addWidget( fromPageData );
	toLabel = new QLabel( this, "toLabel" );
	toLabel->setText( tr( "to:" ) );
	fromToLayout->addWidget( toLabel );
	toPageData = new QSpinBox( 1, maxPage, 1, this, "toPageData" );
	toPageData->setValue( currentPage );
	fromToLayout->addWidget( toPageData );
	dialogLayout->addLayout( fromToLayout );

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget(okButton);
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "cancelButton" );
	cancelButton->setDefault( false );
	okCancelLayout->addWidget(cancelButton);
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( fromPageData, SIGNAL( valueChanged(int) ), this, SLOT( fromChanged(int) ) );
	connect( toPageData, SIGNAL( valueChanged(int) ), this, SLOT( toChanged(int) ) );
}

void DelPages::fromChanged(int pageNumber)
{
	if (pageNumber > toPageData->value())
		toPageData->setValue(pageNumber);
	if ((pageNumber == 1) && (toPageData->value() == toPageData->maxValue()))
		toPageData->setValue(toPageData->maxValue()-1);
}

void DelPages::toChanged(int pageNumber)
{
	if (pageNumber < fromPageData->value())
		fromPageData->setValue(pageNumber);
	if ((fromPageData->value() == 1) && (pageNumber == toPageData->maxValue()))
		fromPageData->setValue(2);
}

const int DelPages::getFromPage()
{
	return fromPageData->value();
}

const int DelPages::getToPage()
{
	return toPageData->value();
}

