/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_DOCUMENTITEMATTRIBUTES_H
#define PREFS_DOCUMENTITEMATTRIBUTES_H

#include "ui_prefs_documentitemattributesbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_DocumentItemAttributes : public QWidget, Ui::Prefs_DocumentItemAttributes
{
	Q_OBJECT

	public:
		Prefs_DocumentItemAttributes(QWidget* parent=0);
		~Prefs_DocumentItemAttributes();
};

#endif // PREFS_DOCUMENTITEMATTRIBUTES_H
