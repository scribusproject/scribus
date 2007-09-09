/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABMISCELLANEOUS_H
#define TABMISCELLANEOUS_H

#include "ui_tabmiscellaneous.h"
#include "scribusapi.h"

struct ApplicationPrefs;

/*! \brief Miscellaneous panel for preferences dialog.
This class is inherited from UI base class.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API TabMiscellaneous : public QWidget, public Ui::TabMiscellaneous
{
	Q_OBJECT

public:
	TabMiscellaneous(QWidget* parent = 0);
	~TabMiscellaneous(){};
	void restoreDefaults(struct ApplicationPrefs *prefsData);

};

#endif
