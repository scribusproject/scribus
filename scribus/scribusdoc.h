/***************************************************************************
                          scribusdoc.h  -  description
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
#ifndef SCRIBUSDOC_H
#define SCRIBUSDOC_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// include files for QT
#include <qobject.h>
#include <qdict.h>
#include <qcolor.h>
#include <qvaluelist.h>
#include <qvaluevector.h>
#include <qptrlist.h>
#include <qfont.h>
#include <qmap.h>
#include <qpointarray.h>
#include <qstringlist.h>
#include <qtimer.h>

#include "scribusstructs.h"
#include "hyphenator.h"
#ifdef HAVE_CMS
	#include CMS_INC
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
class Page;


/**
  * the Document Class
  */

class ScribusDoc : public QObject
{

public:
    ScribusDoc(struct ApplicationPrefs *prefsData);
    ~ScribusDoc();
    void setModified();
    void setUnModified();
    bool isModified() const;
  /** Setzt die Seitenattribute */
	void setPage(double b, double h, double t, double l, double r, double bo, double sp, double ab, bool atf, bool fp);
	void resetPage(double t, double l, double r, double bo, bool fp);
	bool AddFont(QString name, QFont fo);
	void loadStylesFromFile(QString fileName, QValueList<ParagraphStyle> *tempStyles = NULL);


protected:
    bool modified;

public: // Public attributes
	bool SnapGuides;
	bool GuideLock;
	bool loading;
	/** Scratch space around Pages */
	double ScratchLeft;
	double ScratchRight;
	double ScratchTop;
	double ScratchBottom;
  /** List of Pages */
	QPtrList<Page> Pages;
  /** List of Template Pages */
	QPtrList<Page> MasterPages;
  /** List of Document Pages */
	QPtrList<Page> DocPages;
  /** Mapping Template Name to Template Page numbers */
	QMap<QString,int> MasterNames;
  /** List of Objects */
	QPtrList<PageItem> Items;
	QPtrList<PageItem> MasterItems;
	QPtrList<PageItem> DocItems;
  /** Pagewidth  */
	double PageB;
  /** Pageheight */
	double PageH;
  /** Number of Pages */
	int PageC;
  /** Margins */
	MarginStruct PageM;
  /** Number of Columns */
	double PageSp;
  /** Distance of Columns */
	double PageSpa;
  /** Flag for automatic Textframes */
	bool PageAT;
  /** Flag for facing Pages */
	bool PageFP;
	/** Flag fuer Hoch- oder Querformat 0 = Hochformat */
	int PageOri;	
	QString PageSize;
	/** Erste Seitennummer im Dokument */
	int FirstPnum;
  /** Flag fuer Rasterbenutzung */
	bool useRaster;
  /** Im Dokument benutzte Farben */
	ColorList PageColors;
	Page* currentPage;
  /** InfoStrings fuer das aktuelle Dokument */
	QString DocAutor;
	QString DocTitel;
	QString DocComments;
	QString DocKeyWords;
	QString DocPublisher;
	QString DocType;
	QString DocDate;
	QString DocFormat;
	QString DocIdent;
	QString DocSource;
	QString DocLangInfo;
	QString DocRelation;
	QString DocCover;
	QString DocRights;
	QString DocContrib;
	int appMode;
	int SubMode;
	double *ShapeValues;
	int ValCount;
	QString Dpen;
	QString Dbrush;
	double Dwidth;
	int Dshade;
	int Dshade2;
	PenStyle DLineArt;
	QString DpenLine;
	QString DpenText;
	QString DstrokeText;
	int DstartArrow;
	int DendArrow;
	int DCols;
	double DGap;
	int DshadeLine;
	PenStyle DLstyleLine;
	double DwidthLine;
	QString Dfont;
	QString DocName;
	QMap<QString,QFont> UsedFonts;
	SCFonts *AllFonts;
	ApplicationPrefs *prefsValues;
	int Dsize;
	QColor papColor;
	int CurrentSel;
	int CurrentStyle;
	int currentParaStyle;
	QString CurrFont;
	int CurrFontSize;
	QString CurrTextFill;
	int CurrTextFillSh;
	QString CurrTextStroke;
	int CurrTextStrokeSh;
	int CurrTextScale;
	bool EditClip;
	int EditClipMode;
	typoPrefs typographicSetttings;
	guidesPrefs guidesSettings;
  /** Letztes Element fuer AutoTextrahmen */
	PageItem *LastAuto;
  /** Erstes Element fuer AutoTextrahmen */
	PageItem *FirstAuto;
	int MagMin;
	int MagMax;
	int MagStep;
	QString DbrushPict;
	int ShadePict;
	double ScaleX;
	double ScaleY;
	bool ScaleType;
	bool AspectRatio;
	bool Before;
	int docUnitIndex;
	bool DragP;
	bool leaveDrag;
	PageItem *DraggedElem;
	PageItem *ElemToLink;
	QValueList<uint> DragElements;
	QValueList<ParagraphStyle> docParagraphStyles;
	QValueList<Layer> Layers;
	bool MasterP;
	bool FirstPageLeft;
	bool marginColored;
	int GroupCounter;
	CMSData CMSSettings;
	int ActiveLayer;
#ifdef HAVE_CMS
	cmsHPROFILE DocInputProf;
	cmsHPROFILE DocOutputProf;
	cmsHPROFILE DocPrinterProf;
#endif
	bool HasCMS;
	QMap<QString,QString> JavaScripts;
	bool UnDoValid;
	UndoData UnData;
	int TotalItems;
	Hyphenator *docHyphenator;
	int MinWordLen;
	int HyCount;
	QString Language;
	bool Automatic;
	bool AutoCheck;
	bool TemplateMode;
	struct PDFOptions PDF_Optionen;
	bool RePos;
	struct BookMa { 
					QString Title;
					QString Text;
					QString Aktion;
					int Parent;
					int Seite;
					int Element;
					int ItemNr;
					int First;
					int Last;
					int Prev;
					int Next;
					};
	QValueList<BookMa> BookMarks;
	bool OldBM;
	bool hasName;
	int RotMode;
	bool AutoSave;
	int AutoSaveTime;
	QTimer *ASaveTimer;
	FT_Library   library;
	QMap<QString,FT_Face> FFonts;
	QMap<QString,multiLine> MLineStyles;
	QValueList<ArrowDesc> arrowStyles;
	QWidget* WinHan;
	bool DoDrawing;
	QValueList<int> OpenNodes;
	QTimer *CurTimer;
	int PolyC;
	int PolyFd;
	double PolyF;
	bool PolyS;
	double PolyR;
};

#endif
