/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Mar 7 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTSVM_H
#define IMPORTSVM_H


#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "commonstrings.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QStack>
#include <QString>
#include <QRect>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

class emfStyle
{
public:
	emfStyle() :
		styType(0),
		penStyle(Qt::SolidLine),
		penCap(Qt::RoundCap),
		penJoin(Qt::RoundJoin),
		penWidth(0.0),
		dashArray(),
		dashOffset(0.0),
		brushStyle(0),
		hatchStyle(0),
		gradientStart(),
		gradientEnd(),
		gradientAngle(0),
		gradient(VGradient::linear),
		gradientPath(),
		hAlign(0),
		vAlign(0),
		fontUnit(0),
		verticalText(false),
		fontRotation(0.0),
		fontSize(12.0),
		fontName(""),
		penColor(CommonStrings::None),
		brushColor(CommonStrings::None),
		patternName(""),
		patternMode(0),
		fillTrans(0.0),
		penTrans(0.0),
		Coords(),
		MetaFile(false),
		imageType(0),
		imageWidth(0),
		imageHeight(0),
		imagePixelFormat(0),
		imageData()
	{
		gradientPath.resize(0);
		gradientPath.svgInit();
		imageData.resize(0);
		Coords.resize(0);
		Coords.svgInit();
	}
	quint32          styType;
	Qt::PenStyle     penStyle;
	Qt::PenCapStyle  penCap;
	Qt::PenJoinStyle penJoin;
	double           penWidth;
	QVector<double>  dashArray;
	double           dashOffset;
	quint32          brushStyle;
	quint32          hatchStyle;
	QPointF          gradientStart;
	QPointF          gradientEnd;
	double           gradientAngle;
	VGradient        gradient;
	FPointArray      gradientPath;
	quint32          hAlign;
	quint32          vAlign;
	quint32          fontUnit;
	bool             verticalText;
	double           fontRotation;
	double           fontSize;
	QString          fontName;
	QString          penColor;
	QString          brushColor;
	QString          patternName;
	quint32          patternMode;
	double           fillTrans;
	double           penTrans;
	FPointArray      Coords;
	bool             MetaFile;
	quint32          imageType;
	qint32           imageWidth;
	qint32           imageHeight;
	quint32          imagePixelFormat;
	QByteArray       imageData;
};

class SvmPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the SVM importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval SvmPlug plugin
	*/
	SvmPlug( ScribusDoc* doc, int flags );
	~SvmPlug();

	/*!
	\author Franz Schmid
	\date
	\brief Perform import.
	\param fn QString
	\param trSettings undo transaction settings
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval bool true if import was ok
	 */
	bool        import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage      readThumbnail(QString fn);

private:
	void        parseHeader(QString fName, double &x, double &y, double &b, double &h);
	bool        convert(QString fn);
	// Common functions
	void        aligntoQuadWord(QDataStream &ds);
	double      convertLogical2Pts(double in);
	QPointF     convertLogical2Pts(QPointF in);
	FPointArray getPolyPolygonPoints(QDataStream &ds, quint16 version);
	FPointArray getPolyPoints(QDataStream &ds, quint32 count, bool closed);
	QPointF     getPoint(QDataStream &ds);
	void        getColor(QDataStream &ds, QString &colorN);
	QPointF     intersectBoundingRect(PageItem *item, QLineF gradientVector);
	void        finishItem(PageItem* ite, bool fill = true);
	void        handleEllipse(QDataStream &ds);
	void        handleRectangle(QDataStream &ds);
	void        handleRoundRect(QDataStream &ds);
	void        handlePolyline(QDataStream &ds);
	void        handleLine(QDataStream &ds);
	void        handleArc(QDataStream &ds);
	void        handleArcTo(QDataStream &ds);
	void        handleChord(QDataStream &ds);
	void        handlePie(QDataStream &ds);
	void        handleFontDef(QDataStream &ds);
	void        handleSmallText(QDataStream &ds);
	void        handleText(QDataStream &ds, quint16 version);
	void        handleImage(QDataStream &ds, qint64 posi, quint32 totalSize);
	void        handleImageEX(QDataStream &ds, qint64 posi, quint32 totalSize);
	void        handlePolygon(QDataStream &ds);
	void        handlePolyPolygon(QDataStream &ds, quint16 version);
	void        handleTransparent(QDataStream &ds, quint16 version);
	void        handleHatch(QDataStream &ds, quint16 version);
	void        handleGradient(QDataStream &ds);
	void        handleGradientEX(QDataStream &ds, quint16 version);
	void        commonGradient(QDataStream &ds, PageItem* ite);
	QString     handleColor(QColor col);
	void        handleSetClipRegion(QDataStream &ds);
	void        handleComment(QDataStream &ds);
	// Functions for EMF+ Files
	void        handleEMFPlus(QDataStream &ds, quint32 dtaSize);
	void        handleEMPObject(QDataStream &ds, quint8 flagsH, quint8 flagsL, quint32 dataSize);
	quint32     handleEMPBrush(QDataStream &ds, quint16 id, bool first, bool cont, quint32 dataSize);
	void        handleEMPPen(QDataStream &ds, quint16 id);
	void        handleEMPPath(QDataStream &ds, quint16 id);
	void        handleEMPRegion(QDataStream &ds, quint16 id);
	quint32     handleEMPImage(QDataStream &ds, quint16 id, bool first, bool cont, quint32 dataSize);
	quint32     getImageData(QDataStream &ds, quint16 id, bool first, bool cont, quint32 dataSize, emfStyle &sty);
	void        handleEMPFont(QDataStream &ds, quint16 id);
	void        handleEMPSFormat(QDataStream &ds, quint16 id);
	void        handleEMPLineCap(QDataStream &ds, quint16 id);
	void        handleEMFPFillClosedCurve(QDataStream &ds, quint8 flagsL);
	void        handleEMFPFillEllipse(QDataStream &ds, quint8 flagsL);
	void        handleEMFPFillPath(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPFillPie(QDataStream &ds, quint8 flagsL);
	void        handleEMFPFillPolygon(QDataStream &ds, quint8 flagsL);
	void        handleEMFPFillRects(QDataStream &ds, quint8 flagsL);
	void        handleEMFPFillRegion(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawArc(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawBezier(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawClosedCurve(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawCurve(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawEllipse(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawImage(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawImagePoints(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawLines(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawPath(QDataStream &ds, quint8 flagsH);
	void        handleEMFPDrawPie(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawRects(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawDriverString(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPDrawString(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPSetClipRect(QDataStream &ds, quint8 flagsL);
	void        handleEMFPSetClipRegion(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void        handleEMFPSetClipPath(QDataStream &ds, quint8 flagsL, quint8 flagsH);
	void		handleEMFPSerializableObject(QDataStream &ds);
	void        getEMFPBrush(quint32 brushID, bool directBrush);
	void        getEMFPPen(quint32 penID);
	void        getEMFPFont(quint32 fontID);
	void        getEMFPStringFormat(quint32 fontID);
	FPointArray getEMPPathData(QDataStream &ds);
	QPolygonF   getEMFPCurvePoints(QDataStream &ds, quint8 flagsL, quint32 count);
	QPolygonF   getEMFPRect(QDataStream &ds, bool size);
	QPointF     getEMFPPoint(QDataStream &ds, bool size);
	double      getEMFPDistance(QDataStream &ds, bool size);
	QPointF     convertEMFPLogical2Pts(QPointF in, quint16 unit);
	double      convertEMFPLogical2Pts(double in, quint16 unit);
	QPolygonF   gdip_open_curve_tangents(QPolygonF &points, double tension);
	QPolygonF   gdip_closed_curve_tangents(QPolygonF &points, double tension);
	void        append_curve(QPainterPath &path, QPolygonF &points, QPolygonF &tangents, bool type);
	void        GdipAddPathCurve(QPainterPath &path, QPolygonF &points, float tension);
	void        GdipAddPathClosedCurve(QPainterPath &path, QPolygonF &points, float tension);
	void        handleEMFPDrawImageData(QPointF p1, QPointF p2, QPointF p3, quint8 flagsH);
	QImage      getImageDataFromStyle(quint8 flagsH);
	struct dcState
	{
		quint32          m_mapMode;
		quint16          emfPlusUnit;
		quint32          textAlignment;
		quint32          hatchStyle;
		quint32          brushStyle;
		QPointF          gradientStart;
		QPointF          gradientEnd;
		double           gradientAngle;
		VGradient        gradient;
		QTransform       m_WorldMap;
		QTransform       m_WorldMapEMFP;
		bool             fillRule;
		bool             backgroundMode;
		bool		     arcDirection;
		bool             alphaOn;
		double           LineW;
		QVector<double>  dashArray;
		double           dashOffset;
		Qt::PenStyle     penStyle;
		Qt::PenCapStyle  penCap;
		Qt::PenJoinStyle penJoin;
		QString          CurrColorFill;
		QString          CurrColorStroke;
		QString          CurrColorText;
		QString          backColor;
		QString          fontName;
		QString          patternName;
		quint32          patternMode;
		double           fontSize;
		double           fontRotation;
		double           CurrStrokeTrans;
		double           CurrFillTrans;
		FPointArray      Coords;
		FPointArray      clipPath;
		FPointArray      gradientPath;
		QPointF          viewOrigin;
		QPointF          winOrigin;
		QPointF          currentPoint;
		QPointF          originEMFP;
		quint32          hAlign;
		quint32          vAlign;
		quint32          fontUnit;
		bool             verticalText;
		quint16          fontEnc;
		quint16          fontPit;
		quint16          fontWgt;
		quint16          fontUdl;
		quint16          fontStk;
		quint16          fontIta;
		quint16          fontOvl;
		quint8           fontOul;
		quint8           fontShd;
		quint8           fontKer;
		quint16          fontWdt;
	};
	QStack<dcState> dcStack;
	QHash<quint32, dcState> dcStackEMP;
	dcState currentDC;
	QHash<quint32, emfStyle> emfStyleMapEMP;
	QList<PageItem*> Elements;
	double docWidth;
	double docHeight;
	double baseX, baseY;
	double docX;
	double docY;
	quint32 EmfPdpiX, EmfPdpiY;
	quint32 m_records;
	int recordCount;
	qint32 viewPextendX, viewPextendY;
	qint32 winPextendX, winPextendY;
	qint32 winOrigX, winOrigY;
	QStringList importedColors;
	QStringList importedPatterns;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	QString baseFile;
	struct VersionCompat
	{
		quint16  version;
		quint32  length;
	};
	struct Fraction
	{
		quint32  numerator;
		quint32  denominator;
	};
	struct MapMode
	{
		VersionCompat  version;
		quint16        unit;
		QPoint         origin;
		Fraction       scaleX;
		Fraction       scaleY;
		quint8         isSimple;
	};
	struct SvmHeader
	{
		VersionCompat  versionCompat;
		quint32        compressionMode;
		MapMode        mapMode;
		quint32        width;
		quint32        height;
		quint32        actionCount;
	};
	enum ActionType
	{
		META_NULL_ACTION                  = 0,
		META_PIXEL_ACTION                 = 100,
		META_POINT_ACTION                 = 101,
		META_LINE_ACTION                  = 102,
		META_RECT_ACTION                  = 103,
		META_ROUNDRECT_ACTION             = 104,
		META_ELLIPSE_ACTION               = 105,
		META_ARC_ACTION                   = 106,
		META_PIE_ACTION                   = 107,
		META_CHORD_ACTION                 = 108,
		META_POLYLINE_ACTION              = 109,
		META_POLYGON_ACTION               = 110,
		META_POLYPOLYGON_ACTION           = 111,
		META_TEXT_ACTION                  = 112,
		META_TEXTARRAY_ACTION             = 113,
		META_STRETCHTEXT_ACTION           = 114,
		META_TEXTRECT_ACTION              = 115,
		META_BMP_ACTION                   = 116,
		META_BMPSCALE_ACTION              = 117,
		META_BMPSCALEPART_ACTION          = 118,
		META_BMPEX_ACTION                 = 119,
		META_BMPEXSCALE_ACTION            = 120,
		META_BMPEXSCALEPART_ACTION        = 121,
		META_MASK_ACTION                  = 122,
		META_MASKSCALE_ACTION             = 123,
		META_MASKSCALEPART_ACTION         = 124,
		META_GRADIENT_ACTION              = 125,
		META_HATCH_ACTION                 = 126,
		META_WALLPAPER_ACTION             = 127,
		META_CLIPREGION_ACTION            = 128,
		META_ISECTRECTCLIPREGION_ACTION   = 129,
		META_ISECTREGIONCLIPREGION_ACTION = 130,
		META_MOVECLIPREGION_ACTION        = 131,
		META_LINECOLOR_ACTION             = 132,
		META_FILLCOLOR_ACTION             = 133,
		META_TEXTCOLOR_ACTION             = 134,
		META_TEXTFILLCOLOR_ACTION         = 135,
		META_TEXTALIGN_ACTION             = 136,
		META_MAPMODE_ACTION               = 137,
		META_FONT_ACTION                  = 138,
		META_PUSH_ACTION                  = 139,
		META_POP_ACTION                   = 140,
		META_RASTEROP_ACTION              = 141,
		META_TRANSPARENT_ACTION           = 142,
		META_EPS_ACTION                   = 143,
		META_REFPOINT_ACTION              = 144,
		META_TEXTLINECOLOR_ACTION         = 145,
		META_TEXTLINE_ACTION              = 146,
		META_FLOATTRANSPARENT_ACTION      = 147,
		META_GRADIENTEX_ACTION            = 148,
		META_LAYOUTMODE_ACTION            = 149,
		META_TEXTLANGUAGE_ACTION          = 150,
		META_OVERLINECOLOR_ACTION         = 151,
		META_RENDERGRAPHIC_ACTION         = 152,
		META_COMMENT_ACTION               = 512
	};
	enum MapUnit
	{
		MAP_100TH_MM,
		MAP_10TH_MM,
		MAP_MM,
		MAP_CM,
		MAP_1000TH_INCH,
		MAP_100TH_INCH,
		MAP_10TH_INCH,
		MAP_INCH,
		MAP_POINT,
		MAP_TWIP,
		MAP_PIXEL,
		MAP_SYSFONT,
		MAP_APPFONT,
		MAP_RELATIVE,
		MAP_LASTENUMDUMMY
	};
	enum GradientStyle
	{
		GradientStyle_LINEAR = 0,
		GradientStyle_AXIAL = 1,
		GradientStyle_RADIAL = 2,
		GradientStyle_ELLIPTICAL = 3,
		GradientStyle_SQUARE = 4,
		GradientStyle_RECT = 5
	};
	enum FontPitch
	{
		PITCH_DONTKNOW,
		PITCH_FIXED,
		PITCH_VARIABLE
	};
	enum FontWeight
	{
		WEIGHT_DONTKNOW,
		WEIGHT_THIN,
		WEIGHT_ULTRALIGHT,
		WEIGHT_LIGHT,
		WEIGHT_SEMILIGHT,
		WEIGHT_NORMAL,
		WEIGHT_MEDIUM,
		WEIGHT_SEMIBOLD,
		WEIGHT_BOLD,
		WEIGHT_ULTRABOLD,
		WEIGHT_BLACK
	};
	enum FontItalic
	{
		ITALIC_NONE,
		ITALIC_OBLIQUE,
		ITALIC_NORMAL,
		ITALIC_DONTKNOW
	};
	enum FontWidth
	{
		WIDTH_DONTKNOW,
		WIDTH_ULTRA_CONDENSED,
		WIDTH_EXTRA_CONDENSED,
		WIDTH_CONDENSED,
		WIDTH_SEMI_CONDENSED,
		WIDTH_NORMAL,
		WIDTH_SEMI_EXPANDED,
		WIDTH_EXPANDED,
		WIDTH_EXTRA_EXPANDED,
		WIDTH_ULTRA_EXPANDED
	};
	enum FontStrikeout
	{
		STRIKEOUT_NONE,
		STRIKEOUT_SINGLE,
		STRIKEOUT_DOUBLE,
		STRIKEOUT_DONTKNOW,
		STRIKEOUT_BOLD,
		STRIKEOUT_SLASH,
		STRIKEOUT_X
	};
	enum FontUnderline
	{
		UNDERLINE_NONE,
		UNDERLINE_SINGLE,
		UNDERLINE_DOUBLE,
		UNDERLINE_DOTTED,
		UNDERLINE_DONTKNOW,
		UNDERLINE_DASH,
		UNDERLINE_LONGDASH,
		UNDERLINE_DASHDOT,
		UNDERLINE_DASHDOTDOT,
		UNDERLINE_SMALLWAVE,
		UNDERLINE_WAVE,
		UNDERLINE_DOUBLEWAVE,
		UNDERLINE_BOLD,
		UNDERLINE_BOLDDOTTED,
		UNDERLINE_BOLDDASH,
		UNDERLINE_BOLDLONGDASH,
		UNDERLINE_BOLDDASHDOT,
		UNDERLINE_BOLDDASHDOTDOT,
		UNDERLINE_BOLDWAVE
	};
	enum TextAlign
	{
		ALIGN_TOP,
		ALIGN_BASELINE,
		ALIGN_BOTTOM
	};
	SvmHeader head;

	bool inPath;
	bool inEMFPlus;
	bool emfPlusDual;
	float emfPlusScale;
	bool SerializableObject_Valid;
	QList<ImageEffect> m_Effects;
	quint32 m_ObjSize;
	quint32 m_currObjSize;
	quint16 m_objID;
	bool seen_XGRAD_SEQ_BEGIN;
public slots:
	void cancelRequested() { cancel = true; }
};

#endif
