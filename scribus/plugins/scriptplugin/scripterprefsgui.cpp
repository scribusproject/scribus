/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

// include the python header first because on OSX that uses the name "slots"
#include <Python.h>

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
	setCaption( tr("Scripter Preferences"));
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
	syntax->textColor = textButton->paletteBackgroundColor();
	syntax->commentColor = commentButton->paletteBackgroundColor();
	syntax->keywordColor = keywordButton->paletteBackgroundColor();
	syntax->errorColor = errorButton->paletteBackgroundColor();
	syntax->signColor = signButton->paletteBackgroundColor();
	syntax->stringColor = stringButton->paletteBackgroundColor();
	syntax->numberColor = numberButton->paletteBackgroundColor();
	delete(syntax);
}

void ScripterPrefsGui::setColor()
{
	QPushButton* button = (QPushButton*)sender();
	QColor color = QColorDialog::getColor(button->paletteBackgroundColor(), this, tr("Select Color"));
	if (color.isValid())
		button->setPaletteBackgroundColor(color);
}

void ScripterPrefsGui::setupSyntaxColors()
{
	SyntaxColors *syntax = new SyntaxColors();
	textButton->setPaletteBackgroundColor(syntax->textColor);
	commentButton->setPaletteBackgroundColor(syntax->commentColor);
	keywordButton->setPaletteBackgroundColor(syntax->keywordColor);
	errorButton->setPaletteBackgroundColor(syntax->errorColor);
	signButton->setPaletteBackgroundColor(syntax->signColor);
	stringButton->setPaletteBackgroundColor(syntax->stringColor);
	numberButton->setPaletteBackgroundColor(syntax->numberColor);
	delete(syntax);
}

void ScripterPrefsGui::changeStartupScript()
{
	QString currentScript=startupScriptEdit->text();
	QFileInfo fi(startupScriptEdit->text());
	if (!fi.exists())
		currentScript = QDir::homePath();

	QString s = QFileDialog::getOpenFileName(currentScript, "Python Scripts (*.py *.PY)", this, "d", tr("Locate Startup Script"));
	if (!s.isEmpty())
		startupScriptEdit->setText(s);
}
