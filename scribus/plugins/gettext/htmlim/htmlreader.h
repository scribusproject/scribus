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

#include <QString>
#include <QXmlAttributes>

#include <gtparagraphstyle.h>
#include <gtwriter.h>

/*! \brief Parse and import a HTML file.
Supported tags: P, CENTER, BR, A, UL, OL, LI, H1, H2, H3, H4,
B, STRONG, I, EM, CODE, BODY, PRE, IMG, SUB, SUP, DEL, INS, U,
DIV.
*/
class HTMLReader
{
private:
	QString m_currentDir;
	QString m_currentFile;
	QString m_defaultColor;
	QString m_defaultWeight;
	QString m_defaultSlant;
	QString m_templateCategory;
	QString m_href;
	QString m_extLinks;
	int m_extIndex;
	int m_listLevel;
	std::vector<gtParagraphStyle*> m_listStyles;
	std::vector<int> m_nextItemNumbers;
	gtWriter *m_writer;
	gtParagraphStyle *m_pstyle;
	gtParagraphStyle *m_pstylec;
	gtParagraphStyle *m_pstyleh1;
	gtParagraphStyle *m_pstyleh2;
	gtParagraphStyle *m_pstyleh3;
	gtParagraphStyle *m_pstyleh4;
	gtParagraphStyle *m_pstyleh5;
	gtParagraphStyle *m_pstyleh6;
	gtParagraphStyle *m_pstylecode;
	gtParagraphStyle *m_pstylep;
	gtParagraphStyle *m_pstylepre;
	bool m_inOL;
	bool m_wasInOL;
	bool m_inUL;
	bool m_wasInUL;
	bool m_inLI;
	bool m_addedLI;
	bool m_inH1;
	bool m_inH2;
	bool m_inH3;
	bool m_inH4;
	bool m_inH5;
	bool m_inH6;
	bool m_inA;
	bool m_inCenter;
	bool m_inCode;
	bool m_inBody;
	bool m_inPre;
	bool m_inP;
	static bool m_elemJustStarted;
	static bool m_elemJustFinished;
	bool m_lastCharWasSpace;
	bool m_noFormatting;
	void initPStyles();
	void toggleEffect(FontEffect e);
	void setItalicFont();
	void unsetItalicFont();
	void setBlueFont();
	void setDefaultColor();
	void setBoldFont();
	void unSetBoldFont();
	void createListStyle();
	static HTMLReader* m_hreader;
public:
	HTMLReader(gtParagraphStyle *ps, gtWriter *w, bool textOnly);
	~HTMLReader();
	void parse(QString filename);
	static void startElement(void *user_data, const xmlChar * fullname, const xmlChar ** atts);
	static void endElement(void *user_data, const xmlChar * name);
	static void characters(void *user_data, const xmlChar * ch, int len);
	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool endElement(const QString&, const QString&, const QString &name);
	bool characters(const QString &ch);
};

#endif
