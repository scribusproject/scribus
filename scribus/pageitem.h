/***************************************************************************
                          pageitem.h  -  description
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

#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <qobject.h>
#include <qwidget.h>
#include <qpointarray.h>
#include <qptrlist.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfont.h>
#include <qvaluestack.h>
#include <qvaluelist.h>
#include "scribusdoc.h"
/**
  *@author Franz Schmid
  */
							
class PageItem : public QObject  {

public: 
	PageItem(Page* pa, int art, float x, float y, float w, float h, float w2, QString fill, QString outline, ScribusDoc *doc);
	~PageItem() {};
	struct ZZ { QString Zeich;
					 QColor Farb;
					 QFont Zsatz;
					 QString ZFo;
					 float xco;
					 float yco;
					 bool Sele;
					 int Siz;
					 float wide;
				  };
  /** Zeichnet das Item */
  void paintObj(QRect e=QRect(), QPixmap *ppX = 0);
  void CopyIt(struct CLBuf *Buffer);
	void SetZeichAttr(QPainter *p, QFont *ffo, struct Pti *hl, int *chs, QString *chx);
	void SetFarbe(QColor *tmp, QString farbe, int shad);
	void DrawZeichen(QPainter *p, struct Pti *hl);
	void DrawZeichen(QPainter *p, struct ZZ *hl);
	void DrawPoly(QPainter *p, QPointArray pts, QColor BackF, bool bitm = false);
	void DrawPolyL(QPainter *p, QPointArray pts);
	QString ExpandToken(uint base);
	bool Locked;
	bool Reverse;
  /** X-Position auf der Seite */
  float Xpos;
  /** Y-Position auf der Seite */
  float Ypos;
  /** Breite des Elements */
  float Width;
  /** Hoehe des Elements */
  float Height;
  /** Eckrundung von Rechtecken */
  float RadRect;
  /** Art des Items */
  int PType;
  /** Winkel um den das Item gedreht wird */
  float Rot;
  /** Enthaelt das uebergeordnete Objekt */
  QWidget *Parent;
  /** Enthaelt das Dokument */
  ScribusDoc *Doc;
	QString GrColor;
	QString GrColor2;
	int GrShade;
	int GrShade2;
	int GrType;
  /** Fuellfarbe */
  QString Pcolor;
  /** Abstufung fuer Füllfarbe */
  int Shade;
  /** Zeichenfarbe fuer Elemente */
  QString Pcolor2;
  /** Abstufung fuer Zeichenfarbe */
  int Shade2;
  /** Strichstaerke */
  float Pwidth;
  /** Linienart */
  PenStyle PLineArt;
  PenCapStyle PLineEnd;
  PenJoinStyle PLineJoin;
	QString NamedLStyle;
  /** Element selektiert Ja/Nein */
  bool Select;
  /** Definiert die Clipping-Region des Elements; */
  QPointArray Clip;
	FPointArray PoLine;
	QValueList<uint> Segments;
	bool PoShow;
	float BaseOffs;
  bool ClipEdited;
  int FrameType;
  /** Interne Item-Nummer */
  uint ItemNr;
  /** Hat Element Rahmen? */
  bool Frame;
  /** Seite zu der das Element gehoert */
  Page *OwnPage;
  /** Darzustellendes Bild */
  QImage pixm;
  QImage Gpixm;
  /** Dateiname des Bildes */
  QString Pfile;
  QString Pfile2;
  QString Pfile3;
  QString IProfile;
  bool UseEmbedded;
  QString EmProfile;
  int IRender;
  /** Darstellungsart Bild/Titel */
  bool PicArt;
  /** Bild verfuegbar */
  bool PicAvail;
  /** Lokales Scaling x*/
  float LocalScX;
	float LocalViewX;
  /** Lokales Scaling y*/
  float LocalScY;
	float LocalViewY;
  /** Lokales X */
  float LocalX;
  /** Lokales Y */
  float LocalY;
	int OrigW;
	int OrigH;
  /** Anzahl horizontaler Spiegelungen */
  int flippedH;
  /** Anzahl vertikaler Spiegelungen */
  int flippedV;
  /** BoundigBox-X */
  float BBoxX;
  /** BoundingBox-H */
  float BBoxH;
  /** Extra Abstand vom Rand */
  float Extra;
  float TExtra;
  float BExtra;
  float RExtra;
  /** Linespacing */
  float LineSp;
  /** Zeichen X-Position */
  float CurX;
  /** Zeichen Y-Position */
  float CurY;
  /** Cursorposition */
  int CPos;
  /** Text des Elements */
  QPtrList<Pti> Ptext;
  /** Flag ob Element gedruckt wird */
  bool isPrintable;
  /** Flag fuer PDF-Bookmark */
  bool isBookmark;
  int BMnr;
  /** Flag fuer PDF-Annotation */
  bool isAnnotation;
  int AnType;
	int AnActType;
  QString AnAction;
	QString An_E_act;
	QString An_X_act;
	QString An_D_act;
	QString An_Fo_act;
	QString An_Bl_act;
	QString An_K_act;
	QString An_F_act;
	QString An_V_act;
	QString An_C_act;
	QString AnName;
	QString AnToolTip;
	QString AnRollOver;
	QString AnDown;
	QString AnBColor;
	QString An_Extern;
	int AnBsty;
	int AnBwid;
	int AnFeed;
  int AnZiel;
	int AnFlag;
	int AnMaxChar;
	int AnVis;
	int AnChkStil;
	int AnFont;
	bool AnIsChk;
	bool AnAAact;
	bool AnHTML;
	bool AnUseIcons;
	int AnIPlace;
	int AnScaleW;
	int AnFormat;
  /** Flag fuer neuzeichnen im EditMode */
  bool Dirty;
  /** Item Font */
  QString IFont;
  /** Item Fontgroesse */
  int ISize;
  /** Flag fuer Auswahl */
  bool HasSel;
  /** Flag fuer Textfluss */
  bool Textflow;
  bool Textflow2;
  bool FrameOnly;
  PageItem *BackBox;
  PageItem *NextBox;
  int NextIt;
  int NextPg;
  bool Tinput;
  bool isAutoText;
  int Ausrich;
  uint MaxChars;
  bool Redrawn;
  float ExtraV;
  bool isRaster;
  float OldB;
  float OldH;
  float OldB2;
  float OldH2;
  bool Sizing;
  bool toPixmap;
  float DevRes;
  int DevResX;
  int DevResY;
	int LayerNr;
	bool ScaleType;
	bool AspectRatio;
	float Transparency;
  QValueStack<int> Groups;
	bool InvPict;
};

#endif
