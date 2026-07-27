/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef FSW_EXPERIMENTAL_H
#define FSW_EXPERIMENTAL_H

#include <QWizardPage>
#include "ui_fsw_experimental.h"

struct ApplicationPrefs;

//! \brief Single master toggle for experimental features.
class FSW_Experimental : public QWizardPage, Ui::FSW_Experimental
{
		Q_OBJECT

	public:
		explicit FSW_Experimental(QWidget* parent = nullptr);
		void restoreDefaults(const struct ApplicationPrefs* prefsData);  //!< seed the page from @a prefsData
		bool experimentalEnabled() const;

	protected:
		void changeEvent(QEvent* e) override;
};
#endif // FSW_EXPERIMENTAL_H
