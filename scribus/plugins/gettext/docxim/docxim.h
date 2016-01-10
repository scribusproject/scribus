/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Nov 15 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef DOCXIM_H
#define DOCXIM_H

#include "pluginapi.h"
#include "pageitem.h"

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QHash>
class ScZipHandler;

extern "C" PLUGIN_API void GetText2(QString filename, QString encoding, bool textOnly, bool prefix, PageItem *textItem);
extern "C" PLUGIN_API QString FileFormatName();
extern "C" PLUGIN_API QStringList FileExtensions();

class DocXIm
{
	public:
		DocXIm(QString fileName, PageItem *textItem, bool textOnly, bool prefix);
		~DocXIm();
	private:
		void parseContentTypes();
		void parseTheme();
		void parseStyles();
		void parseStyledText(PageItem *textItem);
		void parseParaProps(QDomElement &props, ParagraphStyle &pStyle);
		void parseCharProps(QDomElement &props, ParagraphStyle &pStyle);
		void parsePlainTextOnly(PageItem *textItem);
		QString getFontName(QString name);
		double pixelsFromTwips(double twips);
		QString themePart;
		QString docPart;
		QString stylePart;
		QString themeFont1;
		QString themeFont2;
		ScZipHandler *uz;
		ScribusDoc* m_Doc;
		PageItem* m_item;
		bool m_prefixName;
		ParagraphStyle defaultParagraphStyle;
		ParagraphStyle currentParagraphStyle;
		QHash<QString, QString> map_ID_to_Name;
};

#endif
