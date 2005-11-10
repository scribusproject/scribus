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

#include <q3ptrlist.h>
#include <q3valuelist.h>
#include <qpixmap.h>

#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <QResizeEvent>

#include "scribusapi.h"
#include "scrpalettebase.h"

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class Q3ListView;
class Q3ListViewItem;
class ScribusApp;
class ScribusDoc;
class PageItem;

class SCRIBUS_API Tree : public ScrPaletteBase
{
	Q_OBJECT

public:
	Tree( QWidget* parent, ScribusApp* scApp );
	//void keyPressEvent(QKeyEvent *k);
	//void closeEvent(QCloseEvent *ce);
	void resizeEvent(QResizeEvent *r);
	
	void setDoc(ScribusDoc *);
	void unsetDoc();
	
	void rebuildPageD();
	void reopenTree(Q3ValueList<int> op);
	Q3ListViewItem* getListItem(uint SNr, int Nr);
	void setItemIcon(Q3ListViewItem *item, int typ);
	void parseSubGroup(int level, Q3ListViewItem* object, Q3PtrList<PageItem> *subGroupList, bool onMasterPage);
	Q3ValueList<int> buildReopenVals();

public slots:
	void BuildTree();
	void languageChange();
	void slotShowSelect(uint SNr, int Nr);

signals:
	void ToggleAllPalettes();
	//void Schliessen();
	//void CloseMpal();
	//void CloseSpal();
	void selectElement(int, int, bool);
	void selectPage(int);
	void selectMasterPage(QString);
	void selectMasterPageElement(QString, int);

protected slots:
	//virtual void reject();
	void slotRightClick(Q3ListViewItem* ite, const QPoint &, int);
	void slotDoRename(Q3ListViewItem* ite, int col);
	void slotRemoveElement(uint SNr, uint Nr);
	void slotUpdateElement(uint SNr, uint Nr);
	void slotAddElement(PageItem *item);
	void slotMoveElement(uint SNr, uint NrOld, uint NrNew);
	void slotDelPage(uint Nr);
	void slotAddPage(uint Nr);
	void slotSelect(Q3ListViewItem* ite);
protected:
	void clearPalette();
	int idElemCol;
	Q3ListView* reportDisplay;
	QMap<Q3ListViewItem*, int> itemMap;
	QMap<Q3ListViewItem*, int> groupMap;
	QMap<Q3ListViewItem*, int> pageMap;
	QMap<Q3ListViewItem*, int> masterPageItemMap;
	QMap<Q3ListViewItem*, int> masterPageGroupMap;
	QMap<Q3ListViewItem*, QString> masterPageMap;
/* Reverse mapping for selecting Listview Items */
	QMap<int, Q3ListViewItem*> itemMapRev;
	QMap<int, Q3ListViewItem*> groupMapRev;
	QMap<int, Q3ListViewItem*> pageMapRev;
	QMap<int, Q3ListViewItem*> masterPageItemMapRev;
	QMap<int, Q3ListViewItem*> masterPageGroupMapRev;
	QMap<QString, Q3ListViewItem*> masterPageMapRev;
	Q3ListViewItem* freeObjects;
	Q3ListViewItem* rootObject;
	ScribusApp* ScApp;
	QPixmap imageIcon;
	QPixmap textIcon;
	QPixmap lineIcon;
	QPixmap polygonIcon;
	QPixmap polylineIcon;
	QPixmap groupIcon;
	bool selectionTriggered;
	ScribusDoc *currDoc;
};

#endif // TREE_H
