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
#include <qfile.h> 
/*
 * We need to be able to see ScApp so we can get the
 * Prefs struct.
 */
// Is this the right place for this declaration?
extern ScribusApp *ScApp;

ScribusDoc::ScribusDoc()
{
	modified = false;
	MasterP = false;
	GuideLock = false;
	SnapGuides = false;
	ShowPic = true;
	useRaster = false;
	EditClip = false;
	EditClipMode = 0;
	loading = false;
	minorGrid = 20;
	majorGrid = 100;
	minorColor = QColor(green);
	majorColor = QColor(green);
	guideColor = QColor(darkBlue);
	GuideRad = 10;
	Dpen = "Black";
	Dshade2 = 100;
	Dbrush = "Black";
	Dshade = 100;
	Dwidth = 1;
	DLineArt = SolidLine;
	DocName = QObject::tr("Document")+"-";
	UsedFonts.clear();
	Dfont = "";
	Dsize = 120;
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
	Vorlagen.clear();
	struct StVorL vg;
	vg.Vname = "Normal Internal";
	vg.LineSpa = 15;
	vg.Ausri = 0;
	vg.Indent = 0;
	vg.First = 0;
	vg.Avor = 0;
	vg.Anach = 0;
	vg.Font = "";
	vg.FontSize = Dsize;
	vg.TabValues.clear();
	vg.Drop = false;
	vg.DropLin = 2;
	vg.FontEffect = 0;
	vg.FColor = Dbrush;
	vg.FShade = Dshade;
	vg.SColor = Dpen;
	vg.SShade = Dshade2;
	vg.BaseAdj = false;
	Vorlagen.append(vg);
	vg.Vname = "Center Internal";
	vg.Ausri = 1;
	Vorlagen.append(vg);
	vg.Vname = "Rechts Internal";
	vg.Ausri = 2;
	Vorlagen.append(vg);
	vg.Vname = "Block Internal";
	vg.Ausri = 3;
	Vorlagen.append(vg);
	vg.Vname = "EBlock Internal";
	vg.Ausri = 4;
	Vorlagen.append(vg);
	Layers.clear();
	struct Layer ll;
	ll.LNr = 0;
	ll.Level = 0;
	ll.Name = QObject::tr("Background");
	ll.Sichtbar = true;
	ll.Drucken = true;
	Layers.append(ll);
	ActiveLayer = 0;
	JavaScripts.clear();
	UnDoValid = false;
	UnData.UnCode = -1;
	CurrentStyle = 0;
	CurrentABStil = 0;
	TotalItems = 0;
	TemplateMode = false;
	PDF_Optionen.Thumbnails = false;
	PDF_Optionen.Articles = false;
	PDF_Optionen.Compress = true;
	PDF_Optionen.CompressMethod = 0;
	PDF_Optionen.Quality = 0;
	PDF_Optionen.RecalcPic = false;
	PDF_Optionen.Bookmarks = false;
	PDF_Optionen.PicRes = 300;
	PDF_Optionen.Version = 14;
	PDF_Optionen.Resolution = 300;
	PDF_Optionen.Binding = 0;
	PDF_Optionen.EmbedList.clear();
	PDF_Optionen.SubsetList.clear();
	PDF_Optionen.MirrorH = false;
	PDF_Optionen.MirrorV = false;
	PDF_Optionen.RotateDeg = 0;
	PDF_Optionen.PresentMode = false;
	PDF_Optionen.Datei = "";
	PDF_Optionen.PresentVals.clear();
	PDF_Optionen.UseRGB = true;
	PDF_Optionen.UseProfiles = false;
	PDF_Optionen.UseProfiles2 = false;
	PDF_Optionen.SolidProf = "";
	PDF_Optionen.SComp = 3;
	PDF_Optionen.ImageProf = "";
	PDF_Optionen.PrintProf = "";
	PDF_Optionen.Info = "";
	PDF_Optionen.Intent = 0;
	PDF_Optionen.Intent2 = 0;
	PDF_Optionen.BleedTop = 0;
	PDF_Optionen.BleedLeft = 0;
	PDF_Optionen.BleedRight = 0;
	PDF_Optionen.BleedBottom = 0;
	PDF_Optionen.EmbeddedI = false;
	PDF_Optionen.Encrypt = false;
	PDF_Optionen.PassOwner = "";
	PDF_Optionen.PassUser = "";
	PDF_Optionen.Permissions = -4;
	PDF_Optionen.UseLPI = false;
	PDF_Optionen.LPISettings.clear();
	RePos = false;
	BookMarks.clear();
	OldBM = false;
	hasName = false;
	DoDrawing = true;
	RotMode = 0;
	SubMode = -1;
	ASaveTimer = new QTimer(this);
	MLineStyles.clear();
	FT_Init_FreeType( &library );
	Pages.clear();
	MasterPages.clear();
	DocPages.clear();
	Items.clear();
	MasterItems.clear();
	DocItems.clear();
	ScratchLeft = 100;
	ScratchRight = 100;
	ScratchTop = 20;
	ScratchBottom = 20;
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
void ScribusDoc::loadStylesFromFile(QString fileName, QValueList<StVorL> *tempStyles)
{
	// This won't create the QValueList<StVorL> will it?
	QValueList<StVorL> *wrkStyles = NULL;
	/*
	 * Use the working styles struct if passed, or work directly
	 * on the document styles otherwise. Note that tempStyles,
	 * if passed, MUST have the first five styles initialised already
	 * or this function will segfault.
	 */
	if (tempStyles != NULL)
		wrkStyles = tempStyles;
	 else
		wrkStyles = &Vorlagen;
	if (!fileName.isEmpty())
	{
		ScriXmlDoc *ss = new ScriXmlDoc();
		ss->Vorlagen.clear();
		for (uint x = 5; x < wrkStyles->count(); ++x)
			ss->Vorlagen.append((*wrkStyles)[x]);
		uint old = wrkStyles->count()-5;
		if (ss->ReadStyles(fileName, this, &ScApp->Prefs))
		{
			if (ss->Vorlagen.count() > old)
			{
				for (uint xx=old; xx<ss->Vorlagen.count(); ++xx)
				{
					struct StVorL sty;
					sty.Vname = ss->Vorlagen[xx].Vname;
					sty.LineSpa = ss->Vorlagen[xx].LineSpa;
					sty.Ausri = ss->Vorlagen[xx].Ausri;
					sty.Indent = ss->Vorlagen[xx].Indent;
					sty.First = ss->Vorlagen[xx].First;
					sty.Avor = ss->Vorlagen[xx].Avor;
					sty.Anach = ss->Vorlagen[xx].Anach;
					sty.Font = ss->Vorlagen[xx].Font;
					sty.FontSize = ss->Vorlagen[xx].FontSize;
					sty.TabValues = ss->Vorlagen[xx].TabValues;
					sty.Drop = ss->Vorlagen[xx].Drop;
					sty.DropLin = ss->Vorlagen[xx].DropLin;
					sty.FontEffect = ss->Vorlagen[xx].FontEffect;
					sty.FColor = ss->Vorlagen[xx].FColor;
					sty.FShade = ss->Vorlagen[xx].FShade;
					sty.SColor = ss->Vorlagen[xx].SColor;
					sty.SShade = ss->Vorlagen[xx].SShade;
					sty.BaseAdj = ss->Vorlagen[xx].BaseAdj;
					wrkStyles->append(sty);
				}
			}
		}
		delete ss;
	}
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
