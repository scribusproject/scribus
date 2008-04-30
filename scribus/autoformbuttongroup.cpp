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
// #include <QDebug>

#include "autoformbuttongroup.h"
#include "fpointarray.h"
#include "scpainter.h"
#include "util_icon.h"

AutoformButtonGroup::AutoformButtonGroup( QWidget* parent ) : QMenu( parent )
{
	signalMapper = new QSignalMapper(this);
	menu1 = new QMenu( tr("Default Shapes"));
	addShape(menu1, 0);
	addShape(menu1, 1);
	addShape(menu1, 2);
	addShape(menu1, 3);
	addShape(menu1, 8);
	addShape(menu1, 9);
	addShape(menu1, 10);
	addShape(menu1, 11);
	addShape(menu1, 12);
	addShape(menu1, 13);
	addShape(menu1, 14);
	addShape(menu1, 15);
	addShape(menu1, 16);
	addShape(menu1, 17);
	menu2 = new QMenu( tr("Arrows"));
	addShape(menu2, 4);
	addShape(menu2, 5);
	addShape(menu2, 6);
	addShape(menu2, 7);
	addShape(menu2, 20);
	addShape(menu2, 21);
	addShape(menu2, 22);
	menu3 = new QMenu( tr("Flow Chart"));
	addShape(menu3, 24);
	addShape(menu3, 25);
	addShape(menu3, 26);
	addShape(menu3, 27);
	addShape(menu3, 28);
	addShape(menu3, 29);
	addShape(menu3, 30);
	addShape(menu3, 31);
	menu4 = new QMenu( tr("Jigsaw"));
	addShape(menu4, 32);
	addShape(menu4, 33);
	addShape(menu4, 34);
	addShape(menu4, 35);
	addShape(menu4, 36);
	addShape(menu4, 37);
	addShape(menu4, 38);
	addShape(menu4, 39);
	addShape(menu4, 40);
	addShape(menu4, 41);
	addShape(menu4, 42);
	addShape(menu4, 43);
	menu5 = new QMenu( tr("Specials"));
	addShape(menu5, 18);
	addShape(menu5, 19);
	addShape(menu5, 23);
	addShape(menu5, 44);
	addMenu(menu1);
	addMenu(menu2);
	addMenu(menu3);
	addMenu(menu4);
	addMenu(menu5);
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(selForm(int)));
}

void AutoformButtonGroup::addShape(QMenu* menu, int shapenum)
{
	QAction *action = menu->addAction(QIcon(getIconPixmap(shapenum, 16)), "");
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	signalMapper->setMapping(action, shapenum);
}
/*
AutoformButtonGroup::AutoformButtonGroup( QWidget* parent ) : QFrame( parent )
{
	QSizePolicy policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	buttonGroup1Layout = new QGridLayout( this );
	buttonGroup1Layout->setSpacing( 0 );
	buttonGroup1Layout->setMargin( 0 );
	int rowNum = 0;
	toolButton1 = new QToolButton( this );
	toolButton1->setCheckable( true );
	toolButton1->setIcon(QIcon(getIconPixmap(0,16)));
	toolButton1->setIconSize(QSize(16, 16));
	toolButton1->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton1, rowNum, 0 );
	toolButton2 = new QToolButton( this );
	toolButton2->setCheckable( true );
	toolButton2->setIcon(QIcon(getIconPixmap(1,16)));
	toolButton2->setIconSize(QSize(16, 16));
	toolButton2->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton2, rowNum, 1 );
	toolButton3 = new QToolButton( this );
	toolButton3->setCheckable( true );
	toolButton3->setIcon(QIcon(getIconPixmap(2,16)));
	toolButton3->setIconSize(QSize(16, 16));
	toolButton3->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton3, rowNum, 2 );
	toolButton4 = new QToolButton( this );
	toolButton4->setCheckable( true );
	toolButton4->setIcon(QIcon(getIconPixmap(3,16)));
	toolButton4->setIconSize(QSize(16, 16));
	toolButton4->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton4, rowNum, 3 );
	rowNum++;

	toolButton9 = new QToolButton( this );
	toolButton9->setCheckable( true );
	toolButton9->setIcon(QIcon(getIconPixmap(8,16)));
	toolButton9->setIconSize(QSize(16, 16));
	toolButton9->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton9, rowNum, 0 );
	toolButton10 = new QToolButton( this );
	toolButton10->setCheckable( true );
	toolButton10->setIcon(QIcon(getIconPixmap(9,16)));
	toolButton10->setIconSize(QSize(16, 16));
	toolButton10->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton10, rowNum, 1 );
	toolButton11 = new QToolButton( this );
	toolButton11->setCheckable( true );
	toolButton11->setIcon(QIcon(getIconPixmap(10,16)));
	toolButton11->setIconSize(QSize(16, 16));
	toolButton11->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton11, rowNum, 2 );
	toolButton12 = new QToolButton( this );
	toolButton12->setCheckable( true );
	toolButton12->setIcon(QIcon(getIconPixmap(11,16)));
	toolButton12->setIconSize(QSize(16, 16));
	toolButton12->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton12, rowNum, 3 );
	rowNum++;

	toolButton13 = new QToolButton( this );
	toolButton13->setCheckable( true );
	toolButton13->setIcon(QIcon(getIconPixmap(12,16)));
	toolButton13->setIconSize(QSize(16, 16));
	toolButton13->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton13, rowNum, 0 );
	toolButton14 = new QToolButton( this );
	toolButton14->setCheckable( true );
	toolButton14->setIcon(QIcon(getIconPixmap(13,16)));
	toolButton14->setIconSize(QSize(16, 16));
	toolButton14->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton14, rowNum, 1 );
	toolButton15 = new QToolButton( this );
	toolButton15->setCheckable( true );
	toolButton15->setIcon(QIcon(getIconPixmap(14,16)));
	toolButton15->setIconSize(QSize(16, 16));
	toolButton15->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton15, rowNum, 2 );
	toolButton16 = new QToolButton( this );
	toolButton16->setCheckable( true );
	toolButton16->setIcon(QIcon(getIconPixmap(15,16)));
	toolButton16->setIconSize(QSize(16, 16));
	toolButton16->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton16, rowNum, 3 );
	rowNum++;

	toolButton17 = new QToolButton( this );
	toolButton17->setCheckable( true );
	toolButton17->setIcon(QIcon(getIconPixmap(16,16)));
	toolButton17->setIconSize(QSize(16, 16));
	toolButton17->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton17, rowNum, 0 );
	toolButton18 = new QToolButton( this );
	toolButton18->setCheckable( true );
	toolButton18->setIcon(QIcon(getIconPixmap(17,16)));
	toolButton18->setIconSize(QSize(16, 16));
	toolButton18->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton18, rowNum, 1 );
	rowNum++;

	checkDoubleArrows = new QCheckBox( tr("Arrows"), this);
	checkDoubleArrows->setChecked(true);
	buttonGroup1Layout->addWidget(checkDoubleArrows, rowNum, 0, 1, 4);
	rowNum++;

	toolButton5 = new QToolButton( this );
	toolButton5->setCheckable( true );
	toolButton5->setIcon(QIcon(getIconPixmap(4,16)));
	toolButton5->setIconSize(QSize(16, 16));
	toolButton5->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton5, rowNum, 0 );
	toolButton6 = new QToolButton( this );
	toolButton6->setCheckable( true );
	toolButton6->setIcon(QIcon(getIconPixmap(5,16)));
	toolButton6->setIconSize(QSize(16, 16));
	toolButton6->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton6, rowNum, 1 );
	toolButton7 = new QToolButton( this );
	toolButton7->setCheckable( true );
	toolButton7->setIcon(QIcon(getIconPixmap(6,16)));
	toolButton7->setIconSize(QSize(16, 16));
	toolButton7->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton7, rowNum, 2 );
	toolButton8 = new QToolButton( this );
	toolButton8->setCheckable( true );
	toolButton8->setIcon(QIcon(getIconPixmap(7,16)));
	toolButton8->setIconSize(QSize(16, 16));
	toolButton8->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton8, rowNum, 3 );
	rowNum++;
	toolButton21 = new QToolButton( this );
	toolButton21->setCheckable( true );
	toolButton21->setIcon(QIcon(getIconPixmap(20,16)));
	toolButton21->setIconSize(QSize(16, 16));
	toolButton21->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton21, rowNum, 0 );
	toolButton22 = new QToolButton( this );
	toolButton22->setCheckable( true );
	toolButton22->setIcon(QIcon(getIconPixmap(21,16)));
	toolButton22->setIconSize(QSize(16, 16));
	toolButton22->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton22, rowNum, 1 );
	toolButton23 = new QToolButton( this );
	toolButton23->setCheckable( true );
	toolButton23->setIcon(QIcon(getIconPixmap(22,16)));
	toolButton23->setIconSize(QSize(16, 16));
	toolButton23->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton23, rowNum, 2 );
	rowNum++;
	
	checkFlowChart = new QCheckBox( tr("Flow Chart"), this);
	checkFlowChart->setChecked(true);
	buttonGroup1Layout->addWidget(checkFlowChart, rowNum, 0, 1, 4);
	rowNum++;

	toolButton25 = new QToolButton( this );
	toolButton25->setCheckable( true );
	toolButton25->setIcon(QIcon(getIconPixmap(24,16)));
	toolButton25->setIconSize(QSize(16, 16));
	toolButton25->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton25, rowNum, 0 );
	toolButton26 = new QToolButton( this );
	toolButton26->setCheckable( true );
	toolButton26->setIcon(QIcon(getIconPixmap(25,16)));
	toolButton26->setIconSize(QSize(16, 16));
	toolButton26->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton26, rowNum, 1 );
	toolButton27 = new QToolButton( this );
	toolButton27->setCheckable( true );
	toolButton27->setIcon(QIcon(getIconPixmap(26,16)));
	toolButton27->setIconSize(QSize(16, 16));
	toolButton27->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton27, rowNum, 2 );
	toolButton28 = new QToolButton( this );
	toolButton28->setCheckable( true );
	toolButton28->setIcon(QIcon(getIconPixmap(27,16)));
	toolButton28->setIconSize(QSize(16, 16));
	toolButton28->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton28, rowNum, 3 );
	rowNum++;

	toolButton29 = new QToolButton( this );
	toolButton29->setCheckable( true );
	toolButton29->setIcon(QIcon(getIconPixmap(28,16)));
	toolButton29->setIconSize(QSize(16, 16));
	toolButton29->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton29, rowNum, 0 );
	toolButton30 = new QToolButton( this );
	toolButton30->setCheckable( true );
	toolButton30->setIcon(QIcon(getIconPixmap(29,16)));
	toolButton30->setIconSize(QSize(16, 16));
	toolButton30->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton30, rowNum, 1 );
	toolButton31 = new QToolButton( this );
	toolButton31->setCheckable( true );
	toolButton31->setIcon(QIcon(getIconPixmap(30,16)));
	toolButton31->setIconSize(QSize(16, 16));
	toolButton31->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton31, rowNum, 2 );
	toolButton32 = new QToolButton( this );
	toolButton32->setCheckable( true );
	toolButton32->setIcon(QIcon(getIconPixmap(31,16)));
	toolButton32->setIconSize(QSize(16, 16));
	toolButton32->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton32, rowNum, 3 );
	rowNum++;

	checkJigSaw = new QCheckBox( tr("Jigsaw"), this);
	checkJigSaw->setChecked(true);
	buttonGroup1Layout->addWidget(checkJigSaw, rowNum, 0, 1, 4);
	rowNum++;

	toolButton33 = new QToolButton( this );
	toolButton33->setCheckable( true );
	toolButton33->setIcon(QIcon(getIconPixmap(32,16)));
	toolButton33->setIconSize(QSize(16, 16));
	toolButton33->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton33, rowNum, 0 );
	toolButton34 = new QToolButton( this );
	toolButton34->setCheckable( true );
	toolButton34->setIcon(QIcon(getIconPixmap(33,16)));
	toolButton34->setIconSize(QSize(16, 16));
	toolButton34->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton34, rowNum, 1 );
	toolButton35 = new QToolButton( this );
	toolButton35->setCheckable( true );
	toolButton35->setIcon(QIcon(getIconPixmap(34,16)));
	toolButton35->setIconSize(QSize(16, 16));
	toolButton35->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton35, rowNum, 2 );
	toolButton36 = new QToolButton( this );
	toolButton36->setCheckable( true );
	toolButton36->setIcon(QIcon(getIconPixmap(35,16)));
	toolButton36->setIconSize(QSize(16, 16));
	toolButton36->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton36, rowNum, 3 );
	rowNum++;
	toolButton37 = new QToolButton( this );
	toolButton37->setCheckable( true );
	toolButton37->setIcon(QIcon(getIconPixmap(36,16)));
	toolButton37->setIconSize(QSize(16, 16));
	toolButton37->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton37, rowNum, 0 );
	toolButton38 = new QToolButton( this );
	toolButton38->setCheckable( true );
	toolButton38->setIcon(QIcon(getIconPixmap(37,16)));
	toolButton38->setIconSize(QSize(16, 16));
	toolButton38->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton38, rowNum, 1 );
	toolButton39 = new QToolButton( this );
	toolButton39->setCheckable( true );
	toolButton39->setIcon(QIcon(getIconPixmap(38,16)));
	toolButton39->setIconSize(QSize(16, 16));
	toolButton39->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton39, rowNum, 2 );
	toolButton40 = new QToolButton( this );
	toolButton40->setCheckable( true );
	toolButton40->setIcon(QIcon(getIconPixmap(39,16)));
	toolButton40->setIconSize(QSize(16, 16));
	toolButton40->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton40, rowNum, 3 );
	rowNum++;
	toolButton41 = new QToolButton( this );
	toolButton41->setCheckable( true );
	toolButton41->setIcon(QIcon(getIconPixmap(40,16)));
	toolButton41->setIconSize(QSize(16, 16));
	toolButton41->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton41, rowNum, 0 );
	toolButton42 = new QToolButton( this );
	toolButton42->setCheckable( true );
	toolButton42->setIcon(QIcon(getIconPixmap(41,16)));
	toolButton42->setIconSize(QSize(16, 16));
	toolButton42->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton42, rowNum, 1 );
	toolButton43 = new QToolButton( this );
	toolButton43->setCheckable( true );
	toolButton43->setIcon(QIcon(getIconPixmap(42,16)));
	toolButton43->setIconSize(QSize(16, 16));
	toolButton43->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton43, rowNum, 2 );
	toolButton44 = new QToolButton( this );
	toolButton44->setCheckable( true );
	toolButton44->setIcon(QIcon(getIconPixmap(43,16)));
	toolButton44->setIconSize(QSize(16, 16));
	toolButton44->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton44, rowNum, 3 );
	rowNum++;

	checkSpecials = new QCheckBox( tr("Specials"), this);
	checkSpecials->setChecked(true);
	buttonGroup1Layout->addWidget(checkSpecials, rowNum, 0, 1, 4);
	rowNum++;

	toolButton19 = new QToolButton( this );
	toolButton19->setCheckable( true );
	toolButton19->setIcon(QIcon(getIconPixmap(18,16)));
	toolButton19->setIconSize(QSize(16, 16));
	toolButton19->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton19, rowNum, 0 );
	toolButton20 = new QToolButton( this );
	toolButton20->setCheckable( true );
	toolButton20->setIcon(QIcon(getIconPixmap(19,16)));
	toolButton20->setIconSize(QSize(16, 16));
	toolButton20->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton20, rowNum, 1 );
	toolButton24 = new QToolButton( this );
	toolButton24->setCheckable( true );
	toolButton24->setIcon(QIcon(getIconPixmap(23,16)));
	toolButton24->setIconSize(QSize(16, 16));
	toolButton24->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton24, rowNum, 2 );
	toolButton45 = new QToolButton( this );
	toolButton45->setCheckable( true );
	toolButton45->setIcon(QIcon(getIconPixmap(44,16)));
	toolButton45->setIconSize(QSize(16, 16));
	toolButton45->setSizePolicy(policy);
	buttonGroup1Layout->addWidget( toolButton45, rowNum, 3 );

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
	buttonGroup->addButton(toolButton25, 24);
	buttonGroup->addButton(toolButton26, 25);
	buttonGroup->addButton(toolButton27, 26);
	buttonGroup->addButton(toolButton28, 27);
	buttonGroup->addButton(toolButton29, 28);
	buttonGroup->addButton(toolButton30, 29);
	buttonGroup->addButton(toolButton31, 30);
	buttonGroup->addButton(toolButton32, 31);
	buttonGroup->addButton(toolButton33, 32);
	buttonGroup->addButton(toolButton34, 33);
	buttonGroup->addButton(toolButton35, 34);
	buttonGroup->addButton(toolButton36, 35);
	buttonGroup->addButton(toolButton37, 36);
	buttonGroup->addButton(toolButton38, 37);
	buttonGroup->addButton(toolButton39, 38);
	buttonGroup->addButton(toolButton40, 39);
	buttonGroup->addButton(toolButton41, 40);
	buttonGroup->addButton(toolButton42, 41);
	buttonGroup->addButton(toolButton43, 42);
	buttonGroup->addButton(toolButton44, 43);
	buttonGroup->addButton(toolButton45, 44);
	
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(selForm(int)));
	connect(checkDoubleArrows, SIGNAL(clicked(bool)), this, SLOT(toggleDoubleArrows(bool)));
	connect(checkFlowChart, SIGNAL(clicked(bool)), this, SLOT(toggleFlowChart(bool)));
	connect(checkSpecials, SIGNAL(clicked(bool)), this, SLOT(toggleSpecials(bool)));
	connect(checkJigSaw, SIGNAL(clicked(bool)), this, SLOT(toggleJigSaw(bool)));

}

void AutoformButtonGroup::toggleDoubleArrows(bool visible)
{
	int oldH = minimumSizeHint().height();
	QSizePolicy policy;
	if (visible)
		policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	else
		policy = QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	toolButton5->setVisible(visible);
	toolButton5->setSizePolicy(policy);
	toolButton6->setVisible(visible);
	toolButton6->setSizePolicy(policy);
	toolButton7->setVisible(visible);
	toolButton7->setSizePolicy(policy);
	toolButton8->setVisible(visible);
	toolButton8->setSizePolicy(policy);
	toolButton21->setVisible(visible);
	toolButton21->setSizePolicy(policy);
	toolButton22->setVisible(visible);
	toolButton22->setSizePolicy(policy);
	toolButton23->setVisible(visible);
	toolButton23->setSizePolicy(policy);
	buttonGroup1Layout->activate();
	updateGeometry();
	int newH = minimumSizeHint().height();
	resize(minimumSizeHint());
	QSize pS = parentWidget()->size();
	int pH = pS.height();
	pS.setHeight(pH - (oldH - newH));
	parentWidget()->updateGeometry();
	parentWidget()->resize(pS);
}

void AutoformButtonGroup::toggleFlowChart(bool visible)
{
	int oldH = minimumSizeHint().height();
	QSizePolicy policy;
	if (visible)
		policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	else
		policy = QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	toolButton25->setVisible(visible);
	toolButton25->setSizePolicy(policy);
	toolButton26->setVisible(visible);
	toolButton26->setSizePolicy(policy);
	toolButton27->setVisible(visible);
	toolButton27->setSizePolicy(policy);
	toolButton28->setVisible(visible);
	toolButton28->setSizePolicy(policy);
	toolButton29->setVisible(visible);
	toolButton29->setSizePolicy(policy);
	toolButton30->setVisible(visible);
	toolButton30->setSizePolicy(policy);
	toolButton31->setVisible(visible);
	toolButton31->setSizePolicy(policy);
	toolButton32->setVisible(visible);
	toolButton32->setSizePolicy(policy);
	buttonGroup1Layout->activate();
	updateGeometry();
	int newH = minimumSizeHint().height();
	resize(minimumSizeHint());
	QSize pS = parentWidget()->size();
	int pH = pS.height();
	pS.setHeight(pH - (oldH - newH));
	parentWidget()->updateGeometry();
	parentWidget()->resize(pS);
}

void AutoformButtonGroup::toggleJigSaw(bool visible)
{
	int oldH = minimumSizeHint().height();
	QSizePolicy policy;
	if (visible)
		policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	else
		policy = QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	toolButton33->setVisible(visible);
	toolButton33->setSizePolicy(policy);
	toolButton34->setVisible(visible);
	toolButton34->setSizePolicy(policy);
	toolButton35->setVisible(visible);
	toolButton35->setSizePolicy(policy);
	toolButton36->setVisible(visible);
	toolButton36->setSizePolicy(policy);
	toolButton37->setVisible(visible);
	toolButton37->setSizePolicy(policy);
	toolButton38->setVisible(visible);
	toolButton38->setSizePolicy(policy);
	toolButton39->setVisible(visible);
	toolButton39->setSizePolicy(policy);
	toolButton40->setVisible(visible);
	toolButton40->setSizePolicy(policy);
	toolButton41->setVisible(visible);
	toolButton41->setSizePolicy(policy);
	toolButton42->setVisible(visible);
	toolButton42->setSizePolicy(policy);
	toolButton43->setVisible(visible);
	toolButton43->setSizePolicy(policy);
	toolButton44->setVisible(visible);
	toolButton44->setSizePolicy(policy);
	buttonGroup1Layout->activate();
	updateGeometry();
	int newH = minimumSizeHint().height();
	resize(minimumSizeHint());
	QSize pS = parentWidget()->size();
	int pH = pS.height();
	pS.setHeight(pH - (oldH - newH));
	parentWidget()->updateGeometry();
	parentWidget()->resize(pS);
}

void AutoformButtonGroup::toggleSpecials(bool visible)
{
	int oldH = minimumSizeHint().height();
	QSizePolicy policy;
	if (visible)
		policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	else
		policy = QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	toolButton24->setVisible(visible);
	toolButton24->setSizePolicy(policy);
	toolButton19->setVisible(visible);
	toolButton19->setSizePolicy(policy);
	toolButton20->setVisible(visible);
	toolButton20->setSizePolicy(policy);
	toolButton45->setVisible(visible);
	toolButton45->setSizePolicy(policy);
	buttonGroup1Layout->activate();
	updateGeometry();
	int newH = minimumSizeHint().height();
	resize(minimumSizeHint());
	QSize pS = parentWidget()->size();
	int pH = pS.height();
	pS.setHeight(pH - (oldH - newH));
	parentWidget()->updateGeometry();
	parentWidget()->resize(pS);
}
*/
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
	static double AutoShapes19[] = {50.5178, 88.7331, 50.5178, 88.7331, 33.2052, 100, 44.7028, 100, 33.2052, 100, 33.2052, 100, 17.7654, 93.5724, 24.0951, 100,
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
	static double AutoShapes22[] = {0, 50, 0, 50, 33.3333, 0, 33.3333, 0, 33.3333, 0, 33.3333, 0, 33.3333, 25, 33.3333, 25, 33.3333, 25, 33.3333, 25, 66.6667,
									25, 66.6667, 25, 66.6667, 25, 66.6667, 25, 66.6667, 0, 66.6667, 0, 66.6667, 0, 66.6667, 0, 100, 50, 100, 50, 100, 50, 100, 50,
									66.6667, 100, 66.6667, 100, 66.6667, 100, 66.6667, 100, 66.6667, 75, 66.6667, 75, 66.6667, 75, 66.6667, 75, 33.3333, 75, 33.3333,
									75, 33.3333, 75, 33.3333, 75, 33.3333, 100, 33.3333, 100, 33.3333, 100, 33.3333, 100, 0, 50, 0, 50};
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
	static double AutoShapes24[] = {0, 50, 4.81928, 25, 38.5542, 0, 19.2771, 0, 38.5542, 0, 38.5542, 0, 85.5422, 0, 85.5422, 0, 85.5422, 0, 100, 12.5, 85.5422,
									100, 100, 87.5, 85.5422, 100, 85.5422, 100, 38.5542, 100, 38.5542, 100, 38.5542, 100, 19.2771, 100, 0, 50, 4.81928, 75};
	static double AutoShapes25[] = {0, 16.8052, 0, 16.8052, 50, 16.8052, 21.5158, 33.7102, 50, 16.8052, 78.5196, 0, 100, 16.8052, 100, 16.8052, 100, 16.8052,
									100, 16.8052, 100, 83.0956, 100, 83.0956, 100, 83.0956, 100, 83.0956, 50, 83.0527, 78.5196, 66.2904, 50, 83.0527, 21.5158,
									99.9999, 0, 83.0956, 0, 83.0956, 0, 83.0956, 0, 83.0956, 0, 16.8052, 0, 16.8052};
	static double AutoShapes26[] = {52.381, 100, 52.381, 100, 19.0476, 100, 19.0476, 100, 19.0476, 100, 0, 66.6667, 19.0476, 0, 0, 33.3333, 19.0476, 0, 19.0476,
									0, 100, 0, 100, 0, 100, 0, 80.9524, 33.3333, 100, 100, 80.9524, 66.6667, 100, 100, 100, 100, 52.381, 100, 52.381, 100};
	static double AutoShapes27[] = {0, 50, 0, 50, 20, 0, 20, 0, 20, 0, 20, 0, 80, 0, 80, 0, 80, 0, 80, 0, 100, 50, 100, 50, 100, 50, 100, 50, 80, 100, 80, 100,
									80, 100, 80, 100, 20, 100, 20, 100, 20, 100, 20, 100, 0, 50, 0, 50};
	static double AutoShapes28[] = {0, 0, 10.9237, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 81.2792, 100, 81.2792, 100, 81.2792, 100, 81.2792, 50, 81.2792, 78.4724,
									62.5558, 50, 81.2792, 21.5276, 100, 0, 81.2792, 0, 81.2792, 0, 81.2792, 0, 81.2792, 0, 0, 0, 18.0791};
	static double AutoShapes29[] = {0, 100, 0, 100, 0, 50, 0, 50, 0, 50, 0, 50, 20, 0, 20, 0, 20, 0, 20, 0, 80, 0, 80, 0, 80, 0, 80, 0, 100, 50, 100, 50, 100, 50,
									100, 50, 100, 100, 100, 100, 100, 100, 100, 100, 0, 100, 0, 100};
	static double AutoShapes30[] = {0, 50, 0, 50, 20, 0, 20, 0, 20, 0, 20, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 100, 100, 100, 100, 100, 100, 100, 0, 100, 0,
									100, 0, 100, 0, 100, 0, 50, 0, 50};
	static double AutoShapes31[] = {50, 100, 50, 100, 25, 100, 25, 100, 25, 100, 16.6667, 100, 0, 50, 0, 66.6666, 0, 50, 0, 33.3334, 25, 0, 16.6667, 0, 25, 0, 25,
									0, 75, 0, 75, 0, 75, 0, 83.3333, 0, 100, 50, 100, 33.3334, 100, 50, 100, 66.6666, 75, 100, 83.3333, 100, 75, 100, 75, 100, 50,
									100, 50, 100};
	static double AutoShapes32[] = {20, 0, 35, 0, 40, 0, 30, 0, 40, 0, 50, 0, 50, 20, 25, 20, 50, 20, 75, 20, 60, 0, 50, 0, 60, 0, 70, 0, 80, 0, 75, 0, 80, 0, 80,
									5, 80, 20, 80, 10, 80, 20, 80, 30, 100, 30, 100, 5, 100, 30, 100, 55, 80, 40, 80, 30, 80, 40, 80, 50, 80, 60, 80, 55, 80, 60,
									75, 60, 60, 60, 70, 60, 60, 60, 50, 60, 50, 40, 75, 40, 50, 40, 25, 40, 40, 60, 50, 60, 40, 60, 30, 60, 20, 60, 25, 60, 20, 60,
									20, 55, 20, 40, 20, 50, 20, 40, 20, 30, 0, 30, 0, 55, 0, 30, 0, 5, 20, 20, 20, 30, 20, 20, 20, 10, 20, 0, 20, 5};
	static double AutoShapes33[] = {0, 0, 18.75, 0, 25, 0, 12.5, 0, 25, 0, 37.5, 0, 37.5, 25, 6.25, 25, 37.5, 25, 68.75, 25, 50, 0, 37.5, 0, 50, 0, 62.5, 0, 75,
									0, 68.75, 0, 75, 0, 75, 6.25, 75, 25, 75, 12.5, 75, 25, 75, 37.5, 100, 37.5, 100, 6.25, 100, 37.5, 100, 68.75, 75, 50, 75,
									37.5, 75, 50, 75, 62.5, 75, 75, 75, 68.75, 75, 75, 68.75, 75, 50, 75, 62.5, 75, 50, 75, 37.5, 75, 37.5, 100, 68.75, 100, 37.5,
									100, 6.25, 100, 25, 75, 37.5, 75, 25, 75, 12.5, 75, 0, 75, 6.25, 75, 0, 75, 0, 68.75, 0, 50, 0, 62.5, 0, 50, 0, 37.5, 25, 37.5,
									25, 68.75, 25, 37.5, 25, 6.25, 0, 25, 0, 37.5, 0, 25, 0, 12.5, 0, 0, 0, 6.25};
	static double AutoShapes34[] = {0, 0, 18.75, 0, 25, 0, 12.5, 0, 25, 0, 37.5, 0, 37.5, 25, 6.25, 25, 37.5, 25, 68.75, 25, 50, 0, 37.5, 0, 50, 0, 62.5, 0, 75,
									0, 68.75, 0, 75, 0, 75, 6.25, 75, 25, 75, 12.5, 75, 25, 75, 37.5, 100, 37.5, 100, 6.25, 100, 37.5, 100, 68.75, 75, 50, 75,
									37.5, 75, 50, 75, 62.5, 75, 75, 75, 68.75, 75, 75, 68.75, 75, 50, 75, 62.5, 75, 50, 75, 37.5, 75, 37.5, 50, 68.75, 50, 37.5,
									50, 6.25, 50, 25, 75, 37.5, 75, 25, 75, 12.5, 75, 0, 75, 6.25, 75, 0, 75, 0, 68.75, 0, 50, 0, 62.5, 0, 50, 0, 37.5, 25, 37.5,
									25, 68.75, 25, 37.5, 25, 6.25, 0, 25, 0, 37.5, 0, 25, 0, 12.5, 0, 0, 0, 6.25};
	static double AutoShapes35[] = {0, 0, 25, 0, 33.3333, 0, 16.6667, 0, 33.3333, 0, 50, 0, 50, 33.3333, 8.33333, 33.3333, 50, 33.3333, 91.6667, 33.3333, 66.6667,
									0, 50, 0, 66.6667, 0, 83.3333, 0, 100, 0, 91.6667, 0, 100, 0, 100, 8.33333, 100, 33.3333, 100, 16.6667, 100, 33.3333, 100, 50,
									66.6667, 50, 66.6667, 8.33333, 66.6667, 50, 66.6667, 91.6667, 100, 66.6667, 100, 50, 100, 66.6667, 100, 83.3333, 100, 100, 100,
									91.6667, 100, 100, 91.6667, 100, 66.6667, 100, 83.3333, 100, 66.6667, 100, 50, 100, 50, 66.6667, 91.6667, 66.6667, 50, 66.6667,
									8.33333, 66.6667, 33.3333, 100, 50, 100, 33.3333, 100, 16.6667, 100, 0, 100, 8.33333, 100, 0, 100, 0, 91.6667, 0, 66.6667, 0,
									83.3333, 0, 66.6667, 0, 50, 33.3333, 50, 33.3333, 91.6667, 33.3333, 50, 33.3333, 8.33333, 0, 33.3333, 0, 50, 0, 33.3333, 0,
									16.6667, 0, 0, 0, 8.33333};
	static double AutoShapes36[] = {20, 20, 35, 20, 40, 20, 30, 20, 40, 20, 50, 20, 50, 0, 25, 0, 50, 0, 75, 0, 60, 20, 50, 20, 60, 20, 70, 20, 80, 20, 75, 20, 80,
									20, 80, 25, 80, 40, 80, 30, 80, 40, 80, 50, 60, 50, 60, 25, 60, 50, 60, 75, 80, 60, 80, 50, 80, 60, 80, 70, 80, 80, 80, 75, 80,
									80, 75, 80, 60, 80, 70, 80, 60, 80, 50, 80, 50, 100, 75, 100, 50, 100, 25, 100, 40, 80, 50, 80, 40, 80, 30, 80, 20, 80, 25, 80,
									20, 80, 20, 75, 20, 60, 20, 70, 20, 60, 20, 50, 0, 50, 0, 75, 0, 50, 0, 25, 20, 40, 20, 50, 20, 40, 20, 30, 20, 20, 20, 25};
	static double AutoShapes37[] = {20, 20, 35, 20, 40, 20, 30, 20, 40, 20, 50, 20, 50, 0, 25, 0, 50, 0, 75, 0, 60, 20, 50, 20, 60, 20, 70, 20, 80, 20, 75, 20, 80,
									20, 80, 25, 80, 40, 80, 30, 80, 40, 80, 50, 100, 50, 100, 25, 100, 50, 100, 75, 80, 60, 80, 50, 80, 60, 80, 70, 80, 80, 80, 75,
									80, 80, 75, 80, 60, 80, 70, 80, 60, 80, 50, 80, 50, 100, 75, 100, 50, 100, 25, 100, 40, 80, 50, 80, 40, 80, 30, 80, 20, 80, 25,
									80, 20, 80, 20, 75, 20, 60, 20, 70, 20, 60, 20, 50, 0, 50, 0, 75, 0, 50, 0, 25, 20, 40, 20, 50, 20, 40, 20, 30, 20, 20, 20, 25};
	static double AutoShapes38[] = {0, 0, 18.75, 0, 75, 0, 68.75, 0, 75, 0, 75, 6.25, 75, 25, 75, 12.5, 75, 25, 75, 37.5, 100, 37.5, 100, 6.25, 100, 37.5, 100,
									68.75, 75, 50, 75, 37.5, 75, 50, 75, 62.5, 75, 75, 75, 68.75, 75, 75, 68.75, 75, 50, 75, 62.5, 75, 50, 75, 37.5, 75, 37.5, 50,
									68.75, 50, 37.5, 50, 6.25, 50, 25, 75, 37.5, 75, 25, 75, 12.5, 75, 0, 75, 6.25, 75, 0, 75, 0, 68.75, 0, 0, 0, 6.25};
	static double AutoShapes39[] = {0, 0, 18.75, 0, 75, 0, 68.75, 0, 75, 0, 75, 6.25, 75, 25, 75, 12.5, 75, 25, 75, 37.5, 100, 37.5, 100, 6.25, 100, 37.5, 100,
									68.75, 75, 50, 75, 37.5, 75, 50, 75, 62.5, 75, 75, 75, 68.75, 75, 75, 68.75, 75, 50, 75, 62.5, 75, 50, 75, 37.5, 75, 37.5, 50,
									68.75, 50, 37.5, 50, 6.25, 50, 25, 75, 37.5, 75, 25, 75, 12.5, 75, 0, 75, 6.25, 75, 0, 75, 0, 68.75, 0, 50, 0, 62.5, 0, 50, 0,
									37.5, 25, 37.5, 25, 68.75, 25, 37.5, 25, 6.25, 0, 25, 0, 37.5, 0, 25, 0, 12.5, 0, 0, 0, 6.25};
	static double AutoShapes40[] = {0, 0, 25, 0, 100, 0, 91.6667, 0, 100, 0, 100, 8.33333, 100, 33.3333, 100, 16.6667, 100, 33.3333, 100, 50, 66.6667, 50, 66.6667,
									8.33333, 66.6667, 50, 66.6667, 91.6667, 100, 66.6667, 100, 50, 100, 66.6667, 100, 83.3333, 100, 100, 100, 91.6667, 100, 100,
									91.6667, 100, 66.6667, 100, 83.3333, 100, 66.6667, 100, 50, 100, 50, 66.6667, 91.6667, 66.6667, 50, 66.6667, 8.33333, 66.6667,
									33.3333, 100, 50, 100, 33.3333, 100, 16.6667, 100, 0, 100, 8.33333, 100, 0, 100, 0, 91.6667, 0, 0, 0, 8.33333};
	static double AutoShapes41[] = {0, 0, 25, 0, 100, 0, 91.6667, 0, 100, 0, 100, 8.33333, 100, 33.3333, 100, 16.6667, 100, 33.3333, 100, 50, 66.6667, 50, 66.6667,
									8.33333, 66.6667, 50, 66.6667, 91.6667, 100, 66.6667, 100, 50, 100, 66.6667, 100, 83.3333, 100, 100, 100, 91.6667, 100, 100,
									91.6667, 100, 66.6667, 100, 83.3333, 100, 66.6667, 100, 50, 100, 50, 66.6667, 91.6667, 66.6667, 50, 66.6667, 8.33333, 66.6667,
									33.3333, 100, 50, 100, 33.3333, 100, 16.6667, 100, 0, 100, 8.33333, 100, 0, 100, 0, 91.6667, 0, 66.6667, 0, 83.3333, 0, 66.6667,
									0, 50, 33.3333, 50, 33.3333, 91.6667, 33.3333, 50, 33.3333, 8.33333, 0, 33.3333, 0, 50, 0, 33.3333, 0, 16.6667, 0, 0, 0, 8.33333};
	static double AutoShapes42[] = {0, 0, 18.75, 0, 75, 0, 68.75, 0, 75, 0, 75, 6.25, 75, 25, 75, 12.5, 75, 25, 75, 37.5, 100, 37.5, 100, 6.25, 100, 37.5, 100,
									68.75, 75, 50, 75, 37.5, 75, 50, 75, 62.5, 75, 75, 75, 68.75, 75, 75, 68.75, 75, 50, 75, 62.5, 75, 50, 75, 37.5, 75, 37.5, 100,
									68.75, 100, 37.5, 100, 6.25, 100, 25, 75, 37.5, 75, 25, 75, 12.5, 75, 0, 75, 6.25, 75, 0, 75, 0, 68.75, 0, 0, 0, 6.25};
	static double AutoShapes43[] = {20, 0, 35, 0, 80, 0, 75, 0, 80, 0, 80, 5, 80, 20, 80, 10, 80, 20, 80, 30, 100, 30, 100, 5, 100, 30, 100, 55, 80, 40, 80, 30,
									80, 40, 80, 50, 80, 60, 80, 55, 80, 60, 75, 60, 60, 60, 70, 60, 60, 60, 50, 60, 50, 40, 75, 40, 50, 40, 25, 40, 40, 60, 50,
									60, 40, 60, 30, 60, 20, 60, 25, 60, 20, 60, 20, 55, 20, 40, 20, 50, 20, 40, 20, 30, 0, 30, 0, 55, 0, 30, 0, 5, 20, 20, 20, 30,
									20, 20, 20, 10, 20, 0, 20, 5};
	static double AutoShapes44[] = {40.8712, 24.5328, 40.8712, 24.5328, 40.475, 24.8729, 40.8712, 24.8729, 40.475, 24.8729, 40.475, 24.8729, 40.1368, 24.8168,
									40.3629, 24.8729, 40.1368, 24.8168, 40.1368, 24.8168, 39.6266, 24.3917, 39.9106, 24.759, 39.6266, 24.3917, 39.6266, 24.3917,
									39.3444, 23.4601, 39.3444, 24.0245, 39.3444, 23.4601, 39.3444, 23.4601, 39.5145, 23.1201, 39.3444, 23.1201, 39.5145, 23.1201,
									39.5145, 23.1201, 40.3611, 23.5723, 39.8528, 23.1201, 40.3611, 23.5723, 40.3611, 23.5723, 40.8712, 24.5328, 40.8712, 24.0245,
									40.8712, 24.5328, 40.8712, 24.5328, 40.8712, 24.5328, 40.8712, 24.5328,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									35.1043, 21.255, 35.1043, 21.255, 33.4093, 18.7678, 34.7099, 18.7678, 33.4093, 18.7678, 33.4093, 18.7678, 32.901, 18.8799,
									33.3532, 18.7678, 32.901, 18.8799, 32.901, 18.8799, 33.1271, 19.6161, 33.1271, 19.4461, 33.1271, 19.6161, 33.1271, 19.6161,
									33.8055, 19.6722, 33.2953, 19.4461, 33.8055, 19.6722, 33.8055, 19.6722, 34.3699, 21.3672, 34.1998, 20.3506, 34.3699, 21.3672,
									34.3699, 21.3672, 35.1043, 21.255, 35.1043, 21.255, 35.1043, 21.255, 35.1043, 21.255, 35.1043, 21.255, 35.1043, 21.255,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									46.6942, 19.5583, 46.6942, 19.5583, 47.7108, 21.3672, 47.3146, 19.5583, 47.7108, 21.3672, 47.7108, 21.3672, 48.3891, 21.255,
									48.3891, 21.255, 48.3891, 21.255, 48.3891, 21.255, 48.2191, 20.3506, 48.277, 20.915, 48.2191, 20.3506, 48.2191, 20.3506,
									47.7669, 19.2761, 48.163, 19.7844, 47.7669, 19.2761, 47.7669, 19.2761, 46.7502, 18.7678, 47.3725, 18.7678, 46.7502, 18.7678,
									46.7502, 18.7678, 46.2419, 18.8799, 46.6942, 18.7678, 46.2419, 18.8799, 46.2419, 18.8799, 46.4102, 19.4461, 46.3541, 19.2761,
									46.4102, 19.4461, 46.4102, 19.4461, 46.468, 19.6161, 46.468, 19.6161, 46.468, 19.6161, 46.468, 19.6161, 46.6942, 19.5583,
									46.468, 19.5583, 46.6942, 19.5583, 46.6942, 19.5583, 46.6942, 19.5583, 46.6942, 19.5583,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									36.1227, 24.5328, 36.1227, 24.5328, 36.6871, 23.7134, 36.1227, 24.0245, 36.6871, 23.7134, 36.6871, 23.7134, 37.7055, 23.4023,
									37.2533, 23.4023, 37.7055, 23.4023, 37.7055, 23.4023, 37.8177, 23.5723, 37.8177, 23.4023, 37.8177, 23.5723, 37.8177, 23.5723,
									37.0833, 24.8729, 37.8177, 24.1385, 37.0833, 24.8729, 37.0833, 24.8729, 36.461, 25.043, 36.8571, 25.043, 36.461, 25.043,
									36.461, 25.043, 36.1227, 24.5328, 36.1227, 25.043, 36.1227, 24.5328, 36.1227, 24.5328, 36.1227, 24.5328, 36.1227, 24.5328,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									33.0132, 24.5328, 33.0132, 24.5328, 31.2042, 21.3672, 31.2042, 23.5162, 31.2042, 21.3672, 31.2042, 21.3672, 31.8536, 18.9088,
									31.2042, 19.8983, 31.8536, 18.9088, 31.8536, 18.9088, 33.5215, 17.9194, 32.5048, 17.9194, 33.5215, 17.9194, 33.5215, 17.9194,
									35.2743, 19.1078, 34.5399, 17.9194, 35.2743, 19.1078, 35.2743, 19.1078, 36.0088, 21.8773, 36.0088, 20.2945, 36.0088, 21.8773,
									36.0088, 21.8773, 35.9798, 22.2156, 36.0088, 21.9895, 35.9798, 22.2156, 35.9798, 22.2156, 35.9527, 22.5557, 35.9527, 22.4417,
									35.9527, 22.5557, 35.9527, 22.5557, 36.1499, 22.4706, 36.0088, 22.5557, 36.1499, 22.4706, 36.1499, 22.4706, 36.4049, 22.3856,
									36.2928, 22.3856, 36.4049, 22.3856, 36.4049, 22.3856, 36.745, 20.1245, 36.745, 22.3856, 36.745, 20.1245, 36.745, 20.1245,
									35.9237, 17.2428, 36.745, 18.7117, 35.9237, 17.2428, 35.9237, 17.2428, 33.2953, 15.7721, 35.1043, 15.7721, 33.2953, 15.7721,
									33.2953, 15.7721, 30.922, 17.3839, 31.6564, 15.7721, 30.922, 17.3839, 30.922, 17.3839, 30.1876, 21.1411, 30.1876, 18.9939,
									30.1876, 21.1411, 30.1876, 21.1411, 31.8826, 25.2112, 30.1876, 23.064, 31.8826, 25.2112, 31.8826, 25.2112, 33.0132, 24.5328,
									32.5609, 24.929, 33.0132, 24.5328, 33.0132, 24.5328, 33.0132, 24.5328, 33.0132, 24.5328,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									74.2805, 71.2831, 74.2805, 71.2831, 74.7327, 70.8308, 74.7327, 71.2831, 74.7327, 70.8308, 74.7327, 70.8308, 73.291, 68.2875,
									74.7327, 69.7563, 73.291, 68.2875, 73.291, 68.2875, 68.6836, 66.4225, 71.8493, 66.8168, 68.6836, 66.4225, 68.6836, 66.4225,
									67.5241, 66.3085, 68.0053, 66.3646, 67.5241, 66.3085, 67.5241, 66.3085, 65.9141, 66.1385, 67.0447, 66.2524, 65.9141, 66.1385,
									65.9141, 66.1385, 67.497, 57.9422, 67.497, 61.56, 67.497, 57.9422, 67.497, 57.9422, 65.433, 50.3681, 67.497, 53.1937, 65.433,
									50.3681, 65.433, 50.3681, 61.2218, 46.9764, 63.369, 47.5407, 61.2218, 46.9764, 61.2218, 46.9764, 60.9956, 47.1446, 60.9956,
									47.1446, 60.9956, 47.1446, 60.9956, 47.1446, 60.6556, 47.7108, 60.7695, 47.3146, 60.6556, 47.7108, 60.6556, 47.7108, 64.9518,
									51.8098, 63.369, 48.5013, 64.9518, 51.8098, 64.9518, 51.8098, 66.5346, 58.1683, 66.5346, 55.1166, 66.5346, 58.1683, 66.5346,
									58.1683, 64.7836, 66.4225, 66.5346, 61.1639, 64.7836, 66.4225, 64.7836, 66.4225, 60.9396, 70.0964, 62.8046, 67.213, 60.9396,
									70.0964, 60.9396, 70.0964, 61.2778, 70.5487, 60.9396, 70.5487, 61.2778, 70.5487, 61.2778, 70.5487, 62.1262, 69.7003, 61.4479,
									70.5487, 62.1262, 69.7003, 62.1262, 69.7003, 63.9062, 68.0053, 62.8046, 68.8519, 63.9062, 68.0053, 63.9062, 68.0053, 66.2524,
									67.1569, 65.0097, 67.1569, 66.2524, 67.1569, 66.2524, 67.1569, 70.8019, 67.8081, 69.0219, 67.1569, 70.8019, 67.8081, 70.8019,
									67.8081, 73.092, 69.2209, 72.5837, 68.4575, 73.092, 69.2209, 73.092, 69.2209, 73.8843, 70.6337, 73.6021, 69.9825, 73.8843,
									70.6337, 73.8843, 70.6337, 74.2805, 71.2831, 74.1665, 71.2831, 74.2805, 71.2831, 74.2805, 71.2831, 74.2805, 71.2831, 74.2805,
									71.2831,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									43.9229, 23.234, 43.9229, 23.234, 43.4146, 21.6512, 43.5846, 22.5557, 43.4146, 21.6512, 43.4146, 21.6512, 44.234, 18.7678,
									43.4146, 19.7844, 44.234, 18.7678, 44.234, 18.7678, 46.184, 17.7493, 45.0535, 17.7493, 46.184, 17.7493, 46.184, 17.7493,
									48.3891, 19.0228, 47.4847, 17.7493, 48.3891, 19.0228, 48.3891, 19.0228, 49.2936, 21.5933, 49.2936, 20.2945, 49.2936, 21.5933,
									49.2936, 21.5933, 47.0885, 24.5907, 49.2936, 23.8545, 47.0885, 24.5907, 47.0885, 24.5907, 48.0219, 24.9019, 47.4847, 24.759,
									48.0219, 24.9019, 48.0219, 24.9019, 49.4636, 25.4373, 48.5592, 25.043, 49.4636, 25.4373, 49.4636, 25.4373, 50.2542, 20.6328,
									50.2542, 22.8379, 50.2542, 20.6328, 50.2542, 20.6328, 49.0096, 16.3655, 50.2542, 17.8072, 49.0096, 16.3655, 49.0096, 16.3655,
									46.0158, 14.9238, 47.7669, 14.9238, 46.0158, 14.9238, 46.0158, 14.9238, 43.0473, 16.0543, 44.149, 14.9238, 43.0473, 16.0543,
									43.0473, 16.0543, 41.9457, 18.7678, 41.9457, 17.1849, 41.9457, 18.7678, 41.9457, 18.7678, 42.8502, 22.7818, 41.9457, 21.085,
									42.8502, 22.7818, 42.8502, 22.7818, 43.9229, 23.234, 43.3585, 23.064, 43.9229, 23.234, 43.9229, 23.234, 43.9229, 23.234,
									43.9229, 23.234,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									47.8247, 28.4908, 47.8247, 28.4908, 37.6495, 32.9571, 41.5495, 32.9571, 37.6495, 32.9571, 37.6495, 32.9571, 31.0921, 30.0736,
									34.0877, 32.9571, 31.0921, 30.0736, 31.0921, 30.0736, 32.1087, 31.8826, 31.6004, 31.3743, 32.1087, 31.8826, 32.1087, 31.8826,
									34.0316, 33.7476, 34.0316, 33.7476, 34.0316, 33.7476, 34.0316, 33.7476, 38.6661, 35.8387, 36.1227, 35.8387, 38.6661, 35.8387,
									38.6661, 35.8387, 46.8063, 32.2787, 42.0578, 35.8387, 46.8063, 32.2787, 46.8063, 32.2787, 49.6898, 30.0736, 49.6898, 30.0736,
									49.6898, 30.0736, 49.6898, 30.0736, 51.1025, 27.7564, 51.1025, 29.2252, 51.1025, 27.7564, 51.1025, 27.7564, 50.9904, 27.078,
									51.1025, 27.5303, 50.9904, 27.078, 50.9904, 27.078, 45.5618, 24.4768, 50.2542, 26.3418, 45.5618, 24.4768, 45.5618, 24.4768,
									39.0622, 22.6117, 40.8712, 22.6117, 39.0622, 22.6117, 39.0622, 22.6117, 34.0587, 24.5347, 37.3655, 22.6117, 34.0587, 24.5347,
									34.0587, 24.5347, 30.752, 28.3768, 30.752, 26.4557, 30.752, 28.3768, 30.752, 28.3768, 31.3743, 29.5074, 30.752, 28.773,
									31.3743, 29.5074, 31.3743, 29.5074, 37.4233, 32.2208, 34.0877, 32.2208, 37.4233, 32.2208, 37.4233, 32.2208, 47.4286, 27.6985,
									41.2673, 32.2208, 47.4286, 27.6985, 47.4286, 27.6985, 47.6529, 28.0675, 47.5407, 27.7564, 47.6529, 28.0675, 47.6529, 28.0675,
									47.8247, 28.4908, 47.7669, 28.3768, 47.8247, 28.4908, 47.8247, 28.4908, 47.8247, 28.4908, 47.8247, 28.4908,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									55.5127, 95.2515, 55.5127, 95.2515, 61.7301, 98.9834, 57.6039, 98.9834, 61.7301, 98.9834, 61.7301, 98.9834, 67.3269, 96.7783,
									64.7257, 98.9834, 67.3269, 96.7783, 67.3269, 96.7783, 73.092, 91.9177, 73.092, 91.9177, 73.092, 91.9177, 73.092, 91.9177,
									77.2472, 89.1753, 75.071, 90.3348, 77.2472, 89.1753, 77.2472, 89.1753, 80.6389, 87.5635, 79.4233, 88.0158, 80.6389, 87.5635,
									80.6389, 87.5635, 82.7301, 86.3479, 81.8545, 87.1113, 82.7301, 86.3479, 82.7301, 86.3479, 83.7195, 84.3979, 83.6074, 85.5864,
									83.7195, 84.3979, 83.7195, 84.3979, 82.7301, 82.5039, 83.4373, 83.2112, 82.7301, 82.5039, 82.7301, 82.5039, 81.1472, 81.4312,
									82.0246, 81.7984, 81.1472, 81.4312, 81.1472, 81.4312, 79.3672, 80.5828, 80.2717, 81.0622, 79.3672, 80.5828, 79.3672, 80.5828,
									77.6144, 78.915, 78.4628, 80.1017, 77.6144, 78.915, 77.6144, 78.915, 76.3156, 75.8054, 76.7678, 77.7283, 76.3156, 75.8054,
									76.3156, 75.8054, 75.9755, 73.8843, 75.9755, 73.8843, 75.9755, 73.8843, 75.9755, 73.8843, 75.4111, 71.9614, 75.6372, 72.5276,
									75.4111, 71.9614, 75.4111, 71.9614, 75.071, 71.8493, 75.2971, 71.8493, 75.071, 71.8493, 75.071, 71.8493, 73.5443, 72.6687,
									74.4487, 71.8493, 73.5443, 72.6687, 73.5443, 72.6687, 71.7914, 74.5066, 72.6398, 73.4882, 71.7914, 74.5066, 71.7914, 74.5066,
									69.6152, 76.3716, 70.9448, 75.5232, 69.6152, 76.3716, 69.6152, 76.3716, 66.8747, 77.2182, 68.2875, 77.2182, 66.8747, 77.2182,
									66.8747, 77.2182, 62.1823, 75.071, 63.369, 77.2182, 62.1823, 75.071, 62.1823, 75.071, 60.8817, 71.397, 61.2218, 73.4882,
									60.8817, 71.397, 60.8817, 71.397, 59.1867, 70.5487, 59.9211, 70.5487, 59.1867, 70.5487, 59.1867, 70.5487, 56.6433, 75.7494,
									56.6433, 70.5487, 56.6433, 75.7494, 56.6433, 75.7494, 56.6994, 76.8239, 56.6433, 76.4838, 56.6994, 76.8239, 56.6994, 76.8239,
									56.6994, 80.6679, 56.6994, 80.6679, 56.6994, 80.6679, 56.6994, 80.6679, 56.8694, 83.6074, 56.8694, 82.1946, 56.8694, 83.6074,
									56.8694, 83.6074, 56.8116, 85.0491, 56.8694, 84.5119, 56.8116, 85.0491, 56.8116, 85.0491, 56.6994, 86.0386, 56.7555, 85.5864,
									56.6994, 86.0386, 56.6994, 86.0386, 56.4172, 87.9886, 56.6433, 86.4908, 56.4172, 87.9886, 56.4172, 87.9886, 55.965, 91.5215,
									56.1911, 89.4864, 55.965, 91.5215, 55.965, 91.5215, 55.5127, 95.1955, 55.5127, 95.1955, 55.5127, 95.1955, 55.5127, 95.1955,
									55.5127, 95.2515, 55.5127, 95.2515, 55.5127, 95.2515, 55.5127, 95.2515, 55.5127, 95.2515, 55.5127, 95.2515,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									7.51795, 93.5005, 7.51795, 93.5005, 14.9798, 95.3366, 11.1358, 94.0649, 14.9798, 95.3366, 14.9798, 95.3366, 21.5083,
									97.5977, 18.8238, 96.6082, 21.5083, 97.5977, 21.5083, 97.5977, 25.4952, 98.5872, 24.1946, 98.5872, 25.4952, 98.5872,
									25.4952, 98.5872, 31.3182, 95.5916, 28.9991, 98.5872, 31.3182, 95.5916, 31.3182, 95.5916, 31.6564, 93.3304, 31.6564,
									94.5171, 31.6564, 93.3304, 31.6564, 93.3304, 26.0035, 81.4584, 31.6564, 88.6941, 26.0035, 81.4584, 26.0035, 81.4584,
									23.7423, 78.4628, 23.7423, 78.4628, 23.7423, 78.4628, 23.7423, 78.4628, 21.9895, 75.5811, 23.064, 77.5022, 21.9895,
									75.5811, 21.9895, 75.5811, 20.2095, 72.5566, 20.9168, 73.6582, 20.2095, 72.5566, 20.2095, 72.5566, 18.1744, 70.3225,
									19.5022, 71.4531, 18.1744, 70.3225, 18.1744, 70.3225, 15.2059, 68.7958, 16.8467, 69.1919, 15.2059, 68.7958, 15.2059,
									68.7958, 12.8037, 69.8142, 13.6231, 69.1359, 12.8037, 69.8142, 12.8037, 69.8142, 11.8703, 71.1709, 11.9842, 70.4926,
									11.8703, 71.1709, 11.8703, 71.1709, 11.5881, 72.5566, 11.7581, 71.8493, 11.5881, 72.5566, 11.5881, 72.5566, 10.9676,
									73.6003, 11.4198, 73.2621, 10.9676, 73.6003, 10.9676, 73.6003, 9.32689, 74.1105, 10.2892, 73.9404, 9.32689, 74.1105,
									9.32689, 74.1105, 8.42242, 74.1394, 8.98862, 74.1105, 8.42242, 74.1394, 8.42242, 74.1394, 7.51795, 74.1665, 7.85803,
									74.1665, 7.51795, 74.1665, 7.51795, 74.1665, 3.95796, 74.6748, 4.91851, 74.1665, 3.95796, 74.6748, 3.95796, 74.6748,
									2.71341, 77.8966, 2.71341, 76.0876, 2.71341, 77.8966, 2.71341, 77.8966, 3.10957, 80.5828, 2.71341, 78.745, 3.10957,
									80.5828, 3.10957, 80.5828, 3.50573, 83.4934, 3.50573, 82.4207, 3.50573, 83.4934, 3.50573, 83.4934, 2.88345, 86.3769,
									3.50573, 85.0762, 2.88345, 86.3769, 2.88345, 86.3769, 1.6389, 88.7231, 2.26117, 87.6775, 1.6389, 88.7231, 1.6389, 88.7231,
									1.01843, 90.7871, 1.01843, 89.7686, 1.01843, 90.7871, 1.01843, 90.7871, 7.51795, 93.5005, 1.58282, 92.6521, 7.51795, 93.5005,
									7.51795, 93.5005, 7.51795, 93.5005, 7.51795, 93.5005,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									18.5416, 63.425, 18.5416, 63.425, 20.3506, 55.6538, 18.5416, 60.0333, 20.3506, 55.6538, 20.3506, 55.6538, 23.9124,
									49.2375, 22.1595, 51.2726, 23.9124, 49.2375, 23.9124, 49.2375, 23.4041, 48.9535, 23.7984, 49.1236, 23.4041, 48.9535,
									23.4041, 48.9535, 23.064, 48.7274, 23.064, 48.7274, 23.064, 48.7274, 23.064, 48.7274, 19.5872, 55.2016, 21.7073, 50.3681,
									19.5872, 55.2016, 19.5872, 55.2016, 17.4671, 63.0868, 17.4671, 60.0333, 17.4671, 63.0868, 17.4671, 63.0868, 17.7493,
									65.518, 17.4671, 64.6135, 17.7493, 65.518, 17.7493, 65.518, 19.4461, 67.9763, 18.0333, 66.4225, 19.4461, 67.9763,
									19.4461, 67.9763, 23.7423, 71.6792, 20.8589, 69.5302, 23.7423, 71.6792, 23.7423, 71.6792, 27.2481, 73.9404, 27.2481,
									73.9404, 27.2481, 73.9404, 27.2481, 73.9404, 33.0692, 80.78, 33.0692, 78.801, 33.0692, 80.78, 33.0692, 80.78, 31.7686,
									82.9562, 33.0692, 81.7984, 31.7686, 82.9562, 31.7686, 82.9562, 29.3392, 84.1157, 30.4698, 84.1157, 29.3392, 84.1157,
									29.3392, 84.1157, 29.2252, 84.3418, 29.2252, 84.1157, 29.2252, 84.3418, 29.2252, 84.3418, 30.2437, 86.3208, 29.2252,
									84.3979, 30.2437, 86.3208, 30.2437, 86.3208, 38.6661, 89.1464, 32.5048, 89.1464, 38.6661, 89.1464, 38.6661, 89.1464,
									55.681, 80.2156, 49.6337, 89.1464, 55.681, 80.2156, 55.681, 80.2156, 55.6249, 77.0789, 55.681, 77.7283, 55.6249, 77.0789,
									55.6249, 77.0789, 55.5688, 75.8615, 55.5688, 76.4277, 55.5688, 75.8615, 55.5688, 75.8615, 56.4443, 71.057, 55.5688,
									72.6398, 56.4443, 71.057, 56.4443, 71.057, 58.6784, 69.4741, 57.3217, 69.4741, 58.6784, 69.4741, 58.6784, 69.4741,
									60.6556, 70.2086, 59.5829, 69.4741, 60.6556, 70.2086, 60.6556, 70.2086, 61.2218, 63.4829, 61.2218, 66.4225, 61.2218,
									63.4829, 61.2218, 63.4829, 60.5434, 55.7389, 61.2218, 58.9606, 60.5434, 55.7389, 60.5434, 55.7389, 58.8756, 50.7643,
									59.8651, 52.5153, 58.8756, 50.7643, 58.8756, 50.7643, 56.8966, 47.6837, 57.8861, 49.0114, 56.8966, 47.6837, 56.8966,
									47.6837, 55.1727, 44.6862, 55.9071, 46.3541, 55.1727, 44.6862, 55.1727, 44.6862, 54.3822, 40.8712, 54.4382, 43.0184,
									54.3822, 40.8712, 54.3822, 40.8712, 51.9781, 35.8116, 53.1376, 38.3839, 51.9781, 35.8116, 51.9781, 35.8116, 49.8019,
									31.3164, 50.8204, 33.2393, 49.8019, 31.3164, 49.8019, 31.3164, 46.8063, 33.4654, 46.8063, 33.4654, 46.8063, 33.4654,
									46.8063, 33.4654, 38.6661, 36.9693, 42.1139, 36.9693, 38.6661, 36.9693, 38.6661, 36.9693, 33.8055, 35.1604, 35.4444,
									36.9693, 33.8055, 35.1604, 33.8055, 35.1604, 32.1087, 33.5215, 32.1087, 33.5215, 32.1087, 33.5215, 32.1087, 33.5215,
									30.8659, 37.1393, 31.8265, 35.2182, 30.8659, 37.1393, 30.8659, 37.1393, 28.887, 41.0973, 28.887, 41.0973, 28.887,
									41.0973, 28.887, 41.0973, 27.3602, 45.7336, 27.5303, 44.4891, 27.3602, 45.7336, 27.3602, 45.7336, 27.078, 47.1446,
									27.1902, 46.9764, 27.078, 47.1446, 27.078, 47.1446, 24.5907, 51.9509, 24.5907, 51.9509, 24.5907, 51.9509, 24.5907,
									51.9509, 20.5767, 65.404, 20.5767, 59.695, 20.5767, 65.404, 20.5767, 65.404, 20.7467, 67.7213, 20.5767, 66.5346,
									20.7467, 67.7213, 20.7467, 67.7213, 18.5416, 63.425, 18.5416, 66.1963, 18.5416, 63.425, 18.5416, 63.425, 18.5416,
									63.425, 18.5416, 63.425,
									-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
									42.284, 94.7432, 42.284, 94.7432, 32.2787, 96.4961, 35.9527, 94.7432, 32.2787, 96.4961, 32.2787, 96.4961, 32.2226,
									96.3821, 32.2226, 96.3821, 32.2226, 96.3821, 32.2226, 96.3821, 26.2296, 99.3795, 30.0736, 99.3795, 26.2296, 99.3795,
									26.2296, 99.3795, 18.5977, 97.5417, 23.7984, 99.3795, 18.5977, 97.5417, 18.5977, 97.5417, 9.38478, 94.7993, 13.397,
									95.7038, 9.38478, 94.7993, 9.38478, 94.7993, 7.57403, 94.461, 8.98862, 94.6871, 7.57403, 94.461, 7.57403, 94.461,
									5.0596, 94.0088, 6.16125, 94.2349, 5.0596, 94.0088, 5.0596, 94.0088, 2.68447, 93.3576, 3.95796, 93.7827, 2.68447,
									93.3576, 2.68447, 93.3576, 0.705487, 92.312, 1.41278, 92.9343, 0.705487, 92.312, 0.705487, 92.312, 0, 90.8992, 0,
									91.6915, 0, 90.8992, 0, 90.8992, 0.340081, 89.2042, 0, 90.1087, 0.340081, 89.2042, 0.340081, 89.2042, 1.01662, 88.1297,
									0.678352, 88.638, 1.01662, 88.1297, 1.01662, 88.1297, 1.60996, 87.1402, 1.3567, 87.6214, 1.60996, 87.1402, 1.60996,
									87.1402, 2.06219, 86.2068, 1.86502, 86.6591, 2.06219, 86.2068, 2.06219, 86.2068, 2.37333, 85.2463, 2.26117, 85.7546,
									2.37333, 85.2463, 2.37333, 85.2463, 2.48729, 84.2297, 2.48729, 84.738, 2.48729, 84.2297, 2.48729, 84.2297, 2.09113,
									81.1762, 2.48729, 83.7774, 2.09113, 81.1762, 2.09113, 81.1762, 1.69679, 77.8966, 1.69679, 78.5749, 1.69679, 77.8966,
									1.69679, 77.8966, 2.76949, 74.4505, 1.69679, 75.6933, 2.76949, 74.4505, 2.76949, 74.4505, 4.91851, 73.206, 3.844, 73.206,
									4.91851, 73.206, 4.91851, 73.206, 8.70643, 73.206, 8.70643, 73.206, 8.70643, 73.206, 8.70643, 73.206, 10.1753, 72.6398,
									9.15866, 73.206, 10.1753, 72.6398, 10.1753, 72.6398, 10.7415, 71.2559, 10.5154, 71.8493, 10.7415, 71.2559, 10.7415,
									71.2559, 11.0508, 70.4076, 10.9676, 70.6608, 11.0508, 70.4076, 11.0508, 70.4076, 11.2769, 69.8414, 11.1358, 70.1525,
									11.2769, 69.8414, 11.2769, 69.8414, 11.7581, 69.1359, 11.4198, 69.5302, 11.7581, 69.1359, 11.7581, 69.1359, 11.4198,
									66.9308, 11.1937, 68.3996, 11.4198, 66.9308, 11.4198, 66.9308, 13.1709, 61.8422, 11.4198, 65.1779, 13.1709, 61.8422,
									13.1709, 61.8422, 14.3594, 59.7511, 14.3594, 59.7511, 14.3594, 59.7511, 14.3594, 59.7511, 16.5916, 55.3698, 15.7721,
									57.2638, 16.5916, 55.3698, 16.5916, 55.3698, 18.3716, 49.2375, 17.411, 53.4759, 18.3716, 49.2375, 18.3716, 49.2375,
									22.1595, 42.5101, 19.1639, 45.8458, 22.1595, 42.5101, 22.1595, 42.5101, 24.6468, 39.5706, 24.6468, 39.5706, 24.6468,
									39.5706, 24.6468, 39.5706, 28.1236, 34.4549, 27.1902, 36.461, 28.1236, 34.4549, 28.1236, 34.4549, 29.057, 30.1297,
									29.057, 32.447, 29.057, 30.1297, 29.057, 30.1297, 28.5469, 24.2506, 29.057, 29.1692, 28.5469, 24.2506, 28.5469,
									24.2506, 28.0386, 14.5276, 28.0386, 19.3321, 28.0386, 14.5276, 28.0386, 14.5276, 28.688, 8.9036, 28.0386, 11.362,
									28.688, 8.9036, 28.688, 8.9036, 30.8641, 4.35231, 29.3392, 6.44344, 30.8641, 4.35231, 30.8641, 4.35231, 35.3015,
									1.13059, 32.3909, 2.26117, 35.3015, 1.13059, 35.3015, 1.13059, 42.34, 0, 38.2139, 0, 42.34, 0, 42.34, 0, 45.3085,
									0.255061, 43.7546, 0, 45.3085, 0.255061, 45.3085, 0.255061, 49.0096, 1.44173, 46.8624, 0.508312, 49.0096, 1.44173,
									49.0096, 1.44173, 52.8825, 3.844, 51.1586, 2.37333, 52.8825, 3.844, 52.8825, 3.844, 55.9921, 8.11129, 54.6083,
									5.31286, 55.9921, 8.11129, 55.9921, 8.11129, 57.7739, 14.5276, 57.3778, 10.9097, 57.7739, 14.5276, 57.7739, 14.5276,
									58.3944, 20.0955, 58.1122, 17.0149, 58.3944, 20.0955, 58.3944, 20.0955, 58.8466, 24.7029, 58.6784, 23.1761, 58.8466,
									24.7029, 58.8466, 24.7029, 59.496, 28.4058, 59.0167, 26.2296, 59.496, 28.4058, 59.496, 28.4058, 60.7967, 32.2787,
									59.9772, 30.5819, 60.7967, 32.2787, 60.7967, 32.2787, 63.0868, 36.2638, 61.6179, 33.9737, 63.0868, 36.2638, 63.0868,
									36.2638, 66.8186, 41.3795, 64.5574, 38.5521, 66.8186, 41.3795, 66.8186, 41.3795, 73.4592, 51.8659, 71.057, 46.6363,
									73.4592, 51.8659, 73.4592, 51.8659, 75.8633, 64.2174, 75.8633, 57.0938, 75.8633, 64.2174, 75.8633, 64.2174, 74.8449,
									70.8869, 75.8633, 67.6652, 74.8449, 70.8869, 74.8449, 70.8869, 76.3716, 71.5942, 75.8633, 70.8869, 76.3716, 71.5942,
									76.3716, 71.5942, 77.22, 74.6188, 76.8799, 72.3015, 77.22, 74.6188, 77.22, 74.6188, 77.7283, 77.05, 77.7283, 77.05,
									77.7283, 77.05, 77.7283, 77.05, 79.1411, 79.085, 77.9545, 78.1806, 79.1411, 79.085, 79.1411, 79.085, 81.5434, 80.5268,
									80.3278, 79.9895, 81.5434, 80.5268, 81.5434, 80.5268, 83.7756, 81.9956, 82.759, 81.0622, 83.7756, 81.9956, 83.7756,
									81.9956, 84.7941, 84.0596, 84.7941, 82.929, 84.7941, 84.0596, 84.7941, 84.0596, 83.9167, 86.6301, 84.7941, 85.6985,
									83.9167, 86.6301, 83.9167, 86.6301, 81.6845, 88.0447, 83.0412, 87.5635, 81.6845, 88.0447, 81.6845, 88.0447, 77.7555,
									89.9658, 80.3278, 88.5259, 77.7555, 89.9658, 77.7555, 89.9658, 72.6977, 93.5005, 75.185, 91.4075, 72.6977, 93.5005,
									72.6977, 93.5005, 69.4741, 96.326, 69.4741, 96.326, 69.4741, 96.326, 69.4741, 96.326, 65.6591, 99.1245, 67.2709,
									98.2489, 65.6591, 99.1245, 65.6591, 99.1245, 61.9562, 100, 64.0473, 100, 61.9562, 100, 61.9562, 100, 59.809, 99.7178,
									59.809, 99.7178, 59.809, 99.7178, 59.809, 99.7178, 54.5504, 95.7038, 55.681, 98.7012, 54.5504, 95.7038, 54.5504, 95.7038,
									42.284, 94.7432, 46.298, 94.7432};
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
			*n = 272;
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
			*n = 80;
			vals = AutoShapes22;
			break;
		case 23:
			*n = 304;
			vals = AutoShapes23;
			break;
		case 24:
			*n = 40;
			vals = AutoShapes24;
			break;
		case 25:
			*n = 48;
			vals = AutoShapes25;
			break;
		case 26:
			*n = 40;
			vals = AutoShapes26;
			break;
		case 27:
			*n = 48;
			vals = AutoShapes27;
			break;
		case 28:
			*n = 40;
			vals = AutoShapes28;
			break;
		case 29:
			*n = 48;
			vals = AutoShapes29;
			break;
		case 30:
			*n = 40;
			vals = AutoShapes30;
			break;
		case 31:
			*n = 56;
			vals = AutoShapes31;
			break;
		case 32:
			*n = 128;
			vals = AutoShapes32;
			break;
		case 33:
			*n = 128;
			vals = AutoShapes33;
			break;
		case 34:
			*n = 128;
			vals = AutoShapes34;
			break;
		case 35:
			*n = 128;
			vals = AutoShapes35;
			break;
		case 36:
			*n = 128;
			vals = AutoShapes36;
			break;
		case 37:
			*n = 128;
			vals = AutoShapes37;
			break;
		case 38:
			*n = 80;
			vals = AutoShapes38;
			break;
		case 39:
			*n = 104;
			vals = AutoShapes39;
			break;
		case 40:
			*n = 80;
			vals = AutoShapes40;
			break;
		case 41:
			*n = 104;
			vals = AutoShapes41;
			break;
		case 42:
			*n = 80;
			vals = AutoShapes42;
			break;
		case 43:
			*n = 104;
			vals = AutoShapes43;
			break;
		case 44:
			*n = 2344;
			vals = AutoShapes44;
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
	return QPixmap::fromImage(Ico.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void AutoformButtonGroup::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void AutoformButtonGroup::languageChange()
{
	menu1->setTitle( tr("Default Shapes"));
	menu2->setTitle( tr("Arrows"));
	menu3->setTitle( tr("Flow Chart"));
	menu4->setTitle( tr("Jigsaw"));
	menu5->setTitle( tr("Specials"));
}
