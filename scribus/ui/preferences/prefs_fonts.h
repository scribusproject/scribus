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

class QComboBox;
class ScribusDoc;

class SCRIBUS_API Prefs_Fonts : public Prefs_Pane, Ui::Prefs_Fonts
{
	Q_OBJECT

	public:
		Prefs_Fonts(QWidget* parent, ScribusDoc* doc = nullptr);
		~Prefs_Fonts() = default;

		void restoreDefaults(struct ApplicationPrefs *prefsData) override;
		void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const override;
		virtual void changeUnit(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		void ReplaceSel();
		void DelEntry();
		void SelectPath(QListWidgetItem *c);
		void AddPath();
		void ChangePath();
		void DelPath();

	protected:
		void readPaths();
		void writePaths() const;

		void updateFontList();
		void updateRejectedFontList();

		QMap<QString, QString> RList;
		QList<QComboBox*> FlagsRepl;
		//! List of font names of allowed fonts for substitutions
		QStringList m_usedFonts;
		QString m_currentPath;
		ScribusDoc* m_doc { nullptr };

		SCFonts m_availFonts; //! Fonts that Scribus has available to it, or the current document has available to use
		bool m_askBeforeSubstitute { true }; //! Request that the user confirms a font substitution or not
		QMap<QString,QString> m_GFontSub;
};

#endif // PREFS_FONTS_H
