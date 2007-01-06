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
#include "scribusdoc.moc"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribuswin.h"
#include "guidemanager.h"
#include "seiten.h"

#include <utility>
#include <qeventloop.h>

#include <qfile.h>
#include <qpainter.h>
#include <qprogressbar.h>

#include "fileloader.h"
#include "filewatcher.h"
//CBVTD
#include "hruler.h"
#include "hyphenator.h"
#include "layers.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#include "pagestructs.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scmessagebox.h"
#include "scpainter.h"
#include "scraction.h"
#include "scribusXml.h"
#include "selection.h"
#include "story.h"
#include "tree.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "util.h"
#include "commonstrings.h"
#include "sccolorengine.h"

#include "text/nlsconfig.h"

#include "cmsutil.h"
#include "cmserrorhandling.h"

extern QPixmap loadIcon(QString nam);

ScribusDoc::ScribusDoc() : UndoObject( tr("Document")),
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
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGuides(false), GuideLock(false),
	ScratchLeft(prefsData.ScratchLeft),
	ScratchRight(prefsData.ScratchRight),
	ScratchTop(prefsData.ScratchTop),
	ScratchBottom(prefsData.ScratchBottom),
	minCanvasCoordinate(FPoint(0, 0)),
	maxCanvasCoordinate(FPoint(ScratchLeft + ScratchRight, ScratchTop + ScratchBottom)),
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
	EditClip(false),
	EditClipMode(0),
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
	_itemCreationTransactionStarted(false)
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
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGuides(false), GuideLock(false),
	ScratchLeft(prefsData.ScratchLeft),
	ScratchRight(prefsData.ScratchRight),
	ScratchTop(prefsData.ScratchTop),
	ScratchBottom(prefsData.ScratchBottom),
	minCanvasCoordinate(FPoint(0, 0)),
	maxCanvasCoordinate(FPoint(ScratchLeft + ScratchRight, ScratchTop + ScratchBottom)),
	rulerXoffset(0.0), rulerYoffset(0.0),
	Pages(0), MasterPages(), DocPages(),
	MasterNames(),
	Items(0), MasterItems(), DocItems(), FrameItems(),
	m_Selection(new Selection(this, true)),
	pageWidth(pagesize.width()), pageHeight(pagesize.height()),
	pageMargins(margins),
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
	EditClip(false),
	EditClipMode(0),
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
	_itemCreationTransactionStarted(false)
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

	PageColors.insert("Black", ScColor(0, 0, 0, 255));
	PageColors.insert("White", ScColor(0, 0, 0, 0));
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
	pstyle.setName( tr("Default Paragraph Style"));
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
	pstyle.setUseBaselineGrid(false);
	pstyle.charStyle().setParent("");
	
	CharStyle cstyle;
	cstyle.setName( tr("Default Character Style"));
	cstyle.setFont(prefsData.AvailFonts[toolSettings.defFont]);
	cstyle.setFontSize(toolSettings.defSize);
	cstyle.setEffects(ScStyle_Default);
	cstyle.setFillColor(toolSettings.dBrush);
	cstyle.setFillShade(toolSettings.dShade);
	cstyle.setStrokeColor(toolSettings.dPen);
	cstyle.setStrokeShade(toolSettings.dShade2);
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
	docParagraphStyles[0].charStyle().setBase( & docCharStyles );

	currentStyle = pstyle;
	
	struct Layer ll;
	ll.LNr = 0;
	ll.Level = 0;
	ll.Name = tr("Background");
	ll.isViewable = true;
	ll.isPrintable = true;
	ll.isEditable = true;
	ll.flowControl = true;
	ll.outlineMode = false;
	ll.markerColor = QColor(0, 0, 0);
	ll.transparency = 1.0;
	ll.blendMode = 0;
	Layers.append(ll);
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
	DocItems.setAutoDelete(true);
	FrameItems.setAutoDelete(true);
	DocItems.clear();
	FrameItems.clear();
	MasterPages.setAutoDelete(true);
	DocPages.setAutoDelete(true);
	MasterPages.clear();
	DocPages.clear();
	MasterItems.setAutoDelete(true);
	MasterItems.clear();
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
		lpo.Frequency = 75;
		lpo.SpotFunc = 2;
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
	PageColors.insert("Black", ScColor(0, 0, 0, 255));
	PageColors.insert("White", ScColor(0, 0, 0, 0));
	PageColors.setDocument(this);

	CMSSettings = prefsManager->appPrefs.DCMSset;
	PDF_Options.SolidProf = CMSSettings.DefaultSolidColorRGBProfile;
	PDF_Options.ImageProf = CMSSettings.DefaultImageRGBProfile;
	PDF_Options.PrintProf = CMSSettings.DefaultPrinterProfile;
	PDF_Options.Intent = CMSSettings.DefaultIntentColors;
	PDF_Options.Intent2 = CMSSettings.DefaultIntentImages;
	SoftProofing = CMSSettings.SoftProofOn;
	Gamut = CMSSettings.GamutCheck;
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
			HasCMS = false;
	}
}

void ScribusDoc::setGUI(bool hasgui, ScribusMainWindow* mw, ScribusView* view)
{
	m_hasGUI = hasgui;
	m_ScMW=mw;
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

ScribusView* ScribusDoc::view() const
{
// 	return (WinHan ? (((ScribusWin*) WinHan)->view()) : 0);
	return m_View;
}

void ScribusDoc::CloseCMSProfiles()
{
	HasCMS = false;
	if (ScCore->haveCMS() /*&& CMSSettings.CMSinUse*/)
	{
		if (DocInputImageRGBProf)
			cmsCloseProfile(DocInputImageRGBProf);
		if (DocInputImageCMYKProf)
			cmsCloseProfile(DocInputImageCMYKProf);
		if (DocInputRGBProf)
			cmsCloseProfile(DocInputRGBProf);
		if (DocInputCMYKProf)
			cmsCloseProfile(DocInputCMYKProf);
		if (DocOutputProf)
			cmsCloseProfile(DocOutputProf);
		if (DocPrinterProf)
			cmsCloseProfile(DocPrinterProf);
		if (stdTransRGBMon)
			cmsDeleteTransform(stdTransRGBMon);
		if (stdTransCMYKMon)
			cmsDeleteTransform(stdTransCMYKMon);
		if (stdProof)
			cmsDeleteTransform(stdProof);
		if (stdTransImg)
			cmsDeleteTransform(stdTransImg);
		if (stdProofImg)
			cmsDeleteTransform(stdProofImg);
		if (stdTransCMYK)
			cmsDeleteTransform(stdTransCMYK);
		if (stdProofCMYK)
			cmsDeleteTransform(stdProofCMYK);
		if (stdTransRGB)
			cmsDeleteTransform(stdTransRGB);
		if (stdProofCMYKGC)
			cmsDeleteTransform(stdProofCMYKGC);
		if (stdProofGC)
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
			qWarning( "%s", message.local8Bit().data() );
		return false;
	}
	cmsSetErrorHandler(&cmsErrorHandler);
	const QCString rgbInputProfilePath(InPo[CMSSettings.DefaultSolidColorRGBProfile].local8Bit());
	DocInputRGBProf = cmsOpenProfileFromFile(rgbInputProfilePath.data(), "r");
	const QCString cmykInputProfilePath(InPoCMYK[CMSSettings.DefaultSolidColorCMYKProfile].local8Bit());
	DocInputCMYKProf = cmsOpenProfileFromFile(cmykInputProfilePath.data(), "r");
	const QCString monitorProfilePath(MoPo[CMSSettings.DefaultMonitorProfile].local8Bit());
	DocOutputProf = cmsOpenProfileFromFile(monitorProfilePath.data(), "r");
	const QCString printerProfilePath(PrPo[CMSSettings.DefaultPrinterProfile].local8Bit());
	DocPrinterProf = cmsOpenProfileFromFile(printerProfilePath, "r");
	const QCString rgbInputImgProfilePath(InPo[CMSSettings.DefaultImageRGBProfile].local8Bit());
	DocInputImageRGBProf = cmsOpenProfileFromFile(rgbInputImgProfilePath.data(), "r");
	const QCString cmykInputImgProfilePath(InPoCMYK[CMSSettings.DefaultImageCMYKProfile].local8Bit());
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
	m_ScMW->mainWindowProgressBar->setTotalSteps(cc);
	HasCMS = CMSSettings.CMSinUse;
	SoftProofing = CMSSettings.SoftProofOn;
	Gamut = CMSSettings.GamutCheck;
	IntentColors = CMSSettings.DefaultIntentColors;
	IntentImages = CMSSettings.DefaultIntentImages;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	bool oldCM = CMSSettings.CMSinUse;
	bool newCM = enable;
	CloseCMSProfiles();
	CMSSettings.CMSinUse = newCM;
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
		HasCMS = false;
	m_ScMW->mainWindowProgressBar->setProgress(cc);
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	m_ScMW->setStatusBarInfoText("");
	m_ScMW->mainWindowProgressBar->reset();
}


void ScribusDoc::replaceStyles(QMap<QString,QString> newNameForOld)
{
	// replace names in items
	QPtrList<PageItem> * itemlist = & MasterItems;
	while (itemlist != NULL)
	{
		for (uint i=0; i < itemlist->count(); ++i)
		{
			PageItem_TextFrame * currItem = itemlist->at(i)->asTextFrame();
			if (currItem)
				currItem->itemText.replaceStyles(newNameForOld);
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
		// ... as parent
		const QString& parent(docParagraphStyles[i].parent());
		if (newNameForOld.contains(parent))
			docParagraphStyles[i].setParent(newNameForOld[parent]);
		// ... as name
		if (newNameForOld.contains(docParagraphStyles[i].name()))
			docParagraphStyles.remove(i);
	}
}


void ScribusDoc::replaceCharStyles(QMap<QString,QString> newNameForOld)
{
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
}

void ScribusDoc::redefineStyles(const StyleSet<ParagraphStyle>& newStyles, bool removeUnused)
{
	docParagraphStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (uint i=0; i < docParagraphStyles.count(); ++i)
		{
			const QString& nam(docParagraphStyles[i].name());
			if (newStyles.find(nam) < 0)
				deletion[nam] = deflt;
		}
		if (deletion.count() > 0)
			replaceStyles(deletion);
	}
}

void ScribusDoc::redefineCharStyles(const StyleSet<CharStyle>& newStyles, bool removeUnused)
{
	docCharStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (uint i=0; i < docCharStyles.count(); ++i)
		{
			const QString& nam(docCharStyles[i].name());
			if (newStyles.find(nam) < 0)
				deletion[nam] = deflt;
		}
		if (deletion.count() > 0)
			replaceCharStyles(deletion);
	}
}

/*
 * Split out from loadStyles in editFormats.cpp so it's callable from anywhere,
 * including plugins.
 * - 2004-09-14 Craig Ringer
 */
void ScribusDoc::loadStylesFromFile(QString fileName, QValueList<ParagraphStyle> *tempStyles)
{
	StyleSet<ParagraphStyle> *wrkStyles = NULL;
	/*
	 * Use the working styles struct if passed, or work directly
	 * on the document styles otherwise. 
	 */
	if (tempStyles != NULL) {
		wrkStyles = new StyleSet<ParagraphStyle>;
	}
	else {
		wrkStyles = &docParagraphStyles; 
	}
	if (!fileName.isEmpty())
	{
		FileLoader fl(fileName);
		if (fl.TestFile() == -1)
		//TODO put in nice user warning
			return;

		if (fl.ReadStyles(fileName, this, *wrkStyles))
		{
			if (tempStyles != NULL) {
				for(uint i=0; i < wrkStyles->count(); ++i)
					tempStyles->append((*wrkStyles)[i]);
				delete wrkStyles;
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
	QString uname = name;
	if (name.find("/") > -1)
		uname = name.right(name.length() - name.findRev("/") - 1);
	if (uname.find(".sl") > -1)
		uname = uname.left(uname.findRev(".sl"));
	setUName(uname); // set the name for the undo object
}

void ScribusDoc::setModified(const bool isModified)
{
	modified = isModified;
}

bool ScribusDoc::isModified() const
{
  return modified;
}
/** Setzt die Seitenattribute */
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

void ScribusDoc::resetPage(double t, double l, double r, double bo, int fp)
{
	pageMargins.Top = t;
	pageMargins.Left = l;
	pageMargins.Right = r;
	pageMargins.Bottom = bo;
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
	Q_ASSERT(masterPageMode()==false);
	Page* addedPage = new Page(ScratchLeft, DocPages.count()*(pageHeight+ScratchBottom+ScratchTop)+ScratchTop, pageWidth, pageHeight);
	Q_ASSERT(addedPage!=NULL);
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
	bool insertsuccess=DocPages.insert(pageIndex, addedPage);
	Q_ASSERT(insertsuccess==true && DocPages.at(pageIndex)!=NULL);
	setCurrentPage(addedPage);
	//if (!masterPageMode())
	if (!masterPageName.isEmpty())
		applyMasterPage(masterPageName, pageIndex);
	setLocationBasedPageLRMargins(pageIndex);
	if (addAutoFrame && automaticTextFrames)
		addAutomaticTextFrame(pageIndex);
	changed();
	return addedPage;
}

Page* ScribusDoc::addMasterPage(const int pageNumber, const QString& pageName)
{
	//CB We dont create master pages (yet) with a pageCount based location
	//Page* addedPage = new Page(ScratchLeft, MasterPages.count()*(pageHeight+ScratchBottom+ScratchTop)+ScratchTop, pageWidth, pageHeight);
	Page* addedPage = new Page(ScratchLeft, ScratchTop, pageWidth, pageHeight);
	Q_ASSERT(addedPage!=NULL);
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
	addedPage->MPageNam = "";
	addedPage->setPageName(pageName);
	addedPage->setPageNr(pageNumber);
	MasterNames.insert(pageName, pageNumber);
	bool insertsuccess=MasterPages.insert(pageNumber, addedPage);
	Q_ASSERT(insertsuccess==true && MasterPages.at(pageNumber)!=NULL);
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
		for (Page* docPage = DocPages.first(); docPage; docPage = DocPages.next() )
		{
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
	Q_ASSERT( Pages->count() > 1 && Pages->count() > static_cast<uint>(pageNumber) );
	Page* page = Pages->at(pageNumber);
	QString oldPageName(page->pageName());
	Pages->remove(pageNumber);
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
	setCurrentPage(Pages->at(0));
	changed();
}

void ScribusDoc::deletePage(const int pageNumber)
{
	Q_ASSERT( Pages->count() > 1 && Pages->count() > static_cast<uint>(pageNumber) );
	Page* page = Pages->at(pageNumber);
	Pages->remove(pageNumber);
	delete page;
	setCurrentPage(Pages->at(0));
	changed();
}

void ScribusDoc::movePage(const int from, const int to, const int ziel, const int art)
{
	QPtrList<Page> Buf;
	int zz = ziel;
	Buf.clear();
	for (int a = from; a < to; ++a)
	{
		Buf.append(Pages->at(from));
		Pages->remove(from);
		if (a <= zz)
			--zz;
	}
	uint bufCount=Buf.count();
	switch (art)
	{
		case 0:
			for (uint b = 0; b < bufCount; ++b)
				Pages->insert(zz++, Buf.at(b));
			break;
		case 1:
			for (uint b = 0; b < bufCount; ++b)
				Pages->insert(++zz, Buf.at(b));
			break;
		case 2:
			for (uint b = 0; b < bufCount; ++b)
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
		Items->at(z)->BackBox = LastAuto;
		Items->at(z)->Cols = qRound(PageSp);
		Items->at(z)->ColGap = PageSpa;
		if (LastAuto != 0) {
			LastAuto->NextBox = Items->at(z);
			Items->at(z)->itemText = LastAuto->itemText;
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
	struct Layer ll;
	ll.LNr = Layers.last().LNr + 1;
	ll.Level = Layers.count();
	if (layerName.isNull() || layerName.isEmpty())
	{
		QString tmp;
		ll.Name = tr("New Layer")+" "+tmp.setNum(ll.LNr);
	}
	else
		ll.Name = layerName;
	ll.isViewable = true;
	ll.isPrintable = true;
	ll.isEditable = true;
	ll.flowControl = true;
	ll.outlineMode = false;
	ll.transparency = 1.0;
	ll.blendMode = 0;
	QColor marker;
	switch (ll.LNr % 7)
	{
		case 0:
			marker = Qt::black;
			break;
		case 1:
			marker = Qt::red;
			break;
		case 2:
			marker = Qt::green;
			break;
		case 3:
			marker = Qt::blue;
			break;
		case 4:
			marker = Qt::cyan;
			break;
		case 5:
			marker = Qt::magenta;
			break;
		case 6:
			marker = Qt::yellow;;
			break;
	}
	ll.markerColor = marker;
	Layers.append(ll);
	if (activate)
		setActiveLayer(ll.LNr);

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::AddLayer, "", Um::ICreate);
		ss->set("ADD_LAYER", "add_layer");
		ss->set("ACTIVE", ActiveLayer);
		ss->set("NAME", ll.Name);
		ss->set("LAYER_NR", ll.LNr);
		undoManager->action(this, ss, DocName, Um::ILayer);
	}
	return ll.LNr;
}

void ScribusDoc::copyLayer(int layerNumberToCopy, int whereToInsert)
{
	int GrMax = GroupCounter;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	TableID.clear();
	TableItems.clear();
	uint oldItems = Items->count();
	QPixmap pgPix(10, 10);
	QRect rd = QRect(0,0,9,9);
	ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
	RePos = true;
	for (uint ite = 0; ite < oldItems; ++ite)
	{
		PageItem *itemToCopy = Items->at(ite);
		if (itemToCopy->LayerNr == layerNumberToCopy)
		{
			struct CopyPasteBuffer Buffer;
			itemToCopy->copyToCopyPasteBuffer(&Buffer);
			if (itemToCopy->Groups.count() != 0)
			{
				Buffer.Groups.clear();
				QValueStack<int>::Iterator nx;
				QValueStack<int> tmpGroup;
				for (nx = itemToCopy->Groups.begin(); nx != itemToCopy->Groups.end(); ++nx)
				{
					tmpGroup.push((*nx)+GroupCounter);
					GrMax = QMAX(GrMax, (*nx)+GroupCounter);
				}
				for (nx = tmpGroup.begin(); nx != tmpGroup.end(); ++nx)
				{
					Buffer.Groups.push((*nx));
				}
			}
			m_View->PasteItem(&Buffer, true, true);
			PageItem* Neu = Items->at(Items->count()-1);
			Neu->LayerNr = whereToInsert;
			Neu->OnMasterPage = "";
			if (itemToCopy->isBookmark)
				m_ScMW->AddBookMark(Neu);
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(ite, Neu->ItemNr);
			}
			bool upDtImg = false;
			if (itemToCopy->pixm.imgInfo.valid)
			{
				Neu->pixm.imgInfo = itemToCopy->pixm.imgInfo;
				upDtImg = true;
			}
			if (itemToCopy->effectsInUse.count() != 0)
			{
				Neu->effectsInUse = itemToCopy->effectsInUse;
				upDtImg = true;
			}
			if (upDtImg)
			{
				int fho = Neu->imageFlippedH();
				int fvo = Neu->imageFlippedV();
				loadPict(Neu->Pfile, Neu, true);
				Neu->setImageFlippedH(fho);
				Neu->setImageFlippedV(fvo);
				Neu->AdjustPictScale();
			}
			Neu->DrawObj(painter, rd);
		}
	}
	delete painter;
	RePos = false;
	if (TableItems.count() != 0)
	{
		for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	GroupCounter = GrMax + 1;
	changed();
}

bool ScribusDoc::deleteLayer(const int layerNumber, const bool deleteItems)
{
	if (Layers.count() < 2)
		return false;
	QValueList<Layer>::iterator it2;
	QValueList<Layer>::iterator it2end=Layers.end();
	bool found=false;
	int layerLevel = -1;
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if ((*it2).LNr == layerNumber)
		{
			layerLevel=(*it2).Level;
			found=true;
			break;
		}
	}
	if (!found)
		return false;
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction("Layer", Um::IDocument, Um::DeleteLayer, "", Um::IDelete);

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
				DocItems.at(b)->setLocked(false);
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

	QString name = (*it2).Name;
	Layers.remove(it2);
	QValueList<Layer>::iterator it;
	QValueList<Layer>::iterator itend=Layers.end();
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).Level > layerLevel)
			(*it).Level -= 1;
	}
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::DeleteLayer, "", Um::IDelete);
		ss->set("REMOVE_LAYER", "remove_layer");
		ss->set("ACTIVE", layerNumber);
		ss->set("LEVEL", layerLevel);
		ss->set("NAME", name);
		ss->set("LAYER_NR", layerNumber);
		ss->set("DELETE", deleteItems);
		undoManager->action(this, ss, DocName, Um::ILayer);
		undoManager->commit();
	}
	setActiveLayer(layerNumberFromLevel(0));
	return true;
}

int ScribusDoc::activeLayer()
{
	return ActiveLayer;
}

const QString& ScribusDoc::activeLayerName()
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == ActiveLayer)
		{
			found=true;
			break;
		}
	}
	Q_ASSERT(found);
	return (*it).Name;
}

bool ScribusDoc::setActiveLayer(const int layerToActivate)
{
	bool found=false;
	uint layerCount=Layers.count();

	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerToActivate)
		{
			found = true;
			break;
		}
	}
	Q_ASSERT(found);
	if (found)
		ActiveLayer=layerToActivate;
	return found;
}

bool ScribusDoc::setActiveLayer(const QString& layerNameToActivate)
{
	bool found=false;
	uint layerCount=Layers.count();
	uint i;
	for (i=0; i < layerCount; ++i)
	{
		if (Layers[i].Name == layerNameToActivate)
		{
			found = true;
			break;
		}
	}
	if (found)
		ActiveLayer=Layers[i].LNr;
	return found;
}

bool ScribusDoc::setLayerPrintable(const int layerNumber, const bool isPrintable)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).isPrintable;
	}
	return false;
}

bool ScribusDoc::setLayerVisible(const int layerNumber, const bool isViewable)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).isViewable;
	}
	return false;
}

bool ScribusDoc::setLayerLocked(const int layerNumber, const bool isLocked)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return !(*it).isEditable;
	}
	return false;
}

bool ScribusDoc::setLayerFlow(const int layerNumber, const bool flow)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).flowControl;
	}
	return false;
}

bool ScribusDoc::setLayerTransparency(const int layerNumber, double trans)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).transparency;
	}
	return 1.0;
}

bool ScribusDoc::setLayerBlendMode(const int layerNumber, int blend)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).blendMode;
	}
	return 0;
}

bool ScribusDoc::setLayerOutline(const int layerNumber, const bool outline)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).LNr == layerNumber)
			return (*it).outlineMode;
	}
	return false;
}

bool ScribusDoc::setLayerMarker(const int layerNumber, QColor color)
{
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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
	QValueList<Layer>::iterator itend=Layers.end();
	QValueList<Layer>::iterator it;
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

	QValueList<Layer>::iterator it;
	QValueList<Layer>::iterator itend=Layers.end();
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).Level == layerLevel-1)
			break;
	}
	QValueList<Layer>::iterator it2;
	QValueList<Layer>::iterator it2end=Layers.end();
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

	QValueList<Layer>::iterator it;
	QValueList<Layer>::iterator itend=Layers.end();
	for (it = Layers.begin(); it != itend; ++it)
	{
		if ((*it).Level == layerLevel+1)
			break;
	}
	QValueList<Layer>::iterator it2;
	QValueList<Layer>::iterator it2end=Layers.end();
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if ((*it2).Level == layerLevel)
			break;
	}
	(*it2).Level += 1;
	(*it).Level -= 1;
	return true;
}

const QString& ScribusDoc::layerName(const int layerNumber) const
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
			QValueList<Layer>::iterator itend=Layers.end();
			for (QValueList<Layer>::iterator it = Layers.begin(); it != itend; ++it)
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
	for (uint b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->isTagged())
		{
		}
	}
	//Doc Items
	for (uint b = 0; b < DocItems.count(); ++b)
	{
		if (DocItems.at(b)->isTagged())
		{
		}
	}
	return true;
}

void ScribusDoc::getUsedColors(ColorList &colorsToUse, bool spot)
{
	PageItem* ite;
	bool found;
	colorsToUse.clear();
	colorsToUse.setDocument(this);
	ColorList::Iterator it;
	for (it = PageColors.begin(); it != PageColors.end(); ++it)
	{
		found = false;
		if ((it.key() == toolSettings.dBrush) || (it.key() == toolSettings.dPen) || (it.key() == toolSettings.dBrushPict)
		        || (it.key() == toolSettings.dPenLine) || (it.key() == toolSettings.dPenText))
		{
			if (spot)
			{
				if (it.data().isSpotColor())
					colorsToUse.insert(it.key(), it.data());
			}
			else
				colorsToUse.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < MasterItems.count(); ++c)
		{
			ite = MasterItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
			{
				for (int d=0; d<ite->itemText.length(); ++d)
				{
					if (it.key() == ite->itemText.charStyle(d).fillColor())
						found = true;
					if (it.key() == ite->itemText.charStyle(d).strokeColor())
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - merged if's to one line */
			if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
				found = true;
			if (found)
				break;
		}
		if (found)
		{
			if (spot)
			{
				if (it.data().isSpotColor())
					colorsToUse.insert(it.key(), it.data());
			}
			else
				colorsToUse.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < DocItems.count(); ++c)
		{
			ite = DocItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
			{
				for (int d=0; d<ite->itemText.length(); ++d)
				{
					/* PFJ - 29.02.04 - Merged if's */
					if ((it.key() == ite->itemText.charStyle(d).fillColor()) || (it.key() == ite->itemText.charStyle(d).strokeColor()))
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - Merged if's */
			if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
				found = true;
			if (found)
				break;
		}
		if (found)
		{
			if (spot)
			{
				if (it.data().isSpotColor())
					colorsToUse.insert(it.key(), it.data());
			}
			else
				colorsToUse.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < FrameItems.count(); ++c)
		{
			ite = FrameItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
			{
				for (int d=0; d<ite->itemText.length(); ++d)
				{
					/* PFJ - 29.02.04 - Merged if's */
					if ((it.key() == ite->itemText.charStyle(d).fillColor()) || (it.key() == ite->itemText.charStyle(d).strokeColor()))
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - Merged if's */
			if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
				found = true;
			if (found)
				break;
		}
		if (found)
		{
			if (spot)
			{
				if (it.data().isSpotColor())
					colorsToUse.insert(it.key(), it.data());
			}
			else
				colorsToUse.insert(it.key(), it.data());
			continue;
		}
		QStringList patterns = getUsedPatterns();
		for (uint c = 0; c < patterns.count(); ++c)
		{
			ScPattern pa = docPatterns[patterns[c]];
			for (uint o = 0; o < pa.items.count(); o++)
			{
				ite = pa.items.at(o);
				QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
				for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
				{
					if (it.key() == cstops.at(cst)->name)
						found = true;
					if (found)
						break;
				}
				if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
				{
					for (int d=0; d<ite->itemText.length(); ++d)
					{
						/* PFJ - 29.02.04 - Merged if's */
						if ((it.key() == ite->itemText.charStyle(d).fillColor()) || (it.key() == ite->itemText.charStyle(d).strokeColor()))
							found = true;
						if (found)
							break;
					}
				}
				/* PFJ - 29.02.04 - Merged if's */
				if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
					found = true;
				if (found)
					break;
			}
			if (found)
			{
				if (spot)
				{
					if (it.data().isSpotColor())
						colorsToUse.insert(it.key(), it.data());
				}
				else
					colorsToUse.insert(it.key(), it.data());
				continue;
			}
		}
	}
}

bool ScribusDoc::addPattern(QString &name, ScPattern& pattern)
{
	docPatterns.insert(name, pattern);
	return true;
}

void ScribusDoc::setPatterns(QMap<QString, ScPattern> &patterns)
{
	docPatterns.clear();
	docPatterns=patterns;
// 	for (QMap<QString, ScPattern>::Iterator it = patterns.begin(); it != patterns.end(); ++it)
// 	{
// 		docPatterns.insert(it.key(), it.data());
// 	}
	for (uint c=0; c<DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if ((!docPatterns.contains(ite->pattern())) && (ite->GrType == 8))
		{
			ite->setPattern("");
			ite->GrType = 0;
		}
	}
	for (uint c=0; c<MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if ((!docPatterns.contains(ite->pattern())) && (ite->GrType == 8))
		{
			ite->setPattern("");
			ite->GrType = 0;
		}
	}
	for (uint c=0; c<FrameItems.count(); ++c)
	{
		PageItem *ite = FrameItems.at(c);
		if ((!docPatterns.contains(ite->pattern())) && (ite->GrType == 8))
		{
			ite->setPattern("");
			ite->GrType = 0;
		}
	}
}

QStringList ScribusDoc::getUsedPatterns()
{
	QStringList results;
	for (uint c = 0; c < MasterItems.count(); ++c)
	{
		if ((!results.contains(MasterItems.at(c)->pattern())) && (MasterItems.at(c)->GrType == 8))
			results.append(MasterItems.at(c)->pattern());
	}
	for (uint c = 0; c < DocItems.count(); ++c)
	{
		if ((!results.contains(DocItems.at(c)->pattern())) && (DocItems.at(c)->GrType == 8))
			results.append(DocItems.at(c)->pattern());
	}
	for (uint c = 0; c < FrameItems.count(); ++c)
	{
		if ((!results.contains(FrameItems.at(c)->pattern())) && (FrameItems.at(c)->GrType == 8))
			results.append(FrameItems.at(c)->pattern());
	}
	for (uint c = 0; c < results.count(); ++c)
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
			if ((!results.contains(currItem->pattern())) && (currItem->GrType == 8))
				results.append(currItem->pattern());
		}
		for (uint c = 0; c < results.count(); ++c)
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
	for (uint c = 0; c < pat->items.count(); ++c)
	{
		if ((!results.contains(pat->items.at(c)->pattern())) && (pat->items.at(c)->GrType == 8))
			pats.append(pat->items.at(c)->pattern());
	}
	if (!pats.isEmpty())
	{
		for (uint c = 0; c < pats.count(); ++c)
		{
			getUsedPatternsHelper(pats[c], results);
		}
	}
	return pats;
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
	for (uint c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (uint o = 0; o < pa.items.count(); o++)
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
				if (! Really.contains(it->itemText.charStyle(e).font().replacementName()) ) {
					Really.insert(it->itemText.charStyle(e).font().replacementName(), QMap<uint, FPointArray>());
			}
			uint chr = it->itemText.text(e).unicode();
			if ((chr == 13) || (chr == 32) || (chr == 29))
				continue;
			if (chr == 9)
			{
				for (uint t1 = 0; t1 < it->itemText.paragraphStyle(e).tabValues().count(); t1++)
				{
					if (it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar.isNull())
						continue;
					chstr = QString(it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar);
					if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						if (chstr.upper() != QString(it->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar))
							chstr = chstr.upper();
					}
					chr = chstr[0].unicode();
					uint gl = it->itemText.charStyle(e).font().char2CMap(chstr[0]);
					gly = it->itemText.charStyle(e).font().glyphOutline(gl);
					Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
				}
				for (uint t1 = 0; t1 < it->TabValues.count(); t1++)
				{
					if (it->TabValues[t1].tabFillChar.isNull())
						continue;
					chstr = QString(it->TabValues[t1].tabFillChar);
					if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						if (chstr.upper() != QString(it->TabValues[t1].tabFillChar))
							chstr = chstr.upper();
					}
					chr = chstr[0].unicode();
					uint gl = it->itemText.charStyle(e).font().char2CMap(chstr[0]);
					gly = it->itemText.charStyle(e).font().glyphOutline(gl);
					Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
				}
				continue;
			}
			if (chr == 30)
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
							for (uint nti=0;nti<newText.length();++nti)
								if (pageNumberText.find(newText[nti])==-1)
									pageNumberText+=newText[nti];
						}
					}
				}
				//Now scan and add any glyphs used in page numbers
				for (uint pnti=0;pnti<pageNumberText.length(); ++pnti)
				{
					uint chr = pageNumberText[pnti].unicode();
					if (it->itemText.charStyle(e).font().canRender(chr))
					{
						uint gl = it->itemText.charStyle(e).font().char2CMap(pageNumberText[pnti]);
						FPointArray gly(it->itemText.charStyle(e).font().glyphOutline(gl));
						Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
					}
				}
				continue;
			}
			if (it->itemText.charStyle(e).effects() & ScStyle_SmartHyphenVisible)
			{
				uint gl = it->itemText.charStyle(e).font().char2CMap(QChar('-'));
				FPointArray gly(it->itemText.charStyle(e).font().glyphOutline(gl));
				Really[it->itemText.charStyle(e).font().replacementName()].insert(gl, gly);
			}
			if ((it->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (it->itemText.charStyle(e).effects() & ScStyle_AllCaps))
			{
				chstr = it->itemText.text(e, 1);
				if (chstr.upper() != it->itemText.text(e, 1))
					chstr = chstr.upper();
				chr = chstr[0].unicode();
			}
			if (it->itemText.charStyle(e).font().canRender(chr))
			{
				uint gl = it->itemText.charStyle(e).font().char2CMap(chr);
				gly = it->itemText.charStyle(e).font().glyphOutline(gl);
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
	if( profileNames.findIndex(CMSSettings.DefaultImageRGBProfile) < 0 )
		profileNames.append(CMSSettings.DefaultImageRGBProfile);
	if( profileNames.findIndex(CMSSettings.DefaultImageCMYKProfile) < 0 )
		profileNames.append(CMSSettings.DefaultImageCMYKProfile);
	if( profileNames.findIndex(CMSSettings.DefaultMonitorProfile) < 0 )
		profileNames.append(CMSSettings.DefaultMonitorProfile);
	if( profileNames.findIndex(CMSSettings.DefaultPrinterProfile) < 0 )
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

void ScribusDoc::reorganiseFonts()
{
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
			UsedFonts.remove(itfo);
		}
	}
	PrefsManager* prefsManager=PrefsManager::instance();
	AddFont(prefsManager->appPrefs.toolSettings.defFont);//, prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.toolSettings.defFont]->Font);
	AddFont(toolSettings.defFont);//, prefsManager->appPrefs.AvailFonts[toolSettings.defFont]->Font);
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

	QString mna = pageName;
	if (mna == CommonStrings::trMasterPageNormal)
		mna = CommonStrings::masterPageNormal;
	if (!MasterNames.contains(mna))
		mna = CommonStrings::masterPageNormal;
	Page* Ap = DocPages.at(pageNumber);
	Ap->MPageNam = mna;
	int MpNr = MasterNames[mna];
	Page* Mp = MasterPages.at(MpNr);
	PageItem *currItem;
	for (currItem = Ap->FromMaster.first(); currItem; currItem = Ap->FromMaster.next())
	{
		if (currItem->ChangedMasterItem)
		{
			Ap->FromMaster.remove(currItem);
			delete currItem;
		}
	}
	Ap->FromMaster.clear();
	for (currItem = MasterItems.first(); currItem; currItem = MasterItems.next())
	{
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

//TODO: Handle saving to versions of SLA, and other formats
bool ScribusDoc::save(const QString& fileName)
{
	QFileInfo fi(fileName);
	QDir::setCurrent(fi.dirPath(true));
	QProgressBar* mainWindowProgressBar=NULL;
	if (ScCore->usingGUI())
	{
		mainWindowProgressBar=m_ScMW->mainWindowProgressBar;
		mainWindowProgressBar->reset();
	}
	FileLoader fl(fileName);
	bool ret = fl.SaveFile(fileName, this, mainWindowProgressBar);
	if (ret)
	{
		setModified(false);
		setName(fileName);
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
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			QColor tmpc = ScColorEngine::getRGBColor(PageColors[cstops.at(cst)->name], this);
			ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops.at(cst)->color = tmpc;
		}
	}
	uint masterItemsCount=MasterItems.count();
	for (uint c=0; c<masterItemsCount; ++c)
	{
		PageItem *ite = MasterItems.at(c);
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			QColor tmpc = ScColorEngine::getRGBColor(PageColors[cstops.at(cst)->name], this);
			ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops.at(cst)->color = tmpc;
		}
	}
	uint frameItemsCount=FrameItems.count();
	for (uint c=0; c<frameItemsCount; ++c)
	{
		PageItem *ite = FrameItems.at(c);
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			QColor tmpc = ScColorEngine::getRGBColor(PageColors[cstops.at(cst)->name], this);
			ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops.at(cst)->color = tmpc;
		}
	}
	QStringList patterns = docPatterns.keys();
	for (uint c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (uint o = 0; o < pa.items.count(); o++)
		{
			PageItem *ite = pa.items.at(o);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				QColor tmpc = ScColorEngine::getRGBColor(PageColors[cstops.at(cst)->name], this);
				ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
				cstops.at(cst)->color = tmpc;
			}
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

const bool ScribusDoc::copyPageToMasterPage(const int pageNumber, const int leftPage, const int maxLeftPage,  const QString& masterPageName)
{
	Q_ASSERT(!masterPageMode());
	if (masterPageMode())
		return false;
	//TODO Add Undo here
	int GrMax = GroupCounter;
	Page* sourcePage = Pages->at(pageNumber);
	int nr = MasterPages.count();
	Page* targetPage=addMasterPage(nr, masterPageName);
	Q_ASSERT(targetPage!=NULL);
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
//	QMap<int,int> TableID;
//	QPtrList<PageItem> TableItems;
	sourcePage->guides.copy(&targetPage->guides);
//	struct CopyPasteBuffer BufferT;
	uint end = DocItems.count();
	//CB Need to set this so the paste works correctly. Should not need it really, but its a quick op.
//	setMasterPageMode(true);
	uint end2 = MasterItems.count();
	m_Selection->clear();
	for (uint ite = 0; ite < end; ++ite)
	{
		PageItem *itemToCopy = DocItems.at(ite);
		if (itemToCopy->OwnPage == static_cast<int>(sourcePage->pageNr()))
			m_Selection->addItem(itemToCopy, true);
	}
	ScriXmlDoc *ss = new ScriXmlDoc();
	QString dataS = ss->WriteElem(this, view(), m_Selection);
	setMasterPageMode(true);
	setCurrentPage(targetPage);
	ss->ReadElem(dataS, prefsData.AvailFonts, this, targetPage->xOffset(), targetPage->yOffset(), false, true, prefsData.GFontSub, view());
	m_Selection->clear();
	uint end3 = MasterItems.count();
	for (uint a = end2; a < end3; ++a)
	{
		PageItem *newItem = MasterItems.at(a);
		newItem->OnMasterPage = masterPageName;
		newItem->OwnPage = MasterNames[masterPageName];
	}
/*	for (uint a = 0; a < end; ++a)
	{
		PageItem *itemToCopy = DocItems.at(a);
		if (itemToCopy->OwnPage == static_cast<int>(sourcePage->pageNr()))
		{
			itemToCopy->copyToCopyPasteBuffer(&BufferT);
			if (itemToCopy->Groups.count() != 0)
			{
				BufferT.Groups.clear();
				QValueStack<int>::Iterator nx;
				QValueStack<int> tmpGroup;
				for (nx = itemToCopy->Groups.begin(); nx != itemToCopy->Groups.end(); ++nx)
				{
					tmpGroup.push((*nx)+GroupCounter);
					GrMax = QMAX(GrMax, (*nx)+GroupCounter);
				}
				for (nx = tmpGroup.begin(); nx != tmpGroup.end(); ++nx)
				{
					BufferT.Groups.push((*nx));
				}
			}
			m_View->PasteItem(&BufferT, true, true);
			PageItem* newItem = Items->at(Items->count()-1);
			newItem->OnMasterPage = masterPageName;
			newItem->OwnPage=MasterNames[masterPageName];
			if (newItem->isTableItem)
			{
				TableItems.append(newItem);
				TableID.insert(a, newItem->ItemNr);
			}
			//CB 2906 When this item is pasted it needs moving relative to the origin of the master page
			double OldBX = newItem->BoundingX;
			double OldBY = newItem->BoundingY;
			newItem->moveBy(-sourcePage->xOffset() + targetPage->xOffset(), -sourcePage->yOffset() + targetPage->yOffset());
			newItem->BoundingX = OldBX - sourcePage->xOffset() + targetPage->xOffset();
			newItem->BoundingY = OldBY - sourcePage->yOffset() + targetPage->yOffset();
			bool upDtImg = false;
			if (itemToCopy->pixm.imgInfo.valid)
			{
				newItem->pixm.imgInfo = itemToCopy->pixm.imgInfo;
				upDtImg = true;
			}
			if (itemToCopy->effectsInUse.count() != 0)
			{
				newItem->effectsInUse = itemToCopy->effectsInUse;
				upDtImg = true;
			}
			if (upDtImg)
			{
				int fho = newItem->imageFlippedH();
				int fvo = newItem->imageFlippedV();
				LoadPict(newItem->Pfile, newItem->ItemNr, true);
				newItem->setImageFlippedH(fho);
				newItem->setImageFlippedV(fvo);
				newItem->AdjustPictScale();
			}

		}
	}
	uint tableItemsCount = TableItems.count();
	if (tableItemsCount != 0)
	{
		for (uint ttc = 0; ttc < tableItemsCount; ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	} */
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
	if (UndoManager::undoEnabled() && !_itemCreationTransactionStarted)
	{
		_itemCreationTransactionStarted = true;
		undoManager->beginTransaction();
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
				double lineWidth = w == 0.0 ? 1.0 : w;
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
		default:
			break;
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
		if (itemFinalised)
		{
			//dont think we need this now ... newItem->checkChanges(true);
			undoManager->commit(Pages->at(newItem->OwnPage)->getUName(), newItem->getUPixmap(),
								Um::Create + " " + newItem->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return newItem->ItemNr;
}

int ScribusDoc::itemAddArea(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double w, const QString& fill, const QString& outline, const bool itemFinalised)
{
	double xo = _currentPage->xOffset();
	double yo = _currentPage->yOffset();
	QPair<double, double> tl = _currentPage->guides.topLeft(x - xo, y - yo);
	QPair<double, double> tr = _currentPage->guides.topRight(x - xo, y - yo);
	QPair<double, double> bl = _currentPage->guides.bottomLeft(x - xo, y - yo);
	return itemAdd(itemType, frameType, tl.first + xo, tl.second + yo, tr.first - tl.first, bl.second - tl.second, w, fill, outline, itemFinalised);
}


int ScribusDoc::itemAddUserFrame(InsertAFrameData &iafData)
{
	double x1=0.0,y1=0.0,w1=iafData.width,h1=iafData.height;
	std::vector<int> pageNs;
	if (iafData.locationType==0) // On the current page or ona range of pages
		pageNs.push_back(currentPage()->pageNr()+1);
	else if (iafData.locationType==2) // On all pages
	{
		parsePagesString(QString("1-%1").arg(DocPages.count()), &pageNs, DocPages.count());
	}
	else
		parsePagesString(iafData.pageList, &pageNs, DocPages.count());
	Page* oldCurrentPage = currentPage();
	int z=-2;
	PageItem *prevItem=0; //Previous item for text frame linking

	undoManager->beginTransaction(
			iafData.frameType==PageItem::TextFrame ? Um::TextFrame : Um::ImageFrame,
			iafData.frameType==PageItem::TextFrame ? Um::ITextFrame : Um::IImageFrame,
			Um::InsertFrame, "", Um::ICreate);

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
		else if (iafData.positionType==99) // Frame starts at custom position
		{
			x1=targetPage->xOffset()+iafData.x/docUnitRatio;
			y1=targetPage->yOffset()+iafData.y/docUnitRatio;
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
		else if (iafData.sizeType==99) // Frame is custom size
		{
			w1=iafData.width/docUnitRatio;
			h1=iafData.height/docUnitRatio;
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
					PrefsManager::instance()->prefsFile->getContext("dirs")->set("images", iafData.source.left(iafData.source.findRev("/")));
					currItem->EmProfile = "";
					currItem->pixm.imgInfo.isRequest = false;
					currItem->UseEmbedded = true;
					currItem->IProfile = CMSSettings.DefaultImageRGBProfile;
					currItem->IRender = CMSSettings.DefaultIntentImages;
					qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
					qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
					loadPict(iafData.source, currItem, false, true);
					if (iafData.sizeType==3) //Frame is size of imported image
					{
						currItem->setWidth(static_cast<double>(currItem->OrigW * 72.0 / currItem->pixm.imgInfo.xres));
						currItem->setHeight(static_cast<double>(currItem->OrigH * 72.0 / currItem->pixm.imgInfo.yres));
						currItem->OldB2 = currItem->width();
						currItem->OldH2 = currItem->height();
						currItem->updateClip();
					}
					qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
					qApp->restoreOverrideCursor();
				}
			}
			if (iafData.frameType==PageItem::TextFrame)
			{
				currItem->setColumns(iafData.columnCount);
				currItem->setColumnGap(iafData.columnGap/docUnitRatio);
				if (iafData.linkTextFrames && prevItem!=0)
				{
					currItem->BackBox = prevItem;
					prevItem->NextBox = currItem;
					currItem->itemText = prevItem->itemText;
				}
				if (!iafData.source.isEmpty() && prevItem==0 && QFile::exists(iafData.source))
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

	undoManager->commit();
	setCurrentPage(oldCurrentPage);
	changed();
	emit updateContents();
	return z;
}

void ScribusDoc::itemAddDetails(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const int itemNumber)
{
	PageItem* newItem=Items->at(itemNumber);
	Q_ASSERT(newItem->itemType()==itemType);
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
	if (_itemCreationTransactionStarted && appMode !=  modeDrawBezierLine)
	{
		PageItem *createdItem=m_Selection->itemAt(0);
		if (createdItem!=NULL)
		{
			createdItem->checkChanges(true);
			QString targetName = Um::ScratchSpace;
			if (createdItem->OwnPage > -1)
				targetName = Pages->at(createdItem->OwnPage)->getUName();
			undoManager->commit(targetName, createdItem->getUPixmap(),
								Um::Create + " " + createdItem->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
			if (!isLoading())
				return true;
		}
	}
	return false;
}

uint ScribusDoc::getItemNrfromUniqueID(uint unique)
{
	uint ret = 0;
	for (uint a = 0; a < Items->count(); ++a)
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
	uint itemsCount=Items->count();
	for (uint i = 0; i < itemsCount; ++i)
		Items->at(i)->ItemNr = i;
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
		//TODO: Make this a signal again one day
		//emit RasterPic(pageItem->isRaster);
		m_ScMW->HaveRaster(pageItem->isRaster);
		//TODO: Previously commented out.. unsure why, remove later
		//emit UpdtObj(PageNr, ItNr);
		//CB: probably because we are therefore not always refreshing the view when an image changes...
		emit refreshItem(pageItem);
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
				minx = QMIN(minx, pb.point(pc).x());
				miny = QMIN(miny, pb.point(pc).y());
				maxx = QMAX(maxx, pb.point(pc).x());
				maxy = QMAX(maxy, pb.point(pc).y());
			}
		}
		else
		{
			minx = QMIN(minx, currItem->xPos());
			miny = QMIN(miny, currItem->yPos());
			maxx = QMAX(maxx, currItem->xPos() + currItem->width());
			maxy = QMAX(maxy, currItem->yPos() + currItem->height());
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
		int x = static_cast<int>(currentPage()->xOffset());
		int y = static_cast<int>(currentPage()->yOffset());
		int w = static_cast<int>(currentPage()->width());
		int h = static_cast<int>(currentPage()->height());
		if (QRect(x, y, w, h).contains(qRound(x2), qRound(y2)))
			retw = currentPage()->pageNr();
	}
	else
	{
		uint docPageCount = Pages->count();
		for (uint a = 0; a < docPageCount; ++a)
		{
			int x = static_cast<int>(Pages->at(a)->xOffset());
			int y = static_cast<int>(Pages->at(a)->yOffset());
			int w = static_cast<int>(Pages->at(a)->width());
			int h = static_cast<int>(Pages->at(a)->height());
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
		double x = currentPage()->xOffset();
		double y = currentPage()->yOffset();
		double w = currentPage()->width();
		double h1 = currentPage()->height();
		double x2 = currItem->BoundingX;
		double y2 = currItem->BoundingY;
		double w2 = currItem->BoundingW;
		double h2 = currItem->BoundingH;
		if (( QMAX( x, x2 ) <= QMIN( x+w, x2+w2 ) && QMAX( y, y2 ) <= QMIN( y+h1, y2+h2 )))
			retw = currentPage()->pageNr();
	}
	else
	{
		uint docPageCount = Pages->count();
		for (uint a = 0; a < docPageCount; ++a)
		{
			double x = Pages->at(a)->xOffset();
			double y = Pages->at(a)->yOffset();
			double w = Pages->at(a)->width();
			double h1 = Pages->at(a)->height();
			double x2 = currItem->BoundingX;
			double y2 = currItem->BoundingY;
			double w2 = currItem->BoundingW;
			double h2 = currItem->BoundingH;
			if (( QMAX( x, x2 ) <= QMIN( x+w, x2+w2 ) && QMAX( y, y2 ) <= QMIN( y+h1, y2+h2 )))
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
	QPtrList<PageItem> Objects;
	PageItem* item;
	if (currItem->Groups.count() == 0)
		return;
	int ObjGroup = currItem->Groups.top();
	uint docItemCount=Items->count();
	for (uint a = 0; a < docItemCount; ++a)
	{
		item = Items->at(a);
		if (item->Groups.top() == ObjGroup)
			Objects.append(item);
	}
	int Off_Page = -1;
	int On_Page = 999999;
	uint objectCount=Objects.count();
	for (uint a = 0; a < objectCount; ++a)
	{
		int res = OnPage(Objects.at(a));
		Off_Page = QMAX(Off_Page, res);
		if (res != -1)
			On_Page = QMIN(On_Page, res);
	}
	int final = -1;
	if (Off_Page != -1)
		final = On_Page;
	for (uint a = 0; a < objectCount; ++a)
	{
		Objects.at(a)->OwnPage = final;
	}
}

//CB TODO make a function to determine the place of the page.. ie, so we know the left and right margins
// without running this monster
void ScribusDoc::reformPages(bool moveObjects)
{
	QMap<uint, ScribusView::oldPageVar> pageTable;
	struct ScribusView::oldPageVar oldPg;
	int counter = pageSets[currentPageLayout].FirstPage;
	int rowcounter = 0;
	double maxYPos=0.0, maxXPos=0.0;
	double currentXPos=ScratchLeft, currentYPos=ScratchTop, lastYPos=Pages->at(0)->initialHeight();
	currentXPos += (pageWidth+pageSets[currentPageLayout].GapHorizontal) * counter;

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
			Seite->setXOffset(ScratchLeft);
			Seite->setYOffset(ScratchTop);
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
				currentXPos += Seite->width() + pageSets[currentPageLayout].GapHorizontal;
				lastYPos = QMAX(lastYPos, Seite->height());
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
				currentXPos = ScratchLeft;
				if (pageSets[currentPageLayout].Columns > 1)
					currentYPos += QMAX(lastYPos, Seite->height())+pageSets[currentPageLayout].GapVertical;
				else
					currentYPos += Seite->height()+pageSets[currentPageLayout].GapVertical;
//				lastYPos = QMAX(lastYPos, Seite->height());
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
					currentYPos += pageSets[currentPageLayout].GapBelow;
					rowcounter = 0;
				}
			}
		}
		Seite->Margins.Top = Seite->initialMargins.Top;
		Seite->Margins.Bottom = Seite->initialMargins.Bottom;
		maxXPos = QMAX(maxXPos, Seite->xOffset()+Seite->width()+ScratchRight);
		maxYPos = QMAX(maxYPos, Seite->yOffset()+Seite->height()+ScratchBottom);
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
		FPoint maxSize(QMAX(maxXPos, maxPoint.x()+ScratchRight), QMAX(maxYPos, maxPoint.y()+ScratchBottom));
		adjustCanvas(FPoint(QMIN(0, minPoint.x()-ScratchLeft),QMIN(0, minPoint.y()-ScratchTop)), maxSize, true);
		changed();
	}
	else
	{
		FPoint maxSize(maxXPos, maxYPos);
		adjustCanvas(FPoint(0, 0), maxSize);
	}
}

const double ScribusDoc::getXOffsetForPage(const int pageNumber)
{
	if (Pages->at(pageNumber)!=NULL)
		return Pages->at(pageNumber)->xOffset();
	return -1.0;
}

const double ScribusDoc::getYOffsetForPage(const int pageNumber)
{
	if (Pages->at(pageNumber)!=NULL)
		return Pages->at(pageNumber)->yOffset();
	return -1.0;
}

void ScribusDoc::getBleeds(int pageNumber, double *bleedTop, double *bleedBottom, double *bleedLeft, double *bleedRight)
{
	*bleedBottom = bleeds.Bottom;
	*bleedTop = bleeds.Top;
	if (pageSets[currentPageLayout].Columns == 1)
	{
		*bleedRight = bleeds.Right;
		*bleedLeft = bleeds.Left;
	}
	else
	{
		if (locationOfPage(Pages->at(pageNumber)->pageNr()) == LeftPage)
		{
			*bleedRight = bleeds.Left;
			*bleedLeft = bleeds.Right;
		}
		else if (locationOfPage(Pages->at(pageNumber)->pageNr()) == RightPage)
		{
			*bleedRight = bleeds.Right;
			*bleedLeft = bleeds.Left;
		}
		else
		{
			*bleedRight = bleeds.Left;
			*bleedLeft = bleeds.Left;
		}
	}
}

void ScribusDoc::getBleeds(Page* page, double *bleedTop, double *bleedBottom, double *bleedLeft, double *bleedRight)
{
	*bleedBottom = bleeds.Bottom;
	*bleedTop = bleeds.Top;
	if (pageSets[currentPageLayout].Columns == 1)
	{
		*bleedRight = bleeds.Right;
		*bleedLeft = bleeds.Left;
	}
	else
	{
		if (locationOfPage(page->pageNr()) == LeftPage)
		{
			*bleedRight = bleeds.Left;
			*bleedLeft = bleeds.Right;
		}
		else if (locationOfPage(page->pageNr()) == RightPage)
		{
			*bleedRight = bleeds.Right;
			*bleedLeft = bleeds.Left;
		}
		else
		{
			*bleedRight = bleeds.Left;
			*bleedLeft = bleeds.Left;
		}
	}
}

PageItem* ScribusDoc::convertItemTo(PageItem *currItem, PageItem::ItemType newType, PageItem* secondaryItem)
{
	//Item to convert is null, return
	Q_ASSERT(currItem!=NULL);
	if (currItem==NULL)
		return false;
	//Dont attempt a Line conversion
	if (currItem->itemType()==PageItem::Line || newType==PageItem::Line)
		return false;
	//Take the item to convert from the docs Items list
	PageItem *oldItem = Items->take(currItem->ItemNr);
	//Remove old item from the doc's selection if it was in it
	bool removedFromSelection=m_Selection->removeItem(oldItem);
	//Create a new item from the old one
	bool transactionConversion=false;
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
				undoManager->beginTransaction(currentPage()->getUName(), 0, Um::TextFrame, "", Um::ITextFrame);
				transactionConversion=true;
			}
			break;
		//We dont allow this
		//case PageItem::Line:
		//	newItem = new PageItem_Line(*oldItem);
		//	break;
		case PageItem::Polygon:
			newItem = new PageItem_Polygon(*oldItem);
			break;
		case PageItem::PolyLine:
			newItem = new PageItem_PolyLine(*oldItem);
			break;
		case PageItem::PathText:
			if (secondaryItem==NULL)
				return false;
			if (UndoManager::undoEnabled())
			{
				undoManager->beginTransaction(currentPage()->getUName(), 0, Um::PathText, "", Um::ITextFrame);
				transactionConversion=true;
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
			undoManager->cancelTransaction();
		return false;
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
				polyLineItem->setPolyClip(qRound(QMAX(polyLineItem->lineWidth() / 2, 1)));
				AdjustItemSize(polyLineItem);

				newItem->setLineColor(CommonStrings::None);
				newItem->SetRectFrame();
				newItem->setRedrawBounding();
			}
			break;
		//We dont allow this right now
		//case PageItem::Line:
		//	newItem->convertTo(PageItem::Line);
		//	break;
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
			newItem->setPolyClip(qRound(QMAX(newItem->lineWidth() / 2, 1)));
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
				newItem->updatePolyClip();
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
	Items->insert(oldItem->ItemNr, newItem);
	//Add new item back to selection if old item was in selection
	if (removedFromSelection)
		m_Selection->addItem(newItem);
	//If converting text to path, delete the bezier
	if (newType==PageItem::PathText)
	{
		//FIXME: Stop using the view here
		m_View->SelectItem(secondaryItem);
		itemSelection_DeleteItem();
		emit updateContents();
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
		if (UndoManager::undoEnabled())
			undoManager->commit();
	}
	return newItem;
}

const int ScribusDoc::currentPageNumber()
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
	uint currMinIndex = itprev.data().fromindex;
	uint currMaxIndex = itprev.data().toindex;
	for ( ; it != sections.end(); ++it )
	{
		currMinIndex=it.data().fromindex;
		currMaxIndex=it.data().toindex;

		if (it.key()!=number)
			itprev=it;
		else
			break;
	}
	if (it != itprev)
		itprev.data().toindex=currMaxIndex;
	else {
		// special case: delete first section
		QMap<uint, DocumentSection>::Iterator itnext = it;
		++itnext;
		itnext.data().fromindex=it.data().fromindex;
	}
	sections.remove(it);
	return true;
}

int ScribusDoc::getSectionKeyForPageIndex(const uint pageIndex) const
{
	bool found=false;
	int retVal=-1;
	DocumentSectionMap::ConstIterator it = sections.begin();
	for (; it!= sections.end(); ++it)
	{
		if (pageIndex>=it.data().fromindex && pageIndex<=it.data().toindex)
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
	/*
	bool found=false;

	DocumentSectionMap::ConstIterator it = sections.begin();
	for (; it!= sections.end(); ++it)
	{
		if (pageIndex>=it.data().fromindex && pageIndex<=it.data().toindex)
		{
			found=true;
			break;
		}
	}
	if (!found)
		return retVal;
	*/
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
	for (; it!= sections.end(); ++it)
	{
		if (otherPageIndex>=it.data().fromindex && otherPageIndex<=it.data().toindex)
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
	if (otherPageIndex==it.data().fromindex && location==0 && it!=sections.begin())
		--it2;
	it2.data().toindex+=count;
	++it2;
	while (it2!=sections.end())
	{
		it2.data().fromindex+=count;
		it2.data().toindex+=count;
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
		if (pageIndex>=it.data().fromindex && pageIndex<=it.data().toindex)
		{
			found=true;
			break;
		}
	}
	Q_ASSERT(found);
	if (!found)
		return;
	--it.data().toindex;
	++it;
	while (it!=sections.end())
	{
		--it.data().fromindex;
		--it.data().toindex;
		++it;
	}
	//Now update the Pages' internal storage of their page number
	updateSectionPageNumbersToPages();
}

void ScribusDoc::setFirstSectionFromFirstPageNumber()
{
	DocumentSectionMap::Iterator it = sections.begin();
	it.data().sectionstartindex=FirstPnum;
	updateSectionPageNumbersToPages();
}

void ScribusDoc::copyPage(int pageNumberToCopy, int existingPage, int whereToInsert, int copyCount)
{
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
		Page* destination = new Page(ScratchLeft, DocPages.count()*(pageHeight+ScratchBottom+ScratchTop)+ScratchTop, pageWidth, pageHeight);
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
		m_View->reformPages();
		uint oldItems = Items->count();
		m_Selection->clear();
		if (oldItems>0)
		{
			for (uint ite = 0; ite < oldItems; ++ite)
			{
				PageItem *itemToCopy = Items->at(ite);
				if (itemToCopy->OwnPage == static_cast<int>(from->pageNr()))
					m_Selection->addItem(itemToCopy, true);
			}
			ScriXmlDoc *ss = new ScriXmlDoc();
			ss->ReadElem(ss->WriteElem(this, view(), m_Selection), prefsData.AvailFonts, this, destination->xOffset(), destination->yOffset(), false, true, prefsData.GFontSub, view());
			m_Selection->clear();
		}
		from->guides.copy(&destination->guides);
		GroupCounter = GrMax + 1;
	}
	setUsesAutomaticTextFrames(autoText);
	addPageToSection(existingPage-1, whereToInsert, copyCount);
	if (lastDest != NULL)
		setCurrentPage(lastDest);
	else
		setCurrentPage(from);
	changed();
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

PageLocation ScribusDoc::locationOfPage(int pageIndex)
{
	int setcol=pageSets[currentPageLayout].Columns;
	if (setcol==1)
		return LeftPage;
	int myCol = ((pageIndex % setcol)+pageSets[currentPageLayout].FirstPage)%setcol;
	if (myCol==0) //Left hand page
		return LeftPage;
	else if (myCol>= setcol-1) // Right hand page
		return RightPage;
	else //Middle pages
		return MiddlePage;
}


void ScribusDoc::updateAllItemQColors()
{
	for (uint c=0; c<DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		ite->setLineQColor();
		ite->setFillQColor();
	}
	for (uint c=0; c<MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		ite->setLineQColor();
		ite->setFillQColor();
	}
	for (uint c=0; c<FrameItems.count(); ++c)
	{
		PageItem *ite = FrameItems.at(c);
		ite->setLineQColor();
		ite->setFillQColor();
	}
	QStringList patterns = docPatterns.keys();
	for (uint c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (uint o = 0; o < pa.items.count(); o++)
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
			counter = dia->progress();
		PageItem* it;
		for (uint i=0; i < docItemCount; ++i)
		{
			it = Items->at(i);
			if ((it->itemType() == PageItem::ImageFrame) && (it->PicAvail))
			{
				if (it->pixm.imgInfo.colorspace == 1)
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
					dia->setProgress(counter);
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
			int d = Items->findRef(currItem);
			Items->take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
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
			int d = Items->findRef(currItem);
			Items->take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = 0; c <static_cast<int>(Oindex.count()); ++c)
		{
			Items->append(m_Selection->itemAt(Oindex[c]));
		}
		renumberItemsInListOrder();
		return true;
	}
	return false;
}

void ScribusDoc::ChLineWidth(double w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineWidth, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			//cb moved to setlinewidth
			//currItem->Oldm_lineWidth = currItem->lineWidth();
			currItem->setLineWidth(w);
			if (currItem->asPolyLine())
				currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2, 1)));
			if (currItem->asLine())
			{
				int ph = static_cast<int>(QMAX(1.0, w / 2.0));
				currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
				                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
				                  -ph,static_cast<int>(currItem->height()+ph));
			}
			emit refreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineArt(Qt::PenStyle w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
						  Um::IGroup, Um::LineStyle, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->setLineStyle(w);
			emit refreshItem(m_Selection->itemAt(a));
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineJoin(Qt::PenJoinStyle w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineJoin, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->setLineJoin(w);
			emit refreshItem(m_Selection->itemAt(a));
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineEnd(Qt::PenCapStyle w)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineEnd, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->setLineEnd(w);
			emit refreshItem(m_Selection->itemAt(a));
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
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
	newStyle.setParent(name);
	itemSelection_ApplyCharStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetNamedParagraphStyle(const QString& name, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setParent(name);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::ItemPen(QString farbe)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (farbe == CommonStrings::NoneColor)
			farbe = CommonStrings::None;
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetLineColor, farbe, Um::IFill);
		PageItem *i;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			i = m_Selection->itemAt(a);
			if ((i->asLine()) && (farbe == CommonStrings::None))
				continue;

			i->setLineColor(farbe);
			emit refreshItem(i);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
	changed();
}

void ScribusDoc::itemSelection_SetFillColor(QString farbe, Selection* customSelection)
{
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
	if (farbe == CommonStrings::NoneColor)
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

void ScribusDoc::ItemBrush(QString farbe)
{
	if (farbe == CommonStrings::NoneColor)
		farbe = CommonStrings::None;
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetFill, farbe, Um::IFill);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setFillColor(farbe);
			emit refreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
		changed();
	}
}

void ScribusDoc::ItemBrushShade(int sha)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetShade, QString("%1").arg(sha),
										  Um::IShade);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setFillShade(sha);
			emit refreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
		changed();
	}
}

void ScribusDoc::ItemPenShade(int sha)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
							Um::IGroup, Um::SetLineShade, QString("%1").arg(sha), Um::IShade);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setLineShade(sha);
			emit refreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
		changed();
	}
}

void ScribusDoc::ItemGradFill(int typ)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->GrType = typ;
			if (typ != 8)
				currItem->updateGradientVectors();
			emit refreshItem(currItem);
		}
		changed();
	}
}

void ScribusDoc::ItemPatternFill(QString pattern)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setPattern(pattern);
			emit refreshItem(currItem);
		}
		changed();
	}
}

void ScribusDoc::ItemPatternProps(double scaleX, double scaleY, double offsetX, double offsetY, double rotation)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = m_Selection->itemAt(a);
			currItem->setPatternTransform(scaleX, scaleY, offsetX, offsetY, rotation);
			emit refreshItem(currItem);
		}
		changed();
	}
}

void ScribusDoc::itemSelection_SetEffects(int s, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setEffects(static_cast<StyleFlag>(s));
	itemSelection_ApplyCharStyle(newStyle, customSelection);
	return;
	
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0 && s != ScStyle_None)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontEffect, "", Um::IFont);
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
							currItem->itemText.item(a)->setEffects(fl);
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
						currItem->itemText.item(a)->setEffects(fl);
					}
				}
#endif
				emit refreshItem(currItem);
			}
		}
		if (selectedItemCount > 1)
			undoManager->commit();
		changed();
	}
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
	if (appMode == modeEdit) {
		CharStyle newStyle;
		newStyle.setFontSize(size);
		itemSelection_ApplyCharStyle(newStyle, customSelection);
		return;
	}
/*
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	if (selectedItemCount > 1)
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontSize,
										QString("%1").arg(size/10.0), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);

	ParagraphStyle storyStyle(currItem->itemText.defaultStyle());
	*/
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
	
	/*
		currItem->itemText.setDefaultStyle(storyStyle);
	}
	if (selectedItemCount > 1)
		undoManager->commit();
	 */
}

void ScribusDoc::itemSelection_SetParagraphStyle(const ParagraphStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	if (selectedItemCount > 1)
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle,
									newStyle.displayName(), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		uint currItemTextCount=currItem->itemText.length();
		if (currItemTextCount != 0 && appMode == modeEdit)
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int stop = currItem->itemText.endOfItem(currItem->lastInFrame());
			if (appMode == modeEdit)
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = QMAX(0, QMIN(currItem->itemText.length(), currItem->CPos));
			}
			for (int pos=start; pos < stop; ++pos) {
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP) {
					currItem->itemText.setStyle(pos, newStyle);
				}
			}
			currItem->itemText.setStyle(stop, newStyle);
			currItem->invalid = true;

			if (currItem->asPathText())
			{
				currItem->updatePolyClip();
				AdjustItemSize(currItem);
			}
//				emit refreshItem(currItem);
		}
		else
		{
			currItem->itemText.setDefaultStyle(newStyle);
		}
		
	}
	if (selectedItemCount > 1)
		undoManager->commit();
	emit updateContents();
}


void ScribusDoc::itemSelection_ApplyParagraphStyle(const ParagraphStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	if (selectedItemCount > 1)
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle,
										newStyle.displayName(), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		uint currItemTextCount=currItem->itemText.length();
		if (currItemTextCount != 0 && appMode == modeEdit)
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int stop = currItem->itemText.endOfItem(currItem->lastInFrame());
			if (appMode == modeEdit)
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = QMAX(0, QMIN(currItem->itemText.length(), currItem->CPos));
			}
			for (int pos=start; pos < stop; ++pos) {
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP) {
					currItem->itemText.applyStyle(pos, newStyle);
				}
			}
			currItem->itemText.applyStyle(stop, newStyle);
			currItem->invalid = true;

			if (currItem->asPathText())
			{
				currItem->updatePolyClip();
				AdjustItemSize(currItem);
			}
			//				emit refreshItem(currItem);
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.applyStyle(newStyle);
			currItem->itemText.setDefaultStyle(dstyle);
		}
		
	}
	if (selectedItemCount > 1)
		undoManager->commit();
	emit updateContents();
}

void ScribusDoc::itemSelection_ApplyCharStyle(const CharStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	if (selectedItemCount > 1)
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle,
										newStyle.asString(), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		uint currItemTextCount=currItem->itemText.length();
		if (currItemTextCount != 0 && appMode == modeEdit)
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int length = currItem->itemText.endOfItem(currItem->lastInFrame()) - start;
			if (appMode == modeEdit)
			{
				if (currItem->itemText.lengthOfSelection() > 0) {
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else {
					start = QMAX(currItem->firstInFrame(), currItem->CPos);
					length = 1;
				}
			}
			currItem->itemText.applyCharStyle(start, QMAX(0, length), newStyle);
			currItem->invalid = true;
			
			if (currItem->asPathText())
			{
				currItem->updatePolyClip();
				AdjustItemSize(currItem);
			}
			//				emit refreshItem(currItem);
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.charStyle().applyCharStyle(newStyle);
			currItem->itemText.setDefaultStyle(dstyle);
		}				
	}
	if (selectedItemCount > 1)
		undoManager->commit();
	emit updateContents();
}



void ScribusDoc::itemSelection_SetCharStyle(const CharStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	if (selectedItemCount > 1)
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle,
										newStyle.asString(), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		uint currItemTextCount=currItem->itemText.length();
		if (currItemTextCount != 0 && appMode == modeEdit)
		{
			int start = currItem->itemText.startOfItem(currItem->firstInFrame());
			int length = currItem->itemText.endOfItem(currItem->lastInFrame()) - start;
			if (appMode == modeEdit)
			{
				if (currItem->itemText.lengthOfSelection() > 0) {
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else {
					start = QMAX(currItem->firstInFrame(), currItem->CPos);
					length = 1;
				}
			}
			currItem->itemText.setCharStyle(start, length, newStyle);
			currItem->invalid = true;
			
			if (currItem->asPathText())
			{
				currItem->updatePolyClip();
				AdjustItemSize(currItem);
			}
			//				emit refreshItem(currItem);
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.charStyle().setStyle(newStyle);
			currItem->itemText.setDefaultStyle(dstyle);
		}
		
	}
	if (selectedItemCount > 1)
		undoManager->commit();
	emit updateContents();
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

//CB removed looping, called by itemSelection_FlipH
void ScribusDoc::MirrorPolyH(PageItem* currItem)
{
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (view()->EditContour)
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
		emit updateContents();
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		m_View->MarkClip(currItem, currItem->ContourLine, true);
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
	emit refreshItem(currItem);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
		ss->set("MIRROR_PATH_H", "mirror_path_h");
		ss->set("IS_CONTOUR", false);
		undoManager->action(currItem, ss, Um::IBorder);
	}
	changed();
}

//CB removed looping, called by itemSelection_FlipV
void ScribusDoc::MirrorPolyV(PageItem* currItem)
{
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (view()->EditContour)
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
		emit updateContents();
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		m_View->MarkClip(currItem, currItem->ContourLine, true);
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
	emit refreshItem(currItem);
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
	FPoint maxSize(currItem->BoundingX+currItem->BoundingW+ScratchRight, currItem->BoundingY+currItem->BoundingH+ScratchBottom);
	FPoint minSize(currItem->BoundingX-ScratchLeft, currItem->BoundingY-ScratchTop);
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
		newMaxX = QMAX(maxCanvasCoordinate.x(), maxPos.x());
		newMaxY = QMAX(maxCanvasCoordinate.y(), maxPos.y());
		newMinX = QMIN(minCanvasCoordinate.x(), minPos.x());
		newMinY = QMIN(minCanvasCoordinate.y(), minPos.y());
	}
	if ((newMaxX != maxCanvasCoordinate.x()) || (newMaxY != maxCanvasCoordinate.y())
	|| (newMinX != minCanvasCoordinate.x()) || (newMinY != minCanvasCoordinate.y()))
	{
		//CB TODO Make a list of views we belong to and make this the doc's active view via an internal*
		if (ScCore->usingGUI() && !m_View->operItemMoving)
		{
			//Save the old values for the emit, but update now to ensure we are all ready
			double oldMinX=minCanvasCoordinate.x();
			double oldMinY=minCanvasCoordinate.y();
			maxCanvasCoordinate = FPoint(newMaxX, newMaxY);
			minCanvasCoordinate = FPoint(newMinX, newMinY);
			// Why using a signal here ? much slower than a direct call
//			emit canvasAdjusted(newMaxX - newMinX, newMaxY - newMinY, oldMinX - newMinX, oldMinY - newMinY);
			m_View->adjustCanvas(newMaxX - newMinX, newMaxY - newMinY, oldMinX - newMinX, oldMinY - newMinY);
		}
	}
}

void ScribusDoc::connectDocSignals()
{
	if (ScCore->usingGUI())
	{
		connect(this, SIGNAL(setApplicationMode(int)), m_ScMW, SLOT(setAppMode(int)));
		connect(this, SIGNAL(docChanged()), m_ScMW, SLOT(slotDocCh()));
		connect(this, SIGNAL(firstSelectedItemType(int)), m_ScMW, SLOT(HaveNewSel(int)));
		connect(autoSaveTimer, SIGNAL(timeout()), WinHan, SLOT(slotAutoSave()));
		connect(this, SIGNAL(refreshItem(PageItem*)), view(), SLOT(RefreshItem(PageItem*)));
		connect(this, SIGNAL(updateContents()), view(), SLOT(slotUpdateContents()));
		connect(this, SIGNAL(updateContents(const QRect&)), view(), SLOT(slotUpdateContents(const QRect&)));
//		connect(this, SIGNAL(canvasAdjusted(double, double, double, double)), view(), SLOT(adjustCanvas(double, double, double, double)));
	}
}

//CB Same as RecalcPicturesRes apart from the name checking, which should be able to be removed
void ScribusDoc::updatePict(QString name)
{
	for (uint a = 0; a < DocItems.count(); ++a)
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
	for (uint a = 0; a < MasterItems.count(); ++a)
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
	for (uint a = 0; a <FrameItems.count(); ++a)
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
	for (uint c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (uint o = 0; o < pa.items.count(); o++)
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
	emit updateContents();
	changed();
}

//CB Same as updatePict apart from the name checking, this should be able to be removed
void ScribusDoc::recalcPicturesRes(bool applyNewRes)
{
	int cc = 0;
	int ca = 0;
	m_ScMW->mainWindowProgressBar->reset();
	for (uint a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->PicAvail)
		  cc++;
	}
	for (uint a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->PicAvail)
			cc++;
	}
	for (uint a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if (currItem->PicAvail)
			cc++;
	}
	QStringList patterns = docPatterns.keys();
	for (uint c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (uint o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->PicAvail)
				cc++;
		}
	}
	m_ScMW->mainWindowProgressBar->setTotalSteps(cc);
	for (uint a = 0; a < DocItems.count(); ++a)
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
			m_ScMW->mainWindowProgressBar->setProgress(ca);
			qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
		}
	}
	for (uint a = 0; a < MasterItems.count(); ++a)
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
			m_ScMW->mainWindowProgressBar->setProgress(ca);
			qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
		}
	}
	for (uint a = 0; a < FrameItems.count(); ++a)
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
			m_ScMW->mainWindowProgressBar->setProgress(ca);
			qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
		}
	}
	for (uint c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (uint o = 0; o < pa.items.count(); o++)
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
				m_ScMW->mainWindowProgressBar->setProgress(ca);
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
			}
		}
		PageItem *ite = pa.items.at(0);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items);
	}
	emit updateContents();
	changed();
	m_ScMW->mainWindowProgressBar->reset();
}

void ScribusDoc::removePict(QString name)
{
	for (uint a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = ScImage();
		}
	}
	for (uint a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = ScImage();
		}
	}
	for (uint a = 0; a < FrameItems.count(); ++a)
	{
		PageItem *currItem = FrameItems.at(a);
		if ((currItem->PicAvail) && (currItem->Pfile == name))
		{
			currItem->PicAvail = false;
			currItem->pixm = ScImage();
		}
	}
	QStringList patterns = docPatterns.keys();
	for (uint c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (uint o = 0; o < pa.items.count(); o++)
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
	emit updateContents();
	changed();
}

void ScribusDoc::updatePic()
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount > 0)
	{
		bool toUpdate=false;
		for (uint i = 0; i < docSelectionCount; ++i)
		{
			if (m_Selection->itemAt(i)!=NULL)
				if (m_Selection->itemAt(i)->asImageFrame())
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
		if (toUpdate)
			emit updateContents();
	}
}

//TODO replace with the ScribusDoc::deleteTaggedItems
void ScribusDoc::removeLayer(int l, bool dl)
{
	m_View->Deselect();
	for (uint b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				m_Selection->addItem(MasterItems.at(b));
				DocItems.at(b)->setLocked(false);
			}
			else
				MasterItems.at(b)->setLayer(0);
		}
	}
	if (m_Selection->count() != 0)
		itemSelection_DeleteItem();
	m_Selection->clear();
	for (uint b = 0; b < DocItems.count(); ++b)
	{
		if (DocItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				m_Selection->addItem(DocItems.at(b));
				DocItems.at(b)->setLocked(false);
			}
			else
				DocItems.at(b)->setLayer(0);
		}
	}
	if (m_Selection->count() != 0)
		itemSelection_DeleteItem();
	//FIXME signal these
	m_ScMW->rebuildLayersList();
	m_View->updateLayerMenu();
}

void ScribusDoc::itemSelection_ToggleLock( )
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
		{
			if (m_Selection->itemAt(0)->locked())
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::UnLock, 0, Um::IUnLock);
			else
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Lock, 0, Um::ILock);
		}
		for ( uint a = 0; a < docSelectionCount; ++a)
		{
			m_Selection->itemAt(a)->toggleLock();
			emit refreshItem(m_Selection->itemAt(a));
		}
		if (docSelectionCount > 1)
			undoManager->commit();
		changed();
 		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
}

void ScribusDoc::itemSelection_ToggleSizeLock( )
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
		{
			if (m_Selection->itemAt(0)->sizeLocked())
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SizeUnLock, 0, Um::IUnLock);
			else
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SizeLock, 0, Um::ILock);
		}
		for ( uint a = 0; a < selectedItemCount; ++a)
		{
			m_Selection->itemAt(a)->toggleSizeLock();
			emit refreshItem(m_Selection->itemAt(a));
		}
		if (selectedItemCount > 1)
			undoManager->commit();
		changed();
 		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
}


void ScribusDoc::itemSelection_ToggleImageShown()
{
	if (m_Selection->count() != 0)
	{
		for (uint a = 0; a < m_Selection->count(); ++a)
		{
			PageItem_ImageFrame* imageItem=m_Selection->itemAt(a)->asImageFrame();
			if (imageItem==NULL)
				continue;
			imageItem->setImageShown(!imageItem->imageShown());
			emit refreshItem(imageItem);
		}
		changed();
		//Return to normal mode if in edit mode. We should not allow dragging of
		//an image in a frame if its not shown.
		if (appMode == modeEdit)
			emit setApplicationMode(modeNormal);
	}
}

void ScribusDoc::itemSelection_TogglePrintEnabled( )
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
		{
			if (m_Selection->itemAt(0)->printEnabled())
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::DisablePrint, 0, Um::IDisablePrint);
			else
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::EnablePrint, 0, Um::IEnablePrint);
		}
		for ( uint a = 0; a < docSelectionCount; ++a)
		{
			m_Selection->itemAt(a)->togglePrintEnabled();
			emit refreshItem(m_Selection->itemAt(a));
		}
		if (docSelectionCount > 1)
			undoManager->commit();
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
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::FlipH, 0, Um::IFlipH);
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
				undoManager->commit();
			}
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
		emit updateContents();
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
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::FlipV, 0, Um::IFlipV);
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
			emit updateContents();
			undoManager->commit();
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
			emit updateContents();
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
		disconnect( m_ScMW->scrActions["itemPreviewLow"], SIGNAL(activatedData(int)) , 0, 0 );
		disconnect( m_ScMW->scrActions["itemPreviewNormal"], SIGNAL(activatedData(int)) , 0, 0 );
		disconnect( m_ScMW->scrActions["itemPreviewFull"], SIGNAL(activatedData(int)) , 0, 0 );
		m_ScMW->scrActions["itemPreviewLow"]->setOn(id==m_ScMW->scrActions["itemPreviewLow"]->actionInt());
		m_ScMW->scrActions["itemPreviewNormal"]->setOn(id==m_ScMW->scrActions["itemPreviewNormal"]->actionInt());
		m_ScMW->scrActions["itemPreviewFull"]->setOn(id==m_ScMW->scrActions["itemPreviewFull"]->actionInt());
		connect( m_ScMW->scrActions["itemPreviewLow"], SIGNAL(activatedData(int)), this, SLOT(itemSelection_ChangePreviewResolution(int)) );
		connect( m_ScMW->scrActions["itemPreviewNormal"], SIGNAL(activatedData(int)), this, SLOT(itemSelection_ChangePreviewResolution(int)) );
		connect( m_ScMW->scrActions["itemPreviewFull"], SIGNAL(activatedData(int)), this, SLOT(itemSelection_ChangePreviewResolution(int)) );
	}
}

void ScribusDoc::itemSelection_ClearItem(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
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
				if (currItem->itemText.length() != 0 && (currItem->NextBox == 0 || currItem->BackBox == 0))
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
		emit updateContents();
		changed();
	}
}

void ScribusDoc::itemSelection_DeleteItem(Selection* customSelection, bool forceDeletion)
{
	if (EditClip)
		return;
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	QPtrList<PageItem> delItems;
	PageItem *currItem;
	uint offs = 0;
	QString tooltip = Um::ItemsInvolved + "\n";
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
				return;
			}
		}
		tooltip += "\t" + currItem->getUName() + "\n";
		delItems.append(itemSelection->takeItem(offs));
	}
	if (delItems.count() == 0)
		return;
	selectedItemCount = delItems.count();

	if (selectedItemCount > 1)
		undoManager->beginTransaction(Um::Group + "/" + Um::Selection, Um::IGroup,
										Um::Delete, tooltip, Um::IDelete);

	for (uint de = 0; de < selectedItemCount; ++de)
	{
		currItem = delItems.last();
		if ((currItem->asImageFrame()) && ((ScCore->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->PicAvail)))
			ScCore->fileWatcher->removeFile(currItem->Pfile);
		if (currItem->asTextFrame())
		{
			if ((currItem->NextBox != 0) || (currItem->BackBox != 0))
			{
				if (currItem->BackBox == 0)
				{
					currItem->NextBox->BackBox = 0;
					if ((currItem->isAutoText) && (currItem->NextBox == 0))
						LastAuto = 0;
				}
				else
				{
					currItem->BackBox->NextBox = currItem->NextBox;
					if (currItem->NextBox != 0)
						currItem->NextBox->BackBox = currItem->BackBox;
					else
					{
						if (currItem->isAutoText)
							LastAuto = currItem->BackBox;
					}
				}
			}
			else
			{
				if (currItem->isAutoText)
				{
					LastAuto = 0;
					FirstAuto = 0;
				}
			}
			currItem->itemText = StoryText(this);
		}
		if (currItem->isBookmark)
			//CB From view   emit DelBM(currItem);
			m_ScMW->DelBookMark(currItem);
		Items->remove(currItem);
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
	updateFrameItems();
	renumberItemsInListOrder();
	if (selectedItemCount > 1)
		undoManager->commit();
	emit updateContents();
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	//CB FIXME remove this and tree.h too
	m_ScMW->outlinePalette->BuildTree();

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
		emit updateContents();
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
		emit updateContents();
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
		emit updateContents();
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
		emit updateContents();
		changed();
	}
}

void ScribusDoc::itemSelection_SetLineGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
//Teaser for jghali
}

void ScribusDoc::itemSelection_SetFillGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem;
		currItem = itemSelection->itemAt(i);
		currItem->fill_gradient = newGradient;
		if (selectedItemCount==1)
			emit refreshItem(currItem);
	}
	if (selectedItemCount>1)
		emit updateContents();
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
		m_View->DrawNew(); //CB draw new until NLS for redraw through text chains
		changed();
	}
}


void ScribusDoc::itemSelection_SendToLayer(int layerNumber)
{
	uint selectedItemCount=m_Selection->count();
	if (selectedItemCount != 0)
	{
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			undoManager->beginTransaction();
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			currItem->setLayer(layerNumber);
			tooltip += "\t" + currItem->getUName() + "\n";
		}
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			undoManager->commit(Um::Selection,
								Um::IGroup,
								Um::SendToLayer,
								tooltip,
								Um::ILayerAction);
	}

	//CB why not just the following as we are calling for updatecontents anyway and we arent reflowing text
	//Doc->m_Selection->clear();
	m_View->Deselect(true);
	emit updateContents();
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
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount != 0)
	{
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			undoManager->beginTransaction();
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = itemSelection->itemAt(a);
			currItem->setImageXYOffset(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QWMatrix cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
			tooltip += "\t" + currItem->getUName() + "\n";
			emit refreshItem(currItem);
		}
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			undoManager->commit(Um::Selection,
								Um::IGroup,
								Um::ImageOffset,
								tooltip,
								Um::IImageScaling);
		changed();
	}
}

void ScribusDoc::itemSelection_SetImageScale(double x, double y, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount != 0)
	{
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			undoManager->beginTransaction();
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = itemSelection->itemAt(a);
			currItem->setImageXYScale(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QWMatrix cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
			tooltip += "\t" + currItem->getUName() + "\n";
			emit refreshItem(currItem);
		}
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			undoManager->commit(Um::Selection,
								Um::IGroup,
								Um::ImageScale,
								tooltip,
								Um::IImageScaling);
		changed();
	}
}

void ScribusDoc::itemSelection_SetImageScaleAndOffset(double sx, double sy, double ox, double oy, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		undoManager->beginTransaction();
	QString tooltip = Um::ItemsInvolved + "\n";
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		PageItem *currItem = itemSelection->itemAt(a);
		if (UndoManager::undoEnabled())
			undoManager->beginTransaction();
		currItem->setImageXYScale(sx, sy);
		currItem->setImageXYOffset(ox/sx, oy/sy);
		if (currItem->imageClip.size() != 0)
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
			QWMatrix cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		tooltip += "\t" + currItem->getUName() + "\n";
		if (UndoManager::undoEnabled())
		undoManager->commit(Um::Selection,
							Um::IImageFrame,
							Um::ImageScale,
							tooltip,
							Um::IImageScaling);
		emit refreshItem(currItem);
	}
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		undoManager->commit(Um::Selection,
							Um::IGroup,
							Um::ImageScale,
							tooltip,
							Um::IImageScaling);
	changed();
}

void ScribusDoc::buildAlignItemList(Selection* customSelection)
{
	//CB TODO Handling custom selections
	//Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Selection* itemSelection = m_Selection;
	Q_ASSERT(itemSelection!=0);
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
			for (uint a2 = 0; a2 < AObjects.count(); ++a2)
			{
				if (AObjects[a2].Group == ObjGroup)
				{
					AObjects[a2].x1 = QMIN(AObjects[a2].x1, Object.x1);
					AObjects[a2].y1 = QMIN(AObjects[a2].y1, Object.y1);
					AObjects[a2].x2 = QMAX(AObjects[a2].x2, Object.x2);
					AObjects[a2].y2 = QMAX(AObjects[a2].y2, Object.y2);
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
	for (uint i = 0; i < AObjects.count(); ++i)
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
		for (uint j = 0; j < AObjects[i].Objects.count() && !oneLocked; ++j)
			if (AObjects[i].Objects.at(j)->locked())
				oneLocked=true;
	int t = 2;
	if (oneLocked)
	{
		t = ScMessageBox::warning(m_ScMW, CommonStrings::trWarning, tr("Some objects are locked."),
									tr("&Unlock All"), tr("&Skip locked objects"), CommonStrings::tr_Cancel, 0, 0);
		if (t == 2)
			return false;
	}
	
	QString targetTooltip = Um::ItemsInvolved + "\n";
	for (uint i = 0; i < m_Selection->count(); ++i)
		targetTooltip += "\t" + m_Selection->itemAt(i)->getUName() + "\n";
	// Make the align action a single action in Action History
	undoManager->beginTransaction(Um::Selection, 0, Um::AlignDistribute, targetTooltip, Um::IAlignDistribute);
	if (oneLocked && (t == 0))
	{
		for (uint i = 0; i < alignObjectsCount; ++i)
			for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
				if (AObjects[i].Objects.at(j)->locked())
					AObjects[i].Objects.at(j)->setLocked(false);
	}
	return true;
}

void ScribusDoc::endAlign()
{
	changed();
	m_ScMW->HaveNewSel(m_Selection->itemAt(0)->itemType());
	for (uint i = 0; i < m_Selection->count(); ++i)
		setRedrawBounding(m_Selection->itemAt(i));
	undoManager->commit(); // commit and send the action to the UndoManager
	m_View->updateContents();
}


void ScribusDoc::itemSelection_AlignLeftOut(AlignTo currAlignTo, double guidePosition)
{
	if (!startAlign())
		return;
	uint alignObjectsCount=AObjects.count();
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newX = QMIN(AObjects[a].x1, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newX-AObjects[i].x2;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newX = QMIN(AObjects[a].x1, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newX-AObjects[i].x1;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
	double newX;
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
				minX = QMIN(AObjects[a].x1, minX);
				maxX = QMAX(AObjects[a].x2, maxX);
			}
			newX = minX + (maxX-minX)/2;
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newX-AObjects[i].x1-(AObjects[i].width)/2;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newX = QMAX(AObjects[a].x2, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newX-AObjects[i].x2;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newX = QMAX(AObjects[a].x2, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newX-AObjects[i].x1;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newY = QMIN(AObjects[a].y1, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newY-AObjects[i].y2;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newY = QMIN(AObjects[a].y1, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newY-AObjects[i].y1;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
	double newY;
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
				minY = QMIN(AObjects[a].y1, minY);
				maxY = QMAX(AObjects[a].y2, maxY);
			}
			newY = minY + (maxY-minY)/2;
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newY-AObjects[i].y1-(AObjects[i].height)/2;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newY = QMAX(AObjects[a].y2, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newY-AObjects[i].y2;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
	int loopStart=0, loopEnd=alignObjectsCount;
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
				newY = QMAX(AObjects[a].y2, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newY-AObjects[i].y1;
		for (uint j = 0; j < AObjects[i].Objects.count(); ++j)
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
		Xsorted.insert(AObjects[a].x1, a, false);
		
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
		double diff=minX + i*separation-AObjects[it.data()].x1;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(diff, 0.0);
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
		Xsorted.insert(AObjects[a].x1+(AObjects[a].width)/2, a, false);
		
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
		double diff=minX + i*separation-AObjects[it.data()].x1-(AObjects[it.data()].width)/2;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(diff, 0.0);
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
		Xsorted.insert(AObjects[a].x2, a, false);
		
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
		double diff=minX + i*separation-AObjects[it.data()].x2;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(diff, 0.0);
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
		X1sorted.insert(AObjects[a].x1, a, false);
		X2sorted.insert(AObjects[a].x2, a, false);
	}	
	uint left=X1sorted.begin().data();
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
		if (it.data()==left)
			continue;
		if (it.data()==right && !usingDistance)
			continue;
		currX+=separation;

		double diff=currX-AObjects[it.data()].x1;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(diff, 0.0);
		currX+=AObjects[it.data()].width;
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
		Ysorted.insert(AObjects[a].y2, a, false);
		
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
		double diff=minY + i*separation-AObjects[it.data()].y2;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(0.0, diff);
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
		Ysorted.insert(AObjects[a].y1+(AObjects[a].height)/2, a, false);
		
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
		double diff=minY + i*separation-AObjects[it.data()].y1-(AObjects[it.data()].height)/2;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(0.0, diff);
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
		Ysorted.insert(AObjects[a].y1, a, false);
		
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
		double diff=minY + i*separation-AObjects[it.data()].y1;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(0.0,diff);
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
		Y1sorted.insert(AObjects[a].y1, a, false);
		Y2sorted.insert(AObjects[a].y2, a, false);
	}	
	uint top=Y1sorted.begin().data();
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
		if (it.data()==top)
			continue;
		if (it.data()==bottom && !usingDistance)
			continue;
		currY+=separation;

		double diff=currY-AObjects[it.data()].y1;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(0.0,diff);
		currY+=AObjects[it.data()].height;
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
		X1sorted.insert(AObjects[a].x1, a, false);
		X2sorted.insert(AObjects[a].x2, a, false);
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
		double diff=currX-AObjects[it.data()].x1;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(diff, 0.0);
		currX+=AObjects[it.data()].width;
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
		Y1sorted.insert(AObjects[a].y1, a, false);
		Y2sorted.insert(AObjects[a].y2, a, false);
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
		double diff=currY-AObjects[it.data()].y1;
		for (uint j = 0; j < AObjects[it.data()].Objects.count(); ++j)
			if (!AObjects[it.data()].Objects.at(j)->locked())
				AObjects[it.data()].Objects.at(j)->moveBy(0.0, diff);
		currY+=AObjects[it.data()].height;
	}
	endAlign();
}

void ScribusDoc::changed()
{
	modified=true;
	emit docChanged();
}

Page* ScribusDoc::currentPage()
{
	return _currentPage;
}

void ScribusDoc::setCurrentPage(Page *newPage)
{
	if (newPage==NULL)
		return;
	_currentPage = newPage;
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
	if (UndoManager::undoEnabled())
	{ // Make multiple duplicate a single action in the action history
		if (m_Selection->count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::MultipleDuplicate,"",Um::IMultipleDuplicate);
		else
		{
			PageItem* item=m_Selection->itemAt(0);
			undoManager->beginTransaction(item->getUName(), item->getUPixmap(), Um::MultipleDuplicate, "", Um::IMultipleDuplicate);
		}
	}
	//FIXME: Enable paste without doing this save/restore, and stop using paste with all the refreshes
	bool savedAlignGrid = useRaster;
	bool savedAlignGuides = SnapGuides;
	useRaster = false;
	SnapGuides = false;
	if (mdData.type==0) // Copy and offset or set a gap
	{
		double dH = mdData.copyShiftGapH / docUnitRatio;
		double dV = mdData.copyShiftGapV / docUnitRatio;
		double dH2 = dH;
		double dV2 = dV;
		if (mdData.copyShiftOrGap==1)
		{
			dH2+=m_Selection->width();
			dV2+=m_Selection->height();
		}
		m_ScMW->slotEditCopy();
		m_View->Deselect(true);
		for (int i=0; i<mdData.copyCount; ++i)
		{
			m_ScMW->slotEditPaste();
			for (uint b=0; b<m_Selection->count(); ++b)
			{
				PageItem* bItem=m_Selection->itemAt(b);
				bItem->setLocked(false);
				MoveItem(dH2, dV2, bItem, true);
			}
			dH2 += dH;
			dV2 += dV;
		}
		changed();
		m_View->Deselect(true);
		tooltip = tr("Number of copies: %1\nHorizontal shift: %2\nVertical shift: %3").arg(mdData.copyCount).arg(dH).arg(dV);
	}
	else
	if (mdData.type==1) // Create a grid of duplicated items
	{
		m_ScMW->slotEditCopy();
		int copyCount=mdData.gridRows*mdData.gridCols;
		double dX=mdData.gridGapH/docUnitRatio + m_Selection->width();
		double dY=mdData.gridGapV/docUnitRatio + m_Selection->height();
		m_View->Deselect(true);
		for (int i=0; i<mdData.gridRows; ++i) //skip 0, the item is the one we are copying
		{
			for (int j=0; j<mdData.gridCols; ++j) //skip 0, the item is the one we are copying
			{
				if (i==0 && j==0)
					continue;
				m_ScMW->slotEditPaste();
				for (uint b=0; b<m_Selection->count(); ++b)
				{
					PageItem* bItem=m_Selection->itemAt(b);
					bItem->setLocked(false);
					MoveItem(j*dX, i*dY, bItem, true);
				}
			}
		}
		changed();
		m_View->Deselect(true);
		tooltip = tr("Number of copies: %1\nHorizontal gap: %2\nVertical gap: %3").arg(copyCount-1).arg(mdData.gridGapH).arg(mdData.gridGapV);
	}
	if (UndoManager::undoEnabled())
		undoManager->commit("", 0, "", tooltip, 0);
	//FIXME: Enable paste without doing this save/restore
	useRaster = savedAlignGrid;
	SnapGuides = savedAlignGuides;
}

void ScribusDoc::itemSelection_ApplyImageEffects(ScImageEffectList& newEffectList, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=NULL);
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
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		undoManager->beginTransaction();
	QString tooltip = Um::ItemsInvolved + "\n";
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		PageItem *currItem = itemSelection->itemAt(a);
		if (!currItem->asLine())
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
		emit refreshItem(currItem);
	}
	QString t;
	if (startArrowID!=-1 && endArrowID!=-1)
		t=Um::StartAndEndArrow;
	else
		t=(startArrowID!=-1) ? Um::StartArrow : Um::EndArrow;
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		undoManager->commit(Um::Selection,
							Um::IGroup,
							t,
							tooltip,
							Um::IArrow);
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
	int createCount=QMAX(pageCount,1);
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

void ScribusDoc::getClosestGuides(double xin, double yin, double *xout, double *yout)
{
	m_View->GxM = -1;
	m_View->GyM = -1;
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
		m_View->GyM = tmpGuidesSel.begin().data();
		*yout = tmpGuides[m_View->GyM];
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
		m_View->GxM = tmpGuidesSel.begin().data();
		*xout = tmpGuides[m_View->GxM];
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
		m_View->GyM = tmpGuidesSel.begin().data();
		*yout = tmpGuides[m_View->GyM];
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
		m_View->GxM = tmpGuidesSel.begin().data();
		*xout = tmpGuides[m_View->GxM];
	}
}

//CB-->Doc
void ScribusDoc::SnapToGuides(PageItem *currItem)
{
	int pg = OnPage(currItem);
	double xout, yout;
	if (pg == -1)
		return;
	Page* page = Pages->at(pg);

	getClosestGuides(0, currItem->yPos(), &xout, &yout);
	if (m_View->GyM != -1)
		currItem->setYPos(yout+page->yOffset());
	if (currItem->asLine())
	{
		QWMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
		getClosestGuides(0, my, &xout, &yout);
		if (m_View->GyM != -1)
			currItem->moveBy(0.0, yout - my + page->yOffset());
	}
	else
	{
		getClosestGuides(0, currItem->yPos()+currItem->height(), &xout, &yout);
		if (m_View->GyM != -1)
			currItem->setYPos(yout-currItem->height()+page->yOffset());
	}
	getClosestGuides(currItem->xPos(), 0, &xout, &yout);
	if (m_View->GxM != -1)
		currItem->setXPos(xout+page->xOffset());
	if (currItem->asLine())
	{
		QWMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
		getClosestGuides(mx,  0, &xout, &yout);
		if (m_View->GxM != -1)
			currItem->moveBy(xout - mx + page->xOffset(), 0.0);
	}
	else
	{
		getClosestGuides(currItem->xPos()+currItem->width(), 0, &xout, &yout);
		if (m_View->GxM != -1)
			currItem->setXPos(xout-currItem->width()+page->xOffset());
	}
}


//CB-->Doc?
bool ScribusDoc::ApplyGuides(double *x, double *y)
{
//	m_SnapCounter++;
	bool ret = false;
	double xout, yout;
	int pg = OnPage(*x, *y);
	if (pg == -1)
		return ret;
	Page* page = Pages->at(pg);
//	if ((SnapGuides) && (m_SnapCounter > 1))
	if (SnapGuides)
	{
//		m_SnapCounter = 0;
		getClosestGuides(*x, *y, &xout, &yout);
		if (m_View->GxM != -1)
		{
			*x = xout+page->xOffset();
			ret = true;
		}
		if (m_View->GyM != -1)
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

//CB-->Doc
bool ScribusDoc::MoveItem(double newX, double newY, PageItem* currItem, bool fromMP)
{
	if (currItem->locked())
		return false;
	bool retw = false;
	double oldx = currItem->xPos();
	double oldy = currItem->yPos();
	currItem->moveBy(newX, newY);
	if ((useRaster) && (!m_View->operItemMoving) && (!fromMP) && (static_cast<int>(currentPage()->pageNr()) == currItem->OwnPage))
	{
		currItem->setXYPos(qRound(currItem->xPos() / guidesSettings.minorGrid) * guidesSettings.minorGrid, qRound(currItem->yPos() / guidesSettings.minorGrid) * guidesSettings.minorGrid);
	}
	if ((SnapGuides) && (!m_View->operItemMoving) && (appMode == modeNormal) && (!EditClip) && (!fromMP))
		SnapToGuides(currItem);
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
	QRect oldR(currItem->getRedrawBounding(m_View->scale()));
	setRedrawBounding(currItem);
	QRect newR(currItem->getRedrawBounding(m_View->scale()));
	if ((!m_View->operItemMoving) && (!currItem->Sizing))
		emit updateContents(newR.unite(oldR));
	currItem->OwnPage = OnPage(currItem);
	return retw;
}

//CB-->Doc
void ScribusDoc::RotateItem(double win, int ite)
{
	RotateItem(win, Items->at(ite));
}

//CB-->Doc
void ScribusDoc::RotateItem(double win, PageItem *currItem)
{
	if (currItem->locked())
		return;
	QRect oldR(currItem->getRedrawBounding(m_View->scale()));
//	if ((Doc->RotMode != 0) && (m_MouseButtonPressed))
	if (RotMode != 0)
	{
		QWMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.scale(1, 1);
		ma.rotate(currItem->rotation());
		double ro = win-currItem->rotation();
		currItem->setRotation(win);
		FPoint n(0,0);
		switch (RotMode)
		{
		case 2:
			ma.translate(currItem->width()/2, currItem->height()/2);
			n = FPoint(-currItem->width()/2, -currItem->height()/2);
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
		MoveItem(x-currItem->xPos(), y-currItem->yPos(), currItem);
		currItem->Sizing = oldS;
	}
	else
	{
		currItem->setRotation(win);
		setRedrawBounding(currItem);
	}
	QRect newR(currItem->getRedrawBounding(m_View->scale()));
	emit updateContents(newR.unite(oldR));
	//emit SetAngle(currItem->rotation());
}


//CB-->Doc
void ScribusDoc::MoveRotated(PageItem *currItem, FPoint npv, bool fromMP)
{
	QWMatrix ma;
	ma.translate(currItem->xPos(), currItem->yPos());
	ma.rotate(currItem->rotation());
	double mxc = currItem->xPos() - (ma.m11() * npv.x() + ma.m21() * npv.y() + ma.dx());
	double myc = currItem->yPos() - (ma.m22() * npv.y() + ma.m12() * npv.x() + ma.dy());
	MoveItem(-mxc, -myc, currItem, fromMP);
}

//CB-->Doc
bool ScribusDoc::SizeItem(double newX, double newY, int ite, bool fromMP, bool DoUpdateClip, bool redraw)
{
	return SizeItem(newX, newY, Items->at(ite), fromMP, DoUpdateClip, redraw);
}

//CB-->Doc
bool ScribusDoc::SizeItem(double newX, double newY, PageItem *pi, bool fromMP, bool DoUpdateClip, bool redraw)
{
	PageItem *currItem = pi;
	if (currItem->locked())
		return false;
	QPainter p;
	double viewScale=m_View->scale();
	QRect oldR(currItem->getRedrawBounding(viewScale));
	//Uncomment for stopping resize in any direction
	//and remove the height/width <0 tests in item sizing switch
	/*
	if (!currItem->asLine())
	{
		newX = QMAX(newX, 1);
		newY = QMAX(newY, 1);
	}
	*/
	int ph = static_cast<int>(QMAX(1.0, currItem->lineWidth() / 2.0));
	QWMatrix ma;
	ma.rotate(currItem->rotation());
	double dX = ma.m11() * (currItem->width() - newX) + ma.m21() * (currItem->height() - newY) + ma.dx();
	double dY = ma.m22() * (currItem->height() - newY) + ma.m12() * (currItem->width() - newX) + ma.dy();
	currItem->setWidthHeight(newX, newY);
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
	currItem->setCornerRadius(QMIN(currItem->cornerRadius(), QMIN(currItem->width(),currItem->height())/2));
	if ((currItem->asImageFrame()) && (!currItem->Sizing) && (!EditClip))
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
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		if ((m_View->frameResizeHandle == 1) && !(currItem->asLine()))
			currItem->paintObj();
		if ((currItem->FrameType == 0) || (currItem->asLine()) || (m_View->frameResizeHandle != 1))
			return true;
		QPainter p;
		p.begin(m_View->viewport());
		QPoint in(qRound((currItem->xPos()-minCanvasCoordinate.x())*viewScale), qRound((currItem->yPos()-minCanvasCoordinate.y())*viewScale));
		QPoint out(m_View->contentsToViewport(in));
		p.translate(out.x(), out.y());
		p.scale(viewScale, viewScale);
		p.rotate(currItem->rotation());
		p.setRasterOp(XorROP);
		p.setBrush(NoBrush);
		p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
		p.save();
		if (currItem->OldB2 < 0.0)
		{
			p.scale(-1, 1);
			p.translate(qRound(-currItem->OldB2), 0);
		}
		if (currItem->OldH2 < 0.0)
		{
			p.scale(1, -1);
			p.translate(0, qRound(-currItem->OldH2));
		}
		currItem->DrawPolyL(&p, currItem->Clip);
		p.restore();
		currItem->updateClip();
		currItem->updateGradientVectors();
		p.save();
		if (currItem->width() < 0.0)
		{
			p.scale(-1, 1);
			p.translate(qRound(-currItem->width()), 0);
		}
		if (currItem->height() < 0.0)
		{
			p.scale(1, -1);
			p.translate(0, qRound(-currItem->height()));
		}
		currItem->DrawPolyL(&p, currItem->Clip);
		p.restore();
		p.end();
		return true;
	}
	if (DoUpdateClip)
	{
		double nX=0.0, nY=0.0;
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
			currItem->moveImageInFrame(dX,dY);
		currItem->updateClip();
	}
	currItem->updateGradientVectors();
	if (redraw)
	{
		QRect newR(currItem->getRedrawBounding(viewScale));
		emit updateContents(newR.unite(oldR));
	}
	if (!fromMP)
	{
		if (m_Selection->isMultipleSelection())
		{
			double gx, gy, gh, gw;
			m_Selection->setGroupRect();
			m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			qDebug("doc, emit w&h, when was this used?");
			emit widthAndHeight(gw, gh);
		}
	}
	return true;
}

//CB-->Doc
bool ScribusDoc::MoveSizeItem(FPoint newX, FPoint newY, int ite, bool fromMP, bool constrainRotation)
{
	PageItem *currItem = Items->at(ite);
	double viewScale=m_View->scale();
	QRect oldR(currItem->getRedrawBounding(viewScale));
	if (currItem->asLine())
	{
		QWMatrix ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.dx();
		double my = ma.m12() * currItem->width() + ma.dy();
		MoveItem(newX.x(), newX.y(), currItem, fromMP);
		double newRot=xy2Deg(mx - currItem->xPos(), my - currItem->yPos());
		//CB Hmm should work, doesnt. (constraining on the first point of a line)
		//if (constrainRotation)
		//	qDebug(QString("%1").arg(constrainAngle(newRot)));
		currItem->setRotation(newRot);
		currItem->setWidthHeight(sqrt(pow(mx - currItem->xPos(),2)+pow(my - currItem->yPos(),2)), 1.0);
		currItem->updateClip();
		setRedrawBounding(currItem);
		QRect newR(currItem->getRedrawBounding(viewScale));
		emit updateContents(oldR);
		emit updateContents(newR);
//		updateContents(newR.unite(oldR));
	}
	else
	{
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		if (currItem->rotation() != 0)
		{
			FPoint npv(newX.x(), newX.y());
			QWMatrix ma3;
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

//CB-->Doc
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
	currItem->PoLine = Clip.copy();
	if (currItem->asPolyLine())
		currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2, 1)));
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->Sizing = siz;
}
