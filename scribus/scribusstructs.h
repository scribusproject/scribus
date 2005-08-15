#ifndef SCRIBUSSTRUCTS_H
#define SCRIBUSSTRUCTS_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include <qvaluestack.h>
#include <qvaluelist.h>
#include <qvaluevector.h>
#include <qstring.h>
#include <qaction.h>
#include <vector>

#include "scfonts.h"
#include "sccolor.h"
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
/**	Meaning of the values in cstyle
	1			= Superscript
	2 			= Subscript
	4 			= Outline
	8 			= Underline
	16 		= Strikethru
	32 		= All Caps
	64 		= Small Caps
	128 		= Hyphenation possible here (Smart Hyphen)
	256 	= Shadowed
	512 	= Underline Words
	1024 	= free, not used in the moment
	2048 	= Char is a DropCap
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
	short cextra;
	float xp;
	float yp;
	float PtransX;
	float PtransY;
	float PRot;
	Foi* cfont;
	PageItem* cembedded;
	QString ccolor;
	QString cstroke;
	QString ch;
 /** Special Characters used:
 	QChar(24) =	Non breaking Hyphen
 	QChar(25) =	Marks an inline Object
 	QChar(26) =	Column Break
 	QChar(27) =	Frame Break
 	QChar(28) =	New Line, doesn't break Paragraph
 	QChar(29) =	Non breaking Space
 	QChar(30) =	Automatic Pagenumbering  */
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
	int ExtraV;
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

struct PageSet
{
	QString Name;
	int FirstPage;
	int Rows;
	int Columns;
	double GapHorizontal;
	double GapVertical;
	double GapBelow;
};

struct ParagraphStyle
{
	QString Vname;
	int LineSpaMode;      // 0 = fixed; 1 = Automatic; 2 = Adjust to Baseline Grid
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
	double DropDist;
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
	int scaleH;
	int scaleV;
	int baseOff;
	int kernVal;
};

struct CMSData
{
	QString DefaultMonitorProfile;
	QString DefaultPrinterProfile;
	QString DefaultImageRGBProfile;
	QString DefaultImageCMYKProfile;
	QString DefaultSolidColorProfile;
	int ComponentsInput2;
	int ComponentsPrinter;
	int DefaultIntentMonitor;
	int DefaultIntentImages;
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
typedef QMap<QString,ScColor> ColorList;



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

enum PageLayout
{
	singlePage,
	doublePage,
	triplePage,
	quadroPage
};

enum AppMode
{
	modeNormal,
	modeDrawShapes,
	modeStoryEditor,
	modeDrawPicture,
	modeDrawText,
	modeMagnifier,
	modeEdit,
	modeDrawLine,
	modeRotation,
	modeLinkFrames,
	modeUnlinkFrames,
	modeDrawRegularPolygon,
	modeDrawBezierLine,
	modeInsertPDFButton,
	modeInsertPDFTextfield,
	modeInsertPDFCheckbox,
	modeInsertPDFCombobox,
	modeInsertPDFListbox,
	modeInsertPDFTextAnnotation,
	modeInsertPDFLinkAnnotation,
	modeDrawFreehandLine,
	modeDrawTable,
	modePanning,
	modeMeasurementTool,
	modeEditGradientVectors,
	modeEyeDropper,
	modeCopyProperties
};

struct AlignObjs
{
	int ObjNr;
	int Group;
	double x1;
	double y1;
	double x2;
	double y2;
	double width;
	double height;
	QPtrList<PageItem> Objects;
};

#endif
