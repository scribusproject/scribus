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

extern QPixmap loadIcon ( QString nam );

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
	QSpacerItem* spacer = new QSpacerItem ( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem ( spacer );

	OK = new QPushButton ( this, "OK" );
	OK->setText ( tr ( "Accept" ) );
	OK->setDefault ( true );
	Layout1->addWidget ( OK );
	QSpacerItem* spacer_2 = new QSpacerItem ( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem ( spacer_2 );
	Skip = new QPushButton ( this, "cancel2" );
	Skip->setText ( tr ( "Skip" ) );
	Layout1->addWidget ( Skip );
	QSpacerItem* spacer_3 = new QSpacerItem ( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem ( spacer_3 );
	Cancel = new QPushButton ( this, "cancel" );
	Cancel->setText ( tr ( "Cancel" ) );
	Layout1->addWidget ( Cancel );
	QSpacerItem* spacer_4 = new QSpacerItem ( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem ( spacer_4 );
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

