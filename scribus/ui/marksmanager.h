#ifndef MARKSMANAGER_H
#define MARKSMANAGER_H

#include <QList>
#include <QTreeWidgetItem>
#include "ui/scrpalettebase.h"
#include "ui_marksmanager.h"
#include "marks.h"
class QEvent;
class  ScribusDoc;

class SCRIBUS_API MarksManager : public ScrPaletteBase, Ui::MarksManager
{
	Q_OBJECT

public:
	explicit MarksManager(QWidget *parent = nullptr, const char *name = "MarksManager");
	~MarksManager();

	void updateListView();

protected:
	void changeEvent(QEvent *e) override;

private:
	ScribusDoc* m_Doc;
	PrefsContext* m_prefs;
	Mark* getMarkFromListView();
	void addListItem(MarkType typeMrk, const QString& typeStr, const QList<Mark *> &marks, int &index);
	QStringList m_expandedItems;
	void storeColaption();
	void restoreColaption();

public slots:
	void setDoc(ScribusDoc *doc);
	void languageChange();
	void paletteChange();
	void handleUpdateRequest(int updateFlags);

private slots:
	void on_UpdateButton_clicked();
	void on_EditButton_clicked();
	void on_DeleteButton_clicked();
	void on_listView_doubleClicked(const QModelIndex &index);
	void on_listView_itemSelectionChanged();
};

#endif // MARKSMANAGER_H
