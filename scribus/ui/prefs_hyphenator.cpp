/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QInputDialog>
#include <QListWidget>

#include "prefs_hyphenator.h"
#include "langmgr.h"
#include "prefsstructs.h"
#include "scribuscore.h" //FIXME: for the ScCore call (remove this call)
#include "scribusdoc.h"
#include "scribus.h" //FIXME: for the ScCore call (remove this call)
#include "util_icon.h"
#include "util.h"

Prefs_Hyphenator::Prefs_Hyphenator(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	LanguageManager *lmg(LanguageManager::instance());
	hyphLanguageComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);
	foreach(QString hlang, lmg->hyphLangs())
	{
		hyphLanguageComboBox->addItem( lmg->getLangFromAbbrev(hlang), lmg->getLangFromAbbrev(hlang,false) );
	}

	exceptionAddButton->setIcon(QIcon(loadIcon("16/list-add.png")));
	exceptionEditButton->setEnabled(false);
	exceptionRemoveButton->setIcon(QIcon(loadIcon("16/list-remove.png")));
	exceptionRemoveButton->setEnabled(false);
	ignoreAddButton->setIcon(QIcon(loadIcon("16/list-add.png")));
	ignoreEditButton->setEnabled(false);
	ignoreRemoveButton->setIcon(QIcon(loadIcon("16/list-remove.png")));
	ignoreRemoveButton->setEnabled(false);
	connect(ignoreAddButton, SIGNAL(clicked()), this, SLOT(addToIgnoreList()));
	connect(ignoreEditButton, SIGNAL(clicked()), this, SLOT(editIgnoreListEntry()));
	connect(ignoreRemoveButton, SIGNAL(clicked()), this, SLOT(removeIgnoreListEntry()));
	connect(ignoreListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enableIgnoreButtons()));
	connect(exceptionAddButton, SIGNAL(clicked()), this, SLOT(addToExceptList()));
	connect(exceptionEditButton, SIGNAL(clicked()), this, SLOT(editExceptListEntry()));
	connect(exceptionRemoveButton, SIGNAL(clicked()), this, SLOT(removeExceptListEntry()));
	connect(exceptionListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enableExceptButtons()));
}

Prefs_Hyphenator::~Prefs_Hyphenator()
{
}

void Prefs_Hyphenator::languageChange()
{
}

void Prefs_Hyphenator::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	hyphSuggestionsCheckBox->setChecked(!prefsData->hyphPrefs.Automatic);
	hyphAutoCheckBox->setChecked(prefsData->hyphPrefs.AutoCheck);
	setCurrentComboItem(hyphLanguageComboBox, LanguageManager::instance()->getTransLangFromLang(prefsData->hyphPrefs.Language));
	smallestWordSpinBox->setValue(prefsData->hyphPrefs.MinWordLen);
	maxConsecutiveCountSpinBox->setValue(prefsData->hyphPrefs.HyCount);
	ignoreListWidget->addItems(prefsData->hyphPrefs.ignoredWords.toList());
	ignoreListWidget->sortItems();
	exceptionListWidget->addItems(prefsData->hyphPrefs.specialWords.values());
	exceptionListWidget->sortItems();
}

void Prefs_Hyphenator::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->hyphPrefs.MinWordLen = smallestWordSpinBox->value();
	 //FIXME: remove this ScCore call
	prefsData->hyphPrefs.Language = ScCore->primaryMainWindow()->GetLang(hyphLanguageComboBox->itemData(hyphLanguageComboBox->currentIndex()).toString());
	prefsData->hyphPrefs.Automatic = !hyphSuggestionsCheckBox->isChecked();
	prefsData->hyphPrefs.AutoCheck = hyphAutoCheckBox->isChecked();
	prefsData->hyphPrefs.HyCount = maxConsecutiveCountSpinBox->value();
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
	QString text = QInputDialog::getText(this, tr("Ignore List"), tr("Add a new Entry"), QLineEdit::Normal, "", &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (ignoreListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			ignoreListWidget->addItem(text);
		ignoreListWidget->sortItems();
	}
}

void Prefs_Hyphenator::editIgnoreListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Ignore List"), tr("Edit Entry"), QLineEdit::Normal, ignoreListWidget->currentItem()->text(), &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (ignoreListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			ignoreListWidget->currentItem()->setText(text);
		ignoreListWidget->sortItems();
	}
}

void Prefs_Hyphenator::removeIgnoreListEntry()
{
	QListWidgetItem *item = ignoreListWidget->takeItem(ignoreListWidget->row(ignoreListWidget->currentItem()));
	delete item;
	if (ignoreListWidget->count() == 0)
	{
		ignoreEditButton->setEnabled(false);
		ignoreRemoveButton->setEnabled(false);
	}
}

void Prefs_Hyphenator::enableIgnoreButtons()
{
	ignoreEditButton->setEnabled(true);
	ignoreRemoveButton->setEnabled(true);
}

void Prefs_Hyphenator::addToExceptList()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Exception List"), tr("Add a new Entry"), QLineEdit::Normal, "", &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (exceptionListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			exceptionListWidget->addItem(text);
		exceptionListWidget->sortItems();
	}
}

void Prefs_Hyphenator::editExceptListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Exception List"), tr("Edit Entry"), QLineEdit::Normal, exceptionListWidget->currentItem()->text(), &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (exceptionListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			exceptionListWidget->currentItem()->setText(text);
		exceptionListWidget->sortItems();
	}
}

void Prefs_Hyphenator::removeExceptListEntry()
{
	QListWidgetItem *item = exceptionListWidget->takeItem(exceptionListWidget->row(exceptionListWidget->currentItem()));
	delete item;
	if (exceptionListWidget->count() == 0)
	{
		exceptionEditButton->setEnabled(false);
		exceptionRemoveButton->setEnabled(false);
	}
}

void Prefs_Hyphenator::enableExceptButtons()
{
	exceptionEditButton->setEnabled(true);
	exceptionRemoveButton->setEnabled(true);
}
