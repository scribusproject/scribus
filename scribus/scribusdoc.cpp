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
	PDF_Optionen.Thumbnails = prefsData->PDF_Optionen.Thumbnails;
	PDF_Optionen.Articles = prefsData->PDF_Optionen.Articles;
	PDF_Optionen.Compress = prefsData->PDF_Optionen.Compress;
	PDF_Optionen.CompressMethod = prefsData->PDF_Optionen.CompressMethod;
	PDF_Optionen.Quality = prefsData->PDF_Optionen.Quality;
	PDF_Optionen.RecalcPic = prefsData->PDF_Optionen.RecalcPic;
	PDF_Optionen.Bookmarks = prefsData->PDF_Optionen.Bookmarks;
	PDF_Optionen.PicRes = prefsData->PDF_Optionen.PicRes;
	PDF_Optionen.Version = prefsData->PDF_Optionen.Version;
	PDF_Optionen.Resolution = prefsData->PDF_Optionen.Resolution;
	PDF_Optionen.Binding = prefsData->PDF_Optionen.Binding;
	PDF_Optionen.EmbedList = prefsData->PDF_Optionen.EmbedList;
	PDF_Optionen.SubsetList = prefsData->PDF_Optionen.SubsetList;
	PDF_Optionen.MirrorH = prefsData->PDF_Optionen.MirrorH;
	PDF_Optionen.MirrorV = prefsData->PDF_Optionen.MirrorV;
	PDF_Optionen.RotateDeg = prefsData->PDF_Optionen.RotateDeg;
	PDF_Optionen.PresentMode = prefsData->PDF_Optionen.PresentMode;
	PDF_Optionen.Datei = prefsData->PDF_Optionen.Datei;
	PDF_Optionen.PresentVals = prefsData->PDF_Optionen.PresentVals;
	PDF_Optionen.UseRGB = prefsData->PDF_Optionen.UseRGB;
	PDF_Optionen.UseProfiles = prefsData->PDF_Optionen.UseProfiles;
	PDF_Optionen.UseProfiles2 = prefsData->PDF_Optionen.UseProfiles2;
	PDF_Optionen.SolidProf = prefsData->PDF_Optionen.SolidProf;
	PDF_Optionen.SComp = prefsData->PDF_Optionen.SComp;
	PDF_Optionen.ImageProf = prefsData->PDF_Optionen.ImageProf;
	PDF_Optionen.PrintProf = prefsData->PDF_Optionen.PrintProf;
	PDF_Optionen.Info = prefsData->PDF_Optionen.Info;
	PDF_Optionen.Intent = prefsData->PDF_Optionen.Intent;
	PDF_Optionen.Intent2 = prefsData->PDF_Optionen.Intent2;
	PDF_Optionen.BleedTop = prefsData->PDF_Optionen.BleedTop;
	PDF_Optionen.BleedLeft = prefsData->PDF_Optionen.BleedLeft;
	PDF_Optionen.BleedRight = prefsData->PDF_Optionen.BleedRight;
	PDF_Optionen.BleedBottom = prefsData->PDF_Optionen.BleedBottom;
	PDF_Optionen.EmbeddedI = prefsData->PDF_Optionen.EmbeddedI;
	PDF_Optionen.Encrypt = prefsData->PDF_Optionen.Encrypt;
	PDF_Optionen.PassOwner = prefsData->PDF_Optionen.PassOwner;
	PDF_Optionen.PassUser = prefsData->PDF_Optionen.PassUser;
	PDF_Optionen.Permissions = prefsData->PDF_Optionen.Permissions;
	PDF_Optionen.UseLPI = prefsData->PDF_Optionen.UseLPI;
	PDF_Optionen.LPISettings = prefsData->PDF_Optionen.LPISettings;
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
