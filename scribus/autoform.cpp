/****************************************************************************
** Form implementation generated from reading ui file 'auto.ui'
**
** Created: Son Dez 7 12:53:32 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "autoform.h"
#include "autoform.moc"
extern QPixmap loadIcon(QString nam);

Autoforms::Autoforms( QWidget* parent ) : QToolButton( parent, "auto" )
{
	buttonGroup1 = new QButtonGroup();
	buttonGroup1->setExclusive( true );
	buttonGroup1->setColumnLayout(0, Qt::Vertical );
	buttonGroup1->layout()->setSpacing( 0 );
	buttonGroup1->layout()->setMargin( 0 );
	buttonGroup1Layout = new QGridLayout( buttonGroup1->layout() );
	buttonGroup1Layout->setAlignment( Qt::AlignTop );

	toolButton1 = new QToolButton( buttonGroup1, "toolButton1" );
	toolButton1->setToggleButton( true );
	toolButton1->setPixmap(loadIcon("rect.png"));
	buttonGroup1Layout->addWidget( toolButton1, 0, 0 );
	toolButton2 = new QToolButton( buttonGroup1, "toolButton2" );
	toolButton2->setToggleButton( true );
	toolButton2->setPixmap(loadIcon("oval.png"));
	buttonGroup1Layout->addWidget( toolButton2, 0, 1 );
	toolButton3 = new QToolButton( buttonGroup1, "toolButton3" );
	toolButton3->setToggleButton( true );
	toolButton3->setPixmap(loadIcon("triangle2.png"));
	buttonGroup1Layout->addWidget( toolButton3, 0, 2 );
	toolButton4 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton4->setToggleButton( true );
	toolButton4->setPixmap(loadIcon("cross.png"));
	buttonGroup1Layout->addWidget( toolButton4, 0, 3 );

	toolButton5 = new QToolButton( buttonGroup1, "toolButton3" );
	toolButton5->setToggleButton( true );
	toolButton5->setPixmap(loadIcon("arrowl.png"));
	buttonGroup1Layout->addWidget( toolButton5, 1, 0 );
	toolButton6 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton6->setToggleButton( true );
	toolButton6->setPixmap(loadIcon("arrowr.png"));
	buttonGroup1Layout->addWidget( toolButton6, 1, 1 );
	toolButton7 = new QToolButton( buttonGroup1, "toolButton3" );
	toolButton7->setToggleButton( true );
	toolButton7->setPixmap(loadIcon("arrowu.png"));
	buttonGroup1Layout->addWidget( toolButton7, 1, 2 );
	toolButton8 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton8->setToggleButton( true );
	toolButton8->setPixmap(loadIcon("arrowd.png"));
	buttonGroup1Layout->addWidget( toolButton8, 1, 3 );

	toolButton9 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton9->setToggleButton( true );
	toolButton9->setPixmap(loadIcon("halfovall.png"));
	buttonGroup1Layout->addWidget( toolButton9, 2, 0 );
	toolButton10 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton10->setToggleButton( true );
	toolButton10->setPixmap(loadIcon("halfovalr.png"));
	buttonGroup1Layout->addWidget( toolButton10, 2, 1 );
	toolButton11 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton11->setToggleButton( true );
	toolButton11->setPixmap(loadIcon("halfovalu.png"));
	buttonGroup1Layout->addWidget( toolButton11, 2, 2 );
	toolButton12 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton12->setToggleButton( true );
	toolButton12->setPixmap(loadIcon("halfovald.png"));
	buttonGroup1Layout->addWidget( toolButton12, 2, 3 );

	toolButton13 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton13->setToggleButton( true );
	toolButton13->setPixmap(loadIcon("tril.png"));
	buttonGroup1Layout->addWidget( toolButton13, 3, 0 );
	toolButton14 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton14->setToggleButton( true );
	toolButton14->setPixmap(loadIcon("trir.png"));
	buttonGroup1Layout->addWidget( toolButton14, 3, 1 );
	toolButton15 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton15->setToggleButton( true );
	toolButton15->setPixmap(loadIcon("trit.png"));
	buttonGroup1Layout->addWidget( toolButton15, 3, 2 );
	toolButton16 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton16->setToggleButton( true );
	toolButton16->setPixmap(loadIcon("trid.png"));
	buttonGroup1Layout->addWidget( toolButton16, 3, 3 );

	toolButton17 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton17->setToggleButton( true );
	toolButton17->setPixmap(loadIcon("frame.png"));
	buttonGroup1Layout->addWidget( toolButton17, 4, 0 );
	toolButton18 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton18->setToggleButton( true );
	toolButton18->setPixmap(loadIcon("torus.png"));
	buttonGroup1Layout->addWidget( toolButton18, 4, 1 );

	Men = new QPopupMenu();
	Men->insertItem(buttonGroup1);
	setPopup(Men);
	setPopupDelay(10);
	setPixmap(loadIcon("rect.png"));
	connect(buttonGroup1, SIGNAL(clicked(int)), this, SLOT(selForm(int)));
}

void Autoforms::selForm(int a)
{
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 
									100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes1[] = {100.0, 50.0, 100.0, 77.615235, 50.0, 100.0, 77.615235, 100.0, 50.0, 100.0,
									22.385765, 100.0, 0.0, 50.0, 0.0, 77.615235, 0.0, 50.0, 0.0, 22.385765, 
									50.0, 0.0, 22.385765, 0.0,
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
									0.0, 75.0, 0.0, 75.0, 0.0, 25.0, 0.0, 25.0};
	static double AutoShapes6[] = {0.0, 50.0, 0.0, 50.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0,
									100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 75.0, 50.0, 75.0, 50.0,
									75.0, 50.0, 75.0, 50.0, 75.0, 100.0, 75.0, 100.0, 75.0, 100.0, 75.0, 100.0,
									25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 25.0, 50.0, 25.0, 50.0,
									25.0, 50.0, 25.0, 50.0,	0.0, 50.0, 0.0, 50.0};
	static double AutoShapes7[] = {0.0, 50.0, 0.0, 50.0, 25.0, 50.0, 25.0, 50.0, 25.0, 50.0, 25.0, 50.0,
									25.0, 0.0, 25.0, 0.0, 25.0, 0.0, 25.0, 0.0, 75.0, 0.0, 75.0, 0.0,
									75.0, 0.0, 75.0, 0.0, 75.0, 50.0, 75.0, 50.0, 75.0, 50.0, 75.0, 50.0,
									100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 50.0, 100.0, 50.0, 100.0,
									50.0, 100.0, 50.0, 100.0, 0.0, 50.0, 0.0, 50.0};
	static double AutoShapes8[] = {0.0, 50.0, 0.0, 22.385765, 100.0, 0.0, 44.77716, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0,	100.0, 100.0, 44.77716, 100.0, 0.0, 50.0, 0.0,
									77.615235};
	static double AutoShapes9[] = {100.0, 50.0, 100.0, 77.615235, 0.0, 100.0, 55.22284, 100.0, 0.0, 100.0, 0.0,
									100.0,
									0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 55.22281, 0.0, 100.0, 50.0, 100.0, 22.385765};
	static double AutoShapes10[] = {100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 44.7715,
									50.0, 0.0, 22.3858, 0.0, 50.0, 0.0, 77.6152, 0.0, 100.0, 100.0, 100.0, 
									44.7715};
	static double AutoShapes11[] = {100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 55.2285, 50.0,
									100.0,
									22.3858, 100.0, 50.0, 100.0, 77.6152, 100.0, 100.0, 0.0, 100.0, 55.2285};
	static double AutoShapes12[] = {0.0, 50.0, 0.0, 50.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 50.0, 0.0, 50.0};
	static double AutoShapes13[] = {0.0, 0.0, 0.0, 0.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0,
									0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes14[] = {0.0, 100.0, 0.0, 100.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0, 50.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0,
									100.0};
	static double AutoShapes15[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 50.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	static double AutoShapes16[] = {25.0, 0.0, 25.0, 0.0, 75.0, 0.0, 75.0, 0.0, 75.0, 0.0, 75.0, 13.8071,
									100.0, 25.0, 86.1929, 25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 75.0, 100.0, 75.0,
									100.0, 75.0, 86.1929, 75.0, 75.0, 100.0, 75.0, 86.1929, 75.0, 100.0, 75.0,
									100.0,
									25.0, 100.0, 25.0, 100.0, 25.0, 100.0, 25.0, 86.1929, 0.0, 75.0, 13.8071, 75.0,
									0.0, 75.0, 0.0, 75.0, 0.0, 25.0, 0.0, 25.0, 0.0, 25.0, 13.8071, 25.0,
									25.0, 0.0, 25.0, 13.8071};
	static double AutoShapes17[] = {100.0, 50.0, 100.0, 77.6152, 50.0, 100.0, 77.6152, 100.0, 50.0, 100.0,
									22.3858, 100.0,
									0.0, 50.0, 0.0, 77.6152, 0.0,  50.0, 0.0, 22.3858, 50.0, 0.0, 22.3858, 0.0,
									50.0, 0.0, 77.6152, 0.0, 100.0, 50.0, 100.0, 22.3858, -1.0, -1.0, -1.0, -1.0,
									75.0, 50.0, 75.0, 63.8076, 50.0, 75.0, 63.8076, 75.0, 50.0, 75.0, 36.1929,
									75.0,
									25.0, 50.0, 25.0, 63.8076, 25.0, 50.0, 25.0, 36.1929, 50.0, 25.0, 36.1929,
									25.0,
									50.0, 25.0, 63.8076, 25.0, 75.0, 50.0, 75.0, 36.1929};
	Men->activateItemAt(0);
	const QPixmap *px = buttonGroup1->find(a)->pixmap();
	setPixmap(*px);
	switch (a)
	{
		case 0:
			emit FormSel(a, 32, AutoShapes0);
			break;
		case 1:
			emit FormSel(a, 32, AutoShapes1);
			break;
		case 2:
			emit FormSel(a, 24, AutoShapes2);
			break;
		case 3:
			emit FormSel(a, 96, AutoShapes3);
			break;
		case 4:
			emit FormSel(a, 56, AutoShapes4);
			break;
		case 5:
			emit FormSel(a, 56, AutoShapes5);
			break;
		case 6:
			emit FormSel(a, 56, AutoShapes6);
			break;
		case 7:
			emit FormSel(a, 56, AutoShapes7);
			break;
		case 8:
			emit FormSel(a, 24, AutoShapes8);
			break;
		case 9:
			emit FormSel(a, 24, AutoShapes9);
			break;
		case 10:
			emit FormSel(a, 24, AutoShapes10);
			break;
		case 11:
			emit FormSel(a, 24, AutoShapes11);
			break;
		case 12:
			emit FormSel(a, 24, AutoShapes12);
			break;
		case 13:
			emit FormSel(a, 24, AutoShapes13);
			break;
		case 14:
			emit FormSel(a, 24, AutoShapes14);
			break;
		case 15:
			emit FormSel(a, 24, AutoShapes15);
			break;
		case 16:
			emit FormSel(a, 64, AutoShapes16);
			break;
		case 17:
			emit FormSel(a, 68, AutoShapes17);
			break;
	}
}
