/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>

#include "styleselect.h"

#include "util_icon.h"


StrikeValues::StrikeValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout( this );
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
	group1Layout->setAlignment( Qt::AlignTop );
	LPos = new ScrSpinBox( -0.1, 100, this, 0 );
	LPos->setValue( -0.1 );
	LPos->setWrapping(true);
	LPos->setSpecialValueText( tr( "Auto" ) );
	LPosTxt = new QLabel( "Displacement", this );
	group1Layout->addWidget( LPos, 0, 1 );
	group1Layout->addWidget( LPosTxt, 0 , 0 );
	LWidth = new ScrSpinBox( -0.1, 100, this, 0 );
	LWidth->setValue( -0.1 );
	LWidth->setWrapping(true);
	LWidth->setSpecialValueText( tr( "Auto" ) );
	LWidthTxt = new QLabel( "Linewidth", this );
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

UnderlineValues::UnderlineValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout(this);
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
	group1Layout->setAlignment( Qt::AlignTop );
	LPos = new ScrSpinBox( -0.1, 100, this, 0 );
	LPos->setValue( -0.1 );
	LPos->setWrapping(true);
	LPos->setSpecialValueText( tr( "Auto" ) );
	LPosTxt = new QLabel( "Displacement", this );
	group1Layout->addWidget( LPos, 0, 1 );
	group1Layout->addWidget( LPosTxt, 0 , 0 );
	LWidth = new ScrSpinBox( -0.1, 100, this, 0 );
	LWidth->setValue( -0.1 );
	LWidth->setWrapping(true);
	LWidth->setSpecialValueText( tr( "Auto" ) );
	LWidthTxt = new QLabel( "Linewidth", this );
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

OutlineValues::OutlineValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout( this );
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
	group1Layout->setAlignment( Qt::AlignTop );
	LWidth = new ScrSpinBox( 0, 100, this, 0 );
	LWidth->setValue( 1 );
	LWidthTxt = new QLabel( "Linewidth", this );
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

ShadowValues::ShadowValues( QWidget* parent ) : QFrame( parent )
{
	group1Layout = new QGridLayout( this );
	group1Layout->setSpacing( 3 );
	group1Layout->setMargin( 0 );
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
	XoffsetTxt->setText( tr("X-Offset"));
	Yoffset->setSuffix( tr(" %"));
	YoffsetTxt->setText( tr("Y-Offset"));
	XoffsetTxt->adjustSize();
	YoffsetTxt->adjustSize();
}

StyleSelect::StyleSelect(QWidget* parent) : QWidget(parent)
{
	ssLayout = new QHBoxLayout(this);
	ssLayout->setSpacing( 0 );
	ssLayout->setMargin( 0 );

	UnderlineVal = new UnderlineValues( NULL );
	UnderlinePop = new QMenu();
	UnderlineValAct = new QWidgetAction(this);
	UnderlineValAct->setDefaultWidget(UnderlineVal);
	UnderlinePop->addAction(UnderlineValAct);
	underlineButton = new QToolButton( this );
	underlineButton->setText( "" );
	underlineButton->setMaximumSize( QSize( 22, 22 ) );
	underlineButton->setIcon(QIcon(loadIcon("Unter.xpm")));
	underlineButton->setCheckable( true );
	underlineButton->setMenu(UnderlinePop);
	underlineButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( underlineButton );
	underlineWordButton = new QToolButton( this );
	underlineWordButton->setText( "" );
	underlineWordButton->setMaximumSize( QSize( 22, 22 ) );
	underlineWordButton->setIcon(QIcon(loadIcon("wordsOnly.png")));
	underlineWordButton->setCheckable( true );
	underlineWordButton->setMenu(UnderlinePop);
	underlineWordButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( underlineWordButton );

	subscriptButton = new QToolButton( this );
	subscriptButton->setText( "" );
	subscriptButton->setMaximumSize( QSize( 22, 22 ) );
	subscriptButton->setIcon(QIcon(loadIcon("Tief.xpm")));
	subscriptButton->setCheckable( true );
	ssLayout->addWidget( subscriptButton );
	superscriptButton = new QToolButton( this );
	superscriptButton->setText( "" );
	superscriptButton->setMaximumSize( QSize( 22, 22 ) );
	superscriptButton->setIcon(QIcon(loadIcon("Hoch.xpm")));
	superscriptButton->setCheckable( true );
	ssLayout->addWidget( superscriptButton );

	allcapsButton = new QToolButton( this );
	allcapsButton->setMaximumSize( QSize( 22, 22 ) );
	allcapsButton->setText("");
	allcapsButton->setIcon(QIcon(loadIcon("AllCaps.png")));
	allcapsButton->setCheckable( true );
	ssLayout->addWidget( allcapsButton );
	smallcapsButton = new QToolButton( this );
	smallcapsButton->setMaximumSize( QSize( 22, 22 ) );
	smallcapsButton->setText("");
	smallcapsButton->setIcon(QIcon(loadIcon("Kapital.xpm")));
	smallcapsButton->setCheckable( true );
	ssLayout->addWidget( smallcapsButton );

	StrikeVal = new StrikeValues( NULL );
	StrikePop = new QMenu();
	StrikeValAct = new QWidgetAction(this);
	StrikeValAct->setDefaultWidget(StrikeVal);
	StrikePop->addAction(StrikeValAct);
	strikeoutButton = new QToolButton( this );
	strikeoutButton->setText( "" );
	strikeoutButton->setMaximumSize( QSize( 22, 22 ) );
	strikeoutButton->setIcon(QIcon(loadIcon("Strike.xpm")));
	strikeoutButton->setCheckable( true );
	strikeoutButton->setMenu(StrikePop);
	strikeoutButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( strikeoutButton );

	OutlineVal = new OutlineValues( NULL );
	OutlinePop = new QMenu();
	OutlineValAct = new QWidgetAction(this);
	OutlineValAct->setDefaultWidget(OutlineVal);
	OutlinePop->addAction(OutlineValAct);
	outlineButton = new QToolButton( this );
	outlineButton->setText( "" );
	outlineButton->setMaximumSize( QSize( 22, 22 ) );
	outlineButton->setIcon(QIcon(loadIcon("outlined.png")));
	outlineButton->setCheckable( true );
	outlineButton->setMenu(OutlinePop);
	outlineButton->setPopupMode(QToolButton::DelayedPopup);
	ssLayout->addWidget( outlineButton );

	ShadowVal = new ShadowValues( NULL );
	ShadowPop = new QMenu();
	ShadowValAct = new QWidgetAction(this);
	ShadowValAct->setDefaultWidget(ShadowVal);
	ShadowPop->addAction(ShadowValAct);
	shadowButton = new QToolButton( this );
	shadowButton->setText( "" );
	shadowButton->setMaximumSize( QSize( 22, 22 ) );
	shadowButton->setIcon(QIcon(loadIcon("shadow.png")));
	shadowButton->setCheckable( true );
	shadowButton->setMenu(ShadowPop);
	shadowButton->setPopupMode(QToolButton::DelayedPopup);
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

void StyleSelect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void StyleSelect::languageChange()
{
	ShadowVal->languageChange();
	OutlineVal->languageChange();
	UnderlineVal->languageChange();
	StrikeVal->languageChange();

	underlineButton->setToolTip("");
	underlineWordButton->setToolTip("");
	allcapsButton->setToolTip("");
	smallcapsButton->setToolTip("");
	subscriptButton->setToolTip("");
	superscriptButton->setToolTip("");
	strikeoutButton->setToolTip("");
	outlineButton->setToolTip("");
	shadowButton->setToolTip("");

	underlineButton->setToolTip(  "<qt>" + tr("Underline Text. Hold down the button momentarily to set line width and displacement options."));
	underlineWordButton->setToolTip(  "<qt>" + tr("Underline Words Only. Hold down the button momentarily to set line width and displacement options.") + "</qt>" );
	allcapsButton->setToolTip( tr("All Caps"));
	smallcapsButton->setToolTip( tr("Small Caps"));
	subscriptButton->setToolTip( tr("Subscript"));
	superscriptButton->setToolTip( tr("Superscript"));
	strikeoutButton->setToolTip( "<qt>" + tr("Strike Out. Hold down the button momentarily to set line width and displacement options.") + "</qt>" );
	outlineButton->setToolTip( "<qt>" + tr("Outline. Hold down the button momentarily to change the outline stroke width.", "Text Style Selector") + "</qt>" );
	shadowButton->setToolTip(  "<qt>" + tr("Shadowed Text. Hold down the button momentarily to enable the offset spacing.") + "</qt>" );
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

