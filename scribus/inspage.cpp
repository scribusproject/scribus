/****************************************************************************
** Form implementation generated from reading ui file 'InsPage.ui'
**
** Created: Tue Jun 12 21:21:13 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "inspage.h"
#include "inspage.moc"

extern QPixmap loadIcon(QString nam);

InsPage::InsPage( QWidget* parent, ScribusDoc* doc, int act, int max, bool ds)
		: QDialog( parent, "InsPage", true, 0 )
{
	setCaption( tr( "Insert Page" ) );
	setIcon(loadIcon("AppIcon.png"));
	DLayout = new QVBoxLayout( this );
	DLayout->setSpacing( 5 );
	DLayout->setMargin( 10 );
	Layout4 = new QGridLayout;
	Layout4->setSpacing( 6 );
	Layout4->setMargin( 5 );
	NumPages = new QSpinBox( this, "NumPages" );
	NumPages->setMinValue( 1 );
	NumPages->setValue( 1 );
	TextLabel1 = new QLabel( NumPages, tr( "&Inserting" ), this, "TextLabel1" );
	Layout4->addWidget( TextLabel1, 0, 0 );
	Layout4->addWidget( NumPages, 0, 1 );
	TextLabel2 = new QLabel( tr( "Page(s)" ), this, "TextLabel2" );
	Layout4->addWidget( TextLabel2, 0, 2 );
	Where = new QComboBox( true, this, "Where" );
	Where->insertItem( tr("before Page"));
	Where->insertItem( tr("after Page"));
	Where->insertItem( tr("at End"));
	Where->setEditable(false);
	Where->setCurrentItem(2);
	Layout4->addMultiCellWidget( Where, 1, 1, 0, 1 );
	ActualPage = new QSpinBox( this, "ActualPage" );
	ActualPage->setMinValue( 1 );
	ActualPage->setValue( act+1 );
	ActualPage->setMaxValue(max);
	Layout4->addWidget( ActualPage, 1, 2 );
	Layout4->addColSpacing(0, TextLabel1->fontMetrics().width( tr( "Inserting" )));
	DLayout->addLayout( Layout4 );
	Layout4a = new QHBoxLayout;
	Layout4a->setSpacing( 6 );
	Layout4a->setMargin( 0 );
	TextLabel3 = new QLabel(this, "text");
	TextLabel3->setText(ds ? tr("&Template (Left Page):") : tr("&Template:"));
	Layout4a->addWidget( TextLabel3 );
	QSpacerItem* spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout4a->addItem( spacer3 );
	Based = new QComboBox(true, this, "BA");
	Based->setEditable(false);
	QMap<QString,int>::Iterator it;
	for (it = doc->MasterNames.begin(); it != doc->MasterNames.end(); ++it)
		Based->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
	TextLabel3->setBuddy(Based);
	Layout4a->addWidget(Based);
	DLayout->addLayout(Layout4a);
	if (ds)
	{
		Layout4b = new QHBoxLayout;
		Layout4b->setSpacing( 6 );
		Layout4b->setMargin( 0 );
		TextLabel3a = new QLabel(this, "texta");
		TextLabel3a->setText( tr("Template (Right Page):"));
		Layout4b->addWidget( TextLabel3a );
		QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout4b->addItem( spacer2 );
		Based2 = new QComboBox(true, this, "BaA");
		Based2->setEditable(false);
		QMap<QString,int>::Iterator it2;
		for (it2 = doc->MasterNames.begin(); it2 != doc->MasterNames.end(); ++it2)
			Based2->insertItem(it2.key() == "Normal" ? tr("Normal") : it2.key());
		Layout4b->addWidget(Based2);
		DLayout->addLayout(Layout4b);
	}
	Layout3 = new QHBoxLayout;
	Layout3->setSpacing( 6 );
	Layout3->setMargin( 5 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout3->addItem( spacer );
	PushButton4 = new QPushButton( tr( "&OK" ), this, "PushButton4" );
	PushButton4->setDefault( true );
	Layout3->addWidget( PushButton4 );
	PushButton4_2 = new QPushButton( tr( "&Cancel" ), this, "PushButton4_2" );
	Layout3->addWidget( PushButton4_2 );
	DLayout->addLayout( Layout3 );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( PushButton4, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( PushButton4_2, SIGNAL( clicked() ), this, SLOT( reject() ) );
}


