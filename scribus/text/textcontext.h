/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef TEXTCONTEXT_H
#define TEXTCONTEXT_H

#include "scribusapi.h"
#include "itextcontext.h"
#include "style.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"

class ExpansionPoint;
class InlineFrame;
class PageItem;
class ScribusDoc;

class SCRIBUS_API TextContext : public ITextContext {
	PageItem* m_frame;

public:
	TextContext(PageItem* frame) : m_frame(frame) {}

	virtual const ScribusDoc* getDoc() const;
	virtual const PageItem* getFrame() const { return m_frame; }  
	
	//virtual double height() const  // return getFrame()->height(); }
	//virtual double width() const {} // return getFrame()->width(); }
	
	virtual /* const */ PageItem* object(const InlineFrame& frame) const;
	
	virtual QRectF getVisualBoundingBox(const InlineFrame& frame) const;
	virtual const CharStyle& charStyle() const;
	virtual const ParagraphStyle& paragraphStyle() const;
	virtual QString expand(const ExpansionPoint& expansion); 
	virtual const TypoPrefs& typographicPrefs() const;
};


#endif // TEXTCONTEXT_H
