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

#include "langmgr.h"
#include "prefs_userinterface.h"
#include "prefsstructs.h"
#include "scribusdoc.h"
#include "util.h"


Prefs_UserInterface::Prefs_UserInterface(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	QStringList languageList;
	LanguageManager::instance()->fillInstalledStringList(&languageList, true);
	languageList.sort();
	languageComboBox->addItems( languageList );

	// qt styles
	QStringList styleList = QStyleFactory::keys();
	themeComboBox->addItem("");
	for (int i = 0; i < styleList.count(); ++i)
		themeComboBox->addItem(styleList[i]);

	connect(languageComboBox, SIGNAL(activated(const QString &)), this, SLOT(setSelectedGUILang(const QString &)));
	connect(storyEditorFontPushButton, SIGNAL(clicked()), this, SLOT(changeStoryEditorFont()));
	connect(storyEditorFontColorPushButton, SIGNAL(clicked()), this, SLOT(changeStoryEditorFontColor()));
}

Prefs_UserInterface::~Prefs_UserInterface()
{
}

void Prefs_UserInterface::languageChange()
{
	languageComboBox->setToolTip( "<qt>" + tr( "Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus" )+"</qt>");
	themeComboBox->setToolTip( "<qt>" + tr( "Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins." ) + "</qt>");
	useSmallWidgetsCheckBox->setToolTip( "<qt>" + tr( "Palette windows will use smaller (space savy) widgets. Requires application restart" ) + "</qt>");
	recentDocumentsSpinBox->setToolTip( "<qt>" + tr( "Number of recently edited documents to show in the File menu" ) + "</qt>");
	fontSizeMenuSpinBox->setToolTip( "<qt>" + tr( "Default font size for the menus and windows" ) + "</qt>");
	fontSizePaletteSpinBox->setToolTip( "<qt>" + tr( "Default font size for the tool windows" ) + "</qt>");
	resizeMoveDelaySpinBox->setToolTip( "<qt>" + tr( "" ) + "</qt>");
	wheelJumpSpinBox->setToolTip( "<qt>" + tr( "Number of lines Scribus will scroll for each move of the mouse wheel" ) + "</qt>");
	showSplashCheckBox->setToolTip( "<qt>" + tr( "" ) + "</qt>");
	showStartupDialogCheckBox->setToolTip( "<qt>" + tr( "" ) + "</qt>");

}

void Prefs_UserInterface::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	selectedGUILang = prefsData->uiPrefs.language;
	setCurrentComboItem(languageComboBox, LanguageManager::instance()->getLangFromAbbrev(selectedGUILang));
	setCurrentComboItem(themeComboBox, prefsData->uiPrefs.style);
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
	if (newColor.isValid())
	{
		QPixmap pm(100, 30);
		pm.fill(newColor);
		seFontColor = newColor;
		storyEditorFontColorPushButton->setIcon(pm);
	}
}

void Prefs_UserInterface::changeStoryEditorFont()
{
	bool ok;
	QFont newFont(QFontDialog::getFont( &ok, seFont, this ));
	if (ok)
	{
		seFont = newFont;
		storyEditorFontPushButton->setText(seFont.family());
	}
}
