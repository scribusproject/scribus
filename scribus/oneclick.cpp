/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "oneclick.h"
#include "oneclick.moc"
extern QPixmap loadIcon(QString nam);

#include <qtooltip.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include "mspinbox.h"
#include "linkbutton.h"
#include "units.h"
#include "commonstrings.h"

OneClick::OneClick( QWidget* parent, QString titel, int unitIndex, double defW, double defH, bool remember ) : QDialog( parent, "OneClick", true, 0 )
{
	double m_unitRatio = unitGetRatioFromIndex(unitIndex);
	QString m_suffix = unitGetSuffixFromIndex(unitIndex);
	int decimals = unitGetDecimalsFromIndex(unitIndex);
	setCaption( titel );
	setIcon(loadIcon("AppIcon.png"));
	queryLayout = new QVBoxLayout( this, 10, 5 );
	editLayout = new QGridLayout;
	editLayout->setSpacing( 5 );
	editLayout->setMargin( 0 );
	questionLabel = new QLabel( tr("Width:"), this, "questionLabel" );
	editLayout->addWidget( questionLabel, 0, 0 );
	spinWidth = new MSpinBox( 0, 1000, this, decimals );
	spinWidth->setSuffix( m_suffix );
	spinWidth->setMinValue(0.1*m_unitRatio);
	spinWidth->setMaxValue(30000.0*m_unitRatio);
	spinWidth->setValue(defW * m_unitRatio);
	editLayout->addWidget( spinWidth, 0, 1 );
	questionLabel2 = new QLabel( tr("Height:"), this, "questionLabel2" );
	editLayout->addWidget( questionLabel2, 1, 0 );
	spinHeight = new MSpinBox( 0, 1000, this, decimals );
	spinHeight->setSuffix( m_suffix );
	spinHeight->setMinValue(0.1*m_unitRatio);
	spinHeight->setMaxValue(30000.0*m_unitRatio);
	spinHeight->setValue(defH * m_unitRatio);
	editLayout->addWidget( spinHeight, 1, 1 );
	linkSize = new LinkButton( this );
	linkSize->setToggleButton( true );
	linkSize->setAutoRaise( true );
	linkSize->setMaximumSize( QSize( 15, 32767 ) );
	editLayout->addMultiCellWidget( linkSize, 0, 1, 2, 2 );
	checkRemember = new QCheckBox(this, "checkRemember");
	checkRemember->setText( tr("Remember Values"));
	checkRemember->setChecked(remember);
	editLayout->addMultiCellWidget( checkRemember, 3, 3, 0, 2 );
	queryLayout->addLayout( editLayout );
	okCancelLayout = new QHBoxLayout;
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
	connect(linkSize, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(changeSizesW()));
	connect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(changeSizesH()));
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
