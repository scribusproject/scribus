/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Wed Aug 19 2020
	copyright            : (C) 2020 by Craig Bradney
	email                : cbradney@scribus.info
 ***************************************************************************/
#ifndef MARKDOWNIM_H
#define MARKDOWNIM_H

#include "pluginapi.h"
#include "pageitem.h"

#include <QTextDocument>
#include <QString>
#include <QHash>

extern "C" PLUGIN_API void GetText2(const QString& filename, const QString& encoding, bool textOnly, bool prefix, bool append, PageItem *textItem);
extern "C" PLUGIN_API QString FileFormatName();
extern "C" PLUGIN_API QStringList FileExtensions();

class MarkDownIm
{
	public:
		MarkDownIm(const QString& fileName, PageItem *textItem, bool textOnly, bool prefix, bool append);
		~MarkDownIm() {};

	private:
		void parseMarkDown();

		QTextDocument m_importedText;

		ScribusDoc* m_Doc { nullptr };
		PageItem* m_item { nullptr };
		bool m_prefixName { false };
		bool m_append { false };
		bool m_textOnly { false };
		ParagraphStyle defaultParagraphStyle;
		ParagraphStyle currentParagraphStyle;
};

#endif
