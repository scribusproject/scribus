/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "scraction.h"
#include "stylemanagerbase.h"
#include "scribusstructs.h"
#include <qlistview.h>
#include <qmap.h>
#include <qguardedptr.h>

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
class StyleView;

class StyleManager : public SMBase {
	Q_OBJECT
public:
	StyleManager(QWidget *parent = 0, const char *name = "StyleManager");
	~StyleManager();

	void addStyle(StyleItem *item);

	QMap<QString,Keys> keyMap();

public slots:
	void currentDoc(ScribusDoc *doc);
	void languageChange();

signals:
	void closed();

protected:
	void hideEvent(QHideEvent *e);
	void closeEvent(QCloseEvent *e);
	void showEvent(QShowEvent *e);

private:
	QPtrList<StyleItem> items_;
	StyleItem          *item_;
	QGridLayout        *layout_;
	QTabWidget         *widget_;
	ShortcutWidget     *shortcutWidget_;
	QString             currentType_;
	QPopupMenu         *newPopup_;
	QPopupMenu         *rightClickPopup_;
	StyleView          *styleView;
	QHBoxLayout        *svLayout;

	int                 rcpNewId_;
	int                 rcpDeleteId_;
	int                 rcpEditId_;
	int                 rcpCloneId_;
	int                 rcpToScrapId_;

	bool                isEditMode_;
	QPoint              editPosition_;
	int                 height_;
	int                 width_;
	int                 widthEm_;
	int                 widthLeft_;
	int                 widthRight_;
	QString             rcStyle_;
	QString             rcType_;
	QString             exitEditModeOk_;
	QString             enterEditModeOk_;

	ScribusDoc         *doc_;
	PrefsContext       *prefs_;

	QMap<QString, QGuardedPtr<ScrAction> > styleActions_;

	static const int     NAME_COL     = 0;
	static const int     SHORTCUT_COL = 1;
	static const QString SEPARATOR;

	void insertShortcutPage(QTabWidget *twidget);

	bool nameIsUnique(const QString &name);

	// will be used to map plural style name to it's singular
	QMap<QString, QString> styleClassesPS_;
	QMap<QString, QString> styleClassesSP_;

	// QPair.first will be the type name (null if nothing is selected or
	// if there are styles from more than one type in the selection)
	// QPair.second will be the selected styles
	QPair<QString, QStringList> namesFromSelection();
	void loadType(const QString &name);
	void addNewType(StyleItem *item, bool loadFromDoc = true);
	void createNewStyle(const QString &typeName, const QString &fromParent = QString::null);
	void reloadStyleView(bool loadFromDoc = true); // are the styles loaded from doc or from tmp cache
	bool shortcutExists(const QString &keys);
	void updateActionName(const QString &oldName, const QString &newName);

private slots:
	void slotOk();
	void slotApply();
	void slotDelete();
	void slotImport();
	void slotEdit();
	void slotClone();
	void slotNew();
	void slotNewPopup(int);
	void slotScrap();
	void slotRightClick(QListViewItem *item, const QPoint &point, int col);
	void slotDoubleClick(QListViewItem *item, const QPoint &point, int col);

	void slotNameChanged(const QString& name);
	void slotShortcutChanged(const QString& shortcut);
	void slotSetupWidget();
	void slotApplyStyle(QListViewItem *item);
	void slotDocSelectionChanged();
	void slotDocStylesChanged();

	void slotDirty();
	void slotClean();

	void slotApplyStyle(QString keyString); // keyString == styleClass$$$$styleName
};

class StyleView : public QListView
{
	Q_OBJECT
public:
	StyleView(QWidget *parent);
	~StyleView();
protected:
	void contentsMousePressEvent(QMouseEvent *e);
	void contentsMouseDoubleClickEvent(QMouseEvent *e);
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
	void setKeyMap(const QMap<QString,Keys> &keymap);

	void setShortcut(const QString &shortcut);

	static QString getKeyText(int KeyC);

	void languageChange();

public slots:
	void setKeyText();
	void setNoKey();

signals:
	/**
	 * @brief emitted when a shrotcut is changed.
	 *
	 * Parameter will be QString::null when No key is used
	 */
	void newKey(const QString&);

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
