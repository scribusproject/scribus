/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QColorDialog>
#include <QFontDialog>
#include <QPixmap>
#include <QStyleFactory>

#include "iconmanager.h"
#include "langmgr.h"
#include "prefs_userinterface.h"
#include "prefsstructs.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "util.h"

extern ScribusQApp *ScQApp;

Prefs_UserInterface::Prefs_UserInterface(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	QStringList languageList;
	LanguageManager::instance()->fillInstalledGUIStringList(&languageList);
	if (languageList.isEmpty())
	{
		QString currentGUILang = ScQApp->currGUILanguage();
		if (!currentGUILang.isEmpty())
			languageList << LanguageManager::instance()->getLangFromAbbrev(currentGUILang);
		else
			languageList << LanguageManager::instance()->getLangFromAbbrev("en_GB");
	}
	languageComboBox->addItems(languageList);

	// qt styles
	QStringList styleList = QStyleFactory::keys();
	themeComboBox->addItem("");
	themeComboBox->addItems(styleList);
	QStringList iconSetList;
	iconSetList=IconManager::instance()->nameList(ScQApp->currGUILanguage());
	iconSetComboBox->addItems(iconSetList);

	connect(languageComboBox, SIGNAL(activated(const QString &)), this, SLOT(setSelectedGUILang(const QString &)));
	connect(storyEditorFontPushButton, SIGNAL(clicked()), this, SLOT(changeStoryEditorFont()));
	connect(storyEditorFontColorPushButton, SIGNAL(clicked()), this, SLOT(changeStoryEditorFontColor()));
}

Prefs_UserInterface::~Prefs_UserInterface()
{
}

void Prefs_UserInterface::languageChange()
{
	languageComboBox->setToolTip( "<qt>" + tr( "Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus." )+"</qt>");
	themeComboBox->setToolTip( "<qt>" + tr( "Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins." ) + "</qt>");
	iconSetComboBox->setToolTip( "<qt>" + tr( "Choose the default icon set" ) + "</qt>");
	useSmallWidgetsCheckBox->setToolTip( "<qt>" + tr( "Palette windows will use smaller (space savy) widgets. Requires application restart." ) + "</qt>");
	recentDocumentsSpinBox->setToolTip( "<qt>" + tr( "Number of recently edited documents to show in the File menu" ) + "</qt>");
	fontSizeMenuSpinBox->setToolTip( "<qt>" + tr( "Default font size for the menus and windows" ) + "</qt>");
	fontSizePaletteSpinBox->setToolTip( "<qt>" + tr( "Default font size for the tool windows" ) + "</qt>");
	resizeMoveDelaySpinBox->setToolTip( "<qt>" + tr( "Time before resize or move starts allows for a slight delay between when you click and the operation happens to avoid unintended moves. This can be helpful when dealing with mouse sensitivity settings or accessibility issues related to ergonomic mice, touch pads or moveability of the wrists and hands." ) + "</qt>");
	wheelJumpSpinBox->setToolTip( "<qt>" + tr( "Number of lines Scribus will scroll for each \"notch\" of the mouse wheel" ) + "</qt>");
	//showSplashCheckBox->setToolTip( "<qt>" + tr( "" ) + "</qt>");
	//showStartupDialogCheckBox->setToolTip( "<qt>" + tr( "" ) + "</qt>");
	storyEditorUseSmartSelectionCheckBox->setToolTip( "<qt>" + tr( "The default behavior when double-clicking on a word is to select the word and the first following space. Smart selection will select only the word, without the following space." ) + "</qt>");
}

void Prefs_UserInterface::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	selectedGUILang = prefsData->uiPrefs.language;
	if (selectedGUILang.isEmpty())
		selectedGUILang = ScQApp->currGUILanguage();
	QString langString = LanguageManager::instance()->getLangFromAbbrev(selectedGUILang);
	if (languageComboBox->findText(langString) < 0)
	{
		selectedGUILang = ScQApp->currGUILanguage();
		langString = LanguageManager::instance()->getLangFromAbbrev(selectedGUILang);
	}
	if (languageComboBox->findText(langString) < 0)
	{
		selectedGUILang = "en_GB";
		langString = LanguageManager::instance()->getLangFromAbbrev(selectedGUILang);
	}
	setCurrentComboItem(languageComboBox, langString);
	setCurrentComboItem(themeComboBox, prefsData->uiPrefs.style);
	setCurrentComboItem(iconSetComboBox, prefsData->uiPrefs.iconSet);
	fontSizeMenuSpinBox->setValue( prefsData->uiPrefs.applicationFontSize );
	fontSizePaletteSpinBox->setValue( prefsData->uiPrefs.paletteFontSize);
	wheelJumpSpinBox->setValue( prefsData->uiPrefs.wheelJump );
	resizeMoveDelaySpinBox->setValue(prefsData->uiPrefs.mouseMoveTimeout);
	recentDocumentsSpinBox->setValue( prefsData->uiPrefs.recentDocCount );
	showStartupDialogCheckBox->setChecked(prefsData->uiPrefs.showStartupDialog);
	useTabsForDocumentsCheckBox->setChecked(prefsData->uiPrefs.useTabs);
	showSplashCheckBox->setChecked(prefsData->uiPrefs.showSplashOnStartup);
	useSmallWidgetsCheckBox->setChecked(prefsData->uiPrefs.useSmallWidgets);

	storyEditorUseSmartSelectionCheckBox->setChecked(prefsData->storyEditorPrefs.smartTextSelection);
	seFont.fromString(prefsData->storyEditorPrefs.guiFont);
	storyEditorFontPushButton->setText(seFont.family());
	QPixmap pm(100, 30);
	pm.fill(prefsData->storyEditorPrefs.guiFontColorBackground);
	seFontColor = prefsData->storyEditorPrefs.guiFontColorBackground;
	storyEditorFontColorPushButton->setIcon(pm);
}

void Prefs_UserInterface::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->uiPrefs.language=selectedGUILang;
	prefsData->uiPrefs.style=themeComboBox->currentText();
	prefsData->uiPrefs.iconSet=IconManager::instance()->baseNameForTranslation(iconSetComboBox->currentText());
	prefsData->uiPrefs.applicationFontSize=fontSizeMenuSpinBox->value();
	prefsData->uiPrefs.paletteFontSize=fontSizePaletteSpinBox->value();
	prefsData->uiPrefs.wheelJump=wheelJumpSpinBox->value();
	prefsData->uiPrefs.mouseMoveTimeout=resizeMoveDelaySpinBox->value();
	prefsData->uiPrefs.recentDocCount=recentDocumentsSpinBox->value();
	prefsData->uiPrefs.showStartupDialog=showStartupDialogCheckBox->isChecked();
	prefsData->uiPrefs.useTabs=useTabsForDocumentsCheckBox->isChecked();
	prefsData->uiPrefs.showSplashOnStartup=showSplashCheckBox->isChecked();
	prefsData->uiPrefs.useSmallWidgets=useSmallWidgetsCheckBox->isChecked();

	prefsData->storyEditorPrefs.guiFont=seFont.toString();
	prefsData->storyEditorPrefs.guiFontColorBackground=seFontColor;
	prefsData->storyEditorPrefs.smartTextSelection=storyEditorUseSmartSelectionCheckBox->isChecked();
}


void Prefs_UserInterface::setSelectedGUILang( const QString &newLang )
{
	selectedGUILang = LanguageManager::instance()->getAbbrevFromLang(newLang);
}


void Prefs_UserInterface::changeStoryEditorFontColor()
{
	QColor newColor(QColorDialog::getColor(seFontColor, this));
	if (!newColor.isValid())
		return;
	QPixmap pm(100, 30);
	pm.fill(newColor);
	seFontColor = newColor;
	storyEditorFontColorPushButton->setIcon(pm);
}


void Prefs_UserInterface::changeStoryEditorFont()
{
	bool ok;
	QFont newFont(QFontDialog::getFont( &ok, seFont, this ));
	if (!ok)
		return;
	seFont = newFont;
	storyEditorFontPushButton->setText(seFont.family());
}

