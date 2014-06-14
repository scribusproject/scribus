/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTWPG_H
#define IMPORTWPG_H

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

#if HAVE_REVENGE
	#include <librevenge/librevenge.h>
	#include <librevenge-stream/librevenge-stream.h>
	#include <librevenge-generators/librevenge-generators.h>
	#include <libmspub/libmspub.h>

struct RawPainterPrivate;

class RawPainter : public librevenge::RVNGDrawingInterface
{
public:
	RawPainter(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel);
	~RawPainter();

	void startDocument(const librevenge::RVNGPropertyList &propList);
	void endDocument();
	void setDocumentMetaData(const librevenge::RVNGPropertyList &propList);
	void defineEmbeddedFont(const librevenge::RVNGPropertyList &propList);
	void startPage(const librevenge::RVNGPropertyList &propList);
	void endPage();
	void startMasterPage(const librevenge::RVNGPropertyList &propList);
	void endMasterPage();
	void startLayer(const librevenge::RVNGPropertyList &propList);
	void endLayer();
	void startEmbeddedGraphics(const librevenge::RVNGPropertyList &propList);
	void endEmbeddedGraphics();

	void openGroup(const librevenge::RVNGPropertyList &propList);
	void closeGroup();

	void setStyle(const librevenge::RVNGPropertyList &propList);

	void drawRectangle(const librevenge::RVNGPropertyList &propList);
	void drawEllipse(const librevenge::RVNGPropertyList &propList);
	void drawPolyline(const librevenge::RVNGPropertyList &propList);
	void drawPolygon(const librevenge::RVNGPropertyList &propList);
	void drawPath(const librevenge::RVNGPropertyList &propList);
	void drawGraphicObject(const librevenge::RVNGPropertyList &propList);
	void drawConnector(const librevenge::RVNGPropertyList &propList);
	void startTextObject(const librevenge::RVNGPropertyList &propList);
	void endTextObject();

	void startTableObject(const librevenge::RVNGPropertyList &propList);
	void openTableRow(const librevenge::RVNGPropertyList &propList);
	void closeTableRow();
	void openTableCell(const librevenge::RVNGPropertyList &propList);
	void closeTableCell();
	void insertCoveredTableCell(const librevenge::RVNGPropertyList &propList);
	void endTableObject();

	void openOrderedListLevel(const librevenge::RVNGPropertyList &propList);
	void closeOrderedListLevel();

	void openUnorderedListLevel(const librevenge::RVNGPropertyList &propList);
	void closeUnorderedListLevel();
	void openListElement(const librevenge::RVNGPropertyList &propList);
	void closeListElement();

	void defineParagraphStyle(const librevenge::RVNGPropertyList &propList);
	void openParagraph(const librevenge::RVNGPropertyList &propList);
	void closeParagraph();

	void defineCharacterStyle(const librevenge::RVNGPropertyList &propList);
	void openSpan(const librevenge::RVNGPropertyList &propList);
	void closeSpan();

	void openLink(const librevenge::RVNGPropertyList &propList);
	void closeLink();

	void insertTab();
	void insertSpace();
	void insertText(const librevenge::RVNGString &text);
	void insertLineBreak();
	void insertField(const librevenge::RVNGPropertyList &propList);
	double valueAsPoint(const librevenge::RVNGProperty *prop);
	QString constructFontName(QString fontBaseName, QString fontStyle);
	double fromPercentage(const QString &s );
	QColor  parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void insertImage(PageItem* ite, QString imgExt, QByteArray &imageData);
	void applyFill(PageItem* ite);
	void applyShadow(PageItem* ite);
	void applyFlip(PageItem* ite);
	void recolorItem(PageItem* ite, QString efVal);
	void finishItem(PageItem* ite);
private:
	RawPainterPrivate *m_pImpl;
	ScribusDoc* m_Doc;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	QList<PageItem*> *Elements;
	QStringList *importedColors;
	QStringList *importedPatterns;
	Selection* tmpSel;
	struct groupEntry
	{
		QList<PageItem*> Items;
		FPointArray clip;
	};
	QStack<groupEntry> groupStack;
	double LineW;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade;
	double CurrFillShade;
	double CurrStrokeTrans;
	double CurrFillTrans;
	FPointArray Coords;
	bool fillrule;
	double gradientAngle;
	bool isGradient;
	VGradient currentGradient;
	QString gradColor1Str;
	QColor gradColor1;
	double gradColor1Trans;
	QString gradColor2Str;
	QColor gradColor2;
	double gradColor2Trans;
	QVector<double> dashArray;
	Qt::PenJoinStyle lineJoin;
	Qt::PenCapStyle lineEnd;
	bool firstPage;
	QString baseLayer;
	int actPage;
	librevenge::RVNGPropertyList m_style;
	PageItem *actTextItem;
	ParagraphStyle textStyle;
	CharStyle textCharStyle;
	double m_linespace;
	double m_maxFontSize;
	bool lineSpSet;
	bool lineSpIsPT;
	int importerFlags;
	bool doProcessing;

};

#else
	#include <libwpd-stream/libwpd-stream.h>
	#include <libwpd/libwpd.h>
	#include <libwpg/libwpg.h>
	#include <libmspub/libmspub.h>


class RawPainter : public libwpg::WPGPaintInterface
{
public:
	RawPainter(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel);
	void startGraphics(const ::WPXPropertyList &propList);
	void endGraphics();
	void startLayer(const ::WPXPropertyList &propList);
	void endLayer();
	void startEmbeddedGraphics(const ::WPXPropertyList &propList);
	void endEmbeddedGraphics();
	void setStyle(const ::WPXPropertyList &propList, const ::WPXPropertyListVector &gradient);
	void drawRectangle(const ::WPXPropertyList &propList);
	void drawEllipse(const ::WPXPropertyList &propList);
	void drawPolyline(const ::WPXPropertyListVector &vertices);
	void drawPolygon(const ::WPXPropertyListVector &vertices);
	void drawPath(const ::WPXPropertyListVector &path);
	void drawGraphicObject(const ::WPXPropertyList &propList, const ::WPXBinaryData &binaryData);
	void startTextObject(const ::WPXPropertyList &propList, const ::WPXPropertyListVector &path);
	void endTextObject();
	void startTextLine(const ::WPXPropertyList &propList);
	void endTextLine();
	void startTextSpan(const ::WPXPropertyList &propList);
	void endTextSpan();
	void insertText(const ::WPXString &str);
	QString constructFontName(QString fontBaseName, QString fontStyle);
	double valueAsPoint(const WPXProperty *prop);
	double fromPercentage(const QString &s );
	QColor  parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void insertImage(PageItem* ite, QString imgExt, QByteArray &imageData);
	void applyFill(PageItem* ite);
	void applyShadow(PageItem* ite);
	void applyFlip(PageItem* ite);
	void recolorItem(PageItem* ite, QString efVal);
	void finishItem(PageItem* ite);

	ScribusDoc* m_Doc;
	Selection* tmpSel;
	QList<PageItem*> *Elements;
	struct groupEntry
	{
		QList<PageItem*> Items;
		FPointArray clip;
	};
	QStack<groupEntry> groupStack;
	QStringList *importedColors;
	QStringList *importedPatterns;
	double LineW;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade;
	double CurrFillShade;
	double CurrStrokeTrans;
	double CurrFillTrans;
	FPointArray Coords;
	bool fillrule;
	double gradientAngle;
	bool isGradient;
	VGradient currentGradient;
	QString gradColor1Str;
	QColor gradColor1;
	double gradColor1Trans;
	QString gradColor2Str;
	QColor gradColor2;
	double gradColor2Trans;
	QVector<double> dashArray;
	Qt::PenJoinStyle lineJoin;
	Qt::PenCapStyle lineEnd;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	int importerFlags;
	bool firstPage;
	QString baseLayer;
	int actPage;
	WPXPropertyList m_style;
	PageItem *actTextItem;
	ParagraphStyle textStyle;
	CharStyle textCharStyle;
	double m_linespace;
	double m_maxFontSize;
	bool lineSpSet;
	bool lineSpIsPT;
	bool doProcessing;
};
#endif

//! \brief PUB importer plugin
class PubPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Pub importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	PubPlug( ScribusDoc* doc, int flags );
	~PubPlug();

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

#endif
