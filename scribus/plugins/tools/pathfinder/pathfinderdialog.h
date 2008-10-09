/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PATHFINDERDIALOG_H
#define PATHFINDERDIALOG_H

#include "pluginapi.h"
#include "ui_pathfinderbase.h"
#include <QPainterPath>
#include <QColor>
#include <QList>
#include "pageitem.h"
#include "scribusdoc.h"

class PLUGIN_API PathFinderDialog : public QDialog, Ui::PathFinderBase
{ 
	Q_OBJECT

public:
	PathFinderDialog(QWidget* parent, ScribusDoc* doc, PageItem *shape1, PageItem *shape2);
	~PathFinderDialog() {};
	void updatePreview(QLabel *label, QPainterPath &path, QColor color, double scale);
	void updatePartPreview(QColor color, double scale);
	const QString getOtherFillColor();
	const QString getOtherLineColor();
	int opMode;
	bool swapped;
	int targetColor;
	bool keepItem1;
	bool keepItem2;
	QPainterPath input1;
	QPainterPath input2;
	QPainterPath result;
	QPainterPath result1;
	QPainterPath result2;
private:
	QColor getColorFromItem(QString color, QColor in);
	ScribusDoc* currDoc;
	PageItem* source1;
	PageItem* source2;
private slots:
	void checkKeep();
	void newOpMode();
	void swapObjects();
	void updateAllPreviews();
	void updateResult();
};
#endif
