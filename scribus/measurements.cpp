/****************************************************************************
** Form implementation generated from reading ui file 'measurements.ui'
**
** Created: Fre Mai 14 21:58:51 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "measurements.h"
#include "measurements.moc"

#include <qvariant.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <cmath>

extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

Measurements::Measurements( QWidget* parent ) : QDialog( parent, "M", false, 0 )
{
	setCaption( tr( "Distances" ) );
	setIcon( loadIcon("AppIcon.png") );
	MeasurementsLayout = new QGridLayout( this, 1, 1, 10, 6, "MeasurementsLayout");
	Label1 = new QLabel( this, "Label1" );
	Label1->setText( tr( "X1:" ) );
	MeasurementsLayout->addWidget( Label1, 0, 0 );
	Label2 = new QLabel( this, "Label2" );
	Label2->setText( tr( "Y1:" ) );
	MeasurementsLayout->addWidget( Label2, 1, 0 );
	Label3 = new QLabel( this, "Label3" );
	Label3->setText( tr( "X2:" ) );
	MeasurementsLayout->addWidget( Label3, 2, 0 );
	Label4 = new QLabel( this, "Label4" );
	Label4->setText( tr( "Y2:" ) );
	MeasurementsLayout->addWidget( Label4, 3, 0 );
	Label5 = new QLabel( this, "Label5" );
	Label5->setText( tr( "DX:" ) );
	MeasurementsLayout->addWidget( Label5, 4, 0 );
	Label6 = new QLabel( this, "Label6" );
	Label6->setText( tr( "DY:" ) );
	MeasurementsLayout->addWidget( Label6, 5, 0 );
	Label7 = new QLabel( this, "Label5" );
	Label7->setText( tr( "Angle:" ) );
	MeasurementsLayout->addWidget( Label7, 6, 0 );
	Label8 = new QLabel( this, "Label6" );
	Label8->setText( tr( "Length:" ) );
	MeasurementsLayout->addWidget( Label8, 7, 0 );
	X1 = new QLabel( this, "X1" );
	X1->setText("");
	X1->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	X1->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( X1, 0, 1 );
	Y1 = new QLabel( this, "Y1" );
	Y1->setText("");
	Y1->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	Y1->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( Y1, 1, 1 );
	X2 = new QLabel( this, "X2" );
	X2->setText("");
	X2->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	X2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( X2, 2, 1 );
	Y2 = new QLabel( this, "Y2" );
	Y2->setText("");
	Y2->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	Y2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( Y2, 3, 1 );
	DX = new QLabel( this, "X2" );
	DX->setText("");
	DX->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	DX->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( DX, 4, 1 );
	DY = new QLabel( this, "Y2" );
	DY->setText("");
	DY->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	DY->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( DY, 5, 1 );
	Angle = new QLabel( this, "Angle" );
	Angle->setText("");
	Angle->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	Angle->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( Angle, 6, 1 );
	Length = new QLabel( this, "Length" );
	Length->setText("");
	Length->setMinimumSize(fontMetrics().width("10000.00 pt"), 12);
	Length->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
	MeasurementsLayout->addWidget( Length, 7, 1 );
	resize(minimumSizeHint());
}

void Measurements::setValues(double x1, double y1, double x2, double y2, double angle, double len, int unit)
{
	QString tmp, tmp2;
	int multiplier, precision;
	double divisor;
	switch (unit)
	{
	case 0:
		tmp2 = tr(" pt");
		multiplier = 100;
		divisor = 100.0;
		precision = 2;
		break;
	case 1:
		tmp2 = tr(" mm");
		multiplier = 1000;
		divisor = 1000.0;
		precision = 3;
		break;
	case 2:
		tmp2 = tr(" in");
		multiplier = 10000;
		divisor = 10000.0;
		precision = 4;
		break;
	case 3:
		tmp2 = tr(" p");
		multiplier = 100;
		divisor = 100.0;
		precision = 2;
		break;
	default:  // jjsa 21-03-2004 added default (complains for lint)
		tmp2 = tr(" pt");
		multiplier = 100;
		divisor = 100.0;
		precision = 2;
		break;
	}
	double rr = angle;
	if (angle < 0)
		rr = rr + 360;
	X1->setText(tmp.setNum(qRound(x1*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
	Y1->setText(tmp.setNum(qRound(y1*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
	X2->setText(tmp.setNum(qRound(x2*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
	Y2->setText(tmp.setNum(qRound(y2*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
	DX->setText(tmp.setNum(qRound((x2-x1)*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
	DY->setText(tmp.setNum(qRound((y2-y1)*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
	Angle->setText(tmp.setNum(qRound(fabs(rr)*UmReFaktor * multiplier) / divisor, 'f', precision)+tr(QString::fromUtf8(" °")));
	Length->setText(tmp.setNum(qRound(len*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
}
