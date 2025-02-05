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

//#define SCCLIP_DEBUG 1

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
			bool isBold = false;
			bool isItalic = false;
		};

		bool processText();
		bool processHTML();
		bool html_MSFT_Process();
		bool html_Cocoa_Process();
		void html_Cocoa_Parse(xmlNodePtr node);
		void cocoaParseStyles(xmlNode *node, QMap<QString, QString> &styles);
		QString cocoaExtractText(xmlNode *node, QList<TextSegment> &segments, bool bold = false, bool italic = false);
		void cocoaParseParagraphs(xmlNode *node, QMap<QString, QString> &styles);
		void processCSS(const QMap<QString, QString> &styles);
		bool processHTML_Other();

		QString m_content;
		ContentType m_contentType {ContentType::Unknown};
		QString m_result;
		bool processed {false};
		QMap<QString, QString> cssStyles;
		ScribusDoc* m_doc {nullptr};
		PageItem* m_pageItem {nullptr};
};

#endif // SCCLIPBOARDPROCESSOR_H
