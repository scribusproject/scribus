/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "textlayoutpainter.h"

TextLayoutPainter::TextLayoutPainter()
{
	m_stack.push(State());
}

TextLayoutPainter::~TextLayoutPainter()
{ }

void TextLayoutPainter::setFont(const ScFace font)
{
	if (m_stack.top().font != font)
		m_stack.top().font = font;
}

const ScFace& TextLayoutPainter::font() const
{
	return m_stack.top().font;
}

void TextLayoutPainter::setFontSize(double size)
{
	m_stack.top().fontSize = size;
}

double TextLayoutPainter::fontSize() const
{
	return m_stack.top().fontSize;
}

void TextLayoutPainter::setStrokeColor(const TextLayoutColor color)
{
	m_stack.top().strokeColor = color;
}

const TextLayoutColor& TextLayoutPainter::strokeColor() const
{
	return m_stack.top().strokeColor;
}

void TextLayoutPainter::setFillColor(const TextLayoutColor color)
{
	m_stack.top().fillColor = color;
}

const TextLayoutColor& TextLayoutPainter::fillColor() const
{
	return m_stack.top().fillColor;
}

void TextLayoutPainter::setStrokeWidth(double w)
{
	m_stack.top().strokeWidth = w;
}

double TextLayoutPainter::strokeWidth() const
{
	return m_stack.top().strokeWidth;
}

void TextLayoutPainter::translate(double x, double y)
{
	m_stack.top().x += x;
	m_stack.top().y += y;
}

double TextLayoutPainter::x() const
{
	return m_stack.top().x;
}

double TextLayoutPainter::y() const
{
	return m_stack.top().y;
}

void TextLayoutPainter::setScale(double h, double v)
{
	m_stack.top().scaleH = h;
	m_stack.top().scaleV = v;
}

double TextLayoutPainter::scaleV() const
{
	return m_stack.top().scaleV;
}

double TextLayoutPainter::scaleH() const
{
	return m_stack.top().scaleH;
}

void TextLayoutPainter::setSelected(bool s)
{
	m_stack.top().selected = s;
}

bool TextLayoutPainter::selected() const
{
	return m_stack.top().selected;
}

void TextLayoutPainter::setMatrix(const QTransform matrix)
{
	m_stack.top().matrix = matrix;
}

const QTransform& TextLayoutPainter::matrix() const
{
	return m_stack.top().matrix;
}

void TextLayoutPainter::save()
{
	m_stack.push(m_stack.top());
}

void TextLayoutPainter::restore()
{
	m_stack.pop();
}
