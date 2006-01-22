/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "valuedialog.h"
#include "valuedialog.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "commonstrings.h"

ValueDialog::ValueDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
	: QDialog( parent, name, modal, fl )
{
	if ( !name )
		setName( "ValueDialog" );
	setModal( true );
	ScripterValueLayout = new QGridLayout( this, 1, 1, 11, 6, "ScripterValueLayout");

	layout3 = new QVBoxLayout( 0, 0, 6, "layout3");

	dialogLabel = new QLabel( this, "dialogLabel" );
	dialogLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, dialogLabel->sizePolicy().hasHeightForWidth() ) );
	layout3->addWidget( dialogLabel );

	layout2 = new QVBoxLayout( 0, 0, 6, "layout2");

	valueEdit = new QLineEdit( this, "valueEdit" );
	layout2->addWidget( valueEdit );

	layout1 = new QHBoxLayout( 0, 0, 6, "layout1");
	QSpacerItem* spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer );

	okButton = new QPushButton( this, "okButton" );
	layout1->addWidget( okButton );
	layout2->addLayout( layout1 );
	layout3->addLayout( layout2 );

	ScripterValueLayout->addLayout( layout3, 0, 0 );
	languageChange();
	//resize( QSize(315, 101).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( okButton_pressed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ValueDialog::~ValueDialog()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ValueDialog::languageChange()
{
	setCaption( tr( "Insert value" ) );
	dialogLabel->setText( tr( "Enter a value then press OK." ) );
	QToolTip::add( valueEdit, tr( "Enter a value then press OK" ) );
	okButton->setText( CommonStrings::tr_OK );
	okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
	QToolTip::add( okButton, tr( "Send your value to the script" ) );
}

void ValueDialog::okButton_pressed()
{
	accept();
}
