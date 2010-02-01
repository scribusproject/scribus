/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_SHORTWORDS_H
#define PREFS_SHORTWORDS_H

#include "ui_prefs_shortwordsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_ShortWords : public QWidget, Ui::Prefs_ShortWords
{
	Q_OBJECT

	public:
		Prefs_ShortWords(QWidget* parent=0);
		~Prefs_ShortWords();
};

#endif // PREFS_SHORTWORDS_H
