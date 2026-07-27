/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "fsw_newdocument.h"
#include <QEvent>
#include <QLabel>
#include <QRadioButton>

#include "prefsmanager.h"
#include "prefsstructs.h"
#include "manager/pagepreset_manager.h"

FSW_NewDocument::FSW_NewDocument(QWidget* parent)
	: QWizardPage(parent)
{
	setupUi(this);
	// No preference exists for the advanced text-direction yet, so it is in
	// the .ui but hidden until one does.
	advancedTextDirCheck->setVisible(false);
	advancedTextDirLabel->setVisible(false);
	populate();
	ltrRadio->setChecked(true);
	// Advanced text-direction options default on for RTL, off otherwise; the wizard
	// flips this when an RTL UI language is chosen (soft default, not a one-way door).
	// connect(rtlRadio, &QRadioButton::toggled, this, [this](bool on){
	// 	if (on) advancedTextDirCheck->setChecked(true);
	// });
}

void FSW_NewDocument::populate()
{
	// Mirror the Page Sizes prefs pane: list the *active* sizes, storing each size id
	// (what docSetupPrefs.pageSize holds) as the item data.
	auto& ppm = PagePresetManager::instance();
	const auto activeSizes = ppm.activePageSizes();
	const QStringList categories = ppm.categoriesOrder();
	for (const QString& cat : categories)
	{
		if (cat == QString::fromUtf8("-"))
			continue;
		PageSizeInfoMap sizes = ppm.sizesByCategory(cat);
		for (auto it = sizes.begin(); it != sizes.end(); ++it)
		{
			if (!activeSizes.contains(it.key()))
				continue;
			pageSizeCombo->addItem(it.value().displayName, it.key());
		}
	}
}

void FSW_NewDocument::restoreDefaults(const ApplicationPrefs* prefsData)
{
	if (!prefsData)
		return;

	int i = pageSizeCombo->findData(prefsData->docSetupPrefs.pageSize);
	if (i >= 0)
		pageSizeCombo->setCurrentIndex(i);

	if (prefsData->docSetupPrefs.isRTL)
		rtlRadio->setChecked(true);
	else
		ltrRadio->setChecked(true);
}

QString FSW_NewDocument::pageSizeName() const
{
	return pageSizeCombo->currentData().toString();
}

QString FSW_NewDocument::pageSizeDisplayName() const
{
	return pageSizeCombo->currentText();
}

bool FSW_NewDocument::isRTL() const
{
	return rtlRadio->isChecked();
}

bool FSW_NewDocument::showAdvancedTextDir() const
{
	return advancedTextDirCheck->isChecked();
}

void FSW_NewDocument::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange)
		retranslateUi(this);
	QWizardPage::changeEvent(e);
}
