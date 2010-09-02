/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_SCRIPTER_H
#define PREFS_SCRIPTER_H

#include "ui_prefs_scripterbase.h"
#include "ui/prefs_pane.h"
#include "pconsole.h"

class Prefs_Scripter : public Prefs_Pane, Ui::Prefs_Scripter
{
	Q_OBJECT

	public:
		Prefs_Scripter(QWidget* parent=0);
		~Prefs_Scripter();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		//! \brief Apply changes to prefs. Auto connected.
		void apply();


	protected:
		void setupSyntaxColors();
		QColor textColor;
		QColor commentColor;
		QColor keywordColor;
		QColor errorColor;
		QColor signColor;
		QColor stringColor;
		QColor numberColor;

	protected slots:
		/*! \brief All requests for color change are handled here.
		\author Petr Vanek
		\warning I'm trying to handle multiple signals via single slot here. sender() Returns a pointer to the object that sent the signal, if called in a slot activated by a signal; otherwise the return value is undefined. This function will return something apparently correct in other cases as well. However, its value may change during any function call, depending on what signal-slot connections are activated during that call. In Qt 3.0 the value will change more often than in 2.x. This function violates the object-oriented principle of modularity. However, getting access to the sender might be useful when many signals are connected to a single slot. The sender is undefined if the slot is called as a normal C++ function. */
		void setColor();
		void changeStartupScript();
};

#endif // PREFS_SCRIPTER_H
