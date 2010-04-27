/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_USERINTERFACE_H
#define PREFS_USERINTERFACE_H

#include <QColor>
#include <QFont>
#include <QString>

#include "ui_prefs_userinterfacebase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_UserInterface : public Prefs_Pane, Ui::Prefs_UserInterface
{
	Q_OBJECT

	public:
		Prefs_UserInterface(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_UserInterface();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();

	protected:
		QString selectedGUILang;
		QColor seFontColor;
		QFont seFont;

	private slots:
		void setSelectedGUILang( const QString &newLang );
		void changeStoryEditorFont();
		void changeStoryEditorFontColor();
};

#endif // PREFS_USERINTERFACE_H
