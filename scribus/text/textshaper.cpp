/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "pageitem.h"
#include "pageitem_textframe.h"
#include "scribusdoc.h"
#include "sctextstruct.h"
#include "style.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "textshaper.h"
#include "text/specialchars.h"

TextShaper::TextShaper(PageItem_TextFrame* textItem, int startIndex)
	      : m_startIndex(startIndex),
			m_index(startIndex),
			m_lastKernedIndex(-1),
			m_layoutFlags(ScLayout_None),
			m_item(textItem)
{
	if (m_item->lastInFrame() >= m_item->firstInFrame())
	{
		int charsCount = m_item->lastInFrame() - m_item->firstInFrame() + 1;
		m_runs.reserve(charsCount);
	}
}

bool TextShaper::hasRun(int i)
{
	if (i <= m_lastKernedIndex)
		return true;

	needChars(i);
	return (i <= m_lastKernedIndex);
}

GlyphRun TextShaper::runAt(int i)
{
	// Ensure we get a kerned run 
	if (i <= m_lastKernedIndex)
		return m_runs.at(i);
	
	needChars(i);
	return m_runs.at(i);
}

void TextShaper::needChars(int runIndex)
{
	StoryText& itemText = m_item->itemText;

	for ( ; m_index < itemText.length(); ++m_index)
	{
		Mark* mark = itemText.mark(m_index);
		if ((mark != NULL) && (itemText.hasMark(m_index)))
		{
			mark->OwnPage = m_item->OwnPage;
			//itemPtr and itemName set to this frame only if mark type is different than MARK2ItemType
			if (!mark->isType(MARK2ItemType))
			{
				mark->setItemPtr(m_item);
				mark->setItemName(m_item->itemName());
			}

			//anchors and indexes has no visible inserts in text
			if (mark->isType(MARKAnchorType) || mark->isType(MARKIndexType))
				continue;

			//set note marker charstyle
			if (mark->isNoteType())
			{
				TextNote* note = mark->getNotePtr();
				if (note == NULL)
					continue;
				mark->setItemPtr(m_item);
				NotesStyle* nStyle = note->notesStyle();
				Q_ASSERT(nStyle != NULL);
				QString chsName = nStyle->marksChStyle();
				CharStyle currStyle(itemText.charStyle(m_index));
				if (!chsName.isEmpty())
				{
					CharStyle marksStyle(m_item->doc()->charStyle(chsName));
					if (!currStyle.equiv(marksStyle))
					{
						currStyle.setParent(chsName);
						itemText.applyCharStyle(m_index, 1, currStyle);
					}
				}

				StyleFlag s(itemText.charStyle(m_index).effects());
				if (mark->isType(MARKNoteMasterType))
				{
					if (nStyle->isSuperscriptInMaster())
						s |= ScStyle_Superscript;
					else
						s &= ~ScStyle_Superscript;
				}
				else
				{
					if (nStyle->isSuperscriptInNote())
						s |= ScStyle_Superscript;
					else
						s &= ~ScStyle_Superscript;
				}
				if (s != itemText.charStyle(m_index).effects())
				{
					CharStyle haveSuperscript;
					haveSuperscript.setFeatures(s.featureList());
					itemText.applyCharStyle(m_index, 1, haveSuperscript);
				}
			}
		}

		bool bullet = false;
		if (m_index == 0 || itemText.text(m_index - 1) == SpecialChars::PARSEP)
		{
			ParagraphStyle style = itemText.paragraphStyle(m_index);
			if (style.hasBullet() || style.hasNum())
			{
				bullet = true;
				if (mark == NULL || !mark->isType(MARKBullNumType))
				{
					itemText.insertMark(new BulNumMark(), m_index);
					m_index--;
					continue;
				}
				if (style.hasBullet())
					mark->setString(style.bulletStr());
				else if (style.hasNum() && mark->getString().isEmpty())
				{
					mark->setString("?");
					m_item->doc()->flag_Renumber = true;
				}
			}
		}

		if (!bullet && mark && mark->isType(MARKBullNumType))
		{
			itemText.removeChars(m_index, 1);
			m_index--;
			continue;
		}

		QString str = m_item->ExpandToken(m_index);
		if (str.isEmpty())
			str = SpecialChars::ZWNBSPACE;

		for (int j = 0; j < str.length(); j++)
		{
			const QChar ch(str.at(j));
			GlyphRun run(&itemText.charStyle(m_index), itemText.flags(m_index), m_index, m_index, itemText.object(m_index));
			initGlyphLayout(run, QString(ch), m_runs.count());
			m_runs.append(run);
		}

		// This ensure we have a sufficient number of runs
		// to kern the request run
		if (m_runs.count() >= runIndex + 2)
		{
			++m_index;
			break;
		}
	}

	// Last run cannot be kerned, increment m_lastKernedIndex to its max value
	if (m_index >= itemText.length())
		m_lastKernedIndex = m_runs.count() - 1;
}

void TextShaper::startLine(int i)
{
	m_layoutFlags = ScLayout_StartOfLine;

	while (m_runs.count() > i)
	{
		m_index = m_runs.last().firstChar();
		m_runs.removeLast();
	}

	m_lastKernedIndex = m_runs.count() - 1;
}

void TextShaper::initGlyphLayout(GlyphRun& run, const QString& chars, int runIndex)
{
	int a = run.firstChar();
	const QChar ch = chars.at(0);
	const CharStyle& runStyle = run.style();
	StoryText& itemText = m_item->itemText;

	if (SpecialChars::isExpandingSpace(ch))
		run.setFlag(ScLayout_ExpandingSpace);

	LayoutFlags layoutFlags = static_cast<LayoutFlags>(itemText.flags(a) | m_layoutFlags);
	GlyphLayout gl = m_item->layoutGlyphs(runStyle, chars, layoutFlags);
	m_layoutFlags  = static_cast<LayoutFlags>(m_layoutFlags & (~ScLayout_StartOfLine));

	if (runIndex > 0)
	{
		GlyphLayout& last = m_runs[runIndex - 1].glyphs().last();
		last.xadvance += runStyle.font().glyphKerning(last.glyph, gl.glyph, runStyle.fontSize() / 10);
		m_lastKernedIndex = qMax(m_lastKernedIndex, runIndex - 1);
	}

	//show control characters for marks
	if (itemText.hasMark(a))
	{
		GlyphLayout control;
		control.glyph = SpecialChars::OBJECT.unicode() + ScFace::CONTROL_GLYPHS;
		run.glyphs().append(control);
	}

	if (SpecialChars::isExpandingSpace(ch))
		gl.xadvance *= runStyle.wordTracking();

	if (itemText.hasObject(a))
		gl.xadvance = itemText.object(a)->getVisualBoundingRect().width();

	run.glyphs().append(gl);
}
