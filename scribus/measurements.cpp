/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "measurements.h"

#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QString>

#include <cmath>
#include "units.h"

using namespace std;


Measurements::Measurements( QWidget* parent ) : ScrPaletteBase( parent, "MeasurementsPalette", false, 0 )
{
	measurementsLayout = new QGridLayout( this );
	measurementsLayout->setMargin(10);
	measurementsLayout->setSpacing(5);
	x1Label = new QLabel( this );
	y1Label = new QLabel( this );
	x2Label = new QLabel( this );
	y2Label = new QLabel( this );
	dxLabel = new QLabel( this );
	dyLabel = new QLabel( this );
	angleLabel = new QLabel( this );
	lengthLabel = new QLabel( this );
	measurementsLayout->addWidget( x1Label, 0, 0 );
	measurementsLayout->addWidget( y1Label, 1, 0 );
	measurementsLayout->addWidget( x2Label, 2, 0 );
	measurementsLayout->addWidget( y2Label, 3, 0 );
	measurementsLayout->addWidget( dxLabel, 4, 0 );
	measurementsLayout->addWidget( dyLabel, 5, 0 );
	measurementsLayout->addWidget( angleLabel, 6, 0 );
	measurementsLayout->addWidget( lengthLabel, 7, 0 );

	x1Data = new QLabel( "", this );
	y1Data = new QLabel( "", this );
	x2Data = new QLabel( "", this );
	y2Data = new QLabel( "", this );
	dXData = new QLabel( "", this );
	dYData = new QLabel( "", this );
	angleData = new QLabel( "", this );
	lengthData = new QLabel( "", this );
	
	const QString widthString="10000.00 " + unitGetStrFromIndex(0);
	int textWidth = fontMetrics().width(widthString);
	x1Data->setMinimumSize(textWidth, 12);
	y1Data->setMinimumSize(textWidth, 12);
	x2Data->setMinimumSize(textWidth, 12);
	y2Data->setMinimumSize(textWidth, 12);
	dXData->setMinimumSize(textWidth, 12);
	dYData->setMinimumSize(textWidth, 12);
	angleData->setMinimumSize(textWidth, 12);
	lengthData->setMinimumSize(textWidth, 12);

	Qt::Alignment labelAlignment=Qt::AlignVCenter | Qt::AlignRight;
	x1Data->setAlignment( labelAlignment );
	y1Data->setAlignment( labelAlignment );
	x2Data->setAlignment( labelAlignment );
	y2Data->setAlignment( labelAlignment );
	dXData->setAlignment( labelAlignment );
	dYData->setAlignment( labelAlignment );
	angleData->setAlignment( labelAlignment );
	lengthData->setAlignment( labelAlignment );

	measurementsLayout->addWidget( x1Data, 0, 1 );
	measurementsLayout->addWidget( y1Data, 1, 1 );
	measurementsLayout->addWidget( x2Data, 2, 1 );
	measurementsLayout->addWidget( y2Data, 3, 1 );
	measurementsLayout->addWidget( dXData, 4, 1 );
	measurementsLayout->addWidget( dYData, 5, 1 );
	measurementsLayout->addWidget( angleData, 6, 1 );
	measurementsLayout->addWidget( lengthData, 7, 1 );

	languageChange();

}
/*
void Measurements::closeEvent(QCloseEvent *ce)
{
	emit Schliessen(false);
	ce->accept();
}
*/
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

	double uXm =  unitGetRatioFromIndex(unitIndex) * multiplier;

	x1Data->setText(tmp.setNum(qRound(x1*uXm) / divisor, 'f', precision)+unitSuffix);
	y1Data->setText(tmp.setNum(qRound(y1*uXm) / divisor, 'f', precision)+unitSuffix);
	x2Data->setText(tmp.setNum(qRound(x2*uXm) / divisor, 'f', precision)+unitSuffix);
	y2Data->setText(tmp.setNum(qRound(y2*uXm) / divisor, 'f', precision)+unitSuffix);
	dXData->setText(tmp.setNum(qRound((x2-x1)*uXm) / divisor, 'f', precision)+unitSuffix);
	dYData->setText(tmp.setNum(qRound((y2-y1)*uXm) / divisor, 'f', precision)+unitSuffix);
	angleData->setText(tmp.setNum(fabs(rr), 'f', precision)+ trUtf8(" °"));
	lengthData->setText(tmp.setNum(qRound(len*uXm) / divisor, 'f', precision)+unitSuffix);
}

void Measurements::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
}

void Measurements::languageChange()
{
	setWindowTitle( tr( "Distances" ) );
	
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

