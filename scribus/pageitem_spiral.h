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

#ifndef PAGEITEM_SPIRAL_H
#define PAGEITEM_SPIRAL_H

#include <QString>
#include <QRectF>

#include "scribusapi.h"
#include "pageitem.h"
class ScPainter;
class ScribusDoc;


class SCRIBUS_API PageItem_Spiral : public PageItem
{
	Q_OBJECT

public:
	PageItem_Spiral(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_Spiral(const PageItem& p);
	~PageItem_Spiral() {};

	PageItem_Spiral * asSpiral() override { return this; }
	const PageItem_Spiral * asSpiral() const override { return this; }
	bool isSpiral() const override { return true; }
	ItemType realItemType() const override { return PageItem::Spiral; }
	void applicableActions(QStringList& actionList) override;
	QString infoDescription() const override;
	void recalcPath();

	double spiralStartAngle { 0.0 };    //! angle where the spiral starts
	double spiralEndAngle { 1080 };    //! angle the spiral spans
	double spiralFactor { 1.2 };	//! factor the spiral gets smaller

	void getBoundingRect(double *x1, double *y1, double *x2, double *y2) const override;
	void getOldBoundingRect(double *x1, double *y1, double *x2, double *y2) const override;
	void getVisualBoundingRect(double *x1, double *y1, double *x2, double *y2) const override;
	
protected:
	void DrawObj_Item(ScPainter *p, const QRectF& e) override;

};

#endif
