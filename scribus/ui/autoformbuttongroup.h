/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef AUTOFORMBUTTONGROUP_H
#define AUTOFORMBUTTONGROUP_H

#include <QWidget>
#include <QMenu>
#include <QSignalMapper>
//#include <QToolButton>
//#include <QFrame>
//#include <QGridLayout>
#include <QPixmap>
//#include <QButtonGroup>
//#include <QCheckBox>
#include <QEvent>
#include "scribusapi.h"

class SCRIBUS_API AutoformButtonGroup : public QMenu
{
    Q_OBJECT

public:
	AutoformButtonGroup( QWidget* parent );
	~AutoformButtonGroup() {};

	void addShape(QMenu* menu, int shapenum);
	qreal* getShapeData(int a, int *n);
	QPixmap getIconPixmap(int nr, int pixmapSize=22);

public slots:
	void selForm(int a);
	void languageChange();

signals:
	void FormSel(int, int, qreal *);
	void buttonClicked(int);

protected:
	QSignalMapper *signalMapper;
	QMenu *menu1;
	QMenu *menu2;
	QMenu *menu3;
	QMenu *menu4;
	QMenu *menu5;

	void changeEvent(QEvent *e) override;
};

#endif


