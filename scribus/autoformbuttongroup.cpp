/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qimage.h>
#include <qlayout.h>
#include <qbitmap.h>
#include <qpixmap.h>

#include "autoformbuttongroup.h"
//#include "autoformbuttongroup.moc"
#include "fpointarray.h"
#include "scpainter.h"
#include "util.h"

AutoformButtonGroup::AutoformButtonGroup( QWidget* parent ) : QFrame( parent, "autoformbuttongroup" )
{
	buttonGroup1Layout = new QGridLayout( this );
	buttonGroup1Layout->setSpacing( 0 );
	buttonGroup1Layout->setMargin( 0 );
	toolButton1 = new QToolButton( this, "toolButton1" );
	toolButton1->setCheckable( true );
	toolButton1->setIcon(QIcon(getIconPixmap(0,16)));
	buttonGroup1Layout->addWidget( toolButton1, 0, 0 );
	toolButton2 = new QToolButton( this, "toolButton2" );
	toolButton2->setCheckable( true );
	toolButton2->setIcon(QIcon(getIconPixmap(1,16)));
	buttonGroup1Layout->addWidget( toolButton2, 0, 1 );
	toolButton3 = new QToolButton( this, "toolButton3" );
	toolButton3->setCheckable( true );
	toolButton3->setIcon(QIcon(getIconPixmap(2,16)));
	buttonGroup1Layout->addWidget( toolButton3, 0, 2 );
	toolButton4 = new QToolButton( this, "toolButton4" );
	toolButton4->setCheckable( true );
	toolButton4->setIcon(QIcon(getIconPixmap(3,16)));
	buttonGroup1Layout->addWidget( toolButton4, 0, 3 );

	toolButton5 = new QToolButton( this, "toolButton3" );
	toolButton5->setCheckable( true );
	toolButton5->setIcon(QIcon(getIconPixmap(4,16)));
	buttonGroup1Layout->addWidget( toolButton5, 1, 0 );
	toolButton6 = new QToolButton( this, "toolButton4" );
	toolButton6->setCheckable( true );
	toolButton6->setIcon(QIcon(getIconPixmap(5,16)));
	buttonGroup1Layout->addWidget( toolButton6, 1, 1 );
	toolButton7 = new QToolButton( this, "toolButton3" );
	toolButton7->setCheckable( true );
	toolButton7->setIcon(QIcon(getIconPixmap(6,16)));
	buttonGroup1Layout->addWidget( toolButton7, 1, 2 );
	toolButton8 = new QToolButton( this, "toolButton4" );
	toolButton8->setCheckable( true );
	toolButton8->setIcon(QIcon(getIconPixmap(7,16)));
	buttonGroup1Layout->addWidget( toolButton8, 1, 3 );

	toolButton9 = new QToolButton( this, "toolButton4" );
	toolButton9->setCheckable( true );
	toolButton9->setIcon(QIcon(getIconPixmap(8,16)));
	buttonGroup1Layout->addWidget( toolButton9, 2, 0 );
	toolButton10 = new QToolButton( this, "toolButton4" );
	toolButton10->setCheckable( true );
	toolButton10->setIcon(QIcon(getIconPixmap(9,16)));
	buttonGroup1Layout->addWidget( toolButton10, 2, 1 );
	toolButton11 = new QToolButton( this, "toolButton4" );
	toolButton11->setCheckable( true );
	toolButton11->setIcon(QIcon(getIconPixmap(10,16)));
	buttonGroup1Layout->addWidget( toolButton11, 2, 2 );
	toolButton12 = new QToolButton( this, "toolButton4" );
	toolButton12->setCheckable( true );
	toolButton12->setIcon(QIcon(getIconPixmap(11,16)));
	buttonGroup1Layout->addWidget( toolButton12, 2, 3 );

	toolButton13 = new QToolButton( this, "toolButton4" );
	toolButton13->setCheckable( true );
	toolButton13->setIcon(QIcon(getIconPixmap(12,16)));
	buttonGroup1Layout->addWidget( toolButton13, 3, 0 );
	toolButton14 = new QToolButton( this, "toolButton4" );
	toolButton14->setCheckable( true );
	toolButton14->setIcon(QIcon(getIconPixmap(13,16)));
	buttonGroup1Layout->addWidget( toolButton14, 3, 1 );
	toolButton15 = new QToolButton( this, "toolButton4" );
	toolButton15->setCheckable( true );
	toolButton15->setIcon(QIcon(getIconPixmap(14,16)));
	buttonGroup1Layout->addWidget( toolButton15, 3, 2 );
	toolButton16 = new QToolButton( this, "toolButton4" );
	toolButton16->setCheckable( true );
	toolButton16->setIcon(QIcon(getIconPixmap(15,16)));
	buttonGroup1Layout->addWidget( toolButton16, 3, 3 );

	toolButton17 = new QToolButton( this, "toolButton4" );
	toolButton17->setCheckable( true );
	toolButton17->setIcon(QIcon(getIconPixmap(16,16)));
	buttonGroup1Layout->addWidget( toolButton17, 4, 0 );
	toolButton18 = new QToolButton( this, "toolButton4" );
	toolButton18->setCheckable( true );
	toolButton18->setIcon(QIcon(getIconPixmap(17,16)));
	buttonGroup1Layout->addWidget( toolButton18, 4, 1 );
	buttonGroup = new QButtonGroup(this);
	buttonGroup->addButton(toolButton1, 0);
	buttonGroup->addButton(toolButton2, 1);
	buttonGroup->addButton(toolButton3, 2);
	buttonGroup->addButton(toolButton4, 3);
	buttonGroup->addButton(toolButton5, 4);
	buttonGroup->addButton(toolButton6, 5);
	buttonGroup->addButton(toolButton7, 6);
	buttonGroup->addButton(toolButton8, 7);
	buttonGroup->addButton(toolButton9, 8);
	buttonGroup->addButton(toolButton10, 9);
	buttonGroup->addButton(toolButton11, 10);
	buttonGroup->addButton(toolButton12, 11);
	buttonGroup->addButton(toolButton13, 12);
	buttonGroup->addButton(toolButton14, 13);
	buttonGroup->addButton(toolButton15, 14);
	buttonGroup->addButton(toolButton16, 15);
	buttonGroup->addButton(toolButton17, 16);
	buttonGroup->addButton(toolButton18, 17);
	
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(selForm(int)));

}

double* AutoformButtonGroup::getShapeData(int a, int *n)
{
	double *vals = NULL;
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes1[] = {100.0, 50.0, 100.0, 77.615235, 50.0, 100.0,
									77.615235, 100.0, 50.0, 100.0,
		 							22.385765, 100.0,
									0.0, 50.0, 0.0, 77.615235, 0.0, 50.0, 0.0, 22.385765, 50.0, 0.0,
									22.385765, 0.0,
									50.0, 0.0, 77.615235, 0.0, 100.0, 50.0, 100.0, 22.385765};
	static double AutoShapes2[] = {0.0, 0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0,
								 	0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes3[] = {0.0, 25.0, 0.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
								 	25.0, 0.0, 25.0, 0.0, 25.0, 0.0, 25.0, 0.0, 75.0, 0.0, 75.0, 0.0,
								 	75.0, 0.0, 75.0, 0.0, 75.0, 25.0, 75.0, 25.0, 75.0, 25.0, 75.0, 25.0,
								 	100.0, 25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 25.0,	100.0, 75.0, 100.0, 75.0,
								 	100.0, 75.0, 100.0, 75.0, 75.0, 75.0, 75.0, 75.0, 75.0, 75.0, 75.0, 75.0,
								 	75.0, 100.0, 75.0, 100.0, 75.0, 100.0, 75.0, 100.0, 25.0, 100.0, 25.0, 100.0,
								 	25.0, 100.0, 25.0, 100.0, 25.0, 75.0, 25.0, 75.0, 25.0, 75.0, 25.0, 75.0,
								 	0.0, 75.0, 0.0, 75.0, 0.0, 75.0, 0.0, 75.0,	0.0, 25.0, 0.0, 25.0};
	static double AutoShapes4[] = {0.0, 50.0, 0.0, 50.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0,
									50.0, 25.0, 50.0, 25.0, 50.0, 25.0, 50.0, 25.0,	100.0, 25.0, 100.0, 25.0,
									100.0, 25.0, 100.0,  25.0, 100.0, 75.0, 100.0, 75.0, 100.0, 75.0, 100.0, 75.0,
									50.0, 75.0, 50.0, 75.0, 50.0, 75.0, 50.0, 75.0, 50.0, 100.0, 50.0, 100.0,
									50.0, 100.0, 50.0, 100.0,	0.0, 50.0, 0.0, 50.0};
	static double AutoShapes5[] = {0.0, 25.0, 0.0, 25.0, 50.0, 25.0, 50.0, 25.0, 50.0, 25.0, 50.0, 25.0,
									50.0, 0.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0,	100.0, 50.0, 100.0, 50.0,
									100.0, 50.0, 100.0, 50.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0,
									50.0, 75.0, 50.0, 75.0, 50.0, 75.0, 50.0, 75.0, 0.0, 75.0, 0.0, 75.0,
									0.0, 75.0, 0.0, 75.0,	0.0, 25.0, 0.0, 25.0};
	static double AutoShapes6[] = {0.0, 50.0, 0.0, 50.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0,
									100.0, 50.0, 100.0, 50.0,	100.0, 50.0, 100.0, 50.0, 75.0, 50.0, 75.0, 50.0,
									75.0, 50.0, 75.0, 50.0, 75.0, 100.0, 75.0, 100.0, 75.0, 100.0, 75.0, 100.0,
									25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 25.0, 50.0, 25.0, 50.0,
									25.0, 50.0, 25.0, 50.0,	0.0, 50.0, 0.0, 50.0};
	static double AutoShapes7[] = {0.0, 50.0, 0.0, 50.0, 25.0, 50.0, 25.0, 50.0, 25.0, 50.0, 25.0, 50.0,
									25.0, 0.0, 25.0, 0.0, 25.0, 0.0, 25.0, 0.0, 75.0, 0.0, 75.0, 0.0,
									75.0, 0.0, 75.0, 0.0, 75.0, 50.0, 75.0, 50.0, 75.0, 50.0, 75.0, 50.0,
									100.0, 50.0, 100.0, 50.0,	100.0, 50.0, 100.0, 50.0, 50.0, 100.0, 50.0, 100.0,
									50.0, 100.0, 50.0, 100.0,	0.0, 50.0, 0.0, 50.0};
	static double AutoShapes8[] = {0.0, 50.0, 0.0, 22.385765, 100.0, 0.0, 44.77716, 0.0,	100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0,	100.0, 100.0, 44.77716, 100.0, 0.0, 50.0,
									0.0, 77.615235};
	static double AutoShapes9[] = {100.0, 50.0, 100.0, 77.615235, 0.0, 100.0,
55.22284, 100.0, 0.0, 100.0, 0.0, 100.0,
									0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 55.22281, 0.0, 100.0, 50.0, 100.0, 22.385765};
	static double AutoShapes10[] = {100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 0.0, 100.0, 0.0, 44.7715,
									 50.0, 0.0, 22.3858, 0.0, 50.0, 0.0, 77.6152, 0.0, 100.0, 100.0, 100.0,
44.7715};
	static double AutoShapes11[] = {100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 55.2285, 50.0, 100.0,
									 22.3858, 100.0, 50.0, 100.0, 77.6152, 100.0, 100.0, 0.0, 100.0, 55.2285};
	static double AutoShapes12[] = {0.0, 50.0, 0.0, 50.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 50.0, 0.0, 50.0};
	static double AutoShapes13[] = {0.0, 0.0, 0.0, 0.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0,
									 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes14[] = {0.0, 100.0, 0.0, 100.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0,
									 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0};
	static double AutoShapes15[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes16[] = {25.0, 0.0, 25.0, 0.0, 75.0, 0.0, 75.0, 0.0, 75.0, 0.0, 75.0, 13.8071,
									 100.0, 25.0, 86.1929, 25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 75.0, 100.0, 75.0,
									 100.0, 75.0, 86.1929, 75.0, 75.0, 100.0, 75.0, 86.1929, 75.0, 100.0, 75.0, 100.0,
									 25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 25.0, 86.1929, 0.0, 75.0, 13.8071, 75.0,
									 0.0, 75.0, 0.0, 75.0, 0.0, 25.0, 0.0, 25.0, 0.0, 25.0, 13.8071, 25.0,
									 25.0, 0.0, 25.0, 13.8071};
	static double AutoShapes17[] = {100.0, 50.0, 100.0, 77.6152, 50.0, 100.0, 77.6152, 100.0, 50.0, 100.0, 22.3858, 100.0,
									 0.0, 50.0, 0.0, 77.6152, 0.0,  50.0, 0.0, 22.3858, 50.0, 0.0, 22.3858, 0.0,
									 50.0, 0.0, 77.6152, 0.0, 100.0, 50.0, 100.0, 22.3858, -1.0, -1.0, -1.0, -1.0,
									 75.0, 50.0, 75.0, 63.8076, 50.0, 75.0, 63.8076, 75.0, 50.0, 75.0, 36.1929, 75.0,
									 25.0, 50.0, 25.0, 63.8076, 25.0, 50.0, 25.0, 36.1929, 50.0, 25.0, 36.1929, 25.0,
									 50.0, 25.0, 63.8076, 25.0, 75.0, 50.0, 75.0, 36.1929};
	switch (a)
		{
		case 0:
			*n = 32;
			vals = AutoShapes0;
			break;
		case 1:
			*n = 32;
			vals = AutoShapes1;
			break;
		case 2:
			*n = 24;
			vals = AutoShapes2;
			break;
		case 3:
			*n = 96;
			vals = AutoShapes3;
			break;
		case 4:
			*n = 56;
			vals = AutoShapes4;
			break;
		case 5:
			*n = 56;
			vals = AutoShapes5;
			break;
		case 6:
			*n = 56;
			vals = AutoShapes6;
			break;
		case 7:
			*n = 56;
			vals = AutoShapes7;
			break;
		case 8:
			*n = 24;
			vals = AutoShapes8;
			break;
		case 9:
			*n = 24;
			vals = AutoShapes9;
			break;
		case 10:
			*n = 24;
			vals = AutoShapes10;
			break;
		case 11:
			*n = 24;
			vals = AutoShapes11;
			break;
		case 12:
			*n = 24;
			vals = AutoShapes12;
			break;
		case 13:
			*n = 24;
			vals = AutoShapes13;
			break;
		case 14:
			*n = 24;
			vals = AutoShapes14;
			break;
		case 15:
			*n = 24;
			vals = AutoShapes15;
			break;
		case 16:
			*n = 64;
			vals = AutoShapes16;
			break;
		case 17:
			*n = 68;
			vals = AutoShapes17;
			break;
		default:
			*n = 32;
			vals = AutoShapes0;
			break;
		}
	return vals;
}

void AutoformButtonGroup::selForm(int a)
{
	int n;
	double* AutoShapes;
	AutoShapes = getShapeData(a, &n);
	emit buttonClicked(a);
	emit FormSel(a, n, AutoShapes);
}

QPixmap AutoformButtonGroup::getIconPixmap(int nr, int pixmapSize)
{
	if (pixmapSize!=16 && pixmapSize!=22)
		qDebug("%s", QString("Autoformbuttongroup: Only 16px or 22px sizes supported!").ascii());
	else
	if (nr<18)
	{
		QString strSize=QString("%1").arg(pixmapSize);
		switch(nr)
		{
			case 0:
				return loadIcon(strSize+"/draw-rectangle.png");
			case 1:
				return loadIcon(strSize+"/draw-ellipse.png");
			case 2:
				return loadIcon(strSize+"/draw-triangle.png");
			case 3:
				return loadIcon(strSize+"/draw-cross.png");
			case 4:
				return loadIcon(strSize+"/draw-arrow-back.png");
			case 5:
				return loadIcon(strSize+"/draw-arrow-forward.png");
			case 6:
				return loadIcon(strSize+"/draw-arrow-up.png");
			case 7:
				return loadIcon(strSize+"/draw-arrow-down.png");
			case 8:
				return loadIcon(strSize+"/draw-halfcircle1.png");
			case 9:
				return loadIcon(strSize+"/draw-halfcircle2.png");
			case 10:
				return loadIcon(strSize+"/draw-halfcircle3.png");
			case 11:
				return loadIcon(strSize+"/draw-halfcircle4.png");
			case 12:
				return loadIcon(strSize+"/draw-triangle1.png");
			case 13:
				return loadIcon(strSize+"/draw-triangle2.png");
			case 14:
				return loadIcon(strSize+"/draw-triangle3.png");
			case 15:
				return loadIcon(strSize+"/draw-triangle4.png");
			case 16:
				return loadIcon(strSize+"/draw-square-inverted-corners.png");
			case 17:
				return loadIcon(strSize+"/draw-donut.png");
		}
	}
	int count = 0;
	double* AutoShapes;
	FPointArray Path;
	QPixmap Ico(22, 22);
	Path.resize(0);
	AutoShapes = getShapeData(nr, &count);
	for (int a = 0; a < count-3; a += 4)
	{
		if (AutoShapes[a] < 0)
		{
			Path.setMarker();
			continue;
		}
		double x1 = 16 * AutoShapes[a] / 100.0;
		double y1 = 16 * AutoShapes[a+1] / 100.0;
		double x2 = 16 * AutoShapes[a+2] / 100.0;
		double y2 = 16 * AutoShapes[a+3] / 100.0;
		Path.addPoint(x1, y1);
		Path.addPoint(x2, y2);
	}
	ScPainter *painter = new ScPainter(&Ico, 22, 22);
	painter->setBrush(qRgb(255, 255, 255));
	painter->setPen(qRgb(0, 0, 0));
	painter->setFillMode(1);
	painter->translate(3.0, 3.0);
	painter->setupPolygon(&Path);
	painter->setLineWidth(1.0);
	painter->drawPolygon();
	painter->drawPolyLine();
	painter->end();
	delete painter;
	Ico.setMask( Ico.createHeuristicMask() );

	return Ico;
}
