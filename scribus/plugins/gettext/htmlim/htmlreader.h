/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#ifndef HTMLREADER_H
#define HTMLREADER_H

#include "scconfig.h"

#include <vector>
#include <libxml/HTMLparser.h>

#include <QMap>
#include <QString>

#include <gtparagraphstyle.h>
#include <gtwriter.h>

typedef QMap<QString, QString> HTMLAttributesMap;

/*! \brief Parse and import a HTML file.
Supported tags: P, CENTER, BR, A, UL, OL, LI, H1, H2, H3, H4,
B, STRONG, I, EM, CODE, BODY, PRE, IMG, SUB, SUP, DEL, INS, U,
DIV.
*/
class HTMLReader
{
public:
	HTMLReader(gtParagraphStyle *ps, gtWriter *w, bool textOnly);
	~HTMLReader();

	void parse(const QString& filename);
	static void startElement(void *user_data, const xmlChar * fullname, const xmlChar ** atts);
	static void endElement(void *user_data, const xmlChar * name);
	static void characters(void *user_data, const xmlChar * ch, int len);
	bool startElement(const QString &name, const HTMLAttributesMap &attrs);
	bool endElement(const QString &name);
	bool characters(const QString &ch);

private:
	QString currentDir;
	QString currentFile;
	QString defaultColor;
	QString defaultWeight;
	QString defaultSlant;
	QString templateCategory;
	QString href;
	QString extLinks;
	int extIndex { 1 };
	int listLevel { -1 };
	std::vector<gtParagraphStyle*> listStyles;
	std::vector<int> nextItemNumbers;
	gtWriter *writer { nullptr };
	gtParagraphStyle *pstyle { nullptr };
	gtParagraphStyle *pstylec { nullptr };
	gtParagraphStyle *pstyleh1 { nullptr };
	gtParagraphStyle *pstyleh2 { nullptr };
	gtParagraphStyle *pstyleh3 { nullptr };
	gtParagraphStyle *pstyleh4 { nullptr };
	gtParagraphStyle *pstyleh5 { nullptr };
	gtParagraphStyle *pstyleh6 { nullptr };
	gtParagraphStyle *pstylecode { nullptr };
	gtParagraphStyle *pstylep { nullptr };
	gtParagraphStyle *pstylepre { nullptr };
	bool inOL { false };
	bool wasInOL { false };
	bool inUL { false };
	bool wasInUL { false };
	bool inLI { false };
	bool addedLI { false };
	bool inH1 { false };
	bool inH2 { false };
	bool inH3 { false };
	bool inH4 { false };
	bool inH5 { false };
	bool inH6 { false };
	bool inA { false };
	bool inCenter { false };
	bool inCode { false };
	bool inBody { false };
	bool inPre { false };
	bool inP { false };

	static bool elemJustStarted;
	static bool elemJustFinished;

	bool lastCharWasSpace { false };
	bool noFormatting { false };

	static HTMLReader* hreader;

	void initPStyles();
	void toggleEffect(FontEffect e);
	void setItalicFont();
	void unsetItalicFont();
	void setBlueFont();
	void setDefaultColor();
	void setBoldFont();
	void unSetBoldFont();
	void createListStyle();
};

#endif
