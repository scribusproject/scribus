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
#include "scdockpalette.h"

class ScribusMainWindow;
class ScribusDoc;
class PageItem;
class ScPage;

class SCRIBUS_API OutlineTreeItem : public QTreeWidgetItem
{
public:
	OutlineTreeItem(OutlineTreeItem* parent, OutlineTreeItem* after);
	OutlineTreeItem(QTreeWidget* parent, OutlineTreeItem* after);
	~OutlineTreeItem() {};
	PageItem *PageItemObject;
	ScPage *PageObject;
	ScribusDoc *DocObject;
	int LayerID;
	int type;
};

class SCRIBUS_API OutlineWidget : public QTreeWidget
{
public:
	OutlineWidget(QWidget* parent);
	~OutlineWidget() {};

	void selectItems(QList<QTreeWidgetItem*> items);

protected:
	void dropEvent(QDropEvent *e);
	bool viewportEvent(QEvent *event);
};

class SCRIBUS_API OutlinePalette : public ScDockPalette
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
	QTreeWidgetItem* getListItem(int SNr, PageItem *Nr);
	void setItemIcon(QTreeWidgetItem *item, PageItem *pgItem);
	void parseSubGroup(OutlineTreeItem* object, QList<PageItem*> *subGroupList, int itemType, ScPage *a);
	void buildReopenVals();

public slots:
	void BuildTree(bool storeVals = true);
	void filterTree(const QString& keyword);
	void languageChange();
	void slotShowSelect(uint SNr, PageItem *Nr);
	void setPaletteShown(bool);
	void slotRightClick(QPoint point);
	void setActiveLayer(int layerID);
	void setLayerVisible(int layerID);
	void setLayerLocked(int layerID);
	void setLayerPrintable(int layerID);

signals:
	void ToggleAllPalettes();
	void selectElementByItem(PageItem *, bool);
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
	QWidget* containerWidget;
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
	QPixmap tableIcon;
	QPixmap buttonIcon;
	QPixmap textFieldIcon;
	QPixmap checkBoxIcon;
	QPixmap comboBoxIcon;
	QPixmap listBoxIcon;
	QPixmap annotTextIcon;
	QPixmap annotLinkIcon;
	QPixmap annot3DIcon;
	bool selectionTriggered;
	ScribusDoc *currDoc;
};

#endif // TREE_H
