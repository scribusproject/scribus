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
#include <qvaluestack.h>
#include <qvaluelist.h>
#include "scribusdoc.h"
#include "scpainter.h"
/**
  *@author Franz Schmid
  */
							
class PageItem : public QObject
{
	Q_OBJECT

public: 
	PageItem(Page* pa, int art, double x, double y, double w, double h, double w2, QString fill, 
				QString outline, ScribusDoc *doc);
	~PageItem() {};
	struct ZZ { 
				double xco;
				double yco;
				double wide;
				double kern;
				int Siz;
				int Style;
				int scale;
				int shade;
				int shade2;
				bool Sele;
				QString Zeich;
				QString Farb;
				QString Farb2;
				QString ZFo;
			  };
  /** Zeichnet das Item */
	void paintObj(QRect e=QRect(), QPixmap *ppX = 0);
	void DrawObj(ScPainter *p, QRect e);
	double SetZeichAttr(struct Pti *hl, int *chs, QString *chx);
	void SetFarbe(QColor *tmp, QString farbe, int shad);
	void DrawZeichenS(ScPainter *p, struct ZZ *hl);
	void DrawPolyL(QPainter *p, QPointArray pts);
	QString ExpandToken(uint base);
	bool Locked;
	bool LockRes;
	bool Reverse;
  /** X-Position auf der Seite */
	double Xpos;
  /** Y-Position auf der Seite */
	double Ypos;
  /** Breite des Elements */
	double Width;
  /** Hoehe des Elements */
	double Height;
  /** Eckrundung von Rechtecken */
	double RadRect;
  /** Art des Items */
	int PType;
  /** Winkel um den das Item gedreht wird */
	double Rot;
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
  /** Abstufung fuer Fllfarbe */
	int Shade;
  /** Zeichenfarbe fuer Elemente */
	QString Pcolor2;
  /** Abstufung fuer Zeichenfarbe */
	int Shade2;
	QString TxtStroke;
	QString TxtFill;
	int ShTxtStroke;
	int ShTxtFill;
	int TxtScale;
	int TxTStyle;
	int Cols;
	double ColGap;
  /** Strichstaerke */
	double Pwidth;
	double OldPwidth;
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
	double BaseOffs;
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
	QImage pixmOrg;
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
	double LocalScX;
	double LocalViewX;
  /** Lokales Scaling y*/
	double LocalScY;
	double LocalViewY;
  /** Lokales X */
	double LocalX;
  /** Lokales Y */
	double LocalY;
	int OrigW;
	int OrigH;
	double dpiX;
	double dpiY;
  /** Anzahl horizontaler Spiegelungen */
	int flippedH;
  /** Anzahl vertikaler Spiegelungen */
	int flippedV;
  /** BoundigBox-X */
	double BBoxX;
  /** BoundingBox-H */
	double BBoxH;
  /** Extra Abstand vom Rand */
	double Extra;
	double TExtra;
	double BExtra;
	double RExtra;
  /** Linespacing */
	double LineSp;
  /** Zeichen X-Position */
	double CurX;
  /** Zeichen Y-Position */
	double CurY;
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
	bool AutoName;
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
	double ExtraV;
	bool isRaster;
	double OldB;
	double OldH;
	double OldB2;
	double OldH2;
	bool Sizing;
	bool toPixmap;
	int LayerNr;
	bool ScaleType;
	bool AspectRatio;
	double Transparency;
	double TranspStroke;
	QValueStack<int> Groups;
	bool InvPict;
	QValueList<double> DashValues;
	QValueList<double> TabValues;
	double DashOffset;
	VGradient fill_gradient;
	QString Language;
/* Additions for Table Support */
	PageItem* LeftLink;
	PageItem* RightLink;
	PageItem* TopLink;
	PageItem* BottomLink;
	int LeftLinkID;
	int RightLinkID;
	int TopLinkID;
	int BottomLinkID;
	bool LeftLine;
	bool RightLine;
	bool TopLine;
	bool BottomLine;
	bool isTableItem;
	bool isSingleSel;
};

#endif
