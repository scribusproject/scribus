/****************************************************************************
** Form implementation generated from reading ui file 'barcodegeneratorbase.ui'
**
** Created: Po bře 6 12:51:56 2006
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "barcodegeneratorbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a BarcodeGeneratorBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
BarcodeGeneratorBase::BarcodeGeneratorBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "BarcodeGeneratorBase" );
    BarcodeGeneratorBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "BarcodeGeneratorBaseLayout"); 

    layout15 = new QVBoxLayout( 0, 0, 6, "layout15"); 

    layout13 = new QHBoxLayout( 0, 0, 6, "layout13"); 

    bcodeBox = new QGroupBox( this, "bcodeBox" );
    bcodeBox->setColumnLayout(0, Qt::Vertical );
    bcodeBox->layout()->setSpacing( 6 );
    bcodeBox->layout()->setMargin( 11 );
    bcodeBoxLayout = new QGridLayout( bcodeBox->layout() );
    bcodeBoxLayout->setAlignment( Qt::AlignTop );

    layout16 = new QVBoxLayout( 0, 0, 6, "layout16"); 

    layout15_2 = new QHBoxLayout( 0, 0, 6, "layout15_2"); 

    layout10 = new QVBoxLayout( 0, 0, 6, "layout10"); 

    bcLabel = new QLabel( bcodeBox, "bcLabel" );
    layout10->addWidget( bcLabel );

    codeLabel = new QLabel( bcodeBox, "codeLabel" );
    layout10->addWidget( codeLabel );
    layout15_2->addLayout( layout10 );

    layout14 = new QVBoxLayout( 0, 0, 6, "layout14"); 

    bcCombo = new QComboBox( FALSE, bcodeBox, "bcCombo" );
    layout14->addWidget( bcCombo );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    codeEdit = new QLineEdit( bcodeBox, "codeEdit" );
    codeEdit->setFrameShape( QLineEdit::LineEditPanel );
    codeEdit->setFrameShadow( QLineEdit::Sunken );
    layout12->addWidget( codeEdit );

    resetButton = new QPushButton( bcodeBox, "resetButton" );
    resetButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, resetButton->sizePolicy().hasHeightForWidth() ) );
    resetButton->setMaximumSize( QSize( 24, 32767 ) );
    layout12->addWidget( resetButton );
    layout14->addLayout( layout12 );
    layout15_2->addLayout( layout14 );
    layout16->addLayout( layout15_2 );

    textCheck = new QCheckBox( bcodeBox, "textCheck" );
    textCheck->setChecked( TRUE );
    layout16->addWidget( textCheck );

    guardCheck = new QCheckBox( bcodeBox, "guardCheck" );
    layout16->addWidget( guardCheck );

    bcodeBoxLayout->addLayout( layout16, 0, 0 );
    layout13->addWidget( bcodeBox );

    colorBox = new QGroupBox( this, "colorBox" );
    colorBox->setColumnLayout(0, Qt::Vertical );
    colorBox->layout()->setSpacing( 6 );
    colorBox->layout()->setMargin( 11 );
    colorBoxLayout = new QGridLayout( colorBox->layout() );
    colorBoxLayout->setAlignment( Qt::AlignTop );

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

    layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 

    bgColorButton = new QPushButton( colorBox, "bgColorButton" );
    layout6->addWidget( bgColorButton );

    lnColorButton = new QPushButton( colorBox, "lnColorButton" );
    layout6->addWidget( lnColorButton );

    txtColorButton = new QPushButton( colorBox, "txtColorButton" );
    layout6->addWidget( txtColorButton );
    layout8->addLayout( layout6 );

    layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

    bgLabel = new QLabel( colorBox, "bgLabel" );
    bgLabel->setMinimumSize( QSize( 10, 0 ) );
    bgLabel->setFrameShape( QLabel::Box );
    bgLabel->setScaledContents( TRUE );
    layout7->addWidget( bgLabel );

    linesLabel = new QLabel( colorBox, "linesLabel" );
    linesLabel->setMinimumSize( QSize( 10, 0 ) );
    linesLabel->setFrameShape( QLabel::Box );
    linesLabel->setScaledContents( TRUE );
    layout7->addWidget( linesLabel );

    txtLabel = new QLabel( colorBox, "txtLabel" );
    txtLabel->setMinimumSize( QSize( 10, 0 ) );
    txtLabel->setFrameShape( QLabel::Box );
    txtLabel->setScaledContents( TRUE );
    layout7->addWidget( txtLabel );
    layout8->addLayout( layout7 );

    colorBoxLayout->addLayout( layout8, 0, 0 );
    layout13->addWidget( colorBox );
    layout15->addLayout( layout13 );

    layout14_2 = new QHBoxLayout( 0, 0, 6, "layout14_2"); 

    commentEdit = new QTextEdit( this, "commentEdit" );
    commentEdit->setReadOnly( TRUE );
    layout14_2->addWidget( commentEdit );

    sampleLabel = new QLabel( this, "sampleLabel" );
    sampleLabel->setFrameShape( QLabel::Box );
    sampleLabel->setAlignment( int( QLabel::AlignCenter ) );
    layout14_2->addWidget( sampleLabel );
    layout15->addLayout( layout14_2 );

    layout10_2 = new QHBoxLayout( 0, 0, 6, "layout10_2"); 
    spacer2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10_2->addItem( spacer2 );

    okButton = new QPushButton( this, "okButton" );
    layout10_2->addWidget( okButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    layout10_2->addWidget( cancelButton );
    layout15->addLayout( layout10_2 );

    BarcodeGeneratorBaseLayout->addLayout( layout15, 0, 0 );
    languageChange();
    resize( QSize(489, 348).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( bcCombo, SIGNAL( activated(const QString&) ), this, SLOT( bcComboChanged() ) );
    connect( textCheck, SIGNAL( stateChanged(int) ), this, SLOT( textCheck_changed() ) );
    connect( guardCheck, SIGNAL( stateChanged(int) ), this, SLOT( guardCheck_changed() ) );
    connect( bgColorButton, SIGNAL( clicked() ), this, SLOT( bgColorButton_pressed() ) );
    connect( lnColorButton, SIGNAL( clicked() ), this, SLOT( lnColorButton_pressed() ) );
    connect( txtColorButton, SIGNAL( clicked() ), this, SLOT( txtColorButton_pressed() ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( okButton_pressed() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelButton_pressed() ) );
    connect( codeEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( codeEdit_textChanged(const QString&) ) );
    connect( resetButton, SIGNAL( clicked() ), this, SLOT( resetButton_clicked() ) );

    // buddies
    bcLabel->setBuddy( bcCombo );
    codeLabel->setBuddy( codeEdit );
}

/*
 *  Destroys the object and frees any allocated resources
 */
BarcodeGeneratorBase::~BarcodeGeneratorBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void BarcodeGeneratorBase::languageChange()
{
    setCaption( tr( "Barcode Creator" ) );
    bcodeBox->setTitle( tr( "Barcode" ) );
    bcLabel->setText( tr( "&Type:" ) );
    codeLabel->setText( tr( "Co&de:" ) );
    QToolTip::add( bcCombo, tr( "Select one of the available barcode type here" ) );
    QToolTip::add( codeEdit, tr( "The numeric representation of the code itself. See the help message below" ) );
    resetButton->setText( QString::null );
    QToolTip::add( resetButton, tr( "Reset the barcode samples" ) );
    textCheck->setText( tr( "&Include text in barcode" ) );
    textCheck->setAccel( QKeySequence( tr( "Alt+I" ) ) );
    QToolTip::add( textCheck, tr( "If checked, there will be numbers in the barcode too" ) );
    guardCheck->setText( tr( "&Guard whitespace" ) );
    guardCheck->setAccel( QKeySequence( tr( "Alt+G" ) ) );
    QToolTip::add( guardCheck, tr( "Draw arrows to be sure of space next the code" ) );
    colorBox->setTitle( tr( "Colors" ) );
    bgColorButton->setText( tr( "&Background" ) );
    bgColorButton->setAccel( QKeySequence( tr( "Alt+B" ) ) );
    QToolTip::add( bgColorButton, tr( "Background color - under the code lines" ) );
    lnColorButton->setText( tr( "&Lines" ) );
    lnColorButton->setAccel( QKeySequence( tr( "Alt+L" ) ) );
    QToolTip::add( lnColorButton, tr( "Color of the lines in barcode" ) );
    txtColorButton->setText( tr( "&Text" ) );
    txtColorButton->setAccel( QKeySequence( tr( "Alt+T" ) ) );
    QToolTip::add( txtColorButton, tr( "Color of the text and numbers" ) );
    QToolTip::add( commentEdit, tr( "Hints and help is shown here" ) );
    QToolTip::add( sampleLabel, tr( "Preview of the result. 72dpi sample." ) );
    okButton->setText( QString::null );
    cancelButton->setText( QString::null );
}

void BarcodeGeneratorBase::bcComboChanged()
{
    qWarning( "BarcodeGeneratorBase::bcComboChanged(): Not implemented yet" );
}

void BarcodeGeneratorBase::textCheck_changed()
{
    qWarning( "BarcodeGeneratorBase::textCheck_changed(): Not implemented yet" );
}

void BarcodeGeneratorBase::guardCheck_changed()
{
    qWarning( "BarcodeGeneratorBase::guardCheck_changed(): Not implemented yet" );
}

void BarcodeGeneratorBase::bgColorButton_pressed()
{
    qWarning( "BarcodeGeneratorBase::bgColorButton_pressed(): Not implemented yet" );
}

void BarcodeGeneratorBase::lnColorButton_pressed()
{
    qWarning( "BarcodeGeneratorBase::lnColorButton_pressed(): Not implemented yet" );
}

void BarcodeGeneratorBase::txtColorButton_pressed()
{
    qWarning( "BarcodeGeneratorBase::txtColorButton_pressed(): Not implemented yet" );
}

void BarcodeGeneratorBase::okButton_pressed()
{
    qWarning( "BarcodeGeneratorBase::okButton_pressed(): Not implemented yet" );
}

void BarcodeGeneratorBase::cancelButton_pressed()
{
    qWarning( "BarcodeGeneratorBase::cancelButton_pressed(): Not implemented yet" );
}

void BarcodeGeneratorBase::codeEdit_textChanged(const QString&)
{
    qWarning( "BarcodeGeneratorBase::codeEdit_textChanged(const QString&): Not implemented yet" );
}

void BarcodeGeneratorBase::resetButton_clicked()
{
    qWarning( "BarcodeGeneratorBase::resetButton_clicked(): Not implemented yet" );
}

