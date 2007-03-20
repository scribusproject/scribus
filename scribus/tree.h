/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TREE_H
#define TREE_H

#include <q3ptrlist.h>
#include <q3valuelist.h>
#include <qpixmap.h>
#include <q3listview.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "scribusapi.h"
#include "scrpalettebase.h"

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class ScribusMainWindow;
class ScribusDoc;
class PageItem;
class Page;
// class DynamicTip;

class SCRIBUS_API TreeItem : public Q3ListViewItem
{
public:
	TreeItem(TreeItem* parent, TreeItem* after);
	TreeItem(Q3ListView* parent, TreeItem* after);
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
	void resizeEvent(QResizeEvent *r);
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *);
	void unsetDoc();
	void reopenTree();
	Q3ListViewItem* getListItem(uint SNr, int Nr);
	void setItemIcon(Q3ListViewItem *item, PageItem *pgItem);
	void parseSubGroup(int level, TreeItem* object, Q3PtrList<PageItem> *subGroupList, int itemType);
	void buildReopenVals();

public slots:
	void BuildTree(bool storeVals = true);
	void languageChange();
	void slotShowSelect(uint SNr, int Nr);
	void setPaletteShown(bool);

signals:
	void ToggleAllPalettes();
	void selectElement(int, int, bool);
	void selectPage(int);
	void selectMasterPage(QString);

protected slots:
	void slotRightClick(Q3ListViewItem* ite, const QPoint &, int col);
	void slotRenameItem();
	void slotDoRename(Q3ListViewItem* ite, int col);
	void slotSelect(Q3ListViewItem* ite);
protected:
	void clearPalette();
	int idElemCol;
	Q3ListView* reportDisplay;
	Q3ListViewItem* freeObjects;
	Q3ListViewItem* rootObject;
	Q3ListViewItem* currentObject;
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
// 	DynamicTip* dynTip;
};

#endif // TREE_H
