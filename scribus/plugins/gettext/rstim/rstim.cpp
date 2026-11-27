/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Fri Oct 30 2026
	copyright            : (C) 2026 by Scribus Team
 ***************************************************************************/
#include "rstim.h"

#include <QFile>
#include <QTextCodec>

#include "commonstrings.h"
#include "numeration.h"
#include "prefsmanager.h"
#include "scfonts.h"
#include "scribusdoc.h"
#include "text/specialchars.h"

QString FileFormatName()
{
	return QObject::tr("reStructuredText Document");
}

QStringList FileExtensions()
{
	return QStringList() << "rst" << "rest";
}

void GetText2(const QString& filename, const QString& encoding, bool textOnly, bool prefix, bool append, PageItem *textItem)
{
	RstIm rstim(filename, encoding, textItem, textOnly, prefix, append);
}

namespace
{
	// Enumerated items continue the previous paragraph's count in Scribus unless
	// the local numbering resets (see ScribusDoc::updateLocalNums()): when the
	// previous paragraph is not numbered or uses another format. When it resets,
	// the item's value must come from numStart. All lists use numbering level 0,
	// because Scribus shows higher levels as outline numbers ("2.a)").
	bool scribusResetsNumbering(const RstBlock* previous, const RstBlock& block)
	{
		if (previous == nullptr || previous->type != RstBlockType::Enumerated)
			return true;
		return previous->enumFormat != block.enumFormat;
	}
}

RstIm::RstIm(const QString& fileName, const QString& encoding, PageItem *textItem, bool textOnly, bool prefix, bool append)
	: m_Doc(textItem->doc()),
	  m_item(textItem),
	  m_prefixName(prefix),
	  m_textOnly(textOnly),
	  m_append(append)
{
	QStringList lines = readFile(fileName, encoding);

	if (!m_append)
	{
		ParagraphStyle newStyle;
		newStyle.setDefaultStyle(false);
		newStyle.setParent(CommonStrings::DefaultParagraphStyle);
		m_item->itemText.clear();
		m_item->itemText.setDefaultStyle(newStyle);
	}

	if (!m_textOnly)
		setupFonts();

	RstParser parser;
	insertBlocks(parser.parse(lines));

	if (!m_textOnly)
	{
		m_Doc->redefineStyles(m_paraStyles, false);
		m_Doc->redefineCharStyles(m_charStyles, false);
	}

	m_item->itemText.trim();
	m_item->itemText.invalidateLayout();
}

QStringList RstIm::readFile(const QString& fileName, const QString& encoding) const
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return QStringList();
	QByteArray raw = file.readAll();
	file.close();

	QString text;
	QTextCodec* codec = nullptr;
	if (!encoding.isEmpty())
		codec = QTextCodec::codecForName(encoding.toLatin1());
	// Docutils reads UTF-8 by default; a BOM always wins
	if (raw.startsWith("\xEF\xBB\xBF") || codec == nullptr || codec->mibEnum() == 106)
		text = QString::fromUtf8(raw);
	else
		text = codec->toUnicode(raw);
	return text.split(QLatin1Char('\n'));
}

void RstIm::setupFonts()
{
	SCFonts& fonts = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts;

	const CharStyle& defaultCharStyle = m_item->itemText.defaultStyle().charStyle();
	m_regularFace = defaultCharStyle.font();
	m_baseFontSize = defaultCharStyle.fontSize();
	if (m_regularFace.isNone() || m_regularFace.isReplacement())
		m_regularFace = fonts.findFont(m_Doc->itemToolPrefs().textFont);
	if (m_baseFontSize <= 0.0)
		m_baseFontSize = m_Doc->itemToolPrefs().textSize;

	auto usableFace = [&](const QString& family, const QString& style) -> ScFace {
		if (family.isEmpty() || style.isEmpty())
			return ScFace();
		if (!fonts.fontMap.value(family).contains(style))
			return ScFace();
		const ScFace& face = fonts.findFont(family, style);
		if (face.isNone() || face.isReplacement() || !face.usable())
			return ScFace();
		return face;
	};

	const QString family = m_regularFace.family();
	m_boldFace = usableFace(family, fonts.getBoldStyle(family));
	m_italicFace = usableFace(family, fonts.getItalicStyle(family));

	static const QStringList monoFamilies = {
		QStringLiteral("Menlo"), QStringLiteral("SF Mono"), QStringLiteral("Monaco"),
		QStringLiteral("DejaVu Sans Mono"), QStringLiteral("Liberation Mono"), QStringLiteral("Noto Sans Mono"),
		QStringLiteral("Source Code Pro"), QStringLiteral("Consolas"), QStringLiteral("Cousine"),
		QStringLiteral("Courier New"), QStringLiteral("Courier"), QStringLiteral("Nimbus Mono PS"),
		QStringLiteral("FreeMono")
	};
	for (const QString& mono : monoFamilies)
	{
		m_monoFace = usableFace(mono, fonts.getRegularStyle(mono));
		if (!m_monoFace.isNone())
			break;
	}
	if (m_monoFace.isNone())
	{
		for (auto it = fonts.fontMap.cbegin(); it != fonts.fontMap.cend(); ++it)
		{
			ScFace face = usableFace(it.key(), fonts.getRegularStyle(it.key()));
			if (!face.isNone() && face.isFixedPitch())
			{
				m_monoFace = face;
				break;
			}
		}
	}
}

QString RstIm::styleName(const QString& baseName) const
{
	return m_prefixName ? m_item->itemName() + "_" + baseName : baseName;
}

ParagraphStyle RstIm::newParagraphStyle(const QString& name) const
{
	ParagraphStyle style;
	style.setName(name);
	style.setParent(CommonStrings::DefaultParagraphStyle);
	style.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	style.setContext(m_item->itemText.defaultStyle().context());
	return style;
}

// A style the document already has is used as it is, so that a template's own
// styles are kept. Returns true if the style was added to the import set.
bool RstIm::registerParagraphStyle(const ParagraphStyle& style)
{
	if (m_Doc->paragraphStyles().contains(style.name()) || m_paraStyles.contains(style.name()))
		return false;
	if (!style.charStyle().font().isNone())
		m_Doc->AddFont(style.charStyle().font().scName());
	m_paraStyles.create(style);
	return true;
}

double RstIm::indentWidth(int indent) const
{
	return indent * 1.5 * m_baseFontSize / 10.0;
}

double RstIm::textWidth(const QString& text) const
{
	const double size = m_baseFontSize / 10.0;
	if (m_regularFace.isNone())
		return text.size() * size * 0.5;
	double width = 0.0;
	for (const QChar& c : text)
		width += m_regularFace.glyphWidth(m_regularFace.char2CMap(c.unicode()), size);
	return width;
}

QString RstIm::paragraphStyleFor(const RstBlock& block)
{
	switch (block.type)
	{
		case RstBlockType::Heading:
		{
			static const double factors[] = { 2.0, 1.6, 1.35, 1.2, 1.1, 1.0 };
			const int level = qMax(1, block.level);
			const QString name = styleName(QString("Heading %1").arg(level));
			ParagraphStyle style = newParagraphStyle(name);
			const double size = m_baseFontSize * factors[qMin(level, 6) - 1];
			style.charStyle().setFontSize(size);
			if (!m_boldFace.isNone())
				style.charStyle().setFont(m_boldFace);
			style.setGapBefore(size / 10.0 * 0.75);
			style.setGapAfter(size / 10.0 * 0.25);
			style.setKeepWithNext(true);
			registerParagraphStyle(style);
			return name;
		}
		case RstBlockType::Paragraph:
		{
			if (block.indent == 0 && !block.quoted)
				return QString();
			const QString base = block.quoted ? QStringLiteral("Block Quote %1") : QStringLiteral("Indent %1");
			const QString name = styleName(base.arg(block.indent));
			ParagraphStyle style = newParagraphStyle(name);
			style.setLeftMargin(indentWidth(block.indent));
			registerParagraphStyle(style);
			return name;
		}
		case RstBlockType::Literal:
		{
			const QString name = styleName(block.indent == 0 ? QStringLiteral("Literal Block") : QString("Literal Block %1").arg(block.indent));
			ParagraphStyle style = newParagraphStyle(name);
			style.setLeftMargin(indentWidth(block.indent));
			if (!m_monoFace.isNone())
				style.charStyle().setFont(m_monoFace);
			registerParagraphStyle(style);
			return name;
		}
		case RstBlockType::Bullet:
		{
			const QString name = styleName(QString("List %1").arg(block.indent));
			const QString bullet = (block.indent % 2 == 1) ? QString(QChar(0x2022)) : QString(QChar(0x2013));
			ParagraphStyle style = newParagraphStyle(name);
			style.setHasBullet(true);
			style.setBulletStr(bullet);
			style.setLeftMargin(indentWidth(block.indent));
			style.setFirstIndent(-textWidth(bullet + QLatin1Char(' ')));
			registerParagraphStyle(style);
			return name;
		}
		case RstBlockType::Enumerated:
		{
			// e.g. "Numbered List 1", "Numbered List 2 a)", "Numbered List 1 (I)"
			const QString sample = getStringFromNum(block.enumFormat, 1);
			QString base = QString("Numbered List %1").arg(block.indent);
			if (block.enumFormat != Type_1_2_3 || !block.enumPrefix.isEmpty() || block.enumSuffix != QLatin1String("."))
				base += QLatin1Char(' ') + block.enumPrefix + sample + block.enumSuffix;
			const QString name = styleName(base);
			ParagraphStyle style = newParagraphStyle(name);
			style.setHasNum(true);
			// numName stays empty: Scribus turns it into "<local block>" numbering
			// on first layout (ScribusDoc::updateNumbers()). Setting
			// "<local block>" on a named style directly crashes updateNumbers().
			style.setNumFormat(block.enumFormat);
			style.setNumPrefix(block.enumPrefix);
			style.setNumSuffix(block.enumSuffix);
			style.setNumLevel(0);
			style.setNumStart(1);
			style.setLeftMargin(indentWidth(block.indent));
			const QString widest = (block.enumFormat == Type_1_2_3) ? QStringLiteral("8") : getStringFromNum(block.enumFormat, 2);
			style.setFirstIndent(-textWidth(block.enumPrefix + widest + block.enumSuffix + QLatin1Char(' ')));
			registerParagraphStyle(style);
			return name;
		}
		case RstBlockType::DefinitionTerm:
		{
			const QString name = styleName(block.indent == 0 ? QStringLiteral("Definition Term") : QString("Definition Term %1").arg(block.indent));
			ParagraphStyle style = newParagraphStyle(name);
			style.setLeftMargin(indentWidth(block.indent));
			if (!m_boldFace.isNone())
				style.charStyle().setFont(m_boldFace);
			style.setKeepWithNext(true);
			registerParagraphStyle(style);
			return name;
		}
	}
	return QString();
}

QString RstIm::charStyleFor(RstInlineType type)
{
	QString base;
	ScFace face;
	switch (type)
	{
		case RstInlineType::Strong:
			base = QStringLiteral("Bold");
			face = m_boldFace;
			break;
		case RstInlineType::Emphasis:
			base = QStringLiteral("Italic");
			face = m_italicFace;
			break;
		case RstInlineType::Literal:
			base = QStringLiteral("Code");
			face = m_monoFace;
			break;
	}
	const QString name = styleName(base);
	if (m_Doc->charStyles().contains(name) || m_charStyles.contains(name))
		return name;

	CharStyle style;
	style.setName(name);
	if (!face.isNone())
	{
		style.setFont(face);
		m_Doc->AddFont(face.scName());
	}
	m_charStyles.create(style);
	return name;
}

void RstIm::insertBlocks(const QVector<RstBlock>& blocks)
{
	StoryText& story = m_item->itemText;
	if (m_append && story.length() > 0 && story.text(story.length() - 1) != SpecialChars::PARSEP)
		story.insertChars(story.length(), QString(SpecialChars::PARSEP));

	const RstBlock* previous = nullptr;
	for (const RstBlock& block : blocks)
	{
		QString text = block.text;
		QVector<RstSpan> spans = block.spans;

		// The bullet or number itself comes from the paragraph style, so only a
		// separating space goes into the text. In text-only mode the marker is
		// written out instead.
		QString lead;
		if (block.type == RstBlockType::Bullet)
			lead = m_textOnly ? QString(QChar(0x2022)) + QLatin1Char(' ') : QStringLiteral(" ");
		else if (block.type == RstBlockType::Enumerated)
			lead = m_textOnly ? block.enumPrefix + getStringFromNum(block.enumFormat, block.enumValue) + block.enumSuffix + QLatin1Char(' ') : QStringLiteral(" ");
		if (!lead.isEmpty())
		{
			text.prepend(lead);
			for (RstSpan& span : spans)
				span.start += lead.size();
		}

		const int start = story.length();
		if (!text.isEmpty())
			story.insertChars(start, text);
		story.insertChars(story.length(), QString(SpecialChars::PARSEP));

		if (!m_textOnly)
		{
			ParagraphStyle paragraphStyle;
			const QString name = paragraphStyleFor(block);
			if (!name.isEmpty())
				paragraphStyle.setParent(name);
			if (block.type == RstBlockType::Enumerated && block.enumValue != 1 && scribusResetsNumbering(previous, block))
				paragraphStyle.setNumStart(block.enumValue);
			story.setStyle(story.length() - 1, paragraphStyle);

			for (const RstSpan& span : std::as_const(spans))
			{
				if (span.length <= 0)
					continue;
				CharStyle charStyle;
				charStyle.setParent(charStyleFor(span.type));
				story.applyCharStyle(start + span.start, span.length, charStyle);
			}
		}
		previous = &block;
	}
}
