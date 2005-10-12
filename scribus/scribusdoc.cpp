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
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribusXml.h"

#include <qfile.h>

#include "page.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#include "undomanager.h"
#include "undostate.h"
#include "prefsmanager.h"
#include "layers.h"
#include "units.h"
#include "filewatcher.h"
#include "hyphenator.h"


extern ScribusQApp* ScQApp;

#ifdef HAVE_CMS
extern cmsHPROFILE CMSoutputProf;
extern cmsHPROFILE CMSprinterProf;
extern cmsHTRANSFORM stdTransG;
extern cmsHTRANSFORM stdProofG;
extern cmsHTRANSFORM stdTransImgG;
extern cmsHTRANSFORM stdProofImgG;
extern cmsHTRANSFORM stdTransCMYKG;
extern cmsHTRANSFORM stdProofCMYKG;
extern cmsHTRANSFORM stdTransRGBG;
extern cmsHTRANSFORM stdProofGCG;
extern cmsHTRANSFORM stdProofCMYKGCG;
extern bool BlackPoint;
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
extern int IntentMonitor;
extern int IntentPrinter;
#endif

extern bool CMSavail;
extern QPixmap loadIcon(QString nam);

ScribusDoc::ScribusDoc() : UndoObject( tr("Document"))
{
	ApplicationPrefs* prefsData=&(PrefsManager::instance()->appPrefs);
	modified = false;
	masterPageMode = false;
	_itemCreationTransactionStarted=false;
	NrItems = 0;
	First = 1;
	Last = 0;
	viewCount = 0;
	viewID = 0;
	is12doc=false;
	UsedFonts.clear();
	FT_Init_FreeType( &library );
	AllFonts = &prefsData->AvailFonts;
	AddFont(prefsData->toolSettings.defFont);//, prefsData->AvailFonts[prefsData->toolSettings.defFont]->Font);
	toolSettings.defFont = prefsData->toolSettings.defFont;
	toolSettings.defSize = prefsData->toolSettings.defSize;
	toolSettings.tabFillChar = prefsData->toolSettings.tabFillChar;
	guidesSettings.marginsShown = prefsData->guidesSettings.marginsShown;
	guidesSettings.framesShown = prefsData->guidesSettings.framesShown;
	guidesSettings.gridShown = prefsData->guidesSettings.gridShown;
	guidesSettings.guidesShown = prefsData->guidesSettings.guidesShown;
	guidesSettings.baseShown = prefsData->guidesSettings.baseShown;
	guidesSettings.linkShown = prefsData->guidesSettings.linkShown;
	guidesSettings.showPic = prefsData->guidesSettings.showPic;
	guidesSettings.showControls = prefsData->guidesSettings.showControls;
	guidesSettings.rulerMode = prefsData->guidesSettings.rulerMode;
	guidesSettings.grabRad = prefsData->guidesSettings.grabRad;
	guidesSettings.guideRad = prefsData->guidesSettings.guideRad;
	guidesSettings.minorGrid = prefsData->guidesSettings.minorGrid;
	guidesSettings.majorGrid = prefsData->guidesSettings.majorGrid;
	guidesSettings.minorColor = prefsData->guidesSettings.minorColor;
	guidesSettings.majorColor = prefsData->guidesSettings.majorColor;
	papColor = prefsData->DpapColor;
	guidesSettings.margColor = prefsData->guidesSettings.margColor;
	guidesSettings.guideColor = prefsData->guidesSettings.guideColor;
	guidesSettings.baseColor = prefsData->guidesSettings.baseColor;
	guidesSettings.before = prefsData->guidesSettings.before;
	PageColors.clear();
	PageColors.insert("Black", ScColor(0, 0, 0, 255));
	PageColors.insert("White", ScColor(0, 0, 0, 0));
	if (prefsData->toolSettings.dPen != "None")
		PageColors.insert(prefsData->toolSettings.dPen, prefsData->DColors[prefsData->toolSettings.dPen]);
	toolSettings.dPen = prefsData->toolSettings.dPen;
	if (prefsData->toolSettings.dPenLine != "None")
		PageColors.insert(prefsData->toolSettings.dPenLine, prefsData->DColors[prefsData->toolSettings.dPenLine]);
	toolSettings.dPenLine = prefsData->toolSettings.dPenLine;
	if (prefsData->toolSettings.dPenText != "None")
		PageColors.insert(prefsData->toolSettings.dPenText, prefsData->DColors[prefsData->toolSettings.dPenText]);
	toolSettings.dPenText = prefsData->toolSettings.dPenText;
	if (prefsData->toolSettings.dStrokeText != "None")
		PageColors.insert(prefsData->toolSettings.dStrokeText, prefsData->DColors[prefsData->toolSettings.dStrokeText]);
	toolSettings.dStrokeText = prefsData->toolSettings.dStrokeText;
	if (prefsData->toolSettings.dBrush != "None")
		PageColors.insert(prefsData->toolSettings.dBrush, prefsData->DColors[prefsData->toolSettings.dBrush]);
	toolSettings.dBrush = prefsData->toolSettings.dBrush;
	if (prefsData->toolSettings.dBrushPict != "None")
		PageColors.insert(prefsData->toolSettings.dBrushPict, prefsData->DColors[prefsData->toolSettings.dBrushPict]);
	toolSettings.dBrushPict = prefsData->toolSettings.dBrushPict;
	if (prefsData->toolSettings.dTextBackGround != "None")
		PageColors.insert(prefsData->toolSettings.dTextBackGround, prefsData->DColors[prefsData->toolSettings.dTextBackGround]);
	toolSettings.dTextBackGround = prefsData->toolSettings.dTextBackGround;
	if (prefsData->toolSettings.dTextLineColor != "None")
		PageColors.insert(prefsData->toolSettings.dTextLineColor, prefsData->DColors[prefsData->toolSettings.dTextLineColor]);
	toolSettings.dTextLineColor = prefsData->toolSettings.dTextLineColor;
	toolSettings.dTextBackGroundShade = prefsData->toolSettings.dTextBackGroundShade;
	toolSettings.dTextLineShade = prefsData->toolSettings.dTextLineShade;
	toolSettings.dTextPenShade = prefsData->toolSettings.dTextPenShade;
	toolSettings.dTextStrokeShade = prefsData->toolSettings.dTextStrokeShade;
	typographicSettings.valueSuperScript = prefsData->typographicSettings.valueSuperScript;
	typographicSettings.scalingSuperScript = prefsData->typographicSettings.scalingSuperScript;
	typographicSettings.valueSubScript = prefsData->typographicSettings.valueSubScript;
	typographicSettings.scalingSubScript = prefsData->typographicSettings.scalingSubScript;
	typographicSettings.valueSmallCaps = prefsData->typographicSettings.valueSmallCaps;
	typographicSettings.autoLineSpacing = prefsData->typographicSettings.autoLineSpacing;
	typographicSettings.valueBaseGrid = prefsData->typographicSettings.valueBaseGrid;
	typographicSettings.offsetBaseGrid = prefsData->typographicSettings.offsetBaseGrid;
	typographicSettings.valueUnderlinePos = prefsData->typographicSettings.valueUnderlinePos;
	typographicSettings.valueUnderlineWidth = prefsData->typographicSettings.valueUnderlineWidth;
	typographicSettings.valueStrikeThruPos = prefsData->typographicSettings.valueStrikeThruPos;
	typographicSettings.valueStrikeThruWidth = prefsData->typographicSettings.valueStrikeThruWidth;
	toolSettings.dShade = prefsData->toolSettings.dShade;
	toolSettings.dShade2 = prefsData->toolSettings.dShade2;
	toolSettings.shadePict = prefsData->toolSettings.shadePict;
	toolSettings.scaleX = prefsData->toolSettings.scaleX;
	toolSettings.scaleY = prefsData->toolSettings.scaleY;
	toolSettings.scaleType = prefsData->toolSettings.scaleType;
	toolSettings.aspectRatio = prefsData->toolSettings.aspectRatio;
	toolSettings.useEmbeddedPath = prefsData->toolSettings.useEmbeddedPath;
	toolSettings.lowResType = prefsData->toolSettings.lowResType;
	toolSettings.dCols = prefsData->toolSettings.dCols;
	toolSettings.dGap = prefsData->toolSettings.dGap;
	toolSettings.dTabWidth = prefsData->toolSettings.dTabWidth;
	toolSettings.dLineArt = PenStyle(prefsData->toolSettings.dLineArt);
	toolSettings.dWidth = prefsData->toolSettings.dWidth;
	toolSettings.dShadeLine = prefsData->toolSettings.dShadeLine;
	toolSettings.dLstyleLine = PenStyle(prefsData->toolSettings.dLstyleLine);
	toolSettings.dWidthLine = prefsData->toolSettings.dWidthLine;
	toolSettings.dStartArrow = prefsData->toolSettings.dStartArrow;
	toolSettings.dEndArrow = prefsData->toolSettings.dEndArrow;
	toolSettings.polyC = prefsData->toolSettings.polyC;
	toolSettings.polyF = prefsData->toolSettings.polyF;
	toolSettings.polyS = prefsData->toolSettings.polyS;
	toolSettings.polyFd = prefsData->toolSettings.polyFd;
	toolSettings.polyR = prefsData->toolSettings.polyR;
	toolSettings.magMin = prefsData->toolSettings.magMin;
	toolSettings.magMax = prefsData->toolSettings.magMax;
	toolSettings.magStep = prefsData->toolSettings.magStep;
	checkerProfiles = prefsData->checkerProfiles;
	curCheckProfile = prefsData->curCheckProfile;
	docUnitIndex = prefsData->docUnitIndex;
	marginColored = prefsData->marginColored;
	Language = prefsData->Language;
	MinWordLen = prefsData->MinWordLen;
	HyCount = prefsData->HyCount;
	Automatic = prefsData->Automatic;
	AutoCheck = prefsData->AutoCheck;
	GuideLock = false;
	SnapGuides = false;
	useRaster = false;
	EditClip = false;
	EditClipMode = 0;
	loading = false;
	DocName = tr("Document")+"-";
	CurrentSel = -1;
	pageCount = 0;
	LastAuto = 0;
	FirstAuto = 0;
	DraggedElem = 0;
	GroupCounter = 1;
	rulerXoffset = 0.0;
	rulerYoffset = 0.0;
	docParagraphStyles.clear();
	struct ParagraphStyle vg;
	vg.Vname = "Normal Internal";
	vg.LineSpaMode = 0;
	vg.LineSpa = 15;
	vg.textAlignment = 0;
	vg.Indent = 0;
	vg.First = 0;
	vg.gapBefore = 0;
	vg.gapAfter = 0;
	vg.Font = "";
	vg.FontSize = toolSettings.defSize;
	vg.TabValues.clear();
	vg.Drop = false;
	vg.DropLin = 2;
	vg.DropDist = 0;
	vg.FontEffect = 0;
	vg.FColor = toolSettings.dBrush;
	vg.FShade = toolSettings.dShade;
	vg.SColor = toolSettings.dPen;
	vg.SShade = toolSettings.dShade2;
	vg.BaseAdj = false;
	vg.txtShadowX = 50;
	vg.txtShadowY = -50;
	vg.txtOutline = 10;
	vg.txtUnderPos = typographicSettings.valueUnderlinePos;
	vg.txtUnderWidth = typographicSettings.valueUnderlineWidth;
	vg.txtStrikePos = typographicSettings.valueStrikeThruPos;
	vg.txtStrikeWidth = typographicSettings.valueStrikeThruPos;
	vg.scaleH = 1000;
	vg.scaleV = 1000;
	vg.baseOff = 0;
	vg.kernVal = 0;
	docParagraphStyles.append(vg);
	vg.Vname = "Center Internal";
	vg.textAlignment = 1;
	docParagraphStyles.append(vg);
	vg.Vname = "Rechts Internal";
	vg.textAlignment = 2;
	docParagraphStyles.append(vg);
	vg.Vname = "Block Internal";
	vg.textAlignment = 3;
	docParagraphStyles.append(vg);
	vg.Vname = "EBlock Internal";
	vg.textAlignment = 4;
	docParagraphStyles.append(vg);
	Layers.clear();
	struct Layer ll;
	ll.LNr = 0;
	ll.Level = 0;
	ll.Name = tr("Background");
	ll.isViewable = true;
	ll.isPrintable = true;
	Layers.append(ll);
	ActiveLayer = 0;
	JavaScripts.clear();
	CurrentStyle = 0;
	currentParaStyle = 0;
	TotalItems = 0;
	masterPageMode = false;
	PDF_Options.Thumbnails = prefsData->PDF_Options.Thumbnails;
	PDF_Options.Articles = prefsData->PDF_Options.Articles;
	PDF_Options.Compress = prefsData->PDF_Options.Compress;
	PDF_Options.CompressMethod = prefsData->PDF_Options.CompressMethod;
	PDF_Options.Quality = prefsData->PDF_Options.Quality;
	PDF_Options.RecalcPic = prefsData->PDF_Options.RecalcPic;
	PDF_Options.Bookmarks = prefsData->PDF_Options.Bookmarks;
	PDF_Options.PicRes = prefsData->PDF_Options.PicRes;
	// Fixme: Check PDF version input
	PDF_Options.Version = (PDFOptions::PDFVersion)prefsData->PDF_Options.Version;
	PDF_Options.Resolution = prefsData->PDF_Options.Resolution;
	PDF_Options.Binding = prefsData->PDF_Options.Binding;
	PDF_Options.EmbedList = prefsData->PDF_Options.EmbedList;
	PDF_Options.SubsetList = prefsData->PDF_Options.SubsetList;
	PDF_Options.MirrorH = prefsData->PDF_Options.MirrorH;
	PDF_Options.MirrorV = prefsData->PDF_Options.MirrorV;
	PDF_Options.RotateDeg = prefsData->PDF_Options.RotateDeg;
	PDF_Options.PresentMode = prefsData->PDF_Options.PresentMode;
	PDF_Options.Datei = prefsData->PDF_Options.Datei;
	PDF_Options.PresentVals = prefsData->PDF_Options.PresentVals;
	PDF_Options.isGrayscale = prefsData->PDF_Options.isGrayscale;
	PDF_Options.UseRGB = prefsData->PDF_Options.UseRGB;
	PDF_Options.UseProfiles = prefsData->PDF_Options.UseProfiles;
	PDF_Options.UseProfiles2 = prefsData->PDF_Options.UseProfiles2;
	PDF_Options.SolidProf = prefsData->PDF_Options.SolidProf;
	PDF_Options.SComp = prefsData->PDF_Options.SComp;
	PDF_Options.ImageProf = prefsData->PDF_Options.ImageProf;
	PDF_Options.PrintProf = prefsData->PDF_Options.PrintProf;
	PDF_Options.Info = prefsData->PDF_Options.Info;
	PDF_Options.Intent = prefsData->PDF_Options.Intent;
	PDF_Options.Intent2 = prefsData->PDF_Options.Intent2;
	PDF_Options.BleedTop = prefsData->PDF_Options.BleedTop;
	PDF_Options.BleedLeft = prefsData->PDF_Options.BleedLeft;
	PDF_Options.BleedRight = prefsData->PDF_Options.BleedRight;
	PDF_Options.BleedBottom = prefsData->PDF_Options.BleedBottom;
	PDF_Options.EmbeddedI = prefsData->PDF_Options.EmbeddedI;
	PDF_Options.Encrypt = prefsData->PDF_Options.Encrypt;
	PDF_Options.PassOwner = prefsData->PDF_Options.PassOwner;
	PDF_Options.PassUser = prefsData->PDF_Options.PassUser;
	PDF_Options.Permissions = prefsData->PDF_Options.Permissions;
	PDF_Options.UseLPI = prefsData->PDF_Options.UseLPI;
	PDF_Options.LPISettings = prefsData->PDF_Options.LPISettings;
	PDF_Options.useLayers = prefsData->PDF_Options.useLayers;
	PDF_Options.doMultiFile = prefsData->PDF_Options.doMultiFile;

	docItemAttributes = prefsData->defaultItemAttributes;
	docToCSetups = prefsData->defaultToCSetups;

	RePos = false;
	BookMarks.clear();
	OldBM = false;
	hasName = false;
	DoDrawing = true;
	RotMode = 0;
	SubMode = -1;
	autoSaveTimer = new QTimer(this);
	MLineStyles.clear();
	Pages.clear();
	MasterPages.clear();
	DocPages.clear();
	Items.clear();
	MasterItems.clear();
	DocItems.clear();
	FrameItems.clear();
	ScratchLeft = prefsData->ScratchLeft;
	ScratchRight = prefsData->ScratchRight;
	ScratchTop = prefsData->ScratchTop;
	ScratchBottom = prefsData->ScratchBottom;
	pageSets = prefsData->pageSets;
	minCanvasCoordinate = FPoint(0, 0);
	maxCanvasCoordinate = FPoint(ScratchLeft + ScratchRight, ScratchTop + ScratchBottom);
	arrowStyles = prefsData->arrowStyles;
	undoManager = UndoManager::instance();
	docItemErrors.clear();
	masterItemErrors.clear();
	addSymbols();
}

ScribusDoc::~ScribusDoc()
{
	MasterItems.setAutoDelete(true);
	DocItems.setAutoDelete(true);
	FrameItems.setAutoDelete(true);
	MasterItems.clear();
	DocItems.clear();
	FrameItems.clear();
	MasterPages.setAutoDelete(true);
	DocPages.setAutoDelete(true);
	MasterPages.clear();
	DocPages.clear();
	QMap<QString,int>::Iterator it3;
	for (it3 = UsedFonts.begin(); it3 != UsedFonts.end(); ++it3)
	{
		if (!(*AllFonts)[it3.key()]->PrivateFont.isEmpty())
			(*AllFonts).removeFont(it3.key());
	}
	FT_Done_FreeType( library );
}

void ScribusDoc::setup(const int unitIndex, const int fp, const int firstLeft, const int orientation, const int firstPageNumber, const QString& defaultPageSize, const QString& documentName)
{
	docUnitIndex=unitIndex;
	pageSets[fp].FirstPage = firstLeft;
	PageOri = orientation;
	PageSize = defaultPageSize;
	FirstPnum = firstPageNumber;
	currentPageLayout = fp;
	setName(documentName);
	HasCMS = true;

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

	appMode = modeNormal;
	PrefsManager *prefsManager=PrefsManager::instance();
	PageColors = prefsManager->colorSet();

	CMSSettings.DefaultImageRGBProfile = prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile;
	CMSSettings.DefaultImageCMYKProfile = prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile;
	CMSSettings.DefaultSolidColorProfile = prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile;
	CMSSettings.DefaultMonitorProfile = prefsManager->appPrefs.DCMSset.DefaultMonitorProfile;
	CMSSettings.DefaultPrinterProfile = prefsManager->appPrefs.DCMSset.DefaultPrinterProfile;
	CMSSettings.DefaultIntentPrinter = prefsManager->appPrefs.DCMSset.DefaultIntentPrinter;
	CMSSettings.DefaultIntentMonitor = prefsManager->appPrefs.DCMSset.DefaultIntentMonitor;
	CMSSettings.DefaultIntentImages = prefsManager->appPrefs.DCMSset.DefaultIntentImages;
	CMSSettings.SoftProofOn = prefsManager->appPrefs.DCMSset.SoftProofOn;
	CMSSettings.GamutCheck = prefsManager->appPrefs.DCMSset.GamutCheck;
	CMSSettings.BlackPoint = prefsManager->appPrefs.DCMSset.BlackPoint;
	CMSSettings.CMSinUse = prefsManager->appPrefs.DCMSset.CMSinUse;
	PDF_Options.SolidProf = CMSSettings.DefaultSolidColorProfile;
	PDF_Options.ImageProf = CMSSettings.DefaultImageRGBProfile;
	PDF_Options.PrintProf = CMSSettings.DefaultPrinterProfile;
	PDF_Options.Intent = CMSSettings.DefaultIntentMonitor;
	PDF_Options.Intent2 = CMSSettings.DefaultIntentImages;
#ifdef HAVE_CMS	
	SoftProofing = CMSSettings.SoftProofOn;
	Gamut = CMSSettings.GamutCheck;
	IntentPrinter = CMSSettings.DefaultIntentPrinter;
	IntentMonitor = CMSSettings.DefaultIntentMonitor;
	BlackPoint = CMSSettings.BlackPoint;
#endif
	if ((CMSavail) && (CMSSettings.CMSinUse))
	{
#ifdef HAVE_CMS
		OpenCMSProfiles(ScApp->InputProfiles, ScApp->MonitorProfiles, ScApp->PrinterProfiles);
		stdProofG = stdProof;
		stdTransG = stdTrans;
		stdProofImgG = stdProofImg;
		stdTransImgG = stdTransImg;
		stdProofCMYKG = stdProofCMYK;
		stdTransCMYKG = stdTransCMYK;
		stdTransRGBG = stdTransRGB;
		stdProofGCG = stdProofGC;
		stdProofCMYKGCG = stdProofCMYKGC;
		CMSoutputProf = DocOutputProf;
		CMSprinterProf = DocPrinterProf;
		if (static_cast<int>(cmsGetColorSpace(DocInputProf)) == icSigRgbData)
			CMSSettings.ComponentsInput2 = 3;
		if (static_cast<int>(cmsGetColorSpace(DocInputProf)) == icSigCmykData)
			CMSSettings.ComponentsInput2 = 4;
		if (static_cast<int>(cmsGetColorSpace(DocInputProf)) == icSigCmyData)
			CMSSettings.ComponentsInput2 = 3;
		if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigRgbData)
			CMSSettings.ComponentsPrinter = 3;
		if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigCmykData)
			CMSSettings.ComponentsPrinter = 4;
		if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigCmyData)
			CMSSettings.ComponentsPrinter = 3;
		PDF_Options.SComp = CMSSettings.ComponentsInput2;
#endif
	}
}

void ScribusDoc::setLoading(const bool docLoading)
{
	loading = docLoading;
}

const bool ScribusDoc::isLoading()
{
	return loading;
}

void ScribusDoc::CloseCMSProfiles()
{
#ifdef HAVE_CMS
	if ((CMSavail) && (CMSSettings.CMSinUse))
	{
		cmsCloseProfile(DocInputProf);
		cmsCloseProfile(DocOutputProf);
		cmsCloseProfile(DocPrinterProf);
		cmsDeleteTransform(stdTrans);
		cmsDeleteTransform(stdProof);
		cmsDeleteTransform(stdTransImg);
		cmsDeleteTransform(stdProofImg);
		cmsDeleteTransform(stdTransCMYK);
		cmsDeleteTransform(stdProofCMYK);
		cmsDeleteTransform(stdTransRGB);
		cmsDeleteTransform(stdProofCMYKGC);
		cmsDeleteTransform(stdProofGC);
	}
#endif
}

void ScribusDoc::OpenCMSProfiles(ProfilesL InPo, ProfilesL MoPo, ProfilesL PrPo)
{
#ifdef HAVE_CMS
	DocInputProf = cmsOpenProfileFromFile(InPo[CMSSettings.DefaultSolidColorProfile], "r");
	DocOutputProf = cmsOpenProfileFromFile(MoPo[CMSSettings.DefaultMonitorProfile], "r");
	DocPrinterProf = cmsOpenProfileFromFile(PrPo[CMSSettings.DefaultPrinterProfile], "r");
	if ((DocInputProf == NULL) || (DocOutputProf == NULL) || (DocPrinterProf == NULL))
	{
		CMSSettings.CMSinUse = false;
		return;
	}
	int dcmsFlags = 0;
	int dcmsFlags2 = 0;
	dcmsFlags |= cmsFLAGS_LOWRESPRECALC;
	dcmsFlags2 |= cmsFLAGS_LOWRESPRECALC;
//	int dcmsFlags2 = cmsFLAGS_NOTPRECALC;
	if (CMSSettings.GamutCheck)
		dcmsFlags |= cmsFLAGS_GAMUTCHECK;
	if (CMSSettings.BlackPoint)
	{
		dcmsFlags2 |= cmsFLAGS_BLACKPOINTCOMPENSATION;
		dcmsFlags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
	}
	// set Gamut alarm color to #00ff00
	cmsSetAlarmCodes(0, 255, 0);
	cmsErrorAction(LCMS_ERROR_SHOW);
	stdProof = cmsCreateProofingTransform(DocInputProf, TYPE_RGB_16,
	                                      DocOutputProf, TYPE_RGB_16,
	                                      DocPrinterProf,
	                                      IntentPrinter,
	                                      IntentMonitor, dcmsFlags | cmsFLAGS_SOFTPROOFING);
	stdProofGC = cmsCreateProofingTransform(DocInputProf, TYPE_RGB_16,
	                                      DocOutputProf, TYPE_RGB_16,
	                                      DocPrinterProf,
	                                      IntentPrinter,
	                                      IntentMonitor, dcmsFlags | cmsFLAGS_SOFTPROOFING | cmsFLAGS_GAMUTCHECK);
	stdTrans = cmsCreateTransform(DocInputProf, TYPE_RGB_16,
	                              DocOutputProf, TYPE_RGB_16,
	                              IntentMonitor,
	                              dcmsFlags2);
	stdProofImg = cmsCreateProofingTransform(DocInputProf, TYPE_RGBA_8,
	              DocOutputProf, TYPE_RGBA_8,
	              DocPrinterProf,
	              IntentPrinter,
	              IntentMonitor, dcmsFlags | cmsFLAGS_SOFTPROOFING);
	stdTransImg = cmsCreateTransform(DocInputProf, TYPE_RGBA_8,
	                                DocOutputProf, TYPE_RGBA_8,
	                                 IntentMonitor,
	                                 dcmsFlags2);
	if (static_cast<int>(cmsGetColorSpace(DocPrinterProf)) == icSigCmykData)
	{
		stdProofCMYK = cmsCreateProofingTransform(DocPrinterProf, TYPE_CMYK_16,
							DocOutputProf, TYPE_RGB_16,
							DocPrinterProf,
							IntentPrinter,
							IntentMonitor, dcmsFlags | cmsFLAGS_SOFTPROOFING);
		stdProofCMYKGC = cmsCreateProofingTransform(DocPrinterProf, TYPE_CMYK_16,
							DocOutputProf, TYPE_RGB_16,
							DocPrinterProf,
							IntentPrinter,
							IntentMonitor, dcmsFlags | cmsFLAGS_SOFTPROOFING | cmsFLAGS_GAMUTCHECK);
		stdTransCMYK = cmsCreateTransform(DocInputProf, TYPE_RGB_16,
						DocPrinterProf, TYPE_CMYK_16,
						IntentPrinter,
						dcmsFlags2);
		stdTransRGB = cmsCreateTransform(DocPrinterProf, TYPE_CMYK_16,
						DocInputProf, TYPE_RGB_16,
						IntentMonitor,
						dcmsFlags2);
	}
	else
	{
		stdProofCMYK = cmsCreateProofingTransform(DocPrinterProf, TYPE_RGB_16,
							DocOutputProf, TYPE_RGB_16,
							DocPrinterProf,
							IntentPrinter,
							IntentMonitor, dcmsFlags | cmsFLAGS_SOFTPROOFING);
		stdProofCMYKGC = cmsCreateProofingTransform(DocPrinterProf, TYPE_RGB_16,
							DocOutputProf, TYPE_RGB_16,
							DocPrinterProf,
							IntentPrinter,
							IntentMonitor, dcmsFlags | cmsFLAGS_SOFTPROOFING | cmsFLAGS_GAMUTCHECK);
		stdTransCMYK = cmsCreateTransform(DocInputProf, TYPE_RGB_16,
						DocPrinterProf, TYPE_RGB_16,
						IntentPrinter,
						dcmsFlags2);
		stdTransRGB = cmsCreateTransform(DocPrinterProf, TYPE_RGB_16,
						DocInputProf, TYPE_RGB_16,
						IntentMonitor,
						dcmsFlags2);
	}
#endif
}

/*
 * Split out from loadStyles in editFormats.cpp so it's callable from anywhere,
 * including plugins.
 * - 2004-09-14 Craig Ringer
 */
void ScribusDoc::loadStylesFromFile(QString fileName, QValueList<ParagraphStyle> *tempStyles)
{
	// This won't create the QValueList<ParagraphStyle> will it?
	QValueList<ParagraphStyle> *wrkStyles = NULL;
	/*
	 * Use the working styles struct if passed, or work directly
	 * on the document styles otherwise. Note that tempStyles,
	 * if passed, MUST have the first five styles initialised already
	 * or this function will segfault.
	 */
	if (tempStyles != NULL)
		wrkStyles = tempStyles;
	 else
		wrkStyles = &docParagraphStyles;
	if (!fileName.isEmpty())
	{
		ScriXmlDoc *ss = new ScriXmlDoc();
		ss->docParagraphStyles.clear();
		for (uint x = 5; x < wrkStyles->count(); ++x)
			ss->docParagraphStyles.append((*wrkStyles)[x]);
		uint old = wrkStyles->count()-5;
		if (ss->ReadStyles(fileName, this))
		{
			if (ss->docParagraphStyles.count() > old)
			{
				for (uint xx=old; xx<ss->docParagraphStyles.count(); ++xx)
				{
					struct ParagraphStyle sty;
					sty.Vname = ss->docParagraphStyles[xx].Vname;
					sty.LineSpaMode = ss->docParagraphStyles[xx].LineSpaMode;
					sty.LineSpa = ss->docParagraphStyles[xx].LineSpa;
					sty.textAlignment = ss->docParagraphStyles[xx].textAlignment;
					sty.Indent = ss->docParagraphStyles[xx].Indent;
					sty.First = ss->docParagraphStyles[xx].First;
					sty.gapBefore = ss->docParagraphStyles[xx].gapBefore;
					sty.gapAfter = ss->docParagraphStyles[xx].gapAfter;
					sty.Font = ss->docParagraphStyles[xx].Font;
					sty.FontSize = ss->docParagraphStyles[xx].FontSize;
					sty.TabValues = ss->docParagraphStyles[xx].TabValues;
					sty.Drop = ss->docParagraphStyles[xx].Drop;
					sty.DropLin = ss->docParagraphStyles[xx].DropLin;
					sty.DropDist = ss->docParagraphStyles[xx].DropDist;
					sty.FontEffect = ss->docParagraphStyles[xx].FontEffect;
					sty.FColor = ss->docParagraphStyles[xx].FColor;
					sty.FShade = ss->docParagraphStyles[xx].FShade;
					sty.SColor = ss->docParagraphStyles[xx].SColor;
					sty.SShade = ss->docParagraphStyles[xx].SShade;
					sty.BaseAdj = ss->docParagraphStyles[xx].BaseAdj;
					sty.txtShadowX = ss->docParagraphStyles[xx].txtShadowX;
					sty.txtShadowY = ss->docParagraphStyles[xx].txtShadowY;
					sty.txtOutline = ss->docParagraphStyles[xx].txtOutline;
					sty.txtUnderPos = ss->docParagraphStyles[xx].txtUnderPos;
					sty.txtUnderWidth = ss->docParagraphStyles[xx].txtUnderWidth;
					sty.txtStrikePos = ss->docParagraphStyles[xx].txtStrikePos;
					sty.txtStrikeWidth = ss->docParagraphStyles[xx].txtStrikeWidth;
					sty.scaleH = ss->docParagraphStyles[xx].scaleH;
					sty.scaleV = ss->docParagraphStyles[xx].scaleV;
					sty.baseOff = ss->docParagraphStyles[xx].baseOff;
					sty.kernVal = ss->docParagraphStyles[xx].kernVal;
					wrkStyles->append(sty);
				}
			}
		}
		delete ss;
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
			if (ScQApp->usingGUI())
			{
				ScApp->changeLayer(ss->getInt("ACTIVE"));
				ScApp->layerPalette->rebuildList();
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
	PageAT = atf;
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
	bool error;
	FT_Face      face;

	if (UsedFonts.contains(name))
		return true;

	if (! ((*AllFonts)[name]) )
		return ret;

	error = FT_New_Face( library, (*AllFonts)[name]->fontFilePath(), (*AllFonts)[name]->faceIndex(), &face );
	if (error)
		return ret;

	if ((*AllFonts)[name]->ReadMetrics())
	{
		(*AllFonts)[name]->CharWidth[13] = 0;
		(*AllFonts)[name]->CharWidth[28] = 0;
		(*AllFonts)[name]->CharWidth[26] = 0;
		(*AllFonts)[name]->CharWidth[9] = 1;
		QString afnm = (*AllFonts)[name]->fontFilePath().left((*AllFonts)[name]->fontFilePath().length()-3);
		QFile afm(afnm+"afm");
		if(!(afm.exists()))
		{
			afm.setName(afnm+"Afm");
			if(!(afm.exists()))
				afm.setName(afnm+"AFM");
		}
		if (afm.exists())
			FT_Attach_File(face, afm.name());
		FFonts[name] = face;
		UsedFonts[name] = fsize;
		ret = true;
	}
	else
		FT_Done_Face( face );
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

Page* ScribusDoc::addPage(const int pageNumber)
{
	Page* addedPage = new Page(ScratchLeft, pageCount*(pageHeight+ScratchBottom+ScratchTop)+ScratchTop, pageWidth, pageHeight);
	Q_ASSERT(addedPage!=NULL);
	addedPage->Margins.Top = pageMargins.Top;
	addedPage->Margins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Top = pageMargins.Top;
	addedPage->initialMargins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Left = pageMargins.Left;
	addedPage->initialMargins.Right = pageMargins.Right;
	addedPage->setPageNr(pageNumber);
	addedPage->PageSize = PageSize;
	addedPage->PageOri = PageOri;
	Pages.insert(pageNumber, addedPage);
	if (masterPageMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
	currentPage = addedPage;
	pageCount++;
	return addedPage;
}

bool ScribusDoc::deletePage(const int pageNumber)
{
	if (pageCount == 1)
		return false;
	if (pageCount < pageNumber-1)
		return false;
	Page* Seite = Pages.at(pageNumber);
	PageItem *currItem;
	for (currItem = Seite->FromMaster.first(); currItem; currItem = Seite->FromMaster.next())
	{
		if (currItem->ChangedMasterItem)
		{
			Seite->FromMaster.remove(currItem);
			delete currItem;
		}
	}
	Seite->FromMaster.clear();
	Pages.remove(pageNumber);
	delete Seite;
	pageCount -= 1;
	currentPage = Pages.at(0);
	if (masterPageMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
	return true;
}

void ScribusDoc::movePage(const int from, const int to, const int ziel, const int art)
{
	QPtrList<Page> Buf;
	int zz = ziel;
	Buf.clear();
	for (int a = from; a < to; a++)
	{
		Buf.append(Pages.at(from));
		Pages.remove(from);
		if (a <= zz)
			zz--;
	}
	switch (art)
	{
		case 0:
			for (uint b = 0; b < Buf.count(); b++)
				Pages.insert(zz++, Buf.at(b));
			break;
		case 1:
			for (uint b = 0; b < Buf.count(); b++)
				Pages.insert(++zz, Buf.at(b));
			break;
		case 2:
			for (uint b = 0; b < Buf.count(); b++)
				Pages.append(Buf.at(b));
			break;
	}
	if (masterPageMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
}

const int ScribusDoc::addLayer(const QString& layerName=QString::null, const bool activate=false)
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

const bool ScribusDoc::deleteLayer(const int layerNumber, const bool deleteItems)
{
	if (Layers.count() < 2)
		return false;
	QValueList<Layer>::iterator it2;
	QValueList<Layer>::iterator it2end=Layers.end();
	bool found=false;
	int layerLevel;
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

	if (ScQApp->usingGUI())
		ScApp->LayerRemove(layerNumber, deleteItems);
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

const int ScribusDoc::activeLayer()
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

const bool ScribusDoc::setActiveLayer(const int layerToActivate)
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
	if (found)
		ActiveLayer=layerToActivate;
	return found;
}

const bool ScribusDoc::setActiveLayer(const QString& layerNameToActivate)
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

const bool ScribusDoc::setLayerPrintable(const int layerNumber, const bool isPrintable)
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
	return found;
}

const bool ScribusDoc::layerPrintable(const int layerNumber)
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

const bool ScribusDoc::setLayerVisible(const int layerNumber, const bool isViewable)
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
	return found;
}

const bool ScribusDoc::layerVisible(const int layerNumber)
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

const int ScribusDoc::layerLevelFromNumber(const int layerNumber)
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerNumber)
			return Layers[i].Level;
	}
	return -1;
}

const int ScribusDoc::layerCount()
{
	return Layers.count();
}

const int ScribusDoc::layerNumberFromLevel(const int layerLevel)
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].Level == layerLevel)
			return Layers[i].LNr;
	}
	return -1;
}

const bool ScribusDoc::lowerLayer(const int layerNumber)
{
	return lowerLayerByLevel(layerLevelFromNumber(layerNumber));
}

const bool ScribusDoc::lowerLayerByLevel(const int layerLevel)
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

const bool ScribusDoc::raiseLayer(const int layerNumber)
{
	return raiseLayerByLevel(layerLevelFromNumber(layerNumber));
}

const bool ScribusDoc::raiseLayerByLevel(const int layerLevel)
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

const QString& ScribusDoc::layerName(const int layerNumber)
{
	uint layerCount=Layers.count();
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerNumber)
			return Layers[i].Name;
	}
	return QString::null;
}

const bool ScribusDoc::changeLayerName(const int layerNumber, const QString& newName)
{
	uint layerCount=Layers.count();
	bool found=false;
	for (uint i=0; i < layerCount; ++i)
	{
		if (Layers[i].LNr == layerNumber)
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
			break;
		}
	}
	return found;
}

const bool ScribusDoc::layerContainsItems(const int layerNumber)
{
	if (masterPageMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
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

const bool ScribusDoc::renumberLayer(const int layerNumber, const int newLayerNumber)
{
	uint layerCount=Layers.count();
	uint foundIndex;
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
const bool ScribusDoc::deleteTaggedItems()
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
				for (uint d=0; d<ite->itemText.count(); ++d)
				{
					if (it.key() == ite->itemText.at(d)->ccolor)
						found = true;
					if (it.key() == ite->itemText.at(d)->cstroke)
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
				for (uint d=0; d<ite->itemText.count(); ++d)
				{
					/* PFJ - 29.02.04 - Merged if's */
					if ((it.key() == ite->itemText.at(d)->ccolor) || (it.key() == ite->itemText.at(d)->cstroke))
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
				for (uint d=0; d<ite->itemText.count(); ++d)
				{
					/* PFJ - 29.02.04 - Merged if's */
					if ((it.key() == ite->itemText.at(d)->ccolor) || (it.key() == ite->itemText.at(d)->cstroke))
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

void ScribusDoc::getUsedFonts(QMap<QString,int> *Really)
{
	PageItem* it;
	FPointArray gly;
	QString chx;
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
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				for (uint e = 0; e < it->itemText.count(); ++e)
				{
					Really->insert(it->itemText.at(e)->cfont->scName(), UsedFonts[it->itemText.at(e)->cfont->scName()]);
					uint chr = it->itemText.at(e)->ch[0].unicode();
					if ((chr == 13) || (chr == 32) || (chr == 29))
						continue;
					if (chr == 9)
					{
						for (uint t1 = 0; t1 < docParagraphStyles[it->itemText.at(e)->cab].TabValues.count(); t1++)
						{
							if (docParagraphStyles[it->itemText.at(e)->cab].TabValues[t1].tabFillChar.isNull())
								continue;
							chx = QString(docParagraphStyles[it->itemText.at(e)->cab].TabValues[t1].tabFillChar);
							if ((it->itemText.at(e)->cstyle & 64) || (it->itemText.at(e)->cstyle & 32))
							{
								if (chx.upper() != QString(docParagraphStyles[it->itemText.at(e)->cab].TabValues[t1].tabFillChar))
									chx = chx.upper();
							}
							chr = chx[0].unicode();
							gly = it->itemText.at(e)->cfont->GlyphArray[chr].Outlines.copy();
							it->itemText.at(e)->cfont->RealGlyphs.insert(chr, gly);
						}
						for (uint t1 = 0; t1 < it->TabValues.count(); t1++)
						{
							if (it->TabValues[t1].tabFillChar.isNull())
								continue;
							chx = QString(it->TabValues[t1].tabFillChar);
							if ((it->itemText.at(e)->cstyle & 64) || (it->itemText.at(e)->cstyle & 32))
							{
								if (chx.upper() != QString(it->TabValues[t1].tabFillChar))
									chx = chx.upper();
							}
							chr = chx[0].unicode();
							gly = it->itemText.at(e)->cfont->GlyphArray[chr].Outlines.copy();
							it->itemText.at(e)->cfont->RealGlyphs.insert(chr, gly);
						}
						continue;
					}
					if (chr == 30)
					{
						for (uint numco = 0x30; numco < 0x3A; ++numco)
						{
							if (it->itemText.at(e)->cfont->CharWidth.contains(numco))
							{
								gly = it->itemText.at(e)->cfont->GlyphArray[numco].Outlines.copy();
								it->itemText.at(e)->cfont->RealGlyphs.insert(numco, gly);
							}
						}
						continue;
					}
					if ((it->itemText.at(e)->cstyle & 64) || (it->itemText.at(e)->cstyle & 32))
					{
						chx = it->itemText.at(e)->ch;
						if (chx.upper() != it->itemText.at(e)->ch)
							chx = chx.upper();
						chr = chx[0].unicode();
					}
					if (it->itemText.at(e)->cfont->CharWidth.contains(chr))
					{
						gly = it->itemText.at(e)->cfont->GlyphArray[chr].Outlines.copy();
						it->itemText.at(e)->cfont->RealGlyphs.insert(chr, gly);
					}
				}
			}
		}
	}
}

void ScribusDoc::reorganiseFonts()
{
	QMap<QString,int> Really;
	//QMap<QString,QFont> DocF;
	//DocF = UsedFonts;
	if (!masterPageMode)
		DocPages = Pages;
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
		PageItem* it;
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
			Really.insert(it->IFont, UsedFonts[it->IFont]);
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				uint itemTextCount=it->itemText.count();
				for (uint e = 0; e < itemTextCount; ++e)
				{
					Really.insert(it->itemText.at(e)->cfont->scName(), UsedFonts[it->itemText.at(e)->cfont->scName()]);
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
			FT_Done_Face(FFonts[itfo.key()]);
			FFonts.remove(itfo.key());
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

const int ScribusDoc::unitIndex()
{
	return docUnitIndex;
}

const double ScribusDoc::unitRatio()
{
	return docUnitRatio;
}

const bool ScribusDoc::applyMasterPage(const QString& in, const int pageNumber)
{
	if (UndoManager::undoEnabled())
	{
		if (Pages.at(pageNumber)->MPageNam != in)
		{
			SimpleState *ss = new SimpleState(Um::ApplyMasterPage, QString(Um::FromTo).arg(Pages.at(pageNumber)->MPageNam).arg(in));
			ss->set("PAGE_NUMBER", pageNumber);
			ss->set("OLD_MASTERPAGE", Pages.at(pageNumber)->MPageNam);
			ss->set("NEW_MASTERPAGE", in);
			undoManager->action(this, ss);
		}
	}

	QString mna = in;
	if (mna == tr("Normal"))
		mna = "Normal";
	if (!MasterNames.contains(mna))
		mna = "Normal";
	Page* Ap = Pages.at(pageNumber);
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
	if (Mp->YGuides.count() != 0)
	{
		for (uint y = 0; y < Mp->YGuides.count(); ++y)
		{
			if (Ap->YGuides.contains(Mp->YGuides[y]) == 0)
				Ap->YGuides.append(Mp->YGuides[y]);
		}
		qHeapSort(Ap->YGuides);
	}
	if (Mp->XGuides.count() != 0)
	{
		for (uint x = 0; x < Mp->XGuides.count(); ++x)
		{
			if (Ap->XGuides.contains(Mp->XGuides[x]) == 0)
				Ap->XGuides.append(Mp->XGuides[x]);
		}
		qHeapSort(Ap->XGuides);
	}
	//TODO make a return false if not possible to apply the master page
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
const bool ScribusDoc::save(const QString& fileName)
{
	QFileInfo fi(fileName);
	QDir::setCurrent(fi.dirPath(true));
	QProgressBar* mainWindowProgressBar=NULL;
	if (ScQApp->usingGUI())
	{
		mainWindowProgressBar=ScApp->mainWindowProgressBar;
		mainWindowProgressBar->reset();
	}
	ScriXmlDoc *ss = new ScriXmlDoc();
	bool ret = ss->WriteDoc(fileName, this, mainWindowProgressBar);
	delete ss;
	if (ret)
	{
		setModified(false);
		setName(fileName);
		hasName = true;
	}
	return ret;
}

const bool ScribusDoc::changePageMargins(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double height, const double width, const int orientation, const QString& pageSize, const int pageNumber, const int pageType)
{
	bool retVal=true;
	if (pageNumber==-1)
	{
		//find page and set values
		return false;
	}
	else
	{
		if (currentPage==NULL)
			retVal=false;
		else
		{
			//set the current page's values
			currentPage->initialMargins.Top = initialTop;
			currentPage->initialMargins.Bottom = initialBottom;
			currentPage->initialMargins.Left = initialLeft;
			currentPage->initialMargins.Right = initialRight;
			currentPage->setInitialHeight(initialHeight);
			currentPage->setInitialWidth(initialWidth);
			currentPage->setHeight(height);
			currentPage->setWidth(width);
			currentPage->PageOri = orientation;
			currentPage->PageSize = pageSize;
			currentPage->LeftPg = pageType;
		}
	}
	return retVal;
}

void ScribusDoc::recalculateColors()
{
	if (masterPageMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
	//Recalculate the RGB or CMYK calues to new CMS settings
	ColorList::Iterator it;
	ColorList::Iterator itend=PageColors.end();
	for (it = PageColors.begin(); it != itend; ++it)
		PageColors[it.key()].RecalcRGB();
	//Adjust Items of the 3 types to the colors
	uint itemsCount=Items.count();
	for (uint c=0; c<itemsCount; ++c)
	{
		PageItem *ite = Items.at(c);
		if (ite->fillColor() != "None")
			ite->fillQColor = PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
		if (ite->lineColor() != "None")
			ite->strokeQColor = PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			QColor tmpc = PageColors[cstops.at(cst)->name].getRGBColor();
			ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops.at(cst)->color = tmpc;
		}
	}
	uint masterItemsCount=MasterItems.count();
	for (uint c=0; c<masterItemsCount; ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if (ite->fillColor() != "None")
			ite->fillQColor = PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
		if (ite->lineColor() != "None")
			ite->strokeQColor = PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			QColor tmpc = PageColors[cstops.at(cst)->name].getRGBColor();
			ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops.at(cst)->color = tmpc;
		}
	}
	uint frameItemsCount=FrameItems.count();
	for (uint c=0; c<frameItemsCount; ++c)
	{
		PageItem *ite = FrameItems.at(c);
		if (ite->fillColor() != "None")
			ite->fillQColor = PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
		if (ite->lineColor() != "None")
			ite->strokeQColor = PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			QColor tmpc = PageColors[cstops.at(cst)->name].getRGBColor();
			ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops.at(cst)->color = tmpc;
		}
	}
}

void ScribusDoc::setScTextDefaultsFromDoc(ScText *sctextdata)
{
	if (sctextdata==NULL)
		return;
	sctextdata->cfont = (*AllFonts)[CurrFont];
	sctextdata->csize = CurrFontSize;
	sctextdata->ccolor = CurrTextFill;
	sctextdata->cshade = CurrTextFillSh;
	sctextdata->cstroke = CurrTextStroke;
	sctextdata->cshade2 = CurrTextStrokeSh;
	sctextdata->cscale = CurrTextScale;
	sctextdata->cscalev = CurrTextScaleV;
	sctextdata->cbase = CurrTextBase;
	sctextdata->cshadowx = CurrTextShadowX;
	sctextdata->cshadowy = CurrTextShadowY;
	sctextdata->coutline = CurrTextOutline;
	sctextdata->cunderpos = CurrTextUnderPos;
	sctextdata->cunderwidth = CurrTextUnderWidth;
	sctextdata->cstrikepos = CurrTextStrikePos;
	sctextdata->cstrikewidth = CurrTextStrikeWidth;
	sctextdata->cstyle = CurrentStyle;
	sctextdata->cab = currentParaStyle;
	if (!docParagraphStyles[currentParaStyle].Font.isEmpty())
	{
		sctextdata->cfont = (*AllFonts)[docParagraphStyles[currentParaStyle].Font];
		sctextdata->csize = docParagraphStyles[currentParaStyle].FontSize;
	}
}

const bool ScribusDoc::copyPageToMasterPage(const int pageNumber, const int leftPage, const QString& masterPageName)
{
	//TODO Add Undo here
	int GrMax = GroupCounter;
	Page* sourcePage = Pages.at(pageNumber);
	int nr = Pages.count();
	DocPages = Pages;
	Pages = MasterPages;
	DocItems = Items;
	Items = MasterItems;
	masterPageMode = true;
	MasterNames.insert(masterPageName, nr);
	pageCount = 0;
	bool atf = PageAT;
	PageAT = false;
	//Note we are bypassing the view here, but things seem fine. The master page is offscreen anyway.
	//ScApp->slotNewPage(nr);
	//Page* targetPage = Pages.at(nr);	
	Page* targetPage=addPage(nr);
	setLoading(true);
	if (currentPageLayout != singlePage)
		Pages.at(nr)->LeftPg = leftPage;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	if (sourcePage->YGuides.count() != 0)
	{
		targetPage->YGuides.clear();
		for (uint y = 0; y < sourcePage->YGuides.count(); ++y)
			targetPage->YGuides.append(sourcePage->YGuides[y]);
		qHeapSort(targetPage->YGuides);
	}
	if (sourcePage->XGuides.count() != 0)
	{
		targetPage->XGuides.clear();
		for (uint x = 0; x < sourcePage->XGuides.count(); ++x)
			targetPage->XGuides.append(sourcePage->XGuides[x]);
		qHeapSort(targetPage->XGuides);
	}
	struct CopyPasteBuffer BufferT;
	uint end = DocItems.count();
	for (uint a = 0; a < end; ++a)
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
			ScApp->view->PasteItem(&BufferT, true, true);
			PageItem* Neu = Items.at(Items.count()-1);
			Neu->OnMasterPage = masterPageName;
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(a, Neu->ItemNr);
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
				ta->TopLink = Items.at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = Items.at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = Items.at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = Items.at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	MasterPages = Pages;
	pageCount = DocPages.count();
	Pages = DocPages;
	MasterItems = Items;
	Items = DocItems;
	masterPageMode = false;
	targetPage->setPageName(masterPageName);
	targetPage->MPageNam = "";
	PageAT = atf;
	setLoading(false);
	GroupCounter = GrMax + 1;
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
			//newItem = new PageItem(this, PageItem::ImageFrame, x, y, b, h, 1, toolSettings.dBrushPict, "None");
			newItem = new PageItem_ImageFrame(this, x, y, b, h, 1, toolSettings.dBrushPict, "None");
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::TextFrame:
			//newItem = new PageItem(this, PageItem::TextFrame, x, y, b, h, w, "None", outline);
			newItem = new PageItem_TextFrame(this, x, y, b, h, w, "None", outline);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::Line:
			{
				double lineWidth = w == 0.0 ? 1.0 : w;
				//newItem = new PageItem(this, PageItem::Line, x, y, b, h, lineWidth, "None", outline);
				newItem = new PageItem_Line(this, x, y, b, h, lineWidth, "None", outline);
				Q_ASSERT(frameType==PageItem::Unspecified);
			}
			break;
		case PageItem::Polygon:
			//newItem = new PageItem(this, PageItem::Polygon, x, y, b, h, w, fill, outline);
			newItem = new PageItem_Polygon(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Ellipse || frameType==PageItem::Unspecified);
			break;
		case PageItem::PolyLine:
			//newItem = new PageItem(this, PageItem::PolyLine, x, y, b, h, w, fill, outline);
			newItem = new PageItem_PolyLine(this, x, y, b, h, w, fill, outline);
			Q_ASSERT(frameType==PageItem::Unspecified);
			break;
		case PageItem::PathText:
		//At this point, we cannot create a PathText item like this, only by conversion
			break;
		default:
			break;
	}
	Q_CHECK_PTR(newItem);	
	if (newItem==NULL)
		return -1;
	Items.append(newItem);
	newItem->ItemNr = Items.count()-1;	
	if (masterPageMode)
		MasterItems = Items;
	else
		DocItems = Items;
	//Add in item default values based on itemType and frameType
	itemAddDetails(itemType, frameType, newItem->ItemNr);

	if (UndoManager::undoEnabled())
	{
		ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM", "create_item");
		is->setItem(newItem);
		//Undo target rests with the Page for object specific undo
		UndoObject *target = Pages.at(0);
		if (newItem->OwnPage > -1)
			target = Pages.at(newItem->OwnPage);
		undoManager->action(target, is);
		//If the item is created "complete" (ie, not being created by drag/resize, commit to undomanager)
		if (itemFinalised)
		{
			//dont think we need this now ... newItem->checkChanges(true);
			undoManager->commit(Pages.at(newItem->OwnPage)->getUName(), newItem->getUPixmap(),
								Um::Create + " " + newItem->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
		}
	}
	return newItem->ItemNr;
}

void ScribusDoc::itemAddDetails(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const int itemNumber)
{
	PageItem* newItem=Items.at(itemNumber);
	Q_ASSERT(newItem->itemType()==itemType);
	switch( itemType )
	{
		case PageItem::ImageFrame:
			newItem->setFillShade(toolSettings.shadePict);
			newItem->LocalScX = toolSettings.scaleX;
			newItem->LocalScY = toolSettings.scaleY;
			newItem->ScaleType = toolSettings.scaleType;
			newItem->AspectRatio = toolSettings.aspectRatio;
			newItem->IProfile = CMSSettings.DefaultImageRGBProfile;
			newItem->IRender = CMSSettings.DefaultIntentImages;
			break;
		case PageItem::TextFrame:
			newItem->setFontFillShade(toolSettings.dTextPenShade);
			newItem->setFontStrokeShade(toolSettings.dTextStrokeShade);
			newItem->setFillColor(toolSettings.dTextBackGround);
			newItem->setFillShade(toolSettings.dTextBackGroundShade);
			newItem->setLineColor(toolSettings.dTextLineColor);
			newItem->setLineShade(toolSettings.dTextLineShade);
			break;
		case PageItem::Line:
			newItem->PLineArt = PenStyle(toolSettings.dLstyleLine);
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
		//TODO one day hopefully, if(ScQApp->usingGUI())
		ScApp->view->setRedrawBounding(newItem);
		newItem->ContourLine = newItem->PoLine.copy();
	}
	
	if (frameType==PageItem::Ellipse)
	{
		newItem->SetOvalFrame();
		//TODO one day hopefully, if(ScQApp->usingGUI())
		ScApp->view->setRedrawBounding(newItem);
		newItem->ContourLine = newItem->PoLine.copy();
	}
	
	//ItemType Polygon
	if (itemType==PageItem::Polygon || itemType==PageItem::PolyLine)
	{
		newItem->PLineArt = PenStyle(toolSettings.dLineArt);
		newItem->setFillShade(toolSettings.dShade);
		newItem->setLineShade(toolSettings.dShade2);
	}
	//ItemType::image,text,polygon,polyline
	if (newItem->fillColor() != "None")
		newItem->fillQColor = PageColors[newItem->fillColor()].getShadeColorProof(newItem->fillShade());
	if (newItem->lineColor() != "None")
		newItem->strokeQColor = PageColors[newItem->lineColor()].getShadeColorProof(newItem->lineShade());
}


bool ScribusDoc::itemAddCommit(const int /*itemNumber*/)
{
	//TODO use the parameter
	if (_itemCreationTransactionStarted && appMode !=  modeDrawBezierLine)
	{
		PageItem *createdItem=ScApp->view->SelItem.at(0);
		if (createdItem!=NULL)
		{
			createdItem->checkChanges(true);
			QString targetName = Um::ScratchSpace;
			if (createdItem->OwnPage > -1)
				targetName = Pages.at(createdItem->OwnPage)->getUName();
			undoManager->commit(targetName, createdItem->getUPixmap(),
								Um::Create + " " + createdItem->getUName(),  "", Um::ICreate);
			_itemCreationTransactionStarted = false;
			if (!isLoading())
				return true;
		}
	}
	return false;
}

void ScribusDoc::updateFrameItems()
{
	uint frameItemsCount=FrameItems.count();
	for (uint a = 0; a < frameItemsCount; ++a)
		FrameItems.at(a)->ItemNr = a;
}
