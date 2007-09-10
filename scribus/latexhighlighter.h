//Based on http://doc.trolltech.com/qq/qq21-syntaxhighlighter.html#example
/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexhighlighter.h  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef LATEXHIGHLIGHTER_H
#define LATEXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class LatexHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

	public:
		enum Construct {
			Math,
			Command,
			ScribusSpecial,
			BlockName,
			Comment,
			LastConstruct = Comment
		};

		LatexHighlighter(QTextDocument *document);

		void setFormatFor(Construct construct,
						  const QTextCharFormat &format);
		QTextCharFormat formatFor(Construct construct) const
		{ return m_formats[construct]; }
    
	protected:
		enum State {
			NormalState = -1,
			InDisplayMath,
			InMath,
		};
    
		void highlightBlock(const QString &text);
    
	private:
		QTextCharFormat m_formats[LastConstruct + 1];
};
#endif
