#ifndef SCRIBUSSTRUCTS_H
#define SCRIBUSSTRUCTS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qvaluestack.h>
#include <qvaluelist.h>
#include <qvaluevector.h>
#include <qstring.h>
#include <qaction.h>
#include <vector>

#include "scfonts.h"
#include "cmykcolor.h"
#include "fpointarray.h"
#include "vgradient.h"
#include "pageitem.h"
#include "scraction.h"

/* Struktur fuer Pageitem Text */
struct ScText
{
	bool cselect;
	int csize;
	short cshade;
	short cshade2;
	short cstyle;
	short cab;
	short cscale;
	float cextra;
	float xp;
	float yp;
	float PtransX;
	float PtransY;
	float PRot;
	QString cfont;
	QString ccolor;
	QString cstroke;
	QString ch;
};

struct CopyPasteBuffer
{
	PageItem::ItemType PType;
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
	bool flippedH;
	bool flippedV;
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
	QString itemText;
	QPointArray Clip;
	FPointArray PoLine;
	FPointArray ContourLine;
	bool PoShow;
	double BaseOffs;
	bool Textflow;
	bool Textflow2;
	bool UseContour;
	int textAlignment;
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
	ObjAttrVector pageItemAttributes;
};

  /** Pagemargins */
struct MarginStruct
{
	double Top;
	double Left;
	double Right;
	double Bottom;
};

struct ParagraphStyle
{
	QString Vname;
	double LineSpa;
	int textAlignment;
	double Indent;
	double First;
	double gapBefore;
	double gapAfter;
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

struct CMSData
{
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

struct SingleLine
{
	double Width;
	int Dash;
	int LineEnd;
	int LineJoin;
	QString Color;
	int Shade;
};

struct ArrowDesc
{
	QString name;
	bool userArrow;
	FPointArray points;
};

struct Layer
{
	int LNr;
	int Level;
	QString Name;
	bool isPrintable;
	bool isViewable;
};

struct PDFPresentationData
{
	int pageEffectDuration;
	int pageViewDuration;
	int effectType;
	int Dm;
	int M;
	int Di;
};

struct LPIData
{
	int Frequency;
	int Angle;
	int SpotFunc;
};

struct PDFOptions
{
	bool Thumbnails;
	bool Articles;
	bool useLayers;
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
	QValueList<PDFPresentationData> PresentVals;
	QString Datei;
	bool isGrayscale;
	bool UseRGB;
	bool UseProfiles;
	bool UseProfiles2;
	bool UseLPI;
	QMap<QString,LPIData> LPISettings;
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

struct Keys
{
	QString actionName;
	QString cleanMenuText;
	QString keySequence;
	int tableRow;
};

/** Definition der Farbenlisten */
typedef QMap<QString,CMYKColor> ColorList;

struct checkerPrefs
{
	bool ignoreErrors;
	bool autoCheck;
	bool checkGlyphs;
	bool checkOverflow;
	bool checkOrphans;
	bool checkPictures;
	bool checkResolution;
	double minResolution;
	bool checkTransparency;
	bool checkAnnotations;
	bool checkRasterPDF;
};

struct typoPrefs
{
	int valueSuperScript;
	int scalingSuperScript;
	int valueSubScript;
	int scalingSubScript;
	int valueSmallCaps;
	int autoLineSpacing;
	double valueBaseGrid;
	double offsetBaseGrid;
};

struct windowPrefs
{
	int xPosition;
	int yPosition;
	int width;
	int height;
	bool visible;
	bool docked;
};

struct guidesPrefs
{
	bool gridShown;
	QColor majorColor;
	double majorGrid;
	double minorGrid;
	QColor minorColor;
	double guideRad;
	int grabRad;
	bool framesShown;
	bool guidesShown;
	bool marginsShown;
	bool baseShown;
	bool linkShown;
	bool showPic;
	bool before;
	QColor guideColor;
	QColor baseColor;
	QColor margColor;
};

struct toolPrefs
{
	/* Texttool */
	QString defFont;
	int defSize;
	QString dPenText;
	QString dStrokeText;
	int dCols;
	double dGap;
	/* ShapeTool */
	QString dPen;
	QString dBrush;
	int dShade;
	int dShade2;
	int dLineArt;
	double dWidth;
	/* Line Tool */
	QString dPenLine;
	int dShadeLine;
	double dWidthLine;
	int dLstyleLine;
	int dStartArrow;
	int dEndArrow;
	/* Regular Polygon Tool */
	int polyC;
	int polyFd;
	double polyF;
	bool polyS;
	double polyR;
	/* Picture Tool */
	QString dBrushPict;
	int shadePict;
	double scaleX;
	double scaleY;
	bool scaleType;
	bool aspectRatio;
	/* Magnifier Tool */
	int magMin;
	int magMax;
	int magStep;
};

struct ApplicationPrefs
{
	SCFonts AvailFonts;
	QValueList<ArrowDesc> arrowStyles;
	ColorList DColors;
	int Wheelval;
	int AppFontSize;
	QColor DpapColor;
	QString GUI;
	guidesPrefs guidesSettings;
	typoPrefs typographicSetttings;
	toolPrefs toolSettings;
	QMap<QString, checkerPrefs> checkerProfiles;
	QString curCheckProfile;
	int docUnitIndex;
	windowPrefs mainToolBarSettings;
	windowPrefs pdfToolBarSettings;
	windowPrefs mainWinSettings;
	
	PDFOptions PDF_Options;
	ObjAttrVector defaultItemAttributes;
	ToCSetupVector defaultToCSetups;
	bool SepalT;
	bool SepalN;
	int PSize;
	bool SaveAtQ;
	bool marginColored;
	QStringList RecentDocs;
	uint RecentDCount;
	CMSData DCMSset;
	QMap<QString,Keys> KeyActions;
	QString PrinterName;
	QString PrinterFile;
	QString PrinterCommand;
	QString pageSize;
	int pageOrientation;
	double PageWidth;
	double PageHeight;
	double RandOben;
	double RandUnten;
	double RandLinks;
	double RandRechts;
	bool FacingPages;
	bool LeftPageFirst;
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
	bool gs_AntiAliasText;
	bool gs_AntiAliasGraphics;
	QString gimp_exe;
	bool ClipMargin;
	bool GCRMode;
	QColor STEcolor;
	QString STEfont;
	double ScratchLeft;
	double ScratchRight;
	double ScratchTop;
	double ScratchBottom;
	bool askBeforeSubstituite;
};

struct PrintOptions
{
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

typedef QMap<QString,QString> ProfilesL;
typedef QValueVector<SingleLine> multiLine;
typedef QMap<int, int> errorCodes;

enum AppMode
{
	NormalMode,
	DrawShapes,
	StartStoryEditor,
	DrawPicture,
	DrawText,
	Magnifier,
	EditMode,
	DrawLine,
	Rotation,
	LinkFrames,
	UnlinkFrames,
	DrawRegularPolygon,
	DrawBezierLine,
	InsertPDFButton,
	InsertPDFTextfield,
	InsertPDFCheckbox,
	InsertPDFCombobox,
	InsertPDFListbox,
	InsertPDFTextAnnotation,
	InsertPDFLinkAnnotation,
	DrawFreehandLine,
	DrawTable,
	PanningMode,
	MeasurementTool,
	EditGradientVectors
};



#endif
