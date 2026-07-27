/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef FSW_APPEARANCE_H
#define FSW_APPEARANCE_H

#include <QWizardPage>
#include "ui_fsw_appearance.h"

struct ApplicationPrefs;

//! \brief Theme mode (drives the live splash swap) and workspace density.
class FSW_Appearance : public QWizardPage, Ui::FSW_Appearance
{
		Q_OBJECT
	public:
		explicit FSW_Appearance(QWidget* parent = nullptr);
		void restoreDefaults(const struct ApplicationPrefs* prefsData);  //!< seed the page from @a prefsData

		int themeMode() const; //!< 0 light, 1 dark, 2 automatic
		bool minimalWorkspace() const;

	signals:
		//! \brief Emitted as the user changes the theme so the wizard can swap the splash.
		void themeModeChanged(int mode);

	protected:
		void changeEvent(QEvent* e) override;
	private:
		void wireRadios();
};
#endif // FSW_APPEARANCE_H
