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

#include "scribusdoc.h"
#include "scribusXml.h"
#include "scribus.h"

#include <qfile.h>

#include "page.h"
#include "pageitem.h"
#include "undomanager.h"
#include "undostate.h"
#include "prefsmanager.h"

extern ScribusApp* ScApp;

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
extern bool BlackPoint;
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
extern int IntentMonitor;
extern int IntentPrinter;
#endif

extern bool CMSavail;
extern QPixmap loadIcon(QString nam);

ScribusDoc::ScribusDoc() : UndoObject(QObject::tr("Document"))
{
	ApplicationPrefs* prefsData=&(PrefsManager::instance()->appPrefs);
	modified = false;
	MasterP = false;
	NrItems = 0;
	First = 1;
	Last = 0;
	viewCount = 0;
	viewID = 0;
	is12doc=false;
	UsedFonts.clear();
	FT_Init_FreeType( &library );
	AllFonts = &prefsData->AvailFonts;
	AddFont(prefsData->toolSettings.defFont, prefsData->AvailFonts[prefsData->toolSettings.defFont]->Font);
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
	DocName = QObject::tr("Document")+"-";
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
	ll.Name = QObject::tr("Background");
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
	PageGapVertical = prefsData->PageGapVertical;
	PageGapHorizontal = prefsData->PageGapHorizontal;
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
	QMap<QString,QFont>::Iterator it3;
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
	FirstPageLeft = firstLeft;
	PageOri = orientation;
	PageSize = defaultPageSize;
	FirstPnum = firstPageNumber;
	PageFP = fp;
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
	
	CMSSettings.DefaultInputProfile = prefsManager->appPrefs.DCMSset.DefaultInputProfile;
	CMSSettings.DefaultInputProfile2 = prefsManager->appPrefs.DCMSset.DefaultInputProfile2;
	CMSSettings.DefaultMonitorProfile = prefsManager->appPrefs.DCMSset.DefaultMonitorProfile;
	CMSSettings.DefaultPrinterProfile = prefsManager->appPrefs.DCMSset.DefaultPrinterProfile;
	CMSSettings.DefaultIntentPrinter = prefsManager->appPrefs.DCMSset.DefaultIntentPrinter;
	CMSSettings.DefaultIntentMonitor = prefsManager->appPrefs.DCMSset.DefaultIntentMonitor;
	CMSSettings.DefaultIntentMonitor2 = prefsManager->appPrefs.DCMSset.DefaultIntentMonitor2;
	CMSSettings.SoftProofOn = prefsManager->appPrefs.DCMSset.SoftProofOn;
	CMSSettings.GamutCheck = prefsManager->appPrefs.DCMSset.GamutCheck;
	CMSSettings.BlackPoint = prefsManager->appPrefs.DCMSset.BlackPoint;
	CMSSettings.CMSinUse = prefsManager->appPrefs.DCMSset.CMSinUse;
	PDF_Options.SolidProf = CMSSettings.DefaultInputProfile2;
	PDF_Options.ImageProf = CMSSettings.DefaultInputProfile;
	PDF_Options.PrintProf = CMSSettings.DefaultPrinterProfile;
	PDF_Options.Intent = CMSSettings.DefaultIntentMonitor;
	PDF_Options.Intent2 = CMSSettings.DefaultIntentMonitor2;
	SoftProofing = CMSSettings.SoftProofOn;
	Gamut = CMSSettings.GamutCheck;
	IntentPrinter = CMSSettings.DefaultIntentPrinter;
	IntentMonitor = CMSSettings.DefaultIntentMonitor;
	BlackPoint = CMSSettings.BlackPoint;
	
	if (CMSavail)
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
#endif
}

void ScribusDoc::OpenCMSProfiles(ProfilesL InPo, ProfilesL MoPo, ProfilesL PrPo)
{
#ifdef HAVE_CMS
	DocInputProf = cmsOpenProfileFromFile(InPo[CMSSettings.DefaultInputProfile2], "r");
	DocOutputProf = cmsOpenProfileFromFile(MoPo[CMSSettings.DefaultMonitorProfile], "r");
	DocPrinterProf = cmsOpenProfileFromFile(PrPo[CMSSettings.DefaultPrinterProfile], "r");
	if ((DocInputProf == NULL) || (DocOutputProf == NULL) || (DocPrinterProf == NULL))
	{
		CMSSettings.CMSinUse = false;
		return;
	}
	int dcmsFlags = 0;
	int dcmsFlags2 = cmsFLAGS_NOTPRECALC;
	if (CMSSettings.GamutCheck)
		dcmsFlags |= cmsFLAGS_GAMUTCHECK;
	if (CMSSettings.SoftProofOn)
		dcmsFlags |= cmsFLAGS_SOFTPROOFING;
#ifdef cmsFLAGS_BLACKPOINTCOMPENSATION
	if (CMSSettings.BlackPoint)
	{
		dcmsFlags2 |= cmsFLAGS_BLACKPOINTCOMPENSATION;
		dcmsFlags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
	}
#endif
	// set Gamut alarm color to #00ff00
	cmsSetAlarmCodes(0, 255, 0);
	stdProof = cmsCreateProofingTransform(DocInputProf, TYPE_RGB_16,
	                                      DocOutputProf, TYPE_RGB_16,
	                                      DocPrinterProf,
	                                      IntentPrinter,
	                                      IntentMonitor, dcmsFlags);
	stdTrans = cmsCreateTransform(DocInputProf, TYPE_RGB_16,
	                              DocOutputProf, TYPE_RGB_16,
	                              IntentMonitor,
	                              dcmsFlags2);
	stdProofImg = cmsCreateProofingTransform(DocInputProf, TYPE_RGBA_8,
	              DocOutputProf, TYPE_RGBA_8,
	              DocPrinterProf,
	              IntentPrinter,
	              IntentMonitor, dcmsFlags);
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
							IntentMonitor, dcmsFlags);
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
							IntentMonitor, dcmsFlags);
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
		if (ss->contains("GUIDE_LOCK"))
		{
			if (isUndo)
				GuideLock = !ss->getBool("GUIDE_LOCK");
			else
				GuideLock = ss->getBool("GUIDE_LOCK");	
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
	PageFP = fp;
	PageAT = atf;
}

void ScribusDoc::resetPage(double t, double l, double r, double bo, int fp)
{
	pageMargins.Top = t;
	pageMargins.Left = l;
	pageMargins.Right = r;
	pageMargins.Bottom = bo;
	PageFP = fp;
}

bool ScribusDoc::AddFont(QString name, QFont fo)
{
	bool ret = false;
	bool error;
	FT_Face      face;

	if (UsedFonts.contains(name)) 
		return true; 

	error = FT_New_Face( library, (*AllFonts)[name]->Datei, (*AllFonts)[name]->faceIndex, &face );
	if (error)
		return ret;

	if ((*AllFonts)[name]->ReadMetrics())
	{
		(*AllFonts)[name]->CharWidth[13] = 0;
		(*AllFonts)[name]->CharWidth[28] = 0;
		(*AllFonts)[name]->CharWidth[26] = 0;
		(*AllFonts)[name]->CharWidth[9] = 1;
		QString afnm = (*AllFonts)[name]->Datei.left((*AllFonts)[name]->Datei.length()-3);
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
		UsedFonts[name] = fo;
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
