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

#if 0

#include <QObject>
#include <QString>

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "vgradient.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QVector>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief FH importer plugin
class PagesPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Fh importer window.
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
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

private:
	bool convert(QString fn);

	QList<PageItem*> Elements;
	double baseX, baseY;
	double docWidth;
	double docHeight;

	QStringList importedColors;
	QStringList importedPatterns;

	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;

public slots:
	void cancelRequested() { cancel = true; }
};
#else

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
	int pagecount;
	int mpagecount;
	double topMargin;
	double leftMargin;
	double rightMargin;
	double bottomMargin;
	double pgCols;
	double pgGap;
	QString papersize;
	QHash<int, QString> imageResources;
	QHash<QString, StyleSheet> m_StyleSheets;
	QString m_currentStyleSheet;

	PageItem* addClip(PageItem* retObj, ObjState &obState);

	FPointArray Coords;
	QHash<QString, QPainterPath> pathResources;
	ScZipHandler *uz;

public slots:
	void cancelRequested() { cancel = true; }
};
#endif
#endif
