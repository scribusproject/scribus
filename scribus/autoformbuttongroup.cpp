/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QImage>
#include <QLayout>
#include <QBitmap>
#include <QPixmap>

#include "autoformbuttongroup.h"
#include "fpointarray.h"
#include "scpainter.h"
#include "util_icon.h"

AutoformButtonGroup::AutoformButtonGroup( QWidget* parent ) : QFrame( parent )
{
	QSizePolicy policy = QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3));
	buttonGroup1Layout = new QGridLayout( this );
	buttonGroup1Layout->setSpacing( 0 );
	buttonGroup1Layout->setMargin( 0 );
	toolButton1 = new QToolButton( this );
	toolButton1->setCheckable( true );
	toolButton1->setIcon(QIcon(getIconPixmap(0,16)));
	toolButton1->setIconSize(QSize(16, 16));
	toolButton1->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton1, 0, 0 );
	toolButton2 = new QToolButton( this );
	toolButton2->setCheckable( true );
	toolButton2->setIcon(QIcon(getIconPixmap(1,16)));
	toolButton2->setIconSize(QSize(16, 16));
	toolButton2->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton2, 0, 1 );
	toolButton3 = new QToolButton( this );
	toolButton3->setCheckable( true );
	toolButton3->setIcon(QIcon(getIconPixmap(2,16)));
	toolButton3->setIconSize(QSize(16, 16));
	toolButton3->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton3, 0, 2 );
	toolButton4 = new QToolButton( this );
	toolButton4->setCheckable( true );
	toolButton4->setIcon(QIcon(getIconPixmap(3,16)));
	toolButton4->setIconSize(QSize(16, 16));
	toolButton4->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton4, 0, 3 );

	toolButton5 = new QToolButton( this );
	toolButton5->setCheckable( true );
	toolButton5->setIcon(QIcon(getIconPixmap(4,16)));
	toolButton5->setIconSize(QSize(16, 16));
	toolButton5->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton5, 1, 0 );
	toolButton6 = new QToolButton( this );
	toolButton6->setCheckable( true );
	toolButton6->setIcon(QIcon(getIconPixmap(5,16)));
	toolButton6->setIconSize(QSize(16, 16));
	toolButton6->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton6, 1, 1 );
	toolButton7 = new QToolButton( this );
	toolButton7->setCheckable( true );
	toolButton7->setIcon(QIcon(getIconPixmap(6,16)));
	toolButton7->setIconSize(QSize(16, 16));
	toolButton7->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton7, 1, 2 );
	toolButton8 = new QToolButton( this );
	toolButton8->setCheckable( true );
	toolButton8->setIcon(QIcon(getIconPixmap(7,16)));
	toolButton8->setIconSize(QSize(16, 16));
	toolButton8->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton8, 1, 3 );

	toolButton9 = new QToolButton( this );
	toolButton9->setCheckable( true );
	toolButton9->setIcon(QIcon(getIconPixmap(8,16)));
	toolButton9->setIconSize(QSize(16, 16));
	toolButton9->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton9, 2, 0 );
	toolButton10 = new QToolButton( this );
	toolButton10->setCheckable( true );
	toolButton10->setIcon(QIcon(getIconPixmap(9,16)));
	toolButton10->setIconSize(QSize(16, 16));
	toolButton10->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton10, 2, 1 );
	toolButton11 = new QToolButton( this );
	toolButton11->setCheckable( true );
	toolButton11->setIcon(QIcon(getIconPixmap(10,16)));
	toolButton11->setIconSize(QSize(16, 16));
	toolButton11->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton11, 2, 2 );
	toolButton12 = new QToolButton( this );
	toolButton12->setCheckable( true );
	toolButton12->setIcon(QIcon(getIconPixmap(11,16)));
	toolButton12->setIconSize(QSize(16, 16));
	toolButton12->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton12, 2, 3 );

	toolButton13 = new QToolButton( this );
	toolButton13->setCheckable( true );
	toolButton13->setIcon(QIcon(getIconPixmap(12,16)));
	toolButton13->setIconSize(QSize(16, 16));
	toolButton13->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton13, 3, 0 );
	toolButton14 = new QToolButton( this );
	toolButton14->setCheckable( true );
	toolButton14->setIcon(QIcon(getIconPixmap(13,16)));
	toolButton14->setIconSize(QSize(16, 16));
	toolButton14->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton14, 3, 1 );
	toolButton15 = new QToolButton( this );
	toolButton15->setCheckable( true );
	toolButton15->setIcon(QIcon(getIconPixmap(14,16)));
	toolButton15->setIconSize(QSize(16, 16));
	toolButton15->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton15, 3, 2 );
	toolButton16 = new QToolButton( this );
	toolButton16->setCheckable( true );
	toolButton16->setIcon(QIcon(getIconPixmap(15,16)));
	toolButton16->setIconSize(QSize(16, 16));
	toolButton16->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton16, 3, 3 );

	toolButton17 = new QToolButton( this );
	toolButton17->setCheckable( true );
	toolButton17->setIcon(QIcon(getIconPixmap(16,16)));
	toolButton17->setIconSize(QSize(16, 16));
	toolButton17->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton17, 4, 0 );
	toolButton18 = new QToolButton( this );
	toolButton18->setCheckable( true );
	toolButton18->setIcon(QIcon(getIconPixmap(17,16)));
	toolButton18->setIconSize(QSize(16, 16));
	toolButton18->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton18, 4, 1 );

	toolButton19 = new QToolButton( this );
	toolButton19->setCheckable( true );
	toolButton19->setIcon(QIcon(getIconPixmap(18,16)));
	toolButton19->setIconSize(QSize(16, 16));
	toolButton19->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton19, 4, 2 );

	toolButton20 = new QToolButton( this );
	toolButton20->setCheckable( true );
	toolButton20->setIcon(QIcon(getIconPixmap(19,16)));
	toolButton20->setIconSize(QSize(16, 16));
	toolButton20->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton20, 4, 3 );

	toolButton21 = new QToolButton( this );
	toolButton21->setCheckable( true );
	toolButton21->setIcon(QIcon(getIconPixmap(20,16)));
	toolButton21->setIconSize(QSize(16, 16));
	toolButton21->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton21, 5, 0 );

	toolButton22 = new QToolButton( this );
	toolButton22->setCheckable( true );
	toolButton22->setIcon(QIcon(getIconPixmap(21,16)));
	toolButton22->setIconSize(QSize(16, 16));
	toolButton22->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton22, 5, 1 );

	toolButton23 = new QToolButton( this );
	toolButton23->setCheckable( true );
	toolButton23->setIcon(QIcon(getIconPixmap(22,16)));
	toolButton23->setIconSize(QSize(16, 16));
	toolButton23->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton23, 5, 2 );

	toolButton24 = new QToolButton( this );
	toolButton24->setCheckable( true );
	toolButton24->setIcon(QIcon(getIconPixmap(23,16)));
	toolButton24->setIconSize(QSize(16, 16));
	toolButton24->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton24, 5, 3 );

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
	buttonGroup->addButton(toolButton19, 18);
	buttonGroup->addButton(toolButton20, 19);
	buttonGroup->addButton(toolButton21, 20);
	buttonGroup->addButton(toolButton22, 21);
	buttonGroup->addButton(toolButton23, 22);
	buttonGroup->addButton(toolButton24, 23);
	
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(selForm(int)));

}

double* AutoformButtonGroup::getShapeData(int a, int *n)
{
	double *vals = NULL;
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0,
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes1[] = {100.0, 50.0, 100.0, 77.615235, 50.0, 100.0, 77.615235, 100.0, 50.0, 100.0, 22.385765, 100.0,
									0.0, 50.0, 0.0, 77.615235, 0.0, 50.0, 0.0, 22.385765, 50.0, 0.0, 22.385765, 0.0,
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
	static double AutoShapes9[] = {100.0, 50.0, 100.0, 77.615235, 0.0, 100.0, 55.22284, 100.0, 0.0, 100.0, 0.0, 100.0,
									0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 55.22281, 0.0, 100.0, 50.0, 100.0, 22.385765};
	static double AutoShapes10[] = {100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 0.0, 100.0, 0.0, 44.7715,
									 50.0, 0.0, 22.3858, 0.0, 50.0, 0.0, 77.6152, 0.0, 100.0, 100.0, 100.0, 44.7715};
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
	static double AutoShapes18[] = {50.185, 100, 50.185, 100, 42.0645, 82.3504, 47.3401, 90.5964, 42.0645, 82.3504, 42.0645, 82.3504, 21.6517, 56.7327, 36.7888,
									74.1044, 21.6517, 56.7327, 21.6517, 56.7327, 7.9825, 40.5569, 10.5513, 43.9838, 7.9825, 40.5569, 7.9825, 40.5569, 1.87893,
									30.3195, 3.76079, 34.9805, 1.87893, 30.3195, 1.87893, 30.3195, 0, 20.8776, 0, 25.6585, 0, 20.8776, 0, 20.8776, 6.78762, 6.05574,
									0, 12.0324, 6.78762, 6.05574, 6.78762, 6.05574, 23.5776, 0.0790433, 13.5782, 0.0790433, 23.5776, 0.0790433, 23.5776, 0.0790433,
									41.1015, 6.29542, 33.671, 0.0790433, 41.1015, 6.29542, 41.1015, 6.29542, 50.185, 20.0796, 46.6972, 10.9156, 50.185, 20.0796,
									50.185, 20.0796, 58.7165, 6.37446, 53.2118, 11.0763, 58.7165, 6.37446, 58.7165, 6.37446, 76.3314, 0, 66.332, 0, 76.3314, 0,
									76.3314, 0, 93.1184, 5.9359, 86.2398, 0, 93.1184, 5.9359, 93.1184, 5.9359, 100, 20.0796, 100, 11.8718, 100, 20.0796, 100,
									20.0796, 95.9632, 35.0213, 100, 27.2521, 95.9632, 35.0213, 95.9632, 35.0213, 80.3682, 55.3788, 91.9265, 42.7879, 80.3682,
									55.3788, 80.3682, 55.3788, 58.4405, 82.4702, 65.3221, 71.8733, 58.4405, 82.4702, 58.4405, 82.4702, 50.185, 100, 53.0268, 90.8361};
	static double AutoShapes19[] = {0, 50, 0, 50, 33.3333, 0, 33.3333, 0, 33.3333, 0, 33.3333, 0, 33.3333, 25, 33.3333, 25, 33.3333, 25, 33.3333, 25, 66.6667,
									25, 66.6667, 25, 66.6667, 25, 66.6667, 25, 66.6667, 0, 66.6667, 0, 66.6667, 0, 66.6667, 0, 100, 50, 100, 50, 100, 50, 100, 50,
									66.6667, 100, 66.6667, 100, 66.6667, 100, 66.6667, 100, 66.6667, 75, 66.6667, 75, 66.6667, 75, 66.6667, 75, 33.3333, 75, 33.3333,
									75, 33.3333, 75, 33.3333, 75, 33.3333, 100, 33.3333, 100, 33.3333, 100, 33.3333, 100, 0, 50, 0, 50};
	static double AutoShapes20[] = {50, 0, 50, 0, 100, 33.3333, 100, 33.3333, 100, 33.3333, 100, 33.3333, 75, 33.3333, 75, 33.3333, 75, 33.3333, 75, 33.3333, 75,
									66.6667, 75, 66.6667, 75, 66.6667, 75, 66.6667, 100, 66.6667, 100, 66.6667, 100, 66.6667, 100, 66.6667, 50, 100, 50, 100, 50,
									100, 50, 100, 0, 66.6667, 0, 66.6667, 0, 66.6667, 0, 66.6667, 25, 66.6667, 25, 66.6667, 25, 66.6667, 25, 66.6667, 25, 33.3333,
									25, 33.3333, 25, 33.3333, 25, 33.3333, 0, 33.3333, 0, 33.3333, 0, 33.3333, 0, 33.3333, 50, 0, 50, 0};
	static double AutoShapes21[] = {0, 49.9858, 0, 49.9858, 19.8923, 30.0652, 19.8923, 30.0652, 19.8923, 30.0652, 19.8923, 30.0652, 19.8923, 39.813,
									 19.8923, 39.813, 19.8923, 39.813, 19.8923, 39.813, 39.813, 39.813, 39.813, 39.813, 39.813, 39.813, 39.813, 39.813, 39.813,
									 19.8923, 39.813, 19.8923, 39.813, 19.8923, 39.813, 19.8923, 30.0652, 19.8923, 30.0652, 19.8923, 30.0652, 19.8923, 30.0652,
									 19.8923, 49.9858, 0, 49.9858, 0, 49.9858, 0, 49.9858, 0, 69.9065, 19.8923, 69.9065, 19.8923, 69.9065, 19.8923, 69.9065,
									 19.8923, 60.1587, 19.8923, 60.1587, 19.8923, 60.1587, 19.8923, 60.1587, 19.8923, 60.1587, 39.813, 60.1587, 39.813, 60.1587,
									 39.813, 60.1587, 39.813, 80.0793, 39.813, 80.0793, 39.813, 80.0793, 39.813, 80.0793, 39.813, 80.0793, 30.0652, 80.0793,
									 30.0652, 80.0793, 30.0652, 80.0793, 30.0652, 100, 49.9858, 100, 49.9858, 100, 49.9858, 100, 49.9858, 80.0793, 69.9065,
									 80.0793, 69.9065, 80.0793, 69.9065, 80.0793, 69.9065, 80.0793, 60.1587, 80.0793, 60.1587, 80.0793, 60.1587, 80.0793, 60.1587,
									 60.1587, 60.1587, 60.1587, 60.1587, 60.1587, 60.1587, 60.1587, 60.1587, 60.1587, 80.0793, 60.1587, 80.0793, 60.1587, 80.0793,
									 60.1587, 80.0793, 69.9065, 80.0793, 69.9065, 80.0793, 69.9065, 80.0793, 69.9065, 80.0793, 49.9858, 100, 49.9858, 100, 49.9858,
									 100, 49.9858, 100, 30.0652, 80.0793, 30.0652, 80.0793, 30.0652, 80.0793, 30.0652, 80.0793, 39.813, 80.0793, 39.813, 80.0793,
									 39.813, 80.0793, 39.813, 80.0793, 39.813, 60.1587, 39.813, 60.1587, 39.813, 60.1587, 39.813, 60.1587, 19.8923, 60.1587, 19.8923,
									 60.1587, 19.8923, 60.1587, 19.8923, 60.1587, 19.8923, 69.9065, 19.8923, 69.9065, 19.8923, 69.9065, 19.8923, 69.9065, 0,
									 49.9858, 0, 49.9858};
	static double AutoShapes22[] = {50.5178, 88.7331, 50.5178, 88.7331, 33.2052, 100, 44.7028, 100, 33.2052, 100, 33.2052, 100, 17.7654, 93.5724, 24.0951, 100,
									17.7654, 93.5724, 17.7654, 93.5724, 11.3695, 77.865, 11.3695, 87.1427, 11.3695, 77.865, 11.3695, 77.865, 16.9261, 64.2803,
									11.3695, 71.0408, 16.9261, 64.2803, 16.9261, 64.2803, 4.58708, 57.2568, 9.30233, 62.6898, 4.58708, 57.2568, 4.58708, 57.2568,
									0, 43.935, 0, 51.7559, 0, 43.935, 0, 43.935, 5.81292, 28.4948, 0, 35.1875, 5.81292, 28.4948, 5.81292, 28.4948, 20.2191, 21.8021,
									11.6279, 21.8021, 20.2191, 21.8021, 20.2191, 21.8021, 29.0708, 23.857, 24.1612, 21.8021, 29.0708, 23.857, 29.0708, 23.857,
									28.8124, 17.23, 28.8124, 18.0253, 28.8124, 17.23, 28.8124, 17.23, 35.8532, 4.57206, 28.8124, 9.60853, 35.8532, 4.57206, 35.8532,
									4.57206, 49.8708, 0, 42.1189, 0, 49.8708, 0, 49.8708, 0, 65.2465, 6.62694, 59.5618, 0, 65.2465, 6.62694, 65.2465, 6.62694,
									70.4145, 21.0726, 70.4145, 12.5244, 70.4145, 21.0726, 70.4145, 21.0726, 70.3483, 23.1932, 70.4145, 22.1329, 70.3483, 23.1932,
									70.3483, 23.1932, 79.3282, 20.8733, 75.1938, 20.8733, 79.3282, 20.8733, 79.3282, 20.8733, 94.0568, 27.3687, 88.0496, 20.8733,
									94.0568, 27.3687, 94.0568, 27.3687, 100, 43.0083, 100, 33.9957, 100, 43.0083, 100, 43.0083, 95.4129, 55.9314, 100, 50.4305,
									95.4129, 55.9314, 95.4129, 55.9314, 83.5928, 63.3535, 90.892, 61.4301, 83.5928, 63.3535, 83.5928, 63.3535, 89.5359, 77.5999,
									89.5359, 69.9805, 89.5359, 77.5999, 89.5359, 77.5999, 83.076, 93.3073, 89.5359, 86.6125, 83.076, 93.3073, 83.076, 93.3073,
									67.9587, 100, 76.5499, 100, 67.9587, 100, 67.9587, 100, 50.5178, 88.7331, 56.9116, 100, 50.5178, 88.7331, 50.5178, 88.7331,
									50.5178, 88.7331, 50.5178, 88.7331, -1, -1, -1, -1, -1, -1, -1, -1,
									49.7426, 40.0267, 49.7426, 40.0267, 41.6661, 43.5385, 45.0915, 40.0267, 41.6661, 43.5385, 41.6661, 43.5385, 38.2429, 51.8216,
									38.2429, 47.0502, 38.2429, 51.8216, 38.2429, 51.8216, 41.6661, 59.9733, 38.2429, 56.4615, 41.6661, 59.9733, 41.6661, 59.9733,
									49.7426, 63.485, 45.0915, 63.485, 49.7426, 63.485, 49.7426, 63.485, 57.8171, 60.039, 54.3938, 63.485, 57.8171, 60.039, 57.8171,
									60.039, 61.2403, 51.8216, 61.2403, 56.5273, 61.2403, 51.8216, 61.2403, 51.8216, 57.8171, 43.5385, 61.2403, 47.1838, 57.8171,
									43.5385, 57.8171, 43.5385, 49.7426, 40.0267, 54.2636, 40.0267};
	static double AutoShapes23[] = {100, 50.0376, 100, 50.0376, 85.373, 85.373, 100, 70.7434, 85.373, 85.373, 85.373, 85.373, 50.0402, 100, 70.7459, 100, 50.0402,
									100, 50.0402, 100, 14.6672, 85.373, 29.3344, 100, 14.6672, 85.373, 14.6672, 85.373, 0, 50.0376, 0, 70.7434, 0, 50.0376, 0,
									50.0376, 14.6672, 14.627, 0, 29.2541, 14.6672, 14.627, 14.6672, 14.627, 50.0402, 0, 29.3344, 0, 50.0402, 0, 50.0402, 0, 85.373,
									14.627, 70.7459, 0, 85.373, 14.627, 85.373, 14.627, 100, 50.0376, 100, 29.2541, 100, 50.0376, 100, 50.0376, 100, 50.0376, 100,
									50.0376, -1, -1, -1, -1, -1, -1, -1, -1,
									37.1775, 39.6848, 37.1775, 39.6848, 35.3328, 35.1772, 37.1775, 37.0194, 35.3328, 35.1772, 35.3328, 35.1772, 30.8227, 33.3325,
									33.4906, 33.3325, 30.8227, 33.3325, 30.8227, 33.3325, 26.3528, 35.1772, 28.2351, 33.3325, 26.3528, 35.1772, 26.3528, 35.1772,
									24.4704, 39.6848, 24.4704, 37.0194, 24.4704, 39.6848, 24.4704, 39.6848, 26.3528, 44.1572, 24.4704, 42.2749, 26.3528, 44.1572,
									26.3528, 44.1572, 30.8227, 46.0396, 28.2351, 46.0396, 30.8227, 46.0396, 30.8227, 46.0396, 35.3328, 44.1572, 33.4906, 46.0396,
									35.3328, 44.1572, 35.3328, 44.1572, 37.1775, 39.6848, 37.1775, 42.2749, 37.1775, 39.6848, 37.1775, 39.6848, 37.1775, 39.6848,
									37.1775, 39.6848, -1, -1, -1, -1, -1, -1, -1, -1,
									75.5296, 39.6848, 75.5296, 39.6848, 73.6849, 35.1772, 75.5296, 37.0194, 73.6849, 35.1772, 73.6849, 35.1772, 69.1773, 33.3325,
									71.8427, 33.3325, 69.1773, 33.3325, 69.1773, 33.3325, 64.7048, 35.1772, 66.5872, 33.3325, 64.7048, 35.1772, 64.7048, 35.1772,
									62.8225, 39.6848, 62.8225, 37.0194, 62.8225, 39.6848, 62.8225, 39.6848, 64.7048, 44.1572, 62.8225, 42.2749, 64.7048, 44.1572,
									64.7048, 44.1572, 69.1773, 46.0396, 66.5872, 46.0396, 69.1773, 46.0396, 69.1773, 46.0396, 73.6849, 44.1572, 71.8427, 46.0396,
									73.6849, 44.1572, 73.6849, 44.1572, 75.5296, 39.6848, 75.5296, 42.2749, 75.5296, 39.6848, 75.5296, 39.6848, 75.5296, 39.6848,
									75.5296, 39.6848, -1, -1, -1, -1, -1, -1, -1, -1,
									26.9802, 66.1179, 26.9802, 66.1179, 36.6278, 77.4897, 30.903, 73.5669, 36.6278, 77.4897, 36.6278, 77.4897, 50.0402, 81.41,
									42.3527, 81.41, 50.0402, 81.41, 50.0402, 81.41, 63.4123, 77.4897, 57.6473, 81.41, 63.4123, 77.4897, 63.4123, 77.4897, 73.0976,
									66.1179, 69.1773, 73.5669, 73.0976, 66.1179, 73.0976, 66.1179, 68.1558, 63.2943, 68.1558, 63.2943, 68.1558, 63.2943, 68.1558,
									63.2943, 50.0402, 74.5884, 62.0395, 74.5884, 50.0402, 74.5884, 50.0402, 74.5884, 31.8442, 63.2943, 38.0383, 74.5884, 31.8442,
									63.2943, 31.8442, 63.2943, 26.9802, 66.1179, 26.9802, 66.1179};
//	static double AutoShapes24[] = {};
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
		case 18:
			*n = 136;
			vals = AutoShapes18;
			break;
		case 19:
			*n = 80;
			vals = AutoShapes19;
			break;
		case 20:
			*n = 80;
			vals = AutoShapes20;
			break;
		case 21:
			*n = 192;
			vals = AutoShapes21;
			break;
		case 22:
			*n = 272;
			vals = AutoShapes22;
			break;
		case 23:
			*n = 304;
			vals = AutoShapes23;
			break;
/*		case 24:
			*n = 80;
			vals = AutoShapes24;
			break; */
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
	if ((pixmapSize == 16) || (pixmapSize == 22))
	{
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
	}
//	QPixmap Ico(22, 22);
	QImage Ico(32, 32, QImage::Format_ARGB32);
	Ico.fill(0);
	int count = 0;
	double* AutoShapes;
	FPointArray Path;
	Path.resize(0);
	AutoShapes = getShapeData(nr, &count);
	for (int a = 0; a < count-3; a += 4)
	{
		if (AutoShapes[a] < 0)
		{
			Path.setMarker();
			continue;
		}
		double x1 = 28 * AutoShapes[a] / 100.0;
		double y1 = 28 * AutoShapes[a+1] / 100.0;
		double x2 = 28 * AutoShapes[a+2] / 100.0;
		double y2 = 28 * AutoShapes[a+3] / 100.0;
		Path.addPoint(x1, y1);
		Path.addPoint(x2, y2);
	}
	ScPainter *painter = new ScPainter(&Ico, 32, 32);
	painter->setBrush(qRgb(238, 238, 236));
	painter->setPen(qRgb(137, 139, 134));
	painter->setFillMode(1);
	painter->translate(2.0, 2.0);
	painter->setupPolygon(&Path);
	painter->setLineWidth(2.0);
	painter->drawPolygon();
	painter->drawPolyLine();
	painter->end();
	delete painter;
	return QPixmap::fromImage(Ico.scaled(32, 32));
}
