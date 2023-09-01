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

#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QRect>
#include <QStack>

#include "pluginapi.h"
#include "commonstrings.h"
#include "fpointarray.h"
#include "pageitem.h"
#include "sccolor.h"
#include "scpatterntransform.h"

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

class XarStyle
{
public:
	XarStyle() = default;

	QVector<double> dashArray;
	double dashOffset { 0 };
	QString FontFamily;
	double FontSize { 16.0 };
	double LineHeight { 15.0 };
	double LineWidth { 0.0 };
	double LineWidth2 { 0.0 };
	double FontStretch { 0.0 };
	double FontKerning { 0.0 };
	bool FontBold { false };
	bool FontUnderline { false };
	bool FontItalic { false };
	int TextAlignment { 0 };
	double TextLeftIndent { 0 };
	QString FillCol { CommonStrings::None };
	bool fillRule { true };
	VGradient FillGradient { VGradient::linear };
	VGradient StrokeGradient { VGradient::linear };
	VGradient MaskGradient { VGradient::linear };
	int    FillGradientType { 0 };
	int    StrokeGradientType { 0 };
	double GradFillX1 { 0.0 };
	double GradFillX2 { 0.0 };
	double GradFillY1 { 0.0 };
	double GradFillY2 { 0.0 };
	double GrScale { 1.0 };
	double GrSkew { 0.0 };
	double GradStrokeX1 { 0.0 };
	double GradStrokeX2 { 0.0 };
	double GradStrokeY1 { 0.0 };
	double GradStrokeY2 { 0.0 };
	int    GradMask { 0 };
	double GradMaskX1 { 0.0 };
	double GradMaskX2 { 0.0 };
	double GradMaskY1 { 0.0 };
	double GradMaskY2 { 0.0 };
	double GradMaskScale { 1.0 };
	double GradMaskSkew { 0.0 };
	double LWidth { 0.5 };
	Qt::PenStyle PLineArt { Qt::SolidLine };
	Qt::PenCapStyle PLineEnd { Qt::FlatCap };
	Qt::PenJoinStyle PLineJoin { Qt::BevelJoin };
	QString StrokeCol { "Black" };
	double FillOpacity { 0.0 };
	int    FillBlend { 0 };
	double StrokeOpacity { 0.0 };
	int    StrokeBlend { 0 };
	FPointArray clipPath;
	QString fillPattern;
	ScPatternTransform fillPatternTrans;
	QString maskPattern;
	ScMaskTransform maskPatternTrans;
	QString strokePattern;
	ScStrokePatternTransform strokePatternTrans;
	bool   patternStrokePath { false };
	FPoint GrControl1;
	FPoint GrControl2;
	FPoint GrControl3;
	FPoint GrControl4;
	FPoint GrControl5;
	QString GrColorP1;
	QString GrColorP2;
	QString GrColorP3;
	QString GrColorP4;
	double GrCol1transp { 1.0 };
	double GrCol2transp { 1.0 };
	double GrCol3transp { 1.0 };
	double GrCol4transp { 1.0 };
	int GrCol1Shade { 100 };
	int GrCol2Shade { 100 };
	int GrCol3Shade { 100 };
	int GrCol4Shade { 100 };
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
	bool import(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);
	bool readColors(const QString& fileName, ColorList & colors);

private:
	void parseHeader(const QString& fName, double &x, double &y, double &b, double &h);
	bool convert(const QString& fn);
	void parseXar(QDataStream &ts);
	void handleTags(quint32 tag, quint32 dataLen, QDataStream &ts);
	void createGuideLine(QDataStream &ts);
	void handleTextFontSize(QDataStream &ts);
	void defineTextFontFace(QDataStream &ts, quint32 dataLen);
	void handleTextFont(QDataStream &ts);
	void handleTextString(QDataStream &ts, quint32 dataLen);
	void handleTextChar(QDataStream &ts);
	void handleTextWrap(QDataStream &ts);
	void handleTextIndent(QDataStream &ts);
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
	void handleFirstPage(QDataStream &ts);
	void handlePage(QDataStream &ts);
	void handleComplexColor(QDataStream &ts);
	void handleColorRGB(QDataStream &ts);
	double decodeColorComponent(quint32 data);
	double decodeFixed16(quint32 data);
	void readCoords(QDataStream &ts, double &x, double &y);
	void addToAtomic(quint32 dataLen, QDataStream &ts);
	void addGraphicContext();
	void popGraphicContext();
	
	int importerFlags { 0 };
	int recordCounter { 0 };
	int currentLayer { 0 };
	int pathTextType { 0 };
	int pathGcStackIndex { 0 };
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };
	int pagecount { 1 };
	double TextX { 0.0 };
	double TextY { 0.0 };
	double textRotation { 0.0 };
	double textSkew { 0.0 };
	bool firstLayer { true };
	bool interactive { false };
	bool cancel { false };
	bool inTextLine { false };
	bool inTextBlock { false };
	bool isPathText { false };
	bool recordPath { false };
	QTransform textMatrix;
	struct XarColor
	{
		quint32 colorType { 0 };
		quint32 colorModel { 2 };
		quint32 colorRef { 0 };
		double component1 { 0.0 };
		double component2 { 0.0 };
		double component3 { 0.0 };
		double component4 { 0.0 };
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
		double FontSize { 16.0 };
		double FontStretch { 0.0 };
		double FontKerning { 0.0 };
		bool FontBold { false };
		bool FontUnderline { false };
		bool FontItalic { false };
		QString FillCol { CommonStrings::None };
		VGradient FillGradient { VGradient::linear };
		VGradient StrokeGradient { VGradient::linear };
		VGradient MaskGradient { VGradient::linear };
		int    FillGradientType { 0 };
		int    StrokeGradientType { 0 };
		double GradFillX1 { 0.0 };
		double GradFillX2 { 0.0 };
		double GradFillY1 { 0.0 };
		double GradFillY2 { 0.0 };
		double GrScale { 1.0 };
		double GrSkew { 0.0 };
		FPoint GrControl1;
		FPoint GrControl2;
		FPoint GrControl3;
		FPoint GrControl4;
		FPoint GrControl5;
		QString GrColorP1;
		QString GrColorP2;
		QString GrColorP3;
		QString GrColorP4;
		int GrCol1Shade { 100 };
		int GrCol2Shade { 100 };
		int GrCol3Shade { 100 };
		int GrCol4Shade { 100 };
		double GradStrokeX1 { 0.0 };
		double GradStrokeX2 { 0.0 };
		double GradStrokeY1 { 0.0 };
		double GradStrokeY2 { 0.0 };
		int    GradMask { 0 };
		double GradMaskX1 { 0.0 };
		double GradMaskX2 { 0.0 };
		double GradMaskY1 { 0.0 };
		double GradMaskY2 { 0.0 };
		double GradMaskScale { 1.0 };
		double GradMaskSkew { 0.0 };
		double LWidth { 0.5 };
		Qt::PenStyle PLineArt { Qt::SolidLine };
		Qt::PenCapStyle PLineEnd { Qt::FlatCap };
		Qt::PenJoinStyle PLineJoin { Qt::BevelJoin };
		QString StrokeCol { "Black" };
		double FillOpacity { 0.0 };
		int    FillBlend { 0 };
		double StrokeOpacity { 0.0 };
		int    StrokeBlend { 0 };
		QString fillPattern;
		ScPatternTransform fillPatternTrans;
		QString maskPattern;
		ScMaskTransform maskPatternTrans;
	};
	struct XarTextLine
	{
		QList<XarText> textData;
	};

	QByteArray imageData;
	QList<PageItem*> Elements;
	QList<quint32> atomicTags;
	QList<quint32> ignoreableTags;
	QList<XarText> textData;
	QList<XarTextLine> textLines;
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
	MultiProgressDialog * progressDialog { nullptr };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
