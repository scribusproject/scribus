/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qimage.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qpixmap.h>
#include <qbuttongroup.h>

#include "autoform.h"
#include "autoform.moc"
#include "autoformbuttongroup.h"
#include "fpointarray.h"
#include "scpainter.h"

Autoforms::Autoforms( QWidget* parent ) : QToolButton( parent, "auto" )
{
	Men = new QPopupMenu();
	buttonGroup1 = new AutoformButtonGroup(Men);
	Men->insertItem(buttonGroup1);
	setPopup(Men);
	setPopupDelay(10);
	setPixmap(buttonGroup1->getIconPixmap(0));
	connect(buttonGroup1, SIGNAL(clicked(int)), this, SLOT(selForm(int)));
}

void Autoforms::selForm(int a)
{
	int n;
	double* AutoShapes;
	Men->activateItemAt(0);
	const QPixmap *px = buttonGroup1->find(a)->pixmap();
	setPixmap(*px);
	AutoShapes = buttonGroup1->getShapeData(a, &n);
	emit FormSel(a, n, AutoShapes);
}

QPixmap Autoforms::getIconPixmap(int nr)
{
	return buttonGroup1->getIconPixmap(nr);
}

