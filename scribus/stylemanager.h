/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "stylemanagerbase.h"
#include <qlistview.h>

class StyleItem;
class ScribusDoc;
class QHideEvent;
class QGridLayout;
class QTabWidget;
class QVBoxLayout;
class QHBoxLayout;
class QButtonGroup;
class QRadioButton;
class QLabel;
class QPushButton;
class QSize;
class ShortcutWidget;
class QPopupMenu;
class PrefsContext;

class StyleManager : public SMBase {
	Q_OBJECT
public:
	StyleManager(QWidget *parent = 0, const char *name = 0);
	~StyleManager();

	void addStyle(StyleItem *item);

public slots:
	void currentDoc(ScribusDoc *doc);
	void languageChange();

signals:
	void closed();

protected:
	void hideEvent(QHideEvent *e);

private:
	QPtrList<StyleItem> items_;
	StyleItem          *item_;
	QGridLayout        *layout_;
	QTabWidget         *widget_;
	ShortcutWidget     *shortcutWidget_;
	QString             currentType_;
	QPopupMenu         *newPopup_;
	QPopupMenu         *rightClickPopup_;

	int                 rcpDeleteId_;
	int                 rcpCloneId_;
	int                 rcpToScrapId_;

	bool                isEditMode_;
	int                 styleWidth_;
	QValueList<int>     splitterSizes_;
	int                 height_;
	QPoint              editPosition_;

	PrefsContext       *prefs_;

	static const int    NAME_COL     = 0;
	static const int    SHORTCUT_COL = 1;

	void insertShortcutPage(QTabWidget *twidget);

	bool nameIsUnique(const QString &name);

	// QPair.first will be the type name (null if nothing is selected or
	// if there are styles from more than one type in the selection)
	// QPair.second will be the selected styles
	QPair<QString, QStringList> namesFromSelection();
	void loadType(const QString &name);
	void addNewType(StyleItem *item, bool loadFromDoc = true);
	void createNewStyle(const QString &typeName, const QString &fromParent = QString::null);
	void reloadStyleView(bool loadFromDoc = true); // are the styles loaded from doc or from tmp cache

private slots:
	void slotOk();
	void slotApply();
	void slotDelete();
	void slotImport();
	void slotClone();
	void slotNew();
	void slotNewPopup(int);
	void slotScrap();
	void slotRightClick(QListViewItem *item, const QPoint &point, int col);

	void slotNameChanged(const QString& name);
	void slotSetupWidget();
	void slotApplyStyle(QListViewItem *item);
	void slotDocSelectionChanged();

	void slotDirty();
	void slotClean();
};

class StyleViewItem : public QListViewItem
{
public:
	StyleViewItem(QListView *view, const QString &text);
	StyleViewItem(QListViewItem *parent, const QString &text, const QString &rootName);
	~StyleViewItem();

	void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

	bool isRoot();
	QString parentName();
	QString rootName();
	void setDirty(bool isDirty);
	bool isDirty();

private:
	bool isRoot_;
	bool isDirty_;
	QString parentName_;
	QString rootName_;
};

class ShortcutWidget : public QWidget
{
	Q_OBJECT
public:
	ShortcutWidget(QWidget *parent = 0, const char *name = 0);
	~ShortcutWidget();

	bool event( QEvent* ev );
	void keyPressEvent(QKeyEvent *k);
	void keyReleaseEvent(QKeyEvent *k);

	static QString getKeyText(int KeyC);

	void languageChange();

public slots:
	void setKeyText();
	void setNoKey();

protected:
	QVBoxLayout* keyManagerLayout;
	QGridLayout* keyGroupLayout;
	QHBoxLayout* okCancelLayout;

	QButtonGroup* keyGroup;
	QRadioButton* noKey;
	QRadioButton* userDef;
	QLabel* keyDisplay;
	QPushButton* setKeyButton;

	int keyCode;
	QString Part0;
	QString Part1;
	QString Part2;
	QString Part3;
	QString Part4;
};

#endif
