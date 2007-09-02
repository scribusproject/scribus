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



#ifndef CANVAS_MODE_H
#define CANVAS_MODE_H

#include "scribusapi.h"
#include <QMap>

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QEvent;
class QInputMethodEvent;
class QMouseEvent;
class QKeyEvent;
class QPainter;

class Canvas;
class CanvasViewMode;
class ScribusDoc;
class ScribusView;



/** These aren't real appmodes but open a new window or override behaviour for a short time */
enum SubMode
{
	submodeFirstSubmode = 1000,
	submodePaintingDone,    // return to normal mode
	submodeEndNodeEdit,     // return from node/shape editing
	submodeLoadPic,         // open GetImage dialog
	submodeStatusPic,       // open ManageImages dialog
	submodeEditExternal,    // open external image editor
	submodeAnnotProps,      // open properties dialog
	submodeLastSubmode
};


/**
  This class is a superclass for all mode handlers.
  By default, all events are ignored.
 */
class SCRIBUS_API CanvasMode
{
protected:
	CanvasMode (ScribusView* view);
	
public:
	static CanvasMode* createForAppMode(ScribusView* view, int appMode);
	
	virtual void enterEvent(QEvent *) {}
	virtual void leaveEvent(QEvent *) {}

	virtual void dragEnterEvent(QDragEnterEvent *e) {}
	virtual void dragMoveEvent(QDragMoveEvent *e) {}
	virtual void dragLeaveEvent(QDragLeaveEvent *e) {}
	virtual void dropEvent(QDropEvent *e) {}
	
	virtual void mouseDoubleClickEvent(QMouseEvent *m) {}
	virtual void mouseReleaseEvent(QMouseEvent *m) {}
	virtual void mouseMoveEvent(QMouseEvent *m) {}
	virtual void mousePressEvent(QMouseEvent *m) {}

	virtual void keyPressEvent(QKeyEvent *e) {}
	virtual void keyReleaseEvent(QKeyEvent *e) {}
	virtual void inputMethodEvent(QInputMethodEvent *e) {}

	/**
		Sets appropiate values for this viewmode
	 */
	virtual void updateViewMode(CanvasViewMode* viewmode);
	
	virtual CanvasMode* nextCanvasMode() { return 0; }
		
	/**
		Draws the selection marker. If viewmode.drawSelectionWithControls is true, also draws the selection contents first.
		*/
	virtual void drawSelection(QPainter* p);
	
	/**
		Draws the controls for this mode
	 */
	virtual void drawControls(QPainter* p) {}
	
	
	virtual ~CanvasMode() {}
	
protected:
	ScribusView * const m_view;	
	Canvas * const m_canvas;
	ScribusDoc * const m_doc;
	static QMap<int,CanvasMode*> modeInstances;
};


#endif
