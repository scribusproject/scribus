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
#include <QHash>
#include <QObject>
#include <QPixmap>
#include <QRectF>
#include <QStringList>
#include <QTimer>
#include <QFile>

#include "gtgettext.h" //CB For the ImportSetup struct and itemadduserframe
#include "scribusapi.h"
#include "colormgmt/sccolormgmtengine.h"
#include "documentinformation.h"
#include "numeration.h"
#include "marks.h"
#include "nodeeditcontext.h"
#include "notesstyles.h"
#include "observable.h"
#include "pageitem.h"
#include "pageitem_group.h"
#include "pageitem_latexframe.h"
#include "pageitem_textframe.h"
#include "pagestructs.h"
#include "prefsstructs.h"
#include "scguardedptr.h"
#include "scpage.h"
#include "sclayer.h"
#include "styles/styleset.h"
#include "styles/tablestyle.h"
#include "styles/cellstyle.h"
#include "undoobject.h"
#include "undostate.h"
#include "undotransaction.h"
#include "updatemanager.h"
#include "usertaskstructs.h"

class DocUpdater;
class FPoint;
class UndoManager;
// class UndoState;
class PDFOptions;
class Hyphenator;
class Selection;
class ScribusView;
class ScribusMainWindow;
class ResourceCollection;
class PageSize;
class ScPattern;
class Serializer;
class QProgressBar;
class MarksManager;
class NotesStyle;
class TextNote;



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
	bool inAnEditMode() const;
	bool inASpecialEditMode() const;
	QList<PageItem*> getAllItems(QList<PageItem*> &items);
	QList<PageItem*> *parentGroup(PageItem* item, QList<PageItem*> *list);
	void setup(const int, const int, const int, const int, const int, const QString&, const QString&);
	void setLoading(const bool);
	bool isLoading() const;
	void setModified(const bool);
	bool isModified() const;
/** Setzt die Seitenattribute */
	void setPage(double w, double h, double t, double l, double r, double b, double sp, double ab, bool atf, int fp);
	void resetPage(int fp, MarginStruct* newMargins=0);

	/**
	 * @brief Return the view associated with the document
	 */
	ScribusView* view() const;
	ScribusMainWindow* scMW() const {return m_ScMW;}
	void setGUI(bool hasgui, ScribusMainWindow* mw, ScribusView* view);
	void createHyphenator();

	/**
	 * @brief Return the guarded object associated with the document
	 */
	const ScGuardedPtr<ScribusDoc>& guardedPtr() const;
	
	UpdateManager* updateManager() { return &m_updateManager; }
	MassObservable<PageItem*> * itemsChanged() { return &m_itemsChanged; }
	MassObservable<ScPage*>     * pagesChanged() { return &m_pagesChanged; }
	MassObservable<QRectF>    * regionsChanged() { return &m_regionsChanged; }
	
	void invalidateAll();
	void invalidateLayer(int layerID);
	void invalidateRegion(QRectF region);


	MarginStruct* scratch() { return &m_docPrefsData.displayPrefs.scratch; }
	MarginStruct* bleeds() { return &m_docPrefsData.docSetupPrefs.bleeds; }
	MarginStruct& bleedsVal() { return m_docPrefsData.docSetupPrefs.bleeds; }
	MarginStruct* margins() { return &m_docPrefsData.docSetupPrefs.margins; }
	MarginStruct& marginsVal() { return m_docPrefsData.docSetupPrefs.margins; }
	double pageGapHorizontal() const { return m_docPrefsData.displayPrefs.pageGapHorizontal; }
	double pageGapVertical() const { return m_docPrefsData.displayPrefs.pageGapVertical; }
	void setPageGapHorizontal(double h) { m_docPrefsData.displayPrefs.pageGapHorizontal=h; }
	void setPageGapVertical(double v) { m_docPrefsData.displayPrefs.pageGapVertical=v; }
	const QList<PageSet>& pageSets() const { return m_docPrefsData.pageSets; }
	void setPageSetFirstPage(int layout, int fp);
	void clearPageSets() { m_docPrefsData.pageSets.clear(); }
	void appendToPageSets(const PageSet& ps) { m_docPrefsData.pageSets.append(ps); }
	void setPaperColor(const QColor &c) { m_docPrefsData.displayPrefs.paperColor=c; }
	const QColor& paperColor() const { return m_docPrefsData.displayPrefs.paperColor; }
	int hyphMinimumWordLength() const { return m_docPrefsData.hyphPrefs.MinWordLen; }
	int hyphConsecutiveLines() const { return m_docPrefsData.hyphPrefs.HyCount; }
	const QString& hyphLanguage() const { return m_docPrefsData.hyphPrefs.Language; }
	bool hyphAutomatic() const { return m_docPrefsData.hyphPrefs.Automatic; }
	bool hyphAutoCheck() const { return m_docPrefsData.hyphPrefs.AutoCheck; }
	void setHyphMinimumWordLength(int i) { m_docPrefsData.hyphPrefs.MinWordLen=i; }
	void setHyphConsecutiveLines(int i) { m_docPrefsData.hyphPrefs.HyCount=i; }
	void setHyphLanguage(const QString& s) { m_docPrefsData.hyphPrefs.Language=s; }
	void setHyphAutomatic(bool b) { m_docPrefsData.hyphPrefs.Automatic=b; }
	void setHyphAutoCheck(bool b) { m_docPrefsData.hyphPrefs.AutoCheck=b; }
	bool autoSave() const { return m_docPrefsData.docSetupPrefs.AutoSave; }
	int autoSaveTime() const  { return m_docPrefsData.docSetupPrefs.AutoSaveTime; }
	bool autoSaveClockDisplay() const  { return m_docPrefsData.displayPrefs.showAutosaveClockOnCanvas; }
	void setAutoSave(bool b) { m_docPrefsData.docSetupPrefs.AutoSave=b; }
	void setAutoSaveTime(int i) { m_docPrefsData.docSetupPrefs.AutoSaveTime=i; }
	//FIXME (maybe) :non const, the loaders make a mess here
	PDFOptions& pdfOptions() { return m_docPrefsData.pdfPrefs; }
	ObjAttrVector& itemAttributes() { return m_docPrefsData.itemAttrPrefs.defaultItemAttributes; }
	void setItemAttributes(ObjAttrVector& oav) { m_docPrefsData.itemAttrPrefs.defaultItemAttributes=oav;}
	void clearItemAttributes() { m_docPrefsData.itemAttrPrefs.defaultItemAttributes.clear(); }
	void appendToItemAttributes(const ObjectAttribute& oa) { m_docPrefsData.itemAttrPrefs.defaultItemAttributes.append(oa); }
	ToCSetupVector& tocSetups() { return m_docPrefsData.tocPrefs.defaultToCSetups; }
	void setTocSetups(ToCSetupVector& tsv) { m_docPrefsData.tocPrefs.defaultToCSetups=tsv; }
	void clearTocSetups() { m_docPrefsData.tocPrefs.defaultToCSetups.clear(); }
	void appendToTocSetups(const ToCSetup& ts) { m_docPrefsData.tocPrefs.defaultToCSetups.append(ts); }
	void setArrowStyles(QList<ArrowDesc>& as) { m_docPrefsData.arrowStyles=as; }
	QList<ArrowDesc>& arrowStyles() { return m_docPrefsData.arrowStyles; }
	void appendToArrowStyles(const struct ArrowDesc& as) { m_docPrefsData.arrowStyles.append(as); }
	const bool marginColored() const { return m_docPrefsData.displayPrefs.marginColored; }
	void setMarginColored(bool b) { m_docPrefsData.displayPrefs.marginColored=b; }
	QMap<QString, CheckerPrefs>& checkerProfiles() { return m_docPrefsData.verifierPrefs.checkerPrefsList; }
	void setCheckerProfiles(const QMap<QString, CheckerPrefs>& cl) { m_docPrefsData.verifierPrefs.checkerPrefsList=cl; }
	void set1CheckerProfile(const QString profileName, const struct CheckerPrefs& cs) { m_docPrefsData.verifierPrefs.checkerPrefsList[profileName] = cs; }
	void clearCheckerProfiles() { m_docPrefsData.verifierPrefs.checkerPrefsList.clear(); }
	const QString& curCheckProfile() const { return m_docPrefsData.verifierPrefs.curCheckProfile; }
	void setCurCheckProfile(const QString& s) { m_docPrefsData.verifierPrefs.curCheckProfile=s; }
	int pageOrientation() const { return m_docPrefsData.docSetupPrefs.pageOrientation; }
	void setPageOrientation(int o) { m_docPrefsData.docSetupPrefs.pageOrientation=o; }
	int pagePositioning() const { return m_docPrefsData.docSetupPrefs.pagePositioning; }
	void setPagePositioning(int p) { m_docPrefsData.docSetupPrefs.pagePositioning=p; }
	double pageHeight() const { return m_docPrefsData.docSetupPrefs.pageHeight; }
	double pageWidth() const { return m_docPrefsData.docSetupPrefs.pageWidth; }
	const QString& pageSize() const { return m_docPrefsData.docSetupPrefs.pageSize; }
	void setPageHeight(double h) { m_docPrefsData.docSetupPrefs.pageHeight=h; }
	void setPageWidth(double w) { m_docPrefsData.docSetupPrefs.pageWidth=w; }
	void setPageSize(const QString& s) { m_docPrefsData.docSetupPrefs.pageSize=s; }
	int marginPreset() const { return m_docPrefsData.docSetupPrefs.marginPreset; }
	void setMarginPreset(int mp) { m_docPrefsData.docSetupPrefs.marginPreset=mp; }

	TypoPrefs& typographicPrefs() { return m_docPrefsData.typoPrefs; }
	GuidesPrefs& guidesPrefs() { return m_docPrefsData.guidesPrefs; }
	ItemToolPrefs& itemToolPrefs() { return m_docPrefsData.itemToolPrefs; }
	OperatorToolPrefs& opToolPrefs() { return m_docPrefsData.opToolPrefs; }
	ColorPrefs& colorPrefs() { return m_docPrefsData.colorPrefs; }
	CMSData& cmsSettings() { return m_docPrefsData.colorPrefs.DCMSset; }
	DocumentInformation& documentInfo() { return m_docPrefsData.docInfo; }
	void setDocumentInfo(DocumentInformation di) { m_docPrefsData.docInfo=di; }
	DocumentSectionMap& sections() { return m_docPrefsData.docSectionMap; }
	void setSections(DocumentSectionMap dsm) { m_docPrefsData.docSectionMap=dsm; }
	const QMap<QString, int> & usedFonts() { return UsedFonts; }

	const ApplicationPrefs& prefsData() { return m_docPrefsData; }
	void setNewPrefs(const ApplicationPrefs& prefsData, const ApplicationPrefs& oldPrefsData, bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins);

	// Add, delete and move pages
	
	ScPage* addPage(const int pageNumber, const QString& masterPageName=QString::null, const bool addAutoFrame=false);
	void deletePage(const int);
	//! @brief Add a master page with this function, do not use addPage
	ScPage* addMasterPage(const int, const QString&);
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
	 * @brief Swap two pages
	 * @param a page index
	 * @param b page index
	 */
	void swapPage(const int a, const int b);
	/**
	 * @brief Move page(s) within the document
	 * @param fromPage page index
	 * @param toPage page index
	 * @param count target to move to (page index)
	 * @param position Before, After or at the end
	 */
	void movePage(const int fromPage, const int toPage, const int dest, const int position);
	
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
	 * @param layerIDToCopy source layer
	 * @param whereToInsert target layer
	 * @return Success or failure
	 */
	void copyLayer(int layerIDToCopy, int whereToInsert);
	/**
	 * @brief Delete a layer from the current document
	 * @param layerID of layer
	 * @param deleteItems the items on the layer too?
	 * @return Success or failure
	 */
	bool deleteLayer(const int layerID, const bool deleteItems);
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
	 * @param layerID ID of the layer
	 * @param isPrintable bool true = layer is prantable
	 * @return Success or failure
	 */
	bool setLayerPrintable(const int layerID, const bool isPrintable);
	/**
	 * @brief Is the layer printable
	 * @param layerID ID of the layer
	 * @return Printable or not
	 */
	bool layerPrintable(const int layerID);
	/**
	 * @brief Set the layer visible via the layer number
	 * @param layerID ID of the layer
	 * @param isViewable true = layer is visible
	 * @return Success or failure
	 */
	bool setLayerVisible(const int layerID, const bool isViewable);
	/**
	 * @brief Is the layer visible
	 * @param layerID ID of the layer
	 * @return Visible or not
	 */
	bool layerVisible(const int layerID);
	/**
	 * @brief are objects on the layer selectable
	 * @param layerID ID of the layer
	 * @return Items selectable or not
	 */
	bool layerSelectable(const int layerID);
	/**
	 * @brief Set objects on the layer selectable via the layer ID
	 * @param layerID ID of the layer
	 * @param isSelectable true = layer objects are selectable
	 * @return bool Success or failure
	 */
	bool setLayerSelectable(const int layerID, const bool isSelectable);
	/**
	 * @brief Set the layer locked via the layer number
	 * @param layerID ID of the layer
	 * @param isLocked true = layer is locked
	 * @return Success or failure
	 */
	bool setLayerLocked(const int layerID, const bool isLocked);
	/**
	 * @brief Is the layer locked
	 * @param layerID ID of the layer
	 * @return Locked or not
	 */
	bool layerLocked(const int layerID);
	/**
	 * @brief Set the layer flow via the layer number
	 * @param layerID ID of the layer
	 * @param flow true = Text flows around objects on this layer
	 * @return Success or failure
	 */
	bool setLayerFlow(const int layerID, const bool flow);
	/**
	 * @brief does text flow around objects on this layer
	 * @param layerID ID of the layer
	 * @return flow or not
	 */
	bool layerFlow(const int layerID);
	/**
	 * @brief Set the layer transparency via the layer number
	 * @param layerID ID of the layer
	 * @param trans transparency value 0.0 - 1.0
	 * @return Success or failure
	 */
	bool setLayerTransparency(const int layerID, double trans);
	/**
	 * @brief returns the layer transparency
	 * @param layerID ID of the layer
	 * @return transparency value 0.0 - 1.0
	 */
	double layerTransparency(const int layerID);
	/**
	 * @brief Set the layer layerBlendMode via the layer number
	 * @param layerID ID of the layer
	 * @param blend layerBlendMode
	 * @return Success or failure
	 */
	bool setLayerBlendMode(const int ID, int blend);
	/**
	 * @brief returns the layer BlendMode
	 * @param layerID ID of the layer
	 * @return layerBlendMode
	 */
	int layerBlendMode(const int ID);
	/**
	 * @brief Return the level of the requested layer
	 * @param layerID ID of the layer
	 * @return Level of the layer
	 */
	int layerLevelFromID(const int layerID);
	/**
	 * @brief Set the layer marker color
	 * @param ID Number of the layer
	 * @param color color of the marker
	 * @return Success or failure
	 */
	bool setLayerMarker(const int layerID, QColor color);
	/**
	 * @brief returns the layer marker color
	 * @param layerID ID of the layer
	 * @return marker color
	 */
	QColor layerMarker(const int layerID);
	/**
	 * @brief Set the layer outline mode via the layer number
	 * @param layerID ID of the layer
	 * @param outline true = layer is displayed in outlines only
	 * @return Success or failure
	 */
	bool setLayerOutline(const int layerID, const bool outline);
	/**
	 * @brief is this layer in outline mode
	 * @param layerID ID of the layer
	 * @return outline or not
	 */
	bool layerOutline(const int layerID);
	/**
	 * @brief Return the number of the layer at a certain level
	 * @param layerLevel Layer level
	 * @return Layer ID
	 */
	int layerIDFromLevel(const int layerLevel);
	/**
	 * @brief Return the layer count
	 * @return Number of layers in doc
	 */
	int layerIDFromName(QString name);
	int layerCount() const;
	/**
	 * @brief Lower a layer
	 * @param layerID ID of the layer
	 * @return Success or failure
	 */
	bool lowerLayer(const int layerID);
	/**
	 * @brief Lower a layer using the level
	 * @param layerLevel Level of the layer
	 * @return Success or failure
	 */
	bool lowerLayerByLevel(const int layerLevel);
	/**
	 * @brief Raise a layer
	 * @param layerID ID of the layer
	 * @return Success or failure
	 */
	bool raiseLayer(const int layerID);
	/**
	 * @brief Raise a layer using the level
	 * @param layerLevel Level of the layer
	 * @return Success or failure
	 */
	bool raiseLayerByLevel(const int layerLevel);
	/**
	 * @brief Return the layer name
	 * @param layerID ID of the layer
	 * @return Name of the layer
	 */
	QString layerName(const int layerID) const;
	/**
	 * @brief Change the name of a layer
	 * @param layerID ID of the layer
	 * @param newName new name of the layer
	 * @return Success or failure
	 */
	bool changeLayerName(const int layerID, const QString& newName);
	/**
	 * @brief Does the layer have items on it?
	 * @param layerID ID of the layer
	 * @return Layer contains items bool
	 */
	bool layerContainsItems(const int layerID);
	/**
	 * @brief Renumber a layer. Used in particular for reinsertion for undo/redo
	 * @param layerID old layer ID
	 * @param newLayerID New layer ID
	 * @return Success or failure
	 */
	bool renumberLayer(const int layerID, const int newLayerID);
	/**
	 * @brief Return a list of the layers in their order
	 * @param list QStringList to insert the layer names into
	 */
	void orderedLayerList(QStringList* list);

	int firstLayerID();
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
	
	const ParagraphStyle& paragraphStyle(QString name) { return m_docParagraphStyles.get(name); }
	const StyleSet<ParagraphStyle>& paragraphStyles()   { return m_docParagraphStyles; }
	bool isDefaultStyle( const ParagraphStyle& p ) const { return m_docParagraphStyles.isDefault(p); }
	bool isDefaultStyle( const CharStyle& c ) const { return m_docCharStyles.isDefault(c); }
// 	bool isDefaultStyle( LineStyle& l ) const { return MLineStyles......; }

	/**
	 * Returns the table style named @a name.
	 */
	const TableStyle& tableStyle(QString name) { return m_docTableStyles.get(name); }
	/**
	 * Returns the set of table styles in the document.
	 */
	const StyleSet<TableStyle>& tableStyles()   { return m_docTableStyles; }
	/**
	 * Returns <code>true</code> if @a style is the default table style.
	 */
	bool isDefaultStyle(const TableStyle& style) const { return m_docTableStyles.isDefault(style); }
	/**
	 * Redefines the set of table styles in the document using styles in @a newStyles.
	 * Removes unused table styles if @a removeUnused is <code>true</code>.
	 */
	void redefineTableStyles(const StyleSet<TableStyle>& newStyles, bool removeUnused = false);
	/**
	 * Remove any reference to old table styles and replace with new name.
	 * This needs to be called when a style was removed. New name may be "".
	 * @a newNameForOld is a map which maps the name of any style to remove
	 * to a new table style name
	 */
	void replaceTableStyles(const QMap<QString, QString>& newNameForOld);

	/**
	 * Returns the table cell style named @a name.
	 */
	const CellStyle& cellStyle(QString name) { return m_docCellStyles.get(name); }
	/**
	 * Returns the set of table cell styles in the document.
	 */
	const StyleSet<CellStyle>& cellStyles()   { return m_docCellStyles; }
	/**
	 * Returns <code>true</code> if @a style is the default table cell style.
	 */
	bool isDefaultStyle(const CellStyle& style) const { return m_docCellStyles.isDefault(style); }
	/**
	 * Redefines the set of table cell styles in the document using styles in @a newStyles.
	 * Removes unused table cell styles if @a removeUnused is <code>true</code>.
	 */
	void redefineCellStyles(const StyleSet<CellStyle>& newStyles, bool removeUnused = false);
	/**
	 * Remove any reference to old table cell styles and replace with new name.
	 * This needs to be called when a style was removed. New name may be "".
	 * @a newNameForOld is a map which maps the name of any style to remove
	 * to a new table cell style name
	 */
	void replaceCellStyles(const QMap<QString, QString>& newNameForOld);

	void getNamedResources(ResourceCollection& lists) const;
	struct ResMapped
	{
		ResMapped(ResourceCollection& newNames) { m_newNames = newNames;}

		void operator()(PageItem *item)
		{
			item->replaceNamedResources(m_newNames);
		}

		ResourceCollection m_newNames;
	};
	void replaceNamedResources(ResourceCollection& newNames);
	bool styleExists(QString styleName);
	
	QList<int> getSortedStyleList();
	QList<int> getSortedCharStyleList();
	QList<int> getSortedTableStyleList();
	QList<int> getSortedCellStyleList();
	
	void redefineStyles(const StyleSet<ParagraphStyle>& newStyles, bool removeUnused=false);
	/**
	 * @brief Remove any reference to old styles and replace with new name. This needs to be
	 *        called when a style was removed. New name may be "".
	 * @param newNameForOld a map which maps the name of any style to remove to a new stylename
	 */
	void replaceStyles(const QMap<QString,QString>& newNameForOld);
	/**
	 * @brief Insert styles from another document in this document.
	 *        
	 * @param fileName The path of the document we want to extract its styles
	 */
	void loadStylesFromFile(QString fileName);
	/**
	 * @brief Gather styles from another document.
	 *        
	 * @param fileName The path of the document we want to extract its styles
	 * @param tempStyles A pointer to a StyleSet which will be filled by paragraph styles
	 * @param tempCharStyles A pointer to a StyleSet which will be filled by character styles
	 * @param tempLineStyles A map which will be filled by line styles
	 */
	void loadStylesFromFile(QString fileName, StyleSet<ParagraphStyle> *tempStyles,
	                                          StyleSet<CharStyle> *tempCharStyles,
											  QHash<QString, multiLine> *tempLineStyles);

	const CharStyle& charStyle(QString name) { return m_docCharStyles.get(name); }
	const StyleSet<CharStyle>& charStyles()  { return m_docCharStyles; }
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
	QMap<QString,int> reorganiseFonts();
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
	* @brief Builds a qmap of the gradients used within the document
	*/
	void getUsedGradients(QHash<QString, VGradient> &Gradients);
	/*!
	* @brief Set the gradients for a document
	*/
	bool addGradient(QString &name, VGradient &gradient);
	void setGradients(QHash<QString, VGradient> &gradients);
	/*!
	* @brief Set the patterns for a document
	*/
	bool addPattern(QString &name, ScPattern& pattern);
	void removePattern(QString name);
	void setPatterns(QHash<QString, ScPattern> &patterns);
	/*!
	* @brief Check pattern with specified name and return it if valid
	*/
	ScPattern* checkedPattern(QString &name);
	/*!
	* @brief Builds a QStringList of the patterns used within the document
	*/
	QStringList getUsedPatterns();
	QStringList getUsedPatternsSelection(Selection* customSelection);
	QStringList getUsedPatternsHelper(QString pattern, QStringList &results);
	QStringList getPatternDependencyList(QStringList used);
	/*!
	* @brief Builds a QStringList of the symbols used within the document
	*/
	QStringList getUsedSymbols();
	QStringList getUsedSymbolsHelper(QString pattern, QStringList &results);
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
	void restoreMasterPageApplying(SimpleState* ss, bool isUndo);
	void restoreMasterPageRenaming(SimpleState* ss, bool isUndo);
	void restoreCopyPage(SimpleState* ss, bool isUndo);
	void restoreMovePage(SimpleState* ss, bool isUndo);
	void restoreSwapPage(SimpleState* ss, bool isUndo);
	void restoreAddMasterPage(SimpleState* ss, bool isUndo);
	void restoreChangePageProperties(SimpleState* ss, bool isUndo);
	/**
	 * @brief Undo function for grouping/ungrouping
	 */
	void restoreGrouping(SimpleState* ss, bool isUndo);
	/**
	 * @brief Undo function for level
	 */
	void restoreLevelDown(SimpleState* ss, bool isUndo);
	void restoreLevelBottom(SimpleState* ss, bool isUndo);
	void restoreGuideLock(SimpleState* ss, bool isUndo);
	/**
	 * @brief Save function
	 */
	bool save(const QString& fileName, QString* savedFile = NULL);
	/**
	 * @brief Set the page margins. Current code uses current page only, also provide a (currently, TODO) option for this.
	 */
	bool changePageProperties(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double Height, const double width, const int orientation, const QString& pageSize, const int marginPreset, const bool moveObjects, const int pageNumber=-1, const int pageType = 0);
	/**
	 * @brief Recalculate the colors after CMS settings change. Update the items in the doc accordingly.
	 */
	void recalculateColorsList(QList<PageItem *> *itemList);
	static void recalculateColorItem(PageItem *item);
	void recalculateColors();
	/**
	 * @brief Copies a normal page to be a master pages
	 */
	bool copyPageToMasterPage(const int, const int, const int, const QString&, bool);
	
	
	/**
	 * @brief Just create but don't add to items list and don't create undo record
	 */
	PageItem* createPageItem(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, double x, double y, double b, double h, double w, const QString& fill, const QString& outline);
	
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
	\param noteFrame optional (default false) indicates that noteframes should be created, not text frame
	*/
	int itemAdd(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double b, const double h, const double w, const QString& fill, const QString& outline, PageItem::ItemKind itemKind = PageItem::StandardItem);

	/** Add an item to the page based on the x/y position. Item will be fitted to the closest guides/margins */
	int itemAddArea(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double w, const QString& fill, const QString& outline, PageItem::ItemKind itemKind = PageItem::StandardItem);
	
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
	bool itemAddCommit(PageItem* item);
	
	/**
	 * @brief Finalise item creation. Simply split off code from itemAdd
	 * Only to be called from itemAdd()
	 */
	void itemAddDetails(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, PageItem* newItem);

	uint getItemNrfromUniqueID(uint unique);
	//return pointer to item
	PageItem* getItemFromName(QString name);
	//itemDelete
	//itemBlah...

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
	 * \brief Handle image with color profiles
	 * @param items list of page items to update
	 * @param Pr profile
	 * @param PrCMYK cmyk profile
	 * @param dia optional progress widget
	 */
	void RecalcPictures(QList<PageItem*>* items, ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia = 0);
	/**
	 *
	 * @brief Find the minX,MinY and maxX,maxY for the canvas required for the doc
	 */
	void canvasMinMax(FPoint&, FPoint&);
	
	int  OnPage(double x2, double  y2);
	int  OnPage(PageItem *currItem);
	void GroupOnPage(PageItem *currItem);

	void reformPages(bool moveObjects = true);
	/** @brief Refresh automatic guides once Margin struct has been properly configure by reformPages() */
	void refreshGuides();

	/** @brief Check and fix if needed PageItem OwnPage member */
	void fixItemPageOwner();
	/** @brief Fix paragraph styles */
	void fixParagraphStyles();
	/** @brief Fix notes styles */
	void fixNotesStyles();
	
	/**
	 * @brief Return the x or y offset for a page on the canvas
	 * @retval double containing the offset. Returns -1.0 if page not in Pages list (as -ve is not possible).
	 * Mostly saves bringing in extra includes into files that already have scribusdoc.h
	 */
	double getXOffsetForPage(const int);
	double getYOffsetForPage(const int);
	void getBleeds(int pageNumber, MarginStruct& bleedData);
	void getBleeds(const ScPage* page, MarginStruct& bleedData);
	void getBleeds(const ScPage* page, const MarginStruct& baseValues, MarginStruct& bleedData);
	
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
	 * @brief Set the doc into symbol edit mode
	 */
	void setSymbolEditMode(bool mode, QString symbolName = "");

	/*** Is the document in symbol edit mode? */
	bool symbolEditMode() const { return m_symbolEditMode; }
	
	/*** get the name of the symbol currently in editing */
	QString getEditedSymbol() const { return m_currentEditedSymbol; }
	/**
	 * @brief Set the doc into inline edit mode
	 */
	void setInlineEditMode(bool mode, int id = -1);

	/*** Is the document in symbol edit mode? */
	bool inlineEditMode() const { return m_inlineEditMode; }

	/**
	 * @brief Add a section to the document sections list
	 * Set number to -1 to add in the default section if the map is empty
	 */
	void addSection(const int number=0, const QString& name=QString::null, const uint fromindex=0, const uint toindex=0, const  NumFormat type=Type_1_2_3, const uint sectionstartindex=0, const bool reversed=false, const bool active=true, const QChar fillChar=QChar(), int fieldWidth=0);
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
	 * @brief Gets the page number fill character to be printed based on the section it is in.
	 * Returns QString::null on failure to find the pageIndex
	 */
	const QChar getSectionPageNumberFillCharForPageIndex(const uint) const;
	/**
	 * @brief Gets the page number fill character to be printed based on the section it is in.
	 * Returns QString::null on failure to find the pageIndex
	 */
	int getSectionPageNumberWidthForPageIndex(const uint) const;
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
	 * @param pageIndex page nr
	 * @brief Returns name of section where page is located
	 */
	QString getSectionNameForPageIndex(const uint pageIndex) const;

	//! @brief Some internal align tools
	typedef enum {alignFirst, alignLast, alignPage, alignMargins, alignGuide, alignSelection } AlignTo;
	typedef enum {alignByMoving, alignByResizing } AlignMethod;
	void buildAlignItemList(Selection* customSelection=0);
	bool startAlign(uint minObjects = 1);
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
	/**
	 * \brief Get the column of the page on the canvas, ie, left, middle, or right
	 * @param pageIndex Index of page to find location for
	 * @return int of 0,1,2,3
	 */
	int columnOfPage(int pageIndex) const;


	QList<PageItem*>* GroupOfItem(QList<PageItem*>* itemList, PageItem* item);
	PageItem* groupObjectsSelection(Selection* customSelection=0);
	PageItem* groupObjectsList(QList<PageItem*> &itemList);
	void groupObjectsToItem(PageItem* groupItem, QList<PageItem*> &itemList);
	PageItem * itemSelection_GroupObjects  (bool changeLock, bool lock, Selection* customSelection=0);
	void itemSelection_UnGroupObjects(Selection* customSelection=0);
	void addToGroup(PageItem* group, PageItem* item);
	void removeFromGroup(PageItem* item);
	void rescaleGroup(PageItem* group, double scale);
	void resizeGroupToContents(PageItem* group);
	void itemSelection_resizeGroupToContents(Selection *customSelection=0);
	void itemSelection_convertItemsTo(const PageItem::ItemType newType, Selection* restoredSelection=0, Selection* customSelection=0);
	void itemSelection_convertItemsToSymbol(QString& patternName);
	void itemSelection_ApplyParagraphStyle(const ParagraphStyle & newstyle, Selection* customSelection=0, bool rmDirectFormatting = false);
	void itemSelection_SetParagraphStyle(const ParagraphStyle & newstyle, Selection* customSelection=0);
	void itemSelection_ApplyCharStyle(const CharStyle & newstyle, Selection* customSelection=0, QString ETEA = "");
	void itemSelection_SetCharStyle(const CharStyle & newstyle, Selection* customSelection=0);
	void itemSelection_EraseParagraphStyle(Selection* customSelection=0);
	void itemSelection_EraseCharStyle(Selection* customSelection=0);

	void itemSelection_SetNamedParagraphStyle(const QString & name, Selection* customSelection=0);
	void itemSelection_SetNamedCharStyle(const QString & name, Selection* customSelection=0);
	void itemSelection_SetNamedLineStyle(const QString & name, Selection* customSelection=0);

	void itemSelection_SetSoftShadow(bool has, QString color, double dx, double dy, double radius, int shade, double opac, int blend, bool erase, bool objopa);

	void itemSelection_SetLineWidth(double w);
	void itemSelection_SetLineArt(Qt::PenStyle w);
	void itemSelection_SetLineJoin(Qt::PenJoinStyle w);
	void itemSelection_SetLineEnd(Qt::PenCapStyle w);
	void itemSelection_SetAlignment(int w, Selection* customSelection=0);
	void itemSelection_SetLineSpacing(double w, Selection* customSelection=0);
	void itemSelection_SetLineSpacingMode(int w, Selection* customSelection=0);
	void itemSetFont(const QString& newFont);
	void itemSelection_SetFont(QString fon, Selection* customSelection=0);
	void itemSelection_SetParBackgroundColor(QString farbe, Selection* customSelection=0);
	void itemSelection_SetParBackgroundShade(int sha, Selection* customSelection=0);
	void itemSelection_SetBackgroundColor(QString farbe, Selection* customSelection=0);
	void itemSelection_SetBackgroundShade(int sha, Selection* customSelection=0);
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
	void itemSelection_resetOpticalMargins(Selection* customSelection=0);
	void itemSelection_SetColorProfile(const QString& profileName, Selection* customSelection=0);
	void itemSelection_SetRenderIntent(int intentIndex, Selection* customSelection=0);
	void itemSelection_SetCompressionMethod(int cmIndex, Selection* customSelection=0);
	void itemSelection_SetCompressionQuality(int cqIndex, Selection* customSelection=0);
	void itemSelection_SetTracking(int us, Selection* customSelection=0);
	void itemSelection_SetFontSize(int size, Selection* customSelection=0);
	void MirrorPolyH(PageItem *currItem);
	void MirrorPolyV(PageItem *currItem);
	bool getItem(PageItem **currItem, int nr = -1);
	void setFrameRect();
	void setFrameRounded();
	void setFrameOval();

	void setRedrawBounding(PageItem *currItem);
	void adjustCanvas(FPoint minPos, FPoint maxPos, bool absolute = false);
	struct PicResMapped
	{
		PicResMapped(bool applyNewRes, int lowResType) { m_applyNewRes = applyNewRes; m_lowResType = lowResType;}

		void operator()(PageItem *item)
		{
			QList<PageItem*> allItems;
			if (item->isGroup())
				allItems = item->asGroupFrame()->getItemList();
			else
				allItems.append(item);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				item = allItems.at(ii);
				if (item->imageIsAvailable)
				{
					bool fho = item->imageFlippedH();
					bool fvo = item->imageFlippedV();
					if (m_applyNewRes)
						item->pixm.imgInfo.lowResType = m_lowResType;
					if (item->asLatexFrame())
						item->asLatexFrame()->rerunApplication(false);
					else
						item->loadImage(item->Pfile, true, -1, false);
					item->setImageFlippedH(fho);
					item->setImageFlippedV(fvo);
					item->AdjustPictScale();
				}
			}
			allItems.clear();
		}

		bool m_applyNewRes;
		int m_lowResType;
	};
	void recalcPicturesRes(bool applyNewRes = false);
	int previewQuality();
	void connectDocSignals();
	void disconnectDocSignals();
	void removeLayer(int l, bool dl = false); //FIXME: Make protected once scripter function no longer uses this directly
	/*! \brief We call changed() whenever the document needs to know it has been changed.
	 *  If the document is the primary document in a main window, it will signal to enable/disable
	 * certain operations.
	 */
	void changed();
	/*! \brief Get pointer to the current page
	\retval Page* current page object */
	ScPage* currentPage();
	/*! \brief Set new current page
	\param newPage New current page */
	void setCurrentPage(ScPage *newPage);
	bool hasGUI() const {return m_hasGUI;}
	/*! \brief Apply grid to a QPoint, from ScribusView */
	QPoint ApplyGrid(const QPoint& in);
	/*! \brief Apply grid to an FPoint, from ScribusView */
	FPoint ApplyGridF(const FPoint& in);
	/*! \brief Does this doc have any TOC setups and potentially a TOC to generate */
	bool hasTOCSetup() { return !m_docPrefsData.tocPrefs.defaultToCSetups.empty(); }
	//! \brief Get the closest guide to the given point
	void getClosestGuides(double xin, double yin, double *xout, double *yout, int *GxM, int *GyM, ScPage* refPage = NULL);
	//! \brief Get the closest border of another element to the given point
	void getClosestElementBorder(double xin, double yin, double *xout, double *yout, int *GxM, int *GyM, ScPage* refPage = NULL);
	//! \brief Snap an item to the guides
	void SnapToGuides(PageItem *currItem);
	bool ApplyGuides(double *x, double *y, bool elementSnap = false);
	bool ApplyGuides(FPoint* point, bool elementSnap = false);
	bool moveItem(double newX, double newY, PageItem* ite);
	void rotateItem(double win, PageItem *currItem);
	void moveRotated(PageItem *currItem, FPoint npv);
	bool sizeItem(double newX, double newY, PageItem *pi, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	bool moveSizeItem(FPoint newX, FPoint newY, PageItem* currItem, bool fromMP = false, bool constrainRotation = false);
	void adjustItemSize(PageItem *currItem, bool includeGroup = false, bool moveInGroup = true);
	void moveGroup(double x, double y, Selection* customSelection = 0);
	void rotateGroup(double angle, Selection* customSelection = 0);
	void rotateGroup(double angle, FPoint RCenter, Selection* customSelection = 0);
	void scaleGroup(double scx, double scy, bool scaleText=true, Selection* customSelection = 0, bool scaleLine = false);
	//! \brief Get a list of frames of certain type
	QMap<PageItem*, QString> getDocItemNames(PageItem::ItemType itemType);
	//! \brief Returns a serializer for this document
	Serializer *serializer();
	//! \brief Returns a text serializer for this document, used to paste text chunks
	Serializer *textSerializer();

	//! \brief Get rotation mode
	int RotMode() const {return m_rotMode;}
	//! \brief Set rotation mode
	void RotMode(const int& val);

	//! \brief Fonctions which avoid doc updater and update manager to send too much
	// unncessary signals when doing updates on multiple items
	void beginUpdate();
	void endUpdate();
	int addToInlineFrames(PageItem *item);
	void removeInlineFrame(int fIndex);
	void checkItemForFrames(PageItem *it, int fIndex);
	bool hasPreflightErrors();
	QFileDevice::Permissions filePermissions() { return m_docFilePermissions; }
	void saveFilePermissions(QFileDevice::Permissions p) { m_docFilePermissions=p; }

protected:
	void addSymbols();
	void applyPrefsPageSizingAndMargins(bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins);
	bool m_hasGUI;
	QFileDevice::Permissions m_docFilePermissions;
	ApplicationPrefs& m_appPrefsData;
	ApplicationPrefs m_docPrefsData;
	UndoManager * const m_undoManager;
	bool m_loading;
	bool m_modified;
	int m_ActiveLayer;
	double m_docUnitRatio;
	int m_rotMode;
	bool m_automaticTextFrames; // Flag for automatic Textframes
	bool m_masterPageMode;
	bool m_symbolEditMode;
	bool m_inlineEditMode;
	int  m_storedLayerID;
	bool m_storedLayerLock;
	bool m_storedLayerVis;
	QMap<QString, double> m_constants;
	ScribusMainWindow* m_ScMW;
	ScribusView* m_View;
	ScGuardedObject<ScribusDoc> m_guardedObject;
	Serializer *m_serializer, *m_tserializer;
	QString m_currentEditedSymbol;
	int m_currentEditedIFrame;

public: // Public attributes
	bool is12doc; //public for now, it will be removed later
	int NrItems;
	int First;
	int Last;
	int viewCount;
	int viewID;
	bool SnapGrid;
	bool SnapGuides;
	bool SnapElement;
	bool GuideLock;
	bool dontResize;
	/** \brief Minimum and Maximum Points of Document */
	FPoint minCanvasCoordinate;
	FPoint maxCanvasCoordinate;
	FPoint stored_minCanvasCoordinate;
	FPoint stored_maxCanvasCoordinate;
	double rulerXoffset;
	double rulerYoffset;
	/** \brief List of Pages */
	QList<ScPage*>* Pages;
	/** \brief List of Master Pages */
	QList<ScPage*> MasterPages;
	/** \brief List of Document Pages */
	QList<ScPage*> DocPages;
	/** \brief List for temporary Pages */
	QList<ScPage*> TempPages;
	/** \brief Mapping Master Page Name to Master Page numbers */
	QMap<QString,int> MasterNames;
	/** \brief List of Objects */
	QList<PageItem*>* Items;
	QList<PageItem*> MasterItems;
	QList<PageItem*> DocItems;
	QHash<int, PageItem*> FrameItems;
	QList<PageItem*> EditFrameItems;
	PageItem *currentEditedTextframe;
	Selection* const m_Selection;
	/** \brief Number of Columns */
	double PageSp;
	/** \brief Distance of Columns */
	double PageSpa;
	///** \brief current Pagelayout */
	//int currentPageLayout;
	/** \brief Erste Seitennummer im Dokument */
	int FirstPnum;
	/** \brief Im Dokument benutzte Farben */
	ColorList PageColors;
	int appMode;
	int SubMode;
	double *ShapeValues;
	int ValCount;
	QString DocName;
	QMap<QString,int> UsedFonts;
	SCFonts * const AllFonts;
	QList<AlignObjs> AObjects;
	int CurrentSel;
	ParagraphStyle currentStyle;
	NodeEditContext nodeEdit;
	/** \brief Letztes Element fuer AutoTextrahmen */
	PageItem *LastAuto;
	/** \brief Erstes Element fuer AutoTextrahmen */
	PageItem *FirstAuto;
	bool DragP;
	bool leaveDrag;
	PageItem *DraggedElem;
	PageItem *ElemToLink;
	QList<PageItem*> DragElements;
private:
	StyleSet<ParagraphStyle> m_docParagraphStyles;
	StyleSet<CharStyle> m_docCharStyles;
	StyleSet<TableStyle> m_docTableStyles;
	StyleSet<CellStyle> m_docCellStyles;
public:
	ScLayers Layers;
	//bool marginColored;
	int GroupCounter;

	ScColorMgmtEngine colorEngine;
	ScColorProfile DocInputImageRGBProf;
	ScColorProfile DocInputImageCMYKProf;
	ScColorProfile DocInputRGBProf;
	ScColorProfile DocInputCMYKProf;
	ScColorProfile DocDisplayProf;
	ScColorProfile DocPrinterProf;
	ScColorTransform stdTransRGBMon;
	ScColorTransform stdTransCMYKMon;
	ScColorTransform stdProof;
	ScColorTransform stdTransImg;
	ScColorTransform stdProofImg;
	ScColorTransform stdProofImgCMYK;
	ScColorTransform stdTransCMYK;
	ScColorTransform stdProofCMYK;
	ScColorTransform stdTransRGB;
	ScColorTransform stdProofGC;
	ScColorTransform stdProofCMYKGC;
	ScColorTransform stdLabToRGBTrans;
	ScColorTransform stdLabToCMYKTrans;
	ScColorTransform stdProofLab;
	ScColorTransform stdProofLabGC;
	bool BlackPoint;
	bool SoftProofing;
	bool Gamut;
	eRenderIntent IntentColors;
	eRenderIntent IntentImages;
	bool HasCMS;
	QMap<QString,QString> JavaScripts;
	int TotalItems;
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
	bool isConverted;
	QTimer * const autoSaveTimer;
	QHash<QString,multiLine> MLineStyles;
	QHash<QString, ScPattern> docPatterns;
	QHash<QString, VGradient> docGradients;
	QWidget* WinHan;
	bool DoDrawing;
	bool drawAsPreview;
	bool viewAsPreview;
	bool editOnPreview;
	int previewVisual;
	struct OpenNodesList
	{
		int type;
		ScPage *page;
		PageItem *item;
	};
	QList<OpenNodesList> OpenNodes;
	QTimer *CurTimer;
	QMap<int, errorCodes> pageErrors;
	QMap<int, errorCodes> docLayerErrors;
	QMap<PageItem*, errorCodes> docItemErrors;
	QMap<PageItem*, errorCodes> masterItemErrors;
	FPointArray symReturn;
	FPointArray symNewLine;
	FPointArray symTab;
	FPointArray symNonBreak;
	FPointArray symNewCol;
	FPointArray symNewFrame;
	
	Hyphenator * docHyphenator;
	void itemResizeToMargin(PageItem* item, int direction); //direction reflect enum numbers from Canvas::FrameHandle

private:
	UndoTransaction m_itemCreationTransaction;
	UndoTransaction m_alignTransaction;

	ScPage* m_currentPage;
	UpdateManager m_updateManager;
	MassObservable<PageItem*> m_itemsChanged;
	MassObservable<ScPage*> m_pagesChanged;
	MassObservable<QRectF> m_regionsChanged;
	DocUpdater* m_docUpdater;
	
signals:
	//Lets make our doc talk to our GUI rather than confusing all our normal stuff
	/**
	 * @brief Let the document tell whatever is listening that it has changed
	 */
	void docChanged();
	void saved(QString name);
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

	//! Signal a change in rotation mode (aka basepoint)
	void rotationMode(int);
	void updateEditItem();
	void updateAutoSaveClock();
	void addBookmark(PageItem *);
	void deleteBookmark(PageItem *);
	void changeLayers(int);
	
public slots:
	void selectionChanged();
	void itemSelection_ToggleLock();
	void itemSelection_ToggleSizeLock();
	void itemSelection_ToggleImageShown();
	void itemSelection_TogglePrintEnabled();
	void itemSelection_ToggleBookMark(Selection* customSelection=0);
	void itemSelection_ToggleAnnotation(Selection* customSelection=0);
	void itemSelection_Transform(int nrOfCopies, QTransform matrix, int basepoint);
	void itemSelection_ChangePreviewResolution(int id);

	/*! \brief Change display quality of all images in document.
	\author  OssiLehtinen
	*/
	void allItems_ChangePreviewResolution(int id);

	//FIXME : change to process a selection
	void item_setFrameShape(PageItem* item, int frameType, int count, double* points); 

	void itemSelection_ClearItem(Selection* customSelection=0, bool useWarning=false);
	void itemSelection_TruncateItem(Selection* customSelection=0);
	//! Delete the items in the current selection. When force is true, we do not warn the user and make SE happy too. Force is used from @sa Page::restorePageItemCreation
	void itemSelection_DeleteItem(Selection* customSelection=0, bool forceDeletion=false);
	void itemSelection_SetItemTextReversed(bool reversed, Selection* customSelection=0);
	void itemSelection_SetItemFillTransparency(double t);
	void itemSelection_SetItemLineTransparency(double t);
	void itemSelection_SetItemFillBlend(int t);
	void itemSelection_SetItemLineBlend(int t);
	void itemSelection_SetLineGradient(VGradient& newGradient, Selection* customSelection=0);
	void itemSelection_SetFillGradient(VGradient& newGradient, Selection* customSelection=0);
	void itemSelection_SetMaskGradient(VGradient& newGradient, Selection* customSelection=0);
	void itemSelection_SetOverprint(bool overprint, Selection* customSelection=0);
	void itemSelection_ApplyImageEffects(ScImageEffectList& newEffectList, Selection* customSelection=0);
	void itemSelection_FlipH();
	void itemSelection_FlipV();
	void itemSelection_Rotate(double angle, Selection* customSelection = 0);
	void itemSelection_DoHyphenate();
	void itemSelection_DoDeHyphenate();
	void itemSelection_UnlinkTextFrameWithText(Selection *customSelection=0, bool cutText=false);
	void itemSelection_UnlinkTextFrameWithTextCut(Selection *customSelection=0);
	void itemSelection_SendToLayer(int layerID);
	void itemSelection_SetImageOffset(double x, double y, Selection* customSelection=0);
	void itemSelection_SetImageScale(double x, double y, Selection* customSelection=0);
	void itemSelection_SetImageScaleAndOffset(double ox, double oy, double sx, double sy, Selection* customSelection=0);
	void itemSelection_SetImageRotation(double rot, Selection* customSelection=0);
	void itemSelection_AlignItemLeft(int i, double newX, AlignMethod how);
	void itemSelection_AlignItemRight(int i, double newX, AlignMethod how);
	void itemSelection_AlignItemTop(int i, double newY, AlignMethod how);
	void itemSelection_AlignItemBottom(int i, double newY, AlignMethod how);
	void itemSelection_AlignLeftOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignRightOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignBottomIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignRightIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignBottomOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignCenterHor(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignLeftIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignCenterVer(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignTopOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignTopIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition);

	void sendItemSelectionToBack();
	void bringItemSelectionToFront();
	void itemSelection_LowerItem();
	void itemSelection_RaiseItem();
	void itemSelection_DistributeDistH(bool usingDistance=false, double distance=0.0, bool reverseDistribute=false);
	void itemSelection_DistributeAcrossPage(bool useMargins=false);
	void itemSelection_DistributeRight();
	void itemSelection_DistributeBottom();
	void itemSelection_DistributeCenterH();
	void itemSelection_DistributeDistV(bool usingDistance=false, double distance=0.0, bool reverseDistribute=false);
	void itemSelection_DistributeDownPage(bool useMargins=false);
	void itemSelection_DistributeLeft();
	void itemSelection_DistributeCenterV();
	void itemSelection_DistributeTop();
	void itemSelection_SwapLeft();
	void itemSelection_SwapRight();
	void itemSelection_MultipleDuplicate(ItemMultipleDuplicateData&);
	void itemSelection_UniteItems(Selection* customSelection=0);
	void itemSelection_SplitItems(Selection* customSelection=0);
	/**
	 * Adjust an image frame's size to fit the size of the image in it
	 */
	void itemSelection_AdjustFrametoImageSize(Selection* customSelection=0);
	/**
	 * Adjust an image size to fit the size of the frame
	 */
	void itemSelection_AdjustImagetoFrameSize(Selection* customSelection=0);
	void itemSelection_AdjustFrameHeightToText( Selection *customSelection=0);
	//! @brief startArrowID or endArrowID of -1 mean not applying a selection at this point.
	void itemSelection_ApplyArrowHead(int startArrowID=-1, int endArrowID=-1, Selection* customSelection=0);
	void itemSelection_ApplyArrowScale(int startArrowSc, int endArrowSc, Selection* customSelection);

	void itemSelection_SetItemPen(QString farbe);
	void itemSelection_SetItemPenShade(int sha);
	void itemSelection_SetItemGradStroke(int typ);
	void itemSelection_SetItemBrush(QString farbe);
	void itemSelection_SetItemBrushShade(int sha);
	void itemSelection_SetItemGradMask(int typ);
	void itemSelection_SetItemGradFill(int typ);
	void itemSelection_SetItemPatternFill(QString pattern);
	void itemSelection_SetItemPatternProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);
	void itemSelection_SetItemStrokePattern(QString pattern);
	void itemSelection_SetItemStrokePatternProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, double space, bool mirrorX, bool mirrorY);
	void itemSelection_SetItemStrokePatternType(bool type);
	void itemSelection_SetItemPatternMask(QString pattern);
	void itemSelection_SetItemPatternMaskProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);

	// Table related slots.

	/**
	 * Inserts rows in a table.
	 *
	 * The user will be prompted by a dialog for the number of rows and where they should
	 * be inserted. If in table edit mode, rows may be inserted before or after the active
	 * cell. If in normal mode, rows may be inserted at the beginning or end of the selected
	 * table.
	 *
	 * If no table is selected, then this slot does nothing.
	 */
	void itemSelection_InsertTableRows();

	/**
	 * Inserts columns in a table.
	 *
	 * The user will be prompted by a dialog for the number of columns and where they should
	 * be inserted. If in table edit mode, columns may be inserted before or after the active
	 * cell. If in normal mode, columns may be inserted at the beginning or end of the selected
	 * table.
	 *
	 * If no table is selected, then this slot does nothing.
	 */
	void itemSelection_InsertTableColumns();

	/**
	 * Deletes rows in a table.
	 *
	 * If the there is a cell selection, all rows spanned by the selection will be deleted.
	 * If there is no cell selection, the rows spanned by the active cell will be deleted.
	 *
	 * If no table is selected, or if the application is not in table edit mode, then this slot
	 * does nothing.
	 */
	void itemSelection_DeleteTableRows();

	/**
	 * Deletes columns in a table.
	 *
	 * If the there is a cell selection, all columns spanned by the selection will be deleted.
	 * If there is no cell selection, the columns spanned by the active cell will be deleted.
	 *
	 * If no table is selected, or if the application is not in table edit mode, then this slot
	 * does nothing.
	 */
	void itemSelection_DeleteTableColumns();

	/**
	 * Merges the selected cells in a table.
	 *
	 * The merged area will span from the top left to the bottom right of the selected cells.
	 *
	 * If no table is selected, or if the application is not in table edit mode, or if less
	 * than two cells is selected, then this slot does nothing.
	 */
	void itemSelection_MergeTableCells();

	/**
	 * Splits cells in a table.
	 *
	 * TODO: Implement this.
	 */
	void itemSelection_SplitTableCells() {}

	/**
	 * Sets the height of rows in a table.
	 *
	 * The user will be prompted by a dialog for entering a row height. If in table editing
	 * mode, all rows of the table will get their height set. If the there is a cell selection,
	 * all rows spanned by the selection will get their height set. If there is no cell
	 * selection, the rows spanned by the active cell will get their height set.
	 *
	 * If no table is selected, then this slot does nothing.
	 */
	void itemSelection_SetTableRowHeights();

	/**
	 * Sets the width of columns in a table.
	 *
	 * The user will be prompted by a dialog for entering a column width. If in table editing
	 * mode, all columns of the table will get their width set. If the there is a cell selection,
	 * all columns spanned by the selection will get their width set. If there is no cell
	 * selection, the columns spanned by the active cell will get their width set.
	 *
	 * If no table is selected, then this slot does nothing.
	 */
	void itemSelection_SetTableColumnWidths();

	/**
	 * Distributes rows in a table evenly.
	 *
	 * If in table edit mode and there is a cell selection, each contigous range of selected rows
	 * is distributed. If there is no cell selection, all rows in the table are distributed.
	 *
	 * If there is no table selected, then this slot does nothing.
	 */
	void itemSelection_DistributeTableRowsEvenly();

	/**
	 * Distributes columns in a table evenly.
	 *
	 * If in table edit mode and there is a cell selection, each contigous range of selected columns
	 * is distributed. If there is no cell selection, all columns in the table are distributed.
	 *
	 * If there is no table selected, then this slot does nothing.
	 */
	void itemSelection_DistributeTableColumnsEvenly();

	/**
	 * Adjusts the size of the frames of any selected tables to fit the size of the tables they contain.
	 *
	 * If there are no tables in the current selection, then this slot does nothing.
	 */
	void itemSelection_AdjustFrameToTable();

	/**
	 * Adjusts the size of any selected tables to fit the size of their frames.
	 *
	 * If there are no tables in the current selection, then this slot does nothing.
	 */
	void itemSelection_AdjustTableToFrame();

	void undoRedoBegin();
	void undoRedoDone();

	void updatePic();
	void updatePict(QString name);
	void updatePictDir(QString name);
	void removePict(QString name);

// Marks and notes
public:
	/**
	 * Explanation
	 * master frame - text frame with marks for notes
	 * notesframe - frame with notes
	 * master & note mark - master is mark in "master" text, note mark is at beginning of note in noteframe
	 */
	
	//return page where endnotesframe should be located depending of notes style range and location of master mark
	const ScPage* page4EndNotes(NotesStyle* NS, PageItem* item);

	//data handling structures
private:
	QList<Mark*> m_docMarksList;
	QList<TextNote*> m_docNotesList;
	//flags used for indicating needs of updates
	bool m_flag_notesChanged;

public:
	const QList<Mark*> marksList() { return m_docMarksList; }
	const QList<TextNote*> notesList() { return m_docNotesList; }
	QList<NotesStyle*> m_docNotesStylesList;
	QMap<PageItem_NoteFrame*, rangeItem> m_docEndNotesFramesMap;
	QList<NotesStyle*> ns2Update; //list of notes styles to update

	//returns list of notesframes for given Notes Style
	QList<PageItem_NoteFrame*> listNotesFrames(NotesStyle* NS);

	//flags used for indicating needs of updates
	bool notesChanged() { return m_flag_notesChanged; }
	void setNotesChanged(bool on) { m_flag_notesChanged = on; }
	bool flag_restartMarksRenumbering;
	bool flag_updateMarksLabels;
	bool flag_updateEndNotes;
	bool flag_layoutNotesFrames;

	//returns list of marks labels for given mark type
	QStringList marksLabelsList(MarkType type);

	//return mark with given label and given type
	Mark* getMark(QString label, MarkType type); //returns mark with label and type (labels are unique only for same type marks)
	Mark* newMark(Mark* mrk = NULL);
	TextNote* newNote(NotesStyle* NS);
	
	bool isMarkUsed(Mark* mrk, bool visible = false);
	//set cursor in text where given mark will be found
	void setCursor2MarkPos(Mark* mark);
	//return false if mark was not found
	bool eraseMark(Mark* mrk, bool fromText=false, PageItem* item=NULL, bool force = false); //force is used only for deleting non-unique marks by MarksManager
	void setUndoDelMark(Mark* mrk);
	//invalidate all text frames where given mark will found
	//useful spacially for varaible text marks after changing its text definition
	//if forceUpdate then found master frames are relayouted
	bool invalidateVariableTextFrames(Mark* mrk, bool forceUpdate = false); //returns if any text was changed

	//for foot/endnotes
	NotesStyle* newNotesStyle(NotesStyle NS);
	void renameNotesStyle(NotesStyle* NS, QString newName);
	//delete whole notes style with its notesframes and notes
	void deleteNotesStyle(QString nsName);
	void undoSetNotesStyle(SimpleState* ss, NotesStyle* ns);
	NotesStyle* getNotesStyle(QString nsName);
	//delete note, if fromText than marks for given note will be removed
	void deleteNote(TextNote* note);
	void setUndoDelNote(TextNote* note);
	PageItem_NoteFrame* createNoteFrame(PageItem_TextFrame* inFrame, NotesStyle *nStyle, int index = -1);
	PageItem_NoteFrame* createNoteFrame(NotesStyle *nStyle, double x, double y, double w, double h, double w2, QString fill, QString outline);
	//delete noteframe
	void delNoteFrame(PageItem_NoteFrame *nF, bool removeMarks=true, bool forceDeletion = true);
	//renumber notes for given notes style
	//return true if doc needs update after changing numbers of notes
	bool updateNotesNums(NotesStyle* nStyle);
	//set new text styles for notes marks
	void updateNotesFramesStyles(NotesStyle* nStyle);
	//check conflicts beetween notes styles
	bool validateNSet(NotesStyle NS, QString newName = "");
	//update layout remove empty notesframes
	bool notesFramesUpdate();
	//update notesframes after changing automatic features of notes style
	void updateNotesFramesSettings(NotesStyle* NS);

	//search for endnotesframe for given notes style and item holding master mark
	PageItem_NoteFrame* endNoteFrame(NotesStyle* nStyle, PageItem_TextFrame* master);
	//
	void setEndNoteFrame(PageItem_NoteFrame* nF, void* ptr)   { rangeItem rI={ptr}; m_docEndNotesFramesMap.insert(nF,rI); }
	void setEndNoteFrame(PageItem_NoteFrame* nF, int section)   { rangeItem rI; rI.sectionIndex = section; m_docEndNotesFramesMap.insert(nF, rI); }
	//update all endnotesframes content for given notes style
	void updateEndnotesFrames(NotesStyle* nStyle = NULL, bool invalidate = false);
	//update endnotesframe content
	void updateEndNotesFrameContent(PageItem_NoteFrame* nF, bool invalidate = false);
	//insert noteframe into list of changed
	void endNoteFrameChanged(PageItem_NoteFrame* nF) { m_docEndNotesFramesChanged.append(nF); }
	//update content for changed endnotesframes
	void updateChangedEndNotesFrames();
	//finds mark position in text
	//return true if mark was found, CPos is set for mark`s position
	//if item==NULL then search in all items and if mark is found than item is set
	int findMarkCPos(Mark* mrk, PageItem* &item, int Start = 0);
	QList<PageItem_NoteFrame*> m_docEndNotesFramesChanged;

	//finds item which holds given mark, start searching from next to lastItem index in DocItems
	PageItem* findMarkItem(Mark* mrk, int &lastItem);
	
private:
	//QMap<PageItem_NoteFrame*, QList<TextNote *> > map of notesframes and its list of notes
	NotesInFrameMap m_docNotesInFrameMap;

	PageItem* findFirstMarkItem(Mark* mrk) { int tmp = -1; return findMarkItem(mrk, tmp); }

	//search for endnotesframe for given notes style and item holding master mark or section number
	PageItem_NoteFrame* endNoteFrame(NotesStyle* nStyle, void* item = NULL);
	PageItem_NoteFrame* endNoteFrame(NotesStyle* nStyle, int sectIndex);
	//clear list of notes for given notesframe
	void clearNotesInFrameList(PageItem_NoteFrame* nF) { m_docNotesInFrameMap.insert(nF, QList<TextNote*>()); }
	//renumber notes with given notes style for given frame starting from number num
	void updateItemNotesNums(PageItem_TextFrame *frame, NotesStyle* nStyle, int &num);
	//update notesframes text styles
	void updateItemNotesFramesStyles(PageItem *item, const ParagraphStyle& newStyle);
	
	//not used?
	bool updateEndNotesNums(); //return true if doc needs update
	void invalidateNoteFrames(NotesStyle* nStyle);
	void invalidateMasterFrames(NotesStyle* nStyle);

public slots:
	//update strings (page numbers) for marks
	bool updateMarks(bool updateNotesMarks = false);

//welding two items
public slots:
	void itemSelection_UnWeld();
	void itemSelection_Weld();
	void itemSelection_EditWeld();
	void restartAutoSaveTimer();

protected slots:
	void slotAutoSave();

//auto-numerations
public:
	QMap<QString, NumStruct*> numerations;
	QStringList orgNumNames; //orgNumerations keeps original settings read from paragraph styles for reset settings overrided localy
	void setupNumerations(); //read styles for used auto-numerations, initialize numCounters
	QString getNumberStr(QString numName, int level, bool reset, ParagraphStyle &style);
	void setNumerationCounter(QString numName, int level, int number);
	bool flag_Renumber;
	bool flag_NumUpdateRequest;
	// for local numeration of paragraphs
	bool updateLocalNums(StoryText& itemText); //return true if any num strings were updated and item need s invalidation
	void updateNumbers(bool updateNumerations = false);
	void itemSelection_ClearBulNumStrings(Selection *customSelection);
/* Functions for PDF Form Actions */

public:
	void SubmitForm();
	void ImportData();
	void ResetFormFields();

};

Q_DECLARE_METATYPE(ScribusDoc*);

#endif
