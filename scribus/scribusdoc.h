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

#include "appmodes.h"
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
	QList<PageItem*> getAllItems(QList<PageItem*> &items) const;
	QList<PageItem*> *parentGroup(PageItem* item, QList<PageItem*> *list);
	void setup(int, int, int, int, int, const QString&, const QString&);
	void setLoading(bool);
	bool isLoading() const;
	void setModified(bool);
	bool isModified() const;
	bool isUndoRedoOngoing() const;
/** Setzt die Seitenattribute */
	void setPage(double w, double h, double t, double l, double r, double b, double sp, double ab, bool atf, int fp);

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
	MassObservable<PageItem*>* itemsChanged() { return &m_itemsChanged; }
	MassObservable<ScPage*>* pagesChanged() { return &m_pagesChanged; }
	MassObservable<QRectF>* regionsChanged() { return &m_regionsChanged; }
	
	void invalidateAll();
	void invalidateLayer(int layerID);
	void invalidateRegion(QRectF region);

	MarginStruct* scratch() { return &m_docPrefsData.displayPrefs.scratch; }
	MarginStruct* bleeds() { return &m_docPrefsData.docSetupPrefs.bleeds; }
	MarginStruct& bleedsVal() { return m_docPrefsData.docSetupPrefs.bleeds; }
	void setBleeds(MarginStruct& newBleeds) { m_docPrefsData.docSetupPrefs.bleeds = newBleeds; }
	MarginStruct* margins() { return &m_docPrefsData.docSetupPrefs.margins; }
	MarginStruct& marginsVal() { return m_docPrefsData.docSetupPrefs.margins; }
	void setMargins(MarginStruct& newMargins) { m_docPrefsData.docSetupPrefs.margins = newMargins; }
	double pageGapHorizontal() const { return m_docPrefsData.displayPrefs.pageGapHorizontal; }
	double pageGapVertical() const { return m_docPrefsData.displayPrefs.pageGapVertical; }
	void setPageGapHorizontal(double h) { m_docPrefsData.displayPrefs.pageGapHorizontal = h; }
	void setPageGapVertical(double v) { m_docPrefsData.displayPrefs.pageGapVertical = v; }
	const QList<PageSet>& pageSets() const { return m_docPrefsData.pageSets; }
	void setPageSetFirstPage(int layout, int fp);
	void clearPageSets() { m_docPrefsData.pageSets.clear(); }
	void appendToPageSets(const PageSet& ps) { m_docPrefsData.pageSets.append(ps); }
	void setPaperColor(const QColor &c) { m_docPrefsData.displayPrefs.paperColor = c; }
	const QColor& paperColor() const { return m_docPrefsData.displayPrefs.paperColor; }
	bool hyphAutomatic() const { return m_docPrefsData.hyphPrefs.Automatic; }
	bool hyphAutoCheck() const { return m_docPrefsData.hyphPrefs.AutoCheck; }
	void setHyphAutomatic(bool b) { m_docPrefsData.hyphPrefs.Automatic = b; }
	void setHyphAutoCheck(bool b) { m_docPrefsData.hyphPrefs.AutoCheck = b; }
	bool autoSave() const { return m_docPrefsData.docSetupPrefs.AutoSave; }
	int autoSaveTime() const { return m_docPrefsData.docSetupPrefs.AutoSaveTime; }
	int autoSaveCount() const { return m_docPrefsData.docSetupPrefs.AutoSaveCount; }
	bool autoSaveKeep() const { return m_docPrefsData.docSetupPrefs.AutoSaveKeep; }
	bool autoSaveInDocDir() const { return m_docPrefsData.docSetupPrefs.AutoSaveLocation; }
	QString autoSaveDir() const { return m_docPrefsData.docSetupPrefs.AutoSaveDir; }
	bool autoSaveClockDisplay() const { return m_docPrefsData.displayPrefs.showAutosaveClockOnCanvas; }
	void setAutoSave(bool b) { m_docPrefsData.docSetupPrefs.AutoSave = b; }
	void setAutoSaveTime(int i) { m_docPrefsData.docSetupPrefs.AutoSaveTime = i; }
	void setAutoSaveCount(int i) { m_docPrefsData.docSetupPrefs.AutoSaveCount = i; }
	void setAutoSaveKeep(bool i) { m_docPrefsData.docSetupPrefs.AutoSaveKeep = i; }
	void setAutoSaveInDocDir(bool i) { m_docPrefsData.docSetupPrefs.AutoSaveLocation = i; }
	void setAutoSaveDir(const QString& autoDaveDir) { m_docPrefsData.docSetupPrefs.AutoSaveDir = autoDaveDir; }
	//FIXME (maybe) :non const, the loaders make a mess here
	PDFOptions& pdfOptions() { return m_docPrefsData.pdfPrefs; }

	ObjAttrVector& itemAttributes() { return m_docPrefsData.itemAttrPrefs.defaultItemAttributes; }
	const ObjAttrVector& itemAttributes() const { return m_docPrefsData.itemAttrPrefs.defaultItemAttributes; }

	void setItemAttributes(ObjAttrVector& oav) { m_docPrefsData.itemAttrPrefs.defaultItemAttributes = oav; }
	void clearItemAttributes() { m_docPrefsData.itemAttrPrefs.defaultItemAttributes.clear(); }
	void appendToItemAttributes(const ObjectAttribute& oa) { m_docPrefsData.itemAttrPrefs.defaultItemAttributes.append(oa); }

	ToCSetupVector& tocSetups() { return m_docPrefsData.tocPrefs.defaultToCSetups; }
	void setTocSetups(ToCSetupVector& tsv) { m_docPrefsData.tocPrefs.defaultToCSetups = tsv; }
	void clearTocSetups() { m_docPrefsData.tocPrefs.defaultToCSetups.clear(); }
	void appendToTocSetups(const ToCSetup& ts) { m_docPrefsData.tocPrefs.defaultToCSetups.append(ts); }

	void setArrowStyles(QList<ArrowDesc>& as) { m_docPrefsData.arrowStyles = as; }
	QList<ArrowDesc>& arrowStyles() { return m_docPrefsData.arrowStyles; }
	ArrowDesc* arrowStyle(const QString& name);
	bool hasArrowStyle(const QString& name) const;
	void appendToArrowStyles(const struct ArrowDesc& as) { m_docPrefsData.arrowStyles.append(as); }

	bool marginColored() const { return m_docPrefsData.displayPrefs.marginColored; }
	void setMarginColored(bool b) { m_docPrefsData.displayPrefs.marginColored = b; }

	QMap<QString, CheckerPrefs>& checkerProfiles() { return m_docPrefsData.verifierPrefs.checkerPrefsList; }
	void setCheckerProfiles(const QMap<QString, CheckerPrefs>& cl) { m_docPrefsData.verifierPrefs.checkerPrefsList = cl; }
	void set1CheckerProfile(const QString& profileName, const struct CheckerPrefs& cs) { m_docPrefsData.verifierPrefs.checkerPrefsList[profileName] = cs; }
	void clearCheckerProfiles() { m_docPrefsData.verifierPrefs.checkerPrefsList.clear(); }
	const QString& curCheckProfile() const { return m_docPrefsData.verifierPrefs.curCheckProfile; }
	void setCurCheckProfile(const QString& s) { m_docPrefsData.verifierPrefs.curCheckProfile = s; }

	const QString& language() const { return m_docPrefsData.docSetupPrefs.language; }
	void setLanguage(const QString& s) { m_docPrefsData.docSetupPrefs.language = s; }

	int pageOrientation() const { return m_docPrefsData.docSetupPrefs.pageOrientation; }
	void setPageOrientation(int o) { m_docPrefsData.docSetupPrefs.pageOrientation = o; }
	int pagePositioning() const { return m_docPrefsData.docSetupPrefs.pagePositioning; }
	void setPagePositioning(int p) { m_docPrefsData.docSetupPrefs.pagePositioning = p; }

	double pageHeight() const { return m_docPrefsData.docSetupPrefs.pageHeight; }
	double pageWidth() const { return m_docPrefsData.docSetupPrefs.pageWidth; }
	const QString& pageSize() const { return m_docPrefsData.docSetupPrefs.pageSize; }
	void setPageHeight(double h) { m_docPrefsData.docSetupPrefs.pageHeight = h; }
	void setPageWidth(double w) { m_docPrefsData.docSetupPrefs.pageWidth = w; }
	void setPageSize(const QString& s) { m_docPrefsData.docSetupPrefs.pageSize = s; }

	int marginPreset() const { return m_docPrefsData.docSetupPrefs.marginPreset; }
	void setMarginPreset(int mp) { m_docPrefsData.docSetupPrefs.marginPreset = mp; }

	TypoPrefs& typographicPrefs() { return m_docPrefsData.typoPrefs; }
	GuidesPrefs& guidesPrefs() { return m_docPrefsData.guidesPrefs; }
	ItemToolPrefs& itemToolPrefs() { return m_docPrefsData.itemToolPrefs; }
	OperatorToolPrefs& opToolPrefs() { return m_docPrefsData.opToolPrefs; }
	ColorPrefs& colorPrefs() { return m_docPrefsData.colorPrefs; }
	CMSData& cmsSettings() { return m_docPrefsData.colorPrefs.DCMSset; }
	DocumentInformation& documentInfo() { return m_docPrefsData.docInfo; }
	HyphenatorPrefs& hyphenatorPrefs() { return m_docPrefsData.hyphPrefs; }
	void setDocumentInfo(DocumentInformation di) { m_docPrefsData.docInfo = di; }
	DocumentSectionMap& sections() { return m_docPrefsData.docSectionMap; }
	void setSections(DocumentSectionMap dsm) { m_docPrefsData.docSectionMap = std::move(dsm); }
	const QMap<QString, int> & usedFonts() { return UsedFonts; }

	const ApplicationPrefs& prefsData() { return m_docPrefsData; }
	void setNewPrefs(const ApplicationPrefs& prefsData, const ApplicationPrefs& oldPrefsData, bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins);

	// Add, delete and move pages
	
	ScPage* addPage(int pageNumber, const QString& masterPageName = QString(), bool addAutoFrame = false);
	void deletePage(int);
	//! @brief Add a master page with this function, do not use addPage
	ScPage* addMasterPage(int, const QString&);
	void deleteMasterPage(int);
	//! @brief Rebuild master name list
	void rebuildMasterNames();
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
	int addAutomaticTextFrame(int pageNumber);
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
	void swapPage(int a, int b);
	/**
	 * @brief Move page(s) within the document
	 * @param fromPage page index
	 * @param toPage page index
	 * @param count target to move to (page index)
	 * @param position Before, After or at the end
	 */
	void movePage(int fromPage, int toPage, int dest, int position);
	
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
	int addLayer(const QString& layerName, bool activate = false);
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
	bool deleteLayer(int layerID, bool deleteItems);
	/**
	 * @brief Return the number of the current layer
	 * @return Active layer number
	 */
	int activeLayer() const;

	/**
	 * @brief Return the name of the current layer
	 * @return Name of the layer
	 */
	const QString& activeLayerName() const;
	/**
	 * @brief Set the active layer via the layer number
	 * @param layerToActivate Number of the layer
	 * @return Success or failure
	 */
	bool setActiveLayer(int layerToActivate);
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
	bool setLayerPrintable(int layerID, bool isPrintable);
	/**
	 * @brief Is the layer printable
	 * @param layerID ID of the layer
	 * @return Printable or not
	 */
	bool layerPrintable(int layerID) const;
	/**
	 * @brief Set the layer visible via the layer number
	 * @param layerID ID of the layer
	 * @param isViewable true = layer is visible
	 * @return Success or failure
	 */
	bool setLayerVisible(int layerID, bool isViewable);
	/**
	 * @brief Is the layer visible
	 * @param layerID ID of the layer
	 * @return Visible or not
	 */
	bool layerVisible(int layerID) const;
	/**
	 * @brief are objects on the layer selectable
	 * @param layerID ID of the layer
	 * @return Items selectable or not
	 */
	bool layerSelectable(int layerID) const;
	/**
	 * @brief Set objects on the layer selectable via the layer ID
	 * @param layerID ID of the layer
	 * @param isSelectable true = layer objects are selectable
	 * @return bool Success or failure
	 */
	bool setLayerSelectable(int layerID, bool isSelectable);
	/**
	 * @brief Set the layer locked via the layer number
	 * @param layerID ID of the layer
	 * @param isLocked true = layer is locked
	 * @return Success or failure
	 */
	bool setLayerLocked(int layerID, bool isLocked);
	/**
	 * @brief Is the layer locked
	 * @param layerID ID of the layer
	 * @return Locked or not
	 */
	bool layerLocked(int layerID) const;
	/**
	 * @brief Set the layer flow via the layer number
	 * @param layerID ID of the layer
	 * @param flow true = Text flows around objects on this layer
	 * @return Success or failure
	 */
	bool setLayerFlow(int layerID, bool flow);
	/**
	 * @brief does text flow around objects on this layer
	 * @param layerID ID of the layer
	 * @return flow or not
	 */
	bool layerFlow(int layerID) const;
	/**
	 * @brief Set the layer transparency via the layer number
	 * @param layerID ID of the layer
	 * @param trans transparency value 0.0 - 1.0
	 * @return Success or failure
	 */
	bool setLayerTransparency(int layerID, double trans);
	/**
	 * @brief returns the layer transparency
	 * @param layerID ID of the layer
	 * @return transparency value 0.0 - 1.0
	 */
	double layerTransparency(int layerID) const;
	/**
	 * @brief Set the layer layerBlendMode via the layer number
	 * @param layerID ID of the layer
	 * @param blend layerBlendMode
	 * @return Success or failure
	 */
	bool setLayerBlendMode(int ID, int blend);
	/**
	 * @brief returns the layer BlendMode
	 * @param layerID ID of the layer
	 * @return layerBlendMode
	 */
	int layerBlendMode(int ID) const;
	/**
	 * @brief Return the level of the requested layer
	 * @param layerID ID of the layer
	 * @return Level of the layer
	 */
	int layerLevelFromID(int layerID) const;
	/**
	 * @brief Set the layer marker color
	 * @param ID Number of the layer
	 * @param color color of the marker
	 * @return Success or failure
	 */
	bool setLayerMarker(int layerID, const QColor& color);
	/**
	 * @brief returns the layer marker color
	 * @param layerID ID of the layer
	 * @return marker color
	 */
	QColor layerMarker(int layerID) const;
	/**
	 * @brief Set the layer outline mode via the layer number
	 * @param layerID ID of the layer
	 * @param outline true = layer is displayed in outlines only
	 * @return Success or failure
	 */
	bool setLayerOutline(int layerID, bool outline);
	/**
	 * @brief is this layer in outline mode
	 * @param layerID ID of the layer
	 * @return outline or not
	 */
	bool layerOutline(int layerID) const;
	/**
	 * @brief Return the number of the layer at a certain level
	 * @param layerLevel Layer level
	 * @return Layer ID
	 */
	int layerIDFromLevel(int layerLevel) const;
	/**
	 * @brief Return the layer count
	 * @return Number of layers in doc
	 */
	int layerIDFromName(const QString& name) const;
	int layerCount() const;
	/**
	 * @brief Lower a layer
	 * @param layerID ID of the layer
	 * @return Success or failure
	 */
	bool lowerLayer(int layerID);
	/**
	 * @brief Lower a layer using the level
	 * @param layerLevel Level of the layer
	 * @return Success or failure
	 */
	bool lowerLayerByLevel(int layerLevel);
	/**
	 * @brief Raise a layer
	 * @param layerID ID of the layer
	 * @return Success or failure
	 */
	bool raiseLayer(int layerID);
	/**
	 * @brief Raise a layer using the level
	 * @param layerLevel Level of the layer
	 * @return Success or failure
	 */
	bool raiseLayerByLevel(int layerLevel);
	/**
	 * @brief Return the layer name
	 * @param layerID ID of the layer
	 * @return Name of the layer
	 */
	QString layerName(int layerID) const;
	/**
	 * @brief Change the name of a layer
	 * @param layerID ID of the layer
	 * @param newName new name of the layer
	 * @return Success or failure
	 */
	bool changeLayerName(int layerID, const QString& newName);
	/**
	 * @brief Test if items can be selected on a specific layer
	 * 
	 * This function check if items can be selected on the specified layer
	 * by checking layer visibility and selectable property etc...
	 *
	 * @param layerID ID of the layer
	 * @return a boolean
	 */
	bool canSelectItemOnLayer(int layerID) const;
	/**
	 * @brief Does the layer have items on it?
	 * @param layerID ID of the layer
	 * @return Layer contains items bool
	 */
	bool layerContainsItems(int layerID) const;
	/**
	 * @brief Renumber a layer. Used in particular for reinsertion for undo/redo
	 * @param layerID old layer ID
	 * @param newLayerID New layer ID
	 * @return Success or failure
	 */
	bool renumberLayer(int layerID, int newLayerID);
	/**
	 * @brief Return a list of the layers in their order
	 * @param list QStringList to insert the layer names into
	 */
	void orderedLayerList(QStringList* list) const;

	int firstLayerID() const;

	/*!
		* @brief Builds a qmap of the icc profiles used within the document
	 */
	void getUsedProfiles(ProfilesL& usedProfiles) const;
	bool OpenCMSProfiles(ProfilesL InPo, ProfilesL InPoCMYK, ProfilesL MoPo, ProfilesL PrPo);
	void CloseCMSProfiles();
	void SetDefaultCMSParams();
	/**
	 * @brief Switch Colormanagement on or of
	 * @param enable bool, if true Colormanagement is switched on, else off
	 */
	void enableCMS(bool enable);
	
	const ParagraphStyle& paragraphStyle(const QString& name) const { return m_docParagraphStyles.get(name); }
	const StyleSet<ParagraphStyle>& paragraphStyles()  const { return m_docParagraphStyles; }
	bool isDefaultStyle( const ParagraphStyle& p ) const { return m_docParagraphStyles.isDefault(p); }
	bool isDefaultStyle( const CharStyle& c ) const { return m_docCharStyles.isDefault(c); }
// 	bool isDefaultStyle( LineStyle& l ) const { return docLineStyles......; }

	const QHash<QString, multiLine>& lineStyles() const { return docLineStyles; }

	/**
	 * Returns the table style named @a name.
	 */
	const TableStyle& tableStyle(const QString& name) { return m_docTableStyles.get(name); }
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
	const CellStyle& cellStyle(const QString& name) { return m_docCellStyles.get(name); }
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
	
	bool styleExists(const QString& styleName) const;
	bool charStyleExists(const QString& styleName) const;
	
	QList<int> getSortedStyleList() const;
	QList<int> getSortedCharStyleList() const;
	QList<int> getSortedTableStyleList() const;
	QList<int> getSortedCellStyleList() const;
	
	void redefineStyles(const StyleSet<ParagraphStyle>& newStyles, bool removeUnused = false);
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
	void loadStylesFromFile(const QString& fileName);
	/**
	 * @brief Gather styles from another document.
	 *        
	 * @param fileName The path of the document we want to extract its styles
	 * @param tempStyles A pointer to a StyleSet which will be filled by paragraph styles
	 * @param tempCharStyles A pointer to a StyleSet which will be filled by character styles
	 * @param tempLineStyles A map which will be filled by line styles
	 */
	void loadStylesFromFile(const QString& fileName, StyleSet<ParagraphStyle> *tempStyles,
	                                          StyleSet<CharStyle> *tempCharStyles,
											  QHash<QString, multiLine> *tempLineStyles);

	const CharStyle& charStyle(const QString& name) const { return m_docCharStyles.get(name); }
	const StyleSet<CharStyle>& charStyles() const { return m_docCharStyles; }
	void redefineCharStyles(const StyleSet<CharStyle>& newStyles, bool removeUnused = false);
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

	/*!
	 * @brief Returns a stringlist of the item attributes within the document
	 */
	QStringList getItemAttributeNames() const;

	bool AddFont(const QString& name, int fsize = 10);
	/*!
	 * @brief TODO: Reorganise the fonts.. how? Moved from scribus.cpp
	 * CB: almost the same as getUsedFonts???
	 */
	QMap<QString,int> reorganiseFonts();
	/*!
	 * @brief Returns a qmap of the fonts and  their glyphs used within the document
	 */
	void getUsedFonts(QMap<QString,QMap<uint, QString> > &Really);
	void checkItemForFonts(PageItem *it, QMap<QString, QMap<uint, QString> > & Really, uint lc);

	/*!
	 * @brief Replace line style colors
	 */
	void replaceLineStyleColors(const QMap<QString, QString>& colorMap);
	/*!
	* @brief Builds a qmap of the colours used within the document
	*/
	void getUsedColors(ColorList &colorsToUse, bool spot = false) const;
	/*!
	* @brief Return if a specific color is used by line styles
	*/
	bool lineStylesUseColor(const QString& colorName) const;
	/*!
	* @brief Builds a qmap of the gradients used within the document
	*/
	void getUsedGradients(QHash<QString, VGradient> &Gradients) const;
	/*!
	* @brief Set the gradients for a document
	*/
	bool addGradient(QString &name, const VGradient &gradient);
	void setGradients(const QHash<QString, VGradient> &gradients);
	/*!
	* @brief Set the patterns for a document
	*/
	bool addPattern(QString &name, ScPattern& pattern);
	void removePattern(const QString& name);
	void setPatterns(const QHash<QString, ScPattern> &patterns);
	/*!
	* @brief Check pattern with specified name and return it if valid
	*/
	ScPattern* checkedPattern(const QString &name);
	/*!
	* @brief Get a unique pattern name
	*/
	QString getUniquePatternName(const QString& originalName) const;
	/*!
	* @brief Builds a QStringList of the patterns used within the document
	*/
	QStringList getUsedPatterns() const;
	QStringList getUsedPatternsSelection(Selection* customSelection) const;
	QStringList getUsedPatternsHelper(const QString& pattern, QStringList &results) const;
	QStringList getPatternDependencyList(const QStringList& used) const;
	/*!
	* @brief Builds a QStringList of the symbols used within the document
	*/
	QStringList getUsedSymbols() const;
	QStringList getUsedSymbolsHelper(const QString& pattern, QStringList &results) const;

	/*!
	* @brief Check if document use Acrobat Form Fields
	*/
	bool useAcroFormFields() const;

	/*!
	* @brief Check if document use PDF Annotations
	*/
	bool useAnnotations() const;

	/*!
	* @brief Check if document use effects on images
	*/
	bool useImageEffects() const;

	/*!
	* @brief Check if document use effects on images
	*/
	bool useImageColorEffects() const;

	/**
	 * @brief Set and get the document's unit index
	 */
	void setUnitIndex(int);
	int unitIndex() const;
	double unitRatio() const;
	/**
	 * @brief Apply a master page
	 */
	bool applyMasterPage(const QString& pageName, int pageNumber);
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
	 * @brief Undo functions for marks / notes
	 */
	void restoreMarks(UndoState* ss, bool isUndo);
	void restoreNoteStyle(SimpleState* ss, bool isUndo);
	void restoreDeleteNote(UndoState* ss, bool isUndo);

	/**
	 * @brief Save function
	 */
	bool save(const QString& fileName, QString* savedFile = nullptr);
	/**
	 * @brief Set the page margins. Current code uses current page only, also provide a (currently, TODO) option for this.
	 */
	bool changePageProperties(double initialTop, double initialBottom, double initialLeft, double initialRight, double initialHeight, double initialWidth, double Height, double width, int orientation, const QString& pageSize, int marginPreset, bool moveObjects, int pageNumber=-1, int pageType = 0);
	/**
	 * @brief Recalculate the colors after CMS settings change. Update the items in the doc accordingly.
	 */
	void recalculateColorsList(QList<PageItem *> *itemList);
	static void recalculateColorItem(PageItem *item);
	void recalculateColors();
	/**
	 * @brief Copies a normal page to be a master pages
	 */
	bool copyPageToMasterPage(int, int, int, const QString&, bool);
	
	
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
	int itemAdd(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, double x, double y, double b, double h, double w, const QString& fill, const QString& outline, PageItem::ItemKind itemKind = PageItem::StandardItem);

	/** Add an item to the page based on the x/y position. Item will be fitted to the closest guides/margins */
	int itemAddArea(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, double x, double y, double w, const QString& fill, const QString& outline, PageItem::ItemKind itemKind = PageItem::StandardItem);
	
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

	/**
	 * @brief Get index of item in items list
	 */
	int getItemNrFromUniqueID(uint unique) const;

	/**
	 * @brief Return pointer to item
	 */
	PageItem* getItemFromName(const QString& name) const;

	/**
	 * @brief Rebuild item lists taking into account layer order.
	 * Utility function used in various places, basically handles keeping items numbered in the way
	 * they are layered. When layer is a property and not a function of storage, this should be removed.
	 * @sa updateFrameItems();
	 */
	void rebuildItemLists();
	/**
	 * @brief Doc uses automatic text frames?
	 */
	bool usesAutomaticTextFrames() const;
	void setUsesAutomaticTextFrames(bool);
	
	/**
	 * 
	 * @param fn 
	 * @param pageItem 
	 * @param reload 
	 * @return 
	 */
	bool loadPict(const QString& fn, PageItem *pageItem, bool reload = false, bool showMsg = false);
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
	void RecalcPictures(QList<PageItem*>* items, ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia = nullptr);

	/**
	 * @brief Find the minX,MinY and maxX,maxY for the canvas required for the doc
	 */
	void canvasMinMax(FPoint&, FPoint&);

	/**
	 * @brief Find the optimal area for canvas
	 */
	QRectF canvasOptimalRect();
	
	int  OnPage(double x2, double  y2);
	int  OnPage(PageItem *currItem);
	void GroupOnPage(PageItem *currItem);

	void reformPages(bool moveObjects = true);
	/** @brief Refresh automatic guides once Margin struct has been properly configure by reformPages() */
	void refreshGuides();

	/** @brief Check and fix if needed PageItem OwnPage member */
	void fixItemPageOwner();
	/** @brief Fix character styles */
	void fixCharacterStyles();
	/** @brief Fix paragraph styles */
	void fixParagraphStyles();
	/** @brief Fix notes styles */
	void fixNotesStyles();
	
	/**
	 * @brief Return the x or y offset for a page on the canvas
	 * @retval double containing the offset. Returns -1.0 if page not in Pages list (as -ve is not possible).
	 * Mostly saves bringing in extra includes into files that already have scribusdoc.h
	 */
	double getXOffsetForPage(int) const;
	double getYOffsetForPage(int) const;
	void getBleeds(int pageNumber, MarginStruct& bleedData) const;
	void getBleeds(const ScPage* page, MarginStruct& bleedData) const;
	void getBleeds(const ScPage* page, const MarginStruct& baseValues, MarginStruct& bleedData) const;
	
	/**
	 * @brief Item type conversion functions
	 */
	PageItem* convertItemTo(PageItem* currItem, PageItem::ItemType newType, PageItem* secondaryItem = nullptr);
	
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
	bool itemNameExists(const QString& itemName) const;
	
	/**
	 * @brief Set the doc into Master page mode
	 * Do we need to return if the move to master page mode was successful?
	 */
	void setMasterPageMode(bool);
	void assignPageModeLists();

	/*** Is the document in master page mode? */
	bool masterPageMode() const { return m_masterPageMode; }
	/**
	 * @brief Set the doc into symbol edit mode
	 */
	void setSymbolEditMode(bool mode, const QString& symbolName = "");

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
	void addSection(int number = 0, const QString& name = QString(), const uint fromindex = 0, const uint toindex = 0, const  NumFormat type = Type_1_2_3, const uint sectionstartindex = 0, bool reversed = false, bool active = true, const QChar fillChar = QChar(), int fieldWidth = 0);
	/**
	 * @brief Delete a section from the document sections list
	 */
	bool deleteSection(uint);
	/**
	 * @brief Gets the page number to be printed based on the section it is in.
	 * Returns QString() on failure to find the pageIndex
	 */
	const QString getSectionPageNumberForPageIndex(uint) const;
	/**
	 * @brief Gets the page number fill character to be printed based on the section it is in.
	 * Returns QString() on failure to find the pageIndex
	 */
	const QChar getSectionPageNumberFillCharForPageIndex(uint) const;
	/**
	 * @brief Gets the page number fill character to be printed based on the section it is in.
	 * Returns QString() on failure to find the pageIndex
	 */
	int getSectionPageNumberWidthForPageIndex(uint) const;
	/**
	 * @brief Gets the key of the sections map based on the section the page index is in.
	 * Returns -1 on failure to find the pageIndex
	 */
	int getSectionKeyForPageIndex(uint pageIndex) const;
	/**
	 *
	 *
	 */
	void updateSectionPageNumbersToPages();
	/**
	 * @param otherPageIndex 
	 * @param location 
	 * @param count 
	 */
	void addPageToSection(uint otherPageIndex, uint location, uint count = 1);
	/**
	 * 
	 * @param pageIndex 
	 */
	void removePageFromSection(uint pageIndex);
	/**
	 * 
	 */
	void setFirstSectionFromFirstPageNumber();
	/**
	 * @param pageIndex page nr
	 * @brief Returns name of section where page is located
	 */
	QString getSectionNameForPageIndex(uint pageIndex) const;

	/**
	 * Update annotation links when a page is added
	 */
	void addPageToAnnotLinks(int otherPageIndex, int location, int count = 1);

	/**
	 * Update annotation links when a page is removed
	 */
	void removePageFromAnnotLinks(int pageIndex);

	//! @brief Some internal align tools
	typedef enum {alignFirst, alignLast, alignPage, alignMargins, alignGuide, alignSelection } AlignTo;
	typedef enum {alignByMoving, alignByResizing } AlignMethod;
	void buildAlignItemList(Selection* customSelection = nullptr);
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
	void insertColor(const QString& name, double c, double m, double y, double k);
	
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


	QList<PageItem*>* groupOfItem(QList<PageItem*>* itemList, PageItem* item);
	PageItem* groupObjectsSelection(Selection* customSelection = nullptr);
	PageItem* groupObjectsList(QList<PageItem*> &itemList);
	void groupObjectsToItem(PageItem* groupItem, QList<PageItem*> &itemList);
	PageItem * itemSelection_GroupObjects  (bool changeLock, bool lock, Selection* customSelection = nullptr, PageItem_Group* groupItem = nullptr);
	void itemSelection_UnGroupObjects(Selection* customSelection = nullptr);
	void addToGroup(PageItem* group, PageItem* item);
	void removeFromGroup(PageItem* item);
	void rescaleGroup(PageItem* group, double scale);
	void resizeGroupToContents(PageItem* group);
	void itemSelection_resizeGroupToContents(Selection* customSelection = nullptr);
	void itemSelection_convertItemsTo(const PageItem::ItemType newType, Selection* restoredSelection=0, Selection* customSelection = nullptr);
	void itemSelection_convertItemsToSymbol(QString& patternName);
	void itemSelection_ApplyParagraphStyle(const ParagraphStyle & newstyle, Selection* customSelection = nullptr, bool rmDirectFormatting = false);
	void itemSelection_SetParagraphStyle(const ParagraphStyle & newstyle, Selection* customSelection = nullptr);
	void itemSelection_ApplyCharStyle(const CharStyle & newstyle, Selection* customSelection = nullptr, const QString& ETEA = "");
	void itemSelection_SetCharStyle(const CharStyle & newstyle, Selection* customSelection = nullptr);
	void itemSelection_EraseParagraphStyle(Selection* customSelection = nullptr);
	void itemSelection_EraseCharStyle(Selection* customSelection = nullptr);
	void itemSelection_SetNamedParagraphStyle(const QString & name, Selection* customSelection = nullptr);
	void itemSelection_SetNamedCharStyle(const QString & name, Selection* customSelection = nullptr);
	void itemSelection_SetNamedLineStyle(const QString & name, Selection* customSelection = nullptr);
	void itemSelection_SetNamedCellStyle(const QString & name, Selection* customSelection = nullptr);
	void itemSelection_SetNamedTableStyle(const QString & name, Selection* customSelection = nullptr);
	void itemSelection_SetSoftShadow(bool has, QString color, double dx, double dy, double radius, int shade, double opac, int blend, bool erase, bool objopa);
	void itemSelection_SetLineWidth(double w, Selection* customSelection = nullptr);
	void itemSelection_SetLineArt(Qt::PenStyle w, Selection* customSelection = nullptr);
	void itemSelection_SetLineJoin(Qt::PenJoinStyle w, Selection* customSelection = nullptr);
	void itemSelection_SetLineEnd(Qt::PenCapStyle w, Selection* customSelection = nullptr);
	void itemSelection_SetAlignment(int w, Selection* customSelection = nullptr);
	void itemSelection_SetDirection(int w, Selection* customSelection = nullptr);
	void itemSelection_SetLineSpacing(double w, Selection* customSelection = nullptr);
	void itemSelection_SetLineSpacingMode(int w, Selection* customSelection = nullptr);
	void itemSelection_SetLanguage(const QString& w, Selection* customSelection = nullptr);
	void itemSelection_SetFont(const QString& font, Selection* customSelection = nullptr);
	void itemSelection_SetFontSize(int size, Selection* customSelection = nullptr);
	void itemSelection_SetFontFeatures(const QString& fontfeature, Selection* customSelection = nullptr);
	void itemSelection_SetParBackgroundColor(QString farbe, Selection* customSelection = nullptr);
	void itemSelection_SetParBackgroundShade(int sha, Selection* customSelection = nullptr);
	void itemSelection_SetBackgroundColor(QString farbe, Selection* customSelection = nullptr);
	void itemSelection_SetBackgroundShade(int sha, Selection* customSelection = nullptr);
	void itemSelection_SetFillColor(QString farbe, Selection* customSelection = nullptr);
	void itemSelection_SetFillShade(int sha, Selection* customSelection = nullptr);
	void itemSelection_SetStrokeColor(QString farbe, Selection* customSelection = nullptr);
	void itemSelection_SetStrokeShade(int sha, Selection* customSelection = nullptr);
	void itemSelection_SetScaleV(int, Selection* customSelection = nullptr);
	void itemSelection_SetScaleH(int, Selection* customSelection = nullptr);
	void itemSelection_SetBaselineOffset(int, Selection* customSelection = nullptr);
	void itemSelection_SetOutlineWidth(int, Selection* customSelection = nullptr);
	void itemSelection_SetShadowOffsets(int shx, int shy, Selection* customSelection = nullptr);
	void itemSelection_SetUnderline(int pos, int wid, Selection* customSelection = nullptr);
	void itemSelection_SetStrikethru(int pos, int wid, Selection* customSelection = nullptr);
	void itemSelection_SetEffects(int s, Selection* customSelection = nullptr);
	void itemSelection_SetOpticalMargins(int i, Selection* customSelection = nullptr);
	void itemSelection_resetOpticalMargins(Selection* customSelection = nullptr);
	void itemSelection_SetColorProfile(const QString& profileName, Selection* customSelection = nullptr);
	void itemSelection_SetRenderIntent(int intentIndex, Selection* customSelection = nullptr);
	void itemSelection_SetCompressionMethod(int cmIndex, Selection* customSelection = nullptr);
	void itemSelection_SetCompressionQuality(int cqIndex, Selection* customSelection = nullptr);
	void itemSelection_SetTracking(int us, Selection* customSelection = nullptr);
	void MirrorPolyH(PageItem *currItem);
	void MirrorPolyV(PageItem *currItem);
	bool getItem(PageItem **currItem, int nr = -1);
	void setFrameRect();
	void setFrameRounded();
	void setFrameOval();

	void setRedrawBounding(PageItem *currItem);
	void adjustCanvas(const FPoint& minPos, const FPoint& maxPos, bool absolute = false);
	struct PicResMapped
	{
		PicResMapped(bool applyNewRes, int lowResType) { m_applyNewRes = applyNewRes; m_lowResType = lowResType;}

		void operator()(PageItem *item)
		{
			QList<PageItem*> allItems;
			if (item->isGroup())
				allItems = item->asGroupFrame()->getAllChildren();
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
					if (item->isLatexFrame())
						item->asLatexFrame()->rerunApplication(false);
					else
						item->loadImage(item->Pfile, true, -1, false);
					item->setImageFlippedH(fho);
					item->setImageFlippedV(fvo);
					item->adjustPictScale();
				}
			}
			allItems.clear();
		}

		bool m_applyNewRes;
		int m_lowResType;
	};

	enum RecalcPictureResFlags
	{
		RecalcPicRes_ApplyNewRes = 1,
		RecalcPicRes_ImageWithEffectsOnly = 2,
		RecalcPicRes_ImageWithColorEffectsOnly = 4,
	};
	void recalcPicturesRes(int recalcFlags = 0);

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
	/*! \brief Apply grid to an QRectF, from ScribusView */
	QRectF ApplyGridF(const QRectF& in);
	/*! \brief Does this doc have any TOC setups and potentially a TOC to generate */
	bool hasTOCSetup() { return !m_docPrefsData.tocPrefs.defaultToCSetups.empty(); }

	enum SelectionSkipBehavior
	{
		IncludeSelection = 0,
		ExcludeSelection = 1
	};

	//! \brief Get the closest guide to the given point
	void getClosestGuides(double xin, double yin, double *xout, double *yout, ScPage* refPage = nullptr);
	//! \brief Get the closest border of another element to the given point
	void getClosestElementBorder(double xin, double yin, double *xout, double *yout, SelectionSkipBehavior behavior = IncludeSelection);
	//! \brief Get the closest page margin or bleed
	void getClosestPageBoundaries(double xin, double yin, double &xout, double &yout, ScPage* refPage);
	//! \brief Snap an item to the guides
	void SnapToGuides(PageItem *currItem);
	bool ApplyGuides(double *x, double *y, bool elementSnap = false);
	bool ApplyGuides(FPoint* point, bool elementSnap = false);

	bool moveItem(double newX, double newY, PageItem* ite);
	void rotateItem(double win, PageItem *currItem);
	void moveRotated(PageItem *currItem, const FPoint& npv);
	bool sizeItem(double newX, double newY, PageItem *pi, bool fromMP = false, bool DoUpdateClip = true, bool redraw = true);
	bool moveSizeItem(const FPoint& newX, const FPoint& newY, PageItem* currItem, bool fromMP = false);
	void adjustItemSize(PageItem *currItem, bool includeGroup = false);
	void moveGroup(double x, double y, Selection* customSelection = nullptr);
	void rotateGroup(double angle, Selection* customSelection = nullptr);
	void rotateGroup(double angle, const FPoint& RCenter, Selection* customSelection = nullptr);
	void scaleGroup(double scx, double scy, bool scaleText = true, Selection* customSelection = nullptr, bool scaleLine = false);
	//! \brief Get a list of frames of certain type
	QHash<PageItem*, QString> getDocItemNames(PageItem::ItemType itemType);
	//! \brief Returns a serializer for this document
	Serializer *serializer();
	//! \brief Returns a text serializer for this document, used to paste text chunks
	Serializer *textSerializer();

	//! \brief Retrieve canvas position of text char in specified text chain
	bool textCanvasPosition(PageItem* item, int textPos, QPointF& canvasPos);

	//! \brief Get rotation mode
	int rotationMode() const {return m_rotMode;}
	//! \brief Set rotation mode
	void setRotationMode(int val);

	//! \brief Functions which avoid doc updater and update manager to send too much
	// unnecessary signals when doing updates on multiple items
	void beginUpdate();
	void endUpdate();

	int addToInlineFrames(PageItem *item);
	void removeInlineFrame(int fIndex);
	void checkItemForFrames(PageItem *it, int fIndex);
	bool hasPreflightErrors();
	QFileDevice::Permissions filePermissions() { return m_docFilePermissions; }
	void saveFilePermissions(QFileDevice::Permissions p) { m_docFilePermissions = p; }

protected:
	void addSymbols();
	void applyPrefsPageSizingAndMargins(bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins);
	bool m_hasGUI {false};
	QFileDevice::Permissions m_docFilePermissions {QFileDevice::ReadOwner|QFileDevice::WriteOwner};
	ApplicationPrefs& m_appPrefsData;
	ApplicationPrefs m_docPrefsData;
	UndoManager * const m_undoManager;
	bool m_loading {false};
	bool m_modified {false};
	int  m_undoRedoOngoing {0};
	int m_ActiveLayer {0};
	double m_docUnitRatio;
	int m_rotMode {0};
	bool m_automaticTextFrames; // Flag for automatic Textframes
	bool m_masterPageMode {false};
	bool m_symbolEditMode {false};
	bool m_inlineEditMode {false};
	int  m_storedLayerID {0};
	bool m_storedLayerLock {false};
	bool m_storedLayerVis {false};
	QMap<QString, double> m_constants;
	ScribusMainWindow* m_ScMW {nullptr};
	ScribusView* m_View {nullptr};
	ScGuardedObject<ScribusDoc> m_guardedObject;
	Serializer *m_serializer {nullptr};
	Serializer *m_tserializer {nullptr};
	QString m_currentEditedSymbol;
	int m_currentEditedIFrame {0};
	QString m_documentFileName;

public: // Public attributes
	bool is12doc {false}; //public for now, it will be removed later
	int NrItems {0};
	int First {1};
	int Last {0};
	int viewCount {0};
	int viewID {0};
	bool SnapGrid {false};
	bool SnapGuides {false};
	bool SnapElement {false};
	bool GuideLock {false};
	bool dontResize {false};
	/** \brief Minimum and Maximum Points of Document */
	FPoint minCanvasCoordinate;
	FPoint maxCanvasCoordinate;
	double rulerXoffset {0.0};
	double rulerYoffset {0.0};
	//! Pages is a pointer to the lists of pages that exist in a document. In Normal mode, it points to DocPages. In Master Page mode it points to MasterPages. See ScribusDoc::assignPageModeLists()
	QList<ScPage*>* Pages {nullptr};
	/** \brief List of Master Pages */
	QList<ScPage*> MasterPages;
	/** \brief List of Document Pages */
	QList<ScPage*> DocPages;
	/** \brief List for temporary Pages */
	QList<ScPage*> TempPages;
	/** \brief Mapping Master Page Name to Master Page numbers */
	QMap<QString,int> MasterNames;
	/** \brief List of Objects */
	//! Items is a pointer to the lists of items that exist in a document. In Normal mode, it points to DocItems. In Master Page mode it points to MasterItems. See ScribusDoc::assignPageModeLists()
	QList<PageItem*>* Items {nullptr};
	//! The list of master page items
	QList<PageItem*> MasterItems;
	//! The list of page items
	QList<PageItem*> DocItems;
	QHash<int, PageItem*> FrameItems;
	QList<PageItem*> EditFrameItems;

	Selection* const m_Selection;
	/** \brief Number of Columns */
	double PageSp {1.0};
	/** \brief Distance of Columns */
	double PageSpa {0.0};
	///** \brief current Pagelayout */
	//int currentPageLayout;
	/** \brief First page number in document */
	int FirstPnum {1};
	/** \brief Im Dokument benutzte Farben */
	ColorList PageColors;
	int appMode {modeNormal};
	int SubMode {-1};
	double *ShapeValues {nullptr};
	int ValCount {0};
	QMap<QString,int> UsedFonts;
	SCFonts * const AllFonts;
	QList<AlignObjs> AObjects;
	ParagraphStyle currentStyle;
	NodeEditContext nodeEdit;
	/** \brief Letztes Element fuer AutoTextrahmen */
	PageItem *LastAuto {nullptr};
	/** \brief Erstes Element fuer AutoTextrahmen */
	PageItem *FirstAuto {nullptr};
	bool DragP {false};
	bool leaveDrag {false};
	PageItem *DraggedElem {nullptr};
	PageItem *ElemToLink {nullptr};
	QList<PageItem*> DragElements;

private:
	StyleSet<ParagraphStyle> m_docParagraphStyles;
	StyleSet<CharStyle> m_docCharStyles;
	StyleSet<TableStyle> m_docTableStyles;
	StyleSet<CellStyle> m_docCellStyles;

public:
	ScLayers Layers;
	//bool marginColored;
	int GroupCounter {1};

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
	ScColorTransform stdLabToScreenTrans;
	ScColorTransform stdProofLab;
	ScColorTransform stdProofLabGC;
	bool BlackPoint {true};
	bool SoftProofing {false};
	bool Gamut {false};
	eRenderIntent IntentColors;
	eRenderIntent IntentImages;
	bool HasCMS {false};
	QMap<QString,QString> JavaScripts;
	int TotalItems {0};
	PrintOptions Print_Options;
	bool RePos {false};
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

		bool operator<(const BookMa& other) const { return ItemNr < other.ItemNr; }
	};
	QList<BookMa> BookMarks;
	bool hasName {false};
	bool isConverted {false};
	QTimer * const autoSaveTimer;
	QList<QString> autoSaveFiles;
	QHash<QString, multiLine> docLineStyles;
	QHash<QString, ScPattern> docPatterns;
	QHash<QString, VGradient> docGradients;
	QWidget* WinHan {nullptr};
	bool DoDrawing {true};
	bool drawAsPreview {false};
	bool viewAsPreview {false};
	bool editOnPreview {false};
	int previewVisual {0};
	struct OpenNodesList
	{
		int type { -1 };
		ScPage* page { nullptr };
		PageItem *item { nullptr };
	};
	QList<OpenNodesList> OpenNodes;
	QTimer *CurTimer {nullptr};
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
	
	Hyphenator* docHyphenator {nullptr};
	void itemResizeToMargin(PageItem* item, int direction); //direction reflect enum numbers from Canvas::FrameHandle

private:
	UndoTransaction m_itemCreationTransaction;
	UndoTransaction m_alignTransaction;

	ScPage* m_currentPage {nullptr};
	UpdateManager m_updateManager;
	MassObservable<PageItem*> m_itemsChanged;
	MassObservable<ScPage*> m_pagesChanged;
	MassObservable<QRectF> m_regionsChanged;
	DocUpdater* m_docUpdater {nullptr};
	
signals:
	//Lets make our doc talk to our GUI rather than confusing all our normal stuff
	/**
	 * @brief Let the document tell whatever is listening that it has changed
	 */
	void docChanged();
	void updateContents();
	void updateContents(const QRect &r);
	void firstSelectedItemType(int);

	void updateEditItem();
	void updateAutoSaveClock();
	void addBookmark(PageItem *);
	void deleteBookmark(PageItem *);
	
public slots:
	void selectionChanged();
	void itemSelection_ToggleLock();
	void itemSelection_ToggleSizeLock();
	void itemSelection_ToggleImageShown();
	void itemSelection_TogglePrintEnabled();
	void itemSelection_ToggleBookMark(Selection* customSelection = nullptr);
	void itemSelection_ToggleAnnotation(Selection* customSelection = nullptr);
	void itemSelection_Transform(int nrOfCopies, const QTransform& matrix, int basepoint);
	void itemSelection_ChangePreviewResolution(int id);

	/*! \brief Change display quality of all images in document.
	\author  OssiLehtinen
	*/
	void allItems_ChangePreviewResolution(int id);

	//FIXME : change to process a selection
	void item_setFrameShape(PageItem* item, int frameType, int count, double* points); 

	void itemSelection_ClearItem(Selection* customSelection = nullptr, bool useWarning = false);
	void itemSelection_TruncateItem(Selection* customSelection = nullptr);
	//! Delete the items in the current selection. When force is true, we do not warn the user and make SE happy too. Force is used from @sa Page::restorePageItemCreation
	void itemSelection_DeleteItem(Selection* customSelection = nullptr, bool forceDeletion = false);
	void itemSelection_SetItemFillTransparency(double t, Selection* customSelection = nullptr);
	void itemSelection_SetHyphenWordMin(int wordMin, Selection* customSelection = nullptr);
	void itemSelection_SetHyphenConsecutiveLines(int consecutiveLines, Selection* customSelection = nullptr);
	void itemSelection_SetHyphenChar(uint hyphenChar, Selection* customSelection = nullptr);
	void itemSelection_SetItemLineTransparency(double t, Selection* customSelection = nullptr);
	void itemSelection_SetItemFillBlend(int t);
	void itemSelection_SetItemLineBlend(int t);
	void itemSelection_SetLineGradient(const VGradient& newGradient, Selection* customSelection = nullptr);
	void itemSelection_SetFillGradient(const VGradient& newGradient, Selection* customSelection = nullptr);
	void itemSelection_SetMaskGradient(const VGradient& newGradient, Selection* customSelection = nullptr);
	void itemSelection_SetOverprint(bool overprint, Selection* customSelection = nullptr);
	void itemSelection_ApplyImageEffects(ScImageEffectList& newEffectList, Selection* customSelection = nullptr);
	void itemSelection_FlipH(Selection* customSelection = nullptr);
	void itemSelection_FlipV(Selection* customSelection = nullptr);
	void itemSelection_Rotate(double angle, Selection* customSelection = nullptr);
	void itemSelection_DoHyphenate();
	void itemSelection_DoDeHyphenate();
	void itemSelection_UnlinkTextFrameAndCutText(Selection* customSelection = nullptr);
	void itemSelection_SendToLayer(int layerID);
	void itemSelection_SetImageOffset(double x, double y, Selection* customSelection = nullptr);
	void itemSelection_SetImageScale(double x, double y, Selection* customSelection = nullptr);
	void itemSelection_SetImageScaleAndOffset(double ox, double oy, double sx, double sy, Selection* customSelection = nullptr);
	void itemSelection_SetImageRotation(double rot, Selection* customSelection = nullptr);
	void itemSelection_AlignItemLeft(int i, double newX, ScribusDoc::AlignMethod how);
	void itemSelection_AlignItemRight(int i, double newX, ScribusDoc::AlignMethod how);
	void itemSelection_AlignItemTop(int i, double newY, ScribusDoc::AlignMethod how);
	void itemSelection_AlignItemBottom(int i, double newY, ScribusDoc::AlignMethod how);
	void itemSelection_AlignLeftOut(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignRightOut(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignBottomIn(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignRightIn(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignBottomOut(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignCenterHor(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignLeftIn(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignCenterVer(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignTopOut(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);
	void itemSelection_AlignTopIn(ScribusDoc::AlignTo currAlignTo, ScribusDoc::AlignMethod currAlignMethod, double guidePosition);

	void sendItemSelectionToBack();
	void bringItemSelectionToFront();
	void itemSelection_LowerItem();
	void itemSelection_RaiseItem();
	void itemSelection_DistributeDistH(bool usingDistance = false, double distance = 0.0, bool reverseDistribute = false);
	void itemSelection_DistributeAcrossPage(bool useMargins = false);
	void itemSelection_DistributeRight();
	void itemSelection_DistributeBottom();
	void itemSelection_DistributeCenterH();
	void itemSelection_DistributeDistV(bool usingDistance = false, double distance = 0.0, bool reverseDistribute = false);
	void itemSelection_DistributeDownPage(bool useMargins = false);
	void itemSelection_DistributeLeft();
	void itemSelection_DistributeCenterV();
	void itemSelection_DistributeTop();
	void itemSelection_SwapLeft();
	void itemSelection_SwapRight();
	void itemSelection_Duplicate(double shiftX, double shiftY, Selection* customSelection = nullptr);
	void itemSelection_MultipleDuplicate(const ItemMultipleDuplicateData&, Selection* customSelection = nullptr);
	void itemSelection_UniteItems(Selection* customSelection = nullptr);
	void itemSelection_SplitItems(Selection* customSelection = nullptr);
	/**
	 * Adjust an image frame's size to fit the size of the image in it
	 */
	void itemSelection_AdjustFrametoImageSize(Selection* customSelection = nullptr);
	/**
	 * Adjust an image size to fit the size of the frame
	 */
	void itemSelection_AdjustImagetoFrameSize(Selection* customSelection = nullptr);
	void itemSelection_AdjustFrameHeightToText( Selection *customSelection = nullptr);
	//! @brief startArrowID or endArrowID of -1 mean not applying a selection at this point.
	void itemSelection_ApplyArrowHead(int startArrowID=-1, int endArrowID=-1, Selection* customSelection = nullptr);
	void itemSelection_ApplyArrowScale(int startArrowSc, int endArrowSc, Selection* customSelection);

	void itemSelection_SetItemPen(QString color, Selection* customSelection = nullptr);
	void itemSelection_SetItemPenShade(int sha, Selection* customSelection = nullptr);
	void itemSelection_SetItemGradStroke(int typ, Selection* customSelection = nullptr);
	void itemSelection_SetItemBrush(QString colorName, Selection* customSelection = nullptr);
	void itemSelection_SetItemBrushShade(int sha, Selection* customSelection = nullptr);
	void itemSelection_SetItemGradMask(int typ, Selection* customSelection = nullptr);
	void itemSelection_SetItemGradFill(int typp, Selection* customSelection = nullptr);
	void itemSelection_SetItemPatternFill(const QString& pattern, Selection* customSelection = nullptr);
	void itemSelection_SetItemPatternProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);
	void itemSelection_SetItemStrokePattern(const QString& pattern, Selection* customSelection = nullptr);
	void itemSelection_SetItemStrokePatternProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, double space, bool mirrorX, bool mirrorY, Selection* customSelection = nullptr);
	void itemSelection_SetItemStrokePatternType(bool type, Selection* customSelection = nullptr);
	void itemSelection_SetItemPatternMask(const QString& pattern, Selection* customSelection = nullptr);
	void itemSelection_SetItemPatternMaskProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY, Selection* customSelection = nullptr);

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
	 * If in table edit mode and there is a cell selection, each contiguous range of selected rows
	 * is distributed. If there is no cell selection, all rows in the table are distributed.
	 *
	 * If there is no table selected, then this slot does nothing.
	 */
	void itemSelection_DistributeTableRowsEvenly();

	/**
	 * Distributes columns in a table evenly.
	 *
	 * If in table edit mode and there is a cell selection, each contiguous range of selected columns
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
	void updatePict(const QString& name);
	void updatePictDir(const QString& name);
	void removePict(const QString& name);

// Marks and notes
	/**
	 * Explanation
	 * master frame - text frame with marks for notes
	 * notesframe - frame with notes
	 * master & note mark - master is mark in "master" text, note mark is at beginning of note in noteframe
	 */
	//data handling structures
private:
	bool m_docUsesMarksAndNotes {false};
	QList<Mark*> m_docMarksList;
	QList<TextNote*> m_docNotesList;
	//flags used for indicating needs of updates
	bool m_flag_notesChanged {false};

	void multipleDuplicateByPage(const ItemMultipleDuplicateData& mdData, Selection& selection, QString& tooltip);

public:
	bool usesMarksAndNotes() const { return m_docUsesMarksAndNotes; }
	void setUsesMarksAndNotes(bool b) { m_docUsesMarksAndNotes = b; }
	//return page where endnotesframe should be located depending of notes style range and location of master mark
	const ScPage* page4EndNotes(NotesStyle* NS, PageItem* item);
	const QList<Mark*>& marksList() { return m_docMarksList; }
	const QList<TextNote*>& notesList() { return m_docNotesList; }
	QList<NotesStyle*> m_docNotesStylesList;
	QMap<PageItem_NoteFrame*, rangeItem> m_docEndNotesFramesMap;
	QList<NotesStyle*> ns2Update; //list of notes styles to update

	//returns list of notesframes for given Notes Style
	QList<PageItem_NoteFrame*> listNotesFrames(NotesStyle* NS);

	//flags used for indicating needs of updates
	bool notesChanged() { return m_flag_notesChanged; }
	void setNotesChanged(bool on) { m_flag_notesChanged = on; }
	bool flag_restartMarksRenumbering {false};
	bool flag_updateMarksLabels {false};
	bool flag_updateEndNotes {false};
	bool flag_layoutNotesFrames {true};

	//returns list of marks labels for given mark type
	QStringList marksLabelsList(MarkType type);

	//return mark with given label and given type
	Mark* getMark(const QString& label, MarkType type); //returns mark with label and type (labels are unique only for same type marks)
	Mark* newMark(Mark* mrk = nullptr);
	TextNote* newNote(NotesStyle* NS);
	
	bool isMarkUsed(const Mark* mrk, bool visible = false) const;
	//set cursor in text where given mark will be found
	void setCursor2MarkPos(const Mark* mark);
	//return false if mark was not found
	bool eraseMark(Mark* mrk, bool fromText = false, PageItem* item = nullptr, bool force = false); //force is used only for deleting non-unique marks by MarksManager
	void setUndoDelMark(Mark* mrk);
	//invalidate all text frames where given mark will found
	//useful spacially for variable text marks after changing its text definition
	//if forceUpdate then found master frames are relayouted
	bool invalidateVariableTextFrames(Mark* mrk, bool forceUpdate = false); //returns if any text was changed

	//for foot/endnotes
	NotesStyle* newNotesStyle(const NotesStyle& NS);
	void renameNotesStyle(NotesStyle* NS, const QString& newName);
	//delete whole notes style with its notesframes and notes
	void deleteNotesStyle(const QString& nsName);
	void undoSetNotesStyle(SimpleState* ss, NotesStyle* ns);
	NotesStyle* getNotesStyle(const QString& nsName);
	//delete note, if fromText than marks for given note will be removed
	void deleteNote(TextNote* note);
	void setUndoDelNote(TextNote* note);
	PageItem_NoteFrame* createNoteFrame(PageItem_TextFrame* inFrame, NotesStyle *nStyle, int index = -1);
	PageItem_NoteFrame* createNoteFrame(NotesStyle *nStyle, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	//delete noteframe
	void delNoteFrame(PageItem_NoteFrame *noteFrame, bool removeMarks = true, bool forceDeletion = true);
	//renumber notes for given notes style
	//return true if doc needs update after changing numbers of notes
	bool updateNotesNums(NotesStyle* nStyle);
	//set new text styles for notes marks
	void updateNotesFramesStyles(NotesStyle* nStyle);
	//check conflicts between notes styles
	bool validateNSet(const NotesStyle& NS, QString newName = "");
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
	void updateEndnotesFrames(NotesStyle* nStyle = nullptr, bool invalidate = false);
	//update endnotesframe content
	void updateEndNotesFrameContent(PageItem_NoteFrame* nF, bool invalidate = false);
	//insert noteframe into list of changed
	void endNoteFrameChanged(PageItem_NoteFrame* nF) { m_docEndNotesFramesChanged.append(nF); }
	//update content for changed endnotesframes
	void updateChangedEndNotesFrames();

	//finds mark position in text
	//return true if mark was found, CPos is set for mark`s position
	//if item == nullptr then search in all items and if mark is found than item is set
	int findMarkCPos(const Mark* mrk, PageItem* &item, int start = 0) const;
	QList<PageItem_NoteFrame*> m_docEndNotesFramesChanged;

	//finds item which holds given mark, start searching from next to lastItem index in DocItems
	PageItem* findMarkItem(const Mark* mrk, PageItem* &lastItem) const;
	
private:
	//QMap<PageItem_NoteFrame*, QList<TextNote *> > map of notesframes and its list of notes
	NotesInFrameMap m_docNotesInFrameMap;

	PageItem* findFirstMarkItem(const Mark* mrk) const { PageItem* tmp = nullptr; return findMarkItem(mrk, tmp); }

	//search for endnotesframe for given notes style and item holding master mark or section number
	PageItem_NoteFrame* endNoteFrame(NotesStyle* nStyle, void* item = nullptr);
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
	QStringList orgNumNames; //orgNumerations keeps original settings read from paragraph styles for reset settings overridden localy
	void setupNumerations(); //read styles for used auto-numerations, initialize numCounters
	QString getNumberStr(const QString& numName, int level, bool reset, const ParagraphStyle &style);
	void setNumerationCounter(const QString& numName, int level, int number);
	bool flag_Renumber {false};
	bool flag_NumUpdateRequest {false};
	// for local numeration of paragraphs
	int  updateLocalNums(StoryText& itemText); //return first invalidated char
	void updateNumbers(bool updateNumerations = false);
	void itemSelection_ClearBulNumStrings(Selection *customSelection);
/* Functions for PDF Form Actions */

public:
	void SubmitForm();
	void ImportData();
	void ResetFormFields();
	QString documentFileName() const;
	void setDocumentFileName(const QString& documentFileName);
};

Q_DECLARE_METATYPE(ScribusDoc*);

#endif
