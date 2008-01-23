//Based on http://doc.trolltech.com/qq/qq21-syntaxhighlighter.html#example
/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexhelpers.h  -  description
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

#ifndef LATEXHELPERS_H
#define LATEXHELPERS_H

#include <QSyntaxHighlighter>
#include <QXmlStreamReader>
#include <QString>
#include <QObject>
#include <QPointer>

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

class I18nXmlStreamReader : public QXmlStreamReader
{
	public:
		I18nXmlStreamReader() : QXmlStreamReader() {}
		I18nXmlStreamReader(QIODevice *device) : QXmlStreamReader(device) {}
		QString readI18nText(bool unindent=false);
};

class LatexConfigParser
{
	public:
		LatexConfigParser() {};
		bool parseConfigFile(QString fn);
		QString executable() const { return m_executable; }
		QString imageExtension() const { return m_imageExtension; }
		/** Returns the path to the highlighter definition.
		 * The different options are handled in this function
		 * so the caller doesn't have to care about.
		 */
		QString highlighter() const; //TODO
		QString emptyFrameText() const { return m_emptyFrameText; }
		QString preamble() const { return m_preamble; }
		QString postamble() const { return m_postamble; }
		QString description() const { return m_description; }
		QString error() const { return m_error; }
		QString icon() const { return m_icon; }
		QString filename() const { return m_filename; }
		QMap<QString,QString> properties;
	protected:
		QString m_error;
		QString m_description, m_executable, m_imageExtension, m_emptyFrameText;
		QString m_preamble, m_postamble, m_highlight_kate, m_highlight_file, m_icon;
		QString m_filename;
		I18nXmlStreamReader xml;
		void formatError(QString message);
		void parseElements();
		void parseTab();
		void ignoreList();
};

class LatexConfigCache;
class LatexConfigCache {
	public:
		static LatexConfigCache* instance();
		static QStringList defaultConfigs();
		LatexConfigCache() {}
		LatexConfigParser* parser(QString filename, bool warnOnError = false);
		bool hasError(QString filename);
	protected:
		void createParser(QString filename, bool warnOnError);
	private:
		QMap<QString, LatexConfigParser*> parsers;
		QMap<QString, bool> error;
		static LatexConfigCache *_instance;
};


#endif
