/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABPRINTER_H
#define TABPRINTER_H

#include "ui_tabprinter.h"
#include "scribusapi.h"

class PrefsContext;

class SCRIBUS_API TabPrinter : public QWidget, Ui::TabPrinter
{
	Q_OBJECT

public:
	TabPrinter(QWidget* parent = 0, const char* name = 0);
	~TabPrinter() {};
	void restoreDefaults(struct ApplicationPrefs *prefsData);
	void storeValues();
	void unitChange(int docUnitIndex);
private:
	int docUnitIndex;
	double unitRatio;
	PrefsContext* prefs;
private slots:
	void selOtherComm();
};
#endif
