/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPAGEOUTPUT_H
#define SCPAGEOUTPUT_H

#include "scribusapi.h"
#include "scribusdoc.h"
#include "page.h"
#include "sccolor.h"
#include "scpainterexbase.h"

class PageItem;
class PageItem_ImageFrame;
class PageItem_Line;
class PageItem_PathText;
class PageItem_Polygon;
class PageItem_PolyLine;
class PageItem_TextFrame;

class SCRIBUS_API ScPageOutput
{
protected:

	ScribusDoc* m_doc;

	bool m_reloadImages;
	int m_imageRes;
	bool m_useProfiles;

	virtual void FillPath( PageItem* item, ScPainterExBase* painter, QRect& clip );
	virtual void StrokePath( PageItem* item, ScPainterExBase* painter, QRect& clip );

	virtual void DrawMasterItems( ScPainterExBase *painter, Page *page, QRect& clip);
	virtual void DrawPageItems( ScPainterExBase *painter, Page *page, QRect& clip);

	virtual void DrawItem( PageItem* item, ScPainterExBase* painter, QRect& clip );
	virtual void DrawItem_Pre( PageItem* item, ScPainterExBase* painter );
	virtual void DrawItem_Post( PageItem* item, ScPainterExBase* painter );

	virtual void DrawGlyphs(PageItem* item, ScPainterExBase *painter, const CharStyle& style, GlyphLayout& glyphs, QRect& clip);
	virtual void DrawItem_Embedded( PageItem* item, ScPainterExBase *p, QRect& clip, const CharStyle& style, PageItem* cembedded);
	virtual void DrawPattern( PageItem* item, ScPainterExBase* painter, QRect& clip);
	
	virtual void DrawItem_ImageFrame( PageItem_ImageFrame* item, ScPainterExBase* painter, QRect& clip );
	virtual void DrawItem_Line( PageItem_Line* item, ScPainterExBase* painter, QRect& clip);
	virtual void DrawItem_PathText( PageItem_PathText* item, ScPainterExBase* painter, QRect& clip );
	virtual void DrawItem_Polygon ( PageItem_Polygon* item , ScPainterExBase* painter, QRect& clip );
	virtual void DrawItem_PolyLine( PageItem_PolyLine* item, ScPainterExBase* painte, QRect& clip );
	virtual void DrawItem_TextFrame( PageItem_TextFrame* item, ScPainterExBase* painter, QRect& clip );

	ScImage::RequestType translateImageModeToRequest( ScPainterExBase::ImageMode mode);

public:

	ScPageOutput(ScribusDoc* doc, bool reloadImages = false, int resolution = 72, bool useProfiles = false);

	virtual void begin(void) {};
	virtual void DrawPage( Page* page ) {};
	virtual void DrawPage( Page* page, ScPainterExBase* painter);
	virtual void end(void) {};
};

#endif
