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


#include <utility>

#include <QByteArray>
// #include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QList>
#include <QTime>
#include <QPainter>
#include <QPixmap>
#include <QProgressBar>

#include "canvas.h"
#include "cmserrorhandling.h"
#include "commonstrings.h"
#include "fileloader.h"
#include "filewatcher.h"
#include "guidemanager.h"
#include "hruler.h"
#include "hyphenator.h"
#include "layers.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "pageitem_latexframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#include "pagepalette.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "resourcecollection.h"
#include "sccolorengine.h"
#include "scmessagebox.h"
#include "scpainter.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribuswin.h"
#include "selection.h"
#include "serializer.h"
#include "story.h"
#include "text/nlsconfig.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "util.h"
#include "util_cms.h"
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
public:
	DocUpdater(ScribusDoc* d) : doc(d) {}
	
	void changed(Page* pg)
	{
		QRectF pagebox(pg->xOffset(), pg->yOffset(), pg->width(), pg->height());
		doc->invalidateRegion(pagebox);
		doc->regionsChanged()->update(pagebox);
		doc->changed();
	}
	
	void changed(PageItem* it)
	{
		it->invalidateLayout();
		doc->regionsChanged()->update(it->getBoundingRect());
		doc->changed();
	}
};




ScribusDoc::ScribusDoc() : UndoObject( tr("Document")), Observable<ScribusDoc>(NULL),
	m_hasGUI(false),
	prefsData(PrefsManager::instance()->appPrefs),
	undoManager(UndoManager::instance()),
	loading(false),
	modified(false),
	ActiveLayer(0),
	docUnitIndex(prefsData.docUnitIndex),
	docUnitRatio(unitGetRatioFromIndex(docUnitIndex)),
	automaticTextFrames(0),
	m_masterPageMode(false),
	m_ScMW(0),
	m_View(0),
	m_guardedObject(this),
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGuides(false), GuideLock(false),
	scratch(prefsData.scratch),
	GapHorizontal(prefsData.GapHorizontal),
	GapVertical(prefsData.GapVertical),
// 	ScratchLeft(prefsData.ScratchLeft),
// 	ScratchRight(prefsData.ScratchRight),
// 	ScratchTop(prefsData.ScratchTop),
// 	ScratchBottom(prefsData.ScratchBottom),
	minCanvasCoordinate(FPoint(0, 0)),
	maxCanvasCoordinate(FPoint(scratch.Left + scratch.Right, scratch.Top + scratch.Bottom)),
	rulerXoffset(0.0), rulerYoffset(0.0),
	Pages(0), MasterPages(), DocPages(),
	MasterNames(),
	Items(0), MasterItems(), DocItems(), FrameItems(),
	m_Selection(new Selection(this, true)),
	pageWidth(0), pageHeight(0),
	pageSets(prefsData.pageSets),
	PageSp(1), PageSpa(0),
	currentPageLayout(0),
	PageOri(0), m_pageSize(0),
	FirstPnum(1),
	useRaster(false),
	PageColors(this, true),
	appMode(modeNormal),
	SubMode(-1),
	ShapeValues(0),
	ValCount(0),
	DocName( tr("Document")+"-"),
	UsedFonts(),
	AllFonts(&prefsData.AvailFonts),
	AObjects(),
	papColor(prefsData.DpapColor),
	CurrentSel(-1),
	nodeEdit(),
	typographicSettings(prefsData.typographicSettings),
	guidesSettings(prefsData.guidesSettings),
	toolSettings(prefsData.toolSettings),
	checkerProfiles(prefsData.checkerProfiles),
	curCheckProfile(prefsData.curCheckProfile),
	LastAuto(0), FirstAuto(0),
	DraggedElem(0),
	ElemToLink(0),
	DragElements(),
	docParagraphStyles(),
	docCharStyles(),
	Layers(),
	marginColored(prefsData.marginColored),
	GroupCounter(1),
	JavaScripts(),
	TotalItems(0),
	MinWordLen(prefsData.MinWordLen),
	HyCount(prefsData.HyCount),
	Language(prefsData.Language),
	Automatic(prefsData.Automatic),
	AutoCheck(prefsData.AutoCheck),
	PDF_Options(prefsData.PDF_Options),
	RePos(false),
	BookMarks(),
	OldBM(false),
	hasName(false),
	RotMode(0),
	AutoSave(prefsData.AutoSave),
	AutoSaveTime(prefsData.AutoSaveTime),
	autoSaveTimer(new QTimer(this)),
	MLineStyles(),
	arrowStyles(prefsData.arrowStyles),
	WinHan(0),
	DoDrawing(true),
	OpenNodes(),
	CurTimer(0),
	docLayerErrors(),
	docItemErrors(),
	masterItemErrors(),
	docItemAttributes(prefsData.defaultItemAttributes),
	docToCSetups(prefsData.defaultToCSetups),
	// sections
	symReturn(), symNewLine(), symTab(), symNonBreak(), symNewCol(), symNewFrame(),
	docHyphenator(0),
	m_itemCreationTransaction(NULL),
	m_alignTransaction(NULL),
	m_currentPage(NULL),
	m_updateManager(),
	m_docUpdater(NULL)
{
	init();
	bleeds = prefsData.bleeds;
	PDF_Options.bleeds = bleeds;
	Print_Options.firstUse = true;
}


ScribusDoc::ScribusDoc(const QString& docName, int unitindex, const PageSize& pagesize, const MarginStruct& margins, const DocPagesSetup& pagesSetup) : UndoObject( tr("Document")),
	m_hasGUI(false),
	prefsData(PrefsManager::instance()->appPrefs),
	undoManager(UndoManager::instance()),
	loading(false),
	modified(false),
	ActiveLayer(0),
	docUnitIndex(unitindex),
	docUnitRatio(unitGetRatioFromIndex(docUnitIndex)),
	automaticTextFrames(pagesSetup.autoTextFrames),
	m_masterPageMode(false),
	m_ScMW(0),
	m_View(0),
	m_guardedObject(this),
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGuides(false), GuideLock(false),
	scratch(prefsData.scratch),
	GapHorizontal(prefsData.GapHorizontal),
	GapVertical(prefsData.GapVertical),
// 	ScratchLeft(prefsData.ScratchLeft),
// 	ScratchRight(prefsData.ScratchRight),
// 	ScratchTop(prefsData.ScratchTop),
// 	ScratchBottom(prefsData.ScratchBottom),
	minCanvasCoordinate(FPoint(0, 0)),
	maxCanvasCoordinate(FPoint(scratch.Left + scratch.Right, scratch.Top + scratch.Bottom)),
	rulerXoffset(0.0), rulerYoffset(0.0),
	Pages(0), MasterPages(), DocPages(),
	MasterNames(),
	Items(0), MasterItems(), DocItems(), FrameItems(),
	m_Selection(new Selection(this, true)),
	pageWidth(pagesize.width()), pageHeight(pagesize.height()),
	pageMargins(margins),
	marginPreset(prefsData.marginPreset),
	pageSets(prefsData.pageSets),
	PageSp(pagesSetup.columnCount), PageSpa(pagesSetup.columnDistance),
	currentPageLayout(pagesSetup.pageArrangement),
	PageOri(pagesSetup.orientation), m_pageSize(pagesize.name()),
	FirstPnum(pagesSetup.firstPageNumber),
	useRaster(false),
	PageColors(this, true),
	appMode(modeNormal),
	SubMode(-1),
	ShapeValues(0),
	ValCount(0),
	DocName(docName),
	UsedFonts(),
	AllFonts(&prefsData.AvailFonts),
	AObjects(),
	papColor(prefsData.DpapColor),
	CurrentSel(-1),
	nodeEdit(),
	typographicSettings(prefsData.typographicSettings),
	guidesSettings(prefsData.guidesSettings),
	toolSettings(prefsData.toolSettings),
	checkerProfiles(prefsData.checkerProfiles),
	curCheckProfile(prefsData.curCheckProfile),
	LastAuto(0), FirstAuto(0),
	DraggedElem(0),
	ElemToLink(0),
	DragElements(),
	docParagraphStyles(),
	docCharStyles(),
	Layers(),
	marginColored(prefsData.marginColored),
	GroupCounter(1),
	JavaScripts(),
	TotalItems(0),
	MinWordLen(prefsData.MinWordLen),
	HyCount(prefsData.HyCount),
	Language(prefsData.Language),
	Automatic(prefsData.Automatic),
	AutoCheck(prefsData.AutoCheck),
	PDF_Options(prefsData.PDF_Options),
	RePos(false),
	BookMarks(),
	OldBM(false),
	hasName(false),
	RotMode(0),
	AutoSave(prefsData.AutoSave),
	AutoSaveTime(prefsData.AutoSaveTime),
	autoSaveTimer(new QTimer(this)),
	MLineStyles(),
	arrowStyles(prefsData.arrowStyles),
	WinHan(0),
	DoDrawing(true),
	OpenNodes(),
	CurTimer(0),
	docLayerErrors(),
	docItemErrors(),
	masterItemErrors(),
	docItemAttributes(prefsData.defaultItemAttributes),
	docToCSetups(prefsData.defaultToCSetups),
	// sections
	symReturn(), symNewLine(), symTab(), symNonBreak(), symNewCol(), symNewFrame(),
	docHyphenator(0),
	m_itemCreationTransaction(NULL),
	m_alignTransaction(NULL),
	m_currentPage(NULL),
	m_updateManager(),
	m_docUpdater(NULL)
{
	pageSets[pagesSetup.pageArrangement].FirstPage = pagesSetup.firstPageLocation;
	init();
	bleeds = prefsData.bleeds;
	PDF_Options.bleeds = bleeds;
	Print_Options.firstUse = true;
}


void ScribusDoc::init()
{
	Q_CHECK_PTR(m_Selection);
	Q_CHECK_PTR(autoSaveTimer);

	HasCMS = false;
	CMSSettings.CMSinUse = false;

	DocInputRGBProf = NULL;
	DocInputCMYKProf = NULL;
	DocInputImageRGBProf = NULL;
	DocInputImageCMYKProf = NULL;
	DocOutputProf = NULL;
	DocPrinterProf = NULL;
	stdTransRGBMon = NULL;
	stdTransCMYKMon = NULL;
	stdProof = NULL;
	stdTransImg = NULL;
	stdProofImg = NULL;
	stdTransCMYK = NULL;
	stdProofCMYK = NULL;
	stdTransRGB = NULL;
	stdProofGC = NULL;
	stdProofCMYKGC = NULL;
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
	CMSSettings = prefsManager->appPrefs.DCMSset;
	PDF_Options.SolidProf = CMSSettings.DefaultSolidColorRGBProfile;
	PDF_Options.ImageProf = CMSSettings.DefaultImageRGBProfile;
	PDF_Options.PrintProf = CMSSettings.DefaultPrinterProfile;
	PDF_Options.Intent = CMSSettings.DefaultIntentColors;
	PDF_Options.Intent2 = CMSSettings.DefaultIntentImages;

	AddFont(prefsData.toolSettings.defFont);//, prefsData.AvailFonts[prefsData.toolSettings.defFont]->Font);
	toolSettings.defFont = prefsData.toolSettings.defFont;
	toolSettings.defSize = prefsData.toolSettings.defSize;
	toolSettings.tabFillChar = prefsData.toolSettings.tabFillChar;
	toolSettings.dispX = prefsData.toolSettings.dispX;
	toolSettings.dispY = prefsData.toolSettings.dispY;
	toolSettings.constrain = prefsData.toolSettings.constrain;

	PageColors.ensureBlackAndWhite();
	if (prefsData.toolSettings.dPen != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dPen, prefsData.DColors[prefsData.toolSettings.dPen]);
	toolSettings.dPen = prefsData.toolSettings.dPen;
	if (prefsData.toolSettings.dPenLine != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dPenLine, prefsData.DColors[prefsData.toolSettings.dPenLine]);
	toolSettings.dPenLine = prefsData.toolSettings.dPenLine;
	if (prefsData.toolSettings.dPenText != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dPenText, prefsData.DColors[prefsData.toolSettings.dPenText]);
	toolSettings.dPenText = prefsData.toolSettings.dPenText;
	if (prefsData.toolSettings.dStrokeText != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dStrokeText, prefsData.DColors[prefsData.toolSettings.dStrokeText]);
	toolSettings.dStrokeText = prefsData.toolSettings.dStrokeText;
	if (prefsData.toolSettings.dBrush != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dBrush, prefsData.DColors[prefsData.toolSettings.dBrush]);
	toolSettings.dBrush = prefsData.toolSettings.dBrush;
	if (prefsData.toolSettings.dBrushPict != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dBrushPict, prefsData.DColors[prefsData.toolSettings.dBrushPict]);
	toolSettings.dBrushPict = prefsData.toolSettings.dBrushPict;
	if (prefsData.toolSettings.dTextBackGround != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dTextBackGround, prefsData.DColors[prefsData.toolSettings.dTextBackGround]);
	toolSettings.dTextBackGround = prefsData.toolSettings.dTextBackGround;
	if (prefsData.toolSettings.dTextLineColor != CommonStrings::None)
		PageColors.insert(prefsData.toolSettings.dTextLineColor, prefsData.DColors[prefsData.toolSettings.dTextLineColor]);

	
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
	cstyle.setFont(prefsData.AvailFonts[toolSettings.defFont]);
	cstyle.setFontSize(toolSettings.defSize);
	cstyle.setFeatures(QStringList(CharStyle::INHERIT));
	cstyle.setFillColor(toolSettings.dPenText);
	cstyle.setFillShade(toolSettings.dTextPenShade);
	cstyle.setStrokeColor(toolSettings.dStrokeText);
	cstyle.setStrokeShade(toolSettings.dTextStrokeShade);
	cstyle.setBaselineOffset(0);
	cstyle.setShadowXOffset(50);
	cstyle.setShadowYOffset(-50);
	cstyle.setOutlineWidth(10);
	cstyle.setUnderlineOffset(typographicSettings.valueUnderlinePos);
	cstyle.setUnderlineWidth(typographicSettings.valueUnderlineWidth);
	cstyle.setStrikethruOffset(typographicSettings.valueStrikeThruPos);
	cstyle.setStrikethruWidth(typographicSettings.valueStrikeThruPos);
	cstyle.setScaleH(1000);
	cstyle.setScaleV(1000);
	cstyle.setTracking(0);
	cstyle.setLanguage(PrefsManager::instance()->appPrefs.Language);
	
	docParagraphStyles.create(pstyle);
	docParagraphStyles.makeDefault( &(docParagraphStyles[0]) );
	
	docCharStyles.create(cstyle);
	docCharStyles.makeDefault( &(docCharStyles[0]) );
	
	docParagraphStyles[0].breakImplicitCharStyleInheritance();
	docParagraphStyles[0].charStyle().setContext( & docCharStyles );
//	docParagraphStyles[0].charStyle().setName( "cdocdefault" ); // DONT TRANSLATE

	currentStyle = pstyle;
	
	Layers.addLayer( tr("Background") );
	// Fixme: Check PDF version input
	PDF_Options.Version = (PDFOptions::PDFVersion)prefsData.PDF_Options.Version;

	PDF_Options.firstUse = true;
	docPatterns.clear();

	if (AutoSave && ScCore->usingGUI())
		autoSaveTimer->start(AutoSaveTime);
	//Do this after all the collections have been created and cleared!
	m_masterPageMode=true; // quick hack to force the change of pointers in setMasterPageMode();
	setMasterPageMode(false);
	addSymbols();
}


ScribusDoc::~ScribusDoc()
{
	m_guardedObject.nullify();
	CloseCMSProfiles();
//	DocItems.setAutoDelete(true);
//	FrameItems.setAutoDelete(true);
//	DocItems.clear();
	while (!DocItems.isEmpty())
	{
		delete DocItems.takeFirst();
	}
	FrameItems.clear();

	while (!MasterPages.isEmpty())
	{
		delete MasterPages.takeFirst();
	}
	while (!DocPages.isEmpty())
	{
		delete DocPages.takeFirst();
	}
// 	MasterPages.setAutoDelete(true);
// 	DocPages.setAutoDelete(true);
// 	MasterPages.clear();
// 	DocPages.clear();
//	MasterItems.setAutoDelete(true);
//	MasterItems.clear();
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
}


void ScribusDoc::setup(const int unitIndex, const int fp, const int firstLeft, const int orientation, const int firstPageNumber, const QString& defaultPageSize, const QString& documentName)
{
	docUnitIndex=unitIndex;
	pageSets[fp].FirstPage = firstLeft;
	PageOri = orientation;
	m_pageSize = defaultPageSize;
	FirstPnum = firstPageNumber;
	currentPageLayout = fp;
	setName(documentName);
	HasCMS = false;
	if (!PDF_Options.UseLPI)
	{
		PDF_Options.LPISettings.clear();
		struct LPIData lpo;
		lpo.Frequency = 133;
		lpo.SpotFunc = 3;
		lpo.Angle = 105;
		PDF_Options.LPISettings.insert("Cyan", lpo);
		lpo.Angle = 75;
		PDF_Options.LPISettings.insert("Magenta", lpo);
		lpo.Angle = 90;
		PDF_Options.LPISettings.insert("Yellow", lpo);
		lpo.Angle = 45;
		PDF_Options.LPISettings.insert("Black", lpo);
		ActiveLayer = 0;
	}

	appMode = modeNormal;
	PrefsManager *prefsManager=PrefsManager::instance();
	PageColors = prefsManager->colorSet();
	PageColors.ensureBlackAndWhite();
	PageColors.setDocument(this);

	CMSSettings = prefsManager->appPrefs.DCMSset;
	PDF_Options.SolidProf = CMSSettings.DefaultSolidColorRGBProfile;
	PDF_Options.ImageProf = CMSSettings.DefaultImageRGBProfile;
	PDF_Options.PrintProf = CMSSettings.DefaultPrinterProfile;
	PDF_Options.Intent = CMSSettings.DefaultIntentColors;
	PDF_Options.Intent2 = CMSSettings.DefaultIntentImages;
	BlackPoint   = CMSSettings.BlackPoint;
	SoftProofing = CMSSettings.SoftProofOn;
	Gamut        = CMSSettings.GamutCheck;
	IntentColors = CMSSettings.DefaultIntentColors;
	IntentImages = CMSSettings.DefaultIntentImages;
	if (ScCore->haveCMS() && CMSSettings.CMSinUse)
	{
		if (OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles))
		{
			HasCMS = true;
			PDF_Options.SComp = CMSSettings.ComponentsInput2;
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
	m_ScMW=mw;
	//FIXME: stop using m_View
	m_View=view;
	docHyphenator=new Hyphenator(m_ScMW, this);
	Q_CHECK_PTR(docHyphenator);
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
	if (ScCore->haveCMS() /*&& CMSSettings.CMSinUse*/)
	{
		if (DocInputImageRGBProf && !ScCore->IsDefaultProfile(DocInputImageRGBProf))
			cmsCloseProfile(DocInputImageRGBProf);
		if (DocInputImageCMYKProf && !ScCore->IsDefaultProfile(DocInputImageCMYKProf))
			cmsCloseProfile(DocInputImageCMYKProf);
		if (DocInputRGBProf && !ScCore->IsDefaultProfile(DocInputRGBProf))
			cmsCloseProfile(DocInputRGBProf);
		if (DocInputCMYKProf && !ScCore->IsDefaultProfile(DocInputCMYKProf))
			cmsCloseProfile(DocInputCMYKProf);
		if (DocOutputProf && !ScCore->IsDefaultProfile(DocOutputProf))
			cmsCloseProfile(DocOutputProf);
		if (DocPrinterProf && !ScCore->IsDefaultProfile(DocPrinterProf))
			cmsCloseProfile(DocPrinterProf);
		if (stdTransRGBMon && !ScCore->IsDefaultTransform(stdTransRGBMon))
			cmsDeleteTransform(stdTransRGBMon);
		if (stdTransCMYKMon && !ScCore->IsDefaultTransform(stdTransCMYKMon))
			cmsDeleteTransform(stdTransCMYKMon);
		if (stdProof && !ScCore->IsDefaultTransform(stdProof))
			cmsDeleteTransform(stdProof);
		if (stdTransImg && !ScCore->IsDefaultTransform(stdTransImg))
			cmsDeleteTransform(stdTransImg);
		if (stdProofImg && !ScCore->IsDefaultTransform(stdProofImg))
			cmsDeleteTransform(stdProofImg);
		if (stdTransCMYK && !ScCore->IsDefaultTransform(stdTransCMYK))
			cmsDeleteTransform(stdTransCMYK);
		if (stdProofCMYK && !ScCore->IsDefaultTransform(stdProofCMYK))
			cmsDeleteTransform(stdProofCMYK);
		if (stdTransRGB && !ScCore->IsDefaultTransform(stdTransRGB))
			cmsDeleteTransform(stdTransRGB);
		if (stdProofCMYKGC && !ScCore->IsDefaultTransform(stdProofCMYKGC))
			cmsDeleteTransform(stdProofCMYKGC);
		if (stdProofGC && !ScCore->IsDefaultTransform(stdProofGC))
			cmsDeleteTransform(stdProofGC);
		DocInputRGBProf = NULL;
		DocInputCMYKProf = NULL;
		DocInputImageRGBProf = NULL;
		DocInputImageCMYKProf = NULL;
		DocOutputProf = NULL;
		DocPrinterProf = NULL;
		stdTransRGBMon = NULL;
		stdTransCMYKMon = NULL;
		stdProof = NULL;
		stdTransImg = NULL;
		stdProofImg = NULL;
		stdTransCMYK = NULL;
		stdProofCMYK = NULL;
		stdTransRGB = NULL;
		stdProofCMYKGC = NULL;
		stdProofGC = NULL;
	}
	SetDefaultCMSParams();
}

void ScribusDoc::SetDefaultCMSParams()
{
	BlackPoint     = true;
	SoftProofing   = false;
	Gamut          = false;
	IntentColors   = 1; // INTENT_RELATIVE_COLORIMETRIC
	IntentImages   = 1; // INTENT_RELATIVE_COLORIMETRIC
	DocInputRGBProf       = ScCore->defaultRGBProfile;
	DocInputCMYKProf      = ScCore->defaultCMYKProfile;
	DocInputImageRGBProf  = ScCore->defaultRGBProfile;
	DocInputImageCMYKProf = ScCore->defaultCMYKProfile;
	DocOutputProf         = ScCore->defaultRGBProfile;
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
	cmsHPROFILE inputProf = NULL;
	cmsErrorAction(LCMS_ERROR_ABORT);
	if (setjmp(cmsJumpBuffer))
	{
		// Reset to the default handler otherwise may enter a loop
		// if an error occur afterwards
		cmsSetErrorHandler(NULL);
		cmsErrorAction(LCMS_ERROR_IGNORE);
		CloseCMSProfiles();
		cmsErrorAction(LCMS_ERROR_ABORT);
		CMSSettings.CMSinUse = false;
		QString message = tr("An error occurred while opening ICC profiles, color management is not enabled." );
		if (ScCore->usingGUI())
			QMessageBox::warning(m_ScMW, CommonStrings::trWarning, message, QMessageBox::Ok, 0, 0);
		else
			qWarning( "%s", message.toLocal8Bit().data() );
		return false;
	}
	cmsSetErrorHandler(&cmsErrorHandler);
	const QByteArray rgbInputProfilePath(InPo[CMSSettings.DefaultSolidColorRGBProfile].toLocal8Bit());
	DocInputRGBProf = cmsOpenProfileFromFile(rgbInputProfilePath.data(), "r");
	const QByteArray cmykInputProfilePath(InPoCMYK[CMSSettings.DefaultSolidColorCMYKProfile].toLocal8Bit());
	DocInputCMYKProf = cmsOpenProfileFromFile(cmykInputProfilePath.data(), "r");
	const QByteArray monitorProfilePath(MoPo[CMSSettings.DefaultMonitorProfile].toLocal8Bit());
	DocOutputProf = cmsOpenProfileFromFile(monitorProfilePath.data(), "r");
	const QByteArray printerProfilePath(PrPo[CMSSettings.DefaultPrinterProfile].toLocal8Bit());
	DocPrinterProf = cmsOpenProfileFromFile(printerProfilePath, "r");
	const QByteArray rgbInputImgProfilePath(InPo[CMSSettings.DefaultImageRGBProfile].toLocal8Bit());
	DocInputImageRGBProf = cmsOpenProfileFromFile(rgbInputImgProfilePath.data(), "r");
	const QByteArray cmykInputImgProfilePath(InPoCMYK[CMSSettings.DefaultImageCMYKProfile].toLocal8Bit());
	DocInputImageCMYKProf = cmsOpenProfileFromFile(cmykInputImgProfilePath.data(), "r");
	if ((DocInputRGBProf == NULL) || (DocInputCMYKProf == NULL) || (DocOutputProf == NULL) || (DocPrinterProf == NULL) || (DocInputImageCMYKProf == NULL) || (DocInputImageRGBProf == NULL))
	{
		CMSSettings.CMSinUse = false;
		cmsSetErrorHandler(NULL);
		return false;
	}
	int dcmsFlags = 0;
	int dcmsFlagsGC = 0;
	dcmsFlags |= cmsFLAGS_LOWRESPRECALC;
	dcmsFlagsGC |= cmsFLAGS_LOWRESPRECALC;
//	int dcmsFlags2 = cmsFLAGS_NOTPRECALC;
	if (CMSSettings.GamutCheck)
		dcmsFlagsGC |= cmsFLAGS_GAMUTCHECK;
	if (CMSSettings.BlackPoint)
	{
		dcmsFlags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
		dcmsFlagsGC |= cmsFLAGS_BLACKPOINTCOMPENSATION;
	}
	// set Gamut alarm color to #00ff00
	cmsSetAlarmCodes(0, 255, 0);
	stdTransRGBMon  = scCmsCreateTransform(DocInputRGBProf, TYPE_RGB_16,
										DocOutputProf, TYPE_RGB_16,
										IntentColors,
										dcmsFlags);
	stdTransCMYKMon = scCmsCreateTransform(DocInputCMYKProf, TYPE_CMYK_16,
										DocOutputProf, TYPE_RGB_16,
										IntentColors,
										dcmsFlags);
	// TODO : check input profiles used for images
	stdProofImg = scCmsCreateProofingTransform(DocInputImageRGBProf, TYPE_RGBA_8,
	              DocOutputProf, TYPE_RGBA_8,
	              DocPrinterProf,
	              IntentImages,
	              INTENT_RELATIVE_COLORIMETRIC, dcmsFlagsGC | cmsFLAGS_SOFTPROOFING);
	stdProofImgCMYK = scCmsCreateProofingTransform(DocInputImageCMYKProf, TYPE_CMYK_8,
	              DocOutputProf, TYPE_RGBA_8,
	              DocPrinterProf,
	              IntentImages,
	              INTENT_RELATIVE_COLORIMETRIC, dcmsFlagsGC | cmsFLAGS_SOFTPROOFING);
	stdTransImg = scCmsCreateTransform(DocInputRGBProf, TYPE_RGBA_8,
	                                 DocOutputProf, TYPE_RGBA_8,
	                                 IntentImages,
	                                 dcmsFlags);
	stdTransRGB = scCmsCreateTransform(DocInputCMYKProf, TYPE_CMYK_16,
						DocInputRGBProf, TYPE_RGB_16,
						IntentColors,
						dcmsFlags);
	stdTransCMYK = scCmsCreateTransform(DocInputRGBProf, TYPE_RGB_16,
						DocInputCMYKProf, TYPE_CMYK_16,
						IntentColors,
						dcmsFlags);
	cmsHPROFILE inputProfRGB = NULL;
	cmsHPROFILE inputProfCMYK = NULL;
	if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigCmykData)
	{
		inputProf = (CMSSettings.SoftProofOn && CMSSettings.SoftProofFullOn) ? DocInputCMYKProf : DocPrinterProf;
		inputProfRGB  = DocInputRGBProf;
		inputProfCMYK = inputProf;
	}
	else
	{
		inputProf = (CMSSettings.SoftProofOn && CMSSettings.SoftProofFullOn) ? DocInputRGBProf : DocPrinterProf;
		inputProfRGB  = inputProf;
		inputProfCMYK = DocInputCMYKProf;
	}
	stdProof = scCmsCreateProofingTransform(inputProfRGB, TYPE_RGB_16,
	                    DocOutputProf, TYPE_RGB_16,
	                    DocPrinterProf,
	                    IntentColors,
	                    INTENT_RELATIVE_COLORIMETRIC, dcmsFlags | cmsFLAGS_SOFTPROOFING);
	stdProofGC = scCmsCreateProofingTransform(inputProfRGB, TYPE_RGB_16,
	                    DocOutputProf, TYPE_RGB_16,
	                    DocPrinterProf,
	                    IntentColors,
	                    INTENT_RELATIVE_COLORIMETRIC, dcmsFlags | cmsFLAGS_SOFTPROOFING | cmsFLAGS_GAMUTCHECK);
	stdProofCMYK = scCmsCreateProofingTransform(inputProfCMYK, TYPE_CMYK_16,
						DocOutputProf, TYPE_RGB_16,
						DocPrinterProf,
						IntentColors,
						INTENT_RELATIVE_COLORIMETRIC, dcmsFlags | cmsFLAGS_SOFTPROOFING);
	stdProofCMYKGC = scCmsCreateProofingTransform(inputProfCMYK, TYPE_CMYK_16,
						DocOutputProf, TYPE_RGB_16,
						DocPrinterProf,
						IntentColors,
						INTENT_RELATIVE_COLORIMETRIC, dcmsFlags | cmsFLAGS_SOFTPROOFING | cmsFLAGS_GAMUTCHECK);

	if (static_cast<int>(cmsGetColorSpace(DocInputRGBProf)) == icSigRgbData)
			CMSSettings.ComponentsInput2 = 3;
	if (static_cast<int>(cmsGetColorSpace(DocInputRGBProf)) == icSigCmykData)
			CMSSettings.ComponentsInput2 = 4;
	if (static_cast<int>(cmsGetColorSpace(DocInputRGBProf)) == icSigCmyData)
			CMSSettings.ComponentsInput2 = 3;
	if (static_cast<int>(cmsGetColorSpace(DocInputCMYKProf)) == icSigRgbData)
			CMSSettings.ComponentsInput3 = 3;
	if (static_cast<int>(cmsGetColorSpace(DocInputCMYKProf)) == icSigCmykData)
			CMSSettings.ComponentsInput3 = 4;
	if (static_cast<int>(cmsGetColorSpace(DocInputCMYKProf)) == icSigCmyData)
			CMSSettings.ComponentsInput3 = 3;
	if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigRgbData)
			CMSSettings.ComponentsPrinter = 3;
	if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigCmykData)
			CMSSettings.ComponentsPrinter = 4;
	if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigCmyData)
			CMSSettings.ComponentsPrinter = 3;

	cmsSetErrorHandler(NULL);
	return true;
}


void ScribusDoc::enableCMS(bool enable)
{
	m_ScMW->setStatusBarInfoText( tr("Adjusting Colors"));
	m_ScMW->mainWindowProgressBar->reset();
	int cc = PageColors.count() + Items->count();
	m_ScMW->mainWindowProgressBar->setMaximum(cc);
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	bool oldCM = CMSSettings.CMSinUse;
	bool newCM = enable;
	CloseCMSProfiles();
	CMSSettings.CMSinUse = newCM;
	HasCMS       = CMSSettings.CMSinUse;
	BlackPoint   = CMSSettings.BlackPoint;
	SoftProofing = CMSSettings.SoftProofOn;
	Gamut        = CMSSettings.GamutCheck;
	IntentColors = CMSSettings.DefaultIntentColors;
	IntentImages = CMSSettings.DefaultIntentImages;
	if (!CMSSettings.CMSinUse)
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
		PDF_Options.SComp = CMSSettings.ComponentsInput2;
		PDF_Options.SolidProf = CMSSettings.DefaultSolidColorRGBProfile;
		PDF_Options.ImageProf = CMSSettings.DefaultImageRGBProfile;
		PDF_Options.PrintProf = CMSSettings.DefaultPrinterProfile;
		PDF_Options.Intent = CMSSettings.DefaultIntentColors;
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


void ScribusDoc::restore(UndoState* state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
		bool layersUndo=false;
		if (ss->contains("GUIDE_LOCK"))
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
		else if (ss->contains("ADD_LAYER"))
		{
			if (isUndo)
				deleteLayer(ss->getInt("LAYER_NR"), false);
			else
			{
				int layerNumber=addLayer( ss->get("NAME"), false );
				int newLayerNumber=ss->getInt("LAYER_NR");
				bool renumberedOk=renumberLayer(layerNumber, newLayerNumber);
				Q_ASSERT(renumberedOk);
			}
			layersUndo=true;
		}
		else if (ss->contains("REMOVE_LAYER"))
		{
			if (isUndo)
			{
				int layerNumber=addLayer( ss->get("NAME"), false );
				int newLayerNumber=ss->getInt("LAYER_NR");
				bool renumberedOk=renumberLayer(layerNumber, newLayerNumber);
				Q_ASSERT(renumberedOk);
				layerNumber=newLayerNumber;
				//Layer is at the top now, lower it until it reaches the old level
				int level = ss->getInt("LEVEL");
				while (layerLevelFromNumber(layerNumber)!=level)
					lowerLayer(layerNumber);
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
void ScribusDoc::setPage(double b, double h, double t, double l, double r, double bo, double sp, double ab, bool atf, int fp)
{
	pageWidth = b;
	pageHeight = h;
	pageMargins.Top = t;
	pageMargins.Left = l;
	pageMargins.Right = r;
	pageMargins.Bottom = bo;
	PageSp = sp;
	PageSpa = ab;
	currentPageLayout = fp;
	automaticTextFrames = atf;

	//CB Moved from scribus.cpp. Overrides the defaults...
//	PDF_Options.BleedTop = pageMargins.Top;
//	PDF_Options.BleedLeft = pageMargins.Left;
//	PDF_Options.BleedRight = pageMargins.Right;
//	PDF_Options.BleedBottom = pageMargins.Bottom;
}

void ScribusDoc::resetPage(MarginStruct& newMargins, int fp)
{
	pageMargins = newMargins;
	currentPageLayout = fp;
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

	for(ObjAttrVector::Iterator it = docItemAttributes.begin(); it!= docItemAttributes.end(); ++it)
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
	Page* addedPage = new Page(scratch.Left, DocPages.count()*(pageHeight+scratch.Bottom+scratch.Top)+scratch.Top, pageWidth, pageHeight);
	assert(addedPage!=NULL);
	addedPage->setDocument(this);
	addedPage->Margins.Top = pageMargins.Top;
	addedPage->Margins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Top = pageMargins.Top;
	addedPage->initialMargins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Left = pageMargins.Left;
	addedPage->initialMargins.Right = pageMargins.Right;
	addedPage->setPageNr(pageIndex);
	addedPage->m_pageSize = m_pageSize;
	addedPage->PageOri = PageOri;
	addedPage->marginPreset = marginPreset;
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
	Page* addedPage = new Page(scratch.Left, scratch.Top, pageWidth, pageHeight);
	assert(addedPage!=NULL);
	addedPage->setDocument(this);
	addedPage->Margins.Top = pageMargins.Top;
	addedPage->Margins.Bottom = pageMargins.Bottom;
	addedPage->Margins.Left = pageMargins.Left;//todo fix for layouts
	addedPage->Margins.Right = pageMargins.Right;
	addedPage->initialMargins.Top = pageMargins.Top;
	addedPage->initialMargins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Left = pageMargins.Left;
	addedPage->initialMargins.Right = pageMargins.Right;
	addedPage->m_pageSize = m_pageSize;
	addedPage->PageOri = PageOri;
	addedPage->marginPreset = marginPreset;
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
		                     addToPage->Margins.Top+addToPage->yOffset(), pageWidth-addToPage->Margins.Right-addToPage->Margins.Left,
		                     pageHeight-addToPage->Margins.Bottom-addToPage->Margins.Top,
							 1, CommonStrings::None, toolSettings.dPen, true);
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
	int lnr = Layers.addLayer(layerName);
	if (activate)
		setActiveLayer(lnr);
	const ScLayer* ll = Layers.layerByNumber(lnr);

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::AddLayer, "", Um::ICreate);
		ss->set("ADD_LAYER", "add_layer");
		ss->set("ACTIVE", ActiveLayer);
		ss->set("NAME", ll->Name);
		ss->set("LAYER_NR", ll->LNr);
		undoManager->action(this, ss, DocName, Um::ILayer);
	}
	return lnr;
}


void ScribusDoc::copyLayer(int layerNumberToCopy, int whereToInsert)
{
	if(!setActiveLayer(whereToInsert))
		return;
	Selection sourceSelection(this);
	for (int ite(0); ite < Items->count(); ++ite)
	{
		PageItem *itemToCopy = Items->at(ite);
		if (itemToCopy->LayerNr == layerNumberToCopy)
		{
			sourceSelection.addItem(itemToCopy);
		}
	}
	Selection targetSelection(Serializer(*this).cloneObjects(sourceSelection));
	for(int si(0); si < targetSelection.count(); ++si)
	{
		targetSelection.itemAt(si)->setLayer(whereToInsert);
	}
}


bool ScribusDoc::deleteLayer(const int layerNumber, const bool deleteItems)
{
	if (Layers.count() < 2)
		return false;
	const ScLayer* lToRemove = Layers.layerByNumber(layerNumber);
	if (!lToRemove)
		return false;
	int layerLevel = lToRemove->Level;
	QString name   = lToRemove->Name;
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction("Layer", Um::IDocument, Um::DeleteLayer, "", Um::IDelete));

	rebuildItemLists();
	if (ScCore->usingGUI())
		removeLayer(layerNumber, deleteItems);

	/*
	//Layer found, do we want to delete its items too?
	if (masterPageMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
	for (uint b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->LayerNr == layerNumber)
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
		if (DocItems.at(b)->LayerNr == l)
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
	Layers.removeLayerByNumber(layerNumber);

	if (activeTransaction)
	{
		SimpleState *ss = new SimpleState(Um::DeleteLayer, "", Um::IDelete);
		ss->set("REMOVE_LAYER", "remove_layer");
		ss->set("ACTIVE", layerNumber);
		ss->set("LEVEL", layerLevel);
		ss->set("NAME", name);
		ss->set("LAYER_NR", layerNumber);
		ss->set("DELETE", deleteItems);
		undoManager->action(this, ss, DocName, Um::ILayer);
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
//	setActiveLayer(layerNumberFromLevel(0));
	return true;
}


int ScribusDoc::activeLayer()
{
	return ActiveLayer;
}


const QString& ScribusDoc::activeLayerName()
{
	const ScLayer* ll = Layers.layerByNumber(ActiveLayer);
	Q_ASSERT(ll);
	return ll->Name;
}


bool ScribusDoc::setActiveLayer(const int layerToActivate)
{
	const ScLayer* ll = Layers.layerByNumber(layerToActivate);
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
		ActiveLayer=ll->LNr;
	return (ll != NULL);
}


bool ScribusDoc::setLayerPrintable(const int layerNumber, const bool isPrintable)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			bool oldPrintable = (*it).isPrintable;
			(*it).isPrintable = isPrintable;

			if (oldPrintable!=isPrintable && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(isPrintable ? Um::PrintLayer : Um::DoNotPrintLayer,
						                          "", Um::IPrint);
				ss->set("PRINT_LAYER", "print_layer");
				ss->set("ACTIVE", (*it).LNr);
				ss->set("PRINT", isPrintable);
				undoManager->action(this, ss, DocName, Um::IDocument);
			}
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerPrintable(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).isPrintable;
	}
	return false;
}


bool ScribusDoc::setLayerVisible(const int layerNumber, const bool isViewable)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			(*it).isViewable = isViewable;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerVisible(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).isViewable;
	}
	return false;
}


bool ScribusDoc::setLayerLocked(const int layerNumber, const bool isLocked)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			(*it).isEditable = !isLocked;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerLocked(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return !(*it).isEditable;
	}
	return false;
}


bool ScribusDoc::setLayerFlow(const int layerNumber, const bool flow)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			(*it).flowControl = flow;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerFlow(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).flowControl;
	}
	return false;
}


bool ScribusDoc::setLayerTransparency(const int layerNumber, double trans)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			(*it).transparency = trans;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


double ScribusDoc::layerTransparency(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).transparency;
	}
	return 1.0;
}


bool ScribusDoc::setLayerBlendMode(const int layerNumber, int blend)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			(*it).blendMode = blend;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


int ScribusDoc::layerBlendMode(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).blendMode;
	}
	return 0;
}


bool ScribusDoc::setLayerOutline(const int layerNumber, const bool outline)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			(*it).outlineMode = outline;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerOutline(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).outlineMode;
	}
	return false;
}


bool ScribusDoc::setLayerMarker(const int layerNumber, QColor color)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
		{
			(*it).markerColor = color;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


QColor ScribusDoc::layerMarker(const int layerNumber)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).markerColor;
	}
	return QColor(0, 0, 0);
}


int ScribusDoc::layerLevelFromNumber(const int layerNumber)
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerNumber)
			return Layers[i].Level;
	}
	return -1;
}


int ScribusDoc::layerCount() const
{
	return Layers.count();
}


int ScribusDoc::layerNumberFromLevel(const int layerLevel)
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].Level == layerLevel)
			return Layers[i].LNr;
	}
	return -1;
}


bool ScribusDoc::lowerLayer(const int layerNumber)
{
	return lowerLayerByLevel(layerLevelFromNumber(layerNumber));
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
		if ((*it).Level == layerLevel-1)
			break;
	}
	ScLayers::iterator it2;
	ScLayers::iterator it2end=Layers.end();
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if ((*it2).Level == layerLevel)
			break;
	}
	(*it2).Level -= 1;
	(*it).Level += 1;
	return true;
}


bool ScribusDoc::raiseLayer(const int layerNumber)
{
	return raiseLayerByLevel(layerLevelFromNumber(layerNumber));
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
		if ((*it).Level == layerLevel+1)
			break;
	}
	ScLayers::iterator it2;
	ScLayers::iterator it2end=Layers.end();
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if ((*it2).Level == layerLevel)
			break;
	}
	(*it2).Level += 1;
	(*it).Level -= 1;
	return true;
}


QString ScribusDoc::layerName(const int layerNumber) const
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerNumber)
			return Layers[i].Name;
	}
	return QString::null;
}


bool ScribusDoc::changeLayerName(const int layerNumber, const QString& newName)
{
	uint layerCount=Layers.count();
	bool found=false;
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerNumber)
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


bool ScribusDoc::layerContainsItems(const int layerNumber)
{
	uint masterItemsCount=MasterItems.count();
	for (uint i = 0; i < masterItemsCount; ++i)
	{
		if (MasterItems.at(i)->LayerNr == layerNumber)
			return true;
	}
	uint docItemsCount=DocItems.count();
	for (uint i = 0; i < docItemsCount; ++i)
	{
		if (DocItems.at(i)->LayerNr == layerNumber)
			return true;
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


bool ScribusDoc::renumberLayer(const int layerNumber, const int newLayerNumber)
{
	uint layerCount=Layers.count();
	uint foundIndex = 0;
	bool found=false;
	//Find layer to renumber, if found the new number, return as it exists already.
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerNumber)
		{
			foundIndex=i;
			found=true;
		}
		else
		if (Layers[i].LNr == newLayerNumber)
			return false;
	}
	if (!found)
		return false;
	Layers[foundIndex].LNr=newLayerNumber;
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
		if ((it.key() == toolSettings.dBrush) || (it.key() == toolSettings.dPen) || (it.key() == toolSettings.dBrushPict)
		        || (it.key() == toolSettings.dPenLine) || (it.key() == toolSettings.dPenText))
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


bool ScribusDoc::addPattern(QString &name, ScPattern& pattern)
{
	if (docPatterns.contains(name))
		name = tr("Copy_of_")+name;
	docPatterns.insert(name, pattern);
	return true;
}


void ScribusDoc::setPatterns(QMap<QString, ScPattern> &patterns)
{
	docPatterns.clear();
	docPatterns = patterns;
// 	for (QMap<QString, ScPattern>::Iterator it = patterns.begin(); it != patterns.end(); ++it)
// 	{
// 		docPatterns.insert(it.key(), it.data());
// 	}
/*	for (int c=0; c<DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if ((!docPatterns.contains(ite->pattern())) && (ite->GrType == 8))
		{
			ite->setPattern("");
			ite->GrType = 0;
		}
	}
	for (int c=0; c<MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if ((!docPatterns.contains(ite->pattern())) && (ite->GrType == 8))
		{
			ite->setPattern("");
			ite->GrType = 0;
		}
	}
	for (int c=0; c<FrameItems.count(); ++c)
	{
		PageItem *ite = FrameItems.at(c);
		if ((!docPatterns.contains(ite->pattern())) && (ite->GrType == 8))
		{
			ite->setPattern("");
			ite->GrType = 0;
		}
	} */
}


QStringList ScribusDoc::getUsedPatterns()
{
	QStringList results;
	for (int c = 0; c < MasterItems.count(); ++c)
	{
		if ((!results.contains(MasterItems.at(c)->pattern())) && (MasterItems.at(c)->GrType == 8))
			results.append(MasterItems.at(c)->pattern());
	}
	for (int c = 0; c < DocItems.count(); ++c)
	{
		if ((!results.contains(DocItems.at(c)->pattern())) && (DocItems.at(c)->GrType == 8))
			results.append(DocItems.at(c)->pattern());
	}
	for (int c = 0; c < FrameItems.count(); ++c)
	{
		if ((!results.contains(FrameItems.at(c)->pattern())) && (FrameItems.at(c)->GrType == 8))
			results.append(FrameItems.at(c)->pattern());
	}
	for (int c = 0; c < results.count(); ++c)
	{
		QStringList pats = getUsedPatternsHelper(results[c], results);
		if (!pats.isEmpty())
			results += pats;
	}
	return results;
}


QStringList ScribusDoc::getUsedPatternsSelection()
{
	QStringList results;
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			if (currItem->GrType == 8)
			{
				const QString& pat = currItem->pattern();
				if (!pat.isEmpty() && !results.contains(pat))
					results.append(currItem->pattern());
			}
		}
		for (int c = 0; c < results.count(); ++c)
		{
			QStringList pats = getUsedPatternsHelper(results[c], results);
			if (!pats.isEmpty())
				results += pats;
		}
		return results;
	}
	else
		return QStringList();
}


QStringList ScribusDoc::getUsedPatternsHelper(QString pattern, QStringList &results)
{
	ScPattern *pat = &docPatterns[pattern];
	QStringList pats;
	pats.clear();
	for (int c = 0; c < pat->items.count(); ++c)
	{
		if (pat->items.at(c)->GrType == 8)
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
			getUsedPatternsHelper(pats[c], results);
		}
	}
	return pats;
}

void ScribusDoc::reorganiseFonts()
{
	QTime t;
	t.start();
	QMap<QString,int> Really;
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
			QString fontName(it->itemText.defaultStyle().charStyle().font().replacementName());
			Really.insert(fontName, UsedFonts[fontName]);
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				uint itemTextCount=it->itemText.length();
				for (uint e = 0; e < itemTextCount; ++e)
				{
					Really.insert(it->itemText.charStyle(e).font().replacementName(), UsedFonts[it->itemText.charStyle(e).font().replacementName()]);
				}
			}
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
	AddFont(prefsManager->appPrefs.toolSettings.defFont);//, prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.toolSettings.defFont]->Font);
	AddFont(toolSettings.defFont);//, prefsManager->appPrefs.AvailFonts[toolSettings.defFont]->Font);
	qDebug( "Time elapsed: %d ms", t.elapsed() );
}

void ScribusDoc::getUsedFonts(QMap<QString, QMap<uint, FPointArray> > & Really)
{
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
			checkItemForFonts(it, Really, lc);
		}
	}
	QStringList patterns = getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			checkItemForFonts(pa.items.at(o), Really, 3);
		}
	}
}


void ScribusDoc::checkItemForFonts(PageItem *it, QMap<QString, QMap<uint, FPointArray> > & Really, uint lc)
{
	FPointArray gly;
	QString chstr;
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
			if ((chr == 13) || (chr == 32) || (chr == 29))
				continue;
			if (chr == 9)
			{
				for (int t1 = 0; t1 < it->itemText.paragraphStyle(e).tabValues().count(); t1++)
				{
					if (it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar.isNull())
						continue;
					chstr = QString(it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar);
					if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						if (chstr.toUpper() != QString(it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar))
							chstr = chstr.toUpper();
					}
					chr = chstr[0].unicode();
					uint gl = it->itemText.charStyle(e).font().char2CMap(chstr[0]);
					gly = it->itemText.charStyle(e).font().glyphOutline(gl);
					if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
						Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
				}
				for (int t1 = 0; t1 < it->itemText.defaultStyle().tabValues().count(); t1++)
				{
					if (it->itemText.defaultStyle().tabValues()[t1].tabFillChar.isNull())
						continue;
					chstr = QString(it->itemText.defaultStyle().tabValues()[t1].tabFillChar);
					if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						if (chstr.toUpper() != QString(it->itemText.defaultStyle().tabValues()[t1].tabFillChar))
							chstr = chstr.toUpper();
					}
					chr = chstr[0].unicode();
					uint gl = it->itemText.charStyle(e).font().char2CMap(chstr[0]);
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
				{
					//If not on a master page just get the page number for the page and the text
					if (lc!=0)
						pageNumberText=getSectionPageNumberForPageIndex(it->OwnPage);
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
								newText=getSectionPageNumberForPageIndex(a);
								for (int nti=0;nti<newText.length();++nti)
									if (pageNumberText.indexOf(newText[nti])==-1)
										pageNumberText+=newText[nti];
							}
						}
					}
				}
				else
				{
					if (lc!=0)
					{
						QString out("%1");
						int key = getSectionKeyForPageIndex(it->OwnPage);
						if (key == -1)
							pageNumberText = "";
						else
							pageNumberText = out.arg(getStringFromSequence(sections[key].type, sections[key].toindex - sections[key].fromindex + 1));
					}
					else
					{
						QString newText;
						uint docPageCount=DocPages.count();
						for (uint a = 0; a < docPageCount; ++a)
						{
							if (DocPages.at(a)->MPageNam == it->OnMasterPage)
							{
								QString out("%1");
								int key = getSectionKeyForPageIndex(a);
								if (key == -1)
									newText = "";
								else
									newText = out.arg(getStringFromSequence(sections[key].type, sections[key].toindex - sections[key].fromindex + 1));
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
			if (it->itemText.charStyle(e).effects() & ScStyle_SmartHyphenVisible)
			{
				uint gl = it->itemText.charStyle(e).font().char2CMap(QChar('-'));
				FPointArray gly(it->itemText.charStyle(e).font().glyphOutline(gl));
				if (!it->itemText.charStyle(e).font().replacementName().isEmpty())
					Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
			}
			if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
			{
				chstr = it->itemText.text(e, 1);
				if (chstr.toUpper() != it->itemText.text(e, 1))
					chstr = chstr.toUpper();
				chr = chstr[0].unicode();
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

	profileNames.append(CMSSettings.DefaultSolidColorRGBProfile);
	profileNames.append(CMSSettings.DefaultSolidColorCMYKProfile);
	if( profileNames.indexOf(CMSSettings.DefaultImageRGBProfile) < 0 )
		profileNames.append(CMSSettings.DefaultImageRGBProfile);
	if( profileNames.indexOf(CMSSettings.DefaultImageCMYKProfile) < 0 )
		profileNames.append(CMSSettings.DefaultImageCMYKProfile);
	if( profileNames.indexOf(CMSSettings.DefaultMonitorProfile) < 0 )
		profileNames.append(CMSSettings.DefaultMonitorProfile);
	if( profileNames.indexOf(CMSSettings.DefaultPrinterProfile) < 0 )
		profileNames.append(CMSSettings.DefaultPrinterProfile);
	
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
			if (it->IProfile.isEmpty() || profileNames.contains(it->IProfile))
				continue;
			profileNames.append(it->IProfile);
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
	docUnitIndex=newIndex;
	docUnitRatio = unitGetRatioFromIndex( docUnitIndex );
}


int ScribusDoc::unitIndex() const
{
	return docUnitIndex;
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
		if (pageSets[currentPageLayout].Columns != 1)
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
		else
		{
			Ap->initialMargins.Left = Mp->Margins.Left;
			Ap->initialMargins.Right = Mp->Margins.Right;
		}
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


bool ScribusDoc::changePageMargins(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double height, const double width, const int orientation, const QString& pageSize, const int pageNumber, const int pageType)
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
			//set the current page's values
			currentPage()->initialMargins.Top = initialTop;
			currentPage()->initialMargins.Bottom = initialBottom;
			currentPage()->initialMargins.Left = initialLeft;
			currentPage()->initialMargins.Right = initialRight;
			currentPage()->setInitialHeight(initialHeight);
			currentPage()->setInitialWidth(initialWidth);
			currentPage()->setHeight(height);
			currentPage()->setWidth(width);
			currentPage()->PageOri = orientation;
			currentPage()->m_pageSize = pageSize;
			currentPage()->LeftPg = pageType;
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
	for (uint c=0; c<itemsCount; ++c)
	{
		PageItem *ite = Items->at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
	}
	uint masterItemsCount=MasterItems.count();
	for (uint c=0; c<masterItemsCount; ++c)
	{
		PageItem *ite = MasterItems.at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
	}
	uint frameItemsCount=FrameItems.count();
	for (uint c=0; c<frameItemsCount; ++c)
	{
		PageItem *ite = FrameItems.at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
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
			if (ite->asImageFrame())
				loadPict(ite->Pfile, ite, true, false);
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items);
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
	if (currentPageLayout != singlePage)
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
					setActiveLayer((*it).LNr);
					for (uint ite = 0; ite < end2; ++ite)
					{
						PageItem *itemToCopy = MasterItems.at(ite);
						if ((itemToCopy->OnMasterPage == pageMaster->pageName()) && ((*it).LNr == itemToCopy->LayerNr))
							tempSelection.addItem(itemToCopy, true);
					}
					if (tempSelection.count() != 0)
					{
						setMasterPageMode(true);
						ScriXmlDoc *ss = new ScriXmlDoc();
						QString dataS = ss->WriteElem(this, view(), &tempSelection);
						setCurrentPage(targetPage);
						ss->ReadElem(dataS, prefsData.AvailFonts, this, targetPage->xOffset(), targetPage->yOffset(), false, true, prefsData.GFontSub, view());
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
			setActiveLayer((*it).LNr);
			for (uint ite = 0; ite < end; ++ite)
			{
				PageItem *itemToCopy = DocItems.at(ite);
				if ((itemToCopy->OwnPage == static_cast<int>(sourcePage->pageNr())) && ((*it).LNr == itemToCopy->LayerNr))
					tempSelection.addItem(itemToCopy, true);
			}
			if (tempSelection.count() != 0)
			{
				ScriXmlDoc *ss = new ScriXmlDoc();
				QString dataS = ss->WriteElem(this, view(), &tempSelection);
				setMasterPageMode(true);
				setCurrentPage(targetPage);
				ss->ReadElem(dataS, prefsData.AvailFonts, this, targetPage->xOffset(), targetPage->yOffset(), false, true, prefsData.GFontSub, view());
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
			newItem = new PageItem_ImageFrame(this, x, y, b, h, 1, toolSettings.dBrushPict, CommonStrings::None);
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
			newItem = new PageItem_LatexFrame(this, x, y, b, h, 1, toolSettings.dBrushPict, CommonStrings::None);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		default:
//			qDebug("unknown item type");
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
			x1=targetPage->xOffset()-bleeds.Left;
			y1=targetPage->yOffset()-bleeds.Top;
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
			w1=targetPage->width()+bleeds.Right+bleeds.Left;
			h1=targetPage->height()+bleeds.Bottom+bleeds.Top;
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
		z=itemAdd(iafData.frameType, PageItem::Unspecified, x1, y1, w1, h1, toolSettings.dWidth, CommonStrings::None, toolSettings.dPenText, true);
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
					currItem->IProfile = CMSSettings.DefaultImageRGBProfile;
					currItem->IRender = CMSSettings.DefaultIntentImages;
					qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
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
					qApp->restoreOverrideCursor();
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
			newItem->setFillShade(toolSettings.shadePict);
			newItem->setImageXYScale(toolSettings.scaleX, toolSettings.scaleY);
			newItem->ScaleType = toolSettings.scaleType;
			newItem->AspectRatio = toolSettings.aspectRatio;
			newItem->IProfile = CMSSettings.DefaultImageRGBProfile;
			newItem->IRender = CMSSettings.DefaultIntentImages;
			break;
		case PageItem::TextFrame:
//			newItem->setFontFillShade(toolSettings.dTextPenShade);
//			newItem->setFontStrokeShade(toolSettings.dTextStrokeShade);
			newItem->setFillColor(toolSettings.dTextBackGround);
			newItem->setFillShade(toolSettings.dTextBackGroundShade);
			newItem->setLineColor(toolSettings.dTextLineColor);
			newItem->setLineShade(toolSettings.dTextLineShade);
			break;
		case PageItem::Line:
			newItem->PLineArt = Qt::PenStyle(toolSettings.dLstyleLine);
			newItem->setLineShade(toolSettings.dShadeLine);
			break;
		case PageItem::Polygon:
			if(frameType!=PageItem::Rectangle && frameType!=PageItem::Ellipse)
			{
				newItem->ClipEdited = true;
				newItem->FrameType = 3;
			}
			break;
		case PageItem::PolyLine:
			newItem->ClipEdited = true;
			break;
		case PageItem::PathText:
		//At this point, we cannot create a PathText item like this, only by conversion, do nothing
			break;
		default:
			break;
	}

	if (frameType==PageItem::Rectangle || itemType==PageItem::TextFrame || itemType==PageItem::ImageFrame)
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
	if (itemType==PageItem::Polygon || itemType==PageItem::PolyLine)
	{
		newItem->PLineArt = Qt::PenStyle(toolSettings.dLineArt);
		newItem->setFillShade(toolSettings.dShade);
		newItem->setLineShade(toolSettings.dShade2);
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
	int itemsCount=Items->count();
	for (int i = 0; i < itemsCount; ++i)
		Items->at(i)->ItemNr = i;
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
		int layerNr = Layers.at(la).LNr;
		for (int it = 0; it < DocItems.count(); ++it)
		{
			currItem = DocItems.at(it);
			if (currItem->LayerNr != layerNr)
				continue;
			newDocItems.append(currItem);
			currItem->ItemNr = itemIndex++;
		}
		for (int it = 0; it < MasterItems.count(); ++it)
		{
			currItem = MasterItems.at(it);
			if (currItem->LayerNr != layerNr)
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
		if ((ScCore->fileWatcher->files().contains(pageItem->Pfile) != 0) && (pageItem->PicAvail))
			ScCore->fileWatcher->removeFile(pageItem->Pfile);
	}
	if(!pageItem->loadImage(fn, reload, -1, showMsg))
		return false;
	if (!reload)
		ScCore->fileWatcher->addFile(pageItem->Pfile);
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
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
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
		int x = static_cast<int>(currentPage()->xOffset() - bleeds.Left);
		int y = static_cast<int>(currentPage()->yOffset() - bleeds.Top);
		int w = static_cast<int>(currentPage()->width() + bleeds.Left + bleeds.Right);
		int h = static_cast<int>(currentPage()->height() + bleeds.Bottom + bleeds.Top);
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
		double x = currentPage()->xOffset() - bleeds.Left;
		double y = currentPage()->yOffset() - bleeds.Top;
		double w = currentPage()->width() + bleeds.Left + bleeds.Right;
		double h1 = currentPage()->height() + bleeds.Bottom + bleeds.Top;
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
	QList<PageItem*> Objects;
	PageItem* item;
	if (currItem->Groups.count() == 0)
		return;
	int ObjGroup = currItem->Groups.top();
	uint docItemCount=Items->count();
	for (uint a = 0; a < docItemCount; ++a)
	{
		item = Items->at(a);
		if (item->Groups.count() != 0)
		{
			if (item->Groups.top() == ObjGroup)
				Objects.append(item);
		}
	}
	int Off_Page = -1;
	int On_Page = 999999;
	uint objectCount=Objects.count();
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
	int counter = pageSets[currentPageLayout].FirstPage;
	int rowcounter = 0;
	double maxYPos=0.0, maxXPos=0.0;
	double currentXPos=scratch.Left, currentYPos=scratch.Top, lastYPos=Pages->at(0)->initialHeight();
//	currentXPos += (pageWidth+pageSets[currentPageLayout].GapHorizontal) * counter;
	currentXPos += (pageWidth+GapHorizontal) * counter;

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
			Seite->setXOffset(scratch.Left);
			Seite->setYOffset(scratch.Top);
			if (Seite->LeftPg == 0)
			{
				Seite->Margins.Right = Seite->initialMargins.Right;
				Seite->Margins.Left = Seite->initialMargins.Left;
			}
			else if ((Seite->LeftPg > 1) && (Seite->LeftPg < pageSets[currentPageLayout].Columns))
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
			if (counter < pageSets[currentPageLayout].Columns-1)
			{
//				currentXPos += Seite->width() + pageSets[currentPageLayout].GapHorizontal;
				currentXPos += Seite->width() + GapHorizontal;
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
				currentXPos = scratch.Left;
				if (pageSets[currentPageLayout].Columns > 1)
					currentYPos += qMax(lastYPos, Seite->height())+GapVertical;
//					currentYPos += qMax(lastYPos, Seite->height())+pageSets[currentPageLayout].GapVertical;
				else
					currentYPos += Seite->height()+GapVertical;
//					currentYPos += Seite->height()+pageSets[currentPageLayout].GapVertical;
//				lastYPos = qMax(lastYPos, Seite->height());
				lastYPos = 0;
				Seite->Margins.Right = Seite->initialMargins.Right;
				Seite->Margins.Left = Seite->initialMargins.Left;
			}
			counter++;
			if (counter > pageSets[currentPageLayout].Columns-1)
			{
				counter = 0;
				rowcounter++;
				if (rowcounter > pageSets[currentPageLayout].Rows-1)
				{
//					currentYPos += GapVertical;
//					currentYPos += pageSets[currentPageLayout].GapBelow;
					rowcounter = 0;
				}
			}
		}
		Seite->Margins.Top = Seite->initialMargins.Top;
		Seite->Margins.Bottom = Seite->initialMargins.Bottom;
		maxXPos = qMax(maxXPos, Seite->xOffset()+Seite->width()+scratch.Right);
		maxYPos = qMax(maxYPos, Seite->yOffset()+Seite->height()+scratch.Bottom);
	}
	if (!isLoading())
	{
		uint docItemsCount=Items->count();
		for (uint ite = 0; ite < docItemsCount; ++ite)
		{
			PageItem *item = Items->at(ite);
			if (item->OwnPage < 0)
			{
				if (item->Groups.count() != 0)
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
					if (item->Groups.count() != 0)
						GroupOnPage(item);
					else
						item->OwnPage = OnPage(item);
				}
			}
			item->setRedrawBounding();
		}
	}

	if(isLoading() && is12doc)
		return;
	if (!isLoading())
	{
		FPoint minPoint, maxPoint;
		canvasMinMax(minPoint, maxPoint);
		FPoint maxSize(qMax(maxXPos, maxPoint.x()+scratch.Right), qMax(maxYPos, maxPoint.y()+scratch.Bottom));
		adjustCanvas(FPoint(qMin(0.0, minPoint.x()-scratch.Left),qMin(0.0, minPoint.y()-scratch.Top)), maxSize, true);
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


void ScribusDoc::getBleeds(Page* page, MarginStruct& bleedData)
{
	bleedData.Bottom = bleeds.Bottom;
	bleedData.Top = bleeds.Top;
	if (pageSets[currentPageLayout].Columns == 1)
	{
		bleedData.Right = bleeds.Right;
		bleedData.Left = bleeds.Left;
	}
	else
	{
		if (locationOfPage(page->pageNr()) == LeftPage)
		{
			bleedData.Right = bleeds.Left;
			bleedData.Left = bleeds.Right;
		}
		else if (locationOfPage(page->pageNr()) == RightPage)
		{
			bleedData.Right = bleeds.Right;
			bleedData.Left = bleeds.Left;
		}
		else
		{
			bleedData.Right = bleeds.Left;
			bleedData.Left = bleeds.Left;
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
			if(oldItem->itemType()==PageItem::Line)
			{
				QMatrix ma;
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
	if (oldItem->Groups.count() != 0)
	{
		PageItem *currItem;
		for (int a = 0; a < Items->count(); ++a)
		{
			currItem = Items->at(a);
			if (currItem->isGroupControl)
			{
				if (currItem->groupsLastItem == oldItem)
					currItem->groupsLastItem = newItem;
			}
		}
	}
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
	if (changeToMasterPageMode==m_masterPageMode)
		return;
	if (changeToMasterPageMode)
	{
		Pages=&MasterPages;
		Items=&MasterItems;
	}
	else
	{
		Pages=&DocPages;
		Items=&DocItems;
	}
	m_masterPageMode=changeToMasterPageMode;
}


void ScribusDoc::addSection(const int number, const QString& name, const uint fromindex, const uint toindex, const DocumentSectionType type, const uint sectionstartindex, const bool reversed, const bool active)
{
	struct DocumentSection newSection;
	uint docPageCount=DocPages.count();
	bool empty=sections.isEmpty();
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
		sections.insert(newSection.number, newSection);
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
		sections.insert(newSection.number, newSection);
	}
}


bool ScribusDoc::deleteSection(const uint number)
{
	if (!sections.contains(number))
		return false;
	if (sections.count()<=1)
		return false;
	QMap<uint, DocumentSection>::Iterator itprev=sections.begin();
	QMap<uint, DocumentSection>::Iterator it=itprev;
	uint currMinIndex = itprev.value().fromindex;
	uint currMaxIndex = itprev.value().toindex;
	for ( ; it != sections.end(); ++it )
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
	sections.erase(it);
	return true;
}


int ScribusDoc::getSectionKeyForPageIndex(const uint pageIndex) const
{
	bool found=false;
	int retVal=-1;
	DocumentSectionMap::ConstIterator it = sections.begin();
	for (; it!= sections.end(); ++it)
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

	uint sectionIndexOffset=pageIndex-sections[key].fromindex+sections[key].sectionstartindex;
	//If a section is inactive, theres no page numbers printed
	if (sections[key].active==false)
		return "";
	retVal=getStringFromSequence(sections[key].type, sectionIndexOffset);
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
	//Get the section of the new page index.
	bool found=false;
	DocumentSectionMap::Iterator it = sections.begin();
	if (otherPageIndex==0)
		found=true;
	for (; it!= sections.end(); ++it)
	{
		if (otherPageIndex-1>=it.value().fromindex && otherPageIndex-1<=it.value().toindex)
		{
			found=true;
			break;
		}
	}
	Q_ASSERT(found);
	if (!found)
		return;
	DocumentSectionMap::Iterator it2(it);

	//For this if: We are adding before the beginning of a section, so we must put this
	//new page in the previous section and then increment the rest
	if (otherPageIndex-1==it.value().fromindex && location==0 && it!=sections.begin())
		--it2;
	it2.value().toindex+=count;
	++it2;
	while (it2!=sections.end())
	{
		it2.value().fromindex+=count;
		it2.value().toindex+=count;
		++it2;
	}
	//Now update the Pages' internal storage of their page number
	updateSectionPageNumbersToPages();
}


void ScribusDoc::removePageFromSection(const uint pageIndex)
{
	//Get the section of the new page index.
	bool found=false;
	DocumentSectionMap::Iterator it = sections.begin();
	for (; it!= sections.end(); ++it)
	{
		if (pageIndex>=it.value().fromindex && pageIndex<=it.value().toindex)
		{
			found=true;
			break;
		}
	}
	Q_ASSERT(found);
	if (!found)
		return;
	--it.value().toindex;
	++it;
	while (it!=sections.end())
	{
		--it.value().fromindex;
		--it.value().toindex;
		++it;
	}
	//Now update the Pages' internal storage of their page number
	updateSectionPageNumbersToPages();
}


void ScribusDoc::setFirstSectionFromFirstPageNumber()
{
	DocumentSectionMap::Iterator it = sections.begin();
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

	//CB Should we really be disabled auto text frames here?
	bool autoText = usesAutomaticTextFrames();
	setUsesAutomaticTextFrames(false);
	Page* from = DocPages.at(pageNumberToCopy);
	Page* lastDest = NULL;
	int GrMax = GroupCounter;
	for (int copyNumber=1; copyNumber<=copyCount; ++copyNumber)
	{
		//For multiple insertions we can insert in the same place
		int destLocation=existingPage;
		if (whereToInsert==0)
			--destLocation;
		else if (whereToInsert==2)
			destLocation=DocPages.count();
		Page* destination = new Page(scratch.Left, DocPages.count()*(pageHeight+scratch.Bottom+scratch.Top)+scratch.Top, pageWidth, pageHeight);
		destination->setDocument(this);
		destination->setPageNr(destLocation);
		lastDest = destination;
		DocPages.insert(destLocation, destination);
		setLocationBasedPageLRMargins(destLocation);
		applyMasterPage(from->MPageNam, destLocation);
		destination->setInitialHeight(from->height());
		destination->setInitialWidth(from->width());
		destination->PageOri = from->PageOri;
		destination->m_pageSize = from->m_pageSize;
		destination->initialMargins.Top = from->initialMargins.Top;
		destination->initialMargins.Bottom = from->initialMargins.Bottom;
		if (pageSets[currentPageLayout].Columns == 1)
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
		uint oldItems = Items->count();
		m_Selection->clear();
		m_Selection->delaySignalsOn();
		if (oldItems>0)
		{
			ScLayers::iterator it;
			if (Layers.count()!= 0)
			{
				int currActiveLayer = activeLayer();
				for (it = Layers.begin(); it != Layers.end(); ++it)
				{
					setActiveLayer((*it).LNr);
					for (uint ite = 0; ite < oldItems; ++ite)
					{
						PageItem *itemToCopy = Items->at(ite);
						if ((itemToCopy->OwnPage == static_cast<int>(from->pageNr())) && ((*it).LNr == itemToCopy->LayerNr))
							m_Selection->addItem(itemToCopy, true);
					}
					if (m_Selection->count() != 0)
					{
						ScriXmlDoc *ss = new ScriXmlDoc();
						ss->ReadElem(ss->WriteElem(this, view(), m_Selection), prefsData.AvailFonts, this, destination->xOffset(), destination->yOffset(), false, true, prefsData.GFontSub, view());
						m_Selection->clear();
						delete ss;
					}
				}
				setActiveLayer(currActiveLayer);
			}
		}
		m_Selection->delaySignalsOff();
		from->guides.copy(&destination->guides);
		GroupCounter = GrMax + 1;
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
	int setcol=pageSets[currentPageLayout].Columns;
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
	else if (myCol>= pageSets[currentPageLayout].Columns-1) // Right hand page
		return RightPage;
	else //Middle pages
		return MiddlePage;
}

int ScribusDoc::columnOfPage(int pageIndex) const
{
	int setcol=pageSets[currentPageLayout].Columns;
	return ((pageIndex % setcol) + pageSets[currentPageLayout].FirstPage) % setcol;
}


void ScribusDoc::updateAllItemQColors()
{
	for (int c=0; c<DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		ite->setLineQColor();
		ite->setFillQColor();
	}
	for (int c=0; c<MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		ite->setLineQColor();
		ite->setFillQColor();
	}
	for (int c=0; c<FrameItems.count(); ++c)
	{
		PageItem *ite = FrameItems.at(c);
		ite->setLineQColor();
		ite->setFillQColor();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *ite = pa.items.at(o);
			ite->setLineQColor();
			ite->setFillQColor();
		}
	}
}


//CB Moved from view
void ScribusDoc::RecalcPictures(ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia)
{
	uint docItemCount=Items->count();
	if ( docItemCount!= 0)
	{
		bool usingGUI=ScCore->usingGUI();
		int counter = 0;
		if (usingGUI && dia != NULL)
			counter = dia->value();
		PageItem* it;
		for (uint i=0; i < docItemCount; ++i)
		{
			it = Items->at(i);
			if ((it->itemType() == PageItem::ImageFrame) && (it->PicAvail))
			{
				if (it->pixm.imgInfo.colorspace == ColorSpaceCMYK)
				{
					if (!PrCMYK->contains(it->IProfile))
						it->IProfile = CMSSettings.DefaultImageCMYKProfile;
				}
				else
				{
					if (!Pr->contains(it->IProfile))
						it->IProfile = CMSSettings.DefaultImageRGBProfile;
				}
				LoadPict(it->Pfile, i, true);
			}
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
			if (((currItem->isSingleSel) && (currItem->isGroupControl)) || ((currItem->isSingleSel) && (currItem->isTableItem)))
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
			if (((currItem->isSingleSel) && (currItem->isGroupControl)) || ((currItem->isSingleSel) && (currItem->isTableItem)))
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
			//cb moved to setlinewidth
			//currItem->Oldm_lineWidth = currItem->lineWidth();
			currItem->setLineWidth(w);
			if (currItem->asPolyLine())
				currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
			if (currItem->asLine())
			{
				int ph = static_cast<int>(qMax(1.0, w / 2.0));
				currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
				                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
				                  -ph,static_cast<int>(currItem->height()+ph));
			}
			currItem->update();
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
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
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
									currItem->setFillColor(toolSettings.dBrushPict);
								case PageItem::TextFrame:
								case PageItem::PathText:
									currItem->setFillColor(toolSettings.dTextBackGround);
									break;
								case PageItem::Line:
								case PageItem::PolyLine:
								case PageItem::Polygon:
									currItem->setFillColor(toolSettings.dBrush);
									break;
								default:
									break;
							}
						}
					}
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
			if (typ != 8)
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

void ScribusDoc::itemSelection_SetItemPatternProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		m_updateManager.setUpdatesDisabled();
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setPatternTransform(scaleX, scaleY, offsetX, offsetY, rotation);
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
			storyStyle.setLineSpacing(((size / 10.0) * static_cast<double>(typographicSettings.autoLineSpacing) / 100) + (size / 10.0));
		}
		else if (storyStyle.lineSpacingMode() == 1)
		{
			storyStyle.setLineSpacing(storyStyle.charStyle().font().height(size));
		}
		else
		{
			storyStyle.setLineSpacing(typographicSettings.valueBaseGrid-1);
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

void ScribusDoc::itemSelection_ApplyParagraphStyle(const ParagraphStyle & newStyle, Selection* customSelection)
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
					currItem->itemText.applyStyle(pos, newStyle);
				}
			}
			currItem->itemText.applyStyle(stop, newStyle);
			currItem->invalid = true;
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.applyStyle(newStyle);
			currItem->itemText.setDefaultStyle(dstyle);
			if (currItem->asPathText())
			{
				for (int pos=0; pos < currItem->itemText.length(); ++pos)
				{
					if (currItem->itemText.text(pos) == SpecialChars::PARSEP)
					{
						currItem->itemText.applyStyle(pos, newStyle);
					}
				}
				currItem->itemText.applyStyle(currItem->itemText.length(), newStyle);
			}
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
	QMatrix ma;
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
//		currItem->Tinput = true;
//		currItem->FrameOnly = true;
//		currItem->paintObj();
//		currItem->FrameOnly = false;
//		m_View->MarkClip(currItem, currItem->ContourLine, true);
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
	QMatrix ma;
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
//		currItem->Tinput = true;
//		currItem->FrameOnly = true;
//		currItem->paintObj();
//		currItem->FrameOnly = false;
//		m_View->MarkClip(currItem, currItem->ContourLine, true);
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
	FPoint maxSize(currItem->BoundingX+currItem->BoundingW+scratch.Right, currItem->BoundingY+currItem->BoundingH+scratch.Bottom);
	FPoint minSize(currItem->BoundingX-scratch.Left, currItem->BoundingY-scratch.Top);
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
		connect(this, SIGNAL(docChanged()), m_ScMW, SLOT(slotDocCh()));
		connect(this, SIGNAL(firstSelectedItemType(int)), m_ScMW, SLOT(HaveNewSel(int)));
		connect(autoSaveTimer, SIGNAL(timeout()), WinHan, SLOT(slotAutoSave()));
//		connect(this, SIGNAL(refreshItem(PageItem*)), view(), SLOT(RefreshItem(PageItem*)));
//		connect(this, SIGNAL(updateContents()), view(), SLOT(slotUpdateContents()));
//		connect(this, SIGNAL(updateContents(const QRect&)), view(), SLOT(slotUpdateContents(const QRect&)));
//		connect(this, SIGNAL(canvasAdjusted(double, double, double, double)), view(), SLOT(adjustCanvas(double, double, double, double)));
	}
}


//CB Same as RecalcPicturesRes apart from the name checking, which should be able to be removed
void ScribusDoc::updatePict(QString name)
{
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->AdjustPictScale();
		}
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->AdjustPictScale();
		}
	}
	for (int a = 0; a <FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->AdjustPictScale();
		}
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if ((currItem->PicAvail) && (currItem->Pfile == name))
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->AdjustPictScale();
			}
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items);
	}
	regionsChanged()->update(QRectF());
	changed();
}


//CB Same as updatePict apart from the name checking, this should be able to be removed
void ScribusDoc::recalcPicturesRes(bool applyNewRes)
{
	int cc = 0;
	int ca = 0;
	m_ScMW->mainWindowProgressBar->reset();
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->PicAvail)
		  cc++;
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->PicAvail)
			cc++;
	}
	for (int a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->PicAvail)
			cc++;
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->PicAvail)
				cc++;
		}
	}
	m_ScMW->mainWindowProgressBar->setMaximum((cc > 0) ? cc : 1);
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->PicAvail)
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			if (applyNewRes)
				currItem->pixm.imgInfo.lowResType = toolSettings.lowResType;
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->AdjustPictScale();
			ca++;
			m_ScMW->mainWindowProgressBar->setValue(ca);
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
		}
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->PicAvail)
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			if (applyNewRes)
				currItem->pixm.imgInfo.lowResType = toolSettings.lowResType;
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->AdjustPictScale();
			ca++;
			m_ScMW->mainWindowProgressBar->setValue(ca);
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
		}
	}
	for (int a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->PicAvail)
		{
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			if (applyNewRes)
				currItem->pixm.imgInfo.lowResType = toolSettings.lowResType;
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->AdjustPictScale();
			ca++;
			m_ScMW->mainWindowProgressBar->setValue(ca);
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
		}
	}
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->PicAvail)
			{
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				if (applyNewRes)
					currItem->pixm.imgInfo.lowResType = toolSettings.lowResType;
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->AdjustPictScale();
				ca++;
				m_ScMW->mainWindowProgressBar->setValue(ca);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
			}
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items);
	}
	regionsChanged()->update(QRectF());
	changed();
	m_ScMW->mainWindowProgressBar->reset();
}


void ScribusDoc::removePict(QString name)
{
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = ScImage();
		}
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = ScImage();
		}
	}
	for (int a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = ScImage();
		}
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if ((currItem->PicAvail) && (currItem->Pfile == name))
			{
				currItem->PicAvail = false;
				currItem->pixm = ScImage();
			}
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items);
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
				} else if (m_Selection->itemAt(i)->asImageFrame())
				{
					PageItem *currItem = m_Selection->itemAt(i);
					if (currItem->PicAvail)
					{
						int fho = currItem->imageFlippedH();
						int fvo = currItem->imageFlippedV();
						loadPict(currItem->Pfile, currItem, true);
						currItem->setImageFlippedH(fho);
						currItem->setImageFlippedV(fvo);
						currItem->AdjustPictScale();
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
	int newLayerNr = 0;
	// Find the new layer identifier
	const ScLayer* lcurr = Layers.layerByNumber(l);
	if (lcurr)
	{
		const ScLayer* lbelow = Layers.layerBelow(lcurr->Level);
		if (lcurr == lbelow)
		{
			const ScLayer* labove = Layers.layerAbove(lcurr->Level);
			if (labove)
				newLayerNr = labove->LNr;
		}
		else if (lbelow)
			newLayerNr = lbelow->LNr;
	}
	for (int b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				tmpSelection.addItem(MasterItems.at(b));
				MasterItems.at(b)->setLocked(false);
			}
			else
				MasterItems.at(b)->setLayer(newLayerNr);
		}
	}
	if (tmpSelection.count() != 0)
		itemSelection_DeleteItem(&tmpSelection);
	tmpSelection.clear();
	for (int b = 0; b < DocItems.count(); ++b)
	{
		if (DocItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				tmpSelection.addItem(DocItems.at(b));
				DocItems.at(b)->setLocked(false);
			}
			else
				DocItems.at(b)->setLayer(newLayerNr);
		}
	}
	if (tmpSelection.count() != 0)
		itemSelection_DeleteItem(&tmpSelection);
	tmpSelection.clear();
	//FIXME signal these
	m_ScMW->rebuildLayersList();
	//FIXME: stop using m_View
	m_View->updateLayerMenu();
	setActiveLayer(newLayerNr);
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
	if (docSelectionCount != 0)
	{
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
}


void ScribusDoc::itemSelection_FlipH()
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
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
				if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
					currItem->flipImageH();
				if (currItem->itemType() != PageItem::Line)
					MirrorPolyH(currItem);
				currItem->moveBy(dx, 0, true);
				currItem->setRedrawBounding();
				currItem->GrStartX = currItem->width() - currItem->GrStartX;
				currItem->GrEndX = currItem->width() - currItem->GrEndX;
			}
			trans.commit();
		}
		else
		{
			for (uint a = 0; a < docSelectionCount; ++a)
			{
				PageItem* currItem=m_Selection->itemAt(a);
				if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
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
			}
		}
		regionsChanged()->update(QRectF());
		changed();
		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
}


void ScribusDoc::itemSelection_FlipV()
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
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
				if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
					currItem->flipImageV();
				if (currItem->itemType() != PageItem::Line)
					MirrorPolyV(currItem);
				currItem->moveBy(0, dx, true);
				currItem->setRedrawBounding();
				currItem->GrStartY = currItem->height() - currItem->GrStartY;
				currItem->GrEndY = currItem->height() - currItem->GrEndY;
			}
			regionsChanged()->update(QRectF());
			trans.commit();
		}
		else
		{
			for (uint a = 0; a < docSelectionCount; ++a)
			{
				PageItem* currItem=m_Selection->itemAt(a);
				if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
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
			}
			regionsChanged()->update(QRectF());
		}
		changed();
		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
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
				if ((ScCore->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->PicAvail))
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
	PageItem *currItem;
	uint offs = 0;
	QString tooltip = Um::ItemsInvolved + "\n";
	itemSelection->delaySignalsOn();
	for (uint de = 0; de < selectedItemCount; ++de)
	{
		currItem = itemSelection->itemAt(offs);
		if ((((currItem->isSingleSel) && (currItem->isGroupControl)) || ((currItem->isSingleSel) && (currItem->isTableItem))) || (currItem->locked()))
		{
			offs++;
			continue;
		}
		//CB FIXME remove this and include of story.h too
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
		currItem = delItems.last();
		if ((currItem->asImageFrame()) && ((ScCore->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->PicAvail)))
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
		Items->removeAll(currItem);
		delItems.removeLast();
		// send the undo action to the UndoManager
		if (UndoManager::undoEnabled())
		{
			ItemState<PageItem*> *is = new ItemState<PageItem*>(Um::Delete + " " + currItem->getUName(), "", Um::IDelete);
			is->setItem(currItem);
			is->set("DELETE_ITEM", "delete_item");
			UndoObject *target;
			if (currItem->OwnPage > -1)
				target = Pages->at(currItem->OwnPage);
			else
				target = Pages->at(0);
			undoManager->action(target, is, currItem->getUPixmap());
		}
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
	regionsChanged()->update(QRectF());
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	//CB FIXME remove this and tree.h too
//	m_ScMW->outlinePalette->BuildTree();

	if (itemSelection->isGUISelection())
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
			if (currItem->isGroupControl)
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
			if (currItem->isGroupControl)
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
	/*if (selectedItemCount>1)
		regionsChanged()->update(QRectF());*/
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


void ScribusDoc::itemSelection_SendToLayer(int layerNumber)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction* activeTransaction = NULL;
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			activeTransaction = new UndoTransaction(undoManager->beginTransaction());
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			currItem->setLayer(layerNumber);
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
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = itemSelection->itemAt(a);
			currItem->setImageXYOffset(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QMatrix cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
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
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = itemSelection->itemAt(a);
			currItem->setImageXYScale(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QMatrix cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
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
			QMatrix cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
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


void ScribusDoc::buildAlignItemList(Selection* customSelection)
{
	//CB TODO Handling custom selections
	//Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Selection* itemSelection = m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	PageItem *currItem;
	int ObjGroup;
	struct AlignObjs Object;
	AObjects.clear();
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		Object.Objects.clear();
		currItem->getBoundingRect(&Object.x1, &Object.y1, &Object.x2, &Object.y2);
		if (currItem->Groups.count() > 0)
		{
			ObjGroup = currItem->Groups.top();
			bool found = false;
			for (int a2 = 0; a2 < AObjects.count(); ++a2)
			{
				if (AObjects[a2].Group == ObjGroup)
				{
					AObjects[a2].x1 = qMin(AObjects[a2].x1, Object.x1);
					AObjects[a2].y1 = qMin(AObjects[a2].y1, Object.y1);
					AObjects[a2].x2 = qMax(AObjects[a2].x2, Object.x2);
					AObjects[a2].y2 = qMax(AObjects[a2].y2, Object.y2);
					AObjects[a2].Objects.append(currItem);
					found = true;
					break;
				}
			}
			if (!found)
			{
				Object.Group = ObjGroup;
				Object.ObjNr = 0;
				Object.Objects.append(currItem);
				AObjects.append(Object);
			}
		}
		else
		{
			Object.Group = 0;
			Object.ObjNr = currItem->ItemNr;
			Object.Objects.append(currItem);
			AObjects.append(Object);
		}
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
//		t = ScMessageBox::warning(m_ScMW, CommonStrings::trWarning, tr("Some objects are locked."),
//									tr("&Unlock All"), tr("&Skip locked objects"), CommonStrings::tr_Cancel, 0, 0);
//		if (t == 2)
//			return false;
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
	
	QString targetTooltip = Um::ItemsInvolved + "\n";
	for (int i = 0; i < m_Selection->count(); ++i)
		targetTooltip += "\t" + m_Selection->itemAt(i)->getUName() + "\n";
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


void ScribusDoc::itemSelection_AlignLeftOut(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = 99999.9;
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
	{
		double diff=newX-AObjects[i].x2;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(diff, 0.0);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignLeftIn(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = 99999.9;
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
	{
		double diff=newX-AObjects[i].x1;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(diff, 0.0);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignCenterHor(AlignTo currAlignTo, double guidePosition)
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
			double minX=99999.9, maxX=-99999.9;
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


void ScribusDoc::itemSelection_AlignRightIn(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = -99999.9;
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
	{
		double diff=newX-AObjects[i].x2;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(diff, 0.0);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignRightOut(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newX = -99999.9;
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
	{
		double diff=newX-AObjects[i].x1;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(diff, 0.0);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignTopOut(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = 99999.9;
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
	{
		double diff=newY-AObjects[i].y2;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(0.0, diff);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignTopIn(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = 99999.9;
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
	{
		double diff=newY-AObjects[i].y1;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(0.0, diff);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignCenterVer(AlignTo currAlignTo, double guidePosition)
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
			double minY=99999.9, maxY=-99999.9;
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


void ScribusDoc::itemSelection_AlignBottomIn(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = -99999.9;
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
	{
		double diff=newY-AObjects[i].y2;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(0.0, diff);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignBottomOut(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount-1;
	double newY = -99999.9;
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
	{
		double diff=newY-AObjects[i].y1;
		for (int j = 0; j < AObjects[i].Objects.count(); ++j)
			if (!AObjects[i].Objects.at(j)->locked())
				AObjects[i].Objects.at(j)->moveBy(0.0, diff);
	}
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


void ScribusDoc::itemSelection_DistributeDistH(bool usingDistance, double distance)
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


void ScribusDoc::itemSelection_DistributeDistV(bool usingDistance, double distance)
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


void ScribusDoc::changed()
{
	setModified(true);
	emit docChanged();
}

void ScribusDoc::invalidateRegion(QRectF region)
{
	for (int c=0; c<DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->getBoundingRect().intersects(region))
			ite->invalidateLayout();
	}
	for (int c=0; c<MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		// for now invalidate all masteritems, should be only necessary in masterpagemode
		ite->invalidateLayout();
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
			m_ScMW->guidePalette->setupPage();
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
		np.setX(static_cast<int>(qRound((in.x() - Pages->at(onp)->xOffset()) / guidesSettings.minorGrid) * guidesSettings.minorGrid + Pages->at(onp)->xOffset()));
		np.setY(static_cast<int>(qRound((in.y() - Pages->at(onp)->yOffset()) / guidesSettings.minorGrid) * guidesSettings.minorGrid + Pages->at(onp)->yOffset()));
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
		np.setX(qRound((in.x() - Pages->at(onp)->xOffset()) / guidesSettings.minorGrid) * guidesSettings.minorGrid + Pages->at(onp)->xOffset());
		np.setY(qRound((in.y() - Pages->at(onp)->yOffset()) / guidesSettings.minorGrid) * guidesSettings.minorGrid + Pages->at(onp)->yOffset());
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
	m_ScMW->ScriptRunning = true;
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
	m_ScMW->ScriptRunning = false;
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
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		PageItem *currItem = itemSelection->itemAt(a);
		if (!(currItem->asLine() || currItem->asPolyLine()))
			continue;
		if (startArrowID!=-1)
		{
			currItem->setStartArrowIndex(startArrowID);
		}
		if (endArrowID!=-1)
		{
			currItem->setEndArrowIndex(endArrowID);
		}
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


void ScribusDoc::createDefaultMasterPages()
{
	int setcol = pageSets[currentPageLayout].Columns;
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
	int setcol = pageSets[currentPageLayout].Columns;
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


void ScribusDoc::getClosestGuides(double xin, double yin, double *xout, double *yout, int *GxM, int *GyM)
{
	*GxM = -1;
	*GyM = -1;
	QMap<double, uint> tmpGuidesSel;
	Guides tmpGuides = currentPage()->guides.horizontals(GuideManagerCore::Standard);
	Guides::iterator it;
	uint yg = 0;
	uint xg = 0;
	double viewScale=m_View->scale();
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
	{
		if (((*it)+currentPage()->yOffset()< (yin+guidesSettings.guideRad / viewScale)) && ((*it)+currentPage()->yOffset()> (yin-guidesSettings.guideRad / viewScale)))
			tmpGuidesSel.insert(fabs(((*it)+currentPage()->yOffset()) - yin), yg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GyM = tmpGuidesSel.begin().value();
		*yout = tmpGuides[*GyM];
	}
	tmpGuidesSel.clear();
	tmpGuides = currentPage()->guides.verticals(GuideManagerCore::Standard);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
	{
		if (((*it)+currentPage()->xOffset()< (xin+guidesSettings.guideRad / viewScale)) && ((*it)+currentPage()->xOffset()> (xin-guidesSettings.guideRad / viewScale)))
			tmpGuidesSel.insert(fabs(((*it)+currentPage()->xOffset()) - xin), xg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GxM = tmpGuidesSel.begin().value();
		*xout = tmpGuides[*GxM];
	}
	yg = 0;
	xg = 0;
	tmpGuidesSel.clear();
	tmpGuides = currentPage()->guides.horizontals(GuideManagerCore::Auto);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
	{
		if (((*it)+currentPage()->yOffset()< (yin+guidesSettings.guideRad / viewScale)) && ((*it)+currentPage()->yOffset()> (yin-guidesSettings.guideRad / viewScale)))
			tmpGuidesSel.insert(fabs(((*it)+currentPage()->yOffset()) - yin), yg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GyM = tmpGuidesSel.begin().value();
		*yout = tmpGuides[*GyM];
	}
	tmpGuidesSel.clear();
	tmpGuides = currentPage()->guides.verticals(GuideManagerCore::Auto);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
	{
		if (((*it)+currentPage()->xOffset()< (xin+guidesSettings.guideRad / viewScale)) && ((*it)+currentPage()->xOffset()> (xin-guidesSettings.guideRad / viewScale)))
			tmpGuidesSel.insert(fabs(((*it)+currentPage()->xOffset()) - xin), xg);
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
		QMatrix ma;
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
		QMatrix ma;
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
		getClosestGuides(*x, *y, &xout, &yout, &GxM, &GyM);
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
		if ((page->Margins.Left+page->xOffset() < (*x+guidesSettings.guideRad * invViewScale)) && (page->Margins.Left+page->xOffset() > (*x-guidesSettings.guideRad * invViewScale)))
		{
			*x = page->Margins.Left+page->xOffset();
			ret = true;
		}
		if (((page->width() - page->Margins.Right)+page->xOffset() < (*x+guidesSettings.guideRad * invViewScale)) && ((page->width() - page->Margins.Right)+page->xOffset() > (*x-guidesSettings.guideRad * invViewScale)))
		{
			*x = page->width() - page->Margins.Right+page->xOffset();
			ret = true;
		}
		if ((page->Margins.Top+page->yOffset() < (*y+guidesSettings.guideRad * invViewScale)) && (page->Margins.Top+page->yOffset() > (*y-guidesSettings.guideRad * invViewScale)))
		{
			*y = page->Margins.Top+page->yOffset();
			ret = true;
		}
		if (((page->height() - page->Margins.Bottom)+page->yOffset() < (*y+guidesSettings.guideRad * invViewScale)) && ((page->height() - page->Margins.Bottom)+page->yOffset() > (*y-guidesSettings.guideRad * invViewScale)))
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
	QRect oldR(currItem->getRedrawBounding(m_View->scale()));
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
	if (RotMode != 0)
	{
		QMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.scale(1, 1);
		ma.rotate(currItem->rotation());
		double ro = angle - currItem->rotation();
		currItem->setRotation(angle);
		FPoint n(0,0);
		switch (RotMode)
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
	QMatrix ma;
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
	int ph = static_cast<int>(qMax(1.0, currItem->lineWidth() / 2.0));
	QMatrix ma;
	ma.rotate(currItem->rotation());
	double dX = ma.m11() * (currItem->width() - newX) + ma.m21() * (currItem->height() - newY) + ma.dx();
	double dY = ma.m22() * (currItem->height() - newY) + ma.m12() * (currItem->width() - newX) + ma.dy();
	currItem->setWidthHeight(newX, newY, true);
	if ((RotMode != 0) && (fromMP) && (!isLoading()) && (appMode == modeNormal))
	{
		double moveX=dX, moveY=dY;
		if (RotMode==2)
		{
			moveX/=2.0;
			moveY/=2.0;
		}
		else if (RotMode==3)
			moveX=0.0;
		else if (RotMode==1)
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
			currItem->setRotation(atan2(currItem->height(),currItem->width())*(180.0/M_PI));
			currItem->setWidthHeight(sqrt(pow(currItem->width(),2)+pow(currItem->height(),2)), 1.0);
		}
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
	}
	setRedrawBounding(currItem);
	currItem->OwnPage = OnPage(currItem);
	if (currItem->Sizing)
	{
//		currItem->Tinput = true;
		currItem->FrameOnly = true;
//		if ((m_View->frameResizeHandle == 1) && !(currItem->asLine()))
//			currItem->paintObj();

		if (currItem->FrameType == 0 || currItem->asLine())
			return true;
		
//		QPainter p;
//		p.begin(m_View->viewport());
//		QPoint in(qRound((currItem->xPos()-minCanvasCoordinate.x())*viewScale), qRound((currItem->yPos()-minCanvasCoordinate.y())*viewScale));
//		QPoint out(m_View->contentsToViewport(in));
//		p.translate(out.x(), out.y());
//		p.scale(viewScale, viewScale);
//		p.rotate(currItem->rotation());
//		p.setCompositionMode(QPainter::CompositionMode_Xor);
//		p.setBrush(Qt::NoBrush);
//		p.setPen(QPen(Qt::white, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
//		p.save();
//		if (currItem->OldB2 < 0.0)
//		{
//			p.scale(-1, 1);
//			p.translate(qRound(-currItem->OldB2), 0);
//		}
//		if (currItem->OldH2 < 0.0)
//		{
//			p.scale(1, -1);
//			p.translate(0, qRound(-currItem->OldH2));
//		}
//		currItem->DrawPolyL(&p, currItem->Clip);
//		p.restore();
		currItem->updateClip();
//		currItem->updateGradientVectors();
//		p.save();
//		if (currItem->width() < 0.0)
//		{
//			p.scale(-1, 1);
//			p.translate(qRound(-currItem->width()), 0);
//		}
//		if (currItem->height() < 0.0)
//		{
//			p.scale(1, -1);
//			p.translate(0, qRound(-currItem->height()));
//		}
//		currItem->DrawPolyL(&p, currItem->Clip);
//		p.restore();
//		p.end();
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
//			qDebug("doc, emit w&h, when was this used?");
			emit widthAndHeight(gw, gh);
		}
	}
	currItem->setCornerRadius(qMin(currItem->cornerRadius(), qMin(currItem->width(),currItem->height())/2));
	return true;
}


bool ScribusDoc::MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP, bool constrainRotation)
{
	PageItem *currItem = Items->at(ite);
	QRectF oldR(currItem->getBoundingRect());
	if (currItem->asLine())
	{
		QMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.dx();
		double my = ma.m12() * currItem->width() + ma.dy();
		MoveItem(newX.x(), newX.y(), currItem, fromMP);
		double newRot=xy2Deg(mx - currItem->xPos(), my - currItem->yPos());
		//CB Hmm should work, doesnt. (constraining on the first point of a line) FIXME
		//if (constrainRotation)
		//	qDebug(QString("%1").arg(constrainAngle(newRot)));
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
			QMatrix ma3;
			ma3.translate(currItem->xPos(), currItem->yPos());
			ma3.rotate(currItem->rotation());
			double mxc3 = currItem->xPos() - (ma3.m11() * npv.x() + ma3.m21() * npv.y() + ma3.dx());
			double myc3 = currItem->yPos() - (ma3.m22() * npv.y() + ma3.m12() * npv.x() + ma3.dy());
			SizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), ite, fromMP, true, false);
			MoveItem(-mxc3, -myc3, currItem, fromMP);
		}
		else
		{
			SizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), ite, fromMP, true, false);
			MoveItem(newX.x(), newX.y(), currItem, fromMP);
		}
	}
	return true;
}


void ScribusDoc::AdjustItemSize(PageItem *currItem)
{
	bool siz = currItem->Sizing;
	currItem->Sizing = false;
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
	currItem->ClipEdited = true;
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	currItem->PoLine = Clip.copy();
	if (currItem->asPolyLine())
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


void ScribusDoc::rotateGroup(double angle, FPoint RCenter)
{
	double gxS, gyS, ghS, gwS;
	double sc = 1; // FIXME:av Scale;
	PageItem* currItem;
	m_Selection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
	QMatrix ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(angle);
//	gxS -= minCanvasCoordinate.x();
//	gyS -= minCanvasCoordinate.y();
	QRect oldR = QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10));
	FPoint n;
	for (int a = 0; a < m_Selection->count(); ++a)
	{
		currItem = m_Selection->itemAt(a);
		n = FPoint(currItem->xPos() - RCenter.x(), currItem->yPos() - RCenter.y());
		currItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
		currItem->rotateBy(angle);
		setRedrawBounding(currItem);
	}
	currItem = m_Selection->itemAt(0);
	GroupOnPage(currItem);
	m_Selection->setGroupRect();
	m_Selection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
//	gxS -= minCanvasCoordinate.x();
//	gyS -= minCanvasCoordinate.y();
	regionsChanged()->update(QRectF(gxS-5, gyS-5, gwS+10, ghS+10).unite(oldR));
}

void ScribusDoc::scaleGroup(double scx, double scy, bool scaleText, Selection* customSelection)
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
	int drm = RotMode;
	RotMode = 0;
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
		bb->Sizing = false;
		double oldRot, oldLocalX, oldLocalY;
		oldRot = bb->rotation();
		oldLocalX = bb->imageXOffset();
		oldLocalY = bb->imageYOffset();
		FPointArray gr;
		gr.addPoint(bb->GrStartX, bb->GrStartY);
		gr.addPoint(bb->GrEndX, bb->GrEndY);
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
		if (bb->Groups.count() != 0)				// change the LineWidth only when the item is within a real Group
			bb->setLineWidth(qMax(bb->lineWidth()*((scx+scy)/2), 0.01));
		if (bb->itemType() == PageItem::Line)
		{
			bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
			bb->setWidth(sqrt(pow(t1.x()-b1.x(),2)+pow(t1.y()-b1.y(),2)));
			bb->setXYPos(b1.x()+gx, b1.y()+gy);
		}
		else
		{
			FPoint oldPos(bb->xPos(), bb->yPos());
			QMatrix ma;
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QMatrix ma2;
			ma2.translate(gx-bb->xPos(), gy-bb->yPos());
			ma2.scale(scx, scy);
			bb->PoLine.map(ma2);
			bb->setRotation(0.0);
			bb->ClipEdited = true;
			AdjustItemSize(bb);
			QMatrix ma3;
			ma3.translate(gx, gy);
			ma3.scale(scx, scy);
			FPoint n(gx-oldPos.x(), gy-oldPos.y());
			x = ma3.m11() * n.x() + ma3.m21() * n.y() + ma3.dx();
			y = ma3.m22() * n.y() + ma3.m12() * n.x() + ma3.dy();
			MoveItem(gx-x, gy-y, bb, true);
			if (oldRot != 0)
			{
				bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
				QMatrix ma;
				ma.rotate(-bb->rotation());
				bb->PoLine.map(ma);
				AdjustItemSize(bb);
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
		QMatrix ma4;
		ma4.rotate(oldRot);
		ma4.scale(scx, scy);
		gr.map(ma4);
		bb->ContourLine.map(ma4);
		bb->GrStartX = gr.point(0).x();
		bb->GrStartY = gr.point(0).y();
		bb->GrEndX = gr.point(1).x();
		bb->GrEndY = gr.point(1).y();
		bb->updateGradientVectors();
	}
	bb = itemSelection->itemAt(0);
	GroupOnPage(bb);
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	RotMode = drm;
	if ((RotMode != 0) && (!isLoading()))
	{
		switch (RotMode)
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



void ScribusDoc::itemSelection_GroupObjects(bool changeLock, bool lock, Selection* customSelection)
{
		Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
		if (itemSelection->count() < 1)
			return;
		PageItem *currItem;
		PageItem* bb;
		double x, y, w, h;
		QString tooltip = Um::ItemsInvolved + "\n";
		uint selectedItemCount=itemSelection->count();
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
						for (uint c=0; c<selectedItemCount; ++c)
						{
							bb = itemSelection->itemAt(c);
							bb->setLocked(lock);
							if (m_ScMW && ScCore->usingGUI())
								m_ScMW->scrActions["itemLock"]->setChecked(lock);
							tooltip += "\t" + currItem->getUName() + "\n";
						}
					}
				}
			}
		}
		itemSelection->getGroupRect(&x, &y, &w, &h);
		uint lowestItem = 999999;
		uint highestItem = 0;
		for (uint a=0; a<selectedItemCount; ++a)
		{
			currItem = itemSelection->itemAt(a);
			currItem->gXpos = currItem->xPos() - x;
			currItem->gYpos = currItem->yPos() - y;
			currItem->gWidth = w;
			currItem->gHeight = h;
			lowestItem = qMin(lowestItem, currItem->ItemNr);
			highestItem = qMax(highestItem, currItem->ItemNr);
		}
		double minx = 99999.9;
		double miny = 99999.9;
		double maxx = -99999.9;
		double maxy = -99999.9;
		for (uint ep = 0; ep < selectedItemCount; ++ep)
		{
			PageItem* currItem = itemSelection->itemAt(ep);
			double x1, x2, y1, y2;
			currItem->getBoundingRect(&x1, &y1, &x2, &y2);
			minx = qMin(minx, x1);
			miny = qMin(miny, y1);
			maxx = qMax(maxx, x2);
			maxy = qMax(maxy, y2);
/*
			double lw = 0.0;
			if (currItem->lineColor() != CommonStrings::None)
				lw = currItem->lineWidth() / 2.0;
			if (currItem->rotation() != 0)
			{
				FPointArray pb;
				pb.resize(0);
				pb.addPoint(FPoint(currItem->xPos()-lw, currItem->yPos()-lw));
				pb.addPoint(FPoint(currItem->width()+lw*2.0, -lw, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(currItem->width()+lw*2.0, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(-lw, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
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
				minx = qMin(minx, currItem->xPos()-lw);
				miny = qMin(miny, currItem->yPos()-lw);
				maxx = qMax(maxx, currItem->xPos()-lw + currItem->width()+lw*2.0);
				maxy = qMax(maxy, currItem->yPos()-lw + currItem->height()+lw*2.0);
			}
*/
		}
		double gx = minx;
		double gy = miny;
		double gw = maxx - minx;
		double gh = maxy - miny;
		PageItem *high = Items->at(highestItem);
		undoManager->setUndoEnabled(false);
		int z = itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, toolSettings.dBrush, toolSettings.dPen, true);
		PageItem *neu = Items->takeAt(z);
		Items->insert(lowestItem, neu);
		neu->setItemName( tr("Group%1").arg(GroupCounter));
		neu->AutoName = false;
		neu->isGroupControl = true;
		neu->groupsLastItem = high;
		undoManager->setUndoEnabled(true);

		QMap<int, uint> ObjOrder;
		for (uint c = 0; c < selectedItemCount; ++c)
		{
			currItem = itemSelection->itemAt(c);
			ObjOrder.insert(currItem->ItemNr, c);
			int d = Items->indexOf(currItem);
			Items->takeAt(d);
		}
		QList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			Items->insert(lowestItem+1, itemSelection->itemAt(Oindex[c]));
		}

		renumberItemsInListOrder();
		itemSelection->prependItem(neu);
		selectedItemCount=itemSelection->count();
		SimpleState *ss = new SimpleState(Um::Group, tooltip);
		ss->set("GROUP", "group");
		ss->set("itemcount", selectedItemCount);

		for (uint a=0; a<selectedItemCount; ++a)
		{
			currItem = itemSelection->itemAt(a);
			currItem->Groups.push(GroupCounter);
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
}

void ScribusDoc::itemSelection_UnGroupObjects(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() != 0)
	{
		uint docSelectionCount = itemSelection->count();
		PageItem *currItem;
		uint lowestItem = 999999;
		for (uint a=0; a < docSelectionCount; ++a)
		{
			currItem = itemSelection->itemAt(a);
			if (currItem->Groups.count() != 0)
				currItem->Groups.pop();
			lowestItem = qMin(lowestItem, currItem->ItemNr);
		}
		if (Items->at(lowestItem)->isGroupControl)
		{
			itemSelection->removeItem(Items->at(lowestItem));
			Items->removeAt(lowestItem);
			renumberItemsInListOrder();
		}
		docSelectionCount = itemSelection->count();
		SimpleState *ss = new SimpleState(Um::Ungroup);
		ss->set("UNGROUP", "ungroup");
		ss->set("itemcount", docSelectionCount);
		QString tooltip = Um::ItemsInvolved + "\n";
		emit docChanged();
		m_ScMW->HaveNewSel(itemSelection->itemAt(0)->itemType());
		itemSelection->connectItemToGUI();
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = itemSelection->itemAt(a);
			ss->set(QString("item%1").arg(a), currItem->uniqueNr);
			ss->set(QString("tableitem%1").arg(a), currItem->isTableItem);
			tooltip += "\t" + currItem->getUName() + "\n";
			currItem->isTableItem = false;
			currItem->setSelected(true);
		}
		undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
		double x, y, w, h;
		itemSelection->getGroupRect(&x, &y, &w, &h);
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
		if (currItem->Groups.count() != 0)
			return;
		m_Selection->delaySignalsOn();
		currItem->Frame = false;
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		for (uint a = 1; a < docSelectionCount; ++a)
		{
			bb = m_Selection->itemAt(a);
			toDel.append(bb->ItemNr);
			QMatrix ma;
			ma.translate(bb->xPos(), bb->yPos());
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QMatrix ma2;
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
	uint EndInd = currItem->PoLine.size();
	for (uint a = EndInd-1; a > 0; --a)
	{
		if (currItem->PoLine.point(a).x() > 900000)
		{
			StartInd = a + 1;
			bb = new PageItem_Polygon(*currItem);
			Items->append(bb);
			bb->ItemNr = Items->count()-1;
			bb->convertTo(PageItem::Polygon);
			bb->Frame = false;
			bb->FrameType = 3;
			bb->PoLine.resize(0);
			bb->PoLine.putPoints(0, EndInd - StartInd, currItem->PoLine, StartInd);
			bb->setRotation(currItem->rotation());
			AdjustItemSize(bb);
			bb->ContourLine = bb->PoLine.copy();
			bb->ClipEdited = true;
			a -= 3;
			EndInd = StartInd - 4;
		}
	}
	currItem->PoLine.resize(StartInd-4);
	AdjustItemSize(currItem);
	currItem->ContourLine = currItem->PoLine.copy();
	currItem->ClipEdited = true;
	m_Selection->delaySignalsOff();
	//FIXME: stop using m_View
	m_View->Deselect(true);
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_convertItemsTo(const PageItem::ItemType newType, Selection* customSelection)
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
			PageItem* newItem=convertItemTo(currItem, newType);
			if (newItem)
				newItem->update();
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
				if (currItem->asImageFrame() && currItem->PicAvail && !currItem->isTableItem)
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
				if (currItem->asImageFrame() && currItem->PicAvail && !currItem->isTableItem)
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
	//ScribusDoc* Doc = currItem->doc();
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
	//	RefreshItem(currItem);
	if (isContourLine)
	{
		//		MarkClip(currItem, currItem->ContourLine);
		Clip = currItem->ContourLine.copy();
	}
	else
	{
		//		MarkClip(currItem, currItem->PoLine);
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
		if ((np.x() < 0) && (!isContourLine))
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
		if ((np.y() < 0) && (!isContourLine))
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
		/*
		 {
			 qDebug(QString("is a text %1").arg((currItem->asTextFrame()!=0)));
			 qDebug(QString("is a image %1").arg((currItem->asImageFrame()!=0)));
			 qDebug(QString("is a line %1").arg((currItem->asLine()!=0)));
			 qDebug(QString("is a pathtext %1").arg((currItem->asPathText()!=0)));
			 qDebug(QString("is a polygon %1").arg((currItem->asPolygon()!=0)));
			 qDebug(QString("is a polyline %1").arg((currItem->asPolyLine()!=0)));
			 
			 qDebug(QString("item type is %1").arg(currItem->itemType()));
		 }
		 */
		if (((ClRe == static_cast<int>(StartInd)) || (ClRe == static_cast<int>(EndInd-2))) &&
			//		        ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame())))
			((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)))
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
			//		        ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame())) && (MoveSym))
			((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)) && (MoveSym))
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
		//		RefreshItem(currItem);
		//		MarkClip(currItem, Clip);
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
		Doc->AdjustItemSize(currItem);
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
		Clip.setPoint(StartInd+1, np);
		Clip.setPoint(EndInd-1, np);
	}
	else
	{
		Clip.setPoint(Doc->nodeEdit.ClRe+1, np);
		Clip.setPoint((Doc->nodeEdit.ClRe % 4 != 0 ? Doc->nodeEdit.ClRe + 3 : Doc->nodeEdit.ClRe - 1), np);
	}
	if (!Doc->nodeEdit.isContourLine)
	{
		currItem->PoLine = Clip.copy();
		Doc->AdjustItemSize(currItem);
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
			currItem->IRender = intentIndex;
			LoadPict(currItem->Pfile, currItem->ItemNr, true);
			currItem->update();
		}
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

QMap<PageItem*, QString> ScribusDoc::getDocItemNames(PageItem::ItemType itemType)
{
	QMap<PageItem*, QString> namesMap;
	uint docItemsCount=DocItems.count();
	for (uint i = 0; i < docItemsCount; ++i)
	{
		if (DocItems.at(i)->itemType() == itemType && DocItems.at(i)->nextInChain()==NULL && !DocItems.at(i)->isAutoFrame())
			namesMap.insert(DocItems.at(i), DocItems.at(i)->itemName());
	}
	return namesMap;
}
