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
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribuswin.h"
#include "scribusXml.h"

#include <utility>
#include <qfile.h>
#include <qprogressbar.h>

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
#include "prefsmanager.h"
#include "scfontmetrics.h"
#include "selection.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "util.h"
#include "commonstrings.h"


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

ScribusDoc::ScribusDoc() : UndoObject( tr("Document")),
	prefsData(PrefsManager::instance()->appPrefs),
	undoManager(UndoManager::instance()),
	loading(false),
	modified(false),
	ActiveLayer(0),
	docUnitIndex(prefsData.docUnitIndex),
	docUnitRatio(unitGetRatioFromIndex(docUnitIndex)),
	automaticTextFrames(0),
	m_masterPageMode(false),
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
	selection(new Selection(this, true)),
	pageWidth(0), pageHeight(0),
	// pageCount(0) CR
	// pageMargins
	pageSets(prefsData.pageSets),
	PageSp(1), PageSpa(0),
	currentPageLayout(0),
	PageOri(0), PageSize(0),
	FirstPnum(1),
	useRaster(false),
	currentPage(0),
	// documentInfo
	appMode(modeNormal),
	SubMode(-1),
	ShapeValues(0),
	ValCount(0),
	DocName(tr("Document")+"-"),
	UsedFonts(),
	AllFonts(&prefsData.AvailFonts),
	AObjects(),
	papColor(prefsData.DpapColor),
	CurrentSel(-1),
	CurrentStyle(0), currentParaStyle(0),
	// CurrFont, blah blah to CurrTextStrikeWidth are poked externally
	EditClip(false),
	EditClipMode(0),
	typographicSettings(prefsData.typographicSettings),
	guidesSettings(prefsData.guidesSettings),
	toolSettings(prefsData.toolSettings),
	checkerProfiles(prefsData.checkerProfiles),
	curCheckProfile(prefsData.curCheckProfile),
	LastAuto(0), FirstAuto(0),
	// DragP, leaveDrag
	DraggedElem(0),
	ElemToLink(0),
	DragElements(),
	docParagraphStyles(),
	Layers(),
	marginColored(prefsData.marginColored),
	GroupCounter(1),
	// CMSSettings, HasCMS, etc done by setup()
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
	// library
	FFonts(),
	MLineStyles(),
	arrowStyles(prefsData.arrowStyles),
	WinHan(0),
	DoDrawing(true),
	OpenNodes(),
	CurTimer(0),
	docItemErrors(),
	masterItemErrors(),
	docItemAttributes(prefsData.defaultItemAttributes),
	docToCSetups(prefsData.defaultToCSetups),
	// sections
	symReturn(), symNewLine(), symTab(), symNonBreak(), symNewCol(), symNewFrame(),
	docHyphenator(new Hyphenator(ScMW, this)), // MUST be constructed late
	_itemCreationTransactionStarted(false)
{
	Q_CHECK_PTR(selection);
	Q_CHECK_PTR(docHyphenator);
	Q_CHECK_PTR(autoSaveTimer);

	FT_Init_FreeType( &library );
	AddFont(prefsData.toolSettings.defFont);//, prefsData.AvailFonts[prefsData.toolSettings.defFont]->Font);
	toolSettings.defFont = prefsData.toolSettings.defFont;
	toolSettings.defSize = prefsData.toolSettings.defSize;
	toolSettings.tabFillChar = prefsData.toolSettings.tabFillChar;
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
	struct Layer ll;
	ll.LNr = 0;
	ll.Level = 0;
	ll.Name = tr("Background");
	ll.isViewable = true;
	ll.isPrintable = true;
	Layers.append(ll);
	// Fixme: Check PDF version input
	PDF_Options.Version = (PDFOptions::PDFVersion)prefsData.PDF_Options.Version;

	if (AutoSave && ScQApp->usingGUI())
		autoSaveTimer->start(AutoSaveTime);
	//Do this after all the collections have been created and cleared!
	m_masterPageMode=true; // quick hack to force the change of pointers in setMasterPageMode();
	setMasterPageMode(false);
	addSymbols();
	
	connect(this, SIGNAL(changed()), ScMW, SLOT(slotDocCh()));
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

	CMSSettings = prefsManager->appPrefs.DCMSset;
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
		OpenCMSProfiles(ScMW->InputProfiles, ScMW->MonitorProfiles, ScMW->PrinterProfiles);
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

bool ScribusDoc::isLoading() const
{
	return loading;
}

ScribusView* ScribusDoc::view() const
{
	return (WinHan ? (((ScribusWin*) WinHan)->view()) : 0);
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
	const QCString inputProfilePath(InPo[CMSSettings.DefaultSolidColorProfile].local8Bit());
	DocInputProf = cmsOpenProfileFromFile(inputProfilePath.data(), "r");
	const QCString monitorProfilePath(MoPo[CMSSettings.DefaultMonitorProfile].local8Bit());
	DocOutputProf = cmsOpenProfileFromFile(monitorProfilePath.data(), "r");
	const QCString printerProfilePath(PrPo[CMSSettings.DefaultPrinterProfile].local8Bit());
	DocPrinterProf = cmsOpenProfileFromFile(printerProfilePath, "r");
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
				ScMW->changeLayer(ss->getInt("ACTIVE"));
				ScMW->layerPalette->rebuildList();
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
	PDF_Options.BleedTop = pageMargins.Top;
	PDF_Options.BleedLeft = pageMargins.Left;
	PDF_Options.BleedRight = pageMargins.Right;
	PDF_Options.BleedBottom = pageMargins.Bottom;
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

Page* ScribusDoc::addPage(const int pageIndex, const QString& masterPageName, const bool addAutoFrame)
{
	Q_ASSERT(masterPageMode()==false);
	Page* addedPage = new Page(ScratchLeft, DocPages.count()*(pageHeight+ScratchBottom+ScratchTop)+ScratchTop, pageWidth, pageHeight);
	Q_ASSERT(addedPage!=NULL);
	addedPage->Margins.Top = pageMargins.Top;
	addedPage->Margins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Top = pageMargins.Top;
	addedPage->initialMargins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Left = pageMargins.Left;
	addedPage->initialMargins.Right = pageMargins.Right;
	addedPage->setPageNr(pageIndex);
	addedPage->PageSize = PageSize;
	addedPage->PageOri = PageOri;
	bool insertsuccess=DocPages.insert(pageIndex, addedPage);
	Q_ASSERT(insertsuccess==true && DocPages.at(pageIndex)!=NULL);
	currentPage = addedPage;
	//if (!masterPageMode())
	if (!masterPageName.isEmpty())
		applyMasterPage(masterPageName, pageIndex);
	setLocationBasedPageLRMargins(pageIndex);
	if (addAutoFrame && automaticTextFrames)
		addAutomaticTextFrame(pageIndex);
	return addedPage;
}

Page* ScribusDoc::addMasterPage(const int pageNumber, const QString& pageName)
{
	//CB We dont create master pages (yet) with a pageCount based location
	//Page* addedPage = new Page(ScratchLeft, MasterPages.count()*(pageHeight+ScratchBottom+ScratchTop)+ScratchTop, pageWidth, pageHeight);
	Page* addedPage = new Page(ScratchLeft, ScratchTop, pageWidth, pageHeight);
	Q_ASSERT(addedPage!=NULL);
	addedPage->Margins.Top = pageMargins.Top;
	addedPage->Margins.Bottom = pageMargins.Bottom;
	addedPage->Margins.Left = pageMargins.Left;//todo fix for layouts
	addedPage->Margins.Right = pageMargins.Right;
	addedPage->initialMargins.Top = pageMargins.Top;
	addedPage->initialMargins.Bottom = pageMargins.Bottom;
	addedPage->initialMargins.Left = pageMargins.Left;
	addedPage->initialMargins.Right = pageMargins.Right;
	addedPage->setPageNr(pageNumber);
	addedPage->PageSize = PageSize;
	addedPage->PageOri = PageOri;
	addedPage->MPageNam = "";
	addedPage->setPageName(pageName);
	MasterNames.insert(pageName, pageNumber);
	bool insertsuccess=MasterPages.insert(pageNumber, addedPage);
	Q_ASSERT(insertsuccess==true && MasterPages.at(pageNumber)!=NULL);
	return addedPage;
}

void ScribusDoc::deleteMasterPage(const int pageNumber)
{
	Q_ASSERT( Pages->count() > 1 && Pages->count() > static_cast<uint>(pageNumber) );
	Page* page = Pages->at(pageNumber);
	Pages->remove(pageNumber);
	delete page;
	// remove the master page from the master page name list
	MasterNames.remove(page->PageNam);
	// and fix up any pages that refer to the deleted master page
	for (Page* docPage = DocPages.first(); docPage; docPage = DocPages.next() )
	{
		if (docPage->MPageNam == page->PageNam)
			docPage->MPageNam = "Normal";
	}
}

void ScribusDoc::deletePage(const int pageNumber)
{
	Q_ASSERT( Pages->count() > 1 && Pages->count() > static_cast<uint>(pageNumber) );
	Page* page = Pages->at(pageNumber);
	Pages->remove(pageNumber);
	delete page;
	currentPage = Pages->at(0);
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
		if (LastAuto != 0)
			LastAuto->NextBox = Items->at(z);
		else
			FirstAuto = Items->at(z);
		LastAuto = Items->at(z);
		Items->at(z)->setRedrawBounding();
		return z;
	}
	return -1;
}

int ScribusDoc::addLayer(const QString& layerName=QString::null, const bool activate=false)
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

bool ScribusDoc::deleteLayer(const int layerNumber, const bool deleteItems)
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
		ScMW->LayerRemove(layerNumber, deleteItems);
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
						/* CB Removed forced loading of 0-9 for section based numbering
						for (uint numco = 0x30; numco < 0x3A; ++numco)
						{
							if (it->itemText.at(e)->cfont->CharWidth.contains(numco))
							{
								gly = it->itemText.at(e)->cfont->GlyphArray[numco].Outlines.copy();
								it->itemText.at(e)->cfont->RealGlyphs.insert(numco, gly);
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
								if ((DocPages.at(a)->MPageNam) == (MasterPages.at(it->OwnPage)->PageNam))
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
							if (it->itemText.at(e)->cfont->CharWidth.contains(chr))
							{
								FPointArray gly(it->itemText.at(e)->cfont->GlyphArray[chr].Outlines.copy());
								it->itemText.at(e)->cfont->RealGlyphs.insert(chr, gly);
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
			Really.insert(it->font(), UsedFonts[it->font()]);
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

int ScribusDoc::unitIndex() const
{
	return docUnitIndex;
}

double ScribusDoc::unitRatio() const
{
	return docUnitRatio;
}

bool ScribusDoc::applyMasterPage(const QString& in, const int pageNumber)
{
	if (UndoManager::undoEnabled())
	{
		if (DocPages.at(pageNumber)->MPageNam != in)
		{
			SimpleState *ss = new SimpleState(Um::ApplyMasterPage, QString(Um::FromTo).arg(DocPages.at(pageNumber)->MPageNam).arg(in));
			ss->set("PAGE_NUMBER", pageNumber);
			ss->set("OLD_MASTERPAGE", DocPages.at(pageNumber)->MPageNam);
			ss->set("NEW_MASTERPAGE", in);
			undoManager->action(this, ss);
		}
	}

	QString mna = in;
	if (mna == tr("Normal"))
		mna = "Normal";
	if (!MasterNames.contains(mna))
		mna = "Normal";
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
bool ScribusDoc::save(const QString& fileName)
{
	QFileInfo fi(fileName);
	QDir::setCurrent(fi.dirPath(true));
	QProgressBar* mainWindowProgressBar=NULL;
	if (ScQApp->usingGUI())
	{
		mainWindowProgressBar=ScMW->mainWindowProgressBar;
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
	//Recalculate the RGB or CMYK calues to new CMS settings
	ColorList::Iterator it;
	ColorList::Iterator itend=PageColors.end();
	for (it = PageColors.begin(); it != itend; ++it)
		PageColors[it.key()].RecalcRGB();
	//Adjust Items of the 3 types to the colors
	uint itemsCount=Items->count();
	updateAllItemQColors();
	for (uint c=0; c<itemsCount; ++c)
	{
		PageItem *ite = Items->at(c);
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

const bool ScribusDoc::copyPageToMasterPage(const int pageNumber, const int leftPage, const int maxLeftPage,  const QString& masterPageName)
{
	Q_ASSERT(!masterPageMode());
	if (masterPageMode())
		return false;
	//TODO Add Undo here
	int GrMax = GroupCounter;
	Page* sourcePage = Pages->at(pageNumber);
	int nr = MasterPages.count();
	//pageCount = 0;
	Page* targetPage=addMasterPage(nr, masterPageName);
	Q_ASSERT(targetPage!=NULL);
	setLoading(true);
	targetPage->copySizingProperties(sourcePage, pageMargins);
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
	//CB Need to set this so the paste works correctly. Should not need it really, but its a quick op.
	setMasterPageMode(true);
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
			ScMW->view->PasteItem(&BufferT, true, true);
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
	}
	targetPage->MPageNam = "";
	setLoading(false);
	GroupCounter = GrMax + 1;
	//Reset the current page..
	setMasterPageMode(false);
	currentPage=sourcePage;
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
		newItem->setRedrawBounding();
		//ScMW->view->setRedrawBounding(newItem);
		newItem->ContourLine = newItem->PoLine.copy();
	}

	if (frameType==PageItem::Ellipse)
	{
		newItem->SetOvalFrame();
		//TODO one day hopefully, if(ScQApp->usingGUI())
		newItem->setRedrawBounding();
		//ScMW->view->setRedrawBounding(newItem);
		newItem->ContourLine = newItem->PoLine.copy();
	}

	//ItemType Polygon
	if (itemType==PageItem::Polygon || itemType==PageItem::PolyLine)
	{
		newItem->PLineArt = PenStyle(toolSettings.dLineArt);
		newItem->setFillShade(toolSettings.dShade);
		newItem->setLineShade(toolSettings.dShade2);
	}
}


bool ScribusDoc::itemAddCommit(const int /*itemNumber*/)
{
	//TODO use the parameter
	if (_itemCreationTransactionStarted && appMode !=  modeDrawBezierLine)
	{
		PageItem *createdItem=selection->itemAt(0);
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

void ScribusDoc::updateFrameItems()
{
	uint frameItemsCount=FrameItems.count();
	for (uint a = 0; a < frameItemsCount; ++a)
		FrameItems.at(a)->ItemNr = a;
}

bool ScribusDoc::usesAutomaticTextFrames() const
{
	return automaticTextFrames;
}

void ScribusDoc::setUsesAutomaticTextFrames(const bool atf)
{
	automaticTextFrames=atf;
}

bool ScribusDoc::LoadPict(QString fn, int ItNr, bool reload)
{
	return loadPict(fn, Items->at(ItNr), reload);
}

bool ScribusDoc::loadPict(QString fn, PageItem *pageItem, bool reload)
{
	if (!reload)
	{
		if ((ScMW->fileWatcher->files().contains(pageItem->Pfile) != 0) && (pageItem->PicAvail))
			ScMW->fileWatcher->removeFile(pageItem->Pfile);
	}
	if(!pageItem->loadImage(fn, reload))
		return false;
	if (!reload)
		ScMW->fileWatcher->addFile(pageItem->Pfile);
	if (!isLoading())
	{
		//TODO: Make this a signal again one day
		//emit RasterPic(pageItem->isRaster);
		ScMW->HaveRaster(pageItem->isRaster);
		//TODO: Previously commented out.. unsure why, remove later
		//emit UpdtObj(PageNr, ItNr);
		//TODO: Make this a signal again one day
		//emit DocChanged();
		ScMW->slotDocCh();
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
		int x = static_cast<int>(currentPage->xOffset());
		int y = static_cast<int>(currentPage->yOffset());
		int w = static_cast<int>(currentPage->width());
		int h = static_cast<int>(currentPage->height());
		if (QRect(x, y, w, h).contains(QPoint(qRound(x2), qRound(y2))))
			retw = currentPage->pageNr();
	}
	else
	{
		uint docPageCount=Pages->count();
		for (uint a = 0; a < docPageCount; ++a)
		{
			int x = static_cast<int>(Pages->at(a)->xOffset());
			int y = static_cast<int>(Pages->at(a)->yOffset());
			int w = static_cast<int>(Pages->at(a)->width());
			int h = static_cast<int>(Pages->at(a)->height());
			if (QRect(x, y, w, h).contains(QPoint(qRound(x2), qRound(y2))))
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
		int x = static_cast<int>(currentPage->xOffset());
		int y = static_cast<int>(currentPage->yOffset());
		int w = static_cast<int>(currentPage->width());
		int h = static_cast<int>(currentPage->height());
		int x2 = static_cast<int>(currItem->xPos());
		int y2 = static_cast<int>(currItem->yPos());
		int w2 = QMAX(static_cast<int>(currItem->width()), 1);
		int h2 = QMAX(static_cast<int>(currItem->height()), 1);
		if (QRect(x, y, w, h).intersects(QRect(x2, y2, w2, h2)))
			retw = currentPage->pageNr();
	}
	else
	{
		uint docPageCount=Pages->count();
		for (uint a = 0; a < docPageCount; ++a)
		{
			int x = static_cast<int>(Pages->at(a)->xOffset());
			int y = static_cast<int>(Pages->at(a)->yOffset());
			int w = static_cast<int>(Pages->at(a)->width());
			int h = static_cast<int>(Pages->at(a)->height());
			int x2 = static_cast<int>(currItem->xPos());
			int y2 = static_cast<int>(currItem->yPos());
			int w2 = QMAX(static_cast<int>(currItem->width()), 1);
			int h2 = QMAX(static_cast<int>(currItem->height()), 1);
			if (QRect(x, y, w, h).intersects(QRect(x2, y2, w2, h2)))
			{
				retw = static_cast<int>(a);;
				break;
			}
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
void ScribusDoc::reformPages(double& maxX, double& maxY, bool moveObjects)
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
				currentYPos += QMAX(lastYPos, Seite->height())+pageSets[currentPageLayout].GapVertical;
				lastYPos = QMAX(lastYPos, Seite->height());
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
	maxX=maxXPos;
	maxY=maxYPos;
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
	bool removedFromSelection=selection->removeItem(oldItem);
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
				undoManager->beginTransaction(currentPage->getUName(), 0, Um::TextFrame, "", Um::ITextFrame);
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
				undoManager->beginTransaction(currentPage->getUName(), 0, Um::PathText, "", Um::ITextFrame);
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
				polyLineItem->SetPolyClip(qRound(QMAX(polyLineItem->lineWidth() / 2, 1)));
				ScMW->view->AdjustItemSize(polyLineItem);

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
			newItem->SetPolyClip(qRound(QMAX(newItem->lineWidth() / 2, 1)));
			ScMW->view->AdjustItemSize(newItem);
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
				ScMW->view->AdjustItemSize(newItem);
				newItem->UpdatePolyClip();
				double dx = secondaryItem->xPos() - newItem->xPos();
				double dy = secondaryItem->yPos() - newItem->yPos();
				ScMW->view->MoveItem(dx, dy, newItem);
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
		selection->addItem(newItem);
	//If converting text to path, delete the bezier
	if (newType==PageItem::PathText)
	{
		//FIXME: Stop using the view here
		ScMW->view->SelectItem(secondaryItem);
		ScMW->view->DeleteItem();
		ScMW->view->updateContents();
		ScMW->view->Deselect(true);
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
	return currentPage->pageNr();
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

void ScribusDoc::setMasterPageMode(const bool changeToMasterPageMode)
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
	uint currMinIndex, currMaxIndex;
	QMap<uint, DocumentSection>::Iterator itprev=sections.begin();
	QMap<uint, DocumentSection>::Iterator it=itprev;
	for ( ; it != sections.end(); ++it )
	{
		currMinIndex=it.data().fromindex;
		currMaxIndex=it.data().toindex;

		if (it.key()!=number)
			itprev=it;
		else
			break;
	}
	itprev.data().toindex=currMaxIndex;
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
	int GrMax = GroupCounter;
	for (int copyNumber=1; copyNumber<=copyCount; ++copyNumber)
	{
		//For multiple insertions we can insert in the same place
		int destLocation=existingPage;
		if (whereToInsert==0)
			--destLocation;
		else if (whereToInsert==2)
			destLocation=DocPages.count();
		//ScMW->slotNewPage(destLocation);
		currentPage=addPage(destLocation, from->MPageNam);
		Page* destination = currentPage; //slotNewPage sets currentPage
		destination->setInitialHeight(from->height());
		destination->setInitialWidth(from->width());
		destination->PageOri = from->PageOri;
		destination->PageSize = from->PageSize;
		destination->initialMargins.Top = from->Margins.Top;
		destination->initialMargins.Bottom = from->Margins.Bottom;
		destination->initialMargins.Left = from->Margins.Left;
		destination->initialMargins.Right = from->Margins.Right;
		ScMW->view->reformPages();
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
			if (itemToCopy->OwnPage == static_cast<int>(from->pageNr()))
			{
				struct CopyPasteBuffer Buffer;
				itemToCopy->copyToCopyPasteBuffer(&Buffer);
				Buffer.Xpos = Buffer.Xpos - from->xOffset() + destination->xOffset();
				Buffer.Ypos = Buffer.Ypos - from->yOffset() + destination->yOffset();
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
				ScMW->view->PasteItem(&Buffer, true, true);
				PageItem* Neu = Items->at(Items->count()-1);
				Neu->OnMasterPage = "";
				if (itemToCopy->isBookmark)
					ScMW->AddBookMark(Neu);
				if (Neu->isTableItem)
				{
					TableItems.append(Neu);
					TableID.insert(ite, Neu->ItemNr);
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
		//ScMW->Apply_MasterPage(from->MPageNam, destination->pageNr(), false);
		if (from->YGuides.count() != 0)
		{
			for (uint y = 0; y < from->YGuides.count(); ++y)
			{
				if (destination->YGuides.contains(from->YGuides[y]) == 0)
					destination->YGuides.append(from->YGuides[y]);
			}
			qHeapSort(destination->YGuides);
		}
		if (from->XGuides.count() != 0)
		{
			for (uint x = 0; x < from->XGuides.count(); ++x)
			{
				if (destination->XGuides.contains(from->XGuides[x]) == 0)
					destination->XGuides.append(from->XGuides[x]);
			}
			qHeapSort(destination->XGuides);
		}
		GroupCounter = GrMax + 1;
	}
	setUsesAutomaticTextFrames(autoText);
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
	int myCol=(pageIndex+pageSets[currentPageLayout].FirstPage)%setcol;

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
}

//CB Moved from view
void ScribusDoc::RecalcPictures(ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia)
{
	uint docItemCount=Items->count();
	if ( docItemCount!= 0)
	{
		int counter;
		bool usingGUI=ScQApp->usingGUI();
		if (usingGUI)
		{
			if (dia != NULL)
				counter = dia->progress();
			else
				counter = 0;
		}
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

void ScribusDoc::buildAlignItemList()
{
	PageItem *currItem;
	int ObjGroup;
	struct AlignObjs Object;
	AObjects.clear();
	for (uint a = 0; a < selection->count(); ++a)
	{
		currItem = selection->itemAt(a);
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
	uint docSelectionCount=selection->count();
	if ((Items->count() > 1) && (docSelectionCount != 0))
	{
		QMap<int, uint> ObjOrder;
		PageItem *currItem;
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = selection->itemAt(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return false;
			ObjOrder.insert(currItem->ItemNr, c);
			int d = Items->findRef(currItem);
			Items->take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			Items->prepend(selection->itemAt(Oindex[c]));
		}
		/*
		for (uint a = 0; a < Items->count(); ++a)
		{
			Items->at(a)->ItemNr = a;
			if (Items->at(a)->isBookmark)
				emit NewBMNr(Items->at(a)->BMnr, a);
		}*/
		return true;
	}
	return false;
}

bool ScribusDoc::bringItemSelectionToFront()
{
	uint docSelectionCount=selection->count();
	if ((Items->count() > 1) && (docSelectionCount != 0))
	{
		QMap<int, uint> ObjOrder;
		PageItem *currItem;
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = selection->itemAt(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return false;
			ObjOrder.insert(currItem->ItemNr, c);
			int d = Items->findRef(currItem);
			Items->take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = 0; c <static_cast<int>(Oindex.count()); ++c)
		{
			Items->append(selection->itemAt(Oindex[c]));
		}
		/*
		for (uint a = 0; a < Items->count(); ++a)
		{
			Items->at(a)->ItemNr = a;
			if (Items->at(a)->isBookmark)
				emit NewBMNr(Items->at(a)->BMnr, a);
		}
		*/
		return true;
	}
	return false;
}

void ScribusDoc::ChLineWidth(double w)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineWidth, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			PageItem *currItem = selection->itemAt(a);
			//cb moved to setlinewidth
			//currItem->Oldm_lineWidth = currItem->lineWidth();
			currItem->setLineWidth(w);
			if (currItem->asPolyLine())
				currItem->SetPolyClip(qRound(QMAX(currItem->lineWidth() / 2, 1)));
			if (currItem->asLine())
			{
				int ph = static_cast<int>(QMAX(1.0, w / 2.0));
				currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
				                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
				                  -ph,static_cast<int>(currItem->height()+ph));
			}
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineArt(PenStyle w)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
						  Um::IGroup, Um::LineStyle, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			selection->itemAt(a)->setLineStyle(w);
			ScMW->view->RefreshItem(selection->itemAt(a));
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineJoin(PenJoinStyle w)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineJoin, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			selection->itemAt(a)->setLineJoin(w);
			ScMW->view->RefreshItem(selection->itemAt(a));
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineEnd(PenCapStyle w)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::LineEnd, "", Um::ILineStyle);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			selection->itemAt(a)->setLineEnd(w);
			ScMW->view->RefreshItem(selection->itemAt(a));
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineSpa(double w)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetLineSpacing, QString("%1").arg(w), Um::IFont);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			selection->itemAt(a)->setLineSpacing(w);
			ScMW->view->RefreshItem(selection->itemAt(a));
		}
		docParagraphStyles[0].LineSpa = w;
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLocalXY(double x, double y)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			currItem->setImageXYOffset(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QWMatrix cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::ChLocalSc(double x, double y)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			currItem->setImageXYScale(x, y);
			if (currItem->imageClip.size() != 0)
			{
				currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
				QWMatrix cl;
				cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
				cl.scale(currItem->imageXScale(), currItem->imageYScale());
				currItem->imageClip.map(cl);
			}
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::ItemFont(QString fon)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFont, fon, Um::IFont);
		for (uint aa = 0; aa < selectedItemCount; ++aa)
		{
			PageItem *currItem = selection->itemAt(aa);
			if (appMode == modeNormal)
			{
				currItem->setFont(fon);
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
						currItem->itemText.at(a)->cfont = (*AllFonts)[fon];
					if (currItem->asPathText())
					{
						currItem->UpdatePolyClip();
						ScMW->view->AdjustItemSize(currItem);
					}
/*					if (!Doc->loading)
						emit UpdtObj(Doc->currentPage->pageNr(), currItem->ItemNr); */
					ScMW->view->RefreshItem(currItem);
				}
			}
			if ((currItem->HasSel) && (appMode == modeEdit))
			{
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cselect)
							currItem->itemText.at(a)->cfont = (*AllFonts)[fon];
					}
					ScMW->view->RefreshItem(currItem);
				}
			}
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemPen(QString farbe)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (farbe == ScMW->noneString)
			farbe = CommonStrings::None;
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetLineColor, farbe, Um::IFill);
		PageItem *i;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			i = selection->itemAt(a);
			if ((i->asLine()) && (farbe == CommonStrings::None))
				continue;

			i->setLineColor(farbe);
			ScMW->view->RefreshItem(i);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemTextBrush(QString farbe)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (farbe == ScMW->noneString)
			farbe = CommonStrings::None;
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontFill,
										  farbe, Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
					currItem->setFontFillColor(farbe);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->ccolor = farbe;
					}
					else
						currItem->itemText.at(i)->ccolor = farbe;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemTextBrushS(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		if (selection->count() > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontFillShade,
									  QString("%1").arg(sha), Um::IFont);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if (currItem->asTextFrame())
			{
				if (appMode != modeEdit)
					currItem->setFontFillShade(sha);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cshade = sha;
					}
					else
						currItem->itemText.at(i)->cshade = sha;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemTextPen(QString farbe)
{
	if (farbe == ScMW->noneString)
		farbe = CommonStrings::None;
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontStroke,
										  farbe, Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
					currItem->setFontStrokeColor(farbe);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cstroke = farbe;
					}
					else
						currItem->itemText.at(i)->cstroke = farbe;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemTextPenS(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontStrokeShade,
									  QString("%1").arg(sha), Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if (currItem->asTextFrame())
			{
				if (appMode != modeEdit)
					currItem->setFontStrokeShade(sha);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cshade2 = sha;
					}
					else
						currItem->itemText.at(i)->cshade2 = sha;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemTextScaleV(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontHeight, QString("%1").arg(sha), Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
					currItem->setFontHeight(sha);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cscalev = sha;
					}
					else
						currItem->itemText.at(i)->cscalev = sha;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemTextScale(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontWidth,
										  QString("%1").arg(sha), Um::IFont);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
					currItem->setFontWidth(sha);
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cscale = sha;
					}
					else
						currItem->itemText.at(i)->cscale = sha;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::setItemTextShadow(int shx, int shy)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
				{
					currItem->TxtShadowX = shx;
					currItem->TxtShadowY = shy;
				}
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
						{
							currItem->itemText.at(i)->cshadowx = shx;
							currItem->itemText.at(i)->cshadowy = shy;
						}
					}
					else
					{
						currItem->itemText.at(i)->cshadowx = shx;
						currItem->itemText.at(i)->cshadowy = shy;
					}
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::setItemTextUnderline(int pos, int wid)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
				{
					currItem->TxtUnderPos = pos;
					currItem->TxtUnderWidth = wid;
				}
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
						{
							currItem->itemText.at(i)->cunderpos = pos;
							currItem->itemText.at(i)->cunderwidth = wid;
						}
					}
					else
					{
						currItem->itemText.at(i)->cunderpos = pos;
						currItem->itemText.at(i)->cunderwidth = wid;
					}
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::setItemTextStrike(int pos, int wid)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
				{
					currItem->TxtStrikePos = pos;
					currItem->TxtStrikeWidth = wid;
				}
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
						{
							currItem->itemText.at(i)->cstrikepos = pos;
							currItem->itemText.at(i)->cstrikewidth = wid;
						}
					}
					else
					{
						currItem->itemText.at(i)->cstrikepos = pos;
						currItem->itemText.at(i)->cstrikewidth = wid;
					}
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::setItemTextBase(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
					currItem->TxtBase = sha;
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->cbase = sha;
					}
					else
						currItem->itemText.at(i)->cbase = sha;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::setItemTextOutline(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
				if (appMode != modeEdit)
					currItem->TxtOutline = sha;
				for (uint i=0; i<currItem->itemText.count(); ++i)
				{
					if (appMode == modeEdit)
					{
						if (currItem->itemText.at(i)->cselect)
							currItem->itemText.at(i)->coutline = sha;
					}
					else
						currItem->itemText.at(i)->coutline = sha;
				}
			}
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::ItemBrush(QString farbe)
{
	if (farbe == ScMW->noneString)
		farbe = CommonStrings::None;
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetFill, farbe, Um::IFill);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			currItem->setFillColor(farbe);
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemBrushShade(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
										  Um::IGroup, Um::SetShade, QString("%1").arg(sha),
										  Um::IShade);
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			currItem->setFillShade(sha);
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemPenShade(int sha)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup,
							Um::IGroup, Um::SetLineShade, QString("%1").arg(sha), Um::IShade);
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			currItem->setLineShade(sha);
			ScMW->view->RefreshItem(currItem);
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ItemGradFill(int typ)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		PageItem *currItem;
		for (uint a = 0; a < selectedItemCount; ++a)
		{
			currItem = selection->itemAt(a);
			currItem->GrType = typ;
			currItem->updateGradientVectors();
			ScMW->view->RefreshItem(currItem);
		}
	}
}

void ScribusDoc::chTyStyle(int s)
{
	uint selectedItemCount=selection->count();
	if (selectedItemCount != 0)
	{
		if (selectedItemCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontEffect, "", Um::IFont);
		for (uint aa = 0; aa < selectedItemCount; ++aa)
		{
			PageItem *currItem = selection->itemAt(aa);
			if (appMode != modeEdit)
				currItem->setFontEffects(s);
			if (currItem->itemText.count() != 0)
			{
				if (appMode == modeEdit)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cselect)
						{
							currItem->itemText.at(a)->cstyle &= ~1919;
							currItem->itemText.at(a)->cstyle |= s;
						}
					}
				}
				else
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						currItem->itemText.at(a)->cstyle &= ~1919;
						currItem->itemText.at(a)->cstyle |= s;
					}
				}
				ScMW->view->RefreshItem(currItem);
			}
		}
		if (selectedItemCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::SetAbStyle(int s)
{
	PageItem *currItem;
	if (ScMW->view->GetItem(&currItem))
		chAbStyle(currItem, s);
}

void ScribusDoc::chAbStyle(PageItem *currItem, int s)
{
	int a, ax;
	PageItem *nextItem;
	bool cr = true;
	if (appMode == modeEdit)
	{
		nextItem = currItem;
		a = currItem->CPos;
		if (a == static_cast<int>(nextItem->itemText.count()))
			a -= 1;
		while ((cr) && (nextItem != 0))
		{
			if (nextItem->itemText.count() == 0)
			{
				nextItem = nextItem->BackBox;
				if (nextItem == 0)
				{
					cr = false;
					break;
				}
				a = static_cast<int>(nextItem->itemText.count()-1);
			}
			ax = a;
			for (int xx=0; xx<ax+1; ++xx)
			{
				if (nextItem->itemText.at(a)->ch == QChar(13))
				{
					cr = false;
					break;
				}
				if (s > 4)
				{
					if (!docParagraphStyles[s].Font.isEmpty()
						&& (!nextItem->HasSel || nextItem->itemText.at(a)->cselect))
					{
						nextItem->itemText.at(a)->cfont = (*AllFonts)[docParagraphStyles[s].Font];
						nextItem->itemText.at(a)->csize = docParagraphStyles[s].FontSize;
						nextItem->itemText.at(a)->cstyle &= ~1919;
						nextItem->itemText.at(a)->cstyle |= docParagraphStyles[s].FontEffect;
						nextItem->itemText.at(a)->ccolor = docParagraphStyles[s].FColor;
						nextItem->itemText.at(a)->cshade = docParagraphStyles[s].FShade;
						nextItem->itemText.at(a)->cstroke = docParagraphStyles[s].SColor;
						nextItem->itemText.at(a)->cshade2 = docParagraphStyles[s].SShade;
						nextItem->itemText.at(a)->cshadowx = docParagraphStyles[s].txtShadowX;
						nextItem->itemText.at(a)->cshadowy = docParagraphStyles[s].txtShadowY;
						nextItem->itemText.at(a)->coutline = docParagraphStyles[s].txtOutline;
						nextItem->itemText.at(a)->cunderpos = docParagraphStyles[s].txtUnderPos;
						nextItem->itemText.at(a)->cunderwidth = docParagraphStyles[s].txtUnderWidth;
						nextItem->itemText.at(a)->cstrikepos = docParagraphStyles[s].txtStrikePos;
						nextItem->itemText.at(a)->cstrikewidth = docParagraphStyles[s].txtStrikeWidth;
						nextItem->itemText.at(a)->cscale = docParagraphStyles[s].scaleH;
						nextItem->itemText.at(a)->cscalev = docParagraphStyles[s].scaleV;
						nextItem->itemText.at(a)->cbase = docParagraphStyles[s].baseOff;
						nextItem->itemText.at(a)->cextra = docParagraphStyles[s].kernVal;
					}
				}
				if ((s < 5) && (nextItem->itemText.at(a)->cab > 4))
				{
					nextItem->itemText.at(a)->ccolor = nextItem->TxtFill;
					nextItem->itemText.at(a)->cshade = nextItem->ShTxtFill;
					nextItem->itemText.at(a)->cstroke = nextItem->TxtStroke;
					nextItem->itemText.at(a)->cshade2 = nextItem->ShTxtStroke;
					nextItem->itemText.at(a)->csize = nextItem->fontSize();
					nextItem->itemText.at(a)->cfont = (*AllFonts)[nextItem->font()];
					nextItem->itemText.at(a)->cstyle &= ~1919;
					nextItem->itemText.at(a)->cstyle |= nextItem->TxTStyle;
					nextItem->itemText.at(a)->cshadowx = nextItem->TxtShadowX;
					nextItem->itemText.at(a)->cshadowy = nextItem->TxtShadowY;
					nextItem->itemText.at(a)->coutline = nextItem->TxtOutline;
					nextItem->itemText.at(a)->cunderpos = nextItem->TxtUnderPos;
					nextItem->itemText.at(a)->cunderwidth = nextItem->TxtUnderWidth;
					nextItem->itemText.at(a)->cstrikepos = nextItem->TxtStrikePos;
					nextItem->itemText.at(a)->cstrikewidth = nextItem->TxtStrikeWidth;
					nextItem->itemText.at(a)->cscale = nextItem->TxtScale;
					nextItem->itemText.at(a)->cscalev = nextItem->TxtScaleV;
					nextItem->itemText.at(a)->cbase = nextItem->TxtBase;
					nextItem->itemText.at(a)->cextra = nextItem->ExtraV;
				}
				nextItem->itemText.at(a)->cab = s;
				a--;
			}
			if (cr)
			{
				nextItem = nextItem->BackBox;
				if (nextItem != 0)
					a = static_cast<int>(nextItem->itemText.count()-1);
			}
		}
		a = currItem->CPos;
		cr = true;
		nextItem = currItem;
		if (a == static_cast<int>(nextItem->itemText.count()))
			cr = false;
		while ((cr) && (nextItem != 0))
		{
			while (a < static_cast<int>(nextItem->itemText.count()))
			{
				if (s > 4)
				{
					if (!docParagraphStyles[s].Font.isEmpty()
						&& (!nextItem->HasSel || nextItem->itemText.at(a)->cselect))
					{
						nextItem->itemText.at(a)->cfont = (*AllFonts)[docParagraphStyles[s].Font];
						nextItem->itemText.at(a)->csize = docParagraphStyles[s].FontSize;
						nextItem->itemText.at(a)->cstyle &= ~1919;
						nextItem->itemText.at(a)->cstyle |= docParagraphStyles[s].FontEffect;
						nextItem->itemText.at(a)->ccolor = docParagraphStyles[s].FColor;
						nextItem->itemText.at(a)->cshade = docParagraphStyles[s].FShade;
						nextItem->itemText.at(a)->cstroke = docParagraphStyles[s].SColor;
						nextItem->itemText.at(a)->cshade2 = docParagraphStyles[s].SShade;
						nextItem->itemText.at(a)->cshadowx = docParagraphStyles[s].txtShadowX;
						nextItem->itemText.at(a)->cshadowy = docParagraphStyles[s].txtShadowY;
						nextItem->itemText.at(a)->coutline = docParagraphStyles[s].txtOutline;
						nextItem->itemText.at(a)->cunderpos = docParagraphStyles[s].txtUnderPos;
						nextItem->itemText.at(a)->cunderwidth = docParagraphStyles[s].txtUnderWidth;
						nextItem->itemText.at(a)->cstrikepos = docParagraphStyles[s].txtStrikePos;
						nextItem->itemText.at(a)->cstrikewidth = docParagraphStyles[s].txtStrikeWidth;
						nextItem->itemText.at(a)->cscale = docParagraphStyles[s].scaleH;
						nextItem->itemText.at(a)->cscalev = docParagraphStyles[s].scaleV;
						nextItem->itemText.at(a)->cbase = docParagraphStyles[s].baseOff;
						nextItem->itemText.at(a)->cextra = docParagraphStyles[s].kernVal;
					}
				}
				if ((s < 5) && (nextItem->itemText.at(a)->cab > 4))
				{
					nextItem->itemText.at(a)->ccolor = nextItem->TxtFill;
					nextItem->itemText.at(a)->cshade = nextItem->ShTxtFill;
					nextItem->itemText.at(a)->cstroke = nextItem->TxtStroke;
					nextItem->itemText.at(a)->cshade2 = nextItem->ShTxtStroke;
					nextItem->itemText.at(a)->csize = nextItem->fontSize();
					nextItem->itemText.at(a)->cfont = (*AllFonts)[nextItem->font()];
					nextItem->itemText.at(a)->cstyle &= ~1919;
					nextItem->itemText.at(a)->cstyle |= nextItem->TxTStyle;
					nextItem->itemText.at(a)->cshadowx = nextItem->TxtShadowX;
					nextItem->itemText.at(a)->cshadowy = nextItem->TxtShadowY;
					nextItem->itemText.at(a)->coutline = nextItem->TxtOutline;
					nextItem->itemText.at(a)->cunderpos = nextItem->TxtUnderPos;
					nextItem->itemText.at(a)->cunderwidth = nextItem->TxtUnderWidth;
					nextItem->itemText.at(a)->cstrikepos = nextItem->TxtStrikePos;
					nextItem->itemText.at(a)->cstrikewidth = nextItem->TxtStrikeWidth;
					nextItem->itemText.at(a)->cscale = nextItem->TxtScale;
					nextItem->itemText.at(a)->cscalev = nextItem->TxtScaleV;
					nextItem->itemText.at(a)->cbase = nextItem->TxtBase;
					nextItem->itemText.at(a)->cextra = nextItem->ExtraV;
				}
				nextItem->itemText.at(a)->cab = s;
				if ((nextItem->itemText.at(a)->ch == QChar(13)) && (!nextItem->itemText.at(a)->cselect))
				{
					cr = false;
					break;
				}
				++a;
			}
			if (cr)
			{
				nextItem = nextItem->NextBox;
				a = 0;
			}
		}
	}
	else
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(s > 4 ? Um::SetStyle : Um::AlignText, "", Um::IFont);
			ss->set("PSTYLE", "pstyle");
			ss->set("OLD_STYLE", currItem->textAlignment);
			ss->set("NEW_STYLE", s);
			undoManager->action(currItem, ss);
		}
		currItem->textAlignment = s;
		if (currItem->itemText.count() != 0)
		{
			for (a = 0; a < static_cast<int>(currItem->itemText.count()); ++a)
			{
				if (s > 4)
				{
					if (!docParagraphStyles[s].Font.isEmpty())
					{
						currItem->itemText.at(a)->cfont = (*AllFonts)[docParagraphStyles[s].Font];
						currItem->itemText.at(a)->csize = docParagraphStyles[s].FontSize;
						currItem->itemText.at(a)->cstyle &= ~1919;
						currItem->itemText.at(a)->cstyle |= docParagraphStyles[s].FontEffect;
						currItem->itemText.at(a)->ccolor = docParagraphStyles[s].FColor;
						currItem->itemText.at(a)->cshade = docParagraphStyles[s].FShade;
						currItem->itemText.at(a)->cstroke = docParagraphStyles[s].SColor;
						currItem->itemText.at(a)->cshade2 = docParagraphStyles[s].SShade;
						currItem->itemText.at(a)->cshadowx = docParagraphStyles[s].txtShadowX;
						currItem->itemText.at(a)->cshadowy = docParagraphStyles[s].txtShadowY;
						currItem->itemText.at(a)->coutline = docParagraphStyles[s].txtOutline;
						currItem->itemText.at(a)->cunderpos = docParagraphStyles[s].txtUnderPos;
						currItem->itemText.at(a)->cunderwidth = docParagraphStyles[s].txtUnderWidth;
						currItem->itemText.at(a)->cstrikepos = docParagraphStyles[s].txtStrikePos;
						currItem->itemText.at(a)->cstrikewidth = docParagraphStyles[s].txtStrikeWidth;
						currItem->itemText.at(a)->cscale = docParagraphStyles[s].scaleH;
						currItem->itemText.at(a)->cscalev = docParagraphStyles[s].scaleV;
						currItem->itemText.at(a)->cbase = docParagraphStyles[s].baseOff;
						currItem->itemText.at(a)->cextra = docParagraphStyles[s].kernVal;
					}
				}
				if ((s < 5) && (currItem->itemText.at(a)->cab > 4))
				{
					currItem->itemText.at(a)->ccolor = currItem->TxtFill;
					currItem->itemText.at(a)->cshade = currItem->ShTxtFill;
					currItem->itemText.at(a)->cstroke = currItem->TxtStroke;
					currItem->itemText.at(a)->cshade2 = currItem->ShTxtStroke;
					currItem->itemText.at(a)->cfont = (*AllFonts)[currItem->font()];
					currItem->itemText.at(a)->csize = currItem->fontSize();
					currItem->itemText.at(a)->cstyle &= ~1919;
					currItem->itemText.at(a)->cstyle |= currItem->TxTStyle;
					currItem->itemText.at(a)->cshadowx = currItem->TxtShadowX;
					currItem->itemText.at(a)->cshadowy = currItem->TxtShadowY;
					currItem->itemText.at(a)->coutline = currItem->TxtOutline;
					currItem->itemText.at(a)->cunderpos = currItem->TxtUnderPos;
					currItem->itemText.at(a)->cunderwidth = currItem->TxtUnderWidth;
					currItem->itemText.at(a)->cstrikepos = currItem->TxtStrikePos;
					currItem->itemText.at(a)->cstrikewidth = currItem->TxtStrikeWidth;
					currItem->itemText.at(a)->cscale = currItem->TxtScale;
					currItem->itemText.at(a)->cscalev = currItem->TxtScaleV;
					currItem->itemText.at(a)->cbase = currItem->TxtBase;
					currItem->itemText.at(a)->cextra = currItem->ExtraV;
				}
				currItem->itemText.at(a)->cab = s;
			}
		}
	}
	if (!currItem->Tinput)
		ScMW->view->RefreshItem(currItem);
	if (appMode == modeEdit)
	{
		ScMW->view->horizRuler->setItemPosition(currItem->xPos(), currItem->width());
		if (currItem->lineColor() != CommonStrings::None)
			ScMW->view->horizRuler->lineCorr = currItem->lineWidth() / 2.0;
		else
			ScMW->view->horizRuler->lineCorr = 0;
		ScMW->view->horizRuler->ColGap = currItem->ColGap;
		ScMW->view->horizRuler->Cols = currItem->Cols;
		ScMW->view->horizRuler->Extra = currItem->textToFrameDistLeft();
		ScMW->view->horizRuler->RExtra = currItem->textToFrameDistRight();
		ScMW->view->horizRuler->First = docParagraphStyles[s].First;
		ScMW->view->horizRuler->Indent = docParagraphStyles[s].Indent;
		ScMW->view->horizRuler->Revers = (currItem->imageFlippedH() || (currItem->reversed()));
		ScMW->view->horizRuler->ItemPosValid = true;
		if (s < 5)
			ScMW->view->horizRuler->TabValues = currItem->TabValues;
		else
			ScMW->view->horizRuler->TabValues = docParagraphStyles[s].TabValues;
		ScMW->view->horizRuler->repaint();
	}
}

void ScribusDoc::chKerning(int us)
{
	uint docSelectionCount=selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetKerning,
										  QString("%1").arg(us), Um::IFont);
		for (uint aa = 0; aa < docSelectionCount; ++aa)
		{
			PageItem *currItem = selection->itemAt(aa);
			if ((currItem->HasSel) && (appMode == modeEdit))
			{
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						if (currItem->itemText.at(a)->cselect)
							currItem->itemText.at(a)->cextra = us;
					}
					ScMW->view->RefreshItem(currItem);
				}
			}
			else
			{
				if (appMode != modeEdit)
					currItem->setKerning(us);
				if (currItem->itemText.count() != 0)
				{
					for (uint a = 0; a < currItem->itemText.count(); ++a)
					{
						currItem->itemText.at(a)->cextra = us;
					}
					ScMW->view->RefreshItem(currItem);
				}
			}
		}
		if (docSelectionCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::ChLineSpaMode(int w)
{
	if (appMode != modeEdit)
	{
		if (selection->count() != 0)
		{
			for (uint a = 0; a < selection->count(); ++a)
			{
				PageItem* currItem = selection->itemAt(a);
				currItem->setLineSpacingMode(w);
				if (w == 0)
				{
					currItem->setLineSpacing(((currItem->fontSize() / 10.0) * static_cast<double>(typographicSettings.autoLineSpacing) / 100) + (currItem->fontSize() / 10.0));
					docParagraphStyles[0].BaseAdj = false;
				}
				else if (w == 1)
				{
					docParagraphStyles[0].BaseAdj = false;
					currItem->setLineSpacing(RealFHeight(this, (*AllFonts)[currItem->font()], currItem->fontSize()));
				}
				else
				{
					docParagraphStyles[0].BaseAdj = true;
					currItem->setLineSpacing(typographicSettings.valueBaseGrid-1);
				}
				docParagraphStyles[0].LineSpa = currItem->lineSpacing();
				ScMW->view->RefreshItem(currItem);
			}
			docParagraphStyles[0].LineSpaMode = w;
		}
	}
}

void ScribusDoc::chFSize(int size)
{
	uint docSelectionCount=selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetFontSize,
										  QString("%1").arg(size/10.0), Um::IFont);
		for (uint aa = 0; aa < docSelectionCount; ++aa)
		{
			PageItem *currItem = selection->itemAt(0);
			CurrFontSize = size;
			if (appMode != modeEdit)
			{
				if (currItem->lineSpacingMode() == 0)
				{
					currItem->setLineSpacing(((size / 10.0) * static_cast<double>(typographicSettings.autoLineSpacing) / 100) + (size / 10.0));
					docParagraphStyles[0].LineSpa = currItem->lineSpacing();
				}
				else if (currItem->lineSpacingMode() == 1)
				{
					currItem->setLineSpacing(RealFHeight(this, (*AllFonts)[currItem->font()], currItem->fontSize()));
					docParagraphStyles[0].LineSpa = currItem->lineSpacing();
				}
				else
				{
					currItem->setLineSpacing(typographicSettings.valueBaseGrid-1);
				}
				currItem->setFontSize(size);
				//CB move from view to doc.. why are we emitting column data here?
				//emit ItemTextCols(currItem->Cols, currItem->ColGap);
			}
			uint currItemTextCount=currItem->itemText.count();
			if (currItemTextCount != 0)
			{
				if (appMode == modeEdit)
				{
					for (uint a = 0; a < currItemTextCount; ++a)
					{
						if (currItem->itemText.at(a)->cselect)
							currItem->itemText.at(a)->csize = size;
					}
				}
				else
				{
					for (uint a = 0; a < currItemTextCount; ++a)
					{
						if (currItem->itemText.at(a)->cab < 5)
							currItem->itemText.at(a)->csize = size;
					}
				}
				if (currItem->asPathText())
				{
					currItem->UpdatePolyClip();
					ScMW->view->AdjustItemSize(currItem);
				}
				ScMW->view->RefreshItem(currItem);
			}
		}
		if (docSelectionCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::FlipImageH()
{
	uint docSelectionCount=selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
										  Um::FlipH, 0, Um::IFlipH);
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			selection->itemAt(a)->flipImageH();
			ScMW->view->RefreshItem(selection->itemAt(a));
		}
		emit changed();
		if (docSelectionCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::FlipImageV()
{
	uint docSelectionCount=selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
			undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
										  Um::FlipV, 0, Um::IFlipV);
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			selection->itemAt(a)->flipImageV();
			//ScMW->view->RefreshItem(selection->itemAt(a));
			emit refreshItem(selection->itemAt(a));
		}
		emit changed();
		if (docSelectionCount > 1)
			undoManager->commit();
	}
}

void ScribusDoc::MirrorPolyH()
{
	PageItem *currItem = selection->itemAt(0);
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (ScMW->view->EditContour)
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
		ScMW->view->updateContents();
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		ScMW->view->MarkClip(currItem, currItem->ContourLine, true);
		emit changed();
		return;
	}
	ma.scale(-1, 1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(currItem->width(), 0);
	if (currItem->asPathText())
		currItem->UpdatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	ScMW->view->setRedrawBounding(currItem);
	ScMW->view->RefreshItem(currItem);
	//MarkClip(currItem, currItem->PoLine, true);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
		ss->set("MIRROR_PATH_H", "mirror_path_h");
		ss->set("IS_CONTOUR", false);
		undoManager->action(currItem, ss, Um::IBorder);
	}
	emit changed();
}

void ScribusDoc::MirrorPolyV()
{
	PageItem *currItem = selection->itemAt(0);
	currItem->ClipEdited = true;
	QWMatrix ma;
	if (ScMW->view->EditContour)
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
		ScMW->view->updateContents();
		currItem->FrameOnly = true;
		currItem->Tinput = true;
		currItem->paintObj();
		currItem->FrameOnly = false;
		ScMW->view->MarkClip(currItem, currItem->ContourLine, true);
		emit changed();
		return;
	}
	ma.scale(1, -1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(0, currItem->height());
	if (currItem->asPathText())
		currItem->UpdatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	ScMW->view->setRedrawBounding(currItem);
	ScMW->view->RefreshItem(currItem);
	//MarkClip(currItem, currItem->PoLine, true);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
		ss->set("MIRROR_PATH_V", "mirror_path_v");
		ss->set("IS_CONTOUR", false);
		undoManager->action(currItem, ss, Um::IBorder);
	}
	emit changed();
}
