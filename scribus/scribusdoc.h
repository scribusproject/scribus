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
#include <qpixmap.h>
#include <qptrlist.h>
#include <qfont.h>
#include <qmap.h>
#include <qpointarray.h>
#include <qstringlist.h>
#include <qtimer.h>

#include "scribusstructs.h"
#include "undoobject.h"
#include "hyphenator.h"
#ifdef HAVE_CMS
	#include CMS_INC
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
class Page;
class UndoManager;
class UndoState;


/**
  * the Document Class
  */

class ScribusDoc : public QObject, public UndoObject
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
	void OpenCMSProfiles(ProfilesL InPo, ProfilesL MoPo, ProfilesL PrPo);
	void CloseCMSProfiles();
	void loadStylesFromFile(QString fileName, QValueList<ParagraphStyle> *tempStyles = NULL);
	/** 
	 * @brief Should guides be locked or not
	 * @param isLocked If true guides on pages cannot be moved if false they
	 * can be dragged to new positions.
	 * @author Riku Leino
	 */
	void lockGuides(bool isLocked);
	/**
	 * @brief Method used when an undo/redo is requested.
	 * @param state State describing the action that is wanted to be undone/redone
	 * @param isUndo If true undo is wanted else if false redo.
	 * @author Riku Leino
	 */
	void restore(UndoState* state, bool isUndo);
	/**
	 * @brief Sets the name of the document
	 * @param name Name for the document
	 * @author Riku Leino
	 */
	void setName(const QString& name);

protected:
    bool modified;
    UndoManager *undoManager;

public: // Public attributes
	int NrItems;
	int First;
	int Last;
	int viewCount;
	int viewID;
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
	QString DocName;
	QMap<QString,QFont> UsedFonts;
	SCFonts *AllFonts;
	ApplicationPrefs *prefsValues;
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
	toolPrefs toolSettings;
	QMap<QString, checkerPrefs> checkerProfiles;
	QString curCheckProfile;
  /** Letztes Element fuer AutoTextrahmen */
	PageItem *LastAuto;
  /** Erstes Element fuer AutoTextrahmen */
	PageItem *FirstAuto;
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
	cmsHTRANSFORM stdTrans;
	cmsHTRANSFORM stdProof;
	cmsHTRANSFORM stdTransImg;
	cmsHTRANSFORM stdProofImg;
	cmsHTRANSFORM stdTransCMYK;
	cmsHTRANSFORM stdProofCMYK;
	cmsHTRANSFORM stdTransRGB;
	bool SoftProofing;
	bool Gamut;
	int IntentMonitor;
	int IntentPrinter;
#endif
	bool HasCMS;
	QMap<QString,QString> JavaScripts;
	int TotalItems;
	Hyphenator *docHyphenator;
	int MinWordLen;
	int HyCount;
	QString Language;
	bool Automatic;
	bool AutoCheck;
	bool TemplateMode;
	PDFOptions PDF_Options;
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
	QMap<int, errorCodes> docItemErrors;
	QMap<int, errorCodes> masterItemErrors;
	//Attributes to be applied to frames
	ObjAttrVector docItemAttributes;
};

#endif
