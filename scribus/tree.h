/****************************************************************************
** Form interface generated from reading ui file 'tree.ui'
**
** Created: Thu May 24 10:46:08 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef TREE_H
#define TREE_H

#include <qdialog.h>
#include <qptrlist.h>
#include "scribusview.h"
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QListView;
class QListViewItem;

class Tree : public QDialog
{ 
    Q_OBJECT

public:
    Tree( QWidget* parent, WFlags fl );
    ~Tree() {};

    QListView* ListView1;
    QPtrList<QListViewItem> Seiten;
    QPtrList<QListViewItem> Elemente;
    void closeEvent(QCloseEvent *ce);
    void resizeEvent(QResizeEvent *r);

public slots:
    virtual void slotSelect(QListViewItem* ite);
    void BuildTree(ScribusView *view);
    	
signals:
		void Schliessen();
		void SelectElement(int, int);
		void SelectSeite(int);

};

#endif // TREE_H
