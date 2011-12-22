/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#ifndef PCONSOLE_H
#define PCONSOLE_H

#include <QLabel>
#include <QMainWindow>
#include <QSyntaxHighlighter>
#include "ui_pconsole.h"


/*! \brief This is simple "IDE"/python console for interactive commands execution.
It's used e.g. like Tora (SQLnavigator) console. Sample: highlight some code,
press F9, then see the results.
\author Petr Vanek <petr@yarpen.cz>
*/
class PythonConsole : public QMainWindow, public Ui::PythonConsole
{
	Q_OBJECT

	public:
		PythonConsole( QWidget* parent = 0);
		~PythonConsole();
		
		void setFonts();

		/*! \brief The command string
		\retval QString with prepared Python commands */
		QString command() { return m_command; };
		//! \brief File name for saving the contents
		QString filename;

		//! \brief Close event for turning the action off
		void closeEvent(QCloseEvent *);

		void updateSyntaxHighlighter();

	public slots:
		//! menu operations
		virtual void slot_runScript();
		virtual void slot_runScriptAsConsole();
		virtual void slot_open();
		virtual void slot_save();
		virtual void slot_saveAs();
		virtual void slot_saveOutput();
		virtual void slot_quit();
		/*! \brief Slot processed after user change cursor postion in "programmer's editor".
		*/
		virtual void commandEdit_cursorPositionChanged();

	signals:
		//! \brief Menu indication trigger
		void paletteShown(bool);
		//! \brief Scripter Core launcher
		void runCommand();

	protected:
		//! \brief prepare Python "script" from GUI widget
		void parsePythonString();

		//! \brief String with the script to run (part of the all text)
		QString m_command;

		QLabel * changedLabel;
		QLabel * cursorLabel;
		QString cursorTemplate;

	protected slots:
		virtual void languageChange();
		void documentChanged(bool state);

};

/*! \brief Store colors for syntax highligter.
It provides defaults, loading and storing preferences.
\author Petr Vanek <petr@yarpen.cz> */
class SyntaxColors
{
	public:
		SyntaxColors();

		QColor errorColor;
		QColor commentColor;
		QColor keywordColor;
		QColor signColor;
		QColor numberColor;
		QColor stringColor;
		QColor textColor;

		void saveToPrefs();

	private:
		/*! \brief Converts QColor into #rrggbb string.
		\param color a QColor to convert. */
		QString qcolor2named(QColor color);
};

/*! \brief Simple syntax highlighting for Scripter (QTextEdit).
Based on the source of the Sqliteman and Qt4 examples.
but very simplifier. Improved too (of course).
TODO: colors of the higlited texts. User should set the colors in the
      preferences. Waiting for the new plugin API.
\author Petr Vanek, <petr@yarpen.cz>
*/
class SyntaxHighlighter : public QSyntaxHighlighter
{
	public:
		SyntaxHighlighter(QTextEdit *textEdit);

	protected:
		virtual void highlightBlock(const QString &text);

		struct HighlightingRule
		{
			QRegExp pattern;
			QTextCharFormat format;
		};
		QVector<HighlightingRule> highlightingRules;

		QTextCharFormat keywordFormat;
		QTextCharFormat singleLineCommentFormat;
		QTextCharFormat quotationFormat;
		QTextCharFormat numberFormat;
		QTextCharFormat operatorFormat;

		SyntaxColors colors;

};

#endif
