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

extern "C" PLUGIN_API void GetText2(QString filename, QString encoding, bool textOnly, bool prefix, bool append, PageItem *textItem);
extern "C" PLUGIN_API QString FileFormatName();
extern "C" PLUGIN_API QStringList FileExtensions();

class ObjStyleODT
{
public:
	ObjStyleODT() :
	  CurrColorText("Black"),
	  CurrColorBText(CommonStrings::None),
	  CurrColorBPara(CommonStrings::None),
	  fontName(""),
	  fontStyle(""),
	  fontWeight(""),
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
	  textSmallCaps(false),
	  lineHeight(1.0),
	  absLineHeight(false),
	  margin_top(0.0),
	  margin_bottom(0.0),
	  margin_left(0.0),
	  margin_right(0.0),
	  verticalAlignment(0),
	  tabStops(),
	  breakBefore("auto"),
	  breakAfter("auto")
	  {}
	QString							CurrColorText;
	QString							CurrColorBText;
	QString							CurrColorBPara;
	QString							fontName;
	QString							fontStyle;
	QString							fontWeight;
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
	bool							textSmallCaps;
	double							lineHeight;
	bool							absLineHeight;
	double							margin_top;
	double							margin_bottom;
	double							margin_left;
	double							margin_right;
	int								verticalAlignment;
	QList<ParagraphStyle::TabRecord> tabStops;
	QString							breakBefore;
	QString							breakAfter;
};

class ODTIm
{
	public:
		ODTIm(QString fileName, PageItem *textItem, bool textOnly, bool prefix, bool append);
		~ODTIm();
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
		bool parseRawDocReference(QString designMap);
		bool parseRawDocReferenceXML(QDomDocument &designMapDom);
		void parseRawTextSpan(QDomElement &elem, PageItem* item, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, int &posC);
		void parseRawTextParagraph(QDomNode &elem, PageItem* item, ParagraphStyle &newStyle, int &posC);
		void parseRawText(QDomElement &elem, PageItem* item);

		bool parseStyleSheets(QString designMap);
		bool parseStyleSheetsXML(QDomDocument &designMapDom);
		void parseStyles(QDomElement &sp, QString type);
		bool parseDocReference(QString designMap);
		bool parseDocReferenceXML(QDomDocument &designMapDom);
		void parseTextSpan(QDomElement &elem, PageItem* item, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, ObjStyleODT& tmpOStyle, int &posC);
		void parseTextParagraph(QDomNode &elem, PageItem* item, ParagraphStyle &newStyle, ObjStyleODT &tmpOStyle, int &posC);
		void parseText(QDomElement &elem, PageItem* item, ObjStyleODT& tmpOStyle);
		void insertChars(PageItem *item, QString &txt, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, int &posC);
		void applyCharacterStyle(CharStyle &tmpCStyle, ObjStyleODT &oStyle);
		void applyParagraphStyle(ParagraphStyle &tmpStyle, ObjStyleODT &oStyle);
		void resolveStyle(ObjStyleODT &tmpOStyle, QString pAttrs);
		double parseUnit(const QString &unit);
		QColor parseColorN( const QString &rgbColor );
		QString parseColor( const QString &s );
		QString constructFontName(QString fontBaseName, QString fontStyle);
		void setFontstyle(CharStyle &tmpCStyle, int kind);
		ScZipHandler *uz;
		ScribusDoc* m_Doc;
		PageItem* m_item;
		bool m_prefixName;
		bool m_append;
		QHash<QString, QString> map_ID_to_Name;
		QHash<QString, QString> m_fontMap;
		QHash<QString, DrawStyle> m_Styles;
		QStack<QString> m_textStylesStack;
		DrawStyle parDefaultStyle;
		DrawStyle txtDefaultStyle;
};

#endif
