/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "swsyntaxhighlighter.h"

#include <QFont>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QString>

SWSyntaxHighlighter::SWSyntaxHighlighter(QTextEdit *textEdit)
	: QSyntaxHighlighter(textEdit)
{
}

void SWSyntaxHighlighter::highlightBlock(const QString &text)
{
	// position in the text
	if (text.isEmpty())
		return;
	if (text[0] == '#')
	{
		QFont f(document()->defaultFont());
		f.setItalic(true);
		QTextCharFormat myClassFormat;
		myClassFormat.setFont(f);
		myClassFormat.setForeground(Qt::gray);
		setFormat(0, text.length(), myClassFormat);
	}
	return;
}

