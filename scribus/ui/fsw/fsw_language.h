/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef FSW_LANGUAGE_H
#define FSW_LANGUAGE_H

#include <QWizardPage>
#include "ui_fsw_language.h"

struct ApplicationPrefs;

//! \brief Language & units: interface language, document language, units.
class FSW_Language : public QWizardPage, Ui::FSW_Language
{
		Q_OBJECT
	public:
		explicit FSW_Language(QWidget* parent = nullptr);
		void restoreDefaults(const struct ApplicationPrefs* prefsData);  //!< seed the page from @a prefsData

		QString uiLanguage() const;        //!< interface language (translation key)
		QString uiLanguageName() const;    //!< combo display text, for the summary
		QString documentLanguage() const;  //!< default language for new text
		QString documentLanguageName() const;  //!< combo display text
		int unitIndex() const;         //!< -> docSetupPrefs.docUnitIndex
		QString unitName() const;          //!< combo display text

	protected:
		void changeEvent(QEvent* e) override;

	private:
		void populate();
};
#endif // FSW_LANGUAGE_H
