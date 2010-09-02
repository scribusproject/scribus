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
#include <QPalette>

#include "prefs_scripter.h"
#include "scriptercore.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "prefsstructs.h"

Prefs_Scripter::Prefs_Scripter(QWidget* parent)
	: Prefs_Pane(parent)
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

Prefs_Scripter::~Prefs_Scripter()
{
}

void Prefs_Scripter::languageChange()
{
}

void Prefs_Scripter::restoreDefaults(struct ApplicationPrefs *prefsData)
{
}

void Prefs_Scripter::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
}

// Apply changes to prefs. Auto connected.
void Prefs_Scripter::apply()
{
	scripterCore->setExtensionsEnabled(extensionScriptsChk->isChecked());
	scripterCore->setStartupScript(startupScriptEdit->text());
	// colors
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("scriptplugin");
	if (prefs)
	{
		prefs->set("syntaxerror", errorColor.name());
		prefs->set("syntaxcomment", commentColor.name());
		prefs->set("syntaxkeyword", keywordColor.name());
		prefs->set("syntaxsign", signColor.name());
		prefs->set("syntaxnumber", numberColor.name());
		prefs->set("syntaxstring", stringColor.name());
		prefs->set("syntaxtext", textColor.name());
	}
}

void Prefs_Scripter::setColor()
{
	QPushButton* button = (QPushButton*)sender();
	QColor color = QColorDialog::getColor(button->palette().color(QPalette::Window), this);
	if (color.isValid() && button)
	{
		QPixmap pm(54, 14);
		pm.fill(color);
		button->setIcon(pm);
		if (button==textButton)
			textColor=color;
		if (button==commentButton)
			commentColor=color;
		if (button==keywordButton)
			keywordColor=color;
		if (button==errorButton)
			errorColor=color;
		if (button==signButton)
			signColor=color;
		if (button==stringButton)
			stringColor=color;
		if (button==numberButton)
			numberColor=color;
	}
}

void Prefs_Scripter::setupSyntaxColors()
{
	SyntaxColors *syntax = new SyntaxColors();
	textColor=syntax->textColor;
	commentColor=syntax->commentColor;
	keywordColor=syntax->keywordColor;
	errorColor=syntax->errorColor;
	signColor=syntax->signColor;
	stringColor=syntax->stringColor;
	numberColor=syntax->numberColor;

	QPixmap pm(54, 14);
	pm.fill(textColor);
	textButton->setIcon(pm);
	pm.fill(commentColor);
	commentButton->setIcon(pm);
	pm.fill(keywordColor);
	keywordButton->setIcon(pm);
	pm.fill(errorColor);
	errorButton->setIcon(pm);
	pm.fill(signColor);
	signButton->setIcon(pm);
	pm.fill(stringColor);
	stringButton->setIcon(pm);
	pm.fill(numberColor);
	numberButton->setIcon(pm);

	delete(syntax);
}

void Prefs_Scripter::changeStartupScript()
{
	QString currentScript=startupScriptEdit->text();
	QFileInfo fi(startupScriptEdit->text());
	if (!fi.exists())
		currentScript = QDir::homePath();

	QString s = QFileDialog::getOpenFileName(this, tr("Locate Startup Script"), currentScript, "Python Scripts (*.py *.PY)");
	if (!s.isEmpty())
		startupScriptEdit->setText(s);
}

