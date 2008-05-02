/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "oneclick.h"
#include <QPixmap>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QFrame>
#include <QToolTip>
#include <QSpacerItem>
#include "util_icon.h"

#include "scrspinbox.h"
#include "linkbutton.h"
#include "units.h"
#include "commonstrings.h"

OneClick::OneClick( QWidget* parent, QString titel, int unitIndex, double defW, double defH, bool remember, int origin, int mode ) : QDialog( parent )
{
	double m_unitRatio = unitGetRatioFromIndex(unitIndex);
	QString m_suffix = unitGetSuffixFromIndex(unitIndex);
	setWindowTitle( titel );
	setWindowIcon(loadIcon("AppIcon.png"));
	queryLayout = new QVBoxLayout( this );
	queryLayout->setSpacing( 5 );
	queryLayout->setMargin( 10 );
	editLayout = new QGridLayout();
	editLayout->setSpacing( 5 );
	editLayout->setMargin( 0 );
	RotationGroupContainer = new QGroupBox( tr("Origin"), this);
	Layout12a = new QGridLayout(RotationGroupContainer);
	Layout12a->setSpacing( 0 );
	Layout12a->setMargin( 5 );
	Layout12a->setAlignment( Qt::AlignTop );
	QSpacerItem* spacerT = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout12a->addItem( spacerT, 0, 1 );
	RotationGroup = new QButtonGroup(RotationGroupContainer);
	QSpacerItem* spacerL = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerL, 1, 0 );
	Layout12 = new QGridLayout();
	Layout12->setSpacing( 0 );
	Layout12->setMargin( 0 );
	TopLeft = new QRadioButton( RotationGroupContainer );
	TopLeft->setText( "" );
	TopLeft->setChecked( true );
	TopLeft->setLayoutDirection(Qt::RightToLeft);
	TopLeft->setMaximumSize( TopLeft->iconSize() );
	RotationGroup->addButton(TopLeft, 0);
	Layout12->addWidget( TopLeft, 0, 0, Qt::AlignCenter );
	if (mode == 0)
	{
		Line1 = new QFrame( RotationGroupContainer );
		Line1->setMinimumSize( QSize( 20, 4 ) );
		Line1->setMaximumSize( QSize( 20, 4 ) );
		Line1->setFrameShape( QFrame::HLine );
		Line1->setFrameShadow( QFrame::Plain );
		Line1->setLineWidth( 3 );
		Layout12->addWidget( Line1, 0, 1, Qt::AlignCenter );
		TopRight = new QRadioButton( RotationGroupContainer );
		TopRight->setText( "" );
		TopRight->setMaximumSize( TopRight->iconSize() );
		RotationGroup->addButton(TopRight, 1);
		Layout12->addWidget( TopRight, 0, 2, Qt::AlignCenter );
		Line2 = new QFrame( RotationGroupContainer);
		Line2->setMinimumSize( QSize( 4, 20 ) );
		Line2->setMaximumSize( QSize( 4, 20 ) );
		Line2->setFrameShape( QFrame::VLine );
		Line2->setFrameShadow( QFrame::Plain );
		Line2->setLineWidth( 3 );
		Layout12->addWidget( Line2, 1, 0, Qt::AlignCenter );
		Center = new QRadioButton( RotationGroupContainer);
		Center->setText( "" );
		Center->setMaximumSize( Center->iconSize() );
		RotationGroup->addButton(Center, 2);
		Layout12->addWidget( Center, 1, 1, Qt::AlignCenter );
		Line4 = new QFrame( RotationGroupContainer );
		Line4->setMinimumSize( QSize( 4, 20 ) );
		Line4->setMaximumSize( QSize( 4, 20 ) );
		Line4->setFrameShadow( QFrame::Plain );
		Line4->setLineWidth( 3 );
		Line4->setFrameShape( QFrame::VLine );
		Layout12->addWidget( Line4, 1, 2, Qt::AlignCenter );
		BottomLeft = new QRadioButton( RotationGroupContainer );
		BottomLeft->setText( "" );
		BottomLeft->setLayoutDirection(Qt::RightToLeft);
		BottomLeft->setMaximumSize( BottomLeft->iconSize() );
		RotationGroup->addButton(BottomLeft, 3);
		Layout12->addWidget( BottomLeft, 2, 0, Qt::AlignCenter );
		Line5 = new QFrame( RotationGroupContainer );
		Line5->setMinimumSize( QSize( 20, 4 ) );
		Line5->setMaximumSize( QSize( 20, 4 ) );
		Line5->setFrameShape( QFrame::HLine );
		Line5->setFrameShadow( QFrame::Plain );
		Line5->setLineWidth( 3 );
		Layout12->addWidget( Line5, 2, 1, Qt::AlignCenter );
		BottomRight = new QRadioButton( RotationGroupContainer );
		BottomRight->setText( "" );
		BottomRight->setMaximumSize( BottomRight->iconSize() );
		RotationGroup->addButton(BottomRight, 4);
		Layout12->addWidget( BottomRight, 2, 2, Qt::AlignCenter );
	}
	else
	{
		Line1 = new QFrame( RotationGroupContainer );
		Line1->setMinimumSize( QSize( 10, 4 ) );
		Line1->setMaximumSize( QSize( 10, 4 ) );
		Line1->setFrameShape( QFrame::HLine );
		Line1->setFrameShadow( QFrame::Plain );
		Line1->setLineWidth( 3 );
		Layout12->addWidget( Line1, 0, 1, Qt::AlignCenter );
		Center = new QRadioButton( RotationGroupContainer );
		Center->setText( "" );
		Center->setMaximumSize( Center->iconSize() );
		RotationGroup->addButton(Center, 1);
		Layout12->addWidget( Center, 0, 2, Qt::AlignCenter );
		Line2 = new QFrame( RotationGroupContainer );
		Line2->setMinimumSize( QSize( 10, 4 ) );
		Line2->setMaximumSize( QSize( 10, 4 ) );
		Line2->setFrameShape( QFrame::HLine );
		Line2->setFrameShadow( QFrame::Plain );
		Line2->setLineWidth( 3 );
		Layout12->addWidget( Line2, 0, 3, Qt::AlignCenter );
		TopRight = new QRadioButton( RotationGroupContainer );
		TopRight->setText( "" );
		TopRight->setMaximumSize( TopRight->iconSize() );
		RotationGroup->addButton(TopRight, 2);
		Layout12->addWidget( TopRight, 0, 4, Qt::AlignLeft );
	}
	Layout12a->addLayout(Layout12, 1, 1);
	QSpacerItem* spacerR = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerR, 1, 2 );
	QSpacerItem* spacerB = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout12a->addItem( spacerB, 2, 1 );
	editLayout->addWidget( RotationGroupContainer, 0, 0 );
	RotationGroup->button(origin)->setChecked(true);
	SizeGroup = new QGroupBox( tr("Size"), this );
	SizeGroupLayout = new QGridLayout(SizeGroup);
	SizeGroupLayout->setSpacing( 5 );
	SizeGroupLayout->setMargin( 10 );
	SizeGroupLayout->setAlignment( Qt::AlignTop );
	questionLabel = new QLabel( tr("Width:"), SizeGroup );
	if (mode == 1)
		questionLabel->setText( tr("Length:"));
	SizeGroupLayout->addWidget( questionLabel, 0, 0 );
	spinWidth = new ScrSpinBox( 0, 1000, SizeGroup, unitIndex );
	spinWidth->setSuffix( m_suffix );
	spinWidth->setMinimum(0.1*m_unitRatio);
	spinWidth->setMaximum(30000.0*m_unitRatio);
	spinWidth->setValue(defW * m_unitRatio);
	SizeGroupLayout->addWidget( spinWidth, 0, 1 );
	questionLabel2 = new QLabel( tr("Height:"), SizeGroup );
	if (mode == 1)
		questionLabel2->setText( tr("Angle:"));
	SizeGroupLayout->addWidget( questionLabel2, 1, 0 );
	spinHeight = new ScrSpinBox( 0, 1000, SizeGroup, unitIndex );
	//Qt4 FIXME what are these.. 
	if (mode == 0)
	{
		spinHeight->setSuffix( m_suffix );
		spinHeight->setMinimum(0.1*m_unitRatio);
		spinHeight->setMaximum(30000.0*m_unitRatio);
		spinHeight->setValue(defH * m_unitRatio);
	}
	else
	{
		spinHeight->setSuffix(unitGetSuffixFromIndex(6));
		spinHeight->setWrapping( true );
		spinHeight->setValues( 0, 359.99, 2, 0);
		spinHeight->setValue(defH);
	}
	SizeGroupLayout->addWidget( spinHeight, 1, 1 );
	if (mode == 0)
	{
		linkSize = new LinkButton( SizeGroup );
		linkSize->setCheckable( true );
		linkSize->setAutoRaise( true );
		linkSize->setMaximumSize( QSize( 15, 32767 ) );
		SizeGroupLayout->addWidget( linkSize, 0, 2, 2, 1 );
	}
	editLayout->addWidget( SizeGroup, 0, 1 );

	checkRemember = new QCheckBox(this);
	checkRemember->setText( tr("Remember Values"));
	checkRemember->setChecked(remember);
	editLayout->addWidget( checkRemember, 1, 0, 1, 2 );

	queryLayout->addLayout( editLayout );
	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	okCancelLayout->addWidget( cancelButton );
	queryLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	if (mode == 0)
	{
		connect(linkSize, SIGNAL(clicked()), this, SLOT(ToggleKette()));
		connect(spinWidth, SIGNAL(valueChanged(double)), this, SLOT(changeSizesW()));
		connect(spinHeight, SIGNAL(valueChanged(double)), this, SLOT(changeSizesH()));
	}
}

void OneClick::ToggleKette()
{
	disconnect(spinWidth, SIGNAL(valueChanged(double)), this, SLOT(changeSizesW()));
	disconnect(spinHeight, SIGNAL(valueChanged(double)), this, SLOT(changeSizesH()));
	if (linkSize->isChecked())
		spinHeight->setValue(spinWidth->value());
	connect(spinWidth, SIGNAL(valueChanged(double)), this, SLOT(changeSizesW()));
	connect(spinHeight, SIGNAL(valueChanged(double)), this, SLOT(changeSizesH()));
}

void OneClick::changeSizesH()
{
	disconnect(spinWidth, SIGNAL(valueChanged(double)), this, SLOT(changeSizesW()));
	disconnect(spinHeight, SIGNAL(valueChanged(double)), this, SLOT(changeSizesH()));
	if (linkSize->isChecked())
		spinWidth->setValue(spinHeight->value());
	connect(spinWidth, SIGNAL(valueChanged(double)), this, SLOT(changeSizesW()));
	connect(spinHeight, SIGNAL(valueChanged(double)), this, SLOT(changeSizesH()));
}

void OneClick::changeSizesW()
{
	disconnect(spinWidth, SIGNAL(valueChanged(double)), this, SLOT(changeSizesW()));
	disconnect(spinHeight, SIGNAL(valueChanged(double)), this, SLOT(changeSizesH()));
	if (linkSize->isChecked())
		spinHeight->setValue(spinWidth->value());
	connect(spinWidth, SIGNAL(valueChanged(double)), this, SLOT(changeSizesW()));
	connect(spinHeight, SIGNAL(valueChanged(double)), this, SLOT(changeSizesH()));
}
