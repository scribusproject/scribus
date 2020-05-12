/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem.h  -  description
                             -------------------
    copyright            : Scribus Team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PAGEITEMLINE_H
#define PAGEITEMLINE_H

#include <QPointF>
#include <QRectF>
#include <QString>

#include "scribusapi.h"
#include "pageitem.h"
class ScPainter;
class ScribusDoc;

class SCRIBUS_API PageItem_Line : public PageItem
{
	Q_OBJECT

public:
	PageItem_Line(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_Line(const PageItem & p) : PageItem(p) {}
	~PageItem_Line() {};
	
	PageItem_Line * asLine() override { return this; }
	bool isLine() const override { return true; }
	
	void applicableActions(QStringList& actionList) override;
	QString infoDescription() const override;

	QPointF startPoint() const;
	QPointF endPoint() const;

	void setLineClip();

	void getBoundingRect(double *x1, double *y1, double *x2, double *y2) const override;
	void getOldBoundingRect(double *x1, double *y1, double *x2, double *y2) const override;
	void getVisualBoundingRect(double *x1, double *y1, double *x2, double *y2) const override;

	QRectF getStartArrowBoundingRect() const override;
	QRectF getStartArrowOldBoundingRect() const override;

	QRectF getEndArrowBoundingRect() const override;
	QRectF getEndArrowOldBoundingRect() const override;

	double visualXPos() const override;
	double visualYPos() const override;
	double visualWidth() const override;
	double visualHeight() const override;
	
protected:
	void DrawObj_Item(ScPainter *p, QRectF e) override;

};

#endif
