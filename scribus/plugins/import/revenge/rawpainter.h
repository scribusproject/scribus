/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef RAWPAINTER_H
#define RAWPAINTER_H

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

class ScribusDoc;
class Selection;

#if HAVE_REVENGE
	#include <librevenge/librevenge.h>
	#include <librevenge-stream/librevenge-stream.h>
	#include <librevenge-generators/librevenge-generators.h>

struct RawPainterPrivate;

class RawPainter : public librevenge::RVNGDrawingInterface
{
public:
	RawPainter(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel, QString fTyp);
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
	void applyArrows(PageItem* ite);
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
	QString fileType;
};

class RawPainterPres : public librevenge::RVNGRawTextGenerator
{
public:
	RawPainterPres(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel, QString fTyp);
	~RawPainterPres();
	void startDocument(const librevenge::RVNGPropertyList &propList);
	void endDocument();
	void setDocumentMetaData(const librevenge::RVNGPropertyList &propList);
	void defineEmbeddedFont(const librevenge::RVNGPropertyList &propList);
	void definePageStyle(const librevenge::RVNGPropertyList &propList);
	void openPageSpan(const librevenge::RVNGPropertyList &propList);
	void closePageSpan();
	void openHeader(const librevenge::RVNGPropertyList &propList);
	void closeHeader();
	void openFooter(const librevenge::RVNGPropertyList &propList);
	void closeFooter();
	void defineParagraphStyle(const librevenge::RVNGPropertyList &propList);
	void openParagraph(const librevenge::RVNGPropertyList &propList);
	void closeParagraph();
	void defineCharacterStyle(const librevenge::RVNGPropertyList &propList);
	void openSpan(const librevenge::RVNGPropertyList &propList);
	void closeSpan();
	void openLink(const librevenge::RVNGPropertyList &propList);
	void closeLink();
	void defineSectionStyle(const librevenge::RVNGPropertyList &propList);
	void openSection(const librevenge::RVNGPropertyList &propList);
	void closeSection();
	void insertTab();
	void insertSpace();
	void insertText(const librevenge::RVNGString &text);
	void insertLineBreak();
	void insertField(const librevenge::RVNGPropertyList &propList);
	void openOrderedListLevel(const librevenge::RVNGPropertyList &propList);
	void openUnorderedListLevel(const librevenge::RVNGPropertyList &propList);
	void closeOrderedListLevel();
	void closeUnorderedListLevel();
	void openListElement(const librevenge::RVNGPropertyList &propList);
	void closeListElement();
	void openFootnote(const librevenge::RVNGPropertyList &propList);
	void closeFootnote();
	void openEndnote(const librevenge::RVNGPropertyList &propList);
	void closeEndnote();
	void openComment(const librevenge::RVNGPropertyList &propList);
	void closeComment();
	void openTextBox(const librevenge::RVNGPropertyList &propList);
	void closeTextBox();
	void openTable(const librevenge::RVNGPropertyList &propList);
	void openTableRow(const librevenge::RVNGPropertyList &propList);
	void closeTableRow();
	void openTableCell(const librevenge::RVNGPropertyList &propList);
	void closeTableCell();
	void insertCoveredTableCell(const librevenge::RVNGPropertyList &propList);
	void closeTable();
	void openFrame(const librevenge::RVNGPropertyList &propList);
	void closeFrame();
	void openGroup(const librevenge::RVNGPropertyList &propList);
	void closeGroup();
	void defineGraphicStyle(const librevenge::RVNGPropertyList &propList);
	void drawRectangle(const librevenge::RVNGPropertyList &propList);
	void drawEllipse(const librevenge::RVNGPropertyList &propList);
	void drawPolygon(const librevenge::RVNGPropertyList &propList);
	void drawPolyline(const librevenge::RVNGPropertyList &propList);
	void drawPath(const librevenge::RVNGPropertyList &propList);
	void drawConnector(const librevenge::RVNGPropertyList &propList);
	void insertBinaryObject(const librevenge::RVNGPropertyList &propList);
	void insertEquation(const librevenge::RVNGPropertyList &propList);
private:
	QList<PageItem*> *mElements;
	RawPainter *painter;
	QList<QList<PageItem*> > pageElements;
	ScribusDoc* mDoc;
};

#else
	#include <libwpd-stream/libwpd-stream.h>
	#include <libwpd/libwpd.h>
	#include <libwpg/libwpg.h>


class RawPainter : public libwpg::WPGPaintInterface
{
public:
	RawPainter(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel, QString fTyp);
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
	void applyArrows(PageItem* ite);
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
	QString fileType;
};
#endif

#endif
