/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMCELLSTYLES_H
#define SMCELLSTYLES_H

#include <QList>

#include "styleitem.h"
#include "styles/styleset.h"
#include "styles/cellstyle.h"

class SMCellStyleWidget;
class QTabWidget;
class ScribusDoc;

class SMCellStyle : public StyleItem
{
	Q_OBJECT
public:
	/// Constructor.
	SMCellStyle();
	/// Destructor.
	~SMCellStyle();

	/// Reimplemented from StyleItem.
	QTabWidget* widget();
	/// Reimplemented from StyleItem.
	QString typeNamePlural();
	/// Reimplemented from StyleItem.
	QString typeNameSingular();
	/// Reimplemented from StyleItem.
	void setCurrentDoc(ScribusDoc *doc);
	/// Reimplemented from StyleItem.
	QList<StyleName> styles(bool reloadFromDoc = true);
	/// Reimplemented from StyleItem.
	void reload();
	/// Reimplemented from StyleItem.
	void selected(const QStringList &styleNames);
	/// Reimplemented from StyleItem.
	QString fromSelection() const;
	/// Reimplemented from StyleItem.
	void toSelection(const QString &styleName) const;
	/// Reimplemented from StyleItem.
	QString newStyle();
	/// Reimplemented from StyleItem.
	QString newStyle(const QString &fromStyle);
	/// Reimplemented from StyleItem.
	void apply();
	/// Reimplemented from StyleItem.
	void editMode(bool isOn);
	/// Reimplemented from StyleItem.
	bool isDefaultStyle(const QString &styleName) const;
	/// Reimplemented from StyleItem.
	void setDefaultStyle(bool isDefaultStyle);
	/// Reimplemented from StyleItem.
	QString shortcut(const QString &styleName) const;
	/// Reimplemented from StyleItem.
	void setShortcut(const QString &shortcut);
	/// Reimplemented from StyleItem.
	void deleteStyles(const QList<RemoveItem> &removeList);
	/// Reimplemented from StyleItem.
	void nameChanged(const QString &newName);
	/// Reimplemented from StyleItem.
	QString getUniqueName(const QString &name);
	/// Reimplemented from StyleItem.
	void languageChange();
	/// Reimplemented from StyleItem.
	void unitChange();

signals:
	void selectionDirty();

private:
	QTabWidget *m_widget = nullptr;
	SMCellStyleWidget *m_page = nullptr;
	ScribusDoc *m_doc = nullptr;
	StyleSet<CellStyle> m_cachedStyles;
	QList<CellStyle*> m_selection;
	QList<RemoveItem> m_deleted;
	bool m_selectionIsDirty = false;

	void updateStylesCache();
	void setupConnections();
	void removeConnections();

private slots:
	void slotFillColor();
	void slotFillShade();
	void slotParentChanged(const QString &parent);
};

#endif // SMCELLSTYLES_H
