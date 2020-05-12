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
//	double GapHorizontal;
//	double GapVertical;
//	double GapBelow;
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
} ;

struct SingleLine
{
	double Width;
	int Dash;
	int LineEnd;
	int LineJoin;
	QString Color;
	int Shade;
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

struct Bullet  //used by style reader while importing ODT files
{
	QString name;
	QString charStr;
	double indent;
	double firstLineIndent;
	double tabPosition;
	CharStyle* style;
	Bullet() : indent(0.0), firstLineIndent(0.0), tabPosition(0.0), style(nullptr) {}
	Bullet(QString n, QString ch) : name(n), charStr(ch), indent(0.0), firstLineIndent(0.0), tabPosition(0.0), style(nullptr) {}
	Bullet(QString n, QString ch, double first, double ind, double tab, CharStyle* chStyle)
	    { name = n; charStr = ch, firstLineIndent = first; indent = ind; tabPosition = tab; style = chStyle; }
};

struct ArrowDesc
{
	QString name;
	bool userArrow;
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

enum PrintEngine
{
	PostScript1 = 1,
	PostScript2 = 2,
	PostScript3 = 3,
	WindowsGDI  = 4
};
typedef QMap<QString, PrintEngine> PrintEngineMap;

struct PrintOptions
{
	bool firstUse;
	bool toFile;
	bool useAltPrintCommand;
	bool outputSeparations;
	bool useSpotColors;
	bool useColor;
	bool mirrorH;
	bool mirrorV;
	bool doGCR;
	bool doClip;
	bool setDevParam;
	bool useDocBleeds;
	bool cropMarks;
	bool bleedMarks;
	bool registrationMarks;
	bool colorMarks;
	bool includePDFMarks;
	int  copies;
	PrintEngine prnEngine;
	double markLength;
	double markOffset;
	MarginStruct bleeds;
	std::vector<int> pageNumbers;
	QString printerOptions;
	QString printer;
	QString filename;
	QString separationName;
	QStringList allSeparations;
	QString printerCommand;
	QByteArray devMode; // printer specific options on Windows
};

typedef QMap<QString,QString> ProfilesL;
// typedef QValueVector<SingleLine> multiLine;

class multiLine : public QList<SingleLine> {
public:
	QString shortcut;
	bool operator!=(const multiLine& other) const
	{
		return !(this->operator ==(other));
	}
};

typedef enum {
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
} PreflightError;

typedef QMap<PreflightError, int> errorCodes;


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
typedef QList<double> Guides;

//! \brief from ols scribusXml
struct Linked
{
	int Start;
	int StPag;
};

// this is a quick hack to combine runs until I've thought of something better -- AV
class LastStyles
{
public:
	CharStyle Style;
	int StyleStart;
	QString ParaStyle;
	LastStyles() {
		StyleStart = 0;
	}
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
	bool download;
};

struct DownloadData
{
	int id;
	QUrl url;
	QString name;
	QString downloadLocation;
	QString destinationLocation;
	QString destinationName;
	typedef enum {New, Started, Paused, Finished, Successful, Failed} DownloadState;
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
	PAL_MAX
};

#endif

