/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "oneclick.h"
#include "oneclick.moc"
//Added by qt3to4:
#include <QPixmap>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
extern QPixmap loadIcon(QString nam);

#include <qtooltip.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <q3buttongroup.h>
#include <q3groupbox.h>
#include <qradiobutton.h>
#include <q3frame.h>
#include "mspinbox.h"
#include "linkbutton.h"
#include "units.h"
#include "commonstrings.h"

OneClick::OneClick( QWidget* parent, QString titel, int unitIndex, double defW, double defH, bool remember, int origin, int mode ) : QDialog( parent, "OneClick", true, 0 )
{
	double m_unitRatio = unitGetRatioFromIndex(unitIndex);
	QString m_suffix = unitGetSuffixFromIndex(unitIndex);
	int decimals = unitGetDecimalsFromIndex(unitIndex);
	setCaption( titel );
	setIcon(loadIcon("AppIcon.png"));
	queryLayout = new Q3VBoxLayout( this, 10, 5 );
	editLayout = new Q3GridLayout;
	editLayout->setSpacing( 5 );
	editLayout->setMargin( 0 );
	RotationGroup = new Q3ButtonGroup( tr("Origin"), this, "RotationGroup" );
	RotationGroup->setColumnLayout(0, Qt::Vertical );
	RotationGroup->layout()->setSpacing( 0 );
	RotationGroup->layout()->setMargin( 10 );
	Layout12a = new Q3GridLayout( RotationGroup->layout() );
	Layout12a->setAlignment( Qt::AlignTop );
	QSpacerItem* spacerT = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout12a->addItem( spacerT, 0, 1 );
	QSpacerItem* spacerL = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerL, 1, 0 );
	Layout12 = new Q3GridLayout;
	Layout12->setSpacing( 0 );
	Layout12->setMargin( 0 );
	TopLeft = new QRadioButton( RotationGroup, "TopLeft" );
	TopLeft->setText( "" );
	TopLeft->setChecked( true );
	Layout12->addWidget( TopLeft, 0, 0, Qt::AlignCenter );
	if (mode == 0)
	{
		Line1 = new Q3Frame( RotationGroup, "Line1" );
		Line1->setMinimumSize( QSize( 20, 4 ) );
		Line1->setMaximumSize( QSize( 20, 4 ) );
		Line1->setFrameShape( Q3Frame::HLine );
		Line1->setFrameShadow( Q3Frame::Plain );
		Line1->setLineWidth( 3 );
		Layout12->addWidget( Line1, 0, 1, Qt::AlignCenter );
		TopRight = new QRadioButton( RotationGroup, "TopRight" );
		TopRight->setText( "" );
		Layout12->addWidget( TopRight, 0, 2, Qt::AlignCenter );
		Line2 = new Q3Frame( RotationGroup, "Line2" );
		Line2->setMinimumSize( QSize( 4, 20 ) );
		Line2->setMaximumSize( QSize( 4, 20 ) );
		Line2->setFrameShape( Q3Frame::VLine );
		Line2->setFrameShadow( Q3Frame::Plain );
		Line2->setLineWidth( 3 );
		Layout12->addWidget( Line2, 1, 0, Qt::AlignCenter );
		Center = new QRadioButton( RotationGroup, "Center" );
		Center->setText( "" );
		Layout12->addWidget( Center, 1, 1, Qt::AlignCenter );
		Line4 = new Q3Frame( RotationGroup, "Line4" );
		Line4->setMinimumSize( QSize( 4, 20 ) );
		Line4->setMaximumSize( QSize( 4, 20 ) );
		Line4->setFrameShadow( Q3Frame::Plain );
		Line4->setLineWidth( 3 );
		Line4->setFrameShape( Q3Frame::VLine );
		Layout12->addWidget( Line4, 1, 2, Qt::AlignCenter );
		BottomLeft = new QRadioButton( RotationGroup, "BottomLeft" );
		BottomLeft->setText( "" );
		Layout12->addWidget( BottomLeft, 2, 0, Qt::AlignCenter );
		Line5 = new Q3Frame( RotationGroup, "Line5" );
		Line5->setMinimumSize( QSize( 20, 4 ) );
		Line5->setMaximumSize( QSize( 20, 4 ) );
		Line5->setFrameShape( Q3Frame::HLine );
		Line5->setFrameShadow( Q3Frame::Plain );
		Line5->setLineWidth( 3 );
		Layout12->addWidget( Line5, 2, 1, Qt::AlignCenter );
		BottomRight = new QRadioButton( RotationGroup, "BottomRight" );
		BottomRight->setText( "" );
		Layout12->addWidget( BottomRight, 2, 2, Qt::AlignCenter );
	}
	else
	{
		Line1 = new Q3Frame( RotationGroup, "Line1" );
		Line1->setMinimumSize( QSize( 10, 4 ) );
		Line1->setMaximumSize( QSize( 10, 4 ) );
		Line1->setFrameShape( Q3Frame::HLine );
		Line1->setFrameShadow( Q3Frame::Plain );
		Line1->setLineWidth( 3 );
		Layout12->addWidget( Line1, 0, 1, Qt::AlignCenter );
		Center = new QRadioButton( RotationGroup, "Center" );
		Center->setText( "" );
		Layout12->addWidget( Center, 0, 2, Qt::AlignCenter );
		Line2 = new Q3Frame( RotationGroup, "Line1" );
		Line2->setMinimumSize( QSize( 10, 4 ) );
		Line2->setMaximumSize( QSize( 10, 4 ) );
		Line2->setFrameShape( Q3Frame::HLine );
		Line2->setFrameShadow( Q3Frame::Plain );
		Line2->setLineWidth( 3 );
		Layout12->addWidget( Line2, 0, 3, Qt::AlignCenter );
		TopRight = new QRadioButton( RotationGroup, "TopRight" );
		TopRight->setText( "" );
		Layout12->addWidget( TopRight, 0, 4, Qt::AlignCenter );
	}
	Layout12a->addLayout(Layout12, 1, 1);
	QSpacerItem* spacerR = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerR, 1, 2 );
	QSpacerItem* spacerB = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout12a->addItem( spacerB, 2, 1 );
	editLayout->addWidget( RotationGroup, 0, 0 );
	RotationGroup->setButton(origin);
	SizeGroup = new Q3GroupBox( tr("Size"), this, "SizeGroup" );
	SizeGroup->setColumnLayout(0, Qt::Vertical );
	SizeGroup->layout()->setSpacing( 5 );
	SizeGroup->layout()->setMargin( 10 );
	SizeGroupLayout = new Q3GridLayout( SizeGroup->layout() );
	SizeGroupLayout->setAlignment( Qt::AlignTop );
	questionLabel = new QLabel( tr("Width:"), SizeGroup, "questionLabel" );
	if (mode == 1)
		questionLabel->setText( tr("Length:"));
	SizeGroupLayout->addWidget( questionLabel, 0, 0 );
	spinWidth = new MSpinBox( 0, 1000, SizeGroup, decimals );
	spinWidth->setSuffix( m_suffix );
	spinWidth->setMinValue(0.1*m_unitRatio);
	spinWidth->setMaxValue(30000.0*m_unitRatio);
	spinWidth->setValue(defW * m_unitRatio);
	SizeGroupLayout->addWidget( spinWidth, 0, 1 );
	questionLabel2 = new QLabel( tr("Height:"), SizeGroup, "questionLabel2" );
	if (mode == 1)
		questionLabel2->setText( tr("Angle:"));
	SizeGroupLayout->addWidget( questionLabel2, 1, 0 );
	spinHeight = new MSpinBox( 0, 1000, SizeGroup, decimals );
	if (mode == 0)
	{
		spinHeight->setSuffix( m_suffix );
		spinHeight->setMinValue(0.1*m_unitRatio);
		spinHeight->setMaxValue(30000.0*m_unitRatio);
		spinHeight->setValue(defH * m_unitRatio);
	}
	else
	{
		spinHeight->setWrapping( true );
		spinHeight->setValues( 0, 359.99, 100, 0);
		spinHeight->setValue(defH * m_unitRatio);
	}
	SizeGroupLayout->addWidget( spinHeight, 1, 1 );
	if (mode == 0)
	{
		linkSize = new LinkButton( SizeGroup );
		linkSize->setToggleButton( true );
		linkSize->setAutoRaise( true );
		linkSize->setMaximumSize( QSize( 15, 32767 ) );
		SizeGroupLayout->addMultiCellWidget( linkSize, 0, 1, 2, 2 );
	}
	editLayout->addWidget( SizeGroup, 0, 1 );

	checkRemember = new QCheckBox(this, "checkRemember");
	checkRemember->setText( tr("Remember Values"));
	checkRemember->setChecked(remember);
	editLayout->addMultiCellWidget( checkRemember, 1, 1, 0, 1 );

	queryLayout->addLayout( editLayout );
	okCancelLayout = new Q3HBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "cancelButton" );
	okCancelLayout->addWidget( cancelButton );
	queryLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	if (mode == 0)
	{
		connect(linkSize, SIGNAL(clicked()), this, SLOT(ToggleKette()));
		connect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
		connect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
	}
}

void OneClick::ToggleKette()
{
	disconnect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
	disconnect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
	if (linkSize->isOn())
		spinHeight->setValue(spinWidth->value());
	connect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
	connect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
}

void OneClick::changeSizesH()
{
	disconnect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
	disconnect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
	if (linkSize->isOn())
		spinWidth->setValue(spinHeight->value());
	connect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
	connect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
}

void OneClick::changeSizesW()
{
	disconnect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
	disconnect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
	if (linkSize->isOn())
		spinHeight->setValue(spinWidth->value());
	connect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
	connect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
}
