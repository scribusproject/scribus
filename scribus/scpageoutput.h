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
#include "scribusstructs.h"
#include "sccolor.h"
#include "scimage.h"
#include "scpainterexbase.h"

class CharStyle;
struct GlyphLayout;
class ScPage;
class PageItem;
class PageItem_Arc;
class PageItem_Group;
class PageItem_ImageFrame;
class PageItem_Line;
class PageItem_PathText;
class PageItem_Polygon;
class PageItem_PolyLine;
class PageItem_Spiral;
class PageItem_RegularPolygon;
class PageItem_Table;
class PageItem_TextFrame;
class ScLayer;
class ScribusDoc;

class SCRIBUS_API MarksOptions
{
public:
	MarksOptions(void) = default;
	MarksOptions(const PrintOptions& opt);

	double markLength { 20.0 };
	double markOffset { 0.0 };
	double bleedTop { 0.0 };
	double bleedLeft { 0.0 };
	double bleedRight { 0.0 };
	double bleedBottom { 0.0 };
	bool   cropMarks { false };
	bool   bleedMarks { false };
	bool   registrationMarks { false };
	bool   colorMarks { false };
	bool   docInfoMarks { false };
};

class SCRIBUS_API ScPageOutput
{
	friend class ScPageOutputPainter;

public:
	ScPageOutput(ScribusDoc* doc, bool reloadImages = false, int resolution = 72, bool useProfiles = false);
	virtual ~ScPageOutput() { }

	virtual void begin(void) {};
	virtual void drawPage( ScPage* page ) {};
	virtual void drawPage( ScPage* page, ScPainterExBase* painter);
	virtual void end(void) {};

	void setMarksOptions(const MarksOptions& opt) { m_marksOptions = opt; }

protected:
	ScribusDoc* m_doc { nullptr };

	bool m_reloadImages { true };
	int  m_imageRes { 72 };
	bool m_useProfiles { false };
	MarksOptions m_marksOptions;

	virtual void fillPath(PageItem* item, ScPainterExBase* painter, const QRect& clip);
	virtual void strokePath(PageItem* item, ScPainterExBase* painter, const QRect& clip);

	virtual void drawMasterItems(ScPainterExBase* painter, ScPage* page, const ScLayer& layer, const QRect& clip);
	virtual void drawPageItems(ScPainterExBase* painter, ScPage* page, const ScLayer& layer, const QRect& clip);

	virtual void drawItem(PageItem* item, ScPainterExBase* painter, QRect clip);
	virtual void drawItem_Pre(PageItem* item, ScPainterExBase* painter);
	virtual void drawItem_Post(PageItem* item, ScPainterExBase* painter);

	virtual void drawPattern(PageItem* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawStrokePattern(PageItem* item, ScPainterExBase* painter, const QPainterPath& path);

	virtual void drawItem_Arc(PageItem_Arc* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_Group(PageItem_Group* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_ImageFrame(PageItem_ImageFrame* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_Line(PageItem_Line* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_Polygon(PageItem_Polygon* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_PolyLine(PageItem_PolyLine* item, ScPainterExBase* painte, const QRect& clip);
	virtual void drawItem_RegularPolygon(PageItem_RegularPolygon* item, ScPainterExBase* painte, const QRect& clip);
	virtual void drawItem_Spiral(PageItem_Spiral* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_Table(PageItem_Table* item, ScPainterExBase* painter, const QRect& clip);
	virtual void drawItem_Text(PageItem* item, ScPainterExBase* painter, const QRect& clip);

	virtual void drawArrow(ScPainterExBase* painter, PageItem* item, QTransform& arrowTrans, int arrowIndex);
	virtual void drawMarks(ScPage* page, ScPainterExBase* painter, const MarksOptions& options);
	virtual void drawBoxMarks(ScPainterExBase* painter, const QRectF& box, const QRectF& bleedBox, double offset, double markSize);
	virtual void drawRegistrationCross(ScPainterExBase* painter);

	ScImage::RequestType translateImageModeToRequest(ScPainterExBase::ImageMode mode) const;

	friend class CollapsedTablePainterEx;
};

#endif
