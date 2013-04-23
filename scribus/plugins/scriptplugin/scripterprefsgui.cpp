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

	// Create SyntaxColors with save to prefs
	// disabled by defaults, only save when apply()
	syntaxColors = new SyntaxColors();
	
	languageChange();
	setupSyntaxColors();

	// Set the state of the ext script enable checkbox
	extensionScriptsChk->setChecked(scripterCore->extensionsEnabled());
	// The startup script box should be disabled  if ext scripts are off
	startupScriptEdit->setEnabled(extensionScriptsChk->isChecked());
	startupScriptEdit->setText(scripterCore->startupScript());

	// signals and slots connections
	connect(extensionScriptsChk, SIGNAL(toggled(bool)), startupScriptEdit, SLOT(setEnabled(bool)));
	// colors
	connect(textButton   , SIGNAL(clicked()), this, SLOT(setColor()));
	connect(commentButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(keywordButton, SIGNAL(clicked()), this, SLOT(setColor()));
	connect(errorButton  , SIGNAL(clicked()), this, SLOT(setColor()));
	connect(signButton   , SIGNAL(clicked()), this, SLOT(setColor()));
	connect(stringButton , SIGNAL(clicked()), this, SLOT(setColor()));
	connect(numberButton , SIGNAL(clicked()), this, SLOT(setColor()));
	connect(startupScriptChangeButton, SIGNAL(clicked()), this, SLOT(changeStartupScript()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
ScripterPrefsGui::~ScripterPrefsGui()
{
	delete syntaxColors;
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
	syntaxColors->saveToPrefs();

	// Necessary to update console syntax highlighter
	emit prefsChanged();
}

void ScripterPrefsGui::setColor()
{
	QPushButton* button = (QPushButton*) sender();

	QColor oldColor;
	if (button == textButton)    oldColor = syntaxColors->textColor;
	if (button == commentButton) oldColor = syntaxColors->commentColor;
 	if (button == keywordButton) oldColor = syntaxColors->keywordColor;
 	if (button == errorButton)   oldColor = syntaxColors->errorColor;
 	if (button == signButton)    oldColor = syntaxColors->signColor;
 	if (button == stringButton)  oldColor = syntaxColors->stringColor;
 	if (button == numberButton)  oldColor = syntaxColors->numberColor;

	QColor color = QColorDialog::getColor(oldColor, this);
	if (color.isValid())
	{
		setButtonIcon(button, color);

		if (button == textButton)    syntaxColors->textColor = color;
		if (button == commentButton) syntaxColors->commentColor = color;
 		if (button == keywordButton) syntaxColors->keywordColor = color;
 		if (button == errorButton)   syntaxColors->errorColor = color;
 		if (button == signButton)    syntaxColors->signColor = color;
 		if (button == stringButton)  syntaxColors->stringColor = color;
 		if (button == numberButton)  syntaxColors->numberColor = color;
	}
}

void ScripterPrefsGui::setButtonIcon(QPushButton* button, QColor color)
{
	QSize  iconSize   = button->iconSize();
	double iconWidth  = qMax(iconSize.width() , button->width() / 3);
	double iconHeight = qMin(iconSize.height(), button->height() / 3);
	QSize  newIconSize(iconWidth, iconHeight);
	if (iconSize != newIconSize)
		button->setIconSize(newIconSize);
	QPixmap icon(button->iconSize());
	icon.fill(color);
	button->setIcon(icon);
}

void ScripterPrefsGui::setupSyntaxColors()
{
	SyntaxColors syntax;
	setButtonIcon(textButton   , syntax.textColor);
	setButtonIcon(commentButton, syntax.commentColor);
 	setButtonIcon(keywordButton, syntax.keywordColor);
 	setButtonIcon(errorButton  , syntax.errorColor);
 	setButtonIcon(signButton   , syntax.signColor);
 	setButtonIcon(stringButton , syntax.stringColor);
 	setButtonIcon(numberButton , syntax.numberColor);
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
