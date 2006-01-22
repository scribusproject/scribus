/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "hyask.h"
#include "hyask.moc"
#include <qpixmap.h>
#include <qregexp.h>
extern QPixmap loadIcon(QString nam);

WortEdit::WortEdit(QWidget* parent) : QLineEdit(parent)
{
}

void WortEdit::keyPressEvent(QKeyEvent *k)
{
	int p = cursorPosition();
	QChar cc = text()[p];
	if ((k->key() == Key_Left) || (k->key() == Key_Right))
		QLineEdit::keyPressEvent(k);
	if (k->key() == Key_Delete)
	{
		if (cc == "-")
			QLineEdit::keyPressEvent(k);
		setCursorPosition(p);
	}
	if ((k->key() == Key_Backspace) && (p != 0))
	{
		cc = text()[p-1];
		if (cc == "-")
			QLineEdit::keyPressEvent(k);
		setCursorPosition(p-1);
	}
	if (k->key() == Key_Minus)
		QLineEdit::keyPressEvent(k);
}

HyAsk::HyAsk( QWidget* parent, QString HWort )
    : QDialog( parent, "name", true, 0 )
{
    resize( 322, 73 ); 
    setCaption( tr( "Possible Hyphenation" ) );
  	setIcon(loadIcon("AppIcon.png"));
    HyAskLayout = new QVBoxLayout( this, 11, 6, "HyAskLayout"); 

    Wort = new WortEdit( this );
    Wort->setMinimumSize( QSize( 300, 0 ) );
    Wort->setDragEnabled( false );
    Wort->setText( HWort );
    HyAskLayout->addWidget( Wort );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    OK = new QPushButton( this, "OK" );
    OK->setText( tr( "Accept" ) );
    OK->setDefault( true );
    Layout1->addWidget( OK );
    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_2 );
    Skip = new QPushButton( this, "cancel2" );
    Skip->setText( tr( "Skip" ) );
    Layout1->addWidget( Skip );
    QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_3 );
    Cancel = new QPushButton( this, "cancel" );
    Cancel->setText( tr( "Cancel" ) );
    Layout1->addWidget( Cancel );
    QSpacerItem* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_4 );
    HyAskLayout->addLayout( Layout1 );
	connect(OK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(Cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(Skip, SIGNAL(clicked()), this, SLOT(DoSkip()));
	connect(Wort, SIGNAL(textChanged(const QString &)), this, SLOT(Check()));
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
	disconnect(Wort, SIGNAL(textChanged(const QString &)), this, SLOT(Check()));
	QString in = Wort->text();
	QString out = in.replace(QRegExp("(-)+"), "-");
	Wort->setText(out);
	connect(Wort, SIGNAL(textChanged(const QString &)), this, SLOT(Check()));
}

void HyAsk::DoSkip()
{
	disconnect(Wort, SIGNAL(textChanged(const QString &)), this, SLOT(Check()));
	QString in = Wort->text();
	QString out = in.replace(QRegExp("(-)+"), "");
	Wort->setText(out);
	accept();
}

