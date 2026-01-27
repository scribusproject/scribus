/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QInputDialog>
#include <QListWidget>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QtAlgorithms>

#include "iconmanager.h"
#include "prefs_hyphenator.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_Hyphenator::Prefs_Hyphenator(QWidget* parent, ScribusDoc* /*doc*/)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Hyphenation");
	m_icon = "pref-hyphenator";

	exceptionAddButton->setIcon(IconManager::instance().loadIcon("list-add"));
	exceptionRemoveButton->setIcon(IconManager::instance().loadIcon("list-remove"));
	exceptionRemoveButton->setEnabled(false);
	ignoreAddButton->setIcon(IconManager::instance().loadIcon("list-add"));
	ignoreRemoveButton->setIcon(IconManager::instance().loadIcon("list-remove"));
	ignoreRemoveButton->setEnabled(false);
	connect(ignoreAddButton, SIGNAL(clicked()), this, SLOT(addToIgnoreList()));
	connect(ignoreListWidget, &QListWidget::itemDoubleClicked, this, &Prefs_Hyphenator::editIgnoreListItem);
	connect(ignoreListWidget, &QListWidget::itemChanged, this, &Prefs_Hyphenator::ignoreListItemChanged);
	connect(ignoreRemoveButton, SIGNAL(clicked()), this, SLOT(removeFromIgnoreList()));
	connect(ignoreListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enableIgnoreButtons()));
	connect(exceptionAddButton, SIGNAL(clicked()), this, SLOT(addToExceptionList()));
	connect(exceptionListWidget, &QListWidget::itemDoubleClicked, this, &Prefs_Hyphenator::editExceptionListItem);
	connect(exceptionListWidget, &QListWidget::itemChanged, this, &Prefs_Hyphenator::exceptionListItemChanged);
	connect(exceptionRemoveButton, SIGNAL(clicked()), this, SLOT(removeFromExceptionList()));
	connect(exceptionListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enableExceptionButtons()));
}

void Prefs_Hyphenator::languageChange()
{
	// No need to do anything here, the UI language cannot change while prefs dialog is opened
}

void Prefs_Hyphenator::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	hyphSuggestionsCheckBox->setChecked(!prefsData->hyphPrefs.Automatic);
	hyphAutoCheckBox->setChecked(prefsData->hyphPrefs.AutoCheck);
	ignoreListWidget->addItems(prefsData->hyphPrefs.ignoredWords.values());
	ignoreListWidget->sortItems();
	exceptionListWidget->addItems(prefsData->hyphPrefs.specialWords.values());
	exceptionListWidget->sortItems();
}

void Prefs_Hyphenator::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->hyphPrefs.Automatic = !hyphSuggestionsCheckBox->isChecked();
	prefsData->hyphPrefs.AutoCheck = hyphAutoCheckBox->isChecked();
	QSet<QString> ignoreList;
	for (int row = 0; row < ignoreListWidget->count(); row++)
	{
		ignoreList.insert(ignoreListWidget->item(row)->text());
	}
	prefsData->hyphPrefs.ignoredWords = ignoreList;
	QHash<QString, QString> exceptionList;
	for (int row = 0; row < exceptionListWidget->count(); row++)
	{
		exceptionList.insert(exceptionListWidget->item(row)->text().remove("-"), exceptionListWidget->item(row)->text());
	}
	prefsData->hyphPrefs.specialWords = exceptionList;
}

void Prefs_Hyphenator::addToIgnoreList()
{
	bool ok;
	QString text = QInputDialog::getMultiLineText(this, tr("Ignore List"), tr("Add new Entries"), "", &ok);
	text = text.trimmed();
	if (!ok || text.isEmpty())
	{
		return;
	}
	for (QString& word: text.split("\n"))
	{
		word = word.trimmed();
		if (ignoreListWidget->findItems(word, Qt::MatchExactly).isEmpty())
			ignoreListWidget->addItem(word);
	}
	ignoreListWidget->sortItems();
}

void Prefs_Hyphenator::editIgnoreListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Ignore List"), tr("Edit Entry"), QLineEdit::Normal, ignoreListWidget->currentItem()->text(), &ok);
	if (ok && !text.isEmpty())
	{
		if (ignoreListWidget->findItems(text, Qt::MatchExactly).isEmpty())
			ignoreListWidget->currentItem()->setText(text);
		ignoreListWidget->sortItems();
	}
}

void Prefs_Hyphenator::editIgnoreListItem(QListWidgetItem* item)
{
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	ignoreListWidget->editItem(item);
}


void Prefs_Hyphenator::ignoreListItemChanged(QListWidgetItem*)
{
	ignoreListWidget->sortItems();
}

void Prefs_Hyphenator::removeFromIgnoreList()
{
	qDeleteAll(ignoreListWidget->selectedItems());
}

void Prefs_Hyphenator::enableIgnoreButtons()
{
	ignoreRemoveButton->setEnabled(true);
}

void Prefs_Hyphenator::addToExceptionList()
{
	bool ok;
	QString text = QInputDialog::getMultiLineText(this, tr("Exception List"), tr("Add new Entries"), "", &ok);
	text = text.trimmed();
	if (!ok || text.isEmpty())
	{
		return;
	}
	for (QString& word: text.split("\n"))
	{
		word = word.trimmed();
		if (exceptionListWidget->findItems(word, Qt::MatchExactly).isEmpty())
			exceptionListWidget->addItem(word);
	}
	exceptionListWidget->sortItems();
}

void Prefs_Hyphenator::editExceptionListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Exception List"), tr("Edit Entry"), QLineEdit::Normal, exceptionListWidget->currentItem()->text(), &ok);
	if (ok && !text.isEmpty())
	{
		if (exceptionListWidget->findItems(text, Qt::MatchExactly).isEmpty())
			exceptionListWidget->currentItem()->setText(text);
		exceptionListWidget->sortItems();
	}
}

void Prefs_Hyphenator::editExceptionListItem(QListWidgetItem* item)
{
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	exceptionListWidget->editItem(item);
}


void Prefs_Hyphenator::exceptionListItemChanged(QListWidgetItem*)
{
	exceptionListWidget->sortItems();
}

void Prefs_Hyphenator::removeFromExceptionList()
{
	qDeleteAll(exceptionListWidget->selectedItems());
}

void Prefs_Hyphenator::enableExceptionButtons()
{
	exceptionRemoveButton->setEnabled(true);
}
