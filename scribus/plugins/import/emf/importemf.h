/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Jan 11 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTEMF_H
#define IMPORTEMF_H


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

class EmfPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the EMF importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EmfPlug plugin
	*/
	EmfPlug( ScribusDoc* doc, int flags );
	~EmfPlug();

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
	bool        checkClip(FPointArray &clip);
	void        aligntoQuadWord(QDataStream &ds);
	double      convertDevice2Pts(double in);
	QPointF     convertDevice2Pts(QPointF in);
	double      convertLogical2Pts(double in);
	QPointF     convertLogical2Pts(QPointF in);
	void        createPatternFromDIB(QImage img, quint32 brID);
	void        getPolyInfo(QDataStream &ds, QRectF &bounds, quint32 &count);
	FPointArray getPolyPoints(QDataStream &ds, quint32 count, bool length, bool closed);
	QPointF     getPoint(QDataStream &ds, bool size);
	quint32     getColor(QDataStream &ds);
	void        setWTransform(QTransform mm, quint32 how);
	QPointF     intersectBoundingRect(PageItem *item, QLineF gradientVector);
	void        finishItem(PageItem* ite, bool fill = true);
	void        invalidateClipGroup();
	void        createClipGroup();
	// Functions for normal EMF Files
	void        handleComment(QDataStream &ds);
	void        handleEllipse(QDataStream &ds);
	void        handleRectangle(QDataStream &ds);
	void        handleRoundRect(QDataStream &ds);
	void        handlePolyBezierTo(QDataStream &ds, bool size);
	void        handlePolylineTo(QDataStream &ds, bool size);
	void        handleLineTo(QDataStream &ds);
	void        handleArc(QDataStream &ds);
	void        handleArcTo(QDataStream &ds);
	void        handleChord(QDataStream &ds);
	void        handlePie(QDataStream &ds);
	void        handleSmallText(QDataStream &ds);
	void        handleText(QDataStream &ds, qint64 posi, bool size);
	void        handleImage(qint32 dstX, qint32 dstY, qint32 dstW, qint32 dstH, QImage img);
	void        handlePatternFill(qint32 dstX, qint32 dstY, qint32 dstW, qint32 dstH);
	QImage      handleDIB(QDataStream &ds, qint64 filePos, quint32 offBitH, quint32 sizeBitH, quint32 offBits, quint32 sizeBits);
	void        handleBezier(QDataStream &ds, bool size);
	void        handlePolygon(QDataStream &ds, bool size, bool fill);
	void        handlePolyPolygon(QDataStream &ds, bool size, bool fill);
	void        handlePenDef(quint32 penID, quint32 penStyle, quint32 penWidth, quint32 penColor);
	QString     handleColor(QColor col);
	void        handleFillRegion(QDataStream &ds);
	void        handleFrameRegion(QDataStream &ds);
	void        handleSetClipRegion(QDataStream &ds);
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
		bool             clipValid;
	};
	QStack<dcState> dcStack;
	QHash<quint32, dcState> dcStackEMP;
	dcState currentDC;
	QHash<quint32, emfStyle> emfStyleMap;
	QHash<quint32, emfStyle> emfStyleMapEMP;
	QList<PageItem*> Elements;
	PageItem* clipGroup;
	double docWidth;
	double docHeight;
	double baseX, baseY;
	double docX;
	double docY;
	double dpiX;
	double dpiY;
	quint32 EmfPdpiX, EmfPdpiY;
	quint32 m_records;
	int recordCount;
	qint32 viewPextendX, viewPextendY;
	qint32 winPextendX, winPextendY;
	qint32 winOrigX, winOrigY;
	QRectF bBoxDev;
	QRectF bBoxMM;
	QStringList importedColors;
	QStringList importedPatterns;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	QString baseFile;
	bool inPath;
	bool inEMFPlus;
	bool emfPlusDual;
	bool emfMixed;
	float emfPlusScale;
	bool SerializableObject_Valid;
	QList<ImageEffect> m_Effects;
	quint32 m_ObjSize;
	quint32 m_currObjSize;
	quint16 m_objID;
public slots:
	void cancelRequested() { cancel = true; }
};

#endif
