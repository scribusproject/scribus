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
#include <qvaluelist.h>
#include "scribusview.h"
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QListView;
class QListViewItem;
class ScribusDoc;

class Tree : public QDialog
{
	Q_OBJECT

public:
	Tree( QWidget* parent, WFlags fl );
	~Tree() {};
	void keyPressEvent(QKeyEvent *k);
	void closeEvent(QCloseEvent *ce);
	void resizeEvent(QResizeEvent *r);
	void rebuildPageD();
	void reopenTree(QValueList<int> op);
	QValueList<int> buildReopenVals();

	QListView* reportDisplay;
	QMap<QListViewItem*, int> itemMap;
	QMap<QListViewItem*, int> pageMap;
	QMap<QListViewItem*, QString> templatePageMap;
	QMap<QListViewItem*, int> templateItemMap;
	ScribusDoc* document;
	ScribusView *vie;

public slots:
	void slotRightClick(QListViewItem* ite, const QPoint &, int);
	void slotDoRename(QListViewItem* ite, int col);
	void slotShowSelect(uint SNr, int Nr);
	void slotRemoveElement(uint SNr, uint Nr);
	void slotUpdateElement(uint SNr, uint Nr);
	void slotAddElement(uint SNr, uint Nr);
	void slotMoveElement(uint SNr, uint NrOld, uint NrNew);
	void slotDelPage(uint Nr);
	void slotAddPage(uint Nr);
	void slotSelect(QListViewItem* ite);
	void BuildTree(ScribusDoc *doc);

signals:
	void ToggleAllPalettes();
	void Schliessen();
	void CloseMpal();
	void CloseSpal();
	void selectElement(int, int);
	void selectPage(int);
	void selectTemplatePage(QString);
	void selectTemplateElement(QString, int);

protected slots:
	virtual void reject();
};

#endif // TREE_H
