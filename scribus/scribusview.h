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

#include "styleoptions.h"

#include <vector>
// include files for QT
#include <QScrollArea>
#include <QLineEdit>
#include <QScrollBar>
#if OPTION_USE_QTOOLBUTTON
    #include <QToolButton>
#else
    #include <QPushButton>
#endif
#include <QMap>
#include <QMenu>
#include <QLabel>
#include <QComboBox>
#include <QProgressDialog>
#include <QSpinBox>
#include <QCursor>
#include <QDragLeaveEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QRect>
#include <QRectF>
#include <QTime>
#include <QWheelEvent>
#include <QRubberBand>
#include <QList>

class QEvent;

// application specific includes
#include "observable.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "scrspinbox.h"

class Canvas;
class CanvasMode;
class CanvasGesture;
class Hruler;
class Vruler;
class Page;
class RulerMover;
class PageSelector;
class ScribusWin;
class ScribusMainWindow;
class UndoManager;
class UndoTransaction;



/**
 * This class provides an incomplete base for your application view.
 */

class SCRIBUS_API ScribusView : public QScrollArea, public Observer<QRectF>
{
	Q_OBJECT

public:
    ScribusView(QWidget* win=0, ScribusMainWindow* mw=0, ScribusDoc* doc=0);
    ~ScribusView();
	
	friend class LegacyMode;
	friend class CanvasMode_NodeEdit;
	
	void requestMode(int appMode);
	void startGesture(CanvasGesture*);
	void stopGesture();
	
  /** Vergroesserungseingabefeld */
	ScrSpinBox* zoomSpinBox; //zoom spinbox at bottom of view
	PageSelector* pageSelector; //Page selector at bottom of view
	RulerMover *rulerMover; //Widget between the two rulers for dragging the ruler origin
	Hruler *horizRuler;
	Vruler *vertRuler;
#if OPTION_USE_QTOOLBUTTON
	QToolButton *zoomDefaultToolbarButton;
	QToolButton *zoomOutToolbarButton;
	QToolButton *zoomInToolbarButton;
	QToolButton *cmsToolbarButton;
	QToolButton *previewToolbarButton;
#else
	QPushButton *zoomDefaultToolbarButton;
	QPushButton *zoomOutToolbarButton;
	QPushButton *zoomInToolbarButton;
	QToolButton *cmsToolbarButton;
	QPushButton *previewToolbarButton;
#endif
	QComboBox *layerMenu; //Menu for layers at bottom of view
	QComboBox *unitSwitcher; //Menu for units at bottom of view
	QComboBox *visualMenu;
  /** Dokument zu dem die Seite gehoert */
	ScribusDoc * const Doc;
	Canvas * const m_canvas;
	CanvasMode* m_canvasMode; // might be a CanvasGesture
	QMap<int,CanvasMode*> modeInstances;
	ApplicationPrefs * const Prefs;
	UndoManager * const undoManager;
	ScribusMainWindow* m_ScMW;
	double OldScale;
	double dragX,dragY,dragW,dragH;
	double oldW;
	int RotMode;
	int DrHY;
	int DrVX;
	bool HaveSelRect;
	//bool GroupSel;
	bool DraggedGroup;
	bool DraggedGroupFirst;
	bool MidButt;
	bool updateOn;
	bool FirstPoly;
	bool Magnify;
	bool storedFramesShown;
	bool storedShowControls;
	int redrawMode;
	int redrawCount;
	PageItem *redrawItem;
	QRubberBand *redrawMarker;
	FPoint RCenter;
	void updatesOn(bool on);
	//CB This MUST now be called AFTER a call to doc->addPage or doc->addMasterPage as it
	//does NOT create a page anymore.
	Page* addPage(int nr, bool mov = true);

	void reformPages(bool moveObjects = true);
	void updateLayerMenu();
	void showMasterPage(int nr);
	void hideMasterPage();
	QImage PageToPixmap(int Nr, int maxGr, bool drawFrame = true);
	QImage MPageToPixmap(QString name, int maxGr, bool drawFrame = true);
	void RecalcPicturesRes();
	void rulerMove(QMouseEvent *m);
	/**
	 * Called when the ruler origin is dragged
	 * @param m mouse event
	 */
	void setNewRulerOrigin(QMouseEvent *m);
//	void FromHRuler(QMouseEvent *m);
//	void FromVRuler(QMouseEvent *m);
//	void SetYGuide(QMouseEvent *m, int oldIndex);
//	void SetXGuide(QMouseEvent *m, int oldIndex);
// 	void getClosestGuides(double xin, double yin, double *xout, double *yout);
// 	bool ApplyGuides(double *x, double *y);
// 	void SnapToGuides(PageItem *currItem);
	void getDragRectScreen(double *x, double *y, double *w, double *h);
	void getGroupRectScreen(double *x, double *y, double *w, double *h);
//	void ToView(QPainter *p);
//	void ToView(QMatrix& m);
// 	bool MoveItem(double newX, double newY, PageItem* ite, bool fromMP = false);
	bool PointOnLine(QPoint Start, QPoint Ende, QRect MArea);
	void TransformPoly(int mode, int rot = 1, double scaling = 1.0);
//	void Reset1Control();
//	void ResetControl();
//	void MoveClipPoint(PageItem *currItem, FPoint np);
// 	bool SizeItem(double newX, double newY, int ite, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
// 	bool SizeItem(double newX, double newY, PageItem *pi, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
//	void moveGroup(double x, double y, bool fromMP = false, Selection* customSelection = 0);
// 	void MoveRotated(PageItem *currItem, FPoint npv, bool fromMP = false);
// 	bool MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP = false, bool constrainRotation=false);
//	void RotateGroup(double win);
//	void scaleGroup(double scx, double scy, bool scaleText=true, Selection* customSelection = 0);
// 	void RotateItem(double win, int ite);
// 	void RotateItem(double win, PageItem *currItem);
// 	void AdjustItemSize(PageItem *currItem);
	bool slotSetCurs(int x, int y);
	void HandleCurs(PageItem *currItem, QRect mpo);
//	int HandleSizer(PageItem *currItem, QRect mpo, QMouseEvent *m);
	bool GetItem(PageItem **b, int nr = -1);
	void Deselect(bool prop = true);
	void SelectItemNr(uint nr, bool draw = true, bool single = false);
	void SelectItem(PageItem *pi, bool draw = true, bool single = false);
//	void selectPage(QMouseEvent *m);
//	bool SeleItem(QMouseEvent *m);
//	void SetupDraw(int Nr);
//	void SetupDrawNoResize(int nr);
	void SetFrameRect();
	void SetFrameRounded();
	void SetFrameOval();
	void PasteItem(struct CopyPasteBuffer *Buffer, bool loading, bool drag = false, bool noResize = true);
//	void QueryFarben();
	void rememberOldZoomLocation(int mx=0, int my=0);
	bool groupTransactionStarted() { return m_groupTransactions > 0; }
//	void setGroupTransactionStarted(bool isOn);
	void startGroupTransaction(const QString &actionName = "",
							   const QString &description = "",
							   QPixmap *actionPixmap = 0,
							   Selection* customSelection = 0);
	void endGroupTransaction();
	void cancelGroupTransaction();
	void setScale(const double newScale);
	double scale() const;

	virtual void changed(QRectF re);

	void updateCanvas(QRectF box = QRectF());
	void updateCanvas(double x, double y, double width, double height) { updateCanvas(QRectF(x,y,width,height)); }
	void setCanvasOrigin(double x, double y);
	void setCanvasCenter(double x, double y);
	void scrollCanvasBy(double deltaX, double deltaY);
	FPoint canvasOrigin() const;
	QRectF visibleCanvas() const;
	
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
	void setContentsPos(int x, int y);
	int visibleWidth() { return viewport()->size().width(); } ;
	int visibleHeight() { return viewport()->size().height(); } ;
	void stopAllDrags();
	void scrollBy(int x, int y);
	void zoom(int globalX, int globalY, double scale, bool preservePoint);

public slots: // Public slots
	void languageChange();
	void toggleCMS();
	void switchPreviewVisual(int vis);
	void togglePreview();
	void unitChange();
	void setRulersShown(bool isShown);
	void slotUpdateContents();
	void slotUpdateContents(const QRect &r);
  /** Zooms in or out */
	void slotDoZoom();
	void slotZoom100();
  /** Zooms in */
	void slotZoomIn(int mx=0,int my=0);
	void slotZoomOut(int mx=0,int my=0);
  /** Redraws everything */
	void DrawNew();
	void setMenTxt(int Seite);
	void setLayerMenuText(const QString &);
	void GotoPa(int Seite);
	void GotoLa(int l);
	void GotoPage(int Seite);
	void ChgUnit(int art);
	void SetCPo(double x, double y);
	void SetCCPo(double x, double y);
	void editExtendedImageProperties();
	//void RefreshItem(PageItem *currItem);
	void RefreshGradient(PageItem *currItem, double dx = 8, double dy = 8);
	/**
	 * Adjust an image frame's size to fit the size of the image in it
	 */
// 	void adjustFrametoImageSize();
	void ToggleBookmark();
	void ToggleAnnotation();
// 	void sentToScrap();
	void ToBack();
	void ToFront();
	void LowerItem();
	void RaiseItem();
	void ToPicFrame();
	void ToPolyFrame();
	void ToTextFrame();
	void ToBezierFrame();
	void ToPathText();
	void FromPathText();
// 	void UniteObj();
// 	void SplitObj();
	void Bezier2Poly();
	void PasteToPage();
//	void PasteRecentToPage(int id);
	void TextToPath();
	void adjustCanvas(double width, double height, double dX=0.0, double dY=0.0);
	
private: // Private attributes
	int m_previousMode;
	QMenu *pmen3;
	QMenu *pmenResolution;
	QPoint m_pressLocation;
	QTime m_moveTimer;
	QTimer *m_dragTimer;
	bool m_dragTimerFired;
	bool Ready;
	int  oldX;
	int  oldY;
	int  m_groupTransactions;
	UndoTransaction*  m_groupTransaction;
	bool _isGlobalMode;
//	bool forceRedraw;

	double oldItemX;
	double oldItemY;

private slots:
	void setZoom();
	/**
	 * Called to update the GUI when the canvas(view) scrollbars are moved
	 * @param x 
	 * @param y 
	 */
	void setRulerPos(int x, int y);
	void selectionChanged();
	void setObjectUndoMode();
	void setGlobalUndoMode();
	void dragTimerTimeOut();

public:
	virtual void wheelEvent ( QWheelEvent *ev );
	virtual void changeEvent(QEvent *e);

	void keyPressEvent(QKeyEvent *k);
	void keyReleaseEvent(QKeyEvent *k);

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

protected: // Protected methods
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void resizeEvent ( QResizeEvent * event );
	bool eventFilter(QObject *obj, QEvent *event);

	// those appear to be gone from QScrollArea:
	virtual void contentsDragEnterEvent(QDragEnterEvent *e);
	virtual void contentsDragMoveEvent(QDragMoveEvent *e);
	virtual void contentsDragLeaveEvent(QDragLeaveEvent *e);
	virtual void contentsDropEvent(QDropEvent *e);
	virtual void setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h);
	virtual void setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h);
	void scrollContentsBy(int dx, int dy);
	
	//The width of vertical ruler/height of horizontal ruler, set to 17 in scribusview.cpp
	int m_vhRulerHW;

signals:
	void changeUN(int);
	void changeLA(int);
	void ItemPos(double, double);
	void HaveSel(int);
	void ItemGeom(double, double);
	void DocChanged();
	void PolyOpen();
	void PStatus(int, uint);
	void SetAngle(double);
	void SetSizeValue(double);
	void SetLineArt(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle);
	void SetLocalValues(double, double, double, double);
//	void ItemFarben(QString, QString, int, int);
//	void ItemGradient(int);
//	void ItemTrans(double, double);
//	void ItemBlend(int, int);
	void ItemTextAttr(double);
	void ItemTextUSval(double);
	void ItemTextCols(int, double);
	void SetDistValues(double, double, double, double);
	void ItemTextAbs(int);
	void ItemTextFont(const QString&);
	void ItemTextSize(double);
	void ItemTextSca(double);
	void ItemTextScaV(double);
	void ItemTextBase(double);
	void ItemTextOutline(double);
	void ItemTextShadow(double, double);
	void ItemTextUnderline(double, double);
	void ItemTextStrike(double, double);
	void ItemTextFarben(QString, QString, double, double);
	void ItemTextStil(int);
	//void ItemRadius(double);
	void HasTextSel();
	void HasNoTextSel();
	void MVals(double, double, double, double, double, double, int);
	void PaintingDone();
	void LoadPic();
	void StatusPic();
	void AppendText();
	void DoGroup();
	//void DoUnGroup();
	void CutItem();
	void CopyItem();
	void Amode(int);
	void AddBM(PageItem *);
	void DelBM(PageItem *);
	void ChBMText(PageItem *);
	void ToScrap(QString);
	void LoadElem(QString, double, double, bool, bool, ScribusDoc *, ScribusView *);
	void LevelChanged(uint);
	void HavePoint(bool, bool);
	void ClipPo(double, double);
	void AnnotProps();
	void EndNodeEdit();
	void Hrule(int);
	void Vrule(int);
	//void EditGuides();
	void MousePos(double, double);
	void callGimp();
	void signalGuideInformation(int, double);
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
	return (m_moveTimer.elapsed() > Prefs->moveTimeout);
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

#endif
