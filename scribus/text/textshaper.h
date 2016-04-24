/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
 
#ifndef TEXTSHAPER_H
#define TEXTSHAPER_H

#include <QList>

#include "scribusapi.h"
#include "sctextstruct.h"
#include "text/storytext.h"

class PageItem_TextFrame;

class SCRIBUS_API TextShaper
{
public:
	TextShaper(PageItem_TextFrame* textItem, int startIndex = 0);

	/**
	 * Test if run at specifid index can be retrieved
	 */
	bool hasRun(int i);

	/**
	 * Retrieve glyph run at specified index
	 */
	GlyphRun runAt(int i);

	/**
	 * Indicates that a new line is being started
	 */
	void startLine(int i);
	
protected:

	// Character index from where shaping start
	int m_startIndex;

	// Next character index to be shaped
	int m_index;

	// Last run which has been kerned
	int m_lastKernedIndex;

	// Additional flags used for glyph layout
	LayoutFlags m_layoutFlags;
	
	// The item whose text is being shaped
	PageItem_TextFrame* m_item;

	// Glyph runs waiting to be retrieved
	QList<GlyphRun> m_runs;

	// Get next chars and put them in char queue
	void needChars(int runIndex);

	// Initialize layout data and glyph list of a text run
	void initGlyphLayout(GlyphRun& run, const QString& chars, int runIndex);
};

#endif
