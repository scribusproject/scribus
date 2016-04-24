/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCTEXTSTRUCT_H
#define SCTEXTSTRUCT_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include "scribusapi.h"

#include <QString>

#include "scfonts.h"
#include "style.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"

class PageItem;
class Mark;
class ScribusDoc;

/* Struktur fuer Pageitem Text */


/*
 *  sctext.h
 *  Scribus
 *
 *  Created by Andreas Vox on 29.07.05.
 *  Copyright 2005 under GPL2. All rights reserved.
 *
 */

// from charstlye.h ScStyleFlags
enum LayoutFlags {
	ScLayout_None          = 0,
	ScLayout_BulletNum     = 1,       // new: marks list layout glyphs
	ScLayout_FixedSpace    = 2,       // new: marks a fixed space
	ScLayout_ExpandingSpace= 4,       // new: marks an expanding space
	ScLayout_ImplicitSpace = 8,       // new: marks an implicit space
	ScLayout_TabLeaders    = 16,      // new: marks a tab with fillchar
	ScLayout_HyphenationPossible=128, // Hyphenation possible here (Soft Hyphen)
	ScLayout_DropCap       = 2048,
	ScLayout_SuppressSpace = 4096,    // internal use in PageItem (Suppresses spaces when in Block alignment)
	ScLayout_SoftHyphenVisible=8192,  // Soft Hyphen visible at line end
	ScLayout_StartOfLine   = 16384,   // set for start of line
	ScLayout_Underlined    = 32768    // chararcter should be underlined
};


/**
 * This struct stores a positioned glyph. This is the result of the layout process.
 */
struct SCRIBUS_API GlyphLayout {
	float xadvance;
	float yadvance;
	float xoffset;
	float yoffset;
	double scaleV;
	double scaleH;
	uint glyph;
	
	GlyphLayout() : xadvance(0.0f), yadvance(0.0f), xoffset(0.0f), yoffset(0.0f),
		scaleV(1.0), scaleH(1.0), glyph(0) //, more(NULL)
	{ }
};


class GlyphRun
{
	const CharStyle* m_style;
	LayoutFlags m_flags;
	QList<GlyphLayout> m_glyphs;
	
	int m_firstChar;
	int m_lastChar;
	PageItem* m_object;

public:
	GlyphRun(const CharStyle* style, LayoutFlags flags, int first, int last, PageItem* o)
		: m_style(style)
		, m_flags(flags)
		, m_firstChar(first)
		, m_lastChar(last)
		, m_object(o)
	{}

	const CharStyle&         style()  const { return *m_style; }
	bool       hasFlag(LayoutFlags f) const { return (m_flags & f) == f; }
	void       setFlag(LayoutFlags f)       { m_flags = static_cast<LayoutFlags>(m_flags | f); }
	void     clearFlag(LayoutFlags f)       { m_flags = static_cast<LayoutFlags>(m_flags & ~f); }
	void     clearGlyphs()                  { m_glyphs.clear(); }

	QList<GlyphLayout>&       glyphs()       { return m_glyphs; }
	const QList<GlyphLayout>& glyphs() const { return m_glyphs; }
	int glyphCount()                const    { return m_glyphs.count(); }
	int firstChar()					const	{ return m_firstChar; }
	int lastChar()					const	{ return m_lastChar; }
	qreal width() const;
	PageItem* object()				const	{ return m_object; }
};


class SCRIBUS_API ScText : public CharStyle
{
public:
	ParagraphStyle* parstyle; // only for parseps
	int embedded;
	Mark* mark;
	QChar ch;
	ScText() :
		CharStyle(),
		parstyle(NULL),
		embedded(0), mark(NULL), ch() {}
	ScText(const ScText& other) :
		CharStyle(other),
		parstyle(NULL),
		embedded(other.embedded), mark(NULL), ch(other.ch)
	{
		if (other.parstyle)
			parstyle = new ParagraphStyle(*other.parstyle);
		if (other.mark)
			setNewMark(other.mark);
	}
	~ScText();

	bool hasObject(ScribusDoc *doc) const;
	//returns true if given MRK is found, if MRK is NULL then any mark returns true
	bool hasMark(Mark * MRK = NULL) const;
	QList<PageItem*> getGroupedItems(ScribusDoc *doc);
	PageItem* getItem(ScribusDoc *doc);
private:
	void setNewMark(Mark* mrk);
};


/** @brief First Line Offset Policy
 * Set wether the first line offset is based on max glyph height
 * or some of predefined height.
 * I put a prefix because it could easily conflict 
 */
enum FirstLineOffsetPolicy
{
    FLOPRealGlyphHeight = 0, // Historical
    FLOPFontAscent	 = 1,
    FLOPLineSpacing  = 2,
	FLOPBaselineGrid = 3
};


#endif // SCTEXTSTRUCT_H

