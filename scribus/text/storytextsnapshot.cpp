/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "storytextsnapshot.h"
#include "storytext.h"
#include "styles/charstyle.h"

StoryTextSnapshot StoryTextSnapshot::create(const StoryText& story)
{
	StoryTextSnapshot snapshot;
	
	int textLength = story.length();
	if (textLength == 0)
		return snapshot;
	
	// 1. Extract plain text (PARSEP will be converted to \n)
	snapshot.plainText = story.plainText();
	
	// 2. Extract paragraph boundaries
	uint numParas = story.nrOfParagraphs();
	snapshot.paragraphs.reserve(numParas);
	
	for (uint i = 0; i < numParas; ++i)
	{
		int start = story.startOfParagraph(i);
		int end = story.endOfParagraph(i);
		int length = end - start + 1;
		
		snapshot.paragraphs.append(ParagraphInfo(start, length));
	}
	
	// 3. Extract language runs
	// Multiple languages can appear in a single paragraph or sentence,
	// so we need to track each contiguous run separately
	snapshot.languages.reserve(textLength / 10); // Heuristic: ~10 chars per run
	
	int runStart = 0;
	QString currentLang;
	
	// Get the language of the first character
	if (textLength > 0)
	{
		const CharStyle& firstStyle = story.charStyle(0);
		currentLang = firstStyle.language();
	}
	
	// Scan through the text to find language changes
	for (int pos = 1; pos < textLength; ++pos)
	{
		const CharStyle& style = story.charStyle(pos);
		QString lang = style.language();
		
		// Check if language has changed
		if (lang != currentLang)
		{
			// Save the previous run if it has a valid language
			if (!currentLang.isEmpty() && (pos - runStart) > 0)
			{
				snapshot.languages.append(
					LanguageRun(runStart, pos - runStart, currentLang)
				);
			}
			
			// Start a new run
			runStart = pos;
			currentLang = lang;
		}
	}
	
	// Don't forget the last run
	if (!currentLang.isEmpty() && (textLength - runStart) > 0)
	{
		snapshot.languages.append(
			LanguageRun(runStart, textLength - runStart, currentLang)
		);
	}
	
	return snapshot;
}

QString StoryTextSnapshot::getParagraphText(int paraIndex) const
{
	if (paraIndex < 0 || paraIndex >= paragraphs.size())
		return QString();
	
	const ParagraphInfo& para = paragraphs[paraIndex];
	
	// Make sure we don't go beyond the text length
	int length = qMin(para.length, plainText.length() - para.start);
	if (length <= 0)
		return QString();
	
	return plainText.mid(para.start, length);
}

QString StoryTextSnapshot::getLanguageAt(int pos) const
{
	if (pos < 0 || pos >= plainText.length())
		return QString();
	
	// Find the language run that contains this position
	for (const LanguageRun& run : languages)
	{
		if (run.contains(pos))
			return run.language;
	}
	
	return QString(); // No language found at this position
}

QVector<LanguageRun> StoryTextSnapshot::getLanguageRunsForParagraph(int paraIndex) const
{
	if (paraIndex < 0 || paraIndex >= paragraphs.size())
		return QVector<LanguageRun>();
	
	const ParagraphInfo& para = paragraphs[paraIndex];
	return getLanguageRunsInRange(para.start, para.end());
}

QVector<LanguageRun> StoryTextSnapshot::getLanguageRunsInRange(int start, int end) const
{
	QVector<LanguageRun> result;
	
	for (const LanguageRun& run : languages)
	{
		// Check if this run intersects with the requested range
		int runStart = run.start;
		int runEnd = run.end();
		
		if (runEnd <= start || runStart >= end)
			continue; // No intersection
		
		// Calculate the intersection
		int intersectStart = qMax(runStart, start);
		int intersectEnd = qMin(runEnd, end);
		
		// Create a new run for the intersection
		// Note: positions are adjusted relative to the original range
		LanguageRun intersectRun;
		intersectRun.start = intersectStart;
		intersectRun.length = intersectEnd - intersectStart;
		intersectRun.language = run.language;
		
		result.append(intersectRun);
	}
	
	return result;
}

int StoryTextSnapshot::getParagraphIndexAt(int pos) const
{
	if (pos < 0 || pos >= plainText.length())
		return -1;
	
	// Binary search for efficiency with large documents
	int left = 0;
	int right = paragraphs.size() - 1;
	
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		const ParagraphInfo& para = paragraphs[mid];
		
		if (pos < para.start)
			right = mid - 1;
		else if (pos >= para.end())
			left = mid + 1;
		else
			return mid; // Found it
	}
	
	return -1; // Not found (shouldn't happen with valid input)
}
