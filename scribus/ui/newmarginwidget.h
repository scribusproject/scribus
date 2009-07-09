/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef NEWMARGINWIDGET_H
#define NEWMARGINWIDGET_H

#include "ui_newmarginwidgetbase.h"
#include "scribusapi.h"

class SCRIBUS_API NewMarginWidget : public QWidget, Ui::NewMarginWidget
{
	Q_OBJECT

	public:
		NewMarginWidget(QWidget* parent=0);
		~NewMarginWidget();
};

#endif // NEWMARGINWIDGET_H
