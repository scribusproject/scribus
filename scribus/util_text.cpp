/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//#include <QDebug>

#include <sstream>
#include "desaxe/saxXML.h"
#include "scribusdoc.h"
#include "util_text.h"
#include "serializer.h"

bool checkCJK(QChar ch)
{
	unsigned int code = ch.unicode();
	if (	(0x2E80 < code && code < 0x2EFF)   ||  // CJK Radicals Supplement
		(0x3000 < code && code < 0x303F)   ||  // CJK Symbols and Punctuation
		(0x3040 < code && code <= 0x30FF)  ||  // Hiragana, Katakana
		(0x31C0 < code && code < 0x31EF)   ||  // CJK Strokes
		(0x3200 < code && code < 0x32FF)   ||  // Enclosed CJK Letters and Months
		(0x3300 < code && code < 0x33FF)   ||  // CJK Compatibility
		(0x3400 < code && code < 0x4DBF)   ||  // CJK Unified Ideographs Extension A
		(0x4E00 < code && code < 0x9FFF)   ||  // CJK Unified Ideographs
		(0xF900 < code && code < 0xFAFF)   ||  // CJK Compatibility Ideographs
		(0xFE30 < code && code < 0xFE4F)   ||  // CJK Compatibility Forms
		(0x20000 < code && code < 0x2A6DF) ||  // CJK Unified Ideographs Extension B
		(0x2A700 < code && code < 0x2B73F) ||  // CJK Unified Ideographs Extension C
		(0x2B740 < code && code < 0x2B81F) ||  // CJK Unified Ideographs Extension D
		(0x2F800 < code && code < 0x2FA1F) ||  // CJK Compatibility Ideographs Supplement
		(0xFF01 < code && code < 0xFF0F)   ||
		(0xFF1A < code && code < 0xFF20)   ||
		(0xFF58 < code && code < 0xFFDC)   ||
		(code == 0x3000) ||
		(code == 0x3002) ||
		(code == 0x201C) ||
		(code == 0x201D))
		return true;
	else
		return false;
}

bool allowedCJKBreakAfter(QChar ch)
{
	unsigned int code[] = {0x201C, 0x300C, 0xFF08, 0xFF3B, 0xFF5B, 0xFF5F, 0xFF62, 0xFF0D, 0};
	for (int i = 0; code[i]; ++i)
		if (code[i] == ch.unicode())
			return false;
	return true;
}

int allowedCJKBreakBefore(QChar ch)
{
	unsigned int code[] =
	 {0x201D, 0x3001, 0x3002, 0x300D, 0xFF01, 0xFF09, 0xFF0C, 0xFF0E, 0xFF1A,
	  0xFF1B, 0xFF1F, 0xFF3D, 0xFF5D, 0xFF60, 0xFF63, 0xFF64, 0};
	for (int i = 0; code[i]; ++i)
		if (code[i] == ch.unicode())
			return false;
	return true;
}

bool implicitSpace(QChar f, QChar s)
{
	return checkCJK(f) && checkCJK(s);
}

bool implicitBreak(QChar f, QChar s)
{
	if (checkCJK(f) && checkCJK(s)) {
		return allowedCJKBreakAfter(f) && allowedCJKBreakBefore(s);
	} else
		return false;
}

int findParagraphStyle(ScribusDoc* doc, const ParagraphStyle& parStyle)
{
	bool named = !parStyle.name().isEmpty();
	//qDebug() << QString("looking up %1/ %2").arg(parStyle.name()).arg(parStyle.alignment()); 
	if (named) {
		for (int i=0; i < doc->paragraphStyles().count(); ++i)
		{
			//qDebug() << QString("%1 %2").arg(i).arg(doc->paragraphStyles()[i].name());
			if (parStyle.name() == doc->paragraphStyles()[i].name()) {
				return i;
			}
		}
		assert(false);
		return -1;
	}
	else {
		return -1;
	}
}

int findParagraphStyle(ScribusDoc* doc, const QString &name)
{
	for (int i=0; i < doc->paragraphStyles().count(); ++i)
	{
		if (name == doc->paragraphStyles()[i].name()) {
			return i;
		}
	}
	assert(false);
	return -1;
}

StoryText desaxeString(ScribusDoc* doc, QString saxedString)
{
	assert(!saxedString.isEmpty());

	Serializer* dig = doc->textSerializer();

	QByteArray utf8Data = saxedString.toUtf8();
	dig->parseMemory(utf8Data.constData(), utf8Data.length());

	StoryText* story = dig->result<StoryText>();
	assert (story != NULL);

	StoryText res = *story;
	delete story;
	return res;
}

QString saxedText(StoryText* story)
{
	std::ostringstream xmlString;
	SaxXML xmlStream(xmlString);
	xmlStream.beginDoc();
	story->saxx(xmlStream, "SCRIBUSTEXT");
	xmlStream.endDoc();
	std::string xml(xmlString.str());
	return QString(xml.c_str());
}

