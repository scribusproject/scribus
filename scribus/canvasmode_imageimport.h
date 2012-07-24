/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef CANVASMODE_IMAGEIMPORT_H
#define CANVASMODE_IMAGEIMPORT_H

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QStringList>

#include "canvasmode.h"

class PageItem;
class ScribusView;
class ScribusMainWindow;

class CanvasMode_ImageImport : public CanvasMode
{
public:
	explicit CanvasMode_ImageImport(ScribusView* view);
	virtual ~CanvasMode_ImageImport() {}

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);

	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void keyPressEvent(QKeyEvent *e);
	virtual bool handleKeyEvents() { return true; }
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter* p);

	void setImageList(QStringList l);
	void setImage(PageItem* item);
	void updateList();

private:
	bool m_keyRepeat;
	QString tipText;
	QStringList imageList;
	ScribusMainWindow *m_ScMW;
	double Mx, My;
	void newToolTip(QString name);
};

#endif // CANVASMODE_IMAGEIMPORT_H
