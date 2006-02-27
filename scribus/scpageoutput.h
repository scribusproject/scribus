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
	double m_scale;

	bool m_reloadImages;
	int m_imageRes;
	bool m_useProfiles;

	virtual void DrawMasterItems(ScPainterExBase *painter, Page *page, QRect clip);
	virtual void DrawPageItems(ScPainterExBase *painter, Page *page, QRect clip);

	virtual void DrawItem( PageItem* item, ScPainterExBase* painter, QRect rect );
	virtual void DrawItem_Pre( PageItem* item, ScPainterExBase* painter, double scale );
	virtual void DrawItem_Post( PageItem* item, ScPainterExBase* painter );

	virtual void DrawCharacters( PageItem* item, ScPainterExBase *p, struct PageItem::ZZ *hl);
	virtual void DrawItem_Embedded( PageItem* item, ScPainterExBase *p, QRect e, struct PageItem::ZZ *hl);
	
	virtual void DrawItem_ImageFrame( PageItem_ImageFrame* item, ScPainterExBase* painter, double scale  );
	virtual void DrawItem_Line( PageItem_Line* item, ScPainterExBase* painter );
	virtual void DrawItem_PathText( PageItem_PathText* item, ScPainterExBase* painter, double scale );
	virtual void DrawItem_Polygon ( PageItem_Polygon* item , ScPainterExBase* painter );
	virtual void DrawItem_PolyLine( PageItem_PolyLine* item, ScPainterExBase* painter );
	virtual void DrawItem_TextFrame( PageItem_TextFrame* item, ScPainterExBase* painter, QRect e, double scale );

public:

	ScPageOutput(ScribusDoc* doc, bool reloadImages = false, int resolution = 72, bool useProfiles = false);

	virtual void begin(void) {};
	virtual void DrawPage( Page* page ) {};
	virtual void DrawPage( Page* page, ScPainterExBase* painter);
	virtual void end(void) {};

	double getScale(void) const { return m_scale; }
	void   setScale(double value) { m_scale = value; }
};

#endif
