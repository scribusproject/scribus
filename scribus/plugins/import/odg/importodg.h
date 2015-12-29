/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Feb  9 2014
	copyright            : (C) 2014 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTODG_H
#define IMPORTODG_H

#include "commonstrings.h"
#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "scribusstructs.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;
class ScZipHandler;

class ObjStyle
{
public:
	ObjStyle() :
	  markerViewBox(),
	  markerPath(),
	  startMarkerName(""),
	  startMarkerWidth(0.0),
	  startMarkerCentered(false),
	  endMarkerName(""),
	  endMarkerWidth(0.0),
	  endMarkerCentered(false),
	  stroke_dash_distance(0.0),
	  stroke_dash_dots1(1),
	  stroke_dash_dots1_length(0.0),
	  stroke_dash_dots2(1),
	  stroke_dash_dots2_length(0.0),
	  stroke_dash_style("solid"),
	  dashName(""),
	  CurrColorFill(CommonStrings::None),
	  CurrColorStroke(CommonStrings::None),
	  CurrColorText("Black"),
	  CurrColorBText(CommonStrings::None),
	  CurrColorShadow("Black"),
	  fillOpacity(0.0),
	  strokeOpacity(0.0),
	  LineW(0.0),
	  fontName(""),
	  fontSize(10.0),
	  textIndent(0.0),
	  textAlign(ParagraphStyle::Leftaligned),
	  textPos(""),
	  textOutline(""),
	  textUnderline(false),
	  textUnderlineWords(false),
	  textUnderlineColor(CommonStrings::None),
	  textStrikeThrough(false),
	  textShadow(false),
	  lineHeight(1.0),
	  absLineHeight(false),
	  margin_top(0.0),
	  margin_bottom(0.0),
	  margin_left(0.0),
	  margin_right(0.0),
	  verticalAlignment(0),
	  page_width(595.0),
	  page_height(841.0),
	  page_layout_name(""),
	  fill_type(0),
	  stroke_type(0),
	  gradientAngle(0.0),
	  gradientBorder(0.0),
	  gradientEndColor(CommonStrings::None),
	  gradientEndShade(100.0),
	  gradientStartColor(CommonStrings::None),
	  gradientStartShade(100.0),
	  gradientCenterX(0.0),
	  gradientCenterY(0.0),
	  gradientType("linear"),
	  gradientName(""),
	  hasShadow(false),
	  shadowX(0.0),
	  shadowY(0.0),
	  shadowTrans(0.0),
	  measureDist(0.0),
	  patternName(""),
	  patternPath(""),
	  patternData(),
	  patternWidth(0.0),
	  patternHeight(0.0),
	  patternX(0.0),
	  patternY(0.0),
	  patternDim_W_in_Percent(false),
	  patternDim_H_in_Percent(false),
	  patternStretch(""),
	  hatchName(""),
	  hatchColor(""),
	  hatchDistance(0.0),
	  hatchRotation(0.0),
	  hatchStyle(""),
	  hatchSolidFill(false),
	  opacityName(""),
	  opacityEnd(1.0),
	  opacityStart(0.0)
	  {}
	QRectF							markerViewBox;
	QPainterPath					markerPath;
	QString							startMarkerName;
	double							startMarkerWidth;
	bool							startMarkerCentered;
	QString							endMarkerName;
	double							endMarkerWidth;
	bool							endMarkerCentered;
	double							stroke_dash_distance;
	int								stroke_dash_dots1;
	double							stroke_dash_dots1_length;
	int								stroke_dash_dots2;
	double							stroke_dash_dots2_length;
	QString							stroke_dash_style;
	QString							dashName;
	QString							CurrColorFill;
	QString							CurrColorStroke;
	QString							CurrColorText;
	QString							CurrColorBText;
	QString							CurrColorShadow;
	double							fillOpacity;
	double							strokeOpacity;
	double							LineW;
	QString							fontName;
	double							fontSize;
	double							textIndent;
	ParagraphStyle::AlignmentType	textAlign;    // 0 = left
	QString							textPos;
	QString							textOutline;
	bool							textUnderline;
	bool							textUnderlineWords;
	QString							textUnderlineColor;
	bool							textStrikeThrough;
	bool							textShadow;
	double							lineHeight;
	bool							absLineHeight;
	double							margin_top;
	double							margin_bottom;
	double							margin_left;
	double							margin_right;
	int								verticalAlignment;
	double							page_width;
	double							page_height;
	QString							page_layout_name;
	int								fill_type;		// 0 = none, 1 = solid, 2 = gradient, 3 = bitmap 4 = hatch
	int								stroke_type;	// 0 = none, 1 = solid, 2 = dashed
	double							gradientAngle;
	double							gradientBorder;
	QString							gradientEndColor;
	double							gradientEndShade;
	QString							gradientStartColor;
	double							gradientStartShade;
	double							gradientCenterX;
	double							gradientCenterY;
	QString							gradientType;
	QString							gradientName;
	bool							hasShadow;
	double							shadowX;
	double							shadowY;
	double							shadowTrans;
	double							measureDist;
	QString							patternName;
	QString							patternPath;
	QByteArray						patternData;
	double							patternWidth;
	double							patternHeight;
	double							patternX;
	double							patternY;
	bool							patternDim_W_in_Percent;
	bool							patternDim_H_in_Percent;
	QString							patternStretch;
	QString							hatchName;
	QString							hatchColor;
	double							hatchDistance;
	double							hatchRotation;
	QString							hatchStyle;
	bool							hatchSolidFill;
	QString							opacityName;
	double							opacityEnd;
	double							opacityStart;
};

class OdgPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	OdgPlug( ScribusDoc* doc, int flags );
	~OdgPlug();

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

private:
	struct DrawStyle
	{
		AttributeValue parentStyle;
		AttributeValue markerViewBox;
		AttributeValue markerPath;
		AttributeValue startMarkerName;
		AttributeValue startMarkerWidth;
		AttributeValue startMarkerCentered;
		AttributeValue endMarkerName;
		AttributeValue endMarkerWidth;
		AttributeValue endMarkerCentered;
		AttributeValue stroke_dash_distance;
		AttributeValue stroke_dash_dots1;
		AttributeValue stroke_dash_dots1_length;
		AttributeValue stroke_dash_dots2;
		AttributeValue stroke_dash_dots2_length;
		AttributeValue stroke_dash_style;
		AttributeValue dashName;
		AttributeValue fillMode;
		AttributeValue CurrColorFill;
		AttributeValue strokeMode;
		AttributeValue CurrColorStroke;
		AttributeValue fontColor;
		AttributeValue CurrColorShadow;
		AttributeValue fillOpacity;
		AttributeValue strokeOpacity;
		AttributeValue LineW;
		AttributeValue fontName;
		AttributeValue fontSize;
		AttributeValue textIndent;
		AttributeValue textAlign;
		AttributeValue textPos;
		AttributeValue textOutline;
		AttributeValue textUnderline;
		AttributeValue textUnderlineWords;
		AttributeValue textUnderlineColor;
		AttributeValue textStrikeThrough;
		AttributeValue textShadow;
		AttributeValue textBackgroundColor;
		AttributeValue lineHeight;
		AttributeValue margin_top;
		AttributeValue margin_bottom;
		AttributeValue margin_left;
		AttributeValue margin_right;
		AttributeValue verticalAlignment;
		AttributeValue page_width;
		AttributeValue page_height;
		AttributeValue page_layout_name;
		AttributeValue gradientAngle;
		AttributeValue gradientBorder;
		AttributeValue gradientEndColor;
		AttributeValue gradientEndShade;
		AttributeValue gradientStartColor;
		AttributeValue gradientStartShade;
		AttributeValue gradientCenterX;
		AttributeValue gradientCenterY;
		AttributeValue gradientType;
		AttributeValue gradientName;
		AttributeValue hasShadow;
		AttributeValue shadowX;
		AttributeValue shadowY;
		AttributeValue shadowTrans;
		AttributeValue measureDist;
		AttributeValue patternName;
		AttributeValue patternPath;
		AttributeValue patternData;
		AttributeValue patternWidth;
		AttributeValue patternHeight;
		AttributeValue patternX;
		AttributeValue patternY;
		AttributeValue patternStretch;
		AttributeValue hatchName;
		AttributeValue hatchColor;
		AttributeValue hatchDistance;
		AttributeValue hatchRotation;
		AttributeValue hatchStyle;
		AttributeValue hatchSolidFill;
		AttributeValue opacityName;
		AttributeValue opacityEnd;
		AttributeValue opacityStart;
	};

	bool convert(QString fn);
	bool parseStyleSheets(QString designMap);
	bool parseStyleSheetsXML(QDomDocument &designMapDom);
	bool parseDocReference(QString designMap);
	bool parseDocReferenceXML(QDomDocument &designMapDom);
	PageItem* parseObj(QDomElement &draw);
	PageItem* parseForm(QDomElement &e);
	PageItem* parseConnector(QDomElement &e);
	PageItem* parseCustomShape(QDomElement &e);
	PageItem* parseMeasure(QDomElement &e);
	PageItem* parseLine(QDomElement &e);
	PageItem* parseEllipse(QDomElement &e);
	PageItem* parseRect(QDomElement &e);
	PageItem* parsePolygon(QDomElement &e);
	PageItem* parsePolyline(QDomElement &e);
	PageItem* parsePath(QDomElement &e);
	PageItem* parseFrame(QDomElement &e);
	void parseText(QDomElement &elem, PageItem* item, ObjStyle& tmpOStyle);
	void insertChars(PageItem *item, QString &txt, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, int &posC);
	void applyCharacterStyle(CharStyle &tmpCStyle, ObjStyle &oStyle);
	void applyParagraphStyle(ParagraphStyle &tmpStyle, ObjStyle &oStyle);
	void parseTransform(const QString &transform, double *rotation, double *transX, double *transY);
	void parseTransform(FPointArray *composite, const QString &transform);
	void parseViewBox( const QDomElement& object, double *x, double *y, double *w, double *h );
	void appendPoints(FPointArray *composite, const QDomElement& object, bool closePath);
	void parseStyles(QDomElement &sp);
	QString getStyleName(QDomElement &e);
	void resovleStyle(ObjStyle &tmpOStyle, QString pAttrs);
	double parseUnit(const QString &unit);
	const char * getCoord( const char *ptr, double &number );
	bool parseEnhPath(const QString& svgPath, FPointArray &result, bool &fill, bool &stroke);
	double angleFromPoint(const QPointF &point);
	double radSweepAngle(double start, double stop, bool clockwise);
	double degSweepAngle(double start, double stop, bool clockwise);
	void arcTo(QPainterPath &path, QPointF startpoint, double rx, double ry, double startAngle, double sweepAngle);
	int arcToCurve(double rx, double ry, double startAngle, double sweepAngle, const QPointF & offset, QPointF * curvePoints);
	QString modifyColor(QString name, bool darker, int amount);
	QColor parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	QString constructFontName(QString fontBaseName, QString fontStyle);
	QPointF intersectBoundingRect(PageItem *item, QLineF gradientVector);
	PageItem* applyStartArrow(PageItem* item, ObjStyle &obState);
	PageItem* applyEndArrow(PageItem* ite, ObjStyle &obState);
	PageItem* groupObjects(QList<PageItem*> &GElements);
	void finishItem(PageItem* item, ObjStyle &obState);
	QList<PageItem*> Elements;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	bool interactive;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	MultiProgressDialog * progressDialog;
	bool cancel;
	QStringList importedColors;
	QStringList importedPatterns;
	bool firstPage;
	bool firstLayer;
	int pagecount;
	int mpagecount;
	double topMargin;
	double leftMargin;
	double rightMargin;
	double bottomMargin;
	double pgCols;
	double pgGap;
	QHash<QString, QString> m_fontMap;
	QHash<QString, DrawStyle> m_Styles;
	QHash<QString, int> m_Layers;


	FPointArray Coords;
	QHash<QString, QPainterPath> pathResources;
	ScZipHandler *uz;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
