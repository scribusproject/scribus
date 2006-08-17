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
#include "text/nlsconfig.h"

#ifdef NLS_CONFORMANCE
#define NLS_PRIVATE private
#else
#define NLS_PRIVATE public
#endif

#include <qstring.h>

#include "scfonts.h"
#include "style.h"

class PageItem;

/* Struktur fuer Pageitem Text */


/*
 *  sctext.h
 *  Scribus
 *
 *  Created by Andreas Vox on 29.07.05.
 *  Copyright 2005 under GPL2. All rights reserved.
 *
 */



/**
 * This struct stores a positioned glyph. This is the result of the layout process.
 * If a char gets translated to more than one glyph, a linked list is built.
 */
struct SCRIBUS_API GlyphLayout {
	float xadvance;
	float yadvance;
	float xoffset;
	float yoffset;
	double scaleV;
	double scaleH;
	uint glyph;
	GlyphLayout* more;
	
	GlyphLayout() : xadvance(0.0f), yadvance(0.0f), xoffset(0.0f), yoffset(0.0f),
		scaleV(1.0), scaleH(1.0), glyph(0), more(NULL) 
	{ }
	double wide() const 
	{ 
		double ret = 0; 
		for(const GlyphLayout* p=this; p; p=p->more) 
			ret += p->xadvance; 
		return ret; 
	}
	GlyphLayout* last() 
	{ 
		if (more) 
			return more->last();
		else 
			return this;
	}
	void shrink()
	{
		if (more) {
			more->shrink();
			delete more;
			more = NULL;
		}
	}
	void grow()
	{
		if (!more) {
			more = new GlyphLayout();
		}
	}
	
};

#ifndef NLS_PROTO
class SCRIBUS_API ScText : public CharStyle
{
public:
//	bool cselect; // to go
//	short cab; // to go
	ParagraphStyle* parstyle; // only for parseps
	GlyphLayout glyph;
	float PtransX;
	float PtransY;
	float PRot;
	PageItem* cembedded;
	QString ch;
	ScText() : //cselect(false), cab(0), 
		parstyle(NULL), glyph(), 
		PtransX(0.0f), PtransY(0.0f), PRot(0.0f), cembedded(NULL), ch() {}
	ScText(const ScText& other) : 
		Style(other), CharStyle(other),
		parstyle(NULL), glyph(other.glyph), 
		PtransX(other.PtransX), PtransY(other.PtransY), PRot(other.PRot), 
		cembedded(other.cembedded), ch(other.ch)
	{
		if (other.parstyle)
			parstyle = new ParagraphStyle(*other.parstyle);
	}
	~ScText();
};
#endif

#endif // SCTEXTSTRUCT_H

