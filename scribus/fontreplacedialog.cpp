#include "fontreplacedialog.h"

#include "fontreplacedialog.moc"
#include <qvariant.h>
#include <qlabel.h>
#include <qtable.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qcombobox.h>
#include "fontcombo.h"
extern QPixmap loadIcon(QString nam);

FontReplaceDialog::FontReplaceDialog( QWidget* parent, QMap<QString, QString> *RList) : QDialog( parent, "FontReplaceDialog", true, 0 )
{
	setCaption( tr( "Font Substitution" ) );
	setIcon(loadIcon("AppIcon.png"));
	ReplaceList = RList;
	FontReplaceDialogLayout = new QVBoxLayout( this, 10, 5, "FontReplaceDialogLayout");

	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setAlignment( int( QLabel::AlignVCenter ) );
	textLabel1->setText( tr( "This document contains some fonts that are not installed on your system,\nplease choose a suitable replacement for them." ) );
	FontReplaceDialogLayout->addWidget( textLabel1 );

	replacementTable = new QTable( this, "replacementTable" );
	replacementTable->setNumCols( 2 );
	replacementTable->horizontalHeader()->setLabel( 0, tr( "Original Font" ) );
	replacementTable->horizontalHeader()->setLabel( 1, tr( "Substitution Font" ) );
	replacementTable->setSorting(false);
	replacementTable->setSelectionMode(QTable::NoSelection);
	replacementTable->setLeftMargin(0);
	replacementTable->verticalHeader()->hide();
	replacementTable->setNumRows(RList->count());
	int a = 0;
	QMap<QString,QString>::Iterator itfsu;
	for (itfsu = RList->begin(); itfsu != RList->end(); ++itfsu)
	{
		replacementTable->setText(a, 0, itfsu.key());
		FontCombo* item = new FontCombo(this);
		item->setCurrentText(itfsu.data());
		replacementTable->setCellWidget(a, 1, item);
		a++;
	}
	replacementTable->setColumnStretchable(0, true);
	replacementTable->setColumnStretchable(1, true);
	FontReplaceDialogLayout->addWidget( replacementTable );

	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	stickyReplacements = new QCheckBox( this, "stickyReplacements" );
	stickyReplacements->setText( tr( "Make these substitutions permanent" ) );
	layout1->addWidget( stickyReplacements );
	spacer1 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer1 );
	okButton = new QPushButton( this, "okButton" );
	okButton->setText( tr( "&OK" ) );
	layout1->addWidget( okButton );
	FontReplaceDialogLayout->addLayout( layout1 );
	resize( QSize(474, 247).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}

void FontReplaceDialog::closeEvent(QCloseEvent *closeEvent)
{
	for (int a = 0; a < replacementTable->numRows(); ++a)
	{
		FontCombo* item = (FontCombo*)replacementTable->cellWidget(a, 1);
		ReplaceList->replace(replacementTable->text(a, 0), item->currentText());
	}
	closeEvent->accept();
}

