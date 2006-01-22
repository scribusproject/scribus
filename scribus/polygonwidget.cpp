/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "polygonwidget.h"
#include "polygonwidget.moc"
#include "scconfig.h"
#include <qpointarray.h>
#include <qpainter.h>
#include <qrect.h>
#include <qwmatrix.h>
#include <cmath>
#include "util.h"

using namespace std;

PolygonWidget::PolygonWidget(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR) : QWidget( parent )
{
	Pre = new QPixmap(101, 101);
	Pre->fill(white);
	PFactor = polyF;
	PolygonPropsLayout = new QVBoxLayout( this, 10, 5, "PolygonPropsLayout");
	Layout11 = new QHBoxLayout( 0, 0, 5, "Layout11");
	Layout10 = new QVBoxLayout( 0, 0, 5, "Layout10");
	Layout2 = new QHBoxLayout( 0, 0, 5, "Layout2");
	Ecken = new QSpinBox( this, "Ecken" );
	Ecken->setMaxValue( 999 );
	Ecken->setMinValue( 3 );
	Ecken->setValue(polyC);
	Text1 = new QLabel( Ecken, tr("Corn&ers:"), this, "Text1" );
	Layout2->addWidget( Text1 );
	Layout2->addWidget( Ecken );
	Layout10->addLayout( Layout2 );

	Layout9_2 = new QHBoxLayout( 0, 0, 5, "Layout9_2");
	Layout8_2 = new QVBoxLayout( 0, 0, 5, "Layout8_2");
	Layout7_2 = new QHBoxLayout( 0, 0, 5, "Layout7_2");
	Faktor2 = new QSpinBox( this, "Faktor_2" );
	Faktor2->setSuffix(" ");
	Faktor2->setMaxValue( 180 );
	Faktor2->setMinValue( -180 );
	Faktor2->setValue(static_cast<int>(polyR));
	Text2_2 = new QLabel( Faktor2, tr("&Rotation:"), this, "Text2_2" );
	Layout7_2->addWidget( Text2_2 );
	Layout7_2->addWidget( Faktor2 );
	Layout8_2->addLayout( Layout7_2 );
	Slider2 = new QSlider( this, "Slider1_2" );
	Slider2->setMinValue( -180 );
	Slider2->setMaxValue( 180 );
	Slider2->setValue(static_cast<int>(polyR));
	Slider2->setOrientation( QSlider::Horizontal );
	Slider2->setTickmarks( QSlider::Right );
	Layout8_2->addWidget( Slider2 );
	Layout9_2->addLayout( Layout8_2 );
	Layout10->addLayout( Layout9_2 );

	Konvex = new QCheckBox( this, "Konvex" );
	Konvex->setText( tr( "Apply &Factor" ) );
	Konvex->setChecked(polyS);
	Layout10->addWidget( Konvex );
	Layout9 = new QHBoxLayout( 0, 0, 5, "Layout9");
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout9->addItem( spacer );
	Layout8 = new QVBoxLayout( 0, 0, 5, "Layout8");
	Layout7 = new QHBoxLayout( 0, 0, 5, "Layout7");
	Faktor = new QSpinBox( this, "Faktor" );
	Faktor->setSuffix( tr( " %" ) );
	Faktor->setMaxValue( 100 );
	Faktor->setMinValue( -100 );
	Faktor->setValue(polyFd);
	Text2 = new QLabel( Faktor, tr("&Factor:"), this, "Text2" );
	Layout7->addWidget( Text2 );
	Layout7->addWidget( Faktor );
	Layout8->addLayout( Layout7 );
	Slider1 = new QSlider( this, "Slider1" );
	Slider1->setMinValue( -100 );
	Slider1->setMaxValue( 100 );
	Slider1->setOrientation( QSlider::Horizontal );
	Slider1->setTickmarks( QSlider::Right );
	Slider1->setValue(polyFd);
	if (polyFd == 0)
		Konvex->setChecked(false);
	Layout8->addWidget( Slider1 );
	Layout9->addLayout( Layout8 );
	Layout10->addLayout( Layout9 );
	Layout11->addLayout( Layout10 );
	Preview = new QLabel( this, "Preview" );
	Preview->setMinimumSize( QSize( 106, 106 ) );
	Preview->setMaximumSize( QSize( 106, 106 ) );
	Preview->setFrameShape( QLabel::Panel );
	Preview->setFrameShadow( QLabel::Sunken );
	Preview->setLineWidth(2);
	Preview->setAlignment(AlignCenter);
	Preview->setPixmap(*Pre);
	Layout11->addWidget( Preview );
	PolygonPropsLayout->addLayout( Layout11 );

	UpdatePreView();
	//tooltips
	QToolTip::add( Ecken, "<qt>" + tr( "Number of corners for polygons" ) + "</qt>" );
	QToolTip::add( Faktor2, "<qt>" + tr( "Degrees of rotation for polygons" ) + "</qt>" );
	QToolTip::add( Slider2, "<qt>" + tr( "Degrees of rotation for polygons" ) + "</qt>" );
	QToolTip::add( Konvex, "<qt>" + tr( "Apply Convex/Concave Factor to change shape of Polygons" ) + "</qt>" );
	QToolTip::add( Preview, tr( "Sample Polygon" ) );
	QToolTip::add( Faktor, "<qt>" + tr( "A negative value will make the polygon concave (or star shaped), a positive value will make it convex" ) + "</qt>" );
	QToolTip::add( Slider1, tr( "A negative value will make the polygon concave (or star shaped), a positive value will make it convex" ) + "</qt>");
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
	Pre->fill(white);
	QPainter p;
	p.begin(Pre);
	p.setBrush(NoBrush);
	p.setPen(black);
	QPointArray pp = RegularPolygon(100, 100, Ecken->value(), Konvex->isChecked(), GetFaktor(), Slider2->value());
	QRect br = pp.boundingRect();
	if (br.x() < 0)
		pp.translate(-br.x(), 0);
	if (br.y() < 0)
		pp.translate(0, -br.y());
	br = pp.boundingRect();
	if ((br.height() > 100) || (br.width() > 100))
	{
		QWMatrix ma;
		double sca = 100.0 / static_cast<double>(QMAX(br.width(), br.height()));
		ma.scale(sca, sca);
		pp = ma * pp;
	}
	p.drawPolygon(pp);
	p.end();
	Preview->setPixmap(*Pre);
}

double PolygonWidget::GetZeroFaktor()
{
	return sqrt(pow(1.0,2.0)-pow(((sin((360.0/(Ecken->value()*2))/180*M_PI)* 2.0)/2.0),2.0));
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
