/****************************************************************************
** Form implementation generated from reading ui file 'tree.ui'
**
** Created: Thu May 24 10:46:23 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "tree.h"
#include "tree.moc"
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
// #include <iostream.h>
extern QPixmap loadIcon(QString nam);

Tree::Tree( QWidget* parent, WFlags fl )
    : QDialog( parent, "Tree", false, fl )
{
    resize( 220, 240 );
    setMinimumSize( QSize( 220, 240 ) );
    setMaximumSize( QSize( 800, 600 ) );
    setCaption( tr( "Outline" ) );
  	setIcon(loadIcon("AppIcon.png"));

    ListView1 = new QListView( this, "ListView1" );

    ListView1->setGeometry( QRect( 0, 0, 220, 240 ) );
    ListView1->setMinimumSize( QSize( 220, 240 ) );
    ListView1->setRootIsDecorated( TRUE );
    ListView1->addColumn(tr("Element"));
    ListView1->addColumn(tr("Type"));
    ListView1->addColumn(tr("Information"));
    ListView1->setSorting(0,1);

    // signals and slots connections
    connect( ListView1, SIGNAL( selectionChanged(QListViewItem*) ), this, SLOT( slotSelect(QListViewItem*) ) );
}


void Tree::slotSelect(QListViewItem* ite)
{
    if (ite->text(0).startsWith(tr("Page")))
    	{
    	QStringList Sel = QStringList::split(" ", ite->text(0));
    	emit SelectSeite(Sel[1].toInt()-1);
    	}
    if (ite->text(0).startsWith(tr("Item")))
    	{
    	QStringList Sel = QStringList::split(" ", ite->text(0));
    	QStringList Sep = QStringList::split(" ", ite->parent()->text(0));
    	emit SelectElement(Sep[1].toInt()-1, Sel[1].toInt()-1);
    	}
}

void Tree::closeEvent(QCloseEvent *ce)
{	
	emit Schliessen();
	ce->accept();
}

void Tree::resizeEvent(QResizeEvent *r)
{
	ListView1->resize(r->size());
}

void Tree::BuildTree(ScribusView *view)
{
	uint a, b;
	QString cc;
	QString xp, yp, fon;
	Elemente.clear();
	Seiten.clear();
	ListView1->clear();
	QListViewItem * item = new QListViewItem( ListView1, 0 );
  item->setText( 0, tr(view->Doc->DocName));
  for (a = 0; a < view->Pages.count(); ++a)
  	{
  	Seiten.append(new QListViewItem(item, "Seiten"));
  	Seiten.current()->setText(0, tr("Page")+" "+cc.setNum(a+1));
//  	Seiten.current()->setPixmap(0, view->PageToPixmap(a, 60));
  	if (view->Pages.at(a)->Items.count() != 0)
  		{
  		for (b = 0; b < view->Pages.at(a)->Items.count(); b++)
  			{
  			Elemente.append(new QListViewItem(Seiten.current(), "Items"));
  			Elemente.current()->setText(0, tr("Item")+" "+cc.setNum(b+1));
  			xp = tr("X:")+" "+cc.setNum(view->Pages.at(a)->Items.at(b)->Xpos);
  			yp = tr("Y:")+" "+cc.setNum(view->Pages.at(a)->Items.at(b)->Ypos);
  			fon = tr("Font:")+" "+view->Pages.at(a)->Items.at(b)->IFont;
  			switch (view->Pages.at(a)->Items.at(b)->PType)
  				{
  				case 1:
  					Elemente.current()->setText(1, tr("Ellipse"));
  					Elemente.current()->setText(2, xp+" "+yp);
  					break;
  				case 2:
  					Elemente.current()->setText(1, tr("Image"));
  					Elemente.current()->setText(2, xp+" "+yp+" "+view->Pages.at(a)->Items.at(b)->Pfile);
  					break;
  				case 3:
  					Elemente.current()->setText(1, tr("Rectangle"));
  					Elemente.current()->setText(2, xp+" "+yp);
  					break;
  				case 4:
  					Elemente.current()->setText(1, tr("Text"));
  					Elemente.current()->setText(2, xp+" "+yp+" "+fon);
  					break;
  				case 5:
  					Elemente.current()->setText(1, tr("Line"));
  					Elemente.current()->setText(2, xp+" "+yp);
  					break;
  				}
  			}
  		}
  	}
}
