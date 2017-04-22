#include "textshaper.h"

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb-icu.h>
#include <unicode/ubidi.h>

#include "scrptrun.h"

#include "glyphcluster.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "storytext.h"
#include "styles/paragraphstyle.h"
#include "util.h"


TextShaper::TextShaper(ITextContext* context, ITextSource &story, int firstChar, bool singlePar)
	: m_context(context),
	m_contextNeeded(false),
	m_story(story),
	m_firstChar(firstChar),
	m_singlePar(singlePar)
{ }

TextShaper::TextShaper(ITextSource &story, int firstChar)
	: m_context(NULL),
	m_contextNeeded(false),
	m_story(story),
	m_firstChar(firstChar),
	m_singlePar(false)
{
	for (int i = m_firstChar; i < m_story.length(); ++i)
	{
		QChar ch = m_story.text(i);
		if (ch == SpecialChars::PARSEP || ch == SpecialChars::LINEBREAK)
			continue;
		QString str(ch);
		m_textMap.insert(i, i);
		m_text.append(str);
	}
}

QList<TextShaper::TextRun> TextShaper::itemizeBiDi()
{
	QList<TextRun> textRuns;
	UBiDi *obj = ubidi_open();
	UErrorCode err = U_ZERO_ERROR;

	UBiDiLevel parLevel = UBIDI_LTR;
	ParagraphStyle style = m_story.paragraphStyle(m_firstChar);
	if (style.direction() == ParagraphStyle::RTL)
		parLevel = UBIDI_RTL;

	ubidi_setPara(obj, (const UChar*) m_text.utf16(), m_text.length(), parLevel, NULL, &err);
	if (U_SUCCESS(err))
	{
		int32_t count = ubidi_countRuns(obj, &err);
		if (U_SUCCESS(err))
		{
			textRuns.reserve(count);
			for (int32_t i = 0; i < count; i++)
			{
				int32_t start, length;
				UBiDiDirection dir = ubidi_getVisualRun(obj, i, &start, &length);
				textRuns.append(TextRun(start, length, dir));
			}
		}
	}

	ubidi_close(obj);
	return textRuns;
}

QList<TextShaper::TextRun> TextShaper::itemizeScripts(const QList<TextRun> &runs)
{
	QList<TextRun> newRuns;
	ScriptRun scriptrun((const UChar*) m_text.utf16(), m_text.length());

	foreach (TextRun run, runs)
	{
		int start = run.start;
		QList<TextRun> subRuns;

		while (scriptrun.next())
		{
			if (scriptrun.getScriptStart() <= start && scriptrun.getScriptEnd() > start)
				break;
		}

		while (start < run.start + run.len)
		{
			int end = qMin(scriptrun.getScriptEnd(), run.start + run.len);
			UScriptCode script = scriptrun.getScriptCode();
			if (run.dir == UBIDI_RTL)
				subRuns.prepend(TextRun(start, end - start, run.dir, script));
			else
				subRuns.append(TextRun(start, end - start, run.dir, script));

			start = end;
			scriptrun.next();
		}

		scriptrun.reset();
		newRuns.append(subRuns);
	}

	return newRuns;
}

QList<TextShaper::FeaturesRun> TextShaper::itemizeFeatures(const TextRun &run)
{
	QList<FeaturesRun> newRuns;
	QList<FeaturesRun> subfeature;
	int start = run.start;

	while (start < run.start + run.len)
	{
		int end = start;
		QStringList startFeatures = m_story.charStyle(m_textMap.value(start)).fontFeatures().split(",");
		while (end < run.start + run.len)
		{
			QStringList endFeatures = m_story.charStyle(m_textMap.value(end)).fontFeatures().split(",");
			if (startFeatures != endFeatures)
				break;
			end++;
		}
		subfeature.append(FeaturesRun(start, end - start, startFeatures));
		start = end;
		startFeatures.clear();
	}
	newRuns.append(subfeature);
	return newRuns;
}

QList<TextShaper::TextRun> TextShaper::itemizeStyles(const QList<TextRun> &runs)
{
	QList<TextRun> newRuns;

	foreach (TextRun run, runs) {
		int start = run.start;
		QList<TextRun> subRuns;

		while (start < run.start + run.len)
		{
			int end = start;
			const CharStyle &startStyle = m_story.charStyle(m_textMap.value(start));
			while (end < run.start + run.len)
			{
				const CharStyle &endStyle = m_story.charStyle(m_textMap.value(end));
				if (!startStyle.equivForShaping(endStyle))
					break;
				end++;
			}
			if (run.dir == UBIDI_RTL)
				subRuns.prepend(TextRun(start, end - start, run.dir, run.script));
			else
				subRuns.append(TextRun(start, end - start, run.dir, run.script));
			start = end;
		}

		newRuns.append(subRuns);
	}

	return newRuns;
}

void TextShaper::buildText(int fromPos, int toPos, QVector<int>& smallCaps)
{
	m_text = "";
	
	if (toPos > m_story.length() || toPos < 0)
		toPos = m_story.length();
	
	for (int i = fromPos; i < toPos; ++i)
	{
		QString str = m_story.text(i,1);
		
		if (m_singlePar)
		{
			QChar ch = str[0];
			if (ch == SpecialChars::PARSEP || ch == SpecialChars::LINEBREAK)
				continue;
		}

		if (m_story.hasExpansionPoint(i))
		{
			m_contextNeeded = true;
			if (m_context != NULL)
			{
				str = m_context->expand(m_story.expansionPoint(i));
				if (str.isEmpty())
					str = SpecialChars::ZWNBSPACE;
			}
			else
			{
				str = SpecialChars::OBJECT;
			}
		}
		
		str.replace(SpecialChars::SHYPHEN, SpecialChars::ZWNJ);

		const CharStyle &style = m_story.charStyle(i);
		int effects = style.effects() & ScStyle_UserStyles;
		bool hasSmallCap = false;
		if ((effects & ScStyle_AllCaps) || (effects & ScStyle_SmallCaps))
		{
			QLocale locale(style.language());
			QString upper = locale.toUpper(str);
			if (upper != str)
			{
				if (effects & ScStyle_SmallCaps)
					hasSmallCap = true;
				str = upper;
			}
		}

		for (int j = 0; j < str.length(); j++)
		{
			m_textMap.insert(m_text.length() + j, i);
			if (hasSmallCap)
				smallCaps.append(m_text.length() + j);
		}

		m_text.append(str);
	}
}


ShapedText TextShaper::shape(int fromPos, int toPos)
{
	m_contextNeeded = false;
	
	ShapedText result(ShapedText(&m_story, fromPos, toPos, m_context));
	

	QVector<int> smallCaps;

	buildText(fromPos, toPos, smallCaps);

	QList<TextRun> bidiRuns = itemizeBiDi();
	QList<TextRun> scriptRuns = itemizeScripts(bidiRuns);
	QList<TextRun> textRuns = itemizeStyles(scriptRuns);

	QVector<int32_t> lineBreaks;
	BreakIterator* lineIt = StoryText::getLineIterator();
	// FIXME-HOST: add some fallback code if the iterator failed
	if (lineIt)
	{
		lineIt->setText((const UChar*) m_text.utf16());
		for (int32_t pos = lineIt->first(); pos != BreakIterator::DONE; pos = lineIt->next())
			lineBreaks.append(pos);
	}

	QVector<int32_t> justificationTracking;

	// Insert implicit spaces in justification between characters
	// in scripts that do not use spaces to seperate words
	foreach (const TextRun& run, scriptRuns) {
		switch (run.script) {
		// clustered scripts from https://drafts.csswg.org/css-text-3/#script-groups
		case USCRIPT_KHMER:
		case USCRIPT_LAO:
		case USCRIPT_MYANMAR:
		case USCRIPT_NEW_TAI_LUE:
		case USCRIPT_TAI_LE:
		case USCRIPT_TAI_VIET:
		case USCRIPT_THAI:
		{
			BreakIterator* charIt = StoryText::getGraphemeIterator();
			if (charIt)
			{
				const QString text = m_text.mid(run.start, run.len);
				charIt->setText((const UChar*) text.utf16());
				int32_t pos = charIt->first();
				while (pos != BreakIterator::DONE && pos < text.length())
				{
					UErrorCode status = U_ZERO_ERROR;
					UScriptCode sc = uscript_getScript(text.at(pos).unicode(), &status);
					// do not insert implicit space before punctuation
					// or other non-script specific characters
					if (sc != USCRIPT_COMMON)
						justificationTracking.append(run.start + pos - 1);
					pos = charIt->next();
				}
			}
			break;
		}
		default:
			break;
		}
	}

	foreach (const TextRun& textRun, textRuns) {
		const CharStyle &style = m_story.charStyle(m_textMap.value(textRun.start));

		const ScFace &scFace = style.font();
		hb_font_t *hbFont = reinterpret_cast<hb_font_t*>(scFace.hbFont());
		if (hbFont == NULL)
			continue;

		hb_font_set_scale(hbFont, style.fontSize(), style.fontSize());
		FT_Face ftFace = hb_ft_font_get_face(hbFont);
		if (ftFace)
			FT_Set_Char_Size(ftFace, style.fontSize(), 0, 72, 0);

		hb_direction_t hbDirection = (textRun.dir == UBIDI_LTR) ? HB_DIRECTION_LTR : HB_DIRECTION_RTL;
		hb_script_t hbScript = hb_icu_script_to_script(textRun.script);
		std::string language = style.language().toStdString();
		hb_language_t hbLanguage = hb_language_from_string(language.c_str(), language.length());

		hb_buffer_t *hbBuffer = hb_buffer_create();
		hb_buffer_add_utf16(hbBuffer, m_text.utf16(), m_text.length(), textRun.start, textRun.len);
		hb_buffer_set_direction(hbBuffer, hbDirection);
		hb_buffer_set_script(hbBuffer, hbScript);
		hb_buffer_set_language(hbBuffer, hbLanguage);
		hb_buffer_set_cluster_level(hbBuffer, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);

		QVector<hb_feature_t> hbFeatures;
		QList<FeaturesRun> featuresRuns = itemizeFeatures(textRun);
		foreach (const FeaturesRun& featuresRun, featuresRuns)
		{
			const QStringList& features = featuresRun.features;
			hbFeatures.reserve(features.length());
			foreach (const QString& feature, features) {
				hb_feature_t hbFeature;
				hb_bool_t ok = hb_feature_from_string(feature.toStdString().c_str(), feature.toStdString().length(), &hbFeature);
				if (ok)
				{
					hbFeature.start = featuresRun.start;
					hbFeature.end = featuresRun.len + featuresRun.start;
					hbFeatures.append(hbFeature);
				}
			}
		}

		// #14523: harfbuzz proritize graphite for graphite enabled fonts, however
		// at the point, shaping with graphite fonts is either buggy (harfbuzz 1.4.2)
		// or trigger weird results (harfbuzz 1.4.3), so disable graphite for now.
		// Prevent also use of platform specific shapers for cross-platform reasons
		const char* shapers[] = { "ot", "fallback", 0 };
		hb_shape_full(hbFont, hbBuffer, hbFeatures.data(), hbFeatures.length(), shapers);

		unsigned int count = hb_buffer_get_length(hbBuffer);
		hb_glyph_info_t *glyphs = hb_buffer_get_glyph_infos(hbBuffer, NULL);
		hb_glyph_position_t *positions = hb_buffer_get_glyph_positions(hbBuffer, NULL);

		result.glyphs().reserve(result.glyphs().size() + count);
		for (size_t i = 0; i < count; )
		{
			uint32_t firstCluster = glyphs[i].cluster;
			uint32_t nextCluster = firstCluster;
			if (hbDirection == HB_DIRECTION_LTR)
			{
				size_t j = i + 1;
				while (j < count && nextCluster == firstCluster)
				{
					nextCluster = glyphs[j].cluster;
					j++;
				}
				if (j == count && nextCluster == firstCluster)
					nextCluster = textRun.start + textRun.len;
			}
			else
			{
				int j = i - 1;
				while (j >= 0 && nextCluster == firstCluster)
				{
					nextCluster = glyphs[j].cluster;
					j--;
				}
				if (j <= 0 && nextCluster == firstCluster)
					nextCluster = textRun.start + textRun.len;
			}

			assert(m_textMap.contains(firstCluster));
			assert(m_textMap.contains(nextCluster - 1));
			int firstChar = m_textMap.value(firstCluster);
			int lastChar = m_textMap.value(nextCluster - 1);
			
			QChar ch = m_story.text(firstChar);
			LayoutFlags flags = m_story.flags(firstChar);
			const CharStyle& charStyle(m_story.charStyle(firstChar));
			const StyleFlag& effects = charStyle.effects();

			QString str = m_text.mid(firstChar, lastChar-firstChar+1);
			GlyphCluster run(&charStyle, flags, firstChar, lastChar, m_story.object(firstChar), result.glyphs().length(), str);

			run.clearFlag(ScLayout_HyphenationPossible);
			if (m_story.hasFlag(lastChar, ScLayout_HyphenationPossible))
				run.setFlag(ScLayout_HyphenationPossible);
			
			if (textRun.dir == UBIDI_RTL)
				run.setFlag(ScLayout_RightToLeft);

			if (lineBreaks.contains(firstCluster))
				run.setFlag(ScLayout_LineBoundary);

			if (SpecialChars::isExpandingSpace(ch))
				run.setFlag(ScLayout_ExpandingSpace);
			else if (justificationTracking.contains(firstCluster))
				run.setFlag(ScLayout_JustificationTracking);

			if (effects & ScStyle_Underline)
				run.setFlag(ScLayout_Underlined);
			if (effects & ScStyle_UnderlineWords && !ch.isSpace())
				run.setFlag(ScLayout_Underlined);

			if (firstChar != 0 &&
			    SpecialChars::isCJK(m_story.text(firstChar).unicode()) &&
			    SpecialChars::isCJK(m_story.text(firstChar - 1).unicode()))
				run.setFlag(ScLayout_ImplicitSpace);

			run.setScaleH(charStyle.scaleH() / 1000.0);
			run.setScaleV(charStyle.scaleV() / 1000.0);

			while (i < count && glyphs[i].cluster == firstCluster)
			{
				GlyphLayout gl;
				gl.glyph = glyphs[i].codepoint;
				if (gl.glyph == 0 ||
				    (ch == SpecialChars::LINEBREAK || ch == SpecialChars::PARSEP ||
				     ch == SpecialChars::FRAMEBREAK || ch == SpecialChars::COLBREAK))
				{
					gl.glyph = scFace.emulateGlyph(ch.unicode());
				}

				if (gl.glyph < ScFace::CONTROL_GLYPHS)
				{
					gl.xoffset = positions[i].x_offset / 10.0;
					gl.yoffset = -positions[i].y_offset / 10.0;
					gl.xadvance = positions[i].x_advance / 10.0;
					gl.yadvance = positions[i].y_advance / 10.0;
				}

#if 0
				if (m_story.hasMark(firstChar))
				{
					GlyphLayout control;
					control.glyph = SpecialChars::OBJECT.unicode() + ScFace::CONTROL_GLYPHS;
					run.append(control);
				}
#endif
				
				if (SpecialChars::isExpandingSpace(ch))
					gl.xadvance *= run.style().wordTracking();

				if (m_story.hasObject(firstChar))
				{
					m_contextNeeded = true;
					if (m_context != NULL)
						gl.xadvance = m_context->getVisualBoundingBox(m_story.object(firstChar)).width();
				}

				if ((effects & ScStyle_Superscript) || (effects & ScStyle_Subscript))
				{
					m_contextNeeded = true;
					if (m_context != NULL)
					{
						double scale;
						double asce = style.font().ascent(style.fontSize() / 10.0);
						if (effects & ScStyle_Superscript)
						{
							gl.yoffset -= asce * m_context->typographicPrefs().valueSuperScript / 100.0;
							scale = qMax(m_context->typographicPrefs().scalingSuperScript / 100.0, 10.0 / style.fontSize());
						}
						else // effects & ScStyle_Subscript
						{
							gl.yoffset += asce * m_context->typographicPrefs().valueSubScript / 100.0;
							scale = qMax(m_context->typographicPrefs().scalingSubScript / 100.0, 10.0 / style.fontSize());
						}
						
						run.setScaleH(run.scaleH() * scale);
						run.setScaleV(run.scaleV() * scale);
					}
				}

				if (smallCaps.contains(firstCluster))
				{
					m_contextNeeded = true;
					if (m_context != NULL)
					{
						
						double smallcapsScale = m_context->typographicPrefs().valueSmallCaps / 100.0;
						run.setScaleH(run.scaleH() * smallcapsScale);
						run.setScaleV(run.scaleV() * smallcapsScale);
					}
				}

				if (run.scaleH() == 0.0)
				{
					gl.xadvance = 0.0;
					run.setScaleH(1.0);
				}

				run.append(gl);
				i++;
			}

			// Apply CJK spacing according to JIS X4051
			if (lastChar + 1 < m_story.length())
			{
				double halfEM = run.style().fontSize() / 10 / 2;
				double quarterEM = run.style().fontSize() / 10 / 4;

				int currStat = SpecialChars::getCJKAttr(m_story.text(lastChar));
				int nextStat = SpecialChars::getCJKAttr(m_story.text(lastChar + 1));
				if (currStat != 0)
				{	// current char is CJK
					if (nextStat == 0
							&& !SpecialChars::isBreakingSpace(m_story.text(lastChar + 1))
							&& SpecialChars::isCJK(m_story.text(lastChar + 1).unicode())) {
						switch(currStat & SpecialChars::CJK_CHAR_MASK) {
						case SpecialChars::CJK_KANJI:
						case SpecialChars::CJK_KANA:
						case SpecialChars::CJK_NOTOP:
							run.extraWidth += quarterEM;
						}
					} else {	// next char is CJK, too
						switch(currStat & SpecialChars::CJK_CHAR_MASK) {
						case SpecialChars::CJK_FENCE_END:
							switch(nextStat & SpecialChars::CJK_CHAR_MASK) {
							case SpecialChars::CJK_FENCE_BEGIN:
							case SpecialChars::CJK_FENCE_END:
							case SpecialChars::CJK_COMMA:
							case SpecialChars::CJK_PERIOD:
							case SpecialChars::CJK_MIDPOINT:
								run.extraWidth -= halfEM;
							}
							break;
						case SpecialChars::CJK_COMMA:
						case SpecialChars::CJK_PERIOD:
							switch(nextStat & SpecialChars::CJK_CHAR_MASK) {
							case SpecialChars::CJK_FENCE_BEGIN:
							case SpecialChars::CJK_FENCE_END:
								run.extraWidth -= halfEM;;
							}
							break;
						case SpecialChars::CJK_MIDPOINT:
							switch(nextStat & SpecialChars::CJK_CHAR_MASK) {
							case SpecialChars::CJK_FENCE_BEGIN:
								run.extraWidth -= halfEM;
							}
							break;
						case SpecialChars::CJK_FENCE_BEGIN:
							int prevStat = SpecialChars::getCJKAttr(m_story.text(lastChar - 1));
							if ((prevStat & SpecialChars::CJK_CHAR_MASK) == SpecialChars::CJK_FENCE_BEGIN)
							{
								run.extraWidth -= halfEM;
								run.xoffset -= halfEM;
							}
							else
							{
								run.setFlag(ScLayout_CJKFence);
							}
							break;
						}
					}
				} else {	// current char is not CJK
					if (nextStat != 0
							&& !SpecialChars::isBreakingSpace(m_story.text(lastChar))
							&& !SpecialChars::isCJK(m_story.text(lastChar + 1).unicode())) {
						switch(nextStat & SpecialChars::CJK_CHAR_MASK) {
						case SpecialChars::CJK_KANJI:
						case SpecialChars::CJK_KANA:
						case SpecialChars::CJK_NOTOP:
							// use the size of the current char instead of the next one
							run.extraWidth += quarterEM;
						}
					}
				}
			}

			result.glyphs().append(run);
		}
		hb_buffer_destroy(hbBuffer);

	}

	m_textMap.clear();
	m_text = "";
	result.needsContext(m_contextNeeded);
	return result;
}
