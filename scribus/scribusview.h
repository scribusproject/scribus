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
#include <qscrollview.h>
#include <qptrlist.h>
#include <qlineedit.h>
#include <qscrollbar.h>
#if OPTION_USE_QTOOLBUTTON
    #include <qtoolbutton.h>
#else
    #include <qpushbutton.h>
#endif
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qprogressdialog.h>
#include <qspinbox.h>
#include <qcursor.h>
// application specific includes
#include "scribusapi.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "scpainter.h"
class Page;
#include "mspinbox.h"
//#include "util.h"
class Hruler;
class Vruler;
class UndoManager;
class RulerMover;
class PageSelector;
class ScribusWin;
class ScribusMainWindow;

/**
 * This class provides an incomplete base for your application view.
 */

class SCRIBUS_API ScribusView : public QScrollView
{
	Q_OBJECT

public:
    ScribusView(QWidget* win=0, ScribusMainWindow* mw=0, ScribusDoc* doc=0);
    ~ScribusView() {};
  /** Vergroesserungseingabefeld */
	MSpinBox* zoomSpinBox; //zoom spinbox at bottom of view
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
	ApplicationPrefs * const Prefs;
	UndoManager * const undoManager;
	double OldScale;
	double dragX,dragY,dragW,dragH;
	double oldW;
	int oldCp;
	int Mxp;
	int Myp;
	int dragConstrainInitPtX;
	int dragConstrainInitPtY;
	int Dxp;
	int Dyp;
	int m_SnapCounter;
	/*!
	 * Frame handle used for resize
	 * 283
	 * 7 6
	 * 451
	 */
	int frameResizeHandle;
	int SeRx;
	int SeRy;
	int GyM;
	int GxM;
	int ClRe;
	int ClRe2;
	int SegP1;
	int SegP2;
	int RotMode;
	int DrHY;
	int DrVX;
	bool EdPoints;
	bool m_MouseButtonPressed;
	bool operItemMoving;
	bool shiftSelItems;
	bool MoveGY;
	bool MoveGX;
	bool HaveSelRect;
	bool operItemResizing;
	bool EditContour;
	//bool GroupSel;
	bool DraggedGroup;
	bool DraggedGroupFirst;
	bool operItemResizeInEditMode;
	bool inItemCreation;
	bool MidButt;
	bool updateOn;
	bool FirstPoly;
	bool Magnify;
	bool MoveSym;
	bool previewMode;
	bool viewAsPreview;
	bool storedFramesShown;
	bool storedShowControls;
	int previewVisual;
	FPoint RCenter;
	FPointArray RecordP;
	void DrawMasterItems(ScPainter *painter, Page *page, QRect clip);
	void DrawPageItems(ScPainter *painter, QRect clip);
	void DrawPageMarks(ScPainter *p, Page* page, QRect clip);
	void updatesOn(bool on);
	//CB This MUST now be called AFTER a call to doc->addPage or doc->addMasterPage as it
	//does NOT create a page anymore.
	Page* addPage(int nr, bool mov = true);
	QPtrList<PageItem> linkedFramesToShow;
	QValueList<int> SelNode;

	
	struct oldPageVar
	{
		uint newPg;
		double oldXO;
		double oldYO;
	};
	static const uint moveWithFullOutlinesThreshold = 21;
	static const uint moveWithBoxesOnlyThreshold = 41;
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
	void FromHRuler(QMouseEvent *m);
	void FromVRuler(QMouseEvent *m);
	void SetYGuide(QMouseEvent *m, int oldIndex);
	void SetXGuide(QMouseEvent *m, int oldIndex);
	void getClosestGuides(double xin, double yin, double *xout, double *yout);
	bool ApplyGuides(double *x, double *y);
	void SnapToGuides(PageItem *currItem);
// 	void setGroupRect();
// 	void getGroupRect(double *x, double *y, double *w, double *h);
	void getDragRectScreen(double *x, double *y, double *w, double *h);
	void getGroupRectScreen(double *x, double *y, double *w, double *h);
	void paintGroupRect(bool norm = true);
	void PaintSizeRect(QPainter *p, QRect neu);
	void ToView(QPainter *p);
	void ToView(QWMatrix& m);
	bool MoveItem(double newX, double newY, PageItem* ite, bool fromMP = false);
	void MarkClip(PageItem *currItem, FPointArray cli, bool once = false);
	bool PointOnLine(QPoint Start, QPoint Ende, QRect MArea);
	void TransformPoly(int mode, int rot = 1, double scaling = 1.0);
	void Reset1Control();
	void ResetControl();
	void MoveClipPoint(PageItem *currItem, FPoint np);
	bool SizeItem(double newX, double newY, int ite, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	bool SizeItem(double newX, double newY, PageItem *pi, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	void moveGroup(double x, double y, bool fromMP = false, Selection* customSelection = 0);
	void MoveRotated(PageItem *currItem, FPoint npv, bool fromMP = false);
	bool MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP = false, bool constrainRotation=false);
	void RotateGroup(double win);
	void scaleGroup(double scx, double scy, bool scaleText=true, Selection* customSelection = 0);
	void RotateItem(double win, int ite);
	void RotateItem(double win, PageItem *currItem);
	void AdjustItemSize(PageItem *currItem);
	bool slotSetCurs(int x, int y);
	void slotDoCurs(bool draw);
	void HandleCurs(QPainter *p, PageItem *currItem, QRect mpo);
	void HandleSizer(QPainter *p, PageItem *currItem, QRect mpo, QMouseEvent *m);
	bool GetItem(PageItem **b, int nr = -1);
	void Deselect(bool prop = true);
	void SelectItemNr(uint nr, bool draw = true, bool single = false);
	void SelectItem(PageItem *pi, bool draw = true, bool single = false);
	void selectPage(QMouseEvent *m);
	bool SeleItem(QMouseEvent *m);
	void SetupDraw(int Nr);
	void SetupDrawNoResize(int nr);
	void Transform(PageItem *currItem, QPainter *p);
	void Transform(PageItem *currItem, QWMatrix& m);
	void TransformM(PageItem *currItem, QPainter *p);
	void SetFrameRect();
	void SetFrameRounded();
	void SetFrameOval();
	void PasteItem(struct CopyPasteBuffer *Buffer, bool loading, bool drag = false, bool noResize = true);
	void QueryFarben();
	void rememberOldZoomLocation(int mx=0, int my=0);
	bool mousePressed();
	bool groupTransactionStarted();
	void setGroupTransactionStarted(bool isOn);
	void setScale(const double newScale);
	double scale() const { return Scale; }
	void normalizeSelectionRect();
	FPoint translateToView(double x, double y);
	FPoint translateToView(FPoint in);
	FPoint translateToDoc(double x, double y);
	FPoint translateToDoc(FPoint in);
	FPoint translateFromViewport(double x, double y);
	FPoint translateFromViewport(FPoint in);
	FPoint translateToViewport(double x, double y);
	FPoint translateToViewport(FPoint in);

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
	void RefreshItem(PageItem *currItem);
	void RefreshGradient(PageItem *currItem, double dx = 8, double dy = 8);
	/**
	 * Adjust an image frame's size to fit the size of the image in it
	 */
	void adjustFrametoImageSize();
	void ToggleBookmark();
	void ToggleAnnotation();
	void sentToScrap();
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
	void UniteObj();
	void SplitObj();
	void Bezier2Poly();
	void PasteToPage();
	void PasteRecentToPage(int id);
	void TextToPath();
	void blinkCursor();
	void adjustCanvas(double width, double height, double dX=0.0, double dY=0.0);

private: // Private attributes
	QPopupMenu *pmen3;
	QPopupMenu *pmenResolution;
	QTime moveTimer;
	bool Ready;
	int  oldX;
	int  oldY;
	bool _groupTransactionStarted;
	bool _isGlobalMode;
	bool evSpon;
	bool forceRedraw;
	double Scale;

	// for shape/countour line editing undo actions
	// store oldClip in mousePressed if in edit shape
	// and in mouseRelease send the undo action with the 
	// new clip
	bool isContourLine;
	FPointArray *oldClip;
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

protected: // Protected methods
	virtual void viewportPaintEvent ( QPaintEvent * p );
	virtual void drawContents(QPainter *p, int clipx, int clipy, int clipw, int cliph);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void contentsDragEnterEvent(QDragEnterEvent *e);
	virtual void contentsDragMoveEvent(QDragMoveEvent *e);
	virtual void contentsDragLeaveEvent(QDragLeaveEvent *e);
	virtual void contentsDropEvent(QDropEvent *e);
	virtual void contentsMouseDoubleClickEvent(QMouseEvent *m);
	virtual void contentsMouseReleaseEvent(QMouseEvent *m);
	virtual void contentsMouseMoveEvent(QMouseEvent *m);
	virtual void contentsMousePressEvent(QMouseEvent *m);
	virtual void contentsWheelEvent ( QWheelEvent *ev );
	virtual void setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h);
	virtual void setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h);

	void drawLinkFrameLine(ScPainter* painter, FPoint &start, FPoint &end);
	inline bool moveTimerElapsed();
	
	//The width of vertical ruler/height of horizontal ruler, set to 17 in scribusview.cpp
	int m_vhRulerHW;
	bool m_cursorVisible;
	ScribusMainWindow* m_ScMW;

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
	void ItemFarben(QString, QString, int, int);
	void ItemGradient(int);
	void ItemTrans(double, double);
	void ItemBlend(int, int);
	void ItemTextAttr(double);
	void ItemTextUSval(int);
	void ItemTextCols(int, double);
	void SetDistValues(double, double, double, double);
	void ItemTextAbs(int);
	void ItemTextFont(QString);
	void ItemTextSize(int);
	void ItemTextSca(int);
	void ItemTextScaV(int);
	void ItemTextBase(int);
	void ItemTextOutline(int);
	void ItemTextShadow(int, int);
	void ItemTextUnderline(int, int);
	void ItemTextStrike(int, int);
	void ItemTextFarben(QString, QString, int, int);
	void ItemTextStil(int);
	//void ItemRadius(double);
	void HasTextSel();
	void HasNoTextSel();
	void MVals(double, double, double, double, double, double, int);
	void PaintingDone();
	void LoadPic();
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
	void RasterPic(bool);
	void HavePoint(bool, bool);
	void ClipPo(double, double);
	void AnnotProps();
	void EndNodeEdit();
	void Hrule(int);
	void Vrule(int);
	//void EditGuides();
	void MousePos(double, double);
	void callGimp();
	void AddObj(PageItem *);
	void UpdtObj(uint, uint);
	void signalGuideInformation(int, double);
};
#endif
