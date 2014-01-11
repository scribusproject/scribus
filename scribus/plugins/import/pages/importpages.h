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

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
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

class AttributeValue
{
	public:
		AttributeValue() : valid(false),  value("")
		{
		}
		AttributeValue(QString val) : valid(true),  value(val)
		{
		}
		bool valid;
		QString value;
};

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
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

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
		bool currentPathClosed;
		QPainterPath clipPath;
		QTransform transform;
		QString CurrColorFill;
		double fillOpacity;
		QString CurrColorStroke;
		double strokeOpacity;
		double LineW;
		VGradient currentGradient;
		QPointF gradientStart;
		QPointF gradientEnd;
		QPointF gradientFocus;
		double gradientScale;
		int fillGradientTyp;
		QString imagePath;
		QString patternName;
		VGradient gradientMask;
		QPointF maskStart;
		QPointF maskEnd;
		QPointF maskFocus;
		double maskScale;
		int maskTyp;
		QString patternMask;
		VGradient gradientStroke;
		QPointF strokeStart;
		QPointF strokeEnd;
		QPointF strokeFocus;
		double strokeScale;
		int strokeTyp;
		QString patternStroke;
		int itemType;
		Qt::PenCapStyle CapStyle;
		Qt::PenJoinStyle JoinStyle;
		QVector<double> DashPattern;
		double DashOffset;
		double xPos;
		double yPos;
		double width;
		double height;
		double rotation;
		QString styleRef;
		QString layoutStyleRef;
	};
	bool convert(QString fn);
	bool parseDocReference(QString designMap, bool compressed);
	void parseStyleSheets(QDomElement &drawPag);
	void parsePageReference(QDomElement &drawPag);
	PageItem *parseObjReference(QDomElement &draw);
	void applyParagraphAttrs(ParagraphStyle &newStyle, CharStyle &tmpCStyle, QString pAttrs);
	void applyCharAttrs(CharStyle &tmpCStyle, QString pAttrs);
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
	double topMargin;
	double leftMargin;
	double rightMargin;
	double bottomMargin;
	double pgCols;
	double pgGap;
	QString papersize;
	QHash<int, QString> imageResources;
	QHash<QString, ObjStyle> m_objStyles;
	QHash<QString, ParStyle> m_parStyles;
	QHash<QString, ChrStyle> m_chrStyles;
	QHash<QString, LayoutStyle> m_layoutStyles;

	PageItem* addClip(PageItem* retObj, ObjState &obState);

	FPointArray Coords;
	QHash<QString, QPainterPath> pathResources;
	ScZipHandler *uz;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
