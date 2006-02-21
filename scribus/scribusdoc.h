/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include "page.h"
#include "pageitem.h"
#include "pageitem_line.h"
#include "pageitem_textframe.h"
#include "pagestructs.h"


#ifdef HAVE_CMS
	#include CMS_INC
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

class UndoManager;
class UndoState;
class PDFOptions;
class Hyphenator;
class Selection;
class ScribusView;

class QProgressBar;


/**! \brief the Document Class
  */
class SCRIBUS_API ScribusDoc : public QObject, public UndoObject
{
	Q_OBJECT

public:
	ScribusDoc();
	~ScribusDoc();
	void setup(const int, const int, const int, const int, const int, const QString&, const QString&);
	void setLoading(const bool);
	bool isLoading() const;
	void setModified(const bool);
	bool isModified() const;
/** Setzt die Seitenattribute */
	void setPage(double b, double h, double t, double l, double r, double bo, double sp, double ab, bool atf, int fp);
	void resetPage(double t, double l, double r, double bo, int fp);

	/**
	 * @brief Return the view associated with the document
	 */
	ScribusView* view() const;
	
	// Add, delete and move pages
	
	Page* addPage(const int pageNumber, const QString& masterPageName=QString::null, const bool addAutoFrame=false);
	void deleteMasterPage(const int);
	void deletePage(const int);
	/**
	 * @brief Add a master page with this function, do not use addPage
	 */
	Page* addMasterPage(const int, const QString&);

	/**
	 * @brief Add the automatic text frame to the page
	 * @param pageNumber page number
	 * @return number of frame
	 */
	int addAutomaticTextFrame(const int pageNumber);
	/**
	 * Set the left and right margins based on the location of the page
	 * @param pageIndex 
	 */
	void setLocationBasedPageLRMargins(uint pageIndex);
	/**
	 * @brief Move page(s) within the document
	 * @param from page index
	 * @param to page index
	 * @param ziel target to move to (page index)
	 * @param art Before, After or at the end
	 */
	void movePage(const int from, const int to, const int ziel, const int art);
	
	/**
	 * @brief Copy a page (pageNumberToCopy) copyCount times, whereToInsert(before or after) the existingPage or at the end.
	 * @param pageNumberToCopy 
	 * @param existingPage 
	 * @param whereToInsert 
	 * @param copyCount 
	 */
	void copyPage(int pageNumberToCopy, int existingPage, int whereToInsert, int copyCount);
	
	// Add, delete and move layers
	/**
	 * @brief Add a layer to the current document
	 * @param layerName name of layer
	 * @param activate the layer active
	 * @return Number of the layer created
	 */
	int addLayer(const QString& layerName=QString::null, const bool activate=false);
	/**
	 * @brief Delete a layer from the current document
	 * @param layerNumber of layer
	 * @param deleteItems the items on the layer too?
	 * @return Success or failure
	 */
	bool deleteLayer(const int layerNumber, const bool deleteItems);
	/**
	 * @brief Return the number of the current layer
	 * @return Active layer number
	 */
	int activeLayer();

	/**
	 * @brief Return the name of the current layer
	 * @return Name of the layer
	 */
	const QString& activeLayerName();
	/**
	 * @brief Set the active layer via the layer number
	 * @param layerToActivate Number of the layer
	 * @return Success or failure
	 */
	bool setActiveLayer(const int layerToActivate);
	/**
	 * @brief Set the active layer via the layer name
	 * @param layerNameToActivate Name of the layer
	 * @return Success or failure
	 */
	bool setActiveLayer(const QString & layerNameToActivate);
	/**
	 * @brief Set the layer printable via the layer number
	 * @param layerNumber Number of the layer
	 * @param isPrintable bool true = layer is prantable
	 * @return Success or failure
	 */
	bool setLayerPrintable(const int layerNumber, const bool isPrintable);
	/**
	 * @brief Is the layer printable
	 * @param layerNumber Number of the layer
	 * @return Printable or not
	 */
	bool layerPrintable(const int layerNumber);
	/**
	 * @brief Set the layer visible via the layer number
	 * @param layerNumber Number of the layer
	 * @param isViewable true = layer is visible
	 * @return Success or failure
	 */
	bool setLayerVisible(const int layerNumber, const bool isViewable);
	/**
	 * @brief Is the layer visible
	 * @param layerNumber Number of the layer
	 * @return Visible or not
	 */
	bool layerVisible(const int layerNumber);
	/**
	 * @brief Return the level of the requested layer
	 * @param layerNumber Number of the layer
	 * @return Level of the layer
	 */
	int layerLevelFromNumber(const int layerNumber);
	/**
	 * @brief Return the number of the layer at a certain level
	 * @param layerLevel Layer level
	 * @return Layer number
	 */
	int layerNumberFromLevel(const int layerLevel);
	/**
	 * @brief Return the layer count
	 * @return Number of layers in doc
	 */
	int layerCount() const;
	/**
	 * @brief Lower a layer
	 * @param layerNumber Number of the layer
	 * @return Success or failure
	 */
	bool lowerLayer(const int layerNumber);
	/**
	 * @brief Lower a layer using the level
	 * @param layerLevel Level of the layer
	 * @return Success or failure
	 */
	bool lowerLayerByLevel(const int layerLevel);
	/**
	 * @brief Raise a layer
	 * @param layerNumber Number of the layer
	 * @return Success or failure
	 */
	bool raiseLayer(const int layerNumber);
	/**
	 * @brief Raise a layer using the level
	 * @param layerLevel Level of the layer
	 * @return Success or failure
	 */
	bool raiseLayerByLevel(const int layerLevel);
	/**
	 * @brief Return the layer name
	 * @param layerNumber Number of the layer
	 * @return Name of the layer
	 */
	const QString &layerName(const int layerNumber) const;
	/**
	 * @brief Change the name of a layer
	 * @param layerNumber number of the layer
	 * @param newName new name of the layer
	 * @return Success or failure
	 */
	bool changeLayerName(const int layerNumber, const QString& newName);
	/**
	 * @brief Does the layer have items on it?
	 * @param layerNumber Number of the layer
	 * @return Layer contains items bool
	 */
	bool layerContainsItems(const int layerNumber);
	/**
	 * @brief Renumber a layer. Used in particular for reinsertion for undo/redo
	 * @param layerNumber old layer number
	 * @param newLayerNumber New layer number
	 * @return Success or failure
	 */
	bool renumberLayer(const int layerNumber, const int newLayerNumber);
	/**
	 * @brief Return a list of the layers in their order
	 * @param list QStringList to insert the layer names into
	 */
	void orderedLayerList(QStringList* list);
	//Items
	bool deleteTaggedItems();

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
	int unitIndex() const;
	double unitRatio() const;
	/**
	 * @brief Apply a master page
	 */
	bool applyMasterPage(const QString& in, const int);
	/**
	 * @brief Undo function for applying a master page
	 */
	void restoreMasterPageApplying(SimpleState *state, bool isUndo);
	/**
	 * @brief Save function
	 */
	bool save(const QString&);
	/**
	 * @brief Set the page margins. Current code uses current page only, also provide a (currently, TODO) option for this.
	 */
	bool changePageMargins(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double Height, const double width, const int orientation, const QString& pageSize, const int pageNumber=-1, const int pageType = 0);
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
	\param itemType type
	\param frameType frame type
	\param x X pos
	\param y Y pos
	\param b width
	\param h height
	\param w ?
	\param fill fill color name
	\param outline outline color name
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
	bool usesAutomaticTextFrames() const;
	void setUsesAutomaticTextFrames(const bool);
	
	/**
	 * @brief Load images into an image frame, moved from the view
	 * @retval Return false on failure
	 */
	bool LoadPict(QString fn, int ItNr, bool reload = false);
	/**
	 * 
	 * @param fn 
	 * @param pageItem 
	 * @param reload 
	 * @return 
	 */
	bool loadPict(QString fn, PageItem *pageItem, bool reload = false);
	/**
	 * \brief Handle image with color profiles
	 * @param Pr profile
	   @param PrCMYK cmyk profile
	   @param dia optional progress widget
	 */
	void RecalcPictures(ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia = 0);
	/**
	 * @brief Find the minX,MinY and maxX,maxY for the canvas required for the doc
	 */
	void canvasMinMax(FPoint&, FPoint&);
	
	int OnPage(double x2, double  y2);
	int OnPage(PageItem *currItem);
	void GroupOnPage(PageItem *currItem);
	//void reformPages(double& maxX, double& maxY, bool moveObjects = true);
	void reformPages(bool moveObjects = true);
	
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
	PageItem* convertItemTo(PageItem *currItem, PageItem::ItemType newType, PageItem* secondaryItem=NULL);
	
	/**
	 * @brief The page number of the current page
	 */
	const int currentPageNumber();
	
	/**
	 * @brief Return true iff the passed name is not used by any existing PageItem
	 *        in the same document as this PageItem.
	 * @author Craig Ringer
	 ** CB Moved from PageItem
	 */
	bool itemNameExists(const QString itemName);
	
	/**
	 * @brief Set the doc into Master page mode
	 * Do we need to return if the move to master page mode was successful?
	 */
	void setMasterPageMode(const bool);

	/*** Is the document in master page mode? */
	bool masterPageMode() const { return m_masterPageMode; }
	
	/**
	 * @brief Add a section to the document sections list
	 * Set number to -1 to add in the default section if the map is empty
	 */
	void addSection(const int number=0, const QString& name=QString::null, const uint fromindex=0, const uint toindex=0, const  DocumentSectionType type=Type_1_2_3, const uint sectionstartindex=0, const bool reversed=false, const bool active=true);
	/**
	 * @brief Delete a section from the document sections list
	 */
	bool deleteSection(const uint);
	/**
	 * @brief Gets the page number to be printed based on the section it is in.
	 * Returns QString::null on failure to find the pageIndex
	 */
	const QString getSectionPageNumberForPageIndex(const uint) const;
	/**
	 * @brief Gets the key of the sections map based on the section the page index is in.
	 * Returns -1 on failure to find the pageIndex
	 */
	int getSectionKeyForPageIndex(const uint pageIndex) const;
	/**
	 *
	 *
	 */
	void updateSectionPageNumbersToPages();
	/**
	 *
	 *
	 */
	/**
	 * 
	 * @param otherPageIndex 
	 * @param location 
	 * @param count 
	 */
	void addPageToSection(const uint otherPageIndex, const uint location, const uint count=1);
	/**
	 * 
	 * @param pageIndex 
	 */
	void removePageFromSection(const uint pageIndex);
	/**
	 * 
	 */
	void setFirstSectionFromFirstPageNumber();
	/**
	 * @brief Update the fill and line QColors for all items in the doc
	 */
	void updateAllItemQColors();
	/**
	 * 
	 */
	void buildAlignItemList();
	/**
	 * \brief Insert a color into the documents color list
	 * @param nam Name of the colour
	 * @param c Cyan component
	 * @param m Magenta component
	 * @param y Yellow component
	 * @param k Black component
	 */
	void insertColor(QString nam, double c, double m, double y, double k);
	
	QMap<QString, double>& constants() { return m_constants; }
	/**
	 * \brief Get the location of the page on the canvas, ie, left, middle, or right
	 * Does not give information about middle left, etc.
	 * @param pageIndex Index of page to find location for
	 * @return LeftPage, MiddlePage, RightPage, enum from pagestructs.h
	 */
	PageLocation locationOfPage(int pageIndex);
	
	bool sendItemSelectionToBack();
	bool bringItemSelectionToFront();
	
	void ChLineWidth(double w);
	void ChLineArt(PenStyle w);
	void ChLineJoin(PenJoinStyle w);
	void ChLineEnd(PenCapStyle w);
	void ChLineSpa(double w);
	void ChLineSpaMode(int w);
	void ChLocalXY(double x, double y);
	void ChLocalSc(double x, double y);
	void ItemFont(QString fon);
	void ItemPen(QString farbe);
	void ItemTextBrush(QString farbe);
	void ItemTextBrushS(int sha);
	void ItemTextPen(QString farbe);
	void ItemTextPenS(int sha);
	void ItemTextScaleV(int sha);
	void ItemTextScale(int sha);
	void setItemTextBase(int sha);
	void setItemTextOutline(int sha);
	void setItemTextShadow(int shx, int shy);
	void setItemTextUnderline(int pos, int wid);
	void setItemTextStrike(int pos, int wid);
	void ItemBrush(QString farbe);
	void ItemBrushShade(int sha);
	void ItemPenShade(int sha);
	void ItemGradFill(int typ);
	void chTyStyle(int s);
	void SetAbStyle(int s);
	void chAbStyle(PageItem *currItem, int s);
	void chKerning(int us);
	void chFSize(int size);
	void FlipImageH();
	void FlipImageV();
	void MirrorPolyH();
	void MirrorPolyV();
	
	void setRedrawBounding(PageItem *currItem);
	void adjustCanvas(FPoint minPos, FPoint maxPos, bool absolute = false);
	void recalcPicturesRes();
	void connectDocSignals();
	
	
protected:
	void addSymbols();
	ApplicationPrefs& prefsData;
	UndoManager * const undoManager;
	bool loading;
	bool modified;
	int ActiveLayer;
	int docUnitIndex;
	double docUnitRatio;
	bool automaticTextFrames; // Flag for automatic Textframes
	bool m_masterPageMode;
	QMap<QString, double> m_constants;
	
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
	QPtrList<Page>* Pages;
  /** List of Master Pages */
	QPtrList<Page> MasterPages;
  /** List of Document Pages */
	QPtrList<Page> DocPages;
  /** Mapping Master Page Name to Master Page numbers */
	QMap<QString,int> MasterNames;
  /** List of Objects */
	QPtrList<PageItem>* Items;
	QPtrList<PageItem> MasterItems;
	QPtrList<PageItem> DocItems;
	QPtrList<PageItem> FrameItems;
	/**List of objects for alignment purposes*/
	Selection * const selection;
  /** Pagewidth  */
	double pageWidth;
  /** Pageheight */
	double pageHeight;
  /** Number of Pages */
	// int pageCount; Disabled CR no longer required
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
	SCFonts * const AllFonts;
	QValueList<AlignObjs> AObjects;
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
	int MinWordLen;
	int HyCount;
	QString Language;
	bool Automatic;
	bool AutoCheck;
	
	PDFOptions PDF_Options;
	bool RePos;
	struct BookMa {
					QString Title;
					QString Text;
					QString Aktion;
					PageItem *PageObject;
					int Parent;
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
	QTimer * const autoSaveTimer;
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
	DocumentSectionMap sections;
	FPointArray symReturn;
	FPointArray symNewLine;
	FPointArray symTab;
	FPointArray symNonBreak;
	FPointArray symNewCol;
	FPointArray symNewFrame;
	
	Hyphenator * const docHyphenator;
private:
	bool _itemCreationTransactionStarted;
	
signals:
	//Lets make our doc talk to our GUI rather than confusing all our normal stuff
	/**
	 * @brief Let the document tell whatever is listening that it has changed
	 */
	void changed();
	void updateContents();
	void refreshItem(PageItem *);
	void canvasAdjusted(double width, double height, double dX, double dY);
	void firstSelectedItemType(int);
	void setApplicationMode(int);
	/**
	 * @brief A signal for when the outline palette needs to rebuild itself
	 * Emit when:
	 * - An item is created or deleted
	 * - An item changes page
	 * - An page is created or deleted
	 * - Some items are grouped or a group is ungrouped
	 * This also applies to Master Pages
	 */
	void signalRebuildOutLinePalette();
	
public slots:
	void itemSelection_ToggleLock();
	void itemSelection_ToggleSizeLock();
	void itemSelection_ToggleImageShown();
	void itemSelection_TogglePrintEnabled();
	void updatePic();
	void updatePict(QString name);
	void removePict(QString name);
};

#endif
