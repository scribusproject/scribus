
#include <QDebug>

#include "shapedtextfeed.h"
#include "shapedtextcache.h"



static bool logicalGlyphRunComp(const GlyphCluster &r1, const GlyphCluster &r2)
{
	return r1.firstChar() < r2.firstChar();
}

static bool visualGlyphRunComp(const GlyphCluster &r1, const GlyphCluster &r2)
{
	return r1.visualIndex() < r2.visualIndex();
}





ShapedTextFeed::ShapedTextFeed(ITextSource* source, int firstChar, ITextContext* context, IShapedTextCache* cache) :
    m_textSource(source),
//    m_context(context),
	m_cache(cache),
    m_shaper(context, *source, firstChar),
	m_endChar(firstChar)
{}


bool ShapedTextFeed::haveMoreText(int glyphPos, QList<GlyphCluster>& glyphs)
{
    while (glyphPos >= glyphs.count())
    {
		int nextChar = m_textSource->nextBlockStart(m_endChar);
		ShapedText more(getMore(m_endChar, nextChar));
		
		if (more.glyphs().count() == 0)
			break;
//		qDebug() << "feed" << m_endChar << "-->" << more.lastChar() + 1;
		m_endChar = more.lastChar();
		int nOldGlyphs = glyphs.count();
		glyphs.append(more.glyphs());
		std::sort(glyphs.begin() + nOldGlyphs, glyphs.end(), logicalGlyphRunComp);
    }
    return glyphPos < glyphs.count();
}



ShapedText ShapedTextFeed::getMore(int fromChar, int toChar)
{
	if (m_cache != NULL)
	{
		int len = toChar - fromChar;
		if (!m_cache->contains(fromChar, len))
		{
			m_cache->put(m_shaper.shape(fromChar, toChar));
		}
		return m_cache->get(fromChar, len);
	}
	else
	{
		return m_shaper.shape(fromChar, toChar);
	}
}



QList<GlyphCluster> ShapedTextFeed::putInVisualOrder(const QList<GlyphCluster>& glyphs, int start, int end)
{
    int glyphsCount = end - start;
    QList<GlyphCluster> runs;
    for (int i = 0; i < glyphsCount; ++i)
        runs.append(glyphs.at(start + i));
    std::sort(runs.begin(), runs.end(), visualGlyphRunComp);
    return runs;
}

