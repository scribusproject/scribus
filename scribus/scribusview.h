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
// application specific includes
#include "scribusapi.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "scpainter.h"
#include "page.h"
#include "mspinbox.h"
#include "util.h"
class Hruler;
class Vruler;
class UndoManager;
class RulerMover;
//#include "pageselector.h"
class PageSelector;


/**
 * This class provides an incomplete base for your application view.
 */

class SCRIBUS_API ScribusView : public QScrollView
{
	Q_OBJECT

public:
    ScribusView(QWidget *parent=0, ScribusDoc *doc=0);
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
#else
	QPushButton *zoomDefaultToolbarButton;
	QPushButton *zoomOutToolbarButton;
	QPushButton *zoomInToolbarButton;
#endif
	QComboBox *layerMenu; //Menu for layers at bottom of view
	QComboBox *unitSwitcher; //Menu for units at bottom of view
  /** Dokument zu dem die Seite gehoert */
	ScribusDoc *Doc;
	ApplicationPrefs *Prefs;
	UndoManager *undoManager;
	double OldScale;
	double GroupX;
	double GroupY;
	double GroupW;
	double GroupH;
	double oldW;
	int oldCp;
	int Mxp;
	int Myp;
	int Dxp;
	int Dyp;
	int HowTo;
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
	bool Mpressed;
	bool Imoved;
	bool MoveGY;
	bool MoveGX;
	bool HaveSelRect;
	bool mCG;
	bool EditContour;
	bool GroupSel;
	bool DraggedGroup;
	bool DraggedGroupFirst;
	bool HanMove;
	bool MidButt;
	bool updateOn;
	bool FirstPoly;
	bool Magnify;
	bool MoveSym;
	bool CursVis;
	bool previewMode;
	FPoint RCenter;
	FPointArray RecordP;
	void DrawMasterItems(ScPainter *painter, Page *page, QRect clip);
	void DrawPageItems(ScPainter *painter, QRect clip);
	void DrawPageMarks(ScPainter *p, Page* page, QRect clip);
	Page* addPage(int nr, bool mov = true);
	QPtrList<PageItem> SelItem;
	QPtrList<PageItem> linkedFramesToShow;
	QValueList<int> SelNode;

	QValueList<AlignObjs> AObjects;
	struct oldPageVar
	{
		uint newPg;
		double oldXO;
		double oldYO;
	};
  /** Deletes a Page */
	void delPage(int Nr);
	void reformPages(bool moveObjects = true);
	void updateLayerMenu();
	void showMasterPage(int nr);
	void hideMasterPage();
	int CountElements();
	QImage PageToPixmap(int Nr, int maxGr);
	QImage MPageToPixmap(QString name, int maxGr);
	void RecalcPictures(ProfilesL *Pr, QProgressBar *dia = 0);
	void RecalcPicturesRes();
	void rulerMove(QMouseEvent *m);
	void setRuler(QMouseEvent *m);
	void FromHRuler(QMouseEvent *m);
	void FromVRuler(QMouseEvent *m);
	void SetYGuide(QMouseEvent *m, int oldIndex);
	void SetXGuide(QMouseEvent *m, int oldIndex);
	bool ApplyGuides(double *x, double *y);
	void SnapToGuides(PageItem *currItem);
	QPoint ApplyGrid(QPoint in);
	FPoint ApplyGridF(FPoint in);
	//CB moved to pageitem QRect getRedrawBounding(PageItem *currItem);
	void setRedrawBounding(PageItem *currItem);
	void setGroupRect();
	void getGroupRect(double *x, double *y, double *w, double *h);
	void getGroupRectScreen(double *x, double *y, double *w, double *h);
	void paintGroupRect(bool norm = true);
	void PaintSizeRect(QPainter *p, QRect neu);
	void ToView(QPainter *p);
	void RefreshItem(PageItem *currItem);
	bool MoveItem(double newX, double newY, PageItem* ite, bool fromMP = false);
	void MoveItemI(double newX, double newY, int ite, bool redraw = true);
	void UpdateClip(PageItem *currItem);
	void SetPolyClip(PageItem *currItem, int up);
	void ConvertClip(PageItem *currItem);
	void UpdatePolyClip(PageItem *currItem);
	void MarkClip(PageItem *currItem, FPointArray cli, bool once = false);
	bool PointOnLine(QPoint Start, QPoint Ende, QRect MArea);
	void MirrorPolyH();
	void MirrorPolyV();
	void TransformPoly(int mode, int rot = 1, double scaling = 1.0);
	void Reset1Control();
	void ResetControl();
	void MoveClipPoint(PageItem *currItem, FPoint np);
	bool SizeItem(double newX, double newY, int ite, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	bool SizeItem(double newX, double newY, PageItem *pi, bool fromMP, bool DoUpdateClip, bool redraw);
	void moveGroup(double x, double y, bool fromMP = false);
	void MoveRotated(PageItem *currItem, FPoint npv, bool fromMP = false);
	bool MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP = false);
	void RotateGroup(double win);
	void scaleGroup(double scx, double scy);
	void RotateItem(double win, int ite);
	void RotateItem(double win, PageItem *pi);
	void AdjustItemSize(PageItem *currItem);
	void AdvanceSel(PageItem *currItem, int oldPos, int len, int dir, int expandSel, int state);
	void setNewPos(PageItem *currItem, int oldPos, int len, int dir);
	void ExpandSel(PageItem *currItem, int dir, int oldPos);
	void deselectAll(PageItem *currItem);
	bool slotSetCurs(int x, int y);
	void slotDoCurs(bool draw);
	void HandleCurs(QPainter *p, PageItem *currItem, QRect mpo);
	void HandleSizer(QPainter *p, PageItem *currItem, QRect mpo, QMouseEvent *m);
	bool GetItem(PageItem **b, int nr = -1);
	void Deselect(bool prop = true);
	//int OnPage(double x2, double  y2);
	//int OnPage(PageItem *currItem);
	//void GroupOnPage(PageItem *currItem);
	void SelectItemNr(int nr, bool draw = true, bool single = false);
	void SelectItem(PageItem *pi, bool draw = true, bool single = false);
	void selectPage(QMouseEvent *m);
	bool SeleItem(QMouseEvent *m);
	void SetupDraw(int Nr);
	void updateGradientVectors(PageItem *currItem);
	void EmitValues(PageItem *currItem);
	void Transform(PageItem *currItem, QPainter *p);
	void TransformM(PageItem *currItem, QPainter *p);
	void SetFrameRect();
	void SetFrameRounded();
	void SetFrameOval();
	void insertColor(QString nam, double c, double m, double y, double k);
	void ChLineWidth(double w);
	void ChLineArt(PenStyle w);
	void ChLineJoin(PenJoinStyle w);
	void ChLineEnd(PenCapStyle w);
	void ChLineSpa(double w);
	void ChLineSpaMode(int w);
	void ChLocalXY(double x, double y);
	void ChLocalSc(double x, double y);
	void ItemFont(QString fon);
	void ItemPen(QString farbe);
	void ItemTextBrush(QString farbe);
	void ItemTextBrushS(int sha);
	void ItemTextPen(QString farbe);
	void ItemTextPenS(int sha);
	void ItemTextScaleV(int sha);
	void ItemTextScale(int sha);
	void setItemTextBase(int sha);
	void setItemTextOutline(int sha);
	void setItemTextShadow(int shx, int shy);
	void setItemTextUnderline(int pos, int wid);
	void setItemTextStrike(int pos, int wid);
	void ItemBrush(QString farbe);
	void ItemBrushShade(int sha);
	void ItemPenShade(int sha);
	void ItemGradFill(int typ);
	void chTyStyle(int s);
	void SetAbStyle(int s);
	void chAbStyle(PageItem *currItem, int s);
	void chKerning(int us);
	void chFSize(int size);
	void FlipImageH();
	void FlipImageV();
	//void AdjustPictScale(PageItem *currItem, bool reload = true);
	void PasteItem(struct CopyPasteBuffer *Buffer, bool loading, bool drag = false);
	void BuildAObj();
	void QueryFarben();
	void rememberPreviousSettings(int mx=0, int my=0);
	bool mousePressed();
	bool groupTransactionStarted();
	void setGroupTransactionStarted(bool isOn);
	void setScale(const double newScale);
	const double getScale();
	void adjustCanvas(FPoint minPos, FPoint maxPos);

public slots: // Public slots
	void languageChange();
	void unitChange();
  /** Zooms in or out */
	void slotDoZoom();
	void slotZoom100();
  /** Zooms in */
	void slotZoomIn(int mx=0,int my=0);
	void slotZoomIn2(int mx=0,int my=0);
	void slotZoomOut(int mx=0,int my=0);
	void slotZoomOut2(int mx=0,int my=0);
  /** Redraws everything */
	void DrawNew();
	void setMenTxt(int Seite);
	void setLayerMenuText(const QString &);
	void GotoPa(int Seite);
	void GotoLa(int l);
	void GotoPage(int Seite);
	void ChgUnit(int art);
	void SetCPo(int x, int y);
	void SetCCPo(int x, int y);
	void editExtendedImageProperties();
	void TogglePic();
	void UpdatePic();
	void FrameToPic();
	void ToggleBookmark();
	void ToggleAnnotation();
	void ToggleLock();
	void ToggleSizeLock();
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
	void ClearItem();
	void DeleteItem();
	void PasteToPage();
	void TextToPath();
	void BlinkCurs();
	void updatePict(QString name);
	void removePict(QString name);
	void changePreview(int id);

private: // Private attributes
	QPopupMenu *pmen3;
	QPopupMenu *pmenResolution;
	bool Ready;
	int    oldX;
	int    oldY;
	bool _groupTransactionStarted;
	bool _isGlobalMode;
	bool evSpon;
	bool forceRedraw;
	double Scale;

private slots:
	void Zval();
	void setRulerPos(int x, int y);
	void selectionChanged();
	void setObjectUndoMode();
	void setGlobalUndoMode();

protected: // Protected methods
	virtual void viewportPaintEvent ( QPaintEvent * p );
	virtual void drawContents(QPainter *p, int clipx, int clipy, int clipw, int cliph);
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
	void SetLineArt(PenStyle, PenCapStyle, PenJoinStyle);
	void SetLocalValues(double, double, double, double);
	void ItemFarben(QString, QString, int, int);
	void ItemGradient(int);
	void ItemTrans(double, double);
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
	void ItemRadius(double);
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
	void NewBMNr(int, int);
	void ChBMText(PageItem *);
	void ToScrap(QString);
	void LoadElem(QString, int, int, bool, bool, ScribusDoc *, ScribusView *);
	void LevelChanged(uint);
	void RasterPic(bool);
	void HavePoint(bool, bool);
	void ClipPo(double, double);
	void AnnotProps();
	void EndNodeEdit();
	void Hrule(int);
	void Vrule(int);
	void EditGuides();
	void MousePos(double, double);
	void callGimp();
	void AddObj(PageItem *);
	void UpdtObj(uint, uint);
	void signalGuideInformation(int, double);
};
#endif
