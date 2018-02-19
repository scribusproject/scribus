/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include <math.h>

#include "pageitem.h"
#include "boxes.h"
#include "appmodes.h"
#include "qapplication.h"
#include "pageitem_textframe.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "sccolorengine.h"
#include "colorblind.h"
#include "textlayoutpainter.h"
#include "screenpainter.h"
#include "itextcontext.h"
#include "itextsource.h"

int GroupBox::pointToPosition(QPointF coord, const StoryText &story) const
{
	QPointF rel = coord - QPointF(m_x, m_y);
	foreach (const Box *box, boxes())
	{
		if (box->containsPoint(rel))
		{
			int result = box->pointToPosition(rel, story);
			if (result >= 0)
				return result;
		}
	}

	if (containsPoint(coord) && (m_boxes.count() > 0))
	{
		LineBox* firstLine = dynamic_cast<LineBox*>(m_boxes.first());
		if (firstLine && (coord.y() < firstLine->y()))
			return firstLine->firstChar();

		LineBox* lastLine = dynamic_cast<LineBox*>(m_boxes.last());
		if (lastLine && (coord.y() > lastLine->y() + lastLine->naturalHeight()))
			return lastLine->lastChar() + 1;
	}

	return -1;
}

QLineF GroupBox::positionToPoint(int pos, const StoryText& story) const
{
	QLineF result;
	foreach (const Box *box, boxes())
	{
		if (box->containsPos(pos))
		{
			result = box->positionToPoint(pos, story);
		}
	}
	if (!result.isNull())
		result.translate(x(), y());
	return result;
}

void GroupBox::render(TextLayoutPainter *p) const
{
	p->save();
	p->translate(x(), y());
	foreach (const Box *box, boxes())
	{
		box->render(p);
	}
	p->restore();
}

void GroupBox::render(ScreenPainter *p, ITextContext *ctx) const
{
	p->save();
	p->translate(x(), y());
	foreach (const Box *box, boxes())
	{
		box->render(p, ctx);
	}
	p->restore();
}

void GroupBox::drawSelection(ScreenPainter *p, ITextContext *ctx) const
{
	p->save();
	p->translate(x(), y());
	foreach (const Box *box, boxes())
	{
		box->drawSelection(p, ctx);
	}
	p->restore();
}

double GroupBox::naturalHeight() const
{
	double nH = 0;
	foreach (const Box* box, boxes()) {
		if (m_direction == D_Horizontal)
			nH = qMax(m_naturalHeight, box->naturalHeight());
		else
			nH = ceil(box->y() + box->ascent() - box->naturalDescent());
	}
	return nH;
}

void GroupBox::addBox(const Box* box)
{
	boxes().append(const_cast<Box*>(box));
	update();
}

void GroupBox::removeBox(int i)
{
	if (i < 0 || i > boxes().size())
		return;

	boxes().removeAt(i);
	update();
}

void GroupBox::update()
{
	m_naturalHeight = m_naturalWidth = 0;
	foreach (const Box* box, boxes()) {
		m_firstChar = qMin(m_firstChar, box->firstChar());
		m_lastChar = qMax(m_lastChar, box->lastChar());
		if (m_direction == D_Horizontal)
		{
			m_naturalWidth += box->width();
			m_naturalHeight = qMax(m_naturalHeight, box->naturalHeight());
		}
		else
		{
			m_naturalWidth = qMax(m_naturalWidth, box->naturalWidth());
			m_naturalHeight = ceil(box->y() + box->ascent() - box->naturalDescent());
		}
	}

//	emit boxChanged();
}

#if 0
void GroupBox::justify(const ParagraphStyle& style)
{
	if ((style.alignment() != ParagraphStyle::Justified) && (style.alignment() != ParagraphStyle::Extended))
		return;
	for (int i = 0; i < m_boxes.count(); i++)
	{
		m_boxes[i]->justify(style);
	}
}
#endif

int LineBox::pointToPosition(QPointF coord, const StoryText &story) const
{
	int position = GroupBox::pointToPosition(coord, story);
	if (position < 0)
	{
		if (containsPoint(coord))
		{
			const ParagraphStyle& style = story.paragraphStyle(firstChar());
			if (style.direction() == ParagraphStyle::RTL)
			{
				if (coord.x() < x())
					position = lastChar();
				else
					position = firstChar();
			}
			else
			{
				if (coord.x() < x())
					position = firstChar();
				else
					position = lastChar();
			}
			if (position == story.length() - 1)
			{
				if (m_firstChar == m_lastChar)
					position = m_lastChar;
				else if (!SpecialChars::isBreak(story.text(position)))
					position = story.length();
			}
		}
	}

	return position;
}

QLineF LineBox::positionToPoint(int pos, const StoryText& story) const
{
	QLineF result;
	foreach (const Box *box, boxes())
	{
		if (box->containsPos(pos))
		{
			double xPos = x() + box->positionToPoint(pos, story).x1();
			result = QLineF(xPos, y(), xPos, y() + height());
			break;
		}
	}
	return result;
}

bool LineBox::containsPoint(QPointF coord) const
{
	return QRectF(0, m_y, m_width, height()).contains(coord);
}

void LineBox::render(TextLayoutPainter *p) const
{
	p->translate(x(), y());

	drawBackGround(p);

	p->translate(0, ascent());
	foreach (const Box *box, boxes())
	{
		box->render(p);
	}

	p->translate(-x(), -y() - ascent());
}

void LineBox::render(ScreenPainter *p, ITextContext *ctx) const
{
	p->translate(x(), y());

	drawBackGround(p);
	drawSelection(p, ctx);

	p->translate(0, ascent());
	foreach (const Box *box, boxes())
		box->render(p, ctx);

	p->translate(-x(), -y() - ascent());
}

void LineBox::drawSelection(ScreenPainter *p, ITextContext *ctx) const
{
	foreach (const Box *box, boxes())
		box->drawSelection(p, ctx);
}

void LineBox::drawBackGround(TextLayoutPainter *p) const
{
	QString lastColor;
	double lastShade = 100;
	QRectF lastRect;
	for (int i = 0; i < boxes().count(); i++)
	{
		const GlyphBox* box = dynamic_cast<const GlyphBox*>(boxes()[i]);
		if (!box)
			continue;
		const CharStyle& style = box->style();
		if (style.backColor() != CommonStrings::None)
		{
			QRectF rect(box->x(), 0, box->width(), height());
			if (lastColor.isEmpty() || (lastColor == style.backColor() && lastShade == style.backShade()))
			{
				lastRect |= rect;
			}
			else
			{
				if (!lastRect.isEmpty())
				{
					TextLayoutColor backColor(lastColor, lastShade);
					p->save();
					p->setFillColor(backColor);
					p->setStrokeColor(backColor);
					p->drawRect(lastRect);
					p->restore();
				}
				lastRect = rect;
			}
			lastColor = style.backColor();
			lastShade = style.backShade();
		}
		else
		{
			if (!lastRect.isEmpty())
			{
				TextLayoutColor backColor(lastColor, lastShade);
				p->save();
				p->setFillColor(backColor);
				p->setStrokeColor(backColor);
				p->drawRect(lastRect);
				p->restore();
			}
			lastColor.clear();
			lastShade = 100;
			lastRect = QRectF();
		}
	}
	if (!lastRect.isEmpty())
	{
		TextLayoutColor backColor(lastColor, lastShade);
		p->save();
		p->setFillColor(backColor);
		p->setStrokeColor(backColor);
		p->drawRect(lastRect);
		p->restore();
	}
}

void LineBox::addBox(const Box* box)
{
	m_boxes.append(const_cast<Box*>(box));
	update();
}

void LineBox::removeBox(int i)
{
	if (i < 0 || i > boxes().size())
		return;

	boxes().removeAt(i);
	update();

}

void LineBox::update()
{
	m_naturalWidth = m_naturalAscent = m_naturalDescent = 0;
	for (int i = 0; i < m_boxes.count(); ++i)
	{
		Box* box = m_boxes[i];
		m_firstChar = qMin(m_firstChar, box->firstChar());
		m_lastChar = qMax(m_lastChar, box->lastChar());

		box->moveTo(m_naturalWidth, box->y());
		m_naturalWidth += box->width();

		m_naturalAscent = qMax(m_naturalAscent, box->naturalAsc());
		m_naturalDescent = qMin(m_naturalDescent, box->naturalDescent());
	}

//	emit boxChanged();
}

#if 0
void LineBox::justify(const ParagraphStyle& style)
{
	double glyphNatural = 0;
	double spaceNatural = 0;
	double glyphExtension;
	double spaceExtension;
	int spaceInsertion = 0;
	double imSpace = -1;

	// measure natural widths for glyphs and spaces
	for (int i = 0; i < m_boxes.count(); ++i)
	{
		GlyphBox *box = dynamic_cast<GlyphBox*>(m_boxes.at(i));
		GlyphRun run(box->glyphs);
		if (!run.hasFlag(ScLayout_ExpandingSpace))
		{
			glyphNatural += run.width();
		}
		else if (!run.hasFlag(ScLayout_SuppressSpace))
		{
			spaceNatural += run.width();
			if (imSpace < 0.0 || imSpace > run.width())
				imSpace = run.width();
		}
		if (i != 0 && run.hasFlag(ScLayout_ImplicitSpace))
			//implicitSpace(itemText.text(sof - 1), ch))
		{
			spaceInsertion += 1;
		}
	}

	imSpace /= 2;

	// decision: prio 1: stretch glyph;  prio 2: insert spaces;  prio 3: stretch spaces
	if (width() < spaceNatural + glyphNatural *
			style.minGlyphExtension() && spaceNatural > 0)
	{
		glyphExtension = style.minGlyphExtension() - 1;
		spaceExtension = (width() - glyphNatural * (1 + glyphExtension)) / spaceNatural - 1;
		imSpace = 0;
	}
	else if (width() < spaceNatural + glyphNatural * style.maxGlyphExtension() && glyphNatural > 0)
	{
		spaceExtension = 0;
		glyphExtension = (width() - spaceNatural) / glyphNatural - 1;
		imSpace = 0;
	}
	else
	{
		glyphExtension = style.maxGlyphExtension() - 1;
		if (spaceInsertion) {
			double remaining = width() - glyphNatural * (1 + glyphExtension) - spaceNatural;
			if (imSpace > 0) {
				if (remaining / spaceInsertion < imSpace) {
					imSpace = remaining / spaceInsertion;
					spaceExtension = 0;
				} else {
					spaceExtension = (remaining + spaceNatural) / (spaceNatural + spaceInsertion * imSpace) - 1;
					imSpace *= spaceExtension + 1;
				}
			} else {
				imSpace = remaining / spaceInsertion;
				spaceExtension = 0;
			}
		} else {
			if (spaceNatural > 0)
				spaceExtension = (width() - glyphNatural * (1 + glyphExtension)) / spaceNatural - 1;
			else
				spaceExtension = 0;
		}
	}

	double glyphScale = 1 + glyphExtension;

	/*
		qDebug() << QString("justify: line = %7 natural = %1 + %2 = %3
	(%4); spaces + %5%%; min=%8; glyphs + %6%%; min=%9")
			   .arg(spaceNatural).arg(glyphNatural).arg(spaceNatural+glyphNatural).arg(line.naturalWidth)
			   .arg(spaceExtension).arg(glyphExtension).arg(line.width)
			   .arg(style.minWordTracking()).arg(style.minGlyphExtension());
		*/

	int startItem = 0;
	if (dynamic_cast<GlyphBox*>(m_boxes[startItem])->glyphs.hasFlag(ScLayout_DropCap))
		startItem++;

	// distribute whitespace on spaces and glyphs
	for (int i = startItem; i < m_boxes.count(); ++i)
	{
		GlyphBox *box = dynamic_cast<GlyphBox*>(m_boxes.at(i));
		GlyphRun& run(box->glyphs);
		if (i != 0 && run.hasFlag(ScLayout_ImplicitSpace))
		{
			GlyphBox *lastBox = dynamic_cast<GlyphBox*>(m_boxes.at(i));
			GlyphRun& lastRun(lastBox->glyphs);
			lastRun.glyphs().last().xadvance += imSpace;
		}
		double wide = run.width();
		if (!run.hasFlag(ScLayout_ExpandingSpace))
		{
			for (int j = 0; j < run.glyphs().count(); ++j)
			{
				GlyphLayout& glyph = run.glyphs()[j];
				glyph.xadvance += wide * glyphExtension;
				glyph.xoffset *= glyphScale;
				glyph.scaleH *= glyphScale;
			}
		}
		else if (!run.hasFlag(ScLayout_SuppressSpace))
		{
			GlyphLayout& glyph = run.glyphs().last();
			glyph.xadvance += wide * spaceExtension;
		}
	}
}
#endif


void PathLineBox::update()
{
	foreach (Box* box, boxes()) {
		m_firstChar = qMin(m_firstChar, box->firstChar());
		m_lastChar = qMax(m_lastChar, box->lastChar());
	}

//	emit boxChanged();
}

void PathLineBox::drawBackGround(TextLayoutPainter *p) const
{
	for (int i = 0; i < boxes().count(); i++)
	{
		const GlyphBox* box = dynamic_cast<const GlyphBox*>(boxes()[i]);
		if (!box)
			continue;

		const CharStyle& style = box->style();
		if (style.backColor() != CommonStrings::None)
		{
			QRectF rect(box->x(), -box->ascent(), box->width(), box->height());
			TextLayoutColor backColor(style.backColor(), style.backShade());
			p->save();
			p->setMatrix(box->matrix());
			p->setFillColor(backColor);
			p->setStrokeColor(backColor);
			p->drawRect(rect);
			p->restore();
		}
	}
}


void GlyphBox::render(ScreenPainter *p, ITextContext *ctx) const
{
	const PageItem* item = ctx->getFrame();
	bool s = p->selected();

	int selectionFirst = -1;
	int selectionLast = -1;
	for (int i = firstChar(); i <= lastChar(); i++)
	{
		if (item->itemText.selected(i))
		{
			if (selectionFirst < 0)
				selectionFirst = i;
			selectionLast = i;
		}
	}

	if (((selectionFirst >= 0 && item->isSelected()) ||
	     ((item->nextInChain() != 0 || item->prevInChain() != 0) && selectionFirst >= 0)) &&
		(item->doc()->appMode == modeEdit || item->doc()->appMode == modeEditTable))
	{
		if (selectionFirst == firstChar() && selectionLast == lastChar())
		{
			p->setSelected(true);
			render(p);
		}
		else
		{
			render(p);
			p->saveState();
			p->setSelected(true);
			qreal firstX = positionToPoint(selectionFirst, item->itemText).x1();
			qreal lastX = positionToPoint(selectionLast + 1, item->itemText).x1();
			if (m_glyphRun.hasFlag(ScLayout_RightToLeft))
				p->clip(QRectF(lastX, y(), firstX - lastX, height()));
			else
				p->clip(QRectF(firstX, y(), lastX - firstX, height()));
			render(p);
			p->restoreState();
		}
	}
	else
	{
		render(p);
	}
	p->setSelected(s);
}

void GlyphBox::drawSelection(ScreenPainter *p, ITextContext *ctx) const
{
	const PageItem* item = ctx->getFrame();
	bool s = p->selected();
	double sw = p->strokeWidth();

	//let's deal with ligature such as ffi
	int selectionFirst = -1;
	int selectionLast = -1;
	for (int i = firstChar(); i <= lastChar(); i++)
	{
		if (item->itemText.selected(i))
		{
			if (selectionFirst < 0)
				selectionFirst = i;
			selectionLast = i;
		}
	}

	QRectF rect(x(), 0, width(), height());
	p->setSelected(true);
	p->setStrokeWidth(0);

	if (((selectionFirst >= 0 && item->isSelected()) ||
		 ((item->nextInChain() != 0 || item->prevInChain() != 0) && selectionFirst >= 0)) &&
			(item->doc()->appMode == modeEdit || item->doc()->appMode == modeEditTable))
	{
		if (selectionFirst == firstChar() && selectionLast == lastChar())
			p->drawRect(rect);
		else
		{
			//now in a ligature just draw the selected part only
			qreal firstX = positionToPoint(selectionFirst, item->itemText).x1();
			qreal lastX = positionToPoint(selectionLast + 1, item->itemText).x1();
			if (m_glyphRun.hasFlag(ScLayout_RightToLeft))
				rect = QRectF(lastX, y(), firstX - lastX, height());
			else
				rect = QRectF(firstX, y(), lastX - firstX, height());
			p->drawRect(rect);
		}
	}

	p->setSelected(s);
	p->setStrokeWidth(sw);
}

void GlyphBox::render(TextLayoutPainter *p) const
{
	// This is a very hot method and can be easily called tens of thousands of times per second,
	// so it is always a good idea to profile changes to this code.
	const CharStyle& charStyle = style();
	const ScFace& font = charStyle.font();
	double fontSize = charStyle.fontSize() / 10.0;
	bool hasFillColor = charStyle.fillColor() != CommonStrings::None;
	bool hasStrokeColor = charStyle.strokeColor() != CommonStrings::None;

	p->save();

	p->setFont(font);
	p->setFontSize(fontSize);

	p->translate(x() + m_glyphRun.xoffset, y() + m_glyphRun.yoffset);

	p->setMatrix(m_matrix);

	if (hasFillColor)
		p->setFillColor(TextLayoutColor(charStyle.fillColor(), charStyle.fillShade()));
	if (hasStrokeColor)
		p->setStrokeColor(TextLayoutColor(charStyle.strokeColor(), charStyle.strokeShade()));

	p->save();

	// Do underlining first so you can get typographically correct
	// underlines when drawing a white outline
	if (m_glyphRun.hasFlag(ScLayout_Underlined) && hasStrokeColor)
	{
		double st, lw;
		if ((charStyle.underlineOffset() != -1) || (charStyle.underlineWidth() != -1))
		{
			if (charStyle.underlineOffset() != -1)
				st = (charStyle.underlineOffset() / 1000.0) * font.descent(fontSize);
			else
				st = font.underlinePos(fontSize);
			if (charStyle.underlineWidth() != -1)
				lw = (charStyle.underlineWidth() / 1000.0) * fontSize;
			else
				lw = qMax(font.strokeWidth(fontSize), 1.0);
		}
		else
		{
			st = font.underlinePos(fontSize);
			lw = qMax(font.strokeWidth(fontSize), 1.0);
		}
		if (charStyle.baselineOffset() != 0)
			st += fontSize * (charStyle.baselineOffset() / 1000.0);

		double sw = p->strokeWidth();
		TextLayoutColor sc = p->strokeColor();

		p->setStrokeColor(p->fillColor());
		p->setStrokeWidth(lw);

		double xoffs(0.0), yoffs(0.0);
		if (m_glyphRun.glyphs().count() > 0)
		{
			const GlyphLayout& gl = m_glyphRun.glyphs().first();
			xoffs = gl.xoffset;
			yoffs = gl.yoffset;
		}
		if (charStyle.effects() & ScStyle_Subscript)
			p->drawLine(QPointF(xoffs, yoffs - st), QPointF(xoffs + m_glyphRun.width(), yoffs - st));
		else
			p->drawLine(QPointF(xoffs, -st), QPointF(xoffs + m_glyphRun.width(), -st));

		p->setStrokeWidth(sw);
		p->setStrokeColor(sc);
	}

	if (charStyle.baselineOffset() != 0)
		p->translate(0, -fontSize * (charStyle.baselineOffset() / 1000.0));

	if ((font.isStroked()) && hasStrokeColor && ((charStyle.fontSize() * m_glyphRun.scaleV() * charStyle.outlineWidth() / 10000.0) != 0))
	{
		p->setStrokeColor(p->fillColor());
		p->setStrokeWidth(charStyle.fontSize() * m_glyphRun.scaleV() * charStyle.outlineWidth() / 10000.0);
		p->drawGlyphOutline(m_glyphRun, false);
	}
	else
	{
		if ((m_effects & ScStyle_Shadowed) && hasStrokeColor)
		{
			double xoff = (charStyle.fontSize() * m_glyphRun.scaleH() * charStyle.shadowXOffset() / 10000.0);
			double yoff = (charStyle.fontSize() * m_glyphRun.scaleV() * charStyle.shadowYOffset() / 10000.0);

			bool s = p->selected();
			TextLayoutColor fc = p->fillColor();

			p->translate(xoff, -yoff);

			p->setFillColor(p->strokeColor());
			p->setSelected(false);
			p->drawGlyph(m_glyphRun);

			p->translate(-xoff, yoff);
			p->setSelected(s);
			p->setFillColor(fc);
		}

		if ((charStyle.effects() & ScStyle_Outline) && hasStrokeColor && ((charStyle.fontSize() * m_glyphRun.scaleV() * charStyle.outlineWidth() / 10000.0) != 0))
		{
			p->setStrokeWidth((charStyle.fontSize() * m_glyphRun.scaleV() * charStyle.outlineWidth() / 10000.0));
			p->drawGlyphOutline(m_glyphRun, hasFillColor);
		}
		else if (hasFillColor)
			p->drawGlyph(m_glyphRun);
	}

	if (charStyle.baselineOffset() != 0)
		p->translate(0, fontSize * (charStyle.baselineOffset() / 1000.0));

	if (m_effects & ScStyle_Strikethrough && hasStrokeColor)
	{
		double st, lw;
		if ((charStyle.strikethruOffset() != -1) || (charStyle.strikethruWidth() != -1))
		{
			if (charStyle.strikethruOffset() != -1)
				st = (charStyle.strikethruOffset() / 1000.0) * font.ascent(fontSize);
			else
				st = font.strikeoutPos(fontSize) * m_glyphRun.scaleV();
			if (charStyle.strikethruWidth() != -1)
				lw = (charStyle.strikethruWidth() / 1000.0) * fontSize;
			else
				lw = qMax(font.strokeWidth(fontSize), 1.0);
		}
		else
		{
			st = font.strikeoutPos(fontSize) * m_glyphRun.scaleV();
			lw = qMax(font.strokeWidth(fontSize), 1.0);
		}
		if (charStyle.baselineOffset() != 0)
			st += fontSize * (charStyle.baselineOffset() / 1000.0);

		double sw = p->strokeWidth();
		TextLayoutColor sc = p->strokeColor();

		p->setStrokeColor(p->fillColor());
		p->setStrokeWidth(lw);

		double xoffs(0.0), yoffs(0.0);
		if (m_glyphRun.glyphs().count() > 0)
		{
			const GlyphLayout& gl = m_glyphRun.glyphs().first();
			xoffs = gl.xoffset;
			yoffs = gl.yoffset;
		}
		p->drawLine(QPointF(xoffs, yoffs - st), QPointF(xoffs + m_glyphRun.width(), yoffs - st));

		p->setStrokeWidth(sw);
		p->setStrokeColor(sc);
	}

	p->restore();
	p->translate(m_glyphRun.width(), 0);
	p->restore();
}

int GlyphBox::pointToPosition(QPointF coord, const StoryText& story) const
{
	if (firstChar() != lastChar())
	{
		BreakIterator *it = StoryText::getGraphemeIterator();
		QString text(story.text(firstChar(), lastChar() - firstChar() + 1));
		it->setText((const UChar*) text.utf16());
		int count = 0;
		while (it->next() != BreakIterator::DONE)
			count++;
		if (count == 0)
			qFatal("GlyphBox::pointToPosition: divide by zero error");

		bool rtlLayout = m_glyphRun.hasFlag(ScLayout_RightToLeft);
		double componentWidth = width() / count;
		for (int i = 0; i < count; i++)
		{
			double componentX;
			if (rtlLayout)
				componentX = x() + width() - (componentWidth * (i + 1));
			else
				componentX = x() + (componentWidth * i);

			if ((coord.x() >= componentX && coord.x() <= componentX + componentWidth))
			{
				if (coord.x() <= componentX + componentWidth / 2.0)
					return rtlLayout ? (firstChar() + i + 1) : (firstChar() + i);
				return rtlLayout ? (firstChar() + i) : (firstChar() + i + 1);
			}
		}
	}
	else
	{
		if (coord.x() >= x() && coord.x() <= x() + width())
		{
			bool rtlLayout = m_glyphRun.hasFlag(ScLayout_RightToLeft);
			if (coord.x() <= x() + width() / 2.0)
				return rtlLayout ? (firstChar() + 1) : firstChar();
			return rtlLayout ? firstChar() : (firstChar() + 1);
		}
	}

	return -1;
}

QLineF GlyphBox::positionToPoint(int pos, const StoryText& story) const
{
	double xPos = 0.0;
	if (firstChar() != lastChar())
	{
		int index = 0;
		BreakIterator *it = StoryText::getGraphemeIterator();
		QString text(story.text(firstChar(), lastChar() - firstChar() + 1));
		it->setText((const UChar*) text.utf16());
		int count = 0;
		while (it->next() != BreakIterator::DONE)
		{
			count++;
			if (pos - firstChar() == it->current())
				index = count;
		}
		if (count == 0)
			qFatal("GlyphBox::positionToPoint: divide by zero error");

		double componentWidth = width() / count;
		if (m_glyphRun.hasFlag(ScLayout_RightToLeft))
			xPos = x() + width() - (componentWidth * index);
		else
			xPos = x() + (componentWidth * index);
	}
	else
	{
		if (m_glyphRun.hasFlag(ScLayout_RightToLeft))
			xPos = x() + width();
		else
			xPos = x();
	}

	return QLineF(xPos, y(), xPos, y() + height());
}

void ObjectBox::render(TextLayoutPainter *p) const
{
	p->save();
	double oldX = m_object->xPos();
	double oldY = m_object->yPos();
	bool oldEM = m_object->isEmbedded;
	//m_item->isEmbedded = false; // #14311: fix discrepancy of display between 1.5.1 and 1.5.2+
	const CharStyle& charStyle = style();
	p->translate(x(), y() - ascent());
	if (m_glyphRun.hasFlag(ScLayout_DropCap))
		p->setScale(m_glyphRun.scaleH(), m_glyphRun.scaleV());
	else
		p->setScale(charStyle.scaleH() / 1000.0, charStyle.scaleV() / 1000.0);
	p->setMatrix(m_matrix);

	m_object->setXPos(m_object->gXpos, true);
	m_object->setYPos(m_object->gYpos, true);
	//	m_item->setYPos((m_object->gHeight * (charStyle.scaleV() / 1000.0)) + m_object->gYpos, true);

	if (charStyle.baselineOffset() != 0)
	{
		p->translate(0, -m_object->gHeight * (charStyle.baselineOffset() / 1000.0));
		m_object->setYPos(m_object->yPos() - m_object->gHeight * (charStyle.baselineOffset() / 1000.0));
	}

	p->drawObject(m_object);

	m_object->setXPos(oldX, true);
	m_object->setYPos(oldY, true);
	m_object->isEmbedded = oldEM;
	p->restore();
}

void ObjectBox::render(ScreenPainter *p, ITextContext *ctx) const
{
	render(p);
}
