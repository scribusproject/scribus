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
#include <unicode/brkiter.h>

#include "desaxe/saxXML.h"
#include "scribusdoc.h"
#include "serializer.h"
#include "util_text.h"

using namespace icu;

int findParagraphStyle(ScribusDoc* doc, const ParagraphStyle& parStyle)
{
	bool named = !parStyle.name().isEmpty();
	//qDebug() << QString("looking up %1/ %2").arg(parStyle.name()).arg(parStyle.alignment()); 
	if (named)
	{
		const auto& paragraphStyles = doc->paragraphStyles();
		for (int i=0; i < paragraphStyles.count(); ++i)
		{
			//qDebug() << QString("%1 %2").arg(i).arg(doc->paragraphStyles()[i].name());
			if (parStyle.name() == paragraphStyles[i].name()) {
				return i;
			}
		}
		assert(false);
	}
	return -1;
}

int findParagraphStyle(ScribusDoc* doc, const QString &name)
{
	// This make behavior consistent with findParagraphStyle(ScribusDoc*, const ParagraphStyle&)
	// for unnamed styles
	if (name.isEmpty())
		return -1;

	for (int i = 0; i < doc->paragraphStyles().count(); ++i)
	{
		if (name == doc->paragraphStyles()[i].name())
			return i;
	}
	assert(false);
	return -1;
}

bool SCRIBUS_API localeAwareLessThan(const QString& s1, const QString& s2)
{
	int result = QString::localeAwareCompare(s1, s2);
	return (result < 0);
}

StoryText desaxeString(ScribusDoc* doc, const QString& saxedString)
{
	assert(!saxedString.isEmpty());

	Serializer* dig = doc->textSerializer();
	dig->reset();
	dig->store<ScribusDoc>("<scribusdoc>", doc);

	dig->parseMemory(saxedString);

	StoryText* story = dig->result<StoryText>();
	assert (story != nullptr);

	StoryText res = *story;
	res.setDoc(doc);

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


QString stringToUnicode(const QString &text)
{
	QString out;

	for (auto c : text)
	{
		auto u = static_cast<uint>(c.unicode());
		out += QString("U+%1,").arg(u, 4, 16, QChar('0')).toUpper();
	}

	return out.mid(0, out.length() - 1);
}

QString unicodeToString(const QString &text)
{
	QString out;
	for (const auto &str : text.split(",", Qt::SkipEmptyParts))
	{
		QString tmp(str);
		tmp.remove("U+");

		bool ok;
		out += QChar(tmp.toInt(&ok, 16));
	}

	return out;
}

void toSentenceCase(StoryText &story, int start, int length)
{
	BreakIterator* it = StoryText::getSentenceIterator();
	if (!it)
		return;

	QString plain = story.plainText().mid(start, length);
	icu::UnicodeString unicodeStr(true, (const UChar*) plain.utf16(), plain.length());
	it->setText(unicodeStr);

	// Lowercase everything in the range
	for (int i = start; i < start + length; ++i)
	{
		QChar ch = story.text(i);
		if (ch.isLetter())
			story.replaceChar(i, ch.toLower());
	}

	// Capitalise first letter of each sentence
	int32_t pos = it->first();
	while (pos != BreakIterator::DONE && pos < plain.length())
	{
		for (int32_t i = pos; i < plain.length(); ++i)
		{
			QChar ch = story.text(start + i);
			if (ch.isLetter())
			{
				story.replaceChar(start + i, ch.toUpper());
				break;
			}
		}
		pos = it->next();
	}
}

void capitalize(StoryText &story, int start, int length)
{
	BreakIterator* it = StoryText::getWordIterator();
	if (!it)
		return;

	QString plain = story.plainText().mid(start, length);
	icu::UnicodeString unicodeStr(true, (const UChar*) plain.utf16(), plain.length());
	it->setText(unicodeStr);

	// Lowercase everything in the range
	for (int i = start; i < start + length; ++i)
	{
		QChar ch = story.text(i);
		if (ch.isLetter())
			story.replaceChar(i, ch.toLower());
	}

	// Capitalise first letter of each word
	int32_t pos = it->first();
	while (pos != BreakIterator::DONE && pos < plain.length())
	{
		QChar ch = story.text(start + pos);
		if (ch.isLetter())
			story.replaceChar(start + pos, ch.toUpper());
		pos = it->next();
	}
}

void toToggleCase(StoryText &story, int start, int length)
{
	for (int i = start; i < start + length; ++i)
	{
		QChar ch = story.text(i);
		if (ch.isUpper())
			story.replaceChar(i, ch.toLower());
		else if (ch.isLower())
			story.replaceChar(i, ch.toUpper());
	}
}
