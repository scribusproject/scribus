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
#include "smcellstyle.h"
#include "smcellstylewidget.h"

SMCellStyle::SMCellStyle() : StyleItem(),m_widget(0), m_page(0), m_doc(0)
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
	m_page = 0;
	m_widget = 0;
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

	if (!m_doc)
	{
		removeConnections();
		m_selection.clear();
		m_cachedStyles.clear();
	}

	if (m_doc)
	{
		if (m_page)
		{
			m_page->fillFillColorCombo(m_doc->PageColors);
		}
	}
	else
	{
		removeConnections();
		m_selection.clear();
		m_cachedStyles.clear();
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
	for (int i = 0; i < m_cachedStyles.count(); ++i)
	{
		if (m_cachedStyles[i].hasName())
		{
			QString styleName(m_cachedStyles[i].displayName());
			QString parentName(QString::null);

			if (m_cachedStyles[i].hasParent())
			{
				const Style* parentStyle = m_cachedStyles[i].parentStyle();
				if (parentStyle)
					parentName = parentStyle->displayName();
			}

			stylesList << StyleName(styleName, parentName);
		}
	}

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

	m_cachedStyles.invalidate();

	for (int i = 0; i < m_cachedStyles.count(); ++i)
		cellStyles << m_cachedStyles[i];

	foreach (QString styleName, styleNames)
	{
		int index = m_cachedStyles.find(styleName);
		// FIXME: #7133: Use .isDefaultStyle() instead here rather than relying on tr text comparison
		if (index < 0 && styleName == CommonStrings::trDefaultCellStyle)
			index = m_cachedStyles.find(CommonStrings::DefaultCellStyle);
		if (index > -1)
			m_selection.append(&m_cachedStyles[index]);
	}
	m_page->show(m_selection, cellStyles, PrefsManager::instance()->appPrefs.hyphPrefs.Language, m_doc->unitIndex());
	setupConnections();
}

QString SMCellStyle::fromSelection() const
{
	// TODO: Implement this once we have cell items.
	return QString::null;
}

void SMCellStyle::toSelection(const QString &styleName) const
{
	// TODO: Implement this once we have cell items.
}

QString SMCellStyle::newStyle()
{
	Q_ASSERT(m_doc);

	QString name = getUniqueName(tr("New Style"));
	CellStyle style;
	style.setDefaultStyle(false);
	style.setName(name);
	m_cachedStyles.create(style);
	return name;
}

QString SMCellStyle::newStyle(const QString &fromStyle)
{
	// #7179, do our name switch yet again to handle this properly for default styles
	// FIXME: use isDefaultStyle somehow
	QString fromStyleName(fromStyle);
	if (fromStyle == CommonStrings::trDefaultCellStyle)
		fromStyleName = CommonStrings::DefaultCellStyle;

	Q_ASSERT(m_cachedStyles.resolve(fromStyleName));
	if (!m_cachedStyles.resolve(fromStyleName))
		return QString::null;

	// Copy the style with name constructed from the original name.
	QString styleName(getUniqueName(tr("Clone of %1").arg(fromStyle)));
	CellStyle cellStyle(m_cachedStyles.get(fromStyleName));
	cellStyle.setDefaultStyle(false);
	cellStyle.setName(styleName);
	cellStyle.setShortcut(QString::null);
	m_cachedStyles.create(cellStyle);

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

	m_doc->redefineCellStyles(m_cachedStyles, false);
	m_doc->replaceCellStyles(replacement);

	m_deleted.clear(); // Deletion done at this point.

	// TODO: We should probably have something similar to this for tables/cells.
	//m_doc->scMW()->requestUpdate(reqTextStylesUpdate);

	m_doc->changed();
}

void SMCellStyle::editMode(bool isOn)
{
	if (isOn)
		updateStylesCache();
}

bool SMCellStyle::isDefaultStyle(const QString &styleName) const
{
	int index = m_cachedStyles.find(styleName);
	bool result = false;
	if (index > -1)
		result = m_cachedStyles[index].isDefaultStyle();
	else
	{
		if (CommonStrings::trDefaultCellStyle == styleName)
		{
			index = m_cachedStyles.find(CommonStrings::DefaultCellStyle);
			if (index > -1)
				result = m_cachedStyles[index].isDefaultStyle();
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
	QString result(QString::null);
	int index = m_cachedStyles.find(styleName);
	if (index > -1)
		result = m_cachedStyles[index].shortcut();
	else
	{
		// FIXME: Use isDefaultStyle somehow.
		if (CommonStrings::trDefaultCellStyle == styleName)
		{
			index = m_cachedStyles.find(CommonStrings::DefaultCellStyle);
			if (index > -1)
				result = m_cachedStyles[index].shortcut();
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
	foreach (RemoveItem removeItem, removeList)
	{
		for (int i = 0; i < m_selection.count(); ++i)
		{
			if (m_selection[i]->name() == removeItem.first)
			{
				m_selection.removeAt(i);
				break;
			}
		}

		int index = m_cachedStyles.find(removeItem.first);
		if (index > -1)
			m_cachedStyles.remove(index);
		m_deleted << removeItem;
	}
}

void SMCellStyle::nameChanged(const QString &newName)
{
	// Save the old name.
	QString oldName(m_selection[0]->name());

	// Make a copy of the old style but with new name.
	CellStyle newStyle(*m_selection[0]);
	newStyle.setName(newName);
	m_cachedStyles.create(newStyle);

	// Select the new style.
	m_selection.clear();
	m_selection.append(&m_cachedStyles[m_cachedStyles.find(newName)]);

	// Remove old style from cache.
	for (int j = 0; j < m_cachedStyles.count(); ++j)
	{
		int index = m_cachedStyles.find(oldName);
		if (index > -1)
		{
			m_cachedStyles.remove(index);
			break;
		}
	}

	// Set parent references to old style to new style.
	for (int j = 0; j < m_cachedStyles.count(); ++j)
	{
		if (m_cachedStyles[j].parent() == oldName)
			m_cachedStyles[j].setParent(newName);
	}

	// Update the deleted list to reflect the name change.
	QList<RemoveItem>::iterator it;
	for (it = m_deleted.begin(); it != m_deleted.end(); ++it)
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
	int id = 0;
	bool done = false;
	QString s(name);

	while (!done)
	{
start:
		++id;
		for (int i = 0; i < m_cachedStyles.count(); ++i)
		{
			if (m_cachedStyles[i].name() == s)
			{
				s = tr("%1 (%2)", "This for unique name when creating "
						"a new character style. %1 will be the name "
								"of the style and %2 will be a number forming "
								"a style name like: New Style (2)").arg(name).arg(id);
				goto start;
			}
		}
		done = true;
	}

	return s;
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
	m_cachedStyles.clear();
	m_deleted.clear();
	m_cachedStyles.redefine(m_doc->cellStyles(), true);
}

void SMCellStyle::setupConnections()
{
	if (!m_page)
		return;
	connect(m_page->fillColor, SIGNAL(activated(const QString&)), this, SLOT(slotFillColor()));
	connect(m_page->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	connect(m_page->parentCombo, SIGNAL(activated(const QString&)), this, SLOT(slotParentChanged(const QString&)));
}

void SMCellStyle::removeConnections()
{
	if (!m_page)
		return;
	disconnect(m_page->fillColor, SIGNAL(activated(const QString&)), this, SLOT(slotFillColor()));
	disconnect(m_page->fillShade_, SIGNAL(clicked()), this, SLOT(slotFillShade()));
	disconnect(m_page->parentCombo, SIGNAL(activated(const QString&)), this, SLOT(slotParentChanged(const QString&)));
}

void SMCellStyle::slotFillColor()
{
	if (m_page->fillColor->useParentValue())
	{
		for (int i = 0; i < m_selection.count(); ++i)
			m_selection[i]->resetFillColor();
	}
	else
	{
		QString col(m_page->fillColor->currentText());
		for (int i = 0; i < m_selection.count(); ++i)
		{
			m_selection[i]->setFillColor(col);
		}
	}
	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}

void SMCellStyle::slotFillShade()
{
	if (m_page->fillShade_->useParentValue())
	{
		for (int i = 0; i < m_selection.count(); ++i)
		{
			m_selection[i]->resetFillShade();
		}
	}
	else
	{
		int fs = m_page->fillShade_->getValue();
		for (int i = 0; i < m_selection.count(); ++i)
		{
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
	const Style* parentStyle = (!parent.isEmpty()) ? m_cachedStyles.resolve(parent) : NULL;
	QStringList  sel;

	for (int i = 0; i < m_selection.count(); ++i)
	{
		loop = false;
		// Check if setting parent won't create a loop
		const Style* pStyle = parentStyle;
		while (pStyle)
		{
			if (pStyle->hasParent() && (pStyle->parent() == m_selection[i]->name()))
			{
				loop = parentLoop = true;
				break;
			}
			pStyle = pStyle->hasParent() ? pStyle->parentStyle() : NULL;
		}
		if (!loop)
		{
			m_selection[i]->erase();
			m_selection[i]->setParent(parent);
		}
		sel << m_selection[i]->name();
	}

	if (parentLoop)
		QMessageBox::warning(this->widget(), CommonStrings::trWarning, tr("Setting that style as parent would create an infinite loop."), CommonStrings::tr_OK);

	selected(sel);

	if (!m_selectionIsDirty)
	{
		m_selectionIsDirty = true;
		emit selectionDirty();
	}
}
