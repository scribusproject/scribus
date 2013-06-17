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

class PageItem;
class ScribusDoc;

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

	bool keepItem1;
	bool keepItem2;
	bool swapped;
	int  opMode;
	int  targetColor;

	QPainterPath result;
	QPainterPath result1;
	QPainterPath result2;
private:
	QColor getColorFromItem(QString color, QColor in);

	ScribusDoc* m_doc;
	PageItem* m_source1;
	PageItem* m_source2;

	QPainterPath m_input1;
	QPainterPath m_input2;
	
private slots:
	void checkKeep();
	void newOpMode();
	void swapObjects();
	void updateAllPreviews();
	void updateResult();
};
#endif
