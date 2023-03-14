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

#ifndef PAGEITEM_ARC_H
#define PAGEITEM_ARC_H

#include <QString>
#include <QRectF>

#include "scribusapi.h"
#include "pageitem.h"
class ScPainter;
class ScribusDoc;


class SCRIBUS_API PageItem_Arc : public PageItem
{
	Q_OBJECT

public:
	PageItem_Arc(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_Arc(const PageItem & p);
	~PageItem_Arc() {};

	PageItem_Arc * asArc() override { return this; }
	const PageItem_Arc * asArc() const override { return this; }
	bool isArc() const override { return true; }
	ItemType realItemType() const override { return PageItem::Arc; }
	void applicableActions(QStringList& actionList) override;
	QString infoDescription() const override;
	void recalcPath();

	double arcHeight; //! height of the circumfering ellipse
	double arcWidth;  //! width of the circumfering ellipse
	double arcStartAngle { 30.0 };    //! angle where the arc starts
	double arcSweepAngle { 300.0 };    //! angle the arc spans
	
protected:
	void DrawObj_Item(ScPainter *p, const QRectF& e) override;

};

#endif
