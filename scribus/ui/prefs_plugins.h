/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_PLUGINS_H
#define PREFS_PLUGINS_H

#include "ui_prefs_pluginsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_Plugins : public QWidget, Ui::Prefs_Plugins
{
	Q_OBJECT

	public:
		Prefs_Plugins(QWidget* parent=0);
		~Prefs_Plugins();
};

#endif // PREFS_PLUGINS_H
