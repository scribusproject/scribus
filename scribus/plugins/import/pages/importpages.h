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
	struct StyleSheet
	{
		QHash<QString, ObjStyle> m_objStyles;
		QHash<QString, ParStyle> m_parStyles;
		QHash<QString, ChrStyle> m_chrStyles;
		QHash<QString, LayoutStyle> m_layoutStyles;
	};

	bool convert(QString fn);
	bool parseDocReference(QString designMap, bool compressed);
	void parseStyleSheets(QDomElement &drawPag);
	void parsePageReference(QDomElement &drawPag);
	PageItem *parseObjReference(QDomElement &draw);
	void applyParagraphAttrs(ParagraphStyle &newStyle, CharStyle &tmpCStyle, QString pAttrs);
	void applyCharAttrs(CharStyle &tmpCStyle, QString pAttrs);
	void finishItem(PageItem* item, ObjState &obState);
	QList<PageItem*> m_Elements;
	double m_baseX, m_baseY;
	double m_docWidth;
	double m_docHeight;
	bool m_interactive;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	QStringList m_importedColors;
	QStringList m_importedPatterns;
	bool m_firstPage;
	int m_pagecount;
	int m_mpagecount;
	double m_topMargin;
	double m_leftMargin;
	double m_rightMargin;
	double m_bottomMargin;
	double m_pgCols;
	double m_pgGap;
	QString m_papersize;
	QHash<int, QString> m_imageResources;
	QHash<QString, StyleSheet> m_StyleSheets;
	QString m_currentStyleSheet;

	PageItem* addClip(PageItem* retObj, ObjState &obState);

	FPointArray m_Coords;
	QHash<QString, QPainterPath> m_pathResources;
	ScZipHandler *m_uz;

public slots:
	void cancelRequested() { m_cancel = true; }
};
#endif
#endif
