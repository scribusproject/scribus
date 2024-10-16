/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>

#include "iconmanager.h"
#include "scribusapp.h"
#include "styleselect.h"

StrikeValues::StrikeValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout( this );
	group1Layout->setSpacing(4);
	group1Layout->setContentsMargins(8, 8, 8, 8);
	group1Layout->setAlignment( Qt::AlignTop );
	LPos = new ScrSpinBox( -0.1, 100, this, 0 );
	LPos->setValue( -0.1 );
	LPos->setWrapping(true);
	LPosTxt = new QLabel( "Displacement:", this );
	group1Layout->addWidget( LPos, 0, 1 );
	group1Layout->addWidget( LPosTxt, 0 , 0 );
	LWidth = new ScrSpinBox( -0.1, 100, this, 0 );
	LWidth->setValue( -0.1 );
	LWidth->setWrapping(true);
	LWidthTxt = new QLabel( "Linewidth:", this );
	group1Layout->addWidget( LWidth, 1, 1 );
	group1Layout->addWidget( LWidthTxt, 1 , 0 );
	languageChange();
}

void StrikeValues::languageChange()
{
	LPos->setSuffix( tr(" %"));
	LPos->setSpecialValueText( tr( "Auto" ) );
	LPosTxt->setText( tr("Displacement:"));
	LWidth->setSuffix( tr(" %"));
	LWidth->setSpecialValueText( tr( "Auto" ) );
	LWidthTxt->setText( tr("Linewidth:"));
	LPosTxt->adjustSize();
	LWidthTxt->adjustSize();
}

UnderlineValues::UnderlineValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout(this);
	group1Layout->setSpacing(4);
	group1Layout->setContentsMargins(8, 8, 8, 8);
	group1Layout->setAlignment( Qt::AlignTop );
	LPos = new ScrSpinBox( -0.1, 100, this, 0 );
	LPos->setValue( -0.1 );
	LPos->setWrapping(true);
	LPosTxt = new QLabel( "Displacement:", this );
	group1Layout->addWidget( LPos, 0, 1 );
	group1Layout->addWidget( LPosTxt, 0 , 0 );
	LWidth = new ScrSpinBox( -0.1, 100, this, 0 );
	LWidth->setValue( -0.1 );
	LWidth->setWrapping(true);
	LWidthTxt = new QLabel( "Linewidth:", this );
	group1Layout->addWidget( LWidth, 1, 1 );
	group1Layout->addWidget( LWidthTxt, 1 , 0 );
	languageChange();
}

void UnderlineValues::languageChange()
{
	LPos->setSpecialValueText( tr("Auto") );
	LPos->setSuffix( tr(" %"));
	LPosTxt->setText( tr("Displacement:"));
	LWidth->setSpecialValueText( tr("Auto") );
	LWidth->setSuffix( tr(" %"));
	LWidthTxt->setText( tr("Linewidth:"));
	LPosTxt->adjustSize();
	LWidthTxt->adjustSize();
}

OutlineValues::OutlineValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout( this );
	group1Layout->setSpacing(4);
	group1Layout->setContentsMargins(8, 8, 8, 8);
	group1Layout->setAlignment( Qt::AlignTop );
	LWidth = new ScrSpinBox( 0, 100, this, 0 );
	LWidth->setValue( 1 );
	LWidthTxt = new QLabel( "Linewidth:", this );
	group1Layout->addWidget( LWidth, 0, 1 );
	group1Layout->addWidget( LWidthTxt, 0 , 0 );
	languageChange();
}

void OutlineValues::languageChange()
{
	LWidth->setSuffix( tr(" %"));
	LWidthTxt->setText( tr("Linewidth:"));
	LWidthTxt->adjustSize();
}

ShadowValues::ShadowValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout( this );
	group1Layout->setSpacing(4);
	group1Layout->setContentsMargins(8, 8, 8, 8);
	group1Layout->setAlignment( Qt::AlignTop );
	Xoffset = new ScrSpinBox( -100, 100, this, 0 );
	Xoffset->setValue( 5 );
	XoffsetTxt = new QLabel( "X-Offset", this );
	group1Layout->addWidget( Xoffset, 0, 1 );
	group1Layout->addWidget( XoffsetTxt, 0 , 0 );
	Yoffset = new ScrSpinBox( -100, 100, this, 0 );
	Yoffset->setValue( 5 );
	YoffsetTxt = new QLabel( "Y-Offset", this );
	group1Layout->addWidget( Yoffset, 1, 1 );
	group1Layout->addWidget( YoffsetTxt, 1 , 0 );
	languageChange();
}

void ShadowValues::languageChange()
{
	Xoffset->setSuffix( tr(" %"));
	XoffsetTxt->setText( tr("X-Offset:"));
	Yoffset->setSuffix( tr(" %"));
	YoffsetTxt->setText( tr("Y-Offset:"));
	XoffsetTxt->adjustSize();
	YoffsetTxt->adjustSize();
}

StyleSelect::StyleSelect(QWidget* parent) : QWidget(parent)
{
	IconManager& im = IconManager::instance();
	ssLayout = new QHBoxLayout(this);
	ssLayout->setSpacing(4);
	ssLayout->setContentsMargins(0, 0, 0, 0);

	UnderlineVal = new UnderlineValues( nullptr );
	UnderlinePop = new QMenu();
	UnderlineValAct = new QWidgetAction(this);
	UnderlineValAct->setDefaultWidget(UnderlineVal);
	UnderlinePop->addAction(UnderlineValAct);
	underlineButton = new QToolButton( this );
	underlineButton->setText( "" );
	underlineButton->setCheckable( true );
	underlineButton->setMenu(UnderlinePop);
	underlineButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( underlineButton );
	underlineWordButton = new QToolButton( this );
	underlineWordButton->setText( "" );
	underlineWordButton->setCheckable( true );
	underlineWordButton->setMenu(UnderlinePop);
	underlineWordButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( underlineWordButton );	
	StrikeVal = new StrikeValues( nullptr );
	StrikePop = new QMenu();
	StrikeValAct = new QWidgetAction(this);
	StrikeValAct->setDefaultWidget(StrikeVal);
	StrikePop->addAction(StrikeValAct);
	strikeoutButton = new QToolButton( this );
	strikeoutButton->setText( "" );
	strikeoutButton->setCheckable( true );
	strikeoutButton->setMenu(StrikePop);
	strikeoutButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( strikeoutButton );

	ssLayout->addSpacing(12);

	subscriptButton = new QToolButton( this );
	subscriptButton->setText( "" );
	subscriptButton->setCheckable( true );
	ssLayout->addWidget( subscriptButton );
	superscriptButton = new QToolButton( this );
	superscriptButton->setText( "" );
	superscriptButton->setCheckable( true );
	ssLayout->addWidget( superscriptButton );

	ssLayout->addSpacing(12);

	allcapsButton = new QToolButton( this );
	allcapsButton->setText("");
	allcapsButton->setCheckable( true );
	ssLayout->addWidget( allcapsButton );
	smallcapsButton = new QToolButton( this );
	smallcapsButton->setText("");
	smallcapsButton->setCheckable( true );
	ssLayout->addWidget( smallcapsButton );

	ssLayout->addSpacing(12);

	OutlineVal = new OutlineValues( nullptr );
	OutlinePop = new QMenu();
	OutlineValAct = new QWidgetAction(this);
	OutlineValAct->setDefaultWidget(OutlineVal);
	OutlinePop->addAction(OutlineValAct);
	outlineButton = new QToolButton( this );
	outlineButton->setText( "" );
	outlineButton->setCheckable( true );
	outlineButton->setMenu(OutlinePop);
	outlineButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( outlineButton );

	ShadowVal = new ShadowValues( nullptr );
	ShadowPop = new QMenu();
	ShadowValAct = new QWidgetAction(this);
	ShadowValAct->setDefaultWidget(ShadowVal);
	ShadowPop->addAction(ShadowValAct);
	shadowButton = new QToolButton( this );
	shadowButton->setText( "" );
	shadowButton->setCheckable( true );
	shadowButton->setMenu(ShadowPop);
	shadowButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( shadowButton );

	languageChange();
	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

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

void StyleSelect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void StyleSelect::iconSetChange()
{
	IconManager& im = IconManager::instance();
	
	underlineButton->setIcon(im.loadIcon("character-underline"));
	underlineWordButton->setIcon(im.loadIcon("character-underline-word"));
	subscriptButton->setIcon(im.loadIcon("character-subscript"));
	superscriptButton->setIcon(im.loadIcon("character-superscript"));
	allcapsButton->setIcon(im.loadIcon("character-capitals-all"));
	smallcapsButton->setIcon(im.loadIcon("character-capitals-small"));
	strikeoutButton->setIcon(im.loadIcon("character-strikethrough"));
	outlineButton->setIcon(im.loadIcon("character-outline"));
	shadowButton->setIcon(im.loadIcon("character-shadow"));

}

void StyleSelect::languageChange()
{
	ShadowVal->languageChange();
	OutlineVal->languageChange();
	UnderlineVal->languageChange();
	StrikeVal->languageChange();

	underlineButton->setToolTip(  "<qt>" + tr("Underline Text. Hold down the button momentarily to set line width and displacement options.") + "</qt>");
	underlineWordButton->setToolTip(  "<qt>" + tr("Underline Words Only. Hold down the button momentarily to set line width and displacement options.") + "</qt>" );
	allcapsButton->setToolTip( "<qt>" + tr("All Caps") + "</qt>");
	smallcapsButton->setToolTip( "<qt>" + tr("Fake Small Caps") + "</qt>");
	subscriptButton->setToolTip( "<qt>" + tr("Subscript") + "</qt>");
	superscriptButton->setToolTip( "<qt>" + tr("Superscript") + "</qt>");
	strikeoutButton->setToolTip( "<qt>" + tr("Strike Out. Hold down the button momentarily to set line width and displacement options.") + "</qt>" );
	outlineButton->setToolTip( "<qt>" + tr("Outline. Hold down the button momentarily to change the outline stroke width.") + "</qt>" );
	shadowButton->setToolTip(  "<qt>" + tr("Shadowed Text. Hold down the button momentarily to change the shadow offset values.") + "</qt>" );
}

void StyleSelect::setStyle(int s)
{
	superscriptButton->setChecked(false);
	subscriptButton->setChecked(false);
	strikeoutButton->setChecked(false);
	underlineButton->setChecked(false);
	underlineWordButton->setChecked(false);
	allcapsButton->setChecked(false);
	smallcapsButton->setChecked(false);
	outlineButton->setChecked(false);
	shadowButton->setChecked(false);
	if (s & 1)
		superscriptButton->setChecked(true);
	if (s & 2)
		subscriptButton->setChecked(true);
	if (s & 4)
		outlineButton->setChecked(true);
	if (s & 8)
		underlineButton->setChecked(true);
	if (s & 16)
		strikeoutButton->setChecked(true);
	if (s & 32)
		allcapsButton->setChecked(true);
	if (s & 64)
		smallcapsButton->setChecked(true);
	if (s & 256)
		shadowButton->setChecked(true);
	if (s & 512)
		underlineWordButton->setChecked(true);
}

int StyleSelect::getStyle()
{
	int ret = 0;
	if (superscriptButton->isChecked())
		ret |= 1;
	if (subscriptButton->isChecked())
		ret |= 2;
	if (outlineButton->isChecked())
		ret |= 4;
	if (underlineButton->isChecked())
		ret |= 8;
	if (strikeoutButton->isChecked())
		ret |= 16;
	if (allcapsButton->isChecked())
		ret |= 32;
	if (smallcapsButton->isChecked())
		ret |= 64;
	if (shadowButton->isChecked())
		ret |= 256;
	if (underlineWordButton->isChecked())
		ret |= 512;
	return ret;
}

void StyleSelect::setTypeStyle()
{
	if (superscriptButton == sender())
		subscriptButton->setChecked(false);
	if (subscriptButton == sender())
		superscriptButton->setChecked(false);
	if (allcapsButton == sender())
		smallcapsButton->setChecked(false);
	if (smallcapsButton == sender())
		allcapsButton->setChecked(false);
	if (underlineWordButton == sender())
		underlineButton->setChecked(false);
	if (underlineButton == sender())
		underlineWordButton->setChecked(false);
	emit State(getStyle());
}

