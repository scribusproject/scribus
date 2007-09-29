/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "polygonwidget.h"
#include "scconfig.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QPolygon>
#include <QRect>
#include <QMatrix>
#include <QToolTip>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>

#if _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "util_math.h"
#include "prefsstructs.h"


using namespace std;

PolygonWidget::PolygonWidget(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR) : QWidget( parent )
{
	Pre = new QPixmap(101, 101);
	Pre->fill(Qt::white);
	PFactor = polyF;
	PolygonPropsLayout = new QVBoxLayout( this );
	PolygonPropsLayout->setMargin(10);
	PolygonPropsLayout->setSpacing(5);
	Layout11 = new QHBoxLayout;
	Layout11->setMargin(0);
	Layout11->setSpacing(5);
	Layout10 = new QVBoxLayout;
	Layout10->setMargin(0);
	Layout10->setSpacing(5);
	Layout2 = new QHBoxLayout;
	Layout2->setMargin(0);
	Layout2->setSpacing(5);
	Ecken = new QSpinBox( this );
	Ecken->setMaximum( 999 );
	Ecken->setMinimum( 3 );
	Ecken->setValue(polyC);
	Text1 = new QLabel( tr("Corn&ers:"), this );
	Text1->setBuddy(Ecken);
	Layout2->addWidget( Text1 );
	Layout2->addWidget( Ecken );
	Layout10->addLayout( Layout2 );

	Layout9_2 = new QHBoxLayout;
	Layout9_2->setMargin(0);
	Layout9_2->setSpacing(5);
	Layout8_2 = new QVBoxLayout;
	Layout8_2->setMargin(0);
	Layout8_2->setSpacing(5);
	Layout7_2 = new QHBoxLayout;
	Layout7_2->setMargin(0);
	Layout7_2->setSpacing(5);
	Faktor2 = new QSpinBox( this );
	Faktor2->setSuffix(" ");
	Faktor2->setMaximum( 180 );
	Faktor2->setMinimum( -180 );
	Faktor2->setValue(static_cast<int>(polyR));
	Text2_2 = new QLabel( tr("&Rotation:"), this );
	Text2_2->setBuddy(Faktor2);
	Layout7_2->addWidget( Text2_2 );
	Layout7_2->addWidget( Faktor2 );
	Layout8_2->addLayout( Layout7_2 );
	Slider2 = new QSlider( this );
	Slider2->setMinimum( -180 );
	Slider2->setMaximum( 180 );
	Slider2->setValue(static_cast<int>(polyR));
	Slider2->setOrientation( Qt::Horizontal );
	Slider2->setTickPosition( QSlider::TicksRight );
	Layout8_2->addWidget( Slider2 );
	Layout9_2->addLayout( Layout8_2 );
	Layout10->addLayout( Layout9_2 );

	Konvex = new QCheckBox( this );
	Konvex->setText( tr( "Apply &Factor" ) );
	Konvex->setChecked(polyS);
	Layout10->addWidget( Konvex );
	Layout9 = new QHBoxLayout;
	Layout9->setMargin(0);
	Layout9->setSpacing(5);
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout9->addItem( spacer );
	Layout8 = new QVBoxLayout;
	Layout8->setMargin(0);
	Layout8->setSpacing(5);
	Layout7 = new QHBoxLayout;
	Layout7->setMargin(0);
	Layout7->setSpacing(5);
	Faktor = new QSpinBox( this );
	Faktor->setSuffix( tr( " %" ) );
	Faktor->setMaximum( 100 );
	Faktor->setMinimum( -100 );
	Faktor->setValue(polyFd);
	Text2 = new QLabel( tr("&Factor:"), this );
	Text2->setBuddy(Faktor);
	Layout7->addWidget( Text2 );
	Layout7->addWidget( Faktor );
	Layout8->addLayout( Layout7 );
	Slider1 = new QSlider( this );
	Slider1->setMinimum( -100 );
	Slider1->setMaximum( 100 );
	Slider1->setOrientation( Qt::Horizontal );
	Slider1->setTickPosition(QSlider::TicksRight);
	Slider1->setValue(polyFd);
	if (polyFd == 0)
		Konvex->setChecked(false);
	Layout8->addWidget( Slider1 );
	Layout9->addLayout( Layout8 );
	Layout10->addLayout( Layout9 );
	Layout11->addLayout( Layout10 );
	Preview = new QLabel( this );
	Preview->setMinimumSize( QSize( 106, 106 ) );
	Preview->setMaximumSize( QSize( 106, 106 ) );
	Preview->setFrameShape( QLabel::Panel );
	Preview->setFrameShadow( QLabel::Sunken );
	Preview->setLineWidth(2);
	Preview->setAlignment(Qt::AlignCenter);
	Preview->setPixmap(*Pre);
	Layout11->addWidget( Preview );
	PolygonPropsLayout->addLayout( Layout11 );

	UpdatePreView();
	//tooltips
	Ecken->setToolTip( "<qt>" + tr( "Number of corners for polygons" ) + "</qt>" );
	Faktor2->setToolTip( "<qt>" + tr( "Degrees of rotation for polygons" ) + "</qt>" );
	Slider2->setToolTip( "<qt>" + tr( "Degrees of rotation for polygons" ) + "</qt>" );
	Konvex->setToolTip( "<qt>" + tr( "Apply Convex/Concave Factor to change shape of Polygons" ) + "</qt>" );
	Preview->setToolTip( tr( "Sample Polygon" ) );
	Faktor->setToolTip( "<qt>" + tr( "A negative value will make the polygon concave (or star shaped), a positive value will make it convex" ) + "</qt>" );
	Slider1->setToolTip( tr( "A negative value will make the polygon concave (or star shaped), a positive value will make it convex" ) + "</qt>");
	// signals and slots connections
	connect(Faktor, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin(int)));
	connect(Faktor2, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin2(int)));
	connect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
	connect(Slider1, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
	connect(Slider2, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Ecken, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Konvex, SIGNAL(clicked()), this, SLOT(UpdatePreView()));
}

void PolygonWidget::restoreDefaults(struct toolPrefs *prefsData)
{
	PFactor = prefsData->polyS;
	Ecken->setValue(prefsData->polyC);	
	Faktor2->setValue(static_cast<int>(prefsData->polyR));
	Slider2->setValue(static_cast<int>(prefsData->polyR));
	Konvex->setChecked(prefsData->polyS);
	Faktor->setValue(prefsData->polyFd);
	Slider1->setValue(prefsData->polyFd);
	if (prefsData->polyFd == 0)
		Konvex->setChecked(false);
	UpdatePreView();
}

void PolygonWidget::getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR)
{
	*polyC = Ecken->value();
	*polyF = PFactor;
	*polyS = Konvex->isChecked();
	*polyFd = Slider1->value();
	*polyR = Faktor2->value();
}

void PolygonWidget::ValFromSpin2(int a)
{
	disconnect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
	Slider2->setValue(a);
	connect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
}

void PolygonWidget::ValFromSpin(int a)
{
	disconnect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
	Slider1->setValue(a);
	connect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
}

void PolygonWidget::UpdatePreView()
{
	if (Konvex->isChecked())
	{
		Slider1->setEnabled(true);
		Faktor->setEnabled(true);
	}
	else
	{
		Slider1->setEnabled(false);
		Faktor->setEnabled(false);
	}
	Pre->fill(Qt::white);
	QPainter p;
	p.begin(Pre);
	p.setBrush(Qt::NoBrush);
	p.setPen(Qt::black);
	QPolygon pp = RegularPolygon(100, 100, Ecken->value(), Konvex->isChecked(), GetFaktor(), Slider2->value());
	QRect br = pp.boundingRect();
	if (br.x() < 0)
		pp.translate(-br.x(), 0);
	if (br.y() < 0)
		pp.translate(0, -br.y());
	br = pp.boundingRect();
	if ((br.height() > 100) || (br.width() > 100))
	{
		QMatrix ma;
		double sca = 100.0 / static_cast<double>(qMax(br.width(), br.height()));
		ma.scale(sca, sca);
		pp = pp * ma;
	}
	p.drawPolygon(pp);
	p.end();
	Preview->setPixmap(*Pre);
}

double PolygonWidget::GetZeroFaktor()
{
	return sqrt(pow(1.0,2.0)-pow(((sin((360.0/(Ecken->value()*2))/180* M_PI)* 2.0)/2.0),2.0));
}

double PolygonWidget::GetMaxFaktor()
{
	double win = (360.0/(Ecken->value()*2)) / 180.0 * M_PI;
	double ret;
	if ((360.0/(Ecken->value()*2)) > 45)
		ret = 1/sin(win);
	else
		ret = 1/cos(win);
	return ret;
}

double PolygonWidget::GetFaktor()
{
	int val = Slider1->value();
	if (val < 0)
		PFactor = GetZeroFaktor() * (100.0 + val) / 100.0;
	else
	{
		double ma = GetMaxFaktor();
		double mi = GetZeroFaktor();
		PFactor = ((ma - mi) * val / 100.0) + mi;
	}
	return PFactor;
}
