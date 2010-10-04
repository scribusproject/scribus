/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_FONTS_H
#define PREFS_FONTS_H

#include <QListWidgetItem>
#include <QPushButton>

#include "ui_prefs_fontsbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;
class ScComboBox;

class SCRIBUS_API Prefs_Fonts : public Prefs_Pane, Ui::Prefs_Fonts
{
	Q_OBJECT

	public:
		Prefs_Fonts(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Fonts();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		void ReplaceSel(int r, int c);
		void updateFontList();
		void DelEntry();
		void SelectPath(QListWidgetItem *c);
		void AddPath();
		void ChangePath();
		void DelPath();

	protected:
		void readPaths();
		void writePaths() const;

		QMap<QString,QString> RList;
		QList<ScComboBox*> FlagsRepl;
		//! List of font names of allowed fonts for substitutions
		QStringList UsedFonts;
		QString CurrentPath;
		ScribusDoc* m_doc;


		SCFonts m_availFonts; //! Fonts that Scribus has available to it, or the current document has available to use
		bool m_askBeforeSubstitute; //! Request that the user confirms a font substitution or not
		QMap<QString,QString> m_GFontSub;
};

#endif // PREFS_FONTS_H
