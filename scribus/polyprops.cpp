/****************************************************************************
** Form implementation generated from reading ui file 'polygonprops.ui'
**
** Created: Fre Mai 10 14:30:44 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "polyprops.h"
#include "polyprops.moc"
#include <qpointarray.h>
#include <qpainter.h>
#include <qrect.h>
#include <qwmatrix.h>
#include <cmath>

extern int PolyC;
extern int PolyFd;
extern double PolyF;
extern bool PolyS;
extern double PolyR;
extern QPixmap loadIcon(QString nam);
extern QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota);

PolygonProps::PolygonProps(QWidget* parent) : QDialog( parent, "poly", true, 0 )
{
    setCaption( tr( "Polygon Properties" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
		Pre = new QPixmap(101, 101);
		Pre->fill(white);
    PolygonPropsLayout = new QVBoxLayout( this, 10, 5, "PolygonPropsLayout");
    Layout11 = new QHBoxLayout( 0, 0, 5, "Layout11");
    Layout10 = new QVBoxLayout( 0, 0, 5, "Layout10");
    Layout2 = new QHBoxLayout( 0, 0, 5, "Layout2");
    Text1 = new QLabel( this, "Text1" );
    Text1->setText( tr( "Corners:" ) );
    Layout2->addWidget( Text1 );
    Ecken = new QSpinBox( this, "Ecken" );
    Ecken->setMaxValue( 999 );
    Ecken->setMinValue( 3 );
		Ecken->setValue(PolyC);
    Layout2->addWidget( Ecken );
    Layout10->addLayout( Layout2 );


    Layout9_2 = new QHBoxLayout( 0, 0, 5, "Layout9_2");
    Layout8_2 = new QVBoxLayout( 0, 0, 5, "Layout8_2");
    Layout7_2 = new QHBoxLayout( 0, 0, 5, "Layout7_2");
    Text2_2 = new QLabel( this, "Text2_2" );
    Text2_2->setText( tr( "Rotation:" ) );
    Layout7_2->addWidget( Text2_2 );
    Faktor2 = new QSpinBox( this, "Faktor_2" );
    Faktor2->setSuffix(" °");
    Faktor2->setMaxValue( 180 );
    Faktor2->setMinValue( -180 );
		Faktor2->setValue(static_cast<int>(PolyR));
    Layout7_2->addWidget( Faktor2 );
    Layout8_2->addLayout( Layout7_2 );
    Slider2 = new QSlider( this, "Slider1_2" );
    Slider2->setMinValue( -180 );
    Slider2->setMaxValue( 180 );
		Slider2->setValue(static_cast<int>(PolyR));
    Slider2->setOrientation( QSlider::Horizontal );
    Slider2->setTickmarks( QSlider::Right );
    Layout8_2->addWidget( Slider2 );
    Layout9_2->addLayout( Layout8_2 );
    Layout10->addLayout( Layout9_2 );

    Konvex = new QCheckBox( this, "Konvex" );
    Konvex->setText( tr( "Convex Polygon" ) );
		Konvex->setChecked(PolyS);
    Layout10->addWidget( Konvex );
    Layout9 = new QHBoxLayout( 0, 0, 5, "Layout9");
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout9->addItem( spacer );
    Layout8 = new QVBoxLayout( 0, 0, 5, "Layout8");
    Layout7 = new QHBoxLayout( 0, 0, 5, "Layout7");
    Text2 = new QLabel( this, "Text2" );
    Text2->setText( tr( "Factor:" ) );
    Layout7->addWidget( Text2 );
    Faktor = new QSpinBox( this, "Faktor" );
    Faktor->setSuffix( tr( " %" ) );
    Faktor->setMaxValue( 100 );
    Faktor->setMinValue( -100 );
		Faktor->setValue(PolyFd);
    Layout7->addWidget( Faktor );
    Layout8->addLayout( Layout7 );
    Slider1 = new QSlider( this, "Slider1" );
    Slider1->setMinValue( -100 );
    Slider1->setMaxValue( 100 );
    Slider1->setOrientation( QSlider::Horizontal );
    Slider1->setTickmarks( QSlider::Right );
		Slider1->setValue(PolyFd);
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

    Layout1_2 = new QHBoxLayout( 0, 0, 6, "Layout1_2");
    PushButton1 = new QPushButton( this, "PushButton1" );
    PushButton1->setMinimumSize( QSize( 90, 0 ) );
    PushButton1->setText( tr( "OK" ) );
    PushButton1->setDefault( true );
    Layout1_2->addWidget( PushButton1 );
    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1_2->addItem( spacer_2 );
    PushButton2 = new QPushButton( this, "PushButton2" );
    PushButton2->setText( tr( "Cancel" ) );
    Layout1_2->addWidget( PushButton2 );
    PolygonPropsLayout->addLayout( Layout1_2 );

    UpdatePreView();
    // signals and slots connections
    connect(PushButton1, SIGNAL(clicked()), this, SLOT(accept()));
    connect(PushButton2, SIGNAL(clicked()), this, SLOT(reject()));
    connect(Faktor, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin(int)));
    connect(Faktor2, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin2(int)));
    connect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
    connect(Slider1, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
    connect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
    connect(Slider2, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
		connect(Ecken, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
		connect(Konvex, SIGNAL(clicked()), this, SLOT(UpdatePreView()));
}

void PolygonProps::ValFromSpin2(int a)
{
	disconnect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
	Slider2->setValue(a);
  connect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
}

void PolygonProps::ValFromSpin(int a)
{
	disconnect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
	Slider1->setValue(a);
  connect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
}

void PolygonProps::UpdatePreView()
{
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

double PolygonProps::GetZeroFaktor()
{
	return sqrt(pow(1,2)-pow(((sin((360.0/(Ecken->value()*2))/180*M_PI)* 2.0)/2.0),2));
}

double PolygonProps::GetMaxFaktor()
{
	double win = (360.0/(Ecken->value()*2)) / 180.0 * M_PI;
	double ret;
	if ((360.0/(Ecken->value()*2)) > 45)
		ret = 1/sin(win);
	else
		ret = 1/cos(win);
	return ret;
}

double PolygonProps::GetFaktor()
{
	int val = Slider1->value();
	if (val < 0)
		{
		PFactor = GetZeroFaktor() * (100.0 + val) / 100.0;
		}
	else
		{
		double ma = GetMaxFaktor();
		double mi = GetZeroFaktor();
		PFactor = ((ma - mi) * val / 100.0) + mi;
		}
	return PFactor;
}
