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
#include <config.h>
#endif
// include files for QT
#include <qobject.h>
#include <qdict.h>
#include <qcolor.h>
#include <qvaluelist.h>
#include <qvaluestack.h>
#include <qvaluevector.h>
#include <qptrlist.h>
#include <qfont.h>
#include <qmap.h>
#include <qpointarray.h>
#include <qstringlist.h>
#include <qtimer.h>
#include "cmykcolor.h"
#include "scfonts.h"
#include "fpointarray.h"
#include "hyphenator.h"
#include "vgradient.h"
#ifdef HAVE_CMS
	#include CMS_INC
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

class PageItem;
class Page;

/* Struktur fuer Pageitem Text */
struct Pti { 
			QString ch;
			double xp;
			double yp;
		    int csize;
		    QString cfont;
		    QString ccolor;
			QString cstroke;
		    bool cselect;
		    double cextra;
		    int cshade;
		    int cshade2;
		    int cstyle;
		    int cab;
		    double PtransX;
			double PtransY;
			int cscale;
			double PRot;
		   };

struct CLBuf { 
				int PType;
				double Xpos;
				double Ypos;
				double Width;
				double Height;
				double RadRect;
				int FrameType;
				bool ClipEdited;
				double Pwidth;
				QString Pcolor;
				QString Pcolor2;
				int Shade;
				int Shade2;
				QString GrColor;
				QString GrColor2;
				int GrShade;
				int GrShade2;
				VGradient fill_gradient;
				int GrType;
				double GrStartX;
				double GrStartY;
				double GrEndX;
				double GrEndY;
				QString TxtStroke;
				QString TxtFill;
				int ShTxtStroke;
				int ShTxtFill;
				int TxtScale;
				int TxTStyle;
				double Rot;
				int PLineArt;
				int PLineJoin;
				int PLineEnd;
				double LineSp;
				double ExtraV;
				double LocalScX;
				double LocalScY;
				double LocalX;
				double LocalY;
				bool PicArt;
				int flippedH;
				int flippedV;
				double BBoxX;
				double BBoxH;
				bool isPrintable;
				bool isBookmark;
				int BMnr;
				bool isAnnotation;
				int AnType;
				QString AnAction;
				QString An_E_act;
				QString An_X_act;
				QString An_D_act;
				QString An_Fo_act;
				QString An_Bl_act;
				QString An_K_act;
				QString An_F_act;
				QString An_V_act;
				QString An_C_act;
				QString An_Extern;
				int AnZiel;
				int AnActType;
				QString AnName;
				QString AnToolTip;
				QString AnRollOver;
				QString AnDown;
				QString AnBColor;
				int AnBwid;
				int AnBsty;
				int AnFeed;
				int AnFlag;
				int AnMaxChar;
				int AnVis;
				int AnFont;
				int AnChkStil;
				int AnFormat;
				bool AnIsChk;
				bool AnAAact;
				bool AnHTML;
				bool AnUseIcons;
				int AnIPlace;
				int AnScaleW;
				double Extra;
				double TExtra;
				double BExtra;
				double RExtra;
				QString Pfile;
				QString Pfile2;
				QString Pfile3;
				QString IProfile;
				QString EmProfile;
				int IRender;
				bool UseEmbedded;
				QString Ptext;
				QPointArray Clip;
				FPointArray PoLine;
				FPointArray ContourLine;
				bool PoShow;
				double BaseOffs;
				bool Textflow;
				bool Textflow2;
				bool UseContour;
				int Ausrich;
				QString IFont;
				int ISize;
				QValueStack<int> Groups;
				int LayerNr;
				bool ScaleType;
				bool AspectRatio;
				bool Locked;
				bool LockRes;
				double Transparency;
				double TranspStroke;
				bool Reverse;
				bool InvPict;
				QString NamedLStyle;
				QString Language;
				int Cols;
				double ColGap;
				QValueList<double> TabValues;
				QValueList<double> DashValues;
				double DashOffset;
				bool isTableItem;
				bool TopLine;
				bool LeftLine;
				bool RightLine;
				bool BottomLine;
				int LeftLinkID;
				int RightLinkID;
				int TopLinkID;
				int BottomLinkID;
			};

  /** Seitenraender */
struct Margs { 
				double Top;
           		double Left;
           		double Right;
           		double Bottom;
          	 };

struct StVorL { 
				QString Vname;
				double LineSpa;
				int Ausri;
				double Indent;
				double First;
				double Avor;
				double Anach;
				QString Font;
				int FontSize;
				QValueList<double> TabValues;
				bool Drop;
				int DropLin;
				int FontEffect;
				QString FColor;
				int FShade;
				QString SColor;
				int SShade;
				bool BaseAdj;
			  };

struct CMSset {
  				QString DefaultMonitorProfile;
  				QString DefaultPrinterProfile;
  				QString DefaultInputProfile;
  				QString DefaultInputProfile2;
				int ComponentsInput2;
				int ComponentsPrinter;
  				int DefaultIntentMonitor;
  				int DefaultIntentMonitor2;
  				int DefaultIntentPrinter;
  				bool CMSinUse;
  				bool SoftProofOn;
				bool GamutCheck;
				bool BlackPoint;
				} ;

struct UndoData {
					 PageItem* Item;
  					 uint PageNr;
					 uint ItemNr;
					 double Xpos;
					 double Ypos;
					 double Width;
					 double Height;
					 double Rot;
					 double Gx;
					 double Gy;
					 double Gw;
					 double Gh;
					 int UnCode;
				};

/** Definition der Farbenlisten */
typedef QMap<QString,CMYKColor> CListe;
typedef QMap<QString,QString> ProfilesL;

struct singleLine { 
					double Width;
					int Dash;
 					int LineEnd;
 					int LineJoin;
					QString Color;
					int Shade;
					};
					
typedef QValueVector<singleLine> multiLine;

struct Layer { 
				int LNr;
				int Level;
				QString Name;
				bool Sichtbar;
				bool Drucken;
			};

struct PreSet { 
				int EffektLen;
				int AnzeigeLen;
				int Effekt;
				int Dm;
				int M;
				int Di;
				};

struct LPIset { 
				int Frequency;
				int Angle;
				int SpotFunc;
				};

struct PDFOpt { 
				bool Thumbnails;
				bool Articles;
				bool Compress;
				int CompressMethod;
				int Quality;
				bool RecalcPic;
				bool Bookmarks;
				int PicRes;
				int Version;
				int Resolution;
				int Binding;
				QValueList<QString> EmbedList;
				QValueList<QString> SubsetList;
				bool MirrorH;
				bool MirrorV;
				int RotateDeg;
				bool PresentMode;
				QValueList<PreSet> PresentVals;
				QString Datei;
				bool UseRGB;
				bool UseProfiles;
				bool UseProfiles2;
				bool UseLPI;
				QMap<QString,LPIset> LPISettings;
				QString SolidProf;
				int SComp;
				QString ImageProf;
				bool EmbeddedI;
				int Intent2;
				QString PrintProf;
				QString Info;
				int Intent;
				double BleedTop;
				double BleedLeft;
				double BleedRight;
				double BleedBottom;
				bool Encrypt;
				QString PassOwner;
				QString PassUser;
				int Permissions;
				};

struct Keys { 
				QString Name;
 				int MenuID;
 				int KeyID;
 			};

struct preV  { 
				SCFonts AvailFonts;
				QString DefFont;
				int DefSize;
				CListe DColors;
				int Wheelval;
				int AppFontSize;
				int GrabRad;
				double GuideRad;
				double DminGrid;
				double DmajGrid;
				QColor DminColor;
				QColor DmajColor;
				QColor DpapColor;
				QColor DmargColor;
				QColor guideColor;
				QColor baseColor;
				QString GUI;
				int DVHoch;
				int DVHochSc;
				int DVTief;
				int DVTiefSc;
				int DVKapit;
				QString Dpen;
				QString Dbrush;
				int Dshade;
				int Dshade2;
				int DLineArt;
				double Dwidth;
				QString DpenLine;
				QString DpenText;
				int DshadeLine;
				double DwidthLine;
				int DLstyleLine;
				int MagMin;
				int MagMax;
				int MagStep;
				QString DbrushPict;
				int ShadePict;
				double ScaleX;
				double ScaleY;
				bool Before;
				int Einheit;
				bool Werkv;
				bool WerkvP;
				bool Mpalv;
				int Mpalx;
				int Mpaly;
				bool Mapalv;
				int Mapalx;
				int Mapaly;
				bool Tpalv;
				int Tpalx;
				int Tpaly;
				int Npalx;
				int Npaly;
				int MainX;
				int MainY;
				int MainW;
				int MainH;
				bool SCpalv;
				int SCpalx;
				int SCpaly;
				int SCpalw;
				int SCpalh;
				bool Lpalv;
				int Lpalx;
				int Lpaly;
				bool Sepalv;
				int Sepalx;
				int Sepaly;
				bool Bopalv;
				int Bopalx;
				int Bopaly;
				bool SepalT;
				bool SepalN;
				int PSize;
				bool SaveAtQ;
				bool PagesSbS;
				bool RandFarbig;
				QStringList RecentDocs;
				uint RecentDCount;
				int AutoLine;
				CMSset DCMSset;
    			QMap<int,Keys> KeyActions;
				int PolyC;
				int PolyFd;
				double PolyF;
				bool PolyS;
				double PolyR;
				QString PrinterName;
				QString PrinterFile;
				QString PrinterCommand;
				int PageFormat;
				int Ausrichtung;
				double PageBreite;
				double PageHoehe;
				double RandOben;
				double RandUnten;
				double RandLinks;
				double RandRechts;
				bool DoppelSeiten;
				bool ErsteLinks;
				bool ScaleType;
				bool AspectRatio;
				int MinWordLen;
				int HyCount;
				QString Language;
				QString guiLanguage;
				bool Automatic;
				bool AutoCheck;
				QString DocDir;
				QString ProfileDir;
				QString ScriptDir;
				QString TemplateDir;
				QMap<QString,QString> GFontSub;
				bool PDFTransparency;
				bool AutoSave;
				int AutoSaveTime;
				double DisScale;
				int DCols;
				double DGap;
				QString DColorSet;
				QStringList CustomColorSets;
				bool PrPr_Mode;
				bool Gcr_Mode;
				bool PrPr_AlphaText;
				bool PrPr_AlphaGraphics;
				bool PrPr_Transparency;
				bool PrPr_C;
				bool PrPr_M;
				bool PrPr_Y;
				bool PrPr_K;
				QString gs_exe;
				bool gs_antiText;
				bool gs_antiGraph;
				QString gimp_exe;
				bool FramesShown;
				bool GuidesShown;
				bool MarginsShown;
				bool GridShown;
				bool BaseShown;
				bool ClipMargin;
				bool GCRMode;
				double BaseGrid;
				double BaseOffs;
				QColor STEcolor;
				QString STEfont;
			};
/**
  * the Document Class
  */

class ScribusDoc : public QObject
{

public:
    ScribusDoc();
    ~ScribusDoc();
    void setModified();
    void setUnModified();
    bool isModified() const;
  /** Setzt die Seitenattribute */
	void setPage(double b, double h, double t, double l, double r, double bo, double sp, double ab, bool atf, bool fp);
	void resetPage(double t, double l, double r, double bo, bool fp);
	bool AddFont(QString name, QFont fo);
	void loadStylesFromFile(QString fileName, QValueList<StVorL> *tempStyles = NULL);


protected:
    bool modified;

public: // Public attributes
  /** Vergroesserungsfaktor fuer das Dokument */
	double Scale;
	bool SnapGuides;
	double GuideRad;
	bool GuideLock;
	bool loading;
  /** Breite der Seite  */
	double PageB;
  /** Hoehe der Seite */
	double PageH;
  /** Anzahl der Seiten */
	int PageC;
  /** Seitenraender */
	Margs PageM;
  /** Anzahl der Spalten */
	double PageSp;
  /** Abstand der Spalten */
	double PageSpa;
  /** Flag fuer AutoTextrahmen */
	bool PageAT;
  /** Flag fuer Doppelseiten */
	bool PageFP;
	/** Flag fuer Hoch- oder Querformat 0 = Hochformat */
	int PageOri;
	QString PageSize;
	/** Erste Seitennummer im Dokument */
	int FirstPnum;
  /** Flag fuer Rasterbenutzung */
	bool useRaster;
  /** Flag fuer Bilder */
	bool ShowPic;
	bool FramesShown;
	bool GuidesShown;
	bool MarginsShown;
	bool GridShown;
	bool BaseShown;
  /** Im Dokument benutzte Farben */
	CListe PageColors;
	Page* ActPage;
  /** InfoStrings fuer das aktuelle Dokument */
	QString DocAutor;
	QString DocTitel;
	QString DocComments;
	QString DocKeyWords;
	QString DocPublisher;
	QString DocType;
	QString DocDate;
	QString DocFormat;
	QString DocIdent;
	QString DocSource;
	QString DocLangInfo;
	QString DocRelation;
	QString DocCover;
	QString DocRights;
	QString DocContrib;
	int AppMode;
	int SubMode;
	double *ShapeValues;
	int ValCount;
	QString Dpen;
	QString Dbrush;
	double Dwidth;
	int Dshade;
	int Dshade2;
	PenStyle DLineArt;
	QString DpenLine;
	QString DpenText;
	int DCols;
	double DGap;
	int DshadeLine;
	PenStyle DLstyleLine;
	double DwidthLine;
	QString Dfont;
	QString DocName;
	QMap<QString,QFont> UsedFonts;
	SCFonts *AllFonts;
	int Dsize;
	QColor minorColor;
	QColor majorColor;
	QColor papColor;
	QColor margColor;
	QColor guideColor;
	QColor baseColor;
	double minorGrid;
	double majorGrid;
	int CurrentSel;
	int CurrentStyle;
	int CurrentABStil;
	QString CurrFont;
	int CurrFontSize;
	QString CurrTextFill;
	int CurrTextFillSh;
	QString CurrTextStroke;
	int CurrTextStrokeSh;
	int CurrTextScale;
	bool EditClip;
	int EditClipMode;
  /** um wieviel Prozent wird hochgestellt */
	int VHoch;
  /** Verkleinerung in Prozent bei Hochstellung */
	int VHochSc;
  /** um wieviel Prozent wird tiefgestellt */
	int VTief;
  /** Verkleinerung in Prozent bei Tiefstellung */
	int VTiefSc;
  /** Verkleinerunug in Prozent bei Kapitaelchen */
	int VKapit;
  /** Letztes Element fuer AutoTextrahmen */
	PageItem *LastAuto;
  /** Erstes Element fuer AutoTextrahmen */
	PageItem *FirstAuto;
	int MagMin;
	int MagMax;
	int MagStep;
	int GrabRad;
	QString DbrushPict;
	int ShadePict;
	double ScaleX;
	double ScaleY;
	bool ScaleType;
	bool AspectRatio;
	bool Before;
	int Einheit;
	bool DragP;
	bool leaveDrag;
	PageItem *DraggedElem;
	PageItem *ElemToLink;
	QValueList<uint> DragElements;
	QValueList<StVorL> Vorlagen;
	QValueList<Layer> Layers;
	bool MasterP;
	bool FirstPageLeft;
	bool PagesSbS;
	bool RandFarbig;
	int GroupCounter;
	int AutoLine;
	CMSset CMSSettings;
	int ActiveLayer;
#ifdef HAVE_CMS
	cmsHPROFILE DocInputProf;
	cmsHPROFILE DocOutputProf;
	cmsHPROFILE DocPrinterProf;
#endif
	bool HasCMS;
	QMap<QString,QString> JavaScripts;
	bool UnDoValid;
	UndoData UnData;
	int TotalItems;
	Hyphenator *Trenner;
	int MinWordLen;
	int HyCount;
	QString Language;
	bool Automatic;
	bool AutoCheck;
	bool TemplateMode;
	struct PDFOpt PDF_Optionen;
	bool RePos;
	struct BookMa { 
					QString Title;
					QString Text;
					QString Aktion;
					int Parent;
					int Seite;
					int Element;
					int ItemNr;
					int First;
					int Last;
					int Prev;
					int Next;
					};
	QValueList<BookMa> BookMarks;
	bool OldBM;
	bool hasName;
	int RotMode;
	bool AutoSave;
	int AutoSaveTime;
	QTimer *ASaveTimer;
	FT_Library   library;
	QMap<QString,FT_Face> FFonts;
	QMap<QString,multiLine> MLineStyles;
	QWidget* WinHan;
	bool DoDrawing;
	QValueList<int> OpenNodes;
	QTimer *CurTimer;
	double BaseGrid;
	double BaseOffs;
};

#endif
