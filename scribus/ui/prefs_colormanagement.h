/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_COLORMANAGEMENT_H
#define PREFS_COLORMANAGEMENT_H

#include "ui_prefs_colormanagementbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_ColorManagement : public QWidget, Ui::Prefs_ColorManagement
{
	Q_OBJECT

	public:
		Prefs_ColorManagement(QWidget* parent=0);
		~Prefs_ColorManagement();
};

#endif // PREFS_COLORMANAGEMENT_H
