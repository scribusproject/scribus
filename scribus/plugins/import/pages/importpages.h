/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Mon Dez 30 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTPAGES_H
#define IMPORTPAGES_H

#include <QDomDocument>
#include <QDomElement>
#include <QList>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <QTransform>

#include "fpointarray.h"
#include "pageitem.h"
#include "pluginapi.h"
#include "sccolor.h"
#include "scribusstructs.h"

class MultiProgressDialog;
class ScribusDoc;
class ScZipHandler;
class Selection;
class TransactionSettings;

class PagesPlug : public QObject
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
	PagesPlug( ScribusDoc* doc, int flags );
	~PagesPlug();

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
	struct ObjStyle
	{
		AttributeValue parentStyle;
		AttributeValue CurrColorFill;
		AttributeValue CurrColorStroke;
		AttributeValue fillOpacity;
		AttributeValue strokeOpacity;
		AttributeValue opacity;
		AttributeValue LineW;
		AttributeValue CapStyle;
		AttributeValue JoinStyle;
	};
	struct ChrStyle
	{
		AttributeValue parentStyle;
		AttributeValue fontName;
		AttributeValue fontSize;
		AttributeValue fontColor;
		AttributeValue backColor;
	};
	struct ParStyle
	{
		AttributeValue parentStyle;
		AttributeValue fontName;
		AttributeValue fontSize;
		AttributeValue fontColor;
		AttributeValue justification;
	};
	struct LayoutStyle
	{
		AttributeValue parentStyle;
		AttributeValue Extra;
		AttributeValue TExtra;
		AttributeValue BExtra;
		AttributeValue RExtra;
		AttributeValue TextColumnCount;
		AttributeValue TextColumnGutter;
	};

	struct ObjState
	{
		QPainterPath currentPath;
		bool currentPathClosed {false};
		QPainterPath clipPath;
		QTransform transform;
		QString CurrColorFill {CommonStrings::None};
		double fillOpacity {0.0};
		QString CurrColorStroke {CommonStrings::None};
		double strokeOpacity {0.0};
		double LineW {1.0};
		VGradient currentGradient;
		QPointF gradientStart;
		QPointF gradientEnd;
		QPointF gradientFocus;
		double gradientScale {1.0};
		int fillGradientTyp {0};
		QString imagePath;
		QString patternName;
		VGradient gradientMask;
		QPointF maskStart;
		QPointF maskEnd;
		QPointF maskFocus;
		double maskScale;
		int maskTyp {0};
		QString patternMask;
		VGradient gradientStroke;
		QPointF strokeStart;
		QPointF strokeEnd;
		QPointF strokeFocus;
		double strokeScale;
		int strokeTyp {0};
		QString patternStroke;
		int itemType {0};
		Qt::PenCapStyle CapStyle {Qt::FlatCap};
		Qt::PenJoinStyle JoinStyle {Qt::MiterJoin};
		QVector<double> DashPattern;
		double DashOffset {0.0};
		double xPos {0.0};
		double yPos {0.0};
		double width {0.0};
		double height {0.0};
		double rotation {0.0};
		QString styleRef;
		QString layoutStyleRef;
	};
	struct StyleSheet
	{
		QHash<QString, ObjStyle> m_objStyles;
		QHash<QString, ParStyle> m_parStyles;
		QHash<QString, ChrStyle> m_chrStyles;
		QHash<QString, LayoutStyle> m_layoutStyles;
	};

	bool convert(const QString& fn);
	bool parseDocReference(const QString& designMap, bool compressed);
	void parseStyleSheets(QDomElement &drawPag);
	void parsePageReference(QDomElement &drawPag);
	PageItem *parseObjReference(QDomElement &draw);
	void applyParagraphAttrs(ParagraphStyle &newStyle, CharStyle &tmpCStyle, const QString& pAttrs);
	void applyCharAttrs(CharStyle &tmpCStyle, const QString& pAttrs);
	void finishItem(PageItem* item, ObjState &obState);

	PageItem* addClip(PageItem* retObj, ObjState& obState);

	QList<PageItem*> Elements;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 0.0 };
	double docHeight { 0.0 };
	bool interactive { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	MultiProgressDialog * progressDialog { nullptr };
	bool cancel { false };
	QStringList importedColors;
	QStringList importedPatterns;
	bool firstPage { false };
	int pagecount { 0 };
	int mpagecount { 0 };
	double topMargin { 0.0 };
	double leftMargin { 0.0 };
	double rightMargin { 0.0 };
	double bottomMargin { 0.0 };
	double pgCols { 0.0 };
	double pgGap { 0.0 };
	QString papersize;
	QHash<int, QString> imageResources;
	QHash<QString, StyleSheet> m_StyleSheets;
	QString m_currentStyleSheet;

	FPointArray Coords;
	QHash<QString, QPainterPath> pathResources;
	ScZipHandler *uz { nullptr };

public slots:
	void cancelRequested() { cancel = true; }
};
#endif
