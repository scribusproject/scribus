#ifndef SCRIBUSSTRUCTS_H
#define SCRIBUSSTRUCTS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qvaluestack.h>
#include <qvaluelist.h>
#include <qvaluevector.h> 
#include <qstring.h>
#include <vector>

#include "scfonts.h"
#include "cmykcolor.h"
#include "fpointarray.h"
#include "vgradient.h"
#include "pageitem.h"

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
		    int PtransX;
			int PtransY;
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
				QString guiLanguage;
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
				int startArrowIndex;
				int endArrowIndex;
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

struct arrowDesc {
									QString name;
									bool userArrow;
									FPointArray points;
								 };

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

struct typoStruct {
									int valueSuperScript;
									int scalingSuperScript;
									int valueSubScript;
									int scalingSubScript;
									int valueSmallCaps;
									int autoLineSpacing;
									double valueBaseGrid;
									double offsetBaseGrid;
								  };

struct preV  { 
				SCFonts AvailFonts;
				QValueList<arrowDesc> arrowStyles;
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
				typoStruct typographicSetttings;
				QString Dpen;
				QString Dbrush;
				int Dshade;
				int Dshade2;
				int DLineArt;
				double Dwidth;
				QString DpenLine;
				QString DpenText;
				QString DstrokeText;
				int DshadeLine;
				double DwidthLine;
				int DLstyleLine;
				int DstartArrow;
				int DendArrow;
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
				bool RandFarbig;
				QStringList RecentDocs;
				uint RecentDCount;
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
				bool linkShown;
				bool ShowPic;
				bool ClipMargin;
				bool GCRMode;
				QColor STEcolor;
				QString STEfont;
				double ScratchLeft;
				double ScratchRight;
				double ScratchTop;
				double ScratchBottom;
			};
			
struct PrintOptions {
	QString printer;
	QString filename;
	bool toFile;
	bool useAltPrintCommand;
	QString printerCommand;
	int PSLevel;
	bool outputSeparations;
	QString separationName;
	bool useColor;
	bool mirrorH;
	bool mirrorV;
	bool useICC;
	bool doGCR;
	int copies;
	std::vector<int> pageNumbers;
	QString printerOptions;
};

#endif

