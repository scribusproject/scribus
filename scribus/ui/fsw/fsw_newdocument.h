/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef FSW_NEWDOCUMENT_H
#define FSW_NEWDOCUMENT_H

#include <QWizardPage>
#include "ui_fsw_newdocument.h"

struct ApplicationPrefs;

//! \brief New-document defaults: page size and default text direction.
class FSW_NewDocument : public QWizardPage, Ui::FSW_NewDocument
{
		Q_OBJECT
	public:
		explicit FSW_NewDocument(QWidget* parent = nullptr);
		void restoreDefaults(const struct ApplicationPrefs* prefsData);  //!< seed the page from @a prefsData

		QString pageSizeName() const;       //!< -> docSetupPrefs.pageSize
		QString pageSizeDisplayName() const;
		bool isRTL() const;              //!< -> docSetupPrefs.isRTL
		bool showAdvancedTextDir() const;

	protected:
		void changeEvent(QEvent* e) override;
	private:
		void populate();
};
#endif // FSW_NEWDOCUMENT_H
