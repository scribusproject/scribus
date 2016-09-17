/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "shapedtextcache.h"

#include "index.h"
#include "shapedtext.h"


class ShapedTextCacheImplementation {
	
	IndexedList<ShapedText> m_cache;
	
	
public:
	
	// check if all ShapedText in the cache for this range are valid, i.e. source() != NULL
	bool contains(int charPos, uint len) const
	{
		int last = charPos + len - 1;
		if (!m_cache.containsPosition(charPos) || !m_cache.containsPosition(last))
			return false;
		
		ShapedText st = m_cache(charPos);
		while (st.isValid() && st.lastChar() < last)
		{
			st = m_cache(st.lastChar() + 1);
		}
		
		return st.isValid();
	}
	
	
	ShapedText get(int charPos, uint minLen) const
	{
		int last = charPos + minLen - 1;
		if (!m_cache.containsPosition(charPos) || !m_cache.containsPosition(last))
			return ShapedText::Invalid;

		ShapedText result = m_cache(charPos);
		if (!result.isValid())
			return ShapedText::Invalid;
		
		if (!m_cache.isExactPosition(charPos))
		{
			result = result.split(charPos);
		}
		while (result.lastChar() < last)
		{
			ShapedText st = m_cache(result.lastChar() + 1);
			if (st.source() == NULL || !result.canCombine(st))
				return ShapedText::Invalid;
			result.combine(st);
		}
		return result;
	}
	
	
	void put(const ShapedText& txt)
	{
		int first = txt.firstChar();
		int len = txt.lastChar() + 1 - first;
		
		m_cache.replace(first, len , txt);
	}
	
	void clear(int charPos, uint len)
	{
		put(ShapedText(NULL, charPos, charPos + len - 1));
	}

	
};


ShapedTextCache::ShapedTextCache() : p_impl(new ShapedTextCacheImplementation()) {}

bool ShapedTextCache::contains(int charPos, uint len) const
{ return p_impl->contains(charPos, len); }

ShapedText ShapedTextCache::get(int charPos, uint minLen) const
{ return p_impl->get(charPos, minLen); }

void ShapedTextCache::put(const ShapedText& txt)
{ p_impl->put(txt); }

void ShapedTextCache::clear(int charPos, uint len)
{ p_impl->clear(charPos, len); }
