/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_TABLEOFCONTENTS_H
#define PREFS_TABLEOFCONTENTS_H

#include "ui_prefs_tableofcontentsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_TableOfContents : public QWidget, Ui::Prefs_TableOfContents
{
	Q_OBJECT

	public:
		Prefs_TableOfContents(QWidget* parent=0);
		~Prefs_TableOfContents();
};

#endif // PREFS_TABLEOFCONTENTS_H
