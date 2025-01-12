/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QString>
#include <QTextBlock>
#include <QTextDocument>


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
		ScClipboardProcessor(const QString& content, ContentType contentType);
		void setContent(const QString& content, ContentType contentType);
		bool process();
		const QString &data();
		void reset();
		void dumpClipboardData();

	protected:
		bool processText();
		bool processHTML();
		bool processHTML_MSFT();
		bool processHTML_Cocoa();
		bool processHTML_Other();

		QString m_content;
		ContentType m_contentType {ContentType::Unknown};
		QString m_result;
		bool processed {false};
};

#endif // SCCLIPBOARDPROCESSOR_H
