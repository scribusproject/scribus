/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef AUTOFORMBUTTONGROUP_H
#define AUTOFORMBUTTONGROUP_H

#include <QEvent>
#include <QMenu>
#include <QPixmap>
#include <QSignalMapper>
#include <QWidget>

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
	void iconSetChange();
	void languageChange();

signals:
	void FormSel(int, int, qreal *);
	void buttonClicked(int);

protected:
	QSignalMapper *signalMapper { nullptr };
	QMenu *menu1 { nullptr };
	QMenu *menu2 { nullptr };
	QMenu *menu3 { nullptr };
	QMenu *menu4 { nullptr };
	QMenu *menu5 { nullptr };

	void changeEvent(QEvent *e) override;
};

#endif


