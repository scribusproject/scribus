/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SWSYNTAXHIGHLIGHTER_H
#define SWSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

/*! Simple syntax highlighting for configuration editor (QTextEdit).
\author Petr Vanek, <petr@yarpen.cz>
 */
class SWSyntaxHighlighter : public QSyntaxHighlighter
{
	public:
		SWSyntaxHighlighter(QTextEdit *textEdit);

		/*! Reimplementation of the Qt highligtion for simple cfg file
		\param text string (one row) provided by text editor via QSyntaxHighlighter inheritance.
		 */
		void highlightBlock(const QString &text);
};

#endif
