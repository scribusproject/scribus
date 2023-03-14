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

#ifndef PAGEITEM_IMAGEFRAME_H
#define PAGEITEM_IMAGEFRAME_H

#include <QString>
#include <QRectF>
#include <QKeyEvent>

#include "scribusapi.h"
#include "pageitem.h"
class ScPainter;
class ScribusDoc;

class SCRIBUS_API PageItem_ImageFrame : public PageItem
{
	Q_OBJECT

public:
	PageItem_ImageFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_ImageFrame(const PageItem & p) : PageItem(p) {}
	~PageItem_ImageFrame();

	PageItem_ImageFrame * asImageFrame() override { return this; }
	const PageItem_ImageFrame * asImageFrame() const override { return this; }
	bool isImageFrame() const override { return true; }

	void handleModeEditKey(QKeyEvent *k, bool& keyRepeat) override;
	void clearContents() override;
	
	bool createInfoGroup(QFrame *, QGridLayout *) override;
	void applicableActions(QStringList& actionList) override;
	QString infoDescription() const override;
	
protected:
	void DrawObj_Item(ScPainter *p, const QRectF& e) override;

};

#endif
