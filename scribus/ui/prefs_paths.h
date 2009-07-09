/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_PATHS_H
#define PREFS_PATHS_H

#include "ui_prefs_pathsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_Paths : public QWidget, Ui::Prefs_Paths
{
	Q_OBJECT

	public:
		Prefs_Paths(QWidget* parent=0);
		~Prefs_Paths();
};

#endif // PREFS_PATHS_H
