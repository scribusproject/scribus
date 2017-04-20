/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef SHAPEDTEXTFEED_H
#define SHAPEDTEXTFEED_H


#include <QList>

#include "textshaper.h"
#include "shapedtext.h"

class GlyphCluster;
class ITextContext;
class ITextSource;
class IShapedTextCache;

/**
 * This is a small helper class that automatically calls the shaper when more
 * input is needed.
 */
class ShapedTextFeed
{
	ITextSource* m_textSource;
//	ITextContext* m_context;
	IShapedTextCache* m_cache;
	TextShaper m_shaper;
	int m_endChar;
	
public:
	ShapedTextFeed(ITextSource* source, int startChar, ITextContext* context, IShapedTextCache* cache = NULL);
	
	bool haveMoreText(int glyphPos, QList<GlyphCluster>& glyphs);
	
	static QList<GlyphCluster> putInVisualOrder(const QList<GlyphCluster>& glyphs, int start, int end);
	
private:
	ShapedText getMore(int fromChar, int toChar);
};

#endif


