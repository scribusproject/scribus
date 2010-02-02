/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_SHORTWORDS_H
#define PREFS_SHORTWORDS_H

#include "ui_prefs_shortwordsbase.h"
#include "ui/prefs_pane.h"

class Prefs_ShortWords : public Prefs_Pane, Ui::Prefs_ShortWords
{
	Q_OBJECT

	public:
		Prefs_ShortWords(QWidget* parent=0);
		~Prefs_ShortWords();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		//! \brief Apply changes to prefs. Auto connected.
		void apply();

	protected slots:
		/*! \brief Save the content into user file. */
		virtual void saveButton_pressed();
		/*! \brief Re-reads system wide config file. */
		virtual void resetButton_pressed();
		/*! \brief Text editor changed. */
		virtual void cfgEdit_changed();

	protected:
		/*! \brief Load cfg file.
		\param filename string with full path and name.*/
		bool loadCfgFile(QString filename);
};

#endif // PREFS_SHORTWORDS_H
