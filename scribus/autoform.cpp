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
	buttonGroup1->setExclusive( TRUE );
	buttonGroup1->setColumnLayout(0, Qt::Vertical );
	buttonGroup1->layout()->setSpacing( 1 );
	buttonGroup1->layout()->setMargin( 1 );
	buttonGroup1Layout = new QGridLayout( buttonGroup1->layout() );
	buttonGroup1Layout->setAlignment( Qt::AlignTop );

	toolButton1 = new QToolButton( buttonGroup1, "toolButton1" );
	toolButton1->setToggleButton( TRUE );
  toolButton1->setPixmap(loadIcon("rect.png"));
	buttonGroup1Layout->addWidget( toolButton1, 0, 0 );

	toolButton2 = new QToolButton( buttonGroup1, "toolButton2" );
	toolButton2->setToggleButton( TRUE );
  toolButton2->setPixmap(loadIcon("oval.png"));
	buttonGroup1Layout->addWidget( toolButton2, 0, 1 );

	toolButton3 = new QToolButton( buttonGroup1, "toolButton3" );
	toolButton3->setToggleButton( TRUE );
  toolButton3->setPixmap(loadIcon("triangle2.png"));
	buttonGroup1Layout->addWidget( toolButton3, 0, 2 );

	toolButton4 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton4->setToggleButton( TRUE );
  toolButton4->setPixmap(loadIcon("cross.png"));
	buttonGroup1Layout->addWidget( toolButton4, 0, 3 );

	toolButton5 = new QToolButton( buttonGroup1, "toolButton3" );
	toolButton5->setToggleButton( TRUE );
  toolButton5->setPixmap(loadIcon("arrowl.png"));
	buttonGroup1Layout->addWidget( toolButton5, 1, 0 );

	toolButton6 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton6->setToggleButton( TRUE );
  toolButton6->setPixmap(loadIcon("arrowr.png"));
	buttonGroup1Layout->addWidget( toolButton6, 1, 1 );

	toolButton7 = new QToolButton( buttonGroup1, "toolButton3" );
	toolButton7->setToggleButton( TRUE );
  toolButton7->setPixmap(loadIcon("arrowu.png"));
	buttonGroup1Layout->addWidget( toolButton7, 1, 2 );

	toolButton8 = new QToolButton( buttonGroup1, "toolButton4" );
	toolButton8->setToggleButton( TRUE );
  toolButton8->setPixmap(loadIcon("arrowd.png"));
	buttonGroup1Layout->addWidget( toolButton8, 1, 3 );

	Men = new QPopupMenu();
	Men->insertItem(buttonGroup1);
	setPopup(Men);
	setPopupDelay(10);
  setPixmap(loadIcon("rect.png"));
	connect(buttonGroup1, SIGNAL(clicked(int)), this, SLOT(selForm(int)));
}

void Autoforms::selForm(int a)
{
	Men->activateItemAt(0);
	const QPixmap *px = buttonGroup1->find(a)->pixmap();
	setPixmap(*px);
	emit FormSel(a);
}
