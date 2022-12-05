/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Jan  3 2016
	copyright            : (C) 2016 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef ODTIM_H
#define ODTIM_H

#include "pluginapi.h"
#include "pageitem.h"
#include "scribusstructs.h"

#include <QDomDocument>
#include <QDomElement>
#include <QHash>
#include <QStack>
#include <QString>

class ScZipHandler;

extern "C" PLUGIN_API void GetText2(const QString& filename, const QString& encoding, bool textOnly, bool prefix, bool append, PageItem *textItem);
extern "C" PLUGIN_API QString FileFormatName();
extern "C" PLUGIN_API QStringList FileExtensions();

class ObjStyleODT
{
public:
	ObjStyleODT() {}

	QString CurrColorText {"Black"};
	QString CurrColorBText {CommonStrings::None};
	QString CurrColorBPara {CommonStrings::None};
	QString fontName;
	QString fontStyle;
	QString fontWeight;
	double fontSize {10.0};
	double textIndent {0.0};
	ParagraphStyle::AlignmentType	textAlign {ParagraphStyle::LeftAligned};
	QString textPos;
	QString textOutline;
	bool textUnderline {false};
	bool textUnderlineWords {false};
	QString textUnderlineColor {CommonStrings::None};
	bool textStrikeThrough {false};
	bool textShadow {false};
	bool textSmallCaps {false};
	double lineHeight {1.0};
	bool absLineHeight {false};
	double margin_top {0.0};
	double margin_bottom {0.0};
	double margin_left {0.0};
	double margin_right {0.0};
	int verticalAlignment {0};
	QList<ParagraphStyle::TabRecord> tabStops;
	QString breakBefore {"auto"};
	QString breakAfter {"auto"};
};

class ODTIm
{
public:
	ODTIm(const QString& fileName, PageItem *textItem, bool textOnly, bool prefix, bool append);
	~ODTIm() = default;

private:
	struct DrawStyle
	{
		AttributeValue styleOrigin;
		AttributeValue styleType;
		AttributeValue displayName;
		AttributeValue parentStyle;
		AttributeValue fontColor;
		AttributeValue fontName;
		AttributeValue fontSize;
		AttributeValue fontStyle;
		AttributeValue fontWeight;
		AttributeValue textIndent;
		AttributeValue textAlign;
		AttributeValue textPos;
		AttributeValue textOutline;
		AttributeValue textUnderline;
		AttributeValue textUnderlineWords;
		AttributeValue textUnderlineColor;
		AttributeValue textStrikeThrough;
		AttributeValue textShadow;
		AttributeValue fontVariant;
		AttributeValue textBackgroundColor;
		AttributeValue parBackgroundColor;
		AttributeValue lineHeight;
		AttributeValue margin_top;
		AttributeValue margin_bottom;
		AttributeValue margin_left;
		AttributeValue margin_right;
		AttributeValue verticalAlignment;
		AttributeValue tabDists;
		AttributeValue tabTypes;
		AttributeValue breakBefore;
		AttributeValue breakAfter;
	};
	bool parseRawDocReference(const QString& designMap);
	bool parseRawDocReferenceXML(const QDomDocument &designMapDom);
	void parseRawTextSpan(const QDomElement &elem, PageItem* item, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, int &posC);
	void parseRawTextParagraph(const QDomNode &elem, PageItem* item, ParagraphStyle &newStyle, int &posC);
	void parseRawText(const QDomElement &elem, PageItem* item);

	bool parseStyleSheets(const QString& designMap);
	bool parseStyleSheetsXML(const QDomDocument &designMapDom);
	void parseStyles(const QDomElement &sp, const QString& type);
	bool parseDocReference(const QString& designMap);
	bool parseDocReferenceXML(const QDomDocument &designMapDom);
	void parseTextSpan(const QDomElement &elem, PageItem* item, const ParagraphStyle &tmpStyle, const CharStyle &tmpCStyle, const ObjStyleODT& tmpOStyle, int &posC);
	void parseTextParagraph(const QDomNode &elem, PageItem* item, const ParagraphStyle &newStyle, const ObjStyleODT &tmpOStyle, int &posC);
	void parseText(const QDomElement &elem, PageItem* item, const ObjStyleODT& tmpOStyle);
	void insertChars(PageItem *item, QString &txt, const ParagraphStyle &tmpStyle, const CharStyle &tmpCStyle, int &posC);
	void applyCharacterStyle(CharStyle &tmpCStyle, const ObjStyleODT &oStyle);
	void applyParagraphStyle(ParagraphStyle &tmpStyle, const ObjStyleODT &oStyle);
	void resolveStyle(ObjStyleODT &tmpOStyle, const QString& pAttrs);
	double parseUnit(const QString &unit);
	QString parseColor( const QString &s );
	QString constructFontName(const QString& fontBaseName, const QString& fontStyle);
	void setFontstyle(CharStyle &tmpCStyle, int kind);

	ScZipHandler *uz {nullptr};
	ScribusDoc* m_Doc {nullptr};
	PageItem* m_item {nullptr};
	bool m_prefixName {false};
	bool m_append {false};
	QHash<QString, QString> map_ID_to_Name;
	QHash<QString, QString> m_fontMap;
	QHash<QString, DrawStyle> m_Styles;
	QStack<QString> m_textStylesStack;
	DrawStyle parDefaultStyle;
	DrawStyle txtDefaultStyle;
};

#endif
