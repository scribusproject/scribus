/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "shapedtext.h"

#include <QSharedData>


class ShapedTextImplementation : QSharedData
{
	
public:
	ShapedTextImplementation(ITextSource* src, int firstChar, int lastChar, ITextContext* ctx) : m_needsContext(false), m_source(src), m_context(ctx)
	{
		m_firstChar = firstChar;
		m_lastChar = lastChar < 0? src->length() - 1 : lastChar;
	}
	
	ShapedTextImplementation(const ShapedTextImplementation& o) : m_needsContext(o.m_needsContext), m_source(o.m_source), m_context(o.m_context), m_firstChar(o.m_firstChar), m_lastChar(o.m_lastChar),m_glyphs(o.m_glyphs)
	{}
	
	
	bool m_needsContext;
	const ITextSource* m_source;
	const ITextContext* m_context;
	int m_firstChar;
	int m_lastChar;
	QList<GlyphCluster> m_glyphs;
	
	/** only possible if it also cleanly splits the textsource */
	bool canSplit(int charPos) const
	{
		return splitPosition(charPos) >= 0;
	}
	
	ShapedText split(int charPos)
	{
		int pos = splitPosition(charPos);
		this->m_lastChar = charPos - 1;
		this->m_glyphs.erase(this->m_glyphs.begin() + pos, this->m_glyphs.end());

		ShapedTextImplementation* result = new ShapedTextImplementation(*this);
		result->m_firstChar = charPos;
		result->m_glyphs.erase(result->m_glyphs.begin(), result->m_glyphs.begin() + pos);
		return ShapedText(result);
}
	
	/** only possible if they are adjacent pieces of the same text source */
	bool canCombine(const QSharedPointer<ShapedTextImplementation>  other) const
	{
		return false; // TODO: implement
	}
	
	void combine(QSharedPointer<ShapedTextImplementation>  other)
	{
		// TODO: implement
	}
	
private:
	
	int splitPosition(int charPos) const
	{
		int result = 0;
		while (result < m_glyphs.count() && m_glyphs[result].lastChar() < charPos)
			++result;
		for (int check = result + 1; check < m_glyphs.count(); ++check)
		{
			if (m_glyphs[check].firstChar() < charPos)
				return -1;
		}
		return result;
	}
};



ShapedText ShapedText::Invalid = ShapedText(NULL);

bool ShapedText::isValid() const
{
	return p_impl != NULL && p_impl->m_source != NULL;
}


ShapedText::ShapedText(ShapedTextImplementation* my_p_impl) : p_impl(my_p_impl) {}
ShapedText::ShapedText(ITextSource* src, int firstChar, int lastChar, ITextContext* ctx) : p_impl(new ShapedTextImplementation(src,firstChar,lastChar,ctx)) {}
ShapedText::ShapedText(const ShapedText& other) : p_impl(other.p_impl) {}
	
bool ShapedText::needsContext() const { return p_impl->m_needsContext; }
void ShapedText::needsContext(bool b) { p_impl->m_needsContext = b; }
const ITextSource* ShapedText::source() const { return p_impl->m_source; }
int ShapedText::firstChar() const { return p_impl->m_firstChar; }
int ShapedText::lastChar() const { return p_impl->m_lastChar; }
const QList<GlyphCluster>& ShapedText::glyphs() const { return p_impl->m_glyphs; }
QList<GlyphCluster>& ShapedText::glyphs() { return p_impl->m_glyphs; }

bool ShapedText::canSplit(int pos) const { return p_impl->canSplit(pos); }
ShapedText ShapedText::split(int pos) { return p_impl->split(pos); }
bool ShapedText::canCombine(const ShapedText& other) const { return p_impl->canCombine(other.p_impl); }
void ShapedText::combine(ShapedText& other) { p_impl->combine(other.p_impl); }

