/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "query.h"
#include <QPixmap>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "commonstrings.h"
#include "util_icon.h"

Query::Query( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl, QString text, QString titel )
		: QDialog( parent )
{
	setWindowTitle( titel );
	setWindowIcon(loadIcon("AppIcon.png"));
	setModal(modal);
	queryLayout = new QVBoxLayout( this );
	queryLayout->setMargin(5);
	queryLayout->setSpacing(10);
	editLayout = new QHBoxLayout;
	editLayout->setSpacing( 5 );
	editLayout->setMargin( 0 );
	answerEdit = new QLineEdit( this );
	questionLabel = new QLabel( text, this );
	questionLabel->setBuddy(answerEdit);
	questionLabel->adjustSize();
	editLayout->addWidget( questionLabel );
	editLayout->addWidget( answerEdit );
	queryLayout->addLayout( editLayout );
	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this);
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	okCancelLayout->addWidget( cancelButton );
	queryLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());
	answerEdit->setFocus();

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( Leave() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

	// tab order
	setTabOrder( answerEdit, okButton );
	setTabOrder( okButton, cancelButton );
}

void Query::Leave()
{
	if (answerEdit->text().isEmpty())
		return;
	else
		accept();
}

const QString Query::getEditText()
{
	return answerEdit->text();
}

void Query::setEditText(QString newText, bool setSelected)
{
	answerEdit->setText(newText);
	if (setSelected)
		answerEdit->selectAll();
}
