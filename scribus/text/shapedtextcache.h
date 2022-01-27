/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef SHAPEDTEXTCACHE_H
#define SHAPEDTEXTCACHE_H

#include <QRectF>
#include <QString>
#include <QSharedPointer>

#include "scribusapi.h"

class CharStyle;
class ParagraphStyle;
struct TypoPrefs;
class InlineFrame;
class ExpansionPoint;
class PageItem;
class ShapedText;

/**
 * This class is used to store instances of ShapedText. It can also return segments or combinations
 */
class SCRIBUS_API IShapedTextCache {

public:
	virtual bool contains(int charPos, uint len = 1) const = 0;
	virtual ShapedText get(int charPos, uint minLen=1) const = 0;
	virtual void put(const ShapedText& txt) = 0;
	virtual void clear(int charPos = 0, uint len = -1) = 0;
};


class ShapedTextCacheImplementation;


class SCRIBUS_API ShapedTextCache : public IShapedTextCache
{
	QSharedPointer<ShapedTextCacheImplementation> p_impl;
	
public:
	ShapedTextCache();
	bool contains(int charPos, uint len = 1) const;
	ShapedText get(int charPos, uint minLen=1) const;
	void put(const ShapedText& txt);
	void clear(int charPos = 0, uint len = -1);
};


#endif // SHAPEDTEXTCACHE_H
