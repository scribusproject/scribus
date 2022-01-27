/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QPointer>

class QEvent;

#include "ui/scrpalettebase.h"
#include "ui_stylemanager.h"

class  StyleItem;
class  ScribusDoc;
class  ShortcutWidget;
class  ScrAction;
struct Keys;

class SCRIBUS_API StyleManager : public ScrPaletteBase, Ui::StyleManager
{
	Q_OBJECT
public:
	StyleManager(QWidget* parent = nullptr, const char *name = "StyleManager");
	~StyleManager();

	void addStyle(StyleItem *item);
	void updateColorList();

	QMap<QString,Keys> keyMap();

public slots:
	void setDoc(ScribusDoc *doc);
	void languageChange();
	void unitChange();

signals:
	void closed();

protected:
	void changeEvent(QEvent *e) override;
	void hideEvent(QHideEvent *e) override;
	void closeEvent(QCloseEvent *e) override;
	void showEvent(QShowEvent *e) override;

private:
	QList<StyleItem*>   m_items;
	StyleItem          *m_item { nullptr };
	QGridLayout        *m_layout { nullptr };
	QTabWidget         *m_widget { nullptr };
	ShortcutWidget     *m_shortcutWidget { nullptr };
	QString             m_currentType;
	QMenu              *m_newPopup { nullptr };
	QMenu              *m_rightClickPopup { nullptr };

	QAction            *m_rcpNewId { nullptr };
	QAction            *m_rcpDeleteId { nullptr };
	QAction            *m_rcpEditId { nullptr };
	QAction            *m_rcpCloneId { nullptr };
//	QAction            *m_rcpToScrapId;
	ScrAction	*m_selectedStyleAction { nullptr };

	bool                m_isEditMode { true };
	bool                m_isStoryEditMode { false };
	QPoint              m_editPosition;
	QString             m_rcStyle;
	QString             m_rcType;
	QString             m_exitEditModeOk;
	QString             m_enterEditModeOk;
	QString             m_doneText;
	QString             m_editText;

	ScribusDoc         *m_doc { nullptr };
	PrefsContext       *m_prefs { nullptr };

	QMap<QString, QPointer<ScrAction> > m_styleActions;

	static const int     NAME_COL     = 0;
	static const int     SHORTCUT_COL = 1;
	static const QString SEPARATOR;

	void insertShortcutPage(QTabWidget *twidget);

	bool nameIsUnique(const QString &name);

	// will be used to map plural style name to it's singular
	QMap<QString, QString> m_styleClassesPS;
	QMap<QString, QString> m_styleClassesSP;

	// QPair.first will be the type name (null if nothing is selected or
	// if there are styles from more than one type in the selection)
	// QPair.second will be the selected styles
	QPair<QString, QStringList> namesFromSelection();
	void loadType(const QString &name);
	void addNewType(StyleItem *item, bool loadFromDoc = true);
	void createNewStyle(const QString &typeName, const QString &fromParent = QString());
	void reloadStyleView(bool loadFromDoc = true); // are the styles loaded from doc or from tmp cache
	bool shortcutExists(const QString &keys);
	void updateActionName(const QString &oldName, const QString &newName);
	/* QPair.first = type name and QPair.second = style name */
	void setSelection(const QList<QPair<QString, QString> > &selected);
	void setOkButtonText();

	template<class ItemType> ItemType* item();

private slots:
	void slotOk();
	void slotApply();
	void slotDelete();
	void slotImport();
	void slotEdit();
	void slotClone();
	void slotNew();
	//! \brief Open "New Style" widget from "New" GUI button.
	void slotNewPopup(QAction *);
	/*! \brief Overloaded slot for context QMenu handling.
	QActions inserted by QMenu::addAction() can handle triggered() related slots only
	*/
	void slotNewPopup();
	void slotScrap();
	void slotRightClick(const QPoint &point);
	void slotDoubleClick(QTreeWidgetItem * item, int column);

	void slotNameChanged(const QString& name);
	void slotShortcutChanged(const QString& shortcut);
	void slotSetupWidget();

	void slotApplyStyle(QTreeWidgetItem *item);
	void slotApplyStyle(QTreeWidgetItem *newitem, QTreeWidgetItem *olditem);
	void slotApplyStyle(QTreeWidgetItem *item, int column);

	void slotDocSelectionChanged();
	void slotDocStylesChanged();

	void slotDirty();
	void slotClean();

	void slotApplyStyle(const QString& keyString); // keyString == styleClass$$$$styleName
};

#endif
