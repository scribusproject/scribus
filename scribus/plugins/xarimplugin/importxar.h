/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
		FontStyle("normal"),
		FontWeight("normal"),
		FontStretch("normal"),
		FontSize(12),
		FillCol(CommonStrings::None),
		fillRule(true),
		FillGradient(VGradient::linear),
		StrokeGradient(VGradient::linear),
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
		patternScaleX(0),
		patternScaleY(0),
		patternOffsetX(0),
		patternOffsetY(0),
		patternRotation(0),
		patternSkewX(0),
		patternSkewY(0),
		Elements()
		{
		}
	QVector<double> dashArray;
	double dashOffset;
	QString FontFamily;
	QString FontStyle;
	QString FontWeight;
	QString FontStretch;
	int FontSize;
	QString FillCol;
	bool fillRule;
	VGradient FillGradient;
	VGradient StrokeGradient;
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

private:
	void parseHeader(QString fName, double &x, double &y, double &b, double &h);
	bool convert(QString fn);
	void parseXar(QDataStream &ts);
	void handleTags(quint32 tag, quint32 dataLen, QDataStream &ts);
	void handleFillRule(QDataStream &ts);
	void handleLineEnd(QDataStream &ts);
	void handleLineJoin(QDataStream &ts);
	void handleQuickShapeSimple(QDataStream &ts, quint32 dataLen);
	void handleFlatFillTransparency(QDataStream &ts);
	void handleSimpleGradientTransparency(QDataStream &ts, quint32 dataLen, bool linear);
	void handleEllipticalGradientTransparency(QDataStream &ts, quint32 dataLen);
	int  convertBlendMode(int val);
	void handleSimpleGradientElliptical(QDataStream &ts, quint32 dataLen);
	void handleMultiGradientElliptical(QDataStream &ts);
	void handleMultiGradientSkewed(QDataStream &ts);
	void handleMultiGradient(QDataStream &ts, bool linear);
	void handleSimpleGradientSkewed(QDataStream &ts, quint32 dataLen);
	void handleSimpleGradient(QDataStream &ts, quint32 dataLen, bool linear);
	void handleBitmapFill(QDataStream &ts, quint32 dataLen);
	void defineBitmap(QDataStream &ts, quint32 dataLen, quint32 tag);
	void handleLineColor(QDataStream &ts);
	void handleLineWidth(QDataStream &ts);
	void handleFlatLineTransparency(QDataStream &ts);
	void handleFlatFill(QDataStream &ts);
	void createPolylineItem(int type);
	void createPolygonItem(int type);
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
	QMap<quint32, XarColor> XarColorMap;
	QList<PageItem*> Elements;
	int recordCounter;
	QList<quint32> atomicTags;
	QList<quint32> ignoreableTags;
	QStack<QList<PageItem*> > groupStack;
	QStack<XarStyle*>	m_gc;
	QMap<QString, QString> patternMap;
	QMap<quint32, QString> patternRef;
	QByteArray imageData;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	bool firstLayer;
	QString activeLayer;
	int currentLayer;

	double LineW;
	QString CurrColorFill;
	QColor backColor;
	QString CurrColorStroke;
	QColor foreColor;
	double CurrStrokeShade;
	double CurrFillShade;
	bool patternMode;
	QByteArray patternData;
	QRect currRect;
	int currRectItemNr;
	int currRectType;
	QRect lastImageRect;
	QStringList importedColors;
	QPoint ovalSize;
	QMap<int, QString> fontMap;
	int currentTextSize;
	int currentFontID;
	int currentFontStyle;
	FPointArray lastCoords;

	FPointArray Coords;
	QPoint currentPointT;
	bool lineMode;
	bool postscriptMode;
	bool textIsPostScript;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	int oldDocItemCount;
	QString baseFile;
	int pctVersion;
	bool skipOpcode;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
