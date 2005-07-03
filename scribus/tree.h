/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
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

class Tree : public QDialog
{
	Q_OBJECT

public:
	Tree( QWidget* parent, WFlags fl );
	~Tree() {};

	QListView* ListView1;
	QPtrList<QListViewItem> Seiten;
	struct Elem { QPtrList<QListViewItem> Elemente; };
	QPtrList<Elem> PageObj;
	ScribusView *vie;
	void keyPressEvent(QKeyEvent *k);
	void closeEvent(QCloseEvent *ce);
	void resizeEvent(QResizeEvent *r);
	void rebuildPageD();
	void reopenTree(QValueList<int> op);
	QValueList<int> buildReopenVals();

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
	void BuildTree(ScribusView *view);

signals:
	void ToggleAllPalettes();
	void Schliessen();
	void CloseMpal();
	void CloseSpal();
	void SelectElement(int, int);
	void SelectSeite(int);

protected slots:
	virtual void reject();
};

#endif // TREE_H
