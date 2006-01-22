/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef AUTOFORM_H
#define AUTOFORM_H

#include <qwidget.h>
#include <qtoolbutton.h>
#include "scribusapi.h"
class QGridLayout;
class QPopupMenu;
class QPixmap;
class AutoformButtonGroup;


class SCRIBUS_API Autoforms : public QToolButton
{
    Q_OBJECT

public:
	Autoforms( QWidget* parent );
	~Autoforms() {};
	QPixmap getIconPixmap(int nr);
	QPopupMenu* Men;

public slots:
	void selForm(int a);

signals:
	void FormSel(int, int, double *);

protected:
	QGridLayout* buttonGroup1Layout;
	AutoformButtonGroup* buttonGroup1;
};

#endif

