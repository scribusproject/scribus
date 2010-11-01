/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          importxar.h  -  description
                             -------------------
    begin                : Wed Nov 25 2009
    copyright            : (C) 2009 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef IMPORTXAR_H
#define IMPORTXAR_H


#include "pluginapi.h"
#include "commonstrings.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QRect>
#include <QStack>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

class XarStyle
{
public:
	XarStyle() :
		dashOffset(0),
		FontFamily(""),
		FontSize(16.0),
		LineHeight(15.0),
		LineWidth(0.0),
		FontStretch(1.0),
		FontKerning(0.0),
		FontBold(false),
		FontUnderline(false),
		FontItalic(false),
		TextAlignment(0),
		FillCol(CommonStrings::None),
		fillRule(true),
		FillGradient(VGradient::linear),
		StrokeGradient(VGradient::linear),
		MaskGradient(VGradient::linear),
		FillGradientType(0),
		StrokeGradientType(0),
		GradFillX1(0),
		GradFillX2(0),
		GradFillY1(0),
		GradFillY2(0),
		GrScale(1),
		GrSkew(0),
		GradStrokeX1(0),
		GradStrokeX2(0),
		GradStrokeY1(0),
		GradStrokeY2(0),
		GradMask(0),
		GradMaskX1(0),
		GradMaskX2(0),
		GradMaskY1(0),
		GradMaskY2(0),
		GradMaskScale(1),
		GradMaskSkew(0),
		LWidth(0.5),
		PLineArt(Qt::SolidLine),
		PLineEnd(Qt::FlatCap),
		PLineJoin(Qt::BevelJoin),
		StrokeCol("Black"),
		FillOpacity(0.0),
		FillBlend(0),
		StrokeOpacity(0.0),
		StrokeBlend(0),
		clipPath(),
		fillPattern(""),
		patternScaleX(1),
		patternScaleY(1),
		patternOffsetX(0),
		patternOffsetY(0),
		patternRotation(0),
		patternSkewX(0),
		patternSkewY(0),
		maskPattern(""),
		patternMaskScaleX(1),
		patternMaskScaleY(1),
		patternMaskOffsetX(0),
		patternMaskOffsetY(0),
		patternMaskRotation(0),
		patternMaskSkewX(0),
		patternMaskSkewY(0),
		strokePattern(""),
		patternScaleXS(1),
		patternScaleYS(1),
		patternOffsetXS(0),
		patternOffsetYS(0),
		patternRotationS(0),
		patternSkewXS(0),
		patternSkewYS(0),
		patternSpace(0),
		patternStrokePath(false),
		GrControl1(FPoint()),
		GrControl2(FPoint()),
		GrControl3(FPoint()),
		GrControl4(FPoint()),
		GrControl5(FPoint()),
		GrColorP1(""),
		GrColorP2(""),
		GrColorP3(""),
		GrColorP4(""),
		GrCol1transp(1.0),
		GrCol2transp(1.0),
		GrCol3transp(1.0),
		GrCol4transp(1.0),
		GrCol1Shade(100),
		GrCol2Shade(100),
		GrCol3Shade(100),
		GrCol4Shade(100),
		Elements()
		{
		}
	QVector<double> dashArray;
	double dashOffset;
	QString FontFamily;
	double FontSize;
	double LineHeight;
	double LineWidth;
	double FontStretch;
	double FontKerning;
	bool FontBold;
	bool FontUnderline;
	bool FontItalic;
	int TextAlignment;
	QString FillCol;
	bool fillRule;
	VGradient FillGradient;
	VGradient StrokeGradient;
	VGradient MaskGradient;
	int    FillGradientType;
	int    StrokeGradientType;
	double GradFillX1;
	double GradFillX2;
	double GradFillY1;
	double GradFillY2;
	double GrScale;
	double GrSkew;
	double GradStrokeX1;
	double GradStrokeX2;
	double GradStrokeY1;
	double GradStrokeY2;
	int    GradMask;
	double GradMaskX1;
	double GradMaskX2;
	double GradMaskY1;
	double GradMaskY2;
	double GradMaskScale;
	double GradMaskSkew;
	double LWidth;
	Qt::PenStyle PLineArt;
	Qt::PenCapStyle PLineEnd;
	Qt::PenJoinStyle PLineJoin;
	QString StrokeCol;
	double FillOpacity;
	int    FillBlend;
	double StrokeOpacity;
	int    StrokeBlend;
	FPointArray clipPath;
	QString fillPattern;
	double patternScaleX;
	double patternScaleY;
	double patternOffsetX;
	double patternOffsetY;
	double patternRotation;
	double patternSkewX;
	double patternSkewY;
	QString maskPattern;
	double patternMaskScaleX;
	double patternMaskScaleY;
	double patternMaskOffsetX;
	double patternMaskOffsetY;
	double patternMaskRotation;
	double patternMaskSkewX;
	double patternMaskSkewY;
	QString strokePattern;
	double patternScaleXS;
	double patternScaleYS;
	double patternOffsetXS;
	double patternOffsetYS;
	double patternRotationS;
	double patternSkewXS;
	double patternSkewYS;
	double patternSpace;
	bool   patternStrokePath;
	FPoint GrControl1;
	FPoint GrControl2;
	FPoint GrControl3;
	FPoint GrControl4;
	FPoint GrControl5;
	QString GrColorP1;
	QString GrColorP2;
	QString GrColorP3;
	QString GrColorP4;
	double GrCol1transp;
	double GrCol2transp;
	double GrCol3transp;
	double GrCol4transp;
	int GrCol1Shade;
	int GrCol2Shade;
	int GrCol3Shade;
	int GrCol4Shade;
	QList<PageItem*> Elements;
};

//! \brief Xar (Xara) importer plugin
class XarPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Xar importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	XarPlug( ScribusDoc* doc, int flags );
	~XarPlug();

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
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);
	bool readColors(const QString& fileName, ColorList & colors);

private:
	void parseHeader(QString fName, double &x, double &y, double &b, double &h);
	bool convert(QString fn);
	void parseXar(QDataStream &ts);
	void handleTags(quint32 tag, quint32 dataLen, QDataStream &ts);
	void createGuideLine(QDataStream &ts);
	void handleTextFontSize(QDataStream &ts);
	void defineTextFontFace(QDataStream &ts, quint32 dataLen);
	void handleTextFont(QDataStream &ts);
	void handleTextString(QDataStream &ts, quint32 dataLen);
	void handleTextChar(QDataStream &ts);
	void handleLineInfo(QDataStream &ts);
	void handleTextAlignment(quint32 tag);
	void handleTextTracking(QDataStream &ts);
	void handleTextKerning(QDataStream &ts);
	void handleTextAspectRatio(QDataStream &ts);
	void handleTextBaseline(QDataStream &ts);
	void startTextLine();
	void endTextLine();
	void startSimpleText(QDataStream &ts, quint32 dataLen);
	void startComplexText(QDataStream &ts, quint32 dataLen);
	void startSimplePathText(QDataStream &ts, quint32 dataLen, int type);
	void startComplexPathText(QDataStream &ts, quint32 dataLen, int type);
	void handleFillRule(QDataStream &ts);
	void handleLineEnd(QDataStream &ts);
	void handleLineJoin(QDataStream &ts);
	void handleQuickShapeSimple(QDataStream &ts, quint32 dataLen);
	void handleFlatFillTransparency(QDataStream &ts);
	void handleSimpleGradientTransparency(QDataStream &ts, quint32 dataLen, bool linear);
	void handleSimpleGradientTransparencySkewed(QDataStream &ts, quint32 dataLen);
	void handleEllipticalGradientTransparency(QDataStream &ts, quint32 dataLen);
	void handleBitmapTransparency(QDataStream &ts, quint32 dataLen);
	int  convertBlendMode(int val);
	void handleSimpleGradientElliptical(QDataStream &ts, quint32 dataLen);
	void handleMultiGradientElliptical(QDataStream &ts);
	void handleMultiGradientSkewed(QDataStream &ts);
	void handleMultiGradient(QDataStream &ts, bool linear);
	void handleSimpleGradientSkewed(QDataStream &ts, quint32 dataLen);
	void handleSimpleGradient(QDataStream &ts, quint32 dataLen, bool linear);
	void handleMultiDiamondGradient(QDataStream &ts);
	void handleSimpleDiamondGradient(QDataStream &ts, quint32 dataLen);
	void handleFourColorGradient(QDataStream &ts);
	void handleBitmapFill(QDataStream &ts, quint32 dataLen);
	void handleContoneBitmapFill(QDataStream &ts, quint32 dataLen);
	void handleBitmap(QDataStream &ts);
	void defineBitmap(QDataStream &ts, quint32 dataLen, quint32 tag);
	void handleLineColor(QDataStream &ts);
	void handleLineWidth(QDataStream &ts);
	void handleFlatLineTransparency(QDataStream &ts);
	void handleFlatFill(QDataStream &ts);
	void createRectangleItem(QDataStream &ts, bool ellipse = false);
	void createSimilarItem(QDataStream &ts);
	void createPolylineItem(int type);
	void createPolygonItem(int type);
	void handleBrushItem(QDataStream &ts);
	void createBrushItem(QDataStream &ts);
	void createGroupItem();
	void createClipItem();
	void finishClip();
	void finishItem(int z);
	bool handlePathRel(QDataStream &ts, quint32 len);
	void handleLayerInfo(QDataStream &ts);
	void handleSpreadInfo(QDataStream &ts);
	void handleComplexColor(QDataStream &ts);
	void handleColorRGB(QDataStream &ts);
	double decodeColorComponent(quint32 data);
	double decodeFixed16(quint32 data);
	void readCoords(QDataStream &ts, double &x, double &y);
	void addToAtomic(quint32 dataLen, QDataStream &ts);
	void addGraphicContext();
	void popGraphicContext();
	
	int importerFlags;
	int recordCounter;
	int currentLayer;
	int pathTextType;
	int pathGcStackIndex;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	double TextX;
	double TextY;
	double textRotation;
	double textSkew;
	bool firstLayer;
	bool interactive;
	bool cancel;
	bool inTextLine;
	bool inTextBlock;
	bool isPathText;
	bool recordPath;
	QTransform textMatrix;
	struct XarColor
	{
		quint32 colorType;
		quint32 colorModel;
		quint32 colorRef;
		double component1;
		double component2;
		double component3;
		double component4;
		QString name;
	};
	struct XarGroup
	{
		int index;
		int gcStackDepth;
		bool clipping;
		bool isBrush;
		quint32 idNr;
		PageItem* groupItem;
	};
	struct XarText
	{
		QString FontFamily;
		QString itemText;
		double FontSize;
		double FontStretch;
		double FontKerning;
		bool FontBold;
		bool FontUnderline;
		bool FontItalic;
		bool newLine;
		QString FillCol;
		VGradient FillGradient;
		VGradient StrokeGradient;
		VGradient MaskGradient;
		int    FillGradientType;
		int    StrokeGradientType;
		double GradFillX1;
		double GradFillX2;
		double GradFillY1;
		double GradFillY2;
		double GrScale;
		double GrSkew;
		FPoint GrControl1;
		FPoint GrControl2;
		FPoint GrControl3;
		FPoint GrControl4;
		FPoint GrControl5;
		QString GrColorP1;
		QString GrColorP2;
		QString GrColorP3;
		QString GrColorP4;
		double GrCol1transp;
		double GrCol2transp;
		double GrCol3transp;
		double GrCol4transp;
		int GrCol1Shade;
		int GrCol2Shade;
		int GrCol3Shade;
		int GrCol4Shade;
		double GradStrokeX1;
		double GradStrokeX2;
		double GradStrokeY1;
		double GradStrokeY2;
		int    GradMask;
		double GradMaskX1;
		double GradMaskX2;
		double GradMaskY1;
		double GradMaskY2;
		double GradMaskScale;
		double GradMaskSkew;
		double LWidth;
		Qt::PenStyle PLineArt;
		Qt::PenCapStyle PLineEnd;
		Qt::PenJoinStyle PLineJoin;
		QString StrokeCol;
		double FillOpacity;
		int    FillBlend;
		double StrokeOpacity;
		int    StrokeBlend;
		QString fillPattern;
		double patternScaleX;
		double patternScaleY;
		double patternOffsetX;
		double patternOffsetY;
		double patternRotation;
		double patternSkewX;
		double patternSkewY;
		QString maskPattern;
		double patternMaskScaleX;
		double patternMaskScaleY;
		double patternMaskOffsetX;
		double patternMaskOffsetY;
		double patternMaskRotation;
		double patternMaskSkewX;
		double patternMaskSkewY;
	};
	QByteArray imageData;
	QList<PageItem*> Elements;
	QList<quint32> atomicTags;
	QList<quint32> ignoreableTags;
	QList<XarText> textData;
	QMap<qint32, XarColor> XarColorMap;
	QMap<qint32, PageItem*> pathMap;
	QMap<quint32, QString> brushRef;
	QMap<QString, QString> patternMap;
	QMap<quint32, QString> patternRef;
	QMap<quint32, QString> fontRef;
	QStack<XarGroup> groupStack;
	QStack<XarStyle*>	m_gc;
	QString activeLayer;
	QStringList importedColors;
	QStringList importedPatterns;
	FPointArray clipCoords;
	FPointArray Coords;
	FPointArray textPath;
	MultiProgressDialog * progressDialog;
	ScribusDoc* m_Doc;
	Selection* tmpSel;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
