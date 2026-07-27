/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDir>

#include "commonstrings.h"
#include "firststartwizard.h"
#include "fsw_appearance.h"
#include "fsw_experimental.h"
#include "fsw_finish.h"
#include "fsw_fontsscripts.h"
#include "fsw_language.h"
#include "fsw_newdocument.h"
#include "fsw_sidepanel.h"
#include "fsw_welcome.h"
#include "manager/pagepreset_manager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefsstructs.h"
#include "prefstable.h"
#include "scfonts.h"

FirstStartWizard::FirstStartWizard(ApplicationPrefs* prefsData, QWidget* parent)
	: QWizard(parent),
	  m_prefsData(prefsData)
{
	setObjectName(QString::fromUtf8("FirstStartWizard"));
	setWizardStyle(QWizard::ModernStyle);
	setWindowTitle(tr("Welcome to Scribus"));
	setMinimumSize(720, 520);

	setOption(QWizard::NoBackButtonOnStartPage, true);
	setOption(QWizard::NoCancelButton, true);
	setOption(QWizard::HaveCustomButton1, true);
	setButtonText(QWizard::CustomButton1, tr("Skip setup"));
	setButtonLayout({ QWizard::CustomButton1, QWizard::Stretch,
					  QWizard::BackButton, QWizard::NextButton, QWizard::FinishButton });

	// Slim branded side panel (logo + step list). The full splash banner lives in
	// the body of the Welcome and Finish pages, not here.
	m_side = new FSW_SidePanel(this);
	setSideWidget(m_side);

	m_welcome      = new FSW_Welcome(this);
	m_language     = new FSW_Language(this);
	m_appearance   = new FSW_Appearance(this);
	m_newDocument  = new FSW_NewDocument(this);
	m_fontsScripts = new FSW_FontsScripts(this);
	m_experimental = new FSW_Experimental(this);
	m_finish       = new FSW_Finish(this);

	setPage(Page_Welcome,      m_welcome);
	setPage(Page_Language,     m_language);
	setPage(Page_Appearance,   m_appearance);
	setPage(Page_NewDocument,  m_newDocument);
	setPage(Page_FontsScripts, m_fontsScripts);
	setPage(Page_Experimental, m_experimental);
	setPage(Page_Finish,       m_finish);

	connect(this, &QWizard::customButtonClicked, this, [this](int which) {
		if (which == QWizard::CustomButton1)
			onSkip();
	});
	connect(this, &QWizard::currentIdChanged, this, &FirstStartWizard::onPageChanged);
	connect(m_appearance, &FSW_Appearance::themeModeChanged, this, &FirstStartWizard::onThemeModeChanged);

	// Seed every page from the caller's prefs, so the wizard opens showing the
	// values that are actually in effect rather than its own hardcoded defaults.
	restoreDefaults(m_prefsData);
}

void FirstStartWizard::restoreDefaults(const ApplicationPrefs* prefsData)
{
	if (!prefsData)
		return;

	m_language->restoreDefaults(prefsData);
	m_appearance->restoreDefaults(prefsData);
	m_newDocument->restoreDefaults(prefsData);
	m_fontsScripts->restoreDefaults(prefsData);
	// Snapshot the font folders as seeded: accept() diffs against this to find the
	// ones the user added. It has to be taken here or there is
	// nothing to compare against.
	m_seededFontDirs = m_fontsScripts->fontPaths();
	m_experimental->restoreDefaults(prefsData);

	// Sync the banner art to the seeded theme: setChecked() emits nothing when the
	// radio was already the checked one, so the wizard would otherwise keep the
	// splash it started with.
	onThemeModeChanged(m_appearance->themeMode());
}

void FirstStartWizard::onPageChanged(int id)
{
	if (m_side)
		m_side->setCurrentStep(id);
}

void FirstStartWizard::onThemeModeChanged(int mode)
{
	// Live-preview the splash on the banner pages as the user picks a theme (0 light,
	// 1 dark, 2 automatic). Automatic resolves via IconManager's iconsForDarkMode().
	m_welcome->setThemeMode(mode);
	m_finish->setThemeMode(mode);
}

void FirstStartWizard::accept()
{
	saveGuiToPrefs(m_prefsData);

	rescanAddedFontDirs(m_seededFontDirs);

	QWizard::accept();
}

void FirstStartWizard::rescanAddedFontDirs(const QStringList& before)
{
	// initDefaults() already scanned fonts and picked the default from that list, so a
	// folder added here cannot make one of its fonts the default until next launch. It
	// can, make those fonts available this session -- scan the newly added
	// folders in, as Prefs_Fonts::AddPath does.
	//
	// Scan into the working copy as they will be committed via this struct with
	// setNewPrefs() straight after exec() returns.

	if (!m_prefsData)
		return;

	const QStringList after = m_fontsScripts->fontPaths();
	if (after == before)
		return;

	SCFonts& availFonts = m_prefsData->fontPrefs.AvailFonts;
	bool changed = false;
	for (const QString& nativeDir : after)
	{
		const QString dir = QDir::fromNativeSeparators(nativeDir);
		if (before.contains(nativeDir) || dir.isEmpty())
			continue;
		availFonts.addScalableFonts(dir + "/");
		changed = true;
	}
	if (changed)
	{
		availFonts.updateFontMap();
		availFonts.writeFontCache();
	}
}

void FirstStartWizard::onSkip()
{
	// Nothing is read from the pages: rejecting leaves the caller's prefs untouched.
	// Recording that setup ran is the caller's job (ScribusCore::startGUI).
	QWizard::reject();
}

void FirstStartWizard::saveGuiToPrefs(ApplicationPrefs* prefsData) const
{
	if (!prefsData)
		return;
	ApplicationPrefs& p = *prefsData;

	// --- Language (FSW_Language) ---
	p.uiPrefs.language           = m_language->uiLanguage();        // priAbbrev
	p.docSetupPrefs.language     = m_language->documentLanguage();  // normalised short abbrev
	p.docSetupPrefs.docUnitIndex = m_language->unitIndex();

	// --- Appearance (FSW_Appearance) ---
	switch (m_appearance->themeMode())
	{
		case 0:
			p.uiPrefs.stylePalette = QString::fromUtf8("light");
			break;
		case 1:
			p.uiPrefs.stylePalette = QString::fromUtf8("dark");
			break;
		default:
			p.uiPrefs.stylePalette = QString::fromUtf8("auto");
			break;
	}

	// --- New document defaults (FSW_NewDocument) ---
	p.docSetupPrefs.pageSize = m_newDocument->pageSizeName();
	// Keep the page dimensions in step with the chosen size id. initDefaults() and the
	// Document Setup pane both set width/height alongside pageSize, and the New Document
	// dialog seeds from those dimensions, so writing only the id would leave the dialog
	// showing the old (default) size.
	PageSizeInfo psi = PagePresetManager::instance().pageInfoByName(p.docSetupPrefs.pageSize);
	if (!psi.id.isEmpty() && psi.id != CommonStrings::customPageSize)
	{
		p.docSetupPrefs.pageWidth  = psi.width;
		p.docSetupPrefs.pageHeight = psi.height;
	}
	p.docSetupPrefs.isRTL    = m_newDocument->isRTL();

	// --- Scripts (FSW_FontsScripts) ---
	// pathPrefs.scripts is a single directory, matching the Paths preferences pane.
	// Written unconditionally so clearing the folder clears the preference; the line
	// edit shows native separators, so convert back on the way in.
	p.pathPrefs.scripts = QDir::fromNativeSeparators(m_fontsScripts->scriptPath());

	// --- Additional font folders (FSW_FontsScripts) ---
	// Font search paths are NOT an appPrefs field; they live in the PrefsFile "Fonts"
	// context, table "ExtraFontDirs". The list was seeded from that table, so write it
	// back as a REPLACE (clear + set), mirroring Prefs_Fonts::writePaths() -- appending
	// would duplicate every pre-existing folder.
	PrefsContext* fontCtx = PrefsManager::instance().prefsFile->getContext("Fonts");
	PrefsTable* fontTable = fontCtx->getTable("ExtraFontDirs");
	fontTable->clear();
	const QStringList fontDirs = m_fontsScripts->fontPaths();
	for (int i = 0; i < fontDirs.size(); ++i)
		fontTable->set(i, 0, QDir::fromNativeSeparators(fontDirs.at(i)));

	// --- Experimental (FSW_Experimental) ---
	// The only experimental feature today is "Enable Notes", so the master toggle is there.
	p.experimentalFeaturePrefs.notesEnabled = m_experimental->experimentalEnabled();
}

bool FirstStartWizard::isFirstRun(const ApplicationPrefs& prefsData)
{
	// A fresh profile has no prefs file, so initDefaults() leaves showFirstStartWizard
	// true; an existing profile read it (defaulting to "0" when the attribute is
	// absent).
	return prefsData.uiPrefs.showFirstStartWizard;
}
