/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "fsw_language.h"
#include <QEvent>
#include <QList>
#include <QPair>

#include "langmgr.h"
#include "prefsmanager.h"
#include "prefsstructs.h"
#include "units.h"

FSW_Language::FSW_Language(QWidget* parent)
	: QWizardPage(parent)
{
	setupUi(this);
	populate();
}

void FSW_Language::populate()
{
	LanguageManager* lm = LanguageManager::instance();

	// Interface language: installed GUI translations, abbrev stored as item data.
	QList<QPair<QString, QString>> guiLangs;   // (priAbbrev, displayName)
	lm->fillInstalledGUILangPairs(&guiLangs);
	for (const auto& pair : guiLangs)
		uiLanguageCombo->addItem(pair.second, pair.first);

	// Default document language: languages with an installed spell dictionary.
	QStringList spellLangs;
	lm->fillInstalledSpellStringList(&spellLangs);
	for (const QString& name : spellLangs)
		docLanguageCombo->addItem(name, lm->getAbbrevFromLang(name));

	// Units: unitGetTextUnitList() is index-ordered, so the combo index is the unit
	// index (same call the Document Setup pane uses).
	unitsCombo->addItems(unitGetTextUnitList());
}

void FSW_Language::restoreDefaults(const ApplicationPrefs* prefsData)
{
	if (!prefsData)
		return;

	int uiIdx = uiLanguageCombo->findData(prefsData->uiPrefs.language);
	if (uiIdx >= 0)
		uiLanguageCombo->setCurrentIndex(uiIdx);

	int docIdx = docLanguageCombo->findData(prefsData->docSetupPrefs.language);
	if (docIdx >= 0)
		docLanguageCombo->setCurrentIndex(docIdx);

	const int unitIdx = prefsData->docSetupPrefs.docUnitIndex;
	if (unitIdx >= 0 && unitIdx < unitsCombo->count())
		unitsCombo->setCurrentIndex(unitIdx);
}

QString FSW_Language::uiLanguage() const
{
	return uiLanguageCombo->currentData().toString();   // priAbbrev, e.g. "en_GB"
}

QString FSW_Language::uiLanguageName() const
{
	return uiLanguageCombo->currentText();
}

QString FSW_Language::documentLanguage() const
{
	// Normalised the same way prefsmanager stores docSetupPrefs.language.
	return LanguageManager::instance()->getShortAbbrevFromAbbrevDecomposition(docLanguageCombo->currentData().toString());
}

QString FSW_Language::documentLanguageName() const
{
	return docLanguageCombo->currentText();
}

int FSW_Language::unitIndex() const
{
	return unitsCombo->currentIndex();   // index == unit index
}

QString FSW_Language::unitName() const
{
	return unitsCombo->currentText();
}

void FSW_Language::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange)
		retranslateUi(this);
	QWizardPage::changeEvent(e);
}
