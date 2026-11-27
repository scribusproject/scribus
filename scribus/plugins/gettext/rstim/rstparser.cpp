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
#include "rstparser.h"

#include <QRegularExpression>
#include <QSet>

#include <algorithm>

namespace
{
	QString expandTabs(const QString& s)
	{
		if (!s.contains(QLatin1Char('\t')))
			return s;
		QString result;
		int column = 0;
		for (const QChar& c : s)
		{
			if (c == QLatin1Char('\t'))
			{
				int spaces = 8 - (column % 8);
				result += QString(spaces, QLatin1Char(' '));
				column += spaces;
			}
			else
			{
				result += c;
				++column;
			}
		}
		return result;
	}

	bool isBlank(const QString& s)
	{
		for (const QChar& c : s)
		{
			if (!c.isSpace())
				return false;
		}
		return true;
	}

	int indentOf(const QString& s)
	{
		int n = 0;
		while (n < s.size() && s[n] == QLatin1Char(' '))
			++n;
		return n;
	}

	QString rstrip(const QString& s)
	{
		int end = s.size();
		while (end > 0 && s[end - 1].isSpace())
			--end;
		return s.left(end);
	}

	// Remove the common indentation of all non-blank lines
	QStringList dedent(const QStringList& lines, int maxStrip = -1)
	{
		int strip = -1;
		for (const QString& line : lines)
		{
			if (isBlank(line))
				continue;
			int ind = indentOf(line);
			if (strip < 0 || ind < strip)
				strip = ind;
		}
		if (strip < 0)
			strip = 0;
		if (maxStrip >= 0)
			strip = qMin(strip, maxStrip);
		QStringList result;
		result.reserve(lines.size());
		for (const QString& line : lines)
			result.append(isBlank(line) ? QString() : line.mid(strip));
		return result;
	}

	// Collect lines from 'from' that are blank or indented; stops at the first
	// non-blank, non-indented line. Trailing blank lines are not included.
	int indentedBlockEnd(const QStringList& lines, int from, int* next)
	{
		int j = from;
		while (j < lines.size() && (isBlank(lines[j]) || indentOf(lines[j]) > 0))
			++j;
		if (next)
			*next = j;
		int end = j;
		while (end > from && isBlank(lines[end - 1]))
			--end;
		return end;
	}

	bool isAdornmentChar(QChar c)
	{
		static const QString chars = QStringLiteral("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
		return chars.contains(c);
	}

	// A non-indented line made of a single repeated punctuation character
	bool isAdornmentLine(const QString& line, QChar* ch = nullptr)
	{
		if (line.isEmpty() || line[0].isSpace())
			return false;
		QString t = rstrip(line);
		QChar c = t[0];
		if (!isAdornmentChar(c))
			return false;
		for (const QChar& x : t)
		{
			if (x != c)
				return false;
		}
		if (ch)
			*ch = c;
		return true;
	}

	bool underlineFits(int adornmentLength, int titleLength)
	{
		return adornmentLength >= titleLength || adornmentLength >= 4;
	}

	// Is line i a section title text with an underline on line i + 1?
	bool isUnderlinedTitle(const QStringList& lines, int i, QChar* ch = nullptr)
	{
		if (i + 1 >= lines.size())
			return false;
		const QString& line = lines[i];
		if (isBlank(line) || line[0].isSpace() || isAdornmentLine(line))
			return false;
		QChar c;
		if (!isAdornmentLine(lines[i + 1], &c))
			return false;
		if (!underlineFits(rstrip(lines[i + 1]).size(), line.trimmed().size()))
			return false;
		if (ch)
			*ch = c;
		return true;
	}

	bool isStartPrefixChar(QChar c)
	{
		if (c.isSpace())
			return true;
		switch (c.category())
		{
			case QChar::Punctuation_Dash:
			case QChar::Punctuation_Open:
			case QChar::Punctuation_InitialQuote:
			case QChar::Punctuation_FinalQuote:
			case QChar::Punctuation_Other:
				return true;
			default:
				break;
		}
		return c == QLatin1Char('<');
	}

	bool isEndSuffixChar(QChar c)
	{
		if (c.isSpace())
			return true;
		switch (c.category())
		{
			case QChar::Punctuation_Dash:
			case QChar::Punctuation_Close:
			case QChar::Punctuation_InitialQuote:
			case QChar::Punctuation_FinalQuote:
			case QChar::Punctuation_Other:
				return true;
			default:
				break;
		}
		return c == QLatin1Char('>');
	}

	bool endSuffixOk(const QString& s, int p)
	{
		return p >= s.size() || isEndSuffixChar(s[p]);
	}

	// Inline markup start-string rules from the reStructuredText specification
	bool startOk(const QString& s, int pos, int markLength)
	{
		int after = pos + markLength;
		if (after >= s.size() || s[after].isSpace())
			return false;
		if (pos == 0)
			return true;
		QChar prev = s[pos - 1];
		if (!isStartPrefixChar(prev))
			return false;
		static const QString openers = QStringLiteral("'\"<([{");
		static const QString closers = QStringLiteral("'\">)]}");
		int k = openers.indexOf(prev);
		if (k >= 0 && s[after] == closers[k])
			return false;
		return true;
	}

	// Find an end-string. 'from' is the first content character.
	int findEnd(const QString& s, int from, const QString& endString, bool escapes)
	{
		const int last = s.size() - endString.size();
		for (int p = from; p <= last; ++p)
		{
			if (escapes && s[p] == QLatin1Char('\\'))
			{
				++p;
				continue;
			}
			if (p == from || s[p - 1].isSpace())
				continue;
			if (QStringView(s).mid(p, endString.size()) != endString)
				continue;
			if (endSuffixOk(s, p + endString.size()))
				return p;
		}
		return -1;
	}

	QString unescape(const QString& s)
	{
		QString result;
		result.reserve(s.size());
		for (int k = 0; k < s.size(); ++k)
		{
			if (s[k] == QLatin1Char('\\'))
			{
				if (k + 1 < s.size() && !s[k + 1].isSpace())
					result += s[k + 1];
				++k;
				continue;
			}
			result += s[k];
		}
		return result;
	}

	// "Text <target>" -> "Text"; "<target>" alone -> "target"
	QString referenceDisplay(const QString& content)
	{
		static const QRegularExpression embedded(QStringLiteral("^(.*?)\\s*<([^<>]+)>$"), QRegularExpression::DotMatchesEverythingOption);
		QRegularExpressionMatch m = embedded.match(content);
		if (!m.hasMatch())
			return content;
		QString display = m.captured(1).trimmed();
		return display.isEmpty() ? m.captured(2).trimmed() : display;
	}

	int romanValue(const QString& roman)
	{
		static const QString letters = QStringLiteral("ivxlcdm");
		static const int values[] = { 1, 5, 10, 50, 100, 500, 1000 };
		int total = 0;
		int previous = 0;
		for (int k = roman.size() - 1; k >= 0; --k)
		{
			int idx = letters.indexOf(roman[k].toLower());
			if (idx < 0)
				return -1;
			int v = values[idx];
			if (v < previous)
				total -= v;
			else
			{
				total += v;
				previous = v;
			}
		}
		return total;
	}


	// Option lines (":name: value") at the start of a directive body
	QStringList stripDirectiveOptions(const QStringList& body)
	{
		static const QRegularExpression optionRe(QStringLiteral("^:[^:\\s][^:]*:(\\s.*)?$"));
		int k = 0;
		while (k < body.size() && optionRe.match(body[k]).hasMatch())
		{
			++k;
			while (k < body.size() && !isBlank(body[k]) && indentOf(body[k]) > 0)
				++k; // continuation of an option value
		}
		while (k < body.size() && isBlank(body[k]))
			++k;
		return body.mid(k);
	}

	struct EnumMarker
	{
		bool valid { false };
		NumFormat format { Type_1_2_3 };
		bool autoNumber { false };
		bool ambiguousLetter { false }; // a single letter that could be alpha or roman (i, v, x, ...)
		int value { 1 };
		QString prefix;
		QString suffix;
		int contentColumn { 0 };
	};

	EnumMarker parseEnumMarker(const QString& line)
	{
		static const QRegularExpression enumRe(QStringLiteral("^(\\()?([0-9]+|#|[a-zA-Z]|[ivxlcdm]+|[IVXLCDM]+)([.)])(\\s+|$)"));
		EnumMarker marker;
		QRegularExpressionMatch m = enumRe.match(line);
		if (!m.hasMatch())
			return marker;
		marker.prefix = m.captured(1);
		marker.suffix = m.captured(3);
		if (!marker.prefix.isEmpty() && marker.suffix != QLatin1String(")"))
			return marker;
		QString token = m.captured(2);
		if (token == QLatin1String("#"))
		{
			marker.autoNumber = true;
			marker.value = 1;
		}
		else if (token[0].isDigit())
		{
			marker.format = Type_1_2_3;
			marker.value = token.toInt();
		}
		else if (token.size() == 1)
		{
			QChar c = token[0];
			bool upper = c.isUpper();
			int romanV = romanValue(token);
			if (c.toLower() == QLatin1Char('i'))
			{
				marker.format = upper ? Type_I_II_III : Type_i_ii_iii;
				marker.value = 1;
			}
			else
			{
				marker.format = upper ? Type_A_B_C : Type_a_b_c;
				marker.value = c.toLower().unicode() - 'a' + 1;
			}
			marker.ambiguousLetter = (romanV > 0);
		}
		else
		{
			int v = romanValue(token);
			if (v <= 0)
				return marker;
			marker.format = token[0].isUpper() ? Type_I_II_III : Type_i_ii_iii;
			marker.value = v;
		}
		marker.contentColumn = m.capturedLength(0);
		marker.valid = true;
		return marker;
	}

	int letterValue(QChar c)
	{
		return c.toLower().unicode() - 'a' + 1;
	}
}

QVector<RstBlock> RstParser::parse(const QStringList& lines)
{
	QStringList prepared;
	prepared.reserve(lines.size());
	for (int i = 0; i < lines.size(); ++i)
	{
		QString line = lines[i];
		if (i == 0 && line.startsWith(QChar(0xFEFF)))
			line.remove(0, 1);
		line.remove(QLatin1Char('\r'));
		prepared.append(rstrip(expandTabs(line)));
	}
	m_blocks.clear();
	m_adornments.clear();
	parseBody(prepared, 0, false, 0);
	return m_blocks;
}

int RstParser::headingLevelFor(QChar adornment, bool overlined)
{
	QPair<QChar, bool> key(adornment, overlined);
	int idx = m_adornments.indexOf(key);
	if (idx < 0)
	{
		m_adornments.append(key);
		idx = m_adornments.size() - 1;
	}
	return idx + 1;
}

void RstParser::addParagraph(const QString& source, int indent, bool quoted)
{
	if (isBlank(source))
		return;
	RstBlock block;
	block.type = RstBlockType::Paragraph;
	parseInline(source, block.text, block.spans);
	block.indent = indent;
	block.quoted = quoted;
	m_blocks.append(block);
}

void RstParser::addLiteralLines(const QStringList& lines, int indent)
{
	int first = 0;
	int last = lines.size() - 1;
	while (first <= last && isBlank(lines[first]))
		++first;
	while (last >= first && isBlank(lines[last]))
		--last;
	for (int k = first; k <= last; ++k)
	{
		RstBlock block;
		block.type = RstBlockType::Literal;
		block.text = rstrip(lines[k]);
		block.indent = indent;
		m_blocks.append(block);
	}
}

int RstParser::parseLiteralAfter(const QStringList& lines, int i, int indent)
{
	int k = i;
	while (k < lines.size() && isBlank(lines[k]))
		++k;
	if (k >= lines.size())
		return k;

	if (indentOf(lines[k]) > 0)
	{
		int next = k;
		int end = indentedBlockEnd(lines, k, &next);
		addLiteralLines(dedent(lines.mid(k, end - k)), indent);
		return next;
	}

	// Quoted literal block: unindented lines all starting with the same punctuation
	QChar quote = lines[k][0];
	if (!isAdornmentChar(quote))
		return k;
	int j = k;
	while (j < lines.size() && !isBlank(lines[j]) && lines[j][0] == quote)
		++j;
	if (j < lines.size() && !isBlank(lines[j]))
		return k; // not consistently quoted, treat as normal text
	addLiteralLines(lines.mid(k, j - k), indent);
	return j;
}

int RstParser::parseListItem(const QStringList& lines, int i, int indent, int enumLevel, int contentColumn, const RstBlock& itemTemplate)
{
	QStringList body;
	body.append(lines[i].mid(contentColumn));
	int next = i + 1;
	int end = indentedBlockEnd(lines, i + 1, &next);
	QStringList rest = lines.mid(i + 1, end - (i + 1));
	if (body[0].isEmpty() && !rest.isEmpty())
		body.clear(); // marker alone on its line; the body starts on the next line
	body.append(dedent(rest, contentColumn));

	int childEnumLevel = enumLevel + (itemTemplate.type == RstBlockType::Enumerated ? 1 : 0);
	QVector<RstBlock> saved;
	saved.swap(m_blocks);
	parseBody(body, indent + 1, false, childEnumLevel);
	QVector<RstBlock> children;
	children.swap(m_blocks);
	m_blocks.swap(saved);

	RstBlock item = itemTemplate;
	item.indent = indent + 1;
	if (!children.isEmpty() && children.first().type == RstBlockType::Paragraph && !children.first().quoted)
	{
		item.text = children.first().text;
		item.spans = children.first().spans;
		children.removeFirst();
	}
	m_blocks.append(item);
	m_blocks.append(children);
	return next;
}

int RstParser::parseDirective(const QStringList& lines, int i, int indent, bool quoted, int enumLevel)
{
	const QString rest = lines[i].mid(2).trimmed();
	int next = i + 1;
	int end = indentedBlockEnd(lines, i + 1, &next);
	QStringList body = dedent(lines.mid(i + 1, end - (i + 1)));

	// Hyperlink targets, substitution definitions, anonymous targets
	if (rest.startsWith(QLatin1Char('_')) || rest.startsWith(QLatin1Char('|')))
		return next;

	// Footnotes and citations: ".. [label] text"
	static const QRegularExpression footnoteRe(QStringLiteral("^\\[([^\\]]+)\\]\\s*(.*)$"));
	QRegularExpressionMatch fm = footnoteRe.match(rest);
	if (fm.hasMatch())
	{
		QStringList content;
		content.append(QStringLiteral("[%1] %2").arg(fm.captured(1), fm.captured(2)));
		content.append(body);
		parseBody(content, indent, quoted, enumLevel);
		return next;
	}

	static const QRegularExpression directiveRe(QStringLiteral("^([A-Za-z0-9][A-Za-z0-9_.+:-]*?)\\s*::(?:\\s+(.*))?$"));
	QRegularExpressionMatch dm = directiveRe.match(rest);
	if (!dm.hasMatch())
		return next; // comment

	const QString name = dm.captured(1).toLower();
	const QString argument = dm.captured(2).trimmed();
	QStringList content = stripDirectiveOptions(body);

	static const QSet<QString> literalDirectives = {
		QStringLiteral("code"), QStringLiteral("code-block"), QStringLiteral("sourcecode"),
		QStringLiteral("parsed-literal"), QStringLiteral("math"), QStringLiteral("csv-table")
	};
	static const QSet<QString> admonitions = {
		QStringLiteral("attention"), QStringLiteral("caution"), QStringLiteral("danger"),
		QStringLiteral("error"), QStringLiteral("hint"), QStringLiteral("important"),
		QStringLiteral("note"), QStringLiteral("tip"), QStringLiteral("warning"),
		QStringLiteral("seealso")
	};
	static const QSet<QString> titled = {
		QStringLiteral("admonition"), QStringLiteral("topic"), QStringLiteral("sidebar"),
		QStringLiteral("table"), QStringLiteral("list-table"), QStringLiteral("csv-table")
	};
	static const QSet<QString> quotes = {
		QStringLiteral("epigraph"), QStringLiteral("highlights"), QStringLiteral("pull-quote")
	};
	static const QSet<QString> ignored = {
		QStringLiteral("image"), QStringLiteral("raw"), QStringLiteral("include"),
		QStringLiteral("literalinclude"), QStringLiteral("contents"), QStringLiteral("sectnum"),
		QStringLiteral("section-numbering"), QStringLiteral("header"), QStringLiteral("footer"),
		QStringLiteral("meta"), QStringLiteral("title"), QStringLiteral("toctree"),
		QStringLiteral("index"), QStringLiteral("default-role"), QStringLiteral("role"),
		QStringLiteral("highlight"), QStringLiteral("target-notes"), QStringLiteral("unicode"),
		QStringLiteral("date"), QStringLiteral("replace")
	};

	auto addLabel = [&](const QString& label, int labelIndent) {
		RstBlock block;
		block.type = RstBlockType::Paragraph;
		parseInline(label, block.text, block.spans);
		block.spans.clear();
		if (!block.text.isEmpty())
			block.spans.append({ 0, static_cast<int>(block.text.size()), RstInlineType::Strong });
		block.indent = labelIndent;
		block.quoted = quoted;
		if (!block.text.isEmpty())
			m_blocks.append(block);
	};

	if (ignored.contains(name))
		return next;

	if (titled.contains(name) && !argument.isEmpty())
		addLabel(argument, indent);

	if (literalDirectives.contains(name))
	{
		QStringList literal;
		if (name == QLatin1String("math") && !argument.isEmpty())
			literal.append(argument);
		literal.append(content);
		addLiteralLines(literal, indent);
		return next;
	}

	if (admonitions.contains(name))
	{
		QString label = (name == QLatin1String("seealso")) ? QStringLiteral("See also") : name.left(1).toUpper() + name.mid(1);
		addLabel(label, indent);
		QStringList admonitionBody;
		if (!argument.isEmpty())
			admonitionBody.append(argument);
		admonitionBody.append(content);
		parseBody(admonitionBody, indent + 1, false, enumLevel);
		return next;
	}

	if (name == QLatin1String("rubric"))
	{
		addLabel(argument, indent);
		return next;
	}

	if (quotes.contains(name))
	{
		parseBody(content, indent + 1, true, enumLevel);
		return next;
	}

	// figure (caption and legend), titled directives and unknown directives:
	// keep whatever text the body contains
	parseBody(content, indent, quoted, enumLevel);
	return next;
}

void RstParser::parseBody(const QStringList& lines, int indent, bool quoted, int enumLevel)
{
	static const QRegularExpression bulletRe(QStringLiteral("^([-*+\\x{2022}\\x{2023}\\x{2043}])(\\s+|$)"));
	static const QRegularExpression fieldRe(QStringLiteral("^:((?:[^:\\\\]|\\\\.)+):(\\s+|$)"));
	static const QRegularExpression gridTableRe(QStringLiteral("^\\+([-=]+\\+)+$"));
	static const QRegularExpression simpleTableRe(QStringLiteral("^=+( +=+)+$"));

	// The list an enumerated item would continue, if any
	EnumMarker currentList;
	int listStart = 1;
	int listNextValue = 1;

	const int n = lines.size();
	int i = 0;
	while (i < n)
	{
		const QString& line = lines[i];
		if (isBlank(line))
		{
			++i;
			continue;
		}

		// Indented text that nothing claimed: a block quote
		if (indentOf(line) > 0)
		{
			currentList.valid = false;
			int next = i;
			int end = indentedBlockEnd(lines, i, &next);
			parseBody(dedent(lines.mid(i, end - i)), indent + 1, true, enumLevel);
			i = next;
			continue;
		}

		const QString trimmed = line.trimmed();
		QChar adornment;

		// Section title with overline and underline
		if (isAdornmentLine(line, &adornment) && i + 2 < n && !isBlank(lines[i + 1]))
		{
			QChar under;
			const QString title = lines[i + 1].trimmed();
			if (isAdornmentLine(lines[i + 2], &under) && under == adornment
				&& underlineFits(rstrip(lines[i + 2]).size(), title.size()))
			{
				currentList.valid = false;
				RstBlock block;
				block.type = RstBlockType::Heading;
				block.level = headingLevelFor(adornment, true);
				parseInline(title, block.text, block.spans);
				m_blocks.append(block);
				i += 3;
				continue;
			}
		}

		// Section title with underline only
		if (isUnderlinedTitle(lines, i, &adornment))
		{
			currentList.valid = false;
			RstBlock block;
			block.type = RstBlockType::Heading;
			block.level = headingLevelFor(adornment, false);
			parseInline(trimmed, block.text, block.spans);
			m_blocks.append(block);
			i += 2;
			continue;
		}

		// Transition
		if (isAdornmentLine(line) && trimmed.size() >= 4 && (i == 0 || isBlank(lines[i - 1])) && (i + 1 >= n || isBlank(lines[i + 1])))
		{
			currentList.valid = false;
			++i;
			continue;
		}

		// Comments and directives
		if (trimmed.startsWith(QLatin1String("..")) && (trimmed.size() == 2 || trimmed[2] == QLatin1Char(' ')))
		{
			currentList.valid = false;
			i = parseDirective(lines, i, indent, quoted, enumLevel);
			continue;
		}

		// Tables are kept as literal text
		if (gridTableRe.match(trimmed).hasMatch())
		{
			currentList.valid = false;
			int j = i;
			while (j < n && !isBlank(lines[j]))
				++j;
			addLiteralLines(lines.mid(i, j - i), indent);
			i = j;
			continue;
		}
		if (simpleTableRe.match(trimmed).hasMatch())
		{
			currentList.valid = false;
			int end = -1;
			for (int j = i + 1; j < n; ++j)
			{
				if (simpleTableRe.match(lines[j].trimmed()).hasMatch() && (j + 1 >= n || isBlank(lines[j + 1])))
				{
					end = j;
					break;
				}
			}
			if (end < 0)
			{
				end = i;
				while (end + 1 < n && !isBlank(lines[end + 1]))
					++end;
			}
			addLiteralLines(lines.mid(i, end - i + 1), indent);
			i = end + 1;
			continue;
		}

		// Bullet list item
		QRegularExpressionMatch bm = bulletRe.match(line);
		if (bm.hasMatch())
		{
			currentList.valid = false;
			RstBlock item;
			item.type = RstBlockType::Bullet;
			int column = bm.capturedLength(0);
			if (column == bm.capturedLength(1)) // marker alone on the line
				column = 2;
			i = parseListItem(lines, i, indent, enumLevel, column, item);
			continue;
		}

		// Enumerated list item
		EnumMarker marker = parseEnumMarker(line);
		if (marker.valid)
		{
			// The second line of the item must be blank, indented, or another item
			bool isItem = (i + 1 >= n) || isBlank(lines[i + 1]) || indentOf(lines[i + 1]) > 0 || parseEnumMarker(lines[i + 1]).valid;
			if (isItem)
			{
				bool sameDelimiters = currentList.valid
					&& marker.prefix == currentList.prefix
					&& marker.suffix == currentList.suffix;
				if (sameDelimiters && !marker.autoNumber && marker.ambiguousLetter && marker.format != currentList.format)
				{
					// A letter such as "i" or "v" takes the meaning of the list it continues
					QChar letter = line[marker.prefix.size()];
					bool upper = letter.isUpper();
					if ((currentList.format == Type_a_b_c && !upper) || (currentList.format == Type_A_B_C && upper))
					{
						marker.format = currentList.format;
						marker.value = letterValue(letter);
					}
					else if ((currentList.format == Type_i_ii_iii && !upper) || (currentList.format == Type_I_II_III && upper))
					{
						marker.format = currentList.format;
						marker.value = romanValue(QString(letter));
					}
				}
				bool continues = sameDelimiters && (marker.autoNumber || marker.format == currentList.format);
				if (continues)
				{
					if (marker.autoNumber)
					{
						marker.format = currentList.format;
						marker.value = listNextValue;
					}
				}
				else
				{
					if (marker.autoNumber && !currentList.valid)
						marker.format = Type_1_2_3;
					listStart = marker.value;
					currentList = marker;
				}
				listNextValue = marker.value + 1;

				RstBlock item;
				item.type = RstBlockType::Enumerated;
				item.enumFormat = marker.format;
				item.enumPrefix = marker.prefix;
				item.enumSuffix = marker.suffix;
				item.enumStart = listStart;
				item.enumValue = marker.value;
				item.enumLevel = enumLevel;
				int column = marker.contentColumn;
				if (column == marker.prefix.size() + 1 + marker.suffix.size()) // marker alone on the line
					column += 1;
				i = parseListItem(lines, i, indent, enumLevel, column, item);
				continue;
			}
		}

		currentList.valid = false;

		// Field list item
		QRegularExpressionMatch fieldMatch = fieldRe.match(line);
		if (fieldMatch.hasMatch())
		{
			QStringList body;
			QString first = line.mid(fieldMatch.capturedLength(0));
			if (!first.isEmpty())
				body.append(first);
			int next = i + 1;
			int end = indentedBlockEnd(lines, i + 1, &next);
			body.append(dedent(lines.mid(i + 1, end - (i + 1))));

			QVector<RstBlock> saved;
			saved.swap(m_blocks);
			parseBody(body, indent + 1, false, enumLevel);
			QVector<RstBlock> children;
			children.swap(m_blocks);
			m_blocks.swap(saved);

			RstBlock field;
			field.type = RstBlockType::Paragraph;
			field.indent = indent;
			field.quoted = quoted;
			QString name;
			QVector<RstSpan> nameSpans;
			parseInline(fieldMatch.captured(1), name, nameSpans);
			field.text = name + QStringLiteral(": ");
			field.spans.append({ 0, static_cast<int>(name.size()) + 1, RstInlineType::Strong });
			if (!children.isEmpty() && children.first().type == RstBlockType::Paragraph && !children.first().quoted)
			{
				int offset = field.text.size();
				field.text += children.first().text;
				for (RstSpan span : children.first().spans)
				{
					span.start += offset;
					field.spans.append(span);
				}
				children.removeFirst();
			}
			m_blocks.append(field);
			m_blocks.append(children);
			i = next;
			continue;
		}

		// Line block
		if (trimmed.startsWith(QLatin1Char('|')) && (trimmed.size() == 1 || trimmed[1] == QLatin1Char(' ')))
		{
			QStringList entries;
			int j = i;
			while (j < n && !isBlank(lines[j]))
			{
				const QString& l = lines[j];
				if (indentOf(l) == 0 && l.startsWith(QLatin1Char('|')) && (l.size() == 1 || l[1] == QLatin1Char(' ')))
					entries.append(l.mid(2));
				else if (!entries.isEmpty())
					entries.last() += QLatin1Char(' ') + l.trimmed();
				++j;
			}
			for (const QString& entry : entries)
			{
				RstBlock block;
				block.type = RstBlockType::Paragraph;
				parseInline(entry, block.text, block.spans);
				block.indent = indent;
				block.quoted = quoted;
				m_blocks.append(block); // empty entries are kept as blank lines
			}
			i = j;
			continue;
		}

		// Doctest block
		if (trimmed.startsWith(QLatin1String(">>>")))
		{
			int j = i;
			while (j < n && !isBlank(lines[j]))
				++j;
			addLiteralLines(lines.mid(i, j - i), indent);
			i = j;
			continue;
		}

		// Definition list item: a term followed directly by an indented definition
		if (i + 1 < n && !isBlank(lines[i + 1]) && indentOf(lines[i + 1]) > 0 && !trimmed.endsWith(QLatin1String("::")))
		{
			RstBlock term;
			term.type = RstBlockType::DefinitionTerm;
			parseInline(trimmed, term.text, term.spans);
			term.indent = indent;
			m_blocks.append(term);
			int next = i + 1;
			int end = indentedBlockEnd(lines, i + 1, &next);
			parseBody(dedent(lines.mid(i + 1, end - (i + 1))), indent + 1, false, enumLevel);
			i = next;
			continue;
		}

		// Paragraph
		QStringList paragraphLines;
		int j = i;
		while (j < n && !isBlank(lines[j]) && indentOf(lines[j]) == 0)
		{
			if (j > i && isUnderlinedTitle(lines, j))
				break;
			paragraphLines.append(lines[j].trimmed());
			++j;
		}
		QString text = paragraphLines.join(QLatin1Char(' '));
		bool literalFollows = false;
		if (text.endsWith(QLatin1String("::")) && !text.endsWith(QLatin1String("\\::")))
		{
			literalFollows = true;
			if (text == QLatin1String("::"))
				text.clear();
			else if (text.size() > 2 && text[text.size() - 3].isSpace())
				text = rstrip(text.left(text.size() - 2));
			else
				text.chop(1);
		}
		addParagraph(text, indent, quoted);
		i = j;
		if (literalFollows)
			i = parseLiteralAfter(lines, i, indent);
	}
}

void RstParser::parseInline(const QString& s, QString& text, QVector<RstSpan>& spans)
{
	static const QRegularExpression rolePrefixRe(QStringLiteral(":([A-Za-z0-9](?:[A-Za-z0-9]|[-_.+:](?=[A-Za-z0-9]))*):`"));
	static const QRegularExpression roleSuffixRe(QStringLiteral(":([A-Za-z0-9](?:[A-Za-z0-9]|[-_.+:](?=[A-Za-z0-9]))*):"));
	static const QRegularExpression footnoteRefRe(QStringLiteral("\\[([0-9]+|#[A-Za-z0-9_.-]*|\\*|[A-Za-z][A-Za-z0-9_.-]*)\\]_"));
	static const QRegularExpression simpleRefRe(QStringLiteral("((?:[^\\W_])+(?:[-._+:](?:[^\\W_])+)*)__?"), QRegularExpression::UseUnicodePropertiesOption);

	static const QSet<QString> emphasisRoles = {
		QStringLiteral("emphasis"), QStringLiteral("title-reference"), QStringLiteral("title"),
		QStringLiteral("t"), QStringLiteral("dfn"), QStringLiteral("cite")
	};
	static const QSet<QString> literalRoles = {
		QStringLiteral("literal"), QStringLiteral("code"), QStringLiteral("kbd"), QStringLiteral("file"),
		QStringLiteral("samp"), QStringLiteral("command"), QStringLiteral("program"), QStringLiteral("option"),
		QStringLiteral("envvar"), QStringLiteral("regexp"), QStringLiteral("makevar"),
		QStringLiteral("mailheader"), QStringLiteral("math"),
		// Sphinx domain roles render as code
		QStringLiteral("func"), QStringLiteral("meth"), QStringLiteral("class"), QStringLiteral("mod"),
		QStringLiteral("attr"), QStringLiteral("data"), QStringLiteral("const"), QStringLiteral("exc"),
		QStringLiteral("obj"), QStringLiteral("type"), QStringLiteral("var"), QStringLiteral("macro"),
		QStringLiteral("member"), QStringLiteral("enum"), QStringLiteral("enumerator"),
		QStringLiteral("struct"), QStringLiteral("union"), QStringLiteral("expr")
	};

	text.clear();
	spans.clear();

	auto addSpan = [&](const QString& content, RstInlineType type) {
		if (content.isEmpty())
			return;
		spans.append({ static_cast<int>(text.size()), static_cast<int>(content.size()), type });
		text += content;
	};

	auto emitInterpreted = [&](const QString& rawContent, const QString& role, bool isReference) {
		QString content = unescape(rawContent);
		if (isReference)
		{
			text += referenceDisplay(content);
			return;
		}
		QString r = role.toLower();
		if (r.isEmpty() || emphasisRoles.contains(r))
		{
			addSpan(content, RstInlineType::Emphasis);
			return;
		}
		if (r == QLatin1String("strong"))
		{
			addSpan(content, RstInlineType::Strong);
			return;
		}
		if (r == QLatin1String("pep") || r == QLatin1String("rfc"))
		{
			text += r.toUpper() + QLatin1Char(' ') + referenceDisplay(content);
			return;
		}
		QString display = referenceDisplay(content);
		QString bareRole = r.contains(QLatin1Char(':')) ? r.section(QLatin1Char(':'), -1) : r;
		if (literalRoles.contains(bareRole) || (r.contains(QLatin1Char(':')) && !literalRoles.contains(r)))
		{
			if (display.startsWith(QLatin1Char('!')))
				display.remove(0, 1);
			if (display.startsWith(QLatin1Char('~')))
				display = display.mid(1).section(QLatin1Char('.'), -1);
			addSpan(display, RstInlineType::Literal);
			return;
		}
		// ref, doc, term, sub, sup, abbr and unknown roles: plain text
		text += display;
	};

	// Interpreted text starting at the backtick 'bt'. Returns the index after it, or -1.
	auto parseInterpreted = [&](int bt, const QString& prefixRole) -> int {
		int contentStart = bt + 1;
		if (contentStart >= s.size() || s[contentStart].isSpace())
			return -1;
		for (int p = contentStart + 1; p < s.size(); ++p)
		{
			if (s[p] == QLatin1Char('\\'))
			{
				++p;
				continue;
			}
			if (s[p] != QLatin1Char('`') || s[p - 1].isSpace())
				continue;
			int q = p + 1;
			QString role = prefixRole;
			bool isReference = false;
			if (prefixRole.isEmpty())
			{
				if (QStringView(s).mid(q, 2) == QLatin1String("__"))
				{
					isReference = true;
					q += 2;
				}
				else if (q < s.size() && s[q] == QLatin1Char('_'))
				{
					isReference = true;
					q += 1;
				}
				else
				{
					QRegularExpressionMatch rm = roleSuffixRe.match(s, q, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
					if (rm.hasMatch())
					{
						role = rm.captured(1);
						q += rm.capturedLength(0);
					}
				}
			}
			if (!endSuffixOk(s, q))
				continue;
			emitInterpreted(s.mid(contentStart, p - contentStart), role, isReference);
			return q;
		}
		return -1;
	};

	const int n = s.size();
	int pos = 0;
	while (pos < n)
	{
		const QChar c = s[pos];
		const bool atWordStart = (pos == 0) || isStartPrefixChar(s[pos - 1]);

		if (c == QLatin1Char('\\'))
		{
			if (pos + 1 < n && !s[pos + 1].isSpace())
				text += s[pos + 1];
			pos += 2;
			continue;
		}

		if (c == QLatin1Char('`') && QStringView(s).mid(pos, 2) == QLatin1String("``") && startOk(s, pos, 2))
		{
			int e = findEnd(s, pos + 2, QStringLiteral("``"), false);
			if (e > 0)
			{
				addSpan(s.mid(pos + 2, e - pos - 2), RstInlineType::Literal);
				pos = e + 2;
				continue;
			}
		}

		if (c == QLatin1Char('*') && QStringView(s).mid(pos, 2) == QLatin1String("**") && startOk(s, pos, 2))
		{
			int e = findEnd(s, pos + 2, QStringLiteral("**"), true);
			if (e > 0)
			{
				addSpan(unescape(s.mid(pos + 2, e - pos - 2)), RstInlineType::Strong);
				pos = e + 2;
				continue;
			}
		}

		if (c == QLatin1Char('*') && startOk(s, pos, 1))
		{
			int e = findEnd(s, pos + 1, QStringLiteral("*"), true);
			if (e > 0)
			{
				addSpan(unescape(s.mid(pos + 1, e - pos - 1)), RstInlineType::Emphasis);
				pos = e + 1;
				continue;
			}
		}

		if (c == QLatin1Char('`') && startOk(s, pos, 1))
		{
			int e = parseInterpreted(pos, QString());
			if (e > 0)
			{
				pos = e;
				continue;
			}
		}

		if (c == QLatin1Char(':') && atWordStart)
		{
			QRegularExpressionMatch rm = rolePrefixRe.match(s, pos, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
			if (rm.hasMatch())
			{
				int e = parseInterpreted(pos + rm.capturedLength(0) - 1, rm.captured(1));
				if (e > 0)
				{
					pos = e;
					continue;
				}
			}
		}

		// Inline target: _`text`
		if (c == QLatin1Char('_') && pos + 1 < n && s[pos + 1] == QLatin1Char('`') && startOk(s, pos, 2))
		{
			int e = findEnd(s, pos + 2, QStringLiteral("`"), true);
			if (e > 0)
			{
				text += unescape(s.mid(pos + 2, e - pos - 2));
				pos = e + 1;
				continue;
			}
		}

		// Substitution reference: |name|, |name|_, |name|__
		if (c == QLatin1Char('|') && startOk(s, pos, 1))
		{
			int found = -1;
			for (int p = pos + 2; p < n; ++p)
			{
				if (s[p] == QLatin1Char('\\'))
				{
					++p;
					continue;
				}
				if (s[p] != QLatin1Char('|') || s[p - 1].isSpace())
					continue;
				int q = p + 1;
				while (q < n && q < p + 3 && s[q] == QLatin1Char('_'))
					++q;
				if (endSuffixOk(s, q))
				{
					text += unescape(s.mid(pos + 1, p - pos - 1));
					found = q;
					break;
				}
			}
			if (found > 0)
			{
				pos = found;
				continue;
			}
		}

		// Footnote and citation references: [1]_, [#]_, [*]_, [CIT2002]_
		if (c == QLatin1Char('[') && atWordStart)
		{
			QRegularExpressionMatch fm = footnoteRefRe.match(s, pos, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
			if (fm.hasMatch() && endSuffixOk(s, pos + fm.capturedLength(0)))
			{
				text += QLatin1Char('[') + fm.captured(1) + QLatin1Char(']');
				pos += fm.capturedLength(0);
				continue;
			}
		}

		// Simple hyperlink reference: word_ or word__
		if (c.isLetterOrNumber() && atWordStart)
		{
			QRegularExpressionMatch rm = simpleRefRe.match(s, pos, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
			if (rm.hasMatch() && endSuffixOk(s, pos + rm.capturedLength(0)))
			{
				text += rm.captured(1);
				pos += rm.capturedLength(0);
				continue;
			}
		}

		text += c;
		++pos;
	}
}
