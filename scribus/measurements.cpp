#include "measurements.h"
#include "measurements.moc"

#include <qvariant.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <cmath>
#include "units.h"

using namespace std;

extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

Measurements::Measurements( QWidget* parent ) : QDialog( parent, "M", false, 0 )
{
	const QString widthString="10000.00 " + tr( "pt" );
	setCaption( tr( "Distances" ) );
	setIcon( loadIcon("AppIcon.png") );

	measurementsLayout = new QGridLayout( this, 1, 1, 10, 6, "measurementsLayout");
	x1Label = new QLabel( this, "x1Label" );
	y1Label = new QLabel( this, "y1Label" );
	x2Label = new QLabel( this, "x2Label" );
	y2Label = new QLabel( this, "y2Label" );
	dxLabel = new QLabel( this, "dxLabel" );
	dyLabel = new QLabel( this, "dyLabel" );
	angleLabel = new QLabel( this, "dxLabel" );
	lengthLabel = new QLabel( this, "dyLabel" );
	measurementsLayout->addWidget( x1Label, 0, 0 );
	measurementsLayout->addWidget( y1Label, 1, 0 );
	measurementsLayout->addWidget( x2Label, 2, 0 );
	measurementsLayout->addWidget( y2Label, 3, 0 );
	measurementsLayout->addWidget( dxLabel, 4, 0 );
	measurementsLayout->addWidget( dyLabel, 5, 0 );
	measurementsLayout->addWidget( angleLabel, 6, 0 );
	measurementsLayout->addWidget( lengthLabel, 7, 0 );

	x1Data = new QLabel( "", this, "x1Data" );
	y1Data = new QLabel( "", this, "y1Data" );
	x2Data = new QLabel( "", this, "x2Data" );
	y2Data = new QLabel( "", this, "y2Data" );
	dXData = new QLabel( "", this, "dxData" );
	dYData = new QLabel( "", this, "dyData" );
	angleData = new QLabel( "", this, "angleData" );
	lengthData = new QLabel( "", this, "lengthData" );

	x1Data->setMinimumSize(fontMetrics().width(widthString), 12);
	y1Data->setMinimumSize(fontMetrics().width(widthString), 12);
	x2Data->setMinimumSize(fontMetrics().width(widthString), 12);
	y2Data->setMinimumSize(fontMetrics().width(widthString), 12);
	dXData->setMinimumSize(fontMetrics().width(widthString), 12);
	dYData->setMinimumSize(fontMetrics().width(widthString), 12);
	angleData->setMinimumSize(fontMetrics().width(widthString), 12);
	lengthData->setMinimumSize(fontMetrics().width(widthString), 12);

	x1Data->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	y1Data->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	x2Data->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	y2Data->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	dXData->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	dYData->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	angleData->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	lengthData->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

	measurementsLayout->addWidget( x1Data, 0, 1 );
	measurementsLayout->addWidget( y1Data, 1, 1 );
	measurementsLayout->addWidget( x2Data, 2, 1 );
	measurementsLayout->addWidget( y2Data, 3, 1 );
	measurementsLayout->addWidget( dXData, 4, 1 );
	measurementsLayout->addWidget( dYData, 5, 1 );
	measurementsLayout->addWidget( angleData, 6, 1 );
	measurementsLayout->addWidget( lengthData, 7, 1 );

	changeLanguage();

}

void Measurements::closeEvent(QCloseEvent *ce)
{
	emit Schliessen(false);
	ce->accept();
}

void Measurements::setValues(double x1, double y1, double x2, double y2, double angle, double len, int unitIndex)
{
	QString tmp;
	QString unitSuffix = unitGetSuffixFromIndex(unitIndex);
	int multiplier = unitGetDecimalsFromIndex(unitIndex);
	double divisor = static_cast<double>(multiplier);
	int precision = unitGetPrecisionFromIndex(unitIndex);

	double rr = angle;
	if (angle < 0)
		rr = rr + 360;

	double uXm = UmReFaktor * multiplier;

	x1Data->setText(tmp.setNum(qRound(x1*uXm) / divisor, 'f', precision)+unitSuffix);
	y1Data->setText(tmp.setNum(qRound(y1*uXm) / divisor, 'f', precision)+unitSuffix);
	x2Data->setText(tmp.setNum(qRound(x2*uXm) / divisor, 'f', precision)+unitSuffix);
	y2Data->setText(tmp.setNum(qRound(y2*uXm) / divisor, 'f', precision)+unitSuffix);
	dXData->setText(tmp.setNum(qRound((x2-x1)*uXm) / divisor, 'f', precision)+unitSuffix);
	dYData->setText(tmp.setNum(qRound((y2-y1)*uXm) / divisor, 'f', precision)+unitSuffix);
	angleData->setText(tmp.setNum(fabs(rr), 'f', precision)+tr(QString::fromUtf8(" °")));
	lengthData->setText(tmp.setNum(qRound(len*uXm) / divisor, 'f', precision)+unitSuffix);
}

void Measurements::changeLanguage()
{
	x1Label->setText( tr( "X1:" ) );
	y1Label->setText( tr( "Y1:" ) );
	x2Label->setText( tr( "X2:" ) );
	y2Label->setText( tr( "Y2:" ) );
	dxLabel->setText( tr( "DX:" ) );
	dyLabel->setText( tr( "DY:" ) );
	angleLabel->setText( tr( "Angle:" ) );
	lengthLabel->setText( tr( "Length:" ) );

	resize(minimumSizeHint());
}

