/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QMap>
#include <QString>
#include <QTextBlock>
#include <QTextDocument>

#include <libxml/HTMLparser.h>

class ScribusDoc;
class PageItem;



#ifndef SCCLIPBOARDPROCESSOR_H
#define SCCLIPBOARDPROCESSOR_H

#define SCCLIP_DEBUG 1

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
		bool process();
		const QString &data();
		void reset();
		void dumpClipboardData();

	protected:
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

		bool processText();
		bool processHTML();
		bool processHTML_Other();

		bool html_MSFT_Process();
		void html_MSFT_Parse(xmlNodePtr node);
		void html_MSFT_ParseStyles(xmlNode *node, QMap<QString, QString> &styles);
		bool html_MSFT_StyleToProcess(const QString &style);
		void html_MSFT_Process_CSS(const QMap<QString, QString> &styles);
		void html_MSFT_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles);
		QString html_MSFT_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts);

		bool html_LibreOffice_Process();
		void html_LibreOffice_Parse(xmlNodePtr node);
		void html_LibreOffice_ProcessCSS(const QMap<QString, QString> &styles);
		void html_LibreOffice_ParseStyles(xmlNode *node, QMap<QString, QString> &styles);
		QString html_LibreOffice_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts);
		void html_LibreOffice_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles);

		bool html_Cocoa_Process();
		void html_Cocoa_Parse(xmlNodePtr node);
		void html_Cocoa_ParseStyles(xmlNode *node, QMap<QString, QString> &styles);
		QString html_Cocoa_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts);
		void html_Cocoa_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles);
		void html_Cocoa_ProcessCSS(const QMap<QString, QString> &styles);

		QString m_content;
		ContentType m_contentType {ContentType::Unknown};
		QString m_result;
		bool processed {false};
		QMap<QString, QString> cssStyles;
		ScribusDoc* m_doc {nullptr};
		PageItem* m_pageItem {nullptr};
};

#endif // SCCLIPBOARDPROCESSOR_H
