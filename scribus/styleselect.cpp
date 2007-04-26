/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qgroupbox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpopupmenu.h>
#include <qlabel.h>
#include "mspinbox.h"

#include "styleselect.h"
#include "styleselect.moc"

extern QPixmap loadIcon(QString nam);

StrikeValues::StrikeValues( QWidget* parent ) : QGroupBox( parent, "StrikeValues" )
{
	this->setFrameShape( QGroupBox::NoFrame );
	this->setTitle("");
	this->setColumnLayout(0, Qt::Vertical );
	this->layout()->setSpacing( 0 );
	this->layout()->setMargin( 0 );
	group1Layout = new QGridLayout( this->layout() );
	group1Layout->setAlignment( Qt::AlignTop );
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
	LPos = new MSpinBox( -0.1, 100, this, 1 );
	LPos->setValue( -0.1 );
	LPos->setWrapping(true);
	LPos->setSpecialValueText( tr( "Auto" ) );
	LPosTxt = new QLabel( "Displacement", this, "XoffsetTxt" );
	group1Layout->addWidget( LPos, 0, 1 );
	group1Layout->addWidget( LPosTxt, 0 , 0 );
	LWidth = new MSpinBox( -0.1, 100, this, 1 );
	LWidth->setValue( -0.1 );
	LWidth->setWrapping(true);
	LWidth->setSpecialValueText( tr( "Auto" ) );
	LWidthTxt = new QLabel( "Linewidth", this, "LWidthTxt" );
	group1Layout->addWidget( LWidth, 1, 1 );
	group1Layout->addWidget( LWidthTxt, 1 , 0 );
	languageChange();
}

void StrikeValues::languageChange()
{
	LPos->setSuffix( tr(" %"));
	LPosTxt->setText( tr("Displacement"));
	LWidth->setSuffix( tr(" %"));
	LWidthTxt->setText( tr("Linewidth"));
	LPosTxt->adjustSize();
	LWidthTxt->adjustSize();
}

UnderlineValues::UnderlineValues( QWidget* parent ) : QGroupBox( parent, "ShadowValues" )
{
	this->setFrameShape( QGroupBox::NoFrame );
	this->setTitle("");
	this->setColumnLayout(0, Qt::Vertical );
	this->layout()->setSpacing( 0 );
	this->layout()->setMargin( 0 );
	group1Layout = new QGridLayout( this->layout() );
	group1Layout->setAlignment( Qt::AlignTop );
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
	LPos = new MSpinBox( -0.1, 100, this, 1 );
	LPos->setValue( -0.1 );
	LPos->setWrapping(true);
	LPos->setSpecialValueText( tr( "Auto" ) );
	LPosTxt = new QLabel( "Displacement", this, "XoffsetTxt" );
	group1Layout->addWidget( LPos, 0, 1 );
	group1Layout->addWidget( LPosTxt, 0 , 0 );
	LWidth = new MSpinBox( -0.1, 100, this, 1 );
	LWidth->setValue( -0.1 );
	LWidth->setWrapping(true);
	LWidth->setSpecialValueText( tr( "Auto" ) );
	LWidthTxt = new QLabel( "Linewidth", this, "LWidthTxt" );
	group1Layout->addWidget( LWidth, 1, 1 );
	group1Layout->addWidget( LWidthTxt, 1 , 0 );
	languageChange();
}

void UnderlineValues::languageChange()
{
	LPos->setSuffix( tr(" %"));
	LPosTxt->setText( tr("Displacement"));
	LWidth->setSuffix( tr(" %"));
	LWidthTxt->setText( tr("Linewidth"));
	LPosTxt->adjustSize();
	LWidthTxt->adjustSize();
}

OutlineValues::OutlineValues( QWidget* parent ) : QGroupBox( parent, "ShadowValues" )
{
	this->setFrameShape( QGroupBox::NoFrame );
	this->setTitle("");
	this->setColumnLayout(0, Qt::Vertical );
	this->layout()->setSpacing( 0 );
	this->layout()->setMargin( 0 );
	group1Layout = new QGridLayout( this->layout() );
	group1Layout->setAlignment( Qt::AlignTop );
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
	LWidth = new MSpinBox( 0, 100, this, 1 );
	LWidth->setValue( 1 );
	LWidthTxt = new QLabel( "Linewidth", this, "LWidthTxt" );
	group1Layout->addWidget( LWidth, 0, 1 );
	group1Layout->addWidget( LWidthTxt, 0 , 0 );
	languageChange();
}

void OutlineValues::languageChange()
{
	LWidth->setSuffix( tr(" %"));
	LWidthTxt->setText( tr("Linewidth"));
	LWidthTxt->adjustSize();
}

ShadowValues::ShadowValues( QWidget* parent ) : QGroupBox( parent, "ShadowValues" )
{
	this->setFrameShape( QGroupBox::NoFrame );
	this->setTitle("");
	this->setColumnLayout(0, Qt::Vertical );
	this->layout()->setSpacing( 0 );
	this->layout()->setMargin( 0 );
	group1Layout = new QGridLayout( this->layout() );
	group1Layout->setAlignment( Qt::AlignTop );
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
	Xoffset = new MSpinBox( -100, 100, this, 1 );
	Xoffset->setValue( 5 );
	XoffsetTxt = new QLabel( "X-Offset", this, "XoffsetTxt" );
	group1Layout->addWidget( Xoffset, 0, 1 );
	group1Layout->addWidget( XoffsetTxt, 0 , 0 );
	Yoffset = new MSpinBox( -100, 100, this, 1 );
	Yoffset->setValue( 5 );
	YoffsetTxt = new QLabel( "Y-Offset", this, "YoffsetTxt" );
	group1Layout->addWidget( Yoffset, 1, 1 );
	group1Layout->addWidget( YoffsetTxt, 1 , 0 );
	languageChange();
}

void ShadowValues::languageChange()
{
	Xoffset->setSuffix( tr(" %"));
	XoffsetTxt->setText( tr("X-Offset"));
	Yoffset->setSuffix( tr(" %"));
	YoffsetTxt->setText( tr("Y-Offset"));
	XoffsetTxt->adjustSize();
	YoffsetTxt->adjustSize();
}

StyleSelect::StyleSelect(QWidget* parent) : QWidget(parent, "StyleSelect")
{
	ssLayout = new QHBoxLayout( this, 0, 0, "ssLayout");
	ssLayout->setSpacing( 0 );
	ssLayout->setMargin( 0 );

	UnderlineVal = new UnderlineValues( NULL );
	UnderlinePop = new QPopupMenu();
	UnderlinePop->insertItem(UnderlineVal);
	underlineButton = new QToolButton( this, "underlineButton" );
	underlineButton->setText( "" );
	underlineButton->setMaximumSize( QSize( 22, 22 ) );
	underlineButton->setPixmap(loadIcon("Unter.xpm"));
	underlineButton->setToggleButton( true );
	underlineButton->setPopup(UnderlinePop);
	underlineButton->setPopupDelay(400);
	ssLayout->addWidget( underlineButton );
	underlineWordButton = new QToolButton( this, "underlineButton" );
	underlineWordButton->setText( "" );
	underlineWordButton->setMaximumSize( QSize( 22, 22 ) );
	underlineWordButton->setPixmap(loadIcon("wordsOnly.png"));
	underlineWordButton->setToggleButton( true );
	underlineWordButton->setPopup(UnderlinePop);
	underlineWordButton->setPopupDelay(400);
	ssLayout->addWidget( underlineWordButton );

	subscriptButton = new QToolButton( this, "subscriptButton" );
	subscriptButton->setText( "" );
	subscriptButton->setMaximumSize( QSize( 22, 22 ) );
	subscriptButton->setPixmap(loadIcon("Tief.xpm"));
	subscriptButton->setToggleButton( true );
	ssLayout->addWidget( subscriptButton );
	superscriptButton = new QToolButton( this, "superscriptButton" );
	superscriptButton->setText( "" );
	superscriptButton->setMaximumSize( QSize( 22, 22 ) );
	superscriptButton->setPixmap(loadIcon("Hoch.xpm"));
	superscriptButton->setToggleButton( true );
	ssLayout->addWidget( superscriptButton );

	allcapsButton = new QToolButton( this, "allcapsButton" );
	allcapsButton->setMaximumSize( QSize( 22, 22 ) );
	allcapsButton->setText("");
	allcapsButton->setPixmap(loadIcon("AllCaps.png"));
	allcapsButton->setToggleButton( true );
	ssLayout->addWidget( allcapsButton );
	smallcapsButton = new QToolButton( this, "smallcapsButton" );
	smallcapsButton->setMaximumSize( QSize( 22, 22 ) );
	smallcapsButton->setText("");
	smallcapsButton->setPixmap(loadIcon("Kapital.xpm"));
	smallcapsButton->setToggleButton( true );
	ssLayout->addWidget( smallcapsButton );

	StrikeVal = new StrikeValues( NULL );
	StrikePop = new QPopupMenu();
	StrikePop->insertItem(StrikeVal);
	strikeoutButton = new QToolButton( this, "strikeoutButton" );
	strikeoutButton->setText( "" );
	strikeoutButton->setMaximumSize( QSize( 22, 22 ) );
	strikeoutButton->setPixmap(loadIcon("Strike.xpm"));
	strikeoutButton->setToggleButton( true );
	strikeoutButton->setPopup(StrikePop);
	strikeoutButton->setPopupDelay(400);
	ssLayout->addWidget( strikeoutButton );

	OutlineVal = new OutlineValues( NULL );
	OutlinePop = new QPopupMenu();
	OutlinePop->insertItem(OutlineVal);
	outlineButton = new QToolButton( this, "outlineButton" );
	outlineButton->setText( "" );
	outlineButton->setMaximumSize( QSize( 22, 22 ) );
	outlineButton->setPixmap(loadIcon("outlined.png"));
	outlineButton->setToggleButton( true );
	outlineButton->setPopup(OutlinePop);
	outlineButton->setPopupDelay(400);
	ssLayout->addWidget( outlineButton );

	ShadowVal = new ShadowValues( NULL );
	ShadowPop = new QPopupMenu();
	ShadowPop->insertItem(ShadowVal);
	shadowButton = new QToolButton( this, "shadowButton" );
	shadowButton->setText( "" );
	shadowButton->setMaximumSize( QSize( 22, 22 ) );
	shadowButton->setPixmap(loadIcon("shadow.png"));
	shadowButton->setToggleButton( true );
	shadowButton->setPopup(ShadowPop);
	shadowButton->setPopupDelay(400);
	ssLayout->addWidget( shadowButton );
	
	languageChange();

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

void StyleSelect::languageChange()
{
	ShadowVal->languageChange();

	QToolTip::remove(underlineButton);
	QToolTip::remove(underlineWordButton);
	QToolTip::remove(allcapsButton);
	QToolTip::remove(smallcapsButton);
	QToolTip::remove(subscriptButton);
	QToolTip::remove(superscriptButton);
	QToolTip::remove(strikeoutButton);
	QToolTip::remove(outlineButton);
	QToolTip::remove(shadowButton);

	QToolTip::add(underlineButton,  "<qt>" + tr("Underline Text. Hold down the button momentarily to set line width and displacement options."));
	QToolTip::add(underlineWordButton,  "<qt>" + tr("Underline Words Only. Hold down the button momentarily to set line width and displacement options.") + "</qt>" );
	QToolTip::add(allcapsButton, tr("All Caps"));
	QToolTip::add(smallcapsButton, tr("Small Caps"));
	QToolTip::add(subscriptButton, tr("Subscript"));
	QToolTip::add(superscriptButton, tr("Superscript"));
	QToolTip::add(strikeoutButton, "<qt>" + tr("Strike Out. Hold down the button momentarily to set line width and displacement options.") + "</qt>" );
	QToolTip::add(outlineButton, "<qt>" + tr("Outline. Hold down the button momentarily to change the outline stroke width.", "Text Style Selector") + "</qt>" );
	QToolTip::add(shadowButton,  "<qt>" + tr("Shadowed Text. Hold down the button momentarily to enable the offset spacing.") + "</qt>" );
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

