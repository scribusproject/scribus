/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMTABLESTYLES_H
#define SMTABLESTYLES_H

#include <QList>

#include "styleitem.h"
#include "styles/styleset.h"
#include "styles/tablestyle.h"

class SMTableStyleWidget;
class QTabWidget;
class ScribusDoc;

class SMTableStyle : public StyleItem
{
	Q_OBJECT
public:
	/// Constructor.
	SMTableStyle();
	/// Destructor.
	~SMTableStyle() override;

	/// Reimplemented from StyleItem.
	QTabWidget* widget() override;
	/// Reimplemented from StyleItem.
	QString typeNamePlural() override;
	/// Reimplemented from StyleItem.
	QString typeNameSingular() override;
	/// Reimplemented from StyleItem.
	void setCurrentDoc(ScribusDoc *doc) override;
	/// Reimplemented from StyleItem.
	QList<StyleName> styles(bool reloadFromDoc = true) override;
	/// Reimplemented from StyleItem.
	void reload() override;
	/// Reimplemented from StyleItem.
	void selected(const QStringList &styleNames) override;
	/// Reimplemented from StyleItem.
	QString fromSelection() const override;
	/// Reimplemented from StyleItem.
	void toSelection(const QString &styleName) const override;
	/// Reimplemented from StyleItem.
	QString newStyle() override;
	/// Reimplemented from StyleItem.
	QString newStyle(const QString &fromStyle) override;
	/// Reimplemented from StyleItem.
	void apply() override;
	/// Reimplemented from StyleItem.
	void editMode(bool isOn) override;
	/// Reimplemented from StyleItem.
	bool isDefaultStyle(const QString &styleName) const override;
	/// Reimplemented from StyleItem.
	void setDefaultStyle(bool isDefaultStyle) override;
	/// Reimplemented from StyleItem.
	QString shortcut(const QString &styleName) const override;
	/// Reimplemented from StyleItem.
	void setShortcut(const QString &shortcut) override;
	/// Reimplemented from StyleItem.
	void deleteStyles(const QList<RemoveItem> &removeList) override;
	/// Reimplemented from StyleItem.
	void nameChanged(const QString &newName) override;
	/// Reimplemented from StyleItem.
	QString getUniqueName(const QString &name) override;
	/// Reimplemented from StyleItem.
	void languageChange() override;
	/// Reimplemented from StyleItem.
	void unitChange() override;

signals:
	void selectionDirty();

private:
	QTabWidget *m_widget = nullptr;
	SMTableStyleWidget *m_page = nullptr;
	ScribusDoc *m_doc = nullptr;
	StyleSet<TableStyle> m_cachedStyles;
	QList<TableStyle*> m_selection;
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

#endif // SMTABLESTYLES_H
