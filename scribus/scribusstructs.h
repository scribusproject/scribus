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

#include <QAction>
#include <QByteArray>
#include <QDebug>
#include <QFlags>
#include <QKeySequence>
#include <QList>
#include <QPolygon>
#include <QStack>
#include <QString>
#include <QMap>
#include <QVector>

#include <vector>

#include "scribusapi.h"

#include "annotation.h"
#include "colormgmt/sccolormgmtstructs.h"
#include "fpointarray.h"
#include "margins.h"
#include "pageitem.h"
#include "sccolorstructs.h"
#include "scfonts.h"
#include "scimagestructs.h"
#include "sctextstruct.h"
#include "util_math.h"
#include "vgradient.h"

struct CopyContentsBuffer
{
	PageItem::ItemType sourceType;
	QString contentsFileName;
	double LocalScX;
	double LocalScY;
	double LocalX;
	double LocalY;
	double LocalRot;
	double ItemX;
	double ItemY;
	ScImageEffectList effects;
	QString inputProfile;
	bool   useEmbedded;
	eRenderIntent renderingIntent;
};

struct DocPagesSetup
{
	DocPagesSetup() = default;
	DocPagesSetup(int pa, int fpl, int fpn, int o, bool atf, double cd, double cc) :
		pageArrangement(pa), firstPageLocation(fpl), firstPageNumber(fpn), orientation(o), autoTextFrames(atf),
		columnDistance(cd), columnCount(cc) {}
	int pageArrangement {0};
	int firstPageLocation {0};
	int firstPageNumber {0};
	int orientation {0};
	bool autoTextFrames {false};
	double columnDistance {0.0};
	double columnCount {1.0};
};

struct PageSet
{
	QString Name;
	int FirstPage;
	int Rows;
	int Columns;
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
	eRenderIntent DefaultIntentImages;
	eRenderIntent DefaultIntentColors;
	bool CMSinUse;
	bool SoftProofOn;
	bool SoftProofFullOn;
	bool GamutCheck;
	bool BlackPoint;
};

enum
{
	Gradient_None = 0,
	Gradient_LinearLegacy1 = 1,
	Gradient_LinearLegacy2 = 2,
	Gradient_LinearLegacy3 = 3,
	Gradient_LinearLegacy4 = 4,
	Gradient_RadialLegacy5 = 5,
	Gradient_Linear = 6,
	Gradient_Radial = 7,
	Gradient_Pattern = 8,
	Gradient_4Colors = 9,
	Gradient_Diamond = 10,
	Gradient_Mesh = 11,
	Gradient_PatchMesh = 12,
	Gradient_Conical = 13,
	Gradient_Hatch = 14
};

/*
 * Uncomment if you need a readable string during debugging
static QString gradientTypeDebug(int type)
{
	switch(type)
	{
	case Gradient_None:
		return "Gradient_None";
		break;
	case Gradient_LinearLegacy1:
		return "Gradient_LinearLegacy1";
		break;
	case Gradient_LinearLegacy2:
		return "Gradient_LinearLegacy2";
		break;
	case Gradient_LinearLegacy3:
		return "Gradient_LinearLegacy3";
		break;
	case Gradient_LinearLegacy4:
		return "Gradient_LinearLegacy4";
		break;
	case Gradient_RadialLegacy5:
		return "Gradient_RadialLegacy5";
		break;
	case Gradient_Linear:
		return "Gradient_Linear";
		break;
	case Gradient_Radial:
		return "Gradient_Radial";
		break;
	case Gradient_Pattern:
		return "Gradient_Pattern";
		break;
	case Gradient_4Colors:
		return "Gradient_4Colors";
		break;
	case Gradient_Diamond:
		return "Gradient_Diamond";
		break;
	case Gradient_Mesh:
		return "Gradient_Mesh";
		break;
	case Gradient_PatchMesh:
		return "Gradient_PatchMesh";
		break;
	case Gradient_Conical:
		return "Gradient_Conical";
		break;
	case Gradient_Hatch:
		return "Gradient_Hatch";
		break;
	default:
		return "undefinded type: " + QString::number(type);
		break;
	}
};
*/

enum
{
	GradMask_None = 0,
	GradMask_Linear = 1,
	GradMask_Radial = 2,
	GradMask_Pattern = 3,
	GradMask_LinearLumAlpha = 4,
	GradMask_RadialLumAlpha = 5,
	GradMask_PatternLumAlpha = 6,
	GradMask_PatternLumAlphaInverted = 7,
	GradMask_PatternInverted = 8
};

/*
 * Uncomment if you need a readable string during debugging
static QString gradientMaskDebug(int type)
{
	switch(type)
	{
	case GradMask_None:
		return "GradMask_None";
		break;
	case GradMask_Linear:
		return "GradMask_Linear";
		break;
	case GradMask_Radial:
		return "GradMask_Radial";
		break;
	case GradMask_Pattern:
		return "GradMask_Pattern";
		break;
	case GradMask_LinearLumAlpha:
		return "GradMask_LinearLumAlpha";
		break;
	case GradMask_RadialLumAlpha:
		return "GradMask_RadialLumAlpha";
		break;
	case GradMask_PatternLumAlpha:
		return "GradMask_PatternLumAlpha";
		break;
	case GradMask_PatternLumAlphaInverted:
		return "GradMask_PatternLumAlphaInverted";
		break;
	case GradMask_PatternInverted:
		return "GradMask_PatternInverted";
		break;
	default:
		return "undefinded type: " + QString::number(type);
		break;
	}
};
*/

enum class GradientEdit
{
	// Gradients
	Gradient = 0,
	Gradient_Stroke = 1,
	Gradient_Mask = 2,
	FourColors = 3,
	Diamond = 4,
	// Mesh
	Mesh_Point = 5,
	Mesh_Color = 6,
	Mesh_ControlPoints = 7,
	// Patch Mesh
	PatchMesh_Color = 8,
	PatchMesh_Point = 9,
	PatchMesh_ControlPoints = 10,
	PatchMesh_Polygon = 11
};
Q_DECLARE_METATYPE(GradientEdit)

/*
 * Uncomment if you need a readable string during debugging
static QString gradientEditDebug(GradientEdit type)
{
	switch(type)
	{
	case GradientEdit::Gradient:
		return "Gradient";
		break;
	case GradientEdit::Gradient_Stroke:
		return "Gradient_Stroke";
		break;
	case GradientEdit::Gradient_Mask:
		return "Gradient_Mask";
		break;
	case GradientEdit::FourColors:
		return "FourColors";
		break;
	case GradientEdit::Diamond:
		return "Diamond";
		break;
	case GradientEdit::Mesh_Point:
		return "Mesh_Point";
		break;
	case GradientEdit::Mesh_Color:
		return "Mesh_Color";
		break;
	case GradientEdit::Mesh_ControlPoints:
		return "Mesh_ControlPoints";
		break;
	case GradientEdit::PatchMesh_Color:
		return "PatchMesh_Color";
		break;
	case GradientEdit::PatchMesh_Point:
		return "PatchMesh_Point";
		break;
	case GradientEdit::PatchMesh_ControlPoints:
		return "PatchMesh_ControlPoints";
		break;
	case GradientEdit::PatchMesh_Polygon:
		return "PatchMesh_Polygon";
		break;
	default:
		return "missing type";
	}
};
*/

/**
 * This enum describes the sides that can be selected. A selection can be
 * expressed as an ORed combination of Left, Right, Top and Bottom.
 */
enum class TableSide
{
	None = 0,   /**< None of the sides are selected. */
	Left = 1,   /**< The left side is selected. */
	Right = 2,  /**< The right side is selected. */
	Top = 4,    /**< The top side is selected. */
	Bottom = 8, /**< The bottom side is selected. */
	All = Left | Right | Top | Bottom
};
Q_DECLARE_FLAGS(TableSides, TableSide)
Q_DECLARE_OPERATORS_FOR_FLAGS(TableSides)

struct SingleLine
{
	double Width { 1.0 };
	int Dash { Qt::SolidLine };
	int LineEnd { Qt::FlatCap };
	int LineJoin { Qt::MiterJoin };
	QString Color { "Black" };
	int Shade { 100 };
	// setter necessary for use with serializer/digester
	void setLineWidth(double value) { Width = value; }
	void setDash(int value)         { Dash = value; }
	void setLineEnd(int value)      { LineEnd = value; }
	void setLineJoin(int value)     { LineJoin = value; }
	void setColor(const QString& name) { Color = name; }
	void setShade(int value)        { Shade = value; }
	bool operator==(const SingleLine& other) const
	{
		if (!compareDouble(Width, other.Width) )
			return false;
		if ((Dash != other.Dash)  || (LineEnd != other.LineEnd) || (LineJoin != other.LineJoin) ||
			(Color != other.Color)|| (Shade != other.Shade))
			return false;
		return true;
	}
	bool operator!=(const SingleLine& other) const
	{
		return !(*this == other);
	}
};

class MultiLine : public QList<SingleLine>
{
public:
	QString shortcut;
	// Notice following removal of operator != :
	// MultiLine inherits operator == and != from QList. As a consequence, shortcut
	// will not be taken into account during the comparison, but that was also the
	// case of former operator !=
};

struct Bullet  //used by style reader while importing ODT files
{
	Bullet() = default;
	Bullet(QString n, QString ch) : name(n), charStr(ch) {}
	Bullet(QString n, QString ch, double first, double ind, double tab, CharStyle* chStyle)
	{
		name = n;
		charStr = ch;
		firstLineIndent = first;
		indent = ind;
		tabPosition = tab;
		style = chStyle;
	}

	QString name;
	QString charStr;
	double indent { 0.0 };
	double firstLineIndent { 0.0 };
	double tabPosition { 0.0 };
	CharStyle* style { nullptr };
};

enum class ArrowDirection
{
	StartArrow = 0,
	EndArrow
};

struct ArrowDesc
{
	QString name;
	bool userArrow { false };
	FPointArray points;
};

struct PDFPresentationData
{
	PDFPresentationData() = default;
	int pageEffectDuration {1};
	int pageViewDuration {1};
	int effectType {0};
	int Dm {0};
	int M {0};
	int Di {0};
};

struct LPIData
{
	int Frequency {0};
	int Angle {0};
	int SpotFunc {0};
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

enum class PrintLanguage
{
	PostScript1 = 1,
	PostScript2 = 2,
	PostScript3 = 3,
	WindowsGDI  = 4,
	PDF         = 5
};
using PrintLanguageMap = QMap<QString, PrintLanguage>;

struct PrintOptions
{
	bool firstUse { true };
	bool toFile { false };
	bool useAltPrintCommand { false };
	bool outputSeparations { false };
	bool useSpotColors { true };
	bool useColor { true };
	bool mirrorH { false };
	bool mirrorV { false };
	bool doGCR { false };
	bool doClip { false };
	bool setDevParam { false };
	bool useDocBleeds { true };
	bool cropMarks { false };
	bool bleedMarks { false };
	bool registrationMarks { false };
	bool colorMarks { false };
	bool includePDFMarks { true };
	int  copies { 1 };
	PrintLanguage prnLanguage { PrintLanguage::PostScript3 };
	double markLength { 20.0 };
	double markOffset { 0.0 };
	MarginStruct bleeds;
	std::vector<int> pageNumbers;
	QString printerOptions;
	QString printer;
	QString filename;
	QString separationName { "All" };
	QStringList allSeparations;
	QString printerCommand;
	QByteArray devMode; // printer specific options on Windows
};

using ProfilesL = QMap<QString,QString>;

enum PreflightError
{
	MissingGlyph = 1,
	TextOverflow = 2,
	ObjectNotOnPage = 3,
	MissingImage = 4,
	ImageDPITooLow = 5,
	Transparency = 6,
	PDFAnnotField = 7,
	PlacedPDF = 8,
	ImageDPITooHigh = 9,
	ImageIsGIF = 10,
	BlendMode = 11,
	WrongFontInAnnotation = 12,
	NotCMYKOrSpot = 13,
	DeviceColorsAndOutputIntent = 14,
	FontNotEmbedded = 15,
	EmbeddedFontIsOpenType = 16,
	OffConflictLayers = 17,
	PartFilledImageFrame = 18,
	MarksChanged = 19,
	AppliedMasterDifferentSide = 20,
	EmptyTextFrame = 21
};

using errorCodes = QMap<PreflightError, int>;

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
	PageItem* Object;
};

/*! \brief Human readable orientations */
enum PageOrientation
{
	portraitPage = 0,
	landscapePage = 1,
	customPage = 30
};

/**
* Flags for ScribusView PageToPixmap()
*/
enum PageToPixmapFlag
{
	Pixmap_NoFlags = 0,
	Pixmap_DrawFrame = 1 << 0,
	Pixmap_DrawBackground = 1 << 1,
	Pixmap_DrawWhiteBackground = 1 << 2,
	Pixmap_DontReloadImages = 1 << 3,
	Pixmap_NoCanvasModeChange = 1 << 4
};

Q_DECLARE_FLAGS(PageToPixmapFlags, PageToPixmapFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(PageToPixmapFlags);

enum UpdateRequests
{
	reqColorsUpdate = 1,
	reqCharStylesUpdate = 2,
	reqParaStylesUpdate = 4,
	reqTextStylesUpdate = 6,
	reqArrowStylesUpdate = 8,
	reqLineStylesUpdate  = 16,
	reqSymbolsUpdate     = 32,
	reqDefFontListUpdate = 64,
	reqDocFontListUpdate = 128,
	reqStyleComboDocUpdate = 256,
	reqCmsOptionsUpdate  = 512,
	reqCustomShapeUpdate = 1024,
	reqInlinePalUpdate   = 2048,
	reqMarksUpdate       = 4096,
	reqNumUpdate         = 8192,
	reqUpdateAll = 65535
};

//! \brief Common type for guides list
using Guides = QList<double>;

// this is a quick hack to combine runs until I've thought of something better -- AV
class LastStyles
{
public:
	CharStyle Style;
	int StyleStart {0};
	QString ParaStyle;
};

class AttributeValue
{
	public:
		AttributeValue() = default;
		AttributeValue(const QString& val)
		{
			if (val.isEmpty())
			{
				valid = false;
				value.clear();
			}
			else
			{
				valid = true;
				value = val;
			}
		}
		bool valid {false};
		QString value;
};

//TODO: Dict license showing, URL background unzipping, checksumming, pkg mgr platforms warning
struct DownloadItem
{
	QString lang;
	QString version;
	QString files;
	QString extractfiles;
	QString url;
	QString desc;
	QString license;
	QString filetype;
	QString type;
	QString movetofile;
	QString source;
	bool download {false};
};

struct DownloadData
{
	int id;
	QUrl url;
	QString name;
	QString downloadLocation;
	QString destinationLocation;
	QString destinationName;
	using DownloadState = enum {New, Started, Paused, Finished, Successful, Failed};
	DownloadState state;
};

enum
{
	GS_ALL,
	GS_MARGINS,
	GS_FRAMES,
	GS_GRID,
	GS_GUIDES,
	GS_BASELINE,
	GS_LINKS,
	GS_CONTROLS,
	GS_RULERMODE,
	GS_RULERS,
	GS_COLUMNBORDERS,
	GS_LAYERMARKERS,
	GS_BLEED,
	GS_MAX
};

//Always add new at the end, leave existing and deprecated items in the list
enum
{
	PAL_ALL,
	PAL_PROPERTIES,
	PAL_OUTLINE,
	PAL_SCRAPBOOK,
	PAL_LAYER,
	PAL_PAGE,
	PAL_BOOKMARK,
	PAL_7_UNUSED,
	PAL_UNDO,
	PAL_VERIFIER,
	PAL_DOWNLOADS,
	PAL_TEXT,
	PAL_CONTENT,
	PAL_ALIGNDISTRIBUTE,
	PAL_MAX
};

enum AnchorPoint
{
	None = 0,
	TopLeft = 1,
	Top = 2,
	TopRight = 3,
	Left = 4,
	Center = 5,
	Right = 6,
	BottomLeft = 7,
	Bottom = 8,
	BottomRight = 9
};

#endif

