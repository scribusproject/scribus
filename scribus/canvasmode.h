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

#include <QBrush>
#include <QCursor>
#include <QMap>
#include <QPen>
#include <QPointF>

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QEvent;
class QInputMethodEvent;
class QImage;
class QMouseEvent;
class QKeyEvent;
class QPainter;

class  UndoManager;
class  Canvas;
struct CanvasViewMode;
class  PanGesture;
class  ScribusDoc;
class  ScribusView;
class  ScribusMainWindow;
class  PageItem;
class  PageItemPreview;
class  PageItem_TextFrame;

#include "fpoint.h"

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
	submodeEditSymbol,		// open Symbol editor
	submodeLastSubmode
};


/**
  This class is a superclass for all mode handlers.
  By default, all events are ignored.
 */
class SCRIBUS_API CanvasMode : public QObject
{
	Q_OBJECT
protected:
	explicit CanvasMode (ScribusView* view);
	
public:
	static CanvasMode* createForAppMode(ScribusView* view, int appMode);
	
	/**
	  Is called when this mode becomes active, either because it was selected by the user
	  (fromgesture == false) or because a gesture completed and the canvas returns back to
	  this mode (fromGesture == true)
	 */
	virtual void activate(bool fromGesture) {}
	/**
	  Is called when this mode becomes inactive, either because the canvas switches to
	  another mode (forGesture == false) or because a gesture is activated (forGesture == true)
	 */
	virtual void deactivate(bool forGesture) {}
	
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

	virtual bool handleKeyEvents() { return false; }

	/**
		Sets appropriate values for this canvas mode
	 */
	virtual void updateViewMode(CanvasViewMode* viewmode);
	
	/**
		Draws the controls for this mode and the selection marker. 
	 If viewmode.drawSelectionWithControls is true, also draws the selection contents first.
	 */
	virtual void drawControls(QPainter* p) { } 
	

	/** Draws the handles for the selection marker */
	void drawSelectionHandles(QPainter *psx, QRectF selectionRect, bool background, bool insideGroup = false, double sx = 1.0, double sy = 1.0);
	/** Draws the regular selection marker */
	void drawSelection(QPainter* psx, bool drawHandles);
	/** Draws an outline of selected items */
	void drawSnapLine(QPainter* psx);
	/** Draws an outline of selected items */
	void drawOutline(QPainter* p, double scalex=1.0, double scaley=1.0, double deltax=0.0, double deltay=0.0);
#ifdef GESTURE_FRAME_PREVIEW
	// I don’t know why the methods above have been implemented here and left non-virtual.
	// I need to setup some companion members - pm
	private:
	QMap<PageItem*, PageItemPreview*> m_pixmapCache;
	public:
	void clearPixmapCache();
#endif // GESTURE_FRAME_PREVIEW

	QCursor modeCursor();
	void setModeCursor();
	
	/** main canvas modes don't have a delegate */
	virtual CanvasMode* delegate() { return 0; }
	ScribusView* view() const { return m_view; }
	virtual ~CanvasMode();
	

protected:
	ScribusView * const m_view;
	Canvas * const m_canvas;
	ScribusDoc * const m_doc;
	PanGesture * m_panGesture;
	UndoManager* undoManager;
	double xSnap,ySnap;
	
	void setResizeCursor(int how, double rot = 0.0);
	bool commonMouseMove(QMouseEvent *m);
	void commonDrawControls(QPainter* p, bool drawHandles);
	/// Draws the text cursor for @a textframe, offset by @a offset.
	void commonDrawTextCursor(QPainter* p, PageItem_TextFrame* textframe, const QPointF& offset);
	void commonkeyPressEvent_NormalNodeEdit(QKeyEvent *e);
	void commonkeyReleaseEvent(QKeyEvent *e);
	
private:
	QMap<QString,QPen> m_pen;
	QMap<QString,QBrush> m_brush;

	bool m_keyRepeat;
	bool m_arrowKeyDown;	
	//FPoint m_mousePointDoc;
};


#endif
