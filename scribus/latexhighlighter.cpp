/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexhighlighter.cpp  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include "latexhighlighter.h"

#include <QDebug>

LatexHighlighter::LatexHighlighter(QTextDocument *document)
	: QSyntaxHighlighter(document)
{
	QTextCharFormat mathFormat;
	mathFormat.setForeground(QColor(0, 128, 0));
	mathFormat.setFontWeight(QFont::Bold);
	setFormatFor(Math, mathFormat);

	QTextCharFormat commandFormat;
	commandFormat.setForeground(QColor(255, 0, 0));
	setFormatFor(Command, commandFormat);
	
	QTextCharFormat blockNameFormat;
	blockNameFormat.setForeground(QColor(0, 0, 255));
	setFormatFor(BlockName, blockNameFormat);
	
	QTextCharFormat scribusFormat;
	scribusFormat.setForeground(QColor(192, 16, 112));
	scribusFormat.setFontWeight(QFont::Bold);
	setFormatFor(ScribusSpecial, scribusFormat);

	QTextCharFormat commentFormat;
	commentFormat.setForeground(QColor(128, 128, 128));
	commentFormat.setFontItalic(true);
	setFormatFor(Comment, commentFormat);
}

void LatexHighlighter::setFormatFor(Construct construct, const QTextCharFormat &format)
{
	m_formats[construct] = format;
	rehighlight();
}


void LatexHighlighter::highlightBlock(const QString &text)
{
	int state = previousBlockState();
	int len = text.length();
	int start = 0;
	int pos = 0;
	//qDebug() << "Block:" << text;
	while (pos < len) {
		switch (state) {
			case NormalState:
			default:
				while (pos < len) {
					QChar ch = text.at(pos);
					if (ch == '%') {
						//Comment
						setFormat(pos, len - pos, m_formats[Comment]);
						pos = len; //Do not handle other formats in comments
						break;
					} else if (ch == '\\') {
						start = pos;
						pos++;
						if (pos >= len) break;
						ch = text.at(pos);
						if (ch == '[') {
							state = InMath;
							break;
						} else if ((text.mid(pos, 5) == "begin") || (text.mid(pos, 3) == "end")) {
							int saved_pos = pos;
							if (text.at(pos) == 'b') {
								setFormat(pos-1, 6, m_formats[Command]);
								pos += 5;
							} else {
								setFormat(pos-1, 4, m_formats[Command]);
								pos += 3;
							}
							
							while (pos < len && text.at(pos++).isSpace()) /*Skip over whitespace */;
							if (text.at(pos-1) == '{') {
								start = pos;
								while (pos < len && text.at(pos++) != '}') /*Do nothing */;
								pos--;
								//qDebug() << "Start:" << start << "Stop:" << (pos - start) << text.at(pos-1);
								setFormat(start, pos - start, m_formats[BlockName]);
							} else {
								pos = saved_pos;
							}
						} else {
							while (pos < len) {
								ch = text.at(pos++);
								//Allowed chars are A-Za-z@
								if (!(((ch >= 'A') && (ch <= 'Z')) ||
									((ch >= 'a') && (ch <= 'z')) || (ch == '@'))) break;
							}
							//qDebug() << "Len: " << (pos - start);
							if ((pos - start) == 2) {
								//Special char
								setFormat(start, pos - start, m_formats[ScribusSpecial]);
							} else {
								setFormat(start, pos - start -1, m_formats[Command]);
							}
						}
						break;
					} else if (ch == '$') {
						if (text.mid(pos, 9) == "$scribus_") {
							//Scribus special code
							start = pos;
							pos++;
							while (pos < len && text.at(pos++) != '$') /*Do nothing */;
							setFormat(start, pos - start, m_formats[ScribusSpecial]);
						} else {
							state = InDisplayMath;
							pos++;
							break;
						}
					}
					pos++;
				}
				break;
			case InDisplayMath:
				start = pos;
				if (start > 0) start--;
				while (pos < len) {
					if (text.at(pos) == '$') {
						pos++;
						state = NormalState;
						break;
					}
					pos++;
				}
				setFormat(start, pos - start, m_formats[Math]);
				break;
			case InMath:
				start = pos;
				if (start > 0) start--;
				while (pos < len) {
					if (text.mid(pos,2) == "\\]" ) {
						pos += 2;
						state = NormalState;
						break;
					}
					pos++;
				}
				setFormat(start, pos - start, m_formats[Math]);
				break;
		}
	}
	setCurrentBlockState(state);
}
