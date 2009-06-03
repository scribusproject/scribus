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
	QSpacerItem* spacerL = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerL, 1, 0 );
	RotationGroup = new BasePointWidget(RotationGroupContainer, mode);
	Layout12a->addWidget(RotationGroup, 1, 1);
	QSpacerItem* spacerR = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerR, 1, 2 );
	QSpacerItem* spacerB = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout12a->addItem( spacerB, 2, 1 );
	editLayout->addWidget( RotationGroupContainer, 0, 0 );
	RotationGroup->setCheckedId(origin);
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
	spinHeight = new ScrSpinBox( 0, 1000, SizeGroup, (mode == 0) ? unitIndex : 6);
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
