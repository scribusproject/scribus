/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QTabWidget>
#include <QMessageBox>

#include "prefsmanager.h"
#include "scribus.h"
#include "smcellstyle.h"
#include "smcellstylewidget.h"
#include "ui/scmessagebox.h"
#include "util.h"

SMCellStyle::SMCellStyle()
{
	m_widget = new QTabWidget();
	Q_CHECK_PTR(m_widget);
	m_widget->setContentsMargins(5, 5, 5, 5);

	m_page = new SMCellStyleWidget();
	Q_CHECK_PTR(m_page);
	
	m_widget->addTab(m_page, tr("Properties"));
}

SMCellStyle::~SMCellStyle()
{
	delete m_page;
	delete m_widget;
	m_page = nullptr;
	m_widget = nullptr;
}

QTabWidget* SMCellStyle::widget()
{
	return m_widget;
}

QString SMCellStyle::typeNamePlural()
{
	return tr("Cell Styles");
}

QString SMCellStyle::typeNameSingular()
{
	return tr("Cell Style");
}

void SMCellStyle::setCurrentDoc(ScribusDoc *doc)
{
	m_doc = doc;

	if (m_page)
		m_page->setDoc(doc);

	if (!m_doc)
	{
		removeConnections();
		m_selection.clear();
		m_tmpStyles.clear();
	}
}

QList<StyleName> SMCellStyle::styles(bool reloadFromDoc)
{
	QList<StyleName> stylesList;

	if (!m_doc)
		return stylesList; // No document available.

	if (reloadFromDoc)
		updateStylesCache(); // Update cache.

	// Return a list of names of cached styles.
	for (int i = 0; i < m_tmpStyles.count(); ++i)
	{
		if (m_tmpStyles[i].hasName())
		{
			QString styleName(m_tmpStyles[i].displayName());
			QString parentName;

			if (m_tmpStyles[i].hasParent())
			{
				const BaseStyle* parentStyle = m_tmpStyles[i].parentStyle();
				if (parentStyle)
					parentName = parentStyle->displayName();
			}

			stylesList << StyleName(styleName, parentName);
		}
	}
	std::sort(stylesList.begin(), stylesList.end(), sortingQPairOfStrings);

	return stylesList;
}

void SMCellStyle::reload()
{
	updateStylesCache();
}

void SMCellStyle::selected(const QStringList &styleNames)
{
	m_selection.clear();
	m_selectionIsDirty = false;
	removeConnections();
	QList<CellStyle> cellStyles;

	m_tmpStyles.invalidate();

	for (int i = 0; i < m_tmpStyles.count(); ++i)
		cellStyles << m_tmpStyles[i];

	for (const QString& styleName : styleNames)
	{
		int index = m_tmpStyles.find(styleName);
		// FIXME: #7133: Use .isDefaultStyle() instead here rather than relying on tr text comparison
		if (index < 0 && styleName == CommonStrings::trDefaultCellStyle)
			index = m_tmpStyles.find(CommonStrings::DefaultCellStyle);
		if (index > -1)
			m_selection.append(&m_tmpStyles[index]);
	}
	m_page->show(m_selection, cellStyles, PrefsManager::instance().appPrefs.docSetupPrefs.language, m_doc->unitIndex());
	setupConnections();
}

QString SMCellStyle::fromSelection() const
{
	// TODO: Implement this once we have cell items.
	return QString();
}

void SMCellStyle::toSelection(const QString &styleName) const
{
	if (!m_doc)
		return; // nowhere to apply or no doc

	QString realName = styleName;
	int styleIndex = m_tmpStyles.find(styleName);
	if (styleIndex < 0 && (styleName == CommonStrings::trDefaultCellStyle))
	{
		styleIndex = m_tmpStyles.find(CommonStrings::DefaultCellStyle);
		if (styleIndex >= 0)
			realName = CommonStrings::DefaultCellStyle;
	}
	if (styleIndex >= 0)
	{
		m_doc->itemSelection_SetNamedCellStyle(realName);
	}
}

QString SMCellStyle::newStyle()
{
	Q_ASSERT(m_doc);

	QString name = getUniqueName(tr("New Style"));
	CellStyle style;
	style.setDefaultStyle(false);
	style.setName(name);
	m_tmpStyles.create(style);
	return name;
}

QString SMCellStyle::newStyle(const QString &fromStyle)
{
	// #7179, do our name switch yet again to handle this properly for default styles
	// FIXME: use isDefaultStyle somehow
	QString fromStyleName(fromStyle);
	if (fromStyle == CommonStrings::trDefaultCellStyle)
		fromStyleName = CommonStrings::DefaultCellStyle;

	Q_ASSERT(m_tmpStyles.resolve(fromStyleName));
	if (!m_tmpStyles.resolve(fromStyleName))
		return QString();

	// Copy the style with name constructed from the original name.
	QString styleName(getUniqueName(fromStyle));
	CellStyle cellStyle(m_tmpStyles.get(fromStyleName));
	cellStyle.setDefaultStyle(false);
	cellStyle.setName(styleName);
	cellStyle.setShortcut(QString());
	m_tmpStyles.create(cellStyle);

	return styleName;
}

void SMCellStyle::apply()
{
	if (!m_doc)
		return; // No document available.

	// Handle replacement of deleted styles.
	QMap<QString, QString> replacement;
	for (int i = 0; i < m_deleted.count(); ++i)
	{
		if (m_deleted[i].first == m_deleted[i].second)
			continue; // Nothing to do.
		replacement[m_deleted[i].first] = m_deleted[i].second;
	}

	m_doc->redefineCellStyles(m_tmpStyles, false);
	m_doc->replaceCellStyles(replacement);

	m_deleted.clear(); // Deletion done at this point.

	m_doc->scMW()->requestUpdate(reqTableStylesUpdate);

	m_doc->changed();
	m_doc->changedPagePreview();
}

void SMCellStyle::editMode(bool isOn)
{
	if (isOn)
		updateStylesCache();
}

bool SMCellStyle::isDefaultStyle(const QString &styleName) const
{
	int index = m_tmpStyles.find(styleName);
	bool result = false;
	if (index > -1)
		result = m_tmpStyles[index].isDefaultStyle();
	else
	{
		if (CommonStrings::trDefaultCellStyle == styleName)
		{
			index = m_tmpStyles.find(CommonStrings::DefaultCellStyle);
			if (index > -1)
				result = m_tmpStyles[index].isDefaultStyle();
		}
	}
	return result;
}

void SMCellStyle::setDefaultStyle(bool isDefaultStyle)
{
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	m_selection[0]->setDefaultStyle(isDefaultStyle);
	
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

QString SMCellStyle::shortcut(const QString &styleName) const
{
	QString result;
	int index = m_tmpStyles.find(styleName);
	if (index > -1)
		result = m_tmpStyles[index].shortcut();
	else
	{
		// FIXME: Use isDefaultStyle somehow.
		if (CommonStrings::trDefaultCellStyle == styleName)
		{
			index = m_tmpStyles.find(CommonStrings::DefaultCellStyle);
			if (index > -1)
				result = m_tmpStyles[index].shortcut();
		}
	}
	return result;
}

void SMCellStyle::setShortcut(const QString &shortcut)
{
	Q_ASSERT(m_selection.count() == 1);
	if (m_selection.count() != 1)
		return;

	m_selection[0]->setShortcut(shortcut);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCellStyle::deleteStyles(const QList<RemoveItem> &removeList)
{
	for (const RemoveItem& removeItem : removeList)
	{
		for (int i = 0; i < m_selection.count(); ++i)
		{
			if (m_selection[i]->name() == removeItem.first)
			{
				m_selection.removeAt(i);
				break;
			}
		}

		int index = m_tmpStyles.find(removeItem.first);
		if (index > -1)
			m_tmpStyles.remove(index);
		m_deleted << removeItem;
	}

	// Check other styles and replace inherited styles if necessary
	for (int i = 0; i < m_tmpStyles.count(); ++i)
	{
		CellStyle& cellStyle = m_tmpStyles[i];
		QString parentName = cellStyle.parent();
		if (parentName.isEmpty())
			continue;

		QString replacementName = parentName;
		for (int j = 0; j < removeList.count(); ++j)
		{
			if (removeList.at(j).first == parentName)
			{
				replacementName = removeList.at(j).second;
				break;
			}
		}

		if (replacementName == parentName)
			continue;
		if (replacementName == CommonStrings::trDefaultCellStyle)
			replacementName = CommonStrings::DefaultCellStyle;
		if (!cellStyle.canInherit(replacementName))
			replacementName = QString();
		if (!replacementName.isEmpty() && (m_tmpStyles.find(replacementName) < 0))
			replacementName = QString();
		cellStyle.setParent(replacementName);
	}
}

void SMCellStyle::nameChanged(const QString &newName)
{
	// Save the old name.
	QString oldName(m_selection[0]->name());

	// Make a copy of the old style but with new name.
	CellStyle newStyle(*m_selection[0]);
	newStyle.setName(newName);
	m_tmpStyles.create(newStyle);

	// Select the new style.
	m_selection.clear();
	m_selection.append(&m_tmpStyles[m_tmpStyles.find(newName)]);

	// Remove old style from cache.
	for (int j = 0; j < m_tmpStyles.count(); ++j)
	{
		int index = m_tmpStyles.find(oldName);
		if (index > -1)
		{
			m_tmpStyles.remove(index);
			break;
		}
	}

	// Set parent references to old style to new style.
	for (int j = 0; j < m_tmpStyles.count(); ++j)
	{
		if (m_tmpStyles[j].parent() == oldName)
			m_tmpStyles[j].setParent(newName);
	}

	// Update the deleted list to reflect the name change.
	for (auto it = m_deleted.begin(); it != m_deleted.end(); ++it)
	{
		if ((*it).second == oldName)
		{
			oldName = (*it).first;
			m_deleted.erase(it);
			break;
		}
	}
	m_deleted.append(RemoveItem(oldName, newName));

	// Mark selection as dirty.
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

QString SMCellStyle::getUniqueName(const QString &name)
{
	return m_tmpStyles.getUniqueCopyName(name);
}

void SMCellStyle::languageChange()
{
	if (m_widget && m_page)
	{
		m_widget->setTabText(m_widget->indexOf(m_page), tr("Properties"));
		m_page->languageChange();
	}
}

void SMCellStyle::unitChange()
{
	// Unimplemented.
}

void SMCellStyle::updateStylesCache()
{
	if (!m_doc)
		return; // No document available.
	m_selection.clear();
	m_tmpStyles.clear();
	m_deleted.clear();
	m_tmpStyles.redefine(m_doc->cellStyles(), true);
}

void SMCellStyle::setupConnections()
{
	if (!m_page)
		return;
	connect(m_page->buttonFillColor->colorButton, SIGNAL(changed()), this, SLOT(slotFillColor()));
	connect(m_page->buttonFillColor->parentButton, SIGNAL(clicked()), this, SLOT(slotFillColor()));
	connect(m_page->parentCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(slotParentChanged(QString)));
	connect(m_page->cellPaddingWidget, SIGNAL(valuesChanged(MarginStruct)), this, SLOT(slotCellPaddingChanged(MarginStruct)));
	connect(m_page->verticalAlignmentSelect, SIGNAL(State(int)), this, SLOT(slotVerticalAlignmentChanged()));
	connect(m_page->verticalAlignmentSelect->parentButton, SIGNAL(clicked()), this, SLOT(slotVerticalAlignmentChanged()));
	connect(m_page, SIGNAL(bordersChanged(TableSides, TableBorder)), this, SLOT(slotBordersChanged(TableSides, TableBorder)));
}

void SMCellStyle::removeConnections()
{
	if (!m_page)
		return;
	disconnect(m_page->buttonFillColor->colorButton, SIGNAL(changed()), this, SLOT(slotFillColor()));
	disconnect(m_page->buttonFillColor->parentButton, SIGNAL(clicked()), this, SLOT(slotFillColor()));
	disconnect(m_page->parentCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(slotParentChanged(QString)));
	disconnect(m_page->cellPaddingWidget, SIGNAL(valuesChanged(MarginStruct)), this, SLOT(slotCellPaddingChanged(MarginStruct)));
	disconnect(m_page->verticalAlignmentSelect, SIGNAL(State(int)), this, SLOT(slotVerticalAlignmentChanged()));
	disconnect(m_page->verticalAlignmentSelect->parentButton, SIGNAL(clicked()), this, SLOT(slotVerticalAlignmentChanged()));
	disconnect(m_page, SIGNAL(bordersChanged(TableSides, TableBorder)), this, SLOT(slotBordersChanged(TableSides, TableBorder)));
}

void SMCellStyle::slotFillColor()
{
	if (m_page->buttonFillColor->useParentValue())
	{
		for (int i = 0; i < m_selection.count(); ++i)
		{
			m_selection[i]->resetFillColor();
			m_selection[i]->resetFillShade();
		}
	}
	else
	{
		QString col = m_page->buttonFillColor->colorButton->colorName();
		int fs = m_page->buttonFillColor->colorButton->colorData().Shade;
		for (int i = 0; i < m_selection.count(); ++i)
		{
			m_selection[i]->setFillColor(col);
			m_selection[i]->setFillShade(fs);
		}
	}
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCellStyle::slotParentChanged(const QString &parent)
{
	Q_ASSERT(!parent.isNull());

	bool  loop = false, parentLoop = false;
	const BaseStyle* parentStyle = (!parent.isEmpty()) ? m_tmpStyles.resolve(parent) : nullptr;
	QStringList  sel;

	for (int i = 0; i < m_selection.count(); ++i)
	{
		loop = false;
		// Check if setting parent won't create a loop
		const BaseStyle* pStyle = parentStyle;
		while (pStyle)
		{
			if (pStyle->hasParent() && (pStyle->parent() == m_selection[i]->name()))
			{
				loop = parentLoop = true;
				break;
			}
			pStyle = pStyle->hasParent() ? pStyle->parentStyle() : nullptr;
		}
		if (!loop)
		{
			m_selection[i]->erase();
			m_selection[i]->setParent(parent);
		}
		sel << m_selection[i]->name();
	}

	if (parentLoop)
		ScMessageBox::warning(this->widget(), CommonStrings::trWarning, tr("Setting that style as parent would create an infinite loop."));

	selected(sel);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCellStyle::slotCellPaddingChanged(const MarginStruct &padding)
{
	for (int i = 0; i < m_selection.count(); ++i)
	{
		m_selection[i]->setLeftPadding(padding.left());
		m_selection[i]->setRightPadding(padding.right());
		m_selection[i]->setTopPadding(padding.top());
		m_selection[i]->setBottomPadding(padding.bottom());
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCellStyle::slotVerticalAlignmentChanged()
{
	if (m_page->verticalAlignmentSelect->useParentValue())
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetVerticalAlignment();
	}
	else
	{
		int alignment = m_page->verticalAlignmentSelect->getStyle();
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->setVerticalAlignment(alignment);
	}

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCellStyle::slotBordersChanged(TableSides sides, const TableBorder& border)
{
	for (int i = 0; i < m_selection.count(); ++i)
	{
		if (sides & TableSide::Left)
			m_selection[i]->setLeftBorder(border);
		if (sides & TableSide::Right)
			m_selection[i]->setRightBorder(border);
		if (sides & TableSide::Top)
			m_selection[i]->setTopBorder(border);
		if (sides & TableSide::Bottom)
			m_selection[i]->setBottomBorder(border);
	}
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}
