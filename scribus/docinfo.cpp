/****************************************************************************
** Form implementation generated from reading ui file 'Info.ui'
**
** Created: Mon Apr 30 10:14:47 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "docinfo.h"
extern QPixmap loadIcon(QString nam);

#include <qtooltip.h>

/* 
 *  Constructs a DocInfos which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DocInfos::DocInfos( QWidget* parent, QString Autor, QString Titel, QString Des, QString keyW )
    : QDialog( parent, "i", true, 0 )
{
    setMaximumSize( QSize( 32767, 32767 ) );
    setCaption( tr( "Document Info" ) );
 	setIcon(loadIcon("AppIcon.png"));
    DocInfosLayout = new QVBoxLayout( this );
    DocInfosLayout->setSpacing( 6 );
    DocInfosLayout->setMargin( 11 );
    Layout1 = new QGridLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setMinimumSize(QSize(80, 20));
    TextLabel1->setText( tr( "Author:" ) );
    Layout1->addWidget( TextLabel1, 0, 0 );
    AutorInfo = new QLineEdit( this, "AutorInfo" );
    AutorInfo->setMinimumSize(QSize(200, 20));
    AutorInfo->setText(Autor);
    Layout1->addWidget( AutorInfo, 0, 1 );
    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setMinimumSize(QSize(80, 20));
    TextLabel2->setText( tr( "Title:" ) );
    Layout1->addWidget( TextLabel2, 1, 0 );
    TitelInfo = new QLineEdit( this, "TitelInfo" );
    TitelInfo->setMinimumSize(QSize(200, 20));
    TitelInfo->setText(Titel);
    Layout1->addWidget( TitelInfo, 1, 1 );
    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setMinimumSize(QSize(80, 20));
    TextLabel3->setText( tr( "Description:" ) );
    TextLabel3->setAlignment( static_cast<int>( QLabel::AlignTop | QLabel::AlignLeft ) );
    Layout1->addWidget( TextLabel3, 2, 0 );
    MultiLineEdit1 = new QMultiLineEdit( this, "MultiLineEdit1" );
    MultiLineEdit1->setMinimumSize(QSize(200, 105));
    MultiLineEdit1->setText(Des);
    Layout1->addWidget( MultiLineEdit1, 2, 1 );
    TextLabel3a = new QLabel( this, "TextLabel3" );
    TextLabel3a->setMinimumSize(QSize(80, 20));
    TextLabel3a->setText( tr( "Keywords:" ) );
    TextLabel3a->setAlignment( static_cast<int>( QLabel::AlignTop | QLabel::AlignLeft ) );
    Layout1->addWidget( TextLabel3a, 3, 0 );
    MultiLineEdit2 = new QMultiLineEdit( this, "MultiLineEdit1" );
    MultiLineEdit2->setMinimumSize(QSize(200, 105));
    MultiLineEdit2->setText(keyW);
    Layout1->addWidget( MultiLineEdit2, 3, 1 );
    DocInfosLayout->addLayout( Layout1 );
    Layout3 = new QHBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );
    OK = new QPushButton( this, "OK" );
    OK->setText( tr( "OK" ) );
    OK->setDefault( true );
    Layout3->addWidget(OK);
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer );
    Cancel = new QPushButton( this, "Cancel" );
    Cancel->setText( tr( "Cancel" ) );
    Cancel->setDefault( false );
    Layout3->addWidget(Cancel);
    DocInfosLayout->addLayout( Layout3 );

    // signals and slots connections
    connect( OK, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}
