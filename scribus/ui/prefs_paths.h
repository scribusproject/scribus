/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_PATHS_H
#define PREFS_PATHS_H

#include "ui_prefs_pathsbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Paths : public Prefs_Pane, Ui::Prefs_Paths
{
	Q_OBJECT

	public:
		Prefs_Paths(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Paths();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();

	protected slots:
		/*!
		\author Franz Schmid
		\brief Runs QFileDialog to get Preferences (General) Path to Documents directory
		 */
		void changeDocs();
		/*!
		\author Franz Schmid
		\brief Runs QFileDialog to get Preferences (General) Path to Colour Profiles directory
		*/
		void changeProfs();
		/*!
		\author Franz Schmid
		\brief Runs QFileDialog to get Preferences (General) Path to Scripts directory
		*/
		void changeScripts();
		/*!
		\author Riku Leino
		\brief Runs QFileDialog to get Preferences (General) Path to Document Templates directory
		*/
		void changeDocumentTemplates();
};

#endif // PREFS_PATHS_H
