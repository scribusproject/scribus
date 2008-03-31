/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPAGEOUTPUT_H
#define SCPAGEOUTPUT_H

#include <QRect>
#include <QRectF>

#include "scribusapi.h"
#include "sccolor.h"
#include "scpainterexbase.h"

class ScribusDoc;
class Page;
class CharStyle;
struct GlyphLayout;

class PageItem;
class PageItem_ImageFrame;
class PageItem_Line;
class PageItem_PathText;
class PageItem_Polygon;
class PageItem_PolyLine;
class PageItem_TextFrame;

class SCRIBUS_API MarksOptions
{
public:
	MarksOptions(void);
	MarksOptions(struct PrintOptions& opt);
	double markOffset;
	double BleedTop;
	double BleedLeft;
	double BleedRight;
	double BleedBottom;
	bool   cropMarks;
	bool   bleedMarks;
	bool   registrationMarks;
	bool   colorMarks;
	bool   docInfoMarks;
};

class SCRIBUS_API ScPageOutput
{
protected:

	ScribusDoc* m_doc;

	bool m_reloadImages;
	int  m_imageRes;
	bool m_useProfiles;
	MarksOptions m_marksOptions;

	virtual void fillPath( PageItem* item, ScPainterExBase* painter, const QRect& clip );
	virtual void strokePath( PageItem* item, ScPainterExBase* painter, const QRect& clip );

	virtual void drawMasterItems( ScPainterExBase *painter, Page *page, const QRect& clip);
	virtual void drawPageItems( ScPainterExBase *painter, Page *page, const QRect& clip);

	virtual void drawItem( PageItem* item, ScPainterExBase* painter, const QRect& clip );
	virtual void drawItem_Pre( PageItem* item, ScPainterExBase* painter );
	virtual void drawItem_Post( PageItem* item, ScPainterExBase* painter );

	virtual void drawGlyphs(PageItem* item, ScPainterExBase *painter, const CharStyle& style, GlyphLayout& glyphs, const QRect& clip);
	virtual void drawItem_Embedded( PageItem* item, ScPainterExBase *p, const QRect& clip, const CharStyle& style, PageItem* cembedded);
	virtual void drawPattern( PageItem* item, ScPainterExBase* painter, const QRect& clip);
	
	virtual void drawItem_ImageFrame( PageItem_ImageFrame* item, ScPainterExBase* painter, const QRect& clip );
	virtual void drawItem_Line( PageItem_Line* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_PathText( PageItem_PathText* item, ScPainterExBase* painter, const QRect& clip );
	virtual void drawItem_Polygon ( PageItem_Polygon* item , ScPainterExBase* painter, const QRect& clip );
	virtual void drawItem_PolyLine( PageItem_PolyLine* item, ScPainterExBase* painte, const QRect& clip );
	virtual void drawItem_TextFrame( PageItem_TextFrame* item, ScPainterExBase* painter, const QRect& clip );

	virtual void drawMarks( Page* page, ScPainterExBase* painter, const MarksOptions& options );
	virtual void drawBoxMarks( ScPainterExBase* painter, const QRectF& box, const QRectF& bleedBox, double offset );
	virtual void drawRegistrationCross( ScPainterExBase* painter );

	ScImage::RequestType translateImageModeToRequest( ScPainterExBase::ImageMode mode);

public:
	virtual ~ScPageOutput() { }

	ScPageOutput(ScribusDoc* doc, bool reloadImages = false, int resolution = 72, bool useProfiles = false);

	virtual void begin(void) {};
	virtual void drawPage( Page* page ) {};
	virtual void drawPage( Page* page, ScPainterExBase* painter);
	virtual void end(void) {};

	void setMarksOptions(const MarksOptions& opt) { m_marksOptions = opt; }
};

#endif
