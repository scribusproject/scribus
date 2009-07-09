/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_EXTERNALTOOLS_H
#define PREFS_EXTERNALTOOLS_H

#include "ui_prefs_externaltoolsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_ExternalTools : public QWidget, Ui::Prefs_ExternalTools
{
	Q_OBJECT

	public:
		Prefs_ExternalTools(QWidget* parent=0);
		~Prefs_ExternalTools();
};

#endif // PREFS_EXTERNALTOOLS_H
