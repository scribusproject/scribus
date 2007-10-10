/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scinputdialog.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>


#include "scrspinbox.h"
#include "util.h"
#include "util_icon.h"

ScInputDialog::ScInputDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
	: QDialog( parent )
{
	if ( !name )
		setObjectName( "ScInputDialog" );
	setModal(modal);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	setSizeGripEnabled( TRUE );
	ScInputDialogLayout = new QVBoxLayout( this );
	ScInputDialogLayout->setMargin(10);
	ScInputDialogLayout->setSpacing(5);

	labelEntryLayout = new QHBoxLayout;
	labelEntryLayout->setMargin(0);
	labelEntryLayout->setSpacing(5);

	entryLabel = new QLabel( this );
	labelEntryLayout->addWidget( entryLabel );

	entrySpin = new ScrSpinBox( this, 0 );
	entrySpin->setMinimumSize( QSize( 0, 10 ) );
	labelEntryLayout->addWidget( entrySpin );

	buttonLayout = new QHBoxLayout;
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(5);
	QSpacerItem* hspacing = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addItem(hspacing);

	buttonOk = new QPushButton( this );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );
	buttonLayout->addWidget( buttonOk );

	buttonCancel = new QPushButton( this );
	buttonCancel->setAutoDefault( TRUE );
	buttonLayout->addWidget( buttonCancel );

	ScInputDialogLayout->addLayout(labelEntryLayout);
	ScInputDialogLayout->addLayout(buttonLayout);
	languageChange();

	// signals and slots connections
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void ScInputDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
}

void ScInputDialog::languageChange()
{
	setWindowTitle( tr( "Input Dialog" ) );
	entryLabel->setText( tr( "InputDialog" ) );
	buttonOk->setText( tr( "&OK" ) );
	buttonOk->setShortcut( QKeySequence( QString::null ) );
	buttonCancel->setText( tr( "&Cancel" ) );
	buttonCancel->setShortcut( QKeySequence( QString::null ) );
}

double ScInputDialog::getDouble( const QString &caption, const QString &label, double num,
								double from, double to, int decimals,
								QString suffix, bool *ok,
								QWidget *parent, const char *name )
{
	ScInputDialog *dlg = new ScInputDialog(parent, name, TRUE);
	dlg->setWindowTitle( caption );
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
