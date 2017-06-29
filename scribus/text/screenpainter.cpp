/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include <cairo.h>
#if CAIRO_HAS_FC_FONT
#include <cairo-ft.h>
#endif

#include "screenpainter.h"
#include "scpainter.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "scribusapp.h"
#include "util.h"

ScreenPainter::ScreenPainter(ScPainter *p, PageItem *item)
	: m_painter(p)
	, m_item(item)
	, m_fillColor("", -1)
	, m_strokeColor("", -1)
	, m_cairoFace(NULL)
	, m_faceIndex(-10) // ScFace::faceIndex() defaults to -1, we need a different value
{
	m_painter->save();

	// Use slight hinting to be closer to old Scribus behaviour
	// We can use CAIRO_HINT_STYLE_NONE to be even more closer, but
	// slight is a good compromise
	cairo_font_options_t* options = cairo_font_options_create();
	cairo_font_options_set_hint_style(options, CAIRO_HINT_STYLE_SLIGHT);
	cairo_set_font_options(m_painter->context(), options);
	cairo_font_options_destroy(options);
}

ScreenPainter::~ScreenPainter()
{
	if (m_cairoFace != NULL)
		cairo_font_face_destroy(m_cairoFace);
	m_painter->restore();
}

void ScreenPainter::drawGlyph(const GlyphCluster& gc)
{
	bool showControls = gc.isEmpty() || (m_item->doc()->guidesPrefs().showControls &&
										 ((gc.isSpace() && !gc.hasFlag(ScLayout_ImplicitSpace)) || gc.isControlGlyphs()));
#if CAIRO_HAS_FC_FONT
	if (m_painter->fillMode() == 1 && m_painter->maskMode() <= 0 && !showControls)
	{
		m_painter->save();

		setupState(false);

		cairo_t* cr = m_painter->context();
		double r, g, b;
		m_painter->brush().getRgbF(&r, &g, &b);
		cairo_set_source_rgba(cr, r, g, b, m_painter->brushOpacity());
		m_painter->setRasterOp(m_painter->blendModeFill());

		if (m_fontPath != font().fontFilePath() || m_faceIndex != font().faceIndex() || m_cairoFace == NULL)
		{
			m_fontPath = font().fontFilePath();
			m_faceIndex = font().faceIndex();
			// A very ugly hack as we can’t use the font().ftFace() because
			// Scribus liberally calls FT_Set_CharSize() with all sorts of
			// crazy values, breaking any subsequent call to the layout
			// painter.  FIXME: drop the FontConfig dependency here once
			// Scribus font handling code is made sane!
			FcPattern *pattern = FcPatternBuild(NULL,
							    FC_FILE, FcTypeString, QFile::encodeName(font().fontFilePath()).data(),
							    FC_INDEX, FcTypeInteger, font().faceIndex(),
							    NULL);
			m_cairoFace = cairo_ft_font_face_create_for_pattern(pattern);
			FcPatternDestroy(pattern);
		}

		cairo_set_font_face(cr, m_cairoFace);
		cairo_set_font_size(cr, fontSize());

		double current_x = 0.0;
		foreach (const GlyphLayout& gl, gc.glyphs()) {
			cairo_scale(cr, gl.scaleH, gl.scaleV);
			cairo_glyph_t glyph = { gl.glyph, gl.xoffset + current_x, gl.yoffset };
			cairo_show_glyphs(cr, &glyph, 1);
			current_x += gl.xadvance;
		}
		m_painter->restore();
		return;
	}
#endif
	m_painter->save();

	setupState(false);

	bool fr = m_painter->fillRule();
	m_painter->setFillRule(false);

	//All ControlGlyphs have only one GlyphLayout
	uint gid = gc.glyphs().first().glyph;
	if (showControls)
	{
		bool stroke = false;
		if (gid >= ScFace::CONTROL_GLYPHS)
			gid -= ScFace::CONTROL_GLYPHS;
		else if (gid != 0)
			gid = 32;
		QTransform chma, chma4;
		FPointArray outline;
		if (gid == 0)
		{
			outline = font().glyphOutline(gc.glyphs().first().glyph);
			if (outline.size() <= 3)
			{
				outline.clear();
				double sz = 10;
				outline.addQuadPoint(0,0,0,0,sz,0,sz,0);
				outline.addQuadPoint(sz,0,sz,0,sz,sz,sz,sz);
				outline.addQuadPoint(sz,sz,sz,sz,0,sz,0,sz);
				outline.addQuadPoint(0,sz,0,sz,0,0,0,0);
				stroke = true;
			}
			chma4.translate(0, -fontSize() * gc.scaleV() * 0.8);
		}
		else if (gid == SpecialChars::TAB.unicode())
		{
			outline = m_item->doc()->symTab.copy();
			chma4.translate(gc.width() - fontSize() * 0.7, -fontSize() * gc.scaleV() * 0.5);
			if (gc.hasFlag(ScLayout_RightToLeft))
				chma4.scale(-1, 1);
		}
		else if (gid == SpecialChars::COLBREAK.unicode())
		{
			outline = m_item->doc()->symNewCol.copy();
			chma4.translate(gc.xoffset, -fontSize() * gc.scaleV() * 0.6);
		}
		else if (gid == SpecialChars::FRAMEBREAK.unicode())
		{
			outline = m_item->doc()->symNewFrame.copy();
			chma4.translate(gc.xoffset, -fontSize() * gc.scaleV() * 0.6);
		}
		else if (gid == SpecialChars::PARSEP.unicode())
		{
			outline = m_item->doc()->symReturn.copy();
			chma4.translate(gc.xoffset, -fontSize() * gc.scaleV() * 0.8);
			if (gc.hasFlag(ScLayout_RightToLeft))
				chma4.scale(-1, 1);
		}
		else if (gid == SpecialChars::LINEBREAK.unicode())
		{
			outline = m_item->doc()->symNewLine.copy();
			chma4.translate(gc.xoffset, -fontSize() * gc.scaleV() * 0.4);
			if (gc.hasFlag(ScLayout_RightToLeft))
				chma4.scale(-1, 1);
		}
		else if (gid == SpecialChars::NBSPACE.unicode() || gid == 32)
		{
			stroke = (gid == 32);
			outline = m_item->doc()->symNonBreak.copy();
			chma4.translate(gc.xoffset, -fontSize() * gc.scaleV() * 0.4);
		}
		else if (gid == SpecialChars::NBHYPHEN.unicode())
		{
			outline = font().glyphOutline(font().hyphenGlyph(), fontSize());
			chma4.translate(gc.xoffset, -fontSize() * gc.scaleV());
		}
		else if (gid == SpecialChars::SHYPHEN.unicode())
		{
			outline.resize(0);
			outline.addQuadPoint(0, -10, 0, -10, 0, -6, 0, -6);
			stroke = true;
		}
		else if (gid == SpecialChars::OBJECT.unicode())
		{
			//for showing marks entries as control chars
			outline.resize(0);
			outline.addQuadPoint(0, -8, 1, -8, 0, -6, 1, -6);
			stroke = true;
		}
		else // ???
		{
			outline.resize(0);
			outline.addQuadPoint(0, -10, 0, -10, 0, -9, 0, -9);
			outline.addQuadPoint(0, -9, 0, -9, 1, -9, 1, -9);
			outline.addQuadPoint(1, -9, 1, -9, 1, -10, 1, -10);
			outline.addQuadPoint(1, -10, 1, -10, 0, -10, 0, -10);
		}
		chma.scale(gc.scaleH() * fontSize() / 10.0, gc.scaleV() * fontSize() / 10.0);
		outline.map(chma * chma4);
		m_painter->setupPolygon(&outline, true);
		QColor oldBrush = m_painter->brush();
		m_painter->setBrush(gc.hasFlag(ScLayout_SuppressSpace) ? Qt::green
															   : PrefsManager::instance()->appPrefs.displayPrefs.controlCharColor);
		m_painter->setBrush(PrefsManager::instance()->appPrefs.displayPrefs.controlCharColor);
		if (stroke)
		{
			QColor tmp = m_painter->pen();
			m_painter->setStrokeMode(1);
			m_painter->setPen(m_painter->brush(), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			m_painter->setLineWidth(fontSize() * gc.scaleV() / 20.0);
			m_painter->strokePath();
			m_painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		}
		else
		{
			m_painter->setFillMode(1);
			m_painter->fillPath();
		}
		m_painter->setBrush(oldBrush);
	}
	else
	{
		double sizeFactor = fontSize() / 10.0;
		QVector<FPointArray> outlines = gc.glyphClusterOutline();
		const QList<GlyphLayout>& glyphs = gc.glyphs();
		for (int i = 0; i < glyphs.count(); ++i)
		{
			const FPointArray& outline = outlines.at(i);
			const GlyphLayout& gl = glyphs.at(i);
			m_painter->save();
			m_painter->translate(gl.xoffset, - (fontSize() * gl.scaleV) + gl.yoffset);
			m_painter->scale(gl.scaleH * sizeFactor, gl.scaleV * sizeFactor);
			m_painter->setupPolygon(&outline, true);
			if (outline.size() > 3)
				m_painter->fillPath();
			m_painter->restore();
			m_painter->translate(gl.xadvance, 0.0);
		}
	}
	m_painter->setFillRule(fr);

	m_painter->restore();
}

void ScreenPainter::drawGlyphOutline(const GlyphCluster& gc, bool fill)
{
	if (fill)
		drawGlyph(gc);
	m_painter->save();
	bool fr = m_painter->fillRule();
	m_painter->setFillRule(false);

	setupState(false);
	double current_x = 0.0;
	foreach (const GlyphLayout& gl, gc.glyphs()) {
		m_painter->save();
		m_painter->translate(gl.xoffset + current_x, - (fontSize() * gl.scaleV) + gl.yoffset );
		FPointArray outline = font().glyphOutline(gl.glyph);
		double scaleHv = gl.scaleH * fontSize() / 10.0;
		double scaleVv = gl.scaleV * fontSize() / 10.0;
		QTransform trans;
		trans.scale(scaleHv, scaleVv);
		outline.map(trans);
		m_painter->setupPolygon(&outline, true);
		if (outline.size() > 3)
		{
			m_painter->setLineWidth(strokeWidth());
			m_painter->strokePath();
		}
		m_painter->restore();
		current_x += gl.xadvance;
	}

	m_painter->setFillRule(fr);
	m_painter->restore();

}

void ScreenPainter::drawLine(QPointF start, QPointF end)
{
	m_painter->save();
	setupState(false);
	m_painter->drawLine(start, end);
	m_painter->restore();
}

void ScreenPainter::drawRect(QRectF rect)
{
	m_painter->save();
	setupState(true);
	m_painter->drawRect(rect.x(), rect.y(), rect.width(), rect.height());
	m_painter->restore();
}

void ScreenPainter::drawObject(PageItem* embedded)
{
	QRectF cullingArea;
	if (!embedded)
		return;
	if (!m_item->m_Doc->DoDrawing)
		return;

	m_painter->save();
	setupState(false);

	double pws = embedded->m_lineWidth;

	embedded->Dirty = m_item->Dirty;
	embedded->invalid = true;
	embedded->DrawObj_Pre(m_painter);

	switch(embedded->itemType())
	{
	case PageItem::ImageFrame:
	case PageItem::TextFrame:
	case PageItem::LatexFrame:
	case PageItem::OSGFrame:
	case PageItem::Polygon:
	case PageItem::PathText:
	case PageItem::Symbol:
	case PageItem::Group:
	case PageItem::RegularPolygon:
	case PageItem::Arc:
	case PageItem::Table:
		embedded->DrawObj_Item(m_painter, cullingArea);
		break;
	case PageItem::Line:
	case PageItem::PolyLine:
	case PageItem::Spiral:
	//	embedded->m_lineWidth = pws * qMin(scaleH(), scaleV());
		embedded->DrawObj_Item(m_painter, cullingArea);
		break;
	default:
		break;
	}

//	embedded->m_lineWidth = pws * qMin(scaleH(), scaleV());
	embedded->DrawObj_Post(m_painter);
	embedded->m_lineWidth = pws;

	if (m_item->m_Doc->guidesPrefs().framesShown)
	{
		int fm = m_painter->fillMode();
		m_painter->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameNormColor, 0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
		m_painter->setFillMode(ScPainter::None);
		QRectF bBox = embedded->getVisualBoundingRect();
		m_painter->drawSharpRect(0, 0, bBox.width(), bBox.height());
		m_painter->setFillMode(fm);
	}

	m_painter->restore();
}

void ScreenPainter::clip(QRectF rect)
{
	m_painter->newPath();
	m_painter->moveTo(rect.x() + x(), y());
	m_painter->lineTo(rect.x() + x() + rect.width(), y());
	m_painter->lineTo(rect.x() + x() + rect.width(), y() - rect.height());
	m_painter->lineTo(rect.x() + x() , y() - rect.height());
	m_painter->closePath();
	m_painter->setClipPath();
}

void ScreenPainter::saveState()
{
	m_painter->save();
}

void ScreenPainter::restoreState()
{
	m_painter->restore();
}

void ScreenPainter::setupState(bool rect)
{
	if (selected() && rect)
	{
		// we are drawing a selection rect
		QColor color = qApp->palette().color(QPalette::Active, QPalette::Highlight);
		m_painter->setBrush(color);
		m_painter->setPen(color, strokeWidth(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	}
	else if (selected())
	{
		// we are drawing selected text
		QColor color = qApp->palette().color(QPalette::Active, QPalette::HighlightedText);
		m_painter->setBrush(color);
		m_painter->setPen(color, strokeWidth(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	}
	else
	{
		QColor tmp;
		if (m_fillColor != fillColor())
		{
			m_item->SetQColor(&tmp, fillColor().color, fillColor().shade);
			m_fillQColor = tmp;
			m_fillColor = fillColor();
		}
		if (m_strokeColor != strokeColor())
		{
			m_item->SetQColor(&tmp, strokeColor().color, strokeColor().shade);
			m_fillStrokeQColor = tmp;
			m_strokeColor = strokeColor();
		}
		m_painter->setBrush(m_fillQColor);
		m_painter->setPen(m_fillStrokeQColor, strokeWidth(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	}

	m_painter->translate(x(), y());
	if (scaleH() != 1.0 || scaleV() != 1.0)
		m_painter->scale(scaleH(), scaleV());

	if (matrix() != QTransform())
	{
		m_painter->setWorldMatrix(matrix() * m_painter->worldMatrix());
	}
}
