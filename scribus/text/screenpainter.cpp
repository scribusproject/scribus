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

void ScreenPainter::drawGlyph(const GlyphLayout gl)
{
	bool showControls = (m_item->doc()->guidesPrefs().showControls) &&
			    (gl.glyph == font().char2CMap(QChar(' ')) || gl.glyph >= ScFace::CONTROL_GLYPHS);
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

		cairo_scale(cr, gl.scaleH, gl.scaleV);
		cairo_glyph_t glyph = { gl.glyph, 0, 0 };
		cairo_show_glyphs(cr, &glyph, 1);

		m_painter->restore();
		return;
	}
#endif
	m_painter->save();

	setupState(false);

	bool fr = m_painter->fillRule();
	m_painter->setFillRule(false);

	uint gid = gl.glyph;
	if (showControls)
	{
		bool stroke = false;
		if (gid >= ScFace::CONTROL_GLYPHS)
			gid -= ScFace::CONTROL_GLYPHS;
		else
			gid = 32;
		QTransform chma, chma4;
		FPointArray outline;
		if (gid == SpecialChars::TAB.unicode())
		{
			outline = m_item->doc()->symTab.copy();
			chma4.translate(gl.xadvance - fontSize() * gl.scaleH * 0.7, -fontSize() * gl.scaleV * 0.5);
		}
		else if (gid == SpecialChars::COLBREAK.unicode())
		{
			outline = m_item->doc()->symNewCol.copy();
			chma4.translate(0, -fontSize() * gl.scaleV * 0.6);
		}
		else if (gid == SpecialChars::FRAMEBREAK.unicode())
		{
			outline = m_item->doc()->symNewFrame.copy();
			chma4.translate(0, -fontSize() * gl.scaleV * 0.6);
		}
		else if (gid == SpecialChars::PARSEP.unicode())
		{
			outline = m_item->doc()->symReturn.copy();
			chma4.translate(0, -fontSize() * gl.scaleV * 0.8);
		}
		else if (gid == SpecialChars::LINEBREAK.unicode())
		{
			outline = m_item->doc()->symNewLine.copy();
			chma4.translate(0, -fontSize() * gl.scaleV * 0.4);
		}
		else if (gid == SpecialChars::NBSPACE.unicode() || gid == 32)
		{
			stroke = (gid == 32);
			outline = m_item->doc()->symNonBreak.copy();
			chma4.translate(0, -fontSize() * gl.scaleV * 0.4);
		}
		else if (gid == SpecialChars::NBHYPHEN.unicode())
		{
			outline = font().glyphOutline(font().char2CMap(QChar('-')), fontSize());
			chma4.translate(0, -fontSize() * gl.scaleV);
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
		chma.scale(gl.scaleH * fontSize() / 10.0, gl.scaleV * fontSize() / 10.0);
		outline.map(chma * chma4);
		m_painter->setupPolygon(&outline, true);
		QColor oldBrush = m_painter->brush();
		// FIXME
		/* p->setBrush( (flags & ScLayout_SuppressSpace) ? Qt::green
				: PrefsManager::instance()->appPrefs.displayPrefs.controlCharColor);*/
		m_painter->setBrush(PrefsManager::instance()->appPrefs.displayPrefs.controlCharColor);
		if (stroke)
		{
			QColor tmp = m_painter->pen();
			m_painter->setStrokeMode(1);
			m_painter->setPen(m_painter->brush(), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			m_painter->setLineWidth(fontSize() * gl.scaleV / 20.0);
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
		m_painter->translate(0, -(fontSize() * gl.scaleV));
		double scaleH = gl.scaleH * fontSize() / 10.0;
		double scaleV = gl.scaleV * fontSize() / 10.0;
		m_painter->scale(scaleH, scaleV);
		FPointArray outline = font().glyphOutline(gid);
		m_painter->setupPolygon(&outline, true);
		if (outline.size() > 3)
			m_painter->fillPath();
	}
	m_painter->setFillRule(fr);

	m_painter->restore();
}

void ScreenPainter::drawGlyphOutline(const GlyphLayout gl, bool fill)
{
	if (fill)
		drawGlyph(gl);
	m_painter->save();
	bool fr = m_painter->fillRule();
	m_painter->setFillRule(false);

	setupState(false);
	m_painter->translate(0, -(fontSize() * gl.scaleV));

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
