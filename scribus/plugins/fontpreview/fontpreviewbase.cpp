/****************************************************************************
** Form implementation generated from reading ui file 'fontpreviewbase.ui'
**
** Created: Út bře 21 13:21:18 2006
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "fontpreviewbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qheader.h>
#include <qlistview.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a FontPreviewBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FontPreviewBase::FontPreviewBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "FontPreviewBase" );
    FontPreviewBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "FontPreviewBaseLayout"); 

    layout13 = new QVBoxLayout( 0, 0, 6, "layout13"); 

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    searchLabel = new QLabel( this, "searchLabel" );
    layout1->addWidget( searchLabel );

    searchEdit = new QLineEdit( this, "searchEdit" );
    layout1->addWidget( searchEdit );

    searchButton = new QPushButton( this, "searchButton" );
    layout1->addWidget( searchButton );
    layout13->addLayout( layout1 );

    fontList = new QListView( this, "fontList" );
    fontList->addColumn( tr( "Font Name" ) );
    fontList->addColumn( tr( "Doc" ) );
    fontList->addColumn( tr( "Type" ) );
    fontList->addColumn( tr( "Subset" ) );
    fontList->addColumn( tr( "Access" ) );
    layout13->addWidget( fontList );

    fontPreview = new QLabel( this, "fontPreview" );
    fontPreview->setMinimumSize( QSize( 600, 100 ) );
    fontPreview->setMaximumSize( QSize( 600, 100 ) );
    fontPreview->setFrameShape( QLabel::Box );
    layout13->addWidget( fontPreview );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    sizeLabel = new QLabel( this, "sizeLabel" );
    layout3->addWidget( sizeLabel );

    sizeSpin = new QSpinBox( this, "sizeSpin" );
    sizeSpin->setMaxValue( 72 );
    sizeSpin->setMinValue( 10 );
    layout3->addWidget( sizeSpin );
    layout12->addLayout( layout3 );

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    displayLabel = new QLabel( this, "displayLabel" );
    layout11->addWidget( displayLabel );

    displayEdit = new QLineEdit( this, "displayEdit" );
    layout11->addWidget( displayEdit );

    displayButton = new QPushButton( this, "displayButton" );
    layout11->addWidget( displayButton );

    resetDisplayButton = new QPushButton( this, "resetDisplayButton" );
    layout11->addWidget( resetDisplayButton );
    layout12->addLayout( layout11 );
    layout13->addLayout( layout12 );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
    spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout4->addItem( spacer1 );

    okButton = new QPushButton( this, "okButton" );
    layout4->addWidget( okButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout4->addWidget( cancelButton );
    layout13->addLayout( layout4 );

    FontPreviewBaseLayout->addLayout( layout13, 0, 0 );
    languageChange();
    resize( QSize(624, 490).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( displayButton, SIGNAL( clicked() ), this, SLOT( displayButton_clicked() ) );
    connect( searchEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( searchEdit_textChanged(const QString&) ) );
    connect( searchButton, SIGNAL( clicked() ), this, SLOT( searchButton_clicked() ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( okButton_clicked() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelButton_clicked() ) );
    connect( resetDisplayButton, SIGNAL( clicked() ), this, SLOT( resetDisplayButton_clicked() ) );
    connect( fontList, SIGNAL( mouseButtonClicked(int,QListViewItem*,const QPoint&,int) ), this, SLOT( fontList_mouseButtonClicked(int,QListViewItem*,const QPoint&,int) ) );
    connect( sizeSpin, SIGNAL( valueChanged(int) ), this, SLOT( sizeSpin_valueChanged(int) ) );

    // buddies
    searchLabel->setBuddy( searchEdit );
    sizeLabel->setBuddy( sizeSpin );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FontPreviewBase::~FontPreviewBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FontPreviewBase::languageChange()
{
    setCaption( tr( "Fonts Preview" ) );
    searchLabel->setText( tr( "&Quick Search:" ) );
    searchButton->setText( tr( "&Search" ) );
    searchButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    fontList->header()->setLabel( 0, tr( "Font Name" ) );
    fontList->header()->setLabel( 1, tr( "Doc" ) );
    fontList->header()->setLabel( 2, tr( "Type" ) );
    fontList->header()->setLabel( 3, tr( "Subset" ) );
    fontList->header()->setLabel( 4, tr( "Access" ) );
    sizeLabel->setText( tr( "&Font Size:" ) );
    displayLabel->setText( tr( "Text" ) );
    QToolTip::add( displayEdit, tr( "Sample text to display" ) );
    displayButton->setText( tr( "Se&t" ) );
    displayButton->setAccel( QKeySequence( tr( "Alt+T" ) ) );
    resetDisplayButton->setText( QString::null );
    QToolTip::add( resetDisplayButton, tr( "Reset the text" ) );
    okButton->setText( tr( "&Append" ) );
    okButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    cancelButton->setText( tr( "&Close" ) );
    cancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

void FontPreviewBase::displayButton_clicked()
{
    qWarning( "FontPreviewBase::displayButton_clicked(): Not implemented yet" );
}

void FontPreviewBase::searchEdit_textChanged(const QString&)
{
    qWarning( "FontPreviewBase::searchEdit_textChanged(const QString&): Not implemented yet" );
}

void FontPreviewBase::searchButton_clicked()
{
    qWarning( "FontPreviewBase::searchButton_clicked(): Not implemented yet" );
}

void FontPreviewBase::okButton_clicked()
{
    qWarning( "FontPreviewBase::okButton_clicked(): Not implemented yet" );
}

void FontPreviewBase::cancelButton_clicked()
{
    qWarning( "FontPreviewBase::cancelButton_clicked(): Not implemented yet" );
}

void FontPreviewBase::resetDisplayButton_clicked()
{
    qWarning( "FontPreviewBase::resetDisplayButton_clicked(): Not implemented yet" );
}

void FontPreviewBase::fontList_mouseButtonClicked(int,QListViewItem*,const QPoint&,int)
{
    qWarning( "FontPreviewBase::fontList_mouseButtonClicked(int,QListViewItem*,const QPoint&,int): Not implemented yet" );
}

void FontPreviewBase::sizeSpin_valueChanged(int)
{
    qWarning( "FontPreviewBase::sizeSpin_valueChanged(int): Not implemented yet" );
}

