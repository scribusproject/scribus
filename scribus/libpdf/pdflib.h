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

#include "qvaluelist.h"
#include <qfile.h>

class QString;
class QRect;
class QImage;
class PageItem;
class ScribusView;
class BookMItem;
class BookMView;
#include "scribus.h"

/**
  *@author Franz Schmid
  */

class PDFlib : public QObject
{
	Q_OBJECT

public:
	PDFlib();
	~PDFlib() {};
	bool PDF_Begin_Doc(QString fn, ScribusDoc *docu, ScribusView *vie, PDFOptions *opts, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, BookMView* vi);
	void PDF_Begin_Page(Page* pag, QPixmap pm = 0);
	void PDF_End_Page();
	void PDF_TemplatePage(Page* pag, bool clip = false);
	void PDF_ProcessPage(Page* pag, uint PNr, bool clip = false);
	void PDF_End_Doc(QString PrintPr = "", QString Name = "", int Components = 0);
	struct GlNamInd
	{
		uint Code;
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
	QString FToStr(double c);
	QString IToStr(int c);
	QString setStrokeMulti(struct SingleLine *sl);
	QString SetClipPathArray(FPointArray *ite, bool poly = true);
	QString SetClipPath(PageItem *ite, bool poly = true);
	QString SetFarbe(QString farbe, int Shade);
	QString putColor(QString color, int Shade, bool fill);
	QString setTextSt(PageItem *ite, uint PNr);
	void PutDoc(QString in);
	void PutPage(QString in);
	void StartObj(int nr);
	void WritePDFStream(QString *cc);
	QString PDFEncode(QString in);
	QByteArray ComputeMD5(QString in);
	void PDF_Bookmark(int nr, double ypos);
	void PDF_Gradient(PageItem *currItem);
	void PDF_DoLinGradient(PageItem *currItem, QValueList<double> Stops, QValueList<double> Trans, QStringList Colors);
	void PDF_Transparenz(PageItem *currItem);
	void PDF_Annotation(PageItem *ite, uint PNr);
	void PDF_Form(QString im);
	void PDF_xForm(double w, double h, QString im);
	void PDF_Image(PageItem* c, bool inver, QString fn, double sx, double sy, double x, double y, bool fromAN = false, QString Profil = "", bool Embedded = false, int Intent = 1);
	QString Inhalt;
	ScribusDoc* doc;
	ScribusView* view;
	Page* ActPageP;
	PDFOptions* Options;
	BookMView* Bvie;
	QFile Spool;
	int Dokument;
	struct Dest
	{
		QString Name;
		int Seite;
		QString Act;
	};
	struct Cata
	{
		int Outlines;
		int PageTree;
		int Dest;
	}
	Catalog;
	struct PagT
	{
		QValueList<int> Kids;
		int Count;
	}
	PageTree;
	struct PagL
	{
		int ObjNum;
		int Thumb;
		QMap<QString,int> XObjects;
		QMap<QString,int> FObjects;
		QValueList<int> AObjects;
		QValueList<int> FormObjects;
	}
	Seite;
	struct OutL
	{
		int First;
		int Last;
		int Count;
	}
	Outlines;
	struct Bead
	{
		int Parent;
		int Next;
		int Prev;
		int Page;
		QRect Recht;
	};
	struct ICCD
	{
		int ResNum;
		QString ResName;
		QString ICCArray;
	};
	struct ShIm
	{
		int ResNum;
		int Width;
		int Height;
		double aufl;
		double sxa;
		double sya;
		double xa;
		double ya;
	};
	struct OCGInfo
	{
		int ObjNum;
		bool visible;
		QString Name;
	};
	QMap<QString,ShIm> SharedImages;
	QValueList<uint> XRef;
	QValueList<Dest> NamedDest;
	QValueList<int> Threads;
	QValueList<Bead> Beads;
	QValueList<int> CalcFields;
	QMap<QString,int> Shadings;
	QMap<QString,int> Transpar;
	QMap<QString,ICCD> ICCProfiles;
	QMap<QString, OCGInfo> OCGEntries;
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
	QString HTName;
	bool BookMinUse;
};

#endif

