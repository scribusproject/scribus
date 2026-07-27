/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef FIRSTSTARTWIZARD_H
#define FIRSTSTARTWIZARD_H

#include <QWizard>

#include "scribusapi.h"

struct ApplicationPrefs;

class FSW_SidePanel;
class FSW_Welcome;
class FSW_Language;
class FSW_Appearance;
class FSW_NewDocument;
class FSW_FontsScripts;
class FSW_Experimental;
class FSW_Finish;

/*! \brief First Start Wizard (FSW)

	Shown once, on the first run of Scribus, to capture a small set of
	defaults (language, units, appearance, new-document defaults, font/script
	paths and the experimental-features toggle).

	The pages only collect values; nothing is written until the user reaches
	Finish. applyToPrefs() reads every page once and commits to appPrefs.
	Skip records that setup has run and leaves the PrefsManager::initDefaults()
	values untouched.
*/
class SCRIBUS_API FirstStartWizard : public QWizard
{
		Q_OBJECT

	public:
		/*! \brief Collects first-run settings into @a prefsData.
		The wizard only ever writes to the struct it is given, so the caller can
		hand it a working copy and commit on Finish (see ScribusCore::startGUI). */
		explicit FirstStartWizard(struct ApplicationPrefs* prefsData, QWidget* parent = nullptr);

		//! \brief Page ids, in order. Used to drive the side-panel step list.
		enum PageId
		{
			Page_Welcome = 0,
			Page_Language,
			Page_Appearance,
			Page_NewDocument,
			Page_FontsScripts,
			Page_Experimental,
			Page_Finish
		};

		/*! \brief True when Scribus has never completed first-run setup. */
		static bool isFirstRun(const struct ApplicationPrefs& prefsData);

	protected:
		void accept() override;   //!< commit-at-Finish happens here

	private slots:
		void onSkip();                       //!< CustomButton1: mark done, apply nothing
		void onThemeModeChanged(int mode);   //!< live-swap the splash art (0 light, 1 dark, 2 auto)
		void onPageChanged(int id);          //!< keep the side-panel step list in sync

	private:
		void restoreDefaults(const struct ApplicationPrefs* prefsData);  //!< seed every page
		void rescanAddedFontDirs(const QStringList& before);             //!< scan wizard-added font folders into the session
		void saveGuiToPrefs(struct ApplicationPrefs* prefsData) const;   //!< read every page (Finish only)

		struct ApplicationPrefs* m_prefsData { nullptr };
		//! \brief Font folders as seeded, so accept() can tell which ones the user added.
		QStringList m_seededFontDirs;

		FSW_SidePanel* m_side { nullptr };
		FSW_Welcome* m_welcome { nullptr };
		FSW_Language* m_language { nullptr };
		FSW_Appearance* m_appearance { nullptr };
		FSW_NewDocument* m_newDocument { nullptr };
		FSW_FontsScripts* m_fontsScripts { nullptr };
		FSW_Experimental* m_experimental { nullptr };
		FSW_Finish* m_finish { nullptr };
};

#endif // FIRSTSTARTWIZARD_H
