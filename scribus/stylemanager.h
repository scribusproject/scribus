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

protected slots:
    virtual void slotOk();

private:
	QPtrList<StyleItem> items_;
	StyleItem          *item_;
	QGridLayout        *layout_;
	QTabWidget         *widget_;
	QWidget            *shortcutWidget_;
	QString             currentType_;

	bool                isEditMode_;
	int                 styleWidth_;
	QSize               editSize_;
	QSize               noEditSize_;
	QValueList<int>     editSizes_;
	QValueList<int>     noEditSizes_;

	void insertShortcutPage(QTabWidget *twidget);

private slots:
	void slotApply();
	void slotDelete();
	void slotImport();
	void slotClone();
	void slotNew();
	void slotPageChanged(QWidget*);
	void slotNewType(StyleItem *item);
	void slotStyleChanged();
	void slotNameChanged(const QString& name);
	void slotSelectionChanged();
};

class StyleViewItem : public QListViewItem
{
public:
	StyleViewItem(QListView *view, const QString &text);
	StyleViewItem(QListViewItem *parent, const QString &text, const QString &rootName);
	~StyleViewItem();

	bool isRoot();
	QString parentName();
	QString rootName();

private:
	bool isRoot_;
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
