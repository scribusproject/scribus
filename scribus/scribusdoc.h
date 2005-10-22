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
//#include "page.h"
#include "pageitem.h"
#include "pageitem_line.h"
#include "pageitem_textframe.h"

#ifdef HAVE_CMS
	#include CMS_INC
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

class Page;
class UndoManager;
class UndoState;
class PDFOptions;
class Hyphenator;


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
	/**
	 * @brief Move page(s) within the document
	 * @param From page index
	 * @param To page index
	 * @param Position to move to (page index)
	 * @param Before, After or at the end
	 */
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

	bool AddFont(QString name, int fsize = 10);
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
	void getUsedFonts(QMap<QString,int> *Really);
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
	 * @brief Save function
	 */
	const bool save(const QString&);
	/**
	 * @brief Set the page margins. Current code uses current page only, also provide a (currently, TODO) option for this.
	 */
	const bool changePageMargins(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double Height, const double width, const int orientation, const QString& pageSize, const int pageNumber=-1, const int pageType = 0);
	/**
	 * @brief Recalculate the colors after CMS settings change. Update the items in the doc accordingly.
	 */
	 void recalculateColors();
	/**
	 * @brief Sets up the ScText defaults from the document
	 */
	void setScTextDefaultsFromDoc(ScText *);
	/**
	 * @brief Copies a normal page to be a master pages
	 */
	const bool copyPageToMasterPage(const int, const int, const int, const QString&);
	/**
	 * @brief Paste an item to the document.
	 * The bulk of a paste item process runs here for want of a better place, but its a better place
	 * than the view where it used to be. 
	 * TODO Once the pageitem restructure is done, this is probably unnecessary but it removes the 
	 * unnecessary part from the view for now which is overloaded with non ScrollView code.
	 */
	//TODO: void PasteItem(struct CopyPasteBuffer *Buffer, bool loading, bool drag = false);
	
	/**
	 * @brief Add an Item to the document.
	 * A simple function to create an item of a defined type and add it to the document
	 * Will need extensive rewriting once we have various classes of PageItems, at a guess.
	 *
	 * @param itemFinalised Used to handle item creation for undo while the user is still dragging.
	 * @return Number of created item, -1 on failure.
	 */
	int itemAdd(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double b, const double h, const double w, const QString& fill, const QString& outline, const bool itemFinalised);
	
	/**
	 * @brief Commit item creation when a user has click-drag created an item
	 * Only called from ScribusView. Note the undo target is the page, so the undo code remains their for now.
	 * @return If an item was committed and the view must emit its signal, which needs removing from here, TODO.
	 */
	bool itemAddCommit(const int itemNumber);
	
	/**
	 * @brief Finalise item creation. Simply split off code from itemAdd
	 * Only to be called from itemAdd()
	 */
	void itemAddDetails(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const int itemNumber);
	
	//itemDelete
	//itemBlah...
	
	/**
	 * @brief Run this common frame item update code
	 */
	void updateFrameItems();
	
	/**
	 * @brief Doc uses automatic text frames?
	 */
	const bool usesAutomaticTextFrames();
	void setUsesAutomaticTextFrames(const bool);
	
	/**
	 * @brief Load images into an image frame, moved from the view
	 * @retval Return false on failure
	 */
	const bool LoadPict(QString fn, int ItNr, bool reload = false);
	const bool loadPict(QString fn, PageItem *pageItem, bool reload = false);
	
	/**
	 * @brief Find the minX,MinY and maxX,maxY for the canvas required for the doc
	 */
	void canvasMinMax(FPoint&, FPoint&);
	
	int OnPage(double x2, double  y2);
	int OnPage(PageItem *currItem);
	void GroupOnPage(PageItem *currItem);
	void reformPages(double& maxX, double& maxY, bool moveObjects = true);
	
	/**
	 * @brief Return the x or y offset for a page on the canvas
	 * @retval double containing the offset. Returns -1.0 if page not in Pages list (as -ve is not possible).
	 * Mostly saves bringing in extra includes into files that already have scribusdoc.h
	 */
	const double getXOffsetForPage(const int);
	const double getYOffsetForPage(const int);
	
	/**
	 * @brief Item type conversion functions
	 */
	PageItem* convertItemTo(PageItem *, PageItem::ItemType);
	
	/**
	 * @brief The page number of the current page
	 */
	const int currentPageNumber();

protected:
	void addSymbols();
	bool loading;
	bool modified;
	int ActiveLayer;
	int docUnitIndex;
	double docUnitRatio;
	UndoManager *undoManager;
	bool automaticTextFrames; // Flag for automatic Textframes

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
	QMap<QString,int> UsedFonts;
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
	
private:
	bool _itemCreationTransactionStarted;
};

#endif
