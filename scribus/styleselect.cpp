#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3popupmenu.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QPixmap>
#include <QHBoxLayout>
#include <QGridLayout>
#include "mspinbox.h"

#include "styleselect.h"

extern QPixmap loadIcon(QString nam);

class WidgetPopupMenu : public Q3PopupMenu
{
public:
    WidgetPopupMenu(QWidget *parent = 0) : Q3PopupMenu(parent), w(0){}
    QWidget *widget() const { return w; }
    void setWidget(QWidget *widget) { w = widget; w->setParent(this); w->move(2, 2); }
    QSize sizeHint() const { return w->sizeHint() + QSize(4,4); }
    void insertItem(QWidget *widget) { setWidget(widget); }

private:
    QWidget *w;
};

StrikeValues::StrikeValues( QWidget* parent ) : QWidget( parent, "StrikeValues" )
{
	group1Layout = new QGridLayout( this );
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

UnderlineValues::UnderlineValues( QWidget* parent ) : QWidget( parent, "ShadowValues" )
{
	group1Layout = new QGridLayout( this );
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

OutlineValues::OutlineValues( QWidget* parent ) : QWidget( parent, "ShadowValues" )
{
	group1Layout = new QGridLayout( this );
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

ShadowValues::ShadowValues( QWidget* parent ) : QWidget( parent, "ShadowValues" )
{
	group1Layout = new QGridLayout( this );
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

	buttonGroup3 = new QWidget( this, "buttonGroup" );
	buttonGroup3Layout = new QHBoxLayout( buttonGroup3 );
	buttonGroup3Layout->setAlignment( Qt::AlignTop );
	UnderlineVal = new UnderlineValues( NULL );
	UnderlinePop = new WidgetPopupMenu();
	UnderlinePop->insertItem(UnderlineVal);
	underlineButton = new QToolButton( buttonGroup3, "underlineButton" );
	underlineButton->setText( "" );
	underlineButton->setMaximumSize( QSize( 22, 22 ) );
	underlineButton->setPixmap(loadIcon("Unter.xpm"));
	underlineButton->setToggleButton( true );
	underlineButton->setPopup(UnderlinePop);
	underlineButton->setPopupDelay(400);
	buttonGroup3Layout->addWidget( underlineButton );
	underlineWordButton = new QToolButton( buttonGroup3, "underlineButton" );
	underlineWordButton->setText( "" );
	underlineWordButton->setMaximumSize( QSize( 22, 22 ) );
	underlineWordButton->setPixmap(loadIcon("wordsOnly.png"));
	underlineWordButton->setToggleButton( true );
	underlineWordButton->setPopup(UnderlinePop);
	underlineWordButton->setPopupDelay(400);
	buttonGroup3Layout->addWidget( underlineWordButton );
	ssLayout->addWidget( buttonGroup3 );

	buttonGroup = new QWidget( this, "buttonGroup" );
	buttonGroupLayout = new QHBoxLayout( buttonGroup );
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

	buttonGroup2 = new QWidget( this, "buttonGroup" );
	buttonGroup2Layout = new QHBoxLayout( buttonGroup2 );
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

	buttonGroup4 = new QWidget( this, "buttonGroup" );
	buttonGroup4Layout = new QHBoxLayout( buttonGroup4 );
	buttonGroup4Layout->setAlignment( Qt::AlignTop );
	StrikeVal = new StrikeValues( NULL );
	StrikePop = new WidgetPopupMenu();
	StrikePop->insertItem(StrikeVal);
	strikeoutButton = new QToolButton( buttonGroup4, "strikeoutButton" );
	strikeoutButton->setText( "" );
	strikeoutButton->setMaximumSize( QSize( 22, 22 ) );
	strikeoutButton->setPixmap(loadIcon("Strike.xpm"));
	strikeoutButton->setToggleButton( true );
	strikeoutButton->setPopup(StrikePop);
	strikeoutButton->setPopupDelay(400);
	buttonGroup4Layout->addWidget( strikeoutButton );

	OutlineVal = new OutlineValues( NULL );
	OutlinePop = new WidgetPopupMenu();
	OutlinePop->insertItem(OutlineVal);
	outlineButton = new QToolButton( buttonGroup4, "outlineButton" );
	outlineButton->setText( "" );
	outlineButton->setMaximumSize( QSize( 22, 22 ) );
	outlineButton->setPixmap(loadIcon("outlined.png"));
	outlineButton->setToggleButton( true );
	outlineButton->setPopup(OutlinePop);
	outlineButton->setPopupDelay(400);
	buttonGroup4Layout->addWidget( outlineButton );

	ShadowVal = new ShadowValues( NULL );
	ShadowPop = new WidgetPopupMenu();
	ShadowPop->insertItem(ShadowVal);
	shadowButton = new QToolButton( buttonGroup4, "shadowButton" );
	shadowButton->setText( "" );
	shadowButton->setMaximumSize( QSize( 22, 22 ) );
	shadowButton->setPixmap(loadIcon("shadow.png"));
	shadowButton->setToggleButton( true );
	shadowButton->setPopup(ShadowPop);
	shadowButton->setPopupDelay(400);
	buttonGroup4Layout->addWidget( shadowButton );
	ssLayout->addWidget( buttonGroup4 );
	
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

	QToolTip::add(underlineButton, tr("Underline"));
	QToolTip::add(underlineWordButton, tr("Underline Words Only"));
	QToolTip::add(allcapsButton, tr("All Caps"));
	QToolTip::add(smallcapsButton, tr("Small Caps"));
	QToolTip::add(subscriptButton, tr("Subscript"));
	QToolTip::add(superscriptButton, tr("Superscript"));
	QToolTip::add(strikeoutButton, tr("Strike Out"));
	QToolTip::add(outlineButton, tr("Outline"));
	QToolTip::add(shadowButton, tr("Shadow"));
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

