/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef BOXES_H
#define BOXES_H

#include "glyphcluster.h"
#include "sctextstruct.h"
#include "itextcontext.h"

class StoryText;
class TextLayoutPainter;
class ScreenPainter;

/**
 Class Box has a similar role as TeX's boxes.
 Scribus packs glyph runs into GlyphBoxes, GlyphBoxes and ObjectBoxes into LineBoxes
 and LineBoxes into GroupBox(T_Block).
 (and in the future: math atoms, tables & table cells, ...)
 */
class Box
{
public:
	enum BoxType
	{
		T_Invalid,
		T_Block,
		T_Line,
		T_PathLine,
		T_Glyph,
		T_Object
	};

	enum BoxDirection
	{
		D_Horizontal,
		D_Vertical
	};

	Box() = default;

	virtual ~Box()
	{
		while (!m_boxes.isEmpty())
			delete m_boxes.takeFirst();
	}

	/// The x position of the box relative to its parent.
	double x() const { return m_x; }
	/// The y position of the box relative to its parent.
	double y() const { return m_y; }
	/// Sets x and y positions of the box.
	void moveTo (double x, double y) { m_x = x, m_y = y; }
	/// Increments x and y positions of the box.
	void moveBy (double x, double y) { m_x += x, m_y += y; }

	/// The box width, can be different from its natural width.
	double width() const { return m_width; }
	/// Set the box width.
	void setWidth(double w) { m_width = w; }

	/// The box ascender above baseline.
	double ascent() const { return m_ascent; }
	/// The box descender below baseline.
	double descent() const { return m_descent; }
	/// The box height, can be different from its natural height.
	double height() const { return m_ascent - m_descent; }
	/// Set the box ascender.
	void setAscent(double a) { m_ascent = a; }
	/// Set the box descender.
	void setDescent(double d) { m_descent = d; }

	/// The actual width of the box contents, can be different from the requested width.
	virtual double naturalWidth() const { return width(); }
	/// The actual height of the box contents, can be different from the requested height.
	virtual double naturalHeight() const { return height(); }

	/// The bounding box and position of the box relative to its parent.
	QRectF bbox() const { return QRectF(m_x, m_y, m_width, height()); }

	/// Whether the coordinate is inside the box or not.
	virtual bool containsPoint(const QPointF& coord) const { return bbox().contains(coord); }
	/// Whether the character at index pos is inside the box or not.
	bool containsPos(int pos) const { return firstChar() <= pos && pos <= lastChar(); }

	/// Returns the character index at coorddinate.
	virtual int pointToPosition(const QPointF& coord, const StoryText &story) const = 0;
	/// Returns the position of cursor before the character at index pos.
	virtual QLineF positionToPoint(int, const StoryText&) const { return QLineF(); }

	/// The first character within the box.
	int firstChar() const { return m_firstChar == INT_MAX ? 0 : m_firstChar; }
	/// The last character within the box.
	int lastChar() const { return m_lastChar == INT_MIN ? 0 : m_lastChar; }

	/// Sets the transformation matrix to applied to the box.
	void setMatrix(const QTransform& x) { m_matrix = x; }

	/// The transformation matrix applied to the box.
	const QTransform& matrix() const { return m_matrix; }

//	virtual void justify(const ParagraphStyle& style) {}

	/// Returns the children of the box.
	QList<Box*>& boxes() { return m_boxes; }
	const QList<const Box*>& boxes() const {
		return reinterpret_cast<const QList<const Box*> & > (m_boxes);
	}

	/// Returns the number of child boxes
	int boxCount() const { return m_boxes.count(); }

	/// Returns if current box contains any child boxes
	bool isEmpty() const { return m_boxes.isEmpty(); }

	/// Renders the box and any boxes it contains, recursively.
	virtual void render(TextLayoutPainter *p) const = 0;

	/// Same as render() but handles text selection, for rendering on screen.
	virtual void render(ScreenPainter *p, ITextContext *ctx) const = 0;

	/// Draw selection if the the box selected
	virtual void drawSelection(ScreenPainter *p, ITextContext *ctx) const {}

	/// Get natural ascent and descent.
	virtual double naturalAsc() const { return m_naturalAscent; }
	virtual double naturalDescent() const { return m_naturalDescent; }

	/// return box type
	BoxType type() const { return m_type; }

protected:
	BoxType m_type { T_Invalid };
	BoxDirection m_direction { D_Horizontal };
	double m_x { 0.0 };
	double m_y { 0.0 };
	double m_width { 0.0 };
	double m_descent { 0.0 };
	double m_ascent { 0.0 };
	QList<Box*> m_boxes;
	int m_firstChar { 0 };
	int m_lastChar { 0 };
	QTransform m_matrix;
	double m_naturalAscent { 0.0 };
	double m_naturalDescent { 0.0 };
};


class GroupBox: public Box
{
public:
	explicit GroupBox(BoxDirection direction)
	{
		m_type = T_Block;
		m_direction = direction;
		m_firstChar = INT_MAX;
		m_lastChar = INT_MIN;
		m_naturalWidth = m_naturalHeight = 0;
	}

	int pointToPosition(const QPointF& coord, const StoryText &story) const override;
	QLineF positionToPoint(int pos, const StoryText& story) const override;

	void render(TextLayoutPainter *p) const override;
	void render(ScreenPainter *p, ITextContext *ctx) const override;
	void drawSelection(ScreenPainter *p, ITextContext *ctx) const override;

	double naturalWidth() const override { return m_naturalWidth; }
	double naturalHeight() const override;

//	void justify(const ParagraphStyle& style);

	/// Adds a new child to the box.
	virtual void addBox(const Box* box);
	/// Remove the child at i.
	virtual void removeBox(int i);

protected:
	double m_naturalWidth;
	double m_naturalHeight;

private:
	void update();
};


class LineBox: public GroupBox
{
public:
	LineBox()
		: GroupBox(D_Horizontal)
	{
		m_type = T_Line;
	}

	int pointToPosition(const QPointF& coord, const StoryText &story) const override;
	QLineF positionToPoint(int pos, const StoryText& story) const override;

	bool containsPoint(const QPointF& coord) const override;

	void render(TextLayoutPainter *p) const override;
	void render(ScreenPainter *p, ITextContext *ctx) const override;
	void drawSelection(ScreenPainter *p, ITextContext *ctx) const override;

	double naturalWidth() const override{ return m_naturalWidth; }
	double naturalHeight() const override{ return height(); }

//	void justify(const ParagraphStyle& style);

	void addBox(const Box* box) override;
	void removeBox(int i) override;

protected:
	virtual void drawBackGround(TextLayoutPainter *p) const;
	virtual void update();
};

class PathLineBox: public LineBox
{
public:
	PathLineBox()
	{
		m_type = T_PathLine;
	}

protected:
	void update() override;
	void drawBackGround(TextLayoutPainter *p) const override;
};

class GlyphBox: public Box
{
public:
	explicit GlyphBox(const GlyphCluster& run)
		: m_glyphRun(run)
		, m_effects(run.style().effects())
	{
		m_type = T_Glyph;
		m_firstChar = run.firstChar();
		m_lastChar = run.lastChar();
		m_width = run.width();
		m_naturalAscent = run.ascent();
		m_naturalDescent = run.descent();
	}

	int pointToPosition(const QPointF& coord, const StoryText &story) const override;
	QLineF positionToPoint(int pos, const StoryText& story) const override;

	void render(TextLayoutPainter *p) const override;
	void render(ScreenPainter *p, ITextContext *ctx) const override;
	void drawSelection(ScreenPainter *p, ITextContext *ctx) const override;

	GlyphCluster glyphRun() const { return m_glyphRun; }

	const CharStyle& style() const { return m_glyphRun.style(); }

protected:
	GlyphCluster m_glyphRun;
	const StyleFlag m_effects;
};

class ObjectBox: public GlyphBox
{
public:
	ObjectBox(const GlyphCluster& run, const ITextContext* ctx)
		: GlyphBox(run)
		, m_object(ctx->object(run.object()))
	{
		m_type = T_Object;
	}

	void render(TextLayoutPainter *p) const override;
	void render(ScreenPainter*, ITextContext *ctx) const override;

	void update();

private:
	/* const */ PageItem* m_object;
};

#endif // BOXES_H
