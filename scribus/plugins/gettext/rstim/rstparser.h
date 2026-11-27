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
#ifndef RSTPARSER_H
#define RSTPARSER_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QVector>

#include "numeration.h"

// A reStructuredText parser. It turns RST source into a list of blocks (one per output paragraph)
// carrying plain text and inline style spans. Blocks are mapped to Scribus styles.

enum class RstInlineType
{
	Strong,
	Emphasis,
	Literal
};

struct RstSpan
{
		int start { 0 };
		int length { 0 };
		RstInlineType type { RstInlineType::Emphasis };
};

enum class RstBlockType
{
	Paragraph,      // body text; indent > 0 means nested (list body, definition, quote)
	Heading,        // level = section level (1-based, order of first appearance)
	Literal,        // one line of a literal block, code block or table; no inline markup
	Bullet,         // bullet list item
	Enumerated,     // enumerated list item
	DefinitionTerm  // term of a definition list
};

struct RstBlock
{
		RstBlockType type { RstBlockType::Paragraph };
		QString text;
		QVector<RstSpan> spans;
		int level { 0 };          // Heading: section level
		int indent { 0 };         // nesting depth, used for the left margin
		bool quoted { false };    // Paragraph: part of a block quote
		// Enumerated lists
		NumFormat enumFormat { Type_1_2_3 };
		QString enumPrefix;       // "(" for "(1)"
		QString enumSuffix;       // "." or ")"
		int enumStart { 1 };      // first value of the list this item belongs to
		int enumValue { 1 };      // this item's value
		int enumLevel { 0 };      // number of enumerated lists this one is nested in
};

class RstParser
{
	public:
		QVector<RstBlock> parse(const QStringList& lines);

		static void parseInline(const QString& source, QString& text, QVector<RstSpan>& spans);

	private:
		void parseBody(const QStringList& lines, int indent, bool quoted, int enumLevel);

		// Helpers that consume lines from position i and return the index after the construct
		int parseLiteralAfter(const QStringList& lines, int i, int indent);
		int parseDirective(const QStringList& lines, int i, int indent, bool quoted, int enumLevel);
		int parseListItem(const QStringList& lines, int i, int indent, int enumLevel, int contentColumn, const RstBlock& itemTemplate);

		void addParagraph(const QString& source, int indent, bool quoted);
		void addLiteralLines(const QStringList& lines, int indent);
		int headingLevelFor(QChar adornment, bool overlined);

		QVector<RstBlock> m_blocks;
		QList<QPair<QChar, bool>> m_adornments; // section styles in order of first appearance
};

#endif
