/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "buttonicon.h"

#include <QPixmap>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QComboBox>
#include <QGroupBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

#include "scfonts.h"
#include "scribusstructs.h"
#include "pageitem.h"
#include "util_icon.h"

ButtonIcon::ButtonIcon(QWidget* parent, PageItem* ite) : QDialog( parent )
{
	Item = ite;
	setModal(true);
	setWindowTitle( tr( "Icon Placement" ) );
	setWindowIcon(loadIcon("AppIcon.png"));
	ButtonIconLayout = new QVBoxLayout( this );
	ButtonIconLayout->setMargin(10);
	ButtonIconLayout->setSpacing(5);
	Layout1 = new QGridLayout;
	Layout1->setMargin(0);
	Layout1->setSpacing(5);
	TextLabel1_2 = new QLabel( this );
	TextLabel1_2->setText( tr( "Layout:" ) );
	Layout1->addWidget( TextLabel1_2, 0, 0 );
	Place = new QComboBox(this);
	QString tmp_pla[] = { tr("Caption only"), tr("Icon only"), tr("Caption below Icon"), tr("Caption above Icon"),
	                      tr("Caption right to Icon"), tr("Caption left to Icon"), tr("Caption overlays Icon")};
	size_t array_pla = sizeof(tmp_pla) / sizeof(*tmp_pla);
	/* PFJ - 29/02/04 - Changed from uint to int and var name */
	for (uint prop = 0; prop < array_pla; ++prop)
		Place->addItem(tmp_pla[prop]);
	Place->setEditable(false);
	Layout1->addWidget( Place, 0, 1 );
	TextLabel1 = new QLabel( this );
	TextLabel1->setText( tr( "Scale:" ) );
	Layout1->addWidget( TextLabel1, 1, 0 );
	ScaleW = new QComboBox(this);
	/* PFJ - 29/02/04 - Changed to QString/size_t/for style. It saves a small
						of time during the setup */
	QString scalew[] = { tr("Always"), tr("When Icon is too small"),
	                    tr("When Icon is too big"), tr("Never")};
	size_t scalewArray = sizeof(scalew) / sizeof(*scalew);
	for (uint prop = 0; prop < scalewArray; ++prop)
		ScaleW->addItem(scalew[prop]);
	ScaleW->setEditable(false);
	Layout1->addWidget( ScaleW, 1, 1 );
	TextLabel2 = new QLabel( this );
	TextLabel2->setText( tr( "Scale How:" ) );
	Layout1->addWidget( TextLabel2, 2, 0 );
	ScaleH = new QComboBox(this);
	ScaleH->addItem( tr( "Proportional" ) );
	ScaleH->addItem( tr( "Non Proportional" ) );
	ScaleH->setEditable(false);
	Layout1->addWidget( ScaleH, 2, 1 );
	ButtonIconLayout->addLayout( Layout1 );
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ButtonIconLayout->addItem( spacer );
	Layout5 = new QHBoxLayout;
	Layout5->setMargin(0);
	Layout5->setSpacing(5);
	QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout5->addItem( spacer_2 );
	Layout3 = new QGridLayout;
	Layout3->setMargin(0);
	Layout3->setSpacing(5);
	GroupButton = new QGroupBox( this );
	GroupButton->setMinimumSize( QSize( 150, 150 ) );
	GroupButton->setMaximumSize( QSize( 150, 150 ) );
	GroupButton->setTitle( "" );
	TextLabel3 = new QLabel( GroupButton );
	TextLabel3->setGeometry( QRect( 50, 50, 48, 48 ) );
	TextLabel3->setFrameShape( QLabel::Box );
	TextLabel3->setFrameShadow( QLabel::Raised );
	TextLabel3->setLineWidth( 1 );
	TextLabel3->setText( tr( "Icon" ) );
	Layout3->addWidget( GroupButton, 0, 0 );
	SliderX = new QSlider( this );
	SliderX->setMaximum( 1000 );
	SliderX->setValue( 500 );
	SliderX->setOrientation( Qt::Horizontal );
	SliderX->setTickPosition(QSlider::TicksLeft);
	SliderX->setTickInterval( 100 );
	Layout3->addWidget( SliderX, 1, 0 );
	SliderY = new QSlider( this );
	SliderY->setMaximum( 1000 );
	SliderY->setValue( 500 );
	SliderY->setOrientation( Qt::Vertical );
	SliderY->setTickPosition(QSlider::TicksLeft);
	SliderY->setTickInterval( 100 );
	SliderY->setInvertedAppearance(true);
	Layout3->addWidget( SliderY, 0, 1 );
	Layout5->addLayout( Layout3 );
	QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout5->addItem( spacer_3 );
	ButtonIconLayout->addLayout( Layout5 );
	QSpacerItem* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ButtonIconLayout->addItem( spacer_4 );
	Layout4 = new QHBoxLayout;
	Layout4->setMargin(0);
	Layout4->setSpacing(5);
	OK = new QPushButton( this );
	OK->setText( tr( "OK" ) );
	OK->setDefault( true );
	Layout4->addWidget( OK );
	Cancel = new QPushButton( this );
	Cancel->setText( tr( "Cancel" ) );
	Layout4->addWidget( Cancel );
	Reset = new QPushButton( this );
	Reset->setText( tr( "Reset" ) );
	Layout4->addWidget( Reset );
	ButtonIconLayout->addLayout( Layout4 );

	SetAllVals();

	connect(ScaleW, SIGNAL(activated(int)), this, SLOT(SetScaleHow(int)));
	connect(SliderX, SIGNAL(valueChanged(int)), this, SLOT(MoveIconX(int)));
	connect(SliderY, SIGNAL(valueChanged(int)), this, SLOT(MoveIconY(int)));
	connect(Cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(Reset, SIGNAL(clicked()), this, SLOT(SetAllVals()));
}

void ButtonIcon::MoveIconY(int y)
{
	TextLabel3->setGeometry(QRect(static_cast<int>(SliderX->value() / 1000.0 * 100),
	                              static_cast<int>(y / 1000.0 * 100), 48, 48));
	IcPlaceY = y / 1000.0;
}

void ButtonIcon::MoveIconX(int x)
{
	TextLabel3->setGeometry(QRect(static_cast<int>(x / 1000.0 * 100),
	                              static_cast<int>(SliderY->value() / 1000.0 * 100), 48, 48));
	IcPlaceX = x / 1000.0;
}

void ButtonIcon::SetAllVals()
{
	Place->setCurrentIndex(Item->annotation().IPlace());
	ScaleW->setCurrentIndex(Item->annotation().ScaleW());
	ScaleH->setCurrentIndex(Item->imageXScale() != Item->imageYScale() ? 1 : 0);
	SetScaleHow(ScaleW->currentIndex());
	SliderX->setValue(static_cast<int>(Item->imageXOffset() / (Item->width() - Item->pixm.width()) * 1000));
	SliderY->setValue(static_cast<int>(Item->imageYOffset() / (Item->height() - Item->pixm.height()) * 1000));
	TextLabel3->setGeometry(QRect(static_cast<int>(SliderX->value() / 1000.0 * 100),
	                              static_cast<int>(SliderY->value() / 1000.0 * 100), 48, 48));
}

void ButtonIcon::SetScaleHow(int s)
{
	bool setter = (s == 3) ? true : false;
	GroupButton->setEnabled(setter);
	TextLabel3->setEnabled(setter);
	SliderX->setEnabled(setter);
	SliderY->setEnabled(setter);
	IcScaleH = s;
}
