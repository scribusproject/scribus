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
#include <qkeysequence.h>
#include <vector>

#include "sctextstruct.h"
#include "scfonts.h"
#include "fpointarray.h"
#include "vgradient.h"
#include "annotation.h"
#include "pageitem.h"

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
	bool m_isAnnotation;
	Annotation m_annotation;
	QString AnName;
	/*
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
	*/
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
	MarginStruct() : Top(0), Left(0), Bottom(0), Right(0) {}
	MarginStruct(double top, double left, double bottom, double right) :
		Top(top), Left(left), Bottom(bottom), Right(right) {}
	double Top;
	double Left;
	double Bottom;
	double Right;
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
	QStringList pageNames;
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
	QKeySequence keySequence;
	int tableRow;
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
	QStringList allSeparations;
	bool useSpotColors;
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

/*! Human readable orientations */
enum PageOrientation
{
	portraitPage = 0,
	landscapePage = 1,
	customPage = 30
};

#endif
