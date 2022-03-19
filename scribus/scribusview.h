/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribusview.h  -  description
                             -------------------
    begin                : Fre Apr  6 21:47:55 CEST 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SCRIBUSVIEW_H
#define SCRIBUSVIEW_H

#include <vector>
// include files for QT
#include <QDragLeaveEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QElapsedTimer>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QMouseEvent>
#include <QNativeGestureEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QPushButton>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QStack>
#include <QTimer>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>

class QEvent;
class QMenu;
class QMimeData;

// application specific includes
#include "fpoint.h"
#include "observable.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "selectionrubberband.h"
#include "ui/clockwidget.h"
#include "undotransaction.h"

class Canvas;
class CanvasMode;
class CanvasGesture;
class Hruler;
class Vruler;
class ScPage;
class RulerMover;
class PageItem;
class PageSelector;
class ScribusDoc;
class ScribusWin;
class ScribusMainWindow;
class ScrSpinBox;
class Selection;
class UndoManager;
class TransactionSettings;

/**
 * This class provides an incomplete base for your application view.
 */

class SCRIBUS_API ScribusView : public QScrollArea, public Observer<QRectF>
{
	Q_OBJECT

public:
    ScribusView(QWidget* win=0, ScribusMainWindow* mw=0, ScribusDoc* doc=0);
    ~ScribusView();
	
	friend class CanvasMode_CopyProperties;
	friend class CanvasMode_Edit;
	friend class CanvasMode_EditGradient;
	friend class CanvasMode_EditMeshGradient;
	friend class CanvasMode_EditMeshPatch;
	friend class CanvasMode_EditTable;
	friend class CanvasMode_EditWeldPoint;
	friend class CanvasMode_EditPolygon;
	friend class CanvasMode_EditArc;
	friend class CanvasMode_EditSpiral;
	friend class CanvasMode_FrameLinks;
	friend class CanvasMode_ImageImport;
	friend class CanvasMode_Magnifier;
	friend class CanvasMode_NodeEdit;
	friend class CanvasMode_Normal;
	friend class CanvasMode_ObjImport;
	friend class CanvasMode_Rotate;

	struct ViewState
	{
		int currentPage { 0 };
		int contentX { 0 };
		int contentY { 0 };
		double canvasScale { 1.0 };
		FPoint minCanvasCoordinate;
		FPoint maxCanvasCoordinate;
	};
	
	void requestMode(int appMode);
	void startGesture(CanvasGesture*);
	void stopGesture();
	
  /** Vergroesserungseingabefeld */
	RulerMover *rulerMover; //Widget between the two rulers for dragging the ruler origin
	Hruler *horizRuler;
	Vruler *vertRuler;
	ClockWidget *clockLabel;
	QPushButton *endEditButton;
  /** Dokument zu dem die Seite gehoert */
	ScribusDoc * const m_doc;
	Canvas * const m_canvas;
	CanvasMode* m_canvasMode; // might be a CanvasGesture FIXME make private
	CanvasMode* canvasMode();
	QMap<int, CanvasMode*> modeInstances;
	ApplicationPrefs * const Prefs;
	UndoManager * const undoManager;
	ScribusMainWindow* m_ScMW;
	double OldScale {0.0};
	double dragX {0.0};
	double dragY {0.0};
	double dragW {0.0};
	double dragH {0.0};
	double oldW {-1.0};
	int RotMode {0};
	bool HaveSelRect {false};
	bool DraggedGroup {false};
	bool MidButt {false};
	bool updateOn {true};
	bool Magnify {false};
	bool storedFramesShown;
	bool storedShowControls;
	int editStrokeGradient;
	bool m_AnnotChanged;
	bool m_EditModeWasOn;
	bool m_ChangedState;
	SelectionRubberBand *redrawMarker;
	FPoint RCenter { -1.0, -1.0 };
	FPoint m_mousePointDoc;
	void updatesOn(bool on);
	//CB This MUST now be called AFTER a call to doc->addPage or doc->addMasterPage as it
	//does NOT create a page anymore.
	ScPage* addPage(int nr, bool mov = true);

	void reformPages(bool moveObjects = true);
	void reformPagesView();
	void showMasterPage(int nr);
	void hideMasterPage();
	void showSymbolPage(const QString& symbolName);
	void hideSymbolPage();
	void showInlinePage(int id);
	void hideInlinePage();

	QImage PageToPixmap(int Nr, int maxGr, PageToPixmapFlags flags = Pixmap_DrawFrame | Pixmap_DrawBackground);
	QImage MPageToPixmap(const QString& name, int maxGr, bool drawFrame = true);

	/**
	 * Called when the ruler origin is dragged
	 * @param m mouse event
	 */
	void setNewRulerOrigin(QMouseEvent *m);
	void getDragRectScreen(double *x, double *y, double *w, double *h);
	void getGroupRectScreen(double *x, double *y, double *w, double *h);
	bool PointOnLine(QPoint Start, QPoint Ende, QRect MArea);
	void TransformPoly(int mode, int rot = 1, double scaling = 1.0);
	bool slotSetCurs(int x, int y);
	// \brief return a resize cursor if the mouse is on a handle.
	Qt::CursorShape getResizeCursor(PageItem *currItem, QRect mpo, Qt::CursorShape cursorShape = Qt::ArrowCursor);
	void deselectItems(bool prop = true);
	void selectItemByNumber(int nr, bool draw = true, bool single = false);
	void selectItem(PageItem *pi, bool draw = true, bool single = false);
	void rememberOldZoomLocation(int mx=0, int my=0);
	bool groupTransactionStarted() { return m_groupTransactions > 0; }
	void startGroupTransaction(const QString &actionName = "",
							   const QString &description = "",
							   QPixmap *actionPixmap = 0,
							   Selection* customSelection = 0);
	void endGroupTransaction();
	void cancelGroupTransaction();
	void setScale(double newScale);
	double scale() const;

	void changed(QRectF re, bool) override;

	void updateCanvas(QRectF box = QRectF());
	void updateCanvas(double x, double y, double width, double height) { updateCanvas(QRectF(x, y, width, height)); }
	void setCanvasOrigin(double x, double y);
	void setCanvasCenter(double x, double y);
	FPoint canvasOrigin() const;
	QRectF visibleCanvasRect() const;
	void setRedrawMarkerShown(bool shown);
	
private:
	// legacy:
	void updateContents(QRect box = QRect());
	void updateContents(int x, int y, int w, int h);
	void repaintContents(QRect box);
	void resizeContents(int w, int h);
	QPoint contentsToViewport(QPoint p);
	QPoint viewportToContents(QPoint p);

public: // for now
	int contentsX();
	int contentsY();
	int contentsWidth();
	int contentsHeight();
	int visibleWidth() { return viewport()->size().width(); };
	int visibleHeight() { return viewport()->size().height(); };

	void setCanvasPos(double x, double y);
	void setCanvasCenterPos(double x, double y);
	void setContentsPos(int x, int y);

	void scrollBy(int x, int y);
	void scrollCanvasBy(double deltaX, double deltaY);
	void scrollContentsBy(int dx, int dy) override;

	void zoom(double scale = 0.0);
	void zoom(int canvasX, int canvasY, double scale, bool preservePoint);

	void saveViewState();
	void restoreViewState();
	const ViewState& topViewState() const { return m_viewStates.top(); }

public slots: // Public slots
	void iconSetChange();
	void languageChange();
	void toggleCMS(bool cmsOn);
	void switchPreviewVisual(int vis);
	void togglePreviewEdit(bool edit);
	void togglePreview(bool inPreview);
	void unitChange();
	void setRulersShown(bool isShown);
  /** Zooms in or out */
	void slotZoom100();
  /** Zooms in */
	void slotZoomIn(int mx = 0, int my = 0, bool preservePoint = false);
	void slotZoomOut(int mx = 0, int my = 0, bool preservePoint = false);
  /** Redraws everything */
	void DrawNew();
	void GotoPa(int pageNumber);
	void GotoLayer(int l);
	void GotoPage(int pageIndex);
	void ChgUnit(int art);

	void editExtendedImageProperties();
	void RefreshGradient(PageItem *currItem, double dx = 8, double dy = 8);
	void ToPicFrame();
	void ToPolyFrame();
	void ToTextFrame();
	void ToBezierFrame();
	void ToPathText();
	void FromPathText();
	void Bezier2Poly();
	void PasteToPage();
	void TextToPath();

	//for linking frame after draw new frame
private:
	PageItem * firstFrame;

private: // Private attributes
	int m_previousMode;
	QMenu *pmen3;
	QMenu *pmenResolution;
	QPoint m_pressLocation;
	QElapsedTimer m_moveTimer;
	QTimer *m_dragTimer;
	bool m_dragTimerFired;
	bool m_ready {false};
	int m_oldZoomX {0};
	int m_oldZoomY {0};
	QSize m_oldCanvasSize;
	int m_groupTransactions {0};
	UndoTransaction m_groupTransaction;
	bool _isGlobalMode {true};
	bool linkAfterDraw {false};
	bool ImageAfterDraw {false};
	QStack<ViewState> m_viewStates;

private slots:
	void setZoom();
	/**
	 * Called to update the GUI when the canvas(view) scrollbars are moved
	 * @param x 
	 * @param y 
	 */
	void setRulerPos(int x, int y);
	void setObjectUndoMode();
	void setGlobalUndoMode();
	void dragTimerTimeOut();

public:
	void wheelEvent(QWheelEvent *ev) override;
	void changeEvent(QEvent *e) override;
	void nativeGestureEvent(QNativeGestureEvent *e);
	void keyPressEvent(QKeyEvent *k) override;
	void keyReleaseEvent(QKeyEvent *k) override;
	void inputMethodEvent(QInputMethodEvent * event) override;
	QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;
	
	inline void registerMousePress(QPoint p);
	bool mousePressed();
	void resetMousePressed();
	inline QPoint mousePressLocation();
	inline bool moveTimerElapsed();
	inline void resetMoveTimer();
	
	inline void startDragTimer();
	inline void stopDragTimer();
	inline void resetDragTimer();
	inline bool dragTimerElapsed();
	void stopAllDrags();

	bool handleObjectImport(QMimeData* mimeData, TransactionSettings* trSettings = nullptr);

protected: // Protected methods
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	void resizeEvent(QResizeEvent *event) override;
	bool eventFilter(QObject *obj, QEvent *event) override;

	// those appear to be gone from QScrollArea:
	virtual void contentsDragEnterEvent(QDragEnterEvent *e);
	virtual void contentsDragMoveEvent(QDragMoveEvent *e);
	virtual void contentsDragLeaveEvent(QDragLeaveEvent *e);
	virtual void contentsDropEvent(QDropEvent *e);
	virtual void setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h);
	virtual void setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h);
	
	//The width of vertical ruler/height of horizontal ruler, set to 17 in scribusview.cpp
	int m_vhRulerHW { 17 };

signals:
	void unitChanged(int);
	void layerChanged(int);
	void HaveSel();
	void DocChanged();
	void ItemGeom();
	void PolyOpen();
	void ItemCharStyle(const CharStyle&);
	void ItemTextAlign(int);
	void ItemTextEffects(int);
	void AddBM(PageItem *);
	void DelBM(PageItem *);
	void ChBMText(PageItem *);
	void LoadElem(QString, double, double, bool, bool, ScribusDoc *, ScribusView *);
	void HavePoint(bool, bool);
	void ClipPo(double, double);
	void PolyStatus(int, uint);
	void MousePos(double, double);
};




inline void ScribusView::registerMousePress(QPoint p)
{
	m_pressLocation = p;
	m_moveTimer.start();
	m_dragTimerFired = false;
}


inline QPoint ScribusView::mousePressLocation()
{
	return m_pressLocation;
}


inline bool ScribusView::moveTimerElapsed()
{
	return (m_moveTimer.elapsed() > Prefs->uiPrefs.mouseMoveTimeout);
}


inline void ScribusView::resetMoveTimer()
{
	m_moveTimer.start();
}


inline void ScribusView::startDragTimer()
{
	m_dragTimerFired = false;
	m_dragTimer->setSingleShot(true);
	m_dragTimer->start(1000);			// set Timeout for starting a Drag operation to 1 sec.
}

inline void ScribusView::stopDragTimer()
{
	m_dragTimer->stop();
}


inline void ScribusView::resetDragTimer()
{
	m_dragTimerFired = false;
}


inline bool ScribusView::dragTimerElapsed()
{
	return m_dragTimerFired;
}

inline CanvasMode* ScribusView::canvasMode()
{
	return m_canvasMode;
}

#endif
