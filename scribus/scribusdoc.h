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
#include <QColor>
#include <QFont>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPixmap>
#include <QRectF>
#include <QStringList>
#include <QTimer>


#include "gtgettext.h" //CB For the ImportSetup struct and itemadduserframe
#include "scribusapi.h"
#include "observable.h"
#include "prefsstructs.h"
#include "documentinformation.h"
#include "undoobject.h"
#include "page.h"
#include "pageitem.h"
#include "pagestructs.h"
#include "usertaskstructs.h"
#include "styles/styleset.h"
#include "scguardedptr.h"
#include "updatemanager.h"
#include "sclayer.h"

#include CMS_INC

#include <ft2build.h>
#include FT_FREETYPE_H

class DocUpdater;
class UndoManager;
class UndoState;
class PDFOptions;
class Hyphenator;
class Selection;
class ScribusView;
class ScribusMainWindow;
class ResourceCollection;
class PageSize;
class ScPattern;
class UndoTransaction;

class QProgressBar;

struct SCRIBUS_API NodeEditContext : public MassObservable<QPointF>
{
	enum SubMode { MOVE_POINT = 0, ADD_POINT = 1, DEL_POINT = 2, SPLIT_PATH = 3 };
	int submode;
	bool isContourLine;
	FPointArray *oldClip;
	UndoTransaction* nodeTransaction;
	double oldItemX;
	double oldItemY;
		
	int ClRe;
	int ClRe2;
	int SegP1;
	int SegP2;
	bool EdPoints;
	bool MoveSym;
	QList<int> SelNode;	
	
	NodeEditContext();
	
	bool hasNodeSelected();
	void deselect();
	
	void reset();
	
	void reset1Control(PageItem* currItem);
	void resetControl(PageItem* currItem);
	FPointArray beginTransaction(PageItem* currItem);
	void finishTransaction(PageItem* currItem);
	ItemState<QPair<FPointArray, FPointArray> >* finishTransaction1(PageItem* currItem);
	void finishTransaction2(PageItem* currItem, ItemState<QPair<FPointArray, FPointArray> >* state);
	void moveClipPoint(PageItem *currItem, FPoint ip);
};


/**! \brief the Document Class
  */
class SCRIBUS_API ScribusDoc : public QObject, public UndoObject, public Observable<ScribusDoc>
{
	Q_OBJECT

public:
	ScribusDoc();
	ScribusDoc(const QString& docName, int unitIndex, const PageSize& pagesize, const MarginStruct& margins, const DocPagesSetup& pagesSetup);
	~ScribusDoc();
	void init();
	void setup(const int, const int, const int, const int, const int, const QString&, const QString&);
	void setLoading(const bool);
	bool isLoading() const;
	void setModified(const bool);
	bool isModified() const;
/** Setzt die Seitenattribute */
	void setPage(double b, double h, double t, double l, double r, double bo, double sp, double ab, bool atf, int fp);
	void resetPage(MarginStruct& newMargins, int fp);

	/**
	 * @brief Return the view associated with the document
	 */
	ScribusView* view() const;
	ScribusMainWindow* scMW() const {return m_ScMW;}
	void setGUI(bool hasgui, ScribusMainWindow* mw, ScribusView* view);

	/**
	 * @brief Return the guarded object associated with the document
	 */
	const ScGuardedPtr<ScribusDoc>& guardedPtr() const;
	
	UpdateManager* updateManager() { return &m_updateManager; }
	MassObservable<PageItem*> * itemsChanged() { return &m_itemsChanged; }
	MassObservable<Page*>     * pagesChanged() { return &m_pagesChanged; }
	MassObservable<QRectF>    * regionsChanged() { return &m_regionsChanged; }
	void invalidateRegion(QRectF region);
	
	// Add, delete and move pages
	
	Page* addPage(const int pageNumber, const QString& masterPageName=QString::null, const bool addAutoFrame=false);
	void deletePage(const int);
	//! @brief Add a master page with this function, do not use addPage
	Page* addMasterPage(const int, const QString&);
	void deleteMasterPage(const int);
	//! @brief Rebuild master name list
	void rebuildMasterNames(void);
	//! @brief Replace a master page by default one
	void replaceMasterPage(const QString& oldMasterPage);
	//! @brief Rename a master page
	bool renameMasterPage(const QString& oldPageName, const QString& newPageName);
	//! @brief Create the default master pages based on the layout selected by the user, ie, Normal, Normal Left, etc.
	void createDefaultMasterPages();
	//! @brief Create the requested pages in a new document, run after createDefaultMasterPages()
	void createNewDocPages(int pageCount);
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
	 * @brief Copies a layer from the current document
	 * @param layerNumberToCopy source layer
	 * @param whereToInsert target layer
	 * @return Success or failure
	 */
	void copyLayer(int layerNumberToCopy, int whereToInsert);
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
	 * @brief Set the layer locked via the layer number
	 * @param layerNumber Number of the layer
	 * @param isLocked true = layer is locked
	 * @return Success or failure
	 */
	bool setLayerLocked(const int layerNumber, const bool isLocked);
	/**
	 * @brief Is the layer locked
	 * @param layerNumber Number of the layer
	 * @return Locked or not
	 */
	bool layerLocked(const int layerNumber);
	/**
	 * @brief Set the layer flow via the layer number
	 * @param layerNumber Number of the layer
	 * @param flow true = Text flows around objects on this layer
	 * @return Success or failure
	 */
	bool setLayerFlow(const int layerNumber, const bool flow);
	/**
	 * @brief does text flow around objects on this layer
	 * @param layerNumber Number of the layer
	 * @return flow or not
	 */
	bool layerFlow(const int layerNumber);
	/**
	 * @brief Set the layer transparency via the layer number
	 * @param layerNumber Number of the layer
	 * @param trans transparency value 0.0 - 1.0
	 * @return Success or failure
	 */
	bool setLayerTransparency(const int layerNumber, double trans);
	/**
	 * @brief returns the layer transparency
	 * @param layerNumber Number of the layer
	 * @return transparency value 0.0 - 1.0
	 */
	double layerTransparency(const int layerNumber);
	/**
	 * @brief Set the layer layerBlendMode via the layer number
	 * @param layerNumber Number of the layer
	 * @param blend layerBlendMode
	 * @return Success or failure
	 */
	bool setLayerBlendMode(const int layerNumber, int blend);
	/**
	 * @brief returns the layer BlendMode
	 * @param layerNumber Number of the layer
	 * @return layerBlendMode
	 */
	int layerBlendMode(const int layerNumber);
	/**
	 * @brief Return the level of the requested layer
	 * @param layerNumber Number of the layer
	 * @return Level of the layer
	 */
	int layerLevelFromNumber(const int layerNumber);
	/**
	 * @brief Set the layer marker color
	 * @param layerNumber Number of the layer
	 * @param color color of the marker
	 * @return Success or failure
	 */
	bool setLayerMarker(const int layerNumber, QColor color);
	/**
	 * @brief returns the layer marker color
	 * @param layerNumber Number of the layer
	 * @return marker color
	 */
	QColor layerMarker(const int layerNumber);
	/**
	 * @brief Set the layer outline mode via the layer number
	 * @param layerNumber Number of the layer
	 * @param outline true = layer is displayed in outlines only
	 * @return Success or failure
	 */
	bool setLayerOutline(const int layerNumber, const bool outline);
	/**
	 * @brief is this layer in outline mode
	 * @param layerNumber Number of the layer
	 * @return outline or not
	 */
	bool layerOutline(const int layerNumber);
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
	QString layerName(const int layerNumber) const;
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

	/*!
		* @brief Builds a qmap of the icc profiles used within the document
	 */
	void getUsedProfiles(ProfilesL& usedProfiles);
	bool OpenCMSProfiles(ProfilesL InPo, ProfilesL InPoCMYK, ProfilesL MoPo, ProfilesL PrPo);
	void CloseCMSProfiles();
	void SetDefaultCMSParams();
	/**
	 * @brief Switch Colormanagement on or of
	 * @param enable bool, if true Colormanagement is switched on, else off
	 */
	void enableCMS(bool enable);
	
	const ParagraphStyle& paragraphStyle(QString name) { return docParagraphStyles.get(name); }
	const StyleSet<ParagraphStyle>& paragraphStyles()   { return docParagraphStyles; }
	bool isDefaultStyle( const ParagraphStyle& p ) const { return docParagraphStyles.isDefault(p); }
	bool isDefaultStyle( const CharStyle& c ) const { return docCharStyles.isDefault(c); }
// 	bool isDefaultStyle( LineStyle& l ) const { return MLineStyles......; }

	void getNamedResources(ResourceCollection& lists) const;
	void replaceNamedResources(ResourceCollection& newNames);	
	
	void redefineStyles(const StyleSet<ParagraphStyle>& newStyles, bool removeUnused=false);
	/**
	 * @brief Remove any reference to old styles and replace with new name. This needs to be
	 *        called when a style was removed. New name may be "".
	 * @param newNameForOld a map which maps the name of any style to remove to a new stylename
	 */
	void replaceStyles(const QMap<QString,QString>& newNameForOld);
	void loadStylesFromFile(QString fileName, StyleSet<ParagraphStyle> *tempStyles = NULL,
	                                          StyleSet<CharStyle> *tempCharStyles = NULL,
	                                          QMap<QString, multiLine> *tempLineStyles = NULL);

	const CharStyle& charStyle(QString name) { return docCharStyles.get(name); }
	const StyleSet<CharStyle>& charStyles()  { return docCharStyles; }
	void redefineCharStyles(const StyleSet<CharStyle>& newStyles, bool removeUnused=false);
	/**
	 * @brief Remove any reference to old styles and replace with new name. This needs to be
	 *        called when a style was removed. New name may be "".
	 * @param newNameForOld a map which maps the name of any style to remove to a new stylename
	 */
	void replaceCharStyles(const QMap<QString,QString>& newNameForOld);

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

	bool AddFont(QString name, int fsize = 10);
	/*!
	 * @brief TODO: Reorganise the fonts.. how? Moved from scribus.cpp
	 * CB: almost the same as getUsedFonts???
	 */
	void reorganiseFonts();
	/*!
	 * @brief Returns a qmap of the fonts and  their glyphs used within the document
	 */
	void getUsedFonts(QMap<QString,QMap<uint, FPointArray> > &Really);
	void checkItemForFonts(PageItem *it, QMap<QString, QMap<uint, FPointArray> > & Really, uint lc);

	/*!
	 * @brief Replace line style colors
	 */
	void replaceLineStyleColors(const QMap<QString, QString>& colorMap);
	/*!
	* @brief Builds a qmap of the colours used within the document
	*/
	void getUsedColors(ColorList &colorsToUse, bool spot = false);
	/*!
	* @brief Return if a specific color is used by line styles
	*/
	bool lineStylesUseColor(const QString& colorName);
	/*!
	* @brief Set the patterns for a document
	*/
	bool addPattern(QString &name, ScPattern& pattern);
	void setPatterns(QMap<QString, ScPattern> &patterns);
	/*!
	* @brief Builds a QStringList of the patterns used within the document
	*/
	QStringList getUsedPatterns();
	QStringList getUsedPatternsSelection();
	QStringList getUsedPatternsHelper(QString pattern, QStringList &results);
	/**
	 * @brief Set and get the document's unit index
	 */
	void setUnitIndex(const int);
	int unitIndex() const;
	double unitRatio() const;
	/**
	 * @brief Apply a master page
	 */
	bool applyMasterPage(const QString& pageName, const int pageNumber);
	/**
	 * @brief Undo function for applying a master page
	 */
	void restoreMasterPageApplying(SimpleState *state, bool isUndo);
	void restorePageCopy(SimpleState *state, bool isUndo);
	/**
	 * @brief Save function
	 */
	bool save(const QString& fileName, QString* savedFile = NULL);
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
	bool copyPageToMasterPage(const int, const int, const int, const QString&, bool);
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

	/** Add an item to the page based on the x/y position. Item will be fitted to the closest guides/margins */
	int itemAddArea(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double w, const QString& fill, const QString& outline, const bool itemFinalised);
	
	/**
	 * @brief Allow the user to create a frame easily with some simple placement and sizing options
	 * @param iafData a InsertAFrameData structure with params
	 * @return int item id? FIXME
	 */
	int itemAddUserFrame(InsertAFrameData &iafData);

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

	uint getItemNrfromUniqueID(uint unique);
	//itemDelete
	//itemBlah...
	
	/**
	 * @brief Run this common frame item update code
	 */
	void updateFrameItems();
	/**
	 * @brief Renumbers the items into the order they are stored in in the lists.
	 * Utility function used in various places, basically handles keeping items numbered in the way
	 * they are layered. When layer is a property and not a fuction of storage, this should be removed.
	 * Depends on the Items pointer pointing to the correct item list (doc, master, etc).
	 * @sa updateFrameItems();
	 */
	void renumberItemsInListOrder();
	/**
	 * @brief Rebuild item lists taking into account layer order.
	 * Utility function used in various places, basically handles keeping items numbered in the way
	 * they are layered. When layer is a property and not a fuction of storage, this should be removed.
	 * @sa updateFrameItems();
	 */
	void rebuildItemLists();
	/**
	 * @brief Doc uses automatic text frames?
	 */
	bool usesAutomaticTextFrames() const;
	void setUsesAutomaticTextFrames(const bool);
	
	/**
	 * @brief Load images into an image frame, moved from the view
	 * @retval Return false on failure
	 */
	bool LoadPict(QString fn, int ItNr, bool reload = false, bool showMsg = false);
	/**
	 * 
	 * @param fn 
	 * @param pageItem 
	 * @param reload 
	 * @return 
	 */
	bool loadPict(QString fn, PageItem *pageItem, bool reload = false, bool showMsg = false);
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
	double getXOffsetForPage(const int);
	double getYOffsetForPage(const int);
	void getBleeds(int pageNumber, MarginStruct& bleedData);
	void getBleeds(Page* page, MarginStruct& bleedData);
	
	/**
	 * @brief Item type conversion functions
	 */
	PageItem* convertItemTo(PageItem *currItem, PageItem::ItemType newType, PageItem* secondaryItem=NULL);
	
	/**
	 * @brief The page number of the current page
	 */
	int currentPageNumber();
	
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
	void setMasterPageMode(bool);

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
	//! @brief Some internal align tools
	typedef enum {alignFirst, alignLast, alignPage, alignMargins, alignGuide, alignSelection } AlignTo;
	void buildAlignItemList(Selection* customSelection=0);
	bool startAlign();
	void endAlign();
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
	PageLocation locationOfPage(int pageIndex) const;
	
	bool sendItemSelectionToBack();
	bool bringItemSelectionToFront();

	void itemSelection_GroupObjects  (bool changeLock, bool lock, Selection* customSelection=0);
	void itemSelection_UnGroupObjects(Selection* customSelection=0);
	void itemSelection_convertItemsTo(const PageItem::ItemType newType, Selection* customSelection=0);

	void itemSelection_ApplyParagraphStyle(const ParagraphStyle & newstyle, Selection* customSelection=0);
	void itemSelection_SetParagraphStyle(const ParagraphStyle & newstyle, Selection* customSelection=0);
	void itemSelection_ApplyCharStyle(const CharStyle & newstyle, Selection* customSelection=0);
	void itemSelection_SetCharStyle(const CharStyle & newstyle, Selection* customSelection=0);
	void itemSelection_EraseParagraphStyle(Selection* customSelection=0);
	void itemSelection_EraseCharStyle(Selection* customSelection=0);

	void itemSelection_SetNamedParagraphStyle(const QString & name, Selection* customSelection=0);
	void itemSelection_SetNamedCharStyle(const QString & name, Selection* customSelection=0);
	void itemSelection_SetNamedLineStyle(const QString & name, Selection* customSelection=0);

	void ChLineWidth(double w);
	void ChLineArt(Qt::PenStyle w);
	void ChLineJoin(Qt::PenJoinStyle w);
	void ChLineEnd(Qt::PenCapStyle w);
	void itemSelection_SetAlignment(int w, Selection* customSelection=0);
	void itemSelection_SetLineSpacing(double w, Selection* customSelection=0);
	void itemSelection_SetLineSpacingMode(int w, Selection* customSelection=0);
	//void ChLocalXY(double x, double y);
	//void ChLocalSc(double x, double y);
	void itemSelection_SetFont(QString fon, Selection* customSelection=0);
	void itemSelection_SetFillColor(QString farbe, Selection* customSelection=0);
	void itemSelection_SetFillShade(int sha, Selection* customSelection=0);
	void itemSelection_SetStrokeColor(QString farbe, Selection* customSelection=0);
	void itemSelection_SetStrokeShade(int sha, Selection* customSelection=0);
	void itemSelection_SetScaleV(int, Selection* customSelection=0);
	void itemSelection_SetScaleH(int, Selection* customSelection=0);
	void itemSelection_SetBaselineOffset(int, Selection* customSelection=0);
	void itemSelection_SetOutlineWidth(int, Selection* customSelection=0);
	void itemSelection_SetShadowOffsets(int shx, int shy, Selection* customSelection=0);
	void itemSelection_SetUnderline(int pos, int wid, Selection* customSelection=0);
	void itemSelection_SetStrikethru(int pos, int wid, Selection* customSelection=0);
	void itemSelection_SetEffects(int s, Selection* customSelection=0);
	void itemSelection_SetOpticalMargins(int i, Selection* customSelection=0);
	void itemSelection_SetColorProfile(const QString& profileName, Selection* customSelection=0);
	void itemSelection_SetRenderIntent(int intentIndex, Selection* customSelection=0);
	
//	void chAbStyle(PageItem *currItem, int s);

	void itemSelection_SetTracking(int us, Selection* customSelection=0);
	void itemSelection_SetFontSize(int size, Selection* customSelection=0);
	//void FlipImageH();
	//void FlipImageV();
	void MirrorPolyH(PageItem *currItem);
	void MirrorPolyV(PageItem *currItem);
	
	void setRedrawBounding(PageItem *currItem);
	void adjustCanvas(FPoint minPos, FPoint maxPos, bool absolute = false);
	void recalcPicturesRes(bool applyNewRes = false);
	void connectDocSignals();
	void removeLayer(int l, bool dl = false); //FIXME: Make protected once scripter function no longer uses this directly
	/*! \brief We call changed() whenever the document needs to know it has been changed.
	 *  If the document is the primary document in a main window, it will signal to enable/disable
	 * certain operations.
	 */
	void changed();
	/*! \brief Get pointer to the current page
	\retval Page* current page object */
	Page* currentPage();
	/*! \brief Set new current page
	\param newPage New current page */
	void setCurrentPage(Page *newPage);
	bool hasGUI() const {return m_hasGUI;}
	/*! \brief Apply grid to a QPoint, from ScribusView */
	QPoint ApplyGrid(const QPoint& in);
	/*! \brief Apply grid to an FPoint, from ScribusView */
	FPoint ApplyGridF(const FPoint& in);
	/*! \brief Does this doc have any TOC setups and potentially a TOC to generate */
	bool hasTOCSetup() { return !docToCSetups.empty(); }
	//! \brief Get the closest guide to the given point
	void getClosestGuides(double xin, double yin, double *xout, double *yout, int *GxM, int *GyM);
	//! \brief Snap an item to the guides
	void SnapToGuides(PageItem *currItem);
	bool ApplyGuides(double *x, double *y);
	bool MoveItem(double newX, double newY, PageItem* ite, bool fromMP = false);
	void RotateItem(double win, int ite);
	void RotateItem(double win, PageItem *currItem);
	void MoveRotated(PageItem *currItem, FPoint npv, bool fromMP = false);
	bool SizeItem(double newX, double newY, int ite, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	bool SizeItem(double newX, double newY, PageItem *pi, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	bool MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP = false, bool constrainRotation=false);
	void AdjustItemSize(PageItem *currItem);
	void moveGroup(double x, double y, bool fromMP = false, Selection* customSelection = 0);
	void rotateGroup(double angle, FPoint RCenter);
	void scaleGroup(double scx, double scy, bool scaleText=true, Selection* customSelection = 0);
	//! \brief Get a list of frames of certain type
	QMap<PageItem*, QString> getDocItemNames(PageItem::ItemType itemType);
	
protected:
	void addSymbols();
	bool m_hasGUI;
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
	ScribusMainWindow* m_ScMW;
	ScribusView* m_View;
	ScGuardedObject<ScribusDoc> m_guardedObject;
	
public: // Public attributes
	bool is12doc; //public for now, it will be removed later
	int NrItems;
	int First;
	int Last;
	int viewCount;
	int viewID;
	bool SnapGuides;
	bool GuideLock;
	/** \brief Scratch space around Pages */
	MarginStruct scratch;
	double GapHorizontal;
	double GapVertical;
// 	double ScratchLeft;
// 	double ScratchRight;
// 	double ScratchTop;
// 	double ScratchBottom;
	/** \brief Minimum and Maximum Points of Document */
	FPoint minCanvasCoordinate;
	FPoint maxCanvasCoordinate;
	double rulerXoffset;
	double rulerYoffset;
	/** \brief List of Pages */
	QList<Page*>* Pages;
	/** \brief List of Master Pages */
	QList<Page*> MasterPages;
	/** \brief List of Document Pages */
	QList<Page*> DocPages;
	/** \brief Mapping Master Page Name to Master Page numbers */
	QMap<QString,int> MasterNames;
	/** \brief List of Objects */
	QList<PageItem*>* Items;
	QList<PageItem*> MasterItems;
	QList<PageItem*> DocItems;
	QList<PageItem*> FrameItems;
	Selection* const m_Selection;
	/** \brief Pagewidth  */
	double pageWidth;
	/** \brief Pageheight */
	double pageHeight;
	/* Number of Pages */
	// int pageCount; Disabled CR no longer required
	/** \brief Margins */
	MarginStruct pageMargins;
	int marginPreset;
	QList<PageSet> pageSets;
	MarginStruct bleeds;
// 	double BleedTop;
// 	double BleedLeft;
// 	double BleedRight;
// 	double BleedBottom;
	/** \brief Number of Columns */
	double PageSp;
	/** \brief Distance of Columns */
	double PageSpa;
	/** \brief current Pagelayout */
	int currentPageLayout;
	/** \brief Flag fuer Hoch- oder Querformat 0 = Hochformat */
	int PageOri;
	QString m_pageSize;
	/** \brief Erste Seitennummer im Dokument */
	int FirstPnum;
	/** \brief Flag fuer Rasterbenutzung */
	bool useRaster;
	/** \brief Im Dokument benutzte Farben */
	ColorList PageColors;
	/** \brief InfoStrings fuer das aktuelle Dokument */
	DocumentInformation documentInfo;
	int appMode;
	int SubMode;
	double *ShapeValues;
	int ValCount;
	QString DocName;
	QMap<QString,int> UsedFonts;
	SCFonts * const AllFonts;
	QList<AlignObjs> AObjects;
	QColor papColor;
	int CurrentSel;
	ParagraphStyle currentStyle;

	NodeEditContext nodeEdit;

	typoPrefs typographicSettings;
	guidesPrefs guidesSettings;
	toolPrefs toolSettings;
	QMap<QString, checkerPrefs> checkerProfiles;
	QString curCheckProfile;
	/** \brief Letztes Element fuer AutoTextrahmen */
	PageItem *LastAuto;
	/** \brief Erstes Element fuer AutoTextrahmen */
	PageItem *FirstAuto;
	bool DragP;
	bool leaveDrag;
	PageItem *DraggedElem;
	PageItem *ElemToLink;
	QList<uint> DragElements;
private:
	StyleSet<ParagraphStyle> docParagraphStyles;
	StyleSet<CharStyle> docCharStyles;
public:
	ScLayers Layers;
	bool marginColored;
	int GroupCounter;
	CMSData CMSSettings;
	cmsHPROFILE DocInputImageRGBProf;
	cmsHPROFILE DocInputImageCMYKProf;
	cmsHPROFILE DocInputRGBProf;
	cmsHPROFILE DocInputCMYKProf;
	cmsHPROFILE DocOutputProf;
	cmsHPROFILE DocPrinterProf;
	cmsHTRANSFORM stdTransRGBMon;
	cmsHTRANSFORM stdTransCMYKMon;
	cmsHTRANSFORM stdProof;
	cmsHTRANSFORM stdTransImg;
	cmsHTRANSFORM stdProofImg;
	cmsHTRANSFORM stdProofImgCMYK;
	cmsHTRANSFORM stdTransCMYK;
	cmsHTRANSFORM stdProofCMYK;
	cmsHTRANSFORM stdTransRGB;
	cmsHTRANSFORM stdProofGC;
	cmsHTRANSFORM stdProofCMYKGC;
	bool BlackPoint;
	bool SoftProofing;
	bool Gamut;
	int  IntentColors;
	int  IntentImages;
	bool HasCMS;
	QMap<QString,QString> JavaScripts;
	int TotalItems;
	int MinWordLen;
	int HyCount;
	QString Language;
	bool Automatic;
	bool AutoCheck;
	
	PDFOptions PDF_Options;
	PrintOptions Print_Options;
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
	QList<BookMa> BookMarks;
	bool OldBM;
	bool hasName;
	int RotMode;
	bool AutoSave;
	int AutoSaveTime;
	QTimer * const autoSaveTimer;
	QMap<QString,multiLine> MLineStyles;
	QList<ArrowDesc> arrowStyles;
	QMap<QString, ScPattern> docPatterns;
	QWidget* WinHan;
	bool DoDrawing;
	struct OpenNodesList
	{
		int type;
		Page *page;
		PageItem *item;
	};
	QList<OpenNodesList> OpenNodes;
	QTimer *CurTimer;
	QMap<int, errorCodes> docLayerErrors;
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
	
	Hyphenator * docHyphenator;
private:
	UndoTransaction* m_itemCreationTransaction;
	UndoTransaction* m_alignTransaction;

	Page* m_currentPage;
	UpdateManager m_updateManager;
	MassObservable<PageItem*> m_itemsChanged;
	MassObservable<Page*> m_pagesChanged;
	MassObservable<QRectF> m_regionsChanged;
	DocUpdater* m_docUpdater;
	
signals:
	//Lets make our doc talk to our GUI rather than confusing all our normal stuff
	/**
	 * @brief Let the document tell whatever is listening that it has changed
	 */
	void docChanged();
	void updateContents();
	void updateContents(const QRect &r);
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
	//! Temporary signal for SizeItem
	void widthAndHeight(double, double);
	
public slots:
	void itemSelection_ToggleLock();
	void itemSelection_ToggleSizeLock();
	void itemSelection_ToggleImageShown();
	void itemSelection_TogglePrintEnabled();
	void itemSelection_ChangePreviewResolution(int id);
	void itemSelection_ClearItem(Selection* customSelection=0);
	//! Delete the items in the current selection. When force is true, we do not warn the user and make SE happy too. Force is used from @sa Page::restorePageItemCreation
	void itemSelection_DeleteItem(Selection* customSelection=0, bool forceDeletion=false);
	void itemSelection_SetItemFillTransparency(double t);
	void itemSelection_SetItemLineTransparency(double t);
	void itemSelection_SetItemFillBlend(int t);
	void itemSelection_SetItemLineBlend(int t);
	void itemSelection_SetLineGradient(VGradient& newGradient, Selection* customSelection=0);
	void itemSelection_SetFillGradient(VGradient& newGradient, Selection* customSelection=0);
	void itemSelection_SetOverprint(bool overprint, Selection* customSelection=0);
	void itemSelection_ApplyImageEffects(ScImageEffectList& newEffectList, Selection* customSelection=0);
	void itemSelection_FlipH();
	void itemSelection_FlipV();
	void itemSelection_DoHyphenate();
	void itemSelection_DoDeHyphenate();
	void itemSelection_SendToLayer(int layerNumber);
	void itemSelection_SetImageOffset(double x, double y, Selection* customSelection=0);
	void itemSelection_SetImageScale(double x, double y, Selection* customSelection=0);
	void itemSelection_SetImageScaleAndOffset(double ox, double oy, double sx, double sy, Selection* customSelection=0);
	void itemSelection_AlignLeftOut(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignRightOut(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignBottomIn(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignRightIn(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignBottomOut(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignCenterHor(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignLeftIn(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignCenterVer(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignTopOut(AlignTo currAlignTo, double guidePosition);
	void itemSelection_AlignTopIn(AlignTo currAlignTo, double guidePosition);
	void itemSelection_DistributeDistH(bool usingDistance=false, double distance=0.0);
	void itemSelection_DistributeAcrossPage(bool useMargins=false);
	void itemSelection_DistributeRight();
	void itemSelection_DistributeBottom();
	void itemSelection_DistributeCenterH();
	void itemSelection_DistributeDistV(bool usingDistance=false, double distance=0.0);
	void itemSelection_DistributeDownPage(bool useMargins=false);
	void itemSelection_DistributeLeft();
	void itemSelection_DistributeCenterV();
	void itemSelection_DistributeTop();
	void itemSelection_MultipleDuplicate(ItemMultipleDuplicateData&);
	void itemSelection_UniteItems(Selection* customSelection=0);
	void itemSelection_SplitItems(Selection* customSelection=0);
	/**
	 * Adjust an image frame's size to fit the size of the image in it
	 */
	void itemSelection_adjustFrametoImageSize(Selection* customSelection=0);
	//! @brief startArrowID or endArrowID of -1 mean not applying a selection at this point.
	void itemSelection_ApplyArrowHead(int startArrowID=-1, int endArrowID=-1, Selection* customSelection=0);

	void ItemPen(QString farbe);
	void ItemPenShade(int sha);
	void ItemBrush(QString farbe);
	void ItemBrushShade(int sha);
	void ItemGradFill(int typ);
	void ItemPatternFill(QString pattern);
	void ItemPatternProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation);

	void updatePic();
	void updatePict(QString name);
	void removePict(QString name);
};

#endif
