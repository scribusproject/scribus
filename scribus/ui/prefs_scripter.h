/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_SCRIPTER_H
#define PREFS_SCRIPTER_H

#include "ui_prefs_scripterbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_Scripter : public QWidget, Ui::Prefs_Scripter
{
	Q_OBJECT

	public:
		Prefs_Scripter(QWidget* parent=0);
		~Prefs_Scripter();
};

#endif // PREFS_SCRIPTER_H
