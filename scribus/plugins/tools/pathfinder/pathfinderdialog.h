/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PATHFINDERDIALOG_H
#define PATHFINDERDIALOG_H

#include "scribusapi.h"
#include "ui_pathfinderbase.h"
#include <QPainterPath>
#include <QColor>
#include <QList>
#include "pageitem.h"

class SCRIBUS_API PathFinderDialog : public QDialog, Ui::PathFinderBase
{ 
	Q_OBJECT

public:
	PathFinderDialog(QWidget* parent, PageItem *shape1, PageItem *shape2);
	~PathFinderDialog() {};
	void updatePreview(QLabel *label, QPainterPath &path, QColor color, double scale);
	int opMode;
	bool swapped;
	QPainterPath input1;
	QPainterPath input2;
	QPainterPath result;
	QColor inputC1;
	QColor inputC2;
private slots:
	void newOpMode();
	void swapObjects();
	void updateAllPreviews();
	void updateResult();
};
#endif
