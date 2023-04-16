/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "buttonicon.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QSlider>
#include <QSpacerItem>
#include <QVBoxLayout>

#include "iconmanager.h"
#include "pageitem.h"
#include "scfonts.h"
#include "scribusstructs.h"

ButtonIcon::ButtonIcon(QWidget* parent, PageItem* ite)
	: QDialog(parent),
	  m_item(ite)
{
	setModal(true);
	setWindowTitle( tr( "Icon Placement" ) );
	setWindowIcon(IconManager::instance().loadPixmap("AppIcon.png"));

	auto* ButtonIconLayout = new QVBoxLayout( this );
	ButtonIconLayout->setContentsMargins(9, 9, 9, 9);
	ButtonIconLayout->setSpacing(6);

	auto* Layout1 = new QGridLayout;
	Layout1->setContentsMargins(0, 0, 0, 0);
	Layout1->setSpacing(6);

	TextLabel1_2 = new QLabel( this );
	TextLabel1_2->setText( tr( "Layout:" ) );
	Layout1->addWidget( TextLabel1_2, 0, 0 );

	Place = new QComboBox(this);
	QString tmp_pla[] = { tr("Caption only"), tr("Icon only"), tr("Caption below Icon"), tr("Caption above Icon"),
	                      tr("Caption right to Icon"), tr("Caption left to Icon"), tr("Caption overlays Icon") };
	for (const QString& place : tmp_pla)
		Place->addItem(place);
	Place->setEditable(false);
	Layout1->addWidget( Place, 0, 1 );

	TextLabel1 = new QLabel( this );
	TextLabel1->setText( tr( "Scale:" ) );
	Layout1->addWidget( TextLabel1, 1, 0 );

	ScaleW = new QComboBox(this);
	QString scalew[] = { tr("Always"), tr("When Icon is too small"),
	                     tr("When Icon is too big"), tr("Never") };
	for (const QString& scale : scalew)
		ScaleW->addItem(scale);
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

	auto* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ButtonIconLayout->addItem( spacer );

	auto* Layout5 = new QHBoxLayout;
	Layout5->setContentsMargins(0, 0, 0, 0);
	Layout5->setSpacing(6);

	auto* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout5->addItem( spacer_2 );

	auto* Layout3 = new QGridLayout;
	Layout3->setContentsMargins(0, 0, 0, 0);
	Layout3->setSpacing(6);

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

	auto* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout5->addItem( spacer_3 );

	ButtonIconLayout->addLayout( Layout5 );

	auto* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ButtonIconLayout->addItem( spacer_4 );

	auto* Layout4 = new QHBoxLayout;
	Layout4->setContentsMargins(0, 0, 0, 0);
	Layout4->setSpacing(6);

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

int ButtonIcon::place() const
{
	return Place->currentIndex();
}

int ButtonIcon::scaleH() const
{
	return ScaleH->currentIndex();
}

int ButtonIcon::scaleW() const
{
	return ScaleW->currentIndex();
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
	Place->setCurrentIndex(m_item->annotation().IPlace());
	ScaleW->setCurrentIndex(m_item->annotation().ScaleW());
	ScaleH->setCurrentIndex(m_item->imageXScale() != m_item->imageYScale() ? 1 : 0);
	SetScaleHow(ScaleW->currentIndex());
	SliderX->setValue(static_cast<int>(m_item->imageXOffset() / (m_item->width() - m_item->pixm.width()) * 1000));
	SliderY->setValue(static_cast<int>(m_item->imageYOffset() / (m_item->height() - m_item->pixm.height()) * 1000));
	TextLabel3->setGeometry(QRect(static_cast<int>(SliderX->value() / 1000.0 * 100),
	                              static_cast<int>(SliderY->value() / 1000.0 * 100), 48, 48));
}

void ButtonIcon::SetScaleHow(int s)
{
	bool setter = s == 3;
	GroupButton->setEnabled(setter);
	TextLabel3->setEnabled(setter);
	SliderX->setEnabled(setter);
	SliderY->setEnabled(setter);
	IcScaleH = s;
}
