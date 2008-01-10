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

	virtual void FillPath( PageItem* item, ScPainterExBase* painter, const QRect& clip );
	virtual void StrokePath( PageItem* item, ScPainterExBase* painter, const QRect& clip );

	virtual void DrawMasterItems( ScPainterExBase *painter, Page *page, const QRect& clip);
	virtual void DrawPageItems( ScPainterExBase *painter, Page *page, const QRect& clip);

	virtual void DrawItem( PageItem* item, ScPainterExBase* painter, const QRect& clip );
	virtual void DrawItem_Pre( PageItem* item, ScPainterExBase* painter );
	virtual void DrawItem_Post( PageItem* item, ScPainterExBase* painter );

	virtual void DrawGlyphs(PageItem* item, ScPainterExBase *painter, const CharStyle& style, GlyphLayout& glyphs, const QRect& clip);
	virtual void DrawItem_Embedded( PageItem* item, ScPainterExBase *p, const QRect& clip, const CharStyle& style, PageItem* cembedded);
	virtual void DrawPattern( PageItem* item, ScPainterExBase* painter, const QRect& clip);
	
	virtual void DrawItem_ImageFrame( PageItem_ImageFrame* item, ScPainterExBase* painter, const QRect& clip );
	virtual void DrawItem_Line( PageItem_Line* item, ScPainterExBase* painter, const QRect& clip);
	virtual void DrawItem_PathText( PageItem_PathText* item, ScPainterExBase* painter, const QRect& clip );
	virtual void DrawItem_Polygon ( PageItem_Polygon* item , ScPainterExBase* painter, const QRect& clip );
	virtual void DrawItem_PolyLine( PageItem_PolyLine* item, ScPainterExBase* painte, const QRect& clip );
	virtual void DrawItem_TextFrame( PageItem_TextFrame* item, ScPainterExBase* painter, const QRect& clip );

	virtual void DrawMarks( Page* page, ScPainterExBase* painter, const MarksOptions& options );
	virtual void DrawBoxMarks( ScPainterExBase* painter, const QRectF& box, const QRectF& bleedBox, double offset );
	virtual void DrawRegistrationCross( ScPainterExBase* painter );

	ScImage::RequestType translateImageModeToRequest( ScPainterExBase::ImageMode mode);

public:
	virtual ~ScPageOutput() { }

	ScPageOutput(ScribusDoc* doc, bool reloadImages = false, int resolution = 72, bool useProfiles = false);

	virtual void begin(void) {};
	virtual void DrawPage( Page* page ) {};
	virtual void DrawPage( Page* page, ScPainterExBase* painter);
	virtual void end(void) {};

	void setMarksOptions(const MarksOptions& opt) { m_marksOptions = opt; }
};

#endif
