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

#include "fpointarray.h"
#include "pageitem.h"
#include "pluginapi.h"
#include "sccolor.h"
#include "vgradient.h"

#include <QList>
#include <QMultiMap>
#include <QTransform>
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
	RawPainter(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel, const QString& fTyp);
	~RawPainter() override;

	void startDocument(const librevenge::RVNGPropertyList &propList) override;
	void endDocument() override;
	void setDocumentMetaData(const librevenge::RVNGPropertyList &propList) override;
	void defineEmbeddedFont(const librevenge::RVNGPropertyList &propList) override;
	void startPage(const librevenge::RVNGPropertyList &propList) override;
	void endPage() override;
	void startMasterPage(const librevenge::RVNGPropertyList &propList) override;
	void endMasterPage() override;
	void startLayer(const librevenge::RVNGPropertyList &propList) override;
	void endLayer() override;
	void startEmbeddedGraphics(const librevenge::RVNGPropertyList &propList) override;
	void endEmbeddedGraphics() override;

	void openGroup(const librevenge::RVNGPropertyList &propList) override;
	void closeGroup() override;

	void setStyle(const librevenge::RVNGPropertyList &propList) override;

	void drawRectangle(const librevenge::RVNGPropertyList &propList) override;
	void drawEllipse(const librevenge::RVNGPropertyList &propList) override;
	void drawPolyline(const librevenge::RVNGPropertyList &propList) override;
	void drawPolygon(const librevenge::RVNGPropertyList &propList) override;
	void drawPath(const librevenge::RVNGPropertyList &propList) override;
	void drawGraphicObject(const librevenge::RVNGPropertyList &propList) override;
	void drawConnector(const librevenge::RVNGPropertyList &propList) override;
	void startTextObject(const librevenge::RVNGPropertyList &propList) override;
	void endTextObject() override;

	void startTableObject(const librevenge::RVNGPropertyList &propList) override;
	void openTableRow(const librevenge::RVNGPropertyList &propList) override;
	void closeTableRow() override;
	void openTableCell(const librevenge::RVNGPropertyList &propList) override;
	void closeTableCell() override;
	void insertCoveredTableCell(const librevenge::RVNGPropertyList &propList) override;
	void endTableObject() override;

	void openOrderedListLevel(const librevenge::RVNGPropertyList &propList) override;
	void closeOrderedListLevel() override;

	void openUnorderedListLevel(const librevenge::RVNGPropertyList &propList) override;
	void closeUnorderedListLevel() override;
	void openListElement(const librevenge::RVNGPropertyList &propList) override;
	void closeListElement() override;

	void defineParagraphStyle(const librevenge::RVNGPropertyList &propList) override;
	void openParagraph(const librevenge::RVNGPropertyList &propList) override;
	void closeParagraph() override;

	void defineCharacterStyle(const librevenge::RVNGPropertyList &propList) override;
	void openSpan(const librevenge::RVNGPropertyList &propList) override;
	void closeSpan() override;

	void openLink(const librevenge::RVNGPropertyList &propList) override;
	void closeLink() override;

	void insertTab() override;
	void insertSpace() override;
	void insertText(const librevenge::RVNGString &text) override;
	void insertLineBreak() override;
	void insertField(const librevenge::RVNGPropertyList &propList) override;

	double valueAsPoint(const librevenge::RVNGProperty *prop) const;
	QString constructFontName(const QString& fontBaseName, const QString& fontStyle);
	double fromPercentage(const QString &s) const;
	QString parseColor(const QString &s);
	void insertImage(PageItem* ite, const QString& imgExt, QByteArray &imageData);
	void applyFill(PageItem* ite);
	void applyShadow(PageItem* ite);
	void applyFlip(PageItem* ite) const;
	void recolorItem(PageItem* ite, const QString& efVal);
	void applyArrows(PageItem* ite);
	void applyStartArrow(PageItem* ite);
	void applyEndArrow(PageItem* ite);
	void finishItem(PageItem* ite);

private:
	RawPainterPrivate *m_pImpl { nullptr };
	ScribusDoc* m_doc { nullptr };
	double m_baseX { 0.0 };
	double m_baseY { 0.0 };
	double m_docWidth { 0.0 };
	double m_docHeight { 0.0 };
	QList<PageItem*> *m_elements { nullptr };
	QStringList *m_importedColors { nullptr };
	QStringList *m_importedPatterns { nullptr };
	Selection* m_tmpSel { nullptr };
	struct groupEntry
	{
		QList<PageItem*> Items;
		FPointArray clip;
	};
	QStack<groupEntry> m_groupStack;

	double m_lineWidth { 1.0 };
	QString m_currColorFill { "Black" };
	QString m_currColorStroke { "Black" };
	double  m_currStrokeShade { 100.0 };
	double m_currFillShade { 100.0 };
	double m_currStrokeTrans { 0.0 };
	double m_currFillTrans { 0.0 };
	FPointArray m_coords;
	bool m_fillRule { true };
	double m_gradientAngle { 0.0 };
	bool m_isGradient { false };
	VGradient m_currentGradient;
	QString m_gradColor1Str;
	QColor m_gradColor1;
	double m_gradColor1Trans { 1.0 };
	QString m_gradColor2Str;
	QColor m_gradColor2;
	double m_gradColor2Trans { 1.0 };
	QVector<double> m_dashArray;
	Qt::PenJoinStyle m_lineJoin { Qt::MiterJoin };
	Qt::PenCapStyle m_lineEnd { Qt::FlatCap };
	bool m_firstPage { true };
	int  m_currentPage { 0 };
	QString m_baseLayer;
	librevenge::RVNGPropertyList m_style;
	PageItem *m_currTextItem { nullptr };
	ParagraphStyle m_textStyle;
	CharStyle m_textCharStyle;
	double m_linespace { 1.0 };
	double m_maxFontSize { 1.0 };
	bool m_lineSpSet { false };
	bool m_lineSpIsPT { false };
	int m_importerFlags;
	bool m_doProcessing { true };
	QString m_fileType;
};

class RawPainterPres : public librevenge::RVNGRawTextGenerator
{
public:
	RawPainterPres(ScribusDoc* Doc, double x, double y, double w, double h, int iflags, QList<PageItem*> *Elem, QStringList *iColors, QStringList *iPatterns, Selection* tSel, const QString& fTyp);
	~RawPainterPres() override;

	void startDocument(const librevenge::RVNGPropertyList &propList) override;
	void endDocument() override;
	void setDocumentMetaData(const librevenge::RVNGPropertyList &propList) override;
	void defineEmbeddedFont(const librevenge::RVNGPropertyList &propList) override;
	void definePageStyle(const librevenge::RVNGPropertyList &propList) override;
	void openPageSpan(const librevenge::RVNGPropertyList &propList) override;
	void closePageSpan() override;
	void openHeader(const librevenge::RVNGPropertyList &propList) override;
	void closeHeader() override;
	void openFooter(const librevenge::RVNGPropertyList &propList) override;
	void closeFooter() override;
	void defineParagraphStyle(const librevenge::RVNGPropertyList &propList) override;
	void openParagraph(const librevenge::RVNGPropertyList &propList) override;
	void closeParagraph() override;
	void defineCharacterStyle(const librevenge::RVNGPropertyList &propList) override;
	void openSpan(const librevenge::RVNGPropertyList &propList) override;
	void closeSpan() override;
	void openLink(const librevenge::RVNGPropertyList &propList) override;
	void closeLink() override;
	void defineSectionStyle(const librevenge::RVNGPropertyList &propList) override;
	void openSection(const librevenge::RVNGPropertyList &propList) override;
	void closeSection() override;
	void insertTab() override;
	void insertSpace() override;
	void insertText(const librevenge::RVNGString &text) override;
	void insertLineBreak() override;
	void insertField(const librevenge::RVNGPropertyList &propList) override;
	void openOrderedListLevel(const librevenge::RVNGPropertyList &propList) override;
	void openUnorderedListLevel(const librevenge::RVNGPropertyList &propList) override;
	void closeOrderedListLevel() override;
	void closeUnorderedListLevel() override;
	void openListElement(const librevenge::RVNGPropertyList &propList) override;
	void closeListElement() override;
	void openFootnote(const librevenge::RVNGPropertyList &propList) override;
	void closeFootnote() override;
	void openEndnote(const librevenge::RVNGPropertyList &propList) override;
	void closeEndnote() override;
	void openComment(const librevenge::RVNGPropertyList &propList) override;
	void closeComment() override;
	void openTextBox(const librevenge::RVNGPropertyList &propList) override;
	void closeTextBox() override;
	void openTable(const librevenge::RVNGPropertyList &propList) override;
	void openTableRow(const librevenge::RVNGPropertyList &propList) override;
	void closeTableRow() override;
	void openTableCell(const librevenge::RVNGPropertyList &propList) override;
	void closeTableCell() override;
	void insertCoveredTableCell(const librevenge::RVNGPropertyList &propList) override;
	void closeTable() override;
	void openFrame(const librevenge::RVNGPropertyList &propList) override;
	void closeFrame() override;
	void openGroup(const librevenge::RVNGPropertyList &propList) override;
	void closeGroup() override;
	void defineGraphicStyle(const librevenge::RVNGPropertyList &propList) override;
	void drawRectangle(const librevenge::RVNGPropertyList &propList) override;
	void drawEllipse(const librevenge::RVNGPropertyList &propList) override;
	void drawPolygon(const librevenge::RVNGPropertyList &propList) override;
	void drawPolyline(const librevenge::RVNGPropertyList &propList) override;
	void drawPath(const librevenge::RVNGPropertyList &propList) override;
	void drawConnector(const librevenge::RVNGPropertyList &propList) override;
	void insertBinaryObject(const librevenge::RVNGPropertyList &propList) override;
	void insertEquation(const librevenge::RVNGPropertyList &propList) override;
private:
	QList<PageItem*> *mElements { nullptr };
	RawPainter *painter { nullptr };
	QList<QList<PageItem*> > pageElements;
	ScribusDoc* mDoc { nullptr };
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
	QString constructFontName(const QString& fontBaseName, const QString& fontStyle);
	double valueAsPoint(const WPXProperty *prop) const;
	double fromPercentage(const QString &s) const;
	QColor  parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void insertImage(PageItem* ite, const QString& imgExt, QByteArray &imageData);
	void applyFill(PageItem* ite);
	void applyShadow(PageItem* ite);
	void applyFlip(PageItem* ite) const;
	void recolorItem(PageItem* ite, const QString& efVal);
	void applyArrows(PageItem* ite);
	void finishItem(PageItem* ite);

	ScribusDoc* m_Doc { nullptr };
	Selection* m_tmpSel { nullptr };
	QList<PageItem*> *m_elements { nullptr };
	QStringList *m_importedColors { nullptr };
	QStringList *m_importedPatterns { nullptr };
	struct groupEntry
	{
		QList<PageItem*> Items;
		FPointArray clip;
	};
	QStack<groupEntry> m_groupStack;

	double m_lineWidth { 1.0 };
	QString m_currColorFill { "Black" };
	QString m_currColorStroke { "Black" };
	double m_currStrokeShade { 100.0 };
	double m_currFillShade { 100.0 };
	double m_currStrokeTrans { 0.0 };
	double m_currFillTrans { 0.0 };
	FPointArray m_coords;
	bool m_fillRule { true };
	double m_gradientAngle { 0.0 };
	bool m_isGradient { false };
	VGradient m_currentGradient;
	QString m_gradColor1Str;
	QColor m_gradColor1;
	double m_gradColor1Trans { 1.0 };
	QString m_gradColor2Str;
	QColor m_gradColor2;
	double m_gradColor2Trans { 1.0 };
	QVector<double> m_dashArray;
	Qt::PenJoinStyle m_lineJoin { Qt::MiterJoin };
	Qt::PenCapStyle m_lineEnd { Qt::FlatCap };
	double m_baseX;
	double m_baseY;
	double m_docWidth;
	double m_docHeight;
	int m_importerFlags;
	bool m_firstPage { true };
	int  m_currentPage { 0 };
	QString m_baseLayer;
	WPXPropertyList m_style;
	PageItem *m_currTextItem { nullptr };
	ParagraphStyle m_textStyle;
	CharStyle m_textCharStyle;
	double m_linespace { 1.0 };
	double m_maxFontSize { 1.0 };
	bool m_lineSpSet { false };
	bool m_lineSpIsPT { false };
	bool m_doProcessing { true };
	QString m_fileType;
};
#endif

#endif
