/***************************************************************************
                          page.h  -  description
                             -------------------
    begin                : Sat Apr 7 2001
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

#ifndef PAGE_H
#define PAGE_H
#include <qobject.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qptrlist.h>
#include <qevent.h>
#include <qscrollview.h>
#include <qpopupmenu.h>
#include "fpoint.h"
#include "scribusdoc.h"
#include "pageitem.h"
/**
  *@author Franz Schmid
  */

class Page : public QWidget  {

Q_OBJECT

public:
	Page(QWidget *pa, int x, int y, int b, int h, ScribusDoc *doc, QScrollView *view);
	~Page() {};
  /** Zeichnet eine Ellipse */
	int PaintEllipse(double x, double y, double b, double h, double w, QString fill, QString outline);
  /** Zeichnet eine Bildbox */
	int PaintPict(double x, double y, double b, double h);
  /** Zeichnet ein Rechteck */
	int PaintRect(double x, double y, double b, double h, double w, QString fill, QString outline);
  /** Zeichnet eine Linie */
	int PaintLine(double x, double y, double b, double h, double w, QString outline);
  /** Zeichnet eine Textbox */
	int PaintText(double x, double y, double b, double h, double w, QString outline);
	/** Zeichnet ein Polygon */
	int PaintPoly(double x, double y, double b, double h, double w, QString fill, QString outline);
	int PaintPolyLine(double x, double y, double b, double h, double w, QString fill, QString outline);
  /** Laedt ein Bild */
	void LoadPict(QString fn, int ItNr);
	void insertColor(QString nam, double c, double m, double y, double k);
	void DrawPageMarks(ScPainter *p, QRect rd);
	void DrawPageItems(ScPainter *painter, QRect rd);
	void leaveEvent(QEvent *);
	void paintEvent(QPaintEvent *e);
	void setGroupRect();
	void getGroupRect(double *x, double *y, double *w, double *h);
	void getGroupRectScreen(double *x, double *y, double *w, double *h);
	void paintGroupRect(bool norm = true);
	void moveGroup(double x, double y, bool fromMP = false);
	FPoint transformPointI(FPoint in, double dx, double dy, double rot, double sx, double sy);
	FPoint transformPoint(FPoint in, double dx, double dy, double rot, double sx, double sy);
	void scaleGroup(double scx, double scy);
	void storeUndoInf(PageItem* b);
	void HandleCurs(QPainter *p, PageItem *b, QRect mpo);
	void HandleSizer(QPainter *p, PageItem *b, QRect mpo);
	bool SeleItem(QMouseEvent *m);
	bool SeleItemPos(QPoint m);
	void Deselect(bool prop = true);
	void SelectItemNr(int nr, bool draw = true);
	void SetupDraw(int Nr);
	void MoveItemI(double newX, double newY, int ite);
	bool MoveItem(double newX, double newY, PageItem* ite, bool fromMP = false);
	void UpdateClip(PageItem* b);
	FPoint GetMinClipF(FPointArray Clip);
	QPoint GetMaxClip(QPointArray Clip);
	QPoint GetMinClip(QPointArray Clip);
	void SetPolyClip(PageItem *b, int up, int down);
	void UpdatePolyClip(PageItem *b);
	void ConvertClip(PageItem *b);
	void SetFrameShape(PageItem *b, int count, double *vals);
	void SetFrameRound(PageItem *b);
	void SetRectFrame(PageItem *b);
	void SetOvalFrame(PageItem *b);
	bool PointOnLine(QPoint Start, QPoint Ende, QRect MArea);
	void MirrorPolyH();
	void MirrorPolyV();
	void TransformPoly(int mode);
	void Reset1Control();
	void ResetControl();
	void AdjustItemSize(PageItem *b);
	bool GetItem(PageItem **b, int nr = -1);
	void MoveRotated(PageItem *b, FPoint npv, bool fromMP = false);
	void AdjustPictScale(PageItem *b, bool reload = true);
	bool SizeItem(double newX, double newY, int ite, bool fromMP = false, bool DoUpdateClip = true);
	bool MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP = false);
	void RotateGroup(double win);
	void RotateItem(double win, int ite);
	void MarkClip(PageItem *b);
	void Transform(PageItem *b, QPainter *p);
	void TransformM(PageItem *b, QPainter *p);
	void PaintSizeRect(QPainter *p, QRect neu);
	QPoint ApplyGrid(QPoint in);
	FPoint ApplyGridF(FPoint in);
	void RefreshItem(PageItem *b, bool single = false);
	void RepaintTextRegion(PageItem *b, QRegion alt, bool single = false);
	void EmitValues(PageItem *b);
	void AdjustPreview(PageItem *b, bool reload = true);
	void FromHRuler(QMouseEvent *m);
	void FromVRuler(QMouseEvent *m);
	void SetYGuide(QMouseEvent *m);
	void SetXGuide(QMouseEvent *m);
	void SnapToGuides(PageItem* b);
	bool ApplyGuides(double *x, double *y);
	void UniteObj();
	void SplitObj();
	void PasteItem(struct CLBuf *Buffer, bool loading, bool drag = false);
	void ItemTextBrush(QString farbe);
	void ItemTextBrushS(int sha);
	void ItemTextPen(QString farbe);
	void ItemTextPenS(int sha);
	void ItemTextScale(int sha);
	void AdvanceSel(PageItem *b, int oldPos, int len, int dir, int expandSel, int state);
	void setNewPos(PageItem *b, int oldPos, int len, int dir);
	void ExpandSel(PageItem *b, int dir, int oldPos);
	void deselectAll(PageItem *b);
	void updateGradientVectors(PageItem *b);
	QRegion ViewReg();
  /** Liste der Elemente */
	QPtrList<PageItem> Items;
  /** Dokument zu dem die Seite gehoert */
	ScribusDoc *doku;
  /** Nummer der Seite */
	uint PageNr;
  /** Name der Seite, wenn Musterseite */
	QString PageNam;
	QString MPageNam;
	QScrollView *Anz;
	QPopupMenu *pmen3;
	Margs Margins;
	int Mxp;
	int Myp;
	bool Mpressed;
	bool mCG;
	bool BlockLeave;
	double oldW;
	QPtrList<PageItem> SelItem;
	QValueList<int> SelNode;
	int HowTo;
	int ClRe;
	int ClRe2;
	int SegP1;
	int SegP2;
	bool CursVis;
	int oldCp;
	int Dxp;
	int Dyp;
	bool Imoved;
	int SeRx;
	int SeRy;
	bool HaveSelRect;
	bool LeftPg;
	bool Magnify;
	bool FirstPoly;
	bool EdPoints;
	bool MoveSym;
	bool GroupSel;
	bool DraggedGroup;
	double GroupX;
	double GroupY;
	double GroupW;
	double GroupH;
	bool HanMove;
	int DrHY;
	int DrVX;
	QValueList<double> XGuides;
	QValueList<double> YGuides;
	int GyM;
	bool MoveGY;
	int GxM;
	bool MoveGX;
	FPoint RCenter;
	bool MidButt;
	int RotMode;
	FPointArray RecordP;

public slots:
	void PasteToPage();
	void TextToPath();
	void ChLineWidth(double w);
	void ChLineArt(PenStyle w);
	void ChLineJoin(PenJoinStyle w);
	void ChLineEnd(PenCapStyle w);
	void ChLocalXY(double x, double y);
	void ChLocalSc(double x, double y);
	void LoremIpsum();
	void ToggleBookmark();
	void ToggleAnnotation();
	void ToggleLock();
	void ToggleResize();
	void TogglePic();
	void UpdatePic();
	void FrameToPic();
	void CallGimp();
	void GimpExited();
	void FlipImageH();
	void FlipImageV();
	void ItemFont(QString fon);
	void chFSize(int size);
	void ItemPen(QString farbe);
	void ItemBrush(QString farbe);
	void ItemPenShade(int sha);
	void ItemBrushShade(int sha);
	void ItemGradFill(int typ);
	void QueryFarben();
	void SetFrameRect();
	void SetFrameRounded();
	void SetFrameOval();
	void ToBack();
	void ToFront();
	void LowerItem();
	void RaiseItem();
	void ClearItem();
	void DeleteItem();
	bool slotSetCurs(int x, int y);
	void slotDoCurs(bool draw);
	void BlinkCurs();
	void ChLineSpa(double w);
	void chKerning(double us);
	void chTyStyle(int s);
	void SetAbStyle(int s);
	void chAbStyle(PageItem *b, int s);
	void AlignObj(bool xa, bool ya, bool Vth, bool Vtv, double xdisp, double ydisp, int xart, int yart);
	void sentToScrap();
	void sentToLayer(int id);
	void ToTextFrame();
	void ToPicFrame();
	void ToPolyFrame();
	void ToBezierFrame();
	void MoveClipPoint(PageItem *b, FPoint np);
	void FromPathText();
	void ToPathText();
	void Bezier2Poly();

signals: // Signals
  /** Mausposition X */
	void Hrule(int);
  /** Mausposition Y */
	void Vrule(int);
  /** Seite gewechselt */
	void PgCh(int);
  /** Element gezeichnet */
	void PaintingDone();
  /** Element ausgewaehlt */
	void HaveSel(int);
  /** Seite veraendert */
	void DocChanged();
  /** Position eines Elementes */
	void ItemPos(double, double);
  /** Breite und Hoehe eines Elementes */
	void ItemGeom(double, double);
	void ItemRadius(double);
  /** Setzt den Drehungswinkel des Elementes */
	void SetAngle(double);
  /** Zoomfunktionen */
	void ZoomIn(int, int);
	void ZoomOut(int, int);
	void ZoomAbs();
	void AbsPosi(int, int);
	void AbsPosi2(int, int);
  /** Sendet die Linienstaerke an die Masspalette */
	void SetSizeValue(double);
  /** Sendet die Linienart an die Masspalette */
	void SetLineArt(PenStyle, PenCapStyle, PenJoinStyle);
  /** Sendet die lokalen Bildeigenschaften an die Masspalette */
	void SetLocalValues(double, double, double, double);
	void SetDistValues(double, double, double, double);
  /** Sendet die Stiftfarbe an die Farbpalette */
	void ItemFarben(QString, QString, int, int);
	void ItemGradient(int);
  /** Sendet die Textattribute an die Masspalette */
	void ItemTextAttr(double);
  /** Sendet die Schriftart */
	void ItemTextFont(QString);
  /** Sendet die Schriftgroesse */
	void ItemTextSize(int);
	void ItemTextSca(int);
	void ItemTextUSval(double);
	void ItemTextAbs(int);
	void ItemTextCols(int, double);
	void ItemTextFarben(QString, QString, int, int);
  /** Text in Textbox selektiert */
	void HasTextSel();
	void HasNoTextSel();
	void CutItem();
	void CopyItem();
	void ItemTextStil(int);
	void LoadPic();
	void AppendText();
	void EditText();
	void LoadElem(QString, int, int, bool, bool, ScribusDoc *);
	void Amode(int);
	void AnnotProps();
	void ToScrap(QString);
	void MousePos(double, double);
	void ClipPo(double, double);
	void HavePoint(bool, bool);
	void PolyOpen();
	void PStatus(int, uint);
	void UndoAvail();
	void EditGuides();
	void ItemTrans(double, double);
	void AddBM(PageItem *);
	void DelBM(PageItem *);
	void NewBMNr(int, int);
	void ChBMText(PageItem *);
	void RasterPic(bool);
	void DelObj(uint, uint);
	void MoveObj(uint, uint, uint);
	void UpdtObj(uint, uint);
	void AddObj(uint, uint);
	void DoGroup();
	void DoUnGroup();
	void LevelChanged(uint);
	void MVals(double, double, double, double, double, double, int);
	void EndNodeEdit();
	
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void dragLeaveEvent(QDragLeaveEvent *e);
	virtual void dragEnterEvent(QDragEnterEvent *e);
	virtual void dragMoveEvent(QDragMoveEvent *e);
	virtual void dropEvent(QDropEvent *e);
};

#endif
