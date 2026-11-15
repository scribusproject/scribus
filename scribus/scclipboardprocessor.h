/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QColor>
#include <QMap>
#include <QString>
#include <QTextBlock>
#include <QTextDocument>

#include <libxml/HTMLparser.h>

class ScribusDoc;
class PageItem;
class PageItem_Table;
class PageItem_TextFrame;
class ParagraphStyle;


#ifndef SCCLIPBOARDPROCESSOR_H
#define SCCLIPBOARDPROCESSOR_H

// #define SCCLIP_DEBUG 1

class ScClipboardProcessor
{
	public:
		enum ContentType
		{
			Text = 0,
			HTML = 1,
			Unknown = 99
		};

		ScClipboardProcessor();
		ScClipboardProcessor(ScribusDoc* doc, PageItem *pageItem);

		void setContent(const QString& content, ContentType contentType);
		void setDocAndPageItem(ScribusDoc* doc, PageItem *pageItem);
		/**
		 * Dumps all MIME formats currently on the clipboard to qDebug, with size
		 * and content (truncated when large).
		 * Debug aid for understanding what various applications place on the clipboard.
		 */
		static void debugDumpClipboard();
		bool process();
		const QString &data();
		void reset();
		void setDestTable(PageItem_Table *table);

	protected:
		inline static const QSet<QString> htmlHeadingTags = { "h1", "h2", "h3", "h4", "h5", "h6" };

		struct TextSegment
		{
			QString text;
			QString color;
			bool isBold {false};
			bool isItalic {false};
			bool hasUnderline {false};
			double fontsize {0.0};
			QString family;
		};

		struct ParsedTableParagraph
		{
			QString className;
			QList<TextSegment> segments;
		};

		struct ParsedTableCell
		{
			int row = 0;
			int column = 0;
			int rowSpan = 1;
			int columnSpan = 1;
			QColor background; // invalid when the source sets no background
			QList<ParsedTableParagraph> paragraphs;
		};

		struct ParsedTable
		{
			int rows = 0;
			int columns = 0;
			QList<ParsedTableCell> cells;
		};

		bool processText();
		bool processHTML();
		bool processHTML_Other();

		bool html_MSFT_Process();
		void html_MSFT_Parse(xmlNodePtr node);
		void html_MSFT_ParseStyles(xmlNode *node, QMap<QString, QString> &styles);
		bool html_MSFT_StyleToProcess(const QString &style);
		void html_MSFT_Process_CSS(const QMap<QString, QString> &styles);
		void html_MSFT_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles);
		void html_MSFT_ParseTable(xmlNode *tableNode, ParsedTable &out);
		void applyMSFTCssStyleToSegment(QString styleData, TextSegment &ts);
		QString html_MSFT_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts);

		bool html_LibreOffice_Process();
		void html_LibreOffice_Parse(xmlNodePtr node);
		void html_LibreOffice_ProcessCSS(const QMap<QString, QString> &styles);
		void html_LibreOffice_ParseStyles(xmlNode *node, QMap<QString, QString> &styles);
		QString html_LibreOffice_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts);
		void html_LibreOffice_ParseTable(xmlNode *tableNode, ParsedTable &out);
		void html_LibreOffice_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles);


		/// Returns the background colour of a table cell, or an invalid colour if it has none.
		QColor html_CellBackground(xmlNode *cellNode) const;
		/// Returns the colour set by a CSS background or background-color declaration in \a style.
		static QColor cssBackgroundColor(const QString &style);
		/// Returns a CSS colour value as a colour, or an invalid colour for none or transparent.
		static QColor cssColor(const QString &value);

		void html_ApplyTable(const ParsedTable &pt);
		void html_ApplyParagraphsToFrame(PageItem_TextFrame *frame, const QList<ParsedTableParagraph> &paragraphs);
		void html_ApplySegmentsToFrame(PageItem_TextFrame *frame, const QList<TextSegment> &segments, const ParagraphStyle &seedStyle);


		bool html_Cocoa_Process();
		void html_Cocoa_Parse(xmlNodePtr node);
		void html_Cocoa_ParseStyles(xmlNode *node, QMap<QString, QString> &styles);
		QString html_Cocoa_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts);
		void html_Cocoa_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles);
		void html_Cocoa_ProcessCSS(const QMap<QString, QString> &styles);

		QString m_content;
		ContentType m_contentType {ContentType::Unknown};
		QString m_result;
		bool processed { false };
		QMap<QString, QString> cssStyles;
		/// Background colours from the style sheet's class and tag rules, by selector.
		QMap<QString, QColor> m_cssBackgrounds;
		ScribusDoc* m_doc { nullptr };
		PageItem* m_pageItem { nullptr };
		PageItem_Table* m_tablePageItem { nullptr };
};

#endif // SCCLIPBOARDPROCESSOR_H
