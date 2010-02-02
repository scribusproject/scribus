/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_PANE_H
#define PREFS_PANE_H

#include <QWidget>
#include "scribusapi.h"

class SCRIBUS_API Prefs_Pane : public QWidget
{
	Q_OBJECT

	public:
		Prefs_Pane(QWidget* parent) : QWidget(parent) {};
		~Prefs_Pane() {} ;
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData) = 0;
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const = 0;
};

#endif // PREFS_PANE_H
