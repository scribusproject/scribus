/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scinputdialog.h"
//#include "scinputdialog.moc"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include "mspinbox.h"


ScInputDialog::ScInputDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
	: QDialog( parent, name, modal, fl )
{
	if ( !name )
		setName( "ScInputDialog" );
	setSizeGripEnabled( TRUE );
	ScInputDialogLayout = new Q3GridLayout( this, 1, 1, 11, 6, "ScInputDialogLayout");

	labelEntryLayout = new Q3HBoxLayout( 0, 0, 6, "labelEntryLayout");

	entryLabel = new QLabel( this, "entryLabel" );
	labelEntryLayout->addWidget( entryLabel );

	entrySpin = new MSpinBox( this, "entrySpin" );
	entrySpin->setMinimumSize( QSize( 0, 10 ) );
	labelEntryLayout->addWidget( entrySpin );

	buttonLayout = new Q3HBoxLayout(0, 0, 6, "buttonLayout");
	QSpacerItem* hspacing = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addItem(hspacing);

	buttonOk = new QPushButton( this, "buttonOk" );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );
	buttonLayout->addWidget( buttonOk );

	buttonCancel = new QPushButton( this, "buttonCancel" );
	buttonCancel->setAutoDefault( TRUE );
	buttonLayout->addWidget( buttonCancel );

	mainLayout = new Q3VBoxLayout(0, 0, 6, "mainLayout");
	mainLayout->addLayout(labelEntryLayout);
	mainLayout->addLayout(buttonLayout);

	ScInputDialogLayout->addLayout( mainLayout, 0, 0 );
	languageChange();

	// signals and slots connections
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void ScInputDialog::languageChange()
{
	setCaption( tr( "Input Dialog" ) );
	entryLabel->setText( tr( "InputDialog" ) );
	buttonOk->setText( tr( "&OK" ) );
	buttonOk->setAccel( QKeySequence( QString::null ) );
	buttonCancel->setText( tr( "&Cancel" ) );
	buttonCancel->setAccel( QKeySequence( QString::null ) );
}

double ScInputDialog::getDouble( const QString &caption, const QString &label, double num,
								double from, double to, int decimals,
								QString suffix, bool *ok,
								QWidget *parent, const char *name )
{
	ScInputDialog *dlg = new ScInputDialog(parent, name, TRUE);
	dlg->setCaption( caption );
	dlg->entryLabel->setText(label);
	dlg->entrySpin->setValues(0.0, 1000.0, decimals, num);
	dlg->entrySpin->setSuffix(suffix);
	dlg->entrySpin->selectAll();

	bool accepted = ( dlg->exec() == QDialog::Accepted );
	if ( ok )
		*ok = accepted;

	double result = dlg->entrySpin->value();

	delete dlg;
	return result;
}
