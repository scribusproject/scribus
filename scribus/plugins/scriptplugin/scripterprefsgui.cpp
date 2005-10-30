#include "scripterprefsgui.h"
#include "scriptercore.h"
#include "pconsole.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <prefspanel.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qcolordialog.h>


ScripterPrefsGui::ScripterPrefsGui(QWidget* parent )
	: PrefsPanel(parent, "ScripterPrefsGui")
{
	ScripterPrefsGuiBaseLayout = new QGridLayout(this, 1, 1, 10, 5, "ScripterPrefsGuiBaseLayout");

	tabWidget = new QTabWidget(this, "tabWidget");
	//tabWidget->setGeometry(QRect(70, 50, 380, 230));

	startupTab = new QWidget(tabWidget, "startupTab");
	tabWidget->insertTab(startupTab, tr("Extensions"));
	consoleTab = new QWidget(tabWidget, "consoleTab");
	tabWidget->insertTab(consoleTab, tr("Console"));

	extLayout = new QVBoxLayout(startupTab, 10, 5, "extLayout");
	extLayout->setAlignment(Qt::AlignTop);
	extensionScriptsChk = new QCheckBox(startupTab, "extensionScriptsChk");
	extLayout->addWidget(extensionScriptsChk);

	startupScriptLayout = new QHBoxLayout(startupTab, 10, 5, "startupScriptLayout");

	startupScriptEditLabel = new QLabel(startupTab, "startupScriptEditLabel");
	startupScriptLayout->addWidget(startupScriptEditLabel);

	startupScriptEdit = new QLineEdit(startupTab, "startupScriptEdit");
	startupScriptLayout->addWidget(startupScriptEdit);
	extLayout->addLayout(startupScriptLayout);
	extScriptSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
	extLayout->addItem(extScriptSpacer);

	// colors
	colorLayout = new QGridLayout(consoleTab, 1, 1, 10, 5);

	errorLabel = new QLabel(consoleTab);
	commentLabel = new QLabel(consoleTab);
	keywordLabel = new QLabel(consoleTab);
	signLabel = new QLabel(consoleTab);
	numberLabel = new QLabel(consoleTab);
	stringLabel = new QLabel(consoleTab);
	textLabel = new QLabel(consoleTab);
	errorButton = new QPushButton(consoleTab);
	commentButton = new QPushButton(consoleTab);
	keywordButton = new QPushButton(consoleTab);
	signButton = new QPushButton(consoleTab);
	numberButton = new QPushButton(consoleTab);
	stringButton = new QPushButton(consoleTab);
	textButton = new QPushButton(consoleTab);

	colorLayout->addWidget(textLabel, 0, 0);
	colorLayout->addWidget(textButton, 0, 1);
	colorLayout->addWidget(commentLabel, 1, 0);
	colorLayout->addWidget(commentButton, 1, 1);
	colorLayout->addWidget(keywordLabel, 2, 0);
	colorLayout->addWidget(keywordButton, 2, 1);
	colorLayout->addWidget(signLabel, 3, 0);
	colorLayout->addWidget(signButton, 3, 1);
	colorLayout->addWidget(errorLabel, 4, 0);
	colorLayout->addWidget(errorButton, 4, 1);
	colorLayout->addWidget(stringLabel, 5, 0);
	colorLayout->addWidget(stringButton, 5, 1);
	colorLayout->addWidget(numberLabel, 6, 0);
	colorLayout->addWidget(numberButton, 6, 1);

	ScripterPrefsGuiBaseLayout->addWidget(tabWidget, 0, 0);
	languageChange();
	setupSyntaxColors();
	resize(minimumSizeHint());
	clearWState(WState_Polished);

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
}

/*
 *  Destroys the object and frees any allocated resources
 */
ScripterPrefsGui::~ScripterPrefsGui()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ScripterPrefsGui::languageChange()
{
	setCaption(tr("Scripter Preferences"));
	extensionScriptsChk->setText(tr("Enable Extension Scripts"));
	startupScriptEditLabel->setText(tr("Startup Script:"));
	errorLabel->setText(tr("Errors:", "syntax highlighting"));
	commentLabel->setText(tr("Comments:", "syntax highlighting"));
	keywordLabel->setText(tr("Keywords:", "syntax highlighting"));
	signLabel->setText(tr("Signs:", "syntax highlighting"));
	numberLabel->setText(tr("Numbers:", "syntax highlighting"));
	stringLabel->setText(tr("Strings:", "syntax highlighting"));
	textLabel->setText(tr("Base Texts:", "syntax highlighting"));
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

#include "scripterprefsgui.moc"
