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
#include <qscrollview.h>
#include <qptrlist.h>
#include <qlineedit.h>
#include <qscrollbar.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qprogressdialog.h>
#include <qspinbox.h>
// application specific includes
#include "scribusdoc.h"
#include "scpainter.h"
#include "page.h"
#include "hruler.h"
#include "vruler.h"
#include "mspinbox.h"
#include "pageselector.h"

extern QPixmap loadIcon(QString nam);

/**
 * This class provides an incomplete base for your application view. 
 */

class ScribusView : public QScrollView
{
	Q_OBJECT

public:
    ScribusView(QWidget *parent=0, ScribusDoc *doc=0, preV *prefs=0);
    ~ScribusView() {};
  /** Vergroesserungseingabefeld */
	MSpinBox* LE;
	PageSelector* PGS;
	QToolButton *UN;
	Hruler *HR;
	Vruler *VR;
  /** Dokument zu dem die Seite gehoert */
	ScribusDoc *Doc;
	preV *Prefs;
	double Scale;
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
	FPoint RCenter;
	FPointArray RecordP;
	void DrawMasterItems(ScPainter *painter, Page *page, QRect clip);
	void DrawPageItems(ScPainter *painter, QRect clip);
	void DrawPageMarks(ScPainter *p, Page* page, QRect clip);
	Page* addPage(int nr);
	QPtrList<PageItem> SelItem;
	QPtrList<PageItem> linkedFramesToShow;
	QValueList<int> SelNode;
  /** Deletes a Page */
	void delPage(int Nr);
  /** verschiebt Seiten */
	void movePage(int from, int to, int ziel, int art);
	void reformPages();
	void LaMenu();
	void ShowTemplate(int nr);
	void HideTemplate();
	int CountElements();
	QPixmap PageToPixmap(int Nr, int maxGr);
	QPixmap MPageToPixmap(QString name, int maxGr);
	void RecalcPictures(ProfilesL *Pr, QProgressBar *dia = 0);
	void FromHRuler(QMouseEvent *m);
	void FromVRuler(QMouseEvent *m);
	void SetYGuide(QMouseEvent *m);
	void SetXGuide(QMouseEvent *m);
	bool ApplyGuides(double *x, double *y);
	void SnapToGuides(PageItem* b);
	QPoint ApplyGrid(QPoint in);
	FPoint ApplyGridF(FPoint in);
	FPoint transformPointI(FPoint in, double dx, double dy, double rot, double sx, double sy);
	FPoint transformPoint(FPoint in, double dx, double dy, double rot, double sx, double sy);
	void getBoundingRect(PageItem *b, double *x1, double *y1, double *x2, double *y2);
	QRect getRedrawBounding(PageItem* b);
	void setRedrawBounding(PageItem* b);
	void setGroupRect();
	void getGroupRect(double *x, double *y, double *w, double *h);
	void getGroupRectScreen(double *x, double *y, double *w, double *h);
	void paintGroupRect(bool norm = true);
	void PaintSizeRect(QPainter *p, QRect neu);
	void ToView(QPainter *p);
	void RefreshItem(PageItem *b);
	bool MoveItem(double newX, double newY, PageItem* ite, bool fromMP = false);
	void MoveItemI(double newX, double newY, int ite);
	void UpdateClip(PageItem* b);
	void SetPolyClip(PageItem *b, int up);
	void ConvertClip(PageItem *b);
	void UpdatePolyClip(PageItem *b);
	void MarkClip(PageItem *b);
	bool PointOnLine(QPoint Start, QPoint Ende, QRect MArea);
	void MirrorPolyH();
	void MirrorPolyV();
	void TransformPoly(int mode, int rot = 1, int scaling = 1);
	void Reset1Control();
	void ResetControl();
	void MoveClipPoint(PageItem *b, FPoint np);
	bool SizeItem(double newX, double newY, int ite, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	void moveGroup(double x, double y, bool fromMP = false);
	void MoveRotated(PageItem *b, FPoint npv, bool fromMP = false);
	bool MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP = false);
	void RotateGroup(double win);
	void scaleGroup(double scx, double scy);
	void RotateItem(double win, int ite);
	void AdjustItemSize(PageItem *b);
	FPoint GetMinClipF(FPointArray Clip);
	void AdvanceSel(PageItem *b, int oldPos, int len, int dir, int expandSel, int state);
	void setNewPos(PageItem *b, int oldPos, int len, int dir);
	void ExpandSel(PageItem *b, int dir, int oldPos);
	void deselectAll(PageItem *b);
	bool slotSetCurs(int x, int y);
	void slotDoCurs(bool draw);
	void HandleCurs(QPainter *p, PageItem *b, QRect mpo);
	void HandleSizer(QPainter *p, PageItem *b, QRect mpo);
	void storeUndoInf(PageItem* b);
	bool GetItem(PageItem **b, int nr = -1);
	void Deselect(bool prop = true);
	int OnPage(double x2, double  y2);
	int OnPage(PageItem *b);
	void GroupOnPage(PageItem* b);
	void SelectItemNr(int nr, bool draw = true);
	bool SeleItem(QMouseEvent *m);
	void SetupDraw(int Nr);
	void updateGradientVectors(PageItem *b);
	void EmitValues(PageItem *b);
	void Transform(PageItem *b, QPainter *p);
	void TransformM(PageItem *b, QPainter *p);
	void SetFrameShape(PageItem *b, int count, double *vals);
	void SetRectFrame(PageItem *b);
	void SetOvalFrame(PageItem *b);
	void SetFrameRound(PageItem *b);
	void SetFrameRect();
	void SetFrameRounded();
	void SetFrameOval();
	int PaintEllipse(double x, double y, double b, double h, double w, QString fill, QString outline);
	int PaintPict(double x, double y, double b, double h);
	int PaintRect(double x, double y, double b, double h, double w, QString fill, QString outline);
	int PaintLine(double x, double y, double b, double h, double w, QString outline);
	int PaintText(double x, double y, double b, double h, double w, QString outline);
	int PaintPoly(double x, double y, double b, double h, double w, QString fill, QString outline);
	int PaintPolyLine(double x, double y, double b, double h, double w, QString fill, QString outline);
	void insertColor(QString nam, double c, double m, double y, double k);
	void ChLineWidth(double w);
	void ChLineArt(PenStyle w);
	void ChLineJoin(PenJoinStyle w);
	void ChLineEnd(PenCapStyle w);
	void ChLineSpa(double w);
	void ChLocalXY(double x, double y);
	void ChLocalSc(double x, double y);
	void ItemFont(QString fon);
	void ItemPen(QString farbe);
	void ItemTextBrush(QString farbe);
	void ItemTextBrushS(int sha);
	void ItemTextPen(QString farbe);
	void ItemTextPenS(int sha);
	void ItemTextScale(int sha);
	void ItemBrush(QString farbe);
	void ItemBrushShade(int sha);
	void ItemPenShade(int sha);
	void ItemGradFill(int typ);
	void chTyStyle(int s);
	void SetAbStyle(int s);
	void chAbStyle(PageItem *b, int s);
	void chKerning(double us);
	void chFSize(int size);
	void FlipImageH();
	void FlipImageV();
	void LoadPict(QString fn, int ItNr);
	void AdjustPreview(PageItem *b, bool reload = true);
	void AdjustPictScale(PageItem *b, bool reload = true);
	void PasteItem(struct CLBuf *Buffer, bool loading, bool drag = false);
	void BuildAObj();
	void doAlign(QValueList<uint> &Object, int moveCode, double xp, double xdisp, double ydisp, double minx = 0.0);
	void AlignObj(bool xa, bool ya, bool Vth, bool Vtv, double xdisp, double ydisp, int xart, int yart);
	struct AlignObjs {  int ObjNr;
								int Group;
								double x1;
								double y1;
								double x2;
								double y2;
								QPtrList<PageItem> Objects;
							};
	QValueList<AlignObjs> AObjects;
	void QueryFarben();
	void ToPathText();
	void FromPathText();
	void UniteObj();
	void SplitObj();
	void rememberPreviousSettings(int mx=0, int my=0);

public slots: // Public slots
  /** Fhrt die Vergr�erung/Verkleinerung aus; */
	void slotDoZoom();
  /** Vergr�ert die Ansicht */
	void slotZoomIn(int mx=0,int my=0);
  /** Verkleinert die Ansicht */
	void slotZoomOut(int mx=0,int my=0);
  /** Vergr�ert die Ansicht */
	void slotZoomIn2(int mx=0,int my=0);
  /** Verkleinert die Ansicht */
	void slotZoomOut2(int mx=0,int my=0);
  /** Redraws everything */
	void DrawNew();
	void setMenTxt(int Seite);
	void setLayMenTxt(int l);
	void GotoPa(int Seite);
	void GotoLa(int l);
	void GotoPage(int Seite);
	void ChgUnit(int art);
	void SetCPo(int x, int y);
	void SetCCPo(int x, int y);
	void TogglePic();
	void UpdatePic();
	void FrameToPic();
	void GimpExited();
	void CallGimp();
	void ToggleBookmark();
	void ToggleAnnotation();
	void ToggleLock();
	void ToggleResize();
	void sentToScrap();
	void sentToLayer(int id);
	void LoremIpsum();
	void ToBack();
	void ToFront();
	void LowerItem();
	void RaiseItem();
	void ToPicFrame();
	void ToPolyFrame();
	void ToTextFrame();
	void ToBezierFrame();
	void Bezier2Poly();
	void ClearItem();
	void DeleteItem();
	void PasteToPage();
	void TextToPath();
	void BlinkCurs();

private: // Private attributes
	QPushButton *SB1;
	QPushButton *SB2;
	QPushButton *LY;
	QPopupMenu *Unitmen;
	QPopupMenu *Laymen;
	QPopupMenu *pmen3;
	bool Ready;
	int    oldX;
	int    oldY;
	
private slots:
	void Zval();
	void setRulerPos(int x, int y);

protected: // Protected methods
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
	void UndoAvail();
	void SetAngle(double);
	void SetSizeValue(double);
	void SetLineArt(PenStyle, PenCapStyle, PenJoinStyle);
	void SetLocalValues(double, double, double, double);
	void ItemFarben(QString, QString, int, int);
	void ItemGradient(int);
	void ItemTrans(double, double);
	void ItemTextAttr(double);
	void ItemTextUSval(double);
	void ItemTextCols(int, double);
	void SetDistValues(double, double, double, double);
	void ItemTextAbs(int);
	void ItemTextFont(QString);
	void ItemTextSize(int);
	void ItemTextSca(int);
	void ItemTextFarben(QString, QString, int, int);
	void ItemTextStil(int);
	void UpdtObj(uint, uint);
	void ItemRadius(double);
	void HasTextSel();
	void HasNoTextSel();
	void MVals(double, double, double, double, double, double, int);
	void PaintingDone();
	void LoadPic();
	void AppendText();
	void EditText();
	void DoGroup();
	void DoUnGroup();
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
};
#endif
