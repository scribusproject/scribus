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
#include "page.h"
#include "pageitem.h"
#include "undomanager.h"
#include "undostate.h"
#include <qfile.h>

extern QPixmap loadIcon(QString nam);

ScribusDoc::ScribusDoc(struct ApplicationPrefs *prefsData)
: UndoObject(QObject::tr("Document"))
{
	prefsValues = prefsData;
	modified = false;
	MasterP = false;
	NrItems = 0;
	First = 1;
	Last = 0;
	viewCount = 0;
	viewID = 0;
	UsedFonts.clear();
	FT_Init_FreeType( &library );
	AllFonts = &prefsData->AvailFonts;
	AddFont(prefsData->toolSettings.defFont, prefsData->AvailFonts[prefsData->toolSettings.defFont]->Font);
	toolSettings.defFont = prefsData->toolSettings.defFont;
	toolSettings.defSize = prefsData->toolSettings.defSize;
	guidesSettings.marginsShown = prefsData->guidesSettings.marginsShown;
	guidesSettings.framesShown = prefsData->guidesSettings.framesShown;
	guidesSettings.gridShown = prefsData->guidesSettings.gridShown;
	guidesSettings.guidesShown = prefsData->guidesSettings.guidesShown;
	guidesSettings.baseShown = prefsData->guidesSettings.baseShown;
	guidesSettings.linkShown = prefsData->guidesSettings.linkShown;
	guidesSettings.showPic = prefsData->guidesSettings.showPic;
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
	PageColors.insert("Black", CMYKColor(0, 0, 0, 255));
	PageColors.insert("White", CMYKColor(0, 0, 0, 0));
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
	typographicSetttings.valueSuperScript = prefsData->typographicSetttings.valueSuperScript;
	typographicSetttings.scalingSuperScript = prefsData->typographicSetttings.scalingSuperScript;
	typographicSetttings.valueSubScript = prefsData->typographicSetttings.valueSubScript;
	typographicSetttings.scalingSubScript = prefsData->typographicSetttings.scalingSubScript;
	typographicSetttings.valueSmallCaps = prefsData->typographicSetttings.valueSmallCaps;
	typographicSetttings.autoLineSpacing = prefsData->typographicSetttings.autoLineSpacing;
	typographicSetttings.valueBaseGrid = prefsData->typographicSetttings.valueBaseGrid;
	typographicSetttings.offsetBaseGrid = prefsData->typographicSetttings.offsetBaseGrid;
	toolSettings.dShade = prefsData->toolSettings.dShade;
	toolSettings.dShade2 = prefsData->toolSettings.dShade2;
	toolSettings.shadePict = prefsData->toolSettings.shadePict;
	toolSettings.scaleX = prefsData->toolSettings.scaleX;
	toolSettings.scaleY = prefsData->toolSettings.scaleY;
	toolSettings.scaleType = prefsData->toolSettings.scaleType;
	toolSettings.aspectRatio = prefsData->toolSettings.aspectRatio;
	toolSettings.dCols = prefsData->toolSettings.dCols;
	toolSettings.dGap = prefsData->toolSettings.dGap;
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
	DocTitel = "";
	DocAutor = "";
	DocComments = "";
	DocKeyWords = "";
	DocPublisher = "";
	DocDate = "";
	DocType = "";
	DocFormat = "";
	DocIdent = "";
	DocSource = "";
	DocLangInfo = "";
	DocRelation = "";
	DocCover = "";
	DocRights = "";
	DocContrib = "";
	PageC = 0;
	LastAuto = 0;
	FirstAuto = 0;
	DraggedElem = 0;
	GroupCounter = 1;
	docParagraphStyles.clear();
	struct ParagraphStyle vg;
	vg.Vname = "Normal Internal";
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
	vg.FontEffect = 0;
	vg.FColor = toolSettings.dBrush;
	vg.FShade = toolSettings.dShade;
	vg.SColor = toolSettings.dPen;
	vg.SShade = toolSettings.dShade2;
	vg.BaseAdj = false;
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
	TemplateMode = false;
	PDF_Options.Thumbnails = prefsData->PDF_Options.Thumbnails;
	PDF_Options.Articles = prefsData->PDF_Options.Articles;
	PDF_Options.Compress = prefsData->PDF_Options.Compress;
	PDF_Options.CompressMethod = prefsData->PDF_Options.CompressMethod;
	PDF_Options.Quality = prefsData->PDF_Options.Quality;
	PDF_Options.RecalcPic = prefsData->PDF_Options.RecalcPic;
	PDF_Options.Bookmarks = prefsData->PDF_Options.Bookmarks;
	PDF_Options.PicRes = prefsData->PDF_Options.PicRes;
	PDF_Options.Version = prefsData->PDF_Options.Version;
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
	ASaveTimer = new QTimer(this);
	MLineStyles.clear();
	Pages.clear();
	MasterPages.clear();
	DocPages.clear();
	Items.clear();
	MasterItems.clear();
	DocItems.clear();
	ScratchLeft = prefsData->ScratchLeft;
	ScratchRight = prefsData->ScratchRight;
	ScratchTop = prefsData->ScratchTop;
	ScratchBottom = prefsData->ScratchBottom;
	arrowStyles = prefsData->arrowStyles;
	undoManager = UndoManager::instance();
	docItemErrors.clear();
	masterItemErrors.clear();
}

ScribusDoc::~ScribusDoc()
{
	MasterItems.setAutoDelete(true);
	DocItems.setAutoDelete(true);
	MasterItems.clear();
	DocItems.clear();
	MasterPages.setAutoDelete(true);
	DocPages.setAutoDelete(true);
	MasterPages.clear();
	DocPages.clear();
	QMap<QString,QFont>::Iterator it3;
	for (it3 = UsedFonts.begin(); it3 != UsedFonts.end(); ++it3)
	{
		if ((*AllFonts)[it3.key()]->PrivateFont != "")
			(*AllFonts).removeFont(it3.key());
	}
	FT_Done_FreeType( library );
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
		if (ss->ReadStyles(fileName, this, prefsValues))
		{
			if (ss->docParagraphStyles.count() > old)
			{
				for (uint xx=old; xx<ss->docParagraphStyles.count(); ++xx)
				{
					struct ParagraphStyle sty;
					sty.Vname = ss->docParagraphStyles[xx].Vname;
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
					sty.FontEffect = ss->docParagraphStyles[xx].FontEffect;
					sty.FColor = ss->docParagraphStyles[xx].FColor;
					sty.FShade = ss->docParagraphStyles[xx].FShade;
					sty.SColor = ss->docParagraphStyles[xx].SColor;
					sty.SShade = ss->docParagraphStyles[xx].SShade;
					sty.BaseAdj = ss->docParagraphStyles[xx].BaseAdj;
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

void ScribusDoc::setModified()
{
	modified = true;
}

void ScribusDoc::setUnModified()
{
	modified = false;
}

bool ScribusDoc::isModified() const
{
  return modified;
}
/** Setzt die Seitenattribute */
void ScribusDoc::setPage(double b, double h, double t, double l, double r, double bo, double sp, double ab, bool atf, bool fp)
{
	PageB = b;
	PageH = h;
	PageM.Top = t;
	PageM.Left = l;
	PageM.Right = r;
	PageM.Bottom = bo;
	PageSp = sp;
	PageSpa = ab;
	PageFP = fp;
	PageAT = atf;
}

void ScribusDoc::resetPage(double t, double l, double r, double bo, bool fp)
{
	PageM.Top = t;
	PageM.Left = l;
	PageM.Right = r;
	PageM.Bottom = bo;
	PageFP = fp;
}

bool ScribusDoc::AddFont(QString name, QFont fo)
{
	bool ret = false;
	bool error;
	FT_Face      face;
	if (UsedFonts.contains(name))
		return true;
	error = FT_New_Face( library, (*AllFonts)[name]->Datei, 0, &face );
	if (error)
		return ret;
	if ((*AllFonts)[name]->ReadMetrics())
	{
		(*AllFonts)[name]->CharWidth[13] = 0;
		(*AllFonts)[name]->CharWidth[28] = 0;
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

