/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribusdoc.cpp  -  description
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

#include <memory>
#include <utility>
#include <sstream>

#include <QByteArray>
 #include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QList>
#include <QTime>
#include <QPainter>
#include <QPixmap>
#include <QProgressBar>

#include "canvas.h"
#include "colorblind.h"
#include "commonstrings.h"
#include "desaxe/digester.h"
//#include "desaxe/saxXML.h"
#include "fileloader.h"
#include "filewatcher.h"
#include "ui/guidemanager.h"
#include "hyphenator.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "pageitem_latexframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#ifdef HAVE_OSG
	#include "pageitem_osgframe.h"
#endif
#include "pageitem_symbol.h"
#include "pageitem_group.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_arc.h"
#include "pageitem_spiral.h"
#include "ui/pagepalette.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "resourcecollection.h"
#include "sccolorengine.h"
#include "ui/scmessagebox.h"
#include "colormgmt/sccolormgmtenginefactory.h"
#include "scclocale.h"
#include "scpainter.h"
#include "sclimits.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribuswin.h"
#include "selection.h"
#include "serializer.h"
#include "ui/hruler.h"
#include "ui/layers.h"
#include "ui/storyeditor.h"
#include "text/nlsconfig.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"


// static const bool FRAMESELECTION_EDITS_DEFAULTSTYLE = false;



/**
 This class forwards change events for pages and pageitems to 
 the region occupied by this page or pageitem.
 */
class DocUpdater : public Observer<Page*>, public Observer<PageItem*> 
{
	ScribusDoc* doc;
	int  m_updateEnabled;
	bool m_docChangeNeeded;
public:
	DocUpdater(ScribusDoc* d) : doc(d), m_updateEnabled(0), m_docChangeNeeded(false) {}

	bool inUpdateSession() const
	{ 
		return m_updateEnabled > 0;
	}

	void beginUpdate(void)
	{ 
		if (m_updateEnabled == 0)
			m_docChangeNeeded = false;
		++m_updateEnabled;
	}

	void endUpdate(void)
	{
		--m_updateEnabled;
		if (m_updateEnabled <= 0)
		{
			if (m_docChangeNeeded)
			{
				doc->changed();
				m_docChangeNeeded = false;
			}
		}
	}
	
	void changed(Page* pg)
	{
		QRectF pagebox(pg->xOffset(), pg->yOffset(), pg->width(), pg->height());
		doc->invalidateRegion(pagebox);
		doc->regionsChanged()->update(pagebox);
		if (m_updateEnabled <= 0)
		{
			doc->changed();
			return;
		}
		m_docChangeNeeded = true;
	}
	
	void changed(PageItem* it)
	{
		it->invalidateLayout();
		doc->regionsChanged()->update(it->getVisualBoundingRect());
		if (m_updateEnabled <= 0)
		{
			doc->changed();
			return;
		}
		m_docChangeNeeded = true;
	}
};




ScribusDoc::ScribusDoc() : UndoObject( tr("Document")), Observable<ScribusDoc>(NULL),
	m_hasGUI(false),
	appPrefsData(PrefsManager::instance()->appPrefs),
	docPrefsData(PrefsManager::instance()->appPrefs),
	undoManager(UndoManager::instance()),
	loading(false),
	modified(false),
	ActiveLayer(0),
	rotMode(0),
	automaticTextFrames(0),
	m_masterPageMode(false),
	m_symbolEditMode(false),
	m_ScMW(0),
	m_View(0),
	m_guardedObject(this),
	m_serializer(NULL),
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGuides(false), GuideLock(false),
	minCanvasCoordinate(FPoint(0, 0)),
	rulerXoffset(0.0), rulerYoffset(0.0),
	Pages(0), MasterPages(), DocPages(),
	MasterNames(),
	Items(0), MasterItems(), DocItems(), FrameItems(),
	m_Selection(new Selection(this, true)),
	PageSp(1), PageSpa(0),
	FirstPnum(1),
	useRaster(false),
	PageColors(this, true),
	appMode(modeNormal),
	SubMode(-1),
	ShapeValues(0),
	ValCount(0),
	DocName( tr("Document")+"-"),
	UsedFonts(),
	AllFonts(&appPrefsData.fontPrefs.AvailFonts),
	AObjects(),
	CurrentSel(-1),
	nodeEdit(),
	LastAuto(0), FirstAuto(0),
	DraggedElem(0),
	ElemToLink(0),
	DragElements(),
	docParagraphStyles(),
	docCharStyles(),
	Layers(),
	GroupCounter(1),
	colorEngine(ScCore->defaultEngine),
	JavaScripts(),
	TotalItems(0),
	RePos(false),
	BookMarks(),
	OldBM(false),
	hasName(false),
	autoSaveTimer(new QTimer(this)),
	MLineStyles(),
	WinHan(0),
	DoDrawing(true),
	OpenNodes(),
	CurTimer(0),
	docLayerErrors(),
	docItemErrors(),
	masterItemErrors(),
	symReturn(), symNewLine(), symTab(), symNonBreak(), symNewCol(), symNewFrame(),
	docHyphenator(0),
	m_itemCreationTransaction(NULL),
	m_alignTransaction(NULL),
	m_currentPage(NULL),
	m_updateManager(),
	m_docUpdater(NULL)
{
	docUnitRatio=unitGetRatioFromIndex(docPrefsData.docSetupPrefs.docUnitIndex);
	docPrefsData.docSetupPrefs.pageHeight=0;
	docPrefsData.docSetupPrefs.pageWidth=0;
	docPrefsData.docSetupPrefs.pagePositioning=0;
	maxCanvasCoordinate=(FPoint(docPrefsData.displayPrefs.scratch.Left + docPrefsData.displayPrefs.scratch.Right, docPrefsData.displayPrefs.scratch.Top + docPrefsData.displayPrefs.scratch.Bottom)),
	init();
	docPrefsData.pdfPrefs.bleeds = docPrefsData.docSetupPrefs.bleeds;
	docPrefsData.pdfPrefs.useDocBleeds = true;
	Print_Options.firstUse = true;
	drawAsPreview = false;
	viewAsPreview = false;
	previewVisual = -1;
}


ScribusDoc::ScribusDoc(const QString& docName, int unitindex, const PageSize& pagesize, const MarginStruct& margins, const DocPagesSetup& pagesSetup) : UndoObject( tr("Document")),
	m_hasGUI(false),
	appPrefsData(PrefsManager::instance()->appPrefs),
	docPrefsData(PrefsManager::instance()->appPrefs),
	undoManager(UndoManager::instance()),
	loading(false),
	modified(false),
	ActiveLayer(0),
	docUnitRatio(unitGetRatioFromIndex(appPrefsData.docSetupPrefs.docUnitIndex)),
	rotMode(0),
	automaticTextFrames(pagesSetup.autoTextFrames),
	m_masterPageMode(false),
	m_symbolEditMode(false),
	m_ScMW(0),
	m_View(0),
	m_guardedObject(this),
	m_serializer(NULL),
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGuides(false), GuideLock(false),
	minCanvasCoordinate(FPoint(0, 0)),
	rulerXoffset(0.0), rulerYoffset(0.0),
	Pages(0), MasterPages(), DocPages(),
	MasterNames(),
	Items(0), MasterItems(), DocItems(), FrameItems(),
	m_Selection(new Selection(this, true)),
	PageSp(pagesSetup.columnCount), PageSpa(pagesSetup.columnDistance),
	FirstPnum(pagesSetup.firstPageNumber),
	useRaster(false),
	PageColors(this, true),
	appMode(modeNormal),
	SubMode(-1),
	ShapeValues(0),
	ValCount(0),
	DocName(docName),
	UsedFonts(),
	AllFonts(&appPrefsData.fontPrefs.AvailFonts),
	AObjects(),
	CurrentSel(-1),
	nodeEdit(),
	LastAuto(0), FirstAuto(0),
	DraggedElem(0),
	ElemToLink(0),
	DragElements(),
	docParagraphStyles(),
	docCharStyles(),
	Layers(),
	GroupCounter(1),
	colorEngine(ScCore->defaultEngine),
	JavaScripts(),
	TotalItems(0),
	RePos(false),
	BookMarks(),
	OldBM(false),
	hasName(false),
	autoSaveTimer(new QTimer(this)),
	MLineStyles(),
	WinHan(0),
	DoDrawing(true),
	OpenNodes(),
	CurTimer(0),
	docLayerErrors(),
	docItemErrors(),
	masterItemErrors(),
	symReturn(), symNewLine(), symTab(), symNonBreak(), symNewCol(), symNewFrame(),
	docHyphenator(0),
	m_itemCreationTransaction(NULL),
	m_alignTransaction(NULL),
	m_currentPage(NULL),
	m_updateManager(),
	m_docUpdater(NULL)
{
	docPrefsData.docSetupPrefs.docUnitIndex=unitindex;
	docPrefsData.docSetupPrefs.pageHeight=pagesize.height();
	docPrefsData.docSetupPrefs.pageWidth=pagesize.width();
	docPrefsData.docSetupPrefs.pageSize=pagesize.name();
	docPrefsData.docSetupPrefs.margins=margins;
	maxCanvasCoordinate=(FPoint(docPrefsData.displayPrefs.scratch.Left + docPrefsData.displayPrefs.scratch.Right, docPrefsData.displayPrefs.scratch.Top + docPrefsData.displayPrefs.scratch.Bottom)),
	setPageSetFirstPage(pagesSetup.pageArrangement, pagesSetup.firstPageLocation);
	init();
	docPrefsData.pdfPrefs.bleeds = docPrefsData.docSetupPrefs.bleeds;
	docPrefsData.pdfPrefs.useDocBleeds = true;
	docPrefsData.docSetupPrefs.pageOrientation=pagesSetup.orientation;
	docPrefsData.docSetupPrefs.pagePositioning=pagesSetup.pageArrangement;
	Print_Options.firstUse = true;
	drawAsPreview = false;
	viewAsPreview = false;
	previewVisual = -1;
}


void ScribusDoc::init()
{
	Q_CHECK_PTR(m_Selection);
	Q_CHECK_PTR(autoSaveTimer);

	HasCMS = false;
	docPrefsData.colorPrefs.DCMSset.CMSinUse = false;

	colorEngine = ScCore->defaultEngine;
	SetDefaultCMSParams();

	// init update change management
	setUpdateManager(&m_updateManager);
	m_itemsChanged.setUpdateManager(&m_updateManager);
	m_pagesChanged.setUpdateManager(&m_updateManager);
	m_regionsChanged.setUpdateManager(&m_updateManager);
	nodeEdit.setUpdateManager(&m_updateManager);
	
	// setup update() event chains
	m_docUpdater = new DocUpdater(this);
	m_itemsChanged.connectObserver(m_docUpdater);
	m_pagesChanged.connectObserver(m_docUpdater);

	PrefsManager *prefsManager = PrefsManager::instance();
	docPrefsData.colorPrefs.DCMSset = prefsManager->appPrefs.colorPrefs.DCMSset;
	docPrefsData.pdfPrefs.SolidProf = docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
	docPrefsData.pdfPrefs.ImageProf = docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
	docPrefsData.pdfPrefs.PrintProf = docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
	docPrefsData.pdfPrefs.Intent = docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	docPrefsData.pdfPrefs.Intent2 = docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;

	AddFont(appPrefsData.itemToolPrefs.textFont);//, prefsData.AvailFonts[prefsData.itemToolPrefs.textFont]->Font);
	//FIXME: aren't we doing this now anyway with prefs struct copy?
	docPrefsData.itemToolPrefs.textFont = appPrefsData.itemToolPrefs.textFont;
	docPrefsData.itemToolPrefs.textSize = appPrefsData.itemToolPrefs.textSize;
	docPrefsData.itemToolPrefs.textTabFillChar = appPrefsData.itemToolPrefs.textTabFillChar;
	docPrefsData.opToolPrefs.dispX = appPrefsData.opToolPrefs.dispX;
	docPrefsData.opToolPrefs.dispY = appPrefsData.opToolPrefs.dispY;
	docPrefsData.opToolPrefs.constrain = appPrefsData.opToolPrefs.constrain;

	PageColors.ensureDefaultColors();
	if (appPrefsData.itemToolPrefs.shapeLineColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.shapeLineColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.shapeLineColor]);
	docPrefsData.itemToolPrefs.shapeLineColor = appPrefsData.itemToolPrefs.shapeLineColor;
	if (appPrefsData.itemToolPrefs.lineColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.lineColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.lineColor]);
	docPrefsData.itemToolPrefs.lineColor = appPrefsData.itemToolPrefs.lineColor;
	if (appPrefsData.itemToolPrefs.textColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.textColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.textColor]);
	docPrefsData.itemToolPrefs.textColor = appPrefsData.itemToolPrefs.textColor;
	if (appPrefsData.itemToolPrefs.textStrokeColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.textStrokeColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.textStrokeColor]);
	docPrefsData.itemToolPrefs.textStrokeColor = appPrefsData.itemToolPrefs.textStrokeColor;
	if (appPrefsData.itemToolPrefs.shapeFillColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.shapeFillColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.shapeFillColor]);
	docPrefsData.itemToolPrefs.shapeFillColor = appPrefsData.itemToolPrefs.shapeFillColor;
	if (appPrefsData.itemToolPrefs.imageFillColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.imageFillColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.imageFillColor]);
	docPrefsData.itemToolPrefs.imageFillColor = appPrefsData.itemToolPrefs.imageFillColor;
	if (appPrefsData.itemToolPrefs.textFillColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.textFillColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.textFillColor]);
	docPrefsData.itemToolPrefs.textFillColor = appPrefsData.itemToolPrefs.textFillColor;
	if (appPrefsData.itemToolPrefs.textLineColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.textLineColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.textLineColor]);
	if (appPrefsData.itemToolPrefs.calligrapicPenFillColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.calligrapicPenFillColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.calligrapicPenFillColor]);
	if (appPrefsData.itemToolPrefs.calligrapicPenLineColor != CommonStrings::None)
		PageColors.insert(appPrefsData.itemToolPrefs.calligrapicPenLineColor, appPrefsData.colorPrefs.DColors[appPrefsData.itemToolPrefs.calligrapicPenLineColor]);

	
	ParagraphStyle pstyle;
	pstyle.setDefaultStyle(true);
	pstyle.setName(CommonStrings::DefaultParagraphStyle);
	pstyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
	pstyle.setLineSpacing(15);
	pstyle.setAlignment(ParagraphStyle::Leftaligned);
	pstyle.setLeftMargin(0);
	pstyle.setFirstIndent(0);
	pstyle.setRightMargin(0);
	pstyle.setGapBefore(0);
	pstyle.setGapAfter(0);
	pstyle.setHasDropCap(false);
	pstyle.setDropCapLines(2);
	pstyle.setDropCapOffset(0);
	pstyle.charStyle().setParent("");
	
	CharStyle cstyle;
	cstyle.setDefaultStyle(true);
	cstyle.setName(CommonStrings::DefaultCharacterStyle);
	cstyle.setFont(appPrefsData.fontPrefs.AvailFonts[docPrefsData.itemToolPrefs.textFont]);
	cstyle.setFontSize(docPrefsData.itemToolPrefs.textSize);
	cstyle.setFeatures(QStringList(CharStyle::INHERIT));
	cstyle.setFillColor(docPrefsData.itemToolPrefs.textColor);
	cstyle.setFillShade(docPrefsData.itemToolPrefs.textShade);
	cstyle.setStrokeColor(docPrefsData.itemToolPrefs.textStrokeColor);
	cstyle.setStrokeShade(docPrefsData.itemToolPrefs.textStrokeShade);
	cstyle.setBaselineOffset(0);
	cstyle.setShadowXOffset(50);
	cstyle.setShadowYOffset(-50);
	cstyle.setOutlineWidth(10);
	cstyle.setUnderlineOffset(docPrefsData.typoPrefs.valueUnderlinePos);
	cstyle.setUnderlineWidth(docPrefsData.typoPrefs.valueUnderlineWidth);
	cstyle.setStrikethruOffset(docPrefsData.typoPrefs.valueStrikeThruPos);
	cstyle.setStrikethruWidth(docPrefsData.typoPrefs.valueStrikeThruPos);
	cstyle.setScaleH(1000);
	cstyle.setScaleV(1000);
	cstyle.setTracking(0);
	cstyle.setLanguage(PrefsManager::instance()->appPrefs.hyphPrefs.Language);
	
	docParagraphStyles.create(pstyle);
	docParagraphStyles.makeDefault( &(docParagraphStyles[0]) );
	
	docCharStyles.create(cstyle);
	docCharStyles.makeDefault( &(docCharStyles[0]) );
	
	docParagraphStyles[0].breakImplicitCharStyleInheritance();
	docParagraphStyles[0].charStyle().setContext( & docCharStyles );
//	docParagraphStyles[0].charStyle().setName( "cdocdefault" ); // DONT TRANSLATE

	currentStyle = pstyle;
	
	Layers.addLayer( tr("Background") );
	// FIXME: Check PDF version input
	//TODO: Check if this is needed now we ue appPrefsData --> docPrefsData
	pdfOptions().Version = (PDFOptions::PDFVersion)appPrefsData.pdfPrefs.Version;

	pdfOptions().firstUse = true;
	docPatterns.clear();
	docGradients.clear();

	if (autoSave() && ScCore->usingGUI())
		autoSaveTimer->start(autoSaveTime());
	//Do this after all the collections have been created and cleared!
	m_masterPageMode=true; // quick hack to force the change of pointers in setMasterPageMode();
	setMasterPageMode(false);
	addSymbols();
}


ScribusDoc::~ScribusDoc()
{
	m_guardedObject.nullify();
	CloseCMSProfiles();
	ScCore->fileWatcher->stop();
	ScCore->fileWatcher->removeFile(DocName);
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
			if ((currItem->asImageFrame()) && (!currItem->Pfile.isEmpty()))
			{
				QFileInfo fi(currItem->Pfile);
				ScCore->fileWatcher->removeDir(fi.absolutePath());
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
			if ((currItem->asImageFrame()) && (!currItem->Pfile.isEmpty()))
			{
				QFileInfo fi(currItem->Pfile);
				ScCore->fileWatcher->removeDir(fi.absolutePath());
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
			if ((currItem->asImageFrame()) && (!currItem->Pfile.isEmpty()))
			{
				QFileInfo fi(currItem->Pfile);
				ScCore->fileWatcher->removeDir(fi.absolutePath());
			}
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					if ((currItem->PictureIsAvailable) && (!currItem->Pfile.isEmpty()))
					{
						ScCore->fileWatcher->removeFile(currItem->Pfile);
						QFileInfo fi(currItem->Pfile);
						ScCore->fileWatcher->removeDir(fi.absolutePath());
					}
				}
			}
			allItems.clear();
		}
		while (!pa.items.isEmpty())
		{
			delete pa.items.takeFirst();
		}
	}
	docPatterns.clear();
	docGradients.clear();
	while (!DocItems.isEmpty())
	{
		delete DocItems.takeFirst();
	}
	while (!FrameItems.isEmpty())
	{
		delete FrameItems.takeFirst();
	}
//	FrameItems.clear();

	while (!MasterPages.isEmpty())
	{
		delete MasterPages.takeFirst();
	}
	while (!DocPages.isEmpty())
	{
		delete DocPages.takeFirst();
	}
	while (!MasterItems.isEmpty())
	{
		delete MasterItems.takeFirst();
	}
	QMap<QString,int>::Iterator it3;
	for (it3 = UsedFonts.begin(); it3 != UsedFonts.end(); ++it3)
	{
		if (!(*AllFonts)[it3.key()].localForDocument().isEmpty())
			(*AllFonts).removeFont(it3.key());
		else
			(*AllFonts)[it3.key()].decreaseUsage();
	}
	if (docHyphenator)
		delete docHyphenator;
	if (m_serializer)
		delete m_serializer;
	ScCore->fileWatcher->start();
}

QList<PageItem*> ScribusDoc::getAllItems(QList<PageItem*> &items)
{
	QList<PageItem*> ret;
	for (int em = 0; em < items.count(); ++em)
	{
		PageItem* embedded = items.at(em);
		ret.append(embedded);
		if (embedded->isGroup())
			ret += embedded->getItemList();
	}
	return ret;
}

void ScribusDoc::setup(const int unitIndex, const int fp, const int firstLeft, const int orientation, const int firstPageNumber, const QString& defaultPageSize, const QString& documentName)
{
	docPrefsData.docSetupPrefs.docUnitIndex=unitIndex;
	setPageSetFirstPage(fp, firstLeft);
	docPrefsData.docSetupPrefs.pageOrientation = orientation;
	docPrefsData.docSetupPrefs.pageSize = defaultPageSize;
	FirstPnum = firstPageNumber;
	docPrefsData.docSetupPrefs.pagePositioning = fp;
	setName(documentName);
	HasCMS = false;
	if (!pdfOptions().UseLPI)
	{
		pdfOptions().LPISettings.clear();
		struct LPIData lpo;
		lpo.Frequency = 133;
		lpo.SpotFunc = 3;
		lpo.Angle = 105;
		pdfOptions().LPISettings.insert("Cyan", lpo);
		lpo.Angle = 75;
		pdfOptions().LPISettings.insert("Magenta", lpo);
		lpo.Angle = 90;
		pdfOptions().LPISettings.insert("Yellow", lpo);
		lpo.Angle = 45;
		pdfOptions().LPISettings.insert("Black", lpo);
		ActiveLayer = 0;
	}

	appMode = modeNormal;
	PrefsManager *prefsManager=PrefsManager::instance();

	docPrefsData.colorPrefs.DCMSset = prefsManager->appPrefs.colorPrefs.DCMSset;
	docPrefsData.pdfPrefs.SolidProf = docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
	docPrefsData.pdfPrefs.ImageProf = docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
	docPrefsData.pdfPrefs.PrintProf = docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
	docPrefsData.pdfPrefs.Intent = docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	docPrefsData.pdfPrefs.Intent2 = docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
	BlackPoint   = docPrefsData.colorPrefs.DCMSset.BlackPoint;
	SoftProofing = docPrefsData.colorPrefs.DCMSset.SoftProofOn;
	Gamut        = docPrefsData.colorPrefs.DCMSset.GamutCheck;
	IntentColors = docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	IntentImages = docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
	if (ScCore->haveCMS() && docPrefsData.colorPrefs.DCMSset.CMSinUse)
	{
		if (OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles))
		{
			HasCMS = true;
			pdfOptions().SComp = docPrefsData.colorPrefs.DCMSset.ComponentsInput2;
		}
		else
		{
			SetDefaultCMSParams();
			HasCMS = false;
		}
	}
}


void ScribusDoc::setGUI(bool hasgui, ScribusMainWindow* mw, ScribusView* view)
{
	m_hasGUI = hasgui;
	m_ScMW = mw;
	//FIXME: stop using m_View
	m_View = view;
	if (m_hasGUI)
	{
		docHyphenator = new Hyphenator(m_ScMW, this);
		Q_CHECK_PTR(docHyphenator);
	}
	else
		docHyphenator = NULL;
}


void ScribusDoc::setLoading(const bool docLoading)
{
	loading = docLoading;
}


bool ScribusDoc::isLoading() const
{
	return loading;
}


//AV: eeks. That should not be used
ScribusView* ScribusDoc::view() const
{
// 	return (WinHan ? (((ScribusWin*) WinHan)->view()) : 0);
	return m_View;
}


const ScGuardedPtr<ScribusDoc>& ScribusDoc::guardedPtr() const
{
	return m_guardedObject;
}


void ScribusDoc::CloseCMSProfiles()
{
	HasCMS = false;
	SetDefaultCMSParams();
}

void ScribusDoc::SetDefaultCMSParams()
{
	BlackPoint     = true;
	SoftProofing   = false;
	Gamut          = false;
	IntentColors   = Intent_Relative_Colorimetric;
	IntentImages   = Intent_Relative_Colorimetric;
	DocInputRGBProf       = ScCore->defaultRGBProfile;
	DocInputCMYKProf      = ScCore->defaultCMYKProfile;
	DocInputImageRGBProf  = ScCore->defaultRGBProfile;
	DocInputImageCMYKProf = ScCore->defaultCMYKProfile;
	DocDisplayProf        = ScCore->defaultRGBProfile;
	DocPrinterProf        = ScCore->defaultCMYKProfile;
	stdTransRGBMon        = ScCore->defaultRGBToScreenSolidTrans;
	stdTransCMYKMon       = ScCore->defaultCMYKToRGBTrans;
	stdTransRGB           = ScCore->defaultCMYKToRGBTrans;
	stdTransCMYK          = ScCore->defaultRGBToCMYKTrans;
	stdProof              = ScCore->defaultRGBToScreenSolidTrans;
	stdProofGC            = ScCore->defaultRGBToScreenSolidTrans;
	stdProofCMYK          = ScCore->defaultCMYKToRGBTrans;
	stdProofCMYKGC        = ScCore->defaultCMYKToRGBTrans;
	stdTransImg           = ScCore->defaultRGBToScreenImageTrans;
	stdProofImg           = ScCore->defaultRGBToScreenImageTrans;
	stdProofImgCMYK       = ScCore->defaultCMYKToScreenImageTrans;
}

bool ScribusDoc::OpenCMSProfiles(ProfilesL InPo, ProfilesL InPoCMYK, ProfilesL MoPo, ProfilesL PrPo)
{
	HasCMS = false;
	ScColorProfile inputProf;

	colorEngine = colorMgmtEngineFactory.createDefaultEngine();
	ScColorMgmtStrategy colorStrategy;
	colorStrategy.useBlackPointCompensation = docPrefsData.colorPrefs.DCMSset.BlackPoint;
	colorStrategy.useBlackPreservation      = false;
	colorEngine.setStrategy(colorStrategy);

	DocDisplayProf   = ScCore->monitorProfile;
	DocInputRGBProf  = colorEngine.openProfileFromFile( InPo[docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile] );
	DocInputCMYKProf = colorEngine.openProfileFromFile( InPoCMYK[docPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile] );
	DocPrinterProf   = colorEngine.openProfileFromFile( PrPo[docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile] );
	DocInputImageRGBProf  = colorEngine.openProfileFromFile( InPo[docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile] );
	DocInputImageCMYKProf = colorEngine.openProfileFromFile( InPoCMYK[docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile] );
	if ((!DocDisplayProf) || (!DocInputRGBProf) || (!DocInputCMYKProf) || (!DocPrinterProf) || (!DocInputImageCMYKProf) || (!DocInputImageRGBProf))
	{
		docPrefsData.colorPrefs.DCMSset.CMSinUse = false;
		return false;
	}

	int dcmsFlags   = 0;
	int dcmsFlagsGC = 0;
	if (docPrefsData.colorPrefs.DCMSset.GamutCheck)
		dcmsFlagsGC |= Ctf_GamutCheck;
	if (docPrefsData.colorPrefs.DCMSset.BlackPoint)
	{
		dcmsFlags   |= Ctf_BlackPointCompensation;
		dcmsFlagsGC |= Ctf_BlackPointCompensation;
	}
	stdTransRGBMon  = colorEngine.createTransform(DocInputRGBProf, Format_RGB_16,
										DocDisplayProf, Format_RGB_16,
										IntentColors, dcmsFlags);
	stdTransCMYKMon = colorEngine.createTransform(DocInputCMYKProf, Format_CMYK_16,
										DocDisplayProf, Format_RGB_16,
										IntentColors, dcmsFlags);
	// TODO : check input profiles used for images
	stdProofImg = colorEngine.createProofingTransform(DocInputImageRGBProf, Format_BGRA_8,
	                  DocDisplayProf, Format_BGRA_8, DocPrinterProf,
	                  IntentImages, Intent_Relative_Colorimetric, dcmsFlagsGC);
	stdProofImgCMYK = colorEngine.createProofingTransform(DocInputImageCMYKProf, Format_CMYK_8,
	                  DocDisplayProf, Format_BGRA_8, DocPrinterProf,
	                  IntentImages, Intent_Relative_Colorimetric, dcmsFlagsGC);
	stdTransImg = colorEngine.createTransform(DocInputRGBProf, Format_BGRA_8,
	                  DocDisplayProf, Format_BGRA_8, 
					  IntentImages, dcmsFlags);
	stdTransRGB = colorEngine.createTransform(DocInputCMYKProf, Format_CMYK_16,
					  DocInputRGBProf, Format_RGB_16,
					  IntentColors, dcmsFlags);
	stdTransCMYK = colorEngine.createTransform(DocInputRGBProf, Format_RGB_16,
					  DocInputCMYKProf, Format_CMYK_16,
					  IntentColors, dcmsFlags);

	ScColorProfile inputProfRGB;
	ScColorProfile inputProfCMYK;
	if (DocPrinterProf.colorSpace() == ColorSpace_Cmyk)
	{
		inputProf = (docPrefsData.colorPrefs.DCMSset.SoftProofOn && docPrefsData.colorPrefs.DCMSset.SoftProofFullOn) ? DocInputCMYKProf : DocPrinterProf;
		inputProfRGB  = DocInputRGBProf;
		inputProfCMYK = inputProf;
	}
	else
	{
		inputProf = (docPrefsData.colorPrefs.DCMSset.SoftProofOn && docPrefsData.colorPrefs.DCMSset.SoftProofFullOn) ? DocInputRGBProf : DocPrinterProf;
		inputProfRGB  = inputProf;
		inputProfCMYK = DocInputCMYKProf;
	}
	stdProof = colorEngine.createProofingTransform(inputProfRGB, Format_RGB_16,
	                    DocDisplayProf, Format_RGB_16,
	                    DocPrinterProf,
	                    IntentColors,
	                    Intent_Relative_Colorimetric, dcmsFlags);
	stdProofGC = colorEngine.createProofingTransform(inputProfRGB, Format_RGB_16,
	                    DocDisplayProf, Format_RGB_16,
	                    DocPrinterProf, IntentColors,
	                    Intent_Relative_Colorimetric, dcmsFlags| Ctf_GamutCheck);
	stdProofCMYK = colorEngine.createProofingTransform(inputProfCMYK, Format_CMYK_16,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf, IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags);
	stdProofCMYKGC = colorEngine.createProofingTransform(inputProfCMYK, Format_CMYK_16,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf,
						IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags | Ctf_GamutCheck);

	if (DocInputRGBProf.colorSpace() == ColorSpace_Rgb)
			docPrefsData.colorPrefs.DCMSset.ComponentsInput2 = 3;
	if (DocInputRGBProf.colorSpace() == ColorSpace_Cmyk)
			docPrefsData.colorPrefs.DCMSset.ComponentsInput2 = 4;
	if (DocInputRGBProf.colorSpace() == ColorSpace_Cmy)
			docPrefsData.colorPrefs.DCMSset.ComponentsInput2 = 3;
	if (DocInputCMYKProf.colorSpace() == ColorSpace_Rgb)
			docPrefsData.colorPrefs.DCMSset.ComponentsInput3 = 3;
	if (DocInputCMYKProf.colorSpace() == ColorSpace_Cmyk)
			docPrefsData.colorPrefs.DCMSset.ComponentsInput3 = 4;
	if (DocInputCMYKProf.colorSpace() == ColorSpace_Cmy)
			docPrefsData.colorPrefs.DCMSset.ComponentsInput3 = 3;
	if (DocPrinterProf.colorSpace() == ColorSpace_Rgb)
			docPrefsData.colorPrefs.DCMSset.ComponentsPrinter = 3;
	if (DocPrinterProf.colorSpace() == ColorSpace_Cmyk)
			docPrefsData.colorPrefs.DCMSset.ComponentsPrinter = 4;
	if (DocPrinterProf.colorSpace() == ColorSpace_Cmy)
			docPrefsData.colorPrefs.DCMSset.ComponentsPrinter = 3;

	bool success  = (stdTransRGBMon && stdTransCMYKMon && stdProofImg  && stdProofImgCMYK &&
		             stdTransImg    && stdTransRGB     && stdTransCMYK && stdProof       &&
	                 stdProofGC     && stdProofCMYK    && stdProofCMYKGC);
	if (!success)
	{
		CloseCMSProfiles();
		docPrefsData.colorPrefs.DCMSset.CMSinUse = false;
		QString message = tr("An error occurred while opening ICC profiles, color management is not enabled." );
		if (ScCore->usingGUI())
			QMessageBox::warning(m_ScMW, CommonStrings::trWarning, message, QMessageBox::Ok, 0, 0);
		else
			qWarning( "%s", message.toLocal8Bit().data() );
	}
	return true;
}


void ScribusDoc::enableCMS(bool enable)
{
	m_ScMW->setStatusBarInfoText( tr("Adjusting Colors"));
	m_ScMW->mainWindowProgressBar->reset();
	int cc = PageColors.count() + MasterItems.count() + DocItems.count();
	m_ScMW->mainWindowProgressBar->setMaximum(cc);
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	bool oldCM = docPrefsData.colorPrefs.DCMSset.CMSinUse;
	bool newCM = enable;
	CloseCMSProfiles();
	docPrefsData.colorPrefs.DCMSset.CMSinUse = newCM;
	HasCMS       = docPrefsData.colorPrefs.DCMSset.CMSinUse;
	BlackPoint   = docPrefsData.colorPrefs.DCMSset.BlackPoint;
	SoftProofing = docPrefsData.colorPrefs.DCMSset.SoftProofOn;
	Gamut        = docPrefsData.colorPrefs.DCMSset.GamutCheck;
	IntentColors = docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	IntentImages = docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
	if (!docPrefsData.colorPrefs.DCMSset.CMSinUse)
	{
		HasCMS = false;
		if	(oldCM)
		{
			m_ScMW->recalcColors(m_ScMW->mainWindowProgressBar);
			RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, m_ScMW->mainWindowProgressBar);
		}
	}
	else if (OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles) )
	{
		HasCMS = true;
		docPrefsData.pdfPrefs.SComp = docPrefsData.colorPrefs.DCMSset.ComponentsInput2;
		docPrefsData.pdfPrefs.SolidProf = docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
		docPrefsData.pdfPrefs.ImageProf = docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
		docPrefsData.pdfPrefs.PrintProf = docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
		docPrefsData.pdfPrefs.Intent = docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
		m_ScMW->recalcColors(m_ScMW->mainWindowProgressBar);
		RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, m_ScMW->mainWindowProgressBar);
	}
	else
	{
		SetDefaultCMSParams();
		HasCMS = false;
	}
	m_ScMW->mainWindowProgressBar->setValue(cc);
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	m_ScMW->setStatusBarInfoText("");
	m_ScMW->mainWindowProgressBar->reset();
}


void ScribusDoc::getNamedResources(ResourceCollection& lists) const
{
	lists.availableFonts = AllFonts;
	lists.availableColors = const_cast<ColorList*>(& PageColors);

	const QList<PageItem*> * itemlist = & MasterItems;
	while (itemlist != NULL)
	{
		for (int i=0; i < itemlist->count(); ++i)
		{
			const PageItem * currItem = const_cast<QList<PageItem*>*>(itemlist)->at(i);
			if (currItem)
				currItem->getNamedResources(lists);
		}
		if (itemlist == &MasterItems)
			itemlist = &DocItems;
		else if (itemlist == &DocItems)
			itemlist = &FrameItems;
		else
			itemlist = NULL;
	}
	for (int i = 0; i < docParagraphStyles.count(); ++i)
		docParagraphStyles[i].getNamedResources(lists);
	for (int i = 0; i < docCharStyles.count(); ++i)
		docCharStyles[i].getNamedResources(lists);
//	for (uint i = 0; i < docLineStyles.count(); ++i)
//		docLineStyles[i].getNamedResources(lists);
	
	QMap<QString,ScPattern>::ConstIterator it;
	for (it = docPatterns.begin(); it != docPatterns.end(); ++it)
	{
		ScPattern pa = *it;
		for (int o = 0; o < pa.items.count(); o++)
		{
			pa.items.at(o)->getNamedResources(lists);
		}
	}
	QMap<QString,VGradient>::ConstIterator itg;
	for (itg = docGradients.begin(); itg != docGradients.end(); ++itg)
	{
		QList<VColorStop*> cstops = itg.value().colorStops();
		for (uint cst = 0; cst < itg.value().Stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
}

bool ScribusDoc::styleExists(QString styleName)
{
	for (int ff = 0; ff < paragraphStyles().count(); ++ff)
	{
		if (paragraphStyles()[ff].name() == styleName)
			return true;
	}
	return false;
}

QList<int> ScribusDoc::getSortedStyleList()
{
	QList<int> retList;
	for (int ff = 0; ff < paragraphStyles().count(); ++ff)
	{
		if (paragraphStyles()[ff].parent().isEmpty())
		{
			if (!retList.contains(ff))
				retList.append(ff);
		}
		else
		{
			QList<int> retList2;
			QString par = paragraphStyles()[ff].parent();
			retList2.prepend(ff);
			while (!par.isEmpty())
			{
				for (int pp = 0; pp < paragraphStyles().count(); ++pp)
				{
					if (paragraphStyles()[pp].name() == par)
					{
						if (!retList2.contains(pp))
							retList2.prepend(pp);
						par = paragraphStyles()[pp].parent();
					}
				}
			}
			for (int r = 0; r < retList2.count(); ++r)
			{
				if (!retList.contains(retList2[r]))
					retList.append(retList2[r]);
			}
		}
	}
	return retList;
}

QList<int> ScribusDoc::getSortedCharStyleList()
{
	QList<int> retList;
	for (int ff = 0; ff < docCharStyles.count(); ++ff)
	{
		if (docCharStyles[ff].parent().isEmpty())
		{
			if (!retList.contains(ff))
				retList.append(ff);
		}
		else
		{
			QList<int> retList2;
			QString par = docCharStyles[ff].parent();
			retList2.prepend(ff);
			while (!par.isEmpty())
			{
				for (int pp = 0; pp < docCharStyles.count(); ++pp)
				{
					if (docCharStyles[pp].name() == par)
					{
						if (!retList2.contains(pp))
							retList2.prepend(pp);
						par = docCharStyles[pp].parent();
					}
				}
			}
			for (int r = 0; r < retList2.count(); ++r)
			{
				if (!retList.contains(retList2[r]))
					retList.append(retList2[r]);
			}
		}
	}
	return retList;
}

void ScribusDoc::replaceStyles(const QMap<QString,QString>& newNameForOld)
{
	ResourceCollection newNames;
	newNames.mapStyles(newNameForOld);
	replaceNamedResources(newNames);
}


void ScribusDoc::replaceNamedResources(ResourceCollection& newNames)
{
	// replace names in items
	QList<PageItem*> * itemlist = & MasterItems;
	while (itemlist != NULL)
	{
		for (int i=0; i < itemlist->count(); ++i)
		{
			PageItem * currItem = itemlist->at(i);
			if (currItem)
				currItem->replaceNamedResources(newNames);
		}
		if (itemlist == &MasterItems)
			itemlist = &DocItems;
		else if (itemlist == &DocItems)
			itemlist = &FrameItems;
		else
			itemlist = NULL;
	}
	
	// replace names in styles...
	for (int i=docParagraphStyles.count()-1; i >= 0; --i)
	{
		if (newNames.styles().contains(docParagraphStyles[i].name()))
			docParagraphStyles.remove(i);
		else
			docParagraphStyles[i].replaceNamedResources(newNames);
	}
	for (int i=docCharStyles.count()-1; i >= 0; --i)
	{
		if (newNames.charStyles().contains(docCharStyles[i].name()))
			docCharStyles.remove(i);
		else
			docCharStyles[i].replaceNamedResources(newNames);
	}

	QMap<QString,ScPattern>::Iterator it;
	for (it = docPatterns.begin(); it != docPatterns.end(); ++it)
	{
		if (newNames.patterns().contains(it.key()))
			docPatterns.erase(it);
		else
		{
			ScPattern pa = *it;
			for (int o = 0; o < pa.items.count(); o++)
			{
				pa.items.at(o)->replaceNamedResources(newNames);
			}
		}
	}

	QMap<QString,VGradient>::Iterator itg;
	for (itg = docGradients.begin(); itg != docGradients.end(); ++itg)
	{
		if (newNames.gradients().contains(itg.key()))
			docGradients.erase(itg);
		else
		{
			QMap<QString,QString>::ConstIterator itc;
			QList<VColorStop*> cstops = itg.value().colorStops();
			for (uint cst = 0; cst < itg.value().Stops(); ++cst)
			{
				itc = newNames.colors().find(cstops.at(cst)->name);
				if (itc != newNames.colors().end())
				{
					if (*itc != CommonStrings::None)
						cstops.at(cst)->name = *itc;
				}
			}
		}
	}
	
	if (newNames.colors().count() > 0 || newNames.fonts().count() > 0)
	{
		docCharStyles.invalidate();
		docParagraphStyles.invalidate();	
	}
	else
	{
		if (newNames.charStyles().count() > 0)
			docCharStyles.invalidate();
		if (newNames.styles().count() > 0)
			docParagraphStyles.invalidate();	
	}
	if (!isLoading() && !(newNames.colors().isEmpty() && newNames.fonts().isEmpty() && newNames.patterns().isEmpty() 
						  && newNames.styles().isEmpty() && newNames.charStyles().isEmpty() && newNames.lineStyles().isEmpty()) )
		changed();
}


void ScribusDoc::replaceCharStyles(const QMap<QString,QString>& newNameForOld)
{
	ResourceCollection newNames;
	newNames.mapCharStyles(newNameForOld);
	replaceNamedResources(newNames);
	
	/*
	// replace style in items
	QPtrList<PageItem> * itemlist = & MasterItems;
	while (itemlist != NULL)
	{
		for (uint i=0; i < itemlist->count(); ++i)
		{
			PageItem_TextFrame * currItem = itemlist->at(i)->asTextFrame();
			if (currItem)
				currItem->itemText.replaceCharStyles(newNameForOld);
		}
		if (itemlist == &MasterItems)
			itemlist = &DocItems;
		else if (itemlist == &DocItems)
			itemlist = &FrameItems;
		else
			itemlist = NULL;
	}
	// replace names in styles 
	for (uint i=0; i < docParagraphStyles.count(); ++i)
	{
		// ...parent of parstyle's charstyle
		const QString& parent(docParagraphStyles[i].charStyle().parent());
		if (newNameForOld.contains(parent))
			docParagraphStyles[i].charStyle().setParent(newNameForOld[parent]);
	}
	for (int i=docCharStyles.count()-1; i >= 0; --i)
	{
		// ...parent of charstyle
		const QString& parent(docCharStyles[i].parent());
		if (newNameForOld.contains(parent))
			docCharStyles[i].setParent(newNameForOld[parent]);
		// ... as name
		if (newNameForOld.contains(docCharStyles[i].name()))
			docCharStyles.remove(i);
	}
	*/
}


void ScribusDoc::redefineStyles(const StyleSet<ParagraphStyle>& newStyles, bool removeUnused)
{
	docParagraphStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (int i=0; i < docParagraphStyles.count(); ++i)
		{
			const QString& nam(docParagraphStyles[i].name());
			if (newStyles.find(nam) < 0)
				deletion[nam] = deflt;
		}
		if (deletion.count() > 0)
			replaceStyles(deletion);
	}
	// repair charstyle context:
	for (int i=0; i < docParagraphStyles.count(); ++i)
	{
		ParagraphStyle& sty(docParagraphStyles[i]);
		if (docParagraphStyles.isDefault(sty))
		{
			sty.breakImplicitCharStyleInheritance(true);
			sty.charStyle().setContext( & docCharStyles );
//			sty.charStyle().setName( "cdocdefault" ); // DONT TRANSLATE
		}
		else {
			sty.breakImplicitCharStyleInheritance(false);
		}
	}
	docParagraphStyles.invalidate();
}

void ScribusDoc::redefineCharStyles(const StyleSet<CharStyle>& newStyles, bool removeUnused)
{
	docCharStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (int i=0; i < docCharStyles.count(); ++i)
		{
			const QString& nam(docCharStyles[i].name());
			if (newStyles.find(nam) < 0)
			{
				deletion[nam] = deflt;
			}
		}
		if (deletion.count() > 0)
			replaceCharStyles(deletion);
	}
	docCharStyles.invalidate();
}


/*
 * Split out from loadStyles in editFormats.cpp so it's callable from anywhere,
 * including plugins.
 * - 2004-09-14 Craig Ringer
 */
// dont like this here. could as well be a static method for reading this stuff into temp., then always use redefineXY() - av
void ScribusDoc::loadStylesFromFile(QString fileName)
{
	StyleSet<ParagraphStyle> *wrkStyles     = &docParagraphStyles;
	StyleSet<CharStyle> *wrkCharStyles      = &docCharStyles;
	QMap<QString, multiLine> *wrkLineStyles = &MLineStyles;
	
	int oldStyles = wrkStyles->count();
	int oldCharStyles = wrkCharStyles->count();
	int oldLineStyles = wrkLineStyles->count();
	
	if (!fileName.isEmpty())
	{
		FileLoader fl(fileName);
		if (fl.TestFile() == -1)
		//TODO put in nice user warning
			return;

		if (!fl.ReadStyles(fileName, this, *wrkStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.ReadCharStyles(fileName, this, *wrkCharStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.ReadLineStyles(fileName, wrkLineStyles))
		{
			//TODO put in nice user warning
		}
		
		if ( !isLoading() && ((wrkStyles->count() > oldStyles)
				    || (wrkCharStyles->count() > oldCharStyles)
				    || (wrkLineStyles->count() > oldLineStyles) ) )
			changed();
	}
}

void ScribusDoc::loadStylesFromFile(QString fileName, StyleSet<ParagraphStyle> *tempStyles,
                                                      StyleSet<CharStyle> *tempCharStyles,
                                                      QMap<QString, multiLine> *tempLineStyles)
{
	StyleSet<ParagraphStyle> *wrkStyles     = tempStyles;
	StyleSet<CharStyle> *wrkCharStyles      = tempCharStyles;
	QMap<QString, multiLine> *wrkLineStyles = tempLineStyles;
	
	if (!fileName.isEmpty())
	{
		FileLoader fl(fileName);
		if (fl.TestFile() == -1)
		//TODO put in nice user warning
			return;

		if (!fl.ReadStyles(fileName, this, *wrkStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.ReadCharStyles(fileName, this, *wrkCharStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.ReadLineStyles(fileName, wrkLineStyles))
		{
			//TODO put in nice user warning
		}
		
		// Which are the default styles 
		// Sadly StyleSet is not updated at import
		// and it might break something to do so. 
		// We need to loop then - pm
// 		ParagraphStyle* parDefault(wrkStyles->getDefault());
// 		CharStyle* charDefault(wrkCharStyles->getDefault());
				
		QFileInfo fi(fileName);
		QString importPrefix(tr("Imported ","Prefix of imported default style"));
		QString importSuffix(" (" +  fi.baseName() + ")");
		
		for(int j(0) ; j < wrkStyles->count() ; ++j)
		{
			if((*wrkStyles)[j].isDefaultStyle())
			{
				ParagraphStyle& parDefault((*wrkStyles)[j]);
				parDefault.setDefaultStyle(false);
				QMap<QString, QString> namesMap;
				namesMap[parDefault.name()] = importPrefix + parDefault.name() + importSuffix;
				for(int i(0) ; i < wrkStyles->count() ; ++i)
				{
					if( (*wrkStyles)[i] != parDefault )
						namesMap[(*wrkStyles)[i].name()] = (*wrkStyles)[i].name();
				}
				wrkStyles->rename(namesMap);
			}
		}
		for(int j(0) ; j < wrkCharStyles->count() ; ++j)
		{
			if((*wrkCharStyles)[j].isDefaultStyle())
			{
				CharStyle& charDefault((*wrkCharStyles)[j]);
				charDefault.setDefaultStyle(false);
				QMap<QString, QString> namesMap;
				namesMap[charDefault.name()] = importPrefix + charDefault.name() + importSuffix;
				for(int i(0) ; i < wrkCharStyles->count() ; ++i)
				{
					if( (*wrkCharStyles)[i] != charDefault )
						namesMap[(*wrkCharStyles)[i].name()] = (*wrkCharStyles)[i].name();
				}
				wrkCharStyles->rename(namesMap);
			}
		}
	}
}


void ScribusDoc::lockGuides(bool isLocked)
{
	if (GuideLock == isLocked)
		return;
	GuideLock = isLocked;
	if (UndoManager::undoEnabled())
	{
		QString name;
		if (isLocked)
			name = Um::LockGuides;
		else
			name = Um::UnlockGuides;
		SimpleState *ss = new SimpleState(name, "", Um::ILockGuides);
		ss->set("GUIDE_LOCK", isLocked);
		undoManager->action(this, ss);
	}
}

void ScribusDoc::undoRedoBegin()
{
	m_docUpdater->beginUpdate();
	m_Selection->delaySignalsOn();
}

void ScribusDoc::undoRedoDone()
{
	m_Selection->delaySignalsOff();
	m_docUpdater->endUpdate();
}

void ScribusDoc::restore(UndoState* state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
		bool layersUndo=false;

		if (ss->contains("GROUP"))
			restoreGrouping(ss, isUndo);
		else if (ss->contains("UNGROUP"))
			restoreUngrouping(ss, isUndo);
		else if (ss->contains("GUIDE_LOCK"))
		{
			if (isUndo)
				GuideLock = !ss->getBool("GUIDE_LOCK");
			else
				GuideLock = ss->getBool("GUIDE_LOCK");
		}
		else if (ss->contains("UP_LAYER"))
		{
			if (isUndo)
				lowerLayer(ss->getInt("ACTIVE"));
			else
				raiseLayer(ss->getInt("ACTIVE"));
			layersUndo=true;
		}
		else if (ss->contains("DOWN_LAYER"))
		{
			if (isUndo)
				raiseLayer(ss->getInt("ACTIVE"));
			else
				lowerLayer(ss->getInt("ACTIVE"));
			layersUndo=true;
		}
		else if (ss->contains("PRINT_LAYER"))
		{
			bool print = ss->getBool("PRINT");
			setLayerPrintable(ss->getInt("ACTIVE"), isUndo ? !print : print);
			layersUndo=true;
		}
		else if (ss->contains("LAYER_FLOW"))
		{
			bool flow = ss->getBool("FLOW");
			setLayerFlow(ss->getInt("ACTIVE"), isUndo ? !flow : flow);
			layersUndo=true;
		}
		else if (ss->contains("LAYER_LOCK"))
		{
			bool lock = ss->getBool("LOCK");
			setLayerLocked(ss->getInt("ACTIVE"), isUndo ? !lock : lock);
			layersUndo=true;
		}
		else if (ss->contains("LAYER_TRANSPARENCY"))
		{
			double old_trans = ss->getDouble("OLD_TRANS");
			double new_trans = ss->getDouble("NEW_TRANS");
			setLayerTransparency(ss->getInt("ACTIVE"), isUndo ? old_trans : new_trans);
			layersUndo=true;
		}
		else if (ss->contains("LAYER_BLENDMODE"))
		{
			int old_blend = ss->getInt("OLD_BLENDMODE");
			int new_blend = ss->getInt("NEW_BLENDMODE");
			setLayerBlendMode(ss->getInt("ACTIVE"), isUndo ? old_blend : new_blend);
			layersUndo=true;
		}
		else if (ss->contains("ADD_LAYER"))
		{
			if (isUndo)
				deleteLayer(ss->getInt("LAYER_NR"), false);
			else
			{
				int layerID=addLayer( ss->get("NAME"), false );
				int newLayerID=ss->getInt("LAYER_NR");
				bool renumberedOk=renumberLayer(layerID, newLayerID);
				Q_ASSERT(renumberedOk);
			}
			layersUndo=true;
		}
		else if (ss->contains("REMOVE_LAYER"))
		{
			if (isUndo)
			{
				int layerID=addLayer( ss->get("NAME"), false );
				int newLayerID=ss->getInt("LAYER_NR");
				bool renumberedOk=renumberLayer(layerID, newLayerID);
				Q_ASSERT(renumberedOk);
				layerID=newLayerID;
				//Layer is at the top now, lower it until it reaches the old level
				int level = ss->getInt("LEVEL");
				while (layerLevelFromID(layerID)!=level)
					lowerLayer(layerID);
			}
			else
				deleteLayer(ss->getInt("LAYER_NR"), ss->getBool("DELETE"));
			layersUndo=true;
		}
		else if (ss->contains("CHANGE_NAME"))
		{
			QString name = ss->get("OLD_NAME");
			if (!isUndo)
				name = ss->get("NEW_NAME");
			changeLayerName(ss->getInt("ACTIVE"), name);
			layersUndo=true;
		}
		else if (ss->contains("OLD_MASTERPAGE"))
			restoreMasterPageApplying(ss, isUndo);
		else if (ss->contains("COPY_PAGE"))
			restorePageCopy(ss, isUndo);
		else if (ss->contains("PAGE_CHANGEPROPS"))
		{
			if (isUndo)
			{
				changePageMargins(ss->getDouble("OLD_PAGE_INITIALTOP"), ss->getDouble("OLD_PAGE_INITIALBOTTOM"),
						ss->getDouble("OLD_PAGE_INITIALLEFT"), ss->getDouble("OLD_PAGE_INITIALRIGHT"),
						ss->getDouble("OLD_PAGE_INITIALHEIGHT"), ss->getDouble("OLD_PAGE_INITIALWIDTH"),
						ss->getDouble("OLD_PAGE_HEIGHT"), ss->getDouble("OLD_PAGE_WIDTH"), ss->getInt("OLD_PAGE_ORIENTATION"),
						ss->get("OLD_PAGE_SIZE"), ss->getInt("OLD_PAGE_MARGINPRESET"), ss->getBool("OLD_PAGE_MOVEOBJECTS"), ss->getInt("PAGE_NUM"), ss->getInt("OLD_PAGE_TYPE"));
			}
			else
			{
				changePageMargins(ss->getDouble("NEW_PAGE_INITIALTOP"), ss->getDouble("NEW_PAGE_INITIALBOTTOM"),
						ss->getDouble("NEW_PAGE_INITIALLEFT"), ss->getDouble("NEW_PAGE_INITIALRIGHT"),
						ss->getDouble("NEW_PAGE_INITIALHEIGHT"), ss->getDouble("NEW_PAGE_INITIALWIDTH"),
						ss->getDouble("NEW_PAGE_HEIGHT"), ss->getDouble("NEW_PAGE_WIDTH"), ss->getInt("NEW_PAGE_ORIENTATION"),
						ss->get("NEW_PAGE_SIZE"), ss->getInt("NEW_PAGE_MARGINPRESET"), ss->getBool("OLD_PAGE_MOVEOBJECTS"), ss->getInt("PAGE_NUM"), ss->getInt("NEW_PAGE_TYPE"));
			}
		}
		if (layersUndo)
		{
			if (ScCore->usingGUI())
			{
				m_ScMW->changeLayer(ss->getInt("ACTIVE"));
				m_ScMW->layerPalette->rebuildList();
			}
		}
	}
}

void ScribusDoc::restoreGrouping(SimpleState *state, bool isUndo)
{
	double x, y, w, h;
	Selection tmpSelection(this, false);
	int itemCount = state->getInt("itemcount");
	m_Selection->setGroupRect();
	m_Selection->getGroupRect(&x, &y, &w, &h);
	m_Selection->delaySignalsOn();
	m_Selection->clear();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = getItemNrfromUniqueID(state->getUInt(QString("item%1").arg(i)));
		if (Items->at(itemNr)->uniqueNr == state->getUInt(QString("item%1").arg(i)))
			tmpSelection.addItem(Items->at(itemNr));
	}
	if (isUndo)
		itemSelection_UnGroupObjects(&tmpSelection);
	else
		itemSelection_GroupObjects(false, false, &tmpSelection);
	QRectF rect(x, y , w, h);
	regionsChanged()->update(rect.adjusted(-10, -10, 20, 20));
	m_Selection->delaySignalsOff();
}

void ScribusDoc::restoreUngrouping(SimpleState *state, bool isUndo)
{
	double x, y, w, h;
	Selection tmpSelection(this, false);
	int itemCount = state->getInt("itemcount");
	m_Selection->setGroupRect();
	m_Selection->getGroupRect(&x, &y, &w, &h);
	m_Selection->delaySignalsOn();
	m_Selection->clear();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = getItemNrfromUniqueID(state->getUInt(QString("item%1").arg(i)));
		if (Items->at(itemNr)->uniqueNr == state->getUInt(QString("item%1").arg(i)))
		{
			if (isUndo)
				Items->at(itemNr)->isTableItem = static_cast<bool>(state->getInt(QString("tableitem%1").arg(i)));
			tmpSelection.addItem(Items->at(itemNr));
		}
	}
	if (isUndo)
		itemSelection_GroupObjects(false, false, &tmpSelection);
	else
		itemSelection_UnGroupObjects(&tmpSelection);
	QRectF rect(x, y , w, h);
	regionsChanged()->update(rect.adjusted(-10, -10, 20, 20));
	m_Selection->delaySignalsOff();
}

void ScribusDoc::setName(const QString& name)
{
	DocName = name;
}


void ScribusDoc::setModified(const bool isModified)
{
	if (modified != isModified)
	{
		modified = isModified;
		update();
	}
}


bool ScribusDoc::isModified() const
{
	return modified;
}


/** sets page properties */
void ScribusDoc::setPage(double w, double h, double t, double l, double r, double b, double sp, double ab, bool atf, int fp)
{
	docPrefsData.docSetupPrefs.pageWidth = w;
	docPrefsData.docSetupPrefs.pageHeight = h;
	docPrefsData.docSetupPrefs.margins.set(t, l, b ,r);
	PageSp = sp;
	PageSpa = ab;
	docPrefsData.docSetupPrefs.pagePositioning = fp;
	automaticTextFrames = atf;

	//CB Moved from scribus.cpp. Overrides the defaults...
//	PDF_Options.BleedTop = pageMargins.Top;
//	PDF_Options.BleedLeft = pageMargins.Left;
//	PDF_Options.BleedRight = pageMargins.Right;
//	PDF_Options.BleedBottom = pageMargins.Bottom;
}

void ScribusDoc::resetPage(int fp, MarginStruct* newMargins)
{
	if (newMargins!=0)
		docPrefsData.docSetupPrefs.margins = *newMargins;
	docPrefsData.docSetupPrefs.pagePositioning = fp;
}


bool ScribusDoc::AddFont(QString name, int fsize)
{
	bool ret = false;
//	FT_Face      face;

	if (UsedFonts.contains(name))
		return true;

	if (! AllFonts->contains(name) || name == "" )
		return false;

//	face = (*AllFonts)[name]->ftFace();
//	if ( !face )
//		return false;

/*	if ((*AllFonts)[name].ReadMetrics())         FIXME: needed?
	{
//		(*AllFonts)[name]->CharWidth[13] = 0;
//		(*AllFonts)[name]->CharWidth[28] = 0;
//		(*AllFonts)[name]->CharWidth[26] = 0;
//		(*AllFonts)[name]->CharWidth[9] = 1;
		QString afnm = (*AllFonts)[name].fontFilePath().left((*AllFonts)[name].fontFilePath().length()-3);
		QFile afm(afnm+"afm");
		if(!(afm.exists()))
		{
			afm.setName(afnm+"pfm");
		}
		if(!(afm.exists())) {
			afm.setName(afnm+"AFM");
		}
		if(!(afm.exists()))
		{
			afm.setName(afnm+"PFM");
		}
		if(!(afm.exists()))
		{
			afm.setName(afnm+"Afm");
		}
		if(!(afm.exists()))
		{
			afm.setName(afnm+"Pfm");
		}
		if (afm.exists())
			FT_Attach_File(face, afm.name());
	}
	*/
	UsedFonts[name] = fsize;
	(*AllFonts)[name].increaseUsage();
	ret = true;
	return ret;
}


QStringList ScribusDoc::getItemAttributeNames()
{
	QStringList nameList;

	for(ObjAttrVector::Iterator it = itemAttributes().begin(); it!= itemAttributes().end(); ++it)
		nameList.append((*it).name);
	return nameList;
}


void ScribusDoc::addSymbols()
{
	symReturn.resize(0);
	symReturn.addQuadPoint(1.98438, 9.14062, 1.98438, 9.14062, 1.98438, 4.03125, 1.98438, 4.03125);
	symReturn.addQuadPoint(1.98438, 4.03125, 1.98438, 4.03125, 0.546875, 3.45312, 1.09375, 4);
	symReturn.addQuadPoint(0.546875, 3.45312, 0.546875, 3.45312, 0, 2.0625, 0, 2.92188);
	symReturn.addQuadPoint(0, 2.0625, 0, 2.0625, 0.65625, 0.5, 0, 1.04688);
	symReturn.addQuadPoint(0.65625, 0.5, 0.65625, 0.5, 2.3125, 0, 1.28125, 0);
	symReturn.addQuadPoint(2.3125, 0, 2.3125, 0, 5.40625, 0, 5.40625, 0);
	symReturn.addQuadPoint(5.40625, 0, 5.40625, 0, 5.40625, 0.84375, 5.40625, 0.84375);
	symReturn.addQuadPoint(5.40625, 0.84375, 5.40625, 0.84375, 4.70312, 0.84375, 4.70312, 0.84375);
	symReturn.addQuadPoint(4.70312, 0.84375, 4.70312, 0.84375, 4.70312, 9.14062, 4.70312, 9.14062);
	symReturn.addQuadPoint(4.70312, 9.14062, 4.70312, 9.14062, 3.875, 9.14062, 3.875, 9.14062);
	symReturn.addQuadPoint(3.875, 9.14062, 3.875, 9.14062, 3.875, 0.84375, 3.875, 0.84375);
	symReturn.addQuadPoint(3.875, 0.84375, 3.875, 0.84375, 2.78125, 0.84375, 2.78125, 0.84375);
	symReturn.addQuadPoint(2.78125, 0.84375, 2.78125, 0.84375, 2.78125, 9.14062, 2.78125, 9.14062);
	symReturn.addQuadPoint(2.78125, 9.14062, 2.78125, 9.14062, 1.98438, 9.14062, 1.98438, 9.14062);
	symNewLine.resize(0);
	symNewLine.addQuadPoint(6.51562, 2.625, 6.51562, 2.625, 0.90625, 2.64062, 0.90625, 2.64062);
	symNewLine.addQuadPoint(0.90625, 2.64062, 0.90625, 2.64062, 1.4375, 1.92188, 1.26562, 2.1875);
	symNewLine.addQuadPoint(1.4375, 1.92188, 1.4375, 1.92188, 1.76562, 1.14062, 1.75, 1.42188);
	symNewLine.addQuadPoint(1.76562, 1.14062, 1.76562, 1.14062, 1.60938, 1.03125, 1.60938, 1.03125);
	symNewLine.addQuadPoint(1.60938, 1.03125, 1.60938, 1.03125, 0.90625, 1.92188, 0.90625, 1.92188);
	symNewLine.addQuadPoint(0.90625, 1.92188, 0.90625, 1.92188, 0, 2.90625, 0.578125, 2.23438);
	symNewLine.addQuadPoint(0, 2.90625, 0, 2.90625, 0.75, 3.875, 0.75, 3.875);
	symNewLine.addQuadPoint(0.75, 3.875, 0.75, 3.875, 1.57812, 4.78125, 1.1875, 4.40625);
	symNewLine.addQuadPoint(1.57812, 4.78125, 1.57812, 4.78125, 1.65625, 4.79688, 1.65625, 4.79688);
	symNewLine.addQuadPoint(1.65625, 4.79688, 1.65625, 4.79688, 1.76562, 4.65625, 1.76562, 4.65625);
	symNewLine.addQuadPoint(1.76562, 4.65625, 1.76562, 4.65625, 0.90625, 3.17188, 1.73438, 4.34375);
	symNewLine.addQuadPoint(0.90625, 3.17188, 0.90625, 3.17188, 0.96875, 3.125, 0.96875, 3.125);
	symNewLine.addQuadPoint(0.96875, 3.125, 0.96875, 3.125, 6.75, 3.125, 6.75, 3.125);
	symNewLine.addQuadPoint(6.75, 3.125, 6.75, 3.125, 6.51562, 2.625, 6.51562, 2.625);
	symNewLine.addQuadPoint(6.51562, 2.625, 6.51562, 2.625, 6.51562, 2.625, 6.51562, 2.625);
	symNewLine.addQuadPoint(999999, 999999, 999999, 999999, 999999, 999999, 999999, 999999);
	symNewLine.addQuadPoint(6.875, 0, 6.875, 0, 6.51562, 0, 6.51562, 0);
	symNewLine.addQuadPoint(6.51562, 0, 6.51562, 0, 6.51562, 2.84375, 6.51562, 2.84375);
	symNewLine.addQuadPoint(6.51562, 2.84375, 6.51562, 2.84375, 6.75, 3.125, 6.51562, 3.125);
	symNewLine.addQuadPoint(6.75, 3.125, 6.75, 3.125, 6.85938, 3.0625, 6.85938, 3.0625);
	symNewLine.addQuadPoint(6.85938, 3.0625, 6.85938, 3.0625, 6.875, 0, 6.875, 0);
	symTab.resize(0);
	symTab.addQuadPoint(4.82812, 3.96875, 4.82812, 3.96875, 4.5625, 3.73438, 4.5625, 3.96875);
	symTab.addQuadPoint(4.5625, 3.73438, 4.5625, 3.73438, 5.07812, 3.10938, 4.5625, 3.57812);
	symTab.addQuadPoint(5.07812, 3.10938, 5.07812, 3.10938, 0, 3.10938, 0, 3.10938);
	symTab.addQuadPoint(0, 3.10938, 0, 3.10938, 0, 2.625, 0, 2.625);
	symTab.addQuadPoint(0, 2.625, 0, 2.625, 5.53125, 2.625, 5.53125, 2.625);
	symTab.addQuadPoint(5.53125, 2.625, 5.53125, 2.625, 6.3125, 1.8125, 6.3125, 1.8125);
	symTab.addQuadPoint(6.3125, 1.8125, 6.3125, 1.8125, 5.64062, 1.29688, 5.64062, 1.29688);
	symTab.addQuadPoint(5.64062, 1.29688, 5.64062, 1.29688, 0, 1.29688, 0, 1.29688);
	symTab.addQuadPoint(0, 1.29688, 0, 1.29688, 0, 0.8125, 0, 0.8125);
	symTab.addQuadPoint(0, 0.8125, 0, 0.8125, 5.01562, 0.8125, 5.01562, 0.8125);
	symTab.addQuadPoint(5.01562, 0.8125, 5.01562, 0.8125, 4.45312, 0.265625, 4.45312, 0.453125);
	symTab.addQuadPoint(4.45312, 0.265625, 4.45312, 0.265625, 4.6875, 0, 4.45312, 0);
	symTab.addQuadPoint(4.6875, 0, 4.6875, 0, 5.90625, 0.828125, 4.875, 0);
	symTab.addQuadPoint(5.90625, 0.828125, 5.90625, 0.828125, 6.9375, 1.79688, 6.9375, 1.64062);
	symTab.addQuadPoint(6.9375, 1.79688, 6.9375, 1.79688, 5.95312, 2.96875, 6.9375, 1.95312);
	symTab.addQuadPoint(5.95312, 2.96875, 5.95312, 2.96875, 4.82812, 3.96875, 4.98438, 3.96875);
	symNonBreak.resize(0);
	symNonBreak.addQuadPoint(1.32812, 2.59375, 1.32812, 2.59375, 0.390625, 2.21875, 0.796875, 2.59375);
	symNonBreak.addQuadPoint(0.390625, 2.21875, 0.390625, 2.21875, 0, 1.3125, 0, 1.84375);
	symNonBreak.addQuadPoint(0, 1.3125, 0, 1.3125, 0.390625, 0.390625, 0, 0.765625);
	symNonBreak.addQuadPoint(0.390625, 0.390625, 0.390625, 0.390625, 1.32812, 0, 0.796875, 0);
	symNonBreak.addQuadPoint(1.32812, 0, 1.32812, 0, 2.23438, 0.390625, 1.85938, 0);
	symNonBreak.addQuadPoint(2.23438, 0.390625, 2.23438, 0.390625, 2.60938, 1.29688, 2.60938, 0.765625);
	symNonBreak.addQuadPoint(2.60938, 1.29688, 2.60938, 1.29688, 2.23438, 2.21875, 2.60938, 1.84375);
	symNonBreak.addQuadPoint(2.23438, 2.21875, 2.23438, 2.21875, 1.32812, 2.59375, 1.875, 2.59375);
	symNewCol.resize(0);
	symNewCol.addQuadPoint(1.73438, 0, 1.73438, 0, 2.67188, 0.109375, 2.03125, 0);
	symNewCol.addQuadPoint(2.67188, 0.109375, 2.67188, 0.109375, 3.59375, 0.203125, 3.26562, 0.21875);
	symNewCol.addQuadPoint(3.59375, 0.203125, 3.59375, 0.203125, 3.79688, 0.1875, 3.64062, 0.203125);
	symNewCol.addQuadPoint(3.79688, 0.1875, 3.79688, 0.1875, 4, 0.171875, 3.92188, 0.171875);
	symNewCol.addQuadPoint(4, 0.171875, 4, 0.171875, 4.20312, 0.1875, 4.20312, 0.1875);
	symNewCol.addQuadPoint(4.20312, 0.1875, 4.20312, 0.1875, 4.3125, 1.39062, 4.20312, 0.5625);
	symNewCol.addQuadPoint(4.3125, 1.39062, 4.3125, 1.39062, 4.42188, 2.64062, 4.42188, 2.21875);
	symNewCol.addQuadPoint(4.42188, 2.64062, 4.42188, 2.64062, 4.28125, 2.73438, 4.28125, 2.73438);
	symNewCol.addQuadPoint(4.28125, 2.73438, 4.28125, 2.73438, 3.75, 1.03125, 4.01562, 2.64062);
	symNewCol.addQuadPoint(3.75, 1.03125, 3.75, 1.03125, 3.67188, 1.03125, 3.67188, 1.03125);
	symNewCol.addQuadPoint(3.67188, 1.03125, 3.67188, 1.03125, 0.28125, 6.20312, 0.28125, 6.20312);
	symNewCol.addQuadPoint(0.28125, 6.20312, 0.28125, 6.20312, 0, 5.95312, 0.03125, 6.17188);
	symNewCol.addQuadPoint(0, 5.95312, 0, 5.95312, 3.35938, 0.71875, 3.35938, 0.71875);
	symNewCol.addQuadPoint(3.35938, 0.71875, 3.35938, 0.71875, 3.375, 0.640625, 3.375, 0.640625);
	symNewCol.addQuadPoint(3.375, 0.640625, 3.375, 0.640625, 2.4375, 0.484375, 2.79688, 0.5625);
	symNewCol.addQuadPoint(2.4375, 0.484375, 2.4375, 0.484375, 1.67188, 0.140625, 1.71875, 0.328125);
	symNewCol.addQuadPoint(1.67188, 0.140625, 1.67188, 0.140625, 1.73438, 0, 1.73438, 0);
	symNewFrame.resize(0);
	symNewFrame.addQuadPoint(1.75, 6.20312, 1.75, 6.20312, 2.67188, 6.09375, 2.0625, 6.20312);
	symNewFrame.addQuadPoint(2.67188, 6.09375, 2.67188, 6.09375, 3.60938, 5.98438, 3.28125, 5.98438);
	symNewFrame.addQuadPoint(3.60938, 5.98438, 3.60938, 5.98438, 3.84375, 6.01562, 3.6875, 5.98438);
	symNewFrame.addQuadPoint(3.84375, 6.01562, 3.84375, 6.01562, 4.07812, 6.03125, 4, 6.03125);
	symNewFrame.addQuadPoint(4.07812, 6.03125, 4.07812, 6.03125, 4.20312, 6.01562, 4.20312, 6.01562);
	symNewFrame.addQuadPoint(4.20312, 6.01562, 4.20312, 6.01562, 4.32812, 4.79688, 4.21875, 5.625);
	symNewFrame.addQuadPoint( 4.32812, 4.79688, 4.32812, 4.79688, 4.42188, 3.5625, 4.42188, 3.98438);
	symNewFrame.addQuadPoint(4.42188, 3.5625, 4.42188, 3.5625, 4.29688, 3.45312, 4.29688, 3.45312);
	symNewFrame.addQuadPoint(4.29688, 3.45312, 4.29688, 3.45312, 3.75, 5.17188, 4.03125, 3.54688);
	symNewFrame.addQuadPoint(3.75, 5.17188, 3.75, 5.17188, 3.67188, 5.17188, 3.67188, 5.17188);
	symNewFrame.addQuadPoint(3.67188, 5.17188, 3.67188, 5.17188, 0.28125, 0, 0.28125, 0);
	symNewFrame.addQuadPoint(0.28125, 0, 0.28125, 0, 0, 0.25, 0.03125, 0.015625);
	symNewFrame.addQuadPoint(0, 0.25, 0, 0.25, 3.375, 5.46875, 3.375, 5.46875);
	symNewFrame.addQuadPoint(3.375, 5.46875, 3.375, 5.46875, 3.39062, 5.54688, 3.39062, 5.54688);
	symNewFrame.addQuadPoint(3.39062, 5.54688, 3.39062, 5.54688, 2.4375, 5.70312, 2.8125, 5.625);
	symNewFrame.addQuadPoint(2.4375, 5.70312, 2.4375, 5.70312, 1.67188, 6.0625, 1.71875, 5.875);
	symNewFrame.addQuadPoint(1.67188, 6.0625, 1.67188, 6.0625, 1.75, 6.20312, 1.75, 6.20312);
}


Page* ScribusDoc::addPage(const int pageIndex, const QString& masterPageName, const bool addAutoFrame)
{
	assert(masterPageMode()==false);
	Page* addedPage = new Page(docPrefsData.displayPrefs.scratch.Left, DocPages.count()*(docPrefsData.docSetupPrefs.pageHeight+docPrefsData.displayPrefs.scratch.Bottom+docPrefsData.displayPrefs.scratch.Top)+docPrefsData.displayPrefs.scratch.Top, docPrefsData.docSetupPrefs.pageWidth, docPrefsData.docSetupPrefs.pageHeight);
	assert(addedPage!=NULL);
	addedPage->setDocument(this);
	addedPage->Margins.Top = docPrefsData.docSetupPrefs.margins.Top;
	addedPage->Margins.Bottom = docPrefsData.docSetupPrefs.margins.Bottom;
	addedPage->initialMargins.Top = docPrefsData.docSetupPrefs.margins.Top;
	addedPage->initialMargins.Bottom = docPrefsData.docSetupPrefs.margins.Bottom;
	addedPage->initialMargins.Left = docPrefsData.docSetupPrefs.margins.Left;
	addedPage->initialMargins.Right = docPrefsData.docSetupPrefs.margins.Right;
	addedPage->setPageNr(pageIndex);
	addedPage->m_pageSize = docPrefsData.docSetupPrefs.pageSize;
	addedPage->setOrientation(docPrefsData.docSetupPrefs.pageOrientation);
	addedPage->marginPreset = docPrefsData.docSetupPrefs.marginPreset;
	DocPages.insert(pageIndex, addedPage);
	assert(DocPages.at(pageIndex)!=NULL);
	setCurrentPage(addedPage);
	//if (!masterPageMode())
	if (!masterPageName.isEmpty())
		applyMasterPage(masterPageName, pageIndex);
	setLocationBasedPageLRMargins(pageIndex);
	if (addAutoFrame && automaticTextFrames)
		addAutomaticTextFrame(pageIndex);
//	if (!isLoading())
//		changed();
	return addedPage;
}


Page* ScribusDoc::addMasterPage(const int pageNumber, const QString& pageName)
{
	//CB We dont create master pages (yet) with a pageCount based location
	//Page* addedPage = new Page(ScratchLeft, MasterPages.count()*(pageHeight+ScratchBottom+ScratchTop)+ScratchTop, pageWidth, pageHeight);
	Page* addedPage = new Page(docPrefsData.displayPrefs.scratch.Left, docPrefsData.displayPrefs.scratch.Top, docPrefsData.docSetupPrefs.pageWidth, docPrefsData.docSetupPrefs.pageHeight);
	assert(addedPage!=NULL);
	addedPage->setDocument(this);
	addedPage->Margins = docPrefsData.docSetupPrefs.margins;
	addedPage->initialMargins = docPrefsData.docSetupPrefs.margins;
	addedPage->m_pageSize = docPrefsData.docSetupPrefs.pageSize;
	addedPage->setOrientation(docPrefsData.docSetupPrefs.pageOrientation);
	addedPage->marginPreset = docPrefsData.docSetupPrefs.marginPreset;
	addedPage->MPageNam = "";
	addedPage->setPageName(pageName);
	addedPage->setPageNr(pageNumber);
	MasterNames.insert(pageName, pageNumber);
	MasterPages.insert(pageNumber, addedPage);
	assert(MasterPages.at(pageNumber)!=NULL);
	if  (!isLoading())
		changed();
	return addedPage;
}


bool ScribusDoc::renameMasterPage(const QString& oldPageName, const QString& newPageName)
{
	Q_ASSERT(oldPageName!=CommonStrings::masterPageNormal && oldPageName!=CommonStrings::trMasterPageNormal);
	if (MasterNames.contains(oldPageName) && !MasterNames.contains(newPageName))
	{
		//Rename our master page lists
		int number=MasterNames[oldPageName];
		MasterNames.insert(newPageName, number);
		MasterNames.remove(oldPageName);
		Q_ASSERT(MasterPages.at(number)->pageName()==oldPageName);
		MasterPages.at(number)->setPageName(newPageName);
		//Update any pages that were linking to our old name
		Page* docPage=NULL;
		for (int i=0; i < DocPages.count(); ++i )
		{
			docPage=DocPages[i];
			if (docPage->MPageNam == oldPageName)
				docPage->MPageNam = newPageName;
		}
		//Update any items that were linking to our old name
		uint masterItemsCount=MasterItems.count();
		for (uint i = 0; i < masterItemsCount; ++i)
		{
			if (MasterItems.at(i)->OnMasterPage == oldPageName)
				MasterItems.at(i)->OnMasterPage = newPageName;
		}
		changed();
		return true;
	}
	return false;
}


void ScribusDoc::deleteMasterPage(const int pageNumber)
{
	assert(masterPageMode());
	assert( Pages->count() > 1 && Pages->count() > pageNumber );
	setCurrentPage(Pages->at(0));
	Page* page = Pages->takeAt(pageNumber);
	QString oldPageName(page->pageName());
	delete page;
	// remove the master page from the master page name list
	//MasterNames.remove(page->PageNam);
	/*CB TODO moved back to muster.cpp for now as this must happen after reformPages
	MasterNames.clear();
	for (uint a = 0; a < Pages->count(); ++a)
		MasterNames[Pages->at(a)->PageNam] = Pages->at(a)->pageNr();
	// and fix up any pages that refer to the deleted master page
	for (Page* docPage = DocPages.first(); docPage; docPage = DocPages.next() )
	{
		if (docPage->MPageNam == oldPageName)
			docPage->MPageNam = CommonStrings::masterPageNormal;
	}
	*/
	//QPtrList docs: The item after the removed item becomes the new current list item if the removed item is not the last item in the list. If the last item is removed, the new last item becomes the current item.
	changed();
}


void ScribusDoc::rebuildMasterNames(void)
{
	MasterNames.clear();
	for (int a = 0; a < MasterPages.count(); ++a)
		MasterNames[MasterPages.at(a)->pageName()] = MasterPages.at(a)->pageNr();
}


void ScribusDoc::replaceMasterPage(const QString& oldMasterPage)
{
	uint pageIndex = 0;
	QMap<QString,int>::Iterator it = MasterNames.begin();
	QListIterator<Page *> dpIt(DocPages);
	Page* docPage=NULL;
	while(dpIt.hasNext())
	//for (Page* docPage = currentDoc->DocPages.first(); docPage; docPage = currentDoc->DocPages.next() )
	{
		docPage=dpIt.next();
		if (docPage->MPageNam == oldMasterPage)
		{
			PageLocation pageLoc = locationOfPage(pageIndex);
			if (pageLoc == LeftPage)
			{
				if (MasterNames.contains( CommonStrings::trMasterPageNormalLeft))
					docPage->MPageNam = CommonStrings::trMasterPageNormalLeft;
				else if (MasterNames.contains( CommonStrings::trMasterPageNormal))
					docPage->MPageNam = CommonStrings::trMasterPageNormal;
				else
					docPage->MPageNam = it.key();
			}
			else if (pageLoc == RightPage)
			{
				if (MasterNames.contains( CommonStrings::trMasterPageNormalRight))
					docPage->MPageNam = CommonStrings::trMasterPageNormalRight;
				else if (MasterNames.contains( CommonStrings::trMasterPageNormal))
					docPage->MPageNam = CommonStrings::trMasterPageNormal;
				else
					docPage->MPageNam = it.key();
			}
			else
			{
				if (MasterNames.contains( CommonStrings::trMasterPageNormalMiddle))
					docPage->MPageNam = CommonStrings::trMasterPageNormalMiddle;
				else if (MasterNames.contains( CommonStrings::trMasterPageNormal))
					docPage->MPageNam = CommonStrings::trMasterPageNormal;
				else
					docPage->MPageNam = it.key();
			}
		}
		pageIndex++;
	}
}


void ScribusDoc::deletePage(const int pageNumber)
{
	assert( Pages->count() > 1 && Pages->count() > pageNumber );
	//#5561: If we are going to delete the first page, do not set the current page to it
	setCurrentPage(Pages->at(pageNumber!=0?0:1));
	Page* page = Pages->takeAt(pageNumber);
	delete page;
	changed();
}


void ScribusDoc::movePage(const int from, const int to, const int ziel, const int art)
{
	QList<Page*> Buf;
	int zz = ziel;
	Buf.clear();
	for (int a = from; a < to; ++a)
	{
		Buf.append(Pages->takeAt(from));
		if (a <= zz)
			--zz;
	}
	int bufCount=Buf.count();
	switch (art)
	{
		case 0:
			for (int b = 0; b < bufCount; ++b)
				Pages->insert(zz++, Buf.at(b));
			break;
		case 1:
			for (int b = 0; b < bufCount; ++b)
				Pages->insert(++zz, Buf.at(b));
			break;
		case 2:
			for (int b = 0; b < bufCount; ++b)
				Pages->append(Buf.at(b));
			break;
	}
	changed();
}


int ScribusDoc::addAutomaticTextFrame(const int pageNumber)
{
	if (!automaticTextFrames)
		return -1;
	Page *addToPage=DocPages.at(pageNumber);
	if ((!masterPageMode()) && (usesAutomaticTextFrames()))// && (!isLoading()))
	{
		int z = itemAdd(PageItem::TextFrame, PageItem::Unspecified,
		                     addToPage->Margins.Left+addToPage->xOffset(),
							 addToPage->Margins.Top+addToPage->yOffset(), docPrefsData.docSetupPrefs.pageWidth-addToPage->Margins.Right-addToPage->Margins.Left,
							 docPrefsData.docSetupPrefs.pageHeight-addToPage->Margins.Bottom-addToPage->Margins.Top,
							 1, CommonStrings::None, docPrefsData.itemToolPrefs.shapeLineColor, true);
		Items->at(z)->isAutoText = true;
		Items->at(z)->Cols = qRound(PageSp);
		Items->at(z)->ColGap = PageSpa;
		if (LastAuto != 0) {
			LastAuto->link(Items->at(z));
		}	
		else
			FirstAuto = Items->at(z);
		LastAuto = Items->at(z);
		Items->at(z)->setRedrawBounding();
		return z;
	}
	return -1;
}


int ScribusDoc::addLayer(const QString& layerName, const bool activate)
{
	int lId = Layers.addLayer(layerName);
	if (lId==-1)
		return -1;
	if (activate)
		setActiveLayer(lId);
	const ScLayer* ll = Layers.layerByID(lId);
	if (!ll)
		qWarning() << "Layer added without undo, could not get layer back for undo action creation";
	else
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::AddLayer, "", Um::ICreate);
		ss->set("ADD_LAYER", "add_layer");
		ss->set("ACTIVE", ActiveLayer);
		ss->set("NAME", ll->Name);
		ss->set("LAYER_NR", ll->ID);
		undoManager->action(this, ss, DocName, Um::ILayer);
	}
	return lId;
}


void ScribusDoc::copyLayer(int layerIDToCopy, int whereToInsert)
{
	if(!setActiveLayer(whereToInsert))
		return;
	Selection sourceSelection(this);
	for (int ite = 0; ite < Items->count(); ++ite)
	{
		PageItem *itemToCopy = Items->at(ite);
		if (itemToCopy->LayerID == layerIDToCopy)
		{
			sourceSelection.addItem(itemToCopy);
		}
	}
	if (sourceSelection.count() != 0)
	{
		ScriXmlDoc ss;
		QString dataS = ss.WriteElem(this, &sourceSelection);
		ss.ReadElemToLayer(dataS, appPrefsData.fontPrefs.AvailFonts, this, Pages->at(0)->xOffset(), Pages->at(0)->yOffset(), false, true, appPrefsData.fontPrefs.GFontSub, whereToInsert);
	}
	sourceSelection.clear();
}


bool ScribusDoc::deleteLayer(const int layerID, const bool deleteItems)
{
	if (Layers.count() < 2)
		return false;
	const ScLayer* lToRemove = Layers.layerByID(layerID);
	if (!lToRemove)
		return false;
	int layerLevel = lToRemove->Level;
	QString name   = lToRemove->Name;
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction("Layer", Um::IDocument, Um::DeleteLayer, "", Um::IDelete));

	rebuildItemLists();
	if (ScCore->usingGUI())
		removeLayer(layerID, deleteItems);

	/*
	//Layer found, do we want to delete its items too?
	if (masterPageMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
	for (uint b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->LayerID == layerID)
		{
			if (deleteItems)
			{
				MasterItems.at(b)->setTagged(true);
				MasterItems.at(b)->setLocked(false);
			}
			else
				MasterItems.at(b)->setTagged(false);
		}
	}
//	if (view->SelItem.count() != 0)
//		view->DeleteItem();
//	view->SelItem.clear();
	for (uint b = 0; b < DocItems.count(); ++b)
	{
		if (DocItems.at(b)->LayerID == l)
		{
			if (deleteItems)
			{
				DocItems.at(b)->setTagged(true);
				DocItems.at(b)->setLocked(false);
			}
			else
				DocItems.at(b)->setLayer(0);
		}
	}
//	if (view->SelItem.count() != 0)
//		view->DeleteItem();

	bool deletedOk=deleteTaggedItems();
	Q_ASSERT(deletedOk);

	*/
	//Now delete the layer
	Layers.removeLayerByID(layerID);

	if (activeTransaction)
	{
		SimpleState *ss = new SimpleState(Um::DeleteLayer, "", Um::IDelete);
		ss->set("REMOVE_LAYER", "remove_layer");
		ss->set("ACTIVE", layerID);
		ss->set("LEVEL", layerLevel);
		ss->set("NAME", name);
		ss->set("LAYER_NR", layerID);
		ss->set("DELETE", deleteItems);
		undoManager->action(this, ss, DocName, Um::ILayer);
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
//	setActiveLayer(layerIDFromLevel(0));
	return true;
}


int ScribusDoc::activeLayer()
{
	return ActiveLayer;
}


const QString& ScribusDoc::activeLayerName()
{
	const ScLayer* ll = Layers.layerByID(ActiveLayer);
	Q_ASSERT(ll);
	return ll->Name;
}


bool ScribusDoc::setActiveLayer(const int layerToActivate)
{
	const ScLayer* ll = Layers.layerByID(layerToActivate);
	Q_ASSERT(ll);
	if (ll)
		ActiveLayer=layerToActivate;
	return (ll != NULL);
}


bool ScribusDoc::setActiveLayer(const QString& layerNameToActivate)
{
	const ScLayer* ll = Layers.layerByName(layerNameToActivate);
	Q_ASSERT(ll);
	if (ll)
		ActiveLayer=ll->ID;
	return (ll != NULL);
}


bool ScribusDoc::setLayerPrintable(const int layerID, const bool isPrintable)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->isPrintable!=isPrintable && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(isPrintable ? Um::PrintLayer : Um::DoNotPrintLayer, "", Um::IPrint);
				ss->set("PRINT_LAYER", "print_layer");
				ss->set("ACTIVE", it->ID);
				ss->set("PRINT", isPrintable);
				undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->isPrintable = isPrintable;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerPrintable(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->isPrintable;
	}
	return false;
}


bool ScribusDoc::setLayerVisible(const int layerID, const bool isViewable)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			it->isViewable = isViewable;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerVisible(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->isViewable;
	}
	return false;
}


bool ScribusDoc::setLayerLocked(const int layerID, const bool isLocked)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			// == because isEditable vs isLocked...
			if (it->isEditable==isLocked && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(isLocked ? Um::SetLayerLocked : Um::SetLayerUnlocked, "", Um::ILayer);
				ss->set("LAYER_LOCK", "layer_lock");
				ss->set("ACTIVE", it->ID);
				ss->set("LOCK", isLocked);
				undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->isEditable = !isLocked;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerLocked(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return !it->isEditable;
	}
	return false;
}


bool ScribusDoc::setLayerFlow(const int layerID, const bool flow)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->flowControl!=flow && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(flow ? Um::FlowLayer : Um::DisableFlowLayer, "", Um::ITextFrame);
				ss->set("LAYER_FLOW", "layer_flow");
				ss->set("ACTIVE", it->ID);
				ss->set("FLOW", flow);
				undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->flowControl = flow;
			found=true;
			break;
		}
	}
	if (found)
	{
		// #9188 : invalidate layout of items below layer
		for (it = Layers.begin(); it != itend; ++it)
		{
			if (it->ID == layerID)
				break;
			invalidateLayer(it->ID);
		}
		changed();
	}
	return found;
}


bool ScribusDoc::layerFlow(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->flowControl;
	}
	return false;
}


bool ScribusDoc::setLayerTransparency(const int layerID, double trans)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->transparency!=trans && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(Um::SetLayerTransparency, "", Um::ILayer);
				ss->set("LAYER_TRANSPARENCY", "layer_transparency");
				ss->set("ACTIVE", it->ID);
				ss->set("OLD_TRANS", it->transparency);
				ss->set("NEW_TRANS", trans);
				undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->transparency = trans;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


double ScribusDoc::layerTransparency(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->transparency;
	}
	return 1.0;
}


bool ScribusDoc::setLayerBlendMode(const int layerID, int blend)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->blendMode!=blend && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(Um::SetLayerBlendMode, "", Um::ILayer);
				ss->set("LAYER_BLENDMODE", "layer_blendmode");
				ss->set("ACTIVE", it->ID);
				ss->set("OLD_BLENDMODE", it->blendMode);
				ss->set("NEW_BLENDMODE", blend);
				undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->blendMode = blend;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


int ScribusDoc::layerBlendMode(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->blendMode;
	}
	return 0;
}


bool ScribusDoc::setLayerOutline(const int layerID, const bool outline)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			it->outlineMode = outline;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerOutline(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->outlineMode;
	}
	return false;
}


bool ScribusDoc::setLayerMarker(const int layerID, QColor color)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			it->markerColor = color;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


QColor ScribusDoc::layerMarker(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->markerColor;
	}
	return QColor(0, 0, 0);
}


int ScribusDoc::layerLevelFromID(const int layerID)
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
			return Layers[i].Level;
	}
	return -1;
}


int ScribusDoc::layerCount() const
{
	return Layers.count();
}


int ScribusDoc::layerIDFromLevel(const int layerLevel)
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].Level == layerLevel)
			return Layers[i].ID;
	}
	return -1;
}


bool ScribusDoc::lowerLayer(const int layerID)
{
	return lowerLayerByLevel(layerLevelFromID(layerID));
}


bool ScribusDoc::lowerLayerByLevel(const int layerLevel)
{
	if (Layers.count() < 2)
		return false;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LowerLayer, "", Um::IDown);
		ss->set("DOWN_LAYER", "down_layer");
		ss->set("ACTIVE", layerLevel-1);
		undoManager->action(this, ss, DocName, Um::ILayer);
	}

	ScLayers::iterator it;
	ScLayers::iterator itend=Layers.end();
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->Level == layerLevel-1)
			break;
	}
	ScLayers::iterator it2;
	ScLayers::iterator it2end=Layers.end();
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if (it2->Level == layerLevel)
			break;
	}
	it2->Level -= 1;
	it->Level  += 1;
	// #9188 : invalidate layout of items in below layers
	int maxLevel = qMax(it->Level, it2->Level);
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->flowControl)
			invalidateLayer(it->ID);
		if (it->Level == maxLevel)
			break;
	}
	return true;
}


bool ScribusDoc::raiseLayer(const int layerID)
{
	return raiseLayerByLevel(layerLevelFromID(layerID));
}


bool ScribusDoc::raiseLayerByLevel(const int layerLevel)
{
	if (Layers.count() < 2)
		return false;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::RaiseLayer, "", Um::IUp);
		ss->set("UP_LAYER", "up_layer");
		ss->set("ACTIVE", layerLevel+1);
		undoManager->action(this, ss, DocName, Um::ILayer);
	}

	ScLayers::iterator it;
	ScLayers::iterator itend=Layers.end();
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->Level == layerLevel+1)
			break;
	}
	ScLayers::iterator it2;
	ScLayers::iterator it2end=Layers.end();
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if (it2->Level == layerLevel)
			break;
	}
	it2->Level += 1;
	it->Level  -= 1;
	// #9188 : invalidate layout of items in below layers
	int maxLevel = qMax(it->Level, it2->Level);
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->flowControl)
			invalidateLayer(it->ID);
		if (it->Level == maxLevel)
			break;
	}
	return true;
}


QString ScribusDoc::layerName(const int layerID) const
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
			return Layers[i].Name;
	}
	return QString::null;
}


bool ScribusDoc::changeLayerName(const int layerID, const QString& newName)
{
	uint layerCount=Layers.count();
	bool found=false;
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
		{
			if (Layers[i].Name != newName)
			{
				if (UndoManager::undoEnabled())
				{
					SimpleState *ss = new SimpleState(Um::SetLayerName,
													QString(Um::FromTo).arg(Layers[i].Name).arg(newName),
													Um::IDown);
					ss->set("CHANGE_NAME", "change_name");
					ss->set("ACTIVE", ActiveLayer);
					ss->set("NEW_NAME", newName);
					ss->set("OLD_NAME", Layers[i].Name);
					undoManager->action(this, ss, DocName, Um::ILayer);
				}
				Layers[i].Name = newName;
				found=true;
			}
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerContainsItems(const int layerID)
{
	QList<PageItem*> allItems;
	uint masterItemsCount=MasterItems.count();
	for (uint i = 0; i < masterItemsCount; ++i)
	{
		PageItem* currItem = MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->LayerID == layerID)
				return true;
		}
		allItems.clear();
	}
	uint docItemsCount=DocItems.count();
	for (uint i = 0; i < docItemsCount; ++i)
	{
		PageItem* currItem = DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->LayerID == layerID)
				return true;
		}
		allItems.clear();
	}
	return false;
}


void ScribusDoc::orderedLayerList(QStringList* list)
{
	Q_ASSERT(list!=NULL);
	uint layerCount=Layers.count();
	if (layerCount != 0)
	{
		for (uint i=0; i < layerCount; ++i)
		{
			ScLayers::iterator itend=Layers.end();
			for (ScLayers::iterator it = Layers.begin(); it != itend; ++it)
			{
				if (layerCount-(*it).Level-1 == i)
					list->append((*it).Name);
			}
 		}
 	}
}


bool ScribusDoc::renumberLayer(const int layerID, const int newLayerID)
{
	uint layerCount=Layers.count();
	uint foundIndex = 0;
	bool found=false;
	//Find layer to renumber, if found the new number, return as it exists already.
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
		{
			foundIndex=i;
			found=true;
		}
		else
		if (Layers[i].ID == newLayerID)
			return false;
	}
	if (!found)
		return false;
	Layers[foundIndex].ID=newLayerID;
	return true;
}


//Make the doc delete the items, not the view. TODO: Currently does nada, zilch, zero
bool ScribusDoc::deleteTaggedItems()
{
	QString tooltip = Um::ItemsInvolved + "\n";
	//Master Items
	for (int b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->isTagged())
		{
		}
	}
	//Doc Items
	for (int b = 0; b < DocItems.count(); ++b)
	{
		if (DocItems.at(b)->isTagged())
		{
		}
	}
	return true;
}


void ScribusDoc::replaceLineStyleColors(const QMap<QString, QString>& colorMap)
{
	multiLine::iterator its;
	QMap<QString, QString>::const_iterator it;
	QMap<QString,multiLine>::iterator  itl;
	for (itl = MLineStyles.begin(); itl != MLineStyles.end(); ++itl)
	{
		multiLine& mline = itl.value();
		for (its = mline.begin(); its != mline.end(); ++its)
		{
			struct SingleLine& sline = *its;
			it = colorMap.find(sline.Color);
			if (it != colorMap.end())
				sline.Color = it.value();
		}
	}
}

void ScribusDoc::getUsedColors(ColorList &colorsToUse, bool spot)
{
	bool found;
	colorsToUse.clear();
	colorsToUse.setDocument(this);
	ColorList::Iterator it;

	ResourceCollection resources;
	this->getNamedResources(resources);
	const QMap<QString, QString>& resColors = resources.colors();

	for (it = PageColors.begin(); it != PageColors.end(); ++it)
	{
		found = false;
		// Tool preferences colors
		if ((it.key() == docPrefsData.itemToolPrefs.shapeFillColor) || (it.key() == docPrefsData.itemToolPrefs.shapeLineColor) || (it.key() == docPrefsData.itemToolPrefs.imageFillColor)
				|| (it.key() == docPrefsData.itemToolPrefs.lineColor) || (it.key() == docPrefsData.itemToolPrefs.textColor))
		{
			if (spot)
			{
				if (it.value().isSpotColor())
					colorsToUse.insert(it.key(), it.value());
			}
			else
				colorsToUse.insert(it.key(), it.value());
			continue;
		}
		// Current paragraph style colors
		if ((it.key() == currentStyle.charStyle().fillColor()) || (it.key() == currentStyle.charStyle().strokeColor()))
			found = true;
		// Resources colors
		if (!found)
			found = resColors.contains(it.key());
		// Line styles colors
		if (!found)
			found = lineStylesUseColor(it.key());
		if (found)
		{
			if (spot)
			{
				if (it.value().isSpotColor())
					colorsToUse.insert(it.key(), it.value());
			}
			else
				colorsToUse.insert(it.key(), it.value());
			continue;
		}
	}
}

bool ScribusDoc::lineStylesUseColor(const QString& colorName)
{
	bool found = false;
	QMap<QString,multiLine>::const_iterator itm, itmend;
	multiLine::const_iterator its, itsend;
	itmend = MLineStyles.constEnd();
	for (itm = MLineStyles.constBegin(); itm != itmend && !found; ++itm)
	{
		const multiLine& ml = itm.value();
		itsend = ml.constEnd();
		for (its = ml.constBegin(); its != itsend; ++its)
		{
			if ( its->Color == colorName )
			{
				found = true;
				break;
			}
		}
	}
	return found;
}

void ScribusDoc::getUsedGradients(QMap<QString, VGradient> &gradients)
{
	ResourceCollection resources;
	this->getNamedResources(resources);
	const QMap<QString, QString>& resGradients = resources.gradients();
	QMap<QString, VGradient>::iterator it;
	for (it = docGradients.begin(); it != docGradients.end(); ++it)
	{
		if (resGradients.contains(it.key()))
			gradients.insert(it.key(), docGradients[it.key()]);
	}
}

bool ScribusDoc::addGradient(QString &name, VGradient &gradient)
{
 	for (QMap<QString, VGradient>::Iterator it = docGradients.begin(); it != docGradients.end(); ++it)
 	{
		if (it.value() == gradient)
		{
			name = it.key();
			return false;
		}
 	}
	QString tmp;
	if (docGradients.contains(name))
		name += "("+tmp.setNum(docGradients.count())+")";
	docGradients.insert(name, gradient);
	return true;
}

void ScribusDoc::setGradients(QMap<QString, VGradient> &gradients)
{
	docGradients.clear();
	docGradients = gradients;
}

bool ScribusDoc::addPattern(QString &name, ScPattern& pattern)
{
	QString tmp;
	if (docPatterns.contains(name))
		name += "("+tmp.setNum(docPatterns.count())+")";
//		name = tr("Copy_of_")+name;
	docPatterns.insert(name, pattern);
	return true;
}

void ScribusDoc::setPatterns(QMap<QString, ScPattern> &patterns)
{
	docPatterns.clear();
	docPatterns = patterns;
}

QStringList ScribusDoc::getUsedPatterns()
{
	QList<PageItem*> allItems;
	QStringList results;
	for (int c = 0; c < MasterItems.count(); ++c)
	{
		PageItem* currItem = MasterItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!results.contains(currItem->pattern())) && ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol)))
				results.append(currItem->pattern());
			if (!currItem->strokePattern().isEmpty())
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if (!currItem->patternMask().isEmpty())
			{
				if (!results.contains(currItem->patternMask()))
					results.append(currItem->patternMask());
			}
		}
		allItems.clear();
	}
	for (int c = 0; c < DocItems.count(); ++c)
	{
		PageItem* currItem = DocItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!results.contains(currItem->pattern())) && ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol)))
				results.append(currItem->pattern());
			if (!currItem->strokePattern().isEmpty())
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if (!currItem->patternMask().isEmpty())
			{
				if (!results.contains(currItem->patternMask()))
					results.append(currItem->patternMask());
			}
		}
		allItems.clear();
	}
	for (int c = 0; c < FrameItems.count(); ++c)
	{
		PageItem* currItem = FrameItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!results.contains(currItem->pattern())) && ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol)))
				results.append(currItem->pattern());
			if (!currItem->strokePattern().isEmpty())
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if (!currItem->patternMask().isEmpty())
			{
				if (!results.contains(currItem->patternMask()))
					results.append(currItem->patternMask());
			}
		}
		allItems.clear();
	}
	for (QMap<QString, ScPattern>::Iterator it = docPatterns.begin(); it != docPatterns.end(); ++it)
	{
		for (int c = 0; c < it.value().items.count(); ++c)
		{
			PageItem* currItem = it.value().items.at(c);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol))
				{
					const QString& patName = currItem->pattern();
					if (!patName.isEmpty() && !results.contains(patName))
						results.append(patName);
				}
				const QString& pat2 = currItem->strokePattern();
				if (!pat2.isEmpty() && !results.contains(pat2))
					results.append(pat2);
				const QString& pat3 = currItem->patternMask();
				if (!pat3.isEmpty() && !results.contains(pat3))
					results.append(pat3);
			}
			allItems.clear();
		}
	}
	return results;
}


QStringList ScribusDoc::getUsedPatternsSelection(Selection* customSelection)
{
	QStringList results;
	uint selectedItemCount = customSelection->count();
	if (selectedItemCount != 0)
	{
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = customSelection->itemAt(a);
			if ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol))
			{
				const QString& pat = currItem->pattern();
				if (!pat.isEmpty() && !results.contains(pat))
					results.append(currItem->pattern());
			}
			const QString& pat2 = currItem->strokePattern();
			if (!pat2.isEmpty() && !results.contains(pat2))
				results.append(currItem->strokePattern());
			const QString& pat3 = currItem->patternMask();
			if (!pat3.isEmpty() && !results.contains(pat3))
				results.append(currItem->patternMask());
		}
		QStringList results2 = results;
		for (int c = 0; c < results.count(); ++c)
		{
			QStringList pats = getUsedPatternsHelper(results[c], results2);
			if (!pats.isEmpty())
			{
				for (int cc = 0; cc < pats.count(); cc++)
				{
					if (!results2.contains(pats[cc]))
						results2.append(pats[cc]);
				}
			}
		}
		return results2;
	}
	else
		return QStringList();
}

QStringList ScribusDoc::getUsedPatternsHelper(QString pattern, QStringList &results)
{
	if (!docPatterns.contains(pattern))
		return QStringList();
	ScPattern *pat = &docPatterns[pattern];
	QStringList pats;
	pats.clear();
	for (int c = 0; c < pat->items.count(); ++c)
	{
		if ((pat->items.at(c)->GrType == 8) || (pat->items.at(c)->itemType() == PageItem::Symbol))
		{
			const QString& patName = pat->items.at(c)->pattern();
			if (!patName.isEmpty() && !results.contains(patName))
				pats.append(patName);
		}
		const QString& pat2 = pat->items.at(c)->strokePattern();
		if (!pat2.isEmpty() && !results.contains(pat2))
			pats.append(pat->items.at(c)->strokePattern());
		const QString& pat3 = pat->items.at(c)->patternMask();
		if (!pat3.isEmpty() && !results.contains(pat3))
			pats.append(pat->items.at(c)->patternMask());
	}
	if (!pats.isEmpty())
	{
		results += pats;
		for (int c = 0; c < pats.count(); ++c)
		{
			QStringList pp = getUsedPatternsHelper(pats[c], results);
			if (!pp.isEmpty())
			{
				for (int cc = 0; cc < pp.count(); cc++)
				{
					if (!results.contains(pp[cc]))
						results.append(pp[cc]);
				}
			}
		}
	}
	return results;
}

QStringList ScribusDoc::getPatternDependencyList(QStringList used)
{
	QStringList results;
	QStringList pp;
	QStringList pats = used;
	QStack<QStringList> patternStack;
	patternStack.push(pats);
	while (!pats.isEmpty())
	{
		for (int c = 0; c < pats.count(); ++c)
		{
			pp = getUsedPatternsHelper(pats[c], results);
			if (!pp.isEmpty())
				results += pp;
		}
		pats = results;
		results.clear();
		if (!pats.isEmpty())
			patternStack.push(pats);
	}
	results.clear();
	while (patternStack.count() != 0)
	{
		pp = patternStack.pop();
		for (int c = 0; c < pp.count(); c++)
		{
			if (!results.contains(pp[c]))
				results.prepend(pp[c]);
		}
		if (patternStack.count() == 0)
			break;
	}
	return results;
}

QStringList ScribusDoc::getUsedSymbols()
{
	QList<PageItem*> allItems;
	QStringList results;
	for (int c = 0; c < MasterItems.count(); ++c)
	{
		PageItem* currItem = MasterItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
				results.append(currItem->pattern());
			if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if ((!results.contains(currItem->strokePattern())) && ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath)))
				results.append(currItem->pattern());
		}
		allItems.clear();
	}
	for (int c = 0; c < DocItems.count(); ++c)
	{
		PageItem* currItem = DocItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
				results.append(currItem->pattern());
			if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
		}
		allItems.clear();
	}
	for (int c = 0; c < FrameItems.count(); ++c)
	{
		PageItem* currItem = FrameItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
				results.append(currItem->pattern());
			if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
		}
		allItems.clear();
	}
	for (QMap<QString, ScPattern>::Iterator it = docPatterns.begin(); it != docPatterns.end(); ++it)
	{
		for (int c = 0; c < it.value().items.count(); ++c)
		{
			PageItem* currItem = it.value().items.at(c);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
					results.append(currItem->pattern());
				if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
				{
					if (!results.contains(currItem->strokePattern()))
						results.append(currItem->strokePattern());
				}
			}
			allItems.clear();
		}
	}
	return results;
}

QStringList ScribusDoc::getUsedSymbolsHelper(QString pattern, QStringList &results)
{
	ScPattern *pat = &docPatterns[pattern];
	QStringList pats;
	pats.clear();
	for (int c = 0; c < pat->items.count(); ++c)
	{
		if (pat->items.at(c)->itemType() == PageItem::Symbol)
		{
			const QString& patName = pat->items.at(c)->pattern();
			if (!patName.isEmpty() && !results.contains(patName))
				pats.append(patName);
		}
	}
	if (!pats.isEmpty())
	{
		for (int c = 0; c < pats.count(); ++c)
		{
			getUsedSymbolsHelper(pats[c], results);
		}
	}
	return pats;
}

QMap<QString,int> ScribusDoc::reorganiseFonts()
{
//	QTime t;
//	t.start();
	QMap<QString,int> Really;
	QList<PageItem*> allItems;
	//QMap<QString,QFont> DocF;
	//DocF = UsedFonts;
	uint counter = 0;
	for (uint lc = 0; lc < 3; ++lc)
	{
		switch (lc)
		{
			case 0:
				counter = MasterItems.count();
				break;
			case 1:
				counter = DocItems.count();
				break;
			case 2:
				counter = FrameItems.count();
				break;
		}
		PageItem* it = NULL;
		for (uint d = 0; d < counter; ++d)
		{
			switch (lc)
			{
				case 0:
					it = MasterItems.at(d);
					break;
				case 1:
					it = DocItems.at(d);
					break;
				case 2:
					it = FrameItems.at(d);
					break;
			}
			if (it->isGroup())
				allItems = it->getItemList();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
				{
					QString fontName(it->itemText.defaultStyle().charStyle().font().replacementName());
					Really.insert(fontName, UsedFonts[fontName]);
					int start = it->firstInFrame();
					int stop = it->lastInFrame();
					for (int e = start; e <= stop; ++e)
					{
						QString rep = it->itemText.charStyle(e).font().replacementName();
						if (Really.contains(rep))
							continue;
						Really.insert(rep, UsedFonts[rep]);
					}
				}
			}
			allItems.clear();
		}
	}
	QMap<QString,int>::Iterator itfo, itnext;
	for (itfo = UsedFonts.begin(); itfo != UsedFonts.end(); itfo = itnext)
	{
		itnext = itfo;
		++itnext;
		if (!Really.contains(itfo.key()))
		{
			(*AllFonts)[itfo.key()].decreaseUsage();
			UsedFonts.erase(itfo);
		}
	}
	PrefsManager* prefsManager=PrefsManager::instance();
	AddFont(prefsManager->appPrefs.itemToolPrefs.textFont);//, prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.itemToolPrefs.textFont]->Font);
	AddFont(docPrefsData.itemToolPrefs.textFont);//, prefsManager->appPrefs.AvailFonts[itemToolPrefs.textFont]->Font);
//	qDebug( "Time elapsed: %d ms", t.elapsed() );
	return Really;
}

void ScribusDoc::getUsedFonts(QMap<QString, QMap<uint, FPointArray> > & Really)
{
	QList<PageItem*> allItems;
	PageItem* it = NULL;
	uint counter = 0;
	for (uint lc = 0; lc < 3; ++lc)
	{
		switch (lc)
		{
			case 0:
				counter = MasterItems.count();
				break;
			case 1:
				counter = DocItems.count();
				break;
			case 2:
				counter = FrameItems.count();
				break;
		}
		for (uint d = 0; d < counter; ++d)
		{
			switch (lc)
			{
				case 0:
					it = MasterItems.at(d);
					break;
				case 1:
					it = DocItems.at(d);
					break;
				case 2:
					it = FrameItems.at(d);
					break;
			}
			if (it->isGroup())
				allItems = it->getItemList();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				checkItemForFonts(it, Really, lc);
			}
			allItems.clear();
		}
	}
	QStringList patterns = getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			it = pa.items.at(o);
			if (it->isGroup())
				allItems = it->getItemList();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				checkItemForFonts(it, Really, 3);
			}
			allItems.clear();
		}
	}
}


void ScribusDoc::checkItemForFonts(PageItem *it, QMap<QString, QMap<uint, FPointArray> > & Really, uint lc)
{
	FPointArray gly;
	QChar chstr;
	if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
	{
		for (int e = 0; e < it->itemText.length(); ++e)
		{
			if (! Really.contains(it->itemText.charStyle(e).font().replacementName()) )
			{
				if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
					Really.insert(it->itemText.charStyle(e).font().replacementName(), QMap<uint, FPointArray>());
			}
			uint chr = it->itemText.text(e).unicode();
			if ((chr == 13) || (chr == 32) || ((chr >= 26) && (chr <= 29)))
				continue;
			if (chr == 9)
			{
				for (int t1 = 0; t1 < it->itemText.paragraphStyle(e).tabValues().count(); t1++)
				{
					if (it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar.isNull())
						continue;
					chstr = it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar;
					if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						if (chstr.toUpper() != chstr)
							chstr = chstr.toUpper();
					}
					chr = chstr.unicode();
					uint gl = it->itemText.charStyle(e).font().char2CMap(chstr);
					gly = it->itemText.charStyle(e).font().glyphOutline(gl);
					if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
						Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
				}
				for (int t1 = 0; t1 < it->itemText.defaultStyle().tabValues().count(); t1++)
				{
					if (it->itemText.defaultStyle().tabValues()[t1].tabFillChar.isNull())
						continue;
					chstr = it->itemText.defaultStyle().tabValues()[t1].tabFillChar;
					if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						if (chstr.toUpper() != chstr)
							chstr = chstr.toUpper();
					}
					chr = chstr.unicode();
					uint gl = it->itemText.charStyle(e).font().char2CMap(chstr);
					gly = it->itemText.charStyle(e).font().glyphOutline(gl);
					if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
						Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
				}
				continue;
			}
			if ((chr == 30) || (chr == 23))
			{
				/* CB Removed forced loading of 0-9 for section based numbering
				for (uint numco = 0x30; numco < 0x3A; ++numco)
				{
					if (it->itemText.charStyle(e)->cfont->CharWidth.contains(numco))
					{
						gly = it->itemText.charStyle(e)->cfont->GlyphArray[numco].Outlines.copy();
						it->itemText.charStyle(e)->cfont->RealGlyphs.insert(numco, gly);
					}
				}*/
				//Our page number collection string
				QString pageNumberText(QString::null);
				if (chr == 30)
				{//ch == SpecialChars::PAGENUMBER
					//If not on a master page just get the page number for the page and the text
					if (lc!=0)
					{
//						pageNumberText=getSectionPageNumberForPageIndex(it->OwnPage);
						pageNumberText = QString("%1").arg(getSectionPageNumberForPageIndex(it->OwnPage),
										getSectionPageNumberWidthForPageIndex(it->OwnPage),
										getSectionPageNumberFillCharForPageIndex(it->OwnPage));
					}
					else
					{
						//Else, for a page number in a text frame on a master page we must scan
						//all pages to see which ones use this page and get their page numbers.
						//We only add each character of the pages' page number text if its nothing
						//already in the pageNumberText variable. No need to add glyphs twice.
						QString newText;
						uint docPageCount=DocPages.count();
						for (uint a = 0; a < docPageCount; ++a)
						{
							if (DocPages.at(a)->MPageNam == it->OnMasterPage)
							{
//								newText=getSectionPageNumberForPageIndex(a);
								newText = QString("%1").arg(getSectionPageNumberForPageIndex(a),
												getSectionPageNumberWidthForPageIndex(a),
												getSectionPageNumberFillCharForPageIndex(a));
								for (int nti=0;nti<newText.length();++nti)
									if (pageNumberText.indexOf(newText[nti])==-1)
										pageNumberText+=newText[nti];
							}
						}
					}
				}
				else
				{//ch == SpecialChars::PAGECOUNT
					if (lc!=0)
					{
						int key = getSectionKeyForPageIndex(it->OwnPage);
						if (key == -1)
							pageNumberText = "";
						else
							pageNumberText = QString("%1").arg(getStringFromSequence(docPrefsData.docSectionMap[key].type, docPrefsData.docSectionMap[key].toindex - docPrefsData.docSectionMap[key].fromindex + 1));
					}
					else
					{
						QString newText;
						uint docPageCount=DocPages.count();
						for (uint a = 0; a < docPageCount; ++a)
						{
							if (DocPages.at(a)->MPageNam == it->OnMasterPage)
							{
								int key = getSectionKeyForPageIndex(a);
								if (key == -1)
									newText = "";
								else
									newText = QString("%1").arg(getStringFromSequence(docPrefsData.docSectionMap[key].type, docPrefsData.docSectionMap[key].toindex - docPrefsData.docSectionMap[key].fromindex + 1));
								for (int nti=0;nti<newText.length();++nti)
									if (pageNumberText.indexOf(newText[nti])==-1)
										pageNumberText+=newText[nti];
							}
						}
					}
				}
				//Now scan and add any glyphs used in page numbers
				for (int pnti=0;pnti<pageNumberText.length(); ++pnti)
				{
					uint chr = pageNumberText[pnti].unicode();
					if (it->itemText.charStyle(e).font().canRender(chr))
					{
						uint gl = it->itemText.charStyle(e).font().char2CMap(pageNumberText[pnti]);
						FPointArray gly(it->itemText.charStyle(e).font().glyphOutline(gl));
						if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
							Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
					}
				}
				continue;
			}
			if (it->itemText.charStyle(e).effects() & ScStyle_SoftHyphenVisible)
			{
				uint gl = it->itemText.charStyle(e).font().char2CMap(QChar('-'));
				FPointArray gly(it->itemText.charStyle(e).font().glyphOutline(gl));
				if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
					Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
			}
			if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
			{
				chstr = it->itemText.text(e);
				if (chstr.toUpper() != chstr)
					chstr = chstr.toUpper();
				chr = chstr.unicode();
			}
			if (it->itemText.charStyle(e).font().canRender(chr))
			{
				uint gl = it->itemText.charStyle(e).font().char2CMap(chr);
				gly = it->itemText.charStyle(e).font().glyphOutline(gl);
				if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
					Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
			}
		}
	}
}


void ScribusDoc::getUsedProfiles(ProfilesL& usedProfiles)
{
	PageItem* it = NULL;
	QString profileName;
	QStringList profileNames;
	uint counter = 0;
	usedProfiles.clear();

	QList<PageItem*> allItems;
	profileNames.append(docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile);
	profileNames.append(docPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile);
	if( profileNames.indexOf(docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile) < 0 )
		profileNames.append(docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile);
	if( profileNames.indexOf(docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile) < 0 )
		profileNames.append(docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile);
	if( profileNames.indexOf(docPrefsData.colorPrefs.DCMSset.DefaultMonitorProfile) < 0 )
		profileNames.append(docPrefsData.colorPrefs.DCMSset.DefaultMonitorProfile);
	if( profileNames.indexOf(docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile) < 0 )
		profileNames.append(docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile);
	
	for (uint lc = 0; lc < 3; ++lc)
	{
		if (lc == 0)
			counter = MasterItems.count();
		else if(lc == 1)
			counter = DocItems.count();
		else if(lc == 2)
			counter = FrameItems.count();
		for (uint d = 0; d < counter; ++d)
		{
			if (lc == 0)
				it = MasterItems.at(d);
			else if(lc == 1)
				it = DocItems.at(d);
			else if(lc == 2)
				it = FrameItems.at(d);
			if (it->isGroup())
				allItems = it->getItemList();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				if (it->IProfile.isEmpty() || profileNames.contains(it->IProfile))
					continue;
				profileNames.append(it->IProfile);
			}
			allItems.clear();
		}
	}

	for( QStringList::Iterator pIter = profileNames.begin(); pIter != profileNames.end(); pIter++ )
	{
		if (ScCore->InputProfiles.contains(*pIter))
			usedProfiles[*pIter] = ScCore->InputProfiles[*pIter];
		else if (ScCore->InputProfilesCMYK.contains(*pIter))
			usedProfiles[*pIter] = ScCore->InputProfilesCMYK[*pIter];
		else if (ScCore->PrinterProfiles.contains(*pIter))
			usedProfiles[*pIter] = ScCore->PrinterProfiles[*pIter];
	}
}


void ScribusDoc::setUnitIndex(const int newIndex)
{
	docPrefsData.docSetupPrefs.docUnitIndex=newIndex;
	docUnitRatio = unitGetRatioFromIndex( newIndex );
}


int ScribusDoc::unitIndex() const
{
	return docPrefsData.docSetupPrefs.docUnitIndex;
}


double ScribusDoc::unitRatio() const
{
	return docUnitRatio;
}


bool ScribusDoc::applyMasterPage(const QString& pageName, const int pageNumber)
{
	if (!MasterNames.contains(pageName))
		return false;

	if (UndoManager::undoEnabled())
	{
		if (DocPages.at(pageNumber)->MPageNam != pageName)
		{
			SimpleState *ss = new SimpleState(Um::ApplyMasterPage, QString(Um::FromTo).arg(DocPages.at(pageNumber)->MPageNam).arg(pageName));
			ss->set("PAGE_NUMBER", pageNumber);
			ss->set("OLD_MASTERPAGE", DocPages.at(pageNumber)->MPageNam);
			ss->set("NEW_MASTERPAGE", pageName);
			undoManager->action(this, ss);
		}
	}

// 	QString mna = pageName;
// 	if (mna == CommonStrings::trMasterPageNormal)
// 		mna = CommonStrings::masterPageNormal;
// 	if (!MasterNames.contains(mna))
// 		mna = CommonStrings::masterPageNormal;
	Page* Ap = DocPages.at(pageNumber);
	Ap->MPageNam = pageName;
	const int MpNr = MasterNames[pageName];
	Page* Mp = MasterPages.at(MpNr);
	PageItem *currItem;

// 	for (currItem = Ap->FromMaster.first(); currItem; currItem = Ap->FromMaster.next())
// 	{
// 		if (currItem->ChangedMasterItem)
// 		{
// 			Ap->FromMaster.remove(currItem);
// 			delete currItem;
// 		}
// 	}

	Ap->FromMaster.clear();
	for (int itn = 0; itn < MasterItems.count(); ++itn)
	{
		currItem = MasterItems.at(itn);
		if (currItem->OwnPage == MpNr)
			Ap->FromMaster.append(currItem);
	}
	if (!isLoading())
	{
		// PV - apply auto guides from MP only if there are no auto guides
		// on original page
		if (Ap->guides.horizontalAutoCount() != 0 || Ap->guides.horizontalAutoCount() != 0)
			Mp->guides.copy(&Ap->guides, GuideManagerCore::Standard);
		else
			Mp->guides.copy(&Ap->guides);

		Ap->initialMargins.Top = Mp->Margins.Top;
		Ap->initialMargins.Bottom = Mp->Margins.Bottom;
		if (pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns == 1)
		{
			Ap->initialMargins.Left = Mp->Margins.Left;
			Ap->initialMargins.Right = Mp->Margins.Right;
		}
		else
		{
			PageLocation pageLoc=locationOfPage(pageNumber);
			if (pageLoc==LeftPage) //Left hand page
			{
				if (Mp->LeftPg != 0)
				{
					Ap->initialMargins.Right = Mp->initialMargins.Right;
					Ap->initialMargins.Left = Mp->initialMargins.Left;
				}
				else
				{
					Ap->initialMargins.Left = Mp->initialMargins.Right;
					Ap->initialMargins.Right = Mp->initialMargins.Left;
				}
			}
			else if (pageLoc==RightPage) // Right hand page
			{
				if (Mp->LeftPg != 0)
				{
					Ap->initialMargins.Left = Mp->initialMargins.Right;
					Ap->initialMargins.Right = Mp->initialMargins.Left;
				}
				else
				{
					Ap->initialMargins.Right = Mp->initialMargins.Right;
					Ap->initialMargins.Left = Mp->initialMargins.Left;
				}
			}
			else //Middle pages
			{
				Ap->initialMargins.Left = Mp->initialMargins.Left;
				Ap->initialMargins.Right = Mp->initialMargins.Right;
			}
		}

		//#8212: Apply page settings
		Ap->setInitialHeight(Mp->initialHeight());
		Ap->setInitialWidth(Mp->initialWidth());
		Ap->setHeight(Mp->height());
		Ap->setWidth(Mp->width());
		Ap->setOrientation(Mp->orientation());
		Ap->m_pageSize = Mp->m_pageSize;
	}
	//TODO make a return false if not possible to apply the master page
	if (!isLoading())
		changed();
	return true;
}


void ScribusDoc::restoreMasterPageApplying(SimpleState *state, bool isUndo)
{
	int pageNumber = state->getInt("PAGE_NUMBER");
	QString oldName = state->get("OLD_MASTERPAGE");
	QString newName = state->get("NEW_MASTERPAGE");
	if (isUndo)
		applyMasterPage(oldName, pageNumber);
	else
		applyMasterPage(newName, pageNumber);
}


void ScribusDoc::restorePageCopy(SimpleState *state, bool isUndo)
{
	int pnum = state->getInt("PAGE_NUM");
	int extPage = state->getInt("EXISTING_PAGE");
	int whereTo = state->getInt("WHERE_TO");
	int copyCount = state->getInt("COPY_COUNT");

	if (isUndo)
	{
		int destLocation=extPage + 1;
		if (whereTo==0)
			--destLocation;
		else if (whereTo==2)
			destLocation=DocPages.count();
		for (int i = 0; i < copyCount; ++i)
		{
			m_ScMW->DeletePage(destLocation, destLocation);
			if (whereTo == 2)
				--destLocation;
		}
	}
	else
		copyPage(pnum, extPage, whereTo, copyCount);

}


//TODO: Handle saving to versions of SLA, and other formats
bool ScribusDoc::save(const QString& fileName, QString* savedFile)
{
	QProgressBar* mainWindowProgressBar=NULL;
	if (ScCore->usingGUI())
	{
		mainWindowProgressBar=m_ScMW->mainWindowProgressBar;
		mainWindowProgressBar->reset();
	}
	FileLoader fl(fileName);
	bool ret = fl.SaveFile(fileName, this, savedFile);
	if (ret)
	{
		setName(fileName);
		setModified(false);
		hasName = true;
	}
	return ret;
}


bool ScribusDoc::changePageMargins(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double height, const double width, const int orientation, const QString& pageSize, const int marginPreset, const bool moveObjects, const int pageNumber, const int pageType)
{
	bool retVal=true;
	if (pageNumber==-1)
	{
		//find page and set values
		return false;
	}
	else
	{
		if (currentPage()==NULL)
			retVal=false;
		else
		{
			QRectF pagebox(currentPage()->xOffset(), currentPage()->yOffset(), qMax( currentPage()->width(), width), qMax(currentPage()->height(), height));
			if (UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(Um::ChangePageProps);//, QString("%1").arg(pageNumber), Um::IPage);
				ss->set("PAGE_CHANGEPROPS", "page_changeprops");
				ss->set("PAGE_NUM", pageNumber);
				ss->set("OLD_PAGE_INITIALTOP", currentPage()->initialMargins.Top);
				ss->set("OLD_PAGE_INITIALBOTTOM", currentPage()->initialMargins.Bottom);
				ss->set("OLD_PAGE_INITIALLEFT", currentPage()->initialMargins.Left);
				ss->set("OLD_PAGE_INITIALRIGHT", currentPage()->initialMargins.Right);
				ss->set("OLD_PAGE_INITIALHEIGHT", currentPage()->initialHeight());
				ss->set("OLD_PAGE_INITIALWIDTH", currentPage()->initialWidth());
				ss->set("OLD_PAGE_HEIGHT", currentPage()->height());
				ss->set("OLD_PAGE_WIDTH", currentPage()->width());
				ss->set("OLD_PAGE_ORIENTATION", currentPage()->orientation());
				ss->set("OLD_PAGE_SIZE", currentPage()->m_pageSize);
				ss->set("OLD_PAGE_TYPE", currentPage()->LeftPg);
				ss->set("OLD_PAGE_MARGINPRESET", currentPage()->marginPreset);
				ss->set("OLD_PAGE_MOVEOBJECTS", moveObjects);
				ss->set("NEW_PAGE_INITIALTOP", initialTop);
				ss->set("NEW_PAGE_INITIALBOTTOM", initialBottom);
				ss->set("NEW_PAGE_INITIALLEFT", initialLeft);
				ss->set("NEW_PAGE_INITIALRIGHT", initialRight);
				ss->set("NEW_PAGE_INITIALHEIGHT", initialHeight);
				ss->set("NEW_PAGE_INITIALWIDTH", initialWidth);
				ss->set("NEW_PAGE_HEIGHT", height);
				ss->set("NEW_PAGE_WIDTH", width);
				ss->set("NEW_PAGE_ORIENTATION", orientation);
				ss->set("NEW_PAGE_SIZE", pageSize);
				ss->set("NEW_PAGE_TYPE", pageType);
				ss->set("NEW_PAGE_MARGINPRESET", marginPreset);
				undoManager->action(this, ss);
			}
			//set the current page's values
			currentPage()->initialMargins.Top = initialTop;
			currentPage()->initialMargins.Bottom = initialBottom;
			currentPage()->initialMargins.Left = initialLeft;
			currentPage()->initialMargins.Right = initialRight;
			currentPage()->setInitialHeight(initialHeight);
			currentPage()->setInitialWidth(initialWidth);
			currentPage()->setHeight(height);
			currentPage()->setWidth(width);
			currentPage()->setOrientation(orientation);
			currentPage()->m_pageSize = pageSize;
			currentPage()->LeftPg = pageType;
			currentPage()->marginPreset = marginPreset;
			reformPages(moveObjects);
			invalidateRegion(pagebox);
			regionsChanged()->update(pagebox);
			changed();
		}
	}
	return retVal;
}


void ScribusDoc::recalculateColors()
{
	//Adjust Items of the 3 types to the colors
	uint itemsCount=Items->count();
	updateAllItemQColors();
	QMap<QString, VGradient>::Iterator itGrad;
	for (itGrad = docGradients.begin(); itGrad != docGradients.end(); ++itGrad)
	{
		QList<VColorStop*> cstops = itGrad.value().colorStops();
		for (uint cst = 0; cst < itGrad.value().Stops(); ++cst)
		{
			const ScColor& col = PageColors[cstops.at(cst)->name];
			QColor tmp = ScColorEngine::getShadeColorProof(col, this, cstops.at(cst)->shade);
			if (viewAsPreview)
			{
				VisionDefectColor defect;
				tmp = defect.convertDefect(tmp, previewVisual);
			}
			cstops.at(cst)->color = tmp;
		}
	}
	for (uint c=0; c<itemsCount; ++c)
	{
		PageItem *ite = Items->at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->stroke_gradient.colorStops();
		for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->mask_gradient.colorStops();
		for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
	}
	uint masterItemsCount=MasterItems.count();
	for (uint c=0; c<masterItemsCount; ++c)
	{
		PageItem *ite = MasterItems.at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->stroke_gradient.colorStops();
		for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->mask_gradient.colorStops();
		for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
	}
	uint frameItemsCount=FrameItems.count();
	for (uint c=0; c<frameItemsCount; ++c)
	{
		PageItem *ite = FrameItems.at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->stroke_gradient.colorStops();
		for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->mask_gradient.colorStops();
		for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *ite = pa.items.at(o);
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops = ite->stroke_gradient.colorStops();
			for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops = ite->mask_gradient.colorStops();
			for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			if (ite->asImageFrame())
				loadPict(ite->Pfile, ite, true, false);
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
	}
}


void ScribusDoc::setScTextDefaultsFromDoc(ScText *sctextdata)
{
	if (sctextdata==NULL)
		return;
#ifndef NLS_PROTO
	reinterpret_cast<CharStyle&>(*sctextdata) = currentStyle.charStyle();
//	sctextdata->cab = 0;
#endif
}

bool ScribusDoc::copyPageToMasterPage(const int pageNumber, const int leftPage, const int maxLeftPage,  const QString& masterPageName, bool copyFromAppliedMaster)
{
	assert(!masterPageMode());
	if (masterPageMode())
		return false;
	int GrMax = GroupCounter;
	Page* sourcePage = Pages->at(pageNumber);
	int nr = MasterPages.count();
	Page* targetPage=addMasterPage(nr, masterPageName);
	assert(targetPage!=NULL);
	//Backup currentpage, and dont use sourcepage here as we might convert a non current page
	Page* oldCurrentPage = currentPage();
	//Must set current page for pasteitem to work properly
	setLoading(true);
	targetPage->copySizingProperties(sourcePage, sourcePage->Margins);
	//Grab the left page setting for the current document layout from the dialog, and increment, singlePage==1 remember.
	if (docPrefsData.docSetupPrefs.pagePositioning != singlePage)
	{
		int lp = leftPage;
		if (lp == 0)
			lp = 1;
		else if (lp == maxLeftPage-1)
			lp = 0;
		else
			++lp;
		targetPage->LeftPg = lp;
	}
	sourcePage->guides.copy(&targetPage->guides);
	uint end = DocItems.count();
	uint end2 = MasterItems.count();
	Selection tempSelection(this, false);
	m_Selection->clear();
	//Copy the items from our current document page's applied *master* page
	if (copyFromAppliedMaster)
	{
		if (!sourcePage->MPageNam.isEmpty() && MasterNames.contains(sourcePage->MPageNam))
		{
			Page* pageMaster=NULL;
			for (int i=0; i < MasterPages.count(); ++i )
			{
				pageMaster=MasterPages[i];
				if (pageMaster->pageName() == sourcePage->MPageNam)
					break;
			}
			if (Layers.count()!= 0 && pageMaster!=NULL)
			{
				int currActiveLayer = activeLayer();
				for (ScLayers::iterator it = Layers.begin(); it != Layers.end(); ++it)
				{
					setActiveLayer(it->ID);
					for (uint ite = 0; ite < end2; ++ite)
					{
						PageItem *itemToCopy = MasterItems.at(ite);
						if ((itemToCopy->OnMasterPage == pageMaster->pageName()) && (it->ID == itemToCopy->LayerID))
							tempSelection.addItem(itemToCopy, true);
					}
					if (tempSelection.count() != 0)
					{
						setMasterPageMode(true);
						ScriXmlDoc *ss = new ScriXmlDoc();
						QString dataS = ss->WriteElem(this, &tempSelection);
						setCurrentPage(targetPage);
						ss->ReadElemToLayer(dataS, appPrefsData.fontPrefs.AvailFonts, this, targetPage->xOffset(), targetPage->yOffset(), false, true, appPrefsData.fontPrefs.GFontSub, it->ID);
						delete ss;
						setMasterPageMode(false);
					}
					tempSelection.clear();
				}
				setActiveLayer(currActiveLayer);
			}
		}
	}
	//Copy the items from our current *document* page
	if (Layers.count()!= 0)
	{
		int currActiveLayer = activeLayer();
		for (ScLayers::iterator it = Layers.begin(); it != Layers.end(); ++it)
		{
			setActiveLayer(it->ID);
			for (uint ite = 0; ite < end; ++ite)
			{
				PageItem *itemToCopy = DocItems.at(ite);
				if ((itemToCopy->OwnPage == static_cast<int>(sourcePage->pageNr())) && (it->ID == itemToCopy->LayerID))
					tempSelection.addItem(itemToCopy, true);
			}
			if (tempSelection.count() != 0)
			{
				ScriXmlDoc *ss = new ScriXmlDoc();
				QString dataS = ss->WriteElem(this, &tempSelection);
				setMasterPageMode(true);
				setCurrentPage(targetPage);
				ss->ReadElemToLayer(dataS, appPrefsData.fontPrefs.AvailFonts, this, targetPage->xOffset(), targetPage->yOffset(), false, true, appPrefsData.fontPrefs.GFontSub, it->ID);
				delete ss;
				setMasterPageMode(false);
			}
			tempSelection.clear();
		}
		setActiveLayer(currActiveLayer);
	}
	//Make sure our copied items have the master page name and own page set.
	uint end3 = MasterItems.count();
	for (uint a = end2; a < end3; ++a)
	{
		PageItem *newItem = MasterItems.at(a);
		newItem->OnMasterPage = masterPageName;
		newItem->OwnPage = MasterNames[masterPageName];
	}
	targetPage->MPageNam = "";
	setLoading(false);
	GroupCounter = GrMax + 1;
	//Reset the current page..
	setMasterPageMode(false);
	setCurrentPage(oldCurrentPage);
	changed();
	return true;
}

int ScribusDoc::itemAdd(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double b, const double h, const double w, const QString& fill, const QString& outline, const bool itemFinalised)
{
	assert(itemFinalised); // av: caller must wrap transaction around this if wanted
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled()) // && !m_itemCreationTransaction)
	{
		activeTransaction = new UndoTransaction(undoManager->beginTransaction());
	}
	PageItem* newItem=NULL;
	switch( itemType )
	{
		//Q_ASSERTs here will warn on creation issues when a coder specifies the frameType incorrectly
		//for items that do not have/need a frameType for creation.
		case PageItem::ImageFrame:
			newItem = new PageItem_ImageFrame(this, x, y, b, h, w, docPrefsData.itemToolPrefs.imageFillColor, CommonStrings::None);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::TextFrame:
			newItem = new PageItem_TextFrame(this, x, y, b, h, w, CommonStrings::None, outline);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::Line:
			{
				//CB 5521 Remove false minimum line width
				double lineWidth = w; // == 0.0 ? 1.0 : w;
				newItem = new PageItem_Line(this, x, y, b, h, lineWidth, CommonStrings::None, outline);
				Q_ASSERT(frameType==PageItem::Unspecified);
			}
			break;
		case PageItem::Polygon:
			newItem = new PageItem_Polygon(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Ellipse || frameType==PageItem::Unspecified);
			break;
		case PageItem::PolyLine:
			newItem = new PageItem_PolyLine(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Unspecified);
			break;
		case PageItem::PathText:
			//Currently used only in fileloader
			newItem = new PageItem_PathText(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Unspecified);
			break;
		case PageItem::LatexFrame:
			newItem = new PageItem_LatexFrame(this, x, y, b, h, w, docPrefsData.itemToolPrefs.imageFillColor, CommonStrings::None);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
#ifdef HAVE_OSG
		case PageItem::OSGFrame:
			newItem = new PageItem_OSGFrame(this, x, y, b, h, w, docPrefsData.itemToolPrefs.imageFillColor, CommonStrings::None);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
#endif
		case PageItem::Symbol:
			newItem = new PageItem_Symbol(this, x, y, b, h, w, CommonStrings::None, CommonStrings::None);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::Group:
			newItem = new PageItem_Group(this, x, y, b, h, w, CommonStrings::None, CommonStrings::None);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::RegularPolygon:
			newItem = new PageItem_RegularPolygon(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Ellipse || frameType==PageItem::Unspecified);
			break;
		case PageItem::Arc:
			newItem = new PageItem_Arc(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Ellipse || frameType==PageItem::Unspecified);
			break;
		case PageItem::Spiral:
			newItem = new PageItem_Spiral(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Unspecified);
			break;
		default:
//			qDebug() << "unknown item type";
			assert (false);
	}
	Q_CHECK_PTR(newItem);
	if (newItem==NULL)
		return -1;
	Items->append(newItem);
	newItem->ItemNr = Items->count()-1;
	//Add in item default values based on itemType and frameType
	itemAddDetails(itemType, frameType, newItem->ItemNr);
	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(newItem);
		//Undo target rests with the Page for object specific undo
		UndoObject *target = Pages->at(0);
		if (newItem->OwnPage > -1)
			target = Pages->at(newItem->OwnPage);
		undoManager->action(target, is);
		//If the item is created "complete" (ie, not being created by drag/resize, commit to undomanager)
		if (activeTransaction)
		{
			//dont think we need this now ... newItem->checkChanges(true);
			activeTransaction->commit(Pages->at(newItem->OwnPage)->getUName(), newItem->getUPixmap(),
											  Um::Create + " " + newItem->getUName(),  "", Um::ICreate);
			delete activeTransaction;
			activeTransaction = NULL;
		}
	}
	return newItem->ItemNr;
}


int ScribusDoc::itemAddArea(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double w, const QString& fill, const QString& outline, const bool itemFinalised)
{
	double xo = m_currentPage->xOffset();
	double yo = m_currentPage->yOffset();
	QPair<double, double> tl = m_currentPage->guides.topLeft(x - xo, y - yo);
	QPair<double, double> tr = m_currentPage->guides.topRight(x - xo, y - yo);
	QPair<double, double> bl = m_currentPage->guides.bottomLeft(x - xo, y - yo);
	return itemAdd(itemType, frameType, tl.first + xo, tl.second + yo, tr.first - tl.first, bl.second - tl.second, w, fill, outline, itemFinalised);
}


int ScribusDoc::itemAddUserFrame(InsertAFrameData &iafData)
{
	double x1=0.0,y1=0.0,w1=iafData.width,h1=iafData.height;
	std::vector<int> pageNs;
	if (iafData.locationType==0) // On the current page or on a range of pages
		pageNs.push_back(currentPage()->pageNr()+1);
	else if (iafData.locationType==1) // On all pages
	{
		parsePagesString(QString("1-%1").arg(DocPages.count()), &pageNs, DocPages.count());
	}
	else
		parsePagesString(iafData.pageList, &pageNs, DocPages.count());
	Page* oldCurrentPage = currentPage();
	int z=-2;
	PageItem *prevItem=0; //Previous item for text frame linking
	if (iafData.linkToExistingFrame && iafData.linkToExistingFramePtr!=NULL && 
		   iafData.linkToExistingFramePtr->itemType()==PageItem::TextFrame &&
		   DocItems.contains(iafData.linkToExistingFramePtr))
		prevItem=iafData.linkToExistingFramePtr;
	UndoTransaction transaction(undoManager->beginTransaction(iafData.frameType==PageItem::TextFrame ? Um::TextFrame : Um::ImageFrame,
															  iafData.frameType==PageItem::TextFrame ? Um::ITextFrame : Um::IImageFrame,
															  Um::InsertFrame, "", Um::ICreate));
	for (uint i=0;i<pageNs.size();++i)
	{
		Page* targetPage=Pages->at(pageNs[i]-1);
		//We need this for the itemAdd, FIXME later
		setCurrentPage(targetPage);
		
		if (iafData.positionType==0) // Frame starts at top left of page margins
		{
			x1=targetPage->xOffset()+targetPage->Margins.Left;
			y1=targetPage->yOffset()+targetPage->Margins.Top;
		}
		else if (iafData.positionType==1) // Frame starts at top left of page
		{
			x1=targetPage->xOffset();
			y1=targetPage->yOffset();
		}
		else
		if (iafData.positionType==2) // Frame starts at top left of page - bleeds
		{
			x1=targetPage->xOffset()-docPrefsData.docSetupPrefs.bleeds.Left;
			y1=targetPage->yOffset()-docPrefsData.docSetupPrefs.bleeds.Top;
		}
		else if (iafData.positionType==3) // Frame starts at custom position
		{
			x1=targetPage->xOffset()+iafData.x;
			y1=targetPage->yOffset()+iafData.y;
		}
		
		if (iafData.sizeType==0) // Frame is size of page margins
		{
			w1=targetPage->width()-targetPage->Margins.Right-targetPage->Margins.Left;
			h1=targetPage->height()-targetPage->Margins.Bottom-targetPage->Margins.Top;
		}
		else if (iafData.sizeType==1) // Frame is size of page
		{
			w1=targetPage->width();
			h1=targetPage->height();
		}
		else
		if (iafData.sizeType==2) // Frame is size of page + bleed 
		{
			w1=targetPage->width()+docPrefsData.docSetupPrefs.bleeds.Right+docPrefsData.docSetupPrefs.bleeds.Left;
			h1=targetPage->height()+docPrefsData.docSetupPrefs.bleeds.Bottom+docPrefsData.docSetupPrefs.bleeds.Top;
		}
		else if (iafData.sizeType==3) //Frame is size of imported image, we resize below when we load it
		{
			w1=h1=1;
		}
		else if (iafData.sizeType==4) // Frame is custom size
		{
			w1=iafData.width;
			h1=iafData.height;
		}
		z=itemAdd(iafData.frameType, PageItem::Unspecified, x1, y1, w1, h1, docPrefsData.itemToolPrefs.shapeLineWidth, CommonStrings::None, docPrefsData.itemToolPrefs.textColor, true);
		if (z!=-1)
		{
			PageItem* currItem=Items->at(z);
			setRedrawBounding(currItem);
			if (iafData.frameType==PageItem::ImageFrame && !iafData.source.isEmpty())
			{
				if (QFile::exists(iafData.source))
				{
					PrefsManager::instance()->prefsFile->getContext("dirs")->set("images", iafData.source.left(iafData.source.lastIndexOf("/")));
					currItem->EmProfile = "";
					currItem->pixm.imgInfo.isRequest = false;
					currItem->UseEmbedded = true;
					currItem->IProfile = docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
					currItem->IRender = docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
					qApp->changeOverrideCursor( QCursor(Qt::WaitCursor) );
					qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
					loadPict(iafData.source, currItem, false, true);
					if (iafData.sizeType==3) //Frame is size of imported image
					{
						currItem->setWidth(static_cast<double>(currItem->OrigW * 72.0 / currItem->pixm.imgInfo.xres));
						currItem->setHeight(static_cast<double>(currItem->OrigH * 72.0 / currItem->pixm.imgInfo.yres));
						currItem->OldB2 = currItem->width();
						currItem->OldH2 = currItem->height();
						currItem->updateClip();
					}
					qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
					qApp->changeOverrideCursor(Qt::ArrowCursor);
				}
			}
			if (iafData.frameType==PageItem::TextFrame)
			{
				currItem->setColumns(iafData.columnCount);
				currItem->setColumnGap(iafData.columnGap/docUnitRatio);
				if (i==0 && iafData.linkToExistingFrame && prevItem != NULL)
				{
					prevItem->link(currItem);
				}
				if (i!=0 && iafData.linkTextFrames && prevItem != NULL)
				{
					prevItem->link(currItem);
				}
				if (!iafData.source.isEmpty() && prevItem == NULL && QFile::exists(iafData.source))
				{
					gtGetText* gt = new gtGetText(this);
					if (iafData.impsetup.runDialog)
						gt->launchImporter(iafData.impsetup.importer, iafData.impsetup.filename, iafData.impsetup.textOnly, iafData.impsetup.encoding, true, currItem);
					delete gt;
				}
				prevItem=currItem;
			}
		}
	}

	transaction.commit();
	setCurrentPage(oldCurrentPage);
	changed();
	regionsChanged()->update(QRectF());
	return z;
}


void ScribusDoc::itemAddDetails(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const int itemNumber)
{
	PageItem* newItem=Items->at(itemNumber);
	Q_ASSERT(newItem->realItemType()==itemType);
	switch( itemType )
	{
		case PageItem::ImageFrame:
			newItem->setImageXYScale(docPrefsData.itemToolPrefs.imageScaleX, docPrefsData.itemToolPrefs.imageScaleY);
			newItem->ScaleType = docPrefsData.itemToolPrefs.imageScaleType;
			newItem->AspectRatio = docPrefsData.itemToolPrefs.imageAspectRatio;
			newItem->IProfile = docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
			newItem->IRender = docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
			newItem->setFillShade(docPrefsData.itemToolPrefs.imageFillColorShade);
			break;
		case PageItem::LatexFrame:
#ifdef HAVE_OSG
		case PageItem::OSGFrame:
#endif
			newItem->setFillShade(docPrefsData.itemToolPrefs.imageFillColorShade);
			break;
		case PageItem::TextFrame:
//			newItem->setFontFillShade(itemToolPrefs.textShade);
//			newItem->setFontStrokeShade(itemToolPrefs.textStrokeShade);
			newItem->setFillColor(docPrefsData.itemToolPrefs.textFillColor);
			newItem->setFillShade(docPrefsData.itemToolPrefs.textFillColorShade);
			newItem->setLineColor(docPrefsData.itemToolPrefs.textLineColor);
			newItem->setLineShade(docPrefsData.itemToolPrefs.textLineColorShade);
			break;
		case PageItem::Line:
			newItem->PLineArt = Qt::PenStyle(docPrefsData.itemToolPrefs.lineStyle);
			newItem->setLineShade(docPrefsData.itemToolPrefs.lineColorShade);
			break;
		case PageItem::Polygon:
			if(frameType!=PageItem::Rectangle && frameType!=PageItem::Ellipse)
			{
				newItem->ClipEdited = true;
				newItem->FrameType = 3;
			}
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			newItem->ClipEdited = true;
			break;
		case PageItem::PathText:
			break;
		case PageItem::Symbol:
		case PageItem::Group:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
		//At this point, we cannot create a PathText item like this, only by conversion, do nothing
			break;
		default:
			break;
	}

	if (frameType==PageItem::Rectangle || itemType==PageItem::TextFrame || itemType==PageItem::ImageFrame || itemType==PageItem::LatexFrame || itemType==PageItem::OSGFrame || itemType==PageItem::Symbol || itemType==PageItem::Group)
	{
		newItem->SetRectFrame();
		//TODO one day hopefully, if(ScCore->usingGUI())
		newItem->setRedrawBounding();
		//m_View->setRedrawBounding(newItem);
		newItem->ContourLine = newItem->PoLine.copy();
	}

	if (frameType==PageItem::Ellipse)
	{
		newItem->SetOvalFrame();
		//TODO one day hopefully, if(ScCore->usingGUI())
		newItem->setRedrawBounding();
		//m_View->setRedrawBounding(newItem);
		newItem->ContourLine = newItem->PoLine.copy();
	}

	//ItemType Polygon
	if (itemType==PageItem::Polygon || itemType==PageItem::PolyLine || itemType==PageItem::Spiral || itemType == PageItem::RegularPolygon || itemType == PageItem::Arc)
	{
		newItem->PLineArt = Qt::PenStyle(docPrefsData.itemToolPrefs.shapeLineStyle);
		newItem->setFillShade(docPrefsData.itemToolPrefs.shapeFillColorShade);
		newItem->setLineShade(docPrefsData.itemToolPrefs.shapeLineColorShade);
		if ((itemType == PageItem::Polygon) || (itemType == PageItem::RegularPolygon) || (itemType == PageItem::Arc))
			newItem->ContourLine = newItem->PoLine.copy();
	}
}


bool ScribusDoc::itemAddCommit(const int /*itemNumber*/)
{
	//TODO use the parameter
	if (m_itemCreationTransaction && appMode !=  modeDrawBezierLine)
	{
		PageItem *createdItem=m_Selection->itemAt(0);
		if (createdItem!=NULL)
		{
			createdItem->checkChanges(true);
			QString targetName = Um::ScratchSpace;
			if (createdItem->OwnPage > -1)
				targetName = Pages->at(createdItem->OwnPage)->getUName();
			m_itemCreationTransaction->commit(targetName, createdItem->getUPixmap(),
								Um::Create + " " + createdItem->getUName(),  "", Um::ICreate);
			delete m_itemCreationTransaction;
			m_itemCreationTransaction = NULL;
			if (!isLoading())
				return true;
		}
	}
	return false;
}


uint ScribusDoc::getItemNrfromUniqueID(uint unique)
{
	uint ret = 0;
	for (int a = 0; a < Items->count(); ++a)
	{
		if (Items->at(a)->uniqueNr == unique)
		{
			ret = Items->at(a)->ItemNr;
			break;
		}
	}
	return ret;
}


void ScribusDoc::updateFrameItems()
{
	uint frameItemsCount=FrameItems.count();
	for (uint a = 0; a < frameItemsCount; ++a)
		FrameItems.at(a)->ItemNr = a;
}


void ScribusDoc::renumberItemsInListOrder( )
{
	m_docUpdater->beginUpdate();
	m_updateManager.setUpdatesEnabled(false);
	int itemsCount=Items->count();
	for (int i = 0; i < itemsCount; ++i)
	{
		Items->at(i)->ItemNr = i;
		Items->at(i)->checkTextFlowInteractions(true);
	}
	m_updateManager.setUpdatesEnabled(true);
	m_docUpdater->endUpdate();
}


void ScribusDoc::rebuildItemLists()
{
	// #5826 Rebuild items list in case layer order as been changed
	QList<PageItem*> newDocItems, newMasterItems;
	Layers.sort();
	uint layerCount = this->layerCount();
	uint itemIndex  = 0, masterIndex = 0;
	for (uint la = 0; la < layerCount; ++la)
	{
		PageItem* currItem;
		int layerID = Layers.at(la).ID;
		for (int it = 0; it < DocItems.count(); ++it)
		{
			currItem = DocItems.at(it);
			if (currItem->LayerID != layerID)
				continue;
			newDocItems.append(currItem);
			currItem->ItemNr = itemIndex++;
		}
		for (int it = 0; it < MasterItems.count(); ++it)
		{
			currItem = MasterItems.at(it);
			if (currItem->LayerID != layerID)
				continue;
			newMasterItems.append(currItem);
			currItem->ItemNr = masterIndex++;
		}
	}
	DocItems    = newDocItems;
	MasterItems = newMasterItems;
}


bool ScribusDoc::usesAutomaticTextFrames() const
{
	return automaticTextFrames;
}


void ScribusDoc::setUsesAutomaticTextFrames(const bool atf)
{
	automaticTextFrames=atf;
}


bool ScribusDoc::LoadPict(QString fn, int ItNr, bool reload, bool showMsg )
{
	return loadPict(fn, Items->at(ItNr), reload, showMsg);
}


bool ScribusDoc::loadPict(QString fn, PageItem *pageItem, bool reload, bool showMsg)
{
	if (!reload)
	{
		if ((ScCore->fileWatcher->files().contains(pageItem->Pfile) != 0) && (pageItem->PictureIsAvailable))
		{
			ScCore->fileWatcher->removeFile(pageItem->Pfile);
			if (pageItem->tempImageFile != NULL)
				delete pageItem->tempImageFile;
			pageItem->tempImageFile = NULL;
			pageItem->isInlineImage = false;
		}
	}
	if(!pageItem->loadImage(fn, reload, -1, showMsg))
	{
		if (!reload)
		{
			if (m_hasGUI)
			{
				QFileInfo fi(pageItem->Pfile);
				ScCore->fileWatcher->addDir(fi.absolutePath());
			}
		}
		return false;
	}
	if (!reload)
	{
		if (m_hasGUI)
		{
			QFileInfo fi(pageItem->Pfile);
			ScCore->fileWatcher->addDir(fi.absolutePath());
			ScCore->fileWatcher->addFile(pageItem->Pfile);
		}
	}
	if (!isLoading())
	{
		//TODO: Previously commented out.. unsure why, remove later
		//emit UpdtObj(PageNr, ItNr);
		//CB: probably because we are therefore not always refreshing the view when an image changes...
		pageItem->update();
		changed();
	}
	return true;
}


void ScribusDoc::canvasMinMax(FPoint& minPoint, FPoint& maxPoint)
{
	PageItem *currItem;
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	uint docItemsCount=Items->count();
	if (docItemsCount != 0)
	{
		for (uint ic = 0; ic < docItemsCount; ++ic)
		{
			currItem = Items->at(ic);
			if (currItem->rotation() != 0)
			{
				FPointArray pb;
				pb.resize(0);
				pb.addPoint(FPoint(currItem->xPos(), currItem->yPos()));
				FPoint p1(currItem->width(), 0.0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0);
				pb.addPoint(p1);
				FPoint p2(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0);
				pb.addPoint(p2);
				FPoint p3(0.0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0);
				pb.addPoint(p3);
				for (uint pc = 0; pc < 4; ++pc)
				{
					minx = qMin(minx, pb.point(pc).x());
					miny = qMin(miny, pb.point(pc).y());
					maxx = qMax(maxx, pb.point(pc).x());
					maxy = qMax(maxy, pb.point(pc).y());
				}
			}
			else
			{
				minx = qMin(minx, currItem->xPos());
				miny = qMin(miny, currItem->yPos());
				maxx = qMax(maxx, currItem->xPos() + currItem->width());
				maxy = qMax(maxy, currItem->yPos() + currItem->height());
			}
		}
	}
	else
	{
		Page* Seite;
		uint docPageCount=Pages->count();
		MarginStruct pageBleeds;
		for (uint a = 0; a < docPageCount; ++a)
		{
			Seite = Pages->at(a);
			getBleeds(Seite, pageBleeds);
			minx = qMin(minx, Seite->xOffset() - pageBleeds.Left);
			miny = qMin(miny, Seite->yOffset() - pageBleeds.Top);
			maxx = qMax(maxx, Seite->xOffset() + Seite->width() + pageBleeds.Left + pageBleeds.Right);
			maxy = qMax(maxy, Seite->yOffset() + Seite->height() + pageBleeds.Top + pageBleeds.Bottom);
		}
	}
	minPoint.setX(minx);
	minPoint.setY(miny);
	maxPoint.setX(maxx);
	maxPoint.setY(maxy);
}


int ScribusDoc::OnPage(double x2, double  y2)
{
	int retw = -1;
	if (masterPageMode())
	{
		int x = static_cast<int>(currentPage()->xOffset() - docPrefsData.docSetupPrefs.bleeds.Left);
		int y = static_cast<int>(currentPage()->yOffset() - docPrefsData.docSetupPrefs.bleeds.Top);
		int w = static_cast<int>(currentPage()->width() + docPrefsData.docSetupPrefs.bleeds.Left + docPrefsData.docSetupPrefs.bleeds.Right);
		int h = static_cast<int>(currentPage()->height() + docPrefsData.docSetupPrefs.bleeds.Bottom + docPrefsData.docSetupPrefs.bleeds.Top);
		if (QRect(x, y, w, h).contains(qRound(x2), qRound(y2)))
			retw = currentPage()->pageNr();
	}
	else
	{
		uint docPageCount = Pages->count();
		MarginStruct pageBleeds;
		for (uint a = 0; a < docPageCount; ++a)
		{
			getBleeds(a, pageBleeds);
			int x = static_cast<int>(Pages->at(a)->xOffset() - pageBleeds.Left);
			int y = static_cast<int>(Pages->at(a)->yOffset() - pageBleeds.Top);
			int w = static_cast<int>(Pages->at(a)->width() + pageBleeds.Left + pageBleeds.Right);
			int h = static_cast<int>(Pages->at(a)->height() + pageBleeds.Bottom + pageBleeds.Top);
			if (QRect(x, y, w, h).contains(qRound(x2), qRound(y2)))
			{
				retw = static_cast<int>(a);
				break;
			}
		}
	}
	return retw;
}


int ScribusDoc::OnPage(PageItem *currItem)
{
	int retw = -1;
	if (masterPageMode())
	{
/*		int x = static_cast<int>(currentPage()->xOffset());
		int y = static_cast<int>(currentPage()->yOffset());
		int w = static_cast<int>(currentPage()->width());
		int h = static_cast<int>(currentPage()->height());
		QRect itemRect(qRound(currItem->BoundingX), qRound(currItem->BoundingY), qRound(currItem->BoundingW), qRound(currItem->BoundingH));
		if (QRect(x, y, w, h).intersects(itemRect)) */
		double x = currentPage()->xOffset() - docPrefsData.docSetupPrefs.bleeds.Left;
		double y = currentPage()->yOffset() - docPrefsData.docSetupPrefs.bleeds.Top;
		double w = currentPage()->width() + docPrefsData.docSetupPrefs.bleeds.Left + docPrefsData.docSetupPrefs.bleeds.Right;
		double h1 = currentPage()->height() + docPrefsData.docSetupPrefs.bleeds.Bottom + docPrefsData.docSetupPrefs.bleeds.Top;
		double x2 = currItem->BoundingX;
		double y2 = currItem->BoundingY;
		double w2 = currItem->BoundingW;
		double h2 = currItem->BoundingH;
		if (( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )))
			retw = currentPage()->pageNr();
	}
	else
	{
		MarginStruct pageBleeds;
		uint docPageCount = Pages->count();
		for (uint a = 0; a < docPageCount; ++a)
		{
			getBleeds(a, pageBleeds);
			double x = Pages->at(a)->xOffset() - pageBleeds.Left;
			double y = Pages->at(a)->yOffset() - pageBleeds.Top;
			double w = Pages->at(a)->width() + pageBleeds.Left + pageBleeds.Right;
			double h1 = Pages->at(a)->height() + pageBleeds.Bottom + pageBleeds.Top;
			double x2 = currItem->BoundingX;
			double y2 = currItem->BoundingY;
			double w2 = currItem->BoundingW;
			double h2 = currItem->BoundingH;
			if (( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )))
			{
				retw = static_cast<int>(a);
				break;
			}
/*			int x = static_cast<int>(Pages->at(a)->xOffset());
			int y = static_cast<int>(Pages->at(a)->yOffset());
			int w = static_cast<int>(Pages->at(a)->width());
			int h = static_cast<int>(Pages->at(a)->height());
			QRect itemRect(qRound(currItem->BoundingX), qRound(currItem->BoundingY), qRound(currItem->BoundingW), qRound(currItem->BoundingH));
			if (QRect(x, y, w, h).intersects(itemRect))
			{
				retw = static_cast<int>(a);
				break;
			} */
		}
	}
	if ((retw == -1) && (currItem->isBookmark))
	{
		//FIXME emit DelBM(currItem);
		currItem->isBookmark = false;
	}
	return retw;
}


void ScribusDoc::GroupOnPage(PageItem* currItem)
{
	if (!currItem->isGroup())
		return;
	QList<PageItem*> Objects = currItem->getItemList();
	int Off_Page = -1;
	int On_Page = 999999;
	uint objectCount = Objects.count();
	for (uint a = 0; a < objectCount; ++a)
	{
		int res = OnPage(Objects.at(a));
		Off_Page = qMax(Off_Page, res);
		if (res != -1)
			On_Page = qMin(On_Page, res);
	}
	int final = -1;
	if (Off_Page != -1)
		final = On_Page;
	for (uint a = 0; a < objectCount; ++a)
	{
		Objects.at(a)->OwnPage = final;
	}
}


struct oldPageVar
{
	uint newPg;
	double oldXO;
	double oldYO;
};


//CB TODO make a function to determine the place of the page.. ie, so we know the left and right margins
// without running this monster
void ScribusDoc::reformPages(bool moveObjects)
{
	QMap<uint, oldPageVar> pageTable;
	struct oldPageVar oldPg;
	int counter = pageSets()[docPrefsData.docSetupPrefs.pagePositioning].FirstPage;
	int rowcounter = 0;
	double maxYPos=0.0, maxXPos=0.0;
	double currentXPos=docPrefsData.displayPrefs.scratch.Left, currentYPos=docPrefsData.displayPrefs.scratch.Top, lastYPos=Pages->at(0)->initialHeight();
//	currentXPos += (pageWidth+pageSets[currentPageLayout].GapHorizontal) * counter;
	currentXPos += (docPrefsData.docSetupPrefs.pageWidth+docPrefsData.displayPrefs.pageGapHorizontal) * counter;

	lastYPos = Pages->at(0)->initialHeight();
	Page* Seite;
	uint docPageCount=Pages->count();
	for (uint a = 0; a < docPageCount; ++a)
	{
		Seite = Pages->at(a);
		oldPg.oldXO = Seite->xOffset();
		oldPg.oldYO = Seite->yOffset();
		oldPg.newPg = a;
		pageTable.insert(Seite->pageNr(), oldPg);
		Seite->setPageNr(a);
		if (masterPageMode())
		{
			Seite->setXOffset(docPrefsData.displayPrefs.scratch.Left);
			Seite->setYOffset(docPrefsData.displayPrefs.scratch.Top);
			if (Seite->LeftPg == 0)
			{
				Seite->Margins.Right = Seite->initialMargins.Right;
				Seite->Margins.Left = Seite->initialMargins.Left;
			}
			else if ((Seite->LeftPg > 1) && (Seite->LeftPg < pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns))
			{
				Seite->Margins.Left = Seite->initialMargins.Left;
				Seite->Margins.Right = Seite->initialMargins.Left;
			}
			else
			{
				Seite->Margins.Left = Seite->initialMargins.Right;
				Seite->Margins.Right = Seite->initialMargins.Left;
			}
		}
		else
		{
			Seite->setWidth(Seite->initialWidth());
			Seite->setHeight(Seite->initialHeight());
			Seite->setXOffset(currentXPos);
			Seite->setYOffset(currentYPos);
			if (counter < pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns-1)
			{
//				currentXPos += Seite->width() + pageSets[currentPageLayout].GapHorizontal;
				currentXPos += Seite->width() + docPrefsData.displayPrefs.pageGapHorizontal;
				lastYPos = qMax(lastYPos, Seite->height());
				if (counter == 0)
				{
					Seite->Margins.Left = Seite->initialMargins.Right;
					Seite->Margins.Right = Seite->initialMargins.Left;
				}
				else
				{
					Seite->Margins.Left = Seite->initialMargins.Left;
					Seite->Margins.Right = Seite->initialMargins.Left;
				}
			}
			else
			{
				currentXPos = docPrefsData.displayPrefs.scratch.Left;
				if (pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns > 1)
					currentYPos += qMax(lastYPos, Seite->height())+docPrefsData.displayPrefs.pageGapVertical;
//					currentYPos += qMax(lastYPos, Seite->height())+pageSets[currentPageLayout].GapVertical;
				else
					currentYPos += Seite->height()+docPrefsData.displayPrefs.pageGapVertical;
//					currentYPos += Seite->height()+pageSets[currentPageLayout].GapVertical;
//				lastYPos = qMax(lastYPos, Seite->height());
				lastYPos = 0;
				Seite->Margins.Right = Seite->initialMargins.Right;
				Seite->Margins.Left = Seite->initialMargins.Left;
			}
			counter++;
			if (counter > pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns-1)
			{
				counter = 0;
				rowcounter++;
				if (rowcounter > pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Rows-1)
				{
//					currentYPos += GapVertical;
//					currentYPos += pageSets[currentPageLayout].GapBelow;
					rowcounter = 0;
				}
			}
		}
		Seite->Margins.Top = Seite->initialMargins.Top;
		Seite->Margins.Bottom = Seite->initialMargins.Bottom;
		maxXPos = qMax(maxXPos, Seite->xOffset()+Seite->width()+docPrefsData.displayPrefs.scratch.Right);
		maxYPos = qMax(maxYPos, Seite->yOffset()+Seite->height()+docPrefsData.displayPrefs.scratch.Bottom);
	}
	if (!isLoading())
	{
		undoManager->setUndoEnabled(false);
		uint docItemsCount=Items->count();
		for (uint ite = 0; ite < docItemsCount; ++ite)
		{
			PageItem *item = Items->at(ite);
			if (item->OwnPage < 0)
			{
				if (item->isGroup())
					GroupOnPage(item);
				else
					item->OwnPage = OnPage(item);
			}
			else
			{
				if (moveObjects)
				{
					oldPg = pageTable[item->OwnPage];
					item->moveBy(-oldPg.oldXO + Pages->at(oldPg.newPg)->xOffset(), -oldPg.oldYO + Pages->at(oldPg.newPg)->yOffset());
					item->OwnPage = static_cast<int>(oldPg.newPg);
				}
				else
				{
					if (item->isGroup())
						GroupOnPage(item);
					else
						item->OwnPage = OnPage(item);
				}
			}
			item->setRedrawBounding();
		}
		undoManager->setUndoEnabled(true);
	}

	if(isLoading() && is12doc)
		return;
	if (!isLoading())
	{
		FPoint minPoint, maxPoint;
		canvasMinMax(minPoint, maxPoint);
		FPoint maxSize(qMax(maxXPos, maxPoint.x()+docPrefsData.displayPrefs.scratch.Right), qMax(maxYPos, maxPoint.y()+docPrefsData.displayPrefs.scratch.Bottom));
		adjustCanvas(FPoint(qMin(0.0, minPoint.x()-docPrefsData.displayPrefs.scratch.Left),qMin(0.0, minPoint.y()-docPrefsData.displayPrefs.scratch.Top)), maxSize, true);
		changed();
	}
	else
	{
		FPoint maxSize(maxXPos, maxYPos);
		adjustCanvas(FPoint(0, 0), maxSize);
	}
}

double ScribusDoc::getXOffsetForPage(const int pageNumber)
{
	if (Pages->at(pageNumber)!=NULL)
		return Pages->at(pageNumber)->xOffset();
	return -1.0;
}

double ScribusDoc::getYOffsetForPage(const int pageNumber)
{
	if (Pages->at(pageNumber)!=NULL)
		return Pages->at(pageNumber)->yOffset();
	return -1.0;
}

void ScribusDoc::getBleeds(int pageNumber, MarginStruct &bleedData)
{
	if (pageNumber >= 0 && pageNumber < Pages->size())
		getBleeds(Pages->at(pageNumber), bleedData);
	else
		qCritical() << "Attempting to get bleeds for non-existant page";
}

void ScribusDoc::getBleeds(const Page* page, MarginStruct& bleedData)
{
	getBleeds(page, docPrefsData.docSetupPrefs.bleeds, bleedData);
}

void ScribusDoc::getBleeds(const Page* page, const MarginStruct& baseValues, MarginStruct& bleedData)
{
	bleedData.Bottom = baseValues.Bottom;
	bleedData.Top    = baseValues.Top;
	if (pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns == 1)
	{
		bleedData.Right = baseValues.Right;
		bleedData.Left  = baseValues.Left;
	}
	else
	{
		PageLocation pageLocation = MiddlePage;
		if (page->pageName().isEmpty()) // Standard page
			pageLocation = locationOfPage(page->pageNr());
		else if (page->LeftPg == 1) // Left Master page
			pageLocation = LeftPage;
		else if (page->LeftPg == 0) // Right Master page
			pageLocation = RightPage;
		else // Middle Master page
			pageLocation = MiddlePage;

		if (pageLocation == LeftPage)
		{
			bleedData.Right = baseValues.Left;
			bleedData.Left  = baseValues.Right;
		}
		else if (pageLocation == RightPage)
		{
			bleedData.Right = baseValues.Right;
			bleedData.Left  = baseValues.Left;
		}
		else
		{
			bleedData.Right = baseValues.Left;
			bleedData.Left  = baseValues.Left;
		}
	}
}


PageItem* ScribusDoc::convertItemTo(PageItem *currItem, PageItem::ItemType newType, PageItem* secondaryItem)
{
	//Item to convert is null, return
	Q_ASSERT(currItem!=NULL);
	if (currItem == NULL)
		return NULL;
	//Dont attempt a Line conversion
//	if (currItem->itemType()==PageItem::Line || newType==PageItem::Line)
	if (newType==PageItem::Line)
		return NULL;
	//Take the item to convert from the docs Items list
//	PageItem *oldItem = Items->take(currItem->ItemNr);
	// Don't use take as we will insert the new item later at the same position
	PageItem *oldItem = Items->at(currItem->ItemNr);
	uint oldItemNr = currItem->ItemNr;
	//Remove old item from the doc's selection if it was in it
	bool removedFromSelection=m_Selection->removeItem(oldItem);
	//Create a new item from the old one
	UndoTransaction* transactionConversion = NULL;
	PageItem *newItem;
	switch (newType)
	{
		case PageItem::ImageFrame:
			newItem = new PageItem_ImageFrame(*oldItem);
			break;
		case PageItem::TextFrame:
			newItem = new PageItem_TextFrame(*oldItem);
			if (UndoManager::undoEnabled() && oldItem->itemType()==PageItem::PathText)
			{
				transactionConversion = new UndoTransaction(undoManager->beginTransaction(currentPage()->getUName(), 0, Um::TextFrame, "", Um::ITextFrame));
			}
			break;
		//We dont allow this
/*		case PageItem::Line:
			newItem = new PageItem_PolyLine(*oldItem);
			break; */
		case PageItem::Polygon:
			newItem = new PageItem_Polygon(*oldItem);
			break;
		case PageItem::PolyLine:
			newItem = new PageItem_PolyLine(*oldItem);
			break;
		case PageItem::PathText:
			if (secondaryItem==NULL)
				return NULL;
			if (UndoManager::undoEnabled())
			{
				transactionConversion = new UndoTransaction(undoManager->beginTransaction(currentPage()->getUName(), 0, Um::PathText, "", Um::ITextFrame));
			}
			newItem = new PageItem_PathText(*oldItem);
			break;
		default:
			newItem=NULL;
			break;
	}
	Q_ASSERT(newItem!=NULL);
	//If the new item is null, return. If converting Text to Path, start a transaction
	//as the old bezier will be deleted
	if (newItem==NULL)
	{
		if (transactionConversion)
		{
			transactionConversion->cancel();
			delete transactionConversion;
		}
		return NULL;
	}
	//Do new item type specific adjustments to the new item. Some of this may move when new
	//constructors are built into the item classes
	switch (newType)
	{
		case PageItem::ImageFrame:
			newItem->convertTo(PageItem::ImageFrame);
			newItem->Frame = true;
			break;
		case PageItem::TextFrame:
			newItem->convertTo(PageItem::TextFrame);
			newItem->Frame = true;
			if (oldItem->itemType()==PageItem::PathText)
			{
				uint newPolyItemNo = itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), CommonStrings::None, currItem->lineColor(), true);
				PageItem *polyLineItem = Items->at(newPolyItemNo);
				polyLineItem->PoLine = currItem->PoLine.copy();
				polyLineItem->ClipEdited = true;
				polyLineItem->FrameType = 3;
				polyLineItem->setRotation(currItem->rotation());
//				polyLineItem->setPolyClip(qRound(qMax(polyLineItem->lineWidth() / 2, 1)));
				AdjustItemSize(polyLineItem);

				newItem->setLineColor(CommonStrings::None);
				newItem->SetRectFrame();
				newItem->setRedrawBounding();
			}
			break;
		//We dont allow this right now
	/*	case PageItem::Line:
			break; */
		case PageItem::Polygon:
			newItem->convertTo(PageItem::Polygon);
			newItem->Frame = false;
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
			if(oldItem->itemType()==PageItem::PolyLine)
			{
				newItem->PoLine.addPoint(newItem->PoLine.point(newItem->PoLine.size()-2));
				newItem->PoLine.addPoint(newItem->PoLine.point(newItem->PoLine.size()-3));
				newItem->PoLine.addPoint(newItem->PoLine.point(0));
				newItem->PoLine.addPoint(newItem->PoLine.point(0));
			}
			newItem->Clip = FlattenPath(newItem->PoLine, newItem->Segments);
			newItem->ContourLine = newItem->PoLine.copy();
			break;
		case PageItem::PolyLine:
			newItem->convertTo(PageItem::PolyLine);
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
			if(oldItem->itemType()==PageItem::Line)
			{
				QTransform ma;
				newItem->Frame = false;
				newItem->FrameType = 3;
				ma.rotate(newItem->rotation());
				newItem->PoLine.resize(0);
				newItem->PoLine.addPoint(0.0, 0.0);
				newItem->PoLine.addPoint(0.0, 0.0);
				newItem->PoLine.addPoint(newItem->width(), 0.0);
				newItem->PoLine.addPoint(newItem->width(), 0.0);
				newItem->PoLine.map(ma);
				newItem->setRotation(0.0);
			}
			AdjustItemSize(newItem);
			break;
		case PageItem::PathText:
			{
				newItem->convertTo(PageItem::PathText);
				newItem->Frame = true;
				newItem->ClipEdited = true;
				newItem->PoLine = secondaryItem->PoLine.copy();
				newItem->setLineWidth(secondaryItem->lineWidth());
				newItem->setLineColor(secondaryItem->lineColor());
				newItem->PLineArt = secondaryItem->PLineArt;
				newItem->PLineEnd = secondaryItem->PLineEnd;
				newItem->PLineJoin = secondaryItem->PLineJoin;
				/*	if (!Doc->loading)
					emit UpdtObj(Doc->currentPage->pageNr(), b->ItemNr); */
				//FIXME: Stop using the view here
				AdjustItemSize(newItem);
//				newItem->updatePolyClip();
				double dx = secondaryItem->xPos() - newItem->xPos();
				double dy = secondaryItem->yPos() - newItem->yPos();
				MoveItem(dx, dy, newItem);
				newItem->setRotation(secondaryItem->rotation());
				newItem->FrameType = 3;
			}
			break;
		default:
			newItem=NULL;
			break;
	}
	//Append the new item to the docs items list
	//Items->append(newItem);
	//We could append and renumber the list, or, we can insert at the same position..
	//for (uint a = 0; a < Items->count(); ++a)
	//	Items->at(a)->ItemNr = a;
//	Items->insert(oldItem->ItemNr, newItem);
	newItem->ItemNr = oldItemNr;
	newItem->uniqueNr = oldItem->uniqueNr;
	Items->replace(oldItemNr, newItem);
	//FIXME: shouldn't we delete the oldItem ???
	//Add new item back to selection if old item was in selection
	if (removedFromSelection)
		m_Selection->addItem(newItem);
	// If converting a text frame to another object, drop links
	if (oldItem->asTextFrame() && (newType != PageItem::TextFrame))
		oldItem->dropLinks();
	// If converting text to path, delete the bezier
	if (newType == PageItem::PathText)
	{
		//FIXME: Stop using the view here
		m_View->SelectItem(secondaryItem);
		itemSelection_DeleteItem();
		regionsChanged()->update(QRectF());
		m_View->Deselect(true);
	}
	//Create the undo action for the new item
	if (UndoManager::undoEnabled())
	{
		ItemState<std::pair<PageItem*, PageItem*> > *is = new ItemState<std::pair<PageItem*, PageItem*> >("Convert Item");
		is->set("CONVERT_ITEM", "convert_item");
		is->setItem(std::pair<PageItem*, PageItem*>(oldItem, newItem));
		//Undo target rests with the Page for object specific undo
		UndoObject *target = Pages->at(0);
		if (newItem->OwnPage > -1)
			target = Pages->at(newItem->OwnPage);
		undoManager->action(target, is);
	}
	//Close any undo transaction
	if (transactionConversion)
	{
		transactionConversion->commit();
		delete transactionConversion;
		transactionConversion = NULL;
	}
	return newItem;
}

int ScribusDoc::currentPageNumber()
{
	return currentPage()->pageNr();
}


bool ScribusDoc::itemNameExists(const QString checkItemName)
{
	bool found = false;
	uint docItemCount=Items->count();
	for (uint i = 0; i < docItemCount; ++i)
	{
		if (checkItemName == Items->at(i)->itemName())
		{
			found = true;
			break;
		}
	}
	return found;
}


void ScribusDoc::setMasterPageMode(bool changeToMasterPageMode)
{
	if (changeToMasterPageMode == m_masterPageMode)
		return;
	if (changeToMasterPageMode)
	{
		Pages = &MasterPages;
		Items = &MasterItems;
	}
	else
	{
		Pages = &DocPages;
		Items = &DocItems;
	}
	m_masterPageMode = changeToMasterPageMode;
}

void ScribusDoc::setSymbolEditMode(bool mode, QString symbolName)
{
	if (mode == m_symbolEditMode)
		return;
	m_symbolEditMode = mode;
	if (mode)
	{
		ScPattern pa = docPatterns[symbolName];
		currentEditedSymbol = symbolName;
		Page* addedPage = new Page(docPrefsData.displayPrefs.scratch.Left, docPrefsData.displayPrefs.scratch.Top, pa.width, pa.height);
		addedPage->setDocument(this);
		addedPage->Margins.Top = 0;
		addedPage->Margins.Bottom = 0;
		addedPage->Margins.Left = 0;
		addedPage->Margins.Right = 0;
		addedPage->initialMargins.Top = 0;
		addedPage->initialMargins.Bottom = 0;
		addedPage->initialMargins.Left = 0;
		addedPage->initialMargins.Right = 0;
		addedPage->setPageNr(0);
		addedPage->MPageNam = "";
		addedPage->setPageName("");
		TempPages.clear();
		TempPages.append(addedPage);
		Pages = &TempPages;
		Items = &docPatterns[symbolName].items;
		renumberItemsInListOrder();
		m_Selection->delaySignalsOn();
		for (int as = 0; as < Items->count(); ++as)
		{
			m_Selection->addItem(Items->at(as));
		}
		QRectF sR = m_Selection->getGroupRect();
		moveGroup(-sR.x() + addedPage->xOffset(), -sR.y() + addedPage->yOffset());
		m_Selection->clear();
		m_Selection->delaySignalsOff();
		changed();
	}
	else
	{
		PageItem* currItem = Items->at(0);
		Page* addedPage = TempPages.at(0);
		if (Items->count() > 1)
		{
			if ((!currItem->isGroup()) && (Items->count() > 1))
			{
				itemAdd(PageItem::Group, PageItem::Rectangle, addedPage->xOffset(), addedPage->yOffset(), 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
				PageItem *groupItem = Items->takeLast();
				Items->insert(0, groupItem);
				double minx =  std::numeric_limits<double>::max();
				double miny =  std::numeric_limits<double>::max();
				double maxx = -std::numeric_limits<double>::max();
				double maxy = -std::numeric_limits<double>::max();
				for (int as = 1; as < Items->count(); ++as)
				{
					PageItem* currItem = Items->at(as);
					groupItem->groupItemList.append(currItem);
					double x1, x2, y1, y2;
					currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
					minx = qMin(minx, x1);
					miny = qMin(miny, y1);
					maxx = qMax(maxx, x2);
					maxy = qMax(maxy, y2);
				}
				for (int em = 0; em < groupItem->groupItemList.count(); ++em)
				{
					PageItem* currItem = groupItem->groupItemList.at(em);
					currItem->gXpos = currItem->xPos() - minx;
					currItem->gYpos = currItem->yPos() - miny;
					currItem->gWidth = maxx - minx;
					currItem->gHeight = maxy - miny;
				}
				groupItem->setXYPos(minx, miny, true);
				groupItem->setWidthHeight(maxx - minx, maxy - miny, true);
				groupItem->groupWidth = maxx - minx;
				groupItem->groupHeight = maxy - miny;
				groupItem->SetRectFrame();
				groupItem->ClipEdited = true;
				groupItem->FrameType = 3;
				groupItem->setTextFlowMode(PageItem::TextFlowDisabled);
				groupItem->AutoName = false;
				groupItem->setFillTransparency(0);
				groupItem->setLineTransparency(0);
				GroupCounter++;
				renumberItemsInListOrder();
			}
		}
		currItem = Items->at(0);
		docPatterns[currentEditedSymbol].pattern = currItem->DrawObj_toImage(qMax(currItem->gWidth, currItem->gHeight));
		docPatterns[currentEditedSymbol].width = currItem->gWidth;
		docPatterns[currentEditedSymbol].height = currItem->gHeight;
		if (m_ScMW->patternsDependingOnThis.count() > 1)
		{
			for (int a = 1; a < m_ScMW->patternsDependingOnThis.count(); a++)
			{
				Items = &docPatterns[m_ScMW->patternsDependingOnThis[a]].items;
				renumberItemsInListOrder();
				currItem = Items->at(0);
				docPatterns[m_ScMW->patternsDependingOnThis[a]].pattern = currItem->DrawObj_toImage(qMax(currItem->gWidth, currItem->gHeight));
			}
		}
		if (masterPageMode())
		{
			Pages = &MasterPages;
			Items = &MasterItems;
		}
		else
		{
			Pages = &DocPages;
			Items = &DocItems;
		}
		delete addedPage;
	}
}

void ScribusDoc::addSection(const int number, const QString& name, const uint fromindex, const uint toindex, const DocumentSectionType type, const uint sectionstartindex, const bool reversed, const bool active, const QChar fillChar, int fieldWidth)
{
	struct DocumentSection newSection;
	uint docPageCount=DocPages.count();
	bool empty=docPrefsData.docSectionMap.isEmpty();
	if (empty)
	{
		newSection.number=0;
		newSection.name="0";
		newSection.fromindex=0;
		newSection.toindex=docPageCount-1;
		newSection.type=Type_1_2_3;
		newSection.sectionstartindex=1;
		newSection.reversed=false;
		newSection.active=true;
		newSection.pageNumberFillChar=QChar();
		newSection.pageNumberWidth=0;
		docPrefsData.docSectionMap.insert(newSection.number, newSection);
	}
	else if (number!=-1)
	{
		newSection.number=number;
		newSection.name=name;
		newSection.fromindex=fromindex;
		newSection.toindex=toindex;
		if (newSection.toindex>docPageCount-1)
			newSection.toindex=docPageCount-1;
		newSection.type=type;
		newSection.sectionstartindex=sectionstartindex;
		newSection.reversed=reversed;
		newSection.active=active;
		newSection.pageNumberFillChar=fillChar;
		newSection.pageNumberWidth=fieldWidth;
		docPrefsData.docSectionMap.insert(newSection.number, newSection);
	}
}


bool ScribusDoc::deleteSection(const uint number)
{
	if (!docPrefsData.docSectionMap.contains(number))
		return false;
	if (docPrefsData.docSectionMap.count()<=1)
		return false;
	QMap<uint, DocumentSection>::Iterator itprev=docPrefsData.docSectionMap.begin();
	QMap<uint, DocumentSection>::Iterator it=itprev;
	uint currMinIndex = itprev.value().fromindex;
	uint currMaxIndex = itprev.value().toindex;
	for ( ; it != docPrefsData.docSectionMap.end(); ++it )
	{
		currMinIndex=it.value().fromindex;
		currMaxIndex=it.value().toindex;

		if (it.key()!=number)
			itprev=it;
		else
			break;
	}
	if (it != itprev)
		itprev.value().toindex=currMaxIndex;
	else {
		// special case: delete first section
		QMap<uint, DocumentSection>::Iterator itnext = it;
		++itnext;
		itnext.value().fromindex=it.value().fromindex;
	}
	docPrefsData.docSectionMap.erase(it);
	return true;
}


int ScribusDoc::getSectionKeyForPageIndex(const uint pageIndex) const
{
	bool found=false;
	int retVal=-1;
	DocumentSectionMap::ConstIterator it = docPrefsData.docSectionMap.begin();
	for (; it!= docPrefsData.docSectionMap.end(); ++it)
	{
		if (pageIndex>=it.value().fromindex && pageIndex<=it.value().toindex)
		{
			found=true;
			retVal=it.key();
			break;
		}
	}

	return retVal;
}


const QString ScribusDoc::getSectionPageNumberForPageIndex(const uint pageIndex) const
{
	QString retVal(QString::null);
	int key=getSectionKeyForPageIndex(pageIndex);
	if (key==-1)
		return retVal;
	//If a section is inactive, theres no page numbers printed
	if (docPrefsData.docSectionMap[key].active==false)
		return "";
	uint sectionIndexOffset=pageIndex-docPrefsData.docSectionMap[key].fromindex+docPrefsData.docSectionMap[key].sectionstartindex;
	retVal=getStringFromSequence(docPrefsData.docSectionMap[key].type, sectionIndexOffset);
	return retVal;
}

const QChar ScribusDoc::getSectionPageNumberFillCharForPageIndex(const uint pageIndex) const
{
	QChar retVal;
	int key=getSectionKeyForPageIndex(pageIndex);
	if (key==-1)
		return retVal;

	//If a section is inactive, theres no page numbers printed
	if (docPrefsData.docSectionMap[key].active==false)
		return retVal;
	retVal = docPrefsData.docSectionMap[key].pageNumberFillChar;
	if (retVal == QChar(0))
		retVal = QChar(32);
	return retVal;
}

int ScribusDoc::getSectionPageNumberWidthForPageIndex(const uint pageIndex) const
{
	int retVal=0;
	int key=getSectionKeyForPageIndex(pageIndex);
	if (key==-1)
		return retVal;

	//If a section is inactive, theres no page numbers printed
	if (docPrefsData.docSectionMap[key].active==false)
		return retVal;
	retVal = qMin(docPrefsData.docSectionMap[key].pageNumberWidth, 20);			// added the qmin as a sanity check -> fixes bug #9721
	return retVal;
}

void ScribusDoc::updateSectionPageNumbersToPages()
{
	uint docPageCount=DocPages.count();
	for (uint i=0;i<docPageCount;++i)
		DocPages.at(i)->setPageSectionNumber(getSectionPageNumberForPageIndex(i));
}


void ScribusDoc::addPageToSection(const uint otherPageIndex, const uint location, const uint count)
{
	uint fromIndex, toIndex;
	uint searchedIndex = (otherPageIndex > 0) ? (otherPageIndex - 1) : 0;
	if ((location == 0) && (searchedIndex > 0))
		--searchedIndex;
	DocumentSectionMap::Iterator it = docPrefsData.docSectionMap.begin();
	for (; it!= docPrefsData.docSectionMap.end(); ++it)
	{
		fromIndex = it.value().fromindex;
		toIndex   = it.value().toindex;
		if  (fromIndex > searchedIndex)
			it.value().fromindex += count;
		if  (toIndex >= searchedIndex)
			it.value().toindex += count;
	}
	//Now update the Pages' internal storage of their page number
	updateSectionPageNumbersToPages();
}


void ScribusDoc::removePageFromSection(const uint pageIndex)
{
	//Get the section of the new page index.
	uint fromIndex, toIndex;
	DocumentSectionMap::Iterator it = docPrefsData.docSectionMap.begin();
	for (; it!= docPrefsData.docSectionMap.end(); ++it)
	{
		if (pageIndex>=it.value().fromindex && pageIndex<=it.value().toindex)
		{
			fromIndex = it.value().fromindex;
			toIndex   = it.value().toindex - 1;
			if (fromIndex > toIndex) // Remove section in that case
				docPrefsData.docSectionMap.remove(it.key());
			break;
		}
	}
	for (it = docPrefsData.docSectionMap.begin(); it != docPrefsData.docSectionMap.end(); ++it)
	{
		fromIndex = it.value().fromindex;
		toIndex   = it.value().toindex;
		if  (fromIndex > pageIndex)
			--it.value().fromindex;
		if  (toIndex >= pageIndex)
			--it.value().toindex;
	}
	//Now update the Pages' internal storage of their page number
	updateSectionPageNumbersToPages();
}


void ScribusDoc::setFirstSectionFromFirstPageNumber()
{
	DocumentSectionMap::Iterator it = docPrefsData.docSectionMap.begin();
	it.value().sectionstartindex=FirstPnum;
	updateSectionPageNumbersToPages();
}


void ScribusDoc::copyPage(int pageNumberToCopy, int existingPage, int whereToInsert, int copyCount)
{
	UndoTransaction copyTransaction(undoManager->beginTransaction(getUName(), Um::IDocument, Um::CopyPage, "", Um::ICreate));
	SimpleState *ss = new SimpleState(Um::Copy, "", Um::ICreate);
	ss->set("COPY_PAGE", "copy_page");
	ss->set("PAGE_NUM", pageNumberToCopy);
	ss->set("EXISTING_PAGE", existingPage);
	ss->set("WHERE_TO", whereToInsert);
	ss->set("COPY_COUNT", copyCount);
	undoManager->action(this, ss);

	//CB Should we really be disabling auto text frames here?
	bool autoText = usesAutomaticTextFrames();
	setUsesAutomaticTextFrames(false);
	Page* from = DocPages.at(pageNumberToCopy);
	Page* lastDest = NULL;

	uint oldItems = Items->count();
	QList<QString> itemBuffer;
	Selection tempSelection(this, false);
	m_Selection->clear();
	tempSelection.delaySignalsOn();
	if (oldItems>0)
	{
		if (Layers.count()!= 0)
		{
			int currActiveLayer = activeLayer();
			for (ScLayers::iterator it = Layers.begin(); it != Layers.end(); ++it)
			{
				setActiveLayer(it->ID);
				for (uint ite = 0; ite < oldItems; ++ite)
				{
					PageItem *itemToCopy = Items->at(ite);
					if ((itemToCopy->OwnPage == static_cast<int>(from->pageNr())) && (it->ID == itemToCopy->LayerID))
						tempSelection.addItem(itemToCopy, true);
				}
				if (tempSelection.count() != 0)
				{
					ScriXmlDoc *ss = new ScriXmlDoc();
					QString dataS = ss->WriteElem(this, &tempSelection);
					itemBuffer.append(dataS);
					delete ss;
				}
				else
					itemBuffer.append(QString());
				tempSelection.clear();
			}
			setActiveLayer(currActiveLayer);
		}
	}
	tempSelection.delaySignalsOff();


	for (int copyNumber=1; copyNumber<=copyCount; ++copyNumber)
	{
		//For multiple insertions we can insert in the same place
		int destLocation=existingPage;
		if (whereToInsert==0)
			--destLocation;
		else if (whereToInsert==2)
			destLocation=DocPages.count();
		Page* destination = new Page(docPrefsData.displayPrefs.scratch.Left, DocPages.count()*(docPrefsData.docSetupPrefs.pageHeight+docPrefsData.displayPrefs.scratch.Bottom+docPrefsData.displayPrefs.scratch.Top)+docPrefsData.displayPrefs.scratch.Top, docPrefsData.docSetupPrefs.pageWidth, docPrefsData.docSetupPrefs.pageHeight);
		destination->setDocument(this);
		destination->setPageNr(destLocation);
		lastDest = destination;
		DocPages.insert(destLocation, destination);
		setLocationBasedPageLRMargins(destLocation);
		applyMasterPage(from->MPageNam, destLocation);
		destination->setInitialHeight(from->height());
		destination->setInitialWidth(from->width());
		destination->setOrientation(from->orientation());
		destination->m_pageSize = from->m_pageSize;
		//CB: Can possibly partially use the code from applyMasterPage here instead of runnin all of this again..
		//TODO make a fucntion to do this margin stuff and use elsewhere too
		destination->initialMargins.Top = from->initialMargins.Top;
		destination->initialMargins.Bottom = from->initialMargins.Bottom;
		if (pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns == 1)
		{
			destination->initialMargins.Left = from->initialMargins.Left;
			destination->initialMargins.Right = from->initialMargins.Right;
		}
		else
		{
			if (locationOfPage(destination->pageNr()) != locationOfPage(from->pageNr()))
			{
				if (locationOfPage(destination->pageNr()) == MiddlePage)
				{
					destination->initialMargins.Right = from->initialMargins.Left;
					destination->initialMargins.Left = from->initialMargins.Left;
				}
				else
				{
					destination->initialMargins.Right = from->initialMargins.Left;
					destination->initialMargins.Left = from->initialMargins.Right;
				}
			}
			else
			{
				destination->initialMargins.Left = from->initialMargins.Left;
				destination->initialMargins.Right = from->initialMargins.Right;
			}
		}
		// FIXME: stop using m_View
		m_View->reformPages();
		if (itemBuffer.count() > 0)
		{
			int lcount = 0;
			ScLayers::iterator it;
			if (Layers.count()!= 0)
			{
				int currActiveLayer = activeLayer();
				bool savedAlignGrid   = this->useRaster;
				bool savedAlignGuides = this->SnapGuides;
				this->useRaster  = false;
				this->SnapGuides = false;
				for (it = Layers.begin(); it != Layers.end(); ++it)
				{
					if ((lcount < itemBuffer.count()) && !itemBuffer[lcount].isEmpty())
					{
						QString fragment = itemBuffer[lcount];
						ScriXmlDoc *ss = new ScriXmlDoc();
						ss->ReadElemToLayer(fragment, appPrefsData.fontPrefs.AvailFonts, this, destination->xOffset(), destination->yOffset(), false, true, appPrefsData.fontPrefs.GFontSub, it->ID);
						delete ss;
					}
					lcount++;
				}
				this->useRaster  = savedAlignGrid;
				this->SnapGuides = savedAlignGuides;
				setActiveLayer(currActiveLayer);
			}
		}
		from->guides.copy(&destination->guides);
	}
	setUsesAutomaticTextFrames(autoText);
	addPageToSection(existingPage, whereToInsert, copyCount);
	if (lastDest != NULL)
		setCurrentPage(lastDest);
	else
		setCurrentPage(from);
	changed();
	copyTransaction.commit();
}


void ScribusDoc::setLocationBasedPageLRMargins(const uint pageIndex)
{
	int setcol=pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns;
	if (setcol==1)
	{
		Page* pageToAdjust=DocPages.at(pageIndex);
		pageToAdjust->Margins.Left = pageToAdjust->initialMargins.Left;
		pageToAdjust->Margins.Right = pageToAdjust->initialMargins.Right;
		return;
	}

	Page* pageToAdjust=DocPages.at(pageIndex);
	PageLocation pageLoc=locationOfPage(pageIndex);
	if (pageLoc==LeftPage) //Left hand page
	{
		pageToAdjust->Margins.Left = pageToAdjust->initialMargins.Right;
		pageToAdjust->Margins.Right = pageToAdjust->initialMargins.Left;
	}
	else if (pageLoc==RightPage) // Right hand page
	{
		pageToAdjust->Margins.Right = pageToAdjust->initialMargins.Right;
		pageToAdjust->Margins.Left = pageToAdjust->initialMargins.Left;
	}
	else //Middle pages
	{
		pageToAdjust->Margins.Left = pageToAdjust->initialMargins.Left;
		pageToAdjust->Margins.Right = pageToAdjust->initialMargins.Left;
	}
	/* Can also calc the X pos of the frame too, and Y pos, but thats not done yet
	int myRow=(pageIndex+pageSets[currentPageLayout].FirstPage)/setcol;
	double xOffset=ScratchLeft;
	double yOffset=ScratchTop;
	if (myRow==0)
	{
		if (pageIndex==0)
		{
			for (int i=0; i<myCol; ++i)
				xOffset+=pageWidth+pageSets[currentPageLayout].GapHorizontal;
		}
		else
		{
			xOffset=DocPages.at(0)->xOffset();
			for (int i=0; i<pageIndex; ++i)
				xOffset+=DocPages.at(i)->width()+pageSets[currentPageLayout].GapHorizontal;
		}
	}
	if (myRow>0)
	{
		int firstPageOnRow=pageIndex-myCol;
		for (int i=firstPageOnRow; i<pageIndex; ++i)
			xOffset+=DocPages.at(i)->width()+pageSets[currentPageLayout].GapHorizontal;
	}
	pageToAdjust->setXOffset(xOffset);


	if (myRow!=0)
	{
		for (int i=0;i<myRow;++i)
		{
			double maxHeightOfRow=0.0;
			yOffset+=DocPages.at(i)->width()+pageSets[currentPageLayout].GapHorizontal;
		}

		if (pageIndex==0)
		{
			for (int i=0; i<myCol; ++i)
				xOffset+=pageWidth+pageSets[currentPageLayout].GapHorizontal;
		}
		else
		{
			xOffset=DocPages.at(0)->xOffset();
			for (int i=0; i<pageIndex; ++i)
				xOffset+=DocPages.at(i)->width()+pageSets[currentPageLayout].GapHorizontal;
		}
	}
	if (myRow>0)
	{
		int firstPageOnRow=pageIndex-myCol;
	int setcol=pageSets[currentPageLayout].Columns;
		for (int i=firstPageOnRow; i<pageIndex; ++i)
			xOffset+=DocPages.at(i)->width()+pageSets[currentPageLayout].GapHorizontal;
	}
	pageToAdjust->setXOffset(xOffset);
	*/
}


PageLocation ScribusDoc::locationOfPage(int pageIndex) const
{
	int myCol=columnOfPage(pageIndex);
	if (myCol==0) //Left hand page
		return LeftPage;
	else if (myCol>= pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns-1) // Right hand page
		return RightPage;
	else //Middle pages
		return MiddlePage;
}

int ScribusDoc::columnOfPage(int pageIndex) const
{
	int setcol=pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns;
	return ((pageIndex % setcol) + pageSets()[docPrefsData.docSetupPrefs.pagePositioning].FirstPage) % setcol;
}


void ScribusDoc::updateAllItemQColors()
{
	QList<PageItem*> allItems;
	for (int c = 0; c < DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->setLineQColor();
			ite->setFillQColor();
			ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
			for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
				{
					meshPoint mp = ite->meshGradientArray[grow][gcol];
					ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
				}
			}
		}
		allItems.clear();
	}
	for (int c = 0; c < MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->setLineQColor();
			ite->setFillQColor();
			ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
			for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
				{
					meshPoint mp = ite->meshGradientArray[grow][gcol];
					ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
				}
			}
		}
		allItems.clear();
	}
	for (int c = 0; c < FrameItems.count(); ++c)
	{
		PageItem *ite = FrameItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->setLineQColor();
			ite->setFillQColor();
			ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
			for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
				{
					meshPoint mp = ite->meshGradientArray[grow][gcol];
					ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
				}
			}
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *ite = pa.items.at(o);
			if (ite->isGroup())
				allItems = ite->getItemList();
			else
				allItems.append(ite);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				ite = allItems.at(ii);
				ite->setLineQColor();
				ite->setFillQColor();
				ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
				for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
				{
					for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
					{
						meshPoint mp = ite->meshGradientArray[grow][gcol];
						ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
					}
				}
			}
			allItems.clear();
		}
	}
}


//CB Moved from view
void ScribusDoc::RecalcPictures(ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia)
{
	RecalcPictures(&MasterItems, Pr, PrCMYK, dia);
	RecalcPictures(&DocItems, Pr, PrCMYK, dia);
}

void ScribusDoc::RecalcPictures(QList<PageItem*>* items, ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia)
{
	QList<PageItem*> allItems;
	uint docItemCount=items->count();
	if ( docItemCount!= 0)
	{
		bool usingGUI=ScCore->usingGUI();
		int counter = 0;
		if (usingGUI && dia != NULL)
			counter = dia->value();
		PageItem* it;
		for (uint i=0; i < docItemCount; ++i)
		{
			it = items->at(i);
			if (it->isGroup())
				allItems = it->getItemList();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				if ((it->itemType() == PageItem::ImageFrame) && (it->PictureIsAvailable))
				{
					if (it->pixm.imgInfo.colorspace == ColorSpaceCMYK)
					{
						if (!PrCMYK->contains(it->IProfile))
							it->IProfile = docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile;
					}
					else
					{
						if (!Pr->contains(it->IProfile))
							it->IProfile = docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
					}
					loadPict(it->Pfile, it, true);
				}
			}
			allItems.clear();
			if (usingGUI)
			{
				++counter;
				if (dia != NULL)
					dia->setValue(counter);
			}
		}
	}
}



void ScribusDoc::insertColor(QString nam, double c, double m, double y, double k)
{
	if (!PageColors.contains(nam))
	{
		ScColor tmp = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m),
		                      static_cast<int>(255 * y), static_cast<int>(255 * k));
		PageColors.insert(nam, tmp);
	}
}



bool ScribusDoc::sendItemSelectionToBack()
{
	uint docSelectionCount=m_Selection->count();
	if ((Items->count() > 1) && (docSelectionCount != 0))
	{
		QMap<int, uint> ObjOrder;
		PageItem *currItem;
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = m_Selection->itemAt(c);
			if (((currItem->isSingleSel) && (currItem->isGroup())) || ((currItem->isSingleSel) && (currItem->isTableItem)))
				return false;
			ObjOrder.insert(currItem->ItemNr, c);
			int d = Items->indexOf(currItem);
			Items->takeAt(d);
		}
		QList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			Items->prepend(m_Selection->itemAt(Oindex[c]));
		}
		renumberItemsInListOrder();
		return true;
	}
	return false;
}



bool ScribusDoc::bringItemSelectionToFront()
{
	uint docSelectionCount=m_Selection->count();
	if ((Items->count() > 1) && (docSelectionCount != 0))
	{
		QMap<int, uint> ObjOrder;
		PageItem *currItem;
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = m_Selection->itemAt(c);
			if (((currItem->isSingleSel) && (currItem->isGroup())) || ((currItem->isSingleSel) && (currItem->isTableItem)))
				return false;
			ObjOrder.insert(currItem->ItemNr, c);
			int d = Items->indexOf(currItem);
			Items->takeAt(d);
		}
		QList<uint> Oindex = ObjOrder.values();
		for (int c = 0; c <static_cast<int>(Oindex.count()); ++c)
		{
			Items->append(m_Selection->itemAt(Oindex[c]));
		}
		renumberItemsInListOrder();
		return true;
	}
	return false;
}



void ScribusDoc::itemSelection_SetLineWidth(double w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::LineWidth, "", Um::ILineStyle));
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			QRectF oldRect = currItem->getVisualBoundingRect();
			//cb moved to setlinewidth
			//currItem->Oldm_lineWidth = currItem->lineWidth();
			currItem->setLineWidth(w);
			if (currItem->asPolyLine() || currItem->asSpiral())
				currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
			if (currItem->asLine())
			{
				int ph = static_cast<int>(qMax(1.0, w / 2.0));
				currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
				                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
				                  -ph,static_cast<int>(currItem->height()+ph));
			}
			QRectF newRect = currItem->getVisualBoundingRect();
			//currItem->update();
			currItem->invalidateLayout();
			regionsChanged()->update(newRect.united(oldRect));
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
	}
	changed();
}



void ScribusDoc::itemSelection_SetLineArt(Qt::PenStyle w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::LineStyle, "", Um::ILineStyle));
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->setLineStyle(w);
			m_Selection->itemAt(a)->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
	}
	changed();
}



void ScribusDoc::itemSelection_SetLineJoin(Qt::PenJoinStyle w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::LineJoin, "", Um::ILineStyle));
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->setLineJoin(w);
			m_Selection->itemAt(a)->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
	}
	changed();
}



void ScribusDoc::itemSelection_SetLineEnd(Qt::PenCapStyle w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::LineEnd, "", Um::ILineStyle));
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->setLineEnd(w);
			m_Selection->itemAt(a)->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
	}
	changed();
}



void ScribusDoc::itemSelection_SetLineSpacing(double w, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setLineSpacing(w);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetFont(QString fon, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFont((*AllFonts)[fon]);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}



void ScribusDoc::itemSelection_SetNamedCharStyle(const QString& name, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setParent(name.isEmpty()? Style::INHERIT_PARENT : name);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}



void ScribusDoc::itemSelection_SetNamedParagraphStyle(const QString& name, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setParent(name.isEmpty()? Style::INHERIT_PARENT : name);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection, true);
}


void ScribusDoc::itemSelection_SetNamedLineStyle(const QString &name, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	uint docSelectionCount   = itemSelection->count();
	if (docSelectionCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (docSelectionCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::LineStyle, name, Um::ILineStyle));
		for (uint aa = 0; aa < docSelectionCount; ++aa)
		{
			PageItem *currItem = itemSelection->itemAt(aa);
			currItem->setCustomLineStyle(name);
			currItem->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}


void ScribusDoc::itemSelection_SetItemPen(QString farbe)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (farbe == CommonStrings::tr_NoneColor)
			farbe = CommonStrings::None;
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::SetLineColor, farbe, Um::IFill));
		PageItem *i;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			i = m_Selection->itemAt(a);
			if ((i->asLine()) && (farbe == CommonStrings::None))
				continue;

			i->setLineColor(farbe);
			i->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
	}
	changed();
}

void ScribusDoc::itemSelection_SetFillColor(QString farbe, Selection* customSelection)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	CharStyle newStyle;
	newStyle.setFillColor(farbe);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetFillShade(int sha, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFillShade(sha);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetStrokeColor(QString farbe, Selection* customSelection)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	CharStyle newStyle;
	newStyle.setStrokeColor(farbe);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetStrokeShade(int sha, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setStrokeShade(sha);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetScaleV(int scale, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setScaleV(scale);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetScaleH(int scale, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setScaleH(scale);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetShadowOffsets(int shx, int shy, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setShadowXOffset(shx);
	newStyle.setShadowYOffset(shy);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetUnderline(int pos, int wid, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setUnderlineOffset(pos);
	newStyle.setUnderlineWidth(wid);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetStrikethru(int pos, int wid, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setStrikethruOffset(pos);
	newStyle.setStrikethruWidth(wid);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetBaselineOffset(int sha, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setBaselineOffset(sha);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetOutlineWidth(int wid, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setOutlineWidth(wid);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetItemBrush(QString farbe)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::SetFill, farbe, Um::IFill));
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setFillColor(farbe);
			currItem->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemBrushShade(int sha)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::SetShade, QString("%1").arg(sha),
																				  Um::IShade));
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setFillShade(sha);
			currItem->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemPenShade(int sha)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup,
																				  Um::IGroup, Um::SetLineShade, QString("%1").arg(sha), Um::IShade));
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setLineShade(sha);
			currItem->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemGradMask(int typ)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->GrMask = typ;
			if ((typ > 0) && (typ < 7))
				currItem->updateGradientVectors();
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemGradStroke(int typ)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->GrTypeStroke = typ;
			if (currItem->GrTypeStroke == 0)
			{
				if (currItem->lineColor() != CommonStrings::None)
				{
					if (!PageColors.contains(currItem->lineColor()))
					{
						switch(currItem->itemType())
						{
							case PageItem::TextFrame:
							case PageItem::PathText:
								currItem->setLineColor(docPrefsData.itemToolPrefs.textLineColor);
								break;
							case PageItem::Line:
							case PageItem::PolyLine:
							case PageItem::Polygon:
							case PageItem::ImageFrame:
							case PageItem::LatexFrame:
							case PageItem::Spiral:
								currItem->setLineColor(docPrefsData.itemToolPrefs.shapeLineColor);
								break;
							default:
								break;
						}
					}
				}
			}
			if ((typ > 0) && (typ < 8))
				currItem->updateGradientVectors();
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemGradFill(int typ)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->GrType = typ;
			switch (currItem->GrType)
			{
				case 0:
					if (currItem->fillColor() != CommonStrings::None)
					{
						if (!PageColors.contains(currItem->fillColor()))
						{
							switch(currItem->itemType())
							{
								case PageItem::ImageFrame:
								case PageItem::LatexFrame:
									currItem->setFillColor(docPrefsData.itemToolPrefs.imageFillColor);
								case PageItem::TextFrame:
								case PageItem::PathText:
									currItem->setFillColor(docPrefsData.itemToolPrefs.textFillColor);
									break;
								case PageItem::Line:
								case PageItem::PolyLine:
								case PageItem::Polygon:
								case PageItem::Spiral:
									currItem->setFillColor(docPrefsData.itemToolPrefs.shapeFillColor);
									break;
								default:
									break;
							}
						}
					}
					break;
				case 1:
					currItem->GrStartX = 0;
					currItem->GrStartY = currItem->height() / 2.0;
					currItem->GrEndX = currItem->width();
					currItem->GrEndY = currItem->height() / 2.0;
					break;
				case 2:
					currItem->GrStartX = currItem->width() / 2.0;
					currItem->GrStartY = 0;
					currItem->GrEndX = currItem->width() / 2.0;
					currItem->GrEndY = currItem->height();
					break;
				case 3:
					currItem->GrStartX = 0;
					currItem->GrStartY = 0;
					currItem->GrEndX = currItem->width();
					currItem->GrEndY = currItem->height();
					break;
				case 4:
					currItem->GrStartX = 0;
					currItem->GrStartY = currItem->height();
					currItem->GrEndX = currItem->width();
					currItem->GrEndY = 0;
					break;
				case 5:
					currItem->GrStartX = currItem->width() / 2.0;
					currItem->GrStartY = currItem->height() / 2.0;
					if (currItem->width() >= currItem->height())
					{
						currItem->GrEndX = currItem->width();
						currItem->GrEndY = currItem->height() / 2.0;
					}
					else
					{
						currItem->GrEndX = currItem->width() / 2.0;
						currItem->GrEndY = currItem->height();
					}
					break;
				default:
					break;
			}
			if ((typ > 0) && (typ < 8))
				currItem->updateGradientVectors();
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemPatternFill(QString pattern)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setPattern(pattern);
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemPatternProps(double imageScaleX, double imageScaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setPatternTransform(imageScaleX, imageScaleY, offsetX, offsetY, rotation, skewX, skewY);
			currItem->setPatternFlip(mirrorX, mirrorY);
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemStrokePattern(QString pattern)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setStrokePattern(pattern);
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemStrokePatternProps(double imageScaleX, double imageScaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, double space, bool mirrorX, bool mirrorY)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setStrokePatternTransform(imageScaleX, imageScaleY, offsetX, offsetY, rotation, skewX, skewY, space);
			currItem->setStrokePatternFlip(mirrorX, mirrorY);
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemStrokePatternType(bool type)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setStrokePatternToPath(type);
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemPatternMask(QString pattern)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setPatternMask(pattern);
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemPatternMaskProps(double imageScaleX, double imageScaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setMaskTransform(imageScaleX, imageScaleY, offsetX, offsetY, rotation, skewX, skewY);
			currItem->setMaskFlip(mirrorX, mirrorY);
			currItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetEffects(int s, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFeatures(static_cast<StyleFlag>(s).featureList());
	itemSelection_ApplyCharStyle(newStyle, customSelection);
	return;
	
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0 && s != ScStyle_None)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontEffect, "", Um::IFont));
		for (uint aa = 0; aa < selectedItemCount; ++aa)
		{
			PageItem *currItem = m_Selection->itemAt(aa);
//			if (appMode != modeEdit)
//				currItem->setFontEffects(s);
			if (currItem->itemText.length() != 0)
			{
#ifndef NLS_PROTO
				if (appMode == modeEdit)
				{
					for (int a = 0; a < currItem->itemText.length(); ++a)
					{
						if (currItem->itemText.selected(a))
						{
							StyleFlag fl = currItem->itemText.item(a)->effects();
							fl &= static_cast<StyleFlag>(~1919); // 0x11101111111
							fl |= static_cast<StyleFlag>(s & 1919);
							currItem->itemText.item(a)->setFeatures(fl.featureList());
						}
					}
				}
				else
				{
					for (int a = 0; a < currItem->itemText.length(); ++a)
					{
						StyleFlag fl = currItem->itemText.item(a)->effects();
						fl &= static_cast<StyleFlag>(~1919); // 1024+512+256+64+32+16+8+4+2+1
						fl |= static_cast<StyleFlag>(s & 1919);
						currItem->itemText.item(a)->setFeatures(fl.featureList());
					}
				}
#endif
				currItem->update();
			}
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetOpticalMargins(int i, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setOpticalMargins(i);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}


void ScribusDoc::itemSelection_resetOpticalMargins(Selection* customSelection)
{
	//TODO
//	ParagraphStyle newStyle;
//	newStyle.setOpticalMargins(i);
//	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetTracking(int kern, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setTracking(kern);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetLineSpacingMode(int m, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(m));
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetFontSize(int size, Selection* customSelection)
{
	if (true || appMode == modeEdit) 
	{
		CharStyle newStyle;
		newStyle.setFontSize(size);
		itemSelection_ApplyCharStyle(newStyle, customSelection);
	}
	else
	{
		ParagraphStyle storyStyle;
		storyStyle.charStyle().setFontSize(size);
		if (storyStyle.lineSpacingMode() == 0)
		{
			storyStyle.setLineSpacing(((size / 10.0) * static_cast<double>(docPrefsData.typoPrefs.autoLineSpacing) / 100) + (size / 10.0));
		}
		else if (storyStyle.lineSpacingMode() == 1)
		{
			storyStyle.setLineSpacing(storyStyle.charStyle().font().height(size));
		}
		else
		{
			storyStyle.setLineSpacing(docPrefsData.guidesPrefs.valueBaselineGrid-1);
		}
		itemSelection_ApplyParagraphStyle(storyStyle, customSelection);
	}
}

void ScribusDoc::itemSelection_SetParagraphStyle(const ParagraphStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction* activeTransaction = NULL;
	if (selectedItemCount > 1)
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle, newStyle.displayName(), Um::IFont));
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
//		int currItemTextCount = currItem->lastInFrame() + 1 - currItem->firstInFrame();
//		if (currItemTextCount > 0 && ( appMode == modeEdit || !FRAMESELECTION_EDITS_DEFAULTSTYLE))
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && (appMode == modeEdit))
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int stop = currItem->itemText.endOfItem(currItem->lastInFrame());
			if (appMode == modeEdit)
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = qMax(0, qMin(currItem->itemText.length(), currItem->CPos));
			}
			for (int pos=start; pos < stop; ++pos)
			{
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP)
				{
					currItem->itemText.setStyle(pos, newStyle);
				}
			}
			currItem->itemText.setStyle(stop, newStyle);
			currItem->invalid = true;
		}
		else
		{
			currItem->itemText.setDefaultStyle(newStyle);
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_EraseParagraphStyle(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction* activeTransaction = NULL;
	if (selectedItemCount > 1)
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle, tr( "remove direct paragraph formatting" ), Um::IFont));
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
//		int currItemTextCount = currItem->lastInFrame() + 1 - currItem->firstInFrame();
//		if (currItemTextCount > 0 && ( appMode == modeEdit || !FRAMESELECTION_EDITS_DEFAULTSTYLE))
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && (appMode == modeEdit))
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int stop = currItem->itemText.endOfItem(currItem->lastInFrame());
			if (appMode == modeEdit)
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = qMax(0, qMin(currItem->itemText.length(), currItem->CPos));
			}
			for (int pos=start; pos < stop; ++pos)
			{
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP)
				{
					ParagraphStyle newStyle;
					newStyle.setParent(currItem->itemText.paragraphStyle(pos).parent());
					currItem->itemText.setStyle(pos, newStyle);
				}
			}
			ParagraphStyle newStyle2;
			newStyle2.setParent(currItem->itemText.paragraphStyle(stop).parent());
			currItem->itemText.setStyle(stop, newStyle2);
		}
		else
		{
			ParagraphStyle newStyle;
			newStyle.setParent(currItem->itemText.defaultStyle().parent());
			currItem->itemText.setDefaultStyle(newStyle);
		}
		currItem->invalid = true;
		if (currItem->asPathText())
			currItem->updatePolyClip();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_ApplyParagraphStyle(const ParagraphStyle & newStyle, Selection* customSelection, bool rmDirectFormatting)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction* activeTransaction = NULL;
	if (selectedItemCount > 1)
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle, newStyle.displayName(), Um::IFont));
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount == 0) || (appMode != modeEdit))
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.applyStyle(newStyle);
			currItem->itemText.setDefaultStyle(dstyle);
		}
		if (currItemTextCount > 0)
		{
			int start = currItem->asPathText() ? currItem->itemText.startOfItem(currItem->firstInFrame()) : 0;
			int stop  = currItem->asPathText() ? currItem->itemText.endOfItem(currItem->lastInFrame()) :  currItemTextCount;
			if (appMode == modeEdit)
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = qMax(0, qMin(currItem->itemText.length(), currItem->CPos));
			}
			for (int pos=start; pos < stop; ++pos)
			{
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP)
				{
					currItem->itemText.applyStyle(pos, newStyle, rmDirectFormatting);
				}
			}
			currItem->itemText.applyStyle(stop, newStyle, rmDirectFormatting);
			currItem->invalid = true;
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		currItem->invalidateLayout();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_ApplyCharStyle(const CharStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction* activeTransaction = NULL;
	if (selectedItemCount > 1)
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle, newStyle.asString(), Um::IFont));
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
//		int currItemTextCount = currItem->lastInFrame() + 1 - currItem->firstInFrame();
//		if (currItemTextCount > 0 && ( appMode == modeEdit || !FRAMESELECTION_EDITS_DEFAULTSTYLE))
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && (appMode == modeEdit))
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int length = currItem->itemText.endOfItem(currItem->lastInFrame()) - start;
			if (appMode == modeEdit)
			{
				if (currItem->itemText.lengthOfSelection() > 0)
				{
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else
				{
					start = qMax(currItem->firstInFrame(), currItem->CPos);
					length = (start + 1) < currItem->itemText.length()? 1 : 0;
				}
			}
			currItem->itemText.applyCharStyle(start, qMax(0, length), newStyle);
			currItem->invalid = true;
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.charStyle().applyCharStyle(newStyle);
			currItem->itemText.setDefaultStyle(dstyle);
//			if (currItem->asPathText())
			currItem->itemText.applyCharStyle(0, currItem->itemText.length(), newStyle);
			currItem->invalid = true;
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		currItem->invalidateLayout();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_SetCharStyle(const CharStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction* activeTransaction = NULL;
	if (selectedItemCount > 1)
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle, newStyle.asString(), Um::IFont));
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
//		int currItemTextCount = currItem->lastInFrame() + 1 - currItem->firstInFrame();
//		if (currItemTextCount > 0 && ( appMode == modeEdit || !FRAMESELECTION_EDITS_DEFAULTSTYLE))
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && (appMode == modeEdit))
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int length = currItem->itemText.endOfItem(currItem->lastInFrame()) - start;
			if (appMode == modeEdit)
			{
				if (currItem->itemText.lengthOfSelection() > 0)
				{
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else
				{
					start = qMax(currItem->firstInFrame(), currItem->CPos);
					length = (start + 1) < currItem->itemText.length()? 1 : 0;
				}
			}
			currItem->itemText.setCharStyle(start, length, newStyle);
			currItem->invalid = true;
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.charStyle().setStyle(newStyle);
			currItem->itemText.setDefaultStyle(dstyle);
//			if (currItem->asPathText())
			currItem->itemText.setCharStyle(0, currItem->itemText.length(), newStyle);
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		currItem->invalidateLayout();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_EraseCharStyle(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction* activeTransaction = NULL;
	if (selectedItemCount > 1)
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle, tr( "remove direct char formatting" ), Um::IFont));
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
//		int currItemTextCount = currItem->lastInFrame() + 1 - currItem->firstInFrame();
//		if (currItemTextCount > 0 && ( appMode == modeEdit || !FRAMESELECTION_EDITS_DEFAULTSTYLE))
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && (appMode == modeEdit))
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int length = currItem->itemText.endOfItem(currItem->lastInFrame()) - start;
			if (appMode == modeEdit)
			{
				if (currItem->itemText.lengthOfSelection() > 0)
				{
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else
				{
					start = qMax(currItem->firstInFrame(), currItem->CPos);
					length = (start + 1) < currItem->itemText.length()? 1 : 0;
				}
			}
			QString lastParent;
			int stop = start+length;
			int lastPos = start;
			for (int i=start; i < stop; ++i)
			{
				const QString& curParent(currItem->itemText.charStyle(i).parent());
				if (curParent != lastParent)
				{
					if ( i-lastPos > 0)
					{
						CharStyle newStyle;
						newStyle.setParent(lastParent);
						currItem->itemText.setCharStyle(lastPos, i-lastPos, newStyle);
						lastPos = i;
					}
					lastParent = curParent;
				}
			}
			if (lastPos < stop)
			{
				CharStyle newStyle2;
				newStyle2.setParent(lastParent);
				currItem->itemText.setCharStyle(lastPos, stop-lastPos, newStyle2);
			}
		}
		else
		{
			ParagraphStyle defStyle = currItem->itemText.defaultStyle();
			CharStyle newStyle;
			newStyle.setParent(defStyle.charStyle().parent());
			defStyle.charStyle() = newStyle;
			currItem->itemText.setDefaultStyle(defStyle);
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		currItem->invalidateLayout();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	changed();
	regionsChanged()->update(QRectF());
}



/*
template<typename Arg, void (PageItem::*Fun)(Arg)>
void ScribusDoc::itemSelection_forall(Arg newVal)
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle,
										  newStyle.asString(), Um::IFont);
		for (uint aa = 0; aa < docSelectionCount; ++aa)
		{
			PageItem *currItem = m_Selection->itemAt(aa);
			currItem->Fun(newVal);
		}
	}
	if (docSelectionCount > 1)
		undoManager->commit();
	emit updateContents();
}
*/




/* CB Dont delete, backups
void ScribusDoc::FlipImageH()
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
										  Um::FlipH, 0, Um::IFlipH);
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			m_Selection->itemAt(a)->flipImageH();
			emit refreshItem(m_Selection->itemAt(a));
		}
		changed();
		if (docSelectionCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::FlipImageV()
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
										  Um::FlipV, 0, Um::IFlipV);
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			m_Selection->itemAt(a)->flipImageV();
			emit refreshItem(m_Selection->itemAt(a));
		}
		changed();
		if (docSelectionCount > 1)
			undoManager->commit();
	}
}

*/


//AV -> NodeEditContext
//CB removed looping, called by itemSelection_FlipH
void ScribusDoc::MirrorPolyH(PageItem* currItem)
{
	currItem->ClipEdited = true;
	QTransform ma;
	if (nodeEdit.isContourLine)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
			ss->set("MIRROR_PATH_H", "mirror_path_h");
			ss->set("IS_CONTOUR", true);
			undoManager->action(currItem, ss, Um::IBorder);
		}
		FPoint tp2(getMinClipF(&currItem->ContourLine));
		FPoint tp(getMaxClipF(&currItem->ContourLine));
		ma.translate(qRound(tp.x()), 0);
		ma.scale(-1, 1);
		currItem->ContourLine.map(ma);
		regionsChanged()->update(QRectF());
		changed();
		return;
	}
	ma.scale(-1, 1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(currItem->width(), 0);
	if (currItem->asPathText())
		currItem->updatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	setRedrawBounding(currItem);
	currItem->update();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
		ss->set("MIRROR_PATH_H", "mirror_path_h");
		ss->set("IS_CONTOUR", false);
		undoManager->action(currItem, ss, Um::IBorder);
	}
	changed();
}


//AV -> NodeEditContext
//CB removed looping, called by itemSelection_FlipV
void ScribusDoc::MirrorPolyV(PageItem* currItem)
{
	currItem->ClipEdited = true;
	QTransform ma;
	if (nodeEdit.isContourLine)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
			ss->set("MIRROR_PATH_V", "mirror_path_v");
			ss->set("IS_CONTOUR", true);
			undoManager->action(currItem, ss, Um::IBorder);
		}
		FPoint tp2(getMinClipF(&currItem->ContourLine));
		FPoint tp(getMaxClipF(&currItem->ContourLine));
		ma.translate(0, qRound(tp.y()));
		ma.scale(1, -1);
		currItem->ContourLine.map(ma);
		regionsChanged()->update(QRectF());
		changed();
		return;
	}
	ma.scale(1, -1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(0, currItem->height());
	if (currItem->asPathText())
		currItem->updatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	setRedrawBounding(currItem);
	currItem->update();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
		ss->set("MIRROR_PATH_V", "mirror_path_v");
		ss->set("IS_CONTOUR", false);
		undoManager->action(currItem, ss, Um::IBorder);
	}
	changed();
}


void ScribusDoc::setRedrawBounding(PageItem *currItem)
{
	currItem->setRedrawBounding();
	FPoint maxSize(currItem->BoundingX+currItem->BoundingW+docPrefsData.displayPrefs.scratch.Right, currItem->BoundingY+currItem->BoundingH+docPrefsData.displayPrefs.scratch.Bottom);
	FPoint minSize(currItem->BoundingX-docPrefsData.displayPrefs.scratch.Left, currItem->BoundingY-docPrefsData.displayPrefs.scratch.Top);
	adjustCanvas(minSize, maxSize);
}


void ScribusDoc::adjustCanvas(FPoint minPos, FPoint maxPos, bool absolute)
{
	double newMaxX, newMaxY, newMinX, newMinY;
	if (absolute)
	{
		newMaxX = maxPos.x();
		newMaxY = maxPos.y();
		newMinX = minPos.x();
		newMinY = minPos.y();
	}
	else
	{
		newMaxX = qMax(maxCanvasCoordinate.x(), maxPos.x());
		newMaxY = qMax(maxCanvasCoordinate.y(), maxPos.y());
		newMinX = qMin(minCanvasCoordinate.x(), minPos.x());
		newMinY = qMin(minCanvasCoordinate.y(), minPos.y());
	}
	/*<< CB: comment out as avox is falling asleep and this is b0rken
	// try to limit expansion of canvas to reasonable area:
	
	int columns = pageSets[currentPageLayout].Columns;
	int rows = 1 + DocPages.count() / columns;
	newMinX = qMax(newMinX, -pageWidth);
	newMinY = qMax(newMinY, -pageHeight);
	newMaxX = qMin(newMaxX, (bleeds.Left + pageWidth + bleeds.Right) * columns + pageWidth + scratch.Left + scratch.Right + GapHorizontal*(columns-1) );
	newMaxY = qMin(newMaxY, (bleeds.Top + pageHeight + bleeds.Bottom) * columns + pageHeight + scratch.Top + scratch.Bottom + GapVertical*(rows-1));
	>>*/
	if ((newMaxX != maxCanvasCoordinate.x()) || (newMaxY != maxCanvasCoordinate.y())
	|| (newMinX != minCanvasCoordinate.x()) || (newMinY != minCanvasCoordinate.y()))
	{
//		//Save the old values for the emit, but update now to ensure we are all ready
//		double oldMinX=minCanvasCoordinate.x();
//		double oldMinY=minCanvasCoordinate.y();
		maxCanvasCoordinate = FPoint(newMaxX, newMaxY);
		minCanvasCoordinate = FPoint(newMinX, newMinY);
		regionsChanged()->update(QRectF());

		//CB TODO Make a list of views we belong to and make this the doc's active view via an internal*
//		if (ScCore->usingGUI()) 
//		{
//			// Why using a signal here ? much slower than a direct call
//			emit canvasAdjusted(newMaxX - newMinX, newMaxY - newMinY, oldMinX - newMinX, oldMinY - newMinY);
//FIXME: stop using m_View
//			m_View->adjustCanvas(newMaxX - newMinX, newMaxY - newMinY, oldMinX - newMinX, oldMinY - newMinY);
//		}
	}
}


void ScribusDoc::connectDocSignals()
{
	if (ScCore->usingGUI())
	{
		if (m_hasGUI)
		{
			connect(this, SIGNAL(docChanged()), m_ScMW, SLOT(slotDocCh()));
			connect(this, SIGNAL(firstSelectedItemType(int)), m_ScMW, SLOT(HaveNewSel(int)));
			connect(autoSaveTimer, SIGNAL(timeout()), WinHan, SLOT(slotAutoSave()));
		}
	}
}


//CB Same as RecalcPicturesRes apart from the name checking, which should be able to be removed
void ScribusDoc::updatePict(QString name)
{
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
			}
		}
		allItems.clear();
	}
	for (int a = 0; a <FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
			}
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
				{
					bool fho = currItem->imageFlippedH();
					bool fvo = currItem->imageFlippedV();
					loadPict(currItem->Pfile, currItem, true);
					currItem->setImageFlippedH(fho);
					currItem->setImageFlippedV(fvo);
				}
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
	}
	regionsChanged()->update(QRectF());
	changed();
}

void ScribusDoc::updatePictDir(QString name)
{
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->asImageFrame())
			{
				if ((!currItem->PictureIsAvailable) && (!currItem->Pfile.isEmpty()))
				{
					QFileInfo fi(currItem->Pfile);
					if (fi.absolutePath() == name)
					{
						if (fi.exists())
						{
							bool fho = currItem->imageFlippedH();
							bool fvo = currItem->imageFlippedV();
							loadPict(currItem->Pfile, currItem, true);
							currItem->setImageFlippedH(fho);
							currItem->setImageFlippedV(fvo);
							ScCore->fileWatcher->addFile(currItem->Pfile);
						}
					}
				}
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->asImageFrame())
			{
				if ((!currItem->PictureIsAvailable) && (!currItem->Pfile.isEmpty()))
				{
					QFileInfo fi(currItem->Pfile);
					if (fi.absolutePath() == name)
					{
						if (fi.exists())
						{
							bool fho = currItem->imageFlippedH();
							bool fvo = currItem->imageFlippedV();
							loadPict(currItem->Pfile, currItem, true);
							currItem->setImageFlippedH(fho);
							currItem->setImageFlippedV(fvo);
							ScCore->fileWatcher->addFile(currItem->Pfile);
						}
					}
				}
			}
		}
		allItems.clear();
	}
	for (int a = 0; a <FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->asImageFrame())
			{
				if ((!currItem->PictureIsAvailable) && (!currItem->Pfile.isEmpty()))
				{
					QFileInfo fi(currItem->Pfile);
					if (fi.absolutePath() == name)
					{
						if (fi.exists())
						{
							bool fho = currItem->imageFlippedH();
							bool fvo = currItem->imageFlippedV();
							loadPict(currItem->Pfile, currItem, true);
							currItem->setImageFlippedH(fho);
							currItem->setImageFlippedV(fvo);
							ScCore->fileWatcher->addFile(currItem->Pfile);
						}
					}
				}
			}
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (currItem->asImageFrame())
				{
					if ((!currItem->PictureIsAvailable) && (!currItem->Pfile.isEmpty()))
					{
						QFileInfo fi(currItem->Pfile);
						if (fi.absolutePath() == name)
						{
							if (fi.exists())
							{
								bool fho = currItem->imageFlippedH();
								bool fvo = currItem->imageFlippedV();
								loadPict(currItem->Pfile, currItem, true);
								currItem->setImageFlippedH(fho);
								currItem->setImageFlippedV(fvo);
								ScCore->fileWatcher->addFile(currItem->Pfile);
							}
						}
					}
				}
			}
			allItems.clear();
			PageItem *ite = pa.items.at(0);
			docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
		}
	}
	regionsChanged()->update(QRectF());
	changed();
}

//CB Same as updatePict apart from the name checking, this should be able to be removed
void ScribusDoc::recalcPicturesRes(bool applyNewRes)
{
	int cc = 0;
	int ca = 0;
	ScGuardedPtr<ScribusDoc> docPtr = guardedPtr();
	m_ScMW->mainWindowProgressBar->reset();
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
				cc++;
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
				cc++;
		}
		allItems.clear();
	}
	for (int a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
				cc++;
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (currItem->PictureIsAvailable)
					cc++;
			}
			allItems.clear();
		}
	}
	m_ScMW->mainWindowProgressBar->setMaximum((cc > 0) ? cc : 1);
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				if (applyNewRes)
					currItem->pixm.imgInfo.lowResType = docPrefsData.itemToolPrefs.imageLowResType;
				if (currItem->asLatexFrame())
					currItem->asLatexFrame()->rerunApplication(false);
				else
					loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->AdjustPictScale();
				ca++;
				m_ScMW->mainWindowProgressBar->setValue(ca);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				if (!docPtr) return;
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				if (applyNewRes)
					currItem->pixm.imgInfo.lowResType = docPrefsData.itemToolPrefs.imageLowResType;
				if (currItem->asLatexFrame())
					currItem->asLatexFrame()->rerunApplication(false);
				else
					loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->AdjustPictScale();
				ca++;
				m_ScMW->mainWindowProgressBar->setValue(ca);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				if (!docPtr) return;
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->PictureIsAvailable)
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				if (applyNewRes)
					currItem->pixm.imgInfo.lowResType = docPrefsData.itemToolPrefs.imageLowResType;
				if (currItem->asLatexFrame())
					currItem->asLatexFrame()->rerunApplication(false);
				else
					loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->AdjustPictScale();
				ca++;
				m_ScMW->mainWindowProgressBar->setValue(ca);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				if (!docPtr) return;
			}
		}
		allItems.clear();
	}
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (currItem->PictureIsAvailable)
				{
					bool fho = currItem->imageFlippedH();
					bool fvo = currItem->imageFlippedV();
					if (applyNewRes)
						currItem->pixm.imgInfo.lowResType = docPrefsData.itemToolPrefs.imageLowResType;
					if (currItem->asLatexFrame())
						currItem->asLatexFrame()->rerunApplication(false);
					else
						loadPict(currItem->Pfile, currItem, true);
					currItem->setImageFlippedH(fho);
					currItem->setImageFlippedV(fvo);
					currItem->AdjustPictScale();
					ca++;
					m_ScMW->mainWindowProgressBar->setValue(ca);
					qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
					if (!docPtr) return;
				}
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
	}
	regionsChanged()->update(QRectF());
	changed();
	m_ScMW->mainWindowProgressBar->reset();
}


void ScribusDoc::removePict(QString name)
{
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
			{
				currItem->PictureIsAvailable = false;
				currItem->pixm = ScImage();
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
			{
				currItem->PictureIsAvailable = false;
				currItem->pixm = ScImage();
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
			{
				currItem->PictureIsAvailable = false;
				currItem->pixm = ScImage();
			}
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getItemList();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if ((currItem->PictureIsAvailable) && (currItem->Pfile == name))
				{
					currItem->PictureIsAvailable = false;
					currItem->pixm = ScImage();
				}
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
	}
	regionsChanged()->update(QRectF());
	changed();
}


void ScribusDoc::updatePic()
{
//TODO? Getting the pointer with m_Selection->itemAt(i) over and over again in the loop 
// seems to be a waste of ressources
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount > 0)
	{
		bool toUpdate=false;
		for (uint i = 0; i < docSelectionCount; ++i)
		{
			if (m_Selection->itemAt(i)!=NULL)
			{
				if (m_Selection->itemAt(i)->asLatexFrame()) {
					PageItem_LatexFrame *latexframe =
						m_Selection->itemAt(i)->asLatexFrame();
					latexframe->rerunApplication();
					toUpdate = true;
				}
#ifdef HAVE_OSG
				else if ((m_Selection->itemAt(i)->asImageFrame()) || (m_Selection->itemAt(i)->asOSGFrame()))
#else
				else if (m_Selection->itemAt(i)->asImageFrame())
#endif
				{
					PageItem *currItem = m_Selection->itemAt(i);
					if (currItem->PictureIsAvailable)
					{
						int fho = currItem->imageFlippedH();
						int fvo = currItem->imageFlippedV();
						loadPict(currItem->Pfile, currItem, true);
						currItem->setImageFlippedH(fho);
						currItem->setImageFlippedV(fvo);
						toUpdate=true;
					}
				}
			}
		}
		if (toUpdate)
			regionsChanged()->update(QRectF());
	}
}


//TODO replace with the ScribusDoc::deleteTaggedItems
void ScribusDoc::removeLayer(int l, bool dl)
{
	//FIXME: stop using m_View
	m_View->Deselect();
	Selection tmpSelection(this, false);
	int newLayerID = 0;
	// Find the new layer identifier
	const ScLayer* lcurr = Layers.layerByID(l);
	if (lcurr)
	{
		const ScLayer* lbelow = Layers.layerBelow(lcurr->Level);
		if (lcurr == lbelow)
		{
			const ScLayer* labove = Layers.layerAbove(lcurr->Level);
			if (labove)
				newLayerID = labove->ID;
		}
		else if (lbelow)
			newLayerID = lbelow->ID;
	}
	for (int b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->LayerID == l)
		{
			if (dl)
			{
				tmpSelection.addItem(MasterItems.at(b));
				MasterItems.at(b)->setLocked(false);
			}
			else
				MasterItems.at(b)->setLayer(newLayerID);
		}
	}
	if (tmpSelection.count() != 0)
		itemSelection_DeleteItem(&tmpSelection);
	tmpSelection.clear();
	QList<PageItem*> allItems;
	for (int b = 0; b < DocItems.count(); ++b)
	{
		PageItem* currItem = DocItems.at(b);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->LayerID == l)
			{
				if (dl)
				{
					tmpSelection.addItem(currItem);
					currItem->setLocked(false);
				}
				else
					currItem->setLayer(newLayerID);
			}
		}
		allItems.clear();
	}
	if (tmpSelection.count() != 0)
		itemSelection_DeleteItem(&tmpSelection);
	tmpSelection.clear();
	//FIXME signal these
	m_ScMW->rebuildLayersList();
	//FIXME: stop using m_View
	m_View->updateLayerMenu();
	setActiveLayer(newLayerID);
}

void ScribusDoc::selectionChanged()
{
	if (m_ScMW->isObjectSpecificUndo())
	{
		uint docSelectionCount = m_Selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(m_Selection->itemAt(0)->getUId());
		else if (docSelectionCount == 0)
			undoManager->showObject(currentPage()->getUId());
		else
			undoManager->showObject(Um::NO_UNDO_STACK);
	}
}


void ScribusDoc::itemSelection_ToggleLock( )
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (docSelectionCount > 1)
		{
			if (m_Selection->itemAt(0)->locked())
				activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::UnLock, 0, Um::IUnLock));
			else
				activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Lock, 0, Um::ILock));
		}
		for ( uint a = 0; a < docSelectionCount; ++a)
		{
			m_Selection->itemAt(a)->toggleLock();
			m_Selection->itemAt(a)->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
 		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
}


void ScribusDoc::itemSelection_ToggleSizeLock( )
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1)
		{
			if (m_Selection->itemAt(0)->sizeLocked())
				activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SizeUnLock, 0, Um::IUnLock));
			else
				activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SizeLock, 0, Um::ILock));
		}
		for ( uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->toggleSizeLock();
			m_Selection->itemAt(a)->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
 		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
}


void ScribusDoc::itemSelection_ToggleImageShown()
{
	if (m_Selection->count() != 0)
	{
		m_updateManager.setUpdatesDisabled();
		for (int a = 0; a < m_Selection->count(); ++a)
		{
			PageItem_ImageFrame* imageItem=m_Selection->itemAt(a)->asImageFrame();
			if (imageItem==NULL)
				continue;
			imageItem->setImageShown(!imageItem->imageShown());
			imageItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
		//Return to normal mode if in edit mode. We should not allow dragging of
		//an image in a frame if its not shown.
		if (appMode == modeEdit)
			m_ScMW->view->requestMode(modeNormal);
	}
}


void ScribusDoc::itemSelection_TogglePrintEnabled( )
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	UndoTransaction* activeTransaction = NULL;
	m_updateManager.setUpdatesDisabled();
	if (docSelectionCount > 1)
	{
		if (m_Selection->itemAt(0)->printEnabled())
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::DisablePrint, 0, Um::IDisablePrint));
		else
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::EnablePrint, 0, Um::IEnablePrint));
	}
	for ( uint a = 0; a < docSelectionCount; ++a)
	{
		m_Selection->itemAt(a)->togglePrintEnabled();
		m_Selection->itemAt(a)->update();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	m_updateManager.setUpdatesEnabled();
	changed();
	emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
}

void ScribusDoc::itemSelection_Transform(int nrOfCopies, QTransform matrix, int basepoint)
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	if (nrOfCopies == 0)
	{
		double gx, gy, gh, gw;
		PageItem *currItem = m_Selection->itemAt(0);
		if (m_Selection->count() == 1)
		{
			gx = currItem->xPos();
			gy = currItem->yPos();
			gw = currItem->width();
			gh = currItem->height();
		}
		else
			m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		for (int a = 0; a < m_Selection->count(); ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			double deltaX = currItem->xPos() - gx;
			double deltaY = currItem->yPos() - gy;
			QTransform matrixPre = QTransform();
			QTransform matrixAft = QTransform();
			switch (basepoint)
			{
			case 2:
				matrixPre.translate(-gw/2.0, -gh/2.0);
				matrixAft.translate(gw/2.0, gh/2.0);
				break;
			case 4:
				matrixPre.translate(-gw, -gh);
				matrixAft.translate(gw, gh);
				break;
			case 3:
				matrixPre.translate(0, -gh);
				matrixAft.translate(0, gh);
				break;
			case 1:
				matrixPre.translate(-gw, 0);
				matrixAft.translate(gw, 0);
				break;
			}
			currItem->PoLine.translate(deltaX, deltaY);
			currItem->PoLine.map(matrixPre);
			currItem->PoLine.map(matrix);
			currItem->PoLine.map(matrixAft);
			currItem->PoLine.translate(-deltaX, -deltaY);
			currItem->ContourLine.translate(deltaX, deltaY);
			currItem->ContourLine.map(matrixPre);
			currItem->ContourLine.map(matrix);
			currItem->ContourLine.map(matrixAft);
			currItem->ContourLine.translate(-deltaX, -deltaY);
//			currItem->Frame = false;
			currItem->ClipEdited = true;
//			currItem->FrameType = 3;
			AdjustItemSize(currItem);
		}
	}
	else
	{
		QList<PageItem*> Elements;
		bool savedAlignGrid = useRaster;
		bool savedAlignGuides = SnapGuides;
		useRaster = false;
		SnapGuides = false;
		DoDrawing = false;
		view()->updatesOn(false);
		m_Selection->delaySignalsOn();
		scMW()->setScriptRunning(true);
		QTransform comulatedMatrix = matrix;
		PageItem *currItem = m_Selection->itemAt(0);
		Elements.append(currItem);
		int rotBack = RotMode();
		RotMode ( 0 );
		scMW()->slotEditCopy();
		view()->Deselect(true);
		for (int b = 0; b < nrOfCopies; b++)
		{
			scMW()->slotEditPaste();
			double gx, gy, gh, gw;
			currItem = m_Selection->itemAt(0);
			if (m_Selection->count() == 1)
			{
				gx = currItem->xPos();
				gy = currItem->yPos();
				gw = currItem->width();
				gh = currItem->height();
			}
			else
				m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			for (int a = 0; a < m_Selection->count(); ++a)
			{
				currItem = m_Selection->itemAt(a);
				double deltaX = currItem->xPos() - gx;
				double deltaY = currItem->yPos() - gy;
				QTransform matrixPre = QTransform();
				QTransform matrixAft = QTransform();
				switch (basepoint)
				{
				case 2:
					matrixPre.translate(-gw/2.0, -gh/2.0);
					matrixAft.translate(gw/2.0, gh/2.0);
					break;
				case 4:
					matrixPre.translate(-gw, -gh);
					matrixAft.translate(gw, gh);
					break;
				case 3:
					matrixPre.translate(0, -gh);
					matrixAft.translate(0, gh);
					break;
				case 1:
					matrixPre.translate(-gw, 0);
					matrixAft.translate(gw, 0);
					break;
				}
				currItem->PoLine.translate(deltaX, deltaY);
				currItem->PoLine.map(matrixPre);
				currItem->PoLine.map(comulatedMatrix);
				currItem->PoLine.map(matrixAft);
				currItem->PoLine.translate(-deltaX, -deltaY);
				currItem->ContourLine.translate(deltaX, deltaY);
				currItem->ContourLine.map(matrixPre);
				currItem->ContourLine.map(comulatedMatrix);
				currItem->ContourLine.map(matrixAft);
				currItem->ContourLine.translate(-deltaX, -deltaY);
//				currItem->Frame = false;
				currItem->ClipEdited = true;
//				currItem->FrameType = 3;
				AdjustItemSize(currItem);
				Elements.append(currItem);
			}
			comulatedMatrix *= matrix;
		}
		for (int c = 0; c < Elements.count(); ++c)
		{
			m_Selection->addItem(Elements.at(c), true);
		}
		m_Selection->setGroupRect();
		RotMode (rotBack);
		useRaster = savedAlignGrid;
		SnapGuides = savedAlignGuides;
		DoDrawing = true;
		m_Selection->delaySignalsOff();
		view()->updatesOn(true);
		scMW()->setScriptRunning(false);
		m_Selection->connectItemToGUI();
	}
	m_updateManager.setUpdatesEnabled();
	//view()->DrawNew();
	regionsChanged()->update(QRectF());
	changed();
}


void ScribusDoc::itemSelection_FlipH()
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	if (docSelectionCount > 1)
	{
		UndoTransaction trans(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::FlipH, 0, Um::IFlipH));
		double gx, gy, gh, gw, ix, iy, iw, ih;
		m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			PageItem* currItem=m_Selection->itemAt(a);
			currItem->getBoundingRect(&ix, &iy, &iw, &ih);
			double dx =  ((gw / 2.0) -  ((ix - gx) + (iw - ix) / 2.0)) * 2.0;
			if (currItem->rotation() != 0.0)
			{
				double ix2, iy2, iw2, ih2;
				currItem->rotateBy(currItem->rotation() * -2.0);
				currItem->setRedrawBounding();
				currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
				currItem->moveBy(ix-ix2, iy-iy2, true);
				currItem->setRedrawBounding();
			}
			if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
				currItem->flipImageH();
			if (currItem->itemType() != PageItem::Line)
				MirrorPolyH(currItem);
			currItem->moveBy(dx, 0, true);
			currItem->GrStartX = currItem->width() - currItem->GrStartX;
			currItem->GrEndX = currItem->width() - currItem->GrEndX;
			if (currItem->isArc())
			{
				PageItem_Arc *ar = currItem->asArc();
				ar->arcStartAngle = (180 - ar->arcStartAngle) - ar->arcSweepAngle;
				if (ar->arcStartAngle < 0)
					ar->arcStartAngle += 360;
				else if (ar->arcStartAngle > 360)
					ar->arcStartAngle -= 360;
				ar->recalcPath();
				FPoint tp2(getMinClipF(&currItem->PoLine));
				currItem->PoLine.translate(-tp2.x(), -tp2.y());
				AdjustItemSize(currItem);
				emit updateEditItem();
			}
			else if (currItem->isRegularPolygon())
			{
				PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
				ar->polyRotation *= -1;
				ar->polyInnerRot *= -1;
				ar->recalcPath();
				emit updateEditItem();
			}
			else if (currItem->isSpiral())
				emit updateEditItem();
			currItem->setRedrawBounding();
		}
		trans.commit();
	}
	else
	{
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			PageItem* currItem=m_Selection->itemAt(a);
			if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
				currItem->flipImageH();
			if (currItem->itemType() != PageItem::Line)
				MirrorPolyH(currItem);
			else
			{
				double ix2, iy2, iw2, ih2, ix, iy, iw, ih;
				currItem->getBoundingRect(&ix, &iy, &iw, &ih);
				currItem->rotateBy(currItem->rotation() * -2.0);
				currItem->setRedrawBounding();
				currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
				currItem->moveBy(ix-ix2, iy-iy2, true);
				currItem->setRedrawBounding();
			}
			currItem->GrStartX = currItem->width() - currItem->GrStartX;
			currItem->GrEndX = currItem->width() - currItem->GrEndX;
			if (currItem->isArc())
			{
				PageItem_Arc *ar = currItem->asArc();
				ar->arcStartAngle = (180 - ar->arcStartAngle) - ar->arcSweepAngle;
				if (ar->arcStartAngle < 0)
					ar->arcStartAngle += 360;
				else if (ar->arcStartAngle > 360)
					ar->arcStartAngle -= 360;
				ar->recalcPath();
				FPoint tp2(getMinClipF(&currItem->PoLine));
				currItem->PoLine.translate(-tp2.x(), -tp2.y());
				AdjustItemSize(currItem);
				emit updateEditItem();
			}
			else if (currItem->isRegularPolygon())
			{
				PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
				ar->polyRotation *= -1;
				ar->polyInnerRot *= -1;
				ar->recalcPath();
				emit updateEditItem();
			}
			else if (currItem->isSpiral())
				emit updateEditItem();
		}
	}
	regionsChanged()->update(QRectF());
	changed();
	emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
}


void ScribusDoc::itemSelection_FlipV()
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	if (docSelectionCount > 1)
	{
		UndoTransaction trans(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::FlipV, 0, Um::IFlipV));
		double gx, gy, gh, gw, ix, iy, iw, ih;
		m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			PageItem* currItem=m_Selection->itemAt(a);
			currItem->getBoundingRect(&ix, &iy, &iw, &ih);
			double dx =  ((gh / 2.0) -  ((iy - gy) + (ih - iy) / 2.0)) * 2.0;
			if (currItem->rotation() != 0.0)
			{
				double ix2, iy2, iw2, ih2;
				currItem->rotateBy(currItem->rotation() * -2.0);
				currItem->setRedrawBounding();
				currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
				currItem->moveBy(ix-ix2, iy-iy2, true);
				currItem->setRedrawBounding();
			}
			if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
				currItem->flipImageV();
			if (currItem->itemType() != PageItem::Line)
				MirrorPolyV(currItem);
			currItem->moveBy(0, dx, true);
			currItem->GrStartY = currItem->height() - currItem->GrStartY;
			currItem->GrEndY = currItem->height() - currItem->GrEndY;
			if (currItem->isArc())
			{
				PageItem_Arc *ar = currItem->asArc();
				ar->arcStartAngle *= -1;
				ar->arcStartAngle -= ar->arcSweepAngle;
				if (ar->arcStartAngle < 0)
					ar->arcStartAngle += 360;
				else if (ar->arcStartAngle > 360)
					ar->arcStartAngle -= 360;
				ar->recalcPath();
				FPoint tp2(getMinClipF(&currItem->PoLine));
				currItem->PoLine.translate(-tp2.x(), -tp2.y());
				AdjustItemSize(currItem);
				emit updateEditItem();
			}
			else if (currItem->isRegularPolygon())
			{
				PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
				ar->polyRotation = 180.0 - ar->polyRotation;
				ar->polyRotation *= -1;
				ar->polyInnerRot *= -1;
				ar->recalcPath();
				emit updateEditItem();
			}
			else if (currItem->isSpiral())
				emit updateEditItem();
			currItem->setRedrawBounding();
		}
		regionsChanged()->update(QRectF());
		trans.commit();
	}
	else
	{
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			PageItem* currItem=m_Selection->itemAt(a);
			if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
				currItem->flipImageV();
			if (currItem->itemType() != PageItem::Line)
				MirrorPolyV(currItem);
			else
			{
				double ix2, iy2, iw2, ih2, ix, iy, iw, ih;
				currItem->getBoundingRect(&ix, &iy, &iw, &ih);
				currItem->rotateBy(currItem->rotation() * -2.0);
				currItem->setRedrawBounding();
				currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
				currItem->moveBy(ix-ix2, iy-iy2, true);
				currItem->setRedrawBounding();
			}
			currItem->GrStartY = currItem->height() - currItem->GrStartY;
			currItem->GrEndY = currItem->height() - currItem->GrEndY;
			if (currItem->isArc())
			{
				PageItem_Arc *ar = currItem->asArc();
				ar->arcStartAngle *= -1;
				ar->arcStartAngle -= ar->arcSweepAngle;
				if (ar->arcStartAngle < 0)
					ar->arcStartAngle += 360;
				else if (ar->arcStartAngle > 360)
					ar->arcStartAngle -= 360;
				ar->recalcPath();
				FPoint tp2(getMinClipF(&currItem->PoLine));
				currItem->PoLine.translate(-tp2.x(), -tp2.y());
				AdjustItemSize(currItem);
				emit updateEditItem();
			}
			else if (currItem->isRegularPolygon())
			{
				PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
				ar->polyRotation = 180.0 - ar->polyRotation;
				ar->polyRotation *= -1;
				ar->polyInnerRot *= -1;
				ar->recalcPath();
				emit updateEditItem();
			}
			else if (currItem->isSpiral())
				emit updateEditItem();
		}
		regionsChanged()->update(QRectF());
	}
	changed();
	emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
}

void ScribusDoc::itemSelection_Rotate(double angle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);

	if (itemSelection->count() == 0) return;
	
	if (itemSelection->count() > 1)
	{
		rotateGroup(angle, itemSelection);
	}
	else if (itemSelection->count() == 1)
	{
		RotateItem(angle, itemSelection->itemAt(0));
	}
	changed();
}

void ScribusDoc::itemSelection_ChangePreviewResolution(int id)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		bool found=false;
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			currItem = m_Selection->itemAt(i);
			if (currItem!=NULL)
				if (currItem->asImageFrame())
				{
					currItem->pixm.imgInfo.lowResType = id;
					if (!found)
						found=true;
				}
		}
		if (!found) //No image frames in the current selection!
			return;
		updatePic();
		disconnect( m_ScMW->scrActions["itemPreviewLow"], SIGNAL(triggeredData(int)) , 0, 0 );
		disconnect( m_ScMW->scrActions["itemPreviewNormal"], SIGNAL(triggeredData(int)) , 0, 0 );
		disconnect( m_ScMW->scrActions["itemPreviewFull"], SIGNAL(triggeredData(int)) , 0, 0 );
		m_ScMW->scrActions["itemPreviewLow"]->setChecked(id==m_ScMW->scrActions["itemPreviewLow"]->actionInt());
		m_ScMW->scrActions["itemPreviewNormal"]->setChecked(id==m_ScMW->scrActions["itemPreviewNormal"]->actionInt());
		m_ScMW->scrActions["itemPreviewFull"]->setChecked(id==m_ScMW->scrActions["itemPreviewFull"]->actionInt());
		connect( m_ScMW->scrActions["itemPreviewLow"], SIGNAL(triggeredData(int)), this, SLOT(itemSelection_ChangePreviewResolution(int)) );
		connect( m_ScMW->scrActions["itemPreviewNormal"], SIGNAL(triggeredData(int)), this, SLOT(itemSelection_ChangePreviewResolution(int)) );
		connect( m_ScMW->scrActions["itemPreviewFull"], SIGNAL(triggeredData(int)), this, SLOT(itemSelection_ChangePreviewResolution(int)) );
	}
}

void ScribusDoc::allItems_ChangePreviewResolution(int id)
{
	bool found=false;
	QList<PageItem*> allItems;
	for (int c=0; c<DocItems.count(); ++c)
	{
		PageItem *currItem = DocItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame())
				{
					currItem->pixm.imgInfo.lowResType = id;
					if (!found)
						found=true;
				}
			}
		}
		allItems.clear();
	}
	for (int c=0; c<MasterItems.count(); ++c)
	{
		PageItem *currItem = MasterItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame())
				{
					currItem->pixm.imgInfo.lowResType = id;
					if (!found)
						found=true;
				}
			}
		}
		allItems.clear();
	}
	for (int c=0; c<FrameItems.count(); ++c)
	{
		PageItem *currItem = FrameItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame())
				{
					currItem->pixm.imgInfo.lowResType = id;
					if (!found)
						found=true;
				}
			}
		}
		allItems.clear();
	}
	
	if (!found) //No image frames in the current selection!
		return;
	recalcPicturesRes();
	changed();
}

void ScribusDoc::item_setFrameShape(PageItem* item, int frameType, int count, double* points)
{
	if ((item->itemType() == PageItem::PolyLine) || (item->itemType() == PageItem::PathText))
			return;

	if (UndoManager::undoEnabled())
	{
		// Store shape info in this form:
		// CHANGE_SHAPE_TYPE - ID of the undo operation
		// OLD_FRAME_TYPE - original frame type
		// NEW_FRAME_TYPE - change of frame type
		// binary QPair<FPointArray, FPointArray> - .first original shape, .second new shape
		ItemState<QPair<FPointArray,FPointArray> > *is = new ItemState<QPair<FPointArray,FPointArray> >(Um::ChangeShapeType, "", Um::IBorder);
		is->set("CHANGE_SHAPE_TYPE", "change_shape_type");
		is->set("OLD_FRAME_TYPE", item->FrameType);
		is->set("NEW_FRAME_TYPE", frameType);
		// HACK: this is propably Evil Code (TM). I have to find better way...
		FPointArray newShape;
		int ix = 0;
		for (int i = 0; i < count/2; ++i)
		{
			double x = item->width()  * points[ix] / 100.0;
			double y = item->height() * points[ix+1] / 100.0;
			newShape.addPoint(x, y);
			ix += 2;
		}
		// HACK: end of hack
		is->setItem(qMakePair(item->shape(), newShape));
		UndoManager::instance()->action(item, is);
	}

	switch (frameType)
	{
	case 0:
		item->SetRectFrame();
		this->setRedrawBounding(item);
		break;
	case 1:
		item->SetOvalFrame();
		this->setRedrawBounding(item);
		break;
	default:
		item->SetFrameShape(count, points);
		this->setRedrawBounding(item);
		item->FrameType = frameType + 2;
		break;
	}
	item->update();
	changed();
}

void ScribusDoc::itemSelection_ClearItem(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			currItem = itemSelection->itemAt(i);
			if (currItem->asImageFrame())
			{
				if ((ScCore->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->PictureIsAvailable))
					ScCore->fileWatcher->removeFile(currItem->Pfile);
			}
			else
			if (currItem->asTextFrame() && ScCore->usingGUI())
			{
				if (currItem->itemText.length() != 0 && (currItem->nextInChain() == 0 || currItem->prevInChain() == 0))
				{
					int t = ScMessageBox::warning(m_ScMW, CommonStrings::trWarning,
										tr("Do you really want to clear all your text?"),
										QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
					if (t == QMessageBox::No)
						continue;
				}
			}
			currItem->clearContents();
		}
		updateFrameItems();
		regionsChanged()->update(QRectF());
		changed();
	}
}

QList<PageItem*>* ScribusDoc::GroupOfItem(QList<PageItem*>* itemList, PageItem* item)
{
	if (itemList->contains(item))
		return itemList;
	else
	{
		for (int a = 0; a < itemList->count(); a++)
		{
			if (itemList->at(a)->isGroup())
			{
				QList<PageItem*>* ite = GroupOfItem(&itemList->at(a)->groupItemList, item);
				if (ite != NULL)
					return ite;
			}
		}
		return NULL;
	}
	return NULL;
}

void ScribusDoc::itemSelection_DeleteItem(Selection* customSelection, bool forceDeletion)
{
	if (appMode == modeEditClip)
		return;
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	QList<PageItem*> delItems;
	QList<PageItem*> textInteractionItems;// text frames possibly interested in removal of selected items
	PageItem *currItem;
	QList<PageItem*>* itemList = Items;
	uint offs = 0;
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	itemSelection->delaySignalsOn();
	for (uint de = 0; de < selectedItemCount; ++de)
	{
		currItem = itemSelection->itemAt(offs);
		if ((((currItem->isSingleSel) && (!Items->contains(currItem))) || ((currItem->isSingleSel) && (currItem->isTableItem))) || (currItem->locked()))
		{
			if (currItem->locked())
			{
				offs++;
				continue;
			}
			else
			{
				itemList = GroupOfItem(Items, currItem);
				if (itemList == NULL)
				{
					itemList = Items;
					offs++;
					continue;
				}
			}
		}
		//CB FIXME remove this and include of storyeditor.h too
		if ((currItem->asTextFrame() || currItem->asPathText()) && currItem==m_ScMW->storyEditor->currentItem() && this==m_ScMW->storyEditor->currentDocument())
		{
			if (forceDeletion)
				m_ScMW->storyEditor->setCurrentDocumentAndItem(this, NULL);
			else
			{
				QMessageBox::critical(m_ScMW, tr("Cannot Delete In-Use Item"), tr("The item %1 is currently being edited by Story Editor. The delete operation will be cancelled").arg(currItem->itemName()), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
				itemSelection->delaySignalsOff();
				return;
			}
		}
		if(currItem->textFlowMode() != PageItem::TextFlowDisabled)
		{
			for(int tIdx(currItem->ItemNr - 1);tIdx >= 0;--tIdx)
			{
				if( (Items->at(tIdx)->asTextFrame())
					&& (!itemSelection->containsItem(Items->at(tIdx)))
					&& (currItem->getBoundingRect().intersects(Items->at(tIdx)->getBoundingRect()))
					&& (!textInteractionItems.contains(Items->at(tIdx))) )
				{
					textInteractionItems.append( Items->at(tIdx) );
				}
			}
		}
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		delItems.append(itemSelection->takeItem(offs));
	}
	if (delItems.count() == 0)
	{
		itemSelection->delaySignalsOff();
		return;
	}
	selectedItemCount = delItems.count();

	UndoTransaction* activeTransaction = NULL;
	if ((selectedItemCount > 1) && UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::Group + "/" + Um::Selection, Um::IGroup,
																			  Um::Delete, tooltip, Um::IDelete));

	for (uint de = 0; de < selectedItemCount; ++de)
	{
		currItem = delItems.at(selectedItemCount - (de + 1));
		if ((currItem->asImageFrame()) && ((ScCore->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->PictureIsAvailable)))
			ScCore->fileWatcher->removeFile(currItem->Pfile);
		if (currItem->asTextFrame())
		{
			currItem->dropLinks();
			
			/* this code will instead remove the contained text
			// unlink after
			currItem->unlink();
			if (before != 0)
			{
				// unlink before
				before->unlink();
				// repair link
				if (after != 0)
				{
					before->link(after);
				}
			}
			*/
		}
		if (currItem->isBookmark)
			//CB From view   emit DelBM(currItem);
			m_ScMW->DelBookMark(currItem);
		itemList->removeAll(currItem);
//		if (forceDeletion || !UndoManager::undoEnabled())
		if (forceDeletion)
			delete currItem;
	}
	if (UndoManager::undoEnabled() && (selectedItemCount > 0) && !forceDeletion)
	{
		ItemState< QList<PageItem*> > *is = new ItemState< QList<PageItem*> >(Um::Delete + " " + currItem->getUName(), "", Um::IDelete);
		is->setItem(delItems);
		is->set("DELETE_ITEM", "delete_item");
		undoManager->action(Pages->at(0), is, currItem->getUPixmap());
	}
	itemSelection->delaySignalsOff();
	updateFrameItems();
	renumberItemsInListOrder();
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	// JG resetting ElemToLink fixes #5629
	ElemToLink = NULL;
	
	foreach(PageItem* tii, textInteractionItems)
	{
		tii->update();
	}
	
	regionsChanged()->update(QRectF());
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	//CB FIXME remove this and tree.h too
//	m_ScMW->outlinePalette->BuildTree();

	if (itemSelection->isGUISelection() && !itemSelection->signalsDelayed())
	{
		if (itemSelection->count() == 0)
			emit firstSelectedItemType(-1);
		else
		{
			//emit HaveSel(Doc->m_Selection->itemAt(0)->itemType());
			itemSelection->itemAt(0)->emitAllToGUI();
		}
	}
	changed();
}


void ScribusDoc::itemSelection_SetItemFillTransparency(double t)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = m_Selection->itemAt(i);
			if (currItem->isGroup())
				continue;
			currItem->setFillTransparency(t);
		}
		regionsChanged()->update(QRectF());
		changed();
	}
}


void ScribusDoc::itemSelection_SetItemLineTransparency(double t)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = m_Selection->itemAt(i);
			currItem->setLineTransparency(t);
		}
		regionsChanged()->update(QRectF());
		changed();
	}
}


void ScribusDoc::itemSelection_SetItemFillBlend(int t)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = m_Selection->itemAt(i);
			if (currItem->isGroup())
				continue;
			currItem->setFillBlendmode(t);
		}
		regionsChanged()->update(QRectF());
		changed();
	}
}


void ScribusDoc::itemSelection_SetItemLineBlend(int t)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = m_Selection->itemAt(i);
			currItem->setLineBlendmode(t);
		}
		regionsChanged()->update(QRectF());
		changed();
	}
}


void ScribusDoc::itemSelection_SetLineGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem;
		currItem = itemSelection->itemAt(i);
		currItem->stroke_gradient = newGradient;
		currItem->update();
	}
	/*if (selectedItemCount>1)
		regionsChanged()->update(QRectF());*/
	m_updateManager.setUpdatesEnabled();
	changed();
//Teaser for jghali
}

void ScribusDoc::itemSelection_SetFillGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem;
		currItem = itemSelection->itemAt(i);
		currItem->fill_gradient = newGradient;
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetMaskGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem;
		currItem = itemSelection->itemAt(i);
		currItem->mask_gradient = newGradient;
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}


void ScribusDoc::itemSelection_SetOverprint(bool overprint, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem* currItem = itemSelection->itemAt(i);
		currItem->setOverprint(overprint);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_DoHyphenate()
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = m_Selection->itemAt(i);
			docHyphenator->slotHyphenate(currItem);
		}
		//FIXME: stop using m_View
		m_View->DrawNew(); //CB draw new until NLS for redraw through text chains
		changed();
	}
}

void ScribusDoc::itemSelection_DoDeHyphenate()
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = m_Selection->itemAt(i);
			docHyphenator->slotDeHyphenate(currItem);
		}
		//FIXME: stop using m_View
		m_View->DrawNew(); //CB draw new until NLS for redraw through text chains
		changed();
	}
}

void ScribusDoc::itemSelection_SendToLayer(int layerID)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction());
		QString tooltip = Um::ItemsInvolved + "\n";
		if (selectedItemCount > Um::ItemsInvolvedLimit)
			tooltip = Um::ItemsInvolved2 + "\n";
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			currItem->setLayer(layerID);
			if (selectedItemCount <= Um::ItemsInvolvedLimit)
				tooltip += "\t" + currItem->getUName() + "\n";
		}
		if (activeTransaction)
		{
			activeTransaction->commit(Um::Selection,
									  Um::IGroup,
									  Um::SendToLayer,
									  tooltip,
									  Um::ILayerAction);
			delete activeTransaction;
			activeTransaction = NULL;
		}
	}

	//CB why not just the following as we are calling for updatecontents anyway and we arent reflowing text
	//Doc->m_Selection->clear();
	//FIXME: stop using m_View
	m_View->Deselect(true);
	regionsChanged()->update(QRectF());
	changed();
}

void ScribusDoc::itemSelection_SetAlignment(int s, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(s));
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetImageOffset(double x, double y, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction());
		QString tooltip = Um::ItemsInvolved + "\n";
		if (selectedItemCount > Um::ItemsInvolvedLimit)
			tooltip = Um::ItemsInvolved2 + "\n";
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = itemSelection->itemAt(a);
			currItem->setImageXYOffset(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QTransform cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.rotate(currItem->imageRotation());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
			if (selectedItemCount <= Um::ItemsInvolvedLimit)
				tooltip += "\t" + currItem->getUName() + "\n";
			currItem->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit(Um::Selection,
									  Um::IGroup,
									  Um::ImageOffset,
									  tooltip,
									  Um::IImageScaling);
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetImageScale(double x, double y, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		m_updateManager.setUpdatesDisabled();
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction());
		QString tooltip = Um::ItemsInvolved + "\n";
		if (selectedItemCount > Um::ItemsInvolvedLimit)
			tooltip = Um::ItemsInvolved2 + "\n";
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = itemSelection->itemAt(a);
			currItem->setImageXYScale(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QTransform cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.rotate(currItem->imageRotation());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
			if (selectedItemCount <= Um::ItemsInvolvedLimit)
				tooltip += "\t" + currItem->getUName() + "\n";
			currItem->update();
		}
		if (activeTransaction)
		{
			activeTransaction->commit(Um::Selection,
								Um::IGroup,
								Um::ImageScale,
								tooltip,
								Um::IImageScaling);
			delete activeTransaction;
			activeTransaction = NULL;
		}
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetImageScaleAndOffset(double sx, double sy, double ox, double oy, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	UndoTransaction* outerTransaction = NULL;
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		outerTransaction = new UndoTransaction(undoManager->beginTransaction());
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		UndoTransaction* activeTransaction = NULL;
		PageItem *currItem = itemSelection->itemAt(a);
		if (UndoManager::undoEnabled())
			activeTransaction = new UndoTransaction(undoManager->beginTransaction());
		currItem->setImageXYScale(sx, sy);
		currItem->setImageXYOffset(ox/sx, oy/sy);
		if (currItem->imageClip.size() != 0)
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
			QTransform cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.rotate(currItem->imageRotation());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		if (activeTransaction)
		{	
			activeTransaction->commit(Um::Selection,
									  Um::IImageFrame,
									  Um::ImageScale,
									  tooltip,
									  Um::IImageScaling);
			delete activeTransaction;
			activeTransaction = NULL;
		}
		currItem->update();
	}
	if (outerTransaction)
	{
		outerTransaction->commit(Um::Selection,
								 Um::IGroup,
								 Um::ImageScale,
								 tooltip,
								 Um::IImageScaling);
		delete outerTransaction;
		outerTransaction = NULL;
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetImageRotation(double rot, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount != 0)
	{
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = itemSelection->itemAt(a);
			currItem->setImageRotation(rot);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QTransform cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.rotate(currItem->imageRotation());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
			currItem->update();
		}
		changed();
	}
}

void ScribusDoc::buildAlignItemList(Selection* customSelection)
{
	//CB TODO Handling custom selections
	//Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Selection* itemSelection = m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	PageItem *currItem;
	struct AlignObjs Object;
	AObjects.clear();
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		Object.Objects.clear();
		currItem->getBoundingRect(&Object.x1, &Object.y1, &Object.x2, &Object.y2);
		Object.Group = 0;
		Object.ObjNr = currItem->ItemNr;
		Object.Objects.append(currItem);
		AObjects.append(Object);
	}
	for (int i = 0; i < AObjects.count(); ++i)
	{
		AObjects[i].width = AObjects[i].x2 - AObjects[i].x1;
		AObjects[i].height = AObjects[i].y2 - AObjects[i].y1;
	}
}


bool ScribusDoc::startAlign()
{
	buildAlignItemList();
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount==0)
		return false;
		
	bool oneLocked=false;
	for (uint i = 0; i < alignObjectsCount && !oneLocked; ++i)
		for (int j = 0; j < AObjects[i].Objects.count() && !oneLocked; ++j)
			if (AObjects[i].Objects.at(j)->locked())
				oneLocked=true;
	int t = 2;
	if (oneLocked)
	{
		QMessageBox msgBox;
		QPushButton *abortButton = msgBox.addButton(QMessageBox::Cancel);
		QPushButton *lockButton = msgBox.addButton(tr("&Unlock All"), QMessageBox::AcceptRole);
		QPushButton *unlockButton = msgBox.addButton(tr("&Skip locked objects"), QMessageBox::AcceptRole);
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(CommonStrings::trWarning);
		msgBox.setText( tr("Some objects are locked."));
		msgBox.exec();
		if (msgBox.clickedButton() == abortButton)
			return false;
		else if (msgBox.clickedButton() == lockButton)
			t = 0;
		else if (msgBox.clickedButton() == unlockButton)
			t = 1;
	}
	
	QString targetTooltip;
	if (m_Selection->count() <= (int) Um::ItemsInvolvedLimit)
	{
		targetTooltip = Um::ItemsInvolved + "\n";
		for (int i = 0; i < m_Selection->count(); ++i)
			targetTooltip += "\t" + m_Selection->itemAt(i)->getUName() + "\n";
	}
	else
		targetTooltip = Um::ItemsInvolved2 + "\n";
	// Make the align action a single action in Action History
	m_alignTransaction = new UndoTransaction(undoManager->beginTransaction(Um::Selection, 0, Um::AlignDistribute, targetTooltip, Um::IAlignDistribute));
	if (oneLocked && (t == 0))
	{
		for (uint i = 0; i < alignObjectsCount; ++i)
			for (int j = 0; j < AObjects[i].Objects.count(); ++j)
				if (AObjects[i].Objects.at(j)->locked())
					AObjects[i].Objects.at(j)->setLocked(false);
	}
	return true;
}


void ScribusDoc::endAlign()
{
	changed();
	m_ScMW->HaveNewSel(m_Selection->itemAt(0)->itemType());
	for (int i = 0; i < m_Selection->count(); ++i)
		setRedrawBounding(m_Selection->itemAt(i));
	m_alignTransaction->commit(); // commit and send the action to the UndoManager
	delete m_alignTransaction;
	m_alignTransaction = NULL;
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_AlignItemRight(int i, double newX, AlignMethod how)
{
	double diff=newX-AObjects[i].x2;
	double width=AObjects[i].x2-AObjects[i].x1;
	bool resize = (how == alignByResizing && diff > -width);
	for (int j = 0; j < AObjects[i].Objects.count(); ++j)
		if (!AObjects[i].Objects.at(j)->locked()) {
			if (resize) {
				AObjects[i].Objects.at(j)->resizeBy(diff, 0.0);
				AObjects[i].Objects.at(j)->updateClip();
			}
			else AObjects[i].Objects.at(j)->moveBy(diff, 0.0);
		}
}

void ScribusDoc::itemSelection_AlignItemLeft(int i, double newX, AlignMethod how)
{
	double diff=newX-AObjects[i].x1;
	double width=AObjects[i].x2-AObjects[i].x1;
	bool resize = (how == alignByResizing && -diff > -width);
	for (int j = 0; j < AObjects[i].Objects.count(); ++j)
		if (!AObjects[i].Objects.at(j)->locked()) {
			AObjects[i].Objects.at(j)->moveBy(diff, 0.0);
			if (resize) {
				AObjects[i].Objects.at(j)->resizeBy(-diff, 0.0);
				AObjects[i].Objects.at(j)->updateClip();
			}
		}
}

void ScribusDoc::itemSelection_AlignItemBottom(int i, double newY, AlignMethod how)
{
	double diff=newY-AObjects[i].y2;
	double height=AObjects[i].y2-AObjects[i].y1;
	bool resize = (how == alignByResizing && diff > -height);
	for (int j = 0; j < AObjects[i].Objects.count(); ++j)
		if (!AObjects[i].Objects.at(j)->locked()) {
			if (resize) {
				AObjects[i].Objects.at(j)->resizeBy(0.0, diff);
				AObjects[i].Objects.at(j)->updateClip();
			}
			else AObjects[i].Objects.at(j)->moveBy(0.0, diff);
		}
}

void ScribusDoc::itemSelection_AlignItemTop(int i, double newY, AlignMethod how)
{
	double diff=newY-AObjects[i].y1;
	double height=AObjects[i].y2-AObjects[i].y1;
	bool resize = (how == alignByResizing && -diff > -height);
	for (int j = 0; j < AObjects[i].Objects.count(); ++j)
		if (!AObjects[i].Objects.at(j)->locked()) {
			AObjects[i].Objects.at(j)->moveBy(0.0, diff);
			if (resize) {
				AObjects[i].Objects.at(j)->resizeBy(0.0, -diff);
				AObjects[i].Objects.at(j)->updateClip();
			}
		}
}

void ScribusDoc::itemSelection_AlignLeftOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = std::numeric_limits<double>::max();
	switch ( currAlignTo )
	{
		case alignFirst:
			newX = AObjects[0].x1;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->Margins.Left;
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newX = qMin(AObjects[a].x1, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemRight(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignLeftIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x1;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->Margins.Left;
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newX = qMin(AObjects[a].x1, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemLeft(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignCenterHor(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = 0.0;
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x1 + (AObjects[0].width)/2;
			loopStart=1;
			break;
		case alignLast:
			{
				int objindex=alignObjectsCount-1;
				newX = AObjects[objindex].x1 + (AObjects[objindex].width)/2;
				loopEnd=alignObjectsCount-2;
			}
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			newX += currentPage()->width()/2;
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->Margins.Left;
			newX += (currentPage()->width() - currentPage()->Margins.Right - currentPage()->Margins.Left)/2;
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			double minX =  std::numeric_limits<double>::max();
			double maxX = -std::numeric_limits<double>::max();
			for (uint a = 0; a < alignObjectsCount; ++a)
			{
				minX = qMin(AObjects[a].x1, minX);
				maxX = qMax(AObjects[a].x2, maxX);
			}
			newX = minX + (maxX-minX)/2;
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newX-AObjects[i].x1-(AObjects[i].width)/2;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(diff, 0.0);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignRightIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = -std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x2;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();;
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();;
			newX -= currentPage()->Margins.Right;
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newX = qMax(AObjects[a].x2, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemRight(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignRightOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = -std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x2;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();
			newX -= currentPage()->Margins.Right;
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newX = qMax(AObjects[a].x2, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemLeft(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignTopOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y1;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->Margins.Top;
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newY = qMin(AObjects[a].y1, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemBottom(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignTopIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y1;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->Margins.Top;
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newY = qMin(AObjects[a].y1, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemTop(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignCenterVer(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = 0.0;
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y1 + (AObjects[0].height)/2;
			loopStart=1;
			break;
		case alignLast:
			{
				int objindex=alignObjectsCount-1;
				newY = AObjects[objindex].y1 + (AObjects[objindex].height)/2;
				loopEnd=alignObjectsCount-2;
			}
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			newY += currentPage()->height()/2;
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->Margins.Top;
			newY += (currentPage()->height() - currentPage()->Margins.Bottom - currentPage()->Margins.Top)/2;
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			double minY =  std::numeric_limits<double>::max();
			double maxY = -std::numeric_limits<double>::max();
			for (uint a = 0; a < alignObjectsCount; ++a)
			{
				minY = qMin(AObjects[a].y1, minY);
				maxY = qMax(AObjects[a].y2, maxY);
			}
			newY = minY + (maxY-minY)/2;
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newY-AObjects[i].y1-(AObjects[i].height)/2;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(0.0, diff);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignBottomIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = -std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y2;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			newY -= currentPage()->Margins.Bottom;
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newY = qMax(AObjects[a].y2, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemBottom(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignBottomOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = -std::numeric_limits<double>::max();
	switch ( currAlignTo )
	{
		case alignFirst:
			newY = AObjects[0].y2;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			newY -= currentPage()->Margins.Bottom;
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (uint a = 0; a < alignObjectsCount; ++a)
				newY = qMax(AObjects[a].y2, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemTop(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_DistributeLeft()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Xsorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Xsorted.contains(AObjects[a].x1))
			Xsorted.insert(AObjects[a].x1, a);
	}
	QMap<double,uint>::Iterator it = Xsorted.begin();
	QMap<double,uint>::Iterator itend = Xsorted.end();
	double minX=it.key();
	double maxX=it.key();
	while ( it != itend)
	{
		if (minX>it.key())
			minX=it.key();
		if (maxX<it.key())
			maxX=it.key();
		++it;
	}
		
	double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
	{
		double diff=minX + i*separation-AObjects[it.value()].x1;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(diff, 0.0);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeCenterH()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Xsorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Xsorted.contains(AObjects[a].x1+(AObjects[a].width)/2))
			Xsorted.insert(AObjects[a].x1+(AObjects[a].width)/2, a);
	}
	QMap<double,uint>::Iterator it = Xsorted.begin();
	QMap<double,uint>::Iterator itend = Xsorted.end();
	double minX=it.key();
	double maxX=it.key();
	while ( it != itend)
	{
		if (minX>it.key())
			minX=it.key();
		if (maxX<it.key())
			maxX=it.key();
		++it;
	}
		
	double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
	{
		double diff=minX + i*separation-AObjects[it.value()].x1-(AObjects[it.value()].width)/2;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(diff, 0.0);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeRight()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Xsorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Xsorted.contains(AObjects[a].x2))
			Xsorted.insert(AObjects[a].x2, a);
	}
	QMap<double,uint>::Iterator it = Xsorted.begin();
	QMap<double,uint>::Iterator itend = Xsorted.end();
	double minX=it.key();
	double maxX=it.key();
	while ( it != itend)
	{
		if (minX>it.key())
			minX=it.key();
		if (maxX<it.key())
			maxX=it.key();
		++it;
	}
	
	double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
	{
		double diff=minX + i*separation-AObjects[it.value()].x2;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(diff, 0.0);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeDistH(bool usingDistance, double distance, bool reverseDistribute)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> X1sorted, X2sorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!X1sorted.contains(AObjects[a].x1))
			X1sorted.insert(AObjects[a].x1, a);
		if (!X2sorted.contains(AObjects[a].x2))
			X2sorted.insert(AObjects[a].x2, a);
	}	
	uint left=X1sorted.begin().value();
	uint right=X2sorted[X2sorted.keys().back()];
	double minX=AObjects[left].x2;
	double maxX=AObjects[right].x2;
	double separation=0.0;
	if (!usingDistance)
	{
		double maxX=AObjects[right].x1;
		double totalSpace=maxX-minX;
		double totalWidth=0;
		uint insideObjectCount=0;
		for (uint a = 0; a < alignObjectsCount; ++a)
		{
			if (a==left)
				continue;
			if (a==right)
				continue;
			totalWidth += AObjects[a].width;
			++insideObjectCount;
		}
		separation=(totalSpace-totalWidth)/(insideObjectCount+1);
	}
	else
		separation=value2pts(distance, unitIndex());
	if (!reverseDistribute)
	{
		double currX=minX;
		for ( QMap<double,uint>::Iterator it = X1sorted.begin(); it != X1sorted.end(); ++it )
		{
			if (it.value()==left)
				continue;
			if (it.value()==right && !usingDistance)
				continue;
			currX+=separation;

			double diff=currX-AObjects[it.value()].x1;
			for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
				if (!AObjects[it.value()].Objects.at(j)->locked())
					AObjects[it.value()].Objects.at(j)->moveBy(diff, 0.0);
			currX+=AObjects[it.value()].width;
		}
	}
	else
	{
		QMapIterator<double,uint> it(X1sorted);
		it.toBack();
		double currX=maxX;
		while (it.hasPrevious())
		{
			it.previous();
			if (it.value()==right)
			{
				currX-=AObjects[it.value()].width;
				continue;
			}
			if (it.value()==left && !usingDistance)
				continue;
			currX-=separation;

			double diff=currX-AObjects[it.value()].x2;
			for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
				if (!AObjects[it.value()].Objects.at(j)->locked())
					AObjects[it.value()].Objects.at(j)->moveBy(diff, 0.0);
			currX-=AObjects[it.value()].width;
		}
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeBottom()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Ysorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Ysorted.contains(AObjects[a].y2))
			Ysorted.insert(AObjects[a].y2, a);
	}
	QMap<double,uint>::Iterator it = Ysorted.begin();
	QMap<double,uint>::Iterator itend = Ysorted.end();
	double minY=it.key();
	double maxY=it.key();
	while ( it != itend)
	{
		if (minY>it.key())
			minY=it.key();
		if (maxY<it.key())
			maxY=it.key();
		++it;
	}
		
	double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
	{
		double diff=minY + i*separation-AObjects[it.value()].y2;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(0.0, diff);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeCenterV()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Ysorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Ysorted.contains(AObjects[a].y1+(AObjects[a].height)/2))
			Ysorted.insert(AObjects[a].y1+(AObjects[a].height)/2, a);
	}
	QMap<double,uint>::Iterator it = Ysorted.begin();
	QMap<double,uint>::Iterator itend = Ysorted.end();
	double minY=it.key();
	double maxY=it.key();
	while ( it != itend)
	{
		if (minY>it.key())
			minY=it.key();
		if (maxY<it.key())
			maxY=it.key();
		++it;
	}
		
	double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
	{
		double diff=minY + i*separation-AObjects[it.value()].y1-(AObjects[it.value()].height)/2;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(0.0, diff);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeTop()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Ysorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Ysorted.contains(AObjects[a].y1))
			Ysorted.insert(AObjects[a].y1, a);
	}
	QMap<double,uint>::Iterator it = Ysorted.begin();
	QMap<double,uint>::Iterator itend = Ysorted.end();
	double minY=it.key();
	double maxY=it.key();
	while ( it != itend)
	{
		if (minY>it.key())
			minY=it.key();
		if (maxY<it.key())
			maxY=it.key();
		++it;
	}
		
	double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
	{
		double diff=minY + i*separation-AObjects[it.value()].y1;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(0.0,diff);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeDistV(bool usingDistance, double distance, bool reverseDistribute)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Y1sorted, Y2sorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Y1sorted.contains(AObjects[a].y1))
			Y1sorted.insert(AObjects[a].y1, a);
		if (!Y2sorted.contains(AObjects[a].y2))
			Y2sorted.insert(AObjects[a].y2, a);
	}	
	uint top=Y1sorted.begin().value();
	uint bottom=Y2sorted[Y2sorted.keys().back()];
	double minY=AObjects[top].y2;
	double maxY=AObjects[bottom].y2;
	double separation=0.0;
	if (!usingDistance)
	{
		double maxY=AObjects[bottom].y1;
		double totalSpace=maxY-minY;
		double totalHeight=0;
		uint insideObjectCount=0;
		for (uint a = 0; a < alignObjectsCount; ++a)
		{
			if (a==top)
				continue;
			if (a==bottom)
				continue;
			totalHeight += AObjects[a].height;
			++insideObjectCount;
		}
		separation=(totalSpace-totalHeight)/(insideObjectCount+1);
	}
	else
		separation=value2pts(distance, unitIndex());
	if (!reverseDistribute)
	{
		double currY=minY;
		for ( QMap<double,uint>::Iterator it = Y1sorted.begin(); it != Y1sorted.end(); ++it )
		{
			if (it.value()==top)
				continue;
			if (it.value()==bottom && !usingDistance)
				continue;
			currY+=separation;

			double diff=currY-AObjects[it.value()].y1;
			for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
				if (!AObjects[it.value()].Objects.at(j)->locked())
					AObjects[it.value()].Objects.at(j)->moveBy(0.0,diff);
			currY+=AObjects[it.value()].height;
		}
	}
	else
	{
		QMapIterator<double,uint> it(Y1sorted);
		it.toBack();
		double currY=maxY;
		while (it.hasPrevious())
		{
			it.previous();
			if (it.value()==bottom)
			{
				currY-=AObjects[it.value()].height;
				continue;
			}
			if (it.value()==top && !usingDistance)
				continue;
			currY-=separation;

			double diff=currY-AObjects[it.value()].y2;
			for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
				if (!AObjects[it.value()].Objects.at(j)->locked())
					AObjects[it.value()].Objects.at(j)->moveBy(0.0, diff);
			currY-=AObjects[it.value()].height;
		}
	}
	endAlign();
	usingDistance=false;
}


void ScribusDoc::itemSelection_DistributeAcrossPage(bool useMargins)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> X1sorted, X2sorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!X1sorted.contains(AObjects[a].x1))
			X1sorted.insert(AObjects[a].x1, a);
		if (!X2sorted.contains(AObjects[a].x2))
			X2sorted.insert(AObjects[a].x2, a);
	}	
	
	double totalSpace=0.0;
	if (useMargins)
		totalSpace=currentPage()->width()-currentPage()->Margins.Left-currentPage()->Margins.Right;
	else
		totalSpace=currentPage()->width();
	double totalWidth=0.0;
	uint insideObjectCount=0;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		totalWidth += AObjects[a].width;
		++insideObjectCount;
	}
	double separation=(totalSpace-totalWidth)/(insideObjectCount+1);
	double currX=currentPage()->xOffset();
	if (useMargins)
		currX+=currentPage()->Margins.Left;
	//Handle when our items are too wide for the page.
	if (separation<0.0)
	{
		separation=(totalSpace-totalWidth)/(insideObjectCount-1);
		currX-=separation;
	}
		
	for ( QMap<double,uint>::Iterator it = X1sorted.begin(); it != X1sorted.end(); ++it )
	{
		currX+=separation;
		double diff=currX-AObjects[it.value()].x1;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(diff, 0.0);
		currX+=AObjects[it.value()].width;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeDownPage(bool useMargins)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	QMap<double,uint> Y1sorted, Y2sorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Y1sorted.contains(AObjects[a].y1))
			Y1sorted.insert(AObjects[a].y1, a);
		if (!Y2sorted.contains(AObjects[a].y2))
			Y2sorted.insert(AObjects[a].y2, a);
	}	
	
	double totalSpace=0.0;
	if (useMargins)
		totalSpace=currentPage()->height()-currentPage()->Margins.Top-currentPage()->Margins.Bottom;
	else
		totalSpace=currentPage()->height();
	double totalHeight=0.0;
	uint insideObjectCount=0;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		totalHeight += AObjects[a].height;
		++insideObjectCount;
	}
	double separation=(totalSpace-totalHeight)/(insideObjectCount+1);
	double currY=currentPage()->yOffset();
	if (useMargins)
		currY+=currentPage()->Margins.Top;
	//Handle when our items are too high for the page.
	if (separation<0.0)
	{
		separation=(totalSpace-totalHeight)/(insideObjectCount-1);
		currY-=separation;
	}
		
	for ( QMap<double,uint>::Iterator it = Y1sorted.begin(); it != Y1sorted.end(); ++it )
	{
		currY+=separation;
		double diff=currY-AObjects[it.value()].y1;
		for (int j = 0; j < AObjects[it.value()].Objects.count(); ++j)
			if (!AObjects[it.value()].Objects.at(j)->locked())
				AObjects[it.value()].Objects.at(j)->moveBy(0.0, diff);
		currY+=AObjects[it.value()].height;
	}
	endAlign();
}

void ScribusDoc::beginUpdate()
{
	m_docUpdater->beginUpdate();
	m_updateManager.setUpdatesEnabled(false);
}

void ScribusDoc::endUpdate()
{
	m_updateManager.setUpdatesEnabled(true);
	m_docUpdater->endUpdate();
}

void ScribusDoc::itemSelection_SwapLeft()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	 QList<int> circleList;
	int circleListCounter=0;
	//X
	QMap<double,uint> Xsorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Xsorted.contains(AObjects[a].x1))
			Xsorted.insert(AObjects[a].x1, a);
	}
	QMap<double,uint>::Iterator itX = Xsorted.begin();
	QMap<double,uint>::Iterator itXend = Xsorted.end();
	double minX=itX.key();
	double maxX=itX.key();
	while ( itX != itXend)
	{
		if (minX>itX.key())
			minX=itX.key();
		if (maxX<itX.key())
			maxX=itX.key();
		++itX;
	}
	//Y
	QMap<double,uint> Ysorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Ysorted.contains(AObjects[a].y1))
			Ysorted.insert(AObjects[a].y1, a);
	}
	QMap<double,uint>::Iterator itY = Ysorted.begin();
	QMap<double,uint>::Iterator itYend = Ysorted.end();
	double minY=itY.key();
	double maxY=itY.key();
	while ( itY != itYend)
	{
		if (minY>itY.key())
			minY=itY.key();
		if (maxY<itY.key())
			maxY=itY.key();
		++itY;
	}

	itX = Xsorted.begin(); //first item is left most
	int itemIndex=itX.value(); //get our first item's index in the AObjects array
//	bool found=false;
//	double itXX=itX.key();
	minY =  std::numeric_limits<double>::max();
	maxY = -std::numeric_limits<double>::max();
	int nextItemIndex=itemIndex;
	circleList.append(nextItemIndex);
	++circleListCounter;
	// find the next X item with the minimum Y

	QMap<double,uint>::Iterator itX2_1 = Xsorted.begin();
	QMap<double,uint>::Iterator itLast = Xsorted.begin();
	double xBeginYValue=AObjects[itX2_1.value()].y1;
	while (itX2_1!=Xsorted.end())
	{
		if (AObjects[itX2_1.value()].y1<xBeginYValue)
		{
			circleList.append(itX2_1.value());
			++circleListCounter;
		}
		itLast=itX2_1;
		itX2_1++;
	}


	if (circleListCounter != static_cast<int>(alignObjectsCount)) //need to reverse back now
	{
		QMap<double,uint>::Iterator itX2_2 = itLast;
		while (itX2_2!=Xsorted.begin())
		{
			if (AObjects[itX2_2.value()].y1>=xBeginYValue)
			{
				circleList.append(itX2_2.value());
				++circleListCounter;
			}
			itX2_2--;
		}
	}

	int i=0;
	double swapX=AObjects[i].x1;
	double swapY=AObjects[i].y1;
	for (int j = 0; j < AObjects[i].Objects.count(); ++j)
		if (!AObjects[i].Objects.at(j)->locked())
			AObjects[i].Objects.at(j)->moveBy(AObjects[circleListCounter-1].x1-AObjects[i].x1, AObjects[circleListCounter-1].y1-AObjects[i].y1);
	++i;
	while(i<circleListCounter-1)
	{
		double diffX=swapX-AObjects[i].x1;
		double diffY=swapY-AObjects[i].y1;
		swapX=AObjects[i].x1;
		swapY=AObjects[i].y1;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(diffX, diffY);
		++i;
	}
	double diffX3=swapX-AObjects[circleListCounter-1].x1;
	double diffY3=swapY-AObjects[circleListCounter-1].y1;
	for (int j = 0; j < AObjects[circleListCounter-1].Objects.count(); ++j)
		if (!AObjects[circleListCounter-1].Objects.at(j)->locked())
			AObjects[circleListCounter-1].Objects.at(j)->moveBy(diffX3, diffY3);
	endAlign();
}

void ScribusDoc::itemSelection_SwapRight()
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	if (alignObjectsCount<=1)
		return;
	 QList<int> circleList;
	int circleListCounter=0;
	//X
	QMap<double,uint> Xsorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Xsorted.contains(AObjects[a].x1))
			Xsorted.insert(AObjects[a].x1, a);
	}
	QMap<double,uint>::Iterator itX = Xsorted.begin();
	QMap<double,uint>::Iterator itXend = Xsorted.end();
	double minX=itX.key();
	double maxX=itX.key();
	while ( itX != itXend)
	{
		if (minX>itX.key())
			minX=itX.key();
		if (maxX<itX.key())
			maxX=itX.key();
		++itX;
	}
	//Y
	QMap<double,uint> Ysorted;
	for (uint a = 0; a < alignObjectsCount; ++a)
	{
		if (!Ysorted.contains(AObjects[a].y1))
			Ysorted.insert(AObjects[a].y1, a);
	}
	QMap<double,uint>::Iterator itY = Ysorted.begin();
	QMap<double,uint>::Iterator itYend = Ysorted.end();
	double minY=itY.key();
	double maxY=itY.key();
	while ( itY != itYend)
	{
		if (minY>itY.key())
			minY=itY.key();
		if (maxY<itY.key())
			maxY=itY.key();
		++itY;
	}

	itX = Xsorted.begin(); //first item is left most
	int itemIndex=itX.value(); //get our first item's index in the AObjects array
//	bool found=false;
//	double itXX=itX.key();
	minY =  std::numeric_limits<double>::max();
	maxY = -std::numeric_limits<double>::max();
	int nextItemIndex=itemIndex;
	circleList.append(nextItemIndex);
	++circleListCounter;
	// find the next X item with the minimum Y

	QMap<double,uint>::Iterator itX2_1 = Xsorted.begin();
	QMap<double,uint>::Iterator itLast = Xsorted.begin();
	double xBeginYValue=AObjects[itX2_1.value()].y1;
	while (itX2_1!=Xsorted.end())
	{
		if (AObjects[itX2_1.value()].y1<xBeginYValue)
		{
			circleList.append(itX2_1.value());
			++circleListCounter;
		}
		itLast=itX2_1;
		itX2_1++;
	}


	if (circleListCounter!=static_cast<int>(alignObjectsCount)) //need to reverse back now
	{
		QMap<double,uint>::Iterator itX2_2 = itLast;
		while (itX2_2!=Xsorted.begin())
		{
			if (AObjects[itX2_2.value()].y1>=xBeginYValue)
			{
				circleList.append(itX2_2.value());
				++circleListCounter;
			}
			itX2_2--;
		}
	}

	int i=circleListCounter-1;
	double swapX=AObjects[i].x1;
	double swapY=AObjects[i].y1;
	for (int j = 0; j < AObjects[i].Objects.count(); ++j)
		if (!AObjects[i].Objects.at(j)->locked())
			AObjects[i].Objects.at(j)->moveBy(AObjects[0].x1-AObjects[i].x1, AObjects[0].y1-AObjects[i].y1);
	--i;
	while(i>0)
	{
		double diffX=swapX-AObjects[i].x1;
		double diffY=swapY-AObjects[i].y1;
		swapX=AObjects[i].x1;
		swapY=AObjects[i].y1;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(diffX, diffY);
		--i;
	}
	double diffX3=swapX-AObjects[0].x1;
	double diffY3=swapY-AObjects[0].y1;
	for (int j = 0; j < AObjects[0].Objects.count(); ++j)
		if (!AObjects[0].Objects.at(j)->locked())
			AObjects[0].Objects.at(j)->moveBy(diffX3, diffY3);
	endAlign();
}

void ScribusDoc::changed()
{
	setModified(true);
	// Do not emit docChanged signal() unnecessarily
	// Processing of that signal is slowwwwwww and
	// DocUpdater will trigger it when necessary
	if (!m_docUpdater->inUpdateSession())
	{
		emit docChanged();
	}
}

void ScribusDoc::invalidateAll()
{
	QList<PageItem*> allItems;
	for (int c = 0; c < DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->invalidateLayout();
		}
		allItems.clear();
	}
	for (int c=0; c < MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->invalidateLayout();
		}
		allItems.clear();
	}
	// for now hope that frameitems get invalidated by their parents layout() method.
}

void ScribusDoc::invalidateLayer(int layerID)
{
	QList<PageItem*> allItems;
	for (int c = 0; c < DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			if (ite->LayerID == layerID)
				ite->invalidateLayout();
		}
		allItems.clear();
	}
	if (this->masterPageMode())
	{
		for (int c=0; c < MasterItems.count(); ++c)
		{
			PageItem *ite = MasterItems.at(c);
			if (ite->isGroup())
				allItems = ite->getItemList();
			else
				allItems.append(ite);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				ite = allItems.at(ii);
				if (ite->LayerID == layerID)
					ite->invalidateLayout();
			}
			allItems.clear();
		}
	}
	// for now hope that frameitems get invalidated by their parents layout() method.
}

void ScribusDoc::invalidateRegion(QRectF region)
{
	QList<PageItem*> allItems;
	for (int c=0; c<DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			if (ite->getBoundingRect().intersects(region))
				ite->invalidateLayout();
		}
		allItems.clear();
	}
	for (int c=0; c<MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
		// for now invalidate all masteritems, should be only necessary in masterpagemode
			ite->invalidateLayout();
		}
		allItems.clear();
	}
	// for now hope that frameitems get invalidated by their parents layout() method.
}


Page* ScribusDoc::currentPage()
{
	return m_currentPage;
}


void ScribusDoc::setCurrentPage(Page *newPage)
{
	if (newPage==NULL)
		return;
	m_currentPage = newPage;
	if (m_ScMW)
	{
		m_ScMW->guidePalette->setDoc(this);
		if (!isLoading())
		{
			// #9476 : call setupPage with false arg to setup only guidePalette GUI
			// Otherwise expect problems when applying masterpages with guides
			m_ScMW->guidePalette->setupPage(false);
			m_ScMW->pagePalette->markPage(newPage->pageNr());
		}
	}
}


QPoint ScribusDoc::ApplyGrid(const QPoint& in)
{
	QPoint np;
	int onp = OnPage(in.x(), in.y());
	if (useRaster && (onp != -1))
	{
		np.setX(static_cast<int>(qRound((in.x() - Pages->at(onp)->xOffset()) / docPrefsData.guidesPrefs.minorGridSpacing) * docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset()));
		np.setY(static_cast<int>(qRound((in.y() - Pages->at(onp)->yOffset()) / docPrefsData.guidesPrefs.minorGridSpacing) * docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset()));
	}
	else
		np = in;
	return np;
}


FPoint ScribusDoc::ApplyGridF(const FPoint& in)
{
	FPoint np;
	int onp = OnPage(in.x(), in.y());
	if (useRaster && (onp != -1))
	{
		np.setX(qRound((in.x() - Pages->at(onp)->xOffset()) / docPrefsData.guidesPrefs.minorGridSpacing) * docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset());
		np.setY(qRound((in.y() - Pages->at(onp)->yOffset()) / docPrefsData.guidesPrefs.minorGridSpacing) * docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset());
	}
	else
		np = in;
	return np;
}


void ScribusDoc::itemSelection_MultipleDuplicate(ItemMultipleDuplicateData& mdData)
{
	if ((mdData.type==0 && mdData.copyCount<1) || (mdData.type==1 && (mdData.gridRows==1 && mdData.gridCols==1)))
		return;
	QString tooltip("");
	UndoTransaction* activeTransaction = NULL;

	if (UndoManager::undoEnabled())
	{ // Make multiple duplicate a single action in the action history
		if (m_Selection->count() > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::MultipleDuplicate,"",Um::IMultipleDuplicate));
		else
		{
			PageItem* item=m_Selection->itemAt(0);
			activeTransaction = new UndoTransaction(undoManager->beginTransaction(item->getUName(), item->getUPixmap(), Um::MultipleDuplicate, "", Um::IMultipleDuplicate));
		}
	}
	//FIXME: Enable paste without doing this save/restore, and stop using paste with all the refreshes
	bool savedAlignGrid = useRaster;
	bool savedAlignGuides = SnapGuides;
	useRaster = false;
	SnapGuides = false;
	DoDrawing = false;
	view()->updatesOn(false);
	m_Selection->delaySignalsOn();
	m_ScMW->setScriptRunning(true);
	if (mdData.type==0) // Copy and offset or set a gap
	{
		double dH = mdData.copyShiftGapH / docUnitRatio;
		double dV = mdData.copyShiftGapV / docUnitRatio;
		double dH2 = dH;
		double dV2 = dV;
		double dR = mdData.copyRotation;
		double dR2 = dR;
		if (mdData.copyShiftOrGap==1)
		{
			if (dH != 0.0)
				dH2 += m_Selection->width();
			if (dV != 0.0)
				dV2 += m_Selection->height();
		}
		m_ScMW->slotEditCopy();
		//FIXME: stop using m_View
		m_View->Deselect(true);
		for (int i=0; i<mdData.copyCount; ++i)
		{
			m_ScMW->slotEditPaste();
			for (int b=0; b<m_Selection->count(); ++b)
			{
				PageItem* bItem=m_Selection->itemAt(b);
				bItem->setLocked(false);
				MoveItem(dH2, dV2, bItem, true);
			}
			m_Selection->setGroupRect();
			if (dR != 0.0)
			{
				if (m_Selection->count() > 1)
					rotateGroup(dR2, FPoint(0,0)); //FIXME:av
				else
					RotateItem(dR2, m_Selection->itemAt(0));
			}
			dH2 += dH;
			dV2 += dV;
			if (mdData.copyShiftOrGap==1)
			{
				if (dH != 0.0)
					dH2 += m_Selection->width();
				if (dV != 0.0)
					dV2 += m_Selection->height();
			}
			dR2 += dR;
		}
		tooltip = tr("Number of copies: %1\nHorizontal shift: %2\nVertical shift: %3\nRotation: %4").arg(mdData.copyCount).arg(dH).arg(dV).arg(dR);
	}
	else
	if (mdData.type==1) // Create a grid of duplicated items
	{
		int copyCount=mdData.gridRows*mdData.gridCols;
		double dX=mdData.gridGapH/docUnitRatio + m_Selection->width();
		double dY=mdData.gridGapV/docUnitRatio + m_Selection->height();
// 		Personnaly I would prefer to first cleanup area but I guess it could mess up things elsewhere - pm
// 		m_ScMW->slotEditCut();
		m_ScMW->slotEditCopy();
		for (int i=0; i<mdData.gridRows; ++i) //skip 0, the item is the one we are copying
		{
			for (int j=0; j<mdData.gridCols; ++j) //skip 0, the item is the one we are copying
			{
				// We can comment out this conditional jump if we use slotEditCut(), would not be cool? ;-)
				if (i==0 && j==0)
					continue;
				// The true fix would be in slotEdit{Copy,Cut} but now its a reasonnably clean workaround
				m_View->Deselect(true);
				m_ScMW->slotEditPaste();
				for (int b=0; b<m_Selection->count(); ++b)
				{
					PageItem* bItem=m_Selection->itemAt(b);
					bItem->setLocked(false);
					MoveItem(j*dX, i*dY, bItem, true);
				}
			}
		}
		tooltip = tr("Number of copies: %1\nHorizontal gap: %2\nVertical gap: %3").arg(copyCount-1).arg(mdData.gridGapH).arg(mdData.gridGapV);
	}
	if (activeTransaction)
	{
		activeTransaction->commit("", 0, "", tooltip, 0);
		delete activeTransaction;
		activeTransaction = NULL;
	}
	//FIXME: Enable paste without doing this save/restore
	useRaster = savedAlignGrid;
	SnapGuides = savedAlignGuides;
	DoDrawing = true;
	m_Selection->delaySignalsOff();
	view()->updatesOn(true);
	m_ScMW->setScriptRunning(false);
	//FIXME: stop using m_View
	m_View->Deselect(true);
	view()->DrawNew();
	changed();
}


void ScribusDoc::itemSelection_ApplyImageEffects(ScImageEffectList& newEffectList, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=NULL);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem = itemSelection->itemAt(0);
		ScImageEffectList oldEffects(currItem->effectsInUse);
		currItem->effectsInUse = newEffectList;
		updatePic();
		
		// this messy part is for the undo action
		ItemState<QPair<ScImageEffectList, ScImageEffectList> > *state = 
		new ItemState<QPair<ScImageEffectList, ScImageEffectList> >(
				Um::ImageEffects, "", currItem->getUPixmap());
		state->set("APPLY_IMAGE_EFFECTS", "apply_image_effects");
		state->setItem(QPair<ScImageEffectList, ScImageEffectList>(oldEffects, currItem->effectsInUse));
		undoManager->action(currItem, state);
		changed();
	}
}


void ScribusDoc::itemSelection_ApplyArrowHead(int startArrowID, int endArrowID, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;

	UndoTransaction* activeTransaction = NULL;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = new UndoTransaction(undoManager->beginTransaction());
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		PageItem *currItem = itemSelection->itemAt(a);
		if (!(currItem->asLine() || currItem->asPolyLine() || currItem->asSpiral()))
			continue;
		if (startArrowID!=-1)
		{
			currItem->setStartArrowIndex(startArrowID);
		}
		if (endArrowID!=-1)
		{
			currItem->setEndArrowIndex(endArrowID);
		}
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		currItem->update();
	}
	QString t;
	if (startArrowID!=-1 && endArrowID!=-1)
		t=Um::StartAndEndArrow;
	else
		t=(startArrowID!=-1) ? Um::StartArrow : Um::EndArrow;
	if (activeTransaction)
	{
		activeTransaction->commit(Um::Selection,
								  Um::IGroup,
								  t,
								  tooltip,
								  Um::IArrow);
		delete activeTransaction;
		activeTransaction = NULL;
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_ApplyArrowScale(int startArrowSc, int endArrowSc, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		PageItem *currItem = itemSelection->itemAt(a);
		if (!(currItem->asLine() || currItem->asPolyLine() || currItem->asSpiral()))
			continue;
		if (startArrowSc !=  -1)
		{
			currItem->setStartArrowScale(startArrowSc);
		}
		if (endArrowSc != -1)
		{
			currItem->setEndArrowScale(endArrowSc);
		}
		currItem->update();
	}
	changed();
}


void ScribusDoc::createDefaultMasterPages()
{
	int setcol = pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns;
	if (setcol == 1)
	{
		addMasterPage(0, CommonStrings::trMasterPageNormal);
	}
	else if (setcol == 2)
	{
		Page *lp = addMasterPage(0, CommonStrings::trMasterPageNormalLeft);
		lp->LeftPg = 1;
		lp->Margins.Left = lp->initialMargins.Right;
		lp->Margins.Right = lp->initialMargins.Left;
		lp = addMasterPage(1, CommonStrings::trMasterPageNormalRight);
		lp->LeftPg = 0;
		lp->Margins.Right = lp->initialMargins.Right;
		lp->Margins.Left = lp->initialMargins.Left;
	}
	else if ((setcol == 3) || (setcol == 4))
	{
		Page *lp = addMasterPage(0, CommonStrings::trMasterPageNormalLeft);
		lp->LeftPg = 1;
		lp->Margins.Left = lp->initialMargins.Right;
		lp->Margins.Right = lp->initialMargins.Left;
		lp = addMasterPage(1, CommonStrings::trMasterPageNormalRight);
		lp->LeftPg = 0;
		lp->Margins.Right = lp->initialMargins.Right;
		lp->Margins.Left = lp->initialMargins.Left;
		lp = addMasterPage(2, CommonStrings::trMasterPageNormalMiddle);
		lp->LeftPg = 2;
		lp->Margins.Left = lp->initialMargins.Left;
		lp->Margins.Right = lp->initialMargins.Left;
	}
}


void ScribusDoc::createNewDocPages(int pageCount)
{
	int setcol = pageSets()[docPrefsData.docSetupPrefs.pagePositioning].Columns;
	int createCount=qMax(pageCount,1);
	if (setcol == 1)
	{
		for (int i = 0; i < createCount; ++i)
			addPage(i, CommonStrings::trMasterPageNormal, true);
	}
	else if (setcol == 2)
	{
		for (int i = 0; i < createCount; ++i)
		{
			PageLocation pageLoc = locationOfPage(i);
			if (pageLoc == LeftPage)
				addPage(i, CommonStrings::trMasterPageNormalLeft, true);
			else
				addPage(i, CommonStrings::trMasterPageNormalRight, true);
		}
	}
	else if ((setcol == 3) || (setcol == 4))
	{
		for (int i = 0; i < createCount; ++i)
		{
			PageLocation pageLoc = locationOfPage(i);
			if (pageLoc == LeftPage)
				addPage(i, CommonStrings::trMasterPageNormalLeft, true);
			else if (pageLoc == RightPage)
				addPage(i, CommonStrings::trMasterPageNormalRight, true);
			else
				addPage(i, CommonStrings::trMasterPageNormalMiddle, true);
		}
	}
}


void ScribusDoc::getClosestGuides(double xin, double yin, double *xout, double *yout, int *GxM, int *GyM, Page* refPage)
{
	*GxM = -1;
	*GyM = -1;
	Page* page = (refPage == NULL) ? currentPage() : refPage;
	QMap<double, uint> tmpGuidesSel;
	Guides tmpGuides = page->guides.horizontals(GuideManagerCore::Standard);
	Guides::iterator it;
	uint yg = 0;
	uint xg = 0;
	double viewScale=m_View->scale();
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
	{
		if (fabs((*it) + page->yOffset() - yin) < (docPrefsData.guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->yOffset() - yin), yg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GyM = tmpGuidesSel.begin().value();
		*yout = tmpGuides[*GyM];
	}
	tmpGuidesSel.clear();
	tmpGuides = page->guides.verticals(GuideManagerCore::Standard);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
	{
		if (fabs((*it) + page->xOffset() - xin) < (docPrefsData.guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->xOffset() - xin), xg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GxM = tmpGuidesSel.begin().value();
		*xout = tmpGuides[*GxM];
	}
	yg = 0;
	xg = 0;
	tmpGuidesSel.clear();
	tmpGuides = page->guides.horizontals(GuideManagerCore::Auto);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
	{
		if (fabs((*it) + page->yOffset() - yin) < (docPrefsData.guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->yOffset() - yin), yg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GyM = tmpGuidesSel.begin().value();
		*yout = tmpGuides[*GyM];
	}
	tmpGuidesSel.clear();
	tmpGuides = page->guides.verticals(GuideManagerCore::Auto);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
	{
		if (fabs((*it) + page->xOffset() - xin) < (docPrefsData.guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->xOffset() - xin), xg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GxM = tmpGuidesSel.begin().value();
		*xout = tmpGuides[*GxM];
	}
}


void ScribusDoc::SnapToGuides(PageItem *currItem)
{
	int pg = OnPage(currItem);
	double xout, yout;
	if (pg == -1)
		return;
	Page* page = Pages->at(pg);
	int GxM, GyM;
	
	//FIXME: stop using m_View
	getClosestGuides(0, currItem->yPos(), &xout, &yout, &GxM, &GyM);
	if (GyM != -1)
		currItem->setYPos(yout+page->yOffset());
	if (currItem->asLine())
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
//		double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
		double my = ma.m12() * currItem->width() + ma.dy();
		getClosestGuides(0, my, &xout, &yout, &GxM, &GyM);
		if (GyM != -1)
			currItem->moveBy(0.0, yout - my + page->yOffset());
	}
	else
	{
		getClosestGuides(0, currItem->yPos()+currItem->height(), &xout, &yout, &GxM, &GyM);
		if (GyM != -1)
			currItem->setYPos(yout-currItem->height()+page->yOffset());
	}
	getClosestGuides(currItem->xPos(), 0, &xout, &yout, &GxM, &GyM);
	if (GxM != -1)
		currItem->setXPos(xout+page->xOffset());
	if (currItem->asLine())
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.dx();
//		double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
		getClosestGuides(mx,  0, &xout, &yout, &GxM, &GyM);
		if (GxM != -1)
			currItem->moveBy(xout - mx + page->xOffset(), 0.0);
	}
	else
	{
		getClosestGuides(currItem->xPos()+currItem->width(), 0, &xout, &yout, &GxM, &GyM);
		if (GxM != -1)
			currItem->setXPos(xout-currItem->width()+page->xOffset());
	}
}


bool ScribusDoc::ApplyGuides(double *x, double *y)
{
//	m_SnapCounter++;
	bool ret = false;
	double xout, yout;
	int pg = OnPage(*x, *y);
	if (pg == -1)
		return ret;
	Page* page = Pages->at(pg);
	int GxM, GyM;

	//	if ((SnapGuides) && (m_SnapCounter > 1))
	if (SnapGuides)
	{
//		m_SnapCounter = 0;
		getClosestGuides(*x, *y, &xout, &yout, &GxM, &GyM, page);
		if (GxM != -1)
		{
			*x = xout+page->xOffset();
			ret = true;
		}
		if (GyM != -1)
		{
			*y = yout+page->yOffset();
			ret = true;
		}
		double invViewScale=1/m_View->scale();
		if (fabs(page->Margins.Left + page->xOffset() - *x) < (docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*x = page->Margins.Left+page->xOffset();
			ret = true;
		}
		if (fabs((page->width() - page->Margins.Right) + page->xOffset() - *x) < (docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*x = page->width() - page->Margins.Right+page->xOffset();
			ret = true;
		}
		if (fabs(page->Margins.Top + page->yOffset() - *y) < (docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*y = page->Margins.Top+page->yOffset();
			ret = true;
		}
		if (fabs((page->height() - page->Margins.Bottom)+page->yOffset() - *y) < (docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*y = page->height() - page->Margins.Bottom+page->yOffset();
			ret = true;
		}
	}
//	if (m_SnapCounter > 10)
//		m_SnapCounter = 0;
	return ret;
}


bool ScribusDoc::MoveItem(double newX, double newY, PageItem* currItem, bool fromMP)
{
	if (currItem->locked())
		return false;
	bool retw = false;
	double oldx = currItem->xPos();
	double oldy = currItem->yPos();
	currItem->moveBy(newX, newY);
/*	if ((useRaster) && (!m_View->operItemMoving) && (!fromMP) && (static_cast<int>(currentPage()->pageNr()) == currItem->OwnPage))
	{
		FPoint np = ApplyGridF(FPoint(currItem->xPos(), currItem->yPos()));
		currItem->setXYPos(np.x(), np.y());
	}
	if ((SnapGuides) && (!m_View->operItemMoving) && (appMode == modeNormal) && (!EditClip) && (!fromMP))
		SnapToGuides(currItem); */
	if ((currItem->xPos() != oldx) || (currItem->yPos() != oldy))
		retw = true;
	if (!fromMP)
	{
/*		if (GroupSel)
		{
			double gx, gy, gh, gw;
			setGroupRect();
			getGroupRect(&gx, &gy, &gw, &gh);
			emit ItemPos(gx, gy);
		}
		else */
		//CB if (!GroupSel)
		//CB	emit ItemPos(currItem->xPos(), currItem->yPos());
		//CB qDebug("if (!GroupSel) 			emit ItemPos(currItem->xPos(), currItem->yPos());");
	}
/*	if (!loading)
		emit UpdtObj(currentPage->pageNr(), b->ItemNr); */
	//FIXME: stop using m_View
//	QRect oldR(currItem->getRedrawBounding(m_View->scale()));
	setRedrawBounding(currItem);
//	QRect newR(currItem->getRedrawBounding(m_View->scale()));
//	if ((!m_View->operItemMoving) && (!currItem->Sizing))
//		emit updateContents(newR.unite(oldR));
	currItem->OwnPage = OnPage(currItem);
	return retw;
}


void ScribusDoc::RotateItem(double angle, int ite)
{
	RotateItem(angle, Items->at(ite));
}


void ScribusDoc::RotateItem(double angle, PageItem *currItem)
{
	if (currItem->locked())
		return;
	QRectF oldR = currItem->getBoundingRect();
//	if ((Doc->RotMode != 0) && (m_MouseButtonPressed))
	if (rotMode != 0)
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.scale(1, 1);
		ma.rotate(currItem->rotation());
		double ro = angle - currItem->rotation();
		currItem->setRotation(angle);
		FPoint n(0,0);
		switch (rotMode)
		{
		case 2:
			ma.translate(currItem->width()/2.0, currItem->height()/2.0);
			n = FPoint(-currItem->width()/2.0, -currItem->height()/2.0);
			break;
		case 4:
			ma.translate(currItem->width(), currItem->height());
			n = FPoint(-currItem->width(), -currItem->height());
			break;
		case 3:
			ma.translate(0, currItem->height());
			n = FPoint(0, -currItem->height());
			break;
		case 1:
			ma.translate(currItem->width(), 0);
			n = FPoint(-currItem->width(), 0);
			break;
		}
		ma.rotate(ro);
		double x = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		double y = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		bool oldS = currItem->Sizing;
		currItem->Sizing = true;
		MoveItem(x - currItem->xPos(), y - currItem->yPos(), currItem);
		currItem->Sizing = oldS;
	}
	else
	{
		currItem->setRotation(angle);
		setRedrawBounding(currItem);
	}
	if (!loading)
	{
		QRectF newR(currItem->getBoundingRect());
		regionsChanged()->update(newR.unite(oldR));
	}
	//emit SetAngle(currItem->rotation());
}



void ScribusDoc::MoveRotated(PageItem *currItem, FPoint npv, bool fromMP)
{
	QTransform ma;
	ma.translate(currItem->xPos(), currItem->yPos());
	ma.rotate(currItem->rotation());
	double mxc = currItem->xPos() - (ma.m11() * npv.x() + ma.m21() * npv.y() + ma.dx());
	double myc = currItem->yPos() - (ma.m22() * npv.y() + ma.m12() * npv.x() + ma.dy());
	MoveItem(-mxc, -myc, currItem, fromMP);
}


bool ScribusDoc::SizeItem(double newX, double newY, int ite, bool fromMP, bool DoUpdateClip, bool redraw)
{
	return SizeItem(newX, newY, Items->at(ite), fromMP, DoUpdateClip, redraw);
}


bool ScribusDoc::SizeItem(double newX, double newY, PageItem *pi, bool fromMP, bool DoUpdateClip, bool redraw)
{
	PageItem *currItem = pi;
	if (currItem->locked())
		return false;
	QPainter p;
	QRectF oldR(currItem->getBoundingRect());
	//Uncomment for stopping resize in any direction
	//and remove the height/width <0 tests in item sizing switch
	/*
	if (!currItem->asLine())
	{
		newX = qMax(newX, 1);
		newY = qMax(newY, 1);
	}
	*/
	std::auto_ptr<UndoTransaction> activeTransaction;
	if (UndoManager::undoEnabled())
	{
		QString transacDesc = QString(Um::ResizeFromTo).arg(currItem->oldWidth).arg(currItem->oldHeight).arg(newX).arg(newY);
		activeTransaction.reset( new UndoTransaction(undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), Um::Resize, transacDesc, Um::IResize)) );
	}
	int ph = static_cast<int>(qMax(1.0, currItem->lineWidth() / 2.0));
	QTransform ma;
	ma.rotate(currItem->rotation());
	double dX = ma.m11() * (currItem->width() - newX) + ma.m21() * (currItem->height() - newY) + ma.dx();
	double dY = ma.m22() * (currItem->height() - newY) + ma.m12() * (currItem->width() - newX) + ma.dy();
//	#8541, #8761: "when resizing with ALT-arrow, the size values in the PP aren't updated"
//	currItem->setWidthHeight(newX, newY, true);
	currItem->setWidthHeight(newX, newY);
	if ((rotMode != 0) && (fromMP) && (!isLoading()) && (appMode == modeNormal))
	{
		double moveX=dX, moveY=dY;
		if (rotMode==2)
		{
			moveX/=2.0;
			moveY/=2.0;
		}
		else if (rotMode==3)
			moveX=0.0;
		else if (rotMode==1)
			moveY=0.0;
		MoveItem(moveX, moveY, currItem);
	}
	if ((currItem->asImageFrame()) && (!currItem->Sizing) && (appMode != modeEditClip))
	{
		currItem->AdjustPictScale();
	}
	if (currItem->asLine())
	{
		if (!fromMP)
		{
			FPoint g(currItem->xPos(), currItem->yPos());
			FPoint t(currItem->width(), 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
			t -= g;
			currItem->setRotation( atan2(t.y(), t.x()) * (180.0/M_PI));
			currItem->setWidthHeight( sqrt(pow(t.x(), 2) + pow(t.y(), 2)), 1.0);
			//currItem->setXYPos(currItem->xPos(), currItem->yPos());
		}
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
	}
	setRedrawBounding(currItem);
	currItem->OwnPage = OnPage(currItem);
	if (currItem->Sizing)
	{
		currItem->FrameOnly = true;

		if (currItem->FrameType == 0 || currItem->asLine())
		{
			if (activeTransaction.get())
			{
				currItem->checkChanges();
				activeTransaction->commit();
			}
			return true;
		}
		
		currItem->updateClip();
//		currItem->updateGradientVectors();

		if (activeTransaction.get())
		{
			currItem->checkChanges();
			activeTransaction->commit();
		}
		return true;
	}
	if (DoUpdateClip)
	{
		// (JG) #5988 : break image position when resizing frame especially on undo
		/*double nX=0.0, nY=0.0;
		if (fromMP)
		{
			if (currItem->imageFlippedH())
				nX=-(currItem->width() - currItem->OldB2)/currItem->imageXScale();
			if (currItem->imageFlippedV())
				nY=-(currItem->height() - currItem->OldH2)/currItem->imageYScale();
		}
		else
		{
			if (!currItem->imageFlippedH())
				nX=(currItem->width() - currItem->OldB2)/currItem->imageXScale();
			if (!currItem->imageFlippedV())
				nY=(currItem->height() - currItem->OldH2)/currItem->imageYScale();
		}
		if (nX!=0.0 || nY!=0.0)
			currItem->moveImageInFrame(dX,dY);*/
		currItem->updateClip();
	}
//	currItem->updateGradientVectors();
	if (!loading)
	{
		QRectF newR(currItem->getBoundingRect());
		invalidateRegion(newR.unite(oldR));
	}
	if ((redraw) && (!loading))
	{
		QRectF newR(currItem->getBoundingRect());
		regionsChanged()->update(newR.unite(oldR));
	}
	if (!fromMP)
	{
		if (m_Selection->isMultipleSelection())
		{
			double gx, gy, gh, gw;
			m_Selection->setGroupRect();
			m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
//			qDebug() << "doc, emit w&h, when was this used?";
			emit widthAndHeight(gw, gh);
		}
	}
	currItem->setCornerRadius(qMin(currItem->cornerRadius(), qMin(currItem->width(),currItem->height())/2));
	if (activeTransaction.get())
	{
		currItem->checkChanges();
		activeTransaction->commit();
	}
	return true;
}


bool ScribusDoc::MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP, bool constrainRotation)
{
	PageItem *currItem = Items->at(ite);
	return MoveSizeItem(newX, newY, currItem, fromMP, constrainRotation);
}

bool ScribusDoc::MoveSizeItem(FPoint newX, FPoint newY, PageItem* currItem, bool fromMP, bool constrainRotation)
{
	QRectF oldR(currItem->getBoundingRect());
	if (currItem->asLine())
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.dx();
		double my = ma.m12() * currItem->width() + ma.dy();
		MoveItem(newX.x(), newX.y(), currItem, fromMP);
		double newRot=xy2Deg(mx - currItem->xPos(), my - currItem->yPos());
		//CB Hmm should work, doesnt. (constraining on the first point of a line) FIXME
		//if (constrainRotation)
		//	qDebug() << QString("%1").arg(constrainAngle(newRot)));
		currItem->setRotation(newRot);
		currItem->setWidthHeight(sqrt(pow(mx - currItem->xPos(),2)+pow(my - currItem->yPos(),2)), 1.0);
		currItem->updateClip();
		setRedrawBounding(currItem);
		QRectF newR(currItem->getBoundingRect());
		if (!loading)
		{
			regionsChanged()->update(oldR);
			regionsChanged()->update(newR);
		}
//		updateContents(newR.unite(oldR));
	}
	else
	{
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		if (currItem->rotation() != 0)
		{
			FPoint npv(newX.x(), newX.y());
			QTransform ma3;
			ma3.translate(currItem->xPos(), currItem->yPos());
			ma3.rotate(currItem->rotation());
			double mxc3 = currItem->xPos() - (ma3.m11() * npv.x() + ma3.m21() * npv.y() + ma3.dx());
			double myc3 = currItem->yPos() - (ma3.m22() * npv.y() + ma3.m12() * npv.x() + ma3.dy());
			SizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), currItem, fromMP, true, false);
			MoveItem(-mxc3, -myc3, currItem, fromMP);
		}
		else
		{
			SizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), currItem, fromMP, true, false);
			MoveItem(newX.x(), newX.y(), currItem, fromMP);
		}
	}
	return true;
}


void ScribusDoc::AdjustItemSize(PageItem *currItem, bool includeGroup, bool moveInGroup)
{
	bool siz = currItem->Sizing;
	currItem->Sizing = false;
	if ((!(currItem->isGroup() || currItem->isSymbol())) || includeGroup)
	{
		FPointArray Clip;
		Clip = currItem->PoLine;
		FPoint tp2(getMinClipF(&Clip));
		SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem, true, false, false);
		Clip.translate(-tp2.x(), -tp2.y());
		if (currItem->rotation() != 0)
		{
			FPoint npv(tp2.x(), tp2.y());
			MoveRotated(currItem, npv, true);
		}
		else
			MoveItem(tp2.x(), tp2.y(), currItem, true);
		if (!currItem->imageFlippedH())
			currItem->moveImageInFrame(-tp2.x()/currItem->imageXScale(), 0);
		if (!currItem->imageFlippedV())
			currItem->moveImageInFrame(0, -tp2.y()/currItem->imageYScale());
		FPoint tp(getMaxClipF(&Clip));
		if (currItem->imageFlippedH())
			currItem->moveImageInFrame((currItem->width() - tp.x())/currItem->imageXScale(), 0);
		if (currItem->imageFlippedV())
			currItem->moveImageInFrame(0, (currItem->height() - tp.y())/currItem->imageYScale());
		SizeItem(tp.x(), tp.y(), currItem, true, false);
		currItem->PoLine = Clip.copy();
		if (currItem->isGroup() && includeGroup && moveInGroup)
		{
			for (int em = 0; em < currItem->groupItemList.count(); ++em)
			{
				PageItem* embedded = currItem->groupItemList.at(em);
				MoveItem(-tp2.x(), -tp2.y(), embedded, true);
			}
		}
	}
	currItem->ClipEdited = true;
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if (currItem->asPolyLine() || currItem->asSpiral())
		currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
	else if (currItem->asPathText())
		currItem->updatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->updateGradientVectors();
	currItem->Sizing = siz;
}



void ScribusDoc::moveGroup(double x, double y, bool fromMP, Selection* customSelection)
{
	double Scale = 1; //FIXME:av should all be in doc coordinates
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	if (x == 0.0 && y == 0.0)
		return;
	
	PageItem* currItem;
	double gx, gy, gw, gh;
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	//QPoint in(qRound(gx*Scale), qRound(gy*Scale));
	//in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	//QPoint out = in;//contentsToViewport(in);
	QRectF OldRect = QRectF(gx, gy, gw, gh); //out.x(), out.y(), qRound(gw*Scale), qRound(gh*Scale));
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		MoveItem(x, y, currItem, fromMP);
	}
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	if (itemSelection->isMultipleSelection())
	{
		// FIXME:av emit ItemPos(gx, gy);
		currItem = itemSelection->itemAt(0);
		GroupOnPage(currItem);
	}
	QPoint in2(qRound(gx*Scale), qRound(gy*Scale));
	//in2 -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	OldRect = OldRect.united(QRectF(in2.x()/*+contentsX()*/, in2.y()/*+contentsY()*/, qRound(gw*Scale), qRound(gh*Scale))); //FIXME:av
	regionsChanged()->update(OldRect.adjusted(-10, -10, 20, 20));
}

void ScribusDoc::rotateGroup(double angle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);

	double gx, gy, gh, gw;
	FPoint rotationPoint(0, 0);
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	if (this->rotMode == 0)
		rotationPoint = FPoint(gx, gy);
	if (this->rotMode == 1)
		rotationPoint = FPoint(gx, gy);
	if (this->rotMode == 2)
		rotationPoint = FPoint(gx + gw / 2.0, gy + gh / 2.0);
	if (this->rotMode == 3)
		rotationPoint = FPoint(gx, gy+gh);
	if (this->rotMode == 4)
		rotationPoint = FPoint(gx+gw, gy+gh);
	rotateGroup(angle, rotationPoint, itemSelection);
}

void ScribusDoc::rotateGroup(double angle, FPoint RCenter, Selection* customSelection)
{
	double gxS, gyS, ghS, gwS;
	double sc = 1; // FIXME:av Scale;
	PageItem* currItem;
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	
	itemSelection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
	QTransform ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(angle);
//	gxS -= minCanvasCoordinate.x();
//	gyS -= minCanvasCoordinate.y();
	QRect oldR = QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10));
	FPoint n;
	for (int a = 0; a < itemSelection->count(); ++a)
	{
		currItem = itemSelection->itemAt(a);
		n = FPoint(currItem->xPos() - RCenter.x(), currItem->yPos() - RCenter.y());
		currItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
		currItem->rotateBy(angle);
		setRedrawBounding(currItem);
	}
	currItem = itemSelection->itemAt(0);
	GroupOnPage(currItem);
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
//	gxS -= minCanvasCoordinate.x();
//	gyS -= minCanvasCoordinate.y();
	regionsChanged()->update(QRectF(gxS-5, gyS-5, gwS+10, ghS+10).unite(oldR));
}

void ScribusDoc::scaleGroup(double scx, double scy, bool scaleText, Selection* customSelection, bool scaleLine)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	PageItem *bb;
	double gx, gy, gh, gw, x, y;
	int aa;
	double sc = 1; //FIXME:av Scale;
	int drm = rotMode;
	rotMode = 0;
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
//	gx -= minCanvasCoordinate.x();
//	gy -= minCanvasCoordinate.y();
	QRect oldR = QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10));
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	double origGW = gw;
	double origGH = gh;
	updateManager()->setUpdatesDisabled();

	for (uint a = 0; a < selectedItemCount; ++a)
	{
		bb = itemSelection->itemAt(a);
		if ((bb->locked()) || (bb->sizeLocked()))
			continue;
		bb->OldB = bb->width();
		bb->OldH = bb->height();
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		double dw = (bb->width() * scx) - bb->width();
		double dh = (bb->height() * scy) - bb->height();
		double dsch = 1.0;
		double dscw = 1.0;
		if (bb->isArc())
		{
			PageItem_Arc* item = bb->asArc();
			if (bb->height() != 0.0)
				dsch = item->arcHeight / bb->height();
			if (bb->width() != 0.0)
				dscw = item->arcWidth / bb->width();
		}
		bb->Sizing = false;
		double oldRot, oldLocalX, oldLocalY;
		oldRot = bb->rotation();
		oldLocalX = bb->imageXOffset();
		oldLocalY = bb->imageYOffset();
		FPointArray gr;
		gr.addPoint(bb->GrStartX, bb->GrStartY);
		gr.addPoint(bb->GrEndX, bb->GrEndY);
		gr.addPoint(bb->GrFocalX, bb->GrFocalY);
		gr.addPoint(bb->GrControl1);
		gr.addPoint(bb->GrControl2);
		gr.addPoint(bb->GrControl3);
		gr.addPoint(bb->GrControl4);
		gr.addPoint(bb->GrControl5);
		FPoint g(gx, gy);
		FPoint b(0, 0, bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		b -= g;
		FPoint b1(b.x(), b.y(), 0, 0, 0, scx, scy);
		FPoint t(bb->width(), 0, bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		t -= g;
		FPoint t1(t.x(), t.y(), 0, 0, 0, scx, scy);
		FPoint h(0, bb->height(), bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		h -= g;
		FPoint h1(h.x(), h.y(), 0, 0, 0, scx, scy);
		if (bb->isGroup() || scaleLine)				// change the LineWidth only when the item is within a real Group
		{
			if (bb->lineWidth() != 0)				// don't try to scale hairlines
				bb->setLineWidth(qMax(bb->lineWidth()*((scx+scy)/2), 0.01));
		}
		if (bb->itemType() == PageItem::Line)
		{
			bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
			bb->setWidth(sqrt(pow(t1.x()-b1.x(),2)+pow(t1.y()-b1.y(),2)));
			bb->setXYPos(b1.x()+gx, b1.y()+gy);
		}
		else
		{
			FPoint oldPos(bb->xPos(), bb->yPos());
			QTransform ma;
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QTransform ma2;
			ma2.translate(gx-bb->xPos(), gy-bb->yPos());
			ma2.scale(scx, scy);
			bb->PoLine.map(ma2);
			bb->setRotation(0.0);
			bb->ClipEdited = true;
			if (bb->isArc())
			{
				PageItem_Arc* item = bb->asArc();
				item->arcWidth += dw * dscw;
				item->arcHeight += dh * dsch;
				item->recalcPath();
				FPoint tp2(getMinClipF(&bb->PoLine));
				bb->PoLine.translate(-tp2.x(), -tp2.y());
			}
			if (bb->isSpiral())
			{
				PageItem_Spiral* item = bb->asSpiral();
				item->recalcPath();
			}
			else
				AdjustItemSize(bb, true, false);
			QTransform ma3;
			ma3.translate(gx, gy);
			ma3.scale(scx, scy);
			FPoint n(gx-oldPos.x(), gy-oldPos.y());
			x = ma3.m11() * n.x() + ma3.m21() * n.y() + ma3.dx();
			y = ma3.m22() * n.y() + ma3.m12() * n.x() + ma3.dy();
			MoveItem(gx-x, gy-y, bb, true);
			if (oldRot != 0)
			{
				bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
				QTransform ma;
				ma.rotate(-bb->rotation());
				bb->PoLine.map(ma);
				AdjustItemSize(bb, true, false);
			}
		}
		if (scaleText)
		{
#ifndef NLS_PROTO
//			bb->setFontSize(qMax(qRound(bb->fontSize()*((scx+scy)/2)), 1));
			if ((bb->itemText.length() != 0) && (!bb->isTableItem))
			{
//				bb->setLineSpacing(((bb->fontSize() / 10.0) * static_cast<double>(Doc->typographicSettings.autoLineSpacing) / 100) + (bb->fontSize() / 10.0));
				for (aa = 0; aa < bb->itemText.length(); ++aa)
					bb->itemText.item(aa)->setFontSize(qMax(qRound(bb->itemText.item(aa)->fontSize()*((scx+scy)/2)), 1));
				if (bb->asPathText())
					bb->updatePolyClip();
			}
#endif
		}
		bb->setImageXYOffset(oldLocalX, oldLocalY);
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		QTransform ma4;
		ma4.rotate(oldRot);
		ma4.scale(scx, scy);
		gr.map(ma4);
		for (int grow = 0; grow < bb->meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < bb->meshGradientArray[grow].count(); gcol++)
			{
				bb->meshGradientArray[grow][gcol].transform(ma4);
			}
		}
		bb->ContourLine.map(ma4);
		bb->GrStartX = gr.point(0).x();
		bb->GrStartY = gr.point(0).y();
		bb->GrEndX = gr.point(1).x();
		bb->GrEndY = gr.point(1).y();
		bb->GrFocalX = gr.point(2).x();
		bb->GrFocalY = gr.point(2).y();
		bb->GrControl1 = gr.point(3);
		bb->GrControl2 = gr.point(4);
		bb->GrControl3 = gr.point(5);
		bb->GrControl4 = gr.point(6);
		bb->GrControl5 = gr.point(7);
		bb->updateGradientVectors();
	}
	bb = itemSelection->itemAt(0);
	GroupOnPage(bb);
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	rotMode = drm;
	if ((rotMode != 0) && (!isLoading()))
	{
		switch (rotMode)
		{
		case 2:
			moveGroup((origGW-gw) / 2.0, (origGH-gh) / 2.0, true);
			break;
		case 4:
			moveGroup(origGW-gw, origGH-gh, true);
			break;
		case 3:
			moveGroup(0.0, origGH-gh, true);
			break;
		case 1:
			moveGroup(origGW-gw, 0.0, true);
			break;
		}
	}
//	gx -= minCanvasCoordinate.x();
//	gy -= minCanvasCoordinate.y();
	updateManager()->setUpdatesEnabled();
	regionsChanged()->update(QRectF(gx-5, gy-5, gw+10, gh+10).unite(oldR));
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		PageItem *currItem = itemSelection->itemAt(a);
		currItem->gXpos = currItem->xPos() - gx;
		currItem->gYpos = currItem->yPos() - gy;
		currItem->gWidth = gw;
		currItem->gHeight = gh;
	}
	// FIXME:av emit DocChanged();
}

PageItem* ScribusDoc::groupObjectsSelection(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() < 1)
		return NULL;
	int objectsLayer = itemSelection->objectsLayer();
	if (objectsLayer == -1)
		return NULL;
	PageItem *currItem;
	double x, y, w, h;
	uint selectedItemCount = itemSelection->count();
	itemSelection->getVisualGroupRect(&x, &y, &w, &h);
	uint lowestItem = 999999;
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		currItem->gXpos = currItem->xPos() - x;
		currItem->gYpos = currItem->yPos() - y;
		currItem->gWidth = w;
		currItem->gHeight = h;
		lowestItem = qMin(lowestItem, currItem->ItemNr);
	}
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (uint ep = 0; ep < selectedItemCount; ++ep)
	{
		PageItem* currItem = itemSelection->itemAt(ep);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	double gx = minx;
	double gy = miny;
	double gw = maxx - minx;
	double gh = maxy - miny;
	int z = itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *groupItem = Items->takeAt(z);
	Items->insert(lowestItem, groupItem);
	groupItem->setItemName( tr("Group%1").arg(GroupCounter));
	groupItem->AutoName = false;
	groupItem->groupWidth = gw;
	groupItem->groupHeight = gh;
	groupItem->LayerID = objectsLayer;
	for (uint c = 0; c < selectedItemCount; ++c)
	{
		currItem = itemSelection->itemAt(c);
		int d = Items->indexOf(currItem);
		groupItem->groupItemList.append(Items->takeAt(d));
	}
	renumberItemsInListOrder();
	itemSelection->clear();
	itemSelection->addItem(groupItem);
	GroupCounter++;
	return groupItem;
}

PageItem* ScribusDoc::groupObjectsList(QList<PageItem*> &itemList)
{
	if (itemList.count() < 1)
		return NULL;
	PageItem *currItem;
	uint selectedItemCount = itemList.count();
	uint lowestItem = 999999;
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		currItem = itemList.at(a);
		lowestItem = qMin(lowestItem, currItem->ItemNr);
	}
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (uint ep = 0; ep < selectedItemCount; ++ep)
	{
		currItem = itemList.at(ep);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	double gx = minx;
	double gy = miny;
	double gw = maxx - minx;
	double gh = maxy - miny;
	int z = itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *groupItem = Items->takeAt(z);
	Items->insert(lowestItem, groupItem);
	groupItem->setItemName( tr("Group%1").arg(GroupCounter));
	groupItem->AutoName = false;
	groupItem->groupWidth = gw;
	groupItem->groupHeight = gh;
	for (uint c = 0; c < selectedItemCount; ++c)
	{
		currItem = itemList.at(c);
		int d = Items->indexOf(currItem);
		groupItem->groupItemList.append(Items->takeAt(d));
		currItem->gXpos = currItem->xPos() - minx;
		currItem->gYpos = currItem->yPos() - miny;
		currItem->gWidth = maxx - minx;
		currItem->gHeight = maxy - miny;
	}
	renumberItemsInListOrder();
	GroupCounter++;
	itemList.clear();
	itemList.append(groupItem);
	return groupItem;
}

void ScribusDoc::groupObjectsToItem(PageItem* groupItem, QList<PageItem*> &itemList)
{
	if (itemList.count() < 1)
		return;
	PageItem *currItem;
	uint selectedItemCount = itemList.count();
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (uint ep = 0; ep < selectedItemCount; ++ep)
	{
		currItem = itemList.at(ep);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	groupItem->groupWidth = groupItem->width();
	groupItem->groupHeight = groupItem->height();
	for (uint c = 0; c < selectedItemCount; ++c)
	{
		currItem = itemList.at(c);
		int d = Items->indexOf(currItem);
		groupItem->groupItemList.append(Items->takeAt(d));
		currItem->gXpos = currItem->xPos() - groupItem->xPos();
		currItem->gYpos = currItem->yPos() - groupItem->yPos();
		currItem->gWidth = maxx - minx;
		currItem->gHeight = maxy - miny;
	}
	renumberItemsInListOrder();
	GroupCounter++;
	itemList.clear();
	itemList.append(groupItem);
}

const PageItem * ScribusDoc::itemSelection_GroupObjects(bool changeLock, bool lock, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() < 1)
		return NULL;
	int objectsLayer = itemSelection->objectsLayer();
	if (objectsLayer == -1)
		return NULL;
	PageItem *currItem;
	PageItem* bb;
	double x, y, w, h;
	uint selectedItemCount = itemSelection->count();
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	if (changeLock)
	{
		uint lockedCount=0;
		for (uint a=0; a<selectedItemCount; ++a)
		{
			if (itemSelection->itemAt(a)->locked())
				++lockedCount;
		}
		if (lockedCount!=0 && lockedCount!=selectedItemCount)
		{
			for (uint a=0; a<selectedItemCount; ++a)
			{
				currItem = itemSelection->itemAt(a);
				if (currItem->locked())
				{
					for (uint c=0; c < selectedItemCount; ++c)
					{
						bb = itemSelection->itemAt(c);
						bb->setLocked(lock);
						if (m_ScMW && ScCore->usingGUI())
							m_ScMW->scrActions["itemLock"]->setChecked(lock);
						if (selectedItemCount <= Um::ItemsInvolvedLimit)
							tooltip += "\t" + currItem->getUName() + "\n";
					}
				}
			}
		}
	}
	itemSelection->getVisualGroupRect(&x, &y, &w, &h);
	uint lowestItem = 999999;
	for (uint a=0; a<selectedItemCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		currItem->gXpos = currItem->xPos() - x;
		currItem->gYpos = currItem->yPos() - y;
		currItem->gWidth = w;
		currItem->gHeight = h;
		lowestItem = qMin(lowestItem, currItem->ItemNr);
	}
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (uint ep = 0; ep < selectedItemCount; ++ep)
	{
		PageItem* currItem = itemSelection->itemAt(ep);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		}
	double gx = minx;
	double gy = miny;
	double gw = maxx - minx;
	double gh = maxy - miny;
	bool wasUndo = UndoManager::undoEnabled();
	undoManager->setUndoEnabled(false);
	int z = itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *groupItem = Items->takeAt(z);
	Items->insert(lowestItem, groupItem);
	groupItem->setItemName( tr("Group%1").arg(GroupCounter));
	groupItem->AutoName = false;
	groupItem->groupWidth = gw;
	groupItem->groupHeight = gh;
	groupItem->LayerID = objectsLayer;
	undoManager->setUndoEnabled(wasUndo);
	for (uint c = 0; c < selectedItemCount; ++c)
	{
		currItem = itemSelection->itemAt(c);
		int d = Items->indexOf(currItem);
		groupItem->groupItemList.append(Items->takeAt(d));
	}
	renumberItemsInListOrder();
	itemSelection->clear();
	itemSelection->addItem(groupItem);
	selectedItemCount = itemSelection->count();
	SimpleState *ss = new SimpleState(Um::Group, tooltip);
	ss->set("GROUP", "group");
	ss->set("itemcount", selectedItemCount);
	for (uint a=0; a<selectedItemCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		ss->set(QString("item%1").arg(a), currItem->uniqueNr);
	}
	GroupCounter++;
	regionsChanged()->update(QRectF(gx-5, gy-5, gw+10, gh+10));
	emit docChanged();
	if (m_ScMW && ScCore->usingGUI())
	{
		m_ScMW->scrActions["itemAttachTextToPath"]->setEnabled(false);
		m_ScMW->scrActions["itemGroup"]->setEnabled(false);
		m_ScMW->scrActions["itemUngroup"]->setEnabled(true);
	}
	undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
	return groupItem;
}

void ScribusDoc::itemSelection_UnGroupObjects(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() != 0)
	{
		uint docSelectionCount = itemSelection->count();
		PageItem *currItem;
		QList<PageItem*> toDelete;
		for (uint a=0; a < docSelectionCount; ++a)
		{
			currItem = itemSelection->itemAt(a);
			if (currItem->isGroup())
				toDelete.append(currItem);
		}
		bool wasLoad = isLoading();
		// Remove group control objects
		setLoading(true);
		itemSelection->delaySignalsOn();
		for (int b = 0; b < toDelete.count(); b++)
		{
			currItem = toDelete.at(b);
			int d = Items->indexOf(currItem);
			Items->removeAt(d);
			itemSelection->removeItem(currItem);
			Selection tempSelection(this, false);
			tempSelection.delaySignalsOn();
			int gcount = currItem->groupItemList.count() - 1;
			for (int c = gcount; c >= 0; c--)
			{
				PageItem* gItem = currItem->groupItemList.at(c);
				gItem->setXYPos(currItem->xPos() + gItem->gXpos, currItem->yPos() + gItem->gYpos, true);
				Items->insert(d, gItem);
				itemSelection->addItem(currItem->groupItemList.at(gcount - c));
				tempSelection.addItem(currItem->groupItemList.at(gcount - c));
			}
			if ((currItem->width() != currItem->groupWidth) || (currItem->height() != currItem->groupHeight))
				scaleGroup(currItem->width() / currItem->groupWidth, currItem->height() / currItem->groupHeight, true, &tempSelection, true);
			QTransform ma;
			ma.translate(currItem->xPos(), currItem->yPos());
			ma.rotate(currItem->rotation());
			FPoint n;
			for (int a = 0; a < tempSelection.count(); ++a)
			{
				PageItem* rItem = tempSelection.itemAt(a);
				n = FPoint(rItem->xPos() - currItem->xPos(), rItem->yPos() - currItem->yPos());
				rItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
				rItem->rotateBy(currItem->rotation());
				setRedrawBounding(rItem);
			}
			tempSelection.clear();
			tempSelection.delaySignalsOff();
			renumberItemsInListOrder();
		}
		setLoading(wasLoad);
		itemSelection->delaySignalsOff();

		// Delete items after delaySignalsOff() call so that palette are updated before item deletion
		for (int b = 0; b < toDelete.count(); b++)
			delete currItem;

		// Create undo actions
/*		UndoTransaction* undoTransaction = NULL;
		if (UndoManager::undoEnabled() && toDelete.count() > 1)
		{
			undoTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Ungroup, "", Um::IGroup));
		}
		QMap<int, QList<PageItem*> >::iterator groupIt;
		for (it = toDelete.begin(); it != toDelete.end(); ++it)
		{
//			PageItem* groupItem = it.key();
			int groupId = it.value();
			groupIt = groupObjects.find(groupId);
			if (groupIt == groupObjects.end()) 
				continue;
			QList<PageItem*> groupItems = groupIt.value();

			docSelectionCount = groupItems.count();
			SimpleState *ss = new SimpleState(Um::Ungroup);
			ss->set("UNGROUP", "ungroup");
			ss->set("itemcount", docSelectionCount);
			QString tooltip = Um::ItemsInvolved + "\n";
			if (docSelectionCount > Um::ItemsInvolvedLimit)
				tooltip = Um::ItemsInvolved2 + "\n";
			for (uint a=0; a < docSelectionCount; ++a)
			{
				currItem = groupItems.at(a);
				ss->set(QString("item%1").arg(a), currItem->uniqueNr);
				ss->set(QString("tableitem%1").arg(a), currItem->isTableItem);
				if (docSelectionCount <= Um::ItemsInvolvedLimit)
					tooltip += "\t" + currItem->getUName() + "\n";
				currItem->isTableItem = false;
				currItem->setSelected(true);
			}
			undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
		}
		if (undoTransaction)
		{
			undoTransaction->commit();
			delete undoTransaction;
			undoTransaction = NULL;
		} */
		double x, y, w, h;
		itemSelection->connectItemToGUI();
		itemSelection->getGroupRect(&x, &y, &w, &h);
		emit docChanged();
		m_ScMW->HaveNewSel(itemSelection->itemAt(0)->itemType());
		regionsChanged()->update(QRectF(x-5, y-5, w+10, h+10));
	}
}

void ScribusDoc::itemSelection_UniteItems(Selection* /*customSelection*/)
{
	PageItem *currItem;
	PageItem *bb;
	QList<int> toDel;
	toDel.clear();
	uint docSelectionCount = m_Selection->count();
	if (docSelectionCount > 1)
	{
		currItem = m_Selection->itemAt(0);
		if (currItem->isGroup())
			return;
		m_Selection->delaySignalsOn();
		currItem->Frame = false;
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		for (uint a = 1; a < docSelectionCount; ++a)
		{
			bb = m_Selection->itemAt(a);
			toDel.append(bb->ItemNr);
			QTransform ma;
			ma.translate(bb->xPos(), bb->yPos());
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QTransform ma2;
			ma2.translate(currItem->xPos(), currItem->yPos());
			ma2.rotate(currItem->rotation());
			ma2 = ma2.inverted();
			bb->PoLine.map(ma2);
			currItem->PoLine.setMarker();
			currItem->PoLine.putPoints(currItem->PoLine.size(), bb->PoLine.size(), bb->PoLine);
		}
//		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
		AdjustItemSize(currItem);
		currItem->ContourLine = currItem->PoLine.copy();
		//FIXME: stop using m_View
		m_View->Deselect(true);
		for (int c = 0; c < toDel.count(); ++c)
			m_View->SelectItemNr(toDel.at(c));
		m_Selection->delaySignalsOff();
		itemSelection_DeleteItem();
		regionsChanged()->update(QRectF());
	}
}

void ScribusDoc::itemSelection_SplitItems(Selection* /*customSelection*/)
{
	PageItem *bb;
	uint StartInd = 0;
	m_Selection->delaySignalsOn();
	PageItem *currItem = m_Selection->itemAt(0);
	int currItemNr = currItem->ItemNr;
	uint EndInd = currItem->PoLine.size();
	for (uint a = EndInd-1; a > 0; --a)
	{
		if (currItem->PoLine.point(a).x() > 900000)
		{
			StartInd = a + 1;
			bb = new PageItem_Polygon(*currItem);
			currItemNr++;
			Items->insert(currItemNr, bb);
			bb->ItemNr = currItemNr;
			bb->convertTo(PageItem::Polygon);
			bb->Frame = false;
			bb->FrameType = 3;
			bb->PoLine.resize(0);
			bb->PoLine.putPoints(0, EndInd - StartInd, currItem->PoLine, StartInd);
			bb->setRotation(currItem->rotation());
			AdjustItemSize(bb);
			bb->ContourLine = bb->PoLine.copy();
			bb->ClipEdited = true;
			m_Selection->addItem(bb, false);
			a -= 3;
			EndInd = StartInd - 4;
		}
	}
	currItem->PoLine.resize(StartInd-4);
	AdjustItemSize(currItem);
	currItem->ContourLine = currItem->PoLine.copy();
	currItem->ClipEdited = true;
	m_Selection->delaySignalsOff();
	renumberItemsInListOrder();
	//FIXME: stop using m_View
	m_View->Deselect(true);
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_convertItemsTo(const PageItem::ItemType newType, Selection* restoredSelection, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//Create our copy selection as our item *s will be invalidated as we go through the loop and the selection index wont work
	//convertItemTo does this
	Selection tmpSel(*itemSelection);
	tmpSel.disconnectAllItemsFromGUI();
	tmpSel.delaySignalsOn();
	m_updateManager.setUpdatesDisabled();
	UndoTransaction* convertTransaction = NULL;
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		convertTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ConvertTo, "", Um::IGroup));

	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem)
		{
			if (currItem->isGroup())  // don't convert group control items.
				restoredSelection->addItem(currItem);
			else
			{
				PageItem* newItem=convertItemTo(currItem, newType);
				if (newItem)
				{
					newItem->update();
					if (restoredSelection)
						restoredSelection->addItem(newItem);
				}
			}
		}
	}
	if (convertTransaction)
		convertTransaction->commit();
	tmpSel.delaySignalsOff();
	m_updateManager.setUpdatesEnabled();
	changed();
}

//CB-->Doc
//Fix size/move item calls
//CB TODO Use the selection loop properly
void ScribusDoc::itemSelection_AdjustFrametoImageSize( Selection *customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	if (selectedItemCount > 0)
	{
		UndoTransaction* activeTransaction = NULL;
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = itemSelection->itemAt(i);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame() && currItem->PictureIsAvailable && !currItem->isTableItem)
				{
					if (!activeTransaction)
						activeTransaction = new UndoTransaction(undoManager->beginTransaction(selectedItemCount == 1 ?
																							  currItem->getUName() : Um::SelectionGroup,
																							  selectedItemCount == 1 ?
																							  currItem->getUPixmap() : Um::IGroup,
																							  Um::AdjustFrameToImage,"",Um::IResize));
					double w, h, x, y;
					w = currItem->OrigW * currItem->imageXScale();
					h = currItem->OrigH * currItem->imageYScale();
					x = currItem->imageXOffset() * currItem->imageXScale();
					y = currItem->imageYOffset() * currItem->imageYScale();
					if ((x != 0.0) || (y != 0.0)) // if the image frame has an offset, its assumed that the user wants the image to stay where it is
					{
						SizeItem(w, h, currItem->ItemNr);
						MoveItem(x, y, currItem);
					}
					else
						SizeItem(w, h, currItem->ItemNr, true);
					currItem->setImageXYOffset(0.0, 0.0);
				}
			}
		}
		if (activeTransaction)
		{
			activeTransaction->commit();
			delete activeTransaction;
			activeTransaction = NULL;
			regionsChanged()->update(QRectF());
			changed();
			itemSelection->itemAt(0)->emitAllToGUI();
		}
	}
}
void ScribusDoc::itemSelection_AdjustImagetoFrameSize( Selection *customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	if (selectedItemCount > 0)
	{
		for (uint i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = itemSelection->itemAt(i);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame() && currItem->PictureIsAvailable && !currItem->isTableItem)
					currItem->setImageScalingMode(false, true);
			}
		}
		regionsChanged()->update(QRectF());
		changed();
		itemSelection->itemAt(0)->emitAllToGUI();
	}
}



NodeEditContext::NodeEditContext() 
: submode(MOVE_POINT), isContourLine(false), oldClip(NULL), nodeTransaction(NULL),
ClRe(-1), ClRe2(-1), SegP1(-1), SegP2(-1), EdPoints(true), MoveSym(false), SelNode() 
{
}

bool NodeEditContext::hasNodeSelected() 
{ 
	return ClRe != -1; 
}


void NodeEditContext::deselect() 
{ 
	ClRe = -1; 
}

void NodeEditContext::reset()
{
	submode = MOVE_POINT;
	isContourLine = false;
	ClRe = -1;
	ClRe2 = -1;
	SegP1 = -1;
	SegP2 = -1;
	delete oldClip;
	oldClip = NULL;
	delete nodeTransaction;
	nodeTransaction = NULL;
	MoveSym = false;
	SelNode.clear();
}



FPointArray NodeEditContext::beginTransaction(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	
	FPointArray Clip;
	QString uAction;
	if (Doc->nodeEdit.isContourLine)
	{
		Clip = currItem->ContourLine;
		//					isContourLine = true;
		uAction = Um::EditContour;
	}
	else
	{
		Clip = currItem->PoLine;
		//					isContourLine = false;
		uAction = Um::EditShape;
	}
	oldClip = new FPointArray(Clip);
	oldItemX = currItem->xPos();
	oldItemY = currItem->yPos();
	nodeTransaction = new UndoTransaction(undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), uAction));
	Doc->nodeEdit.deselect();
	return Clip;
}



void NodeEditContext::finishTransaction(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	
	if (nodeTransaction) // is there the old clip stored for the undo action
	{
		FPointArray newClip(Doc->nodeEdit.isContourLine ? currItem->ContourLine : currItem->PoLine);
		if (*oldClip != newClip)
		{
			QString name = Doc->nodeEdit.isContourLine ? Um::EditContour : Um::EditShape;
			ItemState<QPair<FPointArray, FPointArray> > *state =
				new ItemState<QPair<FPointArray, FPointArray> >(name);
			state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
			state->set("IS_CONTOUR", Doc->nodeEdit.isContourLine);
			state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
			state->set("OLD_X", oldItemX);
			state->set("OLD_Y", oldItemY);
			state->set("NEW_X", currItem->xPos());
			state->set("NEW_Y", currItem->yPos());
			undoManager->action(currItem, state);
			nodeTransaction->commit();
		}
		else
			nodeTransaction->cancel();
		
		delete nodeTransaction;
		nodeTransaction = NULL;
		delete oldClip;
		oldClip = 0;
	}
}


/**
  first part: create a new UndoState or cancel the current transaction
 */
ItemState<QPair<FPointArray, FPointArray> >* NodeEditContext::finishTransaction1(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	ItemState<QPair<FPointArray, FPointArray> >* state = NULL;
	
	if (nodeTransaction) // is there the old clip stored for the undo action
	{
		FPointArray newClip(Doc->nodeEdit.isContourLine ? currItem->ContourLine : currItem->PoLine);
		if (*oldClip != newClip)
		{
			QString name = Doc->nodeEdit.isContourLine ? Um::EditContour : Um::EditShape;
			state = new ItemState<QPair<FPointArray, FPointArray> >(name);
			state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
			state->set("IS_CONTOUR", Doc->nodeEdit.isContourLine);
			state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
			undoManager->setUndoEnabled(false);
		}
		else
		{
			delete oldClip;
			oldClip = 0;
			nodeTransaction->cancel();
			delete nodeTransaction;
			nodeTransaction = NULL;
		}
	}
	return state;
}

/**
  second part: take the UndoState returned from finishTransaction1() and commit it
*/
void NodeEditContext::finishTransaction2(PageItem* currItem, ItemState<QPair<FPointArray, FPointArray> >* state)
{
	UndoManager* undoManager = UndoManager::instance();
	
	state->set("OLD_X", oldItemX);
	state->set("OLD_Y", oldItemY);
	state->set("NEW_X", currItem->xPos());
	state->set("NEW_Y", currItem->yPos());
	undoManager->setUndoEnabled(true);
	undoManager->action(currItem, state);
	nodeTransaction->commit();
	delete nodeTransaction;
	nodeTransaction = NULL;
	delete oldClip;
	oldClip = 0;				
}	
	

//CB-->Doc
void NodeEditContext::moveClipPoint(PageItem *currItem, FPoint ip)
{
	ScribusDoc* Doc = currItem->doc();
	if (((EdPoints) && (ClRe % 2 != 0)) || ((!EdPoints) && (ClRe % 2 == 0)))
		return;
	double xposOrig = currItem->xPos();
	double yposOrig = currItem->yPos();
	currItem->ClipEdited = true;
	FPointArray Clip;
	if (isContourLine)
	{
		Clip = currItem->ContourLine.copy();
	}
	else
	{
		Clip = currItem->PoLine.copy();
	}
	currItem->FrameType = 3;
	uint EndInd = Clip.size();
	uint StartInd = 0;
	for (uint n = ClRe; n < Clip.size(); ++n)
	{
		if (Clip.point(n).x() > 900000)
		{
			EndInd = n;
			break;
		}
	}
	if (ClRe > 0)
	{
		for (uint n2 = ClRe; n2 > 0; n2--)
		{
			if (n2 == 0)
				break;
			if (Clip.point(n2).x() > 900000)
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}
	FPoint np(ip);
	if (hasNodeSelected())
	{
		if ((np.x() < 0) && (!isContourLine) && (!(currItem->isGroup() || currItem->isSymbol())))
		{
			Doc->SizeItem(currItem->width() - np.x(), currItem->height(), currItem->ItemNr, false, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(np.x(), 0);
				Doc->MoveRotated(currItem, npv);
			}
			else
				Doc->MoveItem(np.x(), 0, currItem);
			Clip.translate(-np.x(), 0);
			if (!currItem->imageFlippedH())
				currItem->moveImageInFrame(-np.x()/currItem->imageXScale(), 0);
			np.setX(0);
		}
		if ((np.y() < 0) && (!isContourLine) && (!(currItem->isGroup() || currItem->isSymbol())))
		{
			Doc->SizeItem(currItem->width(), currItem->height() - np.y(), currItem->ItemNr, false, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(0, np.y());
				Doc->MoveRotated(currItem, npv);
			}
			else
				Doc->MoveItem(0, np.y(), currItem);
			Clip.translate(0, -np.y());
			if (!currItem->imageFlippedV())
				currItem->moveImageInFrame(0, -np.y()/currItem->imageYScale());
			np.setY(0);
		}
		update(QPointF(np.x(), np.y()));
		if ((ClRe+1 < static_cast<int>(EndInd)) && (ClRe % 2 == 0))
		{
			FPoint ap(Clip.point(ClRe));
			FPoint ap2(Clip.point(ClRe+1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+1, ap2);
		}
		Clip.setPoint(ClRe, np);
		if (((ClRe % 4 != 0) && (ClRe % 2 == 0)) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(ClRe+2));
			FPoint ap2(Clip.point(ClRe+3));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+3, ap2);
			Clip.setPoint(ClRe+2, np);
		}
		if ((ClRe % 4 == 0) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(ClRe-2));
			FPoint ap2(Clip.point(ClRe-1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe-1, ap2);
			Clip.setPoint(ClRe-2, np);
		}
		if (((ClRe == static_cast<int>(StartInd)) || (ClRe == static_cast<int>(EndInd-2))) &&
			((currItem->itemType() == PageItem::Polygon)
			|| (currItem->itemType() == PageItem::Group)
			|| (currItem->itemType() == PageItem::Symbol)
			|| (currItem->itemType() == PageItem::TextFrame)
			|| (currItem->itemType() == PageItem::ImageFrame)))
		{
			if (ClRe == static_cast<int>(StartInd))
			{
				FPoint ap(Clip.point(EndInd-2));
				FPoint ap2(Clip.point(EndInd-1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(EndInd-2, Clip.point(StartInd));
				Clip.setPoint(EndInd-1, ap2);
			}
			else
			{
				FPoint ap(Clip.point(StartInd));
				FPoint ap2(Clip.point(StartInd + 1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(StartInd, Clip.point(EndInd-2));
				Clip.setPoint(StartInd + 1, ap2);
			}
		}
		if (((ClRe == static_cast<int>(StartInd+1)) || (ClRe == static_cast<int>(EndInd-1))) &&
			((currItem->itemType() == PageItem::Polygon)
			|| (currItem->itemType() == PageItem::Group)
			|| (currItem->itemType() == PageItem::Symbol)
			|| (currItem->itemType() == PageItem::TextFrame)
			|| (currItem->itemType() == PageItem::ImageFrame)) && 
			(MoveSym))
		{
			uint kon = 0;
			if (ClRe == static_cast<int>(StartInd+1))
				kon = EndInd-1;
			else
				kon = StartInd + 1;
			FPoint lxy(Clip.point(ClRe-1));
			FPoint lk(Clip.point(ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if ((ClRe % 2 != 0) && (MoveSym) && (ClRe != static_cast<int>(StartInd + 1)) && (ClRe != static_cast<int>(EndInd-1)))
		{
			uint kon = 0;
			if ((ClRe+1) % 4 == 0)
				kon = ClRe + 2;
			else
				kon = ClRe - 2;
			FPoint lxy(Clip.point(ClRe-1));
			FPoint lk(Clip.point(ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if (Doc->nodeEdit.isContourLine)
			currItem->ContourLine = Clip.copy();
		else
			currItem->PoLine = Clip.copy();
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	}
	if (!isContourLine)
		currItem->ContourLine.translate(xposOrig - currItem->xPos(), yposOrig - currItem->yPos());
}


void NodeEditContext::reset1Control(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	if (ClRe % 2 == 0)
		return;
	UndoManager* undoManager = UndoManager::instance();

	// do no record anything else but the core reset points action
	undoManager->setUndoEnabled(false);
	
	oldClip = new FPointArray(Doc->nodeEdit.isContourLine ? currItem->ContourLine.copy() : currItem->PoLine.copy());
	oldItemX = currItem->xPos();
	oldItemY = currItem->yPos();
	currItem->ClipEdited = true;
	FPoint np;
	if (Doc->nodeEdit.isContourLine)
		np = currItem->ContourLine.point(Doc->nodeEdit.ClRe-1);
	else
		np = currItem->PoLine.point(Doc->nodeEdit.ClRe-1);
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if (Doc->nodeEdit.isContourLine)
	{
		currItem->ContourLine.setPoint(Doc->nodeEdit.ClRe, np);
		Doc->regionsChanged()->update(QRectF());
		currItem->FrameOnly = true;
		currItem->update();
	}
	else
	{
		currItem->PoLine.setPoint(Doc->nodeEdit.ClRe, np);
		if (!(currItem->isGroup() || currItem->isSymbol()))
			Doc->AdjustItemSize(currItem);
		Doc->regionsChanged()->update(QRectF());
	}
	undoManager->setUndoEnabled(true);
	FPointArray newClip(Doc->nodeEdit.isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (*oldClip != newClip)
	{
		ItemState<QPair<FPointArray, FPointArray> > *state =
		new ItemState<QPair<FPointArray, FPointArray> >(Um::ResetControlPoint, "",
														currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
		state->set("IS_CONTOUR", Doc->nodeEdit.isContourLine);
		state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
		state->set("OLD_X", oldItemX);
		state->set("OLD_Y", oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = 0;
}	


void NodeEditContext::resetControl(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	
	// do no record anything else but the core reset points action
	undoManager->setUndoEnabled(false);
	
	oldClip = new FPointArray(Doc->nodeEdit.isContourLine ? currItem->ContourLine.copy() : currItem->PoLine.copy());
	oldItemX = currItem->xPos();
	oldItemY = currItem->yPos();
	currItem->ClipEdited = true;
	FPointArray Clip;
	if (Doc->nodeEdit.isContourLine)
		Clip = currItem->ContourLine.copy();
	else
		Clip = currItem->PoLine.copy();
	int EndInd = Clip.size();
	int StartInd = 0;
	for (uint n = ClRe; n < Clip.size(); ++n)
	{
		if (Clip.point(n).x() > 900000)
		{
			EndInd = n;
			break;
		}
	}
	if (ClRe > 0)
	{
		for (uint n2 = ClRe; n2 > 0; n2--)
		{
			if (n2 == 0)
				break;
			if (Clip.point(n2).x() > 900000)
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}
	FPoint np = Clip.point(Doc->nodeEdit.ClRe);
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if ((Doc->nodeEdit.ClRe == StartInd) || (Doc->nodeEdit.ClRe == EndInd-2))
	{
		if (currItem->asPolyLine())
		{
			if (Doc->nodeEdit.ClRe == StartInd)
				Clip.setPoint(StartInd+1, np);
			else
				Clip.setPoint(EndInd-1, np);
		}
		else
		{
			Clip.setPoint(StartInd+1, np);
			Clip.setPoint(EndInd-1, np);
		}
	}
	else
	{
		Clip.setPoint(Doc->nodeEdit.ClRe+1, np);
		Clip.setPoint((Doc->nodeEdit.ClRe % 4 != 0 ? Doc->nodeEdit.ClRe + 3 : Doc->nodeEdit.ClRe - 1), np);
	}
	if (!Doc->nodeEdit.isContourLine)
	{
		currItem->PoLine = Clip.copy();
		if (!(currItem->isGroup() || currItem->isSymbol()))
			Doc->AdjustItemSize(currItem);
		Doc->regionsChanged()->update(QRectF());
	}
	else
	{
		currItem->ContourLine = Clip.copy();
		Doc->regionsChanged()->update(QRectF());
	}
	Doc->update();
	
	undoManager->setUndoEnabled(true);
	FPointArray newClip(Doc->nodeEdit.isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (*oldClip != newClip)
	{
		ItemState<QPair<FPointArray, FPointArray> > *state =
		new ItemState<QPair<FPointArray, FPointArray> >(Um::ResetControlPoints, "",
														currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
		state->set("IS_CONTOUR", Doc->nodeEdit.isContourLine);
		state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
		state->set("OLD_X", oldItemX);
		state->set("OLD_Y", oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = 0;
	
}

void ScribusDoc::itemSelection_SetColorProfile(const QString & profileName, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	m_updateManager.setUpdatesDisabled();
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			currItem->IProfile = profileName;
			currItem->UseEmbedded = profileName.startsWith("Embedded");
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->update();
		}
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetRenderIntent(int intentIndex, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	m_updateManager.setUpdatesDisabled();
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			currItem->IRender = (eRenderIntent) intentIndex;
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->update();
		}
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}
 
void ScribusDoc::itemSelection_SetCompressionMethod(int cmIndex, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			if ((currItem->OverrideCompressionMethod = cmIndex >= 0))
			{
				currItem->CompressionMethodIndex = cmIndex;
			}
		}
	}
}

void ScribusDoc::itemSelection_SetCompressionQuality(int cqIndex, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			if ((currItem->OverrideCompressionQuality = cqIndex >= 0))
			{
				currItem->CompressionQualityIndex = cqIndex;
			}
		}
	}
}

QMap<PageItem*, QString> ScribusDoc::getDocItemNames(PageItem::ItemType itemType)
{
	QMap<PageItem*, QString> namesMap;
	QList<PageItem*> allItems;
	uint docItemsCount = DocItems.count();
	for (uint i = 0; i < docItemsCount; ++i)
	{
		PageItem* ite = DocItems.at(i);
		if (ite->isGroup())
			allItems = ite->getItemList();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			if (ite->itemType() == itemType && ite->nextInChain()==NULL && !ite->isAutoFrame())
				namesMap.insert(ite, ite->itemName());
		}
		allItems.clear();
	}
	return namesMap;
}

Serializer *ScribusDoc::serializer()
{
	if (!m_serializer) 
		m_serializer = new Serializer (*this);
	Q_ASSERT(m_serializer);
	return m_serializer;
}


void ScribusDoc::RotMode(const int& val)
{
	rotMode = val;
	emit rotationMode(rotMode);
}

void ScribusDoc::setPageSetFirstPage(int layout, int fp)
{
	docPrefsData.pageSets[layout].FirstPage=fp;
}

void ScribusDoc::setNewPrefs(const ApplicationPrefs& prefsData, const ApplicationPrefs& oldPrefsData, bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins)
{
	docPrefsData=prefsData;
	double topDisplacement = prefsData.displayPrefs.scratch.Top - oldPrefsData.displayPrefs.scratch.Top;
	double leftDisplacement = prefsData.displayPrefs.scratch.Left - oldPrefsData.displayPrefs.scratch.Left;
	applyPrefsPageSizingAndMargins(resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins);

	uint docItemsCount = MasterItems.count();
	for (uint ite = 0; ite < docItemsCount; ++ite)
	{
		PageItem *item = MasterItems.at(ite);
		item->moveBy(leftDisplacement, topDisplacement);
		item->setRedrawBounding();
	}

//	bool viewToRecalcPictureRes = (docPrefsData.itemToolPrefs.imageLowResType==oldPrefsData.itemToolPrefs.imageLowResType);

	autoSaveTimer->stop();
	if (docPrefsData.docSetupPrefs.AutoSave)
		autoSaveTimer->start(docPrefsData.docSetupPrefs.AutoSaveTime);

/*	FIXME: scribus determines dict by charstyle now, so this setting should go into the doc's default charstyle
		currDoc->docHyphenator->slotNewDict(ScMW->GetLang(tabHyphenator->language->currentText()));
*/

	docHyphenator->slotNewSettings(docPrefsData.hyphPrefs.MinWordLen,
											!docPrefsData.hyphPrefs.Automatic,
											docPrefsData.hyphPrefs.AutoCheck,
											docPrefsData.hyphPrefs.HyCount);
	docHyphenator->ignoredWords = docPrefsData.hyphPrefs.ignoredWords;
	docHyphenator->specialWords = docPrefsData.hyphPrefs.specialWords;
	if (ScCore->haveCMS())
	{
		bool cmsChanged = (
					(docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile != oldPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile) ||
					(docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile != oldPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile) ||
					(docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile != oldPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile) ||
					(docPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile != oldPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile) ||
					(docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile != oldPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile) ||
					(docPrefsData.colorPrefs.DCMSset.DefaultIntentColors != oldPrefsData.colorPrefs.DCMSset.DefaultIntentColors) ||
					(docPrefsData.colorPrefs.DCMSset.DefaultIntentImages != oldPrefsData.colorPrefs.DCMSset.DefaultIntentImages) ||
					(docPrefsData.colorPrefs.DCMSset.SoftProofOn != oldPrefsData.colorPrefs.DCMSset.SoftProofOn) ||
					(docPrefsData.colorPrefs.DCMSset.SoftProofFullOn != oldPrefsData.colorPrefs.DCMSset.SoftProofFullOn) ||
					(docPrefsData.colorPrefs.DCMSset.GamutCheck != oldPrefsData.colorPrefs.DCMSset.GamutCheck) ||
					(docPrefsData.colorPrefs.DCMSset.BlackPoint != oldPrefsData.colorPrefs.DCMSset.BlackPoint) ||
					(docPrefsData.colorPrefs.DCMSset.CMSinUse != oldPrefsData.colorPrefs.DCMSset.CMSinUse)
					);
		if (cmsChanged)
		{
			m_ScMW->setStatusBarInfoText( tr("Adjusting Colors"));
			m_ScMW->mainWindowProgressBar->reset();
			int cc = PageColors.count() + Items->count();
			m_ScMW->mainWindowProgressBar->setMaximum(cc);
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
			bool newCM  = docPrefsData.colorPrefs.DCMSset.CMSinUse;
			bool updCol = false;
			docPrefsData.colorPrefs.DCMSset.CMSinUse = oldPrefsData.colorPrefs.DCMSset.CMSinUse;
			CloseCMSProfiles();
			docPrefsData.colorPrefs.DCMSset.CMSinUse = newCM;
			HasCMS = docPrefsData.colorPrefs.DCMSset.CMSinUse;
			SoftProofing = docPrefsData.colorPrefs.DCMSset.SoftProofOn;
			Gamut = docPrefsData.colorPrefs.DCMSset.GamutCheck;
			IntentColors = docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
			IntentImages = docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
			if (!docPrefsData.colorPrefs.DCMSset.CMSinUse)
			{
				HasCMS = false;
				if	(oldPrefsData.colorPrefs.DCMSset.CMSinUse)
				{
					SetDefaultCMSParams();
					updCol = true;
				}
			}
			else if ( OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles) )
			{
				HasCMS = true;
				docPrefsData.pdfPrefs.SComp = docPrefsData.colorPrefs.DCMSset.ComponentsInput2;
				docPrefsData.pdfPrefs.SolidProf = docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
				docPrefsData.pdfPrefs.ImageProf = docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
				docPrefsData.pdfPrefs.PrintProf = docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
				docPrefsData.pdfPrefs.Intent = docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
				updCol = true;
			}
			else
			{
				SetDefaultCMSParams();
				HasCMS = false;
			}
			if (updCol)
			{
				m_ScMW->recalcColors(m_ScMW->mainWindowProgressBar);
				RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, m_ScMW->mainWindowProgressBar);
			}
			m_ScMW->mainWindowProgressBar->setValue(cc);
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			m_ScMW->setStatusBarInfoText("");
			m_ScMW->mainWindowProgressBar->reset();
		}
	}
//	PrefsManager* prefsManager=PrefsManager::instance();

	QStringList uf(UsedFonts.keys());
	QMap<QString,int>::Iterator it3;
	UsedFonts.clear();
	QStringList::Iterator it3a;
	QStringList::Iterator it3aend=uf.end();
	for (it3a = uf.begin(); it3a != it3aend; ++it3a)
		AddFont(*it3a);

//	currDoc->documentInfo = docInfos->getDocInfo();
//	currDoc->sections = tabDocSections->getNewSections();

	uint itemCount=Items->count();
	for (uint b=0; b<itemCount; ++b)
	{
		if (Items->at(b)->itemType() == PageItem::ImageFrame)
			Items->at(b)->setImageShown(docPrefsData.guidesPrefs.showPic);
	}

	double oldBaseGridValue  = oldPrefsData.guidesPrefs.valueBaselineGrid;
	double oldBaseGridOffset = oldPrefsData.guidesPrefs.offsetBaselineGrid;
	if (oldBaseGridValue  != prefsData.guidesPrefs.valueBaselineGrid ||
		oldBaseGridOffset != prefsData.guidesPrefs.offsetBaselineGrid )
	{
		this->invalidateAll();
	}
}

void ScribusDoc::applyPrefsPageSizingAndMargins(bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins)
{
	for (int p = 0; p < Pages->count(); ++p)
	{
		Page *pp = Pages->at(p);
		if (resizePages)
		{
			pp->setInitialWidth(pageWidth());
			pp->setInitialHeight(pageHeight());
			pp->setHeight(pageHeight());
			pp->setWidth(pageWidth());
			pp->m_pageSize = pageSize();
			pp->setOrientation(pageOrientation());
		}
		if (resizePageMargins)
		{
			pp->initialMargins=docPrefsData.docSetupPrefs.margins;
			pp->marginPreset=docPrefsData.docSetupPrefs.marginPreset;
		}
		else
		if (resizeMasterPageMargins)
		{
			//CB #6796: find the master page (*mp) for the current page (*pp)
			//check if *pp's margins are the same as the *mp's current margins
			//apply new margins if same
			const int masterPageNumber = MasterNames[pp->MPageNam];
			const Page* mp = MasterPages.at(masterPageNumber);
			if (pp->initialMargins.Left == mp->initialMargins.Left &&
				pp->initialMargins.Right == mp->initialMargins.Right &&
				pp->initialMargins.Top == mp->initialMargins.Top &&
				pp->initialMargins.Bottom == mp->initialMargins.Bottom)
			{
				pp->initialMargins=docPrefsData.docSetupPrefs.margins;
				pp->marginPreset=docPrefsData.docSetupPrefs.marginPreset;
			}
		}
	}
	for (int p = 0; p < MasterPages.count(); ++p)
	{
		Page *pp = MasterPages.at(p);
		if (resizeMasterPages)
		{
			pp->setInitialWidth(pageWidth());
			pp->setInitialHeight(pageHeight());
			pp->setHeight(pageHeight());
			pp->setWidth(pageWidth());
			pp->m_pageSize = pageSize();
			pp->setOrientation(pageOrientation());
		}
		if (resizeMasterPageMargins)
		{
			pp->initialMargins=docPrefsData.docSetupPrefs.margins;
			pp->marginPreset=docPrefsData.docSetupPrefs.marginPreset;
		}
		pp->setXOffset(scratch()->Left);
		pp->setYOffset(scratch()->Top);
	}
}

