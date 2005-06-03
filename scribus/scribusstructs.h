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
#include "pdfoptions.h"

/* Struktur fuer Pageitem Text */
struct ScText
{
	bool cselect;
	int csize;
	short cshade;
	short cshade2;
	short cstyle;
/*	Meaning of the values in cstyle
	1 		= Superscript
	2 		= Subscript
	4 		= Outline
	8 		= Underline
	16 		= Strikethru
	32 		= All Caps
	64 		= Small Caps
	128 	= Hyphenation possible here (Smart Hyphen)
	256 	= Shadowed
	512 	= Underline Words
	1024 	= free, not used in the moment
	2048 	= free, not used in the moment
	4096 	= internal use in PageItem (Suppresses spaces when in Block alignment)
	8192 	= Smart Hyphen visible at line end
	16384	= Start of Line
*/
	short cab;
	short cscale;
	short cscalev;
	short cbase;
	short cshadowx;
	short cshadowy;
	short coutline;
	short cunderpos;
	short cunderwidth;
	short cstrikepos;
	short cstrikewidth;
	float cextra;
	float xp;
	float yp;
	float PtransX;
	float PtransY;
	float PRot;
	Foi* cfont;
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
	int TxtScaleV;
	int TxTStyle;
	int TxTBase;
	int TxtShadowX;
	int TxtShadowY;
	int TxtOutline;
	int TxtUnderPos;
	int TxtUnderWidth;
	int TxtStrikePos;
	int TxtStrikeWidth;
	double Rot;
	int PLineArt;
	int PLineJoin;
	int PLineEnd;
	double LineSp;
	int LineSpMode;
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
	QString NamedLStyle;
	QString Language;
	QString guiLanguage;
	int Cols;
	double ColGap;
	QValueList<PageItem::TabRecord> TabValues;
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
	int LineSpaMode;      // 0 = fixed; 1 = Automatic
	double LineSpa;
	int textAlignment;
	double Indent;
	double First;
	double gapBefore;
	double gapAfter;
	QString Font;
	int FontSize;
	QValueList<PageItem::TabRecord> TabValues;
	bool Drop;
	int DropLin;
	int FontEffect;
	QString FColor;
	int FShade;
	QString SColor;
	int SShade;
	bool BaseAdj;
	int txtShadowX;
	int txtShadowY;
	int txtOutline;
	int txtUnderPos;
	int txtUnderWidth;
	int txtStrikePos;
	int txtStrikeWidth;
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
	int valueUnderlinePos;
	int valueUnderlineWidth;
	int valueStrikeThruPos;
	int valueStrikeThruWidth;
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
	bool showControls;
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
	QString tabFillChar;
	double dTabWidth;
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
	int lowResType;
	bool useEmbeddedPath;
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
	QString documentTemplatesDir;
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
	QString imageEditorExecutable;
	bool ClipMargin;
	bool GCRMode;
	QColor STEcolor;
	QString STEfont;
	double ScratchLeft;
	double ScratchRight;
	double ScratchTop;
	double ScratchBottom;
	bool askBeforeSubstituite;
	bool haveStylePreview;
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
	bool setDevParam;
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
	EditGradientVectors,
	EyeDropper,
	CopyProperties
};

struct AlignObjs
{
	int ObjNr;
	int Group;
	double x1;
	double y1;
	double x2;
	double y2;
	QPtrList<PageItem> Objects;
};

#endif
