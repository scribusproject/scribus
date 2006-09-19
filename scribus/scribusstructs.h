/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
	QString pattern;
	double patternScaleX;
	double patternScaleY;
	double patternOffsetX;
	double patternOffsetY;
	double patternRotation;
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
	bool isPrintable;
	bool isBookmark;
	bool m_isAnnotation;
	Annotation m_annotation;
	QString AnName;
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
	int TextflowMode;
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
	int TransBlend;
	int TransBlendS;
	bool Reverse;
	QString NamedLStyle;
	QString Language;
	QString guiLanguage;
	int Cols;
	double ColGap;
	QValueList<ParagraphStyle::TabRecord> TabValues;
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

struct CopyContentsBuffer
{
	PageItem::ItemType sourceType;
	QString contentsFileName;
	double LocalScX;
	double LocalScY;
	double LocalX;
	double LocalY;
	double ItemX;
	double ItemY;
	QString inputProfile;
	bool useEmbedded;
	int renderingIntent;
};

/** \brief Pagemargins */
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

struct DocPagesSetup
{
	DocPagesSetup() : pageArrangement(0), firstPageLocation(0), firstPageNumber(0), orientation(0), autoTextFrames(false), columnDistance(0), columnCount(1) {}
	DocPagesSetup(int pa, int fpl, int fpn, int o, bool atf, double cd, double cc) :
		pageArrangement(pa), firstPageLocation(fpl), firstPageNumber(fpn), orientation(o), autoTextFrames(atf),
		columnDistance(cd), columnCount(cc) {}
	int pageArrangement;
	int firstPageLocation;
	int firstPageNumber;
	int orientation;
	bool autoTextFrames;
	double columnDistance;
	double columnCount;
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

struct CMSData
{
	QString DefaultMonitorProfile;
	QString DefaultPrinterProfile;
	QString DefaultImageRGBProfile;
	QString DefaultImageCMYKProfile;
	QString DefaultSolidColorRGBProfile;
	QString DefaultSolidColorCMYKProfile;
	int ComponentsInput2;
	int ComponentsInput3;
	int ComponentsPrinter;
	int DefaultIntentImages;
	int DefaultIntentColors;
	bool CMSinUse;
	bool SoftProofOn;
	bool SoftProofFullOn;
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
	int blendMode;
	bool isPrintable;
	bool isViewable;
	bool isEditable;
	bool flowControl;
	bool outlineMode;
	double transparency;
	QColor markerColor;
	QString Name;
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
	QString menuName;
	int menuPos;
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
	bool doClip;
	bool setDevParam;
	bool doOverprint;
	int copies;
	std::vector<int> pageNumbers;
	QString printerOptions;
};

typedef QMap<QString,QString> ProfilesL;
typedef QValueVector<SingleLine> multiLine;


typedef enum {
	MissingGlyph=1,
	TextOverflow=2,
	ObjectNotOnPage=3,
	MissingImage=4,
	ImageDPITooLow=5,
	Transparency=6,
	PDFAnnotField=7,
	PlacedPDF=8,
	ImageDPITooHigh=9,
	ImageIsGIF=10,
	BlendMode=11
} PreflightError;

typedef QMap<PreflightError, int> errorCodes;

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

/*! \brief Human readable orientations */
enum PageOrientation
{
	portraitPage = 0,
	landscapePage = 1,
	customPage = 30
};

//! \brief Common type for guides list
typedef QValueList<double> Guides;

//! \brief from ols scribusXml
struct Linked 
{ 
	int Start;
	int StPag;
};

// this is a quick hack to combine runs until I've thought of something better -- AV
class LastStyles {
public:
	CharStyle Style;
	int StyleStart;
	int ParaStyle;
	LastStyles() {
		StyleStart = 0;
		ParaStyle = -1;
	}
};

#endif
