/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "fsw_fontsscripts.h"
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QListWidget>

#include "prefsstructs.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefstable.h"

FSW_FontsScripts::FSW_FontsScripts(QWidget* parent)
	: QWizardPage(parent)
{
	setupUi(this);
	connect(addFontButton,     &QPushButton::clicked, this, &FSW_FontsScripts::addFontFolder);
	connect(removeFontButton,  &QPushButton::clicked, this, &FSW_FontsScripts::removeFontFolder);
	connect(scriptBrowseButton, &QPushButton::clicked, this, &FSW_FontsScripts::browseScriptFolder);
}

void FSW_FontsScripts::restoreDefaults(const ApplicationPrefs* prefsData)
{
	if (!prefsData)
		return;

	// pathPrefs.scripts is a single directory, matching the Paths preferences pane.
	scriptPathEdit->setText(QDir::toNativeSeparators(prefsData->pathPrefs.scripts));

	// Font folders live in the PrefsFile "Fonts" context rather than in
	// ApplicationPrefs. Seed the list the same way Prefs_Fonts::readPaths() does, so
	// the wizard shows any folders already configured instead of starting empty.
	fontList->clear();
	PrefsContext* fontCtx = PrefsManager::instance().prefsFile->getContext("Fonts");
	PrefsTable* fontTable = fontCtx->getTable("ExtraFontDirs");
	for (int i = 0; i < fontTable->getRowCount(); ++i)
		fontList->addItem(QDir::toNativeSeparators(fontTable->get(i, 0)));
}

void FSW_FontsScripts::addFontFolder()
{
	const QString dir = QFileDialog::getExistingDirectory(this, tr("Select a font folder"));
	if (!dir.isEmpty())
		fontList->addItem(QDir::toNativeSeparators(dir));
}

void FSW_FontsScripts::removeFontFolder()
{
	qDeleteAll(fontList->selectedItems());
}

void FSW_FontsScripts::browseScriptFolder()
{
	const QString dir = QFileDialog::getExistingDirectory(this, tr("Select a script folder"), scriptPathEdit->text());
	if (!dir.isEmpty())
		scriptPathEdit->setText(QDir::toNativeSeparators(dir));
}

QStringList FSW_FontsScripts::pathsFromList(const QListWidget* list)
{
	QStringList out;
	for (int i = 0; i < list->count(); ++i)
		out << list->item(i)->text();
	return out;
}

QStringList FSW_FontsScripts::fontPaths() const
{
	return pathsFromList(fontList);
}

QString FSW_FontsScripts::scriptPath() const
{
	return scriptPathEdit->text();
}

void FSW_FontsScripts::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange)
		retranslateUi(this);
	QWizardPage::changeEvent(e);
}
