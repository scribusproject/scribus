/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TREE_H
#define TREE_H

#include <QPixmap>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QResizeEvent>
#include <QList>

class QEvent;

#include "scribusapi.h"
#include "scrpalettebase.h"

class ScribusMainWindow;
class ScribusDoc;
class PageItem;
class Page;

class SCRIBUS_API OutlineTreeItem : public QTreeWidgetItem
{
public:
	OutlineTreeItem(OutlineTreeItem* parent, OutlineTreeItem* after);
	OutlineTreeItem(QTreeWidget* parent, OutlineTreeItem* after);
	~OutlineTreeItem() {};
	PageItem *PageItemObject;
	Page *PageObject;
	int type;
};

class SCRIBUS_API TreeWidget : public QTreeWidget
{
public:
	TreeWidget(QWidget* parent);
	~TreeWidget() {};
protected:
	bool viewportEvent(QEvent *event);
};

class SCRIBUS_API Tree : public ScrPaletteBase
{
	Q_OBJECT

public:
	Tree( QWidget* parent );
	
	virtual void changeEvent(QEvent *e);
	
	void resizeEvent(QResizeEvent *r);
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *);
	void unsetDoc();
	void reopenTree();
	QTreeWidgetItem* getListItem(int SNr, int Nr);
	void setItemIcon(QTreeWidgetItem *item, PageItem *pgItem);
	void parseSubGroup(int level, OutlineTreeItem* object, QList<PageItem*> *subGroupList, int itemType);
	void buildReopenVals();

public slots:
	void BuildTree(bool storeVals = true);
	void languageChange();
	void slotShowSelect(uint SNr, int Nr);
	void setPaletteShown(bool);
	void slotRightClick(QPoint point);

signals:
	void ToggleAllPalettes();
	void selectElement(int, int, bool);
	void selectPage(int);
	void selectMasterPage(QString);

protected slots:
	void slotRenameItem();
	void slotDoRename(QTreeWidgetItem* ite, int col);
	void slotSelect(QTreeWidgetItem* ite, int col);
protected:
	void clearPalette();
	TreeWidget* reportDisplay;
	QTreeWidgetItem* freeObjects;
	QTreeWidgetItem* rootObject;
	QTreeWidgetItem* currentObject;
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
};

#endif // TREE_H
