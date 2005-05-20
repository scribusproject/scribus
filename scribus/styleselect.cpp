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

	buttonGroup3 = new QGroupBox( this, "buttonGroup" );
	buttonGroup3->setFrameShape( QGroupBox::NoFrame );
	buttonGroup3->setTitle("");
	buttonGroup3->setColumnLayout(0, Qt::Vertical );
	buttonGroup3->layout()->setSpacing( 0 );
	buttonGroup3->layout()->setMargin( 0 );
	buttonGroup3Layout = new QHBoxLayout( buttonGroup3->layout() );
	buttonGroup3Layout->setAlignment( Qt::AlignTop );
	underlineButton = new QToolButton( buttonGroup3, "underlineButton" );
	underlineButton->setText( "" );
	underlineButton->setMaximumSize( QSize( 22, 22 ) );
	underlineButton->setPixmap(loadIcon("Unter.xpm"));
	underlineButton->setToggleButton( true );
	buttonGroup3Layout->addWidget( underlineButton );
	underlineWordButton = new QToolButton( buttonGroup3, "underlineButton" );
	underlineWordButton->setText( "" );
	underlineWordButton->setMaximumSize( QSize( 22, 22 ) );
	underlineWordButton->setPixmap(loadIcon("wordsOnly.png"));
	underlineWordButton->setToggleButton( true );
	buttonGroup3Layout->addWidget( underlineWordButton );
	ssLayout->addWidget( buttonGroup3 );

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

	buttonGroup2 = new QGroupBox( this, "buttonGroup" );
	buttonGroup2->setFrameShape( QGroupBox::NoFrame );
	buttonGroup2->setTitle("");
	buttonGroup2->setColumnLayout(0, Qt::Vertical );
	buttonGroup2->layout()->setSpacing( 0 );
	buttonGroup2->layout()->setMargin( 0 );
	buttonGroup2Layout = new QHBoxLayout( buttonGroup2->layout() );
	buttonGroup2Layout->setAlignment( Qt::AlignTop );
	allcapsButton = new QToolButton( buttonGroup2, "allcapsButton" );
	allcapsButton->setMaximumSize( QSize( 22, 22 ) );
	allcapsButton->setText("");
	allcapsButton->setPixmap(loadIcon("AllCaps.png"));
	allcapsButton->setToggleButton( true );
	buttonGroup2Layout->addWidget( allcapsButton );
	smallcapsButton = new QToolButton( buttonGroup2, "smallcapsButton" );
	smallcapsButton->setMaximumSize( QSize( 22, 22 ) );
	smallcapsButton->setText("");
	smallcapsButton->setPixmap(loadIcon("Kapital.xpm"));
	smallcapsButton->setToggleButton( true );
	buttonGroup2Layout->addWidget( smallcapsButton );
	ssLayout->addWidget( buttonGroup2 );

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

	shadowButton = new QToolButton( this, "shadowButton" );
	shadowButton->setText( "" );
	shadowButton->setMaximumSize( QSize( 22, 22 ) );
	shadowButton->setPixmap(loadIcon("shadow.png"));
	shadowButton->setToggleButton( true );
	ssLayout->addWidget( shadowButton );

	QToolTip::add( underlineButton, tr( "Underline" ) );
	QToolTip::add( underlineWordButton, tr( "Underline Words only" ) );
	QToolTip::add( allcapsButton, tr( "All Caps" ) );
	QToolTip::add( smallcapsButton, tr( "Small Caps" ) );
	QToolTip::add( subscriptButton, tr( "Subscript" ) );
	QToolTip::add( superscriptButton, tr( "Superscript" ) );
	QToolTip::add( strikeoutButton, tr( "Strike Out" ) );
	QToolTip::add( outlineButton, tr( "Outline" ) );
	QToolTip::add( shadowButton, tr( "Shadow" ) );

	connect(allcapsButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(smallcapsButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(underlineButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(underlineWordButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(subscriptButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(strikeoutButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(superscriptButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(outlineButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
	connect(shadowButton, SIGNAL(clicked()), this, SLOT(setTypeStyle()));

	resize(minimumSizeHint());
}

void StyleSelect::setStyle(int s)
{
	superscriptButton->setOn(false);
	subscriptButton->setOn(false);
	strikeoutButton->setOn(false);
	underlineButton->setOn(false);
	underlineWordButton->setOn(false);
	allcapsButton->setOn(false);
	smallcapsButton->setOn(false);
	outlineButton->setOn(false);
	shadowButton->setOn(false);
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
	if (s & 32)
		allcapsButton->setOn(true);
	if (s & 64)
		smallcapsButton->setOn(true);
	if (s & 256)
		shadowButton->setOn(true);
	if (s & 512)
		underlineWordButton->setOn(true);
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
	if (allcapsButton->isOn())
		ret |= 32;
	if (smallcapsButton->isOn())
		ret |= 64;
	if (shadowButton->isOn())
		ret |= 256;
	if (underlineWordButton->isOn())
		ret |= 512;
	return ret;
}

void StyleSelect::setTypeStyle()
{
	if (superscriptButton == sender())
		subscriptButton->setOn(false);
	if (subscriptButton == sender())
		superscriptButton->setOn(false);
	if (allcapsButton == sender())
		smallcapsButton->setOn(false);
	if (smallcapsButton == sender())
		allcapsButton->setOn(false);
	if (underlineWordButton == sender())
		underlineButton->setOn(false);
	if (underlineButton == sender())
		underlineWordButton->setOn(false);
	emit State(getStyle());
}

