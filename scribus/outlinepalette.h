/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OUTLINEPALETTE_H
#define OUTLINEPALETTE_H

#include <QPixmap>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QList>
#include <QLabel>

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

class SCRIBUS_API OutlineWidget : public QTreeWidget
{
public:
	OutlineWidget(QWidget* parent);
	~OutlineWidget() {};

	void selectItems(QList<QTreeWidgetItem*> items);

protected:
	bool viewportEvent(QEvent *event);
};

class SCRIBUS_API OutlinePalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	OutlinePalette( QWidget* parent );
	
	virtual void changeEvent(QEvent *e);
	
//	void resizeEvent(QResizeEvent *r);
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
	void filterTree(const QString& keyword);
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
	void slotMultiSelect();
	void slotSelect(QTreeWidgetItem* ite, int col);
protected:
	void filterTree();
	void clearPalette();
	void createContextMenu(PageItem *currItem, double mx, double my);
	OutlineWidget* reportDisplay;
	QTreeWidgetItem* freeObjects;
	QTreeWidgetItem* rootObject;
	QTreeWidgetItem* currentObject;
	QLabel* filterLabel;
	QLineEdit* filterEdit;
	ScribusMainWindow* m_MainWindow;
	QPixmap imageIcon;
	QPixmap latexIcon;
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
