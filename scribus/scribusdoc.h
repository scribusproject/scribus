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
#ifdef HAVE_CMS
	#include CMS_INC
#endif

#ifdef HAVE_FREETYPE
	#include <ft2build.h>
	#include FT_FREETYPE_H
#endif

class PageItem;
class Page;

/* Struktur fuer Pageitem Text */
struct Pti { QString ch;
				     float xp;
				     float yp;
				     int csize;
				     QString cfont;
				     QString ccolor;
				     bool cselect;
				     float cextra;
				     int cshade;
				     int cstyle;
				     int cab;
						 int PtransX;
						 int PtransY;
						 float PRot;
				    };

struct CLBuf { int PType;
							 float Xpos;
							 float Ypos;
							 float Width;
							 float Height;
							 float RadRect;
							 int FrameType;
							 bool ClipEdited;
							 float Pwidth;
							 QString Pcolor;
							 QString Pcolor2;
							 int Shade;
							 int Shade2;
							 QString GrColor;
							 QString GrColor2;
							 int GrShade;
							 int GrShade2;
							 int GrType;
							 float Rot;
							 int PLineArt;
							 int PLineJoin;
							 int PLineEnd;
							 float LineSp;
							 float ExtraV;
							 float LocalScX;
							 float LocalScY;
							 float LocalX;
							 float LocalY;
							 bool PicArt;
							 int flippedH;
							 int flippedV;
							 float BBoxX;
							 float BBoxH;
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
							 float Extra;
							 float TExtra;
							 float BExtra;
							 float RExtra;
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
							 bool PoShow;
							 float BaseOffs;
							 bool Textflow;
							 bool Textflow2;
							 int Ausrich;
							 QString IFont;
							 int ISize;
							 QValueStack<int> Groups;
							 int LayerNr;
							 bool ScaleType;
							 bool AspectRatio;
							 bool Locked;
							 float Transparency;
							 bool Reverse;
							 bool InvPict;
							 QString NamedLStyle;
							};

  /** Seitenraender */
struct Margs { float Top;
           		 float Left;
           		 float Right;
           		 float Bottom;
          		};

struct StVorL { QString Vname;
								float LineSpa;
								int Ausri;
								float Indent;
								float First;
								float Avor;
								float Anach;
								QString Font;
								int FontSize;
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
							} ;

struct UndoData {
								 PageItem* Item;
  							 uint PageNr;
								 uint ItemNr;
								 float Xpos;
								 float Ypos;
								 float Width;
								 float Height;
								 float Rot;
								 float Gx;
								 float Gy;
								 float Gw;
								 float Gh;
								 int UnCode;
								};

/** Definition der Farbenlisten */
typedef QMap<QString,CMYKColor> CListe;
typedef QMap<QString,QString> ProfilesL;

struct singleLine { float Width;
										int Dash;
 										int LineEnd;
 										int LineJoin;
										QString Color;
										int Shade;
									};
typedef QValueVector<singleLine> multiLine;

struct Layer { int LNr;
							 int Level;
							 QString Name;
							 bool Sichtbar;
							 bool Drucken;
						 };

struct PreSet { int EffektLen;
								int AnzeigeLen;
								int Effekt;
								int Dm;
								int M;
								int Di;
							};
							
struct PDFOpt { bool Thumbnails;
								bool Articles;
								bool Compress;
								bool RecalcPic;
								bool Bookmarks;
								int PicRes;
								int Version;
								int Resolution;
								int Binding;
								QValueList<QString> EmbedList;
								bool PresentMode;
								QValueList<PreSet> PresentVals;
								QString Datei;
								bool UseRGB;
								bool UseProfiles;
								bool UseProfiles2;
								QString SolidProf;
								int SComp;
								QString ImageProf;
								bool EmbeddedI;
								int Intent2;
								QString PrintProf;
								QString Info;
								int Intent;
								float BleedTop;
								float BleedLeft;
								float BleedRight;
								float BleedBottom;
								bool Encrypt;
								QString PassOwner;
								QString PassUser;
								int Permissions;
							};
							
struct Keys { QString Name;
 							int MenuID;
 							int KeyID;
 						};
							
struct preV  { SCFonts AvailFonts;
							 QString DefFont;
							 int DefSize;
							 CListe DColors;
							 int Wheelval;
							 int AppFontSize;
							 int GrabRad;
							 float GuideRad;
							 float DminGrid;
							 float DmajGrid;
							 QColor DminColor;
							 QColor DmajColor;
							 QColor DpapColor;
							 QColor DmargColor;
							 QColor guideColor;
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
							 float Dwidth;
							 QString DpenLine;
							 QString DpenText;
							 int DshadeLine;
							 float DwidthLine;
							 int DLstyleLine;
							 int MagMin;
							 int MagMax;
							 int MagStep;
							 QString DbrushPict;
							 int ShadePict;
							 float ScaleX;
							 float ScaleY;
							 bool Before;
							 int Einheit;
							 bool Werkv;
							 bool WerkvP;
							 bool Mpalv;
							 int Mpalx;
							 int Mpaly;
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
							 bool ShFrames;
							 bool RandFarbig;
							 QStringList RecentDocs;
							 uint RecentDCount;
							 int AutoLine;
							 CMSset DCMSset;
    					 QMap<int,Keys> KeyActions;
							 int PolyC;
							 int PolyFd;
							 float PolyF;
							 bool PolyS;
							 float PolyR;
							 QString PrinterName;
							 QString PrinterFile;
							 QString PrinterCommand;
							 int PageFormat;
							 int Ausrichtung;
							 float PageBreite;
							 float PageHoehe;
							 float RandOben;
							 float RandUnten;
							 float RandLinks;
							 float RandRechts;
							 bool DoppelSeiten;
							 bool ErsteLinks;
							 bool ScaleType;
							 bool AspectRatio;
							 int MinWordLen;
							 QString Language;
							 bool Automatic;
							 bool AutoCheck;
							 QString DocDir;
							 QMap<QString,QString> GFontSub;
							 bool PDFTransparency;
							 bool AutoSave;
							 int AutoSaveTime;
							 float DisScale;
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
  void setPage(float b, float h, float t, float l, float r, float bo, float sp, float ab, bool atf, bool fp);
  void resetPage(float t, float l, float r, float bo, bool fp);
  void AddFont(QString name, QFont fo);
  protected:
    bool modified;
	public: // Public attributes
  /** Vergroesserungsfaktor fuer das Dokument */
  float Scale;
  bool Marks;
	bool Guides;
	bool SnapGuides;
	float GuideRad;
	bool GuideLock;
  bool loading;
  /** Breite der Seite  */
  float PageB;
  /** Hoehe der Seite */
  float PageH;
  /** Anzahl der Seiten */
  int PageC;
  /** Seitenraender */
  Margs PageM;
  /** Anzahl der Spalten */
  float PageSp;
  /** Abstand der Spalten */
  float PageSpa;
  /** Flag fuer AutoTextrahmen */
  bool PageAT;
  /** Flag fuer Doppelseiten */
  bool PageFP;
	/** Flag fuer Hoch- oder Querformat 0 = Hochformat */
	int PageOri;
	/** Erste Seitennummer im Dokument */
	int FirstPnum;
  /** Flag fuer Raster */
  bool Raster;
  /** Flag fuer Rasterbenutzung */
  bool useRaster;
  /** Flag fuer Bilder */
  bool ShowPic;
  /** Im Dokument benutzte Farben */
  CListe PageColors;
  Page* ActPage;
  /** InfoStrings fuer das aktuelle Dokument */
  QString DocAutor;
  QString DocTitel;
  QString DocComments;
  int AppMode;
  QString Dpen;
  QString Dbrush;
  float Dwidth;
  int Dshade;
  int Dshade2;
  PenStyle DLineArt;
  QString DpenLine;
  QString DpenText;
  int DshadeLine;
  PenStyle DLstyleLine;
  float DwidthLine;
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
  float minorGrid;
  float majorGrid;
  int CurrentSel;
  int CurrentStyle;
  int CurrentABStil;
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
	float ScaleX;
	float ScaleY;
	bool ScaleType;
	bool AspectRatio;
	bool Before;
	int Einheit;
  bool DragP;
  PageItem *DraggedElem;
  PageItem *ElemToLink;
  QValueList<uint> DragElements;
  QValueList<StVorL> Vorlagen;
	QValueList<Layer> Layers;
  bool MasterP;
  bool FirstPageLeft;
  bool PagesSbS;
  bool ShFrames;
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
	QString Language;
	bool Automatic;
	bool AutoCheck;
  bool TemplateMode;
	struct PDFOpt PDF_Optionen;
	bool RePos;
	struct BookMa { QString Title;
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
	QTimer *ASaveTimer;
#ifdef HAVE_FREETYPE
	FT_Library   library;
  QMap<QString,FT_Face> FFonts;
#endif
	QMap<QString,multiLine> MLineStyles;
	QWidget* WinHan;
};

#endif
