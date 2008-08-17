/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

// include the python header first because on OSX that uses the name "slots"
#include "cmdvar.h"

#include <QColorDialog>
#include <QFileDialog>

#include "scripterprefsgui.h"
#include "scriptercore.h"
#include "pconsole.h"


ScripterPrefsGui::ScripterPrefsGui(QWidget* parent )
	: PrefsPanel(parent)
{
	setupUi(this);
	
	languageChange();
	setupSyntaxColors();

	// Set the state of the ext script enable checkbox
	extensionScriptsChk->setChecked(scripterCore->extensionsEnabled());
	// The startup script box should be disabled  if ext scripts are off
	startupScriptEdit->setEnabled(extensionScriptsChk->isChecked());
	connect(extensionScriptsChk, SIGNAL(toggled(bool)),
			startupScriptEdit, SLOT(setEnabled(bool)));

	// signals and slots connections
	connect(extensionScriptsChk, SIGNAL(toggled(bool)), startupScriptEdit, SLOT(setEnabled(bool)));
	// colors
	connect(textButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(commentButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(keywordButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(errorButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(signButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(stringButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(numberButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(startupScriptChangeButton, SIGNAL(clicked()), this, SLOT(changeStartupScript()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
ScripterPrefsGui::~ScripterPrefsGui()
{
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ScripterPrefsGui::languageChange()
{
	setWindowTitle( tr("Scripter Preferences"));
/*	extensionScriptsChk->setText( tr("Enable Extension Scripts"));
	startupScriptEditLabel->setText( tr("Startup Script:"));
	errorLabel->setText( tr("Errors:", "syntax highlighting"));
	commentLabel->setText( tr("Comments:", "syntax highlighting"));
	keywordLabel->setText( tr("Keywords:", "syntax highlighting"));
	signLabel->setText( tr("Signs:", "syntax highlighting"));
	numberLabel->setText( tr("Numbers:", "syntax highlighting"));
	stringLabel->setText( tr("Strings:", "syntax highlighting"));
	textLabel->setText( tr("Base Texts:", "syntax highlighting"));*/
}

// Apply changes to prefs. Auto connected.
void ScripterPrefsGui::apply()
{
	scripterCore->setExtensionsEnabled(extensionScriptsChk->isChecked());
	scripterCore->setStartupScript(startupScriptEdit->text());
	// colors
	SyntaxColors *syntax = new SyntaxColors();
	syntax->textColor = textButton->palette().color(QPalette::Window);
	syntax->commentColor = commentButton->palette().color(QPalette::Window);
	syntax->keywordColor = keywordButton->palette().color(QPalette::Window);
	syntax->errorColor = errorButton->palette().color(QPalette::Window);
	syntax->signColor = signButton->palette().color(QPalette::Window);
	syntax->stringColor = stringButton->palette().color(QPalette::Window);
	syntax->numberColor = numberButton->palette().color(QPalette::Window);
	delete(syntax);
}

void ScripterPrefsGui::setColor()
{
	QPalette palette;
	QPushButton* button = (QPushButton*)sender();
	QColor color = QColorDialog::getColor(button->palette().color(QPalette::Window), this);
	if (color.isValid())
	{
		palette.setColor(button->backgroundRole(), color);
 		button->setPalette(palette);
	}
}

void ScripterPrefsGui::setupSyntaxColors()
{
	QPalette palette;
	SyntaxColors *syntax = new SyntaxColors();
	palette.setColor(textButton->backgroundRole(), syntax->textColor);
 	textButton->setPalette(palette);
	palette.setColor(commentButton->backgroundRole(), syntax->commentColor);
 	commentButton->setPalette(palette);
	palette.setColor(keywordButton->backgroundRole(), syntax->keywordColor);
 	keywordButton->setPalette(palette);
	palette.setColor(errorButton->backgroundRole(), syntax->errorColor);
 	errorButton->setPalette(palette);
	palette.setColor(signButton->backgroundRole(), syntax->signColor);
 	signButton->setPalette(palette);
	palette.setColor(stringButton->backgroundRole(), syntax->stringColor);
 	stringButton->setPalette(palette);
	palette.setColor(numberButton->backgroundRole(), syntax->numberColor);
 	numberButton->setPalette(palette);
	delete(syntax);
}

void ScripterPrefsGui::changeStartupScript()
{
	QString currentScript=startupScriptEdit->text();
	QFileInfo fi(startupScriptEdit->text());
	if (!fi.exists())
		currentScript = QDir::homePath();

	QString s = QFileDialog::getOpenFileName(this, tr("Locate Startup Script"), currentScript, "Python Scripts (*.py *.PY)");
	if (!s.isEmpty())
		startupScriptEdit->setText(s);
}
