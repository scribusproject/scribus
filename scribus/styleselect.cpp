#include <qgroupbox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

#include "styleselect.h"
#include "styleselect.moc"

extern QPixmap loadIcon(QString nam);

StyleSelect::StyleSelect(QWidget* parent) : QWidget(parent, "StyleSelect")
{
	ssLayout = new QHBoxLayout( this, 0, 0, "ssLayout");

    underlineButton = new QToolButton( this, "underlineButton" );
	underlineButton->setText( "" );
    underlineButton->setMaximumSize( QSize( 22, 22 ) );
    underlineButton->setPixmap(loadIcon("Unter.xpm"));
    underlineButton->setToggleButton( true );
    ssLayout->addWidget( underlineButton );

    buttonGroup = new QGroupBox( this, "buttonGroup" );
    buttonGroup->setFrameShape( QGroupBox::NoFrame );
    buttonGroup->setTitle("");
    buttonGroup->setColumnLayout(0, Qt::Vertical );
    buttonGroup->layout()->setSpacing( 0 );
    buttonGroup->layout()->setMargin( 0 );
    buttonGroupLayout = new QHBoxLayout( buttonGroup->layout() );
    buttonGroupLayout->setAlignment( Qt::AlignTop );

    subscriptButton = new QToolButton( buttonGroup, "subscriptButton" );
	subscriptButton->setText( "" );
    subscriptButton->setMaximumSize( QSize( 22, 22 ) );
    subscriptButton->setPixmap(loadIcon("Tief.xpm"));
    subscriptButton->setToggleButton( true );
    buttonGroupLayout->addWidget( subscriptButton );

    superscriptButton = new QToolButton( buttonGroup, "superscriptButton" );
	superscriptButton->setText( "" );
    superscriptButton->setMaximumSize( QSize( 22, 22 ) );
    superscriptButton->setPixmap(loadIcon("Hoch.xpm"));
    superscriptButton->setToggleButton( true );
    buttonGroupLayout->addWidget( superscriptButton );
    ssLayout->addWidget( buttonGroup );

    smallcapsButton = new QToolButton( this, "smallcapsButton" );
    smallcapsButton->setMaximumSize( QSize( 22, 22 ) );
    smallcapsButton->setText("");
    smallcapsButton->setPixmap(loadIcon("Kapital.xpm"));
    smallcapsButton->setToggleButton( true );
    ssLayout->addWidget( smallcapsButton );

    strikeoutButton = new QToolButton( this, "strikeoutButton" );
	strikeoutButton->setText( "" );
    strikeoutButton->setMaximumSize( QSize( 22, 22 ) );
    strikeoutButton->setPixmap(loadIcon("Strike.xpm"));
    strikeoutButton->setToggleButton( true );
    ssLayout->addWidget( strikeoutButton );

    outlineButton = new QToolButton( this, "outlineButton" );
	outlineButton->setText( "" );
    outlineButton->setMaximumSize( QSize( 22, 22 ) );
    outlineButton->setPixmap(loadIcon("outlined.png"));
    outlineButton->setToggleButton( true );
    ssLayout->addWidget( outlineButton );

    QToolTip::add( underlineButton, tr( "Underline" ) );
    QToolTip::add( smallcapsButton, tr( "Small Caps" ) );
    QToolTip::add( subscriptButton, tr( "Subscript" ) );
    QToolTip::add( superscriptButton, tr( "Superscript" ) );
    QToolTip::add( strikeoutButton, tr( "Strike Out" ) );
    QToolTip::add( outlineButton, tr( "Outline" ) );

    connect(smallcapsButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(underlineButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(subscriptButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(strikeoutButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(superscriptButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(outlineButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));

    resize(minimumSizeHint());
}

void StyleSelect::setStyle(int s)
{
	superscriptButton->setOn(false);
	subscriptButton->setOn(false);
	strikeoutButton->setOn(false);
	underlineButton->setOn(false);
	smallcapsButton->setOn(false);
	outlineButton->setOn(false);
	if (s & 1)
		superscriptButton->setOn(true);
	if (s & 2)
		subscriptButton->setOn(true);
	if (s & 4)
		outlineButton->setOn(true);
	if (s & 8)
		underlineButton->setOn(true);
	if (s & 16)
		strikeoutButton->setOn(true);
	if (s & 64)
		smallcapsButton->setOn(true);
}

int StyleSelect::getStyle()
{
	int ret = 0;
	if (superscriptButton->isOn())
		ret |= 1;
	if (subscriptButton->isOn())
		ret |= 2;
	if (outlineButton->isOn())
		ret |= 4;
	if (underlineButton->isOn())
		ret |= 8;
	if (strikeoutButton->isOn())
		ret |= 16;
	if (smallcapsButton->isOn())
		ret |= 64;
	return ret;
}

void StyleSelect::setTypeStyle()
{
	if (superscriptButton == sender())
		subscriptButton->setOn(false);
	if (subscriptButton == sender())
		superscriptButton->setOn(false);
	emit State(getStyle());
}

