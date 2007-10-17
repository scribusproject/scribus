/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "applytemplatedialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QEvent>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QToolTip>
#include <QVBoxLayout>

#include "commonstrings.h"
#include "scribusdoc.h"
#include "page.h"
#include "util_icon.h"

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
ApplyMasterPageDialog::ApplyMasterPageDialog( QWidget* parent ) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Possible Hyphenation" ));
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	ApplyMasterPageDialogLayout = new QVBoxLayout(this);
	ApplyMasterPageDialogLayout->setMargin(10);
	ApplyMasterPageDialogLayout->setSpacing(5);

	templateNameLayout = new QHBoxLayout;
	templateNameLayout->setMargin(0);
	templateNameLayout->setSpacing(5);

	masterPageLabel = new QLabel( this );
	templateNameLayout->addWidget( masterPageLabel );
	spacer2 = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	templateNameLayout->addItem( spacer2 );

	masterPageComboBox = new QComboBox(this);
	masterPageComboBox->setEditable(false);
	templateNameLayout->addWidget( masterPageComboBox );
	ApplyMasterPageDialogLayout->addLayout( templateNameLayout );

	applyToPageButtonGroup = new QGroupBox(this);
	applyToPageButtonGroup->setMinimumSize( QSize( 250, 0 ) );
	applyToPageButtonGroupLayout = new QVBoxLayout(applyToPageButtonGroup);
	applyToPageButtonGroupLayout->setSpacing( 5 );
	applyToPageButtonGroupLayout->setMargin( 10 );

	currentPageRadioButton = new QRadioButton( applyToPageButtonGroup );
	currentPageRadioButton->setChecked( true );
	applyToPageButtonGroupLayout->addWidget( currentPageRadioButton );

	evenPagesRadioButton = new QRadioButton( applyToPageButtonGroup );
	applyToPageButtonGroupLayout->addWidget( evenPagesRadioButton );

	oddPagesRadioButton = new QRadioButton( applyToPageButtonGroup );
	applyToPageButtonGroupLayout->addWidget( oddPagesRadioButton );

	allPagesRadioButton = new QRadioButton( applyToPageButtonGroup );
	applyToPageButtonGroupLayout->addWidget( allPagesRadioButton );

	rangeLayout = new QHBoxLayout;
	rangeLayout->setSpacing( 5 );
	rangeLayout->setMargin( 0 );

	useRangeCheckBox = new QCheckBox( applyToPageButtonGroup );
	useRangeCheckBox->setEnabled( false );	
	rangeLayout->addWidget( useRangeCheckBox );

	fromPageSpinBox = new QSpinBox( applyToPageButtonGroup );
	fromPageSpinBox->setEnabled( false );
	fromPageSpinBox->setMinimum( 1 );
	rangeLayout->addWidget( fromPageSpinBox );

	toPageLabel = new QLabel( applyToPageButtonGroup );
	rangeLayout->addWidget( toPageLabel );

	toPageSpinBox = new QSpinBox( applyToPageButtonGroup );
	toPageSpinBox->setEnabled( false );
	toPageSpinBox->setMinimum( 1 );
	rangeLayout->addWidget( toPageSpinBox );
	spacer3 = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	rangeLayout->addItem( spacer3 );
	applyToPageButtonGroupLayout->addLayout( rangeLayout );
	ApplyMasterPageDialogLayout->addWidget( applyToPageButtonGroup );

	layout8 = new QHBoxLayout;
	layout8->setSpacing( 5 );
	layout8->setMargin( 0 );
	spacer1 = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout8->addItem( spacer1 );

	okButton = new QPushButton( this );
	layout8->addWidget( okButton );

	cancelButton = new QPushButton( this );
	layout8->addWidget( cancelButton );
	ApplyMasterPageDialogLayout->addLayout( layout8 );
	languageChange();
	resize( QSize(272, 230).expandedTo(minimumSizeHint()) );

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
	QString na = Nam == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : Nam, in;
	int cc = 0;
	for (QMap<QString,int>::Iterator it = view->MasterNames.begin(); it != view->MasterNames.end(); ++it)
	{
		in = it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key();
		masterPageComboBox->addItem(in);
		if (in == na)
			masterPageComboBox->setCurrentIndex(cc);
		++cc;
	}
	const unsigned int docPagesCount = view->Pages->count();
	if (docPagesCount < 2)
		evenPagesRadioButton->setEnabled(false);
	fromPageSpinBox->setMaximum(docPagesCount);
	fromPageSpinBox->setValue(view->currentPage()->pageNr()+1);
	toPageSpinBox->setMaximum(docPagesCount);
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

void ApplyMasterPageDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void ApplyMasterPageDialog::languageChange()
{
	setWindowTitle( tr( "Apply Master Page" ) );
	masterPageLabel->setText( tr( "&Master Page:" ) );
	applyToPageButtonGroup->setTitle( tr( "Apply To" ) );
	currentPageRadioButton->setText( tr( "Current &page" ) );
	currentPageRadioButton->setShortcut( QKeySequence( tr( "Alt+P" ) ) );
	evenPagesRadioButton->setText( tr( "&Even pages" ) );
	evenPagesRadioButton->setShortcut( QKeySequence( tr( "Alt+E" ) ) );
	oddPagesRadioButton->setText( tr( "O&dd pages" ) );
	oddPagesRadioButton->setShortcut( QKeySequence( tr( "Alt+D" ) ) );
	allPagesRadioButton->setText( tr( "&All pages" ) );
	allPagesRadioButton->setShortcut( QKeySequence( tr( "Alt+A" ) ) );
	useRangeCheckBox->setText( tr( "&Within range" ) );
	useRangeCheckBox->setShortcut( QKeySequence( tr( "Alt+W" ) ) );
	useRangeCheckBox->setToolTip( "<qt>" + tr( "Apply the selected master page to even, odd or all pages within the following range") + "</qt>" );
	toPageLabel->setText( tr( "to" ) );
	okButton->setText( CommonStrings::tr_OK );
	okButton->setShortcut( QKeySequence( tr( "Alt+O" ) ) );
	cancelButton->setText( CommonStrings::tr_Cancel );
	cancelButton->setShortcut( QKeySequence( tr( "Alt+C" ) ) );
}
