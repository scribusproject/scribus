/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TREE_H
#define TREE_H

#include <qptrlist.h>
#include <qvaluelist.h>
#include <qpixmap.h>
#include <qlistview.h>

#include "scribusapi.h"
#include "scrpalettebase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class ScribusMainWindow;
class ScribusDoc;
class PageItem;
class Page;
class DynamicTip;

class SCRIBUS_API TreeItem : public QListViewItem
{
public:
	TreeItem(TreeItem* parent, TreeItem* after);
	TreeItem(QListView* parent, TreeItem* after);
	~TreeItem() {};
	PageItem *PageItemObject;
	Page *PageObject;
	int type;
};

class SCRIBUS_API Tree : public ScrPaletteBase
{
	Q_OBJECT

public:
	Tree( QWidget* parent );
	//void keyPressEvent(QKeyEvent *k);
	//void closeEvent(QCloseEvent *ce);
	void resizeEvent(QResizeEvent *r);
	
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *);
	void unsetDoc();
	
	void rebuildPageD();
	void reopenTree();
	QListViewItem* getListItem(uint SNr, int Nr);
	void setItemIcon(QListViewItem *item, PageItem *pgItem);
	void parseSubGroup(int level, TreeItem* object, QPtrList<PageItem> *subGroupList, int itemType);
	void buildReopenVals();

public slots:
	void BuildTree(bool storeVals = true);
	void languageChange();
	void slotShowSelect(uint SNr, int Nr);
	void setPaletteShown(bool);

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
	void slotRightClick(QListViewItem* ite, const QPoint &, int col);
	void slotRenameItem();
	void slotDoRename(QListViewItem* ite, int col);
	void slotRemoveElement(uint SNr, uint Nr);
	void slotUpdateElement(uint SNr, uint Nr);
	void slotAddElement(PageItem *item);
	void slotMoveElement(uint SNr, uint NrOld, uint NrNew);
	void slotDelPage(uint Nr);
	void slotAddPage(uint Nr);
	void slotSelect(QListViewItem* ite);
protected:
	void clearPalette();
	int idElemCol;
	QListView* reportDisplay;
	QListViewItem* freeObjects;
	QListViewItem* rootObject;
	QListViewItem* currentObject;
	int currentColumn;
	ScribusMainWindow* m_MainWindow;
	QPixmap imageIcon;
	QPixmap textIcon;
	QPixmap lineIcon;
	QPixmap polygonIcon;
	QPixmap polylineIcon;
	QPixmap groupIcon;
	QPixmap buttonIcon;
	QPixmap textFieldIcon;
	QPixmap checkBoxIcon;
	QPixmap comboBoxIcon;
	QPixmap listBoxIcon;
	QPixmap annotTextIcon;
	QPixmap annotLinkIcon;
	bool selectionTriggered;
	ScribusDoc *currDoc;
	DynamicTip* dynTip;
};

#endif // TREE_H
