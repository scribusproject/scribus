/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "applytemplatedialog.h"
#include "applytemplatedialog.moc"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "commonstrings.h"
#include "scribusdoc.h"
#include "page.h"

enum {
    CurrentPage,
    EvenPages,
    OddPages,
    AllPages
};

/*
 *  Constructs a ApplyMasterPageDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ApplyMasterPageDialog::ApplyMasterPageDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
		: QDialog( parent, name, modal, fl )
{
	if ( !name )
		setName( "ApplyMasterPageDialog" );
	ApplyMasterPageDialogLayout = new QVBoxLayout( this, 11, 6, "ApplyMasterPageDialogLayout");

	templateNameLayout = new QHBoxLayout( 0, 0, 6, "templateNameLayout");

	masterPageLabel = new QLabel( this, "masterPageLabel" );
	templateNameLayout->addWidget( masterPageLabel );
	spacer2 = new QSpacerItem( 100, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
	templateNameLayout->addItem( spacer2 );

	masterPageComboBox = new QComboBox( false, this, "masterPageComboBox" );
	templateNameLayout->addWidget( masterPageComboBox );
	ApplyMasterPageDialogLayout->addLayout( templateNameLayout );

	applyToPageButtonGroup = new QButtonGroup( this, "applyToPageButtonGroup" );
	applyToPageButtonGroup->setMinimumSize( QSize( 250, 0 ) );
	applyToPageButtonGroup->setFrameShape( QButtonGroup::GroupBoxPanel );
	applyToPageButtonGroup->setColumnLayout(0, Qt::Vertical );
	applyToPageButtonGroup->layout()->setSpacing( 6 );
	applyToPageButtonGroup->layout()->setMargin( 11 );
	applyToPageButtonGroupLayout = new QVBoxLayout( applyToPageButtonGroup->layout() );
	applyToPageButtonGroupLayout->setAlignment( Qt::AlignTop );

	currentPageRadioButton = new QRadioButton( applyToPageButtonGroup, "currentPageRadioButton" );
	currentPageRadioButton->setChecked( true );
	applyToPageButtonGroupLayout->addWidget( currentPageRadioButton );

	evenPagesRadioButton = new QRadioButton( applyToPageButtonGroup, "evenPagesRadioButton" );
	applyToPageButtonGroupLayout->addWidget( evenPagesRadioButton );

	oddPagesRadioButton = new QRadioButton( applyToPageButtonGroup, "oddPagesRadioButton" );
	applyToPageButtonGroupLayout->addWidget( oddPagesRadioButton );

	allPagesRadioButton = new QRadioButton( applyToPageButtonGroup, "allPagesRadioButton" );
	applyToPageButtonGroupLayout->addWidget( allPagesRadioButton );

	rangeLayout = new QHBoxLayout( 0, 0, 6, "rangeLayout");

	useRangeCheckBox = new QCheckBox( applyToPageButtonGroup, "useRangeCheckBox" );
	rangeLayout->addWidget( useRangeCheckBox );

	fromPageSpinBox = new QSpinBox( applyToPageButtonGroup, "fromPageSpinBox" );
	fromPageSpinBox->setEnabled( false );
	fromPageSpinBox->setMinValue( 1 );
	rangeLayout->addWidget( fromPageSpinBox );

	toPageLabel = new QLabel( applyToPageButtonGroup, "toPageLabel" );
	rangeLayout->addWidget( toPageLabel );

	toPageSpinBox = new QSpinBox( applyToPageButtonGroup, "toPageSpinBox" );
	toPageSpinBox->setEnabled( false );
	toPageSpinBox->setMinValue( 1 );
	rangeLayout->addWidget( toPageSpinBox );
	spacer3 = new QSpacerItem( 16, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
	rangeLayout->addItem( spacer3 );
	applyToPageButtonGroupLayout->addLayout( rangeLayout );
	ApplyMasterPageDialogLayout->addWidget( applyToPageButtonGroup );

	layout8 = new QHBoxLayout( 0, 0, 6, "layout8");
	spacer1 = new QSpacerItem( 100, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout8->addItem( spacer1 );

	okButton = new QPushButton( this, "okButton" );
	layout8->addWidget( okButton );

	cancelButton = new QPushButton( this, "cancelButton" );
	layout8->addWidget( cancelButton );
	ApplyMasterPageDialogLayout->addLayout( layout8 );
	languageChange();
	resize( QSize(272, 230).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// signals and slots connections
	connect( useRangeCheckBox, SIGNAL( toggled(bool) ), this, SLOT( enableRange(bool) ) );
	connect( currentPageRadioButton, SIGNAL( clicked() ), this, SLOT( singleSelectable() ) );
	connect( evenPagesRadioButton, SIGNAL( clicked() ), this, SLOT( rangeSelectable() ) );
	connect( oddPagesRadioButton, SIGNAL( clicked() ), this, SLOT( rangeSelectable() ) );
	connect( allPagesRadioButton, SIGNAL( clicked() ), this, SLOT( rangeSelectable() ) );
	connect( fromPageSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( checkRangeFrom() ) );
	connect( toPageSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( checkRangeTo() ) );
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

	// buddies
	masterPageLabel->setBuddy( masterPageComboBox );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ApplyMasterPageDialog::~ApplyMasterPageDialog()
{
	// no need to delete child widgets, Qt does it all for us
}

void ApplyMasterPageDialog::setup(ScribusDoc *view, QString Nam)
{
	QString na = Nam == "Normal" ? tr("Normal") : Nam, in;
	int cc = 0;
	for (QMap<QString,int>::Iterator it = view->MasterNames.begin(); it != view->MasterNames.end(); ++it)
	{
		in = it.key() == "Normal" ? tr("Normal") : it.key();
		masterPageComboBox->insertItem(in);
		if (in == na)
			masterPageComboBox->setCurrentItem(cc);
		++cc;
	}
	const unsigned int docPagesCount = view->Pages->count();
	if (docPagesCount < 2)
		evenPagesRadioButton->setEnabled(false);
	fromPageSpinBox->setMaxValue(docPagesCount);
	fromPageSpinBox->setValue(view->currentPage->pageNr()+1);
	toPageSpinBox->setMaxValue(docPagesCount);
	toPageSpinBox->setValue(docPagesCount);
}


QString ApplyMasterPageDialog::getMasterPageName()
{
	return masterPageComboBox->currentText();
}


int ApplyMasterPageDialog::getPageSelection()
{
	if (currentPageRadioButton->isChecked())
		return CurrentPage;
	else if (evenPagesRadioButton->isChecked())
		return EvenPages;
	else if (oddPagesRadioButton->isChecked())
		return OddPages;
	else
		return AllPages;
}

void ApplyMasterPageDialog::checkRangeFrom()
{
	disconnect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	disconnect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
	if (fromPageSpinBox->value() > toPageSpinBox->value())
		toPageSpinBox->setValue(fromPageSpinBox->value());
	connect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	connect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
}

void ApplyMasterPageDialog::checkRangeTo()
{
	disconnect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	disconnect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
	if (toPageSpinBox->value() < fromPageSpinBox->value())
		fromPageSpinBox->setValue(toPageSpinBox->value());
	connect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	connect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
}

void ApplyMasterPageDialog::enableRange( bool enabled )
{
	fromPageSpinBox->setEnabled(enabled);
	toPageSpinBox->setEnabled(enabled);
}

void ApplyMasterPageDialog::rangeSelectable()
{
	useRangeCheckBox->setEnabled(true);
	enableRange(useRangeCheckBox->isChecked());
}

void ApplyMasterPageDialog::singleSelectable()
{
	useRangeCheckBox->setEnabled(false);
	fromPageSpinBox->setEnabled(false);
	toPageSpinBox->setEnabled(false);
}


bool ApplyMasterPageDialog::usingRange()
{
	return useRangeCheckBox->isChecked();
}


int ApplyMasterPageDialog::getFromPage()
{
	if (useRangeCheckBox->isChecked())
		return fromPageSpinBox->value();
	return -1;
}


int ApplyMasterPageDialog::getToPage()
{
	if (useRangeCheckBox->isChecked())
		return toPageSpinBox->value();
	return -1;
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ApplyMasterPageDialog::languageChange()
{
	setCaption( tr( "Apply Master Page" ) );
	masterPageLabel->setText( tr( "&Master Page:" ) );
	applyToPageButtonGroup->setTitle( tr( "Apply To" ) );
	currentPageRadioButton->setText( tr( "Current &page" ) );
	currentPageRadioButton->setAccel( QKeySequence( tr( "Alt+P" ) ) );
	evenPagesRadioButton->setText( tr( "&Even pages" ) );
	evenPagesRadioButton->setAccel( QKeySequence( tr( "Alt+E" ) ) );
	oddPagesRadioButton->setText( tr( "O&dd pages" ) );
	oddPagesRadioButton->setAccel( QKeySequence( tr( "Alt+D" ) ) );
	allPagesRadioButton->setText( tr( "&All pages" ) );
	allPagesRadioButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
	useRangeCheckBox->setText( tr( "&Within range" ) );
	useRangeCheckBox->setAccel( QKeySequence( tr( "Alt+W" ) ) );
	QToolTip::add( useRangeCheckBox, "<qt>" + tr( "Apply the selected template to even, odd or all pages within the following range") + "</qt>" );
	toPageLabel->setText( tr( "to" ) );
	okButton->setText( CommonStrings::tr_OK );
	okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
	cancelButton->setText( CommonStrings::tr_Cancel );
	cancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}
