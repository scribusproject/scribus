/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef TEXTLAYOUTPAINTER_H
#define TEXTLAYOUTPAINTER_H

#include <stack>

#include "scribusapi.h"
#include "sctextstruct.h"
#include "glyphcluster.h"


struct TextLayoutColor
{
	QString color;
	double shade;

	TextLayoutColor()
		: color("Black")
		, shade(100)
	{ }

	TextLayoutColor(QString c, double s=100)
		: color(c)
		, shade(s)
	{ }

	bool operator ==(const TextLayoutColor &other) const
	{
		return other.color == color && other.shade == shade;
	}

	bool operator !=(const TextLayoutColor &other) const
	{
		return !(*this == other);
	}
};

/**
 * @brief The TextLayoutPainter class
 *
 * This class provides the API for drawing primitives used to render boxes,
 * each subclass of it will provide the actual implementation for drawing
 * glyphs, lines and rectangle specific for the target format.
 */
class SCRIBUS_API TextLayoutPainter
{
public:
	TextLayoutPainter();
	virtual ~TextLayoutPainter();

	/// Sets the current font that will be used for all subsequent glyph
	/// drawings.
	virtual void setFont(const ScFace font);
	/// The current font.
	virtual const ScFace& font() const;

	/// Sets the current font size that will be used for all subsequent
	/// glyph drawings.
	virtual void setFontSize(double size);
	/// The current font size.
	virtual double fontSize() const;

	/// Sets the current color for line strokes.
	virtual void setStrokeColor(const TextLayoutColor c);
	/// The current color for line strokes.
	virtual const TextLayoutColor& strokeColor() const;

	/// Sets the current foreground color.
	virtual void setFillColor(const TextLayoutColor c);
	/// The current foreground color.
	virtual const TextLayoutColor& fillColor() const;

	/// Sets the current width for line strokes.
	virtual void setStrokeWidth(double w);
	/// The current width for line strokes.
	virtual double strokeWidth() const;

	/// Moves the current x and y positions by the specified amount.
	virtual void translate(double x, double y);
	/// The current x positions.
	virtual double x() const;
	/// The current y positions.
	virtual double y() const;

	/// Sets the current horizontal and vertical scales.
	virtual void setScale(double h, double v);
	/// The current horizontal scale.
	virtual double scaleV() const;
	/// The current vertical scale.
	virtual double scaleH() const;

	/// Sets the selection state of subsequent drawing operations, used for
	/// selecting proper foreground and background colors when drawing text
	/// selection.
	virtual void setSelected(bool s);
	/// The current selection sate.
	virtual bool selected() const;

	/// Sets the transformation matrix to be applied to subsequent drawing
	/// operations.
	virtual void setMatrix(const QTransform);
	/// The current transformation matrix.
	virtual const QTransform& matrix() const;

	/// Draws a regular (filled) glyph using the current font, fill color
	/// etc. at the current x and y positions.
	virtual void drawGlyph(const GlyphCluster& gc) = 0;
	/// Same as drawGlyphs() but draws an outlined glyph with current
	/// stroke color, if @fill is true then the glyphs is also filled by
	/// the current fill color.
	virtual void drawGlyphOutline(const GlyphCluster& gc, bool fill) = 0;
	/// Draws a line from @start to @end relative current x and y
	/// positions, with current stroke color and width.
	virtual void drawLine(QPointF start, QPointF end) = 0;
	/// Draws a rectangle at current x and y positions, using current stoke
	/// color and width for its border, and filled with the current fill
	/// color.
	virtual void drawRect(QRectF rect) = 0;
	/// Draws and embedded page item at the current x and y positions.
	virtual void drawObject(PageItem* item) = 0;

	/// Save the current painter state.
	virtual void save();
	/// Restore the last saved painter state.
	virtual void restore();

private:
	struct State
	{
		ScFace font;
		double fontSize;
		TextLayoutColor strokeColor;
		TextLayoutColor fillColor;
		QTransform matrix;
		double strokeWidth;
		double x;
		double y;
		double scaleH;
		double scaleV;
		bool selected;

		State()
			: fontSize(0)
			, strokeWidth(0)
			, x(0)
			, y(0)
			, scaleH(1)
			, scaleV(1)
			, selected(false)
		{}
	};

	std::stack<State> m_stack;
};

#endif // TEXTLAYOUTPAINTER_H
