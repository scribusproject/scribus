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
#include "scconfig.h"
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

#include "scribusapi.h"
#include "scribusstructs.h"
#include "prefsstructs.h"
#include "documentinformation.h"
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
class PDFOptions;


/**
  * the Document Class
  */

class SCRIBUS_API ScribusDoc : public QObject, public UndoObject
{
	Q_OBJECT

public:
	ScribusDoc();
	~ScribusDoc();
	void setup(const int, const int, const int, const int, const int, const QString&, const QString&);
	void setLoading(const bool);
	const bool isLoading();
	void setModified(const bool);
	bool isModified() const;
/** Setzt die Seitenattribute */
	void setPage(double b, double h, double t, double l, double r, double bo, double sp, double ab, bool atf, int fp);
	void resetPage(double t, double l, double r, double bo, int fp);
	// Add, delete and move pages
	Page* addPage(const int);
	bool deletePage(const int);
	void movePage(const int, const int, const int, const int);
	// Add, delete and move layers
	/** 
	 * @brief Add a layer to the current document
	 * @param Name of layer
	 * @param Mark the layer active
	 * @return Number of the layer created
	 */
	const int addLayer(const QString&, const bool);
	/** 
	 * @brief Delete a layer from the current document
	 * @param Number of layer
	 * @param Delete the items on the layer too?
	 * @return Success or failure
	 */
	const bool deleteLayer(const int, const bool);
	/**
	 * @brief Return the number of the current layer
	 * @return Active layer number
	 */
	const int activeLayer();
	
	/**
	 * @brief Return the name of the current layer
	 * @return Name of the layer
	 */
	const QString& activeLayerName();
	/**
	 * @brief Set the active layer via the layer number
	 * @param Number of the layer
	 * @return Success or failure
	 */
	const bool setActiveLayer(const int);
	/**
	 * @brief Set the active layer via the layer name
	 * @param Name of the layer
	 * @return Success or failure
	 */
	const bool setActiveLayer(const QString &);
	/**
	 * @brief Set the layer printable via the layer number
	 * @param Number of the layer
	 * @param Printable bool
	 * @return Success or failure
	 */
	const bool setLayerPrintable(const int, const bool);
	/**
	 * @brief Is the layer printable
	 * @param Number of the layer
	 * @return Printable or not
	 */
	const bool layerPrintable(const int);
	/**
	 * @brief Set the layer visible via the layer number
	 * @param Number of the layer
	 * @param Visible bool
	 * @return Success or failure
	 */
	const bool setLayerVisible(const int, const bool);
	/**
	 * @brief Is the layer visible
	 * @param Number of the layer
	 * @return Visible or not
	 */
	const bool layerVisible(const int);
	/**
	 * @brief Return the level of the requested layer
	 * @param Number of the layer
	 * @return Level of the layer
	 */
	const int layerLevelFromNumber(const int);
	/**
	 * @brief Return the number of the layer at a certain level
	 * @param Layer level
	 * @return Layer number
	 */
	const int layerNumberFromLevel(const int);
	/**
	 * @brief Return the layer count
	 * @return Number of layers in doc
	 */
	const int layerCount();
	/**
	 * @brief Lower a layer
	 * @param Number of the layer
	 * @return Success or failure
	 */
	const bool lowerLayer(const int);
	/**
	 * @brief Lower a layer using the level
	 * @param Level of the layer
	 * @return Success or failure
	 */
	const bool lowerLayerByLevel(const int);
	/**
	 * @brief Raise a layer
	 * @param Number of the layer
	 * @return Success or failure
	 */
	const bool raiseLayer(const int);
	/**
	 * @brief Raise a layer using the level
	 * @param Level of the layer
	 * @return Success or failure
	 */
	const bool raiseLayerByLevel(const int);
	/**
	 * @brief Return the layer name
	 * @param Number of the layer
	 * @return Name of the layer 
	 */
	const QString &layerName(const int);
	/**
	 * @brief Change the name of a layer
	 * @param Number of the layer
	 * @param New name of the layer
	 * @return Success or failure
	 */
	const bool changeLayerName(const int, const QString&);
	/**
	 * @brief Does the layer have items on it?
	 * @param Number of the layer
	 * @return Layer contains items bool
	 */
	const bool layerContainsItems(const int);
	/**
	 * @brief Renumber a layer. Used in particular for reinsertion for undo/redo
	 * @param Existing layer number
	 * @param New layer number
	 * @return Success or failure
	 */
	const bool renumberLayer(const int, const int);
	/**
	 * @brief Return a list of the layers in their order
	 * @param QStringList to insert the layer names into
	 */
	void orderedLayerList(QStringList*);
	//Items
	const bool deleteTaggedItems();
	
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
	/*!
	 * @brief Returns a stringlist of the item attributes within the document
	 */
	QStringList getItemAttributeNames();
	
	/*!
	 * @brief Returns a qmap of the fonts used within the document
	 */
	void getUsedFonts(QMap<QString,QFont> *Really);
	/*!
	* @brief Builds a qmap of the colours used within the document
	*/
	void getUsedColors(ColorList &colorsToUse, bool spot = false);
	/*!
	 * @brief TODO: Reorganise the fonts.. how? Moved from scribus.cpp
	 */
	void reorganiseFonts();
	/**
	 * @brief Set and get the document's unit index
	 */
	void setUnitIndex(const int);
	const int unitIndex();
	const double unitRatio();
	/**
	 * @brief Apply a master page
	 */
	const bool applyMasterPage(const QString& in, const int);
	/**
	 * @brief Undo function for applying a master page
	 */
	void restoreMasterPageApplying(SimpleState *state, bool isUndo);
	/**
	 * @brief Perform the doc only collect for output functions
	 */
	const bool collectForOutput(const QString& newDirectory, const bool withFonts = false);
	/**
	 * @brief Save function
	 */
	const bool save(const QString&);
	/**
	 * @brief Set the page margins. Current code uses current page only, also provide a (currently, TODO) option for this.
	 */
	const bool changePageMargins(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double Height, const double width, const int orientation, const QString& pageSize, const int pageNumber=-1);
	/**
	 * @brief Recalculate the colors after CMS settings change. Update the items in the doc accordingly.
	 */
	 void recalculateColors();
	
protected:
	void addSymbols();
	bool loading;
	bool modified;
	int ActiveLayer;
	int docUnitIndex;
	double docUnitRatio;
	UndoManager *undoManager;

public: // Public attributes
	bool is12doc; //public for now, it will be removed later
	int NrItems;
	int First;
	int Last;
	int viewCount;
	int viewID;
	bool SnapGuides;
	bool GuideLock;
	/** Scratch space around Pages */
	double ScratchLeft;
	double ScratchRight;
	double ScratchTop;
	double ScratchBottom;
/** Minimum and Maximum Points of Document */
	FPoint minCanvasCoordinate;
	FPoint maxCanvasCoordinate;
	double rulerXoffset;
	double rulerYoffset;
  /** List of Pages */
	QPtrList<Page> Pages;
  /** List of Master Pages */
	QPtrList<Page> MasterPages;
  /** List of Document Pages */
	QPtrList<Page> DocPages;
  /** Mapping Master Page Name to Master Page numbers */
	QMap<QString,int> MasterNames;
  /** List of Objects */
	QPtrList<PageItem> Items;
	QPtrList<PageItem> MasterItems;
	QPtrList<PageItem> DocItems;
	QPtrList<PageItem> FrameItems;
  /** Pagewidth  */
	double pageWidth;
  /** Pageheight */
	double pageHeight;
  /** Number of Pages */
	int pageCount;
  /** Margins */
	MarginStruct pageMargins;
	QValueList<PageSet> pageSets;
  /** Number of Columns */
	double PageSp;
  /** Distance of Columns */
	double PageSpa;
  /** Flag for automatic Textframes */
	bool PageAT;
 /** current Pagelayout */
	int currentPageLayout;
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
	DocumentInformation documentInfo;
	int appMode;
	int SubMode;
	double *ShapeValues;
	int ValCount;
	QString DocName;
	QMap<QString,QFont> UsedFonts;
	SCFonts *AllFonts;
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
	int CurrTextScaleV;
	int CurrTextBase;
	int CurrTextShadowX;
	int CurrTextShadowY;
	int CurrTextOutline;
	int CurrTextUnderPos;
	int CurrTextUnderWidth;
	int CurrTextStrikePos;
	int CurrTextStrikeWidth;
	bool EditClip;
	int EditClipMode;
	typoPrefs typographicSettings;
	guidesPrefs guidesSettings;
	toolPrefs toolSettings;
	QMap<QString, checkerPrefs> checkerProfiles;
	QString curCheckProfile;
  /** Letztes Element fuer AutoTextrahmen */
	PageItem *LastAuto;
  /** Erstes Element fuer AutoTextrahmen */
	PageItem *FirstAuto;
	bool DragP;
	bool leaveDrag;
	PageItem *DraggedElem;
	PageItem *ElemToLink;
	QValueList<uint> DragElements;
	QValueList<ParagraphStyle> docParagraphStyles;
	QValueList<Layer> Layers;
	bool MasterP;
	bool marginColored;
	int GroupCounter;
	CMSData CMSSettings;
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
	cmsHTRANSFORM stdProofGC;
	cmsHTRANSFORM stdProofCMYKGC;
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
	bool masterPageMode;
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
	QTimer *autoSaveTimer;
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
	ToCSetupVector docToCSetups;
	FPointArray symReturn;
	FPointArray symNewLine;
	FPointArray symTab;
	FPointArray symNonBreak;
	FPointArray symNewCol;
	FPointArray symNewFrame;
};

#endif
