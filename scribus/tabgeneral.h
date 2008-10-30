/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABGENERAL_H
#define TABGENERAL_H

#include "ui_tabgeneral.h"
#include "scribusapi.h"
#include "langmgr.h"

class QString;


/*! \brief General panel for preferences dialog.
This class is inherited from UI base class.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API TabGeneral : public QWidget, public Ui::TabGeneral
{
	Q_OBJECT

	public:
		TabGeneral(QWidget* parent = 0, const char* name = 0);
		~TabGeneral(){};
		void restoreDefaults(struct ApplicationPrefs *prefsData);

// 		LanguageManager langMgr;
		QString selectedGUILang;

	private slots:
		void setSelectedGUILang( const QString &newLang );
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

#endif
