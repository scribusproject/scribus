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

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <q3syntaxhighlighter.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QCloseEvent>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class Q3ListView;
class Q3ListViewItem;
class Q3TextEdit;
class QMenuBar;
class QStatusBar;


/*! \brief This is simple "IDE"/python console for interactive commands execution.
It's used e.g. like Tora (SQLnavigator) console. Sample: highlight some code,
press F9, then see the results.
\author Petr Vanek <petr@yarpen.cz>
*/
class PythonConsole : public QWidget
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

		//! \brief Programmer's editor ;)
		Q3TextEdit* commandEdit;
		//! \brief Results viewer
		Q3TextEdit* outputEdit;

		//! \brief Close event for turning the action off
		void closeEvent(QCloseEvent *);

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
		\param para number of the paragraph (0 = 1st...)
		\param pos position in the text like (0 = 1st...)
		*/
		virtual void commandEdit_cursorPositionChanged(int para, int pos);

	signals:
		//! \brief Menu indication trigger
		void paletteShown(bool);
		//! \brief Scripter Core launcher
		void runCommand();

	protected:
		//! \brief prepare Python "script" from GUI widget
		void parsePythonString();
		Q3GridLayout* gridLayout;
		Q3VBoxLayout* editorsLayout;
		QMenuBar* menuBar;
		QStatusBar* statusBar;
		//! \brief String with the script to run (part of the all text)
		QString m_command;

	protected slots:
		virtual void languageChange();

};

/*! \brief Store colors for syntax highligter.
It provides defaults, loading and storing preferences.
\author Petr Vanek <petr@yarpen.cz> */
class SyntaxColors
{
	public:
		SyntaxColors();
		~SyntaxColors();
		QColor errorColor;
		QColor commentColor;
		QColor keywordColor;
		QColor signColor;
		QColor numberColor;
		QColor stringColor;
		QColor textColor;
	private:
		/*! \brief Converts QColor into #rrggbb string.
		\param color a QColor to convert. */
		QString qcolor2named(QColor color);
};

/*! \brief Simple syntax highlighting for Scripter (QTextEdit).
Based on the source of the Python Realizer (http://www.python-realizer.net)
but very simplifier. Improved too (of course).
TODO: colors of the higlited texts. User should set the colors in the
      preferences. Waiting for the new plugin API.
\author Petr Vanek, <petr@yarpen.cz>
\author Richard Magnor Stenbro <stenbror@hotmail.com>
*/
class SyntaxHighlighter : public Q3SyntaxHighlighter
{
	public:
		SyntaxHighlighter(Q3TextEdit *textEdit);

		/*! \brief Reimplementation of the Qt highligtion for python.
		\param text string (one row) provided by text editor via QSyntaxHighlighter inheritance.
		\param endStateOfLastPara how is the syntax left for next paragraph? 0 - normal text, 1 - multirows comment continues
		*/
		int highlightParagraph(const QString &text, int endStateOfLastPara);

	private:
		//! \brief Reserved python keywords
		QStringList keywords;
		SyntaxColors colors;

};

#endif
