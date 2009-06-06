/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "insertTable.h"

#include "commonstrings.h"
#include "util_icon.h"

InsertTable::InsertTable( QWidget* parent, int maxRow, int maxCol ) : QDialog( parent )

{
	setWindowTitle( tr( "Insert Table" ) );
	setWindowIcon(loadIcon("AppIcon.png"));
	setModal(true);
	InsertTableLayout = new QVBoxLayout( this );
	InsertTableLayout->setMargin(10);
	InsertTableLayout->setSpacing(5);
	layout2 = new QGridLayout();
	layout2->setMargin(0);
	layout2->setSpacing(5);
	Cols = new QSpinBox(this);
	Cols->setRange(1, maxCol);
	Cols->setValue(1);
	layout2->addWidget( Cols, 1, 1 );
	Text1 = new QLabel( tr( "Number of rows:" ), this);
	Text2 = new QLabel( tr( "Number of columns:" ), this);
	layout2->addWidget( Text1, 0, 0 );
	layout2->addWidget( Text2, 1, 0 );
	Rows = new QSpinBox(this);
	Rows->setRange(1, maxRow);
	Rows->setValue(1);
	layout2->addWidget( Rows, 0, 1 );
	InsertTableLayout->addLayout( layout2 );
	layout1 = new QHBoxLayout();
	layout1->setSpacing(5);
	okButton = new QPushButton( CommonStrings::tr_OK, this);
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this);
	okButton->setDefault( true );
	layout1->addWidget( okButton );
	layout1->addWidget( cancelButton );
	InsertTableLayout->addLayout( layout1 );
	resize( QSize(200, 111).expandedTo(minimumSizeHint()) );

	setTabOrder ( Rows, Cols );
	setTabOrder ( Cols, okButton );
	setTabOrder ( okButton, cancelButton);
	setTabOrder ( cancelButton, Rows );
	Rows->setFocus();
	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

