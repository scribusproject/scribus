#include "insertTable.h"
#include "insertTable.moc"

extern QPixmap loadIcon(QString nam);

InsertTable::InsertTable( QWidget* parent, int maxRow, int maxCol )
		: QDialog( parent, "InsertTable", true, 0 )

{
	setCaption( tr( "Insert Table" ) );
	setIcon(loadIcon("AppIcon.png"));

	InsertTableLayout = new QVBoxLayout( this, 11, 8, "InsertTableLayout");
	layout2 = new QGridLayout( 0, 1, 1, 0, 6, "layout2");
	Cols = new QSpinBox( this, "Cols" );
	Cols->setMaxValue( maxCol );
	Cols->setMinValue( 1 );
	layout2->addWidget( Cols, 1, 1 );
	Text1 = new QLabel( this, "Text1" );
	Text1->setText( tr( "Number of Rows:" ) );
	layout2->addWidget( Text1, 0, 0 );
	Text2 = new QLabel( this, "Text2" );
	Text2->setText( tr( "Number of Columns:" ) );
	layout2->addWidget( Text2, 1, 0 );
	Rows = new QSpinBox( this, "Rows" );
	Rows->setMaxValue( maxRow );
	Rows->setMinValue( 1 );
	layout2->addWidget( Rows, 0, 1 );
	InsertTableLayout->addLayout( layout2 );
	layout1 = new QHBoxLayout( 0, 0, 6, "layout1");
	OKButton = new QPushButton( this, "OKButton" );
	OKButton->setText( tr( "OK" ) );
	OKButton->setDefault( TRUE );
	layout1->addWidget( OKButton );
	CancelB = new QPushButton( this, "CancelB" );
	CancelB->setText( tr( "Cancel" ) );
	layout1->addWidget( CancelB );
	InsertTableLayout->addLayout( layout1 );
	resize( QSize(200, 111).expandedTo(minimumSizeHint()) );

	setTabOrder ( Rows, Cols );
	setTabOrder ( Cols, OKButton );
	setTabOrder ( OKButton, CancelB);
	setTabOrder ( CancelB, Rows );
	Rows->setFocus();
	// signals and slots connections
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

