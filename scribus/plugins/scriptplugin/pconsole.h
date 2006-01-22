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
#include <qwhatsthis.h>
#include <qsyntaxhighlighter.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;
class QTextEdit;
class QMenuBar;

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

		//! String with the script to run (part of the all text)
		QString command;
		//! File name for saving the contents
		QString filename;

		//! Programmer's editor ;)
		QTextEdit* commandEdit;
		//! Results viewer
		QTextEdit* outputEdit;

		//! Close event for turning the action off
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

	signals:
		//! Menu indication trigger
		void paletteShown(bool);
		//! Scripter Core launcher
		void runCommand();

	protected:
		//! prepare Python "script" from GUI widget
		void parsePythonString();
		QGridLayout* gridLayout;
		QVBoxLayout* editorsLayout;
		QMenuBar* menuBar;

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
		\param color a QColor to convert.
		\retval QString with "web" color string */
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
class SyntaxHighlighter : public QSyntaxHighlighter
{
	public:
		SyntaxHighlighter(QTextEdit *textEdit);

		/*! Reimplementation of the Qt highligtion for python.
		\param text string (one row) provided by text editor via QSyntaxHighlighter inheritance.
		\param endStateOfLastPara how is the syntax left for next paragraph? 0 - normal text, 1 - multirows comment continues
		*/
		int highlightParagraph(const QString &text, int endStateOfLastPara);

	private:
		//! Reserved python keywords
		QStringList keywords;
		SyntaxColors colors;

};

#endif
