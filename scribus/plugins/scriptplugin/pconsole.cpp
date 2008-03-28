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

#include "pconsole.h"

#include <QFileDialog>
#include "scribus.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scmessagebox.h"
#include "commonstrings.h"
#include "util_icon.h"


PythonConsole::PythonConsole( QWidget* parent)
	: QMainWindow( parent )
{
	setupUi(this);
	setWindowIcon(loadIcon("AppIcon.png"));

	changedLabel = new QLabel(this);
	cursorTemplate = tr("Col: %1 Row: %2/%3");
	cursorLabel = new QLabel(this);
	statusBar()->addPermanentWidget(changedLabel);
	statusBar()->addPermanentWidget(cursorLabel);

	action_Open->setIcon(loadIcon("16/document-open.png"));
	action_Save->setIcon(loadIcon("16/document-save.png"));
	actionSave_As->setIcon(loadIcon("16/document-save-as.png"));
	action_Exit->setIcon(loadIcon("exit.png"));
	action_Run->setIcon(loadIcon("ok.png"));

	action_Open->setShortcut(tr("Ctrl+O"));
	action_Save->setShortcut(tr("Ctrl+S"));
	action_Run->setShortcut(Qt::Key_F9);
	actionRun_As_Console->setShortcut(Qt::CTRL + Qt::Key_F9);

	commandEdit->setTabStopWidth(qRound(commandEdit->fontPointSize() * 4));

	// install syntax highlighter.
	//SyntaxHighlighter *sxHigh =
	new SyntaxHighlighter(commandEdit);

	languageChange();
	commandEdit_cursorPositionChanged();

	// welcome note
	QString welcomeText("\"\"\"");
	welcomeText += tr("Scribus Python Console");
	welcomeText += "\n\n";
	welcomeText += tr(
			"This is a standard Python console with some \n"
			"known limitations. Please consult the Scribus \n"
			"Scripter documentation for futher information. ");
	welcomeText += "\"\"\"\n";
	commandEdit->setText(welcomeText);
	commandEdit->selectAll();

	connect(commandEdit, SIGNAL(cursorPositionChanged()), this, SLOT(commandEdit_cursorPositionChanged()));
	connect(commandEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(documentChanged(bool)));

	connect(action_Open, SIGNAL(triggered()), this, SLOT(slot_open()));
	connect(action_Save, SIGNAL(triggered()), this, SLOT(slot_save()));
	connect(actionSave_As, SIGNAL(triggered()), this, SLOT(slot_saveAs()));
	connect(action_Exit, SIGNAL(triggered()), this, SLOT(slot_quit()));
	connect(action_Run, SIGNAL(triggered()), this, SLOT(slot_runScript()));
	connect(actionRun_As_Console, SIGNAL(triggered()), this, SLOT(slot_runScriptAsConsole()));
	connect(action_Save_Output, SIGNAL(triggered()), this, SLOT(slot_saveOutput()));
}

PythonConsole::~PythonConsole()
{
}

void PythonConsole::setFonts()
{
	QFont font = QFont("Fixed");
	font.setStyleHint(QFont::TypeWriter);
	font.setPointSize(PrefsManager::instance()->appPrefs.AppFontSize);
	commandEdit->setFont(font);	
	outputEdit->setFont(font);
}

void PythonConsole::closeEvent(QCloseEvent *)
{
	emit paletteShown(false);
}

void PythonConsole::commandEdit_cursorPositionChanged()
{
	QTextCursor cur(commandEdit->textCursor());
	cursorLabel->setText(cursorTemplate.arg(cur.columnNumber()+1)
										.arg(cur.blockNumber()+1)
										.arg(commandEdit->document()->blockCount()));
}

void PythonConsole::documentChanged(bool state)
{
	changedLabel->setText(state ? "*" : " ");
}

void PythonConsole::languageChange()
{
	Ui::PythonConsole::retranslateUi(this);
	//setWindowTitle( tr("Script Console"));
	commandEdit->setToolTip( "<qt>" + tr("Write your commands here. A selection is processed as script") + "</qt>");
	outputEdit->setToolTip( "<qt>" + tr("Output of your script") + "</qt>");
}

void PythonConsole::slot_runScript()
{
	outputEdit->clear();
	parsePythonString();
	emit runCommand();
	commandEdit->textCursor().movePosition(QTextCursor::Start);
}

void PythonConsole::slot_runScriptAsConsole()
{
	parsePythonString();
	commandEdit->clear();
	// content is destroyed. This is to prevent overwriting
	filename = QString::null;
	outputEdit->append("\n>>> " + m_command);
	emit runCommand();
}

void PythonConsole::parsePythonString()
{
	if (commandEdit->textCursor().hasSelection())
		m_command = commandEdit->textCursor().selectedText();
	else
	{
		commandEdit->selectAll();
		m_command = commandEdit->textCursor().selectedText();
	}
	// prevent user's wrong selection
	m_command += '\n';
}

/*
 * supplementary slots. Saving etc.
 */
void PythonConsole::slot_open()
{
	filename = QFileDialog::getOpenFileName(this,
			tr("Open Python Script File"),
			".",
			tr("Python Scripts (*.py *.PY)"));
	if (filename.isNull())
		return;
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
		commandEdit->setPlainText(stream.readAll());
		file.close();
	}
}

void PythonConsole::slot_save()
{
	if (filename.isNull())
	{
		slot_saveAs();
		return;
	}
	QFile f(filename);
	if (f.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&f);
		stream << commandEdit->toPlainText();
		f.close();
	}
}

void PythonConsole::slot_saveAs()
{
	QString oldFname = filename;
	filename = QFileDialog::getSaveFileName(this,
			tr("Save the Python Commands in File"),
			".",
			tr("Python Scripts (*.py *.PY)"));
	if (filename.isNull())
		return;
	QFile f(filename);
	if (f.exists())
	{
		QString fn = QDir::convertSeparators(filename);
		if (ScMessageBox::warning(this, CommonStrings::trWarning,
			"<qt>" + tr(QString("File %1 already exists. Do you want to replace it?").arg(fn).toLocal8Bit().constData()) + "</qt>",
			QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
		{
			filename = oldFname;
			return;
		}
	}
	slot_save();
}

void PythonConsole::slot_saveOutput()
{
	QString fname = QFileDialog::getSaveFileName(this,
			tr("Save Current Output"),
			".",
			tr("Text Files (*.txt)"));
	if (fname == QString::null)
		return;
	QFile f(fname);
	if (!f.exists())
	{
		QString fn = QDir::convertSeparators(filename);
		if (QMessageBox::warning(this, CommonStrings::trWarning,
			"<qt>" + tr(QString("File %1 already exists. Do you want to replace it?").arg(fn).toLocal8Bit().constData()) + "</qt>",
			QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			return;
	}
	// save
	if (f.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&f);
		stream << outputEdit->toPlainText();
		f.close();
	}
}

void PythonConsole::slot_quit()
{
	emit paletteShown(false);
}

/*
 * Syntax highlighting
 */
SyntaxHighlighter::SyntaxHighlighter(QTextEdit *textEdit) : QSyntaxHighlighter(textEdit)
{
	// Reserved keywords in Python 2.4
	QStringList keywords;
	HighlightingRule rule;

	keywords << "and" << "assert" << "break" << "class" << "continue" << "def"
			 << "del" << "elif" << "else" << "except" << "exec" << "finally"
			 << "for" << "from" << "global" << "if" << "import" << "in"
			 << "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
			 << "return" << "try" << "while" << "yield";

	keywordFormat.setForeground(colors.keywordColor);
	keywordFormat.setFontWeight(QFont::Bold);
	singleLineCommentFormat.setForeground(colors.commentColor);
	singleLineCommentFormat.setFontItalic(true);
	quotationFormat.setForeground(colors.stringColor);
	numberFormat.setForeground(colors.numberColor);
	operatorFormat.setForeground(colors.signColor);

	foreach (QString kw, keywords)
	{
		rule.pattern = QRegExp("\\b" + kw + "\\b", Qt::CaseInsensitive);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	rule.pattern = QRegExp("#[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("\'.*\'");
	rule.pattern.setMinimal(true);
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("\".*\"");
	rule.pattern.setMinimal(true);
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("\\b\\d+\\b");
	rule.pattern.setMinimal(true);
	rule.format = numberFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("[\\\\|\\<|\\>|\\=|\\!|\\+|\\-|\\*|\\/|\\%]+");
	rule.pattern.setMinimal(true);
	rule.format = operatorFormat;
	highlightingRules.append(rule);
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
	foreach (HighlightingRule rule, highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = text.indexOf(expression);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
		}
	}
	setCurrentBlockState(0);

	// multiline strings handling
	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf("\"\"\"");

	while (startIndex >= 0)
	{
		int endIndex = text.indexOf("\"\"\"", startIndex);
		int commentLength;

		if (endIndex == -1)
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex + 3;//commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, quotationFormat);
		startIndex = text.indexOf("\"\"\"", startIndex + commentLength);
	}
}

SyntaxColors::SyntaxColors()
{
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("scriptplugin");
	errorColor.setNamedColor(prefs->get("syntaxerror", "#aa0000"));
	commentColor.setNamedColor(prefs->get("syntaxcomment", "#A0A0A0"));
	keywordColor.setNamedColor(prefs->get("syntaxkeyword", "#00007f"));
	signColor.setNamedColor(prefs->get("syntaxsign", "#aa00ff"));
	numberColor.setNamedColor(prefs->get("syntaxnumber", "#ffaa00"));
	stringColor.setNamedColor(prefs->get("syntaxstring", "#005500"));
	textColor.setNamedColor(prefs->get("syntaxtext", "#000000"));
}

SyntaxColors::~SyntaxColors()
{
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("scriptplugin");
	prefs->set("syntaxerror", qcolor2named(errorColor));
	prefs->set("syntaxcomment", qcolor2named(commentColor));
	prefs->set("syntaxkeyword", qcolor2named(keywordColor));
	prefs->set("syntaxsign", qcolor2named(signColor));
	prefs->set("syntaxnumber", qcolor2named(numberColor));
	prefs->set("syntaxstring", qcolor2named(stringColor));
	prefs->set("syntaxtext", qcolor2named(textColor));
}

QString SyntaxColors::qcolor2named(QColor color)
{
	int r, g, b;
	QString retval("#");
	QString oct;
	color.getRgb(&r, &g, &b);
	retval += oct.setNum(r, 16).rightJustified(2, '0');
	retval += oct.setNum(g, 16).rightJustified(2, '0');
	retval += oct.setNum(b, 16).rightJustified(2, '0');
	return retval;
}
