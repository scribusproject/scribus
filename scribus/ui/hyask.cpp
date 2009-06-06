/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "hyask.h"
#include <QPixmap>
#include <QRegExp>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

#include "util_icon.h"

WortEdit::WortEdit ( QWidget* parent ) : QLineEdit ( parent )
{}

void WortEdit::keyPressEvent ( QKeyEvent *k )
{
	int p = cursorPosition();
	QChar cc = text() [p];
	if ( ( k->key() == Qt::Key_Left ) || ( k->key() == Qt::Key_Right ) )
		QLineEdit::keyPressEvent ( k );
	if ( k->key() == Qt::Key_Delete )
	{
		if ( cc == '-' )
			QLineEdit::keyPressEvent ( k );
		setCursorPosition ( p );
	}
	if ( ( k->key() == Qt::Key_Backspace ) && ( p != 0 ) )
	{
		cc = text() [p-1];
		if ( cc == '-' )
			QLineEdit::keyPressEvent ( k );
		setCursorPosition ( p-1 );
	}
	if ( k->key() == Qt::Key_Minus )
		QLineEdit::keyPressEvent ( k );
}

HyAsk::HyAsk ( QWidget* parent, QString HWort ) : QDialog ( parent )
{
	setModal(true);
	setWindowTitle( tr( "Possible Hyphenation" ));
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	HyAskLayout = new QVBoxLayout(this);
	HyAskLayout->setMargin(10);
	HyAskLayout->setSpacing(5);

	Wort = new WortEdit ( this );
	Wort->setMinimumSize ( QSize ( 300, 0 ) );
	Wort->setDragEnabled ( false );
	Wort->setText ( HWort );
	HyAskLayout->addWidget ( Wort );

	Layout1 = new QHBoxLayout;
	Layout1->setMargin(0);
	Layout1->setSpacing(5);

	vboxLayout1 = new QVBoxLayout();
	vboxLayout1->setSpacing(5);
	vboxLayout1->setMargin(0);
	OK = new QPushButton(this);
	OK->setText( tr("Accept"));
	OK->setDefault(true);
	vboxLayout1->addWidget(OK);
	hboxLayout1 = new QHBoxLayout();
	hboxLayout1->setSpacing(0);
	hboxLayout1->setMargin(0);
	addToExceptionList = new QCheckBox(this);
	hboxLayout1->addWidget(addToExceptionList);
	addToExceptionListText = new QLabel(this);
	addToExceptionListText->setText( tr("Add to the\nException List"));
	hboxLayout1->addWidget(addToExceptionListText);
	vboxLayout1->addLayout(hboxLayout1);
	Layout1->addLayout(vboxLayout1);
	addToExceptionList->setEnabled(false);
	addToExceptionListText->setEnabled(false);
	addToExceptionList->setChecked(false);

	vboxLayout2 = new QVBoxLayout();
	vboxLayout2->setSpacing(5);
	vboxLayout2->setMargin(0);
	Skip = new QPushButton(this);
	Skip->setText( tr("Skip"));
	vboxLayout2->addWidget(Skip);
	hboxLayout2 = new QHBoxLayout();
	hboxLayout2->setSpacing(0);
	hboxLayout2->setMargin(0);
	addToIgnoreList = new QCheckBox(this);
	hboxLayout2->addWidget(addToIgnoreList);
	addToIgnoreListText = new QLabel(this);
	addToIgnoreListText->setText( tr("Add to the\nIgnore List"));
	hboxLayout2->addWidget(addToIgnoreListText);
	vboxLayout2->addLayout(hboxLayout2);
	Layout1->addLayout(vboxLayout2);
	addToIgnoreList->setChecked(false);

	vboxLayout3 = new QVBoxLayout();
	vboxLayout3->setSpacing(5);
	vboxLayout3->setMargin(0);
	Cancel = new QPushButton(this);
	Cancel->setText( tr("Cancel"));
	vboxLayout3->addWidget(Cancel);
	QSpacerItem* spacer = new QSpacerItem(2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);
	vboxLayout3->addItem(spacer);
	Layout1->addLayout(vboxLayout3);
	HyAskLayout->addLayout ( Layout1 );

	connect ( OK, SIGNAL ( clicked() ), this, SLOT ( accept() ) );
	connect ( Cancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
	connect ( Skip, SIGNAL ( clicked() ), this, SLOT ( DoSkip() ) );
	connect ( Wort, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( Check() ) );
	resize(minimumSizeHint());
}

void HyAsk::accept()
{
	xpos = pos().x();
	ypos = pos().y();
	QDialog::accept();
}

void HyAsk::reject()
{
	xpos = pos().x();
	ypos = pos().y();
	QDialog::reject();
}

void HyAsk::Check()
{
	disconnect ( Wort, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( Check() ) );
	QString in = Wort->text();
	QString out = in.replace ( QRegExp ( "(-)+" ), "-" );
	Wort->setText ( out );
	addToExceptionList->setEnabled(true);
	addToExceptionListText->setEnabled(true);
	connect ( Wort, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( Check() ) );
}

void HyAsk::DoSkip()
{
	disconnect ( Wort, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( Check() ) );
	QString in = Wort->text();
	QString out = in.replace ( QRegExp ( "(-)+" ), "" );
	Wort->setText ( out );
	accept();
}

