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

#include <memory>

#include <QDomDocument>
#include <QDomElement>
#include <QtGlobal>
#include <QList>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QTransform>

#include "commonstrings.h"
#include "fpointarray.h"
#include "pageitem.h"
#include "pluginapi.h"
#include "sccolor.h"
#include "scribusstructs.h"

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;
class ScZipHandler;

class ObjStyle
{
public:
	ObjStyle() {}

	QRectF							markerViewBox;
	QPainterPath					markerPath;
	QString							startMarkerName;
	double							startMarkerWidth { 0.0 };
	bool							startMarkerCentered { false };
	QString							endMarkerName;
	double							endMarkerWidth { 0.0 };
	bool							endMarkerCentered { false };
	double							stroke_dash_distance { 0.0 };
	int								stroke_dash_dots1 { 1 };
	double							stroke_dash_dots1_length { 0.0 };
	int								stroke_dash_dots2 { 1 };
	double							stroke_dash_dots2_length { 0.0 };
	QString							stroke_dash_style { "solid" };
	QString							dashName;
	QString							currColorFill { CommonStrings::None };
	QString							currColorStroke { CommonStrings::None };
	QString							currColorText { "Black" };
	QString							currColorBText { CommonStrings::None };
	QString							currColorShadow { "Black" };
	double							fillOpacity { 0.0 };
	double							strokeOpacity { 0.0 };
	double							LineW { 0.0 };
	QString							fontName;
	double							fontSize{ 10.0 };
	double							textIndent { 0.0 };
	ParagraphStyle::AlignmentType	textAlign { ParagraphStyle::LeftAligned };    // 0 = left
	QString							textPos;
	QString							textOutline;
	bool							textUnderline { false };
	bool							textUnderlineWords { false };
	QString							textUnderlineColor { CommonStrings::None };
	bool							textStrikeThrough { false };
	bool							textShadow { false };
	double							lineHeight { 1.0 };
	bool							absLineHeight { false };
	double							margin_top { 0.0 };
	double							margin_bottom { 0.0 };
	double							margin_left { 0.0 };
	double							margin_right { 0.0 };
	int								verticalAlignment{ 0 };
	double							page_width { 595.0 };
	double							page_height { 841.0 };
	QString							page_layout_name;
	int								fill_type { 0 };		// 0 = none, 1 = solid, 2 = gradient, 3 = bitmap 4 = hatch
	int								stroke_type { 0 };	// 0 = none, 1 = solid, 2 = dashed
	double							gradientAngle { 0.0 };
	double							gradientBorder { 0.0 };
	QString							gradientEndColor { CommonStrings::None };
	double							gradientEndShade { 100.0 };
	QString							gradientStartColor { CommonStrings::None };
	double							gradientStartShade { 100.0 };
	double							gradientCenterX { 0.0 };
	double							gradientCenterY { 0.0 };
	QString							gradientType { "linear" };
	QString							gradientName;
	bool							hasShadow { false };
	double							shadowX { 0.0 };
	double							shadowY { 0.0 };
	double							shadowTrans { 0.0 };
	double							measureDist { 0.0 };
	QString							patternName;
	QString							patternPath;
	QByteArray						patternData;
	double							patternWidth { 0.0 };
	double							patternHeight { 0.0 };
	double							patternX { 0.0 };
	double							patternY { 0.0 };
	bool							patternDim_W_in_Percent { false };
	bool							patternDim_H_in_Percent { false };
	QString							patternStretch;
	QString							hatchName;
	QString							hatchColor;
	double							hatchDistance { 0.0 };
	double							hatchRotation { 0.0 };
	QString							hatchStyle;
	bool							hatchSolidFill { false };
	QString							opacityName;
	double							opacityEnd { 1.0 };
	double							opacityStart { 0.0 };
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
	bool import(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);

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
		AttributeValue currColorFill;
		AttributeValue strokeMode;
		AttributeValue currColorStroke;
		AttributeValue fontColor;
		AttributeValue currColorShadow;
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

	bool convert(const QString& fn);
	bool parseStyleSheets(const QString& designMap);
	bool parseStyleSheetsXML(QDomDocument &designMapDom);
	bool parseDocReference(const QString& designMap);
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
	void resovleStyle(ObjStyle &tmpOStyle, const QString& pAttrs);
	double parseUnit(const QString &unit);
	const char * getCoord( const char *ptr, double &number );
	bool parseEnhPath(const QString& svgPath, FPointArray &result, bool &fill, bool &stroke);
	double angleFromPoint(const QPointF &point);
	double radSweepAngle(double start, double stop, bool clockwise);
	double degSweepAngle(double start, double stop, bool clockwise);
	void arcTo(QPainterPath &path, QPointF startpoint, double rx, double ry, double startAngle, double sweepAngle);
	int arcToCurve(double rx, double ry, double startAngle, double sweepAngle, const QPointF & offset, QPointF * curvePoints);
	QString modifyColor(const QString& name, bool darker, int amount);
	QString parseColor( const QString &s );
	QString constructFontName(const QString& fontBaseName, const QString& fontStyle);
	QPointF intersectBoundingRect(PageItem *item, QLineF gradientVector);
	PageItem* applyStartArrow(PageItem* item, ObjStyle &obState);
	PageItem* applyEndArrow(PageItem* ite, ObjStyle &obState);
	PageItem* groupObjects(QList<PageItem*> &GElements);
	void finishItem(PageItem* item, ObjStyle &obState);

	QList<PageItem*> Elements;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };
	bool interactive { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	MultiProgressDialog * progressDialog { nullptr };
	bool cancel { false };
	QStringList importedColors;
	QStringList importedPatterns;
	bool firstPage { true };
	bool firstLayer { true };
	int pagecount { 0 };
	int mpagecount { 0 };
	double topMargin { 0.0 };
	double leftMargin { 0.0 };
	double rightMargin { 0.0 };
	double bottomMargin { 0.0 };
	double pgCols { 0.0 };
	double pgGap { 0.0 };
	QHash<QString, QString> m_fontMap;
	QHash<QString, DrawStyle> m_Styles;
	QHash<QString, int> m_Layers;

	FPointArray Coords;
	QHash<QString, QPainterPath> pathResources;
	std::unique_ptr<ScZipHandler> uz;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
