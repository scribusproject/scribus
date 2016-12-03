/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef SCREENPAINTER_H
#define SCREENPAINTER_H

#include <cairo.h>

#include "textlayoutpainter.h"

class ScPainter;

class ScreenPainter: public TextLayoutPainter
{
public:
	ScreenPainter(ScPainter *p, PageItem *item);

	~ScreenPainter();

	void drawGlyph(const GlyphCluster& gc);
	void drawGlyphOutline(const GlyphCluster& gc, bool fill);
	void drawLine(QPointF start, QPointF end);
	void drawRect(QRectF rect);
	void drawObject(PageItem* embedded);
	void clip(QRectF rect);
	void saveState();
	void restoreState();

private:
	void setupState(bool rect);

	ScPainter *m_painter;
	PageItem *m_item;
	TextLayoutColor m_fillColor;
	TextLayoutColor m_strokeColor;
	QColor m_fillQColor;
	QColor m_fillStrokeQColor;
	cairo_font_face_t *m_cairoFace;
	QString m_fontPath;
	int m_faceIndex;
};

#endif // SCREENPAINTER_H
