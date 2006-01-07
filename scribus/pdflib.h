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
#include <string>
#include <vector>

class QString;
class QRect;
class QImage;
class PageItem;
class BookMItem;
class BookMView;
class ScribusDoc;
class Page;
class PDFOptions;
class PrefsContext;
class MultiProgressDialog;

#include "scribusstructs.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API PDFlib : public QObject
{
	Q_OBJECT

public:
	explicit PDFlib(ScribusDoc & docu);
	~PDFlib();
	bool doExport(const QString& fn, const QString& nam, int Components,
				  const std::vector<int> & pageNs, const QMap<int,QPixmap> & thumbs);

	// used by ScFonts
	struct GlNamInd
	{
		uint Code;
		QString Name;
	};

private:
	typedef QMap<uint, GlNamInd> GListeInd;

	bool PDF_Begin_Doc(const QString& fn, SCFonts &AllFonts, QMap<QString,int> DocFonts, BookMView* vi);
	void PDF_Begin_Page(const Page* pag, QPixmap pm = 0);
	void PDF_End_Page();
	void PDF_TemplatePage(const Page* pag, bool clip = false);
	void PDF_ProcessPage(const Page* pag, uint PNr, bool clip = false);
	void PDF_End_Doc(const QString& PrintPr = "", const QString& Name = "", int Components = 0);
	void closeAndCleanup();

	QString EncStream(const QString & in, int ObjNum);
	QString EncString(const QString & in, int ObjNum);
	void CalcOwnerKey(const QString & Owner, const QString & User);
	void CalcUserKey(const QString & User, int Permission);
	QString FitKey(const QString & pass);
	QString setStrokeMulti(struct SingleLine *sl);
	QString SetClipPathArray(FPointArray *ite, bool poly = true);
	QString SetClipPathImage(PageItem *ite);
	QString SetClipPath(PageItem *ite, bool poly = true);
	QString SetFarbe(const QString& farbe, int Shade);
	QString putColor(const QString& color, int Shade, bool fill);
	QString putColorUncached(const QString& color, int Shade, bool fill);
	QString PDF_ProcessItem(PageItem* ite, const Page* pag, uint PNr, bool embedded = false);
	QString setTextSt(PageItem *ite, uint PNr, const Page* pag);
	void setTextCh(PageItem *ite, uint PNr, uint d,  QString &tmp, QString &tmp2, const ScText * hl, const Page* pag);

	// Provide a couple of PutDoc implementations to ease transition away from
	// QString abuse and to provide fast paths for constant strings.
	void PutDoc(const QString & in) { outStream.writeRawBytes(in.latin1(), in.length()); }
	void PutDoc(const QByteArray & in) { outStream.writeRawBytes(in, in.size()); }
	void PutDoc(const char* in) { outStream.writeRawBytes(in, strlen(in)); }
	void PutDoc(const std::string & in) { outStream.writeRawBytes(in.c_str(), in.length()); }

	void PutPage(const QString & in) { Inhalt += in; }
	void StartObj(int nr);
	void WritePDFStream(const QString& cc);
	QString PDFEncode(const QString & in);
	QByteArray ComputeMD5(const QString& in);
	void PDF_Bookmark(int nr, double ypos);
	QString PDF_Gradient(PageItem *currItem);
	QString PDF_DoLinGradient(PageItem *currItem, QValueList<double> Stops, QValueList<double> Trans, const QStringList& Colors);
	QString PDF_Transparenz(PageItem *currItem);
	void PDF_Annotation(PageItem *ite, uint PNr);
	void PDF_Form(const QString& im);
	void PDF_xForm(double w, double h, QString im);
	QString PDF_Image(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN = false, const QString& Profil = "", bool Embedded = false, int Intent = 1);

	int bytesWritten() { return Spool.at(); }

	QMap<QString, GListeInd> GlyphsIdxOfFont;
	QString Inhalt;
	ScribusDoc & doc;
	const Page * ActPageP;
	const PDFOptions & Options;
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
		QMap<QString,int> ImgObjects;
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
	struct SpotC
	{
		int ResNum;
		QString ResName;
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
	QMap<QString, int> RealFonts;
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
	ColorList colorsToUse;
	QMap<QString, SpotC> spotMap;
	QString spotNam;
	int spotCount;
	QTextStream outStream;
	QMap<QString, QString> StdFonts;
	MultiProgressDialog* progressDialog;
	bool abortExport;
	bool usingGUI;
	
protected slots:
	void cancelRequested();
};

#endif

