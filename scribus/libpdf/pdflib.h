/***************************************************************************
                          pdflib.h  -  description
                             -------------------
    begin                : Sat Jan 19 2002
    copyright            : (C) 2002 by Franz Schmid
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

#ifndef PDFLIB_H
#define PDFLIB_H
#include <qstring.h>
#include <qrect.h>
#include <qvaluelist.h>
#include <qimage.h>
#include "pageitem.h"
#include "scribusview.h"
#include "bookmwin.h"
#include <qfile.h>

/**
  *@author Franz Schmid
  */

class PDFlib {

public: 
	PDFlib();
	virtual ~PDFlib();
	virtual bool PDF_Begin_Doc(QString fn, ScribusDoc *docu, ScribusView *vie, PDFOpt *opts, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, BookMView* vi);
	virtual void PDF_Begin_Page(Page* pag, QPixmap pm);
	virtual void PDF_End_Page();
	virtual void PDF_TemplatePage(Page* pag);
	virtual void PDF_ProcessPage(Page* pag, uint PNr);
	virtual void PDF_End_Doc(QString PrintPr = "", QString Name = "", int Components = 0);
	struct GlNamInd { uint Code;
										QString Name;
									};
	typedef QMap<uint, GlNamInd> GListeInd;
	QMap<QString, GListeInd> GlyphsIdxOfFont;
	
private:
	QString EncStream(QString *in, int ObjNum);
	QString EncString(QString in, int ObjNum);
	void CalcOwnerKey(QString Owner, QString User);
	void CalcUserKey(QString User, int Permission);
	QString FitKey(QString pass);
	QString FToStr(float c);
	QString IToStr(int c);
	QString SetClipPath(PageItem *ite);
	QString SetFarbe(QString farbe, int Shade);
	QString setTextSt(PageItem *ite, uint PNr);
	void PutDoc(QString in);
	void PutPage(QString in);
	void StartObj(int nr);
	void WritePDFStream(QString *cc);
	QString PDFEncode(QString in);
	QByteArray ComputeMD5(QString in);
	void PDF_Bookmark(int nr, float ypos);
	void PDF_Gradient(PageItem *b);
	void PDF_Transparenz(PageItem *b);
	void PDF_Annotation(PageItem *ite, uint PNr);
	void PDF_Form(QString im);
  void PDF_xForm(float w, float h, QString im);
	void PDF_Image(QString fn, float sx, float sy, float x, float y, bool fromAN = false, QString Profil = "", bool Embedded = false, int Intent = 1);
	QString Inhalt;
  ScribusDoc* doc;
  ScribusView* view;
  Page* ActPage;
  PDFOpt* Options;
  BookMView* Bvie;
	QFile Spool;
	int Dokument;
	struct Dest {	QString Name;
								int Seite;
								QString Act;
							};
	struct Cata { int Outlines;
								int PageTree;
								int Dest;
							} Catalog;
	struct PagT { QValueList<int> Kids;
								int Count;
							} PageTree;
	struct PagL { int ObjNum;
								int Thumb;
								QMap<QString,int> XObjects;
								QMap<QString,int> FObjects;
								QValueList<int> AObjects;
								QValueList<int> FormObjects;
							} Seite;
	struct OutL { int First;
								int Last;
								int Count;
							} Outlines;
	struct Bead { int Parent;
								int Next;
								int Prev;
								int Page;
								QRect Recht;
							};
	struct ICCD { int ResNum;
								QString ResName;
								QString ICCArray;
							};
	QValueList<uint> XRef;
	QValueList<Dest> NamedDest;
	QValueList<int> Threads;
	QValueList<Bead> Beads;
	QValueList<int> CalcFields;
	QMap<QString,int> Shadings;
	QMap<QString,int> Transpar;
	QMap<QString,ICCD> ICCProfiles;
	int ObjCounter;
	QString ResNam;
	int ResCount;
	QString NDnam;
	QString Datum;
	int NDnum;
	QMap<QString, QString> UsedFontsP;
	QMap<QString, QFont> RealFonts;
	bool CompAvail;
	QByteArray KeyGen;
	QByteArray OwnerKey;
	QByteArray UserKey;
	QByteArray FileID;
	QByteArray EncryKey;
	int Encrypt;
	int KeyLen;
};

#endif
