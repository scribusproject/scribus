#include "applytemplate.h"
#include "applytemplate.moc"
extern QPixmap loadIcon(QString nam);

ApplyT::ApplyT( QWidget* parent, ScribusView *view)
    : QDialog( parent, "apply", true, 0)
{
    setCaption( tr( "Apply Template" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
    ApplyTLayout = new QGridLayout( this ); 
    ApplyTLayout->setSpacing( 8 );
    ApplyTLayout->setMargin( 10 );
    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Template for this Page:" ) );
    ApplyTLayout->addWidget( TextLabel1, 0, 0 );
    Templ = new QComboBox( true, this, "Templ" );
    Templ->setMinimumSize( QSize( 120, 22 ) );
		Templ->setEditable(false);
		QMap<QString,int>::Iterator it;
		for (it = view->MasterNames.begin(); it != view->MasterNames.end(); ++it)
			{
			it.key() == "Normal" ? Templ->insertItem(tr("Normal")) : Templ->insertItem(it.key());
			}
    ApplyTLayout->addWidget( Templ, 0, 1 );
    Layout4 = new QHBoxLayout; 
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer );
    OKButton = new QPushButton( this, "OKButton" );
    OKButton->setText( tr( "OK" ) );
    OKButton->setDefault( TRUE );
    Layout4->addWidget( OKButton );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer_2 );
    CancelB = new QPushButton( this, "CancelB" );
    CancelB->setText( tr( "Cancel" ) );
    Layout4->addWidget( CancelB );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer_3 );
    ApplyTLayout->addMultiCellLayout( Layout4, 2, 2, 0, 1 );
    connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
}


